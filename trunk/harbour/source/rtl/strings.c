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

/* Harbour Project source code
   http://www.Harbour-Project.org/
   The following functions are Copyright 1999 Victor Szel <info@szelvesz.hu>:
      hb_strEmpty().
      hb_strMatchDOS().
      hb_STRZERO().
   See doc/hdr_tpl.txt, Version 1.2 or later, for licensing terms.
*/

#include <ctype.h>
#include <math.h>

#include "extend.h"
#include "dates.h"
#include "itemapi.h"
#include "errorapi.h"
#include "set.h"

#define HB_ISSPACE( c ) ( ( c ) == 9 || ( c ) == 10 || ( c ) == 13 || ( c ) == 32 )

/* DJGPP can sprintf a float that is almost 320 digits long */
#define HB_MAX_DOUBLE_LENGTH 320

#ifdef HARBOUR_STRICT_CLIPPER_COMPATIBILITY

#include "init.h"

static double s_dInfinity = 0;

HB_CALL_ON_STARTUP_BEGIN( Strings_InitInfinity )
   s_dInfinity = -log( 0 );
HB_CALL_ON_STARTUP_END( Strings_InitInfinity )
#if ! defined(__GNUC__)
#pragma startup Strings_InitInfinity
#endif

#endif

BOOL hb_strEmpty( char * szText, ULONG ulLen )
{
   BOOL bRetVal = TRUE;

   while( ulLen-- )
   {
      char c = szText[ ulLen ];

      if( !HB_ISSPACE( c ) )
      {
         bRetVal = FALSE;
         break;
      }
   }

   return bRetVal;
}

/* Harbour Project source code
   http://www.Harbour-Project.org/
   The following function is Copyright 1999 David G. Holm <dholm@jsd-llc.com>:
      hb_stricmp().
   See doc/hdr_tpl.txt, Version 1.2 or later, for licensing terms.
*/

int hb_stricmp( const char *s1, const char *s2 )
{
   int rc = 0, c1, c2;
   ULONG l1, l2, count;

   l1 = strlen( s1 );
   l2 = strlen( s2 );
   if( l1 < l2 ) count = l1;
   else count = l2;
   while( rc == 0 && count > 0 )
   {
      count--;
      c1 = toupper( *s1++ );
      c2 = toupper( *s2++ );
      if( c1 != c2 ) rc = ( c1 < c2 ? -1 : 1 );
   }
   if( rc == 0 && l1 != l2 )
   {
      if( l1 < l2 ) rc = -1;
      else rc = 1;
   }
   return rc;
}

static BOOL  hb_strMatchDOS( char *pszString, char *pszMask )
{
   while( *pszMask && *pszString )
   {
      if( *pszMask == '*' )
      {
         while( *pszMask == '*' )
             pszMask++;

         if( ! ( *pszMask ) )
            return TRUE;
         else
            if( *pszMask == '?' )
               pszString++;
            else
            {
               while( toupper( *pszString ) != toupper( *pszMask ) )
               {
                  if( ! ( *( ++pszString ) ) )
                     return FALSE;
               }
               while( toupper( *pszString ) == toupper( *pszMask ) )
               {
                  if( ! ( *( ++pszString ) ) )
                     break;
               }
               pszMask++;
            }
      }
      else
         if( toupper( *pszMask ) != toupper( *pszString ) && *pszMask != '?' )
            return FALSE;
         else
         {
            pszMask++;
            pszString++;
         }
   }

   return ! ( ( ! ( *pszString ) && *pszMask && *pszMask != '*') ||
              ( ! ( *pszMask ) && *pszString ) );
}

/* TODO: Replace it with a code that supports real regular expressions
 *
 */
BOOL hb_strMatchRegExp( char *szString, char *szMask )
{
   return hb_strMatchDOS( szString, szMask );
}


/* determines if first char of string is letter */
/* TEST: QOUT( "isalpha( 'hello' ) = ", isalpha( 'hello' ) ) */
/* TEST: QOUT( "isalpha( '12345' ) = ", isalpha( '12345' ) ) */
HARBOUR HB_ISALPHA( void )
{
   hb_retl( isalpha( *hb_parc( 1 ) ) );
}

/* determines if first char of string is digit */
/* TEST: QOUT( "isdigit( '12345' ) = ", isdigit( '12345' ) ) */
/* TEST: QOUT( "isdigit( 'abcde' ) = ", isdigit( 'abcde' ) ) */
HARBOUR HB_ISDIGIT( void )
{
   hb_retl( isdigit( *hb_parc( 1 ) ) );
}

/* determines if first char of string is upper-case */
/* TEST: QOUT( "isupper( 'Abcde' ) = ", isupper( 'Abcde' ) ) */
/* TEST: QOUT( "isupper( 'abcde' ) = ", isupper( 'abcde' ) ) */
HARBOUR HB_ISUPPER( void )
{
   hb_retl( isupper( *hb_parc( 1 ) ) );
}

/* determines if first char of string is lower-case */
/* TEST: QOUT( "islower( 'abcde' ) = ", islower( 'abcde' ) ) */
/* TEST: QOUT( "islower( 'Abcde' ) = ", islower( 'Abcde' ) ) */
HARBOUR HB_ISLOWER( void )
{
   hb_retl( islower( *hb_parc( 1 ) ) );
}

/* trims from the left, and returns a new pointer to szText */
/* also returns the new length in lLen */
char *hb_strLTrim( char *szText, ULONG *lLen )
{
   while( *lLen && HB_ISSPACE( *szText ) )
   {
      szText++;
      ( *lLen )--;
   }

   return szText;
}

/* trims leading spaces from a string */
/* TEST: QOUT( "ltrim( '  hello world  ' ) = '" + ltrim( '  hello world  ' ) + "'" ) */
HARBOUR HB_LTRIM( void )
{
   if( hb_pcount() == 1 )
   {
      PHB_ITEM pText = hb_param( 1, IT_STRING );

      if( pText )
      {
         ULONG lLen = pText->item.asString.length;
         char *szText = hb_strLTrim( pText->item.asString.value, &lLen );

         hb_retclen( szText, lLen );
      }
      else
         hb_errRT_BASE( EG_ARG, 1101, NULL, "LTRIM" );
   }
   else
      hb_errRT_BASE( EG_ARGCOUNT, 3000, NULL, "LTRIM" ); /* NOTE: Clipper catches this at compile time! */
}

/* returns szText and the new length in lLen */
ULONG hb_strRTrimLen( char *szText, ULONG lLen, BOOL bAnySpace )
{
   if( bAnySpace )
   {
      while( lLen && HB_ISSPACE( szText[ lLen - 1 ] ) )
         lLen--;
   }
   else
   {
      while( lLen && szText[ lLen - 1 ] == ' ' )
         lLen--;
   }

   return lLen;
}

/* NOTE: The second parameter is a Harbour extension */

/* trims trailing spaces from a string */
/* TEST: QOUT( "rtrim( '  hello world  ' ) = '" + rtrim( '  hello world  ' ) + "'" ) */
HARBOUR HB_RTRIM( void )
{
   if( hb_pcount() >= 1 && hb_pcount() <= 2 )
   {
      PHB_ITEM pText = hb_param( 1, IT_STRING );

      if( pText )
      {
         BOOL bAnySpace = ( ISLOG( 2 ) ? hb_parl( 2 ) : FALSE );
         hb_retclen( pText->item.asString.value, hb_strRTrimLen( pText->item.asString.value, pText->item.asString.length, bAnySpace ) );
      }
      else
#ifdef HARBOUR_STRICT_CLIPPER_COMPATIBILITY
         /* Clipper doesn't error, but only in RTRIM. TRIM() throws an error, though */
         hb_retc( "" );
#else
         hb_errRT_BASE( EG_ARG, 1100, NULL, "RTRIM" );
#endif
   }
   else
      hb_errRT_BASE( EG_ARGCOUNT, 3000, NULL, "RTRIM" ); /* NOTE: Clipper catches this at compile time! */
}

/* NOTE: The second parameter is a Harbour extension */

/* synonymn for RTRIM */
HARBOUR HB_TRIM( void )
{
   if( hb_pcount() >= 1 && hb_pcount() <= 2 )
   {
      PHB_ITEM pText = hb_param( 1, IT_STRING );

      if( pText )
      {
         BOOL bAnySpace = ( ISLOG( 2 ) ? hb_parl( 2 ) : FALSE );
         hb_retclen( pText->item.asString.value, hb_strRTrimLen( pText->item.asString.value, pText->item.asString.length, bAnySpace ) );
      }
      else
         hb_errRT_BASE( EG_ARG, 1100, NULL, "TRIM" );
   }
   else
      hb_errRT_BASE( EG_ARGCOUNT, 3000, NULL, "TRIM" ); /* NOTE: Clipper catches this at compile time! */
}

/* NOTE: The second parameter is a Harbour extension */

/* trims leading and trailing spaces from a string */
/* TEST: QOUT( "alltrim( '  hello world  ' ) = '" + alltrim( '  hello world  ' ) + "'" ) */
HARBOUR HB_ALLTRIM( void )
{
   if( ISCHAR( 1 ) )
   {
      char *szText = hb_parc( 1 );
      BOOL bAnySpace = ( ISLOG( 2 ) ? hb_parl( 2 ) : FALSE );
      ULONG lLen;

      lLen = hb_strRTrimLen( szText, hb_parclen( 1 ), bAnySpace );

      szText = hb_strLTrim( szText, &lLen );

      hb_retclen( szText, lLen );
   }
   else
#ifdef HB_COMPATIBILITY_CLIPPER_53
      hb_errRT_BASE( EG_ARG, 2022, NULL, "ALLTRIM" );
#else
      hb_retc( "" );
#endif
}

/* This function is used by all of the PAD functions to prepare the argument
   being padded. If date, convert to string using hb_dtoc(). If numeric,
   convert to unpadded string. Return pointer to string and set string length */
static char * hb_itemPadConv( PHB_ITEM pItem, char * buffer, ULONG * pulSize )
{
   char * szText = NULL;

   if( pItem ) switch( pItem->type )
   {
      case IT_DATE:
         szText = hb_dtoc( hb_pards( 1 ), buffer, hb_set.HB_SET_DATEFORMAT );
         *pulSize = strlen( szText );
         break;

      case IT_INTEGER:
         sprintf( buffer, "%d", hb_parni( 1 ) );
         szText = buffer;
         *pulSize = strlen( szText );
         break;

      case IT_LONG:
         sprintf( buffer, "%ld", hb_parnl( 1 ) );
         szText = buffer;
         *pulSize = strlen( szText );
         break;

      case IT_DOUBLE:
         if( pItem->item.asDouble.decimal )
            sprintf( buffer, "%.*f", pItem->item.asDouble.decimal, hb_parnd( 1 ) );
         else
            sprintf( buffer, "%ld", hb_parnl( 1 ) );
         szText = buffer;
         *pulSize = strlen( szText );
         break;

      case IT_STRING:
         szText = hb_parc( 1 );
         *pulSize = hb_parclen( 1 );
         break;
   }

   return szText;
}

/* right-pads a date, number, or string with spaces or supplied character */
/* TEST: QOUT( "padr( 'hello', 10 ) = '" + padr( 'hello', 10 ) + "'" ) */
HARBOUR HB_PADR( void )
{
   ULONG ulSize;
   char buffer[ 128 ];
   char *szText = hb_itemPadConv( hb_param( 1, IT_ANY ), buffer, &ulSize );

   if( szText && ISNUM( 2 ) )
   {
      LONG lLen = hb_parnl( 2 );

      if( lLen > ( LONG ) ulSize )
      {
         char *szResult = ( char * ) hb_xgrab( lLen + 1 );
         LONG lPos;
         char cPad;

         memcpy( szResult, szText, ( LONG ) ulSize );

         cPad = ( ISCHAR( 3 ) ? *( hb_parc( 3 ) ) : ' ' );

         for( lPos = ( LONG ) ulSize; lPos < lLen; lPos++ )
            szResult[ lPos ] = cPad;

         hb_retclen( szResult , lLen );
         hb_xfree( szResult );
      }
      else
      {
         if( lLen < 0 )
            lLen = 0;

         hb_retclen( szText, lLen );
      }
   }
   else
      hb_retc( "" );
}

/* synonymn for PADR */
HARBOUR HB_PAD( void )
{
   HB_PADR();
}

/* left-pads a date, number, or string with spaces or supplied character */
/* TEST: QOUT( "padl( 'hello', 10 ) = '" + padl( 'hello', 10 ) + "'" ) */
HARBOUR HB_PADL( void )
{
   ULONG ulSize;
   char buffer[ 128 ];
   char * szText = hb_itemPadConv( hb_param( 1, IT_ANY ), buffer, &ulSize );

   if( szText && ISNUM( 2 ) )
   {
      LONG lLen = hb_parnl( 2 );

      if( lLen > ( LONG ) ulSize )
      {
         char *szResult = ( char * ) hb_xgrab( lLen + 1 );
         LONG lPos = lLen - ( LONG ) ulSize;
         char cPad;

         memcpy( szResult + lPos, szText, ( LONG ) ulSize );

         cPad = ( ISCHAR( 3 ) ? *( hb_parc( 3 ) ) : ' ');

         for(; lPos > 0; lPos-- )
         {
            szResult[ lPos - 1 ] = cPad;
         }

         hb_retclen( szResult, lLen );
         hb_xfree( szResult );
      }
      else
      {
         if( lLen < 0 )
            lLen = 0;

         hb_retclen( szText, lLen );
      }
   }
   else
      hb_retc( "" );
}

/* centre-pads a date, number, or string with spaces or supplied character */
/* TEST: QOUT( "padc( 'hello', 10 ) = '" + padc( 'hello', 10 ) + "'" ) */
HARBOUR HB_PADC( void )
{
   ULONG ulSize;
   char buffer[ 128 ];
   char *szText = hb_itemPadConv( hb_param( 1, IT_ANY ), buffer, &ulSize );

   if( szText && ISNUM( 2 ) )
   {
      LONG lLen = hb_parnl( 2 );

      if( lLen > ( LONG ) ulSize )
      {
         char *szResult = ( char * ) hb_xgrab( lLen + 1 );
         char cPad;
         LONG w, lPos = ( lLen - ( LONG ) ulSize ) / 2;

         memcpy( szResult + lPos, szText, ( LONG ) ulSize + 1 );

         cPad = ( ISCHAR( 3 ) ? *hb_parc( 3 ) : ' ' );

         for( w = 0; w < lPos; w++ )
            szResult[ w ] = cPad;

         for( w = ( LONG ) ulSize + lPos; w < lLen; w++ )
            szResult[ w ] = cPad;

         szResult[ lLen ] = '\0';

         hb_retclen( szResult, lLen );
         hb_xfree( szResult );
      }
      else
      {
         if( lLen < 0 )
            lLen = 0;

         hb_retclen( szText, lLen );
      }
   }
   else
      hb_retc( "" );
}

ULONG hb_strAt( char *szSub, ULONG ulSubLen, char *szText, ULONG ulLen )
{
   if( ulSubLen )
   {
      if( ulLen >= ulSubLen )
      {
         ULONG ulPos = 0, ulSubPos = 0;

         while( ulPos < ulLen && ulSubPos < ulSubLen )
         {
            if( *( szText + ulPos ) == *( szSub + ulSubPos ) )
            {
               ulSubPos++;
               ulPos++;
            }
            else if( ulSubPos )
               ulSubPos = 0;
            else
               ulPos++;
         }
         return ( ulSubPos < ulSubLen ) ? 0 : ( ulPos - ulSubLen + 1 );
      }
      else
         return 0;
   }
   else
      return 1;
}

/* locates a substring in a string */
/* TEST: QOUT( "at( 'cde', 'abcdefgfedcba' ) = '" + at( 'cde', 'abcsefgfedcba' ) + "'" ) */
HARBOUR HB_AT( void )
{
   if( hb_pcount() == 2 )
   {
      PHB_ITEM pSub = hb_param( 1, IT_STRING );
      PHB_ITEM pText = hb_param( 2, IT_STRING );

      if( IS_STRING( pText ) && IS_STRING( pSub ) )
      {
         hb_retnl( hb_strAt( pSub->item.asString.value, pSub->item.asString.length, pText->item.asString.value, pText->item.asString.length ) );
      }
      else
         hb_errRT_BASE( EG_ARG, 1108, NULL, "AT" );
   }
   else
      hb_errRT_BASE( EG_ARGCOUNT, 3000, NULL, "AT" ); /* NOTE: Clipper catches this at compile time! */
}

/* locates a substring in a string starting at the end */
/* TEST: QOUT( "rat( 'cde', 'abcdefgfedcba' ) = '" + rat( 'cde', 'abcdefgfedcba' ) + "'" ) */
HARBOUR HB_RAT( void )
{
   ULONG ulSubLen = hb_parclen( 1 );

   if( ulSubLen )
   {
      long lPos = hb_parclen( 2 ) - ulSubLen;

      if( lPos >= 0 )
      {
         char *szSub = hb_parc( 1 );
         char *szText = hb_parc( 2 );
         BOOL bFound = FALSE;

         while( lPos >= 0 && !bFound )
         {
            if( *( szText + lPos ) == *szSub )
               bFound = ( memcmp( szSub, szText + lPos, ulSubLen ) == 0 );
            lPos--;
         }

         hb_retnl( bFound ? lPos + 2 : 0 );
      }
      else
         hb_retni( 0 );
   }
   else
      /* This function never seems to raise an error */
      hb_retni( 0 );
}

/* converts an ASCII code to a character value */
HARBOUR HB_CHR( void )
{
   if( hb_pcount() == 1 )
   {
      if( ISNUM( 1 ) )
      {
         char chr[ 2 ];

         /* Believe it or not, clipper does this! */
         chr[ 0 ] = hb_parnl( 1 ) % 256;
         chr[ 1 ] = '\0';
         hb_retclen( chr, 1 );
      }
      else
         hb_errRT_BASE( EG_ARG, 1104, NULL, "CHR" );
   }
   else
      hb_errRT_BASE( EG_ARGCOUNT, 3000, NULL, "CHR" ); /* NOTE: Clipper catches this at compile time! */
}

/* converts a character value to an ASCII code */
HARBOUR HB_ASC( void )
{
   if( hb_pcount() == 1 )
   {
      PHB_ITEM pText = hb_param( 1, IT_STRING );

      if( pText )
      {
         if( pText->item.asString.length > 0 )
            hb_retni( ( BYTE ) * ( pText->item.asString.value ) );
         else
            hb_retni( 0 );
      }
      else
         hb_errRT_BASE( EG_ARG, 1107, NULL, "ASC" );
   }
   else
      hb_errRT_BASE( EG_ARGCOUNT, 3000, NULL, "ASC" ); /* NOTE: Clipper catches this at compile time! */
}

/* returns the left-most n characters in string */
HARBOUR HB_LEFT( void )
{
   if( hb_pcount() == 2 )
   {
      PHB_ITEM pText = hb_param( 1, IT_STRING );

      if( pText && ISNUM( 2 ) )
      {
         LONG lLen = hb_parnl( 2 );

         if( lLen > ( LONG ) pText->item.asString.length )
            lLen = ( LONG ) pText->item.asString.length;

         else if( lLen < 0 )
            lLen = 0;

         hb_retclen( pText->item.asString.value, lLen );
      }
      else
         hb_errRT_BASE( EG_ARG, 1124, NULL, "LEFT" );
   }
   else
      hb_errRT_BASE( EG_ARGCOUNT, 3000, NULL, "LEFT" ); /* NOTE: Clipper catches this at compile time! */
}

/* returns the right-most n characters in string */
HARBOUR HB_RIGHT( void )
{
   PHB_ITEM pText = hb_param( 1, IT_STRING );

   if( pText && ISNUM( 2 ) )
   {
      LONG lLen = hb_parnl( 2 );

      if( lLen > ( LONG ) pText->item.asString.length )
         lLen = ( LONG ) pText->item.asString.length;

      else if( lLen < 0 )
         lLen = 0;

      hb_retclen( pText->item.asString.value + pText->item.asString.length - lLen, lLen );
   }
   else
   {
      /* Clipper doesn't error */
      hb_retc( "" );
   }
}

/* returns l characters from n characters into string */
HARBOUR HB_SUBSTR( void )
{
   if( hb_pcount() >= 2 && hb_pcount() <= 3 )
   {
      PHB_ITEM pText = hb_param( 1, IT_STRING );

      if( pText && ISNUM( 2 ) )
      {
         LONG lPos = hb_parnl( 2 );

         if( lPos < 0 )
         {
            lPos += ( LONG ) pText->item.asString.length;
            if( lPos < 0 )
               lPos = 0;
         }
         else if( lPos )
         {
            lPos--;
         }

         if( lPos < ( LONG ) pText->item.asString.length )
         {
            LONG lLen;

            if( ISNUM( 3 ) )
            {
               lLen = hb_parnl( 3 );

               if( lLen > ( LONG ) pText->item.asString.length - lPos )
                  lLen = ( LONG ) pText->item.asString.length - lPos;
            }
            else
               lLen = ( LONG ) pText->item.asString.length - lPos;

            if( lLen > 0 )
               hb_retclen( pText->item.asString.value + lPos, lLen );
            else
               hb_retc( "" );
         }
         else
            hb_retc( "" );
      }
      else
         hb_errRT_BASE( EG_ARG, 1110, NULL, "SUBSTR" );
   }
   else
      hb_errRT_BASE( EG_ARGCOUNT, 3000, NULL, "SUBSTR" ); /* NOTE: Clipper catches this at compile time! */
}

/* converts szText to lower case. Does not create a new string! */
char *hb_strLower( char *szText, ULONG ulLen )
{
   ULONG i;

   for( i = 0; i < ulLen; i++ )
      szText[ i ] = tolower( szText[ i ] );

   return szText;
}

/* converts string to lower case */
HARBOUR HB_LOWER( void )
{
   if( hb_pcount() == 1 )
   {
      PHB_ITEM pText = hb_param( 1, IT_STRING );

      if( pText )
      {
         ULONG ulLen = pText->item.asString.length;

         hb_retclen( hb_strLower( pText->item.asString.value, ulLen ), ulLen );
      }
      else
         hb_errRT_BASE( EG_ARG, 1103, NULL, "LOWER" );
   }
   else
      hb_errRT_BASE( EG_ARGCOUNT, 3000, NULL, "LOWER" ); /* NOTE: Clipper catches this at compile time! */
}

void hb_strupr( char * szText )
{
   char *p;

   for( p = szText; *p; p++ )
      *p = toupper( *p );
}

/* converts szText to upper case. Does not create a new string! */
char *hb_strUpper( char *szText, ULONG ulLen )
{
   ULONG i;

   for( i = 0; i < ulLen; i++ )
      szText[ i ] = toupper( szText[ i ] );

   return szText;
}

/* converts string to upper case */
HARBOUR HB_UPPER( void )
{
   if( hb_pcount() == 1 )
   {
      PHB_ITEM pText = hb_param( 1, IT_STRING );

      if( pText )
      {
         ULONG ulLen = pText->item.asString.length;

         hb_retclen( hb_strUpper( pText->item.asString.value, ulLen ), ulLen );
      }
      else
         hb_errRT_BASE( EG_ARG, 1102, NULL, "UPPER" );
   }
   else
      hb_errRT_BASE( EG_ARGCOUNT, 3000, NULL, "UPPER" ); /* NOTE: Clipper catches this at compile time! */
}

/* returns n copies of given string */
/* TEST: QOUT( "replicate( 'abc', 5 ) = " + replicate( 'abc', 5 ) ) */
HARBOUR HB_REPLICATE( void )
{
   if( hb_pcount() == 2 )
   {
      if( ISCHAR( 1 ) && ISNUM( 2 ) )
      {
         LONG lTimes = hb_parnl( 2 );

         if( lTimes > 0 )
         {
            ULONG ulLen = hb_parclen( 1 );

            if( ( double ) ( ( double ) ulLen * ( double ) lTimes ) < ( double ) ULONG_MAX )
            {
               char *szText = hb_parc( 1 );
               char *szResult = ( char * ) hb_xgrab( ( ulLen * lTimes ) + 1 );
               char *szPtr = szResult;
               LONG i;

               for( i = 0; i < lTimes; i++ )
               {
                  memcpy( szPtr, szText, ulLen );
                  szPtr += ulLen;
               }

               hb_retclen( szResult, ulLen * lTimes );
               hb_xfree( szResult );
            }
            else
               hb_errRT_BASE( EG_STROVERFLOW, 1234, NULL, "REPLICATE" );
         }
         else
            hb_retc( "" );
      }
      else
         hb_errRT_BASE( EG_ARG, 1106, NULL, "REPLICATE" );
   }
   else
      hb_errRT_BASE( EG_ARGCOUNT, 3000, NULL, "REPLICATE" ); /* NOTE: Clipper catches this at compile time! */
}

/* returns n copies of a single space */
/* TEST: QOUT( "space( 5 ) = '" + space( 5 ) + "'" ) */
HARBOUR HB_SPACE( void )
{
   if( hb_pcount() == 1 )
   {
      if( ISNUM( 1 ) )
      {
         LONG lLen = hb_parnl( 1 );

         if( lLen > 0 )
         {
            char *szResult = ( char * ) hb_xgrab( lLen + 1 );

            /* NOTE: String overflow could never occure since a string can */
            /*       be as large as ULONG_MAX, and the maximum length that */
            /*       can be specified is LONG_MAX here.                    */
            /* hb_errRT_BASE( EG_STROVERFLOW, 1233, NULL, "SPACE" ); */

            memset( szResult, ' ', lLen );
            hb_retclen( szResult, lLen );
            hb_xfree( szResult );
         }
         else
            hb_retc( "" );
      }
      else
         hb_errRT_BASE( EG_ARG, 1105, NULL, "SPACE" );
   }
   else
      hb_errRT_BASE( EG_ARGCOUNT, 3000, NULL, "SPACE" ); /* NOTE: Clipper catches this at compile time! */
}

/* replaces characters in a string */
HARBOUR HB_STUFF( void )
{
   if( ISCHAR( 1 ) && ISNUM( 2 ) && ISNUM( 3 ) && ISCHAR( 4 ) )
   {
      char *szText = hb_parc( 1 );
      ULONG ulText = hb_parclen( 1 );
      ULONG ulPos = hb_parnl( 2 );
      ULONG ulDel = hb_parnl( 3 );
      ULONG ulInsert = hb_parclen( 4 );

      ULONG ulTotalLen;

      if( ulPos > 0 )
         ulPos--;

      if( ulPos > ulText )
         ulPos = ulText;

      if( ulDel > ulText - ulPos )
         ulDel = ulText - ulPos;

      if( ( ulTotalLen = ulText + ulInsert - ulDel ) > 0 )
      {
         char * szResult = ( char * ) hb_xgrab( ulTotalLen + 1 );

         memcpy( szResult, szText, ulPos );
         memcpy( szResult + ulPos, hb_parc( 4 ), ulInsert );
         memcpy( szResult + ulPos + ulInsert, szText + ulPos + ulDel, ulText - ( ulPos + ulDel ) );

         szResult[ ulTotalLen ] = '\0';
         hb_retclen( szResult, ulTotalLen );
         hb_xfree( szResult );
      }
      else
         hb_retc( "" );
   }
   else
      hb_retc( "" );
}

/* TODO: Check for string overflow, Clipper can crash if the resulting
         string is too large. Example: 
         StrTran( "...", ".", Replicate( "A", 32000 ) ) */

/* replaces lots of characters in a string */
HARBOUR HB_STRTRAN( void )
{
   PHB_ITEM pText = hb_param( 1, IT_STRING );

   if( pText )
   {
      PHB_ITEM pSeek = hb_param( 2, IT_STRING );

      if( pSeek )
      {
         char *szText = pText->item.asString.value;

         if( pSeek->item.asString.length && pSeek->item.asString.length <= pText->item.asString.length )
         {
            char *szSeek = pSeek->item.asString.value;
            char *szReplace;
            ULONG ulStart;

            ulStart = ( ISNUM( 4 ) ? hb_parnl( 4 ) : 1 );

            if( !ulStart )
            {
               /* Clipper seems to work this way */
               hb_retc( "" );
            }
            else if( ulStart > 0 )
            {
               PHB_ITEM pReplace = hb_param( 3, IT_STRING );
               ULONG ulReplace;
               ULONG ulCount;
               BOOL bAll;

               if( pReplace )
               {
                  szReplace = pReplace->item.asString.value;
                  ulReplace = pReplace->item.asString.length;
               }
               else
               {
                  szReplace = ""; /* shouldn't matter that we don't allocate */
                  ulReplace = 0;
               }

               if( ISNUM( 5 ) )
               {
                  ulCount = hb_parnl( 5 );
                  bAll = FALSE;
               }
               else
               {
                  ulCount = 0;
                  bAll = TRUE;
               }

               if( bAll || ulCount > 0 )
               {
                  ULONG ulFound = 0;
                  LONG lReplaced = 0;
                  ULONG i = 0;
                  ULONG ulLength = pText->item.asString.length;

                  while( i < pText->item.asString.length )
                  {
                     if( ( bAll || lReplaced < ( LONG ) ulCount ) && ! memcmp( szText + i, szSeek, pSeek->item.asString.length ) )
                     {
                        ulFound++;
                        if( ulFound >= ulStart )
                        {
                           lReplaced++;
                           ulLength = ulLength - pSeek->item.asString.length + ulReplace;
                           i += pSeek->item.asString.length;
                        }
                        else
                           i++;
                     }
                     else
                        i++;
                  }

                  if( ulFound )
                  {
                     char *szResult = ( char * ) hb_xgrab( ulLength + 1 );
                     char *szPtr = szResult;

                     ulFound = 0;
                     i = 0;
                     while( i < pText->item.asString.length )
                     {
                        if( lReplaced && ! memcmp( szText + i, szSeek, pSeek->item.asString.length ) )
                        {
                           ulFound++;
                           if( ulFound >= ulStart )
                           {
                              lReplaced--;
                              memcpy( szPtr, szReplace, ulReplace );
                              szPtr += ulReplace;
                              i += pSeek->item.asString.length;
                           }
                           else
                           {
                              *szPtr = szText[ i ];
                              szPtr++;
                              i++;
                           }
                        }
                        else
                        {
                           *szPtr = szText[ i ];
                           szPtr++;
                           i++;
                        }
                     }
                     hb_retclen( szResult, ulLength );
                     hb_xfree( szResult );
                  }
                  else
                     hb_retclen( szText, pText->item.asString.length );
               }
                else
                  hb_retclen( szText, pText->item.asString.length );
            }
            else
               hb_retclen( szText, pText->item.asString.length );
         }
         else
            hb_retclen( szText, pText->item.asString.length );
      }
      else
         hb_errRT_BASE( EG_ARG, 1126, NULL, "STRTRAN" ); /* NOTE: Undocumented but existing Clipper Run-time error */
   }
   else
      hb_errRT_BASE( EG_ARG, 1126, NULL, "STRTRAN" ); /* NOTE: Undocumented but existing Clipper Run-time error */
}

/* returns the numeric value of a character string representation of a number  */
double hb_strVal( char *szText )
{
   return atof( szText );
}

/* returns the numeric value of a character string representation of a number  */
HARBOUR HB_VAL( void )
{
   if( hb_pcount() == 1 )
   {
      PHB_ITEM pText = hb_param( 1, IT_STRING );

      if( pText )
      {
         int nWidth, nDec = 0;
         char * ptr = strchr( pText->item.asString.value, '.' );

         if( ptr )
         {
            nWidth = ptr - pText->item.asString.value;
            nDec = strlen( ptr + 1 );
         }
         else
            nWidth = strlen( pText->item.asString.value );

         hb_retnd( hb_strVal( pText->item.asString.value ) );
         stack.Return.item.asDouble.length = nWidth;
         stack.Return.item.asDouble.decimal = nDec;
      }
      else
         hb_errRT_BASE( EG_ARG, 1098, NULL, "VAL" );
   }
   else
      hb_errRT_BASE( EG_ARGCOUNT, 3000, NULL, "VAL" ); /* NOTE: Clipper catches this at compile time! */
}

/* converts a numeric to a string with optional width & precision.
   This function should be used by any function that wants to format numeric
   data for displaying, printing, or putting in a database.

   Note: The caller is responsible for calling hb_xfree to free the results buffer,
         but ONLY if the return value is not a NULL pointer!
*/
/* TODO: Move it to itemapi.c */
char * hb_itemStr( PHB_ITEM pNumber, PHB_ITEM pWidth, PHB_ITEM pDec )
{
   char * szResult = NULL;

   if( pNumber )
   {
      /* Default to the width and number of decimals specified by the item,
         with a limit of 20 integer places and 9 decimal places */
      int iWidth;
      int iDec;

      if( IS_DOUBLE( pNumber ) )
      {
         iWidth = pNumber->item.asDouble.length;
         iDec   = pNumber->item.asDouble.decimal;
      }
      else if( IS_INTEGER( pNumber ) )
      {
         iWidth = pNumber->item.asInteger.length;
         iDec   = 0;
      }
      else if( IS_LONG( pNumber ) )
      {
         iWidth = pNumber->item.asLong.length;
         iDec   = 0;
      }
      else
      {
         iWidth = 0;
         iDec   = 0;
      }

      if( iWidth > 20 )
         iWidth = 20;
      if( iDec > 9 )
         iDec = 9;
      if( hb_set.HB_SET_FIXED )
         iDec = hb_set.HB_SET_DECIMALS;

      if( pWidth )
      {
         /* If the width parameter is specified, override the default value
            and set the number of decimals to zero */
         iWidth = ( int ) hb_itemGetNL( pWidth );

         if( iWidth < 1 )
            iWidth = 10;                   /* If 0 or negative, use default */
         iDec = 0;
      }

      if( pDec )
      {
         /* This function does not include the decimal places in the width,
            so the width must be adjusted downwards, if the decimal places
            parameter is greater than 0  */
         iDec = ( int ) hb_itemGetNL( pDec );

         if( iDec < 0 )
            iDec = 0;
         else if( iDec > 0 )
            iWidth -= ( iDec + 1 );
      }

      if( iWidth )
      {
         /* We at least have a width value */
         int iBytes;
         int iSize = ( iDec ? iWidth + 1 + iDec : iWidth );

         /* Be paranoid and use a large amount of padding */
         szResult = ( char * ) hb_xgrab( HB_MAX_DOUBLE_LENGTH );

         if( IS_DOUBLE( pNumber ) || iDec != 0 )
         {
            double dNumber = hb_itemGetND( pNumber );

            #ifdef HARBOUR_STRICT_CLIPPER_COMPATIBILITY
            if( pNumber->item.asDouble.length == 99 || dNumber == s_dInfinity || dNumber == -s_dInfinity )
               /* Numeric overflow */
               iBytes = iSize + 1;
            else
            #endif
            {
               if( iDec < pNumber->item.asDouble.decimal )
                  dNumber = hb_numRound( dNumber, iDec );

               if( iDec > 0 )
                  iBytes = sprintf( szResult, "%*.*f", iSize, iDec, dNumber );
               else
                  iBytes = sprintf( szResult, "%*ld", iWidth, ( LONG ) dNumber );
            }
         }
         else switch( pNumber->type & ~IT_BYREF )
         {
            case IT_INTEGER:
                 iBytes = sprintf( szResult, "%*i", iWidth, pNumber->item.asInteger.value );
                 break;

            case IT_LONG:
                 iBytes = sprintf( szResult, "%*li", iWidth, pNumber->item.asLong.value );
                 break;

            default:
                 iBytes = 0;
                 szResult[ 0 ] = '\0';  /* null string */
                 break;
         }
         /* Set to asterisks in case of overflow */
         if( iBytes > iSize )
         {
            memset( szResult, '*', iSize );
            szResult[ iSize ] = '\0';
         }
      }
   }

   return szResult;
}


/*  $DOC$
 *  $FUNCNAME$
 *      STR
 *  $CATEGORY$
 *      Run-time Library, Strings
 *  $ONELINER$
 *      Convert a numeric expression to a character string.
 *  $SYNTAX$
 *      STR(<nNumber>, [<nLength>], [<nDecimals>]) --> cNumber
 *  $ARGUMENTS$
 *      <nNumber> is the numeric expression to be converted to a character
 *      string.
 *      <nLength> is the length of the character string to return, including
 *      decimal digits, decimal point, and sign.
 *      <nDecimals> is the number of decimal places to return.
 *  $RETURNS$
 *      STR() returns <nNumber> formatted as a character string.  If the
 *      optional length and decimal arguments are not specified, STR()
 *      returns the character string according to the following rules:
 *
 *      Results of STR() with No Optional Arguments
 *      ---------------------------------------------------------------
 *      Expression               Return Value Length
 *      ---------------------------------------------------------------
 *      Field Variable           Field length plus decimals
 *      Expressions/constants    Minimum of 10 digits plus decimals
 *      VAL()                    Minimum of 3 digits
 *      MONTH()/DAY()            3 digits
 *      YEAR()                   5 digits
 *      RECNO()                  7 digits
 *      ---------------------------------------------------------------
 *  $DESCRIPTION$
 *      STR() is a numeric conversion function that converts numeric values
 *      to character strings. It is commonly used to concatenate numeric values
 *      to character strings. STR() has applications displaying numbers,
 *      creating codes such as part numbers from numeric values, and creating
 *      index keys that combine numeric and character data.
 *
 *      STR() is like TRANSFORM(), which formats numeric values as character
 *      strings using a mask instead of length and decimal specifications.
 *
 *      The inverse of STR() is VAL(), which converts character numbers to
 *      numerics.
 *
 *      *  If <nLength> is less than the number of whole number digits in
 *         <nNumber>, STR() returns asterisks instead of the number.
 *
 *      *  If <nLength> is less than the number of decimal digits
 *         required for the decimal portion of the returned string, Harbour
 *         rounds the number to the available number of decimal places.
 *
 *      *  If <nLength> is specified but <nDecimals> is omitted (no
 *         decimal places), the return value is rounded to an integer.
 *  $EXAMPLES$
 *      ? STR( 10, 6, 2 ) // " 10.00"
 *      ? STR( -10, 8, 2 ) // "  -10.00"
 *  $TESTS$
 *      see in rtl_test.prg for a comprehensive regression test suit.
 *  $STATUS$
 *      R
 *  $COMPLIANCE$
 *      CA-Clipper compatible.
 *  $SEEALSO$
 *      STRZERO()
 *      VAL()
 *  $END$
 */

HARBOUR HB_STR( void )
{
   if( hb_pcount() >= 1 && hb_pcount() <= 3 )
   {
      BOOL bValid = TRUE;
      PHB_ITEM pNumber = hb_param( 1, IT_NUMERIC );
      PHB_ITEM pWidth  = NULL;
      PHB_ITEM pDec    = NULL;

      if( !pNumber )
         bValid = FALSE;
      else
      {
         if( hb_pcount() > 1 )
         {
            pWidth = hb_param( 2, IT_NUMERIC );
            if( !pWidth )
               bValid = FALSE;
         }
         if( hb_pcount() > 2 )
         {
            pDec = hb_param( 3, IT_NUMERIC );
            if( !pDec )
               bValid = FALSE;
         }
      }
      if( bValid )
      {
         char * szResult = hb_itemStr( pNumber, pWidth, pDec );

         if( szResult )
         {
            hb_retc( szResult );
            hb_xfree( szResult );
         }
         else
            hb_retc( "" );
      }
      else
         hb_errRT_BASE( EG_ARG, 1099, NULL, "STR" );
   }
   else
      hb_errRT_BASE( EG_ARGCOUNT, 3000, NULL, "STR" ); /* NOTE: Clipper catches this at compile time! */
}

/* ------------------------------------------------- */
/* Copyright (C) 1999 Victor Szel <info@szelvesz.hu> */
/* ------------------------------------------------- */

/*  $DOC$
 *  $FUNCNAME$
 *      STRZERO
 *  $CATEGORY$
 *      Run-time Library, Strings
 *  $ONELINER$
 *      Convert a numeric expression to a character string, zero padded.
 *  $SYNTAX$
 *      STRZERO(<nNumber>, [<nLength>], [<nDecimals>]) --> cNumber
 *  $ARGUMENTS$
 *      <nNumber> is the numeric expression to be converted to a character
 *      string.
 *      <nLength> is the length of the character string to return, including
 *      decimal digits, decimal point, and sign.
 *      <nDecimals> is the number of decimal places to return.
 *  $RETURNS$
 *      STRZERO() returns <nNumber> formatted as a character string.  If the
 *      optional length and decimal arguments are not specified, STRZERO()
 *      returns the character string according to the following rules:
 *
 *      Results of STRZERO() with No Optional Arguments
 *      ---------------------------------------------------------------
 *      Expression               Return Value Length
 *      ---------------------------------------------------------------
 *      Field Variable           Field length plus decimals
 *      Expressions/constants    Minimum of 10 digits plus decimals
 *      VAL()                    Minimum of 3 digits
 *      MONTH()/DAY()            3 digits
 *      YEAR()                   5 digits
 *      RECNO()                  7 digits
 *      ---------------------------------------------------------------
 *  $DESCRIPTION$
 *      STRZERO() is a numeric conversion function that converts numeric values
 *      to character strings. It is commonly used to concatenate numeric values
 *      to character strings. STRZERO() has applications displaying numbers,
 *      creating codes such as part numbers from numeric values, and creating
 *      index keys that combine numeric and character data.
 *
 *      STRZERO() is like TRANSFORM(), which formats numeric values as character
 *      strings using a mask instead of length and decimal specifications.
 *
 *      The inverse of STRZERO() is VAL(), which converts character numbers to
 *      numerics.
 *
 *      *  If <nLength> is less than the number of whole number digits in
 *         <nNumber>, STR() returns asterisks instead of the number.
 *
 *      *  If <nLength> is less than the number of decimal digits
 *         required for the decimal portion of the returned string, Harbour
 *         rounds the number to the available number of decimal places.
 *
 *      *  If <nLength> is specified but <nDecimals> is omitted (no
 *         decimal places), the return value is rounded to an integer.
 *  $EXAMPLES$
 *      ? STRZERO( 10, 6, 2 ) // "010.00"
 *      ? STRZERO( -10, 8, 2 ) // "-0010.00"
 *  $TESTS$
 *      see in rtl_test.prg for a comprehensive regression test suit.
 *  $STATUS$
 *      R
 *  $COMPLIANCE$
 *      CA-Clipper compatible (it was not mentioned in the docs though).
 *  $SEEALSO$
 *      STR()
 *      VAL()
 *  $END$
 */

HARBOUR HB_STRZERO( void )
{
   if( hb_pcount() >= 1 && hb_pcount() <= 3 )
   {
      BOOL bValid = TRUE;
      PHB_ITEM pNumber = hb_param( 1, IT_NUMERIC );
      PHB_ITEM pWidth  = NULL;
      PHB_ITEM pDec    = NULL;

      if( !pNumber )
         bValid = FALSE;
      else
      {
         if( hb_pcount() > 1 )
         {
            pWidth = hb_param( 2, IT_NUMERIC );
            if( !pWidth )
               bValid = FALSE;
         }
         if( hb_pcount() > 2 )
         {
            pDec = hb_param( 3, IT_NUMERIC );
            if( !pDec )
               bValid = FALSE;
         }
      }
      if( bValid )
      {
         char * szResult = hb_itemStr( pNumber, pWidth, pDec );

         if( szResult )
         {
            ULONG ulPos = 0;

            while( szResult[ ulPos ] != '\0' && szResult[ ulPos ] != '-' )
            {
               ulPos++;
            }

            if( szResult[ ulPos ] == '-' )
            {
               /* Negative sign found, put the negative sign to the first */
               /* position */

               szResult[ ulPos ] = ' ';

               ulPos = 0;
               while( szResult[ ulPos ] != '\0' && szResult[ ulPos ] == ' ' )
               {
                  szResult[ ulPos++ ] = '0';
               }

               szResult[ 0 ] = '-';
            }
            else
            {
               /* Negative sign not found */

               ulPos = 0;
               while( szResult[ ulPos ] != '\0' && szResult[ ulPos ] == ' ' )
               {
                  szResult[ ulPos++ ] = '0';
               }
            }

            hb_retc( szResult );
            hb_xfree( szResult );
         }
         else
            hb_retc( "" );
      }
      else
         hb_errRT_BASE( EG_ARG, 9999, NULL, "STRZERO" );
   }
}

/* Values returned : HB_STRGREATER_EQUAL, HB_STRGREATER_LEFT, HB_STRGREATER_RIGHT */

int hb_strgreater( char * sz1, char * sz2 )
{
   while( *( sz1 ) && *( sz2 ) && *( sz1 ) == *( sz2 ) )
   {
     sz1++;
     sz2++;
   }
   if( ( *( sz1 ) == 0 && *( sz2 ) != 0 ) ||
       ( *( sz2 ) > *( sz1 ) )               )
      return HB_STRGREATER_RIGHT;

   if( ( *( sz1 ) != 0 && *( sz2 ) == 0 ) ||
       ( *( sz1 ) > *( sz2 ) )               )
      return HB_STRGREATER_LEFT;

   return HB_STRGREATER_EQUAL;
}
