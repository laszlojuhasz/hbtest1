/*
 * $Id$
 */

/*
 * Harbour Project source code:
 * DBFCDX RDD
 *
 * Copyright 1999 Bruno Cantero <bruno@issnet.net>
 * www - http://www.harbour-project.org
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version, with one exception:
 *
 * The exception is that if you link the Harbour Runtime Library (HRL)
 * and/or the Harbour Virtual Machine (HVM) with other files to produce
 * an executable, this does not by itself cause the resulting executable
 * to be covered by the GNU General Public License. Your use of that
 * executable is in no way restricted on account of linking the HRL
 * and/or HVM code into it.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA (or visit
 * their web site at http://www.gnu.org/).
 *
 */

#define SUPERTABLE ( &cdxSuper )

#include <time.h>
#include "hbapi.h"
#include "hbinit.h"
#include "hbapiitm.h"
#include "hbapirdd.h"
#include "rddsys.ch"
#include "hbapierr.h"
#include "hbapilng.h"


typedef struct _DBFHEADER
{
   BYTE   bVersion;
   BYTE   bYear;
   BYTE   bMonth;
   BYTE   bDay;
   ULONG  ulRecords;
   USHORT uiHeaderLen;
   USHORT uiRecordLen;
   BYTE   bReserved1[ 16 ];
   BYTE   bHasTag;
   BYTE   bReserved2[ 3 ];
} DBFHEADER;

typedef DBFHEADER * LPDBFHEADER;


typedef struct _DBFFIELD
{
   BYTE bName[ 11 ];
   BYTE bType;
   BYTE bReserved1[ 4 ];
   BYTE bLen;
   BYTE bDec;
   BYTE bReserved2[ 13 ];
   BYTE bHasTag;
} DBFFIELD;

typedef DBFFIELD * LPDBFFIELD;


typedef struct
{
   ULONG  lNextBlock;
   ULONG  lBlockSize;
} MEMOHEADER;

typedef MEMOHEADER * LPMEMOHEADER;


typedef struct _DBFMEMO
{
   BOOL   fChanged;             /* Memo status */
   BYTE * pData;                /* Memo data */
   USHORT uiLen;                /* Len of data */
} DBFMEMO;

typedef DBFMEMO * LPDBFMEMO;


HB_FUNC( _DBFCDX );
HB_FUNC( DBFCDX_GETFUNCTABLE );

HB_INIT_SYMBOLS_BEGIN( dbfcdx1__InitSymbols )
{ "_DBFCDX",             HB_FS_PUBLIC, HB_FUNCNAME( _DBFCDX ), NULL },
{ "DBFCDX_GETFUNCTABLE", HB_FS_PUBLIC, HB_FUNCNAME( DBFCDX_GETFUNCTABLE ), NULL }
HB_INIT_SYMBOLS_END( dbfcdx1__InitSymbols )
#if defined(_MSC_VER)
   #if _MSC_VER >= 1010
      #pragma data_seg( ".CRT$XIY" )
      #pragma comment( linker, "/Merge:.CRT=.data" )
   #else
      #pragma data_seg( "XIY" )
   #endif
   #pragma warning( disable: 4152 )
   static void *hb_vm_auto_dbfcdx1__InitSymbols = &dbfcdx1__InitSymbols;
   #pragma warning( default: 4152 )
   #pragma data_seg()

#else

   #if ! defined(__GNUC__)
      #pragma startup dbfcdx1__InitSymbols
   #endif
#endif

#define LOCK_START                          0x40000000L
#define LOCK_APPEND                         0x7FFFFFFEL
#define LOCK_FILE                           0x3FFFFFFFL
#define MEMO_BLOCK                                   64
#define CDX_MAX_KEY                                 240

static RDDFUNCS cdxSuper = { 0 };

static BOOL hb_nltoa( LONG lValue, char * szBuffer, USHORT uiLen )
{
   LONG lAbsNumber;
   int iCount, iPos;

   HB_TRACE(HB_TR_DEBUG, ("hb_nltoa(%ld, %p, %hu)", lValue, szBuffer, uiLen));

   lAbsNumber = ( lValue > 0 ) ? lValue : - lValue;
   iCount = iPos = uiLen;
   while( iCount-- > 0 )
   {
      szBuffer[ iCount ] = ( '0' + lAbsNumber % 10 );
      lAbsNumber /= 10;
   }

   if( lAbsNumber > 0 )
   {
      memset( szBuffer, ' ', uiLen );
      return FALSE;
   }

   uiLen--;
   for( iCount = 0; iCount < uiLen; iCount++ )
      if( szBuffer[ iCount ] == '0' )
         szBuffer[ iCount ] = ' ';
      else
         break;

   if( lValue < 0 )
   {
      if( szBuffer[ 0 ] != ' ' )
      {
         memset( szBuffer, ' ', iPos );
         return FALSE;
      }
      for( iCount = uiLen; iCount >= 0; iCount-- )
      {
         if( szBuffer[ iCount ] == ' ' )
         {
            szBuffer[ iCount ] = '-';
            break;
         }
      }
   }
   return TRUE;
}

static ULONG hb_cdxSwapBytes( ULONG ulValue )
{
   BYTE * pValue, pByte;

   HB_TRACE(HB_TR_DEBUG, ("hb_cdxSwapBytes(%lu)", ulValue));

   pValue = ( BYTE * ) &ulValue;
   pByte = pValue[ 0 ];
   pValue[ 0 ] = pValue[ 3 ];
   pValue[ 3 ] = pByte;
   pByte = pValue[ 1 ];
   pValue[ 1 ] = pValue[ 2 ];
   pValue[ 2 ] = pByte;
   return ulValue;
}

static void hb_cdxReadMemo( AREAP pArea, LPDBFMEMO pMemo, ULONG lMemoBlock )
{
   ULONG ulSpaceUsed;
   MEMOHEADER pMemoHeader;

   HB_TRACE(HB_TR_DEBUG, ("hb_cdxReadMemo(%p, %p, %lu)", pArea, pMemo, lMemoBlock));

   hb_fsSeek( pArea->lpDataInfo->pNext->hFile, lMemoBlock * MEMO_BLOCK, FS_SET );
   hb_fsRead( pArea->lpDataInfo->pNext->hFile, ( BYTE * ) &pMemoHeader,
              sizeof( MEMOHEADER ) );
   ulSpaceUsed = hb_cdxSwapBytes( pMemoHeader.lBlockSize );
   if( pMemo->uiLen != ulSpaceUsed )
   {
      if( pMemo->uiLen > 0 )
         pMemo->pData = ( BYTE * ) hb_xrealloc( pMemo->pData, ulSpaceUsed + 1 );
      else
         pMemo->pData = ( BYTE * ) hb_xgrab( ulSpaceUsed + 1 );
      pMemo->uiLen = ( USHORT ) ulSpaceUsed;
   }
   hb_fsRead( pArea->lpDataInfo->pNext->hFile, pMemo->pData, pMemo->uiLen );
}

static BOOL hb_cdxWriteMemo( AREAP pArea, LPDBFMEMO pMemo, ULONG * lNewRecNo )
{
   USHORT uiNumBlocks;
   MEMOHEADER pMemoHeader;
   BYTE * pBuffer;

   HB_TRACE(HB_TR_DEBUG, ("hb_cdxWriteMemo(%p, %p, %p)", pArea, pMemo, lNewRecNo));

   if( !pArea->lpExtendInfo->fExclusive && !pArea->lpDataInfo->fFileLocked &&
       !hb_fsLock( pArea->lpDataInfo->pNext->hFile, LOCK_APPEND - 1, 1, FL_LOCK ) )
      return FALSE;

   uiNumBlocks = 1 + ( pMemo->uiLen + sizeof( MEMOHEADER ) ) / MEMO_BLOCK;
   if( * lNewRecNo > 0 )
   {
      hb_fsSeek( pArea->lpDataInfo->pNext->hFile, * lNewRecNo * MEMO_BLOCK, FS_SET );
      hb_fsRead( pArea->lpDataInfo->pNext->hFile, ( BYTE * ) &pMemoHeader,
                 sizeof( MEMOHEADER ) );
      if( pMemo->uiLen > hb_cdxSwapBytes( pMemoHeader.lBlockSize ) )
         * lNewRecNo = 0;                    /* Not room for data */
   }

   if( * lNewRecNo == 0 )                    /* Add an entry at eof */
   {
      hb_fsSeek( pArea->lpDataInfo->pNext->hFile, 0, FS_SET );
      hb_fsRead( pArea->lpDataInfo->pNext->hFile, ( BYTE * ) &pMemoHeader,
                 sizeof( MEMOHEADER ) );
      * lNewRecNo = hb_cdxSwapBytes( pMemoHeader.lNextBlock );
      pMemoHeader.lNextBlock = hb_cdxSwapBytes( * lNewRecNo + uiNumBlocks );
      hb_fsSeek( pArea->lpDataInfo->pNext->hFile, 0, FS_SET );
      hb_fsWrite( pArea->lpDataInfo->pNext->hFile, ( BYTE * ) &pMemoHeader,
                  sizeof( MEMOHEADER ) );
   }

   hb_fsSeek( pArea->lpDataInfo->pNext->hFile, * lNewRecNo * MEMO_BLOCK, FS_SET );
   pMemoHeader.lNextBlock = hb_cdxSwapBytes( 1 );
   pMemoHeader.lBlockSize = hb_cdxSwapBytes( pMemo->uiLen );
   hb_fsWrite( pArea->lpDataInfo->pNext->hFile, ( BYTE * ) &pMemoHeader,
               sizeof( MEMOHEADER ) );
   if( hb_fsWrite( pArea->lpDataInfo->pNext->hFile, pMemo->pData,
                  pMemo->uiLen ) != pMemo->uiLen )
   {
      if( !pArea->lpExtendInfo->fExclusive && !pArea->lpDataInfo->fFileLocked )
         hb_fsLock( pArea->lpDataInfo->pNext->hFile, LOCK_APPEND - 1, 1, FL_UNLOCK );
      return FALSE;
   }
   uiNumBlocks = ( pMemo->uiLen + sizeof( MEMOHEADER ) ) % MEMO_BLOCK;
   if( uiNumBlocks > 0 )
   {
      pBuffer = ( BYTE * ) hb_xgrab( MEMO_BLOCK );
      memset( pBuffer, 0, MEMO_BLOCK );
      hb_fsWrite( pArea->lpDataInfo->pNext->hFile, pBuffer, MEMO_BLOCK - uiNumBlocks );
      hb_xfree( pBuffer);
   }

   if( !pArea->lpExtendInfo->fExclusive && !pArea->lpDataInfo->fFileLocked )
      hb_fsLock( pArea->lpDataInfo->pNext->hFile, LOCK_APPEND - 1, 1, FL_UNLOCK );
   return TRUE;
}


/*
 * -- CDX METHODS --
 */

#define cdxBof                                  NULL
#define cdxEof                                  NULL
#define cdxFound                                NULL
#define cdxGoBottom                             NULL
#define cdxGoTo                                 NULL
#define cdxGoToId                               NULL
#define cdxGoTop                                NULL
#define cdxSkip                                 NULL
#define cdxSkipFilter                           NULL
#define cdxSkipRaw                              NULL
#define cdxAddField                             NULL
#define cdxAppend                               NULL
#define cdxCreateFields                         NULL
#define cdxDeleteRec                            NULL
#define cdxDeleted                              NULL
#define cdxFieldCount                           NULL
#define cdxFieldDisplay                         NULL
#define cdxFieldInfo                            NULL
#define cdxFieldName                            NULL
#define cdxFlush                                NULL
#define cdxGetRec                               NULL
#define cdxGetValue                             NULL
#define cdxGetVarLen                            NULL
#define cdxGoCold                               NULL
#define cdxGoHot                                NULL
#define cdxPutRec                               NULL
#define cdxPutValue                             NULL
#define cdxRecAll                               NULL
#define cdxRecCount                             NULL
#define cdxRecInfo                              NULL
#define cdxRecNo                                NULL
#define cdxSetFieldsExtent                      NULL
#define cdxAlias                                NULL
#define cdxClose                                NULL
#define cdxCreate                               NULL
#define cdxNewArea                              NULL
#define cdxOpen                                 NULL
#define cdxRelease                              NULL
#define cdxStructSize                           NULL
#define cdxSysName                              NULL
#define cdxEval                                 NULL
#define cdxPack                                 NULL
#define cdxZap                                  NULL
#define cdxOrderCondition                       NULL
#define cdxClearFilter                          NULL
#define cdxClearLocate                          NULL
#define cdxClearScope                           NULL
#define cdxCountScope                           NULL
#define cdxFilterText                           NULL
#define cdxScopeInfo                            NULL
#define cdxSetFilter                            NULL
#define cdxSetLocate                            NULL
#define cdxSetScope                             NULL
#define cdxSkipScope                            NULL
#define cdxCompile                              NULL
#define cdxError                                NULL
#define cdxEvalBlock                            NULL
#define cdxRawLock                              NULL
#define cdxLock                                 NULL
#define cdxUnLock                               NULL
#define cdxCloseMemFile                         NULL
#define cdxReadDBHeader                         NULL
#define cdxWhoCares                             NULL

static ERRCODE cdxCreateMemFile( AREAP pArea, LPDBOPENINFO pCreateInfo )
{
   LPFILEINFO lpMemInfo;
   LPMEMOHEADER pMemoHeader;
   BOOL bError;
   PHB_ITEM pError = NULL;

   HB_TRACE(HB_TR_DEBUG, ("cdxCreateMemFile(%p, %p)", pArea, pCreateInfo));

   lpMemInfo = pArea->lpDataInfo->pNext;
   do
   {
      lpMemInfo->hFile = hb_fsCreate( pCreateInfo->abName, FC_NORMAL );
      if( lpMemInfo->hFile == FS_ERROR )
      {
         if( !pError )
         {
            pError = hb_errNew();
            hb_errPutGenCode( pError, EG_CREATE );
            hb_errPutSubCode( pError, 1005 );
            hb_errPutDescription( pError, hb_langDGetErrorDesc( EG_CREATE ) );
            hb_errPutFileName( pError, ( char * ) pCreateInfo->abName );
            hb_errPutFlags( pError, EF_CANRETRY );
         }
         bError = ( SELF_ERROR( pArea, pError ) == E_RETRY );
      }
      else
         bError = FALSE;
   } while( bError );
   if( pError )
      hb_errRelease( pError );

   if( lpMemInfo->hFile == FS_ERROR )
      return FAILURE;

   pMemoHeader = ( LPMEMOHEADER ) hb_xgrab( 512 );
   memset( pMemoHeader, 0, 512 );
   pMemoHeader->lNextBlock = hb_cdxSwapBytes( 512 / MEMO_BLOCK );
   pMemoHeader->lBlockSize = hb_cdxSwapBytes( MEMO_BLOCK );
   bError = ( hb_fsWrite( lpMemInfo->hFile, ( BYTE * ) pMemoHeader, 512 ) != 512 );
   hb_xfree( pMemoHeader );
   hb_fsClose( lpMemInfo->hFile );
   lpMemInfo->hFile = FS_ERROR;
   if( bError )
      return FAILURE;
   else
      return SUCCESS;
}

static ERRCODE cdxGetValueFile( AREAP pArea, USHORT uiIndex, void * pFile )
{
   ULONG lRecNo, lNewRecNo;
   BYTE * szText, szEndChar;
   LPFIELD pField;

   HB_TRACE(HB_TR_DEBUG, ("cdxGetValueFile(%p, %hu, %p)", pArea, uiIndex, pFile));

   HB_SYMBOL_UNUSED( pFile );
   if( uiIndex > pArea->uiFieldCount )
      return FAILURE;

   pField = pArea->lpFields + uiIndex - 1;
   szText = pArea->lpExtendInfo->bRecord + pField->uiOffset;
   if( !( ( LPDBFMEMO ) pField->memo )->pData )
      memset( szText, ' ', pField->uiLen );
   else
   {
      szEndChar = * ( szText + pField->uiLen );
      * ( szText + pField->uiLen ) = 0;
      lRecNo = atol( ( char * ) szText );
      lNewRecNo = lRecNo;
      if( !hb_cdxWriteMemo( pArea, ( LPDBFMEMO ) pField->memo, &lNewRecNo ) )
         return FAILURE;
      if( lNewRecNo != lRecNo )
         hb_nltoa( lNewRecNo, ( char * ) szText, pField->uiLen );
      * ( szText + pField->uiLen ) = szEndChar;
   }
   ( ( LPDBFMEMO ) pField->memo )->fChanged = FALSE;
   return SUCCESS;
}

static ERRCODE cdxInfo( AREAP pArea, USHORT uiIndex, PHB_ITEM pItem )
{
   HB_TRACE(HB_TR_DEBUG, ("cdxInfo(%p, %hu, %p)", pArea, uiIndex, pItem));

   if( uiIndex == DBI_MEMOEXT )
   {
      hb_itemPutC( pItem, ".fpt" );
      return SUCCESS;
   }

   return SUPER_INFO( pArea, uiIndex, pItem );
}

static ERRCODE cdxOpenMemFile( AREAP pArea, LPDBOPENINFO pOpenInfo )
{
   LPFILEINFO lpMemInfo;
   LPMEMOHEADER pMemoHeader;
   USHORT uiFlags;
   PHB_ITEM pError = NULL;
   BOOL bRetry;

   HB_TRACE(HB_TR_DEBUG, ("cdxOpenMemFile(%p, %p)", pArea, pOpenInfo));

   lpMemInfo = pArea->lpDataInfo->pNext;
   uiFlags = pOpenInfo->fReadonly ? FO_READ : FO_READWRITE;
   uiFlags |= pOpenInfo->fShared ? FO_DENYNONE : FO_EXCLUSIVE;
   do
   {
      lpMemInfo->hFile = hb_fsOpen( pOpenInfo->abName, uiFlags );
      if( lpMemInfo->hFile == FS_ERROR )
      {
         if( !pError )
         {
            pError = hb_errNew();
            hb_errPutGenCode( pError, EG_OPEN );
            hb_errPutSubCode( pError, 1002 );
            hb_errPutDescription( pError, hb_langDGetErrorDesc( EG_OPEN ) );
            hb_errPutFileName( pError, ( char * ) pOpenInfo->abName );
            hb_errPutFlags( pError, EF_CANRETRY );
         }
         bRetry = ( SELF_ERROR( pArea, pError ) == E_RETRY );
      }
      else
         bRetry = FALSE;
   } while( bRetry );
   if( pError )
      hb_errRelease( pError );

   if( lpMemInfo->hFile == FS_ERROR )
      return FAILURE;

   pMemoHeader = ( LPMEMOHEADER ) hb_xgrab( 512 );
   if( hb_fsRead( lpMemInfo->hFile, ( BYTE * ) pMemoHeader, 512 ) != 512 )
   {
      hb_xfree( pMemoHeader );
      return FAILURE;
   }
   hb_xfree( pMemoHeader );
   return SUCCESS;
}

static ERRCODE cdxOrderCreate( AREAP pArea, LPDBORDERCREATEINFO pOrderInfo )
{
   PHB_ITEM pExpr, pResult, pError;
   HB_MACRO_PTR pMacro;
   USHORT uiType, uiLen = 0;
   char * szFileName;
   PHB_FNAME pFileName;
   DBORDERINFO pExtInfo;

   HB_TRACE(HB_TR_DEBUG, ("cdxOrderCreate(%p, %p)", pArea, pOrderInfo));

   if( SELF_GOCOLD( pArea ) == FAILURE )
      return FAILURE;

   /* If we have a codeblock for the expression, use it */
   if( pOrderInfo->itmCobExpr )
      pExpr = pOrderInfo->itmCobExpr;
   else /* Otherwise, try compiling the key expression string */
   {
      if( SELF_COMPILE( pArea, ( BYTE * ) hb_itemGetCPtr( pOrderInfo->abExpr ) ) == FAILURE )
         return FAILURE;
      pExpr = pArea->valResult;
      pArea->valResult = NULL;
   }

   /* Get a blank record before testing expression */
   SELF_GOBOTTOM( pArea );
   SELF_SKIP( pArea, 1 );
   if( hb_itemType( pExpr ) == IT_BLOCK )
   {
      if( SELF_EVALBLOCK( pArea, pExpr ) == FAILURE )
         return FAILURE;
      pResult = pArea->valResult;
      pArea->valResult = NULL;
   }
   else
   {
      pMacro = ( HB_MACRO_PTR ) hb_itemGetPtr( pExpr );
      hb_macroRun( pMacro );
      hb_macroDelete( pMacro );
      pResult = pExpr;
      hb_itemCopy( pResult, &hb_stack.Return );
   }

   uiType = hb_itemType( pResult );
   switch( uiType )
   {
      case IT_INTEGER:
      case IT_LONG:
      case IT_DOUBLE:
         uiLen = 10;
         break;

      case IT_DATE:
         uiLen = 8;
         break;

      case IT_LOGICAL:
         uiLen = 1;
         break;

      case IT_STRING:
         uiLen = ( hb_itemGetCLen( pResult ) > CDX_MAX_KEY ) ? CDX_MAX_KEY :
                 ( USHORT ) hb_itemGetCLen( pResult );
         break;
   }

   hb_itemRelease( pResult );

   /* Make sure uiLen is not 0 */
   if( !uiLen )
   {
      pError = hb_errNew();
      hb_errPutGenCode( pError, EG_DATAWIDTH );
      hb_errPutSubCode( pError, 1026 );
      hb_errPutDescription( pError, hb_langDGetErrorDesc( EG_DATAWIDTH ) );
      SELF_ERROR( pArea, pError );
      hb_errRelease( pError );
      return FAILURE;
   }

   /* Check conditional expression */
   pExpr = NULL;
   if( pArea->lpdbOrdCondInfo )
   {
      /* If we have a codeblock for the conditional expression, use it */
      if( pArea->lpdbOrdCondInfo->itmCobFor )
         pExpr = pArea->lpdbOrdCondInfo->itmCobFor;
      else /* Otherwise, try compiling the conditional expression string */
      {
         if( SELF_COMPILE( pArea, pArea->lpdbOrdCondInfo->abFor ) == FAILURE )
            return FAILURE;
         pExpr = pArea->valResult;
         pArea->valResult = NULL;
      }
   }

   /* Test conditional expression */
   if( pExpr )
   {
      if( hb_itemType( pExpr ) == IT_BLOCK )
      {
         if( SELF_EVALBLOCK( pArea, pExpr ) == FAILURE )
            return FAILURE;
         pResult = pArea->valResult;
      }
      else
      {
         pMacro = ( HB_MACRO_PTR ) hb_itemGetPtr( pExpr );
         hb_macroRun( pMacro );
         hb_macroDelete( pMacro );
         pResult = pExpr;
         hb_itemCopy( pResult, &hb_stack.Return );
      }
      uiType = hb_itemType( pResult );
      hb_itemRelease( pResult );
      if( uiType != IT_LOGICAL )
         return FAILURE;
   }

   /* Check file name */
   szFileName = ( char * ) hb_xgrab( _POSIX_PATH_MAX + 3 );
   szFileName[ 0 ] = '\0';
   if( strlen( ( char * ) pOrderInfo->abBagName ) == 0 )
   {
      pFileName = hb_fsFNameSplit( pArea->lpDataInfo->szFileName );
      if( pFileName->szDrive )
         strcat( szFileName, pFileName->szDrive );
      if( pFileName->szPath )
         strcat( szFileName, pFileName->szPath );
      strcat( szFileName, pFileName->szName );
      pExtInfo.itmResult = hb_itemPutC( NULL, "" );
      SELF_ORDINFO( pArea, DBOI_BAGEXT, &pExtInfo );
      strcat( szFileName, hb_itemGetCPtr( pExtInfo.itmResult ) );
      hb_itemRelease( pExtInfo.itmResult );
   }
   else
   {
      strcpy( szFileName, ( char * ) pOrderInfo->abBagName );
      pFileName = hb_fsFNameSplit( szFileName );
      if( !pFileName->szExtension )
      {
         pExtInfo.itmResult = hb_itemPutC( NULL, "" );
         SELF_ORDINFO( pArea, DBOI_BAGEXT, &pExtInfo );
         strcat( szFileName, hb_itemGetCPtr( pExtInfo.itmResult ) );
         hb_itemRelease( pExtInfo.itmResult );
      }
   }
   hb_xfree( pFileName );

   /* TODO:
      open szFileName
      if error
         create szFileName
         create new tag index
      else
         find tag index
         if not found
            create new tag index
         else
            overwrite tag index
      go top
      while not eof
         add key
         skip
   */

   hb_xfree( szFileName );

   /* Clear pArea->lpdbOrdCondInfo */
   SELF_ORDSETCOND( pArea, NULL );
   return SELF_GOTOP( pArea );
}

static ERRCODE cdxOrderDestroy( AREAP pArea, LPDBORDERINFO pOrderInfo )
{
   HB_TRACE(HB_TR_DEBUG, ("cdxOrderDestroy(%p, %p)", pArea, pOrderInfo));

   HB_SYMBOL_UNUSED( pArea );
   HB_SYMBOL_UNUSED( pOrderInfo );

   return SUCCESS;
}

static ERRCODE cdxOrderInfo( AREAP pArea, USHORT uiIndex, LPDBORDERINFO pInfo )
{
   HB_TRACE(HB_TR_DEBUG, ("cdxOrderInfo(%p, %hu, %p)", pArea, uiIndex, pInfo));

   HB_SYMBOL_UNUSED( pArea );

   switch( uiIndex )
   {
      case DBOI_BAGEXT:
         hb_itemPutC( pInfo->itmResult, ".cdx" );
         break;
   }
   return SUCCESS;
}

static ERRCODE cdxOrderListAdd( AREAP pArea, LPDBORDERINFO pOrderInfo )
{
   HB_TRACE(HB_TR_DEBUG, ("cdxOrderListAdd(%p, %p)", pArea, pOrderInfo));

   HB_SYMBOL_UNUSED( pArea );
   HB_SYMBOL_UNUSED( pOrderInfo );

   return SUCCESS;
}

static ERRCODE cdxOrderListClear( AREAP pArea )
{
   HB_TRACE(HB_TR_DEBUG, ("cdxOrderListClear(%p)", pArea));

   HB_SYMBOL_UNUSED( pArea );

   return SUCCESS;
}

static ERRCODE cdxOrderListFocus( AREAP pArea, LPDBORDERINFO pOrderInfo )
{
   HB_TRACE(HB_TR_DEBUG, ("cdxOrderListFocus(%p, %p)", pArea, pOrderInfo));

   HB_SYMBOL_UNUSED( pArea );
   HB_SYMBOL_UNUSED( pOrderInfo );

   return SUCCESS;
}

static ERRCODE cdxOrderListRebuild( AREAP pArea )
{
   HB_TRACE(HB_TR_DEBUG, ("cdxOrderListRebuild(%p)", pArea));

   HB_SYMBOL_UNUSED( pArea );

   return SUCCESS;
}

static ERRCODE cdxPutValueFile( AREAP pArea, USHORT uiIndex, void * pFile )
{
   LPFIELD pField;
   BYTE * szText, szEndChar;
   ULONG lMemoBlock;

   HB_TRACE(HB_TR_DEBUG, ("cdxPutValueFile(%p, %hu, %p)", pArea, uiIndex, pFile));

   HB_SYMBOL_UNUSED( pFile );

   if( uiIndex > pArea->uiFieldCount )
      return FAILURE;

   pField = pArea->lpFields + uiIndex - 1;;
   szText = pArea->lpExtendInfo->bRecord + pField->uiOffset;
   szEndChar = * ( szText + pField->uiLen );
   * ( szText + pField->uiLen ) = 0;
   lMemoBlock = atol( ( char * ) szText ) * MEMO_BLOCK;
   * ( szText + pField->uiLen ) = szEndChar;
   if( lMemoBlock > 0 )
      hb_cdxReadMemo( pArea, ( LPDBFMEMO ) pField->memo, lMemoBlock );
   else if( ( ( LPDBFMEMO ) pField->memo )->pData )
   {
      hb_xfree( ( ( LPDBFMEMO ) pField->memo )->pData );
      memset( pField->memo, 0, sizeof( DBFMEMO ) );
   }
   return SUCCESS;
}

static ERRCODE cdxSeek( AREAP pArea, BOOL bSoftSeek, PHB_ITEM pKey, BOOL bFindLast )
{
   HB_TRACE(HB_TR_DEBUG, ("cdxSeek(%p, %d, %p, %d)", pArea, bSoftSeek, pKey, bFindLast));

   HB_SYMBOL_UNUSED( pArea );
   HB_SYMBOL_UNUSED( bSoftSeek );
   HB_SYMBOL_UNUSED( pKey );
   HB_SYMBOL_UNUSED( bFindLast );

   return SUCCESS;
}

static ERRCODE cdxWriteDBHeader( AREAP pArea )
{
   DBFHEADER pHeader;
   DBFFIELD pDBField;
   USHORT uiCount;
   LPFIELD pField;
   time_t t;
   struct tm * pTime;

   HB_TRACE(HB_TR_DEBUG, ("cdxWriteDBHeader(%p)", pArea));

   memset( &pHeader, 0, sizeof( DBFHEADER ) );
   pHeader.uiRecordLen = 1;
   pHeader.bVersion = 0x03;
   pField = pArea->lpFields;
   for( uiCount = 0; uiCount < pArea->uiFieldCount; uiCount++ )
   {
      switch( pField->uiType )
      {
         case 'C':
         case 'N':
            pHeader.uiRecordLen += pField->uiLen;
            break;

         case 'M':
            pHeader.uiRecordLen += 10;
            pHeader.bVersion = 0xF5;
            pArea->lpExtendInfo->fHasMemo = TRUE;
            break;

         case 'D':
            pHeader.uiRecordLen += 8;
            break;

         case 'L':
            pHeader.uiRecordLen += 1;
            break;
      }
      pField++;
   }

   time( &t );
   pTime =  localtime( &t );
   pHeader.bYear = ( BYTE ) pTime->tm_year;
   pHeader.bMonth = ( BYTE ) pTime->tm_mon + 1;
   pHeader.bDay = ( BYTE ) pTime->tm_mday;
   pHeader.uiHeaderLen = ( USHORT ) ( 32 * ( pArea->uiFieldCount + 1 ) + 1 );
   pHeader.bHasTag = 0;
   pHeader.ulRecords = 0;
   if( hb_fsWrite( pArea->lpDataInfo->hFile, ( BYTE * ) &pHeader,
                   sizeof( DBFHEADER ) ) != sizeof( DBFHEADER ) )
      return FAILURE;

   pField = pArea->lpFields;
   for( uiCount = 0; uiCount < pArea->uiFieldCount; uiCount++ )
   {
      memset( &pDBField, 0, sizeof( DBFFIELD ) );
      strncpy( ( char * ) pDBField.bName, ( ( PHB_DYNS ) pField->sym )->pSymbol->szName,
               sizeof( pDBField.bName ) );
      pDBField.bType = ( BYTE ) pField->uiType;
      switch( pDBField.bType )
      {
         case 'C':
            pDBField.bLen = pField->uiLen & 0xFF;
            pDBField.bDec = pField->uiLen >> 8;
            break;

         case 'M':
            pDBField.bLen = 10;
            pDBField.bDec = 0;
            break;

         case 'D':
            pDBField.bLen = 8;
            pDBField.bDec = 0;
            break;

         case 'L':
            pDBField.bLen = 1;
            pDBField.bDec = 0;
            break;

         case 'N':
            pDBField.bLen = ( BYTE ) pField->uiLen;
            pDBField.bDec = ( BYTE ) pField->uiDec;
            break;
      }
      if( hb_fsWrite( pArea->lpDataInfo->hFile, ( BYTE * ) &pDBField,
                      sizeof( DBFFIELD ) ) != sizeof( DBFFIELD ) )
         return FAILURE;
      pField++;
   }
   if( hb_fsWrite( pArea->lpDataInfo->hFile, ( BYTE * ) "\15\32", 2 ) != 2 )
      return FAILURE;
   return SUCCESS;
}

static RDDFUNCS cdxTable = { cdxBof,
                             cdxEof,
                             cdxFound,
                             cdxGoBottom,
                             cdxGoTo,
                             cdxGoToId,
                             cdxGoTop,
                             cdxSeek,
                             cdxSkip,
                             cdxSkipFilter,
                             cdxSkipRaw,
                             cdxAddField,
                             cdxAppend,
                             cdxCreateFields,
                             cdxDeleteRec,
                             cdxDeleted,
                             cdxFieldCount,
                             cdxFieldDisplay,
                             cdxFieldInfo,
                             cdxFieldName,
                             cdxFlush,
                             cdxGetRec,
                             cdxGetValue,
                             cdxGetVarLen,
                             cdxGoCold,
                             cdxGoHot,
                             cdxPutRec,
                             cdxPutValue,
                             cdxRecAll,
                             cdxRecCount,
                             cdxRecInfo,
                             cdxRecNo,
                             cdxSetFieldsExtent,
                             cdxAlias,
                             cdxClose,
                             cdxCreate,
                             cdxInfo,
                             cdxNewArea,
                             cdxOpen,
                             cdxRelease,
                             cdxStructSize,
                             cdxSysName,
                             cdxEval,
                             cdxPack,
                             cdxZap,
                             cdxOrderListAdd,
                             cdxOrderListClear,
                             cdxOrderListFocus,
                             cdxOrderListRebuild,
                             cdxOrderCondition,
                             cdxOrderCreate,
                             cdxOrderDestroy,
                             cdxOrderInfo,
                             cdxClearFilter,
                             cdxClearLocate,
                             cdxClearScope,
                             cdxCountScope,
                             cdxFilterText,
                             cdxScopeInfo,
                             cdxSetFilter,
                             cdxSetLocate,
                             cdxSetScope,
                             cdxSkipScope,
                             cdxCompile,
                             cdxError,
                             cdxEvalBlock,
                             cdxRawLock,
                             cdxLock,
                             cdxUnLock,
                             cdxCloseMemFile,
                             cdxCreateMemFile,
                             cdxGetValueFile,
                             cdxOpenMemFile,
                             cdxPutValueFile,
                             cdxReadDBHeader,
                             cdxWriteDBHeader,
                             cdxWhoCares
                           };

HB_FUNC( _DBFCDX )
{
}

HB_FUNC( DBFCDX_GETFUNCTABLE )
{
   RDDFUNCS * pTable;
   USHORT * uiCount;

   uiCount = ( USHORT * ) hb_parnl( 1 );
   * uiCount = RDDFUNCSCOUNT;
   pTable = ( RDDFUNCS * ) hb_parnl( 2 );
   if( pTable )
      hb_retni( hb_rddInherit( pTable, &cdxTable, &cdxSuper, ( BYTE * ) "DBF" ) );
   else
      hb_retni( FAILURE );
}

