/*
 * $Id$
 */

/*
 * Harbour Project source code:
 * Norton Guide Support Code For FT_HELPC
 *
 * Copyright 2000 Luiz Rafael Culik <Culik@sl.conex.net>
 * www - http://www.harbour-project.org
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version, with one exception:
 *
 * The exception is that if you link the Harbour Runtime Library (HRL)
 * and/or the Harbour Virtual Machine (HVM) with other files to produce
 * an executable, this does not by itself cause the resulting executable
 * to be covered by the GNU General Public License. Your use of that
 * executable is in no way restricted on account of linking the HRL
 * and/or HVM code into it.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA (or visit
 * their web site at http://www.gnu.org/).
 *
 */

#define CRLF HB_OSNewLine()

#include 'hbclass.ch'

*+北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
*+
*+    Class TNortonGuide
*+
*+北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
*+
CLASS TNortonGuide

   DATA cFile
   DATA nHandle
   METHOD New( cFile )

   METHOD WritePar( cPar )
   METHOD WriteLink( clink )
   METHOD CLOSE()
   METHOD WriteParBold( cPar )
   METHOD WriteTitle(  cTopic , cTitle )
ENDCLASS

METHOD NEW( cFile ) CLASS TNortonGuide

   IF VALTYPE( cFile ) <> NIL .AND. VALTYPE( cFile ) == "C"
      Self:cFile   := LOWER( cFile )
      Self:nHandle := FCREATE( Self:cFile )
   ENDIF

RETURN Self

METHOD WritePar( cPar ) CLASS TNortonGuide

   FWRITE( Self:nHandle, HB_OEMTOANSI( cPar ) + CRLF )

RETURN Self

METHOD WriteParBold( cPar ) CLASS TNortonGuide
   Self:WritePar("")
   FWRITE( Self:nHandle, '^b' + HB_OEMTOANSI( cPar ) + '^b^' + CRLF )
   Self:WritePar("")
RETURN Self

METHOD WriteTitle( cTopic, cTitle ) CLASS TNortonGuide

   LOCAL cTemp
   LOCAL nPos
   LOCAL cWrite

   cTopic := HB_OEMTOANSI( cTopic )

   FWRITE( Self:nHandle, "!Short: " + cTopic + CRLF )

   Self:WriteParBold(cTitle )

RETURN Self

METHOD CLOSE() CLASS TNortonGuide

   FCLOSE( Self:nHandle )

RETURN Self

METHOD WriteLink( cLink ) CLASS TNortonGuide

   FWRITE( Self:nHandle, cLink )

RETURN Self

/*  $DOC$
 *  $FUNCNAME$
 *     TNortonGuide()
 *  $CATEGORY$
 *     Harbour Tools
 *  $ONELINER$
 *     Norton Guide Class
 *  $SYNTAX$
 *     oNg:=TNortonGuide():New(<cFile>)
 *  $ARGUMENTS$
 *     <cFile> Name of the Ng Source file to create
 *  $RETURNS$
 *     An  instance of the TNortonGuide Class
 *  $DESCRIPTION$
 *     TNortonGuide() is a class that create the Norton Guide Source
 *     Code of the same name you pass to the constructor.
 *     The class methods are as follows:
 *        New(<cFile>) Create a new instance of the THtml class.
 *        Close() Close the create file
 *        WriteTitle(<cTopic>,<cTitle>) Write the file title
 *        WritePar(<cPar>)   Writes a paragrafer
 *        WriteParBold(<cPar>)   Same as WritePar(), but the text is bold style.
 *        WriteLink(<cLink>)  Write a link to another topic
 *  $EXAMPLES$
 *     FUNCTION MAIN()
 *
 *     LOCAL oNg
 *
 *     oNg := TNortonGuide():New( "ngi\harbour.ngi" )
 *     oNg:WriteTitle( "Harbour Reference Guide" )
 *     oNg:WritePar( "HARBOUR" )
 *     oNg:WriteLink( "OverView" )
 *     oNg:WriteLink( "License" )
 *     
 *     oNg:WritePar( "See the Links Above" )
 *     oNg:Close()
 *     RETURN Nil
 *  $TESTS$
 *  $STATUS$
 *     R
 *  $COMPLIANCE$
 *     This is a new Harbour Tools class
 *  $PLATFORMS$
 *     ALL
 *  $FILES$
 *  $SEEALSO$
 *     TCLASS()
 *  $END$
 */
