/*
 * $Id$
 */

/*
 * SQL Base Database Driver
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

#include "hbapi.h"
#include "hbapiitm.h"
#include "hbvm.h"
#include "hbset.h"
#include "hbrddsql.h"
#include "rddsys.ch"


#define SUPERTABLE ( &sqlbaseSuper )

#define CONNECTION_LIST_EXPAND    4


/*==================== SDD registration =====================================*/

static PSDDNODE s_pSdd = NULL;


int hb_sddRegister( PSDDNODE pSdd )
{
   PSDDNODE pNode = s_pSdd;

   while ( pNode )
   {
      if ( ! hb_stricmp( pNode->Name, pSdd->Name ) )
         return 0;
      pNode = pNode->pNext;
   }
   pSdd->pNext = s_pSdd;
   s_pSdd = pSdd;
   return 1;
}



/*==================== SQLBASE code ===========================================*/

static USHORT             s_rddidSQLBASE = 0;
static SQLDDCONNECTION*   s_pConnection = NULL;
static ULONG              s_ulConnectionCount = 0;
static ULONG              s_ulConnectionCurrent = 0;

static RDDFUNCS sqlbaseSuper;


static HB_ERRCODE hb_errRT_SQLBASE( ULONG ulGenCode, ULONG ulSubCode, const char * szDescription, const char * szOperation )
{
   PHB_ITEM pError;
   HB_ERRCODE iRet = HB_FAILURE;

   if ( hb_vmRequestQuery() == 0 )
   {
     pError = hb_errRT_New( ES_ERROR, "SQLBASE", ulGenCode, ulSubCode, szDescription, szOperation, 0, EF_NONE );
     iRet = hb_errLaunch( pError );
     hb_itemRelease( pError );
   }
   return iRet;
}


/*============= RDD METHODS =============================================================*/

static HB_ERRCODE sqlbaseGoBottom( SQLBASEAREAP pArea )
{
   if ( SELF_GOCOLD( (AREAP) pArea ) == HB_FAILURE )
      return HB_FAILURE;


   if ( ! pArea->fFetched && pArea->pSDD->GoTo( pArea, (ULONG) -1 ) == HB_FAILURE )
      return HB_FAILURE;

   pArea->fTop = FALSE;
   pArea->fBottom = TRUE;

   if ( SELF_GOTO( (AREAP) pArea, pArea->ulRecCount ) != HB_SUCCESS )
      return HB_FAILURE;

   return SELF_SKIPFILTER( (AREAP) pArea, -1 );
}


static HB_ERRCODE sqlbaseGoTo( SQLBASEAREAP pArea, ULONG ulRecNo )
{
   if ( SELF_GOCOLD( (AREAP) pArea ) == HB_FAILURE )
      return HB_FAILURE;

   if ( pArea->pSDD->GoTo( pArea, ulRecNo ) == HB_FAILURE )
      return HB_FAILURE;

   if ( pArea->fPositioned )
   {
      pArea->ulRecNo = ulRecNo;
      pArea->fBof = pArea->fEof = FALSE;
   }
   else
   {
      pArea->ulRecNo = pArea->ulRecCount + 1;
      pArea->fBof = pArea->fEof = TRUE;
   }
   pArea->fFound = FALSE;

   return HB_SUCCESS;
}


static HB_ERRCODE sqlbaseGoToId( SQLBASEAREAP pArea, PHB_ITEM pItem )
{
   PHB_ITEM pError;

   if ( HB_IS_NUMERIC( pItem ) )
      return SELF_GOTO( (AREAP) pArea, hb_itemGetNL( pItem ) );
   else
   {
      pError = hb_errNew();
      hb_errPutGenCode( pError, EG_DATATYPE );
      hb_errPutDescription( pError, hb_langDGetErrorDesc( EG_DATATYPE ) );
      hb_errPutSubCode( pError, EDBF_DATATYPE );
      SELF_ERROR( (AREAP) pArea, pError );
      hb_itemRelease( pError );
      return HB_FAILURE;
   }
}


static HB_ERRCODE sqlbaseGoTop( SQLBASEAREAP pArea )
{
   pArea->fTop = TRUE;
   pArea->fBottom = FALSE;

   if ( SELF_GOTO( (AREAP) pArea, 1 ) == HB_FAILURE )
      return HB_FAILURE;

   return SELF_SKIPFILTER( (AREAP) pArea, 1 );
}


static HB_ERRCODE sqlbaseSkip( SQLBASEAREAP pArea, LONG lToSkip )
{
   HB_ERRCODE uiError;

   if ( pArea->lpdbPendingRel )
   {
      if ( SELF_FORCEREL( ( AREAP ) pArea ) != HB_SUCCESS )
         return HB_FAILURE;
   }

   pArea->fTop = pArea->fBottom = FALSE;

   if ( lToSkip == 0 || hb_setGetDeleted() ||
       pArea->dbfi.itmCobExpr || pArea->dbfi.fFilter )
      return SUPER_SKIP( (AREAP) pArea, lToSkip );

   uiError = SELF_SKIPRAW( (AREAP) pArea, lToSkip );

   /* Move first record and set Bof flag */
   if ( uiError == HB_SUCCESS && pArea->fBof && lToSkip < 0 )
   {
      uiError = SELF_GOTOP( (AREAP) pArea );
      pArea->fBof = TRUE;
   }

   if ( lToSkip < 0 )
      pArea->fEof = FALSE;
   else /* if ( lToSkip > 0 ) */
      pArea->fBof = FALSE;

   return uiError;
}


static HB_ERRCODE sqlbaseSkipRaw( SQLBASEAREAP pArea, LONG lToSkip )
{
   HB_ERRCODE uiError;

   if ( pArea->lpdbPendingRel )
   {
      if ( SELF_FORCEREL( ( AREAP ) pArea ) != HB_SUCCESS )
         return HB_FAILURE;
   }

   if ( lToSkip == 0 )
   {
      /* TODO: maybe gocold is enough here?! */
      BOOL bBof, bEof;

      /* Save flags */
      bBof = pArea->fBof;
      bEof = pArea->fEof;

      uiError = SELF_GOTO( (AREAP) pArea, pArea->ulRecNo );

      /* Restore flags */
      pArea->fBof = bBof;
      pArea->fEof = bEof;
   }
   else if ( lToSkip < 0 && (ULONG) ( -lToSkip ) >= pArea->ulRecNo )
   {
      uiError = SELF_GOTO( (AREAP) pArea, 1 );
      pArea->fBof = TRUE;
   }
   else
   {
      uiError = SELF_GOTO( (AREAP) pArea, pArea->ulRecNo + lToSkip );
   }

   return uiError;
}


static HB_ERRCODE sqlbaseAppend( SQLBASEAREAP pArea, BOOL bUnLockAll )
{
/*
   PHB_ITEM   pArray;
   PHB_ITEM   pItem;
   USHORT     us;
*/

   HB_SYMBOL_UNUSED( bUnLockAll );

   /* This GOTO is GOCOLD + GOEOF */
   if ( SELF_GOTO( ( AREAP ) pArea, 0 ) == HB_FAILURE )
      return HB_FAILURE;

   if ( ! pArea->fRecordChanged && SELF_GOHOT( ( AREAP ) pArea ) == HB_FAILURE )
      return HB_FAILURE;

   if ( pArea->ulRecCount + 1 >= pArea->ulRecMax )
   {
      pArea->pRow = (void**) hb_xrealloc( pArea->pRow, ( pArea->ulRecMax + SQLDD_ROWSET_RESIZE ) * sizeof( void* ) );
      pArea->pRowFlags = (BYTE*) hb_xrealloc( pArea->pRowFlags, ( pArea->ulRecMax + SQLDD_ROWSET_RESIZE ) * sizeof( BYTE ) );
      pArea->ulRecMax += SQLDD_ROWSET_RESIZE;
   }

/*  Sita padaro GOHOT
   pArray = hb_itemNew( NULL );
   hb_arrayNew( pArray, pArea->uiFieldCount );
   for ( us = 1; us <= pArea->uiFieldCount; us++ )
   {
      pItem = hb_itemNew( NULL );
      if ( SELF_GETVALUE( (AREAP) pArea, us, pItem ) == HB_SUCCESS )
         hb_arraySetForward( pArray, us, pItem );

      hb_itemRelease( pItem );
   }
   pArea->pRecord = pArray;
   pArea->bRecordFlags = SQLDD_FLAG_CACHED;
   pArea->fRecordChanged = TRUE;
*/
   pArea->fAppend = pArea->fPositioned = TRUE;
   pArea->ulRecCount++;
   pArea->ulRecNo = pArea->ulRecCount;
   pArea->fBof = pArea->fEof = pArea->fFound = FALSE;
   return HB_SUCCESS;
}


static HB_ERRCODE sqlbaseDeleteRec( SQLBASEAREAP pArea )
{
   if ( ! pArea->fPositioned )
      return HB_SUCCESS;

   if ( ! pArea->fRecordChanged && SELF_GOHOT( ( AREAP ) pArea ) == HB_FAILURE )
      return HB_FAILURE;

   pArea->bRecordFlags |= SQLDD_FLAG_DELETED;
   return HB_SUCCESS;
}


static HB_ERRCODE sqlbaseDeleted( SQLBASEAREAP pArea, BOOL* pDeleted )
{
   * pDeleted = pArea->bRecordFlags & SQLDD_FLAG_DELETED;
   return HB_SUCCESS;
}

static HB_ERRCODE sqlbaseGetValue( SQLBASEAREAP pArea, USHORT uiIndex, PHB_ITEM pItem )
{
   if ( pArea->bRecordFlags & SQLDD_FLAG_CACHED )
   {
      hb_arrayGet( pArea->pRecord, uiIndex, pItem );
      return HB_SUCCESS;
   }
   return pArea->pSDD->GetValue( pArea, uiIndex, pItem );
}


static HB_ERRCODE sqlbaseGetVarLen( SQLBASEAREAP pArea, USHORT uiIndex, ULONG * pLength )
{
  /*  TODO: should we use this code?
  if ( pArea->lpFields[ uiIndex ].uiType == HB_IT_MEMO )
  {
     return pArea->pSDD->GetVarLen( pArea, uiIndex, pLength );
  }
  */

  * pLength = pArea->lpFields[ uiIndex - 1 ].uiLen;
  return HB_SUCCESS;
}


static HB_ERRCODE sqlbaseGoCold( SQLBASEAREAP pArea )
{
   if ( pArea->fRecordChanged )
   {
      if ( ! pArea->fAppend && pArea->pRowFlags[ pArea->ulRecNo ] & SQLDD_FLAG_CACHED )
      {
         hb_itemRelease( (PHB_ITEM) ( pArea->pRow[ pArea->ulRecNo ] ) );
      }
      pArea->pRow[ pArea->ulRecNo ] = pArea->pRecord;
      pArea->pRowFlags[ pArea->ulRecNo ] = pArea->bRecordFlags;
      pArea->fRecordChanged = FALSE;
      pArea->fAppend = FALSE;
   }
   return HB_SUCCESS;
}


static HB_ERRCODE sqlbaseGoHot( SQLBASEAREAP pArea )
{
   PHB_ITEM   pArray, pItem;
   USHORT     us;

   pArray = hb_itemNew( NULL );
   hb_arrayNew( pArray, pArea->uiFieldCount );
   for ( us = 1; us <= pArea->uiFieldCount; us++ )
   {
      pItem = hb_itemNew( NULL );
      if ( SELF_GETVALUE( (AREAP) pArea, us, pItem ) == HB_SUCCESS )
         hb_arraySetForward( pArray, us, pItem );
      hb_itemRelease( pItem );
   }
   pArea->pRecord = pArray;
   pArea->bRecordFlags |= SQLDD_FLAG_CACHED;
   pArea->fRecordChanged = TRUE;
   return HB_SUCCESS;
}


static HB_ERRCODE sqlbasePutValue( SQLBASEAREAP pArea, USHORT uiIndex, PHB_ITEM pItem )
{
   LPFIELD    pField;
   HB_ERRCODE    uiError;

   if ( ! pArea->fPositioned )
      return HB_SUCCESS;

   if ( ! pArea->fRecordChanged && SELF_GOHOT( ( AREAP ) pArea ) == HB_FAILURE )
      return HB_FAILURE;

   uiError = HB_SUCCESS;
   pField = pArea->lpFields + ( uiIndex - 1 );

   if ( ( ( HB_IS_MEMO( pItem ) || HB_IS_STRING( pItem ) ) && pField->uiType == HB_FT_STRING ) ||
        ( HB_IS_DATE( pItem ) && pField->uiType == HB_FT_DATE ) ||
        ( HB_IS_NUMBER( pItem ) && ( pField->uiType == HB_FT_INTEGER || pField->uiType == HB_FT_LONG ||
                                     pField->uiType == HB_FT_FLOAT || pField->uiType == HB_FT_DOUBLE ) ) ||
        ( HB_IS_LOGICAL( pItem ) && pField->uiType == HB_FT_LOGICAL ) ||
        HB_IS_NIL( pItem ) )
   {
       hb_arraySet( pArea->pRecord, uiIndex, pItem );
   }
   else
   {
      PHB_ITEM  pError;

      pError = hb_errNew();
      hb_errPutGenCode( pError, EG_DATATYPE );
      hb_errPutDescription( pError, hb_langDGetErrorDesc( EG_DATATYPE ) );
      hb_errPutOperation( pError, hb_dynsymName( (PHB_DYNS) pField->sym ) );
      hb_errPutSubCode( pError, uiError );
      hb_errPutFlags( pError, EF_CANDEFAULT );
      uiError = SELF_ERROR( (AREAP) pArea, pError );
      hb_itemRelease( pError );
      return uiError == E_DEFAULT ? HB_SUCCESS : HB_FAILURE;
   }
   return HB_SUCCESS;
}


static HB_ERRCODE sqlbaseRecall( SQLBASEAREAP pArea )
{
   if ( ! pArea->fPositioned )
      return HB_SUCCESS;

   if ( ! pArea->fRecordChanged && SELF_GOHOT( ( AREAP ) pArea ) != HB_SUCCESS )
      return HB_FAILURE;

   pArea->bRecordFlags &= ~SQLDD_FLAG_DELETED;
   return HB_SUCCESS;
}


static HB_ERRCODE sqlbaseRecCount( SQLBASEAREAP pArea, ULONG * pRecCount )
{
   * pRecCount = pArea->ulRecCount;
   return HB_SUCCESS;
}


static HB_ERRCODE sqlbaseRecNo( SQLBASEAREAP pArea, ULONG * ulRecNo )
{
   * ulRecNo = pArea->ulRecNo;
   return HB_SUCCESS;
}


static HB_ERRCODE sqlbaseRecId( SQLBASEAREAP pArea, PHB_ITEM pRecNo )
{
   HB_ERRCODE errCode;
   ULONG ulRecNo;

   errCode = SELF_RECNO( (AREAP) pArea, &ulRecNo );
   hb_itemPutNInt( pRecNo, ulRecNo );
   return errCode;
}


static HB_ERRCODE sqlbaseClose( SQLBASEAREAP pArea )
{
   if ( SELF_GOCOLD( (AREAP) pArea ) == HB_FAILURE )
      return HB_FAILURE;

   if ( SUPER_CLOSE( (AREAP) pArea ) == HB_FAILURE )
      return HB_FAILURE;

   pArea->pSDD->Close( pArea );

   if ( pArea->pRow )
   {
      ULONG    ulIndex;

      for ( ulIndex = 0; ulIndex <= pArea->ulRecCount; ulIndex++ )
      {
         if ( pArea->pRowFlags[ ulIndex ] & SQLDD_FLAG_CACHED )
         {
            hb_itemRelease( (PHB_ITEM) pArea->pRow[ ulIndex ] );
         }
      }
      hb_xfree( pArea->pRow );
      hb_xfree( pArea->pRowFlags );
   }

   if ( pArea->szQuery )
     hb_xfree( pArea->szQuery );

   return HB_SUCCESS;
}


static HB_ERRCODE sqlbaseCreate( SQLBASEAREAP pArea, LPDBOPENINFO pOpenInfo )
{
   PHB_ITEM    pItemEof, pItem;
   USHORT      uiCount;
   BOOL        bError;

   pArea->ulConnection = pOpenInfo->ulConnection ? pOpenInfo->ulConnection : s_ulConnectionCurrent;

   if ( pArea->ulConnection > s_ulConnectionCount ||
        ( pArea->ulConnection && ! s_pConnection[ pArea->ulConnection - 1 ].hConnection ) )
   {
      hb_errRT_SQLBASE( EG_OPEN, ESQLDD_NOTCONNECTED, "Not connected", NULL );
      return HB_FAILURE;
   }

   if ( pArea->ulConnection )
   {
      pArea->pConnection = & s_pConnection[ pArea->ulConnection - 1 ];
      pArea->pSDD = pArea->pConnection->pSDD;
   }

   pItemEof = hb_itemArrayNew( pArea->uiFieldCount );

   bError = FALSE;
   for ( uiCount = 0; uiCount < pArea->uiFieldCount; uiCount++  )
   {
      LPFIELD pField = pArea->lpFields + uiCount;

      switch ( pField->uiType )
      {
         case HB_FT_STRING:
         {
            char*    pStr;

            pStr = (char*) hb_xgrab( pField->uiLen + 1 );
            memset( pStr, ' ', pField->uiLen );
            pStr[ pField->uiLen ] = '\0';

            pItem = hb_itemPutCL( NULL, pStr, pField->uiLen );
            hb_xfree( pStr );
            break;
         }

         case HB_FT_MEMO:
            pItem = hb_itemPutC( NULL, "" );
            break;

         case HB_FT_INTEGER:
            if ( pField->uiDec )
               pItem = hb_itemPutND( NULL, 0.0 );
            else
               pItem = hb_itemPutNI( NULL, 0 );
            break;

         case HB_FT_LONG:
            if ( pField->uiDec )
               pItem = hb_itemPutND( NULL, 0.0 );
            else
               pItem = hb_itemPutNL( NULL, 0 );
            break;

         case HB_FT_FLOAT:
            pItem = hb_itemPutND( NULL, 0.0 );
            break;

         case HB_FT_DOUBLE:
            pItem = hb_itemPutND( NULL, 0.0 );
            break;

         case HB_FT_DATE:
            pItem = hb_itemPutDS( NULL, "" );
            break;

         case HB_FT_LOGICAL:
            pItem = hb_itemPutL( NULL, FALSE );
            break;

         default:
            pItem = hb_itemNew( NULL );
            bError = TRUE;
            break;
      }

      hb_arraySetForward( pItemEof, uiCount + 1, pItem );
      hb_itemRelease( pItem );

      if ( bError )
         break;
   }

   if ( bError )
   {
      hb_itemClear( pItemEof );
      hb_itemRelease( pItemEof );
      hb_errRT_SQLBASE( EG_CORRUPTION, ESQLDD_INVALIDFIELD, "Invalid field type", NULL );
      SELF_CLOSE( (AREAP) pArea );
      return HB_FAILURE;
   }

   pArea->ulRecCount = 0;

   pArea->pRow = (void**) hb_xalloc( SQLDD_ROWSET_RESIZE * sizeof( void* ) );
   pArea->pRowFlags = (BYTE*) hb_xalloc( SQLDD_ROWSET_RESIZE * sizeof( BYTE ) );
   pArea->ulRecMax = SQLDD_ROWSET_RESIZE;

   * (pArea->pRow) = pItemEof;
   pArea->pRowFlags[ 0 ] = SQLDD_FLAG_CACHED;
   pArea->fFetched = 1;

   if ( SUPER_CREATE( (AREAP) pArea, pOpenInfo ) != HB_SUCCESS )
   {
      SELF_CLOSE( (AREAP) pArea );
      return HB_FAILURE;
   }
   return SELF_GOTOP( (AREAP) pArea );
}


static HB_ERRCODE sqlbaseInfo( SQLBASEAREAP pArea, USHORT uiIndex, PHB_ITEM pItem )
{
   switch( uiIndex )
   {
      case DBI_QUERY:
         hb_itemPutC( pItem, pArea->szQuery );
         break;
   }

   return HB_SUCCESS;
}


static HB_ERRCODE sqlbaseOpen( SQLBASEAREAP pArea, LPDBOPENINFO pOpenInfo )
{
   HB_ERRCODE  errCode;

   pArea->ulConnection = pOpenInfo->ulConnection ? pOpenInfo->ulConnection : s_ulConnectionCurrent;

   if ( pArea->ulConnection <= 0 || pArea->ulConnection > s_ulConnectionCount ||
        ! s_pConnection[ pArea->ulConnection - 1 ].hConnection )
   {
      hb_errRT_SQLBASE( EG_OPEN, ESQLDD_NOTCONNECTED, "Not connected", NULL );
      return HB_FAILURE;
   }

   pArea->pConnection = & s_pConnection[ pArea->ulConnection - 1 ];
   pArea->pSDD = pArea->pConnection->pSDD;

   /* filename is a query */
   pArea->szQuery = (char*) hb_strdup( (char*) pOpenInfo->abName );

   if ( pArea->uiFieldCount )
      /* This should not happen (in __dbTrans()), because RDD is registered with RDT_FULL */
      return HB_FAILURE;

   errCode = pArea->pSDD->Open( pArea );

   if ( errCode == HB_SUCCESS )
   {
      errCode = SUPER_OPEN( (AREAP) pArea, pOpenInfo );
   }

   if( errCode != HB_SUCCESS )
   {
      SELF_CLOSE( (AREAP) pArea );
      return HB_FAILURE;
   }
   return SELF_GOTOP( (AREAP) pArea );
}


static HB_ERRCODE sqlbaseStructSize( SQLBASEAREAP pArea, USHORT * uiSize )
{
   HB_SYMBOL_UNUSED( pArea );

   * uiSize = sizeof( SQLBASEAREA );
   return HB_SUCCESS;
}


/*
static HB_ERRCODE sqlbaseChildEnd( SQLBASEAREAP pArea, LPDBRELINFO pRelInfo )
{
   HB_ERRCODE uiError;

   if ( pArea->lpdbPendingRel == pRelInfo )
      uiError = SELF_FORCEREL( (AREAP) pArea );
   else
      uiError = HB_SUCCESS;
   SUPER_CHILDEND( (AREAP) pArea, pRelInfo );
   return uiError;
}


static HB_ERRCODE sqlbaseChildStart( SQLBASEAREAP pArea, LPDBRELINFO pRelInfo )
{
   if ( SELF_CHILDSYNC( (AREAP) pArea, pRelInfo ) != HB_SUCCESS )
      return HB_FAILURE;
   return SUPER_CHILDSTART( (AREAP) pArea, pRelInfo );
}


static HB_ERRCODE sqlbaseChildSync( SQLBASEAREAP pArea, LPDBRELINFO pRelInfo )
{
   if ( SELF_GOCOLD( (AREAP) pArea ) != HB_SUCCESS )
      return HB_FAILURE;

   pArea->lpdbPendingRel = pRelInfo;

   if ( pArea->lpdbRelations )
      return SELF_SYNCCHILDREN( (AREAP) pArea );

   return HB_SUCCESS;
}


static HB_ERRCODE sqlbaseForceRel( SQLBASEAREAP pArea )
{
   if ( pArea->lpdbPendingRel )
   {
      LPDBRELINFO lpdbPendingRel;

      lpdbPendingRel = pArea->lpdbPendingRel;
      pArea->lpdbPendingRel = NULL;
      return SELF_RELEVAL( (AREAP) pArea, lpdbPendingRel );
   }
   return HB_SUCCESS;
}


static HB_ERRCODE sqlbaseSetFilter( SQLBASEAREAP pArea, LPDBFILTERINFO pFilterInfo )
{
   if ( pArea->lpdbPendingRel )
   {
      if ( SELF_FORCEREL( (AREAP) pArea ) != HB_SUCCESS )
         return HB_FAILURE;
   }
   return SUPER_SETFILTER( (AREAP) pArea, pFilterInfo );
}
*/


static HB_ERRCODE sqlbaseExit( LPRDDNODE pRDD )
{
   ULONG     ul;

   HB_SYMBOL_UNUSED( pRDD );

   if ( s_pConnection )
   {
      /* Disconnect all connections */
      for ( ul = 0; ul < s_ulConnectionCount; ul++ )
      {
         if ( s_pConnection[ ul ].hConnection )
         {
            s_pConnection[ ul ].pSDD->Disconnect( & s_pConnection[ ul ] );
            if ( s_pConnection[ ul ].szError )
               hb_xfree( s_pConnection[ ul ].szError );
            if ( s_pConnection[ ul ].szQuery )
               hb_xfree( s_pConnection[ ul ].szQuery );
            hb_itemRelease( s_pConnection[ ul ].pNewID );
         }
      }
      hb_xfree( s_pConnection );
      s_pConnection = NULL;
      s_ulConnectionCount = 0;
      s_ulConnectionCurrent = 0;
   }

   return HB_SUCCESS;
}


static HB_ERRCODE sqlbaseRddInfo( LPRDDNODE pRDD, USHORT uiIndex, ULONG ulConnect, PHB_ITEM pItem )
{
   ULONG            ulConn;
   SQLDDCONNECTION*   pConn;

   HB_SYMBOL_UNUSED( pRDD );
   /* TraceLog( NULL, "RDDINFO index: %d type:%04X\n", (int) uiIndex, pItem->type ); */

   ulConn = ulConnect ? ulConnect : s_ulConnectionCurrent;
   if ( ulConn > 0 && ulConn < s_ulConnectionCount && s_pConnection[ --ulConn ].hConnection )
      pConn = & s_pConnection[ ulConn ];
   else
      pConn = NULL;

   switch( uiIndex )
   {

      case RDDI_REMOTE:
         hb_itemPutL( pItem, TRUE );
         break;

      case RDDI_CONNECTION:
      {
         ULONG ulNewConnection = 0;

         if ( hb_itemType( pItem ) & HB_IT_NUMERIC )
         {
            ulNewConnection = hb_itemGetNL( pItem );
         }
         hb_itemPutNL( pItem, ulConnect ? ulConnect : s_ulConnectionCurrent );
         if ( ulNewConnection )
         {
            s_ulConnectionCurrent = ulNewConnection;
         }
         break;
      }

      case RDDI_ISDBF:
         hb_itemPutL( pItem, FALSE );
         break;

      case RDDI_CANPUTREC:
         hb_itemPutL( pItem, TRUE );
         break;


      case RDDI_CONNECT:
      {
         PSDDNODE   pNode = NULL;
         ULONG      ul;
         char*      pStr;

         /* Find free connection handle */
         for ( ul = 0; ul < s_ulConnectionCount; ul++ )
         {
            if ( ! s_pConnection[ ul ].hConnection )
               break;
         }

         if ( ul >= s_ulConnectionCount )
         {
            /* Realloc connection table */
            if ( s_pConnection )
               s_pConnection = (SQLDDCONNECTION*) hb_xrealloc( s_pConnection, sizeof( SQLDDCONNECTION ) * ( s_ulConnectionCount + CONNECTION_LIST_EXPAND ) );
            else
               s_pConnection = (SQLDDCONNECTION*) hb_xgrab( sizeof( SQLDDCONNECTION ) * CONNECTION_LIST_EXPAND );

            memset( &s_pConnection[ s_ulConnectionCount ], 0, sizeof( SQLDDCONNECTION ) * CONNECTION_LIST_EXPAND );
            ul = s_ulConnectionCount;
            s_ulConnectionCount += CONNECTION_LIST_EXPAND;
         }

         pStr = hb_arrayGetCPtr( pItem, 1 );
         if ( pStr )
         {
            pNode = s_pSdd;
            while ( pNode )
            {
               if ( ! hb_stricmp( pNode->Name, pStr ) )
                  break;
               pNode = pNode->pNext;
            }
         }

         pConn = & s_pConnection[ ul ];
         ul++;
         if ( pNode && pNode->Connect( pConn, pItem ) == HB_SUCCESS )
         {
            pConn->pSDD = pNode;
            pConn->pNewID = hb_itemNew( NULL );
         }
         else
            ul = 0;

         if ( ul )
            s_ulConnectionCurrent = ul;

         hb_itemPutNI( pItem, ul );
         break;
      }

      case RDDI_DISCONNECT:
      {

         if ( pConn && pConn->pSDD->Disconnect( pConn ) == HB_SUCCESS )
         {
            pConn->hConnection = 0;
            pConn->iError = 0;
            if ( pConn->szError )
            {
               hb_xfree( pConn->szError );
               pConn->szError = NULL;
            }
            if ( pConn->szQuery )
            {
               hb_xfree( pConn->szQuery );
               pConn->szQuery = NULL;
            }
            hb_itemRelease( pConn->pNewID );

            hb_itemPutL( pItem, TRUE );
            return HB_SUCCESS;
         }
         hb_itemPutL( pItem, FALSE );
         return HB_SUCCESS;
      }

      case RDDI_EXECUTE:
      {
         if ( pConn )
         {
            hb_itemClear( pConn->pNewID );
            pConn->iError = 0;
            if ( pConn->szError )
            {
               hb_xfree( pConn->szError );
               pConn->szError = NULL;
            }
            if ( pConn->szQuery )
            {
               hb_xfree( pConn->szQuery );
               pConn->szQuery = NULL;
            }
            pConn->szQuery = hb_strdup( hb_itemGetCPtr( pItem ) );

            hb_itemPutL( pItem, pConn->pSDD->Execute( pConn, pItem ) == HB_SUCCESS );
         }
         else
            hb_itemPutL( pItem, FALSE );

         return HB_SUCCESS;
      }

      case RDDI_ERROR:
      {
         if ( pConn )
            hb_itemPutC( pItem, pConn->szError );
         else
            hb_itemClear( pItem );
         return HB_SUCCESS;
      }

      case RDDI_ERRORNO:
      {
         if ( pConn )
            hb_itemPutNI( pItem, pConn->iError );
         else
            hb_itemClear( pItem );
         return HB_SUCCESS;
      }

      case RDDI_QUERY:
      {
         if ( pConn )
            hb_itemPutC( pItem, pConn->szQuery );
         else
            hb_itemClear( pItem );
         return HB_SUCCESS;
      }

      case RDDI_NEWID:
      {
         if ( pConn )
            hb_itemCopy( pItem, pConn->pNewID );
         else
            hb_itemClear( pItem );
         return HB_SUCCESS;
      }

      case RDDI_AFFECTEDROWS:
      {
         if ( pConn )
            hb_itemPutNInt( pItem, pConn->ulAffectedRows );
         else
            hb_itemClear( pItem );
         return HB_SUCCESS;
      }

/*
      default:
         return SUPER_RDDINFO( pRDD, uiIndex, ulConnect, pItem );
*/

   }

   return HB_SUCCESS;
}


/*====================================================================================*/

static RDDFUNCS sqlbaseTable =
{
   ( DBENTRYP_BP )    NULL,                       /* sqlbaseBof */
   ( DBENTRYP_BP )    NULL,                       /* sqlbaseEof */
   ( DBENTRYP_BP )    NULL,                       /* sqlbaseFound */
   ( DBENTRYP_V )     sqlbaseGoBottom,
   ( DBENTRYP_UL )    sqlbaseGoTo,
   ( DBENTRYP_I )     sqlbaseGoToId,
   ( DBENTRYP_V )     sqlbaseGoTop,
   ( DBENTRYP_BIB )   NULL,                       /* sqlbaseSeek */
   ( DBENTRYP_L )     sqlbaseSkip,
   ( DBENTRYP_L )     NULL,                       /* sqlbaseSkipFilter */
   ( DBENTRYP_L )     sqlbaseSkipRaw,
   ( DBENTRYP_VF )    NULL,                       /* sqlbaseAddField */
   ( DBENTRYP_B )     sqlbaseAppend,
   ( DBENTRYP_I )     NULL,                       /* sqlbaseCreateFields */
   ( DBENTRYP_V )     sqlbaseDeleteRec,
   ( DBENTRYP_BP )    sqlbaseDeleted,
   ( DBENTRYP_SP )    NULL,                       /* sqlbaseFieldCount */
   ( DBENTRYP_VF )    NULL,                       /* sqlbaseFieldDisplay */
   ( DBENTRYP_SSI )   NULL,                       /* sqlbaseFieldInfo */
   ( DBENTRYP_SVP )   NULL,                       /* sqlbaseFieldName */
   ( DBENTRYP_V )     NULL,                       /* sqlbaseFlush */
   ( DBENTRYP_PP )    NULL,                       /* sqlbaseGetRec */
   ( DBENTRYP_SI )    sqlbaseGetValue,
   ( DBENTRYP_SVL )   sqlbaseGetVarLen,
   ( DBENTRYP_V )     sqlbaseGoCold,
   ( DBENTRYP_V )     sqlbaseGoHot,
   ( DBENTRYP_P )     NULL,                       /* sqlbasePutRec */
   ( DBENTRYP_SI )    sqlbasePutValue,
   ( DBENTRYP_V )     sqlbaseRecall,
   ( DBENTRYP_ULP )   sqlbaseRecCount,
   ( DBENTRYP_ISI )   NULL,                       /* sqlbaseRecInfo */
   ( DBENTRYP_ULP )   sqlbaseRecNo,
   ( DBENTRYP_I )     sqlbaseRecId,
   ( DBENTRYP_S )     NULL,                       /* sqlbaseSetFieldExtent */
   ( DBENTRYP_P )     NULL,                       /* sqlbaseAlias */
   ( DBENTRYP_V )     sqlbaseClose,
   ( DBENTRYP_VP )    sqlbaseCreate,
   ( DBENTRYP_SI )    sqlbaseInfo,
   ( DBENTRYP_V )     NULL,                       /* sqlbaseNewArea */
   ( DBENTRYP_VP )    sqlbaseOpen,
   ( DBENTRYP_V )     NULL,                       /* sqlbaseRelease */
   ( DBENTRYP_SP )    sqlbaseStructSize,
   ( DBENTRYP_P )     NULL,                       /* sqlbaseSysName */
   ( DBENTRYP_VEI )   NULL,                       /* sqlbaseEval */
   ( DBENTRYP_V )     NULL,                       /* sqlbasePack */
   ( DBENTRYP_LSP )   NULL,                       /* sqlbasePackRec */
   ( DBENTRYP_VS )    NULL,                       /* sqlbaseSort */
   ( DBENTRYP_VT )    NULL,                       /* sqlbaseTrans */
   ( DBENTRYP_VT )    NULL,                       /* sqlbaseTransRec */
   ( DBENTRYP_V )     NULL,                       /* sqlbaseZap */
   ( DBENTRYP_VR )    NULL,                       /* sqlbaseChildEnd */
   ( DBENTRYP_VR )    NULL,                       /* sqlbaseChildStart */
   ( DBENTRYP_VR )    NULL,                       /* sqlbaseChildSync */
   ( DBENTRYP_V )     NULL,                       /* sqlbaseSyncChildren */
   ( DBENTRYP_V )     NULL,                       /* sqlbaseClearRel */
   ( DBENTRYP_V )     NULL,                       /* sqlbaseForceRel */
   ( DBENTRYP_SVP )   NULL,                       /* sqlbaseRelArea */
   ( DBENTRYP_VR )    NULL,                       /* sqlbaseRelEval */
   ( DBENTRYP_SVP )   NULL,                       /* sqlbaseRelText */
   ( DBENTRYP_VR )    NULL,                       /* sqlbaseSetRel */
   ( DBENTRYP_OI )    NULL,                       /* sqlbaseOrderListAdd */
   ( DBENTRYP_V )     NULL,                       /* sqlbaseOrderListClear */
   ( DBENTRYP_OI )    NULL,                       /* sqlbaseOrderListDelete */
   ( DBENTRYP_OI )    NULL,                       /* sqlbaseOrderListFocus */
   ( DBENTRYP_V )     NULL,                       /* sqlbaseOrderListRebuild */
   ( DBENTRYP_VOI )   NULL,                       /* sqlbaseOrderCondition */
   ( DBENTRYP_VOC )   NULL,                       /* sqlbaseOrderCreate */
   ( DBENTRYP_OI )    NULL,                       /* sqlbaseOrderDestroy */
   ( DBENTRYP_OII )   NULL,                       /* sqlbaseOrderInfo */
   ( DBENTRYP_V )     NULL,                       /* sqlbaseClearFilter */
   ( DBENTRYP_V )     NULL,                       /* sqlbaseClearLocate */
   ( DBENTRYP_V )     NULL,                       /* sqlbaseClearScope */
   ( DBENTRYP_VPLP )  NULL,                       /* sqlbaseCountScope */
   ( DBENTRYP_I )     NULL,                       /* sqlbaseFilterText */
   ( DBENTRYP_SI )    NULL,                       /* sqlbaseScopeInfo */
   ( DBENTRYP_VFI )   NULL,                       /* sqlbaseSetFilter */
   ( DBENTRYP_VLO )   NULL,                       /* sqlbaseSetLocate */
   ( DBENTRYP_VOS )   NULL,                       /* sqlbaseSetScope */
   ( DBENTRYP_VPL )   NULL,                       /* sqlbaseSkipScope */
   ( DBENTRYP_B )     NULL,                       /* sqlbaseLocate */
   ( DBENTRYP_P )     NULL,                       /* sqlbaseCompile */
   ( DBENTRYP_I )     NULL,                       /* sqlbaseError */
   ( DBENTRYP_I )     NULL,                       /* sqlbaseEvalBlock */
   ( DBENTRYP_VSP )   NULL,                       /* sqlbaseRawLock */
   ( DBENTRYP_VL )    NULL,                       /* sqlbaseLock */
   ( DBENTRYP_I )     NULL,                       /* sqlbaseUnLock */
   ( DBENTRYP_V )     NULL,                       /* sqlbaseCloseMemFile */
   ( DBENTRYP_VP )    NULL,                       /* sqlbaseCreateMemFile */
   ( DBENTRYP_SVPB )  NULL,                       /* sqlbaseGetValueFile */
   ( DBENTRYP_VP )    NULL,                       /* sqlbaseOpenMemFile */
   ( DBENTRYP_SVPB )  NULL,                       /* sqlbasePutValueFile */
   ( DBENTRYP_V )     NULL,                       /* sqlbaseReadDBHeader */
   ( DBENTRYP_V )     NULL,                       /* sqlbaseWriteDBHeader */
   ( DBENTRYP_R )     NULL,                       /* sqlbaseInit */
   ( DBENTRYP_R )     sqlbaseExit,
   ( DBENTRYP_RVVL )  NULL,                       /* sqlbaseDrop */
   ( DBENTRYP_RVVL )  NULL,                       /* sqlbaseExists */
   ( DBENTRYP_RSLV )  sqlbaseRddInfo,
   ( DBENTRYP_SVP )   NULL                        /* sqlbaseWhoCares */
};


static void hb_sqlbaseInit( void * cargo )
{
   HB_SYMBOL_UNUSED( cargo );

   if ( hb_rddRegister( "SQLBASE", RDT_FULL ) > 1 )
   {
      hb_errInternal( HB_EI_RDDINVALID, NULL, NULL, NULL );
   }
}


#define __PRG_SOURCE__ __FILE__

#ifdef HB_PCODE_VER
   #undef HB_PRG_PCODE_VER
   #define HB_PRG_PCODE_VER HB_PCODE_VER
#endif


HB_FUNC( SQLBASE ) {;}


HB_FUNC( SQLBASE_GETFUNCTABLE )
{
   RDDFUNCS * pTable;
   USHORT * uiCount, uiRddId;

   uiCount = ( USHORT * ) hb_itemGetPtr( hb_param( 1, HB_IT_POINTER ) );
   * uiCount = RDDFUNCSCOUNT;
   pTable = ( RDDFUNCS * ) hb_itemGetPtr( hb_param( 2, HB_IT_POINTER ) );
   uiRddId = ( USHORT ) hb_parni( 4 );

   if ( pTable )
   {
      HB_ERRCODE errCode;

      errCode = hb_rddInherit( pTable, &sqlbaseTable, &sqlbaseSuper, 0 );
      if ( errCode == HB_SUCCESS )
      {
         s_rddidSQLBASE = uiRddId;
      }
      hb_retni( errCode );
   }
   else
   {
      hb_retni( HB_FAILURE );
   }
}


HB_INIT_SYMBOLS_BEGIN( sqlbase1__InitSymbols )
{ "SQLBASE",              {HB_FS_PUBLIC}, {HB_FUNCNAME( SQLBASE )}, NULL },
{ "SQLBASE_GETFUNCTABLE", {HB_FS_PUBLIC}, {HB_FUNCNAME( SQLBASE_GETFUNCTABLE )}, NULL }
HB_INIT_SYMBOLS_END( sqlbase1__InitSymbols )

HB_CALL_ON_STARTUP_BEGIN( _hb_sqlbase_init_ )
   hb_vmAtInit( hb_sqlbaseInit, NULL );
HB_CALL_ON_STARTUP_END( _hb_sqlbase_init_ )

#if defined( HB_PRAGMA_STARTUP )
   #pragma startup sqlbase1__InitSymbols
   #pragma startup _hb_sqlbase_init_
#elif defined( HB_MSC_STARTUP )
   #if defined( HB_OS_WIN_64 )
      #pragma section( HB_MSC_START_SEGMENT, long, read )
   #endif
   #pragma data_seg( HB_MSC_START_SEGMENT )
   static HB_$INITSYM hb_vm_auto_sqlbase1__InitSymbols = sqlbase1__InitSymbols;
   static HB_$INITSYM hb_vm_auto_sqlbase_init = _hb_sqlbase_init_;
   #pragma data_seg()
#endif
