/*
 * $Id$
 */

/*
 * Harbour Project source code:
 * The FileSys API (Harbour level)
 *
 * Copyright 1999 {list of individual authors and e-mail addresses}
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

/*
 * The following parts are Copyright of the individual authors.
 * www - http://www.harbour-project.org
 *
 * Copyright 1999 Victor Szakats <info@szelvesz.hu>
 *    HB_CURDIR()
 *
 * See doc/license.txt for licensing terms.
 *
 */

#include <ctype.h>

#include "hbapi.h"
#include "hbapifs.h"
#include "hbapierr.h"

HARBOUR HB_FOPEN( void )
{
   if( ISCHAR( 1 ) )
      hb_retni( hb_fsOpen( ( BYTE * ) hb_parc( 1 ),
                           ISNUM( 2 ) ? hb_parni( 2 ) : FO_READ | FO_COMPAT ) );
   else
      hb_errRT_BASE( EG_ARG, 2021, NULL, "FOPEN" ); /* NOTE: Undocumented but existing Clipper Run-time error */
}

HARBOUR HB_FCREATE( void )
{
   if( ISCHAR( 1 ) )
      hb_retni( hb_fsCreate( ( BYTE * ) hb_parc( 1 ),
                             ISNUM( 2 ) ? hb_parni( 2 ) : FC_NORMAL ) );
   else
      hb_retni( FS_ERROR );
}

HARBOUR HB_FREAD( void )
{
   ULONG ulRead;

   if( ISNUM( 1 ) && ISCHAR( 2 ) && ISBYREF( 2 ) && ISNUM( 3 ) )
   {
      ulRead = hb_parnl( 3 );

      /* NOTE: CA-Clipper determines the maximum size by calling _parcsiz()
               instead of _parclen(), this means that the maximum read length
               will be one more than the length of the passed buffer, because
               the terminating zero could be used if needed. [vszakats] */

      if( ulRead <= hb_parcsiz( 2 ) )
         /* NOTE: Warning, the read buffer will be directly modified,
                  this is normal here ! [vszakats] */
         ulRead = hb_fsReadLarge( hb_parni( 1 ),
                                  ( BYTE * ) hb_parc( 2 ),
                                  ulRead );
      else
         ulRead = 0;
   }
   else
      ulRead = 0;

   hb_retnl( ulRead );
}

HARBOUR HB_FWRITE( void )
{
   if( ISNUM( 1 ) && ISCHAR( 2 ) )
      hb_retnl( hb_fsWriteLarge( hb_parni( 1 ),
                                 ( BYTE * ) hb_parc( 2 ),
                                 ISNUM( 3 ) ? hb_parnl( 3 ) : hb_parclen( 2 ) ) );
   else
      hb_retnl( 0 );
}

HARBOUR HB_FERROR( void )
{
   hb_retni( hb_fsError() );
}

HARBOUR HB_FCLOSE( void )
{
   hb_fsSetError( 0 );

   if( ISNUM( 1 ) )
   {
      hb_fsClose( hb_parni( 1 ) );
      hb_retl( hb_fsError() == 0 );
   }
   else
      hb_retl( FALSE );
}

HARBOUR HB_FERASE( void )
{
   hb_fsSetError( 3 );

   if( ISCHAR( 1 ) )
      hb_retni( hb_fsDelete( ( BYTE * ) hb_parc( 1 ) ) );
   else
      hb_retni( -1 );
}

HARBOUR HB_FRENAME( void )
{
   hb_fsSetError( 2 );

   if( ISCHAR( 1 ) && ISCHAR( 2 ) )
      hb_retni( hb_fsRename( ( BYTE * ) hb_parc( 1 ), ( BYTE * ) hb_parc( 2 ) ) );
   else
      hb_retni( -1 );
}

HARBOUR HB_FSEEK( void )
{
   if( ISNUM( 1 ) && ISNUM( 2 ) )
      hb_retnl( hb_fsSeek( hb_parni( 1 ),
                           hb_parnl( 2 ),
                           ISNUM( 3 ) ? hb_parni( 3 ) : FS_SET ) );
   else
      hb_retnl( 0 );
}

HARBOUR HB_FILE( void )
{
   hb_retl( ISCHAR( 1 ) ? hb_fsFile( ( BYTE * ) hb_parc( 1 ) ) : FALSE );
}

HARBOUR HB_FREADSTR( void )
{
   if( ISNUM( 1 ) && ISNUM( 2 ) )
   {
      ULONG ulToRead = ( ULONG ) hb_parnl( 2 );

      if( ulToRead > 0 )
      {
         FHANDLE fhnd = ( FHANDLE ) hb_parni( 1 );
         BYTE * buffer = ( BYTE * ) hb_xgrab( ulToRead + 1 );
         ULONG ulRead;

         ulRead = hb_fsReadLarge( fhnd, buffer, ulToRead );
         buffer[ ulRead ] = '\0';

         /* NOTE: Clipper will not return zero chars from this functions. */

         hb_retc( ( char * ) buffer );

         hb_xfree( buffer );
      }
      else
         hb_retc( "" );
   }
   else
      hb_retc( "" );
}

/* NOTE: This function should not return the leading and trailing */
/*       (back)slashes. [vszakats] */

HARBOUR HB_CURDIR( void )
{
   USHORT uiErrorOld = hb_fsError();
   BYTE * pbyBuffer = ( BYTE * ) hb_xgrab( _POSIX_PATH_MAX + 1 );

   hb_fsCurDirBuff( ( ISCHAR( 1 ) && hb_parclen( 1 ) > 0 ) ?
      ( USHORT )( toupper( *hb_parc( 1 ) ) - 'A' + 1 ) : 0, pbyBuffer, _POSIX_PATH_MAX + 1 );

   hb_retc( ( char * ) pbyBuffer );
   hb_xfree( pbyBuffer );

   hb_fsSetError( uiErrorOld );
}

