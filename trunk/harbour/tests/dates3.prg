/*
 * $Id$
 */

// Testing Harbour dates management.

#include "set.ch"

PROCEDURE Main()

   LOCAL dDate, i

   Set( _SET_DATEFORMAT, "dd/mm/yyyy" )
   dDate := hb_SToD( "19990525" )

   ? dDate, DoW( dDate )

   ? LastMonday( dDate )

   dDate += 3
   ? dDate, DoW( dDate )

   dDate += 4
   ? dDate, DoW( dDate )

   Set( _SET_DATEFORMAT, "mm/dd/yyyy" )
   dDate := hb_SToD( "19990525" )

   ? dDate, DoW( dDate )

   ? LastMonday( dDate )

   dDate += 3
   ? dDate, DoW( dDate )

   dDate += 4
   ? dDate, DoW( dDate )

   ?
   dDate := Date ()
   FOR i := 1 TO 7
      ? dDate, DoW( dDate )
      dDate++
   NEXT
   ? CToD( "" ), DoW( CToD( "" ) )

   RETURN

// Like NG's sample

FUNCTION LastMonday( dDate )

   RETURN dDate - DoW( dDate ) + 2
