/*
 * $Id$
 */

/*
   Guess a number

   Date       : 1999/04/22

   My first application (big word) written in Harbour

   Written by Eddie Runia <eddie@runia.com>
   www - http://harbour-project.org

   Placed in the public domain
*/

EXTERN OutStd

PROCEDURE Main()

  local flGuessed
  local CRLF  := chr(13)+chr(10)
  local nSeed := 241
  local nPick

  QQOut( "Welcome to guess a number....", CRLF )
  QQOut( "You have to guess a number between ",0," and 255", CRLF )

  do while Upper( Read( "Continue Y/N : " ) ) == "Y"
    nSeed := (( nSeed * 11) + 5) % 256
    flGuessed := 0

    do while flGuessed == 0

       nPick := Val( Read( "Value : " ) )
       if nPick > 255
         QQOut( "More than 255", CRLF )
       elseif nPick < 0
         QQOut( "Less than 0", CRLF )
       elseif nPick > nSeed
         QQOut( "Try lower", CRLF )
       elseif nPick < nSeed
         QQOut( "Try higher", CRLF )
       else
         QQOut( "Congratulations, you've guessed the number", CRLF, CRLF )
         flGuessed := 1
       endif

     enddo

   enddo

   RETURN

FUNCTION Read( cPrompt )        /* Simple read function */
   RETURN __Accept( cPrompt )
