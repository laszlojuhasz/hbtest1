/*
 * $Id$
 */

/*
 * Harbour Project source code:
 * ISPRINTER() function
 *
 * Copyright 1999-2007 Viktor Szakats <viktor.szakats@syenar.hu>
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

#include "hbapi.h"
#include "hbapifs.h"

HB_EXPORT BOOL hb_printerIsReady( char * pszPrinterName )
{
   BOOL bIsPrinter;

#if defined(HB_OS_DOS)

   /* NOTE: DOS specific solution, using BIOS interrupt */

   {
      USHORT uiPort;
      
      if( pszPrinterName == NULL )
         pszPrinterName = "LPT1";

      if( hb_strnicmp( pszPrinterName, "PRN", 3 ) == 0 )
      {
         union REGS regs;
      
         regs.h.ah = 2;
         regs.HB_XREGS.dx = 0; /* LPT1 */
      
         HB_DOS_INT86( 0x17, &regs, &regs );
      
         bIsPrinter = ( regs.h.ah == 0x90 );
      }
      else if( strlen( pszPrinterName ) >= 4 && 
               hb_strnicmp( pszPrinterName, "LPT", 3 ) == 0 && 
               ( uiPort = atoi( pszPrinterName + 3 ) ) > 0 )
      {
         union REGS regs;
      
         regs.h.ah = 2;
         regs.HB_XREGS.dx = uiPort - 1;
      
         HB_DOS_INT86( 0x17, &regs, &regs );
      
         bIsPrinter = ( regs.h.ah == 0x90 );
      }
      else
         bIsPrinter = FALSE;
   }

#elif defined(HB_OS_WIN_32)

   /* NOTE: Platform independent method, at least it will compile and run
            on any platform, but the result may not be the expected one,
            since Unix/Linux doesn't support LPT/COM by nature, other OSs
            may not reflect the actual physical presence of the printer when
            trying to open it, since we are talking to the spooler.
            [vszakats] */

   {
      HB_FHANDLE fhnd;
      
      if( pszPrinterName == NULL )
         pszPrinterName = "LPT1";

      fhnd = hb_fsOpen( ( BYTE * ) pszPrinterName, FO_WRITE | FO_SHARED | FO_PRIVATE );
      bIsPrinter = ( fhnd != FS_ERROR );
      hb_fsClose( fhnd );
   }

#else

   {
      /* TODO */

      HB_SYMBOL_UNUSED( pszPrinterName );

      bIsPrinter = FALSE;
   }

#endif

   return bIsPrinter;
}

HB_FUNC( HB_ISPRINTER )
{
   hb_retl( hb_printerIsReady( hb_parc( 1 ) ) );
}

/* NOTE: The parameter is an XBase++ extension over CA-Cl*pper. [vszakats] */

HB_FUNC( ISPRINTER )
{
#ifdef HB_COMPAT_XPP
   HB_FUNC_EXEC( HB_ISPRINTER );
#else
   hb_retl( hb_printerIsReady( NULL ) );
#endif
}
