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
 *  Constructed[ 11/12 [ 91.67% ] ]
 *
 *  *** Unconvered Prototypes ***
 *  -----------------------------
 *
 *  }
 */

#include <QtCore/QPointer>

#include <QtGui/QDrag>
#include <QtGui/QPixmap>
#include <QtCore/QPoint>


/*
 * QDrag ( QWidget * dragSource )
 * ~QDrag ()
 */

typedef struct
{
   QPointer< QDrag > ph;
   bool bNew;
   PHBQT_GC_FUNC func;
   int type;
} HBQT_GC_T_QDrag;

HBQT_GC_FUNC( hbqt_gcRelease_QDrag )
{
   QDrag  * ph = NULL ;
   HBQT_GC_T_QDrag * p = ( HBQT_GC_T_QDrag * ) Cargo;

   if( p && p->bNew && p->ph )
   {
      ph = p->ph;
      if( ph )
      {
         const QMetaObject * m = ( ph )->metaObject();
         if( ( QString ) m->className() != ( QString ) "QObject" )
         {
            HB_TRACE( HB_TR_DEBUG, ( "ph=%p %p YES_rel_QDrag   /.\\   ", (void*) ph, (void*) p->ph ) );
            delete ( p->ph );
            HB_TRACE( HB_TR_DEBUG, ( "ph=%p %p YES_rel_QDrag   \\./   ", (void*) ph, (void*) p->ph ) );
            p->ph = NULL;
         }
         else
         {
            HB_TRACE( HB_TR_DEBUG, ( "ph=%p NO__rel_QDrag          ", ph ) );
            p->ph = NULL;
         }
      }
      else
      {
         HB_TRACE( HB_TR_DEBUG, ( "ph=%p DEL_rel_QDrag    :     Object already deleted!", ph ) );
         p->ph = NULL;
      }
   }
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "ph=%p PTR_rel_QDrag    :    Object not created with new=true", ph ) );
      p->ph = NULL;
   }
}

void * hbqt_gcAllocate_QDrag( void * pObj, bool bNew )
{
   HBQT_GC_T_QDrag * p = ( HBQT_GC_T_QDrag * ) hb_gcAllocate( sizeof( HBQT_GC_T_QDrag ), hbqt_gcFuncs() );

   new( & p->ph ) QPointer< QDrag >( ( QDrag * ) pObj );
   p->bNew = bNew;
   p->func = hbqt_gcRelease_QDrag;
   p->type = HBQT_TYPE_QDrag;

   if( bNew )
   {
      HB_TRACE( HB_TR_DEBUG, ( "ph=%p    _new_QDrag  under p->pq", pObj ) );
   }
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "ph=%p NOT_new_QDrag", pObj ) );
   }
   return p;
}

HB_FUNC( QT_QDRAG )
{
   QDrag * pObj = NULL;

   pObj = new QDrag( hbqt_par_QWidget( 1 ) ) ;

   hb_retptrGC( hbqt_gcAllocate_QDrag( ( void * ) pObj, true ) );
}

/*
 * Qt::DropAction exec ( Qt::DropActions supportedActions = Qt::MoveAction )
 */
HB_FUNC( QT_QDRAG_EXEC )
{
   QDrag * p = hbqt_par_QDrag( 1 );
   if( p )
      hb_retni( ( Qt::DropAction ) ( p )->exec( ( HB_ISNUM( 2 ) ? ( Qt::DropActions ) hb_parni( 2 ) : ( Qt::DropActions ) Qt::MoveAction ) ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QDRAG_EXEC FP=hb_retni( ( Qt::DropAction ) ( p )->exec( ( HB_ISNUM( 2 ) ? ( Qt::DropActions ) hb_parni( 2 ) : ( Qt::DropActions ) Qt::MoveAction ) ) ); p is NULL" ) );
   }
}

/*
 * Qt::DropAction exec ( Qt::DropActions supportedActions, Qt::DropAction defaultDropAction )
 */
HB_FUNC( QT_QDRAG_EXEC_1 )
{
   QDrag * p = hbqt_par_QDrag( 1 );
   if( p )
      hb_retni( ( Qt::DropAction ) ( p )->exec( ( Qt::DropActions ) hb_parni( 2 ), ( Qt::DropAction ) hb_parni( 3 ) ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QDRAG_EXEC_1 FP=hb_retni( ( Qt::DropAction ) ( p )->exec( ( Qt::DropActions ) hb_parni( 2 ), ( Qt::DropAction ) hb_parni( 3 ) ) ); p is NULL" ) );
   }
}

/*
 * QPoint hotSpot () const
 */
HB_FUNC( QT_QDRAG_HOTSPOT )
{
   QDrag * p = hbqt_par_QDrag( 1 );
   if( p )
      hb_retptrGC( hbqt_gcAllocate_QPoint( new QPoint( ( p )->hotSpot() ), true ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QDRAG_HOTSPOT FP=hb_retptrGC( hbqt_gcAllocate_QPoint( new QPoint( ( p )->hotSpot() ), true ) ); p is NULL" ) );
   }
}

/*
 * QMimeData * mimeData () const
 */
HB_FUNC( QT_QDRAG_MIMEDATA )
{
   QDrag * p = hbqt_par_QDrag( 1 );
   if( p )
      hb_retptrGC( hbqt_gcAllocate_QMimeData( ( p )->mimeData(), false ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QDRAG_MIMEDATA FP=hb_retptrGC( hbqt_gcAllocate_QMimeData( ( p )->mimeData(), false ) ); p is NULL" ) );
   }
}

/*
 * QPixmap pixmap () const
 */
HB_FUNC( QT_QDRAG_PIXMAP )
{
   QDrag * p = hbqt_par_QDrag( 1 );
   if( p )
      hb_retptrGC( hbqt_gcAllocate_QPixmap( new QPixmap( ( p )->pixmap() ), true ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QDRAG_PIXMAP FP=hb_retptrGC( hbqt_gcAllocate_QPixmap( new QPixmap( ( p )->pixmap() ), true ) ); p is NULL" ) );
   }
}

/*
 * void setDragCursor ( const QPixmap & cursor, Qt::DropAction action )
 */
HB_FUNC( QT_QDRAG_SETDRAGCURSOR )
{
   QDrag * p = hbqt_par_QDrag( 1 );
   if( p )
      ( p )->setDragCursor( *hbqt_par_QPixmap( 2 ), ( Qt::DropAction ) hb_parni( 3 ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QDRAG_SETDRAGCURSOR FP=( p )->setDragCursor( *hbqt_par_QPixmap( 2 ), ( Qt::DropAction ) hb_parni( 3 ) ); p is NULL" ) );
   }
}

/*
 * void setHotSpot ( const QPoint & hotspot )
 */
HB_FUNC( QT_QDRAG_SETHOTSPOT )
{
   QDrag * p = hbqt_par_QDrag( 1 );
   if( p )
      ( p )->setHotSpot( *hbqt_par_QPoint( 2 ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QDRAG_SETHOTSPOT FP=( p )->setHotSpot( *hbqt_par_QPoint( 2 ) ); p is NULL" ) );
   }
}

/*
 * void setMimeData ( QMimeData * data )
 */
HB_FUNC( QT_QDRAG_SETMIMEDATA )
{
   HBQT_GC_T * p = ( HBQT_GC_T * ) hb_parptrGC( hbqt_gcFuncs(), 1 );
   HBQT_GC_T * q = ( HBQT_GC_T * ) hb_parptrGC( hbqt_gcFuncs(), 2 );
   HB_TRACE( HB_TR_DEBUG, ( "Entering function QT_QDRAG_SETMIMEDATA()" ) );
   if( p && p->ph && q && q->ph )
   {
      HB_TRACE( HB_TR_DEBUG, ( "QT_QDRAG_SETMIMEDATA() Qt object: %p is attached to: %p", p->ph, q->ph ) );
      q->bNew = HB_FALSE;
      hbqt_par_QDrag( 1 )->setMimeData( hbqt_par_QMimeData( 2 ) );
   }
}

/*
 * void setPixmap ( const QPixmap & pixmap )
 */
HB_FUNC( QT_QDRAG_SETPIXMAP )
{
   QDrag * p = hbqt_par_QDrag( 1 );
   if( p )
      ( p )->setPixmap( *hbqt_par_QPixmap( 2 ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QDRAG_SETPIXMAP FP=( p )->setPixmap( *hbqt_par_QPixmap( 2 ) ); p is NULL" ) );
   }
}

/*
 * QWidget * source () const
 */
HB_FUNC( QT_QDRAG_SOURCE )
{
   QDrag * p = hbqt_par_QDrag( 1 );
   if( p )
      hb_retptrGC( hbqt_gcAllocate_QWidget( ( p )->source(), false ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QDRAG_SOURCE FP=hb_retptrGC( hbqt_gcAllocate_QWidget( ( p )->source(), false ) ); p is NULL" ) );
   }
}

/*
 * QWidget * target () const
 */
HB_FUNC( QT_QDRAG_TARGET )
{
   QDrag * p = hbqt_par_QDrag( 1 );
   if( p )
      hb_retptrGC( hbqt_gcAllocate_QWidget( ( p )->target(), false ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QDRAG_TARGET FP=hb_retptrGC( hbqt_gcAllocate_QWidget( ( p )->target(), false ) ); p is NULL" ) );
   }
}


/*----------------------------------------------------------------------*/
#endif             /* #if QT_VERSION >= 0x040500 */
/*----------------------------------------------------------------------*/
