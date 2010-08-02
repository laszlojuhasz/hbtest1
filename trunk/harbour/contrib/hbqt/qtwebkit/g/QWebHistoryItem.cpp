/*
 * $Id$
 */

/* -------------------------------------------------------------------- */
/* WARNING: Automatically generated source file. DO NOT EDIT!           */
/*          Instead, edit corresponding .qth file,                      */
/*          or the generator tool itself, and run regenarate.           */
/* -------------------------------------------------------------------- */

/*
 * Harbour Project source code:
 * QT wrapper main header
 *
 * Copyright 2009-2010 Pritpal Bedi <pritpal@vouchcac.com>
 *
 * Copyright 2009 Marcos Antonio Gambeta <marcosgambeta at gmail dot com>
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

#include "hbqtcore.h"
#include "hbqtwebkit.h"

/*----------------------------------------------------------------------*/
#if QT_VERSION >= 0x040500
/*----------------------------------------------------------------------*/

#include <QtCore/QPointer>

#include "hbqtgui.h"

#include <QtWebKit/QWebHistoryItem>
#include <QtCore/QVariant>


/*
 * QWebHistoryItem ( const QWebHistoryItem & other )
 * ~QWebHistoryItem ()
 */

typedef struct
{
   QWebHistoryItem * ph;
   bool bNew;
   PHBQT_GC_FUNC func;
   int type;
} HBQT_GC_T_QWebHistoryItem;

HBQT_GC_FUNC( hbqt_gcRelease_QWebHistoryItem )
{
   HBQT_GC_T * p = ( HBQT_GC_T * ) Cargo;

   if( p && p->bNew )
   {
      if( p->ph )
      {
         HB_TRACE( HB_TR_DEBUG, ( "ph=%p    _rel_QWebHistoryItem   /.\\", p->ph ) );
         delete ( ( QWebHistoryItem * ) p->ph );
         HB_TRACE( HB_TR_DEBUG, ( "ph=%p YES_rel_QWebHistoryItem   \\./", p->ph ) );
         p->ph = NULL;
      }
      else
      {
         HB_TRACE( HB_TR_DEBUG, ( "ph=%p DEL_rel_QWebHistoryItem    :     Object already deleted!", p->ph ) );
         p->ph = NULL;
      }
   }
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "ph=%p PTR_rel_QWebHistoryItem    :    Object not created with new=true", p->ph ) );
      p->ph = NULL;
   }
}

void * hbqt_gcAllocate_QWebHistoryItem( void * pObj, bool bNew )
{
   HBQT_GC_T * p = ( HBQT_GC_T * ) hb_gcAllocate( sizeof( HBQT_GC_T ), hbqt_gcFuncs() );

   p->ph = ( QWebHistoryItem * ) pObj;
   p->bNew = bNew;
   p->func = hbqt_gcRelease_QWebHistoryItem;
   p->type = HBQT_TYPE_QWebHistoryItem;

   if( bNew )
   {
      HB_TRACE( HB_TR_DEBUG, ( "ph=%p    _new_QWebHistoryItem", pObj ) );
   }
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "ph=%p NOT_new_QWebHistoryItem", pObj ) );
   }
   return p;
}

HB_FUNC( QT_QWEBHISTORYITEM )
{
   QWebHistoryItem * pObj = NULL;

   if( hb_pcount() == 1 && HB_ISPOINTER( 1 ) )
   {
      pObj = new QWebHistoryItem( *hbqt_par_QWebHistoryItem( 1 ) ) ;
   }

   hb_retptrGC( hbqt_gcAllocate_QWebHistoryItem( ( void * ) pObj, true ) );
}

/*
 * QIcon icon () const
 */
HB_FUNC( QT_QWEBHISTORYITEM_ICON )
{
   QWebHistoryItem * p = hbqt_par_QWebHistoryItem( 1 );
   if( p )
      hb_retptrGC( hbqt_gcAllocate_QIcon( new QIcon( ( p )->icon() ), true ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QWEBHISTORYITEM_ICON FP=hb_retptrGC( hbqt_gcAllocate_QIcon( new QIcon( ( p )->icon() ), true ) ); p is NULL" ) );
   }
}

/*
 * bool isValid () const
 */
HB_FUNC( QT_QWEBHISTORYITEM_ISVALID )
{
   QWebHistoryItem * p = hbqt_par_QWebHistoryItem( 1 );
   if( p )
      hb_retl( ( p )->isValid() );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QWEBHISTORYITEM_ISVALID FP=hb_retl( ( p )->isValid() ); p is NULL" ) );
   }
}

/*
 * QDateTime lastVisited () const
 */
HB_FUNC( QT_QWEBHISTORYITEM_LASTVISITED )
{
   QWebHistoryItem * p = hbqt_par_QWebHistoryItem( 1 );
   if( p )
      hb_retptrGC( hbqt_gcAllocate_QDateTime( new QDateTime( ( p )->lastVisited() ), true ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QWEBHISTORYITEM_LASTVISITED FP=hb_retptrGC( hbqt_gcAllocate_QDateTime( new QDateTime( ( p )->lastVisited() ), true ) ); p is NULL" ) );
   }
}

/*
 * QUrl originalUrl () const
 */
HB_FUNC( QT_QWEBHISTORYITEM_ORIGINALURL )
{
   QWebHistoryItem * p = hbqt_par_QWebHistoryItem( 1 );
   if( p )
      hb_retptrGC( hbqt_gcAllocate_QUrl( new QUrl( ( p )->originalUrl() ), true ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QWEBHISTORYITEM_ORIGINALURL FP=hb_retptrGC( hbqt_gcAllocate_QUrl( new QUrl( ( p )->originalUrl() ), true ) ); p is NULL" ) );
   }
}

/*
 * void setUserData ( const QVariant & userData )
 */
HB_FUNC( QT_QWEBHISTORYITEM_SETUSERDATA )
{
   QWebHistoryItem * p = hbqt_par_QWebHistoryItem( 1 );
   if( p )
      ( p )->setUserData( *hbqt_par_QVariant( 2 ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QWEBHISTORYITEM_SETUSERDATA FP=( p )->setUserData( *hbqt_par_QVariant( 2 ) ); p is NULL" ) );
   }
}

/*
 * QString title () const
 */
HB_FUNC( QT_QWEBHISTORYITEM_TITLE )
{
   QWebHistoryItem * p = hbqt_par_QWebHistoryItem( 1 );
   if( p )
      hb_retc( ( p )->title().toAscii().data() );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QWEBHISTORYITEM_TITLE FP=hb_retc( ( p )->title().toAscii().data() ); p is NULL" ) );
   }
}

/*
 * QUrl url () const
 */
HB_FUNC( QT_QWEBHISTORYITEM_URL )
{
   QWebHistoryItem * p = hbqt_par_QWebHistoryItem( 1 );
   if( p )
      hb_retptrGC( hbqt_gcAllocate_QUrl( new QUrl( ( p )->url() ), true ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QWEBHISTORYITEM_URL FP=hb_retptrGC( hbqt_gcAllocate_QUrl( new QUrl( ( p )->url() ), true ) ); p is NULL" ) );
   }
}

/*
 * QVariant userData () const
 */
HB_FUNC( QT_QWEBHISTORYITEM_USERDATA )
{
   QWebHistoryItem * p = hbqt_par_QWebHistoryItem( 1 );
   if( p )
      hb_retptrGC( hbqt_gcAllocate_QVariant( new QVariant( ( p )->userData() ), true ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QWEBHISTORYITEM_USERDATA FP=hb_retptrGC( hbqt_gcAllocate_QVariant( new QVariant( ( p )->userData() ), true ) ); p is NULL" ) );
   }
}


/*----------------------------------------------------------------------*/
#endif             /* #if QT_VERSION >= 0x040500 */
/*----------------------------------------------------------------------*/
