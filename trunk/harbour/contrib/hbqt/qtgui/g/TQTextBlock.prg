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


FUNCTION QTextBlock( ... )
   RETURN HB_QTextBlock():new( ... )

FUNCTION QTextBlockFromPointer( ... )
   RETURN HB_QTextBlock():fromPointer( ... )


CREATE CLASS QTextBlock INHERIT HbQtObjectHandler FUNCTION HB_QTextBlock

   METHOD  new( ... )

   METHOD  blockFormat                   // (  )                                               -> oQTextBlockFormat
   METHOD  blockFormatIndex              // (  )                                               -> nInt
   METHOD  blockNumber                   // (  )                                               -> nInt
   METHOD  charFormat                    // (  )                                               -> oQTextCharFormat
   METHOD  charFormatIndex               // (  )                                               -> nInt
   METHOD  clearLayout                   // (  )                                               -> NIL
   METHOD  contains                      // ( nPosition )                                      -> lBool
   METHOD  document                      // (  )                                               -> oQTextDocument
   METHOD  firstLineNumber               // (  )                                               -> nInt
   METHOD  isValid                       // (  )                                               -> lBool
   METHOD  isVisible                     // (  )                                               -> lBool
   METHOD  layout                        // (  )                                               -> oQTextLayout
   METHOD  length                        // (  )                                               -> nInt
   METHOD  lineCount                     // (  )                                               -> nInt
   METHOD  next                          // (  )                                               -> oQTextBlock
   METHOD  position                      // (  )                                               -> nInt
   METHOD  previous                      // (  )                                               -> oQTextBlock
   METHOD  revision                      // (  )                                               -> nInt
   METHOD  setLineCount                  // ( nCount )                                         -> NIL
   METHOD  setRevision                   // ( nRev )                                           -> NIL
   METHOD  setUserData                   // ( oHBQTextBlockUserData )                          -> NIL
   METHOD  setUserState                  // ( nState )                                         -> NIL
   METHOD  setVisible                    // ( lVisible )                                       -> NIL
   METHOD  text                          // (  )                                               -> cQString
   METHOD  textList                      // (  )                                               -> oQTextList
   METHOD  userData                      // (  )                                               -> oHBQTextBlockUserData
   METHOD  userState                     // (  )                                               -> nInt

   ENDCLASS


METHOD QTextBlock:new( ... )
   LOCAL p
   FOR EACH p IN { ... }
      hb_pvalue( p:__enumIndex(), __hbqt_ptr( p ) )
   NEXT
   ::pPtr := Qt_QTextBlock( ... )
   RETURN Self


METHOD QTextBlock:blockFormat( ... )
   SWITCH PCount()
   CASE 0
      RETURN QTextBlockFormatFromPointer( Qt_QTextBlock_blockFormat( ::pPtr, ... ) )
   ENDSWITCH
   RETURN __hbqt_error()


METHOD QTextBlock:blockFormatIndex( ... )
   SWITCH PCount()
   CASE 0
      RETURN Qt_QTextBlock_blockFormatIndex( ::pPtr, ... )
   ENDSWITCH
   RETURN __hbqt_error()


METHOD QTextBlock:blockNumber( ... )
   SWITCH PCount()
   CASE 0
      RETURN Qt_QTextBlock_blockNumber( ::pPtr, ... )
   ENDSWITCH
   RETURN __hbqt_error()


METHOD QTextBlock:charFormat( ... )
   SWITCH PCount()
   CASE 0
      RETURN QTextCharFormatFromPointer( Qt_QTextBlock_charFormat( ::pPtr, ... ) )
   ENDSWITCH
   RETURN __hbqt_error()


METHOD QTextBlock:charFormatIndex( ... )
   SWITCH PCount()
   CASE 0
      RETURN Qt_QTextBlock_charFormatIndex( ::pPtr, ... )
   ENDSWITCH
   RETURN __hbqt_error()


METHOD QTextBlock:clearLayout( ... )
   SWITCH PCount()
   CASE 0
      RETURN Qt_QTextBlock_clearLayout( ::pPtr, ... )
   ENDSWITCH
   RETURN __hbqt_error()


METHOD QTextBlock:contains( ... )
   SWITCH PCount()
   CASE 1
      DO CASE
      CASE hb_isNumeric( hb_pvalue( 1 ) )
         RETURN Qt_QTextBlock_contains( ::pPtr, ... )
      ENDCASE
      EXIT
   ENDSWITCH
   RETURN __hbqt_error()


METHOD QTextBlock:document( ... )
   SWITCH PCount()
   CASE 0
      RETURN QTextDocumentFromPointer( Qt_QTextBlock_document( ::pPtr, ... ) )
   ENDSWITCH
   RETURN __hbqt_error()


METHOD QTextBlock:firstLineNumber( ... )
   SWITCH PCount()
   CASE 0
      RETURN Qt_QTextBlock_firstLineNumber( ::pPtr, ... )
   ENDSWITCH
   RETURN __hbqt_error()


METHOD QTextBlock:isValid( ... )
   SWITCH PCount()
   CASE 0
      RETURN Qt_QTextBlock_isValid( ::pPtr, ... )
   ENDSWITCH
   RETURN __hbqt_error()


METHOD QTextBlock:isVisible( ... )
   SWITCH PCount()
   CASE 0
      RETURN Qt_QTextBlock_isVisible( ::pPtr, ... )
   ENDSWITCH
   RETURN __hbqt_error()


METHOD QTextBlock:layout( ... )
   SWITCH PCount()
   CASE 0
      RETURN QTextLayoutFromPointer( Qt_QTextBlock_layout( ::pPtr, ... ) )
   ENDSWITCH
   RETURN __hbqt_error()


METHOD QTextBlock:length( ... )
   SWITCH PCount()
   CASE 0
      RETURN Qt_QTextBlock_length( ::pPtr, ... )
   ENDSWITCH
   RETURN __hbqt_error()


METHOD QTextBlock:lineCount( ... )
   SWITCH PCount()
   CASE 0
      RETURN Qt_QTextBlock_lineCount( ::pPtr, ... )
   ENDSWITCH
   RETURN __hbqt_error()


METHOD QTextBlock:next( ... )
   SWITCH PCount()
   CASE 0
      RETURN QTextBlockFromPointer( Qt_QTextBlock_next( ::pPtr, ... ) )
   ENDSWITCH
   RETURN __hbqt_error()


METHOD QTextBlock:position( ... )
   SWITCH PCount()
   CASE 0
      RETURN Qt_QTextBlock_position( ::pPtr, ... )
   ENDSWITCH
   RETURN __hbqt_error()


METHOD QTextBlock:previous( ... )
   SWITCH PCount()
   CASE 0
      RETURN QTextBlockFromPointer( Qt_QTextBlock_previous( ::pPtr, ... ) )
   ENDSWITCH
   RETURN __hbqt_error()


METHOD QTextBlock:revision( ... )
   SWITCH PCount()
   CASE 0
      RETURN Qt_QTextBlock_revision( ::pPtr, ... )
   ENDSWITCH
   RETURN __hbqt_error()


METHOD QTextBlock:setLineCount( ... )
   SWITCH PCount()
   CASE 1
      DO CASE
      CASE hb_isNumeric( hb_pvalue( 1 ) )
         RETURN Qt_QTextBlock_setLineCount( ::pPtr, ... )
      ENDCASE
      EXIT
   ENDSWITCH
   RETURN __hbqt_error()


METHOD QTextBlock:setRevision( ... )
   SWITCH PCount()
   CASE 1
      DO CASE
      CASE hb_isNumeric( hb_pvalue( 1 ) )
         RETURN Qt_QTextBlock_setRevision( ::pPtr, ... )
      ENDCASE
      EXIT
   ENDSWITCH
   RETURN __hbqt_error()


METHOD QTextBlock:setUserData( ... )
   SWITCH PCount()
   CASE 1
      DO CASE
      CASE hb_isObject( hb_pvalue( 1 ) )
         RETURN Qt_QTextBlock_setUserData( ::pPtr, ... )
      ENDCASE
      EXIT
   ENDSWITCH
   RETURN __hbqt_error()


METHOD QTextBlock:setUserState( ... )
   SWITCH PCount()
   CASE 1
      DO CASE
      CASE hb_isNumeric( hb_pvalue( 1 ) )
         RETURN Qt_QTextBlock_setUserState( ::pPtr, ... )
      ENDCASE
      EXIT
   ENDSWITCH
   RETURN __hbqt_error()


METHOD QTextBlock:setVisible( ... )
   SWITCH PCount()
   CASE 1
      DO CASE
      CASE hb_isLogical( hb_pvalue( 1 ) )
         RETURN Qt_QTextBlock_setVisible( ::pPtr, ... )
      ENDCASE
      EXIT
   ENDSWITCH
   RETURN __hbqt_error()


METHOD QTextBlock:text( ... )
   SWITCH PCount()
   CASE 0
      RETURN Qt_QTextBlock_text( ::pPtr, ... )
   ENDSWITCH
   RETURN __hbqt_error()


METHOD QTextBlock:textList( ... )
   SWITCH PCount()
   CASE 0
      RETURN QTextListFromPointer( Qt_QTextBlock_textList( ::pPtr, ... ) )
   ENDSWITCH
   RETURN __hbqt_error()


METHOD QTextBlock:userData( ... )
   SWITCH PCount()
   CASE 0
      RETURN HBQTextBlockUserDataFromPointer( Qt_QTextBlock_userData( ::pPtr, ... ) )
   ENDSWITCH
   RETURN __hbqt_error()


METHOD QTextBlock:userState( ... )
   SWITCH PCount()
   CASE 0
      RETURN Qt_QTextBlock_userState( ::pPtr, ... )
   ENDSWITCH
   RETURN __hbqt_error()

