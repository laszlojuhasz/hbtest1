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

#include <QtGui/QStringListModel>


/*
 * QStringListModel ( QObject * parent = 0 )
 * QStringListModel ( const QStringList & strings, QObject * parent = 0 )
 */

typedef struct
{
   QPointer< QStringListModel > ph;
   bool bNew;
   QT_G_FUNC_PTR func;
   int type;
} QGC_POINTER_QStringListModel;

QT_G_FUNC( hbqt_gcRelease_QStringListModel )
{
   QStringListModel  * ph = NULL ;
   QGC_POINTER_QStringListModel * p = ( QGC_POINTER_QStringListModel * ) Cargo;

   if( p && p->bNew && p->ph )
   {
      ph = p->ph;
      if( ph )
      {
         const QMetaObject * m = ( ph )->metaObject();
         if( ( QString ) m->className() != ( QString ) "QObject" )
         {
            HB_TRACE( HB_TR_DEBUG, ( "ph=%p %p YES_rel_QStringListModel   /.\\   ", (void*) ph, (void*) p->ph ) );
            delete ( p->ph );
            HB_TRACE( HB_TR_DEBUG, ( "ph=%p %p YES_rel_QStringListModel   \\./   ", (void*) ph, (void*) p->ph ) );
            p->ph = NULL;
         }
         else
         {
            HB_TRACE( HB_TR_DEBUG, ( "ph=%p NO__rel_QStringListModel          ", ph ) );
            p->ph = NULL;
         }
      }
      else
      {
         HB_TRACE( HB_TR_DEBUG, ( "ph=%p DEL_rel_QStringListModel    :     Object already deleted!", ph ) );
         p->ph = NULL;
      }
   }
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "ph=%p PTR_rel_QStringListModel    :    Object not created with new=true", ph ) );
      p->ph = NULL;
   }
}

void * hbqt_gcAllocate_QStringListModel( void * pObj, bool bNew )
{
   QGC_POINTER_QStringListModel * p = ( QGC_POINTER_QStringListModel * ) hb_gcAllocate( sizeof( QGC_POINTER_QStringListModel ), hbqt_gcFuncs() );

   new( & p->ph ) QPointer< QStringListModel >( ( QStringListModel * ) pObj );
   p->bNew = bNew;
   p->func = hbqt_gcRelease_QStringListModel;
   p->type = HBQT_TYPE_QStringListModel;

   if( bNew )
   {
      HB_TRACE( HB_TR_DEBUG, ( "ph=%p    _new_QStringListModel  under p->pq", pObj ) );
   }
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "ph=%p NOT_new_QStringListModel", pObj ) );
   }
   return p;
}

HB_FUNC( QT_QSTRINGLISTMODEL )
{
   QStringListModel * pObj = NULL;

   pObj =  new QStringListModel() ;

   hb_retptrGC( hbqt_gcAllocate_QStringListModel( ( void * ) pObj, true ) );
}

/*
 * virtual QVariant data ( const QModelIndex & index, int role ) const
 */
HB_FUNC( QT_QSTRINGLISTMODEL_DATA )
{
   QStringListModel * p = hbqt_par_QStringListModel( 1 );
   if( p )
      hb_retptrGC( hbqt_gcAllocate_QVariant( new QVariant( ( p )->data( *hbqt_par_QModelIndex( 2 ), hb_parni( 3 ) ) ), true ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QSTRINGLISTMODEL_DATA FP=hb_retptrGC( hbqt_gcAllocate_QVariant( new QVariant( ( p )->data( *hbqt_par_QModelIndex( 2 ), hb_parni( 3 ) ) ), true ) ); p is NULL" ) );
   }
}

/*
 * virtual Qt::ItemFlags flags ( const QModelIndex & index ) const
 */
HB_FUNC( QT_QSTRINGLISTMODEL_FLAGS )
{
   QStringListModel * p = hbqt_par_QStringListModel( 1 );
   if( p )
      hb_retni( ( Qt::ItemFlags ) ( p )->flags( *hbqt_par_QModelIndex( 2 ) ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QSTRINGLISTMODEL_FLAGS FP=hb_retni( ( Qt::ItemFlags ) ( p )->flags( *hbqt_par_QModelIndex( 2 ) ) ); p is NULL" ) );
   }
}

/*
 * virtual bool insertRows ( int row, int count, const QModelIndex & parent = QModelIndex() )
 */
HB_FUNC( QT_QSTRINGLISTMODEL_INSERTROWS )
{
   QStringListModel * p = hbqt_par_QStringListModel( 1 );
   if( p )
      hb_retl( ( p )->insertRows( hb_parni( 2 ), hb_parni( 3 ), ( HB_ISPOINTER( 4 ) ? *hbqt_par_QModelIndex( 4 ) : QModelIndex() ) ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QSTRINGLISTMODEL_INSERTROWS FP=hb_retl( ( p )->insertRows( hb_parni( 2 ), hb_parni( 3 ), ( HB_ISPOINTER( 4 ) ? *hbqt_par_QModelIndex( 4 ) : QModelIndex() ) ) ); p is NULL" ) );
   }
}

/*
 * virtual bool removeRows ( int row, int count, const QModelIndex & parent = QModelIndex() )
 */
HB_FUNC( QT_QSTRINGLISTMODEL_REMOVEROWS )
{
   QStringListModel * p = hbqt_par_QStringListModel( 1 );
   if( p )
      hb_retl( ( p )->removeRows( hb_parni( 2 ), hb_parni( 3 ), ( HB_ISPOINTER( 4 ) ? *hbqt_par_QModelIndex( 4 ) : QModelIndex() ) ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QSTRINGLISTMODEL_REMOVEROWS FP=hb_retl( ( p )->removeRows( hb_parni( 2 ), hb_parni( 3 ), ( HB_ISPOINTER( 4 ) ? *hbqt_par_QModelIndex( 4 ) : QModelIndex() ) ) ); p is NULL" ) );
   }
}

/*
 * virtual int rowCount ( const QModelIndex & parent = QModelIndex() ) const
 */
HB_FUNC( QT_QSTRINGLISTMODEL_ROWCOUNT )
{
   QStringListModel * p = hbqt_par_QStringListModel( 1 );
   if( p )
      hb_retni( ( p )->rowCount( ( HB_ISPOINTER( 2 ) ? *hbqt_par_QModelIndex( 2 ) : QModelIndex() ) ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QSTRINGLISTMODEL_ROWCOUNT FP=hb_retni( ( p )->rowCount( ( HB_ISPOINTER( 2 ) ? *hbqt_par_QModelIndex( 2 ) : QModelIndex() ) ) ); p is NULL" ) );
   }
}

/*
 * virtual bool setData ( const QModelIndex & index, const QVariant & value, int role = Qt::EditRole )
 */
HB_FUNC( QT_QSTRINGLISTMODEL_SETDATA )
{
   QStringListModel * p = hbqt_par_QStringListModel( 1 );
   if( p )
      hb_retl( ( p )->setData( *hbqt_par_QModelIndex( 2 ), *hbqt_par_QVariant( 3 ), hb_parnidef( 4, Qt::EditRole ) ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QSTRINGLISTMODEL_SETDATA FP=hb_retl( ( p )->setData( *hbqt_par_QModelIndex( 2 ), *hbqt_par_QVariant( 3 ), hb_parnidef( 4, Qt::EditRole ) ) ); p is NULL" ) );
   }
}

/*
 * void setStringList ( const QStringList & strings )
 */
HB_FUNC( QT_QSTRINGLISTMODEL_SETSTRINGLIST )
{
   QStringListModel * p = hbqt_par_QStringListModel( 1 );
   if( p )
      ( p )->setStringList( *hbqt_par_QStringList( 2 ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QSTRINGLISTMODEL_SETSTRINGLIST FP=( p )->setStringList( *hbqt_par_QStringList( 2 ) ); p is NULL" ) );
   }
}

/*
 * QStringList stringList () const
 */
HB_FUNC( QT_QSTRINGLISTMODEL_STRINGLIST )
{
   QStringListModel * p = hbqt_par_QStringListModel( 1 );
   if( p )
      hb_retptrGC( hbqt_gcAllocate_QStringList( new QStringList( ( p )->stringList() ), true ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QSTRINGLISTMODEL_STRINGLIST FP=hb_retptrGC( hbqt_gcAllocate_QStringList( new QStringList( ( p )->stringList() ), true ) ); p is NULL" ) );
   }
}


/*----------------------------------------------------------------------*/
#endif             /* #if QT_VERSION >= 0x040500 */
/*----------------------------------------------------------------------*/
