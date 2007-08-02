/*
 * $Id$
 */

/*
 * Harbour Project source code:
 *   CT3 video functions:
 *
 * SAYDOWN(), SAYSPREAD(), SAYMOVEIN()
 *
 * Copyright 2007 Przemyslaw Czerpak <druzus / at / priv.onet.pl>
 *
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

#include "hbapigt.h"
#include "hbgtcore.h"

HB_FUNC( SAYDOWN )
{
   ULONG ulLen = hb_parclen( 1 );

   if( ulLen )
   {
      UCHAR * szText = ( UCHAR * ) hb_parc( 1 );
      SHORT sRow, sCol;
      int iRow, iCol, iMaxRow, iMaxCol;
      long lDelay;

      lDelay = ISNUM( 2 ) ? hb_parnl( 2 ) : 4;

      hb_gtGetPos( &sRow, &sCol );
      iRow = ISNUM( 3 ) ? hb_parni( 3 ) : ( int ) sRow;
      iCol = ISNUM( 4 ) ? hb_parni( 4 ) : ( int ) sCol;
      iMaxRow = hb_gtMaxRow();
      iMaxCol = hb_gtMaxCol();

      if( iRow >= 0 && iCol >= 0 && iRow <= iMaxRow && iCol <= iMaxCol )
      {
         BYTE bColor = hb_gt_GetColor();

         if( ulLen > ( ULONG ) ( iMaxRow - iRow + 1 ) )
            ulLen = ( ULONG ) ( iMaxRow - iRow + 1 );

         hb_gtBeginWrite();
         while( ulLen-- )
         {
            hb_gtPutChar( iRow++, iCol, bColor, 0, *szText++ );
            if( lDelay )
            {
               hb_gtEndWrite();
               hb_idleSleep( ( double ) lDelay / 1000 );
               hb_gtBeginWrite();
            }
         }
         hb_gtEndWrite();
      }
   }

   hb_retc( NULL );
}

HB_FUNC( SAYSPREAD )
{
   ULONG ulLen = hb_parclen( 1 );

   if( ulLen )
   {
      UCHAR * szText = ( UCHAR * ) hb_parc( 1 );
      ULONG ulPos, ul;
      SHORT sRow, sCol;
      int iRow, iCol, iMaxRow, iMaxCol;
      long lDelay;

      lDelay = ISNUM( 2 ) ? hb_parnl( 2 ) : 4;

      iMaxRow = hb_gtMaxRow();
      iMaxCol = hb_gtMaxCol();
      hb_gtGetPos( &sRow, &sCol );
      iRow = ISNUM( 3 ) ? hb_parni( 3 ) : ( int ) sRow;
      iCol = ISNUM( 4 ) ? hb_parni( 4 ) : ( iMaxCol >> 1 );

      if( iRow >= 0 && iCol >= 0 && iRow <= iMaxRow && iCol <= iMaxCol )
      {
         BYTE bColor = hb_gt_GetColor();

         ulPos = ulLen >> 1;
         ulLen = ulLen & 1;
         if( !ulLen )
         {
            ulLen = 2;
            --ulPos;
         }

         hb_gtBeginWrite();
         do
         {
            for( ul = 0; ul < ulLen && iCol + ( int ) ul <= iMaxCol; ++ul )
               hb_gtPutChar( iRow, iCol + ul, bColor, 0, szText[ulPos + ul] );
            ulLen += 2;
            if( lDelay )
            {
               hb_gtEndWrite();
               hb_idleSleep( ( double ) lDelay / 1000 );
               hb_gtBeginWrite();
            }
         }
         while( ulPos-- && iCol-- );
         /* CT3 does not respect iCol in the above condition */
         hb_gtEndWrite();
      }
   }

   hb_retc( NULL );
}

HB_FUNC( SAYMOVEIN )
{
   ULONG ulLen = hb_parclen( 1 );

   if( ulLen )
   {
      UCHAR * szText = ( UCHAR * ) hb_parc( 1 );
      ULONG ulChars, ul;
      SHORT sRow, sCol;
      int iRow, iCol, iMaxRow, iMaxCol;
      long lDelay;
      BOOL fBack;

      lDelay = ISNUM( 2 ) ? hb_parnl( 2 ) : 4;
      fBack = ISLOG( 5 ) && hb_parl( 5 );

      iMaxRow = hb_gtMaxRow();
      iMaxCol = hb_gtMaxCol();
      hb_gtGetPos( &sRow, &sCol );
      iRow = ISNUM( 3 ) ? hb_parni( 3 ) : ( int ) sRow;
      iCol = ISNUM( 4 ) ? hb_parni( 4 ) : ( int ) sCol;

      if( iRow >= 0 && iCol >= 0 && iRow <= iMaxRow && iCol <= iMaxCol )
      {
         BYTE bColor = hb_gt_GetColor();

         sRow = iRow;
         sCol = iCol + ulLen;
         if( fBack )
            iCol += ulLen - 1;
         else
            szText += ulLen - 1;
         ulChars = 1;

         hb_gtBeginWrite();
         do
         {
            if( fBack )
            {
               if( iCol <= iMaxCol )
               {
                  for( ul = 0; ul < ulChars; ++ul )
                     hb_gtPutChar( iRow, iCol + ul, bColor, 0, szText[ul] );
               }
               --iCol;
            }
            else
            {
               for( ul = 0; ul < ulChars; ++ul )
                  hb_gtPutChar( iRow, iCol + ul, bColor, 0, szText[ul] );
               --szText;
            }
            if( ( int ) ulChars + iCol <= iMaxCol )
               ++ulChars;

            if( lDelay )
            {
               hb_gtEndWrite();
               hb_idleSleep( ( double ) lDelay / 1000 );
               hb_gtBeginWrite();
            }
         }
         while( --ulLen );
         hb_gtSetPos( sRow, sCol );
         hb_gtEndWrite();
      }
   }

   hb_retc( NULL );
}
