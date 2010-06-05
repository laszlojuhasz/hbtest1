/*
 * $Id$
 */

/*
 * Copyright 2009 Viktor Szakats (harbour.01 syenar.hu)
 * See COPYING for licensing terms.
 */

#pragma warninglevel=3

/* TOFIX: Ugly hack to avoid #include "directry.ch" */
#define F_NAME          1       /* File name */
#define F_ATTR          5       /* File attribute */

#define _PS_            hb_osPathSeparator()

PROCEDURE Main()
   LOCAL nErrorLevel := 0
   LOCAL cFile
   LOCAL aFile

   LOCAL tmp
   LOCAL cOptions
   LOCAL cOldDir

   IF Empty( GetEnv( "HB_PLATFORM" ) ) .OR. ;
      Empty( GetEnv( "HB_COMPILER" ) ) .OR. ;
      Empty( GetEnv( "HB_BIN_INSTALL" ) ) .OR. ;
      Empty( GetEnv( "HB_LIB_INSTALL" ) ) .OR. ;
      Empty( GetEnv( "HB_INC_INSTALL" ) )

      OutStd( "! Error: This program has to be called from the GNU Make process." + hb_osNewLine() )
      ErrorLevel( 1 )
      RETURN
   ENDIF

   /* Creating hbmk.cfg */

   OutStd( "! Making " + GetEnv( "HB_BIN_INSTALL" ) + _PS_ + "hbmk.cfg..." + hb_osNewLine() )

   cFile := ""
   cFile += "# hbmk2 configuration" + hb_osNewLine()
   cFile += "# Generated by Harbour build process" + hb_osNewLine()
   cFile += hb_osNewLine()
   cFile += "libpaths=../contrib/%{hb_name}" + hb_osNewLine()
   cFile += "libpaths=../addons/%{hb_name}" + hb_osNewLine()
   cFile += "libpaths=../examples/%{hb_name}" + hb_osNewLine()

   IF GetEnv( "HB_PLATFORM" ) == "dos" .AND. ;
      ! Empty( GetEnv( "HB_HAS_WATT" ) )
      cFile += hb_osNewLine()
      cFile += "{dos&djgpp}syslibs=watt" + hb_osNewLine()
      cFile += "{dos&watcom}syslibs=wattcpwf" + hb_osNewLine()
      cFile += "{dos}libpaths=${WATT_ROOT}/lib" + hb_osNewLine()
   ENDIF

   IF ! Empty( GetEnv( "HB_HAS_GPM" ) )
      cFile += hb_osNewLine()
      cFile += "{" + GetEnv( "HB_PLATFORM" ) + "&" + GetEnv( "HB_COMPILER" ) + "}syslibs=gpm" + hb_osNewLine()
   ENDIF

   hb_MemoWrit( GetEnv( "HB_BIN_INSTALL" ) + _PS_ + "hbmk.cfg", cFile )

   /* Installing some misc files */

   IF GetEnv( "HB_PLATFORM" ) $ "win|wce|os2|dos" .AND. ;
      ! Empty( GetEnv( "HB_INSTALL_PREFIX" ) )

      FOR EACH aFile IN Directory( "Change*" )
         hb_FCopy( aFile[ F_NAME ], GetEnv( "HB_INSTALL_PREFIX" ) + _PS_ + iif( GetEnv( "HB_PLATFORM" ) == "dos", "CHANGES", aFile[ F_NAME ] ) )
      NEXT

      hb_FCopy( "COPYING", GetEnv( "HB_INSTALL_PREFIX" ) + _PS_ + "COPYING" )
      hb_FCopy( "INSTALL", GetEnv( "HB_INSTALL_PREFIX" ) + _PS_ + "INSTALL" )
      hb_FCopy( "TODO"   , GetEnv( "HB_INSTALL_PREFIX" ) + _PS_ + "TODO" )
   ENDIF

   /* Import library generation */

   IF GetEnv( "HB_PLATFORM" ) $ "win|wce|os2" .AND. ;
      GetEnv( "HB_BUILD_IMPLIB" ) == "yes" .AND. ;
      ! Empty( GetEnv( "HB_HOST_BIN_DIR" ) )

      FOR EACH tmp IN PackageList( "contrib" + _PS_ + hb_osFileMask(), GetEnv( "HB_CONTRIBLIBS" ), GetEnv( "HB_CONTRIB_ADDONS" ) )
         IF hb_FileExists( "contrib" + _PS_ + tmp + _PS_ + tmp + ".hbi" )
            mk_hb_processRun( GetEnv( "HB_HOST_BIN_DIR" ) + _PS_ + "hbmk2" +;
                              " -quiet -lang=en" +;
                              " @contrib/" + tmp + "/" + tmp + ".hbi" +;
                              " -o${HB_LIB_INSTALL}/" )
         ENDIF
      NEXT
   ENDIF

   /* Converting build options to hbmk2 options */

   cOptions := ""
   IF GetEnv( "HB_BUILD_MODE" ) == "cpp"
      cOptions += " -cpp=yes"
   ELSEIF GetEnv( "HB_BUILD_MODE" ) == "c"
      cOptions += " -cpp=no"
   ENDIF
   IF GetEnv( "HB_BUILD_DEBUG" ) == "yes"
      cOptions += " -debug"
   ENDIF

   /* Creating extra binaries */

   IF ! Empty( GetEnv( "HB_BUILD_ADDONS" ) ) .AND. ;
      ! Empty( GetEnv( "HB_HOST_BIN_DIR" ) )

      OutStd( "! Making binaries for .hbp project addons..." + hb_osNewLine() )

      FOR EACH tmp IN hb_ATokens( GetEnv( "HB_BUILD_ADDONS" ),, .T. )
         IF ! Empty( tmp )
            mk_hb_processRun( GetEnv( "HB_HOST_BIN_DIR" ) + _PS_ + "hbmk2" +;
                              " -quiet -lang=en -q0" + cOptions +;
                              " " + Chr( 34 ) + StrTran( tmp, "\", "/" ) + Chr( 34 ) +;
                              " -o${HB_BIN_INSTALL}/" )
         ENDIF
      NEXT
   ENDIF

   /* Creating shared version of Harbour binaries */

   IF !( GetEnv( "HB_PLATFORM" ) $ "dos|linux" ) .AND. ;
      !( GetEnv( "HB_BUILD_DLL" ) == "no" ) .AND. ;
      !( GetEnv( "HB_BUILD_SHARED" ) == "yes" ) .AND. ;
      ! Empty( GetEnv( "HB_HOST_BIN_DIR" ) )

      OutStd( "! Making shared version of Harbour binaries..." + hb_osNewLine() )

      FOR EACH tmp IN Directory( "utils" + _PS_ + hb_osFileMask(), "D" )
         IF "D" $ tmp[ F_ATTR ] .AND. ;
            !( tmp[ F_NAME ] == "." ) .AND. ;
            !( tmp[ F_NAME ] == ".." ) .AND. ;
            hb_FileExists( "utils" + _PS_ + tmp[ F_NAME ] + _PS_ + tmp[ F_NAME ] + ".hbp" )

            mk_hb_processRun( GetEnv( "HB_HOST_BIN_DIR" ) + _PS_ + "hbmk2" +;
                              " -quiet -lang=en -q0 -shared" + cOptions +;
                              " utils/" + tmp[ F_NAME ] + "/" + tmp[ F_NAME ] + ".hbp" +;
                              " -o${HB_BIN_INSTALL}/" + tmp[ F_NAME ] + "-dll" )
         ENDIF
      NEXT
   ENDIF

   /* Creating install packages */

   IF GetEnv( "HB_PLATFORM" ) $ "win|wce|os2|dos" .AND. ;
      GetEnv( "HB_BUILD_PKG" ) == "yes" .AND. ;
      ! Empty( GetEnv( "HB_TOP" ) )

      tmp := GetEnv( "HB_TOP" ) + _PS_ + GetEnv( "HB_PKGNAME" ) + ".zip"

      OutStd( "! Making Harbour .zip install package: '" + tmp + "'" + hb_osNewLine() )

      FErase( tmp )

      /* NOTE: Believe it or not this is the official method to zip a different dir with subdirs
               without including the whole root path in filenames; you have to 'cd' into it.
               Even with zip 3.0. For this reason we need absolute path in HB_TOP. There is also
               no zip 2.x compatible way to force creation of a new .zip, so we have to delete it
               first to avoid mixing in an existing .zip file. [vszakats] */

      cOldDir := _PS_ + CurDir()
      DirChange( GetEnv( "HB_INSTALL_PREFIX" ) + _PS_ + ".." )

      mk_hb_processRun( GetEnv( "HB_DIR_ZIP" ) + "zip" +;
                        " -q -9 -X -r -o" +;
                        " " + FN_Escape( tmp ) +;
                        " . -i " + FN_Escape( GetEnv( "HB_PKGNAME" ) + _PS_ + "*" ) +;
                        " -x *.tds -x *.exp" )

      DirChange( cOldDir )

      IF GetEnv( "HB_PLATFORM" ) $ "win|wce"

         tmp := GetEnv( "HB_TOP" ) + _PS_ + GetEnv( "HB_PKGNAME" ) + ".exe"

         OutStd( "! Making Harbour .exe install package: '" + tmp + "'" + hb_osNewLine() )

         mk_hb_processRun( GetEnv( "HB_DIR_NSIS" ) + "makensis.exe" +;
                           " -V2" +;
                           " " + FN_Escape( StrTran( "package/mpkg_win.nsi", "/", _PS_ ) ) )
      ENDIF
   ENDIF

   ErrorLevel( nErrorLevel )

   RETURN

STATIC FUNCTION mk_hb_processRun( cCommand )

   OutStd( cCommand + hb_osNewLine() )

   RETURN hb_processRun( cCommand )

STATIC FUNCTION FN_Escape( cFN )
   RETURN Chr( 34 ) + cFN + Chr( 34 )

/* NOTE: Must be in sync with contrib/Makefile logic */
STATIC FUNCTION PackageList( cMask, cBase, cAddOn )
   LOCAL aList := {}
   LOCAL aBase
   LOCAL tmp

   aBase := iif( Empty( cBase ), {}, hb_ATokens( cBase,, .T. ) )

   DO CASE
   CASE Len( aBase ) == 1 .AND. aBase[ 1 ] == "no"
      /* fall through */
   CASE Len( aBase ) > 1 .AND. aBase[ 1 ] == "no"
      hb_ADel( aBase, 1, .T. )
      FOR EACH tmp IN Directory( cMask, "D" )
         IF "D" $ tmp[ F_ATTR ] .AND. !( tmp[ F_NAME ] == "." ) .AND. !( tmp[ F_NAME ] == ".." ) .AND. ;
            AScan( aBase, {| tmp1 | tmp1 == tmp[ F_NAME ] } ) == 0
            AAdd( aList, tmp[ F_NAME ] )
         ENDIF
      NEXT
   CASE Len( aBase ) > 0
      aList := hb_ATokens( cBase,, .T. )
   OTHERWISE
      FOR EACH tmp IN Directory( cMask, "D" )
         IF "D" $ tmp[ F_ATTR ] .AND. !( tmp[ F_NAME ] == "." ) .AND. !( tmp[ F_NAME ] == ".." )
            AAdd( aList, tmp[ F_NAME ] )
         ENDIF
      NEXT
   ENDCASE

   IF ! Empty( cAddOn )
      FOR EACH tmp IN hb_ATokens( cAddOn,, .T. )
         IF ! Empty( tmp )
            IF AScan( aList, {| tmp1 | tmp1 == tmp } ) == 0
               AAdd( aList, tmp )
            ENDIF
         ENDIF
      NEXT
   ENDIF

   RETURN aList
