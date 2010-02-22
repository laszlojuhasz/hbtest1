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
/*----------------------------------------------------------------------*/

#include "../hbqt.h"

/*----------------------------------------------------------------------*/
#if QT_VERSION >= 0x040500
/*----------------------------------------------------------------------*/

#include <QtCore/QPointer>

#include <QtGui/QScrollArea>


/*
 * QScrollArea ( QWidget * parent = 0 )
 * ~QScrollArea ()
 */

typedef struct
{
   void * ph;
   bool bNew;
   QT_G_FUNC_PTR func;
   QPointer< QScrollArea > pq;
} QGC_POINTER_QScrollArea;

QT_G_FUNC( hbqt_gcRelease_QScrollArea )
{
   QGC_POINTER_QScrollArea * p = ( QGC_POINTER_QScrollArea * ) Cargo;

   if( p && p->bNew )
   {
      if( p->ph && p->pq )
      {
         const QMetaObject * m = ( ( QObject * ) p->ph )->metaObject();
         if( ( QString ) m->className() != ( QString ) "QObject" )
         {
            HB_TRACE( HB_TR_DEBUG, ( "YES_rel_QScrollArea   /.\\   ph=%p pq=%p", p->ph, (void *)(p->pq) ) );
            delete ( ( QScrollArea * ) p->ph );
            HB_TRACE( HB_TR_DEBUG, ( "YES_rel_QScrollArea   \\./   ph=%p pq=%p", p->ph, (void *)(p->pq) ) );
            p->ph = NULL;
         }
         else
         {
            HB_TRACE( HB_TR_DEBUG, ( "NO__rel_QScrollAreaph=%p pq=%p", p->ph, (void *)(p->pq) ) );
         }
      }
      else
      {
         HB_TRACE( HB_TR_DEBUG, ( "DEL_rel_QScrollArea    :     Object already deleted!" ) );
      }
   }
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "PTR_rel_QScrollArea    :    Object not created with new()" ) );
      p->ph = NULL;
   }
}

void * hbqt_gcAllocate_QScrollArea( void * pObj, bool bNew )
{
   QGC_POINTER_QScrollArea * p = ( QGC_POINTER_QScrollArea * ) hb_gcAllocate( sizeof( QGC_POINTER_QScrollArea ), hbqt_gcFuncs() );

   p->ph = pObj;
   p->bNew = bNew;
   p->func = hbqt_gcRelease_QScrollArea;

   if( bNew )
   {
      new( & p->pq ) QPointer< QScrollArea >( ( QScrollArea * ) pObj );
      HB_TRACE( HB_TR_DEBUG, ( "   _new_QScrollArea                ph=%p %i B %i KB", pObj, ( int ) hb_xquery( 1001 ), hbqt_getmemused() ) );
   }
   return p;
}

HB_FUNC( QT_QSCROLLAREA )
{
   void * pObj = NULL;

   pObj = ( QScrollArea* ) new QScrollArea( hbqt_par_QWidget( 1 ) ) ;

   hb_retptrGC( hbqt_gcAllocate_QScrollArea( pObj, true ) );
}

/*
 * Qt::Alignment alignment () const
 */
HB_FUNC( QT_QSCROLLAREA_ALIGNMENT )
{
   hb_retni( ( Qt::Alignment ) hbqt_par_QScrollArea( 1 )->alignment() );
}

/*
 * void ensureVisible ( int x, int y, int xmargin = 50, int ymargin = 50 )
 */
HB_FUNC( QT_QSCROLLAREA_ENSUREVISIBLE )
{
   hbqt_par_QScrollArea( 1 )->ensureVisible( hb_parni( 2 ), hb_parni( 3 ), ( HB_ISNUM( 4 ) ? hb_parni( 4 ) : 50 ), ( HB_ISNUM( 5 ) ? hb_parni( 5 ) : 50 ) );
}

/*
 * void ensureWidgetVisible ( QWidget * childWidget, int xmargin = 50, int ymargin = 50 )
 */
HB_FUNC( QT_QSCROLLAREA_ENSUREWIDGETVISIBLE )
{
   hbqt_par_QScrollArea( 1 )->ensureWidgetVisible( hbqt_par_QWidget( 2 ), ( HB_ISNUM( 3 ) ? hb_parni( 3 ) : 50 ), ( HB_ISNUM( 4 ) ? hb_parni( 4 ) : 50 ) );
}

/*
 * void setAlignment ( Qt::Alignment )
 */
HB_FUNC( QT_QSCROLLAREA_SETALIGNMENT )
{
   hbqt_par_QScrollArea( 1 )->setAlignment( ( Qt::Alignment ) hb_parni( 2 ) );
}

/*
 * void setWidget ( QWidget * widget )
 */
HB_FUNC( QT_QSCROLLAREA_SETWIDGET )
{
   hbqt_par_QScrollArea( 1 )->setWidget( hbqt_par_QWidget( 2 ) );
}

/*
 * void setWidgetResizable ( bool resizable )
 */
HB_FUNC( QT_QSCROLLAREA_SETWIDGETRESIZABLE )
{
   hbqt_par_QScrollArea( 1 )->setWidgetResizable( hb_parl( 2 ) );
}

/*
 * QWidget * takeWidget ()
 */
HB_FUNC( QT_QSCROLLAREA_TAKEWIDGET )
{
   hb_retptrGC( hbqt_gcAllocate_QWidget( hbqt_par_QScrollArea( 1 )->takeWidget(), false ) );
}

/*
 * QWidget * widget () const
 */
HB_FUNC( QT_QSCROLLAREA_WIDGET )
{
   hb_retptrGC( hbqt_gcAllocate_QWidget( hbqt_par_QScrollArea( 1 )->widget(), false ) );
}

/*
 * bool widgetResizable () const
 */
HB_FUNC( QT_QSCROLLAREA_WIDGETRESIZABLE )
{
   hb_retl( hbqt_par_QScrollArea( 1 )->widgetResizable() );
}


/*----------------------------------------------------------------------*/
#endif             /* #if QT_VERSION >= 0x040500 */
/*----------------------------------------------------------------------*/
