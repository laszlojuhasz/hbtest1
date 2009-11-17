/*
 * $Id$
 */

/*
 * ADS memory index driver
 *
 * Copyright 2007 Mindaugas Kavaliauskas <dbtopas at dbtopas.lt>
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

#define HB_OS_WIN_USED

#include "hbdefs.h"
#include "hbapi.h"
#include "hbinit.h"
#include "hbapiitm.h"
#include "hbapierr.h"
#include "hbdbferr.h"
#include "hbapilng.h"
#include "hbdate.h"
#include "rddads.h"
#include "hbset.h"
#include "hbvm.h"
#include "rddsys.ch"

#ifndef SUPER_ORDDESTROY
#define SUPER_ORDDESTROY(w, ip)          ((*(SUPERTABLE)->orderDestroy)(w, ip))
#endif

#define SUPERTABLE ( &adsxSuper )

#define MIX_MAXKEYLEN          250
#define MIX_MAXTAGNAMELEN       16


#define MIX_KEYPOOLFIRST      256
#define MIX_KEYPOOLRESIZE    1024


typedef struct _MIXKEY
{
   ULONG     rec;
   BYTE      val[ 1 ];
} MIXKEY, *LPMIXKEY;


typedef struct _MIXTAG
{
   struct _MIXTAG*  pNext;

   char*            szName;
   char*            szKeyExpr;
   char*            szForExpr;
   PHB_ITEM         pKeyItem;
   PHB_ITEM         pForItem;
   BYTE             bType;
   USHORT           uiLen;

   LPMIXKEY*        pKeys;
   ULONG            ulRecMax;
   ULONG            ulRecCount;

   PHB_CODEPAGE     pCodepage;  /* National sorttable for character key tags, NULL otherwise */

   ULONG            ulKeyNo;
} MIXTAG, *LPMIXTAG;


typedef struct _ADSXAREA_
{
   ADSAREA adsarea;

   /* ================ Additional fields for ADSX RDD =================== */

   LPMIXTAG     pTagList;
   LPMIXTAG     pTagCurrent;
} ADSXAREA, *ADSXAREAP;



static USHORT   s_uiRddIdADSX = ( USHORT ) -1;
static RDDFUNCS adsxSuper;


/***********************************************************************
*  Misc functions
************************************************************************/

static HB_ERRCODE hb_mixErrorRT( ADSXAREAP pArea,
                                 HB_ERRCODE errGenCode, HB_ERRCODE errSubCode,
                                 char* filename, HB_ERRCODE errOsCode,
                                 USHORT uiFlags )
{
   PHB_ITEM pError;
   HB_ERRCODE iRet = HB_FAILURE;

   if( hb_vmRequestQuery() == 0 )
   {
      pError = hb_errNew();
      hb_errPutGenCode( pError, errGenCode );
      hb_errPutSubCode( pError, errSubCode );
      hb_errPutOsCode( pError, errOsCode );
      hb_errPutDescription( pError, hb_langDGetErrorDesc( errGenCode ) );
      if( filename )
         hb_errPutFileName( pError, filename );
      if( uiFlags )
         hb_errPutFlags( pError, uiFlags );
      iRet = SELF_ERROR( (AREAP) pArea, pError );
      hb_errRelease( pError );
   }
   return iRet;
}

/* Clone of ADS RDD function */
static HB_ERRCODE hb_adsUpdateAreaFlags( ADSXAREAP pArea )
{
   UNSIGNED16 u16Bof, u16Eof, u16Found;

   AdsAtBOF( pArea->adsarea.hTable, &u16Bof );
   AdsAtEOF( pArea->adsarea.hTable, &u16Eof );
   AdsIsFound( pArea->adsarea.hTable, &u16Found );

   pArea->adsarea.area.fBof = u16Bof != 0;
   pArea->adsarea.area.fEof = u16Eof != 0;
   pArea->adsarea.area.fFound = u16Found != 0;

   pArea->adsarea.fPositioned = !pArea->adsarea.area.fBof && !pArea->adsarea.area.fEof;

   return HB_SUCCESS;
}


/************************************************************************
*  Memory Index
*************************************************************************/

static LPMIXKEY mixKeyNew( PHB_ITEM pItem, ULONG ulRecNo, BYTE bType, USHORT uiLen )
{
   LPMIXKEY    pKey;
   ULONG       ul;
   double      dbl;
   BYTE        buf[ 8 ];

   pKey = (LPMIXKEY) hb_xgrab( sizeof( ULONG ) + uiLen );
   pKey->rec = ulRecNo;

   switch ( bType )
   {
      case 'C':
         ul = hb_itemGetCLen( pItem );
         if( ul > (ULONG) uiLen )
            ul = uiLen;
         memcpy( pKey->val, hb_itemGetCPtr( pItem ), ul );
         if( ul < (ULONG) uiLen )
            memset( pKey->val + ul, ' ', (ULONG) uiLen - ul );
         break;

      case 'N':
         dbl = hb_itemGetND( pItem );
         HB_DBL2ORD( &dbl, buf );
         memcpy( pKey->val, buf, 8 );
         break;

      case 'D':
         dbl = (double) hb_itemGetDL( pItem );
         HB_DBL2ORD( &dbl, buf );
         memcpy( pKey->val, buf, 8 );
         break;

      case 'L':
         pKey->val[ 0 ] = ( BYTE ) ( hb_itemGetL( pItem ) ? 'T' : 'F' );
         break;

      default:
         memset( pKey->val, ' ', uiLen );
   }
   return pKey;
}


static LPMIXKEY mixKeyEval( LPMIXTAG pTag, ADSXAREAP pArea )
{
   PHB_ITEM      pItem;
   LPMIXKEY      pKey;
   int           iCurrArea = hb_rddGetCurrentWorkAreaNumber();
   PHB_CODEPAGE  pCodepage = hb_cdpSelect( pArea->adsarea.area.cdPage );

   if( iCurrArea != pArea->adsarea.area.uiArea )
      hb_rddSelectWorkAreaNumber( pArea->adsarea.area.uiArea );
   else
      iCurrArea = 0;

   pItem = hb_vmEvalBlockOrMacro( pTag->pKeyItem );

   pKey = mixKeyNew( pItem, pArea->adsarea.ulRecNo, pTag->bType, pTag->uiLen );

   if( iCurrArea )
      hb_rddSelectWorkAreaNumber( iCurrArea );

   hb_cdpSelect( pCodepage );

   return pKey;
}


static BOOL mixEvalCond( PHB_ITEM pCondItem, ADSXAREAP pArea )
{
   int iCurrArea = 0;
   BOOL fRet;

   if( pArea )
   {
      iCurrArea = hb_rddGetCurrentWorkAreaNumber();

      if( iCurrArea != pArea->adsarea.area.uiArea )
         hb_rddSelectWorkAreaNumber( pArea->adsarea.area.uiArea );
      else
         iCurrArea = 0;
   }

   fRet = hb_itemGetL( hb_vmEvalBlockOrMacro( pCondItem ) );

   if( iCurrArea )
      hb_rddSelectWorkAreaNumber( iCurrArea );

   return fRet;
}


static void mixKeyFree( LPMIXKEY pKey )
{
   hb_xfree( pKey );
}


static int mixQSortCompare( LPMIXKEY p1, LPMIXKEY p2, USHORT uiLen, PHB_CODEPAGE pCodepage )
{
   int   i;

   if( pCodepage )
   {
      i = hb_cdpcmp( ( const char * ) p1->val, ( ULONG ) uiLen, ( const char * ) p2->val, ( ULONG ) uiLen, pCodepage, 0 );
   }
   else
     i = memcmp( p1->val, p2->val, uiLen );

   if( i == 0 )
   {
      if( p1->rec < p2->rec )
         return -1;
      else if( p1->rec > p2->rec )
         return 1;
      else
         return 0;
   }
   else /* This is used to compare keys excluding recno */
   {
      if( i < 0 )  i = -2;
      else         i = 2;
   }
   return i;
}


static void mixQSort( LPMIXKEY* pKeys, ULONG left, ULONG right, USHORT uiLen, PHB_CODEPAGE pCodepage )
{
   ULONG     l, r;
   LPMIXKEY  x, h;

   l = left;
   r = right;

   x = pKeys[ (l + r) / 2 ];

   do {
      while( mixQSortCompare( x, pKeys[ l ], uiLen, pCodepage ) > 0 )
         l++;
      while( mixQSortCompare( pKeys[ r ], x, uiLen, pCodepage ) > 0 )
         r--;

      if( l < r )
      {
         h = pKeys[ l ];  pKeys[ l ] = pKeys[ r ];  pKeys[ r ] = h;
         l++;  r--;
      }
   }
   while( l < r );

   if( left < r && ( r != right ) )
      mixQSort( pKeys, left, r, uiLen, pCodepage );

   if( l < right && ( l != left ) )
      mixQSort( pKeys, l, right, uiLen, pCodepage );
}


static LPMIXKEY mixFindKey( LPMIXTAG pTag, LPMIXKEY pKey, ULONG* ulKeyPos )
{
   ULONG      l, r;
   int        i = 1;

   if( ! pTag->ulRecCount )
   {
      if( ulKeyPos )
         *ulKeyPos = 0;
      return NULL;
   }

   l = 0;
   r = pTag->ulRecCount - 1;

   while( l < r )
   {
      i = mixQSortCompare( pTag->pKeys[ (l + r) / 2 ], pKey, pTag->uiLen, pTag->pCodepage );

      if( i < 0 )
         l = (l + r) / 2 + 1;
      else if( i > 0 )
         r = (l + r) / 2;
      else
         l = r = (l + r) / 2;
   }

   if( i )
   {
      i = mixQSortCompare( pTag->pKeys[ l ], pKey, pTag->uiLen, pTag->pCodepage );
      if( i < 0 )
         l++;
   }

   if( ulKeyPos )
      *ulKeyPos = l;

   return i ? NULL : pTag->pKeys[ l ];
}


static int mixCompareKey( LPMIXTAG pTag, ULONG ulKeyPos, LPMIXKEY pKey )
{
   return mixQSortCompare( pTag->pKeys[ ulKeyPos ], pKey, pTag->uiLen, pTag->pCodepage );
}


static LPMIXTAG mixTagCreate( const char * szTagName, PHB_ITEM pKeyExpr, PHB_ITEM pKeyItem,
                              PHB_ITEM pForItem, PHB_ITEM pWhileItem, BYTE bType,
                              USHORT uiLen, ADSXAREAP pArea )
{
   LPMIXTAG             pTag;
   LPMIXKEY             pKey;
   LPDBORDERCONDINFO    pOrdCondInfo = pArea->adsarea.area.lpdbOrdCondInfo;
   ADSHANDLE            hOrder;
   ULONG                ulRec, ulStartRec, ulNextCount = 0;
   LONG                 lStep = 0;
   PHB_ITEM             pItem, pEvalItem = NULL;


   pTag = (LPMIXTAG) hb_xgrab( sizeof( MIXTAG ) );
   memset( pTag, 0, sizeof( MIXTAG ) );

   pTag->szName   = ( char * ) hb_xgrab( MIX_MAXTAGNAMELEN + 1 );
   hb_strncpyUpperTrim( pTag->szName, szTagName, MIX_MAXTAGNAMELEN );

   pTag->szKeyExpr   = ( char * ) hb_xgrab( hb_itemGetCLen( pKeyExpr ) + 1 );
   hb_strncpyTrim( pTag->szKeyExpr, hb_itemGetCPtr( pKeyExpr ), hb_itemGetCLen( pKeyExpr ) );

   /* TODO: for expresion */
   pTag->szForExpr = NULL;

   pTag->pKeyItem = pKeyItem;
   pTag->pForItem = pForItem;
   pTag->bType = bType;
   pTag->uiLen = uiLen;

   /* Use national support */
   if( bType == 'C' && pArea->adsarea.area.cdPage && pArea->adsarea.area.cdPage->sort )
      pTag->pCodepage = pArea->adsarea.area.cdPage;

   pTag->pKeys = (LPMIXKEY*) hb_xgrab( sizeof( LPMIXKEY ) * MIX_KEYPOOLFIRST );
   pTag->ulRecMax = MIX_KEYPOOLFIRST;

   ulStartRec = 0;


   if( pOrdCondInfo )
   {
      pEvalItem = pOrdCondInfo->itmCobEval;
      lStep = pOrdCondInfo->lStep;
   }

   if( ! pOrdCondInfo || pOrdCondInfo->fAll )
   {
      pArea->adsarea.hOrdCurrent = 0;
   }
   else
   {
      if( pOrdCondInfo->itmRecID )
         ulStartRec = hb_itemGetNL( pOrdCondInfo->itmRecID );

      if( ulStartRec )
      {
         ulNextCount = 1;
      }
      else if( pOrdCondInfo->fRest || pOrdCondInfo->lNextCount > 0 )
      {
         if( pOrdCondInfo->itmStartRecID )
            ulStartRec = hb_itemGetNL( pOrdCondInfo->itmStartRecID );

         if( !ulStartRec )
            ulStartRec = pArea->adsarea.ulRecNo;

         if( pArea->adsarea.area.lpdbOrdCondInfo->lNextCount > 0 )
            ulNextCount = pOrdCondInfo->lNextCount;
      }
      else if( ! pOrdCondInfo->fUseCurrent )
      {
         pArea->adsarea.hOrdCurrent = 0;
      }
   }

   hOrder = pArea->adsarea.hOrdCurrent ? pArea->adsarea.hOrdCurrent :
                                         pArea->adsarea.hTable;

   if( ulStartRec )
      AdsGotoRecord( pArea->adsarea.hTable, ulStartRec );
   else
      AdsGotoTop( pArea->adsarea.hTable );
   hb_adsUpdateAreaFlags( pArea );

   while( ! pArea->adsarea.area.fEof )
   {
      SELF_RECNO( (AREAP) pArea, &ulRec );
      SELF_GOTO( (AREAP) pArea, ulRec );

      if( pEvalItem )
      {
         if( lStep >= pOrdCondInfo->lStep )
         {
            lStep = 0;
            if( ! mixEvalCond( pEvalItem, NULL ) )
               break;
         }
         ++lStep;
      }

      if( pWhileItem && ! mixEvalCond( pWhileItem, NULL ) )
      {
         break;
      }

      if( pForItem == NULL || mixEvalCond( pForItem, NULL ) )
      {
         pItem = hb_vmEvalBlockOrMacro( pKeyItem );

         pKey = mixKeyNew( pItem, ulRec, bType, uiLen );

         if( pTag->ulRecCount == pTag->ulRecMax )
         {
            pTag->pKeys = (LPMIXKEY*) hb_xrealloc( pTag->pKeys, sizeof( LPMIXKEY ) * ( pTag->ulRecMax + MIX_KEYPOOLRESIZE ) );
            pTag->ulRecMax += MIX_KEYPOOLRESIZE;
         }

         pTag->pKeys[ pTag->ulRecCount ] = pKey;
         pTag->ulRecCount++;
      }

      if( ulNextCount )
      {
         ulNextCount--;
         if( !ulNextCount )  break;
      }

      AdsSkip( hOrder, 1 );
      hb_adsUpdateAreaFlags( pArea );
   }

   /* QuickSort */
   if( pTag->ulRecCount >= 2 )
      mixQSort( pTag->pKeys, 0, pTag->ulRecCount - 1, uiLen, pTag->pCodepage );

   return pTag;
}


static void mixTagDestroy( LPMIXTAG pTag )
{
   ULONG   ul;

   hb_xfree( pTag->szName );
   hb_xfree( pTag->szKeyExpr );
   if( pTag->szForExpr )
      hb_xfree( pTag->szForExpr );

   if( pTag->pKeyItem )
      hb_vmDestroyBlockOrMacro( pTag->pKeyItem );

   if( pTag->pForItem )
      hb_vmDestroyBlockOrMacro( pTag->pForItem );

   for( ul = 0; ul < pTag->ulRecCount; ul++ )
      mixKeyFree( pTag->pKeys[ ul ] );

   hb_xfree( pTag->pKeys );

   hb_xfree( pTag );
}


static LPMIXTAG mixFindTag( ADSXAREAP pArea, PHB_ITEM pOrder )
{
   char        szTag[ MIX_MAXTAGNAMELEN + 1 ];
   LPMIXTAG    pTag;

   if( HB_IS_NUMBER( pOrder ) )
   {
      UNSIGNED16      usOrder = 0, usFind;

      usFind = (UNSIGNED16) hb_itemGetNI( pOrder );

      AdsGetNumIndexes( pArea->adsarea.hTable, &usOrder );
      usOrder++;
      pTag = pArea->pTagList;
      while( pTag && usOrder != usFind )
         pTag = pTag->pNext;
   }
   else
   {
      hb_strncpyUpperTrim( szTag, hb_itemGetCPtr( pOrder ), MIX_MAXTAGNAMELEN );
      pTag = pArea->pTagList;
      while( pTag && hb_stricmp( szTag, pTag->szName ) )
         pTag = pTag->pNext;
   }

   return pTag;
}


/************************************************************************
*  ADSX RDD METHODS
*************************************************************************/

static HB_ERRCODE adsxGoBottom( ADSXAREAP pArea )
{
   LPMIXTAG  pTag;
   ULONG     ulRecNo;

   pTag = pArea->pTagCurrent;

   if( ! pTag )
      return SUPER_GOBOTTOM( (AREAP) pArea );

   if( pTag->ulRecCount > 0 )
   {
      ulRecNo = pTag->pKeys[ pTag->ulRecCount - 1 ]->rec;
   }
   else
   {
      ulRecNo = 0;
   }
   if( SUPER_GOTO( (AREAP) pArea, ulRecNo ) == HB_SUCCESS )
   {
      pTag->ulKeyNo = ulRecNo ? pTag->ulRecCount : 0;
      return HB_SUCCESS;
   }
   pTag->ulKeyNo = 0;
   return HB_FAILURE;
}


static HB_ERRCODE adsxGoTop( ADSXAREAP pArea )
{
   LPMIXTAG  pTag;
   ULONG     ulRecNo;

   pTag = pArea->pTagCurrent;

   if( ! pTag )
      return SUPER_GOTOP( (AREAP) pArea );

   if( pTag->ulRecCount > 0 )
   {
      ulRecNo = pTag->pKeys[ 0 ]->rec;
   }
   else
   {
      ulRecNo = 0;
   }
   if( SUPER_GOTO( (AREAP) pArea, ulRecNo ) == HB_SUCCESS )
   {
      pTag->ulKeyNo = ulRecNo ? 1 : 0;
      return HB_SUCCESS;
   }
   pTag->ulKeyNo = 0;
   return HB_FAILURE;
}


static HB_ERRCODE adsxSeek( ADSXAREAP pArea, BOOL bSoftSeek, PHB_ITEM pKey, BOOL bFindLast )
{
   LPMIXKEY     pMixKey;
   ULONG        ulKeyPos, ulRecNo;
   HB_ERRCODE      errCode;
   BOOL         fFound = FALSE;

   if( ! pArea->pTagCurrent )
      return SUPER_SEEK( (AREAP) pArea, bSoftSeek, pKey, bFindLast );

   /* TODO: pKey type validation, EG_DATATYPE runtime error */
   pMixKey = mixKeyNew( pKey, bFindLast ? (ULONG) (-1) : 0, pArea->pTagCurrent->bType,
                        pArea->pTagCurrent->uiLen );

   /* reset any pending relations - I hope ACE make the same and the problem
      reported in GOTO() does not exist here */
   SELF_RESETREL( &pArea->adsarea );

   mixFindKey( pArea->pTagCurrent, pMixKey, &ulKeyPos );

   ulRecNo = 0;
   if( bFindLast )
   {
      if( ulKeyPos > 0 && mixCompareKey( pArea->pTagCurrent, ulKeyPos - 1, pMixKey ) == -1 )
      {
         ulRecNo = pArea->pTagCurrent->pKeys[ ulKeyPos - 1 ]->rec;
         fFound = 1;
      }
      else if( ulKeyPos < pArea->pTagCurrent->ulRecCount )
      {
         ulRecNo = pArea->pTagCurrent->pKeys[ ulKeyPos ]->rec;
         fFound = 0;
      }
   }
   else
   {
      if( ulKeyPos < pArea->pTagCurrent->ulRecCount )
      {
         ulRecNo = pArea->pTagCurrent->pKeys[ ulKeyPos ]->rec;
         fFound = mixCompareKey( pArea->pTagCurrent, ulKeyPos, pMixKey ) == 1;
      }
   }

   mixKeyFree( pMixKey );

   ulRecNo = ( bSoftSeek || fFound ) ? ulRecNo : 0;

   errCode = SELF_GOTO( (AREAP) pArea, ulRecNo );

   pArea->adsarea.area.fEof = ulRecNo == 0;
   pArea->adsarea.area.fBof = FALSE;
   pArea->adsarea.area.fFound = fFound;
   pArea->adsarea.fPositioned = ! pArea->adsarea.area.fEof;
   return errCode;
}


static HB_ERRCODE adsxSkip( ADSXAREAP pArea, LONG lToSkip )
{
   LPMIXKEY    pKey;
   ULONG       ulKeyPos;
   HB_ERRCODE     errCode = HB_SUCCESS;

   if( ! pArea->pTagCurrent || lToSkip == 0 )
      return SUPER_SKIP( (AREAP) pArea, lToSkip );

   /* resolve any pending relations */
   if( pArea->adsarea.lpdbPendingRel )
      SELF_FORCEREL( (AREAP) pArea );

   pArea->adsarea.area.fTop = pArea->adsarea.area.fBottom = FALSE;

   if( lToSkip > 0 )
   {
      if( !pArea->adsarea.fPositioned )
      {
         errCode = SELF_GOTO( (AREAP) pArea, pArea->adsarea.ulRecNo );
      }
      else
      {
         pArea->adsarea.area.fEof = FALSE;
      }

      pKey = mixKeyEval( pArea->pTagCurrent, pArea );

      if( mixFindKey( pArea->pTagCurrent, pKey, &ulKeyPos ) &&
           pArea->pTagCurrent->ulRecCount > (ULONG) lToSkip &&
           ulKeyPos < pArea->pTagCurrent->ulRecCount - (ULONG) lToSkip )
      {
         if( SELF_GOTO( (AREAP) pArea, pArea->pTagCurrent->pKeys[ ulKeyPos + lToSkip ]->rec ) == HB_FAILURE )
            errCode = HB_FAILURE;
      }
      else
      {
         SELF_GOTO( (AREAP) pArea, 0 );
      }

      mixKeyFree( pKey );

      hb_adsUpdateAreaFlags( pArea );
      pArea->adsarea.area.fBof = FALSE;
   }
   else
   {
      if( !pArea->adsarea.fPositioned )
      {
         errCode = SELF_GOBOTTOM( (AREAP) pArea );
         pArea->adsarea.area.fBottom = FALSE;
         ++lToSkip;
      }
      else
      {
         pArea->adsarea.area.fBof = FALSE;
      }

      pKey = mixKeyEval( pArea->pTagCurrent, pArea );

      if( mixFindKey( pArea->pTagCurrent, pKey, &ulKeyPos ) &&
           pArea->pTagCurrent->ulRecCount >= (ULONG) (-lToSkip) &&
           ulKeyPos >= (ULONG) (-lToSkip) )
      {
         if( SELF_GOTO( (AREAP) pArea, pArea->pTagCurrent->pKeys[ ulKeyPos + lToSkip ]->rec ) == HB_FAILURE )
            errCode = HB_FAILURE;
      }
      else
      {
         SELF_GOTO( (AREAP) pArea, 0 );
      }

      mixKeyFree( pKey );

      hb_adsUpdateAreaFlags( pArea );
      pArea->adsarea.area.fEof = FALSE;
   }

   /* Force relational movement in child WorkAreas */
   if( pArea->adsarea.area.lpdbRelations )
      SELF_SYNCCHILDREN( (AREAP) pArea );

   return errCode;
}


static HB_ERRCODE adsxClose( ADSXAREAP pArea )
{
   LPMIXTAG     pTag;

   pArea->pTagCurrent = NULL;
   while( pArea->pTagList )
   {
      pTag = pArea->pTagList;
      pArea->pTagList = pArea->pTagList->pNext;
      mixTagDestroy( pTag );
   }
   return SUPER_CLOSE( (AREAP) pArea );
}


static HB_ERRCODE adsxCreate( ADSXAREAP pArea, LPDBOPENINFO pCreateInfo )
{
   if( SUPER_CREATE( (AREAP) pArea, pCreateInfo ) == HB_SUCCESS )
   {
      if( pCreateInfo->cdpId )
      {
         pArea->adsarea.area.cdPage = hb_cdpFind( pCreateInfo->cdpId );
         if( !pArea->adsarea.area.cdPage )
            pArea->adsarea.area.cdPage = hb_vmCDP();
      }
      else
         pArea->adsarea.area.cdPage = hb_vmCDP();
      return HB_SUCCESS;
   }
   return HB_FAILURE;
}


static HB_ERRCODE adsxOpen( ADSXAREAP pArea, LPDBOPENINFO pOpenInfo )
{
   if( SUPER_OPEN( (AREAP) pArea, pOpenInfo ) == HB_SUCCESS )
   {
      if( pOpenInfo->cdpId )
      {
         pArea->adsarea.area.cdPage = hb_cdpFind( pOpenInfo->cdpId );
         if( !pArea->adsarea.area.cdPage )
            pArea->adsarea.area.cdPage = hb_vmCDP();
      }
      else
         pArea->adsarea.area.cdPage = hb_vmCDP();
      return HB_SUCCESS;
   }
   return HB_FAILURE;
}


static HB_ERRCODE adsxStructSize( ADSXAREAP pArea, USHORT* StructSize )
{
   HB_SYMBOL_UNUSED( pArea );

   * StructSize = sizeof( ADSXAREA );
   return HB_SUCCESS;
}


static HB_ERRCODE adsxOrderListFocus( ADSXAREAP pArea, LPDBORDERINFO pOrderInfo )
{
   if( SUPER_ORDLSTFOCUS( (AREAP) pArea, pOrderInfo ) == HB_SUCCESS )
   {
      if( pArea->pTagCurrent )
      {
         pOrderInfo->itmResult = hb_itemPutC( pOrderInfo->itmResult, pArea->pTagCurrent->szName );
         if( pOrderInfo->itmOrder )
            pArea->pTagCurrent = NULL;
      }
      return HB_SUCCESS;
   }

   if( pArea->pTagCurrent )
      pOrderInfo->itmResult = hb_itemPutC( pOrderInfo->itmResult, pArea->pTagCurrent->szName );

   pArea->pTagCurrent = mixFindTag( pArea, pOrderInfo->itmOrder );
   pArea->adsarea.hOrdCurrent = 0;

   if( pArea->pTagCurrent )
      return HB_SUCCESS;
   else
      return HB_FAILURE;
}


static HB_ERRCODE adsxOrderCreate( ADSXAREAP pArea, LPDBORDERCREATEINFO pOrderInfo )
{
   LPMIXTAG     pTagNew, pTag;
   PHB_ITEM     pKeyItem, pForItem = NULL, pWhileItem = NULL, pResult;
   ULONG        ulRecNo;
   USHORT       uiLen;
   BYTE         bType;
   UNSIGNED16   bValidExpr;
   BOOL         bUseADS;

   /* Test key expression */
   bValidExpr = FALSE;
   AdsIsExprValid( pArea->adsarea.hTable, (UNSIGNED8*) hb_itemGetCPtr( pOrderInfo->abExpr ), &bValidExpr );
   bUseADS = bValidExpr;

   if( bUseADS && pArea->adsarea.area.lpdbOrdCondInfo )
   {
      /* Test FOR expression */
      if( pArea->adsarea.area.lpdbOrdCondInfo->abFor )
      {
         bValidExpr = FALSE;
         AdsIsExprValid( pArea->adsarea.hTable, (UNSIGNED8*) pArea->adsarea.area.lpdbOrdCondInfo->abFor, &bValidExpr );
         bUseADS = bValidExpr;
      }
      else if( pArea->adsarea.area.lpdbOrdCondInfo->itmCobFor )
      {
         bUseADS = FALSE;
      }

      /* Test WHILE expression */
      if( bUseADS )
      {
         if( pArea->adsarea.area.lpdbOrdCondInfo->abWhile )
         {
            bValidExpr = FALSE;
            AdsIsExprValid( pArea->adsarea.hTable, (UNSIGNED8*) pArea->adsarea.area.lpdbOrdCondInfo->abWhile, &bValidExpr );
            bUseADS = bValidExpr;
         }
         else if( pArea->adsarea.area.lpdbOrdCondInfo->itmCobWhile )
            bUseADS = FALSE;
      }
   }

   if( bUseADS )
   {
      return SUPER_ORDCREATE( (AREAP) pArea, pOrderInfo );
   }

   /* Obtain key codeblock */
   if( pOrderInfo->itmCobExpr )
   {
      pKeyItem = hb_itemNew( pOrderInfo->itmCobExpr );
   }
   else
   {
      if( SELF_COMPILE( (AREAP) pArea, hb_itemGetCPtr( pOrderInfo->abExpr ) ) == HB_FAILURE )
         return HB_FAILURE;
      pKeyItem = pArea->adsarea.area.valResult;
      pArea->adsarea.area.valResult = NULL;
   }

   /* Test key codeblock on EOF */
   ulRecNo = pArea->adsarea.ulRecNo;
   SELF_GOTO( (AREAP) pArea, 0 );
   if( SELF_EVALBLOCK( (AREAP) pArea, pKeyItem ) == HB_FAILURE )
   {
      hb_vmDestroyBlockOrMacro( pKeyItem );
      SELF_GOTO( (AREAP) pArea, ulRecNo );
      return HB_FAILURE;
   }

   pResult = pArea->adsarea.area.valResult;
   pArea->adsarea.area.valResult = NULL;

   switch ( hb_itemType( pResult ) )
   {
      case HB_IT_STRING:
      case HB_IT_STRING | HB_IT_MEMO:
         bType = 'C';
         uiLen = (USHORT) hb_itemGetCLen( pResult );
         if( uiLen > MIX_MAXKEYLEN )  uiLen = MIX_MAXKEYLEN;
         break;

      case HB_IT_INTEGER:
      case HB_IT_LONG:
      case HB_IT_DOUBLE:
         bType = 'N';
         uiLen = 8;
         break;

      case HB_IT_DATE:
         bType = 'D';
         uiLen = 8;
         break;

      case HB_IT_LOGICAL:
         bType = 'L';
         uiLen = 1;
         break;

      default:
         bType = 'U';
         uiLen = 0;
   }
   hb_itemRelease( pResult );

   if( bType == 'U' || uiLen == 0 )
   {
      hb_vmDestroyBlockOrMacro( pKeyItem );
      SELF_GOTO( (AREAP) pArea, ulRecNo );
      hb_mixErrorRT( pArea, bType == 'U' ? EG_DATATYPE : EG_DATAWIDTH, 1026, NULL, 0, 0 );
      return HB_FAILURE;
   }

   if( pArea->adsarea.area.lpdbOrdCondInfo )
   {
      /* Obtain FOR codeblock */
      if( pArea->adsarea.area.lpdbOrdCondInfo->itmCobFor )
      {
         pForItem = hb_itemNew( pArea->adsarea.area.lpdbOrdCondInfo->itmCobFor );
      }
      else if( pArea->adsarea.area.lpdbOrdCondInfo->abFor )
      {
         if( SELF_COMPILE( (AREAP) pArea, pArea->adsarea.area.lpdbOrdCondInfo->abFor ) == HB_FAILURE )
         {
            hb_vmDestroyBlockOrMacro( pKeyItem );
            SELF_GOTO( (AREAP) pArea, ulRecNo );
            return HB_FAILURE;
         }
         pForItem = pArea->adsarea.area.valResult;
         pArea->adsarea.area.valResult = NULL;
      }

      /* Obtain WHILE codeblock */
      if( pArea->adsarea.area.lpdbOrdCondInfo->itmCobWhile )
      {
         pWhileItem = hb_itemNew( pArea->adsarea.area.lpdbOrdCondInfo->itmCobWhile );
      }
      else if( pArea->adsarea.area.lpdbOrdCondInfo->abWhile )
      {
         if( SELF_COMPILE( (AREAP) pArea, pArea->adsarea.area.lpdbOrdCondInfo->abWhile ) == HB_FAILURE )
         {
            hb_vmDestroyBlockOrMacro( pKeyItem );
            if( pForItem )
               hb_vmDestroyBlockOrMacro( pForItem );
            SELF_GOTO( (AREAP) pArea, ulRecNo );
            return HB_FAILURE;
         }
         pWhileItem = pArea->adsarea.area.valResult;
         pArea->adsarea.area.valResult = NULL;
      }
   }

   /* Test FOR codeblock on EOF */
   if( pForItem )
   {
      if( SELF_EVALBLOCK( (AREAP) pArea, pForItem ) == HB_FAILURE )
      {
         hb_vmDestroyBlockOrMacro( pKeyItem );
         hb_vmDestroyBlockOrMacro( pForItem );
         if( pWhileItem )
            hb_vmDestroyBlockOrMacro( pWhileItem );
         SELF_GOTO( (AREAP) pArea, ulRecNo );
         return HB_FAILURE;
      }
      if( hb_itemType( pArea->adsarea.area.valResult ) != HB_IT_LOGICAL )
      {
         hb_itemRelease( pArea->adsarea.area.valResult );
         pArea->adsarea.area.valResult = 0;
         hb_vmDestroyBlockOrMacro( pKeyItem );
         hb_vmDestroyBlockOrMacro( pForItem );
         if( pWhileItem )
            hb_vmDestroyBlockOrMacro( pWhileItem );
         SELF_GOTO( (AREAP) pArea, ulRecNo );
         hb_mixErrorRT( pArea, EG_DATATYPE, EDBF_INVALIDFOR, NULL, 0, 0 );
         return HB_FAILURE;
      }
      hb_itemRelease( pArea->adsarea.area.valResult );
      pArea->adsarea.area.valResult = NULL;
   }

   /* TODO: WHILE condition is not tested, like in DBFCDX. Why? Compatibility with Clipper? */

   SELF_GOTO( (AREAP) pArea, ulRecNo );

   pTagNew = mixTagCreate( pOrderInfo->atomBagName, pOrderInfo->abExpr, pKeyItem,
                           pForItem, pWhileItem, bType, uiLen, pArea );

   if( pWhileItem )
      hb_vmDestroyBlockOrMacro( pWhileItem );

   /* Append the tag to the end of list */
   if( pArea->pTagList )
   {
      pTag = pArea->pTagList;
      while( pTag->pNext )
         pTag = pTag->pNext;
      pTag->pNext = pTagNew;
   }
   else
   {
      pArea->pTagList = pTagNew;
   }

   pArea->pTagCurrent = pTagNew;
   pArea->adsarea.hOrdCurrent = 0;

   return HB_SUCCESS;
}


static HB_ERRCODE adsxOrderDestroy( ADSXAREAP pArea, LPDBORDERINFO pOrderInfo )
{
   LPMIXTAG     pTag, pTag2;

   /* TODO: ADS RDD missing implementation of ordDestroy( nOrder ) */
   if( SUPER_ORDDESTROY( (AREAP) pArea, pOrderInfo ) == HB_SUCCESS )
      return HB_SUCCESS;

   pTag = mixFindTag( pArea, pOrderInfo->itmOrder );

   if( pTag )  {

      if( pTag == pArea->pTagList )
         pArea->pTagList = pTag->pNext;
      else
      {
         pTag2 = pArea->pTagList;
         while( pTag2->pNext != pTag )
            pTag2 = pTag2->pNext;
         pTag2->pNext = pTag->pNext;
      }

      if( pTag == pArea->pTagCurrent )
         pArea->pTagCurrent = NULL;

      mixTagDestroy( pTag );
      return HB_SUCCESS;
   }
   else
   {
      pArea->adsarea.hOrdCurrent = 0;
      return HB_FAILURE;
   }
}


static HB_ERRCODE adsxOrderInfo( ADSXAREAP pArea, USHORT uiIndex, LPDBORDERINFO pOrderInfo )
{
   LPMIXTAG        pTag = pArea->pTagCurrent;

   if( ! pTag && uiIndex != DBOI_ORDERCOUNT )
      return SUPER_ORDINFO( (AREAP) pArea, uiIndex, pOrderInfo );

   /* resolve any pending relations */
   if( pArea->adsarea.lpdbPendingRel )
      SELF_FORCEREL( ( AREAP ) pArea );

   switch( uiIndex )
   {
      case DBOI_CONDITION:
         hb_itemPutC( pOrderInfo->itmResult, pTag->szForExpr );
         break;

      case DBOI_EXPRESSION:
         hb_itemPutC( pOrderInfo->itmResult, pTag->szKeyExpr );
         break;

      case DBOI_ISCOND:
         hb_itemPutL( pOrderInfo->itmResult, pTag->pForItem != NULL );
         break;

      case DBOI_ISDESC:
         hb_itemPutL( pOrderInfo->itmResult, FALSE );
         break;

      case DBOI_UNIQUE:
         hb_itemPutL( pOrderInfo->itmResult, FALSE );
         break;

      case DBOI_KEYTYPE:
         hb_itemPutCL( pOrderInfo->itmResult, ( char * ) &pTag->bType, 1 );
         break;

      case DBOI_KEYSIZE:
         hb_itemPutNI( pOrderInfo->itmResult, pTag->uiLen );
         break;

      case DBOI_KEYVAL:
         /* TODO: */
         break;

      case DBOI_KEYCOUNT :
      case DBOI_KEYCOUNTRAW :           /* ignore filter but RESPECT SCOPE */
         hb_itemPutNL( pOrderInfo->itmResult, pTag->ulRecCount );
         break;

      case DBOI_POSITION :
      case DBOI_RECNO :
      case DBOI_KEYNORAW :
         if( uiIndex == DBOI_POSITION && pOrderInfo->itmNewVal && HB_IS_NUMERIC( pOrderInfo->itmNewVal ) )
         {
            ULONG      ulPos;

            ulPos = hb_itemGetNL( pOrderInfo->itmNewVal ) ;

            if( ulPos > 0 && ulPos <= pTag->ulRecCount )
               SELF_GOTO( (AREAP) pArea, pTag->pKeys[ ulPos - 1 ]->rec );

            pOrderInfo->itmResult = hb_itemPutL( pOrderInfo->itmResult, !pArea->adsarea.area.fEof );
         }
         else
         {
            LPMIXKEY   pKey;
            ULONG      ulKeyPos;

            if( !pArea->adsarea.fPositioned )
               SELF_GOTO( (AREAP) pArea, pArea->adsarea.ulRecNo );
            else
               pArea->adsarea.area.fEof = FALSE;

            pKey = mixKeyEval( pTag, pArea );

            hb_itemPutNL( pOrderInfo->itmResult,
                          mixFindKey( pTag, pKey, &ulKeyPos ) ? (ulKeyPos + 1) : 0 );
            mixKeyFree( pKey );
         }
         break;

      case DBOI_RELKEYPOS:
         if( pOrderInfo->itmNewVal && HB_IS_NUMERIC( pOrderInfo->itmNewVal ) )
         {
            ULONG      ulPos;

            ulPos = (ULONG) ( hb_itemGetND( pOrderInfo->itmNewVal ) * (double) pTag->ulRecCount );

            if( ulPos > 0 && ulPos <= pTag->ulRecCount )
               SELF_GOTO( (AREAP) pArea, pTag->pKeys[ ulPos - 1 ]->rec );

            pOrderInfo->itmResult = hb_itemPutL( pOrderInfo->itmResult, !pArea->adsarea.area.fEof );
         }
         else
         {
            LPMIXKEY   pKey;
            ULONG      ulKeyPos;

            if( !pArea->adsarea.fPositioned )
               SELF_GOTO( (AREAP) pArea, pArea->adsarea.ulRecNo );
            else
               pArea->adsarea.area.fEof = FALSE;

            pKey = mixKeyEval( pTag, pArea );

            if( ! mixFindKey( pTag, pKey, &ulKeyPos + 1 )  )
              ulKeyPos = 0;

            mixKeyFree( pKey );

            pOrderInfo->itmResult = hb_itemPutND( pOrderInfo->itmResult,
                                                  (double) ulKeyPos / (double) pTag->ulRecCount );
         }
         break;

      case DBOI_NAME:
         hb_itemPutC( pOrderInfo->itmResult, pTag->szName );
         break;

      case DBOI_BAGNAME:
         hb_itemPutC( pOrderInfo->itmResult, NULL );
         break;

      case DBOI_FULLPATH :
         hb_itemPutC( pOrderInfo->itmResult, NULL );
         break;

      case DBOI_BAGEXT:
         hb_itemPutC( pOrderInfo->itmResult, "mix" );
         break;

      case DBOI_ORDERCOUNT:
      {
         UNSIGNED16      usOrder = 0;

         AdsGetNumIndexes( pArea->adsarea.hTable, &usOrder );
         pTag = pArea->pTagList;
         while( pTag )
         {
            pTag = pTag->pNext;
            usOrder++;
         }
         hb_itemPutNI( pOrderInfo->itmResult, (int) usOrder );
         break;
      }

      case DBOI_NUMBER :
      {
         LPMIXTAG        pTag2;
         UNSIGNED16      usOrder = 0;

         AdsGetNumIndexes( pArea->adsarea.hTable, &usOrder );
         pTag2 = pArea->pTagList;
         usOrder++;
         while ( pTag2 && pTag != pTag2 )
         {
            pTag2 = pTag2->pNext;
            usOrder++;
         }
         hb_itemPutNI( pOrderInfo->itmResult, (int) usOrder );
         break;
      }

      case DBOI_CUSTOM :
         hb_itemPutL( pOrderInfo->itmResult, FALSE );
         break;

      case DBOI_OPTLEVEL :
         hb_itemPutNI( pOrderInfo->itmResult, DBOI_OPTIMIZED_NONE );
         break;

      case DBOI_KEYADD :
         hb_itemPutL( pOrderInfo->itmResult, FALSE );
         break;

      case DBOI_KEYDELETE :
         hb_itemPutL( pOrderInfo->itmResult, FALSE );
         break;

      case DBOI_AUTOOPEN :
         hb_itemPutL( pOrderInfo->itmResult, FALSE );
         break;

      default:
         return SUPER_ORDINFO( ( AREAP ) pArea, uiIndex, pOrderInfo );
   }
   return HB_SUCCESS;
}



static RDDFUNCS adsxTable = { NULL,
                              NULL,
                              NULL,
                              ( DBENTRYP_V ) adsxGoBottom,
                              NULL,
                              NULL,
                              ( DBENTRYP_V ) adsxGoTop,
                              ( DBENTRYP_BIB ) adsxSeek,
                              ( DBENTRYP_L ) adsxSkip,
                              NULL,
                              NULL,
                              NULL,
                              NULL,
                              NULL,
                              NULL,
                              NULL,
                              NULL,
                              NULL,
                              NULL,
                              NULL,
                              NULL,
                              NULL,
                              NULL,
                              NULL,
                              NULL,
                              NULL,
                              NULL,
                              NULL,
                              NULL,
                              NULL,
                              NULL,
                              NULL,
                              NULL,
                              NULL,
                              NULL,
                              ( DBENTRYP_V ) adsxClose,
                              ( DBENTRYP_VO ) adsxCreate,
                              NULL,
                              NULL,
                              ( DBENTRYP_VO ) adsxOpen,
                              NULL,
                              ( DBENTRYP_SP ) adsxStructSize,
                              NULL,
                              NULL,
                              NULL,
                              NULL,
                              NULL,
                              NULL,
                              NULL,
                              NULL,
                              NULL,
                              NULL,
                              NULL,
                              NULL,
                              NULL,
                              NULL,
                              NULL,
                              NULL,
                              NULL,
                              NULL,
                              NULL,
                              NULL,
                              NULL,
                              ( DBENTRYP_VOI ) adsxOrderListFocus,
                              NULL,
                              NULL,
                              ( DBENTRYP_VOC ) adsxOrderCreate,
                              ( DBENTRYP_VOI ) adsxOrderDestroy,
                              ( DBENTRYP_SVOI ) adsxOrderInfo,
                              NULL,
                              NULL,
                              NULL,
                              NULL,
                              NULL,
                              NULL,
                              NULL,
                              NULL,
                              NULL,
                              NULL,
                              NULL,
                              NULL,
                              NULL,
                              NULL,
                              NULL,
                              NULL,
                              NULL,
                              NULL,
                              NULL,
                              NULL,
                              NULL,
                              NULL,
                              NULL,
                              NULL,
                              NULL,
                              NULL,
                              NULL,
                              NULL,
                              NULL,
                              NULL,
                              NULL  };


static void adsxRegisterRDD( USHORT * pusRddId )
{
   RDDFUNCS * pTable;
   USHORT * uiCount, uiRddId;

   uiCount = ( USHORT * ) hb_itemGetPtr( hb_param( 1, HB_IT_POINTER ) );
   * uiCount = RDDFUNCSCOUNT;
   pTable = ( RDDFUNCS * ) hb_itemGetPtr( hb_param( 2, HB_IT_POINTER ) );
   uiRddId = ( USHORT ) hb_parni( 4 );

   if( pTable )
   {
      HB_ERRCODE errCode;

      errCode = hb_rddInherit( pTable, &adsxTable, &adsxSuper, "ADS" );
      if( errCode == HB_SUCCESS )
      {
         *pusRddId = uiRddId;
      }
      hb_retni( errCode );
   }
   else
   {
      hb_retni( HB_FAILURE );
   }
}


HB_FUNC( ADSX_GETFUNCTABLE )
{
   adsxRegisterRDD( &s_uiRddIdADSX );
}


HB_FUNC( ADSX ) { ; }

HB_FUNC_EXTERN( ADS );

static void hb_adsxRddInit( void * cargo )
{
   HB_SYMBOL_UNUSED( cargo );

   if( hb_rddRegister( "ADSX", RDT_FULL ) > 1 )
   {
      /* try different RDD registrer order */
      hb_rddRegister( "ADS", RDT_FULL );

      if ( hb_rddRegister( "ADSX", RDT_FULL ) > 1 )
      {
         hb_errInternal( HB_EI_RDDINVALID, NULL, NULL, NULL );
         /* not executed, only to force linking ADS RDD */
         HB_FUNC_EXEC( ADS );
      }
   }
}

HB_INIT_SYMBOLS_BEGIN( adsx1__InitSymbols )
{ "ADSX",              {HB_FS_PUBLIC|HB_FS_LOCAL}, {HB_FUNCNAME( ADSX )}, NULL },
{ "ADSX_GETFUNCTABLE", {HB_FS_PUBLIC|HB_FS_LOCAL}, {HB_FUNCNAME( ADSX_GETFUNCTABLE )}, NULL }
HB_INIT_SYMBOLS_END( adsx1__InitSymbols )

HB_CALL_ON_STARTUP_BEGIN( _hb_adsx_rdd_init_ )
   hb_vmAtInit( hb_adsxRddInit, NULL );
HB_CALL_ON_STARTUP_END( _hb_adsx_rdd_init_ )

#if defined( HB_PRAGMA_STARTUP )
   #pragma startup adsx1__InitSymbols
   #pragma startup _hb_adsx_rdd_init_
#elif defined( HB_MSC_STARTUP )
   #if defined( HB_OS_WIN_64 )
      #pragma section( HB_MSC_START_SEGMENT, long, read )
   #endif
   #pragma data_seg( HB_MSC_START_SEGMENT )
   static HB_$INITSYM hb_vm_auto_adsx1__InitSymbols = adsx1__InitSymbols;
   static HB_$INITSYM hb_vm_auto_adsx_rdd_init = _hb_adsx_rdd_init_;
   #pragma data_seg()
#endif
