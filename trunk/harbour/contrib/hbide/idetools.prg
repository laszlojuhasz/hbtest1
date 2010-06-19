/*
 * $Id$
 */

/*
 * Harbour Project source code:
 *
 * Copyright 2010 Pritpal Bedi <bedipritpal@hotmail.com>
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
 *                 Pritpal Bedi <bedipritpal@hotmail.com>
 *                               20Mar2010
 */
/*----------------------------------------------------------------------*/
/*----------------------------------------------------------------------*/
/*----------------------------------------------------------------------*/

#include "hbide.ch"
#include "common.ch"
#include "hbclass.ch"
#include "hbqt.ch"

/*----------------------------------------------------------------------*/

CLASS IdeToolsManager INHERIT IdeObject

   DATA   aAct                                    INIT   {}
   DATA   qToolsMenu
   DATA   qToolsButton
   DATA   aPanelsAct                              INIT   {}
   DATA   qPanelsButton
   DATA   qPanelsMenu
   DATA   oProcess
   DATA   lExecuting                              INIT   .f.
   DATA   aHdr                                    INIT   {}
   DATA   aBtns                                   INIT   {}
   DATA   aToolbars                               INIT   { NIL,NIL,NIL,NIL,NIL }

   ACCESS aTools                                  INLINE ::oINI:aTools
   ACCESS aUserToolBars                           INLINE ::oINI:aUserToolbars

   METHOD new( oIde )
   METHOD create( oIde )
   METHOD destroy()
   METHOD show()
   METHOD execEvent( cMode, p )
   METHOD clearList()
   METHOD populateList( aList )
   METHOD execTool( ... )
   METHOD execToolByParams( cCmd, cParams, cStartIn, lCapture, lOpen )
   METHOD ini2controls( nIndex )
   METHOD controls2ini( nIndex )
   METHOD buildToolsButton()
   METHOD buildPanelsButton()
   METHOD addPanelsMenu( cPrompt )
   METHOD showOutput( cOut, mp2, oHbp )
   METHOD finished( nEC, nES, oHbp )
   METHOD parseParams( cP )
   METHOD macro2value( cMacro )
   METHOD ini2toolbarControls( nIndex, nMode )
   METHOD populateButtonsTable( nIndex )
   METHOD buildUserToolbars()

   ENDCLASS

/*----------------------------------------------------------------------*/

METHOD IdeToolsManager:new( oIde )

   ::oIde := oIde

   RETURN Self

/*----------------------------------------------------------------------*/

METHOD IdeToolsManager:create( oIde )
   LOCAL oAct

   DEFAULT oIde TO ::oIde
   ::oIde := oIde

   IF !empty( oAct := ::oAC:getAction( "TB_Tools" ) )
      oAct:setMenu( QMenu():new() )
   ENDIF

   IF empty( ::oINI:aUserToolbars )
      asize( ::oINI:aUserToolbars, 5 )
      DEFAULT ::oINI:aUserToolbars[ 1 ] TO { "","YES","","","YES","YES","YES" }
      DEFAULT ::oINI:aUserToolbars[ 2 ] TO { "","YES","","","YES","YES","YES" }
      DEFAULT ::oINI:aUserToolbars[ 3 ] TO { "","YES","","","YES","YES","YES" }
      DEFAULT ::oINI:aUserToolbars[ 4 ] TO { "","YES","","","YES","YES","YES" }
      DEFAULT ::oINI:aUserToolbars[ 5 ] TO { "","YES","","","YES","YES","YES" }
   ENDIF

   RETURN Self

/*----------------------------------------------------------------------*/

METHOD IdeToolsManager:destroy()
   LOCAL qAct

   IF !empty( ::oUI )
      FOR EACH qAct IN ::aAct
         ::disconnect( qAct, "triggered(bool)" )
         qAct := NIL
      NEXT
      FOR EACH qAct IN ::aPanelsAct
         ::disconnect( qAct, "triggered(bool)" )
         qAct := NIL
      NEXT
      ::disconnect( ::qToolsButton, "clicked()" )
      ::qToolsButton := NIL
      ::clearList()

      ::disconnect( ::qPanelsButton, "clicked()" )

      ::disconnect( ::oUI:q_buttonAdd           , "clicked()" )
      ::disconnect( ::oUI:q_buttonDelete        , "clicked()" )
      ::disconnect( ::oUI:q_buttonUp            , "clicked()" )
      ::disconnect( ::oUI:q_buttonDown          , "clicked()" )
      ::disconnect( ::oUI:q_buttonExec          , "clicked()" )
      ::disconnect( ::oUI:q_buttonBrowse        , "clicked()" )
      ::disconnect( ::oUI:q_buttonUpdate        , "clicked()" )
      ::disconnect( ::oUI:q_buttonClose         , "clicked()" )
      ::disconnect( ::oUI:q_buttonSetImage      , "clicked()" )
      ::disconnect( ::oUI:q_buttonUserToolbarUpd, "clicked()" )
      ::disconnect( ::oUI:q_comboToolbarAsgnd   , "currentIndexChanged(int)" )
      ::disconnect( ::oUI:q_listToolbars        , "itemSelectionChanged()"   )
      ::disconnect( ::oUI:q_listNames           , "itemSelectionChanged()"   )

      ::oUI:destroy()
   ENDIF

   RETURN Self

/*----------------------------------------------------------------------*/

METHOD IdeToolsManager:show()
   LOCAL oTbl, hdr_, n, qItm

   IF empty( ::oUI )
      ::oUI := HbQtUI():new( hbide_uic( "toolsutilities" ), ::oDlg:oWidget ):build()
      ::oUI:setWindowFlags( Qt_Sheet )

      ::oUI:setWindowIcon( hbide_image( "hbide" ) )
      ::oUI:setMaximumWidth( ::oUI:width() )
      ::oUI:setMinimumWidth( ::oUI:width() )
      ::oUI:setMaximumHeight( ::oUI:height() )
      ::oUI:setMinimumHeight( ::oUI:height() )

      ::connect( ::oUI:q_buttonAdd   , "clicked()", {|| ::execEvent( "buttonAdd_clicked"    ) } )
      ::connect( ::oUI:q_buttonDelete, "clicked()", {|| ::execEvent( "buttonDelete_clicked" ) } )
      ::connect( ::oUI:q_buttonUp    , "clicked()", {|| ::execEvent( "buttonUp_clicked"     ) } )
      ::connect( ::oUI:q_buttonDown  , "clicked()", {|| ::execEvent( "buttonDown_clicked"   ) } )
      ::connect( ::oUI:q_buttonExec  , "clicked()", {|| ::execEvent( "buttonExec_clicked"   ) } )
      ::connect( ::oUI:q_buttonBrowse, "clicked()", {|| ::execEvent( "buttonBrowse_clicked" ) } )
      ::connect( ::oUI:q_buttonUpdate, "clicked()", {|| ::execEvent( "buttonUpdate_clicked" ) } )
      ::connect( ::oUI:q_buttonClose , "clicked()", {|| ::execEvent( "buttonClose_clicked"  ) } )

      ::connect( ::oUI:q_listNames   , "itemSelectionChanged()", {|| ::execEvent( "listNames_itemSelectionChanged" ) } )

      ::oUI:q_buttonBtnDown :setIcon( hbide_image( "dc_down" ) )
      ::oUI:q_buttonBtnUp   :setIcon( hbide_image( "dc_up"   ) )

      ::oUI:q_buttonSetImage:setIcon( hbide_image( "open"    ) )
      ::connect( ::oUI:q_buttonSetImage, "clicked()", {|| ::execEvent( "buttonSetImage_clicked" ) } )

      ::connect( ::oUI:q_buttonUserToolbarUpd, "clicked()", {|| ::execEvent( "buttonUserToolbarUpd_clicked" ) } )

      ::oUI:q_comboToolbarAsgnd:addItem( "User_Toolbar_1" )
      ::oUI:q_comboToolbarAsgnd:addItem( "User_Toolbar_2" )
      ::oUI:q_comboToolbarAsgnd:addItem( "User_Toolbar_3" )
      ::oUI:q_comboToolbarAsgnd:addItem( "User_Toolbar_4" )
      ::oUI:q_comboToolbarAsgnd:addItem( "User_Toolbar_5" )
      ::oUI:q_comboToolbarAsgnd:setCurrentIndex( -1 )
      ::connect( ::oUI:q_comboToolbarAsgnd, "currentIndexChanged(int)", {|p| ::execEvent( "comboToolbarAsgnd_currentIndexChanged", p ) } )

      ::oUI:q_listToolbars:addItem( "User_Toolbar_1" )
      ::oUI:q_listToolbars:addItem( "User_Toolbar_2" )
      ::oUI:q_listToolbars:addItem( "User_Toolbar_3" )
      ::oUI:q_listToolbars:addItem( "User_Toolbar_4" )
      ::oUI:q_listToolbars:addItem( "User_Toolbar_5" )
      ::connect( ::oUI:q_listToolbars, "itemSelectionChanged()", {|| ::execEvent( "listToolbars_itemSelectionChanged" ) } )

      ::oUI:q_comboInitPos:addItem( "Left"   )
      ::oUI:q_comboInitPos:addItem( "Top"    )
      ::oUI:q_comboInitPos:addItem( "Right"  )
      ::oUI:q_comboInitPos:addItem( "Bottom" )
      ::oUI:q_comboToolbarAsgnd:setCurrentIndex( -1 )

      ::oUI:q_checkDockTop   :setChecked( .f. )
      ::oUI:q_checkDockLeft  :setChecked( .t. )
      ::oUI:q_checkDockBottom:setChecked( .t. )
      ::oUI:q_checkDockRight :setChecked( .t. )
      ::oUI:q_checkFloatable :setChecked( .t. )

      ::oUI:q_checkToolActive:setChecked( .t. )
      #if 0
      ::connect( ::oUI:q_checkToolActive, "stateChanged(int)", {|i| ::execEvent( "checkToolActive_stateChanged", i ) } )
      #endif

      #if 1
      ::connect( ::oUI:q_checkInactive, "stateChanged(int)", {|i| ::execEvent( "checkToolActive_stateChanged", i ) } )
      #endif

      hdr_:= { { "Img", 30 }, { "Tool", 218 } }
      oTbl := ::oUI:q_tableButtons
      //
      QHeaderView():from( oTbl:verticalHeader() ):hide()
      QHeaderView():from( oTbl:horizontalHeader() ):stretchLastSection( .t. )
      oTbl:setAlternatingRowColors( .t. )
      oTbl:setColumnCount( len( hdr_ ) )
      oTbl:setShowGrid( .t. )
      oTbl:setSelectionMode( QAbstractItemView_SingleSelection )
      oTbl:setSelectionBehavior( QAbstractItemView_SelectRows )
      FOR n := 1 TO len( hdr_ )
         qItm := QTableWidgetItem():new()
         qItm:setText( hdr_[ n,1 ] )
         oTbl:setHorizontalHeaderItem( n-1, qItm )
         oTbl:setColumnWidth( n-1, hdr_[ n,2 ] )
         aadd( ::aHdr, qItm )
      NEXT

      ::oUI:q_listToolbars:setCurrentRow( 0 )
   ENDIF

   ::clearList()
   ::populateList( ::oINI:aTools )
   ::oUI:q_listNames:setCurrentRow( 0 )
   ::oIde:setPosByIniEx( ::oUI:oWidget, ::oINI:cToolsDialogGeometry )
   ::oUI:show()

   RETURN Nil

/*----------------------------------------------------------------------*/

METHOD IdeToolsManager:execEvent( cMode, p )
   LOCAL cFile, cFileName, nIndex, qItem, cName, nRow
   LOCAL aTools := ::oINI:aTools

   HB_SYMBOL_UNUSED( p )

   SWITCH cMode
   CASE "checkToolActive_stateChanged"
      nRow := ::oUI:q_listToolbars:currentRow()
      ::aUserToolbars[ nRow + 1, 3 ] := "YES"
      IF !empty( ::aToolbars[ nRow + 1 ] )
         IF p > 0
            ::aToolbars[ nRow + 1 ]:hide()
         ELSE
            ::aToolbars[ nRow + 1 ]:show()
         ENDIF
      ENDIF
      EXIT
   CASE "buttonSetImage_clicked"
      cFileName := hbide_fetchAFile( ::oDlg, "Select an PNG image", { { "Image Files", "*.png" } },/* cFolder */ , /*cDftSuffix*/ )
      IF !empty( cFileName )
         ::oUI:q_editImage:setText( hbide_pathNormalized( cFileName, .f. ) )
         ::oUI:q_buttonSetImage:setIcon( hbide_pathToOsPath( cFileName ) )
      ENDIF
      EXIT
   CASE "buttonUserToolbarUpd_clicked"
      ::ini2toolbarControls( ::oUI:q_listToolbars:currentRow(), 2 )
      EXIT
   CASE "listToolbars_itemSelectionChanged"
      // Clear q_tableButtons and populate with new values
      ::ini2toolbarControls( ::oUI:q_listToolbars:currentRow(), 1 )
      ::populateButtonsTable( ::oUI:q_listToolbars:currentRow() )
      EXIT
   CASE "comboToolbarAsgnd_currentIndexChanged"
      ::oUI:q_listToolbars:setCurrentRow( p )
      EXIT
   CASE "listNames_itemSelectionChanged"
      qItem := QListWidgetItem():from( ::oUI:q_listNames:currentItem() )
      cName := qItem:text()
      IF ( nIndex := ascan( aTools, {|e_| e_[ 1 ] == cName } ) ) > 0
         ::ini2Controls( nIndex )
      ENDIF
      EXIT
   CASE "buttonAdd_clicked"
      IF !empty( ::oUI:q_editName:text() )
         ::controls2ini()
         ::oUI:q_listNames:addItem( ::oUI:q_editName:text() )
      ENDIF
      EXIT
   CASE "buttonDelete_clicked"
      IF ::oUI:q_listNames:currentRow() >= 0
         qItem := QListWidgetItem():from( ::oUI:q_listNames:currentItem() )
         cName := qItem:text()
         IF ( nIndex := ascan( aTools, {|e_| e_[ 1 ] == cName } ) ) > 0
            hb_adel( ::oINI:aTools, nIndex, .t. )
            ::clearList()
            ::populateList()
         ENDIF
      ENDIF
      EXIT
   CASE "buttonUp_clicked"
      EXIT
   CASE "buttonDown_clicked"
      EXIT
   CASE "buttonExec_clicked"
      IF ! ::lExecuting
         ::lExecuting := .t.
         IF ::oUI:q_listNames:currentRow() >= 0
            qItem := QListWidgetItem():from( ::oUI:q_listNames:currentItem() )
            ::execTool( qItem:text() )
         ENDIF
         ::lExecuting := .f.
      ENDIF
      EXIT
   CASE "buttonBrowse_clicked"
      IF !empty( cFile := hbide_fetchAFile( ::oDlg, "Select a Tool" ) )
         hb_fNameSplit( cFile, , @cFileName )
         ::ini2controls()
         ::oUI:q_editName    : setText( cFileName )
         ::oUI:q_editCmdLine : setText( cFile )
      ENDIF
      EXIT
   CASE "buttonUpdate_clicked"
      IF ( nRow := ::oUI:q_listNames:currentRow() ) >= 0
         qItem := QListWidgetItem():from( ::oUI:q_listNames:currentItem() )
         cName := qItem:text()

         IF ( nIndex := ascan( aTools, {|e_| e_[ 1 ] == cName } ) ) > 0
            ::controls2ini( nIndex )
            ::clearList()
            ::populateList()
            ::oUI:q_listNames:setCurrentRow( nRow )
         ENDIF
      ENDIF
      EXIT
   CASE "buttonClose_clicked"
      ::oIde:oINI:cToolsDialogGeometry := hbide_posAndSize( ::oUI:oWidget )
      ::oUI:done( 1 )
      EXIT
   CASE "User_Toolbar_clicked"
      ::execTool( p )
      EXIT
   ENDSWITCH

   RETURN Self

/*----------------------------------------------------------------------*/

STATIC FUNCTION hbide_toolBlock( o, a_ )
   LOCAL cTool := a_[ 1 ]
   RETURN {|| o:execEvent( "User_Toolbar_clicked", cTool ) }

/*----------------------------------------------------------------------*/

METHOD IdeToolsManager:buildUserToolbars()
   LOCAL a_:={}, b_, qTbar, qTBtn, nn, nIndex
   LOCAL area_:= { Qt_LeftToolBarArea, Qt_TopToolBarArea, Qt_RightToolBarArea, Qt_BottomToolBarArea }
   LOCAL aIndex := {}

   FOR nIndex := 0 TO 4
      nn := nIndex + 1

      FOR EACH b_ IN ::aTools
         IF !empty( b_[ 7 ] ) .AND. val( b_[ 7 ] ) == nIndex
            aadd( a_, b_ )
            aadd( aIndex, b_:__enumIndex() )
         ENDIF
      NEXT
      IF !empty( a_ )
         qTBar := QToolBar():new()
         qTBar:setStyleSheet( GetStyleSheet( "QToolBarLR5", ::nAnimantionMode ) )
         qTBar:setObjectName( "User_Toolbar_" + hb_ntos( nIndex ) )
         qTBar:setWindowTitle( "User Toolbar : " + hb_ntos( nIndex ) )
         qTBar:setIconSize( QSize():new( 16,16 ) )
         qTBar:setToolButtonStyle( Qt_ToolButtonIconOnly )
         qTBar:setAllowedAreas( iif( ::aUserToolbars[ nn,4 ] == "YES", Qt_TopToolBarArea   , 0 ) + ;
                                iif( ::aUserToolbars[ nn,5 ] == "YES", Qt_LeftToolBarArea  , 0 ) + ;
                                iif( ::aUserToolbars[ nn,6 ] == "YES", Qt_BottomToolBarArea, 0 ) + ;
                                iif( ::aUserToolbars[ nn,7 ] == "YES", Qt_RightToolBarArea , 0 ) )

         FOR EACH b_ IN a_
            qTBtn := QToolButton():new()
            qTBtn:setText( b_[ 1 ] )
            qTBtn:setTooltip( b_[ 10 ] )
            qTBtn:setIcon( hbide_pathToOSPath( b_[ 9 ] ) )
            qTBtn:setMaximumWidth( 20 )
            qTBtn:setMaximumHeight( 20 )
            ::connect( qTBtn, "clicked()", hbide_toolBlock( Self, b_ ) )
            qTBar:addWidget( qTBtn )
            IF b_[ 8 ] != "YES"
               qTBtn:setEnabled( .f. )
            ENDIF
            aadd( ::aBtns, qTBtn )
         NEXT

         ::oDlg:oWidget:addToolBar( area_[ val( ::aUserToolbars[ nn,4 ] ) + 1 ], qTBar )
         IF ::aUserToolbars[ nn, 3 ] == "YES"
            qTBar:hide()
         ENDIF
         ::aToolbars[ nn ] := qTBar
         a_:= {}
      ENDIF
   NEXT

   RETURN Self

/*----------------------------------------------------------------------*/

METHOD IdeToolsManager:populateButtonsTable( nIndex )
   LOCAL a_:={}, b_, q0, q1, oTbl, nRow

   oTbl := ::oUI:q_tableButtons
   oTbl:clearContents()

   IF nIndex > -1
      FOR EACH b_ IN ::aTools
         IF !empty( b_[ 7 ] ) .AND. val( b_[ 7 ] ) == nIndex
            aadd( a_, b_ )
         ENDIF
      NEXT

      oTbl:setRowCount( len( a_ ) )

      IF !empty( a_ )
         FOR EACH b_ IN a_
            nRow := b_:__enumIndex()-1

            q0 := QTableWidgetItem():new()
            q0:setIcon( hbide_pathToOSPath( b_[ 9 ] ) )
            q0:setTooltip( b_[ 10 ] )
            oTbl:setItem( nRow, 0, q0 )

            q1 := QTableWidgetItem():new()
            q1:setText( b_[ 1 ] )
            oTbl:setItem( nRow, 1, q1 )

            oTbl:setRowHeight( nRow, 16 )
         NEXT
      ENDIF
   ENDIF

   RETURN Self

/*----------------------------------------------------------------------*/

METHOD IdeToolsManager:ini2toolbarControls( nIndex, nMode )

   IF nIndex > -1
      nIndex++
      IF nMode == 1
         ::oUI:q_comboInitPos   :setCurrentIndex( val( ::aUserToolBars[ nIndex, 1 ] ) )
         ::oUI:q_checkFloatable :setChecked( ::aUserToolBars[ nIndex, 2 ] == "YES" )
         ::oUI:q_checkInactive  :setChecked( ::aUserToolBars[ nIndex, 3 ] == "YES" )
         ::oUI:q_checkDockTop   :setChecked( ::aUserToolBars[ nIndex, 4 ] == "YES" )
         ::oUI:q_checkDockLeft  :setChecked( ::aUserToolBars[ nIndex, 5 ] == "YES" )
         ::oUI:q_checkDockBottom:setChecked( ::aUserToolBars[ nIndex, 6 ] == "YES" )
         ::oUI:q_checkDockRight :setChecked( ::aUserToolBars[ nIndex, 7 ] == "YES" )

      ELSE
         ::aUserToolBars[ nIndex, 1 ] := hb_ntos( ::oUI:q_comboInitPos:currentIndex() )
         ::aUserToolBars[ nIndex, 2 ] := iif( ::oUI:q_checkFloatable :isChecked(), "YES", "NO" )
         ::aUserToolBars[ nIndex, 3 ] := iif( ::oUI:q_checkInactive  :isChecked(), "YES", "NO" )
         ::aUserToolBars[ nIndex, 4 ] := iif( ::oUI:q_checkDockTop   :isChecked(), "YES", "NO" )
         ::aUserToolBars[ nIndex, 5 ] := iif( ::oUI:q_checkDockLeft  :isChecked(), "YES", "NO" )
         ::aUserToolBars[ nIndex, 6 ] := iif( ::oUI:q_checkDockBottom:isChecked(), "YES", "NO" )
         ::aUserToolBars[ nIndex, 7 ] := iif( ::oUI:q_checkDockRight :isChecked(), "YES", "NO" )

      ENDIF
   ENDIF

   RETURN Self

/*----------------------------------------------------------------------*/

METHOD IdeToolsManager:ini2controls( nIndex )

   IF nIndex > 0
      ::oUI:q_editName         :setText( ::aTools[ nIndex, 1 ] )
      ::oUI:q_editCmdLine      :setText( ::aTools[ nIndex, 2 ] )
      ::oUI:q_editParams       :setText( ::aTools[ nIndex, 3 ] )
      ::oUI:q_editStayIn       :setText( ::aTools[ nIndex, 4 ] )
      ::oUI:q_checkCapture     :setChecked( !empty( ::aTools[ nIndex, 5 ] ) )
      ::oUI:q_checkOpenCons    :setChecked( !empty( ::aTools[ nIndex, 6 ] ) )

      ::oUI:q_comboToolbarAsgnd:setCurrentIndex( iif( empty( ::aTools[ nIndex, 7 ] ), -1, val( ::aTools[ nIndex, 7 ] ) ) )
      ::oUI:q_checkToolActive  :setChecked( ::aTools[ nIndex, 8 ] == "YES" )
      ::oUI:q_editImage        :setText( hbide_pathNormalized( ::aTools[ nIndex, 9 ], .f. ) )
      ::oUI:q_buttonSetImage   :setIcon( iif( empty( ::aTools[ nIndex, 9 ] ), hbide_image( "open" ), ;
                                                               hbide_pathToOsPath( ::aTools[ nIndex, 9 ] ) ) )
      ::oUI:q_editTooltip      :setText( ::aTools[ nIndex, 10 ] )

   ELSE
      ::oUI:q_editName         :setText( "" )
      ::oUI:q_editCmdLine      :setText( "" )
      ::oUI:q_editParams       :setText( "" )
      ::oUI:q_editStayIn       :setText( "" )
      ::oUI:q_checkCapture     :setChecked( .f. )
      ::oUI:q_checkOpenCons    :setChecked( .f. )

      ::oUI:q_comboToolbarAsgnd:setCurrentIndex( -1 )
      ::oUI:q_checkToolActive  :setChecked( .t. )
      ::oUI:q_editImage        :setText( "" )
      ::oUI:q_buttonSetImage   :setIcon( hbide_image( "open" ) )
      ::oUI:q_editTooltip      :setText( "" )

   ENDIF

   RETURN Self

/*----------------------------------------------------------------------*/

METHOD IdeToolsManager:controls2ini( nIndex )

   IF empty( nIndex )
      aadd( ::oINI:aTools, {} )
      nIndex := len( ::oINI:aTools )
   ENDIF

   ::oINI:aTools[ nIndex ] := { ::oUI:q_editName:text()   , ;
                                    hbide_pathNormalized( ::oUI:q_editCmdLine:text() ), ;
                                    hbide_pathNormalized( ::oUI:q_editParams:text()  ), ;
                                    hbide_pathNormalized( ::oUI:q_editStayIn:text()  ), ;
                                    iif( ::oUI:q_checkCapture:isChecked(), "YES", "" ), ;
                                    iif( ::oUI:q_checkOpenCons:isChecked(), "YES", "" ), ;
                                    ;
                                    hb_ntos( ::oUI:q_comboToolbarAsgnd:currentIndex() ), ;
                                    iif( ::oUI:q_checkToolActive:isChecked(), "YES", "NO" ), ;
                                    ::oUI:q_editImage:text(), ;
                                    ::oUI:q_editTooltip:text() ;
                                  }
   RETURN Self

/*----------------------------------------------------------------------*/

METHOD IdeToolsManager:clearList()

   ::oUI:q_listNames:clear()

   RETURN Self

/*----------------------------------------------------------------------*/

METHOD IdeToolsManager:populateList( aList )
   LOCAL a_

   DEFAULT aList TO ::oINI:aTools

   FOR EACH a_ IN aList
      ::oUI:q_listNames:addItem( a_[ 1 ] )
   NEXT

   RETURN Self

/*----------------------------------------------------------------------*/

METHOD IdeToolsManager:buildToolsButton()
   LOCAL a_, qAct

   ::qToolsMenu := QMenu():new()
   FOR EACH a_ IN ::aTools
      qAct := ::qToolsMenu:addAction( a_[ 1 ] )
      ::connect( qAct, "triggered(bool)", {|| ::execTool( a_[ 1 ] ) } )
      aadd( ::aAct, qAct )
   NEXT
   ::qToolsButton := QToolButton():new()
   ::qToolsButton:setTooltip( "Tools & Utilities" )
   ::qToolsButton:setIcon( hbide_image( "tools" ) )
   ::qToolsButton:setPopupMode( QToolButton_MenuButtonPopup )
   ::qToolsButton:setMenu( ::qToolsMenu )

   ::connect( ::qToolsButton, "clicked()", {|| ::show() } )

   RETURN ::qToolsButton

/*----------------------------------------------------------------------*/

METHOD IdeToolsManager:buildPanelsButton()
   LOCAL cView

   ::qPanelsMenu := QMenu():new()
   ::addPanelsMenu( "Main" )
   FOR EACH cView IN ::oINI:aViews
      ::addPanelsMenu( cView )
   NEXT
   ::qPanelsButton := QToolButton():new()
   ::qPanelsButton:setTooltip( "Panels" )
   ::qPanelsButton:setIcon( hbide_image( "panel_8" ) )
   ::qPanelsButton:setPopupMode( QToolButton_MenuButtonPopup )
   ::qPanelsButton:setMenu( ::qPanelsMenu )

   ::connect( ::qPanelsButton, "clicked()", {|| ::oDK:setView( "New..." ) } )

   RETURN ::qPanelsButton

/*----------------------------------------------------------------------*/

METHOD IdeToolsManager:addPanelsMenu( cPrompt )
   LOCAL qAct

   qAct := ::qPanelsMenu:addAction( cPrompt )
   QAction():from( qAct ):setIcon( ::oDK:getPanelIcon( cPrompt ) )
   ::connect( qAct, "triggered(bool)", {|| ::oDK:setView( cPrompt ) } )
   aadd( ::aPanelsAct, qAct )

   RETURN Self

/*----------------------------------------------------------------------*/

METHOD IdeToolsManager:execToolByParams( cCmd, cParams, cStartIn, lCapture, lOpen )
   LOCAL cArg

   ::oProcess := HbpProcess():new()

   ::oProcess:output      := {|cOut, mp2, oHbp| ::showOutput( cOut, mp2, oHbp ) }
   ::oProcess:finished    := {|nEC , nES, oHbp| ::finished( nEC, nES, oHbp ) }
   ::oProcess:workingPath := cStartIn
   ::oProcess:lDetached   := !( lCapture )

   IF empty( cCmd )
      cCmd := hbide_getShellCommand()
      cArg := iif( hbide_getOS() == "nix", "", "/C " )
      //cArg := ""
   ELSE
      cArg := ""
   ENDIF
   cArg += ::parseParams( cParams )

   IF lCapture
      IF lOpen
         ::oDockB2:show()
      ENDIF
      ::oOutputResult:oWidget:clear()
      ::oOutputResult:oWidget:append( cCmd )
      ::oOutputResult:oWidget:append( cArg )
      ::oOutputResult:oWidget:append( hbide_outputLine() )
   ENDIF
   ::oProcess:addArg( cArg )
   ::oProcess:start( cCmd )

   RETURN Self

/*----------------------------------------------------------------------*/

METHOD IdeToolsManager:execTool( ... )
   LOCAL nIndex, cCmd, cParams, cStayIn, lCapture, lOpen, aParam

   aParam := hb_aParams()
   IF len( aParam ) == 1
      IF ( nIndex := ascan( ::aTools, {|e_| e_[ 1 ] == aParam[ 1 ] } ) ) > 0
         cCmd     := hbide_pathToOSPath( ::aTools[ nIndex, 2 ] )
         cParams  := ::aTools[ nIndex, 3 ]
         cParams  := iif( "http://" $ lower( cParams ), cParams, hbide_pathToOSPath( cParams ) )
         cStayIn  := hbide_pathToOSPath( ::aTools[ nIndex, 4 ] )
         lCapture := ::aTools[ nIndex, 5 ] == "YES"
         lOpen    := ::aTools[ nIndex, 6 ] == "YES"
      ENDIF
   ELSEIF len( aParam ) > 1
      asize( aParam, 5 )

      DEFAULT aParam[ 1 ] TO ""
      DEFAULT aParam[ 2 ] TO ""
      DEFAULT aParam[ 3 ] TO ""
      DEFAULT aParam[ 4 ] TO ""
      DEFAULT aParam[ 5 ] TO ""

      cCmd     := hbide_pathToOSPath( aParam[ 1 ] )
      cParams  := aParam[ 2 ]
      cParams  := iif( "http://" $ lower( cParams ), cParams, hbide_pathToOSPath( cParams ) )
      cParams  := ::parseParams( cParams )
      cStayIn  := hbide_pathToOSPath( aParam[ 3 ] )
      lCapture := iif( hb_isLogical( aParam[ 4 ] ), aParam[ 4 ], aParam[ 4 ] == "YES" )
      lOpen    := iif( hb_isLogical( aParam[ 5 ] ), aParam[ 5 ], aParam[ 5 ] == "YES" )
   ENDIF

   IF hb_isLogical( lCapture )
      ::execToolByParams( cCmd, cParams, cStayIn, lCapture, lOpen )
   ENDIF

   RETURN Self

/*----------------------------------------------------------------------*/

METHOD IdeToolsManager:showOutput( cOut, mp2, oHbp )

   HB_SYMBOL_UNUSED( mp2 )
   HB_SYMBOL_UNUSED( oHbp )

   hbide_convertBuildStatusMsgToHtml( cOut, ::oOutputResult:oWidget )

   RETURN Self

/*----------------------------------------------------------------------*/

METHOD IdeToolsManager:finished( nEC, nES, oHbp )

   HB_SYMBOL_UNUSED( oHbp )

   ::oOutputResult:oWidget:append( hbide_outputLine() )
   ::oOutputResult:oWidget:append( "Finished: Exit Code = " + hb_ntos( nEC ) + " Status = " + hb_ntos( nES ) )

   RETURN Self

/*----------------------------------------------------------------------*/

METHOD IdeToolsManager:parseParams( cP )
   LOCAL lHas, n, n1, cMacro

   IF !empty( cP )
      DO WHILE .t.
         lHas := .f.

         IF ( n := at( "${" , cP ) ) > 0
            IF ( n1 := at( "}" , cP ) ) > 0
               lHas    := .t.
               cMacro  := substr( cP, n + 2, n1 - n - 2 )
               cP      := substr( cP, 1, n - 1 ) + ::macro2value( cMacro ) + substr( cP, n1 + 1 )
            ENDIF
         ENDIF
         IF ! lHas
            EXIT
         ENDIF
      ENDDO
   ENDIF

   RETURN cP

/*----------------------------------------------------------------------*/

METHOD IdeToolsManager:macro2value( cMacro )
   LOCAL cVal, cMacroL, oEdit, cFile, cPath, cExt

   cMacro  := alltrim( cMacro )
   cMacroL := lower( cMacro )

   oEdit   := ::oEM:getEditorCurrent()
   IF !empty( oEdit )
      hb_fNameSplit( oEdit:sourceFile, @cPath, @cFile, @cExt )
   ELSE
      cPath := ""; cFile := ""; cExt := ""
   ENDIF

   DO CASE
   CASE cMacroL == "source_fullname"
      cVal := hbide_pathToOSPath( cPath + cFile + cExt )

   CASE cMacroL == "source_path"
      cVal := hbide_pathToOSPath( cPath )

   CASE cMacroL == "source_fullname_less_ext"
      cVal := hbide_pathToOSPath( cPath + cFile )

   CASE cMacroL == "source_name"
      cVal := cFile + cExt

   CASE cMacroL == "source_ext"
      cVal := cExt

   OTHERWISE
      cVal := hb_GetEnv( cMacro )

   ENDCASE

   RETURN cVal

/*----------------------------------------------------------------------*/
