/*
 * $Id$

   Copyright(C) 1999 by Antonio Linares.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published
   by the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful, but
   WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR
   PURPOSE.  See the GNU General Public License for more details.

   You should have received a copy of the GNU General Public
   License along with this program; if not, write to:

   The Free Software Foundation, Inc.,
   675 Mass Ave, Cambridge, MA 02139, USA.

   You can contact me at: alinares@fivetech.com
 */

// Standard Harbour ErrorSys system

#include "error.ch"

#define ISCHAR(var)     (ValType(var) == "C")
#define ISNUM(var)      (ValType(var) == "N")

//----------------------------------------------------------------------------//

init procedure ClipInit

   // public getlist := {}    TODO!

   ErrorSys()

return

//----------------------------------------------------------------------------//

static function DefError( oError )
   LOCAL cMessage

   local cInfo := ""
   local n := 2

   cMessage := ErrorMessage(oError)
   IF !Empty(oError:osCode)
      cMessage += " (DOS Error " + LTrim(Str(oError:osCode)) + ")"
   ENDIF

   QOut( cMessage)

   do while ! Empty( ProcName( n ) )
      QOut("Called from " + ProcName( n ) + ;
               "(" + AllTrim( Str( ProcLine( n++ ) ) ) + ")")
   enddo

   // TOFIX: Removing ErrorLevel() call will cause a VM error
   //        don't know why [vszel]
   ErrorLevel(1)
   QUIT

return .F.

//----------------------------------------------------------------------------//

procedure ErrorSys

   ErrorBlock( { | oError | DefError( oError ) } )

return

// [vszel]

STATIC FUNCTION ErrorMessage(oError)
   LOCAL cMessage

   // start error message
   cMessage := iif( oError:severity > ES_WARNING, "Error", "Warning" ) + " "

   // add subsystem name if available
   IF ISCHAR(oError:subsystem)
      cMessage += oError:subsystem()
   ELSE
      cMessage += "???"
   ENDIF

   // add subsystem's error code if available
   IF ISNUM(oError:subCode)
      cMessage += "/" + LTrim(Str(oError:subCode))
   ELSE
      cMessage += "/???"
   ENDIF

   // add error description if available
   IF ISCHAR(oError:description)
      cMessage += "  " + oError:description
   ENDIF

   // add either filename or operation
   DO CASE
   CASE !Empty(oError:filename)
      cMessage += ": " + oError:filename
   CASE !Empty(oError:operation)
      cMessage += ": " + oError:operation
   ENDCASE

   RETURN cMessage

//----------------------------------------------------------------------------//
