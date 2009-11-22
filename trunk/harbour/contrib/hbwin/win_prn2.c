/*
 * $Id$
 */

/*
 * Harbour Project source code:
 * Harbour Windows Printing support functions
 *
 * Copyright 2002 Luiz Rafael Culik <culikr@uol.com.br>
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

#include "hbsetup.h"

#if defined( HB_OS_WIN ) && \
    !( defined( __RSXNT__ ) || defined( __CYGWIN__ ) || defined( HB_OS_WIN_CE ) )

#include <windows.h>

#if defined( __LCC__ )
#   include <winspool.h>
#endif

#define HB_OS_WIN_USED

#include "hbapi.h"
#include "hbapifs.h"
#include "hbapiitm.h"

#define _ENUMPRN_FLAGS_             ( PRINTER_ENUM_LOCAL | PRINTER_ENUM_CONNECTIONS )

#define MAXBUFFERSIZE 255

static BOOL hb_IsLegacyDevice( const char * pszPrinterName )
{
   static const char * s_pszPrnDev[] = { "lpt1", "lpt2", "lpt3", "lpt4", "lpt5", "lpt6", "com1", "com2", "com3", "com4", NULL };
   int i;

   for( i = 0; s_pszPrnDev[ i ]; ++i )
   {
      if( hb_strnicmp( pszPrinterName, s_pszPrnDev[ i ], ( HB_SIZE ) strlen( s_pszPrnDev[ i ] ) ) == 0 )
         return TRUE;
   }

   return FALSE;
}

static BOOL hb_PrinterExists( const char * pszPrinterName )
{
   BOOL bResult = FALSE;

   /* Don't bother with test if '\' in string */
   if( ! strchr( pszPrinterName, HB_OS_PATH_LIST_SEP_CHR ) && ! hb_IsLegacyDevice( pszPrinterName ) )
   {
      DWORD dwNeeded = 0, dwReturned = 0;

      EnumPrinters( _ENUMPRN_FLAGS_, NULL, 5, ( LPBYTE ) NULL, 0, &dwNeeded, &dwReturned );
      if( dwNeeded )
      {
         PRINTER_INFO_5 * pPrinterEnumBak;
         PRINTER_INFO_5 * pPrinterEnum = pPrinterEnumBak = ( PRINTER_INFO_5 * ) hb_xgrab( dwNeeded );

         if( EnumPrinters( _ENUMPRN_FLAGS_, NULL, 5, ( LPBYTE ) pPrinterEnum, dwNeeded, &dwNeeded, &dwReturned ) )
         {
            DWORD i;

            for( i = 0; ! bResult && i < dwReturned; ++i, ++pPrinterEnum )
            {
               char * pszData = HB_TCHAR_CONVFROM( pPrinterEnum->pPrinterName );
               bResult = ( strcmp( pszPrinterName, pszData ) == 0 );
               HB_TCHAR_FREE( pszData );
            }
         }
         hb_xfree( pPrinterEnumBak );
      }
   }
   return bResult;
}

HB_FUNC( PRINTEREXISTS )
{
   hb_retl( HB_ISCHAR( 1 ) && hb_PrinterExists( hb_parc( 1 ) ) );
}

static BOOL hb_GetDefaultPrinter( char * pszPrinterName, HB_SIZE * pulBufferSize )
{
   BOOL bResult = FALSE;

   if( pszPrinterName && pulBufferSize )
   {
      OSVERSIONINFO osvi;

      pszPrinterName[ 0 ] = '\0';

      osvi.dwOSVersionInfoSize = sizeof( OSVERSIONINFO );
      GetVersionEx( &osvi );

      if( osvi.dwPlatformId == VER_PLATFORM_WIN32_NT && osvi.dwMajorVersion >= 5 ) /* Windows 2000 or later */
      {
         typedef BOOL( WINAPI * DEFPRINTER ) ( LPSTR, LPDWORD );
         DEFPRINTER fnGetDefaultPrinter;
         HMODULE hWinSpool = LoadLibrary( TEXT( "winspool.drv" ) );

         if( hWinSpool )
         {
            fnGetDefaultPrinter = ( DEFPRINTER ) GetProcAddress( hWinSpool, "GetDefaultPrinterA" );

            if( fnGetDefaultPrinter )
               bResult = ( *fnGetDefaultPrinter )( pszPrinterName, pulBufferSize );

            FreeLibrary( hWinSpool );
         }
      }

      if( ! bResult ) /* Win9x and Windows NT 4.0 or earlier & 2000+ if necessary for some reason i.e. dll could not load! */
      {
         LPTSTR lpPrinterName = ( LPTSTR ) hb_xgrab( *pulBufferSize * sizeof( TCHAR ) );

         DWORD dwSize = GetProfileString( TEXT( "windows" ), TEXT( "device" ), TEXT( "" ), lpPrinterName, ( DWORD ) *pulBufferSize );

         HB_TCHAR_GETFROM( pszPrinterName, lpPrinterName, *pulBufferSize );

         hb_xfree( lpPrinterName );

         if( dwSize && dwSize < *pulBufferSize )
         {
            dwSize = 0;
            while( pszPrinterName[ dwSize ] != '\0' && pszPrinterName[ dwSize ] != ',' )
               dwSize++;
      
            pszPrinterName[ dwSize ] = '\0';
            *pulBufferSize = dwSize + 1;
            bResult = TRUE;
         }
         else
            *pulBufferSize = dwSize + 1;
      }

      if( ! bResult && osvi.dwPlatformId == VER_PLATFORM_WIN32_WINDOWS )
      {
/*
         This option should never be required but is included because of this article
             http://support.microsoft.com/kb/246772/en-us

         This option will not enumerate any network printers.

         From the SDK technical reference for EnumPrinters();

         If Level is 2 or 5, Name is a pointer to a null-terminated string that specifies
         the name of a server whose printers are to be enumerated.
         If this string is NULL, then the function enumerates the printers installed on the local machine.
*/

         DWORD dwNeeded = 0, dwReturned = 0;

         if( EnumPrinters( PRINTER_ENUM_DEFAULT, NULL, 2, NULL, 0, &dwNeeded, &dwReturned ) )
         {
            if( dwNeeded )
            {
               PRINTER_INFO_2 * pPrinterInfo = ( PRINTER_INFO_2 * ) hb_xgrab( dwNeeded );

               if( EnumPrinters( PRINTER_ENUM_DEFAULT, NULL, 2, ( LPBYTE ) pPrinterInfo, dwNeeded, &dwNeeded, &dwReturned ) && dwReturned )
               {
                  DWORD dwSize = ( DWORD ) lstrlen( pPrinterInfo->pPrinterName );

                  if( dwSize && dwSize < *pulBufferSize )
                  {
                     HB_TCHAR_GETFROM( pszPrinterName, pPrinterInfo->pPrinterName, lstrlen( pPrinterInfo->pPrinterName ) );
                     *pulBufferSize = dwSize + 1;
                     bResult = TRUE;
                  }
               }
               hb_xfree( pPrinterInfo );
            }
         }
      }
   }

   return bResult;
}

HB_FUNC( GETDEFAULTPRINTER )
{
   char szDefaultPrinter[ MAXBUFFERSIZE ];
   HB_SIZE ulBufferSize = sizeof( szDefaultPrinter );

   if( hb_GetDefaultPrinter( szDefaultPrinter, &ulBufferSize ) )
      hb_retclen( szDefaultPrinter, ulBufferSize - 1 );
   else
      hb_retc_null();
}

static BOOL hb_GetJobs( HANDLE hPrinter, JOB_INFO_2 ** ppJobInfo, int * piJobs )
{
   BOOL bResult = FALSE;
   DWORD dwByteNeeded;

   GetPrinter( hPrinter, 2, NULL, 0, &dwByteNeeded );
   if( dwByteNeeded )
   {
      PRINTER_INFO_2 * pPrinterInfo = ( PRINTER_INFO_2 * ) hb_xgrab( dwByteNeeded );
      DWORD dwByteUsed;

      if( GetPrinter( hPrinter, 2, ( LPBYTE ) pPrinterInfo, dwByteNeeded, &dwByteUsed ) )
      {
         DWORD dwReturned;

         EnumJobs( hPrinter, 0, pPrinterInfo->cJobs, 2, NULL, 0, &dwByteNeeded, &dwReturned );

         if( dwByteNeeded )
         {
            JOB_INFO_2 * pJobStorage = ( JOB_INFO_2 * ) hb_xgrab( dwByteNeeded );

            if( EnumJobs( hPrinter, 0, dwReturned, 2, ( LPBYTE ) pJobStorage, dwByteNeeded, &dwByteUsed, &dwReturned ) )
            {
               *piJobs = ( int ) dwReturned;
               *ppJobInfo = pJobStorage;
               bResult = TRUE;
            }
            else
               hb_xfree( pJobStorage );
         }
      }
      hb_xfree( pPrinterInfo );
   }
   return bResult;
}

static DWORD hb_GetPrinterStatus( HANDLE hPrinter )
{
   DWORD dwResult = ( DWORD ) -1;
   DWORD dwByteNeeded;

   GetPrinter( hPrinter, 2, NULL, 0, &dwByteNeeded );

   if( dwByteNeeded )
   {
      PRINTER_INFO_2 * pPrinterInfo = ( PRINTER_INFO_2 * ) hb_xgrab( dwByteNeeded );

      if( GetPrinter( hPrinter, 2, ( LPBYTE ) pPrinterInfo, dwByteNeeded, &dwByteNeeded ) )
         dwResult = pPrinterInfo->Status;

      hb_xfree( pPrinterInfo );
   }

   return dwResult;
}

static DWORD hb_IsPrinterErrorn( HANDLE hPrinter )
{
   DWORD dwError = hb_GetPrinterStatus( hPrinter );

   if( dwError == 0 )
   {
      JOB_INFO_2 * pJobs;
      int iJobs;

      if( hb_GetJobs( hPrinter, &pJobs, &iJobs ) )
      {
         int i;

         for( i = 0; dwError == 0 && i < iJobs; ++i )
         {
            if( pJobs[ i ].Status & JOB_STATUS_ERROR )
               dwError = ( DWORD ) -20;
            else if( pJobs[ i ].Status & JOB_STATUS_OFFLINE )
               dwError = ( DWORD ) -21;
            else if( pJobs[ i ].Status & JOB_STATUS_PAPEROUT )
               dwError = ( DWORD ) -22;
            else if( pJobs[ i ].Status & JOB_STATUS_BLOCKED_DEVQ )
               dwError = ( DWORD ) -23;
         }
         hb_xfree( pJobs );
      }
   }

   return dwError;
}

static DWORD hb_PrinterIsReadyn( const char * pszPrinterName )
{
   DWORD dwPrinter = ( DWORD ) -1;

   if( *pszPrinterName )
   {
      LPTSTR lpPrinterName = HB_TCHAR_CONVTO( pszPrinterName );
      HANDLE hPrinter;

      if( OpenPrinter( lpPrinterName, &hPrinter, NULL ) )
      {
         dwPrinter = hb_IsPrinterErrorn( hPrinter );
         CloseHandle( hPrinter );
      }

      HB_TCHAR_FREE( lpPrinterName );
   }
   return dwPrinter;
}

HB_FUNC( XISPRINTER )
{
   char szDefaultPrinter[ MAXBUFFERSIZE ];
   HB_SIZE ulBufferSize = sizeof( szDefaultPrinter );
   const char * pszPrinterName = hb_parc( 1 );

   if( hb_parclen( 1 ) == 0 )
   {
      hb_GetDefaultPrinter( szDefaultPrinter, &ulBufferSize );
      pszPrinterName = szDefaultPrinter;
   }

   hb_retnl( hb_PrinterIsReadyn( pszPrinterName ) );
}

static BOOL hb_GetPrinterNameByPort( char * pszPrinterName, HB_SIZE * pulBufferSize,
                                      const char * pszPortNameFind, BOOL bSubStr )
{
   BOOL bResult = FALSE;
   DWORD dwNeeded = 0, dwReturned = 0;

   EnumPrinters( _ENUMPRN_FLAGS_, NULL, 5, ( LPBYTE ) NULL, 0, &dwNeeded, &dwReturned );
   if( dwNeeded )
   {
      PRINTER_INFO_5 * pPrinterEnumBak;
      PRINTER_INFO_5 * pPrinterEnum = pPrinterEnumBak = ( PRINTER_INFO_5 * ) hb_xgrab( dwNeeded );

      if( EnumPrinters( _ENUMPRN_FLAGS_, NULL, 5, ( LPBYTE ) pPrinterEnum, dwNeeded, &dwNeeded, &dwReturned ) )
      {
         BOOL bFound = FALSE;
         DWORD i;

         for( i = 0; i < dwReturned && ! bFound; ++i, ++pPrinterEnum )
         {
            char * pszPortName = HB_TCHAR_CONVFROM( pPrinterEnum->pPortName );

            if( bSubStr )
               bFound = ( hb_strnicmp( pszPortName, pszPortNameFind, ( HB_SIZE ) strlen( pszPortNameFind ) ) == 0 );
            else
               bFound = ( hb_stricmp( pszPortName, pszPortNameFind ) == 0 );

            HB_TCHAR_FREE( pszPortName );

            if( bFound )
            {
               char * szPrinterName = HB_TCHAR_CONVFROM( pPrinterEnum->pPrinterName );
               if( *pulBufferSize >= strlen( szPrinterName ) + 1 )
               {
                  hb_strncpy( pszPrinterName, szPrinterName, *pulBufferSize );
                  bResult = TRUE;
               }
               /* Store name length + \0 char for return */
               *pulBufferSize = ( HB_SIZE ) strlen( szPrinterName ) + 1;
               HB_TCHAR_FREE( szPrinterName );
            }
         }
      }
      hb_xfree( pPrinterEnumBak );
   }
   return bResult;
}

HB_FUNC( PRINTERPORTTONAME )
{
   char szDefaultPrinter[ MAXBUFFERSIZE ];
   HB_SIZE ulBufferSize = sizeof( szDefaultPrinter );

   if( hb_parclen( 1 ) > 0 &&
       hb_GetPrinterNameByPort( szDefaultPrinter,
                                &ulBufferSize,
                                hb_parc( 1 ),
                                hb_parl( 2 ) ) )
      hb_retc( szDefaultPrinter );
   else
      hb_retc_null();
}

static int hb_PrintFileRaw( const char * pszPrinterName, const char * pszFileName, const char * pszDocName )
{
   HANDLE hPrinter;
   int iResult;
   LPTSTR lpPrinterName = HB_TCHAR_CONVTO( pszPrinterName );

   if( OpenPrinter( lpPrinterName, &hPrinter, NULL ) != 0 )
   {
      LPTSTR lpDocName = HB_TCHAR_CONVTO( pszDocName );
      DOC_INFO_1 DocInfo;
      DocInfo.pDocName = lpDocName;
      DocInfo.pOutputFile = NULL;
      DocInfo.pDatatype = TEXT( "RAW" );
      if( StartDocPrinter( hPrinter, 1, ( LPBYTE ) &DocInfo ) != 0 )
      {
         if( StartPagePrinter( hPrinter ) != 0 )
         {
            HB_FHANDLE fhnd = hb_fsOpen( pszFileName, FO_READ | FO_SHARED );

            if( fhnd != FS_ERROR )
            {
               BYTE pbyBuffer[ 32 * 1024 ];
               DWORD dwWritten = 0;
               USHORT nRead;

               while( ( nRead = hb_fsRead( fhnd, pbyBuffer, sizeof( pbyBuffer ) ) ) > 0 )
               {
                  /* TOFIX: This check seems wrong for any input files
                            larger than our read buffer, in such case it
                            will strip Chr( 26 ) from inside the file, which
                            means it will corrupt it. [vszakats] */
                  if( pbyBuffer[ nRead - 1 ] == 26 )
                     nRead--;   /* Skip the EOF() character */

                  WritePrinter( hPrinter, pbyBuffer, nRead, &dwWritten );
               }
               iResult = 1;
               hb_fsClose( fhnd );
            }
            else
               iResult = -6;
            EndPagePrinter( hPrinter );
         }
         else
            iResult = -4;
         EndDocPrinter( hPrinter );
      }
      else
         iResult = -3;
      HB_TCHAR_FREE( lpDocName );
      ClosePrinter( hPrinter );
   }
   else
      iResult = -2;

   HB_TCHAR_FREE( lpPrinterName );

   return iResult;
}

HB_FUNC( PRINTFILERAW )
{
   int iResult = -1;

   if( HB_ISCHAR( 1 ) && HB_ISCHAR( 2 ) )
      iResult = hb_PrintFileRaw( hb_parc( 1 ) /* pszPrinterName */,
                                 hb_parc( 2 ) /* pszFileName */,
                                 HB_ISCHAR( 3 ) ? hb_parc( 3 ) : hb_parc( 2 ) /* pszDocName */ );

   hb_retni( iResult );
}

/* Positions for GETPRINTERS() array */

#define HB_WINPRN_NAME              1
#define HB_WINPRN_PORT              2
#define HB_WINPRN_TYPE              3
#define HB_WINPRN_DRIVER            4
#define HB_WINPRN_LEN_              4

HB_FUNC( GETPRINTERS )
{
   BOOL bPrinterNamesOnly = HB_ISLOG( 1 ) ? ! hb_parl( 1 ) : TRUE;
   BOOL bLocalPrintersOnly = hb_parl( 2 );
   DWORD dwNeeded = 0, dwReturned = 0, i;
   PHB_ITEM pTempItem = hb_itemNew( NULL );
   PHB_ITEM pPrinterArray = hb_itemNew( NULL );

   hb_arrayNew( pPrinterArray, 0 );

   EnumPrinters( _ENUMPRN_FLAGS_, NULL, 5, ( LPBYTE ) NULL, 0, &dwNeeded, &dwReturned );

   if( dwNeeded )
   {
      PRINTER_INFO_5 * pPrinterEnumBak;
      PRINTER_INFO_5 * pPrinterEnum = pPrinterEnumBak = ( PRINTER_INFO_5 * ) hb_xgrab( dwNeeded );

      if( EnumPrinters( _ENUMPRN_FLAGS_, NULL, 5, ( LPBYTE ) pPrinterEnum, dwNeeded, &dwNeeded, &dwReturned ) )
      {
         for( i = 0; i < dwReturned; ++i, ++pPrinterEnum )
         {
            char * pszData;

            if( ! bLocalPrintersOnly || pPrinterEnum->Attributes & PRINTER_ATTRIBUTE_LOCAL )
            {
               if( bPrinterNamesOnly )
               {
                  pszData = HB_TCHAR_CONVFROM( pPrinterEnum->pPrinterName );
                  hb_itemPutC( pTempItem, pszData );
                  HB_TCHAR_FREE( pszData );
                  hb_arrayAddForward( pPrinterArray, pTempItem );
               }
               else
               {
                  HANDLE hPrinter;

                  if( OpenPrinter( pPrinterEnum->pPrinterName, &hPrinter, NULL ) )
                  {
                     GetPrinter( hPrinter, 2, NULL, 0, &dwNeeded );
                     if( dwNeeded )
                     {
                        hb_arrayNew( pTempItem, HB_WINPRN_LEN_ );

                        pszData = HB_TCHAR_CONVFROM( pPrinterEnum->pPrinterName );
                        hb_arraySetC( pTempItem, HB_WINPRN_NAME, pszData );
                        HB_TCHAR_FREE( pszData );

                        {
                           PRINTER_INFO_2 * pPrinterInfo2 = ( PRINTER_INFO_2 * ) hb_xgrab( dwNeeded );

                           if( GetPrinter( hPrinter, 2, ( LPBYTE ) pPrinterInfo2, dwNeeded, &dwNeeded ) )
                           {
                              pszData = HB_TCHAR_CONVFROM( pPrinterInfo2->pPortName );
                              hb_arraySetC( pTempItem, HB_WINPRN_PORT, pszData );
                              HB_TCHAR_FREE( pszData );
                              pszData = HB_TCHAR_CONVFROM( pPrinterInfo2->pDriverName );
                              hb_arraySetC( pTempItem, HB_WINPRN_DRIVER, pszData );
                              HB_TCHAR_FREE( pszData );
                           }
                           else
                           {
                              hb_arraySetC( pTempItem, HB_WINPRN_PORT, NULL );
                              hb_arraySetC( pTempItem, HB_WINPRN_DRIVER, NULL );
                           }

                           hb_xfree( pPrinterInfo2 );
                        }

                        if( pPrinterEnum->Attributes & PRINTER_ATTRIBUTE_LOCAL )
                           hb_arraySetC( pTempItem, HB_WINPRN_TYPE, "LOCAL" );
                        else if( pPrinterEnum->Attributes & PRINTER_ATTRIBUTE_NETWORK )
                           hb_arraySetC( pTempItem, HB_WINPRN_TYPE, "NETWORK" );
                        else
                           hb_arraySetC( pTempItem, HB_WINPRN_TYPE, NULL );

                        hb_arrayAddForward( pPrinterArray, pTempItem );
                     }
                  }
                  CloseHandle( hPrinter );
               }
            }
         }
      }
      hb_xfree( pPrinterEnumBak );
   }

   hb_itemReturnRelease( pPrinterArray );

   hb_itemRelease( pTempItem );
}

#endif
