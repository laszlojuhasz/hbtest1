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

/*
 *  enum CurveShape { EaseInCurve, EaseOutCurve, EaseInOutCurve, LinearCurve, SineCurve, CosineCurve }
 *  enum Direction { Forward, Backward }
 *  enum State { NotRunning, Paused, Running }
 */

#include <QtCore/QPointer>

#include <QtCore/QTimeLine>


/*
 * QTimeLine ( int duration = 1000, QObject * parent = 0 )
 * virtual ~QTimeLine ()
 */

typedef struct
{
   QPointer< QTimeLine > ph;
   bool bNew;
   PHBQT_GC_FUNC func;
   int type;
} HBQT_GC_T_QTimeLine;

HBQT_GC_FUNC( hbqt_gcRelease_QTimeLine )
{
   QTimeLine  * ph = NULL ;
   HBQT_GC_T_QTimeLine * p = ( HBQT_GC_T_QTimeLine * ) Cargo;

   if( p && p->bNew && p->ph )
   {
      ph = p->ph;
      if( ph )
      {
         const QMetaObject * m = ( ph )->metaObject();
         if( ( QString ) m->className() != ( QString ) "QObject" )
         {
            HB_TRACE( HB_TR_DEBUG, ( "ph=%p %p YES_rel_QTimeLine   /.\\   ", (void*) ph, (void*) p->ph ) );
            delete ( p->ph );
            HB_TRACE( HB_TR_DEBUG, ( "ph=%p %p YES_rel_QTimeLine   \\./   ", (void*) ph, (void*) p->ph ) );
            p->ph = NULL;
         }
         else
         {
            HB_TRACE( HB_TR_DEBUG, ( "ph=%p NO__rel_QTimeLine          ", ph ) );
            p->ph = NULL;
         }
      }
      else
      {
         HB_TRACE( HB_TR_DEBUG, ( "ph=%p DEL_rel_QTimeLine    :     Object already deleted!", ph ) );
         p->ph = NULL;
      }
   }
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "ph=%p PTR_rel_QTimeLine    :    Object not created with new=true", ph ) );
      p->ph = NULL;
   }
}

void * hbqt_gcAllocate_QTimeLine( void * pObj, bool bNew )
{
   HBQT_GC_T_QTimeLine * p = ( HBQT_GC_T_QTimeLine * ) hb_gcAllocate( sizeof( HBQT_GC_T_QTimeLine ), hbqt_gcFuncs() );

   new( & p->ph ) QPointer< QTimeLine >( ( QTimeLine * ) pObj );
   p->bNew = bNew;
   p->func = hbqt_gcRelease_QTimeLine;
   p->type = HBQT_TYPE_QTimeLine;

   if( bNew )
   {
      HB_TRACE( HB_TR_DEBUG, ( "ph=%p    _new_QTimeLine  under p->pq", pObj ) );
   }
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "ph=%p NOT_new_QTimeLine", pObj ) );
   }
   return p;
}

HB_FUNC( QT_QTIMELINE )
{
   QTimeLine * pObj = NULL;

   pObj = new QTimeLine() ;

   hb_retptrGC( hbqt_gcAllocate_QTimeLine( ( void * ) pObj, true ) );
}

/*
 * int currentFrame () const
 */
HB_FUNC( QT_QTIMELINE_CURRENTFRAME )
{
   QTimeLine * p = hbqt_par_QTimeLine( 1 );
   if( p )
      hb_retni( ( p )->currentFrame() );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QTIMELINE_CURRENTFRAME FP=hb_retni( ( p )->currentFrame() ); p is NULL" ) );
   }
}

/*
 * int currentTime () const
 */
HB_FUNC( QT_QTIMELINE_CURRENTTIME )
{
   QTimeLine * p = hbqt_par_QTimeLine( 1 );
   if( p )
      hb_retni( ( p )->currentTime() );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QTIMELINE_CURRENTTIME FP=hb_retni( ( p )->currentTime() ); p is NULL" ) );
   }
}

/*
 * qreal currentValue () const
 */
HB_FUNC( QT_QTIMELINE_CURRENTVALUE )
{
   QTimeLine * p = hbqt_par_QTimeLine( 1 );
   if( p )
      hb_retnd( ( p )->currentValue() );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QTIMELINE_CURRENTVALUE FP=hb_retnd( ( p )->currentValue() ); p is NULL" ) );
   }
}

/*
 * CurveShape curveShape () const
 */
HB_FUNC( QT_QTIMELINE_CURVESHAPE )
{
   QTimeLine * p = hbqt_par_QTimeLine( 1 );
   if( p )
      hb_retni( ( QTimeLine::CurveShape ) ( p )->curveShape() );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QTIMELINE_CURVESHAPE FP=hb_retni( ( QTimeLine::CurveShape ) ( p )->curveShape() ); p is NULL" ) );
   }
}

/*
 * Direction direction () const
 */
HB_FUNC( QT_QTIMELINE_DIRECTION )
{
   QTimeLine * p = hbqt_par_QTimeLine( 1 );
   if( p )
      hb_retni( ( QTimeLine::Direction ) ( p )->direction() );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QTIMELINE_DIRECTION FP=hb_retni( ( QTimeLine::Direction ) ( p )->direction() ); p is NULL" ) );
   }
}

/*
 * int duration () const
 */
HB_FUNC( QT_QTIMELINE_DURATION )
{
   QTimeLine * p = hbqt_par_QTimeLine( 1 );
   if( p )
      hb_retni( ( p )->duration() );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QTIMELINE_DURATION FP=hb_retni( ( p )->duration() ); p is NULL" ) );
   }
}

/*
 * int endFrame () const
 */
HB_FUNC( QT_QTIMELINE_ENDFRAME )
{
   QTimeLine * p = hbqt_par_QTimeLine( 1 );
   if( p )
      hb_retni( ( p )->endFrame() );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QTIMELINE_ENDFRAME FP=hb_retni( ( p )->endFrame() ); p is NULL" ) );
   }
}

/*
 * int frameForTime ( int msec ) const
 */
HB_FUNC( QT_QTIMELINE_FRAMEFORTIME )
{
   QTimeLine * p = hbqt_par_QTimeLine( 1 );
   if( p )
      hb_retni( ( p )->frameForTime( hb_parni( 2 ) ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QTIMELINE_FRAMEFORTIME FP=hb_retni( ( p )->frameForTime( hb_parni( 2 ) ) ); p is NULL" ) );
   }
}

/*
 * int loopCount () const
 */
HB_FUNC( QT_QTIMELINE_LOOPCOUNT )
{
   QTimeLine * p = hbqt_par_QTimeLine( 1 );
   if( p )
      hb_retni( ( p )->loopCount() );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QTIMELINE_LOOPCOUNT FP=hb_retni( ( p )->loopCount() ); p is NULL" ) );
   }
}

/*
 * void setCurveShape ( CurveShape shape )
 */
HB_FUNC( QT_QTIMELINE_SETCURVESHAPE )
{
   QTimeLine * p = hbqt_par_QTimeLine( 1 );
   if( p )
      ( p )->setCurveShape( ( QTimeLine::CurveShape ) hb_parni( 2 ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QTIMELINE_SETCURVESHAPE FP=( p )->setCurveShape( ( QTimeLine::CurveShape ) hb_parni( 2 ) ); p is NULL" ) );
   }
}

/*
 * void setDirection ( Direction direction )
 */
HB_FUNC( QT_QTIMELINE_SETDIRECTION )
{
   QTimeLine * p = hbqt_par_QTimeLine( 1 );
   if( p )
      ( p )->setDirection( ( QTimeLine::Direction ) hb_parni( 2 ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QTIMELINE_SETDIRECTION FP=( p )->setDirection( ( QTimeLine::Direction ) hb_parni( 2 ) ); p is NULL" ) );
   }
}

/*
 * void setDuration ( int duration )
 */
HB_FUNC( QT_QTIMELINE_SETDURATION )
{
   QTimeLine * p = hbqt_par_QTimeLine( 1 );
   if( p )
      ( p )->setDuration( hb_parni( 2 ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QTIMELINE_SETDURATION FP=( p )->setDuration( hb_parni( 2 ) ); p is NULL" ) );
   }
}

/*
 * void setEndFrame ( int frame )
 */
HB_FUNC( QT_QTIMELINE_SETENDFRAME )
{
   QTimeLine * p = hbqt_par_QTimeLine( 1 );
   if( p )
      ( p )->setEndFrame( hb_parni( 2 ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QTIMELINE_SETENDFRAME FP=( p )->setEndFrame( hb_parni( 2 ) ); p is NULL" ) );
   }
}

/*
 * void setFrameRange ( int startFrame, int endFrame )
 */
HB_FUNC( QT_QTIMELINE_SETFRAMERANGE )
{
   QTimeLine * p = hbqt_par_QTimeLine( 1 );
   if( p )
      ( p )->setFrameRange( hb_parni( 2 ), hb_parni( 3 ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QTIMELINE_SETFRAMERANGE FP=( p )->setFrameRange( hb_parni( 2 ), hb_parni( 3 ) ); p is NULL" ) );
   }
}

/*
 * void setLoopCount ( int count )
 */
HB_FUNC( QT_QTIMELINE_SETLOOPCOUNT )
{
   QTimeLine * p = hbqt_par_QTimeLine( 1 );
   if( p )
      ( p )->setLoopCount( hb_parni( 2 ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QTIMELINE_SETLOOPCOUNT FP=( p )->setLoopCount( hb_parni( 2 ) ); p is NULL" ) );
   }
}

/*
 * void setStartFrame ( int frame )
 */
HB_FUNC( QT_QTIMELINE_SETSTARTFRAME )
{
   QTimeLine * p = hbqt_par_QTimeLine( 1 );
   if( p )
      ( p )->setStartFrame( hb_parni( 2 ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QTIMELINE_SETSTARTFRAME FP=( p )->setStartFrame( hb_parni( 2 ) ); p is NULL" ) );
   }
}

/*
 * void setUpdateInterval ( int interval )
 */
HB_FUNC( QT_QTIMELINE_SETUPDATEINTERVAL )
{
   QTimeLine * p = hbqt_par_QTimeLine( 1 );
   if( p )
      ( p )->setUpdateInterval( hb_parni( 2 ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QTIMELINE_SETUPDATEINTERVAL FP=( p )->setUpdateInterval( hb_parni( 2 ) ); p is NULL" ) );
   }
}

/*
 * int startFrame () const
 */
HB_FUNC( QT_QTIMELINE_STARTFRAME )
{
   QTimeLine * p = hbqt_par_QTimeLine( 1 );
   if( p )
      hb_retni( ( p )->startFrame() );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QTIMELINE_STARTFRAME FP=hb_retni( ( p )->startFrame() ); p is NULL" ) );
   }
}

/*
 * State state () const
 */
HB_FUNC( QT_QTIMELINE_STATE )
{
   QTimeLine * p = hbqt_par_QTimeLine( 1 );
   if( p )
      hb_retni( ( QTimeLine::State ) ( p )->state() );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QTIMELINE_STATE FP=hb_retni( ( QTimeLine::State ) ( p )->state() ); p is NULL" ) );
   }
}

/*
 * int updateInterval () const
 */
HB_FUNC( QT_QTIMELINE_UPDATEINTERVAL )
{
   QTimeLine * p = hbqt_par_QTimeLine( 1 );
   if( p )
      hb_retni( ( p )->updateInterval() );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QTIMELINE_UPDATEINTERVAL FP=hb_retni( ( p )->updateInterval() ); p is NULL" ) );
   }
}

/*
 * virtual qreal valueForTime ( int msec ) const
 */
HB_FUNC( QT_QTIMELINE_VALUEFORTIME )
{
   QTimeLine * p = hbqt_par_QTimeLine( 1 );
   if( p )
      hb_retnd( ( p )->valueForTime( hb_parni( 2 ) ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QTIMELINE_VALUEFORTIME FP=hb_retnd( ( p )->valueForTime( hb_parni( 2 ) ) ); p is NULL" ) );
   }
}

/*
 * void resume ()
 */
HB_FUNC( QT_QTIMELINE_RESUME )
{
   QTimeLine * p = hbqt_par_QTimeLine( 1 );
   if( p )
      ( p )->resume();
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QTIMELINE_RESUME FP=( p )->resume(); p is NULL" ) );
   }
}

/*
 * void setCurrentTime ( int msec )
 */
HB_FUNC( QT_QTIMELINE_SETCURRENTTIME )
{
   QTimeLine * p = hbqt_par_QTimeLine( 1 );
   if( p )
      ( p )->setCurrentTime( hb_parni( 2 ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QTIMELINE_SETCURRENTTIME FP=( p )->setCurrentTime( hb_parni( 2 ) ); p is NULL" ) );
   }
}

/*
 * void setPaused ( bool paused )
 */
HB_FUNC( QT_QTIMELINE_SETPAUSED )
{
   QTimeLine * p = hbqt_par_QTimeLine( 1 );
   if( p )
      ( p )->setPaused( hb_parl( 2 ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QTIMELINE_SETPAUSED FP=( p )->setPaused( hb_parl( 2 ) ); p is NULL" ) );
   }
}

/*
 * void start ()
 */
HB_FUNC( QT_QTIMELINE_START )
{
   QTimeLine * p = hbqt_par_QTimeLine( 1 );
   if( p )
      ( p )->start();
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QTIMELINE_START FP=( p )->start(); p is NULL" ) );
   }
}

/*
 * void stop ()
 */
HB_FUNC( QT_QTIMELINE_STOP )
{
   QTimeLine * p = hbqt_par_QTimeLine( 1 );
   if( p )
      ( p )->stop();
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QTIMELINE_STOP FP=( p )->stop(); p is NULL" ) );
   }
}

/*
 * void toggleDirection ()
 */
HB_FUNC( QT_QTIMELINE_TOGGLEDIRECTION )
{
   QTimeLine * p = hbqt_par_QTimeLine( 1 );
   if( p )
      ( p )->toggleDirection();
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QTIMELINE_TOGGLEDIRECTION FP=( p )->toggleDirection(); p is NULL" ) );
   }
}


/*----------------------------------------------------------------------*/
#endif             /* #if QT_VERSION >= 0x040500 */
/*----------------------------------------------------------------------*/
