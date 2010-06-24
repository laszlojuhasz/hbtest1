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
#include "hbqt.ch"
#include "hbide.ch"
#include "xbp.ch"

/*----------------------------------------------------------------------*/

#define customContextMenuRequested                1
#define textChanged                               2
#define copyAvailable                             3
#define modificationChanged                       4
#define redoAvailable                             5
#define selectionChanged                          6
#define undoAvailable                             7
#define updateRequest                             8
#define cursorPositionChanged                     9

#define timerTimeout                              23

#define selectionMode_stream                      1
#define selectionMode_column                      2
#define selectionMode_line                        3

/*----------------------------------------------------------------------*/

CLASS IdeEdit INHERIT IdeObject

   DATA   oEditor

   DATA   qEdit
   DATA   qHLayout
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
   DATA   qTimer
   DATA   nProtoLine                              INIT -1
   DATA   nProtoCol                               INIT -1
   DATA   isSuspended                             INIT .F.

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

   METHOD new( oIde, oEditor, nMode )
   METHOD create( oIde, oEditor, nMode )
   METHOD destroy()
   METHOD execEvent( nMode, oEdit, p, p1 )
   METHOD execKeyEvent( nMode, nEvent, p, p1 )
   METHOD connectEditSignals( oEdit )
   METHOD disconnectEditSignals( oEdit )

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
   METHOD getCursor()                             INLINE QTextCursor():from( ::qEdit:textCursor() )
   METHOD find( cText, nPosFrom )
   METHOD refresh()
   METHOD isModified()                            INLINE ::oEditor:qDocument:isModified()
   METHOD setFont()
   METHOD markCurrentFunction()
   METHOD copyBlockContents( aCord )
   METHOD pasteBlockContents( nMode )
   METHOD insertBlockContents( aCord )
   METHOD deleteBlockContents( aCord )
   METHOD zoom( nKey )
   METHOD blockConvert( cMode )
   METHOD dispStatusInfo()
   METHOD toggleCurrentLineHighlightMode()

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
   METHOD paintRequested( pPrinter )
   METHOD tabs2spaces()
   METHOD spaces2tabs()
   METHOD removeTrailingSpaces()
   METHOD formatBraces()
   METHOD findEx( cText, nFlags, nStart )
   METHOD highlightAll( cText )
   METHOD unHighlight()

   ENDCLASS

/*----------------------------------------------------------------------*/

METHOD IdeEdit:new( oIde, oEditor, nMode )

   ::oIde    := oIde
   ::oEditor := oEditor
   ::nMode   := nMode

   ::fontFamily       := ::oINI:cFontName
   ::pointSize        := ::oINI:nPointSize
   ::currentPointSize := ::oINI:nPointSize

   RETURN Self

/*----------------------------------------------------------------------*/

METHOD IdeEdit:create( oIde, oEditor, nMode )
   LOCAL nBlock

   DEFAULT oIde    TO ::oIde
   DEFAULT oEditor TO ::oEditor
   DEFAULT nMode   TO ::nMode

   ::oIde    := oIde
   ::oEditor := oEditor
   ::nMode   := nMode
   //::oIde    := ::oEditor:oIde

   ::qEdit   := HBQPlainTextEdit():new()
   //
   ::qEdit:setLineWrapMode( QTextEdit_NoWrap )
   ::qEdit:ensureCursorVisible()
   ::qEdit:setContextMenuPolicy( Qt_CustomContextMenu )
   ::qEdit:installEventFilter( ::pEvents )
   ::qEdit:setTabChangesFocus( .f. )

   ::setFont()

   ::qEdit:hbSetSpaces( ::nTabSpaces )

   ::qEdit:hbSetCompleter( ::qCompleter )

   ::toggleCurrentLineHighlightMode()
   ::toggleLineNumbers()
   ::toggleHorzRuler()

   FOR EACH nBlock IN ::aBookMarks
      ::qEdit:hbBookMarks( nBlock )
   NEXT

   ::qHLayout := QHBoxLayout():new()
   ::qHLayout:setContentsMargins( 0,0,0,0 )
   ::qHLayout:setSpacing( 0 )

   ::qHLayout:addWidget( ::qEdit )

   ::connectEditSignals( Self )

   Qt_Events_Connect( ::pEvents, ::qEdit, QEvent_KeyPress           , {|p| ::execKeyEvent( 101, QEvent_KeyPress, p ) } )
   Qt_Events_Connect( ::pEvents, ::qEdit, QEvent_Wheel              , {|p| ::execKeyEvent( 102, QEvent_Wheel   , p ) } )
   Qt_Events_Connect( ::pEvents, ::qEdit, QEvent_FocusIn            , {| | ::execKeyEvent( 104, QEvent_FocusIn     ) } )
   Qt_Events_Connect( ::pEvents, ::qEdit, QEvent_Resize             , {| | ::execKeyEvent( 106, QEvent_Resize      ) } )
   Qt_Events_Connect( ::pEvents, ::qEdit, QEvent_FocusOut           , {| | ::execKeyEvent( 105, QEvent_FocusOut    ) } )
   Qt_Events_Connect( ::pEvents, ::qEdit, QEvent_MouseButtonDblClick, {|p| ::execKeyEvent( 103, QEvent_MouseButtonDblClick, p ) } )

   ::qEdit:hbSetEventBlock( {|p,p1| ::execKeyEvent( 115, 1001, p, p1 ) } )

   ::qTimer := QTimer():new()
   ::qTimer:setInterval( 2000 )
   ::connect( ::qTimer, "timeout()",  {|| ::execEvent( timerTimeout, Self ) } )

   RETURN Self

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

   ::qFont := QFont():new()
   ::qFont:setFamily( ::fontFamily )
   ::qFont:setFixedPitch( .t. )
   ::qFont:setPointSize( ::currentPointSize )

   ::qEdit:setFont( ::qFont )

   RETURN Self

/*----------------------------------------------------------------------*/

METHOD IdeEdit:destroy()

   ::disconnect( ::qTimer, "timeout()" )
   IF ::qTimer:isActive()
      ::qTimer:stop()
   ENDIF
   ::qTimer := NIL

   Qt_Events_DisConnect( ::pEvents, ::qEdit, QEvent_KeyPress            )
   Qt_Events_DisConnect( ::pEvents, ::qEdit, QEvent_Wheel               )
   Qt_Events_DisConnect( ::pEvents, ::qEdit, QEvent_FocusIn             )
   Qt_Events_DisConnect( ::pEvents, ::qEdit, QEvent_FocusOut            )
   Qt_Events_DisConnect( ::pEvents, ::qEdit, QEvent_MouseButtonDblClick )

   ::disconnectEditSignals( Self )

   ::oEditor:qLayout:removeItem( ::qHLayout )
   //
   ::qHLayout:removeWidget( ::qEdit )
   ::qEdit    := NIL
   ::qHLayout := NIL
   ::qFont    := NIL

   RETURN Self

/*----------------------------------------------------------------------*/

METHOD IdeEdit:disconnectEditSignals( oEdit )
   HB_SYMBOL_UNUSED( oEdit )

   ::disConnect( oEdit:qEdit, "customContextMenuRequested(QPoint)" )
   ::disConnect( oEdit:qEdit, "textChanged()"                      )
   ::disConnect( oEdit:qEdit, "selectionChanged()"                 )
   ::disConnect( oEdit:qEdit, "cursorPositionChanged()"            )
   ::disConnect( oEdit:qEdit, "copyAvailable(bool)"                )

   #if 0
   ::disConnect( oEdit:qEdit, "modificationChanged(bool)"          )
   ::disConnect( oEdit:qEdit, "updateRequest(QRect,int)"           )
   ::disConnect( oEdit:qEdit, "redoAvailable(bool)"                )
   ::disConnect( oEdit:qEdit, "undoAvailable(bool)"                )
   #endif

   RETURN Self

/*----------------------------------------------------------------------*/

METHOD IdeEdit:connectEditSignals( oEdit )
   HB_SYMBOL_UNUSED( oEdit )

   ::connect( oEdit:qEdit, "customContextMenuRequested(QPoint)", {|p   | ::execEvent( 1, oEdit, p     ) } )
   ::Connect( oEdit:qEdit, "textChanged()"                     , {|    | ::execEvent( 2, oEdit,       ) } )
   ::Connect( oEdit:qEdit, "selectionChanged()"                , {|p   | ::execEvent( 6, oEdit, p     ) } )
   ::Connect( oEdit:qEdit, "cursorPositionChanged()"           , {|    | ::execEvent( 9, oEdit,       ) } )
   ::Connect( oEdit:qEdit, "copyAvailable(bool)"               , {|p   | ::execEvent( 3, oEdit, p     ) } )

   #if 0
   ::Connect( oEdit:qEdit, "modificationChanged(bool)"         , {|p   | ::execEvent( 4, oEdit, p     ) } )
   ::Connect( oEdit:qEdit, "updateRequest(QRect,int)"          , {|p,p1| ::execEvent( 8, oEdit, p, p1 ) } )
   ::Connect( oEdit:qEdit, "redoAvailable(bool)"               , {|p   | ::execEvent( 5, oEdit, p     ) } )
   ::Connect( oEdit:qEdit, "undoAvailable(bool)"               , {|p   | ::execEvent( 7, oEdit, p     ) } )
   #endif

   RETURN Self

/*----------------------------------------------------------------------*/

METHOD IdeEdit:execEvent( nMode, oEdit, p, p1 )
   LOCAL pAct, qAct, n, qEdit, oo, nLine, qCursor

   HB_SYMBOL_UNUSED( p1 )

   qEdit   := oEdit:qEdit
   qCursor := QTextCursor():configure( qEdit:textCursor() )
   oEdit:nCurLineNo := qCursor:blockNumber()

   SWITCH nMode

   CASE customContextMenuRequested
      QAction():from( ::oEM:aActions[ 17, 2 ] ):setEnabled( !empty( qCursor:selectedText() ) )

      pAct := ::oEM:qContextMenu:exec_1( qEdit:mapToGlobal( p ) )
      IF !hbqt_isEmptyQtPointer( pAct )
         qAct := QAction():configure( pAct )
         DO CASE
         CASE qAct:text() == "Split Horizontally"
            ::oEditor:split( 1, oEdit )
         CASE qAct:text() == "Split Vertically"
            ::oEditor:split( 2, oEdit )
         CASE qAct:text() == "Close Split Window"
            IF ( n := ascan( ::oEditor:aEdits, {|o| o == oEdit } ) ) > 0  /* 1 == Main Edit */
               oo := ::oEditor:aEdits[ n ]
               hb_adel( ::oEditor:aEdits, n, .t. )
               oo:destroy()
               ::oEditor:relay()
               ::oEditor:qCqEdit := ::oEditor:qEdit
               ::oEditor:qCoEdit := ::oEditor:oEdit
               ::oIde:manageFocusInEditor()
            ENDIF
         CASE qAct:text() == "Save as Skeleton..."
            ::oSK:saveAs( ::getSelectedText() )
         CASE qAct:text() == "Apply Theme"
            ::oEditor:applyTheme()
         CASE qAct:text() == "Goto Function"
            ::gotoFunction()
         ENDCASE
      ENDIF
      EXIT


   CASE textChanged
      //HB_TRACE( HB_TR_ALWAYS, "textChanged()" )
      ::oEditor:setTabImage( qEdit )
      EXIT

   CASE selectionChanged
      //HB_TRACE( HB_TR_ALWAYS, "selectionChanged()" )
      ::oEditor:qCqEdit := qEdit
      ::oEditor:qCoEdit := oEdit

      /* Book Marks reach-out buttons */
      ::relayMarkButtons()
      ::updateTitleBar()

      ::toggleCurrentLineHighlightMode()
      ::toggleLineNumbers()
      ::toggleHorzRuler()
      ::dispStatusInfo()

      ::qEdit:hbGetSelectionInfo()
      IF ::aSelectionInfo[ 1 ] > -1 .AND. ::aSelectionInfo[ 1 ] == ::aSelectionInfo[ 3 ]
         ::oDK:setStatusText( SB_PNL_SELECTEDCHARS, len( ::getSelectedText() ) )
      ELSE
         ::oDK:setStatusText( SB_PNL_SELECTEDCHARS, 0 )
      ENDIF
      ::oUpDn:show()
      ::unHighlight()

      EXIT

   CASE cursorPositionChanged
      //HB_TRACE( HB_TR_ALWAYS, "cursorPositionChanged()", ::nProtoLine, ::nProtoCol, ::isSuspended, ::getLineNo(), ::getColumnNo(), ::cProto )
      ::oEditor:dispEditInfo( qEdit )
      ::handlePreviousWord( ::lUpdatePrevWord )
      ::handleCurrentIndent()

      ::markCurrentFunction()

      IF ::nProtoLine != -1
         nLine := ::getLineNo()
         IF ! ::isSuspended
            IF nLine != ::nProtoLine .OR. ::getColumnNo() <= ::nProtoCol
               ::suspendPrototype()
            ENDIF
         ELSE
            IF nLine == ::nProtoLine .AND. ::getColumnNo() >= ::nProtoCol
               ::resumePrototype()
            ENDIF
         ENDIF
      ENDIF

      EXIT

   CASE copyAvailable
      IF p .AND. ::lCopyWhenDblClicked
         ::qEdit:copy()
      ENDIF
      ::lCopyWhenDblClicked := .f.
      EXIT

   CASE timerTimeout
      IF empty( ::cProto )
         ::hidePrototype()
      ELSE
         ::showPrototype()
      ENDIF
      EXIT

   #if 0
   CASE modificationChanged
      //HB_TRACE( HB_TR_ALWAYS, "modificationChanged(bool)", p )
      EXIT
   CASE redoAvailable
      //HB_TRACE( HB_TR_ALWAYS, "redoAvailable(bool)", p )
      EXIT
   CASE undoAvailable
      //HB_TRACE( HB_TR_ALWAYS, "undoAvailable(bool)", p )
      EXIT
   CASE updateRequest
      EXIT
   #endif
   ENDSWITCH

   RETURN Nil

/*----------------------------------------------------------------------*/

METHOD IdeEdit:dispStatusInfo()
   LOCAL nMode

   ::qEdit:hbGetSelectionInfo()
   nMode := ::aSelectionInfo[ 5 ]

   ::oAC:getAction( "TB_SelectionMode" ):setIcon( hbide_image( iif( nMode == 3, "selectionline", "stream" ) ) )
   ::oAC:getAction( "TB_SelectionMode" ):setChecked( nMode > 1 )

   ::oDK:setStatusText( SB_PNL_STREAM, iif( nMode == 2, "Column", iif( nMode == 3, "Line", "Stream" ) ) )

   RETURN Self

/*----------------------------------------------------------------------*/

METHOD IdeEdit:execKeyEvent( nMode, nEvent, p, p1 )
   LOCAL key, kbm, qEvent, lAlt, lCtrl, lShift

   HB_SYMBOL_UNUSED( nMode )
   HB_SYMBOL_UNUSED( p1 )

   SWITCH nEvent
   CASE QEvent_KeyPress

      qEvent := QKeyEvent():configure( p )

      key    := qEvent:key()
      kbm    := qEvent:modifiers()

      lAlt   := hb_bitAnd( kbm, Qt_AltModifier     ) == Qt_AltModifier
      lCtrl  := hb_bitAnd( kbm, Qt_ControlModifier ) == Qt_ControlModifier
      lShift := hb_bitAnd( kbm, Qt_ShiftModifier   ) == Qt_ShiftModifier

      IF ::oSC:execKey( Self, key, lAlt, lCtrl, lShift )
         RETURN .f.
      ENDIF

      SWITCH ( key )
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
      ENDSWITCH

      EXIT

   CASE QEvent_Enter
   CASE QEvent_FocusIn
      ::resumePrototype()
      IF key == QEvent_FocusIn
         ::oUpDn:show()
      ENDIF
      EXIT

   CASE QEvent_Resize
      ::oUpDn:show()
      EXIT

   CASE QEvent_Leave
   CASE QEvent_FocusOut
      ::suspendPrototype()
      EXIT

   CASE QEvent_Wheel
      EXIT

   CASE QEvent_MouseButtonDblClick
      ::lCopyWhenDblClicked := .t.
      EXIT

   CASE 1001
      IF p == QEvent_MouseButtonDblClick
         ::lCopyWhenDblClicked := .f.       /* not intuitive */
         ::clickFuncHelp()

      ELSEIF p == QEvent_Paint
         // ::oIde:testPainter( p1 )

      ELSEIF p == 21000                     /* Sends Block Info { t,l,b,r,mode,state } hbGetBlockInfo() */
         ::aSelectionInfo := p1

      ELSEIF p == 21001
         ::handlePreviousWord( .t. )

      ELSEIF p == 21011
         ::copyBlockContents( p1 )

      ELSEIF p == 21012
         ::pasteBlockContents( p1 )

      ELSEIF p == 21013
         ::insertBlockContents( p1 )

      ELSEIF p == 21014
         ::deleteBlockContents( p1 )

      ELSEIF p == 21017                     /* Sends Block Info { t,l,b,r,mode,state } hbGetBlockInfo() */
         ::aViewportInfo := p1

      ENDIF
      EXIT

   ENDSWITCH

   RETURN .F.  /* Important */

/*----------------------------------------------------------------------*/

STATIC FUNCTION hbide_blockContents( aContents )
   LOCAL oldContents
   STATIC contents := {}

   oldContents := contents
   IF hb_isArray( aContents )
      contents := aclone( aContents )
   ENDIF

   RETURN oldContents

/*----------------------------------------------------------------------*/

STATIC FUNCTION hbide_setQCursor( qEdit, q_ )
   LOCAL qCursor

   IF hb_isArray( q_ )
      qCursor := q_[ 1 ]
      qCursor:movePosition( QTextCursor_Start, QTextCursor_MoveAnchor )
      qCursor:movePosition( QTextCursor_Down , QTextCursor_MoveAnchor, q_[ 2 ] )
      qCursor:movePosition( QTextCursor_Right, QTextCursor_MoveAnchor, q_[ 3 ] )
      qEdit:setTextCursor( qCursor )
      qCursor:endEditBlock()
   ELSE
      qCursor := QTextCursor():from( qEdit:textCursor() )
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
   nLen := len( cBuffer )
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

METHOD IdeEdit:copyBlockContents( aCord )
   LOCAL nT, nL, nB, nR, nW, i, cLine, nMode, qClip, oLine
   LOCAL cClip := ""

   HB_TRACE( HB_TR_DEBUG, "IdeEdit:copyBlockContents( aCord )" )

   hbide_normalizeRect( aCord, @nT, @nL, @nB, @nR )
   nMode := aCord[ 5 ]

   ::aBlockCopyContents := {}

   nW := nR - nL
   FOR i := nT TO nB
      cLine := ::getLine( i + 1 )
      oLine := cLine
      IF nMode == selectionMode_stream
         IF aCord[ 1 ] > aCord[ 3 ]
            IF i == nT .AND. i == nB
               cLine := substr( cLine, min( aCord[ 2 ], aCord[ 4 ] ) + 1, nW )
            ELSEIF i == aCord[ 1 ]
               cLine := substr( cLine, 1, aCord[ 2 ] )
            ELSEIF i == aCord[ 3 ]
               cLine := substr( cLine, aCord[ 4 ] + 1 )
            ENDIF
         ELSE
            IF i == nT .AND. i == nB
               cLine := substr( cLine, min( aCord[ 2 ], aCord[ 4 ] ) + 1, nW )
            ELSEIF i == aCord[ 1 ]
               cLine := substr( cLine, aCord[ 2 ] + 1 )
            ELSEIF i == aCord[ 3 ]
               cLine := substr( cLine, 1, aCord[ 4 ] )
            ENDIF
         ENDIF

      ELSEIF nMode == selectionMode_column
         cLine := pad( substr( cLine, nL + 1, nW ), nW )

      ELSEIF nMode == selectionMode_line
         // Nothing to do, complete line is already pulled

      ENDIF

      aadd( ::aBlockCopyContents, cLine )
      cClip += cLine + iif( i < nB, hb_osNewLine(), iif( cLine == oLine, hb_osNewLine(), "" ) )
   NEXT

 * HB_TRACE( HB_TR_ALWAYS, "copyBlockContents", cClip )

   hbide_blockContents( { nMode, ::aBlockCopyContents } )

   qClip := QClipboard():new()
   qClip:clear()
   qClip:setText( cClip )

   RETURN Self

/*----------------------------------------------------------------------*/

METHOD IdeEdit:pasteBlockContents( nMode )
   LOCAL i, nCol, qCursor, nMaxCol, aCopy, a_, nPasteMode

   IF ::lReadOnly
      RETURN Self
   ENDIF

// aCopy := hbide_memoToArray( QClipboard():new():text() )
   aCopy := hb_ATokens( StrTran( RTrim( QClipboard():new():text() ), Chr( 13 ) + Chr( 10 ), _EOL ), _EOL )
   IF empty( aCopy[ len( aCopy ) ] )
      hb_adel( aCopy, len( aCopy ), .t. )
   ENDIF
   IF empty( aCopy )
      RETURN Self
   ENDIF

   nPasteMode := nMode   /* OR Stream - needs to be thought carefully */

   a_:= hbide_blockContents()
   IF !empty( a_ )
      IF ( len( a_[ 2 ] ) == len( aCopy ) ) .OR. ( len( a_[ 2 ] ) == len( aCopy ) + 1 )
         IF a_[ 2,1 ] == aCopy[ 1 ]
            nPasteMode := a_[ 1 ]
         ENDIF
      ENDIF
   ENDIF

   nPasteMode := iif( empty( nPasteMode ), selectionMode_stream, nPasteMode )
   qCursor    := QTextCursor():from( ::qEdit:textCursor() )
   nCol       := qCursor:columnNumber()

   qCursor:beginEditBlock()
   //
   DO CASE
   CASE nPasteMode == selectionMode_column
      FOR i := 1 TO len( aCopy )
         qCursor:insertText( aCopy[ i ] )
         IF i < len( aCopy )
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

   CASE nPasteMode == selectionMode_stream
      FOR i := 1 TO len( aCopy )
         qCursor:insertText( aCopy[ i ] )
         IF i < len( aCopy )
            qCursor:insertText( hb_osNewLine() )
         ENDIF
      NEXT

   CASE nPasteMode == selectionMode_line
      qCursor:movePosition( QTextCursor_StartOfLine, QTextCursor_MoveAnchor       )
      FOR i := 1 TO len( aCopy )
         qCursor:insertText( aCopy[ i ] )
         qCursor:insertBlock()
      NEXT

   ENDCASE
   //
   qCursor:endEditBlock()
   ::qEdit:ensureCursorVisible()

   RETURN Self

/*----------------------------------------------------------------------*/

METHOD IdeEdit:insertBlockContents( aCord )
   LOCAL nT, nL, nB, nR, nW, i, cLine, cKey, qCursor

   IF ::lReadOnly
      RETURN Self
   ENDIF

   hbide_normalizeRect( aCord, @nT, @nL, @nB, @nR )

   nW := nR - nL

   cKey := chr( XbpQKeyEventToAppEvent( aCord[ 7 ] ) )

   qCursor := QTextCursor():from( ::qEdit:textCursor() )
   qCursor:beginEditBlock()

   IF nW == 0
      FOR i := nT TO nB
         cLine := ::getLine( i + 1 )
         cLine := pad( substr( cLine, 1, nL ), nL ) + cKey + substr( cLine, nL + 1 )
         hbide_qReplaceLine( qCursor, i, cLine )
      NEXT

      hbide_qPositionCursor( qCursor, nB, nR + 1 )
   ELSE
      FOR i := nT TO nB
         cLine := ::getLine( i + 1 )
         cLine := pad( substr( cLine, 1, nL ), nL ) + replicate( cKey, nW ) + substr( cLine, nR + 1 )

         hbide_qReplaceLine( qCursor, i, cLine )
      NEXT

      hbide_qPositionCursor( qCursor, nB, nR )
   ENDIF
   //
   ::qEdit:setCursorWidth( 1 )
   ::qEdit:setTextCursor( qCursor )
   qCursor:endEditBlock()

   RETURN Self

/*----------------------------------------------------------------------*/

METHOD IdeEdit:deleteBlockContents( aCord )
   LOCAL nT, nL, nB, nR, nW, i, cLine, qCursor, k, nSelMode

   IF ::lReadOnly
      RETURN Self
   ENDIF

   hbide_normalizeRect( aCord, @nT, @nL, @nB, @nR )
   k := aCord[ 7 ]
   k := iif( empty( k ), Qt_Key_X, k )
   IF k == Qt_Key_X
      ::copyBlockContents( aCord )
   ENDIF

   nSelMode := aCord[ 5 ]

   qCursor := QTextCursor():from( ::qEdit:textCursor() )
   qCursor:beginEditBlock()

   nW := nR - nL
   IF nW == 0 .AND. k == Qt_Key_Backspace
      FOR i := nT TO nB
         cLine := ::getLine( i + 1 )
         cLine := pad( substr( cLine, 1, nL - 1 ), nL - 1 ) + substr( cLine, nL + 1 )
         hbide_qReplaceLine( qCursor, i, cLine )
      NEXT
      hbide_qPositionCursor( qCursor, nB, nR - 1 )

   ELSE
      IF k == Qt_Key_Delete .OR. k == Qt_Key_X
         IF nSelMode == selectionMode_column
            FOR i := nT TO nB
               cLine := ::getLine( i + 1 )
               cLine := pad( substr( cLine, 1, nL ), nL ) + substr( cLine, nR + 1 )
               hbide_qReplaceLine( qCursor, i, cLine )
            NEXT
            hbide_qPositionCursor( qCursor, nT, nL )

         ELSEIF nSelMode == selectionMode_stream
            hbide_qPositionCursor( qCursor, nT, nL )
            qCursor:movePosition( QTextCursor_Down       , QTextCursor_KeepAnchor, nB - nT )
            qCursor:movePosition( QTextCursor_StartOfLine, QTextCursor_KeepAnchor          )
            qCursor:movePosition( QTextCursor_Right      , QTextCursor_KeepAnchor, nR      )
            qCursor:removeSelectedText()
            ::qEdit:hbSetSelectionInfo( { -1,-1,-1,-1,1 } )

         ELSEIF nSelMode == selectionMode_line
            hbide_qPositionCursor( qCursor, nT, nL )
            qCursor:movePosition( QTextCursor_Down       , QTextCursor_KeepAnchor, nB - nT + 1 )
            qCursor:movePosition( QTextCursor_StartOfLine, QTextCursor_KeepAnchor          )
            qCursor:removeSelectedText()
            ::qEdit:hbSetSelectionInfo( { -1,-1,-1,-1,1 } )
            ::isLineSelectionON := .f.

         ENDIF
      ENDIF
   ENDIF
   //
   ::qEdit:setTextCursor( qCursor )
   qCursor:endEditBlock()

   RETURN Self

/*----------------------------------------------------------------------*/

METHOD IdeEdit:blockComment()
   LOCAL nT, nL, nB, nR, nW, i, cLine, qCursor, aCord, nMode, q_
   LOCAL cComment := "// "
   LOCAL nLen := len( cComment )

   IF ::lReadOnly
      RETURN Self
   ENDIF

   ::qEdit:hbGetSelectionInfo(); aCord := ::aSelectionInfo
   hbide_normalizeRect( aCord, @nT, @nL, @nB, @nR )
   nW := nR - nL

   IF nW >= 0
      nMode   := aCord[ 5 ]
      q_:= hbide_setQCursor( ::qEdit ) ; qCursor := q_[ 1 ]

      FOR i := nT TO nB
         cLine := ::getLine( i + 1 )

         DO CASE
         CASE nMode == selectionMode_stream .OR. nMode == selectionMode_line
            IF substr( cLine, 1, nLen ) == cComment
               cLine := substr( cLine, nLen + 1 )
            ELSE
               cLine := cComment + cLine
            ENDIF

         CASE nMode == selectionMode_column
            IF substr( cLine, nL + 1, nLen ) == cComment
               cLine := pad( substr( cLine, 1, nL ), nL ) + substr( cLine, nL + nLen + 1 )
            ELSE
               cLine := pad( substr( cLine, 1, nL ), nL ) + cComment + substr( cLine, nL + 1 )
            ENDIF

         ENDCASE

         hbide_qReplaceLine( qCursor, i, cLine )
      NEXT

      hbide_setQCursor( ::qEdit, q_ )
   ENDIF

   RETURN Self

/*----------------------------------------------------------------------*/

METHOD IdeEdit:streamComment()
   LOCAL nT, nL, nB, nR, nW, i, cLine, qCursor, aCord, nMode, q_

   IF ::lReadOnly
      RETURN Self
   ENDIF

   ::qEdit:hbGetSelectionInfo(); aCord := ::aSelectionInfo

   hbide_normalizeRect( aCord, @nT, @nL, @nB, @nR )
   nW := nR - nL

   IF nW >= 0
      nMode   := aCord[ 5 ]
      q_:= hbide_setQCursor( ::qEdit ) ; qCursor := q_[ 1 ]

      FOR i := nT TO nB
         cLine := ::getLine( i + 1 )

         DO CASE
         CASE nMode == selectionMode_stream
            IF i == nT
               cLine := substr( cLine, 1, nL ) + "/* " + substr( cLine, nL + 1 )
            ELSEIF i == nB
               cLine := substr( cLine, 1, nR ) + " */" + substr( cLine, nR + 1 )
            ENDIF

         CASE nMode == selectionMode_line
            IF i == nT
               cLine := "/* " + cLine
            ELSEIF i == nB
               cLine += " */"
            ENDIF

         ENDCASE

         hbide_qReplaceLine( qCursor, i, cLine )
      NEXT

      hbide_setQCursor( ::qEdit, q_ )
   ENDIF

   RETURN Self

/*----------------------------------------------------------------------*/

METHOD IdeEdit:blockIndent( nDirctn )
   LOCAL nT, nL, nB, nR, nW, i, cLine, qCursor, aCord, q_, nMode, cLineSel

   IF ::lReadOnly
      RETURN Self
   ENDIF

   ::qEdit:hbGetSelectionInfo(); aCord := ::aSelectionInfo
   hbide_normalizeRect( aCord, @nT, @nL, @nB, @nR )
   nW := nR - nL

   IF nW >= 0
      nMode := aCord[ 5 ]
      q_:= hbide_setQCursor( ::qEdit ) ; qCursor := q_[ 1 ]

      FOR i := nT TO nB
         cLine := ::getLine( i + 1 )

         DO CASE
         CASE nMode == selectionMode_stream .OR. nMode == selectionMode_line
            IF nDirctn == -1
               IF left( cLine, 1 ) == " "
                  cLine := substr( cLine, 2 )
               ENDIF
            ELSE
               cLine := " " + cLine
            ENDIF

         CASE nMode == selectionMode_column
            cLineSel := pad( substr( cLine, nL + 1, nW ), nW )
            IF nDirctn == -1
               IF left( cLineSel, 1 ) == " "
                  cLineSel := substr( cLineSel, 2 )
               ENDIF
            ELSE
               cLineSel := " " + cLineSel
            ENDIF
            cLine := pad( substr( cLine, 1, nL ), nL ) + cLineSel + substr( cLine, nR + 1 )

         ENDCASE

         hbide_qReplaceLine( qCursor, i, cLine )
      NEXT

      hbide_setQCursor( ::qEdit, q_ )
   ENDIF

   RETURN Self

/*----------------------------------------------------------------------*/

METHOD IdeEdit:blockConvert( cMode )
   LOCAL nT, nL, nB, nR, nW, i, cLine, qCursor, aCord, q_, nMode

   IF ::lReadOnly
      RETURN Self
   ENDIF

   ::qEdit:hbGetSelectionInfo(); aCord := ::aSelectionInfo
   hbide_normalizeRect( aCord, @nT, @nL, @nB, @nR )
   nW := nR - nL

   IF nW >= 0
      nMode := aCord[ 5 ]
      q_:= hbide_setQCursor( ::qEdit ) ; qCursor := q_[ 1 ]

      FOR i := nT TO nB
         cLine := ::getLine( i + 1 )

         DO CASE
         CASE nMode == selectionMode_stream
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

         CASE nMode == selectionMode_column
            cLine := pad( substr( cLine, 1, nL ), nL ) + hbide_convertALine( pad( substr( cLine, nL + 1, nW ), nW ), cMode ) + substr( cLine, nR + 1 )

         CASE nMode == selectionMode_line
            cLine := hbide_convertALine( cLine, cMode )

         ENDCASE

         hbide_qReplaceLine( qCursor, i, cLine )
      NEXT

      hbide_setQCursor( ::qEdit, q_ )
   ENDIF

   RETURN Self

/*----------------------------------------------------------------------*/

METHOD IdeEdit:getSelectedText()
   LOCAL nT, nL, nB, nR, nW, i, cLine, nMode, cClip := "", aCord

   HB_TRACE( HB_TR_DEBUG, "IdeEdit:getSelectedText()", ProcName( 1 ), procName( 2 ) )

   ::qEdit:hbGetSelectionInfo(); aCord := ::aSelectionInfo
   hbide_normalizeRect( aCord, @nT, @nL, @nB, @nR )
   nMode := aCord[ 5 ]

   nW := nR - nL
   FOR i := nT TO nB
      cLine := ::getLine( i + 1 )

      IF nMode == selectionMode_stream
         IF i == nT .AND. i == nB
            cLine := substr( cLine, nL + 1, nR - nL )
         ELSEIF i == nT
            cLine := substr( cLine, nL + 1 )
         ELSEIF i == nB
            cLine := substr( cLine, 1, nR + 1 )
         ENDIF

      ELSEIF nMode == selectionMode_column
         cLine := pad( substr( cLine, nL + 1, nW ), nW )

      ELSEIF nMode == selectionMode_line
         // Nothing to do, complete line is already pulled

      ENDIF

      cClip += cLine + iif( i < nB, hb_osNewLine(), "" )
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

METHOD IdeEdit:markCurrentFunction()
   LOCAL n, nCurLine

   IF ::nPrevLineNo1 != ::getLineNo()
      ::nPrevLineNo1 := ::getLineNo()

      IF !empty( ::aTags )
         nCurLine := ::getLineNo()
         IF len( ::aTags ) == 1
            n := 1
         ELSEIF ( n := ascan( ::aTags, {|e_| e_[ 3 ] >= nCurLine } ) ) == 0
            n := len( ::aTags )
         ELSEIF n > 0
            n--
         ENDIF
         IF n > 0
            ::oIde:oFuncList:setItemColorFG( n, { 255,0,0 } )
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
   ::qEdit:hbSetSelectionMode( iif( ::oAC:getAction( "TB_SelectionMode" ):isChecked(), 2, 1 ), .f. )
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
   ::qEdit:hbCut( Qt_Key_X )
   RETURN Self

/*----------------------------------------------------------------------*/

METHOD IdeEdit:copy()
   ::qEdit:hbCopy()
   RETURN Self

/*----------------------------------------------------------------------*/

METHOD IdeEdit:paste()
   IF ::lReadOnly
      RETURN Self
   ENDIF
   ::qEdit:hbPaste()
   RETURN Self

/*----------------------------------------------------------------------*/

METHOD IdeEdit:selectAll()
   ::qEdit:selectAll()
   RETURN Self

/*----------------------------------------------------------------------*/

METHOD IdeEdit:setReadOnly( lReadOnly )
   IF ::oEditor:lReadOnly
      lReadOnly := .t.
   ELSE
      IF ! hb_isLogical( lReadOnly )
         lReadOnly := ! ::qEdit:isReadOnly()
      ENDIF
   ENDIF
   ::lReadOnly := lReadOnly
   ::qEdit:setReadOnly( lReadOnly )
   ::oEditor:setTabImage()
   RETURN Self

/*----------------------------------------------------------------------*/

METHOD IdeEdit:gotoMark( nIndex )
   IF len( ::aBookMarks ) >= nIndex
      ::qEdit:hbGotoBookmark( ::aBookMarks[ nIndex ] )
      ::qEdit:centerCursor()
   ENDIF
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

METHOD IdeEdit:setNewMark()
   LOCAL qCursor, nBlock, n

   IF !( qCursor := QTextCursor():configure( ::qEdit:textCursor() ) ):isNull()
      nBlock := qCursor:blockNumber() + 1

      IF ( n := ascan( ::aBookMarks, nBlock ) ) > 0
         hb_adel( ::aBookMarks, n, .t. )
         ::aMarkTBtns[ len( ::aBookMarks ) + 1 ]:hide()
      ELSE
         IF len( ::aBookMarks ) == 6
            RETURN Self
         ENDIF
         aadd( ::aBookMarks, nBlock )
         n := len( ::aBookMarks )
         ::aMarkTBtns[ n ]:show()
      ENDIF

      ::qEdit:hbBookMarks( nBlock )
   ENDIF
   RETURN Self

/*----------------------------------------------------------------------*/

METHOD IdeEdit:setLineNumbersBkColor( nR, nG, nB )
   ::qEdit:hbSetLineAreaBkColor( QColor():new( nR, nG, nB ) )
   RETURN Self

/*----------------------------------------------------------------------*/

METHOD IdeEdit:setCurrentLineColor( nR, nG, nB )
   ::qEdit:hbSetCurrentLineColor( QColor():new( nR, nG, nB ) )
   RETURN Self

/*----------------------------------------------------------------------*/
/* TO BE EXTENDED */
METHOD IdeEdit:find( cText, nPosFrom )
   LOCAL lFound, nPos
   LOCAL qCursor := ::getCursor()

   nPos := qCursor:position()
   IF hb_isNumeric( nPosFrom )
      qCursor:setPosition( nPosFrom )
   ENDIF
   ::qEdit:setTextCursor( qCursor )
   IF ! ( lFound := ::qEdit:find( cText, QTextDocument_FindCaseSensitively ) )
      IF ! hb_isNumeric( nPosFrom )
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
   LOCAL qCursor, lFound, cT, nPos

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
      qCursor := ::getCursor()
      cT      := qCursor:selectedText()
      ::qEdit:hbSetSelectionInfo( { qCursor:blockNumber(), qCursor:columnNumber() - len( cT ), ;
                                    qCursor:blockNumber(), qCursor:columnNumber(), 1 } )
      ::qEdit:setTextCursor( qCursor )
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
      lModified := QTextDocument():configure( ::qEdit:document() ):isModified()
      ::qEdit:undo()
      IF ! lModified
         QTextDocument():configure( ::qEdit:document() ):setModified( .f. )
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
   lModified := QTextDocument():configure( ::qEdit:document() ):isModified()

   qCur := ::getCursor()
   qCur:beginEditBlock()

   qCursor   := QTextCursor():new( "QTextDocument", qDoc )
   qFormat   := QTextCharFormat():from( qCursor:charFormat() )
   qFormatHL := qFormat
   qFormatHL:setBackground( QBrush():new( "QColor", QColor():new( Qt_yellow ) ) )

   DO WHILE .t.
      qCursor := QTextCursor():from( qDoc:find( cText, qCursor, 0 ) )
      IF qCursor:isNull()
         EXIT
      ENDIF
      qCursor:mergeCharFormat( qFormatHL )
   ENDDO
   qCur:endEditBlock()

   IF ! lModified
      QTextDocument():configure( ::qEdit:document() ):setModified( .f. )
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
   IF len( cLine ) - ::aViewportInfo[ 2 ] > ::aViewportInfo[ 4 ]
      qCursor:movePosition( QTextCursor_Right, QTextCursor_MoveAnchor, len( cLine ) - ::aViewportInfo[ 2 ] )
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
   LOCAL qDlg := QPrintPreviewDialog():new( ::oDlg:oWidget )

   qDlg:setWindowTitle( "hbIDE Preview Dialog" )
   Qt_Slots_Connect( ::pSlots, qDlg, "paintRequested(QPrinter)", {|p| ::paintRequested( p ) } )
   qDlg:exec()
   Qt_Slots_disConnect( ::pSlots, qDlg, "paintRequested(QPrinter)" )

   RETURN self

/*----------------------------------------------------------------------*/

METHOD IdeEdit:paintRequested( pPrinter )
   LOCAL qPrinter
   qPrinter := QPrinter():configure( pPrinter )
   ::qEdit:print( qPrinter )
   RETURN Self

/*----------------------------------------------------------------------*/

METHOD IdeEdit:formatBraces()
   LOCAL qDoc, cText

   qDoc := QTextDocument():configure( ::qEdit:document() )

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

   qDoc := QTextDocument():from( ::qEdit:document() )
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

   qDoc := QTextDocument():configure( ::qEdit:document() )
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

   qDoc := QTextDocument():configure( ::qEdit:document() )
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
   RETURN QTextCursor():from( ::qEdit:textCursor() ):selectedText()

/*----------------------------------------------------------------------*/

METHOD IdeEdit:getWord( lSelect )
   LOCAL cText, qCursor := QTextCursor():configure( ::qEdit:textCursor() )

   DEFAULT lSelect TO .F.

   qCursor:select( QTextCursor_WordUnderCursor )
   cText := qCursor:selectedText()

   IF lSelect
      ::qEdit:setTextCursor( qCursor )
   ENDIF
   RETURN cText

/*----------------------------------------------------------------------*/

METHOD IdeEdit:goto( nLine )
   LOCAL qCursor := QTextCursor():configure( ::qEdit:textCursor() )

   qCursor:movePosition( QTextCursor_Start )
   qCursor:movePosition( QTextCursor_Down, QTextCursor_MoveAnchor, nLine - 1 )
   ::qEdit:setTextCursor( qCursor )

   RETURN Self

/*----------------------------------------------------------------------*/

METHOD IdeEdit:getLine( nLine, lSelect )
   LOCAL cText, qCursor := QTextCursor():configure( ::qEdit:textCursor() )

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
   RETURN QTextCursor():from( ::qEdit:textCursor() ):columnNumber() + 1

/*----------------------------------------------------------------------*/

METHOD IdeEdit:getLineNo()
   RETURN QTextCursor():from( ::qEdit:textCursor() ):blockNumber() + 1

/*----------------------------------------------------------------------*/

METHOD IdeEdit:insertSeparator( cSep )
   LOCAL qCursor := QTextCursor():configure( ::qEdit:textCursor() )

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

   IF hb_isChar( cText ) .AND. !Empty( cText )
      qCursor := QTextCursor():configure( ::qEdit:textCursor() )

      nL := len( cText )
      nB := qCursor:position() + nL

      qCursor:beginEditBlock()
      qCursor:removeSelectedText()
      qCursor:insertText( cText )
      qCursor:setPosition( nB )
      qCursor:endEditBlock()
   ENDIF
   RETURN Self

/*----------------------------------------------------------------------*/

METHOD IdeEdit:handlePreviousWord( lUpdatePrevWord )
   LOCAL qCursor, qTextBlock, cText, cWord, nB, nL, qEdit, lPrevOnly, nCol, nSpace, nSpaces, nOff

 * HB_TRACE( HB_TR_ALWAYS, "IdeEdit:handlePreviousWord( lUpdatePrevWord )", lUpdatePrevWord )

   IF ! lUpdatePrevWord
      RETURN Self
   ENDIF
   ::lUpdatePrevWord := .f.

   qEdit := ::qEdit

   qCursor    := QTextCursor():configure( qEdit:textCursor() )
   qTextBlock := QTextBlock():configure( qCursor:block() )
   cText      := qTextBlock:text()
   nCol       := qCursor:columnNumber()
   IF ( substr( cText, nCol - 1, 1 ) == " " )
      RETURN nil
   ENDIF
   nSpace := iif( substr( cText, nCol, 1 ) == " ", 1, 0 )
   cWord  := hbide_getPreviousWord( cText, nCol + 1 )

   IF !empty( cWord ) .AND. hbide_isHarbourKeyword( cWord, ::oIde )
      lPrevOnly := left( lower( ltrim( cText ) ), len( cWord ) ) == lower( cWord )

      nL := len( cWord ) + nSpace
      nB := qCursor:position() - nL

      IF ::oEditor:cExt $ ".prg" .AND. ! ::oINI:lSupressHbKWordsToUpper
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

   qCursor := QTextCursor():configure( ::qEdit:textCursor() )
   qTextBlock := QTextBlock():configure( qCursor:block() )

   qTextBlock := QTextBlock():configure( qTextBlock:previous() )
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
      qTextBlock := QTextBlock():configure( qTextBlock:previous() )
   ENDDO

   RETURN nSpaces

/*----------------------------------------------------------------------*/

METHOD IdeEdit:handleCurrentIndent()
   LOCAL qCursor, nSpaces

   IF ::lIndentIt
      ::lIndentIt := .f.
      IF ( nSpaces := ::findLastIndent() ) > 0
         qCursor := QTextCursor():configure( ::qEdit:textCursor() )
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
   qCursor    := QTextCursor():configure( qEdit:textCursor() )
   qTextBlock := QTextBlock():configure( qCursor:block() )
   cText      := qTextBlock:text()
   nCol       := qCursor:columnNumber()
   cWord      := hbide_getPreviousWord( cText, nCol )

   IF !empty( cWord )
      IF ! empty( ::oHL )
         ::oHL:jumpToFunction( cWord )
      ENDIF
      IF !empty( cPro := ::oFN:positionToFunction( cWord, .t. ) )
         IF empty( ::cProto )
            ::showPrototype( ::cProto := hbide_formatProto( cPro ) )
         ENDIF
      ENDIF
   ENDIF
   RETURN Self

/*----------------------------------------------------------------------*/

METHOD IdeEdit:resumePrototype()

   ::isSuspended := .f.
   IF !empty( ::qEdit )
      IF ::getLineNo() == ::nProtoLine .AND. ::getColumnNo() >= ::nProtoCol
         ::qEdit:hbShowPrototype( ::cProto )
      ENDIF
   ENDIF

   RETURN Self

/*----------------------------------------------------------------------*/

METHOD IdeEdit:suspendPrototype()

   ::isSuspended := .t.
   IF !empty( ::qEdit )
      ::qEdit:hbShowPrototype( "" )
   ENDIF

   RETURN Self

/*----------------------------------------------------------------------*/

METHOD IdeEdit:showPrototype( cProto )

   IF ! ::isSuspended  .AND. !empty( ::qEdit )
      IF !empty( cProto )
         ::cProto     := cProto
         ::nProtoLine := ::getLineNo()
         ::nProtoCol  := ::getColumnNo()
         ::qTimer:start()
      ENDIF
      ::qEdit:hbShowPrototype( ::cProto )
   ENDIF
   RETURN Self

/*----------------------------------------------------------------------*/

METHOD IdeEdit:hidePrototype()

   IF !empty( ::qedit )
      ::nProtoLine := -1
      ::nProtoCol  := -1
      ::cProto     := ""
      ::qTimer:stop()
      ::qEdit:hbShowPrototype( "" )
   ENDIF
   RETURN Self

/*----------------------------------------------------------------------*/

METHOD IdeEdit:completeCode( p )
   LOCAL qCursor := QTextCursor():from( ::qEdit:textCursor() )

   qCursor:movePosition( QTextCursor_Left )

   qCursor:movePosition( QTextCursor_StartOfWord )
   qCursor:movePosition( QTextCursor_EndOfWord, QTextCursor_KeepAnchor )
   qCursor:insertText( p )
   qCursor:movePosition( QTextCursor_Left )
   qCursor:movePosition( QTextCursor_Right )

   ::qEdit:setTextCursor( qCursor )

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
      IF substr( cText, ++n, 1 ) != " "
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

FUNCTION hbide_isHarbourKeyword( cWord, oIde )
   STATIC s_b_ := { ;
                    'function' => NIL,;
                    'return' => NIL,;
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
                    'hb_symbol_unused' => NIL,;
                    'error' => NIL,;
                    'handler' => NIL,;
                    'loop' => NIL,;
                    'in' => NIL,;
                    'nil' => NIL,;
                    'or' => NIL,;
                    'and' => NIL }

   HB_SYMBOL_UNUSED( oIde )

   RETURN Lower( cWord ) $ s_b_

/*----------------------------------------------------------------------*/

FUNCTION hbide_formatProto( cProto )
   LOCAL n, n1, cArgs

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

