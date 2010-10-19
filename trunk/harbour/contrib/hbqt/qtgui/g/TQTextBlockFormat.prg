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


FUNCTION QTextBlockFormat( ... )
   RETURN HB_QTextBlockFormat():new( ... )

FUNCTION QTextBlockFormatFromPointer( ... )
   RETURN HB_QTextBlockFormat():fromPointer( ... )


CREATE CLASS QTextBlockFormat INHERIT HbQtObjectHandler, HB_QTextFormat FUNCTION HB_QTextBlockFormat

   METHOD  new( ... )

   METHOD  alignment                     // (  )                                               -> nQt_Alignment
   METHOD  bottomMargin                  // (  )                                               -> nQreal
   METHOD  indent                        // (  )                                               -> nInt
   METHOD  isValid                       // (  )                                               -> lBool
   METHOD  leftMargin                    // (  )                                               -> nQreal
   METHOD  nonBreakableLines             // (  )                                               -> lBool
   METHOD  pageBreakPolicy               // (  )                                               -> nPageBreakFlags
   METHOD  rightMargin                   // (  )                                               -> nQreal
   METHOD  setAlignment                  // ( nAlignment )                                     -> NIL
   METHOD  setBottomMargin               // ( nMargin )                                        -> NIL
   METHOD  setIndent                     // ( nIndentation )                                   -> NIL
   METHOD  setLeftMargin                 // ( nMargin )                                        -> NIL
   METHOD  setNonBreakableLines          // ( lB )                                             -> NIL
   METHOD  setPageBreakPolicy            // ( nPolicy )                                        -> NIL
   METHOD  setRightMargin                // ( nMargin )                                        -> NIL
   METHOD  setTextIndent                 // ( nIndent )                                        -> NIL
   METHOD  setTopMargin                  // ( nMargin )                                        -> NIL
   METHOD  textIndent                    // (  )                                               -> nQreal
   METHOD  topMargin                     // (  )                                               -> nQreal

   ENDCLASS


METHOD QTextBlockFormat:new( ... )
   LOCAL p
   FOR EACH p IN { ... }
      hb_pvalue( p:__enumIndex(), __hbqt_ptr( p ) )
   NEXT
   ::pPtr := Qt_QTextBlockFormat( ... )
   RETURN Self


METHOD QTextBlockFormat:alignment( ... )
   SWITCH PCount()
   CASE 0
      RETURN Qt_QTextBlockFormat_alignment( ::pPtr, ... )
   ENDSWITCH
   RETURN __hbqt_error()


METHOD QTextBlockFormat:bottomMargin( ... )
   SWITCH PCount()
   CASE 0
      RETURN Qt_QTextBlockFormat_bottomMargin( ::pPtr, ... )
   ENDSWITCH
   RETURN __hbqt_error()


METHOD QTextBlockFormat:indent( ... )
   SWITCH PCount()
   CASE 0
      RETURN Qt_QTextBlockFormat_indent( ::pPtr, ... )
   ENDSWITCH
   RETURN __hbqt_error()


METHOD QTextBlockFormat:isValid( ... )
   SWITCH PCount()
   CASE 0
      RETURN Qt_QTextBlockFormat_isValid( ::pPtr, ... )
   ENDSWITCH
   RETURN __hbqt_error()


METHOD QTextBlockFormat:leftMargin( ... )
   SWITCH PCount()
   CASE 0
      RETURN Qt_QTextBlockFormat_leftMargin( ::pPtr, ... )
   ENDSWITCH
   RETURN __hbqt_error()


METHOD QTextBlockFormat:nonBreakableLines( ... )
   SWITCH PCount()
   CASE 0
      RETURN Qt_QTextBlockFormat_nonBreakableLines( ::pPtr, ... )
   ENDSWITCH
   RETURN __hbqt_error()


METHOD QTextBlockFormat:pageBreakPolicy( ... )
   SWITCH PCount()
   CASE 0
      RETURN Qt_QTextBlockFormat_pageBreakPolicy( ::pPtr, ... )
   ENDSWITCH
   RETURN __hbqt_error()


METHOD QTextBlockFormat:rightMargin( ... )
   SWITCH PCount()
   CASE 0
      RETURN Qt_QTextBlockFormat_rightMargin( ::pPtr, ... )
   ENDSWITCH
   RETURN __hbqt_error()


METHOD QTextBlockFormat:setAlignment( ... )
   SWITCH PCount()
   CASE 1
      DO CASE
      CASE hb_isNumeric( hb_pvalue( 1 ) )
         RETURN Qt_QTextBlockFormat_setAlignment( ::pPtr, ... )
      ENDCASE
      EXIT
   ENDSWITCH
   RETURN __hbqt_error()


METHOD QTextBlockFormat:setBottomMargin( ... )
   SWITCH PCount()
   CASE 1
      DO CASE
      CASE hb_isNumeric( hb_pvalue( 1 ) )
         RETURN Qt_QTextBlockFormat_setBottomMargin( ::pPtr, ... )
      ENDCASE
      EXIT
   ENDSWITCH
   RETURN __hbqt_error()


METHOD QTextBlockFormat:setIndent( ... )
   SWITCH PCount()
   CASE 1
      DO CASE
      CASE hb_isNumeric( hb_pvalue( 1 ) )
         RETURN Qt_QTextBlockFormat_setIndent( ::pPtr, ... )
      ENDCASE
      EXIT
   ENDSWITCH
   RETURN __hbqt_error()


METHOD QTextBlockFormat:setLeftMargin( ... )
   SWITCH PCount()
   CASE 1
      DO CASE
      CASE hb_isNumeric( hb_pvalue( 1 ) )
         RETURN Qt_QTextBlockFormat_setLeftMargin( ::pPtr, ... )
      ENDCASE
      EXIT
   ENDSWITCH
   RETURN __hbqt_error()


METHOD QTextBlockFormat:setNonBreakableLines( ... )
   SWITCH PCount()
   CASE 1
      DO CASE
      CASE hb_isLogical( hb_pvalue( 1 ) )
         RETURN Qt_QTextBlockFormat_setNonBreakableLines( ::pPtr, ... )
      ENDCASE
      EXIT
   ENDSWITCH
   RETURN __hbqt_error()


METHOD QTextBlockFormat:setPageBreakPolicy( ... )
   SWITCH PCount()
   CASE 1
      DO CASE
      CASE hb_isNumeric( hb_pvalue( 1 ) )
         RETURN Qt_QTextBlockFormat_setPageBreakPolicy( ::pPtr, ... )
      ENDCASE
      EXIT
   ENDSWITCH
   RETURN __hbqt_error()


METHOD QTextBlockFormat:setRightMargin( ... )
   SWITCH PCount()
   CASE 1
      DO CASE
      CASE hb_isNumeric( hb_pvalue( 1 ) )
         RETURN Qt_QTextBlockFormat_setRightMargin( ::pPtr, ... )
      ENDCASE
      EXIT
   ENDSWITCH
   RETURN __hbqt_error()


METHOD QTextBlockFormat:setTextIndent( ... )
   SWITCH PCount()
   CASE 1
      DO CASE
      CASE hb_isNumeric( hb_pvalue( 1 ) )
         RETURN Qt_QTextBlockFormat_setTextIndent( ::pPtr, ... )
      ENDCASE
      EXIT
   ENDSWITCH
   RETURN __hbqt_error()


METHOD QTextBlockFormat:setTopMargin( ... )
   SWITCH PCount()
   CASE 1
      DO CASE
      CASE hb_isNumeric( hb_pvalue( 1 ) )
         RETURN Qt_QTextBlockFormat_setTopMargin( ::pPtr, ... )
      ENDCASE
      EXIT
   ENDSWITCH
   RETURN __hbqt_error()


METHOD QTextBlockFormat:textIndent( ... )
   SWITCH PCount()
   CASE 0
      RETURN Qt_QTextBlockFormat_textIndent( ::pPtr, ... )
   ENDSWITCH
   RETURN __hbqt_error()


METHOD QTextBlockFormat:topMargin( ... )
   SWITCH PCount()
   CASE 0
      RETURN Qt_QTextBlockFormat_topMargin( ::pPtr, ... )
   ENDSWITCH
   RETURN __hbqt_error()

