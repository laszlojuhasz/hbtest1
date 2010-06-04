/*
 * $Id$
 */

/*
 * The following parts are Copyright of the individual authors.
 * www - http://harbour-project.org
 *
 * Copyright 2010 Viktor Szakats (harbour.01 syenar.hu)
 *    ...
 *
 * See COPYING for licensing terms.
 *
 */

FUNCTION hbnetiosrv_LoadCmds( bQuit, bShowInfo )
   LOCAL hCmds := { ;
                     "?"         => { "Synonym for 'help'."           , {|| cmdHelp( hCmds ) } },;
                     "clear"     => { "Clear screen."                 , {|| Scroll(), SetPos( 0, 0 ) } },;
                     "config"    => { "Show server configuration."    , bShowInfo },;
                     "sysinfo"   => { "Show system/build information.", {|| cmdSysInfo() } },;
                     "quit"      => { "Stop server and exit."         , bQuit },;
                     "help"      => { "Display this help."            , {|| cmdHelp( hCmds ) } };
                  }

   RETURN hCmds

/* TODO: - on the fly change of RPC filter modules
         - listing active connections
         - listing open files
         - listing active locks
         - activity meters (transferred bytes, bandwidth, etc)
         - showing number of connections
         - showing number of open files
         - listing transferred bytes
         - gracefully shutting down server by waiting for connections to close and not accept new ones
         - pausing server */

STATIC PROCEDURE cmdSysInfo()
   QQOut( "OS: "         + OS(), hb_osNewLine() )
   QQOut( "Harbour: "    + Version(), hb_osNewLine() )
   QQOut( "C Compiler: " + hb_Compiler(), hb_osNewLine() )
   QQOut( "Memory: "     + hb_ntos( Memory( 0 ) ) + "KB", hb_osNewLine() )
   RETURN

STATIC PROCEDURE cmdHelp( hCommands )
   LOCAL aTexts := {}
   LOCAL n, c, m

   m := 0
   hb_HEval( hCommands, {| k | m := Max( m, Len( k ) ) } )

   AAdd( aTexts, "Commands:" )

   /* Processing commands */
   FOR EACH n IN hCommands
      AAdd( aTexts, " " + PadR( n:__enumKey(), m ) + " - " + n[ 1 ] )
   NEXT

   ASort( aTexts, 2 )

   AAdd( aTexts, "" )
   AAdd( aTexts, "Keyboard shortcuts:" )
   AAdd( aTexts, PadR( " <Up>", m )    + "  - Move up on historic list." )
   AAdd( aTexts, PadR( " <Down>", m )  + "  - Move down on historic list." )
   AAdd( aTexts, PadR( " <Tab>", m )   + "  - Complete command." )
   AAdd( aTexts, PadR( " <Alt+V>", m ) + "  - Paste Clipboard contents (if apropriate)." )

   c := 0
   m := MaxRow()

   FOR EACH n IN aTexts
      QQOut( n, hb_osNewLine() )

      IF ++c == m
         c := 0
         QQOut( "Press any key to continue..." )
         Inkey( 0 )

         Scroll( Row(), 0, Row(), MaxCol(), 0 )
         SetPos( Row(), 0 )
      ENDIF
   NEXT

   RETURN
