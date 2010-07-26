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
 *                               27Jun2010
 */
/*----------------------------------------------------------------------*/
/*----------------------------------------------------------------------*/
/*----------------------------------------------------------------------*/

#include "common.ch"
#include "hbclass.ch"
#include "hbqt.ch"
#include "hbide.ch"
#include "xbp.ch"
#include "appevent.ch"

/*----------------------------------------------------------------------*/

#define  BRW_TYPE_DBF                             1
#define  BRW_TYPE_ARRAY                           2

/*------------------------------------------------------------------------*/

#define  TBL_PANEL                                1
#define  TBL_NAME                                 2
#define  TBL_ALIAS                                3
#define  TBL_DRIVER                               4
#define  TBL_INDEX                                5
#define  TBL_RECORD                               6
#define  TBL_CURSOR                               7
#define  TBL_GEOMETRY                             8
#define  TBL_ROWPOS                               9
#define  TBL_COLPOS                              10
#define  TBL_HZSCROLL                            11
#define  TBL_CONXN                               12
#define  TBL_NEXT                                13

#define  TBL_VRBLS                               13

#define  SUB_ID                                   1
#define  SUB_WINDOW                               2
#define  SUB_GEOMETRY                             3
#define  SUB_BROWSER                              4
#define  SUB_NIL                                  5

#define  PNL_PANELS                               1
#define  PNL_TABLES                               2
#define  PNL_MISC                                 3
#define  PNL_READY                                4

/*----------------------------------------------------------------------*/

CLASS IdeBrowseManager INHERIT IdeObject

   DATA   qDbu
   DATA   qStack
   DATA   qLayout
   DATA   qVSplitter
   DATA   qToolBar
   DATA   qToolBarL
   DATA   qStruct
   DATA   qRddCombo
   DATA   qConxnCombo
   DATA   qStatus
   DATA   qTimer

   DATA   aStatusPnls                             INIT  {}
   DATA   aPanels                                 INIT  {}
   DATA   aToolBtns                               INIT  {}
   DATA   aButtons                                INIT  {}
   DATA   aIndexAct                               INIT  {}
   DATA   aRdds                                   INIT  { "DBFCDX", "DBFNTX", "DBFNSX" }
   DATA   aConxns                                 INIT  {}

   DATA   oCurBrw
   DATA   oCurPanel

   DATA   qPanelsMenu
   DATA   qIndexMenu
   DATA   qTablesMenu
   DATA   qPanelsButton
   DATA   qIndexButton
   DATA   qTablesButton
   DATA   aPanelsAct                              INIT  {}

   DATA   lStructOpen                             INIT  .f.
   DATA   lDeletedOn                              INIT  .t.

   METHOD new( oIde )
   METHOD create( oIde )
   METHOD show()
   METHOD destroy()
   METHOD buildToolbar()
   METHOD execEvent( cEvent, p, p1 )
   METHOD addArray( aData, aAttr )
   METHOD getPanelNames()
   METHOD getPanelsInfo()
   METHOD addPanels()
   METHOD addPanel( cPanel )
   METHOD setPanel( cPanel )
   METHOD isPanel( cPanel )
   METHOD loadTables()
   METHOD buildPanelsButton()
   METHOD buildIndexButton()
   METHOD buildToolButton( qToolbar, aBtn )
   METHOD addPanelsMenu( cPanel )
   METHOD setStyleSheet( nMode )
   METHOD showStruct()
   METHOD buildUiStruct()
   METHOD populateUiStruct()
   METHOD populateFieldData()
   METHOD updateIndexMenu( oBrw )
   METHOD buildRddsCombo()
   METHOD buildConxnCombo()
   METHOD loadConxnCombo( cDriver )
   ACCESS currentDriver()                         INLINE ::qRddCombo:currentText()
   ACCESS currentConxn()                          INLINE ::qConxnCombo:currentText()
   METHOD buildStatusPanels()
   METHOD dispStatusInfo()
   METHOD buildLeftToolbar()
   METHOD buildTablesButton()
   METHOD showTablesTree()
   METHOD fetchFldsList( cAlias )
   METHOD getBrowserByAlias( cAlias )

   ENDCLASS

/*----------------------------------------------------------------------*/

METHOD IdeBrowseManager:new( oIde )
   ::oIde := oIde
   RETURN Self

/*----------------------------------------------------------------------*/

METHOD IdeBrowseManager:getPanelNames()
   LOCAL oPanel, aNames := {}, aAttr

   FOR EACH oPanel IN ::aPanels
      aAttr := {}

      aadd( aAttr, oPanel:cPanel )
      aadd( aAttr, hb_ntos( oPanel:viewMode() ) )
      aadd( aAttr, hb_ntos( oPanel:nViewStyle ) )

      aadd( aNames,  hbide_array2String( aAttr, "," ) )
   NEXT
   RETURN aNames

/*----------------------------------------------------------------------*/

METHOD IdeBrowseManager:getPanelsInfo()
   LOCAL oBrw, oPanel, aSub
   LOCAL aInfo := {}, aAttr

   FOR EACH oPanel IN ::aPanels
      FOR EACH aSub IN oPanel:subWindows()
         aAttr := array( TBL_VRBLS )
         aAttr[ TBL_PANEL ] := oPanel:cPanel

         oBrw := aSub[ 4 ]

         IF oBrw:nType == BRW_TYPE_DBF
            aAttr[ TBL_NAME     ] := oBrw:cTable
            aAttr[ TBL_ALIAS    ] := oBrw:cAlias
            aAttr[ TBL_DRIVER   ] := oBrw:cDriver
            aAttr[ TBL_INDEX    ] := hb_ntos( oBrw:indexOrd()  )
            aAttr[ TBL_RECORD   ] := hb_ntos( oBrw:recNo()     )
            aAttr[ TBL_CURSOR   ] := hb_ntos( oBrw:nCursorType )
            IF !hb_isObject( aSub[ SUB_GEOMETRY ] )
               aSub[ SUB_GEOMETRY ] := QRect():from( aSub[ SUB_WINDOW ]:geometry() )
            ENDIF
            aAttr[ TBL_GEOMETRY ] := hb_ntos( aSub[ SUB_GEOMETRY ]:x() )     + " " + hb_ntos( aSub[ SUB_GEOMETRY ]:y() ) + " " + ;
                                     hb_ntos( aSub[ SUB_GEOMETRY ]:width() ) + " " + hb_ntos( aSub[ SUB_GEOMETRY ]:height() )
            aAttr[ TBL_ROWPOS   ] := hb_ntos( oBrw:oBrw:rowPos() )
            aAttr[ TBL_COLPOS   ] := hb_ntos( oBrw:oBrw:colPos() )
            aAttr[ TBL_HZSCROLL ] := "" //hb_ntos( oBrw:
            aAttr[ TBL_CONXN    ] := oBrw:cConxnFull
            aAttr[ TBL_NEXT     ] := ""

         ELSEIF oBrw:nType == BRW_TYPE_ARRAY
            //
         ENDIF

         aadd( aInfo, hbide_array2String( aAttr, "," ) )
      NEXT
   NEXT

   RETURN aInfo

/*----------------------------------------------------------------------*/

METHOD IdeBrowseManager:setStyleSheet( nMode )

   ::qToolbar:setStyleSheet( GetStyleSheet( "QToolBar", nMode ) )
   ::qToolbarL:setStyleSheet( GetStyleSheet( "QToolBarLR5", nMode ) )
   ::qStatus:setStyleSheet( GetStyleSheet( "QStatusBar", nMode ) )
   ::qPanelsMenu:setStyleSheet( GetStyleSheet( "QMenuPop", nMode ) )
   ::qIndexMenu:setStyleSheet( GetStyleSheet( "QMenuPop", nMode ) )
   ::qTablesMenu:setStyleSheet( GetStyleSheet( "QMenuPop", nMode ) )

   RETURN Self

/*------------------------------------------------------------------------*/

METHOD IdeBrowseManager:destroy()
   LOCAL oPanel

   FOR EACH oPanel IN ::aPanels
      oPanel:destroy()
   NEXT
   ::aPanels := {}

   RETURN Self

/*----------------------------------------------------------------------*/

METHOD IdeBrowseManager:show()

   ::qToolbar:setStyleSheet( GetStyleSheet( "QToolBar", ::nAnimantionMode ) )
   ::oQScintillaDock:oWidget:raise()

   RETURN Self

/*----------------------------------------------------------------------*/

METHOD IdeBrowseManager:create( oIde )
   LOCAL qDock

   SET DELETED ( ::lDeletedOn )

   DEFAULT oIde TO ::oIde
   ::oIde := oIde

   qDock := ::oIde:oEM:oQScintillaDock:oWidget

   ::qDbu := QWidget():new()

   qDock:setWidget( ::qDbu )

   qDock:setAcceptDrops( .t. )
   qDock:installEventFilter( ::pEvents )
   ::connect( qDock, QEvent_DragEnter, {|p| ::execEvent( "dockDbu_dragEnterEvent", p ) } )
   ::connect( qDock, QEvent_Drop     , {|p| ::execEvent( "dockDbu_dropEvent"     , p ) } )

   /* Layout applied to dbu widget */
   ::qLayout := QGridLayout():new()
   ::qLayout:setContentsMargins( 0,0,0,0 )
   ::qLayout:setSpacing( 0 )

   ::qDbu:setLayout( ::qLayout )

   /* Toolbar */
   ::buildToolbar()
   ::qLayout:addWidget_1( ::qToolbar, 0, 0, 1, 2 )

   /* Toolbar left */
   ::buildLeftToolbar()
   ::qLayout:addWidget_1( ::qToolbarL, 1, 0, 1, 1 )

   /* Stacked widget */
   ::qStack := QStackedWidget():new()
   ::qLayout:addWidget_1( ::qStack, 1, 1, 1, 1 )

   /* StatusBar */
   ::qStatus := QStatusBar():new()
   ::qStatus:setSizeGripEnabled( .f. )
   ::qLayout:addWidget_1( ::qStatus, 2, 0, 1, 2 )

   /* */
   ::buildStatusPanels()

   /* Panels on the stacked widget */
   ::addPanels()

   /* Spread tables onto panels */
   ::loadTables()

   /* Switch to the default panel */
   ::setPanel( "Main" )

   /* Timer to update ststus bar */
   ::qTimer := QTimer():new()
   ::qTimer:setInterval( 2000 )
   ::connect( ::qTimer, "timeout()", {|| ::dispStatusInfo() } )
   ::qTimer:start()

   RETURN Self

/*----------------------------------------------------------------------*/

METHOD IdeBrowseManager:fetchFldsList( cAlias )
   LOCAL aFlds := {}, cA, oBrw, a_, oPanel, aBrw

   cA := upper( cAlias )

   SWITCH cA
   CASE "FIELD"
      FOR EACH oPanel IN ::aPanels
         FOR EACH aBrw IN oPanel:aBrowsers
            oBrw := aBrw[ SUB_BROWSER ]
            FOR EACH a_ IN oBrw:aStruct
               aadd( aFlds, pad( a_[ 1 ], 10 ) + " (" + padc( oBrw:cTableOnly, 12 ) + ")" + str( a_:__enumIndex(),3,0 ) + ", " + a_[ 2 ] + ", " + str( a_[ 3 ],3,0 ) + ", " + hb_ntos( a_[ 4 ] ) + " [f]" )
            NEXT
         NEXT
      NEXT
      EXIT
   OTHERWISE
      IF ! empty( oBrw := ::getBrowserByAlias( cA ) )
         FOR EACH a_ IN oBrw:aStruct
            aadd( aFlds, pad( a_[ 1 ], 10 ) + " ( " + str( a_:__enumIndex(),3,0 ) + ", " + a_[ 2 ] + ", " + str( a_[ 3 ],3,0 ) + ", " + hb_ntos( a_[ 4 ] ) + " )"  + " [f]" )
         NEXT
      ENDIF
      EXIT
   ENDSWITCH

   RETURN aFlds

/*------------------------------------------------------------------------*/

METHOD IdeBrowseManager:getBrowserByAlias( cAlias )
   LOCAL oPanel, aBrw

   FOR EACH oPanel IN ::aPanels
      FOR EACH aBrw IN oPanel:aBrowsers
         IF aBrw[ SUB_BROWSER ]:cAlias == cAlias
            RETURN aBrw[ SUB_BROWSER ]
         ENDIF
      NEXT
   NEXT
   RETURN NIL

/*------------------------------------------------------------------------*/

METHOD IdeBrowseManager:dispStatusInfo()

   ::aStatusPnls[ PNL_PANELS ]:setText( "Panels: " + hb_ntos( len( ::aPanels ) ) + ":" + ::oCurPanel:cPanel )
   ::aStatusPnls[ PNL_TABLES ]:setText( "Tables: " + hb_ntos( len( ::oCurPanel:aBrowsers ) ) )

   ::aStatusPnls[ PNL_MISC   ]:setText( "M:"    )
   ::aStatusPnls[ PNL_READY  ]:setText( "Ready" )

   RETURN Self

/*------------------------------------------------------------------------*/

METHOD IdeBrowseManager:buildStatusPanels()
   LOCAL qLabel

   qLabel := QLabel():new(); qLabel:setMinimumWidth( 40 )
   ::qStatus:addPermanentWidget( qLabel, 0 )
   aadd( ::aStatusPnls, qLabel )

   qLabel := QLabel():new(); qLabel:setMinimumWidth( 40 )
   ::qStatus:addPermanentWidget( qLabel, 0 )
   aadd( ::aStatusPnls, qLabel )

   qLabel := QLabel():new(); qLabel:setMinimumWidth( 40 )
   ::qStatus:addPermanentWidget( qLabel, 0 )
   aadd( ::aStatusPnls, qLabel )

   qLabel := QLabel():new(); qLabel:setMinimumWidth( 40 )
   ::qStatus:addPermanentWidget( qLabel, 1 )
   aadd( ::aStatusPnls, qLabel )

   RETURN Self

/*------------------------------------------------------------------------*/

METHOD IdeBrowseManager:addPanels()
   LOCAL cPanel, aPnl

   ::addPanel( "Main", .t. )           /* The default one */

   FOR EACH cPanel IN ::oINI:aDbuPanelNames
      aPnl := hb_aTokens( cPanel, "," )
      aSize( aPnl, 2 )
      IF empty( aPnl[ 2 ] )
         aPnl[ 2 ] := "NO"
      ENDIF
      IF aPnl[ 1 ] != "Main"
         ::addPanel( aPnl[ 1 ], aPnl[ 2 ] == "YES" )
      ENDIF
   NEXT

   RETURN Self

/*----------------------------------------------------------------------*/

METHOD IdeBrowseManager:addPanel( cPanel )
   LOCAL qPanel

   qPanel := IdeBrowsePanel():new( ::oIde, cPanel, self )
   ::qStack:addWidget( qPanel:qWidget )
   aadd( ::aPanels, qPanel )
   ::addPanelsMenu( cPanel )

   RETURN Self

/*----------------------------------------------------------------------*/

METHOD IdeBrowseManager:addPanelsMenu( cPanel )
   LOCAL qAct

   ( qAct := QAction():from( ::qPanelsMenu:addAction( cPanel ) ) ):setIcon( hbide_image( "panel_7" ) )
   ::connect( qAct, "triggered(bool)", {|| ::setPanel( cPanel ) } )
   aadd( ::aPanelsAct, qAct )

   RETURN Self

/*----------------------------------------------------------------------*/

METHOD IdeBrowseManager:isPanel( cPanel )
   RETURN ascan( ::aPanels, {|o| o:qWidget:objectName() == cPanel } ) > 0

/*----------------------------------------------------------------------*/

METHOD IdeBrowseManager:setPanel( cPanel )
   LOCAL n

   IF ( n := ascan( ::aPanels, {|o| o:qWidget:objectName() == cPanel } ) ) > 0
      ::qStack:setCurrentWidget( ::aPanels[ n ]:qWidget )
      ::oCurPanel := ::aPanels[ n ]
      ::oCurPanel:prepare()
      ::oCurPanel:activateBrowser()
   ENDIF

   RETURN Self

/*----------------------------------------------------------------------*/

METHOD IdeBrowseManager:execEvent( cEvent, p, p1 )
   LOCAL cTable, cPath, cPanel, qEvent, qMime, qList, i, cExt, qUrl

   HB_SYMBOL_UNUSED( p )
   HB_SYMBOL_UNUSED( p1 )

   SWITCH cEvent
   CASE "dockDbu_dragEnterEvent"
      qEvent := QDragEnterEvent():from( p )
      qEvent:acceptProposedAction()
      EXIT

   CASE "dockDbu_dropEvent"
      qEvent := QDropEvent():from( p )
      qMime := QMimeData():from( qEvent:mimeData() )
      IF qMime:hasUrls()
         qList := QStringList():from( qMime:hbUrlList() )
         FOR i := 0 TO qList:size() - 1
            qUrl := QUrl():new( qList:at( i ) )
            hb_fNameSplit( qUrl:toLocalFile(), @cPath, @cTable, @cExt )
            IF lower( cExt ) == ".dbf"
               ::oCurPanel:addBrowser( { NIL, hbide_pathToOSPath( cPath + cTable + cExt ), NIL, ;
                             iif( ! ( ::qRddCombo:currentText() $ "DBFCDX.DBFNTX,DBFNSX" ), "DBFCDX", ::qRddCombo:currentText() ) } )
            ENDIF
         NEXT
      ENDIF
      EXIT

   CASE "buttonShowForm_clicked"
      IF !empty( ::oCurBrw )
         IF ::oCurBrw:qScrollArea:isHidden()
            ::oCurBrw:qScrollArea:show()
            ::aToolBtns[ 3 ]:setChecked( .t. )
         ELSE
            ::oCurBrw:qScrollArea:hide()
            ::aToolBtns[ 3 ]:setChecked( .f. )
         ENDIF
      ENDIF
      EXIT

   CASE "buttonClose_clicked"
      IF !empty( ::oCurBrw )
         ::oCurPanel:destroyBrw( ::oCurBrw )
      ENDIF
      EXIT

   CASE "buttonOpen_clicked"
      IF ::currentDriver() $ "DBFCDX,DBFNTX,DBFNSX"
         IF !empty( cTable := hbide_fetchAFile( ::oIde:oDlg, "Select a Table", { { "Database File", "*.dbf" } }, ::oIde:cWrkFolderLast ) )
            hb_fNameSplit( cTable, @cPath )
            ::oIde:cWrkFolderLast := cPath
            ::oCurPanel:addBrowser( { NIL, cTable } )
         ENDIF
      ELSE
         IF ! empty( cTable := hbide_execScriptFunction( "tableSelect", ::currentDriver(), ::currentConxn() ) )
            ::oCurPanel:addBrowser( { NIL, cTable } )
         ENDIF
      ENDIF
      EXIT

   CASE "qPanelsButton_clicked"
      cPanel := hbide_fetchAString( ::qToolbar, "New...", "Name the Panel", "New Panel" )
      IF cPanel != "New..." .AND. cPanel != "Main"
         IF ::isPanel( cPanel )
            MsgBox( "Panel: " + cPanel + ", already exists" )
         ELSE
            ::addPanel( cPanel )
            ::setPanel( cPanel )
         ENDIF
      ENDIF
      EXIT

   CASE "buttonViewOrganized_clicked"
      ::oCurPanel:nViewStyle  := 0
      ::oCurPanel:restGeometry()
      ::oCurPanel:prepare()
      EXIT

   CASE "buttonSaveLayout_clicked"
      ::oCurPanel:saveGeometry()
      EXIT

   CASE "buttonViewTiled_clicked"
      ::oCurPanel:saveGeometry()
      ::oCurPanel:tileSubWindows()
      ::oCurPanel:nViewStyle  := 1
      ::oCurPanel:prepare()
      EXIT

   CASE "buttonViewCascaded_clicked"
      ::oCurPanel:saveGeometry()
      ::oCurPanel:cascadeSubWindows()
      ::oCurPanel:nViewStyle  := 2
      ::oCurPanel:prepare()
      EXIT

   CASE "buttonViewTabbed_clicked"
      ::oCurPanel:setViewMode( iif( ::oCurPanel:viewMode() == QMdiArea_TabbedView, QMdiArea_SubWindowView, QMdiArea_TabbedView ) )
      EXIT

   CASE "buttonDbStruct_clicked"
      IF !empty( ::oCurBrw )
         ::showStruct()
      ENDIF
      EXIT

   CASE "buttonTables_clicked"
      ::showTablesTree()
      EXIT

   CASE "buttonIndex_clicked"
      EXIT

   CASE "dbStruct_closeEvent"
      ::oIde:oINI:cDbStructDialogGeometry := hbide_posAndSize( ::qStruct:oWidget )
      ::qStruct:close()
      ::lStructOpen := .f.
      EXIT

   CASE "fieldsTable_itemSelectionChanged"
      ::populateFieldData()
      EXIT

   CASE "buttonFind_clicked"
      IF !empty( ::oCurBrw )
         ::oCurBrw:searchAsk()
      ENDIF
      EXIT

   CASE "buttonGoto_clicked"
      IF !empty( ::oCurBrw )
         ::oCurBrw:gotoAsk()
      ENDIF
      EXIT

   CASE "buttonAppendRecord_clicked"
      IF !empty( ::oCurBrw )
         ::oCurBrw:append()
      ENDIF
      EXIT
   CASE "buttonDelRecord_clicked"
      IF !empty( ::oCurBrw )
         ::oCurBrw:delete( .t. )
      ENDIF
      EXIT
   CASE "buttonLockRecord_clicked"
      IF !empty( ::oCurBrw )
         ::oCurBrw:lock()
      ENDIF
      EXIT
   CASE "buttonGoTop_clicked"
      IF !empty( ::oCurBrw )
         ::oCurBrw:goTop()
      ENDIF
      EXIT
   CASE "buttonGoBottom_clicked"
      IF !empty( ::oCurBrw )
         ::oCurBrw:goBottom()
      ENDIF
      EXIT
   CASE "buttonScrollToFirst_clicked"
      IF !empty( ::oCurBrw )
         ::oCurBrw:toColumn( 1 )
      ENDIF
      EXIT
   CASE "buttonScrollToLast_clicked"
      IF !empty( ::oCurBrw )
         ::oCurBrw:toColumn( len( ::oCurBrw:aStruct ) )
      ENDIF
      EXIT
   CASE "buttonSearchInTable_clicked"
      IF !empty( ::oCurBrw )
         ::oCurBrw:searchAsk()
      ENDIF
      EXIT
   CASE "buttonZaptable_clicked"
      EXIT

   ENDSWITCH

   RETURN Self

/*----------------------------------------------------------------------*/

METHOD IdeBrowseManager:showTablesTree()
   LOCAL oUI, qTree, qParent, oPanel, qItm, aBrowser, q, aFld, qFont, nMax, nSz, oBrw
   LOCAL a_:={}

   oUI := hbide_getUI( "tables", ::oCurPanel:qWidget )

   qFont := QFont():new( "Courier New", 8 )
   qTree := oUI:q_treeTables
   qTree:setFont( qFont )

   FOR EACH oPanel IN ::aPanels
      qParent := QTreeWidgetItem():new()
      qParent:setText( 0, oPanel:cPanel )
      qTree:addTopLevelItem( qParent )
      aadd( a_, qParent )
      FOR EACH aBrowser IN oPanel:aBrowsers
         oBrw := aBrowser[ SUB_BROWSER ]

         qItm := QTreeWidgetItem():new()
         qItm:setText( 0, oBrw:cTable )

         qItm:setToolTip( 0, oBrw:cTableOnly + " [ " + oBrw:cDriver + "  " + ;
                          hb_ntos( oBrw:indexOrd() ) + "/" + hb_ntos( oBrw:numIndexes() ) + iif( oBrw:indexOrd() > 0, ":" + oBrw:ordName(), "" ) + ;
                          "  " + hb_ntos( oBrw:recno() ) + "/" + hb_ntos( oBrw:lastRec() ) + " ]  " )

         qParent:addChild( qItm )
         nSz := 0 ; aeval( aBrowser[ SUB_BROWSER ]:aStruct, {|e_| nSz += e_[ 3 ] } )
         nMax := 12
         FOR EACH aFld IN aBrowser[ SUB_BROWSER ]:aStruct
            q := QTreeWidgetItem():new()
            q:setText( 0, pad( aFld[ 1 ], nMax ) + aFld[ 2 ] + str( aFld[ 3 ], 4, 0 ) + str( aFld[ 4 ], 2, 0 ) )
            q:setToolTip( 0, "" )
            qItm:addChild( q )
         NEXT
         q := QTreeWidgetItem():new()
         q:setText( 0, pad( "T", nMax - 2 ) + str( nSz, 7, 0 ) )
         qItm:addChild( q )
     NEXT
      qParent:setExpanded( .t. )
   NEXT
   ::oIde:setPosAndSizeByIniEx( oUI:oWidget, ::oINI:cTablesDialogGeometry )
   ::connect( oUI:q_buttonOk, "clicked()", {|| oUI:done( 1 ) } )
   oUI:exec()
   ::disconnect( oUI:q_buttonOk, "clicked()" )
   ::oIde:oINI:cTablesDialogGeometry := hbide_posAndSize( oUI:oWidget )

   RETURN Self

/*------------------------------------------------------------------------*/

METHOD IdeBrowseManager:showStruct()

   IF empty( ::qStruct )
      ::buildUiStruct()
   ENDIF

   IF ! ::lStructOpen
      ::lStructOpen := .t.
      ::populateUiStruct()
      ::oIde:setPosAndSizeByIniEx( ::qStruct:oWidget, ::oINI:cDbStructDialogGeometry )
      ::qStruct:show()
   ENDIF

   RETURN Self

/*----------------------------------------------------------------------*/

FUNCTION hbide_fldType2Desc( cType )

   SWITCH cType
   CASE "C" ; RETURN "Character"
   CASE "N" ; RETURN "Numeric"
   CASE "D" ; RETURN "Date"
   CASE "L" ; RETURN "Logical"
   ENDSWITCH

   RETURN ""

/*----------------------------------------------------------------------*/

METHOD IdeBrowseManager:populateFieldData()
   LOCAL nRow, qItm

   IF ( nRow := ::qStruct:q_tableFields:currentRow() ) >= 0
      qItm := QTableWidgetItem():from( ::qStruct:q_tableFields:item( nRow, 1 ) )
      ::qStruct:q_editName:setText( qItm:text() )
      qItm := QTableWidgetItem():from( ::qStruct:q_tableFields:item( nRow, 2 ) )
      ::qStruct:q_comboType:setCurrentIndex( ascan( {"Character", "Numeric", "Date", "Logical" }, qItm:text() ) - 1 )
      qItm := QTableWidgetItem():from( ::qStruct:q_tableFields:item( nRow, 3 ) )
      ::qStruct:q_editSize:setText( qItm:text() )
      qItm := QTableWidgetItem():from( ::qStruct:q_tableFields:item( nRow, 4 ) )
      ::qStruct:q_editDec:setText( qItm:text() )
   ENDIF

   RETURN Self

/*----------------------------------------------------------------------*/

METHOD IdeBrowseManager:populateUiStruct()
   LOCAL qItm, fld_, n
   LOCAL oTbl := ::qStruct:q_tableFields
   LOCAL aStruct := ::oCurBrw:dbStruct()

   ::qStruct:q_tableFields:clearContents()

   oTbl:setRowCount( len( aStruct ) )

   n := 0
   FOR EACH fld_ IN aStruct
      qItm := QTableWidgetItem():new()
      qItm:setText( hb_ntos( n+1 ) )
      oTbl:setItem( n, 0, qItm )

      qItm := QTableWidgetItem():new()
      qItm:setText( fld_[ 1 ] )
      oTbl:setItem( n, 1, qItm )

      qItm := QTableWidgetItem():new()
      qItm:setText( hbide_fldType2Desc( fld_[ 2 ] )  )
      oTbl:setItem( n, 2, qItm )

      qItm := QTableWidgetItem():new()
      qItm:setText( hb_ntos( fld_[ 3 ] ) )
      oTbl:setItem( n, 3, qItm )

      qItm := QTableWidgetItem():new()
      qItm:setText( hb_ntos( fld_[ 4 ] ) )
      oTbl:setItem( n, 4, qItm )

      oTbl:setRowHeight( n, 20 )
      n++
   NEXT

   n := 0
   aeval( aStruct, {|e_| n += e_[ 3 ] } )

   ::qStruct:q_labelRecSize:setText( hb_ntos( n + 1 ) )

   oTbl:setCurrentCell( 0,0 )

   RETURN Self

/*----------------------------------------------------------------------*/

METHOD IdeBrowseManager:buildUiStruct()
   LOCAL oTbl, n, qItm
   LOCAL hdr_:= { { "", 50 }, { "Field Name",200 }, { "Type", 100 }, { "Len", 50 }, { "Dec", 70 } }

   ::qStruct := hbide_getUI( "dbstruct", ::qDbu )

   ::qStruct:setWindowFlags( Qt_Dialog )
   ::qStruct:setMaximumHeight( ::qStruct:height() )
   ::qStruct:setMinimumHeight( ::qStruct:height() )
   ::qStruct:setMinimumWidth( ::qStruct:width() )
   ::qStruct:setMaximumWidth( ::qStruct:width() )

   ::qStruct:installEventFilter( ::pEvents )
   ::connect( ::qStruct, QEvent_Close, {|| ::execEvent( "dbStruct_closeEvent" ) } )

   oTbl := ::qStruct:q_tableFields
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
   NEXT

   ::qStruct:q_comboType:addItem( "Character" )
   ::qStruct:q_comboType:addItem( "Numeric"   )
   ::qStruct:q_comboType:addItem( "Date"      )
   ::qStruct:q_comboType:addItem( "Logical"   )

   ::connect( oTbl, "itemSelectionChanged()", {|| ::execEvent( "fieldsTable_itemSelectionChanged" ) } )
   RETURN Self

/*----------------------------------------------------------------------*/

METHOD IdeBrowseManager:loadTables()
   LOCAL cInfo, aInfo, oCurPanel

   oCurPanel := ::oCurPanel

   FOR EACH cInfo IN ::oINI:aDbuPanelsInfo
      aInfo := hb_aTokens( cInfo, "," )
      IF ::isPanel( aInfo[ 1 ] )
         ::setPanel( aInfo[ 1 ] )
         ::oCurPanel:addBrowser( aInfo )
      ENDIF
   NEXT

   ::qStack:setCurrentWidget( oCurPanel )
   RETURN Self

/*----------------------------------------------------------------------*/

METHOD IdeBrowseManager:addArray( aData, aAttr )

   HB_SYMBOL_UNUSED( aData )
   HB_SYMBOL_UNUSED( aAttr )

   RETURN Self

/*----------------------------------------------------------------------*/

METHOD IdeBrowseManager:buildToolbar()
   LOCAL nW := 25

   STATIC sp0,sp1,sp2,sp3

   IF empty( sp0 )
      sp0 := QLabel():new(); sp0:setMinimumWidth( nW )
      sp1 := QLabel():new(); sp1:setMinimumWidth( nW )
      sp2 := QLabel():new(); sp2:setMinimumWidth( nW )
      sp3 := QLabel():new(); sp3:setMinimumWidth( nW )
   ENDIF

   ::qToolbar := QToolbar():new()
   ::qToolbar:setIconSize( QSize():new( 16,16 ) )
   ::qToolbar:setStyleSheet( GetStyleSheet( "QToolBar", ::nAnimantionMode ) )

   ::buildPanelsButton()
   ::qToolbar:addWidget( sp0 )
   ::buildRddsCombo()
   ::buildConxnCombo()
   ::buildToolButton( ::qToolbar, { "Open a table"       , "open"          , "clicked()", {|| ::execEvent( "buttonOpen_clicked"          ) }, .f. } )
   ::qToolbar:addWidget( sp1 )
   ::buildToolButton( ::qToolbar, { "Toggle tabbed view" , "view_tabbed"   , "clicked()", {|| ::execEvent( "buttonViewTabbed_clicked"    ) }, .f. } )
   ::buildToolButton( ::qToolbar, {} )
   ::buildToolButton( ::qToolbar, { "View as arranged"   , "view_organized", "clicked()", {|| ::execEvent( "buttonViewOrganized_clicked" ) }, .f. } )
   ::buildToolButton( ::qToolbar, { "View as cascaded"   , "view_cascaded" , "clicked()", {|| ::execEvent( "buttonViewCascaded_clicked"  ) }, .f. } )
   ::buildToolButton( ::qToolbar, { "View as tiled"      , "view_tiled"    , "clicked()", {|| ::execEvent( "buttonViewTiled_clicked"     ) }, .f. } )
   ::buildToolButton( ::qToolbar, {} )
   ::buildToolButton( ::qToolbar, { "Save layout"        , "save"          , "clicked()", {|| ::execEvent( "buttonSaveLayout_clicked"    ) }, .f. } )
   ::qToolbar:addWidget( sp2 )
   ::buildToolButton( ::qToolbar, { "Show/hide form view", "formview"      , "clicked()", {|| ::execEvent( "buttonShowForm_clicked"      ) }, .t. } )
   ::buildToolButton( ::qToolbar, {} )
   ::buildToolButton( ::qToolbar, { "Table Structure"    , "dbstruct"      , "clicked()", {|| ::execEvent( "buttonDbStruct_clicked"      ) }, .f. } )
   ::buildToolButton( ::qToolbar, {} )
   ::buildIndexButton()
   ::buildToolButton( ::qToolbar, { "Search in table"    , "find"          , "clicked()", {|| ::execEvent( "buttonFind_clicked"          ) }, .f. } )
   ::buildToolButton( ::qToolbar, { "Goto record"        , "gotoline"      , "clicked()", {|| ::execEvent( "buttonGoto_clicked"          ) }, .f. } )
   ::buildToolButton( ::qToolbar, {} )
   ::buildToolButton( ::qToolbar, { "Close current table", "dc_delete"     , "clicked()", {|| ::execEvent( "buttonClose_clicked"         ) }, .f. } )
   ::qToolbar:addWidget( sp3 )
   ::buildTablesButton()

   RETURN Self

/*----------------------------------------------------------------------*/

METHOD IdeBrowseManager:buildLeftToolbar()

   STATIC qSize

   //qSize := QSize():new( 20,20 )
   qSize := QSize():new( 16,16 )

   ::qToolBarL := QToolbar():new()
   ::qToolBarL:setOrientation( Qt_Vertical )
   ::qToolbarL:setIconSize( qSize )
   ::qToolbarL:setMaximumWidth( 24 )
   ::qToolbarL:setStyleSheet( GetStyleSheet( "QToolBar", ::nAnimantionMode ) )

   ::buildToolButton( ::qToolbarL, { "Append a record"       , "database_add"     , "clicked()", {|| ::execEvent( "buttonAppendRecord_clicked"  ) }, .f. } )
   ::buildToolButton( ::qToolbarL, { "Delete a record"       , "database_remove"  , "clicked()", {|| ::execEvent( "buttonDelRecord_clicked"     ) }, .f. } )
   ::buildToolButton( ::qToolbarL, { "Lock/Unlock Record"    , "database_lock"    , "clicked()", {|| ::execEvent( "buttonLockRecord_clicked"    ) }, .f. } )
   ::buildToolButton( ::qToolbarL, {} )
   ::buildToolButton( ::qToolbarL, { "Goto Top"              , "database_up"      , "clicked()", {|| ::execEvent( "buttonGoTop_clicked"         ) }, .f. } )
   ::buildToolButton( ::qToolbarL, { "Goto Bottom"           , "database_down"    , "clicked()", {|| ::execEvent( "buttonGoBottom_clicked"      ) }, .f. } )
   ::buildToolButton( ::qToolbarL, { "Scroll to First Column", "database_previous", "clicked()", {|| ::execEvent( "buttonScrollToFirst_clicked" ) }, .f. } )
   ::buildToolButton( ::qToolbarL, { "Scroll to Last Column" , "database_next"    , "clicked()", {|| ::execEvent( "buttonScrollToLast_clicked"  ) }, .f. } )
   ::buildToolButton( ::qToolbarL, {} )
   ::buildToolButton( ::qToolbarL, { "Search in Table"       , "database_search"  , "clicked()", {|| ::execEvent( "buttonSearchInTable_clicked" ) }, .f. } )
   ::buildToolButton( ::qToolbarL, {} )
   ::buildToolButton( ::qToolbarL, { "Zap Table"             , "database_process" , "clicked()", {|| ::execEvent( "buttonZaptable_clicked"      ) }, .f. } )

   RETURN Self

/*------------------------------------------------------------------------*/

METHOD IdeBrowseManager:buildToolButton( qToolbar, aBtn )
   LOCAL qBtn

   IF empty( aBtn )
      qToolbar:addSeparator()
   ELSE
      qBtn := QToolButton():new()
      qBtn:setTooltip( aBtn[ 1 ] )
      qBtn:setAutoRaise( .t. )
      qBtn:setIcon( hbide_image( aBtn[ 2 ] ) )
      IF aBtn[ 5 ]
         qBtn:setCheckable( .t. )
      ENDIF
      ::connect( qBtn, aBtn[ 3 ],  aBtn[ 4 ] )
      qToolBar:addWidget( qBtn )
      aadd( ::aToolBtns, qBtn )
   ENDIF

   RETURN Self

/*----------------------------------------------------------------------*/

METHOD IdeBrowseManager:buildConxnCombo()

   ::qConxnCombo := QComboBox():new()
   ::qConxnCombo:setToolTip( "Connection to open next table" )
   ::qToolBar:addWidget( ::qConxnCombo )

   RETURN Self

/*----------------------------------------------------------------------*/

METHOD IdeBrowseManager:loadConxnCombo( cDriver )
   LOCAL aConxns, cConxn, a_

   DEFAULT cDriver TO ::currentDriver()

   ::aConxns := {}

   IF !empty( aConxns := hbide_execScriptFunction( "connections", cDriver ) )
      aeval( aConxns, {|e| aadd( ::aConxns, e ) } )
   ENDIF

   ::qConxnCombo:clear()
   FOR EACH cConxn IN ::aConxns
      a_:= hb_aTokens( cConxn, ";" )
      ::qConxnCombo:addItem( alltrim( a_[ 1 ] ) )
   NEXT

   RETURN Self

/*----------------------------------------------------------------------*/

METHOD IdeBrowseManager:buildRddsCombo()
   LOCAL aRdds, cRdd

   IF !empty( aRdds := hbide_execScriptFunction( "rdds" ) )
      aeval( aRdds, {|e| aadd( ::aRdds, e ) } )
   ENDIF

   ::qRddCombo := QComboBox():new()
   ::qRddCombo:setToolTip( "Rdd to open next table" )
   FOR EACH cRdd IN ::aRdds
      cRdd := alltrim( cRdd )
      ::qRddCombo:addItem( cRdd )
   NEXT
   ::qToolBar:addWidget( ::qRddCombo )

   ::connect( ::qRddCombo, "currentIndexChanged(QString)", {|p| ::loadConxnCombo( p ) } )

   RETURN Self

/*----------------------------------------------------------------------*/

METHOD IdeBrowseManager:buildTablesButton()

   ::qTablesMenu := QMenu():new()
   ::qTablesMenu:setStyleSheet( GetStyleSheet( "QMenuPop", ::nAnimantionMode ) )

   ::qTablesButton := QToolButton():new()
   ::qTablesButton:setTooltip( "Tables" )
   ::qTablesButton:setIcon( hbide_image( "database" ) )
   ::qTablesButton:setPopupMode( QToolButton_MenuButtonPopup )
   ::qTablesButton:setMenu( ::qTablesMenu )
   //::qTablesButton:setMaximumWidth( 0 )

   ::connect( ::qTablesButton, "clicked()", {|| ::execEvent( "buttonTables_clicked" ) } )

   ::qToolbar:addWidget( ::qTablesButton )

   RETURN Self

/*----------------------------------------------------------------------*/

METHOD IdeBrowseManager:buildIndexButton()

   ::qIndexMenu := QMenu():new()
   ::qIndexMenu:setStyleSheet( GetStyleSheet( "QMenuPop", ::nAnimantionMode ) )

   ::qIndexButton := QToolButton():new()
   ::qIndexButton:setTooltip( "Indexes" )
   ::qIndexButton:setIcon( hbide_image( "sort" ) )
   ::qIndexButton:setPopupMode( QToolButton_MenuButtonPopup )
   ::qIndexButton:setMenu( ::qIndexMenu )

   ::connect( ::qIndexButton, "clicked()", {|| ::execEvent( "buttonIndex_clicked" ) } )

   ::qToolbar:addWidget( ::qIndexButton )

   RETURN Self

/*----------------------------------------------------------------------*/

STATIC FUNCTION hbide_getMenuBlock( oPanel, oBrw, cIndex )
   RETURN {|| oPanel:setIndex( oBrw, cIndex ) }

/*----------------------------------------------------------------------*/

METHOD IdeBrowseManager:updateIndexMenu( oBrw )
   LOCAL qAct, aIndex, cIndex

   FOR EACH qAct IN ::aIndexAct
      ::disconnect( qAct, "triggered(bool)" )
      qAct := NIL
   NEXT
   ::aIndexAct := {}

   ::qIndexMenu:clear()

   aIndex := ::oCurPanel:getIndexInfo( oBrw )
   FOR EACH cIndex IN aIndex
      qAct := QAction():from( ::qIndexMenu:addAction( cIndex ) )
      ::connect( qAct, "triggered(bool)", hbide_getMenuBlock( ::oCurPanel, oBrw, cIndex ) )
      aadd( ::aIndexAct, qAct )
   NEXT

   RETURN Self

/*----------------------------------------------------------------------*/

METHOD IdeBrowseManager:buildPanelsButton()

   ::qPanelsMenu := QMenu():new()
   ::qPanelsMenu:setStyleSheet( GetStyleSheet( "QMenuPop", ::nAnimantionMode ) )

   ::qPanelsButton := QToolButton():new()
   ::qPanelsButton:setTooltip( "ideDBU Panels" )
   ::qPanelsButton:setIcon( hbide_image( "panel_8" ) )
   ::qPanelsButton:setPopupMode( QToolButton_MenuButtonPopup )
   ::qPanelsButton:setMenu( ::qPanelsMenu )

   ::connect( ::qPanelsButton, "clicked()", {|| ::execEvent( "qPanelsButton_clicked" ) } )

   ::qToolbar:addWidget( ::qPanelsButton )

   RETURN Self

/*----------------------------------------------------------------------*/
//
//                         Class IdeBrowsePanel
//
/*----------------------------------------------------------------------*/

CLASS IdeBrowsePanel INHERIT IdeObject

   DATA   oManager

   DATA   qWidget
   DATA   qMenuWindows
   DATA   qStruct

   DATA   cPanel                                  INIT  ""
   DATA   nViewStyle                              INIT  0    /* 0=asWindows 1=tabbed */
   DATA   lLayoutLocked                           INIT  .f.

   DATA   aBrowsers                               INIT  {}
   ACCESS subWindows()                            INLINE ::aBrowsers

   METHOD new( oIde, cPanel, oManager )
   METHOD destroy()
   METHOD destroyBrw( oBrw )
   METHOD execEvent( cEvent, p )
   METHOD setCurrentBrowser( oBrw )
   METHOD getIndexInfo( oBrw )
   METHOD setIndex( oBrw, cIndex )

   METHOD addBrowser( aInfo )
   METHOD prepare()
   METHOD saveGeometry()
   METHOD restGeometry()
   METHOD activateBrowser()

   METHOD viewMode()                              INLINE ::qWidget:viewMode()
   METHOD setViewMode( nMode )                    INLINE ::qWidget:setViewMode( nMode )
   METHOD tileSubWindows()                        INLINE ::qWidget:tileSubWindows()
   METHOD cascadeSubWindows()                     INLINE ::qWidget:cascadeSubWindows()
   METHOD activateNextSubWindow()                 INLINE ::qWidget:activateNextSubWindow()
   METHOD activatePreviousSubWindow()             INLINE ::qWidget:activatePreviousSubWindow()

   #if 0
   closeActiveSubWindow()
   closeAllSubWindows()
   setActiveSubWindow( QMdiSubWindow )
   #endif

   ENDCLASS

/*----------------------------------------------------------------------*/

METHOD IdeBrowsePanel:new( oIde, cPanel, oManager )

   ::oIde  := oIde
   ::cPanel := cPanel
   ::oManager := oManager

   ::qWidget := QMdiArea():new()
   ::qWidget:setObjectName( ::cPanel )
   ::qWidget:setDocumentMode( .t. )
   ::qWidget:setOption( QMdiArea_DontMaximizeSubWindowOnActivation, .t. )

   ::connect( ::qWidget, "subWindowActivated(QMdiSubWindow)", {|p| ::execEvent( "mdiArea_subWindowActivated", p ) } )

   RETURN Self

/*----------------------------------------------------------------------*/

METHOD IdeBrowsePanel:destroy()
   LOCAL aBrw, oSub

   FOR EACH aBrw IN ::aBrowsers
      oSub := aBrw[ SUB_WINDOW ]
      ::qWidget:removeSubWindow( oSub )
      aBrw[ SUB_BROWSER ]:destroy()
      oSub := NIL
   NEXT

   RETURN Self

/*----------------------------------------------------------------------*/

METHOD IdeBrowsePanel:destroyBrw( oBrw )
   LOCAL n, oSub

   IF ( n := ascan( ::aBrowsers, {|e_| e_[ SUB_BROWSER ] == oBrw } ) )  > 0
      oSub := ::aBrowsers[ n, SUB_WINDOW ]

      ::qWidget:removeSubWindow( oSub )
      oBrw:destroy()
      oSub := NIL

      hb_adel( ::aBrowsers, n, .t. )
   ENDIF

   RETURN Self

/*------------------------------------------------------------------------*/

METHOD IdeBrowsePanel:execEvent( cEvent, p )
   LOCAL n, oBrw

   SWITCH cEvent
   CASE "mdiArea_subWindowActivated"
      IF ! empty( ::aBrowsers )
         IF ( n := ascan( ::aBrowsers, {|e_| hbqt_IsEqualGcQtPointer( e_[ SUB_WINDOW ]:pPtr, p ) } ) )  > 0
            oBrw := ::aBrowsers[ n, SUB_BROWSER ]

            oBrw:configure()
            oBrw:oBrw:setCurrentIndex( .t. )
            oBrw:oBrw:setFocus()

            ::oManager:updateIndexMenu( oBrw )
         ENDIF
      ENDIF
      EXIT
   ENDSWITCH

   RETURN Self

/*------------------------------------------------------------------------*/

METHOD IdeBrowsePanel:setIndex( oBrw, cIndex )
   IF ascan( ::aBrowsers, {|e_| e_[ SUB_BROWSER ] == oBrw } ) > 0
      RETURN oBrw:setIndex( cIndex )
   ENDIF
   RETURN Self

/*----------------------------------------------------------------------*/

METHOD IdeBrowsePanel:getIndexInfo( oBrw )
   IF ascan( ::aBrowsers, {|e_| e_[ SUB_BROWSER ] == oBrw } )  > 0
      RETURN oBrw:getIndexInfo()
   ENDIF
   RETURN {}

/*----------------------------------------------------------------------*/

METHOD IdeBrowsePanel:setCurrentBrowser( oBrw )
   IF ascan( ::aBrowsers, {|e_| e_[ SUB_BROWSER ] == oBrw } )  > 0
      ::oManager:oCurBrw := oBrw
   ENDIF
   RETURN Self

/*------------------------------------------------------------------------*/

METHOD IdeBrowsePanel:prepare()
   LOCAL aSub
   FOR EACH aSub IN ::aBrowsers
      aSub[ SUB_BROWSER ]:configure()
   NEXT
   RETURN Self

/*------------------------------------------------------------------------*/

METHOD IdeBrowsePanel:addBrowser( aInfo )
   LOCAL oBrw
   oBrw := IdeBrowse():new( ::oIde, ::oManager, Self, aInfo ):create()
   IF empty( oBrw:oBrw )
      RETURN Self
   ENDIF
   aadd( ::aBrowsers, { oBrw:nID, oBrw:qMdi, QRect():from( oBrw:qMdi:geometry() ), oBrw, NIL } )
   ::oManager:updateIndexMenu( oBrw )
   RETURN Self

/*------------------------------------------------------------------------*/

METHOD IdeBrowsePanel:activateBrowser()
   IF len( ::aBrowsers ) > 0
      ::qWidget:setActiveSubWindow( ::aBrowsers[ 1, SUB_WINDOW ] )
   ENDIF
   RETURN Self

/*----------------------------------------------------------------------*/

METHOD IdeBrowsePanel:saveGeometry()
   LOCAL a_
   IF ::nViewStyle == 0                 /* Only if in self organized mode */
      FOR EACH a_ IN ::aBrowsers
         a_[ SUB_GEOMETRY ] := QRect():from( a_[ SUB_WINDOW ]:geometry() )
      NEXT
   ENDIF
   RETURN Self

/*------------------------------------------------------------------------*/

METHOD IdeBrowsePanel:restGeometry()
   LOCAL a_
   IF ::nViewStyle == 0
      FOR EACH a_ IN ::aBrowsers
         IF hb_isObject( a_[ SUB_GEOMETRY ] )
            a_[ SUB_WINDOW ]:setGeometry( a_[ SUB_GEOMETRY ] )
         ENDIF
      NEXT
   ENDIF
   RETURN Self

/*----------------------------------------------------------------------*/
//
//                            Class IdeBrowse
//
/*----------------------------------------------------------------------*/

CLASS IdeBrowse INHERIT IdeObject

   DATA   oWnd
   DATA   oBrw
   DATA   qLayout
   DATA   qForm
   DATA   qFLayout
   DATA   qSplitter
   DATA   qTimer
   DATA   qStatus
   DATA   qScrollArea

   DATA   nID                                     INIT  0

   DATA   aForm                                   INIT  {}
   DATA   oManager
   DATA   oPanel
   DATA   qMDI
   DATA   aInfo                                   INIT  {}

   DATA   nType                                   INIT  BRW_TYPE_DBF
   DATA   cAlias                                  INIT  ""
   DATA   cTable                                  INIT  ""
   DATA   cTableOnly                              INIT  ""
   DATA   aData                                   INIT  {}
   DATA   aStruct                                 INIT  {}
   DATA   aAttr                                   INIT  {}
   DATA   nIndex                                  INIT  0
   DATA   cDriver                                 INIT  "DBFCDX"
   DATA   cConxn                                  INIT  ""
   DATA   cConxnFull                              INIT  ""
   DATA   cIndex                                  INIT  ""
   DATA   nOrder                                  INIT  0
   DATA   nArea                                   INIT  0
   DATA   nCursorType                             INIT  XBPBRW_CURSOR_CELL
   DATA   lOpened                                 INIT  .f.

   DATA   qVerSpl
   DATA   qClose
   DATA   aIndex                                  INIT  {}

   DATA   xSearch
   DATA   lInSearch                               INIT  .f.

   DATA   aMenu                                   INIT  {}
   DATA   aIdx                                    INIT  {}
   DATA   aFlds                                   INIT  {}
   DATA   aSeek                                   INIT  {}
   DATA   aToFld                                  INIT  {}

   METHOD new( oIde, oManager, oPanel, aInfo )
   METHOD create( oIde, oManager, oPanel, aInfo )
   METHOD configure()
   METHOD destroy()
   METHOD execEvent( cEvent, p, p1 )
   METHOD buildBrowser()
   METHOD buildColumns()
   METHOD buildMdiWindow()
   METHOD dataLink( nField )
   METHOD getPP( aStruct )

   METHOD skipBlock( nHowMany )

   METHOD use()
   METHOD exists()
   METHOD goTop()
   METHOD goBottom()
   METHOD goTo( nRec )
   METHOD lock()
   METHOD goToAsk()
   METHOD append()
   METHOD delete( lAsk )
   METHOD recall()
   METHOD recNo()
   METHOD lastRec()
   ACCESS dbStruct()                              INLINE ::aStruct
   METHOD indexOrd()
   METHOD ordName( nOrder )
   METHOD IndexKey( nOrder )
   METHOD IndexKeyValue( nOrder )
   METHOD refreshAll()
   METHOD getIndexInfo()
   METHOD setIndex( cIndex )
   METHOD setOrder( nOrder )
   ACCESS numIndexes()                            INLINE len( ::aIndex )

   METHOD dispInfo()
   METHOD search( cSearch, lSoft, lLast, nMode )
   METHOD searchAsk( nMode )
   METHOD seekAsk( nMode )
   METHOD next()
   METHOD previous()
   METHOD buildForm()
   METHOD populateForm()
   METHOD fetchAlias( cTable )
   METHOD saveField( nField, x )
   METHOD toColumn( ncIndex )
   METHOD getSome( cType, cFor )
   METHOD buildContextMenu()

   ENDCLASS

/*----------------------------------------------------------------------*/

METHOD IdeBrowse:new( oIde, oManager, oPanel, aInfo )

   ::oIde     := oIde
   ::oManager := oManager
   ::oPanel   := oPanel
   ::aInfo    := aInfo

   RETURN Self

/*----------------------------------------------------------------------*/

METHOD IdeBrowse:create( oIde, oManager, oPanel, aInfo )
   LOCAL xVrb, cT, cName, n
   LOCAL lMissing := .t.

   DEFAULT oIde     TO ::oIde
   DEFAULT oManager TO ::oManager
   DEFAULT oPanel   TO ::oPanel
   DEFAULT aInfo    TO ::aInfo
   ::oIde     := oIde
   ::oManager := oManager
   ::oPanel   := oPanel
   ::aInfo    := aInfo

   aSize( ::aInfo, TBL_VRBLS )

   DEFAULT ::aInfo[ TBL_PANEL    ] TO ::oPanel:cPanel
   DEFAULT ::aInfo[ TBL_NAME     ] TO ""
   DEFAULT ::aInfo[ TBL_ALIAS    ] TO ""
   DEFAULT ::aInfo[ TBL_DRIVER   ] TO ::oManager:currentDriver()
   DEFAULT ::aInfo[ TBL_INDEX    ] TO ""
   DEFAULT ::aInfo[ TBL_RECORD   ] TO ""
   DEFAULT ::aInfo[ TBL_CURSOR   ] TO ""
   DEFAULT ::aInfo[ TBL_GEOMETRY ] TO ""
   DEFAULT ::aInfo[ TBL_ROWPOS   ] TO "1"
   DEFAULT ::aInfo[ TBL_COLPOS   ] TO "1"
   DEFAULT ::aInfo[ TBL_HZSCROLL ] TO ""
   DEFAULT ::aInfo[ TBL_CONXN    ] TO ::oManager:currentConxn()
   DEFAULT ::aInfo[ TBL_NEXT     ] TO ""

   ::cTable := hbide_pathToOSPath( ::aInfo[ TBL_NAME ] )
   hb_fNameSplit( ::cTable, , @cName )
   ::cTableOnly := cName
   ::cAlias     := ::aInfo[ TBL_ALIAS  ]
   ::cDriver    := ::aInfo[ TBL_DRIVER ]
   ::cConxn     := ::aInfo[ TBL_CONXN  ]

   IF ! ::exists()
      RETURN Self
   ENDIF

   IF !empty( ::oManager:aConxns )
      n := ascan( ::oManager:aConxns, {|e| e = ::cConxn } )
      ::cConxnFull := ::oManager:aConxns[ n ]
   ENDIF

   IF ::nType == BRW_TYPE_DBF
      IF !empty( ::cAlias ) .AND. empty( ::cTable )
         IF select( ::cAlias ) > 0
            lMissing := .f.
         ENDIF
      ENDIF

      IF lMissing .AND. !empty( ::cTable )
         IF ! ( ::lOpened := ::use() )
            RETURN Self
         ENDIF
      ENDIF

      ::aStruct := ( ::cAlias )->( DbStruct() )
   ELSE
      FOR EACH xVrb IN ::aData[ 1 ]
         cT := valtype( xVrb )
         aadd( ::aStruct, "Fld_" + hb_ntos( xVrb:__enumIndex() ) )
         aadd( ::aStruct, cT )
         IF cT == "N"
            aadd( ::aStruct, 12 )
            aadd( ::aStruct,  2 )
         ELSEIF cT == "D"
            aadd( ::aStruct,  8 )
            aadd( ::aStruct,  0 )
         ELSEIF cT == "L"
            aadd( ::aStruct,  1 )
            aadd( ::aStruct,  0 )
         ELSE
            aadd( ::aStruct, len( xVrb ) )
            aadd( ::aStruct,  0 )
         ENDIF
      NEXT
   ENDIF

   ::buildBrowser()
   ::buildColumns()
   ::buildForm()
   ::buildMdiWindow()

   ::oManager:oCurBrw := Self

   ::oBrw:configure()
   ::oBrw:forceStable()
   ::oBrw:rowPos := max( 1, val( aInfo[ TBL_ROWPOS ] ) )
   ::oBrw:colPos := max( 1, val( aInfo[ TBL_COLPOS ] ) )
   ::oBrw:forceStable()
   ::setOrder( val( aInfo[ TBL_INDEX ] ) )
   ::goto( max( 1, val( aInfo[ TBL_RECORD ] ) ) )
   ::oBrw:setCurrentIndex( .t. )

   ::oBrw:navigate := {|mp1,mp2| ::execEvent( "browse_navigate", mp1, mp2 ) }
   ::oBrw:keyboard := {|mp1,mp2| ::execEvent( "browse_keyboard", mp1, mp2 ) }

   ::qTimer := QTimer():new()
   ::qTimer:setInterval( 5 )
   ::connect( ::qTimer, "timeout()",  {|| ::execEvent( "timer_timeout" ) } )

   RETURN Self

/*----------------------------------------------------------------------*/

METHOD IdeBrowse:destroy()

   IF ! empty( ::qMdi )
   *  ::disconnect( ::qMdi, "aboutToActivate()" )
      ::disconnect( ::qMdi, "windowStateChanged(Qt::WindowStates,Qt::WindowStates)" )
      ::disconnect( ::qMdi, QEvent_Close )
   ENDIF

   IF ! empty( ::oWnd )
      ::qLayout:removeWidget( ::qSplitter )
      ::oWnd:destroy()
      ::qForm := NIL
      IF ::lOpened
         ( ::cAlias )->( dbCloseArea() )
      ENDIF
      ::oManager:oCurBrw := NIL
   ENDIF
   RETURN Self

/*----------------------------------------------------------------------*/

METHOD IdeBrowse:buildBrowser()
   LOCAL qLayout, oWnd, oXbpBrowse

   oWnd := XbpWindow():new()
   oWnd:oWidget := QWidget():new()

   qLayout := QHBoxLayout():new()
   oWnd:oWidget:setLayout( qLayout )
   qLayout:setContentsMargins( 0,0,0,0 )
   qLayout:setSpacing( 2 )

   ::qSplitter := QSplitter():new()
   ::qSplitter:setOrientation( Qt_Horizontal )

   qLayout:addWidget( ::qSplitter )

   /* Browse View */
   oXbpBrowse := XbpBrowse():new():create( oWnd, , { 0,0 }, oWnd:currentSize() )
   oXbpBrowse:setFontCompoundName( "10.Courier" )

   ::qSplitter:addWidget( oXbpBrowse:oWidget )

   oXbpBrowse:cursorMode    := ::nCursorType

   oXbpBrowse:skipBlock     := {|n| ::skipBlock( n ) }
   oXbpBrowse:goTopBlock    := {| | ::goTop()        }
   oXbpBrowse:goBottomBlock := {| | ::goBottom()     }
   //
   oXbpBrowse:firstPosBlock := {| | 1                }
   oXbpBrowse:lastPosBlock  := {| | ::lastRec()      }

   oXbpBrowse:posBlock      := {| | ::recNo()        }
   oXbpBrowse:goPosBlock    := {|n| ::goto( n )      }
   oXbpBrowse:phyPosBlock   := {| | ::recNo()        }

   oXbpBrowse:hbContextMenu := {|mp1| ::execEvent( "browser_contextMenu", mp1 ) }

   /* Form View */
   ::qForm := QWidget():new()
   ::qForm:setMinimumSize( QSize():new( 300  , len( ::aStruct ) * 34 ) )
   ::qForm:setMaximumSize( QSize():new( 12000, 48000 ) )

   ::qFLayout := QFormLayout():new()
   ::qForm:setLayout( ::qFLayout )

   ::qScrollArea := QScrollArea():new()
   ::qScrollArea:setWidget( ::qForm )
   ::qScrollArea:hide()

   ::qSplitter:addWidget( ::qScrollArea )

   ::qLayout := qLayout
   ::oWnd    := oWnd
   ::oBrw    := oXbpBrowse

#if 0
   ::qVerSpl := QSplitter():new( Qt_Vertical )
   ::qSplitter:addWidget( ::qVerSpl )

   ::qVerSpl:addWidget( ::qForm )

   ::qClose := QToolButton():new()
   ::qClose:setIcon( hbide_image( "closetab" ) )
   ::qClose:hide()

   ::qVerSpl:addWidget( ::qClose )
#endif
   RETURN Self

/*----------------------------------------------------------------------*/

METHOD IdeBrowse:buildColumns()
   LOCAL oXbpColumn, aPresParam, a_

   IF ::nType == BRW_TYPE_DBF
      FOR EACH a_ IN ::aStruct
         aPresParam := ::getPP( a_ )

         oXbpColumn          := XbpColumn():new()
         oXbpColumn:dataLink := ::dataLink( a_:__enumIndex() )
         oXbpColumn:create( , , , , aPresParam )

         ::oBrw:addColumn( oXbpColumn )
      NEXT
   ELSE
      FOR EACH a_ IN ::aStruct
         ::getPP( a_, a_:__enumIndex() )

         oXbpColumn          := XbpColumn():new()
         oXbpColumn:dataLink := ::dataLink( a_:__enumIndex() )
         oXbpColumn:create( , , , , aPresParam )

         ::oBrw:addColumn( oXbpColumn )
      NEXT
   ENDIF

   RETURN Self

/*----------------------------------------------------------------------*/

METHOD IdeBrowse:buildForm()
   LOCAL a_, qLbl, qEdit

   IF ::nType == BRW_TYPE_DBF
      FOR EACH a_ IN ::aStruct
         qLbl := QLabel():new(); qLbl:setText( a_[ 1 ] )
         qEdit := QLineEdit():new()
         ::qFLayout:addRow( qLbl, qEdit )
         aadd( ::aForm, { qLbl, qEdit } )
      NEXT
   ELSE
   ENDIF
   RETURN Self

/*----------------------------------------------------------------------*/

METHOD IdeBrowse:buildMdiWindow()
   LOCAL qRect, cR

   STATIC nID := 0

   ::nID := ++nID

   ::qMdi := QMdiSubWindow():new( ::oDlg:oWidget )
   //
   ::qMdi:setWidget( ::oWnd:oWidget )
   ::oPanel:qWidget:addSubWindow( ::qMdi )

   ::oWnd:oWidget:show()
   ::qMdi:show()

   ::qMdi:setWindowTitle( ::cTable )
   ::qMdi:setObjectName( hb_ntos( nID ) )

   IF ! empty( ::aInfo[ TBL_GEOMETRY ] )
      qRect := hb_aTokens( ::aInfo[ TBL_GEOMETRY ], " " )
      FOR EACH cR IN qRect
         cR := val( cR )
      NEXT
      qRect := QRect():new( qRect[ 1 ], qRect[ 2 ], qRect[ 3 ], qRect[ 4 ] )
      ::qMdi:setGeometry( qRect )
      ::qMdi:resize( ::qMdi:width()+1, ::qMdi:height()+1 )
      ::qMdi:resize( ::qMdi:width()-1, ::qMdi:height()-1 )
   ELSE
      ::qMdi:resize( 300, 200 )
   ENDIF
   ::dispInfo()

 * ::connect( ::qMdi, "aboutToActivate()", {|| ::execEvent( "mdiSubWindow_aboutToActivate" ) } )
   ::connect( ::qMdi, "windowStateChanged(Qt::WindowStates,Qt::WindowStates)", ;
                                 {|p,p1| ::execEvent( "mdiSubWindow_windowStateChanged", p, p1 ) } )
   ::qMdi:installEventFilter( ::pEvents )
   ::connect( ::qMdi, QEvent_Close, {|| ::execEvent( "mdiSubWindow_buttonXclicked" ) } )

   RETURN Self

/*------------------------------------------------------------------------*/

METHOD IdeBrowse:configure()
   LOCAL nOff
   LOCAL nRowPos := ::oBrw:rowPos()
   LOCAL nColPos := ::oBrw:colPos()

   ::oBrw:configure()

   IF nRowPos > ::oBrw:rowCount()
      nOff := nRowPos - ::oBrw:rowCount()
      ::oBrw:rowPos := ::oBrw:rowCount()
   ELSE
      nOff := 0
   ENDIF
   ::oBrw:colPos := nColPos

   ::oBrw:refreshAll()
   ::oBrw:forceStable()
   ::oBrw:setCurrentIndex( nRowPos > ::oBrw:rowCount() )
   IF nOff > 0
      SetAppEvent( xbeBRW_Navigate, XBPBRW_Navigate_Skip, nOff, ::oBrw )
   ENDIF

   RETURN Self

/*------------------------------------------------------------------------*/

METHOD IdeBrowse:execEvent( cEvent, p, p1 )

   HB_SYMBOL_UNUSED( p  )
   HB_SYMBOL_UNUSED( p1 )

   SWITCH cEvent
   CASE "browse_navigate"
      ::dispInfo()
      ::populateForm()
      ::oManager:oCurBrw := Self
      ::oManager:aToolBtns[ 3 ]:setChecked( ! ::qForm:isHidden() )
      EXIT

   CASE "browse_keyboard"
      IF p == xbeK_CTRL_F
         ::searchAsk()

      ELSEIF p == xbeK_CTRL_G
         ::gotoAsk()

      ELSEIF p == xbeK_CTRL_E
         ::oBrw:oTableView:edit( ::oBrw:getCurrentIndex() )

      ENDIF
      EXIT

   CASE "timer_timeout"
      ::oBrw:down()
      IF ::oBrw:hitBottom
         ::qTimer:stop()
         ::dispInfo()
      ELSEIF eval( ::oBrw:getColumn( ::oBrw:colPos ):block ) = ::xSearch
         ::qTimer:stop()
         ::dispInfo()
      ENDIF
      EXIT

   CASE "mdiSubWindow_buttonXclicked"
      ::oPanel:destroyBrw( Self )
      EXIT

   CASE "mdiSubWindow_aboutToActivate"
      #if 0
      ::oBrw:configure()
      ::oBrw:setCurrentIndex( .t. )
      #endif
      EXIT

   CASE "mdiSubWindow_windowStateChanged"
      IF p1 == 8
         ::oPanel:setCurrentBrowser( Self )
      ENDIF
      EXIT

   CASE "browser_contextMenu"
      IF empty( ::aMenu )
         ::buildContextMenu()
      ENDIF
      hbide_execPopup( ::aMenu, p, ::qMdi )
      EXIT

   CASE "browser_ScrollToColumn"

      EXIT
   ENDSWITCH

   #if 0
   activateNextSubWindow()
   activatePreviousSubWindow()
   closeActiveSubWindow()
   closeAllSubWindows()
   setActiveSubWindow( QMdiSubWindow )
   #endif

   RETURN Self

/*----------------------------------------------------------------------*/

METHOD IdeBrowse:buildContextMenu()
   LOCAL a_, cPmt, nZeros, cIndex

   IF len( ::aIndex ) > 0
      aadd( ::aMenu, { "Set to Natural Order", {|| ::setOrder( 0 ) } } )
      aadd( ::aMenu, { "" } )
   ENDIF

   /* Indexed Order */
   ::getIndexInfo()
   FOR EACH cIndex IN ::aIndex
      aadd( ::aIdx,  hbide_indexArray( Self, cIndex, cIndex:__enumIndex() ) )
   NEXT
   IF ! empty( ::aIdx )
      aadd( ::aMenu, { ::aIdx, "Set to Indexed Order" } )
      aadd( ::aMenu, { "" } )
   ENDIF

   /* Column Scrolling */
   nZeros := iif( len( ::aStruct ) < 10, 1, iif( len( ::aStruct ) < 100, 2, 3 ) )
   FOR EACH a_ IN ::aStruct
      cPmt := strzero( a_:__enumIndex(), nZeros ) + " " + a_[ 2 ] + " . " + a_[ 1 ]
      aadd( ::aFlds, hbide_fieldsArray( Self, cPmt, a_:__enumIndex() ) )
   NEXT
   aadd( ::aMenu, { ::aFlds, "Scroll to Column" } )
   aadd( ::aMenu, { "Scroll to ...", {|v| v := ( QInputDialog():new() ):getText( , "Field Name" ), ::toColumn( v ) } } )
   aadd( ::aMenu, { "" } )

   /* Seeks */
   aadd( ::aSeek, { "Seek"       , {|| ::seekAsk( 0 ) } } )
   aadd( ::aSeek, { "Seek Soft"  , {|| ::seekAsk( 1 ) } } )
   aadd( ::aSeek, { "Seek Last"  , {|| ::seekAsk( 2 ) } } )
   aadd( ::aMenu, { ::aSeek, "Seek..." } )
   aadd( ::aMenu, { "Search in Field", {|| ::searchAsk( 1 ) } } )
   aadd( ::aMenu, { "" } )

   /* Navigation */
   aadd( ::aMenu, { "Go Top"     , {|| ::goTop()      } } )
   aadd( ::aMenu, { "Go Bottom"  , {|| ::goBottom()   } } )
   aadd( ::aMenu, { "Goto Record", {|| ::gotoAsk()    } } )
   aadd( ::aMenu, { "" } )

   /* Manipulation */
   aadd( ::aMenu, { "Append Blank"  , {|| ::append()  } } )
   aadd( ::aMenu, { "Delete Record" , {|| ::delete( .t. ) } } )
   aadd( ::aMenu, { "Recall Deleted", {|| ::recall()  } } )
   aadd( ::aMenu, { "" } )

   /* Miscellaneous */
   aadd( ::aMenu, { "Form View", {|| ::oManager:execEvent( "buttonShowForm_clicked"  ) } } )


   RETURN Self

/*----------------------------------------------------------------------*/

STATIC FUNCTION hbide_fieldsArray( obj, cPmt, nIndex )
   RETURN { cPmt, {|| obj:toColumn( nIndex ) } }

/*----------------------------------------------------------------------*/

STATIC FUNCTION hbide_indexArray( obj, cIndex, nOrder )
   RETURN { cIndex, {|| obj:setOrder( nOrder ) } }

/*----------------------------------------------------------------------*/

METHOD IdeBrowse:dispInfo()

   IF !empty( ::qMdi )
      ::qMdi:setTooltip( ::cTable )

      ::qMdi:setWindowTitle( "[ " + ::cDriver + "  " + ;
                             hb_ntos( ::indexOrd() ) + "/" + hb_ntos( ::numIndexes() ) + iif( ::indexOrd() > 0, ":" + ::ordName(), "" ) + ;
                             "  " + hb_ntos( ::recno() ) + "/" + hb_ntos( ::lastRec() ) + " ]  " + ;
                             ::cTableOnly )
   ENDIF
   RETURN Self

/*----------------------------------------------------------------------*/

METHOD IdeBrowse:fetchAlias( cTable )
   LOCAL cFile

   STATIC n := 0
   n++

   hb_fNameSplit( cTable, , @cFile )

   RETURN upper( "C" + cFile + hb_ntos( n ) )

/*------------------------------------------------------------------------*/

STATIC FUNCTION hbide_xtosForForm( xVrb )
   LOCAL cType := valtype( xVrb )

   DO CASE
   CASE cType == "N" ; RETURN ltrim( str( xVrb ) )
   CASE cType == "L" ; RETURN iif( xVrb, "YES", "NO" )
   CASE cType == "D" ; RETURN dtos( xVrb )
   CASE cType == "C" ; RETURN trim( xVrb )
   ENDCASE

   RETURN ""

/*----------------------------------------------------------------------*/

METHOD IdeBrowse:populateForm()
   LOCAL a_, oCol

   IF ::nType == BRW_TYPE_DBF

      FOR EACH a_ IN ::aForm
         oCol := ::oBrw:getColumn( a_:__enumIndex() )
         ::aForm[ a_:__enumIndex(), 2 ]:setText( hbide_xtosForForm( eval( oCol:block ) ) )
      NEXT
   ELSE

   ENDIF
   RETURN Self

/*----------------------------------------------------------------------*/

METHOD IdeBrowse:saveField( nField, x )
   IF ( ::cAlias )->( DbrLock() )
      ( ::cAlias )->( FieldPut( nField, x ) )
      ( ::cAlias )->( DbCommit() )
      ( ::cAlias )->( DbrUnlock() )
      ::oBrw:refreshCurrent()
      ::oBrw:forceStable()
      ::oBrw:SetCurrentIndex( .f. )
   ENDIF
   RETURN x

/*----------------------------------------------------------------------*/

METHOD IdeBrowse:dataLink( nField )
   LOCAL bBlock

   IF ::nType == BRW_TYPE_DBF
      bBlock := {|x| iif( x == NIL, ( ::cAlias )->( fieldget( nField ) ), ::saveField( nField, x ) ) }
   ELSE
      bBlock := {|| ::aData[ ::nIndex, nField ] }
   ENDIF

   RETURN bBlock

/*----------------------------------------------------------------------*/

METHOD IdeBrowse:getPP( aStruct )
   LOCAL aPresParam := {}

   aadd( aPresParam, { XBP_PP_COL_HA_CAPTION      , aStruct[ 1 ]  } )
   aadd( aPresParam, { XBP_PP_COL_DA_ROWHEIGHT    , 20            } )

   RETURN aPresParam

/*----------------------------------------------------------------------*/

METHOD IdeBrowse:skipBlock( nHowMany )
   LOCAL nRecs, nCurPos
   LOCAL nSkipped := 0

   IF ::nType == BRW_TYPE_DBF
      IF nHowMany == 0
         ( ::cAlias )->( DBSkip( 0 ) )

      ELSEIF nHowMany > 0
         DO WHILE nSkipped != nHowMany .AND. ::next()
            nSkipped++
         ENDDO
      ELSE
         DO WHILE nSkipped != nHowMany .AND. ::previous()
            nSkipped--
         ENDDO
      ENDIF

   ELSE
      nRecs    := len( ::aData )
      nCurPos  := ::nIndex

      IF nHowMany >= 0
         IF ( nCurpos + nHowMany ) > nRecs
            nSkipped := nRecs - nCurpos
            ::nIndex := nRecs
         ELSE
            nSkipped := nHowMany
            ::nIndex += nHowMany
         ENDIF

      ELSE
         IF ( nCurpos + nHowMany ) < 1
            nSkipped := 1 - nCurpos
            ::nIndex := 1
         ELSE
            nSkipped := nHowMany
            ::nIndex += nHowMany
         ENDIF

      ENDIF

   ENDIF

   RETURN nSkipped

/*----------------------------------------------------------------------*/

METHOD IdeBrowse:next()
   LOCAL nSaveRecNum := ( ::cAlias )->( recno() )
   LOCAL lMoved := .T.

   IF ( ::cAlias )->( Eof() )
      lMoved := .F.
   ELSE
      ( ::cAlias )->( DbSkip( 1 ) )
      IF ( ::cAlias )->( Eof() )
         lMoved := .F.
         ( ::cAlias )->( DbGoTo( nSaveRecNum ) )
      ENDIF
   ENDIF

   RETURN lMoved

/*----------------------------------------------------------------------*/

METHOD IdeBrowse:previous()
   LOCAL nSaveRecNum := ( ::cAlias )->( recno() )
   LOCAL lMoved := .T.

   ( ::cAlias )->( DbSkip( -1 ) )

   IF ( ::cAlias )->( Bof() )
      ( ::cAlias )->( DbGoTo( nSaveRecNum ) )
      lMoved := .F.
   ENDIF

   RETURN lMoved

/*----------------------------------------------------------------------*/

METHOD IdeBrowse:getSome( cType, cFor )
   LOCAL nOrd := ::indexOrd()

   SWITCH cType
   CASE "N"
      RETURN ( QInputDialog():new() ):getDouble( ::oWnd:oWidget, "Search for?", cFor, ;
                         0, -2147483647, 2147483647, iif( nOrd > 0, 3, ::aStruct[ ::oBrw:colPos, 4 ] ) )
   CASE "D"
      RETURN hbide_fetchADate( ::oWnd:oWidget, "Search for?", cFor )
   CASE "C"
      RETURN ( QInputDialog():new() ):getText( ::oWnd:oWidget, "Search for?", cFor )
   ENDSWITCH

   RETURN ""

/*------------------------------------------------------------------------*/

METHOD IdeBrowse:seekAsk( nMode )

   IF ::indexOrd() == 0
      RETURN Self
   ENDIF

   ::search( ::getSome( valtype( ::indexKeyValue() ) ), nMode == 1, nMode == 2 )

   RETURN Self

/*------------------------------------------------------------------------*/

METHOD IdeBrowse:searchAsk( nMode )
   LOCAL xValue, cFor

   DEFAULT nMode TO 0

   IF nMode == 0
      xValue := iif( ::indexOrd() > 0, ::indexKeyValue(), eval( ::oBrw:getColumn( ::oBrw:colPos ):block ) )
      cFor   := iif( ::indexOrd() > 0, "Indexed: " + ::indexKey(), ::aStruct[ ::oBrw:colPos, 1 ] )
   ELSEIF nMode == 1
      xValue := eval( ::oBrw:getColumn( ::oBrw:colPos ):block )
      cFor   := ::aStruct[ ::oBrw:colPos, 1 ]
   ENDIF

   ::search( ::getSome( valtype( xValue ), cFor ), .f., .f., nMode )

   RETURN Self

/*----------------------------------------------------------------------*/

METHOD IdeBrowse:search( cSearch, lSoft, lLast, nMode )
   LOCAL nRec

   DEFAULT nMode TO 0

   IF ::lInSearch
      ::qTimer:stop()
      ::lInSearch := .f.
   ENDIF

   IF ::nType == BRW_TYPE_DBF
      IF nMode == 0
         IF ( ::cAlias )->( IndexOrd() ) > 0

            DEFAULT lLast TO .f.
            DEFAULT lSoft TO .f.

            nRec := ::recNo()
            IF ( ::cAlias )->( DbSeek( cSearch, lSoft, lLast ) )
               ::refreshAll()
               ::dispInfo()
            ELSEIF ! lSoft
               ::goto( nRec )
               MsgBox( "Could not find: " + cSearch )
            ENDIF
         ELSE
            ::xSearch   := cSearch
            ::lInSearch := .t.
            ::qTimer:start()
         ENDIF
      ELSE
         ::xSearch   := cSearch
         ::lInSearch := .t.
         ::qTimer:start()
      ENDIF
   ELSE
      // Ascan
   ENDIF

   RETURN Self

/*----------------------------------------------------------------------*/

METHOD IdeBrowse:refreshAll()

   ::oBrw:refreshAll()
   ::oBrw:forceStable()
   ::oBrw:setCurrentIndex( .t. )
   ::dispInfo()

   RETURN Self

/*----------------------------------------------------------------------*/

METHOD IdeBrowse:toColumn( ncIndex )
   LOCAL nIndex

   IF valtype( ncIndex ) == "C"
      ncIndex := upper( ncIndex )
      nIndex := ascan( ::aStruct, {|e_|  e_[ 1 ] = ncIndex } )
   ELSE
      nIndex := ncIndex
   ENDIF

   IF empty( nIndex )
      RETURN Self
   ENDIF

   ::oBrw:colPos := nIndex
   ::oBrw:refreshAll()
   ::oBrw:forceStable()
   ::oBrw:setCurrentIndex( .t. )

   RETURN Self

/*----------------------------------------------------------------------*/

METHOD IdeBrowse:lock()

   IF ::nType == BRW_TYPE_DBF
      IF ! ( ::cAlias )->( DbrLock() )
         MsgBox( "Record could not been locked" )
      ENDIF
   ELSE
      MsgBox( "Record can not be locked" )
   ENDIF

   RETURN Self

/*----------------------------------------------------------------------*/

METHOD IdeBrowse:goToAsk()
   LOCAL nRec

   IF ! empty( nRec := ( QInputDialog():new() ):getInt( , "Goto", "Record_# ?", ::recno(), 1, ::lastrec() ) )
      ::goto( nRec )
      ::refreshAll()
   ENDIF

   RETURN NIL

/*----------------------------------------------------------------------*/

METHOD IdeBrowse:goto( nRec )

   IF ::nType == BRW_TYPE_DBF
      ( ::cAlias )->( DbGoto( nRec ) )
      ::refreshAll()
   ELSE
      IF nRec > 0 .AND. nRec <= len( ::aData )
         ::nIndex := nRec
      ENDIF
   ENDIF

   RETURN Self

/*----------------------------------------------------------------------*/

METHOD IdeBrowse:goTop()

   IF ::nType == BRW_TYPE_DBF
      ( ::cAlias )->( DbGotop() )
      ::refreshAll()
   ELSE
      ::nIndex := 1
   ENDIF
   RETURN NIL

/*----------------------------------------------------------------------*/

METHOD IdeBrowse:goBottom()

   IF ::nType == BRW_TYPE_DBF
      ( ::cAlias )->( DbGoBottom() )
      ::refreshAll()
   ELSE
      ::nIndex := len( ::aData )
   ENDIF

   RETURN NIL

/*----------------------------------------------------------------------*/

METHOD IdeBrowse:setOrder( nOrder )

   IF ::nType == BRW_TYPE_DBF
      ( ::cAlias )->( DbSetOrder( nOrder ) )
      ::refreshAll()
   ENDIF

   RETURN NIL

/*----------------------------------------------------------------------*/

METHOD IdeBrowse:indexOrd()

   IF ::nType == BRW_TYPE_DBF
      RETURN ( ::cAlias )->( IndexOrd() )
   ENDIF

   RETURN 0

/*----------------------------------------------------------------------*/

METHOD IdeBrowse:recNo()

   IF ::nType == BRW_TYPE_DBF
      RETURN ( ::cAlias )->( RecNo() )
   ELSE
      RETURN ::nIndex
   ENDIF

   RETURN NIL

/*----------------------------------------------------------------------*/

METHOD IdeBrowse:lastRec()

   IF ::nType == BRW_TYPE_DBF
      RETURN ( ::cAlias )->( LastRec() )
   ELSE
      RETURN len( ::aData )
   ENDIF

   RETURN NIL

/*----------------------------------------------------------------------*/

METHOD IdeBrowse:setIndex( cIndex )
   LOCAL n

   IF ( n := ascan( ::aIndex, cIndex ) ) > 0
      ( ::cAlias )->( DbSetOrder( n ) )
      ::oBrw:refreshAll()
      ::oBrw:forceStable()
      ::oBrw:setCurrentIndex( .t. )

      ::dispInfo()
   ENDIF

   RETURN Self

/*----------------------------------------------------------------------*/

METHOD IdeBrowse:getIndexInfo()
   LOCAL a_:= {}, i, cKey

   IF ::nType == BRW_TYPE_DBF
      FOR i := 1 to 50
         IF ( cKey := ( ::cAlias )->( IndexKey( i ) ) ) == ''
            EXIT
         ENDIF
         aadd( a_, ( ::cAlias )->( OrdName( i ) ) + ' : ' + cKey )
      NEXT
   ENDIF

   ::aIndex := a_

   RETURN ::aIndex

/*----------------------------------------------------------------------*/

METHOD IdeBrowse:ordName( nOrder )
   DEFAULT nOrder TO ::indexOrd()

   IF ::nType == BRW_TYPE_DBF
      RETURN ( ::cAlias )->( OrdName( nOrder ) )
   ENDIF

   RETURN ""

/*----------------------------------------------------------------------*/

METHOD IdeBrowse:indexKeyValue( nOrder )
   LOCAL xValue

   IF ::nType == BRW_TYPE_DBF
      xValue := ( ::cAlias )->( &( IndexKey( nOrder ) ) )
   ENDIF

   RETURN xValue

/*------------------------------------------------------------------------*/

METHOD IdeBrowse:indexKey( nOrder )
   DEFAULT nOrder TO ::indexOrd()

   IF ::nType == BRW_TYPE_DBF
      RETURN ( ::cAlias )->( IndexKey( nOrder ) )
   ENDIF

   RETURN ""

/*----------------------------------------------------------------------*/

METHOD IdeBrowse:append()

   IF ::nType == BRW_TYPE_DBF
      ( ::cAlias )->( DbAppend() )
      IF ! NetErr()
         ( ::cAlias )->( DbCommit() )
         ( ::cAlias )->( DbrUnlock() )
         ::refreshAll()
      ENDIF
   ELSE

   ENDIF
   RETURN Self

/*------------------------------------------------------------------------*/

METHOD IdeBrowse:delete( lAsk )

   DEFAULT lAsk TO .t.

   IF lAsk
      IF ! hbide_getYesNo( "Delete Record ?", , "Deletion Process" )
         RETURN Self
      ENDIF
   ENDIF

   IF ::nType == BRW_TYPE_DBF
      IF ( ::cAlias )->( DbRLock() )
         ( ::cAlias )->( DbDelete() )
         ( ::cAlias )->( DbCommit() )
         ( ::cAlias )->( DbRUnlock() )
         ::refreshAll()
      ENDIF
   ELSE

   ENDIF
   RETURN Self

/*------------------------------------------------------------------------*/

METHOD IdeBrowse:recall()

   IF ::nType == BRW_TYPE_DBF
      IF ( ::cAlias )->( Deleted() )
         IF ( ::cAlias )->( DbRLock() )
            ( ::cAlias )->( DbRecall() )
            ( ::cAlias )->( DbCommit() )
            ( ::cAlias )->( DbRUnlock() )
            ::refreshAll()
         ENDIF
      ENDIF
   ELSE

   ENDIF
   RETURN Self

/*------------------------------------------------------------------------*/

METHOD IdeBrowse:use()
   LOCAL bError, oErr
   LOCAL lErr := .f.

   SWITCH ::cDriver
   CASE "DBFCDX"
   CASE "DBFNTX"
   CASE "DBFNSX"
      bError := ErrorBlock( {|o| break( o ) } )
      BEGIN SEQUENCE
         IF empty( ::cAlias )
            USE ( ::cTable ) SHARED NEW VIA ( ::cDriver )
         ELSE
            USE ( ::cTable ) ALIAS ( ::cAlias ) SHARED NEW VIA ( ::cDriver )
         ENDIF
         IF NetErr()
            MsgBox( ::cTable, "Could not been opened!" )
            lErr := .t.
         ENDIF
      RECOVER USING oErr
         MsgBox( oErr:description, "Error Opening Table" )
         RETURN Self
      ENDSEQUENCE
      ErrorBlock( bError )

      EXIT
   OTHERWISE
      lErr := hbide_execScriptFunction( "tableUse", ::cTable, ::cAlias, ::cDriver, ::cConxn ) /* cTable holds the information about connection */
      EXIT
   ENDSWITCH

   IF lErr
      RETURN .f.
   ENDIF

   IF empty( ::cAlias )
      ::cAlias := alias()
   ENDIF

   RETURN .t.

/*----------------------------------------------------------------------*/

METHOD IdeBrowse:exists()

   SWITCH ::cDriver
   CASE "DBFCDX"
   CASE "DBFNSX"
   CASE "DBFNTX"
      RETURN hb_fileExists( ::cTable )
   OTHERWISE
      RETURN hbide_execScriptFunction( "tableExists", ::cTable, ::cDriver, ::cConxn )
   ENDSWITCH

   RETURN .f.

/*----------------------------------------------------------------------*/

