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

/*
 *  Constructed[ 12/12 [ 100.00% ] ]
 *
 */

#include <QtCore/QPointer>

#include <QtGui/QGraphicsSceneDragDropEvent>
#include <QtGui/QWidget>
#include <QtCore/QMimeData>
#include <QtCore/QPointF>
#include <QtCore/QPoint>


/*
 * ~QGraphicsSceneDragDropEvent ()
 */

typedef struct
{
   QGraphicsSceneDragDropEvent * ph;
   bool bNew;
   PHBQT_GC_FUNC func;
   int type;
} HBQT_GC_T_QGraphicsSceneDragDropEvent;

HBQT_GC_FUNC( hbqt_gcRelease_QGraphicsSceneDragDropEvent )
{
   HB_SYMBOL_UNUSED( Cargo );
   HBQT_GC_T * p = ( HBQT_GC_T * ) Cargo;

   if( p && p->bNew )
   {
      p->ph = NULL;
   }
}

void * hbqt_gcAllocate_QGraphicsSceneDragDropEvent( void * pObj, bool bNew )
{
   HBQT_GC_T * p = ( HBQT_GC_T * ) hb_gcAllocate( sizeof( HBQT_GC_T ), hbqt_gcFuncs() );

   p->ph = ( QGraphicsSceneDragDropEvent * ) pObj;
   p->bNew = bNew;
   p->func = hbqt_gcRelease_QGraphicsSceneDragDropEvent;
   p->type = HBQT_TYPE_QGraphicsSceneDragDropEvent;

   if( bNew )
   {
      HB_TRACE( HB_TR_DEBUG, ( "ph=%p    _new_QGraphicsSceneDragDropEvent", pObj ) );
   }
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "ph=%p NOT_new_QGraphicsSceneDragDropEvent", pObj ) );
   }
   return p;
}

HB_FUNC( QT_QGRAPHICSSCENEDRAGDROPEVENT )
{
   //hb_retptr( new QGraphicsSceneDragDropEvent() );
}

/*
 * void acceptProposedAction ()
 */
HB_FUNC( QT_QGRAPHICSSCENEDRAGDROPEVENT_ACCEPTPROPOSEDACTION )
{
   QGraphicsSceneDragDropEvent * p = hbqt_par_QGraphicsSceneDragDropEvent( 1 );
   if( p )
   {
      ( p )->acceptProposedAction();
   }
}

/*
 * Qt::MouseButtons buttons () const
 */
HB_FUNC( QT_QGRAPHICSSCENEDRAGDROPEVENT_BUTTONS )
{
   QGraphicsSceneDragDropEvent * p = hbqt_par_QGraphicsSceneDragDropEvent( 1 );
   if( p )
   {
      hb_retni( ( Qt::MouseButtons ) ( p )->buttons() );
   }
}

/*
 * Qt::DropAction dropAction () const
 */
HB_FUNC( QT_QGRAPHICSSCENEDRAGDROPEVENT_DROPACTION )
{
   QGraphicsSceneDragDropEvent * p = hbqt_par_QGraphicsSceneDragDropEvent( 1 );
   if( p )
   {
      hb_retni( ( Qt::DropAction ) ( p )->dropAction() );
   }
}

/*
 * const QMimeData * mimeData () const
 */
HB_FUNC( QT_QGRAPHICSSCENEDRAGDROPEVENT_MIMEDATA )
{
   hb_retptrGC( hbqt_gcAllocate_QMimeData( ( void* ) hbqt_par_QGraphicsSceneDragDropEvent( 1 )->mimeData(), false ) );
}

/*
 * Qt::KeyboardModifiers modifiers () const
 */
HB_FUNC( QT_QGRAPHICSSCENEDRAGDROPEVENT_MODIFIERS )
{
   QGraphicsSceneDragDropEvent * p = hbqt_par_QGraphicsSceneDragDropEvent( 1 );
   if( p )
   {
      hb_retni( ( Qt::KeyboardModifiers ) ( p )->modifiers() );
   }
}

/*
 * QPointF pos () const
 */
HB_FUNC( QT_QGRAPHICSSCENEDRAGDROPEVENT_POS )
{
   QGraphicsSceneDragDropEvent * p = hbqt_par_QGraphicsSceneDragDropEvent( 1 );
   if( p )
   {
      hb_retptrGC( hbqt_gcAllocate_QPointF( new QPointF( ( p )->pos() ), true ) );
   }
}

/*
 * Qt::DropActions possibleActions () const
 */
HB_FUNC( QT_QGRAPHICSSCENEDRAGDROPEVENT_POSSIBLEACTIONS )
{
   QGraphicsSceneDragDropEvent * p = hbqt_par_QGraphicsSceneDragDropEvent( 1 );
   if( p )
   {
      hb_retni( ( Qt::DropActions ) ( p )->possibleActions() );
   }
}

/*
 * Qt::DropAction proposedAction () const
 */
HB_FUNC( QT_QGRAPHICSSCENEDRAGDROPEVENT_PROPOSEDACTION )
{
   QGraphicsSceneDragDropEvent * p = hbqt_par_QGraphicsSceneDragDropEvent( 1 );
   if( p )
   {
      hb_retni( ( Qt::DropAction ) ( p )->proposedAction() );
   }
}

/*
 * QPointF scenePos () const
 */
HB_FUNC( QT_QGRAPHICSSCENEDRAGDROPEVENT_SCENEPOS )
{
   QGraphicsSceneDragDropEvent * p = hbqt_par_QGraphicsSceneDragDropEvent( 1 );
   if( p )
   {
      hb_retptrGC( hbqt_gcAllocate_QPointF( new QPointF( ( p )->scenePos() ), true ) );
   }
}

/*
 * QPoint screenPos () const
 */
HB_FUNC( QT_QGRAPHICSSCENEDRAGDROPEVENT_SCREENPOS )
{
   QGraphicsSceneDragDropEvent * p = hbqt_par_QGraphicsSceneDragDropEvent( 1 );
   if( p )
   {
      hb_retptrGC( hbqt_gcAllocate_QPoint( new QPoint( ( p )->screenPos() ), true ) );
   }
}

/*
 * void setDropAction ( Qt::DropAction action )
 */
HB_FUNC( QT_QGRAPHICSSCENEDRAGDROPEVENT_SETDROPACTION )
{
   QGraphicsSceneDragDropEvent * p = hbqt_par_QGraphicsSceneDragDropEvent( 1 );
   if( p )
   {
      ( p )->setDropAction( ( Qt::DropAction ) hb_parni( 2 ) );
   }
}

/*
 * QWidget * source () const
 */
HB_FUNC( QT_QGRAPHICSSCENEDRAGDROPEVENT_SOURCE )
{
   QGraphicsSceneDragDropEvent * p = hbqt_par_QGraphicsSceneDragDropEvent( 1 );
   if( p )
   {
      hb_retptrGC( hbqt_gcAllocate_QWidget( ( p )->source(), false ) );
   }
}


/*----------------------------------------------------------------------*/
#endif             /* #if QT_VERSION >= 0x040500 */
/*----------------------------------------------------------------------*/
