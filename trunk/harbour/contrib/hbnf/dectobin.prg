/*
 * $Id$
 */

/*
 * File......: dectobin.prg
 * Author....: Greg Lief
 * CIS ID....: 72460,1760
 *
 * This function is an original work by Mr. Grump and is placed in the
 * public domain.
 *
 * Modification history:
 * ---------------------
 *
 *    Rev 1.2   15 Aug 1991 23:03:22   GLENN
 * Forest Belt proofread/edited/cleaned up doc
 *
 *    Rev 1.1   14 Jun 1991 19:51:30   GLENN
 * Minor edit to file header
 *
 *    Rev 1.0   01 Apr 1991 01:01:06   GLENN
 * Nanforum Toolkit
 *
 */

#ifdef FT_TEST

PROCEDURE Main()

   LOCAL X

   FOR X := 1 TO 255
      QOut( FT_DEC2BIN( x ) )
   NEXT

   RETURN

#endif

FUNCTION FT_DEC2BIN( x )

   LOCAL i, buffer := { "0", "0", "0", "0", "0", "0", "0", "0" }

   FOR i := 8 TO 1 step - 1
      IF x >= 2 ^ ( i - 1 )
         x -= 2 ^ ( i - 1 )
         buffer[ 9 - i ] := "1"
      ENDIF
   NEXT

   RETURN buffer[ 1 ] + buffer[ 2 ] + buffer[ 3 ] + buffer[ 4 ] + ;
      buffer[ 5 ] + buffer[ 6 ] + buffer[ 7 ] + buffer[ 8 ]
