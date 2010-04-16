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
 * Copyright 2009-2010 Pritpal Bedi <pritpal@vouchcac.com>
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

#include "../hbqt.h"

/*----------------------------------------------------------------------*/
#if QT_VERSION >= 0x040500
/*----------------------------------------------------------------------*/

#include <QtCore/QPointer>

#include <QtGui/QBitmap>


/*
 * QBitmap ()
 * QBitmap ( const QPixmap & pixmap )
 * QBitmap ( int width, int height )
 * QBitmap ( const QSize & size )
 * QBitmap ( const QString & fileName, const char * format = 0 )
 * ~QBitmap ()
 */

typedef struct
{
   QBitmap * ph;
   bool bNew;
   QT_G_FUNC_PTR func;
} QGC_POINTER_QBitmap;

QT_G_FUNC( hbqt_gcRelease_QBitmap )
{
   QGC_POINTER * p = ( QGC_POINTER * ) Cargo;

   if( p && p->bNew )
   {
      if( p->ph )
      {
         HB_TRACE( HB_TR_DEBUG, ( "ph=%p    _rel_QBitmap   /.\\", p->ph ) );
         delete ( ( QBitmap * ) p->ph );
         HB_TRACE( HB_TR_DEBUG, ( "ph=%p YES_rel_QBitmap   \\./", p->ph ) );
         p->ph = NULL;
      }
      else
      {
         HB_TRACE( HB_TR_DEBUG, ( "ph=%p DEL_rel_QBitmap    :     Object already deleted!", p->ph ) );
         p->ph = NULL;
      }
   }
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "ph=%p PTR_rel_QBitmap    :    Object not created with new=true", p->ph ) );
      p->ph = NULL;
   }
}

void * hbqt_gcAllocate_QBitmap( void * pObj, bool bNew )
{
   QGC_POINTER * p = ( QGC_POINTER * ) hb_gcAllocate( sizeof( QGC_POINTER ), hbqt_gcFuncs() );

   p->ph = ( QBitmap * ) pObj;
   p->bNew = bNew;
   p->func = hbqt_gcRelease_QBitmap;

   if( bNew )
   {
      HB_TRACE( HB_TR_DEBUG, ( "ph=%p    _new_QBitmap", pObj ) );
   }
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "ph=%p NOT_new_QBitmap", pObj ) );
   }
   return p;
}

HB_FUNC( QT_QBITMAP )
{
   QBitmap * pObj = NULL;

   if( hb_pcount() == 1 && HB_ISPOINTER( 1 ) )
   {
      pObj =  new QBitmap( *hbqt_par_QBitmap( 1 ) ) ;
   }
   else if( hb_pcount() == 1 && HB_ISCHAR( 1 ) )
   {
      pObj =  new QBitmap( hbqt_par_QString( 1 ), ( const char * ) 0 ) ;
   }
   else if( hb_pcount() == 2 && HB_ISCHAR( 1 ) && HB_ISCHAR( 2 ) )
   {
      pObj =  new QBitmap( hbqt_par_QString( 1 ), hb_parc( 2 ) ) ;
   }
   else if( hb_pcount() == 2 && HB_ISNUM( 1 ) && HB_ISNUM( 2 ) )
   {
      pObj =  new QBitmap( hb_parni( 1 ), hb_parni( 2 ) ) ;
   }
   else if( hb_pcount() == 2 && HB_ISCHAR( 1 ) && HB_ISPOINTER( 2 ) )
   {
      if(      ( QString ) "QPixmap" == hbqt_par_QString( 1 ) )
      {
         pObj =  new QBitmap( *hbqt_par_QPixmap( 2 ) ) ;
      }
      else if( ( QString ) "QSize"   == hbqt_par_QString( 1 ) )
      {
         pObj =  new QBitmap( *hbqt_par_QSize( 2 ) ) ;
      }
      else
      {
         pObj =  new QBitmap() ;
      }
   }
   else
   {
      pObj =  new QBitmap() ;
   }

   hb_retptrGC( hbqt_gcAllocate_QBitmap( ( void * ) pObj, true ) );
}

/*
 * void clear ()
 */
HB_FUNC( QT_QBITMAP_CLEAR )
{
   QBitmap * p = hbqt_par_QBitmap( 1 );
   if( p )
      ( p )->clear();
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QBITMAP_CLEAR FP=( p )->clear(); p is NULL" ) );
   }
}

/*
 * QBitmap transformed ( const QTransform & matrix ) const
 */
HB_FUNC( QT_QBITMAP_TRANSFORMED )
{
   QBitmap * p = hbqt_par_QBitmap( 1 );
   if( p )
      hb_retptrGC( hbqt_gcAllocate_QBitmap( new QBitmap( ( p )->transformed( *hbqt_par_QTransform( 2 ) ) ), true ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QBITMAP_TRANSFORMED FP=hb_retptrGC( hbqt_gcAllocate_QBitmap( new QBitmap( ( p )->transformed( *hbqt_par_QTransform( 2 ) ) ), true ) ); p is NULL" ) );
   }
}

/*
 * QBitmap transformed ( const QMatrix & matrix ) const
 */
HB_FUNC( QT_QBITMAP_TRANSFORMED_1 )
{
   QBitmap * p = hbqt_par_QBitmap( 1 );
   if( p )
      hb_retptrGC( hbqt_gcAllocate_QBitmap( new QBitmap( ( p )->transformed( *hbqt_par_QMatrix( 2 ) ) ), true ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QBITMAP_TRANSFORMED_1 FP=hb_retptrGC( hbqt_gcAllocate_QBitmap( new QBitmap( ( p )->transformed( *hbqt_par_QMatrix( 2 ) ) ), true ) ); p is NULL" ) );
   }
}

/*
 * QBitmap fromImage ( const QImage & image, Qt::ImageConversionFlags flags = Qt::AutoColor )
 */
HB_FUNC( QT_QBITMAP_FROMIMAGE )
{
   QBitmap * p = hbqt_par_QBitmap( 1 );
   if( p )
      hb_retptrGC( hbqt_gcAllocate_QBitmap( new QBitmap( ( p )->fromImage( *hbqt_par_QImage( 2 ), ( HB_ISNUM( 3 ) ? ( Qt::ImageConversionFlags ) hb_parni( 3 ) : ( Qt::ImageConversionFlags ) Qt::AutoColor ) ) ), true ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QBITMAP_FROMIMAGE FP=hb_retptrGC( hbqt_gcAllocate_QBitmap( new QBitmap( ( p )->fromImage( *hbqt_par_QImage( 2 ), ( HB_ISNUM( 3 ) ? ( Qt::ImageConversionFlags ) hb_parni( 3 ) : ( Qt::ImageConversionFlags ) Qt::AutoColor ) ) ), true ) ); p is NULL" ) );
   }
}


/*----------------------------------------------------------------------*/
#endif             /* #if QT_VERSION >= 0x040500 */
/*----------------------------------------------------------------------*/
