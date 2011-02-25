/*
 * $Id$
 */

/*
 * Harbour Project source code:
 * QT wrapper main header
 *
 * Copyright 2009 Pritpal Bedi <pritpal@vouchcac.com>
 * www - http://harbour-project.org
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
/*----------------------------------------------------------------------*/

#include "hbvmint.h"

#include "hbqt.h"

#include "hbapiitm.h"
#include "hbinit.h"
#include "hbapierr.h"
#include "hbvm.h"
#include "hbapicls.h"
#include "hbstack.h"
#include "hbvm.h"

#if QT_VERSION >= 0x040500

/*----------------------------------------------------------------------*/

HB_EXTERN_BEGIN
extern void * hbqt_gcAllocate_QObject( void * pObj, bool bNew );
HB_EXTERN_END

/*----------------------------------------------------------------------*/

static HB_GARBAGE_FUNC( Q_release )
{
   HBQT_GC_T * p = ( HBQT_GC_T * ) Cargo;

   if( p && p->ph )
      p->func( p );
}

static const HB_GC_FUNCS QT_gcFuncs =
{
   Q_release,
   hb_gcDummyMark
};

const HB_GC_FUNCS * hbqt_gcFuncs( void )
{
   return &QT_gcFuncs;
}

void * hbqt_par_obj( int iParam )
{
   PHB_ITEM pItem;

   HB_TRACE( HB_TR_DEBUG, ( "hbqt_par_obj( %d )", iParam ) );

   if( ( pItem = hb_param( iParam, HB_IT_OBJECT ) ) != NULL )
   {
      HBQT_GC_T * p;
      void * pr;

      hb_vmPushSymbol( hb_dynsymSymbol( hb_dynsymFindName( "PPTR" ) ) );
      hb_vmPush( pItem );
      hb_vmSend( 0 );

      pItem = hb_param( -1, HB_IT_POINTER );

      p = ( HBQT_GC_T * ) hb_itemGetPtrGC( pItem, hbqt_gcFuncs() );

      if( p && p->ph )
         return p->ph;
#if 1
      else if( ( pr = hb_itemGetPtr( pItem ) ) != NULL )
         return pr; /* TOFIX: Only required in QList.cpp hb_retptr() calls. Though the latter should be fixed, rather than this code. */
#endif
      else
         hb_errRT_BASE( EG_ARG, 8001, NULL, HB_ERR_FUNCNAME, HB_ERR_ARGS_BASEPARAMS );
   }
   else if( HB_ISPOINTER( iParam ) )
   {
      return hb_parptr( iParam );
   }
   else
   {
      //hb_errRT_BASE( EG_ARG, 8000, NULL, HB_ERR_FUNCNAME, HB_ERR_ARGS_BASEPARAMS );
      HBQT_GC_T * p;
      p = ( HBQT_GC_T * ) hb_parptrGC( hbqt_gcFuncs(), iParam );
      if( p && p->ph )
         return p->ph;
   }

   return NULL;
}

void * hbqt_gcpointer( int iParam )
{
#if 0
   return hbqt_par_obj( iParam );
#else
   HBQT_GC_T * p;

   HB_TRACE( HB_TR_DEBUG, ( "hbqt_gcpointer( %d )", iParam ) );

   p = ( HBQT_GC_T * ) hb_parptrGC( hbqt_gcFuncs(), iParam );

   if( p && p->ph )
   {
      return p->ph;
   }
   else if( HB_ISPOINTER( iParam ) )
   {
      HB_TRACE( HB_TR_DEBUG, ( "hbqt_gcpointer(): returns RAW pointer: %p", hb_parptr( iParam ) ) );
      return hb_parptr( iParam ); /* TOFIX: In what cases is this needed? Reference counting to avoid referring to freed pointers? */
   }
   else if( HB_ISOBJECT( iParam ) )
   {
      PHB_ITEM pObj = hb_param( iParam, HB_IT_ANY );

      hb_vmPushSymbol( hb_dynsymSymbol( hb_dynsymFindName( "PPTR" ) ) );
      hb_vmPush( pObj );
      hb_vmSend( 0 );
      return hbqt_gcpointer( -1 );
   }
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "hbqt_gcpointer(): returns NULL" ) );
      return NULL; /* TODO: Still better if RTE. */
   }
#endif
}

void * hbqt_pPtrFromObj( int iParam )
{
   PHB_ITEM pObj;

   HB_TRACE( HB_TR_DEBUG, ( "hbqt_pPtrFromObj( %d )", iParam ) );

   pObj = hb_param( iParam, HB_IT_ANY );

   if( hb_itemType( pObj ) == HB_IT_OBJECT )
   {
      hb_vmPushSymbol( hb_dynsymSymbol( hb_dynsymFindName( "PPTR" ) ) );
      hb_vmPush( pObj );
      hb_vmSend( 0 );

      return hbqt_gcpointer( -1 );
   }
   else if( hb_itemType( pObj ) == HB_IT_POINTER )
      return hbqt_gcpointer( iParam );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "hbqt_pPtrFromObj(): returns NULL" ) );
      return NULL; /* TODO: Still better if RTE. */
   }
}

int hbqt_IsObjectType( int iParam, HB_U32 iType )
{
   PHB_ITEM pItem;

   HB_TRACE( HB_TR_DEBUG, ( "hbqt_IsObjectType( %d )", iParam ) );

   if( ( pItem = hb_param( iParam, HB_IT_OBJECT ) ) != NULL )
   {
      HBQT_GC_T * p;

      hb_vmPushSymbol( hb_dynsymSymbol( hb_dynsymFindName( "PPTR" ) ) );
      hb_vmPush( pItem );
      hb_vmSend( 0 );

      pItem = hb_param( -1, HB_IT_POINTER );

      if( pItem )
      {
         p = ( HBQT_GC_T * ) hb_itemGetPtrGC( pItem, hbqt_gcFuncs() );

         if( p && p->ph )
            return p->type == iType;
      }
   }

   /* hbqt_errRT_ARG(); */ /* NOTE: Could not check type for whatever reason */

   return HB_FALSE;
}

HB_FUNC( HBQT_ISOBJECT )
{
   PHB_ITEM pParam = hb_param( 1, HB_IT_OBJECT );

   if( pParam )
   {
      hb_vmPushSymbol( hb_dynsymSymbol( hb_dynsymFindName( "PPTR" ) ) );
      hb_vmPush( pParam );
      hb_vmSend( 0 );

      hb_retl( hb_param( -1, HB_IT_POINTER ) != NULL );
   }
   else
      hb_retl( HB_FALSE );
}

HB_FUNC( __HBQT_PTR )
{
   PHB_ITEM pParam = hb_param( 1, HB_IT_ANY );

   if( hb_itemType( pParam ) & HB_IT_OBJECT )
   {
      PHB_ITEM pRetVal;

      hb_vmPushSymbol( hb_dynsymSymbol( hb_dynsymFindName( "PPTR" ) ) );
      hb_vmPush( pParam );
      hb_vmSend( 0 );

      if( ( pRetVal = hb_param( -1, HB_IT_POINTER ) ) != NULL )
         return;
   }

   hb_itemReturn( pParam );
}

void hbqt_errRT_ARG( void )
{
   hb_errRT_BASE( EG_ARG, 9999, NULL, HB_ERR_FUNCNAME, HB_ERR_ARGS_BASEPARAMS );
}

void * hbqt_detachgcpointer( int iParam )
{
   HBQT_GC_T * p;

   HB_TRACE( HB_TR_DEBUG, ( "hbqt_detachgcpointer( %d )", iParam ) );

   p = ( HBQT_GC_T * ) hb_parptrGC( hbqt_gcFuncs(), iParam );

   if( p && p->ph )
   {
      p->bNew = false;
      return NULL;
   }
   else if( HB_ISOBJECT( iParam ) )
   {
      PHB_ITEM pObj = hb_param( iParam, HB_IT_ANY );

      hb_vmPushSymbol( hb_dynsymSymbol( hb_dynsymFindName( "PPTR" ) ) );
      hb_vmPush( pObj );
      hb_vmSend( 0 );
      return hbqt_detachgcpointer( -1 );
   }
   else
      return NULL;
}

HB_FUNC( __HBQT_ISPOINTER )
{
   HBQT_GC_T * p = ( HBQT_GC_T * ) hb_parptrGC( hbqt_gcFuncs(), 1 );

   hb_retl( p && p->ph );
}

#include <QtCore/QObject>

HB_FUNC( HBQT_FINDCHILD )
{
   QObject * object = ( QObject * ) hbqt_pPtrFromObj( 1 );
   hb_retptr( object->findChild< QObject * >( hbqt_par_QString( 2 ) ) );
}

HB_FUNC( HBQT_ISEQUALGCQTPOINTER )
{
   hb_retl( hbqt_pPtrFromObj( 1 ) == hbqt_pPtrFromObj( 2 ) );
}

HB_FUNC( __HBQT_ERROR )
{
   PHB_ITEM pError = hb_errRT_New( ES_ERROR, "HBQT", EG_ARG, hb_parnidef( 1, 1001 ), NULL, NULL, 0, EF_NONE );
   hb_errLaunch( pError );
   hb_itemRelease( pError );
}

HB_FUNC( __HBQT_SETUTF8 )
{
   PHB_CODEPAGE cdp = hb_cdpFindExt( "UTF8" );

   if( cdp )
      hb_vmSetCDP( cdp );
}

PHB_ITEM hbqt_defineClassBegin( const char* szClsName, PHB_ITEM s_oClass, const char* szParentClsStr )
{
    static PHB_DYNS s__CLSLOCKDEF = NULL;

    PHB_ITEM oClass = NULL;

    if( s__CLSLOCKDEF == NULL )
    {
        s__CLSLOCKDEF = hb_dynsymGetCase( "__CLSLOCKDEF" );
    }

    hb_vmPushDynSym( s__CLSLOCKDEF );
    hb_vmPushNil();
    hb_vmPushItemRef( s_oClass );
    hb_vmDo( 1 );

    if( hb_itemGetL( hb_stackReturnItem() ) )
    {
        static PHB_DYNS s___HBCLASS = NULL;

        if( s___HBCLASS == NULL )
        {
            s___HBCLASS = hb_dynsymGetCase( "HBCLASS" );
        }

        PHB_ITEM pSuper = hb_itemNew( NULL );
        if( szParentClsStr )
        {
           hb_arrayNew( pSuper, 0 );

           char szParentClsBuffer[ strlen( szParentClsStr ) + 1 ];
           char* szSingleClsName;

           strcpy( szParentClsBuffer, szParentClsStr );
           szParentClsBuffer[ strlen( szParentClsStr ) ] = 0;

           szSingleClsName = strtok( szParentClsBuffer, " ," );

           PHB_ITEM pItem = hb_itemNew( NULL );

           while( szSingleClsName != NULL )
           {
              hb_itemPutC( pItem, szSingleClsName );
              hb_arrayAdd( pSuper, hb_itemPutSymbol( pItem, hb_dynsymGetCase( szSingleClsName )->pSymbol ) );
              szSingleClsName = strtok( NULL, " ," );
           }
           hb_itemRelease( pItem );
        }

        hb_vmPushDynSym( s___HBCLASS );
        hb_vmPushNil();
        hb_vmDo( 0 );

        PHB_ITEM pClsName = hb_itemNew( NULL );
        hb_itemPutC( pClsName, szClsName );

        PHB_ITEM pSym_ClsFunc = hb_itemNew( NULL );
        hb_itemPutSymbol( pSym_ClsFunc, hb_dynsymGetCase( szClsName )->pSymbol );

        hb_objSendMsg( hb_stackReturnItem(), "New", 3, pClsName, pSuper, pSym_ClsFunc );

        oClass = hb_itemNew( hb_stackReturnItem() );

        hb_itemRelease( pSym_ClsFunc );
        hb_itemRelease( pSuper );
        hb_itemRelease( pClsName );

        hb_objSendMsg( oClass, "Create", 0 );
        hb_objSendMsg( oClass, "Instance", 0 );
#if 0
        PHB_ITEM pDataName = hb_itemNew( NULL );
        hb_itemPutC( pDataName, "PPTR" );
        hb_objSendMsg( oClass, "AddData", 1, pDataName );
#endif
    }

    return oClass;
}

void hbqt_defineClassEnd( PHB_ITEM s_oClass, PHB_ITEM oClass )
{
    if( s_oClass && oClass )
    {
        static PHB_DYNS s__CLSUNLOCKDEF = NULL;

        if( s__CLSUNLOCKDEF == NULL )
        {
            s__CLSUNLOCKDEF = hb_dynsymGetCase( "__CLSUNLOCKDEF" );
        }

        hb_vmPushDynSym( s__CLSUNLOCKDEF );
        hb_vmPushNil();
        hb_vmPushItemRef( s_oClass );
        hb_vmPush( oClass );
        hb_vmDo( 2 );

        hb_itemRelease( oClass );
    }
}

void * hbqt_getqtptr( void )
{
   PHB_ITEM pObj = hb_stackSelfItem();
   HBQT_GC_T * p = ( HBQT_GC_T * ) hb_arrayGetPtrGC( pObj, 1, hbqt_gcFuncs() );
   if( p && p->ph )
      return p->ph;
   else
      return NULL;
}

PHB_ITEM hbqt_create_object( void * p, const char * objectList, int iIndex )
{
   static PHB_DYNS s_createObject = NULL;

   if( s_createObject == NULL )
   {
       s_createObject = hb_dynsymGetCase( "HBQT_CREATEQTOBJECT" );
   }

   hb_vmPushDynSym( s_createObject );
   hb_vmPushNil();
   hb_vmPushString( objectList, strlen( objectList ) );
   hb_vmPushInteger( iIndex );
   hb_vmPushPointerGC( p );
   hb_vmDo( 3 );

   return hb_stackReturnItem();
}

PHB_ITEM hbqt_create_objectFromEventType( void * pEvent, int type )
{
   static PHB_DYNS s_createObject = NULL;

   if( s_createObject == NULL )
   {
       s_createObject = hb_dynsymGetCase( "HBQT_CREATEQTOBJECTBYTYPE" );
   }

   hb_vmPushDynSym( s_createObject );
   hb_vmPushNil();
   hb_vmPushInteger( type );
   //hb_vmPushPointerGC( hbqt_gcAllocate_QObject( pEvent, false ) );
   hb_vmPushPointer( pEvent );
   hb_vmDo( 2 );

   return hb_stackReturnItem();
}

/*----------------------------------------------------------------------*/

#endif                  // #if QT_VERSION >= 0x040500

