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


FUNCTION QTextLine( ... )
   RETURN HB_QTextLine():new( ... )

FUNCTION QTextLineFrom( ... )
   RETURN HB_QTextLine():from( ... )

FUNCTION QTextLineFromPointer( ... )
   RETURN HB_QTextLine():fromPointer( ... )


CREATE CLASS QTextLine INHERIT HbQtObjectHandler FUNCTION HB_QTextLine

   METHOD  new( ... )

   METHOD  ascent                        // (  )                                               -> nQreal
   METHOD  cursorToX                     // ( @nCursorPos, nEdge )                             -> nQreal
                                         // ( nCursorPos, nEdge )                              -> nQreal
   METHOD  descent                       // (  )                                               -> nQreal
   METHOD  height                        // (  )                                               -> nQreal
   METHOD  isValid                       // (  )                                               -> lBool
   METHOD  lineNumber                    // (  )                                               -> nInt
   METHOD  naturalTextRect               // (  )                                               -> oQRectF
   METHOD  naturalTextWidth              // (  )                                               -> nQreal
   METHOD  position                      // (  )                                               -> oQPointF
   METHOD  rect                          // (  )                                               -> oQRectF
   METHOD  setLineWidth                  // ( nWidth )                                         -> NIL
   METHOD  setNumColumns                 // ( nNumColumns )                                    -> NIL
                                         // ( nNumColumns, nAlignmentWidth )                   -> NIL
   METHOD  setPosition                   // ( oQPointF )                                       -> NIL
   METHOD  textLength                    // (  )                                               -> nInt
   METHOD  textStart                     // (  )                                               -> nInt
   METHOD  width                         // (  )                                               -> nQreal
   METHOD  x                             // (  )                                               -> nQreal
   METHOD  xToCursor                     // ( nX, nCpos )                                      -> nInt
   METHOD  y                             // (  )                                               -> nQreal

   ENDCLASS


METHOD QTextLine:new( ... )
   LOCAL p
   FOR EACH p IN { ... }
      hb_pvalue( p:__enumIndex(), __hbqt_ptr( p ) )
   NEXT
   ::pPtr := Qt_QTextLine( ... )
   RETURN Self


METHOD QTextLine:ascent( ... )
   SWITCH PCount()
   CASE 0
      RETURN Qt_QTextLine_ascent( ::pPtr, ... )
   ENDSWITCH
   RETURN __hbqt_error()


METHOD QTextLine:cursorToX( ... )
   SWITCH PCount()
   CASE 2
      DO CASE
      CASE hb_isNumeric( hb_pvalue( 1 ) ) .AND. hb_isNumeric( hb_pvalue( 2 ) )
         RETURN Qt_QTextLine_cursorToX( ::pPtr, ... )
         // RETURN Qt_QTextLine_cursorToX_1( ::pPtr, ... )
      ENDCASE
      EXIT
   CASE 1
      DO CASE
      CASE hb_isNumeric( hb_pvalue( 1 ) )
         RETURN Qt_QTextLine_cursorToX( ::pPtr, ... )
         // RETURN Qt_QTextLine_cursorToX_1( ::pPtr, ... )
      ENDCASE
      EXIT
   ENDSWITCH
   RETURN __hbqt_error()


METHOD QTextLine:descent( ... )
   SWITCH PCount()
   CASE 0
      RETURN Qt_QTextLine_descent( ::pPtr, ... )
   ENDSWITCH
   RETURN __hbqt_error()


METHOD QTextLine:height( ... )
   SWITCH PCount()
   CASE 0
      RETURN Qt_QTextLine_height( ::pPtr, ... )
   ENDSWITCH
   RETURN __hbqt_error()


METHOD QTextLine:isValid( ... )
   SWITCH PCount()
   CASE 0
      RETURN Qt_QTextLine_isValid( ::pPtr, ... )
   ENDSWITCH
   RETURN __hbqt_error()


METHOD QTextLine:lineNumber( ... )
   SWITCH PCount()
   CASE 0
      RETURN Qt_QTextLine_lineNumber( ::pPtr, ... )
   ENDSWITCH
   RETURN __hbqt_error()


METHOD QTextLine:naturalTextRect( ... )
   SWITCH PCount()
   CASE 0
      RETURN QRectFFromPointer( Qt_QTextLine_naturalTextRect( ::pPtr, ... ) )
   ENDSWITCH
   RETURN __hbqt_error()


METHOD QTextLine:naturalTextWidth( ... )
   SWITCH PCount()
   CASE 0
      RETURN Qt_QTextLine_naturalTextWidth( ::pPtr, ... )
   ENDSWITCH
   RETURN __hbqt_error()


METHOD QTextLine:position( ... )
   SWITCH PCount()
   CASE 0
      RETURN QPointFFromPointer( Qt_QTextLine_position( ::pPtr, ... ) )
   ENDSWITCH
   RETURN __hbqt_error()


METHOD QTextLine:rect( ... )
   SWITCH PCount()
   CASE 0
      RETURN QRectFFromPointer( Qt_QTextLine_rect( ::pPtr, ... ) )
   ENDSWITCH
   RETURN __hbqt_error()


METHOD QTextLine:setLineWidth( ... )
   SWITCH PCount()
   CASE 1
      DO CASE
      CASE hb_isNumeric( hb_pvalue( 1 ) )
         RETURN Qt_QTextLine_setLineWidth( ::pPtr, ... )
      ENDCASE
      EXIT
   ENDSWITCH
   RETURN __hbqt_error()


METHOD QTextLine:setNumColumns( ... )
   SWITCH PCount()
   CASE 2
      DO CASE
      CASE hb_isNumeric( hb_pvalue( 1 ) ) .AND. hb_isNumeric( hb_pvalue( 2 ) )
         RETURN Qt_QTextLine_setNumColumns_1( ::pPtr, ... )
      ENDCASE
      EXIT
   CASE 1
      DO CASE
      CASE hb_isNumeric( hb_pvalue( 1 ) )
         RETURN Qt_QTextLine_setNumColumns( ::pPtr, ... )
      ENDCASE
      EXIT
   ENDSWITCH
   RETURN __hbqt_error()


METHOD QTextLine:setPosition( ... )
   SWITCH PCount()
   CASE 1
      DO CASE
      CASE hb_isObject( hb_pvalue( 1 ) )
         RETURN Qt_QTextLine_setPosition( ::pPtr, ... )
      ENDCASE
      EXIT
   ENDSWITCH
   RETURN __hbqt_error()


METHOD QTextLine:textLength( ... )
   SWITCH PCount()
   CASE 0
      RETURN Qt_QTextLine_textLength( ::pPtr, ... )
   ENDSWITCH
   RETURN __hbqt_error()


METHOD QTextLine:textStart( ... )
   SWITCH PCount()
   CASE 0
      RETURN Qt_QTextLine_textStart( ::pPtr, ... )
   ENDSWITCH
   RETURN __hbqt_error()


METHOD QTextLine:width( ... )
   SWITCH PCount()
   CASE 0
      RETURN Qt_QTextLine_width( ::pPtr, ... )
   ENDSWITCH
   RETURN __hbqt_error()


METHOD QTextLine:x( ... )
   SWITCH PCount()
   CASE 0
      RETURN Qt_QTextLine_x( ::pPtr, ... )
   ENDSWITCH
   RETURN __hbqt_error()


METHOD QTextLine:xToCursor( ... )
   SWITCH PCount()
   CASE 2
      DO CASE
      CASE hb_isNumeric( hb_pvalue( 1 ) ) .AND. hb_isNumeric( hb_pvalue( 2 ) )
         RETURN Qt_QTextLine_xToCursor( ::pPtr, ... )
      ENDCASE
      EXIT
   CASE 1
      DO CASE
      CASE hb_isNumeric( hb_pvalue( 1 ) )
         RETURN Qt_QTextLine_xToCursor( ::pPtr, ... )
      ENDCASE
      EXIT
   ENDSWITCH
   RETURN __hbqt_error()


METHOD QTextLine:y( ... )
   SWITCH PCount()
   CASE 0
      RETURN Qt_QTextLine_y( ::pPtr, ... )
   ENDSWITCH
   RETURN __hbqt_error()

