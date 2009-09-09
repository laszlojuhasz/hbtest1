/*
 * $Id$
 */

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
#include "hbqt.h"

/*----------------------------------------------------------------------*/
#if QT_VERSION >= 0x040500
/*----------------------------------------------------------------------*/


#include <QtGui/QSound>


/*
 * QSound ( const QString & filename, QObject * parent = 0 )
 * ~QSound ()
 */
HB_FUNC( QT_QSOUND )
{
   hb_retptr( ( QSound* ) new QSound( hbqt_par_QString( 1 ) ) );
}

/*
 * DESTRUCTOR
 */
HB_FUNC( QT_QSOUND_DESTROY )
{
   hbqt_par_QSound( 1 )->~QSound();
}

/*
 * QString fileName () const
 */
HB_FUNC( QT_QSOUND_FILENAME )
{
   hb_retc( hbqt_par_QSound( 1 )->fileName().toAscii().data() );
}

/*
 * bool isFinished () const
 */
HB_FUNC( QT_QSOUND_ISFINISHED )
{
   hb_retl( hbqt_par_QSound( 1 )->isFinished() );
}

/*
 * int loops () const
 */
HB_FUNC( QT_QSOUND_LOOPS )
{
   hb_retni( hbqt_par_QSound( 1 )->loops() );
}

/*
 * int loopsRemaining () const
 */
HB_FUNC( QT_QSOUND_LOOPSREMAINING )
{
   hb_retni( hbqt_par_QSound( 1 )->loopsRemaining() );
}

/*
 * void setLoops ( int number )
 */
HB_FUNC( QT_QSOUND_SETLOOPS )
{
   hbqt_par_QSound( 1 )->setLoops( hb_parni( 2 ) );
}

/*
 * bool isAvailable ()
 */
HB_FUNC( QT_QSOUND_ISAVAILABLE )
{
   hb_retl( hbqt_par_QSound( 1 )->isAvailable() );
}

/*
 * void play ( const QString & filename )
 */
HB_FUNC( QT_QSOUND_PLAY )
{
   hbqt_par_QSound( 1 )->play( hbqt_par_QString( 2 ) );
}


/*----------------------------------------------------------------------*/
#endif             /* #if QT_VERSION >= 0x040500 */
/*----------------------------------------------------------------------*/
