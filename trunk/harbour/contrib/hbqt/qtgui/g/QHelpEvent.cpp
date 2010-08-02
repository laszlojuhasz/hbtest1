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

#include <QtGui/QHelpEvent>


/* QHelpEvent ( Type type, const QPoint & pos, const QPoint & globalPos )
 *
 */

typedef struct
{
   QHelpEvent * ph;
   bool bNew;
   PHBQT_GC_FUNC func;
   int type;
} HBQT_GC_T_QHelpEvent;

HBQT_GC_FUNC( hbqt_gcRelease_QHelpEvent )
{
   HB_SYMBOL_UNUSED( Cargo );
   HBQT_GC_T * p = ( HBQT_GC_T * ) Cargo;

   if( p && p->bNew )
   {
      p->ph = NULL;
   }
}

void * hbqt_gcAllocate_QHelpEvent( void * pObj, bool bNew )
{
   HBQT_GC_T * p = ( HBQT_GC_T * ) hb_gcAllocate( sizeof( HBQT_GC_T ), hbqt_gcFuncs() );

   p->ph = ( QHelpEvent * ) pObj;
   p->bNew = bNew;
   p->func = hbqt_gcRelease_QHelpEvent;
   p->type = HBQT_TYPE_QHelpEvent;

   if( bNew )
   {
      HB_TRACE( HB_TR_DEBUG, ( "ph=%p    _new_QHelpEvent", pObj ) );
   }
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "ph=%p NOT_new_QHelpEvent", pObj ) );
   }
   return p;
}

HB_FUNC( QT_QHELPEVENT )
{
   // hb_retptr( new QHelpEvent() );
}

/*
 * const QPoint & globalPos () const
 */
HB_FUNC( QT_QHELPEVENT_GLOBALPOS )
{
   QHelpEvent * p = hbqt_par_QHelpEvent( 1 );
   if( p )
      hb_retptrGC( hbqt_gcAllocate_QPoint( new QPoint( ( p )->globalPos() ), true ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QHELPEVENT_GLOBALPOS FP=hb_retptrGC( hbqt_gcAllocate_QPoint( new QPoint( ( p )->globalPos() ), true ) ); p is NULL" ) );
   }
}

/*
 * int globalX () const
 */
HB_FUNC( QT_QHELPEVENT_GLOBALX )
{
   QHelpEvent * p = hbqt_par_QHelpEvent( 1 );
   if( p )
      hb_retni( ( p )->globalX() );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QHELPEVENT_GLOBALX FP=hb_retni( ( p )->globalX() ); p is NULL" ) );
   }
}

/*
 * int globalY () const
 */
HB_FUNC( QT_QHELPEVENT_GLOBALY )
{
   QHelpEvent * p = hbqt_par_QHelpEvent( 1 );
   if( p )
      hb_retni( ( p )->globalY() );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QHELPEVENT_GLOBALY FP=hb_retni( ( p )->globalY() ); p is NULL" ) );
   }
}

/*
 * const QPoint & pos () const
 */
HB_FUNC( QT_QHELPEVENT_POS )
{
   QHelpEvent * p = hbqt_par_QHelpEvent( 1 );
   if( p )
      hb_retptrGC( hbqt_gcAllocate_QPoint( new QPoint( ( p )->pos() ), true ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QHELPEVENT_POS FP=hb_retptrGC( hbqt_gcAllocate_QPoint( new QPoint( ( p )->pos() ), true ) ); p is NULL" ) );
   }
}

/*
 * int x () const
 */
HB_FUNC( QT_QHELPEVENT_X )
{
   QHelpEvent * p = hbqt_par_QHelpEvent( 1 );
   if( p )
      hb_retni( ( p )->x() );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QHELPEVENT_X FP=hb_retni( ( p )->x() ); p is NULL" ) );
   }
}

/*
 * int y () const
 */
HB_FUNC( QT_QHELPEVENT_Y )
{
   QHelpEvent * p = hbqt_par_QHelpEvent( 1 );
   if( p )
      hb_retni( ( p )->y() );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QHELPEVENT_Y FP=hb_retni( ( p )->y() ); p is NULL" ) );
   }
}


/*----------------------------------------------------------------------*/
#endif             /* #if QT_VERSION >= 0x040500 */
/*----------------------------------------------------------------------*/
