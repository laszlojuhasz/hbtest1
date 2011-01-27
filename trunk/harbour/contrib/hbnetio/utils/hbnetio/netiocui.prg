/*
 * $Id$
 */

/*
 * The following parts are Copyright of the individual authors.
 * www - http://harbour-project.org
 *
 * Copyright 2010 Viktor Szakats (harbour.01 syenar.hu)
 *
 * See COPYING for licensing terms.
 *
 */

#include "color.ch"
#include "inkey.ch"
#include "setcurs.ch"

#include "hbgtinfo.ch"

#define _NETIOMGM_IPV4_DEF  "127.0.0.1"
#define _NETIOMGM_PORT_DEF  2940

#if ! defined( _CONSOLEUI_EMBEDDED )

PROCEDURE Main( ... )
   LOCAL cParam

   LOCAL cIP := _NETIOMGM_IPV4_DEF
   LOCAL nPort := _NETIOMGM_PORT_DEF
   LOCAL cPassword := ""

   SET DATE ANSI
   SET CENTURY ON
   SET CONFIRM ON
   SET SCOREBOARD OFF

   HB_Logo()

   FOR EACH cParam IN { ... }
      DO CASE
      CASE Lower( Left( cParam, 6 ) ) == "-addr="
         IPPortSplit( SubStr( cParam, 7 ), @cIP, @nPort )
         IF Empty( nPort )
            nPort := _NETIOMGM_PORT_DEF
         ENDIF
      CASE Lower( Left( cParam, 6 ) ) == "-pass="
         cPassword := SubStr( cParam, 7 )
         hb_StrClear( @cParam )
      CASE Lower( cParam ) == "--version"
         RETURN
      CASE Lower( cParam ) == "-help" .OR. ;
           Lower( cParam ) == "--help"
         HB_Usage()
         RETURN
      OTHERWISE
         OutStd( "Warning: Unkown parameter ignored: " + cParam + hb_eol() )
      ENDCASE
   NEXT

   netiosrv_cmdUI( cIP, nPort, cPassword )

   RETURN

STATIC PROCEDURE HB_Logo()

   OutStd( "Harbour NETIO Server Management Console " + StrTran( Version(), "Harbour " ) + hb_eol() +;
           "Copyright (c) 2009-2011, Viktor Szakats" + hb_eol() + ;
           "http://harbour-project.org/" + hb_eol() +;
           hb_eol() )

   RETURN

STATIC PROCEDURE HB_Usage()

   OutStd(               "Syntax:"                                                                                , hb_eol() )
   OutStd(                                                                                                          hb_eol() )
   OutStd(               "  netiocui [options]"                                                                   , hb_eol() )
   OutStd(                                                                                                          hb_eol() )
   OutStd(               "Options:"                                                                               , hb_eol() )
   OutStd(                                                                                                          hb_eol() )
   OutStd(               "  -addr=<ip[:port]>  connect to netio server on IPv4 address <ip:port>"                 , hb_eol() )
   OutStd( hb_StrFormat( "                     Default: %1$s:%2$d", _NETIOMGM_IPV4_DEF, _NETIOMGM_PORT_DEF )      , hb_eol() )
   OutStd(               "  -pass=<passwd>     connect to netio server with password"                             , hb_eol() )
   OutStd(                                                                                                          hb_eol() )
   OutStd(               "  --version          display version header only"                                       , hb_eol() )
   OutStd(               "  -help|--help       this help"                                                         , hb_eol() )

   RETURN

#endif

STATIC PROCEDURE cmdHelp( hCommands )
   LOCAL aTexts := {}
   LOCAL n, c, m

   m := 0
   hb_HEval( hCommands, {| k, l | m := Max( m, Len( k + iif( Empty( l[ 1 ] ), "", " " + l[ 1 ] ) ) ) } )

   AAdd( aTexts, "Commands:" )

   /* Processing commands */
   FOR EACH n IN hCommands
      AAdd( aTexts, " " + PadR( n:__enumKey() + iif( Empty( n[ 1 ] ), "", " " + n[ 1 ] ), m ) + " - " + n[ 2 ] )
   NEXT

   ASort( aTexts, 2 )

   AAdd( aTexts, "" )
   AAdd( aTexts, "Keyboard shortcuts:" )
   AAdd( aTexts, PadR( " <Up>", m )    + "  - Move up on historic list." )
   AAdd( aTexts, PadR( " <Down>", m )  + "  - Move down on historic list." )
   AAdd( aTexts, PadR( " <Tab>", m )   + "  - Complete command." )
   AAdd( aTexts, PadR( " <Alt+V>", m ) + "  - Paste from clipboard." )

   c := 0
   m := MaxRow()

   FOR EACH n IN aTexts
      QQOut( n, hb_eol() )

      IF ++c == m
         c := 0
         QQOut( "Press any key to continue..." )
         Inkey( 0 )

         Scroll( Row(), 0, Row(), MaxCol(), 0 )
         SetPos( Row(), 0 )
      ENDIF
   NEXT

   RETURN

STATIC FUNCTION netiosrv_clientinfo()
   LOCAL hInfo := { => }

   hb_hKeepOrder( hInfo, .T. )

   hInfo[ "OS()"          ] := OS()
   hInfo[ "Version()"     ] := Version()
   hInfo[ "hb_Compiler()" ] := hb_Compiler()
   hInfo[ "NetName()"     ] := NetName()
   hInfo[ "hb_UserName()" ] := hb_UserName()

   RETURN hInfo

PROCEDURE netiosrv_cmdUI( cIP, nPort, cPassword )
   LOCAL GetList := {}
   LOCAL hCommands
   LOCAL nSavedRow
   LOCAL nPos
   LOCAL aCommand
   LOCAL cCommand

   LOCAL bKeyDown
   LOCAL bKeyUp
   LOCAL bKeyIns
   LOCAL bKeyPaste
   LOCAL bKeyTab

   LOCAL aHistory, nHistIndex

   LOCAL lQuit

   LOCAL pConnection

   IF ! Empty( cPassword )
      pConnection := ConnectLow( cIP, nPort, cPassword )
      QQOut( hb_eol() )
   ENDIF

   QQOut( "Type a command or '?' for help.", hb_eol() )

   aHistory   := { "quit" }
   nHistIndex := Len( aHistory ) + 1

   hCommands  := { ;
      "?"          => { ""               , "Synonym for 'help'."                   , {|| cmdHelp( hCommands ) } },;
      "exit"       => { ""               , "Exit console."                         , {|| lQuit := .T. } },;
      "clear"      => { ""               , "Clear screen."                         , {|| Scroll(), SetPos( 0, 0 ) } },;
      "connect"    => { "[<ip[:port>]]"  , "Connect."                              , {| cCommand | cmdConnect( cCommand, @pConnection, @cIP, @nPort ) } },;
      "disconnect" => { ""               , "Disconnect."                           , {|| cmdDisconnect( @pConnection ) } },;
      "sysinfo"    => { ""               , "Show system/build information."        , {|| cmdSysInfo( pConnection ) } },;
      "show"       => { ""               , "Show list of connections."             , {|| cmdConnInfo( pConnection, .F. ) } },;
      "showadmin"  => { ""               , "Show list of management connections."  , {|| cmdConnInfo( pConnection, .T. ) } },;
      "noconn"     => { ""               , "Disable incoming connections."         , {|| cmdConnEnable( pConnection, .F. ) } },;
      "conn"       => { ""               , "Enable incoming connections."          , {|| cmdConnEnable( pConnection, .T. ) } },;
      "nologconn"  => { ""               , "Disable logging incoming connections." , {|| cmdConnLogEnable( pConnection, .F. ) } },;
      "logconn"    => { ""               , "Enable logging incoming connections."  , {|| cmdConnLogEnable( pConnection, .T. ) } },;
      "stop"       => { "[<ip:port>|all]", "Stop specified connection(s)."         , {| cCommand | cmdConnStop( pConnection, cCommand ) } },;
      "clientinfo" => { "[<ip:port>"     , "Show client details."                  , {| cCommand | cmdConnClientInfo( pConnection, cCommand ) } },;
      "quit"       => { ""               , "Stop server and exit console."         , {|| cmdShutdown( pConnection ), lQuit := .T. } },;
      "help"       => { ""               , "Display this help."                    , {|| cmdHelp( hCommands ) } } }

   lQuit := .F.

   DO WHILE ! lQuit

      IF ! Empty( pConnection )
         /* Is connection alive? */
         BEGIN SEQUENCE WITH {| oError | Break( oError ) }
            netio_funcexec( pConnection, "hbnetiomgm_ping" )
         RECOVER
            QQOut( "Connection lost.", hb_eol() )
            EXIT
         END SEQUENCE
      ENDIF

      cCommand := Space( 128 )

      IF Empty( pConnection )
         QQOut( "hbnetiosrv$ " )
      ELSE
         QQOut( "hbnetiosrv://" + cIP + ":" + hb_ntos( nPort ) + "$ " )
      ENDIF
      nSavedRow := Row()

      @ nSavedRow, Col() GET cCommand PICTURE "@S" + hb_ntos( MaxCol() - Col() + 1 ) COLOR hb_ColorIndex( SetColor(), CLR_STANDARD ) + "," + hb_ColorIndex( SetColor(), CLR_STANDARD )

      SetCursor( iif( ReadInsert(), SC_INSERT, SC_NORMAL ) )

      bKeyIns   := SetKey( K_INS,;
         {|| SetCursor( iif( ReadInsert( ! ReadInsert() ),;
                          SC_NORMAL, SC_INSERT ) ) } )
      bKeyUp    := SetKey( K_UP,;
         {|| iif( nHistIndex > 1,;
                  cCommand := PadR( aHistory[ --nHistIndex ], Len( cCommand ) ), ),;
                  ManageCursor( cCommand ) } )
      bKeyDown  := SetKey( K_DOWN,;
         {|| cCommand := PadR( iif( nHistIndex < Len( aHistory ),;
             aHistory[ ++nHistIndex ],;
             ( nHistIndex := Len( aHistory ) + 1, "" ) ), Len( cCommand ) ),;
                  ManageCursor( cCommand ) } )
      bKeyPaste := SetKey( K_ALT_V, {|| hb_gtInfo( HB_GTI_CLIPBOARDPASTE )})

      bKeyTab   := SetKey( K_TAB, {|| CompleteCmd( @cCommand, hCommands ) } )

      READ

      /* Positions the cursor on the line previously saved */
      SetPos( nSavedRow, MaxCol() - 1 )

      SetKey( K_ALT_V, bKeyPaste )
      SetKey( K_DOWN,  bKeyDown  )
      SetKey( K_UP,    bKeyUp    )
      SetKey( K_INS,   bKeyIns   )
      SetKey( K_TAB,   bKeyTab   )

      QQOut( hb_eol() )

      cCommand := AllTrim( cCommand )

      IF Empty( cCommand )
         LOOP
      ENDIF

      IF Empty( aHistory ) .OR. ! ATail( aHistory ) == cCommand
         IF Len( aHistory ) < 64
            AAdd( aHistory, cCommand )
         ELSE
            ADel( aHistory, 1 )
            aHistory[ Len( aHistory ) ] := cCommand
         ENDIF
      ENDIF
      nHistIndex := Len( aHistory ) + 1

      aCommand := hb_ATokens( cCommand, " " )
      IF ! Empty( aCommand ) .AND. ( nPos := hb_HPos( hCommands, Lower( aCommand[ 1 ] ) ) ) > 0
         Eval( hb_HValueAt( hCommands, nPos )[ 3 ], cCommand )
      ELSE
         QQOut( "Error: Unknown command '" + cCommand + "'.", hb_eol() )
      ENDIF
   ENDDO

   IF ! Empty( pConnection )

      pConnection := NIL

      IF lQuit
         QQOut( "Connection closed.", hb_eol() )
      ENDIF
   ENDIF

   RETURN

/* connect to the server */
STATIC FUNCTION ConnectLow( cIP, nPort, cPassword )
   LOCAL pConnection

   QQOut( hb_StrFormat( "Connecting to hbnetio server management at %1$s:%2$d...", cIP, nPort ), hb_eol() )

   pConnection := netio_getconnection( cIP, nPort,, cPassword )
   cPassword := NIL

   IF ! Empty( pConnection )

      netio_funcexec( pConnection, "hbnetiomgm_setclientinfo", netiosrv_clientinfo() )
      netio_OpenItemStream( pConnection, "hbnetiomgm_cargo", "netiocui" )

      QQOut( "Connected.", hb_eol() )
   ELSE
      QQOut( "Error connecting server.", hb_eol() )
   ENDIF

   RETURN pConnection

STATIC FUNCTION GetPassword()
   LOCAL GetList := {}
   LOCAL cPassword := Space( 128 )
   LOCAL nSavedRow
   LOCAL bKeyPaste

   QQOut( "Enter password: " )

   nSavedRow := Row()

   AAdd( GetList, hb_Get():New( Row(), Col(), {| v | iif( PCount() == 0, cPassword, cPassword := v ) }, "cPassword", "@S" + hb_ntos( MaxCol() - Col() + 1 ), hb_ColorIndex( SetColor(), CLR_STANDARD ) + "," + hb_ColorIndex( SetColor(), CLR_STANDARD ) ) )
   ATail( GetList ):hideInput( .T. )
   ATail( GetList ):postBlock := {|| ! Empty( cPassword ) }
   ATail( GetList ):display()

   SetCursor( iif( ReadInsert(), SC_INSERT, SC_NORMAL ) )
   bKeyPaste := SetKey( K_ALT_V, {|| hb_gtInfo( HB_GTI_CLIPBOARDPASTE )})

   READ

   /* Positions the cursor on the line previously saved */
   SetPos( nSavedRow, MaxCol() - 1 )
   SetKey( K_ALT_V, bKeyPaste )

   QQOut( hb_eol() )

   RETURN AllTrim( cPassword )

/* Adjusted the positioning of cursor on navigate through history. [vailtom] */
STATIC PROCEDURE ManageCursor( cCommand )
   KEYBOARD Chr( K_HOME ) + iif( ! Empty( cCommand ), Chr( K_END ), "" )
   RETURN

/* Complete the command line, based on the first characters that the user typed. [vailtom] */
STATIC PROCEDURE CompleteCmd( cCommand, hCommands )
   LOCAL s := Lower( AllTrim( cCommand ) )
   LOCAL n

   /* We need at least one character to search */
   IF Len( s ) > 1
      FOR EACH n IN hCommands
         IF s == Lower( Left( n:__enumKey(), Len( s ) ) )
            cCommand := PadR( n:__enumKey(), Len( cCommand ) )
            ManageCursor( cCommand )
            RETURN
         ENDIF
      NEXT
   ENDIF

   RETURN

STATIC PROCEDURE IPPortSplit( cAddr, /* @ */ cIP, /* @ */ nPort )
   LOCAL tmp

   IF ! Empty( cAddr )
      cIP := cAddr
      IF ( tmp := At( ":", cIP ) ) > 0
         nPort := Val( SubStr( cIP, tmp + Len( ":" ) ) )
         cIP := Left( cIP, tmp - 1 )
      ELSE
         nPort := NIL
      ENDIF
   ENDIF

   RETURN

STATIC FUNCTION XToStrX( xValue )
   LOCAL cType := ValType( xValue )

   LOCAL tmp
   LOCAL cRetVal

   SWITCH cType
   CASE "C"

      xValue := StrTran( xValue, Chr(  0 ), '" + Chr(  0 ) + "' )
      xValue := StrTran( xValue, Chr(  9 ), '" + Chr(  9 ) + "' )
      xValue := StrTran( xValue, Chr( 10 ), '" + Chr( 10 ) + "' )
      xValue := StrTran( xValue, Chr( 13 ), '" + Chr( 13 ) + "' )
      xValue := StrTran( xValue, Chr( 26 ), '" + Chr( 26 ) + "' )

      RETURN xValue

   CASE "N" ; RETURN hb_ntos( xValue )
   CASE "D" ; RETURN DToC( xValue )
   CASE "T" ; RETURN hb_TToC( xValue )
   CASE "L" ; RETURN iif( xValue, ".T.", ".F." )
   CASE "O" ; RETURN xValue:className() + " Object"
   CASE "U" ; RETURN "NIL"
   CASE "B" ; RETURN '{||...} -> ' + XToStrX( Eval( xValue ) )
   CASE "A"

      cRetVal := '{ '

      FOR EACH tmp IN xValue
         cRetVal += XToStrX( tmp )
         IF tmp:__enumIndex() < Len( tmp:__enumBase() )
            cRetVal += ", "
         ENDIF
      NEXT

      RETURN cRetVal + ' }'

   CASE "H"

      cRetVal := '{ '

      FOR EACH tmp IN xValue
         cRetVal += tmp:__enumKey() + " => " + XToStrX( tmp )
         IF tmp:__enumIndex() < Len( tmp:__enumBase() )
            cRetVal += ", "
         ENDIF
      NEXT

      RETURN cRetVal + ' }'

   CASE "M" ; RETURN 'M:' + xValue
   ENDSWITCH

   RETURN ""

/* Commands */

STATIC PROCEDURE cmdConnect( cCommand, /* @ */ pConnection, /* @ */ cIP, /* @ */ nPort )
   LOCAL aToken
   LOCAL cPassword

   IF Empty( pConnection )

      aToken := hb_ATokens( cCommand, " " )

      IF Len( aToken ) >= 2
         IPPortSplit( aToken[ 2 ], @cIP, @nPort )
         IF Empty( nPort )
            nPort := _NETIOMGM_PORT_DEF
         ENDIF
      ENDIF
      IF Len( aToken ) >= 3
         cPassword := aToken[ 3 ]
      ELSE
         cPassword := GetPassword()
      ENDIF

      pConnection := ConnectLow( cIP, nPort, cPassword )
   ELSE
      QQOut( "Already connected. Disconnect first.", hb_eol() )
   ENDIF

   RETURN

STATIC PROCEDURE cmdDisconnect( /* @ */ pConnection )

   IF Empty( pConnection )
      QQOut( "Not connected.", hb_eol() )
   ELSE
      pConnection := NIL
   ENDIF

   RETURN

STATIC PROCEDURE cmdSysInfo( pConnection )
   LOCAL cLine

   IF Empty( pConnection )
      QQOut( "Not connected.", hb_eol() )
   ELSE
      FOR EACH cLine IN netio_funcexec( pConnection, "hbnetiomgm_sysinfo" )
         QQOut( cLine, hb_eol() )
      NEXT
   ENDIF

   RETURN

STATIC PROCEDURE cmdConnStop( pConnection, cCommand )
   LOCAL aToken

   IF Empty( pConnection )
      QQOut( "Not connected.", hb_eol() )
   ELSE
      aToken := hb_ATokens( cCommand, " " )
      IF Len( aToken ) > 1
         netio_funcexec( pConnection, "hbnetiomgm_stop", aToken[ 2 ] )
      ELSE
         QQOut( "Error: Invalid syntax.", hb_eol() )
      ENDIF
   ENDIF

   RETURN

STATIC PROCEDURE cmdConnClientInfo( pConnection, cCommand )
   LOCAL aToken
   LOCAL xCargo

   IF Empty( pConnection )
      QQOut( "Not connected.", hb_eol() )
   ELSE
      aToken := hb_ATokens( cCommand, " " )
      IF Len( aToken ) > 1
         xCargo := netio_funcexec( pConnection, "hbnetiomgm_clientinfo", aToken[ 2 ] )
         IF xCargo == NIL
            QQOut( "No information", hb_eol() )
         ELSE
            QQOut( XToStrX( xCargo ), hb_eol() )
         ENDIF
      ELSE
         QQOut( "Error: Invalid syntax.", hb_eol() )
      ENDIF
   ENDIF

   RETURN

STATIC PROCEDURE cmdConnInfo( pConnection, lManagement )
   LOCAL aArray
   LOCAL hConn

   IF Empty( pConnection )
      QQOut( "Not connected.", hb_eol() )
   ELSE
      aArray := netio_funcexec( pConnection, iif( lManagement, "hbnetiomgm_adminfo", "hbnetiomgm_conninfo" ) )

      QQOut( "Number of connections: " + hb_ntos( Len( aArray ) ), hb_eol() )

      FOR EACH hConn IN aArray
         QQOut( "#" + PadR( hb_ntos( hConn[ "nThreadID" ] ), Len( Str( hConn[ "nThreadID" ] ) ) ) + " " +;
                hb_TToC( hConn[ "tStart" ], "YYYY.MM.DD", "HH:MM:SS" ) + " " +;
                PadR( hConn[ "cStatus" ], 12 ) + " " +;
                "fcnt: " + Str( hConn[ "nFilesCount" ] ) + " " +;
                "send: " + Str( hConn[ "nBytesSent" ] ) + " " +;
                "recv: " + Str( hConn[ "nBytesReceived" ] ) + " " +;
                hConn[ "cAddressPeer" ] + " " +;
                iif( "xCargo" $ hconn, hb_ValToStr( hConn[ "xCargo" ] ), "" ), hb_eol() )
      NEXT
   ENDIF

   RETURN

STATIC PROCEDURE cmdShutdown( pConnection )

   IF Empty( pConnection )
      QQOut( "Not connected.", hb_eol() )
   ELSE
      netio_funcexec( pConnection, "hbnetiomgm_shutdown" )
   ENDIF

   RETURN

STATIC PROCEDURE cmdConnEnable( pConnection, lValue )

   IF Empty( pConnection )
      QQOut( "Not connected.", hb_eol() )
   ELSE
      netio_funcexec( pConnection, "hbnetiomgm_conn", lValue )
   ENDIF

   RETURN

STATIC PROCEDURE cmdConnLogEnable( pConnection, lValue )

   IF Empty( pConnection )
      QQOut( "Not connected.", hb_eol() )
   ELSE
      netio_funcexec( pConnection, "hbnetiomgm_logconn", lValue )
   ENDIF

   RETURN
