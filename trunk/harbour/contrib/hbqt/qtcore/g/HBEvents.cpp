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

/*----------------------------------------------------------------------*/
#if QT_VERSION >= 0x040500
/*----------------------------------------------------------------------*/

#include <QtCore/QPointer>

#include <QtCore/QObject>
#include "hbqt_hbevents.h"


/*
 * HBEvents()
 * ~HBEvents()
 *
 */

typedef struct
{
   QPointer< HBEvents > ph;
   bool bNew;
   QT_G_FUNC_PTR func;
   int type;
} QGC_POINTER_HBEvents;

QT_G_FUNC( hbqt_gcRelease_HBEvents )
{
   HBEvents  * ph = NULL ;
   QGC_POINTER_HBEvents * p = ( QGC_POINTER_HBEvents * ) Cargo;

   if( p && p->bNew && p->ph )
   {
      ph = p->ph;
      if( ph )
      {
         const QMetaObject * m = ( ph )->metaObject();
         if( ( QString ) m->className() != ( QString ) "QObject" )
         {
            HB_TRACE( HB_TR_DEBUG, ( "ph=%p %p YES_rel_HBEvents   /.\\   ", (void*) ph, (void*) p->ph ) );
            delete ( p->ph );
            HB_TRACE( HB_TR_DEBUG, ( "ph=%p %p YES_rel_HBEvents   \\./   ", (void*) ph, (void*) p->ph ) );
            p->ph = NULL;
         }
         else
         {
            HB_TRACE( HB_TR_DEBUG, ( "ph=%p NO__rel_HBEvents          ", ph ) );
            p->ph = NULL;
         }
      }
      else
      {
         HB_TRACE( HB_TR_DEBUG, ( "ph=%p DEL_rel_HBEvents    :     Object already deleted!", ph ) );
         p->ph = NULL;
      }
   }
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "ph=%p PTR_rel_HBEvents    :    Object not created with new=true", ph ) );
      p->ph = NULL;
   }
}

void * hbqt_gcAllocate_HBEvents( void * pObj, bool bNew )
{
   QGC_POINTER_HBEvents * p = ( QGC_POINTER_HBEvents * ) hb_gcAllocate( sizeof( QGC_POINTER_HBEvents ), hbqt_gcFuncs() );

   new( & p->ph ) QPointer< HBEvents >( ( HBEvents * ) pObj );
   p->bNew = bNew;
   p->func = hbqt_gcRelease_HBEvents;
   p->type = HBQT_TYPE_HBEvents;

   if( bNew )
   {
      HB_TRACE( HB_TR_DEBUG, ( "ph=%p    _new_HBEvents  under p->pq", pObj ) );
   }
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "ph=%p NOT_new_HBEvents", pObj ) );
   }
   return p;
}

HB_FUNC( QT_HBEVENTS )
{
   HBEvents * pObj = NULL;

   pObj = new HBEvents() ;

   hb_retptrGC( hbqt_gcAllocate_HBEvents( ( void * ) pObj, true ) );
}

/*
 * bool hbConnect( PHB_ITEM obj, int event, PHB_ITEM block )
 */
HB_FUNC( QT_HBEVENTS_HBCONNECT )
{
   HBEvents * p = hbqt_par_HBEvents( 1 );
   if( p )
      hb_retl( ( p )->hbConnect( hb_param( 2, HB_IT_ANY ), hb_parni( 3 ), hb_param( 4, HB_IT_ANY ) ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_HBEVENTS_HBCONNECT FP=hb_retl( ( p )->hbConnect( hb_param( 2, HB_IT_ANY ), hb_parni( 3 ), hb_param( 4, HB_IT_ANY ) ) ); p is NULL" ) );
   }
}

/*
 * bool hbDisconnect( PHB_ITEM obj, int event )
 */
HB_FUNC( QT_HBEVENTS_HBDISCONNECT )
{
   HBEvents * p = hbqt_par_HBEvents( 1 );
   if( p )
      hb_retl( ( p )->hbDisconnect( hb_param( 2, HB_IT_ANY ), hb_parni( 3 ) ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_HBEVENTS_HBDISCONNECT FP=hb_retl( ( p )->hbDisconnect( hb_param( 2, HB_IT_ANY ), hb_parni( 3 ) ) ); p is NULL" ) );
   }
}

/*
 * bool hbClear()
 */
HB_FUNC( QT_HBEVENTS_HBCLEAR )
{
   HBEvents * p = hbqt_par_HBEvents( 1 );
   if( p )
      hb_retl( ( p )->hbClear() );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_HBEVENTS_HBCLEAR FP=hb_retl( ( p )->hbClear() ); p is NULL" ) );
   }
}


/*----------------------------------------------------------------------*/
#endif             /* #if QT_VERSION >= 0x040500 */
/*----------------------------------------------------------------------*/
