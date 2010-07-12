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

#include <QtGui/QAbstractButton>


/*
 * QAbstractButton ( QWidget * parent = 0 )
 * ~QAbstractButton ()
 */

typedef struct
{
   QPointer< QAbstractButton > ph;
   bool bNew;
   QT_G_FUNC_PTR func;
   int type;
} QGC_POINTER_QAbstractButton;

QT_G_FUNC( hbqt_gcRelease_QAbstractButton )
{
   HB_SYMBOL_UNUSED( Cargo );
   QGC_POINTER * p = ( QGC_POINTER * ) Cargo;

   if( p && p->bNew )
   {
      p->ph = NULL;
   }
}

void * hbqt_gcAllocate_QAbstractButton( void * pObj, bool bNew )
{
   QGC_POINTER_QAbstractButton * p = ( QGC_POINTER_QAbstractButton * ) hb_gcAllocate( sizeof( QGC_POINTER_QAbstractButton ), hbqt_gcFuncs() );

   new( & p->ph ) QPointer< QAbstractButton >( ( QAbstractButton * ) pObj );
   p->bNew = bNew;
   p->func = hbqt_gcRelease_QAbstractButton;
   p->type = HBQT_TYPE_QAbstractButton;

   if( bNew )
   {
      HB_TRACE( HB_TR_DEBUG, ( "ph=%p    _new_QAbstractButton  under p->pq", pObj ) );
   }
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "ph=%p NOT_new_QAbstractButton", pObj ) );
   }
   return p;
}

HB_FUNC( QT_QABSTRACTBUTTON )
{

}

/*
 * bool autoExclusive () const
 */
HB_FUNC( QT_QABSTRACTBUTTON_AUTOEXCLUSIVE )
{
   QAbstractButton * p = hbqt_par_QAbstractButton( 1 );
   if( p )
      hb_retl( ( p )->autoExclusive() );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QABSTRACTBUTTON_AUTOEXCLUSIVE FP=hb_retl( ( p )->autoExclusive() ); p is NULL" ) );
   }
}

/*
 * bool autoRepeat () const
 */
HB_FUNC( QT_QABSTRACTBUTTON_AUTOREPEAT )
{
   QAbstractButton * p = hbqt_par_QAbstractButton( 1 );
   if( p )
      hb_retl( ( p )->autoRepeat() );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QABSTRACTBUTTON_AUTOREPEAT FP=hb_retl( ( p )->autoRepeat() ); p is NULL" ) );
   }
}

/*
 * int autoRepeatDelay () const
 */
HB_FUNC( QT_QABSTRACTBUTTON_AUTOREPEATDELAY )
{
   QAbstractButton * p = hbqt_par_QAbstractButton( 1 );
   if( p )
      hb_retni( ( p )->autoRepeatDelay() );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QABSTRACTBUTTON_AUTOREPEATDELAY FP=hb_retni( ( p )->autoRepeatDelay() ); p is NULL" ) );
   }
}

/*
 * int autoRepeatInterval () const
 */
HB_FUNC( QT_QABSTRACTBUTTON_AUTOREPEATINTERVAL )
{
   QAbstractButton * p = hbqt_par_QAbstractButton( 1 );
   if( p )
      hb_retni( ( p )->autoRepeatInterval() );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QABSTRACTBUTTON_AUTOREPEATINTERVAL FP=hb_retni( ( p )->autoRepeatInterval() ); p is NULL" ) );
   }
}

/*
 * QButtonGroup * group () const
 */
HB_FUNC( QT_QABSTRACTBUTTON_GROUP )
{
   QAbstractButton * p = hbqt_par_QAbstractButton( 1 );
   if( p )
      hb_retptrGC( hbqt_gcAllocate_QButtonGroup( ( p )->group(), false ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QABSTRACTBUTTON_GROUP FP=hb_retptrGC( hbqt_gcAllocate_QButtonGroup( ( p )->group(), false ) ); p is NULL" ) );
   }
}

/*
 * QIcon icon () const
 */
HB_FUNC( QT_QABSTRACTBUTTON_ICON )
{
   QAbstractButton * p = hbqt_par_QAbstractButton( 1 );
   if( p )
      hb_retptrGC( hbqt_gcAllocate_QIcon( new QIcon( ( p )->icon() ), true ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QABSTRACTBUTTON_ICON FP=hb_retptrGC( hbqt_gcAllocate_QIcon( new QIcon( ( p )->icon() ), true ) ); p is NULL" ) );
   }
}

/*
 * QSize iconSize () const
 */
HB_FUNC( QT_QABSTRACTBUTTON_ICONSIZE )
{
   QAbstractButton * p = hbqt_par_QAbstractButton( 1 );
   if( p )
      hb_retptrGC( hbqt_gcAllocate_QSize( new QSize( ( p )->iconSize() ), true ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QABSTRACTBUTTON_ICONSIZE FP=hb_retptrGC( hbqt_gcAllocate_QSize( new QSize( ( p )->iconSize() ), true ) ); p is NULL" ) );
   }
}

/*
 * bool isCheckable () const
 */
HB_FUNC( QT_QABSTRACTBUTTON_ISCHECKABLE )
{
   QAbstractButton * p = hbqt_par_QAbstractButton( 1 );
   if( p )
      hb_retl( ( p )->isCheckable() );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QABSTRACTBUTTON_ISCHECKABLE FP=hb_retl( ( p )->isCheckable() ); p is NULL" ) );
   }
}

/*
 * bool isChecked () const
 */
HB_FUNC( QT_QABSTRACTBUTTON_ISCHECKED )
{
   QAbstractButton * p = hbqt_par_QAbstractButton( 1 );
   if( p )
      hb_retl( ( p )->isChecked() );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QABSTRACTBUTTON_ISCHECKED FP=hb_retl( ( p )->isChecked() ); p is NULL" ) );
   }
}

/*
 * bool isDown () const
 */
HB_FUNC( QT_QABSTRACTBUTTON_ISDOWN )
{
   QAbstractButton * p = hbqt_par_QAbstractButton( 1 );
   if( p )
      hb_retl( ( p )->isDown() );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QABSTRACTBUTTON_ISDOWN FP=hb_retl( ( p )->isDown() ); p is NULL" ) );
   }
}

/*
 * void setAutoExclusive ( bool )
 */
HB_FUNC( QT_QABSTRACTBUTTON_SETAUTOEXCLUSIVE )
{
   QAbstractButton * p = hbqt_par_QAbstractButton( 1 );
   if( p )
      ( p )->setAutoExclusive( hb_parl( 2 ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QABSTRACTBUTTON_SETAUTOEXCLUSIVE FP=( p )->setAutoExclusive( hb_parl( 2 ) ); p is NULL" ) );
   }
}

/*
 * void setAutoRepeat ( bool )
 */
HB_FUNC( QT_QABSTRACTBUTTON_SETAUTOREPEAT )
{
   QAbstractButton * p = hbqt_par_QAbstractButton( 1 );
   if( p )
      ( p )->setAutoRepeat( hb_parl( 2 ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QABSTRACTBUTTON_SETAUTOREPEAT FP=( p )->setAutoRepeat( hb_parl( 2 ) ); p is NULL" ) );
   }
}

/*
 * void setAutoRepeatDelay ( int )
 */
HB_FUNC( QT_QABSTRACTBUTTON_SETAUTOREPEATDELAY )
{
   QAbstractButton * p = hbqt_par_QAbstractButton( 1 );
   if( p )
      ( p )->setAutoRepeatDelay( hb_parni( 2 ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QABSTRACTBUTTON_SETAUTOREPEATDELAY FP=( p )->setAutoRepeatDelay( hb_parni( 2 ) ); p is NULL" ) );
   }
}

/*
 * void setAutoRepeatInterval ( int )
 */
HB_FUNC( QT_QABSTRACTBUTTON_SETAUTOREPEATINTERVAL )
{
   QAbstractButton * p = hbqt_par_QAbstractButton( 1 );
   if( p )
      ( p )->setAutoRepeatInterval( hb_parni( 2 ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QABSTRACTBUTTON_SETAUTOREPEATINTERVAL FP=( p )->setAutoRepeatInterval( hb_parni( 2 ) ); p is NULL" ) );
   }
}

/*
 * void setCheckable ( bool )
 */
HB_FUNC( QT_QABSTRACTBUTTON_SETCHECKABLE )
{
   QAbstractButton * p = hbqt_par_QAbstractButton( 1 );
   if( p )
      ( p )->setCheckable( hb_parl( 2 ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QABSTRACTBUTTON_SETCHECKABLE FP=( p )->setCheckable( hb_parl( 2 ) ); p is NULL" ) );
   }
}

/*
 * void setDown ( bool )
 */
HB_FUNC( QT_QABSTRACTBUTTON_SETDOWN )
{
   QAbstractButton * p = hbqt_par_QAbstractButton( 1 );
   if( p )
      ( p )->setDown( hb_parl( 2 ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QABSTRACTBUTTON_SETDOWN FP=( p )->setDown( hb_parl( 2 ) ); p is NULL" ) );
   }
}

/*
 * void setIcon ( const QIcon & icon )
 */
HB_FUNC( QT_QABSTRACTBUTTON_SETICON )
{
   QAbstractButton * p = hbqt_par_QAbstractButton( 1 );
   if( p )
      ( p )->setIcon( QIcon( hbqt_par_QString( 2 ) ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QABSTRACTBUTTON_SETICON FP=( p )->setIcon( QIcon( hbqt_par_QString( 2 ) ) ); p is NULL" ) );
   }
}

/*
 * void setShortcut ( const QKeySequence & key )
 */
HB_FUNC( QT_QABSTRACTBUTTON_SETSHORTCUT )
{
   QAbstractButton * p = hbqt_par_QAbstractButton( 1 );
   if( p )
      ( p )->setShortcut( *hbqt_par_QKeySequence( 2 ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QABSTRACTBUTTON_SETSHORTCUT FP=( p )->setShortcut( *hbqt_par_QKeySequence( 2 ) ); p is NULL" ) );
   }
}

/*
 * void setText ( const QString & text )
 */
HB_FUNC( QT_QABSTRACTBUTTON_SETTEXT )
{
   QAbstractButton * p = hbqt_par_QAbstractButton( 1 );
   if( p )
      ( p )->setText( QAbstractButton::tr( hb_parc( 2 ) ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QABSTRACTBUTTON_SETTEXT FP=( p )->setText( QAbstractButton::tr( hb_parc( 2 ) ) ); p is NULL" ) );
   }
}

/*
 * QKeySequence shortcut () const
 */
HB_FUNC( QT_QABSTRACTBUTTON_SHORTCUT )
{
   QAbstractButton * p = hbqt_par_QAbstractButton( 1 );
   if( p )
      hb_retptrGC( hbqt_gcAllocate_QKeySequence( new QKeySequence( ( p )->shortcut() ), true ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QABSTRACTBUTTON_SHORTCUT FP=hb_retptrGC( hbqt_gcAllocate_QKeySequence( new QKeySequence( ( p )->shortcut() ), true ) ); p is NULL" ) );
   }
}

/*
 * QString text () const
 */
HB_FUNC( QT_QABSTRACTBUTTON_TEXT )
{
   QAbstractButton * p = hbqt_par_QAbstractButton( 1 );
   if( p )
      hb_retc( ( p )->text().toAscii().data() );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QABSTRACTBUTTON_TEXT FP=hb_retc( ( p )->text().toAscii().data() ); p is NULL" ) );
   }
}

/*
 * void animateClick ( int msec = 100 )
 */
HB_FUNC( QT_QABSTRACTBUTTON_ANIMATECLICK )
{
   QAbstractButton * p = hbqt_par_QAbstractButton( 1 );
   if( p )
      ( p )->animateClick( hb_parnidef( 2, 100 ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QABSTRACTBUTTON_ANIMATECLICK FP=( p )->animateClick( hb_parnidef( 2, 100 ) ); p is NULL" ) );
   }
}

/*
 * void click ()
 */
HB_FUNC( QT_QABSTRACTBUTTON_CLICK )
{
   QAbstractButton * p = hbqt_par_QAbstractButton( 1 );
   if( p )
      ( p )->click();
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QABSTRACTBUTTON_CLICK FP=( p )->click(); p is NULL" ) );
   }
}

/*
 * void setChecked ( bool )
 */
HB_FUNC( QT_QABSTRACTBUTTON_SETCHECKED )
{
   QAbstractButton * p = hbqt_par_QAbstractButton( 1 );
   if( p )
      ( p )->setChecked( hb_parl( 2 ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QABSTRACTBUTTON_SETCHECKED FP=( p )->setChecked( hb_parl( 2 ) ); p is NULL" ) );
   }
}

/*
 * void setIconSize ( const QSize & size )
 */
HB_FUNC( QT_QABSTRACTBUTTON_SETICONSIZE )
{
   QAbstractButton * p = hbqt_par_QAbstractButton( 1 );
   if( p )
      ( p )->setIconSize( *hbqt_par_QSize( 2 ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QABSTRACTBUTTON_SETICONSIZE FP=( p )->setIconSize( *hbqt_par_QSize( 2 ) ); p is NULL" ) );
   }
}

/*
 * void toggle ()
 */
HB_FUNC( QT_QABSTRACTBUTTON_TOGGLE )
{
   QAbstractButton * p = hbqt_par_QAbstractButton( 1 );
   if( p )
      ( p )->toggle();
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QABSTRACTBUTTON_TOGGLE FP=( p )->toggle(); p is NULL" ) );
   }
}


/*----------------------------------------------------------------------*/
#endif             /* #if QT_VERSION >= 0x040500 */
/*----------------------------------------------------------------------*/
