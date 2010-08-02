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
 *  enum EndEditHint { NoHint, EditNextItem, EditPreviousItem, SubmitModelCache, RevertModelCache }
 */

#include <QtCore/QPointer>

#include <QtGui/QItemDelegate>


/* QItemDelegate ( QObject * parent = 0 )
 * ~QItemDelegate ()
 *
 */

typedef struct
{
   QPointer< QItemDelegate > ph;
   bool bNew;
   PHBQT_GC_FUNC func;
   int type;
} HBQT_GC_T_QItemDelegate;

HBQT_GC_FUNC( hbqt_gcRelease_QItemDelegate )
{
   QItemDelegate  * ph = NULL ;
   HBQT_GC_T_QItemDelegate * p = ( HBQT_GC_T_QItemDelegate * ) Cargo;

   if( p && p->bNew && p->ph )
   {
      ph = p->ph;
      if( ph )
      {
         const QMetaObject * m = ( ph )->metaObject();
         if( ( QString ) m->className() != ( QString ) "QObject" )
         {
            HB_TRACE( HB_TR_DEBUG, ( "ph=%p %p YES_rel_QItemDelegate   /.\\   ", (void*) ph, (void*) p->ph ) );
            delete ( p->ph );
            HB_TRACE( HB_TR_DEBUG, ( "ph=%p %p YES_rel_QItemDelegate   \\./   ", (void*) ph, (void*) p->ph ) );
            p->ph = NULL;
         }
         else
         {
            HB_TRACE( HB_TR_DEBUG, ( "ph=%p NO__rel_QItemDelegate          ", ph ) );
            p->ph = NULL;
         }
      }
      else
      {
         HB_TRACE( HB_TR_DEBUG, ( "ph=%p DEL_rel_QItemDelegate    :     Object already deleted!", ph ) );
         p->ph = NULL;
      }
   }
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "ph=%p PTR_rel_QItemDelegate    :    Object not created with new=true", ph ) );
      p->ph = NULL;
   }
}

void * hbqt_gcAllocate_QItemDelegate( void * pObj, bool bNew )
{
   HBQT_GC_T_QItemDelegate * p = ( HBQT_GC_T_QItemDelegate * ) hb_gcAllocate( sizeof( HBQT_GC_T_QItemDelegate ), hbqt_gcFuncs() );

   new( & p->ph ) QPointer< QItemDelegate >( ( QItemDelegate * ) pObj );
   p->bNew = bNew;
   p->func = hbqt_gcRelease_QItemDelegate;
   p->type = HBQT_TYPE_QItemDelegate;

   if( bNew )
   {
      HB_TRACE( HB_TR_DEBUG, ( "ph=%p    _new_QItemDelegate  under p->pq", pObj ) );
   }
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "ph=%p NOT_new_QItemDelegate", pObj ) );
   }
   return p;
}

HB_FUNC( QT_QITEMDELEGATE )
{
   QItemDelegate * pObj = NULL;

   if( HB_ISPOINTER( 1 ) )
   {
      pObj = new QItemDelegate( hbqt_par_QObject( 1 ) ) ;
   }
   else
   {
      pObj = new QItemDelegate() ;
   }

   hb_retptrGC( hbqt_gcAllocate_QItemDelegate( ( void * ) pObj, true ) );
}

/*
 * bool hasClipping () const
 */
HB_FUNC( QT_QITEMDELEGATE_HASCLIPPING )
{
   QItemDelegate * p = hbqt_par_QItemDelegate( 1 );
   if( p )
      hb_retl( ( p )->hasClipping() );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QITEMDELEGATE_HASCLIPPING FP=hb_retl( ( p )->hasClipping() ); p is NULL" ) );
   }
}

/*
 * QItemEditorFactory * itemEditorFactory () const
 */
HB_FUNC( QT_QITEMDELEGATE_ITEMEDITORFACTORY )
{
   QItemDelegate * p = hbqt_par_QItemDelegate( 1 );
   if( p )
      hb_retptrGC( hbqt_gcAllocate_QItemEditorFactory( ( p )->itemEditorFactory(), false ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QITEMDELEGATE_ITEMEDITORFACTORY FP=hb_retptrGC( hbqt_gcAllocate_QItemEditorFactory( ( p )->itemEditorFactory(), false ) ); p is NULL" ) );
   }
}

/*
 * void setClipping ( bool clip )
 */
HB_FUNC( QT_QITEMDELEGATE_SETCLIPPING )
{
   QItemDelegate * p = hbqt_par_QItemDelegate( 1 );
   if( p )
      ( p )->setClipping( hb_parl( 2 ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QITEMDELEGATE_SETCLIPPING FP=( p )->setClipping( hb_parl( 2 ) ); p is NULL" ) );
   }
}

/*
 * void setItemEditorFactory ( QItemEditorFactory * factory )
 */
HB_FUNC( QT_QITEMDELEGATE_SETITEMEDITORFACTORY )
{
   QItemDelegate * p = hbqt_par_QItemDelegate( 1 );
   if( p )
      ( p )->setItemEditorFactory( hbqt_par_QItemEditorFactory( 2 ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QITEMDELEGATE_SETITEMEDITORFACTORY FP=( p )->setItemEditorFactory( hbqt_par_QItemEditorFactory( 2 ) ); p is NULL" ) );
   }
}

/*
 * virtual QWidget * createEditor ( QWidget * parent, const QStyleOptionViewItem & option, const QModelIndex & index ) const
 */
HB_FUNC( QT_QITEMDELEGATE_CREATEEDITOR )
{
   QItemDelegate * p = hbqt_par_QItemDelegate( 1 );
   if( p )
      hb_retptrGC( hbqt_gcAllocate_QWidget( ( p )->createEditor( hbqt_par_QWidget( 2 ), *hbqt_par_QStyleOptionViewItem( 3 ), *hbqt_par_QModelIndex( 4 ) ), false ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QITEMDELEGATE_CREATEEDITOR FP=hb_retptrGC( hbqt_gcAllocate_QWidget( ( p )->createEditor( hbqt_par_QWidget( 2 ), *hbqt_par_QStyleOptionViewItem( 3 ), *hbqt_par_QModelIndex( 4 ) ), false ) ); p is NULL" ) );
   }
}

/*
 * virtual void paint ( QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index ) const
 */
HB_FUNC( QT_QITEMDELEGATE_PAINT )
{
   QItemDelegate * p = hbqt_par_QItemDelegate( 1 );
   if( p )
      ( p )->paint( hbqt_par_QPainter( 2 ), *hbqt_par_QStyleOptionViewItem( 3 ), *hbqt_par_QModelIndex( 4 ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QITEMDELEGATE_PAINT FP=( p )->paint( hbqt_par_QPainter( 2 ), *hbqt_par_QStyleOptionViewItem( 3 ), *hbqt_par_QModelIndex( 4 ) ); p is NULL" ) );
   }
}

/*
 * virtual void setEditorData ( QWidget * editor, const QModelIndex & index ) const
 */
HB_FUNC( QT_QITEMDELEGATE_SETEDITORDATA )
{
   QItemDelegate * p = hbqt_par_QItemDelegate( 1 );
   if( p )
      ( p )->setEditorData( hbqt_par_QWidget( 2 ), *hbqt_par_QModelIndex( 3 ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QITEMDELEGATE_SETEDITORDATA FP=( p )->setEditorData( hbqt_par_QWidget( 2 ), *hbqt_par_QModelIndex( 3 ) ); p is NULL" ) );
   }
}

/*
 * virtual void setModelData ( QWidget * editor, QAbstractItemModel * model, const QModelIndex & index ) const
 */
HB_FUNC( QT_QITEMDELEGATE_SETMODELDATA )
{
   QItemDelegate * p = hbqt_par_QItemDelegate( 1 );
   if( p )
      ( p )->setModelData( hbqt_par_QWidget( 2 ), hbqt_par_QAbstractItemModel( 3 ), *hbqt_par_QModelIndex( 4 ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QITEMDELEGATE_SETMODELDATA FP=( p )->setModelData( hbqt_par_QWidget( 2 ), hbqt_par_QAbstractItemModel( 3 ), *hbqt_par_QModelIndex( 4 ) ); p is NULL" ) );
   }
}

/*
 * virtual QSize sizeHint ( const QStyleOptionViewItem & option, const QModelIndex & index ) const
 */
HB_FUNC( QT_QITEMDELEGATE_SIZEHINT )
{
   QItemDelegate * p = hbqt_par_QItemDelegate( 1 );
   if( p )
      hb_retptrGC( hbqt_gcAllocate_QSize( new QSize( ( p )->sizeHint( *hbqt_par_QStyleOptionViewItem( 2 ), *hbqt_par_QModelIndex( 3 ) ) ), true ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QITEMDELEGATE_SIZEHINT FP=hb_retptrGC( hbqt_gcAllocate_QSize( new QSize( ( p )->sizeHint( *hbqt_par_QStyleOptionViewItem( 2 ), *hbqt_par_QModelIndex( 3 ) ) ), true ) ); p is NULL" ) );
   }
}

/*
 * virtual void updateEditorGeometry ( QWidget * editor, const QStyleOptionViewItem & option, const QModelIndex & index ) const
 */
HB_FUNC( QT_QITEMDELEGATE_UPDATEEDITORGEOMETRY )
{
   QItemDelegate * p = hbqt_par_QItemDelegate( 1 );
   if( p )
      ( p )->updateEditorGeometry( hbqt_par_QWidget( 2 ), *hbqt_par_QStyleOptionViewItem( 3 ), *hbqt_par_QModelIndex( 4 ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QITEMDELEGATE_UPDATEEDITORGEOMETRY FP=( p )->updateEditorGeometry( hbqt_par_QWidget( 2 ), *hbqt_par_QStyleOptionViewItem( 3 ), *hbqt_par_QModelIndex( 4 ) ); p is NULL" ) );
   }
}


/*----------------------------------------------------------------------*/
#endif             /* #if QT_VERSION >= 0x040500 */
/*----------------------------------------------------------------------*/
