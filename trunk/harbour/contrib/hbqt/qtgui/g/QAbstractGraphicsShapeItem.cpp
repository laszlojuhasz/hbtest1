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
 *  Constructed[ 4/4 [ 100.00% ] ]
 *
 */

#include <QtCore/QPointer>

#include <QtGui/QAbstractGraphicsShapeItem>
#include <QtGui/QBrush>
#include <QtGui/QPen>


/*
 * QAbstractGraphicsShapeItem ( QGraphicsItem * parent = 0 )
 * ~QAbstractGraphicsShapeItem ()
 */

typedef struct
{
   QAbstractGraphicsShapeItem * ph;
   bool bNew;
   PHBQT_GC_FUNC func;
   int type;
} HBQT_GC_T_QAbstractGraphicsShapeItem;

HBQT_GC_FUNC( hbqt_gcRelease_QAbstractGraphicsShapeItem )
{
   HB_SYMBOL_UNUSED( Cargo );
   HBQT_GC_T * p = ( HBQT_GC_T * ) Cargo;

   if( p && p->bNew )
   {
      p->ph = NULL;
   }
}

void * hbqt_gcAllocate_QAbstractGraphicsShapeItem( void * pObj, bool bNew )
{
   HBQT_GC_T * p = ( HBQT_GC_T * ) hb_gcAllocate( sizeof( HBQT_GC_T ), hbqt_gcFuncs() );

   p->ph = ( QAbstractGraphicsShapeItem * ) pObj;
   p->bNew = bNew;
   p->func = hbqt_gcRelease_QAbstractGraphicsShapeItem;
   p->type = HBQT_TYPE_QAbstractGraphicsShapeItem;

   if( bNew )
   {
      HB_TRACE( HB_TR_DEBUG, ( "ph=%p    _new_QAbstractGraphicsShapeItem", pObj ) );
   }
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "ph=%p NOT_new_QAbstractGraphicsShapeItem", pObj ) );
   }
   return p;
}

HB_FUNC( QT_QABSTRACTGRAPHICSSHAPEITEM )
{
   // hb_retptr( new QAbstractGraphicsShapeItem() );
}

/*
 * QBrush brush () const
 */
HB_FUNC( QT_QABSTRACTGRAPHICSSHAPEITEM_BRUSH )
{
   QAbstractGraphicsShapeItem * p = hbqt_par_QAbstractGraphicsShapeItem( 1 );
   if( p )
   {
      hb_retptrGC( hbqt_gcAllocate_QBrush( new QBrush( ( p )->brush() ), true ) );
   }
}

/*
 * QPen pen () const
 */
HB_FUNC( QT_QABSTRACTGRAPHICSSHAPEITEM_PEN )
{
   QAbstractGraphicsShapeItem * p = hbqt_par_QAbstractGraphicsShapeItem( 1 );
   if( p )
   {
      hb_retptrGC( hbqt_gcAllocate_QPen( new QPen( ( p )->pen() ), true ) );
   }
}

/*
 * void setBrush ( const QBrush & brush )
 */
HB_FUNC( QT_QABSTRACTGRAPHICSSHAPEITEM_SETBRUSH )
{
   QAbstractGraphicsShapeItem * p = hbqt_par_QAbstractGraphicsShapeItem( 1 );
   if( p )
   {
      ( p )->setBrush( *hbqt_par_QBrush( 2 ) );
   }
}

/*
 * void setPen ( const QPen & pen )
 */
HB_FUNC( QT_QABSTRACTGRAPHICSSHAPEITEM_SETPEN )
{
   QAbstractGraphicsShapeItem * p = hbqt_par_QAbstractGraphicsShapeItem( 1 );
   if( p )
   {
      ( p )->setPen( *hbqt_par_QPen( 2 ) );
   }
}


/*----------------------------------------------------------------------*/
#endif             /* #if QT_VERSION >= 0x040500 */
/*----------------------------------------------------------------------*/
