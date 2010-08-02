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

#include <QtCore/QAbstractItemModel>
#include "hbqt_hbqabstractitemmodel.h"


/*
 *
 *
 */

typedef struct
{
   QPointer< HBQAbstractItemModel > ph;
   bool bNew;
   QT_G_FUNC_PTR func;
   int type;
} QGC_POINTER_HBQAbstractItemModel;

QT_G_FUNC( hbqt_gcRelease_HBQAbstractItemModel )
{
   HBQAbstractItemModel  * ph = NULL ;
   QGC_POINTER_HBQAbstractItemModel * p = ( QGC_POINTER_HBQAbstractItemModel * ) Cargo;

   if( p && p->bNew && p->ph )
   {
      ph = p->ph;
      if( ph )
      {
         const QMetaObject * m = ( ph )->metaObject();
         if( ( QString ) m->className() != ( QString ) "QObject" )
         {
            HB_TRACE( HB_TR_DEBUG, ( "ph=%p %p YES_rel_HBQAbstractItemModel   /.\\   ", (void*) ph, (void*) p->ph ) );
            delete ( p->ph );
            HB_TRACE( HB_TR_DEBUG, ( "ph=%p %p YES_rel_HBQAbstractItemModel   \\./   ", (void*) ph, (void*) p->ph ) );
            p->ph = NULL;
         }
         else
         {
            HB_TRACE( HB_TR_DEBUG, ( "ph=%p NO__rel_HBQAbstractItemModel          ", ph ) );
            p->ph = NULL;
         }
      }
      else
      {
         HB_TRACE( HB_TR_DEBUG, ( "ph=%p DEL_rel_HBQAbstractItemModel    :     Object already deleted!", ph ) );
         p->ph = NULL;
      }
   }
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "ph=%p PTR_rel_HBQAbstractItemModel    :    Object not created with new=true", ph ) );
      p->ph = NULL;
   }
}

void * hbqt_gcAllocate_HBQAbstractItemModel( void * pObj, bool bNew )
{
   QGC_POINTER_HBQAbstractItemModel * p = ( QGC_POINTER_HBQAbstractItemModel * ) hb_gcAllocate( sizeof( QGC_POINTER_HBQAbstractItemModel ), hbqt_gcFuncs() );

   new( & p->ph ) QPointer< HBQAbstractItemModel >( ( HBQAbstractItemModel * ) pObj );
   p->bNew = bNew;
   p->func = hbqt_gcRelease_HBQAbstractItemModel;
   p->type = HBQT_TYPE_HBQAbstractItemModel;

   if( bNew )
   {
      HB_TRACE( HB_TR_DEBUG, ( "ph=%p    _new_HBQAbstractItemModel  under p->pq", pObj ) );
   }
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "ph=%p NOT_new_HBQAbstractItemModel", pObj ) );
   }
   return p;
}

HB_FUNC( QT_HBQABSTRACTITEMMODEL )
{
   HBQAbstractItemModel * pObj = NULL;

   pObj = new HBQAbstractItemModel( ( PHB_ITEM ) hb_param( 1, HB_IT_BLOCK ) ) ;

   hb_retptrGC( hbqt_gcAllocate_HBQAbstractItemModel( ( void * ) pObj, true ) );
}

/*
 * void reset()
 */
HB_FUNC( QT_HBQABSTRACTITEMMODEL_RESET )
{
   HBQAbstractItemModel * p = hbqt_par_HBQAbstractItemModel( 1 );
   if( p )
      ( p )->reset();
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_HBQABSTRACTITEMMODEL_RESET FP=( p )->reset(); p is NULL" ) );
   }
}

/*
 * QModelIndex index( int row, int column, const QModelIndex & parent = QModelIndex() ) const
 */
HB_FUNC( QT_HBQABSTRACTITEMMODEL_INDEX )
{
   HBQAbstractItemModel * p = hbqt_par_HBQAbstractItemModel( 1 );
   if( p )
      hb_retptrGC( hbqt_gcAllocate_QModelIndex( new QModelIndex( ( p )->index( hb_parni( 2 ), hb_parni( 3 ), ( HB_ISPOINTER( 4 ) ? *hbqt_par_QModelIndex( 4 ) : QModelIndex() ) ) ), true ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_HBQABSTRACTITEMMODEL_INDEX FP=hb_retptrGC( hbqt_gcAllocate_QModelIndex( new QModelIndex( ( p )->index( hb_parni( 2 ), hb_parni( 3 ), ( HB_ISPOINTER( 4 ) ? *hbqt_par_QModelIndex( 4 ) : QModelIndex() ) ) ), true ) ); p is NULL" ) );
   }
}


/*----------------------------------------------------------------------*/
#endif             /* #if QT_VERSION >= 0x040500 */
/*----------------------------------------------------------------------*/
