/*
 * $Id$
 */

/*
 * Harbour Project source code:
 *
 * Copyright 2009-2010 Pritpal Bedi <bedipritpal@hotmail.com>
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

#define blockCountChanged                         21
#define contentsChange                            22
#define timerTimeout                              23

#define qcompleter_activated                      101
#define qTab_contextMenu                          111

#define EDT_LINNO_WIDTH                           50

/*----------------------------------------------------------------------*/

CLASS IdeEditsManager INHERIT IdeObject

   DATA   qContextMenu
   DATA   aActions                                INIT  {}

   METHOD new( oIde )
   METHOD create( oIde )
   METHOD destroy()
   METHOD removeSourceInTree( cSourceFile )
   METHOD addSourceInTree( cSourceFile, cView )
   METHOD execEvent( nMode, p )
   METHOD buildEditor( cSourceFile, nPos, nHPos, nVPos, cTheme, cView, aBookMarks )
   METHOD getTabBySource( cSource )
   METHOD getTabCurrent()
   METHOD getDocumentCurrent()
   METHOD getEditObjectCurrent()
   METHOD getEditCurrent()
   METHOD getEditorCurrent()
   METHOD getEditorByIndex( nIndex )
   METHOD getEditorByTabObject( oTab )
   METHOD getEditorByTabPosition( nPos )
   METHOD getEditorBySource( cSource )
   METHOD reLoad( cSource )
   METHOD isOpen( cSource )
   METHOD setSourceVisible( cSource )
   METHOD setSourceVisibleByIndex( nIndex )
   METHOD undo()
   METHOD redo()
   METHOD cut()
   METHOD copy()
   METHOD paste()
   METHOD selectAll()
   METHOD switchToReadOnly()
   METHOD convertSelection( cKey )
   METHOD insertText( cKey )
   METHOD insertSeparator( cSep )
   METHOD zoom( nKey )
   METHOD printPreview()
   METHOD paintRequested( pPrinter )
   METHOD setMark()
   METHOD gotoMark( nIndex )
   METHOD goto( nLine )
   METHOD formatBraces()
   METHOD removeTabs()
   METHOD RemoveTrailingSpaces()
   METHOD getSelectedText()
   METHOD duplicateLine()
   METHOD deleteLine()
   METHOD moveLine( nDirection )
   METHOD streamComment()
   METHOD blockComment()
   METHOD indent( nStep )
   METHOD convertQuotes()
   METHOD convertDQuotes()
   METHOD toggleSelectionMode()
   METHOD toggleLineNumbers()
   METHOD toggleLineSelectionMode()

   METHOD getText()
   METHOD getWord( lSelect )
   METHOD getLine( nLine, lSelect )
   METHOD presentSkeletons()
   METHOD gotoFunction()
   METHOD clearSelection()

   ENDCLASS

/*----------------------------------------------------------------------*/

METHOD IdeEditsManager:new( oIde )

   ::oIde := oIde

   RETURN Self

/*----------------------------------------------------------------------*/

METHOD IdeEditsManager:create( oIde )
   LOCAL oSub

   DEFAULT oIde TO ::oIde

   ::oIde := oIde

   ::qContextMenu := QMenu():new()

   aadd( ::aActions, { "GotoFunc"     , ::qContextMenu:addAction_4( ::oAC:getAction( "GotoFunc"      ) ) } )
   aadd( ::aActions, { ""             , ::qContextMenu:addSeparator() } )
   aadd( ::aActions, { "TB_Cut"       , ::qContextMenu:addAction_4( ::oAC:getAction( "TB_Cut"        ) ) } )
   aadd( ::aActions, { "TB_Copy"      , ::qContextMenu:addAction_4( ::oAC:getAction( "TB_Copy"       ) ) } )
   aadd( ::aActions, { "TB_Paste"     , ::qContextMenu:addAction_4( ::oAC:getAction( "TB_Paste"      ) ) } )
   aadd( ::aActions, { ""             , ::qContextMenu:addSeparator() } )
   aadd( ::aActions, { "TB_Undo"      , ::qContextMenu:addAction_4( ::oAC:getAction( "TB_Undo"       ) ) } )
   aadd( ::aActions, { "TB_Redo"      , ::qContextMenu:addAction_4( ::oAC:getAction( "TB_Redo"       ) ) } )
   aadd( ::aActions, { ""             , ::qContextMenu:addSeparator() } )
   aadd( ::aActions, { "TB_Save"      , ::qContextMenu:addAction_4( ::oAC:getAction( "TB_Save"       ) ) } )
   aadd( ::aActions, { "TB_Close"     , ::qContextMenu:addAction_4( ::oAC:getAction( "TB_Close"      ) ) } )
   aadd( ::aActions, { ""             , ::qContextMenu:addSeparator() } )
   aadd( ::aActions, { "TB_Compile"   , ::qContextMenu:addAction_4( ::oAC:getAction( "TB_Compile"    ) ) } )
   aadd( ::aActions, { "TB_CompilePPO", ::qContextMenu:addAction_4( ::oAC:getAction( "TB_CompilePPO" ) ) } )
   aadd( ::aActions, { ""             , ::qContextMenu:addSeparator() } )
   aadd( ::aActions, { "Apply Theme"  , ::qContextMenu:addAction( "Apply Theme"                        ) } )
   aadd( ::aActions, { "Save as Skltn", ::qContextMenu:addAction( "Save as Skeleton..."                ) } )

   oSub := QMenu():configure( ::qContextMenu:addMenu_1( "Split" ) )
   //
   aadd( ::aActions, { "Split H"      , oSub:addAction( "Split Horizontally" ) } )
   aadd( ::aActions, { "Split V"      , oSub:addAction( "Split Vertically"   ) } )
   aadd( ::aActions, { ""             , oSub:addSeparator() } )
   aadd( ::aActions, { "Close Split"  , oSub:addAction( "Close Split Window" ) } )

   ::oIde:qProtoList := QStringList():new()
   ::oIde:qCompModel := QStringListModel():new()
   ::oIde:qCompleter := QCompleter():new()

   ::qCompModel:setStringList( ::qProtoList )
   ::qCompleter:setModel( ::qCompModel )
   ::qCompleter:setModelSorting( QCompleter_CaseInsensitivelySortedModel )
   ::qCompleter:setCaseSensitivity( Qt_CaseInsensitive )
   ::qCompleter:setCompletionMode( QCompleter_PopupCompletion )
   ::qCompleter:setWrapAround( .f. )

   ::connect( ::qCompleter, "activated(QString)", {|p| ::execEvent( qcompleter_activated, p ) } )

   RETURN Self

/*----------------------------------------------------------------------*/

METHOD IdeEditsManager:destroy()
   LOCAL a_

   ::disconnect( ::qCompleter, "activated(QString)" )
   ::oIde:qCompModel := NIL
   ::oIde:qProtoList := NIL
   ::oIde:qCompleter := NIL

   FOR EACH a_ IN ::aActions
      a_[ 2 ] := NIL
   NEXT
   ::aActions := NIL
   ::qContextMenu := NIL

   RETURN Self

/*----------------------------------------------------------------------*/

METHOD IdeEditsManager:removeSourceInTree( cSourceFile )
   LOCAL n

   IF !Empty( cSourceFile )
      IF ( n := aScan( ::aProjData, {|e_| e_[ TRE_ORIGINAL ] == cSourceFile .AND. e_[ 2 ] == "Opened Source" } ) ) > 0
         ::aProjData[ n,3 ]:delItem( ::oIde:aProjData[ n,1 ] )
         hb_adel( ::aProjData, n, .T. )
      ENDIF
   ENDIF

   RETURN Self

/*----------------------------------------------------------------------*/

METHOD IdeEditsManager:addSourceInTree( cSourceFile, cView )
   LOCAL cPath, cFile, cExt, oItem
   //LOCAL oGrand := ::oOpenedSources
   LOCAL oParent := ::oOpenedSources

   IF Empty( cSourceFile )
      RETURN Self
   ENDIF

   hb_fNameSplit( cSourceFile, @cPath, @cFile, @cExt )
   #if 0
   cPathA := hbide_pathNormalized( cPath )

array2table(
   IF ( n := ascan( ::aEditorPath, {|e_| e_[ 2 ] == cPathA } ) ) == 0
      oParent := oGrand:addItem( cPath )
      aadd( ::aProjData, { oParent, "Editor Path", oGrand, cPathA, cSourceFile } )
      aadd( ::aEditorPath, { oParent, cPathA } )
   ELSE
      oParent := ::aEditorPath[ n,1 ]
   ENDIF

   aadd( ::aProjData, { oParent:addItem( cFile + cExt ), "Opened Source", oParent, ;
                                   cSourceFile, hbide_pathNormalized( cSourceFile ) } )
   #endif

   oItem := oParent:addItem( cFile + cExt )
   oItem:tooltipText := cSourceFile
   oItem:oWidget:setIcon( 0, ::oDK:getPanelIcon( cView ) )
   aadd( ::aProjData, { oItem, "Opened Source", oParent, ;
                                   cSourceFile, hbide_pathNormalized( cSourceFile ) } )

   ::oEditTree:oWidget:sortItems( 0 )

   RETURN Self

/*----------------------------------------------------------------------*/

METHOD IdeEditsManager:execEvent( nMode, p )
   LOCAL oEdit

   DO CASE
   CASE nMode == qcompleter_activated
      IF !empty( oEdit := ::getEditObjectCurrent() )
         oEdit:completeCode( p )
      ENDIF

   ENDCASE

   RETURN Nil

/*----------------------------------------------------------------------*/

METHOD IdeEditsManager:buildEditor( cSourceFile, nPos, nHPos, nVPos, cTheme, cView, aBookMarks )

   IdeEditor():new():create( ::oIde, cSourceFile, nPos, nHPos, nVPos, cTheme, cView, aBookMarks )

   RETURN Self

/*----------------------------------------------------------------------*/

METHOD IdeEditsManager:getTabBySource( cSource )

   cSource := hbide_pathNormalized( cSource, .t. )

   RETURN ascan( ::aTabs, {|e_| e_[ TAB_OEDITOR ]:pathNormalized == cSource } )

/*----------------------------------------------------------------------*/

METHOD IdeEditsManager:getTabCurrent()
   LOCAL qTab, nTab

   IF !empty( ::qTabWidget )
      qTab := ::qTabWidget:currentWidget()
      nTab := ascan( ::aTabs, {|e_| hbqt_IsEqualGcQtPointer( e_[ TAB_OTAB ]:oWidget:pPtr, qTab ) } )
   ENDIF
   RETURN nTab

/*----------------------------------------------------------------------*/

METHOD IdeEditsManager:getDocumentCurrent()
   LOCAL qTab, nTab

   IF !empty( ::qTabWidget ) .AND. ::qTabWidget:count() > 0
      qTab := ::qTabWidget:currentWidget()
      IF ( nTab := ascan( ::aTabs, {|e_| hbqt_IsEqualGcQtPointer( e_[ TAB_OTAB ]:oWidget:pPtr, qTab ) } ) ) > 0
         RETURN QTextDocument():configure( ::aTabs[ nTab, TAB_OEDITOR ]:document() )
      ENDIF
   ENDIF

   RETURN Nil

/*----------------------------------------------------------------------*/

METHOD IdeEditsManager:getEditObjectCurrent()
   LOCAL qTab, nTab

   IF !empty( ::qTabWidget ) .AND. ::qTabWidget:count() > 0
      qTab := ::qTabWidget:currentWidget()
      IF ( nTab := ascan( ::aTabs, {|e_| hbqt_IsEqualGcQtPointer( e_[ TAB_OTAB ]:oWidget:pPtr, qTab ) } ) ) > 0
         RETURN ::aTabs[ nTab, TAB_OEDITOR ]:qCoEdit
      ENDIF
   ENDIF

   RETURN Nil

/*----------------------------------------------------------------------*/

METHOD IdeEditsManager:getEditCurrent()
   LOCAL qTab, nTab

   IF !empty( ::qTabWidget ) .AND. ::qTabWidget:count() > 0
      qTab := ::qTabWidget:currentWidget()
      IF ( nTab := ascan( ::aTabs, {|e_| hbqt_IsEqualGcQtPointer( e_[ TAB_OTAB ]:oWidget:pPtr, qTab ) } ) ) > 0
         RETURN ::aTabs[ nTab, TAB_OEDITOR ]:qCqEdit
      ENDIF
   ENDIF

   RETURN Nil

/*----------------------------------------------------------------------*/

METHOD IdeEditsManager:getEditorCurrent()
   LOCAL qTab, nTab

   IF !empty( ::qTabWidget ) .AND. ::qTabWidget:count() > 0
      qTab := ::qTabWidget:currentWidget()
      IF ( nTab := ascan( ::aTabs, {|e_| hbqt_IsEqualGcQtPointer( e_[ TAB_OTAB ]:oWidget:pPtr, qTab ) } ) ) > 0
         RETURN ::aTabs[ nTab, TAB_OEDITOR ]
      ENDIF
   ENDIF

   RETURN Nil

/*----------------------------------------------------------------------*/

METHOD IdeEditsManager:getEditorByIndex( nIndex ) /* Index is 0 based */
   LOCAL pTab, a_

   IF hb_isNumeric( nIndex ) .AND. nIndex >= 0 .AND. nIndex < ::qTabWidget:count()
      pTab := ::qTabWidget:widget( nIndex )
      FOR EACH a_ IN ::aTabs
         IF !empty( a_[ TAB_OTAB ] ) .AND. hbqt_IsEqualGcQtPointer( a_[ TAB_OTAB ]:oWidget:pPtr, pTab )
            RETURN ::aTabs[ a_:__enumIndex(), TAB_OEDITOR ]
         ENDIF
      NEXT
   ENDIF

   RETURN Nil

/*----------------------------------------------------------------------*/

METHOD IdeEditsManager:getEditorByTabObject( oTab )
   LOCAL nPos

   IF hb_isObject( oTab )
      IF ( nPos := ascan( ::aTabs, {|e_| e_[ TAB_OTAB ] == oTab } ) ) > 0
         RETURN ::aTabs[ nPos, TAB_OEDITOR ]
      ENDIF
   ENDIF

   RETURN Nil

/*----------------------------------------------------------------------*/

METHOD IdeEditsManager:getEditorByTabPosition( nPos )

   IF hb_isNumeric( nPos ) .AND. nPos > 0 .AND. nPos <= len( ::aTabs )
      IF !empty( ::aTabs[ nPos, TAB_OEDITOR ] )
         RETURN ::aTabs[ nPos, TAB_OEDITOR ]
      ENDIF
   ENDIF
   RETURN Nil

/*----------------------------------------------------------------------*/

METHOD IdeEditsManager:getEditorBySource( cSource )
   LOCAL n

   cSource := hbide_pathNormalized( cSource, .t. )
   IF ( n := ascan( ::aTabs, {|e_| e_[ TAB_OEDITOR ]:pathNormalized == cSource } ) ) > 0
      RETURN ::aTabs[ n, TAB_OEDITOR ]
   ENDIF

   RETURN Nil

/*----------------------------------------------------------------------*/

METHOD IdeEditsManager:reLoad( cSource )
   LOCAL oEdit

   IF hb_fileExists( cSource ) .AND. hbide_isValidText( cSource )
      IF !empty( oEdit := ::getEditorBySource( cSource ) )
         oEdit:qEdit:clear()
         oEdit:qEdit:setPlainText( hb_memoread( hbide_pathToOSPath( cSource ) ) )
      ENDIF
   ENDIF

   RETURN Self

/*----------------------------------------------------------------------*/

METHOD IdeEditsManager:isOpen( cSource )
   RETURN !empty( ::getEditorBySource( cSource ) )

/*----------------------------------------------------------------------*/

METHOD IdeEditsManager:setSourceVisible( cSource )
   LOCAL oEdit, nIndex

   IF !empty( oEdit := ::getEditorBySource( cSource ) )
      ::oDK:setView( oEdit:cView )

      nIndex := ::qTabWidget:indexOf( oEdit:oTab:oWidget )
      IF ::qTabWidget:currentIndex() != nIndex
         ::qTabWidget:setCurrentIndex( nIndex )
      ELSE
         oEdit:setDocumentProperties()
      ENDIF
      RETURN .t.
   ENDIF

   RETURN .f.

/*----------------------------------------------------------------------*/

METHOD IdeEditsManager:setSourceVisibleByIndex( nIndex ) /* nIndex is 0 based */

   IF ::qTabWidget:count() == 0
      RETURN .f.
   ENDIF

   IF nIndex >= ::qTabWidget:count()
      nIndex := 0
   ENDIF

   ::qTabWidget:setCurrentIndex( nIndex )
   ::getEditorByIndex( nIndex ):setDocumentProperties()

   RETURN .f.

/*----------------------------------------------------------------------*/

METHOD IdeEditsManager:undo()
   LOCAL oEdit
   IF !empty( oEdit := ::getEditObjectCurrent() )
      oEdit:undo()
   ENDIF
   RETURN Self

/*----------------------------------------------------------------------*/

METHOD IdeEditsManager:redo()
   LOCAL oEdit
   IF !empty( oEdit := ::getEditObjectCurrent() )
      oEdit:redo()
   ENDIF
   RETURN Self

/*----------------------------------------------------------------------*/

METHOD IdeEditsManager:cut()
   LOCAL oEdit
   IF !empty( oEdit := ::getEditObjectCurrent() )
      oEdit:cut()
   ENDIF
   RETURN Self

/*----------------------------------------------------------------------*/

METHOD IdeEditsManager:copy()
   LOCAL oEdit
   IF !empty( oEdit := ::getEditObjectCurrent() )
      oEdit:copy()
   ENDIF
   RETURN Self

/*----------------------------------------------------------------------*/

METHOD IdeEditsManager:paste()
   LOCAL oEdit
   IF !empty( oEdit := ::getEditObjectCurrent() )
      oEdit:paste()
   ENDIF
   RETURN Self

/*----------------------------------------------------------------------*/

METHOD IdeEditsManager:selectAll()
   LOCAL oEdit
   IF !empty( oEdit := ::getEditObjectCurrent() )
      oEdit:selectAll()
   ENDIF
   RETURN Self

/*----------------------------------------------------------------------*/

METHOD IdeEditsManager:toggleLineSelectionMode()
   LOCAL oEdit
   IF !empty( oEdit := ::getEditObjectCurrent() )
      oEdit:toggleLineSelectionMode()
   ENDIF
   RETURN Self

/*----------------------------------------------------------------------*/

METHOD IdeEditsManager:toggleSelectionMode()
   LOCAL oEdit
   IF !empty( oEdit := ::getEditObjectCurrent() )
      oEdit:toggleSelectionMode()
   ENDIF
   RETURN Self

/*----------------------------------------------------------------------*/

METHOD IdeEditsManager:duplicateLine()
   LOCAL oEdit
   IF !empty( oEdit := ::getEditObjectCurrent() )
      oEdit:duplicateLine()
   ENDIF
   RETURN Self

/*----------------------------------------------------------------------*/

METHOD IdeEditsManager:moveLine( nDirection )
   LOCAL oEdit
   IF !empty( oEdit := ::getEditObjectCurrent() )
      oEdit:moveLine( nDirection )
   ENDIF
   RETURN Self

/*----------------------------------------------------------------------*/

METHOD IdeEditsManager:deleteLine()
   LOCAL oEdit
   IF !empty( oEdit := ::getEditObjectCurrent() )
      oEdit:deleteLine()
   ENDIF
   RETURN Self

/*----------------------------------------------------------------------*/

METHOD IdeEditsManager:streamComment()
   LOCAL oEdit
   IF !empty( oEdit := ::getEditObjectCurrent() )
      oEdit:streamComment()
   ENDIF
   RETURN Self

/*----------------------------------------------------------------------*/

METHOD IdeEditsManager:blockComment()
   LOCAL oEdit
   IF !empty( oEdit := ::getEditObjectCurrent() )
      oEdit:blockComment()
   ENDIF
   RETURN Self

/*----------------------------------------------------------------------*/

METHOD IdeEditsManager:indent( nStep )
   LOCAL oEdit
   IF !empty( oEdit := ::getEditObjectCurrent() )
      oEdit:blockIndent( nStep )
   ENDIF
   RETURN Self

/*----------------------------------------------------------------------*/

METHOD IdeEditsManager:convertQuotes()
   LOCAL oEdit
   IF !empty( oEdit := ::getEditObjectCurrent() )
      oEdit:convertQuotes()
   ENDIF
   RETURN Self

/*----------------------------------------------------------------------*/

METHOD IdeEditsManager:convertDQuotes()
   LOCAL oEdit
   IF !empty( oEdit := ::getEditObjectCurrent() )
      oEdit:convertDQuotes()
   ENDIF
   RETURN Self

/*----------------------------------------------------------------------*/

METHOD IdeEditsManager:switchToReadOnly()
   LOCAL oEdit
   IF !empty( oEdit := ::getEditObjectCurrent() )
      oEdit:setReadOnly()
   ENDIF
   RETURN Self

/*----------------------------------------------------------------------*/

METHOD IdeEditsManager:presentSkeletons()
   LOCAL oEdit
   IF !empty( oEdit := ::getEditObjectCurrent() )
      oEdit:presentSkeletons()
   ENDIF
   RETURN Self

/*----------------------------------------------------------------------*/

METHOD IdeEditsManager:clearSelection()
   LOCAL oEdit
   IF !empty( oEdit := ::getEditObjectCurrent() )
      oEdit:clearSelection()
   ENDIF
   RETURN Self

/*----------------------------------------------------------------------*/

METHOD IdeEditsManager:toggleLineNumbers()
   LOCAL oEdit
   IF !empty( oEdit := ::getEditObjectCurrent() )
      oEdit:toggleLineNumbers()
   ENDIF
   RETURN Self

/*----------------------------------------------------------------------*/

METHOD IdeEditsManager:gotoFunction()
   LOCAL oEdit
   IF !empty( oEdit := ::getEditObjectCurrent() )
      oEdit:gotoFunction()
   ENDIF
   RETURN Self

/*----------------------------------------------------------------------*/

METHOD IdeEditsManager:getText()
   LOCAL oEdit, cText := ""
   IF !empty( oEdit := ::getEditObjectCurrent() )
      cText := oEdit:getText()
   ENDIF
   RETURN cText

/*----------------------------------------------------------------------*/

METHOD IdeEditsManager:getWord( lSelect )
   LOCAL oEdit, cText := ""
   IF !empty( oEdit := ::getEditObjectCurrent() )
      cText := oEdit:getWord( lSelect )
   ENDIF
   RETURN cText

/*----------------------------------------------------------------------*/

METHOD IdeEditsManager:getLine( nLine, lSelect )
   LOCAL oEdit, cText := ""
   IF !empty( oEdit := ::getEditObjectCurrent() )
      cText := oEdit:getLine( nLine, lSelect )
   ENDIF
   RETURN cText

/*----------------------------------------------------------------------*/

METHOD IdeEditsManager:convertSelection( cKey )
   LOCAL oEdit
   IF !empty( oEdit := ::getEditObjectCurrent() )
      SWITCH cKey
      CASE "ToUpper"
         oEdit:caseUpper()
         EXIT
      CASE "ToLower"
         oEdit:caseLower()
         EXIT
      CASE "Invert"
         oEdit:caseInvert()
         EXIT
      ENDSWITCH
   ENDIF
   RETURN Self

/*----------------------------------------------------------------------*/

METHOD IdeEditsManager:insertSeparator( cSep )
   LOCAL oEdit
   IF !empty( oEdit := ::getEditObjectCurrent() )
      oEdit:insertSeparator( cSep )
   ENDIF
   RETURN Self

/*----------------------------------------------------------------------*/

METHOD IdeEditsManager:insertText( cKey )
   LOCAL cFile, cText, oEdit

   IF ! empty( oEdit := ::getEditObjectCurrent() )
      DO CASE

      CASE cKey == "InsertDateTime"
         cText := DTOC( Date() ) + ' - ' + Time()

      CASE cKey == "InsertRandomName"
         cText := hbide_getUniqueFuncName()

      CASE cKey == "InsertExternalFile"
         cFile := ::oSM:selectSource( "open" )
         IF Empty( cFile ) .OR. !hb_FileExists( cFile )
            RETURN Self
         ENDIF
         IF !( hbide_isValidText( cFile ) )
            MsgBox( "File type unknown or unsupported: " + cFile )
            RETURN Self
         ENDIF
         cText := hb_memoread( cFile )

      OTHERWISE
         RETURN Self

      ENDCASE

      oEdit:insertText( cText )
   ENDIF
   RETURN Self

/*----------------------------------------------------------------------*/
////
METHOD IdeEditsManager:formatBraces()
   LOCAL qEdit, qDoc, cText

   IF !empty( qEdit := ::getEditCurrent() )

      qDoc := QTextDocument():configure( qedit:document() )

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
   ENDIF
   RETURN Self

/*----------------------------------------------------------------------*/
//////
METHOD IdeEditsManager:RemoveTabs()
   LOCAL qEdit, qDoc, cText, cSpaces

   IF ! empty( qEdit := ::getEditCurrent() )

      qDoc := QTextDocument():configure( qedit:document() )

      IF !( qDoc:isEmpty() )
         cSpaces := space( ::nTabSpaces )

         qDoc:setUndoRedoEnabled( .f. )

         cText := qDoc:toPlainText()
         qDoc:clear()
         qDoc:setPlainText( strtran( cText, chr( 9 ), cSpaces ) )

         qDoc:setUndoRedoEnabled( .t. )
      ENDIF
   ENDIF

   RETURN Self

/*----------------------------------------------------------------------*/

METHOD IdeEditsManager:RemoveTrailingSpaces()
   LOCAL qEdit, qDoc, cText, a_, s

   IF ! empty( qEdit := ::getEditCurrent() )

      qDoc := QTextDocument():configure( qedit:document() )

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
   ENDIF
   RETURN Self

/*----------------------------------------------------------------------*/

METHOD IdeEditsManager:zoom( nKey )
   LOCAL oEdit

   IF !empty( oEdit := ::getEditObjectCurrent() )
      oEdit:zoom( nKey )
   ENDIF

   RETURN Self

/*----------------------------------------------------------------------*/
////
METHOD IdeEditsManager:printPreview()
   LOCAL qDlg
   IF ! empty( ::qCurEdit )
      qDlg := QPrintPreviewDialog():new( ::oDlg:oWidget )
      qDlg:setWindowTitle( "Harbour-QT Preview Dialog" )
      Qt_Slots_Connect( ::pSlots, qDlg, "paintRequested(QPrinter)", {|p| ::paintRequested( p ) } )
      qDlg:exec()
      Qt_Slots_disConnect( ::pSlots, qDlg, "paintRequested(QPrinter)" )
   ENDIF
   RETURN self

/*----------------------------------------------------------------------*/

METHOD IdeEditsManager:paintRequested( pPrinter )
   LOCAL qPrinter
   qPrinter := QPrinter():configure( pPrinter )
   ::qCurEdit:print( qPrinter )
   RETURN Self

/*----------------------------------------------------------------------*/

METHOD IdeEditsManager:getSelectedText()
   LOCAL oEdit
   IF !empty( oEdit := ::getEditObjectCurrent() )
      RETURN oEdit:getSelectedText()
   ENDIF
   RETURN ""

/*----------------------------------------------------------------------*/

METHOD IdeEditsManager:setMark()
   LOCAL oEdit
   IF !empty( oEdit := ::getEditObjectCurrent() )
      oEdit:setNewMark()
   ENDIF
   RETURN Self

/*----------------------------------------------------------------------*/

METHOD IdeEditsManager:gotoMark( nIndex )
   LOCAL oEdit
   IF !empty( oEdit := ::getEditObjectCurrent() )
      oEdit:gotoMark( nIndex )
   ENDIF
   RETURN Self

/*----------------------------------------------------------------------*/

METHOD IdeEditsManager:goto( nLine )
   LOCAL qGo, qCursor, nRows, oEdit

   IF ! empty( oEdit := ::oEM:getEditObjectCurrent() )
      qCursor := QTextCursor():configure( oEdit:qEdit:textCursor() )
      nRows   := oEdit:qEdit:blockCount()

      IF hb_isNumeric( nLine ) .AND. nLine >= 0 .AND. nLine <= nRows
         //
      ELSE
         nLine   := qCursor:blockNumber()

         qGo := QInputDialog():new( ::oDlg:oWidget )
         qGo:setInputMode( 1 )
         qGo:setIntMinimum( 1 )
         qGo:setIntMaximum( nRows )
         qGo:setIntValue( nLine + 1 )
         qGo:setLabelText( "Goto Line Number [1-" + hb_ntos( nRows ) + "]" )
         qGo:setWindowTitle( "Harbour" )

         ::setPosByIni( qGo, GotoDialogGeometry )
         qGo:exec()
         ::aIni[ INI_HBIDE, GotoDialogGeometry ] := hbide_posAndSize( qGo )
         nLine := qGo:intValue()
      ENDIF

      oEdit:goto( nLine )
   ENDIF

   RETURN nLine

/*----------------------------------------------------------------------*/
//
//                            CLASS IdeEditor
//                     Holds One Document in One Tab
//
/*----------------------------------------------------------------------*/

#define qTimeSave_timeout                         101

CLASS IdeEditor INHERIT IdeObject

   DATA   oTab
   DATA   cPath
   DATA   cFile                                   INIT   ""
   DATA   cExt                                    INIT   ""
   DATA   cType                                   INIT   ""
   DATA   cTheme                                  INIT   ""
   DATA   cView
   DATA   qDocument
   DATA   qDocLayout
   DATA   qHiliter
   DATA   qTimerSave
   DATA   sourceFile                              INIT   ""
   DATA   pathNormalized
   DATA   qLayout
   DATA   lLoaded                                 INIT   .F.

   DATA   aEdits                                  INIT   {}   /* Hold IdeEdit Objects */
   DATA   oEdit
   DATA   qEdit
   DATA   qCqEdit
   DATA   qCoEdit

   DATA   nBlock                                  INIT   -1
   DATA   nColumn                                 INIT   -1

   DATA   nPos                                    INIT   0
   DATA   nHPos                                   INIT   0
   DATA   nVPos                                   INIT   0
   DATA   nID

   DATA   qCursor
   DATA   aSplits                                 INIT   {}

   DATA   qHLayout
   DATA   qLabel
   DATA   nnRow                                   INIT -99

   DATA   qEvents

   METHOD new( oIde, cSourceFile, nPos, nHPos, nVPos, cTheme, cView )
   METHOD create( oIde, cSourceFile, nPos, nHPos, nVPos, cTheme, cView, aBookMarks )
   METHOD split( nOrient, oEditP )
   METHOD relay( oEdit )
   METHOD destroy()
   METHOD execEvent( nMode, p )
   METHOD setDocumentProperties()
   METHOD activateTab( mp1, mp2, oXbp )
   METHOD buildTabPage( cSource )
   METHOD dispEditInfo( qEdit )
   METHOD setTabImage( qEdit )
   METHOD applyTheme( cTheme )

   ENDCLASS

/*----------------------------------------------------------------------*/

METHOD IdeEditor:new( oIde, cSourceFile, nPos, nHPos, nVPos, cTheme, cView )

   DEFAULT oIde        TO ::oIde
   DEFAULT cSourceFile TO ::sourceFile
   DEFAULT nPos        TO ::nPos
   DEFAULT nHPos       TO ::nHPos
   DEFAULT nVPos       TO ::nVPos
   DEFAULT cTheme      TO ::cTheme
   DEFAULT cView       TO ::cView

   ::oIde       := oIde
   ::sourceFile := cSourceFile
   ::nPos       := nPos
   ::nHPos      := nHPos
   ::nVPos      := nVPos
   ::cTheme     := cTheme
   ::cView      := cView

   ::nID        := hbide_getNextUniqueID()

   RETURN Self

/*----------------------------------------------------------------------*/

METHOD IdeEditor:create( oIde, cSourceFile, nPos, nHPos, nVPos, cTheme, cView, aBookMarks )
   LOCAL cFileTemp

   DEFAULT oIde        TO ::oIde
   DEFAULT cSourceFile TO ::sourceFile
   DEFAULT nPos        TO ::nPos
   DEFAULT nHPos       TO ::nHPos
   DEFAULT nVPos       TO ::nVPos
   DEFAULT cTheme      TO ::cTheme
   DEFAULT cView       TO ::cView
   DEFAULT aBookMarks  TO {}

   ::oIde           := oIde
   ::SourceFile     := hbide_pathNormalized( cSourceFile, .F. )
   ::nPos           := nPos
   ::nHPos          := nHPos
   ::nVPos          := nVPos
   ::cTheme         := cTheme
   ::cView          := cView

   DEFAULT ::cView TO iif( ::cWrkView == "Stats", "Main", ::cWrkView )
   ::oDK:setView( ::cView )

   ::pathNormalized := hbide_pathNormalized( cSourceFile, .t. )

   hb_fNameSplit( cSourceFile, @::cPath, @::cFile, @::cExt )

   cFileTemp := hbide_pathToOSPath( ::cPath + ::cFile + ::cExt + ".tmp" )
   IF hb_fileExists( cFileTemp )
      IF hbide_getYesNo( "An auto saved version already exists, restore ?", cSourceFile, "Last run crash detected" )
         hb_memowrit( hbide_pathToOSPath( cSourceFile ), hb_memoread( cFileTemp ) )
      ELSE
         ferase( cFileTemp )
      ENDIF
   ENDIF

   ::cType := upper( strtran( ::cExt, ".", "" ) )
   ::cType := iif( ::cType $ "PRG,C,CPP,H,CH,PPO", ::cType, "U" )

   ::buildTabPage( ::sourceFile )

   ::qLayout := QGridLayout():new()
   ::qLayout:setContentsMargins( 0,0,0,0 )
   ::qLayout:setHorizontalSpacing( 5 )
   ::qLayout:setVerticalSpacing( 5 )
   //
   ::oTab:oWidget:setLayout( ::qLayout )

   ::oEdit   := IdeEdit():new( Self, 0 )
   ::oEdit:aBookMarks := aBookMarks
   ::oEdit:create()
   ::qEdit   := ::oEdit:qEdit
   ::qCqEdit := ::oEdit:qEdit
   ::qCoEdit := ::oEdit

   ::qDocument  := QTextDocument():configure( ::qEdit:document() )
   ::qDocLayout := QPlainTextDocumentLayout():new( ::qDocument )
   ::qDocument:setDocumentLayout( ::qDocLayout )

   IF ::cType != "U"
      ::qHiliter := ::oTH:SetSyntaxHilighting( ::oEdit:qEdit, @::cTheme )
   ENDIF
   ::qCursor := QTextCursor():configure( ::qEdit:textCursor() )

   ::qLayout:addLayout( ::oEdit:qHLayout, 0, 0 )

   /* Populate Tabs Array */
   aadd( ::aTabs, { ::oTab, Self } )

   /* Populate right at creation */
   ::oEM:addSourceInTree( ::sourceFile, ::cView )

   ::qTabWidget:setStyleSheet( GetStyleSheet( "QTabWidget", ::nAnimantionMode ) )
   ::setTabImage()

   RETURN Self

/*----------------------------------------------------------------------*/

METHOD IdeEditor:destroy()
   LOCAL n, oEdit

   IF !empty( ::qTimerSave )
      ::disconnect( ::qTimerSave, "timeout()" )
      ::qTimerSave:stop()
      ::qTimerSave := NIL
   ENDIF
   /* This code is reached under normal circumstances, so delete auto saved file */
   ferase( hbide_pathToOSPath( ::cPath + ::cFile + ::cExt + ".tmp" ) )

   ::qHiliter := NIL

   ::qCqEdit := NIL
   ::qCoEdit := NIL
   ::qEdit   := NIL
   DO WHILE len( ::aEdits ) > 0
      oEdit := ::aEdits[ 1 ]
      hb_adel( ::aEdits, 1, .t. )
      oEdit:destroy()
   ENDDO
   ::oEdit:destroy()

   IF !Empty( ::qDocument )
      ::qDocument := NIL
   ENDIF

   IF !Empty( ::qHiliter )
      ::qHiliter  := NIL
   ENDIF

   ::oEdit := NIL

   IF !Empty( ::qLayout )
      ::qLayout   := NIL
   ENDIF

   IF ( n := ascan( ::aTabs, {|e_| e_[ TAB_OEDITOR ] == Self } ) ) > 0
      hb_adel( ::oIde:aTabs, n, .T. )
   ENDIF

   ::oEM:removeSourceInTree( ::sourceFile )

   ::qTabWidget:removeTab( ::qTabWidget:indexOf( ::oTab:oWidget ) )
   ::oTab := NIL

   IF ::qTabWidget:count() == 0
      IF ::lDockRVisible
         ::oFuncDock:hide()
         ::oIde:lDockRVisible := .f.
      ENDIF
   ENDIF

   RETURN Self

/*----------------------------------------------------------------------*/

METHOD IdeEditor:setDocumentProperties()
   LOCAL qCursor

   qCursor := QTextCursor():configure( ::qEdit:textCursor() )

   IF !( ::lLoaded )       /* First Time */
      ::qEdit:setPlainText( hb_memoRead( ::sourceFile ) )
      qCursor:setPosition( ::nPos )
      ::qEdit:setTextCursor( qCursor )

      QScrollBar():configure( ::qEdit:horizontalScrollBar() ):setValue( ::nHPos )
      QScrollBar():configure( ::qEdit:verticalScrollBar() ):setValue( ::nVPos )

      QTextDocument():configure( ::qEdit:document() ):setModified( .f. )

      ::qTabWidget:setTabIcon( ::qTabWidget:indexOf( ::oTab:oWidget ), ::resPath + "tabunmodified.png" )
      ::lLoaded := .T.

      IF ::cType $ "PRG,C,CPP,H,CH"
         ::qTimerSave := QTimer():New()
         ::qTimerSave:setInterval( 60000 )  // 1 minutes
         ::connect( ::qTimerSave, "timeout()", {|| ::execEvent( qTimeSave_timeout ) } )
         ::qTimerSave:start()
      ENDIF
   ENDIF

   ::nBlock  := qCursor:blockNumber()
   ::nColumn := qCursor:columnNumber()

   ::oIde:aSources := { ::sourceFile }
   ::oIde:createTags()
   ::oIde:updateFuncList()
   ::oIde:updateTitleBar()

   ::dispEditInfo( ::qEdit )

   ::oIde:manageFocusInEditor()

   RETURN Self

/*----------------------------------------------------------------------*/

METHOD IdeEditor:execEvent( nMode, p )
   LOCAL cFileTemp, aPops := {}

   p := p

   SWITCH nMode
   CASE qTimeSave_timeout
      IF ::qDocument:isModified()
         cFileTemp := hbide_pathToOSPath( ::cPath + ::cFile + ::cExt + ".tmp" )
         hb_memowrit( cFileTemp, ::qEdit:toPlainText() )
      ENDIF
      EXIT

   CASE qTab_contextMenu
HB_TRACE( HB_TR_ALWAYS, "IdeEditor:execEvent( nMode, p )" )
      aadd( aPops, { "Close", {|| MsgBox( "closing" ) } } )
      hbide_ExecPopup( aPops, p, ::oTab:oWidget )

   ENDSWITCH

   RETURN Self

/*----------------------------------------------------------------------*/

METHOD IdeEditor:relay( oEdit )
   LOCAL nCols, oEdt, nR, nC

   ::qLayout:removeItem( ::oEdit:qHLayout )
   FOR EACH oEdt IN ::aEdits
      ::qLayout:removeItem( oEdt:qHLayout )
      //
      oEdt:qHLayout:removeWidget( oEdt:qEdit )
      oEdt:qHLayout := QHBoxLayout():new()
      oEdt:qHLayout:addWidget( oEdt:qEdit )
   NEXT

   IF hb_isObject( oEdit )
      aadd( ::aEdits, oEdit )
   ENDIF
   ::qLayout:addLayout( ::oEdit:qHLayout, 0, 0 )

   nR := 0 ; nC := 0
   FOR EACH oEdt IN ::aEdits
      IF oEdt:nOrient == 1     // Horiz
         nC++
         ::qLayout:addLayout_1( oEdt:qHLayout, 0, nC, 1, 1, Qt_Vertical )
      ENDIF
   NEXT
   nCols := ::qLayout:columnCount()
   FOR EACH oEdt IN ::aEdits
      IF oEdt:nOrient == 2     // Verti
         nR++
         ::qLayout:addLayout_1( oEdt:qHLayout, nR, 0, 1, nCols, Qt_Horizontal )
      ENDIF
   NEXT

   RETURN Self

/*----------------------------------------------------------------------*/

METHOD IdeEditor:split( nOrient, oEditP )
   LOCAL oEdit

   HB_SYMBOL_UNUSED( oEditP  )

   oEdit := IdeEdit():new( Self, 1 ):create()
   oEdit:qEdit:setDocument( ::qDocument )
   oEdit:nOrient := nOrient

   ::relay( oEdit )

   RETURN Self

/*----------------------------------------------------------------------*/

METHOD IdeEditor:activateTab( mp1, mp2, oXbp )
   LOCAL oEdit

   HB_SYMBOL_UNUSED( mp1 )
   HB_SYMBOL_UNUSED( mp2 )

   IF !empty( oEdit := ::oEM:getEditorByTabObject( oXbp ) )
      oEdit:setDocumentProperties()
      oEdit:qCoEdit:relayMarkButtons()
      oEdit:qCoEdit:toggleLineNumbers()
      oEdit:qCoEdit:dispStatusInfo()
   ENDIF

   RETURN Self

/*----------------------------------------------------------------------*/

METHOD IdeEditor:buildTabPage( cSource )

   ::oTab := XbpTabPage():new( ::oTabParent, , { 5,5 }, { 700,400 }, , .t. )

   IF Empty( cSource )
      ::oTab:caption := "Untitled " + hb_ntos( hbide_getNextUntitled() )
   ELSE
      ::oTab:caption := ::cFile + ::cExt
   ENDIF
   ::oTab:minimized := .F.

   ::oTab:create()

   ::qTabWidget:setCurrentIndex( ::qTabWidget:indexOf( ::oTab:oWidget ) )
   ::qTabWidget:setTabTooltip( ::qTabWidget:indexOf( ::oTab:oWidget ), cSource )
   //::connect( ::oTab:oWidget, "customContextMenuRequested(QPoint)", {|p| ::execEvent( qTab_contextMenu, p ) } )
   //::oTab:hbContextMenu := {|| ::execEvent( qTab_contextMenu, p ) }
   ::oTab:tabActivate := {|mp1,mp2,oXbp| ::activateTab( mp1, mp2, oXbp ) }

   RETURN Self

/*----------------------------------------------------------------------*/

METHOD IdeEditor:dispEditInfo( qEdit )
   LOCAL s, qDocument, qCursor

   DEFAULT qEdit TO ::qEdit

   qDocument := QTextDocument():configure( qEdit:document() )
   qCursor   := QTextCursor():configure( qEdit:textCursor() )

   s := "<b>Line "+ hb_ntos( qCursor:blockNumber() + 1 ) + " of " + ;
                    hb_ntos( qDocument:blockCount() ) + "</b>"

   ::oIde:oSBar:getItem( SB_PNL_MAIN     ):caption := "Success"
   ::oIde:oSBar:getItem( SB_PNL_READY    ):caption := "Ready"
   ::oIde:oSBar:getItem( SB_PNL_LINE     ):caption := s
   ::oIde:oSBar:getItem( SB_PNL_COLUMN   ):caption := "Col " + hb_ntos( qCursor:columnNumber() + 1 )
   ::oIde:oSBar:getItem( SB_PNL_INS      ):caption := iif( qEdit:overwriteMode() , " ", "Ins" )
   ::oIde:oSBar:getItem( SB_PNL_MODIFIED ):caption := iif( qDocument:isModified(), "Modified", iif( qEdit:isReadOnly(), "ReadOnly", " " ) )

   ::oIde:oSBar:getItem( SB_PNL_STREAM   ):caption := "Stream"
   ::oIde:oSBar:getItem( SB_PNL_EDIT     ):caption := "Edit"

   RETURN Self

/*----------------------------------------------------------------------*/

METHOD IdeEditor:setTabImage( qEdit )
   LOCAL nIndex, lModified, lReadOnly, cIcon

   DEFAULT qEdit TO ::oEdit:qEdit

   nIndex    := ::qTabWidget:indexOf( ::oTab:oWidget )
   lModified := ::qDocument:isModified()
   lReadOnly := qEdit:isReadOnly()

   IF lModified
      cIcon := "tabmodified.png"
   ELSEIF lReadOnly
      cIcon := "tabreadonly.png"
   ELSE
      cIcon := "tabunmodified.png"
   ENDIF

   ::qTabWidget:setTabIcon( nIndex, ::resPath + cIcon )
   ::oDK:setStatusText( SB_PNL_MODIFIED, iif( lModified, "Modified", iif( lReadOnly, "ReadOnly", " " ) ) )

   RETURN Self

/*----------------------------------------------------------------------*/

METHOD IdeEditor:applyTheme( cTheme )

   IF ::cType != "U"
      IF empty( cTheme )
         cTheme := ::oTH:selectTheme()
      ENDIF

      IF ::oTH:contains( cTheme )
         ::cTheme := cTheme
         ::qHiliter := ::oTH:SetSyntaxHilighting( ::qEdit, @::cTheme )
      ENDIF
   ENDIF
   RETURN Self

/*----------------------------------------------------------------------*/
