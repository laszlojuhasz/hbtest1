/*
 * $Id$
 */

/*
 * Harbour Project source code:
 * generate line information for debugger
 *
 * Copyright 2006 Przemyslaw Czerpak < druzus /at/ priv.onet.pl >
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

#include "hbcomp.h"

PHB_DEBUGINFO hb_compGetDebugInfo( HB_COMP_DECL )
{
   PHB_DEBUGINFO pLineInfo = NULL, pInfo = NULL;
   ULONG ulPos, ulSkip, ulLine, ulOffset;
   const char * pszModuleName = "", * ptr;
   PFUNCTION pFunc;

   pFunc = HB_COMP_PARAM->functions.pFirst;

   while( pFunc )
   {
      if( ( pFunc->funFlags & FUN_FILE_DECL ) == 0 )
      {
         ulPos = ulLine = 0;
         while( ulPos < pFunc->lPCodePos )
         {
            ulSkip = 0;
            switch( pFunc->pCode[ ulPos ] )
            {
               case HB_P_LINE:
                  ulLine = HB_PCODE_MKUSHORT( &pFunc->pCode[ ulPos + 1 ] );
                  break;

               case HB_P_MODULENAME:
                  pszModuleName = ( const char * ) &pFunc->pCode[ ulPos + 1 ];
                  pInfo = NULL;
                  break;

               /*
                * This enables checking also code block bodies,
                * if it's not necessary then simply remove the
                * code below. [druzus]
                */
               case HB_P_PUSHBLOCKLARGE:
                  ulSkip = 8 + HB_PCODE_MKUSHORT( &pFunc->pCode[ ulPos + 6 ] ) * 2;
                  break;

               case HB_P_PUSHBLOCK:
                  ulSkip = 7 + HB_PCODE_MKUSHORT( &pFunc->pCode[ ulPos + 5 ] ) * 2;
                  break;

               case HB_P_PUSHBLOCKSHORT:
                  ulSkip = 2;
                  break;
            }

            if( ulLine != 0 )
            {
               if( !pInfo )
               {
                  int i;

                  ptr = strchr( pszModuleName, ':' );
                  i = ptr ? ( int ) ( ptr - pszModuleName ) : ( int ) strlen( pszModuleName );

                  pInfo = pLineInfo;
                  while( pInfo != NULL )
                  {
                     if( strncmp( pszModuleName, pInfo->pszModuleName, i ) == 0 &&
                         ( pInfo->pszModuleName[ i ] == '\0' ||
                           pInfo->pszModuleName[ i ] == ':' ) )
                        break;
                     pInfo = pInfo->pNext;
                  }
                  if( !pInfo )
                  {
                     pInfo = ( PHB_DEBUGINFO ) hb_xgrab( sizeof( HB_DEBUGINFO ) );
                     pInfo->pszModuleName = hb_strndup( pszModuleName, i );
                     pInfo->ulFirstLine = pInfo->ulLastLine = ulLine;
                     /*
                      * allocate memory in 256 bytes chunks (for 2048 lines)
                      * The last 1 byte is reserved for additional 0 byte if
                      * the caller will want to use the returned buffer as
                      * parameter to hb_compGenPushString(). [druzus]
                      */
                     pInfo->ulAllocated = ( ( ulLine >> 3 ) + 0x100 ) & 0xFFFFFF00L;
                     pInfo->pLineMap = ( BYTE * ) hb_xgrab( pInfo->ulAllocated + 1 );
                     memset( pInfo->pLineMap, 0, pInfo->ulAllocated + 1 );
                     pInfo->pNext = pLineInfo;
                     pLineInfo = pInfo;
                  }
               }
               ulOffset = ulLine >> 3;
               if( pInfo->ulAllocated <= ulOffset )
               {
                  ULONG ulNewSize = ( ( ulLine >> 3 ) + 0x100 ) & 0xFFFFFF00L;
                  pInfo->pLineMap = ( BYTE * ) hb_xrealloc( pInfo->pLineMap, ulNewSize + 1 );
                  memset( pInfo->pLineMap + pInfo->ulAllocated, 0, ulNewSize - pInfo->ulAllocated + 1 );
                  pInfo->ulAllocated = ulNewSize;
               }
               pInfo->pLineMap[ ulOffset ] |= 1 << ( ulLine & 0x7 );
               /*
                * It's possible the the line number will be ascending
                * if some external file is included more then once. [druzus]
                */
               if( pInfo->ulFirstLine > ulLine )
                  pInfo->ulFirstLine = ulLine;
               if( pInfo->ulLastLine < ulLine )
                  pInfo->ulLastLine = ulLine;
               ulLine = 0;
            }

            if( ulSkip == 0 )
            {
               ulSkip = hb_compPCodeSize( pFunc, ulPos );
               if( ulSkip == 0 )
                  break;
            }
            ulPos += ulSkip;
         }
      }
      pFunc = pFunc->pNext;
   }

   return pLineInfo;
}
