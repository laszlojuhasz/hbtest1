/*
 * $Id$
 */

/*
 * Harbour Project source code:
 *    xHarbour compatible wrapper functions
 *
 * Copyright 2007 Przemyslaw Czerpak <druzus / at / priv.onet.pl>
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
 * Copyright 2000 David G. Holm <dholm@jsd-llc.com>
 *    HB_F_EOF()
 *
 * See COPYING for licensing terms.
 *
 */

#include "hbapi.h"
#include "hbapifs.h"
#include "hbapigt.h"
#include "hbapiitm.h"
#include "hbapigt.h"
#include "hbapicls.h"
#include "hbapierr.h"
#include "hbvm.h"

#if 0

/* NOTE: Dangerous functions, commented out. Harbour doesn't implement hb_retclen_const() */

HB_FUNC( HB_POINTER2STRING )
{
   PHB_ITEM pPointer = hb_param( 1, HB_IT_ANY );
   PHB_ITEM pLen     = hb_param( 2, HB_IT_NUMERIC );

   if( HB_IS_POINTER( pPointer ) && pLen )
      hb_retclen_const( ( char * ) hb_itemGetPtr( pPointer ), ( ULONG ) hb_itemGetNL( pLen ) );
   else if( HB_IS_INTEGER( pPointer ) && pLen )
      hb_retclen_const( ( char * ) hb_itemGetNI( pPointer ), ( ULONG ) hb_itemGetNL( pLen ) );
   else if( HB_IS_LONG( pPointer ) && pLen )
      hb_retclen_const( ( char * ) hb_itemGetNL( pPointer ), ( ULONG ) hb_itemGetNL( pLen ) );
   else
      hb_errRT_BASE_SubstR( EG_ARG, 1099, NULL, HB_ERR_FUNCNAME, 2, hb_paramError( 1 ), hb_paramError( 2 ) );
}

HB_FUNC( HB_STRING2POINTER )
{
   PHB_ITEM pString = hb_param( 1, HB_IT_STRING );

   if( pString )
      hb_retptr( ( void * ) hb_itemGetCPtr( pString ) );
   else
      hb_errRT_BASE_SubstR( EG_ARG, 1099, NULL, HB_ERR_FUNCNAME, 1, hb_paramError( 1 ) );
}

#endif

HB_FUNC( HB_CMDARGARGV )
{
   hb_retc( hb_cmdargARGV()[0] );
}

HB_FUNC( HB_VMMODE )
{
#if   defined( HB_NO_PROFILER ) && defined( HB_NO_TRACE ) && !defined( HB_GUI )
   hb_retni( 2 ); /* optimized for console applications */
#elif defined( HB_NO_PROFILER ) && defined( HB_NO_TRACE ) &&  defined( HB_GUI )
   hb_retni( 1 ); /* optimized for gui applications */
#else
   hb_retni( 0 ); /* no optimization */
#endif
}

HB_FUNC( XHB__KEYBOARD )
{
   /* Clear the typeahead buffer without reallocating the keyboard buffer */
   if( !hb_parl( 2 ) )
   {
      hb_inkeyReset();
   }

   if( HB_ISNUM( 1 ) )
   {
      hb_inkeyPut( hb_parni( 1 ) );
   }
   else if( HB_ISCHAR( 1 ) )
   {
      hb_inkeySetText( hb_parc( 1 ), hb_parclen( 1 ) );
   }
   else if( HB_ISARRAY( 1 ) )
   {
      PHB_ITEM pArray = hb_param( 1, HB_IT_ARRAY );
      ULONG ulIndex;
      ULONG ulElements = hb_arrayLen( pArray );

      for( ulIndex = 1; ulIndex <= ulElements; ulIndex++ )
      {
         PHB_ITEM pItem = hb_arrayGetItemPtr( pArray, ulIndex );

         if( HB_IS_NUMBER( pItem ) )
         {
            hb_inkeyPut( hb_itemGetNI( pItem ) );
         }
         else if( HB_IS_STRING( pItem ) )
         {
            hb_inkeySetText( ( const char * ) hb_itemGetCPtr( pItem ), hb_itemGetCLen( pItem ) );
         }
      }
   }
}

HB_FUNC( HB_CREATELEN8 )
{
   char buffer[ 8 ];
   HB_LONG llValue;

   if( HB_ISNUM( 1 ) )
   {
      llValue = hb_parnint( 1 );
      HB_PUT_LE_UINT64( buffer, llValue );
      hb_retclen( buffer, 8 );
   }
   else if( HB_ISBYREF( 1 ) && HB_ISNUM( 2 ) )
   {
      llValue = hb_parnint( 2 );
      HB_PUT_LE_UINT64( buffer, llValue );
      hb_storclen( buffer, 8, 1 );
   }
}

HB_FUNC( HB_GETLEN8 )
{
   const char * buffer = hb_parc( 1 );
   if( buffer && hb_parclen( 1 ) >= 8 )
      hb_retnint( HB_GET_LE_UINT64( buffer ) );
   else
      hb_retni( -1 );
}

HB_FUNC_EXTERN( HB_DESERIALIZE );

HB_FUNC( HB_DESERIALBEGIN )
{
   PHB_ITEM pItem = hb_param( 1, HB_IT_STRING );
   if( pItem )
      hb_itemReturn( pItem );
}

HB_FUNC( HB_DESERIALNEXT )
{
   HB_FUNC_EXEC( HB_DESERIALIZE );
}


HB_FUNC_EXTERN( HB_WILDMATCH );

HB_FUNC( WILDMATCH )
{
   HB_FUNC_EXEC( HB_WILDMATCH );
}

HB_FUNC_EXTERN( HB_ADLER32 );

HB_FUNC( HB_CHECKSUM )
{
   HB_FUNC_EXEC( HB_ADLER32 );
}

HB_FUNC( HB_F_EOF )
{
   HB_ERRCODE uiError = 6;

   if( HB_ISNUM( 1 ) )
   {
      hb_retl( hb_fsEof( hb_numToHandle( hb_parnint( 1 ) ) ) );
      uiError = hb_fsError();
   }
   else
      hb_retl( TRUE );

   hb_fsSetFError( uiError );
}

HB_FUNC( CURDIRX )
{
   HB_ERRCODE uiErrorOld = hb_fsError();
   char * pbyBuffer = ( char * ) hb_xgrab( HB_PATH_MAX + 1 );
   PHB_ITEM pDrv = hb_param( 1, HB_IT_STRING );
   BYTE cCurDrv = hb_fsCurDrv();
   BYTE cDrv;

   if( pDrv && hb_parclen( 1 ) > 0 )
   {
      cDrv = ( BYTE ) ( HB_TOUPPER( *hb_itemGetCPtr( pDrv ) ) - 'A' );
      if( cDrv != cCurDrv )
         hb_fsChDrv( cDrv );
   }
   else
      cDrv = cCurDrv;

   /* NOTE: hb_fsCurDirBuffEx() in xhb, but I couldn't decipher the difference. [vszakats] */
   hb_fsCurDirBuff( cDrv, pbyBuffer, HB_PATH_MAX );

   hb_retc_buffer( pbyBuffer );

   hb_fsChDrv( cCurDrv );

   hb_fsSetError( uiErrorOld );
}

HB_FUNC_EXTERN( HB_CSTR );

HB_FUNC( CSTR )
{
   HB_FUNC_EXEC( HB_CSTR );
}

HB_FUNC( HB_ARRAYID )  /* for debugging: returns the array's "address" so dual references to same array can be seen */
{
   PHB_ITEM pArray = hb_param( 1, HB_IT_ARRAY );

   hb_retptr( pArray ? hb_arrayId( pArray ) : NULL );
}

HB_FUNC( __SENDRAWMSG )
{
   hb_dbg_objSendMessage( 0, hb_param( 1, HB_IT_ANY ),
                          hb_param( 2, HB_IT_ANY ), 3 );
}

HB_FUNC( HB_EXEC )
{
   if( HB_ISSYMBOL( 1 ) )
   {
      BOOL fSend = FALSE;
      int iParams = hb_pcount() - 1;

      if( iParams >= 1 )
      {
         fSend = iParams > 1 && ! HB_IS_NIL( hb_param( 2, HB_IT_ANY ) );
         iParams--;
      }
      else
         hb_vmPushNil();
      if( fSend )
         hb_vmSend( ( USHORT ) iParams );
      else
         hb_vmDo( ( USHORT ) iParams );
   }
   else
      hb_errRT_BASE_SubstR( EG_ARG, 1099, NULL, HB_ERR_FUNCNAME, HB_ERR_ARGS_BASEPARAMS );
}

/* Hash utem functions */
HB_FUNC_EXTERN( HB_HASH );
HB_FUNC_EXTERN( HB_HHASKEY );
HB_FUNC_EXTERN( HB_HPOS );
HB_FUNC_EXTERN( HB_HGET );
HB_FUNC_EXTERN( HB_HSET );
HB_FUNC_EXTERN( HB_HDEL );
HB_FUNC_EXTERN( HB_HKEYAT );
HB_FUNC_EXTERN( HB_HVALUEAT );
HB_FUNC_EXTERN( HB_HVALUEAT );
HB_FUNC_EXTERN( HB_HPAIRAT );
HB_FUNC_EXTERN( HB_HDELAT );
HB_FUNC_EXTERN( HB_HKEYS );
HB_FUNC_EXTERN( HB_HVALUES );
HB_FUNC_EXTERN( HB_HFILL );
HB_FUNC_EXTERN( HB_HCLONE );
HB_FUNC_EXTERN( HB_HCOPY );
HB_FUNC_EXTERN( HB_HMERGE );
HB_FUNC_EXTERN( HB_HEVAL );
HB_FUNC_EXTERN( HB_HSCAN );
HB_FUNC_EXTERN( HB_HCASEMATCH );
HB_FUNC_EXTERN( HB_HCASEMATCH );
HB_FUNC_EXTERN( HB_HAUTOADD );
HB_FUNC_EXTERN( HB_HAUTOADD );
HB_FUNC_EXTERN( HB_HALLOCATE );
HB_FUNC_EXTERN( HB_HDEFAULT );

HB_FUNC( HASH )               { HB_FUNC_EXEC( HB_HASH ); }
HB_FUNC( HHASKEY )            { HB_FUNC_EXEC( HB_HHASKEY ); }
HB_FUNC( HGETPOS )            { HB_FUNC_EXEC( HB_HPOS ); }
HB_FUNC( HGET )               { HB_FUNC_EXEC( HB_HGET ); }
HB_FUNC( HSET )               { HB_FUNC_EXEC( HB_HSET ); }
HB_FUNC( HDEL )               { HB_FUNC_EXEC( HB_HDEL ); }
HB_FUNC( HGETKEYAT )          { HB_FUNC_EXEC( HB_HKEYAT ); }
HB_FUNC( HGETVALUEAT )        { HB_FUNC_EXEC( HB_HVALUEAT ); }
HB_FUNC( HSETVALUEAT )        { HB_FUNC_EXEC( HB_HVALUEAT ); }
HB_FUNC( HGETPAIRAT )         { HB_FUNC_EXEC( HB_HPAIRAT ); }
HB_FUNC( HDELAT )             { HB_FUNC_EXEC( HB_HDELAT ); }
HB_FUNC( HGETKEYS )           { HB_FUNC_EXEC( HB_HKEYS ); }
HB_FUNC( HGETVALUES )         { HB_FUNC_EXEC( HB_HVALUES ); }
HB_FUNC( HFILL )              { HB_FUNC_EXEC( HB_HFILL ); }
HB_FUNC( HCLONE )             { HB_FUNC_EXEC( HB_HCLONE ); }
HB_FUNC( HCOPY )              { HB_FUNC_EXEC( HB_HCOPY ); }
HB_FUNC( HMERGE )             { HB_FUNC_EXEC( HB_HMERGE ); }
HB_FUNC( HEVAL )              { HB_FUNC_EXEC( HB_HEVAL ); }
HB_FUNC( HSCAN )              { HB_FUNC_EXEC( HB_HSCAN ); }
HB_FUNC( HSETCASEMATCH )      { HB_FUNC_EXEC( HB_HCASEMATCH ); hb_itemReturn( hb_param( 1, HB_IT_HASH ) ); }
HB_FUNC( HGETCASEMATCH )      { HB_FUNC_EXEC( HB_HCASEMATCH ); }
HB_FUNC( HSETAUTOADD )        { HB_FUNC_EXEC( HB_HAUTOADD ); hb_itemReturn( hb_param( 1, HB_IT_HASH ) ); }
HB_FUNC( HGETAUTOADD )        { HB_FUNC_EXEC( HB_HAUTOADD ); hb_retl( hb_parni( -1 ) == HB_HASH_AUTOADD_ALWAYS ); }
HB_FUNC( HALLOCATE )          { HB_FUNC_EXEC( HB_HALLOCATE ); }
HB_FUNC( HDEFAULT )           { HB_FUNC_EXEC( HB_HDEFAULT ); }

#if defined( HB_OS_DOS ) && !defined( HB_NO_DEFAULT_INET )
#  define HB_NO_DEFAULT_INET
#endif

#if !defined( HB_NO_DEFAULT_INET )

/* Inet*() functions */
HB_FUNC_EXTERN( HB_INETINIT );
HB_FUNC_EXTERN( HB_INETCLEANUP );
HB_FUNC_EXTERN( HB_INETCREATE );
HB_FUNC_EXTERN( HB_INETCLOSE );
HB_FUNC_EXTERN( HB_INETFD );
HB_FUNC_EXTERN( HB_INETSTATUS );
HB_FUNC_EXTERN( HB_INETERRORCODE );
HB_FUNC_EXTERN( HB_INETERRORDESC );
HB_FUNC_EXTERN( HB_INETCLEARERROR );
HB_FUNC_EXTERN( HB_INETCOUNT );
HB_FUNC_EXTERN( HB_INETADDRESS );
HB_FUNC_EXTERN( HB_INETPORT );
HB_FUNC_EXTERN( HB_INETTIMEOUT );
HB_FUNC_EXTERN( HB_INETTIMEOUT );
HB_FUNC_EXTERN( HB_INETCLEARTIMEOUT );
HB_FUNC_EXTERN( HB_INETTIMELIMIT );
HB_FUNC_EXTERN( HB_INETTIMELIMIT );
HB_FUNC_EXTERN( HB_INETCLEARTIMELIMIT );
HB_FUNC_EXTERN( HB_INETPERIODCALLBACK );
HB_FUNC_EXTERN( HB_INETPERIODCALLBACK );
HB_FUNC_EXTERN( HB_INETCLEARPERIODCALLBACK );
HB_FUNC_EXTERN( HB_INETRECV );
HB_FUNC_EXTERN( HB_INETRECVALL );
HB_FUNC_EXTERN( HB_INETRECVLINE );
HB_FUNC_EXTERN( HB_INETRECVENDBLOCK );
HB_FUNC_EXTERN( HB_INETDATAREADY );
HB_FUNC_EXTERN( HB_INETSEND );
HB_FUNC_EXTERN( HB_INETSENDALL );
HB_FUNC_EXTERN( HB_INETGETHOSTS );
HB_FUNC_EXTERN( HB_INETGETALIAS );
HB_FUNC_EXTERN( HB_INETSERVER );
HB_FUNC_EXTERN( HB_INETACCEPT );
HB_FUNC_EXTERN( HB_INETCONNECT );
HB_FUNC_EXTERN( HB_INETCONNECTIP );
HB_FUNC_EXTERN( HB_INETDGRAMBIND );
HB_FUNC_EXTERN( HB_INETDGRAM );
HB_FUNC_EXTERN( HB_INETDGRAMSEND );
HB_FUNC_EXTERN( HB_INETDGRAMRECV );
HB_FUNC_EXTERN( HB_INETCRLF );
HB_FUNC_EXTERN( HB_INETISSOCKET );

HB_FUNC( INETINIT )                 { HB_FUNC_EXEC( HB_INETINIT ); }
HB_FUNC( INETCLEANUP )              { HB_FUNC_EXEC( HB_INETCLEANUP ); }
HB_FUNC( INETCREATE )               { HB_FUNC_EXEC( HB_INETCREATE ); }
HB_FUNC( INETCLOSE )                { HB_FUNC_EXEC( HB_INETCLOSE ); }
HB_FUNC( INETFD )                   { HB_FUNC_EXEC( HB_INETFD ); }
HB_FUNC( INETSTATUS )               { HB_FUNC_EXEC( HB_INETSTATUS ); }
HB_FUNC( INETERRORCODE )            { HB_FUNC_EXEC( HB_INETERRORCODE ); }
HB_FUNC( INETERRORDESC )            { HB_FUNC_EXEC( HB_INETERRORDESC ); }
HB_FUNC( INETCLEARERROR )           { HB_FUNC_EXEC( HB_INETCLEARERROR ); }
HB_FUNC( INETCOUNT )                { HB_FUNC_EXEC( HB_INETCOUNT ); }
HB_FUNC( INETADDRESS )              { HB_FUNC_EXEC( HB_INETADDRESS ); }
HB_FUNC( INETPORT )                 { HB_FUNC_EXEC( HB_INETPORT ); }
HB_FUNC( INETSETTIMEOUT )           { HB_FUNC_EXEC( HB_INETTIMEOUT ); }
HB_FUNC( INETGETTIMEOUT )           { HB_FUNC_EXEC( HB_INETTIMEOUT ); }
HB_FUNC( INETCLEARTIMEOUT )         { HB_FUNC_EXEC( HB_INETCLEARTIMEOUT ); }
HB_FUNC( INETSETTIMELIMIT )         { HB_FUNC_EXEC( HB_INETTIMELIMIT ); }
HB_FUNC( INETGETTIMELIMIT )         { HB_FUNC_EXEC( HB_INETTIMELIMIT ); }
HB_FUNC( INETCLEARTIMELIMIT )       { HB_FUNC_EXEC( HB_INETCLEARTIMELIMIT ); }
HB_FUNC( INETSETPERIODCALLBACK )    { HB_FUNC_EXEC( HB_INETPERIODCALLBACK ); }
HB_FUNC( INETGETPERIODCALLBACK )    { HB_FUNC_EXEC( HB_INETPERIODCALLBACK ); }
HB_FUNC( INETCLEARPERIODCALLBACK )  { HB_FUNC_EXEC( HB_INETCLEARPERIODCALLBACK ); }
HB_FUNC( INETRECV )                 { HB_FUNC_EXEC( HB_INETRECV ); }
HB_FUNC( INETRECVALL )              { HB_FUNC_EXEC( HB_INETRECVALL ); }
HB_FUNC( INETRECVLINE )             { HB_FUNC_EXEC( HB_INETRECVLINE ); }
HB_FUNC( INETRECVENDBLOCK )         { HB_FUNC_EXEC( HB_INETRECVENDBLOCK ); }
HB_FUNC( INETDATAREADY )            { HB_FUNC_EXEC( HB_INETDATAREADY ); }
HB_FUNC( INETSEND )                 { HB_FUNC_EXEC( HB_INETSEND ); }
HB_FUNC( INETSENDALL )              { HB_FUNC_EXEC( HB_INETSENDALL ); }
HB_FUNC( INETGETHOSTS )             { HB_FUNC_EXEC( HB_INETGETHOSTS ); }
HB_FUNC( INETGETALIAS )             { HB_FUNC_EXEC( HB_INETGETALIAS ); }
HB_FUNC( INETSERVER )               { HB_FUNC_EXEC( HB_INETSERVER ); }
HB_FUNC( INETACCEPT )               { HB_FUNC_EXEC( HB_INETACCEPT ); }
HB_FUNC( INETCONNECT )              { HB_FUNC_EXEC( HB_INETCONNECT ); }
HB_FUNC( INETCONNECTIP )            { HB_FUNC_EXEC( HB_INETCONNECTIP ); }
HB_FUNC( INETDGRAMBIND )            { HB_FUNC_EXEC( HB_INETDGRAMBIND ); }
HB_FUNC( INETDGRAM )                { HB_FUNC_EXEC( HB_INETDGRAM ); }
HB_FUNC( INETDGRAMSEND )            { HB_FUNC_EXEC( HB_INETDGRAMSEND ); }
HB_FUNC( INETDGRAMRECV )            { HB_FUNC_EXEC( HB_INETDGRAMRECV ); }
HB_FUNC( INETCRLF )                 { HB_FUNC_EXEC( HB_INETCRLF ); }
HB_FUNC( ISINETSOCKET )             { HB_FUNC_EXEC( HB_INETISSOCKET ); }
HB_FUNC( INETDESTROY )              { }

#endif /* !HB_NO_DEFAULT_INET */
