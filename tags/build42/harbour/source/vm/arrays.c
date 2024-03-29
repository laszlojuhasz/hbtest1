/*
 * $Id$
 */

/*
 * Harbour Project source code:
 * The Array API (C level)
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
 * Copyright 1999-2001 Viktor Szakats <viktor.szakats@syenar.hu>
 *    hb_arrayIsObject()
 *    hb_arrayCopyC()
 *    hb_arrayGetC()
 *
 * Copyright 2001 Ron Pinkas <ron@profit-master.com>
 *    hb_arrayClone()
 *    hb_arrayFromStack()
 *    hb_arrayFromParams()
 *
 * See doc/license.txt for licensing terms.
 *
 */

#include "hbapi.h"
#include "hbapiitm.h"
#include "hbapierr.h"
#include "hbapilng.h"
#include "hbvm.h"
#include "hbstack.h"

BOOL HB_EXPORT hb_arrayNew( PHB_ITEM pItem, ULONG ulLen ) /* creates a new array */
{
   PHB_BASEARRAY pBaseArray = ( PHB_BASEARRAY ) hb_gcAlloc( sizeof( HB_BASEARRAY ), hb_arrayReleaseGarbage );
   ULONG ulPos;

   HB_TRACE(HB_TR_DEBUG, ("hb_arrayNew(%p, %lu)", pItem, ulLen));

   hb_itemClear( pItem );

   pItem->type = HB_IT_ARRAY;

   if( ulLen > 0 )
      pBaseArray->pItems = ( PHB_ITEM ) hb_xgrab( sizeof( HB_ITEM ) * ulLen );
   else
      pBaseArray->pItems = NULL;

   pBaseArray->ulLen      = ulLen;
   pBaseArray->uiHolders  = 1;
   pBaseArray->uiClass    = 0;
   pBaseArray->uiPrevCls  = 0;

   for( ulPos = 0; ulPos < ulLen; ulPos++ )
      ( pBaseArray->pItems + ulPos )->type = HB_IT_NIL;

   pItem->item.asArray.value = pBaseArray;

   return TRUE;
}

BOOL HB_EXPORT hb_arrayAdd( PHB_ITEM pArray, PHB_ITEM pValue )
{
   HB_TRACE(HB_TR_DEBUG, ("hb_arrayAdd(%p, %p)", pArray, pValue));

   if( HB_IS_ARRAY( pArray ) )
   {
      PHB_BASEARRAY pBaseArray = ( PHB_BASEARRAY ) pArray->item.asArray.value;

      if( pBaseArray->ulLen < ULONG_MAX )
      {
         hb_arraySize( pArray, pBaseArray->ulLen + 1 );
         pBaseArray = ( PHB_BASEARRAY ) pArray->item.asArray.value;
         hb_itemCopy( pBaseArray->pItems + ( pBaseArray->ulLen - 1 ), pValue );

         return TRUE;
      }
   }

   return FALSE;
}

ULONG HB_EXPORT hb_arrayLen( PHB_ITEM pArray )
{
   HB_TRACE(HB_TR_DEBUG, ("hb_arrayLen(%p)", pArray));

   if( HB_IS_ARRAY( pArray ) )
      return pArray->item.asArray.value->ulLen;
   else
      return 0;
}

BOOL HB_EXPORT hb_arrayIsObject( PHB_ITEM pArray )
{
   HB_TRACE(HB_TR_DEBUG, ("hb_arrayIsObject(%p)", pArray));

   if( HB_IS_ARRAY( pArray ) )
      return pArray->item.asArray.value->uiClass != 0;
   else
      return FALSE;
}

BOOL HB_EXPORT hb_arraySize( PHB_ITEM pArray, ULONG ulLen )
{
   HB_TRACE(HB_TR_DEBUG, ("hb_arraySize(%p, %lu)", pArray, ulLen));

   if( HB_IS_ARRAY( pArray ) )
   {
      PHB_BASEARRAY pBaseArray = pArray->item.asArray.value;

      if( ulLen != pBaseArray->ulLen )
      {
         ULONG ulPos;

         if( pBaseArray->ulLen == 0 )
         {
            pBaseArray->pItems = ( PHB_ITEM ) hb_xgrab( ulLen * sizeof( HB_ITEM ) );

            for( ulPos = 0; ulPos < ulLen; ulPos++ )
               ( pBaseArray->pItems + ulPos )->type = HB_IT_NIL;
         }
         else
         {
            if( pBaseArray->ulLen < ulLen )
            {
               pBaseArray->pItems = ( PHB_ITEM ) hb_xrealloc( pBaseArray->pItems, sizeof( HB_ITEM ) * ulLen );

               /* set value for new items */
               for( ulPos = pBaseArray->ulLen; ulPos < ulLen; ulPos++ )
                  ( pBaseArray->pItems + ulPos )->type = HB_IT_NIL;
            }
            else if( pBaseArray->ulLen > ulLen )
            {
               /* release old items */
               for( ulPos = ulLen; ulPos < pBaseArray->ulLen; ulPos++ )
                  hb_itemClear( pBaseArray->pItems + ulPos );

               if( ulLen == 0 )
               {
                  hb_xfree( pBaseArray->pItems );
                  pBaseArray->pItems = NULL;
               }
               else
                  pBaseArray->pItems = ( PHB_ITEM ) hb_xrealloc( pBaseArray->pItems, sizeof( HB_ITEM ) * ulLen );
            }
         }

         pBaseArray->ulLen = ulLen;
      }

      return TRUE;
   }
   else
      return FALSE;
}

BOOL HB_EXPORT hb_arrayDel( PHB_ITEM pArray, ULONG ulIndex )
{
   HB_TRACE(HB_TR_DEBUG, ("hb_arrayDel(%p, %lu)", pArray, ulIndex));

   if( HB_IS_ARRAY( pArray ) )
   {
      ULONG ulLen = pArray->item.asArray.value->ulLen;

      if( ulIndex > 0 && ulIndex <= ulLen )
      {
         PHB_BASEARRAY pBaseArray = pArray->item.asArray.value;

         hb_itemClear( pBaseArray->pItems + ( ulIndex - 1 ) );

         for( ulIndex--; ulIndex < ulLen - 1; ulIndex++ )       /* move items */
         {
            hb_itemCopy( pBaseArray->pItems + ulIndex, pBaseArray->pItems + ( ulIndex + 1 ) );
         }

         hb_itemClear( pBaseArray->pItems + ( ulLen - 1 ) );
      }

      return TRUE;
   }
   else
      return FALSE;
}

BOOL HB_EXPORT hb_arrayIns( PHB_ITEM pArray, ULONG ulIndex )
{
   HB_TRACE(HB_TR_DEBUG, ("hb_arrayIns(%p, %lu)", pArray, ulIndex));

   if( HB_IS_ARRAY( pArray ) )
   {
      ULONG ulLen = pArray->item.asArray.value->ulLen;

      if( ulIndex > 0 && ulIndex <= ulLen )
      {
         PHB_BASEARRAY pBaseArray = pArray->item.asArray.value;

         hb_itemClear( pBaseArray->pItems + ( ulLen - 1 ) );

         for( ulLen--; ulLen >= ulIndex; ulLen-- )          /* move items */
            hb_itemCopy( pBaseArray->pItems + ulLen, pBaseArray->pItems + ( ulLen - 1 ) );

         hb_itemClear( pBaseArray->pItems + ulLen );
      }

      return TRUE;
   }
   else
      return FALSE;
}

BOOL HB_EXPORT hb_arraySet( PHB_ITEM pArray, ULONG ulIndex, PHB_ITEM pItem )
{
   HB_TRACE(HB_TR_DEBUG, ("hb_arraySet(%p, %lu, %p)", pArray, ulIndex, pItem));

   if( HB_IS_ARRAY( pArray ) && ulIndex > 0 && ulIndex <= pArray->item.asArray.value->ulLen )
   {
      hb_itemCopy( pArray->item.asArray.value->pItems + ( ulIndex - 1 ), pItem );
      return TRUE;
   }
   else
      return FALSE;
}

BOOL HB_EXPORT hb_arrayGet( PHB_ITEM pArray, ULONG ulIndex, PHB_ITEM pItem )
{
   HB_TRACE(HB_TR_DEBUG, ("hb_arrayGet(%p, %lu, %p)", pArray, ulIndex, pItem));

   if( HB_IS_ARRAY( pArray ) && ulIndex > 0 && ulIndex <= pArray->item.asArray.value->ulLen )
   {
      hb_itemCopy( pItem, pArray->item.asArray.value->pItems + ( ulIndex - 1 ) );
      return TRUE;
   }
   else
   {
      hb_itemClear( pItem );
      return FALSE;
   }
}

char * hb_arrayGetDS( PHB_ITEM pArray, ULONG ulIndex, char * szDate )
{
   HB_TRACE(HB_TR_DEBUG, ("hb_arrayGetDS(%p, %lu, %s)", pArray, ulIndex, szDate));

   if( HB_IS_ARRAY( pArray ) && ulIndex > 0 && ulIndex <= pArray->item.asArray.value->ulLen )
      return hb_itemGetDS( pArray->item.asArray.value->pItems + ulIndex - 1, szDate );
   else
      /* NOTE: Intentionally calling it with a bad parameter in order to get
               the default value from hb_itemGetDS(). [vszakats] */
      return hb_itemGetDS( NULL, szDate );
}

long hb_arrayGetDL( PHB_ITEM pArray, ULONG ulIndex )
{
   HB_TRACE(HB_TR_DEBUG, ("hb_arrayGetDL(%p, %lu)", pArray, ulIndex ));

   if( HB_IS_ARRAY( pArray ) && ulIndex > 0 && ulIndex <= pArray->item.asArray.value->ulLen )
      return hb_itemGetDL( pArray->item.asArray.value->pItems + ulIndex - 1 );
   else
      /* NOTE: Intentionally calling it with a bad parameter in order to get
               the default value from hb_itemGetDL(). [vszakats] */
      return hb_itemGetDL( NULL );
}

/*
 * This function returns a pointer to an item occupied by the specified
 * array element - it doesn't return an item's value
 */
PHB_ITEM hb_arrayGetItemPtr( PHB_ITEM pArray, ULONG ulIndex )
{
   HB_TRACE(HB_TR_DEBUG, ("hb_arrayGetItemPtr(%p, %lu)", pArray, ulIndex));

   if( HB_IS_ARRAY( pArray ) && ulIndex > 0 && ulIndex <= pArray->item.asArray.value->ulLen )
      return pArray->item.asArray.value->pItems + ulIndex - 1;
   else
      return NULL;
}

BOOL hb_arrayGetL( PHB_ITEM pArray, ULONG ulIndex )
{
   HB_TRACE(HB_TR_DEBUG, ("hb_arrayGetL(%p, %lu)", pArray, ulIndex));

   if( HB_IS_ARRAY( pArray ) && ulIndex > 0 && ulIndex <= pArray->item.asArray.value->ulLen )
      return hb_itemGetL( pArray->item.asArray.value->pItems + ulIndex - 1 );
   else
      return FALSE;
}

int hb_arrayGetNI( PHB_ITEM pArray, ULONG ulIndex )
{
   HB_TRACE(HB_TR_DEBUG, ("hb_arrayGetNI(%p, %lu)", pArray, ulIndex));

   if( HB_IS_ARRAY( pArray ) && ulIndex > 0 && ulIndex <= pArray->item.asArray.value->ulLen )
      return hb_itemGetNI( pArray->item.asArray.value->pItems + ulIndex - 1 );
   else
      return 0;
}

long hb_arrayGetNL( PHB_ITEM pArray, ULONG ulIndex )
{
   HB_TRACE(HB_TR_DEBUG, ("hb_arrayGetNL(%p, %lu)", pArray, ulIndex));

   if( HB_IS_ARRAY( pArray ) && ulIndex > 0 && ulIndex <= pArray->item.asArray.value->ulLen )
      return hb_itemGetNL( pArray->item.asArray.value->pItems + ulIndex - 1 );
   else
      return 0;
}

double hb_arrayGetND( PHB_ITEM pArray, ULONG ulIndex )
{
   HB_TRACE(HB_TR_DEBUG, ("hb_arrayGetND(%p, %lu)", pArray, ulIndex));

   if( HB_IS_ARRAY( pArray ) && ulIndex > 0 && ulIndex <= pArray->item.asArray.value->ulLen )
      return hb_itemGetND( pArray->item.asArray.value->pItems + ulIndex - 1 );
   else
      return 0;
}

ULONG hb_arrayCopyC( PHB_ITEM pArray, ULONG ulIndex, char * szBuffer, ULONG ulLen )
{
   HB_TRACE(HB_TR_DEBUG, ("hb_arrayCopyC(%p, %lu, %s, %lu)", pArray, ulIndex, szBuffer, ulLen));

   if( HB_IS_ARRAY( pArray ) && ulIndex > 0 && ulIndex <= pArray->item.asArray.value->ulLen )
      return hb_itemCopyC( pArray->item.asArray.value->pItems + ulIndex - 1, szBuffer, ulLen );
   else
      return 0;
}

char * hb_arrayGetC( PHB_ITEM pArray, ULONG ulIndex )
{
   HB_TRACE(HB_TR_DEBUG, ("hb_arrayGetC(%p, %lu)", pArray, ulIndex));

   if( HB_IS_ARRAY( pArray ) && ulIndex > 0 && ulIndex <= pArray->item.asArray.value->ulLen )
      return hb_itemGetC( pArray->item.asArray.value->pItems + ulIndex - 1 );
   else
      return NULL;
}

char * hb_arrayGetCPtr( PHB_ITEM pArray, ULONG ulIndex )
{
   HB_TRACE(HB_TR_DEBUG, ("hb_arrayGetCPtr(%p, %lu)", pArray, ulIndex));

   if( HB_IS_ARRAY( pArray ) && ulIndex > 0 && ulIndex <= pArray->item.asArray.value->ulLen )
      return hb_itemGetCPtr( pArray->item.asArray.value->pItems + ulIndex - 1 );
   else
      return "";
}

ULONG hb_arrayGetCLen( PHB_ITEM pArray, ULONG ulIndex )
{
   HB_TRACE(HB_TR_DEBUG, ("hb_arrayGetCLen(%p, %lu)", pArray, ulIndex));

   if( HB_IS_ARRAY( pArray ) && ulIndex > 0 && ulIndex <= pArray->item.asArray.value->ulLen )
      return hb_itemGetCLen( pArray->item.asArray.value->pItems + ulIndex - 1 );
   else
      return 0;
}

USHORT hb_arrayGetType( PHB_ITEM pArray, ULONG ulIndex )
{
   HB_TRACE(HB_TR_DEBUG, ("hb_arrayGetType(%p, %lu)", pArray, ulIndex));

   if( HB_IS_ARRAY( pArray ) && ulIndex > 0 && ulIndex <= pArray->item.asArray.value->ulLen )
      return hb_itemType( pArray->item.asArray.value->pItems + ulIndex - 1 );
   else
      return 0;
}

BOOL hb_arrayLast( PHB_ITEM pArray, PHB_ITEM pResult )
{
   HB_TRACE(HB_TR_DEBUG, ("hb_arrayLast(%p, %p)", pArray, pResult));

   if( HB_IS_ARRAY( pArray ) )
   {
      if( pArray->item.asArray.value->ulLen > 0 )
         hb_itemCopy( pResult, pArray->item.asArray.value->pItems +
                             ( pArray->item.asArray.value->ulLen - 1 ) );
      else
         hb_itemClear( pResult );

      return TRUE;
   }

   hb_itemClear( pResult );

   return FALSE;
}

BOOL hb_arrayFill( PHB_ITEM pArray, PHB_ITEM pValue, ULONG * pulStart, ULONG * pulCount )
{
   HB_TRACE(HB_TR_DEBUG, ("hb_arrayFill(%p, %p, %p, %p)", pArray, pValue, pulStart, pulCount));

   if( HB_IS_ARRAY( pArray ) )
   {
      PHB_BASEARRAY pBaseArray = pArray->item.asArray.value;
      ULONG ulLen = pBaseArray->ulLen;
      ULONG ulStart;
      ULONG ulCount;

      if( pulStart && ( *pulStart >= 1 ) )
         ulStart = *pulStart;
      else
         ulStart = 1;

      if( ulStart <= ulLen )
      {
         if( pulCount && ( *pulCount <= ulLen - ulStart ) )
            ulCount = *pulCount;
         else
            ulCount = ulLen - ulStart + 1;

         if( ulStart + ulCount > ulLen )             /* check range */
            ulCount = ulLen - ulStart + 1;

         for( ; ulCount > 0; ulCount--, ulStart++ )     /* set value items */
            hb_itemCopy( pBaseArray->pItems + ( ulStart - 1 ), pValue );
      }

      return TRUE;
   }
   else
      return FALSE;
}

ULONG hb_arrayScan( PHB_ITEM pArray, PHB_ITEM pValue, ULONG * pulStart, ULONG * pulCount )
{
   HB_TRACE(HB_TR_DEBUG, ("hb_arrayScan(%p, %p, %p, %p)", pArray, pValue, pulStart, pulCount));

   if( HB_IS_ARRAY( pArray ) )
   {
      PHB_BASEARRAY pBaseArray = pArray->item.asArray.value;
      ULONG ulLen = pBaseArray->ulLen;
      ULONG ulStart;
      ULONG ulCount;

      if( pulStart && ( *pulStart >= 1 ) )
         ulStart = *pulStart;
      else
         ulStart = 1;

      if( ulStart <= ulLen )
      {
         if( pulCount && ( *pulCount <= ulLen - ulStart ) )
            ulCount = *pulCount;
         else
            ulCount = ulLen - ulStart + 1;

         if( ulStart + ulCount > ulLen )             /* check range */
            ulCount = ulLen - ulStart + 1;

         /* Make separate search loops for different types to find, so that
            the loop can be faster. */

         if( HB_IS_BLOCK( pValue ) )
         {
            for( ulStart--; ulCount > 0; ulCount--, ulStart++ )
            {
               hb_vmPushSymbol( &hb_symEval );
               hb_vmPush( pValue );
               hb_vmPush( pBaseArray->pItems + ulStart );
               hb_vmPushNumber( ( double ) ( ulStart + 1 ), 0 );
               hb_vmDo( 2 );

               if( HB_IS_LOGICAL( &hb_stack.Return ) && hb_stack.Return.item.asLogical.value )
                  return ulStart + 1;                  /* arrays start from 1 */
            }
         }
         else if( HB_IS_STRING( pValue ) )
         {
            for( ulStart--; ulCount > 0; ulCount--, ulStart++ )
            {
               PHB_ITEM pItem = pBaseArray->pItems + ulStart;

               /* NOTE: The order of the pItem and pValue parameters passed to
                        hb_itemStrCmp() is significant, please don't change it. [vszakats] */
               if( HB_IS_STRING( pItem ) && hb_itemStrCmp( pItem, pValue, FALSE ) == 0 )
                  return ulStart + 1;
            }
         }
         else if( HB_IS_NUMERIC( pValue ) )
         {
            double dValue = hb_itemGetND( pValue );

            for( ulStart--; ulCount > 0; ulCount--, ulStart++ )
            {
               PHB_ITEM pItem = pBaseArray->pItems + ulStart;

               if( HB_IS_NUMERIC( pItem ) && hb_itemGetND( pItem ) == dValue )
                  return ulStart + 1;
            }
         }
         else if( HB_IS_DATE( pValue ) )
         {
            long lValue = hb_itemGetDL( pValue );

            for( ulStart--; ulCount > 0; ulCount--, ulStart++ )
            {
               PHB_ITEM pItem = pBaseArray->pItems + ulStart;

               if( HB_IS_DATE( pItem ) && hb_itemGetDL( pItem ) == lValue )
                  return ulStart + 1;
            }
         }
         else if( HB_IS_LOGICAL( pValue ) )
         {
            BOOL bValue = hb_itemGetL( pValue ); /* NOTE: This is correct: Get the date as a long value. [vszakats] */

            for( ulStart--; ulCount > 0; ulCount--, ulStart++ )
            {
               PHB_ITEM pItem = pBaseArray->pItems + ulStart;

               if( HB_IS_LOGICAL( pItem ) && hb_itemGetL( pItem ) == bValue )
                  return ulStart + 1;
            }
         }
         else if( HB_IS_NIL( pValue ) )
         {
            for( ulStart--; ulCount > 0; ulCount--, ulStart++ )
            {
               if( HB_IS_NIL( pBaseArray->pItems + ulStart ) )
                  return ulStart + 1;
            }
         }
      }
   }

   return 0;
}

BOOL hb_arrayEval( PHB_ITEM pArray, PHB_ITEM bBlock, ULONG * pulStart, ULONG * pulCount )
{
   HB_TRACE(HB_TR_DEBUG, ("hb_arrayEval(%p, %p, %p, %p)", pArray, bBlock, pulStart, pulCount));

   if( HB_IS_ARRAY( pArray ) && HB_IS_BLOCK( bBlock ) )
   {
      PHB_BASEARRAY pBaseArray = pArray->item.asArray.value;
      ULONG ulLen = pBaseArray->ulLen;
      ULONG ulStart;
      ULONG ulCount;

      if( pulStart && ( *pulStart >= 1 ) )
         ulStart = *pulStart;
      else
         ulStart = 1;

      if( ulStart <= ulLen )
      {
         if( pulCount && ( *pulCount <= ulLen - ulStart ) )
            ulCount = *pulCount;
         else
            ulCount = ulLen - ulStart + 1;

         if( ulStart + ulCount > ulLen )             /* check range */
            ulCount = ulLen - ulStart + 1;

         for( ulStart--; ulCount > 0; ulCount--, ulStart++ )
         {
            PHB_ITEM pItem = pBaseArray->pItems + ulStart;

            hb_vmPushSymbol( &hb_symEval );
            hb_vmPush( bBlock );
            hb_vmPush( pItem );
            hb_vmPushNumber( ( double ) ( ulStart + 1 ), 0 );
            hb_vmDo( 2 );
         }
      }

      return TRUE;
   }
   else
      return FALSE;
}

BOOL hb_arrayRelease( PHB_ITEM pArray )
{
   HB_TRACE(HB_TR_DEBUG, ("hb_arrayRelease(%p)", pArray));

   if( HB_IS_ARRAY( pArray ) )
   {
      PHB_BASEARRAY pBaseArray = pArray->item.asArray.value;
      ULONG ulLen = pBaseArray->ulLen;
      ULONG ulPos;

      /* clear object tree as needed */
      if( pBaseArray->uiClass && pBaseArray->puiClsTree )
      {
        hb_xfree(pBaseArray->puiClsTree);
        pBaseArray->puiClsTree = NULL;
      }

      if( pBaseArray->pItems )
      {
         for( ulPos = 0; ulPos < ulLen; ulPos++ )
            hb_itemClear( pBaseArray->pItems + ulPos );
         hb_xfree( pBaseArray->pItems );
      }

      hb_gcFree( ( void * ) pBaseArray );

      pArray->type = HB_IT_NIL;
      pArray->item.asArray.value = NULL;

      return TRUE;
   }
   else
      return FALSE;
}

/* NOTE: CA-Cl*pper 5.3a has a fix for the case when the starting position
         is greater than the length of the array. [vszakats] */

BOOL hb_arrayCopy( PHB_ITEM pSrcArray, PHB_ITEM pDstArray, ULONG * pulStart,
                   ULONG * pulCount, ULONG * pulTarget )
{
   HB_TRACE(HB_TR_DEBUG, ("hb_arrayCopy(%p, %p, %p, %p, %p)", pSrcArray, pDstArray, pulStart, pulCount, pulTarget));

   if( HB_IS_ARRAY( pSrcArray ) && HB_IS_ARRAY( pDstArray ) )
   {
      PHB_BASEARRAY pSrcBaseArray = pSrcArray->item.asArray.value;
      PHB_BASEARRAY pDstBaseArray = pDstArray->item.asArray.value;
      ULONG ulSrcLen = pSrcBaseArray->ulLen;
      ULONG ulDstLen = pDstBaseArray->ulLen;
      ULONG ulStart;
      ULONG ulCount;
      ULONG ulTarget;

      if( pulStart && ( *pulStart >= 1 ) )
         ulStart = *pulStart;
      else
         ulStart = 1;

      if( pulTarget && ( *pulTarget >= 1 ) )
         ulTarget = *pulTarget;
      else
         ulTarget = 1;

#ifdef HB_COMPAT_C53 /* From CA-Cl*pper 5.3a */
      if( ulStart <= ulSrcLen )
#else
      if( ulSrcLen > 0 )
#endif
      {
#ifndef HB_COMPAT_C53 /* From CA-Cl*pper 5.3a */
         if( ulStart > ulSrcLen )
            ulStart = ulSrcLen;
#endif
         if( pulCount && ( *pulCount <= ulSrcLen - ulStart ) )
            ulCount = *pulCount;
         else
            ulCount = ulSrcLen - ulStart + 1;

/* This is probably a bug, present in all versions of CA-Cl*pper. */
#ifdef HB_FIX_ACOPY_BUG
         if( ulTarget <= ulDstLen )
         {
#else
         if( ulDstLen > 0 )
         {
            if( ulTarget > ulDstLen )
               ulTarget = ulDstLen;
#endif

            if( ulCount > ulDstLen - ulTarget )
               ulCount = ulDstLen - ulTarget + 1;

            for( ulTarget--, ulStart--; ulCount > 0; ulCount--, ulStart++, ulTarget++ )
               hb_itemCopy( pDstBaseArray->pItems + ulTarget, pSrcBaseArray->pItems + ulStart );
         }
      }

      return TRUE;
   }
   else
      return FALSE;
}

PHB_ITEM hb_arrayClone( PHB_ITEM pSrcArray, PHB_NESTED_CLONED pClonedList )
{
   HB_TRACE(HB_TR_DEBUG, ("hb_arrayClone(%p, %p)", pSrcArray, pClonedList));

   if( HB_IS_ARRAY( pSrcArray ) )
   {
      PHB_BASEARRAY pSrcBaseArray = pSrcArray->item.asArray.value;
      PHB_BASEARRAY pDstBaseArray;
      ULONG ulSrcLen = pSrcBaseArray->ulLen;
      ULONG ulCount;
      PHB_ITEM pDstArray;
      PHB_NESTED_CLONED pCloned;
      BOOL bTop;

      pDstArray = hb_itemNew( NULL );
      hb_arrayNew( pDstArray, ulSrcLen );

      if( pClonedList == NULL )
      {
         bTop = TRUE;

         pClonedList = ( PHB_NESTED_CLONED ) hb_xgrab( sizeof( HB_NESTED_CLONED ) );

         pCloned = pClonedList;

         pCloned->pSrcBaseArray = pSrcBaseArray;
         pCloned->pDest         = pDstArray;

         pCloned->pNext = NULL;
      }
      else
      {
         bTop = FALSE;

         pCloned = pClonedList;
         while( pCloned->pNext )
         {
            pCloned = pCloned->pNext;
         }

         pCloned->pNext = ( PHB_NESTED_CLONED ) hb_xgrab( sizeof( HB_NESTED_CLONED ) );
         pCloned = pCloned->pNext;

         pCloned->pSrcBaseArray = pSrcBaseArray;
         pCloned->pDest         = pDstArray;

         pCloned->pNext = NULL;
      }

      pDstBaseArray = pDstArray->item.asArray.value;
      pDstBaseArray->uiClass = pSrcBaseArray->uiClass;

      /*-----------------12/20/2001 7:05PM----------------
       * TODO: Is this correct? was taken from __objClone()
       * was token from xHarbour
       * --------------------------------------------------*/
      pDstBaseArray->puiClsTree = NULL;

      for( ulCount = 0; ulCount < ulSrcLen; ulCount++ )
      {
         PHB_ITEM pSrcItem = pSrcBaseArray->pItems + ulCount;

         if( pSrcItem->type == HB_IT_ARRAY )
         {
            PHB_ITEM pClone;

            /* Broken down like this to avoid redundant comparisons. */
            pCloned = pClonedList;
            if( pCloned->pSrcBaseArray == pSrcItem->item.asArray.value )
            {
               pClone = pCloned->pDest;
               goto DontClone;
            }
            while( pCloned->pNext )
            {
               pCloned = pCloned->pNext;

               if( pCloned->pSrcBaseArray == pSrcItem->item.asArray.value )
               {
                  pClone = pCloned->pDest;
                  goto DontClone;
               }
            }
            if( pCloned->pSrcBaseArray == pSrcItem->item.asArray.value )
            {
               pClone = pCloned->pDest;
               goto DontClone;
            }

            pClone = hb_arrayClone( pSrcItem, pClonedList );

           DontClone :
            hb_arraySet( pDstArray, ulCount + 1, pClone );
         }
         else
         {
            hb_arraySet( pDstArray, ulCount + 1, pSrcItem );
         }
      }

      /* Top Level - Release the created list. */
      if( bTop )
      {
         /* 1st. Chain alway exists, and points to our top level pDstArray, which should NOT be (item) released. */
         pCloned     = pClonedList;
         pClonedList = pClonedList->pNext;
         hb_xfree( pCloned );

         while( pClonedList )
         {
            pCloned     = pClonedList;
            pClonedList = pClonedList->pNext;

            hb_itemRelease( pCloned->pDest );
            hb_xfree( pCloned );
         }
      }

      return pDstArray;
   }
   else
   {
      return hb_itemNew( NULL );
   }
}

PHB_ITEM hb_arrayFromStack( USHORT uiLen )
{
   PHB_ITEM pArray = hb_itemNew( NULL );
   PHB_BASEARRAY pBaseArray = ( PHB_BASEARRAY ) hb_gcAlloc( sizeof( HB_BASEARRAY ), hb_arrayReleaseGarbage );
   USHORT uiPos;

   HB_TRACE(HB_TR_DEBUG, ("hb_arrayFromStack(%iu)", uiLen));

   pArray->type = HB_IT_ARRAY;

   if( uiLen > 0 )
      pBaseArray->pItems = ( PHB_ITEM ) hb_xgrab( sizeof( HB_ITEM ) * uiLen );
   else
      pBaseArray->pItems = NULL;

   pBaseArray->ulLen      = uiLen;
   pBaseArray->uiHolders  = 1;
   pBaseArray->uiClass    = 0;
   pBaseArray->uiPrevCls  = 0;

   for( uiPos = 0; uiPos < uiLen; uiPos++ )
   {
      hb_itemCopy( pBaseArray->pItems + uiPos, hb_stackItemFromTop( uiPos - uiLen ) );
   }

   pArray->item.asArray.value = pBaseArray;

   return pArray;
}

PHB_ITEM hb_arrayFromParams( PHB_ITEM *pBase )
{
   PHB_ITEM pArray = hb_itemNew( NULL );
   PHB_BASEARRAY pBaseArray = ( PHB_BASEARRAY ) hb_gcAlloc( sizeof( HB_BASEARRAY ), hb_arrayReleaseGarbage );
   USHORT uiPos, uiPCount = (* pBase)->item.asSymbol.paramcnt;

   HB_TRACE(HB_TR_DEBUG, ("hb_arrayFromParams()"));

   pArray->type = HB_IT_ARRAY;

   if( uiPCount > 0 )
      pBaseArray->pItems = ( PHB_ITEM ) hb_xgrab( sizeof( HB_ITEM ) * uiPCount );
   else
      pBaseArray->pItems = NULL;

   pBaseArray->ulLen      = uiPCount;
   pBaseArray->uiHolders  = 1;
   pBaseArray->uiClass    = 0;
   pBaseArray->uiPrevCls  = 0;

   for( uiPos = 0; uiPos < uiPCount; uiPos++ )
   {
      hb_itemCopy( pBaseArray->pItems + uiPos, *( pBase + uiPos + 2 ) );
   }

   pArray->item.asArray.value = pBaseArray;

   return pArray;
}

/* This releases array when called from the garbage collector */
HB_GARBAGE_FUNC( hb_arrayReleaseGarbage )
{
   PHB_BASEARRAY pBaseArray = ( PHB_BASEARRAY ) Cargo;

   if( pBaseArray->pItems )
   {
      HB_ITEM_PTR pItem = pBaseArray->pItems;
      ULONG ulLen = pBaseArray->ulLen;

      while( ulLen-- )
      {
         /* Only strings should be deallocated.
          * Arrays, objects and codeblock should be released directly by
          * the garbage collector
          */
         if( HB_IS_STRING( pItem ) )
            hb_itemClear( pItem );

         ++pItem;
      }
      hb_xfree( pBaseArray->pItems );
      pBaseArray->pItems = NULL;
      pBaseArray->ulLen  = 0;
   }
}
