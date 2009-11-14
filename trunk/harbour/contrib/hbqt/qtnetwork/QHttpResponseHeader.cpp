/*
 * $Id$
 */

/* -------------------------------------------------------------------- */
/* WARNING: Automatically generated source file. DO NOT EDIT!           */
/*          Instead, edit corresponding .qth file,                      */
/*          or the generator tool itself, and run regenarate.           */
/* -------------------------------------------------------------------- */

/*
 * Harbour Project source code:
 * QT wrapper main header
 *
 * Copyright 2009 Pritpal Bedi <pritpal@vouchcac.com>
 *
 * Copyright 2009 Marcos Antonio Gambeta <marcosgambeta at gmail dot com>
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
/*----------------------------------------------------------------------*/

#include "hbapi.h"
#include "../hbqt.h"

/*----------------------------------------------------------------------*/
#if QT_VERSION >= 0x040500
/*----------------------------------------------------------------------*/

#include <QtCore/QPointer>

#include <QtNetwork/QHttpResponseHeader>


/* QHttpResponseHeader ()
 * QHttpResponseHeader ( const QHttpResponseHeader & header )
 * QHttpResponseHeader ( const QString & str )
 * QHttpResponseHeader ( int code, const QString & text = QString(), int majorVer = 1, int minorVer = 1 )
 */

QT_G_FUNC( release_QHttpResponseHeader )
{
   QGC_POINTER * p = ( QGC_POINTER * ) Cargo;

   HB_TRACE( HB_TR_DEBUG, ( "release_QHttpResponseHeader          p=%p", p ) );
   HB_TRACE( HB_TR_DEBUG, ( "release_QHttpResponseHeader         ph=%p", p->ph ) );

   if( p && p->ph )
   {
      ( ( QHttpResponseHeader * ) p->ph )->~QHttpResponseHeader();
      p->ph = NULL;
      HB_TRACE( HB_TR_DEBUG, ( "release_QHttpResponseHeader         Object deleted!" ) );
      #if defined(__debug__)
         just_debug( "  YES release_QHttpResponseHeader         %i B %i KB", ( int ) hb_xquery( 1001 ), hb_getMemUsed() );
      #endif
   }
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "release_QHttpResponseHeader         Object Allready deleted!" ) );
      #if defined(__debug__)
         just_debug( "  DEL release_QHttpResponseHeader" );
      #endif
   }
}

void * gcAllocate_QHttpResponseHeader( void * pObj )
{
   QGC_POINTER * p = ( QGC_POINTER * ) hb_gcAllocate( sizeof( QGC_POINTER ), gcFuncs() );

   p->ph = pObj;
   p->func = release_QHttpResponseHeader;
   #if defined(__debug__)
      just_debug( "          new_QHttpResponseHeader         %i B %i KB", ( int ) hb_xquery( 1001 ), hb_getMemUsed() );
   #endif
   return( p );
}

HB_FUNC( QT_QHTTPRESPONSEHEADER )
{
   void * pObj = NULL;

   pObj = new QHttpResponseHeader() ;

   hb_retptrGC( gcAllocate_QHttpResponseHeader( pObj ) );
}
/*
 * virtual int majorVersion () const
 */
HB_FUNC( QT_QHTTPRESPONSEHEADER_MAJORVERSION )
{
   hb_retni( hbqt_par_QHttpResponseHeader( 1 )->majorVersion() );
}

/*
 * virtual int minorVersion () const
 */
HB_FUNC( QT_QHTTPRESPONSEHEADER_MINORVERSION )
{
   hb_retni( hbqt_par_QHttpResponseHeader( 1 )->minorVersion() );
}

/*
 * QString reasonPhrase () const
 */
HB_FUNC( QT_QHTTPRESPONSEHEADER_REASONPHRASE )
{
   hb_retc( hbqt_par_QHttpResponseHeader( 1 )->reasonPhrase().toAscii().data() );
}

/*
 * void setStatusLine ( int code, const QString & text = QString(), int majorVer = 1, int minorVer = 1 )
 */
HB_FUNC( QT_QHTTPRESPONSEHEADER_SETSTATUSLINE )
{
   hbqt_par_QHttpResponseHeader( 1 )->setStatusLine( hb_parni( 2 ), hbqt_par_QString( 3 ), ( HB_ISNUM( 4 ) ? hb_parni( 4 ) : 1 ), ( HB_ISNUM( 5 ) ? hb_parni( 5 ) : 1 ) );
}

/*
 * int statusCode () const
 */
HB_FUNC( QT_QHTTPRESPONSEHEADER_STATUSCODE )
{
   hb_retni( hbqt_par_QHttpResponseHeader( 1 )->statusCode() );
}


/*----------------------------------------------------------------------*/
#endif             /* #if QT_VERSION >= 0x040500 */
/*----------------------------------------------------------------------*/
