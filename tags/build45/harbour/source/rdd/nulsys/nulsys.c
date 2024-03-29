/*
 * $Id$
 */

/*
 * xHarbour Project source code:
 * NULL RDD
 *
 * Copyright 2005 Przemyslaw Czerpak <druzus / at / priv.onet.pl>
 * www - http://www.xharbour.org
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
#include "hbapirdd.h"

HB_EXPORT ERRCODE hb_rddSelectWorkAreaAlias( char * szName )
{
   HB_SYMBOL_UNUSED( szName );

   return FAILURE;
}

HB_EXPORT ERRCODE hb_rddSelectWorkAreaNumber( int iArea )
{
   HB_SYMBOL_UNUSED( iArea );

   return FAILURE;
}

HB_EXPORT ERRCODE hb_rddSelectWorkAreaSymbol( PHB_SYMB pSymAlias )
{
   HB_SYMBOL_UNUSED( pSymAlias );

   return FAILURE;
}

HB_EXPORT int hb_rddGetCurrentWorkAreaNumber( void )
{
   return 0;
}

HB_EXPORT ERRCODE hb_rddFieldGet( HB_ITEM_PTR pItem, PHB_SYMB pFieldSymbol )
{
   HB_SYMBOL_UNUSED( pItem );
   HB_SYMBOL_UNUSED( pFieldSymbol );

   return FAILURE;
}

HB_EXPORT ERRCODE hb_rddFieldPut( HB_ITEM_PTR pItem, PHB_SYMB pFieldSymbol )
{
   HB_SYMBOL_UNUSED( pItem );
   HB_SYMBOL_UNUSED( pFieldSymbol );

   return FAILURE;
}

HB_EXPORT ERRCODE hb_rddGetFieldValue( HB_ITEM_PTR pItem, PHB_SYMB pFieldSymbol )
{
   HB_SYMBOL_UNUSED( pItem );
   HB_SYMBOL_UNUSED( pFieldSymbol );

   return FAILURE;
}

HB_EXPORT ERRCODE hb_rddPutFieldValue( HB_ITEM_PTR pItem, PHB_SYMB pFieldSymbol )
{
   HB_SYMBOL_UNUSED( pItem );
   HB_SYMBOL_UNUSED( pFieldSymbol );

   return FAILURE;
}

HB_EXPORT ERRCODE hb_rddGetAliasNumber( char * szAlias, int * iArea )
{
   HB_SYMBOL_UNUSED( szAlias );
   HB_SYMBOL_UNUSED( iArea );

   return FAILURE;
}

HB_EXPORT void hb_rddShutDown( void ) {}


HB_FUNC( RDDSYS ) {}


HB_FUNC( RDDNAME ) { hb_retc( NULL ); }

HB_FUNC( RDDLIST ) { hb_reta( 0 ); }


HB_FUNC( FIELDGET ) { hb_retc( NULL ); }

HB_FUNC( FIELDPUT ) { hb_retc( NULL ); }

HB_FUNC( FIELDPOS ) { hb_retni( 0 ); }

HB_FUNC( FIELDNAME ) { hb_retc( NULL ); }


HB_FUNC( DBCREATE ) {}

HB_FUNC( DBUSEAREA ) {}

HB_FUNC( DBCLOSEAREA ) {}

HB_FUNC( DBGOTO ) { hb_retni( 0 ); }

HB_FUNC( DBGOTOP ) {}

HB_FUNC( DBGOBOTTOM ) {}

HB_FUNC( DBSEEK ) { hb_retl( FALSE ); }

HB_FUNC( DBSKIP ) { hb_retni( 0 ); }

#ifdef HB_COMPAT_XPP
HB_FUNC( DBSKIPPER ) { hb_retni( 0 ); }
#endif

HB_FUNC( DBAPPEND ) {}

HB_FUNC( DBRECALL ) {}

HB_FUNC( DBDELETE ) {}

HB_FUNC( DBRLOCK ) { hb_retl( FALSE ); }

HB_FUNC( DBUNLOCK ) { hb_retl( FALSE ); }

HB_FUNC( DBRELATION ) { hb_retc( NULL ); }

HB_FUNC( DBRSELECT ) { hb_retni( 0 ); }

HB_FUNC( SELECT ) { hb_retni( 0 ); }

HB_FUNC( ALIAS ) { hb_retc( NULL ); }

HB_FUNC( USED ) { hb_retl( FALSE ); }

HB_FUNC( NETERR ) { hb_retl( FALSE ); }

HB_FUNC( LOCK ) { hb_retl( FALSE ); }

HB_FUNC( FLOCK ) { hb_retl( FALSE ); }

HB_FUNC( RLOCK ) { hb_retl( FALSE ); }

HB_FUNC( BOF ) { hb_retl( FALSE ); }

HB_FUNC( EOF ) { hb_retl( FALSE ); }

HB_FUNC( FOUND ) { hb_retl( FALSE ); }

HB_FUNC( DELETED ) { hb_retl( FALSE ); }

HB_FUNC( RECNO ) { hb_retni( 0 ); }

HB_FUNC( RECCOUNT ) { hb_parni( 0 ); }

HB_FUNC( LASTREC ) { hb_retni( 0 ); }

HB_FUNC( FCOUNT ) { hb_parni( 0 ); }


HB_FUNC( INDEXORD ) { hb_parni(1); }

HB_FUNC( INDEXKEY ) { hb_retc( NULL ); }

HB_FUNC( ORDNAME ) { hb_retc( NULL ); }

HB_FUNC( ORDKEY ) { hb_retc( NULL ); }

HB_FUNC( ORDFOR ) { hb_retc( NULL ); }
