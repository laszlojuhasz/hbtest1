/*
 * Harbour Class TBColumn
 * Copyright(C) 1999 by Antonio Linares <alinares@fivetech.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published
 * by the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR
 * PURPOSE.  See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public
 * License along with this program; if not, write to:
 *
 * The Free Software Foundation, Inc.,
 * 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include "classes.ch"

CLASS TBColumn

   DATA Block      // Code block to retrieve data for the column
   DATA Cargo      // User-definable variable
   DATA ColorBlock // Code block that determines color of data items
   DATA ColSep     // Column separator character
   DATA DefColor   // Array of numeric indexes into the color table
   DATA Footing    // Column footing
   DATA FootSep    // Footing separator character
   DATA Heading    // Column heading
   DATA HeadSep    // Heading separator character
   DATA Width      // Column display width

   METHOD New()    // Constructor

ENDCLASS

METHOD New() CLASS TBColumn

   ::DefColor = { 1, 2 }
   ::FootSep  = ""

return Self

function TBColumnNew( cHeading, bBlock )

   local oCol := TBColumn():New()

   oCol:Heading = cHeading
   oCol:block   = bBlock
   oCol:Width   = If( cHeading != nil, Len( cHeading ), Len( Eval( bBlock ) ) )

return oCol

