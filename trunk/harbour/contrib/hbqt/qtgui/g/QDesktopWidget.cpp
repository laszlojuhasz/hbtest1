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

#include "hbqt.h"
#include "hbqtgui_garbage.h"
#include "hbqtcore_garbage.h"

/*----------------------------------------------------------------------*/
#if QT_VERSION >= 0x040500
/*----------------------------------------------------------------------*/

#include <QtCore/QPointer>

#include <QtGui/QDesktopWidget>


/*
 * QDesktopWidget ()
 * ~QDesktopWidget ()
 */

typedef struct
{
   QPointer< QDesktopWidget > ph;
   bool bNew;
   QT_G_FUNC_PTR func;
   int type;
} QGC_POINTER_QDesktopWidget;

QT_G_FUNC( hbqt_gcRelease_QDesktopWidget )
{
   QDesktopWidget  * ph = NULL ;
   QGC_POINTER_QDesktopWidget * p = ( QGC_POINTER_QDesktopWidget * ) Cargo;

   if( p && p->bNew && p->ph )
   {
      ph = p->ph;
      if( ph )
      {
         const QMetaObject * m = ( ph )->metaObject();
         if( ( QString ) m->className() != ( QString ) "QObject" )
         {
            HB_TRACE( HB_TR_DEBUG, ( "ph=%p %p YES_rel_QDesktopWidget   /.\\   ", (void*) ph, (void*) p->ph ) );
            delete ( p->ph );
            HB_TRACE( HB_TR_DEBUG, ( "ph=%p %p YES_rel_QDesktopWidget   \\./   ", (void*) ph, (void*) p->ph ) );
            p->ph = NULL;
         }
         else
         {
            HB_TRACE( HB_TR_DEBUG, ( "ph=%p NO__rel_QDesktopWidget          ", ph ) );
            p->ph = NULL;
         }
      }
      else
      {
         HB_TRACE( HB_TR_DEBUG, ( "ph=%p DEL_rel_QDesktopWidget    :     Object already deleted!", ph ) );
         p->ph = NULL;
      }
   }
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "ph=%p PTR_rel_QDesktopWidget    :    Object not created with new=true", ph ) );
      p->ph = NULL;
   }
}

void * hbqt_gcAllocate_QDesktopWidget( void * pObj, bool bNew )
{
   QGC_POINTER_QDesktopWidget * p = ( QGC_POINTER_QDesktopWidget * ) hb_gcAllocate( sizeof( QGC_POINTER_QDesktopWidget ), hbqt_gcFuncs() );

   new( & p->ph ) QPointer< QDesktopWidget >( ( QDesktopWidget * ) pObj );
   p->bNew = bNew;
   p->func = hbqt_gcRelease_QDesktopWidget;
   p->type = HBQT_TYPE_QDesktopWidget;

   if( bNew )
   {
      HB_TRACE( HB_TR_DEBUG, ( "ph=%p    _new_QDesktopWidget  under p->pq", pObj ) );
   }
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "ph=%p NOT_new_QDesktopWidget", pObj ) );
   }
   return p;
}

HB_FUNC( QT_QDESKTOPWIDGET )
{
   QDesktopWidget * pObj = NULL;

   pObj = new QDesktopWidget() ;

   hb_retptrGC( hbqt_gcAllocate_QDesktopWidget( ( void * ) pObj, true ) );
}

/*
 * const QRect availableGeometry ( int screen = -1 ) const
 */
HB_FUNC( QT_QDESKTOPWIDGET_AVAILABLEGEOMETRY )
{
   QDesktopWidget * p = hbqt_par_QDesktopWidget( 1 );
   if( p )
      hb_retptrGC( hbqt_gcAllocate_QRect( new QRect( ( p )->availableGeometry( hb_parnidef( 2, -1 ) ) ), true ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QDESKTOPWIDGET_AVAILABLEGEOMETRY FP=hb_retptrGC( hbqt_gcAllocate_QRect( new QRect( ( p )->availableGeometry( hb_parnidef( 2, -1 ) ) ), true ) ); p is NULL" ) );
   }
}

/*
 * const QRect availableGeometry ( const QWidget * widget ) const
 */
HB_FUNC( QT_QDESKTOPWIDGET_AVAILABLEGEOMETRY_1 )
{
   QDesktopWidget * p = hbqt_par_QDesktopWidget( 1 );
   if( p )
      hb_retptrGC( hbqt_gcAllocate_QRect( new QRect( ( p )->availableGeometry( hbqt_par_QWidget( 2 ) ) ), true ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QDESKTOPWIDGET_AVAILABLEGEOMETRY_1 FP=hb_retptrGC( hbqt_gcAllocate_QRect( new QRect( ( p )->availableGeometry( hbqt_par_QWidget( 2 ) ) ), true ) ); p is NULL" ) );
   }
}

/*
 * const QRect availableGeometry ( const QPoint & p ) const
 */
HB_FUNC( QT_QDESKTOPWIDGET_AVAILABLEGEOMETRY_2 )
{
   QDesktopWidget * p = hbqt_par_QDesktopWidget( 1 );
   if( p )
      hb_retptrGC( hbqt_gcAllocate_QRect( new QRect( ( p )->availableGeometry( *hbqt_par_QPoint( 2 ) ) ), true ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QDESKTOPWIDGET_AVAILABLEGEOMETRY_2 FP=hb_retptrGC( hbqt_gcAllocate_QRect( new QRect( ( p )->availableGeometry( *hbqt_par_QPoint( 2 ) ) ), true ) ); p is NULL" ) );
   }
}

/*
 * bool isVirtualDesktop () const
 */
HB_FUNC( QT_QDESKTOPWIDGET_ISVIRTUALDESKTOP )
{
   QDesktopWidget * p = hbqt_par_QDesktopWidget( 1 );
   if( p )
      hb_retl( ( p )->isVirtualDesktop() );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QDESKTOPWIDGET_ISVIRTUALDESKTOP FP=hb_retl( ( p )->isVirtualDesktop() ); p is NULL" ) );
   }
}

/*
 * int numScreens () const
 */
HB_FUNC( QT_QDESKTOPWIDGET_NUMSCREENS )
{
   QDesktopWidget * p = hbqt_par_QDesktopWidget( 1 );
   if( p )
      hb_retni( ( p )->numScreens() );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QDESKTOPWIDGET_NUMSCREENS FP=hb_retni( ( p )->numScreens() ); p is NULL" ) );
   }
}

/*
 * int primaryScreen () const
 */
HB_FUNC( QT_QDESKTOPWIDGET_PRIMARYSCREEN )
{
   QDesktopWidget * p = hbqt_par_QDesktopWidget( 1 );
   if( p )
      hb_retni( ( p )->primaryScreen() );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QDESKTOPWIDGET_PRIMARYSCREEN FP=hb_retni( ( p )->primaryScreen() ); p is NULL" ) );
   }
}

/*
 * QWidget * screen ( int screen = -1 )
 */
HB_FUNC( QT_QDESKTOPWIDGET_SCREEN )
{
   QDesktopWidget * p = hbqt_par_QDesktopWidget( 1 );
   if( p )
      hb_retptrGC( hbqt_gcAllocate_QWidget( ( p )->screen( hb_parnidef( 2, -1 ) ), false ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QDESKTOPWIDGET_SCREEN FP=hb_retptrGC( hbqt_gcAllocate_QWidget( ( p )->screen( hb_parnidef( 2, -1 ) ), false ) ); p is NULL" ) );
   }
}

/*
 * const QRect screenGeometry ( int screen = -1 ) const
 */
HB_FUNC( QT_QDESKTOPWIDGET_SCREENGEOMETRY )
{
   QDesktopWidget * p = hbqt_par_QDesktopWidget( 1 );
   if( p )
      hb_retptrGC( hbqt_gcAllocate_QRect( new QRect( ( p )->screenGeometry( hb_parnidef( 2, -1 ) ) ), true ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QDESKTOPWIDGET_SCREENGEOMETRY FP=hb_retptrGC( hbqt_gcAllocate_QRect( new QRect( ( p )->screenGeometry( hb_parnidef( 2, -1 ) ) ), true ) ); p is NULL" ) );
   }
}

/*
 * const QRect screenGeometry ( const QWidget * widget ) const
 */
HB_FUNC( QT_QDESKTOPWIDGET_SCREENGEOMETRY_1 )
{
   QDesktopWidget * p = hbqt_par_QDesktopWidget( 1 );
   if( p )
      hb_retptrGC( hbqt_gcAllocate_QRect( new QRect( ( p )->screenGeometry( hbqt_par_QWidget( 2 ) ) ), true ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QDESKTOPWIDGET_SCREENGEOMETRY_1 FP=hb_retptrGC( hbqt_gcAllocate_QRect( new QRect( ( p )->screenGeometry( hbqt_par_QWidget( 2 ) ) ), true ) ); p is NULL" ) );
   }
}

/*
 * const QRect screenGeometry ( const QPoint & p ) const
 */
HB_FUNC( QT_QDESKTOPWIDGET_SCREENGEOMETRY_2 )
{
   QDesktopWidget * p = hbqt_par_QDesktopWidget( 1 );
   if( p )
      hb_retptrGC( hbqt_gcAllocate_QRect( new QRect( ( p )->screenGeometry( *hbqt_par_QPoint( 2 ) ) ), true ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QDESKTOPWIDGET_SCREENGEOMETRY_2 FP=hb_retptrGC( hbqt_gcAllocate_QRect( new QRect( ( p )->screenGeometry( *hbqt_par_QPoint( 2 ) ) ), true ) ); p is NULL" ) );
   }
}

/*
 * int screenNumber ( const QWidget * widget = 0 ) const
 */
HB_FUNC( QT_QDESKTOPWIDGET_SCREENNUMBER )
{
   QDesktopWidget * p = hbqt_par_QDesktopWidget( 1 );
   if( p )
      hb_retni( ( p )->screenNumber( hbqt_par_QWidget( 2 ) ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QDESKTOPWIDGET_SCREENNUMBER FP=hb_retni( ( p )->screenNumber( hbqt_par_QWidget( 2 ) ) ); p is NULL" ) );
   }
}

/*
 * int screenNumber ( const QPoint & point ) const
 */
HB_FUNC( QT_QDESKTOPWIDGET_SCREENNUMBER_1 )
{
   QDesktopWidget * p = hbqt_par_QDesktopWidget( 1 );
   if( p )
      hb_retni( ( p )->screenNumber( *hbqt_par_QPoint( 2 ) ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QDESKTOPWIDGET_SCREENNUMBER_1 FP=hb_retni( ( p )->screenNumber( *hbqt_par_QPoint( 2 ) ) ); p is NULL" ) );
   }
}


/*----------------------------------------------------------------------*/
#endif             /* #if QT_VERSION >= 0x040500 */
/*----------------------------------------------------------------------*/
