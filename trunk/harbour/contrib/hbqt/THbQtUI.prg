/*
 * $Id$
 */

/*
 * Harbour Project source code:
 * Source file for the Xbp*Classes
 *
 * Copyright 2010 Pritpal Bedi <pritpal@vouchcac.com>
 * http://www.harbour-project.org
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
 *                     Harbour Parts HbQtUI Class
 *
 *                 Pritpal Bedi <pritpal@vouchcac.com>
 *                              28Jan2010
 */
/*----------------------------------------------------------------------*/
/*----------------------------------------------------------------------*/
/*----------------------------------------------------------------------*/

#include "hbclass.ch"
#include "common.ch"
#include "hbqt.ch"

/*----------------------------------------------------------------------*/

CLASS HbQtUI

   DATA     cFile
   DATA     qParent

   DATA     pSlots
   DATA     pEvents
   DATA     oWidget
   DATA     cMainWidgetName

   DATA     qObj                                  INIT hb_hash()
   DATA     widgets                               INIT {}
   DATA     aCommands                             INIT {}

   DATA     aSignals                              INIT {}
   DATA     aEvents                               INIT {}

   DATA     org
   DATA     ui_                                   INIT {=>}

   METHOD   new( cFile, qParent )
   METHOD   create( cFile, qParent )
   METHOD   destroy()

   METHOD   event( cWidget, nEvent, bBlock )
   METHOD   signal( cWidget, cSignal, bBlock )
   METHOD   loadWidgets()
   METHOD   loadContents( cUiFull )
   METHOD   loadUI( cUiFull, qParent )
   METHOD   build( cFileOrBuffer, qParent )
   METHOD   formatCommand( cCmd, lText )

   ERROR HANDLER OnError( ... )

   ENDCLASS

/*----------------------------------------------------------------------*/

METHOD HbQtUI:new( cFile, qParent )

   ::cFile   := cFile
   ::qParent := qParent

   RETURN Self

/*----------------------------------------------------------------------*/

METHOD HbQtUI:create( cFile, qParent )

   DEFAULT cFile   TO ::cFile
   DEFAULT qParent TO ::qParent

   ::cFile := cFile
   ::qParent := qParent

   IF !empty( ::cFile ) .AND. hb_fileExists( ::cFile )
      hb_hCaseMatch( ::qObj, .f. )

      ::loadContents( ::cFile, ::qParent )

      ::oWidget := ::loadUI( ::cFile, ::qParent )

      IF !empty( ::oWidget )
         ::loadWidgets()
      ENDIF
   ENDIF

   RETURN Self

/*----------------------------------------------------------------------*/

METHOD HbQtUI:destroy()
   LOCAL a_, i
   //LOCAL cNam, cCmd, cWdg, n

   ::oWidget:hide()

   FOR EACH a_ IN ::aSignals
      i := Qt_Slots_disConnect( ::pSlots, a_[ 1 ], a_[ 2 ] )
//HB_TRACE( HB_TR_ALWAYS, 300, i, "Qt_Slots_disConnect", a_[ 2 ] )
      a_:= NIL
   NEXT
   ::pSlots := NIL
   FOR EACH a_ IN ::aEvents
      Qt_Events_disConnect( ::pEvents, a_[ 1 ], a_[ 2 ] )
   NEXT
   ::pEvents := NIL

#if 0
   FOR EACH a_ IN ::aCommands
      IF a_[ 1 ] $ ::qObj
         IF ! "splitter" $ lower( a_[ 1 ] )
            cNam := a_[ 1 ] ; cCmd := a_[ 2 ]
            IF ( "addWidget" $ cCmd ) .OR. ( "addWidget" $ cCmd )
               IF ( n := at( "(o[ ", cCmd ) ) > 0
                  cWdg := substr( cCmd, n + 5 )
                  IF ( n := at( '"', cWdg ) ) > 0
                     cWdg := substr( cWdg, 1, n - 1 )
                     IF     "addWidget" $ cCmd
   hbide_dbg( 200, cNam, cWdg )
                        ::qObj[ cNam ]:removeWidget( ::qObj[ cWdg ] )
                     ELSEIF "addLayout" $ cCmd
   hbide_dbg( 205, cNam, cWdg )
                        ::qObj[ cNam ]:removeLayout( ::qObj[ cWdg ] )
                     ENDIF
                  ENDIF
               ENDIF
            ENDIF
         ENDIF
      ENDIF
   NEXT
#endif

   FOR EACH a_ IN ::widgets DESCEND
      IF ( i := a_:__enumIndex() ) > 1
         IF type( a_[ 3 ] ) == "UI"
            IF !( a_[ 1 ] $ "QHBoxLayout,QVBoxLayout,QGridLayout" )
//HB_TRACE( HB_TR_ALWAYS, 400, i, pad( a_[ 1 ], 20 ), pad( a_[ 2 ], 20 ), iif( i > 1, pad( ::widgets[ i - 1, 1 ],20 ), NIL ), i, len( ::widgets ) )
               ::qObj[ a_[ 2 ] ] := NIL
            ENDIF
         ELSEIF type( a_[ 3 ] ) != "UI"
//HB_TRACE( HB_TR_ALWAYS, 500, 0, pad( a_[ 1 ], 20 ), pad( a_[ 2 ], 20 ), iif( i > 1, pad( ::widgets[ i - 1, 1 ],20 ), NIL ), i, len( ::widgets ) )
         ENDIF
      ENDIF
   NEXT

   #if 1
   FOR EACH a_ IN ::widgets DESCEND
      IF ( i := a_:__enumIndex() ) > 1
         IF type( a_[ 3 ] ) == "UI"  .AND. ( a_[ 1 ] $ "QHBoxLayout,QVBoxLayout,QGridLayout" )
            IF i > 2
//HB_TRACE( HB_TR_ALWAYS, 600, i, pad( a_[ 1 ], 20 ), pad( a_[ 2 ], 20 ), iif( i > 1, pad( ::widgets[ i - 1, 1 ],20 ), NIL ), i, len( ::widgets ) )
               ::qObj[ a_[ 2 ] ] := NIL
            ENDIF
         ENDIF
      ENDIF
   NEXT
   #endif

   ::qObj[ ::cMainWidgetName ] := NIL
   ::widgets[ 1, 2 ] := NIL
   ::aEvents  := NIL
   ::qObj := NIL
   ::widgets := {}

HB_TRACE( HB_TR_ALWAYS, 101 )
   ::oWidget:close()
HB_TRACE( HB_TR_ALWAYS, 102 )
//   ::oWidget := NIL  /* Variable Destruction GPFs */
HB_TRACE( HB_TR_ALWAYS, 103 )
   hbide_justACall( i )
   RETURN NIL

/*----------------------------------------------------------------------*/

METHOD HbQtUI:event( cWidget, nEvent, bBlock )

   IF hb_hHasKey( ::qObj, cWidget )
      IF Qt_Events_Connect( ::pEvents, ::qObj[ cWidget ], nEvent, bBlock )
         aadd( ::aEvents, { ::qObj[ cWidget ], nEvent } )
      ENDIF
   ENDIF

   RETURN Self

/*----------------------------------------------------------------------*/

METHOD HbQtUI:signal( cWidget, cSignal, bBlock )

   IF hb_hHasKey( ::qObj, cWidget )
      IF empty( ::pSlots )
         ::pSlots := QT_SLOTS_NEW()
      ENDIF
      IF Qt_Slots_Connect( ::pSlots, ::qObj[ cWidget ], cSignal, bBlock )
         aadd( ::aSignals, { ::qObj[ cWidget ], cSignal } )
      ELSE
         HB_TRACE( HB_TR_ALWAYS, "Failed:", cSignal )
      ENDIF
   ENDIF

   RETURN Self

/*----------------------------------------------------------------------*/

METHOD HbQtUI:loadWidgets()
   LOCAL a_, pPtr, bBlock, x, cBlock

   FOR EACH a_ IN ::widgets
      IF a_:__enumIndex() > 1
         IF type( a_[ 1 ] + "()" ) == "UI"
            cBlock := "{|| " + a_[ 1 ] + "() }"

            pPtr := Qt_findChild( ::oWidget, a_[ 2 ] )
            bBlock := &( cBlock )

            x := eval( bBlock )
            IF hb_isObject( x )
               x:pPtr := pPtr
               ::qObj[ a_[ 2 ] ] := x
            ENDIF
         ENDIF
      ENDIF
   NEXT

   RETURN Self

/*----------------------------------------------------------------------*/

METHOD HbQtUI:loadContents( cUiFull )
   LOCAL cBuffer := memoread( cUiFull )
   LOCAL n, cClass, cWidget

   DO WHILE .t.
      IF ( n := at( "widget class=", cBuffer ) ) == 0
         EXIT
      ENDIF
      cBuffer := substr( cBuffer, n + 13 )

      n := at( "name=", cBuffer )
      cClass := alltrim( strtran( substr( cBuffer, 1, n-1 ), '"', "" ) )
      cBuffer := substr( cBuffer, n + 5 )

      n := at( ">", cBuffer )
      cWidget := alltrim( strtran( substr( cBuffer, 1, n-1 ), '"', "" ) )
      cWidget := strtran( cWidget, "/", "" )

      aadd( ::widgets, { cClass, cWidget } )
   ENDDO

   RETURN Self

/*----------------------------------------------------------------------*/

METHOD HbQtUI:loadUI( cUiFull, qParent )
   LOCAL oWidget, qUiLoader, qFile, pWidget
   #if 1
   LOCAL cBuffer

   /* This method allows to use ui components stored in a
    * database or embedded into sources as a text file stream
    */
   cBuffer := hb_memoRead( cUiFull )
   qFile := QBuffer():new()
   qFile:setData( cBuffer, len( cBuffer ) )
   #else
   qFile := QFile():new( cUiFull )
   #endif
   IF qFile:open( 1 )
      qUiLoader  := QUiLoader():new()
      pWidget    := qUiLoader:load( qFile, qParent )
      DO CASE
      CASE ::widgets[ 1,1 ] == "QWidget"
         oWidget    := QWidget():configure( pWidget )
      CASE ::widgets[ 1,1 ] == "QDialog"
         oWidget    := QDialog():configure( pWidget )
      CASE ::widgets[ 1,1 ] == "QMainWindow"
         oWidget    := QMainWindow():configure( pWidget )
      OTHERWISE
         oWidget    := QWidget():configure( pWidget )
      ENDCASE
      qFile:close()
      qFile := NIL
      qUiLoader := NIL
   ENDIF

   ::oWidget := oWidget

   RETURN oWidget

/*----------------------------------------------------------------------*/

METHOD HbQtUI:OnError( ... )
   LOCAL cMsg, xReturn

   cMsg := __GetMessage()
   IF SubStr( cMsg, 1, 1 ) == "_"
      cMsg := SubStr( cMsg, 2 )
   ENDIF

   IF left( cMsg, 2 ) == "Q_"
      xReturn := ::qObj[ substr( cMsg, 3 ) ]
   ELSE
      xReturn := ::oWidget:&cMsg( ... )
   ENDIF

   RETURN xReturn

/*----------------------------------------------------------------------*/

METHOD HbQtUI:build( cFileOrBuffer, qParent )
   LOCAL s, n, n1, cCls, cNam, lCreateFinished, cMCls, cMNam, cText
   LOCAL cCmd, aReg, aCommands, aConst, cBlock, bBlock, x, a_
   LOCAL regEx := hb_regexComp( "\bQ[A-Za-z_]+ \b" )

   DEFAULT cFileOrBuffer TO ::cFile
   DEFAULT qParent       TO ::qParent

   ::cFile   := cFileOrBuffer
   ::qParent := qParent

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

   hb_hCaseMatch( ::qObj, .f. )

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

   /* Platform is ready */

   ::cMainWidgetName := cMNam

   SWITCH cMCls
   CASE "QDialog"
      ::oWidget := QDialog():new( ::qParent )
      EXIT
   CASE "QWidget"
      ::oWidget := QWidget():new( ::qParent )
      EXIT
   CASE "QMainWindow"
      ::oWidget := QMainWindow():new( ::qParent )
      EXIT
   ENDSWITCH
   ::oWidget:setObjectName( cMNam )

   ::qObj[ cMNam ] := ::oWidget

//HB_TRACE( HB_TR_ALWAYS, "------------------------------------------------------------" )
   FOR EACH a_ IN ::widgets
      IF a_:__enumIndex() > 1
         IF type( a_[ 3 ] ) == "UI"
            cBlock := "{|o| " + a_[ 4 ] + "}"
//HB_TRACE( HB_TR_ALWAYS, "Constr   ", pad( a_[ 2 ], 20 ), cBlock )
            bBlock := &( cBlock )

            x := eval( bBlock, ::qObj )
            IF hb_isObject( x )
               ::qObj[ a_[ 2 ] ] := x
            ENDIF
         ELSE
//HB_TRACE( HB_TR_ALWAYS, "----------------------------", a_[ 3 ] )
         ENDIF
      ENDIF
   NEXT
//HB_TRACE( HB_TR_ALWAYS, "------------------------------------------------------------" )

   ::aCommands := aCommands

   FOR EACH a_ IN aCommands
      IF a_[ 1 ] $ ::qObj
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
            ::qObj[ cNam ]:setToolTip( s )

         ELSEIF "setPlainText(" $ cCmd
            s := hbq_pullToolTip( cCmd )
            ::qObj[ cNam ]:setPlainText( s )

         ELSEIF "setStyleSheet(" $ cCmd
            s := hbq_pullToolTip( cCmd )
            ::qObj[ cNam ]:setStyleSheet( s )

         ELSEIF "setText(" $ cCmd
            s := hbq_pullToolTip( cCmd )
            ::qObj[ cNam ]:setText( s )

         ELSEIF "header()->" $ cCmd
            // TODO: how to handle : __qtreeviewitem->header()->setVisible( .f. )

         ELSE
            cBlock := "{|o,v| o[v]:" + cCmd + "}"
//HB_TRACE( HB_TR_ALWAYS, pad( a_[ 1 ], 20 ), cBlock )
            bBlock := &( cBlock )
            eval( bBlock, ::qObj, cNam )

         ENDIF
      ENDIF
   NEXT
   //::oWidget:exec()

   RETURN Self

/*----------------------------------------------------------------------*/

METHOD HbQtUI:formatCommand( cCmd, lText )
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
         s := strtran( s, '\n', chr( 10 ) )
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
   LOCAL a_:= { "setText(", "setPlainText(", "setStyleSheet(" }

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
