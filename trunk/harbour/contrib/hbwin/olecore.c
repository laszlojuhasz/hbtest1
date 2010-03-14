/*
 * $Id$
 */

/*
 * Harbour Project source code:
 * OLE library
 *
 * Copyright 2000, 2003 Jose F. Gimenez (JFG) <jfgimenez@wanadoo.es>
 * Copyright 2008, 2009 Mindaugas Kavaliauskas <dbtopas at dbtopas.lt>
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

#include "hbwinole.h"
#include "hbinit.h"

/* enable workaround for wrong OLE variant structure definition */
#if ( defined( __POCC__ ) && defined( HB_OS_WIN_CE ) ) || \
    defined( __DMC__ ) || \
    ( defined( _MSC_VER ) && ( _MSC_VER <= 1500 ) )
#  define HB_OLE_NO_LL
#endif

#if ( defined( __POCC__ ) && defined( HB_OS_WIN_CE ) ) || \
    ( defined( __WATCOMC__ ) && ( __WATCOMC__ < 1280 ) ) || \
    defined( __MINGW32__ ) || \
    defined( __DMC__ ) || \
    ( defined( _MSC_VER ) && ( _MSC_VER <= 1500 ) )
#  define HB_OLE_NO_LLREF
#endif


/* base date value in OLE (1899-12-30) as julian day */
#define HB_OLE_DATE_BASE      0x0024D9AB

static PHB_DYNS s_pDyns_hb_oleauto;
static PHB_DYNS s_pDyns_hObjAccess;
static PHB_DYNS s_pDyns_hObjAssign;

typedef struct
{
   IDispatch*  pDisp;
   PHB_ITEM*   pCallBack;
} HB_OLE;

typedef struct
{
   HRESULT  lOleError;
} HB_OLEDATA, * PHB_OLEDATA;

static HB_TSD_NEW( s_oleData, sizeof( HB_OLEDATA ), NULL, NULL );
#define hb_getOleData()       ( ( PHB_OLEDATA ) hb_stackGetTSD( &s_oleData ) )


HB_FUNC_EXTERN( WIN_OLEAUTO );


void hb_oleSetError( HRESULT lOleError )
{
   hb_getOleData()->lOleError = lOleError;
}


HRESULT hb_oleGetError( void )
{
   return hb_getOleData()->lOleError;
}


static void hb_olecore_init( void* cargo )
{
   HB_SYMBOL_UNUSED( cargo );

   s_pDyns_hb_oleauto = hb_dynsymGetCase( "WIN_OLEAUTO" );
   s_pDyns_hObjAccess = hb_dynsymGetCase( "__HOBJ" );
   s_pDyns_hObjAssign = hb_dynsymGetCase( "___HOBJ" );

   if( s_pDyns_hObjAccess == s_pDyns_hObjAssign )
   {
      /* Never executed. Just force linkage */
      HB_FUNC_EXEC( WIN_OLEAUTO );
   }

   hb_oleInit();
}


static HB_GARBAGE_FUNC( hb_ole_destructor )
{
   HB_OLE * pOle = ( HB_OLE * ) Cargo;
   IDispatch * pDisp = pOle->pDisp;

   if( pDisp )
   {
      pOle->pDisp = NULL;
      if( pOle->pCallBack && *pOle->pCallBack )
      {
         PHB_ITEM pCallBack = *pOle->pCallBack;
         *pOle->pCallBack = NULL;
         pOle->pCallBack = NULL;
         hb_itemRelease( pCallBack );
      }
      HB_VTBL( pDisp )->Release( HB_THIS( pDisp ) );
   }
}

static HB_GARBAGE_FUNC( hb_ole_mark )
{
   HB_OLE * pOle = ( HB_OLE * ) Cargo;

   if( pOle->pCallBack && *pOle->pCallBack )
      hb_gcMark( *pOle->pCallBack );
}

static const HB_GC_FUNCS s_gcOleFuncs =
{
   hb_ole_destructor,
   hb_ole_mark
};


static HB_GARBAGE_FUNC( hb_oleenum_destructor )
{
   IEnumVARIANT** ppEnum = ( IEnumVARIANT** ) Cargo;
   IEnumVARIANT*  pEnum = *ppEnum;

   if( pEnum )
   {
      *ppEnum = NULL;
      HB_VTBL( pEnum )->Release( HB_THIS( pEnum ) );
   }
}

static const HB_GC_FUNCS s_gcOleenumFuncs =
{
   hb_oleenum_destructor,
   hb_gcDummyMark
};


static void hb_errRT_OLE( HB_ERRCODE errGenCode, HB_ERRCODE errSubCode, HB_ERRCODE errOsCode, const char * szDescription, const char * szOperation )
{
   PHB_ITEM pError;
   pError = hb_errRT_New( ES_ERROR, "WINOLE", errGenCode, errSubCode, szDescription, szOperation, errOsCode, EF_NONE );

   if( hb_pcount() != 0 )
   {
      /* HB_ERR_ARGS_BASEPARAMS */
      PHB_ITEM  pArray = hb_arrayBaseParams();
      hb_errPutArgsArray( pError, pArray );
      hb_itemRelease( pArray );
   }
   hb_errLaunch( pError );
   hb_errRelease( pError );
}


IDispatch* hb_oleParam( int iParam )
{
   HB_OLE * pOle = ( HB_OLE * ) hb_parptrGC( &s_gcOleFuncs, iParam );

   if( pOle && pOle->pDisp )
      return pOle->pDisp;

   hb_errRT_OLE( EG_ARG, 1001, 0, NULL, HB_ERR_FUNCNAME );
   return NULL;
}


IDispatch* hb_oleItemGet( PHB_ITEM pItem )
{
   HB_OLE * pOle = ( HB_OLE * ) hb_itemGetPtrGC( pItem, &s_gcOleFuncs );
   return pOle ? pOle->pDisp : NULL;
}


PHB_ITEM hb_oleItemPut( PHB_ITEM pItem, IDispatch* pDisp )
{
   HB_OLE * pOle = ( HB_OLE * ) hb_gcAllocate( sizeof( HB_OLE ), &s_gcOleFuncs );

   pOle->pDisp = pDisp;
   pOle->pCallBack = NULL;

   return hb_itemPutPtrGC( pItem, pOle );
}


PHB_ITEM hb_oleItemGetCallBack( PHB_ITEM pItem )
{
   HB_OLE * pOle = ( HB_OLE * ) hb_itemGetPtrGC( pItem, &s_gcOleFuncs );
   return pOle && pOle->pCallBack ? *pOle->pCallBack : NULL;
}


void hb_oleItemSetCallBack( PHB_ITEM pItem, PHB_ITEM* pCallBack )
{
   HB_OLE * pOle = ( HB_OLE * ) hb_itemGetPtrGC( pItem, &s_gcOleFuncs );

   if( pOle )
   {
      if( pOle->pCallBack && *pOle->pCallBack )
      {
         PHB_ITEM pCallBack = *pOle->pCallBack;
         *pOle->pCallBack = NULL;
         pOle->pCallBack = NULL;
         hb_itemRelease( pCallBack );
      }
      if( pCallBack )
      {
         pOle->pCallBack = pCallBack;
         hb_gcUnlock( *pCallBack );
      }
   }
}


static IEnumVARIANT* hb_oleenumParam( int iParam )
{
   IEnumVARIANT**  ppEnum = ( IEnumVARIANT** ) hb_parptrGC( &s_gcOleenumFuncs, iParam );

   if( ppEnum && *ppEnum )
      return *ppEnum;

   hb_errRT_OLE( EG_ARG, 1002, 0, NULL, HB_ERR_FUNCNAME );
   return NULL;
}


/* Unicode string management */

static wchar_t* AnsiToWide( const char* szString )
{
   int       iLen;
   wchar_t*  szWide;

   iLen = MultiByteToWideChar( CP_ACP, MB_PRECOMPOSED, szString, -1, NULL, 0 );
   szWide = ( wchar_t* ) hb_xgrab( iLen * sizeof( wchar_t ) );
   MultiByteToWideChar( CP_ACP, MB_PRECOMPOSED, szString, -1, szWide, iLen );
   return szWide;
}

static void AnsiToWideBuffer( const char* szString, wchar_t* szWide, int iLen )
{
   MultiByteToWideChar( CP_ACP, MB_PRECOMPOSED, szString, -1, szWide, iLen );
   szWide[ iLen - 1 ] = '0';
}

static BSTR hb_oleItemToString( PHB_ITEM pItem )
{
   const char* szString;
   BSTR strVal;
   int iLen, iStrLen;

   szString = hb_itemGetCPtr( pItem );
   iLen = ( int ) hb_itemGetCLen( pItem );
   iStrLen = MultiByteToWideChar( CP_ACP, MB_PRECOMPOSED, szString, iLen, NULL, 0 );
   strVal = SysAllocStringLen( NULL, iStrLen );
   MultiByteToWideChar( CP_ACP, MB_PRECOMPOSED, szString, iLen, strVal, iStrLen + 1 );
   return strVal;
}


static void hb_oleStringToItem( BSTR strVal, PHB_ITEM pItem )
{
   char* szString;
   int iLen, iStrLen;

   iStrLen = ( int ) SysStringLen( strVal );
   iLen = WideCharToMultiByte( CP_ACP, 0, strVal, iStrLen, NULL, 0, NULL, NULL );
   szString = ( char* ) hb_xgrab( ( iLen + 1 ) * sizeof( char ) );
   WideCharToMultiByte( CP_ACP, 0, strVal, iStrLen, szString, iLen + 1, NULL, NULL );
   hb_itemPutCLPtr( pItem, szString, iLen );
}


static IDispatch * hb_oleItemGetDispatch( PHB_ITEM pItem )
{
   if( HB_IS_OBJECT( pItem ) )
   {
      if( hb_clsIsParent( hb_objGetClass( pItem ), "WIN_OLEAUTO" ) )
      {
         hb_vmPushDynSym( s_pDyns_hObjAccess );
         hb_vmPush( pItem );
         hb_vmSend( 0 );

         return hb_oleParam( -1 );
      }
   }
   return hb_oleItemGet( pItem );
}


static void hb_oleDispatchToVariant( VARIANT* pVariant, IDispatch* pDisp,
                                     VARIANT* pVarRef )
{
   /* pVariant will be freed using VariantClear().
      We increment reference count to keep OLE object alive */
   HB_VTBL( pDisp )->AddRef( HB_THIS( pDisp ) );
   pVariant->n1.n2.vt = VT_DISPATCH;
   pVariant->n1.n2.n3.pdispVal = pDisp;
   if( pVarRef )
   {
      pVarRef->n1.n2.vt = VT_DISPATCH | VT_BYREF;
      pVarRef->n1.n2.n3.ppdispVal = &pVariant->n1.n2.n3.pdispVal;
   }
}


/* Item <-> Variant conversion */

static void hb_oleItemToVariantRef( VARIANT* pVariant, PHB_ITEM pItem,
                                    VARIANT* pVarRef )
{
   VariantClear( pVariant );  /* pVariant->n1.n2.vt = VT_EMPTY; */

   switch( hb_itemType( pItem ) )
   {
      case HB_IT_STRING:
      case HB_IT_MEMO:
         pVariant->n1.n2.vt = VT_BSTR;
         pVariant->n1.n2.n3.bstrVal = hb_oleItemToString( pItem );
         if( pVarRef )
         {
            pVarRef->n1.n2.vt = VT_BSTR | VT_BYREF;
            pVarRef->n1.n2.n3.pbstrVal = &pVariant->n1.n2.n3.bstrVal;
         }
         break;

      case HB_IT_LOGICAL:
         pVariant->n1.n2.vt = VT_BOOL;
         pVariant->n1.n2.n3.boolVal = hb_itemGetL( pItem ) ? VARIANT_TRUE : VARIANT_FALSE;
         if( pVarRef )
         {
            pVarRef->n1.n2.vt = VT_BOOL | VT_BYREF;
            pVarRef->n1.n2.n3.pboolVal = &pVariant->n1.n2.n3.boolVal;
         }
         break;

      case HB_IT_INTEGER:
         pVariant->n1.n2.vt = VT_I4;
         pVariant->n1.n2.n3.lVal = hb_itemGetNL( pItem );
         if( pVarRef )
         {
            pVarRef->n1.n2.vt = VT_I4 | VT_BYREF;
            pVarRef->n1.n2.n3.plVal = &pVariant->n1.n2.n3.lVal;
         }
         break;

      case HB_IT_LONG:
#if HB_LONG_MAX == INT32_MAX || defined( HB_LONG_LONG_OFF )
         pVariant->n1.n2.vt = VT_I4;
         pVariant->n1.n2.n3.lVal = hb_itemGetNL( pItem );
         if( pVarRef )
         {
            pVarRef->n1.n2.vt = VT_I4 | VT_BYREF;
            pVarRef->n1.n2.n3.plVal = &pVariant->n1.n2.n3.lVal;
         }
#else
         pVariant->n1.n2.vt = VT_I8;
#  if defined( HB_OLE_NO_LL )
         /* workaround for wrong OLE variant structure definition */
         * ( ( HB_LONGLONG * ) &pVariant->n1.n2.n3.lVal ) = hb_itemGetNInt( pItem );
#  else
         pVariant->n1.n2.n3.llVal = hb_itemGetNInt( pItem );
#  endif
         if( pVarRef )
         {
            pVarRef->n1.n2.vt = VT_I8 | VT_BYREF;
#  if defined( HB_OLE_NO_LLREF ) || defined( HB_OLE_NO_LL )
            /* workaround for wrong OLE variant structure definition */
            pVarRef->n1.n2.n3.pdblVal = &pVariant->n1.n2.n3.dblVal;
#  else
            pVarRef->n1.n2.n3.pllVal = &pVariant->n1.n2.n3.llVal;
#  endif
         }
#endif
         break;

      case HB_IT_DOUBLE:
         pVariant->n1.n2.vt = VT_R8;
         pVariant->n1.n2.n3.dblVal = hb_itemGetND( pItem );
         if( pVarRef )
         {
            pVarRef->n1.n2.vt = VT_R8 | VT_BYREF;
            pVarRef->n1.n2.n3.pdblVal = &pVariant->n1.n2.n3.dblVal;
         }
         break;

      case HB_IT_DATE:
         pVariant->n1.n2.vt = VT_DATE;
         pVariant->n1.n2.n3.dblVal = ( double ) ( hb_itemGetDL( pItem ) - HB_OLE_DATE_BASE );
         if( pVarRef )
         {
            pVarRef->n1.n2.vt = VT_DATE | VT_BYREF;
            pVarRef->n1.n2.n3.pdblVal = &pVariant->n1.n2.n3.dblVal;
         }
         break;

      case HB_IT_TIMESTAMP:
         pVariant->n1.n2.vt = VT_DATE;
         pVariant->n1.n2.n3.dblVal = hb_itemGetTD( pItem ) - HB_OLE_DATE_BASE;
         if( pVarRef )
         {
            pVarRef->n1.n2.vt = VT_DATE | VT_BYREF;
            pVarRef->n1.n2.n3.pdblVal = &pVariant->n1.n2.n3.dblVal;
         }
         break;

      case HB_IT_POINTER:
      {
         IDispatch * pDisp = hb_oleItemGet( pItem );

         if( pDisp )
            hb_oleDispatchToVariant( pVariant, pDisp, pVarRef );
#ifdef HB_OLE_PASS_POINTERS
         else
         {
            pVariant->n1.n2.vt = VT_PTR;
            pVariant->n1.n2.n3.byref = hb_itemGetPtr( pItem );
            if( pVarRef )
            {
               pVarRef->n1.n2.vt = VT_PTR | VT_BYREF;
               pVarRef->n1.n2.n3.byref = &pVariant->n1.n2.n3.byref;
            }
         }
#endif
         break;
      }
      case HB_IT_ARRAY: /* or OBJECT */
         if( HB_IS_OBJECT( pItem ) )
         {
            IDispatch * pDisp = hb_oleItemGetDispatch( pItem );

            if( pDisp )
               hb_oleDispatchToVariant( pVariant, pDisp, pVarRef );
         }
         else
         {
            SAFEARRAY*      pSafeArray;
            SAFEARRAYBOUND  sabound[ 1 ];
            HB_SIZE         ul, ulLen;

            ulLen = hb_arrayLen( pItem );

            sabound[ 0 ].lLbound = 0;
            sabound[ 0 ].cElements = ( long ) ulLen;

            pSafeArray = SafeArrayCreate( VT_VARIANT, 1, sabound );
            pVariant->n1.n2.vt = VT_VARIANT | VT_ARRAY;
            pVariant->n1.n2.n3.parray = pSafeArray;
            if( pVarRef )
            {
               pVarRef->n1.n2.vt = VT_VARIANT | VT_ARRAY | VT_BYREF;
               pVarRef->n1.n2.n3.pparray = &pVariant->n1.n2.n3.parray;
            }

            for( ul = 0; ul < ulLen; ul++ )
            {
               VARIANT  vItem;
               long     lIndex[ 1 ];

               VariantInit( &vItem );
               hb_oleItemToVariantRef( &vItem, hb_arrayGetItemPtr( pItem, ul + 1 ), NULL );
               lIndex[ 0 ] = ( long ) ul;
               SafeArrayPutElement( pSafeArray, lIndex, &vItem );
               VariantClear( &vItem );
            }
         }
         break;

      default:
         if( pVarRef )
         {
            pVarRef->n1.n2.vt = VT_VARIANT | VT_BYREF;
            pVarRef->n1.n2.n3.pvarVal = pVariant;
         }
   }

/* enabling this code may allow to exchange parameters by reference
 * without strong typing restrictions but I do not know if such method
 * is honored by other OLE code
 */
#if 0
   if( pVarRef )
   {
      pVarRef->n1.n2.vt = VT_VARIANT | VT_BYREF;
      pVarRef->n1.n2.n3.pvarVal = pVariant;
   }
#endif
}


void hb_oleItemToVariant( VARIANT* pVariant, PHB_ITEM pItem )
{
   hb_oleItemToVariantRef( pVariant, pItem, NULL );
}


static void hb_oleSafeArrayToItem( PHB_ITEM pItem, SAFEARRAY * pSafeArray,
                                   int iDim, long * plIndex, VARTYPE vt )
{
   long lFrom, lTo;

   SafeArrayGetLBound( pSafeArray, iDim, &lFrom );
   SafeArrayGetUBound( pSafeArray, iDim, &lTo );

   if( lFrom <= lTo )
   {
      HB_SIZE ul = 0;

      hb_arrayNew( pItem, lTo - lFrom + 1 );
      if( --iDim == 0 )
      {
         VARIANT vItem;
         VariantInit( &vItem );
         do
         {
            plIndex[ iDim ] = lFrom;
            /* hack: for non VT_VARIANT arrays create VARIANT dynamically
             *       using pointer to union in variant structure which
             *       holds all variant values except VT_DECIMAL which is
             *       stored in different place.
             */
            if( SafeArrayGetElement( pSafeArray, plIndex,
                                     vt == VT_VARIANT ? ( void * ) &vItem :
                                     ( vt == VT_DECIMAL ? ( void * ) &vItem.n1.decVal :
                                     ( void * ) &vItem.n1.n2.n3 ) ) == S_OK )
            {
               if( vt != VT_VARIANT )
                  vItem.n1.n2.vt = vt; /* it's reserved in VT_DECIMAL structure */
               hb_oleVariantToItem( hb_arrayGetItemPtr( pItem, ++ul ), &vItem );
               VariantClear( &vItem );
            }
         }
         while( ++lFrom <= lTo );
      }
      else
      {
         do
         {
            plIndex[ iDim ] = lFrom;
            hb_oleSafeArrayToItem( hb_arrayGetItemPtr( pItem, ++ul ),
                                   pSafeArray, iDim, plIndex, vt );
         }
         while( ++lFrom <= lTo );
      }
   }
   else
      hb_arrayNew( pItem, 0 );
}


static void hb_oleDispatchToItem( PHB_ITEM pItem, IDispatch* pdispVal )
{
   if( pdispVal )
   {
      if( hb_vmRequestReenter() )
      {
         PHB_ITEM pObject, pPtrGC;

         hb_vmPushDynSym( s_pDyns_hb_oleauto );
         hb_vmPushNil();
         hb_vmDo( 0 );

         pObject = hb_itemNew( hb_stackReturnItem() );

         pPtrGC = hb_oleItemPut( NULL, pdispVal );
         /* Item is one more copy of the object */
         HB_VTBL( pdispVal )->AddRef( HB_THIS( pdispVal ) );

         hb_vmPushDynSym( s_pDyns_hObjAssign );
         hb_vmPush( pObject );
         hb_vmPush( pPtrGC );
         hb_vmSend( 1 );
         hb_itemRelease( pPtrGC );
         hb_vmRequestRestore();

         /* We should store object to pItem after hb_vmRequestRestore(),
          * because pItem actualy can be stack's return item!
          */
         hb_itemMove( pItem, pObject );
         hb_itemRelease( pObject );
      }
   }
}


void hb_oleVariantToItem( PHB_ITEM pItem, VARIANT* pVariant )
{
   if( pVariant->n1.n2.vt == ( VT_VARIANT | VT_BYREF ) )
      pVariant = pVariant->n1.n2.n3.pvarVal;

   switch( pVariant->n1.n2.vt )
   {
      case VT_UNKNOWN:
      case VT_UNKNOWN | VT_BYREF:
      {
         IDispatch* pdispVal = NULL;
         IUnknown* punkVal = pVariant->n1.n2.vt == VT_UNKNOWN ?
                             pVariant->n1.n2.n3.punkVal :
                             *pVariant->n1.n2.n3.ppunkVal;
         hb_itemClear( pItem );
         if( punkVal && HB_VTBL( punkVal )->QueryInterface(
                              HB_THIS_( punkVal ) HB_ID_REF( IID_IDispatch ),
                              ( void** ) ( void * ) &pdispVal ) == S_OK )
         {
            hb_oleDispatchToItem( pItem, pdispVal );
            HB_VTBL( pdispVal )->Release( HB_THIS( pdispVal ) );
         }
         break;
      }

      case VT_DISPATCH:
      case VT_DISPATCH | VT_BYREF:
         hb_itemClear( pItem );
         hb_oleDispatchToItem( pItem, pVariant->n1.n2.vt == VT_DISPATCH ?
                                      pVariant->n1.n2.n3.pdispVal :
                                      *pVariant->n1.n2.n3.ppdispVal );
         break;

      case VT_BSTR:
         hb_oleStringToItem( pVariant->n1.n2.n3.bstrVal, pItem );
         break;

      case VT_BSTR | VT_BYREF:
         hb_oleStringToItem( *pVariant->n1.n2.n3.pbstrVal, pItem );
         break;

      case VT_BOOL:
         hb_itemPutL( pItem, pVariant->n1.n2.n3.boolVal ? HB_TRUE : HB_FALSE );
         break;

      case VT_BOOL | VT_BYREF:
         hb_itemPutL( pItem, *pVariant->n1.n2.n3.pboolVal ? HB_TRUE : HB_FALSE );
         break;

      case VT_I1:
         hb_itemPutNI( pItem, ( signed char ) pVariant->n1.n2.n3.cVal );
         break;

      case VT_I1 | VT_BYREF:
         hb_itemPutNI( pItem, ( signed char ) *pVariant->n1.n2.n3.pcVal );
         break;

      case VT_I2:
         hb_itemPutNI( pItem, ( short ) pVariant->n1.n2.n3.iVal );
         break;

      case VT_I2 | VT_BYREF:
         hb_itemPutNI( pItem, ( short ) *pVariant->n1.n2.n3.piVal );
         break;

      case VT_I4:
         hb_itemPutNL( pItem, pVariant->n1.n2.n3.lVal );
         break;

      case VT_I4 | VT_BYREF:
         hb_itemPutNL( pItem, *pVariant->n1.n2.n3.plVal );
         break;

      case VT_I8:
#if HB_LONG_MAX == INT32_MAX || defined( HB_LONG_LONG_OFF )
         hb_itemPutNInt( pItem, ( HB_MAXINT ) pVariant->n1.n2.n3.lVal );
#elif defined( HB_OLE_NO_LL )
         /* workaround for wrong OLE variant structure definition */
         hb_itemPutNInt( pItem, * ( ( HB_LONGLONG * ) &pVariant->n1.n2.n3.lVal ) );
#else
         hb_itemPutNInt( pItem, pVariant->n1.n2.n3.llVal );
#endif
         break;

      case VT_I8 | VT_BYREF:
#if HB_LONG_MAX == INT32_MAX || defined( HB_LONG_LONG_OFF )
         hb_itemPutNInt( pItem, ( HB_MAXINT ) *pVariant->n1.n2.n3.plVal );
#elif defined( HB_OLE_NO_LLREF )
         /* workaround for wrong OLE variant structure definition */
         hb_itemPutNInt( pItem, * ( HB_LONGLONG * ) pVariant->n1.n2.n3.pdblVal );
#else
         hb_itemPutNInt( pItem, *pVariant->n1.n2.n3.pllVal );
#endif
         break;

      case VT_UI1:
         hb_itemPutNI( pItem, ( unsigned char ) pVariant->n1.n2.n3.bVal );
         break;

      case VT_UI1 | VT_BYREF:
         hb_itemPutNI( pItem, ( unsigned char ) *pVariant->n1.n2.n3.pbVal );
         break;

      case VT_UI2:
         hb_itemPutNI( pItem, ( unsigned short ) pVariant->n1.n2.n3.uiVal );
         break;

      case VT_UI2 | VT_BYREF:
         hb_itemPutNI( pItem, ( unsigned short ) *pVariant->n1.n2.n3.puiVal );
         break;

      case VT_UI4:
         hb_itemPutNInt( pItem, pVariant->n1.n2.n3.ulVal );
         break;

      case VT_UI4 | VT_BYREF:
         hb_itemPutNInt( pItem, *pVariant->n1.n2.n3.pulVal );
         break;

      case VT_UI8:
         /* TODO: sign is lost. Convertion to double will lose significant digits. */
#if HB_LONG_MAX == INT32_MAX || defined( HB_LONG_LONG_OFF )
         hb_itemPutNInt( pItem, ( HB_MAXINT ) pVariant->n1.n2.n3.ulVal );
#elif defined( HB_OLE_NO_LL )
         /* workaround for wrong OLE variant structure definition */
         hb_itemPutNInt( pItem, * ( ( HB_LONGLONG * ) &pVariant->n1.n2.n3.ulVal ) );
#else
         hb_itemPutNInt( pItem, ( HB_MAXINT ) pVariant->n1.n2.n3.ullVal );
#endif
         break;

      case VT_UI8 | VT_BYREF:
         /* TODO: sign is lost. Convertion to double will lose significant digits. */
#if HB_LONG_MAX == INT32_MAX || defined( HB_LONG_LONG_OFF )
         hb_itemPutNInt( pItem, ( HB_MAXINT ) *pVariant->n1.n2.n3.pulVal );
#elif defined( HB_OLE_NO_LLREF )
         /* workaround for wrong OLE variant structure definition */
         hb_itemPutNInt( pItem, * ( HB_LONGLONG * ) pVariant->n1.n2.n3.pdblVal );
#else
         hb_itemPutNInt( pItem, ( HB_MAXINT ) *pVariant->n1.n2.n3.pullVal );
#endif
         break;

      case VT_INT:
         hb_itemPutNI( pItem, pVariant->n1.n2.n3.intVal );
         break;

      case VT_INT | VT_BYREF:
         hb_itemPutNI( pItem, *pVariant->n1.n2.n3.pintVal );
         break;

      case VT_UINT:
         hb_itemPutNInt( pItem, pVariant->n1.n2.n3.uintVal );
         break;

      case VT_UINT | VT_BYREF:
         hb_itemPutNInt( pItem, *pVariant->n1.n2.n3.puintVal );
         break;

      case VT_ERROR:
         hb_itemPutNInt( pItem, pVariant->n1.n2.n3.scode );
         break;

      case VT_ERROR | VT_BYREF:
         hb_itemPutNInt( pItem, *pVariant->n1.n2.n3.pscode );
         break;

      case VT_R4:
         hb_itemPutND( pItem, ( double ) pVariant->n1.n2.n3.fltVal );
         break;

      case VT_R4 | VT_BYREF:
         hb_itemPutND( pItem, ( double ) *pVariant->n1.n2.n3.pfltVal );
         break;

      case VT_R8:
         hb_itemPutND( pItem, pVariant->n1.n2.n3.dblVal );
         break;

      case VT_R8 | VT_BYREF:
         hb_itemPutND( pItem, *pVariant->n1.n2.n3.pdblVal );
         break;

      case VT_CY:
      case VT_CY | VT_BYREF:
      {
         double dblVal;
         VarR8FromCy( pVariant->n1.n2.vt == VT_CY ?
                      pVariant->n1.n2.n3.cyVal :
                      *pVariant->n1.n2.n3.pcyVal, &dblVal );
         hb_itemPutND( pItem, dblVal );
         /* hb_itemPutNDLen( pItem, dblVal, 0, 4 ); */
         break;
      }

      case VT_DECIMAL:
      case VT_DECIMAL | VT_BYREF:
      {
         double dblVal;
         VarR8FromDec( pVariant->n1.n2.vt == VT_DECIMAL ?
                       &pVariant->n1.decVal :
                       pVariant->n1.n2.n3.pdecVal, &dblVal );
         hb_itemPutND( pItem, dblVal );
         break;
      }

      case VT_DATE:
      case VT_DATE | VT_BYREF:
      {
         long lJulian, lMilliSec;
         double dblVal = pVariant->n1.n2.vt == VT_DATE ?
                         pVariant->n1.n2.n3.dblVal :
                         *pVariant->n1.n2.n3.pdblVal;

         hb_timeStampUnpackDT( dblVal + HB_OLE_DATE_BASE, &lJulian, &lMilliSec );
         if( lMilliSec )
            hb_itemPutTDT( pItem, lJulian, lMilliSec );
         else
            hb_itemPutDL( pItem, lJulian );
         break;
      }

#ifdef HB_OLE_PASS_POINTERS
      case VT_PTR:
      case VT_PTR | VT_BYREF:
      case VT_BYREF:
         hb_itemPutPtr( pItem, pVariant->n1.n2.n3.byref );
         break;
#endif

      case VT_EMPTY:
      case VT_EMPTY | VT_BYREF:
      case VT_NULL:
      case VT_NULL | VT_BYREF:
         hb_itemClear( pItem );
         break;

      default:
         if( pVariant->n1.n2.vt & VT_ARRAY )
         {
            SAFEARRAY * pSafeArray = pVariant->n1.n2.vt & VT_BYREF ?
                                     *pVariant->n1.n2.n3.pparray :
                                     pVariant->n1.n2.n3.parray;
            if( pSafeArray )
            {
               int  iDim;

               if( ( iDim = ( int ) SafeArrayGetDim( pSafeArray ) ) >= 1 )
               {
                  long * plIndex = ( long * ) hb_xgrab( iDim * sizeof( long ) );

                  hb_oleSafeArrayToItem( pItem, pSafeArray, iDim, plIndex,
                           ( pVariant->n1.n2.vt & ~( VT_ARRAY | VT_BYREF ) ) );
                  hb_xfree( plIndex );
               }
               else
                  hb_arrayNew( pItem, 0 );
               break;
            }
         }
         /* possible RT error - unsupported variant */
         hb_itemClear( pItem );
   }
}


void hb_oleVariantUpdate( VARIANT* pVariant, PHB_ITEM pItem )
{
   switch( pVariant->n1.n2.vt )
   {
      case VT_DISPATCH | VT_BYREF:
      {
         IDispatch * pDisp = hb_oleItemGetDispatch( pItem );

         if( pDisp )
         {
            IDispatch* pdispVal = *pVariant->n1.n2.n3.ppdispVal;
            if( pdispVal != pDisp )
            {
               HB_VTBL( pDisp )->AddRef( HB_THIS( pDisp ) );
               *pVariant->n1.n2.n3.ppdispVal = pDisp;
               if( pdispVal )
                  HB_VTBL( pdispVal )->Release( HB_THIS( pdispVal ) );
            }
         }
         break;
      }

      case VT_UNKNOWN | VT_BYREF:
      {
         IDispatch* pDisp = hb_oleItemGetDispatch( pItem );

         if( pDisp )
         {
            IUnknown* pUnk = NULL;

            if( HB_VTBL( pDisp )->QueryInterface( HB_THIS_( pDisp )
                                       HB_ID_REF( IID_IEnumVARIANT ),
                                       ( void** ) ( void * ) &pUnk ) == S_OK )
            {
               IUnknown* punkVal = *pVariant->n1.n2.n3.ppunkVal;
               if( punkVal != pUnk )
               {
                  HB_VTBL( pUnk )->AddRef( HB_THIS( pUnk ) );
                  *pVariant->n1.n2.n3.ppunkVal = pUnk;
                  if( punkVal )
                     HB_VTBL( punkVal )->Release( HB_THIS( punkVal ) );
               }
            }
         }
         break;
      }

      case VT_BSTR | VT_BYREF:
         SysFreeString( *pVariant->n1.n2.n3.pbstrVal );
         *pVariant->n1.n2.n3.pbstrVal = hb_oleItemToString( pItem );
         break;

      case VT_BOOL | VT_BYREF:
         *pVariant->n1.n2.n3.pboolVal = ( VARIANT_BOOL ) hb_itemGetL( pItem );
         break;

      case VT_I1 | VT_BYREF:
         *pVariant->n1.n2.n3.pcVal = ( signed char ) hb_itemGetNI( pItem );
         break;

      case VT_I2 | VT_BYREF:
         *pVariant->n1.n2.n3.piVal = ( short ) hb_itemGetNI( pItem );
         break;

      case VT_I4 | VT_BYREF:
         *pVariant->n1.n2.n3.plVal = hb_itemGetNL( pItem );
         break;

      case VT_I8 | VT_BYREF:
#if HB_LONG_MAX == INT32_MAX || defined( HB_LONG_LONG_OFF )
         *pVariant->n1.n2.n3.plVal = ( long ) hb_itemGetNInt( pItem );
#elif defined( HB_OLE_NO_LLREF )
         /* workaround for wrong OLE variant structure definition */
         * ( HB_LONGLONG * ) pVariant->n1.n2.n3.pdblVal = ( HB_LONGLONG ) hb_itemGetNInt( pItem );
#else
         *pVariant->n1.n2.n3.pllVal = ( HB_LONGLONG ) hb_itemGetNInt( pItem );
#endif
         break;

      case VT_UI1 | VT_BYREF:
         *pVariant->n1.n2.n3.pbVal = ( unsigned char ) hb_itemGetNI( pItem );
         break;

      case VT_UI2 | VT_BYREF:
         *pVariant->n1.n2.n3.puiVal = ( unsigned short ) hb_itemGetNI( pItem );
         break;

      case VT_UI4 | VT_BYREF:
         *pVariant->n1.n2.n3.pulVal = ( unsigned long ) hb_itemGetNL( pItem );
         break;

      case VT_UI8 | VT_BYREF:
#if HB_LONG_MAX == INT32_MAX || defined( HB_LONG_LONG_OFF )
         *pVariant->n1.n2.n3.pulVal = ( unsigned long ) hb_itemGetNInt( pItem );
#elif defined( HB_OLE_NO_LLREF )
         /* workaround for wrong OLE variant structure definition */
         * ( HB_ULONGLONG * ) pVariant->n1.n2.n3.pdblVal = ( HB_ULONGLONG ) hb_itemGetNInt( pItem );
#else
         *pVariant->n1.n2.n3.pullVal = ( HB_ULONGLONG ) hb_itemGetNInt( pItem );
#endif
         break;

      case VT_INT | VT_BYREF:
         *pVariant->n1.n2.n3.pintVal = hb_itemGetNI( pItem );
         break;

      case VT_UINT | VT_BYREF:
         *pVariant->n1.n2.n3.puintVal = ( unsigned int ) hb_itemGetNI( pItem );
         break;

      case VT_ERROR | VT_BYREF:
         *pVariant->n1.n2.n3.pscode = ( SCODE ) hb_itemGetNL( pItem );
         break;

      case VT_R4 | VT_BYREF:
         *pVariant->n1.n2.n3.pfltVal = ( float ) hb_itemGetND( pItem );
         break;

      case VT_R8 | VT_BYREF:
         *pVariant->n1.n2.n3.pdblVal = hb_itemGetND( pItem );
         break;

      case VT_CY | VT_BYREF:
         VarCyFromR8( hb_itemGetND( pItem ), pVariant->n1.n2.n3.pcyVal );
         break;

      case VT_DECIMAL | VT_BYREF:
         VarDecFromR8( hb_itemGetND( pItem ), pVariant->n1.n2.n3.pdecVal );
         break;

      case VT_DATE | VT_BYREF:
         *pVariant->n1.n2.n3.pdblVal = hb_itemGetTD( pItem ) - HB_OLE_DATE_BASE;
         break;

#ifdef HB_OLE_PASS_POINTERS
      case VT_PTR | VT_BYREF:
         pVariant->n1.n2.n3.byref = hb_itemGetPtr( pItem );
         break;
#endif

      case VT_BYREF | VT_VARIANT:
         hb_oleItemToVariantRef( pVariant->n1.n2.n3.pvarVal, pItem, NULL );
         break;

      case VT_VARIANT | VT_ARRAY | VT_BYREF:
         /* TODO: */
         break;
   }
}


/* IDispatch parameters, return value handling */

static void GetParams( DISPPARAMS * dispparam )
{
   VARIANTARG   *pArgs = NULL, *pRefs;
   UINT         uiArgCount, uiArg, uiRefs;

   uiArgCount = ( UINT ) hb_pcount();

   if( uiArgCount > 0 )
   {
      uiRefs = 0;
      for( uiArg = 0; uiArg < uiArgCount; uiArg++ )
      {
         if( HB_ISBYREF( uiArg + 1 ) )
            uiRefs++;
      }

      pArgs = ( VARIANTARG* ) hb_xgrab( sizeof( VARIANTARG ) * ( uiArgCount + uiRefs ) );
      pRefs = &pArgs[ uiArgCount ];

      for( uiArg = 0; uiArg < uiArgCount; uiArg++ )
      {
         VariantInit( &pArgs[ uiArg ] );
         if( HB_ISBYREF( uiArgCount - uiArg ) )
         {
            VariantInit( pRefs );
            hb_oleItemToVariantRef( pRefs, hb_param( uiArgCount - uiArg, HB_IT_ANY ),
                                    &pArgs[ uiArg ] );
            ++pRefs;
         }
         else
            hb_oleItemToVariantRef( &pArgs[ uiArg ], hb_param( uiArgCount - uiArg, HB_IT_ANY ), NULL );
      }
   }

   dispparam->rgvarg = pArgs;
   dispparam->cArgs  = uiArgCount;
   dispparam->rgdispidNamedArgs = 0;
   dispparam->cNamedArgs = 0;
}

static void PutParams( DISPPARAMS * dispparam )
{
   VARIANTARG* pRefs = &dispparam->rgvarg[ dispparam->cArgs ];
   PHB_ITEM pItem = NULL;
   UINT uiArg;

   for( uiArg = 1; uiArg <= dispparam->cArgs; uiArg++ )
   {
      if( HB_ISBYREF( uiArg ) )
      {
         if( !pItem )
            pItem = hb_itemNew( NULL );
         hb_oleVariantToItem( pItem, &dispparam->rgvarg[ dispparam->cArgs - uiArg ] );
         hb_itemParamStoreForward( ( HB_USHORT ) uiArg, pItem );
         VariantClear( pRefs );
         pRefs++;
      }
   }
   if( pItem )
      hb_itemRelease( pItem );
}

static void FreeParams( DISPPARAMS * dispparam )
{
   UINT  ui;

   if( dispparam->cArgs > 0 )
   {
      for( ui = 0; ui < dispparam->cArgs; ui++ )
         VariantClear( &dispparam->rgvarg[ ui ] );

      hb_xfree( dispparam->rgvarg );
   }
}


/* PRG level functions and methods */

HB_FUNC( __OLECREATEOBJECT ) /* ( cOleName | cCLSID  [, cIID ] ) */
{
   wchar_t*    cCLSID;
   GUID        ClassID, iid = IID_IDispatch;
   IDispatch*  pDisp = NULL;
   const char* cOleName = hb_parc( 1 );
   const char* cID = hb_parc( 2 );
   HRESULT     lOleError;

   if( cOleName )
   {
      cCLSID = AnsiToWide( cOleName );
      if( cOleName[ 0 ] == '{' )
         lOleError = CLSIDFromString( (LPOLESTR) cCLSID, &ClassID );
      else
         lOleError = CLSIDFromProgID( (LPCOLESTR) cCLSID, &ClassID );
      hb_xfree( cCLSID );

      if( cID )
      {
         if( cID[ 0 ] == '{' )
         {
            cCLSID = AnsiToWide( cID );
            lOleError = CLSIDFromString( ( LPOLESTR ) cCLSID, &iid );
            hb_xfree( cCLSID );
         }
         else if( hb_parclen( 2 ) == ( HB_SIZE ) sizeof( iid ) )
         {
            memcpy( ( LPVOID ) &iid, cID, sizeof( iid ) );
         }
      }

      if( lOleError == S_OK )
         lOleError = CoCreateInstance( HB_ID_REF( ClassID ), NULL, CLSCTX_SERVER, HB_ID_REF( iid ), ( void** ) ( void * ) &pDisp );
   }
   else
      lOleError = CO_E_CLASSSTRING;

   hb_oleSetError( lOleError );
   if( lOleError == S_OK )
      hb_oleItemPut( hb_stackReturnItem(), pDisp );
   else
      hb_ret();
}


HB_FUNC( __OLEGETACTIVEOBJECT ) /* ( cOleName | cCLSID  [, cIID ] ) */
{
   BSTR        wCLSID;
   IID         ClassID, iid = IID_IDispatch;
   IDispatch*  pDisp = NULL;
   IUnknown*   pUnk = NULL;
   const char* cOleName = hb_parc( 1 );
   const char* cID = hb_parc( 2 );
   HRESULT     lOleError;

   if( cOleName )
   {
      wCLSID = ( BSTR ) AnsiToWide( ( LPSTR ) cOleName );
      if( cOleName[ 0 ] == '{' )
         lOleError = CLSIDFromString( wCLSID, ( LPCLSID ) &ClassID );
      else
         lOleError = CLSIDFromProgID( wCLSID, ( LPCLSID ) &ClassID );
      hb_xfree( wCLSID );

      if( cID )
      {
         if( cID[ 0 ] == '{' )
         {
            wCLSID = ( BSTR ) AnsiToWide( ( LPSTR ) cID );
            lOleError = CLSIDFromString( wCLSID, &iid );
            hb_xfree( wCLSID );
         }
         else if( hb_parclen( 2 ) == ( HB_SIZE ) sizeof( iid ) )
         {
            memcpy( ( LPVOID ) &iid, cID, sizeof( iid ) );
         }
      }

      if( lOleError == S_OK )
      {
         lOleError = GetActiveObject( HB_ID_REF( ClassID ), NULL, &pUnk );

         if ( lOleError == S_OK )
            lOleError = HB_VTBL( pUnk )->QueryInterface( HB_THIS_( pUnk ) HB_ID_REF( iid ), ( void** ) ( void * ) &pDisp );
      }
   }
   else
      lOleError = CO_E_CLASSSTRING;

   hb_oleSetError( lOleError );
   if( lOleError == S_OK )
      hb_oleItemPut( hb_stackReturnItem(), pDisp );
   else
      hb_ret();
}


HB_FUNC( __OLEENUMCREATE ) /* ( __hObj ) */
{
   IDispatch *    pDisp = hb_oleParam( 1 );
   IEnumVARIANT * pEnum;
   VARIANTARG     variant;
   DISPPARAMS     dispparam;
   EXCEPINFO      excep;
   UINT           uiArgErr;
   HRESULT        lOleError;

   if( hb_parl( 2 ) )
   {
      hb_oleSetError( S_OK );
      hb_errRT_OLE( EG_UNSUPPORTED, 1003, 0, NULL, HB_ERR_FUNCNAME );
      return;
   }

   memset( &excep, 0, sizeof( excep ) );
   memset( &dispparam, 0, sizeof( dispparam ) ); /* empty parameters */
   VariantInit( &variant );

   lOleError = HB_VTBL( pDisp )->Invoke( HB_THIS_( pDisp ) DISPID_NEWENUM, HB_ID_REF( IID_NULL ),
                                         LOCALE_USER_DEFAULT,
                                         DISPATCH_PROPERTYGET,
                                         &dispparam, &variant, &excep, &uiArgErr );

   if( lOleError == S_OK )
   {
      if( variant.n1.n2.vt == VT_UNKNOWN )
         lOleError = HB_VTBL( variant.n1.n2.n3.punkVal )->QueryInterface(
                            HB_THIS_( variant.n1.n2.n3.punkVal )
                            HB_ID_REF( IID_IEnumVARIANT ), ( void** ) ( void * ) &pEnum );
      else if( variant.n1.n2.vt == VT_DISPATCH )
         lOleError = HB_VTBL( variant.n1.n2.n3.pdispVal )->QueryInterface(
                            HB_THIS_( variant.n1.n2.n3.pdispVal )
                            HB_ID_REF( IID_IEnumVARIANT ), ( void** ) ( void * ) &pEnum );
      else
      {
         hb_oleSetError( lOleError );
         hb_errRT_OLE( EG_ARG, 1004, ( HB_ERRCODE ) lOleError, NULL, HB_ERR_FUNCNAME );
         return;
      }

      VariantClear( &variant );

      if( lOleError == S_OK )
      {
         IEnumVARIANT**  ppEnum;

         hb_oleSetError( S_OK );

         ppEnum = ( IEnumVARIANT** ) hb_gcAllocate( sizeof( IEnumVARIANT* ), &s_gcOleenumFuncs );
         *ppEnum = pEnum;
         hb_retptrGC( ppEnum );
         return;
      }
   }
   hb_oleSetError( lOleError );
   hb_errRT_OLE( EG_ARG, 1005, ( HB_ERRCODE ) lOleError, NULL, HB_ERR_FUNCNAME );
}


HB_FUNC( __OLEENUMNEXT )
{
   IEnumVARIANT * pEnum = hb_oleenumParam( 1 );
   VARIANTARG     variant;

   VariantInit( &variant );
   if( HB_VTBL( pEnum )->Next( HB_THIS_( pEnum ) 1, &variant, NULL ) == S_OK )
   {
      hb_oleVariantToItem( hb_stackReturnItem(), &variant );
      VariantClear( &variant );
      hb_storl( HB_TRUE, 2 );
   }
   else
      hb_storl( HB_FALSE, 2 );
}


HB_FUNC( WIN_OLEERROR )
{
   hb_retnl( hb_oleGetError() );
}


HB_FUNC( WIN_OLEERRORTEXT )
{
   HRESULT  lOleError;

   if( HB_ISNUM( 1 ) )
      lOleError = hb_parnl( 1 );
   else
      lOleError = hb_oleGetError();

   switch( lOleError )
   {
      case S_OK:                    hb_retc_null();                              break;
      case CO_E_CLASSSTRING:        hb_retc_const( "CO_E_CLASSSTRING" );         break;
      case OLE_E_WRONGCOMPOBJ:      hb_retc_const( "OLE_E_WRONGCOMPOBJ" );       break;
      case REGDB_E_CLASSNOTREG:     hb_retc_const( "REGDB_E_CLASSNOTREG" );      break;
      case REGDB_E_WRITEREGDB:      hb_retc_const( "REGDB_E_WRITEREGDB" );       break;
      case E_OUTOFMEMORY:           hb_retc_const( "E_OUTOFMEMORY" );            break;
      case E_INVALIDARG:            hb_retc_const( "E_INVALIDARG" );             break;
      case E_UNEXPECTED:            hb_retc_const( "E_UNEXPECTED" );             break;
      case DISP_E_UNKNOWNNAME:      hb_retc_const( "DISP_E_UNKNOWNNAME" );       break;
      case DISP_E_UNKNOWNLCID:      hb_retc_const( "DISP_E_UNKNOWNLCID" );       break;
      case DISP_E_BADPARAMCOUNT:    hb_retc_const( "DISP_E_BADPARAMCOUNT" );     break;
      case DISP_E_BADVARTYPE:       hb_retc_const( "DISP_E_BADVARTYPE" );        break;
      case DISP_E_EXCEPTION:        hb_retc_const( "DISP_E_EXCEPTION" );         break;
      case DISP_E_MEMBERNOTFOUND:   hb_retc_const( "DISP_E_MEMBERNOTFOUND" );    break;
      case DISP_E_NONAMEDARGS:      hb_retc_const( "DISP_E_NONAMEDARGS" );       break;
      case DISP_E_OVERFLOW:         hb_retc_const( "DISP_E_OVERFLOW" );          break;
      case DISP_E_PARAMNOTFOUND:    hb_retc_const( "DISP_E_PARAMNOTFOUND" );     break;
      case DISP_E_TYPEMISMATCH:     hb_retc_const( "DISP_E_TYPEMISMATCH" );      break;
      case DISP_E_UNKNOWNINTERFACE: hb_retc_const( "DISP_E_UNKNOWNINTERFACE" );  break;
      case DISP_E_PARAMNOTOPTIONAL: hb_retc_const( "DISP_E_PARAMNOTOPTIONAL" );  break;
      default:
      {
         char   buf[ 16 ];

         hb_snprintf( buf, 16, "0x%08x", ( UINT ) ( HB_PTRUINT ) lOleError );
         hb_retc( buf );
      }
   }
}


HB_FUNC( WIN_OLEAUTO___ONERROR )
{
   IDispatch*  pDisp;
   const char* szMethod;
   wchar_t     szMethodWide[ HB_SYMBOL_NAME_LEN + 1 ];
   OLECHAR*    pMemberArray;
   DISPID      dispid;
   DISPPARAMS  dispparam;
   VARIANTARG  variant;
   EXCEPINFO   excep;
   UINT        uiArgErr;
   HRESULT     lOleError;

   /* Get object handle */
   hb_vmPushDynSym( s_pDyns_hObjAccess );
   hb_vmPush( hb_stackSelfItem() );
   hb_vmSend( 0 );

   pDisp = hb_oleParam( -1 );
   if( !pDisp )
      return;

   szMethod = hb_itemGetSymbol( hb_stackBaseItem() )->szName;
   AnsiToWideBuffer( szMethod, szMethodWide, ( int ) HB_SIZEOFARRAY( szMethodWide ) );

   /* Try property put */

   if( szMethod[ 0 ] == '_' && hb_pcount() > 0 )
   {
      pMemberArray = &szMethodWide[ 1 ];
      lOleError = HB_VTBL( pDisp )->GetIDsOfNames( HB_THIS_( pDisp ) HB_ID_REF( IID_NULL ), &pMemberArray,
                                                   1, LOCALE_USER_DEFAULT, &dispid );

      if( lOleError == S_OK )
      {
         DISPID     lPropPut = DISPID_PROPERTYPUT;

         memset( &excep, 0, sizeof( excep ) );
         GetParams( &dispparam );
         dispparam.rgdispidNamedArgs = &lPropPut;
         dispparam.cNamedArgs = 1;

         lOleError = HB_VTBL( pDisp )->Invoke( HB_THIS_( pDisp ) dispid, HB_ID_REF( IID_NULL ),
                                               LOCALE_USER_DEFAULT,
                                               DISPATCH_PROPERTYPUT, &dispparam,
                                               NULL, &excep, &uiArgErr );
         FreeParams( &dispparam );

         /* assign method should return assigned value */
         hb_itemReturn( hb_param( 1, HB_IT_ANY ) );

         hb_oleSetError( lOleError );
         if( lOleError != S_OK )
            hb_errRT_OLE( EG_ARG, 1006, ( HB_ERRCODE ) lOleError, NULL, HB_ERR_FUNCNAME );
         return;
      }
   }

   /* Try property get and invoke */

   pMemberArray = szMethodWide;
   lOleError = HB_VTBL( pDisp )->GetIDsOfNames( HB_THIS_( pDisp ) HB_ID_REF( IID_NULL ),
                                                &pMemberArray, 1, LOCALE_USER_DEFAULT, &dispid );

   if( lOleError == S_OK )
   {
      memset( &excep, 0, sizeof( excep ) );
      VariantInit( &variant );
      GetParams( &dispparam );

      lOleError = HB_VTBL( pDisp )->Invoke( HB_THIS_( pDisp ) dispid, HB_ID_REF( IID_NULL ),
                                            LOCALE_USER_DEFAULT,
                                            DISPATCH_PROPERTYGET | DISPATCH_METHOD,
                                            &dispparam, &variant, &excep, &uiArgErr );

      PutParams( &dispparam );
      FreeParams( &dispparam );

      hb_oleVariantToItem( hb_stackReturnItem(), &variant );
      VariantClear( &variant );

      hb_oleSetError( lOleError );
      if( lOleError != S_OK )
         hb_errRT_OLE( EG_ARG, 1007, ( HB_ERRCODE ) lOleError, NULL, HB_ERR_FUNCNAME );
      return;
   }

   hb_oleSetError( lOleError );

   /* TODO: add description containing TypeName of the object */
   if( szMethod[ 0 ] == '_' )
      hb_errRT_OLE( EG_NOVARMETHOD, 1008, ( HB_ERRCODE ) lOleError, NULL, szMethod + 1 );
   else
      hb_errRT_OLE( EG_NOMETHOD, 1009, ( HB_ERRCODE ) lOleError, NULL, szMethod );
}


HB_CALL_ON_STARTUP_BEGIN( _hb_olecore_init_ )
   hb_vmAtInit( hb_olecore_init, NULL );
HB_CALL_ON_STARTUP_END( _hb_olecore_init_ )

#if defined( HB_PRAGMA_STARTUP )
   #pragma startup _hb_olecore_init_
#elif defined( HB_DATASEG_STARTUP )
   #define HB_DATASEG_BODY    HB_DATASEG_FUNC( _hb_olecore_init_ )
   #include "hbiniseg.h"
#endif
