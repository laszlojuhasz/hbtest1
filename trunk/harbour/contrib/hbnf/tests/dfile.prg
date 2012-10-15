/*
 * $Id$
 */

#require "hbnf"

#include "inkey.ch"

PROCEDURE Main()

   LOCAL cInFile   := __FILE__
   LOCAL nKey
   LOCAL NNCOLOR   := 7
   LOCAL NHCOLOR   := 15
   LOCAL NCOLSKIP  := 5
   LOCAL NRMARGIN  := 132
   LOCAL CEXITKEYS := PadR( "AABBC", 12 )
   LOCAL LBROWSE   := .F.
   LOCAL NSTART    := 1
   LOCAL NBUFFSIZE := 4096
   LOCAL GetList := {}

   LOCAL aExitKeys
   LOCAL tmp

   @ 0, 0 CLEAR

   @ 0, 0  SAY "ENTER FILENAME: "   GET CINFILE
   @ 1, 0  SAY "    FOREGROUND: "   GET NNCOLOR   PICTURE "999"
   @ 2, 0  SAY "     HIGHLIGHT: "   GET NHCOLOR   PICTURE "999"
   @ 3, 0  SAY "     EXIT KEYS: "   GET CEXITKEYS
   @ 4, 0  SAY "   BUFFER SIZE: "   GET NBUFFSIZE PICTURE "9999"
   @ 1, 40 SAY "COLUMN INCREMENT: " GET NCOLSKIP  PICTURE "999"
   @ 2, 40 SAY "   MAX LINE SIZE: " GET NRMARGIN  PICTURE "999"
   @ 3, 40 SAY "     BROWSE MODE? " GET LBROWSE   PICTURE "Y"

   READ

   /*
    * REMEMBER A WINDOW WILL BE ONE SIZE LESS AND GREATER THAN THE PASSED COORD.'S
    *
    * THE 9TH PARAMETER CONTAINS THE KEYS THAT THE ROUTINE WILL TERMINATE ON
    *
    */

   aExitKeys := {}
   FOR EACH tmp IN cExitKeys
      AAdd( aExitKeys, hb_keyCode( tmp ) )
   NEXT

   AAdd( aExitKeys, K_F3 )

   @ 4, 9 TO 11, 71

   FT_DFSETUP( cInFile, 5, 10, 10, 70, nStart, ;
      nNColor, nHColor, aExitKeys, ;
      lBrowse, nColSkip, nRMargin, nBuffSize )

   nKey := FT_DISPFILE()

   FT_DFCLOSE()

   @ 20, 0 SAY "Key pressed was: " + "[" + hb_keyChar( nKey ) + "] (" + hb_ntos( nKey ) + ")"

   RETURN
