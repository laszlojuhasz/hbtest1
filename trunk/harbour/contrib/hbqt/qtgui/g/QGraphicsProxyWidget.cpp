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
#include "hbqtgui.h"

/*----------------------------------------------------------------------*/
#if QT_VERSION >= 0x040500
/*----------------------------------------------------------------------*/

#include <QtCore/QPointer>

#include <QtGui/QGraphicsProxyWidget>


/*
 * QGraphicsProxyWidget ( QGraphicsItem * parent = 0, Qt::WindowFlags wFlags = 0 )
 * ~QGraphicsProxyWidget ()
 */

typedef struct
{
   QPointer< QGraphicsProxyWidget > ph;
   bool bNew;
   PHBQT_GC_FUNC func;
   int type;
} HBQT_GC_T_QGraphicsProxyWidget;

HBQT_GC_FUNC( hbqt_gcRelease_QGraphicsProxyWidget )
{
   QGraphicsProxyWidget  * ph = NULL ;
   HBQT_GC_T_QGraphicsProxyWidget * p = ( HBQT_GC_T_QGraphicsProxyWidget * ) Cargo;

   if( p && p->bNew && p->ph )
   {
      ph = p->ph;
      if( ph )
      {
         const QMetaObject * m = ( ph )->metaObject();
         if( ( QString ) m->className() != ( QString ) "QObject" )
         {
            HB_TRACE( HB_TR_DEBUG, ( "ph=%p %p YES_rel_QGraphicsProxyWidget   /.\\   ", (void*) ph, (void*) p->ph ) );
            delete ( p->ph );
            HB_TRACE( HB_TR_DEBUG, ( "ph=%p %p YES_rel_QGraphicsProxyWidget   \\./   ", (void*) ph, (void*) p->ph ) );
            p->ph = NULL;
         }
         else
         {
            HB_TRACE( HB_TR_DEBUG, ( "ph=%p NO__rel_QGraphicsProxyWidget          ", ph ) );
            p->ph = NULL;
         }
      }
      else
      {
         HB_TRACE( HB_TR_DEBUG, ( "ph=%p DEL_rel_QGraphicsProxyWidget    :     Object already deleted!", ph ) );
         p->ph = NULL;
      }
   }
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "ph=%p PTR_rel_QGraphicsProxyWidget    :    Object not created with new=true", ph ) );
      p->ph = NULL;
   }
}

void * hbqt_gcAllocate_QGraphicsProxyWidget( void * pObj, bool bNew )
{
   HBQT_GC_T_QGraphicsProxyWidget * p = ( HBQT_GC_T_QGraphicsProxyWidget * ) hb_gcAllocate( sizeof( HBQT_GC_T_QGraphicsProxyWidget ), hbqt_gcFuncs() );

   new( & p->ph ) QPointer< QGraphicsProxyWidget >( ( QGraphicsProxyWidget * ) pObj );
   p->bNew = bNew;
   p->func = hbqt_gcRelease_QGraphicsProxyWidget;
   p->type = HBQT_TYPE_QGraphicsProxyWidget;

   if( bNew )
   {
      HB_TRACE( HB_TR_DEBUG, ( "ph=%p    _new_QGraphicsProxyWidget  under p->pq", pObj ) );
   }
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "ph=%p NOT_new_QGraphicsProxyWidget", pObj ) );
   }
   return p;
}

HB_FUNC( QT_QGRAPHICSPROXYWIDGET )
{
   QGraphicsProxyWidget * pObj = NULL;

   if( hb_pcount() >= 1 && HB_ISPOINTER( 1 ) )
   {
      pObj = new QGraphicsProxyWidget( hbqt_par_QGraphicsItem( 1 ), ( Qt::WindowFlags ) ( HB_ISNUM( 2 ) ? hb_parni( 2 ) : 0 ) ) ;
   }
   else
   {
      pObj = new QGraphicsProxyWidget() ;
   }

   hb_retptrGC( hbqt_gcAllocate_QGraphicsProxyWidget( ( void * ) pObj, true ) );
}

/*
 * QGraphicsProxyWidget * createProxyForChildWidget ( QWidget * child )
 */
HB_FUNC( QT_QGRAPHICSPROXYWIDGET_CREATEPROXYFORCHILDWIDGET )
{
   QGraphicsProxyWidget * p = hbqt_par_QGraphicsProxyWidget( 1 );
   if( p )
      hb_retptrGC( hbqt_gcAllocate_QGraphicsProxyWidget( ( p )->createProxyForChildWidget( hbqt_par_QWidget( 2 ) ), false ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QGRAPHICSPROXYWIDGET_CREATEPROXYFORCHILDWIDGET FP=hb_retptrGC( hbqt_gcAllocate_QGraphicsProxyWidget( ( p )->createProxyForChildWidget( hbqt_par_QWidget( 2 ) ), false ) ); p is NULL" ) );
   }
}

/*
 * void setWidget ( QWidget * widget )
 */
HB_FUNC( QT_QGRAPHICSPROXYWIDGET_SETWIDGET )
{
   QGraphicsProxyWidget * p = hbqt_par_QGraphicsProxyWidget( 1 );
   if( p )
      ( p )->setWidget( hbqt_par_QWidget( 2 ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QGRAPHICSPROXYWIDGET_SETWIDGET FP=( p )->setWidget( hbqt_par_QWidget( 2 ) ); p is NULL" ) );
   }
}

/*
 * QRectF subWidgetRect ( const QWidget * widget ) const
 */
HB_FUNC( QT_QGRAPHICSPROXYWIDGET_SUBWIDGETRECT )
{
   QGraphicsProxyWidget * p = hbqt_par_QGraphicsProxyWidget( 1 );
   if( p )
      hb_retptrGC( hbqt_gcAllocate_QRectF( new QRectF( ( p )->subWidgetRect( hbqt_par_QWidget( 2 ) ) ), true ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QGRAPHICSPROXYWIDGET_SUBWIDGETRECT FP=hb_retptrGC( hbqt_gcAllocate_QRectF( new QRectF( ( p )->subWidgetRect( hbqt_par_QWidget( 2 ) ) ), true ) ); p is NULL" ) );
   }
}

/*
 * QWidget * widget () const
 */
HB_FUNC( QT_QGRAPHICSPROXYWIDGET_WIDGET )
{
   QGraphicsProxyWidget * p = hbqt_par_QGraphicsProxyWidget( 1 );
   if( p )
      hb_retptrGC( hbqt_gcAllocate_QWidget( ( p )->widget(), false ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QGRAPHICSPROXYWIDGET_WIDGET FP=hb_retptrGC( hbqt_gcAllocate_QWidget( ( p )->widget(), false ) ); p is NULL" ) );
   }
}


/*----------------------------------------------------------------------*/
#endif             /* #if QT_VERSION >= 0x040500 */
/*----------------------------------------------------------------------*/
