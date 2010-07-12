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

#include <QtCore/QModelIndex>


/*
 * QModelIndex ()
 * QModelIndex ( const QModelIndex & other )
 * ~QModelIndex ()
 */

typedef struct
{
   QModelIndex * ph;
   bool bNew;
   QT_G_FUNC_PTR func;
   int type;
} QGC_POINTER_QModelIndex;

QT_G_FUNC( hbqt_gcRelease_QModelIndex )
{
   QGC_POINTER * p = ( QGC_POINTER * ) Cargo;

   if( p && p->bNew )
   {
      if( p->ph )
      {
         HB_TRACE( HB_TR_DEBUG, ( "ph=%p    _rel_QModelIndex   /.\\", p->ph ) );
         delete ( ( QModelIndex * ) p->ph );
         HB_TRACE( HB_TR_DEBUG, ( "ph=%p YES_rel_QModelIndex   \\./", p->ph ) );
         p->ph = NULL;
      }
      else
      {
         HB_TRACE( HB_TR_DEBUG, ( "ph=%p DEL_rel_QModelIndex    :     Object already deleted!", p->ph ) );
         p->ph = NULL;
      }
   }
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "ph=%p PTR_rel_QModelIndex    :    Object not created with new=true", p->ph ) );
      p->ph = NULL;
   }
}

void * hbqt_gcAllocate_QModelIndex( void * pObj, bool bNew )
{
   QGC_POINTER * p = ( QGC_POINTER * ) hb_gcAllocate( sizeof( QGC_POINTER ), hbqt_gcFuncs() );

   p->ph = ( QModelIndex * ) pObj;
   p->bNew = bNew;
   p->func = hbqt_gcRelease_QModelIndex;
   p->type = HBQT_TYPE_QModelIndex;

   if( bNew )
   {
      HB_TRACE( HB_TR_DEBUG, ( "ph=%p    _new_QModelIndex", pObj ) );
   }
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "ph=%p NOT_new_QModelIndex", pObj ) );
   }
   return p;
}

HB_FUNC( QT_QMODELINDEX )
{
   QModelIndex * pObj = NULL;

   pObj = new QModelIndex() ;

   hb_retptrGC( hbqt_gcAllocate_QModelIndex( ( void * ) pObj, true ) );
}

/*
 * QModelIndex child ( int row, int column ) const
 */
HB_FUNC( QT_QMODELINDEX_CHILD )
{
   QModelIndex * p = hbqt_par_QModelIndex( 1 );
   if( p )
      hb_retptrGC( hbqt_gcAllocate_QModelIndex( new QModelIndex( ( p )->child( hb_parni( 2 ), hb_parni( 3 ) ) ), true ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QMODELINDEX_CHILD FP=hb_retptrGC( hbqt_gcAllocate_QModelIndex( new QModelIndex( ( p )->child( hb_parni( 2 ), hb_parni( 3 ) ) ), true ) ); p is NULL" ) );
   }
}

/*
 * int column () const
 */
HB_FUNC( QT_QMODELINDEX_COLUMN )
{
   QModelIndex * p = hbqt_par_QModelIndex( 1 );
   if( p )
      hb_retni( ( p )->column() );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QMODELINDEX_COLUMN FP=hb_retni( ( p )->column() ); p is NULL" ) );
   }
}

/*
 * QVariant data ( int role = Qt::DisplayRole ) const
 */
HB_FUNC( QT_QMODELINDEX_DATA )
{
   QModelIndex * p = hbqt_par_QModelIndex( 1 );
   if( p )
      hb_retptrGC( hbqt_gcAllocate_QVariant( new QVariant( ( p )->data( hb_parnidef( 2, Qt::DisplayRole ) ) ), true ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QMODELINDEX_DATA FP=hb_retptrGC( hbqt_gcAllocate_QVariant( new QVariant( ( p )->data( hb_parnidef( 2, Qt::DisplayRole ) ) ), true ) ); p is NULL" ) );
   }
}

/*
 * Qt::ItemFlags flags () const
 */
HB_FUNC( QT_QMODELINDEX_FLAGS )
{
   QModelIndex * p = hbqt_par_QModelIndex( 1 );
   if( p )
      hb_retni( ( Qt::ItemFlags ) ( p )->flags() );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QMODELINDEX_FLAGS FP=hb_retni( ( Qt::ItemFlags ) ( p )->flags() ); p is NULL" ) );
   }
}

/*
 * qint64 internalId () const
 */
HB_FUNC( QT_QMODELINDEX_INTERNALID )
{
   QModelIndex * p = hbqt_par_QModelIndex( 1 );
   if( p )
      hb_retnint( ( p )->internalId() );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QMODELINDEX_INTERNALID FP=hb_retnint( ( p )->internalId() ); p is NULL" ) );
   }
}

/*
 * void * internalPointer () const
 */
HB_FUNC( QT_QMODELINDEX_INTERNALPOINTER )
{
   QModelIndex * p = hbqt_par_QModelIndex( 1 );
   if( p )
      ( p )->internalPointer();
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QMODELINDEX_INTERNALPOINTER FP=( p )->internalPointer(); p is NULL" ) );
   }
}

/*
 * bool isValid () const
 */
HB_FUNC( QT_QMODELINDEX_ISVALID )
{
   QModelIndex * p = hbqt_par_QModelIndex( 1 );
   if( p )
      hb_retl( ( p )->isValid() );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QMODELINDEX_ISVALID FP=hb_retl( ( p )->isValid() ); p is NULL" ) );
   }
}

/*
 * const QAbstractItemModel * model () const
 */
HB_FUNC( QT_QMODELINDEX_MODEL )
{
   QModelIndex * p = hbqt_par_QModelIndex( 1 );
   if( p )
      hb_retptrGC( hbqt_gcAllocate_QAbstractItemModel( ( void * ) ( p )->model(), false ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QMODELINDEX_MODEL FP=hb_retptrGC( hbqt_gcAllocate_QAbstractItemModel( ( void * ) ( p )->model(), false ) ); p is NULL" ) );
   }
}

/*
 * QModelIndex parent () const
 */
HB_FUNC( QT_QMODELINDEX_PARENT )
{
   QModelIndex * p = hbqt_par_QModelIndex( 1 );
   if( p )
      hb_retptrGC( hbqt_gcAllocate_QModelIndex( new QModelIndex( ( p )->parent() ), true ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QMODELINDEX_PARENT FP=hb_retptrGC( hbqt_gcAllocate_QModelIndex( new QModelIndex( ( p )->parent() ), true ) ); p is NULL" ) );
   }
}

/*
 * int row () const
 */
HB_FUNC( QT_QMODELINDEX_ROW )
{
   QModelIndex * p = hbqt_par_QModelIndex( 1 );
   if( p )
      hb_retni( ( p )->row() );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QMODELINDEX_ROW FP=hb_retni( ( p )->row() ); p is NULL" ) );
   }
}

/*
 * QModelIndex sibling ( int row, int column ) const
 */
HB_FUNC( QT_QMODELINDEX_SIBLING )
{
   QModelIndex * p = hbqt_par_QModelIndex( 1 );
   if( p )
      hb_retptrGC( hbqt_gcAllocate_QModelIndex( new QModelIndex( ( p )->sibling( hb_parni( 2 ), hb_parni( 3 ) ) ), true ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QMODELINDEX_SIBLING FP=hb_retptrGC( hbqt_gcAllocate_QModelIndex( new QModelIndex( ( p )->sibling( hb_parni( 2 ), hb_parni( 3 ) ) ), true ) ); p is NULL" ) );
   }
}


/*----------------------------------------------------------------------*/
#endif             /* #if QT_VERSION >= 0x040500 */
/*----------------------------------------------------------------------*/
