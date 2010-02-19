/*
 * $Id$
 */

/*
 * Harbour Project source code:
 *
 * Copyright 2009 Pritpal Bedi <pritpal@vouchcac.com>
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
 *                               28Dec2009
 */
/*----------------------------------------------------------------------*/
/*----------------------------------------------------------------------*/
/*----------------------------------------------------------------------*/

#include "hbide.ch"
#include "common.ch"
#include "hbclass.ch"
#include "hbqt.ch"

/*----------------------------------------------------------------------*/

CLASS IdeSearchReplace INHERIT IdeObject

   DATA   qFindLineEdit
   DATA   qReplLineEdit

   DATA   cFind                                   INIT ""

   METHOD new( oIde )
   METHOD create( oIde )
   METHOD destroy()
   METHOD beginFind()
   METHOD setFindString( cText )

   ENDCLASS

/*----------------------------------------------------------------------*/

METHOD IdeSearchReplace:new( oIde )

   ::oIde := oIde

   RETURN Self

/*----------------------------------------------------------------------*/

METHOD IdeSearchReplace:create( oIde )

   DEFAULT oIde TO ::oIde

   ::oIde := oIde

   ::oUI := HbQtUI():new( ::oIde:resPath + "searchreplacepanel.uic", ::oIde:oDlg:oWidget ):build()
   //::oUI := HbQtUI():new( ::oIde:resPath + "searchreplace.uic", ::oIde:oDlg:oWidget ):build()
   //::oUI:setStyleSheet( "QWidget { border: 1px solid red; }" )
   ::oUI:setFocusPolicy( Qt_StrongFocus )

   ::oUI:q_buttonClose:setIcon( ::resPath + "closetab.png" )
   ::oUI:signal( "buttonClose", "clicked()", {|| ::oUI:hide() } )

   ::oUI:q_buttonNext:setIcon( ::resPath + "next.png"     )
   ::oUI:q_buttonNext:setToolTip( "Find Next" )

   ::oUI:q_buttonPrev:setIcon( ::resPath + "previous.png" )
   ::oUI:q_buttonPrev:setToolTip( "Find Previous" )

   ::oUI:q_buttonTop :setIcon( ::resPath + "up.png"       )
   ::oUI:q_buttonTop :setToolTip( "Start from Top" )

   ::qFindLineEdit := QLineEdit():from( ::oUI:q_comboFind:lineEdit() )
   ::qFindLineEdit:setFocusPolicy( Qt_StrongFocus )
   ::connect( ::qFindLineEdit, "textChanged(QString)", {|cText| ::setFindString( cText ) } )

   ::qReplLineEdit := QLineEdit():from( ::oUI:q_comboReplace:lineEdit() )
   ::qReplLineEdit:setFocusPolicy( Qt_StrongFocus )

   RETURN Self

/*----------------------------------------------------------------------*/

METHOD IdeSearchReplace:destroy()

   ::disconnect( ::qFindLineEdit, "textChanged(QString)" )

   ::oUI:destroy()

   RETURN Self

/*----------------------------------------------------------------------*/

METHOD IdeSearchReplace:beginFind()

   ::oUI:show()

   ::cFind := ""

   ::qFindLineEdit:selectAll()
   ::qFindLineEdit:setFocus( Qt_TabFocusReason )

   RETURN Self

/*----------------------------------------------------------------------*/

METHOD IdeSearchReplace:setFindString( cText )
   LOCAL nFlags, lFound, qCursor, nPos

   IF empty( cText )
      RETURN .f.
   ENDIF

   qCursor := QTextCursor():configure( ::qCurEdit:textCursor() )
   nPos := qCursor:position()
   qCursor:setPosition( 0 )
   ::qCurEdit:setTextCursor( qCursor )

   nFlags := 0
   nFlags += iif( ::oUI:q_checkMatchCase:isChecked(), QTextDocument_FindCaseSensitively, 0 )
   lFound := ::oEM:getEditCurrent():find( cText, nFlags )

   IF ! lFound
      qCursor:setPosition( nPos )
      ::qCurEdit:setTextCursor( qCursor )
      ::cFind := ""
      ::qFindLineEdit:setStyleSheet( "background-color: rgba( 240,120,120,255 );" )
   ELSE
      //::qCurEdit:setTextCursor( qCursor )
      ::cFind := cText
      ::qFindLineEdit:setStyleSheet( "" )
   ENDIF

   RETURN lFound

/*----------------------------------------------------------------------*/
/*----------------------------------------------------------------------*/
/*----------------------------------------------------------------------*/
//                           IdeFindReplace
/*----------------------------------------------------------------------*/
/*----------------------------------------------------------------------*/
/*----------------------------------------------------------------------*/

CLASS IdeFindReplace INHERIT IdeObject

   METHOD new( oIde )
   METHOD create( oIde )
   METHOD destroy()
   METHOD show()
   METHOD onClickReplace()
   METHOD replaceSelection( cReplWith )
   METHOD replace()
   METHOD onClickFind()
   METHOD find( lWarn )
   METHOD updateFindReplaceData( cMode )

   ENDCLASS

/*----------------------------------------------------------------------*/

METHOD IdeFindReplace:new( oIde )

   ::oIde := oIde

   RETURN Self

/*----------------------------------------------------------------------*/

METHOD IdeFindReplace:destroy()

   ::oUI:destroy()

   RETURN Self

/*----------------------------------------------------------------------*/

METHOD IdeFindReplace:create( oIde )

   DEFAULT oIde TO ::oIde

   ::oIde := oIde

   #ifdef HBIDE_USE_UIC
   ::oUI := HbQtUI():new( ::oIde:resPath + "finddialog.uic", ::oIde:oDlg:oWidget ):build()
   #else
   ::oUI := HbQtUI():new( ::oIde:resPath + "finddialog.ui", ::oIde:oDlg:oWidget ):create()
   #endif
   ::oUI:setWindowFlags( Qt_Sheet )

   aeval( ::oIde:aIni[ INI_FIND    ], {|e| ::oUI:q_comboFindWhat:addItem( e ) } )
   aeval( ::oIde:aIni[ INI_REPLACE ], {|e| ::oUI:q_comboReplaceWith:addItem( e ) } )

   ::oUI:q_radioFromCursor:setChecked( .t. )
   ::oUI:q_radioDown:setChecked( .t. )

   ::oUI:signal( "buttonFind"   , "clicked()", {|| ::onClickFind() } )
   ::oUI:signal( "buttonReplace", "clicked()", {|| ::onClickReplace() } )
   ::oUI:signal( "buttonClose"  , "clicked()", ;
         {|| ::oIde:aIni[ INI_HBIDE, FindDialogGeometry ] := hbide_posAndSize( ::oUI:oWidget ), ;
              ::oUI:hide(), ::oSearchReplace:oUI:hide() } )

   ::oUI:signal( "comboFindWhat", "editTextChanged(text)", {|| ::oUI:q_radioEntire:setChecked( .t. ) } )

   ::oUI:signal( "comboFindWhat", "currentIndexChanged(text)", ;
                               {|p| ::oIde:oSBar:getItem( SB_PNL_SEARCH ):caption := "FIND: " + p } )

   ::oUI:signal( "checkListOnly", "stateChanged(int)", {|p| ;
                                        ::oUI:q_comboReplaceWith:setEnabled( p == 0 ), ;
                                   iif( p == 1, ::oUI:q_buttonReplace:setEnabled( .f. ), NIL ) } )

   RETURN Self

/*----------------------------------------------------------------------*/

METHOD IdeFindReplace:show()
   LOCAL cText, qLineEdit

   ::oUI:q_buttonReplace:setEnabled( .f. )
   ::oUI:q_checkGlobal:setEnabled( .f. )
   ::oUI:q_checkNoPrompting:setEnabled( .f. )
   ::oUI:q_checkListOnly:setChecked( .f. )
   ::oIde:setPosByIni( ::oUI:oWidget, FindDialogGeometry )
   ::oUI:q_comboFindWhat:setFocus()

   qLineEdit := QLineEdit():configure( ::oUI:q_comboFindWhat:lineEdit() )
   IF !empty( cText := ::oEM:getSelectedText() )
      qLineEdit:setText( cText )
   ENDIF
   qLineEdit:selectAll()

   ::oUI:show()

   RETURN Self

/*----------------------------------------------------------------------*/

METHOD IdeFindReplace:onClickReplace()

   ::updateFindReplaceData( "replace" )

   IF ::oUI:q_comboReplaceWith:isEnabled()
      ::replace()
   ENDIF

   RETURN Self

/*----------------------------------------------------------------------*/

METHOD IdeFindReplace:replaceSelection( cReplWith )
   LOCAL nB, nL, cBuffer, qCursor

   DEFAULT cReplWith TO ""

   qCursor := QTextCursor():configure( ::qCurEdit:textCursor() )
   IF qCursor:hasSelection() .and. !empty( cBuffer := qCursor:selectedText() )
      nL := len( cBuffer )
      nB := qCursor:position() - nL

      qCursor:beginEditBlock()
      qCursor:removeSelectedText()
      qCursor:insertText( cReplWith )
      qCursor:setPosition( nB )
      qCursor:movePosition( QTextCursor_NextCharacter, QTextCursor_KeepAnchor, len( cReplWith ) )
      ::qCurEdit:setTextCursor( qCursor )
      qCursor:endEditBlock()
   ENDIF

   RETURN Self

/*----------------------------------------------------------------------*/

METHOD IdeFindReplace:replace()
   LOCAL cReplWith
   LOCAL nFound

   IF !empty( ::qCurEdit )
      cReplWith := QLineEdit():configure( ::oUI:q_comboReplaceWith:lineEdit() ):text()
      ::replaceSelection( cReplWith )

      IF ::oUI:q_checkGlobal:isChecked()
         IF ::oUI:q_checkNoPrompting:isChecked()
            nFound := 1
            DO WHILE ::find( .f. )
               nFound++
               ::replaceSelection( cReplWith )
            ENDDO
            ::oDK:setStatusText( SB_PNL_MAIN, "Replaced [" + hb_ntos( nFound ) + "] : " + cReplWith )
            ::oUI:q_buttonReplace:setEnabled( .f. )
            ::oUI:q_checkGlobal:setChecked( .f. )
            ::oUI:q_checkNoPrompting:setChecked( .f. )
         ELSE
            ::find()
         ENDIF
      ENDIF
   ENDIF

   RETURN Self

/*----------------------------------------------------------------------*/

METHOD IdeFindReplace:onClickFind()
   LOCAL lFound, nPos, qCursor

   ::updateFindReplaceData( "find" )

   IF ::oUI:q_radioEntire:isChecked()
      ::oUI:q_radioFromCursor:setChecked( .t. )
      qCursor := QTextCursor():configure( ::qCurEdit:textCursor() )
      nPos := qCursor:position()

      qCursor:setPosition( 0 )
      ::qCurEdit:setTextCursor( qCursor )
      IF !( lFound := ::find() )
         qCursor:setPosition( nPos )
         ::qCurEdit:setTextCursor( qCursor )
      ENDIF
   ELSE
      lFound := ::find()
   ENDIF

   IF lFound
      ::oUI:q_buttonReplace:setEnabled( .t. )
      ::oUI:q_checkGlobal:setEnabled( .t. )
      ::oUI:q_checkNoPrompting:setEnabled( .t. )
   ELSE
      ::oUI:q_buttonReplace:setEnabled( .f. )
      ::oUI:q_checkGlobal:setEnabled( .f. )
      ::oUI:q_checkNoPrompting:setEnabled( .f. )

      ::oUI:q_buttonFind:setFocus_1()
   ENDIF

   RETURN Self

/*----------------------------------------------------------------------*/

METHOD IdeFindReplace:find( lWarn )
   LOCAL nFlags
   LOCAL cText := QLineEdit():configure( ::oUI:q_comboFindWhat:lineEdit() ):text()
   LOCAL lFound := .f.

   DEFAULT lWarn TO .t.

   IF !empty( cText )
      nFlags := 0
      nFlags += iif( ::oUI:q_checkMatchCase:isChecked(), QTextDocument_FindCaseSensitively, 0 )
      nFlags += iif( ::oUI:q_radioUp:isChecked(), QTextDocument_FindBackward, 0 )

      //IF !( lFound := ::qCurEdit:find( cText, nFlags ) ) .and. lWarn
      IF !( lFound := ::oEM:getEditCurrent():find( cText, nFlags ) ) .and. lWarn
         hbide_showWarning( "Cannot find : " + cText )
      ENDIF
   ENDIF

   RETURN lFound

/*----------------------------------------------------------------------*/

METHOD IdeFindReplace:updateFindReplaceData( cMode )
   LOCAL cData

   IF cMode == "find"
      cData := QLineEdit():configure( ::oUI:q_comboFindWhat:lineEdit() ):text()
      IF !empty( cData )
         IF ascan( ::oIde:aIni[ INI_FIND ], {|e| e == cData } ) == 0
            hb_ains( ::oIde:aIni[ INI_FIND ], 1, cData, .t. )
            ::oUI:q_comboFindWhat:insertItem( 0, cData )
         ENDIF
      ENDIF
      //
      ::oDK:setStatusText( SB_PNL_SEARCH, cData )
   ELSE
      cData := QLineEdit():configure( ::oUI:q_comboReplaceWith:lineEdit() ):text()
      IF !empty( cData )
         IF ascan( ::oIde:aIni[ INI_REPLACE ], cData ) == 0
            hb_ains( ::oIde:aIni[ INI_REPLACE ], 1, cData, .t. )
            ::oUI:q_comboReplaceWith:insertItem( 0, cData )
         ENDIF
      ENDIF
   ENDIF

   RETURN Self

/*----------------------------------------------------------------------*/
//
//                         Class IdeFindInFiles
//
/*----------------------------------------------------------------------*/

#define L2S( l )                                  iif( l, "Yes", "No" )

#define F_BLACK                                   '<font color = black>'
#define F_GREEN                                   '<font color = green>'
#define F_RED                                     '<font color = red>'
#define F_CYAN                                    '<font color = cyan>'
#define F_BLUE                                    '<font color = blue>'
#define F_YELLOW                                  '<font color = yellow>'

#define F_SECTION                                 '<font color=GoldenRod size="6">'
#define F_SECTION_ITEM                            '<font color=blue size="5">'
#define F_INFO                                    '<font color=LightBlue>'
#define F_FILE                                    '<font color=green>'
#define F_SEARCH                                  '<font color=IndianRed>'

#define F_END                                     '</font>'

#define LOG_MISSING                               1
#define LOG_FINDS                                 2
#define LOG_SEPARATOR                             3
#define LOG_FLAGS                                 4
#define LOG_TERMINATED                            5
#define LOG_SECTION                               6
#define LOG_SECTION_ITEM                          7
#define LOG_EMPTY                                 8
#define LOG_INFO                                  9

/*----------------------------------------------------------------------*/

CLASS IdeFindInFiles INHERIT IdeObject

   DATA   aItems                                  INIT {}
   DATA   lStop                                   INIT .f.
   DATA   aInfo                                   INIT {}

   DATA   nSearched                               INIT 0
   DATA   nFounds                                 INIT 0
   DATA   nMisses                                 INIT 0

   DATA   cOrigExpr
   DATA   compRegEx
   DATA   cReplWith
   DATA   lRegEx                                  INIT .F.
   DATA   lListOnly                               INIT .T.
   DATA   lMatchCase                              INIT .F.
   DATA   lNotDblClick                            INIT .F.
   DATA   lShowOnCreate                           INIT .T.
   DATA   lInDockWindow                           INIT .F.

   METHOD new( oIde, lShowOnCreate )
   METHOD create( oIde, lShowOnCreate )
   METHOD destroy()
   METHOD show()
   METHOD print()
   METHOD paintRequested( pPrinter )
   METHOD find()
   METHOD findInABunch( aFiles )
   METHOD showLog( nType, cMsg, aLines )

   METHOD execEvent( cEvent, p )
   METHOD execContextMenu( p )
   METHOD buildUI()

   ENDCLASS

/*----------------------------------------------------------------------*/

METHOD IdeFindInFiles:new( oIde, lShowOnCreate )

   ::oIde          := oIde
   ::lShowOnCreate := lShowOnCreate

   RETURN Self

/*----------------------------------------------------------------------*/

METHOD IdeFindInFiles:create( oIde, lShowOnCreate )

   DEFAULT oIde          TO ::oIde
   DEFAULT lShowOnCreate TO ::lShowOnCreate

   ::oIde          := oIde
   ::lShowOnCreate := lShowOnCreate

   RETURN Self

/*----------------------------------------------------------------------*/

METHOD IdeFindInFiles:buildUI()
   LOCAL cText, qLineEdit, aProjList, cProj, qItem, n

   ::oUI := HbQtUI():new( ::oIde:resPath + "findinfilesex.uic", ::oFindDock:oWidget ):build()
   ::oUI:hide()
hbide_dbg( "-------------------------", 0 )
   ::oFindDock:qtObject := ::oUI
   ::oFindDock:oWidget:setWidget( ::oUI )

   ::oUI:q_buttonFolder:setIcon( ::resPath + "folder.png" )

   aeval( ::oIde:aIni[ INI_FIND    ], {|e| ::oUI:q_comboExpr:addItem( e ) } )
   aeval( ::oIde:aIni[ INI_REPLACE ], {|e| ::oUI:q_comboRepl:addItem( e ) } )
   aeval( ::oIde:aIni[ INI_FOLDERS ], {|e| ::oUI:q_comboFolder:addItem( e ) } )

   n := ascan( ::oIde:aIni[ INI_FIND    ], {|e| e == ::cWrkFind } )
   ::oUI:q_comboExpr:setCurrentIndex( n-1 )

   n := ascan( ::oIde:aIni[ INI_REPLACE ], {|e| e == ::cWrkReplace } )
   ::oUI:q_comboRepl:setCurrentIndex( n - 1 )

   n := ascan( ::oIde:aIni[ INI_FOLDERS ], {|e| e == ::cWrkFolderFind } )
   ::oUI:q_comboFolder:setCurrentIndex( n - 1 )
   ::oUI:q_comboFolder:setEnabled( .f. )
   ::oUI:q_checkFolders:setChecked( .f. )
   ::oUI:q_checkSubFolders:setChecked( .f. )
   ::oUI:q_checkSubFolders:setEnabled( .f. )

   ::oUI:q_buttonRepl:setEnabled( .f. )
   ::oUI:q_buttonStop:setEnabled( .f. )
   ::oUI:q_comboRepl:setEnabled( .f. )

   ::oUI:q_checkListOnly:setChecked( .t. )
   ::oUI:q_checkPrg:setChecked( .t. )

   ::oIde:setPosAndSizeByIni( ::oUI:oWidget, FindInFilesDialogGeometry )

   qLineEdit := QLineEdit():configure( ::oUI:q_comboExpr:lineEdit() )
   IF !empty( ::oEM )
      IF !empty( cText := ::oEM:getSelectedText() )
         qLineEdit:setText( cText )
      ENDIF
   ENDIF
   qLineEdit:selectAll()

   /* Populate Projects Name */
   IF !empty( ::oPM )
      aProjList := ::oPM:getProjectsTitleList()
      FOR EACH cProj IN aProjList
         IF !empty( cProj )
            qItem := QListWidgetItem():new()
            qItem:setFlags( Qt_ItemIsUserCheckable + Qt_ItemIsEnabled + Qt_ItemIsSelectable )
            qItem:setText( cProj )
            qItem:setCheckState( 0 )
            ::oUI:q_listProjects:addItem_1( qItem )
            aadd( ::aItems, qItem )
         ENDIF
      NEXT
   ENDIF

   ::oUI:q_editResults:setReadOnly( .t. )
   //::oUI:q_editResults:setFontFamily( "Courier New" )
   //::oUI:q_editResults:setFontPointSize( 10 )
   //::oUI:q_editResults:setFont( ::oIde:oFont:oWidget )
   ::oUI:q_editResults:setContextMenuPolicy( Qt_CustomContextMenu )

   ::oUI:q_labelStatus:setText( "Ready" )
   ::oUI:q_comboExpr:setFocus()

   /* Attach all signals */
   ::connect( ::oUI:oWidget, "rejected()", {|| ::execEvent( "buttonClose" ) } )
   //
   ::oUI:signal( "buttonClose"  , "clicked()"                , {| | ::execEvent( "buttonClose"      ) } )
   ::oUI:signal( "buttonFolder" , "clicked()"                , {| | ::execEvent( "buttonFolder"     ) } )
   ::oUI:signal( "buttonFind"   , "clicked()"                , {| | ::execEvent( "buttonFind"       ) } )
   ::oUI:signal( "buttonRepl"   , "clicked()"                , {| | ::execEvent( "buttonRepl"       ) } )
   ::oUI:signal( "buttonStop"   , "clicked()"                , {| | ::execEvent( "buttonStop"       ) } )
   ::oUI:signal( "checkAll"     , "stateChanged(int)"        , {|p| ::execEvent( "checkAll", p      ) } )
   ::oUI:signal( "comboFind"    , "currentIndexChanged(text)", {|p| ::execEvent( "comboFind", p     ) } )
   ::oUI:signal( "checkListOnly", "stateChanged(int)"        , {|p| ::execEvent( "checkListOnly", p ) } )
   ::oUI:signal( "checkFolders" , "stateChanged(int)"        , {|p| ::execEvent( "checkFolders", p  ) } )
   ::oUI:signal( "editResults"  , "copyAvailable(bool)"      , {|l| ::execEvent( "editResults", l   ) } )
   ::oUI:signal( "editResults"  , "customContextMenuRequested(QPoint)", {|p| ::execEvent( "editResults-contextMenu", p ) } )

hbide_dbg( "-------------------------", 1 )
   RETURN Self

/*----------------------------------------------------------------------*/

METHOD IdeFindInFiles:execEvent( cEvent, p )
   LOCAL cPath, qLineEdit, qCursor, cSource, v, nInfo

   SWITCH cEvent

   CASE "buttonClose"
      ::oFindDock:hide()
      #if 0
      IF ::lInDockWindow
         ::oFindDock:hide()
      ELSE
         ::oIde:aIni[ INI_HBIDE, FindInFilesDialogGeometry ] := hbide_posAndSize( ::oUI:oWidget )
         ::destroy()
      ENDIF
      #endif
      EXIT

   CASE "comboFind"
      ::oIde:oSBar:getItem( SB_PNL_SEARCH ):caption := "FIND: " + p
      EXIT

   CASE "checkListOnly"
      ::oUI:q_comboRepl:setEnabled( p == 0 )
      ::oUI:q_buttonRepl:setEnabled( !( p == 1 ) )
      EXIT

   CASE "checkFolders"
      ::oUI:q_comboFolder:setEnabled( p == 2 )
      ::oUI:q_checkSubFolders:setEnabled( p == 2 )
      EXIT

   CASE "buttonFind"
      ::find()
      EXIT

   CASE "buttonRepl"
      EXIT

   CASE "buttonStop"
      ::lStop := .t.
      EXIT

   CASE "buttonFolder"
      cPath := hbide_fetchADir( ::oDlg, "Select a folder for search operation", ::cLastFileOpenPath )
      IF !empty( cPath )
         ::oIde:cLastFileOpenPath := cPath

         qLineEdit := QLineEdit():configure( ::oUI:q_comboFolder:lineEdit() )
         qLineEdit:setText( cPath )
         IF ascan( ::oIde:aIni[ INI_FOLDERS ], {|e| e == cPath } ) == 0
            hb_ains( ::oIde:aIni[ INI_FOLDERS ], 1, cPath, .t. )
         ENDIF
         ::oUI:q_comboFolder:insertItem( 0, cPath )
      ENDIF
      EXIT

   CASE "checkAll"
      v := !( p == 0 )
      ::oUI:q_checkPrg:setChecked( v )
      ::oUI:q_checkC:setChecked( v )
      ::oUI:q_checkCpp:setChecked( v )
      ::oUI:q_checkCh:setChecked( v )
      ::oUI:q_checkH:setChecked( v )
      ::oUI:q_checkRc:setChecked( v )
      EXIT

   CASE "editResults-contextMenu"
      ::execContextMenu( p )
      EXIT

   CASE "editResults"
      IF p .AND. ! ::lNotDblClick
         qCursor := QTextCursor():configure( ::oUI:q_editResults:textCursor() )
         nInfo := qCursor:blockNumber() + 1

         IF nInfo <= len( ::aInfo ) .AND. ::aInfo[ nInfo, 1 ] == -2
            cSource := ::aInfo[ nInfo, 2 ]

            ::oSM:editSource( cSource, 0, 0, 0, NIL, NIL, .f., .t. )
            qCursor := QTextCursor():configure( ::oIde:qCurEdit:textCursor() )
            qCursor:setPosition( 0 )
            qCursor:movePosition( QTextCursor_Down, QTextCursor_MoveAnchor, ::aInfo[ nInfo, 3 ] - 1 )
            qCursor:movePosition( QTextCursor_Right, QTextCursor_MoveAnchor, ::aInfo[ nInfo, 4 ] - 1 )
            qCursor:movePosition( QTextCursor_Right, QTextCursor_KeepAnchor, len( ::aInfo[ nInfo, 5 ] ) )
            ::oIde:qCurEdit:setTextCursor( qCursor )
            ::oIde:manageFocusInEditor()
         ENDIF
      ELSE
         ::lNotDblClick := .F.
      ENDIF
      EXIT
   ENDSWITCH

   RETURN Self

/*----------------------------------------------------------------------*/

METHOD IdeFindInFiles:execContextMenu( p )
   LOCAL nLine, qCursor, qMenu, pAct, qAct, cAct, cFind

   qCursor := QTextCursor():configure( ::oUI:q_editResults:textCursor() )
   nLine := qCursor:blockNumber() + 1

   IF nLine <= len( ::aInfo )
      qMenu := QMenu():new( ::oUI:q_editResults )
      qMenu:addAction( "Copy"       )
      qMenu:addAction( "Select All" )
      qMenu:addAction( "Clear"      )
      qMenu:addAction( "Print"      )
      qMenu:addAction( "Save as..." )
      qMenu:addSeparator()
      qMenu:addAction( "Find"       )
      qMenu:addSeparator()
      IF ::aInfo[ nLine, 1 ] == -2     /* Found Line */
         qMenu:addAction( "Replace Line" )
      ELSEIF ::aInfo[ nLine, 1 ] == -1 /* Source File */
         qMenu:addAction( "Open"        )
         qMenu:addAction( "Replace All" )
      ENDIF
      qMenu:addSeparator()
      qMenu:addAction( "Zoom In"  )
      qMenu:addAction( "Zoom Out" )

      pAct := qMenu:exec_1( ::oUI:q_editResults:mapToGlobal( p ) )
      IF !hbqt_isEmptyQtPointer( pAct )
         qAct := QAction():configure( pAct )
         cAct := qAct:text()

         SWITCH cAct
         CASE "Save as..."
            EXIT
         CASE "Find"
            IF !empty( cFind := hbide_fetchAString( ::oUI:q_editResults, , "Find what?", "Find" ) )
               ::lNotDblClick := .T.
               IF !( ::oUI:q_editResults:find( cFind, 0 ) )
                  MsgBox( "Not Found" )
               ENDIF
            ENDIF
            EXIT
         CASE "Print"
            ::print()
            EXIT
         CASE "Clear"
            ::oUI:q_editResults:clear()
            ::aInfo := {}
            EXIT
         CASE "Copy"
            ::lNotDblClick := .T.
            ::oUI:q_editResults:copy()
            EXIT
         CASE "Select All"
            ::oUI:q_editResults:selectAll()
            EXIT
         CASE "Replace Line"
            EXIT
         CASE "Replace Source"
            EXIT
         CASE "Zoom In"
            ::oUI:q_editResults:zoomIn()
            EXIT
         CASE "Zoom Out"
            ::oUI:q_editResults:zoomOut()
            EXIT
         ENDSWITCH
      ENDIF
   ENDIF

   RETURN Self

/*----------------------------------------------------------------------*/

METHOD IdeFindInFiles:destroy()
   LOCAL qItem

   IF !empty( ::oUI )
      ::disconnect( ::oUI:oWidget, "rejected()" )

      FOR EACH qItem IN ::aItems
         qItem := NIL
      NEXT
   ENDIF

   RETURN Self

/*----------------------------------------------------------------------*/

METHOD IdeFindInFiles:show()

   IF empty( ::oUI )
      ::buildUI()
   ENDIF
   ::oFindDock:show()

   RETURN Self

/*----------------------------------------------------------------------*/

METHOD IdeFindInFiles:find()
   LOCAL lPrg, lC, lCpp, lH, lCh, lRc, a_
   LOCAL lTabs, lSubF, lSubP, cFolder, qItem, aFilter, cExt, cMask, cWrkFolder
   LOCAL nStart, nEnd, cSource, aDir, cProjTitle, aProjFiles
   LOCAL aOpenSrc   := {}
   LOCAL aFolderSrc := {}
   LOCAL aProjSrc   := {}
   LOCAL aProjs     := {}
   LOCAL aPaths     := {}

   IF empty( ::cOrigExpr := ::oUI:q_comboExpr:currentText() )
      RETURN Self
   ENDIF

   ::lListOnly  := ::oUI:q_checkListOnly:isChecked()
   ::lMatchCase := ::oUI:q_checkMatchCase:isChecked()
   ::cReplWith  := ::oUI:q_comboRepl:currentText()

   ::lRegEx := ::oUI:q_checkRegEx:isChecked()
   IF ::lRegEx
      ::compRegEx := hb_regExComp( ::cOrigExpr, ::lMatchCase )
      IF ! hb_isRegEx( ::compRegEx )
         MsgBox( "Error in Regular Expression" )
         RETURN Self
      ENDIF
   ENDIF

   cFolder      := ::oUI:q_comboFolder:currentText()
   cWrkFolder   := cFolder
   lTabs        := ::oUI:q_checkOpenTabs:isChecked()
   lSubF        := ::oUI:q_checkSubFolders:isChecked()
   lSubP        := ::oUI:q_checkSubProjects:isChecked()
   /* Type of files */
   lPrg         := ::oUi:q_checkPrg:isChecked()
   lC           := ::oUI:q_checkC:isChecked()
   lCpp         := ::oUI:q_checkCpp:isChecked()
   lH           := ::oUI:q_checkH:isChecked()
   lCh          := ::oUI:q_checkCh:isChecked()
   lRc          := ::oUI:q_checkRc:isChecked()

   aFilter := hbide_buildFilter( lPrg, lC, lCpp, lH, lCh, lRc )

   /* Process Open Tabs */
   IF lTabs
      FOR EACH a_ IN ::aTabs
         cSource := a_[ 2 ]:sourceFile
         IF hbide_isSourceOfType( cSource, aFilter )
            aadd( aOpenSrc, cSource )
         ENDIF
      NEXT
   ENDIF

   /* Process Folder */
   IF ::oUI:q_checkFolders:isChecked() .AND. ! empty( cFolder )
      hbide_fetchSubPaths( @aPaths, cFolder, ::oUI:q_checkSubFolders:isChecked() )

      FOR EACH cFolder IN aPaths
         FOR EACH cExt IN aFilter
            cMask := hbide_pathToOsPath( cFolder + cExt )
            aDir  := directory( cMask )
            FOR EACH a_ IN aDir
               aadd( aFolderSrc, cFolder + a_[ 1 ] )
            NEXT
         NEXT
      NEXT
   ENDIF

   /* Process Projects */
   IF !empty( ::aItems )
      FOR EACH qItem IN ::aItems
         IF qItem:checkState() == 2
            aadd( aProjs, qItem:text() )
         ENDIF
      NEXT
   ENDIF
   IF !empty( aProjs )
      FOR EACH cProjTitle IN aProjs
         a_:= {}
         IF !empty( aProjFiles := ::oPM:getSourcesByProjectTitle( cProjTitle ) )
            FOR EACH cSource IN aProjFiles
               IF hbide_isSourceOfType( cSource, aFilter )
                  aadd( a_, cSource )
               ENDIF
            NEXT
         ENDIF
         IF !empty( a_ )
            aadd( aProjSrc, { cProjTitle, a_ } )
         ENDIF
      NEXT
   ENDIF

   /* Supress Find button - user must not click it again */
   ::oUI:q_buttonFind:setEnabled( .f. )
   ::oUI:q_buttonStop:setEnabled( .t. )

   ::nSearched := 0
   ::nFounds   := 0
   ::nMisses   := 0

   ::oUI:q_labelStatus:setText( "Ready" )

   /* Fun Begins */
   ::showLog( LOG_SEPARATOR )
   ::showLog( LOG_FLAGS, "[Begins: " + dtoc( date() ) + "-" + time() + "][" + "Find Expression: " + ::cOrigExpr + "]" )
   ::showLog( LOG_FLAGS, hbide_getFlags( lPrg, lC, lCpp, lH, lCh, lRc, lTabs, lSubF, lSubP, ::lRegEx, ::lListOnly, ::lMatchCase ) )
   ::showLog( LOG_EMPTY )

   nStart := seconds()

   IF lTabs
      ::showLog( LOG_SECTION, "OpenTabs" )
      IF !empty( aOpenSrc )
         ::findInABunch( aOpenSrc )
      ELSE
         ::showLog( LOG_INFO, "No matching files found" )
      ENDIF
   ENDIF

   IF ::oUI:q_checkFolders:isChecked() .AND. ! empty( cFolder )
      ::showLog( LOG_SECTION, "Folders" )
      IF !empty( aFolderSrc )
         ::showLog( LOG_SECTION_ITEM, "Folder: " + cFolder )
         ::findInABunch( aFolderSrc )
      ELSE
         ::showLog( LOG_INFO, "No matching files found" )
      ENDIF
   ENDIF

   IF !empty( aProjs )
      ::showLog( LOG_SECTION, "Projects" )
      IF !empty( aProjSrc )
         FOR EACH a_ IN aProjSrc
            ::showLog( LOG_SECTION_ITEM, "Project: " + a_[ 1 ] )
            ::findInABunch( a_[ 2 ] )
         NEXT
      ELSE
         ::showLog( LOG_INFO, "No matching files found" )
      ENDIF
   ENDIF

   nEnd := seconds()

   ::showLog( LOG_EMPTY )
   ::showLog( LOG_FLAGS, "[Ends:" + dtoc( date() ) + "-" + time() + "-" + hb_ntos( nEnd - nStart ) + " Secs]" + ;
                         "[Searched: " + hb_ntos( ::nSearched ) + "][Finds: " + hb_ntos( ::nFounds ) + "]" + ;
                         "[Files not found: " + hb_ntos( ::nMisses ) + "]" )
   ::showLog( LOG_SEPARATOR )
   ::showLog( LOG_EMPTY )

   ::oUI:q_labelStatus:setText( "[ Time Taken: " + hb_ntos( nEnd - nStart ) + " ] " + ;
                         "[ Searched: " + hb_ntos( ::nSearched ) + " ] [ Finds: " + hb_ntos( ::nFounds ) + " ] " + ;
                         "[ Files not found: " + hb_ntos( ::nMisses ) + " ]" )
   ::lStop := .f.
   ::oUI:q_buttonStop:setEnabled( .f. )
   ::oUI:q_buttonFind:setEnabled( .t. )

   IF ::nFounds > 0
      IF ascan( ::oIde:aIni[ INI_FIND ], {|e| e == ::cOrigExpr } ) == 0
         hb_ains( ::oIde:aIni[ INI_FIND ], 1, ::cOrigExpr, .t. )
         ::oUI:q_comboFolder:insertItem( 0, ::cOrigExpr )
      ENDIF
      ::oIde:cWrkFind := ::cOrigExpr
      ::oIde:cWrkFolderFind := cWrkFolder
   ENDIF

   RETURN Self

/*----------------------------------------------------------------------*/

STATIC FUNCTION hbide_fetchSubPaths( aPaths, cRootPath, lSubs )
   LOCAL aDir, a_

   DEFAULT lSubs TO .t.

   IF right( cRootPath, 1 ) != hb_osPathSeparator()
      cRootPath += hb_osPathSeparator()
   ENDIF
   cRootPath := hbide_pathToOSPath( cRootPath )

   aadd( aPaths, cRootPath )

   IF lSubs
      aDir := directory( cRootPath + "*.", "D" )
      FOR EACH a_ IN aDir
         IF a_[ 5 ] == "D" .AND. left( a_[ 1 ], 1 ) != "."
            hbide_fetchSubPaths( @aPaths, cRootPath + a_[ 1 ] )
         ENDIF
      NEXT
   ENDIF

   RETURN NIL

/*----------------------------------------------------------------------*/

METHOD IdeFindInFiles:findInABunch( aFiles )
   LOCAL s, cExpr, nLine, aLines, aBuffer, cLine, nNoMatch, aMatch, regEx

   nNoMatch := 0
   FOR EACH s IN aFiles
      IF ::lStop                                 /* Stop button is pressed */
         ::showLog( LOG_EMPTY )
         ::showLog( LOG_TERMINATED )
         EXIT
      ENDIF
      aLines := {}
      s := hbide_pathToOSPath( s )
      IF hb_fileExists( s )
         ::nSearched++
         aBuffer := hb_ATokens( StrTran( hb_MemoRead( s ), Chr( 13 ) ), Chr( 10 ) )
         nLine := 0

         IF ::lRegEx
            regEx := ::compRegEx
            FOR EACH cLine IN aBuffer
               nLine++
               //       exp, string, lMatchCase, lNewLine, nMaxMatch, nMatchWhich, lMatchOnly
               IF !empty( aMatch := hb_regExAll( regEx, cLine, ::lMatchCase, .F., 0, 1, .F.  ) )
                  aadd( aLines, { nLine, cLine, aMatch } )
               ENDIF
            NEXT
         ELSE
            IF ::lMatchCase
               cExpr := ::cOrigExpr
               FOR EACH cLine IN aBuffer
                  nLine++
                  IF cExpr $ cLine
                     aadd( aLines, { nLine, cLine, NIL } )
                  ENDIF
               NEXT
            ELSE
               cExpr := lower( ::cOrigExpr )
hbide_dbg( cExpr, "llllllllllllllllllll" )
               FOR EACH cLine IN aBuffer
                  nLine++
                  IF cExpr $ lower( cLine )
                     aadd( aLines, { nLine, cLine, NIL } )
                  ENDIF
               NEXT
            ENDIF
         ENDIF

         IF len( aLines ) > 0
            ::showLog( LOG_FINDS, s, aLines )
            ::nFounds++
         ELSE
            nNoMatch++
         ENDIF
      ELSE
         ::showLog( LOG_MISSING, s )
         ::nMisses++
      ENDIF
   NEXT
   IF nNoMatch == len( aFiles )
      ::showLog( LOG_INFO, "Searched (" + hb_ntos( len( aFiles ) ) + ") files, no matches found" )
   ENDIF

   RETURN Self

/*----------------------------------------------------------------------*/

METHOD IdeFindInFiles:showLog( nType, cMsg, aLines )
   LOCAL a_, n, cPre, cPost, nWidth, cText, nB, cL, nL, cT, cExp, aM
   LOCAL qCursor, qResult

   qResult := ::oUI:q_editResults

   DEFAULT cMsg TO ""

   qCursor := QTextCursor():configure( ::oUI:q_editResults:textCursor() )

   SWITCH nType

   CASE LOG_SEPARATOR
      qResult:append( F_BLACK + hbide_outputLine( "=", 68 ) + F_END )
      aadd( ::aInfo, { 0, NIL, NIL } )
      EXIT

   CASE LOG_FLAGS
      qResult:append( F_BLACK + cMsg + F_END )
      aadd( ::aInfo, { 0, NIL, NIL } )
      EXIT

   CASE LOG_INFO
      qResult:append( F_INFO + "<i>" + cMsg + "</i>" + F_END )
      aadd( ::aInfo, { 0, NIL, NIL } )
      EXIT

   CASE LOG_SECTION
      qResult:append( F_SECTION + "<u>" + cMsg + "</u>" + F_END )
      aadd( ::aInfo, { 0, NIL, NIL } )
      EXIT

   CASE LOG_SECTION_ITEM
      qResult:append( F_SECTION_ITEM + cMsg + F_END )
      aadd( ::aInfo, { 0, NIL, NIL } )
      EXIT

   CASE LOG_FINDS
      cText := F_FILE + "<b>" + cMsg + "   ( "+ hb_ntos( len( aLines ) ) + " )" + "</b>" + F_END
      ::oUI:q_editResults:append( cText )
      ::oUI:q_labelStatus:setText( cText )
      aadd( ::aInfo, { -1, cMsg, NIL } )

      n := 0
      aeval( aLines, {|a_| n := max( n, a_[ 1 ] ) } )
      nWidth := iif( n < 10, 1, iif( n < 100, 2, iif( n < 1000, 3, iif( n < 10000, 4, iif( n < 100000, 5, 7 ) ) ) ) )

      IF ::lRegEx
         FOR EACH a_ IN aLines
            nL := a_[ 1 ]
            aM := a_[ 3 ]
            nB := aM[ 1, 2 ]
            cL := hbide_buildResultLine( a_[ 2 ], aM )
            cT := aM[ 1, 1 ]

            qResult:append( F_BLACK + "&nbsp;&nbsp;&nbsp;(" + strzero( nL, nWidth ) + ")&nbsp;&nbsp;" + cL + F_END )

            aadd( ::aInfo, { -2, cMsg, nL, nB, cT  } )
            qCursor:movePosition( QTextCursor_Down )
         NEXT
      ELSE
         cExp := iif( ::lMatchCase, ::cOrigExpr, lower( ::cOrigExpr ) )
         FOR EACH a_ IN aLines
            nL    := a_[ 1 ]
            cL    := a_[ 2 ]
            //nB    := at( cExp, cL )
            nB    := at( cExp, iif( ::lMatchCase, cL, lower( cL ) ) )
            cPre  := substr( cL, 1, nB - 1 )
            cPost := substr( cL, nB + len( cExp ) )
            cT    := substr( cL, nB, len( cExp ) )
            cL    := cPre + F_SEARCH + "<b>" + cT + "</b>" + F_END + cPost

            qResult:append( F_BLACK + "&nbsp;&nbsp;&nbsp;(" + strzero( nL, nWidth ) + ")&nbsp;&nbsp;" + cL + F_END )

            //            mode, source, line#, pos, slctn
            aadd( ::aInfo, { -2, cMsg, nL, nB, cT  } )
            qCursor:movePosition( QTextCursor_Down )
         NEXT
      ENDIF
      EXIT

   CASE LOG_TERMINATED
      qResult:append( F_RED + "---------------- Terminated ---------------" + F_END )
      aadd( ::aInfo, { 0, NIL, NIL } )
      EXIT

   CASE LOG_MISSING
      qResult:append( F_RED + cMsg + F_END )
      aadd( ::aInfo, { 0, NIL, NIL } )
      EXIT

   CASE LOG_EMPTY
      qResult:append( F_BLACK + " " + F_END )
      aadd( ::aInfo, { 0, NIL, NIL } )
      EXIT

   ENDSWITCH

   qCursor:movePosition( QTextCursor_Down )
   ::oUI:q_editResults:setTextCursor( qCursor )

   QApplication():processEvents()
   RETURN Self

/*----------------------------------------------------------------------*/

STATIC FUNCTION hbide_buildResultLine( cLine, aM )
   LOCAL cT, cR, i

   FOR i := 1 TO len( aM )
      cR    := aM[ i, 1 ]
      cT    := replicate( chr( 255 ), len( aM[ i, 1 ] ) )
      cLine := strtran( cLine, cR, cT, 1, 1 )
   NEXT
   FOR i := 1 TO len( aM )
      cR    := replicate( chr( 255 ), len( aM[ i, 1 ] ) )
      cT    := F_SEARCH + "<b>" + aM[ i, 1 ] + "</b>" + F_END
      cLine := strtran( cLine, cR, cT, 1, 1 )
   NEXT

   RETURN cLine

/*----------------------------------------------------------------------*/

METHOD IdeFindInFiles:print()
   LOCAL qDlg

   qDlg := QPrintPreviewDialog():new( ::oUI )
   qDlg:setWindowTitle( "Harbour-QT Preview Dialog" )
   Qt_Slots_Connect( ::pSlots, qDlg, "paintRequested(QPrinter)", {|p| ::paintRequested( p ) } )
   qDlg:exec()
   Qt_Slots_disConnect( ::pSlots, qDlg, "paintRequested(QPrinter)" )

   RETURN self

/*----------------------------------------------------------------------*/

METHOD IdeFindInFiles:paintRequested( pPrinter )
   LOCAL qPrinter

   qPrinter := QPrinter():configure( pPrinter )

   ::oUI:q_editResults:print( qPrinter )

   RETURN Self

/*----------------------------------------------------------------------*/

STATIC FUNCTION hbide_buildFilter( lPrg, lC, lCpp, lH, lCh, lRc )
   LOCAL aFilter := {}

   IF lPrg
      aadd( aFilter, "*.prg" )
   ENDIF
   IF lC
      aadd( aFilter, "*.c" )
   ENDIF
   IF lCpp
      aadd( aFilter, "*.cpp" )
   ENDIF
   IF lh
      aadd( aFilter, "*.h" )
   ENDIF
   IF lCh
      aadd( aFilter, "*.ch" )
   ENDIF
   IF lRc
      aadd( aFilter, "*.rc" )
   ENDIF

   RETURN aFilter

/*----------------------------------------------------------------------*/

STATIC FUNCTION hbide_getFlags( lPrg, lC, lCpp, lH, lCh, lRc, lTabs, lSubF, lSubP, lRegEx, lListOnly, lMatchCase )
   LOCAL s := ""

   HB_SYMBOL_UNUSED( lTabs )
   HB_SYMBOL_UNUSED( lSubF )
   HB_SYMBOL_UNUSED( lSubP )
   HB_SYMBOL_UNUSED( lListOnly )

   s += "[.prg="  + L2S( lPrg        ) + "]"
   s += "[.c="    + L2S( lC          ) + "]"
   s += "[.cpp="  + L2S( lCpp        ) + "]"
   s += "[.h="    + L2S( lH          ) + "]"
   s += "[.ch="   + L2S( lCh         ) + "]"
   s += "[.rc="   + L2S( lRc         ) + "]"
   s += "[RegEx=" + L2S( lRegEx      ) + "]"
   s += "[Case="  + L2S( lMatchCase  ) + "]"

   RETURN s

/*----------------------------------------------------------------------*/

STATIC FUNCTION hbide_isSourceOfType( cSource, aFilter )
   LOCAL cExt

   hb_fNameSplit( cSource, , , @cExt )
   cExt := lower( cExt )

   RETURN  ascan( aFilter, {|e| cExt $ e } ) > 0

/*----------------------------------------------------------------------*/

