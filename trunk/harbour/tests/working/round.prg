//
// $Id$
//

// Testing Harbour rounding.
/* Harbour Project source code
   http://www.Harbour-Project.org/
   Copyright 1999 David G. Holm <dholm@jsd-llc.com>
   See doc/hdr_tpl.txt, Version 1.2 or later, for licensing terms.
*/

function main()
local n, value := -5
local cOs := OS(), cNewLine

   IF "OS/2" $ cOs .OR. "WIN" $ cOs .OR. "DOS" $cOs
      cNewLine := CHR( 13 ) + CHR( 10 )
   ELSE
      cNewLine := CHR( 10 )
   END IF

   for n := 1 to 100
      OUTSTD(cNewLine)
      OUTSTD(value)
      OUTSTD(round(value, 3))
      OUTSTD(round(value, 2))
      OUTSTD(round(value, 1))
      OUTSTD(round(value, 0))
      value += 0.001
   next

return nil
