/*
 * $Id$
 */

/*
 * Author....: Forest Belt, Computer Diagnostic Services, Inc.
 * CIS ID....: ?
 *
 * This is an original work by Forest Belt and is placed in the
 * public domain.
 *
 * Modification history:
 * ---------------------
 *
 *    Rev 1.2   15 Aug 1991 23:03:00   GLENN
 * Forest Belt proofread/edited/cleaned up doc
 *
 *    Rev 1.1   14 Jun 1991 19:51:10   GLENN
 * Minor edit to file header
 *
 *    Rev 1.0   01 Apr 1991 01:00:48   GLENN
 * Nanforum Toolkit
 *
 */

FUNCTION FT_BYT2HEX( cByte )

   LOCAL cHexTable := "0123456789ABCDEF"
   LOCAL xHexString

   IF HB_ISSTRING( cByte )
      xHexString := SubStr( cHexTable, Int( hb_BCode( cByte ) / 16 ) + 1, 1 ) + ;
                    SubStr( cHexTable, Int( hb_BCode( cByte ) % 16 ) + 1, 1 ) + ;
                    "h"
   ELSE
      xHexString := NIL
   ENDIF

   RETURN xHexString
