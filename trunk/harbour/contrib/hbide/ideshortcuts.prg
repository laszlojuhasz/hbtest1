/*
 * $Id$
 */

/*
 * Harbour Project source code:
 *
 * Copyright 2010 Pritpal Bedi <bedipritpal@hotmail.com>
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
 *                              Harbour IDE
 *
 *                 Pritpal Bedi <bedipritpal@hotmail.com>
 *                               04Apr2010
 */
/*----------------------------------------------------------------------*/
/*----------------------------------------------------------------------*/
/*----------------------------------------------------------------------*/

#include "hbide.ch"
#include "hbqt.ch"
#include "common.ch"
#include "hbclass.ch"
#include "appevent.ch"

/*----------------------------------------------------------------------*/

#define listMethods_itemDoubleClicked             101
#define listMethods_currentRowChanged             102
#define tableMacros_itemSelectionChanged          103
#define tableMacros_itemDoubleClicked             104
#define buttonSet_clicked                         105
#define buttonNew_clicked                         106
#define buttonTest_clicked                        107
#define buttonLoad_clicked                        108
#define buttonSave_clicked                        109
#define buttonSaveAs_clicked                      110
#define buttonAssign_clicked                      111
#define buttonDelete_clicked                      112

/*----------------------------------------------------------------------*/

CLASS IdeShortcuts INHERIT IdeObject

   DATA   aHdr                                    INIT {}
   DATA   aKeys                                   INIT {}
   DATA   aMethods                                INIT {}
   DATA   aMtdItms                                INIT {}
   DATA   aDftSCuts                               INIT {}
   DATA   aDftSCutsItms                           INIT {}

   DATA   cName
   DATA   cKey
   DATA   cAlt
   DATA   cCtrl
   DATA   cShift
   DATA   cMenu
   DATA   cBlock
   DATA   cIcon

   DATA   qHiliter

   METHOD new( oIde )
   METHOD create( oIde )
   METHOD destroy()
   METHOD show()
   METHOD execEvent( nMode, p )
   METHOD buildUI()
   METHOD buildSignals()

   METHOD buildBlock( cString )
   METHOD evalMacro( cString )
   METHOD test( cString, lWarn )
   METHOD execKey( nKey, lAlt, lCtrl, lShift )
   METHOD mergeMacros( a_ )

   METHOD loadDftSCuts()
   METHOD loadMethods()
   METHOD loadKeys()

   METHOD clearDftSCuts()
   METHOD populateData( nMode )
   METHOD populateDftSCuts()
   METHOD populateKeys()
   METHOD populateMethods()
   METHOD checkDuplicate( cKey, cAlt, cCtrl, cShift, nRow )
   METHOD controls2vrbls()
   METHOD vrbls2controls( nRow )
   METHOD array2controls( nRow )
   METHOD array2table( nRow, a_ )
   METHOD vrbls2array( nRow )

   /* Public API Methods */
   METHOD getWord( lSelect )
   METHOD getLine( lSelect )
   METHOD getText()
   METHOD execTool( ... )

   /* hbIDE defined Macros as API Methods */
   METHOD help( cTopic )
   METHOD exit( lWarn )
   METHOD newSource( cType )
   METHOD open()
   METHOD save()
   METHOD saveAs()
   METHOD saveAll()
   METHOD close()
   METHOD print()
   METHOD revertToSaved()
   METHOD findDlg()
   METHOD findDlgEx()
   METHOD gotoLine( nLine )
   METHOD duplicateLine()
   METHOD deleteLine()
   METHOD moveLineUp()
   METHOD moveLineDown()
   METHOD indentRight()
   METHOD indentLeft()
   METHOD blockComment()
   METHOD streamComment()
   METHOD build( cProj )
   METHOD buildLaunch( cProj )
   METHOD reBuild( cProj )
   METHOD reBuildLaunch( cProj )
   METHOD launch( cProj )
   METHOD insert( cText )
   METHOD separator( cSep )
   METHOD findAgain()
   METHOD replace()
   METHOD toUpper()
   METHOD toLower()
   METHOD invertCase()
   METHOD zoom( nKey )
   METHOD cut()
   METHOD copy()
   METHOD paste()
   METHOD undo()
   METHOD redo()
   METHOD selectAll()
   METHOD setBookMark()
   METHOD gotoMark()
   METHOD switchToReadOnly()
   METHOD dlgKeyboardMappings()
   METHOD dlgToolsAndUtils()
   METHOD setView( cView )
   METHOD compilePPO()
   METHOD single2doubleQuotes()
   METHOD double2singleQuotes()
   METHOD tabs2spaces()
   METHOD removeTrailingSpaces()
   METHOD toggleLineNumbersDisplay()
   METHOD toggleColumnSelectionMode()
   METHOD toggleStatusBar()
   METHOD toggleLineSelectionMode()
   METHOD presentSkeletons()
   METHOD gotoFunction()
   METHOD clearSelection()
   METHOD execPlugin( cPlugin, ... )
   METHOD toggleCurrentLineHilight()

   ENDCLASS

/*----------------------------------------------------------------------*/

METHOD IdeShortcuts:new( oIde )

   ::oIde := oIde

   RETURN Self

/*----------------------------------------------------------------------*/

METHOD IdeShortcuts:create( oIde )

   DEFAULT oIde TO ::oIde
   ::oIde := oIde

   ::loadMethods()
   ::loadKeys()
   ::loadDftSCuts()

   RETURN Self

/*----------------------------------------------------------------------*/

METHOD IdeShortcuts:destroy()
   LOCAL a_, qItm

   IF !empty( ::oUI )
      Qt_Events_disConnect( ::pEvents, ::oUI:oWidget, QEvent_Close )
      ::oUI:oWidget:removeEventFilter( ::pEvents )

      ::qHiliter := NIL

      FOR EACH qItm IN ::aHdr
         qItm := NIL
      NEXT
      ::aHdr := {}

      FOR EACH qItm IN ::aMtdItms
         qItm := NIL
      NEXT
      ::aMtdItms := {}

      FOR EACH a_ IN ::aDftSCutsItms
         FOR EACH qItm IN a_
            qItm := NIL
         NEXT
      NEXT
      ::aDftSCutsItms := {}

      ::oUI:destroy()
   ENDIF

   RETURN Self

/*----------------------------------------------------------------------*/

METHOD IdeShortcuts:show()

   IF empty( ::oUI )
      ::buildUI()
      ::populateData( 1 )
      ::oIde:setPosAndSizeByIni( ::oUI:oWidget, FindInFilesDialogGeometry )
   ENDIF

   ::oUI:show()
   ::oUI:raise()

   RETURN Self

/*----------------------------------------------------------------------*/

METHOD IdeShortcuts:execEvent( nMode, p )
   LOCAL nRow, cMethod, cFile, cPath, cTemp, cExt, a_

   SWITCH nMode

   CASE buttonDelete_clicked
      nRow := ::oUI:q_tableMacros:currentRow()
      IF nRow >= 0 .AND. nRow < len( ::aDftSCuts )
         nRow++
         IF hbide_getYesNo( "Delete", ::aDftSCuts[ nRow, 1 ], "A Delete Operation Requested" )
            hb_adel( ::aDftSCuts, nRow, .t. )
            ::clearDftSCuts()
            ::populateDftSCuts()
         ENDIF
         IF nRow <= len( ::aDftSCuts )
            ::oUI:q_tableMacros:setCurrentCell( nRow - 1, 0 )
         ENDIF
      ENDIF
      EXIT
   CASE buttonTest_clicked
      IF .t.
         ::controls2vrbls()
         IF !empty( ::cBlock )
            ::test( ::cBlock, .t. )
         ENDIF
      ENDIF
      EXIT
   CASE buttonLoad_clicked
      cTemp := hbide_fetchAFile( ::oDlg, "Select a macro file", { { "hbIDE Script File", "*.scu" } }, ::cPathShortcuts )
      IF ! Empty( cTemp )
         hb_fNameSplit( hbide_pathNormalized( cTemp, .f. ), @cPath, @cFile, @cExt )
         IF lower( cExt ) == ".scu"
            a_:= hb_deSerialize( hb_memoread( cTemp ) )
            IF !empty( a_ )
               ::mergeMacros( a_ )
               ::clearDftSCuts()
               ::populateDftSCuts()
            ENDIF
         ENDIF
      ENDIF
      EXIT
   CASE buttonSave_clicked
      hbide_saveShortcuts( ::oIde, ::aDftSCuts )
      EXIT
   CASE buttonSaveAs_clicked
      cTemp := hbide_saveAFile( ::oDlg, "Select a macro file", { { "hbIDE Script File", "*.scu" } }, ::cPathShortcuts, ".scu" )
      IF ! Empty( cTemp )
         hb_fNameSplit( hbide_pathNormalized( cTemp, .f. ), @cPath, @cFile, @cExt )
         cFile := hbide_pathToOSPath( cPath + cFile + "scu" )
         hbide_saveShortcuts( ::oIde, ::aDftSCuts, cFile )
      ENDIF
      EXIT
   CASE buttonNew_clicked
      IF .t.
         ::controls2vrbls()
         IF !empty( ::cName )
            IF !( ::checkDuplicate( ::cKey, ::cAlt, ::cCtrl, ::cShift ) )
               aadd( ::aDftSCuts, { ::cName, ::cKey, ::cAlt, ::cCtrl, ::cShift, ::cMenu, ::cBlock, ::cIcon } )
               aadd( ::aDftSCutsItms, array( 6 ) )
               ::oUI:q_tableMacros:setRowCount( ::oUI:q_tableMacros:rowCount() + 1 )
               ::array2table( len( ::aDftSCuts ), { ::cName, ::cKey, ::cAlt, ::cCtrl, ::cShift, ::cMenu, ::cBlock, ::cIcon } )
            ELSE
               MsgBox( "Current shortcut is already defined!" )
            ENDIF
         ENDIF
      ENDIF
      EXIT

   CASE buttonSet_clicked
      nRow := ::oUI:q_tableMacros:currentRow()
      IF nRow >= 0 .AND. nRow < len( ::aDftSCuts )
         nRow++
         ::controls2vrbls()
         IF !empty( ::cName ) .AND. !( ::checkDuplicate( ::cKey, ::cAlt, ::cCtrl, ::cShift, nRow ) ) .AND. ::test( ::cBlock, .f. )
            ::vrbls2array( nRow )
            ::vrbls2controls( nRow )
         ENDIF
      ENDIF
      EXIT
   CASE tableMacros_itemDoubleClicked
      EXIT
   CASE tableMacros_itemSelectionChanged
      nRow := ::oUI:q_tableMacros:currentRow()
      IF nRow >= 0 .AND. nRow < len( ::aDftSCuts )
         nRow++
         ::array2controls( nRow )
      ENDIF
      EXIT
   CASE listMethods_itemDoubleClicked
      IF ( nRow := ::oUI:q_listMethods:currentRow() ) >= 0
         nRow++
         cMethod := "::" + ::aMethods[ nRow, 2 ]
         ::oUI:q_plainBlock:insertPlainText( cMethod )
      ENDIF
      EXIT
   CASE listMethods_currentRowChanged
      IF p >= 0 .AND. p < len( ::aMethods )
         ::oUI:q_texteditSyntax:setPlainText( ::aMethods[ p+1, 3 ] )
      ENDIF
      EXIT
   ENDSWITCH

   RETURN Self

/*----------------------------------------------------------------------*/

METHOD IdeShortcuts:array2controls( nRow )
   LOCAL cKey, nKey

   ::oUI:q_editName:setText( ::aDftSCuts[ nRow, 1 ] )

   cKey := ::aDftSCuts[ nRow, 2 ]
   IF ( nKey := ascan( ::aKeys, {|e_| e_[ 2 ] == cKey } ) ) > 0
      ::oUI:q_comboKey:setCurrentIndex( nKey - 1 )
   ENDIF

   ::oUI:q_checkAlt  :setChecked( ::aDftSCuts[ nRow, 3 ] == "YES" )
   ::oUI:q_checkCtrl :setChecked( ::aDftSCuts[ nRow, 4 ] == "YES" )
   ::oUI:q_checkShift:setChecked( ::aDftSCuts[ nRow, 5 ] == "YES" )

   ::oUI:q_editMenu:setText( ::aDftSCuts[ nRow, 6 ] )

   ::oUI:q_plainBlock:setPlainText( ::aDftSCuts[ nRow, 7 ] )

   RETURN Self

/*----------------------------------------------------------------------*/

METHOD IdeShortcuts:vrbls2array( nRow )

   IF nRow == NIL
      aadd( ::aDftSCuts, array( 7 ) )
      nRow := len( ::aDftSCuts )
   ENDIF

   ::aDftSCuts[ nRow, 1 ] := ::cName
   ::aDftSCuts[ nRow, 2 ] := ::cKey
   ::aDftSCuts[ nRow, 3 ] := ::cAlt
   ::aDftSCuts[ nRow, 4 ] := ::cCtrl
   ::aDftSCuts[ nRow, 5 ] := ::cShift
   ::aDftSCuts[ nRow, 6 ] := ::cMenu
   ::aDftSCuts[ nRow, 7 ] := ::cBlock
   //::aDftSCuts[ nRow, 8 ] := ::cIcon

   RETURN Self

/*----------------------------------------------------------------------*/

METHOD IdeShortcuts:vrbls2controls( nRow )

   ::aDftSCutsItms[ nRow, 1 ]:setIcon( hbide_image( ::cIcon ) )
   ::aDftSCutsItms[ nRow, 2 ]:setText( ::cName )
   ::aDftSCutsItms[ nRow, 3 ]:setText( ::cKey )
   ::aDftSCutsItms[ nRow, 4 ]:setIcon( hbide_image( iif( ::cAlt   == "YES", "check", "" ) ) )
   ::aDftSCutsItms[ nRow, 5 ]:setIcon( hbide_image( iif( ::cCtrl  == "YES", "check", "" ) ) )
   ::aDftSCutsItms[ nRow, 6 ]:setIcon( hbide_image( iif( ::cShift == "YES", "check", "" ) ) )

   RETURN Self

/*----------------------------------------------------------------------*/

METHOD IdeShortcuts:array2table( nRow, a_ )
   LOCAL q0, q1, q2, q3, q4, q5
   LOCAL oTbl := ::oUI:q_tableMacros
   LOCAL n := nRow - 1

   q0 := QTableWidgetItem():new()
   q0:setIcon( hbide_image( a_[ 8 ] ) )
   oTbl:setItem( n, 0, q0 )

   q1 := QTableWidgetItem():new()
   q1:setText( a_[ 1 ] )
   oTbl:setItem( n, 1, q1 )

   q2 := QTableWidgetItem():new()
   q2:setText( a_[ 2 ] )
   oTbl:setItem( n, 2, q2 )

   q3 := QTableWidgetItem():new()
   q3:setIcon( iif( a_[ 3 ] == "YES", hbide_image( "check" ), "" ) )
   oTbl:setItem( n, 3, q3 )

   q4 := QTableWidgetItem():new()
   q4:setIcon( iif( a_[ 4 ] == "YES", hbide_image( "check" ), "" ) )
   oTbl:setItem( n, 4, q4 )

   q5 := QTableWidgetItem():new()
   q5:setIcon( iif( a_[ 5 ] == "YES", hbide_image( "check" ), "" ) )
   oTbl:setItem( n, 5, q5 )

   oTbl:setRowHeight( n, 16 )

   ::aDftSCutsItms[ nRow, 1 ] := q0
   ::aDftSCutsItms[ nRow, 2 ] := q1
   ::aDftSCutsItms[ nRow, 3 ] := q2
   ::aDftSCutsItms[ nRow, 4 ] := q3
   ::aDftSCutsItms[ nRow, 5 ] := q4
   ::aDftSCutsItms[ nRow, 6 ] := q5

   RETURN Self

/*----------------------------------------------------------------------*/

METHOD IdeShortcuts:controls2vrbls()
   LOCAL nRow := ::oUI:q_comboKey:currentIndex()

   IF nRow >= 0
      nRow++
      ::cName  := ::oUI:q_editName:text()
      ::cKey   := ::aKeys[ nRow, 2 ]
      ::cAlt   := iif( ::oUI:q_checkAlt  :isChecked(), "YES", "NO" )
      ::cCtrl  := iif( ::oUI:q_checkCtrl :isChecked(), "YES", "NO" )
      ::cShift := iif( ::oUI:q_checkShift:isChecked(), "YES", "NO" )
      ::cMenu  := ::oUI:q_editMenu:text()
      ::cBlock := ::oUI:q_plainBlock:toPlainText()
   ENDIF
   RETURN Self

/*----------------------------------------------------------------------*/

METHOD IdeShortcuts:checkDuplicate( cKey, cAlt, cCtrl, cShift, nRow )
   LOCAL lYes, e_

   IF empty( nRow )
      lYes := ascan( ::aDftSCuts, {|e_| e_[ 2 ] == cKey .AND. e_[ 3 ] == cAlt .AND. ;
                                                   e_[ 4 ] == cCtrl .AND. e_[ 5 ] == cShift } ) > 0
   ELSE
      lYes := .f.
      FOR EACH e_ IN ::aDftSCuts
         IF e_:__enumIndex() != nRow
            IF e_[ 2 ] == cKey .AND. e_[ 3 ] == cAlt .AND. e_[ 4 ] == cCtrl .AND. e_[ 5 ] == cShift
               lYes := .t.
               EXIT
            ENDIF
         ENDIF
      NEXT
   ENDIF

   RETURN lYes

/*----------------------------------------------------------------------*/

METHOD IdeShortcuts:buildUI()
   LOCAL oTbl, n, qItm
   LOCAL hdr_:= { { "Img", 30 }, { "Name", 190 }, { "Key", 50 }, { "Alt", 30 }, { "Ctrl", 30 }, { "Shift", 30 } }

   ::oUI := HbQtUI():new( hbide_uic( "shortcuts" ) ):build()
   ::oUI:setWindowIcon( hbide_image( "hbide" ) )
   ::oUI:setParent( ::oDlg:oWidget )
   ::oUI:setWindowFlags( Qt_Sheet )

   ::oUI:oWidget:installEventFilter( ::pEvents )

   Qt_Events_Connect( ::pEvents, ::oUI:oWidget, QEvent_Close, {|| ::oIde:aIni[ INI_HBIDE, FindInFilesDialogGeometry ] := hbide_posAndSize( ::oUI:oWidget ) } )

   oTbl := ::oUI:q_tableMacros                              /* Build Table Header */
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

   ::oUI:q_listMethods:setAlternatingRowColors( .t. )       /* Public Methods List */

   ::qHiliter := ::oTH:SetSyntaxHilighting( ::oUI:q_plainBlock, "Pritpal's Favourite" )

   ::buildSignals()

   RETURN Self

/*----------------------------------------------------------------------*/

METHOD IdeShortcuts:buildSignals()

   ::oUI:signal( "buttonNew"   , "clicked()"                   , {| | ::execEvent( buttonNew_clicked    ) } )
   ::oUI:signal( "buttonSet"   , "clicked()"                   , {| | ::execEvent( buttonSet_clicked    ) } )
   ::oUI:signal( "buttonTest"  , "clicked()"                   , {| | ::execEvent( buttonTest_clicked   ) } )
   ::oUI:signal( "buttonLoad"  , "clicked()"                   , {| | ::execEvent( buttonLoad_clicked   ) } )
   ::oUI:signal( "buttonSave"  , "clicked()"                   , {| | ::execEvent( buttonSave_clicked   ) } )
   ::oUI:signal( "buttonSaveAs", "clicked()"                   , {| | ::execEvent( buttonSaveAs_clicked ) } )
   ::oUI:signal( "buttonAssign", "clicked()"                   , {| | ::execEvent( buttonAssign_clicked ) } )
   ::oUI:signal( "buttonDelete", "clicked()"                   , {| | ::execEvent( buttonDelete_clicked ) } )

   ::oUI:signal( "listMethods" , "itemDoubleClicked(QLWItem)"  , {|p| ::execEvent( listMethods_itemDoubleClicked, p ) } )
   ::oUI:signal( "listMethods" , "currentRowChanged(int)"      , {|p| ::execEvent( listMethods_currentRowChanged, p ) } )

   ::oUI:signal( "tableMacros" , "itemSelectionChanged()"      , {| | ::execEvent( tableMacros_itemSelectionChanged ) } )
   ::oUI:signal( "tableMacros" , "itemDoubleClicked(QTblWItem)", {|p| ::execEvent( tableMacros_itemDoubleClicked, p ) } )

   RETURN Self

/*----------------------------------------------------------------------*/

METHOD IdeShortcuts:populateData( nMode )

   IF nMode == 1
      ::populateMethods()
      ::populateKeys()
      ::populateDftSCuts()
   ENDIF

   RETURN Self

/*----------------------------------------------------------------------*/

METHOD IdeShortcuts:clearDftSCuts()
   LOCAL a_, qItm
   LOCAL oTbl := ::oUI:q_tableMacros

   FOR EACH a_ IN ::aDftSCutsItms
      FOR EACH qItm IN a_
         qItm := NIL
      NEXT
   NEXT
   ::aDftSCutsItms := {}

   oTbl:clearContents()

   RETURN Self

/*----------------------------------------------------------------------*/

METHOD IdeShortcuts:populateDftSCuts()
   LOCAL a_, nRow
   LOCAL oTbl := ::oUI:q_tableMacros

   oTbl:setRowCount( len( ::aDftSCuts ) )

   nRow := 0
   FOR EACH a_ IN ::aDftSCuts
      nRow++
      aadd( ::aDftSCutsItms, array( 6 ) )
      ::array2table( nRow, a_ )
      QApplication():new():processEvents()
   NEXT
   oTbl:setCurrentCell( 0,0 )

   RETURN Self

/*----------------------------------------------------------------------*/

METHOD IdeShortcuts:populateMethods()
   LOCAL qItem, a_
   LOCAL qLW := ::oUI:q_listMethods

   //qLW:setSortingEnabled( .t. )

   FOR EACH a_ IN ::aMethods
      IF !empty( a_[ 1 ] )
         qItem := QListWidgetItem():new()
         qItem:setText( a_[ 1 ] )
         aadd( ::aMtdItms, qItem )
         qLW:addItem_1( qItem )
      ENDIF
   NEXT
   qLW:setCurrentRow( 0 )

   RETURN Self

/*----------------------------------------------------------------------*/

METHOD IdeShortcuts:populateKeys()
   LOCAL a_
   LOCAL oCB := ::oUI:q_comboKey

   FOR EACH a_ IN ::aKeys
      oCB:addItem( a_[ 2 ] )
   NEXT
   oCB:setCurrentIndex( -1 )

   RETURN Self

/*----------------------------------------------------------------------*/

METHOD IdeShortcuts:buildBlock( cString )
   LOCAL n, cBlock, cParam
   LOCAL a_:= hbide_memoTOarray( cString )

   cString := ""
   aeval( a_, {|e| cString += e } )

   IF ( n := at( "|", cString ) ) > 0
      cString := substr( cString, n + 1 )
      IF ( n := at( "|", cString ) ) == 0
         RETURN Self
      ENDIF
      cParam  := substr( cString, 1, n - 1 )
      cString := substr( cString, n + 1 )
      cBlock  := "{|o," + cParam + "|" + cString + " }"
   ELSE
      cBlock := "{|o| " + cString + " }"
   ENDIF
   cBlock := strtran( cBlock, "::", "o:" )

   RETURN cBlock

/*----------------------------------------------------------------------*/

METHOD IdeShortcuts:test( cString, lWarn )
   LOCAL cBlock, oErr, bBlock
   LOCAL lOk    := .f.
   LOCAL bError := ErrorBlock( {|o| break( o ) } )

   cBlock := ::buildBlock( cString )

   BEGIN SEQUENCE
      bBlock := &( cBlock )
      lOk := .t.
      IF lWarn
         MsgBox( "Script compiles fine!", "Syntax checking", , , , bBlock )
      ENDIF
   RECOVER USING oErr
      MsgBox( "Wrongly defined script, try: |v| ::method( v )", oErr:description )
   ENDSEQUENCE

   ErrorBlock( bError )
   ::oUI:raise()
   ::oUI:setFocus_1()

   RETURN lOk

/*----------------------------------------------------------------------*/

METHOD IdeShortcuts:evalMacro( cString )
   LOCAL bError := ErrorBlock( {|o| break( o ) } )
   LOCAL oErr, bBlock, cBlock
   LOCAL lEvaluated := .f.

   cBlock := ::buildBlock( cString ) ; HB_TRACE( HB_TR_ALWAYS, cString, cBlock )

   bBlock := &( cBlock )

   BEGIN SEQUENCE
      eval( bBlock, self )
      lEvaluated := .t.
   RECOVER USING oErr
      HB_TRACE( HB_TR_ALWAYS, valtype( oErr ), oErr:description )
   END SEQUENCE

   ErrorBlock( bError )
   RETURN lEvaluated

/*----------------------------------------------------------------------*/

METHOD IdeShortcuts:execKey( nKey, lAlt, lCtrl, lShift )
   LOCAL lExecuted := .f.
   LOCAL cKey, n

   IF ( n := ascan( ::aKeys, {|e_| e_[ 1 ] == nKey } ) ) > 0
      cKey := ::aKeys[ n, 2 ]

      n := ascan( ::aDftSCuts, {|e_| e_[ 2 ] == cKey .AND. ;
                                     e_[ 3 ] == iif( lAlt  , "YES", "NO" ) .AND. ;
                                     e_[ 4 ] == iif( lCtrl , "YES", "NO" ) .AND. ;
                                     e_[ 5 ] == iif( lShift, "YES", "NO" )  } )
      IF n > 0
         IF ! empty( ::aDftSCuts[ n, 7 ] )
            HB_TRACE( HB_TR_ALWAYS, nKey, lAlt, lCtrl, lShift, cKey )

            lExecuted := ::evalMacro( ::aDftSCuts[ n, 7 ] )
         ENDIF
      ENDIF
   ENDIF

   RETURN lExecuted

/*----------------------------------------------------------------------*/

METHOD IdeShortcuts:loadKeys()
   LOCAL a_

   aadd( ::aKeys, { Qt_Key_Escape        , "Escape          " } )
   aadd( ::aKeys, { Qt_Key_Tab           , "Tab             " } )
   aadd( ::aKeys, { Qt_Key_Backtab       , "Backtab         " } )
   aadd( ::aKeys, { Qt_Key_Backspace     , "Backspace       " } )
   aadd( ::aKeys, { Qt_Key_Return        , "Return          " } )
   aadd( ::aKeys, { Qt_Key_Enter         , "Enter           " } )
   aadd( ::aKeys, { Qt_Key_Insert        , "Insert          " } )
   aadd( ::aKeys, { Qt_Key_Delete        , "Delete          " } )
   aadd( ::aKeys, { Qt_Key_Pause         , "Pause           " } )
   aadd( ::aKeys, { Qt_Key_Print         , "Print           " } )
   aadd( ::aKeys, { Qt_Key_SysReq        , "SysReq          " } )
   aadd( ::aKeys, { Qt_Key_Clear         , "Clear           " } )
   aadd( ::aKeys, { Qt_Key_Home          , "Home            " } )
   aadd( ::aKeys, { Qt_Key_End           , "End             " } )
   aadd( ::aKeys, { Qt_Key_Left          , "Left            " } )
   aadd( ::aKeys, { Qt_Key_Up            , "Up              " } )
   aadd( ::aKeys, { Qt_Key_Right         , "Right           " } )
   aadd( ::aKeys, { Qt_Key_Down          , "Down            " } )
   aadd( ::aKeys, { Qt_Key_PageUp        , "PageUp          " } )
   aadd( ::aKeys, { Qt_Key_PageDown      , "PageDown        " } )
   aadd( ::aKeys, { Qt_Key_CapsLock      , "CapsLock        " } )
   aadd( ::aKeys, { Qt_Key_NumLock       , "NumLock         " } )
   aadd( ::aKeys, { Qt_Key_ScrollLock    , "ScrollLock      " } )
   aadd( ::aKeys, { Qt_Key_F1            , "F1              " } )
   aadd( ::aKeys, { Qt_Key_F2            , "F2              " } )
   aadd( ::aKeys, { Qt_Key_F3            , "F3              " } )
   aadd( ::aKeys, { Qt_Key_F4            , "F4              " } )
   aadd( ::aKeys, { Qt_Key_F5            , "F5              " } )
   aadd( ::aKeys, { Qt_Key_F6            , "F6              " } )
   aadd( ::aKeys, { Qt_Key_F7            , "F7              " } )
   aadd( ::aKeys, { Qt_Key_F8            , "F8              " } )
   aadd( ::aKeys, { Qt_Key_F9            , "F9              " } )
   aadd( ::aKeys, { Qt_Key_F10           , "F10             " } )
   aadd( ::aKeys, { Qt_Key_F11           , "F11             " } )
   aadd( ::aKeys, { Qt_Key_F12           , "F12             " } )
   aadd( ::aKeys, { Qt_Key_F13           , "F13             " } )
   aadd( ::aKeys, { Qt_Key_F14           , "F14             " } )
   aadd( ::aKeys, { Qt_Key_F15           , "F15             " } )
   aadd( ::aKeys, { Qt_Key_F16           , "F16             " } )
   aadd( ::aKeys, { Qt_Key_F17           , "F17             " } )
   aadd( ::aKeys, { Qt_Key_F18           , "F18             " } )
   aadd( ::aKeys, { Qt_Key_F19           , "F19             " } )
   aadd( ::aKeys, { Qt_Key_F20           , "F20             " } )
   aadd( ::aKeys, { Qt_Key_F21           , "F21             " } )
   aadd( ::aKeys, { Qt_Key_F22           , "F22             " } )
   aadd( ::aKeys, { Qt_Key_F23           , "F23             " } )
   aadd( ::aKeys, { Qt_Key_F24           , "F24             " } )
   aadd( ::aKeys, { Qt_Key_F25           , "F25             " } )
   aadd( ::aKeys, { Qt_Key_F26           , "F26             " } )
   aadd( ::aKeys, { Qt_Key_F27           , "F27             " } )
   aadd( ::aKeys, { Qt_Key_F28           , "F28             " } )
   aadd( ::aKeys, { Qt_Key_F29           , "F29             " } )
   aadd( ::aKeys, { Qt_Key_F30           , "F30             " } )
   aadd( ::aKeys, { Qt_Key_F31           , "F31             " } )
   aadd( ::aKeys, { Qt_Key_F32           , "F32             " } )
   aadd( ::aKeys, { Qt_Key_F33           , "F33             " } )
   aadd( ::aKeys, { Qt_Key_F34           , "F34             " } )
   aadd( ::aKeys, { Qt_Key_F35           , "F35             " } )
   aadd( ::aKeys, { Qt_Key_Space         , "Space           " } )
   aadd( ::aKeys, { Qt_Key_Exclam        , "Exclam          " } )
   aadd( ::aKeys, { Qt_Key_QuoteDbl      , "QuoteDbl        " } )
   aadd( ::aKeys, { Qt_Key_NumberSign    , "NumberSign      " } )
   aadd( ::aKeys, { Qt_Key_Dollar        , "Dollar          " } )
   aadd( ::aKeys, { Qt_Key_Percent       , "Percent         " } )
   aadd( ::aKeys, { Qt_Key_Ampersand     , "Ampersand       " } )
   aadd( ::aKeys, { Qt_Key_Apostrophe    , "Apostrophe      " } )
   aadd( ::aKeys, { Qt_Key_ParenLeft     , "ParenLeft       " } )
   aadd( ::aKeys, { Qt_Key_ParenRight    , "ParenRight      " } )
   aadd( ::aKeys, { Qt_Key_Asterisk      , "Asterisk        " } )
   aadd( ::aKeys, { Qt_Key_Plus          , "Plus            " } )
   aadd( ::aKeys, { Qt_Key_Comma         , "Comma           " } )
   aadd( ::aKeys, { Qt_Key_Minus         , "Minus           " } )
   aadd( ::aKeys, { Qt_Key_Period        , "Period          " } )
   aadd( ::aKeys, { Qt_Key_Slash         , "Slash           " } )
   aadd( ::aKeys, { Qt_Key_0             , "0               " } )
   aadd( ::aKeys, { Qt_Key_1             , "1               " } )
   aadd( ::aKeys, { Qt_Key_2             , "2               " } )
   aadd( ::aKeys, { Qt_Key_3             , "3               " } )
   aadd( ::aKeys, { Qt_Key_4             , "4               " } )
   aadd( ::aKeys, { Qt_Key_5             , "5               " } )
   aadd( ::aKeys, { Qt_Key_6             , "6               " } )
   aadd( ::aKeys, { Qt_Key_7             , "7               " } )
   aadd( ::aKeys, { Qt_Key_8             , "8               " } )
   aadd( ::aKeys, { Qt_Key_9             , "9               " } )
   aadd( ::aKeys, { Qt_Key_Colon         , "Colon           " } )
   aadd( ::aKeys, { Qt_Key_Semicolon     , "Semicolon       " } )
   aadd( ::aKeys, { Qt_Key_Less          , "Less            " } )
   aadd( ::aKeys, { Qt_Key_Equal         , "Equal           " } )
   aadd( ::aKeys, { Qt_Key_Greater       , "Greater         " } )
   aadd( ::aKeys, { Qt_Key_Question      , "Question        " } )
   aadd( ::aKeys, { Qt_Key_At            , "At              " } )
   aadd( ::aKeys, { Qt_Key_A             , "A               " } )
   aadd( ::aKeys, { Qt_Key_B             , "B               " } )
   aadd( ::aKeys, { Qt_Key_C             , "C               " } )
   aadd( ::aKeys, { Qt_Key_D             , "D               " } )
   aadd( ::aKeys, { Qt_Key_E             , "E               " } )
   aadd( ::aKeys, { Qt_Key_F             , "F               " } )
   aadd( ::aKeys, { Qt_Key_G             , "G               " } )
   aadd( ::aKeys, { Qt_Key_H             , "H               " } )
   aadd( ::aKeys, { Qt_Key_I             , "I               " } )
   aadd( ::aKeys, { Qt_Key_J             , "J               " } )
   aadd( ::aKeys, { Qt_Key_K             , "K               " } )
   aadd( ::aKeys, { Qt_Key_L             , "L               " } )
   aadd( ::aKeys, { Qt_Key_M             , "M               " } )
   aadd( ::aKeys, { Qt_Key_N             , "N               " } )
   aadd( ::aKeys, { Qt_Key_O             , "O               " } )
   aadd( ::aKeys, { Qt_Key_P             , "P               " } )
   aadd( ::aKeys, { Qt_Key_Q             , "Q               " } )
   aadd( ::aKeys, { Qt_Key_R             , "R               " } )
   aadd( ::aKeys, { Qt_Key_S             , "S               " } )
   aadd( ::aKeys, { Qt_Key_T             , "T               " } )
   aadd( ::aKeys, { Qt_Key_U             , "U               " } )
   aadd( ::aKeys, { Qt_Key_V             , "V               " } )
   aadd( ::aKeys, { Qt_Key_W             , "W               " } )
   aadd( ::aKeys, { Qt_Key_X             , "X               " } )
   aadd( ::aKeys, { Qt_Key_Y             , "Y               " } )
   aadd( ::aKeys, { Qt_Key_Z             , "Z               " } )
   aadd( ::aKeys, { Qt_Key_BracketLeft   , "BracketLeft     " } )
   aadd( ::aKeys, { Qt_Key_Backslash     , "Backslash       " } )
   aadd( ::aKeys, { Qt_Key_BracketRight  , "BracketRight    " } )
   aadd( ::aKeys, { Qt_Key_AsciiCircum   , "AsciiCircum     " } )
   aadd( ::aKeys, { Qt_Key_Underscore    , "Underscore      " } )
   aadd( ::aKeys, { Qt_Key_QuoteLeft     , "QuoteLeft       " } )
   aadd( ::aKeys, { Qt_Key_BraceLeft     , "BraceLeft       " } )
   aadd( ::aKeys, { Qt_Key_Bar           , "Bar             " } )
   aadd( ::aKeys, { Qt_Key_BraceRight    , "BraceRight      " } )
   aadd( ::aKeys, { Qt_Key_AsciiTilde    , "AsciiTilde      " } )

   FOR EACH a_ IN ::aKeys
      a_[ 2 ] := trim( a_[ 2 ] )
   NEXT
   RETURN Self

/*----------------------------------------------------------------------*/
METHOD IdeShortcuts:execTool( ... )
   RETURN ::oTM:execTool( ... )
/*----------------------------------------------------------------------*/
METHOD IdeShortcuts:getWord( lSelect )
   RETURN ::oEM:getWord( lSelect )
/*----------------------------------------------------------------------*/
METHOD IdeShortcuts:getLine( lSelect )
   RETURN ::oEM:getLine( , lSelect )
/*----------------------------------------------------------------------*/
METHOD IdeShortcuts:getText()
   RETURN ::oEM:getText()
/*----------------------------------------------------------------------*/
METHOD IdeShortcuts:duplicateLine()
   RETURN ::oEM:duplicateLine()
/*----------------------------------------------------------------------*/
METHOD IdeShortcuts:deleteLine()
   RETURN ::oEM:deleteLine()
/*----------------------------------------------------------------------*/
METHOD IdeShortcuts:moveLineUp()
   RETURN ::oEM:moveLine( -1 )
/*----------------------------------------------------------------------*/
METHOD IdeShortcuts:moveLineDown()
   RETURN ::oEM:moveLine( 1 )
/*----------------------------------------------------------------------*/
METHOD IdeShortcuts:help( cTopic )
   HB_SYMBOL_UNUSED( cTopic )
   RETURN ::oHelpDock:show()
/*----------------------------------------------------------------------*/
METHOD IdeShortcuts:exit( lWarn )
   IF hb_isLogical( lWarn ) .AND. lWarn
      IF hbide_getYesNo( "Exit hbIDE ?", , "Macro Executed" )
         PostAppEvent( xbeP_Close, NIL, NIL, ::oDlg )
      ENDIF
   ELSE
      PostAppEvent( xbeP_Close, NIL, NIL, ::oDlg )
   ENDIF
   RETURN Self
/*----------------------------------------------------------------------*/
METHOD IdeShortcuts:newSource( cType )
   HB_SYMBOL_UNUSED( cType )
   RETURN ::oSM:editSource( '' )
/*----------------------------------------------------------------------*/
METHOD IdeShortcuts:open()
   RETURN ::oSM:openSource()
/*----------------------------------------------------------------------*/
METHOD IdeShortcuts:save()
   RETURN ::oSM:saveSource( ::oEM:getTabCurrent(), .f., .f. )
/*----------------------------------------------------------------------*/
METHOD IdeShortcuts:saveAs()
   RETURN ::oSM:saveSource( ::oEM:getTabCurrent(), .t., .t. )
/*----------------------------------------------------------------------*/
METHOD IdeShortcuts:saveAll()
   RETURN ::oSM:saveAllSources()
/*----------------------------------------------------------------------*/
METHOD IdeShortcuts:close()
   RETURN ::oSM:closeSource()
/*----------------------------------------------------------------------*/
METHOD IdeShortcuts:print()
   RETURN ::oEM:printPreview()
/*----------------------------------------------------------------------*/
METHOD IdeShortcuts:revertToSaved()
   RETURN Self
/*----------------------------------------------------------------------*/
METHOD IdeShortcuts:findDlg()
   IF !Empty( ::qCurEdit )
      ::oFR:show()
   ENDIF
   RETURN Self
/*----------------------------------------------------------------------*/
METHOD IdeShortcuts:findDlgEx()
   IF !Empty( ::qCurEdit )
      ::oSearchReplace:beginFind()
   ENDIF
   RETURN Self
/*----------------------------------------------------------------------*/
METHOD IdeShortcuts:gotoLine( nLine )
   RETURN ::oEM:goTo( nLine )
/*----------------------------------------------------------------------*/
METHOD IdeShortcuts:indentRight()
   RETURN ::oEM:indent( 1 )
/*----------------------------------------------------------------------*/
METHOD IdeShortcuts:indentLeft()
   RETURN ::oEM:indent( -1 )
/*----------------------------------------------------------------------*/
METHOD IdeShortcuts:blockComment()
   RETURN ::oEM:blockComment()
/*----------------------------------------------------------------------*/
METHOD IdeShortcuts:streamComment()
   RETURN ::oEM:streamComment()
/*----------------------------------------------------------------------*/
METHOD IdeShortcuts:build( cProj )
   IF ! hb_isChar( cProj )
      cProj := ""
   ENDIF
   RETURN ::oPM:buildProject( cProj, .F., .F. )
/*----------------------------------------------------------------------*/
METHOD IdeShortcuts:buildLaunch( cProj )
   IF ! hb_isChar( cProj )
      cProj := ""
   ENDIF
   RETURN ::oPM:buildProject( cProj, .T., .F. )
/*----------------------------------------------------------------------*/
METHOD IdeShortcuts:reBuild( cProj )
   IF ! hb_isChar( cProj )
      cProj := ""
   ENDIF
   RETURN ::oPM:buildProject( cProj, .F., .T. )
/*----------------------------------------------------------------------*/
METHOD IdeShortcuts:reBuildLaunch( cProj )
   IF ! hb_isChar( cProj )
      cProj := ""
   ENDIF
   RETURN ::oPM:buildProject( cProj, .T., .T. )
/*----------------------------------------------------------------------*/
METHOD IdeShortcuts:launch( cProj )
   IF ! hb_isChar( cProj )
      cProj := ""
   ENDIF
   RETURN ::oPM:launchProject( cProj )
/*----------------------------------------------------------------------*/
METHOD IdeShortcuts:insert( cText )
   LOCAL oEdit
   IF ! empty( oEdit := ::getEditObjectCurrent() )
      IF hb_isChar( cText )
         oEdit:insertText( cText )
      ENDIF
   ENDIF
   RETURN Self
/*----------------------------------------------------------------------*/
METHOD IdeShortcuts:separator( cSep )
   RETURN ::oEM:insertSeparator( cSep )
/*----------------------------------------------------------------------*/
METHOD IdeShortcuts:findAgain()
   IF !empty( ::qCurEdit )
      ::oFR:find()
   ENDIF
   RETURN Self
/*----------------------------------------------------------------------*/
METHOD IdeShortcuts:replace()
   IF !empty( ::qCurEdit )
      ::oFR:replace()
   ENDIF
   RETURN Self
/*----------------------------------------------------------------------*/
METHOD IdeShortcuts:toUpper()
   RETURN ::oEM:convertSelection( "ToUpper" )
/*----------------------------------------------------------------------*/
METHOD IdeShortcuts:toLower()
   RETURN ::oEM:convertSelection( "ToLower" )
/*----------------------------------------------------------------------*/
METHOD IdeShortcuts:invertCase()
   RETURN ::oEM:convertSelection( "Invert" )
/*----------------------------------------------------------------------*/
METHOD IdeShortcuts:zoom( nKey )
   RETURN ::oEM:zoom( nKey )
/*----------------------------------------------------------------------*/
METHOD IdeShortcuts:cut()
   RETURN ::oEM:cut()
/*----------------------------------------------------------------------*/
METHOD IdeShortcuts:copy()
   RETURN ::oEM:copy()
/*----------------------------------------------------------------------*/
METHOD IdeShortcuts:paste()
   RETURN ::oEM:paste()
/*----------------------------------------------------------------------*/
METHOD IdeShortcuts:selectAll()
   RETURN ::oEM:selectAll()
/*----------------------------------------------------------------------*/
METHOD IdeShortcuts:redo()
   RETURN ::oEM:redo()
/*----------------------------------------------------------------------*/
METHOD IdeShortcuts:undo()
   RETURN ::oEM:undo()
/*----------------------------------------------------------------------*/
METHOD IdeShortcuts:setBookMark()
   RETURN ::oEM:setMark()
/*----------------------------------------------------------------------*/
METHOD IdeShortcuts:gotoMark()
   RETURN ::oEM:gotoMark()
/*----------------------------------------------------------------------*/
METHOD IdeShortcuts:switchToReadOnly()
   RETURN ::oEM:switchToReadOnly()
/*----------------------------------------------------------------------*/
METHOD IdeShortcuts:dlgKeyboardMappings()
   RETURN ::oTM:show()
/*----------------------------------------------------------------------*/
METHOD IdeShortcuts:dlgToolsAndUtils()
   RETURN ::oSC:show()
/*----------------------------------------------------------------------*/
METHOD IdeShortcuts:setView( cView )
   IF empty( cView )
      cView := "Stats"
   ENDIF
   RETURN ::oDK:setView( cView )
/*----------------------------------------------------------------------*/
METHOD IdeShortcuts:compilePPO()
   RETURN ::oPM:buildProject( '', .F., .F., .T., .T. )
/*----------------------------------------------------------------------*/
METHOD IdeShortcuts:single2doubleQuotes()
   RETURN ::oEM:convertDQuotes()
/*----------------------------------------------------------------------*/
METHOD IdeShortcuts:double2singleQuotes()
   RETURN ::oEM:convertQuotes()
/*----------------------------------------------------------------------*/
METHOD IdeShortcuts:tabs2spaces()
   RETURN ::oEM:removeTabs()
/*----------------------------------------------------------------------*/
METHOD IdeShortcuts:removeTrailingSpaces()
   RETURN ::oEM:removeTrailingSpaces()
/*----------------------------------------------------------------------*/
METHOD IdeShortcuts:toggleLineNumbersDisplay()
   RETURN ::oEM:toggleLineNumbers()
/*----------------------------------------------------------------------*/
METHOD IdeShortcuts:toggleStatusBar()
   IF ::lStatusBarVisible
      ::oSBar:oWidget:hide()
   ELSE
      ::oSBar:oWidget:show()
   ENDIF
   ::oIde:lStatusBarVisible := ! ::lStatusBarVisible
   RETURN Self
/*----------------------------------------------------------------------*/
METHOD IdeShortcuts:presentSkeletons()
   RETURN ::oEM:presentSkeletons()
/*----------------------------------------------------------------------*/
METHOD IdeShortcuts:gotoFunction()
   RETURN ::oEM:gotoFunction()
/*----------------------------------------------------------------------*/
METHOD IdeShortcuts:execPlugin( cPlugin, ... )
   RETURN hbide_execPlugin( cPlugin, ::oIde, ... )
/*----------------------------------------------------------------------*/
METHOD IdeShortcuts:toggleCurrentLineHilight()
   RETURN ::oEM:toggleCurrentLineHighlightMode()
/*----------------------------------------------------------------------*/
METHOD IdeShortcuts:toggleColumnSelectionMode()
   RETURN ::oEM:toggleColumnSelectionMode()
/*----------------------------------------------------------------------*/
METHOD IdeShortcuts:toggleLineSelectionMode()
   RETURN ::oEM:toggleLineSelectionMode()
/*----------------------------------------------------------------------*/
METHOD IdeShortcuts:clearSelection()
   RETURN ::oEM:clearSelection()
/*----------------------------------------------------------------------*/

METHOD IdeShortcuts:loadMethods()

   aadd( ::aMethods, { 'help( cTopic )', ;
                       'help( "" )', ;
                       'Invokes "Help" docking widget in the right docking area. <cTopic> is not active yet.'  } )
   aadd( ::aMethods, { 'exit( lWarn )', ;
                       'exit( .f. )', ;
                       'Exits hbIDE. If <lWarn == TRUE> then confirmation is requested through a popup dialog. All sources are saved if in modified state after confirmation to do so.'  } )
   aadd( ::aMethods, { 'newSource( cType )', ;
                       'newSource( "" )', ;
                       'Initiates a blank source file in an editing instance on the current panel.'  } )
   aadd( ::aMethods, { 'open()', ;
                       'open()', ;
                       'Invokes "Open File" dialog and if a selection is made and such selection is a hbIDE supported valid text file, that is opened in a new editor instance on visible panel.'  } )
   aadd( ::aMethods, { 'save()', ;
                       'save()', ;
                       'Saves the current editing instance if in modified state. Visual artifacts are updated accordingly.'  } )
   aadd( ::aMethods, { 'saveAs()', ;
                       'saveAs()', ;
                       'Opens "File Save Dialog" to fetch a file name and saves the current editing instance into new file. Visual artifacts are updated accordingly.'  } )
   aadd( ::aMethods, { 'saveAll()', ;
                       'saveAll()', ;
                       'Saves all opened editing instances on the visible panel, if in modified state. Visual artifacts are updated accordingly.'  } )
   aadd( ::aMethods, { 'close()', ;
                       'close()', ;
                       'Closes the current editing instance.'  } )
   aadd( ::aMethods, { 'print()', ;
                       'print()', ;
                       'Invokes "Print Preview" dialog with current source contents ready to be printed.'  } )
   aadd( ::aMethods, { 'findDlg()', ;
                       'findDlg()', ;
                       'Invokes "Find and Replace" dialog.'  } )
   aadd( ::aMethods, { 'findAgain()', ;
                       'findAgain()', ;
                       'Finds last search string without opening the dialog.'  } )
   aadd( ::aMethods, { 'replace()', ;
                       'replace()', ;
                       'Replaces last replace string if some text is already selected without opening the dialog.'  } )
   aadd( ::aMethods, { 'findDlgEx()', ;
                       'findDlgEx()', ;
                       'Invokes extended "Find and Replace" dialog at the bottom of editing area.'  } )
   aadd( ::aMethods, { 'gotoLine( nLine )', ;
                       'gotoLine(  )', ;
                       'Attempt is made to position the cursor at <nLine>. If <nLine> is not supplied, a "Goto" dialog is opened to supply <nLine>.'  } )
   aadd( ::aMethods, { 'indentRight()', ;
                       'indentRight()', ;
                       'Pushes one character right the currently selected text.'  } )
   aadd( ::aMethods, { 'indentLeft()', ;
                       'indentLeft()', ;
                       'Pushes one character left the currently selected text. If there are no columns remains at left nothing happens.'  } )
   aadd( ::aMethods, { 'blockComment()', ;
                       'blockComment()', ;
                       'Encloses currently selected text in line comments where each line is prefixed with //.' } )
   aadd( ::aMethods, { 'streamComment()', ;
                       'streamComment()', ;
                       'Encloses currently selected text in Anci-C like comments /*  */' } )
   aadd( ::aMethods, { 'toUpper()', ;
                       'toUpper()', ;
                       'Converts currently selected text to upper-case.' } )
   aadd( ::aMethods, { 'toLower()', ;
                       'toLower()', ;
                       'Converts currently selected text to lower-case.' } )
   aadd( ::aMethods, { 'invertCase()', ;
                       'invertCase()', ;
                       'Inverts case of currently selected text: upper => lower; lower => upper.' } )
   aadd( ::aMethods, { 'build( cProj )', ;
                       'build( "" )', ;
                       'Builds <cProj> if it is already loaded. All sources are saved if found in modified state before "build" is initiated.' } )
   aadd( ::aMethods, { 'buildLaunch( cProj )', ;
                       'buildLaunch( "" )', ;
                       'Builds and launches <cProj> if it is already loaded. All sources are saved if found in modified state before "build" is initiated.' } )
   aadd( ::aMethods, { 'reBuild( cProj )', ;
                       'reBuild( "" )', ;
                       'Re-builds <cProj> if it is already loaded. All sources are saved if found in modified state before "build" is initiated.' } )
   aadd( ::aMethods, { 'reBuildLaunch( cProj )', ;
                       'reBuildLaunch( "" )', ;
                       'Re-builds and launches <cProj> if it is already loaded. All sources are saved if found in modified state before "build" is initiated.' } )
   aadd( ::aMethods, { 'launch( cProj )'   , ;
                       'launch( "" )'      , ;
                       'Launches <cProj> if it is already loaded.'  } )
   aadd( ::aMethods, { 'insert( cText )'   , ;
                       'insert( "" )'      , ;
                       'Insert <cText> at current cursor position.'  } )
   aadd( ::aMethods, { 'separator( cSep )' , ;
                       'separator( ' + '/*' + replicate( "-", 68 ) + '*/' + ' )', ;
                       'Inserts separator line <cSep> immediately before current line. <cSep> defaults to "/*---*/"'  } )
   aadd( ::aMethods, { 'getWord( lSelect )', ;
                       'getWord( .f. )'    , ;
                       'Returns text of the word under cursor. If <lSelect == .T.> text appears as selected.' } )
   aadd( ::aMethods, { 'getLine( lSelect )', ;
                       'getLine( .f. )'    , ;
                       'Returns text of the current line. If <lSelect == .T.> text appears as selected.' } )
   aadd( ::aMethods, { 'getText()'         , ;
                       'getText()'         , ;
                       'Returns current selected text.' } )
   aadd( ::aMethods, { 'execTool( cName )' , ;
                       'execTool( "" )'    , ;
                       'Executes a Tool defined and visible under tools menu.' } )
   aadd( ::aMethods, { 'execTool( cCmd, cParams, cStartIn, lCapture, lShowOutput )' , ;
                       'execTool( "", "", "", .f., .f. )', ;
                       'Executes a program or file with parameters and other attributes.' + CRLF + ;
                       'http://hbide.vouch.info/ ( Topic: Tools and Utilities )' } )
   aadd( ::aMethods, { 'zoom( nVal )'      , ;
                       'zoom( +1 )'        , ;
                       'Zooms in/out the current editing instance. nVal: 1-one size up; -1-one size less; NIL-original size; 5~30-to size.' } )
   aadd( ::aMethods, { 'cut()'             , ;
                       'cut()'             , ;
                       'Cuts the selected text and copies onto clipboard.' } )
   aadd( ::aMethods, { 'copy()'            , ;
                       'copy()'            , ;
                       'Copies the selected text onto clipboard.' } )
   aadd( ::aMethods, { 'paste()'           , ;
                       'paste()'           , ;
                       'Pastes the text available onto clipboard at the current cursor position.' } )
   aadd( ::aMethods, { 'undo()'            , ;
                       'undo()'            , ;
                       'Attempts to undo the last operation if one still hangs in the buffer.' } )
   aadd( ::aMethods, { 'redo()'            , ;
                       'redo()'            , ;
                       'Attempts to re-do the last undone action.' } )
   aadd( ::aMethods, { 'selectAll()'       , ;
                       'selectAll()'       , ;
                       'Selects the whole text and places the cursor at the end of source.' } )
   aadd( ::aMethods, { 'print()'           , ;
                       'print()'           , ;
                       'Opens Print Preview dialog which will contain the source line broken by pages.' } )
   aadd( ::aMethods, { 'setBookMark()'     , ;
                       'setBookMark()'     , ;
                       'Attempts to install a bookmark onto current cursor position. If successful, mark appears as a colored tool-button on the statusbar and marked line is highlighted with the same color.' } )
   aadd( ::aMethods, { 'gotoMark( nMark )' , ;
                       'gotoMark( 1 )'     , ;
                       'Attempts to reach mark represented by <nMark>. Please note maximum 6 bookmarks are avialable per edit instance.' } )
   aadd( ::aMethods, { 'switchToReadOnly()', ;
                       'switchToReadOnly()', ;
                       'Flags current editing instance read-only.' } )
   aadd( ::aMethods, { 'dlgKeyboardMappings()', ;
                       'dlgKeyboardMappings()', ;
                       'Opens "Keyboard Mappings" dialog.' } )
   aadd( ::aMethods, { 'dlgToolsAndUtils()', ;
                       'dlgToolsAndUtils()', ;
                       'Opens "Toola & Utilities" dialog.' } )
   aadd( ::aMethods, { 'setView( cView )'  , ;
                       'setView( "" )'     , ;
                       'Brings <cView> panel to front. Defaults to "Stats", i.e., <Welcome> tab.' } )
   aadd( ::aMethods, { 'compilePPO()'      , ;
                       'compilePPO()'      , ;
                       'Attemps to compile current source to .ppo formats, and if successful, presents the compiled source in a new edit instance.' } )
   aadd( ::aMethods, { 'single2doubleQuotes()', ;
                       'single2doubleQuotes()', ;
                       'Converts single quotes to double in the currently selected text.' } )
   aadd( ::aMethods, { 'double2singleQuotes()', ;
                       'double2singleQuotes()', ;
                       'Converts double quotes to single in the currently selected text.' } )
   aadd( ::aMethods, { 'tabs2spaces()', ;
                       'tabs2spaces()', ;
                       'Converts tabs to spaces, currently 3, the entire source. However, source is not saved.' } )
   aadd( ::aMethods, { 'removeTrailingSpaces()', ;
                       'removeTrailingSpaces()', ;
                       'Removes trailing spaces per line, the entire source. However, source is not saved.' } )
   aadd( ::aMethods, { 'toggleLineNumbersDisplay()', ;
                       'toggleLineNumbersDisplay()', ;
                       'Toggles line numbers display inside editing instances. This action has global scope and is saved for next run.' } )
   aadd( ::aMethods, { 'toggleColumnSelectionMode()', ;
                       'toggleColumnSelectionMode()', ;
                       'Toggles selection mode from "stream" to "column" or vice-versa.' } )
   aadd( ::aMethods, { 'toggleLineSelectionMode()', ;
                       'toggleLineSelectionMode()', ;
                       'Toggles line selection mode.' } )
   aadd( ::aMethods, { 'clearSelection()', ;
                       'clearSelection()', ;
                       'Clears the selection block, if any, and resets the selection mode to stream.' } )
   aadd( ::aMethods, { 'toggleStatusBar()', ;
                       'toggleStatusBar()', ;
                       'Toggles display of statusbar. The action is not saved for next run.' } )
   aadd( ::aMethods, { 'presentSkeletons()', ;
                       'presentSkeletons()', ;
                       'Present snippets for selection.' } )
   aadd( ::aMethods, { 'gotoFunction()', ;
                       'gotoFunction()', ;
                       'Takes under-cursor word and attempts to open the source containing that function in a new tab.' } )
   aadd( ::aMethods, { 'execPlugin( cPlugin )', ;
                       'execPlugin( "" )', ;
                       'Attempts to execute third-party plugins. First parameter passed is the instance to SELF exposing public API methods. Next parameters are passes as a list.' } )
   aadd( ::aMethods, { 'toggleCurrentLineHilight()', ;
                       'toggleCurrentLineHilight()', ;
                       'Toggles the mode to highlight current line or not in the editor. The effect is global. Setting is not retained for next run' } )

   RETURN Self

/*----------------------------------------------------------------------*/

METHOD IdeShortcuts:loadDftSCuts()
   LOCAL a_, b_

   IF .t.
      b_:= {}

      /*          Name                Key        Alt   Ctrl   Sh   Menu  Expr                      Icon */
      //
      aadd( b_, { "Help"            , "F1"     , "NO", "NO" , "NO" , "", '::help( "" )'          , "help"            , "", "" } )
      aadd( b_, { "Exit"            , "W"      , "NO", "YES", "YES", "", '::exit( .f. )'         , "exit"            , "", "" } )

      aadd( b_, { "Cut"             , "X"      , "NO", "YES", "NO" , "", '::cut()'               , "cut"             , "", "" } )
      aadd( b_, { "Copy"            , "C"      , "NO", "YES", "NO" , "", '::copy()'              , "copy"            , "", "" } )
      aadd( b_, { "Paste"           , "V"      , "NO", "YES", "NO" , "", '::paste()'             , "paste"           , "", "" } )
      aadd( b_, { "Undo"            , "Z"      , "NO", "YES", "NO" , "", '::undo()'              , "undo"            , "", "" } )
      aadd( b_, { "Redo"            , "Y"      , "NO", "YES", "NO" , "", '::redo()'              , "redo"            , "", "" } )
      aadd( b_, { "SelectAll"       , "A"      , "NO", "YES", "NO" , "", '::selectAll()'         , "selectall"       , "", "" } )

      aadd( b_, { "New Source"      , "N"      , "NO", "YES", "NO" , "", '::newSource( "" )'     , "new"             , "", "" } )
      aadd( b_, { "Open"            , "O"      , "NO", "YES", "NO" , "", '::open()'              , "open"            , "", "" } )
      aadd( b_, { "Save"            , "S"      , "NO", "YES", "NO" , "", '::save()'              , "save"            , "", "" } )
      aadd( b_, { "Save All"        , "S"      , "NO", "YES", "YES", "", '::saveAll()'           , "saveall"         , "", "" } )
      aadd( b_, { "Close"           , "W"      , "NO", "YES", "NO" , "", '::close()'             , "close"           , "", "" } )
      aadd( b_, { "Print"           , "P"      , "NO", "YES", "NO" , "", '::print()'             , "print"           , "", "" } )
   *  aadd( b_, { "Revert to Saved" , "R"      , "NO", "NO" , "YES", "", '::revertToSaved()'     , ""                , "", "" } )

      aadd( b_, { "Find Dialog"     , "F"      , "NO", "YES", "NO" , "", '::findDlg()'           , "find"            , "", "" } )
      aadd( b_, { "Find Again"      , "N"      , "NO", "YES", "NO" , "", '::findAgain()'         , ""                , "", "" } )
      aadd( b_, { "Replace"         , "R"      , "NO", "YES", "NO" , "", '::replace()'           , ""                , "", "" } )
      aadd( b_, { "Find Dialog Ex"  , "F"      , "NO", "YES", "YES", "", '::findDlgEx()'         , "find"            , "", "" } )

      aadd( b_, { "Goto Line"       , "G"      , "NO", "YES", "NO" , "", '::gotoLine()'          , "gotoline"        , "", "" } )
      aadd( b_, { "Duplicate Line"  , "D"      , "NO", "YES", "NO" , "", '::duplicateLine()'     , "duplicateline"   , "", "" } )
      aadd( b_, { "Delete Line"     , "Delete" , "NO", "YES", "NO" , "", '::deleteLine()'        , "deleteline"      , "", "" } )
      aadd( b_, { "Line Up"         , "Up"     , "NO", "YES", "YES", "", '::moveLineUp()'        , "movelineup"      , "", "" } )
      aadd( b_, { "Line Down"       , "Down"   , "NO", "YES", "YES", "", '::moveLineDown()'      , "movelinedown"    , "", "" } )

      aadd( b_, { "Indent Right"    , "Tab"    , "NO", "YES", "NO" , "", '::indentRight()'       , "blockindentr"    , "", "" } )
      aadd( b_, { "Indent Left"     , "Tab"    , "NO", "YES", "YES", "", '::indentLeft()'        , "blockindentl"    , "", "" } )
      aadd( b_, { "Block Comment"   , "Slash"  , "NO", "YES", "YES", "", '::blockComment()'      , "blockcomment"    , "", "" } )
      aadd( b_, { "Stream Comment"  , "Q"      , "NO", "YES", "YES", "", '::streamComment()'     , "streamcomment"   , "", "" } )

      aadd( b_, { "Build Project"   , "F9"     , "NO", "YES", "NO" , "", '::build( "" )'         , "build"           , "", "" } )
      aadd( b_, { "Build & Launch"  , "F9"     , "NO", "NO" , "NO" , "", '::buildLaunch( "" )'   , "buildlaunch"     , "", "" } )
      aadd( b_, { "Launch Project"  , "F10"    , "NO", "YES", "NO" , "", '::launch( "" )'        , "launch"          , "", "" } )

      aadd( b_, { "Insert Text"     , "F7"     , "NO", "YES", "NO" , "", '::insert( "" )'        , "insert-external-file", "", "" } )
      aadd( b_, { "Insert Separator", "F7"     , "NO", "NO" , "NO" , "", '::separator( "" )'     , "insert-separator", "", "" } )

      aadd( b_, { "Toggle Line Selection Mode", "F11", "NO", "NO" , "NO" , "", '::toggleLineSelectionMode()', ""     , "", "" } )
      aadd( b_, { "Clear Selection" , "F11"    , "NO", "NO" , "YES", "", '::clearSelection()'    , ""                , "", "" } )
      aadd( b_, { "Present Snippets", "K"      , "NO", "YES", "NO" , "", '::presentSkeletons()'  , ""                , "", "" } )
      aadd( b_, { "Goto Function"   , "T"      , "NO", "YES", "NO" , "", '::gotoFunction()'      , ""                , "", "" } )

      ::aDftSCuts := b_
   ENDIF

   IF !empty( a_:= hbide_loadShortcuts( ::oIde ) )
      ::mergeMacros( a_ )
   ENDIF

   RETURN Self

/*----------------------------------------------------------------------*/

METHOD IdeShortcuts:mergeMacros( a_ )
   LOCAL c_, n

   FOR EACH c_ IN a_
      IF ( n := ascan( ::aDftSCuts, {|e_| e_[ 2 ] == c_[ 2 ] .AND. e_[ 3 ] == c_[ 3 ] .AND. ;
                                          e_[ 4 ] == c_[ 4 ] .AND. e_[ 5 ] == c_[ 5 ] } ) ) == 0
         aadd( ::aDftSCuts, c_ )
      ELSE
         ::aDftSCuts[ n ] := c_
      ENDIF
   NEXT

   RETURN Self

/*----------------------------------------------------------------------*/
   #if 0
   CASE "Environments"
      ::oEV:fetchNew()
      EXIT
   CASE "Animate"
      ::nAnimantionMode := iif( ::nAnimantionMode == HBIDE_ANIMATION_NONE, HBIDE_ANIMATION_GRADIENT, HBIDE_ANIMATION_NONE )
      ::oDK:animateComponents( ::nAnimantionMode )
      EXIT
   CASE "Help"
      ::oHelpDock:show()
      EXIT
   CASE "Goto"
      ::oEM:goTo()
      EXIT
   CASE "FormatBraces"
      ::oEM:formatBraces()
      EXIT
   CASE "SaveExit"
      ::oSM:saveAndExit()
      EXIT
   CASE "Revert"
      ::oSM:RevertSource()
      EXIT
   CASE "CloseAll"
      ::oSM:closeAllSources()
      EXIT
   CASE "CloseOther"
      ::oSM:closeAllOthers()
      EXIT
   CASE "NewProject"
      ::oPM:loadProperties( , .t., .t., .t. )
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
   #endif
/*----------------------------------------------------------------------*/
