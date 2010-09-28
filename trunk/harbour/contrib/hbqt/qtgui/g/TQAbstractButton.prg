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
 * Copyright 2009-2010 Pritpal Bedi <bedipritpal@hotmail.com>
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
/*                            C R E D I T S                             */
/*----------------------------------------------------------------------*/
/*
 * Marcos Antonio Gambeta
 *    for providing first ever prototype parsing methods. Though the current
 *    implementation is diametrically different then what he proposed, still
 *    current code shaped on those footsteps.
 *
 * Viktor Szakats
 *    for directing the project with futuristic vision;
 *    for designing and maintaining a complex build system for hbQT, hbIDE;
 *    for introducing many constructs on PRG and C++ levels;
 *    for streamlining signal/slots and events management classes;
 *
 * Istvan Bisz
 *    for introducing QPointer<> concept in the generator;
 *    for testing the library on numerous accounts;
 *    for showing a way how a GC pointer can be detached;
 *
 * Francesco Perillo
 *    for taking keen interest in hbQT development and peeking the code;
 *    for providing tips here and there to improve the code quality;
 *    for hitting bulls eye to describe why few objects need GC detachment;
 *
 * Carlos Bacco
 *    for implementing HBQT_TYPE_Q*Class enums;
 *    for peeking into the code and suggesting optimization points;
 *
 * Przemyslaw Czerpak
 *    for providing tips and trick to manipulate HVM internals to the best
 *    of its use and always showing a path when we get stuck;
 *    A true tradition of a MASTER...
*/
/*----------------------------------------------------------------------*/


#include "hbclass.ch"


FUNCTION QAbstractButton( ... )
   RETURN HB_QAbstractButton():new( ... )


CREATE CLASS QAbstractButton INHERIT HbQtObjectHandler, HB_QWidget FUNCTION HB_QAbstractButton

   METHOD  new( ... )

   METHOD  autoExclusive()
   METHOD  autoRepeat()
   METHOD  autoRepeatDelay()
   METHOD  autoRepeatInterval()
   METHOD  group()
   METHOD  icon()
   METHOD  iconSize()
   METHOD  isCheckable()
   METHOD  isChecked()
   METHOD  isDown()
   METHOD  setAutoExclusive( lBool )
   METHOD  setAutoRepeat( lBool )
   METHOD  setAutoRepeatDelay( nInt )
   METHOD  setAutoRepeatInterval( nInt )
   METHOD  setCheckable( lBool )
   METHOD  setDown( lBool )
   METHOD  setIcon( pIcon )
   METHOD  setShortcut( pKey )
   METHOD  setText( cText )
   METHOD  shortcut()
   METHOD  text()
   METHOD  animateClick( nMsec )
   METHOD  click()
   METHOD  setChecked( lBool )
   METHOD  setIconSize( pSize )
   METHOD  toggle()

   ENDCLASS


METHOD QAbstractButton:new( ... )
   LOCAL p
   FOR EACH p IN { ... }
      hb_pvalue( p:__enumIndex(), hbqt_ptr( p ) )
   NEXT
   ::pPtr := Qt_QAbstractButton( ... )
   RETURN Self


METHOD QAbstractButton:autoExclusive()
   RETURN Qt_QAbstractButton_autoExclusive( ::pPtr )


METHOD QAbstractButton:autoRepeat()
   RETURN Qt_QAbstractButton_autoRepeat( ::pPtr )


METHOD QAbstractButton:autoRepeatDelay()
   RETURN Qt_QAbstractButton_autoRepeatDelay( ::pPtr )


METHOD QAbstractButton:autoRepeatInterval()
   RETURN Qt_QAbstractButton_autoRepeatInterval( ::pPtr )


METHOD QAbstractButton:group()
   RETURN HB_QButtonGroup():from( Qt_QAbstractButton_group( ::pPtr ) )


METHOD QAbstractButton:icon()
   RETURN HB_QIcon():from( Qt_QAbstractButton_icon( ::pPtr ) )


METHOD QAbstractButton:iconSize()
   RETURN HB_QSize():from( Qt_QAbstractButton_iconSize( ::pPtr ) )


METHOD QAbstractButton:isCheckable()
   RETURN Qt_QAbstractButton_isCheckable( ::pPtr )


METHOD QAbstractButton:isChecked()
   RETURN Qt_QAbstractButton_isChecked( ::pPtr )


METHOD QAbstractButton:isDown()
   RETURN Qt_QAbstractButton_isDown( ::pPtr )


METHOD QAbstractButton:setAutoExclusive( lBool )
   RETURN Qt_QAbstractButton_setAutoExclusive( ::pPtr, lBool )


METHOD QAbstractButton:setAutoRepeat( lBool )
   RETURN Qt_QAbstractButton_setAutoRepeat( ::pPtr, lBool )


METHOD QAbstractButton:setAutoRepeatDelay( nInt )
   RETURN Qt_QAbstractButton_setAutoRepeatDelay( ::pPtr, nInt )


METHOD QAbstractButton:setAutoRepeatInterval( nInt )
   RETURN Qt_QAbstractButton_setAutoRepeatInterval( ::pPtr, nInt )


METHOD QAbstractButton:setCheckable( lBool )
   RETURN Qt_QAbstractButton_setCheckable( ::pPtr, lBool )


METHOD QAbstractButton:setDown( lBool )
   RETURN Qt_QAbstractButton_setDown( ::pPtr, lBool )


METHOD QAbstractButton:setIcon( pIcon )
   RETURN Qt_QAbstractButton_setIcon( ::pPtr, hbqt_ptr( pIcon ) )


METHOD QAbstractButton:setShortcut( pKey )
   RETURN Qt_QAbstractButton_setShortcut( ::pPtr, hbqt_ptr( pKey ) )


METHOD QAbstractButton:setText( cText )
   RETURN Qt_QAbstractButton_setText( ::pPtr, cText )


METHOD QAbstractButton:shortcut()
   RETURN HB_QKeySequence():from( Qt_QAbstractButton_shortcut( ::pPtr ) )


METHOD QAbstractButton:text()
   RETURN Qt_QAbstractButton_text( ::pPtr )


METHOD QAbstractButton:animateClick( nMsec )
   RETURN Qt_QAbstractButton_animateClick( ::pPtr, nMsec )


METHOD QAbstractButton:click()
   RETURN Qt_QAbstractButton_click( ::pPtr )


METHOD QAbstractButton:setChecked( lBool )
   RETURN Qt_QAbstractButton_setChecked( ::pPtr, lBool )


METHOD QAbstractButton:setIconSize( pSize )
   RETURN Qt_QAbstractButton_setIconSize( ::pPtr, hbqt_ptr( pSize ) )


METHOD QAbstractButton:toggle()
   RETURN Qt_QAbstractButton_toggle( ::pPtr )

