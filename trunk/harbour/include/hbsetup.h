/*
 * $Id$
 */

/*
 * Harbour Project source code:
 * Header file for compiler and runtime configuration
 *
 * Copyright 1999 Ryszard Glab <rglab@imid.med.pl>
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

#ifndef HB_SETUP_H_
#define HB_SETUP_H_

#include <limits.h>

/* Include settings common for .PRG and .C files */
#include "hbsetup.ch"

/* ***********************************************************************
 * This symbol defines if Harbour is compiled using C compiler
 * that support strict ANSI C only
 *
 * The only non ANSI C feature that we are using is an ability
 * to call functions before the 'main' module is called.
 * This trick is used to automatically join all symbol tables defined
 * in run-time support modules and in user defined modules.
 *   If strict ANSI C compability is required then all symbol tables
 * have to be joined manually by calling special function named
 * hb_vm_SymbolInit_<module_name>
 * (for example for myfirst.prg it will be: 'hb_vm_SymbolInit_MYFIRST'
 * The generation of this function is performed by the macro called
 * HB_CALL_ON_STARTUP that is defined in 'hbinit.h'
 *
 * By default we are using extensions to ANSI C (symbol is not defined)
*/
/*#define HARBOUR_STRICT_ANSI_C */

/* ***********************************************************************
 * Define this option if you want the /y YACC trace option to be available
 * in the Harbour compiler.
 *
 * Note that if you turn this on, the compiler will slighly grow in size.
 *
 * By default this is turned on.
 * TODO: This should be disabled, when the parser has matured.
*/
/*#define HARBOUR_YYDEBUG*/

/* ***********************************************************************
 * This symbol defines if we are trying to compile using GCC for OS/2
 *
 * By default it is disabled (symbol is not defined)
*/
/*#define HARBOUR_GCC_OS2*/

/* ***********************************************************************
 * The name of starting procedure
 * Note: You have to define it in case when Harbour cannot find the proper
 * starting procedure (due to incorrect order of static data initialization)
 *
 * The list of compilers that require it:
 * - Watcom C/C++ 10.0
 * - GCC on Linux
 *
 * By default we are using automatic lookup (symbol not defined)
*/
#if defined(__WATCOMC__) || defined(__GNUC__)
   #if !defined(__DJGPP__) && !defined(HARBOUR_GCC_OS2)
      #define HARBOUR_START_PROCEDURE "MAIN"
   #endif
#endif

/* ***********************************************************************
 * This symbol defines which national language module should be included
 * in the Harbour run time library. See source/rtl/msgxxx for all allowed
 * values for <nl> in this manifest constant.
 *
 * By default it is disabled (symbol is not defined), which selects UK
*/
/*#define HARBOUR_LANGUAGE_<nl>*/

/* ***********************************************************************
 * This symbol defines if we want an ability to create and link OBJ files
 * generated by Harbour compiler
 *
 * Note that the Virtual Machine support need a platform/compiler specific
 * assembler module, so you will be able to use this only with 32 bits
 * Borland C/C++ compilers.
 *
 * By default it is disabled (symbol is not defined)
*/
/*#define HARBOUR_OBJ_GENERATION*/

/* ***********************************************************************
 * You can select here, what type of main entry will be used in the
 * application (main() or WinMain()).
 *
 * By default the standard C main() function will be used.
*/
/*#define HARBOUR_MAIN_STD*/
/*#define HARBOUR_MAIN_WIN*/

/* ***********************************************************************
 * You can select here, what type of main entry will be used in the
 * application (main() or WinMain()).
 *
 * By default the standard C main() function will be used.
*/
/*#define HARBOUR_MAIN_STD*/
/*#define HARBOUR_MAIN_WIN*/

/* ***********************************************************************
 * You can set here the maximum symbol name length handled by Harbour
 * compiler and runtime. You can override this setting in the make process.
 *
 * By default this value is 63
*/

#ifndef HB_SYMBOL_NAME_LEN
/* NOTE: For complete CA-Cl*pper compatibility you can set the maximum
         symbol name to 10. Sometimes this can be useful for compiling legacy 
         code. [vszakats] */
/*
   #ifdef HARBOUR_STRICT_CLIPPER_COMPATIBILITY
      #define HB_SYMBOL_NAME_LEN   10
   #else
*/
      #define HB_SYMBOL_NAME_LEN   63
/*
   #endif
*/
#endif

/* ***********************************************************************
 * Operating system specific definitions
 */
#if defined(__GNUC__)
   /* The GNU C compiler is used */
   #if defined(__DJGPP__) || defined(HARBOUR_GCC_OS2) || defined(_Windows) || defined(_WIN32)
      /* The DJGPP port of GNU C is used - for DOS platform */
      #define OS_DOS_COMPATIBLE
      #define OS_PATH_LIST_SEPARATOR    ';'
      #define OS_PATH_DELIMITER         '\\'
      #define OS_PATH_DELIMITER_LIST    "\\/:"
      #define OS_OPT_DELIMITER_LIST     "/-"
   #else
      #define OS_UNIX_COMPATIBLE
      #define OS_PATH_LIST_SEPARATOR    ':'
      #define OS_PATH_DELIMITER         '/'
      #define OS_PATH_DELIMITER_LIST    "/"
      #define OS_OPT_DELIMITER_LIST     "-"
   #endif
#else
   /* we are assuming here the DOS compatible OS */
   #define OS_DOS_COMPATIBLE
   #define OS_PATH_LIST_SEPARATOR    ';'
   #define OS_PATH_DELIMITER         '\\'
   #define OS_PATH_DELIMITER_LIST    "\\/:"
   #define OS_OPT_DELIMITER_LIST     "/-"
#endif

#ifndef _POSIX_PATH_MAX
   #define _POSIX_PATH_MAX    255
#endif

#define HB_ISOPTSEP( c ) ( strchr( OS_OPT_DELIMITER_LIST, ( c ) ) != NULL )

/* ***********************************************************************
 * Platform detection
 */

#ifndef HB_OS_DOS
   #if defined(DOS) || defined(_QC) || defined(__DOS__) || defined(MSDOS) || defined(__MSDOS__)
      #define HB_OS_DOS
      #if defined(__386__)
         #define HB_OS_DOS_32
      #else
         #define HB_OS_DOS_16
      #endif
   #endif
#endif

#ifndef HB_OS_OS2
   #if defined(OS2) || defined(__OS2__) || defined(OS_2) || defined(HARBOUR_GCC_OS2)
      #define HB_OS_OS2
      #if defined(__EMX__)
         #define HB_OS_OS2_EMX
      #endif
   #endif
#endif

#ifndef HB_OS_WIN_32
   #if defined(WINNT) || defined(_Windows) || defined(__NT__) || defined(_WIN32) || defined(_WINDOWS_) || defined(__WINDOWS_386__)
      #define HB_OS_WIN_32
   #endif
#endif

#ifndef HB_OS_UNIX
   #if defined(__GNUC__) && !(defined(__DJGPP__) || defined(HARBOUR_GCC_OS2) || defined(__CYGNUS__))
      #define HB_OS_UNIX
   #endif
#endif

#ifndef HB_OS_MAC
   #if defined(__MPW__)
      #define HB_OS_MAC
   #endif
#endif

/* ***********************************************************************
 * Extern "C" detection
 */

#if defined(__cplusplus) && !defined(__IBMCPP__)
   #define HB_EXTERN_C
#endif

#endif /* HB_SETUP_H_ */
