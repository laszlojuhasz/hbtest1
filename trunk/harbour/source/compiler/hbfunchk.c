/*
 * $Id$
 */

/*
 * Harbour Project source code:
 * Compile time RTL argument checking
 *
 * Copyright 1999 Jose Lalin <dezac@corevia.com>
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

#include "hbcomp.h"

/* NOTE: iMinParam = -1, means no checking
 *       iMaxParam = -1, means no upper limit
 */

typedef struct
{
   char * cFuncName;                /* function name              */
   int    iMinParam;                /* min no of parms it needs   */
   int    iMaxParam;                /* max no of parms need       */
} HB_FUNCINFO, * HB_PFUNCINFO;

static HB_FUNCINFO hb_StdFunc[] =
{
   { "AADD"      , 2,  2 },
   { "ABS"       , 1,  1 },
   { "ASC"       , 1,  1 },
   { "AT"        , 2,  2 },
   { "BOF"       , 0,  0 },
   { "BREAK"     , 0,  1 },
   { "CDOW"      , 1,  1 },
   { "CHR"       , 1,  1 },
   { "CMONTH"    , 1,  1 },
   { "COL"       , 0,  0 },
   { "CTOD"      , 1,  1 },
   { "DATE"      , 0,  0 },
   { "DAY"       , 1,  1 },
   { "DELETED"   , 0,  0 },
   { "DEVPOS"    , 2,  2 },
   { "DOW"       , 1,  1 },
   { "DTOC"      , 1,  1 },
   { "DTOS"      , 1,  1 },
   { "EMPTY"     , 1,  1 },
   { "EOF"       , 0,  0 },
   { "EVAL"      , 1, -1 },
   { "EXP"       , 1,  1 },
   { "FCOUNT"    , 0,  0 },
   { "FIELDNAME" , 1,  1 },
   { "FILE"      , 1,  1 },
   { "FLOCK"     , 0,  0 },
   { "FOUND"     , 0,  0 },
   { "INKEY"     , 0,  2 },
   { "INT"       , 1,  1 },
   { "LASTREC"   , 0,  0 },
   { "LEFT"      , 2,  2 },
   { "LEN"       , 1,  1 },
   { "LOCK"      , 0,  0 },
   { "LOG"       , 1,  1 },
   { "LOWER"     , 1,  1 },
   { "LTRIM"     , 1,  1 },
   { "MAX"       , 2,  2 },
   { "MIN"       , 2,  2 },
   { "MONTH"     , 1,  1 },
   { "PCOL"      , 0,  0 },
   { "PCOUNT"    , 0,  0 },
   { "PROW"      , 0,  0 },
   { "RECCOUNT"  , 0,  0 },
   { "RECNO"     , 0,  0 },
   { "REPLICATE" , 2,  2 },
   { "RLOCK"     , 0,  0 },
   { "ROUND"     , 2,  2 },
   { "ROW"       , 0,  0 },
   { "RTRIM"     , 1,  2 }, /* Second parameter is a Harbour extension */
   { "SECONDS"   , 0,  0 },
   { "SELECT"    , 0,  1 },
   { "SETPOS"    , 2,  2 },
   { "SETPOSBS"  , 0,  0 },
   { "SPACE"     , 1,  1 },
   { "SQRT"      , 1,  1 },
   { "STR"       , 1,  3 },
   { "SUBSTR"    , 2,  3 },
   { "TIME"      , 0,  0 },
   { "TRANSFORM" , 2,  2 },
   { "TRIM"      , 1,  2 }, /* Second parameter is a Harbour extension */
   { "TYPE"      , 1,  1 },
   { "UPPER"     , 1,  1 },
   { "VAL"       , 1,  1 },
   { "VALTYPE"   , 1,  1 },
   { "WORD"      , 1,  1 },
   { "YEAR"      , 1,  1 },
   { 0           , 0,  0 }
};

void hb_compFunCallCheck( char * szFuncCall, int iArgs )
{
   HB_FUNCINFO * f = hb_StdFunc;
   int i = 0;
   int iPos = -1;
   int iCmp;

   while( f[ i ].cFuncName )
   {
      iCmp = strncmp( szFuncCall, f[ i ].cFuncName, 4 );
      if( iCmp == 0 )
         iCmp = strncmp( szFuncCall, f[ i ].cFuncName, strlen( szFuncCall ) );
      if( iCmp == 0 )
      {
         iPos = i;
         break;
      }
      else
         ++i;
   }

   if( iPos >= 0 && ( f[ iPos ].iMinParam != -1 ) )
   {
      if( iArgs < f[ iPos ].iMinParam || ( f[ iPos ].iMaxParam != -1 && iArgs > f[ iPos ].iMaxParam ) )
      {
#if defined( HARBOUR_STRICT_CLIPPER_COMPATIBILITY )
         /* Clipper way */
         hb_compGenError( hb_comp_szErrors, 'E', HB_COMP_ERR_CHECKING_ARGS, szFuncCall, NULL );
#else
         char szMsg[ 40 ];

         if( f[ iPos ].iMaxParam == -1 )
            sprintf( szMsg, "\nPassed: %i, expected: at least %i", iArgs, f[ iPos ].iMinParam );
         else if( f[ iPos ].iMinParam == f[ iPos ].iMaxParam )
            sprintf( szMsg, "\nPassed: %i, expected: %i", iArgs, f[ iPos ].iMinParam );
         else
            sprintf( szMsg, "\nPassed: %i, expected: %i - %i", iArgs, f[ iPos ].iMinParam, f[ iPos ].iMaxParam );

         hb_compGenError( hb_comp_szErrors, 'E', HB_COMP_ERR_CHECKING_ARGS, szFuncCall, szMsg );
#endif
      }
   }
}

