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

#include "../hbqt.h"

/*----------------------------------------------------------------------*/
#if QT_VERSION >= 0x040500
/*----------------------------------------------------------------------*/

#include <QtCore/QPointer>

#include <QtGui/QStyledItemDelegate>


/*
 * QStyledItemDelegate ( QObject * parent = 0 )
 * ~QStyledItemDelegate ()
 */

typedef struct
{
   QPointer< QStyledItemDelegate > ph;
   bool bNew;
   QT_G_FUNC_PTR func;
   int type;
} QGC_POINTER_QStyledItemDelegate;

QT_G_FUNC( hbqt_gcRelease_QStyledItemDelegate )
{
   QStyledItemDelegate  * ph = NULL ;
   QGC_POINTER_QStyledItemDelegate * p = ( QGC_POINTER_QStyledItemDelegate * ) Cargo;

   if( p && p->bNew && p->ph )
   {
      ph = p->ph;
      if( ph )
      {
         const QMetaObject * m = ( ph )->metaObject();
         if( ( QString ) m->className() != ( QString ) "QObject" )
         {
            HB_TRACE( HB_TR_DEBUG, ( "ph=%p %p YES_rel_QStyledItemDelegate   /.\\   ", (void*) ph, (void*) p->ph ) );
            delete ( p->ph );
            HB_TRACE( HB_TR_DEBUG, ( "ph=%p %p YES_rel_QStyledItemDelegate   \\./   ", (void*) ph, (void*) p->ph ) );
            p->ph = NULL;
         }
         else
         {
            HB_TRACE( HB_TR_DEBUG, ( "ph=%p NO__rel_QStyledItemDelegate          ", ph ) );
            p->ph = NULL;
         }
      }
      else
      {
         HB_TRACE( HB_TR_DEBUG, ( "ph=%p DEL_rel_QStyledItemDelegate    :     Object already deleted!", ph ) );
         p->ph = NULL;
      }
   }
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "ph=%p PTR_rel_QStyledItemDelegate    :    Object not created with new=true", ph ) );
      p->ph = NULL;
   }
}

void * hbqt_gcAllocate_QStyledItemDelegate( void * pObj, bool bNew )
{
   QGC_POINTER_QStyledItemDelegate * p = ( QGC_POINTER_QStyledItemDelegate * ) hb_gcAllocate( sizeof( QGC_POINTER_QStyledItemDelegate ), hbqt_gcFuncs() );

   new( & p->ph ) QPointer< QStyledItemDelegate >( ( QStyledItemDelegate * ) pObj );
   p->bNew = bNew;
   p->func = hbqt_gcRelease_QStyledItemDelegate;
   p->type = HBQT_TYPE_QStyledItemDelegate;

   if( bNew )
   {
      HB_TRACE( HB_TR_DEBUG, ( "ph=%p    _new_QStyledItemDelegate  under p->pq", pObj ) );
   }
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "ph=%p NOT_new_QStyledItemDelegate", pObj ) );
   }
   return p;
}

HB_FUNC( QT_QSTYLEDITEMDELEGATE )
{
   QStyledItemDelegate * pObj = NULL;

   pObj =  new QStyledItemDelegate( hbqt_par_QObject( 1 ) ) ;

   hb_retptrGC( hbqt_gcAllocate_QStyledItemDelegate( ( void * ) pObj, true ) );
}

/*
 * virtual QWidget * createEditor ( QWidget * parent, const QStyleOptionViewItem & option, const QModelIndex & index ) const
 */
HB_FUNC( QT_QSTYLEDITEMDELEGATE_CREATEEDITOR )
{
   QStyledItemDelegate * p = hbqt_par_QStyledItemDelegate( 1 );
   if( p )
      hb_retptrGC( hbqt_gcAllocate_QWidget( ( p )->createEditor( hbqt_par_QWidget( 2 ), *hbqt_par_QStyleOptionViewItem( 3 ), *hbqt_par_QModelIndex( 4 ) ), false ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QSTYLEDITEMDELEGATE_CREATEEDITOR FP=hb_retptrGC( hbqt_gcAllocate_QWidget( ( p )->createEditor( hbqt_par_QWidget( 2 ), *hbqt_par_QStyleOptionViewItem( 3 ), *hbqt_par_QModelIndex( 4 ) ), false ) ); p is NULL" ) );
   }
}

/*
 * virtual QString displayText ( const QVariant & value, const QLocale & locale ) const
 */
HB_FUNC( QT_QSTYLEDITEMDELEGATE_DISPLAYTEXT )
{
   QStyledItemDelegate * p = hbqt_par_QStyledItemDelegate( 1 );
   if( p )
      hb_retc( ( p )->displayText( *hbqt_par_QVariant( 2 ), *hbqt_par_QLocale( 3 ) ).toAscii().data() );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QSTYLEDITEMDELEGATE_DISPLAYTEXT FP=hb_retc( ( p )->displayText( *hbqt_par_QVariant( 2 ), *hbqt_par_QLocale( 3 ) ).toAscii().data() ); p is NULL" ) );
   }
}

/*
 * virtual void paint ( QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index ) const
 */
HB_FUNC( QT_QSTYLEDITEMDELEGATE_PAINT )
{
   QStyledItemDelegate * p = hbqt_par_QStyledItemDelegate( 1 );
   if( p )
      ( p )->paint( hbqt_par_QPainter( 2 ), *hbqt_par_QStyleOptionViewItem( 3 ), *hbqt_par_QModelIndex( 4 ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QSTYLEDITEMDELEGATE_PAINT FP=( p )->paint( hbqt_par_QPainter( 2 ), *hbqt_par_QStyleOptionViewItem( 3 ), *hbqt_par_QModelIndex( 4 ) ); p is NULL" ) );
   }
}

/*
 * virtual void setEditorData ( QWidget * editor, const QModelIndex & index ) const
 */
HB_FUNC( QT_QSTYLEDITEMDELEGATE_SETEDITORDATA )
{
   QStyledItemDelegate * p = hbqt_par_QStyledItemDelegate( 1 );
   if( p )
      ( p )->setEditorData( hbqt_par_QWidget( 2 ), *hbqt_par_QModelIndex( 3 ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QSTYLEDITEMDELEGATE_SETEDITORDATA FP=( p )->setEditorData( hbqt_par_QWidget( 2 ), *hbqt_par_QModelIndex( 3 ) ); p is NULL" ) );
   }
}

/*
 * virtual void setModelData ( QWidget * editor, QAbstractItemModel * model, const QModelIndex & index ) const
 */
HB_FUNC( QT_QSTYLEDITEMDELEGATE_SETMODELDATA )
{
   QStyledItemDelegate * p = hbqt_par_QStyledItemDelegate( 1 );
   if( p )
      ( p )->setModelData( hbqt_par_QWidget( 2 ), hbqt_par_QAbstractItemModel( 3 ), *hbqt_par_QModelIndex( 4 ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QSTYLEDITEMDELEGATE_SETMODELDATA FP=( p )->setModelData( hbqt_par_QWidget( 2 ), hbqt_par_QAbstractItemModel( 3 ), *hbqt_par_QModelIndex( 4 ) ); p is NULL" ) );
   }
}

/*
 * virtual QSize sizeHint ( const QStyleOptionViewItem & option, const QModelIndex & index ) const
 */
HB_FUNC( QT_QSTYLEDITEMDELEGATE_SIZEHINT )
{
   QStyledItemDelegate * p = hbqt_par_QStyledItemDelegate( 1 );
   if( p )
      hb_retptrGC( hbqt_gcAllocate_QSize( new QSize( ( p )->sizeHint( *hbqt_par_QStyleOptionViewItem( 2 ), *hbqt_par_QModelIndex( 3 ) ) ), true ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QSTYLEDITEMDELEGATE_SIZEHINT FP=hb_retptrGC( hbqt_gcAllocate_QSize( new QSize( ( p )->sizeHint( *hbqt_par_QStyleOptionViewItem( 2 ), *hbqt_par_QModelIndex( 3 ) ) ), true ) ); p is NULL" ) );
   }
}

/*
 * virtual void updateEditorGeometry ( QWidget * editor, const QStyleOptionViewItem & option, const QModelIndex & index ) const
 */
HB_FUNC( QT_QSTYLEDITEMDELEGATE_UPDATEEDITORGEOMETRY )
{
   QStyledItemDelegate * p = hbqt_par_QStyledItemDelegate( 1 );
   if( p )
      ( p )->updateEditorGeometry( hbqt_par_QWidget( 2 ), *hbqt_par_QStyleOptionViewItem( 3 ), *hbqt_par_QModelIndex( 4 ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QSTYLEDITEMDELEGATE_UPDATEEDITORGEOMETRY FP=( p )->updateEditorGeometry( hbqt_par_QWidget( 2 ), *hbqt_par_QStyleOptionViewItem( 3 ), *hbqt_par_QModelIndex( 4 ) ); p is NULL" ) );
   }
}


/*----------------------------------------------------------------------*/
#endif             /* #if QT_VERSION >= 0x040500 */
/*----------------------------------------------------------------------*/
