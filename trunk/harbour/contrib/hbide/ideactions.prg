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
 *                               23Nov2009
 */
/*----------------------------------------------------------------------*/
/*----------------------------------------------------------------------*/
/*----------------------------------------------------------------------*/

#include "common.ch"
#include "hbclass.ch"
#include "xbp.ch"
#include "inkey.ch"
#include "hbide.ch"

/*----------------------------------------------------------------------*/

#define _T( x )  ( mnuNormalizeItem( x ) )

/*----------------------------------------------------------------------*/

CLASS IdeActions INHERIT IdeObject

   DATA   hActions                                INIT hb_hash()

   METHOD new( oIde )
   METHOD create( oIde )
   METHOD destroy()
   METHOD getAction( cKey )
   METHOD buildActions()
   METHOD loadActions()
   METHOD buildToolBar()
   METHOD buildMainMenu()

   ENDCLASS

/*----------------------------------------------------------------------*/

METHOD IdeActions:new( oIde )

   hb_hCaseMatch( ::hActions, .f. )
   ::oIde := oIde

   RETURN Self

/*----------------------------------------------------------------------*/

METHOD IdeActions:create( oIde )

   DEFAULT oIde TO ::oIde
   ::oIde := oIde

   ::buildActions()

   RETURN Self

/*----------------------------------------------------------------------*/

METHOD IdeActions:destroy()
   LOCAL qAction

   FOR EACH qAction IN ::hActions
      qAction:pPtr := NIL
   NEXT
   RETURN Self

/*----------------------------------------------------------------------*/

METHOD IdeActions:getAction( cKey )

   IF hb_hHasKey( ::hActions, cKey )
      RETURN ::hActions[ cKey ]
   ENDIF

   RETURN nil

/*----------------------------------------------------------------------*/

METHOD IdeActions:buildActions()
   LOCAL qAction, aAct, k, a_

   aAct := ::loadActions()

   FOR EACH a_ IN aAct
      IF !( hb_hHasKey( ::hActions, a_[ ACT_NAME ] ) )

         qAction := QAction():new( ::qDlg )
         qAction:setCheckable( iif( empty( a_[ ACT_CHECKABLE ] ), .F., upper( a_[ ACT_CHECKABLE ] ) == "YES" ) )
         qAction:setText( strtran( a_[ ACT_TEXT ], "~", "&" ) )
         IF !empty( a_[ ACT_IMAGE ] )
            qAction:setIcon( ::resPath + a_[ ACT_IMAGE ] + ".png" )
         ENDIF
         IF !empty( a_[ ACT_SHORTCUT ] )
            k := a_[ ACT_SHORTCUT ]
            k := strtran( k, "Sh+", "Shift+" )
            k := strtran( k, "SH+", "Shift+" )
            k := strtran( k, "^"  , "Ctrl+"  )
            qAction:setShortcut( QKeySequence():new( k ) )
         ENDIF
         qAction:setTooltip( strtran( a_[ ACT_TEXT ], "~", "" ) )

         ::hActions[ a_[ ACT_NAME ] ] := qAction

      ENDIF
   NEXT

   RETURN Self

/*----------------------------------------------------------------------*/

METHOD IdeActions:loadActions()
   LOCAL aAct := {}

   //    <Text> can be loaded from .ini or similar mechanism given <Name>
   //
   //            Name                     Text                             Image             Shortcut  Checkable  IconVisInMenu
   //
   aadd( aAct, { "TB_Exit"              , "E~xit"                        , "exit"           , ""     , "No", "Yes" } )
   aadd( aAct, { "TB_New"               , "~Source"                      , "new"            , ""     , "No", "Yes" } )
   aadd( aAct, { "TB_Open"              , "~Open"                        , "open"           , ""     , "No", "Yes" } )
   aadd( aAct, { "TB_Save"              , "~Save"                        , "save"           , ""     , "No", "Yes" } )
   aadd( aAct, { "TB_Close"             , "~Close"                       , "close"          , ""     , "No", "Yes" } )
   aadd( aAct, { "TB_Print"             , "~Print"                       , "print"          , ""     , "No", "Yes" } )
   aadd( aAct, { "TB_Compile"           , "Co~mpile"                     , "compile"        , ""     , "No", "Yes" } )
   aadd( aAct, { "TB_CompilePPO"        , "Com~pile to PPO"              , "ppo"            , ""     , "No", "Yes" } )
   aadd( aAct, { "TB_Build"             , "Build Project"                , "build"          , ""     , "No", "Yes" } )
   aadd( aAct, { "TB_BuildLaunch"       , "Build and Launch"             , "buildlaunch"    , ""     , "No", "Yes" } )
   aadd( aAct, { "TB_Rebuild"           , "Rebuild Project"              , "rebuild"        , ""     , "No", "Yes" } )
   aadd( aAct, { "TB_RebuildLaunch"     , "Rebuild and Launch"           , "rebuildlaunch"  , ""     , "No", "Yes" } )
   aadd( aAct, { "TB_ToggleProjectTree" , "Toggle Project Tree"          , "properties"     , ""     , "No", "Yes" } )
   aadd( aAct, { "TB_ToggleBuildInfo"   , "Toggle Build Info"            , "builderror"     , ""     , "No", "Yes" } )
   aadd( aAct, { "TB_ToggleFuncList"    , "Toggle Function List"         , "modulelist"     , ""     , "No", "Yes" } )
   aadd( aAct, { "TB_Undo"              , "~Undo"                        , "undo"           , ""     , "No", "Yes" } )
   aadd( aAct, { "TB_Redo"              , "~Redo"                        , "redo"           , ""     , "No", "Yes" } )
   aadd( aAct, { "TB_Cut"               , "C~ut"                         , "cut"            , ""     , "No", "Yes" } )
   aadd( aAct, { "TB_Copy"              , "~Copy"                        , "copy"           , ""     , "No", "Yes" } )
   aadd( aAct, { "TB_Paste"             , "~Paste"                       , "paste"          , ""     , "No", "Yes" } )
   aadd( aAct, { "TB_SelectAll"         , "Select ~All"                  , "selectall"      , ""     , "No", "Yes" } )
   aadd( aAct, { "TB_SelectionMode"     , "Toggle Selection Mode"        , "stream"         , ""     , "Yes", "Yes" } )
   aadd( aAct, { "TB_Find"              , "~Find / Replace"              , "find"           , ""     , "No", "Yes" } )
   aadd( aAct, { "TB_Search"            , "Search"                       , "search"         , ""     , "No", "Yes" } )
   aadd( aAct, { "TB_SetMark"           , "Toggle Mark"                  , "placeremovemark", ""     , "No", "Yes" } )
   aadd( aAct, { "TB_GotoMark"          , "Goto Mark"                    , "gotomark"       , ""     , "No", "Yes" } )
   aadd( aAct, { "TB_Goto"              , "~Goto Line"                   , "gotoline"       , ""     , "No", "Yes" } )
   aadd( aAct, { "TB_ToUpper"           , "To Upper"                     , "toupper"        , ""     , "No", "Yes" } )
   aadd( aAct, { "TB_ToLower"           , "To Lower"                     , "tolower"        , ""     , "No", "Yes" } )
   aadd( aAct, { "TB_Invert"            , "Invert"                       , "invertcase"     , ""     , "No", "Yes" } )
   aadd( aAct, { "TB_MatchPairs"        , "Match Pairs"                  , "matchobj"       , ""     , "No", "Yes" } )
   aadd( aAct, { "TB_ZoomIn"            , "ZoomIn"                       , "zoomin"         , ""     , "No", "Yes" } )
   aadd( aAct, { "TB_ZoomOut"           , "ZoomOut"                      , "zoomout"        , ""     , "No", "Yes" } )
   //
   aadd( aAct, { "Help"                 , "~Help"                        , "help"           , "F1"   , "No", "Yes" } )
   aadd( aAct, { "Exit"                 , "E~xit"                        , "exit"           , "Sh+^W", "No", "Yes" } )
   aadd( aAct, { "New"                  , "~Source"                      , "new"            , "^N"   , "No", "Yes" } )
   aadd( aAct, { "Open"                 , "~Open"                        , "open"           , "^O"   , "No", "Yes" } )
   aadd( aAct, { "Save"                 , "~Save"                        , "save"           , "^S"   , "No", "Yes" } )
   aadd( aAct, { "Close"                , "~Close"                       , "close"          , "^W"   , "No", "Yes" } )
   aadd( aAct, { "Print"                , "~Print"                       , "print"          , "^P"   , "No", "Yes" } )
   aadd( aAct, { "Compile"              , "Co~mpile"                     , "compile"        , ""     , "No", "Yes" } )
   aadd( aAct, { "CompilePPO"           , "Com~pile to PPO"              , "ppo"            , ""     , "No", "Yes" } )
   aadd( aAct, { "Build"                , "Build Project"                , "build"          , ""     , "No", "Yes" } )
   aadd( aAct, { "BuildLaunch"          , "Build and Launch"             , "buildlaunch"    , ""     , "No", "Yes" } )
   aadd( aAct, { "Rebuild"              , "Rebuild Project"              , "rebuild"        , ""     , "No", "Yes" } )
   aadd( aAct, { "RebuildLaunch"        , "Rebuild and Launch"           , "rebuildlaunch"  , ""     , "No", "Yes" } )
   aadd( aAct, { "ToggleProjectTree"    , "Toggle Project Tree"          , "properties"     , ""     , "No", "Yes" } )
   aadd( aAct, { "ToggleBuildInfo"      , "Toggle Build Info"            , "builderror"     , ""     , "No", "Yes" } )
   aadd( aAct, { "ToggleFuncList"       , "Toggle Function List"         , "modulelist"     , ""     , "No", "Yes" } )
   aadd( aAct, { "Undo"                 , "~Undo"                        , "undo"           , ""     , "No", "Yes" } )
   aadd( aAct, { "Redo"                 , "~Redo"                        , "redo"           , ""     , "No", "Yes" } )
   aadd( aAct, { "Cut"                  , "C~ut"                         , "cut"            , ""     , "No", "Yes" } )
   aadd( aAct, { "Copy"                 , "~Copy"                        , "copy"           , ""     , "No", "Yes" } )
   aadd( aAct, { "Paste"                , "~Paste"                       , "paste"          , ""     , "No", "Yes" } )
   aadd( aAct, { "SelectAll"            , "Select ~All"                  , "selectall"      , ""     , "No", "Yes" } )
   aadd( aAct, { "SelectionMode"        , "Toggle Selection Mode"        , "stream"         , ""     , "No", "Yes" } )
   aadd( aAct, { "Find"                 , "~Find / Replace"              , "find"           , "^F"   , "No", "Yes" } )
   aadd( aAct, { "Search"               , "F~ind in Files"               , "search"         , ""     , "No", "Yes" } )
   aadd( aAct, { "SetMark"              , "Set Mark"                     , "placeremovemark", ""     , "No", "Yes" } )
   aadd( aAct, { "GotoMark"             , "Goto Mark"                    , "gotomark"       , ""     , "No", "Yes" } )
   aadd( aAct, { "Goto"                 , "~Goto Line"                   , "gotoline"       , "^G"   , "No", "Yes" } )
   aadd( aAct, { "ToUpper"              , "To Upper"                     , "toupper"        , ""     , "No", "Yes" } )
   aadd( aAct, { "ToLower"              , "To Lower"                     , "tolower"        , ""     , "No", "Yes" } )
   aadd( aAct, { "Invert"               , "Invert"                       , "invertcase"     , ""     , "No", "Yes" } )
   aadd( aAct, { "MatchPairs"           , "Match Pairs"                  , "matchobj"       , ""     , "No", "Yes" } )
   aadd( aAct, { "ZoomIn"               , "ZoomIn"                       , "zoomin"         , ""     , "No", "Yes" } )
   aadd( aAct, { "ZoomOut"              , "ZoomOut"                      , "zoomout"        , ""     , "No", "Yes" } )
   //
   aadd( aAct, { "NewProject"           , "~Project"                     , "project"        , ""     , "No", "Yes" } )
   aadd( aAct, { "LoadProject"          , "Open Projec~t"                , ""               , ""     , "No", "Yes" } )
   aadd( aAct, { "SaveAs"               , "Save ~As"                     , "saveas"         , ""     , "No", "Yes" } )
   aadd( aAct, { "SaveAll"              , "Save A~ll"                    , "saveall"        , "Sh+^s", "No", "Yes" } )
   aadd( aAct, { "CloseAll"             , "Clos~e All"                   , "closeall"       , ""     , "No", "Yes" } )
   aadd( aAct, { "CloseOther"           , "Close Ot~hers"                , "closeexcept"    , ""     , "No", "Yes" } )
   aadd( aAct, { "Revert"               , "~Revert to Saved"             , ""               , "Sh+^R", "No", "Yes" } )
   aadd( aAct, { "ExportHTML"           , "~Export to HTML"              , "exporthtml"     , ""     , "No", "Yes" } )
   aadd( aAct, { "InsertDateTime"       , "~Date && Time"                , "insert-datetime", "Sh+F7", "No", "Yes" } )
   aadd( aAct, { "InsertRandomName"     , "~Random Function Name"        , "insert-procname", "Sh+F8", "No", "Yes" } )
   aadd( aAct, { "InsertExternalFile"   , "~External File at Cursor"     , "insert-external-file", "", "No", "Yes" } )
   aadd( aAct, { "InsertSeparator"      , "~Separator"                   , "insert-separator", "F7"   , "No", "Yes" } )
   aadd( aAct, { "switchReadOnly"       , "Switch Read~Only Mode"        , "readonly"       , ""     , "No", "Yes" } )
   aadd( aAct, { "Properties"           , "Properties"                   , ""               , ""     , "No", "Yes" } )
   aadd( aAct, { "ProjAddSource"        , "Add Source to Project"        , "projectadd"     , ""     , "No", "Yes" } )
   aadd( aAct, { "ProjRemSource"        , "Remove Source"                , "projectdel"     , ""     , "No", "Yes" } )
   aadd( aAct, { "ProjMainModule"       , "Select Main Module"           , "setmain"        , ""     , "No", "Yes" } )
   aadd( aAct, { "SelectProject"        , "Select Current Project"       , ""               , ""     , "No", "Yes" } )
   aadd( aAct, { "CloseProject"         , "Close Current Project"        , ""               , ""     , "No", "Yes" } )
   aadd( aAct, { "Build"                , "Build Project"                , "build"          , "^F9"  , "No", "Yes" } )
   aadd( aAct, { "BuildLaunch"          , "Build and Launch Project"     , "buildlaunch"    , "F9"   , "No", "Yes" } )
   aadd( aAct, { "ReBuild"              , "Rebuild Project"              , "rebuild"        , ""     , "No", "Yes" } )
   aadd( aAct, { "ReBuildLaunch"        , "Rebuild and Launch Project"   , "rebuildlaunch"  , ""     , "No", "Yes" } )
   aadd( aAct, { "CompileCurrent"       , "Compile Current Source"       , "compile"        , ""     , "No", "Yes" } )
   aadd( aAct, { "CompilePPO"           , "Compile Current Source to PPO", "ppo"            , ""     , "No", "Yes" } )
   aadd( aAct, { "LaunchProject"        , "Launch Project"               , ""               , "^F10" , "No", "Yes" } )
   aadd( aAct, { "ConfigureTools"       , "Configure Tools...*"          , ""               , ""     , "No", "Yes" } )
#ifdef __PLATFORM__WINDOWS
   aadd( aAct, { "CommandPrompt"        , "Command Prompt...*"           , ""               , ""     , "No", "Yes" } )
#else
   aadd( aAct, { "Terminal"             , "Terminal"                     , ""               , ""     , "No", "Yes" } )
#endif

   aadd( aAct, { "ManageThemes"         , "Manage Themes"                , ""               , ""     , "No", "Yes" } )
   aadd( aAct, { "DefaultTheme"         , "Set Default Theme"            , ""               , ""     , "No", "Yes" } )
   aadd( aAct, { "AboutIDE"             , "About Harbour IDE"            , "vr-16x16"       , ""     , "No", "Yes" } )
   aadd( aAct, { "AboutHarbour"         , "About Harbour"                , "hb-16x16"       , ""     , "No", "Yes" } )
   aadd( aAct, { "HarbourUsersList"     , "Harbour Users (Mailing Lists)", "list-users"     , ""     , "No", "Yes" } )
   aadd( aAct, { "HarbourDevList"       , "Harbour Developers (Mailing Lists)", "list-developers", "", "No", "Yes" } )

   aadd( aAct, { "BuildQt"              , "Build Project"                , "build"          , ""     , "No", "Yes" } )
   aadd( aAct, { "BuildLaunchQt"        , "Build and Launch"             , "buildlaunch"    , ""     , "No", "Yes" } )
   aadd( aAct, { "RebuildQt"            , "Rebuild Project"              , "rebuild"        , ""     , "No", "Yes" } )
   aadd( aAct, { "RebuildLaunchQt"      , "Rebuild and Launch"           , "rebuildlaunch"  , ""     , "No", "Yes" } )

   aadd( aAct, { "RemoveTabs"           , "Replace Tabs with Spaces"     , "tabstospaces"   , ""     , "No", "Yes" } )
   aadd( aAct, { "RemoveTrailingSpaces" , "Remove Trailing Spaces"       , "removetrailingspaces", "", "No", "Yes" } )
   aadd( aAct, { "FormatBraces"         , "Format Braces"                , "ormatbraces"    , ""     , "No", "Yes" } )

   aadd( aAct, { "StreamComment"        , "Stream Comment"               , "streamcomment"  , "Sh+^Q", "No", "Yes" } )
   aadd( aAct, { "BlockComment"         , "Block Comment"                , "blockcomment"   , "Sh+^/", "No", "Yes" } )
   aadd( aAct, { "BlockIndentR"         , "Indent Right"                 , "blockindentr"   , "^Tab" , "No", "Yes" } )
   aadd( aAct, { "BlockIndentL"         , "Indent Left"                  , "blockindentl"   , "Sh+^Tab", "No", "Yes" } )
   aadd( aAct, { "BlockDbl2Sgl"         , "Double Quotes to Single"      , "blockdbl2sgl"   , ""     , "No", "Yes" } )
   aadd( aAct, { "BlockSgl2Dbl"         , "Single Quotes to Double"      , "blocksgl2dbl"   , ""     , "No", "Yes" } )

   aadd( aAct, { "DuplicateLine"        , "Duplicate Current Line"       , "duplicateline"  , "^D"   , "No", "Yes" } )
   aadd( aAct, { "DeleteLine"           , "Delete Line"                  , "deleteline"     , "^Delete", "No", "Yes" } )
   aadd( aAct, { "MoveLineUp"           , "Move Line Up"                 , "movelineup"     , "Sh+^Up", "No", "Yes" } )
   aadd( aAct, { "MoveLineDown"         , "Move Line Down"               , "movelinedown"   , "Sh+^Down", "No", "Yes" } )

   aadd( aAct, { "Environments"         , "Environments..."              , "Envconfig"      , ""     , "No", "Yes" } )

   RETURN aAct

/*----------------------------------------------------------------------*/

METHOD IdeActions:buildToolBar()
   LOCAL oTBar, s
   LOCAL nSep := XBPTOOLBAR_BUTTON_SEPARATOR

   oTBar := XbpToolBar():new( ::oDlg )
   oTBar:imageWidth  := 22
   oTBar:imageHeight := 22
   oTBar:create( , , { 0, ::oDlg:currentSize()[ 2 ]-60 }, { ::oDlg:currentSize()[ 1 ], 60 } )
   oTBar:setStyleSheet( GetStyleSheet( "QToolBar" ) )

   oTBar:oWidget:setMaximumHeight( 28 )

   oTBar:buttonClick := {|oButton| ::oIde:execAction( oButton:key ) }

   oTBar:addItem( ::getAction( "TB_Exit"              ), , , , , , "Exit"              )
   oTBar:addItem( ::getAction( "Help"                 ), , , , , , "Help"              )
   oTBar:addItem( , , , , , nSep )
   oTBar:addItem( ::getAction( "TB_New"               ), , , , , , "New"               )
   oTBar:addItem( ::getAction( "TB_Open"              ), , , , , , "Open"              )
   oTBar:addItem( ::getAction( "TB_Save"              ), , , , , , "Save"              )
   oTBar:addItem( ::getAction( "TB_Close"             ), , , , , , "Close"             )
   oTBar:addItem( ::getAction( "TB_Print"             ), , , , , , "Print"             )
   oTBar:addItem( , , , , , nSep )
   oTBar:addItem( ::getAction( "TB_Compile"           ), , , , , , "Compile"           )
   oTBar:addItem( ::getAction( "TB_CompilePPO"        ), , , , , , "CompilePPO"        )
   oTBar:addItem( ::getAction( "TB_Build"             ), , , , , , "Build"             )
   oTBar:addItem( ::getAction( "TB_BuildLaunch"       ), , , , , , "BuildLaunch"       )
   oTBar:addItem( ::getAction( "TB_Rebuild"           ), , , , , , "Rebuild"           )
   oTBar:addItem( ::getAction( "TB_RebuildLaunch"     ), , , , , , "RebuildLaunch"     )
   oTBar:addItem( , , , , , nSep )
   oTBar:addItem( ::getAction( "TB_ToggleProjectTree" ), , , , , , "ToggleProjectTree" )
   oTBar:addItem( ::getAction( "TB_ToggleBuildInfo"   ), , , , , , "ToggleBuildInfo"   )
   oTBar:addItem( ::getAction( "TB_ToggleFuncList"    ), , , , , , "ToggleFuncList"    )
   oTBar:addItem( , , , , , nSep )
   oTBar:addItem( ::getAction( "TB_Undo"              ), , , , , , "Undo"              )
   oTBar:addItem( ::getAction( "TB_Redo"              ), , , , , , "Redo"              )
   oTBar:addItem( , , , , , nSep )
   oTBar:addItem( ::getAction( "TB_Cut"               ), , , , , , "Cut"               )
   oTBar:addItem( ::getAction( "TB_Copy"              ), , , , , , "Copy"              )
   oTBar:addItem( ::getAction( "TB_Paste"             ), , , , , , "Paste"             )
   oTBar:addItem( ::getAction( "TB_SelectAll"         ), , , , , , "SelectAll"         )
   oTBar:addItem( ::getAction( "TB_SelectionMode"     ), , , , , , "SelectionMode"     )
   oTBar:addItem( , , , , , nSep )
   oTBar:addItem( ::getAction( "TB_Find"              ), , , , , , "Find"              )
   oTBar:addItem( ::getAction( "TB_Search"            ), , , , , , "Search"            )
   oTBar:addItem( , , , , , nSep )
   oTBar:addItem( ::getAction( "TB_SetMark"           ), , , , , , "SetMark"           )
 * oTBar:addItem( ::getAction( "TB_GotoMark"          ), , , , , , "GotoMark"          )
   oTBar:addItem( ::getAction( "TB_Goto"              ), , , , , , "Goto"              )
   oTBar:addItem( , , , , , nSep )
   oTBar:addItem( ::getAction( "TB_ToUpper"           ), , , , , , "ToUpper"           )
   oTBar:addItem( ::getAction( "TB_ToLower"           ), , , , , , "ToLower"           )
   oTBar:addItem( ::getAction( "TB_Invert"            ), , , , , , "Invert"            )
   oTBar:addItem( ::getAction( "TB_MatchPairs"        ), , , , , , "MatchPairs"        )
   oTBar:addItem( , , , , , nSep )
   oTBar:addItem( ::getAction( "ZoomIn"               ), , , , , , "ZoomIn"            )
   oTBar:addItem( ::getAction( "ZoomOut"              ), , , , , , "ZoomOut"           )
   oTBar:addItem( , , , , , nSep )

   oTBar:oWidget:setAllowedAreas( Qt_TopToolBarArea )
   oTBar:oWidget:setMovable( .f. )
   oTBar:oWidget:setFloatable( .f. )

   /* ComboBox to Manage Views */
   ::oIde:qViewsCombo := QComboBox():new()
   oTBar:oWidget:addWidget( ::qViewsCombo )
   ::qViewsCombo:addItem( "New..." )
   ::qViewsCombo:addItem( " " )
   ::qViewsCombo:addItem( "Main"   )
   FOR EACH s IN ::aINI[ INI_VIEWS ]
      ::qViewsCombo:addItem( s )
   NEXT
   ::qViewsCombo:setCurrentIndex( -1 )
   ::connect( ::qViewsCombo, "currentIndexChanged(text)", {|p| ::oDK:setView( p ) } )

   RETURN Self

/*----------------------------------------------------------------------*/

METHOD IdeActions:buildMainMenu()
   LOCAL oMenuBar, oSubMenu, oSubMenu2, n, f
   LOCAL oIde := ::oIde

   oMenuBar := ::oDlg:MenuBar()
   oMenuBar:setStyleSheet( GetStyleSheet( "QMenuBar" ) )

   /*----------------------------------------------------------------------------*/
   /*                                   File                                     */
   /*----------------------------------------------------------------------------*/
   oSubMenu := XbpMenu():new( oMenuBar ):create()
   oSubMenu:title := "~File"

   oSubMenu2 := XbpMenu():new( oSubMenu ):create()

   oSubMenu2:addItem( { ::getAction( "New"        ), {|| oIde:execAction( "New"            ) } } )
   oSubMenu2:addItem( { ::getAction( "NewProject" ), {|| oIde:execAction( "NewProject"     ) } } )
   oMenuBar:addItem( { oSubMenu2,  _T( "~New" ) } )
   oMenuBar:aMenuItems[ oMenuBar:numItems(), 2 ]:setIcon( oIde:resPath + 'new.png' )

   oSubMenu:addItem( { ::getAction( "Open"        ), {|| oIde:execAction( "Open"           ) } } )
   oSubMenu:addItem( { ::getAction( "LoadProject" ), {|| oIde:execAction( "LoadProject"    ) } } )

   hbide_menuAddSep( oSubMenu )

   oSubMenu2 := XbpMenu():new( oSubMenu ):create()
   oSubMenu2:itemSelected := {| nIndex, cFile | cFile := oIde:aIni[ INI_RECENTFILES, nIndex ], ;
                                                oIde:oEM:editSource( cFile ) }
   IF !empty( oIde:aIni[ INI_RECENTFILES ] )
      FOR n := 1 TO Len( oIde:aIni[ INI_RECENTFILES ] )
         f := hbide_pathNormalized( oIde:aIni[ INI_RECENTFILES, n ], .F. )
         oSubMenu2:addItem( { _T( '~' + hb_NumToHex(n) + '. ' + f ), nil } )
         IF !hb_FileExists( f )
            oSubMenu2:disableItem( n )
         ENDIF
      NEXT
   ELSE
      oSubMenu2:addItem( { _T( "** No recent files found **" )   , nil } )
      oSubMenu2:disableItem( 1 )
   ENDIF
   oMenuBar:addItem( { oSubMenu2,  _T( "Recent Files" ) } )

   oSubMenu2 := XbpMenu():new( oSubMenu ):create()
   oSubMenu2:itemSelected := {| nIndex, cFile | cFile := oIde:aIni[ INI_RECENTPROJECTS, nIndex ], ;
                                                ::oPM:loadProperties( cFile, .F., .F., .T. ) }
   IF !empty( oIde:aIni[ INI_RECENTPROJECTS ] )
      FOR n := 1 TO Len( oIde:aIni[ INI_RECENTPROJECTS ] )
          f := hbide_pathNormalized( oIde:aIni[ INI_RECENTPROJECTS, n ], .F. )
          oSubMenu2:addItem( { _T( '~' + hb_NumToHex( n ) + '. ' + f )   , nil } )
          IF !hb_FileExists( f )
             oSubMenu2:disableItem( n )
          ENDIF
      NEXT
   ELSE
      oSubMenu2:addItem( { _T( "** No recent projects found **" )   , nil } )
      oSubMenu2:disableItem( 1 )
   ENDIF
   oMenuBar:addItem( { oSubMenu2,  _T( "Recent Projects" ) } )

   hbide_menuAddSep( oSubMenu )

   oSubMenu:addItem( { ::getAction( "Save"                ), {|| oIde:execAction( "Save"           ) } } )
   oSubMenu:addItem( { ::getAction( "SaveAs"              ), {|| oIde:execAction( "SaveAs"         ) } } )
   oSubMenu:addItem( { ::getAction( "SaveAll"             ), {|| oIde:execAction( "SaveAll"        ) } } )
   oSubMenu:addItem( { ::getAction( "Close"               ), {|| oIde:execAction( "Close"          ) } } )
   oSubMenu:addItem( { ::getAction( "CloseAll"            ), {|| oIde:execAction( "CloseAll"       ) } } )
   oSubMenu:addItem( { ::getAction( "CloseOther"          ), {|| oIde:execAction( "CloseOther"     ) } } )
   oSubMenu:addItem( { ::getAction( "Revert"              ), {|| oIde:execAction( "Revert"         ) } } )
   hbide_menuAddSep( oSubMenu )

   oSubMenu:addItem( { ::getAction( "ExportHTML"          ), {|| oIde:execAction( "ExportHTML"     ) } } )
   oSubMenu:addItem( { ::getAction( "Print"               ), {|| oIde:execAction( "Print"          ) } } )
   hbide_menuAddSep( oSubMenu )
   oSubMenu:addItem( { ::getAction( "SaveExit"            ), {|| oIde:execAction( "SaveExit"       ) } } )
   oSubMenu:addItem( { ::getAction( "Exit"                ), {|| oIde:execAction( "Exit"           ) } } )
   oMenuBar:addItem( { oSubMenu, NIL } )

   /*----------------------------------------------------------------------------*/
   /*                                   Edit                                     */
   /*----------------------------------------------------------------------------*/
   oSubMenu := XbpMenu():new( oMenuBar ):create()
   oSubMenu:title := "~Edit"
   oSubMenu:addItem( { ::getAction( "Undo"                ), {|| oIde:execAction( "Undo"           ) } } )
   oSubMenu:addItem( { ::getAction( "Redo"                ), {|| oIde:execAction( "Redo"           ) } } )
   hbide_menuAddSep( oSubMenu )
   oSubMenu:addItem( { ::getAction( "Cut"                 ), {|| oIde:execAction( "Cut"            ) } } )
   oSubMenu:addItem( { ::getAction( "Copy"                ), {|| oIde:execAction( "Copy"           ) } } )
   oSubMenu:addItem( { ::getAction( "Paste"               ), {|| oIde:execAction( "Paste"          ) } } )
   oSubMenu:addItem( { ::getAction( "SelectAll"           ), {|| oIde:execAction( "SelectAll"      ) } } )
   hbide_menuAddSep( oSubMenu )
   oSubMenu:addItem( { ::getAction( "Find"                ), {|| oIde:execAction( "Find"           ) } } )
   oSubMenu:addItem( { ::getAction( "FindInFiles"         ), {|| oIde:execAction( "FindInFiles"    ) } } )
   oSubMenu:addItem( { ::getAction( "Goto"                ), {|| oIde:execAction( "Goto"           ) } } )
   hbide_menuAddSep( oSubMenu )
   //
   oSubMenu2 := XbpMenu():new( oSubMenu ):create()
   oSubMenu2:addItem( { ::getAction( "DuplicateLine"       ), {|| oIde:execAction( "DuplicateLine"  ) } } )
   oSubMenu2:addItem( { ::getAction( "DeleteLine"          ), {|| oIde:execAction( "DeleteLine"     ) } } )
   oSubMenu2:addItem( { ::getAction( "MoveLineUp"          ), {|| oIde:execAction( "MoveLineUp"     ) } } )
   oSubMenu2:addItem( { ::getAction( "MoveLineDown"        ), {|| oIde:execAction( "MoveLineDown"   ) } } )
   oMenuBar:addItem( { oSubMenu2,  _T( "~Line..." ) } )
   //
   oSubMenu2 := XbpMenu():new( oSubMenu ):create()
   oSubMenu2:addItem( { ::getAction( "StreamComment"       ), {|| oIde:execAction( "StreamComment"  ) } } )
   oSubMenu2:addItem( { ::getAction( "BlockComment"        ), {|| oIde:execAction( "BlockComment"   ) } } )
   oSubMenu2:addItem( { ::getAction( "BlockIndentR"        ), {|| oIde:execAction( "BlockIndentR"   ) } } )
   oSubMenu2:addItem( { ::getAction( "BlockIndentL"        ), {|| oIde:execAction( "BlockIndentL"   ) } } )
   hbide_menuAddSep( oSubMenu2 )
   oSubMenu2:addItem( { ::getAction( "BlockSgl2Dbl"        ), {|| oIde:execAction( "BlockSgl2Dbl"   ) } } )
   oSubMenu2:addItem( { ::getAction( "BlockDbl2Sgl"        ), {|| oIde:execAction( "BlockDbl2Sgl"   ) } } )
   oMenuBar:addItem( { oSubMenu2,  _T( "~Block..." ) } )

   hbide_menuAddSep( oSubMenu )
   oSubMenu2 := XbpMenu():new( oSubMenu ):create()
   oSubMenu2:addItem( { ::getAction( "InsertSeparator"    ), {|| oIde:execAction( "InsertSeparator"    ) } } )
   hbide_menuAddSep( oSubMenu )
   oSubMenu2:addItem( { ::getAction( "InsertDateTime"     ), {|| oIde:execAction( "InsertDateTime"     ) } } )
   oSubMenu2:addItem( { ::getAction( "InsertRandomName"   ), {|| oIde:execAction( "InsertRandomName"   ) } } )
   oSubMenu2:addItem( { ::getAction( "InsertExternalFile" ), {|| oIde:execAction( "InsertExternalFile" ) } } )
   oMenuBar:addItem( { oSubMenu2,  _T( "~Insert" ) } )

   oSubMenu2 := XbpMenu():new( oSubMenu ):create()
   oSubMenu2:addItem( { ::getAction( "RemoveTabs"         ), {|| oIde:execAction( "RemoveTabs"         ) } } )
   hbide_menuAddSep( oSubMenu )
   oSubMenu2:addItem( { ::getAction( "RemoveTrailingSpaces"), {|| oIde:execAction( "RemoveTrailingSpaces" ) } } )
   oSubMenu2:addItem( { ::getAction( "FormatBraces"       ), {|| oIde:execAction( "FormatBraces"       ) } } )
   oMenuBar:addItem( { oSubMenu2,  _T( "~Format" ) } )

   hbide_menuAddSep( oSubMenu )
   oSubMenu:addItem( { ::getAction( "switchReadOnly"      ), {|| oIde:execAction( "switchReadOnly"     ) } } )
   oMenuBar:addItem( { oSubMenu, NIL } )

   /*----------------------------------------------------------------------------*/
   /*                                   Project                                  */
   /*----------------------------------------------------------------------------*/
   oSubMenu := XbpMenu():new( oMenuBar ):create()
   oSubMenu:title := "~Project"
   oSubMenu:addItem( { ::getAction( "Properties"          ), {|| oIde:execAction( "Properties"     ) } } )
   hbide_menuAddSep( oSubMenu )
   oSubMenu:addItem( { ::getAction( "ProjAddSource"       ), {|| oIde:execAction( "ProjAddSource"  ) } } )
   oSubMenu:addItem( { ::getAction( "ProjRemSource"       ), {|| oIde:execAction( "ProjRemSource"  ) } } )
   oSubMenu:addItem( { ::getAction( "ProjMainModule"      ), {|| oIde:execAction( "ProjMainModule" ) } } )
   oSubMenu:disableItem( oSubMenu:numItems )
   hbide_menuAddSep( oSubMenu )
   oSubMenu:addItem( { ::getAction( "SelectProject"       ), {|| oIde:execAction( "SelectProject"  ) } } )
   oSubMenu:addItem( { ::getAction( "CloseProject"        ), {|| oIde:execAction( "CloseProject"   ) } } )
   hbide_menuAddSep( oSubMenu )
   oSubMenu:addItem( { ::getAction( "Environments"        ), {|| oIde:execAction( "Environments"   ) } } )
   oMenuBar:addItem( { oSubMenu, NIL } )

   /*----------------------------------------------------------------------------*/
   /*                                   Build                                    */
   /*----------------------------------------------------------------------------*/
   oSubMenu := XbpMenu():new( oMenuBar ):create()
   oSubMenu:title := "~Build"
   oSubMenu:addItem( { ::getAction( "Build"               ), {|| oIde:execAction( "Build"              ) } } )
   oSubMenu:addItem( { ::getAction( "BuildLaunch"         ), {|| oIde:execAction( "BuildLaunch"        ) } } )
   oSubMenu:addItem( { ::getAction( "Rebuild"             ), {|| oIde:execAction( "Rebuild"            ) } } )
   oSubMenu:addItem( { ::getAction( "RebuildLaunch"       ), {|| oIde:execAction( "RebuildLaunch"      ) } } )
   hbide_menuAddSep( oSubMenu )
   oSubMenu:addItem( { ::getAction( "SaveCompileCurrent"  ), {|| oIde:execAction( "SaveCompileCurrent" ) } } )
   oSubMenu:addItem( { ::getAction( "CompilePPO"          ), {|| oIde:execAction( "CompilePPO"         ) } } )
   hbide_menuAddSep( oSubMenu )
   oSubMenu:addItem( { ::getAction( "LaunchProject"       ), {|| oIde:execAction( "LaunchProject"      ) } } )
   oMenuBar:addItem( { oSubMenu, NIL } )

   /*----------------------------------------------------------------------------*/
   /*                                   Tools                                    */
   /*----------------------------------------------------------------------------*/
   oSubMenu := XbpMenu():new( oMenuBar ):create()
   oSubMenu:title := "~Tools"
   oSubMenu:addItem( { ::getAction( "ConfigureTools"      ), {|| oIde:execAction( "ConfigureTools"     ) } } )
   hbide_menuAddSep( oSubMenu )
#ifdef __PLATFORM__WINDOWS
   oSubMenu:addItem( { ::getAction( "CommandPrompt"       ), {|| oIde:execAction( "CommandPrompt"      ) } } )
#else
   oSubMenu:addItem( { ::getAction( "Terminal"            ), {|| oIde:execAction( "Terminal"           ) } } )
#endif
   oMenuBar:addItem( { oSubMenu, NIL } )

   /*----------------------------------------------------------------------------*/
   /*                                   Options                                  */
   /*----------------------------------------------------------------------------*/
   oSubMenu := XbpMenu():new( oMenuBar ):create()
   oSubMenu:title := "~Setup"
   oSubMenu:addItem( { ::getAction( "ManageThemes"        ), {|| oIde:oThemes:fetch()              } } )
   oSubMenu:addItem( { ::getAction( "DefaultTheme"        ), {|| oIde:oThemes:setWrkTheme()        } } )
   hbide_menuAddSep( oSubMenu )
   oSubMenu2 := hbide_buildCodecMenu( oIde, oSubMenu )
   oSubMenu2:title := "~Codecs"
   oSubMenu:addItem( { oSubMenu2, NIL } )
   hbide_menuAddSep( oSubMenu )
   oMenuBar:addItem( { oSubMenu, NIL } )
   oSubMenu:oWidget:addAction_4( ::oSkeltn:oWidget:toggleViewAction() )

   /*----------------------------------------------------------------------------*/
   /*                                   Docks                                    */
   /*----------------------------------------------------------------------------*/
   oSubMenu := XbpMenu():new( oMenuBar ):create()
   oSubMenu:title := "~Docks"
   oMenuBar:addItem( { oSubMenu, NIL } )

   oSubMenu:oWidget:addAction_4( ::oDockED:oWidget:toggleViewAction() )
   oSubMenu:oWidget:addAction_4( ::oDockPT:oWidget:toggleViewAction() )

   oSubMenu:oWidget:addAction_4( ::oDockR:oWidget:toggleViewAction()  )
   oSubMenu:oWidget:addAction_4( ::oHelp:oWidget:toggleViewAction()   )
   oSubMenu:oWidget:addAction_4( ::oSkeltn:oWidget:toggleViewAction() )
   oSubMenu:oWidget:addAction_4( ::oThemesDock:oWidget:toggleViewAction() )
   oSubMenu:oWidget:addAction_4( ::oEnvironDock:oWidget:toggleViewAction() )
   oSubMenu:oWidget:addAction_4( ::oPropertiesDock:oWidget:toggleViewAction() )
   oSubMenu:oWidget:addAction_4( ::oFindDock:oWidget:toggleViewAction() )

   oSubMenu:oWidget:addAction_4( ::oDockB2:oWidget:toggleViewAction() )
   oSubMenu:oWidget:addAction_4( ::oDockB1:oWidget:toggleViewAction() )
   oSubMenu:oWidget:addAction_4( ::oDockB:oWidget:toggleViewAction()  )

   /*----------------------------------------------------------------------------*/
   /*                                   Help                                     */
   /*----------------------------------------------------------------------------*/
   oSubMenu := XbpMenu():new( oMenuBar ):create()
   oSubMenu:title := "~Help"
   oSubMenu:addItem( { ::getAction( "AboutIDE"            ), {|| hbide_help( 1 ) } } )
   hbide_menuAddSep( oSubMenu )
   oSubMenu:addItem( { ::getAction( "AboutHarbour"        ), {|| hbide_help( 4 ) } } )
   hbide_menuAddSep( oSubMenu )
   oSubMenu:addItem( { ::getAction( "HarbourUsersList"    ), {|| hbide_help( 3 ) } } )
   oSubMenu:addItem( { ::getAction( "HarbourDevList"      ), {|| hbide_help( 2 ) } } )
   oMenuBar:addItem( { oSubMenu, NIL } )

   RETURN Self

/*----------------------------------------------------------------------*/

STATIC FUNCTION hbide_buildCodecMenu( oIde, oMenu )
   LOCAL oSubMenu, oSub1

   oSubMenu := XbpMenu():new( oMenu ):create()

   oSubMenu:addItem( { "Apple Roman "         , {|| oIde:setCodec( "Apple Roman"         ) } } )
   oSubMenu:addItem( { "Big5        "         , {|| oIde:setCodec( "Big5"                ) } } )
   oSubMenu:addItem( { "Big5-HKSCS  "         , {|| oIde:setCodec( "Big5-HKSCS"          ) } } )
   oSubMenu:addItem( { "CP949       "         , {|| oIde:setCodec( "CP949"               ) } } )
   oSubMenu:addItem( { "EUC-JP      "         , {|| oIde:setCodec( "EUC-JP"              ) } } )
   oSubMenu:addItem( { "EUC-KR      "         , {|| oIde:setCodec( "EUC-KR"              ) } } )
   oSubMenu:addItem( { "GB18030-0   "         , {|| oIde:setCodec( "GB18030-0"           ) } } )
   oSubMenu:addItem( { "IBM 850     "         , {|| oIde:setCodec( "IBM 850"             ) } } )
   oSubMenu:addItem( { "IBM 866     "         , {|| oIde:setCodec( "IBM 866"             ) } } )
   oSubMenu:addItem( { "IBM 874     "         , {|| oIde:setCodec( "IBM 874"             ) } } )
   oSubMenu:addItem( { "ISO 2022-JP "         , {|| oIde:setCodec( "ISO 2022-JP"         ) } } )

   oSub1 := XbpMenu():new( oSubMenu ):create()
   oSub1:title := "ISO 8859-1 to 10"
   oSub1:addItem( { "-1"                      , {|| oIde:setCodec( "ISO 8859-1"          ) } } )
   oSub1:addItem( { "-2"                      , {|| oIde:setCodec( "ISO 8859-2"          ) } } )
   oSub1:addItem( { "-3"                      , {|| oIde:setCodec( "ISO 8859-3"          ) } } )
   oSub1:addItem( { "-4"                      , {|| oIde:setCodec( "ISO 8859-4"          ) } } )
   oSub1:addItem( { "-5"                      , {|| oIde:setCodec( "ISO 8859-5"          ) } } )
   oSub1:addItem( { "-6"                      , {|| oIde:setCodec( "ISO 8859-6"          ) } } )
   oSub1:addItem( { "-7"                      , {|| oIde:setCodec( "ISO 8859-7"          ) } } )
   oSub1:addItem( { "-8"                      , {|| oIde:setCodec( "ISO 8859-8"          ) } } )
   oSub1:addItem( { "-9"                      , {|| oIde:setCodec( "ISO 8859-9"          ) } } )
   oSub1:addItem( { "-10"                     , {|| oIde:setCodec( "ISO 8859-10"         ) } } )
   oSubMenu:addItem( { oSub1, NIL } )

   oSub1 := XbpMenu():new( oSubMenu ):create()
   oSub1:title := "ISO 8859-13 to 16"
   oSub1:addItem( { "-13"                     , {|| oIde:setCodec( "ISO 8859-13"         ) } } )
   oSub1:addItem( { "-14"                     , {|| oIde:setCodec( "ISO 8859-14"         ) } } )
   oSub1:addItem( { "-15"                     , {|| oIde:setCodec( "ISO 8859-15"         ) } } )
   oSub1:addItem( { "-16"                     , {|| oIde:setCodec( "ISO 8859-16"         ) } } )
   oSubMenu:addItem( { oSub1, NIL } )

   oSub1 := XbpMenu():new( oSubMenu ):create()
   oSub1:title := "Iscii"
   oSub1:addItem( { "-Bng"                    , {|| oIde:setCodec( "Iscii-Bng"           ) } } )
   oSub1:addItem( { "-Dev"                    , {|| oIde:setCodec( "Iscii-Dev"           ) } } )
   oSub1:addItem( { "-Gjr"                    , {|| oIde:setCodec( "Iscii-Gjr"           ) } } )
   oSub1:addItem( { "-Knd"                    , {|| oIde:setCodec( "Iscii-Knd"           ) } } )
   oSub1:addItem( { "-Mlm"                    , {|| oIde:setCodec( "Iscii-Mlm"           ) } } )
   oSub1:addItem( { "-Ori"                    , {|| oIde:setCodec( "Iscii-Ori"           ) } } )
   oSub1:addItem( { "-Pnj"                    , {|| oIde:setCodec( "Iscii-Pnj"           ) } } )
   oSub1:addItem( { "-Tlg"                    , {|| oIde:setCodec( "Iscii-Tlg"           ) } } )
   oSub1:addItem( { "-Tml"                    , {|| oIde:setCodec( "Iscii-Tml"           ) } } )
   oSubMenu:addItem( { oSub1, NIL } )

   oSubMenu:addItem( { "JIS X 0201  "         , {|| oIde:setCodec( "JIS X 0201"          ) } } )
   oSubMenu:addItem( { "JIS X 0208  "         , {|| oIde:setCodec( "JIS X 0208"          ) } } )
   oSubMenu:addItem( { "KOI8-R      "         , {|| oIde:setCodec( "KOI8-R"              ) } } )
   oSubMenu:addItem( { "KOI8-U      "         , {|| oIde:setCodec( "KOI8-U"              ) } } )
   oSubMenu:addItem( { "MuleLao-1   "         , {|| oIde:setCodec( "MuleLao-1"           ) } } )
   oSubMenu:addItem( { "ROMAN8      "         , {|| oIde:setCodec( "ROMAN8"              ) } } )
   oSubMenu:addItem( { "Shift-JIS   "         , {|| oIde:setCodec( "Shift-JIS"           ) } } )
   oSubMenu:addItem( { "TIS-620     "         , {|| oIde:setCodec( "TIS-620"             ) } } )
   oSubMenu:addItem( { "TSCII       "         , {|| oIde:setCodec( "TSCII"               ) } } )
   oSubMenu:addItem( { "UTF-8       "         , {|| oIde:setCodec( "UTF-8"               ) } } )
   oSubMenu:addItem( { "UTF-16      "         , {|| oIde:setCodec( "UTF-16"              ) } } )
   oSubMenu:addItem( { "UTF-16BE    "         , {|| oIde:setCodec( "UTF-16BE"            ) } } )
   oSubMenu:addItem( { "UTF-16LE    "         , {|| oIde:setCodec( "UTF-16LE"            ) } } )
   oSubMenu:addItem( { "UTF-32      "         , {|| oIde:setCodec( "UTF-32"              ) } } )
   oSubMenu:addItem( { "UTF-32BE    "         , {|| oIde:setCodec( "UTF-32BE"            ) } } )
   oSubMenu:addItem( { "UTF-32LE    "         , {|| oIde:setCodec( "UTF-32LE"            ) } } )

   oSub1 := XbpMenu():new( oSubMenu ):create()
   oSub1:title := "Windows-1250 to 1258"
   oSub1:addItem( { "-1250"                   , {|| oIde:setCodec( "Windows-1250"        ) } } )
   oSub1:addItem( { "-1251"                   , {|| oIde:setCodec( "Windows-1251"        ) } } )
   oSub1:addItem( { "-1252"                   , {|| oIde:setCodec( "Windows-1252"        ) } } )
   oSub1:addItem( { "-1253"                   , {|| oIde:setCodec( "Windows-1253"        ) } } )
   oSub1:addItem( { "-1254"                   , {|| oIde:setCodec( "Windows-1254"        ) } } )
   oSub1:addItem( { "-1255"                   , {|| oIde:setCodec( "Windows-1255"        ) } } )
   oSub1:addItem( { "-1256"                   , {|| oIde:setCodec( "Windows-1256"        ) } } )
   oSub1:addItem( { "-1257"                   , {|| oIde:setCodec( "Windows-1257"        ) } } )
   oSub1:addItem( { "-1258"                   , {|| oIde:setCodec( "Windows-1258"        ) } } )
   oSubMenu:addItem( { oSub1, NIL } )

   oSubMenu:addItem( { "WINSAMI2    "         , {|| oIde:setCodec( "WINSAMI2"            ) } } )

   RETURN oSubMenu

/*----------------------------------------------------------------------*/
/*
 * Normalizes a caption for an menu item with shortcut (or not).
 * TODO: add support for translation of menu items AND support changing shortcuts
 *       loading from a text file for customing hotkeys AND icons. (vailtom)
 * 27/12/2009 - 16:05:32 - vailtom
 */
STATIC FUNCTION mnuNormalizeItem( cCaption )
   LOCAL cKey, cIco, p

   /* Retrieve and update the ICON name for this menu item */
   IF ( ( p := Rat( '|', cCaption ) ) != 00 )
      cIco := Substr( cCaption, p + 1 )
      cIco := alltrim( cIco )

      cCaption := Substr( cCaption, 1, p - 1 )
      cCaption := Alltrim( cCaption )

    * cIco := s_resPath + Alltrim( cIco ) ---> "s_resPath" is need here!
      IF !Empty( cIco )
         cIco := StrTran( cIco, '/', hb_OsPathSeparator() )
         cIco := StrTran( cIco, '\', hb_OsPathSeparator() )

         IF !( hb_OsPathSeparator() $ cIco )
            cIco := hb_DirBase() + "resources" + hb_OsPathSeparator() + cIco + "|"
         ELSE
            cIco := cIco + "|"
         Endif
      Endif
   ELSE
      cIco := ''
   ENDIF

   /* Update the key shortcut for this menu item */
   IF ( ( p := Rat( ',', cCaption ) ) != 00 )
      cKey     := Substr( cCaption, p + 1 )
      cCaption := Substr( cCaption, 1, p - 1 )
      cCaption := alltrim( cCaption )

      cKey := alltrim( cKey )
      cKey := StrTran( cKey, '^', 'Ctrl+' )
      cKey := StrTran( cKey, 'Sh+', 'Shift+' )

      IF !Empty( cKey )
         cKey := Chr( K_TAB ) + cKey
      End
   ELSE
      cKey := ''
   ENDIF

   RETURN ( cIco + cCaption + cKey )

/*----------------------------------------------------------------------*/
/*
 * Add a file name to MRU menu item.
 * 02/01/2010 - 23:23:22 - vailtom
 */
FUNCTION hbide_mnuAddFileToMRU( oIde, cFileName, nType )
   LOCAL nPos

   IF nType != INI_RECENTPROJECTS .AND. nType != INI_RECENTFILES
      RETURN nil
   ENDIF

   cFileName := hbide_pathNormalized( cFileName )

   IF ( nPos := aScan( oIde:aIni[ nType ], {|f| hbide_pathNormalized( f ) == cFileName } ) ) > 0
      hb_aDel( oIde:aIni[ nType ], nPos, .T. )
   ENDIF

   ASize( oIde:aIni[ nType ], len( oIde:aIni[ nType ] ) + 1 )
   AIns( oIde:aIni[ nType ], 1 )

   oIde:aIni[ nType,1 ] := cFileName

   IF Len( oIde:aIni[ nType ] ) > 25
      aSize( oIde:aIni[ nType ], 25 )
   ENDIF

   IF nPos == 0
      hbide_mnuUpdateMRUpopup( oIde, nType )
   ENDIF
   RETURN nil

/*----------------------------------------------------------------------*/
/*
 * 02/01/2010 - 22:44:19
 */
#define QMF_POPUP  1

STATIC FUNCTION hbide_mnuUpdateMRUpopup( oIde, nType )
   LOCAL oMenuBar, oItem, cFindStr, nPos, n, c

   IF Empty( oIde:oDlg )
      RETURN NIL
   ENDIF

   oMenuBar := oIde:oDlg:MenuBar()
   nPos     := 0
   cFindStr := iif( nType == INI_RECENTFILES, 'RECENT FILES', 'RECENT PROJECTS' )

   FOR n := 1 TO oMenuBar:numItems()
       IF oMenuBar:aMenuItems[ n, 1 ] != QMF_POPUP
          LOOP
       ENDIF

       oItem := oMenuBar:aMenuItems[ n ]
       c := Upper( oItem[ 3 ] )
       c := StrTran( c, '~', '' )
       c := StrTran( c, '&', '' )

       IF cFindStr == alltrim( c )
          nPos := n
          EXIT
       ENDIF
   NEXT

   IF nPos == 0
      RETURN nil
   ENDIF

   oItem[ 4 ]:delAllItems()

   IF !empty( oIde:aIni[ nType ] )
      FOR n := 1 TO Len( oIde:aIni[ nType ] )
          c := hbide_pathNormalized( oIde:aIni[ nType, n ], .F. )

          oItem[ 4 ]:addItem( { _T( '~' + hb_NumToHex( n ) + '. ' + c ), nil } )

          IF !hb_FileExists( c )
             oItem[ 4 ]:disableItem( n )
          ENDIF
      NEXT
   ELSE
      IF nType == INI_RECENTFILES
         oItem[ 4 ]:addAction( "** No recent files found **" )
      ELSE
         oItem[ 4 ]:addAction( "** No recent projects found **" )
      ENDIF
      oItem[ 4 ]:disableItem( 1 )
   ENDIF

   RETURN nil

/*----------------------------------------------------------------------*/
/*
 * Find a menu item with same caption as passed on argument.
 * 03/01/2010 - 13:12:42
 */
FUNCTION hbide_mnuFindItem( oIde, cCaption )
   LOCAL oMenuBar, oItem, n, c

   IF Empty( oIde:oDlg )
      RETURN nil
   ENDIF

   oMenuBar := oIde:oDlg:MenuBar()
   cCaption := Alltrim( Upper( cCaption ) )

   FOR n := 1 TO oMenuBar:numItems()

      oItem := oMenuBar:aMenuItems[ n ]
      c := Upper( oItem[ 3 ] )
      c := StrTran( c, '~', '' )
      c := StrTran( c, '&', '' )

      IF cCaption == alltrim( c )
         RETURN oItem
      ENDIF
   NEXT

   RETURN nil

/*----------------------------------------------------------------------*/

