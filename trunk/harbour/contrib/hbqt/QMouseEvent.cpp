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
#include "hbqt.h"

/*----------------------------------------------------------------------*/
#if QT_VERSION >= 0x040500
/*----------------------------------------------------------------------*/


#include <QtGui/QMouseEvent>


/*
 * QMouseEvent ( Type type, const QPoint & position, Qt::MouseButton button, Qt::MouseButtons buttons, Qt::KeyboardModifiers modifiers )
 * QMouseEvent ( Type type, const QPoint & pos, const QPoint & globalPos, Qt::MouseButton button, Qt::MouseButtons buttons, Qt::KeyboardModifiers modifiers )
 * ~QMouseEvent ()
 */
HB_FUNC( QT_QMOUSEEVENT )
{
   hb_retptr( ( QMouseEvent * ) new QMouseEvent( *hbqt_par_QMouseEvent( 1 ) ) );
}

/*
 * DESTRUCTOR
 */
HB_FUNC( QT_QMOUSEEVENT_DESTROY )
{
   delete hbqt_par_QMouseEvent( 1 );
}

/*
 * Qt::MouseButton button () const
 */
HB_FUNC( QT_QMOUSEEVENT_BUTTON )
{
   hb_retni( ( Qt::MouseButton ) hbqt_par_QMouseEvent( 1 )->button() );
}

/*
 * Qt::MouseButtons buttons () const
 */
HB_FUNC( QT_QMOUSEEVENT_BUTTONS )
{
   hb_retni( ( Qt::MouseButtons ) hbqt_par_QMouseEvent( 1 )->buttons() );
}

/*
 * const QPoint & globalPos () const
 */
HB_FUNC( QT_QMOUSEEVENT_GLOBALPOS )
{
   hb_retptr( new QPoint( hbqt_par_QMouseEvent( 1 )->globalPos() ) );
}

/*
 * int globalX () const
 */
HB_FUNC( QT_QMOUSEEVENT_GLOBALX )
{
   hb_retni( hbqt_par_QMouseEvent( 1 )->globalX() );
}

/*
 * int globalY () const
 */
HB_FUNC( QT_QMOUSEEVENT_GLOBALY )
{
   hb_retni( hbqt_par_QMouseEvent( 1 )->globalY() );
}

/*
 * const QPoint & pos () const
 */
HB_FUNC( QT_QMOUSEEVENT_POS )
{
   hb_retptr( new QPoint( hbqt_par_QMouseEvent( 1 )->pos() ) );
}

/*
 * QPointF posF () const
 */
HB_FUNC( QT_QMOUSEEVENT_POSF )
{
   hb_retptr( new QPointF( hbqt_par_QMouseEvent( 1 )->posF() ) );
}

/*
 * int x () const
 */
HB_FUNC( QT_QMOUSEEVENT_X )
{
   hb_retni( hbqt_par_QMouseEvent( 1 )->x() );
}

/*
 * int y () const
 */
HB_FUNC( QT_QMOUSEEVENT_Y )
{
   hb_retni( hbqt_par_QMouseEvent( 1 )->y() );
}


/*----------------------------------------------------------------------*/
#endif             /* #if QT_VERSION >= 0x040500 */
/*----------------------------------------------------------------------*/
