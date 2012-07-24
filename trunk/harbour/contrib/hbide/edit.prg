/*
 * $Id$
 */

/*
 * Harbour Project source code:
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
/*----------------------------------------------------------------------*/
/*----------------------------------------------------------------------*/
/*
 *                                EkOnkar
 *                          ( The LORD is ONE )
 *
 *                            Harbour-Qt IDE
 *
 *                  Pritpal Bedi <pritpal@vouchcac.com>
 *                               27Dec2009
 */
/*----------------------------------------------------------------------*/
/*----------------------------------------------------------------------*/
/*----------------------------------------------------------------------*/

#include "common.ch"
#include "hbclass.ch"
#include "hbqtgui.ch"
#include "hbide.ch"
#include "xbp.ch"

/*----------------------------------------------------------------------*/

#define __customContextMenuRequested__            1
#define __textChanged__                           2
#define __copyAvailable__                         3
#define __modificationChanged__                   4
#define __redoAvailable__                         5
#define __selectionChanged__                      6
#define __undoAvailable__                         7
#define __updateRequest__                         8
#define __cursorPositionChanged__                 9

#define __timerTimeout__                          23

#define __selectionMode_stream__                  1
#define __selectionMode_column__                  2
#define __selectionMode_line__                    3

/*----------------------------------------------------------------------*/

CLASS IdeEdit INHERIT IdeObject

   DATA   oEditor

   DATA   qEdit
   DATA   nOrient                                 INIT  0

   DATA   nMode                                   INIT  0
   DATA   nLineNo                                 INIT  -99
   DATA   nMaxDigits                              INIT  5       // Tobe
   DATA   nMaxRows                                INIT  100
   DATA   nLastLine                               INIT  -99
   DATA   nCurLineNo                              INIT  0
   DATA   nPrevLineNo                             INIT  -1
   DATA   nPrevLineNo1                            INIT  -1

   DATA   aBookMarks                              INIT  {}

   DATA   lModified                               INIT  .F.
   DATA   lIndentIt                               INIT  .F.
   DATA   lUpdatePrevWord                         INIT  .F.
   DATA   lCopyWhenDblClicked                     INIT  .F.
   DATA   cCurLineText                            INIT  ""

   DATA   cProto                                  INIT ""
   DATA   cProtoOrg                               INIT ""
   DATA   qTimer
   DATA   nProtoLine                              INIT -1
   DATA   nProtoCol                               INIT -1
   DATA   isSuspended                             INIT .F.

   DATA   nProtoRows                              INIT 1
   DATA   nProtoCols                              INIT 10

   DATA   fontFamily
   DATA   pointSize
   DATA   currentPointSize
   DATA   qFont
   DATA   aBlockCopyContents                      INIT {}
   DATA   isLineSelectionON                       INIT .F.
   DATA   aSelectionInfo                          INIT { -1,-1,-1,-1,1,0 }
   DATA   aViewportInfo                           INIT { -1,-1,-1,-1,-1,-1 }

   DATA   isColumnSelectionON                     INIT .F.
   DATA   lReadOnly                               INIT .F.
   DATA   isHighLighted                           INIT .f.
   DATA   cLastWord, cCurWord

   METHOD new( oIde, oEditor, nMode )
   METHOD create( oIde, oEditor, nMode )
   METHOD destroy()
   METHOD execEvent( nMode, p, p1 )
   METHOD execKeyEvent( nMode, nEvent, p, p1 )
   METHOD connectEditSignals()
   METHOD disconnectEditSignals()

   METHOD reload()
   METHOD redo()
   METHOD undo()
   METHOD cut()
   METHOD copy()
   METHOD paste()
   METHOD selectAll()

   METHOD setReadOnly( lReadOnly )
   METHOD setNewMark()
   METHOD gotoMark( nIndex )
   METHOD duplicateLine()
   METHOD deleteLine()
   METHOD blockComment()
   METHOD streamComment()
   METHOD blockIndent( nDirctn )
   METHOD moveLine( nDirection )
   METHOD caseUpper()
   METHOD caseLower()
   METHOD caseInvert()
   METHOD convertQuotes()
   METHOD convertDQuotes()
   METHOD findLastIndent()
   METHOD reLayMarkButtons()
   METHOD presentSkeletons()
   METHOD handleCurrentIndent()
   METHOD handlePreviousWord( lUpdatePrevWord )
   METHOD loadFuncHelp()
   METHOD clickFuncHelp()
   METHOD goto( nLine )
   METHOD gotoFunction()
   METHOD toggleLineNumbers()
   METHOD toggleHorzRuler()

   METHOD toggleSelectionMode()
   METHOD toggleStreamSelectionMode()
   METHOD toggleColumnSelectionMode()
   METHOD toggleLineSelectionMode()
   METHOD clearSelection()
   METHOD togglePersistentSelection()
   METHOD toggleCodeCompetion()
   METHOD toggleCompetionTips()

   METHOD getWord( lSelect )
   METHOD getLine( nLine, lSelect )
   METHOD getText()
   METHOD getSelectedText()
   METHOD getColumnNo()
   METHOD getLineNo()
   METHOD insertSeparator( cSep )
   METHOD insertText( cText )

   METHOD suspendPrototype()
   METHOD resumePrototype()
   METHOD showPrototype( cProto )
   METHOD hidePrototype()
   METHOD completeCode( p )

   METHOD setLineNumbersBkColor( nR, nG, nB )
   METHOD setCurrentLineColor( nR, nG, nB )
   METHOD getCursor()                             INLINE ::qEdit:textCursor()
   METHOD find( cText, nPosFrom )
   METHOD refresh()
   METHOD isModified()                            INLINE ::oEditor:qDocument:isModified()
   METHOD setFont()
   METHOD markCurrentFunction()
   METHOD copyBlockContents()
   METHOD pasteBlockContents()
   METHOD insertBlockContents( oKey )
   METHOD cutBlockContents( k )
   METHOD zoom( nKey )
   METHOD blockConvert( cMode )
   METHOD dispStatusInfo()
   METHOD toggleCurrentLineHighlightMode()
   METHOD currentFunctionIndex()
   METHOD toPreviousFunction()
   METHOD toNextFunction()

   METHOD home()
   METHOD end()
   METHOD down()
   METHOD up()
   METHOD goBottom()
   METHOD goTop()
   METHOD left()
   METHOD right()
   METHOD panEnd()
   METHOD panHome()
   METHOD pageUp()
   METHOD pageDown()
   METHOD printPreview()
   METHOD paintRequested( qPrinter )
   METHOD tabs2spaces()
   METHOD spaces2tabs()
   METHOD removeTrailingSpaces()
   METHOD formatBraces()
   METHOD upperCaseKeywords()
   METHOD findEx( cText, nFlags, nStart )
   METHOD highlightAll( cText )
   METHOD unHighlight()
   METHOD parseCodeCompletion( cSyntax )

   METHOD highlightPage()
   METHOD reformatLine( nPos, nAdded, nDeleted )
   METHOD handleTab( key )

   ENDCLASS

/*----------------------------------------------------------------------*/

METHOD IdeEdit:new( oIde, oEditor, nMode )

   DEFAULT oIde    TO ::oIde
   DEFAULT oEditor TO ::oEditor
   DEFAULT nMode   TO ::nMode

   ::oIde    := oIde
   ::oEditor := oEditor
   ::nMode   := nMode

   ::fontFamily       := ::oINI:cFontName
   ::pointSize        := ::oINI:nPointSize
   ::currentPointSize := ::oINI:nPointSize

   RETURN Self

/*----------------------------------------------------------------------*/

METHOD IdeEdit:create( oIde, oEditor, nMode )
   LOCAL nBlock, oPalette

   DEFAULT oIde    TO ::oIde
   DEFAULT oEditor TO ::oEditor
   DEFAULT nMode   TO ::nMode

   ::oIde    := oIde
   ::oEditor := oEditor
   ::nMode   := nMode

   ::qEdit   := HBQPlainTextEdit()
   //
   ::qEdit:setLineWrapMode( QTextEdit_NoWrap )
   ::qEdit:ensureCursorVisible()
   ::qEdit:setContextMenuPolicy( Qt_CustomContextMenu )
   ::qEdit:setTabChangesFocus( .f. )
   ::qEdit:setFocusPolicy( Qt_StrongFocus )
   ::qEdit:setObjectName( hbide_getNextIDasString( "HBQPlainTextEdit" ) )

   oPalette := ::qEdit:palette()
   oPalette:setColor( QPalette_Inactive, QPalette_Highlight, QColor( Qt_yellow ) )
   ::qEdit:setPalette( oPalette )

   ::setFont()

   ::qEdit:hbSetSpaces( ::nTabSpaces )

   ::qEdit:hbSetCompleter( ::qCompleter )

   ::toggleCurrentLineHighlightMode()
   ::toggleLineNumbers()
   ::toggleHorzRuler()

   FOR EACH nBlock IN ::aBookMarks
      ::qEdit:hbBookMarks( nBlock )
   NEXT

   ::connectEditSignals()

   ::qEdit:connect( QEvent_KeyPress           , {|p| ::execKeyEvent( 101, QEvent_KeyPress           , p ) } )
   ::qEdit:connect( QEvent_Wheel              , {|p| ::execKeyEvent( 102, QEvent_Wheel              , p ) } )
   ::qEdit:connect( QEvent_FocusIn            , {| | ::execKeyEvent( 104, QEvent_FocusIn                ) } )
   ::qEdit:connect( QEvent_Resize             , {| | ::execKeyEvent( 106, QEvent_Resize                 ) } )
   ::qEdit:connect( QEvent_FocusOut           , {| | ::execKeyEvent( 105, QEvent_FocusOut               ) } )
   ::qEdit:connect( QEvent_MouseButtonDblClick, {|p| ::execKeyEvent( 103, QEvent_MouseButtonDblClick, p ) } )

   ::qEdit:hbSetEventBlock( {|p,p1| ::execKeyEvent( 115, 1001, p, p1 ) } )

   ::qTimer := QTimer()
   ::qTimer:setInterval( 2000 )
   ::qTimer:connect( "timeout()",  {|| ::execEvent( __timerTimeout__ ) } )

   RETURN Self

/*----------------------------------------------------------------------*/

METHOD IdeEdit:destroy()

   IF Self == ::oEditor:oEdit
      ::oSourceThumbnailDock:oWidget:hide()
   ENDIF

   ::oEditor := NIL

   ::qTimer:disconnect( "timeout()" )
   IF ::qTimer:isActive()
      ::qTimer:stop()
   ENDIF
   ::qTimer := NIL

   ::qEdit:disconnect( QEvent_KeyPress            )
   ::qEdit:disconnect( QEvent_Wheel               )
   ::qEdit:disconnect( QEvent_FocusIn             )
   ::qEdit:disconnect( QEvent_FocusOut            )
   ::qEdit:disconnect( QEvent_Resize              )
   ::qEdit:disconnect( QEvent_MouseButtonDblClick )

   ::disconnectEditSignals()

   //::qEdit  := NIL
   ::qEdit:setParent( QWidget() )  /* Works, but GPF on exit */
   ::qFont  := NIL

   RETURN NIL

/*----------------------------------------------------------------------*/

METHOD IdeEdit:disconnectEditSignals()

   ::qEdit:disConnect( "customContextMenuRequested(QPoint)" )
   ::qEdit:disConnect( "textChanged()"                      )
   ::qEdit:disConnect( "selectionChanged()"                 )
   ::qEdit:disConnect( "cursorPositionChanged()"            )
   ::qEdit:disConnect( "copyAvailable(bool)"                )

   #if 0
   ::qEdit:disConnect( "updateRequest(QRect,int)"           )
   ::qEdit:disConnect( "modificationChanged(bool)"          )
   ::qEdit:disConnect( "redoAvailable(bool)"                )
   ::qEdit:disConnect( "undoAvailable(bool)"                )
   #endif

   RETURN NIL

/*----------------------------------------------------------------------*/

METHOD IdeEdit:connectEditSignals()

   ::qEdit:connect( "customContextMenuRequested(QPoint)", {|p   | ::execEvent( __customContextMenuRequested__, p ) } )
   ::qEdit:connect( "selectionChanged()"                , {|p   | ::execEvent( __selectionChanged__, p    ) } )
   ::qEdit:connect( "cursorPositionChanged()"           , {|    | ::execEvent( __cursorPositionChanged__  ) } )
   ::qEdit:connect( "copyAvailable(bool)"               , {|p   | ::execEvent( __copyAvailable__, p       ) } )

   #if 0
   ::qEdit:connect( "modificationChanged(bool)"         , {|p   | ::execEvent( __modificationChanged__, p ) } )
   ::qEdit:connect( "textChanged()"                     , {|    | ::execEvent( __textChanged__            ) } )
   ::qEdit:connect( "updateRequest(QRect,int)"          , {|p,p1| ::execEvent( __updateRequest__, p, p1   ) } )
   ::qEdit:connect( "redoAvailable(bool)"               , {|p   | ::execEvent( __redoAvailable__, p       ) } )
   ::qEdit:connect( "undoAvailable(bool)"               , {|p   | ::execEvent( __undoAvailable__, p       ) } )
   #endif

   RETURN NIL

/*----------------------------------------------------------------------*/

METHOD IdeEdit:execEvent( nMode, p, p1 )
   LOCAL qAct, n, qCursor, cAct, lOtherEdit

   HB_SYMBOL_UNUSED( p1 )

   IF ::lQuitting
      RETURN NIL
   ENDIF

   qCursor := ::qEdit:textCursor()
   ::nCurLineNo := qCursor:blockNumber()

   SWITCH nMode

   CASE __timerTimeout__
      IF empty( ::cProto )
         ::hidePrototype()
      ELSE
         ::showPrototype()
      ENDIF
      EXIT

   CASE __cursorPositionChanged__
      ::oEditor:dispEditInfo( ::qEdit )   /* Is a MUST */
      ::markCurrentFunction()             /* Optimized */
      EXIT

   CASE __selectionChanged__
      lOtherEdit := ! ( ::oEditor:qCqEdit == ::qEdit )
      IF lOtherEdit
         ::oEditor:qCqEdit := ::qEdit
         ::oEditor:qCoEdit := Self
         IF HB_ISOBJECT( ::oEditor:qHiliter )
            ::oEditor:qHiliter:hbSetEditor( ::qEdit )
            ::qEdit:hbSetHighlighter( ::oEditor:qHiliter )
         ENDIF
      ENDIF
      IF ::aSelectionInfo[ 1 ] > -1 .AND. ::aSelectionInfo[ 1 ] == ::aSelectionInfo[ 3 ]
         ::oDK:setStatusText( SB_PNL_SELECTEDCHARS, Len( ::getSelectedText() ) )
      ELSE
         ::oDK:setStatusText( SB_PNL_SELECTEDCHARS, 0 )
      ENDIF

      ::unHighlight()
      ::oUpDn:show( Self )
      EXIT

   CASE __copyAvailable__
      IF p .AND. ::lCopyWhenDblClicked
         ::qEdit:copy()
      ENDIF
      ::lCopyWhenDblClicked := .f.
      EXIT

   CASE __customContextMenuRequested__
      ::oEM:aActions[ 17, 2 ]:setEnabled( !empty( qCursor:selectedText() ) )

      n := ascan( ::oEditor:aEdits, {|o| o == Self } )

      ::oEM:aActions[ 18, 2 ]:setEnabled( Len( ::oEditor:aEdits ) == 0 .OR. ::oEditor:nSplOrient == -1 .OR. ::oEditor:nSplOrient == 1 )
      ::oEM:aActions[ 19, 2 ]:setEnabled( Len( ::oEditor:aEdits ) == 0 .OR. ::oEditor:nSplOrient == -1 .OR. ::oEditor:nSplOrient == 2 )
      ::oEM:aActions[ 21, 2 ]:setEnabled( n > 0 )

      IF empty( qAct := ::oEM:qContextMenu:exec( ::qEdit:mapToGlobal( p ) ) )
         RETURN Self
      ENDIF
      cAct := strtran( qAct:text(), "&", "" )
      SWITCH cAct
      CASE "Split Horizontally"
         ::oEditor:split( 1, Self )
         EXIT
      CASE "Split Vertically"
         ::oEditor:split( 2, Self )
         EXIT
      CASE "Close Split Window"
         IF n > 0  /* 1 == Main Edit */
            hb_adel( ::oEditor:aEdits, n, .t. )
            ::oEditor:qCqEdit := ::oEditor:qEdit
            ::oEditor:qCoEdit := ::oEditor:oEdit
            ::destroy()
            ::oIde:manageFocusInEditor()
         ENDIF
         EXIT
      CASE "Save as Skeleton..."
         ::oSK:saveAs( ::getSelectedText() )
         EXIT
      CASE "Apply Theme"
         ::oEditor:applyTheme()
         EXIT
      CASE "Goto Function"
         ::gotoFunction()
         EXIT
      CASE "Cut"
         ::cut()
         EXIT
      CASE "Copy"
         ::copy()
         EXIT
      CASE "Paste"
         ::paste()
         EXIT
      CASE "Undo"
         ::undo()
         EXIT
      CASE "Redo"
         ::redo()
         EXIT
      CASE "Diff"
         ::oEditor:vssExecute( "Diff" )
         EXIT
      CASE "Get Latest Version"
         ::oEditor:vssExecute( "Get" )
         EXIT
      CASE "Checkin"
         ::oEditor:vssExecute( "Checkin" )
         EXIT
      CASE "Undo Checkout"
         ::oEditor:vssExecute( "Undocheckout" )
         EXIT
      CASE "Checkout"
         ::oEditor:vssExecute( "Checkout" )
         EXIT
      ENDSWITCH
      EXIT

   #if 0
   CASE __textChanged__
      ::oEditor:setTabImage( ::qEdit )
      ::handlePreviousWord( ::lUpdatePrevWord )
      EXIT
   CASE __modificationChanged__
      ::oEditor:setTabImage( ::qEdit )
      EXIT
   CASE __redoAvailable__
      EXIT
   CASE __undoAvailable__
      EXIT
   CASE __updateRequest__
      EXIT
   #endif
   ENDSWITCH

   RETURN NIL

/*----------------------------------------------------------------------*/

METHOD IdeEdit:execKeyEvent( nMode, nEvent, p, p1 )
   LOCAL key, kbm, lAlt, lCtrl, lShift

   HB_SYMBOL_UNUSED( nMode )
   HB_SYMBOL_UNUSED( p1 )

   SWITCH nEvent
   CASE QEvent_KeyPress
      key    := p:key()
      kbm    := p:modifiers()

      lAlt   := hb_bitAnd( kbm, Qt_AltModifier     ) == Qt_AltModifier
      lCtrl  := hb_bitAnd( kbm, Qt_ControlModifier ) == Qt_ControlModifier
      lShift := hb_bitAnd( kbm, Qt_ShiftModifier   ) == Qt_ShiftModifier

      SWITCH key            /* On top of any user defined action be executed - QPlainTextEdit's default keys */
      CASE Qt_Key_Tab
      CASE Qt_Key_Backtab
         p:accept()
         ::handleTab( key )
         RETURN .t.
      ENDSWITCH

      IF ::oSC:execKey( Self, key, lAlt, lCtrl, lShift )   /* User Defined Actions */
         RETURN .f.
      ENDIF

      SWITCH ( key )

      CASE Qt_Key_F3
         IF ! lCtrl .AND. ! lAlt
            ::oFR:find( .f. )
         ENDIF
         EXIT
      CASE Qt_Key_Insert
         IF lCtrl
            ::copy()
         ENDIF
         EXIT
      CASE Qt_Key_Backspace
         IF ! lCtrl .AND. ! lAlt
            IF ::getLineNo() == ::nProtoLine .AND. ::getColumnNo() <= ::nProtoCol + 1
               ::hidePrototype()
            ENDIF
         ENDIF
         EXIT
      CASE Qt_Key_Space
         IF !lAlt .AND. !lShift .AND. !lCtrl
            ::lUpdatePrevWord := .t.
         ENDIF
         EXIT
      CASE Qt_Key_Return
      CASE Qt_Key_Enter
         ::handlePreviousWord( .t. )
         ::lIndentIt := .t.
         EXIT
      CASE Qt_Key_ParenLeft
         IF ! lCtrl .AND. ! lAlt
            ::loadFuncHelp()     // Also invokes prototype display
         ENDIF
         EXIT
      CASE Qt_Key_ParenRight
         IF ! lCtrl .AND. ! lAlt
            ::hidePrototype()
         ENDIF
         EXIT
      CASE Qt_Key_PageUp
         IF lAlt
            ::toPreviousFunction()
         ENDIF
         EXIT
      CASE Qt_Key_PageDown
         IF lAlt
            ::toNextFunction()
         ENDIF
         EXIT
      ENDSWITCH
      EXIT
   CASE QEvent_Enter
   CASE QEvent_FocusIn
      IF key == QEvent_FocusIn
         ::oUpDn:show()
      ENDIF
      EXIT
   CASE QEvent_Resize
      ::oUpDn:show()
      EXIT
   CASE QEvent_Leave
   CASE QEvent_FocusOut
      EXIT
   CASE QEvent_Wheel
      EXIT
   CASE QEvent_MouseButtonDblClick
      ::lCopyWhenDblClicked := .t.
      ::clickFuncHelp()
      EXIT
   CASE 1001                                /* Fired from hbqt_hbqplaintextedit.cpp */
      SWITCH p
      CASE 21000                            /* Sends Block Info { t,l,b,r,mode,state } hbGetBlockInfo() */
         ::aSelectionInfo := p1
         ::oDK:setButtonState( "SelectionMode", ::aSelectionInfo[ 5 ] > 1 )
         EXIT
      CASE 21001
         ::handlePreviousWord( .t. )
         EXIT
      CASE 21002
         ::loadFuncHelp()
         EXIT
      CASE 21011
         ::copyBlockContents()
         EXIT
      CASE 21012
         ::pasteBlockContents()
         EXIT
      CASE 21013
         ::insertBlockContents( p1 )
         EXIT
      CASE 21014  /* ->hbCut() */
         ::cutBlockContents( p1 )
         EXIT
      CASE 21017                     /* Sends Block Info { t,l,b,r,mode,state } hbGetBlockInfo() */
         ::aViewportInfo := p1
         EXIT
      CASE 21041
         ::qEdit:hbSetFieldsListActive( ::oEM:updateFieldsList( p1 ) )
         EXIT
      CASE 21042
         ::qEdit:hbSetFieldsListActive( ::oEM:updateFieldsList() )
         EXIT
      ENDSWITCH
      EXIT
   ENDSWITCH

   RETURN .F.  /* Important - NEVER CHANGE IT TO .T. */

/*----------------------------------------------------------------------*/

METHOD IdeEdit:zoom( nKey )

   DEFAULT nKey TO 0

   IF nKey == 1
      IF ::currentPointSize + 1 < 30
         ::currentPointSize++
      ENDIF

   ELSEIF nKey == -1
      IF ::currentPointSize - 1 > 3
         ::currentPointSize--
      ENDIF

   ELSEIF nKey == 0
      ::currentPointSize := ::pointSize

   ELSEIF nKey >= 3 .AND. nKey <= 30
      ::currentPointSize := nKey

   ELSE
      RETURN Self

   ENDIF

   ::setFont()

   RETURN Self

/*----------------------------------------------------------------------*/

METHOD IdeEdit:setFont()

   ::qFont := QFont()
   ::qFont:setFamily( ::fontFamily )
   ::qFont:setFixedPitch( .t. )
   ::qFont:setPointSize( ::currentPointSize )

   ::qEdit:setFont( ::qFont )

   RETURN Self

/*----------------------------------------------------------------------*/

METHOD IdeEdit:highlightPage()

   IF HB_ISOBJECT( ::oEditor:qHiliter )
      ::qEdit:hbHighlightPage()
   ENDIF

   RETURN Self

/*----------------------------------------------------------------------*/

METHOD IdeEdit:dispStatusInfo()
   LOCAL nMode

   nMode := ::aSelectionInfo[ 5 ]
   ::oDK:setButtonState( "SelectionMode", nMode > 1 )
   ::oDK:setStatusText( SB_PNL_STREAM, iif( nMode == 2, "Column", iif( nMode == 3, "Line", "Stream" ) ) )

   RETURN Self

/*----------------------------------------------------------------------*/

STATIC FUNCTION hbide_blockContents( aContents )
   LOCAL oldContents
   STATIC contents := {}

   oldContents := contents
   IF HB_ISARRAY( aContents )
      contents := aclone( aContents )
   ENDIF

   RETURN oldContents

/*----------------------------------------------------------------------*/

STATIC FUNCTION hbide_setQCursor( qEdit, a_ )
   LOCAL qCursor

   IF HB_ISARRAY( a_ )
      qCursor := a_[ 1 ]
      qCursor:movePosition( QTextCursor_Start, QTextCursor_MoveAnchor )
      qCursor:movePosition( QTextCursor_Down , QTextCursor_MoveAnchor, a_[ 2 ] )
      qCursor:movePosition( QTextCursor_Right, QTextCursor_MoveAnchor, a_[ 3 ] )
      qEdit:setTextCursor( qCursor )
      qCursor:endEditBlock()
   ELSE
      qCursor := qEdit:textCursor()
      qCursor:beginEditBlock()
      RETURN { qCursor, qCursor:blockNumber(), qCursor:columnNumber() }
   ENDIF

   RETURN NIL

/*----------------------------------------------------------------------*/

STATIC FUNCTION hbide_qReplaceLine( qCursor, nLine, cLine )

   qCursor:movePosition( QTextCursor_Start      , QTextCursor_MoveAnchor )
   qCursor:movePosition( QTextCursor_Down       , QTextCursor_MoveAnchor, nLine )
   qCursor:movePosition( QTextCursor_StartOfLine, QTextCursor_MoveAnchor )
   qCursor:movePosition( QTextCursor_EndOfLine  , QTextCursor_KeepAnchor )
   qCursor:insertText( cLine )

   RETURN NIL

/*----------------------------------------------------------------------*/

STATIC FUNCTION hbide_qPositionCursor( qCursor, nRow, nCol )

   qCursor:movePosition( QTextCursor_Start, QTextCursor_MoveAnchor       )
   qCursor:movePosition( QTextCursor_Down , QTextCursor_MoveAnchor, nRow )
   qCursor:movePosition( QTextCursor_Right, QTextCursor_MoveAnchor, nCol )

   RETURN NIL

/*----------------------------------------------------------------------*/

STATIC FUNCTION hbide_invert( cBuffer )
   LOCAL s, i, c, nLen

   s    := ""
   nLen := Len( cBuffer )
   FOR i := 1 TO nLen
      c := substr( cBuffer, i, 1 )
      IF isAlpha( c )
         s += iif( isUpper( c ), lower( c ), upper( c ) )
      ELSE
         s += c
      ENDIF
   NEXT

   RETURN s

/*----------------------------------------------------------------------*/

STATIC FUNCTION hbide_convertALine( cLine, cMode )

   SWITCH cMode
   CASE "toupper"
      cLine := upper( cLine )
      EXIT
   CASE "tolower"
      cLine := lower( cLine )
      EXIT
   CASE "invert"
      cLine := hbide_invert( cLine )
      EXIT
   CASE "sgl2dbl"
      cLine := strtran( cLine, "'", '"' )
      EXIT
   CASE "dbl2sgl"
      cLine := strtran( cLine, '"', "'" )
      EXIT
   ENDSWITCH

   RETURN cLine

/*----------------------------------------------------------------------*/

STATIC FUNCTION hbide_qCursorDownInsert( qCursor )
   LOCAL nRow := qCursor:blockNumber()

   qCursor:movePosition( QTextCursor_Down, QTextCursor_MoveAnchor )
   IF qCursor:blockNumber() == nRow
      qCursor:movePosition( QTextCursor_EndOfBlock, QTextCursor_MoveAnchor )
      qCursor:insertBlock()
      qCursor:movePosition( QTextCursor_NextBlock, QTextCursor_MoveAnchor )
   ENDIF

   RETURN NIL

/*----------------------------------------------------------------------*/

METHOD IdeEdit:copyBlockContents()
   LOCAL nT, nL, nB, nR, nW, i, cLine, nMode, qClip, aCord
   LOCAL cClip := ""

   HB_TRACE( HB_TR_DEBUG, "IdeEdit:copyBlockContents( aCord )" )

   aCord := ::aSelectionInfo

   hbide_normalizeRect( aCord, @nT, @nL, @nB, @nR )
   nMode := aCord[ 5 ]

   ::aBlockCopyContents := {}

   nW := nR - nL
   FOR i := nT TO nB
      cLine := ::getLine( i + 1 )
      cLine := strtran( cLine, chr( 13 ) )
      cLine := strtran( cLine, chr( 10 ) )

      IF nMode == __selectionMode_stream__
         IF aCord[ 1 ] > aCord[ 3 ]  // Selection - bottom to top
            IF i == nT .AND. i == nB
               cLine := substr( cLine, min( aCord[ 2 ], aCord[ 4 ] ) + 1, nW )
            ELSEIF i == aCord[ 1 ]
               cLine := substr( cLine, 1, aCord[ 2 ] )
            ELSEIF i == aCord[ 3 ]
               cLine := substr( cLine, aCord[ 4 ] + 1 )
            ENDIF
         ELSE                        // Selection - top to bottom or same row
            IF i == nT .AND. i == nB
               cLine := substr( cLine, min( aCord[ 2 ], aCord[ 4 ] ) + 1, nW )
            ELSEIF i == aCord[ 1 ]
               cLine := substr( cLine, aCord[ 2 ] + 1 )
            ELSEIF i == aCord[ 3 ]
               cLine := substr( cLine, 1, aCord[ 4 ] )
            ENDIF
         ENDIF

      ELSEIF nMode == __selectionMode_column__
         cLine := pad( substr( cLine, nL + 1, nW ), nW )

      ELSEIF nMode == __selectionMode_line__
         // Nothing to do, complete line is already pulled

      ENDIF

      aadd( ::aBlockCopyContents, cLine )
      cClip += cLine + iif( nT == nB, "", iif( i < nB, hb_eol(), "" ) )
   NEXT

   hbide_blockContents( { nMode, ::aBlockCopyContents } )

   qClip := QClipboard()
   qClip:clear()
   qClip:setText( cClip )

   RETURN Self

/*----------------------------------------------------------------------*/

METHOD IdeEdit:pasteBlockContents()
   LOCAL i, nCol, qCursor, nMaxCol, aCopy, a_, nPasteMode, nMode

   IF ::lReadOnly
      RETURN Self
   ENDIF

   nMode := ::aSelectionInfo[ 5 ]

   aCopy := hb_ATokens( StrTran( QClipboard():text(), Chr( 13 ) + Chr( 10 ), _EOL ), _EOL )
   IF empty( aCopy )
      RETURN Self
   ENDIF

   nPasteMode := nMode   /* OR Stream - needs to be thought carefully */

   a_:= hbide_blockContents()
   IF !empty( a_ )
      IF ( Len( a_[ 2 ] ) == len( aCopy ) ) .OR. ( len( a_[ 2 ] ) == len( aCopy ) + 1 )
         IF a_[ 2,1 ] == aCopy[ 1 ]
            nPasteMode := a_[ 1 ]
         ENDIF
      ENDIF
   ENDIF

   nPasteMode := iif( empty( nPasteMode ), __selectionMode_stream__, nPasteMode )
   qCursor    := ::qEdit:textCursor()
   nCol       := qCursor:columnNumber()

   qCursor:beginEditBlock()
   //
   SWITCH nPasteMode
   CASE __selectionMode_column__
      FOR i := 1 TO Len( aCopy )
         qCursor:insertText( aCopy[ i ] )
         IF i < Len( aCopy )
            hbide_qCursorDownInsert( qCursor )

            qCursor:movePosition( QTextCursor_EndOfLine, QTextCursor_MoveAnchor )
            nMaxCol := qCursor:columnNumber()
            IF nMaxCol < nCol
               qCursor:insertText( replicate( " ", nCol - nMaxCol ) )
            ENDIF
            qCursor:movePosition( QTextCursor_StartOfLine, QTextCursor_MoveAnchor       )
            qCursor:movePosition( QTextCursor_Right      , QTextCursor_MoveAnchor, nCol )
         ENDIF
      NEXT
      EXIT
   CASE __selectionMode_stream__
      FOR i := 1 TO Len( aCopy )
         qCursor:insertText( aCopy[ i ] )
         IF i < Len( aCopy )
            qCursor:insertText( hb_eol() )
         ENDIF
      NEXT
      EXIT
   CASE __selectionMode_line__
      qCursor:movePosition( QTextCursor_StartOfLine, QTextCursor_MoveAnchor       )
      FOR i := 1 TO Len( aCopy )
         qCursor:insertText( aCopy[ i ] )
         qCursor:insertBlock()
      NEXT
      EXIT
   ENDSWITCH

   qCursor:endEditBlock()
   ::qEdit:ensureCursorVisible()

   RETURN Self

/*----------------------------------------------------------------------*/

METHOD IdeEdit:insertBlockContents( oKey )  /* Only called if block selection is on */
   LOCAL nT, nL, nB, nR, nW, i, cLine, cKey, qCursor, aCord, qCur

   IF ::lReadOnly
      RETURN Self
   ENDIF

   cKey := chr( hbxbp_QKeyEventToAppEvent( oKey ) )

   aCord := ::aSelectionInfo
   hbide_normalizeRect( aCord, @nT, @nL, @nB, @nR )
   nW := nR - nL

   qCursor := ::qEdit:textCursor()
   qCur := ::qEdit:textCursor()
   qCursor:beginEditBlock()

   IF nW == 0
      FOR i := nT TO nB
         cLine := ::getLine( i + 1 )
         cLine := pad( substr( cLine, 1, nL ), nL ) + cKey + substr( cLine, nL + 1 )
         hbide_qReplaceLine( qCursor, i, cLine )
      NEXT

      hbide_qPositionCursor( qCursor, qCur:blockNumber(), nR + 1 )
   ELSE
      FOR i := nT TO nB
         cLine := ::getLine( i + 1 )
         cLine := pad( substr( cLine, 1, nL ), nL ) + replicate( cKey, nW ) + substr( cLine, nR + 1 )
         hbide_qReplaceLine( qCursor, i, cLine )
      NEXT

      hbide_qPositionCursor( qCursor, qCur:blockNumber(), nR )
   ENDIF

   ::qEdit:setTextCursor( qCursor )
   qCursor:endEditBlock()

   RETURN Self

/*----------------------------------------------------------------------*/

METHOD IdeEdit:cutBlockContents( k )
   LOCAL nT, nL, nB, nR, i, cLine, qCursor, nSelMode, aCord, qCur

   IF ::lReadOnly
      RETURN Self
   ENDIF

   k := iif( empty( k ), Qt_Key_X, k )
   IF k == Qt_Key_X
      ::copyBlockContents()
   ENDIF
   aCord := ::aSelectionInfo
   hbide_normalizeRect( aCord, @nT, @nL, @nB, @nR )

   nSelMode := aCord[ 5 ]

   qCursor := ::qEdit:textCursor()
   qCur := ::qEdit:textCursor()
   qCursor:beginEditBlock()

   IF k == Qt_Key_Backspace
      IF nSelMode == __selectionMode_column__
         FOR i := nT TO nB
            cLine := ::getLine( i + 1 )
            cLine := pad( substr( cLine, 1, nL - 1 ), nL - 1 ) + substr( cLine, nL + 1 )
            hbide_qReplaceLine( qCursor, i, cLine )
         NEXT
         hbide_qPositionCursor( qCursor, qCur:blockNumber(), nR - 1 )
      ENDIF
   ELSE
      IF k == Qt_Key_Delete .OR. k == Qt_Key_X
         IF nSelMode == __selectionMode_column__
            FOR i := nT TO nB
               cLine := ::getLine( i + 1 )
               cLine := pad( substr( cLine, 1, nL ), nL ) + substr( cLine, nR + 1 )
               hbide_qReplaceLine( qCursor, i, cLine )
            NEXT
            hbide_qPositionCursor( qCursor, qCur:blockNumber(), nL )
            ::qEdit:hbSetSelectionInfo( { nT, nL, nB, nL, __selectionMode_column__ } )

         ELSEIF nSelMode == __selectionMode_stream__
            hbide_qPositionCursor( qCursor, nT, nL )
            qCursor:movePosition( QTextCursor_Down       , QTextCursor_KeepAnchor, nB - nT )
            qCursor:movePosition( QTextCursor_StartOfLine, QTextCursor_KeepAnchor          )
            qCursor:movePosition( QTextCursor_Right      , QTextCursor_KeepAnchor, nR      )
            qCursor:removeSelectedText()
            ::qEdit:hbSetSelectionInfo( { -1, -1, -1, -1, __selectionMode_stream__ } )

         ELSEIF nSelMode == __selectionMode_line__
            hbide_qPositionCursor( qCursor, nT, nL )
            qCursor:movePosition( QTextCursor_Down       , QTextCursor_KeepAnchor, nB - nT + 1 )
            qCursor:movePosition( QTextCursor_StartOfLine, QTextCursor_KeepAnchor          )
            qCursor:removeSelectedText()
            ::qEdit:hbSetSelectionInfo( { -1, -1, -1, -1, __selectionMode_stream__ } )
            ::isLineSelectionON := .f.

         ENDIF
      ENDIF
   ENDIF

   ::qEdit:setTextCursor( qCursor )
   qCursor:endEditBlock()

   RETURN Self

/*----------------------------------------------------------------------*/

METHOD IdeEdit:blockComment()  /* Toggles the block comments - always inserted at the begining of the line */
   LOCAL nT, nL, nB, nR, nW, i, cLine, qCursor, aCord, nMode, a_
   LOCAL cComment := "// "
   LOCAL nLen := Len( cComment )

   IF ::lReadOnly
      RETURN Self
   ENDIF

   aCord := ::aSelectionInfo
   hbide_normalizeRect( aCord, @nT, @nL, @nB, @nR )
   nW := nR - nL

   IF nW >= 0
      nMode := aCord[ 5 ]
      a_:= hbide_setQCursor( ::qEdit )
      qCursor := a_[ 1 ]

      FOR i := nT TO nB
         cLine := ::getLine( i + 1 )

         SWITCH nMode
         CASE __selectionMode_stream__
         CASE __selectionMode_line__
            IF substr( cLine, 1, nLen ) == cComment
               cLine := substr( cLine, nLen + 1 )
            ELSE
               cLine := cComment + cLine
            ENDIF
            EXIT
         CASE __selectionMode_column__
            IF substr( cLine, nL + 1, nLen ) == cComment
               cLine := pad( substr( cLine, 1, nL ), nL ) + substr( cLine, nL + nLen + 1 )
            ELSE
               cLine := pad( substr( cLine, 1, nL ), nL ) + cComment + substr( cLine, nL + 1 )
            ENDIF
            EXIT
         ENDSWITCH

         hbide_qReplaceLine( qCursor, i, cLine )
      NEXT
#if 1
      hbide_setQCursor( ::qEdit, a_ )
#else
      hbide_qPositionCursor( qCursor, qCur:blockNumber(), nL )
      ::qEdit:hbSetSelectionInfo( { nT, nL, nB, nL, __selectionMode_column__ } )
#endif
   ENDIF

   RETURN Self

/*----------------------------------------------------------------------*/

METHOD IdeEdit:streamComment()
   LOCAL nT, nL, nB, nR, nW, i, cLine, qCursor, aCord, nMode, a_

   IF ::lReadOnly
      RETURN Self
   ENDIF

   aCord := ::aSelectionInfo
   hbide_normalizeRect( aCord, @nT, @nL, @nB, @nR )
   nW := nR - nL

   IF nW >= 0
      nMode   := aCord[ 5 ]
      a_:= hbide_setQCursor( ::qEdit ) ; qCursor := a_[ 1 ]

      FOR i := nT TO nB
         cLine := ::getLine( i + 1 )

         DO CASE
         CASE nMode == __selectionMode_stream__
            IF i == nT
               cLine := substr( cLine, 1, nL ) + "/* " + substr( cLine, nL + 1 )
            ELSEIF i == nB
               cLine := substr( cLine, 1, nR ) + " */" + substr( cLine, nR + 1 )
            ENDIF

         CASE nMode == __selectionMode_line__
            IF i == nT
               cLine := "/* " + cLine
            ELSEIF i == nB
               cLine += " */"
            ENDIF

         ENDCASE

         hbide_qReplaceLine( qCursor, i, cLine )
      NEXT

      hbide_setQCursor( ::qEdit, a_ )
   ENDIF

   RETURN Self

/*----------------------------------------------------------------------*/

METHOD IdeEdit:handleTab( key )
   LOCAL nT, nL, nB, nR, i, cLine, qCursor, aCord, nMode, nCol, nRow
   LOCAL cComment := space( ::nTabSpaces )
   LOCAL nLen := ::nTabSpaces
   LOCAL nOff := iif( key == Qt_Key_Tab, nLen, -nLen )

   HB_SYMBOL_UNUSED( key )
   IF ::lReadOnly
      RETURN Self
   ENDIF

   aCord := ::aSelectionInfo
   hbide_normalizeRect( aCord, @nT, @nL, @nB, @nR )
   nMode := aCord[ 5 ]

   qCursor := ::qEdit:textCursor()
   qCursor:beginEditBlock()
   nCol := qCursor:columnNumber()
   nRow := qCursor:blockNumber()

   SWITCH nMode
   CASE __selectionMode_column__
      FOR i := nT TO nB
         cLine := ::getLine( i + 1 )
         IF key == Qt_Key_Tab
            cLine := substr( cLine, 1, nCol ) + cComment + substr( cLine, nCol + 1 )
         ELSE
            cLine := substr( cLine, 1, nCol - 3 ) + substr( cLine, nCol + 1 )
         ENDIF
         hbide_qReplaceLine( qCursor, i, cLine )
      NEXT
      hbide_qPositionCursor( qCursor, nRow, max( 0, nCol + nOff ) )
      ::qEdit:hbSetSelectionInfo( { nT, max( 0, nL + nOff ), nB, max( 0, nR + nOff ), __selectionMode_column__ } )
      EXIT
   CASE __selectionMode_stream__
   CASE __selectionMode_line__
      IF nL >= 0    /* Selection is marked */
         ::cutBlockContents( Qt_Key_Delete )
      ENDIF
      cLine := ::getLine( nRow + 1 )
      IF key == Qt_Key_Tab
         cLine := substr( cLine, 1, nCol ) + cComment + substr( cLine, nCol + 1 )
      ELSE
         cLine := substr( cLine, 1, nCol - nLen ) + substr( cLine, nCol + 1 )
      ENDIF
      hbide_qReplaceLine( qCursor, nRow, cLine )
      hbide_qPositionCursor( qCursor, nRow, max( 0, nCol + nOff ) )
      EXIT
   ENDSWITCH
   ::qEdit:setTextCursor( qCursor )
   qCursor:endEditBlock()

   RETURN .t.

/*----------------------------------------------------------------------*/

METHOD IdeEdit:blockIndent( nDirctn )
   LOCAL nT, nL, nB, nR, nW, i, cLine, qCursor, aCord, a_, nMode, cLineSel

   IF ::lReadOnly
      RETURN Self
   ENDIF

   aCord := ::aSelectionInfo
   hbide_normalizeRect( aCord, @nT, @nL, @nB, @nR )
   nW := nR - nL

   IF nW >= 0
      nMode := aCord[ 5 ]
      a_:= hbide_setQCursor( ::qEdit )
      qCursor := a_[ 1 ]

      FOR i := nT TO nB
         cLine := ::getLine( i + 1 )

         SWITCH nMode
         CASE __selectionMode_column__
            cLineSel := pad( substr( cLine, nL + 1, nW ), nW )
            IF nDirctn == -1
               IF left( cLineSel, 1 ) == " "
                  cLineSel := substr( cLineSel, 2 )
               ENDIF
            ELSE
               cLineSel := " " + cLineSel
            ENDIF
            cLine := pad( substr( cLine, 1, nL ), nL ) + cLineSel + substr( cLine, nR + 1 )
            EXIT
         CASE __selectionMode_stream__
         CASE __selectionMode_line__
            IF nDirctn == -1
               IF left( cLine, 1 ) == " "
                  cLine := substr( cLine, 2 )
               ENDIF
            ELSE
               cLine := " " + cLine
            ENDIF
            EXIT
         ENDSWITCH

         hbide_qReplaceLine( qCursor, i, cLine )
      NEXT

      hbide_setQCursor( ::qEdit, a_ )
   ENDIF

   RETURN Self

/*----------------------------------------------------------------------*/

METHOD IdeEdit:blockConvert( cMode )
   LOCAL nT, nL, nB, nR, nW, i, cLine, qCursor, aCord, a_, nMode

   IF ::lReadOnly
      RETURN Self
   ENDIF

   aCord := ::aSelectionInfo
   hbide_normalizeRect( aCord, @nT, @nL, @nB, @nR )
   nW := nR - nL

   IF nW >= 0
      nMode := aCord[ 5 ]
      a_:= hbide_setQCursor( ::qEdit ) ; qCursor := a_[ 1 ]

      FOR i := nT TO nB
         cLine := ::getLine( i + 1 )

         DO CASE
         CASE nMode == __selectionMode_stream__
            IF nT == nB
               cLine := substr( cLine, 1, nL ) + hbide_convertALine( substr( cLine, nL + 1, nW ), cMode ) + substr( cLine, nL + 1 + nW )
            ELSE
               IF i == nT
                  cLine := substr( cLine, 1, nL ) + hbide_convertALine( substr( cLine, nL + 1 ), cMode )
               ELSEIF i == nB
                  cLine := hbide_convertALine( substr( cLine, 1, nR ), cMode ) + substr( cLine, nR + 1 )
               ELSE
                  cLine := hbide_convertALine( cLine, cMode )
               ENDIF
            ENDIF

         CASE nMode == __selectionMode_column__
            cLine := pad( substr( cLine, 1, nL ), nL ) + hbide_convertALine( pad( substr( cLine, nL + 1, nW ), nW ), cMode ) + substr( cLine, nR + 1 )

         CASE nMode == __selectionMode_line__
            cLine := hbide_convertALine( cLine, cMode )

         ENDCASE

         hbide_qReplaceLine( qCursor, i, cLine )
      NEXT

      hbide_setQCursor( ::qEdit, a_ )
   ENDIF

   RETURN Self

/*----------------------------------------------------------------------*/

METHOD IdeEdit:getSelectedText()
   LOCAL nT, nL, nB, nR, nW, i, cLine, nMode, cClip := "", aCord

   HB_TRACE( HB_TR_DEBUG, "IdeEdit:getSelectedText()", ProcName( 1 ), procName( 2 ) )

   aCord := ::aSelectionInfo
   hbide_normalizeRect( aCord, @nT, @nL, @nB, @nR )
   nMode := aCord[ 5 ]

   nW := nR - nL
   FOR i := nT TO nB
      cLine := ::getLine( i + 1 )

      IF nMode == __selectionMode_stream__
         IF i == nT .AND. i == nB
            cLine := substr( cLine, nL + 1, nR - nL )
         ELSEIF i == nT
            cLine := substr( cLine, nL + 1 )
         ELSEIF i == nB
            cLine := substr( cLine, 1, nR + 1 )
         ENDIF

      ELSEIF nMode == __selectionMode_column__
         cLine := pad( substr( cLine, nL + 1, nW ), nW )

      ELSEIF nMode == __selectionMode_line__
         // Nothing to do, complete line is already pulled

      ENDIF

      cClip += cLine + iif( i < nB, hb_eol(), "" )
   NEXT

   RETURN cClip

/*----------------------------------------------------------------------*/

METHOD IdeEdit:caseUpper()
   RETURN ::blockConvert( "toupper" )

/*----------------------------------------------------------------------*/

METHOD IdeEdit:caseLower()
   RETURN ::blockConvert( "tolower" )

/*----------------------------------------------------------------------*/

METHOD IdeEdit:caseInvert()
   RETURN ::blockConvert( "invert" )

/*----------------------------------------------------------------------*/

METHOD IdeEdit:convertQuotes()
   RETURN ::blockConvert( "dbl2sgl" )

/*----------------------------------------------------------------------*/

METHOD IdeEdit:convertDQuotes()
   RETURN ::blockConvert( "sgl2dbl" )

/*----------------------------------------------------------------------*/

METHOD IdeEdit:currentFunctionIndex()
   LOCAL n := -1, nCurLine

   IF !empty( ::aTags )
      nCurLine := ::getLineNo()
      IF Len( ::aTags ) == 1
         n := 1
      ELSEIF ( n := ascan( ::aTags, {|e_| e_[ 3 ] >= nCurLine } ) ) == 0
         n := Len( ::aTags )
      ELSEIF n > 0
         n--
      ENDIF
   ENDIF

   RETURN n

/*----------------------------------------------------------------------*/

METHOD IdeEdit:toNextFunction()
   LOCAL n

   IF ( n := ::currentFunctionIndex() ) >= 0
      IF n < Len( ::aTags )
         IF ::find( ::aTags[ n+1, 8 ], QTextDocument_FindCaseSensitively )
            ::qEdit:centerCursor()
            ::down()
         ENDIF
      ENDIF
   ENDIF

   RETURN Self

/*----------------------------------------------------------------------*/

METHOD IdeEdit:toPreviousFunction()
   LOCAL n

   IF ( n := ::currentFunctionIndex() ) > 1
      IF ::find( ::aTags[ n-1, 8 ], QTextDocument_FindCaseSensitively )
         ::qEdit:centerCursor()
         ::down()
      ENDIF
   ENDIF

   RETURN Self

/*----------------------------------------------------------------------*/

METHOD IdeEdit:markCurrentFunction()
   LOCAL n

   IF ::oFuncDock:oWidget:isVisible()
      IF ::nPrevLineNo1 != ::getLineNo()
         ::nPrevLineNo1 := ::getLineNo()
         IF ( n := ::currentFunctionIndex() ) > 0
            ::oIde:oFuncList:setItemColorFG( ::aTags[ n,7 ], { 255,0,0 } )
            ::oIde:oFuncList:setVisible( ::aTags[ n,7 ] )
         ENDIF
      ENDIF
   ENDIF

   RETURN Self

/*----------------------------------------------------------------------*/

METHOD IdeEdit:presentSkeletons()
   ::oSK:selectByMenuAndPostText( ::qEdit )
   RETURN Self

/*----------------------------------------------------------------------*/

METHOD IdeEdit:toggleCurrentLineHighlightMode()
   ::qEdit:hbHighlightCurrentLine( ::lCurrentLineHighlightEnabled )
   RETURN Self

/*----------------------------------------------------------------------*/

METHOD IdeEdit:toggleLineNumbers()
   ::qEdit:hbNumberBlockVisible( ::lLineNumbersVisible )
   RETURN Self

/*----------------------------------------------------------------------*/

METHOD IdeEdit:toggleHorzRuler()
   ::qEdit:hbHorzRulerVisible( ::lHorzRulerVisible )
   RETURN Self

/*----------------------------------------------------------------------*/
/* Fired by icon */

METHOD IdeEdit:toggleSelectionMode()
   LOCAL qFocus

   qFocus := QApplication():focusWidget()
   ::qEdit:hbSetSelectionMode( iif( ::oDK:setButtonState( "SelectionMode" ), 2, 1 ), .f. )
   qFocus:setFocus( 0 )

   RETURN Self

/*----------------------------------------------------------------------*/

METHOD IdeEdit:toggleStreamSelectionMode()
   ::qEdit:hbSetSelectionMode( 1, .t. )
   RETURN Self

/*----------------------------------------------------------------------*/

METHOD IdeEdit:toggleColumnSelectionMode()
   ::qEdit:hbSetSelectionMode( 2, .t. )
   RETURN Self

/*----------------------------------------------------------------------*/

METHOD IdeEdit:toggleLineSelectionMode()
   ::qEdit:hbSetSelectionMode( 3, .t. )
   RETURN Self

/*----------------------------------------------------------------------*/

METHOD IdeEdit:clearSelection()
   ::qEdit:hbSetSelectionMode( 0, .t. )
   RETURN Self

/*----------------------------------------------------------------------*/

METHOD IdeEdit:togglePersistentSelection()
   ::qEdit:hbTogglePersistentSelection()
   RETURN Self

/*----------------------------------------------------------------------*/

METHOD IdeEdit:toggleCodeCompetion()
   ::qEdit:hbToggleCodeCompetion()
   RETURN Self

/*----------------------------------------------------------------------*/

METHOD IdeEdit:toggleCompetionTips()
   ::qEdit:hbToggleCompetionTips()
   RETURN Self

/*----------------------------------------------------------------------*/

METHOD IdeEdit:reload()
   LOCAL lLoad := .t.

   IF ::oEditor:qDocument:isModified()
      lLoad := hbide_getYesNo( "Source is in modified state", "Reload it anyway?", "Reload" )
   ENDIF
   IF lLoad
      ::oEditor:reload()
   ENDIF
   RETURN Self

/*----------------------------------------------------------------------*/

METHOD IdeEdit:redo()
   ::qEdit:redo()
   RETURN Self

/*----------------------------------------------------------------------*/

METHOD IdeEdit:undo()
   ::qEdit:undo()
   RETURN Self

/*----------------------------------------------------------------------*/

METHOD IdeEdit:cut()
   IF ::lReadOnly
      RETURN Self
   ENDIF
   ::cutBlockContents( Qt_Key_X )
   RETURN Self

/*----------------------------------------------------------------------*/

METHOD IdeEdit:copy()
   ::copyBlockContents()
   RETURN Self

/*----------------------------------------------------------------------*/

METHOD IdeEdit:paste()

   IF ::lReadOnly
      RETURN Self
   ENDIF
   IF ::aSelectionInfo[ 1 ] > -1
      ::cutBlockContents( Qt_Key_Delete )
   ENDIF
   ::pasteBlockContents()

   RETURN Self

/*----------------------------------------------------------------------*/

METHOD IdeEdit:selectAll()
   ::qEdit:hbSelectAll()
   RETURN Self

/*----------------------------------------------------------------------*/

METHOD IdeEdit:setReadOnly( lReadOnly )
   IF ::oEditor:lReadOnly
      lReadOnly := .t.
   ELSE
      IF ! HB_ISLOGICAL( lReadOnly )
         lReadOnly := ! ::qEdit:isReadOnly()
      ENDIF
   ENDIF
   ::lReadOnly := lReadOnly
   ::qEdit:setReadOnly( lReadOnly )
   ::oEditor:setTabImage()
   RETURN Self

/*----------------------------------------------------------------------*/

METHOD IdeEdit:relayMarkButtons()
   LOCAL oBtn
   FOR EACH oBtn IN ::aMarkTBtns
      oBtn:hide()
   NEXT
   FOR EACH oBtn IN ::aBookMarks
      ::aMarkTBtns[ oBtn:__enumIndex() ]:show()
   NEXT
   RETURN Self

/*----------------------------------------------------------------------*/

METHOD IdeEdit:gotoMark( nIndex )

   IF Len( ::aBookMarks ) >= nIndex
      ::qEdit:hbGotoBookmark( ::aBookMarks[ nIndex ] )
      ::qEdit:centerCursor()
   ENDIF

   RETURN Self

/*----------------------------------------------------------------------*/

METHOD IdeEdit:setNewMark()
   LOCAL qCursor, nBlock, n

   IF !( qCursor := ::qEdit:textCursor() ):isNull()
      nBlock := qCursor:blockNumber() + 1

      IF ( n := ascan( ::aBookMarks, nBlock ) ) > 0
         hb_adel( ::aBookMarks, n, .t. )
         ::aMarkTBtns[ Len( ::aBookMarks ) + 1 ]:hide()
      ELSE
         IF Len( ::aBookMarks ) == 6
            RETURN Self
         ENDIF
         aadd( ::aBookMarks, nBlock )
         n := Len( ::aBookMarks )
         ::aMarkTBtns[ n ]:show()
      ENDIF

      ::qEdit:hbBookMarks( nBlock )
      ::qEdit:repaint()
   ENDIF
   RETURN Self

/*----------------------------------------------------------------------*/

METHOD IdeEdit:setLineNumbersBkColor( nR, nG, nB )
   ::qEdit:hbSetLineAreaBkColor( QColor( nR, nG, nB ) )
   RETURN Self

/*----------------------------------------------------------------------*/

METHOD IdeEdit:setCurrentLineColor( nR, nG, nB )
   ::qEdit:hbSetCurrentLineColor( QColor( nR, nG, nB ) )
   RETURN Self

/*----------------------------------------------------------------------*/
/* TO BE EXTENDED */
METHOD IdeEdit:find( cText, nPosFrom )
   LOCAL lFound, nPos
   LOCAL qCursor := ::getCursor()

   nPos := qCursor:position()
   IF HB_ISNUMERIC( nPosFrom )
      qCursor:setPosition( nPosFrom )
   ENDIF
   ::qEdit:setTextCursor( qCursor )
   IF ! ( lFound := ::qEdit:find( cText, QTextDocument_FindCaseSensitively ) )
      IF ! HB_ISNUMERIC( nPosFrom )
         lFound := ::qEdit:find( cText, QTextDocument_FindBackward + QTextDocument_FindCaseSensitively )
      ENDIF
   ENDIF

   IF ! lFound
      qCursor:setPosition( nPos )
      ::qEdit:setTextCursor( qCursor )
   ELSE
      ::qEdit:centerCursor()
   ENDIF

   RETURN lFound

/*----------------------------------------------------------------------*/
/*  nFlags will decide the position, case sensitivity and direction
 */
METHOD IdeEdit:findEx( cText, nFlags, nStart )
   LOCAL qCursor, lFound, nPos

   DEFAULT cText  TO ::getSelectedText()
   DEFAULT nFlags TO 0
   DEFAULT nStart TO 0

   qCursor := ::getCursor()
   nPos    := qCursor:position()

   IF nStart == 0
      // No need to move cursor
   ELSEIF nStart == 1
      ::qEdit:moveCursor( QTextCursor_Start )
   ELSEIF nStart == 2
      ::qEdit:moveCursor( QTextCursor_End )
   ENDIF

   IF ( lFound := ::qEdit:find( cText, nFlags ) )
      ::qEdit:centerCursor()
      qCursor := ::qEdit:textCursor()

      ::qEdit:hbSetSelectionInfo( { qCursor:blockNumber(), qCursor:columnNumber() - Len( cText ), ;
                                    qCursor:blockNumber(), qCursor:columnNumber(), 1, .t., .f. } )
      qCursor:clearSelection()
   ELSE
      qCursor:setPosition( nPos )
      ::qEdit:setTextCursor( qCursor )
   ENDIF

   RETURN lFound

/*----------------------------------------------------------------------*/

METHOD IdeEdit:unHighlight()
   LOCAL qCursor, nPos, lModified

   IF ::isHighLighted
      ::isHighLighted := .f.
      qCursor := ::getCursor()
      nPos := qCursor:position()
      lModified := ::qEdit:document():isModified()
      ::qEdit:undo()
      IF ! lModified
         ::qEdit:document():setModified( .f. )
         ::oEditor:setTabImage( ::qEdit )
      ENDIF
      qCursor:setPosition( nPos )
      ::qEdit:setTextCursor( qCursor )
      RETURN .t.
   ENDIF

   RETURN .f.

/*----------------------------------------------------------------------*/

METHOD IdeEdit:highlightAll( cText )
   LOCAL qDoc, qFormat, qCursor, qFormatHL, qCur, lModified

   IF ::unHighLight()
      RETURN Self
   ENDIF

   ::isHighLighted := .t.

   qDoc := ::oEditor:qDocument
   lModified := ::qEdit:document():isModified()

   qCur := ::getCursor()
   qCur:beginEditBlock()

   qCursor   := QTextCursor( "QTextDocument", qDoc )
   qFormat   := qCursor:charFormat()
   qFormatHL := qFormat
   qFormatHL:setBackground( QBrush( QColor( Qt_yellow ) ) )

   DO WHILE .t.
      qCursor := qDoc:find( cText, qCursor, 0 )
      IF qCursor:isNull()
         EXIT
      ENDIF
      qCursor:mergeCharFormat( qFormatHL )
   ENDDO
   qCur:endEditBlock()

   IF ! lModified
      ::qEdit:document():setModified( .f. )
      ::oEditor:setTabImage( ::qEdit )
   ENDIF

   RETURN Self

/*----------------------------------------------------------------------*/

METHOD IdeEdit:refresh()
   ::qEdit:hbRefresh()
   RETURN Self

/*----------------------------------------------------------------------*/
//                       TBrowse Like Navigation
/*----------------------------------------------------------------------*/

METHOD IdeEdit:home()
   ::qEdit:hbApplyKey( Qt_Key_Home )
   RETURN Self

/*----------------------------------------------------------------------*/

METHOD IdeEdit:end()
   ::qEdit:hbApplyKey( Qt_Key_End )
   RETURN Self

/*----------------------------------------------------------------------*/

METHOD IdeEdit:down()
   ::qEdit:hbApplyKey( Qt_Key_Down )
   RETURN Self

/*----------------------------------------------------------------------*/

METHOD IdeEdit:up()
   ::qEdit:hbApplyKey( Qt_Key_Up )
   RETURN Self

/*----------------------------------------------------------------------*/

METHOD IdeEdit:goBottom()
   ::qEdit:hbApplyKey( Qt_Key_End, Qt_ControlModifier )
   RETURN Self

/*----------------------------------------------------------------------*/

METHOD IdeEdit:goTop()
   ::qEdit:hbApplyKey( Qt_Key_Home, Qt_ControlModifier )
   RETURN Self

/*----------------------------------------------------------------------*/

METHOD IdeEdit:left()
   ::qEdit:hbApplyKey( Qt_Key_Left )
   RETURN Self

/*----------------------------------------------------------------------*/

METHOD IdeEdit:right()
   ::qEdit:hbApplyKey( Qt_Key_Right )
   RETURN Self

/*----------------------------------------------------------------------*/

METHOD IdeEdit:panEnd()
   LOCAL qCursor := ::getCursor()
   LOCAL cLine := ::getLine()
   ::qEdit:hbGetViewportInfo()
   IF Len( cLine ) - ::aViewportInfo[ 2 ] > ::aViewportInfo[ 4 ]
      qCursor:movePosition( QTextCursor_Right, QTextCursor_MoveAnchor, Len( cLine ) - ::aViewportInfo[ 2 ] )
   ELSE
      qCursor:movePosition( QTextCursor_EndOfLine )
   ENDIF
   ::qEdit:setTextCursor( qCursor )
   RETURN Self

/*----------------------------------------------------------------------*/

METHOD IdeEdit:panHome()
   LOCAL qCursor := ::getCursor()
   ::qEdit:hbGetViewportInfo()
   IF ::aViewportInfo[ 2 ] == 0
      qCursor:movePosition( QTextCursor_StartOfLine )
   ELSE
      qCursor:movePosition( QTextCursor_Left, QTextCursor_MoveAnchor, qCursor:columnNumber() - ::aViewportInfo[ 2 ] )
   ENDIF
   ::qEdit:setTextCursor( qCursor )
   RETURN Self

/*----------------------------------------------------------------------*/

METHOD IdeEdit:pageUp()
   ::qEdit:hbApplyKey( Qt_Key_PageUp )
   RETURN Self

/*----------------------------------------------------------------------*/

METHOD IdeEdit:pageDown()
   ::qEdit:hbApplyKey( Qt_Key_PageDown )
   RETURN Self

/*----------------------------------------------------------------------*/
//
/*----------------------------------------------------------------------*/

METHOD IdeEdit:printPreview()
   LOCAL qDlg := QPrintPreviewDialog( ::oDlg:oWidget )

   qDlg:setWindowTitle( "hbIDE Preview Dialog" )
   qDlg:connect( "paintRequested(QPrinter*)", {|p| ::paintRequested( p ) } )
 * qDlg:setWindowState( Qt_WindowMaximized )
   qDlg:exec()
   qDlg:disconnect( "paintRequested(QPrinter*)" )

   RETURN self

/*----------------------------------------------------------------------*/

METHOD IdeEdit:paintRequested( qPrinter )
   ::qEdit:print( qPrinter )
   RETURN Self

/*----------------------------------------------------------------------*/

METHOD IdeEdit:upperCaseKeywords()
   LOCAL qDoc, cText, cRegEx, aMatches, aMatch, b_

   qDoc := ::qEdit:document()

   IF !( qDoc:isEmpty() )
      qDoc:setUndoRedoEnabled( .f. )

      cText := qDoc:toPlainText()

      b_:= { 'function','procedure','thread','return','static','local','default', ;
             'if','else','elseif','endif','end', ;
             'docase','case','endcase','otherwise', ;
             'switch','endswitch', ;
             'do','while','exit','enddo','loop',;
             'for','each','next','step','to','in',;
             'with','replace','object','endwith','request',;
             'nil','and','or','in','not','self',;
             'class','endclass','method','data','var','destructor','inline','assign','access',;
             'inherit','init','create','virtual','message', 'from', 'setget',;
             'begin','sequence','try','catch','always','recover','hb_symbol_unused', ;
             'error','handler','private','public' }
      cRegEx := ""
      aeval( b_, {|e| cRegEx += iif( empty( cRegEx ), "", "|" ) + "\b" + e + "\b" } )

      aMatches := hb_regExAll( cRegEx, cText, .f., .f., 0, 1, .f. )

      IF ! empty( aMatches )
         FOR EACH aMatch IN aMatches
            cText := stuff( cText, aMatch[ 2 ], aMatch[ 3 ] - aMatch[ 2 ] + 1, upper( aMatch[ 1 ] ) )
         NEXT
      ENDIF

      qDoc:clear()
      qDoc:setPlainText( cText )

      qDoc:setUndoRedoEnabled( .t. )
   ENDIF

   RETURN Self

/*----------------------------------------------------------------------*/

METHOD IdeEdit:formatBraces()
   LOCAL qDoc, cText

   qDoc := ::qEdit:document()

   IF !( qDoc:isEmpty() )
      qDoc:setUndoRedoEnabled( .f. )

      cText := qDoc:toPlainText()

      cText := strtran( cText, "( ", "(" )
      cText := strtran( cText, "(  ", "(" )
      cText := strtran( cText, "(   ", "(" )
      cText := strtran( cText, "(    ", "(" )
      cText := strtran( cText, "(     ", "(" )
      cText := strtran( cText, "(      ", "(" )
      cText := strtran( cText, " (", "(" )
      cText := strtran( cText, "  (", "(" )
      cText := strtran( cText, "   (", "(" )
      cText := strtran( cText, "    (", "(" )
      cText := strtran( cText, "     (", "(" )

      cText := strtran( cText, "      )", ")" )
      cText := strtran( cText, "     )", ")" )
      cText := strtran( cText, "    )", ")" )
      cText := strtran( cText, "   )", ")" )
      cText := strtran( cText, "  )", ")" )
      cText := strtran( cText, " )", ")" )

      cText := strtran( cText, "(", "( " )
      cText := strtran( cText, ")", " )" )

      cText := strtran( cText, "(     )", "()" )
      cText := strtran( cText, "(    )", "()" )
      cText := strtran( cText, "(   )", "()" )
      cText := strtran( cText, "(  )", "()" )
      cText := strtran( cText, "( )", "()" )

      qDoc:clear()
      qDoc:setPlainText( cText )

      qDoc:setUndoRedoEnabled( .t. )
   ENDIF
   RETURN Self

/*----------------------------------------------------------------------*/

METHOD IdeEdit:removeTrailingSpaces()
   LOCAL qDoc, cText, a_, s

   qDoc := ::qEdit:document()
   IF !( qDoc:isEmpty() )
      qDoc:setUndoRedoEnabled( .f. )
      cText := qDoc:toPlainText()
      a_:= hbide_memoToArray( cText )
      FOR EACH s IN a_
         s := trim( s )
      NEXT
      cText := hbide_arrayToMemo( a_ )
      qDoc:clear()
      qDoc:setPlainText( cText )
      qDoc:setUndoRedoEnabled( .t. )
   ENDIF
   RETURN Self

/*----------------------------------------------------------------------*/

METHOD IdeEdit:tabs2spaces()
   LOCAL qDoc, cText, cSpaces

   qDoc := ::qEdit:document()
   IF !( qDoc:isEmpty() )
      cSpaces := space( ::nTabSpaces )

      qDoc:setUndoRedoEnabled( .f. )

      cText := qDoc:toPlainText()
      qDoc:clear()
      qDoc:setPlainText( strtran( cText, chr( 9 ), cSpaces ) )

      qDoc:setUndoRedoEnabled( .t. )
   ENDIF
   RETURN Self

/*----------------------------------------------------------------------*/

METHOD IdeEdit:spaces2tabs()
   LOCAL qDoc, cText, cSpaces

   qDoc := ::qEdit:document()
   IF !( qDoc:isEmpty() )
      cSpaces := space( ::nTabSpaces )

      qDoc:setUndoRedoEnabled( .f. )

      cText := qDoc:toPlainText()
      qDoc:clear()
      qDoc:setPlainText( strtran( cText, cSpaces, chr( 9 ) ) )

      qDoc:setUndoRedoEnabled( .t. )
   ENDIF
   RETURN Self

/*----------------------------------------------------------------------*/

METHOD IdeEdit:duplicateLine()
   ::qEdit:hbDuplicateLine()
   RETURN Self

/*----------------------------------------------------------------------*/

METHOD IdeEdit:deleteLine()
   ::qEdit:hbDeleteLine()
   RETURN Self

/*----------------------------------------------------------------------*/

METHOD IdeEdit:moveLine( nDirection )
   ::qEdit:hbMoveLine( nDirection )
   RETURN Self

/*----------------------------------------------------------------------*/

METHOD IdeEdit:getText()
   RETURN ::qEdit:textCursor():selectedText()

/*----------------------------------------------------------------------*/

METHOD IdeEdit:getWord( lSelect )
   LOCAL cText, qCursor := ::qEdit:textCursor()

   DEFAULT lSelect TO .F.

   qCursor:select( QTextCursor_WordUnderCursor )
   cText := qCursor:selectedText()

   IF lSelect
      ::qEdit:setTextCursor( qCursor )
   ENDIF
   RETURN cText

/*----------------------------------------------------------------------*/

METHOD IdeEdit:goto( nLine )
   LOCAL nRows, qGo
   LOCAL qCursor := ::qEdit:textCursor()

   IF empty( nLine )
      nRows := ::qEdit:blockCount()
      nLine := qCursor:blockNumber()

      qGo := QInputDialog( ::oDlg:oWidget )
      qGo:setInputMode( 1 )
      qGo:setIntMinimum( 1 )
      qGo:setIntMaximum( nRows )
      qGo:setIntValue( nLine + 1 )
      qGo:setLabelText( "Goto Line Number [1-" + hb_ntos( nRows ) + "]" )
      qGo:setWindowTitle( "Harbour" )

      ::oIde:setPosByIniEx( qGo, ::oINI:cGotoDialogGeometry )
      qGo:exec()
      ::oIde:oINI:cGotoDialogGeometry := hbide_posAndSize( qGo )
      nLine := qGo:intValue()
      ::qEdit:setFocus()
   ENDIF

   qCursor:movePosition( QTextCursor_Start )
   qCursor:movePosition( QTextCursor_Down, QTextCursor_MoveAnchor, nLine - 1 )
   ::qEdit:setTextCursor( qCursor )

   RETURN Self

/*----------------------------------------------------------------------*/

METHOD IdeEdit:getLine( nLine, lSelect )
   LOCAL cText, qCursor := ::qEdit:textCursor()

   DEFAULT nLine   TO qCursor:blockNumber() + 1
   DEFAULT lSelect TO .F.

   IF nLine != qCursor:blockNumber() + 1
      qCursor:movePosition( QTextCursor_Start )
      qCursor:movePosition( QTextCursor_Down, QTextCursor_MoveAnchor, nLine - 1 )
   ENDIF

   qCursor:select( QTextCursor_LineUnderCursor )
   cText := qCursor:selectedText()
   IF lSelect
      ::qEdit:setTextCursor( qCursor )
   ENDIF

   RETURN cText

/*----------------------------------------------------------------------*/

METHOD IdeEdit:getColumnNo()
   RETURN ::qEdit:textCursor():columnNumber() + 1

/*----------------------------------------------------------------------*/

METHOD IdeEdit:getLineNo()
   RETURN ::qEdit:textCursor():blockNumber() + 1

/*----------------------------------------------------------------------*/

METHOD IdeEdit:insertSeparator( cSep )
   LOCAL qCursor := ::qEdit:textCursor()

   IF empty( cSep )
      cSep := ::cSeparator
   ENDIF
   qCursor:beginEditBlock()
   qCursor:movePosition( QTextCursor_StartOfBlock )
   qCursor:insertBlock()
   qCursor:movePosition( QTextCursor_PreviousBlock )
   qCursor:insertText( cSep )
   qCursor:movePosition( QTextCursor_NextBlock )
   qCursor:movePosition( QTextCursor_StartOfBlock )
   qCursor:endEditBlock()

   RETURN Self

/*----------------------------------------------------------------------*/

METHOD IdeEdit:insertText( cText )
   LOCAL qCursor, nL, nB

   IF HB_ISSTRING( cText ) .AND. !Empty( cText )
      qCursor := ::qEdit:textCursor()

      nL := Len( cText )
      nB := qCursor:position() + nL

      qCursor:beginEditBlock()
      qCursor:removeSelectedText()
      qCursor:insertText( cText )
      qCursor:setPosition( nB )
      qCursor:endEditBlock()
   ENDIF
   RETURN Self

/*----------------------------------------------------------------------*/
/* called via qDocument:contentsChange(*/

METHOD IdeEdit:reformatLine( nPos, nAdded, nDeleted )
   LOCAL cProto, nRows, nCols
   //LOCAL cWord, nColumn
   //LOCAL qCursor := ::qEdit:textCursor()
   //LOCAL cLine := ::getLine()

   HB_SYMBOL_UNUSED( nPos )
   HB_SYMBOL_UNUSED( nAdded )
   HB_SYMBOL_UNUSED( nDeleted )

   //nColumn := qCursor:columnNumber() + 1
   //cWord := ::getWord()

   //HB_TRACE( HB_TR_ALWAYS, nPos, nAdded, nDeleted, nColumn, len( cWord ), cWord )

   ::handlePreviousWord( ::lUpdatePrevWord )
   ::handleCurrentIndent()

   IF ::nProtoLine != -1
      IF ::getLineNo() == ::nProtoLine .AND. ::getColumnNo() >= ::nProtoCol + 1
         IF !empty( cProto := hbide_formatProto_1( ::cProtoOrg, ::getLine(), ::nProtoCol, ::getColumnNo(), @nRows, @nCols ) )
            ::cProto     := cProto
            ::nProtoRows := nRows
            ::nProtoCols := nCols
            ::showProtoType()
         ENDIF
      ENDIF
   ENDIF

   RETURN Self

/*----------------------------------------------------------------------*/

METHOD IdeEdit:handlePreviousWord( lUpdatePrevWord )
   LOCAL qCursor, qTextBlock, cText, cWord, nB, nL, qEdit, lPrevOnly, nCol, nSpace, nSpaces, nOff

   IF ! lUpdatePrevWord
      RETURN Self
   ENDIF
   ::lUpdatePrevWord := .f.

   qEdit := ::qEdit

   qCursor    := qEdit:textCursor()
   qTextBlock := qCursor:block()
   cText      := qTextBlock:text()
   nCol       := qCursor:columnNumber()
   IF ( substr( cText, nCol - 1, 1 ) == " " )
      RETURN nil
   ENDIF
   nSpace := iif( substr( cText, nCol, 1 ) == " ", 1, 0 )
   cWord  := hbide_getPreviousWord( cText, nCol + 1 )

   IF !empty( cWord ) .AND. hbide_isHarbourKeyword( cWord, ::oIde )
      lPrevOnly := left( lower( ltrim( cText ) ), Len( cWord ) ) == lower( cWord )

      nL := Len( cWord ) + nSpace
      nB := qCursor:position() - nL

      IF lower( ::oEditor:cExt ) $ ".prg,.hb" .AND. ! ::oINI:lSupressHbKWordsToUpper
         qCursor:beginEditBlock()
         qCursor:setPosition( nB )
         qCursor:movePosition( QTextCursor_NextCharacter, QTextCursor_KeepAnchor, nL )
         qCursor:removeSelectedText()
         qCursor:insertText( upper( cWord ) + space( nSpace ) )
         qCursor:endEditBlock()
         qEdit:setTextCursor( qCursor )
      ENDIF

      IF hbide_isStartingKeyword( cWord, ::oIde )
         IF lPrevOnly
            qCursor:setPosition( nB )
            IF ( nCol := qCursor:columnNumber() ) > 0
               qCursor:beginEditBlock()
               qCursor:movePosition( QTextCursor_StartOfBlock )
               qCursor:movePosition( QTextCursor_NextCharacter, QTextCursor_KeepAnchor, nCol )
               qCursor:removeSelectedText()
               qCursor:movePosition( QTextCursor_NextCharacter, QTextCursor_MoveAnchor, nL )
               qCursor:endEditBlock()
               qEdit:setTextCursor( qCursor )
            ENDIF
         ENDIF

      ELSEIF hbide_isMinimumIndentableKeyword( cWord, ::oIde ) .AND. ::oINI:lAutoIndent
         IF lPrevOnly
            qCursor:setPosition( nB )
            IF ( nCol := qCursor:columnNumber() ) >= 0
               qCursor:beginEditBlock()
               qCursor:movePosition( QTextCursor_StartOfBlock )
               qCursor:movePosition( QTextCursor_NextCharacter, QTextCursor_KeepAnchor, nCol )
               qCursor:removeSelectedText()
               qCursor:insertText( space( ::nTabSpaces ) )
               qCursor:movePosition( QTextCursor_NextCharacter, QTextCursor_MoveAnchor, nL )
               qEdit:setTextCursor( qCursor )
               qCursor:endEditBlock()
            ENDIF
         ENDIF

      ELSEIF hbide_isIndentableKeyword( cWord, ::oIde ) .AND. ::oINI:lAutoIndent
         IF lPrevOnly
            nSpaces := hbide_getFrontSpacesAndWord( cText )
            IF nSpaces > 0 .AND. ( nOff := nSpaces % ::nTabSpaces ) > 0
               qCursor:setPosition( nB )
               qCursor:beginEditBlock()
               qCursor:movePosition( QTextCursor_PreviousCharacter, QTextCursor_KeepAnchor, nOff )
               qCursor:removeSelectedText()
               qCursor:movePosition( QTextCursor_NextCharacter, QTextCursor_MoveAnchor, nL )
               qEdit:setTextCursor( qCursor )
               qCursor:endEditBlock()
            ENDIF
         ENDIF
      ENDIF
   ENDIF

   RETURN .t.

/*----------------------------------------------------------------------*/

METHOD IdeEdit:findLastIndent()
   LOCAL qCursor, qTextBlock, cText, cWord
   LOCAL nSpaces := 0

   qCursor := ::qEdit:textCursor()
   qTextBlock := qCursor:block()

   qTextBlock := qTextBlock:previous()
   DO WHILE .t.
      IF !( qTextBlock:isValid() )
         EXIT
      ENDIF
      IF !empty( cText := qTextBlock:text() )
         nSpaces := hbide_getFrontSpacesAndWord( cText, @cWord )
         IF !empty( cWord )
            IF ::oINI:lSmartIndent .AND. hbide_isIndentableKeyword( cWord, ::oIde )
               nSpaces += ::nTabSpaces
            ENDIF
            EXIT
         ENDIF
      ENDIF
      qTextBlock := qTextBlock:previous()
   ENDDO

   RETURN nSpaces

/*----------------------------------------------------------------------*/

METHOD IdeEdit:handleCurrentIndent()
   LOCAL qCursor, nSpaces

   IF ::lIndentIt
      ::lIndentIt := .f.
      IF ( nSpaces := ::findLastIndent() ) > 0
         qCursor := ::qEdit:textCursor()
         qCursor:insertText( space( nSpaces ) )
      ENDIF
   ENDIF

   RETURN Self

/*----------------------------------------------------------------------*/

METHOD IdeEdit:gotoFunction()
   LOCAL cWord, n
   LOCAL lFindCur := .f.
   IF !empty( cWord := ::getWord( .f. ) )
      IF ( n := ascan( ::aTags, {|e_| lower( cWord ) $ lower( e_[ 7 ] ) } ) ) > 0
         IF ::find( alltrim( ::aTags[ n,8 ] ) )
            lFindCur := .t.
         ENDIF
      ENDIF
      IF ! lFindCur
         ::oFN:jumpToFunction( cWord, .t. )
      ENDIF
   ENDIF
   RETURN Self

/*----------------------------------------------------------------------*/

METHOD IdeEdit:clickFuncHelp()
   LOCAL cWord
   IF !empty( cWord := ::getWord( .f. ) )
      IF ! empty( ::oHL )
         ::oHL:jumpToFunction( cWord )
      ENDIF
   ENDIF
   RETURN Self

/*----------------------------------------------------------------------*/

METHOD IdeEdit:loadFuncHelp()
   LOCAL qEdit, qCursor, qTextBlock, cText, cWord, nCol, cPro

   qEdit := ::qEdit
   qCursor    := qEdit:textCursor()
   qTextBlock := qCursor:block()
   cText      := qTextBlock:text()
   nCol       := qCursor:columnNumber()
   cWord      := hbide_getPreviousWord( cText, nCol )

   IF !empty( cWord )
      IF empty( cPro := ::oEM:getProto( cWord ) )
         IF ! empty( ::oHL )
            ::oHL:jumpToFunction( cWord )
         ENDIF
         IF !empty( cPro := ::oFN:positionToFunction( cWord, .t. ) )
            IF empty( ::cProto )
               ::showPrototype( cPro )
            ENDIF
         ENDIF
      ELSE
         ::showPrototype( cPro )
      ENDIF
   ENDIF
   RETURN Self

/*----------------------------------------------------------------------*/

METHOD IdeEdit:resumePrototype()

   ::isSuspended := .f.
   IF !empty( ::qEdit )
      IF ::getLineNo() == ::nProtoLine .AND. ::getColumnNo() >= ::nProtoCol
         ::qEdit:hbShowPrototype( ::cProto, ::nProtoRows, ::nProtoCols )
      ENDIF
   ENDIF

   RETURN Self

/*----------------------------------------------------------------------*/

METHOD IdeEdit:suspendPrototype()

   ::isSuspended := .t.
   IF !empty( ::qEdit )
      ::qEdit:hbShowPrototype( "", 0, 0 )
   ENDIF

   RETURN Self

/*----------------------------------------------------------------------*/

METHOD IdeEdit:showPrototype( cProto )

   IF ! ::isSuspended  .AND. !empty( ::qEdit )
      IF !empty( cProto )
         ::cProtoOrg  := cProto
         ::cProto     := hbide_formatProto( cProto )
         ::nProtoLine := ::getLineNo()
         ::nProtoCol  := ::getColumnNo()
      ENDIF
      ::qEdit:hbShowPrototype( ::cProto, ::nProtoRows, ::nProtoCols )
   ENDIF
   RETURN Self

/*----------------------------------------------------------------------*/

METHOD IdeEdit:hidePrototype()

   IF !empty( ::qedit )
      ::nProtoLine := -1
      ::nProtoCol  := -1
      ::cProto     := ""
      ::nProtoCols := 10
      ::nProtoRows := 1
      ::qEdit:hbShowPrototype( "", 0, 0 )
   ENDIF
   RETURN Self

/*----------------------------------------------------------------------*/

METHOD IdeEdit:parseCodeCompletion( cSyntax )
   LOCAL cText, n, nFun, nAbr, nSpc

   nAbr := at( "-", cSyntax )
   nSpc := at( " ", cSyntax )
   nFun := at( "(", cSyntax )

   IF "[f]" $ cSyntax
      cText := alltrim( substr( cSyntax, 1, nSpc ) )

   ELSEIF nAbr > 0 .AND. iif( nSpc == 0, .t., nAbr < nSpc ).AND. iif( nFun == 0, .t., nAbr < nFun )
      cText := alltrim( substr( cSyntax, nAbr + 1 ) )

   ELSE
      IF ::oINI:lCompleteArgumented
         IF ( n := rat( ")", cSyntax ) ) > 0
            cText := trim( substr( cSyntax, 1, n ) )
         ELSE
            cText := trim( cSyntax )
         ENDIF
      ELSE
         IF nFun > 0 .AND. iif( nSpc == 0, .t., nFun < nSpc )
            cText := trim( substr( cSyntax, 1, nFun - 1 ) )
         ELSE
            cText := trim( cSyntax )
         ENDIF
      ENDIF
   ENDIF

   RETURN cText

/*----------------------------------------------------------------------*/

METHOD IdeEdit:completeCode( p )
   LOCAL qCursor := ::qEdit:textCursor()
   LOCAL cWord

   qCursor:movePosition( QTextCursor_Left )

   qCursor:movePosition( QTextCursor_StartOfWord )
   qCursor:movePosition( QTextCursor_EndOfWord, QTextCursor_KeepAnchor )

   cWord := qCursor:selectedText()
   IF cWord == "->"
      qCursor:clearSelection()
   ENDIF

   qCursor:insertText( ::parseCodeCompletion( p ) )
   qCursor:movePosition( QTextCursor_Left )
   qCursor:movePosition( QTextCursor_Right )

   ::qEdit:setTextCursor( qCursor )

   ::qEdit:hbSetFieldsListActive( ::oEM:updateFieldsList() )

   RETURN Self

/*----------------------------------------------------------------------*/

FUNCTION hbide_getPreviousWord( cText, nPos )
   LOCAL cWord, n

   cText := alltrim( substr( cText, 1, nPos ) )
   IF ( n := rat( " ", cText ) ) > 0
      cWord := substr( cText, n + 1 )
   ELSE
      cWord := cText
   ENDIF

   RETURN cWord

/*----------------------------------------------------------------------*/

FUNCTION hbide_getFirstWord( cText )
   LOCAL cWord, n

   cText := alltrim( cText )
   IF ( n := at( " ", cText ) ) > 0
      cWord := left( cText, n-1 )
   ELSE
      cWord := cText
   ENDIF

   RETURN cWord

/*----------------------------------------------------------------------*/

FUNCTION hbide_getFrontSpacesAndWord( cText, cWord )
   LOCAL n := 0

   DO WHILE .t.
      IF !( substr( cText, ++n, 1 ) == " " )
         EXIT
      ENDIF
   ENDDO
   n--

   cWord := hbide_getFirstWord( cText )

   RETURN n

/*----------------------------------------------------------------------*/

FUNCTION hbide_isStartingKeyword( cWord, oIde )
   LOCAL s_b_

   IF empty( s_b_ )
      IF ! oIde:oINI:lReturnAsBeginKeyword
         s_b_ := { ;
                    'function' => NIL,;
                    'class' => NIL,;
                    'method' => NIL }
      ELSE
         s_b_ := { ;
                    'function' => NIL,;
                    'class' => NIL,;
                    'return' => NIL,;
                    'method' => NIL }
      ENDIF
   ENDIF

   RETURN Lower( cWord ) $ s_b_

/*----------------------------------------------------------------------*/

FUNCTION hbide_isMinimumIndentableKeyword( cWord, oIde )
   LOCAL s_b_

   IF empty( s_b_ )
      IF ! oIde:oINI:lReturnAsBeginKeyword
         s_b_ := { ;
                    'local' => NIL,;
                    'static' => NIL,;
                    'return' => NIL,;
                    'default' => NIL }
      ELSE
         s_b_ := { ;
                    'local' => NIL,;
                    'static' => NIL,;
                    'default' => NIL }
      ENDIF
   ENDIF

   RETURN Lower( cWord ) $ s_b_

/*----------------------------------------------------------------------*/

FUNCTION hbide_isIndentableKeyword( cWord, oIde )
   STATIC s_b_ := { ;
                    'if' => NIL,;
                    'else' => NIL,;
                    'elseif' => NIL,;
                    'docase' => NIL,;
                    'case' => NIL,;
                    'otherwise' => NIL,;
                    'do' => NIL,;
                    'while' => NIL,;
                    'switch' => NIL,;
                    'for' => NIL,;
                    'next' => NIL,;
                    'begin' => NIL,;
                    'sequence' => NIL,;
                    'try' => NIL,;
                    'catch' => NIL,;
                    'always' => NIL,;
                    'recover' => NIL,;
                    'finally' => NIL }

   HB_SYMBOL_UNUSED( oIde )

   RETURN Lower( cWord ) $ s_b_

/*----------------------------------------------------------------------*/

FUNCTION hbide_harbourKeywords()
   STATIC s_b_ := { ;
                    'function' => NIL,;
                    'procedure' => NIL,;
                    'thread' => NIL,;
                    'return' => NIL,;
                    'request' => NIL,;
                    'static' => NIL,;
                    'local' => NIL,;
                    'default' => NIL,;
                    'if' => NIL,;
                    'else' => NIL,;
                    'elseif' => NIL,;
                    'endif' => NIL,;
                    'end' => NIL,;
                    'endswitch' => NIL,;
                    'docase' => NIL,;
                    'case' => NIL,;
                    'endcase' => NIL,;
                    'otherwise' => NIL,;
                    'switch' => NIL,;
                    'do' => NIL,;
                    'while' => NIL,;
                    'enddo' => NIL,;
                    'exit' => NIL,;
                    'for' => NIL,;
                    'each' => NIL,;
                    'next' => NIL,;
                    'step' => NIL,;
                    'to' => NIL,;
                    'class' => NIL,;
                    'endclass' => NIL,;
                    'method' => NIL,;
                    'data' => NIL,;
                    'var' => NIL,;
                    'destructor' => NIL,;
                    'inline' => NIL,;
                    'setget' => NIL,;
                    'assign' => NIL,;
                    'access' => NIL,;
                    'inherit' => NIL,;
                    'init' => NIL,;
                    'create' => NIL,;
                    'virtual' => NIL,;
                    'message' => NIL,;
                    'begin' => NIL,;
                    'sequence' => NIL,;
                    'try' => NIL,;
                    'catch' => NIL,;
                    'always' => NIL,;
                    'recover' => NIL,;
                    'with' => NIL,;
                    'replace' => NIL,;
                    'hb_symbol_unused' => NIL,;
                    'error' => NIL,;
                    'handler' => NIL,;
                    'loop' => NIL,;
                    'in' => NIL,;
                    'nil' => NIL,;
                    'or' => NIL,;
                    'not' => NIL,;
                    'and' => NIL }

   RETURN s_b_

/*----------------------------------------------------------------------*/

FUNCTION hbide_isHarbourKeyword( cWord, oIde )

   HB_SYMBOL_UNUSED( oIde )

   RETURN Lower( cWord ) $ hbide_harbourKeywords()

/*----------------------------------------------------------------------*/

FUNCTION hbide_formatProto_1( cProto, cText, nProtoCol, nCurCol, nRows, nCols )
   LOCAL s, nArgs, cArgs, aArgs, cArg, n, n1, i, nnn, cPro, cFunc

   IF nCurCol > nProtoCol
      n  := at( "(", cProto ) ; n1 := at( ")", cProto )
      IF n > 0 .AND. n1 > 0 .AND. "," $ cProto
         cProto := substr( cProto, 1, n1 )

         s := substr( cText, nProtoCol, nCurCol - nProtoCol )
         nArgs := 1
         FOR i := 1 TO Len( s )
            IF substr( s, i, 1 ) == ","
               nArgs++
            ENDIF
         NEXT

         nRows := 1; nCols := 0

         IF nArgs > 0
            n := at( "(", cProto ) ; n1 := at( ")", cProto )

            cFunc := substr( cProto, 1, n - 1 )
            cArgs := substr( cProto, n + 1, n1 - n - 1 )
            aArgs := hb_aTokens( cArgs, "," )
            cArgs := ""
            nCols := Len( cFunc ) + 1
            FOR EACH cArg IN aArgs
               cArg := alltrim( cArg )

               nRows++
               nCols := max( nCols, Len( cArg ) + 3 )

               cArg := StrTran( cArg, "<", "&lt;" )
               cArg := StrTran( cArg, ">", "&gt;" )

               nnn  := cArg:__enumIndex()
               IF nnn == nArgs
                  cArg := "<font color=red><b>" + cArg + "</b></font>"
               ENDIF
               IF nnn == Len( aArgs )
                  cArgs += "<br>" + "   " + cArg
               ELSE
                  cArgs += "<br>" + "   " + cArg + "<font color=red><b>" + "," + "</b></font>"
               ENDIF
            NEXT
            nCols += iif( nCols <= Len( cFunc ), 0, 1 )

            //cPro  := "<p style='white-space:pre'>" + "<font color=darkgreen><b>" + cFunc + "</b></font>" + ;
            cPro  := "<p style='white-space:pre'>" + "<b>" + cFunc + "</b>" + ;
                        "<font color=red><b>" + "(" + "</b></font>" + ;
                           cArgs + ;
                              "<font color=red><b>" + ")" + "</font>" + "</b></p>"
         ENDIF
      ENDIF
   ENDIF

   RETURN cPro

/*------------------------------------------------------------------------*/

FUNCTION hbide_formatProto( cProto )
   LOCAL n, n1, cArgs

   cProto := StrTran( cProto, "<", "&lt;" )
   cProto := StrTran( cProto, ">", "&gt;" )

   n  := at( "(", cProto )
   n1 := at( ")", cProto )

   IF n > 0 .AND. n1 > 0
      cArgs  := substr( cProto, n + 1, n1 - n - 1 )
      cArgs  := strtran( cArgs, ",", "<font color=red><b>" + "," + "</b></font>" )
      cProto := "<p style='white-space:pre'>" + "<b>" + substr( cProto, 1, n - 1 ) + "</b>" + ;
                   "<font color=red><b>" + "(" + "</b></font>" + ;
                      cArgs + ;
                         "<font color=red><b>" + ")" + "</font>" + "</b></p>"
   ENDIF
   RETURN cProto

/*----------------------------------------------------------------------*/

STATIC FUNCTION hbide_normalizeRect( aCord, nT, nL, nB, nR )
   nT := iif( aCord[ 1 ] > aCord[ 3 ], aCord[ 3 ], aCord[ 1 ] )
   nB := iif( aCord[ 1 ] > aCord[ 3 ], aCord[ 1 ], aCord[ 3 ] )
   nL := iif( aCord[ 2 ] > aCord[ 4 ], aCord[ 4 ], aCord[ 2 ] )
   nR := iif( aCord[ 2 ] > aCord[ 4 ], aCord[ 2 ], aCord[ 4 ] )
   RETURN NIL

/*----------------------------------------------------------------------*/

