/*
 * $Id$
 */

/*
 * Harbour Project source code:
 * Compiler main file
 *
 * Copyright 1999 Antonio Linares <alinares@fivetechsoft.com>
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
 * The following parts are Copyright of the individual authors.
 * www - http://www.harbour-project.org
 *
 * Copyright 2000 RonPinkas <Ron@Profit-Master.com>
 *    hb_compPrepareOptimize()
 *    hb_compOptimizeJumps()
 *    hb_compOptimizeFrames()
 *    hb_compAutoOpenAdd()
 *    hb_compAutoOpenFind()
 *    hb_compAutoOpen()
 *    hb_compDeclaredParameterAdd()
 *    hb_compClassAdd()
 *    hb_compClassFind()
 *    hb_compMethodAdd()
 *    hb_compMethodFind()
 *    hb_compDeclaredAdd()
 *    hb_compDeclaredInit()
 *
 * See doc/license.txt for licensing terms.
 *
 */

/* malloc.h has been obsoleted by stdlib.h, which is included via hbcomp.h
#include <malloc.h> 
*/

/*
 * Avoid tracing in preprocessor/compiler.
 */
#if ! defined(HB_TRACE_UTILS)
   #if defined(HB_TRACE_LEVEL)
      #undef HB_TRACE_LEVEL
   #endif
#endif

#include "hbcomp.h"
#include "hbhash.h"

#if defined(HB_OS_DOS) && defined(__BORLANDC__)
   #include <limits.h>
   extern unsigned _stklen = UINT_MAX;
#endif

#define HB_COMP_SINGLEFILE    1
#define HB_COMP_AUTOADDFILE   2
#define HB_COMP_MEMBUFFER     3

static int hb_compCompile( HB_COMP_DECL, const char * szPrg, int iFileType );
static int hb_compProcessRSPFile( HB_COMP_DECL, char * ); /* process response file */
static int hb_compAutoOpen( HB_COMP_DECL, const char * szPrg, BOOL * bSkipGen, int iFileType );

static void hb_compDeclaredInit( HB_COMP_DECL );


/* ************************************************************************* */

int hb_compMain( int argc, char * argv[], BYTE ** pBufPtr, ULONG * pulSize,
                 const char * szSource )
{
   HB_COMP_DECL;
   int iStatus = EXIT_SUCCESS;
   BOOL bAnyFiles = FALSE;
   int i;

   HB_TRACE(HB_TR_DEBUG, ("hb_compMain()"));

   HB_COMP_PARAM = hb_comp_new();

   HB_COMP_PARAM->pOutPath = NULL;

   /* First check the environment variables */
   hb_compChkCompilerSwitch( HB_COMP_PARAM, 0, NULL );

   /* Then check command line arguments
      This will override duplicated environment settings */
   hb_compChkCompilerSwitch( HB_COMP_PARAM, argc, argv );
   if( !HB_COMP_PARAM->fExit )
   {
      if( pBufPtr && pulSize )
      {
         HB_COMP_PARAM->iLanguage = HB_LANG_PORT_OBJ_BUF;
      }

      if( HB_COMP_PARAM->fLogo )
         hb_compPrintLogo( HB_COMP_PARAM );

      if( HB_COMP_PARAM->fBuildInfo )
      {
         hb_compOutStd( HB_COMP_PARAM, "\n" );
         hb_verBuildInfo();
         hb_comp_free( HB_COMP_PARAM );
         return iStatus;
      }

      if( HB_COMP_PARAM->fCredits )
      {
         hb_compPrintCredits( HB_COMP_PARAM );
         hb_comp_free( HB_COMP_PARAM );
         return iStatus;
      }

      /* Set Search Path */
      hb_compChkPaths( HB_COMP_PARAM );

      /* Set standard rules */
      hb_compInitPP( HB_COMP_PARAM, argc, argv );

      /* Prepare the table of identifiers */
      hb_compIdentifierOpen( HB_COMP_PARAM );

      /* Load standard Declarations. */
      if( HB_COMP_PARAM->iWarnings >= 3 )
         hb_compDeclaredInit( HB_COMP_PARAM );
   }

   if( szSource )
   {
      bAnyFiles = TRUE;
      iStatus = hb_compCompile( HB_COMP_PARAM, szSource, HB_COMP_MEMBUFFER );
   }
   else
   {
      /* Process all files passed via the command line. */
      for( i = 1; i < argc && !HB_COMP_PARAM->fExit; i++ )
      {
         HB_TRACE(HB_TR_DEBUG, ("main LOOP(%i,%s)", i, argv[i]));
         if( ! HB_ISOPTSEP( argv[ i ][ 0 ] ) )
         {
            bAnyFiles = TRUE;
            if( argv[ i ][ 0 ] == '@' )
               iStatus = hb_compProcessRSPFile( HB_COMP_PARAM, argv[ i ] + 1 );
            else
               iStatus = hb_compCompile( HB_COMP_PARAM, argv[ i ], HB_COMP_SINGLEFILE );
            if( iStatus != EXIT_SUCCESS )
               break;
         }
      }
   }

   if( ! bAnyFiles && ! HB_COMP_PARAM->fQuiet )
   {
      hb_compPrintUsage( HB_COMP_PARAM, argv[ 0 ] );
      iStatus = EXIT_FAILURE;
   }

   if( HB_COMP_PARAM->iErrorCount > 0 )
      iStatus = EXIT_FAILURE;

   if( pBufPtr && pulSize )
   {
      if( iStatus == EXIT_SUCCESS )
      {
         * pBufPtr = HB_COMP_PARAM->pOutBuf;
         * pulSize = HB_COMP_PARAM->ulOutBufSize;
         HB_COMP_PARAM->pOutBuf = NULL;
         HB_COMP_PARAM->ulOutBufSize = 0;
      }
      else
      {
         * pBufPtr = NULL;
         * pulSize = 0;
      }
   }

   hb_comp_free( HB_COMP_PARAM );

   return iStatus;
}

static int hb_compProcessRSPFile( HB_COMP_DECL, char * szRspName )
{
   char szFile[ _POSIX_PATH_MAX + 1 ];
   int iStatus = EXIT_SUCCESS;
   PHB_FNAME pFileName;
   FILE *inFile;

   pFileName = hb_fsFNameSplit( szRspName );

   if( !pFileName->szExtension )
   {
      pFileName->szExtension = ".clp";
      hb_fsFNameMerge( szFile, pFileName );
      szRspName = szFile;
   }

   inFile = hb_fopen( szRspName, "r" );
   if( !inFile )
   {
      char buffer[ _POSIX_PATH_MAX + 80 ];
      snprintf( buffer, sizeof( buffer ),
                "Cannot open input file: %s\n", szRspName );
      hb_compOutErr( HB_COMP_PARAM, buffer );
      iStatus = EXIT_FAILURE;
   }
   else
   {
      int i = 0, ch;
      BOOL bAutoOpen = HB_COMP_PARAM->fAutoOpen;

      HB_COMP_PARAM->fAutoOpen = TRUE;

      do
      {
         ch = fgetc( inFile );

         /*
          * '"' - quoting file names is Harbour extension - 
          * Clipper does not serve it, [druzus]
          */
         if( ch == '"' )
         {
            while( ( ch = fgetc ( inFile ) ) != EOF && ch != '"' && ch != '\n' )
            {
               if( i < _POSIX_PATH_MAX )
                  szFile[ i++ ] = (char) ch;
            }
            if( ch == '"' )
               continue;
         }

         while( i == 0 && HB_ISSPACE( ch ) )
            ch = fgetc( inFile );
            
         if( ch == EOF || HB_ISSPACE( ch ) || ch == '#' )
         {
            szFile[ i ] = '\0';

            if( i > 0 )
               hb_compAutoOpenAdd( HB_COMP_PARAM,
                        hb_compIdentifierNew( HB_COMP_PARAM, szFile, HB_IDENT_COPY ) );
            i = 0;
            while( ch != EOF && ch != '\n' )
               ch = fgetc( inFile );
         }
         else if( i < _POSIX_PATH_MAX )
            szFile[ i++ ] = (char) ch;
      }
      while( ch != EOF );

      fclose( inFile );

      HB_COMP_PARAM->fAutoOpen = bAutoOpen;
      
      hb_fsFNameMerge( szFile, pFileName );
      hb_compCompile( HB_COMP_PARAM, szFile, HB_COMP_AUTOADDFILE );
   }

   hb_xfree( pFileName );

   return iStatus;
}

/* ------------------------------------------------------------------------- */
/*                           ACTIONS                                         */
/* ------------------------------------------------------------------------- */


static PCOMSYMBOL hb_compSymbolAdd( HB_COMP_DECL, char * szSymbolName, USHORT * pwPos, BOOL bFunction )
{
   PCOMSYMBOL pSym;

   if( szSymbolName[ 0 ] )
   {
      /* Create a symbol for non-empty names only.
       * NOTE: an empty name is passed for a fake starting function when
       * '-n' switch is used
       */
      pSym = ( PCOMSYMBOL ) hb_xgrab( sizeof( COMSYMBOL ) );

      pSym->szName = szSymbolName;
      pSym->cScope = 0;
      pSym->pNext = NULL;
      pSym->bFunc = bFunction;

      if( ! HB_COMP_PARAM->symbols.iCount )
      {
         HB_COMP_PARAM->symbols.pFirst = pSym;
         HB_COMP_PARAM->symbols.pLast  = pSym;
      }
      else
      {
         ( ( PCOMSYMBOL ) HB_COMP_PARAM->symbols.pLast )->pNext = pSym;
         HB_COMP_PARAM->symbols.pLast = pSym;
      }
      HB_COMP_PARAM->symbols.iCount++;

      if( pwPos )
         *pwPos = HB_COMP_PARAM->symbols.iCount -1; /* position number starts form 0 */
   }
   else
      pSym = NULL;

   return pSym;
}

static PCOMSYMBOL hb_compSymbolFind( HB_COMP_DECL, char * szSymbolName, USHORT * pwPos, BOOL bFunction )
{
   PCOMSYMBOL pSym = HB_COMP_PARAM->symbols.pFirst;
   USHORT wCnt = 0;

   if( pwPos )
      *pwPos = 0;
   while( pSym )
   {
      if( ! strcmp( pSym->szName, szSymbolName ) )
      {
         if( bFunction == pSym->bFunc )
         {
            if( pwPos )
               *pwPos = wCnt;
            return pSym;
         }
      }

      if( pSym->pNext )
      {
         pSym = pSym->pNext;
         ++wCnt;
      }
      else
         return NULL;
   }
   return NULL;
}

/* NOTE: Name of symbols are released in hbident.c  on exit */
static PCOMSYMBOL hb_compSymbolKill( PCOMSYMBOL pSym )
{
   PCOMSYMBOL pNext = pSym->pNext;

   hb_xfree( ( void * ) pSym );

   return pNext;
}

/* returns a symbol name based on its index on the symbol table
 * index starts from 0
 */
char * hb_compSymbolName( HB_COMP_DECL, USHORT uiSymbol )
{
   PCOMSYMBOL pSym = HB_COMP_PARAM->symbols.pFirst;

   while( pSym )
   {
      if( uiSymbol-- == 0 )
         return pSym->szName;
      pSym = pSym->pNext;
   }
   return NULL;
}

static void hb_compCheckDuplVars( HB_COMP_DECL, PVAR pVar, char * szVarName )
{
   while( pVar )
   {
      if( ! strcmp( pVar->szName, szVarName ) )
      {
         HB_COMP_ERROR_DUPLVAR( szVarName );
         break;
      }
      else
         pVar = pVar->pNext;
   }
}

static USHORT hb_compVarListAdd( PVAR * pVarLst, PVAR pVar )
{
   USHORT uiVar = 1;
   while( *pVarLst )
   {
      pVarLst = &( *pVarLst )->pNext;
      ++uiVar;
   }
   *pVarLst = pVar;

   return uiVar;
}

void hb_compVariableAdd( HB_COMP_DECL, char * szVarName, BYTE cValueType )
{
   PFUNCTION pFunc = HB_COMP_PARAM->functions.pLast;
   PVAR pVar;
   BOOL bFreeVar = TRUE;
   
   HB_SYMBOL_UNUSED( cValueType );

   if( ! HB_COMP_PARAM->fStartProc && HB_COMP_PARAM->functions.iCount <= 1 &&
       ( HB_COMP_PARAM->iVarScope == VS_LOCAL ||
         HB_COMP_PARAM->iVarScope == ( VS_PRIVATE | VS_PARAMETER ) ) )
   {
      /* Variable declaration is outside of function/procedure body.
         In this case only STATIC and PARAMETERS variables are allowed. */
      hb_compGenError( HB_COMP_PARAM, hb_comp_szErrors, 'E', HB_COMP_ERR_OUTSIDE, NULL, NULL );
      return;
   }

   /* check if we are declaring local/static variable after some
    * executable statements
    */
   if( pFunc->bFlags & FUN_STATEMENTS )
   {
      char * szVarScope;
      switch( HB_COMP_PARAM->iVarScope )
      {
         case VS_LOCAL:
            szVarScope = "LOCAL";
            break;
         case VS_STATIC:
            szVarScope = "STATIC";
            break;
         case VS_FIELD:
            szVarScope = "FIELD";
            break;
         case VS_MEMVAR:
            szVarScope = "MEMVAR";
            break;
         default:
            szVarScope = NULL;
      }
      if( szVarScope )
      {
         hb_compGenError( HB_COMP_PARAM, hb_comp_szErrors, 'E', HB_COMP_ERR_FOLLOWS_EXEC, szVarScope, NULL );
         return;
      }
   }

   /* Check if a declaration of duplicated variable name is requested */
   if( pFunc->szName )
   {
      /* variable defined in a function/procedure */
      hb_compCheckDuplVars( HB_COMP_PARAM, pFunc->pFields, szVarName );
      hb_compCheckDuplVars( HB_COMP_PARAM, pFunc->pStatics, szVarName );
      /* NOTE: Clipper warns if PARAMETER variable duplicates the MEMVAR
       * declaration
       */
      if( !( HB_COMP_PARAM->iVarScope == VS_PRIVATE ||
             HB_COMP_PARAM->iVarScope == VS_PUBLIC ) )
         hb_compCheckDuplVars( HB_COMP_PARAM, pFunc->pMemvars, szVarName );
   }
   else if( pFunc->bFlags & FUN_EXTBLOCK )
   {
      /* variable defined in an extended codeblock */
      hb_compCheckDuplVars( HB_COMP_PARAM, pFunc->pFields, szVarName );
      hb_compCheckDuplVars( HB_COMP_PARAM, pFunc->pStatics, szVarName );
   }
   else if( HB_COMP_PARAM->iVarScope != VS_PARAMETER )
   {
      char buffer[ 80 ];
      snprintf( buffer, sizeof( buffer ),
                "Wrong type of codeblock parameter, is: %d, should be: %d\r\n",
                HB_COMP_PARAM->iVarScope, VS_PARAMETER );
      hb_compOutErr( HB_COMP_PARAM, buffer );
      /* variable defined in a codeblock */
      HB_COMP_PARAM->iVarScope = VS_PARAMETER;
   }

   hb_compCheckDuplVars( HB_COMP_PARAM, pFunc->pLocals, szVarName );

   pVar = ( PVAR ) hb_xgrab( sizeof( VAR ) );
   pVar->szName = szVarName;
   pVar->szAlias = NULL;
   pVar->cType = cValueType;
   pVar->iUsed = VU_NOT_USED;
   pVar->pNext = NULL;
   pVar->iDeclLine = HB_COMP_PARAM->currLine;

   if( toupper( cValueType ) == 'S' )
   {
      /* printf( "\nVariable %s is of Class: %s\n", szVarName, HB_COMP_PARAM->szFromClass ); */
      pVar->pClass = hb_compClassFind( HB_COMP_PARAM, HB_COMP_PARAM->szFromClass );
      if( ! pVar->pClass )
      {
         hb_compGenWarning( HB_COMP_PARAM, hb_comp_szWarnings, 'W', HB_COMP_WARN_CLASS_NOT_FOUND, HB_COMP_PARAM->szFromClass, szVarName );
         pVar->cType = 'O';
      }
      /* Resetting */
      HB_COMP_PARAM->szFromClass = NULL;
   }

   if( HB_COMP_PARAM->iVarScope & VS_PARAMETER )
      pVar->iUsed = VU_INITIALIZED;

   if( HB_COMP_PARAM->iVarScope & VS_MEMVAR )
   {
      PCOMSYMBOL pSym;
      USHORT wPos;

      if( HB_COMP_PARAM->fAutoMemvarAssume || HB_COMP_PARAM->iVarScope == VS_MEMVAR )
      {
         /* add this variable to the list of MEMVAR variables
          */
         if( pFunc->pMemvars )
            hb_compCheckDuplVars( HB_COMP_PARAM, pFunc->pMemvars, szVarName );

         hb_compVarListAdd( &pFunc->pMemvars, pVar );
         bFreeVar = FALSE;
      }

      switch( HB_COMP_PARAM->iVarScope )
      {
         case VS_MEMVAR:
            /* variable declared in MEMVAR statement */
            break;

         case( VS_PARAMETER | VS_PRIVATE ):
            if( ++pFunc->wParamNum > pFunc->wParamCount )
               pFunc->wParamCount = pFunc->wParamNum;

            pSym = hb_compSymbolFind( HB_COMP_PARAM, szVarName, &wPos, HB_SYM_MEMVAR ); /* check if symbol exists already */
            if( ! pSym )
               pSym = hb_compSymbolAdd( HB_COMP_PARAM, szVarName, &wPos, HB_SYM_MEMVAR );
            pSym->cScope |= HB_FS_MEMVAR;

            hb_compGenPCode4( HB_P_PARAMETER, HB_LOBYTE( wPos ), HB_HIBYTE( wPos ), HB_LOBYTE( pFunc->wParamNum ), HB_COMP_PARAM );

            if( HB_COMP_PARAM->iWarnings >= 3 && bFreeVar )
            {
               PVAR pMemVar = pFunc->pMemvars;
               while( pMemVar && strcmp( pMemVar->szName, pVar->szName ) != 0 )
                  pMemVar = pMemVar->pNext;
               /* Not declared as memvar. */
               if( pMemVar == NULL )
               {
                  /* add this variable to the list of PRIVATE variables. */
                  hb_compVarListAdd( &pFunc->pPrivates, pVar );
                  bFreeVar = FALSE;
               }
            }
            if( bFreeVar )
               hb_xfree( pVar );
            break;

         case VS_PRIVATE:
            pSym = hb_compSymbolFind( HB_COMP_PARAM, szVarName, &wPos, HB_SYM_MEMVAR ); /* check if symbol exists already */
            if( ! pSym )
               pSym = hb_compSymbolAdd( HB_COMP_PARAM, szVarName, &wPos, HB_SYM_MEMVAR );
            pSym->cScope |= HB_FS_MEMVAR;

            if( HB_COMP_PARAM->iWarnings >= 3 && bFreeVar )
            {
               PVAR pMemVar = pFunc->pMemvars;
               while( pMemVar && strcmp( pMemVar->szName, pVar->szName ) != 0 )
                  pMemVar = pMemVar->pNext;
               /* Not declared as memvar. */
               if( pMemVar == NULL )
               {
                  /* add this variable to the list of PRIVATE variables. */
                  hb_compVarListAdd( &pFunc->pPrivates, pVar );
                  bFreeVar = FALSE;
               }
            }
            if( bFreeVar )
               hb_xfree( pVar );
            break;

         case VS_PUBLIC:
            pSym = hb_compSymbolFind( HB_COMP_PARAM, szVarName, &wPos, HB_SYM_MEMVAR ); /* check if symbol exists already */
            if( ! pSym )
               pSym = hb_compSymbolAdd( HB_COMP_PARAM, szVarName, &wPos, HB_SYM_MEMVAR );
            pSym->cScope |= HB_FS_MEMVAR;
            if( bFreeVar )
               hb_xfree( pVar );
            break;
      }
   }
   else
   {
      switch( HB_COMP_PARAM->iVarScope )
      {
         case VS_LOCAL:
         case VS_PARAMETER:
         {
            USHORT wLocal = hb_compVarListAdd( &pFunc->pLocals, pVar );

            if( HB_COMP_PARAM->iVarScope == VS_PARAMETER )
            {
               ++pFunc->wParamCount;
               pFunc->bFlags |= FUN_USES_LOCAL_PARAMS;
            }
            if( HB_COMP_PARAM->fDebugInfo )
            {
               hb_compGenPCode3( HB_P_LOCALNAME, HB_LOBYTE( wLocal ), HB_HIBYTE( wLocal ), HB_COMP_PARAM );
               hb_compGenPCodeN( ( BYTE * ) szVarName, strlen( szVarName ) + 1, HB_COMP_PARAM );
            }
            break;
         }
         case VS_STATIC:
            hb_compVarListAdd( &pFunc->pStatics, pVar );
            break;

         case VS_FIELD:
            hb_compVarListAdd( &pFunc->pFields, pVar );
            break;
      }
   }
}

/* Set the name of an alias for the list of previously declared FIELDs
 *
 * szAlias -> name of the alias
 * iField  -> position of the first FIELD name to change
 */
void hb_compFieldSetAlias( HB_COMP_DECL, char * szAlias, int iField )
{
   PVAR pVar;

   pVar = HB_COMP_PARAM->functions.pLast->pFields;
   while( iField-- && pVar )
      pVar = pVar->pNext;

   while( pVar )
   {
      pVar->szAlias = szAlias;
      pVar = pVar->pNext;
   }
}

/* This functions counts the number of FIELD declaration in a function
 * We will required this information in hb_compFieldSetAlias function
 */
int hb_compFieldsCount( HB_COMP_DECL )
{
   int iFields = 0;
   PVAR pVar = HB_COMP_PARAM->functions.pLast->pFields;

   while( pVar )
   {
      ++iFields;
      pVar = pVar->pNext;
   }

   return iFields;
}

static PVAR hb_compVariableGet( PVAR pVars, char * szVarName, int * piPos )
{
   int iVar = 1;

   while( pVars )
   {
      if( pVars->szName && ! strcmp( pVars->szName, szVarName ) )
      {
         pVars->iUsed |= VU_USED;
         *piPos = iVar;
         return pVars;
      }
      pVars = pVars->pNext;
      ++iVar;
   }
   return NULL;
}

/* returns variable pointer if defined or NULL */
static PVAR hb_compVariableGetVar( PVAR pVars, USHORT wOrder )
{
   while( pVars && --wOrder )
      pVars = pVars->pNext;
   return pVars;
}

/* returns the order + 1 of a variable if defined or zero */
static USHORT hb_compVariableGetPos( PVAR pVars, char * szVarName )
{
   USHORT wVar = 1;

   while( pVars )
   {
      if( pVars->szName && ! strcmp( pVars->szName, szVarName ) )
      {
         pVars->iUsed |= VU_USED;
         return wVar;
      }
      pVars = pVars->pNext;
      ++wVar;
   }
   return 0;
}

PVAR hb_compVariableFind( HB_COMP_DECL, char * szVarName, int * piPos, int * piScope )
{
   PFUNCTION pFunc, pGlobal, pOutBlock = NULL;
   BOOL fStatic = FALSE, fBlock = FALSE, fGlobal = FALSE;
   PVAR pVar = NULL;
   int iPos = 0, iScope = 0, iLevel = 0;

   if( piPos )
      *piPos = 0;
   else
      piPos = &iPos;
   if( piScope )
      *piScope = HB_VS_UNDECLARED;
   else
      piScope = &iScope;

   /* check current function/codeblock variables */
   pFunc = HB_COMP_PARAM->functions.pLast;
   pGlobal = ( HB_COMP_PARAM->fStartProc ||
               HB_COMP_PARAM->functions.pFirst == pFunc )
             ? NULL : HB_COMP_PARAM->functions.pFirst;

   while( pFunc )
   {
      if( ( pFunc->cScope & HB_FS_INITEXIT ) == HB_FS_INITEXIT )
      {  /* static initialization function */
         fStatic = TRUE;
      }
      else if( pFunc->szName )
      {  /* normal function/procedure */
         /* check local parameters */
         pVar = hb_compVariableGet( pFunc->pLocals, szVarName, piPos );
         if( pVar )
         {
            *piScope = HB_VS_LOCAL_VAR;
            if( fStatic )
            {
               /* local variable was referenced in a codeblock during
                * initialization of static variable. This cannot be supported
                * because static variables are initialized at program
                * startup when there is no local variables yet - hence we
                * cannot detach this local variable
                * For example:
                * LOCAL locvar
                * STATIC stavar:={ | x | locvar}
                *
                * NOTE: Clipper creates such a codeblock however at the
                * time of codeblock evaluation it generates a runtime error:
                * 'bound error: array acccess'
                * Called from: (b)STATICS$(0)
                */
               hb_compGenError( HB_COMP_PARAM, hb_comp_szErrors, 'E', HB_COMP_ERR_ILLEGAL_INIT, "(b)", szVarName );
            }
            else if( fBlock )
            {
               /* We want to access a local variable defined in a function
                * that owns this codeblock. We cannot access this variable in
                * a normal way because at runtime the stack base will point
                * to local variables of EVAL function.
                */
               /* NOTE: The list of local variables defined in a function
                * and referenced in a codeblock will be stored in a outer
                * codeblock only. This makes sure that all variables will be
                * detached properly - the inner codeblock can be created
                * outside of a function where it was defined when the local
                * variables are not accessible.
                */
               *piPos = -hb_compVariableGetPos( pOutBlock->pDetached, szVarName );
               if( *piPos == 0 )
               {
                  /* this variable was not referenced yet - add it to the list */
                  pVar = ( PVAR ) hb_xgrab( sizeof( VAR ) );

                  pVar->szName = szVarName;
                  pVar->szAlias = NULL;
                  pVar->cType = ' ';
                  pVar->iUsed = VU_NOT_USED;
                  pVar->pNext  = NULL;
                  pVar->iDeclLine = HB_COMP_PARAM->currLine;
                  /* Use negative order to signal that we are accessing a local
                   * variable from a codeblock
                   */
                  *piPos = -hb_compVarListAdd( &pOutBlock->pDetached, pVar );
               }
               *piScope = HB_VS_CBLOCAL_VAR;
            }
         }
         else
         {
            /* check static variables */
            pVar = hb_compVariableGet( pFunc->pStatics, szVarName, piPos );
            if( pVar )
            {
               *piScope = HB_VS_STATIC_VAR;
               *piPos += pFunc->iStaticsBase;
            }
            else
            {
               /* check FIELDs */
               pVar = hb_compVariableGet( pFunc->pFields, szVarName, piPos );
               if( pVar )
                  *piScope = HB_VS_LOCAL_FIELD;
               else
               {
                  /* check MEMVARs */
                  pVar = hb_compVariableGet( pFunc->pMemvars, szVarName, piPos );
                  if( pVar )
                     *piScope = HB_VS_LOCAL_MEMVAR;
               }
            }
         }
      }
      else
      {  /* codeblock */
         fBlock = TRUE;
         /* check local parameters */
         pVar = hb_compVariableGet( pFunc->pLocals, szVarName, piPos );
         if( pVar )
         {
            *piScope = HB_VS_LOCAL_VAR;
            if( iLevel )
            {
               /* this variable is defined in a parent codeblock
                * It is not possible to access a parameter of a codeblock
                * in which the current codeblock is defined
                */
               hb_compGenError( HB_COMP_PARAM, hb_comp_szErrors, 'E', HB_COMP_ERR_OUTER_VAR, szVarName, NULL );
            }
         }
         else if( pFunc->bFlags & FUN_EXTBLOCK )
         {  /* extended codeblock */
            /* check static variables */
            pVar = hb_compVariableGet( pFunc->pStatics, szVarName, piPos );
            if( pVar )
            {
               *piScope = HB_VS_STATIC_VAR;
               *piPos += pFunc->iStaticsBase;
            }
            else
            {
               /* check FIELDs */
               pVar = hb_compVariableGet( pFunc->pFields, szVarName, piPos );
               if( pVar )
                  *piScope = HB_VS_LOCAL_FIELD;
               else
               {
                  /* check MEMVARs */
                  pVar = hb_compVariableGet( pFunc->pMemvars, szVarName, piPos );
                  if( pVar )
                     *piScope = HB_VS_LOCAL_MEMVAR;
               }
            }
         }
      }

      if( pVar )
         break;

      pOutBlock = pFunc;
      pFunc = pFunc->pOwner;
      if( !pFunc && !fGlobal )
      {
         /* instead of making this trick with pGlobal switching it will be
          * much cleaner to set pOwner in each compiled function to first
          * global pseudo function created when -n compiler switch is used
          * [druzus]
          */
         pFunc = pGlobal;
         fGlobal = TRUE;
      }
      ++iLevel;
   }

   if( pVar && fGlobal )
      *piScope |= HB_VS_FILEWIDE;

   return pVar;
}

/* return local variable name using its order after final fixing */
char * hb_compLocalVariableName( PFUNCTION pFunc, USHORT wVar )
{
   PVAR pVar;

   if( pFunc->wParamCount && !( pFunc->bFlags & FUN_USES_LOCAL_PARAMS ) )
      wVar -= pFunc->wParamCount;
   pVar = hb_compVariableGetVar( pFunc->pLocals, wVar );

   return pVar ? pVar->szName : NULL;
}

char * hb_compStaticVariableName( HB_COMP_DECL, USHORT wVar )
{
   PVAR pVar;
   PFUNCTION pTmp = HB_COMP_PARAM->functions.pFirst;

   while( pTmp->pNext && pTmp->pNext->iStaticsBase < wVar )
      pTmp = pTmp->pNext;
   pVar = hb_compVariableGetVar( pTmp->pStatics, ( USHORT ) ( wVar - pTmp->iStaticsBase ) );

   return pVar ? pVar->szName : NULL;
}

int hb_compVariableScope( HB_COMP_DECL, char * szVarName )
{
   int iScope;

   hb_compVariableFind( HB_COMP_PARAM, szVarName, NULL, &iScope );

   return iScope;
}

BOOL hb_compIsValidMacroText( HB_COMP_DECL, char * szText, ULONG ulLen )
{
   BOOL fFound = FALSE;
   ULONG ul = 0;

   while( ul < ulLen )
   {
      if( szText[ ul++ ] == '&' )
      {
         char szSymName[ HB_SYMBOL_NAME_LEN + 1 ];
         int iSize = 0, iScope;

         /* Check if macro operator is used inside a string
          * Macro operator is ignored if it is the last char or
          * next char is '(' e.g. "this is &(ignored)"
          * (except if strict Clipper compatibility mode is enabled)
          *
          * NOTE: This uses _a-zA-Z pattern to check for
          * beginning of a variable name
          */

         while( ul < ulLen && iSize < HB_SYMBOL_NAME_LEN )
         {
            char ch = szText[ ul ];
            if( ch >= 'a' && ch <= 'z' )
               szSymName[ iSize++ ] = ch - ( 'a' - 'A' );
            else if( ch == '_' || ( ch >= 'A' && ch <= 'Z' ) ||
                                  ( ch >= '0' && ch <= '9' ) )
               szSymName[ iSize++ ] = ch;
            else
               break;
            ++ul;
         }
   
         if( iSize )
         {
            szSymName[ iSize ] = '\0';

            /* NOTE: All variables are assumed memvars in macro compiler -
             * there is no need to check for a valid name but to be Clipper
             * compatible we should check if macrotext variable does not refer
             * to local, static or field and generate error in such case.
             * Only MEMVAR or undeclared (memvar will be assumed)
             * variables can be used in macro text.
             */
            fFound = TRUE;
            iScope = hb_compVariableScope( HB_COMP_PARAM, szSymName );
            if( iScope != HB_VS_UNDECLARED && !( iScope & HB_VS_LOCAL_MEMVAR ) )
            {
               hb_compErrorMacro( HB_COMP_PARAM, szText );
               break;
            }
         }
         else if( ! HB_SUPPORT_HARBOUR )
            fFound = TRUE;    /* always macro substitution in Clipper */
      }
   }

   return fFound;
}



/*
 * DECLARATIONS
 */

PCOMCLASS hb_compClassFind( HB_COMP_DECL, char * szClassName )
{
   PCOMCLASS pClass = HB_COMP_PARAM->pFirstClass;

   if( HB_COMP_PARAM->iWarnings < 3 )
      return NULL;

   while( pClass )
   {
      if( ! strcmp( pClass->szName, szClassName ) )
         return pClass;
      pClass = pClass->pNext;
   }
   return NULL;
}

PCOMCLASS hb_compClassAdd( HB_COMP_DECL, char * szClassName, char * szClassFunc )
{
   PCOMCLASS pClass;
   PCOMDECLARED pDeclared;

   /*printf( "Declaring Class: %s\n", szClassName );*/

   if( HB_COMP_PARAM->iWarnings < 3 )
      return NULL;

   if( ( pClass = hb_compClassFind( HB_COMP_PARAM, szClassName ) ) != NULL )
   {
      PCOMCLASS * pClassPtr = &HB_COMP_PARAM->pFirstClass;

      while( *pClassPtr && *pClassPtr != HB_COMP_PARAM->pReleaseClass )
      {
         /* It's predefined class, allow to redefine them */
         if( *pClassPtr == pClass )
         {
            *pClassPtr = pClass->pNext;
            pClass = NULL;
            break;
         }
         pClassPtr = &( *pClassPtr )->pNext;
      }

      if( pClass )
      {
         hb_compGenWarning( HB_COMP_PARAM, hb_comp_szWarnings, 'W', HB_COMP_WARN_DUP_DECLARATION, "Class", szClassName );
         return pClass;
      }
   }

   pClass = ( PCOMCLASS ) hb_xgrab( sizeof( COMCLASS ) );

   pClass->szName = szClassName;
   pClass->pMethod = NULL;
   pClass->pNext = NULL;

   if( HB_COMP_PARAM->pFirstClass == NULL )
      HB_COMP_PARAM->pFirstClass = pClass;
   else
      HB_COMP_PARAM->pLastClass->pNext = pClass;

   HB_COMP_PARAM->pLastClass = pClass;

   /* Auto declaration for the Class Function. */
   pDeclared = hb_compDeclaredAdd( HB_COMP_PARAM, szClassFunc ? szClassFunc : szClassName );
   pDeclared->cType = 'S';
   pDeclared->pClass = pClass;
   
   if( ! HB_COMP_PARAM->pReleaseClass )
   {
      HB_COMP_PARAM->pReleaseClass = pClass;
   }
   
   return pClass;
}

PCOMDECLARED hb_compMethodFind( PCOMCLASS pClass, char * szMethodName )
{
   if( pClass )
   {
      PCOMDECLARED pMethod = pClass->pMethod;

      while( pMethod )
      {
         if( ! strcmp( pMethod->szName, szMethodName ) )
            return pMethod;
         pMethod = pMethod->pNext;
      }
   }

   return NULL;
}

PCOMDECLARED hb_compMethodAdd( HB_COMP_DECL, PCOMCLASS pClass, char * szMethodName )
{
   PCOMDECLARED pMethod;

   /*printf( "\nDeclaring Method: %s of Class: %s Pointer: %li\n", szMethodName, pClass->szName, pClass );*/

   if( HB_COMP_PARAM->iWarnings < 3 )
      return NULL;

   if( ( pMethod = hb_compMethodFind( pClass, szMethodName ) ) != NULL )
   {
      hb_compGenWarning( HB_COMP_PARAM, hb_comp_szWarnings, 'W', HB_COMP_WARN_DUP_DECLARATION, "Method", szMethodName );

      /* Last Declaration override previous declarations */
      pMethod->cParamTypes = NULL;
      pMethod->iParamCount = 0;
      pMethod->pParamClasses = NULL;

      return pMethod;
   }

   pMethod = ( PCOMDECLARED ) hb_xgrab( sizeof( COMDECLARED ) );

   pMethod->szName = szMethodName;
   pMethod->cType = ' '; /* Not known yet */
   pMethod->cParamTypes = NULL;
   pMethod->iParamCount = 0;
   pMethod->pParamClasses = NULL;
   pMethod->pNext = NULL;

   if( pClass->pMethod == NULL )
      pClass->pMethod = pMethod;
   else
      pClass->pLastMethod->pNext = pMethod;

   pClass->pLastMethod = pMethod;

   HB_COMP_PARAM->pLastMethod = pMethod;

   return pMethod;
}

static void hb_compDeclaredInit( HB_COMP_DECL )
{
  #define _DECL static COMDECLARED

  /*
    \x5c -> ByRef    (+60)             '-'  -> NIL
    \x7a -> Optional (+90)             'U'  -> Undefined

    ' '  -> AnyType                    'A'  -> Array                     'B'  -> Array
    'A'  -> Array of AnyType           'a'  -> Array of Arrays           'b'  -> Array of Blocks
    \x7a -> Optional AnyType           \x9b -> Optional Array            \x9c -> Optional Block
    \x94 -> Optional Array of AnyType  \xb5 -> Optional Array of Arrays  \xb6 -> Optional Array of Blocks

    'C'  -> Character/String           'D'  -> Date                      'L'  -> Logical
    'c'  -> Array of Strings           'd'  -> Array of Dates            'l'  -> Array of Logicals
    \x9d -> Optional Character         \x9e -> Optional Date             \xa6 -> Optional Logical
    \xb7 -> Optional Array of Strings  \xb8 -> Optional Array of Dates   \xc0 -> Optional Array of Logicals

    'N'  -> Numeric                    'O'  -> Object                    'S'  -> Class
    'n'  -> Array of Numerics          'o'  -> Array of Objects          's'  -> Array of Classes
    \xa8 -> Optional Numeric           \xa9 -> Optional Object           \xad -> Optional Class
    \xc2 -> Optional Array of Numerics \xc3 -> Optional Array of Objects \xc7 -> Optional Array of Classes
   */

   /* ------------------------------------------------- Standard Functions -------------------------------------------------- */

   /*              Name        Ret  # of Prams  Param Types                                                   Ret Class  Param Classes  Next
                   ----------  ---  ----------  ------------------------------                                ---------  -------------  ------ */
   _DECL s_001 = { "AADD"            , ' ', 2 , (BYTE*)"A "                                                   , NULL     , NULL , NULL  };
   _DECL s_002 = { "ABS"             , 'N', 1 , (BYTE*)"N"                                                    , NULL     , NULL , &s_001};
   _DECL s_003 = { "ACHOICE"         , 'N', 9 , (BYTE*)"NNNNA\x7a\x9d\xa8\xa8"                                , NULL     , NULL , &s_002};
   _DECL s_004 = { "ACLONE"          , 'A', 1 , (BYTE*)"A"                                                    , NULL     , NULL , &s_003};
   _DECL s_005 = { "ACOPY"           , 'A', 5 , (BYTE*)"AA\xa8\xa8\xa8"                                       , NULL     , NULL , &s_004};
   _DECL s_006 = { "ADEL"            , 'A', 2 , (BYTE*)"AN"                                                   , NULL     , NULL , &s_005};
   _DECL s_007 = { "ADIR"            , 'N', 6 , (BYTE*)"\x9d\x9b\x9b\x9b\x9b\x9b"                             , NULL     , NULL , &s_006};
   _DECL s_008 = { "AEVAL"           , 'A', 4 , (BYTE*)"AB\xa8\xa8"                                           , NULL     , NULL , &s_007};
   _DECL s_009 = { "AFIELDS"         , 'N', 4 , (BYTE*)"A\x9b\x9b\x9b"                                        , NULL     , NULL , &s_008};
   _DECL s_010 = { "AFILL"           , 'A', 4 , (BYTE*)"A \xa8\xa8"                                           , NULL     , NULL , &s_009};
   _DECL s_011 = { "AINS"            , 'A', 2 , (BYTE*)"AN"                                                   , NULL     , NULL , &s_010};
   _DECL s_012 = { "ALERT"           , 'N', 4 , (BYTE*)"C\x9b\x9d\xa8"                                        , NULL     , NULL , &s_011};
   _DECL s_013 = { "ALIAS"           , 'C', 1 , (BYTE*)"\xa8"                                                 , NULL     , NULL , &s_012};
   _DECL s_014 = { "ALLTRIM"         , 'C', 1 , (BYTE*)"C"                                                    , NULL     , NULL , &s_013};
   _DECL s_015 = { "AMPM"            , 'C', 1 , (BYTE*)"C"                                                    , NULL     , NULL , &s_014};
   _DECL s_016 = { "ARRAY"           , 'A', 3 , (BYTE*)"N\xa8\xa8"                                            , NULL     , NULL , &s_015};
   _DECL s_017 = { "ASC"             , 'N', 1 , (BYTE*)"C"                                                    , NULL     , NULL , &s_016};
   _DECL s_018 = { "ASCAN"           , 'N', 4 , (BYTE*)"A\xa7\xa8\xa8"                                        , NULL     , NULL , &s_017};
   _DECL s_019 = { "ASIZE"           , 'A', 2 , (BYTE*)"AN"                                                   , NULL     , NULL , &s_018};
   _DECL s_020 = { "ASORT"           , 'A', 4 , (BYTE*)"A\xa8\xa8\x9c"                                        , NULL     , NULL , &s_019};
   _DECL s_021 = { "AT"              , 'N', 2 , (BYTE*)"CC"                                                   , NULL     , NULL , &s_020};
   _DECL s_022 = { "ATAIL"           , ' ', 1 , (BYTE*)"A"                                                    , NULL     , NULL , &s_021};
   _DECL s_023 = { "BIN2I"           , 'N', 1 , (BYTE*)"C"                                                    , NULL     , NULL , &s_022};
   _DECL s_024 = { "BIN2L"           , 'N', 1 , (BYTE*)"C"                                                    , NULL     , NULL , &s_023};
   _DECL s_025 = { "BIN2U"           , 'N', 1 , (BYTE*)"C"                                                    , NULL     , NULL , &s_024};
   _DECL s_026 = { "BIN2W"           , 'N', 1 , (BYTE*)"C"                                                    , NULL     , NULL , &s_025};
   _DECL s_027 = { "BOF"             , 'L', 0 , (BYTE*)NULL                                                   , NULL     , NULL , &s_026};
   _DECL s_028 = { "BROWSE"          , '-', 4 , (BYTE*)"\xa8\xa8\xa8\xa8"                                     , NULL     , NULL , &s_027};
   _DECL s_029 = { "CDOW"            , 'C', 1 , (BYTE*)"D"                                                    , NULL     , NULL , &s_028};
   _DECL s_030 = { "CHR"             , 'C', 1 , (BYTE*)"N"                                                    , NULL     , NULL , &s_029};
   _DECL s_031 = { "CMONTH"          , 'C', 1 , (BYTE*)"D"                                                    , NULL     , NULL , &s_030};
   _DECL s_032 = { "COL"             , 'N', 0 , (BYTE*)NULL                                                   , NULL     , NULL , &s_031};
   _DECL s_033 = { "CTOD"            , 'D', 1 , (BYTE*)"C"                                                    , NULL     , NULL , &s_032};
   _DECL s_034 = { "CURDIR"          , 'C', 1 , (BYTE*)"\x9d"                                                 , NULL     , NULL , &s_033};
   _DECL s_035 = { "DATE"            , 'D', 0 , (BYTE*)NULL                                                   , NULL     , NULL , &s_034};
   _DECL s_036 = { "DAY"             , 'N', 1 , (BYTE*)"D"                                                    , NULL     , NULL , &s_035};
   _DECL s_037 = { "DAYS"            , 'N', 1 , (BYTE*)"N"                                                    , NULL     , NULL , &s_036};
   _DECL s_038 = { "DBAPPEND"        , '-', 1 , (BYTE*)"\xa6"                                                 , NULL     , NULL , &s_037};
   _DECL s_039 = { "DBCLEARFILTER"   , '-', 0 , (BYTE*)NULL                                                   , NULL     , NULL , &s_038};
   _DECL s_040 = { "DBCLEARINDEX"    , '-', 0 , (BYTE*)NULL                                                   , NULL     , NULL , &s_039};
   _DECL s_041 = { "DBCLEARRELATION" , '-', 0 , (BYTE*)NULL                                                   , NULL     , NULL , &s_040};
   _DECL s_042 = { "DBCLOSEALL"      , '-', 0 , (BYTE*)NULL                                                   , NULL     , NULL , &s_041};
   _DECL s_043 = { "DBCLOSEAREA"     , '-', 0 , (BYTE*)NULL                                                   , NULL     , NULL , &s_042};
   _DECL s_044 = { "DBCOMMIT"        , '-', 0 , (BYTE*)NULL                                                   , NULL     , NULL , &s_043};
   _DECL s_045 = { "DBCOMMITALL"     , '-', 0 , (BYTE*)NULL                                                   , NULL     , NULL , &s_044};
   _DECL s_046 = { "DBCREATE"        , '-', 5 , (BYTE*)"CA\x9d\xa6\x9d"                                       , NULL     , NULL , &s_045};
   _DECL s_047 = { "DBCREATEINDEX"   , '-', 5 , (BYTE*)"C B\xa6\xa6"                                          , NULL     , NULL , &s_046};
   _DECL s_048 = { "DBDELETE"        , '-', 0 , (BYTE*)NULL                                                   , NULL     , NULL , &s_047};
   _DECL s_049 = { "DBEDIT"          , '-',12 , (BYTE*)"\xa8\xa8\xa8\xa8\x7a\x7a\x7a\x7a\x7a\x7a\x7a\x7a"     , NULL     , NULL , &s_048};
   _DECL s_050 = { "DBEVAL"          , '-', 6 , (BYTE*)"B\x9c\x9cNNL"                                         , NULL     , NULL , &s_049};
   _DECL s_051 = { "DBF"             , 'C', 0 , (BYTE*)NULL                                                   , NULL     , NULL , &s_050};
   _DECL s_052 = { "DBFILTER"        , ' ', 0 , (BYTE*)NULL                                                   , NULL     , NULL , &s_051};
   _DECL s_053 = { "DBGOBOTTOM"      , '-', 0 , (BYTE*)NULL                                                   , NULL     , NULL , &s_052};
   _DECL s_054 = { "DBGOTO"          , '-', 1 , (BYTE*)"\x7a"                                                 , NULL     , NULL , &s_053};
   _DECL s_055 = { "DBGOTOP"         , '-', 0 , (BYTE*)NULL                                                   , NULL     , NULL , &s_054};
   _DECL s_056 = { "DBRECALL"        , '-', 0 , (BYTE*)NULL                                                   , NULL     , NULL , &s_055};
   _DECL s_057 = { "DBREINDEX"       , '-', 0 , (BYTE*)NULL                                                   , NULL     , NULL , &s_056};
   _DECL s_058 = { "DBRELATION"      , ' ', 1 , (BYTE*)"N"                                                    , NULL     , NULL , &s_057};
   _DECL s_059 = { "DBRLOCK"         , 'L', 1 , (BYTE*)"\x7a"                                                 , NULL     , NULL , &s_058};
   _DECL s_060 = { "DBRLOCKLIST"     , 'A', 0 , (BYTE*)NULL                                                   , NULL     , NULL , &s_059};
   _DECL s_061 = { "DBRSELECT"       , 'N', 1 , (BYTE*)"N"                                                    , NULL     , NULL , &s_060};
   _DECL s_062 = { "DBRUNLOCK"       , '-', 1 , (BYTE*)"\x7a"                                                 , NULL     , NULL , &s_061};
   _DECL s_063 = { "DBSEEK"          , 'L', 3 , (BYTE*)" \xa6\xa6"                                            , NULL     , NULL , &s_062};
   _DECL s_064 = { "DBSELECTAREA"    , '-', 1 , (BYTE*)" "                                                    , NULL     , NULL , &s_063};
   _DECL s_065 = { "DBSETDRIVER"     , 'C', 1 , (BYTE*)"\x9d"                                                 , NULL     , NULL , &s_064};
   _DECL s_066 = { "DBSETFILTER"     , '-', 2 , (BYTE*)"B\x9d"                                                , NULL     , NULL , &s_065};
   _DECL s_067 = { "DBSETINDEX"      , '-', 1 , (BYTE*)"C"                                                    , NULL     , NULL , &s_066};
   _DECL s_068 = { "DBSETORDER"      , '-', 1 , (BYTE*)"N"                                                    , NULL     , NULL , &s_067};
   _DECL s_069 = { "DBSETRELATION"   , '-', 3 , (BYTE*)" BC"                                                  , NULL     , NULL , &s_068};
   _DECL s_070 = { "DBSKIP"          , '-', 1 , (BYTE*)"\xa8"                                                 , NULL     , NULL , &s_069};
   _DECL s_071 = { "DBSTRUCT"        , 'A', 0 , (BYTE*)NULL                                                   , NULL     , NULL , &s_070};
   _DECL s_072 = { "DBUNLOCK"        , '-', 0 , (BYTE*)NULL                                                   , NULL     , NULL , &s_071};
   _DECL s_073 = { "DBUNLOCKALL"     , '-', 0 , (BYTE*)NULL                                                   , NULL     , NULL , &s_072};
   _DECL s_074 = { "DBUSEAREA"       , '-', 6 , (BYTE*)"\xa6\x9d""C\x9d\xa6\xa6"                              , NULL     , NULL , &s_073};
   _DECL s_075 = { "DELETED"         , 'L', 0 , (BYTE*)NULL                                                   , NULL     , NULL , &s_074};
   _DECL s_076 = { "DESCEND"         , ' ', 1 , (BYTE*)" "                                                    , NULL     , NULL , &s_075};
   _DECL s_077 = { "DEVOUT"          , '-', 2 , (BYTE*)" \x9d"                                                , NULL     , NULL , &s_076};
   _DECL s_078 = { "DEVOUTPICT"      , '-', 3 , (BYTE*)" C\x9d"                                               , NULL     , NULL , &s_077};
   _DECL s_079 = { "DEVPOS"          , '-', 2 , (BYTE*)"NN"                                                   , NULL     , NULL , &s_078};
   _DECL s_080 = { "DIRECTORY"       , 'A', 3 , (BYTE*)"\x9d\x9d\xa6"                                         , NULL     , NULL , &s_079};
   _DECL s_081 = { "DIRCHANGE"       , 'N', 1 , (BYTE*)"C"                                                    , NULL     , NULL , &s_080};
   _DECL s_082 = { "DIRREMOVE"       , 'N', 1 , (BYTE*)"C"                                                    , NULL     , NULL , &s_081};
   _DECL s_083 = { "DISKSPACE"       , 'N', 1 , (BYTE*)"\xa8"                                                 , NULL     , NULL , &s_082};
   _DECL s_084 = { "DISPBEGIN"       , '-', 0 , (BYTE*)NULL                                                   , NULL     , NULL , &s_083};
   _DECL s_085 = { "DISPCOUNT"       , 'N', 0 , (BYTE*)NULL                                                   , NULL     , NULL , &s_084};
   _DECL s_086 = { "DISPEND"         , '-', 0 , (BYTE*)NULL                                                   , NULL     , NULL , &s_085};
   _DECL s_087 = { "DISPOUT"         , '-', 2 , (BYTE*)" \x9d"                                                , NULL     , NULL , &s_086};
   _DECL s_088 = { "DOW"             , 'N', 1 , (BYTE*)"D"                                                    , NULL     , NULL , &s_087};
   _DECL s_089 = { "DTOC"            , 'C', 1 , (BYTE*)"D"                                                    , NULL     , NULL , &s_088};
   _DECL s_090 = { "DTOS"            , 'C', 1 , (BYTE*)"D"                                                    , NULL     , NULL , &s_089};
   _DECL s_091 = { "ELAPTIME"        , 'C', 2 , (BYTE*)"CC"                                                   , NULL     , NULL , &s_090};
   _DECL s_092 = { "EMPTY"           , 'L', 1 , (BYTE*)" "                                                    , NULL     , NULL , &s_091};
   _DECL s_093 = { "EOF"             , 'L', 0 , (BYTE*)NULL                                                   , NULL     , NULL , &s_092};
   _DECL s_094 = { "ERRORNEW"        , 'S', 0 , (BYTE*)NULL                                                   , NULL     , NULL , &s_093};
   _DECL s_095 = { "EVAL"            , ' ', 3 , (BYTE*)"B\x7a\x7a"                                            , NULL     , NULL , &s_094};
   _DECL s_096 = { "EXP"             , 'N', 1 , (BYTE*)"N"                                                    , NULL     , NULL , &s_095};
   _DECL s_097 = { "FCLOSE"          , 'L', 1 , (BYTE*)"N"                                                    , NULL     , NULL , &s_096};
   _DECL s_098 = { "FCOUNT"          , 'N', 0 , (BYTE*)NULL                                                   , NULL     , NULL , &s_097};
   _DECL s_099 = { "FCREATE"         , 'N', 2 , (BYTE*)"C\xa8"                                                , NULL     , NULL , &s_098};
   _DECL s_100 = { "FERASE"          , 'N', 1 , (BYTE*)"C"                                                    , NULL     , NULL , &s_099};
   _DECL s_101 = { "FERROR"          , 'N', 0 , (BYTE*)NULL                                                   , NULL     , NULL , &s_100};
   _DECL s_102 = { "FIELD"           , 'C', 1 , (BYTE*)"N"                                                    , NULL     , NULL , &s_101};
   _DECL s_103 = { "FIELDBLOCK"      , 'B', 1 , (BYTE*)"C"                                                    , NULL     , NULL , &s_102};
   _DECL s_104 = { "FIELDGET"        , ' ', 1 , (BYTE*)"N"                                                    , NULL     , NULL , &s_103};
   _DECL s_105 = { "FIELDNAME"       , 'C', 1 , (BYTE*)"N"                                                    , NULL     , NULL , &s_104};
   _DECL s_106 = { "FIELDPOS"        , 'N', 1 , (BYTE*)"C"                                                    , NULL     , NULL , &s_105};
   _DECL s_107 = { "FIELDPUT"        , ' ', 2 , (BYTE*)"N "                                                   , NULL     , NULL , &s_106};
   _DECL s_108 = { "FIELDWBLOCK"     , 'B', 2 , (BYTE*)"CN"                                                   , NULL     , NULL , &s_107};
   _DECL s_109 = { "FILE"            , 'L', 1 , (BYTE*)"C"                                                    , NULL     , NULL , &s_108};
   _DECL s_110 = { "FLOCK"           , 'L', 0 , (BYTE*)NULL                                                   , NULL     , NULL , &s_109};
   _DECL s_111 = { "FOPEN"           , 'N', 2 , (BYTE*)"C\xa8"                                                , NULL     , NULL , &s_110};
   _DECL s_112 = { "FOUND"           , 'L', 0 , (BYTE*)NULL                                                   , NULL     , NULL , &s_111};
   _DECL s_113 = { "FREAD"           , 'N', 3 , (BYTE*)"N\x5cN"                                               , NULL     , NULL , &s_112};
   _DECL s_114 = { "FREADSTR"        , 'C', 2 , (BYTE*)"NN"                                                   , NULL     , NULL , &s_113};
   _DECL s_115 = { "FRENAME"         , 'N', 2 , (BYTE*)"CC"                                                   , NULL     , NULL , &s_114};
   _DECL s_116 = { "FSEEK"           , 'N', 3 , (BYTE*)"NN\xa8"                                               , NULL     , NULL , &s_115};
   _DECL s_117 = { "FWRITE"          , 'N', 3 , (BYTE*)"NC\xa8"                                               , NULL     , NULL , &s_116};
   _DECL s_118 = { "GETACTIVE"       , '-', 1 , (BYTE*)"O"                                                    , NULL     , NULL , &s_117};
   _DECL s_119 = { "GETAPPLYKEY"     , '-', 2 , (BYTE*)"ON"                                                   , NULL     , NULL , &s_118};
   _DECL s_120 = { "GETDOSETKEY"     , '-', 2 , (BYTE*)"BO"                                                   , NULL     , NULL , &s_119};
   _DECL s_121 = { "GETENV"          , 'C', 1 , (BYTE*)"C"                                                    , NULL     , NULL , &s_120};
   _DECL s_122 = { "GETNEW"          , 'O', 6 , (BYTE*)"\xa8\xa8\x9c\x9d\x9d\x9d"                             , NULL     , NULL , &s_121};
   _DECL s_123 = { "GETPREVALIDATE"  , 'L', 1 , (BYTE*)"O"                                                    , NULL     , NULL , &s_122};
   _DECL s_124 = { "GETPOSTVALIDATE" , 'L', 1 , (BYTE*)"O"                                                    , NULL     , NULL , &s_123};
   _DECL s_125 = { "GETREADER"       , '-', 1 , (BYTE*)"O"                                                    , NULL     , NULL , &s_124};
   _DECL s_126 = { "HARDCR"          , 'C', 1 , (BYTE*)"C"                                                    , NULL     , NULL , &s_125};
   _DECL s_127 = { "HB_ANSITOOEM"    , 'C', 1 , (BYTE*)"C"                                                    , NULL     , NULL , &s_126};
   _DECL s_128 = { "HB_DISKSPACE"    , 'N', 2 , (BYTE*)"\x9d\xa8"                                             , NULL     , NULL , &s_127};
   _DECL s_129 = { "HB_FEOF"         , 'L', 1 , (BYTE*)"N"                                                    , NULL     , NULL , &s_128};
   _DECL s_130 = { "HB_OEMTOANSI"    , 'C', 1 , (BYTE*)"C"                                                    , NULL     , NULL , &s_129};
   _DECL s_131 = { "HEADER"          , 'N', 0 , (BYTE*)NULL                                                   , NULL     , NULL , &s_130};
   _DECL s_132 = { "I2BIN"           , 'C', 1 , (BYTE*)"N"                                                    , NULL     , NULL , &s_131};
   _DECL s_133 = { "IF"              , ' ', 3 , (BYTE*)"L  "                                                  , NULL     , NULL , &s_132};
   _DECL s_134 = { "INDEXEXT"        , 'N', 0 , (BYTE*)NULL                                                   , NULL     , NULL , &s_133};
   _DECL s_135 = { "INDEXKEY"        , 'C', 1 , (BYTE*)"N"                                                    , NULL     , NULL , &s_134};
   _DECL s_136 = { "INDEXORD"        , 'N', 0 , (BYTE*)NULL                                                   , NULL     , NULL , &s_135};
   _DECL s_137 = { "INKEY"           , 'N', 2 , (BYTE*)"\xa8\xa8"                                             , NULL     , NULL , &s_136};
   _DECL s_138 = { "INT"             , 'N', 1 , (BYTE*)"N"                                                    , NULL     , NULL , &s_137};
   _DECL s_139 = { "ISAFFIRM"        , 'L', 1 , (BYTE*)"C"                                                    , NULL     , NULL , &s_138};
   _DECL s_140 = { "ISALPHA"         , 'L', 1 , (BYTE*)"C"                                                    , NULL     , NULL , &s_139};
   _DECL s_141 = { "ISDIGIT"         , 'L', 1 , (BYTE*)"C"                                                    , NULL     , NULL , &s_140};
   _DECL s_142 = { "ISDISK"          , 'L', 1 , (BYTE*)"C"                                                    , NULL     , NULL , &s_141};
   _DECL s_143 = { "ISLOWER"         , 'L', 1 , (BYTE*)"C"                                                    , NULL     , NULL , &s_142};
   _DECL s_144 = { "ISNEGATIVE"      , 'L', 1 , (BYTE*)"C"                                                    , NULL     , NULL , &s_143};
   _DECL s_145 = { "ISUPPER"         , 'L', 1 , (BYTE*)"C"                                                    , NULL     , NULL , &s_144};
   _DECL s_146 = { "L2BIN"           , 'C', 1 , (BYTE*)"N"                                                    , NULL     , NULL , &s_145};
   _DECL s_147 = { "LASTKEY"         , 'N', 1 , (BYTE*)"\xa8"                                                 , NULL     , NULL , &s_146};
   _DECL s_148 = { "LASTREC"         , 'N', 1 , (BYTE*)" "                                                    , NULL     , NULL , &s_147};
   _DECL s_149 = { "LEFT"            , 'C', 2 , (BYTE*)"CN"                                                   , NULL     , NULL , &s_148};
   _DECL s_150 = { "LEN"             , 'N', 1 , (BYTE*)" "                                                    , NULL     , NULL , &s_149};
   _DECL s_151 = { "LOG"             , 'N', 1 , (BYTE*)"N"                                                    , NULL     , NULL , &s_150};
   _DECL s_152 = { "LOWER"           , 'C', 1 , (BYTE*)"C"                                                    , NULL     , NULL , &s_151};
   _DECL s_153 = { "LTRIM"           , 'C', 1 , (BYTE*)"C"                                                    , NULL     , NULL , &s_152};
   _DECL s_154 = { "LUPDATE"         , 'D', 0 , (BYTE*)NULL                                                   , NULL     , NULL , &s_153};
   _DECL s_155 = { "MAKEDIR"         , 'N', 1 , (BYTE*)"C"                                                    , NULL     , NULL , &s_154};
   _DECL s_156 = { "MAX"             , ' ', 2 , (BYTE*)"  "                                                   , NULL     , NULL , &s_155};
   _DECL s_157 = { "MAXCOL"          , 'N', 0 , (BYTE*)NULL                                                   , NULL     , NULL , &s_156};
   _DECL s_158 = { "MAXROW"          , 'N', 0 , (BYTE*)NULL                                                   , NULL     , NULL , &s_157};
   _DECL s_159 = { "MCOL"            , 'N', 0 , (BYTE*)NULL                                                   , NULL     , NULL , &s_158};
   _DECL s_160 = { "MEMOEDIT"        , 'C',13 , (BYTE*)"\x9d\xa8\xa8\xa8\xa8\xa6\x9d\xa8\xa8\xa8\xa8\xa8\xa8" , NULL     , NULL , &s_159};
   _DECL s_161 = { "MEMOTRAN"        , 'C', 3 , (BYTE*)"C\x9d\x9d"                                            , NULL     , NULL , &s_160};
   _DECL s_162 = { "MEMOLINE"        , 'C', 5 , (BYTE*)"C\xa8\xa8\xa8\xa6"                                    , NULL     , NULL , &s_161};
   _DECL s_163 = { "MEMORY"          , 'N', 1 , (BYTE*)"N"                                                    , NULL     , NULL , &s_162};
   _DECL s_164 = { "MEMOREAD"        , 'C', 1 , (BYTE*)"C"                                                    , NULL     , NULL , &s_163};
   _DECL s_165 = { "MEMOTRAN"        , 'C', 3 , (BYTE*)"C\x9d\x9d"                                            , NULL     , NULL , &s_164};
   _DECL s_166 = { "MEMOWRIT"        , 'L', 2 , (BYTE*)"CC"                                                   , NULL     , NULL , &s_165};
   _DECL s_167 = { "MEMVARBLOCK"     , 'B', 1 , (BYTE*)"C"                                                    , NULL     , NULL , &s_166};
   _DECL s_168 = { "MIN"             , ' ', 2 , (BYTE*)"  "                                                   , NULL     , NULL , &s_167};
   _DECL s_169 = { "MLCOUNT"         , 'N', 4 , (BYTE*)"C\xa8\xa8\xa6"                                        , NULL     , NULL , &s_168};
   _DECL s_170 = { "MLCTOPOS"        , 'L', 0 , (BYTE*)NULL                                                   , NULL     , NULL , &s_169};
   _DECL s_171 = { "MLPOS"           , 'N', 5 , (BYTE*)"CNN\xa8\xa6"                                          , NULL     , NULL , &s_170};
   _DECL s_172 = { "MOD"             , 'N', 2 , (BYTE*)"NN"                                                   , NULL     , NULL , &s_171};
   _DECL s_173 = { "MONTH"           , 'N', 1 , (BYTE*)"D"                                                    , NULL     , NULL , &s_172};
   _DECL s_174 = { "MPOSTOLC"        , 'L', 0 , (BYTE*)NULL                                                   , NULL     , NULL , &s_173};
   _DECL s_175 = { "MROW"            , 'N', 0 , (BYTE*)NULL                                                   , NULL     , NULL , &s_174};
   _DECL s_176 = { "NATIONMSG"       , 'C', 1 , (BYTE*)"N"                                                    , NULL     , NULL , &s_175};
   _DECL s_177 = { "NETERR"          , 'L', 1 , (BYTE*)"\xa6"                                                 , NULL     , NULL , &s_176};
   _DECL s_178 = { "NETNAME"         , 'C', 0 , (BYTE*)NULL                                                   , NULL     , NULL , &s_177};
   _DECL s_179 = { "NEXTKEY"         , 'N', 1 , (BYTE*)"\xa8"                                                 , NULL     , NULL , &s_178};
   _DECL s_180 = { "ORDBAGEXT"       , 'C', 0 , (BYTE*)NULL                                                   , NULL     , NULL , &s_179};
   _DECL s_181 = { "ORDBAGNAME"      , 'C', 1 , (BYTE*)" "                                                    , NULL     , NULL , &s_180};
   _DECL s_182 = { "ORDCREATE"       , '-', 5 , (BYTE*)"C\x9d \x9c\xa6"                                       , NULL     , NULL , &s_181};
   _DECL s_183 = { "ORDDESTROY"      , '-', 2 , (BYTE*)"C\x9d"                                                , NULL     , NULL , &s_182};
   _DECL s_184 = { "ORDFOR"          , 'C', 2 , (BYTE*)" \x9d"                                                , NULL     , NULL , &s_183};
   _DECL s_185 = { "ORDKEY"          , 'C', 2 , (BYTE*)" \x9d"                                                , NULL     , NULL , &s_184};
   _DECL s_186 = { "ORDLISTADD"      , '-', 2 , (BYTE*)"C\x9d"                                                , NULL     , NULL , &s_185};
   _DECL s_187 = { "ORDLISTCLEAR"    , '-', 0 , (BYTE*)NULL                                                   , NULL     , NULL , &s_186};
   _DECL s_188 = { "ORDLISTREBUILD"  , '-', 0 , (BYTE*)NULL                                                   , NULL     , NULL , &s_187};
   _DECL s_189 = { "ORDNAME"         , 'C', 2 , (BYTE*)"N\x9d"                                                , NULL     , NULL , &s_188};
   _DECL s_190 = { "ORDNUMBER"       , 'N', 2 , (BYTE*)"C\x9d"                                                , NULL     , NULL , &s_189};
   _DECL s_191 = { "ORDSETFOCUS"     , 'C', 2 , (BYTE*)"\x7a\x9d"                                             , NULL     , NULL , &s_190};
   _DECL s_192 = { "OS"              , 'C', 0 , (BYTE*)NULL                                                   , NULL     , NULL , &s_191};
   _DECL s_193 = { "OUTERR"          , '-', 1 , (BYTE*)"\x7a"                                                 , NULL     , NULL , &s_192};
   _DECL s_194 = { "OUTSTD"          , '-', 1 , (BYTE*)"\x7a"                                                 , NULL     , NULL , &s_193};
   _DECL s_195 = { "PADC"            , 'C', 3 , (BYTE*)" N\x9d"                                               , NULL     , NULL , &s_194};
   _DECL s_196 = { "PADL"            , 'C', 3 , (BYTE*)" N\x9d"                                               , NULL     , NULL , &s_195};
   _DECL s_197 = { "PADR"            , 'C', 3 , (BYTE*)" N\x9d"                                               , NULL     , NULL , &s_196};
   _DECL s_198 = { "PCOL"            , 'N', 0 , (BYTE*)NULL                                                   , NULL     , NULL , &s_197};
   _DECL s_199 = { "PCOUNT"          , 'N', 0 , (BYTE*)NULL                                                   , NULL     , NULL , &s_198};
   _DECL s_200 = { "PROCFILE"        , 'C', 1 , (BYTE*)"\x7a"                                                 , NULL     , NULL , &s_199};
   _DECL s_201 = { "PROCLINE"        , 'N', 1 , (BYTE*)"\xa8"                                                 , NULL     , NULL , &s_200};
   _DECL s_202 = { "PROCNAME"        , 'N', 1 , (BYTE*)"\xa8"                                                 , NULL     , NULL , &s_201};
   _DECL s_203 = { "PROW"            , 'N', 0 , (BYTE*)NULL                                                   , NULL     , NULL , &s_202};
/*
 * Both QOUT and QQOUT can take from 0 to as many parameters as you like
 * of any type, so including them in the parameter checking is of no use,
 * because the list requires an upper limit and a type declaration for all
 * of the parameters. So instead of trying to fix the unfixable, I have
 * commented them out and fixed the linkage for s_206 to point to s_203.
 * - David G. Holm <dholm@jsd-llc.com>
 *
 *   _DECL s_204 = { "QOUT"            , '-', 2 , (BYTE*)"\x7a\x7a"                                             , NULL     , NULL , &s_203};
 *   _DECL s_205 = { "QQOUT"           , '-', 2 , (BYTE*)"\x7a\x7a"                                             , NULL     , NULL , &s_204};
*/
   _DECL s_206 = { "RAT"             , 'N', 2 , (BYTE*)"CC"                                                   , NULL     , NULL , &s_203};
   _DECL s_207 = { "RDDLIST"         , 'A', 1 , (BYTE*)"\xa8"                                                 , NULL     , NULL , &s_206};
   _DECL s_208 = { "RDDNAME"         , 'C', 0 , (BYTE*)NULL                                                   , NULL     , NULL , &s_207};
   _DECL s_209 = { "RDDSETDEFAULT"   , 'C', 1 , (BYTE*)"\x9d"                                                 , NULL     , NULL , &s_208};
   _DECL s_210 = { "READEXIT"        , 'L', 1 , (BYTE*)"\xa6"                                                 , NULL     , NULL , &s_209};
   _DECL s_211 = { "READUPDATED"     , 'L', 1 , (BYTE*)"\xa6"                                                 , NULL     , NULL , &s_210};
   _DECL s_212 = { "READINSERT"      , 'L', 1 , (BYTE*)"\xa6"                                                 , NULL     , NULL , &s_211};
   _DECL s_213 = { "READKEY"         , 'N', 0 , (BYTE*)NULL                                                   , NULL     , NULL , &s_212};
   _DECL s_214 = { "READFORMAT"      , 'B', 1 , (BYTE*)"B"                                                    , NULL     , NULL , &s_213};
   _DECL s_215 = { "READKILL"        , 'L', 1 , (BYTE*)"\xa6"                                                 , NULL     , NULL , &s_214};
   _DECL s_216 = { "READMODAL"       , 'L', 2 , (BYTE*)"A\xa8"                                                , NULL     , NULL , &s_215};
   _DECL s_217 = { "READUPDATED"     , 'L', 1 , (BYTE*)"\xa6"                                                 , NULL     , NULL , &s_216};
   _DECL s_218 = { "READVAR"         , 'C', 1 , (BYTE*)"\x9d"                                                 , NULL     , NULL , &s_217};
   _DECL s_219 = { "RECCOUNT"        , 'N', 0 , (BYTE*)NULL                                                   , NULL     , NULL , &s_218};
   _DECL s_220 = { "RECNO"           , 'N', 0 , (BYTE*)NULL                                                   , NULL     , NULL , &s_219};
   _DECL s_221 = { "RECSIZE"         , 'N', 0 , (BYTE*)NULL                                                   , NULL     , NULL , &s_220};
   _DECL s_222 = { "REPLICATE"       , 'C', 2 , (BYTE*)"CN"                                                   , NULL     , NULL , &s_221};
   _DECL s_223 = { "RESTSCREEN"      , '-', 5 , (BYTE*)"\xa8\xa8\xa8\xa8\x9d"                                 , NULL     , NULL , &s_222};
   _DECL s_224 = { "RIGHT"           , 'C', 2 , (BYTE*)"CN"                                                   , NULL     , NULL , &s_223};
   _DECL s_225 = { "RLOCK"           , 'L', 0 , (BYTE*)NULL                                                   , NULL     , NULL , &s_224};
   _DECL s_226 = { "ROUND"           , 'N', 2 , (BYTE*)"NN"                                                   , NULL     , NULL , &s_225};
   _DECL s_227 = { "ROW"             , 'N', 0 , (BYTE*)NULL                                                   , NULL     , NULL , &s_226};
   _DECL s_228 = { "RTRIM"           , 'C', 1 , (BYTE*)"C"                                                    , NULL     , NULL , &s_227};
   _DECL s_229 = { "SAVESCREEN"      , '-', 4 , (BYTE*)"\xa8\xa8\xa8\xa8"                                     , NULL     , NULL , &s_228};
   _DECL s_230 = { "SCROLL"          , '-', 6 , (BYTE*)"\xa8\xa8\xa8\xa8\xa8\xa8"                             , NULL     , NULL , &s_229};
   _DECL s_231 = { "SECONDS"         , 'N', 0 , (BYTE*)NULL                                                   , NULL     , NULL , &s_230};
   _DECL s_232 = { "SECS"            , 'N', 1 , (BYTE*)"C"                                                    , NULL     , NULL , &s_231};
   _DECL s_233 = { "SELECT"          , 'N', 1 , (BYTE*)"\x9d"                                                 , NULL     , NULL , &s_232};
   _DECL s_234 = { "SET"             , ' ', 3 , (BYTE*)"N\x7a\xa6"                                            , NULL     , NULL , &s_233};
   _DECL s_235 = { "SETCOLOR"        , 'C', 1 , (BYTE*)"\x9d"                                                 , NULL     , NULL , &s_234};
   _DECL s_236 = { "SETCURSOR"       , 'N', 1 , (BYTE*)"\xa8"                                                 , NULL     , NULL , &s_235};
   _DECL s_237 = { "SETKEY"          , ' ', 3 , (BYTE*)"N\x9c\x9c"                                            , NULL     , NULL , &s_236};
   _DECL s_238 = { "SETMODE"         , 'L', 2 , (BYTE*)"NN"                                                   , NULL     , NULL , &s_237};
   _DECL s_239 = { "SETPOS"          , '-', 2 , (BYTE*)"NN"                                                   , NULL     , NULL , &s_238};
   _DECL s_240 = { "SETPRC"          , '-', 2 , (BYTE*)"NN"                                                   , NULL     , NULL , &s_239};
   _DECL s_241 = { "SETTYPEAHEAD"    , '-', 1 , (BYTE*)"N"                                                    , NULL     , NULL , &s_240};
   _DECL s_242 = { "SPACE"           , 'C', 1 , (BYTE*)"N"                                                    , NULL     , NULL , &s_241};
   _DECL s_243 = { "SQRT"            , 'N', 1 , (BYTE*)"N"                                                    , NULL     , NULL , &s_242};
   _DECL s_244 = { "STR"             , 'C', 3 , (BYTE*)"N\xa8\xa8"                                            , NULL     , NULL , &s_243};
   _DECL s_245 = { "STRTRAN"         , 'C', 5 , (BYTE*)"CC\x9d\xa8\xa8"                                       , NULL     , NULL , &s_244};
   _DECL s_246 = { "STRZERO"         , 'C', 3 , (BYTE*)"N\xa8\xa8"                                            , NULL     , NULL , &s_245};
   _DECL s_247 = { "STUFF"           , 'C', 4 , (BYTE*)"CNNC"                                                 , NULL     , NULL , &s_246};
   _DECL s_248 = { "SUBSTR"          , 'C', 3 , (BYTE*)"CN\xa8"                                               , NULL     , NULL , &s_247};
   _DECL s_249 = { "TBROWSENEW"      , 'O', 4 , (BYTE*)"NNNN"                                                 , NULL     , NULL , &s_248};
   _DECL s_250 = { "TBROWSEDB"       , 'O', 4 , (BYTE*)"NNNN"                                                 , NULL     , NULL , &s_249};
   _DECL s_251 = { "TBCOLUMNNEW"     , 'O', 2 , (BYTE*)"CB"                                                   , NULL     , NULL , &s_250};
   _DECL s_252 = { "TIME"            , 'C', 0 , (BYTE*)NULL                                                   , NULL     , NULL , &s_251};
   _DECL s_253 = { "TONE"            , '-', 2 , (BYTE*)"NN"                                                   , NULL     , NULL , &s_252};
   _DECL s_254 = { "TRANSFORM"       , 'C', 2 , (BYTE*)" C"                                                   , NULL     , NULL , &s_253};
   _DECL s_255 = { "TRIM"            , 'C', 1 , (BYTE*)"C"                                                    , NULL     , NULL , &s_254};
   _DECL s_256 = { "TYPE"            , 'C', 1 , (BYTE*)" "                                                    , NULL     , NULL , &s_255};
   _DECL s_257 = { "U2BIN"           , 'C', 1 , (BYTE*)"N"                                                    , NULL     , NULL , &s_256};
   _DECL s_258 = { "UPDATED"         , 'L', 0 , (BYTE*)NULL                                                   , NULL     , NULL , &s_257};
   _DECL s_259 = { "UPPER"           , 'C', 1 , (BYTE*)"C"                                                    , NULL     , NULL , &s_258};
   _DECL s_260 = { "USED"            , 'L', 0 , (BYTE*)NULL                                                   , NULL     , NULL , &s_259};
   _DECL s_261 = { "VAL"             , 'N', 1 , (BYTE*)"C"                                                    , NULL     , NULL , &s_260};
   _DECL s_262 = { "VALTYPE"         , ' ', 1 , (BYTE*)" "                                                    , NULL     , NULL , &s_261};
   _DECL s_263 = { "VERSION"         , 'C', 0 , (BYTE*)NULL                                                   , NULL     , NULL , &s_262};
   _DECL s_264 = { "W2BIN"           , 'C', 1 , (BYTE*)"N"                                                    , NULL     , NULL , &s_263};
   _DECL s_265 = { "WORD"            , 'N', 1 , (BYTE*)"N"                                                    , NULL     , NULL , &s_264};
   _DECL s_266 = { "HB_FNAMESPLIT"   , '-', 5 , (BYTE*)"C\x5c\x5c\x5c\x5c"                                    , NULL     , NULL , &s_265};
   _DECL s_267 = { "HB_FNAMEMERGE"   , 'C', 4 , (BYTE*)"CCCC"                                                 , NULL     , NULL , &s_266};
   /* TODO: Rest of Standard Functions. */

   /* -------------------------------------------------- Standard Classes --------------------------------------------------- */

   static COMCLASS s_ERROR    = { "ERROR"   , NULL, NULL, NULL };
   static COMCLASS s_GET      = { "GET"     , NULL, NULL, NULL };
   static COMCLASS s_TBCOLUMN = { "TBCOLUMN", NULL, NULL, NULL };
   static COMCLASS s_TBROWSE  = { "TBROWSE" , NULL, NULL, NULL };

  /*       Name     Ret  # of Prams  Param Types   Ret Class  Param Classes  Next
   ---------------  ---  ----------  --------------------  ---------  -------------  --------------- */
   _DECL s_ERROR_01    = { "ARGS" , 'A', 0 , (BYTE*)NULL , NULL     , NULL , NULL    };
   _DECL s_ERROR_02    = { "CANDEFAULT"   , 'B', 0 , (BYTE*)NULL , NULL     , NULL , &s_ERROR_01     };
   _DECL s_ERROR_03    = { "CANRETRY"     , 'B', 0 , (BYTE*)NULL , NULL     , NULL , &s_ERROR_02     };
   _DECL s_ERROR_04    = { "CANSUBSTITUTE", 'B', 0 , (BYTE*)NULL , NULL     , NULL , &s_ERROR_03     };
   _DECL s_ERROR_05    = { "CARGO"        , ' ', 0 , (BYTE*)NULL , NULL     , NULL , &s_ERROR_04     };
   _DECL s_ERROR_06    = { "DESCRIPTION"  , 'S', 0 , (BYTE*)NULL , NULL     , NULL , &s_ERROR_05     };
   _DECL s_ERROR_07    = { "FILENAME"     , 'S', 0 , (BYTE*)NULL , NULL     , NULL , &s_ERROR_06     };
   _DECL s_ERROR_08    = { "GENCODE"      , 'N', 0 , (BYTE*)NULL , NULL     , NULL , &s_ERROR_07     };
   _DECL s_ERROR_09    = { "OPERATION"    , 'S', 0 , (BYTE*)NULL , NULL     , NULL , &s_ERROR_08     };
   _DECL s_ERROR_10    = { "OSCODE"       , 'N', 0 , (BYTE*)NULL , NULL     , NULL , &s_ERROR_09     };
   _DECL s_ERROR_11    = { "SEVERITY"     , 'N', 0 , (BYTE*)NULL , NULL     , NULL , &s_ERROR_10     };
   _DECL s_ERROR_12    = { "SUBCODE"      , 'N', 0 , (BYTE*)NULL , NULL     , NULL , &s_ERROR_11     };
   _DECL s_ERROR_13    = { "SUBSYSTEM"    , 'S', 0 , (BYTE*)NULL , NULL     , NULL , &s_ERROR_12     };
   _DECL s_ERROR_14    = { "TRIES"        , 'N', 0 , (BYTE*)NULL , NULL     , NULL , &s_ERROR_13     };

  /*       Name                             Ret  # of Prams  Param Types   Ret Class  Param Classes  Next
   ---------------                          ---  ----------  --------------------  ---------  -------------  --------------- */
   _DECL s_GET_01      = { "ASSIGN"       , ' ', 0 , (BYTE*)NULL   , NULL     , NULL , NULL    };
   _DECL s_GET_02      = { "COLORDISP"    , 'S', 1 , (BYTE*)"\x9d" , &s_GET   , NULL , &s_GET_01       };
   _DECL s_GET_03      = { "DISPLAY"      , 'S', 1 , (BYTE*)"\xa6" , &s_GET   , NULL , &s_GET_02       };
   _DECL s_GET_04      = { "KILLFOCUS"    , 'S', 0 , (BYTE*)NULL   , &s_GET   , NULL , &s_GET_03       };
   _DECL s_GET_05      = { "PARSEPICT"    , 'C', 1 , (BYTE*)"C"    , NULL     , NULL , &s_GET_04       };
   _DECL s_GET_06      = { "RESET"        , 'S', 0 , (BYTE*)NULL   , &s_GET   , NULL , &s_GET_05       };
   _DECL s_GET_07      = { "SETFOCUS"     , 'S', 0 , (BYTE*)NULL   , &s_GET   , NULL , &s_GET_06       };
   _DECL s_GET_08      = { "UNDO"         , 'S', 0 , (BYTE*)NULL   , &s_GET   , NULL , &s_GET_07       };
   _DECL s_GET_09      = { "UNTRANSFORM"  , 'S', 1 , (BYTE*)"\x9d" , &s_GET   , NULL , &s_GET_08       };
   _DECL s_GET_10      = { "UPDATEBUFFER" , 'S', 0 , (BYTE*)NULL   , &s_GET   , NULL , &s_GET_09       };
   _DECL s_GET_11      = { "VARGET"       , ' ', 0 , (BYTE*)NULL   , NULL     , NULL , &s_GET_10       };
   _DECL s_GET_12      = { "VARPUT"       , ' ', 1 , (BYTE*)" "    , NULL     , NULL , &s_GET_11       };

   _DECL s_GET_13      = { "END"          , 'S', 0 , (BYTE*)NULL   , &s_GET   , NULL , &s_GET_12       };
   _DECL s_GET_14      = { "HOME"         , 'S', 0 , (BYTE*)NULL   , &s_GET   , NULL , &s_GET_13       };
   _DECL s_GET_15      = { "LEFT"         , 'S', 1 , (BYTE*)"\xa6" , &s_GET   , NULL , &s_GET_14       };
   _DECL s_GET_16      = { "RIGHT"        , 'S', 1 , (BYTE*)"\xa6" , &s_GET   , NULL , &s_GET_15       };
   _DECL s_GET_17      = { "TODECPOS"     , 'S', 0 , (BYTE*)"L"    , &s_GET   , NULL , &s_GET_16       };
   _DECL s_GET_18      = { "WORDLEFT"     , 'S', 1 , (BYTE*)"\xa6" , &s_GET   , NULL , &s_GET_17       };
   _DECL s_GET_19      = { "WORDRIGHT"    , 'S', 1 , (BYTE*)"\xa6" , &s_GET   , NULL , &s_GET_18       };

   _DECL s_GET_20      = { "BACKSPACE"    , 'S', 1 , (BYTE*)"\xa6" , &s_GET   , NULL , &s_GET_19       };
   _DECL s_GET_21      = { "DELETE"       , 'S', 1 , (BYTE*)"\xa6" , &s_GET   , NULL , &s_GET_20       };
   _DECL s_GET_22      = { "DELEND"       , 'S', 0 , (BYTE*)NULL   , &s_GET   , NULL , &s_GET_21       };
   _DECL s_GET_23      = { "DELLEFT"      , 'S', 0 , (BYTE*)NULL   , &s_GET   , NULL , &s_GET_22       };
   _DECL s_GET_24      = { "DELRIGHT"     , 'S', 0 , (BYTE*)NULL   , &s_GET   , NULL , &s_GET_23       };
   _DECL s_GET_25      = { "DELWORDLEFT"  , 'S', 0 , (BYTE*)NULL   , &s_GET   , NULL , &s_GET_24       };
   _DECL s_GET_26      = { "DELWORDRIGHT" , 'S', 0 , (BYTE*)NULL   , &s_GET   , NULL , &s_GET_25       };

   _DECL s_GET_27      = { "INSERT"       , 'S', 1 , (BYTE*)"C"    , &s_GET   , NULL , &s_GET_26       };
   _DECL s_GET_28      = { "OVERSTRIKE"   , 'S', 1 , (BYTE*)"C"    , &s_GET   , NULL , &s_GET_27       };

   _DECL s_GET_29      = { "DELETEALL"    , 'S', 0 , (BYTE*)NULL   , &s_GET   , NULL , &s_GET_28       };
   _DECL s_GET_30      = { "ISEDITABLE"   , 'L', 1 , (BYTE*)"N"    , NULL     , NULL , &s_GET_29       };
   _DECL s_GET_31      = { "INPUT"        , 'C', 0 , (BYTE*)"C"    , NULL     , NULL , &s_GET_30       };
   _DECL s_GET_32      = { "PUTMASK"      , 'C', 2 , (BYTE*)"CL"   , NULL     , NULL , &s_GET_31       };
   _DECL s_GET_33      = { "HASSCROLL"    , 'L', 0 , (BYTE*)NULL   , NULL     , NULL , &s_GET_32       };

  /*       Name     Ret  # of Prams  Param Types   Ret Class  Param Classes  Next
   ---------------  ---  ----------  --------------------  ---------  -------------  --------------- */
   _DECL s_TBCOLUMN_01 = { "BLOCK"        , ' ', 0 , (BYTE*)NULL , NULL     , NULL , NULL    };
   _DECL s_TBCOLUMN_02 = { "CARGO"        , ' ', 0 , (BYTE*)NULL , NULL     , NULL , &s_TBCOLUMN_01  };
   _DECL s_TBCOLUMN_03 = { "COLORBLOCK"   , 'A', 0 , (BYTE*)NULL , NULL     , NULL , &s_TBCOLUMN_02  };
   _DECL s_TBCOLUMN_04 = { "COLSEP"       , 'C', 0 , (BYTE*)NULL , NULL     , NULL , &s_TBCOLUMN_03  };
   _DECL s_TBCOLUMN_05 = { "DEFCOLOR"     , 'A', 0 , (BYTE*)NULL , NULL     , NULL , &s_TBCOLUMN_04  };
   _DECL s_TBCOLUMN_06 = { "FOOTING"      , 'C', 0 , (BYTE*)NULL , NULL     , NULL , &s_TBCOLUMN_05  };
   _DECL s_TBCOLUMN_07 = { "FOOTSEP"      , 'C', 0 , (BYTE*)NULL , NULL     , NULL , &s_TBCOLUMN_06  };
   _DECL s_TBCOLUMN_08 = { "HEADING"      , 'C', 0 , (BYTE*)NULL , NULL     , NULL , &s_TBCOLUMN_07  };
   _DECL s_TBCOLUMN_09 = { "PICTURE"      , ' ', 0 , (BYTE*)NULL , NULL     , NULL , &s_TBCOLUMN_08  };
   _DECL s_TBCOLUMN_10 = { "WIDTH"        , 'N', 0 , (BYTE*)NULL , NULL     , NULL , &s_TBCOLUMN_09  };
   _DECL s_TBCOLUMN_11 = { "COLPOS"       , ' ', 0 , (BYTE*)NULL , NULL     , NULL , &s_TBCOLUMN_10  };
   _DECL s_TBCOLUMN_12 = { "HEADSEP"      , ' ', 0 , (BYTE*)NULL , NULL     , NULL , &s_TBCOLUMN_11  };

   /* TODO: Finish definition of GET, and add definitions for TBROWSE. */

   #undef _DECL

   /* ------- */

   /* First (bottom) Method */
   s_ERROR.pMethod     = &s_ERROR_14;
   /* Last (top) Method. */
   s_ERROR.pLastMethod = &s_ERROR_01;
   /* Next class definition pointer */
   s_ERROR.pNext = NULL;

   /* ------- */

   /* First (bottom) Method */
   s_GET.pMethod     = &s_GET_33; /* Change to BOTTOM Method. */
   /* Last (top) Method. */
   s_GET.pLastMethod = &s_GET_01;
   /* Next class definition pointer */
   s_GET.pNext = &s_ERROR;

   /* ------- */

   /* First (bottom) Method */
   s_TBCOLUMN.pMethod     = &s_TBCOLUMN_12; /* Change to BOTTOM Method. */
   /* Last (top) Method. */
   s_TBCOLUMN.pLastMethod = &s_TBCOLUMN_01;
   /* Next class definition pointer */
   s_TBCOLUMN.pNext = &s_GET;

   /* ------- */

   /* First (bottom) Method */
   s_TBROWSE.pMethod     = NULL; /* Change to BOTTOM Method. */
   /* Last (top) Method. */
   s_TBROWSE.pLastMethod = NULL;
   /* Next class definition pointer */
   s_TBROWSE.pNext = &s_TBCOLUMN;

   /* ------- */

   HB_COMP_PARAM->pFirstDeclared   = &s_267; /* Change to BOTTOM Function. */
   HB_COMP_PARAM->pLastDeclared    = &s_001;
   HB_COMP_PARAM->pReleaseDeclared = NULL;

   HB_COMP_PARAM->pFirstClass      = &s_TBROWSE;
   HB_COMP_PARAM->pLastClass       = &s_ERROR;
   HB_COMP_PARAM->pReleaseClass    = NULL;
}

/* returns a symbol pointer from the symbol table
 * and sets its position in the symbol table.
 * NOTE: symbol's position number starts from 0
 */
static PCOMDECLARED hb_compDeclaredFind( HB_COMP_DECL, char * szDeclaredName )
{
   PCOMDECLARED pSym = HB_COMP_PARAM->pFirstDeclared;

   while( pSym )
   {
      if( ! strcmp( pSym->szName, szDeclaredName ) )
         return pSym;
      pSym = pSym->pNext;
   }
   return NULL;
}

PCOMDECLARED hb_compDeclaredAdd( HB_COMP_DECL, char * szDeclaredName )
{
   PCOMDECLARED pDeclared;

   if( HB_COMP_PARAM->iWarnings < 3 )
      return NULL;

   /*printf( "\nDeclaring Function: %s\n", szDeclaredName, NULL );*/

   if( ( pDeclared = hb_compDeclaredFind( HB_COMP_PARAM, szDeclaredName ) ) != NULL )
   {
      hb_compGenWarning( HB_COMP_PARAM, hb_comp_szWarnings, 'W', HB_COMP_WARN_DUP_DECLARATION, "Function", szDeclaredName );

      /* Last declaration will take effect. */
      pDeclared->cType = ' '; /* Not known yet */
      pDeclared->cParamTypes = NULL;
      pDeclared->iParamCount = 0;
      pDeclared->pParamClasses = NULL;

      return pDeclared;
   }

   pDeclared = ( PCOMDECLARED ) hb_xgrab( sizeof( COMDECLARED ) );

   pDeclared->szName = szDeclaredName;
   pDeclared->cType = ' '; /* Not known yet */
   pDeclared->cParamTypes = NULL;
   pDeclared->iParamCount = 0;
   pDeclared->pParamClasses = NULL;
   pDeclared->pNext = NULL;

   HB_COMP_PARAM->pLastDeclared->pNext = pDeclared;
   HB_COMP_PARAM->pLastDeclared = pDeclared;
   if( ! HB_COMP_PARAM->pReleaseDeclared )
   {
      HB_COMP_PARAM->pReleaseDeclared = pDeclared;
   }

   return pDeclared;
}

void hb_compDeclaredParameterAdd( HB_COMP_DECL, char * szVarName, BYTE cValueType )
{
   /* Nothing to do since no warnings requested.*/
   if( HB_COMP_PARAM->iWarnings < 3 )
   {
      HB_SYMBOL_UNUSED( szVarName );
      return;
   }

   /* Either a Declared Function Parameter or a Declared Method Parameter. */
   if( HB_COMP_PARAM->szDeclaredFun )
   {
      /* Find the Declared Function owner of this parameter. */
      PCOMDECLARED pDeclared = hb_compDeclaredFind( HB_COMP_PARAM, HB_COMP_PARAM->szDeclaredFun );

      if( pDeclared )
      {
         pDeclared->iParamCount++;


         /* TOFIX: these allocations causes memory leaks */
         if( pDeclared->cParamTypes )
         {
            pDeclared->cParamTypes = ( BYTE * ) hb_xrealloc( pDeclared->cParamTypes, pDeclared->iParamCount );
            pDeclared->pParamClasses = ( PCOMCLASS * ) hb_xrealloc( pDeclared->pParamClasses, pDeclared->iParamCount * sizeof( PCOMCLASS ) );
         }
         else
         {
            pDeclared->cParamTypes = ( BYTE * ) hb_xgrab( 1 );
            pDeclared->pParamClasses = ( PCOMCLASS * ) hb_xgrab( sizeof( PCOMCLASS ) );
         }

         pDeclared->cParamTypes[ pDeclared->iParamCount - 1 ] = cValueType;

         if( toupper( cValueType ) == 'S' )
         {
            pDeclared->pParamClasses[ pDeclared->iParamCount - 1 ] = hb_compClassFind( HB_COMP_PARAM, HB_COMP_PARAM->szFromClass );

            /* Resetting */
            HB_COMP_PARAM->szFromClass = NULL;
         }
      }
   }
   else /* Declared Method Parameter */
   {
      /* printf( "\nAdding parameter: %s Type: %c In Method: %s Class: %s FROM CLASS: %s\n", szVarName, cValueType, HB_COMP_PARAM->pLastMethod->szName, HB_COMP_PARAM->pLastClass->szName, HB_COMP_PARAM->szFromClass ); */

      HB_COMP_PARAM->pLastMethod->iParamCount++;

      /* TOFIX: these allocations causes memory leaks */
      if( HB_COMP_PARAM->pLastMethod->cParamTypes )
      {
         HB_COMP_PARAM->pLastMethod->cParamTypes = ( BYTE * ) hb_xrealloc( HB_COMP_PARAM->pLastMethod->cParamTypes, HB_COMP_PARAM->pLastMethod->iParamCount );
         HB_COMP_PARAM->pLastMethod->pParamClasses = ( PCOMCLASS * ) hb_xrealloc( HB_COMP_PARAM->pLastMethod->pParamClasses, HB_COMP_PARAM->pLastMethod->iParamCount * sizeof( COMCLASS ) );
      }
      else
      {
         HB_COMP_PARAM->pLastMethod->cParamTypes = ( BYTE * ) hb_xgrab( 1 );
         HB_COMP_PARAM->pLastMethod->pParamClasses = ( PCOMCLASS * ) hb_xgrab( sizeof( COMCLASS ) );
      }

      HB_COMP_PARAM->pLastMethod->cParamTypes[ HB_COMP_PARAM->pLastMethod->iParamCount - 1 ] = cValueType;

      if( toupper( cValueType ) == 'S' )
      {
         HB_COMP_PARAM->pLastMethod->pParamClasses[ HB_COMP_PARAM->pLastMethod->iParamCount - 1 ] = hb_compClassFind( HB_COMP_PARAM, HB_COMP_PARAM->szFromClass );

         /* printf( "\nParameter: %s FROM CLASS: %s\n", szVarName, HB_COMP_PARAM->pLastMethod->pParamClasses[ HB_COMP_PARAM->pLastMethod->iParamCount - 1 ]->szName ); */

         /* Resetting */
         HB_COMP_PARAM->szFromClass = NULL;
      }
   }
}


/*
 * FUNCTIONS
 */

static int hb_compSort_ULONG( const void * pLeft, const void * pRight )
{
    ULONG ulLeft  = *( ( ULONG * ) ( pLeft ) );
    ULONG ulRight = *( ( ULONG * ) ( pRight ) );

    if( ulLeft == ulRight )
       return 0 ;
    else if( ulLeft < ulRight )
       return -1;
    else
       return 1;
}

/* Jump Optimizer and dummy code eliminator */
static void hb_compOptimizeJumps( HB_COMP_DECL )
{
   BYTE * pCode = HB_COMP_PARAM->functions.pLast->pCode;
   ULONG * pNOOPs, * pJumps;
   ULONG ulOptimized, ulNextByte, ulBytes2Copy, ulJumpAddr, iNOOP, iJump;
   BOOL fLineStrip = !HB_COMP_PARAM->fDebugInfo && HB_COMP_PARAM->fLineNumbers;
   int iPass;

   if( ! HB_COMP_ISSUPPORTED(HB_COMPFLAG_OPTJUMP) )
      return;

   hb_compOptimizePCode( HB_COMP_PARAM, HB_COMP_PARAM->functions.pLast );
   hb_compCodeTraceMarkDead( HB_COMP_PARAM, HB_COMP_PARAM->functions.pLast );

   for( iPass = 0; iPass < 4 && !HB_COMP_PARAM->fExit; ++iPass )
   {
      LONG lOffset;

      if( iPass == 3 && fLineStrip )
      {
         hb_compStripFuncLines( HB_COMP_PARAM->functions.pLast );
         fLineStrip = FALSE;
      }

      if( HB_COMP_PARAM->functions.pLast->iJumps > 0 )
      {
         pJumps = HB_COMP_PARAM->functions.pLast->pJumps;
         iJump = HB_COMP_PARAM->functions.pLast->iJumps - 1;

         do
         {
            ulJumpAddr = pJumps[ iJump ];

            /*
             * optimize existing jumps, it will be good to also join
             * unconditional jump chain calculating total jump offset but
             * it will be necessary to add some code to protect against
             * infinite loop which will appear when we add optimization
             * for the PCODE sequences like:
             *
             *    HB_P_{FALSE|TRUE},
             * [ no jump targets or stack modification here ]
             *    HB_P_JUMP{FALSE|TRUE}*,
             *
             * I'll think about sth like that later, [druzus]
             */
            switch( pCode[ ulJumpAddr ] )
            {
               case HB_P_JUMPNEAR:
                  if( ( signed char ) pCode[ ulJumpAddr + 1 ] == 2 )
                     hb_compNOOPfill( HB_COMP_PARAM->functions.pLast, ulJumpAddr, 2, FALSE, FALSE );
                  break;

               case HB_P_JUMPFALSENEAR:
               case HB_P_JUMPTRUENEAR:
                  if( ( signed char ) pCode[ ulJumpAddr + 1 ] == 2 )
                     hb_compNOOPfill( HB_COMP_PARAM->functions.pLast, ulJumpAddr, 2, TRUE, FALSE );
                  break;

               case HB_P_JUMP:
                  lOffset = HB_PCODE_MKSHORT( &pCode[ ulJumpAddr + 1 ] );
                  if( lOffset == 3 )
                     hb_compNOOPfill( HB_COMP_PARAM->functions.pLast, ulJumpAddr, 3, FALSE, FALSE );
                  else if( HB_LIM_INT8( lOffset ) )
                  {
                     pCode[ ulJumpAddr ] = HB_P_JUMPNEAR;
                     hb_compNOOPfill( HB_COMP_PARAM->functions.pLast, ulJumpAddr + 2, 1, FALSE, FALSE );
                  }
                  break;

               case HB_P_JUMPFALSE:
                  lOffset = HB_PCODE_MKSHORT( &pCode[ ulJumpAddr + 1 ] );
                  if( lOffset == 3 )
                     hb_compNOOPfill( HB_COMP_PARAM->functions.pLast, ulJumpAddr, 3, TRUE, FALSE );
                  else if( HB_LIM_INT8( lOffset ) )
                  {
                     pCode[ ulJumpAddr ] = HB_P_JUMPFALSENEAR;
                     hb_compNOOPfill( HB_COMP_PARAM->functions.pLast, ulJumpAddr + 2, 1, FALSE, FALSE );
                  }
                  break;

               case HB_P_JUMPTRUE:
                  lOffset = HB_PCODE_MKSHORT( &pCode[ ulJumpAddr + 1 ] );
                  if( lOffset == 3 )
                     hb_compNOOPfill( HB_COMP_PARAM->functions.pLast, ulJumpAddr, 3, TRUE, FALSE );
                  else if( HB_LIM_INT8( lOffset ) )
                  {
                     pCode[ ulJumpAddr ] = HB_P_JUMPTRUENEAR;
                     hb_compNOOPfill( HB_COMP_PARAM->functions.pLast, ulJumpAddr + 2, 1, FALSE, FALSE );
                  }
                  break;

               case HB_P_JUMPFAR:
                  lOffset = HB_PCODE_MKINT24( &pCode[ ulJumpAddr + 1 ] );
                  if( lOffset == 4 )
                     hb_compNOOPfill( HB_COMP_PARAM->functions.pLast, ulJumpAddr, 4, FALSE, FALSE );
                  else if( iPass > 0 && HB_LIM_INT16( lOffset ) )
                  {
                     if( HB_LIM_INT8( lOffset ) )
                     {
                        pCode[ ulJumpAddr ] = HB_P_JUMPNEAR;
                        hb_compNOOPfill( HB_COMP_PARAM->functions.pLast, ulJumpAddr + 2, 2, FALSE, FALSE );
                     }
                     else
                     {
                        pCode[ ulJumpAddr ] = HB_P_JUMP;
                        hb_compNOOPfill( HB_COMP_PARAM->functions.pLast, ulJumpAddr + 3, 1, FALSE, FALSE );
                     }
                  }
                  break;

               case HB_P_JUMPFALSEFAR:
                  lOffset = HB_PCODE_MKINT24( &pCode[ ulJumpAddr + 1 ] );
                  if( lOffset == 4 )
                     hb_compNOOPfill( HB_COMP_PARAM->functions.pLast, ulJumpAddr, 4, TRUE, FALSE );
                  else if( iPass > 0 && HB_LIM_INT16( lOffset ) )
                  {
                     if( HB_LIM_INT8( lOffset ) )
                     {
                        pCode[ ulJumpAddr ] = HB_P_JUMPFALSENEAR;
                        hb_compNOOPfill( HB_COMP_PARAM->functions.pLast, ulJumpAddr + 2, 2, FALSE, FALSE );
                     }
                     else
                     {
                        pCode[ ulJumpAddr ] = HB_P_JUMPFALSE;
                        hb_compNOOPfill( HB_COMP_PARAM->functions.pLast, ulJumpAddr + 3, 1, FALSE, FALSE );
                     }
                  }
                  break;

               case HB_P_JUMPTRUEFAR:
                  lOffset = HB_PCODE_MKINT24( &pCode[ ulJumpAddr + 1 ] );
                  if( lOffset == 4 )
                     hb_compNOOPfill( HB_COMP_PARAM->functions.pLast, ulJumpAddr, 4, TRUE, FALSE );
                  else if( iPass > 0 && HB_LIM_INT16( lOffset ) )
                  {
                     if( HB_LIM_INT8( lOffset ) )
                     {
                        pCode[ ulJumpAddr ] = HB_P_JUMPTRUENEAR;
                        hb_compNOOPfill( HB_COMP_PARAM->functions.pLast, ulJumpAddr + 2, 2, FALSE, FALSE );
                     }
                     else
                     {
                        pCode[ ulJumpAddr ] = HB_P_JUMPTRUE;
                        hb_compNOOPfill( HB_COMP_PARAM->functions.pLast, ulJumpAddr + 3, 1, FALSE, FALSE );
                     }
                  }
                  break;
            }

            /* remove dummy jumps (over dead code) */
            if( pCode[ ulJumpAddr ] == HB_P_NOOP ||
                pCode[ ulJumpAddr ] == HB_P_POP )
            {
               if( HB_COMP_PARAM->functions.pLast->iJumps > iJump + 1 )
                  memmove( &pJumps[ iJump ], &pJumps[ iJump + 1 ],
                           ( HB_COMP_PARAM->functions.pLast->iJumps - iJump - 1 ) *
                           sizeof( ULONG ) );
               HB_COMP_PARAM->functions.pLast->iJumps--;
            }
         }
         while( iJump-- );

         if( HB_COMP_PARAM->functions.pLast->iJumps == 0 )
         {
            hb_xfree( HB_COMP_PARAM->functions.pLast->pJumps );
            HB_COMP_PARAM->functions.pLast->pJumps = NULL;
         }
      }

      if( HB_COMP_PARAM->functions.pLast->iNOOPs == 0 )
      {
         if( iPass == 0 )
            continue;
         if( fLineStrip )
            hb_compStripFuncLines( HB_COMP_PARAM->functions.pLast );
         if( HB_COMP_PARAM->functions.pLast->iNOOPs == 0 )
            return;
      }

      pNOOPs = HB_COMP_PARAM->functions.pLast->pNOOPs;

      /* Needed so the pasting of PCODE pieces below will work correctly */
      qsort( ( void * ) pNOOPs, HB_COMP_PARAM->functions.pLast->iNOOPs, sizeof( ULONG ), hb_compSort_ULONG );

      if( HB_COMP_PARAM->functions.pLast->iJumps )
      {
         LONG * plSizes, * plShifts;
         ULONG ulSize;

         pJumps = HB_COMP_PARAM->functions.pLast->pJumps;
         ulSize = sizeof( LONG ) * HB_COMP_PARAM->functions.pLast->iJumps;
         plSizes = ( LONG * ) hb_xgrab( ulSize );
         plShifts = ( LONG * ) hb_xgrab( ulSize );

         for( iJump = 0; iJump < HB_COMP_PARAM->functions.pLast->iJumps; iJump++ )
            plSizes[ iJump ] = plShifts[ iJump ] = 0;

         /* First Scan NOOPS - Adjust Jump addresses. */
         for( iNOOP = 0; iNOOP < HB_COMP_PARAM->functions.pLast->iNOOPs; iNOOP++ )
         {
            /* Adjusting preceding jumps that pooint to code beyond the current NOOP
               or trailing backward jumps pointing to lower address. */
            for( iJump = 0; iJump < HB_COMP_PARAM->functions.pLast->iJumps ; iJump++ )
            {
               ulJumpAddr = pJumps[ iJump ];
               switch( pCode[ ulJumpAddr ] )
               {
                  case HB_P_JUMPNEAR:
                  case HB_P_JUMPFALSENEAR:
                  case HB_P_JUMPTRUENEAR:
                     lOffset = ( signed char ) pCode[ ulJumpAddr + 1 ];
                     break;

                  case HB_P_JUMP:
                  case HB_P_JUMPFALSE:
                  case HB_P_JUMPTRUE:
                     lOffset = HB_PCODE_MKSHORT( &pCode[ ulJumpAddr + 1 ] );
                     break;

                  case HB_P_JUMPFAR:
                  case HB_P_JUMPTRUEFAR:
                  case HB_P_JUMPFALSEFAR:
                  case HB_P_ALWAYSBEGIN:
                  case HB_P_SEQALWAYS:
                  case HB_P_SEQBEGIN:
                  case HB_P_SEQEND:
                     lOffset = HB_PCODE_MKINT24( &pCode[ ulJumpAddr + 1 ] );
                     break;

                  default:
                     hb_compGenError( HB_COMP_PARAM, hb_comp_szErrors, 'F', HB_COMP_ERR_JUMP_NOT_FOUND, NULL, NULL );
                     continue;
               }

               /* update jump size */
               if( lOffset > 0 ) /* forward (positive) jump */
               {
                  /* Only if points to code beyond the current fix. */
                  if( pNOOPs[ iNOOP ] > ulJumpAddr &&
                      pNOOPs[ iNOOP ] < ( ULONG ) ( ulJumpAddr + lOffset ) )
                     plSizes[ iJump ]--;
               }
               else /* if( lOffset < 0 ) - backword (negative) jump */
               {
                  /* Only if points to code prior the current fix. */
                  if( pNOOPs[ iNOOP ] < ulJumpAddr &&
                      pNOOPs[ iNOOP ] >= ( ULONG ) ( ulJumpAddr + lOffset ) )
                     plSizes[ iJump ]++;
               }

               /* update jump address */
               if( pNOOPs[ iNOOP ] < ulJumpAddr )
                  plShifts[ iJump ]++;
            }
         }

         for( iJump = 0; iJump < HB_COMP_PARAM->functions.pLast->iJumps; iJump++ )
         {
            lOffset = plSizes[ iJump ];
            if( lOffset != 0 )
            {
               ulJumpAddr = pJumps[ iJump ];
               switch( pCode[ ulJumpAddr ] )
               {
                  case HB_P_JUMPNEAR:
                  case HB_P_JUMPFALSENEAR:
                  case HB_P_JUMPTRUENEAR:
                     lOffset += ( signed char ) pCode[ ulJumpAddr + 1 ];
                     pCode[ ulJumpAddr + 1 ] = HB_LOBYTE( lOffset );
                     break;

                  case HB_P_JUMP:
                  case HB_P_JUMPFALSE:
                  case HB_P_JUMPTRUE:
                     lOffset += HB_PCODE_MKSHORT( &pCode[ ulJumpAddr + 1 ] );
                     HB_PUT_LE_UINT16( &pCode[ ulJumpAddr + 1 ], lOffset );
                     break;

                  default:
                     lOffset += HB_PCODE_MKINT24( &pCode[ ulJumpAddr + 1 ] );
                     HB_PUT_LE_UINT24( &pCode[ ulJumpAddr + 1 ], lOffset );
                     break;
               }
            }
            pJumps[ iJump ] -= plShifts[ iJump ];
         }
         hb_xfree( plSizes );
         hb_xfree( plShifts );
      }

      ulOptimized = ulNextByte = 0;
      /* Second Scan, after all adjustements been made, we can copy the optimized code. */
      for( iNOOP = 0; iNOOP < HB_COMP_PARAM->functions.pLast->iNOOPs; iNOOP++ )
      {
         ulBytes2Copy = ( pNOOPs[ iNOOP ] - ulNextByte ) ;

         memmove( pCode + ulOptimized, pCode + ulNextByte, ulBytes2Copy );

         ulOptimized += ulBytes2Copy;
         ulNextByte  += ulBytes2Copy;

         /* Skip the NOOP and point to next valid byte */
         ulNextByte++;
      }

      ulBytes2Copy = ( HB_COMP_PARAM->functions.pLast->lPCodePos - ulNextByte ) ;
      memmove( pCode + ulOptimized, pCode + ulNextByte, ulBytes2Copy );
      ulOptimized += ulBytes2Copy;

      HB_COMP_PARAM->functions.pLast->lPCodePos  = ulOptimized;
      HB_COMP_PARAM->functions.pLast->lPCodeSize = ulOptimized;

      hb_xfree( HB_COMP_PARAM->functions.pLast->pNOOPs );
      HB_COMP_PARAM->functions.pLast->pNOOPs = NULL;
      HB_COMP_PARAM->functions.pLast->iNOOPs = 0;

      if( iPass <= 1 )
      {
         hb_compOptimizePCode( HB_COMP_PARAM, HB_COMP_PARAM->functions.pLast );
         hb_compCodeTraceMarkDead( HB_COMP_PARAM, HB_COMP_PARAM->functions.pLast );
      }
   }
}

static void hb_compOptimizeFrames( HB_COMP_DECL, PFUNCTION pFunc )
{
   USHORT w;

   if( pFunc == NULL )
      return;

   if( pFunc == HB_COMP_PARAM->pInitFunc )
   {
      if( pFunc->pCode[ 0 ] == HB_P_STATICS &&
          pFunc->pCode[ 5 ] == HB_P_SFRAME )
      {
         hb_compSymbolFind( HB_COMP_PARAM, HB_COMP_PARAM->pInitFunc->szName, &w, HB_SYM_FUNCNAME );
         pFunc->pCode[ 1 ] = HB_LOBYTE( w );
         pFunc->pCode[ 2 ] = HB_HIBYTE( w );
         pFunc->pCode[ 6 ] = HB_LOBYTE( w );
         pFunc->pCode[ 7 ] = HB_HIBYTE( w );

         /* Remove the SFRAME pcode if there's no global static
            initialization: */

         /* NOTE: For some reason this will not work for the static init
            function, so I'm using an ugly hack instead. [vszakats] */
/*       if( !( pFunc->bFlags & FUN_USES_STATICS ) ) */
         if( pFunc->pCode[ 8 ] == HB_P_ENDPROC )
         {
            pFunc->lPCodePos -= 3;
            memmove( pFunc->pCode + 5, pFunc->pCode + 8, pFunc->lPCodePos - 5 );
         }
         else /* Check Global Statics. */
         {
            /* PVAR pVar = pFunc->pStatics; */
            PVAR pVar = HB_COMP_PARAM->functions.pFirst->pStatics;

            while( pVar )
            {
               /*printf( "\nChecking: %s Used: %i\n", pVar->szName, pVar->iUsed );*/

               if( ! ( pVar->iUsed & VU_USED ) && (pVar->iUsed & VU_INITIALIZED) )
                  hb_compGenWarning( HB_COMP_PARAM, hb_comp_szWarnings, 'W', HB_COMP_WARN_VAL_NOT_USED, pVar->szName, NULL );

               /* May have been initialized in previous execution of the function.
                  else if( ( pVar->iUsed & VU_USED ) && ! ( pVar->iUsed & VU_INITIALIZED ) )
                  hb_compGenWarning( HB_COMP_PARAM, hb_comp_szWarnings, 'W', HB_COMP_WARN_NOT_INITIALIZED, pVar->szName, NULL );
               */
               pVar = pVar->pNext;
            }
         }
      }
   }
   else if( pFunc->pCode[ 0 ] == HB_P_FRAME && pFunc->pCode[ 3 ] == HB_P_SFRAME )
   {
      PVAR pLocal;
      int iLocals = 0, iOffset = 0;
      BOOL bSkipFRAME;
      BOOL bSkipSFRAME;

      pLocal = pFunc->pLocals;

      while( pLocal )
      {
         pLocal = pLocal->pNext;
         iLocals++;
      }

      if( pFunc->bFlags & FUN_USES_STATICS )
      {
         hb_compSymbolFind( HB_COMP_PARAM, HB_COMP_PARAM->pInitFunc->szName, &w, HB_SYM_FUNCNAME );
         pFunc->pCode[ 4 ] = HB_LOBYTE( w );
         pFunc->pCode[ 5 ] = HB_HIBYTE( w );
         bSkipSFRAME = FALSE;
      }
      else
         bSkipSFRAME = TRUE;

      if( iLocals || pFunc->wParamCount )
      {
         /* Parameters declared with PARAMETERS statement are not
          * placed in the local variable list.
          */
         if( pFunc->bFlags & FUN_USES_LOCAL_PARAMS )
            iLocals -= pFunc->wParamCount;

         if( iLocals > 255 )
         {
            /* more then 255 local variables,
             * make a room for HB_P_LARGE[V]FRAME
             */
            hb_compGenPCode1( 0, HB_COMP_PARAM );
            memmove( pFunc->pCode + 4, pFunc->pCode + 3, pFunc->lPCodePos - 4 );
            pFunc->pCode[ 0 ] = HB_P_LARGEFRAME;
            pFunc->pCode[ 1 ] = HB_LOBYTE( iLocals );
            pFunc->pCode[ 2 ] = HB_HIBYTE( iLocals );
            pFunc->pCode[ 3 ] = ( BYTE )( pFunc->wParamCount );
            iOffset = 1;
         }
         else
         {
            pFunc->pCode[ 1 ] = ( BYTE )( iLocals );
            pFunc->pCode[ 2 ] = ( BYTE )( pFunc->wParamCount );
         }
         bSkipFRAME = FALSE;
      }
      else
         /* Skip LOCALs frame only when function is not declared with
          * variable number of parameters (HB_P_[LARGE]VFRAME)
          */
         bSkipFRAME = !pFunc->fVParams;

      /* Remove the frame pcodes if they are not needed */
      if( bSkipFRAME )
      {
         if( bSkipSFRAME )
         {
            pFunc->lPCodePos -= 6;
            memmove( pFunc->pCode, pFunc->pCode + 6, pFunc->lPCodePos );
         }
         else
         {
            pFunc->lPCodePos -= 3;
            memmove( pFunc->pCode, pFunc->pCode + 3, pFunc->lPCodePos );
         }
      }
      else
      {
         if( pFunc->fVParams )
            pFunc->pCode[ 0 ] = iOffset ? HB_P_LARGEVFRAME : HB_P_VFRAME;

         if( bSkipSFRAME )
         {
            pFunc->lPCodePos -= 3;
            memmove( pFunc->pCode + 3 + iOffset, pFunc->pCode + 6 + iOffset,
                     pFunc->lPCodePos - 3 - iOffset );
         }
      }
   }
}

static void hb_compFinalizeFunction( HB_COMP_DECL ) /* fixes all last defined function returns jumps offsets */
{
   PFUNCTION pFunc = HB_COMP_PARAM->functions.pLast;

   if( pFunc )
   {
      if( ( pFunc->bFlags & FUN_WITH_RETURN ) == 0 )
      {
         /* The last statement in a function/procedure was not a RETURN
          * Generate end-of-procedure pcode
          */
         hb_compGenPCode1( HB_P_ENDPROC, HB_COMP_PARAM );
      }

      hb_compCheckUnclosedStru( HB_COMP_PARAM, pFunc );

      hb_compRTVariableKill( HB_COMP_PARAM, pFunc );
      hb_compSwitchKill( HB_COMP_PARAM, pFunc );
      hb_compElseIfKill( pFunc );
      hb_compLoopKill( pFunc );

      if( !pFunc->bError )
      {
         if( pFunc->wParamCount && !( pFunc->bFlags & FUN_USES_LOCAL_PARAMS ) )
         {
            /* There was a PARAMETERS statement used.
             * NOTE: This fixes local variables references in a case when
             * there is PARAMETERS statement after a LOCAL variable declarations.
             * All local variables are numbered from 1 - which means use first
             * item from the eval stack. However if PARAMETERS statement is used
             * then there are additional items on the eval stack - the
             * function arguments. Then first local variable is at the position
             * (1 + <number of arguments>). We cannot fix this numbering
             * because the PARAMETERS statement can be used even at the end
             * of function body when all local variables are already created.
             */
            hb_compFixFuncPCode( HB_COMP_PARAM, pFunc );
         }

         hb_compOptimizeJumps( HB_COMP_PARAM );
      }

      if( HB_COMP_PARAM->iWarnings )
      {
         PVAR pVar;

         pVar = pFunc->pLocals;
         while( pVar )
         {
            if( pVar->szName && pFunc->szName && pFunc->szName[0] && (! ( pVar->iUsed & VU_USED )) )
            {
               char szFun[ 256 ];
               snprintf( szFun, sizeof( szFun ), "%s(%i)", pFunc->szName, pVar->iDeclLine );
               hb_compGenWarning( HB_COMP_PARAM, hb_comp_szWarnings, 'W', HB_COMP_WARN_VAR_NOT_USED, pVar->szName, szFun );
            }

            pVar = pVar->pNext;
         }

         pVar = pFunc->pStatics;
         while( pVar )
         {
            if( pVar->szName && pFunc->szName && pFunc->szName[0] && ! ( pVar->iUsed & VU_USED ) )
            {
               char szFun[ 256 ];
               snprintf( szFun, sizeof( szFun ), "%s(%i)", pFunc->szName, pVar->iDeclLine );
               hb_compGenWarning( HB_COMP_PARAM, hb_comp_szWarnings, 'W', HB_COMP_WARN_VAR_NOT_USED, pVar->szName, szFun );
            }

            pVar = pVar->pNext;
         }

         /* Check if the function returned some value
          */
         if( (pFunc->bFlags & FUN_WITH_RETURN) == 0 &&
             (pFunc->bFlags & FUN_PROCEDURE) == 0 )
            hb_compGenWarning( HB_COMP_PARAM, hb_comp_szWarnings, 'W', HB_COMP_WARN_FUN_WITH_NO_RETURN,
                               pFunc->szName, NULL );
      }
   }
}

/*
 * This function creates and initialises the _FUNC structure
 */
static PFUNCTION hb_compFunctionNew( HB_COMP_DECL, char * szName, HB_SYMBOLSCOPE cScope )
{
   PFUNCTION pFunc;

   pFunc = ( PFUNCTION ) hb_xgrab( sizeof( _FUNC ) );
   memset( pFunc, 0, sizeof( _FUNC ) );

   pFunc->szName          = szName;
   pFunc->cScope          = cScope;
   pFunc->iStaticsBase    = HB_COMP_PARAM->iStaticCnt;
   pFunc->bLateEval       = TRUE;
   pFunc->fVParams        = FALSE;
   pFunc->bError          = FALSE;

   return pFunc;
}

static PINLINE hb_compInlineNew( HB_COMP_DECL, char * szName, int iLine )
{
   PINLINE pInline;

   pInline = ( PINLINE ) hb_xgrab( sizeof( _INLINE ) );

   pInline->szName     = szName;
   pInline->pCode      = NULL;
   pInline->lPCodeSize = 0;
   pInline->pNext      = NULL;
   pInline->szFileName = hb_compIdentifierNew( HB_COMP_PARAM,
                  hb_pp_fileName( HB_COMP_PARAM->pLex->pPP ), HB_IDENT_COPY );
   pInline->iLine      = iLine;

   return pInline;
}

/* NOTE: Names of variables and functions are released in hbident.c on exit */
static PFUNCTION hb_compFunctionKill( HB_COMP_DECL, PFUNCTION pFunc )
{
   PFUNCTION pNext = pFunc->pNext;
   HB_ENUMERATOR_PTR pEVar;
   PVAR pVar;

   hb_compRTVariableKill( HB_COMP_PARAM, pFunc );
   hb_compSwitchKill( HB_COMP_PARAM, pFunc );
   hb_compElseIfKill( pFunc );
   hb_compLoopKill( pFunc );

   while( pFunc->pLocals )
   {
      pVar = pFunc->pLocals;
      pFunc->pLocals = pVar->pNext;
      hb_xfree( ( void * ) pVar );
   }

   while( pFunc->pStatics )
   {
      pVar = pFunc->pStatics;
      pFunc->pStatics = pVar->pNext;
      hb_xfree( ( void * ) pVar );
   }

   while( pFunc->pFields )
   {
      pVar = pFunc->pFields;
      pFunc->pFields = pVar->pNext;
      hb_xfree( ( void * ) pVar );
   }

   while( pFunc->pMemvars )
   {
      pVar = pFunc->pMemvars;
      pFunc->pMemvars = pVar->pNext;
      hb_xfree( ( void * ) pVar );
   }

   while( pFunc->pDetached )
   {
      pVar = pFunc->pDetached;
      pFunc->pDetached = pVar->pNext;
      hb_xfree( ( void * ) pVar );
   }

   while( pFunc->pPrivates )
   {
      pVar = pFunc->pPrivates;
      pFunc->pPrivates = pVar->pNext;
      hb_xfree( ( void * ) pVar );
   }

   while( pFunc->pEnum )
   {
      pEVar = pFunc->pEnum;
      pFunc->pEnum = pEVar->pNext;
      hb_xfree( pEVar );      
   }

   /* Release the NOOP array. */
   if( pFunc->pNOOPs )
      hb_xfree( ( void * ) pFunc->pNOOPs );

   /* Release the Jumps array. */
   if( pFunc->pJumps )
      hb_xfree( ( void * ) pFunc->pJumps );

   hb_xfree( ( void * ) pFunc->pCode );
   hb_xfree( ( void * ) pFunc );

   return pNext;
}

/*
 * This function adds the name of called function into the list
 * as they have to be placed on the symbol table later than the
 * first public symbol
 */
static PFUNCALL hb_compFunCallAdd( HB_COMP_DECL, char * szFunctionName )
{
   PFUNCALL pFunc = ( PFUNCALL ) hb_xgrab( sizeof( _FUNCALL ) );

   pFunc->szName = szFunctionName;
   pFunc->pNext  = NULL;
   if( ! HB_COMP_PARAM->funcalls.iCount )
   {
      HB_COMP_PARAM->funcalls.pFirst = pFunc;
      HB_COMP_PARAM->funcalls.pLast  = pFunc;
   }
   else
   {
      HB_COMP_PARAM->funcalls.pLast->pNext = pFunc;
      HB_COMP_PARAM->funcalls.pLast = pFunc;
   }
   HB_COMP_PARAM->funcalls.iCount++;

   return pFunc;
}

/*
 * This function adds the name of external symbol into the list of externals
 * as they have to be placed on the symbol table later than the first
 * public symbol
 */
void hb_compExternAdd( HB_COMP_DECL, char * szExternName, HB_SYMBOLSCOPE cScope ) /* defines a new extern name */
{
   PEXTERN pExtern = ( PEXTERN ) hb_xgrab( sizeof( _EXTERN ) ), pLast;

   if( strcmp( "_GET_", szExternName ) == 0 )
   {
      /* special function to implement @ GET statement */
      hb_compExternAdd( HB_COMP_PARAM, "__GETA", 0 );
      pExtern->szName = "__GET";
   }
   else
   {
      pExtern->szName = szExternName;
   }
   pExtern->cScope = cScope;
   pExtern->pNext  = NULL;

   if( HB_COMP_PARAM->externs == NULL )
      HB_COMP_PARAM->externs = pExtern;
   else
   {
      pLast = HB_COMP_PARAM->externs;
      while( pLast->pNext )
         pLast = pLast->pNext;
      pLast->pNext = pExtern;
   }
}

static void hb_compAddFunc( HB_COMP_DECL, PFUNCTION pFunc )
{
   while( HB_COMP_PARAM->functions.pLast &&
       !HB_COMP_PARAM->functions.pLast->szName )
   {
      PFUNCTION pBlock = HB_COMP_PARAM->functions.pLast;
      HB_COMP_PARAM->functions.pLast = pBlock->pOwner;
      hb_compFunctionKill( HB_COMP_PARAM, pBlock );
   }

   if( HB_COMP_PARAM->functions.iCount == 0 )
      HB_COMP_PARAM->functions.pFirst = pFunc;
   else
      HB_COMP_PARAM->functions.pLast->pNext = pFunc;
   HB_COMP_PARAM->functions.pLast = pFunc;
   HB_COMP_PARAM->functions.iCount++;
}

/*
 * Stores a Clipper defined function/procedure
 * szFunName - name of a function
 * cScope    - scope of a function
 * iType     - FUN_PROCEDURE if a procedure or 0
 */
void hb_compFunctionAdd( HB_COMP_DECL, char * szFunName, HB_SYMBOLSCOPE cScope, int iType )
{
   PCOMSYMBOL   pSym;
   PFUNCTION pFunc;
   char * szFunction;

   hb_compFinalizeFunction( HB_COMP_PARAM );    /* fix all previous function returns offsets */

   if( cScope & (HB_FS_INIT | HB_FS_EXIT) )
   {
      char szNewName[ HB_SYMBOL_NAME_LEN + 1 ];
      int iLen;

      iLen = strlen( szFunName );
      if( iLen >= HB_SYMBOL_NAME_LEN )
         iLen = HB_SYMBOL_NAME_LEN - 1;
      memcpy( szNewName, szFunName, iLen );
      szNewName[ iLen ] ='$';
      szNewName[ iLen + 1 ] = '\0';
      szFunName = hb_compIdentifierNew( HB_COMP_PARAM, szNewName, HB_IDENT_COPY );
   }
   pFunc = hb_compFunctionFind( HB_COMP_PARAM, szFunName );
   if( pFunc )
   {
      /* The name of a function/procedure is already defined */
      if( pFunc != HB_COMP_PARAM->functions.pFirst || HB_COMP_PARAM->fStartProc )
         /* it is not a starting procedure that was automatically created */
         hb_compGenError( HB_COMP_PARAM, hb_comp_szErrors, 'F', HB_COMP_ERR_FUNC_DUPL, szFunName, NULL );
   }

   szFunction = hb_compReservedName( szFunName );
   if( szFunction && !( HB_COMP_PARAM->functions.iCount == 0 && !HB_COMP_PARAM->fStartProc ) )
   {
      /* We are ignoring it when it is the name of PRG file and we are
       * not creating implicit starting procedure
       */
      hb_compGenError( HB_COMP_PARAM, hb_comp_szErrors, 'E', HB_COMP_ERR_FUNC_RESERVED, szFunction, szFunName );
   }

   HB_COMP_PARAM->iFunctionCnt++;

   pSym = hb_compSymbolFind( HB_COMP_PARAM, szFunName, NULL, HB_SYM_FUNCNAME );
   if( ! pSym )
   {
      /* there is not a symbol on the symbol table for this function name */
      pSym = hb_compSymbolAdd( HB_COMP_PARAM, szFunName, NULL, HB_SYM_FUNCNAME );
   }
   if( pSym )
      pSym->cScope |= cScope | HB_FS_LOCAL;

   pFunc = hb_compFunctionNew( HB_COMP_PARAM, szFunName, cScope );
   pFunc->bFlags |= iType;

   hb_compAddFunc( HB_COMP_PARAM, pFunc );

   HB_COMP_PARAM->lastLinePos = 0;  /* optimization of line numbers opcode generation */
   HB_COMP_PARAM->ilastLineErr = 0; /* position of last syntax error (line number) */

   hb_compGenPCode3( HB_P_FRAME, 0, 0, HB_COMP_PARAM );     /* frame for locals and parameters */
   hb_compGenPCode3( HB_P_SFRAME, 0, 0, HB_COMP_PARAM );    /* frame for statics variables */

   if( HB_COMP_PARAM->fDebugInfo )
      hb_compGenModuleName( HB_COMP_PARAM, szFunName );
   else
      HB_COMP_PARAM->lastLine = -1;
}

PINLINE hb_compInlineAdd( HB_COMP_DECL, char * szFunName, int iLine )
{
   PINLINE pInline;
   PCOMSYMBOL   pSym;

   if( szFunName )
   {
      pSym = hb_compSymbolFind( HB_COMP_PARAM, szFunName, NULL, HB_SYM_FUNCNAME );
      if( ! pSym )
      {
         pSym = hb_compSymbolAdd( HB_COMP_PARAM, szFunName, NULL, HB_SYM_FUNCNAME );
      }
      if( pSym )
      {
         pSym->cScope |= HB_FS_STATIC | HB_FS_LOCAL;
      }
   }
   pInline = hb_compInlineNew( pComp, szFunName, iLine );

   if( HB_COMP_PARAM->inlines.iCount == 0 )
   {
      HB_COMP_PARAM->inlines.pFirst = pInline;
      HB_COMP_PARAM->inlines.pLast  = pInline;
   }
   else
   {
      HB_COMP_PARAM->inlines.pLast->pNext = pInline;
      HB_COMP_PARAM->inlines.pLast = pInline;
   }

   HB_COMP_PARAM->inlines.iCount++;

   return pInline;
}

/* create an ANNOUNCEd procedure
 */
void hb_compAnnounce( HB_COMP_DECL, char * szFunName )
{
   PFUNCTION pFunc;

   pFunc = hb_compFunctionFind( HB_COMP_PARAM, szFunName );
   if( pFunc )
   {
      /* there is a function/procedure defined already - ANNOUNCEd procedure
       * have to be a public symbol - check if existing symbol is public
       */
      if( pFunc->cScope & HB_FS_STATIC )
         hb_compGenError( HB_COMP_PARAM, hb_comp_szErrors, 'F', HB_COMP_ERR_FUNC_ANNOUNCE, szFunName, NULL );
   }
   else
   {
      PCOMSYMBOL pSym;

      /* create a new procedure
       */
      pSym = hb_compSymbolAdd( HB_COMP_PARAM, szFunName, NULL, HB_SYM_FUNCNAME );
      pSym->cScope = HB_FS_PUBLIC | HB_FS_LOCAL;

      pFunc = hb_compFunctionNew( HB_COMP_PARAM, szFunName, pSym->cScope );
      pFunc->bFlags |= FUN_PROCEDURE;

      hb_compAddFunc( HB_COMP_PARAM, pFunc );
      HB_COMP_PARAM->iFunctionCnt++;

      /* this function have a very limited functionality
       */
      hb_compGenPCode1( HB_P_ENDPROC, HB_COMP_PARAM );
   }
}

void hb_compGenBreak( HB_COMP_DECL )
{
   hb_compGenPushFunCall( "BREAK", HB_COMP_PARAM );
}

void hb_compExternGen( HB_COMP_DECL ) /* generates the symbols for the EXTERN names */
{
   PEXTERN pDelete;

   if( HB_COMP_PARAM->fDebugInfo )
      hb_compExternAdd( HB_COMP_PARAM, "__DBGENTRY", 0 );

   while( HB_COMP_PARAM->externs )
   {
      PCOMSYMBOL pSym = hb_compSymbolFind( HB_COMP_PARAM, HB_COMP_PARAM->externs->szName, NULL, HB_SYM_FUNCNAME );
      if( pSym )
      {
         if( ! hb_compFunCallFind( HB_COMP_PARAM, HB_COMP_PARAM->externs->szName ) )
            hb_compFunCallAdd( HB_COMP_PARAM, HB_COMP_PARAM->externs->szName );
      }
      else
      {
          pSym = hb_compSymbolAdd( HB_COMP_PARAM, HB_COMP_PARAM->externs->szName, NULL, HB_SYM_FUNCNAME );
          hb_compFunCallAdd( HB_COMP_PARAM, HB_COMP_PARAM->externs->szName );
      }
      pSym->cScope |= HB_COMP_PARAM->externs->cScope;
      pDelete = HB_COMP_PARAM->externs;
      HB_COMP_PARAM->externs = HB_COMP_PARAM->externs->pNext;
      hb_xfree( ( void * ) pDelete );
   }
}

PFUNCALL hb_compFunCallFind( HB_COMP_DECL, char * szFunctionName ) /* returns a previously called defined function */
{
   PFUNCALL pFunc = HB_COMP_PARAM->funcalls.pFirst;

   while( pFunc )
   {
      if( ! strcmp( pFunc->szName, szFunctionName ) )
         break;
      pFunc = pFunc->pNext;
   }
   return pFunc;
}

PFUNCTION hb_compFunctionFind( HB_COMP_DECL, char * szFunctionName ) /* returns a previously defined function */
{
   PFUNCTION pFunc = HB_COMP_PARAM->functions.pFirst;

   while( pFunc )
   {
      if( ! strcmp( pFunc->szName, szFunctionName ) )
         break;
      pFunc = pFunc->pNext;
   }
   return pFunc;
}

PINLINE hb_compInlineFind( HB_COMP_DECL, char * szFunctionName )
{
   PINLINE pInline = HB_COMP_PARAM->inlines.pFirst;

   while( pInline )
   {
      if( pInline->szName && strcmp( pInline->szName, szFunctionName ) == 0 )
         break;
      pInline = pInline->pNext;
   }
   return pInline;
}

/* check if function exists, ignore case of letters in function name */
static BOOL hb_compIsFunction( HB_COMP_DECL, char * szFunctionName )
{
   PFUNCTION pFunc = HB_COMP_PARAM->functions.pFirst;

   while( pFunc )
   {
      if( ! hb_stricmp( pFunc->szName, szFunctionName ) )
         break;
      pFunc = pFunc->pNext;
   }
   return pFunc != NULL;
}

static void hb_compNOOPadd( PFUNCTION pFunc, ULONG ulPos )
{
   pFunc->pCode[ ulPos ] = HB_P_NOOP;

   if( pFunc->iNOOPs )
      pFunc->pNOOPs = ( ULONG * ) hb_xrealloc( pFunc->pNOOPs, sizeof( ULONG ) * ( pFunc->iNOOPs + 1 ) );
   else
      pFunc->pNOOPs = ( ULONG * ) hb_xgrab( sizeof( ULONG ) );
   pFunc->pNOOPs[ pFunc->iNOOPs++ ] = ulPos;
}

/* NOTE: To disable jump optimization, just make this function a dummy one.
 [vszakats] */

static void hb_compPrepareOptimize( HB_COMP_DECL )
{
   if( HB_COMP_ISSUPPORTED(HB_COMPFLAG_OPTJUMP) )
   {
      PFUNCTION pFunc = HB_COMP_PARAM->functions.pLast;

      if( pFunc->iJumps )
         pFunc->pJumps = ( ULONG * ) hb_xrealloc( pFunc->pJumps, sizeof( ULONG ) * ( pFunc->iJumps + 1 ) );
      else
         pFunc->pJumps = ( ULONG * ) hb_xgrab( sizeof( ULONG ) );
      pFunc->pJumps[ pFunc->iJumps++ ] = ( ULONG ) ( pFunc->lPCodePos - 4 );
   }
}

ULONG hb_compGenJump( LONG lOffset, HB_COMP_DECL )
{
   if( !HB_LIM_INT24( lOffset ) )
      hb_compGenError( HB_COMP_PARAM, hb_comp_szErrors, 'F', HB_COMP_ERR_JUMP_TOO_LONG, NULL, NULL );

   hb_compGenPCode4( HB_P_JUMPFAR, HB_LOBYTE( lOffset ), HB_HIBYTE( lOffset ), ( BYTE ) ( ( lOffset >> 16 ) & 0xFF ), HB_COMP_PARAM );
   hb_compPrepareOptimize( HB_COMP_PARAM );

   return HB_COMP_PARAM->functions.pLast->lPCodePos - 3;
}

ULONG hb_compGenJumpFalse( LONG lOffset, HB_COMP_DECL )
{
   if( !HB_LIM_INT24( lOffset ) )
      hb_compGenError( HB_COMP_PARAM, hb_comp_szErrors, 'F', HB_COMP_ERR_JUMP_TOO_LONG, NULL, NULL );

   hb_compGenPCode4( HB_P_JUMPFALSEFAR, HB_LOBYTE( lOffset ), HB_HIBYTE( lOffset ), ( BYTE ) ( ( lOffset >> 16 ) & 0xFF ), HB_COMP_PARAM );
   hb_compPrepareOptimize( HB_COMP_PARAM );

   return HB_COMP_PARAM->functions.pLast->lPCodePos - 3;
}

ULONG hb_compGenJumpTrue( LONG lOffset, HB_COMP_DECL )
{
   if( !HB_LIM_INT24( lOffset ) )
      hb_compGenError( HB_COMP_PARAM, hb_comp_szErrors, 'F', HB_COMP_ERR_JUMP_TOO_LONG, NULL, NULL );

   hb_compGenPCode4( HB_P_JUMPTRUEFAR, HB_LOBYTE( lOffset ), HB_HIBYTE( lOffset ), ( BYTE ) ( ( lOffset >> 16 ) & 0xFF ), HB_COMP_PARAM );
   hb_compPrepareOptimize( HB_COMP_PARAM );

   return HB_COMP_PARAM->functions.pLast->lPCodePos - 3;
}

void hb_compGenJumpThere( ULONG ulFrom, ULONG ulTo, HB_COMP_DECL )
{
   BYTE * pCode = HB_COMP_PARAM->functions.pLast->pCode;
   LONG lOffset = ulTo - ulFrom + 1;

   if( HB_LIM_INT24( lOffset ) )
   {
      HB_PUT_LE_UINT24( &pCode[ ulFrom ], lOffset );
   }
   else
      hb_compGenError( HB_COMP_PARAM, hb_comp_szErrors, 'F', HB_COMP_ERR_JUMP_TOO_LONG, NULL, NULL );
}

void hb_compGenJumpHere( ULONG ulOffset, HB_COMP_DECL )
{
   hb_compGenJumpThere( ulOffset, HB_COMP_PARAM->functions.pLast->lPCodePos, HB_COMP_PARAM );
}

void hb_compLinePush( HB_COMP_DECL ) /* generates the pcode with the currently compiled source code line */
{
   if( HB_COMP_PARAM->fLineNumbers )
   {
      if( HB_COMP_PARAM->fDebugInfo && HB_COMP_PARAM->lastModule != HB_COMP_PARAM->currModule )
      {
         if( HB_COMP_PARAM->functions.pLast->pCode[ HB_COMP_PARAM->lastLinePos ] == HB_P_LINE &&
             HB_COMP_PARAM->functions.pLast->lPCodePos - HB_COMP_PARAM->lastLinePos == 3 )
            HB_COMP_PARAM->functions.pLast->lPCodePos -= 3;
         hb_compGenModuleName( HB_COMP_PARAM, NULL );
      }

      if( HB_COMP_PARAM->currLine != HB_COMP_PARAM->lastLine )
      {
         if( HB_COMP_PARAM->functions.pLast->lPCodePos - HB_COMP_PARAM->lastLinePos == 3 &&
             HB_COMP_PARAM->functions.pLast->pCode[ HB_COMP_PARAM->lastLinePos ] == HB_P_LINE )
         {
            HB_COMP_PARAM->functions.pLast->pCode[ HB_COMP_PARAM->lastLinePos + 1 ] = HB_LOBYTE( HB_COMP_PARAM->currLine );
            HB_COMP_PARAM->functions.pLast->pCode[ HB_COMP_PARAM->lastLinePos + 2 ] = HB_HIBYTE( HB_COMP_PARAM->currLine );
         }
         else
         {
            HB_COMP_PARAM->lastLinePos = HB_COMP_PARAM->functions.pLast->lPCodePos;
            hb_compGenPCode3( HB_P_LINE, HB_LOBYTE( HB_COMP_PARAM->currLine ),
                                         HB_HIBYTE( HB_COMP_PARAM->currLine ), HB_COMP_PARAM );
         }
         HB_COMP_PARAM->lastLine = HB_COMP_PARAM->currLine;
      }
   }

   if( HB_COMP_PARAM->functions.pLast->bFlags & FUN_BREAK_CODE )
   {
      /* previous line contained RETURN/BREAK/LOOP/EXIT statement */
      hb_compGenWarning( HB_COMP_PARAM, hb_comp_szWarnings, 'W', HB_COMP_WARN_UNREACHABLE, NULL, NULL );
      /* clear RETURN/BREAK flag */
      HB_COMP_PARAM->functions.pLast->bFlags &= ~ ( /*FUN_WITH_RETURN |*/ FUN_BREAK_CODE );
   }
}

/*
 * Test if we can generate statement (without line pushing)
 */
void hb_compStatmentStart( HB_COMP_DECL )
{
   if( ( HB_COMP_PARAM->functions.pLast->bFlags & FUN_STATEMENTS ) == 0 )
   {
      if( ! HB_COMP_PARAM->fStartProc && HB_COMP_PARAM->functions.iCount <= 1 &&
          HB_COMP_PARAM->functions.pLast != HB_COMP_PARAM->pInitFunc &&
          HB_COMP_PARAM->functions.pLast->szName )
      {
         hb_compGenError( HB_COMP_PARAM, hb_comp_szErrors, 'E', HB_COMP_ERR_OUTSIDE, NULL, NULL );
         return;
      }
      HB_COMP_PARAM->functions.pLast->bFlags |= FUN_STATEMENTS;
   }
}

void hb_compLinePushIfInside( HB_COMP_DECL ) /* generates the pcode with the currently compiled source code line */
{
   hb_compStatmentStart( HB_COMP_PARAM );
   hb_compLinePush( HB_COMP_PARAM );
}

/* Generates the pcode with the currently compiled source code line
 * if debug code was requested only
 */
void hb_compLinePushIfDebugger( HB_COMP_DECL )
{
   hb_compStatmentStart( HB_COMP_PARAM );

   if( HB_COMP_PARAM->fDebugInfo )
      hb_compLinePush( HB_COMP_PARAM );
   else
   {
      if( HB_COMP_PARAM->functions.pLast->bFlags & FUN_BREAK_CODE )
      {
         /* previous line contained RETURN/BREAK/LOOP/EXIT statement */
         hb_compGenWarning( HB_COMP_PARAM, hb_comp_szWarnings, 'W', HB_COMP_WARN_UNREACHABLE, NULL, NULL );
      }
      HB_COMP_PARAM->functions.pLast->bFlags &= ~ ( /*FUN_WITH_RETURN |*/ FUN_BREAK_CODE );  /* clear RETURN flag */
   }
}

/* generates the pcode with the currently compiled module and function name */
void hb_compGenModuleName( HB_COMP_DECL, char * szFunName )
{
   hb_compGenPCode1( HB_P_MODULENAME, HB_COMP_PARAM );
   hb_compGenPCodeN( ( BYTE * ) HB_COMP_PARAM->currModule,
                     strlen( HB_COMP_PARAM->currModule ), HB_COMP_PARAM );
   hb_compGenPCode1( ':', HB_COMP_PARAM );
   if( szFunName && *szFunName )
      hb_compGenPCodeN( ( BYTE * ) szFunName, strlen( szFunName ) + 1, HB_COMP_PARAM );
   else /* special version "filename:" when the file changes within function */
      hb_compGenPCode1( '\0', HB_COMP_PARAM );
   HB_COMP_PARAM->lastModule = HB_COMP_PARAM->currModule;
   HB_COMP_PARAM->lastLine = -1;
}

#if 0
void hb_compGenStaticName( char *szVarName, HB_COMP_DECL )
{
  if( HB_COMP_PARAM->fDebugInfo )
  {
      BYTE bGlobal = 0;
      PFUNCTION pFunc;
      int iVar;
      
      if( ! HB_COMP_PARAM->fStartProc && HB_COMP_PARAM->functions.iCount <= 1 )
      {
         /* Variable declaration is outside of function/procedure body.
            File-wide static variable
         */
         hb_compStaticDefStart( HB_COMP_PARAM );
         bGlobal = 1;
      }
      pFunc = HB_COMP_PARAM->functions.pLast;
      iVar = hb_compStaticGetPos( szVarName, pFunc );

      hb_compGenPCode4( HB_P_STATICNAME, bGlobal, HB_LOBYTE( iVar ), HB_HIBYTE( iVar ), HB_COMP_PARAM );
      hb_compGenPCodeN( ( BYTE * ) szVarName, strlen( szVarName ) + 1, HB_COMP_PARAM );

      if( bGlobal )
         hb_compStaticDefEnd( HB_COMP_PARAM );
   }
}
#endif

/*
 * Function generates passed pcode for passed runtime variable
 * (field or memvar)
 */
static void hb_compGenVarPCode( BYTE bPCode, char * szVarName, HB_COMP_DECL )
{
   USHORT wVar;
   PCOMSYMBOL pSym;

   /* Check if this variable name is placed into the symbol table
    */
   pSym = hb_compSymbolFind( HB_COMP_PARAM, szVarName, &wVar, HB_SYM_MEMVAR );
   if( ! pSym )
      pSym = hb_compSymbolAdd( HB_COMP_PARAM, szVarName, &wVar, HB_SYM_MEMVAR );
   pSym->cScope |= HB_FS_MEMVAR;

   if( bPCode == HB_P_PUSHALIASEDFIELD && wVar <= 255 )
      hb_compGenPCode2( HB_P_PUSHALIASEDFIELDNEAR, ( BYTE ) wVar, HB_COMP_PARAM );
   else if( bPCode == HB_P_POPALIASEDFIELD && wVar <= 255 )
      hb_compGenPCode2( HB_P_POPALIASEDFIELDNEAR, ( BYTE ) wVar, HB_COMP_PARAM );
   else
      hb_compGenPCode3( bPCode, HB_LOBYTE( wVar ), HB_HIBYTE( wVar ), HB_COMP_PARAM );
}

/*
 * Function generates pcode for undeclared variable
 */
static void hb_compGenVariablePCode( HB_COMP_DECL, BYTE bPCode, char * szVarName )
{
   BOOL bGenCode;
   /*
    * NOTE:
    * Clipper always assumes a memvar variable if undeclared variable
    * is popped (a value is asssigned to a variable).
    */
   if( HB_COMP_ISSUPPORTED( HB_COMPFLAG_HARBOUR ) )
      bGenCode = HB_COMP_PARAM->fForceMemvars;    /* harbour compatibility */
   else
      bGenCode = ( HB_COMP_PARAM->fForceMemvars || bPCode == HB_P_POPVARIABLE );

   if( bGenCode )
   {
      /* -v switch was used -> assume it is a memvar variable
       */
      hb_compGenWarning( HB_COMP_PARAM, hb_comp_szWarnings, 'W', HB_COMP_WARN_MEMVAR_ASSUMED, szVarName, NULL );

      if( bPCode == HB_P_POPVARIABLE )
         bPCode = HB_P_POPMEMVAR;
      else if( bPCode == HB_P_PUSHVARIABLE )
         bPCode = HB_P_PUSHMEMVAR;
      else
         bPCode = HB_P_PUSHMEMVARREF;
   }
   else
      hb_compGenWarning( HB_COMP_PARAM, hb_comp_szWarnings, 'W', HB_COMP_WARN_AMBIGUOUS_VAR, szVarName, NULL );

   hb_compGenVarPCode( bPCode, szVarName, HB_COMP_PARAM );
}

/* Generate a pcode for a field variable
 */
static void hb_compGenFieldPCode( HB_COMP_DECL, BYTE bPCode, PVAR pField )
{
   if( pField->szAlias )
   {  /* the alias was specified in FIELD declaration
       * Push alias symbol before the field symbol
       */
      if( bPCode == HB_P_POPFIELD )
         bPCode = HB_P_POPALIASEDFIELD;
      else if( bPCode == HB_P_PUSHFIELD )
         bPCode = HB_P_PUSHALIASEDFIELD;

      hb_compGenPushSymbol( pField->szAlias, HB_SYM_ALIAS, HB_COMP_PARAM );
   }
   hb_compGenVarPCode( bPCode, pField->szName, HB_COMP_PARAM );
}

/* sends a message to an object */
/* bIsObject = TRUE if we are sending a message to real object 
   bIsObject is FALSE if we are sending a message to an object specified
   with WITH OBJECT statement.
*/
void hb_compGenMessage( char * szMsgName, BOOL bIsObject, HB_COMP_DECL )
{
   USHORT wSym;
   PCOMSYMBOL pSym;
   
   if( szMsgName )
   {
      pSym = hb_compSymbolFind( HB_COMP_PARAM, szMsgName, &wSym, HB_SYM_MSGNAME );

      if( ! pSym )  /* the symbol was not found on the symbol table */
         pSym = hb_compSymbolAdd( HB_COMP_PARAM, szMsgName, &wSym, HB_SYM_MSGNAME );
      pSym->cScope |= HB_FS_MESSAGE;
      if( bIsObject )
         hb_compGenPCode3( HB_P_MESSAGE, HB_LOBYTE( wSym ), HB_HIBYTE( wSym ), HB_COMP_PARAM );
      else
         hb_compGenPCode3( HB_P_WITHOBJECTMESSAGE, HB_LOBYTE( wSym ), HB_HIBYTE( wSym ), HB_COMP_PARAM );
   }
   else
   {
      wSym = 0xFFFF;
      hb_compGenPCode3( HB_P_WITHOBJECTMESSAGE, HB_LOBYTE( wSym ), HB_HIBYTE( wSym ), HB_COMP_PARAM );
   }
}

void hb_compGenMessageData( char * szMsg, BOOL bIsObject, HB_COMP_DECL ) /* generates an underscore-symbol name for a data assignment */
{
   char szResult[ HB_SYMBOL_NAME_LEN + 1 ];
   int iLen = strlen( szMsg );

   if( iLen >= HB_SYMBOL_NAME_LEN )
      iLen = HB_SYMBOL_NAME_LEN - 1;
   szResult[ 0 ] = '_';
   memcpy( szResult + 1, szMsg, iLen );
   szResult[ iLen + 1 ] = '\0';

   hb_compGenMessage( hb_compIdentifierNew( HB_COMP_PARAM, szResult, HB_IDENT_COPY ), bIsObject, HB_COMP_PARAM );
}

static void hb_compCheckEarlyMacroEval( HB_COMP_DECL, char *szVarName )
{
   int iScope = hb_compVariableScope( HB_COMP_PARAM, szVarName );

   if( iScope == HB_VS_CBLOCAL_VAR ||
       iScope == HB_VS_STATIC_VAR ||
       iScope == HB_VS_GLOBAL_STATIC ||
       iScope == HB_VS_LOCAL_FIELD ||
       iScope == HB_VS_GLOBAL_FIELD ||
       iScope == HB_VS_LOCAL_MEMVAR ||
       iScope == HB_VS_GLOBAL_MEMVAR )
   {
      hb_compErrorCodeblock( HB_COMP_PARAM, szVarName );
   }
}

/* Check variable in the following order:
 * LOCAL variable
 *    local STATIC variable
 *       local FIELD variable
 *  local MEMVAR variable
 * global STATIC variable
 *    global FIELD variable
 *       global MEMVAR variable
 * (if not found - it is an undeclared variable)
 */
void hb_compGenPopVar( char * szVarName, HB_COMP_DECL ) /* generates the pcode to pop a value from the virtual machine stack onto a variable */
{
   int iVar, iScope;
   PVAR pVar;

   if( ! HB_COMP_PARAM->functions.pLast->bLateEval )
   {
      /* pseudo-generation of pcode for a codeblock with macro symbol */
      hb_compCheckEarlyMacroEval( HB_COMP_PARAM, szVarName );
      return;
   }

   pVar = hb_compVariableFind( HB_COMP_PARAM, szVarName, &iVar, &iScope );
   if( pVar )
   {
      switch( iScope )
      {
         case HB_VS_LOCAL_VAR:
         case HB_VS_CBLOCAL_VAR:
            /* local variable */
            /* local variables used in a coddeblock will not be adjusted
             * if PARAMETERS statement will be used then it is safe to
             * use 2 bytes for LOCALNEAR
             */
            if( HB_LIM_INT8( iVar ) && !HB_COMP_PARAM->functions.pLast->szName &&
                !( HB_COMP_PARAM->functions.pLast->bFlags & FUN_EXTBLOCK ) )
               hb_compGenPCode2( HB_P_POPLOCALNEAR, ( BYTE ) iVar, HB_COMP_PARAM );
            else
               hb_compGenPCode3( HB_P_POPLOCAL, HB_LOBYTE( iVar ), HB_HIBYTE( iVar ), HB_COMP_PARAM );
            break;

         case HB_VS_STATIC_VAR:
         case HB_VS_GLOBAL_STATIC:
            /* Static variable */
            hb_compGenPCode3( HB_P_POPSTATIC, HB_LOBYTE( iVar ), HB_HIBYTE( iVar ), HB_COMP_PARAM );
            {
               PFUNCTION pFunc;
               /* Check if we are generating a pop code for static variable
                * initialization function - if YES then we have to switch to a function
                * where the static variable was declared
                */
               pFunc = HB_COMP_PARAM->functions.pLast;
               if( ( HB_COMP_PARAM->functions.pLast->cScope & HB_FS_INITEXIT ) == HB_FS_INITEXIT )
                  pFunc = pFunc->pOwner;
               pFunc->bFlags |= FUN_USES_STATICS;
            }
            break;

         case HB_VS_LOCAL_FIELD:
         case HB_VS_GLOBAL_FIELD:
            /* declared field */
            hb_compGenFieldPCode( HB_COMP_PARAM, HB_P_POPFIELD, pVar );
            break;

         case HB_VS_LOCAL_MEMVAR:
         case HB_VS_GLOBAL_MEMVAR:
            /* declared memvar variable */
            hb_compGenVarPCode( HB_P_POPMEMVAR, szVarName, HB_COMP_PARAM );
            break;

         default:
            /* undeclared variable */
            hb_compGenVariablePCode( HB_COMP_PARAM, HB_P_POPVARIABLE, szVarName );
            break;
      }
   }
   else
   {  /* undeclared variable */
      hb_compGenVariablePCode( HB_COMP_PARAM, HB_P_POPVARIABLE, szVarName );
   }
}

/* generates the pcode to pop a value from the virtual machine stack onto a memvar variable */
void hb_compGenPopMemvar( char * szVarName, HB_COMP_DECL )
{
   if( ( hb_compVariableScope( HB_COMP_PARAM, szVarName ) & HB_VS_LOCAL_MEMVAR ) == 0 )
      hb_compGenWarning( HB_COMP_PARAM, hb_comp_szWarnings, 'W', HB_COMP_WARN_MEMVAR_ASSUMED, szVarName, NULL );
   hb_compGenVarPCode( HB_P_POPMEMVAR, szVarName, HB_COMP_PARAM );
}

/* generates the pcode to push a nonaliased variable value to the virtual
 * machine stack
 * bMacroVar is TRUE if macro &szVarName context
 */
void hb_compGenPushVar( char * szVarName, BOOL bMacroVar, HB_COMP_DECL )
{
   int iVar, iScope;
   PVAR pVar;

   if( ! HB_COMP_PARAM->functions.pLast->bLateEval && ! bMacroVar )
   {
      /* pseudo-generation of pcode for a codeblock with macro symbol */
      hb_compCheckEarlyMacroEval( HB_COMP_PARAM, szVarName );
      return;
   }

   pVar = hb_compVariableFind( HB_COMP_PARAM, szVarName, &iVar, &iScope );
   if( pVar )
   {
      switch( iScope )
      {
         case HB_VS_LOCAL_VAR:
         case HB_VS_CBLOCAL_VAR:
            /* local variable */
            /* local variables used in a coddeblock will not be adjusted
             * if PARAMETERS statement will be used then it is safe to
             * use 2 bytes for LOCALNEAR
             */
            if( HB_LIM_INT8( iVar ) && !HB_COMP_PARAM->functions.pLast->szName &&
                !( HB_COMP_PARAM->functions.pLast->bFlags & FUN_EXTBLOCK ) )
               hb_compGenPCode2( HB_P_PUSHLOCALNEAR, ( BYTE ) iVar, HB_COMP_PARAM );
            else
               hb_compGenPCode3( HB_P_PUSHLOCAL, HB_LOBYTE( iVar ), HB_HIBYTE( iVar ), HB_COMP_PARAM );
            break;

         case HB_VS_STATIC_VAR:
         case HB_VS_GLOBAL_STATIC:
            /* Static variable */
            hb_compGenPCode3( HB_P_PUSHSTATIC, HB_LOBYTE( iVar ), HB_HIBYTE( iVar ), HB_COMP_PARAM );
            HB_COMP_PARAM->functions.pLast->bFlags |= FUN_USES_STATICS;
            break;

         case HB_VS_LOCAL_FIELD:
         case HB_VS_GLOBAL_FIELD:
            /* declared field */
            hb_compGenFieldPCode( HB_COMP_PARAM, HB_P_PUSHFIELD, pVar );
            break;

         case HB_VS_LOCAL_MEMVAR:
         case HB_VS_GLOBAL_MEMVAR:
            /* declared memvar variable */
            hb_compGenVarPCode( HB_P_PUSHMEMVAR, szVarName, HB_COMP_PARAM );
            break;

         default:
            /* undeclared variable */
            hb_compGenVariablePCode( HB_COMP_PARAM, HB_P_PUSHVARIABLE, szVarName );
            break;
      }
   }
   else
   {  /* undeclared variable */
      hb_compGenVariablePCode( HB_COMP_PARAM, HB_P_PUSHVARIABLE, szVarName );
   }
}

void hb_compGenPushVarRef( char * szVarName, HB_COMP_DECL ) /* generates the pcode to push a variable by reference to the virtual machine stack */
{
   int iVar, iScope;
   PVAR pVar;

   if( ! HB_COMP_PARAM->functions.pLast->bLateEval )
   {
      /* pseudo-generation of pcode for a codeblock with macro symbol */
      hb_compCheckEarlyMacroEval( HB_COMP_PARAM, szVarName );
      return;
   }

   pVar = hb_compVariableFind( HB_COMP_PARAM, szVarName, &iVar, &iScope );
   if( pVar )
   {
      switch( iScope )
      {
         case HB_VS_LOCAL_VAR:
         case HB_VS_CBLOCAL_VAR:
            /* local variable */
            hb_compGenPCode3( HB_P_PUSHLOCALREF, HB_LOBYTE( iVar ), HB_HIBYTE( iVar ), HB_COMP_PARAM );
            break;

         case HB_VS_STATIC_VAR:
         case HB_VS_GLOBAL_STATIC:
            /* Static variable */
            hb_compGenPCode3( HB_P_PUSHSTATICREF, HB_LOBYTE( iVar ), HB_HIBYTE( iVar ), HB_COMP_PARAM );
            HB_COMP_PARAM->functions.pLast->bFlags |= FUN_USES_STATICS;
            break;

         case HB_VS_LOCAL_FIELD:
         case HB_VS_GLOBAL_FIELD:
            /* pushing fields by reference is not allowed */
            hb_compErrorRefer( HB_COMP_PARAM, NULL, szVarName );
            break;

         case HB_VS_LOCAL_MEMVAR:
         case HB_VS_GLOBAL_MEMVAR:
            /* declared memvar variable */
            hb_compGenVarPCode( HB_P_PUSHMEMVARREF, szVarName, HB_COMP_PARAM );
            break;

         default:
            /* undeclared variable */
            /* field cannot be passed by the reference - assume the memvar */
            hb_compGenVariablePCode( HB_COMP_PARAM, HB_P_PUSHMEMVARREF, szVarName );
            break;
      }
   }
   else
   {  /* undeclared variable */
      /* field cannot be passed by the reference - assume the memvar */
      hb_compGenVariablePCode( HB_COMP_PARAM, HB_P_PUSHMEMVARREF, szVarName );
   }
}

void hb_compGenPushMemvarRef( char * szVarName, HB_COMP_DECL ) /* generates the pcode to push memvar variable by reference to the virtual machine stack */
{
   hb_compGenVarPCode( HB_P_PUSHMEMVARREF, szVarName, HB_COMP_PARAM );
}

/* generates the pcode to pop a value from the virtual machine stack onto
 * an aliased variable
 */
void hb_compGenPopAliasedVar( char * szVarName,
                              BOOL bPushAliasValue,
                              char * szAlias,
                              HB_LONG lWorkarea,
                              HB_COMP_DECL )
{
   if( bPushAliasValue )
   {
      if( szAlias )
      {
         int iLen = strlen( szAlias );
         if( szAlias[ 0 ] == 'M' && ( iLen == 1 ||
             ( iLen >= 4 && iLen <= 6 &&
               memcmp( szAlias, "MEMVAR", iLen ) == 0 ) ) )
         {  /* M->variable or MEMV[A[R]]->variable */
            hb_compGenVarPCode( HB_P_POPMEMVAR, szVarName, HB_COMP_PARAM );
         }
         else if( iLen >= 4 && iLen <= 5 &&
                  memcmp( szAlias, "FIELD", iLen ) == 0 )
         {  /* FIEL[D]->variable */
            hb_compGenVarPCode( HB_P_POPFIELD, szVarName, HB_COMP_PARAM );
         }
         else
         {  /* database alias */
            hb_compGenPushSymbol( szAlias, HB_SYM_ALIAS, HB_COMP_PARAM );
            hb_compGenVarPCode( HB_P_POPALIASEDFIELD, szVarName, HB_COMP_PARAM );
         }
      }
      else
      {
         hb_compGenPushLong( lWorkarea, HB_COMP_PARAM );
         hb_compGenVarPCode( HB_P_POPALIASEDFIELD, szVarName, HB_COMP_PARAM );
      }
   }
   else
      /* Alias is already placed on stack
       * NOTE: An alias will be determined at runtime then we cannot decide
       * here if passed name is either a field or a memvar
       */
      hb_compGenVarPCode( HB_P_POPALIASEDVAR, szVarName, HB_COMP_PARAM );
}

/* generates the pcode to push an aliased variable value to the virtual
 * machine stack
 */
void hb_compGenPushAliasedVar( char * szVarName,
                               BOOL bPushAliasValue,
                               char * szAlias,
                               HB_LONG lWorkarea,
                               HB_COMP_DECL )
{
   if( bPushAliasValue )
   {
      if( szAlias )
      {
         int iLen = strlen( szAlias );
         /* myalias->var
          * FIELD->var
          * MEMVAR->var
          */
         if( szAlias[ 0 ] == 'M' && ( iLen == 1 ||
             ( iLen >= 4 && iLen <= 6 &&
               memcmp( szAlias, "MEMVAR", iLen ) == 0 ) ) )
         {  /* M->variable or MEMV[A[R]]->variable */
            hb_compGenVarPCode( HB_P_PUSHMEMVAR, szVarName, HB_COMP_PARAM );
         }
         else if( iLen >= 4 && iLen <= 5 &&
                  memcmp( szAlias, "FIELD", iLen ) == 0 )
         {  /* FIEL[D]->variable */
            hb_compGenVarPCode( HB_P_PUSHFIELD, szVarName, HB_COMP_PARAM );
         }
         else
         {  /* database alias */
            hb_compGenPushSymbol( szAlias, HB_SYM_ALIAS, HB_COMP_PARAM );
            hb_compGenVarPCode( HB_P_PUSHALIASEDFIELD, szVarName, HB_COMP_PARAM );
         }
      }
      else
      {
         hb_compGenPushLong( lWorkarea, HB_COMP_PARAM );
         hb_compGenVarPCode( HB_P_PUSHALIASEDFIELD, szVarName, HB_COMP_PARAM );
      }
   }
   else
      /* Alias is already placed on stack
       * NOTE: An alias will be determined at runtime then we cannot decide
       * here if passed name is either a field or a memvar
       */
      hb_compGenVarPCode( HB_P_PUSHALIASEDVAR, szVarName, HB_COMP_PARAM );
}


void hb_compGenPushLogical( int iTrueFalse, HB_COMP_DECL ) /* pushes a logical value on the virtual machine stack */
{
   hb_compGenPCode1( ( BYTE ) ( iTrueFalse ? HB_P_TRUE : HB_P_FALSE ), HB_COMP_PARAM );
}

void hb_compGenPushNil( HB_COMP_DECL )
{
   hb_compGenPCode1( HB_P_PUSHNIL, HB_COMP_PARAM );
}

/* generates the pcode to push a double number on the virtual machine stack */
void hb_compGenPushDouble( double dNumber, BYTE bWidth, BYTE bDec, HB_COMP_DECL )
{
   BYTE pBuffer[ sizeof( double ) + sizeof( BYTE ) + sizeof( BYTE ) + 1 ];

   pBuffer[ 0 ] = HB_P_PUSHDOUBLE;
   HB_PUT_LE_DOUBLE( &( pBuffer[ 1 ] ), dNumber );

   pBuffer[ 1 + sizeof( double ) ] = bWidth;
   pBuffer[ 1 + sizeof( double ) + sizeof( BYTE ) ] = bDec;

   hb_compGenPCodeN( pBuffer, sizeof( pBuffer ), HB_COMP_PARAM );
}

void hb_compGenPushFunCall( char * szFunName, HB_COMP_DECL )
{
   char * szFunction;
   USHORT wSym;

   /* if abbreviated function name was used - change it for whole name */
   szFunction = hb_compReservedName( szFunName );
   if( szFunction )
      szFunName = szFunction;

   if( hb_compSymbolFind( HB_COMP_PARAM, szFunName, &wSym, HB_SYM_FUNCNAME ) != NULL )
   {
      if( ! hb_compFunCallFind( HB_COMP_PARAM, szFunName ) )
         hb_compFunCallAdd( HB_COMP_PARAM, szFunName );
   }
   else
   {
      hb_compSymbolAdd( HB_COMP_PARAM, szFunName, &wSym, HB_SYM_FUNCNAME );
      hb_compFunCallAdd( HB_COMP_PARAM, szFunName );
   }
   hb_compGenPCode3( HB_P_PUSHFUNCSYM, HB_LOBYTE( wSym ), HB_HIBYTE( wSym ), HB_COMP_PARAM );
}

void hb_compGenPushFunSym( char * szFunName, HB_COMP_DECL )
{
   char * szFunction;

   /* if abbreviated function name was used - change it for whole name */
   szFunction = hb_compReservedName( szFunName );
   hb_compGenPushSymbol( szFunction ? szFunction : szFunName,
                         HB_SYM_FUNCNAME, HB_COMP_PARAM );
}

void hb_compGenPushFunRef( char * szFunName, HB_COMP_DECL )
{
   char * szFunction;

   /* if abbreviated function name was used - change it for whole name */
   szFunction = hb_compReservedName( szFunName );
   hb_compGenPushSymbol( szFunction ? szFunction : szFunName,
                         HB_SYM_FUNCNAME, HB_COMP_PARAM );
}

/* generates the pcode to push a symbol on the virtual machine stack */
void hb_compGenPushSymbol( char * szSymbolName, BOOL bFunction, HB_COMP_DECL )
{
   USHORT wSym;

   if( hb_compSymbolFind( HB_COMP_PARAM, szSymbolName, &wSym, bFunction ) != NULL )  /* the symbol was found on the symbol table */
   {
      if( bFunction && ! hb_compFunCallFind( HB_COMP_PARAM, szSymbolName ) )
         hb_compFunCallAdd( HB_COMP_PARAM, szSymbolName );
   }
   else
   {
      hb_compSymbolAdd( HB_COMP_PARAM, szSymbolName, &wSym, bFunction );
      if( bFunction )
         hb_compFunCallAdd( HB_COMP_PARAM, szSymbolName );
   }

   if( wSym > 255 )
      hb_compGenPCode3( HB_P_PUSHSYM, HB_LOBYTE( wSym ), HB_HIBYTE( wSym ), HB_COMP_PARAM );
   else
      hb_compGenPCode2( HB_P_PUSHSYMNEAR, ( BYTE ) wSym, HB_COMP_PARAM );
}

/* generates the pcode to push a long number on the virtual machine stack */
void hb_compGenPushLong( HB_LONG lNumber, HB_COMP_DECL )
{
   if( HB_COMP_PARAM->fLongOptimize )
   {
      if( lNumber == 0 )
         hb_compGenPCode1( HB_P_ZERO, HB_COMP_PARAM );
      else if( lNumber == 1 )
         hb_compGenPCode1( HB_P_ONE, HB_COMP_PARAM );
      else if( HB_LIM_INT8( lNumber ) )
         hb_compGenPCode2( HB_P_PUSHBYTE, (BYTE) lNumber, HB_COMP_PARAM );
      else if( HB_LIM_INT16( lNumber ) )
         hb_compGenPCode3( HB_P_PUSHINT, HB_LOBYTE( lNumber ), HB_HIBYTE( lNumber ), HB_COMP_PARAM );
      else if( HB_LIM_INT32( lNumber ) )
      {
         BYTE pBuffer[ 5 ];
         pBuffer[ 0 ] = HB_P_PUSHLONG;
         HB_PUT_LE_UINT32( pBuffer + 1, lNumber );
         hb_compGenPCodeN( pBuffer, sizeof( pBuffer ), HB_COMP_PARAM );
      }
      else
      {
         BYTE pBuffer[ 9 ];
         pBuffer[ 0 ] = HB_P_PUSHLONGLONG;
         HB_PUT_LE_UINT64( pBuffer + 1, lNumber );
         hb_compGenPCodeN( pBuffer, sizeof( pBuffer ), HB_COMP_PARAM );
      }
   }
   else
   {
      if( HB_LIM_INT32( lNumber ) )
      {
         BYTE pBuffer[ 5 ];
         pBuffer[ 0 ] = HB_P_PUSHLONG;
         HB_PUT_LE_UINT32( pBuffer + 1, lNumber );
         hb_compGenPCodeN( pBuffer, sizeof( pBuffer ), HB_COMP_PARAM );
      }
      else
      {
         BYTE pBuffer[ 9 ];
         pBuffer[ 0 ] = HB_P_PUSHLONGLONG;
         HB_PUT_LE_UINT64( pBuffer + 1, lNumber );
         hb_compGenPCodeN( pBuffer, sizeof( pBuffer ), HB_COMP_PARAM );
      }
   }
}

void hb_compGenPushDate( HB_LONG lNumber, HB_COMP_DECL )
{
   BYTE pBuffer[ 5 ];

   pBuffer[ 0 ] = HB_P_PUSHDATE;
   HB_PUT_LE_UINT32( pBuffer + 1, lNumber );
   hb_compGenPCodeN( pBuffer, sizeof( pBuffer ), HB_COMP_PARAM );
}

/* generates the pcode to push a string on the virtual machine stack */
void hb_compGenPushString( char * szText, ULONG ulStrLen, HB_COMP_DECL )
{
   if( HB_COMP_PARAM->iHidden )
   {
      --ulStrLen;
      szText = hb_compEncodeString( HB_COMP_PARAM->iHidden, szText, &ulStrLen );
      hb_compGenPCode4( HB_P_PUSHSTRHIDDEN, ( BYTE ) HB_COMP_PARAM->iHidden,
                        HB_LOBYTE( ulStrLen ), HB_HIBYTE( ulStrLen ), HB_COMP_PARAM );
      hb_compGenPCodeN( ( BYTE * ) szText, ulStrLen, HB_COMP_PARAM );
      hb_xfree( szText );
   }
   else
   {
      if( ulStrLen > UINT24_MAX )
         hb_compGenError( HB_COMP_PARAM, hb_comp_szErrors, 'E', HB_COMP_ERR_STRING_TOO_LONG, NULL, NULL );
      else
      {
         if( ulStrLen > USHRT_MAX )
            hb_compGenPCode4( HB_P_PUSHSTRLARGE, HB_LOBYTE( ulStrLen ), HB_HIBYTE( ulStrLen ), HB_ULBYTE( ulStrLen ), HB_COMP_PARAM );
         else if( ulStrLen > UCHAR_MAX )
            hb_compGenPCode3( HB_P_PUSHSTR, HB_LOBYTE( ulStrLen ), HB_HIBYTE( ulStrLen ), HB_COMP_PARAM );
         else
            hb_compGenPCode2( HB_P_PUSHSTRSHORT, ( BYTE ) ulStrLen, HB_COMP_PARAM );
         hb_compGenPCodeN( ( BYTE * ) szText, ulStrLen, HB_COMP_PARAM );
      }
   }
}

void hb_compNOOPfill( PFUNCTION pFunc, ULONG ulFrom, int iCount, BOOL fPop, BOOL fCheck )
{
   ULONG ul;

   while( iCount-- )
   {
      if( fPop )
      {
         pFunc->pCode[ ulFrom ] = HB_P_POP;
         fPop = FALSE;
      }
      else if( fCheck && pFunc->pCode[ ulFrom ] == HB_P_NOOP && pFunc->iNOOPs )
      {
         for( ul = 0; ul < pFunc->iNOOPs; ++ul )
         {
            if( pFunc->pNOOPs[ ul ] == ulFrom )
               break;
         }
         if( ul == pFunc->iNOOPs )
            hb_compNOOPadd( pFunc, ulFrom );
      }
      else
         hb_compNOOPadd( pFunc, ulFrom );
      ++ulFrom;
   }
}

/*
 * Warning - when jump optimization is disabled this function can be used
 * _ONLY_ in very limited situations when there is no jumps over the
 * removed block
 */
static void hb_compRemovePCODE( HB_COMP_DECL, ULONG ulPos, ULONG ulCount,
                                BOOL fCanMove )
{
   PFUNCTION pFunc = HB_COMP_PARAM->functions.pLast;
   ULONG ul;

   if( HB_COMP_ISSUPPORTED( HB_COMPFLAG_OPTJUMP ) || !fCanMove )
   {
      /*
       * We can safely remove the dead code when Jump Optimization
       * is enabled by replacing it with HB_P_NOOP opcodes - which
       * will be later eliminated and jump data updated.
       */
      hb_compNOOPfill( pFunc, ulPos, ulCount, FALSE, TRUE );
   }
   else
   {
      memmove( pFunc->pCode + ulPos, pFunc->pCode + ulPos + ulCount,
               pFunc->lPCodePos - ulPos - ulCount );
      pFunc->lPCodePos -= ulCount;

      for( ul = pFunc->iNOOPs; ul; --ul )
      {
         if( pFunc->pNOOPs[ ul ] >= ulPos )
         {
            if( pFunc->pNOOPs[ ul ] < ulPos + ulCount )
            {
               memmove( &pFunc->pNOOPs[ ul ], &pFunc->pNOOPs[ ul + 1 ],
                        pFunc->iNOOPs - ul );
               pFunc->iNOOPs--;
            }
            else
            {
               pFunc->pNOOPs[ ul ] -= ulCount;
            }
         }
      }
   }
}

BOOL hb_compIsJump( HB_COMP_DECL, PFUNCTION pFunc, ULONG ulPos )
{
   ULONG iJump;
   /*
    * Do not allow any optimization (code striping) when Jump Optimization
    * is disabled and we do not have any information about jump addreses
    */
   if( ! HB_COMP_ISSUPPORTED( HB_COMPFLAG_OPTJUMP ) )
      return TRUE;

   for( iJump = 0; iJump < pFunc->iJumps; iJump++ )
   {
      ULONG ulJumpAddr = pFunc->pJumps[ iJump ];
      switch( pFunc->pCode[ ulJumpAddr ] )
      {
         case HB_P_JUMPNEAR:
         case HB_P_JUMPFALSENEAR:
         case HB_P_JUMPTRUENEAR:
            ulJumpAddr += ( signed char ) pFunc->pCode[ ulJumpAddr + 1 ];
            break;

         case HB_P_JUMP:
         case HB_P_JUMPFALSE:
         case HB_P_JUMPTRUE:
            ulJumpAddr += HB_PCODE_MKSHORT( &pFunc->pCode[ ulJumpAddr + 1 ] );
            break;

         /* Jump can be replaced by series of NOOPs or POP and NOOPs
          * and not stripped yet
          */
         case HB_P_NOOP:
         case HB_P_POP:
            ulJumpAddr = ulPos + 1;
            break;

         default:
            ulJumpAddr += HB_PCODE_MKINT24( &pFunc->pCode[ ulJumpAddr + 1 ] );
            break;
      }
      if( ulJumpAddr == ulPos )
         return TRUE;
   }

   return FALSE;
}

/* Generate the opcode to open BEGIN/END sequence
 * This code is simmilar to JUMP opcode - the offset will be filled with
 * - either the address of HB_P_SEQEND opcode if there is no RECOVER clause
 * - or the address of RECOVER code
 */
ULONG hb_compSequenceBegin( HB_COMP_DECL )
{
   hb_compGenPCode4( HB_P_SEQALWAYS, 0, 0, 0, HB_COMP_PARAM );
   hb_compPrepareOptimize( HB_COMP_PARAM );

   hb_compGenPCode4( HB_P_SEQBEGIN, 0, 0, 0, HB_COMP_PARAM );
   hb_compPrepareOptimize( HB_COMP_PARAM );

   return HB_COMP_PARAM->functions.pLast->lPCodePos - 3;
}

/* Generate the opcode to close BEGIN/END sequence
 * This code is simmilar to JUMP opcode - the offset will be filled with
 * the address of first line after END SEQUENCE
 * This opcode will be executed if recover code was not requested (as the
 * last statement in code beetwen BEGIN ... RECOVER) or if BREAK was requested
 * and there was no matching RECOVER clause.
 */
ULONG hb_compSequenceEnd( HB_COMP_DECL )
{
   hb_compGenPCode4( HB_P_SEQEND, 0, 0, 0, HB_COMP_PARAM );

   hb_compPrepareOptimize( HB_COMP_PARAM );

   return HB_COMP_PARAM->functions.pLast->lPCodePos - 3;
}

ULONG hb_compSequenceAlways( HB_COMP_DECL )
{
   hb_compGenPCode4( HB_P_ALWAYSBEGIN, 0, 0, 0, HB_COMP_PARAM );

   hb_compPrepareOptimize( HB_COMP_PARAM );

   return HB_COMP_PARAM->functions.pLast->lPCodePos - 3;
}

/* Remove unnecessary opcodes in case there were no executable statements
 * beetwen BEGIN and RECOVER sequence
 */
void hb_compSequenceFinish( HB_COMP_DECL, ULONG ulStartPos, ULONG ulEndPos,
                            ULONG ulAlways, BOOL fUsualStmts, BOOL fRecover,
                            BOOL fCanMove )
{
   --ulStartPos;  /* HB_P_SEQBEGIN address */

   if( !fUsualStmts && fCanMove && !HB_COMP_PARAM->fDebugInfo )
   {
      ulStartPos -= 4;
      if( ulAlways )
      {
         /* remove HB_P_ALWAYSEND opcode */
         HB_COMP_PARAM->functions.pLast->lPCodePos--;
         /* remove HB_P_SEQALWAYS ... HB_P_ALWAYSBEGIN opcodes */
         hb_compRemovePCODE( HB_COMP_PARAM, ulStartPos,
                             ulAlways - ulStartPos + 4, fCanMove );
      }
      else
      {
         hb_compRemovePCODE( HB_COMP_PARAM, ulStartPos,
                             HB_COMP_PARAM->functions.pLast->lPCodePos -
                             ulStartPos, fCanMove );
      }
      HB_COMP_PARAM->lastLinePos = ulStartPos - 3;
   }
   else if( !ulAlways )
   {
      /* remove HB_P_SEQALWAYS opcode */
      hb_compRemovePCODE( HB_COMP_PARAM, ulStartPos - 4, 4, fCanMove );
   }
   else
   {
      if( !fRecover )
      {
         /* remove HB_P_SEQBEGIN and HB_P_SEQEND */
         hb_compRemovePCODE( HB_COMP_PARAM, ulEndPos - 1, 4, fCanMove );
         hb_compRemovePCODE( HB_COMP_PARAM, ulStartPos, 4, fCanMove );
         if( ! HB_COMP_ISSUPPORTED( HB_COMPFLAG_OPTJUMP ) )
         {
            /* Fix ALWAYS address in HB_P_SEQALWAYS opcode */
            ulAlways -= 8;
            hb_compGenJumpThere( ulStartPos - 3, ulAlways, HB_COMP_PARAM );
         }
      }
      /* empty always block? */
      if( HB_COMP_PARAM->functions.pLast->lPCodePos - ulAlways == 5 &&
          !HB_COMP_PARAM->fDebugInfo )
      {
         /* remove HB_P_ALWAYSBEGIN and HB_P_ALWAYSEND opcodes */
         hb_compRemovePCODE( HB_COMP_PARAM, ulAlways, 5, TRUE );
         /* remove HB_P_SEQALWAYS opcode */
         hb_compRemovePCODE( HB_COMP_PARAM, ulStartPos - 4, 4, fCanMove );
      }
   }
}

/*
 * Start of definition of static variable
 * We are using here the special function HB_COMP_PARAM->pInitFunc which will store
 * pcode needed to initialize all static variables declared in PRG module.
 * pOwner member will point to a function where the static variable is
 * declared:
 */
void hb_compStaticDefStart( HB_COMP_DECL )
{
   HB_COMP_PARAM->functions.pLast->bFlags |= FUN_USES_STATICS;
   if( ! HB_COMP_PARAM->pInitFunc )
   {
      BYTE pBuffer[ 5 ];

      HB_COMP_PARAM->pInitFunc = hb_compFunctionNew( HB_COMP_PARAM, "(_INITSTATICS)", HB_FS_INITEXIT );
      HB_COMP_PARAM->pInitFunc->pOwner = HB_COMP_PARAM->functions.pLast;
      HB_COMP_PARAM->pInitFunc->bFlags = FUN_USES_STATICS | FUN_PROCEDURE;
      HB_COMP_PARAM->pInitFunc->cScope = HB_FS_INITEXIT | HB_FS_LOCAL;
      HB_COMP_PARAM->functions.pLast = HB_COMP_PARAM->pInitFunc;

      pBuffer[ 0 ] = HB_P_STATICS;
      pBuffer[ 1 ] = 0;
      pBuffer[ 2 ] = 0;
      pBuffer[ 3 ] = 1; /* the number of static variables is unknown now */
      pBuffer[ 4 ] = 0;

      hb_compGenPCodeN( pBuffer, 5, HB_COMP_PARAM );

      hb_compGenPCode3( HB_P_SFRAME, 0, 0, HB_COMP_PARAM );     /* frame for statics variables */
      
      if( HB_COMP_PARAM->fDebugInfo )
      {
         /* uncomment this if you want to always set main module name
            not the one where first static variable was declared */
         /* HB_COMP_PARAM->currModule = HB_COMP_PARAM->szFile; */
         hb_compGenModuleName( HB_COMP_PARAM, HB_COMP_PARAM->pInitFunc->szName );
      }
   }
   else
   {
      HB_COMP_PARAM->pInitFunc->pOwner = HB_COMP_PARAM->functions.pLast;
      HB_COMP_PARAM->functions.pLast = HB_COMP_PARAM->pInitFunc;
   }
}

/*
 * End of definition of static variable
 * Return to previously pcoded function.
 */
void hb_compStaticDefEnd( HB_COMP_DECL, char * szVarName )
{
   HB_COMP_PARAM->functions.pLast = HB_COMP_PARAM->pInitFunc->pOwner;
   HB_COMP_PARAM->pInitFunc->pOwner = NULL;
   ++HB_COMP_PARAM->iStaticCnt;
   if( HB_COMP_PARAM->fDebugInfo )
   {
      BYTE bGlobal = 0;
      int iVar;

      if( ! HB_COMP_PARAM->fStartProc && HB_COMP_PARAM->functions.iCount <= 1 )
      {
         /* Variable declaration is outside of function/procedure body.
          * File-wide static variable
          */
         HB_COMP_PARAM->pInitFunc->pOwner = HB_COMP_PARAM->functions.pLast;
         HB_COMP_PARAM->functions.pLast = HB_COMP_PARAM->pInitFunc;
         bGlobal = 1;
      }

      iVar = HB_COMP_PARAM->iStaticCnt;
      hb_compGenPCode4( HB_P_STATICNAME, bGlobal, HB_LOBYTE( iVar ), HB_HIBYTE( iVar ), HB_COMP_PARAM );
      hb_compGenPCodeN( ( BYTE * ) szVarName, strlen( szVarName ) + 1, HB_COMP_PARAM );
      if( bGlobal )
      {
         HB_COMP_PARAM->functions.pLast = HB_COMP_PARAM->pInitFunc->pOwner;
         HB_COMP_PARAM->pInitFunc->pOwner = NULL;
      }
   }
}

/*
 * Start of stop line number info generation
 */
static void hb_compLineNumberDefStart( HB_COMP_DECL )
{
   if( ! HB_COMP_PARAM->pLineFunc )
   {
      HB_COMP_PARAM->pLineFunc = hb_compFunctionNew( HB_COMP_PARAM, "(_INITLINES)", HB_FS_INITEXIT );
      HB_COMP_PARAM->pLineFunc->pOwner = HB_COMP_PARAM->functions.pLast;
      HB_COMP_PARAM->pLineFunc->bFlags = 0;
      HB_COMP_PARAM->pLineFunc->cScope = HB_FS_INITEXIT | HB_FS_LOCAL;
      HB_COMP_PARAM->functions.pLast = HB_COMP_PARAM->pLineFunc;

      if( HB_COMP_PARAM->fDebugInfo )
      {
         /* set main module name */
         HB_COMP_PARAM->currModule = HB_COMP_PARAM->szFile;
         hb_compGenModuleName( HB_COMP_PARAM, HB_COMP_PARAM->pLineFunc->szName );
      }
   }
   else
   {
      HB_COMP_PARAM->pLineFunc->pOwner = HB_COMP_PARAM->functions.pLast;
      HB_COMP_PARAM->functions.pLast = HB_COMP_PARAM->pLineFunc;
   }
}

/*
 * End of stop line number info generation
 * Return to previously pcoded function.
 */
static void hb_compLineNumberDefEnd( HB_COMP_DECL )
{
   HB_COMP_PARAM->functions.pLast = HB_COMP_PARAM->pLineFunc->pOwner;
   HB_COMP_PARAM->pLineFunc->pOwner = NULL;
}

/*
 * Start a new fake-function that will hold pcodes for a codeblock
*/
void hb_compCodeBlockStart( HB_COMP_DECL, BOOL bLateEval )
{
   PFUNCTION pBlock;

   pBlock               = hb_compFunctionNew( HB_COMP_PARAM, NULL, HB_FS_STATIC | HB_FS_LOCAL );
   pBlock->pOwner       = HB_COMP_PARAM->functions.pLast;
   pBlock->bLateEval    = bLateEval;

   HB_COMP_PARAM->functions.pLast = pBlock;
   HB_COMP_PARAM->lastLinePos = 0;
}

void hb_compCodeBlockEnd( HB_COMP_DECL )
{
   PFUNCTION pCodeblock;   /* pointer to the current codeblock */
   PFUNCTION pFunc;        /* pointer to a function that owns a codeblock */
   char * pFuncName;
   ULONG  ulSize;
   USHORT wLocals = 0;     /* number of referenced local variables */
   USHORT wLocalsCnt, wLocalsLen;
   USHORT wPos;
   int iLocalPos;
   PVAR pVar;

   pCodeblock = HB_COMP_PARAM->functions.pLast;

   /* Check if the extended codeblock has return statement */
   if( ( pCodeblock->bFlags & FUN_EXTBLOCK ) &&
       !( pCodeblock->bFlags & FUN_WITH_RETURN ) )
   {
      if( HB_COMP_PARAM->iWarnings >= 1 )
         hb_compGenWarning( HB_COMP_PARAM, hb_comp_szWarnings, 'W', HB_COMP_WARN_FUN_WITH_NO_RETURN,
                            "{||...}", NULL );
      /* finish the codeblock without popping the return value from HVM stack */
      hb_compGenPCode1( HB_P_ENDPROC, HB_COMP_PARAM );
   }

   hb_compGenPCode1( HB_P_ENDBLOCK, HB_COMP_PARAM ); /* finish the codeblock */

   if( !pCodeblock->bError )
   {
      if( pCodeblock->wParamCount && !( pCodeblock->bFlags & FUN_USES_LOCAL_PARAMS ) )
         /* PARAMETERs were used after LOCALs in extended codeblock
          * fix generated local indexes
          */
         hb_compFixFuncPCode( HB_COMP_PARAM, pCodeblock );
      hb_compOptimizeJumps( HB_COMP_PARAM );
   }

   /* return to pcode buffer of function/codeblock in which the current
    * codeblock was defined
    */
   HB_COMP_PARAM->functions.pLast = pCodeblock->pOwner;

   /* find the function that owns the codeblock */
   pFunc = pCodeblock->pOwner;
   pFuncName = pFunc->szName;
   while( pFunc->pOwner )
   {
      pFunc = pFunc->pOwner;
      if( pFunc->szName && *pFunc->szName )
         pFuncName = pFunc->szName;
   }
   pFunc->bFlags |= ( pCodeblock->bFlags & FUN_USES_STATICS );

   /* generate a proper codeblock frame with a codeblock size and with
    * a number of expected parameters
    */

   /* Count the number of referenced local variables */
   wLocalsLen = 0;
   pVar = pCodeblock->pDetached;
   while( pVar )
   {
      if( HB_COMP_PARAM->fDebugInfo )
         wLocalsLen += 4 + strlen( pVar->szName );
      pVar = pVar->pNext;
      ++wLocals;
   }
   wLocalsCnt = wLocals;
   
   ulSize = pCodeblock->lPCodePos + 2;
   if( HB_COMP_PARAM->fDebugInfo )
   {
      ulSize += 3 + strlen( HB_COMP_PARAM->currModule ) + strlen( pFuncName );
      ulSize += wLocalsLen;
   }

   if( ulSize <= 255 && pCodeblock->wParamCount == 0 && wLocals == 0 )
   {
      /* NOTE: 2 = HB_P_PUSHBLOCK + BYTE( size ) */
      hb_compGenPCode2( HB_P_PUSHBLOCKSHORT, ( BYTE ) ulSize, HB_COMP_PARAM );
   }
   else
   {
      /* NOTE: 8 = HB_P_PUSHBLOCK + USHORT( size ) + USHORT( wParams ) + USHORT( wLocals ) + _ENDBLOCK */
      ulSize += 5 + wLocals * 2;
      if( ulSize <= USHRT_MAX )
         hb_compGenPCode3( HB_P_PUSHBLOCK, HB_LOBYTE( ulSize ), HB_HIBYTE( ulSize ), HB_COMP_PARAM );
      else if( ulSize < UINT24_MAX )
      {
         ++ulSize;
         hb_compGenPCode4( HB_P_PUSHBLOCKLARGE, HB_LOBYTE( ulSize ), HB_HIBYTE( ulSize ), HB_ULBYTE( ulSize ), HB_COMP_PARAM );
      }
      else
         hb_compGenError( HB_COMP_PARAM, hb_comp_szErrors, 'E', HB_COMP_ERR_BLOCK_TOO_BIG, NULL, NULL );

      /* generate the number of local parameters */
      hb_compGenPCode2( HB_LOBYTE( pCodeblock->wParamCount ), HB_HIBYTE( pCodeblock->wParamCount ), HB_COMP_PARAM );
      /* generate the number of referenced local variables */
      hb_compGenPCode2( HB_LOBYTE( wLocals ), HB_HIBYTE( wLocals ), HB_COMP_PARAM );

      /* generate the table of referenced local variables */
      pVar = pCodeblock->pDetached;
      while( wLocals-- )
      {
         wPos = hb_compVariableGetPos( pFunc->pLocals, pVar->szName );
         hb_compGenPCode2( HB_LOBYTE( wPos ), HB_HIBYTE( wPos ), HB_COMP_PARAM );
         pVar = pVar->pNext;
      }
   }

   if( HB_COMP_PARAM->fDebugInfo )
   {
      hb_compGenModuleName( HB_COMP_PARAM, pFuncName );

      /* generate the name of referenced local variables */
      pVar = pCodeblock->pDetached;
      iLocalPos = -1;
      while( wLocalsCnt-- )
      {
         hb_compGenPCode3( HB_P_LOCALNAME, HB_LOBYTE( iLocalPos ), HB_HIBYTE( iLocalPos ), HB_COMP_PARAM );
         hb_compGenPCodeN( ( BYTE * ) pVar->szName, strlen( pVar->szName ) + 1, HB_COMP_PARAM );
         iLocalPos--;
         pVar = pVar->pNext;
      }

   }

   hb_compGenPCodeN( pCodeblock->pCode, pCodeblock->lPCodePos, HB_COMP_PARAM );

   if( HB_COMP_PARAM->iWarnings )
   {
      pVar = pCodeblock->pLocals;
      while( pVar )
      {
         if( HB_COMP_PARAM->iWarnings && pFunc->szName && pVar->szName && ! ( pVar->iUsed & VU_USED ) )
            hb_compGenWarning( HB_COMP_PARAM, hb_comp_szWarnings, 'W', HB_COMP_WARN_BLOCKVAR_NOT_USED, pVar->szName, pFunc->szName );
         pVar = pVar->pNext;
      }
      pVar = pCodeblock->pStatics;
      while( pVar )
      {
         if( HB_COMP_PARAM->iWarnings && pFunc->szName && pVar->szName && ! ( pVar->iUsed & VU_USED ) )
            hb_compGenWarning( HB_COMP_PARAM, hb_comp_szWarnings, 'W', HB_COMP_WARN_VAR_NOT_USED, pVar->szName, "{||...}" );
         pVar = pVar->pNext;
      }
   }

   hb_compFunctionKill( HB_COMP_PARAM, pCodeblock );
}

void hb_compCodeBlockStop( HB_COMP_DECL )
{
   PFUNCTION pCodeblock;   /* pointer to the current codeblock */

   pCodeblock = HB_COMP_PARAM->functions.pLast;

   /* return to pcode buffer of function/codeblock in which the current
    * codeblock was defined
    */
   HB_COMP_PARAM->functions.pLast = pCodeblock->pOwner;
   hb_compGenPCodeN( pCodeblock->pCode, pCodeblock->lPCodePos, HB_COMP_PARAM );

   if( HB_COMP_PARAM->iWarnings )
   {
      PVAR pVar = pCodeblock->pLocals;
      /* find the function that owns the codeblock */
      PFUNCTION pFunc = pCodeblock->pOwner;
      while( pFunc->pOwner )
         pFunc = pFunc->pOwner;
      while( pVar )
      {
         if( pFunc->szName && pVar->szName && ! ( pVar->iUsed & VU_USED ) )
            hb_compGenWarning( HB_COMP_PARAM, hb_comp_szWarnings, 'W', HB_COMP_WARN_BLOCKVAR_NOT_USED, pVar->szName, pFunc->szName );
         pVar = pVar->pNext;
      }
   }

   hb_compFunctionKill( HB_COMP_PARAM, pCodeblock );
}

void hb_compCodeBlockRewind( HB_COMP_DECL )
{
   PFUNCTION pCodeblock;   /* pointer to the current codeblock */

   pCodeblock = HB_COMP_PARAM->functions.pLast;
   pCodeblock->lPCodePos = 0;

   /* Release the NOOP array. */
   if( pCodeblock->pNOOPs )
   {
      hb_xfree( ( void * ) pCodeblock->pNOOPs );
      pCodeblock->pNOOPs = NULL;
      pCodeblock->iNOOPs = 0;
   }
   /* Release the Jumps array. */
   if( pCodeblock->pJumps )
   {
      hb_xfree( ( void * ) pCodeblock->pJumps );
      pCodeblock->pJumps = NULL;
      pCodeblock->iJumps = 0;
   }
}

/* ============================ I18N ============================ */
static PHB_I18NTABLE hb_compI18nCreate( void )
{
   PHB_I18NTABLE  pI18n;

   pI18n = ( PHB_I18NTABLE ) hb_xgrab( sizeof( HB_I18NTABLE ) );
   pI18n->pString = NULL;
   pI18n->uiCount = 0;
   pI18n->uiAllocated = 0;
   return pI18n;
}

static void hb_compI18nFree( PHB_I18NTABLE pI18n )
{
   UINT    ui;

   if( pI18n->pString )
   {
      for( ui = 0; ui < pI18n->uiCount; ui++ )
      {
         if( pI18n->pString[ ui ].szText )
            hb_xfree( pI18n->pString[ ui ].szText );
      
         if( pI18n->pString[ ui ].szContext )
            hb_xfree( pI18n->pString[ ui ].szContext );
      
         if( pI18n->pString[ ui ].pLine )
            hb_xfree( pI18n->pString[ ui ].pLine );
      }
      hb_xfree( pI18n->pString );
   }
   hb_xfree( pI18n );
}

static int hb_compI18nCompare( PHB_I18NSTRING pString, const char* pText, const char* pContext )
{
   int  i;

   i = strcmp( pString->szText, pText );

   if( i == 0 )
   {
      if( pContext )
      {
         if( pString->szContext )
            return strcmp( pString->szContext, pContext );
         else
            return -1;
      }
      else
      {
         if( pString->szContext )
            return 1;
         else 
            return 0;
      }
   }
   return i;
}

void hb_compI18nAdd( HB_COMP_DECL, const char* szText, const char* szContext, UINT uiLine )
{
   PHB_I18NTABLE    pI18n;
   PHB_I18NSTRING   pString;
   UINT             uiLeft, uiRight, uiMiddle;
   int              iCompare;

   pI18n = HB_COMP_PARAM->pI18n;

   if( ! pI18n )
      return;

   if( pI18n->uiCount >= pI18n->uiAllocated )
   {
      if( pI18n->pString )
      {
         pI18n->uiAllocated += 32;
         pI18n->pString = ( PHB_I18NSTRING ) hb_xrealloc( pI18n->pString, sizeof( HB_I18NSTRING ) 
                                                                          * pI18n->uiAllocated );
      }
      else
      {
         pI18n->pString = ( PHB_I18NSTRING ) hb_xgrab( sizeof( HB_I18NSTRING ) * 32 );
         pI18n->uiAllocated = 32;
      }
   }

   uiLeft = 0;
   uiRight = pI18n->uiCount;

   while( uiLeft < uiRight )
   {
      uiMiddle = ( uiLeft + uiRight ) >> 1;

      iCompare = hb_compI18nCompare( & pI18n->pString[ uiMiddle ], szText, szContext );

      if( iCompare == 0 )
      {
         pString = & pI18n->pString[ uiMiddle ];

         if( pString->pLine )
         {
            pString->pLine = ( UINT* ) hb_xrealloc( pString->pLine, ( pString->uiLineCount + 1 ) * sizeof( UINT ) );
            pString->pLine[ pString->uiLineCount ] = uiLine;
            pString->uiLineCount++;
         }
         else
         {
            pString->pLine = ( UINT* ) hb_xgrab( sizeof( UINT ) );
            pString->pLine[ 0 ] = uiLine;
            pString->uiLineCount = 1;
         }
         return;
      }
      else if( iCompare < 0 )
         uiLeft = uiMiddle + 1;
      else
         uiRight = uiMiddle;
   }

   memmove( &pI18n->pString[ uiLeft + 1 ], &pI18n->pString[ uiLeft ],
            ( pI18n->uiCount - uiLeft ) * sizeof( HB_I18NSTRING ) );

   pString = & pI18n->pString[ uiLeft ];
   pString->szText = hb_strdup( szText );
   if( szContext )
      pString->szContext = hb_strdup( szContext );
   else
      pString->szContext = NULL;
   pString->uiLine = uiLine;
   pString->pLine = NULL;
   pString->uiLineCount = 0;

   pI18n->uiCount++;
}


static void hb_compI18nEscapeString( FILE* file, char* szText )
{
   while( * szText )
   {
      if( (unsigned char) * szText < ' ' )
      {
        if( * szText == '\t' )
           fprintf( file, "\\t" );
        else if( * szText == '\n' )
           fprintf( file, "\\n" );
        else if( * szText == '\r' )
           fprintf( file, "\\r" );
        else
           fprintf( file, "\\x%02X", * szText );
      }
      else if( * szText == '"' )
      {
        fprintf( file, "\\\"" );
      }
      else if( * szText == '\\' )
      {
        fprintf( file, "\\\\" );
      }
      else
      {
        fprintf( file, "%c", * szText );
      }

      szText++;
   }
}


static BOOL hb_compI18nSave( HB_COMP_DECL )
{
   PHB_I18NTABLE    pI18n;
   PHB_I18NSTRING   pString;
   HB_FNAME         FileName;
   char             szFileName[ _POSIX_PATH_MAX + 1 ];
   char*            szText;
   UINT             uiIndex, uiLine;
   FILE*            file; 

   pI18n = HB_COMP_PARAM->pI18n;

   FileName.szPath = FileName.szName = FileName.szExtension = FileName.szDrive = NULL;

   if( HB_COMP_PARAM->pOutPath )
   {
      FileName.szDrive = HB_COMP_PARAM->pOutPath->szDrive;
      FileName.szPath = HB_COMP_PARAM->pOutPath->szPath;
   }

   if( HB_COMP_PARAM->pI18nFileName )
   {
      if( HB_COMP_PARAM->pI18nFileName->szName )
         FileName.szName = HB_COMP_PARAM->pI18nFileName->szName;

      if( HB_COMP_PARAM->pI18nFileName->szExtension )
         FileName.szExtension = HB_COMP_PARAM->pI18nFileName->szExtension;

      if( HB_COMP_PARAM->pI18nFileName->szPath )
      {
         FileName.szDrive = HB_COMP_PARAM->pI18nFileName->szDrive;
         FileName.szPath = HB_COMP_PARAM->pI18nFileName->szPath;
      }
   }

   if( ! FileName.szName )
      FileName.szName = HB_COMP_PARAM->pFileName->szName;

   if( ! FileName.szExtension )
      FileName.szExtension = ".pot";

   hb_fsFNameMerge( szFileName, &FileName );

   file = hb_fopen( szFileName, "wb" );

   if( ! file )
   {
      hb_compGenError( HB_COMP_PARAM, hb_comp_szErrors, 'E', HB_COMP_ERR_CREATE_OUTPUT, szFileName, NULL );
      return FALSE;
   }

   szText = hb_verHarbour();
   fprintf( file, "#\n# This file is generated by %s\n#\n\n", szText );
   hb_xfree( szText );

   hb_strncpy( szFileName, HB_COMP_PARAM->pFileName->szName, _POSIX_PATH_MAX );

   for( uiIndex = 0; uiIndex < pI18n->uiCount; uiIndex++ )
   {
      pString = & pI18n->pString[ uiIndex ];

      fprintf( file, "#: %s:%d", szFileName, pString->uiLine );

      for( uiLine = 0; uiLine < pString->uiLineCount; uiLine++ )
         fprintf( file, " %s:%d", szFileName, pString->pLine[ uiLine ] );

      fprintf( file, "\n#, c-format\n" );

      if( pString->szContext )
      {
         fprintf( file, "msgctxt \"" );
         hb_compI18nEscapeString( file, pString->szContext );
         fprintf( file, "\"\n" );
      }

      fprintf( file, "msgid \"" );
      hb_compI18nEscapeString( file, pString->szText );
      fprintf( file, "\"\nmsgstr \"\"\n\n" );
   }

   fclose( file );
   return TRUE;
}

/* ************************************************************************* */

/* initialize support variables */
static void hb_compInitVars( HB_COMP_DECL )
{
   if( HB_COMP_PARAM->iErrorCount != 0 )
      hb_compExprLstDealloc( HB_COMP_PARAM );

   HB_COMP_PARAM->functions.iCount = 0;
   HB_COMP_PARAM->functions.pFirst = NULL;
   HB_COMP_PARAM->functions.pLast  = NULL;
   HB_COMP_PARAM->funcalls.iCount  = 0;
   HB_COMP_PARAM->funcalls.pFirst  = NULL;
   HB_COMP_PARAM->funcalls.pLast   = NULL;
   HB_COMP_PARAM->szAnnounce       = NULL;
   HB_COMP_PARAM->fTextSubst       = ( HB_COMP_PARAM->supported & HB_COMPFLAG_MACROTEXT ) != 0;
   HB_COMP_PARAM->fLongOptimize    = TRUE;

   HB_COMP_PARAM->symbols.iCount   = 0;
   HB_COMP_PARAM->symbols.pFirst   = NULL;
   HB_COMP_PARAM->symbols.pLast    = NULL;
   HB_COMP_PARAM->pInitFunc        = NULL;
   HB_COMP_PARAM->pLineFunc        = NULL;
   HB_COMP_PARAM->fAnyWarning      = FALSE;

   HB_COMP_PARAM->iFunctionCnt     = 0;
   HB_COMP_PARAM->iErrorCount      = 0;
   HB_COMP_PARAM->cVarType         = ' ';
   HB_COMP_PARAM->lastLinePos      = 0;
   HB_COMP_PARAM->iStaticCnt       = 0;
   HB_COMP_PARAM->iVarScope        = VS_LOCAL;

   HB_COMP_PARAM->inlines.iCount   = 0;
   HB_COMP_PARAM->inlines.pFirst   = NULL;
   HB_COMP_PARAM->inlines.pLast    = NULL;
}

static void hb_compGenOutput( HB_COMP_DECL, int iLanguage )
{
   HB_TRACE(HB_TR_DEBUG, ("hb_compGenOutput()"));

   switch( iLanguage )
   {
      case HB_LANG_C:
         hb_compGenCCode( HB_COMP_PARAM, HB_COMP_PARAM->pFileName );
         break;

#ifdef HB_GEN_W32_OBJ
      case HB_LANG_OBJ32:
         hb_compGenObj32( HB_COMP_PARAM, HB_COMP_PARAM->pFileName );
         break;
#endif

      case HB_LANG_PORT_OBJ:
         hb_compGenPortObj( HB_COMP_PARAM, HB_COMP_PARAM->pFileName );
         break;

      case HB_LANG_PORT_OBJ_BUF:
         hb_compGenBufPortObj( HB_COMP_PARAM, &HB_COMP_PARAM->pOutBuf, &HB_COMP_PARAM->ulOutBufSize );
         break;

      case HB_LANG_OBJ_MODULE:
         hb_compGenCObj( HB_COMP_PARAM, HB_COMP_PARAM->pFileName );
         break;
   }
}

static void hb_compPpoFile( HB_COMP_DECL, const char * szPrg, const char * szExt,
                            char * szPpoName )
{
   PHB_FNAME pFilePpo = hb_fsFNameSplit( szPrg );

   pFilePpo->szExtension = ( char * ) szExt;
   if( HB_COMP_PARAM->pPpoPath )
   {
      pFilePpo->szPath = HB_COMP_PARAM->pPpoPath->szPath;
      if( HB_COMP_PARAM->pPpoPath->szName )
      {
         pFilePpo->szName = HB_COMP_PARAM->pPpoPath->szName;
         pFilePpo->szExtension = HB_COMP_PARAM->pPpoPath->szExtension;
      }
   }
   hb_fsFNameMerge( szPpoName, pFilePpo );
   hb_xfree( pFilePpo );
}

static void hb_compOutputFile( HB_COMP_DECL )
{
   HB_TRACE(HB_TR_DEBUG, ("hb_compOutputFile()"));

   HB_COMP_PARAM->pFileName->szPath = NULL;
   HB_COMP_PARAM->pFileName->szExtension = NULL;

   /* we create the output file name */
   if( HB_COMP_PARAM->pOutPath )
   {
      if( HB_COMP_PARAM->pOutPath->szPath )
         HB_COMP_PARAM->pFileName->szPath = HB_COMP_PARAM->pOutPath->szPath;

      if( HB_COMP_PARAM->pOutPath->szName )
      {
         HB_COMP_PARAM->pFileName->szName = HB_COMP_PARAM->pOutPath->szName;
         if( HB_COMP_PARAM->pOutPath->szExtension )
            HB_COMP_PARAM->pFileName->szExtension = HB_COMP_PARAM->pOutPath->szExtension;
      }
   }
}

static void hb_compAddInitFunc( HB_COMP_DECL, PFUNCTION pFunc )
{
   PCOMSYMBOL pSym = hb_compSymbolAdd( HB_COMP_PARAM, pFunc->szName, NULL, HB_SYM_FUNCNAME );

   pSym->cScope |= pFunc->cScope;
   hb_compAddFunc( HB_COMP_PARAM, pFunc );
   hb_compGenPCode1( HB_P_ENDPROC, HB_COMP_PARAM );
}

void hb_compCompileEnd( HB_COMP_DECL )
{
   if( HB_COMP_PARAM->pI18n )
   {
      hb_compI18nFree( HB_COMP_PARAM->pI18n );
      HB_COMP_PARAM->pI18n = NULL;
   }

   if( HB_COMP_PARAM->pI18nFileName )
   {
      hb_xfree( HB_COMP_PARAM->pI18nFileName );
      HB_COMP_PARAM->pI18nFileName = NULL;
   }

   if( HB_COMP_PARAM->pMainFileName )
   {
      if( HB_COMP_PARAM->pFileName != HB_COMP_PARAM->pMainFileName )
         /* currently compiled file was autoopened - close also
          * the main module
          */
         hb_xfree( HB_COMP_PARAM->pMainFileName );
      HB_COMP_PARAM->pMainFileName = NULL;
   }

   if( HB_COMP_PARAM->pFileName )
   {
      hb_xfree( HB_COMP_PARAM->pFileName );
      HB_COMP_PARAM->pFileName = NULL;
   }

   while( HB_COMP_PARAM->functions.pLast &&
          !HB_COMP_PARAM->functions.pLast->szName )
   {
      PFUNCTION pFunc = HB_COMP_PARAM->functions.pLast;
      HB_COMP_PARAM->functions.pLast = pFunc->pOwner;
      hb_compFunctionKill( HB_COMP_PARAM, pFunc );
   }
   HB_COMP_PARAM->functions.pLast = NULL;

   if( HB_COMP_PARAM->functions.pFirst )
   {
      PFUNCTION pFunc = HB_COMP_PARAM->functions.pFirst;

      while( pFunc )
         pFunc = hb_compFunctionKill( HB_COMP_PARAM, pFunc );
      HB_COMP_PARAM->functions.pFirst = NULL;
   }

   while( HB_COMP_PARAM->funcalls.pFirst )
   {
      PFUNCALL pFunc = HB_COMP_PARAM->funcalls.pFirst;

      HB_COMP_PARAM->funcalls.pFirst = pFunc->pNext;
      hb_xfree( ( void * ) pFunc );
   }

   while( HB_COMP_PARAM->externs )
   {
      PEXTERN pExtern = HB_COMP_PARAM->externs;

      HB_COMP_PARAM->externs = HB_COMP_PARAM->externs->pNext;
      hb_xfree( pExtern );
   }

   while( HB_COMP_PARAM->inlines.pFirst )
   {
      PINLINE pInline = HB_COMP_PARAM->inlines.pFirst;

      HB_COMP_PARAM->inlines.pFirst = pInline->pNext;
      if( pInline->pCode )
         hb_xfree( pInline->pCode );
      hb_xfree( ( void * ) pInline );
   }

   while( HB_COMP_PARAM->pReleaseDeclared )
   {
      PCOMDECLARED pDeclared = HB_COMP_PARAM->pReleaseDeclared;
      HB_COMP_PARAM->pReleaseDeclared = pDeclared->pNext;
      if( pDeclared->cParamTypes )
         hb_xfree( ( void * ) pDeclared->cParamTypes );
      if( pDeclared->pParamClasses )
         hb_xfree( ( void * ) pDeclared->pParamClasses );
      hb_xfree( ( void * ) pDeclared );
   }
   HB_COMP_PARAM->pFirstDeclared = HB_COMP_PARAM->pLastDeclared = NULL;

   while( HB_COMP_PARAM->pReleaseClass )
   {
      PCOMCLASS pClass = HB_COMP_PARAM->pReleaseClass;
      HB_COMP_PARAM->pReleaseClass = pClass->pNext;
      while( pClass->pMethod )
      {
         PCOMDECLARED pDeclared = pClass->pMethod;
         pClass->pMethod = pDeclared->pNext;
         if( pDeclared->cParamTypes )
            hb_xfree( ( void * ) pDeclared->cParamTypes );
         if( pDeclared->pParamClasses )
            hb_xfree( ( void * ) pDeclared->pParamClasses );
         hb_xfree( ( void * ) pDeclared );
      }
      hb_xfree( ( void * ) pClass );
   }
   HB_COMP_PARAM->pFirstClass = HB_COMP_PARAM->pLastClass = NULL;

   if( HB_COMP_PARAM->symbols.pFirst )
   {
      PCOMSYMBOL pSym = HB_COMP_PARAM->symbols.pFirst;
      while( pSym )
         pSym = hb_compSymbolKill( pSym );
      HB_COMP_PARAM->symbols.pFirst = NULL;
   }
}

static int hb_compCompile( HB_COMP_DECL, const char * szPrg, int iFileType )
{
   const char * szBuffer = NULL;
   int iStatus = EXIT_SUCCESS;

   HB_TRACE(HB_TR_DEBUG, ("hb_compCompile(%s,%d)", szPrg, iFileType));

   if( iFileType == HB_COMP_MEMBUFFER )
   {
      szBuffer = szPrg;
      szPrg = "{SOURCE}";
   }

   HB_COMP_PARAM->pMainFileName = hb_fsFNameSplit( szPrg );
   HB_COMP_PARAM->pFileName = HB_COMP_PARAM->pMainFileName;

   if( HB_COMP_PARAM->pFileName->szName )
   {
      char szFileName[ _POSIX_PATH_MAX + 1 ];    /* filename to parse */
      char szPpoName[ _POSIX_PATH_MAX + 1 ];
      char buffer[ _POSIX_PATH_MAX * 2 + 80 ];

      /* Add /D command line or envvar defines */
      /* hb_compChkDefines( argc, argv ); */

      /* Initialize support variables */
      hb_compInitVars( HB_COMP_PARAM );

      /* Clear and reinitialize preprocessor state */
      hb_pp_reset( HB_COMP_PARAM->pLex->pPP );

      if( !HB_COMP_PARAM->pFileName->szExtension )
         HB_COMP_PARAM->pFileName->szExtension = ".prg";
      hb_fsFNameMerge( szFileName, HB_COMP_PARAM->pFileName );

      if( iFileType == HB_COMP_MEMBUFFER )
      {
         if( !hb_pp_inBuffer( HB_COMP_PARAM->pLex->pPP, szBuffer, strlen( szBuffer ) ) )
         {
            hb_compOutErr( HB_COMP_PARAM, "Cannot create preprocessor buffer." );
            iStatus = EXIT_FAILURE;
         }
      }
      else if( !hb_pp_inFile( HB_COMP_PARAM->pLex->pPP, szFileName, FALSE, NULL, FALSE ) )
      {
         snprintf( buffer, sizeof( buffer ),
                   "Cannot open input file: %s\n", szFileName );
         hb_compOutErr( HB_COMP_PARAM, buffer );
         iStatus = EXIT_FAILURE;
      }
      else if( iFileType == HB_COMP_SINGLEFILE )
      {
         if( HB_COMP_PARAM->fPPT )
         {
            hb_compPpoFile( HB_COMP_PARAM, szPrg, ".ppt", szPpoName );
            if( !hb_pp_traceFile( HB_COMP_PARAM->pLex->pPP, szPpoName, NULL ) )
               iStatus = EXIT_FAILURE;
         }

         if( HB_COMP_PARAM->fPPO && iStatus == EXIT_SUCCESS )
         {
            hb_compPpoFile( HB_COMP_PARAM, szPrg, ".ppo", szPpoName );
            if( !hb_pp_outFile( HB_COMP_PARAM->pLex->pPP, szPpoName, NULL ) )
               iStatus = EXIT_FAILURE;
         }
      }

      if( iStatus == EXIT_SUCCESS && !HB_COMP_PARAM->fExit )
      {
         BOOL bSkipGen = FALSE ;

         HB_COMP_PARAM->szFile = HB_COMP_PARAM->currModule =
            hb_compIdentifierNew( HB_COMP_PARAM, szFileName, HB_IDENT_COPY );
         HB_COMP_PARAM->currLine = 1;

         if( iFileType == HB_COMP_MEMBUFFER )
         {
            hb_compFunctionAdd( HB_COMP_PARAM, "", 0, FUN_PROCEDURE );
            hb_compparse( HB_COMP_PARAM );
         }
         else if( iFileType == HB_COMP_SINGLEFILE )
         {
            if( ! HB_COMP_PARAM->fQuiet )
            {
               if( HB_COMP_PARAM->fPPO )
                  snprintf( buffer, sizeof( buffer ),
                            "Compiling '%s' and generating preprocessed output to '%s'...\n",
                            szFileName, szPpoName );
               else
                  snprintf( buffer, sizeof( buffer ), "Compiling '%s'...\n", szFileName );
               hb_compOutStd( HB_COMP_PARAM, buffer );
            }

            if( HB_COMP_PARAM->fI18n )
            {
               HB_COMP_PARAM->pI18n = hb_compI18nCreate();
            }

            /* Generate the starting procedure frame */
            if( HB_COMP_PARAM->fStartProc )
            {
               hb_compFunctionAdd( HB_COMP_PARAM, hb_compIdentifierNew( HB_COMP_PARAM, hb_strupr( hb_strdup( HB_COMP_PARAM->pFileName->szName ) ), HB_IDENT_FREE ), HB_FS_PUBLIC, FUN_PROCEDURE );
            }
            else
            {
               /* Don't pass the name of module if the code for starting procedure
               * will be not generated. The name cannot be placed as first symbol
               * because this symbol can be used as function call or memvar's name.
               */
               hb_compFunctionAdd( HB_COMP_PARAM, "", 0, FUN_PROCEDURE );
            }

            if( !HB_COMP_PARAM->fExit )
            {
               hb_compparse( HB_COMP_PARAM );
            }
         }
         else
         {
            snprintf( buffer, sizeof( buffer ), "Reading '%s'...\n", szFileName );
            hb_compOutStd( HB_COMP_PARAM, buffer );
         }

         /* Open refernced modules (using DO ... WITh statement
          * or from @.clp command line option
         */
         while( HB_COMP_PARAM->autoopen && !HB_COMP_PARAM->fExit )
         {
            PAUTOOPEN pAutoOpen = HB_COMP_PARAM->autoopen;

            if( ! hb_compIsFunction( HB_COMP_PARAM, pAutoOpen->szName ) )
               hb_compAutoOpen( HB_COMP_PARAM, pAutoOpen->szName, &bSkipGen, iFileType );

            HB_COMP_PARAM->autoopen = HB_COMP_PARAM->autoopen->pNext;
            hb_xfree( pAutoOpen );
         }

         /* Begin of finalization phase. */

         /* fix all previous function returns offsets */
         hb_compFinalizeFunction( HB_COMP_PARAM );

         hb_compExternGen( HB_COMP_PARAM );       /* generates EXTERN symbols names */

         if( HB_COMP_PARAM->pInitFunc )
         {
            char szNewName[ 25 ];

            /* Fix the number of static variables */
            HB_COMP_PARAM->pInitFunc->pCode[ 3 ] = HB_LOBYTE( HB_COMP_PARAM->iStaticCnt );
            HB_COMP_PARAM->pInitFunc->pCode[ 4 ] = HB_HIBYTE( HB_COMP_PARAM->iStaticCnt );
            HB_COMP_PARAM->pInitFunc->iStaticsBase = HB_COMP_PARAM->iStaticCnt;
            /* Update pseudo function name */
            snprintf( szNewName, sizeof( szNewName ), "(_INITSTATICS%05d)", HB_COMP_PARAM->iStaticCnt );
            HB_COMP_PARAM->pInitFunc->szName = hb_compIdentifierNew( HB_COMP_PARAM, szNewName, HB_IDENT_COPY );

            hb_compAddInitFunc( HB_COMP_PARAM, HB_COMP_PARAM->pInitFunc );
         }

         if( HB_COMP_PARAM->fLineNumbers && HB_COMP_PARAM->fDebugInfo )
         {
            PHB_DEBUGINFO pInfo = hb_compGetDebugInfo( HB_COMP_PARAM ), pNext;
            if( pInfo )
            {
               int iModules = 0;
               hb_compLineNumberDefStart( HB_COMP_PARAM );
               do
               {
                  ULONG ulSkip = pInfo->ulFirstLine >> 3;
                  ULONG ulLen = ( ( pInfo->ulLastLine + 7 ) >> 3 ) - ulSkip;

                  hb_compGenPushString( pInfo->pszModuleName, strlen( pInfo->pszModuleName ) + 1, HB_COMP_PARAM );
                  hb_compGenPushLong( ulSkip << 3, HB_COMP_PARAM );
                  hb_compGenPushString( ( char * ) pInfo->pLineMap + ulSkip, ulLen + 1, HB_COMP_PARAM );
                  hb_compGenPCode3( HB_P_ARRAYGEN, 3, 0, HB_COMP_PARAM );
                  iModules++;

                  pNext = pInfo->pNext;
                  hb_xfree( pInfo->pszModuleName );
                  hb_xfree( pInfo->pLineMap );
                  hb_xfree( pInfo );
                  pInfo = pNext;
               }
               while( pInfo );

               hb_compGenPCode3( HB_P_ARRAYGEN, HB_LOBYTE( iModules ), HB_HIBYTE( iModules ), HB_COMP_PARAM );
               hb_compGenPCode1( HB_P_RETVALUE, HB_COMP_PARAM );
               hb_compLineNumberDefEnd( HB_COMP_PARAM );
               hb_compAddInitFunc( HB_COMP_PARAM, HB_COMP_PARAM->pLineFunc );
            }
         }

         if( HB_COMP_PARAM->szAnnounce )
            hb_compAnnounce( HB_COMP_PARAM, HB_COMP_PARAM->szAnnounce );

         /* End of finalization phase. */

         if( HB_COMP_PARAM->iErrorCount || HB_COMP_PARAM->fAnyWarning )
         {
            if( HB_COMP_PARAM->iErrorCount )
            {
               iStatus = EXIT_FAILURE;
               bSkipGen = TRUE;
               snprintf( buffer, sizeof( buffer ),
                         "\r%i error%s\n\nNo code generated\n",
                         HB_COMP_PARAM->iErrorCount,
                         HB_COMP_PARAM->iErrorCount > 1 ? "s" : "" );
               hb_compOutStd( HB_COMP_PARAM, buffer );
            }
            else if( HB_COMP_PARAM->iExitLevel == HB_EXITLEVEL_SETEXIT )
            {
               iStatus = EXIT_FAILURE;
            }
            else if( HB_COMP_PARAM->iExitLevel == HB_EXITLEVEL_DELTARGET )
            {
               iStatus = EXIT_FAILURE;
               bSkipGen = TRUE;
               hb_compOutStd( HB_COMP_PARAM, "\nNo code generated.\n" );
            }
         }

         if( ! HB_COMP_PARAM->fSyntaxCheckOnly && ! bSkipGen &&
             HB_COMP_PARAM->iErrorCount == 0 )
         {
            char * szFirstFunction = NULL;
            PFUNCTION *pFunPtr;

            /* we create the output file name */
            hb_compOutputFile( HB_COMP_PARAM );

            pFunPtr = &HB_COMP_PARAM->functions.pFirst;
            if( ! HB_COMP_PARAM->fStartProc )
            {
               /* skip first non-startup procedure */
               hb_compOptimizeFrames( HB_COMP_PARAM, *pFunPtr );
               pFunPtr = &(*pFunPtr)->pNext;
               HB_COMP_PARAM->iFunctionCnt--;
            }

            while( *pFunPtr && !HB_COMP_PARAM->fExit )
            {
               /* remove function frames with no names */
               if( ! HB_COMP_PARAM->fStartProc && ! (*pFunPtr)->szName[0] )
               {
                  *pFunPtr = hb_compFunctionKill( HB_COMP_PARAM, *pFunPtr );
                  HB_COMP_PARAM->functions.iCount--;
                  HB_COMP_PARAM->iFunctionCnt--;
               }
               else
               {
                  hb_compOptimizeFrames( HB_COMP_PARAM, *pFunPtr );

                  if( szFirstFunction == NULL && 
                     ! ( ( *pFunPtr )->cScope & (HB_FS_INIT | HB_FS_EXIT) ) )
                  {
                     szFirstFunction = ( *pFunPtr )->szName;
                  }
                  pFunPtr = &(*pFunPtr)->pNext;
               }
            }

            if( szFirstFunction )
            {
               PCOMSYMBOL pSym = hb_compSymbolFind( HB_COMP_PARAM, szFirstFunction,
                                                    NULL, HB_SYM_FUNCNAME );
               if( pSym )
                  pSym->cScope |= HB_FS_FIRST;
            }

            if( ! HB_COMP_PARAM->fQuiet )
            {
               snprintf( buffer, sizeof( buffer ),
                         "\rLines %i, Functions/Procedures %i\n",
                         hb_pp_lineTot( HB_COMP_PARAM->pLex->pPP ),
                         HB_COMP_PARAM->iFunctionCnt );
               hb_compOutStd( HB_COMP_PARAM, buffer );
            }

            hb_compGenOutput( HB_COMP_PARAM, HB_COMP_PARAM->iLanguage );
            if( HB_COMP_PARAM->pI18n )
            {
               hb_compI18nSave( HB_COMP_PARAM );
            }
         }
      }
   }
   else
   {
      hb_compGenError( HB_COMP_PARAM, hb_comp_szErrors, 'F', HB_COMP_ERR_BADFILENAME, szPrg, NULL );
      iStatus = EXIT_FAILURE;
   }
   hb_compCompileEnd( HB_COMP_PARAM );

   return HB_COMP_PARAM->fExit ? EXIT_FAILURE : iStatus;
}

static BOOL hb_compAutoOpenFind( HB_COMP_DECL, const char * szName )
{
   PAUTOOPEN pLast = HB_COMP_PARAM->autoopen;

   HB_TRACE(HB_TR_DEBUG, ("hb_compAutoOpenFind(%s)", szName));

   if( pLast == NULL )
      return FALSE;

   if( hb_stricmp( pLast->szName, szName ) == 0 )
      return TRUE;
   else
   {
      while( pLast->pNext )
      {
         pLast = pLast->pNext;

         if( strcmp( pLast->szName, szName ) == 0 )
            return TRUE;
      }
   }
   return FALSE;
}

void hb_compAutoOpenAdd( HB_COMP_DECL, const char * szName )
{
   HB_TRACE(HB_TR_DEBUG, ("hb_compAutoOpenAdd(%p,%s)", HB_COMP_PARAM, szName));

   if( HB_COMP_PARAM->fAutoOpen && ! hb_compAutoOpenFind( HB_COMP_PARAM, szName ) )
   {
      PAUTOOPEN pAutoOpen = ( PAUTOOPEN ) hb_xgrab( sizeof( AUTOOPEN ) ), pLast;

      pAutoOpen->szName = ( char * ) szName;
      pAutoOpen->pNext  = NULL;

      if( HB_COMP_PARAM->autoopen == NULL )
         HB_COMP_PARAM->autoopen = pAutoOpen;
      else
      {
         pLast = HB_COMP_PARAM->autoopen;
         while( pLast->pNext )
            pLast = pLast->pNext;

         pLast->pNext = pAutoOpen;
      }
   }
}

static int hb_compAutoOpen( HB_COMP_DECL, const char * szPrg, BOOL * pbSkipGen, int iFileType )
{
   int iStatus = EXIT_SUCCESS;
   PHB_FNAME pMainFileName = HB_COMP_PARAM->pFileName;

   HB_COMP_PARAM->pFileName = hb_fsFNameSplit( szPrg );

   if( HB_COMP_PARAM->pFileName->szName )
   {
      char szFileName[ _POSIX_PATH_MAX + 1 ];    /* filename to parse */
      char szPpoName[ _POSIX_PATH_MAX + 1 ];
      char buffer[ _POSIX_PATH_MAX * 2 + 80 ];

      /* Clear and reinitialize preprocessor state */
      hb_pp_reset( HB_COMP_PARAM->pLex->pPP );

      if( !HB_COMP_PARAM->pFileName->szExtension )
         HB_COMP_PARAM->pFileName->szExtension = ".prg";

      hb_fsFNameMerge( szFileName, HB_COMP_PARAM->pFileName );

      if( !hb_pp_inFile( HB_COMP_PARAM->pLex->pPP, szFileName, FALSE, NULL, FALSE ) )
      {
         snprintf( buffer, sizeof( buffer ),
                   "Cannot open %s, assumed external\n", szFileName );
         hb_compOutErr( HB_COMP_PARAM, buffer );
         iStatus = EXIT_FAILURE;
      }
      else if( HB_COMP_PARAM->fPPT )
      {
         HB_COMP_PARAM->pFileName->szExtension = ".ppt";
         hb_fsFNameMerge( szPpoName, HB_COMP_PARAM->pFileName );
         if( !hb_pp_traceFile( HB_COMP_PARAM->pLex->pPP, szPpoName, NULL ) )
            iStatus = EXIT_FAILURE;
      }
      if( HB_COMP_PARAM->fPPO && iStatus == EXIT_SUCCESS )
      {
         HB_COMP_PARAM->pFileName->szExtension = ".ppo";
         hb_fsFNameMerge( szPpoName, HB_COMP_PARAM->pFileName );
         if( !hb_pp_outFile( HB_COMP_PARAM->pLex->pPP, szPpoName, NULL ) )
            iStatus = EXIT_FAILURE;
      }

      if( iStatus == EXIT_SUCCESS && !HB_COMP_PARAM->fExit )
      {
         HB_COMP_PARAM->currModule =
            hb_compIdentifierNew( HB_COMP_PARAM, szFileName, HB_IDENT_COPY );
         HB_COMP_PARAM->currLine = 1;

         if( ! HB_COMP_PARAM->fQuiet )
         {
            if( HB_COMP_PARAM->fPPO )
               snprintf( buffer, sizeof( buffer ),
                         "Compiling module '%s' and generating preprocessed output to '%s'...\n",
                         szFileName, szPpoName );
            else
               snprintf( buffer, sizeof( buffer ),
                         "Compiling module '%s'...\n", szFileName );
            hb_compOutStd( HB_COMP_PARAM, buffer );
         }

         /* Generate the starting procedure frame */
         if( HB_COMP_PARAM->fStartProc )
            hb_compFunctionAdd( HB_COMP_PARAM, hb_compIdentifierNew( HB_COMP_PARAM, hb_strupr( hb_strdup( HB_COMP_PARAM->pFileName->szName ) ), HB_IDENT_FREE ), HB_FS_PUBLIC, FUN_PROCEDURE );
         else if( iFileType != HB_COMP_SINGLEFILE )
            hb_compFunctionAdd( HB_COMP_PARAM, "", 0, FUN_PROCEDURE );

         if( !HB_COMP_PARAM->fExit )
         {
            int i = HB_COMP_PARAM->iExitLevel ;
            BOOL b = HB_COMP_PARAM->fAnyWarning;

            hb_compparse( HB_COMP_PARAM );

            HB_COMP_PARAM->iExitLevel = ( i > HB_COMP_PARAM->iExitLevel ? i : HB_COMP_PARAM->iExitLevel );
            HB_COMP_PARAM->fAnyWarning = ( b ? b : HB_COMP_PARAM->fAnyWarning );
         }

         if( HB_COMP_PARAM->fAnyWarning )
         {
            if( HB_COMP_PARAM->iExitLevel == HB_EXITLEVEL_SETEXIT )
            {
               iStatus = EXIT_FAILURE;
            }
            else if( HB_COMP_PARAM->iExitLevel == HB_EXITLEVEL_DELTARGET )
            {
               iStatus = EXIT_FAILURE;
               *pbSkipGen = TRUE;
               hb_compOutStd( HB_COMP_PARAM, "\nNo code generated.\n" );
            }
         }
      }
   }
   else
   {
      hb_compGenError( HB_COMP_PARAM, hb_comp_szErrors, 'F', HB_COMP_ERR_BADFILENAME, szPrg, NULL );
      iStatus = EXIT_FAILURE;
   }

   hb_xfree( HB_COMP_PARAM->pFileName );
   HB_COMP_PARAM->pFileName = pMainFileName;

   return HB_COMP_PARAM->fExit ? EXIT_FAILURE : iStatus;
}
