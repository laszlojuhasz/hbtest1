/*
 * $Id$
 */

/*
 * Harbour Project source code:
 * EMPTY() function
 *
 * Copyright 1999 {list of individual authors and e-mail addresses}
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

#include "hbapi.h"
#include "hbapiitm.h"

HB_FUNC( EMPTY )
{
   PHB_ITEM pItem = hb_param( 1, IT_ANY );

   switch( hb_itemType( pItem ) & ~IT_BYREF )
   {
      case IT_ARRAY:
         hb_retl( hb_arrayLen( pItem ) == 0 );
         break;

      case IT_STRING:
      case IT_MEMO:
         hb_retl( hb_strEmpty( hb_itemGetCPtr( pItem ), hb_itemGetCLen( pItem ) ) );
         break;

      case IT_INTEGER:
         hb_retl( hb_itemGetNI( pItem ) == 0 );
         break;

      case IT_LONG:
         hb_retl( hb_itemGetNL( pItem ) == 0 );
         break;

      case IT_DOUBLE:
         hb_retl( hb_itemGetND( pItem ) == 0.0 );
         break;

      case IT_DATE:
         hb_retl( hb_itemGetDL( pItem ) == 0 );
         break;

      case IT_LOGICAL:
         hb_retl( ! hb_itemGetL( pItem ) );
         break;

      case IT_BLOCK:
         hb_retl( FALSE );
         break;

      default:
         hb_retl( TRUE );
         break;
   }
}

