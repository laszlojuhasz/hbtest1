/*
 * $Id$
 */

/*
   Harbour Project source code

   This file provides the bare minimum of declarations
   which is required to compile a Harbour generated .C file.

   Copyright (C) 1999  Victor Szel <info@szelvesz.hu>
   www - http://www.harbour-project.org

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version, with one exception:

   The exception is that if you link the Harbour Runtime Library (HRL)
   and/or the Harbour Virtual Machine (HVM) with other files to produce
   an executable, this does not by itself cause the resulting executable
   to be covered by the GNU General Public License. Your use of that
   executable is in no way restricted on account of linking the HRL
   and/or HVM code into it.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA (or visit
   their web site at http://www.gnu.org/).
*/

#ifndef HB_VMPUB_H_
#define HB_VMPUB_H_

#include "pcode.h"

/* Dummy definitions */

typedef void * PHB_DYNS;

/* Parts copied from hbdefs.h */

typedef unsigned char BYTE;   /* 1 byte unsigned */
typedef unsigned short int WORD;

#ifdef __GNUC__
   #define pascal __attribute__ ((stdcall))
#endif

#ifdef _MSC_VER
   #define HARBOUR void
#else
   #ifdef __IBMCPP__
      #define HARBOUR void
   #else
      #define HARBOUR void pascal
   #endif
#endif
typedef void * PHB_FUNC;

typedef char SYMBOLSCOPE;   /* stores symbol's scope */

/* Parts copied from extend.h */

/* symbol support structure */
typedef struct
{
   char *      szName;  /* the name of the symbol */
   SYMBOLSCOPE cScope;  /* the scope of the symbol */
   PHB_FUNC    pFunPtr; /* function address for function symbol table entries */
   PHB_DYNS    pDynSym; /* pointer to its dynamic symbol if defined */
} HB_SYMB, * PHB_SYMB;

extern void hb_vmExecute( BYTE * pCode, PHB_SYMB pSymbols );  /* invokes the virtual machine */

/* Harbour Functions scope (SYMBOLSCOPE) */
#define FS_PUBLIC       0x00
#define FS_STATIC       0x02
#define FS_INIT         0x08
#define FS_EXIT         0x10
#define FS_INITEXIT     ( FS_INIT | FS_EXIT )
#define FS_MESSAGE      0x20
#define FS_MEMVAR       0x80

/* This should always follow the type declarations */

#include "init.h"

#endif /* HB_VMPUB_H_ */
