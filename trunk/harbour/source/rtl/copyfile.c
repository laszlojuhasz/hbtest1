/*
 * $Id$

   Copyright(C) 1999 by Andi Jahja

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published
   by the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful, but
   WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR
   PURPOSE.  See the GNU General Public License for more details.

   You should have received a copy of the GNU General Public
   License along with this program; if not, write to:

   The Free Software Foundation, Inc.,
   675 Mass Ave, Cambridge, MA 02139, USA.

   You can contact me at: andij@aonlippo.co.id
*/

#include "extend.h"
#include "errorapi.h"
#include "filesys.h"

#ifdef OS_UNIX_COMPATIBLE
   #include <sys/stat.h>
   #include <unistd.h>
#endif

#define BUFFER_SIZE 8192

static BOOL hb_fsCopy( char * szSource, char * szDest, ULONG * pulWrittenTotal )
{
   BOOL bRetVal = FALSE;
   ULONG ulWrittenTotal = 0L;

   FHANDLE fhndSource;
   FHANDLE fhndDest;

   while( ( fhndSource = hb_fsOpen( ( BYTE * ) szSource, FO_READ ) ) == FS_ERROR )
   {
      if( hb_errRT_BASE_Ext1( EG_OPEN, 2012, NULL, szSource, hb_fsError(), EF_CANDEFAULT | EF_CANRETRY ) == E_DEFAULT )
      {
         ulWrittenTotal = ( ULONG ) -1L;
         break;
      }
   }

   if( fhndSource != FS_ERROR )
   {
      while( ( fhndDest = hb_fsCreate( ( BYTE * ) szDest, FC_NORMAL ) ) == FS_ERROR )
      {
         if( hb_errRT_BASE_Ext1( EG_CREATE, 2012, NULL, szDest, hb_fsError(), EF_CANDEFAULT | EF_CANRETRY ) == E_DEFAULT )
         {
            ulWrittenTotal = ( ULONG ) -2L;
            break;
         }
      }

      if( fhndDest != FS_ERROR )
      {
#ifdef OS_UNIX_COMPATIBLE
         struct stat struFileInfo;
         int iSuccess = fstat( fhndSource, &struFileInfo );
#endif
         BYTE * buffer;
         USHORT usRead;
         USHORT usWritten;

         buffer = ( BYTE * ) hb_xgrab( BUFFER_SIZE );

         /* QUESTION: Does Clipper throw an error on read or write operation ? */
         /* QUESTION: What is the E_DEFAULT behaviour on that error ? */

         bRetVal = TRUE;

         while( ( usRead = hb_fsRead( fhndSource, buffer, BUFFER_SIZE ) ) != 0 )
         {
            while( ( usWritten = hb_fsWrite( fhndDest, buffer, usRead ) ) != usRead )
            {
               if( hb_errRT_BASE_Ext1( EG_WRITE, 2012, NULL, szDest, hb_fsError(), EF_CANDEFAULT | EF_CANRETRY ) == E_DEFAULT )
               {
                  bRetVal = FALSE;
                  break;
               }
            }

            ulWrittenTotal += ( ULONG ) usWritten;
         }

         hb_xfree( buffer );

#ifdef OS_UNIX_COMPATIBLE
         if( iSuccess == 0 )
            fchmod( fhndDest, struFileInfo.st_mode );
#endif

         hb_fsClose( fhndDest );
      }

      hb_fsClose( fhndSource );
   }

   if( pulWrittenTotal != NULL )
      *pulWrittenTotal = ulWrittenTotal;

   return bRetVal;
}

/* Clipper returns .F. on failure and NIL on success */

HARBOUR HB___COPYFILE( void )
{
   if( ISCHAR( 1 ) && ISCHAR( 2 ) )
   {
#ifdef HARBOUR_STRICT_CLIPPER_COMPATIBILITY
      if( ! hb_fsCopy( hb_parc( 1 ), hb_parc( 2 ), NULL ) )
         hb_retl( FALSE );
#else
      ULONG ulWrittenTotal;
      hb_fsCopy( hb_parc( 1 ), hb_parc( 2 ), &ulWrittenTotal );
      hb_retnl( ulWrittenTotal );
#endif
   }
   else
      hb_errRT_BASE( EG_ARG, 2010, NULL, "__COPYFILE" );
}
