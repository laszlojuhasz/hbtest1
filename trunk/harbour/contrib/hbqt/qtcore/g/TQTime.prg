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


FUNCTION QTime( ... )
   RETURN HB_QTime():new( ... )

FUNCTION QTimeFrom( ... )
   RETURN HB_QTime():from( ... )

FUNCTION QTimeFromPointer( ... )
   RETURN HB_QTime():fromPointer( ... )


CREATE CLASS QTime INHERIT HbQtObjectHandler FUNCTION HB_QTime

   METHOD  new( ... )

   METHOD  addMSecs                      // ( nMs )                                            -> oQTime
   METHOD  addSecs                       // ( nS )                                             -> oQTime
   METHOD  elapsed                       // (  )                                               -> nInt
   METHOD  hour                          // (  )                                               -> nInt
   METHOD  isNull                        // (  )                                               -> lBool
   METHOD  isValid                       // (  )                                               -> lBool
   METHOD  minute                        // (  )                                               -> nInt
   METHOD  msec                          // (  )                                               -> nInt
   METHOD  msecsTo                       // ( oQTime )                                         -> nInt
   METHOD  restart                       // (  )                                               -> nInt
   METHOD  second                        // (  )                                               -> nInt
   METHOD  secsTo                        // ( oQTime )                                         -> nInt
   METHOD  setHMS                        // ( nH, nM, nS, nMs )                                -> lBool
   METHOD  start                         // (  )                                               -> NIL
   METHOD  toString                      // ( cFormat )                                        -> cQString
                                         // ( nFormat )                                        -> cQString
   METHOD  currentTime                   // (  )                                               -> oQTime
   METHOD  fromString                    // ( cString, nFormat )                               -> oQTime
                                         // ( cString, cFormat )                               -> oQTime
                                         // ( nH, nM, nS, nMs )                                -> lBool

   ENDCLASS


METHOD QTime:new( ... )
   LOCAL p
   FOR EACH p IN { ... }
      hb_pvalue( p:__enumIndex(), hbqt_ptr( p ) )
   NEXT
   ::pPtr := Qt_QTime( ... )
   RETURN Self


METHOD QTime:addMSecs( ... )
   SWITCH PCount()
   CASE 1
      DO CASE
      CASE hb_isNumeric( hb_pvalue( 1 ) )
         RETURN QTimeFromPointer( Qt_QTime_addMSecs( ::pPtr, ... ) )
      ENDCASE
      EXIT
   ENDSWITCH
   RETURN hbqt_error()


METHOD QTime:addSecs( ... )
   SWITCH PCount()
   CASE 1
      DO CASE
      CASE hb_isNumeric( hb_pvalue( 1 ) )
         RETURN QTimeFromPointer( Qt_QTime_addSecs( ::pPtr, ... ) )
      ENDCASE
      EXIT
   ENDSWITCH
   RETURN hbqt_error()


METHOD QTime:elapsed( ... )
   SWITCH PCount()
   CASE 0
      RETURN Qt_QTime_elapsed( ::pPtr, ... )
   ENDSWITCH
   RETURN hbqt_error()


METHOD QTime:hour( ... )
   SWITCH PCount()
   CASE 0
      RETURN Qt_QTime_hour( ::pPtr, ... )
   ENDSWITCH
   RETURN hbqt_error()


METHOD QTime:isNull( ... )
   SWITCH PCount()
   CASE 0
      RETURN Qt_QTime_isNull( ::pPtr, ... )
   ENDSWITCH
   RETURN hbqt_error()


METHOD QTime:isValid( ... )
   SWITCH PCount()
   CASE 4
      DO CASE
      CASE hb_isNumeric( hb_pvalue( 1 ) ) .AND. hb_isNumeric( hb_pvalue( 2 ) ) .AND. hb_isNumeric( hb_pvalue( 3 ) ) .AND. hb_isNumeric( hb_pvalue( 4 ) )
         RETURN Qt_QTime_isValid_1( ::pPtr, ... )
      ENDCASE
      EXIT
   CASE 3
      DO CASE
      CASE hb_isNumeric( hb_pvalue( 1 ) ) .AND. hb_isNumeric( hb_pvalue( 2 ) ) .AND. hb_isNumeric( hb_pvalue( 3 ) )
         RETURN Qt_QTime_isValid_1( ::pPtr, ... )
      ENDCASE
      EXIT
   CASE 0
      RETURN Qt_QTime_isValid( ::pPtr, ... )
   ENDSWITCH
   RETURN hbqt_error()


METHOD QTime:minute( ... )
   SWITCH PCount()
   CASE 0
      RETURN Qt_QTime_minute( ::pPtr, ... )
   ENDSWITCH
   RETURN hbqt_error()


METHOD QTime:msec( ... )
   SWITCH PCount()
   CASE 0
      RETURN Qt_QTime_msec( ::pPtr, ... )
   ENDSWITCH
   RETURN hbqt_error()


METHOD QTime:msecsTo( ... )
   SWITCH PCount()
   CASE 1
      DO CASE
      CASE hb_isObject( hb_pvalue( 1 ) )
         RETURN Qt_QTime_msecsTo( ::pPtr, ... )
      ENDCASE
      EXIT
   ENDSWITCH
   RETURN hbqt_error()


METHOD QTime:restart( ... )
   SWITCH PCount()
   CASE 0
      RETURN Qt_QTime_restart( ::pPtr, ... )
   ENDSWITCH
   RETURN hbqt_error()


METHOD QTime:second( ... )
   SWITCH PCount()
   CASE 0
      RETURN Qt_QTime_second( ::pPtr, ... )
   ENDSWITCH
   RETURN hbqt_error()


METHOD QTime:secsTo( ... )
   SWITCH PCount()
   CASE 1
      DO CASE
      CASE hb_isObject( hb_pvalue( 1 ) )
         RETURN Qt_QTime_secsTo( ::pPtr, ... )
      ENDCASE
      EXIT
   ENDSWITCH
   RETURN hbqt_error()


METHOD QTime:setHMS( ... )
   SWITCH PCount()
   CASE 4
      DO CASE
      CASE hb_isNumeric( hb_pvalue( 1 ) ) .AND. hb_isNumeric( hb_pvalue( 2 ) ) .AND. hb_isNumeric( hb_pvalue( 3 ) ) .AND. hb_isNumeric( hb_pvalue( 4 ) )
         RETURN Qt_QTime_setHMS( ::pPtr, ... )
      ENDCASE
      EXIT
   CASE 3
      DO CASE
      CASE hb_isNumeric( hb_pvalue( 1 ) ) .AND. hb_isNumeric( hb_pvalue( 2 ) ) .AND. hb_isNumeric( hb_pvalue( 3 ) )
         RETURN Qt_QTime_setHMS( ::pPtr, ... )
      ENDCASE
      EXIT
   ENDSWITCH
   RETURN hbqt_error()


METHOD QTime:start( ... )
   SWITCH PCount()
   CASE 0
      RETURN Qt_QTime_start( ::pPtr, ... )
   ENDSWITCH
   RETURN hbqt_error()


METHOD QTime:toString( ... )
   SWITCH PCount()
   CASE 1
      DO CASE
      CASE hb_isChar( hb_pvalue( 1 ) )
         RETURN Qt_QTime_toString( ::pPtr, ... )
      CASE hb_isNumeric( hb_pvalue( 1 ) )
         RETURN Qt_QTime_toString_1( ::pPtr, ... )
      ENDCASE
      EXIT
   CASE 0
      RETURN Qt_QTime_toString_1( ::pPtr, ... )
   ENDSWITCH
   RETURN hbqt_error()


METHOD QTime:currentTime( ... )
   SWITCH PCount()
   CASE 0
      RETURN QTimeFromPointer( Qt_QTime_currentTime( ::pPtr, ... ) )
   ENDSWITCH
   RETURN hbqt_error()


METHOD QTime:fromString( ... )
   SWITCH PCount()
   CASE 2
      DO CASE
      CASE hb_isChar( hb_pvalue( 1 ) ) .AND. hb_isChar( hb_pvalue( 2 ) )
         RETURN QTimeFromPointer( Qt_QTime_fromString_1( ::pPtr, ... ) )
      CASE hb_isChar( hb_pvalue( 1 ) ) .AND. hb_isNumeric( hb_pvalue( 2 ) )
         RETURN QTimeFromPointer( Qt_QTime_fromString( ::pPtr, ... ) )
      ENDCASE
      EXIT
   CASE 1
      DO CASE
      CASE hb_isChar( hb_pvalue( 1 ) )
         RETURN QTimeFromPointer( Qt_QTime_fromString( ::pPtr, ... ) )
      ENDCASE
      EXIT
   ENDSWITCH
   RETURN hbqt_error()

