/*
 * $Id$
 */

/*
 * Author....: Robert A. DiFalco
 * CIS ID....: ?
 *
 * This is an original work by Robert DiFalco and is placed in the
 * public domain.
 *
 * Modification history:
 * ---------------------
 *
 *    Rev 1.3   17 Aug 1991 15:32:56   GLENN
 * Don Caton fixed some spelling errors in the doc
 *
 *    Rev 1.2   15 Aug 1991 23:03:42   GLENN
 * Forest Belt proofread/edited/cleaned up doc
 *
 *    Rev 1.1   14 Jun 1991 19:51:58   GLENN
 * Minor edit to file header
 *
 *    Rev 1.0   01 Apr 1991 01:01:28   GLENN
 * Nanforum Toolkit
 *
 */

FUNCTION FT_HEX2DEC( cHexNum )

   LOCAL n, nDec := 0, nHexPower := 1

   FOR n := Len( cHexNum ) TO 1 STEP -1
      nDec += ( At( SubStr( Upper( cHexNum ), n, 1 ), "0123456789ABCDEF" ) - 1 ) * nHexPower
      nHexPower *= 16
   NEXT

   RETURN nDec
