/*
 * $Id$
 */

/*
 * SQL MIX (Memory Index) Database Driver
 *
 * Copyright 2007 Mindaugas Kavaliauskas <dbtopas at dbtopas.lt>
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
#include "hbinit.h"
#include "hbapiitm.h"
#include "hbapirdd.h"
#include "hbapierr.h"
#include "hbdbferr.h"
#include "hbapilng.h"
#include "hbdate.h"
#include "hbset.h"
#include "hbvm.h"
#include "rddsys.ch"

#include "hbrddsql.h"

#define SUPERTABLE ( &sqlmixSuper )



#define MIX_KEY( tag, node, index ) \
   ( (PMIXKEY) & ( (BYTE*) (node) ) [ ( (node)->Leaf ? sizeof( MIXNODELEAF ) : sizeof( MIXNODE ) ) + (index) * (tag)->uiTotalLen ] )

#define MIX_COPY_KEYS_INTERNAL( tag, node, dst, src, count ) \
   memmove( ( (BYTE*) (node) ) + ( (node)->Leaf ? sizeof( MIXNODELEAF ) : sizeof( MIXNODE ) ) + (dst) * (tag)->uiTotalLen, \
            ( (BYTE*) (node) ) + ( (node)->Leaf ? sizeof( MIXNODELEAF ) : sizeof( MIXNODE ) ) + (src) * (tag)->uiTotalLen, \
            (count) * (tag)->uiTotalLen )

#define MIX_COPY_KEYS_EXTERNAL( tag, ndst, dst, nsrc, src, count ) \
   memmove( ( (BYTE*) (ndst) ) + ( (ndst)->Leaf ? sizeof( MIXNODELEAF ) : sizeof( MIXNODE ) ) + (dst) * (tag)->uiTotalLen, \
            ( (BYTE*) (nsrc) ) + ( (nsrc)->Leaf ? sizeof( MIXNODELEAF ) : sizeof( MIXNODE ) ) + (src) * (tag)->uiTotalLen, \
            (count) * (tag)->uiTotalLen )

#define MIX_ASSIGN_KEY( tag, node, dst, src ) \
   memmove( ( (BYTE*) (node) ) + ( (node)->Leaf ? sizeof( MIXNODELEAF ) : sizeof( MIXNODE ) ) + (dst) * (tag)->uiTotalLen, \
            (src), (tag)->uiTotalLen )

#define MIX_COPY_CHILDS_INTERNAL( tag, node, dst, src, count ) \
   memmove( & ((node)->Child[ (dst) ]), & ((node)->Child[ (src) ]), (count) * sizeof( void* ) )

#define MIX_COPY_CHILDS_EXTERNAL( tag, ndst, dst, nsrc, src, count ) \
   memmove( & ((ndst)->Child[ (dst) ]), & ((nsrc)->Child[ (src) ]), (count) * sizeof( void* ) )



static USHORT s_uiRddIdSQLMIX = ( USHORT ) -1;

static RDDFUNCS sqlmixSuper;


/*
=======================================================================
  Misc functions
=======================================================================
*/


static HB_ERRCODE sqlmixErrorRT( SQLMIXAREAP pArea, USHORT uiGenCode, USHORT uiSubCode, char* filename, USHORT uiOsCode, USHORT uiFlags )
{
   PHB_ITEM pError;
   HB_ERRCODE iRet = HB_FAILURE;

   if ( hb_vmRequestQuery() == 0 )
   {
      pError = hb_errNew();
      hb_errPutGenCode( pError, uiGenCode );
      hb_errPutSubCode( pError, uiSubCode );
      hb_errPutOsCode( pError, uiOsCode );
      hb_errPutDescription( pError, hb_langDGetErrorDesc( uiGenCode ) );
      if ( filename )
         hb_errPutFileName( pError, filename );
      if ( uiFlags )
         hb_errPutFlags( pError, uiFlags );
      iRet = SELF_ERROR( (AREAP) pArea, pError );
      hb_errRelease( pError );
   }
   return iRet;
}


/*
=======================================================================
  Memory Index
=======================================================================
*/

/* -------------------------- Key management ---------------------------- */
/* hb_mixKey*() */

static PMIXKEY hb_mixKeyNew( PMIXTAG pTag )
{
   return (PMIXKEY) hb_xgrab( pTag->uiTotalLen );
}


static PMIXKEY hb_mixKeyPutItem( PMIXKEY pKey, PHB_ITEM pItem, ULONG ulRecNo, PMIXTAG pTag )
{
   ULONG       ul;
   double      dbl;
   BYTE        buf[ 8 ];

   if ( ! pKey )
      pKey = hb_mixKeyNew( pTag );

   pKey->rec = ulRecNo;
   pKey->notnul = 1;

   /* TODO: check valtype */
   switch ( pTag->bType )
   {
      case 'C':
         ul = hb_itemGetCLen( pItem );

         if ( ul > (ULONG) pTag->uiKeyLen )
            ul = pTag->uiKeyLen;

         memcpy( pKey->val, hb_itemGetCPtr( pItem ), ul );

         if ( ul < (ULONG) pTag->uiKeyLen )
            memset( pKey->val + ul, ' ', (ULONG) pTag->uiKeyLen - ul );

         break;

      case 'N':
         dbl = hb_itemGetND( pItem );
         HB_DBL2ORD( &dbl, buf );
         memcpy( pKey->val, buf, 8 );
         break;

      case 'D':
         dbl = (double) hb_itemGetDL( pItem );
         HB_DBL2ORD( &dbl, buf );
         memcpy( pKey->val, buf, 8 );
         break;

      case 'L':
         pKey->val[ 0 ] = ( BYTE ) ( hb_itemGetL( pItem ) ? 'T' : 'F' );
         break;

      default:
         pKey->notnul = 0;
         memset( pKey->val, ' ', pTag->uiKeyLen );
   }
   return pKey;
}


static PMIXKEY hb_mixKeyEval( PMIXKEY pKey, PMIXTAG pTag )
{
   PHB_ITEM      pItem;
   SQLMIXAREAP   pArea = pTag->pArea;
   int           iCurrArea = hb_rddGetCurrentWorkAreaNumber();
   PHB_CODEPAGE  pCodepage = hb_cdpSelect( pArea->sqlarea.area.cdPage );

   if ( iCurrArea != pArea->sqlarea.area.uiArea )
      hb_rddSelectWorkAreaNumber( pArea->sqlarea.area.uiArea );
   else
      iCurrArea = 0;

   pItem = hb_vmEvalBlockOrMacro( pTag->pKeyItem );

   pKey = hb_mixKeyPutItem( pKey, pItem, pArea->sqlarea.ulRecNo, pTag );

   if ( iCurrArea )
      hb_rddSelectWorkAreaNumber( iCurrArea );

   hb_cdpSelect( pCodepage );

   return pKey;
}


static BOOL hb_mixEvalCond( SQLMIXAREAP pArea, PHB_ITEM pCondItem )
{
   int iCurrArea = 0;
   BOOL fRet;

   if ( pArea )
   {
      iCurrArea = hb_rddGetCurrentWorkAreaNumber();

      if ( iCurrArea != pArea->sqlarea.area.uiArea )
         hb_rddSelectWorkAreaNumber( pArea->sqlarea.area.uiArea );
      else
         iCurrArea = 0;
   }

   fRet = hb_itemGetL( hb_vmEvalBlockOrMacro( pCondItem ) );

   if ( iCurrArea )
      hb_rddSelectWorkAreaNumber( iCurrArea );

   return fRet;
}


static void hb_mixKeyFree( PMIXKEY pKey )
{
   hb_xfree( pKey );
}


static int hb_mixKeyCompare( PMIXTAG pTag, PMIXKEY pKey1, PMIXKEY pKey2, unsigned int uiLen )
{
   unsigned int  uiSize;
   int    i;

   if ( ! pKey1->notnul || ! pKey2->notnul )
   {
      return (int) pKey1->notnul - (int) pKey2->notnul;
   }

   i = 0;
   uiSize = pTag->uiKeyLen > uiLen ? uiLen : pTag->uiKeyLen;

   if ( pTag->pCodepage )
   {
      i = hb_cdpcmp( ( const char * ) pKey1->val, ( ULONG ) uiSize, ( const char * ) pKey2->val, ( ULONG ) uiSize, pTag->pCodepage, 0 );
   }
   else
   {
      if ( uiSize > 0 )
         i = memcmp( pKey1->val, pKey2->val, uiSize );
   }

   if ( i == 0 )
   {
      if ( pKey2->rec == (ULONG) -1 )
      {
         /* This condition seems inverted, but it's ok for seek last */
         if ( pTag->uiKeyLen > uiLen )
            i = -1;
      }
      else
      {
         if ( pTag->uiKeyLen > uiLen )
            i = 1;
         else if ( pTag->uiKeyLen < uiLen )
            i = -1;
      }
   }


   if ( i != 0 )
   {
      if ( i < 0 )
      {
         return -2;
      }
      return 2;
   }

   if ( pKey1->rec < pKey2->rec )
   {
      return -1;
   }
   else if ( pKey1->rec > pKey2->rec )
   {
      return 1;
   }
   return 0;
}


/* -------------------------- Tag management ---------------------------- */
/* hb_mixTag*() */

/* This function is used for debugging purposes. Uncomment it, if you need it.
static void hb_mixTagPrintNode( PMIXTAG pTag, PMIXNODE pNode, int iLevel )
{
   unsigned int  i;

   if ( !pNode )
      return;

   if ( pNode->KeyCount < MIX_NODE_ORDER / 2 && pNode->Parent )
   {
      printf("!!! Too few keys\n");
   }

   for ( i = 0; i < pNode->KeyCount; i++ )
   {
      if ( ! pNode->Leaf )
      {
         if ( pNode->Child[ i ]->Parent != pNode )
            printf("!!! Invalid parent\n");

         hb_mixTagPrintNode( pTag, pNode->Child[ i ], iLevel + 1 );
      }
      printf("%*ld %*s\n", iLevel * 10 + 5, MIX_KEY( pTag, pNode, i )->rec, pTag->uiKeyLen,
             MIX_KEY( pTag, pNode, i )->notnul ? ( char * ) MIX_KEY( pTag, pNode, i )->val : "NULL" );
   }

   if ( ! pNode->Leaf )
   {
      if ( pNode->Child[ pNode->KeyCount ]->Parent != pNode )
         printf("!!! Invalid parent\n");

      hb_mixTagPrintNode( pTag, pNode->Child[ pNode->KeyCount ], iLevel + 1 );
   }
}
*/

static PMIXNODE hb_mixTagCreateNode( PMIXTAG pTag, BOOL fLeaf )
{
   PMIXNODE    pNode;
   ULONG       ulSize;

   ulSize = ( fLeaf ? sizeof( MIXNODELEAF ) : sizeof( MIXNODE ) ) + MIX_NODE_ORDER * pTag->uiTotalLen;

   pNode = (PMIXNODE) hb_xgrab( ulSize );
   memset( pNode, 0, ulSize );
   pNode->Leaf = fLeaf ? 1 : 0;
   return pNode;
}


static unsigned int hb_mixTagNodeParentIndex( PMIXNODE pNode )
{
   PMIXNODE  pParent = pNode->Parent;
   unsigned int  ui;

   /* Find position in the parent node */
   ui = pParent->KeyCount;
   do {
      if ( pParent->Child[ ui ] == pNode )
         return ui;
   } while ( ui-- );

   return ( unsigned int ) -1;
}


static int hb_mixTagFindKey( PMIXTAG pTag, PMIXKEY pKey, unsigned int uiLen, PMIXNODE* ppNode, unsigned int* puiPos, BOOL fValidKey )
{
   PMIXNODE      pNode;
   unsigned int  ui;
   int           i;

   pNode = pTag->Root;

   while ( 1 )
   {
      i = -2;

      /* TODO: binary search */
      for ( ui = 0; ui < pNode->KeyCount; ui++ )
      {
         i = hb_mixKeyCompare( pTag, MIX_KEY( pTag, pNode, ui ), pKey, uiLen );

         if ( i >= 0 )
         {
            break;
         }
      }

      if ( i == 0 || pNode->Leaf )
         break;
      else
         pNode = pNode->Child[ ui ];
   }

   if ( fValidKey && ui >= pNode->KeyCount )
   {
      /* unsuccessful find always finds position in leaf */

      while ( pNode->Parent && pNode->Parent->Child[ pNode->Parent->KeyCount ] == pNode )
         pNode = pNode->Parent;

      if ( pNode->Parent )
      {
         for ( ui = 0; ui < pNode->Parent->KeyCount; ui++ )
         {
            if ( pNode->Parent->Child[ ui ] == pNode )
            {
               pNode = pNode->Parent;
               break;
            }
         }
      }
      else
      {
         ui = pNode->KeyCount + 1;   /* EOF */
      }
   }

   * ppNode = pNode;
   * puiPos = ui;
   return i;
}


static void hb_mixTagSetCurrent( PMIXTAG pTag, PMIXNODE pNode, unsigned int uiPos )
{
   if ( uiPos < pNode->KeyCount )
   {
      pTag->CurNode = pNode;
      pTag->CurPos = uiPos;
      pTag->CurKey = MIX_KEY( pTag, pNode, uiPos );
      pTag->fEof = FALSE;
   }
   else
   {
      pTag->fEof = TRUE;
   }
}


static BOOL hb_mixTagRefreshKey( PMIXTAG pTag )
{
   SQLMIXAREAP   pArea;

   pArea = pTag->pArea;

   if ( pArea->sqlarea.lpdbPendingRel )
      SELF_FORCEREL( ( AREAP ) pArea );

   if ( ! pArea->sqlarea.fPositioned )
   {
      pTag->fEof = TRUE;
      return FALSE;
   }
   else if ( pTag->fEof || pTag->CurKey->rec != pArea->sqlarea.ulRecNo )
   {
      PMIXKEY       pKey;
      PMIXNODE      pNode;
      unsigned int  ui;

      pKey = hb_mixKeyEval( NULL, pTag );

      hb_mixTagFindKey( pTag, pKey, pTag->uiKeyLen, &pNode, &ui, FALSE );
      hb_mixTagSetCurrent( pTag, pNode, ui );

      hb_mixKeyFree( pKey );
      return ! pTag->fEof && pTag->CurKey->rec == pArea->sqlarea.ulRecNo;
   }
   pTag->fBof = pTag->fEof = FALSE;
   return TRUE;
}


static void hb_mixTagAddKeyNode( PMIXTAG pTag, PMIXNODE pNode, unsigned int uiPos, PMIXKEY pKey, PMIXNODE pChildLeft, PMIXNODE pChildRight )
{
   MIX_COPY_KEYS_INTERNAL( pTag, pNode, uiPos + 1, uiPos, pNode->KeyCount - uiPos );
   if ( ! pNode->Leaf )
   {
      MIX_COPY_CHILDS_INTERNAL( pTag, pNode, uiPos + 2, uiPos + 1, pNode->KeyCount - uiPos );
      pNode->Child[ uiPos ] = pChildLeft;
      pNode->Child[ uiPos + 1 ] = pChildRight;
      pChildLeft->Parent = pNode;
      pChildRight->Parent = pNode;
   }
   MIX_ASSIGN_KEY( pTag, pNode, uiPos, pKey );
   pNode->KeyCount++;
}


static void hb_mixTagAddKeyPos( PMIXTAG pTag, PMIXNODE pNode, unsigned int uiPos, PMIXKEY pKey, PMIXNODE pChildLeft, PMIXNODE pChildRight )
{
   PMIXNODE      pNewNode;
   unsigned int  j, k;

   if ( pNode->KeyCount < MIX_NODE_ORDER )
   {
      hb_mixTagAddKeyNode( pTag, pNode, uiPos, pKey, pChildLeft, pChildRight );
      return;
   }

#ifdef USE_SIBLINGS
   /* Try use siblings, if leaf node is full */

   if ( pNode->Leaf && pNode->Parent )
   {
     j = hb_mixTagNodeParentIndex( pNode );

     if ( j > 0 && pNode->Parent->Child[ j - 1 ]->KeyCount < MIX_NODE_ORDER )
     {
        MIX_COPY_KEYS_EXTERNAL( pTag, pNode->Parent->Child[ j - 1 ], pNode->Parent->Child[ j - 1 ]->KeyCount, pNode->Parent, j - 1, 1 );
        pNode->Parent->Child[ j - 1 ]->KeyCount++;

        if ( uiPos == 0 )
        {
           MIX_ASSIGN_KEY( pTag, pNode->Parent, j - 1, pKey );
           pNode->Parent->Key[ j - 1 ] = pKey;
        }
        else
        {
           MIX_COPY_KEYS_EXTERNAL( pTag, pNode->Parent, j - 1, pNode, 0 , 1 );
           uiPos--;
           MIX_COPY_KEYS_INTERNAL( pTag, pNode, 0, 1, uiPos );
           MIX_ASSIGN_KEY( pTag, pNode, uiPos, pKey );
        }

        return;
     }
     else if ( j < pNode->Parent->KeyCount && pNode->Parent->Child[ j + 1 ]->KeyCount < MIX_NODE_ORDER )
     {
        MIX_COPY_KEYS_INTERNAL( pTag, pNode->Parent->Child[ j + 1 ], 1, 0, pNode->Parent->Child[ j + 1 ]->KeyCount );
        MIX_COPY_KEYS_EXTERNAL( pTag, pNode->Parent->Child[ j + 1 ], 0, pNode->Parent, j, 1 );
        pNode->Parent->Child[ j + 1 ]->KeyCount++;

        if ( uiPos == MIX_NODE_ORDER )
        {
           MIX_ASSIGN_KEY( pTag, pNode->Parent, j, pKey );
        }
        else
        {
           MIX_COPY_KEYS_EXTERNAL( pTag, pNode->Parent, j, pNode, MIX_NODE_ORDER - 1, 1 );
           MIX_COPY_KEYS_INTERNAL( pTag, pNode, uiPos + 1, uiPos, MIX_NODE_ORDER - uiPos - 1 );
           MIX_ASSIGN_KEY( pTag, pNode, uiPos, pKey );
        }
        return;
     }
   }
#endif /* USE_SIBLINGS */


   /* Create new node */
   pNewNode = hb_mixTagCreateNode( pTag, pNode->Leaf );

   /* Move half of items to new node */
   k = MIX_NODE_ORDER / 2 + ( ( uiPos <= MIX_NODE_ORDER / 2 ) ? 0 : 1 );
   MIX_COPY_KEYS_EXTERNAL( pTag, pNewNode, 0, pNode, k, MIX_NODE_ORDER - k );
   if ( ! pNode->Leaf )
   {
      MIX_COPY_CHILDS_EXTERNAL( pTag, pNewNode, 1, pNode, k + 1, MIX_NODE_ORDER - k );
      for ( j = 1; j <= MIX_NODE_ORDER - k; j++ )
      {
         pNewNode->Child[ j ]->Parent = pNewNode;  // Do NOT forget to re-parent
      }
   }
   pNode->KeyCount = k;
   pNewNode->KeyCount = MIX_NODE_ORDER - k;


   /* Insert new item to the left node or right node */
   if ( uiPos <= MIX_NODE_ORDER / 2 )
      hb_mixTagAddKeyNode( pTag, pNode, uiPos, pKey, pChildLeft, pChildRight );
   else
      hb_mixTagAddKeyNode( pTag, pNewNode, uiPos - MIX_NODE_ORDER / 2 - 1, pKey, pChildLeft, pChildRight );


   /* Assign the leftmost child of the new node */
   if ( ! pNode->Leaf )
   {
      pNewNode->Child[ 0 ] = pNode->Child[ pNode->KeyCount ];
      pNewNode->Child[ 0 ]->Parent = pNewNode;
   }

   pNode->KeyCount--;

   /* Move middle (last+1 in first node) item up */
   if ( pNode->Parent )
   {
      hb_mixTagAddKeyPos( pTag, pNode->Parent, hb_mixTagNodeParentIndex( pNode ),
                          MIX_KEY( pTag, pNode, pNode->KeyCount ), pNode, pNewNode );
   }
   else
   {
      pTag->Root = hb_mixTagCreateNode( pTag, 0 );
      hb_mixTagAddKeyNode( pTag, pTag->Root, 0, MIX_KEY( pTag, pNode, pNode->KeyCount ), pNode, pNewNode );
   }

}


static BOOL hb_mixTagAddKey( PMIXTAG pTag, PMIXKEY pKey )
{
   PMIXNODE      pNode;
   unsigned int  ui;
   int           i;

   i = hb_mixTagFindKey( pTag, pKey, pTag->uiKeyLen, &pNode, &ui, FALSE );

   /* Key can not be duplicated */
   if ( ! i )
      return FALSE;

   hb_mixTagAddKeyPos( pTag, pNode, ui, pKey, NULL, NULL );
   return TRUE;
}


static void hb_mixTagDelKeyNode( PMIXTAG pTag, PMIXNODE pNode, unsigned int uiPos )
{
   MIX_COPY_KEYS_INTERNAL( pTag, pNode, uiPos, uiPos + 1, pNode->KeyCount - uiPos - 1 );
   if ( ! pNode->Leaf )
   {
      MIX_COPY_CHILDS_INTERNAL( pTag, pNode, uiPos, uiPos + 1, pNode->KeyCount - uiPos );
   }
   pNode->KeyCount--;
}


static void hb_mixTagNodeAdjust( PMIXTAG pTag, PMIXNODE pNode )
{
   unsigned int  i, j;
   PMIXNODE      pParent, pSibling;


   while ( 1 )
   {
      if ( pNode->KeyCount >= MIX_NODE_ORDER / 2 )
         return;

      /* Check siblings */

      if ( pNode->Parent )
      {
         pParent = pNode->Parent;
         j = hb_mixTagNodeParentIndex( pNode );

         if ( j > 0 && pParent->Child[ j - 1 ]->KeyCount > MIX_NODE_ORDER / 2 )
         {
            /* Borrow from left */

            pSibling = pParent->Child[ j - 1 ];

            /* It could not be pNode->Child[ 0 ] if it is not Leaf!!! */
            hb_mixTagAddKeyNode( pTag, pNode, 0, MIX_KEY( pTag, pParent, j - 1 ), pSibling->Child[ pSibling->KeyCount ], pNode->Child[ 0 ] );
            MIX_COPY_KEYS_EXTERNAL( pTag, pParent, j - 1, pSibling, pSibling->KeyCount - 1, 1 );
            pSibling->KeyCount--;
            return;
         }
         else if ( j < pParent->KeyCount && pParent->Child[ j + 1 ]->KeyCount > MIX_NODE_ORDER / 2 )
         {
            /* Borrow from right */

            pSibling = pParent->Child[ j + 1 ];
            hb_mixTagAddKeyNode( pTag, pNode, pNode->KeyCount, MIX_KEY( pTag, pParent, j ), pNode->Child[ pNode->KeyCount ], pSibling->Child[ 0 ] );
            MIX_COPY_KEYS_EXTERNAL( pTag, pParent, j, pSibling, 0, 1 );
            hb_mixTagDelKeyNode( pTag, pSibling, 0 );
            return;
         }
         else if ( j > 0 )
         {
            /* Join with left */

            pSibling = pParent->Child[ j - 1 ];
            MIX_COPY_KEYS_EXTERNAL( pTag, pSibling, pSibling->KeyCount, pParent, j - 1, 1 );
            pSibling->KeyCount++;
            MIX_COPY_KEYS_EXTERNAL( pTag, pSibling, pSibling->KeyCount, pNode, 0, pNode->KeyCount );

            if ( pNode->Leaf )
            {
               pSibling->KeyCount += pNode->KeyCount;
            }
            else
            {
               MIX_COPY_CHILDS_EXTERNAL( pTag, pSibling, pSibling->KeyCount, pNode, 0, pNode->KeyCount );
               for ( i = 0; i < pNode->KeyCount; i++ )
               {
                  pSibling->Child[ pSibling->KeyCount++ ]->Parent = pSibling;
               }
               pSibling->Child[ pSibling->KeyCount ] = pNode->Child[ i ];
               pSibling->Child[ pSibling->KeyCount ]->Parent = pSibling;
            }
            hb_xfree( pNode );
            pParent->Child[ j ] = pSibling;
            hb_mixTagDelKeyNode( pTag, pParent, j - 1 );
            pNode = pParent;
         }
         else if ( j < pParent->KeyCount )
         {
            /* Join with right */

            pSibling = pParent->Child[ j + 1 ];
            MIX_COPY_KEYS_EXTERNAL( pTag, pNode, pNode->KeyCount, pParent, j, 1 );
            pNode->KeyCount++;
            MIX_COPY_KEYS_EXTERNAL( pTag, pNode, pNode->KeyCount, pSibling, 0, pSibling->KeyCount );
            if ( pNode->Leaf )
            {
               pNode->KeyCount += pSibling->KeyCount;
            }
            else
            {
               MIX_COPY_CHILDS_EXTERNAL( pTag, pNode, pNode->KeyCount, pSibling, 0, pSibling->KeyCount );
               for ( i = 0; i < pSibling->KeyCount; i++ )
               {
                  pNode->Child[ pNode->KeyCount++ ]->Parent = pNode;
               }
               pNode->Child[ pNode->KeyCount ] = pSibling->Child[ i ];
               pNode->Child[ pNode->KeyCount ]->Parent = pNode;
            }
            hb_xfree( pSibling );
            pParent->Child[ j + 1 ] = pNode;
            hb_mixTagDelKeyNode( pTag, pParent, j );
            pNode = pParent;
         }
      }
      else
      {
         /* Adjust root */

         if ( ! pNode->KeyCount && ! pNode->Leaf )
         {
            pTag->Root = pNode->Child[ 0 ];
            pTag->Root->Parent = NULL;
            hb_xfree( pNode );
         }
         return;
      }
   }
}


static void hb_mixTagDelKeyPos( PMIXTAG pTag, PMIXNODE pNode, unsigned int uiPos )
{
   if ( pNode->Leaf )
   {
      hb_mixTagDelKeyNode( pTag, pNode, uiPos );
      hb_mixTagNodeAdjust( pTag, pNode );
   }
   else
   {
      PMIXNODE   pLeaf;

      pLeaf = pNode->Child[ uiPos + 1 ];
      while ( ! pLeaf->Leaf )
      {
         pLeaf = pLeaf->Child[ 0 ];
      }
      MIX_COPY_KEYS_EXTERNAL( pTag, pNode, uiPos, pLeaf, 0, 1 );
      hb_mixTagDelKeyNode( pTag, pLeaf, 0 );
      hb_mixTagNodeAdjust( pTag, pLeaf );
   }
}


static BOOL hb_mixTagDelKey( PMIXTAG pTag, PMIXKEY pKey )
{
   PMIXNODE      pNode;
   unsigned int  ui;
   int           i;

   i = hb_mixTagFindKey( pTag, pKey, pTag->uiKeyLen, &pNode, &ui, FALSE );

   if ( i )
      return FALSE;

   hb_mixTagDelKeyPos( pTag, pNode, ui );
   return 1;
}


static PMIXTAG hb_mixTagCreate( const char* szTagName, PHB_ITEM pKeyExpr, PHB_ITEM pKeyItem, PHB_ITEM pForItem, PHB_ITEM pWhileItem, BYTE bType, unsigned int uiKeyLen, SQLMIXAREAP pArea )
{
   PMIXTAG            pTag;
   PMIXKEY            pKey = NULL;
   LPDBORDERCONDINFO  pOrdCondInfo = pArea->sqlarea.area.lpdbOrdCondInfo;
   ULONG              ulStartRec, ulNextCount = 0;
   LONG               lStep = 0;
   PHB_ITEM           pItem, pEvalItem = NULL;

   pTag = ( PMIXTAG ) hb_xgrab( sizeof( MIXTAG ) );
   memset( pTag, 0, sizeof( MIXTAG ) );

   pTag->pArea = pArea;

   pTag->szName   = ( char * ) hb_xgrab( MIX_MAXTAGNAMELEN + 1 );
   hb_strncpyUpperTrim( pTag->szName, szTagName, MIX_MAXTAGNAMELEN );

   pTag->szKeyExpr   = ( char * ) hb_xgrab( hb_itemGetCLen( pKeyExpr ) + 1 );
   hb_strncpyTrim( pTag->szKeyExpr, hb_itemGetCPtr( pKeyExpr ), hb_itemGetCLen( pKeyExpr ) );

   /* TODO: FOR expresion */
   pTag->szForExpr = NULL;

   pTag->pKeyItem = pKeyItem;
   pTag->pForItem = pForItem;
   pTag->bType = bType;
   pTag->uiKeyLen = uiKeyLen;

   pTag->uiTotalLen = sizeof( MIXKEY ) + pTag->uiKeyLen;

   /* Use national support */
   if ( bType == 'C' )
   {
      if( pArea->sqlarea.area.cdPage && pArea->sqlarea.area.cdPage->sort )
      {
         pTag->pCodepage = pArea->sqlarea.area.cdPage;
      }
   }

   pTag->Root = hb_mixTagCreateNode( pTag, TRUE );

   ulStartRec = 0;

   if ( pOrdCondInfo )
   {
      pEvalItem = pOrdCondInfo->itmCobEval;
      lStep = pOrdCondInfo->lStep;
   }

   if ( ! pOrdCondInfo || pOrdCondInfo->fAll )
   {
      pArea->pTag = NULL;
   }
   else
   {
      if ( pOrdCondInfo->itmRecID )
         ulStartRec = hb_itemGetNL( pOrdCondInfo->itmRecID );

      if ( ulStartRec )
      {
         ulNextCount = 1;
      }
      else if ( pOrdCondInfo->fRest || pOrdCondInfo->lNextCount > 0 )
      {
         if ( pOrdCondInfo->itmStartRecID )
            ulStartRec = hb_itemGetNL( pOrdCondInfo->itmStartRecID );

         if ( !ulStartRec )
            ulStartRec = pArea->sqlarea.ulRecNo;

         if ( pArea->sqlarea.area.lpdbOrdCondInfo->lNextCount > 0 )
            ulNextCount = pOrdCondInfo->lNextCount;
      }
      else if ( ! pOrdCondInfo->fUseCurrent )
      {
         pArea->pTag = NULL;
      }
   }

   if ( ulStartRec == 0 && pArea->pTag == NULL )
      ulStartRec = 1;

   if ( ulStartRec )
   {
      SELF_GOTO( (AREAP) pArea, ulStartRec );
   }
   else
   {
      SELF_GOTOP( (AREAP) pArea );
   }

   while ( ! pArea->sqlarea.area.fEof )
   {
      if ( pEvalItem )
      {
         if ( lStep >= pOrdCondInfo->lStep )
         {
            lStep = 0;
            if ( ! hb_mixEvalCond( NULL, pEvalItem ) )
               break;
         }
         ++lStep;
      }

      if ( pWhileItem && ! hb_mixEvalCond( NULL, pWhileItem ) )
      {
         break;
      }

      if ( pForItem == NULL || hb_mixEvalCond( NULL, pForItem ) )
      {
         pItem = hb_vmEvalBlockOrMacro( pKeyItem );

         pKey = hb_mixKeyPutItem( pKey, pItem, pArea->sqlarea.ulRecNo, pTag );
         hb_mixTagAddKey( pTag, pKey );
      }

      if ( ulNextCount )
      {
         ulNextCount--;
         if ( !ulNextCount )  break;
      }
      if ( SELF_SKIPRAW( ( AREAP ) pArea, 1 ) == HB_FAILURE )
         break;
   }
   if ( pKey )
      hb_mixKeyFree( pKey );

   return pTag;
}


static void hb_mixTagDestroyNode( PMIXNODE pNode )
{
   if ( ! pNode->Leaf )
   {
      unsigned int  ui;

      for ( ui = 0; ui <= pNode->KeyCount; ui++ )
          hb_mixTagDestroyNode( pNode->Child[ ui ] );
   }
   hb_xfree( pNode );
}


static void hb_mixTagDestroy( PMIXTAG pTag )
{
   if ( pTag->szName )
      hb_xfree( pTag->szName );

   if ( pTag->szKeyExpr )
      hb_xfree( pTag->szKeyExpr );

   if ( pTag->szForExpr )
      hb_xfree( pTag->szForExpr );

   if ( pTag->pKeyItem )
      hb_vmDestroyBlockOrMacro( pTag->pKeyItem );

   if ( pTag->pForItem )
      hb_vmDestroyBlockOrMacro( pTag->pForItem );

   if ( pTag->Root )
   {
      hb_mixTagDestroyNode( pTag->Root );
   }

   hb_xfree( pTag );
}


static void hb_mixTagGoTop( PMIXTAG pTag )
{
   PMIXNODE     pNode;

   pNode = pTag->Root;
   while ( ! pNode->Leaf )
   {
      pNode = pNode->Child[ 0 ];
   }

   if ( ! pNode->KeyCount )
   {
      pTag->fEof = TRUE;
      return;
   }
   pTag->fEof = FALSE;
   pTag->CurNode = pNode;
   pTag->CurPos = 0;
   pTag->CurKey = MIX_KEY( pTag, pTag->CurNode, 0 );
}


static void hb_mixTagGoBottom( PMIXTAG pTag )
{
   PMIXNODE     pNode;

   pNode = pTag->Root;
   while ( ! pNode->Leaf )
   {
      pNode = pNode->Child[ pNode->KeyCount ];
   }

   if ( ! pNode->KeyCount )
   {
      pTag->fEof = TRUE;
      return;
   }
   pTag->fEof = FALSE;
   pTag->CurNode = pNode;
   pTag->CurPos = pNode->KeyCount - 1;
   pTag->CurKey = MIX_KEY( pTag, pTag->CurNode, pTag->CurPos );
}


static void hb_mixTagSkip( PMIXTAG pTag, LONG lSkip )
{
   PMIXNODE      pNode, pNode2;
   unsigned int  uiPos, uiPos2;

   pNode = pTag->CurNode;
   uiPos = pTag->CurPos;

/*   printf("hb_mixTagSkip: CurNode=%p, CurPos=%d lSkip=%d\n", pNode, uiPos, lSkip ); */

   if ( lSkip > 0 )
   {
      pTag->fBof = FALSE;
      while ( ! pTag->fEof && lSkip > 0 )
      {
         if ( pNode->Leaf )
         {
            if ( (LONG) (pNode->KeyCount - 1 - uiPos) >= lSkip )
            {
               uiPos += lSkip;
               lSkip = 0;
            }
            else if ( pNode->KeyCount - 1 > uiPos )
            {
               lSkip -= (LONG) (pNode->KeyCount - 1 - uiPos);
               uiPos = pNode->KeyCount - 1;
            }
            if ( lSkip )
            {
               do {
                  if ( pNode->Parent )
                     uiPos = hb_mixTagNodeParentIndex( pNode );
                  pNode = pNode->Parent;
               } while ( pNode && uiPos == pNode->KeyCount );

               if ( pNode )
               {
                  lSkip--;
               }
               else
               {
                  pTag->fEof = TRUE;
               }
            }
         }
         else
         {
            pNode = pNode->Child[ uiPos + 1 ];
            while ( ! pNode->Leaf )
            {
               pNode = pNode->Child[ 0 ];
            }
            uiPos = 0;
            lSkip--;
         }
      }
   }
   else if ( lSkip < 0 )
   {
      lSkip = - lSkip;

/*
      This is not needed. skip(-1) from Eof is processed inside sqlmixSkipRaw
      if ( pTag->fEof )
      {
         hb_mixTagGoBottom( pTag );
         lSkip--;
         pTag->fBof = pTag->fEof;
      }
*/
      pTag->fBof = pTag->fEof;

      while ( ! pTag->fBof && lSkip > 0 )
      {
         if ( pNode->Leaf )
         {
            if ( (LONG) uiPos >= lSkip )
            {
               uiPos -= lSkip;
               lSkip = 0;
            }
            else if ( uiPos )
            {
               lSkip -= uiPos;
               uiPos = 0;
            }
            if ( lSkip )
            {
               pNode2 = pNode;
               uiPos2 = uiPos;
               do {
                  if ( pNode->Parent )
                     uiPos = hb_mixTagNodeParentIndex( pNode );
                  pNode = pNode->Parent;
               } while ( pNode && uiPos == 0 );

               if ( pNode )
               {
                  uiPos--;
                  lSkip--;
               }
               else
               {
                  pNode = pNode2;
                  uiPos = uiPos2;
                  pTag->fBof = TRUE;
               }
            }
         }
         else
         {
            do
            {
               pNode = pNode->Child[ uiPos ];
               uiPos = pNode->KeyCount;
            }
            while ( ! pNode->Leaf );
            uiPos--;
            lSkip--;
         }
      }
   }
   if ( ! pTag->fEof )
   {
      pTag->CurNode = pNode;
      pTag->CurPos = uiPos;
      pTag->CurKey = MIX_KEY( pTag, pNode, uiPos );
   }
}

/* -------------------------- Misc functions ---------------------------- */
/* hb_mix*() */

static PMIXTAG hb_mixFindTag( SQLMIXAREAP pArea, PHB_ITEM pOrder )
{
   PMIXTAG    pTag;

   if ( HB_IS_NUMBER( pOrder ) )
   {
      int   iOrder, iCurr = 0;

      iOrder = hb_itemGetNI( pOrder );

      pTag = pArea->pTagList;
      while ( pTag && iOrder != ++iCurr )
      {
         pTag = pTag->pNext;
      }
   }
   else
   {
      char  szTag[ MIX_MAXTAGNAMELEN + 1 ];

      hb_strncpyUpperTrim( szTag, hb_itemGetCPtr( pOrder ), MIX_MAXTAGNAMELEN );
      pTag = pArea->pTagList;
      while ( pTag && hb_stricmp( szTag, pTag->szName ) )
      {
         pTag = pTag->pNext;
      }
   }
   return pTag;
}


/*=======================================================================*/


static ULONG hb_mixTagNodeKeyCount( PMIXNODE pNode )
{
   ULONG         ulKeyCount;
   unsigned int  ui;

   ulKeyCount = pNode->KeyCount;
   if ( ! pNode->Leaf )
   {
      for ( ui = 0; ui <= pNode->KeyCount; ui++ )
          ulKeyCount += hb_mixTagNodeKeyCount( pNode->Child[ ui ] );
   }
   return ulKeyCount;
}


static BOOL hb_mixCheckRecordFilter( SQLMIXAREAP pArea, ULONG ulRecNo )
{
   BOOL lResult = FALSE;

   if ( pArea->sqlarea.area.dbfi.itmCobExpr || hb_setGetDeleted() )
   {
      if ( pArea->sqlarea.ulRecNo != ulRecNo || pArea->sqlarea.lpdbPendingRel )
         SELF_GOTO( ( AREAP ) pArea, ulRecNo );

      if ( hb_setGetDeleted() )
         SUPER_DELETED( ( AREAP ) pArea, &lResult );

      if ( !lResult && pArea->sqlarea.area.dbfi.itmCobExpr )
      {
         PHB_ITEM pResult = hb_vmEvalBlock( pArea->sqlarea.area.dbfi.itmCobExpr );
         lResult = HB_IS_LOGICAL( pResult ) && !hb_itemGetL( pResult );
      }
   }
   return !lResult;
}


static ULONG hb_mixDBOIKeyCount( PMIXTAG pTag, BOOL fFilter )
{
   ULONG ulKeyCount;

   if ( ! pTag )
      return 0;

   if ( fFilter && pTag->pArea->sqlarea.area.dbfi.fFilter )
   {
      PMIXNODE      pNode = pTag->CurNode;
      unsigned int  uiPos = pTag->CurPos;
      ULONG         ulRecNo = pTag->pArea->sqlarea.ulRecNo;

      ulKeyCount = 0;

      hb_mixTagGoTop( pTag );
      while ( ! pTag->fEof )
      {
         if ( hb_mixCheckRecordFilter( pTag->pArea, pTag->CurKey->rec ) )
            ulKeyCount++;
         hb_mixTagSkip( pTag, 1 );
      }
      hb_mixTagSetCurrent( pTag, pNode, uiPos );
      SELF_GOTO( (AREAP) pTag->pArea, ulRecNo );

   }
   else
   {
      ulKeyCount = hb_mixTagNodeKeyCount( pTag->Root );
   }
   return ulKeyCount;
}


static ULONG hb_mixDBOIKeyNo( PMIXTAG pTag, BOOL fFilter )
{
   ULONG ulKeyCount;

   if ( ! pTag )
      return 0;

   if ( fFilter )
      ulKeyCount = 0;
   else
   {
      PMIXNODE      pNode = pTag->CurNode;
      unsigned int  ui, uiPos = pTag->CurPos;

      ulKeyCount = 1;

      while ( pNode )
      {
         ulKeyCount += uiPos;
         if ( ! pNode->Leaf )
         {
            for ( ui = 0; ui < uiPos; ui++ )
               ulKeyCount += hb_mixTagNodeKeyCount( pNode->Child[ ui ] );
         }
         pNode = pNode->Parent;
         if ( pNode )
            uiPos = hb_mixTagNodeParentIndex( pNode );
      }
   }
   return ulKeyCount;
}


/*
=======================================================================
  SQLMIX RDD METHODS
=======================================================================
*/

static HB_ERRCODE sqlmixGoBottom( SQLMIXAREAP pArea )
{
   HB_ERRCODE retval;

   if ( SELF_GOCOLD( ( AREAP ) pArea ) == HB_FAILURE )
      return HB_FAILURE;

   if ( ! pArea->pTag )
      return SUPER_GOBOTTOM( ( AREAP ) pArea );

   if ( pArea->sqlarea.lpdbPendingRel && pArea->sqlarea.lpdbPendingRel->isScoped )
      SELF_FORCEREL( ( AREAP ) pArea );

   hb_mixTagGoBottom( pArea->pTag );

   pArea->sqlarea.area.fTop = FALSE;
   pArea->sqlarea.area.fBottom = TRUE;

   retval = SELF_GOTO( ( AREAP ) pArea, pArea->pTag->CurKey ? pArea->pTag->CurKey->rec : 0 );
   if ( retval != HB_FAILURE && pArea->sqlarea.fPositioned )
      retval = SELF_SKIPFILTER( ( AREAP ) pArea, -1 );

   return retval;
}


static HB_ERRCODE sqlmixGoTop( SQLMIXAREAP pArea )
{
   HB_ERRCODE retval;

   if ( SELF_GOCOLD( ( AREAP ) pArea ) == HB_FAILURE )
      return HB_FAILURE;

   if ( ! pArea->pTag )
      return SUPER_GOTOP( ( AREAP ) pArea );

   if ( pArea->sqlarea.lpdbPendingRel && pArea->sqlarea.lpdbPendingRel->isScoped )
      SELF_FORCEREL( ( AREAP ) pArea );

   hb_mixTagGoTop( pArea->pTag );

   pArea->sqlarea.area.fTop = TRUE;
   pArea->sqlarea.area.fBottom = FALSE;

   retval = SELF_GOTO( ( AREAP ) pArea, pArea->pTag->CurKey ? pArea->pTag->CurKey->rec : 0 );
   if ( retval != HB_FAILURE && pArea->sqlarea.fPositioned )
      retval = SELF_SKIPFILTER( ( AREAP ) pArea, -1 );

   return retval;
}


static HB_ERRCODE sqlmixSeek( SQLMIXAREAP pArea, BOOL fSoftSeek, PHB_ITEM pItem, BOOL fFindLast )
{
   if ( SELF_GOCOLD( ( AREAP ) pArea ) == HB_FAILURE )
      return HB_FAILURE;

   if ( ! pArea->pTag )
   {
      sqlmixErrorRT( pArea, EG_NOORDER, 1201, NULL, 0, EF_CANDEFAULT );
      return HB_FAILURE;
   }
   else
   {
      PMIXKEY   pKey;
      HB_ERRCODE   retval = HB_SUCCESS;
      BOOL      fEOF;
      PMIXTAG   pTag = pArea->pTag;
      PMIXNODE  pNode;
      unsigned int  uiKeyLen, ui;

      if ( pArea->sqlarea.lpdbPendingRel && pArea->sqlarea.lpdbPendingRel->isScoped )
         SELF_FORCEREL( ( AREAP ) pArea );

      pArea->sqlarea.area.fTop = pArea->sqlarea.area.fBottom = FALSE;
      pArea->sqlarea.area.fEof = FALSE;

      pKey = hb_mixKeyPutItem( NULL, pItem, fFindLast ? (ULONG) -1 : 0, pTag );

      uiKeyLen = pTag->uiKeyLen;
      if ( pTag->bType == 'C' )
      {
         uiKeyLen = ( unsigned int ) hb_itemGetCLen( pItem );
         if ( uiKeyLen > pTag->uiKeyLen )
            uiKeyLen = pTag->uiKeyLen;
      }

      hb_mixTagFindKey( pTag, pKey, uiKeyLen, &pNode, &ui, TRUE );
      hb_mixTagSetCurrent( pTag, pNode, ui );

      if ( fFindLast )
      {
         if ( pTag->fEof )
            hb_mixTagGoBottom( pTag );
         else
            hb_mixTagSkip( pTag, -1 );

         pArea->sqlarea.area.fFound = ! pTag->fEof && ( uiKeyLen == 0 || memcmp( pTag->CurKey->val, pKey->val, (ULONG) uiKeyLen ) == 0 );

         if ( ! pArea->sqlarea.area.fFound )
            hb_mixTagSetCurrent( pTag, pNode, ui );
      }
      else
      {
         pArea->sqlarea.area.fFound = ! pTag->fEof && ( uiKeyLen == 0 || memcmp( pTag->CurKey->val, pKey->val, (ULONG) uiKeyLen ) == 0 );
      }

      fEOF = pTag->fEof;

      if ( !fEOF )
      {
         retval = SELF_GOTO( ( AREAP ) pArea, pTag->CurKey->rec );
         if ( retval != HB_FAILURE && pArea->sqlarea.fPositioned )
         {
            retval = SELF_SKIPFILTER( ( AREAP ) pArea, fFindLast ? -1 : 1 );
            if ( retval != HB_FAILURE && pArea->sqlarea.fPositioned )
            {
               pArea->sqlarea.area.fFound = ( uiKeyLen == 0 || memcmp( pTag->CurKey->val, pKey->val, (ULONG) uiKeyLen ) == 0 );
               if ( ! pArea->sqlarea.area.fFound && ! fSoftSeek )
                  fEOF = TRUE;
            }
         }
      }
      if ( retval != HB_FAILURE && fEOF )
      {
         retval = SELF_GOTO( ( AREAP ) pArea, 0 );
      }
      pArea->sqlarea.area.fBof = FALSE;

      hb_mixKeyFree( pKey );
      return retval;
   }
}


static HB_ERRCODE sqlmixSkipRaw( SQLMIXAREAP pArea, LONG lToSkip )
{
   PMIXTAG    pTag = pArea->pTag;
   BOOL       fOut = FALSE;

   if ( SELF_GOCOLD( ( AREAP ) pArea ) == HB_FAILURE )
      return HB_FAILURE;

   if ( ! pTag || lToSkip == 0 )
      return SUPER_SKIPRAW( ( AREAP ) pArea, lToSkip );

   if ( pArea->sqlarea.lpdbPendingRel )
      SELF_FORCEREL( ( AREAP ) pArea );

   if ( ! hb_mixTagRefreshKey( pTag ) )
   {
      if ( lToSkip > 0 || pArea->sqlarea.fPositioned )
         fOut = TRUE;
      else
      {
         hb_mixTagGoBottom( pTag );
         fOut = pTag->fEof;
         lToSkip++;
      }
   }

   if ( ! fOut )
      hb_mixTagSkip( pTag, lToSkip );

   if ( SELF_GOTO( ( AREAP ) pArea, ( pTag->fEof || fOut ) ? 0 : pTag->CurKey->rec ) != HB_SUCCESS )
      return HB_FAILURE;
   pArea->sqlarea.area.fEof = pTag->fEof;
   pArea->sqlarea.area.fBof = pTag->fBof;
   return HB_SUCCESS;
}


static HB_ERRCODE sqlmixGoCold( SQLMIXAREAP pArea )
{
   BOOL fRecordChanged = pArea->sqlarea.fRecordChanged;
   BOOL fAppend = pArea->sqlarea.fAppend;

   if ( SUPER_GOCOLD( ( AREAP ) pArea ) == HB_FAILURE )
      return HB_FAILURE;

   if ( fRecordChanged && pArea->pTagList )
   {
      PMIXTAG      pTag;
      PMIXKEY      pKey = NULL;
      BOOL         fAdd, fDel;
      LPDBRELINFO  lpdbPendingRel;

      lpdbPendingRel = pArea->sqlarea.lpdbPendingRel;
      pArea->sqlarea.lpdbPendingRel = NULL;

      pTag = pArea->pTagList;
      while ( pTag )
      {

         if ( ! pTag->fCustom )
         {
            pKey = hb_mixKeyEval( pKey, pTag );

/*            printf( "sqlmixGoCold NEWKEY:%s rec:%d\n", pKey->val, pKey->rec); */

            if ( pTag->pForItem != NULL )
               fAdd = hb_mixEvalCond( pArea, pTag->pForItem );
            else
               fAdd = TRUE;

            if ( fAppend )
               fDel = FALSE;
            else
            {
               if ( hb_mixKeyCompare( pTag, pKey, pTag->HotKey, pTag->uiKeyLen ) == 0 )
               {
                  fDel = !fAdd &&  pTag->HotFor;
                  fAdd =  fAdd && !pTag->HotFor;
               }
               else
               {
                  fDel = pTag->HotFor;
               }
            }

            if ( fDel )
               hb_mixTagDelKey( pTag, pTag->HotKey );

            if ( fAdd )
               hb_mixTagAddKey( pTag, pKey );
         }
         pTag = pTag->pNext;
      }

      if ( pKey )
         hb_mixKeyFree( pKey );

      pArea->sqlarea.lpdbPendingRel = lpdbPendingRel;
   }

   return HB_SUCCESS;
}


static HB_ERRCODE sqlmixGoHot( SQLMIXAREAP pArea )
{
   PMIXTAG pTag;

/*
   if ( pArea->fRecordChanged )
      printf( "sqlmixGoHot: multiple marking buffer as hot." );
*/

   if ( SUPER_GOHOT( (AREAP) pArea ) == HB_FAILURE )
      return HB_FAILURE;

   pTag = pArea->pTagList;
   while ( pTag )
   {
      if ( !pTag->fCustom )
      {
         pTag->HotKey = hb_mixKeyEval( pTag->HotKey, pTag );
         pTag->HotFor = pTag->pForItem == NULL || hb_mixEvalCond( pArea, pTag->pForItem );
      }
      pTag = pTag->pNext;
   }
   return HB_SUCCESS;
}


static HB_ERRCODE sqlmixClose( SQLMIXAREAP pArea )
{
   if ( SELF_GOCOLD( (AREAP) pArea ) == HB_FAILURE )
      return HB_FAILURE;

   if ( SUPER_CLOSE( (AREAP) pArea ) == HB_FAILURE )
      return HB_FAILURE;

   if ( SELF_ORDLSTCLEAR( (AREAP) pArea ) == HB_FAILURE )
      return HB_FAILURE;

   return HB_SUCCESS;
}


static HB_ERRCODE sqlmixStructSize( SQLMIXAREAP pArea, USHORT* StructSize )
{
   HB_SYMBOL_UNUSED( pArea );

   * StructSize = sizeof( SQLMIXAREA );
   return HB_SUCCESS;
}


static HB_ERRCODE sqlmixOrderListClear( SQLMIXAREAP pArea )
{
   PMIXTAG    pTag;

   while ( pArea->pTagList )
   {
      pTag = pArea->pTagList;
      pArea->pTagList = pTag->pNext;
      hb_mixTagDestroy( pTag );
   }
   pArea->pTag = NULL;
   return HB_SUCCESS;
}


static HB_ERRCODE sqlmixOrderListFocus( SQLMIXAREAP pArea, LPDBORDERINFO pOrderInfo )
{
   if ( pArea->pTag )
   {
      pOrderInfo->itmResult = hb_itemPutC( pOrderInfo->itmResult, pArea->pTag->szName );
   }

   if ( pOrderInfo->itmOrder )
   {
      pArea->pTag = hb_mixFindTag( pArea, pOrderInfo->itmOrder );
   }
   return pArea->pTag ? HB_SUCCESS : HB_FAILURE;
}


static HB_ERRCODE sqlmixOrderCreate( SQLMIXAREAP pArea, LPDBORDERCREATEINFO pOrderInfo )
{
   PMIXTAG      pTagNew, pTag;
   PHB_ITEM     pKeyItem, pForItem = NULL, pWhileItem = NULL, pResult;
   ULONG        ulRecNo;
   USHORT       uiLen;
   BYTE         bType;

   /* Obtain key codeblock */
   if ( pOrderInfo->itmCobExpr )
   {
      pKeyItem = hb_itemNew( pOrderInfo->itmCobExpr );
   }
   else
   {
      if ( SELF_COMPILE( (AREAP) pArea, hb_itemGetCPtr( pOrderInfo->abExpr ) ) == HB_FAILURE )
         return HB_FAILURE;
      pKeyItem = pArea->sqlarea.area.valResult;
      pArea->sqlarea.area.valResult = NULL;
   }

   /* Test key codeblock on EOF */
   ulRecNo = pArea->sqlarea.ulRecNo;
   SELF_GOTO( (AREAP) pArea, 0 );
   if ( SELF_EVALBLOCK( (AREAP) pArea, pKeyItem ) == HB_FAILURE )
   {
      hb_vmDestroyBlockOrMacro( pKeyItem );
      SELF_GOTO( (AREAP) pArea, ulRecNo );
      return HB_FAILURE;
   }

   pResult = pArea->sqlarea.area.valResult;
   pArea->sqlarea.area.valResult = NULL;

   switch ( hb_itemType( pResult ) )
   {
      case HB_IT_STRING:
      case HB_IT_STRING | HB_IT_MEMO:
         bType = 'C';
         uiLen = (USHORT) hb_itemGetCLen( pResult );
         if ( uiLen > MIX_MAXKEYLEN )  uiLen = MIX_MAXKEYLEN;
         break;

      case HB_IT_INTEGER:
      case HB_IT_LONG:
      case HB_IT_DOUBLE:
         bType = 'N';
         uiLen = 8;
         break;

      case HB_IT_DATE:
         bType = 'D';
         uiLen = 8;
         break;

      case HB_IT_LOGICAL:
         bType = 'L';
         uiLen = 1;
         break;

      default:
         bType = 'U';
         uiLen = 0;
   }
   hb_itemRelease( pResult );

   if ( bType == 'U' || uiLen == 0 )
   {
      hb_vmDestroyBlockOrMacro( pKeyItem );
      SELF_GOTO( (AREAP) pArea, ulRecNo );
      sqlmixErrorRT( pArea, bType == 'U' ? EG_DATATYPE : EG_DATAWIDTH, 1026, NULL, 0, 0 );
      return HB_FAILURE;
   }

   if ( pArea->sqlarea.area.lpdbOrdCondInfo )
   {
      /* Obtain FOR codeblock */
      if ( pArea->sqlarea.area.lpdbOrdCondInfo->itmCobFor )
      {
         pForItem = hb_itemNew( pArea->sqlarea.area.lpdbOrdCondInfo->itmCobFor );
      }
      else if ( pArea->sqlarea.area.lpdbOrdCondInfo->abFor )
      {
         if ( SELF_COMPILE( (AREAP) pArea, pArea->sqlarea.area.lpdbOrdCondInfo->abFor ) == HB_FAILURE )
         {
            hb_vmDestroyBlockOrMacro( pKeyItem );
            SELF_GOTO( (AREAP) pArea, ulRecNo );
            return HB_FAILURE;
         }
         pForItem = pArea->sqlarea.area.valResult;
         pArea->sqlarea.area.valResult = NULL;
      }

      /* Obtain WHILE codeblock */
      if ( pArea->sqlarea.area.lpdbOrdCondInfo->itmCobWhile )
      {
         pWhileItem = hb_itemNew( pArea->sqlarea.area.lpdbOrdCondInfo->itmCobWhile );
      }
      else if ( pArea->sqlarea.area.lpdbOrdCondInfo->abWhile )
      {
         if ( SELF_COMPILE( (AREAP) pArea, pArea->sqlarea.area.lpdbOrdCondInfo->abWhile ) == HB_FAILURE )
         {
            hb_vmDestroyBlockOrMacro( pKeyItem );
            if ( pForItem )
               hb_vmDestroyBlockOrMacro( pForItem );
            SELF_GOTO( (AREAP) pArea, ulRecNo );
            return HB_FAILURE;
         }
         pWhileItem = pArea->sqlarea.area.valResult;
         pArea->sqlarea.area.valResult = NULL;
      }
   }

   /* Test FOR codeblock on EOF */
   if ( pForItem )
   {
      if ( SELF_EVALBLOCK( (AREAP) pArea, pForItem ) == HB_FAILURE )
      {
         hb_vmDestroyBlockOrMacro( pKeyItem );
         hb_vmDestroyBlockOrMacro( pForItem );
         if ( pWhileItem )
            hb_vmDestroyBlockOrMacro( pWhileItem );
         SELF_GOTO( (AREAP) pArea, ulRecNo );
         return HB_FAILURE;
      }
      if ( hb_itemType( pArea->sqlarea.area.valResult ) != HB_IT_LOGICAL )
      {
         hb_itemRelease( pArea->sqlarea.area.valResult );
         pArea->sqlarea.area.valResult = 0;
         hb_vmDestroyBlockOrMacro( pKeyItem );
         hb_vmDestroyBlockOrMacro( pForItem );
         if ( pWhileItem )
            hb_vmDestroyBlockOrMacro( pWhileItem );
         SELF_GOTO( (AREAP) pArea, ulRecNo );
         sqlmixErrorRT( pArea, EG_DATATYPE, EDBF_INVALIDFOR, NULL, 0, 0 );
         return HB_FAILURE;
      }
      hb_itemRelease( pArea->sqlarea.area.valResult );
      pArea->sqlarea.area.valResult = 0;
   }

   /* TODO: WHILE condition is not tested, like in DBFCDX. Why? Compatibility with Clipper? */

   SELF_GOTO( (AREAP) pArea, ulRecNo );

   pTagNew = hb_mixTagCreate( pOrderInfo->atomBagName, pOrderInfo->abExpr, pKeyItem, pForItem, pWhileItem, bType, uiLen, pArea );

   if ( pWhileItem )
      hb_vmDestroyBlockOrMacro( pWhileItem );

   /* Append the tag to the end of list */
   if ( pArea->pTagList )
   {
      pTag = pArea->pTagList;
      while ( pTag->pNext )
      {
         pTag = pTag->pNext;
      }
      pTag->pNext = pTagNew;
   }
   else
   {
      pArea->pTagList = pTagNew;
   }

   pArea->pTag = pTagNew;
   return HB_SUCCESS;
}


static HB_ERRCODE sqlmixOrderInfo( SQLMIXAREAP pArea, USHORT uiIndex, LPDBORDERINFO pOrderInfo )
{
   PMIXTAG   pTag;
   USHORT    uiTag = 0;


   switch( uiIndex )
   {
      case DBOI_EVALSTEP:
         pOrderInfo->itmResult = hb_itemPutNL( pOrderInfo->itmResult,
                  pArea->sqlarea.area.lpdbOrdCondInfo ? pArea->sqlarea.area.lpdbOrdCondInfo->lStep : 0 );
         return HB_SUCCESS;

      case DBOI_ORDERCOUNT:
         pTag = pArea->pTagList;
         while ( pTag )
         {
           pTag = pTag->pNext;
           uiTag++;
         }
         pOrderInfo->itmResult = hb_itemPutNI( pOrderInfo->itmResult, uiTag );
         return HB_SUCCESS;
   }


   if ( SELF_GOCOLD( ( AREAP ) pArea ) == HB_FAILURE )
      return HB_FAILURE;


   if ( pOrderInfo->itmOrder )
      pTag = hb_mixFindTag( pArea, pOrderInfo->itmOrder );
   else
      pTag = pArea->pTag;


   switch( uiIndex )
   {
      case DBOI_CONDITION:
         pOrderInfo->itmResult = hb_itemPutC( pOrderInfo->itmResult, ( pTag ? pTag->szForExpr : NULL ) );
         if ( pTag && pOrderInfo->itmNewVal && HB_IS_STRING( pOrderInfo->itmNewVal ) )
         {
            if ( pTag->szForExpr != NULL )
            {
               hb_xfree( pTag->szForExpr );
               pTag->szForExpr = NULL;
            }
            if ( pTag->pForItem != NULL )
            {
               hb_vmDestroyBlockOrMacro( pTag->pForItem );
               pTag->pForItem = NULL;
            }
            if ( hb_itemGetCLen( pOrderInfo->itmNewVal ) > 0 )
            {
               const char* pForExpr = hb_itemGetCPtr( pOrderInfo->itmNewVal );

               if ( SELF_COMPILE( (AREAP) pArea, pForExpr ) == HB_SUCCESS )
               {
                  PHB_ITEM   pForItem = pArea->sqlarea.area.valResult;

                  pArea->sqlarea.area.valResult = NULL;
                  if ( SELF_EVALBLOCK( (AREAP) pArea, pForItem ) == HB_SUCCESS )
                  {
                     if ( hb_itemType( pArea->sqlarea.area.valResult ) == HB_IT_LOGICAL )
                     {
                        pTag->szForExpr = hb_strdup( pForExpr );
                        pTag->pForItem = pForItem;
                        pForItem = NULL;
                     }
                     hb_itemRelease( pArea->sqlarea.area.valResult );
                     pArea->sqlarea.area.valResult = NULL;
                  }
                  if ( pForItem )
                     hb_vmDestroyBlockOrMacro( pForItem );
               }
            }
         }
         break;

      case DBOI_EXPRESSION:
         pOrderInfo->itmResult = hb_itemPutC( pOrderInfo->itmResult, pTag ? pTag->szKeyExpr : NULL );
         break;

      case DBOI_POSITION:
      case DBOI_KEYNORAW:
         if ( pOrderInfo->itmNewVal && HB_IS_NUMERIC( pOrderInfo->itmNewVal ) )
         {

/* TODO:
            pOrderInfo->itmResult = hb_itemPutL( pOrderInfo->itmResult,
                hb_cdxDBOIKeyGoto( pArea, pTag,
                   hb_itemGetNL( pOrderInfo->itmNewVal ), uiIndex == DBOI_POSITION ) == HB_SUCCESS );
*/
         }
         else
            pOrderInfo->itmResult = hb_itemPutNL( pOrderInfo->itmResult,
                                       hb_mixDBOIKeyNo( pTag, uiIndex == DBOI_POSITION ) );
         break;

      case DBOI_KEYCOUNT:
      case DBOI_KEYCOUNTRAW:
         pOrderInfo->itmResult = hb_itemPutNL( pOrderInfo->itmResult,
                                    hb_mixDBOIKeyCount( pTag, uiIndex == DBOI_KEYCOUNT ) );
         break;

/* TODO:
      case DBOI_FINDREC:
         pOrderInfo->itmResult = hb_itemPutL( pOrderInfo->itmResult,
                  hb_cdxDBOIFindRec( pArea, pTag,
                              hb_itemGetNL( pOrderInfo->itmNewVal ), FALSE ) );
         break;

      case DBOI_FINDRECCONT:
         pOrderInfo->itmResult = hb_itemPutL( pOrderInfo->itmResult,
                  hb_cdxDBOIFindRec( pArea, pTag,
                              hb_itemGetNL( pOrderInfo->itmNewVal ), TRUE ) );
         break;
*/
      case DBOI_NAME:
         pOrderInfo->itmResult = hb_itemPutC( pOrderInfo->itmResult, pTag ? pTag->szName : NULL );
         break;

      case DBOI_NUMBER:
         pOrderInfo->itmResult = hb_itemPutNI( pOrderInfo->itmResult, uiTag );  // kitaip
         break;

      case DBOI_ISCOND:
         pOrderInfo->itmResult = hb_itemPutL( pOrderInfo->itmResult, pTag && pTag->szForExpr != NULL );
         break;

      case DBOI_ISDESC:
      case DBOI_UNIQUE:
         pOrderInfo->itmResult = hb_itemPutL( pOrderInfo->itmResult, FALSE );
         break;

      case DBOI_SCOPETOP:
      case DBOI_SCOPEBOTTOM:
         if ( pOrderInfo->itmResult )
            hb_itemClear( pOrderInfo->itmResult );
         break;

      case DBOI_KEYTYPE:
         if ( pTag )
         {
            char   szType[ 2 ];

            szType[ 0 ] = ( char ) pTag->bType;
            szType[ 1 ] = 0;
            pOrderInfo->itmResult = hb_itemPutC( pOrderInfo->itmResult, szType );
         }
         else
            pOrderInfo->itmResult = hb_itemPutC( pOrderInfo->itmResult, NULL );
         break;

      case DBOI_KEYSIZE:
         pOrderInfo->itmResult = hb_itemPutNI( pOrderInfo->itmResult, pTag ? pTag->uiKeyLen : 0 );
         break;

      case DBOI_KEYDEC:
         pOrderInfo->itmResult = hb_itemPutNI( pOrderInfo->itmResult, 0 );
         break;

/* TODO:
      case DBOI_KEYVAL:
         hb_itemClear( pOrderInfo->itmResult );
         if ( pArea->sqlarea.lpdbPendingRel )
            SELF_FORCEREL( ( AREAP ) pArea );
         if ( pTag && pArea->sqlarea.fPositioned )
         {
            if ( pTag->CurKey->rec != pArea->sqlarea.ulRecNo )
            {
               hb_cdxIndexLockRead( pTag->pIndex );
               hb_cdxCurKeyRefresh( pArea, pTag );
               hb_cdxIndexUnLockRead( pTag->pIndex );
            }
            if ( pTag->CurKey->rec == pArea->sqlarea.ulRecNo )
               pOrderInfo->itmResult = hb_cdxKeyGetItem( pTag->CurKey,
                                           pOrderInfo->itmResult, pTag, TRUE );
         }
         break;

      case DBOI_CUSTOM:
         pOrderInfo->itmResult = hb_itemPutL( pOrderInfo->itmResult, ( pTag ? pTag->fCustom : FALSE ) );
         if ( pOrderInfo->itmNewVal && HB_IS_LOGICAL( pOrderInfo->itmNewVal )
                                    && hb_itemGetL( pOrderInfo->itmNewVal ) )
         {
            pTag->fCustom = TRUE;
         }
         break;

      case DBOI_KEYADD:
         if ( ! pTag )
         {
            pOrderInfo->itmResult = hb_itemPutL( pOrderInfo->itmResult, FALSE );
         }
         else
         {
            if ( pTag->fCustom )
            {
               if ( pArea->sqlarea.lpdbPendingRel )
                  SELF_FORCEREL( ( AREAP ) pArea );

               if ( ! pArea->sqlarea.fPositioned ||
                    ( pTag->pForItem &&
                      ! hb_cdxEvalCond( pArea, pTag->pForItem, TRUE ) ) )
               {
                  pOrderInfo->itmResult = hb_itemPutL( pOrderInfo->itmResult, FALSE );
               }
               else
               {
                  LPCDXKEY pKey;
                  hb_cdxIndexLockWrite( pTag->pIndex );
                  if ( pOrderInfo->itmNewVal && ! HB_IS_NIL( pOrderInfo->itmNewVal ) )
                     pKey = hb_cdxKeyPutItem( NULL, pOrderInfo->itmNewVal, pArea->sqlarea.ulRecNo, pTag, TRUE, TRUE );
                  else
                     pKey = hb_cdxKeyEval( NULL, pTag );
                  pOrderInfo->itmResult = hb_itemPutL( pOrderInfo->itmResult,
                                                hb_cdxTagKeyAdd( pTag, pKey ) );
                  hb_cdxIndexUnLockWrite( pTag->pIndex );
                  hb_cdxKeyFree( pKey );
               }
            }
            else
            {
               sqlmixErrorRT( pArea, 0, 1052, NULL, 0, 0 );
            }
         }
         break;

      case DBOI_KEYDELETE:
         if ( ! pTag )
         {
            pOrderInfo->itmResult = hb_itemPutL( pOrderInfo->itmResult, FALSE );
         }
         else
         {
            if ( pTag->Custom )
            {
               if ( pArea->sqlarea.lpdbPendingRel )
                  SELF_FORCEREL( ( AREAP ) pArea );

               if ( !pArea->sqlarea.fPositioned ||
                    ( pTag->pForItem &&
                      !hb_cdxEvalCond( pArea, pTag->pForItem, TRUE ) ) )
               {
                  pOrderInfo->itmResult = hb_itemPutL( pOrderInfo->itmResult, FALSE );
               }
               else
               {
                  LPCDXKEY pKey;
                  hb_cdxIndexLockWrite( pTag->pIndex );
                  if ( pOrderInfo->itmNewVal && !HB_IS_NIL( pOrderInfo->itmNewVal ) )
                     pKey = hb_cdxKeyPutItem( NULL, pOrderInfo->itmNewVal, pArea->sqlarea.ulRecNo, pTag, TRUE, TRUE );
                  else
                     pKey = hb_cdxKeyEval( NULL, pTag );
                  pOrderInfo->itmResult = hb_itemPutL( pOrderInfo->itmResult,
                                                hb_cdxTagKeyDel( pTag, pKey ) );
                  hb_cdxIndexUnLockWrite( pTag->pIndex );
                  hb_cdxKeyFree( pKey );
               }
            }
            else
            {
               sqlmixErrorRT( pArea, 0, 1052, NULL, 0, 0 );
            }
         }
         break;

*/
      case DBOI_SHARED:
      case DBOI_ISREADONLY:
         pOrderInfo->itmResult = hb_itemPutL( pOrderInfo->itmResult, FALSE );
         break;

      default:
         return SUPER_ORDINFO( ( AREAP ) pArea, uiIndex, pOrderInfo );

   }
   return HB_SUCCESS;
}


static HB_ERRCODE sqlmixExit( LPRDDNODE pRDD )
{
   /* This empty method is used to avoid duplicated sqlbase exit call */
   HB_SYMBOL_UNUSED( pRDD );
   return HB_SUCCESS;
}


static RDDFUNCS sqlmixTable =
       {( DBENTRYP_BP )   NULL,                   /* sqlmixBof */
        ( DBENTRYP_BP )   NULL,                   /* sqlmixEof */
        ( DBENTRYP_BP )   NULL,                   /* sqlmixFound */
        ( DBENTRYP_V )    sqlmixGoBottom,
        ( DBENTRYP_UL )   NULL,                   /* sqlmixGoTo */
        ( DBENTRYP_I )    NULL,                   /* sqlmixGoToId */
        ( DBENTRYP_V )    sqlmixGoTop,
        ( DBENTRYP_BIB )  sqlmixSeek,
        ( DBENTRYP_L )    NULL,                   /* sqlmixSkip */
        ( DBENTRYP_L )    NULL,                   /* sqlmixSkipFilter */
        ( DBENTRYP_L )    sqlmixSkipRaw,
        ( DBENTRYP_VF )   NULL,                   /* sqlmixAddField */
        ( DBENTRYP_B )    NULL,                   /* sqlmixAppend */
        ( DBENTRYP_I )    NULL,                   /* sqlmixCreateFields */
        ( DBENTRYP_V )    NULL,                   /* sqlmixDeleteRec */
        ( DBENTRYP_BP )   NULL,                   /* sqlmixDeleted */
        ( DBENTRYP_SP )   NULL,                   /* sqlmixFieldCount */
        ( DBENTRYP_VF )   NULL,                   /* sqlmixFieldDisplay */
        ( DBENTRYP_SSI )  NULL,                   /* sqlmixFieldInfo */
        ( DBENTRYP_SCP )  NULL,                   /* sqlmixFieldName */
        ( DBENTRYP_V )    NULL,                   /* sqlmixFlush */
        ( DBENTRYP_PP )   NULL,                   /* sqlmixGetRec */
        ( DBENTRYP_SI )   NULL,                   /* sqlmixGetValue */
        ( DBENTRYP_SVL )  NULL,                   /* sqlmixGetVarLen */
        ( DBENTRYP_V )    sqlmixGoCold,
        ( DBENTRYP_V )    sqlmixGoHot,
        ( DBENTRYP_P )    NULL,                   /* sqlmixPutRec */
        ( DBENTRYP_SI )   NULL,                   /* sqlmixPutValue */
        ( DBENTRYP_V )    NULL,                   /* sqlmixRecall */
        ( DBENTRYP_ULP )  NULL,                   /* sqlmixRecCount */
        ( DBENTRYP_ISI )  NULL,                   /* sqlmixRecInfo */
        ( DBENTRYP_ULP )  NULL,                   /* sqlmixRecNo */
        ( DBENTRYP_I )    NULL,                   /* sqlmixRecId */
        ( DBENTRYP_S )    NULL,                   /* sqlmixSetFieldExtent */
        ( DBENTRYP_CP )   NULL,                   /* sqlmixAlias */
        ( DBENTRYP_V )    sqlmixClose,
        ( DBENTRYP_VO )   NULL,                   /* sqlmixCreate */
        ( DBENTRYP_SI )   NULL,                   /* sqlmixInfo */
        ( DBENTRYP_V )    NULL,                   /* sqlmixNewArea */
        ( DBENTRYP_VO )   NULL,                   /* sqlmixOpen */
        ( DBENTRYP_V )    NULL,                   /* sqlmixRelease */
        ( DBENTRYP_SP )   sqlmixStructSize,
        ( DBENTRYP_CP )   NULL,                   /* sqlmixSysName */
        ( DBENTRYP_VEI )  NULL,                   /* sqlmixEval */
        ( DBENTRYP_V )    NULL,                   /* sqlmixPack */
        ( DBENTRYP_LSP )  NULL,                   /* sqlmixPackRec */
        ( DBENTRYP_VS )   NULL,                   /* sqlmixSort */
        ( DBENTRYP_VT )   NULL,                   /* sqlmixTrans */
        ( DBENTRYP_VT )   NULL,                   /* sqlmixTransRec */
        ( DBENTRYP_V )    NULL,                   /* sqlmixZap */
        ( DBENTRYP_VR )   NULL,                   /* sqlmixChildEnd */
        ( DBENTRYP_VR )   NULL,                   /* sqlmixChildStart */
        ( DBENTRYP_VR )   NULL,                   /* sqlmixChildSync */
        ( DBENTRYP_V )    NULL,                   /* sqlmixSyncChildren */
        ( DBENTRYP_V )    NULL,                   /* sqlmixClearRel */
        ( DBENTRYP_V )    NULL,                   /* sqlmixForceRel */
        ( DBENTRYP_SSP )  NULL,                   /* sqlmixRelArea */
        ( DBENTRYP_VR )   NULL,                   /* sqlmixRelEval */
        ( DBENTRYP_SI )   NULL,                   /* sqlmixRelText */
        ( DBENTRYP_VR )   NULL,                   /* sqlmixSetRel */
        ( DBENTRYP_VOI )  NULL,                   /* sqlmixOrderListAdd */
        ( DBENTRYP_V )    sqlmixOrderListClear,
        ( DBENTRYP_VOI )  NULL,                   /* sqlmixOrderListDelete */
        ( DBENTRYP_VOI )  sqlmixOrderListFocus,
        ( DBENTRYP_V )    NULL,                   /* sqlmixOrderListRebuild */
        ( DBENTRYP_VOO )  NULL,                   /* sqlmixOrderCondition */
        ( DBENTRYP_VOC )  sqlmixOrderCreate,
        ( DBENTRYP_VOI )  NULL,                   /* sqlmixOrderDestroy */
        ( DBENTRYP_SVOI ) sqlmixOrderInfo,
        ( DBENTRYP_V )    NULL,                   /* sqlmixClearFilter */
        ( DBENTRYP_V )    NULL,                   /* sqlmixClearLocate */
        ( DBENTRYP_V )    NULL,                   /* sqlmixClearScope */
        ( DBENTRYP_VPLP ) NULL,                   /* sqlmixCountScope */
        ( DBENTRYP_I )    NULL,                   /* sqlmixFilterText */
        ( DBENTRYP_SI )   NULL,                   /* sqlmixScopeInfo */
        ( DBENTRYP_VFI )  NULL,                   /* sqlmixSetFilter */
        ( DBENTRYP_VLO )  NULL,                   /* sqlmixSetLocate */
        ( DBENTRYP_VOS )  NULL,                   /* sqlmixSetScope */
        ( DBENTRYP_VPL )  NULL,                   /* sqlmixSkipScope */
        ( DBENTRYP_B )    NULL,                   /* sqlmixLocate */
        ( DBENTRYP_CC )   NULL,                   /* sqlmixCompile */
        ( DBENTRYP_I )    NULL,                   /* sqlmixError */
        ( DBENTRYP_I )    NULL,                   /* sqlmixEvalBlock */
        ( DBENTRYP_VSP )  NULL,                   /* sqlmixRawLock */
        ( DBENTRYP_VL )   NULL,                   /* sqlmixLock */
        ( DBENTRYP_I )    NULL,                   /* sqlmixUnLock */
        ( DBENTRYP_V )    NULL,                   /* sqlmixCloseMemFile */
        ( DBENTRYP_VO )   NULL,                   /* sqlmixCreateMemFile */
        ( DBENTRYP_SCCS ) NULL,                   /* sqlmixGetValueFile */
        ( DBENTRYP_VO )   NULL,                   /* sqlmixOpenMemFile */
        ( DBENTRYP_SCCS ) NULL,                   /* sqlmixPutValueFile */
        ( DBENTRYP_V )    NULL,                   /* sqlmixReadDBHeader */
        ( DBENTRYP_V )    NULL,                   /* sqlmixWriteDBHeader */
        ( DBENTRYP_R )    NULL,                   /* sqlmixInit */
        ( DBENTRYP_R )    sqlmixExit,
        ( DBENTRYP_RVVL)  NULL,                   /* sqlmixDrop */
        ( DBENTRYP_RVVL)  NULL,                   /* sqlmixExists */
        ( DBENTRYP_RVVVL) NULL,                   /* sqlmixRename */
        ( DBENTRYP_RSLV ) NULL,                   /* sqlmixRddInfo */
        ( DBENTRYP_SVP )  NULL,                   /* sqlmixWhoCares */
       };


HB_FUNC( SQLMIX_GETFUNCTABLE )
{
   RDDFUNCS * pTable;
   USHORT * uiCount, uiRddId;

   uiCount = ( USHORT * ) hb_itemGetPtr( hb_param( 1, HB_IT_POINTER ) );
   * uiCount = RDDFUNCSCOUNT;
   pTable = ( RDDFUNCS * ) hb_itemGetPtr( hb_param( 2, HB_IT_POINTER ) );
   uiRddId = ( USHORT ) hb_parni( 4 );

   if ( pTable )
   {
      HB_ERRCODE errCode;

      errCode = hb_rddInherit( pTable, &sqlmixTable, &sqlmixSuper, "SQLBASE" );
      if ( errCode == HB_SUCCESS )
      {
         s_uiRddIdSQLMIX = uiRddId;
      }
      hb_retni( errCode );
   }
   else
   {
       hb_retni( HB_FAILURE );
   }
}

HB_FUNC( SQLMIX ) { ; }

HB_FUNC_EXTERN( SQLBASE );

static void hb_sqlmixRddInit( void * cargo )
{
   HB_SYMBOL_UNUSED( cargo );

   if ( hb_rddRegister( "SQLMIX", RDT_FULL ) > 1 )
   {
      /* try different RDD registrer order */
      hb_rddRegister( "SQLBASE", RDT_FULL );

      if ( hb_rddRegister( "SQLMIX", RDT_FULL ) > 1 )
      {
         hb_errInternal( HB_EI_RDDINVALID, NULL, NULL, NULL );
         HB_FUNC_EXEC( SQLBASE );   /* force SQLBASE linking */
      }
   }
}

HB_INIT_SYMBOLS_BEGIN( sqlmix__InitSymbols )
{ "SQLMIX",              {HB_FS_PUBLIC|HB_FS_LOCAL}, {HB_FUNCNAME( SQLMIX )}, NULL },
{ "SQLMIX_GETFUNCTABLE", {HB_FS_PUBLIC|HB_FS_LOCAL}, {HB_FUNCNAME( SQLMIX_GETFUNCTABLE )}, NULL }
HB_INIT_SYMBOLS_END( sqlmix__InitSymbols )

HB_CALL_ON_STARTUP_BEGIN( _hb_sqlmix_rdd_init_ )
   hb_vmAtInit( hb_sqlmixRddInit, NULL );
HB_CALL_ON_STARTUP_END( _hb_sqlmix_rdd_init_ )

#if defined( HB_PRAGMA_STARTUP )
   #pragma startup sqlmix__InitSymbols
   #pragma startup _hb_sqlmix_rdd_init_
#elif defined( HB_MSC_STARTUP )
   #if defined( HB_OS_WIN_64 )
      #pragma section( HB_MSC_START_SEGMENT, long, read )
   #endif
   #pragma data_seg( HB_MSC_START_SEGMENT )
   static HB_$INITSYM hb_vm_auto_sqlmix__InitSymbols = sqlmix__InitSymbols;
   static HB_$INITSYM hb_vm_auto_sqlmix_rdd_init = _hb_sqlmix_rdd_init_;
   #pragma data_seg()
#endif
