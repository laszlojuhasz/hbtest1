/*
 * $Id$
 */

/*
 * Harbour Project source code:
 * Language Support Module (SLWIN)
 *
 * Copyright 2003 Mitja Podgornik <Mitja.Podgornik@zgs.gov.si>
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

/* Language name: Slovenian */
/* ISO language code (2 chars): SL */
/* Codepage: 1250 */

#include "hbapilng.h"

static HB_LANG s_lang =
{
   {
      /* Identification */

      "SLWIN",                    /* ID */
      "Slovenian",                /* Name (in English) */
      "Sloven��ina",              /* Name (in native language) */
      "SL",                       /* RFC ID */
      "1250",                     /* Codepage */
      "$Revision$ $Date$",     /* Version */

      /* Month names */

      "Januar",
      "Februar",
      "Marec",
      "April",
      "Maj",
      "Junij",
      "Julij",
      "Avgust",
      "September",
      "Oktober",
      "November",
      "December",

      /* Day names */

      "Nedelja",
      "Ponedeljek",
      "Torek",
      "Sreda",
      "�etrtek",
      "Petek",
      "Sobota",

      /* CA-Cl*pper compatible natmsg items */

      "Database Files    # Records    Last Update     Size",
      "Do you want more samples?",
      "Page No.",
      "** Subtotal **",
      "* Subsubtotal *",
      "*** Total ***",
      "Ins",
      "   ",
      "Invalid date",
      "Range: ",
      " - ",
      "D/N",
      "INVALID EXPRESSION",

      /* Error description names */

      "Unknown error",
      "Argument error",
      "Bound error",
      "String overflow",
      "Numeric overflow",
      "Zero divisor",
      "Numeric error",
      "Syntax error",
      "Operation too complex",
      "",
      "",
      "Memory low",
      "Undefined function",
      "No exported method",
      "Variable does not exist",
      "Alias does not exist",
      "No exported variable",
      "Illegal characters in alias",
      "Alias already in use",
      "",
      "Create error",
      "Open error",
      "Close error",
      "Read error",
      "Write error",
      "Print error",
      "",
      "",
      "",
      "",
      "Operation not supported",
      "Limit exceeded",
      "Corruption detected",
      "Data type error",
      "Data width error",
      "Workarea not in use",
      "Workarea not indexed",
      "Exclusive required",
      "Lock required",
      "Write not allowed",
      "Append lock failed",
      "Lock Failure",
      "",
      "",
      "",
      "",
      "array access",
      "array assign",
      "array dimension",
      "not an array",
      "conditional",

      /* Internal error names */

      "Unrecoverable error %lu: ",
      "Error recovery failure",
      "No ERRORBLOCK() for error",
      "Too many recursive error handler calls",
      "RDD invalid or failed to load",
      "Invalid method type from %s",
      "hb_xgrab can't allocate memory",
      "hb_xrealloc called with a NULL pointer",
      "hb_xrealloc called with an invalid pointer",
      "hb_xrealloc can't reallocate memory",
      "hb_xfree called with an invalid pointer",
      "hb_xfree called with a NULL pointer",
      "Can\'t locate the starting procedure: \'%s\'",
      "No starting procedure",
      "Unsupported VM opcode",
      "Symbol item expected from %s",
      "Invalid symbol type for self from %s",
      "Codeblock expected from %s",
      "Incorrect item type on the stack trying to pop from %s",
      "Stack underflow",
      "An item was going to be copied to itself from %s",
      "Invalid symbol item passed as memvar %s",
      "Memory buffer overflow",
      "hb_xgrab requested to allocate zero bytes",
      "hb_xrealloc requested to resize to zero bytes",
      "hb_xalloc requested to allocate zero bytes",

      /* Texts */

      "DD-MM-LLLL",
      "D",
      "N"
   }
};

HB_LANG_ANNOUNCE( SLWIN );

HB_CALL_ON_STARTUP_BEGIN( hb_lang_Init_SLWIN )
   hb_langRegister( &s_lang );
HB_CALL_ON_STARTUP_END( hb_lang_Init_SLWIN )

#if defined(HB_PRAGMA_STARTUP)                                         
   #pragma startup hb_lang_Init_SLWIN                                     
#elif defined(HB_MSC_STARTUP)                                          
   #if _MSC_VER >= 1010                                                
      #pragma data_seg( ".CRT$XIY" )                                   
      #pragma comment( linker, "/Merge:.CRT=.data" )                   
   #else                                                               
      #pragma data_seg( "XIY" )                                        
   #endif                                                              
   static HB_$INITSYM hb_vm_auto_hb_lang_Init_SLWIN = hb_lang_Init_SLWIN;    
   #pragma data_seg()                                                  
#endif                                                                 
