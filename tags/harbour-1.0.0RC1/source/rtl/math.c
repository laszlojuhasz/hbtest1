/*
 * $Id$
 */

/*
 * Harbour Project source code:
 * Math functions
 *
 * Copyright 1999 Matthew Hamilton <mhamilton@bunge.com.au>
 *
 * Functions for user defined math error handlers, changes and fixes
 * Copyright 2001/2002 IntTec GmbH, Freiburg, Germany,
 *                Author: Martin Vogel <vogel@inttec.de>
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

#if defined(__DJGPP__)
#   include <libm/math.h>
_LIB_VERSION_TYPE _LIB_VERSION = _XOPEN_;
#else
#   include <math.h>
#endif

#include "hbapi.h"
#include "hbapiitm.h"
#include "hbapierr.h"
#include "hbvm.h"
#include "hbmath.h"

#if defined(HB_MATH_ERRNO)
#   include <errno.h>
#endif
#if defined(HB_OS_SUNOS)
#   include <ieeefp.h>
#endif

/*
 * ************************************************************
 * Harbour Math functions Part I:
 * handling math errors, C math lib redirection
 * ************************************************************
 */

#if defined(HB_MATH_HANDLER)
static HB_MATH_EXCEPTION s_hb_exc = { HB_MATH_ERR_NONE, "", "", 0.0, 0.0, 0.0, -1, -1, 0 };
#endif

/* reset math error information */
void hb_mathResetError( HB_MATH_EXCEPTION * phb_exc )
{
   HB_TRACE( HB_TR_DEBUG, ( "hb_mathResetError(%p)", phb_exc ) );

   HB_SYMBOL_UNUSED( phb_exc );

#if defined(HB_MATH_HANDLER)
   s_hb_exc.type = HB_MATH_ERR_NONE;
   s_hb_exc.funcname = "";
   s_hb_exc.error = "";
   s_hb_exc.arg1 = 0.0;
   s_hb_exc.arg2 = 0.0;
   s_hb_exc.retval = 0.0;
   s_hb_exc.retvalwidth = -1;   /* we don't know */
   s_hb_exc.retvaldec = -1;     /* use standard SET DECIMALS */
   s_hb_exc.handled = 1;
#elif defined(HB_MATH_ERRNO)
   errno = 0;
#endif
}

/* route C math lib errors to Harbour error handling */
#if defined(HB_MATH_HANDLER)

HB_EXPORT int matherr( struct exception *err )
{
   int retval;
   HB_MATH_HANDLERPROC mathHandler;

   HB_TRACE( HB_TR_DEBUG, ( "matherr(%p)", err ) );

   /* map math error types */
   switch( err->type )
   {
      case DOMAIN:
         s_hb_exc.type = HB_MATH_ERR_DOMAIN;
         s_hb_exc.error = "Argument not in domain of function";
         break;

      case SING:
         s_hb_exc.type = HB_MATH_ERR_SING;
         s_hb_exc.error = "Calculation results in singularity";
         break;

      case OVERFLOW:
         s_hb_exc.type = HB_MATH_ERR_OVERFLOW;
         s_hb_exc.error = "Calculation result too large to represent";
         break;

      case UNDERFLOW:
         s_hb_exc.type = HB_MATH_ERR_UNDERFLOW;
         s_hb_exc.error = "Calculation result too small to represent";
         break;

      case TLOSS:
         s_hb_exc.type = HB_MATH_ERR_TLOSS;
         s_hb_exc.error = "Total loss of significant digits";
         break;

      case PLOSS:
         s_hb_exc.type = HB_MATH_ERR_PLOSS;
         s_hb_exc.error = "Partial loss of significant digits";
         break;

      default:
         s_hb_exc.type = HB_MATH_ERR_UNKNOWN;
         s_hb_exc.error = "Unknown math error";
         break;
   }

   s_hb_exc.funcname = ( char * ) err->name;    /* (char *) Avoid warning in DJGPP */
   s_hb_exc.arg1 = err->arg1;
   s_hb_exc.arg2 = err->arg2;
   s_hb_exc.retval = err->retval;
   s_hb_exc.handled = 0;

   mathHandler = hb_mathGetHandler();
   if( mathHandler != NULL )
   {
      retval = ( *( mathHandler ) ) ( &s_hb_exc );
      err->retval = s_hb_exc.retval;
   }
   else
   {
      /* there is no custom math handler */
      retval = 1;               /* don't print any message, don't set errno and use return value provided by C RTL */
   }
   return retval;
}
#endif

BOOL hb_mathGetError( HB_MATH_EXCEPTION * phb_exc, const char *szFunc,
                      double arg1, double arg2, double dResult )
{
#if defined(HB_MATH_ERRNO)

   int errCode;

   HB_TRACE( HB_TR_DEBUG, ( "hb_mathGetError(%p,%s,%lf,%lf,%lf)", phb_exc, szFunc, arg1, arg2, dResult ) );

   switch( errno )
   {
      case 0:
         return FALSE;
      case EDOM:
      case ERANGE:
#   if defined(EOVERFLOW)
      case EOVERFLOW:
#   endif
         errCode = errno;
         break;

      default:
         if( isnan( dResult ) )
            errCode = EDOM;
#   if defined(HB_OS_SUNOS)
         else if( !finite( dResult ) )
#   elif defined(HB_OS_OS2)
         else if( !isfinite( dResult ) )
#   else
         else if( isinf( dResult ) )
#   endif
            errCode = ERANGE;
         else
            errCode = errno;
   }

   /* map math error types */
   switch( errCode )
   {
      case EDOM:
         phb_exc->type = HB_MATH_ERR_DOMAIN;
         phb_exc->error = "Argument not in domain of function";
         break;

      case ERANGE:
         phb_exc->type = HB_MATH_ERR_SING;
         phb_exc->error = "Calculation results in singularity";
         break;
#   if defined(EOVERFLOW)
      case EOVERFLOW:
         phb_exc->type = HB_MATH_ERR_OVERFLOW;
         phb_exc->error = "Calculation result too large to represent";
         break;
#   endif
      default:
         phb_exc->type = HB_MATH_ERR_UNKNOWN;
         phb_exc->error = "Unknown math error";
         break;
   }

   phb_exc->funcname = szFunc;
   phb_exc->arg1 = arg1;
   phb_exc->arg2 = arg2;
   phb_exc->retval = dResult;
   phb_exc->handled = 0;
   phb_exc->retvalwidth = -1; /* we don't know */
   phb_exc->retvaldec = -1;   /* use standard SET DECIMALS */

   {
      HB_MATH_HANDLERPROC mathHandler = hb_mathGetHandler();
      if( mathHandler != NULL )
         ( *mathHandler )( phb_exc );
   }
   return TRUE;
#else
   HB_TRACE( HB_TR_DEBUG, ( "hb_mathGetError(%p,%s,%lf,%lf,%lf)", phb_exc, szFunc, arg1, arg2, dResult ) );

   HB_SYMBOL_UNUSED( dResult );
   HB_SYMBOL_UNUSED( arg1 );
   HB_SYMBOL_UNUSED( arg2 );
   HB_SYMBOL_UNUSED( szFunc );

#  if defined(HB_MATH_HANDLER)

   memcpy( phb_exc, &s_hb_exc, sizeof( HB_MATH_EXCEPTION ) );
   return phb_exc->type != HB_MATH_ERR_NONE;

#  else

   HB_SYMBOL_UNUSED( phb_exc );
   return FALSE;

#  endif

#endif
}


/*
 * ************************************************************
 * Harbour Math functions Part II:
 * handling math errors, Harbour default handling routine
 * ************************************************************
 */

static int s_hb_matherr_mode = HB_MATH_ERRMODE_DEFAULT; /* TODO: make this thread safe */

/* set error handling mode of hb_matherr() */
int hb_mathSetErrMode( int imode )
{
   int oldmode;

   HB_TRACE( HB_TR_DEBUG, ( "hb_mathSetErrMode (%i)", imode ) );

   oldmode = s_hb_matherr_mode;

   if( ( imode == HB_MATH_ERRMODE_DEFAULT ) ||
       ( imode == HB_MATH_ERRMODE_CDEFAULT ) ||
       ( imode == HB_MATH_ERRMODE_USER ) ||
       ( imode == HB_MATH_ERRMODE_USERDEFAULT ) || ( imode == HB_MATH_ERRMODE_USERCDEFAULT ) )
   {
      s_hb_matherr_mode = imode;
   }

   return oldmode;
}

/* get error handling mode of hb_matherr() */
int hb_mathGetErrMode( void )
{
   HB_TRACE( HB_TR_DEBUG, ( "hb_mathGetErrMode()" ) );
   return s_hb_matherr_mode;
}

/* Harbour equivalent to mathSet/GetErrMode */
HB_FUNC( HB_MATHERMODE )        /* ([<nNewMode>]) -> <nOldMode> */
{
   hb_retni( hb_mathGetErrMode() );

   /* set new mode */
   if( ISNUM( 1 ) )
      hb_mathSetErrMode( hb_parni( 1 ) );
}

/* Harbour default math error handling routine */
int hb_matherr( HB_MATH_EXCEPTION * pexc )
{
   int mode = hb_mathGetErrMode();
   int iRet = 1;

   HB_TRACE( HB_TR_DEBUG, ( "hb_matherr(%p)", pexc ) );

   if( pexc == NULL || pexc->handled != 0 )
   {
      /* error already handled by other handlers ! */
      return 1;
   }

   if( mode == HB_MATH_ERRMODE_USER || mode == HB_MATH_ERRMODE_USERDEFAULT ||
       mode == HB_MATH_ERRMODE_USERCDEFAULT )
   {
      PHB_ITEM pArg1, pArg2, pError;
      PHB_ITEM pMatherrResult;

      /* create an error object */
      /* NOTE: In case of HB_MATH_ERRMODE_USER[C]DEFAULT, I am setting both EF_CANSUBSTITUTE and EF_CANDEFAULT to .T. here.
         This is forbidden according to the original Cl*pper docs, but I think this reflects the situation best here:
         The error handler can either substitute the errorneous value (by returning a numeric value) or choose the
         default error handling (by returning .F., as usual) [martin vogel] */
      pError = hb_errRT_New_Subst( ES_ERROR, "MATH", EG_NUMERR, pexc->type,
                                   pexc->error, pexc->funcname, 0, EF_CANSUBSTITUTE |
                                   ( mode == HB_MATH_ERRMODE_USER ? 0 : EF_CANDEFAULT ) );

      /* Assign the new array to the object data item. */
      /* NOTE: Unfortunately, we cannot decide whether one or two parameters have been used when the
         math function has been called, so we always take two */
      pArg1 = hb_itemPutND( NULL, pexc->arg1 );
      pArg2 = hb_itemPutND( NULL, pexc->arg2 );
      hb_errPutArgs( pError, 2, pArg1, pArg2 );
      hb_itemRelease( pArg1 );
      hb_itemRelease( pArg2 );

      /* launch error codeblock */
      pMatherrResult = hb_errLaunchSubst( pError );
      hb_errRelease( pError );

      if( pMatherrResult != NULL )
      {
         if( HB_IS_NUMERIC( pMatherrResult ) )
         {
            pexc->retval = hb_itemGetND( pMatherrResult );
            hb_itemGetNLen( pMatherrResult, &pexc->retvalwidth, &pexc->retvaldec );
            pexc->handled = 1;
         }
         hb_itemRelease( pMatherrResult );
      }
   }

   /* math exception not handled by Harbour error routine above ? */
   if( pexc->handled == 0 )
   {
      switch( mode )
      {
         case HB_MATH_ERRMODE_USER:
            /* user failed to handle the math exception, so quit the app [yes, that's the meaning of this mode !!] */
            iRet = 0;
            hb_vmRequestQuit();
            break;

         case HB_MATH_ERRMODE_DEFAULT:
         case HB_MATH_ERRMODE_USERDEFAULT:
            /* return 1 to suppress C RTL error msgs, but leave error handling to the calling Harbour routine */
            break;

         case HB_MATH_ERRMODE_CDEFAULT:
         case HB_MATH_ERRMODE_USERCDEFAULT:
            /* use the correction value supplied in pexc->retval */
            pexc->handled = 1;
            break;
      }
   }

   return iRet;                 /* error handling successful */
}


/*
 * ************************************************************
 * Harbour Math functions Part III:
 * (de)installing and (de)activating custom math error handlers
 * ************************************************************
 */

/* static slot for current math error handler, this is hb_matherr by default */
static HB_MATH_HANDLERPROC s_mathHandlerProc = hb_matherr;      /* TODO: make this thread safe */

/* install a harbour-like math error handler (that will be called by the matherr() function), return old handler */
HB_MATH_HANDLERPROC hb_mathSetHandler( HB_MATH_HANDLERPROC handlerproc )
{
   HB_MATH_HANDLERPROC oldHandlerProc;

   HB_TRACE( HB_TR_DEBUG, ( "hb_mathSetHandler (%p)", handlerproc ) );

   oldHandlerProc = s_mathHandlerProc;
   s_mathHandlerProc = handlerproc;

   return oldHandlerProc;
}

/* get current harbour-like math error handler */
HB_MATH_HANDLERPROC hb_mathGetHandler( void )
{
   HB_TRACE( HB_TR_DEBUG, ( "hb_mathGetHandler ()" ) );

   return s_mathHandlerProc;
}

/*
 * ************************************************************
 * Harbour Math functions Part IV:
 * example of hb_mathSet/GetHandler: add a new math handler that
 * calls a given codeblock for every math error
 * ************************************************************
 */

static PHB_ITEM spMathErrorBlock = NULL;
static HB_MATH_HANDLERPROC sPrevMathHandler = NULL;

static int hb_matherrblock( HB_MATH_EXCEPTION * pexc )
{
   int retval;

   /* call codeblock for both case: handled and unhandled exceptions */

   if( spMathErrorBlock != NULL )
   {
      PHB_ITEM pArray, pRet;
      PHB_ITEM pType, pFuncname, pError, pArg1, pArg2, pRetval, pHandled;

      pType = hb_itemPutNI( NULL, pexc->type );
      pFuncname = hb_itemPutC( NULL, pexc->funcname );
      pError = hb_itemPutC( NULL, pexc->error );
      pArg1 = hb_itemPutND( NULL, pexc->arg1 );
      pArg2 = hb_itemPutND( NULL, pexc->arg2 );
      pRetval = hb_itemPutNDLen( NULL, pexc->retval, pexc->retvalwidth, pexc->retvaldec );
      pHandled = hb_itemPutL( NULL, pexc->handled );

      pArray = hb_itemArrayNew( 2 );
      hb_itemArrayPut( pArray, 1, pRetval );
      hb_itemArrayPut( pArray, 2, pHandled );

      /* launch error codeblock that can
         a) change the members of the array = {dRetval, lHandled} to set the return value of the math C RTL routine and
         the <exception handled flag> and it
         b) can return an integer value to set the return value of matherr().
         NOTE that these values are only used if lHandled was .F. and is set to .T. within the codeblock */
      pRet = hb_itemDo( spMathErrorBlock, 6, pType, pFuncname, pError, pArg1, pArg2, pArray );

      hb_itemRelease( pType );
      hb_itemRelease( pFuncname );
      hb_itemRelease( pError );
      hb_itemRelease( pArg1 );
      hb_itemRelease( pArg2 );
      hb_itemRelease( pRetval );
      hb_itemRelease( pHandled );

      if( pexc->handled )
      {
         /* math exception has already been handled, so codeblock call above was only informative */
         retval = 1;
      }
      else
      {
         /* exception handled by codeblock ? */
         pHandled = hb_itemArrayGet( pArray, 2 );
         if( pHandled != NULL )
         {
            pexc->handled = hb_itemGetL( pHandled );
            hb_itemRelease( pHandled );
         }

         if( pexc->handled )
         {
            /* YES ! */
            /* extract retval for math routine and matherr() */
            pRetval = hb_itemArrayGet( pArray, 1 );
            if( pRetval != NULL )
            {
               pexc->retval = hb_itemGetND( pRetval );
               hb_itemGetNLen( pRetval, &pexc->retvalwidth, &pexc->retvaldec );
               hb_itemRelease( pRetval );
            }
            if( pRet != NULL && HB_IS_NUMERIC( pRet ) )
            {
               retval = hb_itemGetNI( pRet );   /* block may also return 0 to force C math lib warnings */
               hb_itemRelease( pRet );
            }
            else
            {
               retval = 1;      /* default return value to suppress C math lib warnings */
            }
         }
         else
         {
            /* NO ! */
            retval = 1;
         }
      }
      hb_itemRelease( pArray );
   }
   else
   {
      retval = 1;               /* default return value to suppress C math lib warnings */
   }

   if( sPrevMathHandler != NULL )
   {
      if( pexc->handled )
      {
         /* the error is handled, so simply inform the previous handler */
         ( *sPrevMathHandler ) ( pexc );
      }
      else
      {
         /* else go on error handling within previous handler */
         retval = ( *sPrevMathHandler ) ( pexc );
      }
   }
   return retval;
}

/* set/get math error block */
HB_FUNC( HB_MATHERBLOCK )       /* ([<nNewErrorBlock>]) -> <nOldErrorBlock> */
{

   /* immediately install hb_matherrblock and keep it permanently installed !
      This is not dangerous because hb_matherrorblock will always call the previous error handler */
   if( sPrevMathHandler == NULL )
   {
      sPrevMathHandler = hb_mathSetHandler( hb_matherrblock );
   }

   /* return old math handler */
   if( spMathErrorBlock == NULL )
   {
      hb_ret();
   }
   else
   {
      hb_itemReturn( spMathErrorBlock );
   }

   if( hb_pcount() > 0 )
   {
      /* set new error block */
      PHB_ITEM pNewErrorBlock = hb_param( 1, HB_IT_BLOCK );

      if( pNewErrorBlock != NULL )
      {
         if( spMathErrorBlock == NULL )
         {
            spMathErrorBlock = hb_itemNew( NULL );
         }
         hb_itemCopy( spMathErrorBlock, pNewErrorBlock );
      }
      else
      {
         /* a parameter other than a block has been passed -> delete error handler ! */
         if( spMathErrorBlock )
         {
            hb_itemRelease( spMathErrorBlock );
            spMathErrorBlock = NULL;
         }
      }
   }
}

/*
 * ************************************************************
 * Harbour Math functions Part V:
 * EXP(), LOG(), SQRT()
 * ************************************************************
 */

HB_FUNC( EXP )
{
   if( ISNUM( 1 ) )
   {
      HB_MATH_EXCEPTION hb_exc;
      double dResult, dArg = hb_parnd( 1 );

      hb_mathResetError( &hb_exc );
      dResult = exp( dArg );
      if( hb_mathGetError( &hb_exc, "EXP", dArg, 0.0, dResult ) )
      {
         if( hb_exc.handled )
            hb_retndlen( hb_exc.retval, hb_exc.retvalwidth, hb_exc.retvaldec );
         else
         {
            /* math exception is up to the Harbour function, so do this as Clipper compatible as possible */
            if( hb_exc.type == HB_MATH_ERR_OVERFLOW )
               hb_retndlen( HUGE_VAL, -1, -1 );
            else
               hb_retnd( 0.0 );
         }
      }
      else
         hb_retnd( dResult );
   }
   else
   {
      hb_errRT_BASE_SubstR( EG_ARG, 1096, NULL, "EXP", HB_ERR_ARGS_BASEPARAMS );
   }
}

HB_FUNC( LOG )
{
   if( ISNUM( 1 ) )
   {
      HB_MATH_EXCEPTION hb_exc;
      double dResult, dArg = hb_parnd( 1 );

      if( dArg <= 0 )
         hb_retndlen( -HUGE_VAL, -1, -1 );  /* return -infinity */
      else
      {
         hb_mathResetError( &hb_exc );
         dResult = log( dArg );
         if( hb_mathGetError( &hb_exc, "LOG", dArg, 0.0, dResult ) )
         {
            if( hb_exc.handled )
               hb_retndlen( hb_exc.retval, hb_exc.retvalwidth, hb_exc.retvaldec );
            else
            {
               /* math exception is up to the Harbour function, so do this as Clipper compatible as possible */
               switch( hb_exc.type )
               {
                  case HB_MATH_ERR_SING:       /* argument to log was 0.0 */
                  case HB_MATH_ERR_DOMAIN:     /* argument to log was < 0.0 */
                     hb_retndlen( -HUGE_VAL, -1, -1 );  /* return -infinity */
                     break;

                  default:
                     hb_retnd( 0.0 );
                     break;
               }
            }
         }
         else
            hb_retnd( dResult );
      }
   }
   else
      hb_errRT_BASE_SubstR( EG_ARG, 1095, NULL, "LOG", HB_ERR_ARGS_BASEPARAMS );
}

HB_FUNC( SQRT )
{
   if( ISNUM( 1 ) )
   {
      HB_MATH_EXCEPTION hb_exc;
      double dResult, dArg = hb_parnd( 1 );

      if( dArg <= 0 )
         hb_retnd( 0.0 );
      else
      {
         hb_mathResetError( &hb_exc );
         dResult = sqrt( dArg );
         if( hb_mathGetError( &hb_exc, "SQRT", dArg, 0.0, dResult ) )
         {
            if( hb_exc.handled )
               hb_retndlen( hb_exc.retval, hb_exc.retvalwidth, hb_exc.retvaldec );
            else
               /* math exception is up to the Harbour function, so do this as Clipper compatible as possible */
               hb_retnd( 0.0 ); /* return 0.0 on all errors (all (?) of type DOMAIN) */
         }
         else
            hb_retnd( dResult );
      }
   }
   else
      hb_errRT_BASE_SubstR( EG_ARG, 1097, NULL, "SQRT", HB_ERR_ARGS_BASEPARAMS );
}
