/*
 * $Id$
 */

/*
 * MySQL Database Driver
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
#include "hbsqldd.h"

#ifndef my_socket_defined
#define my_socket_defined
typedef int my_socket;
#endif

#include "mysql.h"

/* TOFIX: HACK to make it compile under MSVC to avoid 'invalid integer constant expression' errors. */

/* sizeof() inside #if will not work with any C compiler which has
 * preprocessor separated from compiler so it will not work with most of
 * existing C compilers. To make it working is necessary to mix preprocessor
 * and compiler logic so sizeof() for newly defined types by 'typedef' will
 * work. It's rather seldom situation when C compiler authors make sth like
 * that.
 */
#if defined( __BORLANDC__ )

#if sizeof( MYSQL_ROW_OFFSET ) != sizeof( void* )
   #error "MySQLDD error: sizeof( MYSQL_ROW_OFFSET ) != sizeof( void* )"
#endif

#if sizeof( MYSQL_ROW ) != sizeof( void* )
   #error "MySQLDD error: sizeof( MYSQL_ROW ) != sizeof( void* )"
#endif

#endif

#ifndef MYSQL_TYPE_NEWDECIMAL
#define MYSQL_TYPE_NEWDECIMAL   246
#endif


static ERRCODE mysqlConnect( SQLDDCONNECTION* pConnection, PHB_ITEM pItem );
static ERRCODE mysqlDisconnect( SQLDDCONNECTION* pConnection );
static ERRCODE mysqlExecute( SQLDDCONNECTION* pConnection, PHB_ITEM pItem );
static ERRCODE mysqlOpen( SQLBASEAREAP pArea );
static ERRCODE mysqlClose( SQLBASEAREAP pArea );
static ERRCODE mysqlGoTo( SQLBASEAREAP pArea, ULONG ulRecNo );
static ERRCODE mysqlGetValue( SQLBASEAREAP pArea, USHORT uiIndex, PHB_ITEM pItem );
static ERRCODE mysqlGetVarLen( SQLBASEAREAP pArea, USHORT uiIndex, ULONG * pLength );


static SDDNODE mysqldd =
{
   NULL,
   "MYSQL",
   (SDDFUNC_CONNECT)    mysqlConnect,
   (SDDFUNC_DISCONNECT) mysqlDisconnect,
   (SDDFUNC_EXECUTE)    mysqlExecute,
   (SDDFUNC_OPEN)       mysqlOpen,
   (SDDFUNC_CLOSE)      mysqlClose,
   (SDDFUNC_GOTO)       mysqlGoTo,
   (SDDFUNC_GETVALUE)   mysqlGetValue,
   (SDDFUNC_GETVARLEN)  mysqlGetVarLen
};


HB_FUNC_EXTERN( SQLBASE );


static void hb_mysqldd_init( void * cargo )
{
   HB_SYMBOL_UNUSED( cargo );

   if ( ! hb_sddRegister( & mysqldd ) )
   {
      hb_errInternal( HB_EI_RDDINVALID, NULL, NULL, NULL );
      HB_FUNC_EXEC( SQLBASE );   /* force SQLBASE linking */
   }
}


#define __PRG_SOURCE__ __FILE__

#ifdef HB_PCODE_VER
   #undef HB_PRG_PCODE_VER
   #define HB_PRG_PCODE_VER HB_PCODE_VER
#endif


HB_FUNC( MYSQLDD ) {;}


HB_INIT_SYMBOLS_BEGIN( mysqldd__InitSymbols )
{ "MYSQLDD", {HB_FS_PUBLIC}, {HB_FUNCNAME( MYSQLDD )}, NULL },
HB_INIT_SYMBOLS_END( mysqldd__InitSymbols )

HB_CALL_ON_STARTUP_BEGIN( _hb_mysqldd_init_ )
   hb_vmAtInit( hb_mysqldd_init, NULL );
HB_CALL_ON_STARTUP_END( _hb_mysqldd_init_ )

#if defined( HB_PRAGMA_STARTUP )
   #pragma startup mysqldd__InitSymbols
   #pragma startup _hb_mysqldd_init_
#elif defined( HB_MSC_STARTUP )
   #if defined( HB_OS_WIN_64 )
      #pragma section( HB_MSC_START_SEGMENT, long, read )
   #endif
   #pragma data_seg( HB_MSC_START_SEGMENT )
   static HB_$INITSYM hb_vm_auto_mysqldd__InitSymbols = mysqldd__InitSymbols;
   static HB_$INITSYM hb_vm_auto_mysqldd_init = _hb_mysqldd_init_;
   #pragma data_seg()
#endif


/*=====================================================================================*/
static USHORT hb_errRT_MySQLDD( ULONG ulGenCode, ULONG ulSubCode, const char * szDescription, const char * szOperation, USHORT uiOsCode )
{
   USHORT uiAction;
   PHB_ITEM pError;

   pError = hb_errRT_New( ES_ERROR, "MYSQLDD", ulGenCode, ulSubCode, szDescription, szOperation, uiOsCode, EF_NONE );

   uiAction = hb_errLaunch( pError );

   hb_itemRelease( pError );

   return uiAction;
}

/*============= SDD METHODS =============================================================*/

static ERRCODE mysqlConnect( SQLDDCONNECTION* pConnection, PHB_ITEM pItem )
{
   MYSQL*   pMySql;

/*   TraceLog( NULL, "mysqlConnect type:%04X s2:%s s3:%s s4:%s s5:%s\n", pItem->type, hb_arrayGetCPtr( pItem, 2 ), hb_arrayGetCPtr( pItem, 3 ), hb_arrayGetCPtr( pItem, 4 ), hb_arrayGetCPtr( pItem, 5 ) ); */
   
   pMySql = mysql_init( NULL );
   if ( ! mysql_real_connect( pMySql, hb_arrayGetCPtr( pItem, 2 ), hb_arrayGetCPtr( pItem, 3 ), hb_arrayGetCPtr( pItem, 4 ), 
                              hb_arrayGetCPtr( pItem, 5 ), 0 /* port */, NULL /* pipe */, 0 /* flags*/ ) )  
   {
      mysql_close( pMySql );
      return FAILURE;
   }
   pConnection->hConnection = (void*) pMySql;
   return SUCCESS;
}


static ERRCODE mysqlDisconnect( SQLDDCONNECTION* pConnection )
{
   mysql_close( ( MYSQL * ) pConnection->hConnection );
   return SUCCESS;
}


static ERRCODE mysqlExecute( SQLDDCONNECTION* pConnection, PHB_ITEM pItem )
{
   MYSQL_RES*   pResult;

   if ( mysql_real_query( (MYSQL*) pConnection->hConnection, hb_itemGetCPtr( pItem ), hb_itemGetCLen( pItem ) ) ) 
   {
      const char*    szError;

      pConnection->iError = (int) mysql_errno( (MYSQL*) pConnection->hConnection );
      szError = mysql_error( (MYSQL*) pConnection->hConnection );
      if ( pConnection->szError )
         pConnection->szError = ( char * ) hb_xrealloc( pConnection->szError, strlen( szError ) + 1 );
      else
         pConnection->szError = ( char * ) hb_xgrab( strlen( szError ) + 1 );
      hb_strncpy( pConnection->szError, szError, strlen( szError ) );
      return FAILURE;
   }

   pResult = mysql_store_result( (MYSQL*) pConnection->hConnection );
   if ( pResult )
   {
      pConnection->ulAffectedRows = (ULONG) mysql_num_rows( pResult );
      mysql_free_result( pResult );
   }
   else
   {
      if ( mysql_field_count( (MYSQL*) pConnection->hConnection ) == 0 )
      {
         pConnection->ulAffectedRows = (ULONG) mysql_affected_rows( (MYSQL*) pConnection->hConnection );
         if ( mysql_insert_id( (MYSQL*) pConnection->hConnection ) != 0 )
            hb_itemPutNInt( pConnection->pNewID, mysql_insert_id( (MYSQL*) pConnection->hConnection ) ); 
      }
      else /* error */
      {
         const char*    szError;
     
         pConnection->iError = (int) mysql_errno( (MYSQL*) pConnection->hConnection );
         szError = mysql_error( (MYSQL*) pConnection->hConnection );
         if ( pConnection->szError )
            pConnection->szError = ( char * ) hb_xrealloc( pConnection->szError, strlen( szError ) + 1 );
         else
            pConnection->szError = ( char * ) hb_xgrab( strlen( szError ) + 1 );
         hb_strncpy( pConnection->szError, szError, strlen( szError ) );
         return FAILURE;
      }
   }
   return SUCCESS;
}


static ERRCODE mysqlOpen( SQLBASEAREAP pArea )
{
   PHB_ITEM      pItemEof, pItem;
   ULONG         ulIndex;
   USHORT        uiFields, uiCount, uiError = 0;
   BOOL          bError;
   BYTE*         pBuffer;
   DBFIELDINFO   pFieldInfo;
   MYSQL_FIELD*  pMyField;
   void**        pRow;
 
   if ( mysql_real_query( (MYSQL*) pArea->pConnection->hConnection, pArea->szQuery, strlen( pArea->szQuery ) ) ) 
   {
      hb_errRT_MySQLDD( EG_OPEN, ESQLDD_INVALIDQUERY, (char*) mysql_error( (MYSQL*) pArea->pConnection->hConnection ), pArea->szQuery, 
                        mysql_errno( (MYSQL*) pArea->pConnection->hConnection ) );
      return FAILURE;
   }
  
   if ( ( pArea->pResult = mysql_store_result( (MYSQL*) pArea->pConnection->hConnection ) ) == NULL ) 
   {
      hb_errRT_MySQLDD( EG_MEM, ESQLDD_INVALIDQUERY, (char*) mysql_error( (MYSQL*) pArea->pConnection->hConnection ), pArea->szQuery, 
                        mysql_errno( (MYSQL*) pArea->pConnection->hConnection ) );
      return FAILURE;
   }

   /* TODO: In what cases pArea->uiFieldCount can be equal to zero? Do we need to assign pItemEof in that case? */
   if ( ! pArea->uiFieldCount ) 
   { 
      uiFields = mysql_num_fields( (MYSQL_RES*) pArea->pResult );
      SELF_SETFIELDEXTENT( (AREAP) pArea, uiFields );
    
      pItemEof = hb_itemArrayNew( uiFields );

      pBuffer = (BYTE*) hb_xgrab( 256 );
    
      bError = FALSE;
      for ( uiCount = 0; uiCount < uiFields; uiCount++  )
      {
         pMyField = mysql_fetch_field_direct( (MYSQL_RES*) pArea->pResult, uiCount );
        
         hb_strncpy( ( char * ) pBuffer, pMyField->name, 256 - 1 );
         pFieldInfo.atomName = pBuffer;
         pFieldInfo.atomName[ MAX_FIELD_NAME ] = '\0';
         hb_strUpper( (char *) pFieldInfo.atomName, MAX_FIELD_NAME + 1 );
        
         pFieldInfo.uiLen = pMyField->length;
         pFieldInfo.uiDec = 0;
        
         switch( pMyField->type )
         {
            case MYSQL_TYPE_TINY:
            case MYSQL_TYPE_SHORT:
              pFieldInfo.uiType = HB_FT_INTEGER;
              break;

            case MYSQL_TYPE_LONG:
            case MYSQL_TYPE_LONGLONG:
            case MYSQL_TYPE_INT24:
              pFieldInfo.uiType = HB_FT_LONG;
              break;
          
            case MYSQL_TYPE_DECIMAL:
            case MYSQL_TYPE_NEWDECIMAL:
            case MYSQL_TYPE_FLOAT:
            case MYSQL_TYPE_DOUBLE:
              pFieldInfo.uiType = HB_FT_DOUBLE;
              pFieldInfo.uiDec = pMyField->decimals;
              break;
          
            case MYSQL_TYPE_STRING:
            case MYSQL_TYPE_VAR_STRING:
            case MYSQL_TYPE_ENUM:
            case MYSQL_TYPE_DATETIME:
              pFieldInfo.uiType = HB_FT_STRING;
              break;
          
            case MYSQL_TYPE_DATE:
              pFieldInfo.uiType = HB_FT_DATE;
              break;
          
            case MYSQL_TYPE_TINY_BLOB:
            case MYSQL_TYPE_MEDIUM_BLOB:
            case MYSQL_TYPE_LONG_BLOB:
            case MYSQL_TYPE_BLOB:
              pFieldInfo.uiType = HB_FT_MEMO;
              break;
          
/*
            case MYSQL_TYPE_TIMESTAMP:
            case MYSQL_TYPE_TIME:
            case MYSQL_TYPE_DATETIME:
            case MYSQL_TYPE_YEAR:
            case MYSQL_TYPE_NEWDATE:
            case MYSQL_TYPE_ENUM:
            case MYSQL_TYPE_SET:
*/

            default:
              bError = TRUE;
              uiError = (USHORT) pMyField->type;
              pFieldInfo.uiType = 0;
              break;
         }
        
         if ( ! bError )
         {
            switch ( pFieldInfo.uiType )
            {
               case HB_FT_STRING:
               {
                  char*    pStr;

                  pStr = (char*) hb_xgrab( pFieldInfo.uiLen + 1 );
                  memset( pStr, ' ', pFieldInfo.uiLen );
                  pStr[ pFieldInfo.uiLen ] = '\0';

                  pItem = hb_itemPutCL( NULL, pStr, pFieldInfo.uiLen );
                  hb_xfree( pStr );
                  break;
               }

               case HB_FT_MEMO:
                  pItem = hb_itemPutC( NULL, "" );
                  break;

               case HB_FT_INTEGER:
                  pItem = hb_itemPutNI( NULL, 0 );
                  break;

               case HB_FT_LONG:
                  pItem = hb_itemPutNL( NULL, 0 );
                  break;

               case HB_FT_DOUBLE:
                  pItem = hb_itemPutND( NULL, 0.0 );
                  break;

               case HB_FT_DATE:
                  pItem = hb_itemPutDS( NULL, "" );
                  break;

               default:
                  pItem = hb_itemNew( NULL );
                  bError = TRUE;
                  break;
            }

            hb_arraySetForward( pItemEof, uiCount + 1, pItem );
            hb_itemRelease( pItem );

/*            if ( pFieldInfo.uiType == HB_IT_DOUBLE || pFieldInfo.uiType == HB_IT_INTEGER )
            {
               pFieldInfo.uiType = HB_IT_LONG;
            }*/

            if ( ! bError )
               bError = ( SELF_ADDFIELD( (AREAP) pArea, &pFieldInfo ) == FAILURE );
         }
        
         if ( bError )
            break;
      }
    
      hb_xfree( pBuffer );
    
      if ( bError )
      {
        hb_itemRelease( pItemEof );
        hb_errRT_MySQLDD( EG_CORRUPTION, ESQLDD_INVALIDFIELD, "Invalid field type", pArea->szQuery, uiError );
        return FAILURE;
      }
   }
 
   pArea->ulRecCount = (ULONG) mysql_num_rows( (MYSQL_RES*) pArea->pResult );

   pArea->pRow = (void**) hb_xgrab( ( pArea->ulRecCount + 1 ) * sizeof( void* ) );
   pArea->pRowFlags = (BYTE*) hb_xgrab( ( pArea->ulRecCount + 1 ) * sizeof( BYTE ) );
   memset( pArea->pRowFlags, 0, ( pArea->ulRecCount + 1 ) * sizeof( BYTE ) );
   pArea->ulRecMax = pArea->ulRecCount + 1;

   pRow = pArea->pRow;

   *pRow = pItemEof;
   pArea->pRowFlags[ 0 ] = SQLDD_FLAG_CACHED;

   pRow++;
   for ( ulIndex = 1; ulIndex <= pArea->ulRecCount; ulIndex++ ) 
   {
     *pRow++ = (void*) mysql_row_tell( (MYSQL_RES*) pArea->pResult );
     mysql_fetch_row( (MYSQL_RES*) pArea->pResult );
   }
   pArea->fFetched = 1;

   return SUCCESS;
}


static ERRCODE mysqlClose( SQLBASEAREAP pArea )
{
   if ( pArea->pResult )
      mysql_free_result( (MYSQL_RES*) pArea->pResult );
   return SUCCESS;
}


static ERRCODE mysqlGoTo( SQLBASEAREAP pArea, ULONG ulRecNo )
{
   if ( ulRecNo <= 0 || ulRecNo > pArea->ulRecCount )
   {
      pArea->pRecord = pArea->pRow[ 0 ];
      pArea->bRecordFlags = pArea->pRowFlags[ 0 ];

      pArea->fPositioned = FALSE;
   }
   else
   {
      pArea->pRecord = pArea->pRow[ ulRecNo ];
      pArea->bRecordFlags = pArea->pRowFlags[ ulRecNo ];

      if ( ! ( pArea->bRecordFlags & SQLDD_FLAG_CACHED ) )
      {
         mysql_row_seek( ( MYSQL_RES * ) pArea->pResult, ( MYSQL_ROW_OFFSET ) pArea->pRecord );
         pArea->pNatRecord = ( void * ) mysql_fetch_row( ( MYSQL_RES * ) pArea->pResult );
         pArea->pNatLength = ( void * ) mysql_fetch_lengths( ( MYSQL_RES * ) pArea->pResult );
      }

      pArea->fPositioned = TRUE;
   }
   return SUCCESS;
}


static ERRCODE mysqlGetValue( SQLBASEAREAP pArea, USHORT uiIndex, PHB_ITEM pItem )
{
   LPFIELD   pField;
   char*     pValue;
   char      szBuffer[ 64 ];
   BOOL      bError;
   PHB_ITEM  pError;
   ULONG     ulLen;

   bError = FALSE;
   uiIndex--;
   pField = pArea->lpFields + uiIndex;

   pValue = ( (MYSQL_ROW) ( pArea->pNatRecord ) ) [ uiIndex ];
   ulLen = ( (unsigned long*) ( pArea->pNatLength ) ) [ uiIndex ];
   
   /* NULL => NIL (?) */
   if ( ! pValue )
   {
      hb_itemClear( pItem );
      return SUCCESS;
   }

   switch( pField->uiType )
   {
      case HB_FT_STRING: 
      {
#if 0        
         char*  pStr;

         /* Do NOT trim strings */
         pStr = (char*) hb_xgrab( pField->uiLen + 1 );
         if ( pValue )  
            memcpy( pStr, pValue, ulLen );
        
         if ( (ULONG)pField->uiLen > ulLen )  
            memset( pStr + ulLen, ' ', pField->uiLen - ulLen );
        
         pStr[ pField->uiLen ] = '\0';
         hb_itemPutCRaw( pItem, pStr, pField->uiLen );
#else
         /* Trim strings */
         if ( pValue )  
            hb_itemPutCL( pItem, pValue, ulLen );
         else
            hb_itemPutCL( pItem, "", 0 );
#endif
         break;
      }

      case HB_FT_MEMO:
         if ( pValue )  
            hb_itemPutCL( pItem, pValue, ulLen );
         else
            hb_itemPutCL( pItem, "", 0 );
       
         hb_itemSetCMemo( pItem );
         break;

      case HB_FT_INTEGER:
      case HB_FT_LONG:
      case HB_FT_DOUBLE:
         if ( pValue ) 
         {
            hb_strncpy( szBuffer, pValue, sizeof( szBuffer ) - 1 );
        
            if ( pField->uiDec )
            {
               hb_itemPutNDLen( pItem, atof( szBuffer ),
                                (int) pField->uiLen - ( (int) pField->uiDec + 1 ),
                                (int) pField->uiDec );
            }
            else
                  hb_itemPutNLLen( pItem, atol( szBuffer ), (int) pField->uiLen );
         } 
         else 
         {
            if ( pField->uiDec )
               hb_itemPutNDLen( pItem, 0.0,
                                (int) pField->uiLen - ( (int) pField->uiDec + 1 ),
                                (int) pField->uiDec );
            else 
                  hb_itemPutNLLen( pItem, 0, (int) pField->uiLen );
         }
         break;
        
      case HB_FT_DATE: 
      {
         char  szDate[ 9 ];
        
         szDate[ 0 ] = pValue[ 0 ];
         szDate[ 1 ] = pValue[ 1 ];
         szDate[ 2 ] = pValue[ 2 ];
         szDate[ 3 ] = pValue[ 3 ];
         szDate[ 4 ] = pValue[ 5 ];
         szDate[ 5 ] = pValue[ 6 ];
         szDate[ 6 ] = pValue[ 8 ];
         szDate[ 7 ] = pValue[ 9 ];
         szDate[ 8 ] = '\0';
         hb_itemPutDS( pItem, szDate );
         break;
      }

      default:
         bError = TRUE;
         break;
   }

   if ( bError )
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


static ERRCODE mysqlGetVarLen( SQLBASEAREAP pArea, USHORT uiIndex, ULONG * pLength )
{
   HB_SYMBOL_UNUSED( pArea );
   HB_SYMBOL_UNUSED( uiIndex );
   HB_SYMBOL_UNUSED( pLength );
   return SUCCESS;
}
