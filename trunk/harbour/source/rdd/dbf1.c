/*
 * $Id$
 */

/*
 * Harbour Project source code:
 * DBF RDD module
 *
 * Copyright 1999 Bruno Cantero <bruno@issnet.net>
 * www - http://www.harbour-project.org
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this software; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 59 Temple Place, Suite 330,
 * Boston, MA 02111-1307 USA (or visit the web site http://www.gnu.org/).
 *
 * As a special exception, the Harbour Project gives permission for
 * additional uses of the text contained in its release of Harbour.
 *
 * The exception is that, if you link the Harbour libraries with other
 * files to produce an executable, this does not by itself cause the
 * resulting executable to be covered by the GNU General Public License.
 * Your use of that executable is in no way restricted on account of
 * linking the Harbour library code into it.
 *
 * This exception does not however invalidate any other reasons why
 * the executable file might be covered by the GNU General Public License.
 *
 * This exception applies only to the code released by the Harbour
 * Project under the name Harbour.  If you copy code from other
 * Harbour Project or Free Software Foundation releases into a copy of
 * Harbour, as the General Public License permits, the exception does
 * not apply to the code that you add in this way.  To avoid misleading
 * anyone as to the status of such modified files, you must delete
 * this exception notice from them.
 *
 * If you write modifications of your own for Harbour, it is your choice
 * whether to permit this exception to apply to your modifications.
 * If you do not wish that, delete this exception notice.
 *
 */

#include "hbapi.h"
#include "hbinit.h"
#include "hbvm.h"
#include "hbapiitm.h"
#include "hbrdddbf.h"
#include "hbdbf.h"
#include "hbapierr.h"
#include "hbapilng.h"
#include "hbset.h"
#include "hbdate.h"
#include "hbmath.h"
#include "hbdbsort.h"
#include "error.ch"
#include <errno.h>
#include "hbapicdp.h"
extern PHB_CODEPAGE s_cdpage;

#define __PRG_SOURCE__ __FILE__

HB_FUNC( _DBFC );
HB_FUNC( DBF_GETFUNCTABLE );
#define __PRG_SOURCE__ __FILE__
#undef HB_PRG_PCODE_VER
#define HB_PRG_PCODE_VER HB_PCODE_VER


HB_INIT_SYMBOLS_BEGIN( dbf1__InitSymbols )
{ "_DBFC",            HB_FS_PUBLIC, {HB_FUNCNAME( _DBFC )}, NULL },
{ "DBF_GETFUNCTABLE", HB_FS_PUBLIC, {HB_FUNCNAME( DBF_GETFUNCTABLE )}, NULL }
HB_INIT_SYMBOLS_END( dbf1__InitSymbols )

#if defined(HB_STATIC_STARTUP)
   #pragma startup dbf1__InitSymbols
#elif defined(_MSC_VER)
   #if _MSC_VER >= 1010
      #pragma data_seg( ".CRT$XIY" )
      #pragma comment( linker, "/Merge:.CRT=.data" )
   #else
      #pragma data_seg( "XIY" )
   #endif
   static HB_$INITSYM hb_vm_auto_dbf1__InitSymbols = dbf1__InitSymbols;
   #pragma data_seg()
#elif ! defined(__GNUC__)
   #pragma startup dbf1__InitSymbols
#endif

static RDDFUNCS dbfSuper;
static RDDFUNCS dbfTable = { ( DBENTRYP_BP ) hb_dbfBof,
                             ( DBENTRYP_BP ) hb_dbfEof,
                             ( DBENTRYP_BP ) hb_dbfFound,
                             ( DBENTRYP_V ) hb_dbfGoBottom,
                             ( DBENTRYP_UL ) hb_dbfGoTo,
                             ( DBENTRYP_I ) hb_dbfGoToId,
                             ( DBENTRYP_V ) hb_dbfGoTop,
                             ( DBENTRYP_BIB ) hb_dbfSeek,
                             ( DBENTRYP_L ) hb_dbfSkip,
                             ( DBENTRYP_L ) hb_dbfSkipFilter,
                             ( DBENTRYP_L ) hb_dbfSkipRaw,
                             ( DBENTRYP_VF ) hb_dbfAddField,
                             ( DBENTRYP_B ) hb_dbfAppend,
                             ( DBENTRYP_I ) hb_dbfCreateFields,
                             ( DBENTRYP_V ) hb_dbfDeleteRec,
                             ( DBENTRYP_BP ) hb_dbfDeleted,
                             ( DBENTRYP_SP ) hb_dbfFieldCount,
                             ( DBENTRYP_VF ) hb_dbfFieldDisplay,
                             ( DBENTRYP_SSI ) hb_dbfFieldInfo,
                             ( DBENTRYP_SVP ) hb_dbfFieldName,
                             ( DBENTRYP_V ) hb_dbfFlush,
                             ( DBENTRYP_PP ) hb_dbfGetRec,
                             ( DBENTRYP_SI ) hb_dbfGetValue,
                             ( DBENTRYP_SVL ) hb_dbfGetVarLen,
                             ( DBENTRYP_V ) hb_dbfGoCold,
                             ( DBENTRYP_V ) hb_dbfGoHot,
                             ( DBENTRYP_P ) hb_dbfPutRec,
                             ( DBENTRYP_SI ) hb_dbfPutValue,
                             ( DBENTRYP_V ) hb_dbfRecall,
                             ( DBENTRYP_ULP ) hb_dbfRecCount,
                             ( DBENTRYP_ISI ) hb_dbfRecInfo,
                             ( DBENTRYP_I ) hb_dbfRecNo,
                             ( DBENTRYP_S ) hb_dbfSetFieldExtent,
                             ( DBENTRYP_P ) hb_dbfAlias,
                             ( DBENTRYP_V ) hb_dbfClose,
                             ( DBENTRYP_VP ) hb_dbfCreate,
                             ( DBENTRYP_SI ) hb_dbfInfo,
                             ( DBENTRYP_V ) hb_dbfNewArea,
                             ( DBENTRYP_VP ) hb_dbfOpen,
                             ( DBENTRYP_V ) hb_dbfRelease,
                             ( DBENTRYP_SP ) hb_dbfStructSize,
                             ( DBENTRYP_P ) hb_dbfSysName,
                             ( DBENTRYP_VEI ) hb_dbfEval,
                             ( DBENTRYP_V ) hb_dbfPack,
                             ( DBENTRYP_LSP ) hb_dbfPackRec,
                             ( DBENTRYP_VS ) hb_dbfSort,
                             ( DBENTRYP_VT ) hb_dbfTrans,
                             ( DBENTRYP_VT ) hb_dbfTransRec,
                             ( DBENTRYP_V ) hb_dbfZap,
                             ( DBENTRYP_VR ) hb_dbfChildEnd,
                             ( DBENTRYP_VR ) hb_dbfChildStart,
                             ( DBENTRYP_VR ) hb_dbfChildSync,
                             ( DBENTRYP_V ) hb_dbfSyncChildren,
                             ( DBENTRYP_V ) hb_dbfClearRel,
                             ( DBENTRYP_V ) hb_dbfForceRel,
                             ( DBENTRYP_SVP ) hb_dbfRelArea,
                             ( DBENTRYP_VR ) hb_dbfRelEval,
                             ( DBENTRYP_SVP ) hb_dbfRelText,
                             ( DBENTRYP_VR ) hb_dbfSetRel,
                             ( DBENTRYP_OI ) hb_dbfOrderListAdd,
                             ( DBENTRYP_V ) hb_dbfOrderListClear,
                             ( DBENTRYP_VP ) hb_dbfOrderListDelete,
                             ( DBENTRYP_OI ) hb_dbfOrderListFocus,
                             ( DBENTRYP_V ) hb_dbfOrderListRebuild,
                             ( DBENTRYP_VOI ) hb_dbfOrderCondition,
                             ( DBENTRYP_VOC ) hb_dbfOrderCreate,
                             ( DBENTRYP_OI ) hb_dbfOrderDestroy,
                             ( DBENTRYP_OII ) hb_dbfOrderInfo,
                             ( DBENTRYP_V ) hb_dbfClearFilter,
                             ( DBENTRYP_V ) hb_dbfClearLocate,
                             ( DBENTRYP_V ) hb_dbfClearScope,
                             ( DBENTRYP_VPLP ) hb_dbfCountScope,
                             ( DBENTRYP_I ) hb_dbfFilterText,
                             ( DBENTRYP_SI ) hb_dbfScopeInfo,
                             ( DBENTRYP_VFI ) hb_dbfSetFilter,
                             ( DBENTRYP_VLO ) hb_dbfSetLocate,
                             ( DBENTRYP_VOS ) hb_dbfSetScope,
                             ( DBENTRYP_VPL ) hb_dbfSkipScope,
                             ( DBENTRYP_P ) hb_dbfCompile,
                             ( DBENTRYP_I ) hb_dbfError,
                             ( DBENTRYP_I ) hb_dbfEvalBlock,
                             ( DBENTRYP_VSP ) hb_dbfRawLock,
                             ( DBENTRYP_VL ) hb_dbfLock,
                             ( DBENTRYP_UL ) hb_dbfUnLock,
                             ( DBENTRYP_V ) hb_dbfCloseMemFile,
                             ( DBENTRYP_VP ) hb_dbfCreateMemFile,
                             ( DBENTRYP_SVPB ) hb_dbfGetValueFile,
                             ( DBENTRYP_VP ) hb_dbfOpenMemFile,
                             ( DBENTRYP_SVP ) hb_dbfPutValueFile,
                             ( DBENTRYP_V ) hb_dbfReadDBHeader,
                             ( DBENTRYP_V ) hb_dbfWriteDBHeader,
                             ( DBENTRYP_I0 ) hb_dbfExit,
                             ( DBENTRYP_I1 ) hb_dbfDrop,
                             ( DBENTRYP_I2 ) hb_dbfExists,
                             ( DBENTRYP_SVP ) hb_dbfWhoCares
                           };

/*
 * Common functions.
 */

/*
 * Return the total number of records.
 */
static ULONG hb_dbfCalcRecCount( DBFAREAP pArea )
{
   HB_TRACE(HB_TR_DEBUG, ("hb_dbfCalcRecCount(%p)", pArea));

   return ( hb_fsSeek( pArea->hDataFile, 0, FS_END ) - pArea->uiHeaderLen ) /
            pArea->uiRecordLen;
}

/*
 * Read current record from file.
 */
static BOOL hb_dbfReadRecord( DBFAREAP pArea )
{
   PHB_ITEM pError;

   HB_TRACE(HB_TR_DEBUG, ("hb_dbfReadRecord(%p)", pArea));

   if( pArea->fEof )
   {
      pArea->fValidBuffer = TRUE;
      return TRUE;
   }

   if( pArea->ulRecNo > pArea->ulRecCount )
   {
      /* Update record count */
      if( pArea->fShared )
         pArea->ulRecCount = hb_dbfCalcRecCount( pArea );

      if( pArea->ulRecNo > pArea->ulRecCount )
      {
         pArea->fEof = pArea->fValidBuffer = TRUE;
         return TRUE;
      }
   }

   /* Read data */
   hb_fsSeek( pArea->hDataFile, pArea->uiHeaderLen + ( pArea->ulRecNo - 1 ) *
              pArea->uiRecordLen, FS_SET );
   if( hb_fsRead( pArea->hDataFile, pArea->pRecord, pArea->uiRecordLen ) !=
       pArea->uiRecordLen )
   {
      pError = hb_errNew();
      hb_errPutGenCode( pError, EG_READ );
      hb_errPutDescription( pError, hb_langDGetErrorDesc( EG_READ ) );
      hb_errPutSubCode( pError, EDBF_READ );
      SELF_ERROR( ( AREAP ) pArea, pError );
      hb_itemRelease( pError );
      return FALSE;
   }

   /* Set flags */
   pArea->fValidBuffer = pArea->fPositioned = TRUE;
   pArea->fDeleted = ( pArea->pRecord[ 0 ] == '*' );
   return TRUE;
}

/*
 * Write current record to file.
 */
static BOOL hb_dbfWriteRecord( DBFAREAP pArea )
{
   PHB_ITEM pError;

   HB_TRACE(HB_TR_DEBUG, ("hb_dbfWriteRecord(%p)", pArea));

   /* Write data */
   hb_fsSeek( pArea->hDataFile, pArea->uiHeaderLen + ( pArea->ulRecNo - 1 ) *
              pArea->uiRecordLen, FS_SET );
   if( hb_fsWrite( pArea->hDataFile, pArea->pRecord, pArea->uiRecordLen ) !=
       pArea->uiRecordLen )
   {
      pError = hb_errNew();
      hb_errPutGenCode( pError, EG_WRITE );
      hb_errPutDescription( pError, hb_langDGetErrorDesc( EG_WRITE ) );
      hb_errPutSubCode( pError, EDBF_WRITE );
      SELF_ERROR( ( AREAP ) pArea, pError );
      hb_itemRelease( pError );
      return FALSE;
   }
   return TRUE;
}

/*
 * Unlock all records.
 */
static ERRCODE hb_dbfUnlockAllRecords( DBFAREAP pArea )
{
   ERRCODE uiError = SUCCESS;

   HB_TRACE(HB_TR_DEBUG, ("hb_dbfUnlockAllRecords(%p, %p)", pArea ));

   if( pArea->pLocksPos )
   {
      ULONG ulCount;

      uiError = SELF_GOCOLD( ( AREAP ) pArea );
      for( ulCount = 0; ulCount < pArea->ulNumLocksPos; ulCount++ )
         SELF_RAWLOCK( ( AREAP ) pArea, REC_UNLOCK, pArea->pLocksPos[ ulCount ] );
      hb_xfree( pArea->pLocksPos );
      pArea->pLocksPos = NULL;
   }
   pArea->ulNumLocksPos = 0;
   return uiError;
}

/*
 * Unlock a records.
 */
static ERRCODE hb_dbfUnlockRecord( DBFAREAP pArea, ULONG ulRecNo )
{
   ERRCODE uiError = SUCCESS;
   ULONG ulCount, * pList;

   HB_TRACE(HB_TR_DEBUG, ("hb_dbfUnlockRecord(%p, %lu)", pArea, ulRecNo));

   /* Search the locked record */
   for( ulCount = 0; ulCount < pArea->ulNumLocksPos &&
                     pArea->pLocksPos[ ulCount ] != ulRecNo; ulCount++ );

   if( ulCount < pArea->ulNumLocksPos )
   {
      uiError = SELF_GOCOLD( ( AREAP ) pArea );
      SELF_RAWLOCK( ( AREAP ) pArea, REC_UNLOCK, ulRecNo );
      if( pArea->ulNumLocksPos == 1 )            /* Delete the list */
      {
         hb_xfree( pArea->pLocksPos );
         pArea->pLocksPos = NULL;
         pArea->ulNumLocksPos = 0;
      }
      else                                       /* Resize the list */
      {
         pList = pArea->pLocksPos + ulCount;
         memmove( pList, pList + 1, ( pArea->ulNumLocksPos - ulCount - 1 ) *
                  sizeof( ULONG ) );
         pArea->pLocksPos = ( ULONG * ) hb_xrealloc( pArea->pLocksPos,
                                                     ( pArea->ulNumLocksPos - 1 ) *
                                                     sizeof( ULONG ) );
         pArea->ulNumLocksPos --;
      }
   }
   return uiError;
}

/*
 * Lock a record.
 */
static ERRCODE hb_dbfLockRecord( DBFAREAP pArea, ULONG ulRecNo, BOOL * pResult,
                                 BOOL bExclusive )
{
   HB_TRACE(HB_TR_DEBUG, ("hb_dbfLockRecord(%p, %lu, %p, %i)", pArea, ulRecNo,
            pResult, (int) bExclusive));

   if( pArea->lpdbPendingRel )
      SELF_FORCEREL( ( AREAP ) pArea );

   if( pArea->fFLocked )
   {
      * pResult = TRUE;
      return SUCCESS;
   }

   /* TODO: what is this ??? */
   if( ulRecNo <= 0 )
      ulRecNo = 1;

   if( bExclusive )
      hb_dbfUnlockAllRecords( pArea );

   if( pArea->ulNumLocksPos > 0 )
   {
      ULONG ul;
      for( ul=0; ul< pArea->ulNumLocksPos; ul++ )
         if( pArea->pLocksPos[ ul ] == ulRecNo )
         {
            * pResult = TRUE;
            return SUCCESS;
         }
   }

   if( SELF_RAWLOCK( ( AREAP ) pArea, REC_LOCK, ulRecNo ) == SUCCESS )
   {
      if( pArea->ulNumLocksPos == 0 )               /* Create the list */
      {
         pArea->pLocksPos = ( ULONG * ) hb_xgrab( sizeof( ULONG ) );
         pArea->pLocksPos[ 0 ] = ulRecNo;
      }
      else                                          /* Resize the list */
      {
         pArea->pLocksPos = ( ULONG * ) hb_xrealloc( pArea->pLocksPos,
                                                   ( pArea->ulNumLocksPos + 1 ) *
                                                     sizeof( ULONG ) );
         pArea->pLocksPos[ pArea->ulNumLocksPos ] = ulRecNo;
      }
      pArea->ulNumLocksPos ++;
      * pResult = TRUE;
      if( !pArea->fPositioned )
         hb_dbfGoTo( pArea, pArea->ulRecNo );
      else if ( !pArea->fRecordChanged )
         pArea->fValidBuffer = FALSE;
   }
   else
      * pResult = FALSE;
   return SUCCESS;
}

/*
 * Lock a file.
 */
static ERRCODE hb_dbfLockFile( DBFAREAP pArea, BOOL * pResult )
{
   HB_TRACE(HB_TR_DEBUG, ("hb_dbfLockFile(%p, %p)", pArea, pResult));

   if( !pArea->fFLocked )
   {
      if( pArea->lpdbPendingRel )
         SELF_FORCEREL( ( AREAP ) pArea );

      hb_dbfUnlockAllRecords( pArea );

      SELF_RAWLOCK( ( AREAP ) pArea, FILE_LOCK, 0 );
      * pResult = pArea->fFLocked;

      if( !pArea->fPositioned )
         hb_dbfGoTo( pArea, pArea->ulRecNo );
      else if ( !pArea->fRecordChanged )
         pArea->fValidBuffer = FALSE;
   }
   else
      * pResult = TRUE;

   return SUCCESS;
}

/*
 * Unlock a file.
 */
static ERRCODE hb_dbfUnlockFile( DBFAREAP pArea )
{
   ERRCODE uiError = SUCCESS;

   HB_TRACE(HB_TR_DEBUG, ("hb_dbfUnlockFile(%p)", pArea));

   if( pArea->fFLocked )
   {
      uiError = SELF_GOCOLD( ( AREAP ) pArea );
      SELF_RAWLOCK( ( AREAP ) pArea, FILE_UNLOCK, 0 );
   }
   return uiError;
}

/*
 * Test if a record is locked.
 */
static BOOL hb_dbfIsLocked( DBFAREAP pArea, ULONG ulRecNo )
{
   ULONG ulCount;

   HB_TRACE(HB_TR_DEBUG, ("hb_dbfIsLocked(%p)", pArea));

   ulCount = pArea->ulNumLocksPos;
   while( ulCount > 0 )
   {
      if( pArea->pLocksPos[ ulCount - 1 ] == ulRecNo )
         return TRUE;
      ulCount --;
   }

   return FALSE;
}

/*
 * Return an array filled all locked records.
 */
static void hb_dbfGetLockArray( DBFAREAP pArea, PHB_ITEM pItem )
{
   ULONG ulCount;
   PHB_ITEM pRecNo;

   pRecNo = hb_itemNew( NULL );
   HB_TRACE(HB_TR_DEBUG, ("hb_dbfGetLockArray(%p, %p)", pArea, pItem));

   for( ulCount = 0; ulCount < pArea->ulNumLocksPos; ulCount++ )
      hb_arrayAdd( pItem, hb_itemPutNL( pRecNo, pArea->pLocksPos[ ulCount ] ) );
   hb_itemRelease( pRecNo );
}

/*
 * Converts EDBF_* error code into EG_* one.
 * This function is common for different DBF based RDD implementation
 * so I don't make it static
 */
ERRCODE hb_dbfGetEGcode( ERRCODE errCode )
{
   ERRCODE errEGcode;

   HB_TRACE(HB_TR_DEBUG, ("hb_dbfGetEGcode(%hu)", errCode));

   switch ( errCode )
   {
      case EDBF_OPEN_DBF:
         errEGcode = EG_OPEN;
         break;
      case EDBF_CREATE_DBF:
         errEGcode = EG_CREATE;
         break;
      case EDBF_READ:
         errEGcode = EG_READ;
         break;
      case EDBF_WRITE:
         errEGcode = EG_WRITE;
         break;
      case EDBF_CORRUPT:
         errEGcode = EG_CORRUPTION;
         break;
      case EDBF_DATATYPE:
         errEGcode = EG_DATATYPE;
         break;
      case EDBF_DATAWIDTH:
         errEGcode = EG_DATAWIDTH;
         break;
      case EDBF_UNLOCKED:
         errEGcode = EG_UNLOCKED;
         break;
      case EDBF_SHARED:
         errEGcode = EG_SHARED;
         break;
      case EDBF_APPENDLOCK:
         errEGcode = EG_APPENDLOCK;
         break;
      case EDBF_READONLY:
         errEGcode = EG_READONLY;
         break;
      case EDBF_LOCK:
         errEGcode = EG_LOCK;
         break;
      case EDBF_INVALIDKEY:
      default:
         errEGcode = EG_UNSUPPORTED;
         break;
   }

   return errEGcode;
}

/*
 * Converts memo block offset into ASCII.
 * This function is common for different MEMO implementation
 * so I left it in DBF.
 */
ULONG hb_dbfGetMemoBlock( DBFAREAP pArea, USHORT uiIndex )
{
   USHORT uiCount;
   BYTE bByte;
   ULONG ulBlock;

   HB_TRACE(HB_TR_DEBUG, ("hb_dbfGetMemoBlock(%p, %hu)", pArea, uiIndex));

   ulBlock = 0;
   for( uiCount = 0; uiCount < 10; uiCount++ )
   {
      bByte = pArea->pRecord[ pArea->pFieldOffset[ uiIndex ] + uiCount ];
      if( bByte >= '0' && bByte <= '9' )
         ulBlock = ulBlock * 10 + ( bByte - '0' );
   }
   return ulBlock;
}

/*
 * Converts ASCII data into memo block offset.
 * This function is common for different MEMO implementation
 * so I left it in DBF.
 */
void hb_dbfPutMemoBlock( DBFAREAP pArea, USHORT uiIndex, ULONG ulBlock )
{
   SHORT iCount;

   HB_TRACE(HB_TR_DEBUG, ("hb_dbfPutMemoBlock(%p, %hu, %lu)", pArea, uiIndex, ulBlock));

   for( iCount = 9; iCount >= 0; iCount-- )
   {
      if( ulBlock > 0 )
      {
         pArea->pRecord[ pArea->pFieldOffset[ uiIndex ] + iCount ] = ( BYTE )( ulBlock % 10 ) + '0';
         ulBlock /= 10;
      }
      else
         pArea->pRecord[ pArea->pFieldOffset[ uiIndex ] + iCount ] = ' ';
   }
}

/*
 * Get information about locking schemes for additional files (MEMO, INDEX)
 * This function is common for different MEMO implementation
 * so I left it in DBF.
 */
BOOL HB_EXPORT hb_dbfLockIdxGetData( BYTE bScheme, ULONG *ulPos, ULONG *ulPool )
{
   switch ( bScheme )
   {
      case HB_SET_DBFLOCK_CLIP:
         *ulPos  = IDX_LOCKPOS_CLIP;
         *ulPool = IDX_LOCKPOOL_CLIP;
         break;

      case HB_SET_DBFLOCK_CL53:
         *ulPos  = IDX_LOCKPOS_CL53;
         *ulPool = IDX_LOCKPOOL_CL53;
         break;

      case HB_SET_DBFLOCK_VFP:
         *ulPos  = IDX_LOCKPOS_VFP;
         *ulPool = IDX_LOCKPOOL_VFP;
         break;

      default:
         return FALSE;
   }
   return TRUE;
}

/*
 * Set lock using current locking schemes in additional files (MEMO, INDEX)
 * This function is common for different MEMO implementation
 * so I left it in DBF.
 */
BOOL HB_EXPORT hb_dbfLockIdxFile( FHANDLE hFile, BYTE bScheme, USHORT usMode, ULONG *pPoolPos )
{
   ULONG ulPos, ulPool, ulSize = 1;
   BOOL fRet = FALSE, fWait;

   if ( !hb_dbfLockIdxGetData( bScheme, &ulPos, &ulPool ) )
      return fRet;

   do
   {
      switch ( usMode & FL_MASK )
      {
         case FL_LOCK:
            if ( ulPool )
            {
               if ( ( usMode & FLX_SHARED ) != 0 )
                  *pPoolPos = (ULONG) ( hb_random_num() * ulPool ) + 1;
               else
               {
                  *pPoolPos = 0;
                  ulSize = ulPool + 1;
               }
            }
            break;

         case FL_UNLOCK:
            if ( ulPool )
            {
               if ( ! *pPoolPos )
                  ulSize = ulPool + 1;
            }
            break;

         default:
            return FALSE;
      }
      fRet = hb_fsLock( hFile, ulPos + *pPoolPos, ulSize, usMode );
      fWait = ( !fRet && ( usMode & FLX_WAIT ) != 0 && ( usMode & FL_MASK ) == FL_LOCK );
      /* TODO: call special error handler (LOCKHANDLER) hiere if fWait */

   } while ( fWait );
   return fRet;
}

/*
 * -- DBF METHODS --
 */

/*
 * Determine logical beginning of file.
 */
static ERRCODE hb_dbfBof( DBFAREAP pArea, BOOL * pBof )
{
   HB_TRACE(HB_TR_DEBUG, ("hb_dbfBof(%p, %p)", pArea, pBof));

   if( pArea->lpdbPendingRel )
      SELF_FORCEREL( ( AREAP ) pArea );

   * pBof = pArea->fBof;
   return SUCCESS;
}

/*
 * Determine logical end of file.
 */
static ERRCODE hb_dbfEof( DBFAREAP pArea, BOOL * pEof )
{
   HB_TRACE(HB_TR_DEBUG, ("hb_dbfEof(%p, %p)", pArea, pEof));

   if( pArea->lpdbPendingRel )
      SELF_FORCEREL( ( AREAP ) pArea );

   * pEof = pArea->fEof;
   return SUCCESS;
}

/*
 * Determine outcome of the last search operation.
 */
static ERRCODE hb_dbfFound( DBFAREAP pArea, BOOL * pFound )
{
   HB_TRACE(HB_TR_DEBUG, ("hb_dbfFound(%p, %p)", pArea, pFound));

   if( pArea->lpdbPendingRel )
      SELF_FORCEREL( ( AREAP ) pArea );

   * pFound = pArea->fFound;
   return SUCCESS;
}

/*
 * Position cursor at the last record.
 */
static ERRCODE hb_dbfGoBottom( DBFAREAP pArea )
{
   HB_TRACE(HB_TR_DEBUG, ("hb_dbfGoBottom(%p)", pArea));

   if( SELF_GOCOLD( ( AREAP ) pArea ) == FAILURE )
      return FAILURE;

   /* Update record count */
   if( pArea->fShared )
      pArea->ulRecCount = hb_dbfCalcRecCount( pArea );

   pArea->fTop = FALSE;
   pArea->fBottom = TRUE;
   SELF_GOTO( ( AREAP ) pArea, pArea->ulRecCount );

   return SELF_SKIPFILTER( ( AREAP ) pArea, -1 );
}

/*
 * Position cursor at a specific physical record.
 */
static ERRCODE hb_dbfGoTo( DBFAREAP pArea, ULONG ulRecNo )
{
   HB_TRACE(HB_TR_DEBUG, ("hb_dbfGoTo(%p, %lu)", pArea, ulRecNo));

   if( SELF_GOCOLD( ( AREAP ) pArea ) == FAILURE )
      return FAILURE;

   /* Reset parent rel struct */
   pArea->lpdbPendingRel = NULL;

   /* Update record count */
   if( ulRecNo > pArea->ulRecCount && pArea->fShared )
      pArea->ulRecCount = hb_dbfCalcRecCount( pArea );

   if( ulRecNo <= pArea->ulRecCount && ulRecNo >= 1 )
   {
      pArea->ulRecNo = ulRecNo;
      pArea->fBof = pArea->fEof = pArea->fValidBuffer = FALSE;
      pArea->fPositioned = TRUE;
   }
   else /* Out of space */
   {
      pArea->ulRecNo = pArea->ulRecCount + 1;
      pArea->fBof = pArea->fEof = pArea->fValidBuffer = TRUE;
      pArea->fPositioned = pArea->fDeleted = FALSE;

      /* Clear buffer */
      memset( pArea->pRecord, ' ', pArea->uiRecordLen );
   }
   pArea->fFound = FALSE;

   /* Force relational movement in child WorkAreas */
   if( pArea->lpdbRelations )
      return SELF_SYNCCHILDREN( ( AREAP ) pArea );
   else
      return SUCCESS;
}

/*
 * Position the cursor to a specific, physical identity.
 */
static ERRCODE hb_dbfGoToId( DBFAREAP pArea, PHB_ITEM pItem )
{
   PHB_ITEM pError;

   HB_TRACE(HB_TR_DEBUG, ("hb_dbfGoToId(%p, %p)", pArea, pItem));

   if( HB_IS_NUMERIC( pItem ) )
      return SELF_GOTO( ( AREAP ) pArea, hb_itemGetNL( pItem ) );
   else
   {
      pError = hb_errNew();
      hb_errPutGenCode( pError, EG_DATATYPE );
      hb_errPutDescription( pError, hb_langDGetErrorDesc( EG_DATATYPE ) );
      hb_errPutSubCode( pError, EDBF_DATATYPE );
      SELF_ERROR( ( AREAP ) pArea, pError );
      hb_itemRelease( pError );
      return FAILURE;
   }
}

/*
 * Position cursor at the first record.
 */
static ERRCODE hb_dbfGoTop( DBFAREAP pArea )
{
   HB_TRACE(HB_TR_DEBUG, ("hb_dbfGoTop(%p)", pArea));

   pArea->fTop = TRUE;
   pArea->fBottom = FALSE;

   if( hb_dbfGoTo( pArea, 1 ) == FAILURE )
      return FAILURE;

   return SELF_SKIPFILTER( ( AREAP ) pArea, 1 );
}

/*
 * Reposition cursor relative to current position.
 */
static ERRCODE hb_dbfSkip( DBFAREAP pArea, LONG lToSkip )
{
   ERRCODE uiError;

   HB_TRACE(HB_TR_DEBUG, ("hb_dbfSkip(%p, %ld)", pArea, lToSkip));

   if( pArea->lpdbPendingRel )
      SELF_FORCEREL( ( AREAP ) pArea );

   pArea->fTop = pArea->fBottom = FALSE;

   if( lToSkip == 0 || hb_set.HB_SET_DELETED || pArea->dbfi.itmCobExpr )
      return SUPER_SKIP( ( AREAP ) pArea, lToSkip );

   uiError = SELF_SKIPRAW( ( AREAP ) pArea, lToSkip );

   /* Move first record and set Bof flag */
   if( uiError == SUCCESS && pArea->fBof && lToSkip < 0 )
   {
      uiError = SELF_GOTOP( ( AREAP ) pArea );
      pArea->fBof = TRUE;
   }

   /* Update Bof and Eof flags */
   if( lToSkip < 0 )
      pArea->fEof = FALSE;
   else /* if( lToSkip > 0 ) */
      pArea->fBof = FALSE;

   return uiError;
}

/*
 * Reposition cursor, regardless of filter.
 */
static ERRCODE hb_dbfSkipRaw( DBFAREAP pArea, LONG lToSkip )
{
   BOOL bBof, bEof;

   HB_TRACE(HB_TR_DEBUG, ("hb_dbfSkipRaw(%p, %ld)", pArea, lToSkip));

   if( lToSkip == 0 )
   {
      /* Save flags */
      bBof = pArea->fBof;
      bEof = pArea->fEof;

      SELF_GOTO( ( AREAP ) pArea, pArea->ulRecNo );

      /* Restore flags */
      pArea->fBof = bBof;
      pArea->fEof = bEof;
      return SUCCESS;
   }
   else
      /* return SELF_GOTO( ( AREAP ) pArea, pArea->ulRecNo + lToSkip ); */
      return SELF_GOTO( ( AREAP ) pArea, ((LONG)pArea->ulRecNo + lToSkip) <= 0 ? 0 : (pArea->ulRecNo + lToSkip));
}

/*
 * Add a field to the WorkArea.
 */
static ERRCODE hb_dbfAddField( DBFAREAP pArea, LPDBFIELDINFO pFieldInfo )
{
   HB_TRACE(HB_TR_DEBUG, ("hb_dbfAddField(%p, %p)", pArea, pFieldInfo));

   /* Update field offset */
   pArea->pFieldOffset[ pArea->uiFieldCount ] = pArea->uiRecordLen;
   pArea->uiRecordLen += pFieldInfo->uiLen;
   return SUPER_ADDFIELD( ( AREAP ) pArea, pFieldInfo );
}

/*
 * Append a record to the WorkArea.
 */
static ERRCODE hb_dbfAppend( DBFAREAP pArea, BOOL bUnLockAll )
{
   ULONG ulNewRecord;
   PHB_ITEM pError;
   BOOL bLocked;
   ERRCODE uiError;

   HB_TRACE(HB_TR_DEBUG, ("hb_dbfAppend(%p, %d)", pArea, (int) bUnLockAll));

   if( pArea->fReadonly )
   {
      pError = hb_errNew();
      hb_errPutGenCode( pError, EG_READONLY );
      hb_errPutDescription( pError, hb_langDGetErrorDesc( EG_READONLY ) );
      hb_errPutSubCode( pError, EDBF_READONLY );
      SELF_ERROR( ( AREAP ) pArea, pError );
      hb_itemRelease( pError );
      return FAILURE;
   }

   if( SELF_GOCOLD( ( AREAP ) pArea ) == FAILURE )
      return FAILURE;

   /* Reset parent rel struct */
   pArea->lpdbPendingRel = NULL;

   if( pArea->fShared )
   {
      bLocked = FALSE;
      if( SELF_RAWLOCK( ( AREAP ) pArea, APPEND_LOCK, 0 ) == SUCCESS )
      {
         /* Update RecCount */
         pArea->ulRecCount = hb_dbfCalcRecCount( pArea );
         ulNewRecord = pArea->ulRecCount + 1;
         if( pArea->fFLocked || hb_dbfIsLocked( pArea, ulNewRecord ) )
            bLocked = TRUE;
         else
            hb_dbfLockRecord( pArea, ulNewRecord, &bLocked, bUnLockAll );
      }
      if( !bLocked )
      {
         SELF_RAWLOCK( ( AREAP ) pArea, APPEND_UNLOCK, 0 );
         pError = hb_errNew();
         hb_errPutGenCode( pError, EG_APPENDLOCK );
         hb_errPutDescription( pError, hb_langDGetErrorDesc( EG_APPENDLOCK ) );
         hb_errPutSubCode( pError, EDBF_APPENDLOCK );
         hb_errPutFlags( pError, EF_CANDEFAULT );
         SELF_ERROR( ( AREAP ) pArea, pError );
         hb_itemRelease( pError );
         return FAILURE;
      }
   }

   /* Clear buffer and update pArea */
   memset( pArea->pRecord, ' ', pArea->uiRecordLen );
   pArea->fValidBuffer = pArea->fUpdateHeader = pArea->fRecordChanged =
   pArea->fAppend = pArea->fPositioned = TRUE;
   pArea->ulRecCount ++;
   pArea->ulRecNo = pArea->ulRecCount;
   pArea->fDeleted = pArea->fBof = pArea->fEof = pArea->fFound = FALSE;

   if( pArea->fShared )
   {
      uiError = SELF_GOCOLD( ( AREAP ) pArea );
      SELF_RAWLOCK( ( AREAP ) pArea, APPEND_UNLOCK, 0 );
      return uiError;
   }
   return SUCCESS;
}

/*
 * Delete a record.
 */
static ERRCODE hb_dbfDeleteRec( DBFAREAP pArea )
{
   HB_TRACE(HB_TR_DEBUG, ("hb_dbfDeleteRec(%p)", pArea));

   if( pArea->lpdbPendingRel )
      SELF_FORCEREL( ( AREAP ) pArea );

   /* Read record */
   if( !pArea->fValidBuffer && !hb_dbfReadRecord( pArea ) )
      return FAILURE;

   if( !pArea->fPositioned )
      return SUCCESS;

   /* Buffer is hot? */
   if( !pArea->fRecordChanged && SELF_GOHOT( ( AREAP ) pArea ) == FAILURE )
      return FAILURE;

   pArea->pRecord[ 0 ] = '*';
   pArea->fDeleted = TRUE;
   return SUCCESS;
}

/*
 * Determine deleted status for a record.
 */
static ERRCODE hb_dbfDeleted( DBFAREAP pArea, BOOL * pDeleted )
{
   HB_TRACE(HB_TR_DEBUG, ("hb_dbfDeleted(%p, %p)", pArea, pDeleted));

   if( pArea->lpdbPendingRel )
      SELF_FORCEREL( ( AREAP ) pArea );

   /* Read record */
   if( !pArea->fValidBuffer && !hb_dbfReadRecord( pArea ) )
      return FAILURE;

   * pDeleted = pArea->fDeleted;
   return SUCCESS;
}

/*
 * Write data buffer to the data store.
 */
static ERRCODE hb_dbfFlush( DBFAREAP pArea )
{
   ERRCODE uiError;

   HB_TRACE(HB_TR_DEBUG, ("hb_dbfFlush(%p)", pArea));

   uiError = SELF_GOCOLD( ( AREAP ) pArea );
   if( pArea->fUpdateHeader )
   {
      /* Update record count */
      if( pArea->fShared )
         pArea->ulRecCount = hb_dbfCalcRecCount( pArea );
      else
      {
         /* Seek to logical eof and write eof mark */
         hb_fsSeek( pArea->hDataFile, pArea->uiHeaderLen +
                    pArea->uiRecordLen * pArea->ulRecCount, FS_SET );
         hb_fsWrite( pArea->hDataFile, ( BYTE * ) "\032", 1 );
         hb_fsWrite( pArea->hDataFile, NULL, 0 );
      }
      SELF_WRITEDBHEADER( ( AREAP ) pArea );
   }

   hb_fsCommit( pArea->hDataFile );
   if( pArea->fHasMemo && pArea->hMemoFile != FS_ERROR )
      hb_fsCommit( pArea->hMemoFile );
   return uiError;
}

/*
 * Obtain the current value of a field.
 */
static ERRCODE hb_dbfGetValue( DBFAREAP pArea, USHORT uiIndex, PHB_ITEM pItem )
{
   LPFIELD pField;
   char szBuffer[ 256 ];
   BOOL bError;
   PHB_ITEM pError;

   HB_TRACE(HB_TR_DEBUG, ("hb_dbfGetValue(%p, %hu, %p)", pArea, uiIndex, pItem));

   if( pArea->lpdbPendingRel )
      SELF_FORCEREL( ( AREAP ) pArea );

   /* Read record */
   if( !pArea->fValidBuffer && !hb_dbfReadRecord( pArea ) )
      return FAILURE;

   bError = FALSE;
   uiIndex--;
   pField = pArea->lpFields + uiIndex;
   switch( pField->uiType )
   {
      case HB_IT_STRING:
         hb_itemPutCL( pItem, ( char * ) pArea->pRecord + pArea->pFieldOffset[ uiIndex ],
                       pField->uiLen );
         hb_cdpTranslate( pItem->item.asString.value, pArea->cdPage, s_cdpage );
         break;

      case HB_IT_LOGICAL:
         hb_itemPutL( pItem, pArea->pRecord[ pArea->pFieldOffset[ uiIndex ] ] == 'T' ||
                      pArea->pRecord[ pArea->pFieldOffset[ uiIndex ] ] == 't' ||
                      pArea->pRecord[ pArea->pFieldOffset[ uiIndex ] ] == 'Y' ||
                      pArea->pRecord[ pArea->pFieldOffset[ uiIndex ] ] == 'y' );
         break;

      case HB_IT_DATE:
         memcpy( szBuffer, pArea->pRecord + pArea->pFieldOffset[ uiIndex ], 8 );
         szBuffer[ 8 ] = 0;
         hb_itemPutDS( pItem, szBuffer );
         break;

      case HB_IT_LONG:
         /* DBASE documentation defines maximum numeric field size as 20
          * but Clipper alows to create longer fileds so I remove this
          * limit, Druzus
          */
         /*
         if( pField->uiLen > 20 )
            bError = TRUE;
         else
         */
         {
            memcpy( szBuffer, pArea->pRecord + pArea->pFieldOffset[ uiIndex ],
                    pField->uiLen );
            szBuffer[ pField->uiLen ] = 0;
            if( pField->uiDec )
               hb_itemPutNDLen( pItem, atof( szBuffer ),
                                ( int ) pField->uiLen - ( ( int ) pField->uiDec + 1 ),
                                ( int ) pField->uiDec );
            else
               if( pField->uiLen > 9 )
                  hb_itemPutNDLen( pItem, atof( szBuffer ),
                                   ( int ) pField->uiLen, ( int ) pField->uiDec );
               else
                  hb_itemPutNLLen( pItem, atol( szBuffer ), ( int ) pField->uiLen );
         }
         break;

      case HB_IT_MEMO:
      default:
         bError = TRUE;
         break;
   }

   /* Any error? */
   if( bError )
   {
      pError = hb_errNew();
      hb_errPutGenCode( pError, EG_DATATYPE );
      hb_errPutDescription( pError, hb_langDGetErrorDesc( EG_DATATYPE ) );
      hb_errPutSubCode( pError, EDBF_DATATYPE );
      SELF_ERROR( ( AREAP ) pArea, pError );
      hb_itemRelease( pError );
      return FAILURE;
   }
   return SUCCESS;
}

/*
 * Obtain the length of a field value.
 */
static ERRCODE hb_dbfGetVarLen( DBFAREAP pArea, USHORT uiIndex, ULONG * pLength )
{
   HB_TRACE(HB_TR_DEBUG, ("hb_dbfGetVarLen(%p, %hu, %p)", pArea, uiIndex, pLength));

   if( pArea->lpdbPendingRel )
      SELF_FORCEREL( ( AREAP ) pArea );

   /* Read record */
   if( !pArea->fValidBuffer && !hb_dbfReadRecord( pArea ) )
      return FAILURE;

   * pLength = pArea->lpFields[ uiIndex - 1 ].uiLen;

   return SUCCESS;
}

/*
 * Perform a write of WorkArea memory to the data store.
 */
static ERRCODE hb_dbfGoCold( DBFAREAP pArea )
{
   HB_TRACE(HB_TR_DEBUG, ("hb_dbfGoCold(%p)", pArea));

   if( pArea->fRecordChanged )
   {
      /* Write current record */
      if( !hb_dbfWriteRecord( pArea ) )
         return FAILURE;

      pArea->fRecordChanged = FALSE;

      if( pArea->fAppend )
      {
         pArea->fUpdateHeader = TRUE;
         pArea->fAppend = FALSE;
      }

      /* Update header */
      if( pArea->fShared && pArea->fUpdateHeader )
         SELF_WRITEDBHEADER( ( AREAP ) pArea );
   }
   return SUCCESS;
}

/*
 * Mark the WorkArea data buffer as hot.
 */
static ERRCODE hb_dbfGoHot( DBFAREAP pArea )
{
   PHB_ITEM pError;

   HB_TRACE(HB_TR_DEBUG, ("hb_dbfGoHot(%p)", pArea));

   if( pArea->fReadonly )
   {
      pError = hb_errNew();
      hb_errPutGenCode( pError, EG_READONLY );
      hb_errPutDescription( pError, hb_langDGetErrorDesc( EG_READONLY ) );
      hb_errPutSubCode( pError, EDBF_READONLY );
      SELF_ERROR( ( AREAP ) pArea, pError );
      hb_itemRelease( pError );
      return FAILURE;
   }
   else if( pArea->fShared && !pArea->fFLocked &&
            !hb_dbfIsLocked( pArea, pArea->ulRecNo ) )
   {
      pError = hb_errNew();
      hb_errPutGenCode( pError, EG_UNLOCKED );
      hb_errPutDescription( pError, hb_langDGetErrorDesc( EG_UNLOCKED ) );
      hb_errPutSubCode( pError, EDBF_UNLOCKED );
      SELF_ERROR( ( AREAP ) pArea, pError );
      hb_itemRelease( pError );
      return FAILURE;
   }
   pArea->fRecordChanged = TRUE;
   return SUCCESS;
}

/*
 * Replace the current record.
 */
static ERRCODE hb_dbfPutRec( DBFAREAP pArea, BYTE * pBuffer )
{
   HB_TRACE(HB_TR_DEBUG, ("hb_dbfPutRec(%p, %p)", pArea, pBuffer));

   if( !pArea->fRecordChanged && SELF_GOHOT( ( AREAP ) pArea ) == FAILURE )
      return FAILURE;

   /* Copy data to buffer */
   memcpy( pArea->pRecord, pBuffer, pArea->uiRecordLen );
   return SUCCESS;
}

/*
 * Assign a value to a field.
 */
static ERRCODE hb_dbfPutValue( DBFAREAP pArea, USHORT uiIndex, PHB_ITEM pItem )
{
   USHORT uiSize;
   LPFIELD pField;
   /* this buffer is for date and number conversion,
    * DBASE documentation defines maximum numeric field size as 20
    * but Clipper alows to create longer fileds so I remove this limit, Druzus
    */
   char szBuffer[ 256 ];
   PHB_ITEM pError;
   ERRCODE uiError;

   HB_TRACE(HB_TR_DEBUG, ("hb_dbfPutValue(%p, %hu, %p)", pArea, uiIndex, pItem));

   if( pArea->lpdbPendingRel )
      SELF_FORCEREL( ( AREAP ) pArea );

   /* Read record */
   if( !pArea->fValidBuffer && !hb_dbfReadRecord( pArea ) )
      return FAILURE;

   if( !pArea->fPositioned )
      return SUCCESS;

   /* Buffer is hot? */
   if( !pArea->fRecordChanged && SELF_GOHOT( ( AREAP ) pArea ) == FAILURE )
      return FAILURE;

   uiError = SUCCESS;
   uiIndex--;
   pField = pArea->lpFields + uiIndex;
   if( pField->uiType == HB_IT_MEMO )
      uiError = EDBF_DATATYPE;
   else
   {
      if( HB_IS_MEMO( pItem ) || HB_IS_STRING( pItem ) )
      {
         if( pField->uiType == HB_IT_STRING )
         {
            uiSize = ( USHORT ) hb_itemGetCLen( pItem );
            if( uiSize > pField->uiLen )
               uiSize = pField->uiLen;
            memcpy( pArea->pRecord + pArea->pFieldOffset[ uiIndex ],
                    hb_itemGetCPtr( pItem ), uiSize );
            if( HB_IS_STRING( pItem ) )
               hb_cdpnTranslate( (char *) pArea->pRecord + pArea->pFieldOffset[ uiIndex ], s_cdpage, pArea->cdPage, uiSize );
            memset( pArea->pRecord + pArea->pFieldOffset[ uiIndex ] + uiSize,
                    ' ', pField->uiLen - uiSize );
         }
         else
            uiError = EDBF_DATATYPE;
      }
      // Must precede HB_IS_NUMERIC() because a DATE is also a NUMERIC. (xHarbour)
      else if( HB_IS_DATE( pItem ) )
      {
         if( pField->uiType == HB_IT_DATE )
         {
            hb_itemGetDS( pItem, szBuffer );
            memcpy( pArea->pRecord + pArea->pFieldOffset[ uiIndex ], szBuffer, 8 );
         }
         else
            uiError = EDBF_DATATYPE;
      }
      else if( HB_IS_NUMERIC( pItem ) )
      {
         if( pField->uiType == HB_IT_LONG )
         {
            if ( hb_itemStrBuf( szBuffer, pItem, pField->uiLen, pField->uiDec ) )
            {
               memcpy( pArea->pRecord + pArea->pFieldOffset[ uiIndex ],
                       szBuffer, pField->uiLen );
            }
            else
            {
               uiError = EDBF_DATAWIDTH;
               memset( pArea->pRecord + pArea->pFieldOffset[ uiIndex ],
                       '*', pField->uiLen );
            }
         }
         else
            uiError = EDBF_DATATYPE;
      }
      else if( HB_IS_LOGICAL( pItem ) )
      {
         if( pField->uiType == HB_IT_LOGICAL )
            pArea->pRecord[ pArea->pFieldOffset[ uiIndex ] ] = hb_itemGetL( pItem ) ? 'T' : 'F';
         else
            uiError = EDBF_DATATYPE;
      }
      else
         uiError = EDBF_DATATYPE;
   }

   /* Exit if any error */
   if( uiError != SUCCESS )
   {
      pError = hb_errNew();
      hb_errPutGenCode( pError, hb_dbfGetEGcode( uiError ) );
      hb_errPutDescription( pError, hb_langDGetErrorDesc( hb_dbfGetEGcode( uiError ) ) );
      hb_errPutSubCode( pError, uiError );
      hb_errPutFlags( pError, EF_CANDEFAULT );
      SELF_ERROR( ( AREAP ) pArea, pError );
      hb_itemRelease( pError );
      return FAILURE;
   }

   /* Update deleted flag */
   pArea->pRecord[ 0 ] = pArea->fDeleted ? '*' : ' ';
   return SUCCESS;
}

/*
 * Undelete the current record.
 */
static ERRCODE hb_dbfRecall( DBFAREAP pArea )
{
   HB_TRACE(HB_TR_DEBUG, ("hb_dbfRecall(%p)", pArea));

   if( pArea->lpdbPendingRel )
      SELF_FORCEREL( ( AREAP ) pArea );

   /* Read record */
   if( !pArea->fValidBuffer && !hb_dbfReadRecord( pArea ) )
      return FAILURE;

   if( !pArea->fPositioned )
      return SUCCESS;

   /* Buffer is hot? */
   if( !pArea->fRecordChanged && SELF_GOHOT( ( AREAP ) pArea ) == FAILURE )
      return FAILURE;

   pArea->pRecord[ 0 ] = ' ';
   pArea->fDeleted = FALSE;
   return SUCCESS;
}

/*
 * Obtain number of records in WorkArea.
 */
static ERRCODE hb_dbfRecCount( DBFAREAP pArea, ULONG * pRecCount )
{
   HB_TRACE(HB_TR_DEBUG, ("hb_dbfRecCount(%p, %p)", pArea, pRecCount));

   /* Update record count */
   if( pArea->fShared )
      pArea->ulRecCount = hb_dbfCalcRecCount( pArea );

   * pRecCount = pArea->ulRecCount;
   return SUCCESS;
}

/*
 * Obtain physical row number at current WorkArea cursor position.
 */
static ERRCODE hb_dbfRecNo( DBFAREAP pArea, PHB_ITEM pRecNo )
{
   HB_TRACE(HB_TR_DEBUG, ("hb_dbfRecNo(%p, %p)", pArea, pRecNo));

   if( pArea->lpdbPendingRel )
      SELF_FORCEREL( ( AREAP ) pArea );
   hb_itemPutNL( pRecNo, pArea->ulRecNo );
   return SUCCESS;
}

/*
 * Establish the extent of the array of fields for a WorkArea.
 */
static ERRCODE hb_dbfSetFieldExtent( DBFAREAP pArea, USHORT uiFieldExtent )
{
   HB_TRACE(HB_TR_DEBUG, ("hb_dbfSetFieldExtent(%p, %hu)", pArea, uiFieldExtent));

   if( SUPER_SETFIELDEXTENT( ( AREAP ) pArea, uiFieldExtent ) == FAILURE )
      return FAILURE;

   /* Alloc field offsets array */
   pArea->pFieldOffset = ( USHORT * ) hb_xgrab( uiFieldExtent * sizeof( USHORT * ) );
   memset( pArea->pFieldOffset, 0, uiFieldExtent * sizeof( USHORT * ) );
   return SUCCESS;
}

/*
 * Close the table in the WorkArea.
 */
static ERRCODE hb_dbfClose( DBFAREAP pArea )
{
   HB_TRACE(HB_TR_DEBUG, ("hb_dbfClose(%p)", pArea));
   /* Reset parent rel struct */
   pArea->lpdbPendingRel = NULL;

   SUPER_CLOSE( ( AREAP ) pArea );

   /* Update record and unlock records */
   if( pArea->hDataFile != FS_ERROR )
   {
      SELF_GOCOLD( ( AREAP ) pArea );

      /* Unlock all records */
      SELF_UNLOCK( ( AREAP ) pArea, 0 );

      /* Update header */
      if( pArea->fUpdateHeader )
      {
         /* Update record count */
         if( pArea->fShared )
            pArea->ulRecCount = hb_dbfCalcRecCount( pArea );

         /* hb_dbfWriteDBHeader( pArea ); */
         SELF_WRITEDBHEADER( ( AREAP ) pArea );

         /* Seek to logical eof and write eof mark */
         hb_fsSeek( pArea->hDataFile, pArea->uiHeaderLen +
                    pArea->uiRecordLen * pArea->ulRecCount, FS_SET );
         hb_fsWrite( pArea->hDataFile, ( BYTE * ) "\032", 1 );
         hb_fsWrite( pArea->hDataFile, NULL, 0 );
      }
   }

   /* Close the data file */
   if( pArea->hDataFile != FS_ERROR )
   {
      hb_fsCommit( pArea->hDataFile );
      hb_fsClose( pArea->hDataFile );
      pArea->hDataFile = FS_ERROR;
   }

   /* Close the memo file */
   if( pArea->fHasMemo && pArea->hMemoFile != FS_ERROR )
   {
      hb_fsCommit( pArea->hMemoFile );
      hb_fsClose( pArea->hMemoFile );
      pArea->hMemoFile = FS_ERROR;
   }

   /* Free field offset array */
   if( pArea->pFieldOffset )
   {
      hb_xfree( pArea->pFieldOffset );
      pArea->pFieldOffset = NULL;
   }

   /* Free buffer */
   if( pArea->pRecord )
   {
      hb_xfree( pArea->pRecord );
      pArea->pRecord = NULL;
   }

   /* Free all filenames */
   if( pArea->szDataFileName )
   {
      hb_xfree( pArea->szDataFileName );
      pArea->szDataFileName = NULL;
   }
   if( pArea->szMemoFileName )
   {
      hb_xfree( pArea->szMemoFileName );
      pArea->szMemoFileName = NULL;
   }

   return SUCCESS;
}

/*
 * Create a data store in the specified WorkArea.
 */
static ERRCODE hb_dbfCreate( DBFAREAP pArea, LPDBOPENINFO pCreateInfo )
{
   USHORT uiSize, uiCount;
   BOOL bHasMemo, bRetry;
   DBFFIELD * pBuffer, dbField;
   PHB_FNAME pFileName;
   PHB_ITEM pFileExt, pError;

   HB_TRACE(HB_TR_DEBUG, ("hb_dbfCreate(%p, %p)", pArea, pCreateInfo));

   pError = NULL;
   /* Try create */
   do
   {
      pArea->hDataFile = hb_spCreate( pCreateInfo->abName, FC_NORMAL );
      if( pArea->hDataFile == FS_ERROR )
      {
         if( !pError )
         {
            pError = hb_errNew();
            hb_errPutGenCode( pError, EG_CREATE );
            hb_errPutSubCode( pError, EDBF_CREATE_DBF );
            hb_errPutDescription( pError, hb_langDGetErrorDesc( EG_CREATE ) );
            hb_errPutFileName( pError, ( char * ) pCreateInfo->abName );
            hb_errPutFlags( pError, EF_CANRETRY );
         }
         bRetry = ( SELF_ERROR( ( AREAP ) pArea, pError ) == E_RETRY );
      }
      else
         bRetry = FALSE;
   } while( bRetry );

   if( pError )
   {
      hb_itemRelease( pError );
   }

   if( pArea->hDataFile == FS_ERROR )
   {
      return FAILURE;
   }

   pArea->szDataFileName = (char *) hb_xgrab( strlen( (char * ) pCreateInfo->abName)+1 );
   strcpy( pArea->szDataFileName, ( char * ) pCreateInfo->abName );
   uiSize = pArea->uiFieldCount * sizeof( DBFFIELD );

   pBuffer = ( DBFFIELD * ) hb_xgrab( uiSize );
   pArea->uiRecordLen = 1;
   bHasMemo = FALSE;
   memset( &dbField, 0, sizeof( DBFFIELD ) );
   for( uiCount = 0; uiCount < pArea->uiFieldCount; uiCount++ )
   {
      strncpy( ( char * ) dbField.bName,
               ( ( PHB_DYNS ) pArea->lpFields[ uiCount ].sym )->pSymbol->szName, 10 );
      switch( pArea->lpFields[ uiCount ].uiType )
      {
         case HB_IT_STRING:
            dbField.bType = 'C';
            dbField.bLen = ( BYTE ) pArea->lpFields[ uiCount ].uiLen;
            dbField.bDec = ( BYTE ) ( pArea->lpFields[ uiCount ].uiLen / 256 );
            pArea->uiRecordLen += pArea->lpFields[ uiCount ].uiLen;
            break;

         case HB_IT_LOGICAL:
            dbField.bType = 'L';
            dbField.bLen = 1;
            dbField.bDec = 0;
            pArea->uiRecordLen ++;
            break;

         case HB_IT_MEMO:
            dbField.bType = 'M';
            dbField.bLen = 10;
            dbField.bDec = 0;
            pArea->uiRecordLen += 10;
            bHasMemo = TRUE;
            break;

         case HB_IT_DATE:
            dbField.bType = 'D';
            dbField.bLen = 8;
            dbField.bDec = 0;
            pArea->uiRecordLen += 8;
            break;

         case HB_IT_LONG:
            dbField.bType = 'N';
            dbField.bLen = ( BYTE ) pArea->lpFields[ uiCount ].uiLen;
            dbField.bDec = ( BYTE ) pArea->lpFields[ uiCount ].uiDec;
            pArea->uiRecordLen += pArea->lpFields[ uiCount ].uiLen;
            break;

         default:
            hb_xfree( pBuffer );
            return FAILURE;
      }
      pBuffer[ uiCount ] = dbField;
   }

   pArea->fShared = pCreateInfo->fShared;
   pArea->ulRecCount = 0;
   pArea->uiHeaderLen = sizeof( DBFHEADER ) + uiSize + 2;
   pArea->fHasMemo = bHasMemo;
   pArea->uiMemoBlockSize = 0;
   pArea->bVersion = 0x03;

   /* Write header */
   if( SELF_WRITEDBHEADER( ( AREAP ) pArea ) == FAILURE )
   {
      hb_fsClose( pArea->hDataFile );
      pArea->hDataFile = FS_ERROR;
      hb_xfree( pBuffer );
      return FAILURE;
   }

   /* Write fields and eof mark */
   if( hb_fsWrite( pArea->hDataFile, ( BYTE * ) pBuffer, uiSize ) != uiSize ||
       hb_fsWrite( pArea->hDataFile, ( BYTE * ) "\r\0\032", 3 ) != 3 )
   {
      hb_fsClose( pArea->hDataFile );
      pArea->hDataFile = FS_ERROR;
      hb_xfree( pBuffer );
      return FAILURE;
   }
   hb_xfree( pBuffer );

   /* Create memo file */
   if( bHasMemo )
   {
      BYTE *tmp;
      ERRCODE result;
      pArea->szMemoFileName = ( char * ) hb_xgrab( _POSIX_PATH_MAX + 1 );
      pFileName = hb_fsFNameSplit( ( char * ) pCreateInfo->abName );
      pArea->szMemoFileName[ 0 ] = 0;
      if( pFileName->szPath )
         strcat( pArea->szMemoFileName, pFileName->szPath );
      strcat( pArea->szMemoFileName, pFileName->szName );
      hb_xfree( pFileName );
      pFileExt = hb_itemPutC( NULL, "" );
      SELF_INFO( ( AREAP ) pArea, DBI_MEMOEXT, pFileExt );
      strncat( pArea->szMemoFileName, pFileExt->item.asString.value,
               _POSIX_PATH_MAX - strlen( pArea->szMemoFileName ) );
      hb_itemRelease( pFileExt );
      tmp = pCreateInfo->abName;
      pCreateInfo->abName = ( BYTE * ) pArea->szMemoFileName;
      result = SELF_CREATEMEMFILE( ( AREAP ) pArea, pCreateInfo );
      pCreateInfo->abName = tmp;
      return result;
   }
   else
      return SUCCESS;
}

/*
 * Retrieve information about the current driver.
 */
static ERRCODE hb_dbfInfo( DBFAREAP pArea, USHORT uiIndex, PHB_ITEM pItem )
{
   HB_TRACE(HB_TR_DEBUG, ("hb_dbfInfo(%p, %hu, %p)", pArea, uiIndex, pItem));

   switch( uiIndex )
   {
      case DBI_ISDBF:
      case DBI_CANPUTREC:
         hb_itemPutL( pItem, TRUE );
         break;

      case DBI_GETHEADERSIZE:
         hb_itemPutNL( pItem, pArea->uiHeaderLen );
         break;

      case DBI_LASTUPDATE:
         hb_itemPutD( pItem, 1900 + pArea->bYear, pArea->bMonth, pArea->bDay );
         break;

      case DBI_GETDELIMITER:
      case DBI_SETDELIMITER:
         break;

      case DBI_GETRECSIZE:
         hb_itemPutNL( pItem, pArea->uiRecordLen );
         break;

      case DBI_GETLOCKARRAY:
         hb_dbfGetLockArray( pArea, pItem );
         break;

      case DBI_TABLEEXT:
         hb_itemPutC( pItem, DBF_TABLEEXT );
         break;

      case DBI_MEMOEXT:
         hb_itemPutC( pItem, "" );
         break;

      case DBI_MEMOBLOCKSIZE:
         hb_itemPutNI( pItem, pArea->uiMemoBlockSize );
         break;

      case DBI_FULLPATH:
         hb_itemPutC( pItem, pArea->szDataFileName);
         break;

      case DBI_SHARED:
         hb_itemPutL( pItem, pArea->fShared );
         break;

      case DBI_FILEHANDLE:
         hb_itemPutNL( pItem, (LONG)pArea->hDataFile );
         break;

      case DBI_MEMOHANDLE:
         hb_itemPutNL( pItem, (LONG)pArea->hMemoFile );
         break;

   }

   return SUCCESS;
}

/*
 * Clear the WorkArea for use.
 */
static ERRCODE hb_dbfNewArea( DBFAREAP pArea )
{
   HB_TRACE(HB_TR_DEBUG, ("hb_dbfNewArea(%p)", pArea));

   if( SUPER_NEW( ( AREAP ) pArea ) == FAILURE )
      return FAILURE;

   pArea->hDataFile = pArea->hMemoFile = FS_ERROR;

   /* Size for deleted records flag */
   pArea->uiRecordLen = 1;
   return SUCCESS;
}

/*
 * Open a data store in the WorkArea.
 */
static ERRCODE hb_dbfOpen( DBFAREAP pArea, LPDBOPENINFO pOpenInfo )
{
   USHORT uiFlags, uiFields, uiSize, uiCount;
   BOOL bRetry, bError, bLock;
   PHB_ITEM pError, pFileExt;
   PHB_FNAME pFileName;
   char * szFileName;
   BYTE * pBuffer;
   LPDBFFIELD pField;
   DBFIELDINFO pFieldInfo;

   HB_TRACE(HB_TR_DEBUG, ("hb_dbfOpen(%p, %p)", pArea, pOpenInfo));

   pArea->atomAlias = hb_dynsymGet( ( char * ) pOpenInfo->atomAlias );

   if( ( ( PHB_DYNS ) pArea->atomAlias )->hArea )
   {
      hb_errRT_DBCMD( EG_DUPALIAS, EDBCMD_DUPALIAS, NULL, ( char * ) pOpenInfo->atomAlias );
      return FAILURE;
   }

   if ( !pArea->bLockType )
   {
      if ( hb_set.HB_SET_DBFLOCKSCHEME )
         pArea->bLockType = hb_set.HB_SET_DBFLOCKSCHEME;
      else
         pArea->bLockType = HB_SET_DBFLOCK_CLIP;
   }
   ( ( PHB_DYNS ) pArea->atomAlias )->hArea = pOpenInfo->uiArea;
   if( pOpenInfo->cdpId )
   {
      pArea->cdPage = hb_cdpFind( (char *) pOpenInfo->cdpId );
      if( !pArea->cdPage )
         pArea->cdPage = s_cdpage;
   }
   else
      pArea->cdPage = s_cdpage;
   pArea->fShared = pOpenInfo->fShared;
   pArea->fReadonly = pOpenInfo->fReadonly;
   /* Force exclusive mode
    *   0: AUTOSHARE disabled.
    *   1: AUTOSHARE enabled.
    *   2: force exclusive mode.
    * */
   if( hb_set.HB_SET_AUTOSHARE == 2 )
      pArea->fShared = FALSE;
   uiFlags = (pArea->fReadonly ? FO_READ : FO_READWRITE) |
             (pArea->fShared ? FO_DENYNONE : FO_EXCLUSIVE);
   pError = NULL;

   /* Try open */
   do
   {
      bLock = FALSE;
      pArea->hDataFile = hb_spOpen( pOpenInfo->abName, uiFlags );
#ifdef DBF_EXLUSIVE_LOCKPOS
      if( pArea->hDataFile != FS_ERROR )
      {
         if ( !hb_fsLock( pArea->hDataFile, DBF_EXLUSIVE_LOCKPOS, DBF_EXLUSIVE_LOCKSIZE,
                          FL_LOCK | ( pArea->fShared ? FLX_SHARED : FLX_EXCLUSIVE ) ) )
         {
            hb_fsClose( pArea->hDataFile );
            pArea->hDataFile = FS_ERROR;
	       bLock = TRUE;
         }
      }
#endif
      if( pArea->hDataFile == FS_ERROR )
      {
         if( !pError )
         {
            pError = hb_errNew();
            hb_errPutGenCode( pError, EG_OPEN );
            hb_errPutSubCode( pError, EDBF_OPEN_DBF );
            hb_errPutDescription( pError, hb_langDGetErrorDesc( EG_OPEN ) );
            hb_errPutFileName( pError, ( char * ) pOpenInfo->abName );
            /*
             * Temporary fix for neterr() support and Clipper compatibility,
             * should be revised with a better solution.
             */
            if ( hb_fsError() == EACCES || bLock )
               hb_errPutOsCode( pError, 32 );
            else
               hb_errPutOsCode( pError, hb_fsError() );
            hb_errPutFlags( pError, EF_CANRETRY | EF_CANDEFAULT );
         }
         bRetry = ( SELF_ERROR( ( AREAP ) pArea, pError ) == E_RETRY );
      }
      else
         bRetry = FALSE;
   } while( bRetry );
   if( pError )
   {
      hb_itemRelease( pError );
      pError = NULL;
   }

   /* Exit if error */
   if( pArea->hDataFile == FS_ERROR )
   {
      pArea->szDataFileName = NULL;
      return FAILURE;
   }

   /* Allocate only after succesfully open file */
   pArea->szDataFileName = (char *) hb_xgrab( strlen( (char * ) pOpenInfo->abName)+1 );
   strcpy( pArea->szDataFileName, ( char * ) pOpenInfo->abName );

   /* Read file header and exit if error */
   if( SELF_READDBHEADER( ( AREAP ) pArea ) == FAILURE )
   {
      SELF_CLOSE( ( AREAP ) pArea );
      return FAILURE;
   }

   /* Open memo file if exists */
   if( pArea->fHasMemo )
   {
      BYTE *tmp;
      pFileName = hb_fsFNameSplit( ( char * ) pOpenInfo->abName );
      pFileExt = hb_itemPutC( NULL, "" );
      SELF_INFO( ( AREAP ) pArea, DBI_MEMOEXT, pFileExt );
      szFileName = ( char * ) hb_xgrab( _POSIX_PATH_MAX + 1 );
      szFileName[ 0 ] = 0;
      if( pFileName->szPath )
         strcat( szFileName, pFileName->szPath );
      strcat( szFileName, pFileName->szName );
      strncat( szFileName, hb_itemGetCPtr( pFileExt ), _POSIX_PATH_MAX -
               strlen( szFileName ) );
      hb_itemRelease( pFileExt );
      hb_xfree( pFileName );
      tmp = pOpenInfo->abName;
      pOpenInfo->abName = ( BYTE * ) szFileName;
      pArea->szMemoFileName = szFileName;

      /* Open memo file and exit if error */
      if( SELF_OPENMEMFILE( ( AREAP ) pArea, pOpenInfo ) == FAILURE )
      {
         pOpenInfo->abName = tmp;
         SELF_CLOSE( ( AREAP ) pArea );
         return FAILURE;
      }
      pOpenInfo->abName = tmp;
   }

   /* Add fields */
   uiFields = ( pArea->uiHeaderLen - sizeof( DBFHEADER ) ) / sizeof( DBFFIELD );
   SELF_SETFIELDEXTENT( ( AREAP ) pArea, uiFields );
   uiSize = uiFields * sizeof( DBFFIELD );
   pBuffer = ( BYTE * ) hb_xgrab( uiSize );

   /* Read fields and exit if error */
   do
   {
      hb_fsSeek( pArea->hDataFile, sizeof( DBFHEADER ), FS_SET );
      if( hb_fsRead( pArea->hDataFile, pBuffer, uiSize ) != uiSize )
      {
         bError = TRUE;
         if( !pError )
         {
            pError = hb_errNew();
            hb_errPutGenCode( pError, EG_CORRUPTION );
            hb_errPutSubCode( pError, EDBF_CORRUPT );
            hb_errPutDescription( pError, hb_langDGetErrorDesc( EG_CORRUPTION ) );
            hb_errPutFileName( pError, pArea->szDataFileName );
            hb_errPutFlags( pError, EF_CANRETRY | EF_CANDEFAULT );
         }
         bRetry = ( SELF_ERROR( ( AREAP ) pArea, pError ) == E_RETRY );
      }
      else
         bRetry = bError = FALSE;
   } while( bRetry );
   if( pError )
   {
      hb_itemRelease( pError );
      pError = NULL;
   }

   /* Exit if error */
   if( bError )
   {
      hb_xfree( pBuffer );
      SELF_CLOSE( ( AREAP ) pArea );
      return FAILURE;
   }

   /* Size for deleted flag */
   pArea->uiRecordLen = 1;

   pFieldInfo.uiTypeExtended = 0;
   for( uiCount = 0; uiCount < uiFields; uiCount++ )
   {
      pField = ( LPDBFFIELD ) ( pBuffer + uiCount * sizeof( DBFFIELD ) );
      pFieldInfo.atomName = pField->bName;
      pFieldInfo.atomName[10] = '\0';
      hb_strUpper( (char *) pFieldInfo.atomName, 10 );
      pFieldInfo.uiLen = pField->bLen;
      pFieldInfo.uiDec = 0;
      switch( pField->bType )
      {
         case 'C':
            pFieldInfo.uiType = HB_IT_STRING;
            pFieldInfo.uiLen = pField->bLen + pField->bDec * 256;
            break;

         case 'L':
            pFieldInfo.uiType = HB_IT_LOGICAL;
            pFieldInfo.uiLen = 1;
            break;

         case 'M':
            pFieldInfo.uiType = HB_IT_MEMO;
            pFieldInfo.uiLen = 10;
            break;

         case 'D':
            pFieldInfo.uiType = HB_IT_DATE;
            pFieldInfo.uiLen = 8;
            break;

         case 'N':
            pFieldInfo.uiType = HB_IT_LONG;
            if( pField->bLen > 20 )
               bError = TRUE;
            else
               pFieldInfo.uiDec = pField->bDec;
            break;

         default:
            bError = TRUE;
            break;
      }
      /* Add field */
      if( !bError )
         bError = ( SELF_ADDFIELD( ( AREAP ) pArea, &pFieldInfo ) == FAILURE );

      /* Exit if error */
      if( bError )
         break;
   }
   hb_xfree( pBuffer );

   /* Exit if error */
   if( bError )
   {
      if( !pError )
      {
         pError = hb_errNew();
         hb_errPutGenCode( pError, EG_CORRUPTION );
         hb_errPutSubCode( pError, EDBF_CORRUPT );
         hb_errPutDescription( pError, hb_langDGetErrorDesc( EG_CORRUPTION ) );
         hb_errPutFileName( pError, pArea->szDataFileName );
         hb_errPutFlags( pError, EF_CANRETRY | EF_CANDEFAULT );
      }
      SELF_ERROR( ( AREAP ) pArea, pError );
      hb_itemRelease( pError );
      SELF_CLOSE( ( AREAP ) pArea );
      return FAILURE;
   }

   /* Alloc buffer */
   pArea->pRecord = ( BYTE * ) hb_xgrab( pArea->uiRecordLen );
   pArea->fValidBuffer = FALSE;

   /* Update the number of record for corrupted headers */
   pArea->ulRecCount = hb_dbfCalcRecCount( pArea );

   /* Position cursor at the first record */
   return SELF_GOTOP( ( AREAP ) pArea );
}

/*
 * Retrieve the size of the WorkArea structure.
 */
static ERRCODE hb_dbfStructSize( DBFAREAP pArea, USHORT * uiSize )
{
   HB_TRACE(HB_TR_DEBUG, ("hb_dbfStrucSize(%p, %p)", pArea, uiSize));
   HB_SYMBOL_UNUSED( pArea );

   * uiSize = sizeof( DBFAREA );
   return SUCCESS;
}

/*
 * Obtain the name of replaceable database driver (RDD) subsystem.
 */
static ERRCODE hb_dbfSysName( DBFAREAP pArea, BYTE * pBuffer )
{
   HB_TRACE(HB_TR_DEBUG, ("hb_dbfSysName(%p, %p)", pArea, pBuffer));
   HB_SYMBOL_UNUSED( pArea );

   strncpy( ( char * ) pBuffer, "DBF", 4 );
   return SUCCESS;
}

/*
 * Remove records marked for deletion from a database.
 */
static ERRCODE hb_dbfPack( DBFAREAP pArea )
{
   ULONG ulRecIn, ulRecOut, ulEvery, ulUserEvery;
   PHB_ITEM pError, pBlock;

   HB_TRACE(HB_TR_DEBUG, ("hb_dbfPack(%p)", pArea));

   if( pArea->fShared )
   {
      pError = hb_errNew();
      hb_errPutGenCode( pError, EG_SHARED );
      hb_errPutDescription( pError, hb_langDGetErrorDesc( EG_SHARED ) );
      hb_errPutSubCode( pError, EDBF_SHARED );
      SELF_ERROR( ( AREAP ) pArea, pError );
      hb_itemRelease( pError );
      return FAILURE;
   }

   if( SELF_GOCOLD( ( AREAP ) pArea ) == FAILURE )
      return FAILURE;

   if( HB_IS_ARRAY( pArea->valResult ) && hb_arrayLen( pArea->valResult ) == 2 )
   {
      pBlock = hb_arrayGetItemPtr( pArea->valResult, 1 );
      ulUserEvery = hb_arrayGetNL( pArea->valResult, 2 );
      if( ulUserEvery < 1 )
         ulUserEvery = 1;
   }
   else
   {
      pBlock = NULL;
      ulUserEvery = 1;
   }

   /* Force write new header */
   pArea->fUpdateHeader = TRUE;
   ulRecOut = ulEvery = 0;
   ulRecIn = 1;
   while ( ulRecIn <= pArea->ulRecCount )
   {
      hb_dbfGoTo( pArea, ulRecIn );
      hb_dbfReadRecord( pArea );

      /* Execute the Code Block */
      if( pBlock )
      {
         ulEvery ++;
         if( ulEvery >= ulUserEvery )
         {
            ulEvery = 0;
            hb_vmPushSymbol( &hb_symEval );
            hb_vmPush( pBlock );
            hb_vmDo( 0 );
         }
      }

      /* Copy record */
      if( !pArea->fDeleted )
      {
         ulRecOut++;
         if( ulRecIn != ulRecOut )
         {
            pArea->ulRecNo = ulRecOut;
            hb_dbfWriteRecord( pArea );
            pArea->fRecordChanged = FALSE;
         }
      }
      ulRecIn++;
   }

   /* Execute the Code Block for pending record */
   if( pBlock && ulEvery > 0 )
   {
      hb_vmPushSymbol( &hb_symEval );
      hb_vmPush( pBlock );
      hb_vmDo( 0 );
   }

   pArea->ulRecCount = ulRecOut;
   return hb_dbfGoTo( pArea, 1 );
}

/*
 * Physically reorder a database.
 */
static ERRCODE hb_dbfSort( DBFAREAP pArea, LPDBSORTINFO pSortInfo )
{
   ULONG ulRecNo;
   USHORT uiCount;
   BOOL bMoreRecords, bLimited, bValidRecord;
   ERRCODE uiError;
   DBQUICKSORT dbQuickSort;
   BYTE * pBuffer;

   HB_TRACE(HB_TR_DEBUG, ("hb_dbfSort(%p, %p)", pArea, pSortInfo));

   if( SELF_GOCOLD( ( AREAP ) pArea ) == FAILURE )
      return FAILURE;

   if( !hb_dbQSortInit( &dbQuickSort, pSortInfo, pArea->uiRecordLen ) )
      return FAILURE;

   uiError = SUCCESS;
   uiCount = 0;
   pBuffer = dbQuickSort.pBuffer;
   ulRecNo = 1;
   if( pSortInfo->dbtri.dbsci.itmRecID )
   {
      uiError = hb_dbfGoTo( pArea, hb_itemGetNL( pSortInfo->dbtri.dbsci.itmRecID ) );
      bMoreRecords = bLimited = TRUE;
   }
   else if( pSortInfo->dbtri.dbsci.lNext )
   {
      ulRecNo = hb_itemGetNL( pSortInfo->dbtri.dbsci.lNext );
      bLimited = TRUE;
      bMoreRecords = ( ulRecNo > 0 );
   }
   else
   {
      if( !pSortInfo->dbtri.dbsci.itmCobWhile &&
          ( !pSortInfo->dbtri.dbsci.fRest ||
            !hb_itemGetL( pSortInfo->dbtri.dbsci.fRest ) ) )
         uiError = hb_dbfGoTop( pArea );
      bMoreRecords = TRUE;
      bLimited = FALSE;
   }

   while( uiError == SUCCESS && !pArea->fEof && bMoreRecords )
   {
      if( pSortInfo->dbtri.dbsci.itmCobWhile )
         bMoreRecords = hb_itemGetL( hb_vmEvalBlock( pSortInfo->dbtri.dbsci.itmCobWhile ) );

      if( bMoreRecords && pSortInfo->dbtri.dbsci.itmCobFor )
         bValidRecord = hb_itemGetL( hb_vmEvalBlock( pSortInfo->dbtri.dbsci.itmCobFor ) );
      else
         bValidRecord = bMoreRecords;

      if( bValidRecord )
      {
         if( uiCount == dbQuickSort.uiMaxRecords )
         {
            if( !hb_dbQSortAdvance( &dbQuickSort, uiCount ) )
            {
               hb_dbQSortExit( &dbQuickSort );
               return FAILURE;
            }
            pBuffer = dbQuickSort.pBuffer;
            uiCount = 0;
         }

         /* Read record */
         if( !pArea->fValidBuffer && !hb_dbfReadRecord( pArea ) )
         {
            hb_dbQSortExit( &dbQuickSort );
            return FAILURE;
         }

         /* Copy data */
         memcpy( pBuffer, pArea->pRecord, pArea->uiRecordLen );
         pBuffer += pArea->uiRecordLen;
         uiCount++;
      }

      if( bMoreRecords && bLimited )
         bMoreRecords = ( --ulRecNo > 0 );
      if( bMoreRecords )
         uiError = hb_dbfSkip( pArea, 1 );
   }

   /* Copy last records */
   if( uiCount > 0 )
   {
      if( !hb_dbQSortAdvance( &dbQuickSort, uiCount ) )
      {
         hb_dbQSortExit( &dbQuickSort );
         return FAILURE;
      }
   }

   /* Sort records */
   hb_dbQSortComplete( &dbQuickSort );
   return SUCCESS;
}

/*
 * Copy one or more records from one WorkArea to another.
 */
static ERRCODE hb_dbfTrans( DBFAREAP pArea, LPDBTRANSINFO pTransInfo )
{
   PHB_ITEM pPutRec;

   HB_TRACE(HB_TR_DEBUG, ("hb_dbfTrans(%p, %p)", pArea, pTransInfo));

   if( pTransInfo->uiFlags & DBTF_MATCH && !pArea->fHasMemo )
   {
      pPutRec = hb_itemPutL( NULL, FALSE );
      SELF_INFO( ( AREAP ) pTransInfo->lpaDest, DBI_CANPUTREC, pPutRec );
      if( hb_itemGetL( pPutRec ) )
         pTransInfo->uiFlags |= DBTF_PUTREC;
      hb_itemRelease( pPutRec );
   }
   return SUPER_TRANS( ( AREAP ) pArea, pTransInfo );
}

/*
 * Copy a record to another WorkArea.
 */
static ERRCODE hb_dbfTransRec( DBFAREAP pArea, LPDBTRANSINFO pTransInfo )
{
   BOOL bDeleted;

   HB_TRACE(HB_TR_DEBUG, ("hb_dbfTransRec(%p, %p)", pArea, pTransInfo));

   if( pTransInfo->uiFlags & DBTF_PUTREC )
   {
      /* Append a new record */
      if( SELF_APPEND( ( AREAP ) pTransInfo->lpaDest, TRUE ) == FAILURE )
         return FAILURE;

      /* Read record */
      if( !pArea->fValidBuffer && !hb_dbfReadRecord( pArea ) )
         return FAILURE;

      /* Copy record */
      if( SELF_PUTREC( ( AREAP ) pTransInfo->lpaDest, pArea->pRecord ) == FAILURE )
         return FAILURE;

      /* Record deleted? */
      if( SELF_DELETED( ( AREAP ) pArea, &bDeleted ) == FAILURE )
         return FAILURE;

      /* Delete the new record */
      if( bDeleted )
         return SELF_DELETE( ( AREAP ) pTransInfo->lpaDest );
   }
   return SUPER_TRANSREC( ( AREAP ) pArea, pTransInfo );
}

/*
 * Physically remove all records from data store.
 */
static ERRCODE hb_dbfZap( DBFAREAP pArea )
{
   PHB_ITEM pError;

   HB_TRACE(HB_TR_DEBUG, ("hb_dbfZap(%p)", pArea));

   if( pArea->fShared )
   {
      pError = hb_errNew();
      hb_errPutGenCode( pError, EG_SHARED );
      hb_errPutDescription( pError, hb_langDGetErrorDesc( EG_SHARED ) );
      hb_errPutSubCode( pError, EDBF_SHARED );
      SELF_ERROR( ( AREAP ) pArea, pError );
      hb_itemRelease( pError );
      return FAILURE;
   }

   if( SELF_GOCOLD( ( AREAP ) pArea ) == FAILURE )
      return FAILURE;

   hb_dbfGoTo( pArea, 0 );
   pArea->fUpdateHeader = TRUE;
   pArea->ulRecCount = 0;

   /* Zap memo file */
   if( pArea->fHasMemo )
   {
      if ( SELF_CREATEMEMFILE( ( AREAP ) pArea, NULL ) == FAILURE  )
         return FAILURE;
   }
   return SUCCESS;
}

/*
 * Report end of relation.
 */
static ERRCODE hb_dbfChildEnd( DBFAREAP pArea, LPDBRELINFO pRelInfo )
{
   ERRCODE uiError;

   HB_TRACE(HB_TR_DEBUG, ("hb_dbfChildEnd(%p, %p)", pArea, pRelInfo));

   if( pArea->lpdbPendingRel == pRelInfo )
      uiError = SELF_FORCEREL( ( AREAP ) pArea );
   else
      uiError = SUCCESS;
   SUPER_CHILDEND( ( AREAP ) pArea, pRelInfo );
   return uiError;
}

/*
 * Report initialization of a relation.
 */
static ERRCODE hb_dbfChildStart( DBFAREAP pArea, LPDBRELINFO pRelInfo )
{
   HB_TRACE(HB_TR_DEBUG, ("hb_dbfChildStart(%p, %p)", pArea, pRelInfo));

   SELF_CHILDSYNC( ( AREAP ) pArea, pRelInfo );
   return SUPER_CHILDSTART( ( AREAP ) pArea, pRelInfo );
}

/*
 * Post a pending relational movement.
 */
static ERRCODE hb_dbfChildSync( DBFAREAP pArea, LPDBRELINFO pRelInfo )
{
   HB_TRACE(HB_TR_DEBUG, ("hb_dbfChildSync(%p, %p)", pArea, pRelInfo));

   SELF_GOCOLD( ( AREAP ) pArea );
   pArea->lpdbPendingRel = pRelInfo;
   SELF_SYNCCHILDREN( ( AREAP ) pArea );
   return SUCCESS;
}

/*
 * Force relational seeks in the specified WorkArea.
 */
static ERRCODE hb_dbfForceRel( DBFAREAP pArea )
{
   LPDBRELINFO lpdbPendingRel;
   ERRCODE uiError;
/*
   ULONG ulRecNo;
*/

   HB_TRACE(HB_TR_DEBUG, ("hb_dbfForceRel(%p)", pArea));

   if( pArea->lpdbPendingRel )
   {
      lpdbPendingRel = pArea->lpdbPendingRel;
      pArea->lpdbPendingRel = NULL;
      uiError = SELF_RELEVAL( ( AREAP ) pArea, lpdbPendingRel );
/*
      if( uiError == SUCCESS && !lpdbPendingRel->lpaParent->fEof &&
          HB_IS_NUMERIC( pArea->valResult ) )
         ulRecNo = hb_itemGetNL( pArea->valResult );
      else
         ulRecNo = 0;
      uiError = SELF_GOTO( ( AREAP ) pArea, ulRecNo );
      pArea->fFound = pArea->fPositioned;
      pArea->fBof = FALSE;
*/
      return uiError;
   }
   return SUCCESS;
}

/*
 * Set the filter condition for the specified WorkArea.
 */
static ERRCODE hb_dbfSetFilter( DBFAREAP pArea, LPDBFILTERINFO pFilterInfo )
{
   HB_TRACE(HB_TR_DEBUG, ("hb_dbfSetFilter(%p, %p)", pArea, pFilterInfo));

   if( pArea->lpdbPendingRel )
      SELF_FORCEREL( ( AREAP ) pArea );
   return SUPER_SETFILTER( ( AREAP ) pArea, pFilterInfo );
}

/*
 * Perform a network lowlevel lock in the specified WorkArea.
 */
static ERRCODE hb_dbfRawLock( DBFAREAP pArea, USHORT uiAction, ULONG ulRecNo )
{
   ERRCODE uiErr = SUCCESS;
   ULONG ulPos, ulFlSize, ulRlSize;
   SHORT iDir;
   BOOL fLck;

   HB_TRACE(HB_TR_DEBUG, ("hb_dbfRawLock(%p, %hu, %lu)", pArea, uiAction, ulRecNo));

   if( pArea->fShared )
   {
      switch( pArea->bLockType )
      {
         case HB_SET_DBFLOCK_CLIP:
            ulPos = DBF_LOCKPOS_CLIP;
            iDir = DBF_LOCKDIR_CLIP;
            ulFlSize = DBF_FLCKSIZE_CLIP;
            ulRlSize = DBF_RLCKSIZE_CLIP;
            break;

         case HB_SET_DBFLOCK_CL53:
            ulPos = DBF_LOCKPOS_CL53;
            iDir = DBF_LOCKDIR_CL53;
            ulFlSize = DBF_FLCKSIZE_CL53;
            ulRlSize = DBF_RLCKSIZE_CL53;
            break;

         case HB_SET_DBFLOCK_VFP:
            if ( pArea->fHasTags )
            {
               ulPos = DBF_LOCKPOS_VFPX;
               iDir = DBF_LOCKDIR_VFPX;
               ulFlSize = DBF_FLCKSIZE_VFPX;
               ulRlSize = DBF_RLCKSIZE_VFPX;
            }
            else
            {
               ulPos = DBF_LOCKPOS_VFP;
               iDir = DBF_LOCKDIR_VFP;
               ulFlSize = DBF_FLCKSIZE_VFP;
               ulRlSize = DBF_RLCKSIZE_VFP;
            }
            break;

         default:
            return FAILURE;
      }

      switch( uiAction )
      {
         case FILE_LOCK:
            if( !pArea->fFLocked )
            {
               if ( iDir < 0 )
                  fLck = hb_fsLock( pArea->hDataFile, ulPos - ulFlSize, ulFlSize, FL_LOCK );
               else
                  fLck = hb_fsLock( pArea->hDataFile, ulPos + 1, ulFlSize, FL_LOCK );

               if( !fLck )
                  uiErr = FAILURE;
               else
                  pArea->fFLocked = TRUE;
            }
            break;

         case FILE_UNLOCK:
            if( pArea->fFLocked )
            {
               if ( iDir < 0 )
                  fLck = hb_fsLock( pArea->hDataFile, ulPos - ulFlSize, ulFlSize, FL_UNLOCK );
               else
                  fLck = hb_fsLock( pArea->hDataFile, ulPos + 1, ulFlSize, FL_UNLOCK );

               if( !fLck )
                  uiErr = FAILURE;
               pArea->fFLocked = FALSE;
            }
            break;

         case REC_LOCK:
            if( !pArea->fFLocked )
            {
               if ( iDir < 0 )
                  fLck = hb_fsLock( pArea->hDataFile, ulPos - ulRecNo, ulRlSize, FL_LOCK );
               else if ( iDir == 2 )
                  fLck = hb_fsLock( pArea->hDataFile, ulPos + ( ulRecNo - 1 ) * pArea->uiRecordLen + pArea->uiHeaderLen, ulRlSize, FL_LOCK );
               else
                  fLck = hb_fsLock( pArea->hDataFile, ulPos + ulRecNo, ulRlSize, FL_LOCK );

               if( !fLck )
                  uiErr = FAILURE;
            }
            break;

         case REC_UNLOCK:
            if( !pArea->fFLocked )
            {
               if ( iDir < 0 )
                  fLck = hb_fsLock( pArea->hDataFile, ulPos - ulRecNo, ulRlSize, FL_UNLOCK );
               else if ( iDir == 2 )
                  fLck = hb_fsLock( pArea->hDataFile, ulPos + ( ulRecNo - 1 ) * pArea->uiRecordLen + pArea->uiHeaderLen, ulRlSize, FL_UNLOCK );
               else
                  fLck = hb_fsLock( pArea->hDataFile, ulPos + ulRecNo, ulRlSize, FL_UNLOCK );
               if( !fLck )
                  uiErr = FAILURE;
            }
            break;

         case APPEND_LOCK:
         case HEADER_LOCK:
            if( !pArea->fHeaderLocked )
            {
               do
               {
                  fLck = hb_fsLock( pArea->hDataFile, ulPos, 1, FL_LOCK | FLX_WAIT );
                  /* TODO: call special error handler (LOCKHANDLER) hiere if !fLck */
               } while ( !fLck );
               if( !fLck )
                  uiErr = FAILURE;
               else
                  pArea->fHeaderLocked = TRUE;
            }
            break;

         case APPEND_UNLOCK:
         case HEADER_UNLOCK:
            if( pArea->fHeaderLocked )
            {
               if( !hb_fsLock( pArea->hDataFile, ulPos, 1, FL_UNLOCK ) )
                  uiErr = FAILURE;
               pArea->fHeaderLocked = FALSE;
            }
            break;
      }
   }
   return uiErr;
}

/*
 * Perform a network lock in the specified WorkArea.
 */
static ERRCODE hb_dbfLock( DBFAREAP pArea, LPDBLOCKINFO pLockInfo )
{
   HB_TRACE(HB_TR_DEBUG, ("hb_dbfLock(%p, %p)", pArea, pLockInfo));

   if( pArea->fShared )
   {
      switch( pLockInfo->uiMethod )
      {
         case DBLM_EXCLUSIVE:
            return hb_dbfLockRecord( pArea, pArea->ulRecNo, &pLockInfo->fResult,
                                     ( pLockInfo->uiMethod == DBLM_EXCLUSIVE ) );

         case DBLM_MULTIPLE:
            return hb_dbfLockRecord( pArea, hb_itemGetNL( pLockInfo->itmRecID ),
                                     &pLockInfo->fResult,
                                     ( pLockInfo->uiMethod == DBLM_EXCLUSIVE ) );

         case DBLM_FILE:
            return hb_dbfLockFile( pArea, &pLockInfo->fResult );

         default:
            pLockInfo->fResult = FALSE;
      }
   }
   else
      pLockInfo->fResult = TRUE;

   return SUCCESS;
}

/*
 * Release network locks in the specified WorkArea.
 */
static ERRCODE hb_dbfUnLock( DBFAREAP pArea, ULONG ulRecNo )
{
   ERRCODE uiError;

   HB_TRACE(HB_TR_DEBUG, ("dbfUnLock(%p, %lu)", pArea, ulRecNo));

   uiError = SUCCESS;
   if( pArea->fShared )
   {
      if( pArea->ulNumLocksPos > 0 )
      {
         /* Unlock all records? */
         if( ulRecNo == 0 )
            uiError = hb_dbfUnlockAllRecords( pArea );
         else if( hb_dbfIsLocked( pArea, ulRecNo ) )
            uiError = hb_dbfUnlockRecord( pArea, ulRecNo );
      }
      if( pArea->fFLocked )
      {
         uiError = hb_dbfUnlockFile( pArea );
      }
   }
   return uiError;
}

/*
 * Create a memo file in the WorkArea.
 */
static ERRCODE hb_dbfCreateMemFile( DBFAREAP pArea, LPDBOPENINFO pCreateInfo )
{
   PHB_ITEM pError;

   HB_TRACE(HB_TR_DEBUG, ("hb_dbfCreateMemFile(%p, %p)", pArea, pCreateInfo));

   if( pCreateInfo )
   {
      pError = hb_errNew();
      hb_errPutGenCode( pError, EG_DATATYPE );
      hb_errPutSubCode( pError, EDBF_DATATYPE );
      hb_errPutDescription( pError, hb_langDGetErrorDesc( EG_CREATE ) );
      hb_errPutFileName( pError, ( char * ) pCreateInfo->abName );
      SELF_ERROR( ( AREAP ) pArea, pError );
      hb_itemRelease( pError );
   }
   pArea->fHasMemo = FALSE;
   return FAILURE;
}

/*
 * Open a memo file in the specified WorkArea.
 */
static ERRCODE hb_dbfOpenMemFile( DBFAREAP pArea, LPDBOPENINFO pOpenInfo )
{
   PHB_ITEM pError;

   HB_TRACE(HB_TR_DEBUG, ("hb_dbfOpenMemFile(%p, %p)", pArea, pOpenInfo));

   pError = hb_errNew();
   hb_errPutGenCode( pError, EG_OPEN );
   hb_errPutSubCode( pError, EDBF_OPEN_DBF );
   hb_errPutDescription( pError, hb_langDGetErrorDesc( EG_OPEN ) );
   hb_errPutFileName( pError, ( char * ) pOpenInfo->abName );
   SELF_ERROR( ( AREAP ) pArea, pError );
   hb_itemRelease( pError );
   return FAILURE;
}
/*
 * Read the database file header record in the WorkArea.
 */
static ERRCODE hb_dbfReadDBHeader( DBFAREAP pArea )
{
   DBFHEADER dbHeader;
   BOOL bRetry, bError;
   PHB_ITEM pError;

   HB_TRACE(HB_TR_DEBUG, ("hb_dbfReadDBHeader(%p)", pArea));

   pError = NULL;
   /* Try read */
   do
   {
      hb_fsSeek( pArea->hDataFile, 0, FS_SET );
      if( hb_fsRead( pArea->hDataFile, ( BYTE * ) &dbHeader, sizeof( DBFHEADER ) ) != sizeof( DBFHEADER ) ||
          ( dbHeader.bVersion != 0x03 &&    // dBase III
            dbHeader.bVersion != 0x30 &&    // VisualFoxPro 6.0
            dbHeader.bVersion != 0x83 &&    // dBase III w/memo
            dbHeader.bVersion != 0xF5       // FoxPro??? w/memo
        ) )
      {
         bError = TRUE;
         if( !pError )
         {
            pError = hb_errNew();
            hb_errPutGenCode( pError, EG_CORRUPTION );
            hb_errPutSubCode( pError, EDBF_CORRUPT );
            hb_errPutDescription( pError, hb_langDGetErrorDesc( EG_CORRUPTION ) );
            hb_errPutFileName( pError, pArea->szDataFileName );
            hb_errPutFlags( pError, EF_CANRETRY | EF_CANDEFAULT );
         }
         bRetry = ( SELF_ERROR( ( AREAP ) pArea, pError ) == E_RETRY );
      }
      else
         bRetry = bError = FALSE;
   } while( bRetry );

   if( pError )
      hb_itemRelease( pError );

   /* Read error? */
   if( bError )
      return FAILURE;

   pArea->bDay = dbHeader.bDay;
   pArea->bMonth = dbHeader.bMonth;
   pArea->bYear = dbHeader.bYear;
   pArea->fHasTags = dbHeader.bHasTags;
   pArea->bVersion = dbHeader.bVersion;
   pArea->bCodePage = dbHeader.bCodePage;
   pArea->uiHeaderLen = HB_USHORT_FROM_LE( dbHeader.uiHeaderLen );
   pArea->ulRecCount = HB_ULONG_FROM_LE( dbHeader.ulRecCount );
   pArea->fHasMemo = FALSE;
   return SUCCESS;
}

/*
 * Write the database file header record in the WorkArea.
 */
static ERRCODE hb_dbfWriteDBHeader( DBFAREAP pArea )
{
   BOOL fLck = FALSE;
   DBFHEADER dbfHeader;
   LONG lYear, lMonth, lDay;

   HB_TRACE(HB_TR_DEBUG, ("hb_dbfWriteDBHeader(%p)", pArea));

   memset( &dbfHeader, 0, sizeof( DBFHEADER ) );
   dbfHeader.bVersion = pArea->bVersion;
   hb_dateToday( &lYear, &lMonth, &lDay );
   dbfHeader.bYear = ( BYTE ) ( lYear - 1900 );
   dbfHeader.bMonth = ( BYTE ) lMonth;
   dbfHeader.bDay = ( BYTE ) lDay;
   dbfHeader.bHasTags = ( BYTE ) pArea->fHasTags;
   dbfHeader.bCodePage = pArea->bCodePage;

   /* Update record count */
   if( pArea->fShared )
   {
      if( !pArea->fHeaderLocked )
      {
         if( SELF_RAWLOCK( ( AREAP ) pArea, HEADER_LOCK, 0 ) == FAILURE )
            return FAILURE;
         fLck = TRUE;
      }
      pArea->ulRecCount = hb_dbfCalcRecCount( pArea );
   }

   HB_PUT_LE_ULONG( &dbfHeader.ulRecCount, pArea->ulRecCount );
   HB_PUT_LE_USHORT( &dbfHeader.uiHeaderLen, pArea->uiHeaderLen );
   HB_PUT_LE_USHORT( &dbfHeader.uiRecordLen, pArea->uiRecordLen );
   hb_fsSeek( pArea->hDataFile, 0, FS_SET );
   hb_fsWrite( pArea->hDataFile, ( BYTE * ) &dbfHeader, sizeof( DBFHEADER ) );
   pArea->fUpdateHeader = FALSE;
   if( fLck )
      SELF_RAWLOCK( ( AREAP ) pArea, HEADER_UNLOCK, 0 );
   return SUCCESS;
}

static ERRCODE hb_dbfDrop( PHB_ITEM pItemTable )
{
  BYTE   * pBuffer;
  char szFileName[ _POSIX_PATH_MAX + 1 ];

  pBuffer = (BYTE *) hb_itemGetCPtr( pItemTable );
  strcpy( (char *) szFileName, (char *) pBuffer );
  if ( !strchr( szFileName, '.' ))
    strcat( szFileName, DBF_TABLEEXT );
  return hb_fsDelete( (BYTE *) szFileName );
}

/* returns 1 if exists, 0 else */
BOOL hb_dbfExists( PHB_ITEM pItemTable, PHB_ITEM pItemIndex )
{
  char szFileName[ _POSIX_PATH_MAX + 1 ];
  BYTE * pBuffer;

  pBuffer = (BYTE *) hb_itemGetCPtr( pItemIndex != NULL ? pItemIndex : pItemTable );
  strcpy( (char *) szFileName, (char *) pBuffer );
  if ( pItemTable && !strchr( szFileName, '.' ))
    strcat( szFileName, DBF_TABLEEXT );
  return hb_fsFile( (BYTE *) szFileName );
}


HB_FUNC( _DBFC )
{
}

HB_FUNC( DBF_GETFUNCTABLE )
{
   RDDFUNCS * pTable;
   USHORT * uiCount;

   uiCount = ( USHORT * ) hb_itemGetPtr( hb_param( 1, HB_IT_POINTER ) );
   * uiCount = RDDFUNCSCOUNT;
   pTable = ( RDDFUNCS * ) hb_itemGetPtr( hb_param( 2, HB_IT_POINTER ) );

   HB_TRACE(HB_TR_DEBUG, ("DBF_GETFUNCTABLE(%i, %p)", uiCount, pTable));

   if( pTable )
      hb_retni( hb_rddInherit( pTable, &dbfTable, &dbfSuper, 0 ) );
   else
      hb_retni( FAILURE );
}
