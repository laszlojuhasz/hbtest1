/*
 * $Id$
 */

/*
 * Harbour Project source code:
 * BIN2W(), BIN2I(), BIN2L(), BIN2U(), I2BIN(), L2BIN(), W2BIN(), U2BIN() functions
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

/*
 * The following parts are Copyright of the individual authors.
 * www - http://www.harbour-project.org
 *
 * Copyright 1999 Victor Szel <info@szelvesz.hu>
 *    BIN2U()
 *    W2BIN()
 *    U2BIN()
 *
 * See doc/license.txt for licensing terms.
 *
 */

#include "extend.h"
#include "itemapi.h"

HARBOUR HB_BIN2W( void )
{
   PHB_ITEM pItem = hb_param( 1, IT_STRING );

   if( pItem )
   {
      char * pszString = hb_itemGetCPtr( pItem );
      ULONG ulLen = hb_itemGetCLen( pItem );
      BYTE byBuffer[ 2 ];

      byBuffer[ 0 ] = ( ulLen >= 1 ) ? ( BYTE ) pszString[ 0 ] : 0;
      byBuffer[ 1 ] = ( ulLen >= 2 ) ? ( BYTE ) pszString[ 1 ] : 0;

      hb_retni( HB_MKUSHORT( byBuffer[ 0 ],
                             byBuffer[ 1 ] ) );
   }
   else
      hb_retni( 0 );
}

HARBOUR HB_BIN2I( void )
{
   PHB_ITEM pItem = hb_param( 1, IT_STRING );

   if( pItem )
   {
      char * pszString = hb_itemGetCPtr( pItem );
      ULONG ulLen = hb_itemGetCLen( pItem );
      BYTE byBuffer[ 2 ];

      byBuffer[ 0 ] = ( ulLen >= 1 ) ? ( BYTE ) pszString[ 0 ] : 0;
      byBuffer[ 1 ] = ( ulLen >= 2 ) ? ( BYTE ) pszString[ 1 ] : 0;

      hb_retni( HB_MKSHORT( byBuffer[ 0 ],
                            byBuffer[ 1 ] ) );
   }
   else
      hb_retni( 0 );
}

HARBOUR HB_BIN2L( void )
{
   PHB_ITEM pItem = hb_param( 1, IT_STRING );

   if( pItem )
   {
      char * pszString = hb_itemGetCPtr( pItem );
      ULONG ulLen = hb_itemGetCLen( pItem );
      BYTE byBuffer[ 4 ];

      byBuffer[ 0 ] = ( ulLen >= 1 ) ? ( BYTE ) pszString[ 0 ] : 0;
      byBuffer[ 1 ] = ( ulLen >= 2 ) ? ( BYTE ) pszString[ 1 ] : 0;
      byBuffer[ 2 ] = ( ulLen >= 3 ) ? ( BYTE ) pszString[ 2 ] : 0;
      byBuffer[ 3 ] = ( ulLen >= 4 ) ? ( BYTE ) pszString[ 3 ] : 0;

      hb_retnl( HB_MKLONG( byBuffer[ 0 ],
                           byBuffer[ 1 ],
                           byBuffer[ 2 ],
                           byBuffer[ 3 ] ) );
   }
   else
      hb_retnl( 0 );
}

#ifdef HB_COMPAT_XPP

/* NOTE: XBase++ compatible function */

HARBOUR HB_BIN2U( void )
{
   PHB_ITEM pItem = hb_param( 1, IT_STRING );

   if( pItem )
   {
      char * pszString = hb_itemGetCPtr( pItem );
      ULONG ulLen = hb_itemGetCLen( pItem );
      BYTE byBuffer[ 4 ];

      byBuffer[ 0 ] = ( ulLen >= 1 ) ? ( BYTE ) pszString[ 0 ] : 0;
      byBuffer[ 1 ] = ( ulLen >= 2 ) ? ( BYTE ) pszString[ 1 ] : 0;
      byBuffer[ 2 ] = ( ulLen >= 3 ) ? ( BYTE ) pszString[ 2 ] : 0;
      byBuffer[ 3 ] = ( ulLen >= 4 ) ? ( BYTE ) pszString[ 3 ] : 0;

      hb_retnl( HB_MKULONG( byBuffer[ 0 ],
                            byBuffer[ 1 ],
                            byBuffer[ 2 ],
                            byBuffer[ 3 ] ) );
   }
   else
      hb_retnl( 0 );
}

#endif

HARBOUR HB_I2BIN( void )
{
   char szString[ 2 ];

   if( ISNUM( 1 ) )
   {
      SHORT iValue = hb_parni( 1 );

      szString[ 0 ] =   iValue & 0x00FF;
      szString[ 1 ] = ( iValue & 0xFF00 ) >> 8;
   }
   else
   {
      szString[ 0 ] =
      szString[ 1 ] = '\0';
   }

   hb_retclen( szString, 2 );
}

#ifdef HB_COMPAT_XPP

/* NOTE: XBase++ compatible function */

HARBOUR HB_W2BIN( void )
{
   char szString[ 2 ];

   if( ISNUM( 1 ) )
   {
      USHORT uiValue = ( USHORT ) hb_parni( 1 );

      szString[ 0 ] =   uiValue & 0x00FF;
      szString[ 1 ] = ( uiValue & 0xFF00 ) >> 8;
   }
   else
   {
      szString[ 0 ] =
      szString[ 1 ] = '\0';
   }

   hb_retclen( szString, 2 );
}

#endif

HARBOUR HB_L2BIN( void )
{
   char szString[ 4 ];

   if( ISNUM( 1 ) )
   {
      LONG lValue = hb_parnl( 1 );

      szString[ 0 ] =   lValue & 0x000000FF;
      szString[ 1 ] = ( lValue & 0x0000FF00 ) >> 8;
      szString[ 2 ] = ( lValue & 0x00FF0000 ) >> 16;
      szString[ 3 ] = ( lValue & 0xFF000000 ) >> 24;
   }
   else
   {
      szString[ 0 ] =
      szString[ 1 ] =
      szString[ 2 ] =
      szString[ 3 ] = '\0';
   }

   hb_retclen( szString, 4 );
}

#ifdef HB_COMPAT_XPP

/* NOTE: XBase++ compatible function */

HARBOUR HB_U2BIN( void )
{
   char szString[ 4 ];

   if( ISNUM( 1 ) )
   {
      ULONG ulValue = ( ULONG ) hb_parnl( 1 );

      szString[ 0 ] =   ulValue & 0x000000FF;
      szString[ 1 ] = ( ulValue & 0x0000FF00 ) >> 8;
      szString[ 2 ] = ( ulValue & 0x00FF0000 ) >> 16;
      szString[ 3 ] = ( ulValue & 0xFF000000 ) >> 24;
   }
   else
   {
      szString[ 0 ] =
      szString[ 1 ] =
      szString[ 2 ] =
      szString[ 3 ] = '\0';
   }

   hb_retclen( szString, 4 );
}

#endif
