/*
 * $Id$
 */

/*
 * Harbour Project source code:
 *
 * Copyright 2010 Pritpal Bedi <pritpal@vouchcac.com>
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
 *                               20Feb2010
 */
/*----------------------------------------------------------------------*/
/*----------------------------------------------------------------------*/
/*----------------------------------------------------------------------*/

#include "hbide.ch"
#include "common.ch"
#include "hbclass.ch"
#include "hbqt.ch"

/*----------------------------------------------------------------------*/

#define buttonInstall_clicked                     1
#define editInstall_textChanged                   2
#define buttonHome_clicked                        3
#define buttonBackward_clicked                    4
#define buttonForward_clicked                     5
#define buttonRefresh_clicked                     6
#define buttonPrint_clicked                       7
#define buttonPdf_clicked                         8
#define editIndex_textChanged                     9
#define treeDoc_doubleClicked                     10
#define treeDoc_itemSelectionChanged              11
#define editIndex_returnPressed                   12
#define listIndex_ItemDoubleClicked               13
#define buttonUp_clicked                          14
#define browserView_anchorClicked                 15
#define tabWidgetContents_currentChanged          16
#define treeCategory_itemSelectionChanged         17

/*----------------------------------------------------------------------*/

#define DOC_FUN_BEGINS                           -5
#define DOC_FUN_ENDS                             -1
#define DOC_FUN_NONE                              0
#define DOC_FUN_TEMPLATE                          1
#define DOC_FUN_FUNCNAME                          2
#define DOC_FUN_CATEGORY                          3
#define DOC_FUN_SUBCATEGORY                       4
#define DOC_FUN_ONELINER                          5
#define DOC_FUN_SYNTAX                            6
#define DOC_FUN_ARGUMENTS                         7
#define DOC_FUN_RETURNS                           8
#define DOC_FUN_DESCRIPTION                       9
#define DOC_FUN_EXAMPLES                          10
#define DOC_FUN_TESTS                             11
#define DOC_FUN_FILES                             12
#define DOC_FUN_STATUS                            13
#define DOC_FUN_PLATFORMS                         14
#define DOC_FUN_SEEALSO                           15
#define DOC_FUN_VERSION                           16
#define DOC_FUN_INHERITS                          17
#define DOC_FUN_METHODS                           18
#define DOC_FUN_EXTERNALLINK                      19

/*----------------------------------------------------------------------*/

CLASS IdeDocFunction

   DATA   cName                                   INIT ""
   DATA   cTemplate                               INIT ""
   DATA   cCategory                               INIT ""
   DATA   cSubCategory                            INIT ""
   DATA   cOneliner                               INIT ""
   DATA   cStatus                                 INIT ""
   DATA   cPlatforms                              INIT ""
   DATA   cSeaAlso                                INIT ""
   DATA   cVersion                                INIT ""
   DATA   cInherits                               INIT ""
   DATA   cExternalLink                           INIT ""
   DATA   aSyntax                                 INIT {}
   DATA   aArguments                              INIT {}
   DATA   aReturns                                INIT {}
   DATA   aDescription                            INIT {}
   DATA   aExamples                               INIT {}
   DATA   aTests                                  INIT {}
   DATA   aFiles                                  INIT {}
   DATA   aMethods                                INIT {}
   DATA   aSource                                 INIT {}

   DATA   oTVItem
   DATA   cSourceTxt                              INIT ""

   METHOD new()                                   INLINE Self

   ENDCLASS

/*----------------------------------------------------------------------*/

CLASS IdeHarbourHelp INHERIT IdeObject

   DATA   oUI
   DATA   cPathInstall                            INIT ""
   DATA   cDocPrefix

   DATA   aNodes                                  INIT {}
   DATA   aFunctions                              INIT {}
   DATA   aFuncByFile                             INIT {}
   DATA   aHistory                                INIT {}
   DATA   aCategory                               INIT {}

   DATA   nCurTVItem                              INIT 0
   DATA   nCurInHist                              INIT 0

   DATA   qHiliter

   DATA   hIndex                                  INIT {=>}

   METHOD new( oIde )
   METHOD create( oIde )
   METHOD show()
   METHOD destroy()
   METHOD destroyTrees()

   METHOD execEvent( nMode, p, p1 )

   METHOD setImages()
   METHOD setTooltips()
   METHOD setParameters()

   METHOD installSignals()
   METHOD refreshDocTree()
   METHOD updateViewer( aHtm )
   METHOD populateFuncDetails( n )
   METHOD populateTextFile( cTextFile )
   METHOD populateRootInfo()
   METHOD populatePathInfo( cPath )
   METHOD populateIndex()
   METHOD populateIndexedSelection()
   METHOD buildView( oFunc )
   METHOD print()
   METHOD exportAsPdf()
   METHOD paintRequested( pPrinter )
   METHOD parseTextFile( cTextFile, oParent )
   METHOD jumpToFunction( cFunction )

   ENDCLASS

/*----------------------------------------------------------------------*/

METHOD IdeHarbourHelp:new( oIde )

   ::oIde := oIde

   RETURN Self

/*----------------------------------------------------------------------*/

METHOD IdeHarbourHelp:create( oIde )

   DEFAULT oIde TO ::oIde
   ::oIde := oIde

   RETURN Self

/*----------------------------------------------------------------------*/

METHOD IdeHarbourHelp:show()

   IF empty( ::oUI )
      ::oUI := HbQtUI():new( ::resPath + "docviewgenerator.uic", ::oDlg:oWidget ):build()

      ::oDocViewDock:oWidget:setWidget( ::oUI )

      ::setImages()
      ::setTooltips()
      ::installSignals()
      ::setParameters()

      ::populateRootInfo()

      ::refreshDocTree()
   ENDIF

   RETURN Self

/*----------------------------------------------------------------------*/

METHOD IdeHarbourHelp:destroy()

   IF empty( ::oUI )
      RETURN Self
   ENDIF

   ::destroyTrees()

   ::oUI:destroy()

   RETURN Self

/*----------------------------------------------------------------------*/

METHOD IdeHarbourHelp:destroyTrees()
   LOCAL a_

   ::disconnect( ::oUI:q_treeDoc, "itemSelectionChanged()" )
   ::disconnect( ::oUI:q_treeCategory, "itemSelectionChanged()" )

   ::aHistory    := {}
   ::aFuncByFile := {}

   FOR EACH a_ IN ::aCategory
      a_[ 4 ] := NIL              // Reference to Contents node
   NEXT
   FOR EACH a_ IN ::aFunctions
      a_[ 4 ] := NIL              // Reference to Contents node
   NEXT

   /* Contents Tab */
   FOR EACH a_ IN ::aNodes
      IF a_[ 2 ] == "Function"
         a_[ 3 ]:removeChild( a_[ 1 ] )
         a_[ 1 ] := NIL ; a_[ 3 ] := NIL
      ENDIF
   NEXT
   FOR EACH a_ IN ::aNodes
      IF a_[ 2 ] == "File"
         a_[ 3 ]:removeChild( a_[ 1 ] )
         a_[ 1 ] := NIL ; a_[ 3 ] := NIL
      ENDIF
   NEXT
   FOR EACH a_ IN ::aNodes
      IF a_[ 2 ] == "Path"
         a_[ 3 ]:removeChild( a_[ 1 ] )
         a_[ 1 ] := NIL ; a_[ 3 ] := NIL
      ENDIF
   NEXT
   IF !empty( ::aNodes )
      ::aNodes[ 1, 1 ] := NIL
   ENDIF
   ::aNodes := {}

   /* Index Tab */
   FOR EACH a_ IN ::aFunctions
      a_[ 5 ] := NIL
   NEXT
   ::aFunctions := {}

   /* Category Tab */
   FOR EACH a_ IN ::aCategory
      IF a_[ 7 ] == " "
         a_[ 6 ]:removeChild( a_[ 5 ] )
         a_[ 6 ] := NIL ; a_[ 5 ] := NIL
      ENDIF
   NEXT
   FOR EACH a_ IN ::aCategory
      IF a_[ 7 ] == "U"
         a_[ 5 ] := NIL
      ENDIF
   NEXT
   ::aCategory := {}

   RETURN Self

/*----------------------------------------------------------------------*/

METHOD IdeHarbourHelp:setImages()
   LOCAL oUI := ::oUI

   oUI:q_buttonHome:setIcon( hbide_image( "dc_home" ) )
   oUI:q_buttonBackward:setIcon( hbide_image( "dc_left" ) )
   oUI:q_buttonForward:setIcon( hbide_image( "dc_right" ) )
   oUI:q_buttonUp:setIcon( hbide_image( "dc_up" ) )
   oUI:q_buttonRefresh:setIcon( hbide_image( "dc_refresh" ) )
   oUI:q_buttonPrint:setIcon( hbide_image( "dc_print" ) )
   oUI:q_buttonPdf:setIcon( hbide_image( "dc_pdffile" ) )

   oUI:q_buttonSave:setIcon( hbide_image( "save" ) )
   oUI:q_buttonExit:setIcon( hbide_image( "dc_quit" ) )

   oUI:q_buttonInstall:setIcon( hbide_image( "dc_folder" ) )

   oUI:q_buttonArgPlus:setIcon( hbide_image( "dc_plus" ) )
   oUI:q_buttonArgMinus:setIcon( hbide_image( "dc_delete" ) )
   oUI:q_buttonArgUp:setIcon( hbide_image( "dc_up" ) )
   oUI:q_buttonArgDown:setIcon( hbide_image( "dc_down" ) )

   RETURN Self

/*----------------------------------------------------------------------*/

METHOD IdeHarbourHelp:setTooltips()
   LOCAL oUI := ::oUI

   oUI:q_buttonHome:setToolTip( "Home" )
   oUI:q_buttonBackward:setToolTip( "Backward" )
   oUI:q_buttonForward:setToolTip( "Forward" )
   oUI:q_buttonRefresh:setToolTip( "Refresh" )
   oUI:q_buttonUp:setToolTip( "Up" )
   oUI:q_buttonPrint:setToolTip( "Print" )
   oUI:q_buttonPdf:setToolTip( "Export as PDF Document" )

   oUI:q_buttonSave:setToolTip( "Save" )
   oUI:q_buttonExit:setToolTip( "Exit" )

   oUI:q_buttonInstall:setToolTip( "Select Harbour Installation Path" )

   oUI:q_buttonArgPlus:setToolTip( "Add new argument" )
   oUI:q_buttonArgMinus:setToolTip( "Delete argument" )
   oUI:q_buttonArgUp:setToolTip( "Up one position" )
   oUI:q_buttonArgDown:setToolTip( "Down one position" )

   RETURN Self

/*----------------------------------------------------------------------*/

METHOD IdeHarbourHelp:setParameters()
   LOCAL oUI := ::oUI

   oUI:q_treeDoc:setHeaderHidden( .t. )
   oUI:q_treeCategory:setHeaderHidden( .t. )
   oUI:q_editInstall:setText( ::cWrkHarbour )

   ::qHiliter := ::oThemes:SetSyntaxHilighting( oUI:q_plainExamples, "Bare Minimum" )

   oUI:q_plainExamples:setFont( ::oFont:oWidget )
   oUI:q_plainDescription:setFont( ::oFont:oWidget )
   oUI:q_plainArguments:setFont( ::oFont:oWidget )
   oUI:q_plainArgDesc:setFont( ::oFont:oWidget )
   oUI:q_plainTests:setFont( ::oFont:oWidget )

   oUI:q_plainExamples:setLineWrapMode( QTextEdit_NoWrap )
   oUI:q_plainTests:setLineWrapMode( QTextEdit_NoWrap )

   oUI:q_treeDoc:expandsOnDoubleClick( .f. )

   oUI:q_browserView:setOpenLinks( .t. )
   oUI:q_browserView:setOpenExternalLinks( .t. )
   oUI:q_tabWidgetContents:setFocusPolicy( Qt_NoFocus )

   RETURN Self

/*----------------------------------------------------------------------*/

METHOD IdeHarbourHelp:installSignals()

   ::oUI:signal( "buttonInstall" , "clicked()"                 , {| | ::execEvent( buttonInstall_clicked  )     } )
   ::oUI:signal( "buttonHome"    , "clicked()"                 , {| | ::execEvent( buttonHome_clicked     )     } )
   ::oUI:signal( "buttonBackward", "clicked()"                 , {| | ::execEvent( buttonBackward_clicked )     } )
   ::oUI:signal( "buttonForward" , "clicked()"                 , {| | ::execEvent( buttonForward_clicked  )     } )
   ::oUI:signal( "buttonUp"      , "clicked()"                 , {| | ::execEvent( buttonUp_clicked       )     } )
   ::oUI:signal( "buttonRefresh" , "clicked()"                 , {| | ::execEvent( buttonRefresh_clicked  )     } )
   ::oUI:signal( "buttonPrint"   , "clicked()"                 , {| | ::execEvent( buttonPrint_clicked    )     } )
   ::oUI:signal( "buttonPdf"     , "clicked()"                 , {| | ::execEvent( buttonPdf_clicked      )     } )
   ::oUI:signal( "editInstall"   , "textChanged(QString)"      , {|p| ::execEvent( editInstall_textChanged, p ) } )
   ::oUI:signal( "editIndex"     , "textChanged(QString)"      , {|p| ::execEvent( editIndex_textChanged, p   ) } )
   ::oUI:signal( "editIndex"     , "returnPressed()"           , {| | ::execEvent( editIndex_returnPressed    ) } )
   ::oUI:signal( "listIndex"     , "itemDoubleClicked(QLWItem)", {|p| ::execEvent( listIndex_ItemDoubleClicked, p ) } )
   ::oUI:signal( "browserView"   , "anchorClicked(QUrl)"       , {|p| ::execEvent( browserView_anchorClicked, p ) } )
   ::oUI:signal( "tabWidgetContents", "currentChanged(int)"    , {|p| ::execEvent( tabWidgetContents_currentChanged, p ) } )

   RETURN Self

/*----------------------------------------------------------------------*/

METHOD IdeHarbourHelp:execEvent( nMode, p, p1 )
   LOCAL cPath, qTWItem, cText, n, nn, nLen, cLower, qUrl

   HB_SYMBOL_UNUSED( p1 )

   SWITCH nMode

   CASE buttonInstall_clicked
      cPath := hbide_fetchADir( ::oDocViewDock, "Harbour Install Root" )
      IF !empty( cPath )
         ::oUI:q_editInstall:setText( cPath )
      ENDIF
      EXIT

   CASE tabWidgetContents_currentChanged
      IF p == 1
         ::oUI:q_editIndex:setFocus_1()
      ENDIF
      EXIT

   CASE browserView_anchorClicked
      qUrl := QUrl():from( p )
      cText := lower( qUrl:toString() )
      nLen := len( cText )
      IF ( n := ascan( ::aFunctions, {|e_| left( e_[ 6 ], nLen ) == cText } ) ) > 0
         ::oUI:q_listIndex:setCurrentItem( ::aFunctions[ n, 5 ] )
         ::populateIndexedSelection()
      ENDIF
      EXIT

   CASE listIndex_ItemDoubleClicked
      ::populateIndexedSelection()
      ::oUI:q_editIndex:setFocus_1()
      EXIT

   CASE editIndex_returnPressed
      IF !empty( ::oUI:q_editIndex:text() )
         ::populateIndexedSelection()
         ::oUI:q_editIndex:setFocus_1()
      ENDIF
      EXIT

   CASE editIndex_textChanged
      nLen := len( p )
      cLower := lower( p )
      IF ( n := ascan( ::aFunctions, {|e_| left( e_[ 6 ], nLen ) == cLower } ) ) > 0
         ::oUI:q_listIndex:setCurrentItem( ::aFunctions[ n, 5 ] )
      ENDIF
      EXIT

   CASE editInstall_textChanged
      IF hb_dirExists( p )
         ::oUI:q_editInstall:setStyleSheet( "" )
         ::cPathInstall := hbide_pathStripLastSlash( hbide_pathNormalized( p, .f. ) )
         ::oIde:cWrkHarbour := ::cPathInstall
      ELSE
         ::oUI:q_editInstall:setStyleSheet( getStyleSheet( "PathIsWrong" ) )
      ENDIF
      EXIT

   CASE buttonHome_clicked
      IF !empty( ::aNodes )
         ::oUI:q_treeDoc:setCurrentItem( ::aNodes[ 1, 1 ], 0 )
      ENDIF
      EXIT

   CASE buttonBackward_clicked
      IF ::nCurInHist > 1
         ::oUI:q_treeDoc:setCurrentItem( ::aNodes[ ::aHistory[ ::nCurInHist - 1 ], 1 ], 0 )
      ENDIF
      EXIT

   CASE buttonForward_clicked
      IF ::nCurInHist < len( ::aHistory )
         ::oUI:q_treeDoc:setCurrentItem( ::aNodes[ ::aHistory[ ::nCurInHist + 1 ], 1 ], 0 )
      ENDIF
      EXIT

   CASE buttonUp_clicked
      IF ::nCurInHist > 1 .AND. ::nCurInHist <= len( ::aHistory )
         IF !empty( qTWItem := ::oUI:q_treeDoc:itemAbove( ::oUI:q_treeDoc:currentItem( 0 ) ) )
            ::oUI:q_treeDoc:setCurrentItem( qTWItem, 0 )
         ENDIF
      ENDIF
      EXIT

   CASE buttonRefresh_clicked
      ::refreshDocTree()
      EXIT

   CASE buttonPrint_clicked
      ::print()
      EXIT

   CASE buttonPdf_clicked
      ::exportAsPdf()
      EXIT

   CASE treeCategory_itemSelectionChanged
      qTWItem := ::oUI:q_treeCategory:currentItem()
      n := ascan( ::aCategory, {|e_| hbqt_IsEqualGcQtPointer( e_[ 5 ]:pPtr, qTWItem ) } )
      IF n > 0
         IF ::aCategory[ n, 5 ]:childCount() == 0
            ::oUI:q_treeDoc:setCurrentItem( ::aCategory[ n, 4 ], 0 )
         ENDIF
      ENDIF
      EXIT

   CASE treeDoc_itemSelectionChanged
      qTWItem := QTreeWidgetItem():from( ::oUI:q_treeDoc:currentItem() )
      cText   := qTWItem:text( 0 )

      IF ( n := ascan( ::aNodes, {|e_| e_[ 5 ] == cText } ) ) > 0
         IF ( nn := ascan( ::aHistory, n ) ) == 0
            aadd( ::aHistory, n )
            ::nCurInHist := len( ::aHistory )
         ELSE
            ::nCurInHist := nn
         ENDIF
         ::nCurTVItem := n

         IF     ::aNodes[ n, 2 ] == "Root"
            ::populateRootInfo()
         ELSEIF ::aNodes[ n, 2 ] == "Path"
            ::populatePathInfo( ::aNodes[ n, 4 ] )
         ELSEIF ::aNodes[ n, 2 ] == "File"
            ::populateTextFile( ::aNodes[ n, 4 ] )
         ELSEIF ::aNodes[ n, 2 ] == "Function"
            ::populateFuncDetails( n )
         ENDIF
      ENDIF
      EXIT

   ENDSWITCH

   RETURN Self

/*----------------------------------------------------------------------*/

METHOD IdeHarbourHelp:jumpToFunction( cFunction )
   LOCAL n, nLen

   nLen := len( cFunction )
   cFunction := lower( cFunction )
   IF !empty( ::aNodes )
      IF ( n := ascan( ::aFunctions, {|e_| lower( left( e_[ 2 ], nLen ) ) == cFunction } ) ) > 0
         ::oUI:q_treeDoc:setCurrentItem( ::aFunctions[ n, 4 ] )
      ENDIF
   ENDIF

   RETURN Self

/*----------------------------------------------------------------------*/

METHOD IdeHarbourHelp:populateIndexedSelection()
   LOCAL qItem, cText, n

   IF !empty( ::aNodes )
      IF !empty( qItem := ::oUI:q_listIndex:currentItem() )
         qItem := QListWidgetItem():from( qItem )
         cText := qItem:text()
         IF ( n := ascan( ::aFunctions, {|e_| e_[ 2 ] == cText } ) ) > 0
            ::oUI:q_treeDoc:setCurrentItem( ::aFunctions[ n, 4 ] )
         ENDIF
      ENDIF
   ENDIF
   RETURN Self

/*----------------------------------------------------------------------*/

METHOD IdeHarbourHelp:refreshDocTree()
   LOCAL aPaths, cFolder, cNFolder, aDocs, oChild, oParent, oRoot, cRoot
   LOCAL aDir, a_, cTextFile, n

   IF empty( ::cPathInstall ) .OR. ! hb_dirExists( ::cPathInstall )
      RETURN Self
   ENDIF

   /* Clean Environment */
   ::destroyTrees()
   ::oUI:q_treeDoc:clear()
   //
   ::connect( ::oUI:q_treeDoc     , "itemSelectionChanged()" , {| | ::execEvent( treeDoc_itemSelectionChanged ) } )
   ::connect( ::oUI:q_treeCategory, "itemSelectionChanged()" , {| | ::execEvent( treeCategory_itemSelectionChanged ) } )
   //
   ::aNodes      := {}
   ::aFuncByFile := {}
   ::aHistory    := {}
   ::aFunctions  := {}
   ::nCurTVItem  := 0
   ::nCurInHist  := 0

   aPaths := {}
   aDocs  := {}
   hbide_fetchSubPaths( @aPaths, ::cPathInstall, .t. )
   cRoot  := aPaths[ 1 ]

   FOR EACH cFolder IN aPaths
      cNFolder := hbide_pathNormalized( cFolder, .t. )
      IF ( "/doc" $ cNFolder ) .OR. ( "/doc/en" $ cNFolder )
         aadd( aDocs, cFolder )
      ENDIF
   NEXT

   oRoot := QTreeWidgetItem():new()
   oRoot:setText( 0, aPaths[ 1 ] )
   oRoot:setIcon( 0, hbide_image( "dc_home" ) )
   oRoot:setToolTip( 0, aPaths[ 1 ] )
   oRoot:setExpanded( .t. )

   ::oUI:q_treeDoc:addTopLevelItem( oRoot )

   aadd( ::aNodes, { oRoot, "Path", NIL, cRoot, cRoot } )
   hbide_buildFoldersTree( ::aNodes, aDocs )
   ::aNodes[ 1,2 ] := "Root"

   FOR EACH cFolder IN aDocs
      IF ( n := ascan( ::aNodes, {|e_| e_[ 2 ] == "Path" .AND. lower( e_[ 4 ] ) == lower( cFolder ) } ) ) > 0
         oParent := ::aNodes[ n, 1 ]

         aDir    := directory( cFolder + "*.txt" )
         FOR EACH a_ IN aDir
            IF a_[ 5 ] != "D"
               cTextFile := cFolder + a_[ 1 ]
               oChild := QTreeWidgetItem():new()
               oChild:setText( 0, a_[ 1 ]  )
               oChild:setIcon( 0, ::resPath + "dc_textdoc.png" )
               oChild:setToolTip( 0, cTextFile )
               oParent:addChild( oChild )
               aadd( ::aNodes, { oChild, "File", oParent, cTextFile, a_[ 1 ] } )
               ::parseTextFile( cTextFile, oChild )
            ENDIF
         NEXT
      ENDIF
   NEXT

   ::populateIndex()

   ::oUI:q_treeDoc:expandItem( oRoot )

   RETURN Self

/*----------------------------------------------------------------------*/

STATIC FUNCTION hbide_buildFoldersTree( aNodes, aPaths )
   LOCAL cRoot, cPath, s, aSubs, i, n, cCPath, cPPath, nP, cOSPath, oParent, oChild
   LOCAL cIcon := hbide_image( "dc_folder" )

   cRoot := aNodes[ 1, 4 ]

   FOR EACH s IN aPaths
      cPath := s
      cPath := hbide_stripRoot( cRoot, cPath )
      cPath := hbide_pathNormalized( cPath, .t. )

      aSubs := hb_aTokens( cPath, "/" )

      FOR i := 1 TO len( aSubs )
         IF !empty( aSubs[ i ] )
            cCPath := hbide_buildPathFromSubs( aSubs, i )
            n := ascan( aNodes, {|e_| hbide_pathNormalized( e_[ 4 ], .t. ) == hbide_pathNormalized( cRoot + cCPath, .t. ) } )

            IF n == 0
               cPPath  := hbide_buildPathFromSubs( aSubs, i - 1 )
               nP      := ascan( aNodes, {|e_| hbide_pathNormalized( e_[ 4 ], .t. ) == hbide_pathNormalized( cRoot + cPPath, .t. ) } )

               oParent := aNodes[ nP, 1 ]

               cOSPath := hbide_pathToOSPath( cRoot + cCPath )

               oChild  := QTreeWidgetItem():new()
               oChild:setText( 0, aSubs[ i ] )
               oChild:setIcon( 0, cIcon )
               oChild:setToolTip( 0, cOSPath )

               oParent:addChild( oChild )

               aadd( aNodes, { oChild, "Path", oParent, cOSPath, aSubs[ i ] } )
            ENDIF
         ENDIF
      NEXT
   NEXT

   RETURN NIL

/*----------------------------------------------------------------------*/

STATIC FUNCTION hbide_buildPathFromSubs( aSubs, nUpto )
   LOCAL i, cPath := ""

   IF nUpto > 0
      FOR i := 1 TO nUpto
         cPath += aSubs[ i ] + "/"
      NEXT
   ENDIF
   RETURN cPath

/*----------------------------------------------------------------------*/

STATIC FUNCTION hbide_stripRoot( cRoot, cPath )
   LOCAL cLRoot, cLPath, cP

   cLRoot := hbide_pathNormalized( cRoot, .t. )
   cLPath := hbide_pathNormalized( cPath, .t. )
   IF left( cLPath, len( cLRoot ) ) == cLRoot
      cP := substr( cLPath, len( cRoot ) + 1 )
      RETURN cP
   ENDIF

   RETURN cPath

/*----------------------------------------------------------------------*/

METHOD IdeHarbourHelp:populateIndex()
   LOCAL a_, qItem, oFunc, oParent, n
   LOCAL aUnq := {}

   asort( ::aFunctions, , , {|e_, f_| e_[ 2 ] < f_[ 2 ] } )

   ::oUI:q_listIndex:setSortingEnabled( .t. )

   FOR EACH a_ IN ::aFunctions
      IF !empty( a_[ 2 ] )
         qItem := QListWidgetItem():new()
         qItem:setText( a_[ 2 ] )
         a_[ 5 ] := qItem
         ::oUI:q_listIndex:addItem_1( qItem )
      ENDIF
   NEXT

   FOR EACH a_ IN ::aFunctions
      oFunc := a_[ 3 ]
      IF !empty( oFunc:cCategory )
         IF ascan( aUnq, {|e_| e_[ 1 ] == oFunc:cCategory } ) == 0
            aadd( aUnq, { oFunc:cCategory, NIL } )
            aadd( ::aCategory, { oFunc:cCategory, oFunc:cSubCategory, oFunc, a_[ 4 ], NIL, NIL, "U" } )
         ELSE
            aadd( ::aCategory, { oFunc:cCategory, oFunc:cSubCategory, oFunc, a_[ 4 ], NIL, NIL, " " } )
         ENDIF
      ENDIF
   NEXT
   IF !empty( ::aCategory )
      asort( ::aCategory, , , {|e_, f_| e_[ 1 ] < f_[ 1 ] } )
   ENDIF
   FOR EACH a_ IN aUnq
      qItem := QTreeWidgetItem():new()
      qItem:setText( 0, a_[ 1 ] )
      ::oUI:q_treeCategory:addTopLevelItem( qItem )
      a_[ 2 ] := qItem
   NEXT
   FOR EACH a_ IN ::aCategory
      IF ( n := ascan( aUnq, {|e_| e_[ 1 ] == a_[ 1 ] } ) ) > 0
         oParent := aUnq[ n, 2 ]

         qItem := QTreeWidgetItem():new()
         qItem:setText( 0, a_[ 3 ]:cName )

         oParent:addChild( qItem )
         a_[ 5 ] := qItem
         a_[ 6 ] := oParent
      ENDIF
   NEXT

   RETURN Self

/*----------------------------------------------------------------------*/

METHOD IdeHarbourHelp:parseTextFile( cTextFile, oParent )
   LOCAL a_, s, nPart, oFunc, oTWItem
   LOCAL lIsFunc := .f.
   LOCAL cIcon   := hbide_image( "dc_function" )
   LOCAL aFn     := {}
   LOCAL nParsed := ascan( ::aFuncByFile, {|e_| e_[ 1 ] == cTextFile } )

   IF nParsed == 0
      nPart := DOC_FUN_NONE

      a_:= hbide_readSource( cTextFile )

      FOR EACH s IN a_
         DO CASE

         CASE "$DOC$"         $ s
            lIsFunc := .t.
            nPart   := DOC_FUN_BEGINS
            oFunc   := IdeDocFunction():new()

         CASE "$END$"         $ s
            IF lIsFunc
               lIsFunc := .f.
               nPart   := DOC_FUN_ENDS
               oTWItem := QTreeWidgetItem():new()
               oTWItem:setText( 0, oFunc:cName )
               oTWItem:setIcon( 0, cIcon )
               oTWItem:setTooltip( 0, oFunc:cName )
               oParent:addChild( oTWItem )
               aadd( ::aNodes, { oTWItem, "Function", oParent, cTextFile + "<::>" + oFunc:cName, oFunc:cName } )
               aadd( ::aFunctions, { cTextFile, oFunc:cName, oFunc, oTWItem, NIL, lower( oFunc:cName ) } )
               aadd( aFn, oFunc )
            ENDIF

         CASE "$TEMPLATE$"    $ s
            nPart := DOC_FUN_TEMPLATE
         CASE "$FUNCNAME$"    $ s   .OR.  "$NAME$" $ s
            nPart := DOC_FUN_FUNCNAME
         CASE "$CATEGORY$"    $ s
            nPart := DOC_FUN_CATEGORY
         CASE "$SUBCATEGORY$" $ s
            nPart := DOC_FUN_SUBCATEGORY
         CASE "$ONELINER$"    $ s
            nPart := DOC_FUN_ONELINER
         CASE "$SYNTAX$"      $ s
            nPart := DOC_FUN_SYNTAX
         CASE "$ARGUMENTS$"   $ s
            nPart := DOC_FUN_ARGUMENTS
         CASE "$RETURNS$"     $ s
            nPart := DOC_FUN_RETURNS
         CASE "$DESCRIPTION$" $ s
            nPart := DOC_FUN_DESCRIPTION
         CASE "$EXAMPLES$"    $ s
            nPart := DOC_FUN_EXAMPLES
         CASE "$TESTS$"       $ s
            nPart := DOC_FUN_TESTS
         CASE "$FILES$"       $ s
            nPart := DOC_FUN_FILES
         CASE "$STATUS$"       $ s
            nPart := DOC_FUN_STATUS
         CASE "$PLATFORMS$"   $ s  .OR.  "$COMPLIANCE$" $ s
            nPart := DOC_FUN_PLATFORMS
         CASE "$SEEALSO$"     $ s
            nPart := DOC_FUN_SEEALSO
         CASE "$VERSION$"     $ s
            nPart := DOC_FUN_VERSION
         CASE "$INHERITS"     $ s
            nPart := DOC_FUN_INHERITS
         CASE "$METHODS"      $ s
            nPart := DOC_FUN_METHODS
         CASE "$EXTERNALLINK" $ s
            nPart := DOC_FUN_EXTERNALLINK
         OTHERWISE
            IF ! lIsFunc
               LOOP   // It is a fake line not within $DOC$ => $END$ block
            ENDIF
            s := substr( s, 9 )

            SWITCH nPart
            CASE DOC_FUN_BEGINS
               EXIT
            CASE DOC_FUN_TEMPLATE
               oFunc:cTemplate    := s
               EXIT
            CASE DOC_FUN_FUNCNAME
               oFunc:cName        := alltrim( s )
               EXIT
            CASE DOC_FUN_CATEGORY
               oFunc:cCategory    := alltrim( s )
               EXIT
            CASE DOC_FUN_SUBCATEGORY
               oFunc:cSubCategory := alltrim( s )
               EXIT
            CASE DOC_FUN_ONELINER
               oFunc:cOneLiner    := s
               EXIT
            CASE DOC_FUN_SYNTAX
               aadd( oFunc:aSyntax     , s )
               EXIT
            CASE DOC_FUN_ARGUMENTS
               aadd( oFunc:aArguments  , s )
               EXIT
            CASE DOC_FUN_RETURNS
               aadd( oFunc:aReturns    , s )
               EXIT
            CASE DOC_FUN_DESCRIPTION
               aadd( oFunc:aDescription, s )
               EXIT
            CASE DOC_FUN_EXAMPLES
               aadd( oFunc:aExamples   , s )
               EXIT
            CASE DOC_FUN_TESTS
               aadd( oFunc:aTests      , s )
               EXIT
            CASE DOC_FUN_FILES
               aadd( oFunc:aFiles      , s )
               EXIT
            CASE DOC_FUN_STATUS
               oFunc:cStatus    := alltrim( s )
               EXIT
            CASE DOC_FUN_PLATFORMS
               oFunc:cPlatForms := alltrim( s )
               EXIT
            CASE DOC_FUN_SEEALSO
               oFunc:cSeaAlso   := alltrim( s )
               EXIT
            CASE DOC_FUN_SEEALSO
               oFunc:cVersion   := alltrim( s )
               EXIT
            CASE DOC_FUN_INHERITS
               oFunc:cInherits  := alltrim( s )
               EXIT
            CASE DOC_FUN_METHODS
               aadd( oFunc:aMethods    , s )
               EXIT
            CASE DOC_FUN_VERSION
               oFunc:cVersion   := alltrim( s )
               EXIT
            CASE DOC_FUN_EXTERNALLINK
               oFunc:cExternalLink := alltrim( s )
               EXIT
            OTHERWISE
               nPart := DOC_FUN_NONE
               EXIT
            ENDSWITCH
         ENDCASE
      NEXT

      aadd( ::aFuncByFile, { cTextFile, aFn } )
   ENDIF

   RETURN Self

/*----------------------------------------------------------------------*/

METHOD IdeHarbourHelp:updateViewer( aHtm )

   ::oUI:q_browserView:setHTML( hbide_arrayToMemo( aHtm ) )

   RETURN Self

/*----------------------------------------------------------------------*/

METHOD IdeHarbourHelp:populateRootInfo()
   LOCAL aHtm := {}

   aadd( aHtm, "<HTML>" )
   aadd( aHtm, ' <BODY ALIGN=center VALIGN=center>' )
   aadd( aHtm, '  <H1><FONT color=green>' + "Welcome" + '</FONT></H1>' )
   aadd( aHtm, '  <BR>' + '&nbsp;' + '</BR>' )
   aadd( aHtm, '  <H2><FONT color=blue>' + ::cPathInstall + '</FONT></H2>' )
   aadd( aHtm, '  <BR>&nbsp;</BR>' )
   aadd( aHtm, '  <BR>&nbsp;</BR>' )
   aadd( aHtm, '  <IMG src="' + 'resources/harbour.png' + '" width="300" height="200"</IMG></BR>' )
   aadd( aHtm, " </BODY>" )
   aadd( aHtm, "</HTML>" )

   ::updateViewer( aHtm )

   RETURN Self

/*----------------------------------------------------------------------*/

METHOD IdeHarbourHelp:populatePathInfo( cPath )
   LOCAL aHtm := {}

   aadd( aHtm, "<HTML>" )
   aadd( aHtm, " <BODY ALIGN=center VALIGN=center>" )
   aadd( aHtm, '  <H2><FONT color=blue>' + cPath + '</FONT></H2>' )
   aadd( aHtm, " </BODY>" )
   aadd( aHtm, "</HTML>" )

   ::updateViewer( aHtm )

   RETURN Self

/*----------------------------------------------------------------------*/

METHOD IdeHarbourHelp:populateTextFile( cTextFile )
   LOCAL aHtm, aFn, oFunc
   LOCAL nParsed := ascan( ::aFuncByFile, {|e_| e_[ 1 ] == cTextFile } )

   /* Build HTML */
   aHtm := {}
   aadd( aHtm, "<HTML>" )
   aadd( aHtm, " <BODY>" )
   aadd( aHtm, '  <H3 align=center><FONT color=blue>' + cTextFile + '</FONT></H3>' )
   aadd( aHtm, '   <BR>' + '&nbsp;  <HR></HR></BR>' )
   IF nParsed > 0
      aFn := ::aFuncByFile[ nParsed, 2 ]
      IF len( aFn ) > 0
         FOR EACH oFunc IN aFn
            IF hb_isObject( oFunc )
               aadd( aHtm, '   <BR>' + oFunc:cName + '</BR>' )
            ENDIF
         NEXT
      ELSE
         aadd( aHtm, '   <BR><PRE>' + hb_memoread( cTextFile ) + '</PRE></BR>' )
      ENDIF
   ENDIF
   aadd( aHtm, " </BODY>" )
   aadd( aHtm, "</HTML>" )

   ::updateViewer( aHtm )

   RETURN Self

/*----------------------------------------------------------------------*/

METHOD IdeHarbourHelp:populateFuncDetails( n )
   LOCAL oTWItem := ::aNodes[ n, 1 ]
   LOCAL nIndex, oFunc

   nIndex := ascan( ::aFunctions, {|e_| e_[ 4 ] == oTWItem } )
   oFunc := ::aFunctions[ nIndex, 3 ]

   ::oUI:q_editTemplate    :setText( iif( empty( oFunc:cTemplate ), "FUNCTION", oFunc:cTemplate ) )
   ::oUI:q_editName        :setText( oFunc:cName        )
   ::oUI:q_editCategory    :setText( oFunc:cCategory    )
   ::oUI:q_editSubCategory :setText( oFunc:cSubCategory )
   ::oUI:q_editOneLiner    :setText( oFunc:cOneLiner    )
   ::oUI:q_editSeeAlso     :setText( oFunc:cSeaAlso     )
   ::oUI:q_editStatus      :setText( oFunc:cStatus      )
   ::oUI:q_editPlatforms   :setText( oFunc:cPlatforms   )

   ::oUI:q_editReturns     :setText( hbide_arrayToMemoEx( oFunc:aReturns ) )  //TODO : a line

   ::oUI:q_plainSyntax     :setPlainText( hbide_arrayToMemoEx2( oFunc:aSyntax      ) )
   ::oUI:q_plainFiles      :setPlainText( hbide_arrayToMemoEx2( oFunc:aFiles       ) )
   ::oUI:q_plainDescription:setPlainText( hbide_arrayToMemoEx2( oFunc:aDescription ) )
   ::oUI:q_plainExamples   :setPlainText( hbide_arrayToMemoEx2( oFunc:aExamples    ) )
   ::oUI:q_plainTests      :setPlainText( hbide_arrayToMemoEx2( oFunc:aTests       ) )
   ::oUI:q_plainArguments  :setPlainText( hbide_arrayToMemoEx2( oFunc:aArguments   ) )

   ::oUI:q_editTextPath    :setText( ::aFunctions[ nIndex, 1 ] )

   ::buildView( oFunc )

   RETURN Self

/*----------------------------------------------------------------------*/

METHOD IdeHarbourHelp:buildView( oFunc )
   LOCAL s, x, y, v, w, z, n, s1, a_, cTxt
   LOCAL aHtm := {}

   aadd( aHtm, "<HTML>" )

   aadd( aHtm, '<head>                                                             ' )
   aadd( aHtm, '  <meta name="Author" CONTENT=Pritpal Bedi [pritpal@vouchcac.com]">' )
   aadd( aHtm, '  <meta http-equiv="content-style-type" content="text/css" >       ' )
   aadd( aHtm, '  <meta http-equiv="content-script-type" content="text/javascript">' )
   aadd( aHtm, '                                                                   ' )
   aadd( aHtm, '  <style type="text/css">                                          ' )
   aadd( aHtm, '    a                                                              ' )
   aadd( aHtm, '    {                                                              ' )
   aadd( aHtm, '      text-decoration  : none;                                     ' )
   aadd( aHtm, '      color-hover      : #FF9900;                                  ' )
   aadd( aHtm, '    }                                                              ' )
   aadd( aHtm, '    th                                                             ' )
   aadd( aHtm, '    {                                                              ' )
   aadd( aHtm, '      colspan          : 1;                                        ' )
   aadd( aHtm, '      text-align       : center;                                   ' )
   aadd( aHtm, '      vertical-align   : baseline;                                 ' )
   aadd( aHtm, '      horizontal-align : left;                                     ' )
   aadd( aHtm, '    }                                                              ' )
   aadd( aHtm, '    td                                                             ' )
   aadd( aHtm, '    {                                                              ' )
   aadd( aHtm, '      vertical-align   : top;                                      ' )
   aadd( aHtm, '      horizontal-align : left;                                     ' )
   aadd( aHtm, '    }                                                              ' )
   aadd( aHtm, '    pre                                                            ' )
   aadd( aHtm, '    {                                                              ' )
   aadd( aHtm, '      font-family      : Courier New;                              ' )
   aadd( aHtm, '      font-size        : .9;                                       ' )
   aadd( aHtm, '      color            : black;                                    ' )
   aadd( aHtm, '      cursor           : text;                                     ' )
   aadd( aHtm, '    }                                                              ' )
   aadd( aHtm, '  </style>                                                         ' )
   aadd( aHtm, '</head>                                                            ' )

   aadd( aHtm, ' <BODY>'    )
   aadd( aHtm, '  <CENTER>' )

   s := '   <TABLE '            +;
        'Border='      + '0 '   +;
        'Frame='       + 'ALL ' +;
        'CellPadding=' + '0 '   +;
        'CellSpacing=' + '0 '   +;
        'Cols='        + '1 '   +;
        'Width='       + '95% ' +;
        '   >'
   aadd( aHtm, s )

   aadd( aHtm, '<CAPTION align=TOP><FONT SIZE="6"><B>' + oFunc:cName + '</B></FONT></CAPTION>' )
   //aadd( aHtm, '<BR><FONT color="#6699ff"><B>' + oFunc:cOneLiner + '</B></FONT></BR>' )
   aadd( aHtm, '<BR><FONT color="#FF4719"><B>' + oFunc:cOneLiner + '</B></FONT></BR>' )
   cTxt := " "
   IF !empty( oFunc:cCategory )
      cTxt += "Category: <B>" + oFunc:cCategory + "</B> "
   ENDIF
   IF !empty( oFunc:cSubCategory )
      cTxt += "Sub: <B>" + oFunc:cSubCategory + "</B> "
   ENDIF
   IF !empty( oFunc:cVersion )
      cTxt += "Version: <B>" + oFunc:cVersion + "</B> "
   ENDIF
   IF !empty( cTxt )
      aadd( aHtm, "<BR>" + "[" + cTxt + "]" + "</BR>" )
   ENDIF
   IF !empty( s1 := oFunc:cExternalLink )
      aadd( aHtm, '<BR><a href="' + s1 + '">' + "<B>" + s1 + "</B>" + "</a></BR>" )
   ENDIF
   aadd( aHtm, '<HR color="#6699ff" size="5"></HR>' )

   x := '<TR><TD align=LEFT><font size="5" color="#FF4719">' ; y := "</font></TD></TR>"
   v := '<TR><TD margin-left: 20px><pre>'                    ; w := "</pre></TD></TR>"
   z := "<TR><TD>&nbsp;</TD></TR>"

   IF !empty( oFunc:cInherits )
      aadd( aHtm, x + "Inherits"       + y )
      aadd( aHtm, v + oFunc:cInherits  + w )
      aadd( aHtm, z )
   ENDIF
   #if 0
   aadd( aHtm, x + "Category"       + y )
   aadd( aHtm, v + oFunc:cCategory  + w )
   aadd( aHtm, z )
   aadd( aHtm, x + "SubCategory"    + y )
   aadd( aHtm, v + oFunc:cSubCategory+ w )
   aadd( aHtm, z )
   #endif
   IF !empty( s := hbide_arrayToMemoHtml( oFunc:aSyntax ) )
      aadd( aHtm, x + "Syntax"         + y )
      aadd( aHtm, v + s + w )
      aadd( aHtm, z )
   ENDIF
   IF !empty( s := hbide_arrayToMemoHtml( oFunc:aArguments ) )
      aadd( aHtm, x + "Arguments"      + y )
      aadd( aHtm, v + s + w )
      aadd( aHtm, z )
   ENDIF
   IF !empty( s := hbide_arrayToMemoHtml( oFunc:aReturns ) )
      aadd( aHtm, x + "Returns"        + y )
      aadd( aHtm, v + s + w )
      aadd( aHtm, z )
   ENDIF
   IF !empty( s := hbide_arrayToMemoHtml( oFunc:aMethods ) )
      aadd( aHtm, x + "Methods"     + y )
      aadd( aHtm, v + s + w )
      aadd( aHtm, z )
   ENDIF
   IF !empty( s := hbide_arrayToMemoHtml( oFunc:aDescription ) )
      aadd( aHtm, x + "Description"    + y )
      aadd( aHtm, v + s + w )
      aadd( aHtm, z )
   ENDIF
   IF !empty( s := hbide_arrayToMemoHtml( oFunc:aExamples ) )
      aadd( aHtm, x + "Examples"       + y )
      aadd( aHtm, v + s + w )
      aadd( aHtm, z )
   ENDIF
   #if 0
   aadd( aHtm, x + "Vesrion"        + y )
   aadd( aHtm, v + oFunc:cVersion   + w )
   aadd( aHtm, z )
   #endif
   IF !empty( s := hbide_arrayToMemoHtml( oFunc:aFiles ) )
      aadd( aHtm, x + "Files"          + y )
      aadd( aHtm, v + s + w )
      aadd( aHtm, z )
   ENDIF
   a_:= hb_atokens( oFunc:cSeaAlso, "," )
   IF !empty( a_ )
      aadd( aHtm, x + "SeeAlso"        + y )
      aadd( aHtm, "<TR><TD>" )

      FOR EACH s IN a_
         s := alltrim( s )
         IF ( n := at( "(", s ) ) > 0
            s1 := substr( s, 1, n-1 )
         ELSE
            s1 := s
         ENDIF
         aadd( aHtm, '<a href="' + s1 + '">' + s + "</a>" + ;
                                     iif( s:__enumIndex() == len( a_ ), "", ",&nbsp;" ) )
      NEXT
      aadd( aHtm, "</TD></TR>" )
      aadd( aHtm, z )
   ENDIF
   IF !empty( oFunc:cPlatforms )
      aadd( aHtm, x + "Platforms"      + y )
      aadd( aHtm, v + oFunc:cPlatforms + w )
      aadd( aHtm, z )
   ENDIF
   IF !empty( oFunc:cStatus )
      aadd( aHtm, x + "Status"         + y )
      aadd( aHtm, v + oFunc:cStatus    + w )
      aadd( aHtm, z )
   ENDIF

   aadd( aHtm, "   </TABLE>"  )
   aadd( aHtm, "  </CENTER>"  )
   aadd( aHtm, " </BODY>"     )
   aadd( aHtm, "</HTML>"      )

   ::updateViewer( aHtm )

   RETURN Self

/*----------------------------------------------------------------------*/

METHOD IdeHarbourHelp:exportAsPdf()
   LOCAL cPdf, qPrinter, cExt, cPath, cFile

   IF !empty( cPdf := hbide_fetchAFile( ::oDlg, "Provide a file name", { { "Pdf Documents", "*.pdf" } } ) )
      hb_fNameSplit( cPdf, @cPath, @cFile, @cExt )
      IF empty( cExt ) .OR. lower( cExt ) != ".pdf"
         cPdf := cPath + cFile + ".pdf"
      ENDIF
      qPrinter := QPrinter():new()
      qPrinter:setOutputFileName( cPdf )
      ::oUI:q_browserView:print( qPrinter )
   ENDIF

   RETURN Self

/*----------------------------------------------------------------------*/

METHOD IdeHarbourHelp:print()
   LOCAL qDlg

   qDlg := QPrintPreviewDialog():new( ::oUI )
   qDlg:setWindowTitle( "Harbour Help Document" )
   Qt_Slots_Connect( ::pSlots, qDlg, "paintRequested(QPrinter)", {|p| ::paintRequested( p ) } )
   qDlg:exec()
   Qt_Slots_disConnect( ::pSlots, qDlg, "paintRequested(QPrinter)" )

   RETURN self

/*----------------------------------------------------------------------*/

METHOD IdeHarbourHelp:paintRequested( pPrinter )
   LOCAL qPrinter := QPrinter():configure( pPrinter )

   ::oUI:q_browserView:print( qPrinter )

   RETURN Self

/*----------------------------------------------------------------------*/

