/*
 * $Id$
 */

/*
 * Harbour Project source code:
 * TBROWSEDB() function
 *
 * Copyright 1999 Paul Tucker <ptucker@sympatico.ca>
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

#include "hbsetup.ch"

FUNCTION TBrowseDB( nTop, nLeft, nBottom, nRight )

   LOCAL oBrowse := TBrowseNew( nTop, nLeft, nBottom, nRight )

   oBrowse:SkipBlock     := { | nRecs | Skipped( nRecs ) }
   oBrowse:GoTopBlock    := { || dbGoTop() }
   oBrowse:GoBottomBlock := { || dbGoBottom() }

   RETURN oBrowse

STATIC FUNCTION Skipped( nRecs )

   LOCAL nSkipped := 0

   IF LastRec() != 0
      IF nRecs == 0
         dbSkip( 0 )
      ELSEIF nRecs > 0 .AND. RecNo() != LastRec() + 1
         DO WHILE nSkipped < nRecs
            dbSkip( 1 )
            IF Eof()
               dbSkip( -1 )
               EXIT
            ENDIF
            nSkipped++
         ENDDO
      ELSEIF nRecs < 0
         DO WHILE nSkipped > nRecs
            dbSkip( -1 )
            IF Bof()
               EXIT
            ENDIF
            nSkipped--
         ENDDO
      ENDIF
   ENDIF

   RETURN nSkipped

