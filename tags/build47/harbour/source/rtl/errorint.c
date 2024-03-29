/*
 * $Id$
 */

/*
 * Harbour Project source code:
 * The Error API (internal error)
 *
 * Copyright 1999-2004 Viktor Szakats <viktor.szakats@syenar.hu>
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
#include "hbapierr.h"
#include "hbapilng.h"
#include "hbstack.h"

/* NOTE: Use as minimal calls from here, as possible.
         Don't allocate memory from this function. [vszakats] */

void hb_errInternal( ULONG ulIntCode, const char * szText, const char * szPar1, const char * szPar2 )
{
   char buffer[ 1024 ];
   BOOL fLang;

   HB_TRACE(HB_TR_DEBUG, ("hb_errInternal(%lu, %s, %s, %s)", ulIntCode, szText, szPar1, szPar2));

   fLang = ( hb_langID() != NULL );

   hb_conOutErr( hb_conNewLine(), 0 );
   if( fLang )
      snprintf( buffer, sizeof( buffer ), ( char * ) hb_langDGetItem( HB_LANG_ITEM_BASE_ERRINTR ), ulIntCode );
   else
      snprintf( buffer, sizeof( buffer ), "Unrecoverable error %lu: ", ulIntCode );

   hb_conOutErr( buffer, 0 );
   if( szText )
      snprintf( buffer, sizeof( buffer ), szText, szPar1, szPar2 );
   else if( fLang )
      snprintf( buffer, sizeof( buffer ), ( char * ) hb_langDGetItem( HB_LANG_ITEM_BASE_ERRINTR + ulIntCode - 9000 ), szPar1, szPar2 );
   else
      buffer[ 0 ] = '\0';
   hb_conOutErr( buffer, 0 );
   hb_conOutErr( hb_conNewLine(), 0 );

   hb_stackDispCall();

   /* release console settings */
   hb_conRelease();

   if( hb_cmdargCheck( "ERRGPF" ) )
   {
       int *pGPF = NULL;
       *pGPF = 0;
       *(--pGPF) = 0;
   }

   exit( EXIT_FAILURE );
}
