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

/*
 *  Constructed[ 32/36 [ 88.89% ] ]
 *
 *  *** Unconvered Prototypes ***
 *  -----------------------------
 *
 *  virtual QMap<int, QVariant> itemData ( const QModelIndex & index ) const
 *  virtual bool setItemData ( const QModelIndex & index, const QMap<int, QVariant> & roles )
 *
 *  *** Commented out protos which construct fine but do not compile ***
 *
 *  // virtual QModelIndexList match ( const QModelIndex & start, int role, const QVariant & value, int hits = 1, Qt::MatchFlags flags = Qt::MatchFlags( Qt::MatchStartsWith | Qt::MatchWrap ) ) const
 *  // virtual QMimeData * mimeData ( const QModelIndexList & indexes ) const
 */

#include <QtCore/QPointer>

#include <QSize>
#include <QStringList>
#include <QtCore/QAbstractItemModel>


/*
 * QAbstractItemModel ( QObject * parent = 0 )
 * virtual ~QAbstractItemModel ()
 */



typedef struct
{
   QPointer< QAbstractItemModel > ph;
   bool bNew;
   QT_G_FUNC_PTR func;
} QGC_POINTER_QAbstractItemModel;

QT_G_FUNC( hbqt_gcRelease_QAbstractItemModel )
{
   HB_SYMBOL_UNUSED( Cargo );
   QGC_POINTER * p = ( QGC_POINTER * ) Cargo;

   if( p && p->bNew )
   {
      p->ph = NULL;
   }
}

void * hbqt_gcAllocate_QAbstractItemModel( void * pObj, bool bNew )
{
   QGC_POINTER_QAbstractItemModel * p = ( QGC_POINTER_QAbstractItemModel * ) hb_gcAllocate( sizeof( QGC_POINTER_QAbstractItemModel ), hbqt_gcFuncs() );

   new( & p->ph ) QPointer< QAbstractItemModel >( ( QAbstractItemModel * ) pObj );
   p->bNew = bNew;
   p->func = hbqt_gcRelease_QAbstractItemModel;

   if( bNew )
   {
      HB_TRACE( HB_TR_DEBUG, ( "ph=%p    _new_QAbstractItemModel  under p->pq", pObj ) );
   }
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "ph=%p NOT_new_QAbstractItemModel", pObj ) );
   }
   return p;
}

HB_FUNC( QT_QABSTRACTITEMMODEL )
{
}

/*
 * virtual QModelIndex buddy ( const QModelIndex & index ) const
 */
HB_FUNC( QT_QABSTRACTITEMMODEL_BUDDY )
{
   QAbstractItemModel * p = hbqt_par_QAbstractItemModel( 1 );
   if( p )
      hb_retptrGC( hbqt_gcAllocate_QModelIndex( new QModelIndex( ( p )->buddy( *hbqt_par_QModelIndex( 2 ) ) ), true ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QABSTRACTITEMMODEL_BUDDY FP=hb_retptrGC( hbqt_gcAllocate_QModelIndex( new QModelIndex( ( p )->buddy( *hbqt_par_QModelIndex( 2 ) ) ), true ) ); p is NULL" ) );
   }
}

/*
 * virtual bool canFetchMore ( const QModelIndex & parent ) const
 */
HB_FUNC( QT_QABSTRACTITEMMODEL_CANFETCHMORE )
{
   QAbstractItemModel * p = hbqt_par_QAbstractItemModel( 1 );
   if( p )
      hb_retl( ( p )->canFetchMore( *hbqt_par_QModelIndex( 2 ) ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QABSTRACTITEMMODEL_CANFETCHMORE FP=hb_retl( ( p )->canFetchMore( *hbqt_par_QModelIndex( 2 ) ) ); p is NULL" ) );
   }
}

/*
 * virtual int columnCount ( const QModelIndex & parent = QModelIndex() ) const = 0
 */
HB_FUNC( QT_QABSTRACTITEMMODEL_COLUMNCOUNT )
{
   QAbstractItemModel * p = hbqt_par_QAbstractItemModel( 1 );
   if( p )
      hb_retni( ( p )->columnCount( ( HB_ISPOINTER( 2 ) ? *hbqt_par_QModelIndex( 2 ) : QModelIndex() ) ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QABSTRACTITEMMODEL_COLUMNCOUNT FP=hb_retni( ( p )->columnCount( ( HB_ISPOINTER( 2 ) ? *hbqt_par_QModelIndex( 2 ) : QModelIndex() ) ) ); p is NULL" ) );
   }
}

/*
 * virtual QVariant data ( const QModelIndex & index, int role = Qt::DisplayRole ) const = 0
 */
HB_FUNC( QT_QABSTRACTITEMMODEL_DATA )
{
   QAbstractItemModel * p = hbqt_par_QAbstractItemModel( 1 );
   if( p )
      hb_retptrGC( hbqt_gcAllocate_QVariant( new QVariant( ( p )->data( *hbqt_par_QModelIndex( 2 ), ( HB_ISNUM( 3 ) ? hb_parni( 3 ) : Qt::DisplayRole ) ) ), true ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QABSTRACTITEMMODEL_DATA FP=hb_retptrGC( hbqt_gcAllocate_QVariant( new QVariant( ( p )->data( *hbqt_par_QModelIndex( 2 ), ( HB_ISNUM( 3 ) ? hb_parni( 3 ) : Qt::DisplayRole ) ) ), true ) ); p is NULL" ) );
   }
}

/*
 * virtual bool dropMimeData ( const QMimeData * data, Qt::DropAction action, int row, int column, const QModelIndex & parent )
 */
HB_FUNC( QT_QABSTRACTITEMMODEL_DROPMIMEDATA )
{
   QAbstractItemModel * p = hbqt_par_QAbstractItemModel( 1 );
   if( p )
      hb_retl( ( p )->dropMimeData( hbqt_par_QMimeData( 2 ), ( Qt::DropAction ) hb_parni( 3 ), hb_parni( 4 ), hb_parni( 5 ), *hbqt_par_QModelIndex( 6 ) ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QABSTRACTITEMMODEL_DROPMIMEDATA FP=hb_retl( ( p )->dropMimeData( hbqt_par_QMimeData( 2 ), ( Qt::DropAction ) hb_parni( 3 ), hb_parni( 4 ), hb_parni( 5 ), *hbqt_par_QModelIndex( 6 ) ) ); p is NULL" ) );
   }
}

/*
 * virtual void fetchMore ( const QModelIndex & parent )
 */
HB_FUNC( QT_QABSTRACTITEMMODEL_FETCHMORE )
{
   QAbstractItemModel * p = hbqt_par_QAbstractItemModel( 1 );
   if( p )
      ( p )->fetchMore( *hbqt_par_QModelIndex( 2 ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QABSTRACTITEMMODEL_FETCHMORE FP=( p )->fetchMore( *hbqt_par_QModelIndex( 2 ) ); p is NULL" ) );
   }
}

/*
 * virtual Qt::ItemFlags flags ( const QModelIndex & index ) const
 */
HB_FUNC( QT_QABSTRACTITEMMODEL_FLAGS )
{
   QAbstractItemModel * p = hbqt_par_QAbstractItemModel( 1 );
   if( p )
      hb_retni( ( Qt::ItemFlags ) ( p )->flags( *hbqt_par_QModelIndex( 2 ) ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QABSTRACTITEMMODEL_FLAGS FP=hb_retni( ( Qt::ItemFlags ) ( p )->flags( *hbqt_par_QModelIndex( 2 ) ) ); p is NULL" ) );
   }
}

/*
 * virtual bool hasChildren ( const QModelIndex & parent = QModelIndex() ) const
 */
HB_FUNC( QT_QABSTRACTITEMMODEL_HASCHILDREN )
{
   QAbstractItemModel * p = hbqt_par_QAbstractItemModel( 1 );
   if( p )
      hb_retl( ( p )->hasChildren( ( HB_ISPOINTER( 2 ) ? *hbqt_par_QModelIndex( 2 ) : QModelIndex() ) ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QABSTRACTITEMMODEL_HASCHILDREN FP=hb_retl( ( p )->hasChildren( ( HB_ISPOINTER( 2 ) ? *hbqt_par_QModelIndex( 2 ) : QModelIndex() ) ) ); p is NULL" ) );
   }
}

/*
 * bool hasIndex ( int row, int column, const QModelIndex & parent = QModelIndex() ) const
 */
HB_FUNC( QT_QABSTRACTITEMMODEL_HASINDEX )
{
   QAbstractItemModel * p = hbqt_par_QAbstractItemModel( 1 );
   if( p )
      hb_retl( ( p )->hasIndex( hb_parni( 2 ), hb_parni( 3 ), ( HB_ISPOINTER( 4 ) ? *hbqt_par_QModelIndex( 4 ) : QModelIndex() ) ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QABSTRACTITEMMODEL_HASINDEX FP=hb_retl( ( p )->hasIndex( hb_parni( 2 ), hb_parni( 3 ), ( HB_ISPOINTER( 4 ) ? *hbqt_par_QModelIndex( 4 ) : QModelIndex() ) ) ); p is NULL" ) );
   }
}

/*
 * virtual QVariant headerData ( int section, Qt::Orientation orientation, int role = Qt::DisplayRole ) const
 */
HB_FUNC( QT_QABSTRACTITEMMODEL_HEADERDATA )
{
   QAbstractItemModel * p = hbqt_par_QAbstractItemModel( 1 );
   if( p )
      hb_retptrGC( hbqt_gcAllocate_QVariant( new QVariant( ( p )->headerData( hb_parni( 2 ), ( Qt::Orientation ) hb_parni( 3 ), ( HB_ISNUM( 4 ) ? hb_parni( 4 ) : Qt::DisplayRole ) ) ), true ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QABSTRACTITEMMODEL_HEADERDATA FP=hb_retptrGC( hbqt_gcAllocate_QVariant( new QVariant( ( p )->headerData( hb_parni( 2 ), ( Qt::Orientation ) hb_parni( 3 ), ( HB_ISNUM( 4 ) ? hb_parni( 4 ) : Qt::DisplayRole ) ) ), true ) ); p is NULL" ) );
   }
}

/*
 * virtual QModelIndex index ( int row, int column, const QModelIndex & parent = QModelIndex() ) const = 0
 */
HB_FUNC( QT_QABSTRACTITEMMODEL_INDEX )
{
   QAbstractItemModel * p = hbqt_par_QAbstractItemModel( 1 );
   if( p )
      hb_retptrGC( hbqt_gcAllocate_QModelIndex( new QModelIndex( ( p )->index( hb_parni( 2 ), hb_parni( 3 ), ( HB_ISPOINTER( 4 ) ? *hbqt_par_QModelIndex( 4 ) : QModelIndex() ) ) ), true ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QABSTRACTITEMMODEL_INDEX FP=hb_retptrGC( hbqt_gcAllocate_QModelIndex( new QModelIndex( ( p )->index( hb_parni( 2 ), hb_parni( 3 ), ( HB_ISPOINTER( 4 ) ? *hbqt_par_QModelIndex( 4 ) : QModelIndex() ) ) ), true ) ); p is NULL" ) );
   }
}

/*
 * bool insertColumn ( int column, const QModelIndex & parent = QModelIndex() )
 */
HB_FUNC( QT_QABSTRACTITEMMODEL_INSERTCOLUMN )
{
   QAbstractItemModel * p = hbqt_par_QAbstractItemModel( 1 );
   if( p )
      hb_retl( ( p )->insertColumn( hb_parni( 2 ), ( HB_ISPOINTER( 3 ) ? *hbqt_par_QModelIndex( 3 ) : QModelIndex() ) ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QABSTRACTITEMMODEL_INSERTCOLUMN FP=hb_retl( ( p )->insertColumn( hb_parni( 2 ), ( HB_ISPOINTER( 3 ) ? *hbqt_par_QModelIndex( 3 ) : QModelIndex() ) ) ); p is NULL" ) );
   }
}

/*
 * virtual bool insertColumns ( int column, int count, const QModelIndex & parent = QModelIndex() )
 */
HB_FUNC( QT_QABSTRACTITEMMODEL_INSERTCOLUMNS )
{
   QAbstractItemModel * p = hbqt_par_QAbstractItemModel( 1 );
   if( p )
      hb_retl( ( p )->insertColumns( hb_parni( 2 ), hb_parni( 3 ), ( HB_ISPOINTER( 4 ) ? *hbqt_par_QModelIndex( 4 ) : QModelIndex() ) ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QABSTRACTITEMMODEL_INSERTCOLUMNS FP=hb_retl( ( p )->insertColumns( hb_parni( 2 ), hb_parni( 3 ), ( HB_ISPOINTER( 4 ) ? *hbqt_par_QModelIndex( 4 ) : QModelIndex() ) ) ); p is NULL" ) );
   }
}

/*
 * bool insertRow ( int row, const QModelIndex & parent = QModelIndex() )
 */
HB_FUNC( QT_QABSTRACTITEMMODEL_INSERTROW )
{
   QAbstractItemModel * p = hbqt_par_QAbstractItemModel( 1 );
   if( p )
      hb_retl( ( p )->insertRow( hb_parni( 2 ), ( HB_ISPOINTER( 3 ) ? *hbqt_par_QModelIndex( 3 ) : QModelIndex() ) ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QABSTRACTITEMMODEL_INSERTROW FP=hb_retl( ( p )->insertRow( hb_parni( 2 ), ( HB_ISPOINTER( 3 ) ? *hbqt_par_QModelIndex( 3 ) : QModelIndex() ) ) ); p is NULL" ) );
   }
}

/*
 * virtual bool insertRows ( int row, int count, const QModelIndex & parent = QModelIndex() )
 */
HB_FUNC( QT_QABSTRACTITEMMODEL_INSERTROWS )
{
   QAbstractItemModel * p = hbqt_par_QAbstractItemModel( 1 );
   if( p )
      hb_retl( ( p )->insertRows( hb_parni( 2 ), hb_parni( 3 ), ( HB_ISPOINTER( 4 ) ? *hbqt_par_QModelIndex( 4 ) : QModelIndex() ) ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QABSTRACTITEMMODEL_INSERTROWS FP=hb_retl( ( p )->insertRows( hb_parni( 2 ), hb_parni( 3 ), ( HB_ISPOINTER( 4 ) ? *hbqt_par_QModelIndex( 4 ) : QModelIndex() ) ) ); p is NULL" ) );
   }
}

/*
 * virtual QStringList mimeTypes () const
 */
HB_FUNC( QT_QABSTRACTITEMMODEL_MIMETYPES )
{
   QAbstractItemModel * p = hbqt_par_QAbstractItemModel( 1 );
   if( p )
      hb_retptrGC( hbqt_gcAllocate_QStringList( new QStringList( ( p )->mimeTypes() ), true ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QABSTRACTITEMMODEL_MIMETYPES FP=hb_retptrGC( hbqt_gcAllocate_QStringList( new QStringList( ( p )->mimeTypes() ), true ) ); p is NULL" ) );
   }
}

/*
 * virtual QModelIndex parent ( const QModelIndex & index ) const = 0
 */
HB_FUNC( QT_QABSTRACTITEMMODEL_PARENT )
{
   QAbstractItemModel * p = hbqt_par_QAbstractItemModel( 1 );
   if( p )
      hb_retptrGC( hbqt_gcAllocate_QModelIndex( new QModelIndex( ( p )->parent( *hbqt_par_QModelIndex( 2 ) ) ), true ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QABSTRACTITEMMODEL_PARENT FP=hb_retptrGC( hbqt_gcAllocate_QModelIndex( new QModelIndex( ( p )->parent( *hbqt_par_QModelIndex( 2 ) ) ), true ) ); p is NULL" ) );
   }
}

/*
 * bool removeColumn ( int column, const QModelIndex & parent = QModelIndex() )
 */
HB_FUNC( QT_QABSTRACTITEMMODEL_REMOVECOLUMN )
{
   QAbstractItemModel * p = hbqt_par_QAbstractItemModel( 1 );
   if( p )
      hb_retl( ( p )->removeColumn( hb_parni( 2 ), ( HB_ISPOINTER( 3 ) ? *hbqt_par_QModelIndex( 3 ) : QModelIndex() ) ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QABSTRACTITEMMODEL_REMOVECOLUMN FP=hb_retl( ( p )->removeColumn( hb_parni( 2 ), ( HB_ISPOINTER( 3 ) ? *hbqt_par_QModelIndex( 3 ) : QModelIndex() ) ) ); p is NULL" ) );
   }
}

/*
 * virtual bool removeColumns ( int column, int count, const QModelIndex & parent = QModelIndex() )
 */
HB_FUNC( QT_QABSTRACTITEMMODEL_REMOVECOLUMNS )
{
   QAbstractItemModel * p = hbqt_par_QAbstractItemModel( 1 );
   if( p )
      hb_retl( ( p )->removeColumns( hb_parni( 2 ), hb_parni( 3 ), ( HB_ISPOINTER( 4 ) ? *hbqt_par_QModelIndex( 4 ) : QModelIndex() ) ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QABSTRACTITEMMODEL_REMOVECOLUMNS FP=hb_retl( ( p )->removeColumns( hb_parni( 2 ), hb_parni( 3 ), ( HB_ISPOINTER( 4 ) ? *hbqt_par_QModelIndex( 4 ) : QModelIndex() ) ) ); p is NULL" ) );
   }
}

/*
 * bool removeRow ( int row, const QModelIndex & parent = QModelIndex() )
 */
HB_FUNC( QT_QABSTRACTITEMMODEL_REMOVEROW )
{
   QAbstractItemModel * p = hbqt_par_QAbstractItemModel( 1 );
   if( p )
      hb_retl( ( p )->removeRow( hb_parni( 2 ), ( HB_ISPOINTER( 3 ) ? *hbqt_par_QModelIndex( 3 ) : QModelIndex() ) ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QABSTRACTITEMMODEL_REMOVEROW FP=hb_retl( ( p )->removeRow( hb_parni( 2 ), ( HB_ISPOINTER( 3 ) ? *hbqt_par_QModelIndex( 3 ) : QModelIndex() ) ) ); p is NULL" ) );
   }
}

/*
 * virtual bool removeRows ( int row, int count, const QModelIndex & parent = QModelIndex() )
 */
HB_FUNC( QT_QABSTRACTITEMMODEL_REMOVEROWS )
{
   QAbstractItemModel * p = hbqt_par_QAbstractItemModel( 1 );
   if( p )
      hb_retl( ( p )->removeRows( hb_parni( 2 ), hb_parni( 3 ), ( HB_ISPOINTER( 4 ) ? *hbqt_par_QModelIndex( 4 ) : QModelIndex() ) ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QABSTRACTITEMMODEL_REMOVEROWS FP=hb_retl( ( p )->removeRows( hb_parni( 2 ), hb_parni( 3 ), ( HB_ISPOINTER( 4 ) ? *hbqt_par_QModelIndex( 4 ) : QModelIndex() ) ) ); p is NULL" ) );
   }
}

/*
 * virtual int rowCount ( const QModelIndex & parent = QModelIndex() ) const = 0
 */
HB_FUNC( QT_QABSTRACTITEMMODEL_ROWCOUNT )
{
   QAbstractItemModel * p = hbqt_par_QAbstractItemModel( 1 );
   if( p )
      hb_retni( ( p )->rowCount( ( HB_ISPOINTER( 2 ) ? *hbqt_par_QModelIndex( 2 ) : QModelIndex() ) ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QABSTRACTITEMMODEL_ROWCOUNT FP=hb_retni( ( p )->rowCount( ( HB_ISPOINTER( 2 ) ? *hbqt_par_QModelIndex( 2 ) : QModelIndex() ) ) ); p is NULL" ) );
   }
}

/*
 * virtual bool setData ( const QModelIndex & index, const QVariant & value, int role = Qt::EditRole )
 */
HB_FUNC( QT_QABSTRACTITEMMODEL_SETDATA )
{
   QAbstractItemModel * p = hbqt_par_QAbstractItemModel( 1 );
   if( p )
      hb_retl( ( p )->setData( *hbqt_par_QModelIndex( 2 ), *hbqt_par_QVariant( 3 ), ( HB_ISNUM( 4 ) ? hb_parni( 4 ) : Qt::EditRole ) ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QABSTRACTITEMMODEL_SETDATA FP=hb_retl( ( p )->setData( *hbqt_par_QModelIndex( 2 ), *hbqt_par_QVariant( 3 ), ( HB_ISNUM( 4 ) ? hb_parni( 4 ) : Qt::EditRole ) ) ); p is NULL" ) );
   }
}

/*
 * virtual bool setHeaderData ( int section, Qt::Orientation orientation, const QVariant & value, int role = Qt::EditRole )
 */
HB_FUNC( QT_QABSTRACTITEMMODEL_SETHEADERDATA )
{
   QAbstractItemModel * p = hbqt_par_QAbstractItemModel( 1 );
   if( p )
      hb_retl( ( p )->setHeaderData( hb_parni( 2 ), ( Qt::Orientation ) hb_parni( 3 ), *hbqt_par_QVariant( 4 ), ( HB_ISNUM( 5 ) ? hb_parni( 5 ) : Qt::EditRole ) ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QABSTRACTITEMMODEL_SETHEADERDATA FP=hb_retl( ( p )->setHeaderData( hb_parni( 2 ), ( Qt::Orientation ) hb_parni( 3 ), *hbqt_par_QVariant( 4 ), ( HB_ISNUM( 5 ) ? hb_parni( 5 ) : Qt::EditRole ) ) ); p is NULL" ) );
   }
}

/*
 * void setSupportedDragActions ( Qt::DropActions actions )
 */
HB_FUNC( QT_QABSTRACTITEMMODEL_SETSUPPORTEDDRAGACTIONS )
{
   QAbstractItemModel * p = hbqt_par_QAbstractItemModel( 1 );
   if( p )
      ( p )->setSupportedDragActions( ( Qt::DropActions ) hb_parni( 2 ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QABSTRACTITEMMODEL_SETSUPPORTEDDRAGACTIONS FP=( p )->setSupportedDragActions( ( Qt::DropActions ) hb_parni( 2 ) ); p is NULL" ) );
   }
}

/*
 * QModelIndex sibling ( int row, int column, const QModelIndex & index ) const
 */
HB_FUNC( QT_QABSTRACTITEMMODEL_SIBLING )
{
   QAbstractItemModel * p = hbqt_par_QAbstractItemModel( 1 );
   if( p )
      hb_retptrGC( hbqt_gcAllocate_QModelIndex( new QModelIndex( ( p )->sibling( hb_parni( 2 ), hb_parni( 3 ), *hbqt_par_QModelIndex( 4 ) ) ), true ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QABSTRACTITEMMODEL_SIBLING FP=hb_retptrGC( hbqt_gcAllocate_QModelIndex( new QModelIndex( ( p )->sibling( hb_parni( 2 ), hb_parni( 3 ), *hbqt_par_QModelIndex( 4 ) ) ), true ) ); p is NULL" ) );
   }
}

/*
 * virtual void sort ( int column, Qt::SortOrder order = Qt::AscendingOrder )
 */
HB_FUNC( QT_QABSTRACTITEMMODEL_SORT )
{
   QAbstractItemModel * p = hbqt_par_QAbstractItemModel( 1 );
   if( p )
      ( p )->sort( hb_parni( 2 ), ( HB_ISNUM( 3 ) ? ( Qt::SortOrder ) hb_parni( 3 ) : ( Qt::SortOrder ) Qt::AscendingOrder ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QABSTRACTITEMMODEL_SORT FP=( p )->sort( hb_parni( 2 ), ( HB_ISNUM( 3 ) ? ( Qt::SortOrder ) hb_parni( 3 ) : ( Qt::SortOrder ) Qt::AscendingOrder ) ); p is NULL" ) );
   }
}

/*
 * virtual QSize span ( const QModelIndex & index ) const
 */
HB_FUNC( QT_QABSTRACTITEMMODEL_SPAN )
{
   QAbstractItemModel * p = hbqt_par_QAbstractItemModel( 1 );
   if( p )
      hb_retptrGC( hbqt_gcAllocate_QSize( new QSize( ( p )->span( *hbqt_par_QModelIndex( 2 ) ) ), true ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QABSTRACTITEMMODEL_SPAN FP=hb_retptrGC( hbqt_gcAllocate_QSize( new QSize( ( p )->span( *hbqt_par_QModelIndex( 2 ) ) ), true ) ); p is NULL" ) );
   }
}

/*
 * Qt::DropActions supportedDragActions () const
 */
HB_FUNC( QT_QABSTRACTITEMMODEL_SUPPORTEDDRAGACTIONS )
{
   QAbstractItemModel * p = hbqt_par_QAbstractItemModel( 1 );
   if( p )
      hb_retni( ( Qt::DropActions ) ( p )->supportedDragActions() );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QABSTRACTITEMMODEL_SUPPORTEDDRAGACTIONS FP=hb_retni( ( Qt::DropActions ) ( p )->supportedDragActions() ); p is NULL" ) );
   }
}

/*
 * virtual Qt::DropActions supportedDropActions () const
 */
HB_FUNC( QT_QABSTRACTITEMMODEL_SUPPORTEDDROPACTIONS )
{
   QAbstractItemModel * p = hbqt_par_QAbstractItemModel( 1 );
   if( p )
      hb_retni( ( Qt::DropActions ) ( p )->supportedDropActions() );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QABSTRACTITEMMODEL_SUPPORTEDDROPACTIONS FP=hb_retni( ( Qt::DropActions ) ( p )->supportedDropActions() ); p is NULL" ) );
   }
}

/*
 * virtual void revert ()
 */
HB_FUNC( QT_QABSTRACTITEMMODEL_REVERT )
{
   QAbstractItemModel * p = hbqt_par_QAbstractItemModel( 1 );
   if( p )
      ( p )->revert();
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QABSTRACTITEMMODEL_REVERT FP=( p )->revert(); p is NULL" ) );
   }
}

/*
 * virtual bool submit ()
 */
HB_FUNC( QT_QABSTRACTITEMMODEL_SUBMIT )
{
   QAbstractItemModel * p = hbqt_par_QAbstractItemModel( 1 );
   if( p )
      hb_retl( ( p )->submit() );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QABSTRACTITEMMODEL_SUBMIT FP=hb_retl( ( p )->submit() ); p is NULL" ) );
   }
}


/*----------------------------------------------------------------------*/
#endif             /* #if QT_VERSION >= 0x040500 */
/*----------------------------------------------------------------------*/
