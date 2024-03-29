/*
 * $Id$
 */

/*
 * Harbour Project source code:
 * Exception handlers
 *
 * Copyright 1999 Antonio Linares <alinares@fivetech.com>
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

/*
 * The following parts are Copyright of the individual authors.
 * www - http://www.harbour-project.org
 *
 * Copyright 2008 Mindaugas Kavaliauskas (dbtopas at dbtopas.lt)
 *    hb_win32ExceptionHandler() Windows exception info dump code.
 *
 * See doc/license.txt for licensing terms.
 *
 */

#define HB_OS_WIN_32_USED

#include "hbapi.h"
#include "hbvm.h"
#include "hbapifs.h"
#include "hbdate.h"
#include "hbapierr.h"
#include "hbset.h"

#if defined(HB_OS_WIN_32) && !defined(HB_WINCE)

LONG WINAPI hb_win32ExceptionHandler( struct _EXCEPTION_POINTERS * pExceptionInfo )
{
   char msg[ ( HB_SYMBOL_NAME_LEN + HB_SYMBOL_NAME_LEN + 32 ) * 32 ];
   char buffer[ HB_SYMBOL_NAME_LEN + HB_SYMBOL_NAME_LEN + 5 ];
   char * ptr;
   USHORT uiLine;
   int iLevel;

   FILE * hLog = hb_fopen( hb_setGetCPtr( HB_SET_HBOUTLOG ), "a+" );

   if( hLog )
   {
      char szTime[ 9 ];
      int iYear, iMonth, iDay;

      char errmsg[ 4096 ];
      int errmsglen = sizeof( errmsg ) - 1;
      
      hb_dateToday( &iYear, &iMonth, &iDay );
      hb_dateTimeStr( szTime );
         
      fprintf( hLog, HB_I_("Application Exception - %s\n"), hb_cmdargARGV()[0] );
      fprintf( hLog, HB_I_("Terminated at: %04d.%02d.%02d %s\n"), iYear, iMonth, iDay, szTime );
      if( *hb_setGetCPtr( HB_SET_HBOUTLOGINFO ) )
         fprintf( hLog, HB_I_("Info: %s\n"), hb_setGetCPtr( HB_SET_HBOUTLOGINFO ) );

#if defined(HB_WINCE)
      {
         /* TODO */
      }
#elif defined(HB_OS_WIN_64)
      {
         /* TODO */
      }
#else
      {
         char              buf[ 32 ];
         PEXCEPTION_RECORD pExceptionRecord = pExceptionInfo->ExceptionRecord;
         PCONTEXT          pCtx = pExceptionInfo->ContextRecord;
         DWORD             dwExceptCode = pExceptionInfo->ExceptionRecord->ExceptionCode;
         unsigned char *   pc;
         unsigned int *    sc;
         unsigned int *    ebp;
         unsigned int      eip;
         unsigned int      j;
         int               i;
         
         snprintf( errmsg, errmsglen,
                   "\n"
                   "    Exception Code:%08X\n"
                   "    Exception Address:%08X\n"
                   "    EAX:%08X  EBX:%08X  ECX:%08X  EDX:%08X\n"
                   "    ESI:%08X  EDI:%08X EBP:%08X\n"
                   "    CS:EIP:%04X:%08X  SS:ESP:%04X:%08X\n"
                   "    DS:%04X  ES:%04X  FS:%04X  GS:%04X\n"
                   "    Flags:%08X\n",
                   ( UINT32 ) dwExceptCode, ( UINT32 ) pExceptionRecord->ExceptionAddress,
                   ( UINT32 ) pCtx->Eax, ( UINT32 ) pCtx->Ebx, ( UINT32 ) pCtx->Ecx,
                   ( UINT32 ) pCtx->Edx, ( UINT32 ) pCtx->Esi, ( UINT32 ) pCtx->Edi,
                   ( UINT32 ) pCtx->Ebp,
                   ( UINT32 ) pCtx->SegCs, ( UINT32 ) pCtx->Eip, ( UINT32 ) pCtx->SegSs,
                   ( UINT32 ) pCtx->Esp, ( UINT32 ) pCtx->SegDs, ( UINT32 ) pCtx->SegEs,
                   ( UINT32 ) pCtx->SegFs, ( UINT32 ) pCtx->SegGs,
                   ( UINT32 ) pCtx->EFlags );

         hb_strncat( errmsg, "    CS:EIP:", errmsglen );
         pc = ( unsigned char * ) pCtx->Eip;
         for( i = 0; i < 16; i++ )
         {
            if( IsBadReadPtr( pc, 1 ) )
               break;
            snprintf( buf, sizeof( buf ) - 1, " %02X", ( int ) pc[ i ] );
            hb_strncat( errmsg, buf, errmsglen );
         }
         hb_strncat( errmsg, "\n    SS:ESP:", errmsglen );
         sc = ( unsigned int * ) pCtx->Esp;
         for( i = 0; i < 16; i++ )
         {
            if( IsBadReadPtr( sc, 4 ) )
               break;
            snprintf( buf, sizeof( buf ), " %08X", sc[ i ] );
            hb_strncat( errmsg, buf, errmsglen );
         }
         hb_strncat( errmsg, "\n\n", errmsglen );
         hb_strncat( errmsg, "    C stack:\n", errmsglen );
         hb_strncat( errmsg, "    EIP:     EBP:       Frame: OldEBP, RetAddr, Params...\n", errmsglen );
         eip = pCtx->Eip;
         ebp = ( unsigned int * ) pCtx->Ebp;
         if( ! IsBadWritePtr( ebp, 8 ) )
         {
            for( i = 0; i < 20; i++ )
            {
               if( ( unsigned int ) ebp % 4 != 0 || IsBadWritePtr( ebp, 40 ) || ( unsigned int ) ebp >= ebp[ 0 ] )
                  break;
               snprintf( buf, sizeof( buf ), "    %08X %08X  ", ( int ) eip, ( int ) ebp );
               hb_strncat( errmsg, buf, errmsglen );
               for( j = 0; j < 10 && ( unsigned int )( ebp + j ) < ebp[ 0 ]; j++ )
               {
                  snprintf( buf, sizeof( buf ), " %08X", ebp[ j ] );
                  hb_strncat( errmsg, buf, errmsglen );
               }
               hb_strncat( errmsg, "\n", errmsglen );
               eip = ebp[ 1 ];
               ebp = ( unsigned int * ) ebp[ 0 ];
            }
            hb_strncat( errmsg, "\n", errmsglen );
         }

         fwrite( errmsg, sizeof( char ), strlen( errmsg ), hLog );
      }
#endif
   }

   msg[ 0 ] = '\0';
   ptr = msg;
   iLevel = 0;

   while( hb_procinfo( iLevel++, buffer, &uiLine, NULL ) )
   {
      snprintf( ptr, sizeof( msg ) - ( ptr - msg ), 
                HB_I_("Called from %s(%hu)\n"), buffer, uiLine );

      if( hLog )
         fwrite( ptr, sizeof( *ptr ), strlen( ptr ), hLog );

      ptr += strlen( ptr );
   }

   /* GUI */
   {
      LPTSTR lpStr = HB_TCHAR_CONVTO( msg );
      MessageBox( NULL, lpStr, TEXT( HB_I_("Application Exception") ), MB_ICONSTOP );
      HB_TCHAR_FREE( lpStr );
   }

   if( hLog )
   {
      fprintf( hLog, "------------------------------------------------------------------------\n");
      fclose( hLog );
   }

   return EXCEPTION_CONTINUE_SEARCH; /* EXCEPTION_EXECUTE_HANDLER; */
}

#endif

#if defined(HB_OS_OS2)

static EXCEPTIONREGISTRATIONRECORD s_regRec;    /* Exception Registration Record */

ULONG _System hb_os2ExceptionHandler( PEXCEPTIONREPORTRECORD       p1,
                                      PEXCEPTIONREGISTRATIONRECORD p2,
                                      PCONTEXTRECORD               p3,
                                      PVOID                        pv )
{
   HB_SYMBOL_UNUSED( p1 );
   HB_SYMBOL_UNUSED( p2 );
   HB_SYMBOL_UNUSED( p3 );
   HB_SYMBOL_UNUSED( pv );

   /* Don't print stack trace if inside unwind, normal process termination or process killed or
      during debugging */
   if( p1->ExceptionNum != XCPT_UNWIND && p1->ExceptionNum < XCPT_BREAKPOINT )
   {
      char buffer[ HB_SYMBOL_NAME_LEN + HB_SYMBOL_NAME_LEN + 5 ];
      USHORT uiLine;
      int iLevel = 0;

      fprintf( stderr, HB_I_("\nException %lx at address %p \n"), p1->ExceptionNum, p1->ExceptionAddress );

      while( hb_procinfo( iLevel++, buffer, &uiLine, NULL ) )
         fprintf( stderr, HB_I_("Called from %s(%hu)\n"), buffer, uiLine );
   }

   return XCPT_CONTINUE_SEARCH;          /* Exception not resolved... */
}

#endif

void hb_vmSetExceptionHandler( void )
{
#if defined(HB_OS_WIN_32) && !defined(HB_WINCE)
   {
      LPTOP_LEVEL_EXCEPTION_FILTER ef = SetUnhandledExceptionFilter( hb_win32ExceptionHandler );
      HB_SYMBOL_UNUSED( ef );
   }
#elif defined(HB_OS_OS2) /* Add OS2TermHandler to this thread's chain of exception handlers */
   {
      APIRET rc;                             /* Return code                   */

      memset( &s_regRec, 0, sizeof( s_regRec ) );
      s_regRec.ExceptionHandler = ( ERR ) hb_os2ExceptionHandler;
      rc = DosSetExceptionHandler( &s_regRec );
      if( rc != NO_ERROR )
         hb_errInternal( HB_EI_ERRUNRECOV, "Unable to setup exception handler (DosSetExceptionHandler())", NULL, NULL );
   }
#endif
}

void hb_vmUnsetExceptionHandler( void )
{
#if defined(HB_OS_OS2) /* Add OS2TermHandler to this thread's chain of exception handlers */
   {
      APIRET rc;                             /* Return code                   */

      /* I don't do any check on return code since harbour is exiting in any case */
      rc = DosUnsetExceptionHandler( &s_regRec );
      HB_SYMBOL_UNUSED( rc );
   }
#endif
}
