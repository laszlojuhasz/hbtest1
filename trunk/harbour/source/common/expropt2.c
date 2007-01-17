/*
 * $Id$
 */

/*
 * Harbour Project source code:
 * Compiler Expression Optimizer - reducing expressions
 *
 * Copyright 1999 Ryszard Glab
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

/* NOTE: This must be the first definition
 *    This is a common code shared by macro and standalone compiler
 */
#define  HB_COMMON_SUPPORT

#include <math.h>
#include "hbmacro.h"
#include "hbcomp.h"
#include "hbdate.h"

static HB_EXPR_PTR hb_compExprReducePlusStrings( HB_EXPR_PTR pLeft, HB_EXPR_PTR pRight, HB_COMP_DECL )
{
   if( pLeft->value.asString.dealloc )
   {
      pLeft->value.asString.string = (char *) hb_xrealloc( pLeft->value.asString.string, pLeft->ulLength + pRight->ulLength + 1 );
      memcpy( pLeft->value.asString.string + pLeft->ulLength,
              pRight->value.asString.string, pRight->ulLength );
      pLeft->ulLength += pRight->ulLength;
      pLeft->value.asString.string[ pLeft->ulLength ] = '\0';
   }
   else
   {
      char *szString;
      szString = (char *) hb_xgrab( pLeft->ulLength + pRight->ulLength + 1 );
      memcpy( szString, pLeft->value.asString.string, pLeft->ulLength );
      memcpy( szString + pLeft->ulLength, pRight->value.asString.string, pRight->ulLength );
      pLeft->ulLength += pRight->ulLength;
      szString[ pLeft->ulLength ] = '\0';
      pLeft->value.asString.string = szString;
      pLeft->value.asString.dealloc = TRUE;
   }
   hb_compExprFree( pRight, HB_COMP_PARAM );
   return pLeft;
}

HB_EXPR_PTR hb_compExprReduceMod( HB_EXPR_PTR pSelf, HB_COMP_DECL )
{
   HB_EXPR_PTR pLeft, pRight;

   pLeft  = pSelf->value.asOperator.pLeft;
   pRight = pSelf->value.asOperator.pRight;

   if( pLeft->ExprType == HB_ET_NUMERIC && pRight->ExprType == HB_ET_NUMERIC )
   {
      if( pLeft->value.asNum.NumType == HB_ET_LONG && pRight->value.asNum.NumType == HB_ET_LONG )
      {
         if( pRight->value.asNum.val.l )
         {
            HB_LONG lVal = pLeft->value.asNum.val.l % pRight->value.asNum.val.l;

            pSelf->value.asNum.val.l = lVal;
            pSelf->value.asNum.bDec = 0;
            pSelf->value.asNum.NumType = HB_ET_LONG;
            pSelf->ExprType = HB_ET_NUMERIC;
            pSelf->ValType  = HB_EV_NUMERIC;
            hb_compExprFree( pLeft, HB_COMP_PARAM );
            hb_compExprFree( pRight, HB_COMP_PARAM );
         }
      }
   }
   else
   {
      /* TODO: Check for incompatible types e.g.  3 % "txt"
      */
   }
   return pSelf;
}

HB_EXPR_PTR hb_compExprReduceDiv( HB_EXPR_PTR pSelf, HB_COMP_DECL )
{
   HB_EXPR_PTR pLeft, pRight;

   pLeft  = pSelf->value.asOperator.pLeft;
   pRight = pSelf->value.asOperator.pRight;

   if( pLeft->ExprType == HB_ET_NUMERIC && pRight->ExprType == HB_ET_NUMERIC )
   {
      BYTE bType = ( pLeft->value.asNum.NumType & pRight->value.asNum.NumType );

      switch( bType )
      {
         case HB_ET_LONG:

            if( pRight->value.asNum.val.l )
            {
               if( pLeft->value.asNum.val.l % pRight->value.asNum.val.l == 0 )
               {
                  /* Return integer results as long */
                  pSelf->value.asNum.val.l = pLeft->value.asNum.val.l / pRight->value.asNum.val.l;
                  pSelf->value.asNum.bDec = 0;
                  pSelf->value.asNum.NumType = HB_ET_LONG;
               }
               else
               {
                  /* Return non-integer results as double */
                  pSelf->value.asNum.val.d = ( double ) pLeft->value.asNum.val.l / ( double ) pRight->value.asNum.val.l;
                  pSelf->value.asNum.bWidth = HB_DEFAULT_WIDTH;
                  pSelf->value.asNum.bDec = HB_DEFAULT_DECIMALS;
                  pSelf->value.asNum.NumType = HB_ET_DOUBLE;
               }
               pSelf->ExprType = HB_ET_NUMERIC;
            }
            break;

         case HB_ET_DOUBLE:

            if( pRight->value.asNum.val.d != 0.0 )
            {
               pSelf->value.asNum.val.d = pLeft->value.asNum.val.d / pRight->value.asNum.val.d;
               pSelf->value.asNum.bWidth = HB_DEFAULT_WIDTH;
               pSelf->value.asNum.bDec = HB_DEFAULT_DECIMALS;
               pSelf->value.asNum.NumType = HB_ET_DOUBLE;
               pSelf->ExprType = HB_ET_NUMERIC;
            }
            break;

         default:

            if( pLeft->value.asNum.NumType == HB_ET_DOUBLE )
            {
               if( pRight->value.asNum.val.l )
               {
                  pSelf->value.asNum.val.d = pLeft->value.asNum.val.d / ( double ) pRight->value.asNum.val.l;
                  pSelf->value.asNum.bWidth = HB_DEFAULT_WIDTH;
                  pSelf->value.asNum.bDec = HB_DEFAULT_DECIMALS;
               }
            }
            else
            {
               if( pRight->value.asNum.val.d != 0.0 )
               {
                  pSelf->value.asNum.val.d = ( double ) pLeft->value.asNum.val.l / pRight->value.asNum.val.d;
                  pSelf->value.asNum.bWidth = HB_DEFAULT_WIDTH;
                  pSelf->value.asNum.bDec = HB_DEFAULT_DECIMALS;
               }
            }

            pSelf->value.asNum.NumType = HB_ET_DOUBLE;
            pSelf->ExprType = HB_ET_NUMERIC;

      } /* switch bType */

      if( pSelf->ExprType == HB_ET_NUMERIC )
      {
         /* The expression was reduced - delete old components */
         pSelf->ValType = HB_EV_NUMERIC;
         hb_compExprFree( pLeft, HB_COMP_PARAM );
         hb_compExprFree( pRight, HB_COMP_PARAM );
      }
   }
   else
   {
      /* TODO: Check for incompatible types e.g.  3 / "txt"
      */
   }
   return pSelf;
}

HB_EXPR_PTR hb_compExprReduceMult( HB_EXPR_PTR pSelf, HB_COMP_DECL )
{
   HB_EXPR_PTR pLeft, pRight;

   pLeft  = pSelf->value.asOperator.pLeft;
   pRight = pSelf->value.asOperator.pRight;

   if( pLeft->ExprType == HB_ET_NUMERIC && pRight->ExprType == HB_ET_NUMERIC )
   {
      BYTE bType = ( pLeft->value.asNum.NumType & pRight->value.asNum.NumType );

      switch( bType )
      {
         case HB_ET_LONG:
         {
            HB_MAXDBL dVal = ( HB_MAXDBL ) pLeft->value.asNum.val.l * ( HB_MAXDBL ) pRight->value.asNum.val.l;

            if ( HB_DBL_LIM_LONG( dVal ) )
            {
               pSelf->value.asNum.val.l = ( HB_LONG ) dVal;
               pSelf->value.asNum.bDec = 0;
               pSelf->value.asNum.NumType = HB_ET_LONG;
            }
            else
            {
               pSelf->value.asNum.val.d = ( double ) dVal;
               pSelf->value.asNum.bWidth = HB_DEFAULT_WIDTH;
               pSelf->value.asNum.bDec = 0;
               pSelf->value.asNum.NumType = HB_ET_DOUBLE;
            }

            break;
         }

         case HB_ET_DOUBLE:
         {
            pSelf->value.asNum.val.d = pLeft->value.asNum.val.d * pRight->value.asNum.val.d;
            pSelf->value.asNum.bWidth = HB_DEFAULT_WIDTH;
            pSelf->value.asNum.bDec = pLeft->value.asNum.bDec + pRight->value.asNum.bDec;
            pSelf->value.asNum.NumType = HB_ET_DOUBLE;

            break;
         }

         default:
         {
            if( pLeft->value.asNum.NumType == HB_ET_DOUBLE )
            {
               pSelf->value.asNum.val.d = pLeft->value.asNum.val.d * ( double ) pRight->value.asNum.val.l;
               pSelf->value.asNum.bWidth = HB_DEFAULT_WIDTH;
               pSelf->value.asNum.bDec = pLeft->value.asNum.bDec;
            }
            else
            {
               pSelf->value.asNum.val.d = ( double ) pLeft->value.asNum.val.l * pRight->value.asNum.val.d;
               pSelf->value.asNum.bWidth = HB_DEFAULT_WIDTH;
               pSelf->value.asNum.bDec = pRight->value.asNum.bDec;
            }
            pSelf->value.asNum.NumType = HB_ET_DOUBLE;
         }
      }
      pSelf->ExprType = HB_ET_NUMERIC;
      pSelf->ValType  = HB_EV_NUMERIC;
      hb_compExprFree( pLeft, HB_COMP_PARAM );
      hb_compExprFree( pRight, HB_COMP_PARAM );
   }
   else
   {
      /* TODO: Check for incompatible types e.g. 3 * "txt"
      */
   }
   return pSelf;
}

HB_EXPR_PTR hb_compExprReduceMinus( HB_EXPR_PTR pSelf, HB_COMP_DECL )
{
   HB_EXPR_PTR pLeft, pRight;

   pLeft  = pSelf->value.asOperator.pLeft;
   pRight = pSelf->value.asOperator.pRight;

   if( pLeft->ExprType == HB_ET_NUMERIC && pRight->ExprType == HB_ET_NUMERIC )
   {
      BYTE bType = ( pLeft->value.asNum.NumType & pRight->value.asNum.NumType );

      switch( bType )
      {
         case HB_ET_LONG:
         {
            HB_MAXDBL dVal = ( HB_MAXDBL ) pLeft->value.asNum.val.l - ( HB_MAXDBL ) pRight->value.asNum.val.l;

            if ( HB_DBL_LIM_LONG( dVal ) )
            {
               pSelf->value.asNum.val.l = ( HB_LONG ) dVal;
               pSelf->value.asNum.bDec = 0;
               pSelf->value.asNum.NumType = HB_ET_LONG;
            }
            else
            {
               pSelf->value.asNum.val.d = ( double ) dVal;
               pSelf->value.asNum.bWidth = HB_DEFAULT_WIDTH;
               pSelf->value.asNum.bDec = 0;
               pSelf->value.asNum.NumType = HB_ET_DOUBLE;
            }

            break;
         }

         case HB_ET_DOUBLE:
         {
            pSelf->value.asNum.val.d = pLeft->value.asNum.val.d - pRight->value.asNum.val.d;
            pSelf->value.asNum.bWidth = HB_DEFAULT_WIDTH;
            if( pLeft->value.asNum.bDec < pRight->value.asNum.bDec )
               pSelf->value.asNum.bDec = pRight->value.asNum.bDec;
            else
               pSelf->value.asNum.bDec = pLeft->value.asNum.bDec;
            pSelf->value.asNum.NumType = HB_ET_DOUBLE;

            break;
         }

         default:
         {
            if( pLeft->value.asNum.NumType == HB_ET_DOUBLE )
            {
               pSelf->value.asNum.val.d = pLeft->value.asNum.val.d - ( double ) pRight->value.asNum.val.l;
               pSelf->value.asNum.bWidth = HB_DEFAULT_WIDTH;
               pSelf->value.asNum.bDec = pLeft->value.asNum.bDec;
            }
            else
            {
               pSelf->value.asNum.val.d = ( double ) pLeft->value.asNum.val.l - pRight->value.asNum.val.d;
               pSelf->value.asNum.bWidth = HB_DEFAULT_WIDTH;
               pSelf->value.asNum.bDec = pRight->value.asNum.bDec;
            }
            pSelf->value.asNum.NumType = HB_ET_DOUBLE;
         }
      }
      pSelf->ExprType = HB_ET_NUMERIC;
      pSelf->ValType  = HB_EV_NUMERIC;
      hb_compExprFree( pLeft, HB_COMP_PARAM );
      hb_compExprFree( pRight, HB_COMP_PARAM );
   }
   else if( pLeft->ExprType == HB_ET_DATE && pRight->ExprType == HB_ET_DATE )
   {
      pSelf->value.asNum.val.l = pLeft->value.asNum.val.l - pRight->value.asNum.val.l;
      pSelf->value.asNum.bDec = 0;
      pSelf->value.asNum.NumType = HB_ET_LONG;
      pSelf->ExprType = HB_ET_NUMERIC;
      pSelf->ValType  = HB_EV_NUMERIC;
      hb_compExprFree( pLeft, HB_COMP_PARAM );
      hb_compExprFree( pRight, HB_COMP_PARAM );
   }
   else if( pLeft->ExprType == HB_ET_DATE && pRight->ExprType == HB_ET_NUMERIC )
   {
      if( pRight->value.asNum.NumType == HB_ET_LONG )
      {
         pSelf->value.asNum.val.l = pLeft->value.asNum.val.l - pRight->value.asNum.val.l;
      }
      else
      {
         pSelf->value.asNum.val.l = pLeft->value.asNum.val.l - ( HB_LONG ) pRight->value.asNum.val.d;
      }
      pSelf->ExprType = HB_ET_DATE;
      pSelf->ValType  = HB_EV_DATE;
      hb_compExprFree( pLeft, HB_COMP_PARAM );
      hb_compExprFree( pRight, HB_COMP_PARAM );
   }
   else if( pLeft->ExprType == HB_ET_STRING && pRight->ExprType == HB_ET_STRING )
   {
      /* TODO:
       */
   }
   else
   {
      /* TODO: Check for incompatible types e.g. "txt" - 3
      */
   }
   return pSelf;
}

HB_EXPR_PTR hb_compExprReducePlus( HB_EXPR_PTR pSelf, HB_COMP_DECL )
{
   HB_EXPR_PTR pLeft, pRight;

   pLeft  = pSelf->value.asOperator.pLeft;
   pRight = pSelf->value.asOperator.pRight;

   if( pLeft->ExprType == HB_ET_NUMERIC )
   {
      if( pRight->ExprType == HB_ET_NUMERIC )
      {
         BYTE bType = ( pLeft->value.asNum.NumType & pRight->value.asNum.NumType );

         switch( bType )
         {
            case HB_ET_LONG:
            {
               HB_MAXDBL dVal = ( HB_MAXDBL ) pLeft->value.asNum.val.l + ( HB_MAXDBL ) pRight->value.asNum.val.l;

               if ( HB_DBL_LIM_LONG( dVal ) )
               {
                  pSelf->value.asNum.val.l = ( HB_LONG ) dVal;
                  pSelf->value.asNum.bDec = 0;
                  pSelf->value.asNum.NumType = HB_ET_LONG;
               }
               else
               {
                  pSelf->value.asNum.val.d = ( double ) dVal;
                  pSelf->value.asNum.bWidth = HB_DEFAULT_WIDTH;
                  pSelf->value.asNum.bDec = 0;
                  pSelf->value.asNum.NumType = HB_ET_DOUBLE;
               }
               break;
            }

            case HB_ET_DOUBLE:
               pSelf->value.asNum.val.d = pLeft->value.asNum.val.d + pRight->value.asNum.val.d;
               pSelf->value.asNum.bWidth = HB_DEFAULT_WIDTH;
               if( pLeft->value.asNum.bDec < pRight->value.asNum.bDec )
                  pSelf->value.asNum.bDec = pRight->value.asNum.bDec;
               else
                  pSelf->value.asNum.bDec = pLeft->value.asNum.bDec;
               pSelf->value.asNum.NumType = HB_ET_DOUBLE;
               break;

            default:
               if( pLeft->value.asNum.NumType == HB_ET_DOUBLE )
               {
                  pSelf->value.asNum.val.d = pLeft->value.asNum.val.d + ( double ) pRight->value.asNum.val.l;
                  pSelf->value.asNum.bWidth = HB_DEFAULT_WIDTH;
                  pSelf->value.asNum.bDec = pLeft->value.asNum.bDec;
               }
               else
               {
                  pSelf->value.asNum.val.d = ( double ) pLeft->value.asNum.val.l + pRight->value.asNum.val.d;
                  pSelf->value.asNum.bWidth = HB_DEFAULT_WIDTH;
                  pSelf->value.asNum.bDec = pRight->value.asNum.bDec;
               }
               pSelf->value.asNum.NumType = HB_ET_DOUBLE;
         }
         pSelf->ExprType = HB_ET_NUMERIC;
         pSelf->ValType  = HB_EV_NUMERIC;
         hb_compExprFree( pLeft, HB_COMP_PARAM );
         hb_compExprFree( pRight, HB_COMP_PARAM );
      }
      else if( pRight->ExprType == HB_ET_DATE )
      {
         if( pLeft->value.asNum.NumType == HB_ET_LONG )
            pSelf->value.asNum.val.l = pRight->value.asNum.val.l + pLeft->value.asNum.val.l;
         else
            pSelf->value.asNum.val.l = pRight->value.asNum.val.l + ( HB_LONG ) pLeft->value.asNum.val.d;
         pSelf->ExprType = HB_ET_DATE;
         pSelf->ValType  = HB_EV_DATE;
         hb_compExprFree( pLeft, HB_COMP_PARAM );
         hb_compExprFree( pRight, HB_COMP_PARAM );
      }
      else if( HB_SUPPORT_HARBOUR &&
               ( pLeft->value.asNum.NumType == HB_ET_LONG ?
                 pLeft->value.asNum.val.l == 0 :
                 pLeft->value.asNum.val.d == 0 ) )
      {
         /* NOTE: This will not generate a runtime error if incompatible
          * data type is used
          */
         pSelf->ExprType = HB_ET_NONE; /* suppress deletion of operator components */
         hb_compExprFree( pSelf, HB_COMP_PARAM );
         pSelf = pRight;
         hb_compExprFree( pLeft, HB_COMP_PARAM );
      }
      else
      {
         /* TODO: Check for incompatible types e.g. "txt" + 3
         */
      }
   }
   else if( pRight->ExprType == HB_ET_NUMERIC )
   {
      if( pLeft->ExprType == HB_ET_DATE )
      {
         if( pRight->value.asNum.NumType == HB_ET_LONG )
            pSelf->value.asNum.val.l = pLeft->value.asNum.val.l + pRight->value.asNum.val.l;
         else
            pSelf->value.asNum.val.l = pLeft->value.asNum.val.l + ( HB_LONG ) pRight->value.asNum.val.d;
         pSelf->ExprType = HB_ET_DATE;
         pSelf->ValType  = HB_EV_DATE;
         hb_compExprFree( pLeft, HB_COMP_PARAM );
         hb_compExprFree( pRight, HB_COMP_PARAM );
      }
      else if( HB_SUPPORT_HARBOUR &&
               ( pRight->value.asNum.NumType == HB_ET_LONG ?
                 pRight->value.asNum.val.l == 0 :
                 pRight->value.asNum.val.d == 0 ) )
      {
         /* NOTE: This will not generate a runtime error if incompatible
          * data type is used
          */
         pSelf->ExprType = HB_ET_NONE; /* suppress deletion of operator components */
         hb_compExprFree( pSelf, HB_COMP_PARAM );
         pSelf = pLeft;
         hb_compExprFree( pRight, HB_COMP_PARAM );
      }
      else
      {
         /* TODO: Check for incompatible types e.g. "txt" + 3
         */
      }
   }
   else if( pLeft->ExprType == HB_ET_STRING && pRight->ExprType == HB_ET_STRING )
   {
      if( pRight->ulLength == 0 )
      {
         pSelf->ExprType = HB_ET_NONE; /* suppress deletion of operator components */
         hb_compExprFree( pSelf, HB_COMP_PARAM );
         pSelf = pLeft;
         hb_compExprFree( pRight, HB_COMP_PARAM );
      }
      else if( pLeft->ulLength == 0 )
      {
         pSelf->ExprType = HB_ET_NONE; /* suppress deletion of operator components */
         hb_compExprFree( pSelf, HB_COMP_PARAM );
         pSelf = pRight;
         hb_compExprFree( pLeft, HB_COMP_PARAM );
      }
      else
      {
         /* Do not reduce strings with the macro operator '&'
         */
         char * szText = pLeft->value.asString.string;
         ULONG ulLen = pLeft->ulLength;
         BOOL fReduce = TRUE;

         while( ulLen-- )
         {
            if( *szText++ == '&' )
            {
               char ch = ulLen ? *szText : *pRight->value.asString.string;
               if( ( ch >= 'A' && ch <= 'Z' ) ||
                   ( ch >= 'a' && ch <= 'z' ) || ch == '_' ||
                   ! HB_SUPPORT_HARBOUR )
                  fReduce = FALSE;
            }
         }

         if( fReduce )
         {
            pSelf->ExprType = HB_ET_NONE; /* suppress deletion of operator components */
            hb_compExprFree( pSelf, HB_COMP_PARAM );
         	pSelf = hb_compExprReducePlusStrings( pLeft, pRight, HB_COMP_PARAM );
         }
      }
   }
   else
   {
      /* TODO: Check for incompatible types e.g. "txt" + 3
      */
   }
   return pSelf;
}


HB_EXPR_PTR hb_compExprReduceIN( HB_EXPR_PTR pSelf, HB_COMP_DECL )
{
   if( ( pSelf->value.asOperator.pLeft->ExprType == pSelf->value.asOperator.pRight->ExprType ) && pSelf->value.asOperator.pLeft->ExprType == HB_ET_STRING )
   {
      /* Both arguments are literal strings
       */
      BOOL bResult;

      /* NOTE: CA-Cl*pper has a bug where the $ operator returns .T.
               when an empty string is searched [vszakats] */

      if( pSelf->value.asOperator.pLeft->ulLength == 0 )
         bResult = TRUE;
      else
         bResult = ( hb_strAt( pSelf->value.asOperator.pLeft->value.asString.string, pSelf->value.asOperator.pLeft->ulLength,
                     pSelf->value.asOperator.pRight->value.asString.string, pSelf->value.asOperator.pRight->ulLength ) != 0 );

      /* NOTE:
       * "" $ "XXX" = .T.
       * "" $ "" = .T.
       */
      hb_compExprFree( pSelf->value.asOperator.pLeft, HB_COMP_PARAM );
      hb_compExprFree( pSelf->value.asOperator.pRight, HB_COMP_PARAM );
      pSelf->ExprType = HB_ET_LOGICAL;
      pSelf->ValType  = HB_EV_LOGICAL;
      pSelf->value.asLogical = bResult;
   }
   /* TODO: add checking for incompatible types
    */
   return pSelf;
}

HB_EXPR_PTR hb_compExprReduceNE( HB_EXPR_PTR pSelf, HB_COMP_DECL )
{
   HB_EXPR_PTR pLeft, pRight;

   pLeft  = pSelf->value.asOperator.pLeft;
   pRight = pSelf->value.asOperator.pRight;

   if( pLeft->ExprType == pRight->ExprType )
   {
      switch( pLeft->ExprType )
      {
         case HB_ET_LOGICAL:
            {
               /* .F. != .T.  = .T.
               * .T. != .T.  = .F.
               * .F. != .F.  = .F.
               * .T. != .F.  = .T.
               */
               BOOL bResult = ( pLeft->value.asLogical != pRight->value.asLogical );
               hb_compExprFree( pLeft, HB_COMP_PARAM );
               hb_compExprFree( pRight, HB_COMP_PARAM );
               pSelf->ExprType = HB_ET_LOGICAL;
               pSelf->ValType  = HB_EV_LOGICAL;
               pSelf->value.asLogical = bResult;
            }
            break;

         case HB_ET_STRING:
            /* NOTE: the result depends on SET EXACT setting then it
            * cannot be optimized except the case when NULL string are
            * compared - "" != "" is always FALSE regardless of EXACT
            * setting
            */
            if( ( pLeft->ulLength | pRight->ulLength ) == 0 )
            {
               hb_compExprFree( pLeft, HB_COMP_PARAM );
               hb_compExprFree( pRight, HB_COMP_PARAM );
               pSelf->ExprType = HB_ET_LOGICAL;
               pSelf->ValType  = HB_EV_LOGICAL;
               pSelf->value.asLogical = FALSE;

               /* NOTE: COMPATIBILITY: Clipper doesn't optimize this */
            }
            break;

         case HB_ET_NUMERIC:
            {
               BOOL bResult;

               switch( pLeft->value.asNum.NumType & pRight->value.asNum.NumType )
               {
                  case HB_ET_LONG:
                     bResult = ( pLeft->value.asNum.val.l != pRight->value.asNum.val.l );
                     break;
                  case HB_ET_DOUBLE:
                     bResult = ( pLeft->value.asNum.val.d != pRight->value.asNum.val.d );
                     break;
                  default:
                     {
                        if( pLeft->value.asNum.NumType == HB_ET_LONG )
                           bResult = ( pLeft->value.asNum.val.l != pRight->value.asNum.val.d );
                        else
                           bResult = ( pLeft->value.asNum.val.d != pRight->value.asNum.val.l );
                     }
                     break;
               }
               hb_compExprFree( pLeft, HB_COMP_PARAM );
               hb_compExprFree( pRight, HB_COMP_PARAM );
               pSelf->ExprType = HB_ET_LOGICAL;
               pSelf->ValType  = HB_EV_LOGICAL;
               pSelf->value.asLogical = bResult;
            }
            break;

         case HB_ET_NIL:
            hb_compExprFree( pLeft, HB_COMP_PARAM );
            hb_compExprFree( pRight, HB_COMP_PARAM );
            pSelf->ExprType = HB_ET_LOGICAL;
            pSelf->ValType  = HB_EV_LOGICAL;
            pSelf->value.asLogical = FALSE;
            break;
      }
   }
   /* TODO: add checking of incompatible types
   else
   {
   }
   */
   return pSelf;
}

HB_EXPR_PTR hb_compExprReduceGE( HB_EXPR_PTR pSelf, HB_COMP_DECL )
{
   HB_EXPR_PTR pLeft, pRight;

   pLeft  = pSelf->value.asOperator.pLeft;
   pRight = pSelf->value.asOperator.pRight;

   if( pLeft->ExprType == pRight->ExprType )
      switch( pLeft->ExprType )
      {
         case HB_ET_LOGICAL:
            {
               /* .T. >= .F.  = .T.
                * .T. >= .T.  = .T.
                * .F. >= .F.  = .T.
                * .F. >= .T.  = .f.
                */
               BOOL bResult = ! ( ! pLeft->value.asLogical && pRight->value.asLogical );
               hb_compExprFree( pLeft, HB_COMP_PARAM );
               hb_compExprFree( pRight, HB_COMP_PARAM );
               pSelf->ExprType = HB_ET_LOGICAL;
               pSelf->ValType  = HB_EV_LOGICAL;
               pSelf->value.asLogical = bResult;
            }
            break;

         case HB_ET_NUMERIC:
            {
               BOOL bResult;

               switch( pLeft->value.asNum.NumType & pRight->value.asNum.NumType )
               {
                  case HB_ET_LONG:
                     bResult = ( pLeft->value.asNum.val.l >= pRight->value.asNum.val.l );
                     break;
                  case HB_ET_DOUBLE:
                     bResult = ( pLeft->value.asNum.val.d >= pRight->value.asNum.val.d );
                     break;
                  default:
                     {
                        if( pLeft->value.asNum.NumType == HB_ET_LONG )
                           bResult = ( pLeft->value.asNum.val.l >= pRight->value.asNum.val.d );
                        else
                           bResult = ( pLeft->value.asNum.val.d >= pRight->value.asNum.val.l );
                     }
                     break;
               }
               hb_compExprFree( pLeft, HB_COMP_PARAM );
               hb_compExprFree( pRight, HB_COMP_PARAM );
               pSelf->ExprType = HB_ET_LOGICAL;
               pSelf->ValType  = HB_EV_LOGICAL;
               pSelf->value.asLogical = bResult;
            }
            break;

      }
   /* TODO: add checking of incompatible types
   else
   {
   }
   */
   return pSelf;
}

HB_EXPR_PTR hb_compExprReduceLE( HB_EXPR_PTR pSelf, HB_COMP_DECL )
{
   HB_EXPR_PTR pLeft, pRight;

   pLeft  = pSelf->value.asOperator.pLeft;
   pRight = pSelf->value.asOperator.pRight;

   if( pLeft->ExprType == pRight->ExprType )
      switch( pLeft->ExprType )
      {
         case HB_ET_LOGICAL:
            {
               /* .T. <= .F.  = .F.
                * .T. <= .T.  = .T.
                * .F. <= .F.  = .T.
                * .F. <= .T.  = .T.
                */
               BOOL bResult = ! ( pLeft->value.asLogical && ! pRight->value.asLogical );
               hb_compExprFree( pLeft, HB_COMP_PARAM );
               hb_compExprFree( pRight, HB_COMP_PARAM );
               pSelf->ExprType = HB_ET_LOGICAL;
               pSelf->ValType  = HB_EV_LOGICAL;
               pSelf->value.asLogical = bResult;
            }
            break;

         case HB_ET_NUMERIC:
            {
               BOOL bResult;

               switch( pLeft->value.asNum.NumType & pRight->value.asNum.NumType )
               {
                  case HB_ET_LONG:
                     bResult = ( pLeft->value.asNum.val.l <= pRight->value.asNum.val.l );
                     break;
                  case HB_ET_DOUBLE:
                     bResult = ( pLeft->value.asNum.val.d <= pRight->value.asNum.val.d );
                     break;
                  default:
                     {
                        if( pLeft->value.asNum.NumType == HB_ET_LONG )
                           bResult = ( pLeft->value.asNum.val.l <= pRight->value.asNum.val.d );
                        else
                           bResult = ( pLeft->value.asNum.val.d <= pRight->value.asNum.val.l );
                     }
                     break;
               }
               hb_compExprFree( pLeft, HB_COMP_PARAM );
               hb_compExprFree( pRight, HB_COMP_PARAM );
               pSelf->ExprType = HB_ET_LOGICAL;
               pSelf->ValType  = HB_EV_LOGICAL;
               pSelf->value.asLogical = bResult;
            }
            break;

      }
   /* TODO: add checking of incompatible types
   else
   {
   }
   */
   return pSelf;
}

HB_EXPR_PTR hb_compExprReduceGT( HB_EXPR_PTR pSelf, HB_COMP_DECL )
{
   HB_EXPR_PTR pLeft, pRight;

   pLeft  = pSelf->value.asOperator.pLeft;
   pRight = pSelf->value.asOperator.pRight;

   if( pLeft->ExprType == pRight->ExprType )
      switch( pLeft->ExprType )
      {
         case HB_ET_LOGICAL:
            {
               /* .T. > .F.  = .T.
                * .T. > .T.  = .F.
                * .F. > .F.  = .F.
                * .F. > .T.  = .F.
                */
               BOOL bResult = ( pLeft->value.asLogical && ! pRight->value.asLogical );
               hb_compExprFree( pLeft, HB_COMP_PARAM );
               hb_compExprFree( pRight, HB_COMP_PARAM );
               pSelf->ExprType = HB_ET_LOGICAL;
               pSelf->ValType  = HB_EV_LOGICAL;
               pSelf->value.asLogical = bResult;
            }
            break;

         case HB_ET_NUMERIC:
            {
               BOOL bResult;

               switch( pLeft->value.asNum.NumType & pRight->value.asNum.NumType )
               {
                  case HB_ET_LONG:
                     bResult = ( pLeft->value.asNum.val.l > pRight->value.asNum.val.l );
                     break;
                  case HB_ET_DOUBLE:
                     bResult = ( pLeft->value.asNum.val.d > pRight->value.asNum.val.d );
                     break;
                  default:
                     {
                        if( pLeft->value.asNum.NumType == HB_ET_LONG )
                           bResult = ( pLeft->value.asNum.val.l > pRight->value.asNum.val.d );
                        else
                           bResult = ( pLeft->value.asNum.val.d > pRight->value.asNum.val.l );
                     }
                     break;
               }
               hb_compExprFree( pLeft, HB_COMP_PARAM );
               hb_compExprFree( pRight, HB_COMP_PARAM );
               pSelf->ExprType = HB_ET_LOGICAL;
               pSelf->ValType  = HB_EV_LOGICAL;
               pSelf->value.asLogical = bResult;
            }
            break;

      }
   /* TODO: add checking of incompatible types
   else
   {
   }
   */
   return pSelf;
}

HB_EXPR_PTR hb_compExprReduceLT( HB_EXPR_PTR pSelf, HB_COMP_DECL )
{
   HB_EXPR_PTR pLeft, pRight;

   pLeft  = pSelf->value.asOperator.pLeft;
   pRight = pSelf->value.asOperator.pRight;

   if( pLeft->ExprType == pRight->ExprType )
      switch( pLeft->ExprType )
      {
         case HB_ET_LOGICAL:
            {
               /* .F. < .T.  = .T.
                * .T. < .T.  = .F.
                * .F. < .F.  = .F.
                * .T. < .F.  = .F.
                */
               BOOL bResult = ( ! pLeft->value.asLogical && pRight->value.asLogical );
               hb_compExprFree( pLeft, HB_COMP_PARAM );
               hb_compExprFree( pRight, HB_COMP_PARAM );
               pSelf->ExprType = HB_ET_LOGICAL;
               pSelf->ValType  = HB_EV_LOGICAL;
               pSelf->value.asLogical = bResult;
            }
            break;

         case HB_ET_NUMERIC:
            {
               BOOL bResult;

               switch( pLeft->value.asNum.NumType & pRight->value.asNum.NumType )
               {
                  case HB_ET_LONG:
                     bResult = ( pLeft->value.asNum.val.l < pRight->value.asNum.val.l );
                     break;
                  case HB_ET_DOUBLE:
                     bResult = ( pLeft->value.asNum.val.d < pRight->value.asNum.val.d );
                     break;
                  default:
                     {
                        if( pLeft->value.asNum.NumType == HB_ET_LONG )
                           bResult = ( pLeft->value.asNum.val.l < pRight->value.asNum.val.d );
                        else
                           bResult = ( pLeft->value.asNum.val.d < pRight->value.asNum.val.l );
                     }
                     break;
               }
               hb_compExprFree( pLeft, HB_COMP_PARAM );
               hb_compExprFree( pRight, HB_COMP_PARAM );
               pSelf->ExprType = HB_ET_LOGICAL;
               pSelf->ValType  = HB_EV_LOGICAL;
               pSelf->value.asLogical = bResult;
            }
            break;

         default:
            break;
      }
   /* TODO: add checking of incompatible types
   else
   {
   }
   */
   return pSelf;
}

HB_EXPR_PTR hb_compExprReduceEQ( HB_EXPR_PTR pSelf, HB_COMP_DECL )
{
   HB_EXPR_PTR pLeft, pRight;

   pLeft  = pSelf->value.asOperator.pLeft;
   pRight = pSelf->value.asOperator.pRight;

   if( pLeft->ExprType == pRight->ExprType )
   {
      switch( pLeft->ExprType )
      {
         case HB_ET_LOGICAL:
            {
               BOOL bResult = ( pLeft->value.asLogical == pRight->value.asLogical );
               hb_compExprFree( pLeft, HB_COMP_PARAM );
               hb_compExprFree( pRight, HB_COMP_PARAM );
               pSelf->ExprType = HB_ET_LOGICAL;
               pSelf->ValType  = HB_EV_LOGICAL;
               pSelf->value.asLogical = bResult;
            }
            break;

         case HB_ET_STRING:
            /* NOTE: when not exact comparison (==) is used 
             * the result depends on SET EXACT setting then it
             * cannot be optimized except the case when NULL string are
             * compared - "" = "" is always FALSE regardless of EXACT
             * setting
             */
            if( pSelf->ExprType == HB_EO_EQ ||
                ( pLeft->ulLength | pRight->ulLength ) == 0 )
            {
               BOOL bResult = pLeft->ulLength == pRight->ulLength &&
                              memcmp( pLeft->value.asString.string,
                                      pRight->value.asString.string,
                                      pLeft->ulLength ) == 0;
               hb_compExprFree( pLeft, HB_COMP_PARAM );
               hb_compExprFree( pRight, HB_COMP_PARAM );
               pSelf->ExprType = HB_ET_LOGICAL;
               pSelf->ValType  = HB_EV_LOGICAL;
               pSelf->value.asLogical = bResult;
            }
            break;

         case HB_ET_NUMERIC:
            {
               BOOL bResult;

               switch( pLeft->value.asNum.NumType & pRight->value.asNum.NumType )
               {
                  case HB_ET_LONG:
                     bResult = ( pLeft->value.asNum.val.l == pRight->value.asNum.val.l );
                     break;
                  case HB_ET_DOUBLE:
                     bResult = ( pLeft->value.asNum.val.d == pRight->value.asNum.val.d );
                     break;
                  default:
                     if( pLeft->value.asNum.NumType == HB_ET_LONG )
                        bResult = ( pLeft->value.asNum.val.l == pRight->value.asNum.val.d );
                     else
                        bResult = ( pLeft->value.asNum.val.d == pRight->value.asNum.val.l );
                     break;
               }
               hb_compExprFree( pLeft, HB_COMP_PARAM );
               hb_compExprFree( pRight, HB_COMP_PARAM );
               pSelf->ExprType = HB_ET_LOGICAL;
               pSelf->ValType  = HB_EV_LOGICAL;
               pSelf->value.asLogical = bResult;
            }
            break;

         case HB_ET_NIL:
            hb_compExprFree( pLeft, HB_COMP_PARAM );
            hb_compExprFree( pRight, HB_COMP_PARAM );
            pSelf->ExprType = HB_ET_LOGICAL;
            pSelf->ValType  = HB_EV_LOGICAL;
            pSelf->value.asLogical = TRUE;
            break;
      }
   }
   /* TODO: add checking of incompatible types
   else
   {
   }
   */
   return pSelf;
}

HB_EXPR_PTR hb_compExprReduceAnd( HB_EXPR_PTR pSelf, HB_COMP_DECL )
{
   HB_EXPR_PTR pLeft, pRight;

   pLeft  = pSelf->value.asOperator.pLeft;
   pRight = pSelf->value.asOperator.pRight;

   if( pLeft->ExprType == HB_ET_LOGICAL && pRight->ExprType == HB_ET_LOGICAL )
   {
      BOOL bResult;

      bResult = pLeft->value.asLogical && pRight->value.asLogical;
      hb_compExprFree( pLeft, HB_COMP_PARAM );
      hb_compExprFree( pRight, HB_COMP_PARAM );
      pSelf->ExprType = HB_ET_LOGICAL;
      pSelf->ValType  = HB_EV_LOGICAL;
      pSelf->value.asLogical = bResult;
   }
   else if( pLeft->ExprType == HB_ET_LOGICAL &&
            HB_COMP_ISSUPPORTED( HB_COMPFLAG_SHORTCUTS ) )
   {
      if( pLeft->value.asLogical )
      {
         /* .T. .AND. expr => expr
          */
         hb_compExprFree( pLeft, HB_COMP_PARAM);
         pSelf->ExprType = HB_ET_NONE;    /* don't delete expression components */
         hb_compExprFree( pSelf, HB_COMP_PARAM );
         pSelf = pRight;
      }
      else
      {
         /* .F. .AND. expr => .F.
          */
         hb_compExprFree( pLeft, HB_COMP_PARAM );
         hb_compExprFree( pRight, HB_COMP_PARAM );         /* discard expression */
         pSelf->ExprType = HB_ET_LOGICAL;
         pSelf->ValType  = HB_EV_LOGICAL;
         pSelf->value.asLogical = FALSE;
      }
   }
   else if( pRight->ExprType == HB_ET_LOGICAL &&
            HB_COMP_ISSUPPORTED( HB_COMPFLAG_SHORTCUTS ) )
   {
      if( pRight->value.asLogical )
      {
         /* expr .AND. .T. => expr
          */
         hb_compExprFree( pRight, HB_COMP_PARAM );
         pSelf->ExprType = HB_ET_NONE;    /* don't delete expression components */
         hb_compExprFree( pSelf, HB_COMP_PARAM );
         pSelf = pLeft;
      }
      else
      {
         /* expr .AND. .F. => .F.
          */
         hb_compExprFree( pLeft, HB_COMP_PARAM );         /* discard expression */
         hb_compExprFree( pRight, HB_COMP_PARAM );
         pSelf->ExprType = HB_ET_LOGICAL;
         pSelf->ValType  = HB_EV_LOGICAL;
         pSelf->value.asLogical = FALSE;
      }
   }
   return pSelf;
}

HB_EXPR_PTR hb_compExprReduceOr( HB_EXPR_PTR pSelf, HB_COMP_DECL )
{
   HB_EXPR_PTR pLeft, pRight;

   pLeft  = pSelf->value.asOperator.pLeft;
   pRight = pSelf->value.asOperator.pRight;

   if( pLeft->ExprType == HB_ET_LOGICAL && pRight->ExprType == HB_ET_LOGICAL )
   {
      BOOL bResult;

      bResult = pLeft->value.asLogical || pRight->value.asLogical;
      hb_compExprFree( pLeft, HB_COMP_PARAM );
      hb_compExprFree( pRight, HB_COMP_PARAM );
      pSelf->ExprType = HB_ET_LOGICAL;
      pSelf->ValType  = HB_EV_LOGICAL;
      pSelf->value.asLogical = bResult;
   }
   else if( pLeft->ExprType == HB_ET_LOGICAL &&
            HB_COMP_ISSUPPORTED( HB_COMPFLAG_SHORTCUTS ) )
   {
      if( pLeft->value.asLogical )
      {
         /* .T. .OR. expr => .T.
          */
         hb_compExprFree( pLeft, HB_COMP_PARAM );
         hb_compExprFree( pRight, HB_COMP_PARAM );         /* discard expression */
         pSelf->ExprType = HB_ET_LOGICAL;
         pSelf->ValType  = HB_EV_LOGICAL;
         pSelf->value.asLogical = TRUE;
      }
      else
      {
         /* .F. .OR. expr => expr
          */
         hb_compExprFree( pLeft, HB_COMP_PARAM );
         pSelf->ExprType = HB_ET_NONE;    /* don't delete expression components */
         hb_compExprFree( pSelf, HB_COMP_PARAM );
         pSelf = pRight;
      }
   }
   else if( pRight->ExprType == HB_ET_LOGICAL &&
            HB_COMP_ISSUPPORTED( HB_COMPFLAG_SHORTCUTS ) )
   {
      if( pRight->value.asLogical )
      {
         /* expr .OR. .T. => .T.
          */
         hb_compExprFree( pLeft, HB_COMP_PARAM );         /* discard expression */
         hb_compExprFree( pRight, HB_COMP_PARAM );
         pSelf->ExprType = HB_ET_LOGICAL;
         pSelf->ValType  = HB_EV_LOGICAL;
         pSelf->value.asLogical = TRUE;
      }
      else
      {
         /* expr .OR. .F. => expr
          */
         hb_compExprFree( pRight, HB_COMP_PARAM );
         pSelf->ExprType = HB_ET_NONE;    /* don't delete expression components */
         hb_compExprFree( pSelf, HB_COMP_PARAM );
         pSelf = pLeft;
      }
   }
   return pSelf;
}

HB_EXPR_PTR hb_compExprReduceIIF( HB_EXPR_PTR pSelf, HB_COMP_DECL )
{
   HB_EXPR_PTR pExpr;

   pExpr = pSelf->value.asList.pExprList;  /* get conditional expression */
   if( pExpr->ExprType == HB_ET_LOGICAL )
   {
      /* the condition was reduced to a logical value: .T. or .F.
      */
      if( pExpr->value.asLogical )
      {
         /* .T. was specified
         */
         pExpr = pExpr->pNext;   /* skip to TRUE expression */
         /* delete condition  - it is no longer needed
            */
         hb_compExprFree( pSelf->value.asList.pExprList, HB_COMP_PARAM );
         /* assign NULL to a start of expressions list to suppress
          * deletion of expression's components - we are deleting them
          * here
          */
         pSelf->value.asList.pExprList = NULL;
         hb_compExprFree( pSelf, HB_COMP_PARAM );
         /* store the TRUE expression as a result of reduction
          */
         pSelf = pExpr;
         pExpr = pExpr->pNext;     /* skip to FALSE expression */
         hb_compExprFree( pExpr, HB_COMP_PARAM );   /* delete FALSE expr */
         pSelf->pNext = NULL;
      }
      else
      {
         /* .F. was specified
         */
         pExpr = pExpr->pNext;   /* skip to TRUE expression */
         /* delete condition  - it is no longer needed
          */
         hb_compExprFree( pSelf->value.asList.pExprList, HB_COMP_PARAM );
         /* assign NULL to a start of expressions list to suppress
          * deletion of expression's components - we are deleting them
          * here
          */
         pSelf->value.asList.pExprList = NULL;
         hb_compExprFree( pSelf, HB_COMP_PARAM );
         /* store the FALSE expression as a result of reduction
            */
         pSelf = pExpr->pNext;
         hb_compExprFree( pExpr, HB_COMP_PARAM );   /* delete TRUE expr */
         pSelf->pNext = NULL;
      }

      /* this will cause warning when IIF is used as statement */
      /*
      if( pSelf->ExprType == HB_ET_NONE )
      {
         pSelf->ExprType = HB_ET_NIL;
         pSelf->ValType = HB_EV_NIL;
      }
      */
   }
   /* check if valid expression is passed
   */
   else if( pExpr->ExprType == HB_ET_NIL ||
            pExpr->ExprType == HB_ET_NUMERIC ||
            pExpr->ExprType == HB_ET_DATE ||
            pExpr->ExprType == HB_ET_STRING ||
            pExpr->ExprType == HB_ET_CODEBLOCK ||
            pExpr->ExprType == HB_ET_ARRAY ||
            pExpr->ExprType == HB_ET_VARREF ||
            pExpr->ExprType == HB_ET_REFERENCE ||
            pExpr->ExprType == HB_ET_FUNREF )
   {
      hb_compExprErrorType( pExpr, HB_COMP_PARAM );
   }
   return pSelf;
}

/* replace the list containing a single expression with a simple expression
 * - strips parenthesis
 *  ( EXPR ) -> EXPR
 */
HB_EXPR_PTR hb_compExprListStrip( HB_EXPR_PTR pSelf, HB_COMP_DECL )
{
   if( pSelf->ExprType == HB_ET_LIST )
   {
      ULONG ulCount = hb_compExprListLen( pSelf );

      if( ulCount == 1 && pSelf->value.asList.pExprList->ExprType <= HB_ET_VARIABLE )
      {
         /* replace the list with a simple expression
          *  ( EXPR ) -> EXPR
          */
         HB_EXPR_PTR pExpr = pSelf;

         pSelf = pSelf->value.asList.pExprList;
         pExpr->value.asList.pExprList = NULL;
         hb_compExprFree( pExpr, HB_COMP_PARAM );
      }
   }
   return pSelf;
}

BOOL hb_compExprReduceAT( HB_EXPR_PTR pSelf, HB_COMP_DECL )
{
   HB_EXPR_PTR pParms = pSelf->value.asFunCall.pParms;
   HB_EXPR_PTR pSub  = pParms->value.asList.pExprList;
   HB_EXPR_PTR pText = pSub->pNext;
   HB_EXPR_PTR pReduced;

   if( pSub->ExprType == HB_ET_STRING && pText->ExprType == HB_ET_STRING )
   {
      /* This is CA-Clipper optimizer behavior */
      if( pSub->ulLength == 0 )
      {
         pReduced = hb_compExprNewLong( 1, HB_COMP_PARAM );
      }
      else
      {
         pReduced = hb_compExprNewLong( hb_strAt( pSub->value.asString.string,
                               pSub->ulLength, pText->value.asString.string,
                               pText->ulLength ), HB_COMP_PARAM );
      }

      hb_compExprFree( pSelf->value.asFunCall.pFunName, HB_COMP_PARAM );
      hb_compExprFree( pSelf->value.asFunCall.pParms, HB_COMP_PARAM );

      memcpy( pSelf, pReduced, sizeof( HB_EXPR ) );
      hb_compExprClear( pReduced, HB_COMP_PARAM );
      return TRUE;
   }
   else
      return FALSE;
}

BOOL hb_compExprReduceCHR( HB_EXPR_PTR pSelf, HB_COMP_DECL )
{
   HB_EXPR_PTR pParms = pSelf->value.asFunCall.pParms;
   HB_EXPR_PTR pArg = pParms->value.asList.pExprList;

   /* try to change it into a string */
   if( pArg->ExprType == HB_ET_NUMERIC )
   {
      /* NOTE: CA-Cl*pper's compiler optimizer will be wrong for those
               CHR() cases where the passed parameter is a constant which
               can be divided by 256 but it's not zero, in this case it
               will return an empty string instead of a Chr(0). [vszakats] */

      HB_EXPR_PTR pExpr = hb_compExprNew( HB_ET_STRING, HB_COMP_PARAM );

      pExpr->ValType = HB_EV_STRING;
      if( pArg->value.asNum.NumType == HB_ET_LONG )
      {
         if( ( pArg->value.asNum.val.l & 0xff ) == 0 &&
               pArg->value.asNum.val.l != 0 )
         {
            pExpr->value.asString.string = "";
            pExpr->value.asString.dealloc = FALSE;
            pExpr->ulLength = 0;
         }
         else
         {
            pExpr->value.asString.string = ( char * ) hb_xgrab( 2 );
            pExpr->value.asString.string[ 0 ] = ( char ) pArg->value.asNum.val.l;
            pExpr->value.asString.string[ 1 ] = '\0';
            pExpr->value.asString.dealloc = TRUE;
            pExpr->ulLength = 1;
         }
      }
      else
      {
         pExpr->value.asString.string = ( char * ) hb_xgrab( 2 );
         pExpr->value.asString.string[ 0 ] = ( char ) ( ( unsigned int ) pArg->value.asNum.val.d & 0xff );
         pExpr->value.asString.string[ 1 ] = '\0';
         pExpr->value.asString.dealloc = TRUE;
         pExpr->ulLength = 1;
      }
      
      hb_compExprFree( pParms, HB_COMP_PARAM );
      hb_compExprFree( pSelf->value.asFunCall.pFunName, HB_COMP_PARAM );
      memcpy( pSelf, pExpr, sizeof( HB_EXPR ) );
      hb_compExprClear( pExpr, HB_COMP_PARAM );
      return TRUE;
   }
   
   return FALSE;
}

BOOL hb_compExprReduceLEN( HB_EXPR_PTR pSelf, HB_COMP_DECL )
{
   HB_EXPR_PTR pParms = pSelf->value.asFunCall.pParms;
   HB_EXPR_PTR pArg = pParms->value.asList.pExprList;

   if( pArg->ExprType == HB_ET_STRING || pArg->ExprType == HB_ET_ARRAY )
   {
      HB_EXPR_PTR pExpr = hb_compExprNewLong( pArg->ulLength, HB_COMP_PARAM );

      hb_compExprFree( pParms, HB_COMP_PARAM );
      hb_compExprFree( pSelf->value.asFunCall.pFunName, HB_COMP_PARAM );
      memcpy( pSelf, pExpr, sizeof( HB_EXPR ) );
      hb_compExprClear( pExpr, HB_COMP_PARAM );
      return TRUE;
   }
   return FALSE;
}

BOOL hb_compExprReduceASC( HB_EXPR_PTR pSelf, HB_COMP_DECL )
{
   HB_EXPR_PTR pParms = pSelf->value.asFunCall.pParms;
   HB_EXPR_PTR pArg = pParms->value.asList.pExprList;

   if( pArg->ExprType == HB_ET_STRING )
   {
      HB_EXPR_PTR pExpr = hb_compExprNewLong(
                ( UCHAR ) pArg->value.asString.string[0], HB_COMP_PARAM );

      hb_compExprFree( pParms, HB_COMP_PARAM );
      hb_compExprFree( pSelf->value.asFunCall.pFunName, HB_COMP_PARAM );
      memcpy( pSelf, pExpr, sizeof( HB_EXPR ) );
      hb_compExprClear( pExpr, HB_COMP_PARAM );
      return TRUE;
   }
   return FALSE;
}

BOOL hb_compExprReduceSTOD( HB_EXPR_PTR pSelf, USHORT usCount, HB_COMP_DECL )
{
   if( usCount == 1 )
   {
      HB_EXPR_PTR pParms = pSelf->value.asFunCall.pParms;
      HB_EXPR_PTR pArg = pParms->value.asList.pExprList;
      
      if( pArg->ExprType == HB_ET_STRING && ( pArg->ulLength == 8 || pArg->ulLength == 0 ) )
      {
         HB_EXPR_PTR pExpr = hb_compExprNewDate( pArg->ulLength == 0 ? 0 :
                                  hb_dateEncStr( pArg->value.asString.string ),
                                  HB_COMP_PARAM );

         hb_compExprFree( pParms, HB_COMP_PARAM );
         hb_compExprFree( pSelf->value.asFunCall.pFunName, HB_COMP_PARAM );
         memcpy( pSelf, pExpr, sizeof( HB_EXPR ) );
         hb_compExprClear( pExpr, HB_COMP_PARAM );
         return TRUE;
      }
   }
   else
   {
      HB_EXPR_PTR pExpr = hb_compExprNewDate( 0, HB_COMP_PARAM );
      
      hb_compExprFree( pSelf->value.asFunCall.pParms, HB_COMP_PARAM );
      hb_compExprFree( pSelf->value.asFunCall.pFunName, HB_COMP_PARAM );
      memcpy( pSelf, pExpr, sizeof( HB_EXPR ) );
      hb_compExprClear( pExpr, HB_COMP_PARAM );
      return TRUE;
   }

   return FALSE;
}
