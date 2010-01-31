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

#include "hbapi.h"
#include "../hbqt.h"

/*----------------------------------------------------------------------*/
#if QT_VERSION >= 0x040500
/*----------------------------------------------------------------------*/

/*
 *  enum HBitmapFormat { NoAlpha, PremultipliedAlpha, Alpha }
 *  enum ShareMode { ImplicitlyShared, ExplicitlyShared }
 */

#include <QtCore/QPointer>

#include <QtGui/QPixmap>
#include <QtGui/QBitmap>

/*
 * QPixmap ()
 * QPixmap ( int width, int height )
 * QPixmap ( const QString & fileName, const char * format = 0, Qt::ImageConversionFlags flags = Qt::AutoColor )
 * QPixmap ( const char * const[] xpm )
 * QPixmap ( const QPixmap & pixmap )
 * QPixmap ( const QSize & size )
 * ~QPixmap ()
 */

typedef struct
{
   void * ph;
   bool bNew;
   QT_G_FUNC_PTR func;
} QGC_POINTER_QPixmap;

QT_G_FUNC( hbqt_gcRelease_QPixmap )
{
   QGC_POINTER * p = ( QGC_POINTER * ) Cargo;

   if( p && p->bNew )
   {
      if( p->ph )
      {
         HB_TRACE( HB_TR_DEBUG, ( "YES_rel_QPixmap   /.\\    ph=%p", p->ph ) );
         delete ( ( QPixmap * ) p->ph );
         HB_TRACE( HB_TR_DEBUG, ( "YES_rel_QPixmap   \\./    ph=%p", p->ph ) );
         p->ph = NULL;
      }
      else
      {
         HB_TRACE( HB_TR_DEBUG, ( "DEL_rel_QPixmap    :     Object already deleted!" ) );
      }
   }
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "PTR_rel_QPixmap    :    Object not created with new()" ) );
      p->ph = NULL;
   }
}

void * hbqt_gcAllocate_QPixmap( void * pObj, bool bNew )
{
   QGC_POINTER * p = ( QGC_POINTER * ) hb_gcAllocate( sizeof( QGC_POINTER ), hbqt_gcFuncs() );

   p->ph = pObj;
   p->bNew = bNew;
   p->func = hbqt_gcRelease_QPixmap;

   if( bNew )
   {
      HB_TRACE( HB_TR_DEBUG, ( "   _new_QPixmap                    ph=%p %i B %i KB", pObj, ( int ) hb_xquery( 1001 ), hbqt_getmemused() ) );
   }
   return p;
}

HB_FUNC( QT_QPIXMAP )
{
   void * pObj = NULL;

   if( hb_pcount() == 1 && HB_ISCHAR( 1 ) )
   {
      pObj = new QPixmap( hbqt_par_QString( 1 ), ( const char * ) 0, Qt::AutoColor ) ;
   }
   else if( hb_pcount() == 1 && HB_ISPOINTER( 1 ) )
   {
      pObj = new QPixmap( *hbqt_par_QPixmap( 1 ) ) ;
   }
   else if( hb_pcount() == 2 && HB_ISNUM( 1 ) && HB_ISNUM( 2 ) )
   {
      pObj = new QPixmap( hb_parni( 1 ), hb_parni( 2 ) ) ;
   }
   else
   {
      pObj = new QPixmap() ;
   }

   hb_retptrGC( hbqt_gcAllocate_QPixmap( pObj, true ) );
}

/*
 * QPixmap alphaChannel () const
 */
HB_FUNC( QT_QPIXMAP_ALPHACHANNEL )
{
   hb_retptrGC( hbqt_gcAllocate_QPixmap( new QPixmap( hbqt_par_QPixmap( 1 )->alphaChannel() ), true ) );
}

/*
 * qint64 cacheKey () const
 */
HB_FUNC( QT_QPIXMAP_CACHEKEY )
{
   hb_retnint( hbqt_par_QPixmap( 1 )->cacheKey() );
}

/*
 * QPixmap copy ( const QRect & rectangle = QRect() ) const
 */
HB_FUNC( QT_QPIXMAP_COPY )
{
   hb_retptrGC( hbqt_gcAllocate_QPixmap( new QPixmap( hbqt_par_QPixmap( 1 )->copy( ( HB_ISPOINTER( 2 ) ? *hbqt_par_QRect( 2 ) : QRect() ) ) ), true ) );
}

/*
 * QPixmap copy ( int x, int y, int width, int height ) const
 */
HB_FUNC( QT_QPIXMAP_COPY_1 )
{
   hb_retptrGC( hbqt_gcAllocate_QPixmap( new QPixmap( hbqt_par_QPixmap( 1 )->copy( hb_parni( 2 ), hb_parni( 3 ), hb_parni( 4 ), hb_parni( 5 ) ) ), true ) );
}

/*
 * QBitmap createHeuristicMask ( bool clipTight = true ) const
 */
HB_FUNC( QT_QPIXMAP_CREATEHEURISTICMASK )
{
   hb_retptrGC( hbqt_gcAllocate_QBitmap( new QBitmap( hbqt_par_QPixmap( 1 )->createHeuristicMask( hb_parl( 2 ) ) ), true ) );
}

/*
 * QBitmap createMaskFromColor ( const QColor & maskColor, Qt::MaskMode mode ) const
 */
HB_FUNC( QT_QPIXMAP_CREATEMASKFROMCOLOR )
{
   hb_retptrGC( hbqt_gcAllocate_QBitmap( new QBitmap( hbqt_par_QPixmap( 1 )->createMaskFromColor( *hbqt_par_QColor( 2 ), ( Qt::MaskMode ) hb_parni( 3 ) ) ), true ) );
}

/*
 * QBitmap createMaskFromColor ( const QColor & maskColor ) const
 */
HB_FUNC( QT_QPIXMAP_CREATEMASKFROMCOLOR_1 )
{
   hb_retptrGC( hbqt_gcAllocate_QBitmap( new QBitmap( hbqt_par_QPixmap( 1 )->createMaskFromColor( *hbqt_par_QColor( 2 ) ) ), true ) );
}

/*
 * int depth () const
 */
HB_FUNC( QT_QPIXMAP_DEPTH )
{
   hb_retni( hbqt_par_QPixmap( 1 )->depth() );
}

/*
 * void detach ()
 */
HB_FUNC( QT_QPIXMAP_DETACH )
{
   hbqt_par_QPixmap( 1 )->detach();
}

/*
 * void fill ( const QColor & color = Qt::white )
 */
HB_FUNC( QT_QPIXMAP_FILL )
{
   hbqt_par_QPixmap( 1 )->fill( *hbqt_par_QColor( 2 ) );
}

/*
 * void fill ( const QWidget * widget, const QPoint & offset )
 */
HB_FUNC( QT_QPIXMAP_FILL_1 )
{
   hbqt_par_QPixmap( 1 )->fill( hbqt_par_QWidget( 2 ), *hbqt_par_QPoint( 3 ) );
}

/*
 * void fill ( const QWidget * widget, int x, int y )
 */
HB_FUNC( QT_QPIXMAP_FILL_2 )
{
   hbqt_par_QPixmap( 1 )->fill( hbqt_par_QWidget( 2 ), hb_parni( 3 ), hb_parni( 4 ) );
}

/*
 * bool hasAlpha () const
 */
HB_FUNC( QT_QPIXMAP_HASALPHA )
{
   hb_retl( hbqt_par_QPixmap( 1 )->hasAlpha() );
}

/*
 * bool hasAlphaChannel () const
 */
HB_FUNC( QT_QPIXMAP_HASALPHACHANNEL )
{
   hb_retl( hbqt_par_QPixmap( 1 )->hasAlphaChannel() );
}

/*
 * int height () const
 */
HB_FUNC( QT_QPIXMAP_HEIGHT )
{
   hb_retni( hbqt_par_QPixmap( 1 )->height() );
}

/*
 * bool isNull () const
 */
HB_FUNC( QT_QPIXMAP_ISNULL )
{
   hb_retl( hbqt_par_QPixmap( 1 )->isNull() );
}

/*
 * bool isQBitmap () const
 */
HB_FUNC( QT_QPIXMAP_ISQBITMAP )
{
   hb_retl( hbqt_par_QPixmap( 1 )->isQBitmap() );
}

/*
 * bool load ( const QString & fileName, const char * format = 0, Qt::ImageConversionFlags flags = Qt::AutoColor )
 */
HB_FUNC( QT_QPIXMAP_LOAD )
{
   hb_retl( hbqt_par_QPixmap( 1 )->load( hbqt_par_QString( 2 ), hbqt_par_char( 3 ), ( HB_ISNUM( 4 ) ? ( Qt::ImageConversionFlags ) hb_parni( 4 ) : ( Qt::ImageConversionFlags ) Qt::AutoColor ) ) );
}

/*
 * bool loadFromData ( const QByteArray & data, const char * format = 0, Qt::ImageConversionFlags flags = Qt::AutoColor )
 */
HB_FUNC( QT_QPIXMAP_LOADFROMDATA )
{
   hb_retl( hbqt_par_QPixmap( 1 )->loadFromData( *hbqt_par_QByteArray( 2 ), hbqt_par_char( 3 ), ( HB_ISNUM( 4 ) ? ( Qt::ImageConversionFlags ) hb_parni( 4 ) : ( Qt::ImageConversionFlags ) Qt::AutoColor ) ) );
}

/*
 * QBitmap mask () const
 */
HB_FUNC( QT_QPIXMAP_MASK )
{
   hb_retptrGC( hbqt_gcAllocate_QBitmap( new QBitmap( hbqt_par_QPixmap( 1 )->mask() ), true ) );
}

/*
 * QRect rect () const
 */
HB_FUNC( QT_QPIXMAP_RECT )
{
   hb_retptrGC( hbqt_gcAllocate_QRect( new QRect( hbqt_par_QPixmap( 1 )->rect() ), true ) );
}

/*
 * bool save ( const QString & fileName, const char * format = 0, int quality = -1 ) const
 */
HB_FUNC( QT_QPIXMAP_SAVE )
{
   hb_retl( hbqt_par_QPixmap( 1 )->save( hbqt_par_QString( 2 ), hbqt_par_char( 3 ), ( HB_ISNUM( 4 ) ? hb_parni( 4 ) : -1 ) ) );
}

/*
 * bool save ( QIODevice * device, const char * format = 0, int quality = -1 ) const
 */
HB_FUNC( QT_QPIXMAP_SAVE_1 )
{
   hb_retl( hbqt_par_QPixmap( 1 )->save( hbqt_par_QIODevice( 2 ), hbqt_par_char( 3 ), ( HB_ISNUM( 4 ) ? hb_parni( 4 ) : -1 ) ) );
}

/*
 * QPixmap scaled ( int width, int height, Qt::AspectRatioMode aspectRatioMode = Qt::IgnoreAspectRatio, Qt::TransformationMode transformMode = Qt::FastTransformation ) const
 */
HB_FUNC( QT_QPIXMAP_SCALED )
{
   hb_retptrGC( hbqt_gcAllocate_QPixmap( new QPixmap( hbqt_par_QPixmap( 1 )->scaled( hb_parni( 2 ), hb_parni( 3 ), ( HB_ISNUM( 4 ) ? ( Qt::AspectRatioMode ) hb_parni( 4 ) : ( Qt::AspectRatioMode ) Qt::IgnoreAspectRatio ), ( HB_ISNUM( 5 ) ? ( Qt::TransformationMode ) hb_parni( 5 ) : ( Qt::TransformationMode ) Qt::FastTransformation ) ) ), true ) );
}

/*
 * QPixmap scaled ( const QSize & size, Qt::AspectRatioMode aspectRatioMode = Qt::IgnoreAspectRatio, Qt::TransformationMode transformMode = Qt::FastTransformation ) const
 */
HB_FUNC( QT_QPIXMAP_SCALED_1 )
{
   hb_retptrGC( hbqt_gcAllocate_QPixmap( new QPixmap( hbqt_par_QPixmap( 1 )->scaled( *hbqt_par_QSize( 2 ), ( HB_ISNUM( 3 ) ? ( Qt::AspectRatioMode ) hb_parni( 3 ) : ( Qt::AspectRatioMode ) Qt::IgnoreAspectRatio ), ( HB_ISNUM( 4 ) ? ( Qt::TransformationMode ) hb_parni( 4 ) : ( Qt::TransformationMode ) Qt::FastTransformation ) ) ), true ) );
}

/*
 * QPixmap scaledToHeight ( int height, Qt::TransformationMode mode = Qt::FastTransformation ) const
 */
HB_FUNC( QT_QPIXMAP_SCALEDTOHEIGHT )
{
   hb_retptrGC( hbqt_gcAllocate_QPixmap( new QPixmap( hbqt_par_QPixmap( 1 )->scaledToHeight( hb_parni( 2 ), ( HB_ISNUM( 3 ) ? ( Qt::TransformationMode ) hb_parni( 3 ) : ( Qt::TransformationMode ) Qt::FastTransformation ) ) ), true ) );
}

/*
 * QPixmap scaledToWidth ( int width, Qt::TransformationMode mode = Qt::FastTransformation ) const
 */
HB_FUNC( QT_QPIXMAP_SCALEDTOWIDTH )
{
   hb_retptrGC( hbqt_gcAllocate_QPixmap( new QPixmap( hbqt_par_QPixmap( 1 )->scaledToWidth( hb_parni( 2 ), ( HB_ISNUM( 3 ) ? ( Qt::TransformationMode ) hb_parni( 3 ) : ( Qt::TransformationMode ) Qt::FastTransformation ) ) ), true ) );
}

/*
 * void setAlphaChannel ( const QPixmap & alphaChannel )
 */
HB_FUNC( QT_QPIXMAP_SETALPHACHANNEL )
{
   hbqt_par_QPixmap( 1 )->setAlphaChannel( *hbqt_par_QPixmap( 2 ) );
}

/*
 * void setMask ( const QBitmap & mask )
 */
HB_FUNC( QT_QPIXMAP_SETMASK )
{
   hbqt_par_QPixmap( 1 )->setMask( *hbqt_par_QBitmap( 2 ) );
}

/*
 * QSize size () const
 */
HB_FUNC( QT_QPIXMAP_SIZE )
{
   hb_retptrGC( hbqt_gcAllocate_QSize( new QSize( hbqt_par_QPixmap( 1 )->size() ), true ) );
}

/*
 * QImage toImage () const
 */
HB_FUNC( QT_QPIXMAP_TOIMAGE )
{
   hb_retptrGC( hbqt_gcAllocate_QImage( new QImage( hbqt_par_QPixmap( 1 )->toImage() ), true ) );
}

/*
 * QPixmap transformed ( const QTransform & transform, Qt::TransformationMode mode = Qt::FastTransformation ) const
 */
HB_FUNC( QT_QPIXMAP_TRANSFORMED )
{
   hb_retptrGC( hbqt_gcAllocate_QPixmap( new QPixmap( hbqt_par_QPixmap( 1 )->transformed( *hbqt_par_QTransform( 2 ), ( HB_ISNUM( 3 ) ? ( Qt::TransformationMode ) hb_parni( 3 ) : ( Qt::TransformationMode ) Qt::FastTransformation ) ) ), true ) );
}

/*
 * QPixmap transformed ( const QMatrix & matrix, Qt::TransformationMode mode = Qt::FastTransformation ) const
 */
HB_FUNC( QT_QPIXMAP_TRANSFORMED_1 )
{
   hb_retptrGC( hbqt_gcAllocate_QPixmap( new QPixmap( hbqt_par_QPixmap( 1 )->transformed( *hbqt_par_QMatrix( 2 ), ( HB_ISNUM( 3 ) ? ( Qt::TransformationMode ) hb_parni( 3 ) : ( Qt::TransformationMode ) Qt::FastTransformation ) ) ), true ) );
}

/*
 * int width () const
 */
HB_FUNC( QT_QPIXMAP_WIDTH )
{
   hb_retni( hbqt_par_QPixmap( 1 )->width() );
}

/*
 * int defaultDepth ()
 */
HB_FUNC( QT_QPIXMAP_DEFAULTDEPTH )
{
   hb_retni( hbqt_par_QPixmap( 1 )->defaultDepth() );
}

/*
 * QPixmap fromImage ( const QImage & image, Qt::ImageConversionFlags flags = Qt::AutoColor )
 */
HB_FUNC( QT_QPIXMAP_FROMIMAGE )
{
   hb_retptrGC( hbqt_gcAllocate_QPixmap( new QPixmap( hbqt_par_QPixmap( 1 )->fromImage( *hbqt_par_QImage( 2 ), ( HB_ISNUM( 3 ) ? ( Qt::ImageConversionFlags ) hb_parni( 3 ) : ( Qt::ImageConversionFlags ) Qt::AutoColor ) ) ), true ) );
}

/*
 * QPixmap grabWidget ( QWidget * widget, const QRect & rectangle )
 */
HB_FUNC( QT_QPIXMAP_GRABWIDGET )
{
   hb_retptrGC( hbqt_gcAllocate_QPixmap( new QPixmap( hbqt_par_QPixmap( 1 )->grabWidget( hbqt_par_QWidget( 2 ), *hbqt_par_QRect( 3 ) ) ), true ) );
}

/*
 * QPixmap grabWidget ( QWidget * widget, int x = 0, int y = 0, int width = -1, int height = -1 )
 */
HB_FUNC( QT_QPIXMAP_GRABWIDGET_1 )
{
   hb_retptrGC( hbqt_gcAllocate_QPixmap( new QPixmap( hbqt_par_QPixmap( 1 )->grabWidget( hbqt_par_QWidget( 2 ), hb_parni( 3 ), hb_parni( 4 ), ( HB_ISNUM( 5 ) ? hb_parni( 5 ) : -1 ), ( HB_ISNUM( 6 ) ? hb_parni( 6 ) : -1 ) ) ), true ) );
}

/*
 * QTransform trueMatrix ( const QTransform & matrix, int width, int height )
 */
HB_FUNC( QT_QPIXMAP_TRUEMATRIX )
{
   hb_retptrGC( hbqt_gcAllocate_QTransform( new QTransform( hbqt_par_QPixmap( 1 )->trueMatrix( *hbqt_par_QTransform( 2 ), hb_parni( 3 ), hb_parni( 4 ) ) ), true ) );
}

/*
 * QMatrix trueMatrix ( const QMatrix & m, int w, int h )
 */
HB_FUNC( QT_QPIXMAP_TRUEMATRIX_1 )
{
   hb_retptrGC( hbqt_gcAllocate_QMatrix( new QMatrix( hbqt_par_QPixmap( 1 )->trueMatrix( *hbqt_par_QMatrix( 2 ), hb_parni( 3 ), hb_parni( 4 ) ) ), true ) );
}


/*----------------------------------------------------------------------*/
#endif             /* #if QT_VERSION >= 0x040500 */
/*----------------------------------------------------------------------*/
