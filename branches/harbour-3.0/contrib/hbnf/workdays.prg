/*
 * $Id$
 */

/*
 * File......: workdays.prg
 * Author....: John F. Kaster
 * CIS_ID....: 71510,3321
 *
 * The functions contained herein are the original work of John Kaster
 * and are placed in the public domain.
 *
 * Modification history:
 * ---------------------
 *
 *    Rev 1.2   07 Mar 1992 22:15:06   GLENN
 * Mark K. Zechiel discovered a bug where the incorrect number of
 * workdays was reported when <dStart> was a Tuesday through Friday and
 * dStop was a multiple of 7 days away from dStart (i.e., 7, or 14, or
 * 21, etc).  Fixed.
 *
 *    Rev 1.1   15 Aug 1991 23:05:48   GLENN
 * Forest Belt proofread/edited/cleaned up doc
 *
 *    Rev 1.0   12 Jun 1991 01:33:10   GLENN
 * Initial revision.
 *
 */

#ifdef FT_TEST
  function main( cStart, cStop )
     return qout( ft_workdays( ctod( cStart ), ctod( cStop ) ) )
#endif

FUNCTION FT_WorkDays( dStart, dStop )
   LOCAL nWorkDays := 0, nDays, nAdjust

   IF dStart # NIL .AND. dStop # NIL
      IF dStart # dStop
         IF dStart > dStop   // Swap the values
            nAdjust    := dStop
            dStop    := dStart
            dStart    := nAdjust
         ENDIF

         IF ( nDays := Dow( dStart ) ) == 1 // Sunday (change to next Monday)
            dStart++
         ELSEIF nDays == 7 // Saturday (change to next Monday)
            dStart += 2
         ENDIF

         IF ( nDays := Dow( dStop ) ) == 1 // Sunday (change to prev Friday)
            dStop -= 2
         ELSEIF nDays == 7 // Saturday (change to prev Friday)
            dStop--
         ENDIF

         nAdjust := ( nDays := dStop - dStart + 1 ) % 7

         IF Dow( dStop ) + 1 < Dow( dStart ) // Weekend adjustment
            nAdjust -= 2
         ENDIF

         nWorkDays := Int( nDays / 7 ) * 5 + nAdjust

      ELSEIF ( Dow( dStart ) # 1 .AND. Dow( dStart ) # 7 )

         nWorkDays := 1

      ENDIF

   ENDIF

RETURN ( IIF(nWorkDays>0,nWorkDays,0) )
