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
 * Copyright 2009 Pritpal Bedi <pritpal@vouchcac.com>
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

#include "hbapi.h"
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
  void * ph;
  QT_G_FUNC_PTR func;
  QPointer< QStyledItemDelegate > pq;
} QGC_POINTER_QStyledItemDelegate;

QT_G_FUNC( release_QStyledItemDelegate )
{
   QGC_POINTER_QStyledItemDelegate * p = ( QGC_POINTER_QStyledItemDelegate * ) Cargo;

   HB_TRACE( HB_TR_DEBUG, ( "release_QStyledItemDelegate          p=%p", p));
   HB_TRACE( HB_TR_DEBUG, ( "release_QStyledItemDelegate         ph=%p pq=%p", p->ph, (void *)(p->pq)));

   if( p && p->ph && p->pq )
   {
      const QMetaObject * m = ( ( QObject * ) p->ph )->metaObject();
      if( ( QString ) m->className() != ( QString ) "QObject" )
      {
         switch( hbqt_get_object_release_method() )
         {
         case HBQT_RELEASE_WITH_DELETE:
            delete ( ( QStyledItemDelegate * ) p->ph );
            break;
         case HBQT_RELEASE_WITH_DESTRUTOR:
            ( ( QStyledItemDelegate * ) p->ph )->~QStyledItemDelegate();
            break;
         case HBQT_RELEASE_WITH_DELETE_LATER:
            ( ( QStyledItemDelegate * ) p->ph )->deleteLater();
            break;
         }
         p->ph = NULL;
         HB_TRACE( HB_TR_DEBUG, ( "release_QStyledItemDelegate         Object deleted! %i B %i KB", ( int ) hb_xquery( 1001 ), hbqt_getmemused() ) );
      }
      else
      {
         HB_TRACE( HB_TR_DEBUG, ( "NO release_QStyledItemDelegate         Object Name Missing!" ) );
      }
   }
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "DEL release_QStyledItemDelegate         Object Allready deleted!" ) );
   }
}

void * gcAllocate_QStyledItemDelegate( void * pObj )
{
   QGC_POINTER_QStyledItemDelegate * p = ( QGC_POINTER_QStyledItemDelegate * ) hb_gcAllocate( sizeof( QGC_POINTER_QStyledItemDelegate ), gcFuncs() );

   p->ph = pObj;
   p->func = release_QStyledItemDelegate;
   new( & p->pq ) QPointer< QStyledItemDelegate >( ( QStyledItemDelegate * ) pObj );
   HB_TRACE( HB_TR_DEBUG, ( "          new_QStyledItemDelegate         %i B %i KB", ( int ) hb_xquery( 1001 ), hbqt_getmemused() ) );
   return( p );
}

HB_FUNC( QT_QSTYLEDITEMDELEGATE )
{
   void * pObj = NULL;

   pObj = ( QStyledItemDelegate* ) new QStyledItemDelegate( hbqt_par_QObject( 1 ) ) ;

   hb_retptrGC( gcAllocate_QStyledItemDelegate( pObj ) );
}
/*
 * virtual QWidget * createEditor ( QWidget * parent, const QStyleOptionViewItem & option, const QModelIndex & index ) const
 */
HB_FUNC( QT_QSTYLEDITEMDELEGATE_CREATEEDITOR )
{
   hb_retptr( ( QWidget* ) hbqt_par_QStyledItemDelegate( 1 )->createEditor( hbqt_par_QWidget( 2 ), *hbqt_par_QStyleOptionViewItem( 3 ), *hbqt_par_QModelIndex( 4 ) ) );
}

/*
 * virtual QString displayText ( const QVariant & value, const QLocale & locale ) const
 */
HB_FUNC( QT_QSTYLEDITEMDELEGATE_DISPLAYTEXT )
{
   hb_retc( hbqt_par_QStyledItemDelegate( 1 )->displayText( *hbqt_par_QVariant( 2 ), *hbqt_par_QLocale( 3 ) ).toAscii().data() );
}

/*
 * virtual void paint ( QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index ) const
 */
HB_FUNC( QT_QSTYLEDITEMDELEGATE_PAINT )
{
   hbqt_par_QStyledItemDelegate( 1 )->paint( hbqt_par_QPainter( 2 ), *hbqt_par_QStyleOptionViewItem( 3 ), *hbqt_par_QModelIndex( 4 ) );
}

/*
 * virtual void setEditorData ( QWidget * editor, const QModelIndex & index ) const
 */
HB_FUNC( QT_QSTYLEDITEMDELEGATE_SETEDITORDATA )
{
   hbqt_par_QStyledItemDelegate( 1 )->setEditorData( hbqt_par_QWidget( 2 ), *hbqt_par_QModelIndex( 3 ) );
}

/*
 * virtual void setModelData ( QWidget * editor, QAbstractItemModel * model, const QModelIndex & index ) const
 */
HB_FUNC( QT_QSTYLEDITEMDELEGATE_SETMODELDATA )
{
   hbqt_par_QStyledItemDelegate( 1 )->setModelData( hbqt_par_QWidget( 2 ), hbqt_par_QAbstractItemModel( 3 ), *hbqt_par_QModelIndex( 4 ) );
}

/*
 * virtual QSize sizeHint ( const QStyleOptionViewItem & option, const QModelIndex & index ) const
 */
HB_FUNC( QT_QSTYLEDITEMDELEGATE_SIZEHINT )
{
   hb_retptrGC( gcAllocate_QSize( new QSize( hbqt_par_QStyledItemDelegate( 1 )->sizeHint( *hbqt_par_QStyleOptionViewItem( 2 ), *hbqt_par_QModelIndex( 3 ) ) ) ) );
}

/*
 * virtual void updateEditorGeometry ( QWidget * editor, const QStyleOptionViewItem & option, const QModelIndex & index ) const
 */
HB_FUNC( QT_QSTYLEDITEMDELEGATE_UPDATEEDITORGEOMETRY )
{
   hbqt_par_QStyledItemDelegate( 1 )->updateEditorGeometry( hbqt_par_QWidget( 2 ), *hbqt_par_QStyleOptionViewItem( 3 ), *hbqt_par_QModelIndex( 4 ) );
}


/*----------------------------------------------------------------------*/
#endif             /* #if QT_VERSION >= 0x040500 */
/*----------------------------------------------------------------------*/
