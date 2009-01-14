/*
 * $Id$
 */

/*
 * xHarbour Project source code:
 * TIP Class oriented Internet protocol library
 *
 * Copyright 2003 Giancarlo Niccolai <gian@niccolai.ws>
 *
 * www - http://www.harbour-project.org
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this software; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 59 Temple Place, Suite 330,
 * Boston, MA 02111-1307 USA (or visit the web site http://www.gnu.org/).
 *
 * As a special exception, the Harbour Project gives permission for
 * additional uses of the text contained in its release of Harbour.
 *
 * The exception is that, if you link the Harbour libraries with other
 * files to produce an executable, this does not by itself cause the
 * resulting executable to be covered by the GNU General Public License.
 * Your use of that executable is in no way restricted on account of
 * linking the Harbour library code into it.
 *
 * This exception does not however invalidate any other reasons why
 * the executable file might be covered by the GNU General Public License.
 *
 * This exception applies only to the code released by the Harbour
 * Project under the name Harbour.  If you copy code from other
 * Harbour Project or Free Software Foundation releases into a copy of
 * Harbour, as the General Public License permits, the exception does
 * not apply to the code that you add in this way.  To avoid misleading
 * anyone as to the status of such modified files, you must delete
 * this exception notice from them.
 *
 * If you write modifications of your own for Harbour, it is your choice
 * whether to permit this exception to apply to your modifications.
 * If you do not wish that, delete this exception notice.
 *
 */

#include "hbapi.h"

HB_FUNC( BUILDUSERPASSSTRING )
{
   char * szUser = hb_parcx( 1 );
   char * szPass = hb_parcx( 2 );
   size_t u_len = strlen( szUser );
   size_t p_len = strlen( szPass );
   char * s = ( char * ) hb_xgrab( u_len + p_len + 3 );

   s[ 0 ] = '\0';
   memcpy( s + 1, szUser, u_len );
   s[ u_len + 1 ] = '\0';
   memcpy( s + u_len + 2, szPass, p_len );

   hb_retclen_buffer( s, u_len + p_len + 2 );
}

HB_FUNC( HB_BASE64 )
{
   ULONG len = hb_parclen( 1 );

   if( len <= INT_MAX ) /* TOFIX */
   {
      char * s = hb_parcx( 1 );
      char * t, * p;

      t = p = ( char * ) hb_xgrab( ( 4 * ( ( len + 2 ) / 3 ) + 1 ) * sizeof( *t ) );

      while( len-- > 0 )
      {
         static const char s_b64chars[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
         int x, y;

         x = *s++;
         *p++ = s_b64chars[ ( x >> 2 ) & 0x3F ];

         if( len-- <= 0 )
         {
            *p++ = s_b64chars[ ( x << 4 ) & 0x3F ];
            *p++ = '=';
            *p++ = '=';
            break;
         }
         y = *s++;
         *p++ = s_b64chars[ ( ( x << 4 ) | ( ( y >> 4 ) & 0x0F ) ) & 0x3F ];

         if( len-- <= 0 )
         {
            *p++ = s_b64chars[ ( y << 2 ) & 0x3F ];
            *p++ = '=';
            break;
         }
         x = *s++;
         *p++ = s_b64chars[ ( ( y << 2 ) | ( ( x >> 6 ) & 3 ) ) & 0x3F ];
         *p++ = s_b64chars[ x & 0x3F ];
      }
      *p = '\0';

      hb_retc_buffer( t );
   }
   else
      hb_retc_null();
}
