/*
 * $Id$
 */

/*
 * Harbour Project source code:
 * POSUPPER() CA-Tools function
 *
 * Copyright 2000 Victor Szakats <info@szelvesz.hu>
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

#include <ctype.h>

#include "hbapi.h"

HB_FUNC( POSUPPER )
{
   BYTE * pbyString = ( BYTE * ) hb_parc( 1 );
   ULONG ulLen = hb_parclen( 1 );
   ULONG ulPos;

   for( ulPos = 0; ulPos < ulLen; ulPos++ )
   {
      if( isupper( pbyString[ ulPos ] ) )
         hb_retnl( ulPos + 1 );
   }

   hb_retnl( 0 );
}

