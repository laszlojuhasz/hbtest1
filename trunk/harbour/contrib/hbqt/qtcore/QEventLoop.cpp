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

/*
 *  enum ProcessEventsFlag { AllEvents, ExcludeUserInputEvents, ExcludeSocketNotifiers, WaitForMoreEvents, DeferredDeletion }
 *  flags ProcessEventsFlags
 */

#include <QtCore/QPointer>

#include <QtCore/QEventLoop>
#include <QtCore/QEvent>

/*
 * QEventLoop ( QObject * parent = 0 )
 * ~QEventLoop ()
 */

QT_G_FUNC( release_QEventLoop )
{
#if defined(__debug__)
   hb_snprintf( str, sizeof(str), "release_QEventLoop" );  OutputDebugString( str );
#endif
   void * ph = ( void * ) Cargo;
   if( ph )
   {
      const QMetaObject * m = ( ( QObject * ) ph )->metaObject();
      if( ( QString ) m->className() != ( QString ) "QObject" )
      {
         delete ( ( QEventLoop * ) ph );
         ph = NULL;
      }
      else
      {
#if defined(__debug__)
   hb_snprintf( str, sizeof(str), "  Object Name Missing: QEventLoop" );  OutputDebugString( str );
#endif
      }
   }
}

HB_FUNC( QT_QEVENTLOOP )
{
   QGC_POINTER * p = ( QGC_POINTER * ) hb_gcAllocate( sizeof( QGC_POINTER ), gcFuncs() );
   QPointer< QEventLoop > pObj = NULL;

   pObj = new QEventLoop( hbqt_par_QObject( 1 ) ) ;

   p->ph = pObj;
   p->func = release_QEventLoop;

   hb_retptrGC( p );
}
/*
 * int exec ( ProcessEventsFlags flags = AllEvents )
 */
HB_FUNC( QT_QEVENTLOOP_EXEC )
{
   hb_retni( hbqt_par_QEventLoop( 1 )->exec( ( HB_ISNUM( 2 ) ? ( QEventLoop::ProcessEventsFlags ) hb_parni( 2 ) : ( QEventLoop::ProcessEventsFlags ) QEventLoop::AllEvents ) ) );
}

/*
 * void exit ( int returnCode = 0 )
 */
HB_FUNC( QT_QEVENTLOOP_EXIT )
{
   hbqt_par_QEventLoop( 1 )->exit( hb_parni( 2 ) );
}

/*
 * bool isRunning () const
 */
HB_FUNC( QT_QEVENTLOOP_ISRUNNING )
{
   hb_retl( hbqt_par_QEventLoop( 1 )->isRunning() );
}

/*
 * bool processEvents ( ProcessEventsFlags flags = AllEvents )
 */
HB_FUNC( QT_QEVENTLOOP_PROCESSEVENTS )
{
   hb_retl( hbqt_par_QEventLoop( 1 )->processEvents( ( HB_ISNUM( 2 ) ? ( QEventLoop::ProcessEventsFlags ) hb_parni( 2 ) : ( QEventLoop::ProcessEventsFlags ) QEventLoop::AllEvents ) ) );
}

/*
 * void processEvents ( ProcessEventsFlags flags, int maxTime )
 */
HB_FUNC( QT_QEVENTLOOP_PROCESSEVENTS_1 )
{
   hbqt_par_QEventLoop( 1 )->processEvents( ( QEventLoop::ProcessEventsFlags ) hb_parni( 2 ), hb_parni( 3 ) );
}

/*
 * void wakeUp ()
 */
HB_FUNC( QT_QEVENTLOOP_WAKEUP )
{
   hbqt_par_QEventLoop( 1 )->wakeUp();
}


/*----------------------------------------------------------------------*/
#endif             /* #if QT_VERSION >= 0x040500 */
/*----------------------------------------------------------------------*/
