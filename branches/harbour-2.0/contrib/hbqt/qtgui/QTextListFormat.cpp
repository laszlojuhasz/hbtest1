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

/*
 *  enum Style { ListDisc, ListCircle, ListSquare, ListDecimal, ListLowerAlpha, ListUpperAlpha }
 */

#include <QtCore/QPointer>

#include <QtGui/QTextListFormat>


/* QTextListFormat ()
 *
 */

QT_G_FUNC( hbqt_gcRelease_QTextListFormat )
{
   QGC_POINTER * p = ( QGC_POINTER * ) Cargo;

   HB_TRACE( HB_TR_DEBUG, ( "hbqt_gcRelease_QTextListFormat              p=%p", p ) );
   HB_TRACE( HB_TR_DEBUG, ( "hbqt_gcRelease_QTextListFormat             ph=%p", p->ph ) );

   if( p && p->ph )
   {
      delete ( ( QTextListFormat * ) p->ph );
      p->ph = NULL;
      HB_TRACE( HB_TR_DEBUG, ( "YES hbqt_gcRelease_QTextListFormat             Object deleted! %i B %i KB", ( int ) hb_xquery( 1001 ), hbqt_getmemused() ) );
   }
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "DEL hbqt_gcRelease_QTextListFormat             Object Already deleted!" ) );
   }
}

void * hbqt_gcAllocate_QTextListFormat( void * pObj )
{
   QGC_POINTER * p = ( QGC_POINTER * ) hb_gcAllocate( sizeof( QGC_POINTER ), hbqt_gcFuncs() );

   p->ph = pObj;
   p->func = hbqt_gcRelease_QTextListFormat;
   HB_TRACE( HB_TR_DEBUG, ( "          new_QTextListFormat             %i B %i KB", ( int ) hb_xquery( 1001 ), hbqt_getmemused() ) );
   return( p );
}

HB_FUNC( QT_QTEXTLISTFORMAT )
{
   void * pObj = NULL;

   pObj = new QTextListFormat() ;

   hb_retptrGC( hbqt_gcAllocate_QTextListFormat( pObj ) );
}
/*
 * int indent () const
 */
HB_FUNC( QT_QTEXTLISTFORMAT_INDENT )
{
   hb_retni( hbqt_par_QTextListFormat( 1 )->indent() );
}

/*
 * bool isValid () const
 */
HB_FUNC( QT_QTEXTLISTFORMAT_ISVALID )
{
   hb_retl( hbqt_par_QTextListFormat( 1 )->isValid() );
}

/*
 * void setIndent ( int indentation )
 */
HB_FUNC( QT_QTEXTLISTFORMAT_SETINDENT )
{
   hbqt_par_QTextListFormat( 1 )->setIndent( hb_parni( 2 ) );
}

/*
 * void setStyle ( Style style )
 */
HB_FUNC( QT_QTEXTLISTFORMAT_SETSTYLE )
{
   hbqt_par_QTextListFormat( 1 )->setStyle( ( QTextListFormat::Style ) hb_parni( 2 ) );
}

/*
 * Style style () const
 */
HB_FUNC( QT_QTEXTLISTFORMAT_STYLE )
{
   hb_retni( ( QTextListFormat::Style ) hbqt_par_QTextListFormat( 1 )->style() );
}


/*----------------------------------------------------------------------*/
#endif             /* #if QT_VERSION >= 0x040500 */
/*----------------------------------------------------------------------*/
