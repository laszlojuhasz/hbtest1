/*
 * $Id$
 */

/*
 * Harbour Project source code:
 * Header file for the Preprocesor
 *
 * Copyright 1999 Alexander S.Kresin <alex@belacy.belgorod.su>
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

/* Definitions related to the preprocessor */

#ifndef HB_PP_H_
#define HB_PP_H_

#include "hbdefs.h"
#include "hbfsapi.h"

/* the list of pathnames to search with #include */
typedef struct _PATHNAMES 
{
  char * szPath;
  struct _PATHNAMES *pNext;
} PATHNAMES;

struct _DEFINES;
typedef struct _DEFINES
{
  char * name;
  char * pars;
  int    npars;
  char * value;
  struct _DEFINES * last;
} DEFINES;

struct _COMMANDS;
typedef struct _COMMANDS
{
  int com_or_xcom;
  char * name;
  char * mpatt;
  char * value;
  struct _COMMANDS * last;
} COMMANDS;

#define HB_PP_STR_SIZE  8192
#define HB_PP_BUFF_SIZE 2048

#define HB_SKIPTABSPACES( sptr ) while( *sptr == ' ' || *sptr == '\t' ) ( sptr )++

/* HBPP.C exported functions */

extern int    hb_pp_Parse( FILE *, FILE *, char * );
extern int    hb_pp_ParseDirective( char * ); /* Parsing preprocessor directives ( #... ) */
extern int    hb_pp_ParseExpression( char *, char * ); /* Parsing a line ( without preprocessor directive ) */
extern int    hb_pp_WrStr( FILE *, char * );
extern int    hb_pp_RdStr( FILE *, char *, int, BOOL, char *, int *, int * );
extern void   hb_pp_Stuff( char *, char *, int, int, int );
extern int    hb_pp_strocpy( char *, char * );
extern char * hb_pp_strodup( char * );
extern DEFINES * hb_pp_AddDefine( char *, char * );         /* Add new #define to a linked list */

/* HBPPINT.C exported functions */

extern void   hb_pp_Init( void );
extern int    hb_pp_Internal( FILE *, FILE *, char * );

/* HBPP.C exported variables */

extern int    hb_pp_lInclude;
extern int *  hb_pp_aCondCompile;
extern int    hb_pp_nCondCompile;
extern char * hb_pp_szErrors[];
extern char * hb_pp_szWarnings[];

/* TABLE.C exported variables */

extern DEFINES *  hb_pp_topDefine;
extern COMMANDS * hb_pp_topCommand;
extern COMMANDS * hb_pp_topTranslate;

/* Needed support modules, but not contained in HBPP.C */

extern void *   hb_xgrab( ULONG lSize );   /* allocates memory, exists on failure */
extern void *   hb_xrealloc( void * pMem, ULONG lSize );   /* reallocates memory */
extern void     hb_xfree( void * pMem );    /* frees memory */

extern ULONG    hb_strAt( const char * szSub, ULONG ulSubLen, const char * szText, ULONG ulLen );
extern void     hb_strupr( char * szText );

#include "compiler.h"

#endif /* HB_PP_H_ */
