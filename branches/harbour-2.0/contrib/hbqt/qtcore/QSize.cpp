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

#include <QtCore/QSize>


/*
 * QSize ()
 * QSize ( int width, int height )
 * ~QSize ()
 */

QT_G_FUNC( hbqt_gcRelease_QSize )
{
   QGC_POINTER * p = ( QGC_POINTER * ) Cargo;

   HB_TRACE( HB_TR_DEBUG, ( "hbqt_gcRelease_QSize                        p=%p", p ) );
   HB_TRACE( HB_TR_DEBUG, ( "hbqt_gcRelease_QSize                       ph=%p", p->ph ) );

   if( p && p->ph )
   {
      delete ( ( QSize * ) p->ph );
      p->ph = NULL;
      HB_TRACE( HB_TR_DEBUG, ( "YES hbqt_gcRelease_QSize                       Object deleted! %i B %i KB", ( int ) hb_xquery( 1001 ), hbqt_getmemused() ) );
   }
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "DEL hbqt_gcRelease_QSize                       Object Already deleted!" ) );
   }
}

void * hbqt_gcAllocate_QSize( void * pObj )
{
   QGC_POINTER * p = ( QGC_POINTER * ) hb_gcAllocate( sizeof( QGC_POINTER ), hbqt_gcFuncs() );

   p->ph = pObj;
   p->func = hbqt_gcRelease_QSize;
   HB_TRACE( HB_TR_DEBUG, ( "          new_QSize                       %i B %i KB", ( int ) hb_xquery( 1001 ), hbqt_getmemused() ) );
   return( p );
}

HB_FUNC( QT_QSIZE )
{
   void * pObj = NULL;

   if( hb_pcount() == 2 && HB_ISNUM( 1 ) && HB_ISNUM( 2 ) )
   {
      pObj = ( QSize* ) new QSize( hb_parni( 1 ), hb_parni( 2 ) ) ;
   }
   else if( hb_pcount() == 1 && HB_ISPOINTER( 1 ) )
   {
      pObj = ( QSize* ) new QSize( *hbqt_par_QSize( 1 ) ) ;
   }
   else
   {
      pObj = ( QSize* ) new QSize() ;
   }

   hb_retptrGC( hbqt_gcAllocate_QSize( pObj ) );
}
/*
 * int height () const
 */
HB_FUNC( QT_QSIZE_HEIGHT )
{
   hb_retni( hbqt_par_QSize( 1 )->height() );
}

/*
 * bool isEmpty () const
 */
HB_FUNC( QT_QSIZE_ISEMPTY )
{
   hb_retl( hbqt_par_QSize( 1 )->isEmpty() );
}

/*
 * bool isNull () const
 */
HB_FUNC( QT_QSIZE_ISNULL )
{
   hb_retl( hbqt_par_QSize( 1 )->isNull() );
}

/*
 * bool isValid () const
 */
HB_FUNC( QT_QSIZE_ISVALID )
{
   hb_retl( hbqt_par_QSize( 1 )->isValid() );
}

/*
 * int & rheight ()
 */
HB_FUNC( QT_QSIZE_RHEIGHT )
{
   hb_retni( hbqt_par_QSize( 1 )->rheight() );
}

/*
 * int & rwidth ()
 */
HB_FUNC( QT_QSIZE_RWIDTH )
{
   hb_retni( hbqt_par_QSize( 1 )->rwidth() );
}

/*
 * void scale ( int width, int height, Qt::AspectRatioMode mode )
 */
HB_FUNC( QT_QSIZE_SCALE )
{
   hbqt_par_QSize( 1 )->scale( hb_parni( 2 ), hb_parni( 3 ), ( Qt::AspectRatioMode ) hb_parni( 4 ) );
}

/*
 * void scale ( const QSize & size, Qt::AspectRatioMode mode )
 */
HB_FUNC( QT_QSIZE_SCALE_1 )
{
   hbqt_par_QSize( 1 )->scale( *hbqt_par_QSize( 2 ), ( Qt::AspectRatioMode ) hb_parni( 3 ) );
}

/*
 * void setHeight ( int height )
 */
HB_FUNC( QT_QSIZE_SETHEIGHT )
{
   hbqt_par_QSize( 1 )->setHeight( hb_parni( 2 ) );
}

/*
 * void setWidth ( int width )
 */
HB_FUNC( QT_QSIZE_SETWIDTH )
{
   hbqt_par_QSize( 1 )->setWidth( hb_parni( 2 ) );
}

/*
 * void transpose ()
 */
HB_FUNC( QT_QSIZE_TRANSPOSE )
{
   hbqt_par_QSize( 1 )->transpose();
}

/*
 * int width () const
 */
HB_FUNC( QT_QSIZE_WIDTH )
{
   hb_retni( hbqt_par_QSize( 1 )->width() );
}

/*
 * QSize boundedTo ( const QSize & otherSize ) const
 */
HB_FUNC( QT_QSIZE_BOUNDEDTO )
{
   hb_retptrGC( hbqt_gcAllocate_QSize( new QSize( hbqt_par_QSize( 1 )->boundedTo( *hbqt_par_QSize( 2 ) ) ) ) );
}

/*
 * QSize expandedTo ( const QSize & otherSize ) const
 */
HB_FUNC( QT_QSIZE_EXPANDEDTO )
{
   hb_retptrGC( hbqt_gcAllocate_QSize( new QSize( hbqt_par_QSize( 1 )->expandedTo( *hbqt_par_QSize( 2 ) ) ) ) );
}


/*----------------------------------------------------------------------*/
#endif             /* #if QT_VERSION >= 0x040500 */
/*----------------------------------------------------------------------*/
