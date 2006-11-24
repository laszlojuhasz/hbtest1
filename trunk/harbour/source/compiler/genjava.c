/*
 * $Id$
 */

/*
 * Harbour Project source code:
 * Compiler Java source generation
 *
 * Copyright 1999 Matteo Baccan <baccan@isanet.it>
 *                Based on a work of Eddie Runia <eddie@runia.com>
 * www - http://www.harbour-project.org
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
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

#include "hbcomp.h"

#define SYM_NOLINK  0              /* Symbol does not have to be linked */
#define SYM_FUNC    1              /* Defined function                  */
#define SYM_EXTERN  2              /* Previously defined function       */

static void hb_fputc( BYTE b );
static void hb_fputs( char * szName );

static FILE * s_yyc;
static int s_nChar = 0;

void hb_compGenJava( HB_COMP_DECL, PHB_FNAME pFileName )
{
   char szFileName[ _POSIX_PATH_MAX ], * szVer;
   PFUNCTION pFunc /*= HB_COMP_PARAM->functions.pFirst */;
   PCOMSYMBOL pSym = HB_COMP_PARAM->symbols.pFirst;
   ULONG lPCodePos;
   LONG lSymbols;
   ULONG ulCodeLength;

   if( ! pFileName->szExtension )
      pFileName->szExtension = ".java";
   hb_fsFNameMerge( szFileName, pFileName );

   s_yyc = fopen( szFileName, "wb" );
   if( ! s_yyc )
   {
      hb_compGenError( HB_COMP_PARAM, hb_comp_szErrors, 'E', HB_COMP_ERR_CREATE_OUTPUT, szFileName, NULL );
      return;
   }

   if( ! HB_COMP_PARAM->fQuiet )
   {
      printf( "Generating Java source output to \'%s\'... ", szFileName );
      fflush( stdout );
   }

   s_nChar = 0;

   szVer = hb_verHarbour();
   fprintf( s_yyc, "/*\n * %s\n * Generated JAVA source code\n */\n\n", szVer );
   hb_xfree( szVer );

   fprintf( s_yyc, "public class %s\n", pFileName->szName );
   fprintf( s_yyc, "{\n" );
   fprintf( s_yyc, "   public static int[] pCode =\n" );
   fprintf( s_yyc, "   {\n" );
   fprintf( s_yyc, "      " );

   /* writes the symbol table */

   lSymbols = 0;                /* Count number of symbols */
   while( pSym )
   {
      lSymbols++;
      pSym = pSym->pNext;
   }
   hb_fputc( ( BYTE ) ( ( lSymbols       ) & 255 ) ); /* Write number symbols */
   hb_fputc( ( BYTE ) ( ( lSymbols >> 8  ) & 255 ) );
   hb_fputc( ( BYTE ) ( ( lSymbols >> 16 ) & 255 ) );
   hb_fputc( ( BYTE ) ( ( lSymbols >> 24 ) & 255 ) );

   pSym = HB_COMP_PARAM->symbols.pFirst;
   while( pSym )
   {
      hb_fputs( pSym->szName );
      hb_fputc( 0 );
      hb_fputc( pSym->cScope );

      /* specify the function address if it is a defined function or a
         external called function */
      if( hb_compFunctionFind( HB_COMP_PARAM, pSym->szName ) ) /* is it a defined function ? */
         hb_fputc( SYM_FUNC );
      else
      {
         if( hb_compFunCallFind( HB_COMP_PARAM, pSym->szName ) )
            hb_fputc( SYM_EXTERN );
         else
            hb_fputc( SYM_NOLINK );
      }
      pSym = pSym->pNext;
   }

   pFunc = HB_COMP_PARAM->functions.pFirst;

   lSymbols = 0;                /* Count number of symbols */
   while( pFunc )
   {
      lSymbols++;
      pFunc = pFunc->pNext;
   }
   hb_fputc( ( BYTE ) ( ( lSymbols       ) & 255 ) ); /* Write number symbols */
   hb_fputc( ( BYTE ) ( ( lSymbols >> 8  ) & 255 ) );
   hb_fputc( ( BYTE ) ( ( lSymbols >> 16 ) & 255 ) );
   hb_fputc( ( BYTE ) ( ( lSymbols >> 24 ) & 255 ) );

   /* Generate functions data
    */
   pFunc = HB_COMP_PARAM->functions.pFirst;
   if( ! HB_COMP_PARAM->fStartProc )
      pFunc = pFunc->pNext;

   while( pFunc )
   {
      hb_fputs( pFunc->szName );
      hb_fputc( 0 );
      ulCodeLength = pFunc->lPCodePos;
      hb_fputc( ( BYTE ) ( ( ulCodeLength       ) & 255 ) ); /* Write size */
      hb_fputc( ( BYTE ) ( ( ulCodeLength >> 8  ) & 255 ) );
      hb_fputc( ( BYTE ) ( ( ulCodeLength >> 16 ) & 255 ) );
      hb_fputc( ( BYTE ) ( ( ulCodeLength >> 24 ) & 255 ) );

      lPCodePos = 0;
      while( lPCodePos < pFunc->lPCodePos )
         hb_fputc( pFunc->pCode[ lPCodePos++ ] );

      pFunc = pFunc->pNext;
   }

   fprintf( s_yyc, "\n   };\n\n" );
   fprintf( s_yyc, "   static public void main( String argv[] )\n" );
   fprintf( s_yyc, "   {\n" );
   fprintf( s_yyc, "      Harbour.Run( %s.pCode ); \n", pFileName->szName );
   fprintf( s_yyc, "   }\n\n" );
   fprintf( s_yyc, "}\n" );

   fclose( s_yyc );

   if( ! HB_COMP_PARAM->fQuiet )
      printf( "Done.\n" );
}

static void hb_fputc( BYTE b )
{
   if( ++s_nChar > 1 )
      fprintf( s_yyc, ", " );

   if( s_nChar == 9 )
   {
      fprintf( s_yyc, "\n      " );
      s_nChar = 1;
   }

   fprintf( s_yyc, "0x%02X", ( int ) b );
}

static void hb_fputs( char * szName )
{
   unsigned int nPos = 0;

   while( nPos < strlen( szName ) )
      hb_fputc( szName[ nPos++ ] );
}
