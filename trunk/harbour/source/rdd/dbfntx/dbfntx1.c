/*
 * $Id$
 */

/*
 * Harbour Project source code:
 * DBFNTX RDD
 *
 * Copyright 1999 Bruno Cantero <bruno@issnet.net>
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
 * The following functions are added by
 *       Alexander Kresin <alex@belacy.belgorod.su>
 *
 * commonError()
 * hb_IncString()
 * numToStr()
 * checkLogicalExpr()
 * hb__ntxTagKeyCount()
 * hb_ntxInTopScope()
 * hb_ntxInBottomScope()
 * hb_ntxTagKeyNo()
 * hb_ntxTagKeyCount()
 * hb_ntxClearScope()
 * hb_ntxGoEof()
 * hb_ntxGetKeyType()
 * hb_ntxTagKeyFind()
 * hb_ntxPageKeySearch()
 * hb_ntxTagFindCurrentKey()
 * hb_ntxIsRecBad()
 * hb_ntxPageFindCurrentKey()
 * hb_ntxGetCurrentKey()
 * hb_ntxTagGoToNextKey()
 * hb_ntxTagGoToPrevKey()
 * hb_ntxTagGoToTopKey()
 * hb_ntxTagGoToBottomKey()
 * hb_ntxTagKeyGoTo()
 * hb_ntxPageRelease()
 * hb_ntxKeysMove()
 * hb_ntxPageSplit()
 * hb_ntxPageJoin()
 * hb_ntxPageBalance()
 * hb_ntxTagBalance()
 * hb_ntxPageKeyDel()
 * hb_ntxTagKeyAdd()
 * hb_ntxSwapPageSave()
 * hb_ntxKeysSort()
 * hb_ntxSortKeyAdd()
 * hb_ntxSortKeyEnd()
 * hb_ntxRootPage()
 * hb_ntxGetSortedKey()
 * hb_ntxBufferSave()
 * hb_ntxReadBuf()
 * hb_ntxPageFind()
 * hb_ntxPageLast()
 * ntxFindIndex()
 * ntxGoBottom()
 * ntxGoTo()
 * ntxGoTop()
 * ntxSeek()
 * ntxSkipRaw()
 * ntxGoCold()
 * ntxGoHot()
 * ntxSysName()
 * ntxPack()
 * ntxZap()
 * ntxClearScope()
 * ntxScopeInfo()
 * ntxOrderListAdd()
 * ntxOrderListClear()
 * ntxOrderListFocus()
 * ntxOrderListRebuild()
 * ntxSetScope()
 */

#include <math.h>
#include "hbapi.h"
#include "hbinit.h"
#include "hbapiitm.h"
#include "hbvm.h"
#include "hbstack.h"
#include "rddsys.ch"
#include "hbset.h"
#include "hbapierr.h"
#include "hbapilng.h"
#include "hbrddntx.h"

HB_FUNC( _DBFNTX );
HB_FUNC( DBFNTX_GETFUNCTABLE );

HB_INIT_SYMBOLS_BEGIN( dbfntx1__InitSymbols )
{ "_DBFNTX",             HB_FS_PUBLIC, HB_FUNCNAME( _DBFNTX ),             0 },
{ "DBFNTX_GETFUNCTABLE", HB_FS_PUBLIC, HB_FUNCNAME( DBFNTX_GETFUNCTABLE) , 0 }
HB_INIT_SYMBOLS_END( dbfntx1__InitSymbols )
#if defined(_MSC_VER)
   #if _MSC_VER >= 1010
      #pragma data_seg( ".CRT$XIY" )
      #pragma comment( linker, "/Merge:.CRT=.data" )
   #else
      #pragma data_seg( "XIY" )
   #endif
   static HB_$INITSYM hb_vm_auto_dbfntx1__InitSymbols = dbfntx1__InitSymbols;
   #pragma data_seg()
#elif ! defined(__GNUC__)
   #pragma startup dbfntx1__InitSymbols
#endif

static RDDFUNCS ntxSuper = { 0 };
static USHORT maxPagesPerTag = 50;

/* Internal functions */
static LPKEYINFO hb_ntxKeyNew( LPKEYINFO pKeyFrom, int keylen );
static LONG hb_ntxTagKeyFind( LPTAGINFO pTag, LPKEYINFO pKey, BOOL* result );
static BOOL hb_ntxIsRecBad( NTXAREAP pArea, LONG ulRecNo );
static int hb_ntxTagFindCurrentKey( LPPAGEINFO pPage, LPKEYINFO pKey, BOOL bExact, BOOL lSeek );
static USHORT hb_ntxPageFindCurrentKey( LPPAGEINFO pPage, ULONG ulRecno );
static void hb_ntxGetCurrentKey( LPTAGINFO pTag, LPKEYINFO pKey );
static void hb_ntxTagKeyGoTo( LPTAGINFO pTag, BYTE bTypRead, BOOL * lContinue );
static LPPAGEINFO hb_ntxPageFind( LPNTXINDEX pIndex ,LONG ulOffset );
static LPPAGEINFO hb_ntxPageLast( LPNTXINDEX pIndex );
static ERRCODE hb_ntxHeaderRead( LPNTXINDEX pIndex );
static ERRCODE hb_ntxHeaderLoad( LPNTXINDEX pIndex , char * ITN );
         /* Load NTX header an fill structures pIndex */
static void hb_ntxHeaderSave( LPNTXINDEX pIndex, BOOL bFull );
         /* Save NTX header */

static LPNTXINDEX hb_ntxIndexNew( NTXAREAP pArea );
         /* Allocate space for information about Index and find free ID */
static void  hb_ntxIndexFree( LPNTXINDEX pIndex );
         /* Release all resources associated with index */
static ERRCODE hb_ntxIndexCreate( LPNTXINDEX pIndex );
         /* Create index from database */

static LPTAGINFO hb_ntxTagNew( LPNTXINDEX PIF, char * ITN, char *szKeyExpr,
         PHB_ITEM pKeyExpr, BYTE bKeyType, USHORT uiKeyLen, USHORT uiKeyDec, char *szForExp,
         PHB_ITEM pForExp, BOOL fAscendKey, BOOL fUnique, BOOL fCustom );
         /* Create Compound Tag with information about index */

static LPPAGEINFO hb_ntxPageNew(LPTAGINFO pParentTag );
         /* Allocate space for new page */
static void hb_ntxPageRelease( LPPAGEINFO pPage );
static void hb_ntxPageFree( LPPAGEINFO pPage, BOOL lFull );
         /* Release memory allocated for page. If page was modified save it */
static void hb_ntxPageSave( LPPAGEINFO pPage );
         /* Save page */
static LPPAGEINFO hb_ntxPageLoad( LPNTXINDEX pIndex, ULONG ulOffset );
         /* Load page from disk */
static int hb_ntxItemCompare( char* s1, char* s2, int ilen1, int ilen2, BOOL Exact );

#define KEYITEM(P,N) ( (NTXITEM*)( (P)->buffer+ *((USHORT*)((P)->buffer+(N)*2+2)) ) )
#define KEYPOINTER(P,N) ( (USHORT*)((P)->buffer+(N)*2+2) )
#define hb_ntxKeyFree(K) hb_xfree(K)

static ULONG* hb_ntxKeysInPage( ULONG ulRecCount, USHORT maxkeys )
{
  double dSum = 0, koeff, _maxkeys = (double) maxkeys, 
         recCount = (double)ulRecCount, dMul = 1;
  int iLevel = 0, i, j;
  ULONG *lpArray;

  do
  {
     dSum += maxkeys * dMul;
     dMul *= (_maxkeys+1);
     iLevel ++;
  }
  while( dSum < recCount );

  lpArray = (ULONG*) hb_xgrab( sizeof(int) * (iLevel+2) );
  lpArray[0] = (ULONG)iLevel;
  for( i=1; i<=iLevel; i++ )
     lpArray[i] = 0;

  if( recCount > 0 )
  {
     for( i=iLevel; i; i-- )
     {
        koeff = dSum / recCount;
        lpArray[i] = (ULONG) ceil( _maxkeys/koeff );
        dMul = 1;
        for( j=iLevel,dSum=0; j; j-- )
        {
           dSum += ( (lpArray[j])? lpArray[j]:maxkeys ) * dMul;
           if( j > 1 )
              dMul *= (double)(( (lpArray[j])? lpArray[j]:maxkeys )+1);
        }
     }
     dSum -= recCount;
     lpArray[iLevel+1] = (dMul > dSum)? (ULONG)(dMul - dSum):(ULONG)dMul;
  }
  else
  {
     lpArray[1] = maxkeys;
     lpArray[2] = 0;
  }

  return lpArray;
}

static void commonError( NTXAREAP pArea, USHORT uiGenCode, USHORT uiSubCode, char* filename, USHORT uiFlags )
{
   PHB_ITEM pError;

   pError = hb_errNew();
   hb_errPutGenCode( pError, uiGenCode );
   hb_errPutSubCode( pError, uiSubCode );
   hb_errPutDescription( pError, hb_langDGetErrorDesc( uiGenCode ) );
   if( filename )
      hb_errPutFileName( pError, filename );
   if( uiFlags )
      hb_errPutFlags( pError, uiFlags );
   SUPER_ERROR( ( AREAP ) pArea, pError );
   hb_errRelease( pError );
   return;
}

static void hb_IncString( char* s, int slen )
{
   char *ptr;
   unsigned int nsymb;

   for( ptr=s+slen-1;ptr>=s;ptr-- )
   {
      nsymb = (unsigned int) *ptr;
      if( nsymb < 255 )
      {
         *ptr = (char) ++nsymb;
         break;
      }
   }
}

static char * numToStr( PHB_ITEM pItem, char* szBuffer, USHORT length, USHORT dec )
{
   if( HB_IS_DOUBLE( pItem ) )
   {
      if( dec == 0 )
      {
         if( length > 9 )
            sprintf( szBuffer, "%0*.0f", length,
                hb_numRound( hb_itemGetND( pItem ), 0 ) );
         else
            sprintf( szBuffer, "%0*li", length,
                ( LONG ) hb_numRound( hb_itemGetND( pItem ), 0 ) );
      }
      else
         sprintf( szBuffer, "%0*.*f", length,
                dec, hb_numRound( hb_itemGetND( pItem ),
                dec ) );
   }
   else
   {
      if( dec == 0 )
         sprintf( szBuffer, "%0*li", length, hb_itemGetNL( pItem ) );
      else
         sprintf( szBuffer, "%0*.*f", length,
                dec, hb_itemGetND( pItem ) );
   }
   szBuffer[ length ] = 0;
   if( hb_itemGetND( pItem ) < 0 )
   {
      char *ptr = szBuffer;
      *ptr++ = ',';
      for( ;*ptr;ptr++ )
         if( *ptr >= '0' && *ptr <= '9' )
            *ptr = (char) ( 92 - (int)*ptr );
   }
   return szBuffer;
}

/* Implementation of internal functions */

static BOOL checkLogicalExpr( PHB_ITEM pForItem, PHB_ITEM pItem )
{
   HB_MACRO_PTR pMacro;
   BOOL res, lNewItem = FALSE;

   if( !pItem )
   {
      pItem = hb_itemNew( NULL );
      lNewItem = TRUE;
   }
   if( hb_itemType( pForItem ) == HB_IT_BLOCK )
   {
      hb_vmPushSymbol( &hb_symEval );
      hb_vmPush( pForItem );
      hb_vmSend( 0 );
      hb_itemCopy( pItem, &hb_stack.Return );
   }
   else
   {
      pMacro = ( HB_MACRO_PTR ) hb_itemGetPtr( pForItem );
      hb_macroRun( pMacro );
      hb_itemCopy( pItem, hb_stackItemFromTop( - 1 ) );
      hb_stackPop();
   }
   res = hb_itemGetL( pItem );
   if( lNewItem )
      hb_itemRelease( pItem );
   return res;
}

static void hb__ntxTagKeyCount( LPPAGEINFO pPage, ULONG* ulKeyCount )
{
   LPNTXITEM p;
   int i;

   *ulKeyCount += pPage->uiKeys;
   for( i=0;i<pPage->uiKeys+1;i++ )
   {
      p = KEYITEM( pPage, i );
      if( p->page )
         hb__ntxTagKeyCount( hb_ntxPageLoad( pPage->TagParent->Owner,p->page ),
                                    ulKeyCount );
   }
   hb_ntxPageRelease( pPage );
}

static BOOL hb_ntxInTopScope( LPTAGINFO pTag, char* key )
{
   if( pTag->topScope )
      return ( hb_ntxItemCompare( pTag->topScope->item.asString.value,
                  key, pTag->topScope->item.asString.length,
                  pTag->KeyLength,0 ) <= 0 );
   else
      return TRUE;
}

static BOOL hb_ntxInBottomScope( LPTAGINFO pTag, char* key )
{
   if( pTag->bottomScope )
      return ( hb_ntxItemCompare( pTag->bottomScope->item.asString.value,
                  key, pTag->bottomScope->item.asString.length,
                  pTag->KeyLength,0 ) >= 0 );
   else
      return TRUE;
}

static ULONG hb_ntxTagKeyNo( LPTAGINFO pTag )
{
   ULONG ulKeyNo = 0;

   if( pTag->Owner->Owner->fShared )
   {
      while( !hb_fsLock( pTag->Owner->DiskFile, NTX_LOCK_OFFSET, 1, FL_LOCK ) );
      pTag->Owner->Locked = TRUE;
   }

   if( pTag->topScope || pTag->bottomScope )
   {
      hb_ntxGetCurrentKey( pTag, pTag->CurKeyInfo );
      if( hb_ntxInTopScope( pTag, pTag->CurKeyInfo->key ) &&
            hb_ntxInBottomScope( pTag, pTag->CurKeyInfo->key ) )
      {
         BOOL lContinue = FALSE;
         ULONG ulRecNo = pTag->Owner->Owner->ulRecNo;
         LPKEYINFO pKeyTmp = hb_ntxKeyNew( pTag->CurKeyInfo,pTag->KeyLength );

         do
         {
            ulKeyNo ++;
            hb_ntxTagKeyGoTo( pTag, PREV_RECORD, &lContinue );
         }
         while( !pTag->TagBOF && hb_ntxInTopScope( pTag, pTag->CurKeyInfo->key ) );
         strcpy( pTag->CurKeyInfo->key, pKeyTmp->key );
         pTag->CurKeyInfo->Tag = pKeyTmp->Tag;
         pTag->CurKeyInfo->Xtra = pKeyTmp->Xtra;
         hb_ntxKeyFree( pKeyTmp );
         pTag->Owner->Owner->ulRecNo = ulRecNo;
      }
   }
   else
   {
      int seekRes;
      SHORT i, j;
      LPPAGEINFO pPage;
      LPNTXITEM p;
      LPKEYINFO pKey = hb_ntxKeyNew( NULL,pTag->KeyLength );

      hb_ntxGetCurrentKey( pTag,pKey );
      seekRes = hb_ntxTagFindCurrentKey( hb_ntxPageLoad( pTag->Owner,0 ), pKey, FALSE, FALSE );
      hb_ntxKeyFree( pKey );
      if( seekRes )
      {
         printf( "\n\rhb_ntxTagKeyNo: Cannot find current key:" );
         return 0;
      }
      ulKeyNo = 1;
      for( i=0;i<=pTag->stackLevel;i++ )
      {
         pPage = hb_ntxPageLoad( pTag->Owner,pTag->stack[i].page );
         if( pTag->stack[i].ikey )
         {
            ulKeyNo += pTag->stack[i].ikey;
            for( j=0;j<pTag->stack[i].ikey;j++ )
            {
               p = KEYITEM( pPage, j );
               if( p->page )
                  hb__ntxTagKeyCount( hb_ntxPageLoad( pTag->Owner,p->page ),
                                          &ulKeyNo );
            }
         }
         hb_ntxPageRelease( pPage );
      }
   }

   if( pTag->Owner->Owner->fShared )
   {
      hb_ntxPageFree( pTag->RootPage,FALSE );
      hb_fsLock( pTag->Owner->DiskFile, NTX_LOCK_OFFSET, 1, FL_UNLOCK );
      pTag->Owner->Locked = FALSE;
   }
   return ulKeyNo;
}

static ULONG hb_ntxTagKeyCount( LPTAGINFO pTag )
{
   LPPAGEINFO pPage;
   LPNTXITEM p;
   ULONG ulKeyCount = 0;
   int i;

   if( pTag->Owner->Owner->fShared )
   {
      while( !hb_fsLock( pTag->Owner->DiskFile, NTX_LOCK_OFFSET, 1, FL_LOCK ) );
      pTag->Owner->Locked = TRUE;
   }
   else if( pTag->keyCount )
      return pTag->keyCount;

   if( pTag->topScope || pTag->bottomScope )
   {
      BOOL lContinue = FALSE;
      LONG lRecno = 1;
      LPKEYINFO pKeyTmp = hb_ntxKeyNew( pTag->CurKeyInfo,pTag->KeyLength );
      ULONG ulRecNo = pTag->Owner->Owner->ulRecNo;

      if( pTag->topScope )
      {
         LPKEYINFO pKey = hb_ntxKeyNew( NULL,pTag->KeyLength );

         strcpy( pKey->key,pTag->topScope->item.asString.value );
         lRecno = ( hb_ntxTagFindCurrentKey( hb_ntxPageLoad( pTag->Owner,0 ),
                      pKey, FALSE, TRUE ) <= 0 )? pTag->CurKeyInfo->Xtra:0;
         hb_ntxKeyFree( pKey );
      }
      else
      {
         hb_ntxTagKeyGoTo( pTag, TOP_RECORD, NULL );
      }
      if( lRecno )
         do
         {
            ulKeyCount ++;
            hb_ntxTagKeyGoTo( pTag, NEXT_RECORD, &lContinue );
         }
         while( !pTag->TagEOF && hb_ntxInBottomScope( pTag, pTag->CurKeyInfo->key ) );

      strcpy( pTag->CurKeyInfo->key, pKeyTmp->key );
      pTag->CurKeyInfo->Tag = pKeyTmp->Tag;
      pTag->CurKeyInfo->Xtra = pKeyTmp->Xtra;
      hb_ntxKeyFree( pKeyTmp );
      pTag->Owner->Owner->ulRecNo = ulRecNo;
   }
   else
   {
      pPage = hb_ntxPageLoad( pTag->Owner,0 );
      ulKeyCount += pPage->uiKeys;
      for( i=0;i<pPage->uiKeys+1;i++ )
      {
         p = KEYITEM( pPage, i );
         if( p->page )
            hb__ntxTagKeyCount( hb_ntxPageLoad( pTag->Owner,p->page ),
                                    &ulKeyCount );
      }
      hb_ntxPageRelease( pPage );
   }

   if( pTag->Owner->Owner->fShared )
   {
      hb_ntxPageFree( pTag->RootPage,FALSE );
      hb_fsLock( pTag->Owner->DiskFile, NTX_LOCK_OFFSET, 1, FL_UNLOCK );
      pTag->Owner->Locked = FALSE;
   }
   else
      pTag->keyCount = ulKeyCount;
   return ulKeyCount;
}

static void hb_ntxClearScope( LPTAGINFO pTag, USHORT nScope )
{
   if( nScope == 0 )
   {
      if( pTag->topScope )
      {
         hb_itemRelease( pTag->topScope );
         pTag->topScope = NULL;
      }
   }
   else
   {
      if( pTag->bottomScope )
      {
         hb_itemRelease( pTag->bottomScope );
         pTag->bottomScope = NULL;
      }
   }
}

static ERRCODE hb_ntxGoEof( NTXAREAP pArea )
{
   ERRCODE  retvalue;
   LPNTXINDEX lpCurIndex;

   lpCurIndex = pArea->lpCurIndex;
   pArea->lpCurIndex = NULL;
   retvalue = SUPER_GOTO( ( AREAP ) pArea, pArea->ulRecCount+1 );
   if( pArea->ulRecCount )
      pArea->fBof = lpCurIndex->CompoundTag->TagBOF = FALSE;
   pArea->fEof = lpCurIndex->CompoundTag->TagEOF = TRUE;
   pArea->lpCurIndex = lpCurIndex;
   return retvalue;
}

static USHORT hb_ntxGetKeyType( LPTAGINFO pTag )
{
   if( hb_itemType( pTag->pKeyItem ) == HB_IT_BLOCK )
   {
      hb_vmPushSymbol( &hb_symEval );
      hb_vmPush( pTag->pKeyItem );
      hb_vmSend( 0 );
      return hb_itemType( &hb_stack.Return );
   }
   else
   {
      USHORT nType;
      HB_MACRO_PTR pMacro;
      pMacro = ( HB_MACRO_PTR ) hb_itemGetPtr( pTag->pKeyItem );
      hb_macroRun( pMacro );
      nType = hb_itemType( hb_stackItemFromTop( - 1 ) );
      hb_stackPop();
      return nType;
   }
}

static LPKEYINFO hb_ntxKeyNew( LPKEYINFO pKeyFrom, int keylen )
{
   LPKEYINFO pKey;

   pKey = ( LPKEYINFO ) hb_xgrab( sizeof( KEYINFO ) + keylen );
   if( pKeyFrom )
   {
      strcpy( pKey->key, pKeyFrom->key );
      pKey->Tag = pKeyFrom->Tag;
      pKey->Xtra = pKeyFrom->Xtra;
   }
   else
   {
      *(pKey->key) = '\0';
      pKey->Tag = pKey->Xtra = 0;
   }
   return pKey;
}

static LONG hb_ntxTagKeyFind( LPTAGINFO pTag, LPKEYINFO pKey, BOOL * result )
{
   int K;

   pTag->CurKeyInfo->Tag = 0;
   pTag->TagBOF = pTag->TagEOF = *result = FALSE;
   K = hb_ntxTagFindCurrentKey( hb_ntxPageLoad( pTag->Owner,0 ), pKey, FALSE, TRUE );
   if( K == 0 )
   {
      *result = TRUE;
      return pTag->CurKeyInfo->Xtra;
   }
   else if( K < 0 )
   {
      return pTag->CurKeyInfo->Xtra;
   }
   else
      pTag->TagEOF = TRUE;
   return 0;
}

static int hb_ntxPageKeySearch( LPPAGEINFO pPage, char* key, BOOL bExact, BOOL bInsert )
{
   SHORT i, iLast = -1, k, keylen = strlen( key );
   SHORT iBegin = 0, iEnd = pPage->uiKeys - 1;
   LPTAGINFO pTag = pPage->TagParent;

   while( iBegin <= iEnd )
   {
      i = ( iBegin + iEnd ) / 2;
      k = hb_ntxItemCompare( key, KEYITEM( pPage, i )->key, keylen, pTag->KeyLength,bExact );
      if( !pTag->AscendKey )
         k = -k;
      if( k > 0 || ( bInsert && !k ) )
         iBegin = i + 1;
      else if( k < 0 )
      {
         iEnd = i - 1;
         iLast = i;
      }
      else
      {
         while( !k && i-- )
            k = hb_ntxItemCompare( key, KEYITEM( pPage, i )->key,
                         keylen, pTag->KeyLength, bExact );
         pPage->CurKey = i + 1;
         return 0;
      }
   }
   if( iLast >= 0 )
   {
      pPage->CurKey = iLast;
      return -1;
   }
   else
   {
      pPage->CurKey = pPage->uiKeys;
      return 1;
   }
}

static int hb_ntxTagFindCurrentKey( LPPAGEINFO pPage, LPKEYINFO pKey, BOOL bExact, BOOL lSeek )
{
   int k, kChild, keylen = strlen( pKey->key );
   LPNTXITEM p;
   LPTAGINFO pTag = pPage->TagParent;

   bExact = ( bExact || pTag->KeyType != 'C' );
   k = hb_ntxPageKeySearch( pPage, pKey->key, bExact, FALSE );
   do
   {
      p = KEYITEM( pPage, pPage->CurKey );
      if( pPage->CurKey == pPage->uiKeys )
      {
         if( !p->page )
            break;
         k = -1;
      }
      if( k <= 0 )
      /* pKey <= p */
      {
         if( ( k == 0 && !lSeek && (ULONG)p->rec_no != pTag->Owner->Owner->ulRecNo )
             || ( lSeek && hb_ntxIsRecBad( pTag->Owner->Owner, p->rec_no ) ) )
            k = 1;
         if( k <= 0 && pPage->CurKey < pPage->uiKeys )
         {
            pTag->CurKeyInfo->Xtra = p->rec_no;
            pTag->CurKeyInfo->Tag = pPage->Page;
         }
         if( p->page && ( k < 0 || lSeek || ( (ULONG)p->rec_no != pTag->Owner->Owner->ulRecNo ) ) )
         {
            kChild = hb_ntxTagFindCurrentKey( hb_ntxPageLoad(
                     pTag->Owner,p->page ), pKey, bExact, lSeek );
            if( k != 0 || kChild == 0 )
               k = kChild;

            if( k <= 0 )
            {
               if( ++(pTag->stackLevel) >= pTag->stackDepth )
               {
                  pTag->stackDepth += 32;
                  pTag->stack = (LPTREESTACK) hb_xrealloc( pTag->stack,
                         sizeof(TREE_STACK) * pTag->stackDepth );
               }
            }
         }
         else if( k <= 0 && pPage->CurKey < pPage->uiKeys )
            pTag->stackLevel = 0;
      }
      if( k > 0 && ++pPage->CurKey <= pPage->uiKeys )
      {
         if( pPage->CurKey < pPage->uiKeys )
         {
            k = hb_ntxItemCompare( pKey->key, KEYITEM( pPage, pPage->CurKey )->key, keylen, pTag->KeyLength,bExact );
            if( !pTag->AscendKey )
               k = -k;
         }
      }
      else
         break;
   }
   while( 1 );
   if( k <= 0 )
   {
      pTag->stack[pTag->stackLevel].page = pPage->Page;
      pTag->stack[pTag->stackLevel].ikey = pPage->CurKey;
   }
   hb_ntxPageRelease( pPage );
   return k;
}

static BOOL hb_ntxIsRecBad( NTXAREAP pArea, LONG ulRecNo )
{

   BOOL lResult = FALSE;

   if( hb_set.HB_SET_DELETED || pArea->dbfi.itmCobExpr )
      SELF_GOTO( ( AREAP ) pArea,ulRecNo );

   if( hb_set.HB_SET_DELETED )
      SUPER_DELETED( ( AREAP ) pArea,&lResult );

   if( !lResult && pArea->dbfi.itmCobExpr )
   {
      PHB_ITEM pResult = hb_vmEvalBlock( pArea->dbfi.itmCobExpr );
      lResult = HB_IS_LOGICAL( pResult ) && !hb_itemGetL( pResult );
   }

   return lResult;
}

static USHORT hb_ntxPageFindCurrentKey( LPPAGEINFO pPage, ULONG ulRecno )
{
   int i;
   for( i=0; i < pPage->uiKeys; i++ )
      if( KEYITEM( pPage,i )->rec_no == ulRecno )
          return ( i+1 );
   return 0;
}

static void hb_ntxGetCurrentKey( LPTAGINFO pTag, LPKEYINFO pKey )
{
   char szBuffer[ NTX_MAX_KEY ];
   if( hb_itemType( pTag->pKeyItem ) == HB_IT_BLOCK )
   {
      hb_vmPushSymbol( &hb_symEval );
      hb_vmPush( pTag->pKeyItem );
      hb_vmSend( 0 );
      switch( hb_itemType( &hb_stack.Return ) )
      {
         case HB_IT_STRING:
            strcpy( pKey->key, (&hb_stack.Return)->item.asString.value );
            break;
         case HB_IT_INTEGER:
         case HB_IT_LONG:
         case HB_IT_DOUBLE:
            strcpy( pKey->key, numToStr( &hb_stack.Return, szBuffer, pTag->KeyLength, pTag->KeyDec ) );
            break;
        case HB_IT_DATE:
           hb_itemGetDS( &hb_stack.Return, szBuffer );
           strcpy( pKey->key,szBuffer );
           break;
        case HB_IT_LOGICAL:
           szBuffer[0] = ( hb_itemGetL( &hb_stack.Return ) ? 'T':'F' );
           szBuffer[1] = 0;
           strcpy( pKey->key, szBuffer );
           break;
      }
   }
   else
   {
      HB_MACRO_PTR pMacro;
      pMacro = ( HB_MACRO_PTR ) hb_itemGetPtr( pTag->pKeyItem );
      hb_macroRun( pMacro );
      switch( hb_itemType( hb_stackItemFromTop( - 1 ) ) )
      {
         case HB_IT_STRING:
            strcpy( pKey->key, hb_stackItemFromTop( - 1 )->item.asString.value );
            break;
         case HB_IT_INTEGER:
         case HB_IT_LONG:
         case HB_IT_DOUBLE:
            strcpy( pKey->key, numToStr( hb_stackItemFromTop( - 1 ), szBuffer, pTag->KeyLength, pTag->KeyDec ) );
            break;
        case HB_IT_DATE:
           hb_itemGetDS( hb_stackItemFromTop( - 1 ), szBuffer );
           strcpy( pKey->key,szBuffer );
           break;
        case HB_IT_LOGICAL:
           szBuffer[0] = ( hb_itemGetL( hb_stackItemFromTop( - 1 ) ) ? 'T' : 'F' );
           szBuffer[1] = 0;
           strcpy( pKey->key, szBuffer );
           break;
      }
      hb_stackPop();
   }
   pKey->Xtra = pTag->Owner->Owner->ulRecNo;
}

static BOOL hb_ntxTagGoToNextKey( LPTAGINFO pTag, BOOL lContinue )
{
   BOOL lCurrrentKey = FALSE;
   LPPAGEINFO pPage, pChildPage;
   LPNTXITEM p;

   // pTag->blockNext = 0; pTag->keyNext = 0;
   if( pTag->CurKeyInfo->Tag )
   {
      pPage =  hb_ntxPageLoad( pTag->Owner,pTag->CurKeyInfo->Tag );
      pPage->CurKey =  hb_ntxPageFindCurrentKey( pPage,pTag->CurKeyInfo->Xtra );
      if( pPage->CurKey &&
           ( pPage->CurKey < pPage->uiKeys ||
           ( pPage->CurKey == pPage->uiKeys &&
             ( KEYITEM( pPage, pPage->CurKey ) )->page ) ) )
        lCurrrentKey = TRUE;
      else
        hb_ntxPageRelease( pPage );
   }

   if( !lCurrrentKey )
   {
      int seekRes;
      LPKEYINFO pKey;

      pKey = hb_ntxKeyNew( NULL,pTag->KeyLength );
      if( lContinue )
      {
         strcpy( pKey->key,pTag->CurKeyInfo->key );
         pTag->Owner->Owner->ulRecNo = pTag->CurKeyInfo->Xtra;
      }
      else
         hb_ntxGetCurrentKey( pTag,pKey );
      seekRes = hb_ntxTagFindCurrentKey( hb_ntxPageLoad( pTag->Owner,0 ), pKey, FALSE, FALSE );
      hb_ntxKeyFree( pKey );
      if( seekRes )
      {
         printf( "\n\rhb_ntxFindNextKey: Cannot find current key:" );
         return FALSE;
      }
      pPage =  hb_ntxPageLoad( pTag->Owner,pTag->CurKeyInfo->Tag );
      pPage->CurKey =  hb_ntxPageFindCurrentKey( pPage,pTag->CurKeyInfo->Xtra );
   }

   if( pPage->CurKey < pPage->uiKeys ||
          ( pPage->CurKey == pPage->uiKeys && ( KEYITEM( pPage, pPage->CurKey ) )->page ) )
   {
      while( ( p = KEYITEM( pPage, pPage->CurKey ) )->page )
      {
         pChildPage = hb_ntxPageLoad( pTag->Owner,p->page );
         hb_ntxPageRelease( pPage );
         pPage = pChildPage;
         pPage->CurKey = 0;
      }
      strcpy( pTag->CurKeyInfo->key, p->key );
      pTag->CurKeyInfo->Xtra = p->rec_no;
      pTag->CurKeyInfo->Tag = pPage->Page;
      hb_ntxPageRelease( pPage );
      return TRUE;
   }
   else
   {
      SHORT i = 1;
      hb_ntxPageRelease( pPage );
      /*
      if( pTag->blockNext )
      {
         pPage = hb_ntxPageLoad( pTag->Owner,pTag->blockNext );
         pPage->CurKey =  pTag->keyNext;
      */
      while( pTag->stackLevel >= i )
      {
         pPage = hb_ntxPageLoad( pTag->Owner,pTag->stack[i].page );
         if( pTag->stack[i].ikey < pPage->uiKeys )
            break;
         hb_ntxPageRelease( pPage );
         i ++;
      }
      if( pTag->stackLevel >= i )
      {
         pPage->CurKey =  pTag->stack[i].ikey;
         /*
            while( ( pPage->pKeys+pPage->CurKey )->Tag )
            {
               pChildPage = hb_ntxPageLoad( ( pPage->pKeys+pPage->CurKey )->Tag );
               hb_ntxPageRelease( pPage );
               pPage = pChildPage;
               pPage->CurKey = 0;
            }
         */
         strcpy( pTag->CurKeyInfo->key, ( KEYITEM( pPage, pPage->CurKey ) )->key );
         pTag->CurKeyInfo->Xtra = ( KEYITEM( pPage, pPage->CurKey ) )->rec_no;
         pTag->CurKeyInfo->Tag = pPage->Page;
         hb_ntxPageRelease( pPage );
         return TRUE;
      }
      else
         return FALSE;
   }
}

static BOOL hb_ntxTagGoToPrevKey( LPTAGINFO pTag, BOOL lContinue )
{
   BOOL lCurrrentKey = FALSE;
   LPPAGEINFO pPage, pChildPage;

   // pTag->blockPrev = 0; pTag->keyPrev = 0;
   if( pTag->CurKeyInfo->Tag )
   {
      pPage =  hb_ntxPageLoad( pTag->Owner,pTag->CurKeyInfo->Tag );
      pPage->CurKey =  hb_ntxPageFindCurrentKey( pPage,pTag->CurKeyInfo->Xtra );
      if( pPage->CurKey-- &&
             ( (KEYITEM( pPage, pPage->CurKey ))->page || pPage->CurKey >= 1 ) )
         lCurrrentKey = TRUE;
      else
        hb_ntxPageRelease( pPage );
   }

   if( !lCurrrentKey )
   {
      int seekRes;
      LPKEYINFO pKey;

      pKey = hb_ntxKeyNew( NULL,pTag->KeyLength );
      if( lContinue )
      {
         strcpy( pKey->key,pTag->CurKeyInfo->key );
         pTag->Owner->Owner->ulRecNo = pTag->CurKeyInfo->Xtra;
      }
      else
         hb_ntxGetCurrentKey( pTag, pKey );
      seekRes = hb_ntxTagFindCurrentKey( hb_ntxPageLoad( pTag->Owner,0 ), pKey, FALSE, FALSE );
      hb_ntxKeyFree( pKey );
      if( seekRes )
      {
         printf( "\n\rhb_ntxFindPrevKey: Cannot find current key: |%ld %s|",pTag->Owner->Owner->ulRecNo,pKey->key );
         return FALSE;
      }
      pPage =  hb_ntxPageLoad( pTag->Owner,pTag->CurKeyInfo->Tag );
      pPage->CurKey = hb_ntxPageFindCurrentKey( pPage,pTag->CurKeyInfo->Xtra );
      pPage->CurKey--;
   }

   if( ( KEYITEM( pPage, pPage->CurKey ) )->page )
   {
      do
      {
         pChildPage = hb_ntxPageLoad( pTag->Owner,( KEYITEM( pPage, pPage->CurKey ) )->page );
         hb_ntxPageRelease( pPage );
         pPage = pChildPage;
         pPage->CurKey = pPage->uiKeys;
      }
      while( ( KEYITEM( pPage, pPage->CurKey ) )->page );
      pPage->CurKey--;
   }
   else
      pPage->CurKey--;
   if( pPage->CurKey >= 0 )
   {
      strcpy( pTag->CurKeyInfo->key, ( KEYITEM( pPage, pPage->CurKey ) )->key );
      pTag->CurKeyInfo->Xtra = ( KEYITEM( pPage, pPage->CurKey ) )->rec_no;
      pTag->CurKeyInfo->Tag = pPage->Page;
      hb_ntxPageRelease( pPage );
      return TRUE;
   }
   else
   {
      SHORT i = 1;
      hb_ntxPageRelease( pPage );
      /*
      if( pTag->blockPrev )
      {
         pPage = hb_ntxPageLoad( pTag->Owner,pTag->blockPrev );
         pPage->CurKey =  pTag->keyPrev;
      */
      while( pTag->stackLevel >= i )
      {
         pPage = hb_ntxPageLoad( pTag->Owner,pTag->stack[i].page );
         pPage->CurKey =  pTag->stack[i].ikey - 1;
         if( pPage->CurKey < pPage->uiKeys && pPage->CurKey >= 0 )
            break;
         hb_ntxPageRelease( pPage );
         i ++;
      }
      if( pTag->stackLevel >= i )
      {
         strcpy( pTag->CurKeyInfo->key, ( KEYITEM( pPage, pPage->CurKey ) )->key );
         pTag->CurKeyInfo->Xtra = ( KEYITEM( pPage, pPage->CurKey ) )->rec_no;
         pTag->CurKeyInfo->Tag = pPage->Page;
         hb_ntxPageRelease( pPage );
         return TRUE;
      }
      else
         return FALSE;
   }
}

static BOOL hb_ntxTagGoToTopKey( LPTAGINFO pTag, LPPAGEINFO pPage, ULONG ulOffset )
{
   LPPAGEINFO pChildPage;
   LPNTXITEM p;

   pChildPage = hb_ntxPageLoad( pTag->Owner,ulOffset );
   if( pPage )
   {
      hb_ntxPageRelease( pPage );
   }
   if( pChildPage != NULL && pChildPage->uiKeys )
   {
      p = KEYITEM( pChildPage, 0 );
      ulOffset = p->page;
      if( ulOffset )
      {
         return hb_ntxTagGoToTopKey( pTag,pChildPage,ulOffset );
      }
      else
      {
         strcpy( pTag->CurKeyInfo->key, p->key );
         pTag->CurKeyInfo->Xtra = p->rec_no;
         pTag->CurKeyInfo->Tag = pChildPage->Page;
         hb_ntxPageRelease( pChildPage );
         return TRUE;
      }
   }
   else
      return FALSE;
}

static BOOL hb_ntxTagGoToBottomKey( LPTAGINFO pTag, LPPAGEINFO pPage, ULONG ulOffset )
{
   LPPAGEINFO pChildPage;
   LPNTXITEM p;

   pChildPage = hb_ntxPageLoad( pTag->Owner,ulOffset );
   if( pPage )
      hb_ntxPageRelease( pPage );
   if( pChildPage != NULL && pChildPage->uiKeys )
   {
      ulOffset = ( KEYITEM( pChildPage, pChildPage->uiKeys ) )->page;
      if( ulOffset )
         return hb_ntxTagGoToBottomKey( pTag,pChildPage,ulOffset );
      else
      {
         p = KEYITEM( pChildPage, pChildPage->uiKeys-1 );
         strcpy( pTag->CurKeyInfo->key, p->key );
         pTag->CurKeyInfo->Xtra = p->rec_no;
         pTag->CurKeyInfo->Tag = pChildPage->Page;
         hb_ntxPageRelease( pChildPage );
         return TRUE;
      }
   }
   else
      return FALSE;
}

static void hb_ntxTagKeyGoTo( LPTAGINFO pTag, BYTE bTypRead, BOOL * lContinue )
{
   BOOL wasLocked = FALSE;

   pTag->TagBOF = pTag->TagEOF = FALSE;
   if( pTag->Owner->Owner->ulRecCount )
   {
      if( pTag->Owner->Owner->fShared && !pTag->Owner->Locked )
      {
         while( !hb_fsLock( pTag->Owner->DiskFile, NTX_LOCK_OFFSET, 1, FL_LOCK ) );
         wasLocked = pTag->Owner->Locked;
         pTag->Owner->Locked = TRUE;
      }

      switch( bTypRead )
      {
         case TOP_RECORD:
            pTag->TagBOF = !hb_ntxTagGoToTopKey( pTag,NULL,0 );
            pTag->TagEOF = pTag->TagBOF;
            break;

         case BTTM_RECORD:
            pTag->TagEOF = !hb_ntxTagGoToBottomKey( pTag,NULL,0 );
            pTag->TagBOF = pTag->TagEOF;
            break;

         case NEXT_RECORD:
            pTag->TagEOF = !hb_ntxTagGoToNextKey( pTag, *lContinue );
            break;

         case PREV_RECORD:
            pTag->TagBOF = !hb_ntxTagGoToPrevKey( pTag, *lContinue );
            break;
      }
      if( pTag->Owner->Owner->fShared )
      {
         hb_ntxPageFree( pTag->RootPage,FALSE );
         // pTag->RootPage = NULL;
         if( !wasLocked )
         {
            hb_fsLock( pTag->Owner->DiskFile, NTX_LOCK_OFFSET, 1, FL_UNLOCK );
            pTag->Owner->Locked = FALSE;
         }
      }
   }
   else
   {
      if( bTypRead == PREV_RECORD)
      {
         pTag->TagBOF = TRUE; pTag->TagEOF = FALSE;
      }
      else
      {
         pTag->TagBOF = TRUE; pTag->TagEOF = TRUE;
      }
   }
   if( pTag->TagBOF || pTag->TagEOF )
   {
      if( !pTag->AscendKey )
      {
         pTag->CurKeyInfo->Xtra = pTag->TagEOF;
         pTag->TagEOF = pTag->TagBOF;
         pTag->TagBOF = ( BOOL ) pTag->CurKeyInfo->Xtra;
      }
      pTag->CurKeyInfo->Xtra = 0;
   }
   if( lContinue )
      *lContinue = TRUE;
}

static int hb_ntxItemCompare( char* s1, char* s2, int ilen1, int ilen2, BOOL Exact )
{
   int iLimit, iResult, i;

   if( ( s2 == NULL || ilen2 == 0 ) && s1 && ilen1 )
      return 1;
   if( s1 == NULL || ilen1 == 0 )
      return -1;

   iLimit = ( ilen1 > ilen2 ) ? ilen2 : ilen1;
   if( ( iResult = memcmp( s1, s2, iLimit ) ) == 0 )
   {
      if( ( iResult = ilen1 - ilen2 ) != 0 )
      {
         if( iResult > 0 )
         {
            i = ilen1;
            while(  i > iLimit && s1[ i - 1 ] == ' ' )  i--;
         }
         else
         {
            i = ilen2;
            while(  i > iLimit && s2[ i - 1 ] == ' ' )  i--;
         }
         if( i == iLimit )
            iResult = 0;
      }
      if( iResult < 0 && !Exact )
         iResult = 0;
   }

   if( iResult < 0 )
      return -1;
   else if( iResult > 0 )
      return 1;
   else
      return 0;
}

static void hb_ntxPageSave( LPPAGEINFO pPage )
{
   ( ( LPNTXBUFFER ) pPage->buffer )->item_count = pPage->uiKeys;
   hb_fsSeek( pPage->TagParent->Owner->DiskFile, pPage->Page, FS_SET );
   hb_fsWrite( pPage->TagParent->Owner->DiskFile, (unsigned char *) pPage->buffer, NTXBLOCKSIZE );
   pPage->Changed = FALSE;
}

static LPPAGEINFO hb_ntxPageLoad( LPNTXINDEX pIndex, ULONG ulOffset )
{
   LPPAGEINFO pPage;
   BOOL bReplace = FALSE;

   if( !ulOffset )
   {
      if( pIndex->Owner->fShared )
         hb_ntxHeaderRead( pIndex );
      ulOffset = pIndex->CompoundTag->RootBlock;
   }
   pPage = hb_ntxPageFind( pIndex, ulOffset );
   if( pPage )
   {
      pPage->lBusy = TRUE;
      return pPage;
   }
   if( pIndex->CompoundTag->uiPages > maxPagesPerTag || pIndex->Owner->fShared )
   {
      pPage = pIndex->CompoundTag->RootPage;
      while( pPage )
      {
      /*
         if( ( !pIndex->Owner->fShared && !pPage->lBusy ) ||
               ( pIndex->Owner->fShared && pPage->Page == -1 ) )
      */
         if( !pPage->lBusy )
           break;
         pPage = pPage->pNext;
      }
      if( pPage )
         bReplace = TRUE;
   }

   if( !bReplace )
   {
      LPPAGEINFO pLastPage;

      pPage = ( LPPAGEINFO ) hb_xgrab( sizeof( HB_PAGEINFO ) );
      memset( pPage , 0 ,sizeof( HB_PAGEINFO ) );
      pPage->buffer = ( char* ) hb_xgrab( NTXBLOCKSIZE );
      pPage->TagParent = pIndex->CompoundTag;
      pPage->Page = ulOffset;

      pLastPage = hb_ntxPageLast( pIndex );
      pPage->pPrev = pLastPage;
      if( pLastPage )
         pLastPage->pNext = pPage;
      else
         pIndex->CompoundTag->RootPage = pPage;

      pPage->pNext = NULL;
      pPage->TagParent->uiPages ++;
   }
   else
      pPage->Page = ulOffset;

   hb_fsSeek( pIndex->DiskFile, ulOffset, FS_SET );
   if( hb_fsRead( pIndex->DiskFile, (unsigned char *) pPage->buffer, NTXBLOCKSIZE )
            != NTXBLOCKSIZE )
      return NULL;

   pPage->CurKey = -1;
   pPage->lBusy = TRUE;

   pPage->uiKeys = * (USHORT*)pPage->buffer;

   return pPage;
}

static void hb_ntxPageRelease( LPPAGEINFO pPage )
{

   pPage->lBusy = FALSE;
   if( pPage->Changed )
      hb_ntxPageSave( pPage );
   if( pPage->NewRoot )
   {
      pPage->TagParent->RootBlock = pPage->Page;
      hb_ntxHeaderSave( pPage->TagParent->Owner, FALSE );
      pPage->NewRoot = FALSE;
   }
}

static void hb_ntxPageFree( LPPAGEINFO pPage, BOOL lFull )
{

   if( pPage->pNext )
   {
      hb_ntxPageFree( pPage->pNext,lFull );
   }
   if( pPage->Changed )
      hb_ntxPageSave( pPage );
   if( pPage->NewRoot )
   {
      pPage->TagParent->RootBlock = pPage->Page;
      hb_ntxHeaderSave( pPage->TagParent->Owner, FALSE );
      pPage->NewRoot = FALSE;
   }
   if( lFull )
   {
      if( pPage->pPrev )
         pPage->pPrev->pNext = NULL;
      pPage->pNext = NULL;
      if( pPage->buffer )
      {
         pPage->TagParent->uiPages --;
         hb_xfree( pPage->buffer );
         pPage->buffer = NULL;
         hb_xfree( pPage );
      }
   }
   else
      pPage->Page = -1;
   /*
   else
   {
      if( pPage->pPrev )
      {
         pPage->pPrev->pNext = pPage->pNext;
         if( pPage->pNext )
            pPage->pNext->pPrev = pPage->pPrev;
      }
      else
      {
         pPage->TagParent->RootPage = pPage->pNext;
         if( pPage->pNext )
            pPage->pNext->pPrev = NULL;
      }
      pPage->pNext = NULL;
   }
   */

}

static LPPAGEINFO hb_ntxPageNew( LPTAGINFO pParentTag )
{
   LPPAGEINFO pPage, pLastPage;

   if( pParentTag->Owner->NextAvail > 0 )
   {
      /* Handling of a pool of empty pages.
         Some sources says that this address is in the first 4 bytes of
         a page ( http://www.e-bachmann.dk/docs/xbase.htm ).
         But as I understood, studying dumps of Clipper ntx'es, address of the
         next available page is in the address field of a first key item
         in the page - it is done here now in such a way.
         = Alexander Kresin =
      */
      pPage = hb_ntxPageLoad( pParentTag->Owner,pParentTag->Owner->NextAvail );
      pPage->Page = pParentTag->Owner->NextAvail;
      pParentTag->Owner->NextAvail = KEYITEM( pPage, 0 )->page;
      hb_ntxHeaderSave( pParentTag->Owner, FALSE );
   }
   else
   {
      int i;
      LPNTXBUFFER itemlist;
      BOOL bReplace = FALSE;

      if( pParentTag->uiPages > maxPagesPerTag )
      {
         pPage = pParentTag->RootPage;
         while( pPage )
         {
            if( !pPage->lBusy )
              break;
            pPage = pPage->pNext;
         }
         if( pPage )
            bReplace = TRUE;
      }

      if( bReplace )
      {
         pPage->uiKeys = 0;
      }
      else
      {
         pPage = ( LPPAGEINFO ) hb_xgrab( sizeof( HB_PAGEINFO ) );
         memset( pPage, 0, sizeof( HB_PAGEINFO ) );
         pPage->TagParent = pParentTag;
         pLastPage = hb_ntxPageLast( pParentTag->Owner );
         if( pLastPage )
         {
            pPage->pPrev = pLastPage;
            pLastPage->pNext = pPage;
         }
         else
         {
            pPage->pPrev = NULL;
            pParentTag->RootPage = pPage;
         }
         pParentTag->uiPages ++;
         pPage->buffer = ( char* ) hb_xgrab( NTXBLOCKSIZE );
      }
      memset( pPage->buffer, 0, NTXBLOCKSIZE );
      itemlist = ( LPNTXBUFFER ) pPage->buffer;
      for( i = 0; i < pParentTag->MaxKeys+1; i++ )
         itemlist->item_offset[i] = 2 + 2 * ( pParentTag->MaxKeys + 1 ) +
               i * ( pParentTag->KeyLength + 8 );
      pParentTag->TagBlock += 1024;
      pPage->Page = pParentTag->TagBlock;
      pPage->CurKey = -1;
      pPage->lBusy = TRUE;
   }

   return pPage;
}

static void hb_ntxKeysMove( LPPAGEINFO pPageDest, LPPAGEINFO pPageSrc, USHORT iDest, USHORT iSrc, USHORT iKeys )
{
   int i, keyLength = pPageDest->TagParent->KeyLength + 8;
   for( i=0;i<iKeys;i++ )
      memmove( pPageDest->buffer + *((USHORT*)(pPageDest->buffer+(i+iDest)*2+2)),
        pPageSrc->buffer+ *((USHORT*)(pPageSrc->buffer+(i+iSrc)*2+2)), keyLength );
}

static BOOL hb_ntxPageJoin( LPPAGEINFO pPageParent, LPPAGEINFO pPage1, LPPAGEINFO pPage2, SHORT ikey1, SHORT ikey2 )
{
   int i;
   USHORT ntmp;

   KEYITEM( pPage1, pPage1->uiKeys )->rec_no = KEYITEM( pPageParent, ikey1 )->rec_no;
   strcpy( KEYITEM( pPage1, pPage1->uiKeys )->key,KEYITEM( pPageParent, ikey1 )->key );
   KEYITEM( pPageParent, ikey2 )->page = pPage1->Page;

   hb_ntxKeysMove( pPage1, pPage2, pPage1->uiKeys+1, 0, pPage2->uiKeys+1 );

   pPage1->uiKeys += pPage2->uiKeys + 1;
   pPage1->Changed = TRUE;
   pPage2->uiKeys = 0;
   pPage2->Changed = TRUE;
   KEYITEM( pPage2, 0 )->page = pPage2->TagParent->Owner->NextAvail;
   pPage2->TagParent->Owner->NextAvail = pPage2->Page;
   hb_ntxHeaderSave( pPage2->TagParent->Owner, FALSE );

   hb_ntxPageRelease( pPage1 );
   hb_ntxPageRelease( pPage2 );

   ntmp = *KEYPOINTER( pPageParent,ikey1 );
   for( i=ikey1; i<=pPageParent->uiKeys; i++ )
      *KEYPOINTER( pPageParent,i ) = *KEYPOINTER( pPageParent,i+1 );
   *KEYPOINTER( pPageParent,pPageParent->uiKeys ) = ntmp;
   pPageParent->uiKeys--;
   pPageParent->Changed = TRUE;
   if( pPageParent->uiKeys < pPage2->TagParent->MaxKeys/2 )
      return FALSE;

   return TRUE;

}

static void hb_ntxPageBalance( LPPAGEINFO pPageParent, LPPAGEINFO pPage1, LPPAGEINFO pPage2, SHORT ikey1, SHORT ikey2 )
{
   int i;
   USHORT ntmp;
   int nKeys = ( pPage1->uiKeys + pPage2->uiKeys ) / 2 - pPage1->uiKeys;

   if( ikey2 > ikey1 )
   {
      /* printf( "\nntxPageBalance - 10 %d %d %d ",pPage1->uiKeys,pPage2->uiKeys,nKeys ); */
      KEYITEM( pPage1, pPage1->uiKeys )->rec_no = KEYITEM( pPageParent, ikey1 )->rec_no;
      strcpy( KEYITEM( pPage1, pPage1->uiKeys )->key,KEYITEM( pPageParent, ikey1 )->key );

      hb_ntxKeysMove( pPage1, pPage2, pPage1->uiKeys+1, 0, nKeys-1 );

      KEYITEM( pPageParent, ikey1 )->rec_no = KEYITEM( pPage2, nKeys-1 )->rec_no;
      strcpy( KEYITEM( pPageParent, ikey1 )->key, KEYITEM( pPage2, nKeys-1 )->key );
      KEYITEM( pPage1, pPage1->uiKeys+nKeys )->page = KEYITEM( pPage2, nKeys-1 )->page;

      pPage2->uiKeys -= nKeys;
      for( i=0; i<=pPage2->uiKeys; i++ )
      {
         ntmp = *KEYPOINTER( pPage2,i );
         *KEYPOINTER( pPage2,i ) = *KEYPOINTER( pPage2,i+nKeys );
         *KEYPOINTER( pPage2,i+nKeys ) = ntmp;
      }
   }
   else
   {
      /* printf( "\nntxPageBalance - 20 %d",nKeys ); */
      for( i=pPage1->uiKeys; i>=0; i-- )
      {
         ntmp = *KEYPOINTER( pPage1,i+nKeys );
         *KEYPOINTER( pPage1,i+nKeys ) = *KEYPOINTER( pPage1,i );
         *KEYPOINTER( pPage1,i ) = ntmp;
      }

      KEYITEM( pPage1, nKeys-1 )->rec_no = KEYITEM( pPageParent, ikey2 )->rec_no;
      strcpy( KEYITEM( pPage1, nKeys-1 )->key,KEYITEM( pPageParent, ikey2 )->key );
      KEYITEM( pPage1, nKeys-1 )->page = KEYITEM( pPage2, pPage2->uiKeys )->page;

      pPage2->uiKeys -= nKeys;
      hb_ntxKeysMove( pPage1, pPage2, 0, pPage2->uiKeys+1, nKeys-1 );

      KEYITEM( pPageParent, ikey2 )->rec_no = KEYITEM( pPage2, pPage2->uiKeys )->rec_no;
      strcpy( KEYITEM( pPageParent, ikey2 )->key, KEYITEM( pPage2, pPage2->uiKeys )->key );
   }
   pPage1->uiKeys += nKeys;
   pPage1->Changed = TRUE;
   pPage2->Changed = TRUE;
   pPageParent->Changed = TRUE;
   hb_ntxPageRelease( pPage1 );
   hb_ntxPageRelease( pPage2 );
   hb_ntxPageRelease( pPageParent );
}

static void hb_ntxTagBalance( LPTAGINFO pTag, int level )
{

   if( pTag->stackLevel > level )
   {
      SHORT thiskey,pairkey;
      LPPAGEINFO pPagePair;
      LPPAGEINFO pPage = hb_ntxPageLoad( pTag->Owner, pTag->stack[level].page );
      LPPAGEINFO pPageParent = hb_ntxPageLoad( pTag->Owner, pTag->stack[level+1].page );

      /* printf( "\nntxTagBalance-1, %d %d %x %x",pTag->stackLevel,level,pPageParent->Page,pPage->Page ); */
      thiskey = pairkey = pTag->stack[level+1].ikey;
      do
         pairkey ++;
      while( pairkey <= pPageParent->uiKeys && !KEYITEM( pPageParent, pairkey )->page );
      if( pairkey > pPageParent->uiKeys && thiskey >= pPageParent->uiKeys-1 )
      {
        pairkey = thiskey;
        do
           pairkey --;
        while( pairkey >= 0 && !(KEYITEM( pPageParent, pairkey )->page) );
        if( pairkey < 0 && thiskey == 0 )
        {
           hb_ntxPageRelease( pPageParent );
           return;
        }
        if( thiskey - pairkey > 1 )
        {
           hb_ntxPageRelease( pPageParent );
        }
        else
        {
           pPagePair = hb_ntxPageLoad( pTag->Owner, KEYITEM( pPageParent, pairkey )->page );
           if( pPageParent->uiKeys == 1 && 
                 ( pPage->uiKeys + pPagePair->uiKeys ) > pTag->MaxKeys - 2 )
           {
              hb_ntxPageRelease( pPageParent );
              hb_ntxPageRelease( pPagePair );
              return;
           }
             
           if( pPagePair->uiKeys <= pTag->MaxKeys/2 )
           {
              /* printf( "\nntxTagBalance - 10, %d %d",thiskey,pairkey ); */
              if( !hb_ntxPageJoin( pPageParent,pPagePair,pPage,pairkey,thiskey ) )
                 hb_ntxTagBalance( pTag, level+1 );
              hb_ntxPageRelease( pPageParent );
           }
           else
           {
              /* printf( "\nntxTagBalance - 20, %d %d",thiskey,pairkey ); */
              hb_ntxPageBalance( pPageParent, pPage,pPagePair,thiskey,pairkey );
           }
        }
      }
      else
      {
         if( pairkey - thiskey > 1 )
         {
           hb_ntxPageRelease( pPageParent );
         }
         else
         {
           pPagePair = hb_ntxPageLoad( pTag->Owner, KEYITEM( pPageParent, pairkey )->page );
           if( pPageParent->uiKeys == 1 && 
                 ( pPage->uiKeys + pPagePair->uiKeys ) > pTag->MaxKeys - 2 )
           {
              hb_ntxPageRelease( pPageParent );
              hb_ntxPageRelease( pPagePair );
              return;
           }
           /* printf( "\nntxTagBalance - 3A %x %x %x %d",pPageParent->Page,pPage->Page,pPagePair->Page,pPagePair->uiKeys ); */
           if( pPagePair->uiKeys <= pTag->MaxKeys/2 )
           {
              /* printf( "\nntxTagBalance - 30, %d %d",thiskey,pairkey ); */
              if( !hb_ntxPageJoin( pPageParent,pPage,pPagePair,thiskey,pairkey ) )
                 hb_ntxTagBalance( pTag, level+1 );
              hb_ntxPageRelease( pPageParent );
           }
           else
           {
              /* printf( "\nntxTagBalance - 40, %d %d",thiskey,pairkey ); */
              hb_ntxPageBalance( pPageParent, pPage,pPagePair,thiskey,pairkey );
           }
         }
      }
   }
}

static void hb_ntxPageKeyInsert( LPPAGEINFO pPage, LPKEYINFO pKey, int pos )
{
   int i;
   USHORT ntmp;

   ntmp = *KEYPOINTER( pPage,pPage->uiKeys+1 );

   for( i=pPage->uiKeys; i>=pos; i-- )
      *KEYPOINTER( pPage,i+1 ) = *KEYPOINTER( pPage,i );

   *KEYPOINTER( pPage,pos ) = ntmp;

   pPage->uiKeys++;
   pPage->Changed = TRUE;
   KEYITEM( pPage, pos )->rec_no = pKey->Xtra;
   strcpy( KEYITEM( pPage, pos )->key, pKey->key );
   KEYITEM( pPage, pos )->page = pKey->Tag;
}

static LPKEYINFO hb_ntxPageKeyDel( LPPAGEINFO pPage, SHORT pos, USHORT level )
{
   int i;
   LPKEYINFO pKey = NULL;
   LPTAGINFO pTag = pPage->TagParent;

   if( KEYITEM( pPage, pos )->page )
   {
      LPPAGEINFO pPageChild = hb_ntxPageLoad( pTag->Owner,KEYITEM( pPage, pos )->page );
      LPKEYINFO pKeyNew = NULL;
      SHORT CurKey = ( KEYITEM( pPageChild, pPageChild->uiKeys )->page )? 
                           pPageChild->uiKeys : pPageChild->uiKeys-1;

      /* printf( "\nhb_ntxPageKeyDel-1 %d %d %x",level,pos,pPage->Page ); */
      for( i=pTag->stackLevel; i>=0; i-- )
      {
         pTag->stack[i+1].page = pTag->stack[i].page;
         pTag->stack[i+1].ikey = pTag->stack[i].ikey;
      }
      if( ++(pTag->stackLevel) >= pTag->stackDepth )
      {
         pTag->stackDepth += 32;
         pTag->stack = (LPTREESTACK) hb_xrealloc( pTag->stack,
                sizeof(TREE_STACK) * pTag->stackDepth );
      }
      pTag->stack[0].page = pPageChild->Page;
      pTag->stack[0].ikey = CurKey;

      pKey = hb_ntxPageKeyDel( pPageChild, CurKey,level+1 );
      hb_ntxPageRelease( pPageChild );
      if( level > 1 )
      {
         if( pos == pPage->uiKeys )
         {
            pKey->Tag = pPage->Page;
            /* printf( "\nhb_ntxPageKeyDel-A %d %x",pKey->Xtra,pPage->Page ); */
            return pKey;
         }
         else
         {
            pKeyNew = hb_ntxKeyNew( NULL,pTag->KeyLength );
            strcpy( pKeyNew->key, KEYITEM( pPage, pos )->key );
            pKeyNew->Xtra = KEYITEM( pPage, pos )->rec_no;
            pKeyNew->Tag = pPage->Page;
            /* printf( "\nhb_ntxPageKeyDel-B %d %x",pKeyNew->Xtra,pPage->Page ); */
         }
      }
      strcpy( KEYITEM( pPage, pos )->key, pKey->key );
      KEYITEM( pPage, pos )->page = pKey->Tag;
      KEYITEM( pPage, pos )->rec_no = pKey->Xtra;
      /* printf( "\nhb_ntxPageKeyDel-C %d %x",pKey->Xtra,pKey->Tag ); */
      pPage->Changed = TRUE;
      hb_ntxKeyFree( pKey );
      return pKeyNew;
   }
   else
   {
      /* printf( "\nhb_ntxPageKeyDel-2 %d %d %x",level,pos,pPage->Page ); */
      if( level > 1 )
      {
         pKey = hb_ntxKeyNew( NULL,pTag->KeyLength );
         strcpy( pKey->key, KEYITEM( pPage, pos )->key );
         pKey->Xtra = KEYITEM( pPage, pos )->rec_no;
         pKey->Tag = pPage->Page;
      }
      else
      {
         USHORT ntmp = *KEYPOINTER( pPage,pos );
         for( i=pos; i<=pPage->uiKeys; i++ )
            *KEYPOINTER( pPage,i ) = *KEYPOINTER( pPage,i+1 );
         *KEYPOINTER( pPage,pPage->uiKeys ) = ntmp;
      }
      pPage->uiKeys--;
      pPage->Changed = TRUE;
      if( pPage->uiKeys < pTag->MaxKeys/2 )
         pTag->stack[0].ikey = -1;
      /*
      if( !pPage->uiKeys )
      {
         if( pKey )
            pKey->Tag = 0;
         KEYITEM( pPage, 0 )->page = pTag->Owner->NextAvail;
         // pPage->pKeys->Tag = pTag->Owner->NextAvail;
         pTag->Owner->NextAvail = pPage->Page;
         hb_ntxHeaderSave( pTag->Owner, FALSE );
      }
      */
      return pKey;
   }
}

static LPKEYINFO hb_ntxPageSplit( LPPAGEINFO pPage, LPKEYINFO pKey, int pos )
{
   LPTAGINFO pTag = pPage->TagParent;
   LPPAGEINFO pNewPage = hb_ntxPageNew( pTag );
   LPKEYINFO pKeyNew = hb_ntxKeyNew( NULL, pTag->KeyLength );
   int i, j, iHalf = pPage->uiKeys/2;
   USHORT ntmp;

   if( pos < iHalf+1 )
   {
      /* Move to the new page the keys, less than inserted */
      if( pos )
         hb_ntxKeysMove( pNewPage, pPage, 0, 0, pos );
      if( pos < iHalf )
      {
         /* Copy the inserted key  */
         KEYITEM( pNewPage, pos )->rec_no = pKey->Xtra;
         strcpy( KEYITEM( pNewPage, pos )->key, pKey->key );
         KEYITEM( pNewPage, pos )->page = pKey->Tag;
         /* Move to the new page the keys, greater than inserted */
         hb_ntxKeysMove( pNewPage, pPage, pos+1, pos, iHalf-pos-1 );
         /* Create key for the parent page  */
         pKeyNew->Xtra = KEYITEM( pPage,iHalf-1 )->rec_no;
         pKeyNew->Tag = pNewPage->Page;
         strcpy( pKeyNew->key, KEYITEM( pPage,iHalf-1 )->key );

         KEYITEM( pNewPage,iHalf )->page = KEYITEM( pPage,iHalf-1 )->page;
      }
      else
      {
         /* Create key for the parent page  */
         pKeyNew->Xtra = pKey->Xtra;
         pKeyNew->Tag = pNewPage->Page;
         strcpy( pKeyNew->key, pKey->key );

         KEYITEM( pNewPage,iHalf )->page = pKey->Tag;
      }

      /* Move second half of a page to the moved first half */
      for( i=0,j=iHalf; j<=pPage->uiKeys; i++,j++ )
      {
         ntmp = *KEYPOINTER( pPage,i );
         *KEYPOINTER( pPage,i ) = *KEYPOINTER( pPage,j );
         *KEYPOINTER( pPage,j ) = ntmp;
      }
      pPage->uiKeys -= iHalf;
   }
   else
   {
      /* Move to the new page the first half */
      hb_ntxKeysMove( pNewPage, pPage, 0, 0, iHalf );
      /* Create key for the parent page  */
      pKeyNew->Xtra = KEYITEM( pPage,iHalf )->rec_no;
      pKeyNew->Tag = pNewPage->Page;
      strcpy( pKeyNew->key, KEYITEM( pPage,iHalf )->key );
      KEYITEM( pNewPage,iHalf )->page = KEYITEM( pPage,iHalf )->page;

      /* Move second half of a page to the moved first half */
      for( i=0,j=iHalf+1; j<=pPage->uiKeys; i++,j++ )
      {
         ntmp = *KEYPOINTER( pPage,i );
         *KEYPOINTER( pPage,i ) = *KEYPOINTER( pPage,j );
         *KEYPOINTER( pPage,j ) = ntmp;
      }
      pPage->uiKeys -= (iHalf+1);
      /* Insert new key */
      hb_ntxPageKeyInsert( pPage, pKey, pos-iHalf-1 );
   }
   pNewPage->uiKeys = iHalf;
   pPage->Changed = pNewPage->Changed = TRUE;
   return pKeyNew;
}

static LPKEYINFO hb_ntxPageKeyAdd( LPPAGEINFO pPage, LPKEYINFO pKey )
{
   LPTAGINFO pTag = pPage->TagParent;

   hb_ntxPageKeySearch( pPage, pKey->key, TRUE, TRUE );

   if( KEYITEM( pPage, pPage->CurKey )->page != 0 )
   {
      LPKEYINFO pKeyFromChild, pKeyReturn = NULL;
      LPPAGEINFO pChildPage = hb_ntxPageLoad( pTag->Owner,
                                  KEYITEM( pPage, pPage->CurKey )->page );
      pKeyFromChild = hb_ntxPageKeyAdd( pChildPage, pKey );
      hb_ntxPageRelease( pChildPage );
      if( pKeyFromChild )
      {
         if( pPage->uiKeys == pTag->MaxKeys )
            pKeyReturn = hb_ntxPageSplit( pPage, pKeyFromChild, pPage->CurKey );
         else
            hb_ntxPageKeyInsert( pPage, pKeyFromChild, pPage->CurKey );

         hb_ntxKeyFree( pKeyFromChild );
      }
      return pKeyReturn;
   }
   else if( pPage->uiKeys == pTag->MaxKeys )
   {
      return hb_ntxPageSplit( pPage, pKey, pPage->CurKey );
   }
   else
   {
      hb_ntxPageKeyInsert( pPage, pKey, pPage->CurKey );
      return NULL;
   }
}

static void hb_ntxTagKeyAdd( LPTAGINFO pTag, LPKEYINFO pKey )
{

   LPPAGEINFO pPage = hb_ntxPageLoad( pTag->Owner,0 );

   if( pPage->uiKeys == 0 )
   {
      pPage->uiKeys = 1;
      KEYITEM( pPage, 0 )->rec_no = pKey->Xtra;
      strcpy( KEYITEM( pPage, 0 )->key, pKey->key );
      pPage->Changed = TRUE;
   }
   else
   {
      LPKEYINFO pKeyFromChild = hb_ntxPageKeyAdd( pPage, pKey );

      if( pKeyFromChild )
      {
         LPPAGEINFO pNewPage = hb_ntxPageNew( pTag );

         pNewPage->uiKeys = 1;
         KEYITEM( pNewPage, 0 )->rec_no = pKeyFromChild->Xtra;
         KEYITEM( pNewPage, 0 )->page = pKeyFromChild->Tag;
         strcpy( KEYITEM( pNewPage, 0 )->key, pKeyFromChild->key );
         KEYITEM( pNewPage, 1 )->page = pPage->Page;

         pNewPage->Changed = pNewPage->NewRoot = TRUE;
         hb_ntxPageRelease( pNewPage );
         hb_ntxKeyFree( pKeyFromChild );
      }
   }
   hb_ntxPageRelease( pPage );
}

typedef struct _SWAPPAGE
{
   ULONG    offset;
   USHORT   numAllkeys;
   USHORT   numReadkeys;
   USHORT   numkeys;
   short int curkey;
   char     page[ 512 ];
} SWAPPAGE;

typedef SWAPPAGE * LPSWAPPAGE;

typedef struct _PAGEITEM
{
   ULONG    rec_no;
   char     key[ 1 ];
} PAGEITEM;

typedef PAGEITEM * LPPAGEITEM;

struct _SORTITEM;
typedef struct _SORTITEM
{
   struct _SORTITEM *  pNext;
   ULONG    rec_no;
   char     key[ 1 ];
} SORTITEM;

typedef SORTITEM * LPSORTITEM;

typedef struct _NTXSORTINFO
{
   ULONG       Tag;
   ULONG       ulKeyCount;
   ULONG       ulSqrt;
   ULONG       nItems;
   USHORT      itemLength;
   USHORT      nSwappages;
   BYTE *      sortBuffer;
   LPSORTITEM  pKeyFirst;
   LPSORTITEM  pKeyTemp;
   LPSORTITEM  pKey1;
   LPSORTITEM  pKey2;
   char**      pageBuffers;
   BYTE*       swappages;
   FHANDLE     tempHandle;
} NTXSORTINFO;

typedef NTXSORTINFO * LPNTXSORTINFO;

static void hb_ntxSwapPageSave( LPTAGINFO pTag, LPNTXSORTINFO pSortInfo, USHORT nPart )
{
   LPSORTITEM pKey = pSortInfo->pKeyFirst;
   USHORT itemLength = sizeof( ULONG ) + pTag->KeyLength;
   USHORT numKeys = 0, maxKeys = 512/itemLength, numAllkeys = 0;
   LPSWAPPAGE pSwapPage = (LPSWAPPAGE) ( pSortInfo->swappages + sizeof(SWAPPAGE)*nPart );
   LPPAGEITEM ptr;

   pSwapPage->offset = hb_fsSeek( pSortInfo->tempHandle, 0, SEEK_END );
   pSwapPage->numReadkeys = pSwapPage->curkey = pSwapPage->numkeys = 0;
   while( pKey )
   {
      ptr = (LPPAGEITEM) ( pSwapPage->page + numKeys*itemLength );
      ptr->rec_no = pKey->rec_no;
      memcpy( ptr->key, pKey->key, pTag->KeyLength );
      pKey = pKey->pNext;
      numAllkeys ++;
      if( ++numKeys == maxKeys || !pKey )
      {
         hb_fsWrite( pSortInfo->tempHandle, (BYTE *) pSwapPage->page,
                numKeys * itemLength );
         numKeys = 0;
      }
   }
   pSwapPage->numAllkeys = numAllkeys;
}

static void hb_ntxKeysSort( LPNTXSORTINFO pSortInfo, LPSORTITEM* pKeyFirst, LPSORTITEM pKeyNew, int KeyLength, BOOL fDescend, BOOL fUnique )
{
   LPSORTITEM pKey, pKeyTmp, pKeyLast = NULL, pKeyPrev;
   int result;

   while( pKeyNew )
   {
      pKeyPrev = NULL;
      pKeyTmp = pKeyNew->pNext;
      pKeyNew->pNext = NULL;

      if( pKeyLast )
      {
         pKeyPrev = pKeyLast;
         pKey = pKeyLast->pNext;
      }
      else if( pSortInfo->pKey1 )
      {
         result = memcmp( pKeyNew->key, pSortInfo->pKey1->key, KeyLength );
         if( fDescend && result )
            result = ( result > 0 )? -1:1;
         if( result >= 0 )
         {
            if( !result && fUnique )
            {
               pSortInfo->ulKeyCount --;
               pKeyNew = pKeyTmp;
               continue;
            }
            else
            {
               pKeyPrev = pSortInfo->pKey1;
               pKey = pSortInfo->pKey1->pNext;
            }
         }
         else
            pKey = *pKeyFirst;
      }
      else
         pKey = *pKeyFirst;
      while( pKey )
      {
         result = memcmp( pKeyNew->key, pKey->key, KeyLength );
         if( fDescend && result )
            result = ( result > 0 )? -1:1;
         if( result < 0 )
         {
            pKeyNew->pNext = pKey;
            if( pKeyPrev )
            {
               pKeyPrev->pNext = pKeyNew;
               pSortInfo->pKey1 = pKeyNew;
            }
            else
               *pKeyFirst = pKeyNew;
            break;
         }
         else if( !result  && fUnique )
         {
            pSortInfo->ulKeyCount --;
            pKeyNew = pKeyLast;
            break;
         }
         pKeyPrev = pKey;
         pKey = pKey->pNext;
      }
      if( !pKey )
      {
         pKeyPrev->pNext = pKeyNew;
         pSortInfo->pKey1 = pKeyNew;
      }

      pKeyLast = pKeyNew;
      pKeyNew = pKeyTmp;
   }
}

static void hb_ntxSortKeyAdd( LPTAGINFO pTag, LPNTXSORTINFO pSortInfo, char* szkey, ULONG ulKeyNo )
{
   LPSORTITEM pKeyNew = (LPSORTITEM) ( pSortInfo->sortBuffer +
                             pSortInfo->itemLength * ( ulKeyNo - 1 ) );
   pKeyNew->rec_no = pTag->Owner->Owner->ulRecNo;
   pKeyNew->pNext = NULL;
   memcpy( pKeyNew->key, szkey, pTag->KeyLength );

   if( ++(pSortInfo->nItems) < 2 )
   {
      pSortInfo->pKeyTemp = pKeyNew;
   }
   else
   {
      hb_ntxKeysSort( pSortInfo, &(pSortInfo->pKeyTemp), pKeyNew, pTag->KeyLength, !pTag->AscendKey, pTag->UniqueKey );

      if( pSortInfo->nItems == pSortInfo->ulSqrt )
      {
         if( !pSortInfo->pKeyFirst )
         {
            pSortInfo->pKeyFirst = pSortInfo->pKeyTemp;
            pSortInfo->pKey2 = pSortInfo->pKey1;
         }
         else
         {
            pSortInfo->pKey1 = pSortInfo->pKey2;
            hb_ntxKeysSort( pSortInfo, &(pSortInfo->pKeyFirst), pSortInfo->pKeyTemp, pTag->KeyLength, !pTag->AscendKey, pTag->UniqueKey );
            pSortInfo->pKey2 = pSortInfo->pKey1;
         }
         pSortInfo->nItems = 0;
      }
   }
/*
   {
      int i = 0;
      char ctmp[30];
      ctmp[ pTag->KeyLength ] = 0;
      printf( "\n\r------------------" );
      for( pKey = pSortInfo->pKeyFirst; pKey ; pKey = pKey->pNext,i++ )
      {
         memcpy( ctmp,pKey->key,pTag->KeyLength );
         printf( "\n\r%d %s",i,ctmp );
      }
   }
*/
}

static void hb_ntxSortKeyEnd( LPTAGINFO pTag, LPNTXSORTINFO pSortInfo )
{
   if( pSortInfo->nItems )
   {
      if( !pSortInfo->pKeyFirst )
         pSortInfo->pKeyFirst = pSortInfo->pKeyTemp;
      else
      {
         pSortInfo->pKey1 = pSortInfo->pKey2;
         hb_ntxKeysSort( pSortInfo, &(pSortInfo->pKeyFirst), pSortInfo->pKeyTemp, pTag->KeyLength, !pTag->AscendKey, pTag->UniqueKey );
      }
      pSortInfo->nItems = 0;
   }
}

static void hb_ntxRootPage( LPTAGINFO pTag, LPNTXSORTINFO pSortInfo, LPSORTITEM pKey, ULONG* lpArray, USHORT level )
{
   USHORT i, maxKeys = lpArray[level+1];
   LPNTXBUFFER itemlist;
   LPNTXITEM item;

   if( !pSortInfo->pageBuffers[ level ] )
   {
      if( !pKey )
         return;
      pSortInfo->pageBuffers[ level ] = (char*) hb_xgrab( NTXBLOCKSIZE );
      memset( pSortInfo->pageBuffers[ level ], 0, NTXBLOCKSIZE );
      itemlist = ( LPNTXBUFFER ) pSortInfo->pageBuffers[ level ];
      for( i = 0; i < pTag->MaxKeys+1; i++ )
         itemlist->item_offset[i] = 2 + 2 * ( pTag->MaxKeys + 1 ) +
               i * ( pTag->KeyLength + 8 );
   }
   else
      itemlist = ( LPNTXBUFFER ) pSortInfo->pageBuffers[ level ];
   item = (NTXITEM *)( pSortInfo->pageBuffers[ level ] + itemlist->item_offset[itemlist->item_count] );
      item->page = pSortInfo->Tag;
   if( itemlist->item_count < maxKeys && pKey )
   {
      item->rec_no = pKey->rec_no;
      memcpy( item->key, pKey->key, pTag->KeyLength );
      itemlist->item_count++;
   }
   else
   {
      if( pKey || itemlist->item_count > 0 )
      {
         pSortInfo->Tag += NTXBLOCKSIZE;
         hb_fsWrite( pTag->Owner->DiskFile, (BYTE *) pSortInfo->pageBuffers[ level ], NTXBLOCKSIZE );
         itemlist->item_count = 0;
         memset( pSortInfo->pageBuffers[ level ] + 4 + 2 * ( pTag->MaxKeys + 1 ),
            0, NTXBLOCKSIZE - 6 - 2 * ( pTag->MaxKeys + 1 ) );
      }
      if( !pKey )
         pTag->RootBlock = pSortInfo->Tag;
      hb_ntxRootPage( pTag, pSortInfo, pKey, lpArray, level+1 );
   }
}

static BOOL hb_ntxGetSortedKey( LPTAGINFO pTag, LPNTXSORTINFO pSortInfo, LPSORTITEM* ppKey, LPSORTITEM pKeyRoot )
{
   char *key1, *key2;
   short int nPage, iPage;
   int result;
   BOOL fDescend = !pTag->AscendKey;
   USHORT itemLength = sizeof( ULONG ) + pTag->KeyLength;
   LPSORTITEM pKey = *ppKey;
   LPSWAPPAGE pSwapPage = (LPSWAPPAGE) ( pSortInfo->swappages );

   if( pSwapPage->curkey >= 0 )
      key1 = ( (LPPAGEITEM) ( pSwapPage->page +
                 itemLength*pSwapPage->curkey ) )->key;
   else
      key1 = NULL;
   nPage = 0;
   for( iPage=1;iPage<pSortInfo->nSwappages;iPage++ )
   {
      pSwapPage = (LPSWAPPAGE) ( pSortInfo->swappages + sizeof(SWAPPAGE)*iPage );
      if( pSwapPage->curkey >= 0 )
      {
         key2 = ( (LPPAGEITEM) ( pSwapPage->page +
                       itemLength*pSwapPage->curkey ) )->key;
         if( key1 )
         {
            result = memcmp( (BYTE*)key1,(BYTE*)key2,pTag->KeyLength );
            if( fDescend && result )
               result = ( result > 0 )? -1:1;
         }
         if( !key1 || result > 0 )
         {
            nPage = iPage;
            key1 = key2;
         }
      }
   }
   if( pKey && key1 )
   {
      result = memcmp( (BYTE*)key1,(BYTE*)pKey->key,pTag->KeyLength );
      if( fDescend && result )
         result = ( result > 0 )? -1:1;
   }
   if( ( pKey && !key1 ) || ( pKey && result > 0 ) )
   {
      pKeyRoot->rec_no = pKey->rec_no;
      memcpy( pKeyRoot->key, pKey->key, pTag->KeyLength );
      *ppKey = pKey->pNext;
      /* printf( "\nSortedKey - 1 %d %c%c%c%c%c%c",*rec_no,key1[0],key1[1],key1[2],key1[3],key1[4],key1[5] ); */
   }
   else if( key1 )
   {
      LPSWAPPAGE pSwapPage = (LPSWAPPAGE) ( pSortInfo->swappages + sizeof(SWAPPAGE)*nPage );

      pKeyRoot->rec_no = ( (LPPAGEITEM) ( pSwapPage->page +
                    itemLength*pSwapPage->curkey ) )->rec_no;
      memcpy( pKeyRoot->key, key1, pTag->KeyLength );
      /* printf( "\nSortedKey - 2 %d %d %c%c%c%c%c%c",nPage,*rec_no,key1[0],key1[1],key1[2],key1[3],key1[4],key1[5] ); */
      if( ++(pSwapPage->curkey) == pSwapPage->numkeys )
      {
         USHORT pageItemLength = sizeof( ULONG ) + pTag->KeyLength;
         USHORT maxKeys = 512/pageItemLength, nRead;

         if( pSwapPage->numReadkeys >= pSwapPage->numAllkeys )
            pSwapPage->curkey = -1;
         else
         {
            hb_fsSeek( pSortInfo->tempHandle, pSwapPage->offset +
                 pageItemLength*pSwapPage->numReadkeys , SEEK_SET );
            nRead = hb_fsRead( pSortInfo->tempHandle, (BYTE*)pSwapPage->page,
                 ( (pSwapPage->numAllkeys-pSwapPage->numReadkeys < maxKeys)?
                    pSwapPage->numAllkeys-pSwapPage->numReadkeys:maxKeys ) * pageItemLength );
            pSwapPage->numkeys = nRead/pageItemLength;
            pSwapPage->numReadkeys += nRead/pageItemLength;
            pSwapPage->curkey = 0;
         }
      }
      /* printf( "\nSortedKey - 3 %d %d %c%c%c%c%c%c",nPage,*rec_no,key1[0],key1[1],key1[2],key1[3],key1[4],key1[5] ); */
   }
   else
      return FALSE;
   return TRUE;
}

static void hb_ntxBufferSave( LPTAGINFO pTag, LPNTXSORTINFO pSortInfo )
{
   USHORT i, maxKeys;
   LPNTXBUFFER itemlist;
   LPNTXITEM item;
   ULONG numKey = 0, ulFullNodes;
   LPSORTITEM pKey = pSortInfo->pKeyFirst;
   char* buffer;
   BOOL lSave = FALSE;
   ULONG* lpArray = hb_ntxKeysInPage( pSortInfo->ulKeyCount, pTag->MaxKeys );

   maxKeys = (USHORT)lpArray[1];
   ulFullNodes = lpArray[lpArray[0]+1];
   hb_fsSeek( pTag->Owner->DiskFile, 1024, FS_SET );
   pSortInfo->Tag = 0;
   pSortInfo->pageBuffers = (char**) hb_xgrab( sizeof( char* ) * 10 );
   for( i = 0; i < 10; i++ )
      pSortInfo->pageBuffers[i] = NULL;
   pSortInfo->pageBuffers[0] = (char*) hb_xgrab( NTXBLOCKSIZE );
   memset( pSortInfo->pageBuffers[0], 0, NTXBLOCKSIZE );
   itemlist = ( LPNTXBUFFER ) pSortInfo->pageBuffers[0];
   for( i = 0; i < pTag->MaxKeys+1; i++ )
      itemlist->item_offset[i] = 2 + 2 * ( pTag->MaxKeys + 1 ) +
            i * ( pTag->KeyLength + 8 );
   buffer = pSortInfo->pageBuffers[0];

   if( !pKey )
   {
      itemlist->item_count = 0;
      pSortInfo->Tag += NTXBLOCKSIZE;
      hb_fsWrite( pTag->Owner->DiskFile, (BYTE *) buffer, NTXBLOCKSIZE );
   }

   /* printf( "\nhb_ntxBufferSave - 1 ( maxKeys=%d )",maxKeys ); */
   if( pSortInfo->nSwappages > 1 )
   {
      BOOL  lKeys;
      LPSORTITEM pKeyRoot = (LPSORTITEM) hb_xgrab( pSortInfo->itemLength );
      USHORT pageItemLength = sizeof( ULONG ) + pTag->KeyLength;
      USHORT maxKeysSwapPage = 512/pageItemLength, nRead;
      LPSWAPPAGE pSwapPage;

      for( i = 0; i < pSortInfo->nSwappages; i++ )
      {
         pSwapPage = (LPSWAPPAGE) ( pSortInfo->swappages + sizeof(SWAPPAGE)*i );
         hb_fsSeek( pSortInfo->tempHandle, pSwapPage->offset, SEEK_SET );
         nRead = hb_fsRead( pSortInfo->tempHandle, (BYTE*)pSwapPage->page,
            ( (pSwapPage->numAllkeys-pSwapPage->numReadkeys < maxKeysSwapPage)?
               pSwapPage->numAllkeys-pSwapPage->numReadkeys:maxKeysSwapPage ) * pageItemLength );
         pSwapPage->numkeys = nRead/pageItemLength;
         pSwapPage->numReadkeys = nRead/pageItemLength;
         pSwapPage->curkey = 0;
      }

      do
      {
         /* for( i = 0; i < maxKeys; i++ ) */
         for( i = 0; i < maxKeys || numKey == pSortInfo->ulKeyCount-1; i++ )
         {
            lKeys = hb_ntxGetSortedKey( pTag, pSortInfo, &pKey, pKeyRoot );
            if( !lKeys )
               break;
            numKey++;
            item = (NTXITEM *)( buffer + itemlist->item_offset[i] );
            item->page = 0;
            item->rec_no = pKeyRoot->rec_no;
            memcpy( item->key, pKeyRoot->key, pTag->KeyLength );
         }
         itemlist->item_count = i;

         if( itemlist->item_count )
         {
            pSortInfo->Tag += NTXBLOCKSIZE;
            hb_fsWrite( pTag->Owner->DiskFile, (BYTE *) buffer, NTXBLOCKSIZE );

            lKeys = hb_ntxGetSortedKey( pTag, pSortInfo, &pKey, pKeyRoot );
            if( lKeys )
            {
               hb_ntxRootPage( pTag, pSortInfo, pKeyRoot, lpArray, 1 );
               numKey++;
            }
            else
               hb_ntxRootPage( pTag, pSortInfo, NULL, lpArray, 1 );
            if( ulFullNodes )
            {
               ulFullNodes --;
               if( !ulFullNodes )
                  maxKeys --;
            }
         }
      }
      while( lKeys );
      hb_xfree( pKeyRoot );
   }
   else
   {
      while( pKey )
      {
         for( i = 0; ( i < maxKeys  || numKey == pSortInfo->ulKeyCount-1 ) && pKey > 0; i++, numKey++, pKey = pKey->pNext )
         {
            /* printf( "\nhb_ntxBufferSave - 2 ( i=%d maxKeys=%d )",i,maxKeys ); */
            item = (NTXITEM *)( buffer + itemlist->item_offset[i] );
            item->page = 0;
            item->rec_no = pKey->rec_no;
            memcpy( item->key, pKey->key, pTag->KeyLength );
         }
         itemlist->item_count = i;

         if( itemlist->item_count )
         {
            pSortInfo->Tag += NTXBLOCKSIZE;
            hb_fsWrite( pTag->Owner->DiskFile, (BYTE *) buffer, NTXBLOCKSIZE );
         }
         /* printf( "\nhb_ntxBufferSave - 5 ( numKey=%d )",numKey ); */
         hb_ntxRootPage( pTag, pSortInfo, pKey, lpArray, 1 );
         if( ulFullNodes )
         {
            ulFullNodes --;
            if( !ulFullNodes )
               maxKeys --;
         }
         if( pKey )
         {
            numKey ++;
            pKey = pKey->pNext;
         }
      }
   }
   hb_xfree( pSortInfo->pageBuffers[ 0 ] );
   for( i = 1; i < 10; i++ )
      if( pSortInfo->pageBuffers[ i ] )
      {
         itemlist = ( LPNTXBUFFER ) pSortInfo->pageBuffers[ i ];
         if( itemlist->item_count )
         {
            if( lSave )
            {
               item = (NTXITEM *)( pSortInfo->pageBuffers[ i ] + itemlist->item_offset[itemlist->item_count] );
               item->page = pSortInfo->Tag;
            }
            lSave = TRUE;
            pSortInfo->Tag += NTXBLOCKSIZE;
            hb_fsWrite( pTag->Owner->DiskFile, (BYTE *) pSortInfo->pageBuffers[ i ], NTXBLOCKSIZE );
            pTag->RootBlock = pSortInfo->Tag;
         }
         hb_xfree( pSortInfo->pageBuffers[ i ] );
      }
   hb_xfree( pSortInfo->pageBuffers );
   hb_xfree( lpArray );
   if( !pTag->RootBlock )
      pTag->RootBlock = 1024;
}

static BOOL hb_ntxReadBuf( NTXAREAP pArea, BYTE* readBuffer, USHORT* numRecinBuf, LPDBORDERCONDINFO lpdbOrdCondInfo )
{
   if( !lpdbOrdCondInfo || lpdbOrdCondInfo->fAll )
   {
      if( *numRecinBuf == 10 )
         *numRecinBuf = 0;
      if( *numRecinBuf == 0 )
         hb_fsReadLarge( pArea->hDataFile, readBuffer, pArea->uiRecordLen  * 10 );

      pArea->pRecord = readBuffer + (*numRecinBuf) * pArea->uiRecordLen;
      pArea->fDeleted = ( pArea->pRecord[ 0 ] == '*' );
      (*numRecinBuf) ++;
      return TRUE;
   }
   else
   {
      if( lpdbOrdCondInfo->lNextCount < 0 )
         return FALSE;
      if( lpdbOrdCondInfo->lRecno )
      {
         SELF_GOTO( ( AREAP ) pArea, (ULONG)lpdbOrdCondInfo->lRecno );
         lpdbOrdCondInfo->lNextCount = -1;
         return TRUE;
      }
      if( lpdbOrdCondInfo->lNextCount > 0 )
      {
         lpdbOrdCondInfo->lNextCount --;
         if( !lpdbOrdCondInfo->lNextCount )
            lpdbOrdCondInfo->lNextCount --;
         return TRUE;
      }
      if( lpdbOrdCondInfo->itmCobWhile )
         return checkLogicalExpr( lpdbOrdCondInfo->itmCobWhile, NULL );
      if( lpdbOrdCondInfo->fUseCurrent && pArea->fEof )
         return FALSE;

      return TRUE;
   }
}

/* DJGPP can sprintf a float that is almost 320 digits long */
#define HB_MAX_DOUBLE_LENGTH 320

static ERRCODE hb_ntxIndexCreate( LPNTXINDEX pIndex )
{

   ULONG ulRecNo, ulRecCount, ulKeyNo = 0, lStep = 0, ulRecMax;
   USHORT uiCurLen;
   char szBuffer[ HB_MAX_DOUBLE_LENGTH + 1 ];
   char * pszTempName = NULL;
   NTXAREAP pArea;
   LPTAGINFO pTag;
   HB_MACRO_PTR pMacro;
   PHB_ITEM pItem;
   BOOL bWhileOk;
   NTXSORTINFO sortInfo;
   BYTE* readBuffer;
   USHORT numRecinBuf = 0, nParts = 0;
   BYTE * pRecordTmp;
   BOOL fValidBuffer;

   ulRecCount = pIndex->Owner->ulRecCount;
   pArea = pIndex->Owner;
   pTag = pIndex->CompoundTag;
   pItem = hb_itemNew( NULL );

   memset( &sortInfo, 0, sizeof( sortInfo ) );
   readBuffer = (BYTE*) hb_xgrab( pArea->uiRecordLen * 10 );
   /* itemLength = sizeof( LPSORTITEM ) + sizeof( ULONG ) + pTag->KeyLength; */
   sortInfo.itemLength = sizeof( LPSORTITEM ) + sizeof( ULONG ) + pTag->KeyLength;
   sortInfo.nItems = 0;
   sortInfo.pKey1 = sortInfo.pKey2 = sortInfo.pKeyFirst = sortInfo.pKeyTemp = NULL;
   if( pArea->lpdbOrdCondInfo && pArea->lpdbOrdCondInfo->fCustom )
      ulRecCount = 0;
   if( ulRecCount )
   {
      ulRecMax = ulRecCount;
      sortInfo.sortBuffer = (BYTE*) hb_xalloc( ulRecCount * sortInfo.itemLength );
      if( !sortInfo.sortBuffer )
      {
         /* If there isn't memory enough for the sortBuffer, we need to
            create a buffer of less size and use swapping */
         do
         {
            nParts = (nParts)? nParts*2:2;
            sortInfo.sortBuffer = (BYTE*) hb_xalloc(
                  (ulRecCount/nParts+1) * sortInfo.itemLength );
            if( sortInfo.sortBuffer )
               sortInfo.swappages = (BYTE*) hb_xalloc( nParts * sizeof( SWAPPAGE ) );
            if( !sortInfo.swappages )
            {
               hb_xfree( sortInfo.sortBuffer );
               sortInfo.sortBuffer = NULL;
            }
         }
         while( !sortInfo.sortBuffer && nParts < 200 );
         if( !sortInfo.sortBuffer )
            hb_errInternal( HB_EI_ERRUNRECOV, "Not enough room for index buffer", "hb_ntxIndexCreate", NULL );
         sortInfo.nSwappages = nParts - 1;
         ulRecMax = ulRecCount/nParts+1;
         /* printf( "\nnParts=%d ulRecMax=%d",nParts,ulRecMax ); */
         nParts = 1;
      }
      sortInfo.ulSqrt = (ulRecMax>50)? (ULONG)floor( sqrt( ( double) ulRecMax ) ):ulRecMax;
   }
   else
      sortInfo.sortBuffer = NULL;

   if( !pArea->lpdbOrdCondInfo || pArea->lpdbOrdCondInfo->fAll )
   {
      pRecordTmp = pArea->pRecord;
      fValidBuffer = pArea->fValidBuffer;
      pArea->fValidBuffer = TRUE;
      hb_fsSeek( pArea->hDataFile, pArea->uiHeaderLen, FS_SET );
   }
   else if( pArea->lpdbOrdCondInfo->fUseCurrent )
      SELF_GOTOP( ( AREAP ) pArea );
   for( ulRecNo = 1; ulRecNo <= ulRecCount; ulRecNo++)
   {
      if( !hb_ntxReadBuf( pArea, readBuffer, &numRecinBuf, pArea->lpdbOrdCondInfo ) )
         break;
      if( !pArea->lpdbOrdCondInfo || pArea->lpdbOrdCondInfo->fAll )
         pArea->ulRecNo = ulRecNo;
      if( pTag->pForItem != NULL )
         bWhileOk = checkLogicalExpr( pTag->pForItem, pItem );
      else
         bWhileOk = TRUE;
      if( bWhileOk )
      {
         ulKeyNo ++;
         sortInfo.ulKeyCount ++;
         if( sortInfo.nSwappages && ulKeyNo > ulRecMax )
         {
            if( nParts == 1 )
            {
               USHORT nAttemptLeft = 999;
               pszTempName = (char*) hb_xgrab( _POSIX_PATH_MAX );
               // sortInfo.tempHandle = hb_fsCreateTemp( NULL, NULL, FC_NORMAL, pszTempName );
               while( --nAttemptLeft )
               {
                  tmpnam( pszTempName );
                  sortInfo.tempHandle = hb_fsCreate( (BYTE*)pszTempName,FC_NORMAL );
                  if( sortInfo.tempHandle != FS_ERROR )
                     break;
               }
               if( sortInfo.tempHandle == FS_ERROR )
                  hb_errInternal( HB_EI_ERRUNRECOV, "Cannot create temp file", "hb_ntxIndexCreate", NULL );
            }
            hb_ntxSortKeyEnd( pTag, &sortInfo );
            hb_ntxSwapPageSave( pTag, &sortInfo, nParts-1 );
            sortInfo.nItems = 0;
            sortInfo.pKey1 = sortInfo.pKey2 = sortInfo.pKeyFirst = sortInfo.pKeyTemp = NULL;
            ulKeyNo = 1;
            nParts ++;
         }
         if( hb_itemType( pTag->pKeyItem ) == HB_IT_BLOCK )
         {
            hb_vmPushSymbol( &hb_symEval );
            hb_vmPush( pTag->pKeyItem );
            hb_vmSend( 0 );
            hb_itemCopy( pItem, &hb_stack.Return );
         }
         else
         {
            pMacro = ( HB_MACRO_PTR ) hb_itemGetPtr( pTag->pKeyItem );
            hb_macroRun( pMacro );
            hb_itemCopy( pItem, hb_stackItemFromTop( - 1 ) );
            hb_stackPop();
         }
         switch( hb_itemType( pItem ) )
         {
            case HB_IT_STRING:
               uiCurLen = (USHORT) hb_itemGetCLen( pItem );
               if(uiCurLen > NTX_MAX_KEY )
                  uiCurLen = NTX_MAX_KEY ;
               if( pTag->KeyLength != uiCurLen )
               {
                  hb_itemRelease( pItem );
                  return FAILURE;
               }
               hb_ntxSortKeyAdd( pTag, &sortInfo, pItem->item.asString.value, ulKeyNo );
               break;
            case HB_IT_INTEGER:
            case HB_IT_LONG:
            case HB_IT_DOUBLE:
               numToStr( pItem, szBuffer, pTag->KeyLength, pTag->KeyDec );
               hb_ntxSortKeyAdd( pTag, &sortInfo,szBuffer, ulKeyNo );
               break;
            case HB_IT_DATE:
               hb_itemGetDS( pItem, szBuffer );
               hb_ntxSortKeyAdd( pTag, &sortInfo, szBuffer, ulKeyNo );
               break;
            case HB_IT_LOGICAL:
               szBuffer[0] = ( hb_itemGetL( pItem ) ? 'T' : 'F' );
               szBuffer[1] = 0;
               hb_ntxSortKeyAdd( pTag, &sortInfo, szBuffer, ulKeyNo );
               break;
            default:
               printf( "ntxCreateOrder" );
         }
      }
      if( pArea->lpdbOrdCondInfo )
      {
         if( !pArea->lpdbOrdCondInfo->fAll )
            SELF_SKIP( ( AREAP ) pArea, 1 );
         if( pArea->lpdbOrdCondInfo->lStep )
         {
            lStep ++;
            if( lStep == (ULONG) pArea->lpdbOrdCondInfo->lStep )
               lStep = 0;
         }
         if( pArea->lpdbOrdCondInfo->itmCobEval && !lStep )
         {
            hb_vmPushSymbol( &hb_symEval );
            hb_vmPush( pArea->lpdbOrdCondInfo->itmCobEval );
            hb_vmSend( 0 );
         }
      }
   }
   hb_ntxSortKeyEnd( pTag, &sortInfo );
   if( !pArea->lpdbOrdCondInfo || pArea->lpdbOrdCondInfo->fAll )
   {
      pArea->pRecord = pRecordTmp;
      pArea->fValidBuffer = fValidBuffer;
   }
   if( sortInfo.nSwappages )
      sortInfo.nSwappages = nParts - 1;

   /* Building index file with previously sorted keys */
   hb_ntxBufferSave( pTag, &sortInfo );

   if( pszTempName )
   {  /*  Close temporary swap file, delete it and free name buffer */
      hb_fsClose( sortInfo.tempHandle );
      hb_fsDelete( (BYTE*) pszTempName );
      hb_xfree( pszTempName );
   }
   if( sortInfo.swappages )
      hb_xfree( sortInfo.swappages );
   if( sortInfo.sortBuffer )
      hb_xfree( sortInfo.sortBuffer );
   hb_xfree( readBuffer );
   hb_itemRelease( pItem );
   pTag->RootPage = NULL;
   return SUCCESS;
}

static void hb_ntxHeaderSave( LPNTXINDEX pIndex, BOOL bFull )
{
   NTXHEADER Header;

   hb_fsSeek( pIndex->DiskFile , 0 , 0 );
   memset( (void*) &Header, 0, sizeof( NTXHEADER ) );
   Header.type = 15;
   Header.version = 1;
   Header.root = pIndex->CompoundTag->RootBlock;
   Header.next_page = pIndex->NextAvail;
   Header.item_size = pIndex->CompoundTag->KeyLength+8;
   Header.key_size = pIndex->CompoundTag->KeyLength;
   Header.key_dec = pIndex->CompoundTag->KeyDec;
   if( bFull )
   {
      Header.max_item = pIndex->CompoundTag->MaxKeys;
      Header.half_page = Header.max_item/2;
      strcpy( Header.key_expr , pIndex->CompoundTag->KeyExpr );
      if( pIndex->CompoundTag->ForExpr )
         strcpy( Header.for_expr , pIndex->CompoundTag->ForExpr );
      Header.unique = pIndex->CompoundTag->UniqueKey;
      Header.descend = !pIndex->CompoundTag->AscendKey;
      Header.custom = pIndex->CompoundTag->Custom;
      hb_fsWrite( pIndex->DiskFile,(BYTE*)&Header,sizeof(NTXHEADER) );
   }
   else
      hb_fsWrite( pIndex->DiskFile,(BYTE*)&Header,16 );
}

static LPTAGINFO hb_ntxTagNew( LPNTXINDEX PIF, char * ITN, char *szKeyExpr, PHB_ITEM pKeyExpr, BYTE bKeyType, USHORT uiKeyLen, USHORT uiKeyDec, char *szForExp, PHB_ITEM pForExp, BOOL fAscendKey, BOOL fUnique, BOOL fCustom )
{
   LPTAGINFO pTag;

   pTag = ( LPTAGINFO ) hb_xgrab( sizeof( TAGINFO ) );
   memset( pTag, 0, sizeof( TAGINFO ) );
   pTag->TagName = ITN;
   if( szKeyExpr )
   {
      pTag->KeyExpr = (char *) hb_xgrab( NTX_MAX_KEY );
      strcpy( pTag->KeyExpr, szKeyExpr );
   }
   if( szForExp )
   {
      pTag->ForExpr = (char *) hb_xgrab( NTX_MAX_KEY );
      strcpy( pTag->ForExpr, szForExp );
   }
   pTag->pKeyItem = pKeyExpr;
   pTag->pForItem = pForExp;
   pTag->AscendKey = fAscendKey;
   pTag->UniqueKey = fUnique;
   pTag->Custom = fCustom;
   pTag->KeyType = bKeyType;
   pTag->KeyLength = uiKeyLen;
   pTag->KeyDec = uiKeyDec;
   pTag->Owner = PIF;
   pTag->MaxKeys = (NTXBLOCKSIZE-6)/(uiKeyLen+10) - 1;
   pTag->CurKeyInfo = hb_ntxKeyNew( NULL,pTag->KeyLength );
   pTag->uiPages = 0;
   pTag->stack = (LPTREESTACK) hb_xgrab( sizeof(TREE_STACK) * 32 );
   pTag->stackDepth = 32;
   return pTag;
}

static LPNTXINDEX hb_ntxIndexNew( NTXAREAP pArea )
{
   LPNTXINDEX pIndex;

   pIndex = ( LPNTXINDEX ) hb_xgrab( sizeof( NTXINDEX ) );
   memset( pIndex, 0, sizeof( NTXINDEX ) );
   pIndex->DiskFile = FS_ERROR;

   pIndex->Owner = pArea;
   pIndex->NextAvail = 0;
   pIndex->TagRoot = 1;
   return pIndex;
}

static void hb_ntxIndexFree( LPNTXINDEX pIndex )
{
   LPTAGINFO pTag;

   pTag = pIndex->CompoundTag;
   if( pTag->RootPage > 0 )
      hb_ntxPageFree( pTag->RootPage,TRUE );
   pTag->RootPage = NULL;
   hb_fsClose( pIndex->DiskFile );
   hb_xfree( pTag->TagName );
   if( pTag->KeyExpr != NULL )
      hb_xfree( pTag->KeyExpr );
   if( pTag->ForExpr != NULL )
      hb_xfree( pTag->ForExpr );
   if( pTag->pKeyItem != NULL )
   {
      if( hb_itemType( pTag->pKeyItem ) != HB_IT_BLOCK )
         hb_macroDelete( ( HB_MACRO_PTR ) hb_itemGetPtr( pTag->pKeyItem ) );
      hb_itemRelease( pTag->pKeyItem );
   }
   if( pTag->pForItem != NULL )
   {
      if( hb_itemType( pTag->pForItem ) != HB_IT_BLOCK )
         hb_macroDelete( ( HB_MACRO_PTR ) hb_itemGetPtr( pTag->pForItem ) );
      hb_itemRelease( pTag->pForItem );
   }
   hb_ntxKeyFree( pTag->CurKeyInfo );
   hb_ntxClearScope( pTag,0 );
   hb_ntxClearScope( pTag,1 );
   hb_xfree( pTag->stack );
   hb_xfree( pTag );
   hb_xfree( pIndex->IndexName );
   hb_xfree( pIndex );
}

static ERRCODE hb_ntxHeaderRead( LPNTXINDEX pIndex )
{
   NTXHEADER Header;
   ULONG ulPos;

   ulPos = hb_fsSeek( pIndex->DiskFile, 0, SEEK_END );
   hb_fsSeek( pIndex->DiskFile , 0 , 0 );
   if( hb_fsRead( pIndex->DiskFile,(BYTE*)&Header,16 ) != 16 )
      return FAILURE;

   pIndex->NextAvail = Header.next_page;
   pIndex->CompoundTag->TagBlock = ulPos - 1024;
   pIndex->CompoundTag->RootBlock = Header.root;
   return SUCCESS;
}

static ERRCODE hb_ntxHeaderLoad( LPNTXINDEX pIndex , char *ITN)
{
   NTXHEADER Header;
   LPTAGINFO pTag;
   PHB_ITEM pKeyExp, pForExp = NULL;
   ULONG ulPos;

   ulPos = hb_fsSeek( pIndex->DiskFile, 0, SEEK_END );

   hb_fsSeek( pIndex->DiskFile , 0 , 0 );
   if( hb_fsRead( pIndex->DiskFile,(BYTE*)&Header,sizeof(NTXHEADER)) != sizeof(NTXHEADER) )
      return FAILURE;

   if( SELF_COMPILE( ( AREAP ) pIndex->Owner, (BYTE*)Header.key_expr ) == FAILURE )
      return FAILURE;
   pKeyExp = hb_itemNew( NULL );
   hb_itemCopy( pKeyExp, pIndex->Owner->valResult );

   if( Header.for_expr[0] > 20 )
   {
      if( SELF_COMPILE( ( AREAP ) pIndex->Owner, (BYTE*)Header.for_expr ) == FAILURE )
         return FAILURE;
      pForExp = hb_itemNew( NULL );
      hb_itemCopy( pForExp, pIndex->Owner->valResult );
   }

   pTag = ( LPTAGINFO ) hb_xgrab( sizeof( TAGINFO ) );
   memset( pTag, 0, sizeof( TAGINFO ) );
   pIndex->CompoundTag = pTag;
   pIndex->NextAvail = Header.next_page;
   pTag->TagBlock = ulPos - 1024;
   pTag->RootBlock = Header.root;
   pTag->TagName = (char *) hb_xgrab( strlen( ITN ) + 1 );
   hb_strncpyUpper( pTag->TagName, ITN, strlen( ITN ) );
   pTag->KeyExpr = (char *) hb_xgrab( NTX_MAX_KEY );
   strcpy( pTag->KeyExpr, Header.key_expr );

   if( pForExp )
   {
      pTag->ForExpr = (char *) hb_xgrab( NTX_MAX_KEY );
      strcpy( pTag->ForExpr, Header.for_expr );
   }

   pTag->pKeyItem = pKeyExp;
   pTag->pForItem = pForExp;
   pTag->UniqueKey = Header.unique;
   pTag->AscendKey = !Header.descend;
   pTag->Custom = Header.custom;
   pTag->KeyType = 'C'; /* bKeyType; */
   pTag->KeyLength = Header.key_size;
   pTag->KeyDec = Header.key_dec;
   pTag->Owner = pIndex;
   pTag->MaxKeys = Header.max_item;
   pTag->CurKeyInfo = hb_ntxKeyNew( NULL,pTag->KeyLength );
   pTag->stack = (LPTREESTACK) hb_xgrab( sizeof(TREE_STACK) * 32 );
   pTag->stackDepth = 32;
   return SUCCESS;
}

static LPPAGEINFO hb_ntxPageFind( LPNTXINDEX pIndex, LONG ulOffset )
{
   LPPAGEINFO pPage = pIndex->CompoundTag->RootPage;
      /* printf( "\nntxPageFind - 0 ( %5lx %5lx %d )",pIndex,pPage,ulOffset ); */
   while( pPage )
   {
      if( pPage->Page == ulOffset )
         break;
      pPage = pPage->pNext;
   }
   return pPage;
}

static LPPAGEINFO hb_ntxPageLast( LPNTXINDEX pIndex )
{
   LPPAGEINFO pPage = pIndex->CompoundTag->RootPage;
   if( pPage )
   {
      while( pPage->pNext )
      {
         pPage = pPage->pNext;
      }
   }
   return pPage;
}

static LPNTXINDEX ntxFindIndex( NTXAREAP pArea , PHB_ITEM lpOrder )
{
   LPNTXINDEX start, current;

   start = pArea->lpNtxIndex;
   if( !start )
      return NULL;
   current = start;

   if( !lpOrder )
      return pArea->lpCurIndex;
   else if( hb_itemType( lpOrder ) == HB_IT_STRING )
   {
      do
      {
         if( !strcmp( current->CompoundTag->TagName , hb_itemGetCPtr( lpOrder ) ) )
            return current;
         current = current->pNext;
      } while( current );
   }
   else
   {
      do
      {
         if( current->TagRoot == hb_itemGetNI( lpOrder ) )
            return current;
         current = current->pNext;
      } while( current );
   }
   return NULL;
}

/* Implementation of exported functions */

static ERRCODE ntxGoBottom( NTXAREAP pArea )
{
   HB_TRACE(HB_TR_DEBUG, ("ntxGoBottom(%p)", pArea));

   if( SELF_GOCOLD( ( AREAP ) pArea ) == FAILURE )
      return FAILURE;
   if ( !pArea->lpCurIndex || !pArea->lpNtxIndex )
     SUPER_GOBOTTOM( ( AREAP ) pArea );
   else
   {
     LPTAGINFO pTag;

     pTag = pArea->lpCurIndex->CompoundTag;
     if( pTag->bottomScope )
        ntxSeek( pArea, 1, pTag->bottomScope, 1 );
     else
        hb_ntxTagKeyGoTo( pTag, BTTM_RECORD, NULL );
     SELF_GOTO( ( AREAP ) pArea, pTag->CurKeyInfo->Xtra );
   }
   return SELF_SKIPFILTER( ( AREAP ) pArea, -1 );
}

static ERRCODE ntxGoTo( NTXAREAP pArea, ULONG ulRecNo )
{
   HB_TRACE(HB_TR_DEBUG, ("ntxGoTo(%p, %lu)", pArea, ulRecNo));
   if( pArea->lpCurIndex &&
         (ULONG)pArea->lpCurIndex->CompoundTag->CurKeyInfo->Xtra != ulRecNo )
   {
      pArea->lpCurIndex->CompoundTag->CurKeyInfo->Tag = 0;
      pArea->lpCurIndex->CompoundTag->CurKeyInfo->Xtra = 0;
      pArea->lpCurIndex->CompoundTag->TagBOF = FALSE;
      pArea->lpCurIndex->CompoundTag->TagEOF = FALSE;
   }
   return SUPER_GOTO( ( AREAP ) pArea,ulRecNo );
}

static ERRCODE ntxGoTop( NTXAREAP pArea )
{
   HB_TRACE(HB_TR_DEBUG, ("ntxGoTop(%p)", pArea));

   if( SELF_GOCOLD( ( AREAP ) pArea ) == FAILURE )
      return FAILURE;
   if ( !pArea->lpCurIndex || !pArea->lpNtxIndex )
     SUPER_GOTOP( ( AREAP ) pArea );
   else
   {
     LPTAGINFO pTag;

     pTag = pArea->lpCurIndex->CompoundTag;
     if( pTag->topScope )
        ntxSeek( pArea, 1, pTag->topScope, 0 );
     else
        hb_ntxTagKeyGoTo( pTag, TOP_RECORD, NULL );
     SELF_GOTO( ( AREAP ) pArea, pTag->CurKeyInfo->Xtra );
   }
   return SELF_SKIPFILTER( ( AREAP ) pArea, 1 );
}

static ERRCODE ntxSeek( NTXAREAP pArea, BOOL bSoftSeek, PHB_ITEM pKey, BOOL bFindLast )
{
   ERRCODE  retvalue;
   BOOL     result;
   HB_TRACE(HB_TR_DEBUG, ("ntxSeek(%p, %d, %p, %d)", pArea, bSoftSeek, pKey, bFindLast));

   if( SELF_GOCOLD( ( AREAP ) pArea ) == FAILURE )
      return FAILURE;
   if ( ! pArea->lpCurIndex )
   {
     commonError( pArea, EG_NOORDER, 1020, NULL, EF_CANDEFAULT );
     return FAILURE;
   }
   else
   {
     LONG lRecno;
     LPKEYINFO pKey2;
     LPTAGINFO pTag;
     char szBuffer[ NTX_MAX_KEY ];

     pTag = pArea->lpCurIndex->CompoundTag;
     pKey2 = hb_ntxKeyNew( NULL,pTag->KeyLength );
     switch( hb_itemType( pKey ) )
     {
        case HB_IT_STRING:
           if( pKey->item.asString.length == 0 )
           {
             hb_ntxKeyFree( pKey2 );
             retvalue = SELF_GOTOP( ( AREAP ) pArea );
             pArea->fFound = TRUE;
             return retvalue;
           }
           strncpy( pKey2->key,pKey->item.asString.value,pTag->KeyLength );
           break;
        case HB_IT_INTEGER:
        case HB_IT_LONG:
        case HB_IT_DOUBLE:
           strncpy( pKey2->key, numToStr( pKey, szBuffer, pTag->KeyLength, pTag->KeyDec ),pTag->KeyLength );
           break;
        case HB_IT_DATE:
           hb_itemGetDS( pKey, szBuffer );
           strcpy( pKey2->key,szBuffer );
           break;
        case HB_IT_LOGICAL:
           szBuffer[0] = ( hb_itemGetL( pKey ) ? 'T' : 'F' );
           szBuffer[1] = 0;
           strcpy( pKey2->key, szBuffer );
           break;
     }
     pKey2->key[ pTag->KeyLength ] = '\0';
     if( !hb_ntxInTopScope( pTag, pKey2->key ) ||
           !hb_ntxInBottomScope( pTag, pKey2->key ) )
     {
       hb_ntxKeyFree( pKey2 );
       return hb_ntxGoEof( pArea );
     }
     pKey2->Xtra = 0;

     if( pArea->fShared )
     {
        while( !hb_fsLock( pArea->lpCurIndex->DiskFile, NTX_LOCK_OFFSET, 1, FL_LOCK ) );
        pArea->lpCurIndex->Locked = TRUE;
     }
     lRecno = hb_ntxTagKeyFind( pTag, pKey2, &result );
     if( bFindLast && lRecno > 0 && result )
     {
        LONG lRecnoLast;

        pArea->fEof = pArea->fBof = FALSE;
        hb_IncString( pKey2->key,strlen( pKey2->key ) );
        lRecnoLast = hb_ntxTagKeyFind( pTag, pKey2, &result );
        hb_ntxKeyFree( pKey2 );
        if( lRecnoLast > 0 )
        {
           BOOL lContinue = FALSE;

           pArea->ulRecNo = lRecnoLast;
           pArea->fValidBuffer = FALSE;
           do
              hb_ntxTagKeyGoTo( pTag, PREV_RECORD, &lContinue );
           while( hb_ntxIsRecBad( pArea, pTag->CurKeyInfo->Xtra ) );
           retvalue = SELF_GOTO( ( AREAP ) pArea, pTag->CurKeyInfo->Xtra );
           pArea->fFound = TRUE;
           return retvalue;
        }
        else
        {
           retvalue = SELF_GOTO( ( AREAP ) pArea, lRecno );
           pArea->fFound = TRUE;
           return retvalue;
        }
     }
     if( pArea->fShared )
     {
        hb_ntxPageFree( pTag->RootPage,FALSE );
        hb_fsLock( pArea->lpCurIndex->DiskFile, NTX_LOCK_OFFSET, 1, FL_UNLOCK );
        pArea->lpCurIndex->Locked = FALSE;
     }
     pArea->fEof = pTag->TagEOF;
     pArea->fBof = pTag->TagBOF;
     hb_ntxKeyFree( pKey2 );
     if ( lRecno > 0 && result )
     {
        retvalue = SELF_GOTO( ( AREAP ) pArea, lRecno );
        pArea->fFound = TRUE;
        return retvalue;
     }
     else
     {
       pArea->fFound = FALSE;
       if ( lRecno > 0 && !result && bSoftSeek && !pTag->TagEOF )
         return SELF_GOTO( ( AREAP ) pArea, lRecno );
       else
          return hb_ntxGoEof( pArea );
     }
   }
}

static ERRCODE ntxSkipRaw( NTXAREAP pArea, LONG lToSkip )
{
   HB_TRACE(HB_TR_DEBUG, ("ntxSkipRaw(%p, %ld)", pArea, lToSkip));

   if( SELF_GOCOLD( ( AREAP ) pArea ) == FAILURE )
      return FAILURE;
   if ( ! pArea->lpCurIndex )
     SUPER_SKIPRAW( ( AREAP ) pArea, lToSkip );
   else
   {
     LPTAGINFO pTag = pArea->lpCurIndex->CompoundTag;
     BOOL lContinue = FALSE;
     ULONG ulRecNo = pArea->ulRecNo;

     if ( pArea->fBof )
        SELF_GOTOP( ( AREAP ) pArea );

     if ( lToSkip == 0 )
       SUPER_SKIPRAW( ( AREAP ) pArea, 0 );
     else if ( lToSkip > 0 )
     {
       if ( !pArea->fEof )
       {
          while ( !pTag->TagEOF && lToSkip-- > 0 )
          {
            hb_ntxTagKeyGoTo( pTag, NEXT_RECORD, &lContinue );
            if( !hb_ntxInTopScope( pTag, pTag->CurKeyInfo->key ) )
            {
               ntxSeek( pArea, 1, pTag->topScope, 0 );
            }
            else if( !hb_ntxInBottomScope( pTag, pTag->CurKeyInfo->key ) )
            {
               pTag->TagEOF = TRUE;
            }
          }
          pArea->ulRecNo = ulRecNo;
          if ( !pTag->TagEOF )
            SELF_GOTO( ( AREAP ) pArea, pTag->CurKeyInfo->Xtra );
          else
          {
            hb_ntxGoEof( pArea );
          }
       }
     }
     else /* ( lToSkip < 0 ) */
     {
       if ( pArea->fEof )
       {
         SELF_GOBOTTOM( ( AREAP ) pArea );
         lToSkip++;
       }
       pTag->TagBOF = FALSE;
       while ( !pTag->TagBOF && lToSkip++ < 0 )
       {
         hb_ntxTagKeyGoTo( pTag, PREV_RECORD, &lContinue );
         if( !hb_ntxInTopScope( pTag, pTag->CurKeyInfo->key ) )
         {
            ntxSeek( pArea, 1, pTag->topScope, 0 );
            pTag->TagBOF = TRUE;
         }
         else if( !hb_ntxInBottomScope( pTag, pTag->CurKeyInfo->key ) )
         {
            ntxSeek( pArea, 1, pTag->bottomScope, 1 );
         }
       }
       pArea->ulRecNo = ulRecNo;
       if ( !pTag->TagBOF )
         SELF_GOTO( ( AREAP ) pArea, pTag->CurKeyInfo->Xtra );
       else
       {
         pTag->TagBOF = FALSE;
         SELF_GOTOP( ( AREAP ) pArea );
         pArea->fBof = pTag->TagBOF = TRUE;
       }
     }
   }
   return SUCCESS;
}

static ERRCODE ntxGoCold( NTXAREAP pArea )
{
   LPNTXINDEX lpIndex, lpIndexTmp;
   LPKEYINFO pKey;
   LPTAGINFO pTag;
   LPPAGEINFO pPage;
   BOOL fRecordChanged = pArea->fRecordChanged;
   BOOL fAppend = pArea->fAppend;
   BOOL InIndex;

   HB_TRACE(HB_TR_DEBUG, ("ntxGoCold(%p)", pArea));

   if( SUPER_GOCOLD( ( AREAP ) pArea ) == SUCCESS )
   {
      if( fRecordChanged )
      {
         lpIndexTmp = pArea->lpCurIndex;
         lpIndex = pArea->lpNtxIndex;
         while( lpIndex )
         {
            pTag = lpIndex->CompoundTag;
            pKey = hb_ntxKeyNew( NULL,pTag->KeyLength );
            hb_ntxGetCurrentKey( pTag, pKey );
            if( pTag->pForItem == NULL || checkLogicalExpr( pTag->pForItem, NULL ) )
               InIndex = TRUE;
            else
               InIndex = FALSE;

            if( fAppend || hb_ntxItemCompare( pKey->key,
                    pTag->CurKeyInfo->key,
                    pTag->KeyLength, pTag->KeyLength, TRUE )
                || InIndex != pTag->InIndex )
            {
               pArea->lpCurIndex = lpIndex;
               if( pArea->fShared )
                  while( !hb_fsLock( lpIndex->DiskFile, NTX_LOCK_OFFSET, 1, FL_LOCK ) );
               if( !fAppend && pTag->InIndex )
               {
                  LPKEYINFO pKeyOld = hb_ntxKeyNew( pTag->CurKeyInfo,pTag->KeyLength );

                  if( hb_ntxTagFindCurrentKey( hb_ntxPageLoad( pTag->Owner,0 ), pKeyOld, FALSE, FALSE ) )
                  {
                      printf( "\n\rntxGoCold: Cannot find current key:" );
                      lpIndex = lpIndex->pNext;
                      continue;
                  }
                  pPage = hb_ntxPageLoad( pTag->Owner,pTag->CurKeyInfo->Tag );
                  pPage->CurKey =  hb_ntxPageFindCurrentKey( pPage,pTag->CurKeyInfo->Xtra ) - 1;
                  hb_ntxPageKeyDel( pPage, pPage->CurKey, 1 );
                  if( pTag->stack[0].ikey < 0 )
                     hb_ntxTagBalance( pTag,0 );
                  hb_ntxPageRelease( pPage );
                  if( !pArea->fShared && pTag->keyCount &&
                        hb_ntxInTopScope( pTag, pKeyOld->key ) &&
                        hb_ntxInBottomScope( pTag, pKeyOld->key ) )
                     pTag->keyCount --;
                  hb_ntxKeyFree( pKeyOld );
               }
               if( InIndex )
               {
                  pKey->Tag = 0;
                  hb_ntxTagKeyAdd( pTag, pKey );
                  if( !pArea->fShared && pTag->keyCount &&
                        hb_ntxInTopScope( pTag, pKey->key ) &&
                        hb_ntxInBottomScope( pTag, pKey->key ) )
                     pTag->keyCount ++;
               }
               if( pArea->fShared )
               {
                  hb_ntxPageFree( pTag->RootPage,FALSE );
                  hb_fsLock( lpIndex->DiskFile, NTX_LOCK_OFFSET, 1, FL_UNLOCK );
               }
            }
            lpIndex = lpIndex->pNext;
            hb_ntxKeyFree( pKey );
         }
         pArea->lpCurIndex = lpIndexTmp;
      }
      return SUCCESS;
   }
   else
      return FAILURE;
}

static ERRCODE ntxGoHot( NTXAREAP pArea )
{
   LPNTXINDEX lpIndex;
   LPTAGINFO pTag;

   HB_TRACE(HB_TR_DEBUG, ("ntxGoHot(%p)", pArea));

   if( SUPER_GOHOT( ( AREAP ) pArea ) == SUCCESS )
   {
      lpIndex = pArea->lpNtxIndex;
      while( lpIndex )
      {
         pTag = lpIndex->CompoundTag;
         hb_ntxGetCurrentKey( pTag, pTag->CurKeyInfo );
         if( pTag->pForItem == NULL || checkLogicalExpr( pTag->pForItem, NULL ) )
            pTag->InIndex = TRUE;
         else
            pTag->InIndex = FALSE;
         lpIndex = lpIndex->pNext;
      }
      return SUCCESS;
   }
   else
      return FAILURE;
}

/*
 * Retrieve the size of the WorkArea structure.
 */
static ERRCODE ntxStructSize( NTXAREAP pArea, USHORT * uiSize )

{
   HB_TRACE(HB_TR_DEBUG, ("ntxStrucSize(%p, %p)", pArea, uiSize));
   HB_SYMBOL_UNUSED( pArea );

   * uiSize = sizeof( NTXAREA );
   return SUCCESS;
}

static ERRCODE ntxSysName( NTXAREAP pArea, BYTE * pBuffer )
{
   HB_TRACE(HB_TR_DEBUG, ("ntxSysName(%p, %p)", pArea, pBuffer));
   HB_SYMBOL_UNUSED( pArea );

   strncpy( ( char * ) pBuffer, "DBFNTX", HARBOUR_MAX_RDD_DRIVERNAME_LENGTH );
   return SUCCESS;
}

static ERRCODE ntxPack( NTXAREAP pArea )
{
   HB_TRACE(HB_TR_DEBUG, ("ntxPack(%p)", pArea ));

   if( SUPER_PACK( ( AREAP ) pArea ) == SUCCESS )
     return ntxOrderListRebuild( pArea );
   else
     return FAILURE;
}

static ERRCODE ntxZap( NTXAREAP pArea )
{
   HB_TRACE(HB_TR_DEBUG, ("ntxZap(%p)", pArea ));

   if( SUPER_ZAP( ( AREAP ) pArea ) == SUCCESS )
   {
      LPNTXINDEX lpIndex, lpIndexTmp;
      LPTAGINFO pTag;
      char* buffer;
      USHORT i;
      LPNTXBUFFER itemlist;

      buffer = (char*) hb_xgrab( NTXBLOCKSIZE );
      lpIndex = pArea->lpNtxIndex;
      lpIndexTmp = pArea->lpCurIndex;
      while( lpIndex )
      {
         if( lpIndex->CompoundTag->RootPage > 0 )
            hb_ntxPageFree( lpIndex->CompoundTag->RootPage,TRUE );
         lpIndex->CompoundTag->RootPage = NULL;
         lpIndex->CompoundTag->RootBlock = NTXBLOCKSIZE;
         hb_ntxHeaderSave( lpIndex, FALSE );

         pTag = lpIndex->CompoundTag;
         memset( buffer, 0, NTXBLOCKSIZE );
         itemlist = ( LPNTXBUFFER ) buffer;
         itemlist->item_count = 0;
         for( i = 0; i < pTag->MaxKeys+1; i++ )
            itemlist->item_offset[i] = 2 + 2 * ( pTag->MaxKeys + 1 ) +
               i * ( pTag->KeyLength + 8 );

         hb_fsSeek( lpIndex->DiskFile, NTXBLOCKSIZE, FS_SET );
         hb_fsWrite( lpIndex->DiskFile, (BYTE *) buffer, NTXBLOCKSIZE );
         hb_fsWrite( lpIndex->DiskFile, NULL, 0 );

         lpIndex = lpIndex->pNext;
      }
      pArea->lpCurIndex = lpIndexTmp;
      hb_xfree( buffer );
      return SELF_GOTOP( ( AREAP ) pArea );
   }
   else
     return FAILURE;
}

static ERRCODE ntxOrderCreate( NTXAREAP pArea, LPDBORDERCREATEINFO pOrderInfo )
{
   PHB_ITEM pExpr, pResult;
   PHB_ITEM pKeyExp, pForExp;
   HB_MACRO_PTR pExpMacro, pForMacro;
   USHORT uiType;
   int uiLen, uiDec;
   char * szFileName, * szTagName;
   LPNTXINDEX pIndex;
   LPTAGINFO pTag;
   PHB_FNAME pFileName;
   DBORDERINFO pExtInfo;
   BYTE bType;

   HB_TRACE(HB_TR_DEBUG, ("ntxOrderCreate(%p, %p)", pArea, pOrderInfo));

   /* printf( "\nntxOrderCreate - 0\n" ); */
   if( SELF_GOCOLD( ( AREAP ) pArea ) == FAILURE )
      return FAILURE;
   if( !pArea->lpdbOrdCondInfo || ( pArea->lpdbOrdCondInfo->fAll && 
                                    !pArea->lpdbOrdCondInfo->fAdditive ) )
      SELF_ORDLSTCLEAR( ( AREAP ) pArea );

   /* If we have a codeblock for the expression, use it */
   if( pOrderInfo->itmCobExpr )
      pExpr = pOrderInfo->itmCobExpr;
   else /* Otherwise, try compiling the key expression string */
   {
      if( SELF_COMPILE( ( AREAP ) pArea, ( BYTE * ) pOrderInfo->abExpr->item.asString.value ) == FAILURE )
         return FAILURE;
      pExpr = pArea->valResult;
      /* pArea->valResult = NULL; */
   }
   /* Save for later use */
   pKeyExp = hb_itemNew( NULL );
   hb_itemCopy( pKeyExp, pExpr );

   /* Get a blank record before testing expression */
   /* SELF_GOBOTTOM( ( AREAP ) pArea );
      SELF_SKIP( ( AREAP ) pArea, 1 ); */
   pExpMacro = pForMacro = NULL;
   if( hb_itemType( pExpr ) == HB_IT_BLOCK )
   {
      if( SELF_EVALBLOCK( ( AREAP ) pArea, pExpr ) == FAILURE )
      {
         hb_itemRelease( pKeyExp );
         return FAILURE;
      }
      pResult = pArea->valResult;
      /* pArea->valResult = NULL; */
   }
   else
   {
      pExpMacro = ( HB_MACRO_PTR ) hb_itemGetPtr( pExpr );
      hb_macroRun( pExpMacro );
      pResult = pExpr;
      hb_itemCopy( pResult, &hb_stack.Return );
   }

   uiType = hb_itemType( pResult );
   uiLen = uiDec = 0;
   bType = '\0';
   switch( uiType )
   {
      case HB_IT_INTEGER:
      case HB_IT_LONG:
      case HB_IT_DOUBLE:
         bType = 'N';
         hb_itemGetNLen( pResult, (int*) &uiLen, (int*) &uiDec );
         if( uiDec )
            uiLen += uiDec + 1;
         /* printf( "\nLength: %d %d",uiLen,uiDec ); */
         break;

      case HB_IT_DATE:
         bType = 'D';
         uiLen = 8;
         break;

      case HB_IT_LOGICAL:
         bType = 'C';
         uiLen = 1;
         break;

      case HB_IT_STRING:
         bType = 'C';
         uiLen = pResult->item.asString.length > NTX_MAX_KEY ? NTX_MAX_KEY :
                 pResult->item.asString.length ;
         break;
   }

   /* Make sure uiLen is not 0 */
   if( !uiLen )
   {
      hb_itemRelease( pKeyExp );
      commonError( pArea, EG_DATAWIDTH, 1026, NULL, 0 );
      if( pExpMacro != NULL )
         hb_macroDelete( pExpMacro );
      return FAILURE;
   }

   /* Check conditional expression */
   pExpr = pForExp = NULL;
   if( pArea->lpdbOrdCondInfo && ( pArea->lpdbOrdCondInfo->itmCobFor || pArea->lpdbOrdCondInfo->abFor ) )
   {
      /* If we have a codeblock for the conditional expression, use it */
      if( pArea->lpdbOrdCondInfo->itmCobFor )
      {
         pExpr = pArea->lpdbOrdCondInfo->itmCobFor;
      }
      else /* Otherwise, try compiling the conditional expression string */
      {
         if( SELF_COMPILE( ( AREAP ) pArea, pArea->lpdbOrdCondInfo->abFor ) == FAILURE )
            return FAILURE;
         pExpr = pArea->valResult;
      }
      /* Save for later use */
      pForExp = hb_itemNew( NULL );
      hb_itemCopy( pForExp, pExpr );
   }

   /* Test conditional expression */
   if( pExpr )
   {
      if( hb_itemType( pExpr ) == HB_IT_BLOCK )
      {
         if( SELF_EVALBLOCK( ( AREAP ) pArea, pExpr ) == FAILURE )
         {
            hb_itemRelease( pKeyExp );
            hb_itemRelease( pForExp );
            if( pExpMacro != NULL )
                hb_macroDelete( pExpMacro );
            return FAILURE;
         }
         pResult = pArea->valResult;
      }
      else
      {
         pForMacro = ( HB_MACRO_PTR ) hb_itemGetPtr( pExpr );
         hb_macroRun( pForMacro );
         pResult = pExpr;
         hb_itemCopy( pResult, &hb_stack.Return );
      }
      uiType = hb_itemType( pResult );
      if( uiType != HB_IT_LOGICAL )
      {
         hb_itemRelease( pKeyExp );
         hb_itemRelease( pForExp );
         if( pExpMacro != NULL )
            hb_macroDelete( pExpMacro );
         if( pForMacro != NULL )
            hb_macroDelete( pForMacro );
         return FAILURE;
      }
   }

   /* Check file name */
   szFileName = ( char * ) hb_xgrab( _POSIX_PATH_MAX + 3 );
   szFileName[ 0 ] = '\0';
   if( strlen( ( char * ) pOrderInfo->abBagName ) == 0 )
   {
      pFileName = hb_fsFNameSplit( pArea->szDataFileName );
      if( pFileName->szDrive )
         strcat( szFileName, pFileName->szDrive );
      if( pFileName->szPath )
         strcat( szFileName, pFileName->szPath );
      strcat( szFileName, pFileName->szName );
      pExtInfo.itmResult = hb_itemPutC( NULL, "" );
      SELF_ORDINFO( ( AREAP ) pArea, DBOI_BAGEXT, &pExtInfo );
      strcat( szFileName, hb_itemGetCPtr( pExtInfo.itmResult ) );
      hb_itemRelease( pExtInfo.itmResult );
   }
   else
   {
      strcpy( szFileName, ( char * ) pOrderInfo->abBagName );
      pFileName = hb_fsFNameSplit( szFileName );
      if( !pFileName->szExtension )
      {
         pExtInfo.itmResult = hb_itemPutC( NULL, "" );
         SELF_ORDINFO( ( AREAP ) pArea, DBOI_BAGEXT, &pExtInfo );
         strcat( szFileName, hb_itemGetCPtr( pExtInfo.itmResult ) );
         hb_itemRelease( pExtInfo.itmResult );
      }
   }
   szTagName = ( char * ) hb_xgrab( strlen( pFileName->szName ) + 1 );
   hb_strncpyUpper( szTagName, pFileName->szName, strlen( pFileName->szName ) );
   hb_xfree( pFileName );

   pIndex = hb_ntxIndexNew( pArea );
   pIndex->IndexName = szFileName;
   pTag = hb_ntxTagNew( pIndex, szTagName, pOrderInfo->abExpr->item.asString.value,
                        pKeyExp, bType, (USHORT) uiLen, (USHORT) uiDec, (char *) ( pArea->lpdbOrdCondInfo ? pArea->lpdbOrdCondInfo->abFor : NULL ),
                        pForExp, pArea->lpdbOrdCondInfo ? !pArea->lpdbOrdCondInfo->fDescending : TRUE,
                        pOrderInfo->fUnique, pArea->lpdbOrdCondInfo ? pArea->lpdbOrdCondInfo->fCustom : FALSE );
   pIndex->CompoundTag = pTag;

   pIndex->DiskFile = hb_spCreate( ( BYTE * ) szFileName , FC_NORMAL );
   if(pIndex->DiskFile == FS_ERROR) {
      hb_xfree( szFileName );
      hb_itemRelease( pKeyExp );
      if( pForExp != NULL )
         hb_itemRelease( pForExp );
      if( pExpMacro != NULL )
         hb_macroDelete( pExpMacro );
      if( pForMacro != NULL )
         hb_macroDelete( pForMacro );
      return FAILURE;
   }
   if( hb_ntxIndexCreate( pIndex ) == FAILURE )
   {
      return FAILURE;
   }
   if( pArea->lpdbOrdCondInfo && !pArea->lpdbOrdCondInfo->fAll && 
                                 !pArea->lpdbOrdCondInfo->fAdditive )
      SELF_ORDLSTCLEAR( ( AREAP ) pArea );
   if( pArea->lpdbOrdCondInfo && pArea->lpdbOrdCondInfo->fAdditive )
   {
      if( pArea->lpNtxIndex )
      {
         LPNTXINDEX pIndexNext = pArea->lpNtxIndex;

         pIndex->TagRoot++;
         while( pIndexNext->pNext )
         {
            pIndex->TagRoot++;
            pIndexNext = pIndexNext->pNext;
         }
         pIndexNext->pNext = pIndex;
      }
   }
   else
      pArea->lpNtxIndex = pIndex;
   pArea->lpCurIndex = pIndex;
   hb_ntxHeaderSave( pIndex, TRUE );
   {
      BYTE emptyBuffer[250];
      memset( emptyBuffer, 0, 250 );
      hb_fsWrite( pIndex->DiskFile, emptyBuffer, 250 );
   }
   pIndex->CompoundTag->TagBlock = hb_fsSeek( pIndex->DiskFile, 0, SEEK_END ) - 1024;
   SELF_ORDSETCOND( ( AREAP ) pArea, NULL );
   return SELF_GOTOP( ( AREAP ) pArea );
}

static ERRCODE ntxOrderInfo( NTXAREAP pArea, USHORT uiIndex, LPDBORDERINFO pInfo )
{
   LPNTXINDEX pIndex;
   HB_TRACE(HB_TR_DEBUG, ("ntxOrderInfo(%p, %hu, %p)", pArea, uiIndex, pInfo));

   switch( uiIndex )
   {
      case DBOI_BAGEXT:
         hb_itemPutC( pInfo->itmResult, ".ntx" );
         return SUCCESS;
   }

   if( pArea->lpNtxIndex &&
       ( pIndex = ntxFindIndex( pArea , pInfo->itmOrder ) ) != NULL )
   {
      switch( uiIndex )
      {
         case DBOI_CONDITION:
            if( pIndex->CompoundTag->ForExpr )
               hb_itemPutC( pInfo->itmResult , pIndex->CompoundTag->ForExpr );
            else
               hb_itemPutC( pInfo->itmResult, "" );
            break;
         case DBOI_EXPRESSION:
            hb_itemPutC( pInfo->itmResult , pIndex->CompoundTag->KeyExpr );
            break;
         case DBOI_NUMBER:
            hb_itemPutNI( pInfo->itmResult, pIndex->TagRoot );
            /* TODO: Raise recoverable error */
            break;
         case DBOI_BAGNAME:
            hb_itemPutC( pInfo->itmResult, pIndex->IndexName );
            break;
         case DBOI_NAME:
            hb_itemPutC( pInfo->itmResult, pIndex->CompoundTag->TagName );
            break;
         case DBOI_KEYCOUNT:
            hb_itemPutNL( pInfo->itmResult, hb_ntxTagKeyCount( pIndex->CompoundTag ) );
            break;
         case DBOI_POSITION:
            hb_itemPutNL( pInfo->itmResult, hb_ntxTagKeyNo( pIndex->CompoundTag ) );
            break;
         case DBOI_ISCOND:
            hb_itemPutL( pInfo->itmResult, (pIndex->CompoundTag->ForExpr!=NULL) );
            break;
         case DBOI_ISDESC:
            hb_itemPutL( pInfo->itmResult, !pIndex->CompoundTag->AscendKey );
            break;
         case DBOI_UNIQUE:
            hb_itemPutL( pInfo->itmResult, pIndex->CompoundTag->UniqueKey );
            break;
         case DBOI_SCOPETOP :
            ntxScopeInfo(  pArea, 0, pInfo->itmResult ) ;
            break;
         case DBOI_SCOPEBOTTOM :
            ntxScopeInfo(  pArea, 1, pInfo->itmResult ) ;
            break;
         case DBOI_SCOPETOPCLEAR :
            ntxScopeInfo(  pArea, 0, pInfo->itmResult ) ;
            hb_ntxClearScope( pIndex->CompoundTag, 0 );
            break;
         case DBOI_SCOPEBOTTOMCLEAR :
            ntxScopeInfo(  pArea, 1, pInfo->itmResult ) ;
            hb_ntxClearScope( pIndex->CompoundTag, 1 );
            break;
      }
   }
   else
      switch( uiIndex )
      {
         case DBOI_KEYCOUNT:
         case DBOI_POSITION:
            hb_itemPutND( pInfo->itmResult,0 );
            break;
         case DBOI_ISCOND:
         case DBOI_ISDESC:
         case DBOI_UNIQUE:
            hb_itemPutL( pInfo->itmResult, 0 );
            break;
         case DBOI_SCOPETOP :
         case DBOI_SCOPEBOTTOM :
         case DBOI_SCOPETOPCLEAR :
         case DBOI_SCOPEBOTTOMCLEAR :
            hb_itemClear( pInfo->itmResult );
            break;
         default:
            hb_itemPutC( pInfo->itmResult, "" );
      }
   return SUCCESS;
}

static ERRCODE ntxClearScope( NTXAREAP pArea )
{
   HB_TRACE(HB_TR_DEBUG, ("ntxClearScope(%p)", pArea ));
   if( pArea->lpCurIndex )
   {
      hb_ntxClearScope( pArea->lpCurIndex->CompoundTag, 0 );
      hb_ntxClearScope( pArea->lpCurIndex->CompoundTag, 1 );
   }
   return SUCCESS;
}

static ERRCODE ntxScopeInfo( NTXAREAP pArea, USHORT nScope, PHB_ITEM pItem )
{
   USHORT nKeyType;

   HB_TRACE(HB_TR_DEBUG, ("ntxScopeInfo(%p, %hu, %p)", pArea, nScope, pItem));

   if( !pArea->lpCurIndex || ( nScope == 0 && !pArea->lpCurIndex->CompoundTag->topScope ) ||
            ( nScope == 1 && !pArea->lpCurIndex->CompoundTag->bottomScope ) )
      hb_itemClear( pItem );
   else
   {
      LPTAGINFO pTag = pArea->lpCurIndex->CompoundTag;

      nKeyType = hb_ntxGetKeyType( pTag );
      switch( nKeyType )
      {
         case HB_IT_STRING:
            hb_itemCopy( pItem, (nScope)? pTag->bottomScope:pTag->topScope );
            break;
         case HB_IT_INTEGER:
         case HB_IT_LONG:
         case HB_IT_DOUBLE:
            hb_itemPutND( pItem, hb_strVal(
              hb_itemGetCPtr( (nScope)? pTag->bottomScope:pTag->topScope ),
              hb_itemGetCLen( (nScope)? pTag->bottomScope:pTag->topScope ) ) );
            break;
        case HB_IT_DATE:
           hb_itemPutDS( pItem, hb_itemGetCPtr( (nScope)? pTag->bottomScope:pTag->topScope ) );
           break;
      }
   }
   return SUCCESS;
}

static ERRCODE ntxOrderListAdd( NTXAREAP pArea, LPDBORDERINFO pOrderInfo )
{
   USHORT uiFlags;
   char * szFileName;
   PHB_FNAME pFileName;
   DBORDERINFO pExtInfo;
   LPNTXINDEX pIndex, pIndexNext;
   PHB_ITEM pError = NULL;
   BOOL bRetry;

   HB_TRACE(HB_TR_DEBUG, ("ntxOrderListAdd(%p, %p)", pArea, pOrderInfo));

   if( SELF_GOCOLD( ( AREAP ) pArea ) == FAILURE )
      return FAILURE;

   szFileName = ( char * ) hb_xgrab( _POSIX_PATH_MAX + 3 );
   szFileName[ 0 ] = '\0';
   strcpy( szFileName, hb_itemGetCPtr( pOrderInfo->atomBagName ) );
   if( strlen( szFileName ) == 0 )
   {
      hb_xfree( szFileName );
      return FAILURE;
   }
   pFileName = hb_fsFNameSplit( szFileName );
   if( !pFileName->szExtension )
   {
      pExtInfo.itmResult = hb_itemPutC( NULL, "" );
      SELF_ORDINFO( ( AREAP ) pArea, DBOI_BAGEXT, &pExtInfo );
      strcat( szFileName, hb_itemGetCPtr( pExtInfo.itmResult ) );
      hb_itemRelease( pExtInfo.itmResult );
   }
   pIndex = hb_ntxIndexNew( pArea );

   /* Index file could be opened with FO_READ only in exclusive readonly mode
      to allow locking in other modes  */
   uiFlags =  !pArea->fShared && pArea->fReadonly  ? FO_READ : FO_READWRITE;
   uiFlags |= pArea->fShared ? FO_DENYNONE : FO_EXCLUSIVE;

   do
   {
     pIndex->DiskFile = hb_spOpen( ( BYTE * ) szFileName, uiFlags );
     if( pIndex->DiskFile == FS_ERROR )
     {
       if( !pError )
       {
         pError = hb_errNew();
         hb_errPutGenCode( pError, EG_OPEN );
         hb_errPutSubCode( pError, 1003 );
         hb_errPutDescription( pError, hb_langDGetErrorDesc( EG_OPEN ) );
         hb_errPutFileName( pError, szFileName );
         hb_errPutFlags( pError, EF_CANRETRY | EF_CANDEFAULT );
       }
       bRetry = ( SELF_ERROR( ( AREAP ) pArea, pError ) == E_RETRY );
     }
     else
       bRetry = FALSE;
   } while( bRetry );
   if( pError )
      hb_errRelease( pError );

   if( pIndex->DiskFile == FS_ERROR )
   {
      ntxOrderListClear( pArea );
      hb_xfree( pIndex );
      hb_xfree( szFileName );
      hb_xfree( pFileName );
      return FAILURE;
   }

   if( hb_ntxHeaderLoad( pIndex, pFileName->szName ) == FAILURE )
   {
      hb_xfree( pIndex );
      hb_xfree( szFileName );
      hb_xfree( pFileName );
      hb_fsClose( pIndex->DiskFile );
      return FAILURE;
   }
   pIndex->IndexName = ( char * ) hb_xgrab( _POSIX_PATH_MAX + 1 );
   strncpy( pIndex->IndexName, szFileName, _POSIX_PATH_MAX );

   if( pArea->lpNtxIndex )
   {
      pIndex->TagRoot++;
      pIndexNext = pArea->lpNtxIndex;
      while( pIndexNext->pNext )
      {
         pIndex->TagRoot++;
         pIndexNext = pIndexNext->pNext;
      }
      pIndexNext->pNext = pIndex;
   }
   else
   {
      pArea->lpNtxIndex = pIndex;
   }
   if( !pArea->lpCurIndex )
      pArea->lpCurIndex = pIndex;
   hb_xfree( szFileName );
   hb_xfree( pFileName );
   return SUCCESS;
}

static ERRCODE ntxOrderListClear( NTXAREAP pArea )
{
   LPNTXINDEX pIndex, pIndexNext;

   HB_TRACE(HB_TR_DEBUG, ("ntxOrderListClear(%p)", pArea));

   pIndex = pArea->lpNtxIndex;
   while( pIndex )
   {
      pIndexNext = pIndex->pNext;
      hb_ntxIndexFree( pIndex );
      pIndex = pIndexNext;
   }
   pArea->lpNtxIndex = NULL;
   pArea->lpCurIndex = NULL;
   return SUCCESS;
}

static ERRCODE ntxOrderListFocus( NTXAREAP pArea, LPDBORDERINFO pOrderInfo )
{
   LPNTXINDEX pIndex;

   HB_TRACE(HB_TR_DEBUG, ("ntxOrderListFocus(%p, %p)", pArea, pOrderInfo));

   hb_itemPutC( pOrderInfo->itmResult, (pArea->lpCurIndex)? pArea->lpCurIndex->CompoundTag->TagName:"" );

   if( pOrderInfo->itmOrder )
   {
      if( hb_itemType( pOrderInfo->itmOrder ) != HB_IT_STRING &&
                  hb_itemGetNI( pOrderInfo->itmOrder ) == 0 )
         pArea->lpCurIndex = NULL;
      else
      {
         pIndex = ntxFindIndex( pArea, pOrderInfo->itmOrder );
         if( pIndex )
            pArea->lpCurIndex = pIndex;
      }
   }

   return SUCCESS;
}

static ERRCODE ntxOrderListRebuild( NTXAREAP pArea )
{
   LPNTXINDEX lpIndex, lpIndexTmp;

   HB_TRACE(HB_TR_DEBUG, ("ntxOrderListRebuild(%p)", pArea));

   lpIndex = pArea->lpNtxIndex;
   lpIndexTmp = pArea->lpCurIndex;
   pArea->fValidBuffer = TRUE;
   while( lpIndex )
   {
      if( lpIndex->CompoundTag->RootPage )
         hb_ntxPageFree( lpIndex->CompoundTag->RootPage,TRUE );
      hb_fsSeek( lpIndex->DiskFile, NTXBLOCKSIZE, FS_SET );
      hb_fsWrite( lpIndex->DiskFile, NULL, 0 );
      hb_ntxIndexCreate( lpIndex );

      hb_ntxHeaderSave( lpIndex, FALSE );
      {
         BYTE emptyBuffer[250];
         memset( emptyBuffer, 0, 250 );
         hb_fsWrite( lpIndex->DiskFile, emptyBuffer, 250 );
      }
      hb_fsSeek( lpIndex->DiskFile , 0 , 0 );
      lpIndex->CompoundTag->TagBlock =
              hb_fsSeek( lpIndex->DiskFile, 0, SEEK_END ) - 1024;

      lpIndex = lpIndex->pNext;
   }
   pArea->lpCurIndex = lpIndexTmp;
   return SELF_GOTOP( ( AREAP ) pArea );
}

static ERRCODE ntxClose( NTXAREAP pArea )
{
   HB_TRACE(HB_TR_DEBUG, ("ntxClose(%p)", pArea));

   if( SELF_GOCOLD( ( AREAP ) pArea ) == FAILURE )
      return FAILURE;
   ntxOrderListClear( pArea );
   return SUPER_CLOSE( ( AREAP ) pArea );
}

static ERRCODE ntxSetScope( NTXAREAP pArea, LPDBORDSCOPEINFO sInfo )
{
   if( pArea->lpCurIndex )
   {
      LPTAGINFO pTag = pArea->lpCurIndex->CompoundTag;
      USHORT nKeyType = hb_ntxGetKeyType( pTag );
      PHB_ITEM *ppItem = (sInfo->nScope)? &(pTag->bottomScope):&(pTag->topScope);
      char szBuffer[ NTX_MAX_KEY ];

      pTag->keyCount = 0;
      if( !sInfo->scopeValue )
      {
         hb_itemRelease( *ppItem );
         *ppItem = NULL;
      }
      else
         switch( nKeyType )
         {
            case HB_IT_STRING:
               if ( sInfo->scopeValue->type == HB_IT_STRING )
               {
                  if( *ppItem == NULL )
                     *ppItem = hb_itemNew( NULL );
                  hb_itemCopy( *ppItem, sInfo->scopeValue );
               }
               break;
            case HB_IT_INTEGER:
            case HB_IT_LONG:
            case HB_IT_DOUBLE:
               if ( sInfo->scopeValue->type & HB_IT_NUMERIC )
               {
                  if( *ppItem == NULL )
                     *ppItem = hb_itemNew( NULL );
                  hb_itemPutC( *ppItem, numToStr( sInfo->scopeValue, szBuffer, pTag->KeyLength, pTag->KeyDec ) );
               }
               break;
           case HB_IT_DATE:
               if ( sInfo->scopeValue->type == HB_IT_DATE )
               {
                  if( *ppItem == NULL )
                     *ppItem = hb_itemNew( NULL );
                  hb_itemGetDS( sInfo->scopeValue, szBuffer );
                  hb_itemPutC( *ppItem,szBuffer );
               }
               break;
         }
   }
   return SUCCESS;
}

static RDDFUNCS ntxTable = { ntxBof,
                             ntxEof,
                             ntxFound,
                             ( DBENTRYP_V ) ntxGoBottom,
                             ( DBENTRYP_UL ) ntxGoTo,
                             ntxGoToId,
                             ( DBENTRYP_V ) ntxGoTop,
                             ( DBENTRYP_BIB ) ntxSeek,
                             ntxSkip,
                             ntxSkipFilter,
                             ( DBENTRYP_L ) ntxSkipRaw,
                             ntxAddField,
                             ( DBENTRYP_B ) ntxAppend,
                             ntxCreateFields,
                             ntxDeleteRec,
                             ntxDeleted,
                             ntxFieldCount,
                             ntxFieldDisplay,
                             ntxFieldInfo,
                             ntxFieldName,
                             ntxFlush,
                             ntxGetRec,
                             ntxGetValue,
                             ntxGetVarLen,
                             ( DBENTRYP_V ) ntxGoCold,
                             ( DBENTRYP_V ) ntxGoHot,
                             ntxPutRec,
                             ntxPutValue,
                             ntxRecall,
                             ntxRecCount,
                             ntxRecInfo,
                             ntxRecNo,
                             ntxSetFieldsExtent,
                             ntxAlias,
                             ( DBENTRYP_V ) ntxClose,
                             ntxCreate,
                             ntxInfo,
                             ntxNewArea,
                             ntxOpen,
                             ntxRelease,
                             ( DBENTRYP_SP ) ntxStructSize,
                             ( DBENTRYP_P ) ntxSysName,
                             ntxEval,
                             ( DBENTRYP_V ) ntxPack,
                             ntPackRec,
                             ntxSort,
                             ntxTrans,
                             ntxTransRec,
                             ( DBENTRYP_V ) ntxZap,
                             ntxchildEnd,
                             ntxchildStart,
                             ntxchildSync,
                             ntxsyncChildren,
                             ntxclearRel,
                             ntxforceRel,
                             ntxrelArea,
                             ntxrelEval,
                             ntxrelText,
                             ntxsetRel,
                             ( DBENTRYP_OI ) ntxOrderListAdd,
                             ( DBENTRYP_V ) ntxOrderListClear,
                             ntxOrderListDelete,
                             ( DBENTRYP_OI ) ntxOrderListFocus,
                             ( DBENTRYP_V ) ntxOrderListRebuild,
                             ntxOrderCondition,
                             ( DBENTRYP_VOC ) ntxOrderCreate,
                             ntxOrderDestroy,
                             ( DBENTRYP_OII ) ntxOrderInfo,
                             ntxClearFilter,
                             ntxClearLocate,
                             ( DBENTRYP_V ) ntxClearScope,
                             ntxCountScope,
                             ntxFilterText,
                             ( DBENTRYP_SI ) ntxScopeInfo,
                             ntxSetFilter,
                             ntxSetLocate,
                             ( DBENTRYP_VOS ) ntxSetScope,
                             ntxSkipScope,
                             ntxCompile,
                             ntxError,
                             ntxEvalBlock,
                             ntxRawLock,
                             ntxLock,
                             ntxUnLock,
                             ntxCloseMemFile,
                             ntxCreateMemFile,
                             ntxGetValueFile,
                             ntxOpenMemFile,
                             ntxPutValueFile,
                             ntxReadDBHeader,
                             ntxWriteDBHeader,
                             ntxWhoCares
                           };

HB_FUNC(_DBFNTX )
{
}

HB_FUNC( DBFNTX_GETFUNCTABLE )
{
   RDDFUNCS * pTable;
   USHORT * uiCount;

   uiCount = ( USHORT * ) hb_itemGetPtr( hb_param( 1, HB_IT_POINTER ) );
   * uiCount = RDDFUNCSCOUNT;
   pTable = ( RDDFUNCS * ) hb_itemGetPtr( hb_param( 2, HB_IT_POINTER ) );
   if( pTable )
      hb_retni( hb_rddInherit( pTable, &ntxTable, &ntxSuper, ( BYTE * ) "DBF" ) );
   else
      hb_retni( FAILURE );
}
