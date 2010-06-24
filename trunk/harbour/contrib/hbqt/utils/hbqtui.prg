/*
 * $Id$
 */

/*
 * Harbour Project source code:
 * Source file for the Xbp*Classes
 *
 * Copyright 2010 Pritpal Bedi <bedipritpal@hotmail.com>
 * http://harbour-project.org
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
 *                               EkOnkar
 *                         ( The LORD is ONE )
 *
 *                     Harbour Utility .ui => .prg
 *
 *                Pritpal Bedi <bedipritpal@hotmail.com>
 *                              22Jun2010
 */
/*----------------------------------------------------------------------*/
/*----------------------------------------------------------------------*/
/*----------------------------------------------------------------------*/

#include "hbclass.ch"
#include "common.ch"
#include "hbqt.ch"

/*----------------------------------------------------------------------*/

#define STRINGIFY( cStr )    '"' + cStr + '"'
#define PAD_30( cStr )       pad( cStr, max( len( cStr ), 20 ) )
#define STRIP_SQ( cStr )     strtran( strtran( strtran( strtran( s, "[", " " ), "]", " " ), "\n", " " ), chr( 10 ), " " )

/*----------------------------------------------------------------------*/

ANNOUNCE HB_GTSYS
REQUEST HB_GT_CGI_DEFAULT

PROCEDURE Main( ... )
   LOCAL s, cL, cExt, cPath, cFile
   LOCAL oGen, cCmd, cUic, cPrg, cUiFile
   LOCAL cPathOut := ""
   LOCAL aUI :={}, a_, aUiFiles := {}
   LOCAL lToPath := .f.
   LOCAL lDelUic := .t.

   FOR EACH s IN hb_aParams()
      cL := lower( alltrim( s ) )

      DO CASE
      CASE left( cL, 1 ) == "@"
         aadd( aUiFiles, substr( s, 2 ) )

      CASE left( cL, 2 ) == "-o"
         cPathOut := alltrim( substr( s, 3 ) )
         cPathOut := strtran( cPathOut, "\", "/" )
         lToPath  := right( cPathOut, 1 ) == "/"

      CASE cL == "-nodeluic"
         lDelUic := .f.

      OTHERWISE
         s := StrTran( s, "\", "/" )
         hb_fNameSplit( s, , , @cExt )
         IF lower( cExt ) == ".ui"
            aadd( aUI, s )
         ENDIF

      ENDCASE
   NEXT

   FOR EACH cUiFile IN aUiFiles
      a_:= hb_ATokens( StrTran( hb_MemoRead( cUiFile ), Chr( 13 ) ), Chr( 10 ) )
      FOR EACH s IN a_
         s := alltrim( s )
         IF ! Empty( s )
            IF left( s, 1 ) $ "#;"
               LOOP
            ENDIF
            s := StrTran( s, "/", hb_osPathSeparator() )
            s := StrTran( s, "\", hb_osPathSeparator() )
            IF hb_fileExists( s )
               aadd( aUI, StrTran( s, "\", "/" ) )
            ENDIF
         ENDIF
      NEXT
   NEXT

   FOR EACH s IN aUI
      hb_fNameSplit( s, @cPath, @cFile, @cExt )

      cUic := cPath + cFile + ".uic" /* always to be created along .ui */
      cPrg := iif( lToPath, cPathOut + cFile + ".uip", cPathOut )
      cCmd := "uic -o " + cUic + " " + s

      hb_processRun( cCmd )

      oGen := HbUIGen():new( hb_memoread( cUic ) )
      oGen:cFuncName := "ui" + upper( left( cFile, 1 ) ) + lower( substr( cFile, 2 ) )

      s := ""
      aeval( oGen:create(), {|e| s += e + hb_osNewLine() } )
      hb_memowrit( StrTran( cPrg, "/", hb_osPathSeparator() ), s )

      IF lDelUic
         ferase( cUic )
      ENDIF
   NEXT

   RETURN

/*----------------------------------------------------------------------*/

CLASS HbUIGen

   DATA     cFile
   DATA     org
   DATA     cFuncName

   DATA     qObj                                  INIT hb_hash()
   DATA     widgets                               INIT {}
   DATA     aCommands                             INIT {}

   METHOD   new( cFile )
   METHOD   create( cFile )
   METHOD   formatCommand( cCmd, lText )

   ENDCLASS

/*----------------------------------------------------------------------*/

METHOD HbUIGen:new( cFile )

   ::cFile   := cFile

   RETURN Self

/*----------------------------------------------------------------------*/

METHOD HbUIGen:create( cFile )
   LOCAL s, n, n1, cCls, cNam, lCreateFinished, cMCls, cMNam, cText
   LOCAL cCmd, aReg, aCommands, aConst, a_, prg_
   LOCAL regEx := hb_regexComp( "\bQ[A-Za-z_]+ \b" )

   DEFAULT cFile TO ::cFile

   ::cFile   := cFile

   IF empty( ::cFile )
      RETURN Self
   ENDIF
   IF hb_fileExists( ::cFile )
      ::org := hb_ATokens( StrTran( hb_MemoRead( ::cFile ), Chr( 13 ) ), Chr( 10 ) )
   ELSEIF len( ::cFile ) > 256
      ::org := hb_ATokens( StrTran( ::cFile, Chr( 13 ) ), Chr( 10 ) )
   ELSE
      RETURN Self  /* RTE will be generated on appln level */
   ENDIF

   aCommands := {}
   lCreateFinished := .f.

   /* Pullout the widget */
   n := ascan( ::org, {|e| "void setupUi" $ e } )
   IF n == 0
      RETURN Self
   ENDIF
   s     := alltrim( ::org[ n ] )
   n     := at( "*", s )
   cMCls := alltrim( substr( s, 1, n - 1 ) )
   cMNam := alltrim( substr( s, n + 1 ) )
   hbq_stripFront( @cMCls, "(" )
   hbq_stripRear( @cMNam, ")" )
   //
   //   HB_TRACE( HB_TR_ALWAYS, "Widget   ", pad( cMNam, 20 ), pad( cMCls, 20 ), cMCls+"():new()" )
   //                               Validator   Constructor
   aadd( ::widgets, { cMCls, cMNam, cMCls+"()", cMCls+"():new()" } )

   /* Replace Qt #define constants with values */
   aConst := hbq_getConstants()

   /* Normalize */
   FOR EACH s IN ::org
      s := alltrim( s )
      s := alltrim( s )
      IF right( s, 1 ) == ";"
         s := substr( s, 1, len( s ) - 1 )
      ENDIF
      IF left( s, 1 ) $ "/,*,{,}"
         s := ""
      ENDIF
   NEXT

   FOR EACH s IN ::org
      IF empty( s )
         LOOP
      ENDIF

      /* Replace Qt::* with actual values */
      hbq_replaceConstants( @s, aConst )

      IF ( "setupUi" $ s )
         lCreateFinished := .t.

      ELSEIF left( s, 1 ) == "Q" .AND. !( lCreateFinished ) .AND. ( n := at( "*", s ) ) > 0
         // We eill deal later - just skip

      ELSEIF hbq_notAString( s ) .AND. !empty( aReg := hb_Regex( regEx, s ) )
         cCls := trim( aReg[ 1 ] )
         s := alltrim( strtran( s, cCls, "" ) )
         IF ( n := at( "(", s ) ) > 0
            cNam := substr( s, 1, n - 1 )
            aadd( ::widgets, { cCls, cNam, cCls+"()", cCls+"():new"+substr( s, n ) } )
            //
         *  HB_TRACE( HB_TR_ALWAYS, "Object   ", pad( cNam, 20 ), pad( cCls, 20 ), cCls+"():new"+substr( s, n ) )
         ELSE
            cNam := s
            aadd( ::widgets, { cCls, cNam, cCls+"()", cCls+"():new()" } )
            //
         *  HB_TRACE( HB_TR_ALWAYS, "Object   ", pad( cNam, 20 ), pad( cCls,20 ), cCls+"():new()" )
         ENDIF

      ELSEIF hbq_isObjectNameSet( s )
         // Skip - we already know the object name and will set after construction

      ELSEIF !empty( cText := hbq_pullSetToolTip( ::org, s:__enumIndex() ) )
         n := at( "->", cText )
         cNam := alltrim( substr( cText, 1, n - 1 ) )
         cCmd := ::formatCommand( substr( cText, n + 2 ), .t. )
         aadd( aCommands, { cNam, cCmd } )
         //
      *  HB_TRACE( HB_TR_ALWAYS, "Command  ", pad( cNam, 20 ), cCmd )

      ELSEIF !empty( cText := hbq_pullText( ::org, s:__enumIndex() ) )
         n := at( "->", cText )
         cNam := alltrim( substr( cText, 1, n - 1 ) )
         cCmd := ::formatCommand( substr( cText, n + 2 ), .t. )
         aadd( aCommands, { cNam, cCmd } )
         //
      *  HB_TRACE( HB_TR_ALWAYS, "Command  ", pad( cNam, 20 ), cCmd )

      ELSEIF hbq_isValidCmdLine( s ) .AND. !( "->" $ s ) .AND. ( ( n := at( ".", s ) ) > 0  )  /* Assignment to objects on stack */
         cNam := substr( s, 1, n - 1 )
         cCmd := substr( s, n + 1 )
         cCmd := ::formatCommand( cCmd, .f. )
         cCmd := hbq_setObjects( cCmd, ::widgets )
         cCmd := hbq_setObjects( cCmd, ::widgets )
         aadd( aCommands, { cNam, cCmd } )
         //
      *  HB_TRACE( HB_TR_ALWAYS, "Command  ", pad( cNam, 20 ), cCmd )

      ELSEIF !( left( s, 1 ) $ '#/*"' ) .AND. ;          /* Assignment with properties from objects */
                     ( ( n := at( ".", s ) ) > 0  ) .AND. ;
                     ( at( "->", s ) > n )
         cNam := substr( s, 1, n - 1 )
         cCmd := substr( s, n + 1 )
         cCmd := ::formatCommand( cCmd, .f. )
         cCmd := hbq_setObjects( cCmd, ::widgets )
         cCmd := hbq_setObjects( cCmd, ::widgets )
         aadd( aCommands, { cNam, cCmd } )
         //
      *  HB_TRACE( HB_TR_ALWAYS, "Command  ", pad( cNam, 20 ), cCmd )

      ELSEIF ( n := at( "->", s ) ) > 0                  /* Assignments or calls to objects on heap */
         cNam := substr( s, 1, n - 1 )
         cCmd := ::formatCommand( substr( s, n + 2 ), .f. )
         cCmd := hbq_setObjects( cCmd, ::widgets )
         aadd( aCommands, { cNam, cCmd } )
         //
      *  HB_TRACE( HB_TR_ALWAYS, "Command  ", pad( cNam, 20 ), cCmd )

      ELSEIF ( n := at( "= new", s ) ) > 0
         IF ( n1 := at( "*", s ) ) > 0 .AND. n1 < n
            s := alltrim( substr( s, n1 + 1 ) )
         ENDIF
         n    := at( "= new", s )
         cNam := alltrim( substr( s, 1, n - 1 ) )
         cCmd := alltrim( substr( s, n + len( "= new" ) ) )
         cCmd := hbq_setObjects( cCmd, ::widgets )
         n := at( "(", cCmd )
         cCls := substr( cCmd, 1, n - 1 )
         aadd( ::widgets, { cCls, cNam, cCls+"()", cCls+"():new"+substr(cCmd,n) } )
      *  HB_TRACE( HB_TR_ALWAYS, "new      ", pad( cNam, 20 ), cCmd )

      ENDIF
   NEXT

   prg_:={}

   hbq_addCopyRight( @prg_ )

   aadd( prg_, "" )
   aadd( prg_, "FUNCTION " + ::cFuncName + "( qParent )" )
   aadd( prg_, "   LOCAL oUI" )
   aadd( prg_, "   LOCAL oWidget" )
   aadd( prg_, "   LOCAL qObj := {=>}" )
   aadd( prg_, "" )
   aadd( prg_, "   hb_hCaseMatch( qObj, .f. )" )
   aadd( prg_, "" )

   SWITCH cMCls
   CASE "QDialog"
      aadd( prg_, "   oWidget := QDialog():new( qParent )" )
      EXIT
   CASE "QWidget"
      aadd( prg_, "   oWidget := QWidget():new( qParent )" )
      EXIT
   CASE "QMainWindow"
      aadd( prg_, "   oWidget := QMainWindow():new( qParent )" )
      EXIT
   ENDSWITCH
   aadd( prg_, "  " )
   aadd( prg_, "   oWidget:setObjectName( " + STRINGIFY( cMNam ) + " )" )
   aadd( prg_, "  " )
   aadd( prg_, "   qObj[ " + PAD_30( STRINGIFY( cMNam ) ) + " ] := oWidget" )
   aadd( prg_, "  " )

   FOR EACH a_ IN ::widgets
      IF a_:__enumIndex() > 1
         aadd( prg_, "   qObj[ " + PAD_30( STRINGIFY( a_[ 2 ] ) ) + " ] := " + strtran( a_[ 4 ], "o[", "qObj[" ) )
      ENDIF
   NEXT
   aadd( prg_, "   " )

   FOR EACH a_ IN aCommands
      cNam := a_[ 1 ]
      cCmd := a_[ 2 ]
      cCmd := strtran( cCmd, "true" , ".T." )
      cCmd := strtran( cCmd, "false", ".F." )

      IF "addWidget" $ cCmd
         IF hbq_occurs( cCmd, "," ) >= 4
            cCmd := strtran( cCmd, "addWidget", "addWidget_1" )
         ENDIF

      ELSEIF "addLayout" $ cCmd
         IF hbq_occurs( cCmd, "," ) >= 4
            cCmd := strtran( cCmd, "addLayout", "addLayout_1" )
         ENDIF
      ENDIF

      IF "setToolTip(" $ cCmd
         s := hbq_pullToolTip( cCmd )
         aadd( prg_, "   qObj[ " + PAD_30( STRINGIFY( cNam ) ) + " ]:setToolTip( [" + STRIP_SQ( s ) + "] )" )

      ELSEIF "setPlainText(" $ cCmd
         s := hbq_pullToolTip( cCmd )
         aadd( prg_, "   qObj[ " + PAD_30( STRINGIFY( cNam ) ) + " ]:setPlainText( [" + STRIP_SQ( s ) + "] )" )

      ELSEIF "setStyleSheet(" $ cCmd
         s := hbq_pullToolTip( cCmd )
         aadd( prg_, "   qObj[ " + PAD_30( STRINGIFY( cNam ) ) + " ]:setStyleSheet( [" + STRIP_SQ( s ) + "] )" )

      ELSEIF "setText(" $ cCmd
         s := hbq_pullToolTip( cCmd )
         aadd( prg_, "   qObj[ " + PAD_30( STRINGIFY( cNam ) ) + " ]:setText( [" + STRIP_SQ( s ) + "] )" )

      ELSEIF "setWhatsThis(" $ cCmd
         s := hbq_pullToolTip( cCmd )
         aadd( prg_, "   qObj[ " + PAD_30( STRINGIFY( cNam ) ) + " ]:setWhatsThis( [" + STRIP_SQ( s ) + "] )" )

      ELSEIF "header()->" $ cCmd
         // TODO: how to handle : __qtreeviewitem->header()->setVisible( .f. )

      ELSEIF cCmd == "pPtr"
         // Nothing TO DO

      ELSE
         aadd( prg_, "   qObj[ " + PAD_30( STRINGIFY( cNam ) ) + " ]:" + strtran( cCmd, "o[", "qObj[" ) )

      ENDIF
   NEXT
   aadd( prg_, "" )
   aadd( prg_, "   oUI         := HbQtUI():new()" )
   aadd( prg_, "   oUI:qObj    := qObj"    )
   aadd( prg_, "   oUI:oWidget := oWidget" )
   aadd( prg_, "" )
   aadd( prg_, "   RETURN oUI" )
   aadd( prg_, "" )
   aadd( prg_, "/*----------------------------------------------------------------------*/" )
   aadd( prg_, "" )

   RETURN prg_

/*----------------------------------------------------------------------*/

METHOD HbUIGen:formatCommand( cCmd, lText )
   LOCAL regDefine, aDefine, n, n1, cNam, cCmd1

   STATIC nn := 100

   DEFAULT lText TO .t.

   cCmd := strtran( cCmd, "QApplication::translate"  , "q__tr"        )
   cCmd := strtran( cCmd, "QApplication::UnicodeUTF8", '"UTF8"'       )
   cCmd := strtran( cCmd, "QString()"                , '""'           )
   cCmd := strtran( cCmd, "QSize("                   , "QSize():new(" )
   cCmd := strtran( cCmd, "QRect("                   , "QRect():new(" )

   IF ( "::" $ cCmd )
      regDefine := hb_RegexComp( "\b[A-Za-z_]+\:\:[A-Za-z_]+\b" )
      aDefine := hb_RegEx( regDefine, cCmd )
      IF !empty( aDefine )
         cCmd := strtran( cCmd, "::", "_" )    /* Qt Defines  - how to handle */
      ENDIF
   ENDIF

   IF ! lText .AND. ( at( ".", cCmd ) ) > 0
      // sizePolicy     setHeightForWidth(ProjectProperties->sizePolicy().hasHeightForWidth());
      //
      IF ( at( "setHeightForWidth(", cCmd ) ) > 0
         cNam := "__qsizePolicy" + hb_ntos( ++nn )
         n    := at( "(", cCmd )
         n1   := at( ".", cCmd )
         cCmd1 := hbq_setObjects( substr( cCmd, n + 1, n1 - n - 1 ), ::widgets )
         cCmd1 := strtran( cCmd1, "->", ":" )
         aadd( ::widgets, { "QSizePolicy", cNam, "QSizePolicy()", "QSizePolicy():configure(" + cCmd1 + ")" } )
         cCmd := 'setHeightForWidth(o[ "' + cNam + '" ]:' + substr( cCmd, n1 + 1 )

      ELSE
         cCmd := "pPtr"

      ENDIF
   ENDIF

   RETURN cCmd

/*----------------------------------------------------------------------*/

STATIC FUNCTION hbq_isObjectNameSet( s )
   RETURN ( "objectName" $ s .OR. "ObjectName" $ s )

/*----------------------------------------------------------------------*/

STATIC FUNCTION hbq_isValidCmdLine( s )
   RETURN !( left( s, 1 ) $ '#/*"' )

/*----------------------------------------------------------------------*/

STATIC FUNCTION hbq_notAString( s )
   RETURN !( left( s, 1 ) == '"' )

/*----------------------------------------------------------------------*/

STATIC FUNCTION hbq_occurs( s, c )
   LOCAL i, n, nLen := len( s )

   n := 0
   FOR i := 1 TO nLen
      IF substr( s, i, 1 ) == c
         n++
      ENDIF
   NEXT
   RETURN n

/*----------------------------------------------------------------------*/

STATIC FUNCTION hbq_pullToolTip( cCmd )
   LOCAL n, s := ""

   IF ( n := at( ', "', cCmd ) ) > 0
      s := alltrim( substr( cCmd, n + 2 ) )
      IF ( n := at( '", 0', s ) ) > 0
         s := alltrim( substr( s, 1, n ) )
         s := strtran( s, '\"', '"' )
         //s := strtran( s, '\n', chr( 10 ) )
         s := strtran( s, '""', "" )
         s := substr( s, 2, len( s ) - 2 )
      ENDIF
   ENDIF

   RETURN s

/*----------------------------------------------------------------------*/

STATIC FUNCTION hbq_replaceConstants( s, hConst )
   LOCAL a_, regDefine, cConst, cCmdB, cCmdE, cOR, n
   LOCAL g := s
   LOCAL b_:= {}
   LOCAL nOrs := hbq_occurs( s, "|" )

   regDefine := hb_RegexComp( "\b[A-Za-z_]+\:\:[A-Za-z_]+\b" )

   IF nOrs > 0
      FOR n := 1 TO nOrs + 1
         a_:= hb_RegEx( regDefine, g )
         IF !empty( a_ )
            aadd( b_, a_[ 1 ] )
            g := substr( g, at( a_[ 1 ], g ) + len( a_[ 1 ] ) )
         ENDIF
      NEXT
   ENDIF

   IF !empty( b_ )
      cOR := "hb_bitOR(" + b_[ 1 ] + "," + b_[ 2 ] +")"
      FOR n := 3 TO len( b_ )
         cOR := "hb_bitOR(" + cOR + "," + b_[ n ] + ")"
      NEXT
      cCmdB  := substr( s, 1, at( b_[ 1 ], s ) - 1 )
      cConst := b_[ len( b_ ) ]
      cCmdE  := substr( s, at( cConst, s ) + len( cConst ) )
      s      := cCmdB + cOR + cCmdE
   ENDIF

   IF ( "::" $ s )
      DO WHILE .t.
         a_:= hb_RegEx( regDefine, s )
         IF empty( a_ )
            EXIT
         ENDIF
         cConst := strtran( a_[ 1 ], "::", "_" )
         IF !( cConst $ hConst )
            EXIT
         ENDIF
         s := strtran( s, a_[ 1 ], hb_ntos( hConst[ cConst ] ) )
      ENDDO
   ENDIF

   RETURN NIL

/*----------------------------------------------------------------------*/

STATIC FUNCTION hbq_setObjects( cCmd, aObj_ )
   LOCAL n, cObj
   IF ( n := ascan( aObj_, {|e_| ( e_[ 2 ] + "," ) $ cCmd } ) ) > 0
      cObj := aObj_[ n, 2 ]
      cCmd := strtran( cCmd, ( cObj + "," ), 'o[ "' + cObj + '" ],' )
   ENDIF
   IF ( n := ascan( aObj_, {|e_| ( e_[ 2 ] + ")" ) $ cCmd } ) ) > 0
      cObj := aObj_[ n, 2 ]
      cCmd := strtran( cCmd, ( cObj + ")" ), 'o[ "' + cObj + '" ])' )
   ENDIF
   IF ( n := ascan( aObj_, {|e_| ( e_[ 2 ] + "->" ) $ cCmd } ) ) > 0
      cObj := aObj_[ n, 2 ]
      cCmd := strtran( cCmd, ( cObj + "->" ), 'o[ "' + cObj + '" ]:' )
   ENDIF
   RETURN cCmd

/*----------------------------------------------------------------------*/

FUNCTION q__tr( p1, p2, p3, p4 )

   HB_SYMBOL_UNUSED( p1 )
   HB_SYMBOL_UNUSED( p3 )
   HB_SYMBOL_UNUSED( p4 )

   RETURN p2

/*----------------------------------------------------------------------*/

STATIC FUNCTION hbq_pullText( org_, nFrom )
   LOCAL s := "", nLen := len( org_ )
   LOCAL a_:= { "setText(", "setPlainText(", "setStyleSheet(", "setWhatsThis(" }

   IF ascan( a_, {|e| e $ org_[ nFrom ] } ) > 0
      s := org_[ nFrom ]
      nFrom ++
      DO WHILE nFrom <= nLen
         IF !( left( org_[ nFrom ], 1 ) == '"' )
            EXIT
         ENDIF
         s += org_[ nFrom ]
         org_[ nFrom ] := ""
         nFrom++
      ENDDO
   ENDIF

   RETURN s

/*----------------------------------------------------------------------*/

STATIC FUNCTION hbq_pullSetToolTip( org_, nFrom )
   LOCAL s := "", nLen := len( org_ )

   IF ( "#ifndef QT_NO_TOOLTIP" $ org_[ nFrom ] )
      nFrom++
      DO WHILE nFrom <= nLen
         IF ( "#endif // QT_NO_TOOLTIP" $ org_[ nFrom ] )
            EXIT
         ENDIF
         s += org_[ nFrom ]
         org_[ nFrom ] := ""
         nFrom++
      ENDDO
   ENDIF
   RETURN s

/*----------------------------------------------------------------------*/

STATIC FUNCTION hbq_stripFront( s, cTkn )
   LOCAL n
   LOCAL nLen := len( cTkn )

   IF ( n := at( cTkn, s ) ) > 0
      s := substr( s, n + nLen )
      RETURN .t.
   ENDIF

   RETURN .f.

/*----------------------------------------------------------------------*/

STATIC FUNCTION hbq_stripRear( s, cTkn )
   LOCAL n

   IF ( n := rat( cTkn, s ) ) > 0
      s := substr( s, 1, n - 1 )
      RETURN .t.
   ENDIF

   RETURN .f.

/*----------------------------------------------------------------------*/

STATIC FUNCTION hbq_getConstants()
   STATIC h_

   IF empty( h_ )
      h_:= ;
      { ;
         "QSizePolicy_Fixed"                      => QSizePolicy_Fixed                     , ;
         "QSizePolicy_Minimum"                    => QSizePolicy_Minimum                   , ;
         "QSizePolicy_Maximum"                    => QSizePolicy_Maximum                   , ;
         "QSizePolicy_Preferred"                  => QSizePolicy_Preferred                 , ;
         "QSizePolicy_Expanding"                  => QSizePolicy_Expanding                 , ;
         "QSizePolicy_MinimumExpanding"           => QSizePolicy_MinimumExpanding          , ;
         "QSizePolicy_Ignored"                    => QSizePolicy_Ignored                   , ;
         ;
         "Qt_AlignLeft"                           => Qt_AlignLeft                          , ;
         "Qt_AlignRight"                          => Qt_AlignRight                         , ;
         "Qt_AlignHCenter"                        => Qt_AlignHCenter                       , ;
         "Qt_AlignJustify"                        => Qt_AlignJustify                       , ;
         "Qt_AlignTop"                            => Qt_AlignTop                           , ;
         "Qt_AlignBottom"                         => Qt_AlignBottom                        , ;
         "Qt_AlignVCenter"                        => Qt_AlignVCenter                       , ;
         "Qt_AlignCenter"                         => Qt_AlignCenter                        , ;
         "Qt_AlignAbsolute"                       => Qt_AlignAbsolute                      , ;
         "Qt_AlignLeading"                        => Qt_AlignLeading                       , ;
         "Qt_AlignTrailing"                       => Qt_AlignTrailing                      , ;
         ;
         "QPlainTextEdit_NoWrap"                  => QPlainTextEdit_NoWrap                 , ;
         "QPlainTextEdit_WidgetWidth"             => QPlainTextEdit_WidgetWidth            , ;
         ;
         "QTabWidget_North"                       => QTabWidget_North                      , ;
         "QTabWidget_South"                       => QTabWidget_South                      , ;
         "QTabWidget_West"                        => QTabWidget_West                       , ;
         "QTabWidget_East"                        => QTabWidget_East                       , ;
         "QTabWidget_Rounded"                     => QTabWidget_Rounded                    , ;
         "QTabWidget_Triangular"                  => QTabWidget_Triangular                 , ;
         "QMainWindow_AnimatedDocks"              => QMainWindow_AnimatedDocks             , ;
         "QMainWindow_AllowNestedDocks"           => QMainWindow_AllowNestedDocks          , ;
         "QMainWindow_AllowTabbedDocks"           => QMainWindow_AllowTabbedDocks          , ;
         "QMainWindow_ForceTabbedDocks"           => QMainWindow_ForceTabbedDocks          , ;
         "QMainWindow_VerticalTabs"               => QMainWindow_VerticalTabs              , ;
         ;
         "QLayout_SetDefaultConstraint"           => QLayout_SetDefaultConstraint          , ;
         "QLayout_SetFixedSize"                   => QLayout_SetFixedSize                  , ;
         "QLayout_SetMinimumSize"                 => QLayout_SetMinimumSize                , ;
         "QLayout_SetMaximumSize"                 => QLayout_SetMaximumSize                , ;
         "QLayout_SetMinAndMaxSize"               => QLayout_SetMinAndMaxSize              , ;
         "QLayout_SetNoConstraint"                => QLayout_SetNoConstraint               , ;
         ;
         "QFrame_Plain"                           => QFrame_Plain                          , ;
         "QFrame_Raised"                          => QFrame_Raised                         , ;
         "QFrame_Sunken"                          => QFrame_Sunken                         , ;
         "QFrame_NoFrame"                         => QFrame_NoFrame                        , ;
         "QFrame_Box"                             => QFrame_Box                            , ;
         "QFrame_Panel"                           => QFrame_Panel                          , ;
         "QFrame_StyledPanel"                     => QFrame_StyledPanel                    , ;
         "QFrame_HLine"                           => QFrame_HLine                          , ;
         "QFrame_VLine"                           => QFrame_VLine                          , ;
         "QFrame_WinPanel"                        => QFrame_WinPanel                       , ;
         "QFrame_Shadow_Mask"                     => QFrame_Shadow_Mask                    , ;
         "QFrame_Shape_Mask"                      => QFrame_Shape_Mask                     , ;
         ;
         "QAbstractItemView_NoEditTriggers"       => QAbstractItemView_NoEditTriggers      , ;
         "QAbstractItemView_CurrentChanged"       => QAbstractItemView_CurrentChanged      , ;
         "QAbstractItemView_DoubleClicked"        => QAbstractItemView_DoubleClicked       , ;
         "QAbstractItemView_SelectedClicked"      => QAbstractItemView_SelectedClicked     , ;
         "QAbstractItemView_EditKeyPressed"       => QAbstractItemView_EditKeyPressed      , ;
         "QAbstractItemView_AnyKeyPressed"        => QAbstractItemView_AnyKeyPressed       , ;
         "QAbstractItemView_AllEditTriggers"      => QAbstractItemView_AllEditTriggers     , ;
         "QAbstractItemView_NoSelection"          => QAbstractItemView_NoSelection         , ;
         "QAbstractItemView_MultiSelection"       => QAbstractItemView_MultiSelection      , ;
         "QAbstractItemView_SingleSelection"      => QAbstractItemView_SingleSelection     , ;
         "QAbstractItemView_ContiguousSelection"  => QAbstractItemView_ContiguousSelection , ;
         "QAbstractItemView_ExtendedSelection"    => QAbstractItemView_ExtendedSelection   , ;
         ;
         "QTextEdit_NoWrap"                       => QTextEdit_NoWrap                      , ;
         "QTextEdit_WidgetWidth"                  => QTextEdit_WidgetWidth                 , ;
         "QTextEdit_FixedPixelWidth"              => QTextEdit_FixedPixelWidth             , ;
         "QTextEdit_FixedColumnWidth"             => QTextEdit_FixedColumnWidth            , ;
         ;
         "Qt_ScrollBarAsNeeded"                   => Qt_ScrollBarAsNeeded                  , ;
         "Qt_ScrollBarAlwaysOff"                  => Qt_ScrollBarAlwaysOff                 , ;
         "Qt_ScrollBarAlwaysOn"                   => Qt_ScrollBarAlwaysOn                  , ;
         ;
         "Qt_Horizontal"                          => Qt_Horizontal                         , ;
         "Qt_Vertical"                            => Qt_Vertical                           , ;
         ;
         "Qt_TabFocus"                            => Qt_TabFocus                           , ;
         "Qt_ClickFocus"                          => Qt_ClickFocus                         , ;
         "Qt_StrongFocus"                         => Qt_StrongFocus                        , ;
         "Qt_WheelFocus"                          => Qt_WheelFocus                         , ;
         "Qt_NoFocus"                             => Qt_NoFocus                              ;
      }
   ENDIF

   RETURN h_

/*----------------------------------------------------------------------*/

STATIC FUNCTION hbq_addCopyRight( prg_ )

   aadd( prg_, "/*" )
   aadd( prg_, " * " + "$" + "Id" + "$" )
   aadd( prg_, " */" )
   aadd( prg_, "" )
   aadd( prg_, "/* -------------------------------------------------------------------- */" )
   aadd( prg_, "/* WARNING: Automatically generated source file. DO NOT EDIT!           */" )
   aadd( prg_, "/*          Instead, edit corresponding .ui file,                       */" )
   aadd( prg_, "/*          with Qt Generator, and run hbqtui.exe.                      */" )
   aadd( prg_, "/* -------------------------------------------------------------------- */" )
   aadd( prg_, "/*                                                                      */" )
   aadd( prg_, "/*               Pritpal Bedi <bedipritpal@hotmail.com>                 */" )
   aadd( prg_, "/*                                                                      */" )
   aadd( prg_, "/* -------------------------------------------------------------------- */" )

   RETURN NIL

/*----------------------------------------------------------------------*/
