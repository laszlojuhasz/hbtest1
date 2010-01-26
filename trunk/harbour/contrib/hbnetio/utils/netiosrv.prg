/*
 * $Id$
 */

/*
 * Harbour Project source code:
 *    demonstration/test code for alternative RDD IO API which uses own
 *    very simple TCP/IP file server.
 *
 * Copyright 2009 Przemyslaw Czerpak <druzus / at / priv.onet.pl>
 * www - http://www.harbour-project.org
 *
 */

/*
 * The following parts are Copyright of the individual authors.
 * www - http://www.harbour-project.org
 *
 * Copyright 2010 Viktor Szakats (harbour.01 syenar.hu)
 *    ...
 *
 * See COPYING for licensing terms.
 *
 */

#include "hbhrb.ch"

#include "color.ch"
#include "hbgtinfo.ch"
#include "inkey.ch"
#include "setcurs.ch"

/* netio_mtserver() needs MT HVM version */
REQUEST HB_MT

#define _RPC_FILTER "HBNETIOSRV_RPCMAIN"

/* enable this if you need all core functions in RPC support */
#ifdef HB_EXTERN
   REQUEST __HB_EXTERN__
#endif

#define _NETIOSRV_nPort             1
#define _NETIOSRV_cIFAddr           2
#define _NETIOSRV_cRootDir          3
#define _NETIOSRV_lRPC              4
#define _NETIOSRV_cRPCFFileName     5
#define _NETIOSRV_cRPCFHRB          6
#define _NETIOSRV_lEncryption       7
#define _NETIOSRV_pListenSocket     8
#define _NETIOSRV_MAX_              8

PROCEDURE Main( ... )
   LOCAL netiosrv[ _NETIOSRV_MAX_ ]

   LOCAL cParam
   LOCAL cCommand
   LOCAL cPassword

   LOCAL bKeyDown
   LOCAL bKeyUp
   LOCAL bKeyIns
   LOCAL bKeyPaste
   LOCAL bKeyTab

   LOCAL GetList   := {}
   LOCAL lQuit     := .F.
   LOCAL hCommands
   LOCAL nSavedRow
   LOCAL nPos
   LOCAL aCmd

   LOCAL aHistory, nHistIndex

   HB_Logo()

   netiosrv[ _NETIOSRV_nPort ]         := 2941
   netiosrv[ _NETIOSRV_cIFAddr ]       := "0.0.0.0"
   netiosrv[ _NETIOSRV_cRootDir ]      := hb_dirBase()
   netiosrv[ _NETIOSRV_lRPC ]          := .F.
   netiosrv[ _NETIOSRV_lEncryption ]   := .F.

   FOR EACH cParam IN { ... }
      DO CASE
      CASE Lower( Left( cParam, 6 ) ) == "-port="
         netiosrv[ _NETIOSRV_nPort ] := Val( SubStr( cParam, 7 ) )
      CASE Lower( Left( cParam, 7 ) ) == "-iface="
         netiosrv[ _NETIOSRV_cIFAddr ] := SubStr( cParam, 8 )
      CASE Lower( Left( cParam, 9 ) ) == "-rootdir="
         netiosrv[ _NETIOSRV_cRootDir ] := SubStr( cParam, 10 )
      CASE Lower( Left( cParam, 6 ) ) == "-pass="
         cPassword := SubStr( cParam, 7 )
         hb_StrClear( @cParam )
      CASE Lower( Left( cParam, 5 ) ) == "-rpc="
         netiosrv[ _NETIOSRV_cRPCFFileName ] := SubStr( cParam, 6 )
         netiosrv[ _NETIOSRV_cRPCFHRB ] := hb_hrbLoad( netiosrv[ _NETIOSRV_cRPCFFileName ] )
         netiosrv[ _NETIOSRV_lRPC ] := ! Empty( netiosrv[ _NETIOSRV_cRPCFHRB ] ) .AND. ! Empty( hb_hrbGetFunSym( netiosrv[ _NETIOSRV_cRPCFHRB ], _RPC_FILTER ) )
         IF ! netiosrv[ _NETIOSRV_lRPC ]
            netiosrv[ _NETIOSRV_cRPCFFileName ] := NIL
            netiosrv[ _NETIOSRV_cRPCFHRB ] := NIL
         ENDIF
      CASE Lower( cParam ) == "-rpc"
         netiosrv[ _NETIOSRV_lRPC ] := .T.
      CASE Lower( cParam ) == "--version"
         RETURN
      CASE Lower( cParam ) == "-help" .OR. ;
           Lower( cParam ) == "--help"
         HB_Usage()
         RETURN
      OTHERWISE
         OutStd( "Warning: Unkown parameter ignored: " + cParam + hb_osNewLine() )
      ENDCASE
   NEXT

   SetCancel( .F. )

   netiosrv[ _NETIOSRV_pListenSocket ] := ;
      netio_mtserver( netiosrv[ _NETIOSRV_nPort ],;
                      netiosrv[ _NETIOSRV_cIFAddr ],;
                      netiosrv[ _NETIOSRV_cRootDir ],;
                      iif( Empty( netiosrv[ _NETIOSRV_cRPCFHRB ] ), netiosrv[ _NETIOSRV_lRPC ], hb_hrbGetFunSym( netiosrv[ _NETIOSRV_cRPCFHRB ], _RPC_FILTER ) ),;
                      @cPassword )

   netiosrv[ _NETIOSRV_lEncryption ] := ! Empty( cPassword )
   cPassword := NIL

   IF Empty( netiosrv[ _NETIOSRV_pListenSocket ] )
      OutStd( "Cannot start server." + hb_osNewLine() )
   ELSE
      ShowConfig( netiosrv )

      OutStd( hb_osNewLine() )
      OutStd( "Type a command or '?' for help.", hb_osNewLine() )

      lQuit      := .F.
      aHistory   := { "quit" }
      nHistIndex := Len( aHistory ) + 1
      hCommands  := hbnetiosrv_LoadCmds( {|| lQuit := .T. },;            /* codeblock to quit */
                                         {|| ShowConfig( netiosrv ) } )  /* codeblock to display config both uses local vars */

      /* Command prompt */
      DO WHILE ! lQuit

         cCommand := Space( 128 )

         QQOut( "hbnetiosrv$ " )
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

         QQOut( hb_osNewLine() )

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

         nPos := iif( Empty( cCommand ), 0, hb_HPos( hCommands, Lower( cCommand ) ) )
         IF nPos > 0
            aCmd := hb_HValueAt( hCommands, nPos )
            Eval( aCmd[ 2 ], cCommand, netiosrv )
         ELSE
            QQOut( "Error: Unknown command '" + cCommand + "'.", hb_osNewLine() )
         ENDIF
      ENDDO

      netio_serverstop( netiosrv[ _NETIOSRV_pListenSocket ] )
      netiosrv[ _NETIOSRV_pListenSocket ] := NIL

      OutStd( hb_osNewLine() )
      OutStd( "Server stopped.", hb_osNewLine() )
   ENDIF

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

/* Adjusted the positioning of cursor on navigate through history. [vailtom] */
STATIC PROCEDURE ManageCursor( cCommand )
   KEYBOARD Chr( K_HOME ) + iif( ! Empty( cCommand ), Chr( K_END ), "" )
   RETURN

STATIC PROCEDURE ShowConfig( netiosrv )

   QQOut( "Listening on: "      + netiosrv[ _NETIOSRV_cIFAddr ] + ":" + hb_ntos( netiosrv[ _NETIOSRV_nPort ] ), hb_osNewLine() )
   QQOut( "Root filesystem: "   + netiosrv[ _NETIOSRV_cRootDir ], hb_osNewLine() )
   QQOut( "RPC support: "       + iif( netiosrv[ _NETIOSRV_lRPC ], "enabled", "disabled" ), hb_osNewLine() )
   QQOut( "Encryption: "        + iif( netiosrv[ _NETIOSRV_lEncryption ], "enabled", "disabled" ), hb_osNewLine() )
   QQOut( "RPC filter module: " + iif( Empty( netiosrv[ _NETIOSRV_cRPCFHRB ] ), iif( netiosrv[ _NETIOSRV_lRPC ], "not set (WARNING: unsafe open server)", "not set" ), netiosrv[ _NETIOSRV_cRPCFFileName ] ), hb_osNewLine() )

   RETURN

STATIC PROCEDURE HB_Logo()

   OutStd( "Harbour NETIO Server " + HBRawVersion() + hb_osNewLine() +;
           "Copyright (c) 2009, Przemyslaw Czerpak" + hb_osNewLine() + ;
           "http://www.harbour-project.org/" + hb_osNewLine() +;
           hb_osNewLine() )

   RETURN

STATIC PROCEDURE HB_Usage()

   OutStd(               "Syntax:"                                                                                   , hb_osNewLine() )
   OutStd(                                                                                                             hb_osNewLine() )
   OutStd(               "  netiosrv [options]"                                                                      , hb_osNewLine() )
   OutStd(                                                                                                             hb_osNewLine() )
   OutStd(               "Options:"                                                                                  , hb_osNewLine() )
   OutStd(                                                                                                             hb_osNewLine() )
   OutStd(               "  -port=<port>        accept incoming connections on IP port <port>"                       , hb_osNewLine() )
   OutStd(               "  -iface=<ipaddr>     accept incoming connections on IPv4 interface <ipaddress>"           , hb_osNewLine() )
   OutStd(               "  -rootdir=<rootdir>  use <rootdir> as root directory for served file system"              , hb_osNewLine() )
   OutStd(               "  -rpc                accept RPC requests"                                                 , hb_osNewLine() )
   OutStd(               "  -rpc=<file.hrb>     set RPC processor .hrb module to <file.hrb>"                         , hb_osNewLine() )
   OutStd( hb_StrFormat( "                      file.hrb needs to have an entry function named %1$s()", _RPC_FILTER ), hb_osNewLine() )
   OutStd(               "  -pass=<passwd>      set server password"                                                 , hb_osNewLine() )
   OutStd(                                                                                                             hb_osNewLine() )
   OutStd(               "  --version           display version header only"                                         , hb_osNewLine() )
   OutStd(               "  -help|--help        this help"                                                           , hb_osNewLine() )

   RETURN

STATIC FUNCTION HBRawVersion()
   RETURN StrTran( Version(), "Harbour " )
