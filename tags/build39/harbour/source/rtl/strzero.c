/*
 * $Id$
 */

/*
 * Harbour Project source code:
 * STRZERO() function
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

#include "hbapi.h"
#include "hbapiitm.h"
#include "hbapierr.h"

HB_FUNC( STRZERO )
{
   if( hb_pcount() >= 1 && hb_pcount() <= 3 )
   {
      BOOL bValid;
      PHB_ITEM pNumber = hb_param( 1, HB_IT_NUMERIC );
      PHB_ITEM pWidth  = NULL;
      PHB_ITEM pDec    = NULL;

      if( pNumber )
      {
         bValid = TRUE;

         if( hb_pcount() >= 2 )
         {
            pWidth = hb_param( 2, HB_IT_NUMERIC );
            if( !pWidth )
               bValid = FALSE;
         }

         if( hb_pcount() >= 3 )
         {
            pDec = hb_param( 3, HB_IT_NUMERIC );
            if( !pDec )
               bValid = FALSE;
         }
      }
      else
         bValid = FALSE;

      if( bValid )
      {
         char * szResult = hb_itemStr( pNumber, pWidth, pDec );

         if( szResult )
         {
            ULONG ulPos = 0;

            while( szResult[ ulPos ] != '\0' && szResult[ ulPos ] != '-' )
               ulPos++;

            if( szResult[ ulPos ] == '-' )
            {
               /* NOTE: Negative sign found, put it to the first position */

               szResult[ ulPos ] = ' ';

               ulPos = 0;
               while( szResult[ ulPos ] != '\0' && szResult[ ulPos ] == ' ' )
                  szResult[ ulPos++ ] = '0';

               szResult[ 0 ] = '-';
            }
            else
            {
               /* Negative sign not found */

               ulPos = 0;
               while( szResult[ ulPos ] != '\0' && szResult[ ulPos ] == ' ' )
                  szResult[ ulPos++ ] = '0';
            }

            hb_retc_buffer( szResult );
         }
         else
            hb_retc( NULL );
      }
      else
#ifdef HB_C52_STRICT
         /* NOTE: In CA-Cl*pper STRZERO() is written in Clipper, and will call
                  STR() to do the job, the error (if any) will also be thrown
                  by STR().  [vszakats] */
         hb_errRT_BASE_SubstR( EG_ARG, 1099, NULL, "STR", 3, hb_paramError( 1 ), hb_paramError( 2 ), hb_paramError( 3 ) );
#else
         hb_errRT_BASE_SubstR( EG_ARG, 9999, NULL, "STRZERO", 3, hb_paramError( 1 ), hb_paramError( 2 ), hb_paramError( 3 ) );
#endif
   }
}
