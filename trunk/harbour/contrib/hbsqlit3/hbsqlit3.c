/*
 * $Id$
 */

/*
 * Harbour Project source code:
 * SQLite3 library low level (client api) interface code
 *
 * Copyright 2007-2009 P.Chornyj <myorg63@mail.ru>
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
 * See COPYING for licensing terms.
 *
 */

#include "sqlite3.h"

#include "hbvm.h"
#include "hbapi.h"
#include "hbapiitm.h"
#include "hbapierr.h"
#include "hbapifs.h"
#include "hbstack.h"

/* TOFIX: verify the exact SQLITE3 version */
#if SQLITE_VERSION_NUMBER <= 3004001
#define sqlite3_int64   LONGLONG
#define sqlite3_uint64  ULONGLONG
#endif

#define HB_SQLITE3_DB                        6000001

#define HB_ERR_MEMSTRU_NOT_MEM_BLOCK         4001
#define HB_ERR_MEMSTRU_WRONG_MEMSTRU_BLOCK   4002
#define HB_ERR_MEMSTRU_DESTROYED             4003

#ifdef SQLITE3_LIB
extern char *sqlite3_temp_directory;
#endif /* SQLITE3_LIB */
PHB_ITEM    hb_sqlite3_itemPut( PHB_ITEM pItem, void *pMemAddr, int iType );
void        *hb_sqlite3_itemGet( PHB_ITEM pItem, int iType, BOOL fError );
void        hb_sqlite3_ret( void *pMemAddr, int iType );
void        *hb_sqlite3_param( int iParam, int iType, BOOL fError );
BOOL        hb_sqlite3_store( void *pMemAddr, int iType, int iParam );

static int  callback( void *, int, char **, char ** );
static int  authorizer( void *, int, const char *, const char *, const char *, const char * );
static int  busy_handler( void *, int );
static int  progress_handler( void * );
static int  hook_commit( void * );
static void hook_rollback( void * );

static int  callback( void *, int, char **, char ** );
static int  hook_commit( void * );
static void hook_rollback( void * );
static int  authorizer( void *, int, const char *, const char *, const char *, const char * );

typedef struct
{
   sqlite3  *db;
   PHB_DYNS cbAuthorizer;
   PHB_DYNS cbBusyHandler;
   PHB_DYNS cbProgressHandler;
   PHB_DYNS cbHookCommit;
   PHB_DYNS cbHookRollback;
} HB_SQLITE3, *PHB_SQLITE3;

typedef struct
{
   int         type;
   HB_SQLITE3  *hbsqlite3;
} HB_SQLITE3_HOLDER, *PHB_SQLITE3_HOLDER;

typedef sqlite3_stmt *psqlite3_stmt;

/**
   destructor, it's executed automatically
*/

static HB_GARBAGE_FUNC( hb_sqlite3_destructor )
{
   PHB_SQLITE3_HOLDER   pStructHolder = ( PHB_SQLITE3_HOLDER ) Cargo;

   if( pStructHolder->hbsqlite3 )
   {
      if( pStructHolder->hbsqlite3->db )
      {
         sqlite3_close( pStructHolder->hbsqlite3->db );
         pStructHolder->hbsqlite3->db = NULL;
      }

      if( pStructHolder->hbsqlite3->cbAuthorizer )
      {
         pStructHolder->hbsqlite3->cbAuthorizer = NULL;
      }

      if( pStructHolder->hbsqlite3->cbBusyHandler )
      {
         pStructHolder->hbsqlite3->cbBusyHandler = NULL;
      }

      if( pStructHolder->hbsqlite3->cbProgressHandler )
      {
         pStructHolder->hbsqlite3->cbProgressHandler = NULL;
      }

      if( pStructHolder->hbsqlite3->cbHookCommit )
      {
         pStructHolder->hbsqlite3->cbHookCommit = NULL;
      }

      if( pStructHolder->hbsqlite3->cbHookRollback )
      {
         pStructHolder->hbsqlite3->cbHookRollback = NULL;
      }

      hb_xfree( pStructHolder->hbsqlite3 );
      pStructHolder->hbsqlite3 = NULL;
   }
}

PHB_ITEM hb_sqlite3_itemPut( PHB_ITEM pItem, void *pMemAddr, int iType )
{
   PHB_SQLITE3_HOLDER   pStructHolder;

   if( pItem )
   {
      if( HB_IS_COMPLEX(pItem) )
      {
         hb_itemClear( pItem );
      }
   }
   else
   {
      pItem = hb_itemNew( pItem );
   }

   pStructHolder = ( PHB_SQLITE3_HOLDER ) hb_gcAlloc( sizeof(HB_SQLITE3_HOLDER), hb_sqlite3_destructor );
   pStructHolder->hbsqlite3 = ( HB_SQLITE3 * ) pMemAddr;
   pStructHolder->type = iType;

   return hb_itemPutPtrGC( pItem, pStructHolder );
}

void *hb_sqlite3_itemGet( PHB_ITEM pItem, int iType, BOOL fError )
{
   PHB_SQLITE3_HOLDER   pStructHolder = ( PHB_SQLITE3_HOLDER ) hb_itemGetPtrGC( pItem, hb_sqlite3_destructor );
   int                  iError = 0;

   HB_SYMBOL_UNUSED( iError );

   if( !pStructHolder )
   {
      iError = HB_ERR_MEMSTRU_NOT_MEM_BLOCK;
   }
   else if( pStructHolder->type != iType )
   {
      iError = HB_ERR_MEMSTRU_WRONG_MEMSTRU_BLOCK;
   }
   else if( !pStructHolder->hbsqlite3 )
   {
      iError = HB_ERR_MEMSTRU_DESTROYED;
   }
   else
   {
      return pStructHolder->hbsqlite3;
   }

   if( fError )
   {
      hb_errRT_BASE_SubstR( EG_ARG, iError, NULL, HB_ERR_FUNCNAME, HB_ERR_ARGS_BASEPARAMS );
   }

   return NULL;
}

void hb_sqlite3_ret( void *pMemAddr, int iType )
{
   hb_sqlite3_itemPut( hb_stackReturnItem(), pMemAddr, iType );
}

void *hb_sqlite3_param( int iParam, int iType, BOOL fError )
{
   return hb_sqlite3_itemGet( hb_param(iParam, HB_IT_POINTER), iType, fError );
}

BOOL hb_sqlite3_store( void *pMemAddr, int iType, int iParam )
{
   PHB_ITEM pItem = hb_param( iParam, HB_IT_BYREF );
   if( !pItem )
   {
      return FALSE;
   }

   hb_sqlite3_itemPut( pItem, pMemAddr, iType );
   return TRUE;
}

/**
   Callbacs helpers:
      Compile-Time Authorization Callback
      A Callback To Handle SQLITE_BUSY Errors
      Query Progress Callbacks
      Commit And Rollback Notification Callback
*/

static int callback( void *Cargo, int argc, char **argv, char **azColName )
{
   PHB_DYNS pSym = ( PHB_DYNS ) Cargo;

   if( pSym )
   {
      PHB_ITEM pArrayValue = hb_itemArrayNew( argc );
      PHB_ITEM pArrayColName = hb_itemArrayNew( argc );
      int      iRes, i;

      for( i = 0; i < argc; i++ )
      {
         hb_arraySetC( pArrayValue, i + 1, argv[i] ? argv[i] : "NULL" );
         hb_arraySetC( pArrayColName, i + 1, azColName[i] );
      }

      hb_vmPushDynSym( pSym );
      hb_vmPushNil();
      hb_vmPushInteger( argc );
      hb_vmPush( pArrayValue );
      hb_vmPush( pArrayColName );
      hb_vmDo( 3 );

      iRes = hb_parni( -1 );

      hb_itemRelease( pArrayValue );
      hb_itemRelease( pArrayColName );

      return iRes;
   }

   return 0;
}

static int authorizer( void *Cargo, int iAction, const char *sName1, const char *sName2, const char *sName3, const char *sName4 )
{
   PHB_DYNS pSym = ( PHB_DYNS ) Cargo;

   if( pSym )
   {
      int      iRes;
      PHB_ITEM pItem1 = hb_itemPutCConst( NULL, sName1 );
      PHB_ITEM pItem2 = hb_itemPutCConst( NULL, sName2 );
      PHB_ITEM pItem3 = hb_itemPutCConst( NULL, sName3 );
      PHB_ITEM pItem4 = hb_itemPutCConst( NULL, sName4 );

      hb_vmPushDynSym( pSym );
      hb_vmPushNil();
      hb_vmPushInteger( iAction );
      hb_vmPush( pItem1 );
      hb_vmPush( pItem2 );
      hb_vmPush( pItem3 );
      hb_vmPush( pItem4 );

      hb_vmDo( 5 );
      iRes = hb_parni( -1 );

      hb_itemRelease( pItem1 );
      hb_itemRelease( pItem2 );
      hb_itemRelease( pItem3 );
      hb_itemRelease( pItem4 );

      return iRes;
   }

   return 0;
}

static int busy_handler( void *Cargo, int iNumberOfTimes )
{
   PHB_DYNS pSym = ( PHB_DYNS ) Cargo;

   if( pSym )
   {
      int   iRes;

      hb_vmPushDynSym( pSym );
      hb_vmPushNil();
      hb_vmPushInteger( iNumberOfTimes );
      hb_vmDo( 1 );

      iRes = hb_parni( -1 );

      return iRes;
   }

   return 0;
}

static int progress_handler( void *Cargo )
{
   PHB_DYNS pSym = ( PHB_DYNS ) Cargo;

   if( pSym )
   {
      int   iRes;

      hb_vmPushDynSym( pSym );
      hb_vmPushNil();
      hb_vmDo( 0 );

      iRes = hb_parni( -1 );

      return iRes;
   }

   return 0;
}

static int hook_commit( void *Cargo )
{
   PHB_DYNS pSym = ( PHB_DYNS ) Cargo;

   if( pSym )
   {
      int   iRes;

      hb_vmPushDynSym( pSym );
      hb_vmPushNil();
      hb_vmDo( 0 );

      iRes = hb_parni( -1 );

      return iRes;
   }

   return 0;
}

static void hook_rollback( void *Cargo )
{
   PHB_DYNS pSym = ( PHB_DYNS ) Cargo;

   if( pSym )
   {
      hb_vmPushDynSym( pSym );
      hb_vmPushNil();
      hb_vmDo( 0 );
   }
}

/**
   sqlite3_libversion()         -> cVersion
   sqlite3_libversion_number()  -> nVersionNumber

   Returns values equivalent to the header constants
   SQLITE_VERSION and SQLITE_VERSION_NUMBER.
*/

HB_FUNC( SQLITE3_LIBVERSION )
{
   hb_retc( sqlite3_libversion() );
}

HB_FUNC( SQLITE3_LIBVERSION_NUMBER )
{
   hb_retni( sqlite3_libversion_number() );
}

/**
   sqlite3_initialize() -> nResult
   sqlite3_shutdown()   -> nResult

   The sqlite3_initialize() routine initializes the SQLite library.
   The sqlite3_shutdown() routine deallocates any resources that were
   allocated by sqlite3_initialize()
*/

HB_FUNC( SQLITE3_INITIALIZE )
{
#if SQLITE_VERSION_NUMBER >= 3006000
   hb_retni( sqlite3_initialize() );
#else
   hb_retni( -1 );
#endif /* SQLITE_VERSION_NUMBER >= 3006000 */
}

HB_FUNC( SQLITE3_SHUTDOWN )
{
#if SQLITE_VERSION_NUMBER >= 3006000
   hb_retni( sqlite3_shutdown() );
#else
   hb_retni( -1 );
#endif /* SQLITE_VERSION_NUMBER >= 3006000 */
}

/**
   Enable Or Disable Extended Result Codes

   sqlite3_extended_result_codes( db, lOnOff) -> nResultCode
*/

HB_FUNC( SQLITE3_EXTENDED_RESULT_CODES )
{
   HB_SQLITE3  *pHbSqlite3 = ( HB_SQLITE3 * ) hb_sqlite3_param( 1, HB_SQLITE3_DB, TRUE );

   if( pHbSqlite3 && pHbSqlite3->db )
   {
      hb_retni( sqlite3_extended_result_codes(pHbSqlite3->db, hb_parl(2)) );
   }
   else
   {
      hb_errRT_BASE_SubstR( EG_ARG, 0, NULL, HB_ERR_FUNCNAME, 1, hb_paramError(1) );
   }
}

/**
   Error Codes And Messages

   sqlite3_errcode( db ) -> returns the numeric result code or extended result
                            code
   sqlite3_errmsg( db )  -> return English-language text
                            that describes the error
*/

HB_FUNC( SQLITE3_ERRCODE )
{
   HB_SQLITE3  *pHbSqlite3 = ( HB_SQLITE3 * ) hb_sqlite3_param( 1, HB_SQLITE3_DB, TRUE );

   if( pHbSqlite3 && pHbSqlite3->db )
   {
      hb_retni( sqlite3_errcode(pHbSqlite3->db) );
   }
   else
   {
      hb_errRT_BASE_SubstR( EG_ARG, 0, NULL, HB_ERR_FUNCNAME, 1, hb_paramError(1) );
   }
}

HB_FUNC( SQLITE3_EXTENDED_ERRCODE )
{
#if SQLITE_VERSION_NUMBER >= 3006005
   HB_SQLITE3  *pHbSqlite3 = ( HB_SQLITE3 * ) hb_sqlite3_param( 1, HB_SQLITE3_DB, TRUE );

   if( pHbSqlite3 && pHbSqlite3->db )
   {
      hb_retni( sqlite3_extended_errcode(pHbSqlite3->db) );
   }
   else
   {
      hb_errRT_BASE_SubstR( EG_ARG, 0, NULL, HB_ERR_FUNCNAME, 1, hb_paramError(1) );
   }
#else
   hb_retni( -1 );
#endif /* SQLITE_VERSION_NUMBER >= 3006005 */
}

HB_FUNC( SQLITE3_ERRMSG )
{
   HB_SQLITE3  *pHbSqlite3 = ( HB_SQLITE3 * ) hb_sqlite3_param( 1, HB_SQLITE3_DB, TRUE );

   if( pHbSqlite3 && pHbSqlite3->db )
   {
      hb_retc( sqlite3_errmsg(pHbSqlite3->db) );
   }
   else
   {
      hb_errRT_BASE_SubstR( EG_ARG, 0, NULL, HB_ERR_FUNCNAME, 1, hb_paramError(1) );
   }
}

/**
   Suspend Execution For A Short Time

   sqlite3_sleep( ms )
*/

HB_FUNC( SQLITE3_SLEEP )
{
   hb_retni( sqlite3_sleep(hb_parni(1)) );
}

/**
   Last Insert Rowid

   sqlite3_last_insert_rowid( db ) -> nROWID
*/

HB_FUNC( SQLITE3_LAST_INSERT_ROWID )
{
   HB_SQLITE3  *pHbSqlite3 = ( HB_SQLITE3 * ) hb_sqlite3_param( 1, HB_SQLITE3_DB, TRUE );

   if( pHbSqlite3 && pHbSqlite3->db )
   {
      hb_retnint( sqlite3_last_insert_rowid(pHbSqlite3->db) );
   }
   else
   {
      hb_errRT_BASE_SubstR( EG_ARG, 0, NULL, HB_ERR_FUNCNAME, 1, hb_paramError(1) );
   }
}

/**
   Name Of The Folder Holding Temporary Files

   sqlite3_temp_directory( cDirName ) -> lResult (TRUE/FALSE)
*/

HB_FUNC( SQLITE3_TEMP_DIRECTORY )
{
   BOOL  bResult = FALSE;

   #ifdef SQLITE3_LIB
   {
      BOOL  fFree;
      BYTE  *pszDirName = hb_fsNameConv( hb_parcx(1), &fFree );

      if( hb_fsIsDirectory(pszDirName) )
      {
         bResult = TRUE;
      }
      else
      {
         if( hb_parl(2) )  /* create temp directory if not exist */
         {
            if( hb_fsMkDir(pszDirName) )
            {
               bResult = TRUE;
            }
            else
            {
               HB_TRACE( HB_TR_DEBUG, ("sqlite_temp_directory(): Can't create directory %s", pszDirName) );
            }
         }
         else
         {
            HB_TRACE( HB_TR_DEBUG, ("sqlite_temp_directory(): Directory doesn't exist %s", pszDirName) );
         }
      }

      if( bResult )
      {
         sqlite3_temp_directory = hb_strdup( ( const char * ) pszDirName );
      }

      if( fFree )
      {
         hb_xfree( ( void * ) pszDirName );
      }
   }
   #endif /* SQLITE3_LIB */
   hb_retl( bResult );
}

/**
   Opening( creating ) A New Database Connection

   sqlite3_open( cDatabace, lCreateIfNotExist ) -> return pointer to Db
                                                   or NIL if error occurs
   sqlite3_open_v2( cDatabace, nOpenMode )      -> return pHbSqlite3 or NIL
*/

HB_FUNC( SQLITE3_OPEN )
{
   sqlite3  *db;
   char     *pszFree;
   const char *pszdbName = hb_fsNameConv( hb_parcx(1), &pszFree );

   if( hb_fsFileExists(( const char * ) pszdbName) || hb_parl(2) )
   {
      if( sqlite3_open(pszdbName, &db) == SQLITE_OK )
      {
         HB_SQLITE3  *hbsqlite3;

         hbsqlite3 = ( HB_SQLITE3 * ) hb_xgrab( sizeof(HB_SQLITE3) );
         hb_xmemset( hbsqlite3, 0, sizeof(HB_SQLITE3) );
         hbsqlite3->db = db;
         hb_sqlite3_ret( hbsqlite3, HB_SQLITE3_DB );
      }
      else
      {
         sqlite3_close( db );

         hb_retptr( NULL );
      }
   }
   else
   {
      HB_TRACE( HB_TR_DEBUG, ("sqlite3_open(): Database doesn't exist %s", pszdbName) );

      hb_retptr( NULL );
   }

   if( pszFree )
      hb_xfree( pszFree );
}

HB_FUNC( SQLITE3_OPEN_V2 )
{
#if SQLITE_VERSION_NUMBER >= 3005000
   sqlite3  *db;
   char     *pszFree;
   const char *pszdbName = hb_fsNameConv( hb_parcx(1), &pszFree );

   if( sqlite3_open_v2(pszdbName, &db, hb_parni(2), NULL) == SQLITE_OK )
   {
      HB_SQLITE3  *hbsqlite3;

      hbsqlite3 = ( HB_SQLITE3 * ) hb_xgrab( sizeof(HB_SQLITE3) );
      hb_xmemset( hbsqlite3, 0, sizeof(HB_SQLITE3) );
      hbsqlite3->db = db;
      hb_sqlite3_ret( hbsqlite3, HB_SQLITE3_DB );
   }
   else
   {
      sqlite3_close( db );

      hb_retptr( NULL );
   }

   if( pszFree )
      hb_xfree( pszFree );
#else
   hb_retptr( NULL );
#endif /* SQLITE_VERSION_NUMBER >= 3005000 */
}

/**
   One-Step Query Execution Interface

   sqlite3_exec( db, cSQLTEXT, [pCallbackFunc]|[cCallbackFunc] ) -> nResultCode
*/

HB_FUNC( SQLITE3_EXEC )
{
   HB_SQLITE3  *pHbSqlite3 = ( HB_SQLITE3 * ) hb_sqlite3_param( 1, HB_SQLITE3_DB, TRUE );

   if( pHbSqlite3 && pHbSqlite3->db )
   {
      char     *pszErrMsg = NULL;
      PHB_DYNS pDynSym;
      int      rc;

      if( HB_ISCHAR(3) || HB_ISSYMBOL(3) )
      {
         if( HB_ISCHAR(3) )
         {
            pDynSym = hb_dynsymFindName( hb_parc(3) );
         }
         else
         {
            pDynSym = hb_dynsymNew( hb_itemGetSymbol(hb_param(3, HB_IT_SYMBOL)) );
         }

         if( pDynSym && hb_dynsymIsFunction(pDynSym) )
         {
            rc = sqlite3_exec( pHbSqlite3->db, hb_parc(2), callback, ( void * ) pDynSym, &pszErrMsg );
         }
         else
         {
            rc = sqlite3_exec( pHbSqlite3->db, hb_parc(2), NULL, 0, &pszErrMsg );
         }
      }
      else
      {
         rc = sqlite3_exec( pHbSqlite3->db, hb_parc(2), NULL, 0, &pszErrMsg );
      }

      if( rc != SQLITE_OK )
      {
         HB_TRACE( HB_TR_DEBUG, ("sqlite3_exec(): Returned error: %s", pszErrMsg) );
         sqlite3_free( pszErrMsg );
      }

      hb_retni( rc );
   }
   else
   {
      hb_errRT_BASE_SubstR( EG_ARG, 0, NULL, HB_ERR_FUNCNAME, 1, hb_paramError(1) );
   }
}

/**
   Compiling An SQL Statement

   sqlite3_prepare( db, cSQLTEXT )
   -> return pointer to compiled statement or NIL if error occurs

   TODO: pszTail?
*/

HB_FUNC( SQLITE3_PREPARE )
{
   HB_SQLITE3  *pHbSqlite3 = ( HB_SQLITE3 * ) hb_sqlite3_param( 1, HB_SQLITE3_DB, TRUE );

   if( pHbSqlite3 && pHbSqlite3->db )
   {
      PHB_ITEM SQL = hb_param( 2, HB_IT_STRING );

      if( SQL )
      {
         const char     *pSQL = hb_itemGetCPtr( SQL );
         ULONG          ulLen = hb_itemGetCLen( SQL );
         psqlite3_stmt  pStmt;
         const char     *pszTail;

         if( sqlite3_prepare_v2(pHbSqlite3->db, pSQL, ulLen, &pStmt, &pszTail) == SQLITE_OK )
         {
            hb_retptr( pStmt );
         }
         else
         {
            sqlite3_finalize( pStmt );

            hb_retptr( NULL );
         }
      }
      else
      {
         hb_errRT_BASE_SubstR( EG_ARG, 0, NULL, HB_ERR_FUNCNAME, 1, hb_paramError(2) );
      }
   }
   else
   {
      hb_errRT_BASE_SubstR( EG_ARG, 0, NULL, HB_ERR_FUNCNAME, 1, hb_paramError(1) );
   }
}

/**
   Determine If An SQL Statement Is Complete

   sqlite3_complete( sqlText ) -> lResult
*/

HB_FUNC( SQLITE3_COMPLETE )
{
   hb_retl( sqlite3_complete(hb_parc(1)) );
}

/**
   This interface can be used to retrieve a saved copy of the original SQL text
   used to create a prepared statement
   if that statement was compiled using either sqlite3_prepare()

   sqlite3_sql( pStmt ) -> cSQLTEXT
*/

HB_FUNC( SQLITE3_SQL )
{
/* TOFIX: verify the exact SQLITE3 version */
#if SQLITE_VERSION_NUMBER > 3004001
   psqlite3_stmt  pStmt = ( psqlite3_stmt ) hb_parptr( 1 );

   if( pStmt )
   {
      hb_retc( sqlite3_sql(pStmt) );
   }
   else
   {
      hb_errRT_BASE_SubstR( EG_ARG, 0, NULL, HB_ERR_FUNCNAME, 1, hb_paramError(1) );
   }
#else
   hb_retc( NULL );
#endif
}

/**
   Prepared Statement Status.

   sqlite3_stmt_status( pStmt, nOp, lResetFlag) -> nStatus
*/

HB_FUNC( SQLITE3_STMT_STATUS )
{
#if SQLITE_VERSION_NUMBER >= 3006004
   psqlite3_stmt  pStmt = ( psqlite3_stmt ) hb_parptr( 1 );

   if( pStmt )
   {
      hb_retni( sqlite3_stmt_status(pStmt, hb_parni(2), ( int ) hb_parl(3)) );
   }
   else
   {
      hb_errRT_BASE_SubstR( EG_ARG, 0, NULL, HB_ERR_FUNCNAME, 1, hb_paramError(1) );
   }
#else
   hb_retni( -1 );
#endif /* SQLITE_VERSION_NUMBER >= 3006004 */
}

/**
   Find The Database Handle Associated With A Prepared Statement

   sqlite3_db_handle( pStmt ) -> pHbSqlite3
*/

/*
HB_FUNC( SQLITE3_DB_HANDLE )
{
   psqlite3_stmt  pStmt = ( psqlite3_stmt ) hb_parptr( 1 );

   if( pStmt )
   {
      ;
   }
   else
   {
      hb_errRT_BASE_SubstR( EG_ARG, 0, NULL, HB_ERR_FUNCNAME, 1, hb_paramError(1) );
   }
}
*/

/**
   Evaluate An Prepared SQL Statement

   sqlite3_step( pStmt ) -> nResultCode
*/

HB_FUNC( SQLITE3_STEP )
{
   psqlite3_stmt  pStmt = ( psqlite3_stmt ) hb_parptr( 1 );

   if( pStmt )
   {
      hb_retni( sqlite3_step(pStmt) );
   }
   else
   {
      hb_errRT_BASE_SubstR( EG_ARG, 0, NULL, HB_ERR_FUNCNAME, 1, hb_paramError(1) );
   }
}

/**
   Reset All Bindings On A Prepared Statement

   sqlite3_clear_bindings( pStmt ) -> nResultCode

   Contrary to the intuition of many,
   sqlite3_reset() does not reset the bindings on a prepared statement.
   Use this routine to reset all host parameters to NULL.
*/

HB_FUNC( SQLITE3_CLEAR_BINDINGS )
{
   psqlite3_stmt  pStmt = ( psqlite3_stmt ) hb_parptr( 1 );

   if( pStmt )
   {
      hb_retni( sqlite3_clear_bindings(pStmt) );
   }
   else
   {
      hb_errRT_BASE_SubstR( EG_ARG, 0, NULL, HB_ERR_FUNCNAME, 1, hb_paramError(1) );
   }
}

/**
   Reset A Prepared Statement Object

   sqlite3_reset( pStmt ) -> nResultCode
*/

HB_FUNC( SQLITE3_RESET )
{
   psqlite3_stmt  pStmt = ( psqlite3_stmt ) hb_parptr( 1 );

   if( pStmt )
   {
      hb_retni( sqlite3_reset(pStmt) );
   }
   else
   {
      hb_errRT_BASE_SubstR( EG_ARG, 0, NULL, HB_ERR_FUNCNAME, 1, hb_paramError(1) );
   }
}

/**
   Finalize A Prepared Statement Object

   sqlite3_finalize( pStmt ) -> nResultCode
*/

HB_FUNC( SQLITE3_FINALIZE )
{
   psqlite3_stmt  pStmt = ( psqlite3_stmt ) hb_parptr( 1 );

   if( pStmt )
   {
      hb_retni( sqlite3_finalize(pStmt) );
   }
   else
   {
      hb_errRT_BASE_SubstR( EG_ARG, 0, NULL, HB_ERR_FUNCNAME, 1, hb_paramError(1) );
   }
}

/*
int sqlite3_bind_blob(sqlite3_stmt*, int, const void*, int n, void(*)(void*));
int sqlite3_bind_double(sqlite3_stmt*, int, double);
int sqlite3_bind_int(sqlite3_stmt*, int, int);
int sqlite3_bind_int64(sqlite3_stmt*, int, sqlite3_int64);
int sqlite3_bind_null(sqlite3_stmt*, int);
int sqlite3_bind_text(sqlite3_stmt*, int, const char*, int n, void(*)(void*));
int sqlite3_bind_value(sqlite3_stmt*, int, const sqlite3_value*);
int sqlite3_bind_zeroblob(sqlite3_stmt*, int, int n)
*/

/**
   Binding Values To Prepared Statements

   These routines return SQLITE_OK on success or an error code if anything
   goes wrong.
   SQLITE_RANGE is returned if the parameter index is out of range.
   SQLITE_NOMEM is returned if malloc fails.
   SQLITE_MISUSE is returned if these routines are called on a virtual machine
   that is the wrong state or which has already been finalized.
*/

HB_FUNC( SQLITE3_BIND_BLOB )
{
   psqlite3_stmt  pStmt = ( psqlite3_stmt ) hb_parptr( 1 );

   if( pStmt )
   {
      hb_retni( sqlite3_bind_blob(pStmt, hb_parni(2), hb_parcx(3), hb_parcsiz(3) - 1, SQLITE_TRANSIENT) );
   }
   else
   {
      hb_errRT_BASE_SubstR( EG_ARG, 0, NULL, HB_ERR_FUNCNAME, 1, hb_paramError(1) );
   }
}

HB_FUNC( SQLITE3_BIND_DOUBLE )
{
   psqlite3_stmt  pStmt = ( psqlite3_stmt ) hb_parptr( 1 );

   if( pStmt )
   {
      hb_retni( sqlite3_bind_double(pStmt, hb_parni(2), hb_parnd(3)) );
   }
   else
   {
      hb_errRT_BASE_SubstR( EG_ARG, 0, NULL, HB_ERR_FUNCNAME, 1, hb_paramError(1) );
   }
}

HB_FUNC( SQLITE3_BIND_INT )
{
   psqlite3_stmt  pStmt = ( psqlite3_stmt ) hb_parptr( 1 );

   if( pStmt )
   {
      hb_retni( sqlite3_bind_int(pStmt, hb_parni(2), hb_parni(3)) );
   }
   else
   {
      hb_errRT_BASE_SubstR( EG_ARG, 0, NULL, HB_ERR_FUNCNAME, 1, hb_paramError(1) );
   }
}

HB_FUNC( SQLITE3_BIND_INT64 )
{
   psqlite3_stmt  pStmt = ( psqlite3_stmt ) hb_parptr( 1 );
   sqlite3_int64  int64 = hb_parnint( 3 );

   if( pStmt )
   {
      hb_retni( sqlite3_bind_int64(pStmt, hb_parni(2), int64) );
   }
   else
   {
      hb_errRT_BASE_SubstR( EG_ARG, 0, NULL, HB_ERR_FUNCNAME, 1, hb_paramError(1) );
   }
}

HB_FUNC( SQLITE3_BIND_NULL )
{
   psqlite3_stmt  pStmt = ( psqlite3_stmt ) hb_parptr( 1 );

   if( pStmt )
   {
      hb_retni( sqlite3_bind_null(pStmt, hb_parni(2)) );
   }
   else
   {
      hb_errRT_BASE_SubstR( EG_ARG, 0, NULL, HB_ERR_FUNCNAME, 1, hb_paramError(1) );
   }
}

HB_FUNC( SQLITE3_BIND_TEXT )
{
   psqlite3_stmt  pStmt = ( psqlite3_stmt ) hb_parptr( 1 );

   if( pStmt )
   {
      hb_retni( sqlite3_bind_text(pStmt, hb_parni(2), hb_parc(3), hb_parclen(3), SQLITE_TRANSIENT) );
   }
   else
   {
      hb_errRT_BASE_SubstR( EG_ARG, 0, NULL, HB_ERR_FUNCNAME, 1, hb_paramError(1) );
   }
}

HB_FUNC( SQLITE3_BIND_ZEROBLOB )
{
   psqlite3_stmt  pStmt = ( psqlite3_stmt ) hb_parptr( 1 );

   if( pStmt )
   {
      hb_retni( sqlite3_bind_zeroblob(pStmt, hb_parni(2), hb_parni(3)) );
   }
   else
   {
      hb_errRT_BASE_SubstR( EG_ARG, 0, NULL, HB_ERR_FUNCNAME, 1, hb_paramError(1) );
   }
}

/**
   Number Of Host Parameters

   sqlite3_bind_parameter_count( pStmt ) -> nResult
*/

HB_FUNC( SQLITE3_BIND_PARAMETER_COUNT )
{
   psqlite3_stmt  pStmt = ( psqlite3_stmt ) hb_parptr( 1 );

   if( pStmt )
   {
      hb_retni( sqlite3_bind_parameter_count(pStmt) );
   }
   else
   {
      hb_errRT_BASE_SubstR( EG_ARG, 0, NULL, HB_ERR_FUNCNAME, 1, hb_paramError(1) );
   }
}

/**
   Index Of A Parameter With A Given Name

   sqlite3_bind_parameter_index( pStmt, cParameterName ) -> nResult
*/

HB_FUNC( SQLITE3_BIND_PARAMETER_INDEX )
{
   psqlite3_stmt  pStmt = ( psqlite3_stmt ) hb_parptr( 1 );

   if( pStmt )
   {
      hb_retni( sqlite3_bind_parameter_index(pStmt, hb_parc(2)) );
   }
   else
   {
      hb_errRT_BASE_SubstR( EG_ARG, 0, NULL, HB_ERR_FUNCNAME, 1, hb_paramError(1) );
   }
}

/**
   Name Of A Host Parameter

   sqlite3_bind_parameter_name( pStmt, nParameterIndex ) -> cParameterName
*/

HB_FUNC( SQLITE3_BIND_PARAMETER_NAME )
{
   psqlite3_stmt  pStmt = ( psqlite3_stmt ) hb_parptr( 1 );

   if( pStmt )
   {
      hb_retc( sqlite3_bind_parameter_name(pStmt, hb_parni(2)) );
   }
   else
   {
      hb_errRT_BASE_SubstR( EG_ARG, 0, NULL, HB_ERR_FUNCNAME, 1, hb_paramError(1) );
   }
}

/**
   Count The Number Of Rows Modified

   sqlite3_changes( db ) -> nRowCount
*/

HB_FUNC( SQLITE3_CHANGES )
{
   HB_SQLITE3  *pHbSqlite3 = ( HB_SQLITE3 * ) hb_sqlite3_param( 1, HB_SQLITE3_DB, TRUE );

   if( pHbSqlite3 && pHbSqlite3->db )
   {
      hb_retni( sqlite3_changes(pHbSqlite3->db) );
   }
   else
   {
      hb_errRT_BASE_SubstR( EG_ARG, 0, NULL, HB_ERR_FUNCNAME, 1, hb_paramError(1) );
   }
}

/**
   Total Number Of Rows Modified

   sqlite3_total_changes( db ) -> nRowCount
*/

HB_FUNC( SQLITE3_TOTAL_CHANGES )
{
   HB_SQLITE3  *pHbSqlite3 = ( HB_SQLITE3 * ) hb_sqlite3_param( 1, HB_SQLITE3_DB, TRUE );

   if( pHbSqlite3 && pHbSqlite3->db )
   {
      hb_retni( sqlite3_total_changes(pHbSqlite3->db) );
   }
   else
   {
      hb_errRT_BASE_SubstR( EG_ARG, 0, NULL, HB_ERR_FUNCNAME, 1, hb_paramError(1) );
   }
}

/**
   Number Of Columns In A Result Set

   sqlite3_column_count( pStmt ) -> nColumnCount
*/

HB_FUNC( SQLITE3_COLUMN_COUNT )
{
   psqlite3_stmt  pStmt = ( psqlite3_stmt ) hb_parptr( 1 );

   if( pStmt )
   {
      hb_retni( sqlite3_column_count(pStmt) );
   }
   else
   {
      hb_errRT_BASE_SubstR( EG_ARG, 0, NULL, HB_ERR_FUNCNAME, 1, hb_paramError(1) );
   }
}

/**
   sqlite3_column_type( pStmt, nIndex ) -> nColumnType
   nColumnType is Datatype code for the initial data type of the result column

   SQLITE_INTEGER      1
   SQLITE_FLOAT        2
   SQLITE_TEXT         3
   SQLITE3_TEXT        3
   SQLITE_BLOB         4
   SQLITE_NULL         5

   Declared Datatype Of A Query Result (see doc)
   sqlite3_column_decltype( pStmt, nIndex ) -> nColumnDeclType
*/

HB_FUNC( SQLITE3_COLUMN_TYPE )
{
   psqlite3_stmt  pStmt = ( psqlite3_stmt ) hb_parptr( 1 );

   if( pStmt )
   {
      hb_retni( sqlite3_column_type(pStmt, hb_parni(2) - 1) );
   }
   else
   {
      hb_errRT_BASE_SubstR( EG_ARG, 0, NULL, HB_ERR_FUNCNAME, 1, hb_paramError(1) );
   }
}

HB_FUNC( SQLITE3_COLUMN_DECLTYPE )
{
   psqlite3_stmt  pStmt = ( psqlite3_stmt ) hb_parptr( 1 );

   if( pStmt )
   {
      hb_retc( sqlite3_column_decltype(pStmt, hb_parni(2) - 1) );
   }
   else
   {
      hb_errRT_BASE_SubstR( EG_ARG, 0, NULL, HB_ERR_FUNCNAME, 1, hb_paramError(1) );
   }
}

/**
   Column Names In A Result Set

   sqlite3_column_name( pStmt, columnIndex) -> columnName
*/

HB_FUNC( SQLITE3_COLUMN_NAME )
{
   psqlite3_stmt  pStmt = ( psqlite3_stmt ) hb_parptr( 1 );

   if( pStmt )
   {
      hb_retc( sqlite3_column_name(pStmt, hb_parni(2) - 1) );
   }
   else
   {
      hb_errRT_BASE_SubstR( EG_ARG, 0, NULL, HB_ERR_FUNCNAME, 1, hb_paramError(1) );
   }
}

/**
   sqlite3_column_bytes( pStmt, columnIndex )
   -> returns the number of bytes in that BLOB or string

   Results Values From A Query

   sqlite3_column_blob( pStmt, columnIndex )   -> value as BLOB
   sqlite3_column_double( pStmt, columnIndex ) -> value as double
   sqlite3_column_int( pStmt, columnIndex )    -> value as integer
   sqlite3_column_int64( pStmt, columnIndex )  -> value as long long
   sqlite3_column_text( pStmt, columnIndex )   -> value as text
*/

HB_FUNC( SQLITE3_COLUMN_BYTES )
{
   psqlite3_stmt  pStmt = ( psqlite3_stmt ) hb_parptr( 1 );

   if( pStmt )
   {
      hb_retni( sqlite3_column_bytes(pStmt, hb_parni(2) - 1) );
   }
   else
   {
      hb_errRT_BASE_SubstR( EG_ARG, 0, NULL, HB_ERR_FUNCNAME, 1, hb_paramError(1) );
   }
}

HB_FUNC( SQLITE3_COLUMN_BLOB )
{
   psqlite3_stmt  pStmt = ( psqlite3_stmt ) hb_parptr( 1 );

   if( pStmt )
   {
      int   index = hb_parni( 2 ) - 1;

      hb_retclen( ( const char * ) sqlite3_column_blob(pStmt, index), sqlite3_column_bytes(pStmt, index) );
   }
   else
   {
      hb_errRT_BASE_SubstR( EG_ARG, 0, NULL, HB_ERR_FUNCNAME, 1, hb_paramError(1) );
   }
}

HB_FUNC( SQLITE3_COLUMN_DOUBLE )
{
   psqlite3_stmt  pStmt = ( psqlite3_stmt ) hb_parptr( 1 );

   if( pStmt )
   {
      hb_retnd( sqlite3_column_double(pStmt, hb_parni(2) - 1) );
   }
   else
   {
      hb_errRT_BASE_SubstR( EG_ARG, 0, NULL, HB_ERR_FUNCNAME, 1, hb_paramError(1) );
   }
}

HB_FUNC( SQLITE3_COLUMN_INT )
{
   psqlite3_stmt  pStmt = ( psqlite3_stmt ) hb_parptr( 1 );

   if( pStmt )
   {
      hb_retni( sqlite3_column_int(pStmt, hb_parni(2) - 1) );
   }
   else
   {
      hb_errRT_BASE_SubstR( EG_ARG, 0, NULL, HB_ERR_FUNCNAME, 1, hb_paramError(1) );
   }
}

HB_FUNC( SQLITE3_COLUMN_INT64 )
{
   psqlite3_stmt  pStmt = ( psqlite3_stmt ) hb_parptr( 1 );

   if( pStmt )
   {
      hb_retnint( sqlite3_column_int64(pStmt, hb_parni(2) - 1) );
   }
   else
   {
      hb_errRT_BASE_SubstR( EG_ARG, 0, NULL, HB_ERR_FUNCNAME, 1, hb_paramError(1) );
   }
}

HB_FUNC( SQLITE3_COLUMN_TEXT )
{
   psqlite3_stmt  pStmt = ( psqlite3_stmt ) hb_parptr( 1 );

   if( pStmt )
   {
      int   index = hb_parni( 2 ) - 1;
      hb_retclen( ( char * ) sqlite3_column_text(pStmt, index), sqlite3_column_bytes(pStmt, index) );
   }
   else
   {
      hb_errRT_BASE_SubstR( EG_ARG, 0, NULL, HB_ERR_FUNCNAME, 1, hb_paramError(1) );
   }
}

/**
   Enable Or Disable Extension Loading

   sqlite3_enable_load_extension( db, lOnOff ) -> prev.state
*/

HB_FUNC( SQLITE3_ENABLE_LOAD_EXTENSION )
{
   HB_SQLITE3  *pHbSqlite3 = ( HB_SQLITE3 * ) hb_sqlite3_param( 1, HB_SQLITE3_DB, TRUE );

   if( pHbSqlite3 && pHbSqlite3->db )
   {
      hb_retni( sqlite3_enable_load_extension(pHbSqlite3->db, hb_parl(2)) );
   }
   else
   {
      hb_errRT_BASE_SubstR( EG_ARG, 0, NULL, HB_ERR_FUNCNAME, 1, hb_paramError(1) );
   }
}

/*
   Reset Automatic Extension Loading

   sqlite3_reset_auto_extension()
*/

HB_FUNC( SQLITE3_RESET_AUTO_EXTENSION )
{
   sqlite3_reset_auto_extension();
}

/**
   Set A Busy Timeout

   sqlite3_busy_timeout( db, ms )
*/

HB_FUNC( SQLITE3_BUSY_TIMEOUT )
{
   HB_SQLITE3  *pHbSqlite3 = ( HB_SQLITE3 * ) hb_sqlite3_param( 1, HB_SQLITE3_DB, TRUE );

   if( pHbSqlite3 && pHbSqlite3->db )
   {
      hb_retni( sqlite3_busy_timeout(pHbSqlite3->db, hb_parni(2)) );
   }
   else
   {
      hb_errRT_BASE_SubstR( EG_ARG, 0, NULL, HB_ERR_FUNCNAME, 1, hb_paramError(1) );
   }
}

/**
   Convenience Routines For Running Queries

   sqlite3_get_table( db, sqlText ) -> aResult
*/

HB_FUNC( SQLITE3_GET_TABLE )
{
   HB_SQLITE3  *pHbSqlite3 = ( HB_SQLITE3 * ) hb_sqlite3_param( 1, HB_SQLITE3_DB, TRUE );

   if( pHbSqlite3 && pHbSqlite3->db )
   {
      PHB_ITEM pResultList = hb_itemArrayNew( 0 );
      int      iRow, iCol;
      char     *pszErrMsg = NULL;
      char     **pResult;

      if( sqlite3_get_table(pHbSqlite3->db, hb_parc(2), &pResult, &iRow, &iCol, &pszErrMsg) == SQLITE_OK )
      {
         int   i, j, k = 0;

         for( i = 0; i < iRow + 1; i++ )
         {
            PHB_ITEM pArray = hb_itemArrayNew( iCol );

            for( j = 1; j <= iCol; j++, k++ )
            {
               hb_arraySetC( pArray, j, pResult[k] );
            }

            hb_arrayAddForward( pResultList, pArray );
            hb_itemRelease( pArray );
         }
      }
      else
      {
         HB_TRACE( HB_TR_DEBUG, ("sqlite3_get_table(): Returned error: %s", pszErrMsg) );
         sqlite3_free( pszErrMsg );
      }

      sqlite3_free_table( pResult );

      hb_itemReturnRelease( pResultList );
   }
   else
   {
      hb_errRT_BASE_SubstR( EG_ARG, 0, NULL, HB_ERR_FUNCNAME, 1, hb_paramError(1) );
   }
}

/**
   Extract Metadata About A Column Of A Table
   based on
   int sqlite3_table_column_metadata(
     sqlite3 *db,                - IN:  Connection handle
     const char *zDbName,        - IN:  Database name or NULL
     const char *zTableName,     - IN:  Table name
     const char *zColumnName,    - IN:  Column name
     char const **pzDataType,    - OUT: Declared data type
     char const **pzCollSeq,     - OUT: Collation sequence name
     int *pNotNull,              - OUT: True if NOT NULL constraint exists
     int *pPrimaryKey,           - OUT: True if column part of PK
     int *pAutoinc               - OUT: True if column is auto-increment
   );
*/

#ifdef SQLITE_ENABLE_COLUMN_METADATA
HB_FUNC( SQLITE3_TABLE_COLUMN_METADATA )
{
   HB_SQLITE3  *pHbSqlite3 = ( HB_SQLITE3 * ) hb_sqlite3_param( 1, HB_SQLITE3_DB, TRUE );

   if( pHbSqlite3 && pHbSqlite3->db )
   {
      char const  *pzDataType = NULL;
      char const  *pzCollSeq = NULL;
      int         iNotNull = 0;
      int         iPrimaryKey = 0;
      int         iAutoinc = 0;

      if
      (
         sqlite3_table_column_metadata
            (
               pHbSqlite3->db,
               hb_parc(2) /* zDbName */,
               hb_parc(3) /* zTableName */,
               hb_parc(4) /* zColumnName */,
               &pzDataType /* pzDataDtype */,
               &pzCollSeq /* pzCollSeq */,
               &iNotNull,
               &iPrimaryKey,
               &iAutoinc
            ) == SQLITE_OK
      )
      {
         PHB_ITEM pArray = hb_itemArrayNew( 5 );

         hb_arraySetC( pArray, 1, ( char * ) pzDataType );
         hb_arraySetC( pArray, 2, ( char * ) pzCollSeq );
         hb_arraySetL( pArray, 3, (BOOL) iNotNull );
         hb_arraySetL( pArray, 4, (BOOL) iPrimaryKey );
         hb_arraySetL( pArray, 5, (BOOL) iAutoinc );

         hb_itemReturnRelease( pArray );
      }
   }
   else
   {
      hb_errRT_BASE_SubstR( EG_ARG, 0, NULL, HB_ERR_FUNCNAME, 1, hb_paramError(1) );
   }
}

/**
   Source Of Data In A Query Result

   sqlite3_column_database_name( pStmt, ColumnIndex) -> cDatabaseName
   sqlite3_column_table_name( pStmt, ColumnIndex)    -> cTableName
   sqlite3_column_origin_name( pStmt, ColumnIndex)   -> cColumnName
*/

HB_FUNC( SQLITE3_COLUMN_DATABASE_NAME )
{
   psqlite3_stmt  pStmt = ( psqlite3_stmt ) hb_parptr( 1 );

   if( pStmt )
   {
      hb_retc( sqlite3_column_database_name(pStmt, hb_parni(2) - 1) );
   }
   else
   {
      hb_errRT_BASE_SubstR( EG_ARG, 0, NULL, HB_ERR_FUNCNAME, 1, hb_paramError(1) );
   }
}

HB_FUNC( SQLITE3_COLUMN_TABLE_NAME )
{
   psqlite3_stmt  pStmt = ( psqlite3_stmt ) hb_parptr( 1 );

   if( pStmt )
   {
      hb_retc( sqlite3_column_table_name(pStmt, hb_parni(2) - 1) );
   }
   else
   {
      hb_errRT_BASE_SubstR( EG_ARG, 0, NULL, HB_ERR_FUNCNAME, 1, hb_paramError(1) );
   }
}

HB_FUNC( SQLITE3_COLUMN_ORIGIN_NAME )
{
   psqlite3_stmt  pStmt = ( psqlite3_stmt ) hb_parptr( 1 );

   if( pStmt )
   {
      hb_retc( sqlite3_column_origin_name(pStmt, hb_parni(2) - 1) );
   }
   else
   {
      hb_errRT_BASE_SubstR( EG_ARG, 0, NULL, HB_ERR_FUNCNAME, 1, hb_paramError(1) );
   }
}
#endif /* SQLITE_ENABLE_COLUMN_METADATA */

/*
   BLOB I/O
*/

/**
   Open A BLOB For Incremental I/O

   Open a handle to the blob located in row iRow, column zColumn, table zTable
   in database zDb. i.e. the same blob that would be selected by:

   SELECT zColumn FROM zDb.zTable WHERE rowid = iRow;
*/

HB_FUNC( SQLITE3_BLOB_OPEN )
{
   HB_SQLITE3  *pHbSqlite3 = ( HB_SQLITE3 * ) hb_sqlite3_param( 1, HB_SQLITE3_DB, TRUE );

   if( pHbSqlite3 && pHbSqlite3->db )
   {
      sqlite3_blob   *ppBlob = NULL;

      if
      (
         sqlite3_blob_open
            (
               pHbSqlite3->db,
               hb_parc(2) /* zDb */,
               hb_parc(3) /* zTable */,
               hb_parc(4) /* zColumn */,
               (sqlite3_int64) hb_parnint(5) /* iRow */,
               hb_parni(6) /* flags */,
               &ppBlob
            ) == SQLITE_OK
      )
      {
         hb_retptr( ppBlob );
      }
      else
      {
         hb_retptr( NULL );
      }
   }
   else
   {
      hb_retptr( NULL );
   }
}

/**
   Close A BLOB Handle
*/

HB_FUNC( SQLITE3_BLOB_CLOSE )
{
   sqlite3_blob   *pBlob = ( sqlite3_blob * ) hb_parptr( 1 );

   if( pBlob )
   {
      hb_retni( sqlite3_blob_close(pBlob) );
   }
   else
   {
      hb_errRT_BASE_SubstR( EG_ARG, 0, NULL, HB_ERR_FUNCNAME, 1, hb_paramError(1) );
   }
}

/**
   Return The Size Of An Open BLOB
*/

HB_FUNC( SQLITE3_BLOB_BYTES )
{
   sqlite3_blob   *pBlob = ( sqlite3_blob * ) hb_parptr( 1 );

   if( pBlob )
   {
      hb_retni( sqlite3_blob_bytes(pBlob) );
   }
   else
   {
      hb_errRT_BASE_SubstR( EG_ARG, 0, NULL, HB_ERR_FUNCNAME, 1, hb_paramError(1) );
   }
}

/**
   Read Data From A BLOB Incrementally
*/

HB_FUNC( SQLITE3_BLOB_READ )
{
   sqlite3_blob   *pBlob = ( sqlite3_blob * ) hb_parptr( 1 );

   if( pBlob )
   {
      int   iLen = hb_parni( 2 );
      BYTE  *buffer;

      if( iLen == 0 )
      {
         iLen = sqlite3_blob_bytes( pBlob );
      }

      buffer = ( BYTE * ) hb_xgrab( iLen + 1 );

      /*hb_xmemset( buffer, 0, iLen );*/

      if( SQLITE_OK == sqlite3_blob_read(pBlob, buffer, iLen, hb_parni(3)) )
      {
         buffer[iLen] = '\0';
         hb_retclen_buffer( ( char * ) buffer, iLen );
      }
      else
      {
         hb_xfree( buffer );
      }
   }
   else
   {
      hb_errRT_BASE_SubstR( EG_ARG, 0, NULL, HB_ERR_FUNCNAME, 1, hb_paramError(1) );
   }
}

/**
   Write Data Into A BLOB Incrementally
*/

HB_FUNC( SQLITE3_BLOB_WRITE )
{
   sqlite3_blob   *pBlob = ( sqlite3_blob * ) hb_parptr( 1 );

   if( pBlob )
   {
      int   iLen = hb_parni( 3 );

      if( iLen == 0 )
      {
         iLen = hb_parcsiz( 2 ) - 1;
      }

      hb_retni( sqlite3_blob_write(pBlob, hb_parcx(2), iLen, hb_parni(4)) );
   }
   else
   {
      hb_errRT_BASE_SubstR( EG_ARG, 0, NULL, HB_ERR_FUNCNAME, 1, hb_paramError(1) );
   }
}

/**
    Test To See If The Database Is In Auto-Commit Mode

    sqlite3_get_autocommit( db ) -> lResult (TRUE/FALSE)
*/

HB_FUNC( SQLITE3_GET_AUTOCOMMIT )
{
   HB_SQLITE3  *pHbSqlite3 = ( HB_SQLITE3 * ) hb_sqlite3_param( 1, HB_SQLITE3_DB, TRUE );

   if( pHbSqlite3 && pHbSqlite3->db )
   {
      hb_retl( sqlite3_get_autocommit(pHbSqlite3->db) );
   }
   else
   {
      hb_errRT_BASE_SubstR( EG_ARG, 0, NULL, HB_ERR_FUNCNAME, 1, hb_paramError(1) );
   }
}

/**
   Enable Or Disable Shared Pager Cache

   sqlite3_enable_shared_cache( lOnOff ) -> nResultCode
*/

HB_FUNC( SQLITE3_ENABLE_SHARED_CACHE )
{
   hb_retni( sqlite3_enable_shared_cache(hb_parl(1)) );
}

/**
   Tracing And Profiling Functions

   sqlite3_trace( db, lOnOff )
   sqlite3_profile( db, lOnOff )
*/

static void SQL3ProfileLog( void *sFile, const char *sProfileMsg, sqlite3_uint64 int64 )
{
   if( sProfileMsg )
   {
      FILE  *hFile = fopen( sFile ? ( const char * ) sFile : "hbsq3_pr.log", "a" );

      if( hFile )
      {
         fprintf( hFile, "%s - %"PFLL "d\n", sProfileMsg, int64 );
         fclose( hFile );
      }
   }
}

static void SQL3TraceLog( void *sFile, const char *sTraceMsg )
{
   if( sTraceMsg )
   {
      FILE  *hFile = fopen( sFile ? ( const char * ) sFile : "hbsq3_tr.log", "a" );

      if( hFile )
      {
         fprintf( hFile, "%s \n", sTraceMsg );
         fclose( hFile );
      }
   }
}

HB_FUNC( SQLITE3_PROFILE )
{
   HB_SQLITE3  *pHbSqlite3 = ( HB_SQLITE3 * ) hb_sqlite3_param( 1, HB_SQLITE3_DB, TRUE );

   if( pHbSqlite3 && pHbSqlite3->db )
   {
      sqlite3_profile( pHbSqlite3->db, hb_parl(2) ? SQL3ProfileLog : NULL, ( void * ) (HB_ISCHAR(3) ? hb_parcx(3) : NULL) );
   }
   else
   {
      hb_errRT_BASE_SubstR( EG_ARG, 0, NULL, HB_ERR_FUNCNAME, 1, hb_paramError(1) );
   }
}

HB_FUNC( SQLITE3_TRACE )
{
   HB_SQLITE3  *pHbSqlite3 = ( HB_SQLITE3 * ) hb_sqlite3_param( 1, HB_SQLITE3_DB, TRUE );

   if( pHbSqlite3 && pHbSqlite3->db )
   {
      sqlite3_trace( pHbSqlite3->db, hb_parl(2) ? SQL3TraceLog : NULL, ( void * ) (HB_ISCHAR(3) ? hb_parcx(3) : NULL) );
   }
   else
   {
      hb_errRT_BASE_SubstR( EG_ARG, 0, NULL, HB_ERR_FUNCNAME, 1, hb_paramError(1) );
   }
}

/**
   BLOB Import/export
*/

HB_FUNC( SQLITE3_FILE_TO_BUFF )
{
   int   handle = hb_fsOpen( hb_parcx(1), FO_READ );

   if( handle != FS_ERROR )
   {
      BYTE  *buffer;
      ULONG iSize;

      iSize = ( int ) hb_fsSeek( handle, 0, FS_END );
      iSize -= ( int ) hb_fsSeek( handle, 0, FS_SET );
      buffer = ( BYTE * ) hb_xgrab( iSize + 1 );
      iSize = hb_fsReadLarge( handle, buffer, iSize );
      buffer[iSize] = '\0';
      hb_fsClose( handle );

      hb_retclen_buffer( ( char * ) buffer, iSize );
   }
   else
   {
      hb_retc_null();
   }
}

HB_FUNC( SQLITE3_BUFF_TO_FILE )
{
   int   handle = hb_fsCreate( hb_parcx(1), FC_NORMAL );
   ULONG iSize = hb_parcsiz( 2 ) - 1;

   if( handle != FS_ERROR && iSize > 0 )
   {
      hb_retni( hb_fsWriteLarge(handle, ( BYTE * ) hb_parcx(2), iSize) == iSize ? 0 : -1 );
      hb_fsClose( handle );
   }
   else
   {
      hb_retni( 1 );
   }
}

/**
   Causes any pending database operation to abort and return at its
   earliest opportunity.

   sqlite3_interrupt( db) -> Nil
*/

HB_FUNC( SQLITE3_INTERRUPT )
{
   HB_SQLITE3  *pHbSqlite3 = ( HB_SQLITE3 * ) hb_sqlite3_param( 1, HB_SQLITE3_DB, TRUE );

   if( pHbSqlite3 && pHbSqlite3->db )
   {
      sqlite3_interrupt( pHbSqlite3->db );
   }
}

/**
   A Callback To Handle SQLITE_BUSY Errors

   sqlite3_busy_handler( db, nNumOfOpCodes, [Nil]|[cFunc]|[sFunc] )
*/

HB_FUNC( SQLITE3_BUSY_HANDLER )
{
   HB_SQLITE3  *pHbSqlite3 = ( HB_SQLITE3 * ) hb_sqlite3_param( 1, HB_SQLITE3_DB, TRUE );

   if( pHbSqlite3 && pHbSqlite3->db )
   {
      if( (hb_pcount() > 2) && (HB_ISCHAR(2) || HB_ISSYMBOL(2)) )
      {
         PHB_DYNS pDynSym;

         if( HB_ISCHAR(2) )
         {
            pDynSym = hb_dynsymFindName( hb_parc(2) );
         }
         else
         {
            pDynSym = hb_dynsymNew( hb_itemGetSymbol(hb_param(2, HB_IT_SYMBOL)) );
         }

         if( pDynSym && hb_dynsymIsFunction(pDynSym) )
         {
            if( pHbSqlite3->cbBusyHandler )
            {
               pHbSqlite3->cbBusyHandler = NULL;
            }

            pHbSqlite3->cbBusyHandler = pDynSym;

            sqlite3_busy_handler( pHbSqlite3->db, busy_handler, ( void * ) pHbSqlite3->cbBusyHandler );
         }

         /*
         else
         {
            if (pHbSqlite3->cbBusyHandler)
            {
               pHbSqlite3->cbBusyHandler = NULL;
            }

            sqlite3_busy_handler( pHbSqlite3->db, NULL, NULL );
         }
*/

      }
      else
      {
         if( pHbSqlite3->cbBusyHandler )
         {
            pHbSqlite3->cbBusyHandler = NULL;
         }

         sqlite3_busy_handler( pHbSqlite3->db, NULL, NULL );
      }
   }
}

/**
   Query Progress Callbacks

   sqlite3_progress_handler( db, nNumOfOpCodes, [Nil]|[cFunc]|[sFunc] )
*/

HB_FUNC( SQLITE3_PROGRESS_HANDLER )
{
   HB_SQLITE3  *pHbSqlite3 = ( HB_SQLITE3 * ) hb_sqlite3_param( 1, HB_SQLITE3_DB, TRUE );

   if( pHbSqlite3 && pHbSqlite3->db )
   {
      if( (hb_pcount() > 2) && HB_ISNUM(2) && (HB_ISCHAR(3) || HB_ISSYMBOL(3)) )
      {
         PHB_DYNS pDynSym;

         if( HB_ISCHAR(3) )
         {
            pDynSym = hb_dynsymFindName( hb_parc(3) );
         }
         else
         {
            pDynSym = hb_dynsymNew( hb_itemGetSymbol(hb_param(3, HB_IT_SYMBOL)) );
         }

         if( pDynSym && hb_dynsymIsFunction(pDynSym) )
         {
            if( pHbSqlite3->cbProgressHandler )
            {
               pHbSqlite3->cbProgressHandler = NULL;
            }

            pHbSqlite3->cbProgressHandler = pDynSym;

            sqlite3_progress_handler( pHbSqlite3->db, hb_parni(2), progress_handler, ( void * ) pHbSqlite3->cbProgressHandler );
         }

         /*
         else
         {
            if (pHbSqlite3->cbProgressHandler)
            {
               pHbSqlite3->cbProgressHandler = NULL;
            }

            sqlite3_progress_handler( pHbSqlite3->db, 0, NULL, NULL );
         }
*/

      }
      else
      {
         if( pHbSqlite3->cbProgressHandler )
         {
            pHbSqlite3->cbProgressHandler = NULL;
         }

         sqlite3_progress_handler( pHbSqlite3->db, 0, NULL, NULL );
      }
   }
}

/**
   Commit And Rollback Notification Callbacks

   sqlite3_commit_hook( db, [Nil]|[cFunc]|[sFunc] )
   sqlite3_rollback_hook( db, [Nil]|[cFunc]|[sFunc] )
*/

HB_FUNC( SQLITE3_COMMIT_HOOK )
{
   HB_SQLITE3  *pHbSqlite3 = ( HB_SQLITE3 * ) hb_sqlite3_param( 1, HB_SQLITE3_DB, TRUE );

   if( pHbSqlite3 && pHbSqlite3->db )
   {
      if( (hb_pcount() > 1) && (HB_ISCHAR(2) || HB_ISSYMBOL(2)) )
      {
         PHB_DYNS pDynSym;

         if( HB_ISCHAR(2) )
         {
            pDynSym = hb_dynsymFindName( hb_parc(2) );
         }
         else
         {
            pDynSym = hb_dynsymNew( hb_itemGetSymbol(hb_param(2, HB_IT_SYMBOL)) );
         }

         if( pDynSym && hb_dynsymIsFunction(pDynSym) )
         {
            if( pHbSqlite3->cbHookCommit )
            {
               pHbSqlite3->cbHookCommit = NULL;
            }

            pHbSqlite3->cbHookCommit = pDynSym;

            sqlite3_commit_hook( pHbSqlite3->db, hook_commit, ( void * ) pHbSqlite3->cbHookCommit );
         }

         /*
         else
         {
            if (pHbSqlite3->cbHookCommit)
            {
               pHbSqlite3->cbHookCommit = NULL;
            }

            sqlite3_commit_hook( pHbSqlite3->db, NULL, NULL );
         }
*/

      }
      else
      {
         if( pHbSqlite3->cbHookCommit )
         {
            pHbSqlite3->cbHookCommit = NULL;
         }

         sqlite3_commit_hook( pHbSqlite3->db, NULL, NULL );
      }
   }
}

HB_FUNC( SQLITE3_ROLLBACK_HOOK )
{
   HB_SQLITE3  *pHbSqlite3 = ( HB_SQLITE3 * ) hb_sqlite3_param( 1, HB_SQLITE3_DB, TRUE );

   if( pHbSqlite3 && pHbSqlite3->db )
   {
      if( (hb_pcount() > 1) && (HB_ISCHAR(2) || HB_ISSYMBOL(2)) )
      {
         PHB_DYNS pDynSym;

         if( HB_ISCHAR(2) )
         {
            pDynSym = hb_dynsymFindName( hb_parc(2) );
         }
         else
         {
            pDynSym = hb_dynsymNew( hb_itemGetSymbol(hb_param(2, HB_IT_SYMBOL)) );
         }

         if( pDynSym && hb_dynsymIsFunction(pDynSym) )
         {
            if( pHbSqlite3->cbHookRollback )
            {
               pHbSqlite3->cbHookRollback = NULL;
            }

            pHbSqlite3->cbHookRollback = pDynSym;

            sqlite3_rollback_hook( pHbSqlite3->db, hook_rollback, ( void * ) pHbSqlite3->cbHookRollback );
         }

         /*
         else
         {
            if (pHbSqlite3->cbHookRollback)
            {
               pHbSqlite3->cbHookRollback = NULL;
            }

            sqlite3_rollback_hook( pHbSqlite3->db, NULL, NULL );
         }
*/

      }
      else
      {
         if( pHbSqlite3->cbHookRollback )
         {
            pHbSqlite3->cbHookRollback = NULL;
         }

         sqlite3_rollback_hook( pHbSqlite3->db, NULL, NULL );
      }
   }
}

/**
   Compile-Time Authorization Callbacks

   sqlite3_set_authorizer( pDb, [Nil]|[cFunc]|[sFunc] )
*/

HB_FUNC( SQLITE3_SET_AUTHORIZER )
{
   HB_SQLITE3  *pHbSqlite3 = ( HB_SQLITE3 * ) hb_sqlite3_param( 1, HB_SQLITE3_DB, TRUE );

   if( pHbSqlite3 && pHbSqlite3->db )
   {
      if( (hb_pcount() > 1) && (HB_ISCHAR(2) || HB_ISSYMBOL(2)) )
      {
         PHB_DYNS pDynSym;

         if( HB_ISCHAR(2) )
         {
            pDynSym = hb_dynsymFindName( hb_parc(2) );
         }
         else
         {
            pDynSym = hb_dynsymNew( hb_itemGetSymbol(hb_param(2, HB_IT_SYMBOL)) );
         }

         if( pDynSym && hb_dynsymIsFunction(pDynSym) )
         {
            if( pHbSqlite3->cbAuthorizer )
            {
               pHbSqlite3->cbAuthorizer = NULL;
            }

            pHbSqlite3->cbAuthorizer = pDynSym;

            hb_retni( sqlite3_set_authorizer(pHbSqlite3->db, authorizer, ( void * ) pHbSqlite3->cbAuthorizer) );
         }

         /*
         else
         {
            if (pHbSqlite3->cbAuthorizer)
            {
               pHbSqlite3->cbAuthorizer = NULL;
            }

            hb_retni( sqlite3_set_authorizer(pHbSqlite3->db, NULL, NULL) );
         }
*/

      }
      else
      {
         if( pHbSqlite3->cbAuthorizer )
         {
            pHbSqlite3->cbAuthorizer = NULL;
         }

         hb_retni( sqlite3_set_authorizer(pHbSqlite3->db, NULL, NULL) );
      }
   }
}

/**
   This API is used to overwrite the contents of one database with that
   of another. It is useful either for creating backups of databases or
   for copying in-memory databases to or from persistent files.

   ! Experimental !

   sqlite3_backup_init( DbDest, cDestName, DbSource, cSourceName ) ->
               return pointer to Backup or NIL if error occurs

   sqlite3_backup_step( pBackup, nPage ) -> nResult
   sqlite3_backup_finish( pBackup ) -> nResult
   sqlite3_backup_remaining( pBackup ) -> nResult
   sqlite3_backup_pagecount( pBackup ) -> nResult
*/

HB_FUNC( SQLITE3_BACKUP_INIT )
{
#if SQLITE_VERSION_NUMBER >= 3006011
   HB_SQLITE3     *pHbSqlite3Dest = ( HB_SQLITE3 * ) hb_sqlite3_param( 1, HB_SQLITE3_DB, TRUE );
   HB_SQLITE3     *pHbSqlite3Source = ( HB_SQLITE3 * ) hb_sqlite3_param( 3, HB_SQLITE3_DB, TRUE );
   sqlite3_backup *pBackup;

   if( pHbSqlite3Dest && pHbSqlite3Dest->db && pHbSqlite3Source && pHbSqlite3Source->db && HB_ISCHAR(2) && HB_ISCHAR(4) )
   {
      pBackup = sqlite3_backup_init( pHbSqlite3Dest->db, hb_parcx(2), pHbSqlite3Source->db, hb_parcx(4) );

      if( pBackup )
      {
         hb_retptr( pBackup );
      }
      else
      {
         hb_retptr( NULL );
      }
   }
   else
#endif /* SQLITE_VERSION_NUMBER >= 3006011 */
   {
      hb_retptr( NULL );
   }
}

HB_FUNC( SQLITE3_BACKUP_STEP )
{
#if SQLITE_VERSION_NUMBER >= 3006011
   sqlite3_backup *pBackup = ( sqlite3_backup * ) hb_parptr( 1 );

   if( pBackup )
   {
      hb_retni( sqlite3_backup_step(pBackup, hb_parni(2)) );
   }
   else
#endif /* SQLITE_VERSION_NUMBER >= 3006011 */
   {
      hb_retni( -1 );
   }
}

HB_FUNC( SQLITE3_BACKUP_FINISH )
{
#if SQLITE_VERSION_NUMBER >= 3006011
   sqlite3_backup *pBackup = ( sqlite3_backup * ) hb_parptr( 1 );

   if( pBackup )
   {
      hb_retni( sqlite3_backup_finish(pBackup) );
   }
   else
#endif /* SQLITE_VERSION_NUMBER >= 3006011 */
   {
      hb_retni( -1 );
   }
}

HB_FUNC( SQLITE3_BACKUP_REMAINING )
{
#if SQLITE_VERSION_NUMBER >= 3006011
   sqlite3_backup *pBackup = ( sqlite3_backup * ) hb_parptr( 1 );

   if( pBackup )
   {
      hb_retni( sqlite3_backup_remaining(pBackup) );
   }
   else
#endif /* SQLITE_VERSION_NUMBER >= 3006011 */
   {
      hb_retni( -1 );
   }
}

HB_FUNC( SQLITE3_BACKUP_PAGECOUNT )
{
#if SQLITE_VERSION_NUMBER >= 3006011
   sqlite3_backup *pBackup = ( sqlite3_backup * ) hb_parptr( 1 );

   if( pBackup )
   {
      hb_retni( sqlite3_backup_pagecount(pBackup) );
   }
   else
#endif /* SQLITE_VERSION_NUMBER >= 3006011 */
   {
      hb_retni( -1 );
   }
}

/**
   Memory Allocator Statistics

   sqlite3_memory_used() -> nResult
   sqlite3_memory_highwater( lResetFlag ) -> nResult
*/

HB_FUNC( SQLITE3_MEMORY_USED )
{
/* TOFIX: verify the exact SQLITE3 version */
#if SQLITE_VERSION_NUMBER > 3004001
   hb_retnint( sqlite3_memory_used() );
#else
   hb_retnint( -1 );
#endif
}

HB_FUNC( SQLITE3_MEMORY_HIGHWATER )
{
/* TOFIX: verify the exact SQLITE3 version */
#if SQLITE_VERSION_NUMBER > 3004001
   hb_retnint( sqlite3_memory_highwater(( int ) hb_parl(1)) );
#else
   hb_retnint( -1 );
#endif
}

/**
   Test To See If The Library Is Threadsafe

   sqlite3_threadsafe() -> nResult
*/

HB_FUNC( SQLITE3_THREADSAFE )
{
/* TOFIX: verify the exact SQLITE3 version */
#if SQLITE_VERSION_NUMBER > 3004001
   hb_retni( sqlite3_threadsafe() );
#else
   hb_retni( -1 );
#endif
}

/**
   SQLite Runtime Status

   sqlite3_status( nOp, @nCurrent, @nHighwater, lResetFlag);
*/

HB_FUNC( SQLITE3_STATUS )
{
#if SQLITE_VERSION_NUMBER >= 3006000
   int   iCurrent, iHighwater;

   if( hb_pcount() > 3 && (HB_ISNUM(2) && HB_ISBYREF(2)) && (HB_ISNUM(3) && HB_ISBYREF(3)) )
   {
      hb_retni( sqlite3_status(hb_parni(1), &iCurrent, &iHighwater, ( int ) hb_parl(4)) );

      hb_storni( iCurrent, 2 );
      hb_storni( iHighwater, 3 );
   }
   else
#endif /* SQLITE_VERSION_NUMBER >= 3006000 */
   {
      hb_retni( -1 );
   }
}

/**
   Database Connection Status

   sqlite3_db_status( pDb, nOp, @nCurrent, @nHighwater, lResetFlag);
*/

HB_FUNC( SQLITE3_DB_STATUS )
{
#if SQLITE_VERSION_NUMBER >= 3006001
   int         iCurrent, iHighwater;
   HB_SQLITE3  *pHbSqlite3 = ( HB_SQLITE3 * ) hb_sqlite3_param( 1, HB_SQLITE3_DB, TRUE );

   if( pHbSqlite3 && pHbSqlite3->db && (hb_pcount() > 4) && (HB_ISNUM(3) && HB_ISBYREF(3)) && (HB_ISNUM(4) && HB_ISBYREF(4)) )
   {
      hb_retni( sqlite3_db_status(pHbSqlite3->db, hb_parni(2), &iCurrent, &iHighwater, ( int ) hb_parl(5)) );

      hb_storni( iCurrent, 3 );
      hb_storni( iHighwater, 4 );
   }
   else
#endif /* SQLITE_VERSION_NUMBER >= 3006001 */
   {
      hb_retni( -1 );
   }
}
