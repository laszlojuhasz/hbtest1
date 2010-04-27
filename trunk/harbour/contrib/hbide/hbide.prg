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
 *                               17Nov2009
 */
/*----------------------------------------------------------------------*/
/*----------------------------------------------------------------------*/
/*----------------------------------------------------------------------*/
/*
 *     Many thanks to Vailton Renato for adding new functionalities.
 */
/*----------------------------------------------------------------------*/

#include "hbide.ch"
#include "common.ch"
#include "xbp.ch"
#include "appevent.ch"
#include "inkey.ch"
#include "gra.ch"
#include "set.ch"

#include "hbclass.ch"
#include "hbver.ch"

#define UNU( x ) HB_SYMBOL_UNUSED( x )

/*----------------------------------------------------------------------*/

REQUEST HB_QT

STATIC s_resPath
STATIC s_pathSep

/*----------------------------------------------------------------------*/

PROCEDURE Main( ... )
   LOCAL oIde

   SET CENTURY ON
   SET EPOCH TO 1970

   s_resPath := hb_DirBase() + "resources" + hb_OsPathSeparator()
   s_pathSep := hb_OsPathSeparator()

   oIde := HbIde():new( hb_aParams() ):create()
   oIde:destroy()

   RETURN

/*----------------------------------------------------------------------*/

CLASS HbIde

   ACCESS pSlots                                  INLINE hbxbp_getSlotsPtr()
   ACCESS pEvents                                 INLINE hbxbp_getEventsPtr()

   DATA   aParams
   DATA   cProjIni

   DATA   oAC                                            /* Actions Manager                */
   DATA   oDK                                            /* Main Window Components Manager */
   DATA   oDW                                            /* Document Writer Manager        */
   DATA   oEM                                            /* Editor Tabs Manager            */
   DATA   oEV                                            /* Available Environments         */
   DATA   oFF                                            /* Find in Files Manager          */
   DATA   oFN                                            /* Functions Tags Manager         */
   DATA   oFR                                            /* Find Replace Manager           */
   DATA   oHL                                            /* Harbour Help Manager           */
   DATA   oHM                                            /* <Stats> panel manager          */
   DATA   oPM                                            /* Project Manager                */
   DATA   oSM                                            /* Souces Manager                 */
   DATA   oSK                                            /* Skeletons Managet              */
   DATA   oSC                                            /* Shortcuts Manager              */
   DATA   oTM                                            /* Plugin Tools Manager           */
   DATA   oTH                                            /* Themes Manager                 */

   DATA   nRunMode                                INIT   HBIDE_RUN_MODE_INI
   DATA   nAnimantionMode                         INIT   HBIDE_ANIMATION_NONE

   DATA   oUI

   DATA   aMeta                                   INIT   {}  /* Holds current definition only  */

   DATA   mp1, mp2, oXbp, nEvent

   DATA   aTabs                                   INIT   {}
   DATA   aINI                                    INIT   {}
   DATA   aViews                                  INIT   {}
   DATA   aProjData                               INIT   {}
   DATA   aPrpObjs                                INIT   {}
   DATA   aEditorPath                             INIT   {}
   DATA   aSrcOnCmdLine                           INIT   {}
   DATA   aHbpOnCmdLine                           INIT   {}

   /* HBQT Objects */
   DATA   qLayout

   DATA   qTabWidget
   DATA   oTabParent
   DATA   oFrame
   DATA   qLayoutFrame
   DATA   qViewsCombo

   DATA   qFindDlg

   DATA   qFontWrkProject
   DATA   qBrushWrkProject
   DATA   qProcess
   DATA   qHelpBrw
   DATA   qTBarLines
   DATA   qTBarPanels
   DATA   qTBarDocks
   DATA   qCompleter
   DATA   qCompModel
   DATA   qProtoList

   ACCESS oCurEditor                              INLINE ::oEM:getEditorCurrent()
   ACCESS qCurEdit                                INLINE ::oEM:getEditCurrent()
   ACCESS qCurDocument                            INLINE ::oEM:getDocumentCurrent()

   /* XBP Objects */
   DATA   oDlg
   DATA   oDa
   DATA   oSBar
   DATA   oMenu
   DATA   oTBar
   DATA   oStackedWidget
   DATA   oStackedWidgetMisc
   DATA   oFont
   DATA   oProjTree
   DATA   oEditTree
   DATA   oFuncList
   DATA   oOutputResult
   DATA   oCompileResult
   DATA   oLinkResult
   DATA   oNewDlg
   DATA   oPBFind, oPBRepl, oPBClose, oFind, oRepl
   DATA   oCurProjItem
   DATA   oCurProject
   DATA   oProjRoot
   DATA   oExes
   DATA   oLibs
   DATA   oDlls
   DATA   oProps
   DATA   oGeneral
   DATA   oSearchReplace
   DATA   oMainToolbar

   DATA   oDockR
   DATA   oDockB
   DATA   oDockB1
   DATA   oDockB2
   DATA   oDockPT
   DATA   oDockED
   DATA   oThemesDock
   DATA   oPropertiesDock
   DATA   oEnvironDock
   DATA   oFuncDock
   DATA   oDocViewDock
   DATA   oDocWriteDock
   DATA   oFunctionsDock
   DATA   oSkltnsTreeDock
   DATA   oHelpDock
   DATA   oSkeltnDock
   DATA   oFindDock

   DATA   lProjTreeVisible                        INIT   .t.
   DATA   lDockRVisible                           INIT   .f.
   DATA   lDockBVisible                           INIT   .f.
   DATA   lTabCloseRequested                      INIT   .f.
   DATA   isColumnSelectionEnabled                INIT   .f.
   DATA   lLineNumbersVisible                     INIT   .t.

   DATA   cWrkProject                             INIT   ""
   DATA   cWrkTheme                               INIT   ""
   DATA   cWrkCodec                               INIT   ""
   DATA   cWrkPathMk2                             INIT   hb_getenv( "HBIDE_DIR_HBMK2" )
   DATA   cWrkPathEnv                             INIT   hb_DirBase() + "resources"
   DATA   cWrkEnvironment                         INIT   ""
   DATA   cWrkFind                                INIT   ""
   DATA   cWrkFolderFind                          INIT   ""
   DATA   cWrkReplace                             INIT   ""
   DATA   cWrkView                                INIT   ""
   DATA   cWrkHarbour                             INIT   ""
   DATA   cPathShortcuts                          INIT   ""
   DATA   cTextExtensions                         INIT   ""

   DATA   oEnvironment

   DATA   cPathSkltns                             INIT   ""
   DATA   cSaveTo                                 INIT   ""
   DATA   oOpenedSources
   DATA   resPath                                 INIT   hb_DirBase() + "resources" + hb_OsPathSeparator()
   DATA   pathSep                                 INIT   hb_OsPathSeparator()
   DATA   cLastFileOpenPath                       INIT   hb_DirBase() + "projects"
   DATA   cProcessInfo
   DATA   cIniThemes
   DATA   cSeparator                              INIT   "/*" + replicate( "-", 70 ) + "*/"

   DATA   nTabSpaces                              INIT   3           /* Via User Setup */
   DATA   cTabSpaces                              INIT   space( 3 )  //::nTabSpaces )

   DATA   aTags                                   INIT   {}
   DATA   aText                                   INIT   {}
   DATA   aSkltns                                 INIT   {}
   DATA   aSources                                INIT   {}
   DATA   aFuncList                               INIT   {}
   DATA   aLines                                  INIT   {}
   DATA   aComments                               INIT   {}
   DATA   aProjects                               INIT   {}

   DATA   aMarkTBtns                              INIT   array( 6 )
   DATA   lClosing                                INIT   .f.

   METHOD new( aParams )
   METHOD create( aParams )
   METHOD destroy()
   METHOD setPosAndSizeByIni( qWidget, nPart )
   METHOD setPosByIni( qWidget, nPart )
   METHOD setSizeByIni( qWidget, nPart )
   METHOD manageFocusInEditor()
   METHOD removeProjectTree( aPrj )
   METHOD updateProjectTree( aPrj )
   METHOD manageItemSelected( oXbpTreeItem )
   METHOD manageProjectContext( mp1, mp2, oXbpTreeItem )
   METHOD updateFuncList()
   METHOD gotoFunction( mp1, mp2, oListBox )
   METHOD manageFuncContext( mp1 )
   METHOD createTags()
   METHOD updateProjectMenu()
   METHOD updateTitleBar()
   METHOD setCodec( cCodec )

   METHOD execAction( cKey )
   METHOD execProjectAction( cKey )
   METHOD execSourceAction( cKey )
   METHOD execEditorAction( cKey )

   METHOD showApplicationCursor( nCursor )
   METHOD testPainter( qPainter )

   METHOD parseParams()

   ENDCLASS

/*----------------------------------------------------------------------*/

METHOD HbIde:destroy()

   ::oSBar := NIL
   ::oMenu := NIL
   ::oTBar := NIL

   RETURN self

/*----------------------------------------------------------------------*/

METHOD HbIde:new( aParams )

   ::aParams := aParams

   RETURN self

/*----------------------------------------------------------------------*/

METHOD HbIde:create( aParams )
   LOCAL qPixmap, qSplash, n, cView

HB_TRACE( HB_TR_ALWAYS, "HbIde:create( cProjIni )", "#Params=" )

   qPixmap := QPixmap():new( hb_dirBase() + "resources" + hb_osPathSeparator() + "hbidesplash.png" )
   qSplash := QSplashScreen():new()
 * qSplash:setWindowFlags( hb_bitOr( Qt_WindowStaysOnTopHint, qSplash:windowFlags() ) )
   qSplash:setPixmap( qPixmap )
   qSplash:show()
   ::showApplicationCursor( Qt_BusyCursor )
   QApplication():new():processEvents()

   /* Initiate the place holders */
   ::aINI := array( INI_SECTIONS_COUNT )
   ::aINI[ 1 ] := afill( array( INI_HBIDE_VRBLS ), "" )
   //
   FOR n := 2 TO INI_SECTIONS_COUNT
      ::aIni[ n ] := array( 0 )
   NEXT

   DEFAULT aParams TO ::aParams
   ::aParams := aParams
   ::parseParams()

   /* Setup GUI Error Reporting System*/
   hbqt_errorsys()

   /* Editor's Font - TODO: User Managed Interface */
   ::oFont := XbpFont():new()
   ::oFont:fixed := .t.
   ::oFont:create( "10.Courier" )

   /* Functions Tag Manager */
   ::oFN := IdeFunctions():new():create( Self )

   /* Skeletons Manager     */
   ::oSK := IdeSkeletons():new( Self ):create()

   /* Initialte Project Manager */
   ::oPM := IdeProjManager():new( Self ):create()

   /* Load IDE Settings */
   IF ::nRunMode == HBIDE_RUN_MODE_INI
      hbide_loadINI( Self, ::cProjIni )
   ENDIF

   /* Shortcuts */
   ::oSC := IdeShortcuts():new( Self ):create()

   /* Insert command line projects */
   aeval( ::aHbpOnCmdLine, {|e| aadd( ::aINI[ INI_PROJECTS ], e ) } )
   /* Insert command line sources */
   aeval( ::aSrcOnCmdLine, {|e| aadd( ::aINI[ INI_FILES    ], hbide_parseSourceComponents( e ) ) } )

   /* Set variables from last session */
   ::cWrkTheme       := ::aINI[ INI_HBIDE, CurrentTheme       ]
   ::cWrkCodec       := ::aINI[ INI_HBIDE, CurrentCodec       ]
   ::cWrkEnvironment := ::aINI[ INI_HBIDE, CurrentEnvironment ]
   ::cWrkFind        := ::aINI[ INI_HBIDE, CurrentFind        ]
   ::cWrkFolderFind  := ::aINI[ INI_HBIDE, CurrentFolderFind  ]
   ::cWrkReplace     := ::aINI[ INI_HBIDE, CurrentReplace     ]
   ::cWrkView        := ::aINI[ INI_HBIDE, CurrentView        ]
   ::cWrkHarbour     := ::aINI[ INI_HBIDE, CurrentHarbour     ]

   /* Store to restore when all preliminary operations are completed */
   cView := ::cWrkView

   /* Load Code Skeletons */
   hbide_loadSkltns( Self )

   /* Set Codec at the Begining */
   HbXbp_SetCodec( ::cWrkCodec )

   /* Load IDE|User defined Themes */
   hbide_loadThemes( Self )

   /* Harbour Help Object */
   ::oHL := ideHarbourHelp():new():create( Self )

   /* DOCKing windows and ancilliary windows */
   ::oDK := IdeDocks():new():create( Self )
   /* IDE's Main Window */
   ::oDK:buildDialog()
   /* Actions */
   ::oAC := IdeActions():new():create( Self )
   /* Tools Manager */
   ::oTM := IdeToolsManager():new( Self ):create()

   /* Docking Widgets */
   ::oDK:buildDockWidgets()
   /* Toolbar */
   ::oAC:buildToolBar()
   /* Main Menu */
   ::oAC:buildMainMenu()

   /* Initialize Doc Writer Manager */
   ::oDW := IdeDocWriter():new( Self ):create()

   /* Once create Find/Replace dialog */
   ::oFR := IdeFindReplace():new( Self ):create()
   ::oFF := IdeFindInFiles():new( Self ):create()

   /* Sources Manager */
   ::oSM := IdeSourcesManager():new( Self ):create()

   /* Edits Manager */
   ::oEM := IdeEditsManager():new( Self ):create()

   /* Load Environments */
   ::oEV := IdeEnvironments():new( Self, hbide_pathToOSPath( ::aINI[ INI_HBIDE, PathEnv ] + ::pathSep + "hbide.env" ) ):create()

   /* Home Implementation */
   ::oHM := IdeHome():new():create( Self )

   /* Fill various elements of the IDE */
   ::cWrkProject := ::aINI[ INI_HBIDE, CurrentProject ]
   ::oPM:populate()
   ::oSM:loadSources()
   #if 0
   ::oDK:setView( ::cWrkView )
   IF !empty( ::aIni[ INI_FILES ] )
      ::oEM:setSourceVisibleByIndex( max( 0, val( ::aIni[ INI_HBIDE, RecentTabIndex ] ) )
   ENDIF
   #endif

   ::updateTitleBar()
   /* Set some last settings */
   ::oPM:setCurrentProject( ::cWrkProject, .f. )

   /* Restore Settings */
   hbide_restSettings( Self )
   /* Again to be displayed in Statusbar */
   HbXbp_SetCodec( ::cWrkCodec )
   ::oDK:setStatusText( SB_PNL_CODEC, ::cWrkCodec )
   ::oDK:setStatusText( SB_PNL_THEME, ::cWrkTheme )

   /* Display cWrkEnvironment in StatusBar */
   ::oDK:dispEnvironment( ::cWrkEnvironment )

   /* These docks must not be visible even IDE exits them open */
   #if 0
   ::oPropertiesDock:hide()
   ::oEnvironDock:hide()
   ::oThemesDock:hide()
   ::oSkeltnDock:hide()
   ::oHelpDock:hide()
   ::oFindDock:hide()
   ::oDockB1:hide()
   ::oDockB2:hide()
   ::oDockB:hide()
   ::oDocViewDock:hide()
   ::oDocWriteDock:hide()
   #endif

   #if 0 /* for screen capture */
   n := seconds()
   DO WHILE .t.
      IF seconds() > n + 10
         EXIT
      ENDIF
      QApplication():new():processEvents()
   ENDDO
   #endif

   /* Request Main Window to Appear on the Screen */
   ::oHM:refresh()
   ::oDlg:Show()
   IF ::nRunMode == HBIDE_RUN_MODE_PRG
      ::oDockPT:hide()
      ::oDockED:hide()
      ::oDK:setView( "Main" )
   ELSEIF ::nRunMode == HBIDE_RUN_MODE_HBP
      ::oDockED:hide()
      ::oDK:setView( "Stats" )
   ELSE
      ::oDK:setView( "Stats" )
      ::oDK:setView( cView )
   ENDIF

   ::showApplicationCursor()
   qSplash:close()

   /* Load tags last tagged projects */
   ::oFN:loadTags( ::aINI[ INI_TAGGEDPROJECTS ] )

   DO WHILE .t.
      ::nEvent := AppEvent( @::mp1, @::mp2, @::oXbp )

      IF ::nEvent == xbeP_Quit
         HB_TRACE( HB_TR_ALWAYS, "---------------- xbeP_Quit" )
         hbide_saveINI( Self )
         EXIT
      ENDIF

      IF ::nEvent == xbeP_Close
         HB_TRACE( HB_TR_ALWAYS, "================ xbeP_Close" )
         hbide_saveINI( Self )
         HB_TRACE( HB_TR_ALWAYS, "================ xbeP_Close", "after: hbide_saveINI( Self )"   )
         ::oSM:closeAllSources()
         HB_TRACE( HB_TR_ALWAYS, "================ xbeP_Close", "after: ::oSM:closeAllSources()" )
         EXIT

      ELSEIF ::nEvent == xbeP_Keyboard
         DO CASE

         CASE ::mp1 == xbeK_INS
            IF !empty( ::qCurEdit )
               ::qCurEdit:setOverwriteMode( !::qCurEdit:overwriteMode() )
               ::oCurEditor:dispEditInfo( ::qCurEdit )
            ENDIF

         CASE ::mp1 == xbeK_ESC
            ::oSM:closeSource()

         CASE ::mp1 == xbeK_CTRL_G
            ::oEM:goto()

         CASE ::mp1 == xbeK_CTRL_F
            IF !empty( ::qCurEdit )
               ::oFR:show()
            ENDIF

         CASE ::mp1 == xbeK_CTRL_N
            IF !empty( ::qCurEdit )
               ::oFR:find()
            ENDIF

         CASE ::mp1 == xbeK_CTRL_R
            IF !empty( ::qCurEdit )
               ::oFR:replace()
            ENDIF

         ENDCASE

      ENDIF

      ::oXbp:handleEvent( ::nEvent, ::mp1, ::mp2 )
   ENDDO

   /* Very important - destroy resources */
   HB_TRACE( HB_TR_ALWAYS, "======================================================" )
   HB_TRACE( HB_TR_ALWAYS, "Before    ::oDlg:destroy()", memory( 1001 )             )
   HB_TRACE( HB_TR_ALWAYS, "                                                      " )
#if 1
   ::oTM:destroy()
   ::oSK:destroy()
   ::oSC:destroy()
   ::oDW:destroy()
   ::oEV:destroy()
   ::oFN:destroy()
   ::oHM:destroy()
   ::oHL:destroy()
   ::oTH:destroy()
   ::oFF:destroy()
   ::oFR:destroy()
   ::oPM:destroy()
   ::oEM:destroy()
   ::oDK:destroy()
   ::oSearchReplace:destroy()
   ::oDlg:destroy()
   ::oAC:destroy()

   ::oFont := NIL
   qSplash := NIL
#endif
   HB_TRACE( HB_TR_ALWAYS, "                                                      " )
   HB_TRACE( HB_TR_ALWAYS, "After     ::oDlg:destroy()", memory( 1001 )             )
   HB_TRACE( HB_TR_ALWAYS, "======================================================" )

   RETURN self

/*----------------------------------------------------------------------*/

METHOD HbIde:parseParams()
   LOCAL s, cExt
   LOCAL aIni := {}

   FOR EACH s IN ::aParams
      s := alltrim( s )

      DO CASE
      CASE left( s, 1 ) == "-"
         // Switches, futuristic
      OTHERWISE
         hb_fNameSplit( s, , , @cExt )
         cExt := lower( cExt )
         DO CASE
         CASE cExt == ".ini"
            aadd( aIni, s )
         CASE cExt == ".hbp"
            aadd( ::aHbpOnCmdLine, s )
         CASE cExt $ ".prg.cpp"
            aadd( ::aSrcOnCmdLine, s )
         ENDCASE
      ENDCASE
   NEXT

   IF !empty( aIni )                       /* Discard aHbp */
      ::cProjIni := aIni[ 1 ]
      ::nRunMode := HBIDE_RUN_MODE_INI
   ELSEIF !empty( ::aHbpOnCmdLine )
      ::cProjIni := ""
      ::nRunMode := HBIDE_RUN_MODE_HBP
   ELSEIF !empty( ::aSrcOnCmdLine )
      ::cProjIni := ""
      ::nRunMode := HBIDE_RUN_MODE_PRG
   ELSE
      ::cProjIni := ""
      ::nRunMode := HBIDE_RUN_MODE_INI
   ENDIF

   RETURN Self

/*----------------------------------------------------------------------*/

METHOD HbIde:showApplicationCursor( nCursor )

   LOCAL qCrs

   IF empty( nCursor )
      QApplication():new():restoreOverrideCursor()
   ELSE
      qCrs := QCursor():new( nCursor )
      QApplication():new():setOverrideCursor( qCrs )
   ENDIF

   RETURN Self

/*----------------------------------------------------------------------*/

METHOD HbIde:execAction( cKey )

   SWITCH cKey
   CASE "Tools"
      ::oTM:show()
      EXIT
   CASE "Environments"
      ::oEV:fetchNew()
      EXIT
   CASE "Exit"
      PostAppEvent( xbeP_Close, NIL, NIL, ::oDlg )
      EXIT
   CASE "Home"
      ::oDK:setView( "Stats" )
      EXIT
   CASE "Animate"
      ::nAnimantionMode := iif( ::nAnimantionMode == HBIDE_ANIMATION_NONE, HBIDE_ANIMATION_GRADIENT, HBIDE_ANIMATION_NONE )
      ::oDK:animateComponents( ::nAnimantionMode )
      EXIT
   CASE "Shortcuts"
      ::oSC:show()
      EXIT
   CASE "NewProject"
   CASE "LoadProject"
   CASE "LaunchProject"
   CASE "Build"
   CASE "BuildLaunch"
   CASE "Rebuild"
   CASE "RebuildLaunch"
   CASE "Compile"
   CASE "CompilePPO"
   CASE "Properties"
   CASE "SelectProject"
   CASE "CloseProject"
      ::execProjectAction( cKey )
      EXIT
   CASE "New"
   CASE "Open"
   CASE "Save"
   CASE "SaveAs"
   CASE "SaveAll"
   CASE "SaveExit"
   CASE "Revert"
   CASE "Close"
   CASE "CloseAll"
   CASE "CloseOther"
      ::execSourceAction( cKey )
      EXIT
   CASE "Print"
   CASE "Undo"
   CASE "Redo"
   CASE "Cut"
   CASE "Copy"
   CASE "Paste"
   CASE "SelectAll"
   CASE "SelectionMode"
   CASE "DuplicateLine"
   CASE "DeleteLine"
   CASE "MoveLineUp"
   CASE "MoveLineDown"
   CASE "BlockComment"
   CASE "StreamComment"
   CASE "BlockIndentR"
   CASE "BlockIndentL"
   CASE "BlockSgl2Dbl"
   CASE "BlockDbl2Sgl"
   CASE "switchReadOnly"
   CASE "Find"
   CASE "FindEx"
   CASE "SetMark"
   CASE "GotoMark"
   CASE "Goto"
   CASE "ToUpper"
   CASE "ToLower"
   CASE "Invert"
   CASE "MatchPairs"
   CASE "Tools"
   CASE "InsertSeparator"
   CASE "InsertDateTime"
   CASE "InsertRandomName"
   CASE "InsertExternalFile"
   CASE "ZoomIn"
   CASE "ZoomOut"
   CASE "FormatBraces"
   CASE "RemoveTabs"
   CASE "RemoveTrailingSpaces"
      ::execEditorAction( cKey )
      EXIT
   CASE "ToggleProjectTree"
   CASE "ToggleBuildInfo"
   CASE "ToggleFuncList"
      //::execWindowsAction( cKey )
      EXIT
   CASE "Help"
      ::oHelpDock:show()
      EXIT
   ENDSWITCH

   ::manageFocusInEditor()

   RETURN nil

/*----------------------------------------------------------------------*/

METHOD HbIde:execEditorAction( cKey )

   SWITCH cKey
   CASE "Print"
      ::oEM:printPreview()
      EXIT
   CASE "Undo"
      ::oEM:undo()
      EXIT
   CASE "Redo"
      ::oEM:redo()
      EXIT
   CASE "Cut"
      ::oEM:cut()
      EXIT
   CASE "Copy"
      ::oEM:copy()
      EXIT
   CASE "Paste"
      ::oEM:paste()
      EXIT
   CASE "SelectAll"
      ::oEM:selectAll()
      EXIT
   CASE "SelectionMode"
      ::isColumnSelectionEnabled := ! ::isColumnSelectionEnabled
      ::oEM:toggleSelectionMode()
      EXIT
   CASE "DuplicateLine"
      ::oEM:duplicateLine()
      EXIT
   CASE "MoveLineUp"
      ::oEM:moveLine( -1 )
      EXIT
   CASE "MoveLineDown"
      ::oEM:moveLine( 1 )
      EXIT
   CASE "DeleteLine"
      ::oEM:deleteLine()
      EXIT
   CASE "BlockComment"
      ::oEM:blockComment()
      EXIT
   CASE "StreamComment"
      ::oEM:streamComment()
      EXIT
   CASE "BlockIndentR"
      ::oEM:indent( 1 )
      EXIT
   CASE "BlockIndentL"
      ::oEM:indent( -1 )
      EXIT
   CASE "BlockSgl2Dbl"
      ::oEM:convertDQuotes()
      EXIT
   CASE "BlockDbl2Sgl"
      ::oEM:convertQuotes()
      EXIT
   CASE "switchReadOnly"
      ::oEM:switchToReadOnly()
      EXIT
   CASE "Find"
      IF !Empty( ::qCurEdit )
         ::oFR:show()
      ENDIF
      EXIT
   CASE "FindEx"
      IF !Empty( ::qCurEdit )
         ::oSearchReplace:beginFind()
      ENDIF
      EXIT
   CASE "SetMark"
      ::oEM:setMark()
      EXIT
   CASE "GotoMark"
      ::oEM:gotoMark()
      EXIT
   CASE "Goto"
      ::oEM:goTo()
      EXIT
   CASE "ToUpper"
      ::oEM:convertSelection( cKey )
      EXIT
   CASE "ToLower"
      ::oEM:convertSelection( cKey )
      EXIT
   CASE "Invert"
      ::oEM:convertSelection( cKey )
      EXIT
   CASE "MatchPairs"
      EXIT
   CASE "Tools"
      ::oTM:show()
      EXIT
   CASE "InsertSeparator"
      ::oEM:insertSeparator()
      EXIT
   CASE "InsertDateTime"
      ::oEM:insertText( cKey )
      EXIT
   CASE "InsertRandomName"
      ::oEM:insertText( cKey )
      EXIT
   CASE "InsertExternalFile"
      ::oEM:insertText( cKey )
      EXIT
   CASE "ZoomIn"
      ::oEM:zoom( 1 )
      EXIT
   CASE "ZoomOut"
      ::oEM:zoom( 0 )
      EXIT
   CASE "FormatBraces"
      ::oEM:formatBraces()
      EXIT
   CASE "RemoveTabs"
      ::oEM:removeTabs()
      EXIT
   CASE "RemoveTrailingSpaces"
      ::oEM:removeTrailingSpaces()
      EXIT
   ENDSWITCH
   RETURN Self

/*----------------------------------------------------------------------*/

METHOD HbIde:execSourceAction( cKey )
   SWITCH cKey
   CASE "New"
      ::oSM:editSource( '' )
      EXIT
   CASE "Open"
      ::oSM:openSource()
      EXIT
   CASE "Save"
      ::oSM:saveSource( ::oEM:getTabCurrent(), .f., .f. )
      EXIT
   CASE "SaveAs"
      ::oSM:saveSource( ::oEM:getTabCurrent(), .t., .t. )
      EXIT
   CASE "SaveAll"
      ::oSM:saveAllSources()
      EXIT
   CASE "SaveExit"
      ::oSM:saveAndExit()
      EXIT
   CASE "Revert"
      ::oSM:RevertSource()
      EXIT
   CASE "Close"
      ::oSM:closeSource()
      EXIT
   CASE "CloseAll"
      ::oSM:closeAllSources()
      EXIT
   CASE "CloseOther"
      ::oSM:closeAllOthers()
      EXIT
   ENDSWITCH
   RETURN Self

/*----------------------------------------------------------------------*/

METHOD HbIde:execProjectAction( cKey )
   SWITCH cKey
   CASE "NewProject"
      ::oPM:loadProperties( , .t., .t., .t. )
      EXIT
   CASE "LoadProject"
      ::oPM:loadProperties( , .f., .f., .t. )
      EXIT
   CASE "LaunchProject"
      ::oPM:launchProject()
      EXIT
   CASE "Build"
      ::oPM:buildProject( '', .F., .F. )
      EXIT
   CASE "BuildLaunch"
      ::oPM:buildProject( '', .T., .F. )
      EXIT
   CASE "Rebuild"
      ::oPM:buildProject( '', .F., .T. )
      EXIT
   CASE "RebuildLaunch"
      ::oPM:buildProject( '', .T., .T. )
      EXIT
   CASE "Compile"
      //
      EXIT
   CASE "CompilePPO"
      ::oPM:buildProject( '', .F., .F., .T., .T. )
      EXIT
   CASE "Properties"
      ::oPM:getProperties()
      EXIT
   CASE "SelectProject"
      ::oPM:selectCurrentProject()
      EXIT
   CASE "CloseProject"
      ::oPM:closeProject()
      EXIT
   ENDSWITCH
   RETURN Self

/*----------------------------------------------------------------------*/

METHOD HbIde:setPosAndSizeByIni( qWidget, nPart )
   LOCAL aRect

   IF !empty( ::aIni[ INI_HBIDE, nPart ] )
      aRect := hb_atokens( ::aIni[ INI_HBIDE, nPart ], "," )
      aeval( aRect, {|e,i| aRect[ i ] := val( e ) } )

      qWidget:move( aRect[ 1 ], aRect[ 2 ] )
      qWidget:resize( aRect[ 3 ], aRect[ 4 ] )
   ENDIF

   RETURN Self

/*----------------------------------------------------------------------*/

METHOD HbIde:setPosByIni( qWidget, nPart )
   LOCAL aRect

   IF !empty( ::aIni[ INI_HBIDE, nPart ] )
      aRect := hb_atokens( ::aIni[ INI_HBIDE, nPart ], "," )
      aeval( aRect, {|e,i| aRect[ i ] := val( e ) } )

      qWidget:move( aRect[ 1 ], aRect[ 2 ] )
   ENDIF

   RETURN Self

/*----------------------------------------------------------------------*/

METHOD HbIde:setSizeByIni( qWidget, nPart )
   LOCAL aRect

   IF !empty( ::aIni[ INI_HBIDE, nPart ] )
      aRect := hb_atokens( ::aIni[ INI_HBIDE, nPart ], "," )
      aeval( aRect, {|e,i| aRect[ i ] := val( e ) } )
      qWidget:resize( aRect[ 3 ], aRect[ 4 ] )
   ENDIF

   RETURN Self

/*----------------------------------------------------------------------*/

METHOD HbIde:manageFocusInEditor()
   LOCAL qEdit

   IF !empty( qEdit := ::oEM:getEditCurrent() )
      qEdit:setFocus()
   ENDIF

   RETURN self

/*----------------------------------------------------------------------*/

METHOD HbIde:removeProjectTree( aPrj )
   LOCAL oProject, nIndex, oParent, oP, n

   oProject := IdeProject():new( Self, aPrj )
   IF empty( oProject:title )
      RETURN Self
   ENDIF
   nIndex := aScan( ::aProjData, {|e_| e_[ TRE_TYPE ] == "Project Name" .AND. e_[ TRE_ORIGINAL ] == oProject:title } )
   IF nIndex > 0
      oParent := ::aProjData[ nIndex, TRE_OITEM ]
      DO WHILE .t.
         n := ascan( ::aProjData, {|e_| e_[ TRE_OPARENT ] == oParent } )
         IF n == 0
            EXIT
         ENDIF
         oParent:delItem( ::aProjData[ n, TRE_OITEM ] )
         hb_adel( ::aProjData, n, .t. )
      ENDDO
   ENDIF

   oP := oParent

   SWITCH oProject:type
   CASE "Executable"
      oParent := ::aProjData[ 1, 1 ]
      EXIT
   CASE "Lib"
      oParent := ::aProjData[ 2, 1 ]
      EXIT
   CASE "Dll"
      oParent := ::aProjData[ 3, 1 ]
      EXIT
   ENDSWITCH

   nIndex := aScan( ::aProjData, {|e_| e_[ TRE_OITEM ] == oP } )
   oParent:delItem( oP )
   hb_adel( ::aProjData, nIndex, .t. )

   RETURN Self

/*----------------------------------------------------------------------*/

METHOD HbIde:updateProjectTree( aPrj )
   LOCAL oProject, n, oSource, oItem, nProjExists, oP, oParent

   oProject := IdeProject():new( Self, aPrj )

   IF empty( oProject:title )
      RETURN Self
   ENDIF

   SWITCH oProject:type
   CASE "Executable"
      oParent := ::aProjData[ 1, 1 ]
      EXIT
   CASE "Lib"
      oParent := ::aProjData[ 2, 1 ]
      EXIT
   CASE "Dll"
      oParent := ::aProjData[ 3, 1 ]
      EXIT
   ENDSWITCH

   nProjExists := aScan( ::aProjData, {|e_| e_[ TRE_TYPE ] == "Project Name" .AND. e_[ TRE_ORIGINAL ] == oProject:title } )

   IF nProjExists > 0
      nProjExists := aScan( oParent:aChilds, {|o| o:caption == oProject:title } )
      IF nProjExists > 0
         oP := oParent:aChilds[ nProjExists ]
      ELSE
         RETURN Self  /* Some Error - It must never happen */
      ENDIF
      /* Delete Existing Nodes */
      DO WHILE .t.
         IF ( n := ascan( ::aProjData, {|e_| e_[ TRE_OPARENT ] == oP } ) ) == 0
            EXIT
         ENDIF
         oP:delItem( ::aProjData[ n, TRE_OITEM ] )
         hb_adel( ::aProjData, n, .t. )
      ENDDO
   ENDIF
   IF empty( oP )
      oParent:expand( .t. )
      oP := oParent:addItem( oProject:title )
      aadd( ::aProjData, { oP, "Project Name", oParent, oProject:title, aPrj, oProject } )
   ENDIF
   FOR EACH oSource IN oProject:hSources
      oItem := oP:addItem( oSource:file + oSource:ext )
      oItem:tooltipText := oSource:original
      oItem:oWidget:setIcon( 0, hbide_image( hbide_imageForFileType( oSource:ext ) ) )
      aadd( ::aProjData, { oItem, "Source File", oP, oSource:original, oProject:title } )
   NEXT
   oP:oWidget:sortChildren( 0 )

   RETURN Self

/*----------------------------------------------------------------------*/

METHOD HbIde:manageItemSelected( oXbpTreeItem )
   LOCAL n, cHbp

   IF     oXbpTreeItem == ::oProjRoot
      n := -1
   ELSEIF oXbpTreeItem == ::oOpenedSources
      n := -2
   ELSE
      n := ascan( ::aProjData, {|e_| e_[ 1 ] == oXbpTreeItem } )
   ENDIF

   DO CASE
   CASE n ==  0  // Source File - nothing to do
   CASE n == -2  // "Files"
   CASE n == -1
   CASE ::aProjData[ n, TRE_TYPE ] == "Project Name"
      cHbp := ::oPM:getProjectFileNameFromTitle( ::aProjData[ n, TRE_ORIGINAL ] )
      ::oPM:loadProperties( cHbp, .f., .t., .f. )

   CASE ::aProjData[ n, TRE_TYPE ] == "Source File"
      ::oSM:editSource( hbide_stripFilter( ::aProjData[ n, TRE_ORIGINAL ] ) )

   CASE ::aProjData[ n, TRE_TYPE ] == "Opened Source"
      ::oEM:setSourceVisible( ::aProjData[ n, TRE_DATA ] )

   CASE ::aProjData[ n, TRE_TYPE ] == "Path"

   ENDCASE

   // ::manageFocusInEditor()
   RETURN Self

/*----------------------------------------------------------------------*/

METHOD HbIde:manageProjectContext( mp1, mp2, oXbpTreeItem )
   LOCAL n, cHbp, s
   LOCAL aPops := {}, aSub :={}

   HB_SYMBOL_UNUSED( mp2 )

   oXbpTreeItem := ::oCurProjItem

   IF     oXbpTreeItem == ::oProjRoot
      n  := -1
   ELSEIF oXbpTreeItem == ::oOpenedSources
      n  := -2
   ELSE
      n := ascan( ::aProjData, {|e_| e_[ 1 ] == oXbpTreeItem } )
   ENDIF

   DO CASE
   CASE n ==  0  // Source File - nothing to do
   CASE n == -2  // "Files"
   CASE n == -1  // Project Root
      aadd( aPops, { "New Project"                       , {|| ::oPM:loadProperties( NIL, .t., .t., .t. ) } } )
      aadd( aPops, { "" } )
      aadd( aPops, { "Open Project..."                   , {|| ::oPM:loadProperties( NIL, .f., .f., .t. ) } } )
      aadd( aPops, { "" } )
      //
      IF !empty( ::oEV:getNames() )
         aadd( aPops, { "" } )
         FOR EACH s IN ::oEV:getNames()
            aadd( aSub, { s                              , {|x| ::cWrkEnvironment := x, ::oDK:dispEnvironment( x ) } } )
         NEXT
         aadd( aPops, { aSub, "Environment..." } )
      ENDIF
      //
      hbide_ExecPopup( aPops, mp1, ::oProjTree:oWidget )

   CASE ::aProjData[ n, TRE_TYPE ] == "Project Name"
      cHbp := hbide_pathToOSPath( ::oPM:getProjectFileNameFromTitle( ::aProjData[ n, TRE_ORIGINAL ] ) )
      //
      IF Alltrim( Upper( ::cWrkProject ) ) != Alltrim( Upper( oXbpTreeItem:caption ) )
         aadd( aPops, { "Set as Current"                 , {|| ::oPM:setCurrentProject( oXbpTreeItem:caption ) } } )
      End
      aadd( aPops, { "Properties"                        , {|| ::oPM:loadProperties( cHbp, .f., .t., .t. ) } } )
      aadd( aPops, { "" } )
      aadd( aPops, { ::oAC:getAction( "BuildQt"         ), {|| ::oPM:buildProject( oXbpTreeItem:caption, .F.,    , , .T. ) } } )
      aadd( aPops, { ::oAC:getAction( "BuildLaunchQt"   ), {|| ::oPM:buildProject( oXbpTreeItem:caption, .T.,    , , .T. ) } } )
      aadd( aPops, { ::oAC:getAction( "ReBuildQt"       ), {|| ::oPM:buildProject( oXbpTreeItem:caption, .F., .T., , .T. ) } } )
      aadd( aPops, { ::oAC:getAction( "ReBuildLaunchQt" ), {|| ::oPM:buildProject( oXbpTreeItem:caption, .T., .T., , .T. ) } } )
      aadd( aPops, { "" } )
      aadd( aPops, { "Launch"                            , {|| ::oPM:launchProject( oXbpTreeItem:caption ) } } )
      aadd( aPops, { "" } )
      aadd( aPops, { "Remove Project"                    , {|| ::oPM:removeProject( oXbpTreeItem:caption ) } } )
      IF !empty( ::oEV:getNames() )
         aadd( aPops, { "" } )
         FOR EACH s IN ::oEV:getNames()
            aadd( aSub, { s                              , {|x| ::cWrkEnvironment := x, ::oDK:dispEnvironment( x ) } } )
         NEXT
         aadd( aPops, { aSub, "Select an environment" } )
      ENDIF

      hbide_ExecPopup( aPops, mp1, ::oProjTree:oWidget )

   CASE ::aProjData[ n, TRE_TYPE ] == "Source File"
      //

   CASE ::aProjData[ n, TRE_TYPE ] == "Opened Source"
      n := ::oEM:getTabBySource( ::aProjData[ n, 5 ] )
      //
      aadd( aPops, { "Save"                              , {|| ::oSM:saveSource( n )                 } } )
      aadd( aPops, { "Save As"                           , {|| ::oSM:saveSource( n, , .t. )          } } )
      aadd( aPops, { "" } )
      aadd( aPops, { "Close"                             , {|| ::oSM:closeSource( n )                } } )
      aadd( aPops, { "Close Others"                      , {|| ::oSM:closeAllOthers( n )             } } )
      aadd( aPops, { "" } )
      aadd( aPops, { "Apply Theme"                       , {|| ::oEM:getEditorCurrent():applyTheme() } } )
      //
      hbide_ExecPopup( aPops, mp1, ::oProjTree:oWidget )

   CASE ::aProjData[ n, 2 ] == "Path"

   ENDCASE

   ::manageFocusInEditor()
   RETURN Self

/*----------------------------------------------------------------------*/

METHOD HbIde:updateFuncList()
   LOCAL o

   ::oFuncList:clear()
   IF !empty( ::aTags )
      aeval( ::aTags, {|e_| o := ::oFuncList:addItem( e_[ 7 ] ) } )
   ENDIF

   RETURN Self

/*----------------------------------------------------------------------*/

METHOD HbIde:gotoFunction( mp1, mp2, oListBox )
   LOCAL n, cAnchor, oEdit, lFound

   mp1 := oListBox:getData()
   mp2 := oListBox:getItem( mp1 )

   IF ( n := ascan( ::aTags, {|e_| mp2 == e_[ 7 ] } ) ) > 0
      cAnchor := trim( ::aText[ ::aTags[ n,3 ] ] )
      IF !empty( oEdit := ::oEM:getEditCurrent() )
         IF !( lFound := oEdit:find( cAnchor, QTextDocument_FindCaseSensitively ) )
            lFound := oEdit:find( cAnchor, QTextDocument_FindBackward + QTextDocument_FindCaseSensitively )
         ENDIF
         IF lFound
            oEdit:centerCursor()
         ENDIF
      ENDIF
   ENDIF
   ::manageFocusInEditor()

   RETURN Self

/*----------------------------------------------------------------------*/

METHOD HbIde:manageFuncContext( mp1 )
   LOCAL aPops := {}

   IF ::oFuncList:numItems() > 0
      aadd( aPops, { 'Comment out'           , {|| NIL } } )
      aadd( aPops, { 'Reformat'              , {|| NIL } } )
      aadd( aPops, { 'Print'                 , {|| NIL } } )
      aadd( aPops, { 'Delete'                , {|| NIL } } )
      aadd( aPops, { 'Move to another source', {|| NIL } } )

      hbide_ExecPopup( aPops, mp1, ::oFuncList:oWidget )

      ::manageFocusInEditor()
   ENDIF
   RETURN Self

/*----------------------------------------------------------------------*/

METHOD HbIde:CreateTags()
   LOCAL aSumData := ""
   LOCAL cComments, aSummary, i, cPath, cSource, cExt

   ::aTags := {}

   FOR i := 1 TO Len( ::aSources )
      HB_FNameSplit( ::aSources[ i ], @cPath, @cSource, @cExt )

      IF Upper( cExt ) $ ".PRG.CPP"
         IF !empty( ::aText := hbide_readSource( ::aSources[ i ] ) )
            aSumData  := {}

            cComments := CheckComments( ::aText )
            aSummary  := Summarize( ::aText, cComments, @aSumData , iif( Upper( cExt ) == ".PRG", 9, 1 ) )
            ::aTags   := UpdateTags( ::aSources[ i ], aSummary, aSumData, @::aFuncList, @::aLines )

            #if 0
            IF !empty( aTags )
               aeval( aTags, {|e_| aadd( ::aTags, e_ ) } )
               ::hData[ cSource+cExt ] := { a[ i ], aTags, aclone( ::aText ), cComments, ::aFuncList, ::aLines }
               aadd( ::aSrcLines, ::aText   )
               aadd( ::aComments, cComments )
            ENDIF
            #endif
         ENDIF
      ENDIF
   NEXT

   RETURN ( NIL )

//----------------------------------------------------------------------//
/*
 * Update the project menu to show current info.
 * 03/01/2010 - 12:48:18 - vailtom
 */
METHOD HbIde:updateProjectMenu()
   LOCAL oItem := hbide_mnuFindItem( Self, 'Project' )

   IF Empty( oItem )
      RETURN Self
   ENDIF

   IF Empty( ::cWrkProject )
      oItem[ 2 ]:setDisabled( .T. )
      RETURN Self
   ENDIF

   oItem[ 2 ]:setEnabled( .T. )
   RETURN Self

/*----------------------------------------------------------------------*/
/*
 * Updates the title bar of the main window, indicating the project and the
 * current filename.
 * 02/01/2010 - 16:30:06 - vailtom
 */
METHOD HbIde:updateTitleBar()
   LOCAL cTitle := "Harbour IDE (r" + hb_ntos( hb_version( HB_VERSION_REVISION ) ) + ")"
   LOCAL oEdit

   IF Empty( ::oDlg )
      RETURN Self
   ENDIF

   IF !Empty( ::cWrkProject )
      cTitle += " [" + ::cWrkProject + "] "
   ENDIF

   IF !empty( oEdit := ::oEM:getEditorCurrent() )
      IF Empty( oEdit:sourceFile )
         cTitle += " [" + oEdit:oTab:caption + "]"
      ELSE
         cTitle += " [" + oEdit:sourceFile + "]"
      ENDIF
   ENDIF

   ::oDlg:Title := cTitle
   ::oDlg:oWidget:setWindowTitle( ::oDlg:Title )

   RETURN Self

/*----------------------------------------------------------------------*/

METHOD HbIde:setCodec( cCodec )

   DEFAULT cCodec TO ::cWrkCodec

   ::cWrkCodec := cCodec

   HbXbp_SetCodec( ::cWrkCodec )

   ::oDK:setStatusText( SB_PNL_CODEC, ::cWrkCodec )

   RETURN Self

/*----------------------------------------------------------------------*/

METHOD HbIde:testPainter( qPainter )
   LOCAL qP := QPainter():from( qPainter )

   HB_TRACE( HB_TR_ALWAYS, "qPainter:isActive()", qP:isActive() )

   qP:setPen_2( Qt_red )
   qP:drawEllipse_2( 100,300,100,150 )
   qP:setFont( ::oFont:oWidget )
   qP:drawText_4( 100,300,"Harbour" )

   //qPainter:fillRect_8( 100, 100, 500, 500, QColor():new( 175, 175, 255 ) )

   RETURN NIL

/*----------------------------------------------------------------------*/
