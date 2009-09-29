/*
 * $Id$
 */

/*
 * Copyright 2009 Viktor Szakats (harbour.01 syenar.hu)
 * See COPYING for licensing terms.
 */

PROCEDURE Main()
   LOCAL nErrorLevel := 0
   LOCAL cFile

   IF Empty( GetEnv( "HB_PLATFORM" ) ) .OR. ;
      Empty( GetEnv( "HB_COMPILER" ) ) .OR. ;
      Empty( GetEnv( "HB_BIN_INSTALL" ) ) .OR. ;
      Empty( GetEnv( "HB_LIB_INSTALL" ) ) .OR. ;
      Empty( GetEnv( "HB_INC_INSTALL" ) )

      OutStd( "! Error: This program has to be called from the GNU Make process." + hb_osNewLine() )
      ErrorLevel( 1 )
      RETURN
   ENDIF

   OutStd( "! Making " + GetEnv( "HB_BIN_INSTALL" ) + hb_osPathSeparator() + "hbmk.cfg..." + hb_osNewLine() )

   cFile := ""
   cFile += "# hbmk2 configuration" + hb_osNewLine()
   cFile += "# Generated by Harbour build process" + hb_osNewLine()
   cFile += hb_osNewLine()
   cFile += "libpaths=../contrib/%{hb_name}" + hb_osNewLine()
   cFile += "libpaths=../contrib/rddsql/%{hb_name}" + hb_osNewLine()
   cFile += "libpaths=../addons/%{hb_name}" + hb_osNewLine()
   cFile += "libpaths=../examples/%{hb_name}" + hb_osNewLine()

   IF GetEnv( "HB_PLATFORM" ) == "dos" .AND. ;
      ! Empty( GetEnv( "HB_HAS_WATT" ) )
      cFile += hb_osNewLine()
      cFile += "{dos&djgpp}syslibs=watt" + hb_osNewLine()
      cFile += "{dos&watcom}syslibs=wattcpwf" + hb_osNewLine()
      cFile += "{dos}libpaths=${WATT_ROOT}/lib" + hb_osNewLine()
   ENDIF

   hb_MemoWrit( GetEnv( "HB_BIN_INSTALL" ) + hb_osPathSeparator() + "hbmk.cfg" )

   IF GetEnv( "HB_PLATFORM" ) $ "win|wce|os2|dos" .AND. ;
      ! Empty( GetEnv( "HB_INSTALL_PREFIX" ) )

      FOR EACH cFile IN Directory( "Change*" )
         hb_FCopy( cFile, GetEnv( "HB_INSTALL_PREFIX" ) + hb_osPathSeparator() + iif( GetEnv( "HB_PLATFORM" ) == "dos", "CHANGES", cFile ) )
      NEXT

      hb_FCopy( "COPYING", GetEnv( "HB_INSTALL_PREFIX" ) + hb_osPathSeparator() + "COPYING" )
      hb_FCopy( "INSTALL", GetEnv( "HB_INSTALL_PREFIX" ) + hb_osPathSeparator() + "INSTALL" )
      hb_FCopy( "TODO"   , GetEnv( "HB_INSTALL_PREFIX" ) + hb_osPathSeparator() + "TODO" )

      IF GetEnv( "HB_PLATFORM" ) $ "win|wce"
         hb_FCopy( "bin" + hb_osNewLine() + "hb-mkimp.bat", GetEnv( "HB_BIN_INSTALL" ) + hb_osPathSeparator() + "hb-mkimp.bat" )
      ENDIF
   ENDIF

   ErrorLevel( nErrorLevel )

   RETURN 
