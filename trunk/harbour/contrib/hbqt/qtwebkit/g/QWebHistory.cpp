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

#include <QtWebKit/QWebHistory>


/*
 *
 */

typedef struct
{
   QWebHistory * ph;
   bool bNew;
   PHBQT_GC_FUNC func;
   int type;
} HBQT_GC_T_QWebHistory;

HBQT_GC_FUNC( hbqt_gcRelease_QWebHistory )
{
   HB_SYMBOL_UNUSED( Cargo );
   HBQT_GC_T * p = ( HBQT_GC_T * ) Cargo;

   if( p && p->bNew )
   {
      p->ph = NULL;
   }
}

void * hbqt_gcAllocate_QWebHistory( void * pObj, bool bNew )
{
   HBQT_GC_T * p = ( HBQT_GC_T * ) hb_gcAllocate( sizeof( HBQT_GC_T ), hbqt_gcFuncs() );

   p->ph = ( QWebHistory * ) pObj;
   p->bNew = bNew;
   p->func = hbqt_gcRelease_QWebHistory;
   p->type = HBQT_TYPE_QWebHistory;

   if( bNew )
   {
      HB_TRACE( HB_TR_DEBUG, ( "ph=%p    _new_QWebHistory", pObj ) );
   }
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "ph=%p NOT_new_QWebHistory", pObj ) );
   }
   return p;
}

HB_FUNC( QT_QWEBHISTORY )
{
   //hb_retptr( ( * ) new () );
}

/*
 * void back ()
 */
HB_FUNC( QT_QWEBHISTORY_BACK )
{
   QWebHistory * p = hbqt_par_QWebHistory( 1 );
   if( p )
   {
      ( p )->back();
   }
}

/*
 * QWebHistoryItem backItem () const
 */
HB_FUNC( QT_QWEBHISTORY_BACKITEM )
{
   QWebHistory * p = hbqt_par_QWebHistory( 1 );
   if( p )
   {
      hb_retptrGC( hbqt_gcAllocate_QWebHistoryItem( new QWebHistoryItem( ( p )->backItem() ), true ) );
   }
}

/*
 * QList<QWebHistoryItem> backItems ( int maxItems ) const
 */
HB_FUNC( QT_QWEBHISTORY_BACKITEMS )
{
   QWebHistory * p = hbqt_par_QWebHistory( 1 );
   if( p )
   {
      hb_retptrGC( hbqt_gcAllocate_QList( new QList<QWebHistoryItem>( ( p )->backItems( hb_parni( 2 ) ) ), true ) );
   }
}

/*
 * bool canGoBack () const
 */
HB_FUNC( QT_QWEBHISTORY_CANGOBACK )
{
   QWebHistory * p = hbqt_par_QWebHistory( 1 );
   if( p )
   {
      hb_retl( ( p )->canGoBack() );
   }
}

/*
 * bool canGoForward () const
 */
HB_FUNC( QT_QWEBHISTORY_CANGOFORWARD )
{
   QWebHistory * p = hbqt_par_QWebHistory( 1 );
   if( p )
   {
      hb_retl( ( p )->canGoForward() );
   }
}

/*
 * void clear ()
 */
HB_FUNC( QT_QWEBHISTORY_CLEAR )
{
   QWebHistory * p = hbqt_par_QWebHistory( 1 );
   if( p )
   {
      ( p )->clear();
   }
}

/*
 * int count () const
 */
HB_FUNC( QT_QWEBHISTORY_COUNT )
{
   QWebHistory * p = hbqt_par_QWebHistory( 1 );
   if( p )
   {
      hb_retni( ( p )->count() );
   }
}

/*
 * QWebHistoryItem currentItem () const
 */
HB_FUNC( QT_QWEBHISTORY_CURRENTITEM )
{
   QWebHistory * p = hbqt_par_QWebHistory( 1 );
   if( p )
   {
      hb_retptrGC( hbqt_gcAllocate_QWebHistoryItem( new QWebHistoryItem( ( p )->currentItem() ), true ) );
   }
}

/*
 * int currentItemIndex () const
 */
HB_FUNC( QT_QWEBHISTORY_CURRENTITEMINDEX )
{
   QWebHistory * p = hbqt_par_QWebHistory( 1 );
   if( p )
   {
      hb_retni( ( p )->currentItemIndex() );
   }
}

/*
 * void forward ()
 */
HB_FUNC( QT_QWEBHISTORY_FORWARD )
{
   QWebHistory * p = hbqt_par_QWebHistory( 1 );
   if( p )
   {
      ( p )->forward();
   }
}

/*
 * QWebHistoryItem forwardItem () const
 */
HB_FUNC( QT_QWEBHISTORY_FORWARDITEM )
{
   QWebHistory * p = hbqt_par_QWebHistory( 1 );
   if( p )
   {
      hb_retptrGC( hbqt_gcAllocate_QWebHistoryItem( new QWebHistoryItem( ( p )->forwardItem() ), true ) );
   }
}

/*
 * QList<QWebHistoryItem> forwardItems ( int maxItems ) const
 */
HB_FUNC( QT_QWEBHISTORY_FORWARDITEMS )
{
   QWebHistory * p = hbqt_par_QWebHistory( 1 );
   if( p )
   {
      hb_retptrGC( hbqt_gcAllocate_QList( new QList<QWebHistoryItem>( ( p )->forwardItems( hb_parni( 2 ) ) ), true ) );
   }
}

/*
 * void goToItem ( const QWebHistoryItem & item )
 */
HB_FUNC( QT_QWEBHISTORY_GOTOITEM )
{
   QWebHistory * p = hbqt_par_QWebHistory( 1 );
   if( p )
   {
      ( p )->goToItem( *hbqt_par_QWebHistoryItem( 2 ) );
   }
}

/*
 * QWebHistoryItem itemAt ( int i ) const
 */
HB_FUNC( QT_QWEBHISTORY_ITEMAT )
{
   QWebHistory * p = hbqt_par_QWebHistory( 1 );
   if( p )
   {
      hb_retptrGC( hbqt_gcAllocate_QWebHistoryItem( new QWebHistoryItem( ( p )->itemAt( hb_parni( 2 ) ) ), true ) );
   }
}

/*
 * QList<QWebHistoryItem> items () const
 */
HB_FUNC( QT_QWEBHISTORY_ITEMS )
{
   QWebHistory * p = hbqt_par_QWebHistory( 1 );
   if( p )
   {
      hb_retptrGC( hbqt_gcAllocate_QList( new QList<QWebHistoryItem>( ( p )->items() ), true ) );
   }
}

/*
 * int maximumItemCount () const
 */
HB_FUNC( QT_QWEBHISTORY_MAXIMUMITEMCOUNT )
{
   QWebHistory * p = hbqt_par_QWebHistory( 1 );
   if( p )
   {
      hb_retni( ( p )->maximumItemCount() );
   }
}

/*
 * void setMaximumItemCount ( int count )
 */
HB_FUNC( QT_QWEBHISTORY_SETMAXIMUMITEMCOUNT )
{
   QWebHistory * p = hbqt_par_QWebHistory( 1 );
   if( p )
   {
      ( p )->setMaximumItemCount( hb_parni( 2 ) );
   }
}


/*----------------------------------------------------------------------*/
#endif             /* #if QT_VERSION >= 0x040500 */
/*----------------------------------------------------------------------*/
