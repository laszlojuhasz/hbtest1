/*
 * $Id$
 */

/*
 * Harbour Project source code:
 *    "DOt Prompt" Console and .prg/.hrb runner for the Harbour Language
 *
 * Copyright 2007 Przemyslaw Czerpak <druzus / at / priv.onet.pl>
 * Copyright 2008-2011 Viktor Szakats (harbour.01 syenar.hu)
 * www - http://harbour-project.org
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

#include "common.ch"
#include "fileio.ch"
#include "inkey.ch"
#include "setcurs.ch"

#include "hbgtinfo.ch"

/* NOTE: use hbextern library instead of #include "hbextern.ch"
 *       in dynamic builds it will greatly reduce the size because
 *       all function symbols will be registered by harbour shared
 *       library (.dll, .so, .sl, .dyn, ...) not by this code
 */

REQUEST __HB_EXTERN__

REQUEST HB_GT_CGI
REQUEST HB_GT_PCA
REQUEST HB_GT_STD


/* command to store header files in hash array */
#command ADD HEADER TO <hash> FILE <(cFile)> => ;
         #pragma __streaminclude <(cFile)>|<hash>\[ <(cFile)> \] := %s


#define HB_HISTORY_LEN 500
#define HB_LINE_LEN    256
#define HB_PROMPT      "."

STATIC s_nRow
STATIC s_nCol := 0
STATIC s_aHistory := {}
STATIC s_lPreserveHistory := .T.
STATIC s_lWasLoad := .F.

STATIC s_cDirBase
STATIC s_cProgName

/* ********************************************************************** */

PROCEDURE _APPMAIN( cFile, ... )
   LOCAL cExt
   LOCAL hHeaders

   IF PCount() > 0
      SWITCH Lower( cFile )
         CASE "-?"
         CASE "-h"
         CASE "--help"
         CASE "/?"
         CASE "/h"
            hbrun_Usage()
            EXIT
         CASE "-v"
         CASE "/v"
            hbrun_Prompt( "? hb_version()" )
            EXIT
#if defined( __PLATFORM__WINDOWS )
         CASE "-r"
         CASE "-ra"
         CASE "/r"
         CASE "/ra"
            IF win_reg( .T., Right( Lower( cFile ), 1 ) == "a" )
               OutStd( "hbrun: Harbour Script File registered" + hb_eol() )
            ELSE
               OutErr( "hbrun: Error: Registering Harbour Script File" + hb_eol() )
            ENDIF
            EXIT
         CASE "-u"
         CASE "-ua"
         CASE "/u"
         CASE "/ua"
            IF win_reg( .F., Right( Lower( cFile ), 1 ) == "a" )
               OutStd( "hbrun: Harbour Script File unregistered" + hb_eol() )
            ELSE
               OutErr( "hbrun: Error: Unregistering Harbour Script File" + hb_eol() )
            ENDIF
            EXIT
#endif
         CASE "-p"
         CASE "/p"
            s_lPreserveHistory := .F.
            hbrun_Prompt()
            EXIT
         OTHERWISE
            cFile := hbrun_FindInPath( cFile )
            IF ! Empty( cFile )
               hb_FNameSplit( cFile, NIL, NIL, @cExt )
               cExt := Lower( cExt )
               SWITCH cExt
                  CASE ".prg"
                  CASE ".hbs"
                  CASE ".hrb"
                  CASE ".dbf"
                     EXIT
                  OTHERWISE
                     cExt := hbrun_FileSig( cFile )
               ENDSWITCH
               SWITCH cExt
                  CASE ".dbf"
                     hbrun_Prompt( "USE " + cFile + " SHARED" )
                     EXIT
                  CASE ".prg"
                  CASE ".hbs"
                     IF Empty( getenv( "HBRUN_NOHEAD" ) )
                        hHeaders := hbrun_CoreHeaderFiles() /* add core header files */
                     ENDIF

                     cFile := HB_COMPILEBUF( hHeaders, hb_ProgName(), "-n2", "-w", "-es2", "-q0", ;
                                             "-I" + hb_FNameDir( cFile ), "-D" + "__HBSCRIPT__HBRUN", cFile )
                     IF cFile == NIL
                        ERRORLEVEL( 1 )
                        EXIT
                     ENDIF
                  OTHERWISE
                     s_cDirBase := hb_DirBase()
                     s_cProgName := hb_ProgName()
                     hb_argShift( .T. )
                     hb_hrbRun( cFile, ... )
                     EXIT
               ENDSWITCH
            ENDIF
      ENDSWITCH
   ELSE
      hbrun_Prompt()
   ENDIF

   RETURN

STATIC FUNCTION hbrun_CoreHeaderFiles()
   LOCAL hHeaders

#ifdef HBRUN_WITH_HEADERS

   hHeaders := { => }

   ADD HEADER TO hHeaders FILE "achoice.ch"
   ADD HEADER TO hHeaders FILE "assert.ch"
   ADD HEADER TO hHeaders FILE "blob.ch"
   ADD HEADER TO hHeaders FILE "box.ch"
   ADD HEADER TO hHeaders FILE "button.ch"
   ADD HEADER TO hHeaders FILE "color.ch"
   ADD HEADER TO hHeaders FILE "common.ch"
   ADD HEADER TO hHeaders FILE "dbedit.ch"
   ADD HEADER TO hHeaders FILE "dbinfo.ch"
   ADD HEADER TO hHeaders FILE "dbstruct.ch"
   ADD HEADER TO hHeaders FILE "directry.ch"
   ADD HEADER TO hHeaders FILE "error.ch"
   ADD HEADER TO hHeaders FILE "fileio.ch"
   ADD HEADER TO hHeaders FILE "getexit.ch"
   ADD HEADER TO hHeaders FILE "hb.ch"
   ADD HEADER TO hHeaders FILE "hbclass.ch"
   ADD HEADER TO hHeaders FILE "hbcom.ch"
   ADD HEADER TO hHeaders FILE "hbdebug.ch"
   ADD HEADER TO hHeaders FILE "hbdyn.ch"
   ADD HEADER TO hHeaders FILE "hbextcdp.ch"
   ADD HEADER TO hHeaders FILE "hbextern.ch"
   ADD HEADER TO hHeaders FILE "hbextlng.ch"
   ADD HEADER TO hHeaders FILE "hbgfx.ch"
   ADD HEADER TO hHeaders FILE "hbgfxdef.ch"
   ADD HEADER TO hHeaders FILE "hbgtinfo.ch"
   ADD HEADER TO hHeaders FILE "hbhrb.ch"
   ADD HEADER TO hHeaders FILE "hbinkey.ch"
   ADD HEADER TO hHeaders FILE "hblang.ch"
   ADD HEADER TO hHeaders FILE "hblpp.ch"
   ADD HEADER TO hHeaders FILE "hbmacro.ch"
   ADD HEADER TO hHeaders FILE "hbmath.ch"
   ADD HEADER TO hHeaders FILE "hbmemory.ch"
   ADD HEADER TO hHeaders FILE "hbmemvar.ch"
   ADD HEADER TO hHeaders FILE "hboo.ch"
   ADD HEADER TO hHeaders FILE "hbpers.ch"
   ADD HEADER TO hHeaders FILE "hbsetup.ch"
   ADD HEADER TO hHeaders FILE "hbsix.ch"
   ADD HEADER TO hHeaders FILE "hbsocket.ch"
   ADD HEADER TO hHeaders FILE "hbstdgen.ch"
   ADD HEADER TO hHeaders FILE "hbsxdef.ch"
   ADD HEADER TO hHeaders FILE "hbthread.ch"
   ADD HEADER TO hHeaders FILE "hbtrace.ch"
   ADD HEADER TO hHeaders FILE "hbusrrdd.ch"
   ADD HEADER TO hHeaders FILE "hbver.ch"
   ADD HEADER TO hHeaders FILE "hbzlib.ch"
   ADD HEADER TO hHeaders FILE "inkey.ch"
   ADD HEADER TO hHeaders FILE "memoedit.ch"
   ADD HEADER TO hHeaders FILE "ord.ch"
   ADD HEADER TO hHeaders FILE "rddsys.ch"
   ADD HEADER TO hHeaders FILE "reserved.ch"
   ADD HEADER TO hHeaders FILE "set.ch"
   ADD HEADER TO hHeaders FILE "setcurs.ch"
   ADD HEADER TO hHeaders FILE "simpleio.ch"
   ADD HEADER TO hHeaders FILE "std.ch"
   ADD HEADER TO hHeaders FILE "tbrowse.ch"
   ADD HEADER TO hHeaders FILE "harbour.hbx"
   ADD HEADER TO hHeaders FILE "hbcpage.hbx"
   ADD HEADER TO hHeaders FILE "hblang.hbx"
   ADD HEADER TO hHeaders FILE "hbscalar.hbx"
   ADD HEADER TO hHeaders FILE "hbusrrdd.hbx"

   #if defined( __PLATFORM__UNIX )
      hb_HCaseMatch( hHeaders, .T. )
   #else
      hb_HCaseMatch( hHeaders, .F. )
   #endif

#else

   hHeaders := NIL

#endif /* HBRUN_WITH_HEADERS */

   RETURN hHeaders

/* Public hbrun API */
FUNCTION hbrun_DirBase()
   RETURN s_cDirBase

/* Public hbrun API */
FUNCTION hbrun_ProgName()
   RETURN s_cProgName

EXIT PROCEDURE hbrun_exit()

   hbrun_HistorySave()

   RETURN

STATIC FUNCTION hbrun_extensionlist()
   STATIC s_aList

   IF s_aList == NIL
      s_aList := iif( Type( "__HBRUN_EXTENSIONS()" ) == "UI", &("__hbrun_extensions()"), {} )
      ASort( s_aList )
   ENDIF

   RETURN s_aList

STATIC FUNCTION hbrun_FileSig( cFile )
   LOCAL hFile
   LOCAL cBuff, cSig, cExt

   cExt := ".prg"
   hFile := FOpen( cFile, FO_READ )
   IF hFile != F_ERROR
      cSig := hb_hrbSignature()
      cBuff := Space( Len( cSig ) )
      FRead( hFile, @cBuff, Len( cSig ) )
      FClose( hFile )
      IF cBuff == cSig
         cExt := ".hrb"
      ENDIF
   ENDIF

   RETURN cExt

STATIC PROCEDURE hbrun_Prompt( cCommand )
   LOCAL GetList
   LOCAL cLine
   LOCAL nMaxRow, nMaxCol
   LOCAL nHistIndex
   LOCAL bKeyUP, bKeyDown, bKeyIns, bKeyResize
   LOCAL lResize := .F.

   IF hb_gtVersion( 0 ) == "CGI"
      OutErr( "hbrun: Error: Interactive session not possible with GTCGI terminal driver" + hb_eol() )
      RETURN
   ENDIF

   CLEAR SCREEN
   SET SCOREBOARD OFF
   GetList := {}

   hbrun_HistoryLoad()

   AADD( s_aHistory, padr( "quit", HB_LINE_LEN ) )
   nHistIndex := Len( s_aHistory ) + 1

   IF ISCHARACTER( cCommand )
      AADD( s_aHistory, PadR( cCommand, HB_LINE_LEN ) )
      hbrun_Info( cCommand )
      hbrun_Exec( cCommand )
   ELSE
      cCommand := ""
   ENDIF

   hb_gtInfo( HB_GTI_RESIZEMODE, HB_GTI_RESIZEMODE_ROWS )

   SetKey( K_ALT_V, {|| hb_gtInfo( HB_GTI_CLIPBOARDPASTE ) } )

   Set( _SET_EVENTMASK, hb_bitOr( INKEY_KEYBOARD, HB_INKEY_GTEVENT ) )

   s_nRow := 2 + iif( Empty( hbrun_extensionlist() ), 0, 1 )

   DO WHILE .T.

      IF cLine == NIL
         cLine := Space( HB_LINE_LEN )
      ENDIF

      hbrun_Info( cCommand )

      nMaxRow := MaxRow()
      nMaxCol := MaxCol()
      @ nMaxRow, 0 SAY HB_PROMPT
      @ nMaxRow, Col() GET cLine ;
                       PICTURE "@KS" + hb_NToS( nMaxCol - Col() + 1 )

      SetCursor( iif( ReadInsert(), SC_INSERT, SC_NORMAL ) )

      bKeyIns  := SetKey( K_INS, ;
         {|| SetCursor( iif( ReadInsert( !ReadInsert() ), ;
                          SC_NORMAL, SC_INSERT ) ) } )
      bKeyUp   := SetKey( K_UP, ;
         {|| iif( nHistIndex > 1, ;
                  cLine := s_aHistory[ --nHistIndex ], ) } )
      bKeyDown := SetKey( K_DOWN, ;
         {|| cLine := iif( nHistIndex < LEN( s_aHistory ), ;
             s_aHistory[ ++nHistIndex ], ;
             ( nHistIndex := LEN( s_aHistory ) + 1, Space( HB_LINE_LEN ) ) ) } )
      bKeyResize := SetKey( HB_K_RESIZE,;
         {|| lResize := .T., hb_KeyPut( K_ENTER ) } )

      READ

      SetKey( K_DOWN, bKeyDown )
      SetKey( K_UP, bKeyUp )
      SetKey( K_INS, bKeyIns )
      SetKey( HB_K_RESIZE, bKeyResize )

      IF LastKey() == K_ESC .OR. EMPTY( cLine ) .OR. ;
         ( lResize .AND. LastKey() == K_ENTER )
         IF lResize
            lResize := .F.
         ELSE
            cLine := NIL
         ENDIF
         IF nMaxRow != MaxRow() .OR. nMaxCol != MaxCol()
            @ nMaxRow, 0 CLEAR
         ENDIF
         LOOP
      ENDIF

      IF EMPTY( s_aHistory ) .OR. ! ATAIL( s_aHistory ) == cLine
         IF LEN( s_aHistory ) < HB_HISTORY_LEN
            AADD( s_aHistory, cLine )
         ELSE
            ADEL( s_aHistory, 1 )
            s_aHistory[ LEN( s_aHistory ) ] := cLine
         ENDIF
      ENDIF
      nHistIndex := LEN( s_aHistory ) + 1

      cCommand := AllTrim( cLine, " " )
      cLine := NIL
      @ nMaxRow, 0 CLEAR
      hbrun_Info( cCommand )

      hbrun_Exec( cCommand )

      IF s_nRow >= MaxRow()
         Scroll( 2 + iif( Empty( hbrun_extensionlist() ), 0, 1 ), 0, MaxRow(), MaxCol(), 1 )
         s_nRow := MaxRow() - 1
      ENDIF

   ENDDO

   RETURN

/* ********************************************************************** */

STATIC PROCEDURE hbrun_Usage()

   OutStd( 'Harbour "DOt Prompt" Console / runner ' + HBRawVersion() + hb_eol() +;
           "Copyright (c) 1999-2011, Przemyslaw Czerpak, Viktor Szakats" + hb_eol() + ;
           "http://harbour-project.org/" + hb_eol() +;
           hb_eol() +;
           "Syntax:  hbrun [<file[.prg|.hbs|.hrb]> [<parameters,...>]]" + hb_eol() )

   RETURN

/* ********************************************************************** */

STATIC PROCEDURE hbrun_Info( cCommand )

   IF cCommand != NIL
      hb_DispOutAt( 0, 0, "PP: " )
      hb_DispOutAt( 0, 4, PadR( cCommand, MaxCol() - 3 ), "N/R" )
   ENDIF
   IF Used()
      hb_DispOutAt( 1, 0, ;
         PadR( "RDD: " + PadR( RddName(), 6 ) + ;
               " | Area:" + Str( Select(), 3 ) + ;
               " | Dbf: " + PadR( Alias(), 10 ) + ;
               " | Index: " + PadR( OrdName( IndexOrd() ), 8 ) + ;
               " | # " + Str( RecNo(), 7 ) + "/" + Str( RecCount(), 7 ), ;
               MaxCol() + 1 ), "N/BG" )
   ELSE
      hb_DispOutAt( 1, 0, ;
         PadR( "RDD: " + Space( 6 ) + ;
               " | Area:" + Space( 3 ) + ;
               " | Dbf: " + Space( 10 ) + ;
               " | Index: " + Space( 8 ) + ;
               " | # " + Space( 7 ) + "/" + Space( 7 ), ;
               MaxCol() + 1 ), "N/BG" )
   ENDIF
   IF s_lPreserveHistory
      hb_DispOutAt( 1, MaxCol(), "o", "R/BG" )
   ENDIF
   IF ! Empty( hbrun_extensionlist() )
      hb_DispOutAt( 2, 0, PadR( "Ext: " + ArrayToList( hbrun_extensionlist() ), MaxCol() + 1 ), "W/B" )
   ENDIF

   RETURN

STATIC FUNCTION ArrayToList( array )
   LOCAL cString := ""
   LOCAL tmp

   FOR tmp := 1 TO Len( array )
      cString += array[ tmp ]
      IF tmp < Len( array )
         cString += ", "
      ENDIF
   NEXT

   RETURN cString

/* ********************************************************************** */

STATIC PROCEDURE hbrun_Err( oErr, cCommand )

   LOCAL xArg, cMessage

   cMessage := "Sorry, could not execute:;;" + cCommand + ";;"
   IF oErr:ClassName == "ERROR"
      cMessage += oErr:Description
      IF !Empty( oErr:Operation )
         cMessage += " " + oErr:Operation
      ENDIF
      IF ISARRAY( oErr:Args ) .AND. Len( oErr:Args ) > 0
         cMessage += ";Arguments:"
         FOR EACH xArg IN oErr:Args
            cMessage += ";" + HB_CStr( xArg )
         NEXT
      ENDIF
   ELSEIF ISCHARACTER( oErr )
      cMessage += oErr
   ENDIF
   cMessage += ";;" + ProcName( 2 ) + "(" + hb_NToS( ProcLine( 2 ) ) + ")"

   Alert( cMessage )

   BREAK( oErr )

/* ********************************************************************** */

STATIC PROCEDURE hbrun_Exec( cCommand )
   LOCAL pHRB, cHRB, cFunc, bBlock, cEol, nRowMin

   cEol := hb_eol()
   cFunc := "STATIC FUNC __HBDOT()" + cEol + ;
            "RETURN {||" + cEol + ;
            "   " + cCommand + cEol + ;
            "   RETURN __MVSETBASE()" + cEol + ;
            "}" + cEol

   BEGIN SEQUENCE WITH {|oErr| hbrun_Err( oErr, cCommand ) }

      cHRB := HB_COMPILEFROMBUF( cFunc, hb_ProgName(), "-n2", "-q2" )
      IF cHRB == NIL
         EVAL( ErrorBlock(), "Syntax error." )
      ELSE
         pHRB := hb_hrbLoad( cHRB )
         IF pHrb != NIL
            bBlock := hb_hrbDo( pHRB )
            DevPos( s_nRow, s_nCol )
            Eval( bBlock )
            s_nRow := Row()
            s_nCol := Col()
            nRowMin := 2 + iif( Empty( hbrun_extensionlist() ), 0, 1 )
            IF s_nRow < nRowMin
               s_nRow := nRowMin
            ENDIF
         ENDIF
      ENDIF

   ENDSEQUENCE

   __MVSETBASE()

   RETURN

STATIC FUNCTION HBRawVersion()
   RETURN StrTran( Version(), "Harbour " )

/* ********************************************************************** */

#define _HISTORY_DISABLE_LINE "no"

STATIC PROCEDURE hbrun_HistoryLoad()
   LOCAL cHistory
   LOCAL cLine

   s_lWasLoad := .T.

   IF s_lPreserveHistory
      cHistory := StrTran( MemoRead( hbrun_HistoryFileName() ), Chr( 13 ) )
      IF Left( cHistory, Len( _HISTORY_DISABLE_LINE + Chr( 10 ) ) ) == _HISTORY_DISABLE_LINE + Chr( 10 )
         s_lPreserveHistory := .F.
      ELSE
         FOR EACH cLine IN hb_ATokens( StrTran( cHistory, Chr( 13 ) ), Chr( 10 ) )
            IF ! Empty( cLine )
               AAdd( s_aHistory, PadR( cLine, HB_LINE_LEN ) )
            ENDIF
         NEXT
      ENDIF
   ENDIF

   RETURN

STATIC PROCEDURE hbrun_HistorySave()
   LOCAL cHistory
   LOCAL cLine

   IF s_lWasLoad .AND. s_lPreserveHistory
      cHistory := ""
      FOR EACH cLine IN s_aHistory
         IF !( Lower( AllTrim( cLine ) ) == "quit" )
            cHistory += AllTrim( cLine ) + hb_eol()
         ENDIF
      NEXT
      hb_MemoWrit( hbrun_HistoryFileName(), cHistory )
   ENDIF

   RETURN

STATIC FUNCTION hbrun_HistoryFileName()
   LOCAL cEnvVar
   LOCAL cDir
   LOCAL cFileName

#if defined( __PLATFORM__WINDOWS )
   cEnvVar := "APPDATA"
#else
   cEnvVar := "HOME"
#endif

#if defined( __PLATFORM__DOS )
   cFileName := "hbrunhst.ini"
#else
   cFileName := ".hbrun_history"
#endif

   IF ! Empty( GetEnv( cEnvVar ) )
#if defined( __PLATFORM__DOS )
      cDir := GetEnv( cEnvVar ) + hb_ps() + "~harbour"
#else
      cDir := GetEnv( cEnvVar ) + hb_ps() + ".harbour"
#endif
   ELSE
      cDir := hb_dirBase()
   ENDIF

   IF ! hb_dirExists( cDir )
      hb_dirCreate( cDir )
   ENDIF

   RETURN cDir + hb_ps() + cFileName

STATIC FUNCTION hbrun_FindInPath( cFileName )
   LOCAL cDir
   LOCAL cName
   LOCAL cExt
   LOCAL cFullName
   LOCAL aExt

   hb_FNameSplit( cFileName, @cDir, @cName, @cExt )
   aExt := iif( Empty( cExt ), { ".hbs", ".hrb" }, { cExt } )

   FOR EACH cExt IN aExt
      /* Check original filename (in supplied path or current dir) */
      IF hb_FileExists( cFullName := hb_FNameMerge( cDir, cName, cExt ) )
         RETURN cFullName
      ENDIF
   NEXT

   IF Empty( cDir )
      IF ! Empty( cDir := hb_DirBase() )
         /* Check in the dir of this executable. */
         FOR EACH cExt IN aExt
            IF hb_FileExists( cFullName := hb_FNameMerge( cDir, cName, cExt ) )
               RETURN cFullName
            ENDIF
         NEXT
      ENDIF

      FOR EACH cExt IN aExt
         /* Check in the PATH. */
         #if defined( __PLATFORM__WINDOWS ) .OR. ;
             defined( __PLATFORM__DOS ) .OR. ;
             defined( __PLATFORM__OS2 )
         FOR EACH cDir IN hb_ATokens( GetEnv( "PATH" ), hb_osPathListSeparator(), .T., .T. )
            IF Left( cDir, 1 ) == '"' .AND. Right( cDir, 1 ) == '"'
               cDir := SubStr( cDir, 2, Len( cDir ) - 2 )
            ENDIF
         #else
         FOR EACH cDir IN hb_ATokens( GetEnv( "PATH" ), hb_osPathListSeparator() )
         #endif
            IF ! Empty( cDir )
               IF hb_FileExists( cFullName := hb_FNameMerge( cDir ), cName, cExt )
                  RETURN cFullName
               ENDIF
            ENDIF
         NEXT
      NEXT
   ENDIF

   RETURN cFileName

#if defined( __PLATFORM__WINDOWS )

STATIC FUNCTION win_reg( lRegister, lAllUser )
   LOCAL lRetVal
   LOCAL cFileName
   LOCAL fhnd := hb_FTempCreateEx( @cFileName )

   IF fhnd != F_ERROR
      FWrite( fhnd, win_reg_file( lRegister, lAllUser ) )
      FClose( fhnd )
      /* The regedit version I tested (win7) didn't return an errorlevel on error. [vszakats] */
      lRetVal := ( hb_processRun( "regedit.exe /s " + Chr( 34 ) + cFileName + Chr( 34 ) ) == 0 )
      FErase( cFileName )
   ELSE
      lRetVal := .F.
   ENDIF

   RETURN lRetVal

STATIC FUNCTION win_reg_file( lRegister, lAllUser, cAppPath )
   LOCAL cHive := iif( hb_isLogical( lAllUser ) .AND. lAllUser, "HKEY_CLASSES_ROOT", "HKEY_CURRENT_USER\Software\Classes" )

   IF ! hb_isString( cAppPath )
      cAppPath := hb_ProgName()
   ENDIF

   IF hb_isLogical( lRegister ) .AND. ! lRegister
      /* unregister */
      RETURN ;
         'REGEDIT4' + hb_eol() +;
         hb_eol() +;
         '[-' + cHive + '\.hbs]' + hb_eol() +;
         hb_eol() +;
         '[-' + cHive + '\HBSFile]' + hb_eol()
   ENDIF

   /* register */
   RETURN ;
      'REGEDIT4' + hb_eol() +;
      hb_eol() +;
      '[' + cHive + '\.hbs]' + hb_eol() +;
      '@="HBSFile"' + hb_eol() +;
      hb_eol() +;
      '[' + cHive + '\HBSFile]' + hb_eol() +;
      '@="Harbour Script File"' + hb_eol() +;
      hb_eol() +;
      '[' + cHive + '\HBSFile\DefaultIcon]' + hb_eol() +;
      '@="' + StrTran( cAppPath, "\", "\\" ) + ',-1"' + hb_eol() +;
      hb_eol() +;
      '[' + cHive + '\HBSFile\Shell]' + hb_eol() +;
      '@="Run"' + hb_eol() +;
      hb_eol() +;
      '[' + cHive + '\HBSFile\Shell\Run\Command]' + hb_eol() +;
      '@="' + StrTran( cAppPath, "\", "\\" ) + ' \"%1\""'

#endif
