/*
 * $Id$
 */

/*
 * Harbour Project source code:
 * DIRCHANGE(), MAKEDIR(), DIRREMOVE(), ISDISK(), DISKCHANGE(), DISKNAME() functions
 *
 * Copyright 1999-2001 Viktor Szakats <viktor.szakats@syenar.hu>
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

/* NOTE: Clipper 5.3 functions */

#include <ctype.h>

#include "hbapi.h"
#include "hbapifs.h"

#ifdef HB_COMPAT_C53

HB_FUNC( DIRCHANGE )
{
   if( ISCHAR( 1 ) )
      hb_retni( hb_fsChDir( ( BYTE * ) hb_parc( 1 ) ) ? 0 : hb_fsError() );
   else
      hb_retni( -1 );
}

/* NOTE: Clipper 5.3 NG incorrectly states that the name of this function is
         DIRMAKE(), in reality it's not. */

HB_FUNC( MAKEDIR )
{
   if( ISCHAR( 1 ) )
      hb_retni( hb_fsMkDir( ( BYTE * ) hb_parc( 1 ) ) ? 0 : hb_fsError() );
   else
      hb_retni( -1 );
}

HB_FUNC( DIRREMOVE )
{
   if( ISCHAR( 1 ) )
      hb_retni( hb_fsRmDir( ( BYTE * ) hb_parc( 1 ) ) ? 0 : hb_fsError() );
   else
      hb_retni( -1 );
}

/* NOTE: Clipper 5.3 undocumented */

HB_FUNC( ISDISK )
{
   hb_retl( ( ISCHAR( 1 ) && hb_parclen( 1 ) > 0 ) ?
            hb_fsIsDrv( ( BYTE )( toupper( *hb_parc( 1 ) ) - 'A' ) ) == 0 :
            FALSE );
}

HB_FUNC( DISKCHANGE )
{
   hb_retl( ( ISCHAR( 1 ) && hb_parclen( 1 ) > 0 ) ?
            hb_fsChDrv( ( BYTE )( toupper( *hb_parc( 1 ) ) - 'A' ) ) == 0 :
            FALSE );
}

HB_FUNC( DISKNAME )
{
   char szDrive[ 1 ];

   szDrive[ 0 ] = ( ( char ) hb_fsCurDrv() ) + 'A';
   hb_retclen( szDrive, 1 );
}

#endif

