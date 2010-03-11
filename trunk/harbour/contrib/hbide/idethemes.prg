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
 *                               31Dec2009
 */
/*----------------------------------------------------------------------*/
/*----------------------------------------------------------------------*/
/*----------------------------------------------------------------------*/

#include "common.ch"
#include "hbclass.ch"
#include "hbqt.ch"
#include "hbide.ch"

/*----------------------------------------------------------------------*/

#define THM_ATR_R                                 1
#define THM_ATR_G                                 2
#define THM_ATR_B                                 3
#define THM_ATR_ITALIC                            4
#define THM_ATR_BOLD                              5
#define THM_ATR_ULINE                             6

#define THM_NUM_ATTRBS                            6

#define applyMenu_triggered_applyToCurrentTab     1
#define applyMenu_triggered_setAsDefault          2
#define applyMenu_triggered_applyToAllTabs        3
#define listThemes_currentRowChanged              4
#define listItems_currentRowChanged               5

/*----------------------------------------------------------------------*/

FUNCTION hbide_loadThemes( oIde )

   IF empty( oIde:cIniThemes )
      oIde:cIniThemes := hb_dirBase() + "hbide.hbt"
   ENDIF

   oIde:oThemes := IdeThemes():new( oIde, oIde:cIniThemes ):create()

   RETURN nil

/*----------------------------------------------------------------------*/

CLASS IdeThemes INHERIT IdeObject

   VAR    lDefault                                INIT .t.
   VAR    cIniFile                                INIT ""

   VAR    aIni                                    INIT {}
   VAR    aThemes                                 INIT {}
   VAR    aControls                               INIT {}
   VAR    aItems                                  INIT {}
   VAR    aPatterns                               INIT {}
   VAR    aApplyAct                               INIT {}
   VAR    nCurTheme                               INIT 1
   VAR    nCurItem                                INIT 1

   VAR    qEdit
   VAR    qHiliter
   VAR    qMenuApply

   VAR    lCreating                               INIT .f.

   METHOD new( oIde, cIniFile )
   METHOD create( oIde, cIniFile )
   METHOD destroy()
   METHOD setWrkTheme( cTheme )
   METHOD contains( cTheme )
   METHOD load( cFile )
   METHOD save( lAsk )
   METHOD execEvent( nMode, p )
   METHOD getThemeAttribute( cAttr, cTheme )
   METHOD buildSyntaxFormat( aAttr )
   METHOD setForeBackGround( qEdit, cTheme )
   METHOD setQuotesRule( qHiliter, cTheme )
   METHOD setMultiLineCommentRule( qHiliter, cTheme )
   METHOD setSingleLineCommentRule( qHiliter, cTheme )
   METHOD setSyntaxRule( qHiliter, cName, cPattern, aAttr )
   METHOD setSyntaxFormat( qHiliter, cName, aAttr )
   METHOD setSyntaxHilighting( qEdit, cTheme, lNew )
   METHOD show()
   METHOD copy()
   METHOD setTheme()
   METHOD setAttributes()
   METHOD updateColor()
   METHOD updateAttribute( nAttr, iState )
   METHOD selectTheme()
   METHOD selectThemeProc( nMode, p, oSL, cTheme )
   METHOD buildINI()
   METHOD parseINI( lAppend )
   METHOD updateLineNumbersBkColor()
   METHOD updateCurrentLineColor()

   ENDCLASS

/*----------------------------------------------------------------------*/

METHOD IdeThemes:new( oIde, cIniFile )

   ::oIde  := oIde
   ::cIniFile := cIniFile

   RETURN Self

/*----------------------------------------------------------------------*/

METHOD IdeThemes:create( oIde, cIniFile )
   LOCAL s, b_

   DEFAULT oIde     TO ::oIde
   DEFAULT cIniFile TO ::cIniFile

   ::oIde  := oIde
   ::cIniFile := cIniFile

   /* next always load default themes */
   ::aIni := hbide_loadDefaultThemes()
   ::parseINI()

   /* first load user defined themes */
   ::load( ::cIniFile )

   /* These are the supported patterns - rest will be ignore until implemented */

   /* Compiler Directives */
   b_:= { "include","define","ifndef","ifdef","else","endif","command","xcommand","translate","xtranslate" }
   s := ""; aeval( b_, {|e| s += iif( empty( s ), "", "|" ) + "#" + upper( e ) + "\b|#" + e + "\b" } )
   aadd( ::aPatterns, { "PreprocessorDirectives", s } )

   /* Harbour Keywords */
   b_:= { 'function','return','static','local','default', ;
          'if','else','elseif','endif','end', 'endswitch', ;
          'docase','case','endcase','otherwise', 'switch', ;
          'do','while','exit',;
          'for','each','next','step','to',;
          'class','endclass','method','data','var','destructor','inline','assign','access',;
          'inherit','init','create','virtual','message',;
          'begin','sequence','try','catch','always','recover','hb_symbol_unused', ;
          'error','handler' }
   s := ""; aeval( b_, {|e| s += iif( empty( s ), "", "|" ) + "\b" + upper( e ) + "\b|\b" + e + "\b" } )
   aadd( ::aPatterns, { "HarbourKeywords"   , s } )

   /* C Language Keywords - Only for C or CPP sources - mutually exclusive with Harbour Sources */
   b_:= { "char", "class", "const", "double", "enum", "explicit", "friend", "inline", ;
          "int",  "long", "namespace", "operator", "private", "protected", "public", ;
          "short", "signals", "signed", "slots", "static", "struct", "template", ;
          "typedef", "typename", "union", "unsigned", "virtual", "void", "volatile" }
   s := ""; aeval( b_, {|e| s += iif( empty( s ), "", "|" ) + "\b" + e + "\b" } )
   aadd( ::aPatterns, { "CLanguageKeywords" , s                       } )

   s := "\:\=|\:|\+|\-|\\|\*|\ IN\ |\ in\ |\=|\>|\<|\^|\%|\$|\&|\@|\.or\.|\.and\.|\.OR\.|\.AND\.|\!"
   aadd( ::aPatterns, { "Operators"         , s                       } )

   aadd( ::aPatterns, { "NumericalConstants", "\b[0-9.]+\b"           } )

   aadd( ::aPatterns, { "BracketsAndBraces" , "\(|\)|\{|\}|\[|\]|\|"  } )

   aadd( ::aPatterns, { "FunctionsBody"     , "\b[A-Za-z0-9_]+(?=\()" } )

   RETURN Self

/*----------------------------------------------------------------------*/

METHOD IdeThemes:destroy()

   IF !empty( ::oUI )
      ::qHiliter := NIL
      ::qEdit    := NIL

      ::oThemesDock:oWidget:setWidget( QWidget():new() )
      IF !empty( ::oUI )
         ::oUI:destroy()
      ENDIF
   ENDIF

   RETURN Self

/*----------------------------------------------------------------------*/

METHOD IdeThemes:execEvent( nMode, p )
   LOCAL oEditor, a_

   HB_SYMBOL_UNUSED( p )

   DO CASE
   CASE nMode == listItems_currentRowChanged
      ::nCurItem  := p+1

      IF ::nCurItem == 13
         ::updateCurrentLineColor()
      ELSEIF ::nCurItem == 16
         ::updateLineNumbersBkColor()
      ELSE
         ::setAttributes( p )
      ENDIF

   CASE nMode == listThemes_currentRowChanged
      ::nCurTheme := p+1
      ::setTheme( p )

   CASE nMode == applyMenu_triggered_applyToAllTabs
      FOR EACH a_ IN ::aTabs
         a_[ TAB_OEDITOR ]:applyTheme( ::aThemes[ ::nCurTheme, 1 ] )
      NEXT

   CASE nMode == applyMenu_triggered_applyToCurrentTab
      IF !empty( oEditor := ::oEM:getEditorCurrent() )
         oEditor:applyTheme( ::aThemes[ ::nCurTheme, 1 ] )
      ENDIF

   CASE nMode == applyMenu_triggered_setAsDefault
      ::setWrkTheme( ::aThemes[ ::nCurTheme, 1 ] )

   ENDCASE

   RETURN Self

/*----------------------------------------------------------------------*/

METHOD IdeThemes:setWrkTheme( cTheme )

   IF empty( cTheme )
      cTheme := ::selectTheme()
   ENDIF
   IF !empty( cTheme )
      ::oIde:cWrkTheme := cTheme
      ::oDK:setStatusText( SB_PNL_THEME, ::cWrkTheme )
   ENDIF

   RETURN Self

/*----------------------------------------------------------------------*/

METHOD IdeThemes:contains( cTheme )

   RETURN ascan( ::aThemes, {|a_| a_[ 1 ] == cTheme } ) > 0

/*----------------------------------------------------------------------*/

METHOD IdeThemes:load( cFile )

   IF hb_isChar( cFile ) .AND. !empty( cFile ) .AND. hb_FileExists( cFile )
      ::aIni:= hbide_readSource( cFile )
      ::parseINI()
      ::lDefault := .f.
   ENDIF

   RETURN Self

/*----------------------------------------------------------------------*/

METHOD IdeThemes:save( lAsk )
   LOCAL cFile, cINI

   DEFAULT lAsk TO .f.

   IF ::lDefault
      lAsk := .t.
   ENDIF

   IF lAsk
      cFile := hbide_saveAFile( ::oIde:oDlg, ;
                          "Select a File to Save Themes (.hbt)", ;
                          { { "Harbour IDE Themes", "*.hbt" } }, ;
                          ::cIniFile, ;
                          "hbt"  )
   ELSE
      cFile := ::cIniFile
   ENDIF

   IF !empty( cFile )
      cINI := ::buildINI()
      hb_memowrit( cFile, cINI )
      IF hb_FileExists( cFile )
         ::oIde:cIniThemes := cFile
         ::cIniFile := cFile
      ENDIF
   ENDIF

   RETURN Self

/*----------------------------------------------------------------------*/

METHOD IdeThemes:getThemeAttribute( cAttr, cTheme )
   LOCAL nTheme, aAttr

   IF !empty( cAttr )
      IF !empty( cTheme ) .and. hb_isChar( cTheme ) .and. ( nTheme := ascan( ::aThemes, {|e_| e_[ 1 ] == cTheme } ) ) > 0
         aAttr := GetKeyValue( ::aThemes[ nTheme, 2 ], cAttr )
      ENDIF
   ENDIF

   RETURN aAttr

/*----------------------------------------------------------------------*/

METHOD IdeThemes:buildSyntaxFormat( aAttr )
   LOCAL qFormat

   qFormat := QTextCharFormat():new()
   //
   qFormat:setFontItalic( aAttr[ THM_ATR_ITALIC ] )
   IF aAttr[ THM_ATR_BOLD ]
      qFormat:setFontWeight( 1000 )
   ENDIF
   qFormat:setFontUnderline( aAttr[ THM_ATR_ULINE ] )
   //
   qFormat:setForeground( QBrush():new( "QColor", QColor():new( aAttr[ THM_ATR_R ], aAttr[ THM_ATR_G ], aAttr[ THM_ATR_B ] ) ) )

   RETURN qFormat

/*----------------------------------------------------------------------*/

METHOD IdeThemes:setForeBackGround( qEdit, cTheme )
   LOCAL aAttr, s

   IF !empty( aAttr := ::getThemeAttribute( "Background", cTheme ) )
      s := 'QPlainTextEdit { background-color: rgba( ' + Attr2StrRGB( aAttr ) +", 255 ); "
      aAttr := ::getThemeAttribute( "UnrecognizedText", cTheme )
      s += ' color: rgba( ' +  Attr2StrRGB( aAttr ) + ", 255 ); }"
      qEdit:setStyleSheet( s )
   ENDIF

   RETURN Self

/*----------------------------------------------------------------------*/

METHOD IdeThemes:setQuotesRule( qHiliter, cTheme )
   LOCAL aAttr

   IF !empty( aAttr := ::getThemeAttribute( "TerminatedStrings", cTheme ) )
      qHiliter:hbSetFormat( "TerminatedStrings", ::buildSyntaxFormat( aAttr ) )
   ENDIF

   RETURN Self

/*----------------------------------------------------------------------*/

METHOD IdeThemes:setSingleLineCommentRule( qHiliter, cTheme )
   LOCAL aAttr

   IF !empty( aAttr := ::getThemeAttribute( "CommentsAndRemarks", cTheme ) )
      qHiliter:hbSetSingleLineCommentFormat( ::buildSyntaxFormat( aAttr ) )
   ENDIF

   RETURN Self

/*----------------------------------------------------------------------*/

METHOD IdeThemes:setMultiLineCommentRule( qHiliter, cTheme )
   LOCAL aAttr

   IF !empty( aAttr := ::getThemeAttribute( "CommentsAndRemarks", cTheme ) )
      qHiliter:hbSetMultiLineCommentFormat( ::buildSyntaxFormat( aAttr ) )
   ENDIF

   RETURN Self

/*----------------------------------------------------------------------*/

METHOD IdeThemes:setSyntaxRule( qHiliter, cName, cPattern, aAttr )

   qHiliter:hbSetRule( cName, cPattern, ::buildSyntaxFormat( aAttr ) )

   RETURN Self

/*----------------------------------------------------------------------*/

METHOD IdeThemes:setSyntaxFormat( qHiliter, cName, aAttr )

   qHiliter:hbSetFormat( cName, ::buildSyntaxFormat( aAttr ) )

   RETURN Self

/*----------------------------------------------------------------------*/
/*                         setSyntaxHilighting                          */
METHOD IdeThemes:setSyntaxHilighting( qEdit, cTheme, lNew )
   LOCAL a_, aAttr, qHiliter

   IF empty( cTheme )
      cTheme := ::cWrkTheme
   ENDIF
   IF empty( cTheme )
      cTheme := "Bare Minimum"   /* "Pritpal's Favourite" */
   ENDIF
   DEFAULT lNew TO .f.           /* Apply one which is already formed */

   HB_SYMBOL_UNUSED( lNew )

   ::setForeBackGround( qEdit, cTheme )

   qHiliter := HBQSyntaxHighlighter():new()

   FOR EACH a_ IN ::aPatterns
      IF !empty( aAttr := ::getThemeAttribute( a_[ 1 ], cTheme ) )
         ::setSyntaxRule( qHiliter, a_[ 1 ], a_[ 2 ], aAttr )
      ENDIF
   NEXT
   ::setMultiLineCommentRule( qHiliter, cTheme )
   ::setSingleLineCommentRule( qHiliter, cTheme )
   ::setQuotesRule( qHiliter, cTheme )

   IF __ObjGetClsName( qEdit ) == "HBQPLAINTEXTEDIT"
      aAttr := ::getThemeAttribute( "CurrentLineBackground", cTheme )
      qEdit:hbSetCurrentLineColor( QColor():new( aAttr[ THM_ATR_R ], aAttr[ THM_ATR_G ], aAttr[ THM_ATR_B ] ) )

      aAttr := ::getThemeAttribute( "LineNumbersBkColor", cTheme )
      qEdit:hbSetLineAreaBkColor( QColor():new( aAttr[ THM_ATR_R ], aAttr[ THM_ATR_G ], aAttr[ THM_ATR_B ] ) )
   ENDIF

   qHiliter:setDocument( qEdit:document() )

   RETURN qHiliter

/*----------------------------------------------------------------------*/

METHOD IdeThemes:show()

   IF empty( ::oUI )
      ::lCreating := .t.

      ::oUI := HbQtUI():new( hbide_uic( "themesex" ) ):build()
      ::oThemesDock:oWidget:setWidget( ::oUI )

      ::oUI:signal( "listThemes"   , "currentRowChanged(int)"   , {|i| ::execEvent( listThemes_currentRowChanged, i ) } )
      ::oUI:signal( "listItems"    , "currentRowChanged(int)"   , {|i| ::execEvent( listItems_currentRowChanged, i ) } )

      ::oUI:signal( "buttonColor"   , "clicked()"               , {|| ::updateColor() } )
      ::oUI:signal( "buttonSave"    , "clicked()"               , {|| ::save( .f. ) } )
      ::oUI:signal( "buttonSaveAs"  , "clicked()"               , {|| ::save( .t. ) } )
      ::oUI:signal( "buttonCopy"    , "clicked()"               , {|| ::copy( .t. ) } )
      ::oUI:signal( "buttonApply"   , "clicked()"               , {|| ::execEvent( applyMenu_triggered_applyToCurrentTab ) } )
      ::oUI:signal( "buttonApplyAll", "clicked()"               , {|| ::execEvent( applyMenu_triggered_applyToAllTabs    ) } )
      ::oUI:signal( "buttonDefault" , "clicked()"               , {|| ::execEvent( applyMenu_triggered_setAsDefault      ) } )

      ::oUI:signal( "checkItalic"   , "stateChanged(int)"       , {|i| ::updateAttribute( THM_ATR_ITALIC, i ) } )
      ::oUI:signal( "checkBold"     , "stateChanged(int)"       , {|i| ::updateAttribute( THM_ATR_BOLD  , i ) } )
      ::oUI:signal( "checkUnderline", "stateChanged(int)"       , {|i| ::updateAttribute( THM_ATR_ULINE , i ) } )

      ::oUI:signal( "buttonClose"   , "clicked()"               , {||  ::oThemesDock:hide() } )

      /* Fill Themes Dialog Values */
      ::oUI:setWindowTitle( GetKeyValue( ::aControls, "dialogTitle" ) )
      //
      ::oUI:qObj[ "labelItems"     ]:setText( GetKeyValue( ::aControls, "labelItems"    , "Items"     ) )
      ::oUI:qObj[ "labelTheme"     ]:setText( GetKeyValue( ::aControls, "labelTheme"    , "Theme"     ) )
      //
      ::oUI:qObj[ "checkItalic"    ]:setText( GetKeyValue( ::aControls, "checkItalic"   , "Italic"    ) )
      ::oUI:qObj[ "checkBold"      ]:setText( GetKeyValue( ::aControls, "checkBold"     , "Bold"      ) )
      ::oUI:qObj[ "checkUnderline" ]:setText( GetKeyValue( ::aControls, "checkUnderline", "Underline" ) )
      //
      ::oUI:qObj[ "buttonColor"    ]:setText( GetKeyValue( ::aControls, "buttonColor"   , "Color"     ) )
      ::oUI:qObj[ "buttonSave"     ]:setText( GetKeyValue( ::aControls, "buttonSave"    , "Save"      ) )
      ::oUI:qObj[ "buttonSaveAs"   ]:setText( GetKeyValue( ::aControls, "buttonSaveAs"  , "SaveAs"    ) )
      ::oUI:qObj[ "buttonClose"    ]:setText( GetKeyValue( ::aControls, "buttonClose"   , "Close"     ) )
      ::oUI:qObj[ "buttonCopy"     ]:setText( GetKeyValue( ::aControls, "buttonCopy"    , "Copy"      ) )

      aeval( ::aThemes, {|e_| ::oUI:q_listThemes:addItem( e_[ 1 ] ) } )
      aeval( ::aItems , {|e_| ::oUI:q_listItems:addItem( e_[ 2 ] ) } )

      ::qEdit := ::oUI:q_plainThemeText
      ::qEdit:setPlainText( GetSource() )
      ::qEdit:setLineWrapMode( QTextEdit_NoWrap )
      ::qEdit:setFont( ::oIde:oFont:oWidget )
      ::qEdit:ensureCursorVisible()

      ::lCreating := .f.

      ::oUI:q_listThemes:setCurrentRow( 0 )
      ::oUI:q_listItems:setCurrentRow( 0 )

      //::nCurTheme := 1
      //::nCurItem  := 1

      ::setTheme()
      ::setAttributes()
   ENDIF

   RETURN Self

/*----------------------------------------------------------------------*/

METHOD IdeThemes:copy()
   LOCAL aItems, qGo, cTheme

   qGo := QInputDialog():new( ::oUI )
   qGo:setTextValue( ::aThemes[ ::nCurTheme, 1 ] )
   qGo:setLabelText( "Name of new Theme?" )
   qGo:setWindowTitle( "Harbour-Qt [ Get a Value ]" )

   qGo:exec()

   cTheme := qGo:textValue()

   IF !empty( cTheme ) .and. ( cTheme != ::aThemes[ ::nCurTheme, 1 ] )
      aItems := aclone( ::aThemes[ ::nCurTheme ] )
      aItems[ 1 ] := cTheme
      aadd( ::aThemes, aItems )
      ::oUI:qObj[ "listThemes" ]:addItem( cTheme )
      ::oUI:qObj[ "listThemes" ]:setCurrentRow( len( ::aThemes ) - 1 )
   ENDIF

   RETURN Self

/*----------------------------------------------------------------------*/

METHOD IdeThemes:setTheme()

   IF !::lCreating
      ::qHiliter := ::setSyntaxHilighting( ::qEdit, ::aThemes[ ::nCurTheme, 1 ], .t. )
      ::setAttributes()
   ENDIF

   RETURN Self

/*----------------------------------------------------------------------*/

METHOD IdeThemes:setAttributes()
   LOCAL aAttr

   IF !::lCreating
      aAttr := ::aThemes[ ::nCurTheme, 2, ::nCurItem, 2 ]
      //
      ::oUI:qObj[ "checkItalic"    ]:setChecked( aAttr[ THM_ATR_ITALIC ] )
      ::oUI:qObj[ "checkBold"      ]:setChecked( aAttr[ THM_ATR_BOLD   ] )
      ::oUI:qObj[ "checkUnderline" ]:setChecked( aAttr[ THM_ATR_ULINE  ] )

      ::oUI:qObj[ "buttonColor" ]:setStyleSheet( "color: " + Attr2RGBfnRev( aAttr ) + ";" + ;
                                                 "background-color: " + Attr2RGBfn( aAttr ) + ";" )
   ENDIF

   RETURN Self

/*----------------------------------------------------------------------*/

METHOD IdeThemes:updateLineNumbersBkColor()
   LOCAL oEdit, aAttr

   IF !empty( oEdit := ::oEM:getEditObjectCurrent() )
      aAttr := ::getThemeAttribute( "LineNumbersBkColor", ::aThemes[ ::nCurTheme, 1 ] )
      oEdit:setLineNumbersBkColor( aAttr[ THM_ATR_R ], aAttr[ THM_ATR_G ], aAttr[ THM_ATR_B ] )
      oEdit:refresh()
      ::setAttributes()
   ENDIF

   RETURN Self

/*----------------------------------------------------------------------*/

METHOD IdeThemes:updateCurrentLineColor()
   LOCAL oEdit, aAttr

   IF !empty( oEdit := ::oEM:getEditObjectCurrent() )
      aAttr := ::getThemeAttribute( "CurrentLineBackground", ::aThemes[ ::nCurTheme, 1 ] )
      oEdit:setCurrentLineColor( aAttr[ THM_ATR_R ], aAttr[ THM_ATR_G ], aAttr[ THM_ATR_B ] )
      oEdit:refresh()
      ::setAttributes()
   ENDIF

   RETURN Self

/*----------------------------------------------------------------------*/

METHOD IdeThemes:updateColor()
   LOCAL aAttr, oDlg, qColor, s, aF, aB

   aAttr := ::aThemes[ ::nCurTheme, 2, ::nCurItem ]

   qColor := QColor():new( aAttr[ 2, THM_ATR_R ], aAttr[ 2, THM_ATR_G ], aAttr[ 2, THM_ATR_B ] )

   oDlg := QColorDialog():new( ::oUI:oWidget )
   oDlg:setWindowTitle( "Select a Color" )
   oDlg:setCurrentColor( qColor )
   oDlg:exec()

   qColor:configure( oDlg:currentColor() )

   ::aThemes[ ::nCurTheme, 2, ::nCurItem, 2, THM_ATR_R ] := qColor:red()
   ::aThemes[ ::nCurTheme, 2, ::nCurItem, 2, THM_ATR_G ] := qColor:green()
   ::aThemes[ ::nCurTheme, 2, ::nCurItem, 2, THM_ATR_B ] := qColor:blue()

   IF aAttr[ 1 ] $ "Background,UnrecognizedText"
      aF := GetKeyValue( ::aThemes[ ::nCurTheme, 2 ], "UnrecognizedText" )
      aB := GetKeyValue( ::aThemes[ ::nCurTheme, 2 ], "Background"       )
      //
      s := "QPlainTextEdit { "
      s += "           color: rgba( " + Attr2StrRGB( aF ) + ", 255 );  "
      s += "background-color: rgba( " + Attr2StrRGB( aB ) + ", 255 ); }"
      //
      ::qEdit:setStyleSheet( s )

   ELSEIF aAttr[ 1 ] == "CommentsAndRemarks"
      ::setMultiLineCommentRule( ::qHiliter, ::aThemes[ ::nCurTheme, 1 ] )
      ::setSyntaxFormat( ::qHiliter, aAttr[ 1 ], aAttr[ 2 ] )

   ELSEIF aAttr[ 1 ] == "CurrentLineBackground"
      ::updateCurrentLineColor()
      RETURN Self

   ELSEIF aAttr[ 1 ] == "LineNumbersBkColor"
      ::updateLineNumbersBkColor()
      RETURN Self

   ELSE
      ::setSyntaxFormat( ::qHiliter, aAttr[ 1 ], aAttr[ 2 ] )

   ENDIF

   ::qHiliter:rehighlight()
   ::setAttributes()
   RETURN Self

/*----------------------------------------------------------------------*/

METHOD IdeThemes:updateAttribute( nAttr, iState )
   LOCAL aAttr

   aAttr := ::aThemes[ ::nCurTheme, 2, ::nCurItem ]

   ::aThemes[ ::nCurTheme, 2, ::nCurItem, 2, nAttr ] := ( iState == 2 )

   ::setSyntaxFormat( ::qHiliter, aAttr[ 1 ], aAttr[ 2 ] )
   ::qHiliter:rehighlight()

   RETURN Self

/*----------------------------------------------------------------------*/

METHOD IdeThemes:selectTheme()
   LOCAL cTheme := ""
   LOCAL oSL, oStrList, oStrModel, a_, nDone
   LOCAL pSlots := Qt_Slots_New()

   oSL := HbQtUI():new( ::oIde:resPath + "selectionlist.uic", ::oIde:oDlg:oWidget ):build()

   oSL:setWindowTitle( "Available Themes" )

   oStrList := QStringList():new()
   FOR EACH a_ IN ::aThemes
      oStrList:append( a_[ 1 ] )
   NEXT

   oStrModel := QStringListModel():new()
   oStrModel:setStringList( oStrList )

   oSL:qObj[ "listOptions" ]:setModel( oStrModel )

   Qt_Slots_Connect( pSlots, oSL:qObj[ "listOptions"  ], "doubleClicked(QModelIndex)", {|p| ::selectThemeProc( 1, p, oSL, @cTheme ) } )
   Qt_Slots_Connect( pSlots, oSL:qObj[ "buttonOk"     ], "clicked()"                 , {|p| ::selectThemeProc( 2, p, oSL, @cTheme ) } )
   Qt_Slots_Connect( pSlots, oSL:qObj[ "buttonCancel" ], "clicked()"                 , {|p| ::selectThemeProc( 3, p, oSL          ) } )

   nDone := oSL:exec()

   Qt_Slots_disConnect( pSlots, oSL:qObj[ "listOptions"  ], "doubleClicked(QModelIndex)" )
   Qt_Slots_disConnect( pSlots, oSL:qObj[ "buttonOk"     ], "clicked()" )
   Qt_Slots_disConnect( pSlots, oSL:qObj[ "buttonCancel" ], "clicked()" )

   oSL:destroy()

   RETURN iif( nDone == 1, cTheme, "" )

/*----------------------------------------------------------------------*/

METHOD IdeThemes:selectThemeProc( nMode, p, oSL, cTheme )
   LOCAL qModalIndex

   DO CASE
   CASE nMode == 1
      qModalIndex := QModelIndex():configure( p )
      cTheme := ::aThemes[ qModalIndex:row() + 1, 1 ]
      oSL:done( 1 )

   CASE nMode == 2
      qModalIndex := QModelIndex():configure( oSL:qObj[ "listOptions" ]:currentIndex() )
      cTheme := ::aThemes[ qModalIndex:row() + 1, 1 ]
      oSL:done( 1 )

   CASE nMode == 3
      oSL:done( 0 )

   ENDCASE

   RETURN Nil

/*----------------------------------------------------------------------*/

METHOD IdeThemes:buildINI()
   LOCAL a_, b_
   LOCAL txt_ := {}
   LOCAL cINI := ""

   aadd( txt_, "#  " )
   aadd( txt_, "#  Harbour IDE Editor Themes" )
   aadd( txt_, "#  Version 0.7" )
   aadd( txt_, "#  Generated on " + dtoc( date() ) + "  " + time() )
   aadd( txt_, "#  " )
   aadd( txt_, "   " )
   aadd( txt_, "[ Controls ]" )
   aadd( txt_, "   " )
   FOR EACH a_ IN ::aControls
      aadd( txt_, pad( a_[ 1 ], 30 ) + " = " + a_[ 2 ] )
   NEXT
   aadd( txt_, "   " )
   aadd( txt_, "   " )
   aadd( txt_, "[ Items ]" )
   aadd( txt_, "   " )
   FOR EACH a_ IN ::aItems
      aadd( txt_, pad( a_[ 1 ], 30 ) + " = " + a_[ 2 ] )
   NEXT
   FOR EACH a_ IN ::aThemes
      aadd( txt_, "   " )
      aadd( txt_, "   " )
      aadd( txt_, "[ Theme : " + a_[ 1 ] + " ]" )
      aadd( txt_, "   " )
      FOR EACH b_ IN a_[ 2 ]
          aadd( txt_, pad( b_[ 1 ], 30 ) + " = " + Attr2Str( b_[ 2 ] ) )
      NEXT
   NEXT
   aadd( txt_, "   " )

   aeval( txt_, {|e| cINI += e + CRLF } )

   RETURN cINI

/*----------------------------------------------------------------------*/

METHOD IdeThemes:parseINI( lAppend )
   LOCAL s, n, cKey, cVal, nPart, nTheme, aVal, aV

   IF empty( ::aIni )
      RETURN Self
   ENDIF

   DEFAULT lAppend TO .t.

   IF !( lAppend )
      ::aControls := {}
      ::aThemes   := {}
      ::aItems    := {}
   ENDIF

   FOR EACH s IN ::aIni
      IF !empty( s := alltrim( s ) ) .and. !left( s, 1 ) == "#" /* Comment */
         DO case
         CASE s == "[ Controls ]"
            nPart := 1
         CASE s == "[ Items ]"
            nPart := 2
         CASE left( s, 7 ) == "[ Theme"
            IF ( n := at( ":", s ) ) > 0
               cKey := alltrim( strtran( substr( s, n+1 ), "]" ) )
            ENDIF
            IF !empty( cKey )
               nPart := 3
               IF ( nTheme := ascan( ::aThemes, {|e_| e_[ 1 ] == cKey } ) ) == 0
                  aadd( ::aThemes, { cKey, {} } )
                  nTheme := len( ::aThemes )
               ENDIF
            ELSE
               nPart := 0
            ENDIF
         OTHERWISE
            DO CASE
            CASE nPart == 1 /* Controls */
               IF hbide_parseKeyValPair( s, @cKey, @cVal )
                  IF ( n := ascan( ::aControls, cKey ) ) > 0
                     ::aControls[ n, 2 ] := cVal
                  ELSE
                     aadd( ::aControls, { cKey, cVal } )
                  ENDIF
               ENDIF
            CASE nPart == 2 /* Items   */
               IF hbide_parseKeyValPair( s, @cKey, @cVal )
                  IF ( n := ascan( ::aThemes, cKey ) ) > 0
                     ::aThemes[ n, 2 ] := cVal
                  ELSE
                     aadd( ::aItems, { cKey, cVal } )
                  ENDIF
               ENDIF
            CASE nPart == 3 /* Theams  */
               IF hbide_parseKeyValPair( s, @cKey, @cVal )
                  aV   := FillAttrbs()
                  aVal := hb_aTokens( cVal, "," )
                  FOR n := 1 TO THM_NUM_ATTRBS
                     s := alltrim( aVal[ n ] )
                     IF n <= 3
                        aV[ n ] := val( s )
                     ELSE
                        aV[ n ] := lower( s ) == "yes"
                     ENDIF
                  NEXT
                  IF ( n := ascan( ::aThemes[ nTheme, 2 ], {|e_| e_[ 1 ] == cKey } ) ) > 0
                     ::aThemes[ nTheme, 2, n, 2 ] := aV
                  ELSE
                     aadd( ::aThemes[ nTheme, 2 ], { cKey, aV } )
                  ENDIF
               ENDIF
            ENDCASE
         ENDCASE
      ENDIF
   NEXT

   RETURN Self

/*----------------------------------------------------------------------*/

STATIC FUNCTION FillAttrbs()
   RETURN { 0, 0, 0, .f., .f., .f. }

/*----------------------------------------------------------------------*/

STATIC FUNCTION Attr2RGBfn( a_ )
   RETURN "rgba( " + Attr2StrRGB( a_ ) + ", 255 )"

/*----------------------------------------------------------------------*/

STATIC FUNCTION Attr2RGBfnRev( a_ )
   LOCAL b_:= { ( a_[ THM_ATR_R ] + 255 ) % 256, ( a_[ THM_ATR_G ] + 255 ) % 256, ( a_[ THM_ATR_B ] + 255 ) % 256 }
   RETURN "rgba( " + Attr2StrRGB( b_ ) + ", 255 )"

/*----------------------------------------------------------------------*/

STATIC FUNCTION Attr2StrRGB( a_ )
   RETURN hb_ntos( a_[ THM_ATR_R ] ) +","+ hb_ntos( a_[ THM_ATR_G ] ) +","+ hb_ntos( a_[ THM_ATR_B ] )

/*----------------------------------------------------------------------*/

STATIC FUNCTION Attr2Str( a_ )

   RETURN padl( hb_ntos( a_[ 1 ] ), 4 ) + "," +;
          padl( hb_ntos( a_[ 2 ] ), 4 ) + "," +;
          padl( hb_ntos( a_[ 3 ] ), 4 ) + "," +;
          IF( a_[ 4 ], " Yes", "  No" ) + "," +;
          IF( a_[ 5 ], " Yes", "  No" ) + "," +;
          IF( a_[ 6 ], " Yes", "  No" ) + ","

/*----------------------------------------------------------------------*/

STATIC FUNCTION GetKeyValue( aKeys, cKey, cDef )
   LOCAL xVal, n

   DEFAULT cDef TO ""

   IF ( n := ascan( aKeys, {|e_| e_[ 1 ] == cKey } ) ) > 0
      xVal := aKeys[ n, 2 ]
   ELSE
      xVal := cDef
   ENDIF

   RETURN xVal

/*----------------------------------------------------------------------*/

STATIC FUNCTION GetSource()
   LOCAL s := ""
   LOCAL txt_:= {}

   aadd( txt_, '/* Copyright 2009-2010 Pritpal Bedi <pritpal@vouchcac.com>                 ' )
   aadd( txt_, ' *                                                                         ' )
   aadd( txt_, ' * This program is free software; you can redistribute it and/or modify    ' )
   aadd( txt_, '*/                                                                         ' )
   aadd( txt_, '#include "hbide.ch"                                                        ' )
   aadd( txt_, '                                                                           ' )
   aadd( txt_, 'CLASS IdeThemes                                                            ' )
   aadd( txt_, '   VAR    oIde                                                             ' )
   aadd( txt_, '   METHOD new()                                                            ' )
   aadd( txt_, '   ENDCLASS                                                                ' )
   aadd( txt_, '/*----------------------------------------------------------------------*/ ' )
   aadd( txt_, 'METHOD IdeThemes:new( oIde, cIniFile )                                     ' )
   aadd( txt_, '                                                                           ' )
   aadd( txt_, '   ::oIde  := oIde                                                         ' )
   aadd( txt_, '   ::cIniFile := cIniFile                                                  ' )
   aadd( txt_, '                                                                           ' )
   aadd( txt_, '   RETURN Self                                                             ' )
   aadd( txt_, '/*----------------------------------------------------------------------*/ ' )
   aadd( txt_, '// This function is used to retrieve value given a key ...                 ' )
   aadd( txt_, 'STATIC FUNCTION GetKeyValue( aKeys, cKey, cDef )                           ' )
   aadd( txt_, '   LOCAL xVal                                                              ' )
   aadd( txt_, '                                                                           ' )
   aadd( txt_, '   DEFAULT cDef TO ""                                                      ' )
   aadd( txt_, '                                                                           ' )
   aadd( txt_, '   IF ( n := ascan( aKeys, {|e_| e_[ 1 ] == cKey } ) ) > 0                 ' )
   aadd( txt_, '      xVal := aKeys[ n, 2 ]                                                ' )
   aadd( txt_, '   ELSE                                                                    ' )
   aadd( txt_, '      xVal := cDef                                                         ' )
   aadd( txt_, '   ENDIF                                                                   ' )
   aadd( txt_, '   RETURN xVal                                                             ' )
   aadd( txt_, '/*----------------------------------------------------------------------*/ ' )

   aeval( txt_, {|e| s += trim( e ) + CRLF } )

   RETURN s

/*----------------------------------------------------------------------*/

STATIC FUNCTION hbide_setSyntaxAttrbs( qHiliter, cPattern, cName, nR, nG, nB, lItalic, lBold, lUnderline )
   LOCAL qFormat

   qFormat  := QTextCharFormat():new()

   IF hb_isLogical( lItalic )
      qFormat:setFontItalic( lItalic )
   ENDIF
   IF hb_isLogical( lBold ) .and. lBold
      qFormat:setFontWeight( 1000 )
   ENDIF
   IF hb_isLogical( lUnderline )
      qFormat:setFontUnderline( lUnderline )
   ENDIF
   qFormat:setForeGround( QBrush():new( "QColor", QColor():new( nR, nG, nB ) ) )

   qHiliter:hbSetRule( cName, cPattern, qFormat )

   RETURN nil

/*----------------------------------------------------------------------*/

STATIC FUNCTION hbide_loadDefaultThemes()
   LOCAL aIni := {}

   IF .t.
      aadd( aIni, "[ Controls ]                                                         " )
      aadd( aIni, "                                                                     " )
      aadd( aIni, "dialogTitle                    = HBIDE - Source Syntax Highlighting  " )
      aadd( aIni, "labelItem                      = Item                                " )
      aadd( aIni, "labelTheme                     = Theme                               " )
      aadd( aIni, "checkItalic                    = Italic                              " )
      aadd( aIni, "checkBold                      = Bold                                " )
      aadd( aIni, "checkUnderline                 = Underline                           " )
      aadd( aIni, "buttonColor                    = Color                               " )
      aadd( aIni, "buttonSave                     = Save                                " )
      aadd( aIni, "buttonSaveAs                   = Save As                             " )
      aadd( aIni, "buttonApply                    = Apply                               " )
      aadd( aIni, "buttonCancel                   = Cancel                              " )
      aadd( aIni, "buttonCopy                     = Copy                                " )
      aadd( aIni, "                                                                     " )
      aadd( aIni, "[ Items ]                                                            " )
      aadd( aIni, "                                                                     " )
      aadd( aIni, "Background                     = Background                          " )
      aadd( aIni, "PreprocessorDirectives         = Preprocessor Directives             " )
      aadd( aIni, "HarbourKeywords                = Harbour Keywords                    " )
      aadd( aIni, "CLanguageKeywords              = C-CPP Language Keywords             " )
      aadd( aIni, "Operators                      = Operators                           " )
      aadd( aIni, "NumericalConstants             = Numerical Constants                 " )
      aadd( aIni, "BracketsAndBraces              = Brackets and Braces                 " )
      aadd( aIni, "FunctionsBody                  = Functions Body                      " )
      aadd( aIni, "TerminatedStrings              = Terminated Strings                  " )
      aadd( aIni, "CommentsAndRemarks             = Comments and Remarks                " )
      aadd( aIni, "UnrecognizedText               = Unrecognized Text                   " )
      aadd( aIni, "BookMarkLineBackground         = BookMark Line Background            " )
      aadd( aIni, "SelectionBackground            = Selection Background                " )
      aadd( aIni, "CurrentLineBackground          = Current Line Background             " )
      aadd( aIni, "UnterminatedStrings            = Unterminated Strings                " )
      aadd( aIni, "LineNumbersBkColor             = Line Numbers Background             " )
      aadd( aIni, "UserDictionary                 = UserDictionary                      " )
      aadd( aIni, "                                                                     " )
      aadd( aIni, "                                                                     " )
      aadd( aIni, "[ Theme : Pritpal's Favourite ]                                      " )
      aadd( aIni, "                                                                     " )
      aadd( aIni, "Background                     =  245, 255, 216,  No,  No,  No,      " )
      aadd( aIni, "PreprocessorDirectives         =   69, 138,   0,  No, Yes,  No,      " )
      aadd( aIni, "HarbourKeywords                =   54,   0, 162,  No, Yes,  No,      " )
      aadd( aIni, "CLanguageKeywords              =    0,   0, 128,  No,  No,  No,      " )
      aadd( aIni, "Operators                      =  172,  39, 255,  No, Yes,  No,      " )
      aadd( aIni, "NumericalConstants             =    0, 128,   0,  No,  No,  No,      " )
      aadd( aIni, "BracketsAndBraces              =  255,  85,   0,  No,  No,  No,      " )
      aadd( aIni, "FunctionsBody                  =   15, 122, 255,  No, Yes,  No,      " )
      aadd( aIni, "TerminatedStrings              =  255,   0,   0,  No,  No,  No,      " )
      aadd( aIni, "CommentsAndRemarks             =  165, 165, 165,  No,  No,  No,      " )
      aadd( aIni, "UnrecognizedText               =    0,   0,   0,  No,  No,  No,      " )
      aadd( aIni, "BookMarkLineBackground         =    0, 255, 255,  No,  No,  No,      " )
      aadd( aIni, "SelectionBackground            =  255, 128, 255,  No,  No,  No,      " )
      aadd( aIni, "CurrentLineBackground          =  255, 215, 155,  No,  No,  No,      " )
      aadd( aIni, "UnterminatedStrings            =  255, 128, 128,  No,  No,  No,      " )
      aadd( aIni, "LineNumbersBkColor             =  255, 215, 155,  No,  No,  No,      " )
      aadd( aIni, "UserDictionary                 =    0,   0,   0,  No,  No,  No,      " )
      aadd( aIni, "                                                                     " )
      aadd( aIni, "                                                                     " )
      aadd( aIni, "[ Theme : Bare Minimum ]                                             " )
      aadd( aIni, "                                                                     " )
      aadd( aIni, "Background                     =  255, 255, 255,  No,  No,  No,      " )
      aadd( aIni, "PreprocessorDirectives         =  127,   0, 127,  No,  No,  No,      " )
      aadd( aIni, "HarbourKeywords                =    0, 127, 127,  No, Yes,  No,      " )
      aadd( aIni, "CLanguageKeywords              =    0,   0, 128,  No,  No,  No,      " )
      aadd( aIni, "Operators                      =    0,   0,   0,  No,  No,  No,      " )
      aadd( aIni, "NumericalConstants             =    0,   0,   0,  No,  No,  No,      " )
      aadd( aIni, "BracketsAndBraces              =    0,   0,   0,  No,  No,  No,      " )
      aadd( aIni, "FunctionsBody                  =    0,   0, 255,  No,  No,  No,      " )
      aadd( aIni, "TerminatedStrings              =  255,   0,   0,  No,  No,  No,      " )
      aadd( aIni, "CommentsAndRemarks             =  165, 165, 165,  No,  No,  No,      " )
      aadd( aIni, "UnrecognizedText               =    0,   0,   0,  No,  No,  No,      " )
      aadd( aIni, "BookMarkLineBackground         =    0, 255, 255,  No,  No,  No,      " )
      aadd( aIni, "SelectionBackground            =  255, 128, 255,  No,  No,  No,      " )
      aadd( aIni, "CurrentLineBackground          =  235, 235, 235,  No,  No,  No,      " )
      aadd( aIni, "UnterminatedStrings            =  255, 128, 128,  No,  No,  No,      " )
      aadd( aIni, "LineNumbersBkColor             =  235, 235, 235,  No,  No,  No,      " )
      aadd( aIni, "UserDictionary                 =    0,   0,   0,  No,  No,  No,      " )
      aadd( aIni, "                                                                     " )
      aadd( aIni, "                                                                     " )
      aadd( aIni, "[ Theme : Classic ]                                                  " )
      aadd( aIni, "                                                                     " )
      aadd( aIni, "Background                     = 255,255,255    ,  No,  No,  No,     " )
      aadd( aIni, "PreprocessorDirectives         = 128,128,0      ,  No,  No,  No,     " )
      aadd( aIni, "HarbourKeywords                = 128,0,128      ,  No,  No,  No,     " )
      aadd( aIni, "CLanguageKeywords              = 0,0,128        ,  No,  No,  No,     " )
      aadd( aIni, "Operators                      = 0,0,0          ,  No,  No,  No,     " )
      aadd( aIni, "NumericalConstants             = 0,128,0        ,  No,  No,  No,     " )
      aadd( aIni, "BracketsAndBraces              = 64,0,0         ,  No,  No,  No,     " )
      aadd( aIni, "FunctionsBody                  = 0,0,192        ,  No,  No,  No,     " )
      aadd( aIni, "TerminatedStrings              = 255,0,0        ,  No,  No,  No,     " )
      aadd( aIni, "CommentsAndRemarks             = 0,128,255      ,  No,  No,  No,     " )
      aadd( aIni, "UnrecognizedText               = 0,0,0          ,  No,  No,  No,     " )
      aadd( aIni, "BookMarkLineBackground         = 0,255,255      ,  No,  No,  No,     " )
      aadd( aIni, "SelectionBackground            = 255,128,255    ,  No,  No,  No,     " )
      aadd( aIni, "CurrentLineBackground          = 220,220,220    ,  No,  No,  No,     " )
      aadd( aIni, "UnterminatedStrings            = 255,128,128    ,  No,  No,  No,     " )
      aadd( aIni, "LineNumbersBkColor             = 220,220,220    ,  No,  No,  No,     " )
      aadd( aIni, "UserDictionary                 = 0,0,0          ,  No,  No,  No,     " )
      aadd( aIni, "                                                                     " )
      aadd( aIni, "                                                                     " )
      aadd( aIni, "[ Theme : City Lights ]                                              " )
      aadd( aIni, "                                                                     " )
      aadd( aIni, "Background                     = 0,0,0          ,  No,  No,  No,     " )
      aadd( aIni, "PreprocessorDirectives         = 255,0,0        ,  No,  No,  No,     " )
      aadd( aIni, "HarbourKeywords                = 128,0,128      ,  No,  No,  No,     " )
      aadd( aIni, "CLanguageKeywords              = 0,0,128        ,  No,  No,  No,     " )
      aadd( aIni, "Operators                      = 128,255,0      ,  No,  No,  No,     " )
      aadd( aIni, "NumericalConstants             = 0,255,255      ,  No,  No,  No,     " )
      aadd( aIni, "BracketsAndBraces              = 255,128,128    ,  No,  No,  No,     " )
      aadd( aIni, "FunctionsBody                  = 128,128,255    ,  No,  No,  No,     " )
      aadd( aIni, "TerminatedStrings              = 0,255,0        ,  No,  No,  No,     " )
      aadd( aIni, "CommentsAndRemarks             = 255,255,0      ,  No,  No,  No,     " )
      aadd( aIni, "UnrecognizedText               = 255,255,255    ,  No,  No,  No,     " )
      aadd( aIni, "BookMarkLineBackground         = 128,128,128    ,  No,  No,  No,     " )
      aadd( aIni, "SelectionBackground            = 255,128,255    ,  No,  No,  No,     " )
      aadd( aIni, "CurrentLineBackground          = 0,0,255        ,  No,  No,  No,     " )
      aadd( aIni, "UnterminatedStrings            = 255,255,255    ,  No,  No,  No,     " )
      aadd( aIni, "LineNumbersBkColor             = 0,0,255        ,  No,  No,  No,     " )
      aadd( aIni, "UserDictionary                 = 0,0,0          ,  No,  No,  No,     " )
      aadd( aIni, "                                                                     " )
      aadd( aIni, "                                                                     " )
      aadd( aIni, "[ Theme : Evening Glamour ]                                          " )
      aadd( aIni, "                                                                     " )
      aadd( aIni, "Background                     = 0,64,128       ,  No,  No,  No,     " )
      aadd( aIni, "PreprocessorDirectives         = 255,128,192    ,  No,  No,  No,     " )
      aadd( aIni, "HarbourKeywords                = 255,128,192    ,  No,  No,  No,     " )
      aadd( aIni, "CLanguageKeywords              = 0,0,128        ,  No,  No,  No,     " )
      aadd( aIni, "Operators                      = 255,255,255    ,  No,  No,  No,     " )
      aadd( aIni, "NumericalConstants             = 0,255,0        ,  No,  No,  No,     " )
      aadd( aIni, "BracketsAndBraces              = 128,255,255    ,  No,  No,  No,     " )
      aadd( aIni, "FunctionsBody                  = 128,255,128    ,  No,  No,  No,     " )
      aadd( aIni, "TerminatedStrings              = 255,255,128    ,  No,  No,  No,     " )
      aadd( aIni, "CommentsAndRemarks             = 192,192,192    ,  No,  No,  No,     " )
      aadd( aIni, "UnrecognizedText               = 255,255,255    ,  No,  No,  No,     " )
      aadd( aIni, "BookMarkLineBackground         = 128,0,255      ,  No,  No,  No,     " )
      aadd( aIni, "SelectionBackground            = 0,128,255      ,  No,  No,  No,     " )
      aadd( aIni, "CurrentLineBackground          = 90,180,180     ,  No,  No,  No,     " )
      aadd( aIni, "UnterminatedStrings            = 255,128,64     ,  No,  No,  No,     " )
      aadd( aIni, "LineNumbersBkColor             = 90,180,180     ,  No,  No,  No,     " )
      aadd( aIni, "UserDictionary                 = 0,0,0          ,  No,  No,  No,     " )
      aadd( aIni, "                                                                     " )
      aadd( aIni, "                                                                     " )
      aadd( aIni, "[ Theme : Sand Storm ]                                               " )
      aadd( aIni, "                                                                     " )
      aadd( aIni, "Background                     = 255,255,192    ,  No,  No,  No,     " )
      aadd( aIni, "PreprocessorDirectives         = 255,0,0        ,  No,  No,  No,     " )
      aadd( aIni, "HarbourKeywords                = 128,0,128      ,  No,  No,  No,     " )
      aadd( aIni, "CLanguageKeywords              = 0,0,128        ,  No,  No,  No,     " )
      aadd( aIni, "Operators                      = 0,0,0          ,  No,  No,  No,     " )
      aadd( aIni, "NumericalConstants             = 0,128,128      ,  No,  No,  No,     " )
      aadd( aIni, "BracketsAndBraces              = 0,0,0          ,  No,  No,  No,     " )
      aadd( aIni, "FunctionsBody                  = 0,0,192        ,  No,  No,  No,     " )
      aadd( aIni, "TerminatedStrings              = 0,128,0        ,  No,  No,  No,     " )
      aadd( aIni, "CommentsAndRemarks             = 128,128,128    ,  No,  No,  No,     " )
      aadd( aIni, "UnrecognizedText               = 0,0,0          ,  No,  No,  No,     " )
      aadd( aIni, "BookMarkLineBackground         = 0,255,255      ,  No,  No,  No,     " )
      aadd( aIni, "SelectionBackground            = 255,0,255      ,  No,  No,  No,     " )
      aadd( aIni, "CurrentLineBackground          = 220,220,110    ,  No,  No,  No,     " )
      aadd( aIni, "UnterminatedStrings            = 128,128,0      ,  No,  No,  No,     " )
      aadd( aIni, "LineNumbersBkColor             = 220,220,110    ,  No,  No,  No,     " )
      aadd( aIni, "UserDictionary                 = 0,0,0          ,  No,  No,  No,     " )
      aadd( aIni, "                                                                     " )
   ENDIF

   RETURN aIni

/*----------------------------------------------------------------------*/
#if 0
                                                       [Classic]      [CityLights]  [Evening]     [SandStorm]

Background                 = Background                255,255,255   0,0,0         0,64,128      255,255,192
PreprocessorDirectives     = Preprocessor Directives   128,128,0     255,0,0       255,128,192   255,0,0
HarbourKeywords            = Harbour Keywords          128,0,128     128,0,128     255,128,192   128,0,128
CLanguageKeywords          = C-CPP Language Keywords   0,0,128       0,0,128       0,0,128       0,0,128
Operators                  = Operators                 0,0,0         128,255,0     255,255,255   0,0,0
NumericalConstants         = Numerical Constants       0,128,0       0,255,255     0,255,0       0,128,128
BracketsAndBraces          = Brackets and Braces       64,0,0        255,128,128   128,255,255   0,0,0
FunctionsBody              = Functions Body            0,0,192       128,128,255   128,255,128   0,0,192
TerminatedStrings          = Terminated Strings        255,0,0       0,255,0       255,255,128   0,128,0
CommentsAndRemarks         = Comments and Remarks      0,128,255     255,255,0     192,192,192   128,128,128
BookMarkLineBackground     = BookMark Line Background  0,255,255     128,128,128   128,0,255     0,255,255
SelectionBackground        = Selection Background      255,128,255   255,128,255   0,128,255     255,0,255
CurrentLineBackground      = Current Line Background   128,0,0       0,0,255       128,255,255   128,0,0
UnrecognizedText           = Unrecognized Text         0,0,0         255,255,255   255,255,255   0,0,0
UnterminatedStrings        = Unterminated Strings      255,128,128   255,255,255   255,128,64    128,128,0
LineNumbersBkColor         = WAPIDictionary            0,0,128       0,0,128       128,128,64    0,0,128
UserDictionary             = UserDictionary            0,0,0         0,0,0         0,0,0         0,0,0

#endif

/*----------------------------------------------------------------------*/
