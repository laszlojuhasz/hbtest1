/*
 * $Id$
 */

/*
 * Harbour Project source code:
 * Some dbf structure related functions
 *
 * Copyright 2000 Alexander Kresin <alex@belacy.belgorod.su>
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

#include "extend.h"
#include "rddapi.h"
 
HARBOUR HB_FIELDTYPE( void )
{
   USHORT uiField;
   LPFIELD pField;
   AREAP pArea;

   uiField = hb_parni( 1 );
   pArea = ( AREAP ) hb_rddGetCurrentWorkAreaPointer();
   pField = pArea->lpFields + uiField - 1;

   hb_retc( ( char * ) &( pField->uiType ) );
}

HARBOUR HB_FIELDSIZE( void )
{
   USHORT uiField;
   LPFIELD pField;
   AREAP pArea;

   uiField = hb_parni( 1 );
   pArea = ( AREAP ) hb_rddGetCurrentWorkAreaPointer();
   pField = pArea->lpFields + uiField - 1;

   hb_retni( pField->uiLen );
}

HARBOUR HB_FIELDDECI( void )
{
   USHORT uiField;
   LPFIELD pField;
   AREAP pArea;

   uiField = hb_parni( 1 );
   pArea = ( AREAP ) hb_rddGetCurrentWorkAreaPointer();
   pField = pArea->lpFields + uiField - 1;

   hb_retni( pField->uiDec );
}
