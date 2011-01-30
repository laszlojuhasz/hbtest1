/*
 * $Id$
 */

/*
 * Harbour Project source code:
 * HB_BASE64DECODE() function
 *
 * Copyright 2011 Viktor Szakats (harbour.01 syenar.hu)
 * www - http://harbour-project.org
 *
 * [ base64_decode_* functions are part of the libb64 project, and has
 *   been placed in the public domain. Author: Chris Venter
 *   For details, see http://sourceforge.net/projects/libb64 ]
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

typedef enum
{
   step_a, step_b, step_c, step_d
} base64_decodestep;

typedef struct
{
   base64_decodestep step;
   char plainchar;
} base64_decodestate;

static int base64_decode_value( char value_in )
{
   static const char s_decoding[] =
   {
      62, -1, -1, -1, 63, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, -1, -1, -1, -2, -1,
      -1, -1,  0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14, 15, 16, 17,
      18, 19, 20, 21, 22, 23, 24, 25, -1, -1, -1, -1, -1, -1, 26, 27, 28, 29, 30, 31,
      32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51
   };

   value_in -= 43;
   if( value_in < 0 || value_in > ( char ) HB_SIZEOFARRAY( s_decoding ) )
      return -1;

   return s_decoding[ ( int ) value_in ];
}

static int base64_decode_block( const char * code_in, const int length_in, char * pszPlainttextOut )
{
   base64_decodestep    step;
   char                 plainchar;

   const char *         codechar = code_in;
   char *               pszPlainchar = pszPlainttextOut;
   char                 fragment;

   step       = step_a;
   plainchar  = 0;

   *pszPlainchar = plainchar;

   switch( step )
   {
      for( ;; )
      {
         case step_a:
            do
            {
               if( codechar == code_in + length_in )
               {
                  step       = step_a;
                  plainchar  = *pszPlainchar;
                  return pszPlainchar - pszPlainttextOut;
               }
               fragment = ( char ) base64_decode_value( *codechar++ );
            }
            while( fragment < 0 );
            *pszPlainchar = ( fragment & 0x03f ) << 2;
         case step_b:
            do
            {
               if( codechar == code_in + length_in )
               {
                  step       = step_b;
                  plainchar  = *pszPlainchar;
                  return pszPlainchar - pszPlainttextOut;
               }
               fragment = ( char ) base64_decode_value( *codechar++ );
            }
            while( fragment < 0 );
            *pszPlainchar++ |= ( fragment & 0x030 ) >> 4;
            *pszPlainchar    = ( fragment & 0x00f ) << 4;
         case step_c:
            do
            {
               if( codechar == code_in + length_in )
               {
                  step       = step_c;
                  plainchar  = *pszPlainchar;
                  return pszPlainchar - pszPlainttextOut;
               }
               fragment = ( char ) base64_decode_value( *codechar++ );
            }
            while( fragment < 0 );
            *pszPlainchar++ |= ( fragment & 0x03c ) >> 2;
            *pszPlainchar    = ( fragment & 0x003 ) << 6;
         case step_d:
            do
            {
               if( codechar == code_in + length_in )
               {
                  step       = step_d;
                  plainchar  = *pszPlainchar;
                  return pszPlainchar - pszPlainttextOut;
               }
               fragment = ( char ) base64_decode_value( *codechar++ );
            }
            while( fragment < 0 );
            *pszPlainchar++ |= ( fragment & 0x03f );
      }
   }
   /* control should not reach here */
   return pszPlainchar - pszPlainttextOut;
}

HB_FUNC( HB_BASE64DECODE )
{
   HB_SIZE len = hb_parclen( 1 );

   if( len <= INT_MAX ) /* TOFIX */
   {
      char * code = ( char * ) hb_xgrab( ( ( ( ( len - 1 ) * 3 ) / 4 ) + 1 ) * sizeof( char ) );

      base64_decode_block( hb_parcx( 1 ), len, code );

      hb_retc_buffer( code );
   }
   else
      hb_retc_null();
}
