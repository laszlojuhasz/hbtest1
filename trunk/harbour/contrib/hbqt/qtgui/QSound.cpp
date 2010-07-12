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

#include <QtGui/QSound>


/*
 * QSound ( const QString & filename, QObject * parent = 0 )
 * ~QSound ()
 */

typedef struct
{
   QPointer< QSound > ph;
   bool bNew;
   QT_G_FUNC_PTR func;
   int type;
} QGC_POINTER_QSound;

QT_G_FUNC( hbqt_gcRelease_QSound )
{
   QSound  * ph = NULL ;
   QGC_POINTER_QSound * p = ( QGC_POINTER_QSound * ) Cargo;

   if( p && p->bNew && p->ph )
   {
      ph = p->ph;
      if( ph )
      {
         const QMetaObject * m = ( ph )->metaObject();
         if( ( QString ) m->className() != ( QString ) "QObject" )
         {
            HB_TRACE( HB_TR_DEBUG, ( "ph=%p %p YES_rel_QSound   /.\\   ", (void*) ph, (void*) p->ph ) );
            delete ( p->ph );
            HB_TRACE( HB_TR_DEBUG, ( "ph=%p %p YES_rel_QSound   \\./   ", (void*) ph, (void*) p->ph ) );
            p->ph = NULL;
         }
         else
         {
            HB_TRACE( HB_TR_DEBUG, ( "ph=%p NO__rel_QSound          ", ph ) );
            p->ph = NULL;
         }
      }
      else
      {
         HB_TRACE( HB_TR_DEBUG, ( "ph=%p DEL_rel_QSound    :     Object already deleted!", ph ) );
         p->ph = NULL;
      }
   }
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "ph=%p PTR_rel_QSound    :    Object not created with new=true", ph ) );
      p->ph = NULL;
   }
}

void * hbqt_gcAllocate_QSound( void * pObj, bool bNew )
{
   QGC_POINTER_QSound * p = ( QGC_POINTER_QSound * ) hb_gcAllocate( sizeof( QGC_POINTER_QSound ), hbqt_gcFuncs() );

   new( & p->ph ) QPointer< QSound >( ( QSound * ) pObj );
   p->bNew = bNew;
   p->func = hbqt_gcRelease_QSound;
   p->type = HBQT_TYPE_QSound;

   if( bNew )
   {
      HB_TRACE( HB_TR_DEBUG, ( "ph=%p    _new_QSound  under p->pq", pObj ) );
   }
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "ph=%p NOT_new_QSound", pObj ) );
   }
   return p;
}

HB_FUNC( QT_QSOUND )
{
   QSound * pObj = NULL;

   pObj =  new QSound( hbqt_par_QString( 1 ) ) ;

   hb_retptrGC( hbqt_gcAllocate_QSound( ( void * ) pObj, true ) );
}

/*
 * QString fileName () const
 */
HB_FUNC( QT_QSOUND_FILENAME )
{
   QSound * p = hbqt_par_QSound( 1 );
   if( p )
      hb_retc( ( p )->fileName().toAscii().data() );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QSOUND_FILENAME FP=hb_retc( ( p )->fileName().toAscii().data() ); p is NULL" ) );
   }
}

/*
 * bool isFinished () const
 */
HB_FUNC( QT_QSOUND_ISFINISHED )
{
   QSound * p = hbqt_par_QSound( 1 );
   if( p )
      hb_retl( ( p )->isFinished() );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QSOUND_ISFINISHED FP=hb_retl( ( p )->isFinished() ); p is NULL" ) );
   }
}

/*
 * int loops () const
 */
HB_FUNC( QT_QSOUND_LOOPS )
{
   QSound * p = hbqt_par_QSound( 1 );
   if( p )
      hb_retni( ( p )->loops() );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QSOUND_LOOPS FP=hb_retni( ( p )->loops() ); p is NULL" ) );
   }
}

/*
 * int loopsRemaining () const
 */
HB_FUNC( QT_QSOUND_LOOPSREMAINING )
{
   QSound * p = hbqt_par_QSound( 1 );
   if( p )
      hb_retni( ( p )->loopsRemaining() );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QSOUND_LOOPSREMAINING FP=hb_retni( ( p )->loopsRemaining() ); p is NULL" ) );
   }
}

/*
 * void setLoops ( int number )
 */
HB_FUNC( QT_QSOUND_SETLOOPS )
{
   QSound * p = hbqt_par_QSound( 1 );
   if( p )
      ( p )->setLoops( hb_parni( 2 ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QSOUND_SETLOOPS FP=( p )->setLoops( hb_parni( 2 ) ); p is NULL" ) );
   }
}

/*
 * bool isAvailable ()
 */
HB_FUNC( QT_QSOUND_ISAVAILABLE )
{
   QSound * p = hbqt_par_QSound( 1 );
   if( p )
      hb_retl( ( p )->isAvailable() );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QSOUND_ISAVAILABLE FP=hb_retl( ( p )->isAvailable() ); p is NULL" ) );
   }
}

/*
 * void play ( const QString & filename )
 */
HB_FUNC( QT_QSOUND_PLAY )
{
   QSound * p = hbqt_par_QSound( 1 );
   if( p )
      ( p )->play( QSound::tr( hb_parc( 2 ) ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QSOUND_PLAY FP=( p )->play( QSound::tr( hb_parc( 2 ) ) ); p is NULL" ) );
   }
}


/*----------------------------------------------------------------------*/
#endif             /* #if QT_VERSION >= 0x040500 */
/*----------------------------------------------------------------------*/
