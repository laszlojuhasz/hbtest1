/*
 * $Id$
 */

/*
 * Harbour Project source code:
 * Windows pcode DLL entry point and VM/RTL routing functions
 *
 * Copyright 2001 Antonio Linares <alinares@fivetech.com>
 * www - http://www.harbour-project.org
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version, with one exception:
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

#define HB_OS_WIN_32_USED

#include "hbvm.h"
#include "hbapiitm.h"

typedef void ( * VM_PROCESS_DLL_SYMBOLS ) ( PHB_SYMB pModuleSymbols,
                                            USHORT uiModuleSymbols );

typedef void ( * VM_DLL_EXECUTE ) ( const BYTE * pCode, PHB_SYMB pSymbols );

typedef BOOL ( * EXT_IS_ARRAY ) ( int iParam );
typedef char * ( * EXT_PARC1 )  ( int iParam );
typedef char * ( * EXT_PARC2 )  ( int iParam, ULONG ulArrayIndex );


#if defined(HB_OS_WIN_32)

BOOL WINAPI HB_EXPORT DllEntryPoint( HINSTANCE hInstance, DWORD fdwReason, PVOID pvReserved )
{
   HB_TRACE( HB_TR_DEBUG, ("DllEntryPoint(%p, %p, %d)", hInstance, fdwReason,
             pvReserved ) );

   HB_SYMBOL_UNUSED( hInstance );
   HB_SYMBOL_UNUSED( fdwReason );
   HB_SYMBOL_UNUSED( pvReserved );

   switch( fdwReason )
   {
      case DLL_PROCESS_ATTACH:
           break;

      case DLL_PROCESS_DETACH:
           break;
   }

   return TRUE;
}

/* module symbols initialization */
void hb_vmProcessSymbols( PHB_SYMB pModuleSymbols, USHORT uiModuleSymbols )
{
   /* notice hb_vmProcessDllSymbols() must be used, and not
    * hb_vmProcessSymbols(), as some special symbols pointers
    * adjustments are required
    */
   FARPROC pProcessSymbols = GetProcAddress( GetModuleHandle( NULL ),
                                             "_hb_vmProcessDllSymbols" );
   if( pProcessSymbols )
      ( ( VM_PROCESS_DLL_SYMBOLS ) pProcessSymbols ) ( pModuleSymbols,
                                                       uiModuleSymbols );
   /* else
    *    may we issue an error ? */
}

void hb_vmExecute( const BYTE * pCode, PHB_SYMB pSymbols )
{
   FARPROC pExecute = GetProcAddress( GetModuleHandle( NULL ), "_hb_vmExecute" );

   if( pExecute )
      ( ( VM_DLL_EXECUTE ) pExecute ) ( pCode, pSymbols );

   /* else
    *    may we issue an error ? */
}

/* extend API implementation for pcode DLLs */

char * hb_parc( int iParam, ... )
{
   FARPROC pExtIsArray = GetProcAddress( GetModuleHandle( NULL ), "_hb_extIsArray" );
   FARPROC pParC = GetProcAddress( GetModuleHandle( NULL ), "_hb_parc" );

   if( pExtIsArray && pParC )
   {
      if( ( ( EXT_IS_ARRAY ) pExtIsArray ) ( iParam ) )
      {
         va_list va;
         ULONG ulArrayIndex;

         va_start( va, iParam );
         ulArrayIndex = va_arg( va, ULONG );
         va_end( va );

         return ( ( EXT_PARC2 ) pParC )( iParam, ulArrayIndex );
      }
      else
         return ( ( EXT_PARC1 ) pParC )( iParam );
   }
   else
      return "";
}

#endif