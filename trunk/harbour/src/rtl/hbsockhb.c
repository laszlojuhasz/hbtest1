/*
 * $Id$
 */

/*
 * Harbour Project source code:
 *    Socket API wrapper functions
 *
 * Copyright 2010 Mindaugas Kavaliauskas <dbtopas / at / dbtopas.lt>
 * www - http://harbour-project.org/
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
 * HB_SOCKETGETERROR() --> nSocketError
 * HB_SOCKETGETOSERROR() --> nOSError 
 * HB_SOCKETERRORSTRING( [ nSocketErrror = hb_socketGetError() ] ) --> cError
 * HB_SOCKETGETSOCKNAME( hSocket ) --> aAddr | NIL
 * HB_SOCKETGETPEERNAME( hSocket ) --> aAddr | NIL
 * HB_SOCKETOPEN( [ nDomain = HB_SOCKET_AF_INET ] , [ nType = HB_SOCKET_PT_STREAM ], [ nProtocol = 0 ] ) --> hSocket
 * HB_SOCKETCLOSE( hSocket ) --> lSuccess
 * HB_SOCKETSHUTDOWN( hSocket, [ nMode = HB_SOCKET_SHUT_RDWR ] ) --> lSuccess
 * HB_SOCKETBIND( hSocket, aAddr ) --> lSuccess
 * HB_SOCKETLISTEN( hSocket, [ iQueueLen = 10 ] ) --> lSuccess
 * HB_SOCKETACCEPT( hSocket, [ @aAddr ], [ nTimeout = FOREVER ] ) --> hConnectionSocket
 * HB_SOCKETCONNECT( hSocket, aAddr, [ nTimeout = FOREVER ] ) --> lSuccess
 * HB_SOCKETSEND( hSocket, cBuffer, [ nLen = LEN( cBuffer ) ], [ nFlags = 0 ], [ nTimeout = FOREVER ] ) --> nBytesSent
 * HB_SOCKETSENDTO( hSocket, cBuffer, [ nLen = LEN( cBuffer ) ], [ nFlags = 0 ], aAddr, [ nTimeout = FOREVER ] ) --> nBytesSent
 * HB_SOCKETRECV( hSocket, @cBuffer, [ nLen = LEN( cBuffer ) ], [ nFlags = 0 ], [ nTimeout = FOREVER ] ) --> nBytesRecv
 * HB_SOCKETRECVFROM( hSocket, @cBuffer, [ nLen = LEN( cBuffer ) ], [ nFlags = 0 ], @aAddr, [ nTimeout = FOREVER ] ) --> nBytesRecv
 * HB_SOCKETSETBLOCKINGIO( hSocket, lValue ) --> lSuccess
 * HB_SOCKETSETNODELAY( hSocket, lValue ) --> lSuccess
 * HB_SOCKETSETREUSEADDR( hSocket, lValue ) --> lSuccess
 * HB_SOCKETSETKEEPALIVE( hSocket, lValue ) --> lSuccess
 * HB_SOCKETSETBROADCAST( hSocket, lValue ) --> lSuccess
 * HB_SOCKETSETSNDBUFSIZE( hSocket, nValue ) --> lSuccess
 * HB_SOCKETSETRCVBUFSIZE( hSocket, nValue ) --> lSuccess
 * HB_SOCKETGETSNDBUFSIZE( hSocket, @nValue ) --> lSuccess
 * HB_SOCKETGETRCVBUFSIZE( hSocket, @nValue ) --> lSuccess
 * HB_SOCKETSETMULTICAST( hSocket, [ nFamily = HB_SOCKET_AF_INET ], cAddr ) --> lSuccess
 * HB_SOCKETSELECTREAD( hSocket,  [ nTimeout = FOREVER ] ) --> nRet
 * HB_SOCKETSELECTWRITE( hSocket,  [ nTimeout = FOREVER ] ) --> nRet
 * HB_SOCKETSELECTWRITEEX( hSocket,  [ nTimeout = FOREVER ] ) --> nRet
 * HB_SOCKETSELECT( aRead, lSetRead, aWrite, lSetWrite, aExcep, lSetExcep, [ nTimeout = FOREVER ] ) --> nRet
 * HB_SOCKETRESOLVEINETADDR( cAddr, nPort ) --> aAddr | NIL
 * HB_SOCKETRESOLVEADDR( cAddr, [ nFamily = HB_SOCKET_AF_INET ] ) --> cResolved
 * HB_SOCKETGETHOSTS( cAddr, [ nFamily = HB_SOCKET_AF_INET ] ) --> aHosts
 * HB_SOCKETGETIFACES( [ nFamily ], [ lNoAliases ] ) --> aIfaces
 */

#include "hbapiitm.h"
#include "hbapierr.h"
#include "hbvm.h"
#include "hbsocket.h"

typedef struct
{
   HB_SOCKET    socket;
} HB_PRG_SOCKET, * PHB_PRG_SOCKET;


static HB_BOOL s_fInit = HB_FALSE;

static HB_GARBAGE_FUNC( hb_socket_destructor )
{
   PHB_PRG_SOCKET pSocket = ( PHB_PRG_SOCKET ) Cargo;
   if( pSocket->socket != HB_NO_SOCKET )
   {
      hb_socketClose( pSocket->socket );
      pSocket->socket = HB_NO_SOCKET;
   }
}

static const HB_GC_FUNCS s_gcSocketFuncs =
{
   hb_socket_destructor,
   hb_gcDummyMark
};

static PHB_PRG_SOCKET socketParam( int iParam )
{
   PHB_PRG_SOCKET pSocket = ( PHB_PRG_SOCKET ) hb_parptrGC( &s_gcSocketFuncs, iParam );

   if( pSocket && pSocket->socket != HB_NO_SOCKET )
      return pSocket;

   hb_errRT_BASE_SubstR( EG_ARG, 3012, NULL, HB_ERR_FUNCNAME, HB_ERR_ARGS_BASEPARAMS );
   return NULL;
}

static HB_BOOL socketaddrParam( int iParam, void ** pAddr, unsigned int * puiLen )
{
   PHB_ITEM pItem = hb_param( iParam, HB_IT_ARRAY );

   if( pItem && hb_socketAddrFromItem( pAddr, puiLen, pItem ) )
      return HB_TRUE;

   hb_errRT_BASE_SubstR( EG_ARG, 3012, NULL, HB_ERR_FUNCNAME, HB_ERR_ARGS_BASEPARAMS );
   return HB_FALSE;
}


static void socket_exit( void * cargo )
{
   HB_SYMBOL_UNUSED( cargo );

   if( s_fInit )
   {
      hb_socketCleanup();
      s_fInit = HB_FALSE;
   }
}

static void socket_init( void )
{
   if( ! s_fInit )
   {
      hb_socketInit();
      hb_vmAtQuit( socket_exit, NULL );
      s_fInit = HB_TRUE;
   }
}


HB_FUNC( HB_SOCKETGETERROR )
{
   hb_retni( hb_socketGetError() );
}

HB_FUNC( HB_SOCKETGETOSERROR )
{
   hb_retni( hb_socketGetOsError() );
}

HB_FUNC( HB_SOCKETERRORSTRING )
{
   if( HB_ISNUM( 1 ) )
     hb_retc( hb_socketErrorStr( hb_parni( 1 ) ) );
   else
     hb_retc( hb_socketErrorStr( hb_socketGetError() ) );
}

HB_FUNC( HB_SOCKETGETSOCKNAME )
{
   PHB_PRG_SOCKET pSocket = socketParam( 1 );
   if( pSocket )
   {
      void * addr;
      unsigned int len;

      if( hb_socketGetSockName( pSocket->socket, &addr, &len ) == 0 )
      {
         PHB_ITEM pItem = hb_socketAddrToItem( addr, len );

         if( addr ) 
            hb_xfree( addr );

         if( pItem )
         {
            hb_itemReturnRelease( pItem );
            return;
         }
      }
      hb_ret();
   }
}

HB_FUNC( HB_SOCKETGETPEERNAME )
{
   PHB_PRG_SOCKET pSocket = socketParam( 1 );
   if( pSocket )
   {
      void * addr;
      unsigned int len;

      if( hb_socketGetPeerName( pSocket->socket, &addr, &len ) == 0 )
      {
         PHB_ITEM pItem = hb_socketAddrToItem( addr, len );

         if( addr )
            hb_xfree( addr );

         if( pItem )
         {
            hb_itemReturnRelease( pItem );
            return;
         }
      }
      hb_ret();
   }
}

HB_FUNC( HB_SOCKETOPEN )
{
   HB_SOCKET socket;
   int iDomain = hb_parnidef( 1, HB_SOCKET_AF_INET );
   int iType = hb_parnidef( 2, HB_SOCKET_PT_STREAM );
   int iProtocol = hb_parni( 3 );

   socket_init();
   if( ( socket = hb_socketOpen( iDomain, iType, iProtocol ) ) != HB_NO_SOCKET )
   {
      PHB_PRG_SOCKET pSocket = ( PHB_PRG_SOCKET ) hb_gcAllocate( sizeof( HB_PRG_SOCKET ), 
                                                                 &s_gcSocketFuncs );
      pSocket->socket = socket;
      hb_retptrGC( pSocket );
   }
   else
      hb_retptr( NULL );
}

HB_FUNC( HB_SOCKETCLOSE )
{
   PHB_PRG_SOCKET pSocket = socketParam( 1 );
   if( pSocket )
   {
      int iRet = hb_socketClose( pSocket->socket );
      pSocket->socket = HB_NO_SOCKET;
      hb_retl( iRet == 0 );
   }
}

HB_FUNC( HB_SOCKETSHUTDOWN )
{
   PHB_PRG_SOCKET pSocket = socketParam( 1 );
   if( pSocket )
   {
      hb_retl( hb_socketShutdown( pSocket->socket, hb_parnidef( 2, HB_SOCKET_SHUT_RDWR ) ) == 0 );
   }
}

HB_FUNC( HB_SOCKETBIND )
{
   PHB_PRG_SOCKET pSocket = socketParam( 1 );
   void * addr;
   unsigned int len;

   if( pSocket && socketaddrParam( 2, &addr, &len ) )
   {
      hb_retl( hb_socketBind( pSocket->socket, addr, len ) == 0 );
      hb_xfree( addr );
   }
}

HB_FUNC( HB_SOCKETLISTEN )
{
   PHB_PRG_SOCKET pSocket = socketParam( 1 );
   if( pSocket )
   {
      hb_retl( hb_socketListen( pSocket->socket, hb_parnidef( 2, 10 ) ) == 0 );
   }
}

HB_FUNC( HB_SOCKETACCEPT )
{
   PHB_PRG_SOCKET pSocket = socketParam( 1 );
   if( pSocket )
   {
      HB_SOCKET socket;
      void * addr = NULL;
      unsigned int len;

      socket = hb_socketAccept( pSocket->socket, &addr, &len, hb_parnintdef( 3, -1 ) );

      if( socket != HB_NO_SOCKET )
      {
         PHB_PRG_SOCKET pSocket = ( PHB_PRG_SOCKET ) hb_gcAllocate( sizeof( HB_PRG_SOCKET ), 
                                                                    &s_gcSocketFuncs );
         pSocket->socket = socket;
         hb_retptrGC( pSocket );
      }
      else
         hb_retptr( NULL );
      

      if( HB_ISBYREF( 2 ) )
      {
         PHB_ITEM pItem;
         if( socket != HB_NO_SOCKET && ( pItem = hb_socketAddrToItem( addr, len ) ) != NULL )
         {
            hb_itemParamStoreForward( 2, pItem );
            hb_itemRelease( pItem );
         }
         else
            hb_stor( 2 );
      }

      if( addr )
         hb_xfree( addr );
   }
}

HB_FUNC( HB_SOCKETCONNECT )
{
   PHB_PRG_SOCKET pSocket = socketParam( 1 );
   void * addr;
   unsigned int len;

   if( pSocket && socketaddrParam( 2, &addr, &len ) )
   {
      hb_retl( hb_socketConnect( pSocket->socket, addr, len, hb_parnintdef( 3, -1 ) ) == 0 );
      hb_xfree( addr );
   }
}
      
HB_FUNC( HB_SOCKETSEND )
{
   PHB_PRG_SOCKET pSocket = socketParam( 1 );
   if( pSocket )
   {
      long  lLen = ( long ) hb_parclen( 2 );

      if( HB_ISNUM( 3 ) )
      {
         long lParam = hb_parnl( 3 );

         if( lParam >= 0 && lParam < lLen )
            lLen = lParam;
      }
      hb_retnl( hb_socketSend( pSocket->socket, hb_parc( 2 ), lLen, hb_parni( 4 ), 
                               hb_parnintdef( 5, -1 ) ) );
   }
}

HB_FUNC( HB_SOCKETSENDTO )
{
   PHB_PRG_SOCKET pSocket = socketParam( 1 );
   void * addr;
   unsigned int len;

   if( pSocket && socketaddrParam( 5, &addr, &len ) )
   {
      long  lLen = ( long ) hb_parclen( 2 );

      if( HB_ISNUM( 3 ) )
      {
         long lParam = hb_parnl( 3 );

         if( lParam >= 0 && lParam < lLen )
            lLen = lParam;
      }
      hb_retnl( hb_socketSendTo( pSocket->socket, hb_parc( 2 ), lLen, hb_parni( 4 ), 
                                 addr, len, hb_parnintdef( 6, -1 ) ) );
      hb_xfree( addr );
   }
}

HB_FUNC( HB_SOCKETRECV )
{
   PHB_PRG_SOCKET pSocket = socketParam( 1 );
   if( pSocket )
   {
      PHB_ITEM pItem = hb_param( 2, HB_IT_STRING );
      char * pBuffer;
      HB_SIZE iLen;

      if( pItem && HB_ISBYREF( 2 ) && hb_itemGetWriteCL( pItem, &pBuffer, &iLen ) )
      {
         if( HB_ISNUM( 3 ) )
         {
            long lRead = hb_parnl( 3 );
            if( lRead >= 0 && lRead < ( long ) iLen )
               iLen = lRead;
         }
         hb_retnl( hb_socketRecv( pSocket->socket, pBuffer, ( long ) iLen, 
                                  hb_parni( 4 ), hb_parnintdef( 5, -1 ) ) );
         return;
      }
      hb_errRT_BASE_SubstR( EG_ARG, 3012, NULL, HB_ERR_FUNCNAME, HB_ERR_ARGS_BASEPARAMS );
   }
}

HB_FUNC( HB_SOCKETRECVFROM )
{
   PHB_PRG_SOCKET pSocket = socketParam( 1 );
   if( pSocket )
   {
      PHB_ITEM pItem = hb_param( 2, HB_IT_STRING );
      char * pBuffer;
      HB_SIZE iLen;

      if( pItem && HB_ISBYREF( 2 ) && hb_itemGetWriteCL( pItem, &pBuffer, &iLen ) )
      {
         void * addr = NULL;
         unsigned int len;
         long lRet;

         if( HB_ISNUM( 3 ) )
         {
            long lRead = hb_parnl( 3 );
            if( lRead >= 0 && lRead < ( long ) iLen )
               iLen = lRead;
         }
         hb_retnl( lRet = hb_socketRecvFrom( pSocket->socket, pBuffer, ( long ) iLen, 
                                             hb_parni( 4 ), &addr, &len, 
                                             hb_parnintdef( 6, -1 ) ) );
         if( HB_ISBYREF( 5 ) )
         {
            PHB_ITEM pAddr; 

            if( lRet != -1 && ( pAddr = hb_socketAddrToItem( addr, len ) ) != NULL )
            {
               hb_itemParamStoreForward( 5, pAddr );
               hb_itemRelease( pAddr );
            }
            else
               hb_stor( 5 );
         } 

         if( addr )
            hb_xfree( addr );
         return;
      }
      hb_errRT_BASE_SubstR( EG_ARG, 3012, NULL, HB_ERR_FUNCNAME, HB_ERR_ARGS_BASEPARAMS );
   }
}

HB_FUNC( HB_SOCKETSETBLOCKINGIO )
{
   PHB_PRG_SOCKET pSocket = socketParam( 1 );
   if( pSocket )
   {
      hb_retl( hb_socketSetBlockingIO( pSocket->socket, hb_parl( 2 ) ) == 0 );
   }
}

HB_FUNC( HB_SOCKETSETNODELAY )
{
   PHB_PRG_SOCKET pSocket = socketParam( 1 );
   if( pSocket )
   {
      hb_retl( hb_socketSetNoDelay( pSocket->socket, hb_parl( 2 ) ) == 0 );
   }
}

HB_FUNC( HB_SOCKETSETREUSEADDR )
{
   PHB_PRG_SOCKET pSocket = socketParam( 1 );
   if( pSocket )
   {
      hb_retl( hb_socketSetReuseAddr( pSocket->socket, hb_parl( 2 ) ) == 0 );
   }
}

HB_FUNC( HB_SOCKETSETKEEPALIVE )
{
   PHB_PRG_SOCKET pSocket = socketParam( 1 );
   if( pSocket )
   {
      hb_retl( hb_socketSetKeepAlive( pSocket->socket, hb_parl( 2 ) ) == 0 );
   }
}

HB_FUNC( HB_SOCKETSETBROADCAST )
{
   PHB_PRG_SOCKET pSocket = socketParam( 1 );
   if( pSocket )
   {
      hb_retl( hb_socketSetBroadcast( pSocket->socket, hb_parl( 2 ) ) == 0 );
   }
}

HB_FUNC( HB_SOCKETSETSNDBUFSIZE )
{
   PHB_PRG_SOCKET pSocket = socketParam( 1 );
   if( pSocket )
   {
      hb_retl( hb_socketSetSndBufSize( pSocket->socket, hb_parni( 2 ) ) == 0 );
   }
}

HB_FUNC( HB_SOCKETSETRCVBUFSIZE )
{
   PHB_PRG_SOCKET pSocket = socketParam( 1 );
   if( pSocket )
   {
      hb_retl( hb_socketSetRcvBufSize( pSocket->socket, hb_parni( 2 ) ) == 0 );
   }
}

HB_FUNC( HB_SOCKETGETSNDBUFSIZE )
{
   PHB_PRG_SOCKET pSocket = socketParam( 1 );
   if( pSocket )
   {
      int size;
      hb_retl( hb_socketGetSndBufSize( pSocket->socket, &size ) == 0 );
      hb_storni( size, 2 );
   }
}

HB_FUNC( HB_SOCKETGETRCVBUFSIZE )
{
   PHB_PRG_SOCKET pSocket = socketParam( 1 );
   if( pSocket )
   {
      int size;
      hb_retl( hb_socketGetRcvBufSize( pSocket->socket, &size ) == 0 );
      hb_storni( size, 2 );
   }
}

HB_FUNC( HB_SOCKETSETMULTICAST )
{
   PHB_PRG_SOCKET pSocket = socketParam( 1 );
   if( pSocket )
   {
      hb_retl( hb_socketSetMulticast( pSocket->socket, hb_parnidef( 2, HB_SOCKET_AF_INET ), hb_parc( 3 ) ) == 0 );
   }
}

HB_FUNC( HB_SOCKETSELECTREAD )
{
   PHB_PRG_SOCKET pSocket = socketParam( 1 );
   if( pSocket )
   {
      hb_retni( hb_socketSelectRead( pSocket->socket, hb_parnintdef( 2, -1 ) ) );
   }
}

HB_FUNC( HB_SOCKETSELECTWRITE )
{
   PHB_PRG_SOCKET pSocket = socketParam( 1 );
   if( pSocket )
   {
      hb_retni( hb_socketSelectWrite( pSocket->socket, hb_parnintdef( 2, -1 ) ) );
   }
}

HB_FUNC( HB_SOCKETSELECTWRITEEX )
{
   PHB_PRG_SOCKET pSocket = socketParam( 1 );
   if( pSocket )
   {
      hb_retni( hb_socketSelectWriteEx( pSocket->socket, hb_parnintdef( 2, -1 ) ) );
   }
}

static HB_SOCKET socketSelectCallback( PHB_ITEM pItem )
{
   PHB_PRG_SOCKET pSocket = ( PHB_PRG_SOCKET ) hb_itemGetPtrGC( pItem, &s_gcSocketFuncs );

   if( pSocket && pSocket->socket != HB_NO_SOCKET )
      return pSocket->socket;

   hb_errRT_BASE_SubstR( EG_ARG, 3012, NULL, HB_ERR_FUNCNAME, HB_ERR_ARGS_BASEPARAMS );
   return HB_NO_SOCKET;
}

HB_FUNC( HB_SOCKETSELECT )
{
   socket_init();
   hb_retni( hb_socketSelect( hb_param( 1, HB_IT_ARRAY ), hb_parl( 2 ), 
                              hb_param( 3, HB_IT_ARRAY ), hb_parl( 4 ), 
                              hb_param( 5, HB_IT_ARRAY ), hb_parl( 6 ), 
                              hb_parnintdef( 7, -1 ), socketSelectCallback ) );
}

HB_FUNC( HB_SOCKETRESOLVEINETADDR )
{
   void * addr;
   unsigned int len;

   socket_init();
   if( hb_socketResolveInetAddr( &addr, &len, hb_parc( 1 ), hb_parni( 2 ) ) )
   {
      PHB_ITEM pItem = hb_socketAddrToItem( addr, len );

      if( addr )
         hb_xfree( addr );

      if( pItem )
      {
         hb_itemReturnRelease( pItem );
         return;
      }
   }
   hb_ret();
}

HB_FUNC( HB_SOCKETRESOLVEADDR )
{
   char * szAddr;

   socket_init();
   szAddr = hb_socketResolveAddr( hb_parc( 1 ), hb_parnidef( 2, HB_SOCKET_AF_INET ) );
   if( szAddr )
      hb_retc_buffer( szAddr );
   else
      hb_retc( "" );
}

HB_FUNC( HB_SOCKETGETHOSTS )
{
   PHB_ITEM pItem;

   socket_init();
   pItem = hb_socketGetHosts( hb_parc( 1 ), hb_parnidef( 2, HB_SOCKET_AF_INET ) );
   if( pItem )
      hb_itemReturnRelease( pItem );
   else
      hb_reta( 0 );
}

/*
This function is not implemented at C level, yet [Mindaugas]

HB_FUNC( HB_SOCKETGETALIASES )
{
   PHB_ITEM pItem;

   socket_init();
   pItem = hb_socketGetAliases( hb_parc( 1 ), hb_parnidef( 2, HB_SOCKET_AF_INET ) );
   if( pItem )
      hb_itemReturnRelease( pItem );
   else
      hb_reta( 0 );
}
*/

HB_FUNC( HB_SOCKETGETIFACES )
{
   PHB_ITEM pItem;

   socket_init();
   pItem = hb_socketGetIFaces( hb_parni( 1 ), hb_parl( 2 ) );
   if( pItem )
      hb_itemReturnRelease( pItem );
   else
      hb_reta( 0 );
}
