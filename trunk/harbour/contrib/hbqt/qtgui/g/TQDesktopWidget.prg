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


FUNCTION QDesktopWidget( ... )
   RETURN HB_QDesktopWidget():new( ... )

FUNCTION QDesktopWidgetFrom( ... )
   RETURN HB_QDesktopWidget():from( ... )

FUNCTION QDesktopWidgetFromPointer( ... )
   RETURN HB_QDesktopWidget():fromPointer( ... )


CREATE CLASS QDesktopWidget INHERIT HbQtObjectHandler, HB_QWidget FUNCTION HB_QDesktopWidget

   METHOD  new( ... )

   METHOD  availableGeometry             // ( nScreen )                                        -> oQRect
                                         // ( oQWidget )                                       -> oQRect
                                         // ( oQPoint )                                        -> oQRect
   METHOD  isVirtualDesktop              // (  )                                               -> lBool
   METHOD  numScreens                    // (  )                                               -> nInt
   METHOD  primaryScreen                 // (  )                                               -> nInt
   METHOD  screen                        // ( nScreen )                                        -> oQWidget
   METHOD  screenGeometry                // ( nScreen )                                        -> oQRect
                                         // ( oQWidget )                                       -> oQRect
                                         // ( oQPoint )                                        -> oQRect
   METHOD  screenNumber                  // ( oQWidget )                                       -> nInt
                                         // ( oQPoint )                                        -> nInt

   ENDCLASS


METHOD QDesktopWidget:new( ... )
   LOCAL p
   FOR EACH p IN { ... }
      hb_pvalue( p:__enumIndex(), __hbqt_ptr( p ) )
   NEXT
   ::pPtr := Qt_QDesktopWidget( ... )
   RETURN Self


METHOD QDesktopWidget:availableGeometry( ... )
   SWITCH PCount()
   CASE 1
      DO CASE
      CASE hb_isNumeric( hb_pvalue( 1 ) )
         RETURN QRectFromPointer( Qt_QDesktopWidget_availableGeometry( ::pPtr, ... ) )
      CASE hb_isObject( hb_pvalue( 1 ) )
         SWITCH __objGetClsName( hb_pvalue( 1 ) )
         CASE "QWIDGET"
            RETURN QRectFromPointer( Qt_QDesktopWidget_availableGeometry_1( ::pPtr, ... ) )
         CASE "QPOINT"
            RETURN QRectFromPointer( Qt_QDesktopWidget_availableGeometry_2( ::pPtr, ... ) )
         ENDSWITCH
      ENDCASE
      EXIT
   CASE 0
      RETURN QRectFromPointer( Qt_QDesktopWidget_availableGeometry( ::pPtr, ... ) )
   ENDSWITCH
   RETURN __hbqt_error()


METHOD QDesktopWidget:isVirtualDesktop( ... )
   SWITCH PCount()
   CASE 0
      RETURN Qt_QDesktopWidget_isVirtualDesktop( ::pPtr, ... )
   ENDSWITCH
   RETURN __hbqt_error()


METHOD QDesktopWidget:numScreens( ... )
   SWITCH PCount()
   CASE 0
      RETURN Qt_QDesktopWidget_numScreens( ::pPtr, ... )
   ENDSWITCH
   RETURN __hbqt_error()


METHOD QDesktopWidget:primaryScreen( ... )
   SWITCH PCount()
   CASE 0
      RETURN Qt_QDesktopWidget_primaryScreen( ::pPtr, ... )
   ENDSWITCH
   RETURN __hbqt_error()


METHOD QDesktopWidget:screen( ... )
   SWITCH PCount()
   CASE 1
      DO CASE
      CASE hb_isNumeric( hb_pvalue( 1 ) )
         RETURN QWidgetFromPointer( Qt_QDesktopWidget_screen( ::pPtr, ... ) )
      ENDCASE
      EXIT
   CASE 0
      RETURN QWidgetFromPointer( Qt_QDesktopWidget_screen( ::pPtr, ... ) )
   ENDSWITCH
   RETURN __hbqt_error()


METHOD QDesktopWidget:screenGeometry( ... )
   SWITCH PCount()
   CASE 1
      DO CASE
      CASE hb_isNumeric( hb_pvalue( 1 ) )
         RETURN QRectFromPointer( Qt_QDesktopWidget_screenGeometry( ::pPtr, ... ) )
      CASE hb_isObject( hb_pvalue( 1 ) )
         SWITCH __objGetClsName( hb_pvalue( 1 ) )
         CASE "QWIDGET"
            RETURN QRectFromPointer( Qt_QDesktopWidget_screenGeometry_1( ::pPtr, ... ) )
         CASE "QPOINT"
            RETURN QRectFromPointer( Qt_QDesktopWidget_screenGeometry_2( ::pPtr, ... ) )
         ENDSWITCH
      ENDCASE
      EXIT
   CASE 0
      RETURN QRectFromPointer( Qt_QDesktopWidget_screenGeometry( ::pPtr, ... ) )
   ENDSWITCH
   RETURN __hbqt_error()


METHOD QDesktopWidget:screenNumber( ... )
   SWITCH PCount()
   CASE 1
      DO CASE
      CASE hb_isObject( hb_pvalue( 1 ) )
         SWITCH __objGetClsName( hb_pvalue( 1 ) )
         CASE "QWIDGET"
            RETURN Qt_QDesktopWidget_screenNumber( ::pPtr, ... )
         CASE "QPOINT"
            RETURN Qt_QDesktopWidget_screenNumber_1( ::pPtr, ... )
         ENDSWITCH
      ENDCASE
      EXIT
   CASE 0
      RETURN Qt_QDesktopWidget_screenNumber( ::pPtr, ... )
   ENDSWITCH
   RETURN __hbqt_error()

