/*
 * $Id$
 */

/*
 * Harbour Project source code:
 * Compiler Expression Optimizer
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

#include "hbcomp.h"
#include "hbmacro.ch"

#if !defined( HB_HASH_USES_ARRAY_INDEXES )
#  define HB_HASH_USES_ARRAY_INDEXES
#endif

#define HB_USE_ARRAYAT_REF
/* Temporary disabled optimization with references to object variables
   until we will not have extended reference items in our HVM [druzus] */
/* #define HB_USE_OBJMSG_REF */


/* Forward declarations
 */

/* forward declaration of callback functions
 */
static HB_EXPR_FUNC( hb_compExprUseDummy );
static HB_EXPR_FUNC( hb_compExprUseNil );
static HB_EXPR_FUNC( hb_compExprUseNumeric );
static HB_EXPR_FUNC( hb_compExprUseDate );
static HB_EXPR_FUNC( hb_compExprUseTimeStamp );
static HB_EXPR_FUNC( hb_compExprUseString );
static HB_EXPR_FUNC( hb_compExprUseCodeblock );
static HB_EXPR_FUNC( hb_compExprUseLogical );
static HB_EXPR_FUNC( hb_compExprUseSelf );
static HB_EXPR_FUNC( hb_compExprUseArray );
static HB_EXPR_FUNC( hb_compExprUseHash );
static HB_EXPR_FUNC( hb_compExprUseVarRef );
static HB_EXPR_FUNC( hb_compExprUseRef );
static HB_EXPR_FUNC( hb_compExprUseFunRef );
static HB_EXPR_FUNC( hb_compExprUseIIF );
static HB_EXPR_FUNC( hb_compExprUseList );
static HB_EXPR_FUNC( hb_compExprUseArgList );
static HB_EXPR_FUNC( hb_compExprUseMacroArgList );
static HB_EXPR_FUNC( hb_compExprUseArrayAt );
static HB_EXPR_FUNC( hb_compExprUseMacro );
static HB_EXPR_FUNC( hb_compExprUseFunCall );
static HB_EXPR_FUNC( hb_compExprUseAliasVar );
static HB_EXPR_FUNC( hb_compExprUseAliasExpr );
static HB_EXPR_FUNC( hb_compExprUseSend );
static HB_EXPR_FUNC( hb_compExprUseFunName );
static HB_EXPR_FUNC( hb_compExprUseAlias );
static HB_EXPR_FUNC( hb_compExprUseRTVariable );
static HB_EXPR_FUNC( hb_compExprUseVariable );
static HB_EXPR_FUNC( hb_compExprUseAssign );
static HB_EXPR_FUNC( hb_compExprUseEqual );
static HB_EXPR_FUNC( hb_compExprUsePlus );
static HB_EXPR_FUNC( hb_compExprUseMinus );
static HB_EXPR_FUNC( hb_compExprUseMult );
static HB_EXPR_FUNC( hb_compExprUseDiv );
static HB_EXPR_FUNC( hb_compExprUseMod );
static HB_EXPR_FUNC( hb_compExprUsePower );
static HB_EXPR_FUNC( hb_compExprUsePostInc );
static HB_EXPR_FUNC( hb_compExprUsePostDec );
static HB_EXPR_FUNC( hb_compExprUsePreInc );
static HB_EXPR_FUNC( hb_compExprUsePreDec );
static HB_EXPR_FUNC( hb_compExprUsePlusEq );
static HB_EXPR_FUNC( hb_compExprUseMinusEq );
static HB_EXPR_FUNC( hb_compExprUseMultEq );
static HB_EXPR_FUNC( hb_compExprUseDivEq );
static HB_EXPR_FUNC( hb_compExprUseModEq );
static HB_EXPR_FUNC( hb_compExprUseExpEq );
static HB_EXPR_FUNC( hb_compExprUseAnd );
static HB_EXPR_FUNC( hb_compExprUseOr );
static HB_EXPR_FUNC( hb_compExprUseNot );
static HB_EXPR_FUNC( hb_compExprUseEQ );
static HB_EXPR_FUNC( hb_compExprUseLT );
static HB_EXPR_FUNC( hb_compExprUseGT );
static HB_EXPR_FUNC( hb_compExprUseLE );
static HB_EXPR_FUNC( hb_compExprUseGE );
static HB_EXPR_FUNC( hb_compExprUseNE );
static HB_EXPR_FUNC( hb_compExprUseIN );
static HB_EXPR_FUNC( hb_compExprUseNegate );

/* other helper functions
*/
#if defined( HB_MACRO_SUPPORT )
   static void hb_compExprCodeblockPush( HB_EXPR_PTR, HB_COMP_DECL );
#else
   static void hb_compExprCodeblockPush( HB_EXPR_PTR, BOOL, HB_COMP_DECL );
   static void hb_compExprCodeblockEarly( HB_EXPR_PTR, HB_COMP_DECL );
   static void hb_compExprCodeblockExtPush( HB_EXPR_PTR pSelf, HB_COMP_DECL );
#endif

static void hb_compExprPushSendPop( HB_EXPR_PTR pSelf, HB_COMP_DECL );
static void hb_compExprPushSendPush( HB_EXPR_PTR pSelf, HB_COMP_DECL );
static void hb_compExprPushOperEq( HB_EXPR_PTR pSelf, BYTE bOpEq, HB_COMP_DECL );
static void hb_compExprUseOperEq( HB_EXPR_PTR pSelf, BYTE bOpEq, HB_COMP_DECL );
static void hb_compExprPushPreOp( HB_EXPR_PTR pSelf, BYTE bOper, HB_COMP_DECL );
static void hb_compExprPushPostOp( HB_EXPR_PTR pSelf, BYTE bOper, HB_COMP_DECL );
static void hb_compExprUsePreOp( HB_EXPR_PTR pSelf, BYTE bOper, HB_COMP_DECL );
static void hb_compExprUseAliasMacro( HB_EXPR_PTR pAliasedVar, BYTE bAction, HB_COMP_DECL );
static HB_EXPR_PTR hb_compExprReduceList( HB_EXPR_PTR pExpr, HB_COMP_DECL );
static HB_EXPR_PTR hb_compExprReduceAliasString( HB_EXPR_PTR pExpr, HB_EXPR_PTR pAlias, HB_COMP_DECL );
static BOOL hb_compExprIsMemvarAlias( const char *szAlias );


const HB_EXPR_FUNC_PTR hb_comp_ExprTable[ HB_EXPR_COUNT ] = {
   hb_compExprUseDummy,
   hb_compExprUseNil,
   hb_compExprUseNumeric,
   hb_compExprUseDate,
   hb_compExprUseTimeStamp,
   hb_compExprUseString,
   hb_compExprUseCodeblock,
   hb_compExprUseLogical,
   hb_compExprUseSelf,
   hb_compExprUseArray,
   hb_compExprUseHash,
   hb_compExprUseFunRef,
   hb_compExprUseVarRef,
   hb_compExprUseRef,
   hb_compExprUseIIF,
   hb_compExprUseList,
   hb_compExprUseArgList,
   hb_compExprUseMacroArgList,
   hb_compExprUseArrayAt,
   hb_compExprUseMacro,
   hb_compExprUseFunCall,
   hb_compExprUseAliasVar,
   hb_compExprUseAliasExpr,
   hb_compExprUseSend,
   hb_compExprUseFunName,
   hb_compExprUseAlias,
   hb_compExprUseRTVariable,
   hb_compExprUseVariable,
   hb_compExprUsePostInc,      /* post-operators -> lowest precedence */
   hb_compExprUsePostDec,
   hb_compExprUseAssign,       /* assigments */
   hb_compExprUsePlusEq,
   hb_compExprUseMinusEq,
   hb_compExprUseMultEq,
   hb_compExprUseDivEq,
   hb_compExprUseModEq,
   hb_compExprUseExpEq,
   hb_compExprUseOr,           /* logical operators */
   hb_compExprUseAnd,
   hb_compExprUseNot,
   hb_compExprUseEqual,        /* relational operators */
   hb_compExprUseEQ,
   hb_compExprUseNE,
   hb_compExprUseIN,
   hb_compExprUseLT,
   hb_compExprUseGT,
   hb_compExprUseLE,
   hb_compExprUseGE,
   hb_compExprUsePlus,      /* addition */
   hb_compExprUseMinus,
   hb_compExprUseMult,      /* multiple */
   hb_compExprUseDiv,
   hb_compExprUseMod,
   hb_compExprUsePower,
   hb_compExprUseNegate,    /* sign operator */
   hb_compExprUsePreInc,
   hb_compExprUsePreDec     /* highest precedence */
};

/* ************************************************************************* */

static HB_EXPR_FUNC( hb_compExprUseDummy )
{
   switch( iMessage )
   {
      case HB_EA_REDUCE:
         break;
      case HB_EA_ARRAY_AT:
         HB_COMP_ERROR_TYPE( pSelf );
      case HB_EA_ARRAY_INDEX:
         break;
      case HB_EA_LVALUE:
         hb_compErrorLValue( HB_COMP_PARAM, pSelf );
         break;
      case HB_EA_PUSH_PCODE:
         HB_GEN_FUNC1( PCode1, HB_P_PUSHNIL );
         break;
      case HB_EA_POP_PCODE:
      case HB_EA_PUSH_POP:
      case HB_EA_STATEMENT:
      case HB_EA_DELETE:
         break;
   }
   return pSelf;
}

/* actions for HB_ET_NIL expression
 */
static HB_EXPR_FUNC( hb_compExprUseNil )
{
   switch( iMessage )
   {
      case HB_EA_REDUCE:
         break;
      case HB_EA_ARRAY_AT:
         HB_COMP_ERROR_TYPE( pSelf );
         break;
      case HB_EA_ARRAY_INDEX:
         hb_compErrorIndex( HB_COMP_PARAM, pSelf );     /* NIL cannot be used as index element */
         break;
      case HB_EA_LVALUE:
         hb_compErrorLValue( HB_COMP_PARAM, pSelf );
         break;
      case HB_EA_PUSH_PCODE:
         HB_GEN_FUNC1( PCode1, HB_P_PUSHNIL );
         break;
      case HB_EA_POP_PCODE:
         break;
      case HB_EA_PUSH_POP:
      case HB_EA_STATEMENT:
         hb_compWarnMeaningless( HB_COMP_PARAM, pSelf );
      case HB_EA_DELETE:
         break;
   }

   return pSelf;
}

/* actions for HB_ET_NUMERIC expression
 */
static HB_EXPR_FUNC( hb_compExprUseNumeric )
{
   switch( iMessage )
   {
      case HB_EA_REDUCE:
         break;
      case HB_EA_ARRAY_AT:
         HB_COMP_ERROR_TYPE( pSelf );
         break;
      case HB_EA_ARRAY_INDEX:
         break;
      case HB_EA_LVALUE:
         hb_compErrorLValue( HB_COMP_PARAM, pSelf );
         break;
      case HB_EA_PUSH_PCODE:
         if( pSelf->value.asNum.NumType == HB_ET_DOUBLE )
            HB_GEN_FUNC3( PushDouble, pSelf->value.asNum.val.d, pSelf->value.asNum.bWidth, pSelf->value.asNum.bDec );
         else
            HB_GEN_FUNC1( PushLong, pSelf->value.asNum.val.l );
         break;
      case HB_EA_POP_PCODE:
         break;
      case HB_EA_PUSH_POP:
      case HB_EA_STATEMENT:
         hb_compWarnMeaningless( HB_COMP_PARAM, pSelf );
      case HB_EA_DELETE:
         break;
   }
   return pSelf;
}

/* actions for HB_ET_DATE expression
 */
static HB_EXPR_FUNC( hb_compExprUseDate )
{
   switch( iMessage )
   {
      case HB_EA_REDUCE:
         break;
      case HB_EA_ARRAY_AT:
         HB_COMP_ERROR_TYPE( pSelf );
         break;
      case HB_EA_ARRAY_INDEX:
#ifdef HB_HASH_USES_ARRAY_INDEXES
         if( !HB_SUPPORT_HARBOUR )
#endif
            hb_compErrorIndex( HB_COMP_PARAM, pSelf );     /* Date cannot be used as index element */
         break;
      case HB_EA_LVALUE:
         hb_compErrorLValue( HB_COMP_PARAM, pSelf );
         break;
      case HB_EA_PUSH_PCODE:
         HB_GEN_FUNC1( PushDate, pSelf->value.asDate.lDate );
         break;
      case HB_EA_POP_PCODE:
         break;
      case HB_EA_PUSH_POP:
      case HB_EA_STATEMENT:
         hb_compWarnMeaningless( HB_COMP_PARAM, pSelf );
      case HB_EA_DELETE:
         break;
   }

   return pSelf;
}

/* actions for HB_ET_TIMESTAMP expression
 */
static HB_EXPR_FUNC( hb_compExprUseTimeStamp )
{
   switch( iMessage )
   {
      case HB_EA_REDUCE:
         break;
      case HB_EA_ARRAY_AT:
         HB_COMP_ERROR_TYPE( pSelf );
         break;
      case HB_EA_ARRAY_INDEX:
#ifdef HB_HASH_USES_ARRAY_INDEXES
         if( !HB_SUPPORT_HARBOUR )
#endif
            hb_compErrorIndex( HB_COMP_PARAM, pSelf );     /* timestamp cannot be used as index element */
         break;
      case HB_EA_LVALUE:
         hb_compErrorLValue( HB_COMP_PARAM, pSelf );
         break;
      case HB_EA_PUSH_PCODE:
         HB_GEN_FUNC2( PushTimeStamp, pSelf->value.asDate.lDate,
                                      pSelf->value.asDate.lTime );
         break;
      case HB_EA_POP_PCODE:
         break;
      case HB_EA_PUSH_POP:
      case HB_EA_STATEMENT:
         hb_compWarnMeaningless( HB_COMP_PARAM, pSelf );
      case HB_EA_DELETE:
         break;
   }

   return pSelf;
}

/* actions for HB_ET_STRING expression
 */
static HB_EXPR_FUNC( hb_compExprUseString )
{
   switch( iMessage )
   {
      case HB_EA_REDUCE:
         break;
      case HB_EA_ARRAY_AT:
         HB_COMP_ERROR_TYPE( pSelf );
         break;
      case HB_EA_ARRAY_INDEX:
#ifdef HB_HASH_USES_ARRAY_INDEXES
         if( !HB_SUPPORT_HARBOUR )
#endif
            hb_compErrorIndex( HB_COMP_PARAM, pSelf );     /* string cannot be used as index element */
         break;
      case HB_EA_LVALUE:
         hb_compErrorLValue( HB_COMP_PARAM, pSelf );
         break;
      case HB_EA_PUSH_PCODE:
      {
         HB_GEN_FUNC2( PushString, pSelf->value.asString.string,
                       pSelf->ulLength + 1 );
#if ! defined( HB_MACRO_SUPPORT )
         if( HB_SUPPORT_MACROTEXT &&
             hb_compIsValidMacroText( HB_COMP_PARAM,
                                      pSelf->value.asString.string,
                                      pSelf->ulLength ) )
#else
         if( hb_macroIsValidMacroText( pSelf->value.asString.string,
                                       pSelf->ulLength ) )
#endif
         {
            HB_GEN_FUNC1( PCode1, HB_P_MACROTEXT );
         }
         break;
      }
      case HB_EA_POP_PCODE:
         break;

      case HB_EA_PUSH_POP:
      case HB_EA_STATEMENT:
         hb_compWarnMeaningless( HB_COMP_PARAM, pSelf );
         break;

      case HB_EA_DELETE:
         if( pSelf->value.asString.dealloc )
            hb_xfree( pSelf->value.asString.string );
         break;
   }
   return pSelf;
}

/* actions for HB_ET_CODEBLOCK expression
 */
static HB_EXPR_FUNC( hb_compExprUseCodeblock )
{
   switch( iMessage )
   {
      case HB_EA_REDUCE:
         break;
      case HB_EA_ARRAY_AT:
         HB_COMP_ERROR_TYPE( pSelf );
         break;
      case HB_EA_ARRAY_INDEX:
         hb_compErrorIndex( HB_COMP_PARAM, pSelf );     /* codeblock cannot be used as index element */
         break;
      case HB_EA_LVALUE:
         hb_compErrorLValue( HB_COMP_PARAM, pSelf );
         break;
      case HB_EA_PUSH_PCODE:
      {
#if defined( HB_MACRO_SUPPORT )
         HB_EXPR_PCODE1( hb_compExprCodeblockPush, pSelf );
#else
         if( pSelf->value.asCodeblock.flags & HB_BLOCK_EXT )
            hb_compExprCodeblockExtPush( pSelf, HB_COMP_PARAM );
         else if( ( pSelf->value.asCodeblock.flags & HB_BLOCK_MACRO ) &&
                 !( pSelf->value.asCodeblock.flags &
                                 ( HB_BLOCK_LATEEVAL | HB_BLOCK_VPARAMS ) ) )
            /* early evaluation of a macro */
            hb_compExprCodeblockEarly( pSelf, HB_COMP_PARAM );
         else
            hb_compExprCodeblockPush( pSelf, TRUE, HB_COMP_PARAM );
#endif
         break;
      }
      case HB_EA_POP_PCODE:
         break;
      case HB_EA_PUSH_POP:
      case HB_EA_STATEMENT:
         hb_compWarnMeaningless( HB_COMP_PARAM, pSelf );
         break;

      case HB_EA_DELETE:
      {
         HB_EXPR_PTR pExp = pSelf->value.asCodeblock.pExprList;
         HB_EXPR_PTR pNext;

         hb_compExprCBVarDel( pSelf->value.asCodeblock.pLocals );

         if( pSelf->value.asCodeblock.string )
            hb_xfree( pSelf->value.asCodeblock.string );

         /* Delete all expressions of the block. */
         while( pExp )
         {
            pNext = pExp->pNext;
            HB_COMP_EXPR_DELETE( pExp );
            pExp = pNext;
         }

         break;
      }
   }
   return pSelf;
}

/* actions for HB_ET_LOGICAL expression
 */
static HB_EXPR_FUNC( hb_compExprUseLogical )
{
   switch( iMessage )
   {
      case HB_EA_REDUCE:
         break;
      case HB_EA_ARRAY_AT:
         HB_COMP_ERROR_TYPE( pSelf );
         break;
      case HB_EA_ARRAY_INDEX:
         hb_compErrorIndex( HB_COMP_PARAM, pSelf );     /* logical cannot be used as array index element */
         break;
      case HB_EA_LVALUE:
         hb_compErrorLValue( HB_COMP_PARAM, pSelf );
         break;
      case HB_EA_PUSH_PCODE:
         HB_GEN_FUNC1( PushLogical, pSelf->value.asLogical );
         break;
      case HB_EA_POP_PCODE:
         break;
      case HB_EA_PUSH_POP:
      case HB_EA_STATEMENT:
         hb_compWarnMeaningless( HB_COMP_PARAM, pSelf );
      case HB_EA_DELETE:
         break;
   }
   return pSelf;
}

/* actions for HB_ET_SELF expression
 */
static HB_EXPR_FUNC( hb_compExprUseSelf )
{
   switch( iMessage )
   {
      case HB_EA_REDUCE:
         break;
      case HB_EA_ARRAY_AT:
         /* Clipper allows such operation and because some valid Clipper
          * code needs it then I disabled error message, [druzus]
          */
         /* HB_COMP_ERROR_TYPE( pSelf ); */
         break;
      case HB_EA_ARRAY_INDEX:
         hb_compErrorIndex( HB_COMP_PARAM, pSelf );     /* SELF cannot be used as array index element */
         break;
      case HB_EA_LVALUE:
         hb_compErrorLValue( HB_COMP_PARAM, pSelf );
         break;
      case HB_EA_PUSH_PCODE:
         HB_GEN_FUNC1( PCode1, HB_P_PUSHSELF );
         break;
      case HB_EA_POP_PCODE:
         break;
      case HB_EA_PUSH_POP:
      case HB_EA_STATEMENT:
         hb_compWarnMeaningless( HB_COMP_PARAM, pSelf );
      case HB_EA_DELETE:
         break;
   }
   return pSelf;
}

/* actions for a literal array { , , , ... }
 */
static HB_EXPR_FUNC( hb_compExprUseArray )
{
   switch( iMessage )
   {
      case HB_EA_REDUCE:
         pSelf = hb_compExprReduceList( pSelf, HB_COMP_PARAM );
         break;

      case HB_EA_ARRAY_AT:
         break;

      case HB_EA_ARRAY_INDEX:
         hb_compErrorIndex( HB_COMP_PARAM, pSelf );     /* array cannot be used as index element */
         break;

      case HB_EA_LVALUE:
         hb_compErrorLValue( HB_COMP_PARAM, pSelf );
         break;

      case HB_EA_PUSH_PCODE:
      {
         USHORT usItems = ( USHORT ) hb_compExprParamListCheck( HB_COMP_PARAM, pSelf );

         if( usItems == 0 )
         {
            HB_GEN_FUNC3( PCode3, HB_P_ARRAYGEN, 0, 0 );
         }
         else
         {
            BOOL fArgsList = pSelf->ExprType == HB_ET_MACROARGLIST;

            if( !fArgsList )
            {
               /* Note: direct type change */
               pSelf->ExprType = HB_ET_ARGLIST;
            }
            HB_EXPR_USE( pSelf, HB_EA_PUSH_PCODE );
            /* restore original expression type */
            pSelf->ExprType = HB_ET_ARRAY;

            if( fArgsList )
            {
               HB_GEN_FUNC3( PCode3, HB_P_MACROARRAYGEN,
                               HB_LOBYTE( usItems ), HB_HIBYTE( usItems ) );
            }
            else
            {
               HB_GEN_FUNC3( PCode3, HB_P_ARRAYGEN,
                               HB_LOBYTE( usItems ), HB_HIBYTE( usItems ) );
            }
         }
         break;
      }

      case HB_EA_POP_PCODE:
         break;

      case HB_EA_PUSH_POP:
      {
         HB_EXPR_PTR pElem = pSelf->value.asList.pExprList;
         /* Push non-constant values only
          */
         while( pElem )
         {
            HB_EXPR_USE( pElem, HB_EA_PUSH_POP );
            pElem = pElem->pNext;
         }
      }
      break;

      case HB_EA_STATEMENT:
         hb_compWarnMeaningless( HB_COMP_PARAM, pSelf );
         break;

      case HB_EA_DELETE:
      {
         HB_EXPR_PTR pElem = pSelf->value.asList.pExprList;
         /* Delete all elements of the array
         */
         HB_EXPR_PTR pNext;
         while( pElem )
         {
            pNext = pElem->pNext;
            HB_COMP_EXPR_DELETE( pElem );
            pElem = pNext;
         }
      }
      break;
   }

   return pSelf;
}

/* actions for HB_ET_HASH literal hash
 *    { key1=>val1, key2=>val2, ... keyN=>valN }
 */
static HB_EXPR_FUNC( hb_compExprUseHash )
{
   switch( iMessage )
   {
      case HB_EA_REDUCE:
         pSelf = hb_compExprReduceList( pSelf, HB_COMP_PARAM );
         break;

      case HB_EA_ARRAY_AT:
         break;

      case HB_EA_ARRAY_INDEX:
         hb_compErrorIndex( HB_COMP_PARAM, pSelf );     /* array cannot be used as index element */
         break;

      case HB_EA_LVALUE:
         hb_compErrorLValue( HB_COMP_PARAM, pSelf );
         break;

      case HB_EA_PUSH_PCODE:
      {
         USHORT usItems = ( USHORT ) ( pSelf->ulLength >> 1 );
         /* Note: direct type change */
         pSelf->ExprType = HB_ET_ARGLIST;
         HB_EXPR_USE( pSelf, HB_EA_PUSH_PCODE );
         /* restore original expression type */
         pSelf->ExprType = HB_ET_HASH;
         HB_GEN_FUNC3( PCode3, HB_P_HASHGEN, HB_LOBYTE( usItems ), HB_HIBYTE( usItems ) );
         break;
      }

      case HB_EA_POP_PCODE:
         break;

      case HB_EA_PUSH_POP:
      {
         HB_EXPR_PTR pElem = pSelf->value.asList.pExprList;
         /* Push non-constant values only */
         while( pElem )
         {
            HB_EXPR_USE( pElem, HB_EA_PUSH_POP );
            pElem = pElem->pNext;
         }
      }
      break;

      case HB_EA_STATEMENT:
         hb_compWarnMeaningless( HB_COMP_PARAM, pSelf );
         break;

      case HB_EA_DELETE:
      {
         HB_EXPR_PTR pElem = pSelf->value.asList.pExprList;
         /* Delete all elements of the array
         */
         HB_EXPR_PTR pNext;
         while( pElem )
         {
            pNext = pElem->pNext;
            HB_COMP_EXPR_DELETE( pElem );
            pElem = pNext;
         }
      }
      break;
   }

   return pSelf;
}

/* actions for HB_ET_VARREF expression
 */
static HB_EXPR_FUNC( hb_compExprUseVarRef )
{
   switch( iMessage )
   {
      case HB_EA_REDUCE:
         break;

      case HB_EA_ARRAY_AT:
         HB_COMP_ERROR_TYPE( pSelf );
         break;

      case HB_EA_ARRAY_INDEX:
         break;

      case HB_EA_LVALUE:
         hb_compErrorLValue( HB_COMP_PARAM, pSelf );
         break;

      case HB_EA_PUSH_PCODE:
         HB_GEN_FUNC1( PushVarRef, pSelf->value.asSymbol );
         break;

      case HB_EA_POP_PCODE:
         break;

      case HB_EA_PUSH_POP:
      case HB_EA_STATEMENT:
         hb_compWarnMeaningless( HB_COMP_PARAM, pSelf );
      case HB_EA_DELETE:
         break;
   }
   return pSelf;
}

/* actions for HB_ET_FUNREF expression
 */
static HB_EXPR_FUNC( hb_compExprUseFunRef )
{
   switch( iMessage )
   {
      case HB_EA_REDUCE:
         break;

      case HB_EA_ARRAY_AT:
         HB_COMP_ERROR_TYPE( pSelf );
         break;

      case HB_EA_ARRAY_INDEX:
         break;

      case HB_EA_LVALUE:
         hb_compErrorLValue( HB_COMP_PARAM, pSelf );
         break;

      case HB_EA_PUSH_PCODE:
         HB_GEN_FUNC1( PushFunRef, pSelf->value.asSymbol );
         break;

      case HB_EA_POP_PCODE:
         break;

      case HB_EA_PUSH_POP:
      case HB_EA_STATEMENT:
         hb_compWarnMeaningless( HB_COMP_PARAM, pSelf );
      case HB_EA_DELETE:
         break;
   }
   return pSelf;
}

/* actions for HB_ET_REFERENCE expression
 */
static HB_EXPR_FUNC( hb_compExprUseRef )
{
   switch( iMessage )
   {
      case HB_EA_REDUCE:
         pSelf->value.asReference = HB_EXPR_USE( pSelf->value.asReference, HB_EA_REDUCE );
         if( pSelf->value.asReference->ExprType == HB_ET_IIF )
         {
            HB_EXPR_PTR pCond, pIIF, pFalse;
            pIIF = pSelf->value.asReference;
            pCond = pIIF->value.asList.pExprList;
            pFalse = hb_compExprNewRef( pCond->pNext->pNext, HB_COMP_PARAM );
            pCond->pNext = hb_compExprNewRef( pCond->pNext, HB_COMP_PARAM );
            pCond->pNext->pNext = pFalse;
            memcpy( pSelf, pIIF, sizeof( HB_EXPR ) );
            HB_COMP_EXPR_CLEAR( pIIF );
         }
         break;
      case HB_EA_ARRAY_AT:
         HB_COMP_ERROR_TYPE( pSelf );
         break;
      case HB_EA_ARRAY_INDEX:
         break;
      case HB_EA_LVALUE:
         hb_compErrorLValue( HB_COMP_PARAM, pSelf );
         break;
      case HB_EA_PUSH_PCODE:
      {
         HB_EXPR_PTR pExp = pSelf->value.asReference;
         if( pExp->ExprType == HB_ET_MACRO )
         {
            pExp->value.asMacro.SubType = HB_ET_MACRO_REFER;
            HB_EXPR_USE( pExp, HB_EA_PUSH_PCODE );
            break;
         }
         else if( pExp->ExprType == HB_ET_ARRAYAT )
         {
            pExp->value.asList.reference = TRUE;
            HB_EXPR_USE( pExp, HB_EA_PUSH_PCODE );
            break;
         }
         else if( pExp->ExprType == HB_ET_SEND )
         {
            /* HB_EXPR_PTR pSend = pExp->value.asMessage.pObject;
            if( !pSend || pSend->ExprType == HB_ET_VARIABLE ) */
            {
               hb_compExprPushSendPop( pExp, HB_COMP_PARAM );
               HB_GEN_FUNC1( PCode1, HB_P_PUSHOVARREF );
               break;
            }
         }
         else if( pExp->ExprType == HB_ET_VARIABLE )
         {
            pExp->ExprType = HB_ET_VARREF;
            HB_EXPR_USE( pExp, HB_EA_PUSH_PCODE );
            pExp->ExprType = HB_ET_VARIABLE;
            break;
         }
         else if( pExp->ExprType == HB_ET_ALIASVAR )
         {
            if( pExp->value.asAlias.pVar->ExprType == HB_ET_VARIABLE &&
                hb_compExprIsMemvarAlias( pExp->value.asAlias.pAlias->value.asSymbol ) )
            {  /* @M-> @MEMVAR-> or @MEMVA-> or @MEMV-> */
               HB_GEN_FUNC1( PushMemvarRef, pExp->value.asAlias.pVar->value.asSymbol );
               break;
            }
         }
         else if( pExp->ExprType == HB_ET_VARREF ||
                  pExp->ExprType == HB_ET_REFERENCE )
         {
            HB_EXPR_USE( pExp, HB_EA_PUSH_PCODE );
            break;
         }

         hb_compErrorRefer( HB_COMP_PARAM, NULL, hb_compExprDescription( pExp ) );
         break;
      }

      case HB_EA_POP_PCODE:
         break;
      case HB_EA_PUSH_POP:
      case HB_EA_STATEMENT:
         hb_compWarnMeaningless( HB_COMP_PARAM, pSelf );
      case HB_EA_DELETE:
         HB_COMP_EXPR_DELETE( pSelf->value.asReference );
         break;
   }
   return pSelf;
}


/* actions for HB_ET_IIF expression
 */
static HB_EXPR_FUNC( hb_compExprUseIIF )
{
   switch( iMessage )
   {
      case HB_EA_REDUCE:
         pSelf = hb_compExprReduceIIF( hb_compExprReduceList( pSelf, HB_COMP_PARAM ), HB_COMP_PARAM );
         break;

      case HB_EA_ARRAY_AT:
      case HB_EA_ARRAY_INDEX:
         break;

      case HB_EA_LVALUE:
         if( HB_SUPPORT_HARBOUR )
         {
            HB_EXPR_PTR pExpr = pSelf->value.asList.pExprList->pNext;
            HB_EXPR_USE( pExpr, HB_EA_LVALUE );
            HB_EXPR_USE( pExpr->pNext, HB_EA_LVALUE );
         }
         else
            hb_compErrorLValue( HB_COMP_PARAM, pSelf );
         break;

      case HB_EA_PUSH_PCODE:
      {
         /* this is called if all three parts of IIF expression should be generated
         */
         LONG lPosFalse, lPosEnd;
         HB_EXPR_PTR pExpr = pSelf->value.asList.pExprList;

         HB_EXPR_USE( pExpr, HB_EA_PUSH_PCODE );
         lPosFalse = HB_GEN_FUNC1( JumpFalse, 0 );
         pExpr =pExpr->pNext;

         HB_EXPR_USE( pExpr, HB_EA_PUSH_PCODE );
         lPosEnd = HB_GEN_FUNC1( Jump, 0 );
         pExpr =pExpr->pNext;

         HB_GEN_FUNC1( JumpHere, lPosFalse );
         HB_EXPR_USE( pExpr, HB_EA_PUSH_PCODE );
         HB_GEN_FUNC1( JumpHere, lPosEnd );
         break;
      }
      case HB_EA_POP_PCODE:
      {
         /* this is called if all three parts of IIF expression should be generated
         */
         LONG lPosFalse, lPosEnd;
         HB_EXPR_PTR pExpr = pSelf->value.asList.pExprList;

         HB_EXPR_USE( pExpr, HB_EA_PUSH_PCODE );
         lPosFalse = HB_GEN_FUNC1( JumpFalse, 0 );
         pExpr =pExpr->pNext;

         HB_EXPR_USE( pExpr, HB_EA_POP_PCODE );
         lPosEnd = HB_GEN_FUNC1( Jump, 0 );
         pExpr =pExpr->pNext;

         HB_GEN_FUNC1( JumpHere, lPosFalse );
         HB_EXPR_USE( pExpr, HB_EA_POP_PCODE );
         HB_GEN_FUNC1( JumpHere, lPosEnd );
         break;
      }

      case HB_EA_PUSH_POP:
      case HB_EA_STATEMENT:
      {
#ifdef HB_CLP_STRICT
         HB_EXPR_USE( pSelf, HB_EA_PUSH_PCODE );
         HB_GEN_FUNC1( PCode1, HB_P_POP );  /* remove a value if used in statement */
#else
         ULONG ulPosFalse, ulPosEnd;
         HB_EXPR_PTR pExpr = pSelf->value.asList.pExprList;

         HB_EXPR_USE( pExpr, HB_EA_PUSH_PCODE );
         ulPosFalse = HB_GEN_FUNC1( JumpFalse, 0 );
         pExpr =pExpr->pNext;

         /* do not generate warning about meaningless use of expression NIL */
         if( pExpr->ExprType != HB_ET_NIL )
            HB_EXPR_USE( pExpr, HB_EA_PUSH_POP );
         pExpr =pExpr->pNext;

#if defined( HB_MACRO_SUPPORT )
         if( HB_PCODE_DATA->lPCodePos == ulPosFalse + 3 )
         {
            HB_PCODE_DATA->pCode[ ulPosFalse - 1 ] = HB_P_JUMPTRUEFAR;
            ulPosEnd = ulPosFalse;
         }
#else
         if( HB_COMP_PARAM->functions.pLast->lPCodePos == ulPosFalse + 3 )
         {
            HB_COMP_PARAM->functions.pLast->pCode[ ulPosFalse - 1 ] = HB_P_JUMPTRUEFAR;
            ulPosEnd = ulPosFalse;
         }
#endif
         else
         {
            ulPosEnd = HB_GEN_FUNC1( Jump, 0 );
            HB_GEN_FUNC1( JumpHere, ulPosFalse );
         }
         /* do not generate warning about meaningless use of expression NIL */
         if( pExpr->ExprType != HB_ET_NIL )
            HB_EXPR_USE( pExpr, HB_EA_PUSH_POP );
         HB_GEN_FUNC1( JumpHere, ulPosEnd );
#endif
         break;
      }
      case HB_EA_DELETE:
         if( pSelf->value.asList.pExprList )
         {
            HB_EXPR_PTR pTmp, pExpr = pSelf->value.asList.pExprList;
            while( pExpr )
            {
               pTmp = pExpr->pNext;    /* store next expression */
               HB_COMP_EXPR_DELETE( pExpr );
               pExpr =pTmp;
            }
            pSelf->value.asList.pExprList = NULL;
         }
         break;
   }
   return pSelf;  /* return self */
}

/* NOTE: In PUSH operation it leaves on the eval stack the last expression only
 */
static HB_EXPR_FUNC( hb_compExprUseList )
{
   switch( iMessage )
   {
      case HB_EA_REDUCE:
         pSelf = hb_compExprReduceList( pSelf, HB_COMP_PARAM );

         if( HB_SUPPORT_HARBOUR )
            pSelf = hb_compExprListStrip( pSelf, HB_COMP_PARAM );

         if( HB_SUPPORT_XBASE && pSelf->ExprType == HB_ET_LIST )
         {
            if( hb_compExprListLen( pSelf ) == 1 )
            {
               HB_EXPR_PTR pExpr = pSelf->value.asList.pExprList;
               if( pExpr->ExprType == HB_ET_MACRO &&
                   pExpr->value.asMacro.SubType != HB_ET_MACRO_SYMBOL &&
                   pExpr->value.asMacro.SubType != HB_ET_MACRO_REFER &&
                   pExpr->value.asMacro.SubType != HB_ET_MACRO_ALIASED )
               {
                  pExpr->value.asMacro.SubType |= HB_ET_MACRO_PARE;
               }
            }
         }
         break;

      case HB_EA_ARRAY_AT:
      case HB_EA_ARRAY_INDEX:
         break;

      case HB_EA_LVALUE:
         if( hb_compExprListLen( pSelf ) == 1 )
         {
            /* For example:
             * ( a ) := 4
             */
            hb_compErrorLValue( HB_COMP_PARAM, pSelf->value.asList.pExprList );
         }
         else
            hb_compErrorLValue( HB_COMP_PARAM, pSelf );
         break;

      case HB_EA_PUSH_PCODE:
         {
            HB_EXPR_PTR pExpr = pSelf->value.asList.pExprList;

            if( pExpr->ExprType == HB_ET_NONE && pExpr->pNext == NULL )
            {
               /* Empty list was used ()
                */
               HB_COMP_ERROR_SYNTAX( pExpr );
            }
            else
            {
               while( pExpr )
               {
                  if( HB_SUPPORT_XBASE )
                  {
                     if( pExpr->ExprType == HB_ET_MACRO &&
                         pExpr->value.asMacro.SubType != HB_ET_MACRO_SYMBOL &&
                         pExpr->value.asMacro.SubType != HB_ET_MACRO_REFER &&
                         pExpr->value.asMacro.SubType != HB_ET_MACRO_ALIASED )
                     {
                        pExpr->value.asMacro.SubType |= HB_ET_MACRO_PARE;
                     }
                  }

                  if( pExpr->pNext )
                     HB_EXPR_USE( pExpr, HB_EA_PUSH_POP );
                  else
                     HB_EXPR_USE( pExpr, HB_EA_PUSH_PCODE );   /* the last expression */
                  pExpr = pExpr->pNext;
               }
            }
         }
         break;

      case HB_EA_POP_PCODE:
         break;

      case HB_EA_PUSH_POP:
      case HB_EA_STATEMENT:
         {
            HB_EXPR_PTR pExpr = pSelf->value.asList.pExprList;

            while( pExpr )
            {
               if( HB_SUPPORT_XBASE )
               {
                  if( pExpr->ExprType == HB_ET_MACRO &&
                      pExpr->value.asMacro.SubType != HB_ET_MACRO_SYMBOL &&
                      pExpr->value.asMacro.SubType != HB_ET_MACRO_REFER &&
                      pExpr->value.asMacro.SubType != HB_ET_MACRO_ALIASED )
                  {
                      pExpr->value.asMacro.SubType |= HB_ET_MACRO_PARE;
                  }
               }

               HB_EXPR_USE( pExpr, HB_EA_PUSH_POP );
               pExpr = pExpr->pNext;
            }
         }
         break;

      case HB_EA_DELETE:
         if( pSelf->value.asList.pExprList )
         {
            HB_EXPR_PTR pTmp, pExpr = pSelf->value.asList.pExprList;
            while( pExpr )
            {
               pTmp = pExpr->pNext;    /* store next expression */
               HB_COMP_EXPR_DELETE( pExpr );
               pExpr =pTmp;
            }
            pSelf->value.asList.pExprList = NULL;
         }
         break;
   }
   return pSelf;
}

/* NOTE: In PUSH operation it leaves all expressions on the eval stack
 */
static HB_EXPR_FUNC( hb_compExprUseArgList )
{
   switch( iMessage )
   {
      case HB_EA_REDUCE:
         pSelf = hb_compExprReduceList( pSelf, HB_COMP_PARAM );
         break;

      case HB_EA_ARRAY_AT:
      case HB_EA_ARRAY_INDEX:
      case HB_EA_LVALUE:
         break;

      case HB_EA_PUSH_PCODE:
         if( pSelf->value.asList.reference )
         {
#if defined( HB_MACRO_SUPPORT )
            if( !HB_PCODE_DATA->fVParams )
#else
            if( !HB_COMP_PARAM->functions.pLast->fVParams )
#endif
            {
               hb_compErrorVParams( HB_COMP_PARAM,
                                    HB_COMP_PARAM->functions.pLast->szName ?
                                    "Function" : "Codeblock" );
            }
            HB_GEN_FUNC1( PCode1, HB_P_PUSHVPARAMS );
         }
         else
         {
            HB_EXPR_PTR pExpr = pSelf->value.asList.pExprList;
            while( pExpr )
            {
               HB_EXPR_USE( pExpr, HB_EA_PUSH_PCODE );
               pExpr = pExpr->pNext;
            }
         }
         break;

      case HB_EA_POP_PCODE:
      case HB_EA_PUSH_POP:
      case HB_EA_STATEMENT:
         break;

      case HB_EA_DELETE:
         if( pSelf->value.asList.pExprList )
         {
            HB_EXPR_PTR pTmp, pExpr = pSelf->value.asList.pExprList;
            while( pExpr )
            {
               pTmp = pExpr->pNext;    /* store next expression */
               HB_COMP_EXPR_DELETE( pExpr );
               pExpr =pTmp;
            }
            pSelf->value.asList.pExprList = NULL;
         }
         break;
   }
   return pSelf;
}

/* NOTE: In PUSH operation it leaves all expressions on the eval stack,
 *       the expresions are divided into macro compiled blocks
 */
static HB_EXPR_FUNC( hb_compExprUseMacroArgList )
{
   switch( iMessage )
   {
      case HB_EA_REDUCE:
         pSelf = hb_compExprReduceList( pSelf, HB_COMP_PARAM );
         break;

      case HB_EA_ARRAY_AT:
      case HB_EA_ARRAY_INDEX:
      case HB_EA_LVALUE:
         break;

      case HB_EA_PUSH_PCODE:
         {
            HB_EXPR_PTR pExpr = pSelf->value.asList.pExprList;
            USHORT usItems = 0;

            while( pExpr )
            {
               if( ( pExpr->ExprType == HB_ET_MACRO &&
                     ( pExpr->value.asMacro.SubType & HB_ET_MACRO_LIST ) ) ||
                   ( pExpr->ExprType == HB_ET_ARGLIST &&
                     pExpr->value.asList.reference ) )
               {
                  if( usItems )
                  {
                     HB_GEN_FUNC1( PushLong, usItems );
                     usItems = 0;
                  }
               }
               else
                  ++usItems;
               HB_EXPR_USE( pExpr, HB_EA_PUSH_PCODE );
               pExpr = pExpr->pNext;
            }
            if( usItems )
            {
               HB_GEN_FUNC1( PushLong, usItems );
            }
         }
         break;

      case HB_EA_POP_PCODE:
      case HB_EA_PUSH_POP:
      case HB_EA_STATEMENT:
         break;

      case HB_EA_DELETE:
         if( pSelf->value.asList.pExprList )
         {
            HB_EXPR_PTR pTmp, pExpr = pSelf->value.asList.pExprList;
            while( pExpr )
            {
               pTmp = pExpr->pNext;    /* store next expression */
               HB_COMP_EXPR_DELETE( pExpr );
               pExpr =pTmp;
            }
            pSelf->value.asList.pExprList = NULL;
         }
         break;
   }
   return pSelf;
}

/* handler for ( ( array[ idx ] )[ idx ] )[ idx ]
 */
static HB_EXPR_FUNC( hb_compExprUseArrayAt )
{
   switch( iMessage )
   {
      case HB_EA_REDUCE:
      {
         HB_EXPR_PTR pIdx;

         /* Clipper forces memvar context for undeclared variables used with
          * array index, f.e.: var[ n ]
          * but not for code like: ( var )[ n ]
          */
         if( pSelf->value.asList.pExprList->ExprType == HB_ET_VARIABLE )
         {
#if !defined( HB_MACRO_SUPPORT )
            int iScope;
            hb_compVariableFind( HB_COMP_PARAM, pSelf->value.asList.pExprList->value.asSymbol, NULL, &iScope );
            if( iScope == HB_VS_UNDECLARED )
            {
               hb_compGenWarning( HB_COMP_PARAM, hb_comp_szWarnings, 'W', HB_COMP_WARN_MEMVAR_ASSUMED,
                                  pSelf->value.asList.pExprList->value.asSymbol, NULL );
#else
            if( hb_macroLocalVarGetPos( pSelf->value.asList.pExprList->value.asSymbol, HB_COMP_PARAM ) == 0 )
            {
#endif
               pSelf->value.asList.pExprList = hb_compExprNewAliasVar(
                              hb_compExprNewAlias( "MEMVAR", HB_COMP_PARAM ),
                              pSelf->value.asList.pExprList, HB_COMP_PARAM );
            }
         }
         pSelf->value.asList.pExprList = HB_EXPR_USE( pSelf->value.asList.pExprList, HB_EA_REDUCE );
         pSelf->value.asList.pIndex = HB_EXPR_USE( pSelf->value.asList.pIndex, HB_EA_REDUCE );
         pIdx = pSelf->value.asList.pIndex;
         if( pIdx->ExprType == HB_ET_NUMERIC )
         {
            HB_EXPR_PTR pExpr = pSelf->value.asList.pExprList; /* the expression that holds an array */
            LONG lIndex;

            if( pIdx->value.asNum.NumType == HB_ET_LONG )
               lIndex = ( LONG ) pIdx->value.asNum.val.l;
            else
               lIndex = ( LONG ) pIdx->value.asNum.val.d;

            if( pExpr->ExprType == HB_ET_ARRAY )   /* is it a literal array */
            {
               ULONG ulSize = hb_compExprParamListCheck( HB_COMP_PARAM, pExpr );

               if( pExpr->ExprType == HB_ET_MACROARGLIST )
                  /* restore original expression type */
                  pExpr->ExprType = HB_ET_ARRAY;
               else if( !HB_IS_VALID_INDEX( lIndex, ulSize ) )
               {
                  if( !HB_SUPPORT_ARRSTR )
                     hb_compErrorBound( HB_COMP_PARAM, pIdx );
               }
               else
               {
                  pExpr = pExpr->value.asList.pExprList; /* the first element in the array */
                  while( --lIndex && pExpr )
                     pExpr = pExpr->pNext;

                  if( pExpr ) /* found ? */
                  {
                     /* extract a single expression from the array
                      */
                     HB_EXPR_PTR pNew = HB_COMP_EXPR_NEW( HB_ET_NONE );
                     memcpy( pNew, pExpr, sizeof( HB_EXPR ) );
                     /* This will suppres releasing of memory occupied by components of
                      * the expression - we have just copied them into the new expression.
                      * This method is simpler then traversing the list and releasing all
                      * but this choosen one.
                      */
                     pExpr->ExprType = HB_ET_NONE;
                     /* Here comes the magic */
                     HB_COMP_EXPR_DELETE( pSelf );
                     pSelf = pNew;
                  }
                  else if( !HB_SUPPORT_ARRSTR )
                     hb_compErrorBound( HB_COMP_PARAM, pIdx );
               }
            }
#if 0
            else if( pExpr->ExprType == HB_ET_STRING && HB_SUPPORT_ARRSTR )   /* is it a literal string */
            {
               if( HB_IS_VALID_INDEX( lIndex, pExpr->ulLength ) )
               {
                  UCHAR ucValue = ( UCHAR ) pExpr->value.asString.string[ lIndex - 1 ];

                  HB_COMP_EXPR_DELETE( pSelf );
                  pSelf = hb_compExprNewLong( ucValue, HB_COMP_PARAM );
               }
               else
                  hb_compErrorBound( HB_COMP_PARAM, pIdx );
            }
#endif
            else if( !HB_SUPPORT_ARRSTR )
            {
               HB_EXPR_USE( pExpr, HB_EA_ARRAY_AT );
            }
         }
         break;
      }

      case HB_EA_ARRAY_AT:
      case HB_EA_ARRAY_INDEX:
      case HB_EA_LVALUE:
         break;

      case HB_EA_PUSH_PCODE:
      {
         BOOL fMacroIndex = FALSE;

         if( pSelf->value.asList.pIndex->ExprType == HB_ET_MACRO )
         {
            if( HB_SUPPORT_XBASE )
            {
               if( pSelf->value.asMacro.SubType != HB_ET_MACRO_SYMBOL &&
                   pSelf->value.asMacro.SubType != HB_ET_MACRO_REFER &&
                   pSelf->value.asMacro.SubType != HB_ET_MACRO_ALIASED )
               {
                  pSelf->value.asList.pIndex->value.asMacro.SubType |= HB_ET_MACRO_LIST;
                  fMacroIndex = TRUE;
               }
            }
         }
         else if( pSelf->value.asList.pIndex->ExprType == HB_ET_ARGLIST )
         {
            fMacroIndex = pSelf->value.asList.pIndex->value.asList.reference;
         }

         if( pSelf->value.asList.reference && HB_SUPPORT_ARRSTR )
         {
            HB_EXPR_PTR pList = pSelf->value.asList.pExprList;
            if( pList->ExprType == HB_ET_VARIABLE )
            {
               pList->ExprType = HB_ET_VARREF;
               HB_EXPR_USE( pList, HB_EA_PUSH_PCODE );
               pList->ExprType = HB_ET_VARIABLE;
            }
            else if( pList->ExprType == HB_ET_ALIASVAR &&
                     pList->value.asAlias.pVar->ExprType == HB_ET_VARIABLE &&
                     hb_compExprIsMemvarAlias( pList->value.asAlias.pAlias->value.asSymbol ) )
            {
               HB_GEN_FUNC1( PushMemvarRef, pList->value.asAlias.pVar->value.asSymbol );
            }
            else if( pList->ExprType == HB_ET_SEND )
            {
               hb_compExprPushSendPop( pList, HB_COMP_PARAM );
               HB_GEN_FUNC1( PCode1, HB_P_PUSHOVARREF );
            }
            else if( pList->ExprType == HB_ET_ARRAYAT &&
                     !pList->value.asList.reference )
            {
               pList->value.asList.reference = TRUE;
               HB_EXPR_USE( pList, HB_EA_PUSH_PCODE );
               pList->value.asList.reference = FALSE;
            }
            else if( pList->ExprType == HB_ET_MACRO &&
                     pList->value.asMacro.SubType == HB_ET_MACRO_VAR )
            {
               pList->value.asMacro.SubType = HB_ET_MACRO_REFER;
               HB_EXPR_USE( pList, HB_EA_PUSH_PCODE );
               pList->value.asMacro.SubType = HB_ET_MACRO_VAR;
            }
            else
               HB_EXPR_USE( pList, HB_EA_PUSH_PCODE );
         }
         else
            HB_EXPR_USE( pSelf->value.asList.pExprList, HB_EA_PUSH_PCODE );
         HB_EXPR_USE( pSelf->value.asList.pIndex, HB_EA_PUSH_PCODE );
         if( fMacroIndex )
            HB_GEN_FUNC1( PCode1, HB_P_MACROPUSHINDEX );
         if( pSelf->value.asList.reference )
            HB_GEN_FUNC1( PCode1, HB_P_ARRAYPUSHREF );
         else
            HB_GEN_FUNC1( PCode1, HB_P_ARRAYPUSH );
         break;
      }

      case HB_EA_POP_PCODE:
      {
         BOOL fMacroIndex = FALSE;
         if( pSelf->value.asList.pIndex->ExprType == HB_ET_MACRO )
         {
            if( HB_SUPPORT_XBASE )
            {
               if( pSelf->value.asMacro.SubType != HB_ET_MACRO_SYMBOL &&
                   pSelf->value.asMacro.SubType != HB_ET_MACRO_REFER &&
                   pSelf->value.asMacro.SubType != HB_ET_MACRO_ALIASED )
               {
                  pSelf->value.asList.pIndex->value.asMacro.SubType |= HB_ET_MACRO_LIST;
                  fMacroIndex = TRUE;
               }
            }
         }
         else if( pSelf->value.asList.pIndex->ExprType == HB_ET_ARGLIST )
         {
            fMacroIndex = pSelf->value.asList.pIndex->value.asList.reference;
         }
         /* to manage strings as bytes arrays, they must be pushed by reference */
         /* arrays also are passed by reference */
         if( HB_SUPPORT_ARRSTR )
         {
            HB_EXPR_PTR pList = pSelf->value.asList.pExprList;
            if( pList->ExprType == HB_ET_VARIABLE )
            {
               pList->ExprType = HB_ET_VARREF;
               HB_EXPR_USE( pList, HB_EA_PUSH_PCODE );
               pList->ExprType = HB_ET_VARIABLE;
            }
            else if( pList->ExprType == HB_ET_ALIASVAR &&
                     pList->value.asAlias.pVar->ExprType == HB_ET_VARIABLE &&
                     hb_compExprIsMemvarAlias( pList->value.asAlias.pAlias->value.asSymbol ) )
            {
               HB_GEN_FUNC1( PushMemvarRef, pList->value.asAlias.pVar->value.asSymbol );
            }
            else if( pList->ExprType == HB_ET_SEND )
            {
               hb_compExprPushSendPop( pList, HB_COMP_PARAM );
               HB_GEN_FUNC1( PCode1, HB_P_PUSHOVARREF );
            }
            else if( pList->ExprType == HB_ET_ARRAYAT &&
                     !pList->value.asList.reference )
            {
               pList->value.asList.reference = TRUE;
               HB_EXPR_USE( pList, HB_EA_PUSH_PCODE );
               pList->value.asList.reference = FALSE;
            }
            else if( pList->ExprType == HB_ET_MACRO &&
                     pList->value.asMacro.SubType == HB_ET_MACRO_VAR )
            {
               pList->value.asMacro.SubType = HB_ET_MACRO_REFER;
               HB_EXPR_USE( pList, HB_EA_PUSH_PCODE );
               pList->value.asMacro.SubType = HB_ET_MACRO_VAR;
            }
            else
               HB_EXPR_USE( pList, HB_EA_PUSH_PCODE );
         }
         else
            HB_EXPR_USE( pSelf->value.asList.pExprList, HB_EA_PUSH_PCODE );
         HB_EXPR_USE( pSelf->value.asList.pIndex, HB_EA_PUSH_PCODE );
         if( fMacroIndex )
            HB_GEN_FUNC1( PCode1, HB_P_MACROPUSHINDEX );
         HB_GEN_FUNC1( PCode1, HB_P_ARRAYPOP );
         break;
      }

      case HB_EA_PUSH_POP:
         /* NOTE: This is highly optimized code - this will work even
          * if accessed value isn't an array. It will work also if
          * the index is invalid
          */
         HB_EXPR_USE( pSelf->value.asList.pExprList, HB_EA_PUSH_POP );
         HB_EXPR_USE( pSelf->value.asList.pIndex, HB_EA_PUSH_POP );
         /* no break */
      case HB_EA_STATEMENT:
         hb_compWarnMeaningless( HB_COMP_PARAM, pSelf );
         break;

      case HB_EA_DELETE:
         HB_COMP_EXPR_DELETE( pSelf->value.asList.pExprList );
         HB_COMP_EXPR_DELETE( pSelf->value.asList.pIndex );
         break;
   }
   return pSelf;
}

static HB_EXPR_FUNC( hb_compExprUseMacro )
{
   switch( iMessage )
   {
      case HB_EA_REDUCE:
         if( pSelf->value.asMacro.pExprList )
            pSelf->value.asMacro.pExprList = HB_EXPR_USE( pSelf->value.asMacro.pExprList, HB_EA_REDUCE );
         break;

      case HB_EA_ARRAY_AT:
      case HB_EA_ARRAY_INDEX:
      case HB_EA_LVALUE:
         break;

      case HB_EA_PUSH_PCODE:
         if( pSelf->value.asMacro.SubType & HB_ET_MACRO_ASSIGN )
            HB_GEN_FUNC2( PushString, "_", 2 );

         if( pSelf->value.asMacro.pExprList )
         {
            /* macro expression: &( expressions_list )
             * NOTE: only the last expression will be macro-compiled
             */
            HB_EXPR_USE( pSelf->value.asMacro.pExprList, HB_EA_PUSH_PCODE );
         }
         else
         {
            if( pSelf->value.asMacro.cMacroOp )
            {
               /* simple macro variable expansion: &variable
                * 'szMacro' is a variable name
                */
               HB_GEN_FUNC2( PushVar, pSelf->value.asMacro.szMacro, TRUE );
            }
            else
            {
               /* complex macro expression: prefix&var.suffix
                * all components should be placed as a string that will
                * be compiled after text susbstitution
                */

               /* Check if macrotext variable does not refer to
                * local, static or field.
                */
#if ! defined( HB_MACRO_SUPPORT )
               hb_compIsValidMacroText( HB_COMP_PARAM,
                                        pSelf->value.asMacro.szMacro,
                                        strlen( pSelf->value.asMacro.szMacro ) );
#endif
               HB_GEN_FUNC2( PushString, pSelf->value.asMacro.szMacro, strlen( pSelf->value.asMacro.szMacro ) + 1 );
            }
         }

         if( pSelf->value.asMacro.SubType & HB_ET_MACRO_ASSIGN )
         {
            HB_GEN_FUNC1( PCode1, HB_P_PLUS );
            pSelf->value.asMacro.SubType &= ~HB_ET_MACRO_ASSIGN;
         }

         /* compile & run - leave a result on the eval stack
          */
         if( pSelf->value.asMacro.SubType == HB_ET_MACRO_SYMBOL )
            HB_GEN_FUNC1( PCode1, HB_P_MACROSYMBOL );

         else if( pSelf->value.asMacro.SubType == HB_ET_MACRO_REFER )
            HB_GEN_FUNC1( PCode1, HB_P_MACROPUSHREF );

         else if( pSelf->value.asMacro.SubType != HB_ET_MACRO_ALIASED )
         {
            if( HB_SUPPORT_XBASE )
            {
               if( pSelf->value.asMacro.SubType & HB_ET_MACRO_LIST )
               {
                  /* { &macro }, funCall( &macro ) or var[ &macro ] */
                  HB_GEN_FUNC1( PCode1, HB_P_MACROPUSHLIST );
               }
               else if( pSelf->value.asMacro.SubType & HB_ET_MACRO_PARE )
               {
                  /* var := (somevalue, &macro) - in Xbase++ compatibility mode
                   * EVAL( {|| &macro} ) - in all cases
                   */
                  HB_GEN_FUNC1( PCode1, HB_P_MACROPUSHPARE );
               }
               else
               {
                  /* usual &macro */
                  HB_GEN_FUNC1( PCode1, HB_P_MACROPUSH );
               }
            }
            else
               /* usual &macro */
               HB_GEN_FUNC1( PCode1, HB_P_MACROPUSH );

            /* Always add byte to pcode indicating requested macro compiler flag. */
            HB_GEN_FUNC1( PCode1, ( BYTE ) HB_MACRO_GENFLAGS );
         }

         /* NOTE: pcode for alias context is generated in
          * hb_compExprUseAliasVar()
          */
         break;

      case HB_EA_POP_PCODE:
         if( pSelf->value.asMacro.pExprList )
         {
            /* macro expression: &( expressions_list )
             * NOTE: only the last expression will be macro-compiled
             */
            HB_EXPR_USE( pSelf->value.asMacro.pExprList, HB_EA_PUSH_PCODE );
         }
         else
         {
            if( pSelf->value.asMacro.cMacroOp )
            {
               /* simple macro variable expansion: &variable
                * 'szMacro' is a variable name
                */
               HB_GEN_FUNC2( PushVar, pSelf->value.asMacro.szMacro, TRUE );
            }
            else
            {
               /* complex macro expression: prefix&var.suffix
                * all components should be placed as a string that will
                * be compiled after text susbstitution
                */

               /* Check if macrotext variable does not refer to
                * local, static or field.
                */
#if ! defined( HB_MACRO_SUPPORT )
               hb_compIsValidMacroText( HB_COMP_PARAM,
                                        pSelf->value.asMacro.szMacro,
                                        strlen( pSelf->value.asMacro.szMacro ) );
#endif
               HB_GEN_FUNC2( PushString, pSelf->value.asMacro.szMacro, strlen( pSelf->value.asMacro.szMacro ) + 1 );
            }
         }
         /* compile & run - macro compiler will generate pcode to pop a value
          * from the eval stack
          */
         if( pSelf->value.asMacro.SubType != HB_ET_MACRO_ALIASED )
         {
            HB_GEN_FUNC1( PCode1, HB_P_MACROPOP );

            /* Always add byte to pcode indicating requested macro compiler flag. */
            HB_GEN_FUNC1( PCode1, ( BYTE ) HB_MACRO_GENFLAGS );
         }
         break;

      case HB_EA_PUSH_POP:
      case HB_EA_STATEMENT:
         HB_EXPR_USE( pSelf, HB_EA_PUSH_PCODE );
         HB_GEN_FUNC1( PCode1, HB_P_POP );
         break;

      case HB_EA_DELETE:
         if( pSelf->value.asMacro.pExprList )
            HB_COMP_EXPR_DELETE( pSelf->value.asMacro.pExprList );
         break;
   }
   return pSelf;
}

static HB_EXPR_FUNC( hb_compExprUseFunCall )
{
   switch( iMessage )
   {
      case HB_EA_REDUCE:
         /* Reduce the expressions on the list of arguments
          */
         if( pSelf->value.asFunCall.pParms )
            pSelf->value.asFunCall.pParms = HB_EXPR_USE( pSelf->value.asFunCall.pParms, HB_EA_REDUCE );

         if( pSelf->value.asFunCall.pFunName->ExprType == HB_ET_FUNNAME )
         {
            HB_EXPR_PTR pName = pSelf->value.asFunCall.pFunName;
            HB_EXPR_PTR pParms = pSelf->value.asFunCall.pParms;
            USHORT usCount = ( USHORT ) hb_compExprParamListLen( pParms );

#ifndef HB_MACRO_SUPPORT
            if( hb_compFunCallCheck( HB_COMP_PARAM, pName->value.asSymbol, usCount ) )
#endif
            {
               if( strcmp( "AT", pName->value.asSymbol ) == 0 )
               {
                  if( usCount == 2 )
                     hb_compExprReduceAT( pSelf, HB_COMP_PARAM );
               }
               else if( strcmp( "CHR", pName->value.asSymbol ) == 0 )
               {
                  if( usCount )
                     hb_compExprReduceCHR( pSelf, HB_COMP_PARAM );
               }
               else if( strcmp( "LEN", pName->value.asSymbol ) == 0 )
               {
                  if( usCount && HB_SUPPORT_HARBOUR )
                     hb_compExprReduceLEN( pSelf, HB_COMP_PARAM );
               }
               else if( strcmp( "EMPTY", pName->value.asSymbol ) == 0 )
               {
                  if( usCount && HB_SUPPORT_HARBOUR )
                     hb_compExprReduceEMPTY( pSelf, HB_COMP_PARAM );
               }
               else if( strcmp( "ASC", pName->value.asSymbol ) == 0 )
               {
                  if( usCount && HB_SUPPORT_HARBOUR )
                     hb_compExprReduceASC( pSelf, HB_COMP_PARAM );
               }
               else if( strcmp( "INT", pName->value.asSymbol ) == 0 )
               {
                  if( usCount == 1 && HB_SUPPORT_HARBOUR )
                     hb_compExprReduceINT( pSelf, HB_COMP_PARAM );
               }
               else if( strcmp( "STOD", pName->value.asSymbol ) == 0 ||
                        strcmp( "HB_STOD", pName->value.asSymbol ) == 0 )
               {
                  if( usCount < 2 && HB_SUPPORT_HARBOUR )
                     hb_compExprReduceSTOD( pSelf, usCount, HB_COMP_PARAM );
               }
               else if( strcmp( "HB_STOT", pName->value.asSymbol ) == 0 )
               {
                  hb_compExprReduceSTOT( pSelf, usCount, HB_COMP_PARAM );
               }
               else if( strcmp( "DTOS", pName->value.asSymbol ) == 0 )
               {
                  if( usCount == 1 )
                     hb_compExprReduceDTOS( pSelf, HB_COMP_PARAM );
               }
               else if( strcmp( "CTOD", pName->value.asSymbol ) == 0 )
               {
                  if( usCount && HB_SUPPORT_HARBOUR )
                     hb_compExprReduceCTOD( pSelf, HB_COMP_PARAM );
               }
               else if( strcmp( "MIN", pName->value.asSymbol ) == 0 )
               {
                  if( usCount == 2 && HB_SUPPORT_HARBOUR )
                     hb_compExprReduceMIN( pSelf, HB_COMP_PARAM );
               }
               else if( strcmp( "MAX", pName->value.asSymbol ) == 0 )
               {
                  if( usCount == 2 && HB_SUPPORT_HARBOUR )
                     hb_compExprReduceMAX( pSelf, HB_COMP_PARAM );
               }
               else if( strcmp( "UPPER", pName->value.asSymbol ) == 0 )
               {
                  if( usCount )
                     hb_compExprReduceUPPER( pSelf, HB_COMP_PARAM );
               }
               else if( strncmp( "HB_BIT", pName->value.asSymbol, 6 ) == 0 &&
                        usCount && pParms->value.asList.pExprList->ExprType == HB_ET_NUMERIC )
               {
                  HB_EXPR_PTR   pArg = pParms->value.asList.pExprList;
                  HB_LONG lResult = 0;
                  BOOL fOptimize = FALSE, fBool = FALSE;

                  if( usCount >= 2 )
                  {
                     if( pArg->pNext->ExprType == HB_ET_NUMERIC )
                     {
                        if( strcmp( "AND", pName->value.asSymbol + 6 ) == 0 )
                        {
                           fOptimize = TRUE;
                           lResult = hb_compExprAsLongNum( pArg );
                           while( --usCount )
                           {
                              pArg = pArg->pNext;
                              if( pArg->ExprType != HB_ET_NUMERIC )
                              {
                                 fOptimize = FALSE;
                                 break;
                              }
                              lResult &= hb_compExprAsLongNum( pArg );
                           }
                        }
                        else if( strcmp( "OR", pName->value.asSymbol + 6 ) == 0 )
                        {
                           fOptimize = TRUE;
                           lResult = hb_compExprAsLongNum( pArg );
                           while( --usCount )
                           {
                              pArg = pArg->pNext;
                              if( pArg->ExprType != HB_ET_NUMERIC )
                              {
                                 fOptimize = FALSE;
                                 break;
                              }
                              lResult |= hb_compExprAsLongNum( pArg );
                           }
                        }
                        else if( strcmp( "XOR", pName->value.asSymbol + 6 ) == 0 )
                        {
                           fOptimize = TRUE;
                           lResult = hb_compExprAsLongNum( pArg );
                           while( --usCount )
                           {
                              pArg = pArg->pNext;
                              if( pArg->ExprType != HB_ET_NUMERIC )
                              {
                                 fOptimize = FALSE;
                                 break;
                              }
                              lResult ^= hb_compExprAsLongNum( pArg );
                           }
                        }
                        else if( strcmp( "TEST", pName->value.asSymbol + 6 ) == 0 )
                        {
                           HB_LONG lBit = hb_compExprAsLongNum( pArg->pNext );
                           lResult = ( hb_compExprAsLongNum( pArg ) &
                                       ( ( HB_LONG ) 1 << lBit ) ) != 0;
                           fOptimize = fBool = TRUE;
                        }
                        else if( strcmp( "SET", pName->value.asSymbol + 6 ) == 0 )
                        {
                           HB_LONG lBit = hb_compExprAsLongNum( pArg->pNext );
                           lResult = hb_compExprAsLongNum( pArg ) |
                                     ( ( HB_LONG ) 1 << lBit );
                           fOptimize = TRUE;
                        }
                        else if( strcmp( "RESET", pName->value.asSymbol + 6 ) == 0 )
                        {
                           HB_LONG lBit = hb_compExprAsLongNum( pArg->pNext );
                           lResult = hb_compExprAsLongNum( pArg ) &
                                     ( ~ ( ( HB_LONG ) 1 << lBit ) );
                           fOptimize = TRUE;
                        }
                        else if( strcmp( "SHIFT", pName->value.asSymbol + 6 ) == 0 )
                        {
                           HB_LONG lBits = hb_compExprAsLongNum( pArg->pNext );
                           lResult = hb_compExprAsLongNum( pArg );
                           if( lBits < 0 )
                              lResult >>= -lBits;
                           else
                              lResult <<= lBits;
                           fOptimize = TRUE;
                        }
                     }
                  }
                  else if( strcmp( "NOT", pName->value.asSymbol + 6 ) == 0 )
                  {
                     lResult = ~hb_compExprAsLongNum( pArg );
                     fOptimize = TRUE;
                  }
                  if( fOptimize )
                     hb_compExprReduceBitFunc( pSelf, lResult, fBool, HB_COMP_PARAM );
               }
#ifndef HB_MACRO_SUPPORT
               else if( strncmp( "HB_I18N_", pName->value.asSymbol, 8 ) == 0 )
               {
                  HB_EXPR_PTR pArg = pParms->value.asList.pExprList, pCount = NULL;
                  BOOL        fStrict, fNoop, fPlural, fI18nFunc;
                  ULONG       ulPos = 8;

                  fStrict = fNoop = fPlural = fI18nFunc = FALSE;
                  if( pName->value.asSymbol[ ulPos ] == 'N' )
                  {
                     fPlural = TRUE;
                     ulPos++;
                  }
                  if( strncmp( "GETTEXT", &pName->value.asSymbol[ ulPos ], 7 ) == 0 )
                  {
                     ulPos += 7;
                     if( strncmp( "_STRICT", &pName->value.asSymbol[ ulPos ], 7 ) == 0 )
                     {
                        ulPos += 7;
                        if( !pName->value.asSymbol[ ulPos ] || pName->value.asSymbol[ ulPos ] == '_' )
                           fI18nFunc = fStrict = TRUE;
                     }
                     else if( strncmp( "_NOOP", &pName->value.asSymbol[ ulPos ], 5 ) == 0 )
                     {
                        ulPos += 5;
                        if( !pName->value.asSymbol[ ulPos ] || pName->value.asSymbol[ ulPos ] == '_' )
                           fI18nFunc = fNoop = TRUE;
                     }
                     else if( !pName->value.asSymbol[ ulPos ] || pName->value.asSymbol[ ulPos ] == '_' )
                        fI18nFunc = TRUE;
                  }
                  if( fI18nFunc )
                  {
                     int            iWarning = 0;
                     HB_EXPR_PTR    pBadParam = NULL;
                     const char *   szExpect = NULL;
                     const char *   szContext = NULL;

                     if( fPlural && usCount )
                     {
                        pCount = pArg;
                        pArg = pArg->pNext;
                        --usCount;
                        if( pCount->ExprType <= HB_ET_FUNREF &&
                            pCount->ExprType != HB_ET_NUMERIC )
                        {
                           iWarning = HB_COMP_WARN_PARAM_TYPE;
                           pBadParam = pCount;
                           szExpect = "Numeric expression";
                        }
                     }
                     if( usCount == 2 )
                     {
                        if( pArg->pNext->ExprType == HB_ET_STRING && pArg->pNext->ulLength > 0 )
                        {
                           szContext = pArg->pNext->value.asString.string;
                           --usCount;
                        }
                        else
                        {
                           iWarning = HB_COMP_WARN_PARAM_TYPE;
                           pBadParam = pArg->pNext;
                           szExpect = "String";
                        }
                     }
                     if( iWarning == 0 )
                     {
                        const char * szPlurals[ HB_I18N_PLURAL_MAX ];
                        if( usCount == 1 )
                        {
                           if( pArg->ExprType == HB_ET_STRING )
                           {
                              if( HB_COMP_PARAM->fI18n && pArg->ulLength > 0 )
                              {
                                 if( pCount )
                                 {
                                    szPlurals[ 0 ] = pArg->value.asString.string;
                                    hb_compI18nAddPlural( HB_COMP_PARAM, szPlurals, 1, szContext,
                                                          HB_COMP_PARAM->currModule, HB_COMP_PARAM->currLine );
                                 }
                                 else
                                    hb_compI18nAdd( HB_COMP_PARAM, pArg->value.asString.string, szContext,
                                                    HB_COMP_PARAM->currModule, HB_COMP_PARAM->currLine );
                              }
                           }
                           else if( pCount && pArg->ExprType == HB_ET_ARRAY &&
                                    hb_compExprListTypeCheck( pArg, HB_ET_STRING ) )
                           {
                              if( HB_COMP_PARAM->fI18n )
                              {
                                 ULONG ulLen = hb_compExprListLen( pArg ), ul;
                                 HB_EXPR_PTR pArgExp = pArg->value.asList.pExprList;

                                 if( ulLen > HB_I18N_PLURAL_MAX )
                                    ulLen = HB_I18N_PLURAL_MAX;
                                 for( ul = 0; ul < ulLen; ++ul )
                                 {
                                    szPlurals[ ul ] = pArgExp->value.asString.string;
                                    pArgExp = pArgExp->pNext;
                                 }
                                 hb_compI18nAddPlural( HB_COMP_PARAM, szPlurals, ulLen, szContext,
                                                       HB_COMP_PARAM->currModule, HB_COMP_PARAM->currLine );
                              }
                           }
                           else if( fStrict || fNoop || pArg->ExprType <= HB_ET_FUNREF )
                           {
                              iWarning = HB_COMP_WARN_PARAM_TYPE;
                              pBadParam = pArg;
                              szExpect = fPlural ? "String or Array of Strings" : "String";
                           }
                        }
                        else
                           iWarning = HB_COMP_WARN_PARAM_COUNT;
                     }
                     if( iWarning != 0 )
                     {
                        /* TODO: warning message does not fit very well, because it requires
                         *       type of used parameter. Let's print "unknown", to avoid deeper
                         *       analysis of parameter.
                         */
                        if( iWarning == HB_COMP_WARN_PARAM_TYPE )
                           hb_compGenWarning( HB_COMP_PARAM, hb_comp_szWarnings, 'W', HB_COMP_WARN_PARAM_TYPE,
                                              pBadParam && pBadParam->ExprType > HB_ET_NONE &&
                                              pBadParam->ExprType <= HB_ET_FUNREF ?
                                              hb_compExprDescription( pBadParam ) : "Unknown", szExpect );
                        else
                        {
                           char buf[ 16 ];
                           hb_snprintf( buf, sizeof( buf ), "%d", ( int ) usCount + ( pCount ? 1 : 0 ) + ( szContext ? 1 : 0 ) );
                           hb_compGenWarning( HB_COMP_PARAM, hb_comp_szWarnings, 'W', HB_COMP_WARN_PARAM_COUNT, buf, fPlural ? "2 or 3" : "1 or 2" );
                        }
                     }
                     /* hb_i18n_gettext_noop() is not a real function. It is used to
                        force writing of string to .pot file. So, we should try to
                        replace function call by first argument regardless fI18n flag
                        and warnings.
                      */
                     else if( fNoop && usCount )
                     {
                        pParms->value.asList.pExprList = pArg->pNext; /* skip first parameter */
                        pArg->pNext = NULL;
                        HB_COMP_EXPR_FREE( pParms );
                        HB_COMP_EXPR_FREE( pSelf->value.asFunCall.pFunName );
                        if( pCount )
                        {
                           if( pArg->ExprType == HB_ET_ARRAY )
                           {
                              if( hb_compExprListLen( pArg ) == 1 )
                              {
                                 HB_COMP_EXPR_FREE( pCount );
                                 pCount = pArg;
                                 pArg = pArg->value.asList.pExprList;
                                 pCount->value.asList.pExprList = NULL;
                              }
                              else
                              {
                                 /* build expression: pArray[ iif( pCount == 1, 1, 2 ) ] */
                                 HB_EXPR_PTR pIndex;

                                 /* create pCount == 1 */
                                 pIndex = hb_compExprSetOperand( hb_compExprNewEQ( pCount, HB_COMP_PARAM ),
                                                                 hb_compExprNewLong( 1, HB_COMP_PARAM ), HB_COMP_PARAM );
                                 /* create: ( pCount == 1, */
                                 pIndex = hb_compExprNewList( pIndex, HB_COMP_PARAM );
                                 /* create: ( pCount == 1, 1, */
                                 pIndex = hb_compExprAddListExpr( pIndex, hb_compExprNewLong( 1, HB_COMP_PARAM ) );
                                 /* create: ( pCount == 1, 1, 2 )*/
                                 pIndex = hb_compExprAddListExpr( pIndex, hb_compExprNewLong( 2, HB_COMP_PARAM ) );
                                 /* create: IIF() expression */
                                 pIndex = hb_compExprNewIIF( pIndex );
                                 /* create: pArray[ iif( pCount == 1, 1, 2 ) ] */
                                 pArg = hb_compExprNewArrayAt( pArg, pIndex, HB_COMP_PARAM );
                                 /* reduce the final expression */
                                 pArg = HB_EXPR_USE( pArg, HB_EA_REDUCE );
                                 pCount = NULL;
                              }
                           }
                           if( pCount )
                              HB_COMP_EXPR_FREE( pCount );
                        }
                        memcpy( pSelf, pArg, sizeof( HB_EXPR ) );
                        /* free pArg expression body but without freeing its subexpressions */
                        HB_COMP_EXPR_CLEAR( pArg );
                     }
                  }
               }
#endif
            }
         }
         break;

      case HB_EA_ARRAY_AT:
      case HB_EA_ARRAY_INDEX:
         break;

      case HB_EA_LVALUE:
         hb_compErrorLValue( HB_COMP_PARAM, pSelf );
         break;

      case HB_EA_PUSH_PCODE:
      {
         BOOL fArgsList = FALSE;
         USHORT usCount = 0;

         if( pSelf->value.asFunCall.pFunName->ExprType == HB_ET_FUNNAME )
         {
            HB_GEN_FUNC1( PushFunCall, pSelf->value.asFunCall.pFunName->value.asSymbol );
         }
         else
         {
            HB_EXPR_USE( pSelf->value.asFunCall.pFunName, HB_EA_PUSH_PCODE );
            HB_GEN_FUNC1( PCode1, HB_P_PUSHNIL );
         }

         /* NOTE: pParms will be NULL in 'DO procname' (if there is
          * no WITH keyword)
          */
         if( pSelf->value.asFunCall.pParms )
         {
            usCount = ( USHORT ) hb_compExprParamListCheck( HB_COMP_PARAM, pSelf->value.asFunCall.pParms );
            fArgsList = pSelf->value.asFunCall.pParms->ExprType == HB_ET_MACROARGLIST;
            if( usCount )
               HB_EXPR_USE( pSelf->value.asFunCall.pParms, HB_EA_PUSH_PCODE );
         }

         if( fArgsList )
         {
            HB_GEN_FUNC3( PCode3, HB_P_MACROFUNC, HB_LOBYTE( usCount ), HB_HIBYTE( usCount ) );
            /* restore original expression type */
            pSelf->value.asFunCall.pParms->ExprType = HB_ET_ARGLIST;
         }
         else if( usCount > 255 )
            HB_GEN_FUNC3( PCode3, HB_P_FUNCTION, HB_LOBYTE( usCount ), HB_HIBYTE( usCount ) );
         else
            HB_GEN_FUNC2( PCode2, HB_P_FUNCTIONSHORT, ( BYTE ) usCount );
         break;
      }
      case HB_EA_POP_PCODE:
         break;

      case HB_EA_PUSH_POP:
      case HB_EA_STATEMENT:
      {
         BOOL fArgsList = FALSE;
         USHORT usCount = 0;

         if( pSelf->value.asFunCall.pFunName->ExprType == HB_ET_FUNNAME )
         {
            HB_GEN_FUNC1( PushFunCall, pSelf->value.asFunCall.pFunName->value.asSymbol );
         }
         else
         {
            HB_EXPR_USE( pSelf->value.asFunCall.pFunName, HB_EA_PUSH_PCODE );
            HB_GEN_FUNC1( PCode1, HB_P_PUSHNIL );
         }

         if( pSelf->value.asFunCall.pParms )
         {
            usCount = ( USHORT ) hb_compExprParamListCheck( HB_COMP_PARAM, pSelf->value.asFunCall.pParms );
            fArgsList = pSelf->value.asFunCall.pParms->ExprType == HB_ET_MACROARGLIST;
            if( usCount )
               HB_EXPR_USE( pSelf->value.asFunCall.pParms, HB_EA_PUSH_PCODE );
         }

         if( fArgsList )
         {
            HB_GEN_FUNC3( PCode3, HB_P_MACRODO, HB_LOBYTE( usCount ), HB_HIBYTE( usCount ) );
            /* restore original expression type */
            pSelf->value.asFunCall.pParms->ExprType = HB_ET_ARGLIST;
         }
         else if( usCount > 255 )
            HB_GEN_FUNC3( PCode3, HB_P_DO, HB_LOBYTE( usCount ), HB_HIBYTE( usCount ) );
         else
            HB_GEN_FUNC2( PCode2, HB_P_DOSHORT, ( BYTE ) usCount );
         break;
      }
      case HB_EA_DELETE:
         if( pSelf->value.asFunCall.pParms )
            HB_COMP_EXPR_DELETE( pSelf->value.asFunCall.pParms );
         HB_COMP_EXPR_DELETE( pSelf->value.asFunCall.pFunName );
         break;
   }
   return pSelf;
}

/* handler for expression->identifier syntax
 */
static HB_EXPR_FUNC( hb_compExprUseAliasVar )
{
   HB_TRACE(HB_TR_DEBUG, ("hb_compExprUseAliasVar()"));

   switch( iMessage )
   {
      case HB_EA_REDUCE:
         /* NOTE: direct reduction not used for HB_ET_LIST to avoid
          *       list stripping before PUSH/POP operations
          */
         if( pSelf->value.asAlias.pAlias->ExprType == HB_ET_LIST )
         {
            pSelf->value.asAlias.pAlias = hb_compExprReduceList(
                                 pSelf->value.asAlias.pAlias, HB_COMP_PARAM );
            if( HB_SUPPORT_EXTOPT &&
                pSelf->value.asAlias.pAlias->value.asList.pExprList->ExprType == HB_ET_STRING &&
                pSelf->value.asAlias.pAlias->value.asList.pExprList->pNext == NULL )
            {
               pSelf->value.asAlias.pAlias = hb_compExprReduceAliasString(
                           pSelf->value.asAlias.pAlias,
                           pSelf->value.asAlias.pAlias->value.asList.pExprList,
                           HB_COMP_PARAM );
            }
         }
         else
            pSelf->value.asAlias.pAlias = HB_EXPR_USE( pSelf->value.asAlias.pAlias, HB_EA_REDUCE );
         break;

      case HB_EA_ARRAY_AT:
      case HB_EA_ARRAY_INDEX:
      case HB_EA_LVALUE:
         break;

      case HB_EA_PUSH_PCODE:
      {
         HB_EXPR_PTR pAlias = pSelf->value.asAlias.pAlias;

         if( pAlias->ExprType == HB_ET_MACRO ||
             pSelf->value.asAlias.pVar->ExprType == HB_ET_MACRO )
         {
            /* Macro operator is used on the left or right side of an alias
             * operator - handle it with a special care
             */
            HB_EXPR_PCODE2( hb_compExprUseAliasMacro, pSelf, HB_EA_PUSH_PCODE );
         }
         else if( pAlias->ExprType == HB_ET_ALIAS )
         {
            /*
             * myalias->var
             * FIELD->var
             * MEMVAR->var
             *
             * NOTE: TRUE = push also alias
             */
             HB_GEN_FUNC4( PushAliasedVar, pSelf->value.asAlias.pVar->value.asSymbol, TRUE, pAlias->value.asSymbol, 0 );
         }
         else if( pAlias->ExprType == HB_ET_NUMERIC )
         {
            /* numeric alias
             * 2->var
             *
             * NOTE: only integer (long) values are allowed
             */
            if( pAlias->value.asNum.NumType == HB_ET_LONG )
               HB_GEN_FUNC4( PushAliasedVar, pSelf->value.asAlias.pVar->value.asSymbol, TRUE, NULL, pAlias->value.asNum.val.l );
            else
               hb_compErrorAlias( HB_COMP_PARAM, pAlias );
         }
         else if( pAlias->ExprType == HB_ET_LIST )
         {
            /*
             * ( expression )->var
             *
             * NOTE: FALSE = don't push alias value
             */
            HB_EXPR_USE( pAlias, HB_EA_PUSH_PCODE );
            HB_GEN_FUNC4( PushAliasedVar, pSelf->value.asAlias.pVar->value.asSymbol, FALSE, NULL, 0 );
         }
         else
            hb_compErrorAlias( HB_COMP_PARAM, pAlias );
         break;
      }
      case HB_EA_POP_PCODE:
      {
         HB_EXPR_PTR pAlias = pSelf->value.asAlias.pAlias;

         if( pAlias->ExprType == HB_ET_MACRO || pSelf->value.asAlias.pVar->ExprType == HB_ET_MACRO )
         {
            /* Macro operator is used on the left or right side of an alias
             * operator - handle it with a special care
             * (we need convert to a string the whole expression)
             */
            HB_EXPR_PCODE2( hb_compExprUseAliasMacro, pSelf, HB_EA_POP_PCODE );
         }
         else if( pAlias->ExprType == HB_ET_ALIAS )
         {
            /*
             * myalias->var
             * FIELD->var
             * MEMVAR->var
             */
            HB_GEN_FUNC4( PopAliasedVar, pSelf->value.asAlias.pVar->value.asSymbol, TRUE, pAlias->value.asSymbol, 0 );
         }
         else if( pAlias->ExprType == HB_ET_NUMERIC )
         {
            /* numeric alias
             * 2->var
             *
             * NOTE: only integer (long) values are allowed
             */
            if( pAlias->value.asNum.NumType == HB_ET_LONG )
               HB_GEN_FUNC4( PopAliasedVar, pSelf->value.asAlias.pVar->value.asSymbol, TRUE, NULL, pAlias->value.asNum.val.l );
            else
               hb_compErrorAlias( HB_COMP_PARAM, pAlias );
         }
         else if( pAlias->ExprType == HB_ET_LIST )
         {
            /*
             * ( expression )->var
             *
             * NOTE: FALSE = don't push alias value
             */
            HB_EXPR_USE( pAlias, HB_EA_PUSH_PCODE );
            HB_GEN_FUNC4( PopAliasedVar, pSelf->value.asAlias.pVar->value.asSymbol, FALSE, NULL, 0 );
         }
         else
            hb_compErrorAlias( HB_COMP_PARAM, pAlias );
         break;
      }
      case HB_EA_PUSH_POP:
      case HB_EA_STATEMENT:
         HB_EXPR_USE( pSelf, HB_EA_PUSH_PCODE );
         HB_GEN_FUNC1( PCode1, HB_P_POP );
         break;

      case HB_EA_DELETE:
         HB_COMP_EXPR_DELETE( pSelf->value.asAlias.pAlias );
         if( pSelf->value.asAlias.pVar )
            HB_COMP_EXPR_DELETE( pSelf->value.asAlias.pVar );
         break;
   }
   return pSelf;
}

/* handler for expression->( exression, ... ) syntax
 */
static HB_EXPR_FUNC( hb_compExprUseAliasExpr )
{
   switch( iMessage )
   {
      case HB_EA_REDUCE:
         pSelf->value.asAlias.pAlias   = HB_EXPR_USE( pSelf->value.asAlias.pAlias, HB_EA_REDUCE );
         pSelf->value.asAlias.pExpList = HB_EXPR_USE( pSelf->value.asAlias.pExpList, HB_EA_REDUCE );
         if( HB_SUPPORT_EXTOPT && pSelf->value.asAlias.pAlias->ExprType == HB_ET_STRING )
            pSelf->value.asAlias.pAlias = hb_compExprReduceAliasString(
                                 pSelf->value.asAlias.pAlias,
                                 pSelf->value.asAlias.pAlias, HB_COMP_PARAM );
         break;

      case HB_EA_ARRAY_AT:
      case HB_EA_ARRAY_INDEX:
         break;

      case HB_EA_LVALUE:
         hb_compErrorLValue( HB_COMP_PARAM, pSelf );
         break;

      case HB_EA_PUSH_PCODE:
         /* save currently selected workarea
          */
         HB_GEN_FUNC1( PCode1, HB_P_PUSHALIAS );
         /* push the expression that will return a new workarea
          */
         HB_EXPR_USE( pSelf->value.asAlias.pAlias, HB_EA_PUSH_PCODE );
         /* pop the value from the stack and select it as current workarea
          */
         HB_GEN_FUNC1( PCode1, HB_P_POPALIAS );
         /* evaluate any expression
          */
         HB_EXPR_USE( pSelf->value.asAlias.pExpList, HB_EA_PUSH_PCODE );
         /* swap the two last items on the eval stack: one item is a
          * value returned by evaluated expression and the second item
          * is previously selected workarea. After swaping select again
          * the restored workarea.
          */
         HB_GEN_FUNC1( PCode1, HB_P_SWAPALIAS );
         break;

      case HB_EA_POP_PCODE:
         hb_compErrorLValue( HB_COMP_PARAM, pSelf );
         break;

      case HB_EA_PUSH_POP:
      case HB_EA_STATEMENT:
         /* save currently selected workarea
          */
         HB_GEN_FUNC1( PCode1, HB_P_PUSHALIAS );
         /* push the expression that will return a new workarea
          */
         HB_EXPR_USE( pSelf->value.asAlias.pAlias, HB_EA_PUSH_PCODE );
         /* pop the value from the stack and select it as current workarea
          */
         HB_GEN_FUNC1( PCode1, HB_P_POPALIAS );
         /* evaluate any expression - it will not leave any return
          * value on the eval stack
          */
         HB_EXPR_USE( pSelf->value.asAlias.pExpList, HB_EA_PUSH_POP );
         /* Pop and select again the restored workarea.
          */
         HB_GEN_FUNC1( PCode1, HB_P_POPALIAS );
         break;

      case HB_EA_DELETE:
         HB_COMP_EXPR_DELETE( pSelf->value.asAlias.pAlias );
         HB_COMP_EXPR_DELETE( pSelf->value.asAlias.pExpList );
         break;
   }
   return pSelf;
}

static HB_EXPR_FUNC( hb_compExprUseAlias )
{
   switch( iMessage )
   {
      case HB_EA_REDUCE:
      case HB_EA_ARRAY_AT:
      case HB_EA_ARRAY_INDEX:
      case HB_EA_LVALUE:
         break;

      case HB_EA_PUSH_PCODE:
         HB_GEN_FUNC2( PushSymbol, pSelf->value.asSymbol, HB_SYM_ALIAS );
         break;

      case HB_EA_POP_PCODE:
      case HB_EA_PUSH_POP:
      case HB_EA_STATEMENT:
         break;

      case HB_EA_DELETE:
         break;
   }
   return pSelf;
}

static HB_EXPR_FUNC( hb_compExprUseFunName )
{
   switch( iMessage )
   {
      case HB_EA_REDUCE:
      case HB_EA_ARRAY_AT:
      case HB_EA_ARRAY_INDEX:
      case HB_EA_LVALUE:
         break;

      case HB_EA_PUSH_PCODE:
         HB_GEN_FUNC1( PushFunSym, pSelf->value.asSymbol );
         break;

      case HB_EA_POP_PCODE:
      case HB_EA_PUSH_POP:
      case HB_EA_STATEMENT:
      case HB_EA_DELETE:
         break;
   }
   return pSelf;
}

static HB_EXPR_FUNC( hb_compExprUseRTVariable )
{
   switch( iMessage )
   {
      case HB_EA_REDUCE:
      case HB_EA_ARRAY_AT:
      case HB_EA_ARRAY_INDEX:
      case HB_EA_LVALUE:
         break;
      case HB_EA_PUSH_PCODE:
         if( pSelf->value.asRTVar.szName )
            HB_GEN_FUNC2( PushSymbol, pSelf->value.asRTVar.szName, HB_SYM_MEMVAR );  /* this is not a function */
         else
            HB_EXPR_USE( pSelf->value.asRTVar.pMacro, HB_EA_PUSH_PCODE );
         break;
      case HB_EA_POP_PCODE:
         if( pSelf->value.asRTVar.szName )
            HB_GEN_FUNC1( PopMemvar, pSelf->value.asRTVar.szName );
         else
            HB_EXPR_USE( pSelf->value.asRTVar.pMacro, HB_EA_POP_PCODE );
         break;

      case HB_EA_PUSH_POP:
      case HB_EA_STATEMENT:
         break;
      case HB_EA_DELETE:
#if ! defined( HB_MACRO_SUPPORT )
         if( ! pSelf->value.asRTVar.szName )
            HB_COMP_EXPR_DELETE( pSelf->value.asRTVar.pMacro );
#endif
         break;
   }
   return pSelf;
}


static HB_EXPR_FUNC( hb_compExprUseVariable )
{
   switch( iMessage )
   {
      case HB_EA_REDUCE:
      case HB_EA_ARRAY_AT:
      case HB_EA_ARRAY_INDEX:
      case HB_EA_LVALUE:
         break;
      case HB_EA_PUSH_PCODE:
#if defined( HB_MACRO_SUPPORT )
         {
            /* NOTE: When the following syntax is used:
            *    ( any_expr )->&var2
            * then macro compiler is compiling the right side of alias
            * operator only (if 'any_expr' is not a string) - an alias value
            * is placed on the eval stack before macro compilation.
            * The HB_MACRO_GEN_ALIASED flag is used to signal that we have to
            * genearate alias aware pcode even if we known a variable part only.
            */
            if( HB_MACRO_DATA->Flags & HB_MACRO_GEN_ALIASED )
               HB_GEN_FUNC4( PushAliasedVar, pSelf->value.asSymbol, FALSE, NULL, 0 );
            else
               HB_GEN_FUNC2( PushVar, pSelf->value.asSymbol, FALSE );
         }
#else
         HB_GEN_FUNC2( PushVar, pSelf->value.asSymbol, FALSE );
#endif
          break;

       case HB_EA_POP_PCODE:
#if defined( HB_MACRO_SUPPORT )
         {
            if( HB_MACRO_DATA->Flags & HB_MACRO_GEN_ALIASED )
               HB_GEN_FUNC4( PopAliasedVar, pSelf->value.asSymbol, FALSE, NULL, 0 );
            else
               HB_GEN_FUNC1( PopVar, pSelf->value.asSymbol );
         }
#else
         HB_GEN_FUNC1( PopVar, pSelf->value.asSymbol );
#endif
         break;

      case HB_EA_PUSH_POP:
      case HB_EA_STATEMENT:
         HB_GEN_FUNC2( PushVar, pSelf->value.asSymbol, FALSE );
         HB_GEN_FUNC1( PCode1, HB_P_POP );
         break;

      case HB_EA_DELETE:
         break;
   }
   return pSelf;
}

static HB_EXPR_FUNC( hb_compExprUseSend )
{
   switch( iMessage )
   {
      case HB_EA_REDUCE:
         /* Clipper does not reduce object expressions */
         if( HB_SUPPORT_HARBOUR && pSelf->value.asMessage.pObject )
            pSelf->value.asMessage.pObject = HB_EXPR_USE( pSelf->value.asMessage.pObject, HB_EA_REDUCE );
         if( pSelf->value.asMessage.pParms )  /* Is it a method call ? */
            pSelf->value.asMessage.pParms = HB_EXPR_USE( pSelf->value.asMessage.pParms, HB_EA_REDUCE );
         break;

      case HB_EA_ARRAY_AT:
      case HB_EA_ARRAY_INDEX:
         break;

      case HB_EA_LVALUE:
         if( pSelf->value.asMessage.pParms )
            hb_compErrorLValue( HB_COMP_PARAM, pSelf );
         break;

      case HB_EA_PUSH_PCODE:
         if( pSelf->value.asMessage.pParms )  /* Is it a method call ? */
         {
            BOOL fArgsList = FALSE;
            int iParms = ( int ) hb_compExprParamListCheck( HB_COMP_PARAM, pSelf->value.asMessage.pParms );

            hb_compExprPushSendPush( pSelf, HB_COMP_PARAM );
            if( iParms )
            {
               HB_EXPR_USE( pSelf->value.asMessage.pParms, HB_EA_PUSH_PCODE );
               fArgsList = pSelf->value.asMessage.pParms->ExprType == HB_ET_MACROARGLIST;
            }

            if( fArgsList )
            {
               HB_GEN_FUNC3( PCode3, HB_P_MACROSEND, HB_LOBYTE( iParms ), HB_HIBYTE( iParms ) );
               /* restore original expression type */
               pSelf->value.asMessage.pParms->ExprType = HB_ET_ARGLIST;
            }
            else if( iParms > 255 )
               HB_GEN_FUNC3( PCode3, HB_P_SEND, HB_LOBYTE( iParms ), HB_HIBYTE( iParms ) );
            else
               HB_GEN_FUNC2( PCode2, HB_P_SENDSHORT, ( BYTE ) iParms );
         }
         else
         {
            /* acces to instance variable */
            hb_compExprPushSendPush( pSelf, HB_COMP_PARAM );
            HB_GEN_FUNC2( PCode2, HB_P_SENDSHORT, 0 );
         }
         break;

      case HB_EA_POP_PCODE:
         hb_compExprPushSendPop( pSelf, HB_COMP_PARAM );
         if( pSelf->value.asMessage.pParms )
         {
            HB_EXPR_USE( pSelf->value.asMessage.pParms, HB_EA_PUSH_PCODE );
         }
         else
         {
            /* executed from macro compiler */
            HB_GEN_FUNC2( PCode2, HB_P_SWAP, 1 );
            HB_GEN_FUNC2( PCode2, HB_P_SWAP, 1 );
         }
         HB_GEN_FUNC2( PCode2, HB_P_SENDSHORT, 1 );
         if( !pSelf->value.asMessage.pParms )
            HB_GEN_FUNC1( PCode1, HB_P_POP );
         break;

      case HB_EA_PUSH_POP:
      case HB_EA_STATEMENT:
         HB_EXPR_USE( pSelf, HB_EA_PUSH_PCODE );
         HB_GEN_FUNC1( PCode1, HB_P_POP );
         if( ! pSelf->value.asMessage.pParms )  /* Is it a method call ? */
         {
            /* instance variable */
            /* QUESTION: This warning can be misleading if nested messages
             * are used, e.g. a:b():c - should we generate it ?
             */
            hb_compWarnMeaningless( HB_COMP_PARAM, pSelf );
         }
         break;

      case HB_EA_DELETE:
         {
            if( pSelf->value.asMessage.pObject )
            {
               HB_COMP_EXPR_DELETE( pSelf->value.asMessage.pObject );
            }
            if( pSelf->value.asMessage.pParms )
            {
               HB_COMP_EXPR_DELETE( pSelf->value.asMessage.pParms );
            }
            if( pSelf->value.asMessage.pMessage )
            {
               HB_COMP_EXPR_DELETE( pSelf->value.asMessage.pMessage );
            }
         }
         break;
   }
   return pSelf;
}

static HB_EXPR_FUNC( hb_compExprUsePostInc )
{
   switch( iMessage )
   {
      case HB_EA_REDUCE:
         pSelf->value.asOperator.pLeft = HB_EXPR_USE( pSelf->value.asOperator.pLeft, HB_EA_REDUCE );
         HB_EXPR_USE( pSelf->value.asOperator.pLeft, HB_EA_LVALUE );
         break;
      case HB_EA_ARRAY_AT:
      case HB_EA_ARRAY_INDEX:
         break;

      case HB_EA_LVALUE:
         hb_compErrorLValue( HB_COMP_PARAM, pSelf );
         break;

      case HB_EA_PUSH_PCODE:
         HB_EXPR_PCODE2( hb_compExprPushPostOp, pSelf, HB_P_INC );
         break;

      case HB_EA_POP_PCODE:
         break;

      case HB_EA_PUSH_POP:
      case HB_EA_STATEMENT:
         /* a++ used standalone as a statement is the same as ++a
          */
         HB_EXPR_PCODE2( hb_compExprUsePreOp, pSelf, HB_P_INC );
         break;

      case HB_EA_DELETE:
         if( pSelf->value.asOperator.pLeft )
            HB_COMP_EXPR_DELETE( pSelf->value.asOperator.pLeft );
         break;
   }
   return pSelf;
}

static HB_EXPR_FUNC( hb_compExprUsePostDec )
{
   switch( iMessage )
   {
      case HB_EA_REDUCE:
         pSelf->value.asOperator.pLeft = HB_EXPR_USE( pSelf->value.asOperator.pLeft, HB_EA_REDUCE );
         HB_EXPR_USE( pSelf->value.asOperator.pLeft, HB_EA_LVALUE );
         break;
      case HB_EA_ARRAY_AT:
      case HB_EA_ARRAY_INDEX:
         break;

      case HB_EA_LVALUE:
         hb_compErrorLValue( HB_COMP_PARAM, pSelf );
         break;

      case HB_EA_PUSH_PCODE:
         HB_EXPR_PCODE2( hb_compExprPushPostOp, pSelf, HB_P_DEC );
         break;

      case HB_EA_POP_PCODE:
         break;

      case HB_EA_PUSH_POP:
      case HB_EA_STATEMENT:
         HB_EXPR_PCODE2( hb_compExprUsePreOp, pSelf, HB_P_DEC );
         break;

      case HB_EA_DELETE:
         if( pSelf->value.asOperator.pLeft )
            HB_COMP_EXPR_DELETE( pSelf->value.asOperator.pLeft );
         break;
   }
   return pSelf;
}

static HB_EXPR_FUNC( hb_compExprUseAssign )
{
   switch( iMessage )
   {
      case HB_EA_REDUCE:
      {
         HB_EXPR_PTR pExpr;

         pSelf->value.asOperator.pLeft  = HB_EXPR_USE( pSelf->value.asOperator.pLeft, HB_EA_REDUCE );
         pSelf->value.asOperator.pRight = HB_EXPR_USE( pSelf->value.asOperator.pRight, HB_EA_REDUCE );
         HB_EXPR_USE( pSelf->value.asOperator.pLeft, HB_EA_LVALUE );

         /* optimize:
          *    var := var <op> <exp>
          * to:
          *    var <op>= <exp>
          */
         pExpr = pSelf->value.asOperator.pRight;
         if( HB_SUPPORT_HARBOUR &&
             pSelf->value.asOperator.pLeft->ExprType == HB_ET_VARIABLE &&
             ( pExpr->ExprType == HB_EO_PLUS || pExpr->ExprType == HB_EO_MINUS ||
               pExpr->ExprType == HB_EO_MULT || pExpr->ExprType == HB_EO_DIV ||
               pExpr->ExprType == HB_EO_MOD  || pExpr->ExprType == HB_EO_POWER ) &&
             pExpr->value.asOperator.pLeft->ExprType == HB_ET_VARIABLE &&
             strcmp( pSelf->value.asOperator.pLeft->value.asSymbol,
                     pExpr->value.asOperator.pLeft->value.asSymbol ) == 0 )
         {
            /* NOTE: direct type change */
            switch( pExpr->ExprType )
            {
               case HB_EO_PLUS:
                  pSelf->ExprType = HB_EO_PLUSEQ;
                  break;
               case HB_EO_MINUS:
                  pSelf->ExprType = HB_EO_MINUSEQ;
                  break;
               case HB_EO_MULT:
                  pSelf->ExprType = HB_EO_MULTEQ;
                  break;
               case HB_EO_DIV:
                  pSelf->ExprType = HB_EO_DIVEQ;
                  break;
               case HB_EO_MOD:
                  pSelf->ExprType = HB_EO_MODEQ;
                  break;
               case HB_EO_POWER:
                  pSelf->ExprType = HB_EO_EXPEQ;
                  break;
            }
            pSelf->value.asOperator.pRight = pExpr->value.asOperator.pRight;
            pExpr->value.asOperator.pRight = NULL;
            HB_COMP_EXPR_FREE( pExpr );
         }
         break;
      }

      case HB_EA_ARRAY_AT:
      case HB_EA_ARRAY_INDEX:
      case HB_EA_LVALUE:
         break;

      case HB_EA_PUSH_PCODE:
         /* NOTE: assigment to an object instance variable needs special handling
          */
         if( pSelf->value.asOperator.pLeft->ExprType == HB_ET_SEND )
         {
            HB_EXPR_PTR pObj = pSelf->value.asOperator.pLeft;
            pObj->value.asMessage.pParms = pSelf->value.asOperator.pRight;
            HB_EXPR_USE( pObj, HB_EA_POP_PCODE );
            pObj->value.asMessage.pParms = NULL; /* to suppress duplicated releasing */
         }
         else
         {
            /* it assigns a value and leaves it on the stack */

            HB_EXPR_USE( pSelf->value.asOperator.pRight, HB_EA_PUSH_PCODE );
            /* QUESTION: Can  we replace DUPLICATE+POP with a single PUT opcode
            */
            HB_GEN_FUNC1( PCode1, HB_P_PUSHUNREF );
            HB_EXPR_USE( pSelf->value.asOperator.pLeft, HB_EA_POP_PCODE );
         }
         break;

      case HB_EA_POP_PCODE:
         break;

      case HB_EA_PUSH_POP:
      case HB_EA_STATEMENT:
         /* NOTE: assigment to an object instance variable needs special handling
          */
         if( pSelf->value.asOperator.pLeft->ExprType == HB_ET_SEND )
         {
            HB_EXPR_PTR pObj = pSelf->value.asOperator.pLeft;
            pObj->value.asMessage.pParms = pSelf->value.asOperator.pRight;
            HB_EXPR_USE( pObj, HB_EA_POP_PCODE );
            pObj->value.asMessage.pParms = NULL; /* to suppress duplicated releasing */
            /* Remove the return value */
            HB_GEN_FUNC1( PCode1, HB_P_POP );
         }
         else
         {
            /* it assigns a value and removes it from the stack */
            HB_EXPR_USE( pSelf->value.asOperator.pRight, HB_EA_PUSH_PCODE );
            HB_EXPR_USE( pSelf->value.asOperator.pLeft, HB_EA_POP_PCODE );
         }
         break;

      case HB_EA_DELETE:
         HB_EXPR_PCODE1( hb_compExprDelOperator, pSelf );
         break;
   }
   return pSelf;
}

static HB_EXPR_FUNC( hb_compExprUsePlusEq )
{
   switch( iMessage )
   {
      case HB_EA_REDUCE:
         pSelf->value.asOperator.pLeft  = HB_EXPR_USE( pSelf->value.asOperator.pLeft, HB_EA_REDUCE );
         pSelf->value.asOperator.pRight = HB_EXPR_USE( pSelf->value.asOperator.pRight, HB_EA_REDUCE );
         HB_EXPR_USE( pSelf->value.asOperator.pLeft, HB_EA_LVALUE );
         break;

      case HB_EA_ARRAY_AT:
         HB_COMP_ERROR_TYPE( pSelf );
         break;

      case HB_EA_ARRAY_INDEX:
         break;

      case HB_EA_LVALUE:
         hb_compErrorLValue( HB_COMP_PARAM, pSelf );
         break;

      case HB_EA_PUSH_PCODE:
         HB_EXPR_PCODE2( hb_compExprPushOperEq, pSelf, HB_P_PLUS );
         break;

      case HB_EA_POP_PCODE:
         break;

      case HB_EA_PUSH_POP:
      case HB_EA_STATEMENT:
         HB_EXPR_PCODE2( hb_compExprUseOperEq, pSelf, HB_P_PLUS );
         break;

      case HB_EA_DELETE:
         HB_EXPR_PCODE1( hb_compExprDelOperator, pSelf );
         break;
   }
   return pSelf;
}

static HB_EXPR_FUNC( hb_compExprUseMinusEq )
{
   switch( iMessage )
   {
      case HB_EA_REDUCE:
         pSelf->value.asOperator.pLeft  = HB_EXPR_USE( pSelf->value.asOperator.pLeft, HB_EA_REDUCE );
         pSelf->value.asOperator.pRight = HB_EXPR_USE( pSelf->value.asOperator.pRight, HB_EA_REDUCE );
         HB_EXPR_USE( pSelf->value.asOperator.pLeft, HB_EA_LVALUE );
         break;

      case HB_EA_ARRAY_AT:
         HB_COMP_ERROR_TYPE( pSelf );
         break;

      case HB_EA_ARRAY_INDEX:
         break;

      case HB_EA_LVALUE:
         hb_compErrorLValue( HB_COMP_PARAM, pSelf );
         break;

      case HB_EA_PUSH_PCODE:
         HB_EXPR_PCODE2( hb_compExprPushOperEq, pSelf, HB_P_MINUS );
         break;

      case HB_EA_POP_PCODE:
         break;

      case HB_EA_PUSH_POP:
      case HB_EA_STATEMENT:
         HB_EXPR_PCODE2( hb_compExprUseOperEq, pSelf, HB_P_MINUS );
         break;

      case HB_EA_DELETE:
         HB_EXPR_PCODE1( hb_compExprDelOperator, pSelf );
         break;
   }
   return pSelf;
}

static HB_EXPR_FUNC( hb_compExprUseMultEq )
{
   switch( iMessage )
   {
      case HB_EA_REDUCE:
         pSelf->value.asOperator.pLeft  = HB_EXPR_USE( pSelf->value.asOperator.pLeft, HB_EA_REDUCE );
         pSelf->value.asOperator.pRight = HB_EXPR_USE( pSelf->value.asOperator.pRight, HB_EA_REDUCE );
         HB_EXPR_USE( pSelf->value.asOperator.pLeft, HB_EA_LVALUE );
         break;

      case HB_EA_ARRAY_AT:
         HB_COMP_ERROR_TYPE( pSelf );
         break;

      case HB_EA_ARRAY_INDEX:
         break;

      case HB_EA_LVALUE:
         hb_compErrorLValue( HB_COMP_PARAM, pSelf );
         break;

      case HB_EA_PUSH_PCODE:
         HB_EXPR_PCODE2( hb_compExprPushOperEq, pSelf, HB_P_MULT );
         break;

      case HB_EA_POP_PCODE:
         break;

      case HB_EA_PUSH_POP:
      case HB_EA_STATEMENT:
         HB_EXPR_PCODE2( hb_compExprUseOperEq, pSelf, HB_P_MULT );
         break;

      case HB_EA_DELETE:
         HB_EXPR_PCODE1( hb_compExprDelOperator, pSelf );
         break;
   }
   return pSelf;
}

static HB_EXPR_FUNC( hb_compExprUseDivEq )
{
   switch( iMessage )
   {
      case HB_EA_REDUCE:
         pSelf->value.asOperator.pLeft  = HB_EXPR_USE( pSelf->value.asOperator.pLeft, HB_EA_REDUCE );
         pSelf->value.asOperator.pRight = HB_EXPR_USE( pSelf->value.asOperator.pRight, HB_EA_REDUCE );
         HB_EXPR_USE( pSelf->value.asOperator.pLeft, HB_EA_LVALUE );
         break;

      case HB_EA_ARRAY_AT:
         HB_COMP_ERROR_TYPE( pSelf );
         break;

      case HB_EA_ARRAY_INDEX:
         break;

      case HB_EA_LVALUE:
         hb_compErrorLValue( HB_COMP_PARAM, pSelf );
         break;

      case HB_EA_PUSH_PCODE:
         HB_EXPR_PCODE2( hb_compExprPushOperEq, pSelf, HB_P_DIVIDE );
         break;

      case HB_EA_POP_PCODE:
         break;

      case HB_EA_PUSH_POP:
      case HB_EA_STATEMENT:
         HB_EXPR_PCODE2( hb_compExprUseOperEq, pSelf, HB_P_DIVIDE );
         break;

      case HB_EA_DELETE:
         HB_EXPR_PCODE1( hb_compExprDelOperator, pSelf );
         break;
   }
   return pSelf;
}

static HB_EXPR_FUNC( hb_compExprUseModEq )
{
   switch( iMessage )
   {
      case HB_EA_REDUCE:
         pSelf->value.asOperator.pLeft  = HB_EXPR_USE( pSelf->value.asOperator.pLeft, HB_EA_REDUCE );
         pSelf->value.asOperator.pRight = HB_EXPR_USE( pSelf->value.asOperator.pRight, HB_EA_REDUCE );
         HB_EXPR_USE( pSelf->value.asOperator.pLeft, HB_EA_LVALUE );
         break;

      case HB_EA_ARRAY_AT:
         HB_COMP_ERROR_TYPE( pSelf );
         break;

      case HB_EA_ARRAY_INDEX:
         break;

      case HB_EA_LVALUE:
         hb_compErrorLValue( HB_COMP_PARAM, pSelf );
         break;

      case HB_EA_PUSH_PCODE:
         HB_EXPR_PCODE2( hb_compExprPushOperEq, pSelf, HB_P_MODULUS );
         break;

      case HB_EA_POP_PCODE:
         break;

      case HB_EA_PUSH_POP:
      case HB_EA_STATEMENT:
         HB_EXPR_PCODE2( hb_compExprUseOperEq, pSelf, HB_P_MODULUS );
         break;

      case HB_EA_DELETE:
         HB_EXPR_PCODE1( hb_compExprDelOperator, pSelf );
         break;
   }
   return pSelf;
}

static HB_EXPR_FUNC( hb_compExprUseExpEq )
{
   switch( iMessage )
   {
      case HB_EA_REDUCE:
         pSelf->value.asOperator.pLeft  = HB_EXPR_USE( pSelf->value.asOperator.pLeft, HB_EA_REDUCE );
         pSelf->value.asOperator.pRight = HB_EXPR_USE( pSelf->value.asOperator.pRight, HB_EA_REDUCE );
         HB_EXPR_USE( pSelf->value.asOperator.pLeft, HB_EA_LVALUE );
         break;

      case HB_EA_ARRAY_AT:
         HB_COMP_ERROR_TYPE( pSelf );
         break;

      case HB_EA_ARRAY_INDEX:
         break;

      case HB_EA_LVALUE:
         hb_compErrorLValue( HB_COMP_PARAM, pSelf );
         break;

      case HB_EA_PUSH_PCODE:
         HB_EXPR_PCODE2( hb_compExprPushOperEq, pSelf, HB_P_POWER );
         break;

      case HB_EA_POP_PCODE:
         break;

      case HB_EA_PUSH_POP:
      case HB_EA_STATEMENT:
         HB_EXPR_PCODE2( hb_compExprUseOperEq, pSelf, HB_P_POWER );
         break;

      case HB_EA_DELETE:
         HB_EXPR_PCODE1( hb_compExprDelOperator, pSelf );
         break;
   }
   return pSelf;
}

static HB_EXPR_FUNC( hb_compExprUseOr )
{
   switch( iMessage )
   {
      case HB_EA_REDUCE:
         pSelf->value.asOperator.pLeft  = HB_EXPR_USE( pSelf->value.asOperator.pLeft, HB_EA_REDUCE );
         pSelf->value.asOperator.pRight = HB_EXPR_USE( pSelf->value.asOperator.pRight, HB_EA_REDUCE );
         pSelf = hb_compExprReduceOr( pSelf, HB_COMP_PARAM );
         break;

      case HB_EA_ARRAY_AT:
         HB_COMP_ERROR_TYPE( pSelf );
         break;

      case HB_EA_ARRAY_INDEX:
         hb_compErrorIndex( HB_COMP_PARAM, pSelf );
         break;

      case HB_EA_LVALUE:
         hb_compErrorLValue( HB_COMP_PARAM, pSelf );
         break;

      case HB_EA_PUSH_PCODE:
         if( HB_COMP_ISSUPPORTED( HB_COMPFLAG_SHORTCUTS ) )
         {
            LONG lEndPos;

            HB_EXPR_USE( pSelf->value.asOperator.pLeft, HB_EA_PUSH_PCODE );
            HB_GEN_FUNC1( PCode1, HB_P_DUPLICATE );
            lEndPos = HB_GEN_FUNC1( JumpTrue, 0 );
            HB_GEN_FUNC1( PCode1, HB_P_POP );
            HB_EXPR_USE( pSelf->value.asOperator.pRight, HB_EA_PUSH_PCODE );
            HB_GEN_FUNC1( JumpHere, lEndPos );
         }
         else
         {
            HB_EXPR_USE( pSelf->value.asOperator.pLeft, HB_EA_PUSH_PCODE );
            HB_EXPR_USE( pSelf->value.asOperator.pRight, HB_EA_PUSH_PCODE );
            HB_GEN_FUNC1( PCode1, HB_P_OR );
         }
         break;

      case HB_EA_POP_PCODE:
         break;

      case HB_EA_PUSH_POP:
         if( HB_COMP_ISSUPPORTED( HB_COMPFLAG_SHORTCUTS ) )
         {
            LONG lEndPos;
            HB_EXPR_USE( pSelf->value.asOperator.pLeft, HB_EA_PUSH_PCODE );
            lEndPos = HB_GEN_FUNC1( JumpTrue, 0 );
            /* NOTE: This will not generate a runtime error if incompatible
             * data type is used
             */
#if defined( HB_MACRO_SUPPORT )
            HB_EXPR_USE( pSelf->value.asOperator.pRight, HB_EA_PUSH_POP );
#else
            {
               BOOL fMeaningful = HB_COMP_PARAM->fMeaningful;
               /* do not generate warning about meaningless expression usage */
               HB_COMP_PARAM->fMeaningful = TRUE;
               HB_EXPR_USE( pSelf->value.asOperator.pRight, HB_EA_PUSH_POP );
               HB_COMP_PARAM->fMeaningful = fMeaningful;
            }
#endif
            HB_GEN_FUNC1( JumpHere, lEndPos );
         }
         else
         {
            HB_EXPR_USE( pSelf, HB_EA_PUSH_PCODE );
            HB_GEN_FUNC1( PCode1, HB_P_POP );
         }
         break;

      case HB_EA_STATEMENT:
         HB_COMP_ERROR_SYNTAX( pSelf );
         break;

      case HB_EA_DELETE:
         HB_EXPR_PCODE1( hb_compExprDelOperator, pSelf );
         break;
   }
   return pSelf;
}

static HB_EXPR_FUNC( hb_compExprUseAnd )
{
   switch( iMessage )
   {
      case HB_EA_REDUCE:
         pSelf->value.asOperator.pLeft  = HB_EXPR_USE( pSelf->value.asOperator.pLeft, HB_EA_REDUCE );
         pSelf->value.asOperator.pRight = HB_EXPR_USE( pSelf->value.asOperator.pRight, HB_EA_REDUCE );
         pSelf = hb_compExprReduceAnd( pSelf, HB_COMP_PARAM );
         break;

      case HB_EA_ARRAY_AT:
         HB_COMP_ERROR_TYPE( pSelf );
         break;

      case HB_EA_ARRAY_INDEX:
         hb_compErrorIndex( HB_COMP_PARAM, pSelf );
         break;

      case HB_EA_LVALUE:
         hb_compErrorLValue( HB_COMP_PARAM, pSelf );
         break;

      case HB_EA_PUSH_PCODE:
         if( HB_COMP_ISSUPPORTED( HB_COMPFLAG_SHORTCUTS ) )
         {
            LONG lEndPos;

            HB_EXPR_USE( pSelf->value.asOperator.pLeft, HB_EA_PUSH_PCODE );
            HB_GEN_FUNC1( PCode1, HB_P_DUPLICATE );
            lEndPos = HB_GEN_FUNC1( JumpFalse, 0 );
            HB_GEN_FUNC1( PCode1, HB_P_POP );
            HB_EXPR_USE( pSelf->value.asOperator.pRight, HB_EA_PUSH_PCODE );
            HB_GEN_FUNC1( JumpHere, lEndPos );
         }
         else
         {
            HB_EXPR_USE( pSelf->value.asOperator.pLeft, HB_EA_PUSH_PCODE );
            HB_EXPR_USE( pSelf->value.asOperator.pRight, HB_EA_PUSH_PCODE );
            HB_GEN_FUNC1( PCode1, HB_P_AND );
         }
         break;

      case HB_EA_POP_PCODE:
         break;

      case HB_EA_PUSH_POP:
         if( HB_COMP_ISSUPPORTED( HB_COMPFLAG_SHORTCUTS ) )
         {
            LONG lEndPos;
            HB_EXPR_USE( pSelf->value.asOperator.pLeft, HB_EA_PUSH_PCODE );
            lEndPos = HB_GEN_FUNC1( JumpFalse, 0 );
            /* NOTE: This will not generate a runtime error if incompatible
             * data type is used
             */
#if defined( HB_MACRO_SUPPORT )
            HB_EXPR_USE( pSelf->value.asOperator.pRight, HB_EA_PUSH_POP );
#else
            {
               BOOL fMeaningful = HB_COMP_PARAM->fMeaningful;
               /* do not generate warning about meaningless expression usage */
               HB_COMP_PARAM->fMeaningful = TRUE;
               HB_EXPR_USE( pSelf->value.asOperator.pRight, HB_EA_PUSH_POP );
               HB_COMP_PARAM->fMeaningful = fMeaningful;
            }
#endif
            HB_GEN_FUNC1( JumpHere, lEndPos );
         }
         else
         {
            HB_EXPR_USE( pSelf, HB_EA_PUSH_PCODE );
            HB_GEN_FUNC1( PCode1, HB_P_POP );
         }
         break;

      case HB_EA_STATEMENT:
         HB_COMP_ERROR_SYNTAX( pSelf );
         break;

      case HB_EA_DELETE:
         HB_EXPR_PCODE1( hb_compExprDelOperator, pSelf );
         break;
   }
   return pSelf;
}

static HB_EXPR_FUNC( hb_compExprUseNot )
{
   switch( iMessage )
   {
      case HB_EA_REDUCE:
      {
         HB_EXPR_PTR pExpr;

         pSelf->value.asOperator.pLeft = HB_EXPR_USE( pSelf->value.asOperator.pLeft, HB_EA_REDUCE );
         pExpr = pSelf->value.asOperator.pLeft;

         if( pExpr->ExprType == HB_ET_LOGICAL )
         {
            pExpr->value.asLogical = ! pExpr->value.asLogical;
            pSelf->ExprType = HB_ET_NONE;  /* do not delete operator parameter - we are still using it */
            HB_COMP_EXPR_DELETE( pSelf );
            pSelf = pExpr;
         }
         else if( pExpr->ExprType == HB_EO_NOT && HB_SUPPORT_EXTOPT )
         {
            /* NOTE: This will not generate a runtime error if incompatible
             * data type is used
             */
            pExpr->ExprType = HB_ET_NONE;  /* do not delete operator parameter - we are still using it */
            pExpr = pExpr->value.asOperator.pLeft;
            HB_COMP_EXPR_DELETE( pSelf );
            pSelf = pExpr;
         }
         break;
      }
      case HB_EA_ARRAY_AT:
         HB_COMP_ERROR_TYPE( pSelf );
         break;

      case HB_EA_ARRAY_INDEX:
         hb_compErrorIndex( HB_COMP_PARAM, pSelf );
         break;

      case HB_EA_LVALUE:
         hb_compErrorLValue( HB_COMP_PARAM, pSelf );
         break;

      case HB_EA_PUSH_PCODE:
         HB_EXPR_USE( pSelf->value.asOperator.pLeft, HB_EA_PUSH_PCODE );
         HB_GEN_FUNC1( PCode1, HB_P_NOT );
         break;

      case HB_EA_POP_PCODE:
         break;

      case HB_EA_PUSH_POP:
         if( HB_SUPPORT_HARBOUR )
         {
            /* NOTE: This will not generate a runtime error if incompatible
             * data type is used
             */
            HB_EXPR_USE( pSelf->value.asOperator.pLeft, HB_EA_PUSH_POP );
         }
         else
         {
            HB_EXPR_USE( pSelf, HB_EA_PUSH_PCODE );
            HB_GEN_FUNC1( PCode1, HB_P_POP );
         }
         break;

      case HB_EA_STATEMENT:
         HB_COMP_ERROR_SYNTAX( pSelf );
         break;

      case HB_EA_DELETE:
         HB_COMP_EXPR_DELETE( pSelf->value.asOperator.pLeft );
         break;
   }
   return pSelf;
}

/* handler for = operator
 */
static HB_EXPR_FUNC( hb_compExprUseEqual )
{
   switch( iMessage )
   {
      case HB_EA_REDUCE:
         pSelf->value.asOperator.pLeft  = HB_EXPR_USE( pSelf->value.asOperator.pLeft, HB_EA_REDUCE );
         pSelf->value.asOperator.pRight = HB_EXPR_USE( pSelf->value.asOperator.pRight, HB_EA_REDUCE );
         pSelf = hb_compExprReduceEQ( pSelf, HB_COMP_PARAM );
         break;

      case HB_EA_ARRAY_AT:
         HB_COMP_ERROR_TYPE( pSelf );
         break;

      case HB_EA_ARRAY_INDEX:
         break;

      case HB_EA_LVALUE:
         hb_compErrorLValue( HB_COMP_PARAM, pSelf );
         break;
      case HB_EA_PUSH_PCODE:
         HB_EXPR_USE( pSelf->value.asOperator.pLeft, HB_EA_PUSH_PCODE );
         HB_EXPR_USE( pSelf->value.asOperator.pRight, HB_EA_PUSH_PCODE );
         HB_GEN_FUNC1( PCode1, HB_P_EQUAL );
         break;

      case HB_EA_POP_PCODE:
         break;

      case HB_EA_PUSH_POP:
         if( HB_SUPPORT_HARBOUR )
         {
            /* NOTE: This will not generate a runtime error if incompatible
             * data type is used
             */
            HB_EXPR_USE( pSelf->value.asOperator.pLeft, HB_EA_PUSH_POP );
            HB_EXPR_USE( pSelf->value.asOperator.pRight, HB_EA_PUSH_POP );
         }
         else
         {
            HB_EXPR_USE( pSelf, HB_EA_PUSH_PCODE );
            HB_GEN_FUNC1( PCode1, HB_P_POP );
         }
         break;

      case HB_EA_STATEMENT:
         HB_COMP_ERROR_SYNTAX( pSelf );
         break;

      case HB_EA_DELETE:
         HB_EXPR_PCODE1( hb_compExprDelOperator, pSelf );
         break;
   }
   return pSelf;
}

/* handler for == operator
 */
static HB_EXPR_FUNC( hb_compExprUseEQ )
{
   switch( iMessage )
   {
      case HB_EA_REDUCE:
         pSelf->value.asOperator.pLeft  = HB_EXPR_USE( pSelf->value.asOperator.pLeft, HB_EA_REDUCE );
         pSelf->value.asOperator.pRight = HB_EXPR_USE( pSelf->value.asOperator.pRight, HB_EA_REDUCE );
         pSelf = hb_compExprReduceEQ( pSelf, HB_COMP_PARAM );
         break;

      case HB_EA_ARRAY_AT:
         HB_COMP_ERROR_TYPE( pSelf );
         break;

      case HB_EA_ARRAY_INDEX:
         break;

      case HB_EA_LVALUE:
         hb_compErrorLValue( HB_COMP_PARAM, pSelf );
         break;
      case HB_EA_PUSH_PCODE:
         HB_EXPR_USE( pSelf->value.asOperator.pLeft, HB_EA_PUSH_PCODE );
         HB_EXPR_USE( pSelf->value.asOperator.pRight, HB_EA_PUSH_PCODE );
         HB_GEN_FUNC1( PCode1, HB_P_EXACTLYEQUAL );
         break;

      case HB_EA_POP_PCODE:
         break;

      case HB_EA_PUSH_POP:
         if( HB_SUPPORT_HARBOUR )
         {
            /* NOTE: This will not generate a runtime error if incompatible
             * data type is used
             */
            HB_EXPR_USE( pSelf->value.asOperator.pLeft, HB_EA_PUSH_POP );
            HB_EXPR_USE( pSelf->value.asOperator.pRight, HB_EA_PUSH_POP );
         }
         else
         {
            HB_EXPR_USE( pSelf, HB_EA_PUSH_PCODE );
            HB_GEN_FUNC1( PCode1, HB_P_POP );
         }
         break;

      case HB_EA_STATEMENT:
         HB_COMP_ERROR_SYNTAX( pSelf );
         break;

      case HB_EA_DELETE:
         HB_EXPR_PCODE1( hb_compExprDelOperator, pSelf );
         break;
   }
   return pSelf;
}

static HB_EXPR_FUNC( hb_compExprUseLT )
{
   switch( iMessage )
   {
      case HB_EA_REDUCE:
         pSelf->value.asOperator.pLeft  = HB_EXPR_USE( pSelf->value.asOperator.pLeft, HB_EA_REDUCE );
         pSelf->value.asOperator.pRight = HB_EXPR_USE( pSelf->value.asOperator.pRight, HB_EA_REDUCE );
         pSelf = hb_compExprReduceLT( pSelf, HB_COMP_PARAM );
         break;

      case HB_EA_ARRAY_AT:
         HB_COMP_ERROR_TYPE( pSelf );
         break;

      case HB_EA_ARRAY_INDEX:
         break;

      case HB_EA_LVALUE:
         hb_compErrorLValue( HB_COMP_PARAM, pSelf );
         break;
      case HB_EA_PUSH_PCODE:
         HB_EXPR_USE( pSelf->value.asOperator.pLeft, HB_EA_PUSH_PCODE );
         HB_EXPR_USE( pSelf->value.asOperator.pRight, HB_EA_PUSH_PCODE );
         HB_GEN_FUNC1( PCode1, HB_P_LESS );
         break;

      case HB_EA_POP_PCODE:
         break;

      case HB_EA_PUSH_POP:
         if( HB_SUPPORT_HARBOUR )
         {
            /* NOTE: This will not generate a runtime error if incompatible
             * data type is used
             */
            HB_EXPR_USE( pSelf->value.asOperator.pLeft, HB_EA_PUSH_POP );
            HB_EXPR_USE( pSelf->value.asOperator.pRight, HB_EA_PUSH_POP );
         }
         else
         {
            HB_EXPR_USE( pSelf, HB_EA_PUSH_PCODE );
            HB_GEN_FUNC1( PCode1, HB_P_POP );
         }
         break;

      case HB_EA_STATEMENT:
         HB_COMP_ERROR_SYNTAX( pSelf );
         break;

      case HB_EA_DELETE:
         HB_EXPR_PCODE1( hb_compExprDelOperator, pSelf );
         break;
   }
   return pSelf;
}

static HB_EXPR_FUNC( hb_compExprUseGT )
{
   switch( iMessage )
   {
      case HB_EA_REDUCE:
         pSelf->value.asOperator.pLeft  = HB_EXPR_USE( pSelf->value.asOperator.pLeft, HB_EA_REDUCE );
         pSelf->value.asOperator.pRight = HB_EXPR_USE( pSelf->value.asOperator.pRight, HB_EA_REDUCE );
         pSelf = hb_compExprReduceGT( pSelf, HB_COMP_PARAM );
         break;

      case HB_EA_ARRAY_AT:
         HB_COMP_ERROR_TYPE( pSelf );
         break;

      case HB_EA_ARRAY_INDEX:
         break;

      case HB_EA_LVALUE:
         hb_compErrorLValue( HB_COMP_PARAM, pSelf );
         break;
      case HB_EA_PUSH_PCODE:
         HB_EXPR_USE( pSelf->value.asOperator.pLeft, HB_EA_PUSH_PCODE );
         HB_EXPR_USE( pSelf->value.asOperator.pRight, HB_EA_PUSH_PCODE );
         HB_GEN_FUNC1( PCode1, HB_P_GREATER );
         break;

      case HB_EA_POP_PCODE:
         break;

      case HB_EA_PUSH_POP:
         if( HB_SUPPORT_HARBOUR )
         {
            /* NOTE: This will not generate a runtime error if incompatible
             * data type is used
             */
            HB_EXPR_USE( pSelf->value.asOperator.pLeft, HB_EA_PUSH_POP );
            HB_EXPR_USE( pSelf->value.asOperator.pRight, HB_EA_PUSH_POP );
         }
         else
         {
            HB_EXPR_USE( pSelf, HB_EA_PUSH_PCODE );
            HB_GEN_FUNC1( PCode1, HB_P_POP );
         }
         break;

      case HB_EA_STATEMENT:
         HB_COMP_ERROR_SYNTAX( pSelf );
         break;

      case HB_EA_DELETE:
         HB_EXPR_PCODE1( hb_compExprDelOperator, pSelf );
         break;
   }
   return pSelf;
}

static HB_EXPR_FUNC( hb_compExprUseLE )
{
   switch( iMessage )
   {
      case HB_EA_REDUCE:
         pSelf->value.asOperator.pLeft  = HB_EXPR_USE( pSelf->value.asOperator.pLeft, HB_EA_REDUCE );
         pSelf->value.asOperator.pRight = HB_EXPR_USE( pSelf->value.asOperator.pRight, HB_EA_REDUCE );
         pSelf = hb_compExprReduceLE( pSelf, HB_COMP_PARAM );
         break;

      case HB_EA_ARRAY_AT:
         HB_COMP_ERROR_TYPE( pSelf );
         break;

      case HB_EA_ARRAY_INDEX:
         break;

      case HB_EA_LVALUE:
         hb_compErrorLValue( HB_COMP_PARAM, pSelf );
         break;

      case HB_EA_PUSH_PCODE:
         HB_EXPR_USE( pSelf->value.asOperator.pLeft, HB_EA_PUSH_PCODE );
         HB_EXPR_USE( pSelf->value.asOperator.pRight, HB_EA_PUSH_PCODE );
         HB_GEN_FUNC1( PCode1, HB_P_LESSEQUAL );
         break;

      case HB_EA_POP_PCODE:
         break;

      case HB_EA_PUSH_POP:
         if( HB_SUPPORT_HARBOUR )
         {
            /* NOTE: This will not generate a runtime error if incompatible
             * data type is used
             */
            HB_EXPR_USE( pSelf->value.asOperator.pLeft, HB_EA_PUSH_POP );
            HB_EXPR_USE( pSelf->value.asOperator.pRight, HB_EA_PUSH_POP );
         }
         else
         {
            HB_EXPR_USE( pSelf, HB_EA_PUSH_PCODE );
            HB_GEN_FUNC1( PCode1, HB_P_POP );
         }
         break;

      case HB_EA_STATEMENT:
         HB_COMP_ERROR_SYNTAX( pSelf );
         break;

      case HB_EA_DELETE:
         HB_EXPR_PCODE1( hb_compExprDelOperator, pSelf );
         break;
   }
   return pSelf;
}

static HB_EXPR_FUNC( hb_compExprUseGE )
{
   switch( iMessage )
   {
      case HB_EA_REDUCE:
         pSelf->value.asOperator.pLeft  = HB_EXPR_USE( pSelf->value.asOperator.pLeft, HB_EA_REDUCE );
         pSelf->value.asOperator.pRight = HB_EXPR_USE( pSelf->value.asOperator.pRight, HB_EA_REDUCE );
         pSelf = hb_compExprReduceGE( pSelf, HB_COMP_PARAM );
         break;

      case HB_EA_ARRAY_AT:
         HB_COMP_ERROR_TYPE( pSelf );
         break;

      case HB_EA_ARRAY_INDEX:
         break;

      case HB_EA_LVALUE:
         hb_compErrorLValue( HB_COMP_PARAM, pSelf );
         break;

      case HB_EA_PUSH_PCODE:
         HB_EXPR_USE( pSelf->value.asOperator.pLeft, HB_EA_PUSH_PCODE );
         HB_EXPR_USE( pSelf->value.asOperator.pRight, HB_EA_PUSH_PCODE );
         HB_GEN_FUNC1( PCode1, HB_P_GREATEREQUAL );
         break;

      case HB_EA_POP_PCODE:
         break;

      case HB_EA_PUSH_POP:
         if( HB_SUPPORT_HARBOUR )
         {
            /* NOTE: This will not generate a runtime error if incompatible
             * data type is used
             */
            HB_EXPR_USE( pSelf->value.asOperator.pLeft, HB_EA_PUSH_POP );
            HB_EXPR_USE( pSelf->value.asOperator.pRight, HB_EA_PUSH_POP );
         }
         else
         {
            HB_EXPR_USE( pSelf, HB_EA_PUSH_PCODE );
            HB_GEN_FUNC1( PCode1, HB_P_POP );
         }
         break;

      case HB_EA_STATEMENT:
         HB_COMP_ERROR_SYNTAX( pSelf );
         break;

      case HB_EA_DELETE:
         HB_EXPR_PCODE1( hb_compExprDelOperator, pSelf );
         break;
   }
   return pSelf;
}

static HB_EXPR_FUNC( hb_compExprUseNE )
{
   switch( iMessage )
   {
      case HB_EA_REDUCE:
         pSelf->value.asOperator.pLeft  = HB_EXPR_USE( pSelf->value.asOperator.pLeft, HB_EA_REDUCE );
         pSelf->value.asOperator.pRight = HB_EXPR_USE( pSelf->value.asOperator.pRight, HB_EA_REDUCE );
         pSelf = hb_compExprReduceNE( pSelf, HB_COMP_PARAM );
         break;

      case HB_EA_ARRAY_AT:
         HB_COMP_ERROR_TYPE( pSelf );
         break;

      case HB_EA_ARRAY_INDEX:
         break;

      case HB_EA_LVALUE:
         hb_compErrorLValue( HB_COMP_PARAM, pSelf );
         break;

      case HB_EA_PUSH_PCODE:
         HB_EXPR_USE( pSelf->value.asOperator.pLeft, HB_EA_PUSH_PCODE );
         HB_EXPR_USE( pSelf->value.asOperator.pRight, HB_EA_PUSH_PCODE );
         HB_GEN_FUNC1( PCode1, HB_P_NOTEQUAL );
         break;

      case HB_EA_POP_PCODE:
         break;

      case HB_EA_PUSH_POP:
         if( HB_SUPPORT_HARBOUR )
         {
            /* NOTE: This will not generate a runtime error if incompatible
             * data type is used
             */
            HB_EXPR_USE( pSelf->value.asOperator.pLeft, HB_EA_PUSH_POP );
            HB_EXPR_USE( pSelf->value.asOperator.pRight, HB_EA_PUSH_POP );
         }
         else
         {
            HB_EXPR_USE( pSelf, HB_EA_PUSH_PCODE );
            HB_GEN_FUNC1( PCode1, HB_P_POP );
         }
         break;

      case HB_EA_STATEMENT:
         HB_COMP_ERROR_SYNTAX( pSelf );
         break;

      case HB_EA_DELETE:
         HB_EXPR_PCODE1( hb_compExprDelOperator, pSelf );
         break;
   }
   return pSelf;
}

static HB_EXPR_FUNC( hb_compExprUseIN )
{
   switch( iMessage )
   {
      case HB_EA_REDUCE:
         pSelf->value.asOperator.pLeft  = HB_EXPR_USE( pSelf->value.asOperator.pLeft, HB_EA_REDUCE );
         pSelf->value.asOperator.pRight = HB_EXPR_USE( pSelf->value.asOperator.pRight, HB_EA_REDUCE );
         pSelf = hb_compExprReduceIN( pSelf, HB_COMP_PARAM );
         break;

      case HB_EA_ARRAY_AT:
         HB_COMP_ERROR_TYPE( pSelf );
         break;

      case HB_EA_ARRAY_INDEX:
         break;

      case HB_EA_LVALUE:
         hb_compErrorLValue( HB_COMP_PARAM, pSelf );
         break;

      case HB_EA_PUSH_PCODE:
         HB_EXPR_USE( pSelf->value.asOperator.pLeft, HB_EA_PUSH_PCODE );
         HB_EXPR_USE( pSelf->value.asOperator.pRight, HB_EA_PUSH_PCODE );
         HB_GEN_FUNC1( PCode1, HB_P_INSTRING );
         break;

      case HB_EA_POP_PCODE:
         break;

      case HB_EA_PUSH_POP:
         if( HB_SUPPORT_HARBOUR )
         {
            /* NOTE: This will not generate a runtime error if incompatible
             * data type is used
             */
            HB_EXPR_USE( pSelf->value.asOperator.pLeft, HB_EA_PUSH_POP );
            HB_EXPR_USE( pSelf->value.asOperator.pRight, HB_EA_PUSH_POP );
         }
         else
         {
            HB_EXPR_USE( pSelf, HB_EA_PUSH_PCODE );
            HB_GEN_FUNC1( PCode1, HB_P_POP );
         }
         break;

      case HB_EA_STATEMENT:
         HB_COMP_ERROR_SYNTAX( pSelf );
         break;

      case HB_EA_DELETE:
         HB_EXPR_PCODE1( hb_compExprDelOperator, pSelf );
         break;
   }
   return pSelf;
}

static HB_EXPR_FUNC( hb_compExprUsePlus )
{
   switch( iMessage )
   {
      case HB_EA_REDUCE:
         pSelf->value.asOperator.pLeft  = HB_EXPR_USE( pSelf->value.asOperator.pLeft, HB_EA_REDUCE );
         pSelf->value.asOperator.pRight = HB_EXPR_USE( pSelf->value.asOperator.pRight, HB_EA_REDUCE );
         pSelf = hb_compExprReducePlus( pSelf, HB_COMP_PARAM );
         break;

      case HB_EA_ARRAY_AT:
         HB_COMP_ERROR_TYPE( pSelf );
         break;

      case HB_EA_ARRAY_INDEX:
         break;

      case HB_EA_LVALUE:
         hb_compErrorLValue( HB_COMP_PARAM, pSelf );
         break;

      case HB_EA_PUSH_PCODE:
         if( HB_SUPPORT_EXTOPT )
         {
            HB_EXPR_PTR pLeft, pRight;
            pLeft  = pSelf->value.asOperator.pLeft;
            pRight = pSelf->value.asOperator.pRight;
            if( pLeft->ExprType == HB_ET_NUMERIC )
            {
               if( pLeft->value.asNum.NumType == HB_ET_LONG ?
                   pLeft->value.asNum.val.l == 1 :
                   pLeft->value.asNum.val.d == 1 )
               {
                  HB_EXPR_USE( pRight, HB_EA_PUSH_PCODE );
                  HB_GEN_FUNC1( PCode1, HB_P_INC );
                  break;
               }
               else if( pLeft->value.asNum.NumType == HB_ET_LONG ?
                   pLeft->value.asNum.val.l == -1 :
                   pLeft->value.asNum.val.d == -1 )
               {
                  HB_EXPR_USE( pRight, HB_EA_PUSH_PCODE );
                  HB_GEN_FUNC1( PCode1, HB_P_DEC );
                  break;
               }
            }
            else if( pRight->ExprType == HB_ET_NUMERIC )
            {
               if( pRight->value.asNum.NumType == HB_ET_LONG ?
                   pRight->value.asNum.val.l == 1 :
                   pRight->value.asNum.val.d == 1 )
               {
                  HB_EXPR_USE( pLeft, HB_EA_PUSH_PCODE );
                  HB_GEN_FUNC1( PCode1, HB_P_INC );
                  break;
               }
               else if( pRight->value.asNum.NumType == HB_ET_LONG ?
                   pRight->value.asNum.val.l == -1 :
                   pRight->value.asNum.val.d == -1 )
               {
                  HB_EXPR_USE( pLeft, HB_EA_PUSH_PCODE );
                  HB_GEN_FUNC1( PCode1, HB_P_DEC );
                  break;
               }
            }
         }
         HB_EXPR_USE( pSelf->value.asOperator.pLeft,  HB_EA_PUSH_PCODE );
         HB_EXPR_USE( pSelf->value.asOperator.pRight, HB_EA_PUSH_PCODE );
         HB_GEN_FUNC1( PCode1, HB_P_PLUS );
         break;

      case HB_EA_POP_PCODE:
         break;

      case HB_EA_PUSH_POP:
         if( HB_SUPPORT_HARBOUR )
         {
            /* NOTE: This will not generate a runtime error if incompatible
             * data type is used
             */
            HB_EXPR_USE( pSelf->value.asOperator.pLeft, HB_EA_PUSH_POP );
            HB_EXPR_USE( pSelf->value.asOperator.pRight, HB_EA_PUSH_POP );
         }
         else
         {
            HB_EXPR_USE( pSelf, HB_EA_PUSH_PCODE );
            HB_GEN_FUNC1( PCode1, HB_P_POP );
         }
         break;

      case HB_EA_STATEMENT:
         HB_COMP_ERROR_SYNTAX( pSelf );
         break;

      case HB_EA_DELETE:
         HB_EXPR_PCODE1( hb_compExprDelOperator, pSelf );
         break;
   }
   return pSelf;
}

static HB_EXPR_FUNC( hb_compExprUseMinus )
{
   switch( iMessage )
   {
      case HB_EA_REDUCE:
         pSelf->value.asOperator.pLeft  = HB_EXPR_USE( pSelf->value.asOperator.pLeft, HB_EA_REDUCE );
         pSelf->value.asOperator.pRight = HB_EXPR_USE( pSelf->value.asOperator.pRight, HB_EA_REDUCE );
         pSelf = hb_compExprReduceMinus( pSelf, HB_COMP_PARAM );
         break;

      case HB_EA_ARRAY_AT:
         HB_COMP_ERROR_TYPE( pSelf );
         break;

      case HB_EA_ARRAY_INDEX:
         break;

      case HB_EA_LVALUE:
         hb_compErrorLValue( HB_COMP_PARAM, pSelf );
         break;

      case HB_EA_PUSH_PCODE:
         if( HB_SUPPORT_EXTOPT )
         {
            HB_EXPR_PTR pRight = pSelf->value.asOperator.pRight;
            if( pRight->ExprType == HB_ET_NUMERIC )
            {
               if( pRight->value.asNum.NumType == HB_ET_LONG ?
                   pRight->value.asNum.val.l == 1 :
                   pRight->value.asNum.val.d == 1 )
               {
                  HB_EXPR_USE( pSelf->value.asOperator.pLeft, HB_EA_PUSH_PCODE );
                  HB_GEN_FUNC1( PCode1, HB_P_DEC );
                  break;
               }
               else if( pRight->value.asNum.NumType == HB_ET_LONG ?
                   pRight->value.asNum.val.l == -1 :
                   pRight->value.asNum.val.d == -1 )
               {
                  HB_EXPR_USE( pSelf->value.asOperator.pLeft, HB_EA_PUSH_PCODE );
                  HB_GEN_FUNC1( PCode1, HB_P_INC );
                  break;
               }
            }
         }
         HB_EXPR_USE( pSelf->value.asOperator.pLeft,  HB_EA_PUSH_PCODE );
         HB_EXPR_USE( pSelf->value.asOperator.pRight, HB_EA_PUSH_PCODE );
         HB_GEN_FUNC1( PCode1, HB_P_MINUS );
         break;

      case HB_EA_POP_PCODE:
         break;

      case HB_EA_PUSH_POP:
         if( HB_SUPPORT_HARBOUR )
         {
            /* NOTE: This will not generate a runtime error if incompatible
             * data type is used
             */
            HB_EXPR_USE( pSelf->value.asOperator.pLeft, HB_EA_PUSH_POP );
            HB_EXPR_USE( pSelf->value.asOperator.pRight, HB_EA_PUSH_POP );
         }
         else
         {
            HB_EXPR_USE( pSelf, HB_EA_PUSH_PCODE );
            HB_GEN_FUNC1( PCode1, HB_P_POP );
         }
         break;

      case HB_EA_STATEMENT:
         HB_COMP_ERROR_SYNTAX( pSelf );
         break;

      case HB_EA_DELETE:
         HB_EXPR_PCODE1( hb_compExprDelOperator, pSelf );
         break;
   }
   return pSelf;
}

static HB_EXPR_FUNC( hb_compExprUseMult )
{
   switch( iMessage )
   {
      case HB_EA_REDUCE:
         pSelf->value.asOperator.pLeft  = HB_EXPR_USE( pSelf->value.asOperator.pLeft, HB_EA_REDUCE );
         pSelf->value.asOperator.pRight = HB_EXPR_USE( pSelf->value.asOperator.pRight, HB_EA_REDUCE );
         pSelf = hb_compExprReduceMult( pSelf, HB_COMP_PARAM );
         break;

      case HB_EA_ARRAY_AT:
         HB_COMP_ERROR_TYPE( pSelf );
         break;

      case HB_EA_ARRAY_INDEX:
         break;

      case HB_EA_LVALUE:
         hb_compErrorLValue( HB_COMP_PARAM, pSelf );
         break;

      case HB_EA_PUSH_PCODE:
         HB_EXPR_USE( pSelf->value.asOperator.pLeft,  HB_EA_PUSH_PCODE );
         HB_EXPR_USE( pSelf->value.asOperator.pRight, HB_EA_PUSH_PCODE );
         HB_GEN_FUNC1( PCode1, HB_P_MULT );
         break;

      case HB_EA_POP_PCODE:
         break;

      case HB_EA_PUSH_POP:
         if( HB_SUPPORT_HARBOUR )
         {
            /* NOTE: This will not generate a runtime error if incompatible
             * data type is used
             */
            HB_EXPR_USE( pSelf->value.asOperator.pLeft, HB_EA_PUSH_POP );
            HB_EXPR_USE( pSelf->value.asOperator.pRight, HB_EA_PUSH_POP );
         }
         else
         {
            HB_EXPR_USE( pSelf, HB_EA_PUSH_PCODE );
            HB_GEN_FUNC1( PCode1, HB_P_POP );
         }
         break;

      case HB_EA_STATEMENT:
         HB_COMP_ERROR_SYNTAX( pSelf );
         break;

      case HB_EA_DELETE:
         HB_EXPR_PCODE1( hb_compExprDelOperator, pSelf );
         break;
   }
   return pSelf;
}

static HB_EXPR_FUNC( hb_compExprUseDiv )
{
   switch( iMessage )
   {
      case HB_EA_REDUCE:
         pSelf->value.asOperator.pLeft  = HB_EXPR_USE( pSelf->value.asOperator.pLeft, HB_EA_REDUCE );
         pSelf->value.asOperator.pRight = HB_EXPR_USE( pSelf->value.asOperator.pRight, HB_EA_REDUCE );
         pSelf = hb_compExprReduceDiv( pSelf, HB_COMP_PARAM );
         break;

      case HB_EA_ARRAY_AT:
         HB_COMP_ERROR_TYPE( pSelf );
         break;

      case HB_EA_ARRAY_INDEX:
         break;

      case HB_EA_LVALUE:
         hb_compErrorLValue( HB_COMP_PARAM, pSelf );
         break;

      case HB_EA_PUSH_PCODE:
         HB_EXPR_USE( pSelf->value.asOperator.pLeft,  HB_EA_PUSH_PCODE );
         HB_EXPR_USE( pSelf->value.asOperator.pRight, HB_EA_PUSH_PCODE );
         HB_GEN_FUNC1( PCode1, HB_P_DIVIDE );
         break;

      case HB_EA_POP_PCODE:
         break;

      case HB_EA_PUSH_POP:
         if( HB_SUPPORT_HARBOUR )
         {
            /* NOTE: This will not generate a runtime error if incompatible
             * data type is used
             */
            HB_EXPR_USE( pSelf->value.asOperator.pLeft, HB_EA_PUSH_POP );
            HB_EXPR_USE( pSelf->value.asOperator.pRight, HB_EA_PUSH_POP );
         }
         else
         {
            HB_EXPR_USE( pSelf, HB_EA_PUSH_PCODE );
            HB_GEN_FUNC1( PCode1, HB_P_POP );
         }
         break;

      case HB_EA_STATEMENT:
         HB_COMP_ERROR_SYNTAX( pSelf );
         break;

      case HB_EA_DELETE:
         HB_EXPR_PCODE1( hb_compExprDelOperator, pSelf );
         break;
   }
   return pSelf;
}

static HB_EXPR_FUNC( hb_compExprUseMod )
{
   switch( iMessage )
   {
      case HB_EA_REDUCE:
         pSelf->value.asOperator.pLeft  = HB_EXPR_USE( pSelf->value.asOperator.pLeft, HB_EA_REDUCE );
         pSelf->value.asOperator.pRight = HB_EXPR_USE( pSelf->value.asOperator.pRight, HB_EA_REDUCE );
         pSelf =hb_compExprReduceMod( pSelf, HB_COMP_PARAM );
         break;

      case HB_EA_ARRAY_AT:
         HB_COMP_ERROR_TYPE( pSelf );
         break;

      case HB_EA_ARRAY_INDEX:
         break;

      case HB_EA_LVALUE:
         hb_compErrorLValue( HB_COMP_PARAM, pSelf );
         break;

      case HB_EA_PUSH_PCODE:
         HB_EXPR_USE( pSelf->value.asOperator.pLeft,  HB_EA_PUSH_PCODE );
         HB_EXPR_USE( pSelf->value.asOperator.pRight, HB_EA_PUSH_PCODE );
         HB_GEN_FUNC1( PCode1, HB_P_MODULUS );
         break;

      case HB_EA_POP_PCODE:
         break;

      case HB_EA_PUSH_POP:
         if( HB_SUPPORT_HARBOUR )
         {
            /* NOTE: This will not generate a runtime error if incompatible
             * data type is used
             */
            HB_EXPR_USE( pSelf->value.asOperator.pLeft, HB_EA_PUSH_POP );
            HB_EXPR_USE( pSelf->value.asOperator.pRight, HB_EA_PUSH_POP );
         }
         else
         {
            HB_EXPR_USE( pSelf, HB_EA_PUSH_PCODE );
            HB_GEN_FUNC1( PCode1, HB_P_POP );
         }
         break;

      case HB_EA_STATEMENT:
         HB_COMP_ERROR_SYNTAX( pSelf );
         break;

      case HB_EA_DELETE:
         HB_EXPR_PCODE1( hb_compExprDelOperator, pSelf );
         break;
   }
   return pSelf;
}

static HB_EXPR_FUNC( hb_compExprUsePower )
{
   switch( iMessage )
   {
      case HB_EA_REDUCE:
         pSelf->value.asOperator.pLeft  = HB_EXPR_USE( pSelf->value.asOperator.pLeft, HB_EA_REDUCE );
         pSelf->value.asOperator.pRight = HB_EXPR_USE( pSelf->value.asOperator.pRight, HB_EA_REDUCE );
         if( HB_SUPPORT_HARBOUR )   /* Clipper doesn't optimize it */
            pSelf =hb_compExprReducePower( pSelf, HB_COMP_PARAM );
         break;

      case HB_EA_ARRAY_AT:
         HB_COMP_ERROR_TYPE( pSelf );
         break;

      case HB_EA_ARRAY_INDEX:
         break;

      case HB_EA_LVALUE:
         hb_compErrorLValue( HB_COMP_PARAM, pSelf );
         break;

      case HB_EA_PUSH_PCODE:
         HB_EXPR_USE( pSelf->value.asOperator.pLeft,  HB_EA_PUSH_PCODE );
         HB_EXPR_USE( pSelf->value.asOperator.pRight, HB_EA_PUSH_PCODE );
         HB_GEN_FUNC1( PCode1, HB_P_POWER );
         break;

      case HB_EA_POP_PCODE:
         break;

      case HB_EA_PUSH_POP:
         if( HB_SUPPORT_HARBOUR )
         {
            /* NOTE: This will not generate a runtime error if incompatible
             * data type is used
             */
            HB_EXPR_USE( pSelf->value.asOperator.pLeft, HB_EA_PUSH_POP );
            HB_EXPR_USE( pSelf->value.asOperator.pRight, HB_EA_PUSH_POP );
         }
         else
         {
            HB_EXPR_USE( pSelf, HB_EA_PUSH_PCODE );
            HB_GEN_FUNC1( PCode1, HB_P_POP );
         }
         break;

      case HB_EA_STATEMENT:
         HB_COMP_ERROR_SYNTAX( pSelf );
         break;

      case HB_EA_DELETE:
         HB_EXPR_PCODE1( hb_compExprDelOperator, pSelf );
         break;
   }
   return pSelf;
}

static HB_EXPR_FUNC( hb_compExprUseNegate )
{
   switch( iMessage )
   {
      case HB_EA_REDUCE:
         pSelf->value.asOperator.pLeft = HB_EXPR_USE( pSelf->value.asOperator.pLeft, HB_EA_REDUCE );
         pSelf = hb_compExprReduceNegate( pSelf, HB_COMP_PARAM );
         break;

      case HB_EA_ARRAY_AT:
         HB_COMP_ERROR_TYPE( pSelf );
         break;

      case HB_EA_ARRAY_INDEX:
         break;

      case HB_EA_LVALUE:
         hb_compErrorLValue( HB_COMP_PARAM, pSelf );
         break;

      case HB_EA_PUSH_PCODE:
         HB_EXPR_USE( pSelf->value.asOperator.pLeft,  HB_EA_PUSH_PCODE );
         HB_GEN_FUNC1( PCode1, HB_P_NEGATE );
         break;

      case HB_EA_POP_PCODE:
         break;

      case HB_EA_PUSH_POP:
         if( HB_SUPPORT_HARBOUR )
         {
            /* NOTE: This will not generate a runtime error if incompatible
             * data type is used
             */
            HB_EXPR_USE( pSelf->value.asOperator.pLeft, HB_EA_PUSH_POP );
         }
         else
         {
            HB_EXPR_USE( pSelf, HB_EA_PUSH_PCODE );
            HB_GEN_FUNC1( PCode1, HB_P_POP );
         }
         break;

      case HB_EA_STATEMENT:
         HB_COMP_ERROR_SYNTAX( pSelf );
         break;

      case HB_EA_DELETE:
         if( pSelf->value.asOperator.pLeft )
            HB_COMP_EXPR_DELETE( pSelf->value.asOperator.pLeft );
         break;
   }
   return pSelf;
}

static HB_EXPR_FUNC( hb_compExprUsePreInc )
{
   switch( iMessage )
   {
      case HB_EA_REDUCE:
         pSelf->value.asOperator.pLeft = HB_EXPR_USE( pSelf->value.asOperator.pLeft, HB_EA_REDUCE );
         HB_EXPR_USE( pSelf->value.asOperator.pLeft, HB_EA_LVALUE );
         break;

      case HB_EA_ARRAY_AT:
         HB_COMP_ERROR_TYPE( pSelf );
         break;

      case HB_EA_ARRAY_INDEX:
         break;

      case HB_EA_LVALUE:
         hb_compErrorLValue( HB_COMP_PARAM, pSelf );
         break;

      case HB_EA_PUSH_PCODE:
         HB_EXPR_PCODE2( hb_compExprPushPreOp, pSelf, HB_P_INC );
         break;

      case HB_EA_POP_PCODE:
         break;

      case HB_EA_PUSH_POP:
      case HB_EA_STATEMENT:
         HB_EXPR_PCODE2( hb_compExprUsePreOp, pSelf, HB_P_INC );
         break;

      case HB_EA_DELETE:
         if( pSelf->value.asOperator.pLeft )
            HB_COMP_EXPR_DELETE( pSelf->value.asOperator.pLeft );
         break;
   }
   return pSelf;
}

static HB_EXPR_FUNC( hb_compExprUsePreDec )
{
   switch( iMessage )
   {
      case HB_EA_REDUCE:
         pSelf->value.asOperator.pLeft = HB_EXPR_USE( pSelf->value.asOperator.pLeft, HB_EA_REDUCE );
         HB_EXPR_USE( pSelf->value.asOperator.pLeft, HB_EA_LVALUE );
         break;

      case HB_EA_ARRAY_AT:
         HB_COMP_ERROR_TYPE( pSelf );
         break;

      case HB_EA_ARRAY_INDEX:
         break;

      case HB_EA_LVALUE:
         hb_compErrorLValue( HB_COMP_PARAM, pSelf );
         break;

      case HB_EA_PUSH_PCODE:
         HB_EXPR_PCODE2( hb_compExprPushPreOp, pSelf, HB_P_DEC );
         break;

      case HB_EA_POP_PCODE:
         break;

      case HB_EA_PUSH_POP:
      case HB_EA_STATEMENT:
         HB_EXPR_PCODE2( hb_compExprUsePreOp, pSelf, HB_P_DEC );
         break;

      case HB_EA_DELETE:
         if( pSelf->value.asOperator.pLeft )
            HB_COMP_EXPR_DELETE( pSelf->value.asOperator.pLeft );
         break;
   }
   return pSelf;
}

/* ************************************************************************* */

/* This generates a push pcode for a codeblock (with no macro expression or
   with late evaluation of a macro)
*/
#if defined( HB_MACRO_SUPPORT )
static void hb_compExprCodeblockPush( HB_EXPR_PTR pSelf, HB_COMP_DECL )
#else
static void hb_compExprCodeblockPush( HB_EXPR_PTR pSelf, BOOL bLateEval, HB_COMP_DECL )
#endif
{
   HB_EXPR_PTR pExpr, pNext;
   HB_EXPR_PTR * pPrev;

   /* Define requested local variables
    */
#if defined( HB_MACRO_SUPPORT )
   hb_macroCodeBlockStart( HB_COMP_PARAM );
   HB_PCODE_DATA->pLocals = pSelf->value.asCodeblock.pLocals;
   HB_PCODE_DATA->fVParams =
                  ( pSelf->value.asCodeblock.flags & HB_BLOCK_VPARAMS ) != 0;
#else
   hb_compCodeBlockStart( HB_COMP_PARAM, bLateEval );
   HB_COMP_PARAM->functions.pLast->fVParams =
                  ( pSelf->value.asCodeblock.flags & HB_BLOCK_VPARAMS ) != 0;

   {
      HB_CBVAR_PTR pVar;

      HB_COMP_PARAM->iVarScope = VS_PARAMETER;
      pVar = pSelf->value.asCodeblock.pLocals;
      while( pVar )
      {
         hb_compVariableAdd( HB_COMP_PARAM, pVar->szName, hb_compVarTypeNew( HB_COMP_PARAM, pVar->bType, NULL ) );
         pVar =pVar->pNext;
      }
   }

   HB_EXPR_PCODE0( hb_compLinePushIfDebugger );
#endif
   pExpr = pSelf->value.asCodeblock.pExprList;
   pPrev = &pSelf->value.asCodeblock.pExprList;
   while( pExpr )
   {
      if( pExpr->ExprType == HB_ET_MACRO &&
          pExpr->value.asMacro.SubType != HB_ET_MACRO_SYMBOL &&
          pExpr->value.asMacro.SubType != HB_ET_MACRO_REFER &&
          pExpr->value.asMacro.SubType != HB_ET_MACRO_ALIASED )
      {
         /* Clipper allows for list expressions in a codeblock
          * macro := "1,2"
          * EVAL( {|| &macro} )
          */
         pExpr->value.asMacro.SubType |= HB_ET_MACRO_PARE;
      }

      /* store next expression in case the current  will be reduced
       * NOTE: During reduction the expression can be replaced by the
       *    new one - this will break the linked list of expressions.
       */
      pNext = pExpr->pNext; /* store next expression in case the current  will be reduced */
      pExpr = HB_EXPR_USE( pExpr, HB_EA_REDUCE );
      /* Generate push/pop pcodes for all expresions except the last one
       * The value of the last expression is used as a return value
       * of a codeblock evaluation
       */
      /* NOTE: This will genereate warnings if constant value is
       * used as an expression - some operators will generate it too
       * e.g.
       * EVAL( {|| 3+5, func()} )
       */
      *pPrev = pExpr;   /* store a new expression into the previous one */
      pExpr->pNext = pNext;  /* restore the link to next expression */
#if defined( HB_MACRO_SUPPORT )
      if( pNext )
         HB_EXPR_USE( pExpr, HB_EA_PUSH_POP );
      else
         HB_EXPR_USE( pExpr, HB_EA_PUSH_PCODE );
#else
      if( pNext && bLateEval )
         HB_EXPR_USE( pExpr, HB_EA_PUSH_POP );
      else
         HB_EXPR_USE( pExpr, HB_EA_PUSH_PCODE );
#endif
      pPrev  = &pExpr->pNext;
      pExpr  = pNext;
   }
#if defined( HB_MACRO_SUPPORT )
   hb_macroCodeBlockEnd( HB_COMP_PARAM );
#else
   if( bLateEval )
      hb_compCodeBlockEnd( HB_COMP_PARAM );
   else
      hb_compCodeBlockRewind( HB_COMP_PARAM );
#endif
}

/* This generates a push pcode for early evaluation of a macro
*/
#if !defined( HB_MACRO_SUPPORT )
static void hb_compExprCodeblockExtPush( HB_EXPR_PTR pSelf, HB_COMP_DECL )
{
   hb_compGenPCodeN( ( BYTE * ) pSelf->value.asCodeblock.string,
                     pSelf->ulLength, HB_COMP_PARAM );
}

static void hb_compExprCodeblockEarly( HB_EXPR_PTR pSelf, HB_COMP_DECL )
{
   HB_EXPR_PTR pExpr;

   /* check first expression */
   pExpr = pSelf->value.asCodeblock.pExprList;
   if( pExpr->ExprType == HB_ET_MACRO && pExpr->value.asMacro.cMacroOp )
   {
      /* simple macro variable expansion: &variable
       * 'szMacro' is a variable name
       * {|| &variable} => &( '{||' + variable +'}' )
       */
      HB_EXPR_PTR pVar, pNew;

      pVar = hb_compExprNewVar( pExpr->value.asMacro.szMacro, HB_COMP_PARAM );
      pNew = hb_compExprNewString( "{||", 3, FALSE, HB_COMP_PARAM );
      pNew = hb_compExprSetOperand( hb_compExprNewPlus( pNew, HB_COMP_PARAM ), pVar, HB_COMP_PARAM );
      pNew = hb_compExprSetOperand( hb_compExprNewPlus( pNew, HB_COMP_PARAM ), hb_compExprNewString( "}", 1, FALSE, HB_COMP_PARAM ), HB_COMP_PARAM );
      pNew = hb_compExprNewMacro( pNew, 0, NULL, HB_COMP_PARAM );
      HB_EXPR_USE( pNew, HB_EA_PUSH_PCODE );
      HB_COMP_EXPR_DELETE( pNew );
   }
   else
   {
      /* everything else is macro compiled at runtime
       * {|| &variable+1} => &( '{|| &variable+1}' )
       */
      HB_EXPR_PTR pNew;

      hb_compExprCodeblockPush( pSelf, FALSE, HB_COMP_PARAM );
      pNew = hb_compExprNewMacro( hb_compExprNewString( pSelf->value.asCodeblock.string, pSelf->ulLength, FALSE, HB_COMP_PARAM ), 0, NULL, HB_COMP_PARAM );
      HB_EXPR_USE( pNew, HB_EA_PUSH_PCODE );
      HB_COMP_EXPR_DELETE( pNew );
      hb_compCodeBlockStop( HB_COMP_PARAM );
   }
}
#endif      /*HB_MACRO_SUPPORT*/


static void hb_compExprPushSendPop( HB_EXPR_PTR pSelf, HB_COMP_DECL )
{
   if( pSelf->value.asMessage.pObject )
   {
      /* Push _message */
      if( pSelf->value.asMessage.szMessage )
      {
         HB_GEN_FUNC2( MessageData, pSelf->value.asMessage.szMessage, TRUE );
      }
      else
      {
         if( pSelf->value.asMessage.pMessage->ExprType == HB_ET_MACRO )
            /* o:&macro := value
             * set ASSIGN flag in macro expression
             * it's cleared just after use
             */
            pSelf->value.asMessage.pMessage->value.asMacro.SubType |= HB_ET_MACRO_ASSIGN;

         HB_EXPR_USE( pSelf->value.asMessage.pMessage, HB_EA_PUSH_PCODE );
      }
      /* Push object */
      HB_EXPR_USE( pSelf->value.asMessage.pObject, HB_EA_PUSH_PCODE );
   }
   else /* WITH OBJECT */
   {
      /* Push _message and object */
      if( pSelf->value.asMessage.szMessage )
      {
         HB_GEN_FUNC2( MessageData, pSelf->value.asMessage.szMessage, FALSE );
      }
      else
      {
         if( pSelf->value.asMessage.pMessage->ExprType == HB_ET_MACRO )
            /* o:&macro := value
             * set ASSIGN flag in macro expression
             * it's cleared just after use
             */
            pSelf->value.asMessage.pMessage->value.asMacro.SubType |= HB_ET_MACRO_ASSIGN;

         HB_EXPR_USE( pSelf->value.asMessage.pMessage, HB_EA_PUSH_PCODE );
         /* Push object using WITHOBJECTMESSAGE pcode */
         HB_GEN_FUNC2( Message, NULL, FALSE );
      }
   }
}

static void hb_compExprPushSendPush( HB_EXPR_PTR pSelf, HB_COMP_DECL )
{
   if( pSelf->value.asMessage.pObject )
   {
      /* Push message */
      if( pSelf->value.asMessage.szMessage )
      {
         HB_GEN_FUNC2( Message, pSelf->value.asMessage.szMessage, TRUE );
      }
      else
      {
         HB_EXPR_USE( pSelf->value.asMessage.pMessage, HB_EA_PUSH_PCODE );
      }
      /* Push object */
      HB_EXPR_USE( pSelf->value.asMessage.pObject, HB_EA_PUSH_PCODE );
   }
   else /* WITH OBJECT */
   {
      if( pSelf->value.asMessage.szMessage )
      {
         /* Push message and object */
         HB_GEN_FUNC2( Message, pSelf->value.asMessage.szMessage, FALSE );
      }
      else
      {
         /* Push message */
         HB_EXPR_USE( pSelf->value.asMessage.pMessage, HB_EA_PUSH_PCODE );
         /* Push object using WITHOBJECTMESSAGE pcode */
         HB_GEN_FUNC2( Message, NULL, FALSE );
      }
   }
}

static void hb_compExprPushSendPopPush( HB_EXPR_PTR pObj, HB_EXPR_PTR pValue,
                                        BOOL fPreOp, BYTE bOper, HB_COMP_DECL )
{
   if( HB_SUPPORT_HARBOUR )
   {
      hb_compExprPushSendPop( pObj, HB_COMP_PARAM );
      /* duplicate object variable */
      HB_GEN_FUNC1( PCode1, HB_P_DUPLICATE );
      /* Push message */
      if( pObj->value.asMessage.szMessage )
      {
         /* TRUE used intnetionally to not push object variable in WITH OBJECT */
         HB_GEN_FUNC2( Message, pObj->value.asMessage.szMessage, TRUE );
      }
      else
      {
         HB_EXPR_USE( pObj->value.asMessage.pMessage, HB_EA_PUSH_PCODE );
      }
      HB_GEN_FUNC2( PCode2, HB_P_SWAP, 0 );
      HB_GEN_FUNC2( PCode2, HB_P_SENDSHORT, 0 );
      if( fPreOp )
      {
         /* push the result on the stack */
         HB_GEN_FUNC1( PCode1, HB_P_DUPLICATE );
         HB_GEN_FUNC2( PCode2, HB_P_SWAP, 2 );
      }
   }
   else
   {
      if( fPreOp )
      {
         /* push current value - it will be a result of whole expression */
         HB_EXPR_USE( pObj, HB_EA_PUSH_PCODE );
      }
      hb_compExprPushSendPop( pObj, HB_COMP_PARAM );
      hb_compExprPushSendPush( pObj, HB_COMP_PARAM );
      HB_GEN_FUNC2( PCode2, HB_P_SENDSHORT, 0 );
   }
   /* push increment value */
   if( pValue )
   {
      HB_EXPR_USE( pValue, HB_EA_PUSH_PCODE );
   }
   /* do operation */
   HB_GEN_FUNC1( PCode1, bOper );
   /* Now do the assignment - call pop message with one argument */
   HB_GEN_FUNC2( PCode2, HB_P_SENDSHORT, 1 );
   if( fPreOp )
   {
      /* pop the unneeded value left by assignment message from the stack */
      HB_GEN_FUNC1( PCode1, HB_P_POP );
   }
}

/* Generates pcodes for compound operators    += -= *= /= %= ^=
 *
 * pExpr is an expression created by hb_compExprNew<operator>Eq functions
 */
/* NOTE: COMPATIBILITY ISSUE:
 * The HB_SUPPORT_HARBOUR in code below determines
 * the way the chained send messages are handled.
 * For example, the following code:
 *
 * a:b( COUNT() ):c += 1
 *
 * will be handled as:
 *
 * a:b( COUNT() ):c := a:b( COUNT() ):c + 1
 *
 * in strict Clipper compatibility mode
 * (HB_SUPPORT_HARBOUR is not set: -kc compiler switch ) and
 *
 * temp := a:b( COUNT() ), temp:c += 1
 *
 * in non-strict mode (-kh).
 * In practice in Clipper it will call COUNT() function two times: the
 * first time before addition and the second one after addition - in Harbour,
 * COUNT() function will be called only once, before addition.
 * The Harbour (non-strict) method is:
 * 1) faster
 * 2) it guarantees that the same instance variable of the same object will
 *   be changed
 */

static void hb_compExprPushOperEq( HB_EXPR_PTR pSelf, BYTE bOpEq, HB_COMP_DECL )
{
   BYTE bNewOp;

   if( HB_SUPPORT_HARBOUR )
   {
      switch( bOpEq )
      {
         case HB_P_PLUS:
            bNewOp = HB_P_PLUSEQ;
            break;
         case HB_P_MINUS:
            bNewOp = HB_P_MINUSEQ;
            break;
         case HB_P_MULT:
            bNewOp = HB_P_MULTEQ;
            break;
         case HB_P_DIVIDE:
            bNewOp = HB_P_DIVEQ;
            break;
         case HB_P_MODULUS:
            bNewOp = HB_P_MODEQ;
            break;
         case HB_P_POWER:
            bNewOp = HB_P_EXPEQ;
            break;
         default:
            bNewOp = bOpEq;
            break;
      }
   }
   else
      bNewOp = bOpEq;

   /* NOTE: an object instance variable needs special handling
    */
   if( pSelf->value.asOperator.pLeft->ExprType == HB_ET_SEND )
   {
#ifdef HB_USE_OBJMSG_REF
      if( bOpEq != bNewOp )
      {
         hb_compExprPushSendPop( pSelf->value.asOperator.pLeft, HB_COMP_PARAM );
         HB_GEN_FUNC1( PCode1, HB_P_PUSHOVARREF );
         /* push increment value */
         HB_EXPR_USE( pSelf->value.asOperator.pRight, HB_EA_PUSH_PCODE );
         HB_GEN_FUNC1( PCode1, bNewOp );
      }
      else
#endif
      {
         hb_compExprPushSendPopPush( pSelf->value.asOperator.pLeft,
                                     pSelf->value.asOperator.pRight,
                                     FALSE, bOpEq, HB_COMP_PARAM );
      }
      return;
   }
   else if( bOpEq != bNewOp )
   {
      if( pSelf->value.asOperator.pLeft->ExprType == HB_ET_MACRO )
      {
         USHORT usType = pSelf->value.asOperator.pLeft->value.asMacro.SubType;
         if( usType == HB_ET_MACRO_VAR )
         {
            /* NOTE: direct type change */
            pSelf->value.asOperator.pLeft->value.asMacro.SubType = HB_ET_MACRO_REFER;
            HB_EXPR_USE( pSelf->value.asOperator.pLeft, HB_EA_PUSH_PCODE );
            HB_EXPR_USE( pSelf->value.asOperator.pRight, HB_EA_PUSH_PCODE );
            HB_GEN_FUNC1( PCode1, bNewOp );
            pSelf->value.asOperator.pLeft->value.asMacro.SubType = usType;
            return;
         }
      }
#ifdef HB_USE_ARRAYAT_REF
      /* NOTE: code for arrays is differ to correctly handle a[ i++ ]++ */
      else if( pSelf->value.asOperator.pLeft->ExprType == HB_ET_ARRAYAT )
      {
         /* Note: change type to array reference */
         pSelf->value.asOperator.pLeft->value.asList.reference = TRUE;
         HB_EXPR_USE( pSelf->value.asOperator.pLeft, HB_EA_PUSH_PCODE );
         pSelf->value.asOperator.pLeft->value.asList.reference = FALSE;

         HB_EXPR_USE( pSelf->value.asOperator.pRight, HB_EA_PUSH_PCODE );
         HB_GEN_FUNC1( PCode1, bNewOp );
         return;
      }
#endif
      else if( pSelf->value.asOperator.pLeft->ExprType == HB_ET_VARIABLE )
      {
#if defined( HB_MACRO_SUPPORT )
         {
#else
         int iVar, iScope;

         hb_compVariableFind( HB_COMP_PARAM, pSelf->value.asOperator.pLeft->value.asSymbol, &iVar, &iScope );
         if( iScope != HB_VS_LOCAL_FIELD && iScope != HB_VS_GLOBAL_FIELD &&
             iScope != HB_VS_UNDECLARED )
         {
            if( iScope == HB_VS_LOCAL_VAR &&
                pSelf->value.asOperator.pRight->ExprType == HB_ET_NUMERIC &&
                ( bOpEq == HB_P_PLUS || bOpEq == HB_P_MINUS ) )
            {
               if( hb_compExprIsInteger( pSelf->value.asOperator.pRight ) )
               {
                  short iIncrement = ( short ) pSelf->value.asOperator.pRight->value.asNum.val.l;

                  if( bOpEq != HB_P_MINUS || iIncrement >= -INT16_MAX )
                  {
                     BYTE buffer[ 5 ];

                     if( bOpEq == HB_P_MINUS )
                        iIncrement = -iIncrement;

                     buffer[ 0 ] = HB_P_LOCALADDINT;
                     buffer[ 1 ] = HB_LOBYTE( iVar );
                     buffer[ 2 ] = HB_HIBYTE( iVar );
                     buffer[ 3 ] = HB_LOBYTE( iIncrement );
                     buffer[ 4 ] = HB_HIBYTE( iIncrement );
                     HB_GEN_FUNC2( PCodeN, buffer, 5 );

                     HB_EXPR_USE( pSelf->value.asOperator.pLeft, HB_EA_PUSH_PCODE );
                     return;
                  }
               }
            }
#endif
            /* NOTE: direct type change */
            pSelf->value.asOperator.pLeft->ExprType = HB_ET_VARREF;

            HB_EXPR_USE( pSelf->value.asOperator.pLeft, HB_EA_PUSH_PCODE );
            HB_EXPR_USE( pSelf->value.asOperator.pRight, HB_EA_PUSH_PCODE );
            HB_GEN_FUNC1( PCode1, bNewOp );
            return;
         }
      }
   }
   /* push old value */
   HB_EXPR_USE( pSelf->value.asOperator.pLeft, HB_EA_PUSH_PCODE );
   /* push increment value */
   HB_EXPR_USE( pSelf->value.asOperator.pRight, HB_EA_PUSH_PCODE );
   /* perform operation and duplicate the new value */
   HB_GEN_FUNC1( PCode1, bOpEq );
   HB_GEN_FUNC1( PCode1, HB_P_DUPLICATE );
   /* pop the new value into variable and leave the copy on the stack */
   HB_EXPR_USE( pSelf->value.asOperator.pLeft, HB_EA_POP_PCODE );
}

/* Generates pcodes for <operator>= syntax
 * used standalone as a statement (it cannot leave the value on the stack)
 */
static void hb_compExprUseOperEq( HB_EXPR_PTR pSelf, BYTE bOpEq, HB_COMP_DECL )
{
   BYTE bNewOp;

   if( HB_SUPPORT_HARBOUR )
   {
      switch( bOpEq )
      {
         case HB_P_PLUS:
            bNewOp = HB_P_PLUSEQPOP;
            break;
         case HB_P_MINUS:
            bNewOp = HB_P_MINUSEQPOP;
            break;
         case HB_P_MULT:
            bNewOp = HB_P_MULTEQPOP;
            break;
         case HB_P_DIVIDE:
            bNewOp = HB_P_DIVEQPOP;
            break;
         case HB_P_MODULUS:
            bNewOp = HB_P_MODEQPOP;
            break;
         case HB_P_POWER:
            bNewOp = HB_P_EXPEQPOP;
            break;
         default:
            bNewOp = bOpEq;
            break;
      }
   }
   else
      bNewOp = bOpEq;

   /* NOTE: an object instance variable needs special handling
    */
   if( pSelf->value.asOperator.pLeft->ExprType == HB_ET_SEND )
   {
#ifdef HB_USE_OBJMSG_REF
      if( bOpEq != bNewOp )
      {
         hb_compExprPushSendPop( pSelf->value.asOperator.pLeft, HB_COMP_PARAM );
         HB_GEN_FUNC1( PCode1, HB_P_PUSHOVARREF );
         /* push increment value */
         HB_EXPR_USE( pSelf->value.asOperator.pRight, HB_EA_PUSH_PCODE );
         HB_GEN_FUNC1( PCode1, bNewOp );
      }
      else
#endif
      {
         hb_compExprPushSendPopPush( pSelf->value.asOperator.pLeft,
                                     pSelf->value.asOperator.pRight,
                                     FALSE, bOpEq, HB_COMP_PARAM );
         /* pop the unneeded value from the stack */
         HB_GEN_FUNC1( PCode1, HB_P_POP );
      }
      return;
   }
   else if( bOpEq != bNewOp )
   {
      if( pSelf->value.asOperator.pLeft->ExprType == HB_ET_MACRO )
      {
         USHORT usType = pSelf->value.asOperator.pLeft->value.asMacro.SubType;
         if( usType == HB_ET_MACRO_VAR )
         {
            /* NOTE: direct type change */
            pSelf->value.asOperator.pLeft->value.asMacro.SubType = HB_ET_MACRO_REFER;
            HB_EXPR_USE( pSelf->value.asOperator.pLeft, HB_EA_PUSH_PCODE );
            HB_EXPR_USE( pSelf->value.asOperator.pRight, HB_EA_PUSH_PCODE );
            HB_GEN_FUNC1( PCode1, bNewOp );
            pSelf->value.asOperator.pLeft->value.asMacro.SubType = usType;
            return;
         }
      }
#ifdef HB_USE_ARRAYAT_REF
      /* NOTE: code for arrays is differ to correctly handle a[ i++ ]++ */
      else if( pSelf->value.asOperator.pLeft->ExprType == HB_ET_ARRAYAT )
      {
         /* Note: change type to array reference */
         pSelf->value.asOperator.pLeft->value.asList.reference = TRUE;
         HB_EXPR_USE( pSelf->value.asOperator.pLeft, HB_EA_PUSH_PCODE );
         pSelf->value.asOperator.pLeft->value.asList.reference = FALSE;

         HB_EXPR_USE( pSelf->value.asOperator.pRight, HB_EA_PUSH_PCODE );
         HB_GEN_FUNC1( PCode1, bNewOp );
         return;
      }
#endif
      else if( pSelf->value.asOperator.pLeft->ExprType == HB_ET_VARIABLE )
      {
         HB_EXPRTYPE iOldType;
#if defined( HB_MACRO_SUPPORT )
         {
#else
         int iVar, iScope;

         hb_compVariableFind( HB_COMP_PARAM, pSelf->value.asOperator.pLeft->value.asSymbol, &iVar, &iScope );
         if( iScope != HB_VS_LOCAL_FIELD && iScope != HB_VS_GLOBAL_FIELD &&
             iScope != HB_VS_UNDECLARED )
         {
            if( iScope == HB_VS_LOCAL_VAR &&
                pSelf->value.asOperator.pRight->ExprType == HB_ET_NUMERIC &&
                ( bOpEq == HB_P_PLUS || bOpEq == HB_P_MINUS ) )
            {
               if( hb_compExprIsInteger( pSelf->value.asOperator.pRight ) )
               {
                  short iIncrement = ( short ) pSelf->value.asOperator.pRight->value.asNum.val.l;

                  if( bOpEq != HB_P_MINUS || iIncrement >= -INT16_MAX )
                  {
                     BYTE buffer[ 5 ];

                     if( bOpEq == HB_P_MINUS )
                        iIncrement = -iIncrement;

                     buffer[ 0 ] = HB_P_LOCALADDINT;
                     buffer[ 1 ] = HB_LOBYTE( iVar );
                     buffer[ 2 ] = HB_HIBYTE( iVar );
                     buffer[ 3 ] = HB_LOBYTE( iIncrement );
                     buffer[ 4 ] = HB_HIBYTE( iIncrement );
                     HB_GEN_FUNC2( PCodeN, buffer, 5 );
                     return;
                  }
               }
            }
#endif
            /* NOTE: direct type change */
            iOldType = pSelf->value.asOperator.pLeft->ExprType;
            pSelf->value.asOperator.pLeft->ExprType = HB_ET_VARREF;

            HB_EXPR_USE( pSelf->value.asOperator.pLeft, HB_EA_PUSH_PCODE );
            HB_EXPR_USE( pSelf->value.asOperator.pRight, HB_EA_PUSH_PCODE );
            HB_GEN_FUNC1( PCode1, bNewOp );
            pSelf->value.asOperator.pLeft->ExprType = iOldType;
            return;
         }
      }
   }
   /* push old value */
   HB_EXPR_USE( pSelf->value.asOperator.pLeft, HB_EA_PUSH_PCODE );
   /* push increment value */
   HB_EXPR_USE( pSelf->value.asOperator.pRight, HB_EA_PUSH_PCODE );
   /* add */
   HB_GEN_FUNC1( PCode1, bOpEq );
   /* pop the new value into variable and remove it from the stack */
   HB_EXPR_USE( pSelf->value.asOperator.pLeft, HB_EA_POP_PCODE );
}

/* Generates the pcodes for pre- increment/decrement expressions
 */
static void hb_compExprPushPreOp( HB_EXPR_PTR pSelf, BYTE bOper, HB_COMP_DECL )
{
   /* NOTE: an object instance variable needs special handling
    */
   if( pSelf->value.asOperator.pLeft->ExprType == HB_ET_SEND )
   {
#ifdef HB_USE_OBJMSG_REF
      if( HB_SUPPORT_HARBOUR )
      {
         hb_compExprPushSendPop( pSelf->value.asOperator.pLeft, HB_COMP_PARAM );
         HB_GEN_FUNC1( PCode1, HB_P_PUSHOVARREF );
         /* increase/decrease operation, leave unreferenced value on stack */
         HB_GEN_FUNC1( PCode1, ( bOper == HB_P_INC ) ? HB_P_INCEQ : HB_P_DECEQ );
      }
      else
#endif
      {
         hb_compExprPushSendPopPush( pSelf->value.asOperator.pLeft, NULL,
                                     FALSE, bOper, HB_COMP_PARAM );
      }
      return;
   }
   else if( HB_SUPPORT_HARBOUR )
   {
      if( pSelf->value.asOperator.pLeft->ExprType == HB_ET_MACRO &&
          pSelf->value.asOperator.pLeft->value.asMacro.SubType == HB_ET_MACRO_VAR )
      {
         USHORT usType = pSelf->value.asOperator.pLeft->value.asMacro.SubType;
         /* NOTE: direct type change */
         pSelf->value.asOperator.pLeft->value.asMacro.SubType = HB_ET_MACRO_REFER;
         HB_EXPR_USE( pSelf->value.asOperator.pLeft, HB_EA_PUSH_PCODE );
         pSelf->value.asOperator.pLeft->value.asMacro.SubType = usType;

         /* increase/decrease operation, leave unreferenced value on stack */
         HB_GEN_FUNC1( PCode1, ( BYTE ) ( bOper == HB_P_INC ? HB_P_INCEQ : HB_P_DECEQ ) );
         return;
      }
#ifdef HB_USE_ARRAYAT_REF
      /* NOTE: code for arrays is differ to correctly handle a[ i++ ]++ */
      else if( pSelf->value.asOperator.pLeft->ExprType == HB_ET_ARRAYAT )
      {
         /* push reference to current value */
         /* Note: change type to array reference */
         pSelf->value.asOperator.pLeft->value.asList.reference = TRUE;
         HB_EXPR_USE( pSelf->value.asOperator.pLeft, HB_EA_PUSH_PCODE );
         pSelf->value.asOperator.pLeft->value.asList.reference = FALSE;

         /* increase/decrease operation, leave unreferenced value on stack */
         HB_GEN_FUNC1( PCode1, ( BYTE ) ( bOper == HB_P_INC ? HB_P_INCEQ : HB_P_DECEQ ) );
         return;
      }
#endif
#if !defined( HB_MACRO_SUPPORT )
      else if( pSelf->value.asOperator.pLeft->ExprType == HB_ET_VARIABLE )
      {
         int iVar, iScope;

         hb_compVariableFind( HB_COMP_PARAM, pSelf->value.asOperator.pLeft->value.asSymbol, &iVar, &iScope );
         if( iScope != HB_VS_LOCAL_FIELD && iScope != HB_VS_GLOBAL_FIELD &&
             iScope != HB_VS_UNDECLARED )
         {
            if( iScope == HB_VS_LOCAL_VAR )
            {
               if( bOper == HB_P_INC )
               {
                  HB_GEN_FUNC3( PCode3, HB_P_LOCALINCPUSH, HB_LOBYTE( iVar ), HB_HIBYTE( iVar ) );
               }
               else
               {
                  HB_GEN_FUNC3( PCode3, HB_P_LOCALDEC, HB_LOBYTE( iVar ), HB_HIBYTE( iVar ) );
                  /* Push current value */
                  HB_EXPR_USE( pSelf->value.asOperator.pLeft, HB_EA_PUSH_PCODE );
               }
            }
            else
            {
               /* NOTE: direct type change */
               HB_EXPRTYPE iOldType = pSelf->value.asOperator.pLeft->ExprType;
               pSelf->value.asOperator.pLeft->ExprType = HB_ET_VARREF;
               HB_EXPR_USE( pSelf->value.asOperator.pLeft, HB_EA_PUSH_PCODE );
               HB_GEN_FUNC1( PCode1, ( BYTE ) ( bOper == HB_P_INC ? HB_P_INCEQ : HB_P_DECEQ ) );
               pSelf->value.asOperator.pLeft->ExprType = iOldType;
            }
            return;
         }
      }
#endif
   }

   /* Push current value */
   HB_EXPR_USE( pSelf->value.asOperator.pLeft, HB_EA_PUSH_PCODE );
   /* Increment */
   HB_GEN_FUNC1( PCode1, bOper );
   /* duplicate a value */
   HB_GEN_FUNC1( PCode1, HB_P_DUPLICATE );
   /* pop new value and leave the duplicated copy of it on the stack */
   HB_EXPR_USE( pSelf->value.asOperator.pLeft, HB_EA_POP_PCODE );
}

/* Generates the pcodes for post- increment/decrement expressions
 */
static void hb_compExprPushPostOp( HB_EXPR_PTR pSelf, BYTE bOper, HB_COMP_DECL )
{
   /* NOTE: an object instance variable needs special handling
    */
   if( pSelf->value.asOperator.pLeft->ExprType == HB_ET_SEND )
   {
#ifdef HB_USE_OBJMSG_REF
      if( HB_SUPPORT_HARBOUR )
      {
         /* push reference to current value */
         hb_compExprPushSendPop( pSelf->value.asOperator.pLeft, HB_COMP_PARAM );
         HB_GEN_FUNC1( PCode1, HB_P_PUSHOVARREF );
         /* Duplicate the reference and unref the original one -
          * it will be the result of whole expression
          */
         HB_GEN_FUNC1( PCode1, HB_P_DUPLUNREF );
         /* increment/decrement the value */
         HB_GEN_FUNC1( PCode1, ( bOper == HB_P_INC ) ? HB_P_INCEQPOP : HB_P_DECEQPOP );
      }
      else
#endif
      {
         hb_compExprPushSendPopPush( pSelf->value.asOperator.pLeft, NULL,
                                     TRUE, bOper, HB_COMP_PARAM );
      }
      return;
   }
   else if( HB_SUPPORT_HARBOUR )
   {
      if( pSelf->value.asOperator.pLeft->ExprType == HB_ET_MACRO &&
          pSelf->value.asOperator.pLeft->value.asMacro.SubType == HB_ET_MACRO_VAR )
      {
         USHORT usType = pSelf->value.asOperator.pLeft->value.asMacro.SubType;
         /* NOTE: direct type change */
         pSelf->value.asOperator.pLeft->value.asMacro.SubType = HB_ET_MACRO_REFER;
         HB_EXPR_USE( pSelf->value.asOperator.pLeft, HB_EA_PUSH_PCODE );
         pSelf->value.asOperator.pLeft->value.asMacro.SubType = usType;

         /* Duplicate the reference and unref the original one -
          * it will be the result of whole expression
          */
         HB_GEN_FUNC1( PCode1, HB_P_DUPLUNREF );
         /* increase/decrease operation */
         HB_GEN_FUNC1( PCode1, ( BYTE ) ( bOper == HB_P_INC ? HB_P_INCEQPOP : HB_P_DECEQPOP ) );
         return;
      }
#ifdef HB_USE_ARRAYAT_REF
      /* NOTE: code for arrays is differ to correctly handle a[ i++ ]++ */
      else if( pSelf->value.asOperator.pLeft->ExprType == HB_ET_ARRAYAT )
      {
         /* push reference to current value */
         /* Note: change type to array reference */
         pSelf->value.asOperator.pLeft->value.asList.reference = TRUE;
         HB_EXPR_USE( pSelf->value.asOperator.pLeft, HB_EA_PUSH_PCODE );
         pSelf->value.asOperator.pLeft->value.asList.reference = FALSE;

         /* Duplicate the reference and unref the original one -
          * it will be the result of whole expression
          */
         HB_GEN_FUNC1( PCode1, HB_P_DUPLUNREF );
         /* increase/decrease operation */
         HB_GEN_FUNC1( PCode1, ( BYTE ) ( bOper == HB_P_INC ? HB_P_INCEQPOP : HB_P_DECEQPOP ) );
         return;
      }
#endif
#if !defined( HB_MACRO_SUPPORT )
      else if( pSelf->value.asOperator.pLeft->ExprType == HB_ET_VARIABLE )
      {
         int iVar, iScope;

         hb_compVariableFind( HB_COMP_PARAM, pSelf->value.asOperator.pLeft->value.asSymbol, &iVar, &iScope );
         if( iScope != HB_VS_LOCAL_FIELD && iScope != HB_VS_GLOBAL_FIELD &&
             iScope != HB_VS_UNDECLARED )
         {
            if( iScope == HB_VS_LOCAL_VAR )
            {
               /* Push current value */
               HB_EXPR_USE( pSelf->value.asOperator.pLeft, HB_EA_PUSH_PCODE );
               HB_GEN_FUNC3( PCode3, ( BYTE ) ( bOper == HB_P_INC ? HB_P_LOCALINC : HB_P_LOCALDEC ),
                             HB_LOBYTE( iVar ), HB_HIBYTE( iVar ) );
            }
            else
            {
               /* NOTE: direct type change */
               HB_EXPRTYPE iOldType = pSelf->value.asOperator.pLeft->ExprType;
               pSelf->value.asOperator.pLeft->ExprType = HB_ET_VARREF;
               HB_EXPR_USE( pSelf->value.asOperator.pLeft, HB_EA_PUSH_PCODE );
               HB_GEN_FUNC1( PCode1, HB_P_DUPLUNREF );
               HB_GEN_FUNC1( PCode1, ( BYTE ) ( bOper == HB_P_INC ? HB_P_INCEQPOP : HB_P_DECEQPOP ) );
               pSelf->value.asOperator.pLeft->ExprType = iOldType;
            }
            return;
         }
      }
#endif
   }

   /* Push current value */
   HB_EXPR_USE( pSelf->value.asOperator.pLeft, HB_EA_PUSH_PCODE );
   /* Duplicate value */
   HB_GEN_FUNC1( PCode1, HB_P_DUPLICATE );
   /* Increment */
   HB_GEN_FUNC1( PCode1, bOper );
   /* pop new value from the stack */
   HB_EXPR_USE( pSelf->value.asOperator.pLeft, HB_EA_POP_PCODE );
}

/* Generates the pcodes for increment/decrement operations
 * used standalone as a statement
 */
static void hb_compExprUsePreOp( HB_EXPR_PTR pSelf, BYTE bOper, HB_COMP_DECL )
{
   /* NOTE: an object instance variable needs special handling
    */
   if( pSelf->value.asOperator.pLeft->ExprType == HB_ET_SEND )
   {
#ifdef HB_USE_OBJMSG_REF
      if( HB_SUPPORT_HARBOUR )
      {
         /* push reference to current value */
         hb_compExprPushSendPop( pSelf->value.asOperator.pLeft, HB_COMP_PARAM );
         HB_GEN_FUNC1( PCode1, HB_P_PUSHOVARREF );
         /* increment/decrement the value */
         HB_GEN_FUNC1( PCode1, ( bOper == HB_P_INC ) ? HB_P_INCEQPOP : HB_P_DECEQPOP );
      }
      else
#endif
      {
         hb_compExprPushSendPopPush( pSelf->value.asOperator.pLeft, NULL,
                                     FALSE, bOper, HB_COMP_PARAM );
         /* pop the value from the stack */
         HB_GEN_FUNC1( PCode1, HB_P_POP );
      }
      return;
   }
   else if( HB_SUPPORT_HARBOUR )
   {
      if( pSelf->value.asOperator.pLeft->ExprType == HB_ET_MACRO &&
          pSelf->value.asOperator.pLeft->value.asMacro.SubType == HB_ET_MACRO_VAR )
      {
         USHORT usType = pSelf->value.asOperator.pLeft->value.asMacro.SubType;
         /* NOTE: direct type change */
         pSelf->value.asOperator.pLeft->value.asMacro.SubType = HB_ET_MACRO_REFER;
         HB_EXPR_USE( pSelf->value.asOperator.pLeft, HB_EA_PUSH_PCODE );
         pSelf->value.asOperator.pLeft->value.asMacro.SubType = usType;

         /* increase/decrease operation */
         HB_GEN_FUNC1( PCode1, ( BYTE ) ( bOper == HB_P_INC ? HB_P_INCEQPOP : HB_P_DECEQPOP ) );
         return;
      }
#ifdef HB_USE_ARRAYAT_REF
      /* NOTE: code for arrays is differ to correctly handle a[ i++ ]++ */
      else if( pSelf->value.asOperator.pLeft->ExprType == HB_ET_ARRAYAT )
      {
         /* push reference to current value */
         /* Note: change type to array reference */
         pSelf->value.asOperator.pLeft->value.asList.reference = TRUE;
         HB_EXPR_USE( pSelf->value.asOperator.pLeft, HB_EA_PUSH_PCODE );
         pSelf->value.asOperator.pLeft->value.asList.reference = FALSE;
         /* increase/decrease operation */
         HB_GEN_FUNC1( PCode1, ( BYTE ) ( bOper == HB_P_INC ? HB_P_INCEQPOP : HB_P_DECEQPOP ) );
         return;
      }
#endif
#if !defined( HB_MACRO_SUPPORT )
      else if( pSelf->value.asOperator.pLeft->ExprType == HB_ET_VARIABLE )
      {
         int iVar, iScope;

         hb_compVariableFind( HB_COMP_PARAM, pSelf->value.asOperator.pLeft->value.asSymbol, &iVar, &iScope );
         if( iScope != HB_VS_LOCAL_FIELD && iScope != HB_VS_GLOBAL_FIELD &&
             iScope != HB_VS_UNDECLARED )
         {
            if( iScope == HB_VS_LOCAL_VAR )
            {
               HB_GEN_FUNC3( PCode3, ( BYTE ) ( bOper == HB_P_INC ? HB_P_LOCALINC : HB_P_LOCALDEC ),
                             HB_LOBYTE( iVar ), HB_HIBYTE( iVar ) );
            }
            else
            {
               /* NOTE: direct type change */
               HB_EXPRTYPE iOldType = pSelf->value.asOperator.pLeft->ExprType;
               pSelf->value.asOperator.pLeft->ExprType = HB_ET_VARREF;
               HB_EXPR_USE( pSelf->value.asOperator.pLeft, HB_EA_PUSH_PCODE );
               HB_GEN_FUNC1( PCode1, ( BYTE ) ( bOper == HB_P_INC ? HB_P_INCEQPOP : HB_P_DECEQPOP ) );
               pSelf->value.asOperator.pLeft->ExprType = iOldType;
            }
            return;
         }
      }
#endif
   }

   /* Push current value */
   HB_EXPR_USE( pSelf->value.asOperator.pLeft, HB_EA_PUSH_PCODE );
   /* Increment */
   HB_GEN_FUNC1( PCode1, bOper );
   /* pop new value from the stack */
   HB_EXPR_USE( pSelf->value.asOperator.pLeft, HB_EA_POP_PCODE );
}

/* Generate pcode for aliased expression which contains macro operator on
 * the left or right side of the alias operator
 * expression->&macro or &macro->expression or &macro->&macro
 */
static void hb_compExprUseAliasMacro( HB_EXPR_PTR pAliasedVar, BYTE bAction, HB_COMP_DECL )
{
   HB_EXPR_PTR pAlias, pVar;

   /* Alias->Var
    */
   pAlias = pAliasedVar->value.asAlias.pAlias;
   pVar   = pAliasedVar->value.asAlias.pVar;
   if( pAlias->ExprType == HB_ET_ALIAS )
   {
      /* database alias */
      /* Push alias identifier as string so it can be joined with
       * variable at runtime
       * NOTE:
       *    ALIAS->&var is the same as &( "ALIAS->" + var )
       *
       */
      HB_GEN_FUNC2( PushString, pAlias->value.asSymbol, strlen( pAlias->value.asSymbol ) + 1 );
      HB_EXPR_USE( pVar, HB_EA_PUSH_PCODE );
      if( bAction == HB_EA_PUSH_PCODE )
         HB_GEN_FUNC1( PCode1, HB_P_MACROPUSHALIASED );
      else
         HB_GEN_FUNC1( PCode1, HB_P_MACROPOPALIASED );
   }
   else if( pVar->ExprType == HB_ET_VARIABLE )
   {
      /* NOTE:
       *    &macro->var is the  same as: &( macro + "->var" )
       */
      HB_EXPR_USE( pAlias, HB_EA_PUSH_PCODE );
      HB_GEN_FUNC2( PushString, pVar->value.asSymbol, strlen( pVar->value.asSymbol ) + 1 );
      if( bAction == HB_EA_PUSH_PCODE )
         HB_GEN_FUNC1( PCode1, HB_P_MACROPUSHALIASED );
      else
         HB_GEN_FUNC1( PCode1, HB_P_MACROPOPALIASED );
   }
   else
   {
      HB_EXPR_USE( pAlias, HB_EA_PUSH_PCODE );
      HB_EXPR_USE( pVar, HB_EA_PUSH_PCODE );
      if( bAction == HB_EA_PUSH_PCODE )
         HB_GEN_FUNC1( PCode1, HB_P_MACROPUSHALIASED );
      else
         HB_GEN_FUNC1( PCode1, HB_P_MACROPOPALIASED );
   }

   /* Always add byte to pcode indicating requested macro compiler flag. */
   HB_GEN_FUNC1( PCode1, ( BYTE ) HB_MACRO_GENFLAGS );
}


/* Reduces the list of expressions
 *
 * pExpr is the first expression on the list
 */
static HB_EXPR_PTR hb_compExprReduceList( HB_EXPR_PTR pList, HB_COMP_DECL )
{
   HB_EXPR_PTR pNext, pExpr;
   HB_EXPR_PTR * pPrev;

   /* NOTE: During optimalization an expression on the list can be
    * replaced by the new one
    */

   pPrev = &pList->value.asList.pExprList;
   pExpr = pList->value.asList.pExprList;
   while( pExpr )
   {
      pNext  = pExpr->pNext; /* store next expression in case the current will be reduced */
      pExpr  = HB_EXPR_USE( pExpr, HB_EA_REDUCE );
      if( HB_SUPPORT_HARBOUR )
         pExpr = hb_compExprListStrip( pExpr, HB_COMP_PARAM );
      *pPrev = pExpr;   /* store a new expression into the previous one */
      pExpr->pNext = pNext;  /* restore the link to next expression */
      pPrev  = &pExpr->pNext;
      pExpr  = pNext;
   }
   return pList;
}

/* reduce ( "alias" )-> to ALIAS->
 */
static HB_EXPR_PTR hb_compExprReduceAliasString( HB_EXPR_PTR pExpr, HB_EXPR_PTR pAlias, HB_COMP_DECL )
{
   const char * szAlias = pAlias->value.asString.string;

   if( HB_ISFIRSTIDCHAR( *szAlias ) )
   {
      ULONG ulLen = pAlias->ulLength;
      if( ulLen <= HB_SYMBOL_NAME_LEN )
      {
         BOOL fLower = FALSE;
         while( ulLen )
         {
            char c = szAlias[ ulLen - 1 ];
            if( !HB_ISNEXTIDCHAR( c ) )
               break;
            if( HB_ISLOWER( c ) )
               fLower = TRUE;
            --ulLen;
         }
         if( ulLen == 0 )
         {
#if defined( HB_MACRO_SUPPORT )
            if( fLower )
               szAlias = hb_macroIdentNew( HB_COMP_PARAM, hb_strupr( hb_strdup( szAlias ) ) );
            else if( pAlias->value.asString.dealloc )
               szAlias = hb_macroIdentNew( HB_COMP_PARAM, hb_strdup( szAlias ) );
#else
            if( fLower )
               szAlias = hb_compIdentifierNew( HB_COMP_PARAM, hb_strupr( hb_strdup( szAlias ) ), HB_IDENT_FREE );
            else if( pAlias->value.asString.dealloc )
               szAlias = hb_compIdentifierNew( HB_COMP_PARAM, szAlias, HB_IDENT_COPY );
#endif
            HB_COMP_EXPR_DELETE( pExpr );
            pExpr = hb_compExprNewAlias( szAlias, HB_COMP_PARAM );
         }
      }
   }
   return pExpr;
}

static BOOL hb_compExprIsMemvarAlias( const char *szAlias )
{
   int iLen = strlen( szAlias );
   /* @M-> @MEMVAR-> or @MEMVA-> or @MEMV-> */
   return ( iLen == 1 || ( iLen >= 4 && iLen <= 6 ) ) &&
          memcmp( szAlias, "MEMVAR", iLen ) == 0;
}
