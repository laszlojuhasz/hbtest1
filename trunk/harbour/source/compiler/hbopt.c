/*
 * $Id$
 */

/*
 * Harbour Project source code:
 *    Compiler PCODE optimizer
 *
 * Copyright 2007 Przemyslaw Czerpak <druzus / at / priv.onet.pl>
 * Copyright 2008 Mindaugas Kavaliauskas <dbtopas / at / dbtopas.lt>
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

#include "hbcomp.h"
#include "hbassert.h"

#define HB_OPT_FUNC( func ) HB_PCODE_FUNC( func, void* )
typedef HB_OPT_FUNC( HB_OPT_FUNC_ );
typedef HB_OPT_FUNC_ * HB_OPT_FUNC_PTR;


static HB_OPT_FUNC( hb_p_poplocal )
{
   BYTE * pVar = &pFunc->pCode[ lPCodePos + 1 ];
   SHORT iVar = HB_PCODE_MKSHORT( pVar );

   HB_SYMBOL_UNUSED( cargo );

   if( HB_LIM_INT8( iVar ) )
   {
      pFunc->pCode[ lPCodePos ] = HB_P_POPLOCALNEAR;
      hb_compNOOPfill( pFunc, lPCodePos + 2, 1, FALSE, FALSE );
   }

   return 3;
}

static HB_OPT_FUNC( hb_p_pushlocal )
{
   BYTE * pVar = &pFunc->pCode[ lPCodePos + 1 ];
   SHORT iVar = HB_PCODE_MKSHORT( pVar );

   HB_SYMBOL_UNUSED( cargo );

   if( pFunc->pCode[ lPCodePos + 3 ] == HB_P_POPLOCAL &&
      HB_PCODE_MKSHORT( &pFunc->pCode[ lPCodePos + 4 ] ) == iVar &&
      ! hb_compHasJump( pFunc, lPCodePos + 3 ) )
   {
      hb_compNOOPfill( pFunc, lPCodePos, 6, FALSE, FALSE );
   }
   else if( pFunc->pCode[ lPCodePos + 3 ] == HB_P_POPLOCALNEAR &&
            ( SCHAR ) pFunc->pCode[ lPCodePos + 4 ] == iVar &&
            ! hb_compHasJump( pFunc, lPCodePos + 3 ) )
   {
      hb_compNOOPfill( pFunc, lPCodePos, 5, FALSE, FALSE );
   }
   else if( pFunc->pCode[ lPCodePos + 3 ] == HB_P_POP &&
            ! hb_compHasJump( pFunc, lPCodePos + 3 ) )
   {
      hb_compNOOPfill( pFunc, lPCodePos, 4, FALSE, FALSE );
   }
   else if( HB_LIM_INT8( iVar ) )
   {
      pFunc->pCode[ lPCodePos ] = HB_P_PUSHLOCALNEAR;
      hb_compNOOPfill( pFunc, lPCodePos + 2, 1, FALSE, FALSE );
   }

   return 3;
}

static HB_OPT_FUNC( hb_p_pushlocalnear )
{
   HB_SYMBOL_UNUSED( cargo );

   if( pFunc->pCode[ lPCodePos + 2 ] == HB_P_POPLOCAL &&
      ( SCHAR ) pFunc->pCode[ lPCodePos + 1 ] ==
      HB_PCODE_MKSHORT( &pFunc->pCode[ lPCodePos + 3 ] ) &&
      ! hb_compHasJump( pFunc, lPCodePos + 2 ) )
   {
      hb_compNOOPfill( pFunc, lPCodePos, 5, FALSE, FALSE );
   }
   else if( pFunc->pCode[ lPCodePos + 2 ] == HB_P_POPLOCALNEAR &&
            pFunc->pCode[ lPCodePos + 1 ] == pFunc->pCode[ lPCodePos + 3 ] &&
            ! hb_compHasJump( pFunc, lPCodePos + 2 ) )
   {
      hb_compNOOPfill( pFunc, lPCodePos, 4, FALSE, FALSE );
   }
   else if( pFunc->pCode[ lPCodePos + 2 ] == HB_P_POP &&
            ! hb_compHasJump( pFunc, lPCodePos + 2 ) )
   {
      hb_compNOOPfill( pFunc, lPCodePos, 3, FALSE, FALSE );
   }

   return 2;
}

static HB_OPT_FUNC( hb_p_localaddint )
{
   BYTE * pVar = &pFunc->pCode[ lPCodePos + 1 ];
   SHORT iVar = HB_PCODE_MKSHORT( pVar );

   HB_SYMBOL_UNUSED( cargo );

   if( HB_LIM_INT8( iVar ) )
   {
      pVar[ 0 ] = HB_P_LOCALNEARADDINT;
      pVar[ 1 ] = HB_LOBYTE( iVar );
      hb_compNOOPfill( pFunc, lPCodePos, 1, FALSE, FALSE );
   }

   return 5;
}

static HB_OPT_FUNC( hb_p_pushstatic )
{
   HB_SYMBOL_UNUSED( cargo );

   if( pFunc->pCode[ lPCodePos + 3 ] == HB_P_POPSTATIC &&
       HB_PCODE_MKUSHORT( &pFunc->pCode[ lPCodePos + 1 ] ) ==
       HB_PCODE_MKUSHORT( &pFunc->pCode[ lPCodePos + 4 ] ) &&
       ! hb_compHasJump( pFunc, lPCodePos + 3 ) )
   {
      hb_compNOOPfill( pFunc, lPCodePos, 6, FALSE, FALSE );
   }
   else if( pFunc->pCode[ lPCodePos + 3 ] == HB_P_POP &&
            ! hb_compHasJump( pFunc, lPCodePos + 3 ) )
   {
      hb_compNOOPfill( pFunc, lPCodePos, 4, FALSE, FALSE );
   }

   return 3;
}

static HB_OPT_FUNC( hb_p_pushmemvar )
{
   HB_SYMBOL_UNUSED( cargo );

   if( pFunc->pCode[ lPCodePos + 3 ] == HB_P_POPMEMVAR &&
       HB_PCODE_MKUSHORT( &pFunc->pCode[ lPCodePos + 1 ] ) ==
       HB_PCODE_MKUSHORT( &pFunc->pCode[ lPCodePos + 4 ] ) &&
       ! hb_compHasJump( pFunc, lPCodePos + 3 ) )
   {
      hb_compNOOPfill( pFunc, lPCodePos, 6, FALSE, FALSE );
   }
   else if( pFunc->pCode[ lPCodePos + 3 ] == HB_P_POP &&
            ! hb_compHasJump( pFunc, lPCodePos + 3 ) )
   {
      hb_compNOOPfill( pFunc, lPCodePos, 4, FALSE, FALSE );
   }

   return 3;
}

static HB_OPT_FUNC( hb_p_pushnil )
{
   HB_SYMBOL_UNUSED( cargo );

   if( pFunc->pCode[ lPCodePos + 1 ] == HB_P_POP &&
       ! hb_compHasJump( pFunc, lPCodePos + 1 ) )
   {
      hb_compNOOPfill( pFunc, lPCodePos, 2, FALSE, FALSE );
   }

   return 1;
}

static HB_OPT_FUNC( hb_p_false )
{
   HB_SYMBOL_UNUSED( cargo );

   switch( pFunc->pCode[ lPCodePos + 1 ] )
   {
      case HB_P_POP:
      case HB_P_NOT:
      case HB_P_JUMPFALSENEAR:
      case HB_P_JUMPFALSE:
      case HB_P_JUMPFALSEFAR:
      case HB_P_JUMPTRUENEAR:
      case HB_P_JUMPTRUE:
      case HB_P_JUMPTRUEFAR:
         if( ! hb_compHasJump( pFunc, lPCodePos + 1 ) )
         {
            int iCount = 1;

            switch( pFunc->pCode[ lPCodePos + 1 ] )
            {
               case HB_P_JUMPFALSENEAR:
                  pFunc->pCode[ lPCodePos + 1 ] = HB_P_JUMPNEAR;
                  break;
               case HB_P_JUMPFALSE:
                  pFunc->pCode[ lPCodePos + 1 ] = HB_P_JUMP;
                  break;
               case HB_P_JUMPFALSEFAR:
                  pFunc->pCode[ lPCodePos + 1 ] = HB_P_JUMPFAR;
                  break;
               case HB_P_NOT:
                  pFunc->pCode[ lPCodePos + 1 ] = HB_P_TRUE;
                  break;
               case HB_P_POP:
                  iCount = 2;
                  break;
               case HB_P_JUMPTRUENEAR:
                  iCount = 3;
                  break;
               case HB_P_JUMPTRUE:
                  iCount = 4;
                  break;
               case HB_P_JUMPTRUEFAR:
                  iCount = 5;
                  break;
            }
            hb_compNOOPfill( pFunc, lPCodePos, iCount, FALSE, FALSE );
         }
         break;
   }
   return 1;
}

static HB_OPT_FUNC( hb_p_true )
{
   HB_SYMBOL_UNUSED( cargo );

   switch( pFunc->pCode[ lPCodePos + 1 ] )
   {
      case HB_P_POP:
      case HB_P_NOT:
      case HB_P_JUMPTRUENEAR:
      case HB_P_JUMPTRUE:
      case HB_P_JUMPTRUEFAR:
      case HB_P_JUMPFALSENEAR:
      case HB_P_JUMPFALSE:
      case HB_P_JUMPFALSEFAR:
         if( ! hb_compHasJump( pFunc, lPCodePos + 1 ) )
         {
            int iCount = 1;
   
            switch( pFunc->pCode[ lPCodePos + 1 ] )
            {
               case HB_P_JUMPTRUENEAR:
                  pFunc->pCode[ lPCodePos + 1 ] = HB_P_JUMPNEAR;
                  break;
               case HB_P_JUMPTRUE:
                  pFunc->pCode[ lPCodePos + 1 ] = HB_P_JUMP;
                  break;
               case HB_P_JUMPTRUEFAR:
                  pFunc->pCode[ lPCodePos + 1 ] = HB_P_JUMPFAR;
                  break;
               case HB_P_NOT:
                  pFunc->pCode[ lPCodePos + 1 ] = HB_P_FALSE;
                  break;
               case HB_P_POP:
                  iCount = 2;
                  break;
               case HB_P_JUMPFALSENEAR:
                  iCount = 3;
                  break;
               case HB_P_JUMPFALSE:
                  iCount = 4;
                  break;
               case HB_P_JUMPFALSEFAR:
                  iCount = 5;
                  break;
            }
            hb_compNOOPfill( pFunc, lPCodePos, iCount, FALSE, FALSE );
         }
         break;
   }
   return 1;
}

static HB_OPT_FUNC( hb_p_duplicate )
{
   HB_SYMBOL_UNUSED( cargo );

   switch( pFunc->pCode[ lPCodePos + 1 ] )
   {
      case HB_P_JUMPTRUEFAR:
      case HB_P_JUMPFALSEFAR:
         if( pFunc->pCode[ lPCodePos + 5 ] == HB_P_POP )
         {
            BYTE * pAddr = &pFunc->pCode[ lPCodePos + 2 ];
            LONG lOffset = HB_PCODE_MKINT24( pAddr ), lLastOffset = 0;
            ULONG ulNewPos = lPCodePos + 1 + lOffset;
            BOOL fNot = FALSE, fRepeat = TRUE;

            do
            {
               if( pFunc->pCode[ ulNewPos ] == HB_P_DUPLICATE )
               {
                  if( lOffset > 0 )
                     hb_p_duplicate( pFunc, ulNewPos, NULL );
               }

               if( pFunc->pCode[ ulNewPos ] == HB_P_NOOP )
               {
                  ulNewPos++;
                  lOffset++;
               }
               else if( pFunc->pCode[ ulNewPos ] == HB_P_NOT )
               {
                  ulNewPos++;
                  lOffset++;
                  fNot = !fNot;
               }
               else if( pFunc->pCode[ ulNewPos ] == HB_P_DUPLICATE &&
                        ( pFunc->pCode[ ulNewPos + 1 ] == HB_P_JUMPTRUEFAR ||
                          pFunc->pCode[ ulNewPos + 1 ] == HB_P_JUMPFALSEFAR ) )
               {
                  LONG lJump;
                  if( pFunc->pCode[ ulNewPos + 1 ] != pFunc->pCode[ lPCodePos + 1 ] )
                     fNot = !fNot;
                  lJump = fNot ? 4 : HB_PCODE_MKINT24( &pFunc->pCode[ ulNewPos + 2 ] );
                  lOffset += lJump + 1;
                  ulNewPos = lPCodePos + 1 + lOffset;
                  fRepeat = lJump > 0;
               }
               else
                  fRepeat = FALSE;

               if( !fNot )
                  lLastOffset = lOffset;
            }
            while( fRepeat );

            if( ( pFunc->pCode[ ulNewPos ] == HB_P_JUMPTRUEFAR ||
                  pFunc->pCode[ ulNewPos ] == HB_P_JUMPFALSEFAR ) &&
                !hb_compHasJump( pFunc, lPCodePos + 1 ) &&
                !hb_compHasJump( pFunc, lPCodePos + 5 ) )
            {
               if( pFunc->pCode[ ulNewPos ] != pFunc->pCode[ lPCodePos + 1 ] )
                  fNot = !fNot;
               if( fNot )
                  lOffset += 4;
               else
                  lOffset += HB_PCODE_MKINT24( &pFunc->pCode[ ulNewPos + 1 ] );

               HB_PUT_LE_UINT24( pAddr, lOffset );
               hb_compNOOPfill( pFunc, lPCodePos, 1, FALSE, FALSE );
               hb_compNOOPfill( pFunc, lPCodePos + 5, 1, FALSE, FALSE );
            }
            else if( lLastOffset )
            {
               HB_PUT_LE_UINT24( pAddr, lLastOffset );
            }
         }
         break;
   }
   return 1;
}

static HB_OPT_FUNC( hb_p_not )
{
   BYTE opcode;

   HB_SYMBOL_UNUSED( cargo );

   switch( pFunc->pCode[ lPCodePos + 1 ] )
   {
      case HB_P_NOT:
         opcode = HB_P_NOOP;
         break;
      case HB_P_JUMPTRUENEAR:
         opcode = HB_P_JUMPFALSENEAR;
         break;
      case HB_P_JUMPTRUE:
         opcode = HB_P_JUMPFALSE;
         break;
      case HB_P_JUMPTRUEFAR:
         opcode = HB_P_JUMPFALSEFAR;
         break;
      case HB_P_JUMPFALSENEAR:
         opcode = HB_P_JUMPTRUENEAR;
         break;
      case HB_P_JUMPFALSE:
         opcode = HB_P_JUMPTRUE;
         break;
      case HB_P_JUMPFALSEFAR:
         opcode = HB_P_JUMPTRUEFAR;
         break;
/* This optimization will be enabled in the future in a little bit differ form */
#if 0
      case HB_P_DUPLICATE:
         if( ( pFunc->pCode[ lPCodePos + 2 ] == HB_P_JUMPTRUEFAR ||
               pFunc->pCode[ lPCodePos + 2 ] == HB_P_JUMPFALSEFAR ) &&
             pFunc->pCode[ lPCodePos + 6 ] == HB_P_POP )
         {
            BYTE * pAddr = &pFunc->pCode[ lPCodePos + 3 ];
            LONG lOffset = HB_PCODE_MKINT24( pAddr );

            if( lOffset > 0 )
            {
               hb_p_duplicate( pFunc, lPCodePos + 1, NULL );
               lOffset = HB_PCODE_MKINT24( pAddr );
            }

            if( ( pFunc->pCode[ lPCodePos + 1 ] == HB_P_NOT ||
                  ( pFunc->pCode[ lPCodePos + 1 ] == HB_P_DUPLICATE &&
                    pFunc->pCode[ lPCodePos + lOffset + 2 ] == HB_P_NOT ) ) &&
                ! hb_compHasJump( pFunc, lPCodePos + 1 ) )
            {
               hb_compNOOPfill( pFunc, lPCodePos, 1, FALSE, FALSE );
               if( pFunc->pCode[ lPCodePos + 2 ] == HB_P_JUMPTRUEFAR )
                  pFunc->pCode[ lPCodePos + 2 ] = HB_P_JUMPFALSEFAR;
               else
                  pFunc->pCode[ lPCodePos + 2 ] = HB_P_JUMPTRUEFAR;
               if( pFunc->pCode[ lPCodePos + 1 ] == HB_P_DUPLICATE )
               {
                  ++lOffset;
                  HB_PUT_LE_UINT24( pAddr, lOffset );
               }
            }
         }
         /* no break; */
#endif
      default:
         opcode = HB_P_LAST_PCODE;
         break;
   }

   if( opcode < HB_P_LAST_PCODE &&
       ! hb_compHasJump( pFunc, lPCodePos + 1 ) )
   {
      hb_compNOOPfill( pFunc, lPCodePos, 1, FALSE, FALSE );
      if( opcode == HB_P_NOOP )
         hb_compNOOPfill( pFunc, lPCodePos + 1, 1, FALSE, FALSE );
      else
         pFunc->pCode[ lPCodePos + 1 ] = opcode;
   }
   return 1;
}

static HB_OPT_FUNC( hb_p_jumpfar )
{
   BYTE * pAddr = &pFunc->pCode[ lPCodePos + 1 ];
   LONG lOffset = HB_PCODE_MKINT24( pAddr );
   ULONG ulNewPos = lPCodePos + lOffset;
   BOOL fLine = FALSE;

   HB_SYMBOL_UNUSED( cargo );

   if( lOffset == 4 )
   {
      hb_compNOOPfill( pFunc, lPCodePos, 4, FALSE, FALSE );
   }
   else
   {
      if( pFunc->pCode[ ulNewPos ] == HB_P_LINE )
      {
         fLine = TRUE;
         ulNewPos += 3;
         lOffset += 3;
      }
      switch( pFunc->pCode[ ulNewPos ] )
      {
         case HB_P_JUMPFAR:
            lOffset += HB_PCODE_MKINT24( &pFunc->pCode[ ulNewPos + 1 ] );
            if( !fLine || pFunc->pCode[ lPCodePos + lOffset ] == HB_P_LINE )
               HB_PUT_LE_UINT24( pAddr, lOffset );
            break;

         case HB_P_JUMPFALSEFAR:
            ulNewPos += HB_PCODE_MKINT24( &pFunc->pCode[ ulNewPos + 1 ] );
            if( ulNewPos == lPCodePos + 4 && ( !fLine ||
                ( pFunc->pCode[ ulNewPos ] == HB_P_LINE &&
                  pFunc->pCode[ lPCodePos + lOffset + 4 ] == HB_P_LINE ) ) )
            {
               pFunc->pCode[ lPCodePos ] = HB_P_JUMPTRUEFAR;
               HB_PUT_LE_UINT24( pAddr, lOffset + 4 );
            }
            break;

         case HB_P_JUMPTRUEFAR:
            ulNewPos += HB_PCODE_MKINT24( &pFunc->pCode[ ulNewPos + 1 ] );
            if( ulNewPos == lPCodePos + 4 && ( !fLine ||
                ( pFunc->pCode[ ulNewPos ] == HB_P_LINE &&
                  pFunc->pCode[ lPCodePos + lOffset + 4 ] == HB_P_LINE ) ) )
            {
               pFunc->pCode[ lPCodePos ] = HB_P_JUMPFALSEFAR;
               HB_PUT_LE_UINT24( pAddr, lOffset + 4 );
            }
            break;
      }
   }
   return 4;
}

static HB_OPT_FUNC( hb_p_jumpfalsefar )
{
   BYTE * pAddr = &pFunc->pCode[ lPCodePos + 1 ];
   LONG lOffset = HB_PCODE_MKINT24( pAddr );
   ULONG ulNewPos = lPCodePos + lOffset;
   BOOL fLine = FALSE;

   HB_SYMBOL_UNUSED( cargo );

   if( lOffset == 8 && pFunc->pCode[ lPCodePos + 4 ] == HB_P_JUMPFAR &&
       ! hb_compHasJump( pFunc, lPCodePos + 4 ) )
   {
      hb_compNOOPfill( pFunc, lPCodePos, 4, FALSE, FALSE );
      pFunc->pCode[ lPCodePos + 4 ] = HB_P_JUMPTRUEFAR;
   }
   else if( lOffset == 11 && pFunc->pCode[ lPCodePos + 4 ] == HB_P_LINE &&
            pFunc->pCode[ lPCodePos + 11 ] == HB_P_LINE &&
            pFunc->pCode[ lPCodePos + 7 ] == HB_P_JUMPFAR &&
            pFunc->pCode[ lPCodePos + 7 +
               HB_PCODE_MKINT24( &pFunc->pCode[ lPCodePos + 8 ] ) ] == HB_P_LINE &&
            ! hb_compHasJump( pFunc, lPCodePos + 4 ) &&
            ! hb_compHasJump( pFunc, lPCodePos + 7 ) )
   {
      hb_compNOOPfill( pFunc, lPCodePos, 7, FALSE, FALSE );
      pFunc->pCode[ lPCodePos + 7 ] = HB_P_JUMPTRUEFAR;
   }
   else
   {
      if( pFunc->pCode[ ulNewPos ] == HB_P_LINE )
      {
         fLine = TRUE;
         ulNewPos += 3;
         lOffset += 3;
      }
      if( pFunc->pCode[ ulNewPos ] == HB_P_JUMPFAR )
      {
         lOffset += HB_PCODE_MKINT24( &pFunc->pCode[ ulNewPos + 1 ] );
         if( !fLine || pFunc->pCode[ lPCodePos + lOffset ] == HB_P_LINE )
            HB_PUT_LE_UINT24( pAddr, lOffset );
      }
   }
   return 4;
}

static HB_OPT_FUNC( hb_p_jumptruefar )
{
   BYTE * pAddr = &pFunc->pCode[ lPCodePos + 1 ];
   LONG lOffset = HB_PCODE_MKINT24( pAddr );
   ULONG ulNewPos = lPCodePos + lOffset;
   BOOL fLine = FALSE;

   HB_SYMBOL_UNUSED( cargo );

   if( lOffset == 8 && pFunc->pCode[ lPCodePos + 4 ] == HB_P_JUMPFAR &&
       ! hb_compHasJump( pFunc, lPCodePos + 4 ) )
   {
      hb_compNOOPfill( pFunc, lPCodePos, 4, FALSE, FALSE );
      pFunc->pCode[ lPCodePos + 4 ] = HB_P_JUMPFALSEFAR;
   }
   else if( lOffset == 11 && pFunc->pCode[ lPCodePos + 4 ] == HB_P_LINE &&
            pFunc->pCode[ lPCodePos + 11 ] == HB_P_LINE &&
            pFunc->pCode[ lPCodePos + 7 ] == HB_P_JUMPFAR &&
            pFunc->pCode[ lPCodePos + 7 +
               HB_PCODE_MKINT24( &pFunc->pCode[ lPCodePos + 8 ] ) ] == HB_P_LINE &&
            ! hb_compHasJump( pFunc, lPCodePos + 4 ) &&
            ! hb_compHasJump( pFunc, lPCodePos + 7 ) )
   {
      hb_compNOOPfill( pFunc, lPCodePos, 7, FALSE, FALSE );
      pFunc->pCode[ lPCodePos + 7 ] = HB_P_JUMPFALSEFAR;
   }
   else
   {
      if( pFunc->pCode[ ulNewPos ] == HB_P_LINE )
      {
         fLine = TRUE;
         ulNewPos += 3;
         lOffset += 3;
      }
      if( pFunc->pCode[ ulNewPos ] == HB_P_JUMPFAR )
      {
         lOffset += HB_PCODE_MKINT24( &pFunc->pCode[ ulNewPos + 1 ] );
         if( !fLine || pFunc->pCode[ lPCodePos + lOffset ] == HB_P_LINE )
            HB_PUT_LE_UINT24( pAddr, lOffset );
      }
   }
   return 4;
}

static HB_OPT_FUNC( hb_p_switch )
{
   USHORT usCases = HB_PCODE_MKUSHORT( &pFunc->pCode[ lPCodePos + 1 ] ), us;
   ULONG ulStart = lPCodePos;

   HB_SYMBOL_UNUSED( cargo );

   lPCodePos += 3;
   for( us = 0; us < usCases; ++us )
   {
      switch( pFunc->pCode[ lPCodePos ] )
      {
         case HB_P_PUSHBYTE:
            lPCodePos += 2;
            break;
         case HB_P_PUSHINT:
            lPCodePos += 3;
            break;
         case HB_P_PUSHLONG:
         case HB_P_PUSHDATE:
            lPCodePos += 5;
            break;
         case HB_P_PUSHLONGLONG:
            lPCodePos += 9;
            break;
         case HB_P_PUSHSTRSHORT:
            lPCodePos += 2 + pFunc->pCode[ lPCodePos + 1 ];
            break;
         case HB_P_PUSHSTR:
            lPCodePos += 3 + HB_PCODE_MKUSHORT( &pFunc->pCode[ lPCodePos + 1 ] );
            break;
         case HB_P_PUSHSTRLARGE:
            lPCodePos += 4 + HB_PCODE_MKUINT24( &pFunc->pCode[ lPCodePos + 1 ] );
            break;
         case HB_P_PUSHNIL:
            /* default clause */
            us = usCases;
            lPCodePos++;
            break;
      }
      switch( pFunc->pCode[ lPCodePos ] )
      {
         case HB_P_JUMPNEAR:
            lPCodePos += 2;
            break;
         case HB_P_JUMP:
            lPCodePos += 3;
            break;
         /*case HB_P_JUMPFAR:*/
         default:
            lPCodePos += 4;
            break;
      }
   }

   return lPCodePos - ulStart;
}

static HB_OPT_FUNC( hb_p_function )
{
   HB_SYMBOL_UNUSED( cargo );

   if( pFunc->pCode[ lPCodePos + 3 ] == HB_P_RETVALUE &&
       ! hb_compHasJump( pFunc, lPCodePos + 3 ) )
   {
      pFunc->pCode[ lPCodePos ] = HB_P_DO;
      hb_compNOOPfill( pFunc, lPCodePos + 3, 1, FALSE, FALSE );
   }
   return 3;
}

static HB_OPT_FUNC( hb_p_functionshort )
{
   HB_SYMBOL_UNUSED( cargo );

   if( pFunc->pCode[ lPCodePos + 2 ] == HB_P_RETVALUE &&
       ! hb_compHasJump( pFunc, lPCodePos + 2 ) )
   {
      pFunc->pCode[ lPCodePos ] = HB_P_DOSHORT;
      hb_compNOOPfill( pFunc, lPCodePos + 2, 1, FALSE, FALSE );
   }
   return 2;
}

static HB_OPT_FUNC( hb_p_macrofunc )
{
   HB_SYMBOL_UNUSED( cargo );

   if( pFunc->pCode[ lPCodePos + 3 ] == HB_P_RETVALUE &&
       ! hb_compHasJump( pFunc, lPCodePos + 3 ) )
   {
      pFunc->pCode[ lPCodePos ] = HB_P_MACRODO;
      hb_compNOOPfill( pFunc, lPCodePos + 3, 1, FALSE, FALSE );
   }
   return 3;
}

static HB_OPT_FUNC( hb_p_endblock )
{
   HB_SYMBOL_UNUSED( cargo );

   if( lPCodePos + 1 < pFunc->lPCodePos &&
       pFunc->pCode[ lPCodePos + 1 ] == HB_P_ENDBLOCK )
   {
      hb_compNOOPfill( pFunc, lPCodePos, 1, FALSE, FALSE );
   }
   return 1;
}

static HB_OPT_FUNC( hb_p_endproc )
{
   HB_SYMBOL_UNUSED( cargo );

   if( lPCodePos + 1 < pFunc->lPCodePos &&
       pFunc->pCode[ lPCodePos + 1 ] == HB_P_ENDPROC )
   {
      hb_compNOOPfill( pFunc, lPCodePos, 1, FALSE, FALSE );
   }
   return 1;
}

/* NOTE: The  order of functions have to match the order of opcodes mnemonics
 */
static const HB_OPT_FUNC_PTR s_opt_table[] =
{
   NULL,                       /* HB_P_AND,                  */
   NULL,                       /* HB_P_ARRAYPUSH,            */
   NULL,                       /* HB_P_ARRAYPOP,             */
   NULL,                       /* HB_P_ARRAYDIM,             */
   NULL,                       /* HB_P_ARRAYGEN,             */
   NULL,                       /* HB_P_EQUAL,                */
   hb_p_endblock,              /* HB_P_ENDBLOCK,             */
   hb_p_endproc,               /* HB_P_ENDPROC,              */
   NULL,                       /* HB_P_EXACTLYEQUAL,         */
   hb_p_false,                 /* HB_P_FALSE,                */
   NULL,                       /* HB_P_FORTEST,              */
   hb_p_function,              /* HB_P_FUNCTION,             */
   hb_p_functionshort,         /* HB_P_FUNCTIONSHORT,        */
   NULL,                       /* HB_P_FRAME,                */
   NULL,                       /* HB_P_FUNCPTR,              */
   NULL,                       /* HB_P_GREATER,              */
   NULL,                       /* HB_P_GREATEREQUAL,         */
   NULL,                       /* HB_P_DEC,                  */
   NULL,                       /* HB_P_DIVIDE,               */
   NULL,                       /* HB_P_DO,                   */
   NULL,                       /* HB_P_DOSHORT,              */
   hb_p_duplicate,             /* HB_P_DUPLICATE,            */
   NULL,                       /* HB_P_DUPLTWO,              */
   NULL,                       /* HB_P_INC,                  */
   NULL,                       /* HB_P_INSTRING,             */
   NULL,                       /* HB_P_JUMPNEAR,             */
   NULL,                       /* HB_P_JUMP,                 */
   hb_p_jumpfar,               /* HB_P_JUMPFAR,              */
   NULL,                       /* HB_P_JUMPFALSENEAR,        */
   NULL,                       /* HB_P_JUMPFALSE,            */
   hb_p_jumpfalsefar,          /* HB_P_JUMPFALSEFAR,         */
   NULL,                       /* HB_P_JUMPTRUENEAR,         */
   NULL,                       /* HB_P_JUMPTRUE,             */
   hb_p_jumptruefar,           /* HB_P_JUMPTRUEFAR,          */
   NULL,                       /* HB_P_LESSEQUAL,            */
   NULL,                       /* HB_P_LESS,                 */
   NULL,                       /* HB_P_LINE,                 */
   NULL,                       /* HB_P_LOCALNAME,            */
   NULL,                       /* HB_P_MACROPOP,             */
   NULL,                       /* HB_P_MACROPOPALIASED,      */
   NULL,                       /* HB_P_MACROPUSH,            */
   NULL,                       /* HB_P_MACROARRAYGEN,        */
   NULL,                       /* HB_P_MACROPUSHLIST,        */
   NULL,                       /* HB_P_MACROPUSHINDEX,       */
   NULL,                       /* HB_P_MACROPUSHPARE,        */
   NULL,                       /* HB_P_MACROPUSHALIASED,     */
   NULL,                       /* HB_P_MACROSYMBOL,          */
   NULL,                       /* HB_P_MACROTEXT,            */
   NULL,                       /* HB_P_MESSAGE,              */
   NULL,                       /* HB_P_MINUS,                */
   NULL,                       /* HB_P_MODULUS,              */
   NULL,                       /* HB_P_MODULENAME,           */
                               /* start: pcodes generated by macro compiler */
   NULL,                       /* HB_P_MMESSAGE,             */
   NULL,                       /* HB_P_MPOPALIASEDFIELD,     */
   NULL,                       /* HB_P_MPOPALIASEDVAR,       */
   NULL,                       /* HB_P_MPOPFIELD,            */
   NULL,                       /* HB_P_MPOPMEMVAR,           */
   NULL,                       /* HB_P_MPUSHALIASEDFIELD,    */
   NULL,                       /* HB_P_MPUSHALIASEDVAR,      */
   NULL,                       /* HB_P_MPUSHBLOCK,           */
   NULL,                       /* HB_P_MPUSHFIELD,           */
   NULL,                       /* HB_P_MPUSHMEMVAR,          */
   NULL,                       /* HB_P_MPUSHMEMVARREF,       */
   NULL,                       /* HB_P_MPUSHSYM,             */
   NULL,                       /* HB_P_MPUSHVARIABLE,        */
                               /* end: */
   NULL,                       /* HB_P_MULT,                 */
   NULL,                       /* HB_P_NEGATE,               */
   NULL,                       /* HB_P_NOOP,                 */
   hb_p_not,                   /* HB_P_NOT,                  */
   NULL,                       /* HB_P_NOTEQUAL,             */
   NULL,                       /* HB_P_OR,                   */
   NULL,                       /* HB_P_PARAMETER,            */
   NULL,                       /* HB_P_PLUS,                 */
   NULL,                       /* HB_P_POP,                  */
   NULL,                       /* HB_P_POPALIAS,             */
   NULL,                       /* HB_P_POPALIASEDFIELD,      */
   NULL,                       /* HB_P_POPALIASEDFIELDNEAR,  */
   NULL,                       /* HB_P_POPALIASEDVAR,        */
   NULL,                       /* HB_P_POPFIELD,             */
   hb_p_poplocal,              /* HB_P_POPLOCAL,             */
   NULL,                       /* HB_P_POPLOCALNEAR,         */
   NULL,                       /* HB_P_POPMEMVAR,            */
   NULL,                       /* HB_P_POPSTATIC,            */
   NULL,                       /* HB_P_POPVARIABLE,          */
   NULL,                       /* HB_P_POWER,                */
   NULL,                       /* HB_P_PUSHALIAS,            */
   NULL,                       /* HB_P_PUSHALIASEDFIELD,     */
   NULL,                       /* HB_P_PUSHALIASEDFIELDNEAR, */
   NULL,                       /* HB_P_PUSHALIASEDVAR,       */
   NULL,                       /* HB_P_PUSHBLOCK,            */
   NULL,                       /* HB_P_PUSHBLOCKSHORT,       */
   NULL,                       /* HB_P_PUSHFIELD,            */
   NULL,                       /* HB_P_PUSHBYTE,             */
   NULL,                       /* HB_P_PUSHINT,              */
   hb_p_pushlocal,             /* HB_P_PUSHLOCAL,            */
   hb_p_pushlocalnear,         /* HB_P_PUSHLOCALNEAR,        */
   NULL,                       /* HB_P_PUSHLOCALREF,         */
   NULL,                       /* HB_P_PUSHLONG,             */
   hb_p_pushmemvar,            /* HB_P_PUSHMEMVAR,           */
   NULL,                       /* HB_P_PUSHMEMVARREF,        */
   hb_p_pushnil,               /* HB_P_PUSHNIL,              */
   NULL,                       /* HB_P_PUSHDOUBLE,           */
   NULL,                       /* HB_P_PUSHSELF,             */
   hb_p_pushstatic,            /* HB_P_PUSHSTATIC,           */
   NULL,                       /* HB_P_PUSHSTATICREF,        */
   NULL,                       /* HB_P_PUSHSTR,              */
   NULL,                       /* HB_P_PUSHSTRSHORT,         */
   NULL,                       /* HB_P_PUSHSYM,              */
   NULL,                       /* HB_P_PUSHSYMNEAR,          */
   NULL,                       /* HB_P_PUSHVARIABLE,         */
   NULL,                       /* HB_P_RETVALUE,             */
   NULL,                       /* HB_P_SEND,                 */
   NULL,                       /* HB_P_SENDSHORT,            */
   NULL,                       /* HB_P_SEQBEGIN,             */
   NULL,                       /* HB_P_SEQEND,               */
   NULL,                       /* HB_P_SEQRECOVER,           */
   NULL,                       /* HB_P_SFRAME,               */
   NULL,                       /* HB_P_STATICS,              */
   NULL,                       /* HB_P_STATICNAME,           */
   NULL,                       /* HB_P_SWAPALIAS,            */
   hb_p_true,                  /* HB_P_TRUE,                 */
   NULL,                       /* HB_P_ZERO,                 */
   NULL,                       /* HB_P_ONE,                  */
   hb_p_macrofunc,             /* HB_P_MACROFUNC,            */
   NULL,                       /* HB_P_MACRODO,              */
   NULL,                       /* HB_P_MPUSHSTR,             */
   NULL,                       /* HB_P_LOCALNEARADDINT,      */
   NULL,                       /* HB_P_MACROPUSHREF          */
   NULL,                       /* HB_P_PUSHLONGLONG          */
   NULL,                       /* HB_P_ENUMSTART             */
   NULL,                       /* HB_P_ENUMNEXT              */
   NULL,                       /* HB_P_ENUMPREV              */
   NULL,                       /* HB_P_ENUMEND               */
   hb_p_switch,                /* HB_P_SWITCH                */
   NULL,                       /* HB_P_PUSHDATE              */
   NULL,                       /* HB_P_PLUSEQPOP             */
   NULL,                       /* HB_P_MINUSEQPOP            */
   NULL,                       /* HB_P_MULTEQPOP             */
   NULL,                       /* HB_P_DIVEQPOP              */
   NULL,                       /* HB_P_PLUSEQ                */
   NULL,                       /* HB_P_MINUSEQ               */
   NULL,                       /* HB_P_MULTEQ                */
   NULL,                       /* HB_P_DIVEQ                 */
   NULL,                       /* HB_P_WITHOBJECTSTART       */
   NULL,                       /* HB_P_WITHOBJECTMESSAGE     */
   NULL,                       /* HB_P_WITHOBJECTEND         */
   NULL,                       /* HB_P_MACROSEND             */
   NULL,                       /* HB_P_PUSHOVARREF           */
   NULL,                       /* HB_P_ARRAYPUSHREF          */
   NULL,                       /* HB_P_VFRAME                */
   NULL,                       /* HB_P_LARGEFRAME            */
   NULL,                       /* HB_P_LARGEVFRAME           */
   NULL,                       /* HB_P_PUSHSTRHIDDEN         */
   hb_p_localaddint,           /* HB_P_LOCALADDINT           */
   NULL,                       /* HB_P_MODEQPOP              */
   NULL,                       /* HB_P_EXPEQPOP              */
   NULL,                       /* HB_P_MODEQ                 */
   NULL,                       /* HB_P_EXPEQ                 */
   NULL,                       /* HB_P_DUPLUNREF             */
   NULL,                       /* HB_P_MPUSHBLOCKLARGE       */
   NULL,                       /* HB_P_MPUSHSTRLARGE         */
   NULL,                       /* HB_P_PUSHBLOCKLARGE        */
   NULL,                       /* HB_P_PUSHSTRLARGE          */
   NULL,                       /* HB_P_SWAP                  */
   NULL,                       /* HB_P_PUSHVPARAMS           */
   NULL,                       /* HB_P_PUSHUNREF             */
   NULL,                       /* HB_P_SEQALWAYS             */
   NULL,                       /* HB_P_ALWAYSBEGIN           */
   NULL,                       /* HB_P_ALWAYSEND             */
   NULL,                       /* HB_P_DECEQPOP              */
   NULL,                       /* HB_P_INCEQPOP              */
   NULL,                       /* HB_P_DECEQ                 */
   NULL,                       /* HB_P_INCEQ                 */
   NULL,                       /* HB_P_LOCALDEC              */
   NULL,                       /* HB_P_LOCALINC              */
   NULL,                       /* HB_P_LOCALINCPUSH          */
   NULL,                       /* HB_P_PUSHFUNCSYM           */
   NULL,                       /* HB_P_HASHGEN               */
   NULL,                       /* HB_P_SEQBLOCK              */
   NULL                        /* HB_P_THREADSTATICS         */
};

void hb_compOptimizePCode( HB_COMP_DECL, PFUNCTION pFunc )
{
   const HB_OPT_FUNC_PTR * pFuncTable = s_opt_table;

   HB_SYMBOL_UNUSED( HB_COMP_PARAM );

   assert( HB_P_LAST_PCODE == sizeof( s_opt_table ) / sizeof( HB_OPT_FUNC_PTR ) );

   hb_compPCodeEval( pFunc, ( const HB_PCODE_FUNC_PTR * ) pFuncTable, NULL );
}


/*
*   PCode trace optimizer
*/

#define OPT_LOCAL_FLAG_BLOCK        1
#define OPT_LOCAL_FLAG_PUSH         2
#define OPT_LOCAL_FLAG_POP          4
#define OPT_LOCAL_FLAG_PUSHREF      8
#define OPT_LOCAL_FLAG_POPSELF     16
#define OPT_LOCAL_FLAG_CHANGE      32

typedef struct 
{
  SHORT    isNumber;
  BYTE     bFlags;
} HB_OPT_LOCAL, * PHB_OPT_LOCAL;


static BOOL hb_compIsJump( BYTE bPCode )
{
   return ( bPCode >= HB_P_JUMPNEAR && bPCode <= HB_P_JUMPTRUEFAR ) || /* All jumps */
            bPCode == HB_P_SEQBEGIN || bPCode == HB_P_SEQEND || 
            bPCode == HB_P_SEQALWAYS || bPCode == HB_P_ALWAYSBEGIN;
}


static BOOL hb_compIsUncondJump( BYTE bPCode )
{
   return bPCode == HB_P_JUMPNEAR ||
          bPCode == HB_P_JUMP ||
          bPCode == HB_P_JUMPFAR ||
          bPCode == HB_P_SEQEND;
}

/*
static SHORT hb_compIsLocalOp( BYTE bCode )
{
   return bCode == HB_P_POPLOCAL || 
          bCode == HB_P_POPLOCALNEAR || 
          bCode == HB_P_PUSHLOCAL ||
          bCode == HB_P_PUSHLOCALNEAR ||
          bCode == HB_P_PUSHLOCALREF ||
          bCode == HB_P_LOCALNEARADDINT ||
          bCode == HB_P_LOCALADDINT ||
          bCode == HB_P_LOCALDEC ||
          bCode == HB_P_LOCALINC ||
          bCode == HB_P_LOCALINCPUSH;
}
*/

static SHORT hb_compLocalGetNumber( BYTE* pCode )
{
   switch( *pCode )
   {
      case HB_P_POPLOCALNEAR:
      case HB_P_PUSHLOCALNEAR:
      case HB_P_LOCALNEARADDINT:
         return * ( (signed char*) pCode + 1 );

      case HB_P_POPLOCAL:
      case HB_P_PUSHLOCAL:
      case HB_P_PUSHLOCALREF:
      case HB_P_LOCALDEC:
      case HB_P_LOCALINC:
      case HB_P_LOCALINCPUSH:
      case HB_P_LOCALADDINT:
         return HB_PCODE_MKSHORT( pCode + 1 );
   }
   assert( 0 );
   return 0;
}


static LONG hb_compJumpGetOffset( BYTE * pCode )
{
   switch( *pCode )
   {
      case HB_P_JUMPNEAR:
      case HB_P_JUMPFALSENEAR:
      case HB_P_JUMPTRUENEAR:
         return * ( ( signed char* ) pCode + 1 );

      case HB_P_JUMP:
      case HB_P_JUMPFALSE:
      case HB_P_JUMPTRUE:
         return HB_PCODE_MKSHORT( pCode + 1 );

      case HB_P_JUMPFAR:
      case HB_P_JUMPFALSEFAR:
      case HB_P_JUMPTRUEFAR:
      case HB_P_SEQBEGIN:
      case HB_P_SEQEND:
      case HB_P_SEQALWAYS:
      case HB_P_ALWAYSBEGIN:
         return HB_PCODE_MKINT24( pCode + 1 );
   }
   assert( 0 );
   return 0;
}


static void hb_compPCodeEnumScanLocals( PFUNCTION pFunc, PHB_OPT_LOCAL pLocals )
{
   ULONG   ulPos = 0, ulLastPos = 0; 
   SHORT   isVar = 0;
   BOOL    fWasJump = 0;

   while( ulPos < pFunc->lPCodePos )
   {
      if( hb_compIsJump( pFunc->pCode[ ulPos ] ) )
         fWasJump = 1;

      switch( pFunc->pCode[ ulPos ] )
      {
         case HB_P_POPLOCALNEAR: 
         case HB_P_PUSHLOCALNEAR: 
         case HB_P_LOCALNEARADDINT:
            isVar = ( signed char ) pFunc->pCode[ ulPos + 1 ];
            break;

         case HB_P_LOCALNAME:
         case HB_P_POPLOCAL:
         case HB_P_PUSHLOCAL:
         case HB_P_PUSHLOCALREF:
         case HB_P_LOCALADDINT:
         case HB_P_LOCALDEC:
         case HB_P_LOCALINC:
         case HB_P_LOCALINCPUSH:
            isVar = HB_PCODE_MKSHORT( &pFunc->pCode[ ulPos + 1 ] );
            break;
      }

      switch( pFunc->pCode[ ulPos ] )
      {
         case HB_P_POPLOCALNEAR: 
         case HB_P_POPLOCAL:
            if( isVar > 0 )
            {
               if( ulPos > 0 && pFunc->pCode[ ulLastPos ] == HB_P_PUSHSELF &&
                   ! hb_compHasJump( pFunc, ulPos ) && ! fWasJump )
               {
                  /* For real POPSELF support we need to do backward tree 
                     tracing. This is not implemented, but using fWasJump 
                     we can easy optimize Self := QSelf() at the beginning 
                     of functions. [Mindaugas]
                   */
                  pLocals[ isVar - 1 ].bFlags |= OPT_LOCAL_FLAG_POPSELF;
               }
               else
                  pLocals[ isVar - 1 ].bFlags |= OPT_LOCAL_FLAG_POP;
            }
            break;

         case HB_P_PUSHLOCALNEAR: 
         case HB_P_PUSHLOCAL:
            if( isVar > 0 )
               pLocals[ isVar - 1 ].bFlags |= OPT_LOCAL_FLAG_PUSH;
            break;

         case HB_P_PUSHLOCALREF:
            if( isVar > 0 )
            {
               ULONG  ulPosNext = ulPos + hb_compPCodeSize( pFunc, ulPos );
               BYTE   bCodeNext = pFunc->pCode[ ulPosNext ];
               BYTE   bCodeNext2 = pFunc->pCode[ ulPosNext + hb_compPCodeSize( pFunc, ulPosNext ) ];

               if( ( bCodeNext == HB_P_PUSHBLOCK ||
                     bCodeNext == HB_P_PUSHBLOCKSHORT ||
                     bCodeNext == HB_P_PUSHFIELD ||
                     bCodeNext == HB_P_PUSHBYTE ||
                     bCodeNext == HB_P_PUSHINT ||
                     bCodeNext == HB_P_PUSHLOCAL ||
                     bCodeNext == HB_P_PUSHLOCALNEAR ||
                     bCodeNext == HB_P_PUSHLONG ||
                     bCodeNext == HB_P_PUSHMEMVAR ||
                     bCodeNext == HB_P_PUSHNIL ||
                     bCodeNext == HB_P_PUSHDOUBLE ||
                     bCodeNext == HB_P_PUSHSELF ||
                     bCodeNext == HB_P_PUSHSTATIC ||
                     bCodeNext == HB_P_PUSHSTR ||
                     bCodeNext == HB_P_PUSHSTRSHORT ||
                     bCodeNext == HB_P_PUSHVARIABLE ||
                     bCodeNext == HB_P_PUSHLONGLONG ||
                     bCodeNext == HB_P_PUSHDATE ||
                     bCodeNext == HB_P_PUSHSTRHIDDEN ||
                     bCodeNext == HB_P_PUSHBLOCKLARGE ||
                     bCodeNext == HB_P_PUSHSTRLARGE ||
                     bCodeNext == HB_P_LOCALINCPUSH ) &&
                   ( bCodeNext2 == HB_P_PLUSEQPOP ||
                     bCodeNext2 == HB_P_MINUSEQPOP ||
                     bCodeNext2 == HB_P_MULTEQPOP ||
                     bCodeNext2 == HB_P_DIVEQPOP ||
                     bCodeNext2 == HB_P_PLUSEQ ||
                     bCodeNext2 == HB_P_MINUSEQ ||
                     bCodeNext2 == HB_P_MULTEQ ||
                     bCodeNext2 == HB_P_DIVEQ ||
                     bCodeNext2 == HB_P_MODEQPOP ||
                     bCodeNext2 == HB_P_EXPEQPOP ||
                     bCodeNext2 == HB_P_MODEQ ||
                     bCodeNext2 == HB_P_EXPEQ ||
                     bCodeNext2 == HB_P_DECEQPOP ||
                     bCodeNext2 == HB_P_INCEQPOP ||
                     bCodeNext2 == HB_P_DECEQ ||
                     bCodeNext2 == HB_P_INCEQ ) )
               {
                  pLocals[ isVar - 1 ].bFlags |= OPT_LOCAL_FLAG_CHANGE;
               }
               else
               {
                  pLocals[ isVar - 1 ].bFlags |= OPT_LOCAL_FLAG_PUSHREF;
               }
            }
            break;

         case HB_P_LOCALADDINT:
         case HB_P_LOCALNEARADDINT:
         case HB_P_LOCALDEC:
         case HB_P_LOCALINC:
            if( isVar > 0 )
               pLocals[ isVar - 1 ].bFlags |= OPT_LOCAL_FLAG_CHANGE;
            break;

         case HB_P_LOCALINCPUSH:
            if( isVar > 0 )
               pLocals[ isVar - 1 ].bFlags |= ( OPT_LOCAL_FLAG_CHANGE | OPT_LOCAL_FLAG_PUSH );
            break;

         case HB_P_PUSHBLOCK:
         case HB_P_PUSHBLOCKLARGE: 
         {
            BYTE *  pCode = &pFunc->pCode[ ulPos + 5 ];
            USHORT  usVarCount, usVar;

            if( pFunc->pCode[ ulPos ] == HB_P_PUSHBLOCKLARGE )
               pCode++;

            usVarCount = HB_PCODE_MKUSHORT( pCode );
            while( usVarCount-- )
            {
               pCode += 2;
               usVar = HB_PCODE_MKUSHORT( pCode );
               if( usVar > 0 ) 
                 pLocals[ usVar - 1 ].bFlags |= OPT_LOCAL_FLAG_BLOCK;
            }
            break;
         }

         /* local name is not a big usage...
         case HB_P_LOCALNAME: 
         */
      }
      ulLastPos = ulPos;
      ulPos += hb_compPCodeSize( pFunc, ulPos );
   }
}


static void hb_compPCodeEnumSelfifyLocal( PFUNCTION pFunc, SHORT isLocal )
{
   ULONG   ulPos = 0, ulLastPos = 0;

   while( ulPos < pFunc->lPCodePos )
   {
      switch( pFunc->pCode[ ulPos ] )
      {
         case HB_P_PUSHLOCALNEAR: 
            if( isLocal == ( signed char ) pFunc->pCode[ ulPos + 1 ] )
            {
               pFunc->pCode[ ulPos ] = HB_P_PUSHSELF;
               hb_compNOOPfill( pFunc, ulPos + 1, 1, FALSE, FALSE );
            }
            break;

         case HB_P_PUSHLOCAL:
            if( isLocal == HB_PCODE_MKSHORT( &pFunc->pCode[ ulPos + 1 ] ) )
            {
               pFunc->pCode[ ulPos ] = HB_P_PUSHSELF;
               hb_compNOOPfill( pFunc, ulPos + 1, 2, FALSE, FALSE );
            }
            break;

         case HB_P_POPLOCALNEAR:
            if( isLocal == ( signed char ) pFunc->pCode[ ulPos + 1 ] )
            {
               assert( ulPos > 0 && pFunc->pCode[ ulLastPos ] == HB_P_PUSHSELF &&
                       ! hb_compHasJump( pFunc, ulPos ) );
               hb_compNOOPfill( pFunc, ulLastPos, 1, FALSE, FALSE );
               hb_compNOOPfill( pFunc, ulPos, 2, FALSE, FALSE );
            }
            break;

         case HB_P_POPLOCAL:
            if( isLocal == HB_PCODE_MKSHORT( &pFunc->pCode[ ulPos + 1 ] ) )
            {
               assert( ulPos > 0 && pFunc->pCode[ ulLastPos ] == HB_P_PUSHSELF &&
                       ! hb_compHasJump( pFunc, ulPos ) );

               hb_compNOOPfill( pFunc, ulLastPos, 1, FALSE, FALSE );
               hb_compNOOPfill( pFunc, ulPos, 3, FALSE, FALSE );
            }
            break;
      }
      ulLastPos = ulPos;
      ulPos += hb_compPCodeSize( pFunc, ulPos );
   }
}


static int hb_compPCodeTraceAssignedUnused( PFUNCTION pFunc, ULONG ulPos, BYTE * pMap, SHORT isLocal )
{
   while( 1 )
   {
      if( pMap[ ulPos ] )
         return 0;

      pMap[ ulPos ] = 1;

      if( pFunc->pCode[ ulPos ] == HB_P_POPLOCAL || 
          pFunc->pCode[ ulPos ] == HB_P_POPLOCALNEAR || 
          pFunc->pCode[ ulPos ] == HB_P_PUSHLOCAL ||
          pFunc->pCode[ ulPos ] == HB_P_PUSHLOCALNEAR ||
          pFunc->pCode[ ulPos ] == HB_P_PUSHLOCALREF ||
          pFunc->pCode[ ulPos ] == HB_P_LOCALINCPUSH ||
          pFunc->pCode[ ulPos ] == HB_P_LOCALDEC ||
          pFunc->pCode[ ulPos ] == HB_P_LOCALINC ||
          pFunc->pCode[ ulPos ] == HB_P_LOCALADDINT ||
          pFunc->pCode[ ulPos ] == HB_P_LOCALNEARADDINT )

      {
         if( hb_compLocalGetNumber( pFunc->pCode + ulPos ) == isLocal ) 
         {
            if( pFunc->pCode[ ulPos ] == HB_P_POPLOCAL || 
                pFunc->pCode[ ulPos ] == HB_P_POPLOCALNEAR )
               return 0;
            else
               return 1;
         }
      }

      /* The following part of the function is standard for all recursive pcode 
         tracing, except recursive function calls are hardcoded. We can implement 
         universal recursive tracer by putting all parameters into "cargo" 
         structure. [Mindaugas] */

      if( hb_compIsJump( pFunc->pCode[ ulPos ] ) )
      {
         ULONG  ulPos2 = ulPos + hb_compJumpGetOffset( &pFunc->pCode[ ulPos ] );

         if( hb_compIsUncondJump( pFunc->pCode[ ulPos ] ) )
         {
            ulPos = ulPos2;
            continue;
         }

         if( hb_compPCodeTraceAssignedUnused( pFunc, ulPos2, pMap, isLocal ) )
            return 1;
      }
      else if( pFunc->pCode[ ulPos ] == HB_P_SWITCH ) /* Switch is multiplace jump */
      {
         USHORT us, usCount = HB_PCODE_MKUSHORT( pFunc->pCode + ulPos + 1 );

         ulPos += 3;
         for( us = 0; us < usCount; us++ )
         {
            if( hb_compPCodeTraceAssignedUnused( pFunc, ulPos, pMap, isLocal ) )
               return 1;

            ulPos += hb_compPCodeSize( pFunc, ulPos );
            ulPos += hb_compPCodeSize( pFunc, ulPos );
         }
         continue;
      }

      if( pFunc->pCode[ ulPos ] == HB_P_ENDPROC || pFunc->pCode[ ulPos ] == HB_P_ENDBLOCK )
         break;

      ulPos += hb_compPCodeSize( pFunc, ulPos );
   }
   return 0;
}


static void hb_compPCodeEnumAssignedUnused( HB_COMP_DECL, PFUNCTION pFunc, PHB_OPT_LOCAL pLocals )
{
   BYTE *  pMap;
   ULONG   ulPos = 0, ulLastPos = 0;
   SHORT   isLocal;
   USHORT  usLine = 0;

   pMap = ( BYTE * ) hb_xgrab( pFunc->lPCodePos );

   while( ulPos < pFunc->lPCodePos )
   {
      BOOL  fCheck;

      /* skip pop NIL (var := NIL), to allow force garbage collection */
      fCheck = ( pFunc->pCode[ ulPos ] == HB_P_POPLOCAL || 
                 pFunc->pCode[ ulPos ] == HB_P_POPLOCALNEAR ) &&
                 ! ( ulPos > 0 && pFunc->pCode[ ulLastPos ] == HB_P_PUSHNIL );

      if( !fCheck && ( pFunc->pCode[ ulPos ] == HB_P_LOCALDEC ||
                       pFunc->pCode[ ulPos ] == HB_P_LOCALINC ||
                       pFunc->pCode[ ulPos ] == HB_P_LOCALADDINT ||
                       pFunc->pCode[ ulPos ] == HB_P_LOCALNEARADDINT ) )
      {
         fCheck = TRUE;
      }

      if( !fCheck && pFunc->pCode[ ulPos ] == HB_P_PUSHLOCALREF )
      {
         ULONG  ulPosNext = ulPos + hb_compPCodeSize( pFunc, ulPos );
         BYTE   bCodeNext = pFunc->pCode[ ulPosNext ];
         BYTE   bCodeNext2 = pFunc->pCode[ ulPosNext + hb_compPCodeSize( pFunc, ulPosNext ) ];

         if( ( bCodeNext == HB_P_PUSHBLOCK ||
               bCodeNext == HB_P_PUSHBLOCKSHORT ||
               bCodeNext == HB_P_PUSHFIELD ||
               bCodeNext == HB_P_PUSHBYTE ||
               bCodeNext == HB_P_PUSHINT ||
               bCodeNext == HB_P_PUSHLOCAL ||
               bCodeNext == HB_P_PUSHLOCALNEAR ||
               bCodeNext == HB_P_PUSHLONG ||
               bCodeNext == HB_P_PUSHMEMVAR ||
               bCodeNext == HB_P_PUSHNIL ||
               bCodeNext == HB_P_PUSHDOUBLE ||
               bCodeNext == HB_P_PUSHSELF ||
               bCodeNext == HB_P_PUSHSTATIC ||
               bCodeNext == HB_P_PUSHSTR ||
               bCodeNext == HB_P_PUSHSTRSHORT ||
               bCodeNext == HB_P_PUSHVARIABLE ||
               bCodeNext == HB_P_PUSHLONGLONG ||
               bCodeNext == HB_P_PUSHDATE ||
               bCodeNext == HB_P_PUSHSTRHIDDEN ||
               bCodeNext == HB_P_PUSHBLOCKLARGE ||
               bCodeNext == HB_P_PUSHSTRLARGE ||
               bCodeNext == HB_P_LOCALINCPUSH ) &&
             ( bCodeNext2 == HB_P_PLUSEQPOP ||
               bCodeNext2 == HB_P_MINUSEQPOP ||
               bCodeNext2 == HB_P_MULTEQPOP ||
               bCodeNext2 == HB_P_DIVEQPOP ||
               bCodeNext2 == HB_P_PLUSEQ ||
               bCodeNext2 == HB_P_MINUSEQ ||
               bCodeNext2 == HB_P_MULTEQ ||
               bCodeNext2 == HB_P_DIVEQ ||
               bCodeNext2 == HB_P_MODEQPOP ||
               bCodeNext2 == HB_P_EXPEQPOP ||
               bCodeNext2 == HB_P_MODEQ ||
               bCodeNext2 == HB_P_EXPEQ ||
               bCodeNext2 == HB_P_DECEQPOP ||
               bCodeNext2 == HB_P_INCEQPOP ||
               bCodeNext2 == HB_P_DECEQ ||
               bCodeNext2 == HB_P_INCEQ ) )
         {
            fCheck = 1;
         }
      }

      if( fCheck && ( isLocal = hb_compLocalGetNumber( &pFunc->pCode[ ulPos ] ) ) > pFunc->wParamCount )
      {
         PVAR    pVar = pFunc->pLocals;
         SHORT   is;

         for( is = 1; is < isLocal; is++ )
            pVar = pVar->pNext;

         assert( pLocals[ isLocal - 1 ].bFlags != 0 ); 

         /* Skip detachables, referenced, optimizable self */
         if( ( pLocals[ isLocal - 1 ].bFlags & ( OPT_LOCAL_FLAG_BLOCK | OPT_LOCAL_FLAG_PUSHREF ) ) == 0 && 
             pLocals[ isLocal - 1 ].bFlags != OPT_LOCAL_FLAG_POPSELF && 
             pLocals[ isLocal - 1 ].bFlags != ( OPT_LOCAL_FLAG_PUSH | OPT_LOCAL_FLAG_POPSELF ) )
         {
            memset( pMap, 0, pFunc->lPCodePos );
            pMap[ ulPos ] = 1;

            if( ! hb_compPCodeTraceAssignedUnused( pFunc, ulPos + hb_compPCodeSize( pFunc, ulPos ), 
                                                   pMap, isLocal ) )
            {
               char    szFun[ 256 ];

               /* TOFIX: We calculate line number by simple tracking last HB_P_LINE, 
                  but it can work bad, if line number optimizator is clever enough. 
                  To obtain real line number we need one more tree scan or other 
                  algorithm. [Mindaugas] */

               hb_snprintf( szFun, sizeof( szFun ), "%s(%i)", pFunc->szName, usLine );
               hb_compGenWarning( HB_COMP_PARAM, hb_comp_szWarnings, 'W', HB_COMP_WARN_ASSIGNED_UNUSED, pVar->szName, szFun );
            }
         }
      }
      else if( pFunc->pCode[ ulPos ] == HB_P_LINE )
      {
         usLine = HB_PCODE_MKUSHORT( pFunc->pCode + ulPos + 1 );
      }
      ulLastPos = ulPos;
      ulPos += hb_compPCodeSize( pFunc, ulPos );
   }
   hb_xfree( pMap );
}


static void hb_compPCodeEnumRenumberLocals( PFUNCTION pFunc, PHB_OPT_LOCAL pLocals )
{
   ULONG   ulPos = 0;

   while( ulPos < pFunc->lPCodePos )
   {
      switch( pFunc->pCode[ ulPos ] )
      {
         case HB_P_POPLOCALNEAR: 
         case HB_P_PUSHLOCALNEAR: 
         case HB_P_LOCALNEARADDINT:
         {
            BYTE *  pVar = &pFunc->pCode[ ulPos + 1 ];
            SHORT   isVar = ( signed char ) pVar[ 0 ];

            if( isVar > 0 && pLocals[ isVar - 1 ].isNumber != isVar )
            {
               isVar = pLocals[ isVar - 1 ].isNumber;
               if( isVar > 0 )
               {
                  pVar[ 0 ] = HB_LOBYTE( isVar );
                  pVar[ 1 ] = HB_HIBYTE( isVar );
               }
               else
               {
                  hb_compNOOPfill( pFunc, ulPos, hb_compPCodeSize( pFunc, ulPos ), FALSE, FALSE );
               }
            }
            break;
         }

         case HB_P_LOCALNAME:
         case HB_P_POPLOCAL:
         case HB_P_PUSHLOCAL:
         case HB_P_PUSHLOCALREF:
         case HB_P_LOCALADDINT:
         case HB_P_LOCALDEC:
         case HB_P_LOCALINC:
         case HB_P_LOCALINCPUSH:
         {
            BYTE *  pVar = &pFunc->pCode[ ulPos + 1 ];
            SHORT   isVar = HB_PCODE_MKSHORT( pVar );

            if( isVar > 0 && pLocals[ isVar - 1 ].isNumber != isVar )
            {
               isVar = pLocals[ isVar - 1 ].isNumber;
               if( isVar > 0 )
               {
                  pVar[ 0 ] = HB_LOBYTE( isVar );
                  pVar[ 1 ] = HB_HIBYTE( isVar );
               }
               else
               {
                  hb_compNOOPfill( pFunc, ulPos, hb_compPCodeSize( pFunc, ulPos ), FALSE, FALSE );
               }
            }
            break;
         }

         case HB_P_PUSHBLOCK:
         case HB_P_PUSHBLOCKLARGE: 
         {
            BYTE *  pVar = &pFunc->pCode[ ulPos + 5 ];
            USHORT  usVarCount, isVar;

            if( pFunc->pCode[ ulPos ] == HB_P_PUSHBLOCKLARGE )
               pVar++;

            usVarCount = HB_PCODE_MKUSHORT( pVar );
            while( usVarCount-- )
            {
               pVar += 2;
               isVar = HB_PCODE_MKSHORT( pVar );

               if( isVar > 0 && pLocals[ isVar - 1 ].isNumber != isVar )
               {
                  isVar = pLocals[ isVar - 1 ].isNumber;

                  assert( isVar > 0 );  /*  We do not allow removal of detached locals */

                  pVar[ 0 ] = HB_LOBYTE( isVar );
                  pVar[ 1 ] = HB_HIBYTE( isVar );
               }
            }
            break;
         }
      }
      ulPos += hb_compPCodeSize( pFunc, ulPos );
   }
}


void hb_compPCodeTraceOptimizer( HB_COMP_DECL )
{
   PFUNCTION       pFunc = HB_COMP_PARAM->functions.pLast;
   PHB_OPT_LOCAL   pLocals;
   PVAR            pVar, * ppVar;
   USHORT          usLocalCount, usIndex;
   BOOL            fBool;

   /* Many (perhaps ALL) functions of pcode trace optimization dependes on pcodes.
      Please, check these functions if new pcode is added, or existing changed. 
      Special attention should be paid, if new pcode introduces branching, codeblocks,
      or are related to parameters, local variables. [Mindaugas] */

   assert( HB_P_LAST_PCODE == 180 );

   usLocalCount = 0;
   pVar = pFunc->pLocals;
   while( pVar )
   {
      pVar = pVar->pNext;
      usLocalCount++;
   }

   if( ! usLocalCount )
      return;

   /* TOFIX: Support for PARAMETER sentence is not implemented.
             The temporary solution is to disable optmisation at all if PARAMETER is used.  */
   {
      ULONG   ulPos = 0;

      while( ulPos < pFunc->lPCodePos )
      {
         if( pFunc->pCode[ ulPos ] == HB_P_PARAMETER )
            return;
         ulPos += hb_compPCodeSize( pFunc, ulPos );
      }
   }

   /* Initial scan */
   pLocals = ( PHB_OPT_LOCAL ) hb_xgrab( sizeof( HB_OPT_LOCAL ) * usLocalCount );
   memset( pLocals, 0, sizeof( HB_OPT_LOCAL ) * usLocalCount );
   hb_compPCodeEnumScanLocals( pFunc, pLocals );

   /* Check */
   usIndex = 0;
   pVar = pFunc->pLocals;
   while( pVar )
   {
      /* Compiler and optimizer should have the same opinion about variable usage */
      assert( ( ! ( pVar->iUsed & VU_USED ) && pLocals[ usIndex ].bFlags == 0 ) || 
              (   ( pVar->iUsed & VU_USED ) && pLocals[ usIndex ].bFlags != 0 ) );

      if( usIndex >= pFunc->wParamCount && pLocals[ usIndex ].bFlags == OPT_LOCAL_FLAG_PUSH )
      {
         char    szFun[ 256 ];

         hb_snprintf( szFun, sizeof( szFun ), "%s(%i)", pFunc->szName, pVar->iDeclLine );
         hb_compGenWarning( HB_COMP_PARAM, hb_comp_szWarnings, 'W', HB_COMP_WARN_NEVER_ASSIGNED, pVar->szName, szFun );
      }

      pVar = pVar->pNext;
      usIndex++;
   }

   /* Selfifying */
   if( HB_COMP_ISSUPPORTED(HB_COMPFLAG_OPTJUMP) && ! HB_COMP_PARAM->fDebugInfo )
   {
      pVar = pFunc->pLocals;
      for( usIndex = 0; usIndex < pFunc->wParamCount; usIndex++ )
         pVar = pVar->pNext;

      for( usIndex = pFunc->wParamCount; usIndex < usLocalCount; usIndex++ )
      {
         if( pLocals[ usIndex ].bFlags == ( OPT_LOCAL_FLAG_PUSH | OPT_LOCAL_FLAG_POPSELF ) || 
             pLocals[ usIndex ].bFlags == OPT_LOCAL_FLAG_POPSELF ) 
         {
            /* printf( "Info: %s(%d) selfifying variable '%s'\n", pFunc->szName, pVar->iDeclLine, pVar->szName ); */
            hb_compPCodeEnumSelfifyLocal( pFunc, usIndex + 1 );
            pLocals[ usIndex ].bFlags = 0;
         }
         pVar = pVar->pNext;
      }
   }

   /* Scan assigned, but not used */
   hb_compPCodeEnumAssignedUnused( HB_COMP_PARAM, pFunc, pLocals );

   /* Delete unused */
   if( HB_COMP_ISSUPPORTED(HB_COMPFLAG_OPTJUMP) && ! HB_COMP_PARAM->fDebugInfo )
   {
      fBool = 0;
      for( usIndex = pFunc->wParamCount; usIndex < usLocalCount; usIndex++ )
      {
         if( pLocals[ usIndex ].bFlags == 0 )
         {
            fBool = 1;
            break;
         }
      }

      if( fBool )
      {
         usIndex = usLocalCount = 0;
         ppVar = & pFunc->pLocals;
         pVar = pFunc->pLocals;
         while( pVar )
         {
            if( usLocalCount < pFunc->wParamCount || pLocals[ usLocalCount ].bFlags != 0 )
            {
               pLocals[ usLocalCount ].isNumber = ++usIndex;
               ppVar = & pVar->pNext;
               pVar = pVar->pNext;
            }
            else
            {
               /* printf( "Info: %s(%d) removing unused variable '%s'\n", pFunc->szName, pVar->iDeclLine, pVar->szName ); */

               /* Delete pVar from the linked list */
               *ppVar = pVar->pNext;
               hb_xfree( pVar );
               pVar = *ppVar;
            }
            usLocalCount++;
         }
         hb_compPCodeEnumRenumberLocals( pFunc, pLocals );
      }
   }
   hb_xfree( pLocals );
}
