/*
 * $Id$
 */

/*
 * Harbour Project source code:
 *    Harbour i18n .pot/.hbl file manger
 *
 * Copyright 2009 Przemyslaw Czerpak <druzus / at / priv.onet.pl>
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

#include "hbgtinfo.ch"

#define _HB_I18N_MARGE  1
#define _HB_I18N_GENHBL 2

PROCEDURE Main( ... )
   LOCAL aParams, aFiles
   LOCAL cFileOut
   LOCAL lError, lEmpty, lQuiet
   LOCAL nMode, n
   LOCAL param

   aParams := hb_aParams()
   IF Empty( aParams )
      Syntax()
   ENDIF

   lError := lEmpty := lQuiet := .F.
   aFiles := {}
   nMode := 0
   FOR n := 1 TO Len( aParams )
      IF aParams[ n ] = "-"
         param := SubStr( aParams[ n ], 2 )
         IF param == "m"
            IF nMode != 0
               lError := .T.
            ELSE
               nMode := _HB_I18N_MARGE
            ENDIF
         ELSEIF param == "g"
            IF nMode != 0
               lError := .T.
            ELSE
               nMode := _HB_I18N_GENHBL
            ENDIF
         ELSEIF param = "o"
            IF !Empty( param := SubStr( param, 2 ) )
               cFileOut := param
            ELSEIF n < Len( aParams ) .AND. aParams[ n + 1 ] != "-"
               cFileOut := aParams[ ++n ]
            ELSE
               lError := .T.
            ENDIF
         ELSEIF param == "e"
            lEmpty := .T.
         ELSEIF param == "q"
            lQuiet := .T.
         ELSE
            lError := .T.
         ENDIF
      ELSE
         AAdd( aFiles, aParams[ n ] )
      ENDIF
      IF lError
         Syntax()
      ENDIF
   NEXT

   IF nMode == 0 .OR. Empty( aFiles )
      Syntax()
   ENDIF

   IF !lQuiet
      Logo()
   ENDIF

   IF nMode == _HB_I18N_MARGE
      Merge( aFiles, cFileOut )
   ELSEIF nMode == _HB_I18N_GENHBL
      GenHbl( aFiles, cFileOut, lEmpty )
   ENDIF

   RETURN


STATIC FUNCTION HBRawVersion()
   RETURN StrTran( Version(), "Harbour ", "" )

STATIC PROCEDURE Logo()

   OutStd( "Harbour i18n .pot/.hbl file manger " + HBRawVersion() + HB_OSNewLine() +;
           "Copyright (c) 2009, Przemyslaw Czerpak" + HB_OSNewLine() + ;
           "http://www.harbour-project.org/" + HB_OSNewLine() +;
           HB_OSNewLine() )
   RETURN


STATIC PROCEDURE Syntax()

   Logo()
   OutStd( "Syntax: hbi18n -m | -g [-o<outfile>] [-e] [-q] <files1[.pot] ...>" + HB_OSNewLine() + ;
           HB_OSNewLine() + ;
           "    -m          merge given .pot files" + HB_OSNewLine() + ;
           "    -g          generate .hbl file from given .pot files" + HB_OSNewLine() + ;
           "    -o<outfile> output file name" + HB_OSNewLine() + ;
           "                default is first .pot file name with" + HB_OSNewLine() + ;
           "                .po_ (merge) or .hbl extension" + HB_OSNewLine() + ;
           "    -e          do not strip empty translation rules from .hbl files" + HB_OSNewLine() + ;
           "    -q          quiet mode" + HB_OSNewLine() + ;
           HB_OSNewLine() )

   IF hb_gtInfo( HB_GTI_ISGRAPHIC )
      OutStd( "Press any key to continue..." )
      Inkey( 0 )
   ENDIF
   ErrorLevel( 1 )
   QUIT


STATIC PROCEDURE ErrorMsg( cErrorMsg )

   OutStd( "error: " + StrTran( cErrorMsg, ";", hb_osNewLine() ) + HB_OSNewLine() )

   IF hb_gtInfo( HB_GTI_ISGRAPHIC )
      OutStd( "Press any key to continue..." )
      Inkey( 0 )
   ENDIF
   ErrorLevel( 1 )
   QUIT


STATIC FUNCTION FileExt( cFile, cDefExt, lForce )
   LOCAL cPath, cName, cExt

   hb_FNameSplit( cFile, @cPath, @cName, @cExt )
   IF lForce .OR. Empty( cExt )
      cFile := hb_FNameMerge( cPath, cName, cDefExt )
   ENDIF
   RETURN cFile


STATIC FUNCTION LoadFiles( aFiles )
   LOCAL aTrans, aTrans2
   LOCAL cErrorMsg
   LOCAL n

   aTrans := __I18N_potArrayLoad( aFiles[ 1 ], @cErrorMsg )
   IF aTrans == NIL
      ErrorMsg( cErrorMsg )
   ENDIF
   FOR n := 2 TO Len( aFiles )
      aTrans2 := __I18N_potArrayLoad( aFiles[ n ], @cErrorMsg )
      IF aTrans2 == NIL
         ErrorMsg( cErrorMsg )
      ENDIF
      __I18N_potArrayJoin( aTrans, aTrans2 )
   NEXT

   RETURN aTrans

STATIC PROCEDURE Merge( aFiles, cFileOut )
   LOCAL cErrorMsg

   IF Empty( cFileOut )
      cFileOut := FileExt( aFiles[ 1 ], ".pot", .T. )
   ELSE
      cFileOut := FileExt( cFileOut, ".pot", .F. )
   ENDIF

   IF !__I18N_potArraySave( cFileOut, LoadFiles( aFiles ), @cErrorMsg )
      ErrorMsg( cErrorMsg )
   ENDIF

   RETURN


STATIC PROCEDURE GenHbl( aFiles, cFileOut, lEmpty )
   LOCAL cHblBody
   LOCAL pI18N

   IF Empty( cFileOut )
      cFileOut := FileExt( aFiles[ 1 ], ".hbl", .T. )
   ELSE
      cFileOut := FileExt( cFileOut, ".hbl", .F. )
   ENDIF

   pI18N := __I18N_hashTable( __I18N_potArrayToHash( LoadFiles( aFiles ), ;
                                                     lEmpty ) )
   cHblBody := HB_I18N_SaveTable( pI18N )
   IF !hb_memoWrit( cFileOut, cHblBody )
      ErrorMsg( "cannot create file: " + cFileOut )
   ENDIF

   RETURN
