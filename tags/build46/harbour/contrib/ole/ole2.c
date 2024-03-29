/*
 * $Id$
 */

/*
 * Harbour Project source code:
 * OLE library
 *
 * Copyright 2000,2003 Jos� F. Gim�nez (JFG) <jfgimenez@wanadoo.es>
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

/*************************************************************************
*                                                                        *
*  CreateOleObject( cOleName | cCLSID [, cIID ] ) -> hOleObject          *
*                                                                        *
*  OleInvoke( hOleObject, cMethodName, uParam1, ..., uParamN )           *
*          -> uResult                                                    *
*                                                                        *
*  OleSetProperty( hOleObject, cPropertyName, uParam1, ..., uParamN )    *
*          -> lOk                                                        *
*                                                                        *
*  OleGetProperty( hOleObject, cPropertyName, uParam1, ..., uParamN )    *
*          -> uResult                                                    *
*                                                                        *
*  OleIsObject() -> lIsObject                                            *
*                                                                        *
*  OleError() -> nError                                                  *
*                                                                        *
*  Ole2TxtError() -> cError                                              *
*                                                                        *
*  OleUninitialize() -> Nil                                              *
*                                                                        *
\************************************************************************/

#define HB_OS_WIN_32_USED

#include <windows.h>
#include <ole2.h>

#include "hbvm.h"
#include "hbapiitm.h"
#include "hbdate.h"

static far VARIANTARG RetVal;
static EXCEPINFO excep;
static HRESULT nOleError = 0;
static int lInitialized = 0;

static double DateToDbl( LPSTR cDate )
{
   double nDate;

   nDate = hb_dateEncStr( cDate ) - 0x0024d9abL;

   return ( nDate );
}

static LPSTR DblToDate( double nDate )
{
   static char *cDate = "00000000";

   hb_dateDecStr( cDate, nDate + 0x0024d9abL );

   return ( cDate );
}

static LPSTR AnsiToWide( LPSTR cAnsi )
{
   unsigned short wLen;
   LPSTR cString;

   wLen  = MultiByteToWideChar( CP_ACP, MB_PRECOMPOSED, cAnsi, -1, 0, 0 );
   cString = ( char * ) hb_xgrab( wLen * 2 );
   MultiByteToWideChar( CP_ACP, MB_PRECOMPOSED, cAnsi, -1,
                        ( LPWSTR ) cString, wLen );
   return ( cString );
}

static LPSTR WideToAnsi( LPSTR cWide )
{
   unsigned short wLen;
   LPSTR cString;

   wLen = WideCharToMultiByte( CP_ACP, 0, ( LPWSTR ) cWide, -1,
                               NULL, 0, NULL, NULL );
   cString = hb_xgrab( (!wLen) ? 2 : wLen );
   WideCharToMultiByte( CP_ACP, 0, ( LPWSTR ) cWide, -1,
                        cString, wLen, NULL, NULL );
   return ( cString );
}

static void GetParams(DISPPARAMS * dParams)
{
   VARIANTARG * pArgs = NULL;
   PHB_ITEM uParam;
   int n, nArgs, nArg;
   LPSTR cString;

   nArgs = hb_pcount() - 2;

   if( nArgs > 0 )
   {
      pArgs = ( VARIANTARG * ) hb_xgrab( sizeof( VARIANTARG ) * nArgs );

      for( n = 0; n < nArgs; n++ )
      {
         // Los parametros en VARIANTARG[] hay que ponerlos en orden inverso
         nArg = nArgs + 2 - n;

         VariantInit( &( pArgs[ n ] ) );

         uParam = hb_param( nArg, 0xFFFF );

         switch( hb_itemType( uParam ) )
         {
            case '\0':
#if !defined(__BORLANDC__)
                 pArgs[ n ].vt   = VT_EMPTY;
#else
                 pArgs[ n ].n1.n2.vt   = VT_EMPTY;
#endif
                 break;

            case HB_IT_STRING:
            case HB_IT_MEMO:
#if !defined(__BORLANDC__)
                 pArgs[ n ].vt   = VT_BSTR;
#else
                 pArgs[ n ].n1.n2.vt   = VT_BSTR;
#endif
                 cString = AnsiToWide( hb_parc( nArg ) );
#if !defined(__BORLANDC__)
                 pArgs[ n ].bstrVal = SysAllocString( (LPVOID) cString );
#else
                 pArgs[ n ].n1.n2.n3.bstrVal = SysAllocString( (LPVOID) cString );
#endif
                    hb_xfree( cString );
                 break;

            case HB_IT_LOGICAL:
#if !defined(__BORLANDC__)
                 pArgs[ n ].vt   = VT_BOOL;
                 pArgs[ n ].boolVal = hb_parl( nArg );
#else
                 pArgs[ n ].n1.n2.vt   = VT_BOOL;
                 pArgs[ n ].n1.n2.n3.boolVal = hb_parl( nArg );
#endif
                 break;

            case HB_IT_INTEGER:
            case HB_IT_LONG:
            case HB_IT_NUMERIC:
#if !defined(__BORLANDC__)
                 pArgs[ n ].vt   = VT_I4;
                 pArgs[ n ].lVal = hb_parnl( nArg );
#else
                 pArgs[ n ].n1.n2.vt   = VT_I4;
                 pArgs[ n ].n1.n2.n3.lVal = hb_parnl( nArg );
#endif
                 break;

            case HB_IT_DOUBLE:
#if !defined(__BORLANDC__)
                 pArgs[ n ].vt   = VT_R8;
                 pArgs[ n ].dblVal = hb_parnd( nArg );
#else
                 pArgs[ n ].n1.n2.vt   = VT_R8;
                 pArgs[ n ].n1.n2.n3.dblVal = hb_parnd( nArg );
#endif
                 break;
            case HB_IT_DATE:
#if !defined(__BORLANDC__)
                 pArgs[ n ].vt   = VT_DATE;
                 pArgs[ n ].dblVal = DateToDbl( hb_pards( nArg ) );
#else
                 pArgs[ n ].n1.n2.vt   = VT_DATE;
                 pArgs[ n ].n1.n2.n3.dblVal = DateToDbl( hb_pards( nArg ) );
#endif
                 break;

            case HB_IT_OBJECT:
            {
                 PHB_DYNS pData;
#if !defined(__BORLANDC__)
                 pArgs[ n ].vt = VT_EMPTY;
#else
                 pArgs[ n ].n1.n2.vt = VT_EMPTY;
#endif
                 if ( hb_stricmp( hb_objGetClsName( uParam ), "TOleAuto" ) == 0 )
                 {
                    pData = hb_dynsymFindName( "hObj" );
                    if( pData )
                    {
                       hb_vmPushSymbol( hb_dynsymSymbol( pData ) );
                       hb_vmPush( uParam );
                       hb_vmDo( 0 );
#if !defined(__BORLANDC__)
                       pArgs[ n ].vt = VT_DISPATCH;
                       pArgs[ n ].pdispVal = ( IDispatch * ) hb_parnl( -1 );
#else
                       pArgs[ n ].n1.n2.vt = VT_DISPATCH;
                       pArgs[ n ].n1.n2.n3.pdispVal = ( IDispatch * ) hb_parnl( -1 );
#endif
                    }
                 }
             }
             break;
         }
      }
   }

   dParams->rgvarg = pArgs;
   dParams->cArgs  = nArgs;
   dParams->rgdispidNamedArgs = 0;
   dParams->cNamedArgs = 0;

}

static void FreeParams(DISPPARAMS * dParams)
{
   int n;

   if( dParams->cArgs > 0 )
   {
      for( n = 0; n < ( int ) dParams->cArgs; n++ )
         VariantClear( &(dParams->rgvarg[ n ]) );

      hb_xfree( ( LPVOID ) dParams->rgvarg );
   }
}

static void RetValue( void )
{
   LPSTR cString;

#if !defined(__BORLANDC__)
   switch( RetVal.vt )
   {
      case VT_BSTR:
           cString = WideToAnsi( ( LPSTR ) RetVal.bstrVal );
           hb_retc( cString );
           hb_xfree( cString );
           break;

      case VT_BOOL:
              hb_retl( RetVal.boolVal );
           break;

      case VT_DISPATCH:
           hb_retnl( ( LONG ) RetVal.pdispVal );
           break;

      case VT_I4:
           hb_retnl( ( LONG ) RetVal.iVal );
           break;

      case VT_R8:
           hb_retnd( RetVal.dblVal );
           break;

      case VT_DATE:
           hb_retds( DblToDate( RetVal.dblVal ) );
           break;

      case VT_EMPTY:
           hb_ret();
           break;

      default:
           if ( nOleError == S_OK )
              (LONG) nOleError = -1;
           hb_ret();
           break;
   }

   if( RetVal.vt != VT_DISPATCH )
      VariantClear( &RetVal );
#else
   switch( RetVal.n1.n2.vt )
   {
      case VT_BSTR:
           cString = WideToAnsi( ( LPSTR ) RetVal.n1.n2.n3.bstrVal );
           hb_retc( cString );
           hb_xfree( cString );
           break;

      case VT_BOOL:
              hb_retl( RetVal.n1.n2.n3.boolVal );
           break;

      case VT_DISPATCH:
           hb_retnl( ( LONG ) RetVal.n1.n2.n3.pdispVal );
           break;

      case VT_I4:
           hb_retnl( ( LONG ) RetVal.n1.n2.n3.iVal );
           break;

      case VT_R8:
           hb_retnd( RetVal.n1.n2.n3.dblVal );
           break;

      case VT_DATE:
           hb_retds( DblToDate( RetVal.n1.n2.n3.dblVal ) );
           break;

      case VT_EMPTY:
           hb_ret();
           break;

      default:
           if ( nOleError == S_OK )
              (LONG) nOleError = -1;
           hb_ret();
           break;
   }

   if( RetVal.n1.n2.vt != VT_DISPATCH )
      VariantClear( &RetVal );
#endif

}


HB_FUNC( CREATEOLEOBJECT ) // ( cOleName | cCLSID  [, cIID ] )
{
   LPSTR cCLSID;
   GUID ClassID, iid;
   LPIID riid = (LPIID) &IID_IDispatch;
   void *pDisp = NULL; /* IDispatch */
   /* void *
    * used intentionally to inform compiler that there is no
    * strict-aliasing
    */

   nOleError = S_OK;

   if ( !lInitialized )
      nOleError = OleInitialize( NULL );
   lInitialized = 1;

   if ( (nOleError == S_OK) || (nOleError == (HRESULT) S_FALSE) )
   {

      cCLSID = AnsiToWide( hb_parc( 1 ) );
      if ( hb_parc( 1 )[ 0 ] == '{' )
         nOleError = CLSIDFromString( ( LPOLESTR ) cCLSID, (LPCLSID) &ClassID );
      else
         nOleError = CLSIDFromProgID( ( LPCOLESTR ) cCLSID, (LPCLSID) &ClassID );
      hb_xfree( cCLSID );

      if ( hb_pcount() == 2 )
      {
         if ( hb_parc( 2 )[ 0 ] == '{' )
         {
            cCLSID = AnsiToWide( hb_parc( 2 ) );
            nOleError = CLSIDFromString( ( LPOLESTR ) cCLSID, &iid );
            hb_xfree( cCLSID );
         }
         else
            memcpy( ( LPVOID ) &iid, hb_parc( 2 ), sizeof( iid ) );

         riid = &iid;
      }

      if ( nOleError == S_OK )
         nOleError = CoCreateInstance( &ClassID, NULL, CLSCTX_SERVER,
                                       (REFIID) riid, &pDisp );
   }

   hb_retnl( ( LONG ) pDisp );

}

HB_FUNC( OLESHOWEXCEPTION )
{
   if ( (LONG) nOleError == DISP_E_EXCEPTION )
   {
      LPSTR source, description;

      source = WideToAnsi( (LPVOID) excep.bstrSource );
      description = WideToAnsi( (LPVOID) excep.bstrDescription );
      MessageBox( NULL, description, source, MB_ICONHAND );
      hb_xfree( source );
      hb_xfree( description );
   }
}

HB_FUNC( OLEINVOKE ) // (hOleObject, szMethodName, uParams...)
{
   IDispatch * pDisp = ( IDispatch * ) hb_parnl( 1 );
   LPSTR cMember;
   DISPID lDispID;
   DISPPARAMS dParams;
   UINT uArgErr;

   VariantInit( &RetVal );
   memset( (LPBYTE) &excep, 0, sizeof( excep ) );

   cMember = AnsiToWide( hb_parc( 2 ) );
   nOleError = pDisp->lpVtbl->GetIDsOfNames( pDisp, &IID_NULL,
                                             ( LPVOID ) &cMember, 1,
                                             LOCALE_USER_DEFAULT, &lDispID );
   hb_xfree( cMember );

   if ( nOleError == S_OK )
   {
      GetParams( &dParams );
      nOleError = pDisp->lpVtbl->Invoke( pDisp,
                                         lDispID,
                                         &IID_NULL,
                                         LOCALE_USER_DEFAULT,
                                         DISPATCH_METHOD,
                                         &dParams,
                                         &RetVal,
                                         &excep,
                                         &uArgErr ) ;
      FreeParams( &dParams );
   }

   RetValue();
}

HB_FUNC( OLESETPROPERTY ) // (hOleObject, cPropName, uValue, uParams...)
{
   IDispatch * pDisp = ( IDispatch * ) hb_parnl( 1 );
   LPSTR cMember;
   DISPID lDispID, lPropPut = DISPID_PROPERTYPUT;
   DISPPARAMS dParams;
   UINT uArgErr;

   VariantInit( &RetVal );
   memset( (LPBYTE) &excep, 0, sizeof( excep ) );

   cMember = AnsiToWide( hb_parc( 2 ) );
   nOleError = pDisp->lpVtbl->GetIDsOfNames( pDisp, &IID_NULL,
                                             ( LPVOID ) &cMember, 1,
                                             LOCALE_USER_DEFAULT, &lDispID );
   hb_xfree( cMember );

   if ( nOleError == S_OK )
   {
      GetParams( &dParams );
      dParams.rgdispidNamedArgs = &lPropPut;
      dParams.cNamedArgs = 1;

      nOleError = pDisp->lpVtbl->Invoke( pDisp,
                                         lDispID,
                                         &IID_NULL,
                                         LOCALE_USER_DEFAULT,
                                         DISPATCH_PROPERTYPUT,
                                         &dParams,
                                         NULL,    // No return value
                                         &excep,
                                         &uArgErr );

      FreeParams( &dParams );
   }

   hb_ret();
}

HB_FUNC( OLEGETPROPERTY )  // (hOleObject, cPropName, uParams...)
{
   IDispatch * pDisp = ( IDispatch * ) hb_parnl( 1 );
   LPSTR cMember;
   DISPID lDispID;
   DISPPARAMS dParams;
   UINT uArgErr;

   VariantInit( &RetVal );
   memset( (LPBYTE) &excep, 0, sizeof( excep ) );

   cMember = AnsiToWide( hb_parc( 2 ) );
   nOleError = pDisp->lpVtbl->GetIDsOfNames( pDisp, &IID_NULL,
                                             ( LPVOID ) &cMember, 1,
                                             LOCALE_USER_DEFAULT, &lDispID );
   hb_xfree( cMember );

   if ( nOleError == S_OK )
   {
      GetParams( &dParams );
      nOleError = pDisp->lpVtbl->Invoke( pDisp,
                                         lDispID,
                                         &IID_NULL,
                                         LOCALE_USER_DEFAULT,
                                         DISPATCH_PROPERTYGET,
                                         &dParams,
                                         &RetVal,
                                         &excep,
                                         &uArgErr );

      FreeParams( &dParams );
   }

   RetValue();

}

HB_FUNC( OLEERROR )
{
   hb_retnl( (LONG) nOleError );
}

HB_FUNC( OLEISOBJECT )
{
#if !defined(__BORLANDC__)
   hb_retl( RetVal.vt == VT_DISPATCH );
#else
   hb_retl( RetVal.n1.n2.vt == VT_DISPATCH );
#endif
}

HB_FUNC( OLEUNINITIALIZE )
{
   if( lInitialized )
      OleUninitialize();
   lInitialized = 0;
}

HB_FUNC( OLE2TXTERROR )
{
   switch ( (LONG) nOleError)
   {
      case S_OK:
         hb_retc( "S_OK" );
         break;

      case CO_E_CLASSSTRING:
         hb_retc( "CO_E_CLASSSTRING" );
         break;

      case OLE_E_WRONGCOMPOBJ:
         hb_retc( "OLE_E_WRONGCOMPOBJ" );
         break;

      case REGDB_E_CLASSNOTREG:
         hb_retc( "REGDB_E_CLASSNOTREG" );
         break;

      case REGDB_E_WRITEREGDB:
         hb_retc( "REGDB_E_WRITEREGDB" );
         break;

      case E_OUTOFMEMORY:
         hb_retc( "E_OUTOFMEMORY" );
         break;

      case E_INVALIDARG:
         hb_retc( "E_INVALIDARG" );
         break;

      case E_UNEXPECTED:
         hb_retc( "E_UNEXPECTED" );
         break;

      case DISP_E_UNKNOWNNAME:
         hb_retc( "DISP_E_UNKNOWNNAME" );
         break;

      case DISP_E_UNKNOWNLCID:
         hb_retc( "DISP_E_UNKNOWNLCID" );
         break;

      case DISP_E_BADPARAMCOUNT:
         hb_retc( "DISP_E_BADPARAMCOUNT" );
         break;

      case DISP_E_BADVARTYPE:
         hb_retc( "DISP_E_BADVARTYPE" );
         break;

      case DISP_E_EXCEPTION:
         hb_retc( "DISP_E_EXCEPTION" );
         break;

      case DISP_E_MEMBERNOTFOUND:
         hb_retc( "DISP_E_MEMBERNOTFOUND" );
         break;

      case DISP_E_NONAMEDARGS:
         hb_retc( "DISP_E_NONAMEDARGS" );
         break;

      case DISP_E_OVERFLOW:
         hb_retc( "DISP_E_OVERFLOW" );
         break;

      case DISP_E_PARAMNOTFOUND:
         hb_retc( "DISP_E_PARAMNOTFOUND" );
         break;

      case DISP_E_TYPEMISMATCH:
         hb_retc( "DISP_E_TYPEMISMATCH" );
         break;

      case DISP_E_UNKNOWNINTERFACE:
         hb_retc( "DISP_E_UNKNOWNINTERFACE" );
         break;

      case DISP_E_PARAMNOTOPTIONAL:
         hb_retc( "DISP_E_PARAMNOTOPTIONAL" );
         break;

      default:
         hb_retc( "Unknown error" );
         break;
   };
}

HB_FUNC( GETOLEOBJECT )
{
   BSTR wCLSID;
   IID ClassID, iid;
   LPIID riid = (LPIID) &IID_IDispatch;
   IUnknown *pUnk = NULL;
   char *cOleName = hb_parc( 1 );
   void *pDisp = NULL; /* IDispatch */
   /* void *
    * used intentionally to inform compiler that there is no
    * strict-aliasing
    */

   nOleError = S_OK;

   wCLSID = (BSTR) AnsiToWide( (LPSTR)cOleName );

   if ( cOleName[ 0 ] == '{' )
   {
      nOleError = CLSIDFromString( wCLSID, (LPCLSID) &ClassID );
   }
   else
   {
      nOleError = CLSIDFromProgID( wCLSID, (LPCLSID) &ClassID );
   }

   hb_xfree( wCLSID );

   if ( hb_pcount() == 2 )
   {
      char * cID = hb_parc( 2 );
      if ( cID[ 0 ] == '{' )
      {
         wCLSID = (BSTR)AnsiToWide( (LPSTR)cID );
         nOleError = CLSIDFromString( wCLSID, &iid );
         hb_xfree( wCLSID );
      }
      else
      {
         memcpy( ( LPVOID ) &iid, cID, sizeof( iid ) );
      }

      riid = &iid;
   }

   if ( nOleError == S_OK )
   {
      nOleError = GetActiveObject( &ClassID, NULL, &pUnk );

      if ( nOleError == S_OK )
      {
         nOleError = pUnk->lpVtbl->QueryInterface( pUnk, riid, &pDisp );
      }
   }

   hb_retnl( ( LONG ) pDisp );
}

HB_FUNC( MESSAGEBOX )
{
   hb_retni( MessageBox( ( HWND ) hb_parnl( 1 ), hb_parc( 2 ), hb_parc( 3 ), hb_parni( 4 ) ) );
}
