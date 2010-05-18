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


#include "hbclass.ch"


CREATE CLASS HBQPlainTextEdit INHERIT HbQtObjectHandler, QPlainTextEdit

   METHOD  new( ... )

   METHOD  hbGetIndex( pCrQTextCursor )
   METHOD  hbGetLine( pCrQTextCursor )
   METHOD  hbLineNumberAreaWidth()
   METHOD  hbGetSpaces()
   METHOD  hbSetSpaces( nNewSpaces )
   METHOD  hbBookmarks( nBlock )
   METHOD  hbNextBookmark( nBlock )
   METHOD  hbPrevBookmark( nBlock )
   METHOD  hbGotoBookmark( nBlock )
   METHOD  hbNumberBlockVisible( lB )
   METHOD  hbNumberBlockVisible_1()
   METHOD  hbHighlightCurrentLine( lB )
   METHOD  hbHighlightCurrentLine_1()
   METHOD  hbSetEventBlock( xBlock )
   METHOD  hbUpdateLineNumberAreaWidth( nNewBlockCount )
   METHOD  hbCaseUpper()
   METHOD  hbCaseLower()
   METHOD  hbEscapeQuotes()
   METHOD  hbEscapeDQuotes()
   METHOD  hbUnescapeQuotes()
   METHOD  hbUnescapeDQuotes()
   METHOD  hbConvertQuotes()
   METHOD  hbConvertDQuotes()
   METHOD  hbBlockComment()
   METHOD  hbStreamComment()
   METHOD  hbDuplicateLine()
   METHOD  hbReplaceSelection( cTxt )
   METHOD  hbBlockIndent( nSteps )
   METHOD  hbDeleteLine()
   METHOD  hbMoveLine( nIDirection )
   METHOD  hbGetSelectedText()
   METHOD  hbTextUnderCursor()
   METHOD  hbShowPrototype( cTip )
   METHOD  hbSetCompleter( pCompleter )
   METHOD  hbSetCurrentLineColor( pColor )
   METHOD  hbSetLineAreaBkColor( pColor )
   METHOD  hbRefresh()
   METHOD  hbCut( nKey )
   METHOD  hbCopy()
   METHOD  hbPaste()
   METHOD  hbSetSelectionMode( nMode, lOn )
   METHOD  hbGetSelectionInfo()
   METHOD  hbSetSelectionInfo( xSelectionInfo )
   METHOD  hbSetSelectionColor( pColor )
   METHOD  hbSetMatchBraces( lAll )
   METHOD  hbGetViewportInfo()
   METHOD  hbApplyKey( nKey, nModifiers, cTxt )

   ENDCLASS


METHOD HBQPlainTextEdit:new( ... )
   LOCAL p
   FOR EACH p IN { ... }
      hb_pvalue( p:__enumIndex(), hbqt_ptr( p ) )
   NEXT
   ::pPtr := Qt_HBQPlainTextEdit( ... )
   RETURN Self


METHOD HBQPlainTextEdit:hbGetIndex( pCrQTextCursor )
   RETURN Qt_HBQPlainTextEdit_hbGetIndex( ::pPtr, hbqt_ptr( pCrQTextCursor ) )


METHOD HBQPlainTextEdit:hbGetLine( pCrQTextCursor )
   RETURN Qt_HBQPlainTextEdit_hbGetLine( ::pPtr, hbqt_ptr( pCrQTextCursor ) )


METHOD HBQPlainTextEdit:hbLineNumberAreaWidth()
   RETURN Qt_HBQPlainTextEdit_hbLineNumberAreaWidth( ::pPtr )


METHOD HBQPlainTextEdit:hbGetSpaces()
   RETURN Qt_HBQPlainTextEdit_hbGetSpaces( ::pPtr )


METHOD HBQPlainTextEdit:hbSetSpaces( nNewSpaces )
   RETURN Qt_HBQPlainTextEdit_hbSetSpaces( ::pPtr, nNewSpaces )


METHOD HBQPlainTextEdit:hbBookmarks( nBlock )
   RETURN Qt_HBQPlainTextEdit_hbBookmarks( ::pPtr, nBlock )


METHOD HBQPlainTextEdit:hbNextBookmark( nBlock )
   RETURN Qt_HBQPlainTextEdit_hbNextBookmark( ::pPtr, nBlock )


METHOD HBQPlainTextEdit:hbPrevBookmark( nBlock )
   RETURN Qt_HBQPlainTextEdit_hbPrevBookmark( ::pPtr, nBlock )


METHOD HBQPlainTextEdit:hbGotoBookmark( nBlock )
   RETURN Qt_HBQPlainTextEdit_hbGotoBookmark( ::pPtr, nBlock )


METHOD HBQPlainTextEdit:hbNumberBlockVisible( lB )
   RETURN Qt_HBQPlainTextEdit_hbNumberBlockVisible( ::pPtr, lB )


METHOD HBQPlainTextEdit:hbNumberBlockVisible_1()
   RETURN Qt_HBQPlainTextEdit_hbNumberBlockVisible_1( ::pPtr )


METHOD HBQPlainTextEdit:hbHighlightCurrentLine( lB )
   RETURN Qt_HBQPlainTextEdit_hbHighlightCurrentLine( ::pPtr, lB )


METHOD HBQPlainTextEdit:hbHighlightCurrentLine_1()
   RETURN Qt_HBQPlainTextEdit_hbHighlightCurrentLine_1( ::pPtr )


METHOD HBQPlainTextEdit:hbSetEventBlock( xBlock )
   RETURN Qt_HBQPlainTextEdit_hbSetEventBlock( ::pPtr, xBlock )


METHOD HBQPlainTextEdit:hbUpdateLineNumberAreaWidth( nNewBlockCount )
   RETURN Qt_HBQPlainTextEdit_hbUpdateLineNumberAreaWidth( ::pPtr, nNewBlockCount )


METHOD HBQPlainTextEdit:hbCaseUpper()
   RETURN Qt_HBQPlainTextEdit_hbCaseUpper( ::pPtr )


METHOD HBQPlainTextEdit:hbCaseLower()
   RETURN Qt_HBQPlainTextEdit_hbCaseLower( ::pPtr )


METHOD HBQPlainTextEdit:hbEscapeQuotes()
   RETURN Qt_HBQPlainTextEdit_hbEscapeQuotes( ::pPtr )


METHOD HBQPlainTextEdit:hbEscapeDQuotes()
   RETURN Qt_HBQPlainTextEdit_hbEscapeDQuotes( ::pPtr )


METHOD HBQPlainTextEdit:hbUnescapeQuotes()
   RETURN Qt_HBQPlainTextEdit_hbUnescapeQuotes( ::pPtr )


METHOD HBQPlainTextEdit:hbUnescapeDQuotes()
   RETURN Qt_HBQPlainTextEdit_hbUnescapeDQuotes( ::pPtr )


METHOD HBQPlainTextEdit:hbConvertQuotes()
   RETURN Qt_HBQPlainTextEdit_hbConvertQuotes( ::pPtr )


METHOD HBQPlainTextEdit:hbConvertDQuotes()
   RETURN Qt_HBQPlainTextEdit_hbConvertDQuotes( ::pPtr )


METHOD HBQPlainTextEdit:hbBlockComment()
   RETURN Qt_HBQPlainTextEdit_hbBlockComment( ::pPtr )


METHOD HBQPlainTextEdit:hbStreamComment()
   RETURN Qt_HBQPlainTextEdit_hbStreamComment( ::pPtr )


METHOD HBQPlainTextEdit:hbDuplicateLine()
   RETURN Qt_HBQPlainTextEdit_hbDuplicateLine( ::pPtr )


METHOD HBQPlainTextEdit:hbReplaceSelection( cTxt )
   RETURN Qt_HBQPlainTextEdit_hbReplaceSelection( ::pPtr, cTxt )


METHOD HBQPlainTextEdit:hbBlockIndent( nSteps )
   RETURN Qt_HBQPlainTextEdit_hbBlockIndent( ::pPtr, nSteps )


METHOD HBQPlainTextEdit:hbDeleteLine()
   RETURN Qt_HBQPlainTextEdit_hbDeleteLine( ::pPtr )


METHOD HBQPlainTextEdit:hbMoveLine( nIDirection )
   RETURN Qt_HBQPlainTextEdit_hbMoveLine( ::pPtr, nIDirection )


METHOD HBQPlainTextEdit:hbGetSelectedText()
   RETURN Qt_HBQPlainTextEdit_hbGetSelectedText( ::pPtr )


METHOD HBQPlainTextEdit:hbTextUnderCursor()
   RETURN Qt_HBQPlainTextEdit_hbTextUnderCursor( ::pPtr )


METHOD HBQPlainTextEdit:hbShowPrototype( cTip )
   RETURN Qt_HBQPlainTextEdit_hbShowPrototype( ::pPtr, cTip )


METHOD HBQPlainTextEdit:hbSetCompleter( pCompleter )
   RETURN Qt_HBQPlainTextEdit_hbSetCompleter( ::pPtr, hbqt_ptr( pCompleter ) )


METHOD HBQPlainTextEdit:hbSetCurrentLineColor( pColor )
   RETURN Qt_HBQPlainTextEdit_hbSetCurrentLineColor( ::pPtr, hbqt_ptr( pColor ) )


METHOD HBQPlainTextEdit:hbSetLineAreaBkColor( pColor )
   RETURN Qt_HBQPlainTextEdit_hbSetLineAreaBkColor( ::pPtr, hbqt_ptr( pColor ) )


METHOD HBQPlainTextEdit:hbRefresh()
   RETURN Qt_HBQPlainTextEdit_hbRefresh( ::pPtr )


METHOD HBQPlainTextEdit:hbCut( nKey )
   RETURN Qt_HBQPlainTextEdit_hbCut( ::pPtr, nKey )


METHOD HBQPlainTextEdit:hbCopy()
   RETURN Qt_HBQPlainTextEdit_hbCopy( ::pPtr )


METHOD HBQPlainTextEdit:hbPaste()
   RETURN Qt_HBQPlainTextEdit_hbPaste( ::pPtr )


METHOD HBQPlainTextEdit:hbSetSelectionMode( nMode, lOn )
   RETURN Qt_HBQPlainTextEdit_hbSetSelectionMode( ::pPtr, nMode, lOn )


METHOD HBQPlainTextEdit:hbGetSelectionInfo()
   RETURN Qt_HBQPlainTextEdit_hbGetSelectionInfo( ::pPtr )


METHOD HBQPlainTextEdit:hbSetSelectionInfo( xSelectionInfo )
   RETURN Qt_HBQPlainTextEdit_hbSetSelectionInfo( ::pPtr, xSelectionInfo )


METHOD HBQPlainTextEdit:hbSetSelectionColor( pColor )
   RETURN Qt_HBQPlainTextEdit_hbSetSelectionColor( ::pPtr, hbqt_ptr( pColor ) )


METHOD HBQPlainTextEdit:hbSetMatchBraces( lAll )
   RETURN Qt_HBQPlainTextEdit_hbSetMatchBraces( ::pPtr, lAll )


METHOD HBQPlainTextEdit:hbGetViewportInfo()
   RETURN Qt_HBQPlainTextEdit_hbGetViewportInfo( ::pPtr )


METHOD HBQPlainTextEdit:hbApplyKey( nKey, nModifiers, cTxt )
   RETURN Qt_HBQPlainTextEdit_hbApplyKey( ::pPtr, nKey, nModifiers, cTxt )

