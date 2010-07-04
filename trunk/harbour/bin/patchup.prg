#!/usr/bin/hbrun --hb:gtcgi
/*
 * $Id$
 */

/*
 * Copyright 2010 Tamas Tevesz
 * See COPYING for licensing terms.
 */

/*
 * patchup - a tool to help update external components while keeping local fixes
 *
 * 1. CONFIGURATION
 * ----------------
 *
 * For proper operation, the following external tools are required to be present
 * somewhere in your $PATH:
 *
 * - The GNU version of `patch', `diff' and `tar' (patchup will figure it out
 *   if you have them by the names of `gpatch', `gdiff' or `gtar')
 *
 * - curl, gzip, bzip2 and unzip (only the Info-ZIP version of unzip has been
 *   tested)
 *
 * patchup requires several meta data (in the form of specially formatted lines)
 * in the component's Makefile or .hbp file. Formatting rules are as follows:
 *
 * - The first character on the line is a hash mark (`#'), followed by any number
 *   of white spaces.
 * - Next comes a keyword, followed by any number of white spaces.
 * - The keyword is followed by one or more arguments, separated by white spaces.
 *   The number of arguments taken depends on the keyword itself.
 *
 * The keywords themselves are case sensitive (only upper case keywords are
 * recognized). The arguments are case senstive as well.
 *
 * Currently recognized keywords and their arguments are as follows:
 *
 * ORIGIN
 *   Takes one argument, the URL of component's home page. Not currently used,
 *   but greatly helps locating resources regarding the component.
 *   Example: for PCRE, it is `http://www.pcre.org/'.
 *
 * VER
 *   Takes one argument, the version number of the component currently in the
 *   Harbour tree. Not currently used, but greatly helps checking whether the
 *   component needs an update.
 *   Example: for PCRE, at the time of this writing, it is `8.02'.
 *
 * URL
 *   Takes one argument, the URL to the archive to the currently installed
 *   version of the component. Used by patchup.
 *   Example: for PCRE, at the time of this writing, it is
 *   `ftp://ftp.csx.cam.ac.uk/pub/software/programming/pcre/pcre-8.02.zip'.
 *   patchup can currently unpack only `tar.gz', `tar.bz2', `tgz', `tbz',
 *   `tbz2' or `zip' archives -- one of these must be chosen.
 *
 *   patchup will also use the URL parameter to figure out what type of
 *   file it is working with, so a URL containing this sort if information must
 *   be picked. As an example, SourceForge-style distributed download URLs like
 *   `http://sourceforge.net/projects/libpng/files/01-libpng-master/1.4.2/lpng142.zip/download'
 *   are OK, but `http://example.com/download/latest' is not, even if latter
 *   would ultimately result (perhaps by the server using Content-Disposition
 *   or similar headers) in a file named `example-pkg-54.tar.gz'.
 *
 * DIFF
 *   Takes one argument, the file name of the diff file containing local changes
 *   needed by Harbour. In `rediff' mode, this parameter is optional; if not
 *   specified, defaults to `$(component).dif'.
 *   Example: for PCRE, it is `pcre.dif'.
 *
 * MAP
 *   Takes one or two arguments, specifying the correspondence of the file names
 *   between the original sources and the Harbour sources (which are reduced to
 *   8+3 format, in order to stay compatible with DOS).
 *   If a particular file name is the same both in the upstream and the Harbour
 *   trees, it is sufficient to specify it only once, but every file that needs
 *   to be brought over to the Harbour tree must be specified.
 *   The very first `MAP' occurrence is treated specially: it's argument is used
 *   by patchup to locate the root of the extracted upstream source tree.
 *   Examples:
 *
 *   # MAP LICENCE
 *
 *      The file named `LICENCE' needs to be brought over from the upstream tree
 *      to the Harbour tree unchanged. In case of PCRE, `MAP LICENCE' being the
 *      first `MAP' line also means that patchup will use the directory
 *      containing this file as a base for all other files occurring later.
 *      Accordingly, the first `MAP' entry must be flat even on the source side.
 *
 *   # MAP config.h.generic config.h
 *
 *      The file named `config.h.generic' in the upstream source tree needs to
 *      be brought over to the Harbour tree by the name `config.h'.
 *
 *   # MAP foo/bar.h
 *
 *      If the upstream source tree is not flat, relative paths may also be
 *      specified. The above means "bring `foo/bar.h' from upstream as `bar.h'
 *      into the Harbour tree". Note that the Harbour component tree is always
 *      flat, it is illegal to specify - for example - `MAP foo/bar.h zoink/baz.h'.
 *
 *   # MAP foo/bar.h baz.h
 *
 *      The upstream source file `foo/bar.h' needs to be brought over to the
 *      Harbour tree as `baz.h'. All notes above about hierarchical and flat
 *      trees strictly apply.
 *
 *   For hierarchical source trees, the path separator must always be the UNIX
 *   forward slash (`/'). DOS-style backslash separators are not recognized and
 *   will produce undefined results.
 *
 * 2. MODES OF OPERATION
 * ---------------------
 *
 * By default, patchup operates in `component version updating' mode - that is,
 * refreshing the component version to a newer upstream version.
 *
 * If patchup is called with the `-rediff' command line argument, it switches
 * to a `local diff refresh' mode. This mode is used to refresh the local diff
 * after Harbour-specific modifications have been made to the component's
 * source. In order to help with the initial diff creation, patchup will proceed
 * even if no `DIFF' is specified amongst the meta data, and defaults to
 * creating a diff named `$(component).dif').
 *
 * If no differences between the original and the Harbour trees were found,
 * a possibly pre-existing diff file is removed. Following this change up
 * in the component's Makefile (or .hbp file) is left for the operator -- patchup
 * will communicate if there is a likely need to perform this action.
 *
 * It is strongly advised not to try to mix the two modes. If there are any
 * pending local modifications, a rediff should be done before a component
 * version update is performed.
 *
 * 3. TYPICAL WORKFLOW
 * -------------------
 *
 * Once it has been determined that a particular component needs an update, the URL
 * argument has to be modified to point to the new source tree archive. VER should
 * also be updated. While residing in the component's directory, patchup needs
 * to be run. The rest is mostly automatic - patchup retrieves, unpacks and
 * otherwise prepares the updated source tree, applies any local modifications,
 * and copies any changes back to the Harbour tree (the current working directory).
 * After some inspection and a test, it is ready to be committed.
 *
 * In rediff mode, care must be taken for the URL keyword to contain a reference
 * to the version that is in the current Harbour tree (that basically means `do not
 * touch anything', assuming correct information in the first place). After patchup
 * is finished rediffing, the new `local changes' file (see `DIFF') may be inspected,
 * and is ready to be committed.
 *
 * If errors are encountered during rediff, the contents of the temporary directory
 * may be used as a starting basis for manual re-diffing.
 *
 * 4. TROUBLESHOOTING
 * ------------------
 *
 * Several things can go wrong, and patchup tries hard handle them as gracefully as
 * possible. First and foremost, in case of even the slightest sign of something
 * not happening as intended, patchup will not modify the Harbour tree at all.
 * Everything is happening inside a temporary directory, which is not erased when
 * patchup exits (not even when it exits normally), and where certain log files are
 * created. These log files may contain information to help debugging in case of
 * an (unhandled) error.
 *
 * The organization of the temporary directory is as follows:
 *
 * $(component)         These directories are the extracted and modified versions
 * $(component).orig    of the upstream source tree. The `.orig' tree consists of
 *                      only renamed (and renames followed up in the files),
 *                      whereas the $(component) tree will have the local
 *                      modifications (see the `DIFF' keyword) applied. These two
 *                      directories are used for re-creating the local changes.
 *
 * root                 The new upstream source is unpacked in this directory.
 *
 * fetch.log            The output `curl' produced while fetching the source
 *                      archive.
 *
 * extract.log          The output the extractor (gzip, bzip2) produced while
 *                      uncompressing the archive.
 *
 * archive.log          The output the archiver (tar, unzip) produced while
 *                      unpacking the archive.
 *
 * patch.log            The output `patch' produced while applying the local
 *                      changes to $(component).
 *
 * diff.log             The standard error of `diff' produced while re-creating
 *                      the local changes (the standard output is the diff itself
 *                      and is placed in the local changes file, see `DIFF').
 *
 * some archive file    The new source tree archive.
 *
 * In all error cases patchup will produce a meaningful error message. Armed with
 * that and the information here, troubleshooting should not be much of a problem.
 *
 * 4. BUGS
 * -------
 *
 * None known. More testing on non-UNIX systems is desired.
 *
 */

#pragma warninglevel=3

#if 0
   #include "directry.ch"
#else
   #define F_NAME      1  /* directry.ch */
   #define F_ATTR      5  /* directry.ch */
#endif

#if defined( _TRACE )
   #define TRACE( str )    OutStd( "T: " + str + OSNL )
#else
   #define TRACE( str )
#endif

#define OSPS                 hb_osPathSeparator()
#define OSNL                 hb_osNewLine()

#define ONEARG_KW   2        /* one-arg line keyword */
#define ONEARG_ARG  3        /* one-arg line argument */
#define TWOARG_KW   2        /* two-arg line keyword */
#define TWOARG_ARG1 3        /* two-arg line first argument */
#define TWOARG_ARG2 4        /* two-arg line second argument */

#define FN_ORIG     1        /* original file name in maps */
#define FN_HB       2        /* hb file name in maps */

STATIC s_aChangeMap := {}    /* from-to file name map */
STATIC s_cTempDir := NIL
STATIC s_nErrors := 0        /* error indicator */
STATIC s_cSourceRoot := NIL  /* top directory of the newly-unpacked source tree */

STATIC s_aTools := {             ;
   "patch"  => NIL,              ;
   "diff"   => NIL,              ;
   "curl"   => NIL,              ;
   "tar"    => NIL,              ;
   "gzip"   => NIL,              ;
   "bzip2"  => NIL,              ;
   "unzip"  => NIL               ;
}

PROCEDURE Main( ... )

   LOCAL cFileName
   LOCAL cFile             /* memorized input make file */
   LOCAL aDir
   LOCAL aRegexMatch       /* regex match results */
   LOCAL cMemoLine         /* MemoLine */
   LOCAL nMemoLine         /* Line number */
   LOCAL cDiffFile         /* Local modifications */
   LOCAL cCWD
   LOCAL cThisComponent    /* component being processed */
   LOCAL aOneMap           /* one pair from s_aChangeMap */
   LOCAL cCommand          /* patch/diff commands */
   LOCAL nRunResult        /* patch/diff exit vals */
   LOCAL cDiffText         /* diff will return the new diff in this */
   LOCAL nDiffFD           /* for writing newly created diff file */
   LOCAL cArchiveURL       /* URL for the component */
   LOCAL cTopIndicator     /* file signifying the top of the component's source tree */
   LOCAL cStdOut           /* stdout and stderr for various externally-run apps */
   LOCAL cStdErr
   LOCAL lRediff := .F.    /* whether or not operating as rediff */
   LOCAL aArgv
   LOCAL cRoot := NIL

   LOCAL hRegexTake1Line := hb_regexComp( "^#[[:blank:]]*(ORIGIN|VER|URL|DIFF)[[:blank:]]+(.+?)[[:blank:]]*$" )
   LOCAL hRegexTake2Line := hb_regexComp( "^#[[:blank:]]*(MAP)[[:blank:]]+(.+?)[[:blank:]]+(.+?)[[:blank:]]*$" )

   aArgv := hb_AParams()
   IF ! Empty( aArgv )
      SWITCH aArgv[ 1 ]
         CASE "-rediff"
            lRediff := .T.
            EXIT
         CASE "-h"
         CASE "-help"
         CASE "--help"
         CASE "/?"
            Usage( 0 )
         OTHERWISE
            Usage( 1 )
      ENDSWITCH
   ENDIF

   IF ! hb_FileExists( cFileName := "Makefile" )
      IF Empty( aDir := Directory( "*.hbp" ) )
         OutStd( "No `Makefile' or '*.hbp' file in the current directory." + OSNL )
         ErrorLevel( 1 )
         QUIT
      ELSE
         ASort( aDir,,, { | tmp, tmp1 | tmp[ F_NAME ] < tmp1[ F_NAME ] } )
         cFileName := aDir[ 1 ][ F_NAME ]
      ENDIF
   ENDIF

   SetupTools()

   cFile := MemoRead( cFileName )
   cDiffFile := NIL        /* default to `no local diff' */

   nMemoLine := 0

   FOR EACH cMemoLine IN hb_ATokens( StrTran( cFile, Chr( 13 ) ), Chr( 10 ) )

      cMemoLine := AllTrim( cMemoLine )
      nMemoLine++

      IF ! Empty( aRegexMatch := hb_regex( hRegexTake1Line, cMemoLine ) )
         /* Process one-arg keywords */
         IF aRegexMatch[ ONEARG_KW ] == "DIFF"
            cDiffFile := iif( Empty( AllTrim( aRegexMatch[ ONEARG_ARG ] ) ), NIL,         ;
                              AllTrim( aRegexMatch[ ONEARG_ARG ] ) )
         ELSEIF aRegexMatch[ ONEARG_KW ] == "URL"
            cArchiveURL := AllTrim( aRegexMatch[ ONEARG_ARG ] )
         ENDIF

      ELSEIF ! Empty( aRegexMatch := hb_regex( hRegexTake2Line, cMemoLine ) )
         /* Process two-arg keywords */
         IF aRegexMatch[ TWOARG_KW ] == "MAP"
            /* Do not allow implicit destination with non-flat source spec */
            IF Empty( aRegexMatch[ TWOARG_ARG1 ] ) .AND. "/" $ aRegexMatch[ TWOARG_ARG2 ]
               OutStd( hb_strFormat( "E: Non-flat source spec with implicit " +           ;
                                     "destination, offending line %d:%s:", nMemoLine, OSNL ) )
               OutStd( aRegexMatch[ 1 ] + OSNL )
               ErrorLevel( 2 )
               QUIT
            ENDIF
            /* Do not allow tree spec in the destination ever */
            IF "/" $ aRegexMatch[ TWOARG_ARG2 ]
               OutStd( hb_strFormat( "E: Non-flat destination, offending line %d:%s",     ;
                                     nMemoLine, OSNL ) )
               OutStd( aRegexMatch[ 1 ] + OSNL )
               ErrorLevel( 2 )
               QUIT
            ENDIF
            /* If the source argument indicates the source tree is not flat, convert
             * path separator to native. The HB tree is always flattened. */
            IF "/" $ aRegexMatch[ TWOARG_ARG1 ]
               aRegexMatch[ TWOARG_ARG1 ] := StrTran( aRegexMatch[ TWOARG_ARG1 ], "/", OSPS )
            ENDIF
            /* The destination argument must fit in the 8+3 scheme */
            IF Len( FN_NameGet( aRegexMatch[ TWOARG_ARG2 ] ) ) > 8 .OR.                   ;
                  Len( FN_ExtGet( aRegexMatch[ TWOARG_ARG2 ] ) ) > 3
               OutStd( hb_strFormat( "E: Destination does not fit 8+3, offending "+       ;
                                     "line %d:%s", nMemoLine, OSNL ) )
               OutStd( aRegexMatch[ 1 ] + OSNL )
               ErrorLevel( 2 )
               QUIT
            ENDIF
            /* In case the priginal and the HB file names are identical, the
             * second argument to `MAP' is optional. Due to the way the regex is
             * constructed, in this case the last backref will contain the only
             * file name, so shuffle arguments around accordingly
             */
            AAdd( s_aChangeMap, {                                                         ;
               iif( Empty( aRegexMatch[ TWOARG_ARG1 ] ),                                  ;
                     aRegexMatch[ TWOARG_ARG2 ],                                          ;
                     aRegexMatch[ TWOARG_ARG1 ] ), aRegexMatch[ TWOARG_ARG2 ]             ;
            } )
            /* If this is the first MAP entry, treat the original part as the
             * source tree root indicator */
            IF Len( s_aChangeMap ) == 1
               cTopIndicator := s_aChangeMap[ 1 ][ FN_ORIG ]
               IF "/" $ cTopIndicator
                  OutStd( hb_strFormat( "E: First `MAP' entry is not flat, offending " +  ;
                                        "line %d:%s", nMemoLine, OSNL ) )
                  OutStd( aRegexMatch[ 1 ] + OSNL )
                  ErrorLevel( 2 )
                  QUIT
               ENDIF
            ENDIF
         ENDIF
      ENDIF
   NEXT

   IF Empty( s_aChangeMap ) .AND. cDiffFile == NIL
      OutStd( "No file name changes and no local diff, nothing to do." + OSNL )
      QUIT
   ENDIF

   IF ! lRediff .AND. cDiffFile != NIL .AND. ! hb_FileExists( cDiffFile )
      OutStd( "E: `" + cDiffFile + "' does not exist" + OSNL )
      ErrorLevel( 2 )
      QUIT
   ENDIF

   cCWD := hb_CurDrive() + hb_osDriveSeparator() + OSPS + CurDir()

   #if defined( _CURDIR )
      cRoot := cCWD + OSPS
   #endif

   FClose( hb_FTempCreateEx( @s_cTempDir, cRoot, FN_NameGet( hb_ProgName() ) + "_" ) )
   FErase( s_cTempDir )
   MakeDir( s_cTempDir )

   cThisComponent := FN_NameGet( cCWD )

   MakeDir( CombinePath( s_cTempDir, cThisComponent ) )
   MakeDir( CombinePath( s_cTempDir, cThisComponent + ".orig" ) )
   MakeDir( CombinePath( s_cTempDir, "root" ) )

   IF lRediff .AND. cDiffFile == NIL
      OutStd( "Requested rediff mode with no existing local diff, attempting to create one." + OSNL )
      cDiffFile := cThisComponent + ".dif"
   ENDIF

   IF ! FetchAndExtract( cArchiveURL )
      OutStd( "E: Fetching or extracting the source archive failed." + OSNL )
      OutStd( "   Inspect `" + s_cTempDir + "' for further clues." + OSNL )
      ErrorLevel( 2 )
      QUIT
   ENDIF

   s_cSourceRoot := WalkAndFind( CombinePath( s_cTempDir, "root" ), cTopIndicator )
   IF s_cSourceRoot == NIL
      OutStd( "E: Unable to find the new tree's root" + OSNL )
      OutStd( "   Inspect `" + s_cTempDir + "'" + OSNL )
      ErrorLevel( 2 )
      QUIT
   ENDIF

   /*
    * Create two copies of the relevant portions of the source archive.
    * The pristine tree is for reference, used as the left component of the diff
    * Our tree will have the local diff applied, and used as the right component of the diff
    */
   FOR EACH aOneMap IN s_aChangeMap
      IF ! hb_FileExists( CombinePath( s_cSourceRoot, aOneMap[ FN_ORIG ] ) )
         OutStd( "W: `" + aOneMap[ FN_ORIG ] + "' does not exist in the source tree" + OSNL )
         OutStd( "   I will do what i can, but you'd better check the results manually." + OSNL )
         s_nErrors++
      ELSE
         /* Create the `pristine tree' */
         hb_FCopy( CombinePath( s_cSourceRoot, aOneMap[ FN_ORIG ] ),                      ;
                   CombinePath( s_cTempDir, cThisComponent + ".orig", aOneMap[ FN_HB ] ) )

         /* Munch the file, applying the appropriate xforms */
         hb_FileTran( CombinePath( s_cTempDir, cThisComponent + ".orig", aOneMap[ FN_HB ] ) )

         /* If operating in `rediff' mode, copy the current Harbour component tree;
          * otherwise, duplicate the pristine tree */

         IF lRediff
            hb_FCopy( aOneMap[ FN_HB ],                                                   ;
                      CombinePath( s_cTempDir, cThisComponent, aOneMap[ FN_HB ] ) )

         ELSE
            /* Copy it to `our tree' */
            hb_FCopy( CombinePath( s_cTempDir, cThisComponent + ".orig", aOneMap[ FN_HB ] ),;
                      CombinePath( s_cTempDir, cThisComponent, aOneMap[ FN_HB ] ) )
         ENDIF

      ENDIF
   NEXT

   IF cDiffFile != NIL

      IF ! lRediff /* If we have a local diff, and are not to re-create it, apply */
         cCommand := hb_strFormat( "%s -d %s -p 1 -i %s",                                 ;
                     s_aTools[ "patch" ],                                                 ;
                     CombinePath( s_cTempDir, cThisComponent ),                           ;
                     CombinePath( cCWD, cDiffFile ) )
         TRACE( "Running " + cCommand )
         nRunResult := hb_processRun( cCommand, , @cStdOut, @cStdErr, .F. )
         SaveLog( "patch", cStdOut, cStdErr )
         IF nRunResult != 0
            OutStd( "W: Unexpected events happened during patching, inspect " + s_cTempDir + OSNL )
            s_nErrors++
         ENDIF
      ENDIF

      /* Re-create the diff */
      cCommand := hb_strFormat( "%s -urN %s %s",                                          ;
                  s_aTools[ "diff" ], cThisComponent + ".orig", cThisComponent )

      DirChange( s_cTempDir )
      TRACE( "Running " + cCommand )
      hb_processRun( cCommand, , @cDiffText, @cStdErr, .F. )
      DirChange( cCWD )

      SaveLog( "diff", NIL, cStdErr )

      IF Len( cDiffText ) > 0
         nDiffFD := FCreate( cDiffFile )
         FWrite( nDiffFD, cDiffText )
         FClose( nDiffFD )
         OutStd( "Local changes saved to `" + cDiffFile + "'; you may need to adjust `DIFF'." + OSNL )
      ELSE
         OutStd( "No local changes; you may need to adjust `DIFF'." + OSNL )
         IF hb_FileExists( cDiffFile )
            FErase( cDiffFile )
            OutStd( "Removed existing `" + cDiffFile + "'." + OSNL )
         ENDIF
      ENDIF

   ENDIF

   /* Only copy files back to the live tree if no errors were encountered */
   IF s_nErrors == 0
      IF ! lRediff
         /* Only copy the complete new tree back if not in Rediff mode */
         FOR EACH aOneMap IN s_aChangeMap
            hb_FCopy( CombinePath( s_cTempDir, cThisComponent, aOneMap[ FN_HB ] ), aOneMap[ FN_HB ] )
         NEXT
      ENDIF

      IF cDiffFile != NIL
         /* Copy the diff back to the live tree */
         hb_FCopy( CombinePath( s_cTempDir, cDiffFile ), cDiffFile )
      ENDIF

   ELSE
      OutStd( "Errors were encountered, no changes are made to your Harbour tree." + OSNL )
      OutStd( "Inspect " + s_cTempDir + " for further clues." + OSNL )
   ENDIF

   IF ! lRediff
      OutStd( "Don't forget to update `" + cFileName + "' with the new version and URL information." + OSNL )
   ENDIF
   OutStd( "The temporary directory `" + s_cTempDir + "' has not been removed." +OSNL )

   RETURN

/* Utility functions */

STATIC PROCEDURE SetupTools()

#if defined( __PLATFORM__UNIX )
   LOCAL cExeExt := ""
#else
   LOCAL cExeExt := ".exe"
#endif
   LOCAL cPathComp
   LOCAL cTool

   /* Look for g$tool first, only attempt raw name if it isn't found
    * Helps non-GNU userland systems with GNU tools installed.
    * Only several of the tools are known to have GNU variants. */
   FOR EACH cPathComp IN hb_ATokens( hb_GetEnv( "PATH" ), hb_osPathListSeparator() )
      FOR EACH cTool IN hb_HKeys( s_aTools )
         IF cTool $ "patch|diff|tar" .AND. hb_FileExists( CombinePath( cPathComp, "g" + cTool ) + cExeExt )
            s_aTools[ cTool ] := CombinePath( cPathComp, "g" + cTool )
         ENDIF
      NEXT
   NEXT

   FOR EACH cPathComp in hb_ATokens( hb_GetEnv( "PATH" ), hb_osPathListSeparator() )
      FOR EACH cTool IN hb_HKeys( s_aTools )
         IF s_aTools[ cTool ] == NIL .AND. hb_FileExists( CombinePath( cPathComp, cTool ) + cExeExt )
            s_aTools[ cTool ] := CombinePath( cPathComp, cTool )
         ENDIF
      NEXT
   NEXT

   FOR EACH cTool in hb_HKeys( s_aTools )
      IF s_aTools[ cTool ] == NIL
         OutStd( "E: Can not find " + cTool + OSNL )
         ErrorLevel( 1 )
         QUIT
      ENDIF
   NEXT

   RETURN

STATIC FUNCTION CombinePath( ... )

   LOCAL aArguments := hb_AParams()
   LOCAL cRetVal := ""
   LOCAL nI

   IF Len( aArguments ) == 2
      cRetVal := aArguments[ 1 ] + OSPS + aArguments[ 2 ]
   ELSE
      cRetVal := aArguments[ 1 ] + OSPS
      FOR nI := 2 TO Len( aArguments ) - 1
         cRetVal += aArguments[ nI ] + OSPS
      NEXT
      cRetVal += aArguments[ Len( aArguments ) ]
   ENDIF

   RETURN cRetVal

STATIC FUNCTION WalkAndFind( cTop, cLookFor )

   LOCAL aDir
   LOCAL aDirEntry
   LOCAL cRetVal := NIL

   cTop += iif( Right( cTop, 1 ) $ "/\", "", hb_osPathSeparator() )
   aDir := Directory( cTop + hb_osFileMask(), "D" )

   ASORT( aDir,,, { |aLeft| !( aLeft[ F_ATTR ] $ "D" ) } )   /* Files first */

   FOR EACH aDirEntry IN aDir
      IF !( aDirEntry[ F_ATTR ] == "D" )
         IF aDirEntry[ F_NAME ] == cLookFor
            cRetVal := cTop
            EXIT
         ENDIF
      ELSEIF !( aDirEntry[ F_NAME ] == "." ) .AND. ;
             !( aDirEntry[ F_NAME ] == ".." )
         cRetVal := WalkAndFind( cTop + aDirEntry[ F_NAME ], cLookFor )
         IF ! Empty( cRetVal )
            EXIT
         ENDIF
      ENDIF
   NEXT

   RETURN cRetVal

STATIC FUNCTION FetchAndExtract( cArchiveURL )

   LOCAL cCommand
   LOCAL cExtractor := NIL
   LOCAL cExtractorArgs := NIL
   LOCAL cExtractedFileName := NIL
   LOCAL cArchiver := NIL
   LOCAL cArchiverArgs := NIL
   LOCAL nResult
   LOCAL cStdOut
   LOCAL cStdErr
   LOCAL cPattern
   LOCAL cMatchedPattern
   LOCAL cFileName
   LOCAL cFrag
   LOCAL cCWD

   /* Any given package is surely available in at least one of these formats,
    * pick one of these, refrain from the more exotic ones. */

   LOCAL aActionMap := {                                                                  ;
      '.tar.gz|.tgz' => {                                                                 ;
         'Extractor'          => 'gzip',                                                  ;
         'ExtractorArgs'      => '-d',                                                    ;
         'ExtractedFile'      => '.tar',                                                  ;
         'Archiver'           => 'tar',                                                   ;
         'ArchiverArgs'       => '--force-local -xvf'                                     ;
      },                                                                                  ;
      '.tar.bz2|.tbz|.tbz2' => {                                                          ;
         'Extractor'          => 'bzip2',                                                 ;
         'ExtractorArgs'      => '-d',                                                    ;
         'ExtractedFile'      => '.tar',                                                  ;
         'Archiver'           => 'tar',                                                   ;
         'ArchiverArgs'       => '--force-local -xvf'                                     ;
      },                                                                                  ;
      '.zip' => {                                                                         ;
         'Extractor'          => NIL,                                                     ;
         'ExtractorArgs'      => NIL,                                                     ;
         'ExtractedFile'      => NIL,                                                     ;
         'Archiver'           => 'unzip',                                                 ;
         'ArchiverArgs'       => ''                                                       ;
      }                                                                                   ;
   }

   cFileName := URL_GetFileName( cArchiveURL )

   FOR EACH cPattern IN hb_HKeys( aActionMap )
      FOR EACH cFrag IN HB_ATokens( cPattern, "|" )
         IF At( cFrag, cFileName ) != 0
            cMatchedPattern := cFrag
            cExtractor := aActionMap[ cPattern ][ 'Extractor' ]
            cExtractorArgs := aActionMap[ cPattern ][ 'ExtractorArgs' ]
            cExtractedFileName := iif( aActionMap[ cPattern ][ 'ExtractedFile' ] == NIL,  ;
                                    NIL,                                                  ;
                                    Left( cFileName, Len( cFileName ) -                   ;
                                       Len( cMatchedPattern ) ) +                         ;
                                       aActionMap[ cPattern ][ 'ExtractedFile' ]          ;
                                  )
            cArchiver := aActionMap[ cPattern ][ 'Archiver' ]
            cArchiverArgs := aActionMap[ cPattern ][ 'ArchiverArgs' ]
            EXIT
         ENDIF
      NEXT
   NEXT

   IF cArchiver == NIL
      OutStd( "E: Can not find archiver for `" +                                          ;
               FN_NameExtGet( cArchiveURL ) + "'" + OSNL )
      RETURN .F.
   ELSE
      /* Fetch */
      cCommand := hb_strFormat( "%s -L -# -o %s %s", s_aTools[ "curl" ],                  ;
                  CombinePath( s_cTempDir, cFileName ),                                   ;
                  FN_Escape( cArchiveURL ) )
      TRACE( "Running " + cCommand )
      nResult := hb_processRun( cCommand, , , @cStdErr, .F. )
      SaveLog( "fetch", cStdOut, cStdErr )
      IF nResult != 0
         OutStd( "E: Error fetching " + cArchiveURL + OSNL )
         RETURN .F.
      ENDIF

      /* Extract */
      IF cExtractor != NIL /* May not need extraction */
         cCommand := hb_strFormat( "%s " + cExtractorArgs + " %s",                        ;
                     cExtractor, CombinePath( s_cTempDir, cFileName ) )
         TRACE( "Running " + cCommand )
         nResult := hb_processRun( cCommand, , @cStdOut, @cStdErr, .F. )
         SaveLog( "extract", cStdOut, cStdErr )
         IF nResult != 0
            OutStd( "E: Error extracting " + cFileName + OSNL )
            RETURN .F.
         ENDIF
      ELSE
         cExtractedFileName := cFileName
      ENDIF

      /* Unarchive */
      cCommand := hb_strFormat( "%s " + cArchiverArgs + " %s",                            ;
                  cArchiver, CombinePath( s_cTempDir, cExtractedFileName ) )
      TRACE( "Running " + cCommand )
      cCWD := hb_CurDrive() + hb_osDriveSeparator() + OSPS + CurDir()
      DirChange( CombinePath( s_cTempDir, "root" ) )
      nResult := hb_processRun( cCommand, , @cStdOut, @cStdErr, .F. )
      DirChange( cCWD )
      SaveLog( "archive", cStdOut, cStdErr )
      IF nResult != 0
         OutStd( "E: Error unarchiving " + cFileName + OSNL )
         RETURN .F.
      ENDIF
   ENDIF

   RETURN .T.

PROCEDURE SaveLog( cFNTemplate, cStdOut, cStdErr )

   LOCAL nLogFD

   nLogFD := FCreate( CombinePath( s_cTempDir, cFNTemplate + ".log" ) )

   IF cStdOut != NIL
      FWrite( nLogFd, "stdout:" + OSNL )
      FWrite( nLogFD, cStdOut )
   ENDIF
   IF cStdErr != NIL
      FWrite( nLogFd, "stderr:" + OSNL )
      FWrite( nLogFD, cStdErr )
   ENDIF
   FClose( nLogFD )

   RETURN

PROCEDURE Usage( nExitVal )

   OutStd( "Usage: " + FN_NameExtGet( hb_ProgName() ) + " [-h|-help|-rediff]" + OSNL )
   OutStd( "       Documentation is provided in the source code." + OSNL )
   ErrorLevel( nExitVal )
   QUIT

   RETURN

/* from hbmk2 */

STATIC FUNCTION FN_DirGet( cFileName )

   LOCAL cDir

   hb_FNameSplit( cFileName, @cDir )

   RETURN cDir

STATIC FUNCTION FN_NameGet( cFileName )

   LOCAL cName

   hb_FNameSplit( cFileName,, @cName )

   RETURN cName

STATIC FUNCTION FN_NameExtGet( cFileName )

   LOCAL cName, cExt

   hb_FNameSplit( cFileName,, @cName, @cExt )

   RETURN hb_FNameMerge( NIL, cName, cExt )

STATIC FUNCTION FN_ExtGet( cFileName )

   LOCAL cExt

   hb_FNameSplit( cFileName,,, @cExt )

   RETURN cExt

STATIC FUNCTION URL_GetFileName( cURL )

   LOCAL aComponents
   LOCAL cName
   LOCAL nIdx

   aComponents := hb_ATokens( cURL, "/" )
   nIdx := Len( aComponents )

   IF nIdx < 4 /* now what */
      RETURN .F.
   ENDIF

   cName := aComponents[ nIdx ]
   cName := iif( "?" $ cName, Left( cName, At( "?", cName ) - 1 ), cName ) /* strip params */

   DO WHILE !( "." $ cName )
      cName := aComponents[ --nIdx ]
      IF nIdx < 4 /* don't drain all components */
         RETURN .F.
      ENDIF
   ENDDO

   RETURN cName

STATIC FUNCTION hb_FileTran( cFileName )

   LOCAL cFileContent
   LOCAL cTransformedContent
   LOCAL aChange
   LOCAL cChangeFrom
   LOCAL cChangeTo

   cFileContent := hb_MemoRead( cFileName )

   /* CRLF -> LF */
   cTransformedContent := StrTran( cFileContent, Chr( 13 ) + Chr( 10 ), Chr( 10 ) )

   /* LF -> native */
   cTransformedContent := StrTran( cTransformedContent, Chr( 10 ), OSNL )

   FOR EACH aChange IN s_aChangeMap

      /* This is a shot in the dark. Haru works with this transform,
       * but other components may very well need different handling. */
      cChangeFrom := FN_NameExtGet( aChange[ 1 ] )
      cChangeTo := aChange[ 2 ]

      /* Local-style includes */
      cTransformedContent := StrTran( cTransformedContent,                                ;
                                      Chr( 34 ) + cChangeFrom + Chr( 34 ),                ;
                                      Chr( 34 ) + cChangeTo + Chr( 34 ) )

      /* System-style include */
      cTransformedContent := StrTran( cTransformedContent,                                ;
                                      "<" + cChangeFrom + ">",                            ;
                                      "<" + cChangeTo + ">" )

   NEXT

   RETURN hb_MemoWrit( cFileName, cTransformedContent )

STATIC FUNCTION FN_Escape( cFileName )
#if defined( __PLATFORM__UNIX )
   RETURN cFileName
#else
   RETURN Chr( 34 ) + cFileName + Chr( 34 )
#endif

/*
 * vim: ts=3 expandtab
 */
