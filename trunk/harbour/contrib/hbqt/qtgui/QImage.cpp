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
 * www - http://harbour-project.org
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
#include "hbqtgui_garbage.h"
#include "hbqtcore_garbage.h"

/*----------------------------------------------------------------------*/
#if QT_VERSION >= 0x040500
/*----------------------------------------------------------------------*/

/*
 *  enum Format { Format_Invalid, Format_Mono, Format_MonoLSB, Format_Indexed8, ..., Format_ARGB4444_Premultiplied }
 *  enum InvertMode { InvertRgb, InvertRgba }
 */

/*
 *  Constructed[ 60/64 [ 93.75% ] ]
 *
 *  *** Unconvered Prototypes ***
 *  -----------------------------
 *
 *  QVector<QRgb> colorTable () const
 *  QImage convertToFormat ( Format format, const QVector<QRgb> & colorTable, Qt::ImageConversionFlags flags = Qt::AutoColor ) const
 *  void setColorTable ( const QVector<QRgb> colors )
 *
 *  *** Commented out protos which construct fine but do not compile ***
 *
 *  // bool loadFromData ( const uchar * data, int len, const char * format = 0 )
 */

#include <QtCore/QPointer>

#include <QtCore/QStringList>
#include <QtGui/QImage>


/*
 * QImage ()
 * QImage ( const QSize & size, Format format )
 * QImage ( int width, int height, Format format )
 * QImage ( uchar * data, int width, int height, Format format )
 * QImage ( const uchar * data, int width, int height, Format format )
 * QImage ( uchar * data, int width, int height, int bytesPerLine, Format format )
 * QImage ( const uchar * data, int width, int height, int bytesPerLine, Format format )
 * QImage ( const char * const[] xpm )
 * QImage ( const QString & fileName, const char * format = 0 )
 * QImage ( const char * fileName, const char * format = 0 )
 * QImage ( const QImage & image )
 * ~QImage ()
 */

typedef struct
{
   QImage * ph;
   bool bNew;
   QT_G_FUNC_PTR func;
   int type;
} QGC_POINTER_QImage;

QT_G_FUNC( hbqt_gcRelease_QImage )
{
   QGC_POINTER * p = ( QGC_POINTER * ) Cargo;

   if( p && p->bNew )
   {
      if( p->ph )
      {
         HB_TRACE( HB_TR_DEBUG, ( "ph=%p    _rel_QImage   /.\\", p->ph ) );
         delete ( ( QImage * ) p->ph );
         HB_TRACE( HB_TR_DEBUG, ( "ph=%p YES_rel_QImage   \\./", p->ph ) );
         p->ph = NULL;
      }
      else
      {
         HB_TRACE( HB_TR_DEBUG, ( "ph=%p DEL_rel_QImage    :     Object already deleted!", p->ph ) );
         p->ph = NULL;
      }
   }
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "ph=%p PTR_rel_QImage    :    Object not created with new=true", p->ph ) );
      p->ph = NULL;
   }
}

void * hbqt_gcAllocate_QImage( void * pObj, bool bNew )
{
   QGC_POINTER * p = ( QGC_POINTER * ) hb_gcAllocate( sizeof( QGC_POINTER ), hbqt_gcFuncs() );

   p->ph = ( QImage * ) pObj;
   p->bNew = bNew;
   p->func = hbqt_gcRelease_QImage;
   p->type = HBQT_TYPE_QImage;

   if( bNew )
   {
      HB_TRACE( HB_TR_DEBUG, ( "ph=%p    _new_QImage", pObj ) );
   }
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "ph=%p NOT_new_QImage", pObj ) );
   }
   return p;
}

HB_FUNC( QT_QIMAGE )
{
   QImage * pObj = NULL;

   if( hb_pcount() == 1 && HB_ISPOINTER( 1 ) )
   {
      pObj =  new QImage( *hbqt_par_QImage( 1 ) ) ;
   }
   else if( hb_pcount() == 1 && HB_ISCHAR( 1 ) )
   {
      pObj =  new QImage( hbqt_par_QString( 1 ), ( const char * ) 0 ) ;
   }
   else if( hb_pcount() == 2 && HB_ISCHAR( 1 ) && HB_ISCHAR( 2 ) )
   {
      pObj =  new QImage( hbqt_par_QString( 1 ), ( const char * ) hb_parcx( 2 ) ) ;
   }
   else if( hb_pcount() == 2 && HB_ISPOINTER( 1 ) && HB_ISNUM( 2 ) )
   {
      pObj =  new QImage( *hbqt_par_QSize( 1 ), ( QImage::Format ) hb_parni( 2 ) ) ;
   }
   else if( hb_pcount() == 3 && HB_ISNUM( 1 ) && HB_ISNUM( 2 ) && HB_ISNUM( 3 ) )
   {
      pObj =  new QImage( hb_parni( 1 ), hb_parni( 2 ), ( QImage::Format ) hb_parni( 3 ) ) ;
   }
   else if( hb_pcount() == 4 && HB_ISCHAR( 1 ) && HB_ISNUM( 2 ) && HB_ISNUM( 3 ) && HB_ISNUM( 4 ) )
   {
      pObj =  new QImage( ( const uchar * ) hb_parc( 1 ), hb_parni( 2 ), hb_parni( 3 ), ( QImage::Format ) hb_parni( 4 ) ) ;
   }
   else if( hb_pcount() == 5 && HB_ISCHAR( 1 ) && HB_ISNUM( 2 ) && HB_ISNUM( 3 ) && HB_ISNUM( 4 ) && HB_ISNUM( 5 ) )
   {
      pObj =  new QImage( ( const uchar * ) hb_parc( 1 ), hb_parni( 2 ), hb_parni( 3 ), hb_parni( 4 ), ( QImage::Format ) hb_parni( 5 ) ) ;
   }
   else
   {
      pObj =  new QImage() ;
   }

   hb_retptrGC( hbqt_gcAllocate_QImage( ( void * ) pObj, true ) );
}

/*
 * bool allGray () const
 */
HB_FUNC( QT_QIMAGE_ALLGRAY )
{
   QImage * p = hbqt_par_QImage( 1 );
   if( p )
      hb_retl( ( p )->allGray() );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QIMAGE_ALLGRAY FP=hb_retl( ( p )->allGray() ); p is NULL" ) );
   }
}

/*
 * QImage alphaChannel () const
 */
HB_FUNC( QT_QIMAGE_ALPHACHANNEL )
{
   QImage * p = hbqt_par_QImage( 1 );
   if( p )
      hb_retptrGC( hbqt_gcAllocate_QImage( new QImage( ( p )->alphaChannel() ), true ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QIMAGE_ALPHACHANNEL FP=hb_retptrGC( hbqt_gcAllocate_QImage( new QImage( ( p )->alphaChannel() ), true ) ); p is NULL" ) );
   }
}

/*
 * uchar * bits ()
 */
HB_FUNC( QT_QIMAGE_BITS )
{
   QImage * p = hbqt_par_QImage( 1 );
   if( p )
      hb_retc( ( const char * ) ( p )->bits() );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QIMAGE_BITS FP=hb_retc( ( const char * ) ( p )->bits() ); p is NULL" ) );
   }
}

/*
 * const uchar * bits () const
 */
HB_FUNC( QT_QIMAGE_BITS_1 )
{
   QImage * p = hbqt_par_QImage( 1 );
   if( p )
      hb_retc( ( const char * ) ( p )->bits() );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QIMAGE_BITS_1 FP=hb_retc( ( const char * ) ( p )->bits() ); p is NULL" ) );
   }
}

/*
 * int bytesPerLine () const
 */
HB_FUNC( QT_QIMAGE_BYTESPERLINE )
{
   QImage * p = hbqt_par_QImage( 1 );
   if( p )
      hb_retni( ( p )->bytesPerLine() );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QIMAGE_BYTESPERLINE FP=hb_retni( ( p )->bytesPerLine() ); p is NULL" ) );
   }
}

/*
 * qint64 cacheKey () const
 */
HB_FUNC( QT_QIMAGE_CACHEKEY )
{
   QImage * p = hbqt_par_QImage( 1 );
   if( p )
      hb_retnint( ( p )->cacheKey() );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QIMAGE_CACHEKEY FP=hb_retnint( ( p )->cacheKey() ); p is NULL" ) );
   }
}

/*
 * QRgb color ( int i ) const
 */
HB_FUNC( QT_QIMAGE_COLOR )
{
   QImage * p = hbqt_par_QImage( 1 );
   if( p )
      hb_retnl( ( p )->color( hb_parni( 2 ) ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QIMAGE_COLOR FP=hb_retnl( ( p )->color( hb_parni( 2 ) ) ); p is NULL" ) );
   }
}

/*
 * QImage convertToFormat ( Format format, Qt::ImageConversionFlags flags = Qt::AutoColor ) const
 */
HB_FUNC( QT_QIMAGE_CONVERTTOFORMAT )
{
   QImage * p = hbqt_par_QImage( 1 );
   if( p )
      hb_retptrGC( hbqt_gcAllocate_QImage( new QImage( ( p )->convertToFormat( ( QImage::Format ) hb_parni( 2 ), ( HB_ISNUM( 3 ) ? ( Qt::ImageConversionFlags ) hb_parni( 3 ) : ( Qt::ImageConversionFlags ) Qt::AutoColor ) ) ), true ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QIMAGE_CONVERTTOFORMAT FP=hb_retptrGC( hbqt_gcAllocate_QImage( new QImage( ( p )->convertToFormat( ( QImage::Format ) hb_parni( 2 ), ( HB_ISNUM( 3 ) ? ( Qt::ImageConversionFlags ) hb_parni( 3 ) : ( Qt::ImageConversionFlags ) Qt::AutoColor ) ) ), true ) ); p is NULL" ) );
   }
}

/*
 * QImage copy ( const QRect & rectangle = QRect() ) const
 */
HB_FUNC( QT_QIMAGE_COPY )
{
   QImage * p = hbqt_par_QImage( 1 );
   if( p )
      hb_retptrGC( hbqt_gcAllocate_QImage( new QImage( ( p )->copy( ( HB_ISPOINTER( 2 ) ? *hbqt_par_QRect( 2 ) : QRect() ) ) ), true ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QIMAGE_COPY FP=hb_retptrGC( hbqt_gcAllocate_QImage( new QImage( ( p )->copy( ( HB_ISPOINTER( 2 ) ? *hbqt_par_QRect( 2 ) : QRect() ) ) ), true ) ); p is NULL" ) );
   }
}

/*
 * QImage copy ( int x, int y, int width, int height ) const
 */
HB_FUNC( QT_QIMAGE_COPY_1 )
{
   QImage * p = hbqt_par_QImage( 1 );
   if( p )
      hb_retptrGC( hbqt_gcAllocate_QImage( new QImage( ( p )->copy( hb_parni( 2 ), hb_parni( 3 ), hb_parni( 4 ), hb_parni( 5 ) ) ), true ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QIMAGE_COPY_1 FP=hb_retptrGC( hbqt_gcAllocate_QImage( new QImage( ( p )->copy( hb_parni( 2 ), hb_parni( 3 ), hb_parni( 4 ), hb_parni( 5 ) ) ), true ) ); p is NULL" ) );
   }
}

/*
 * QImage createAlphaMask ( Qt::ImageConversionFlags flags = Qt::AutoColor ) const
 */
HB_FUNC( QT_QIMAGE_CREATEALPHAMASK )
{
   QImage * p = hbqt_par_QImage( 1 );
   if( p )
      hb_retptrGC( hbqt_gcAllocate_QImage( new QImage( ( p )->createAlphaMask( ( HB_ISNUM( 2 ) ? ( Qt::ImageConversionFlags ) hb_parni( 2 ) : ( Qt::ImageConversionFlags ) Qt::AutoColor ) ) ), true ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QIMAGE_CREATEALPHAMASK FP=hb_retptrGC( hbqt_gcAllocate_QImage( new QImage( ( p )->createAlphaMask( ( HB_ISNUM( 2 ) ? ( Qt::ImageConversionFlags ) hb_parni( 2 ) : ( Qt::ImageConversionFlags ) Qt::AutoColor ) ) ), true ) ); p is NULL" ) );
   }
}

/*
 * QImage createHeuristicMask ( bool clipTight = true ) const
 */
HB_FUNC( QT_QIMAGE_CREATEHEURISTICMASK )
{
   QImage * p = hbqt_par_QImage( 1 );
   if( p )
      hb_retptrGC( hbqt_gcAllocate_QImage( new QImage( ( p )->createHeuristicMask( hb_parl( 2 ) ) ), true ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QIMAGE_CREATEHEURISTICMASK FP=hb_retptrGC( hbqt_gcAllocate_QImage( new QImage( ( p )->createHeuristicMask( hb_parl( 2 ) ) ), true ) ); p is NULL" ) );
   }
}

/*
 * QImage createMaskFromColor ( QRgb color, Qt::MaskMode mode = Qt::MaskInColor ) const
 */
HB_FUNC( QT_QIMAGE_CREATEMASKFROMCOLOR )
{
   QImage * p = hbqt_par_QImage( 1 );
   if( p )
      hb_retptrGC( hbqt_gcAllocate_QImage( new QImage( ( p )->createMaskFromColor( hb_parnl( 2 ), ( HB_ISNUM( 3 ) ? ( Qt::MaskMode ) hb_parni( 3 ) : ( Qt::MaskMode ) Qt::MaskInColor ) ) ), true ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QIMAGE_CREATEMASKFROMCOLOR FP=hb_retptrGC( hbqt_gcAllocate_QImage( new QImage( ( p )->createMaskFromColor( hb_parnl( 2 ), ( HB_ISNUM( 3 ) ? ( Qt::MaskMode ) hb_parni( 3 ) : ( Qt::MaskMode ) Qt::MaskInColor ) ) ), true ) ); p is NULL" ) );
   }
}

/*
 * int depth () const
 */
HB_FUNC( QT_QIMAGE_DEPTH )
{
   QImage * p = hbqt_par_QImage( 1 );
   if( p )
      hb_retni( ( p )->depth() );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QIMAGE_DEPTH FP=hb_retni( ( p )->depth() ); p is NULL" ) );
   }
}

/*
 * int dotsPerMeterX () const
 */
HB_FUNC( QT_QIMAGE_DOTSPERMETERX )
{
   QImage * p = hbqt_par_QImage( 1 );
   if( p )
      hb_retni( ( p )->dotsPerMeterX() );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QIMAGE_DOTSPERMETERX FP=hb_retni( ( p )->dotsPerMeterX() ); p is NULL" ) );
   }
}

/*
 * int dotsPerMeterY () const
 */
HB_FUNC( QT_QIMAGE_DOTSPERMETERY )
{
   QImage * p = hbqt_par_QImage( 1 );
   if( p )
      hb_retni( ( p )->dotsPerMeterY() );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QIMAGE_DOTSPERMETERY FP=hb_retni( ( p )->dotsPerMeterY() ); p is NULL" ) );
   }
}

/*
 * void fill ( uint pixelValue )
 */
HB_FUNC( QT_QIMAGE_FILL )
{
   QImage * p = hbqt_par_QImage( 1 );
   if( p )
      ( p )->fill( hb_parni( 2 ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QIMAGE_FILL FP=( p )->fill( hb_parni( 2 ) ); p is NULL" ) );
   }
}

/*
 * Format format () const
 */
HB_FUNC( QT_QIMAGE_FORMAT )
{
   QImage * p = hbqt_par_QImage( 1 );
   if( p )
      hb_retni( ( QImage::Format ) ( p )->format() );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QIMAGE_FORMAT FP=hb_retni( ( QImage::Format ) ( p )->format() ); p is NULL" ) );
   }
}

/*
 * bool hasAlphaChannel () const
 */
HB_FUNC( QT_QIMAGE_HASALPHACHANNEL )
{
   QImage * p = hbqt_par_QImage( 1 );
   if( p )
      hb_retl( ( p )->hasAlphaChannel() );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QIMAGE_HASALPHACHANNEL FP=hb_retl( ( p )->hasAlphaChannel() ); p is NULL" ) );
   }
}

/*
 * int height () const
 */
HB_FUNC( QT_QIMAGE_HEIGHT )
{
   QImage * p = hbqt_par_QImage( 1 );
   if( p )
      hb_retni( ( p )->height() );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QIMAGE_HEIGHT FP=hb_retni( ( p )->height() ); p is NULL" ) );
   }
}

/*
 * void invertPixels ( InvertMode mode = InvertRgb )
 */
HB_FUNC( QT_QIMAGE_INVERTPIXELS )
{
   QImage * p = hbqt_par_QImage( 1 );
   if( p )
      ( p )->invertPixels( ( HB_ISNUM( 2 ) ? ( QImage::InvertMode ) hb_parni( 2 ) : ( QImage::InvertMode ) QImage::InvertRgb ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QIMAGE_INVERTPIXELS FP=( p )->invertPixels( ( HB_ISNUM( 2 ) ? ( QImage::InvertMode ) hb_parni( 2 ) : ( QImage::InvertMode ) QImage::InvertRgb ) ); p is NULL" ) );
   }
}

/*
 * bool isGrayscale () const
 */
HB_FUNC( QT_QIMAGE_ISGRAYSCALE )
{
   QImage * p = hbqt_par_QImage( 1 );
   if( p )
      hb_retl( ( p )->isGrayscale() );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QIMAGE_ISGRAYSCALE FP=hb_retl( ( p )->isGrayscale() ); p is NULL" ) );
   }
}

/*
 * bool isNull () const
 */
HB_FUNC( QT_QIMAGE_ISNULL )
{
   QImage * p = hbqt_par_QImage( 1 );
   if( p )
      hb_retl( ( p )->isNull() );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QIMAGE_ISNULL FP=hb_retl( ( p )->isNull() ); p is NULL" ) );
   }
}

/*
 * bool load ( const QString & fileName, const char * format = 0 )
 */
HB_FUNC( QT_QIMAGE_LOAD )
{
   QImage * p = hbqt_par_QImage( 1 );
   if( p )
      hb_retl( ( p )->load( hbqt_par_QString( 2 ), hbqt_par_char( 3 ) ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QIMAGE_LOAD FP=hb_retl( ( p )->load( hbqt_par_QString( 2 ), hbqt_par_char( 3 ) ) ); p is NULL" ) );
   }
}

/*
 * bool load ( QIODevice * device, const char * format )
 */
HB_FUNC( QT_QIMAGE_LOAD_1 )
{
   QImage * p = hbqt_par_QImage( 1 );
   if( p )
      hb_retl( ( p )->load( hbqt_par_QIODevice( 2 ), hbqt_par_char( 3 ) ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QIMAGE_LOAD_1 FP=hb_retl( ( p )->load( hbqt_par_QIODevice( 2 ), hbqt_par_char( 3 ) ) ); p is NULL" ) );
   }
}

/*
 * bool loadFromData ( const QByteArray & data, const char * format = 0 )
 */
HB_FUNC( QT_QIMAGE_LOADFROMDATA )
{
   QImage * p = hbqt_par_QImage( 1 );
   if( p )
      hb_retl( ( p )->loadFromData( *hbqt_par_QByteArray( 2 ), hbqt_par_char( 3 ) ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QIMAGE_LOADFROMDATA FP=hb_retl( ( p )->loadFromData( *hbqt_par_QByteArray( 2 ), hbqt_par_char( 3 ) ) ); p is NULL" ) );
   }
}

/*
 * QImage mirrored ( bool horizontal = false, bool vertical = true ) const
 */
HB_FUNC( QT_QIMAGE_MIRRORED )
{
   QImage * p = hbqt_par_QImage( 1 );
   if( p )
      hb_retptrGC( hbqt_gcAllocate_QImage( new QImage( ( p )->mirrored( hb_parl( 2 ), hb_parl( 3 ) ) ), true ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QIMAGE_MIRRORED FP=hb_retptrGC( hbqt_gcAllocate_QImage( new QImage( ( p )->mirrored( hb_parl( 2 ), hb_parl( 3 ) ) ), true ) ); p is NULL" ) );
   }
}

/*
 * int numBytes () const
 */
HB_FUNC( QT_QIMAGE_NUMBYTES )
{
   QImage * p = hbqt_par_QImage( 1 );
   if( p )
      hb_retni( ( p )->numBytes() );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QIMAGE_NUMBYTES FP=hb_retni( ( p )->numBytes() ); p is NULL" ) );
   }
}

/*
 * int numColors () const
 */
HB_FUNC( QT_QIMAGE_NUMCOLORS )
{
   QImage * p = hbqt_par_QImage( 1 );
   if( p )
      hb_retni( ( p )->numColors() );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QIMAGE_NUMCOLORS FP=hb_retni( ( p )->numColors() ); p is NULL" ) );
   }
}

/*
 * QPoint offset () const
 */
HB_FUNC( QT_QIMAGE_OFFSET )
{
   QImage * p = hbqt_par_QImage( 1 );
   if( p )
      hb_retptrGC( hbqt_gcAllocate_QPoint( new QPoint( ( p )->offset() ), true ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QIMAGE_OFFSET FP=hb_retptrGC( hbqt_gcAllocate_QPoint( new QPoint( ( p )->offset() ), true ) ); p is NULL" ) );
   }
}

/*
 * QRgb pixel ( const QPoint & position ) const
 */
HB_FUNC( QT_QIMAGE_PIXEL )
{
   QImage * p = hbqt_par_QImage( 1 );
   if( p )
      hb_retnl( ( p )->pixel( *hbqt_par_QPoint( 2 ) ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QIMAGE_PIXEL FP=hb_retnl( ( p )->pixel( *hbqt_par_QPoint( 2 ) ) ); p is NULL" ) );
   }
}

/*
 * QRgb pixel ( int x, int y ) const
 */
HB_FUNC( QT_QIMAGE_PIXEL_1 )
{
   QImage * p = hbqt_par_QImage( 1 );
   if( p )
      hb_retnl( ( p )->pixel( hb_parni( 2 ), hb_parni( 3 ) ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QIMAGE_PIXEL_1 FP=hb_retnl( ( p )->pixel( hb_parni( 2 ), hb_parni( 3 ) ) ); p is NULL" ) );
   }
}

/*
 * int pixelIndex ( const QPoint & position ) const
 */
HB_FUNC( QT_QIMAGE_PIXELINDEX )
{
   QImage * p = hbqt_par_QImage( 1 );
   if( p )
      hb_retni( ( p )->pixelIndex( *hbqt_par_QPoint( 2 ) ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QIMAGE_PIXELINDEX FP=hb_retni( ( p )->pixelIndex( *hbqt_par_QPoint( 2 ) ) ); p is NULL" ) );
   }
}

/*
 * int pixelIndex ( int x, int y ) const
 */
HB_FUNC( QT_QIMAGE_PIXELINDEX_1 )
{
   QImage * p = hbqt_par_QImage( 1 );
   if( p )
      hb_retni( ( p )->pixelIndex( hb_parni( 2 ), hb_parni( 3 ) ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QIMAGE_PIXELINDEX_1 FP=hb_retni( ( p )->pixelIndex( hb_parni( 2 ), hb_parni( 3 ) ) ); p is NULL" ) );
   }
}

/*
 * QRect rect () const
 */
HB_FUNC( QT_QIMAGE_RECT )
{
   QImage * p = hbqt_par_QImage( 1 );
   if( p )
      hb_retptrGC( hbqt_gcAllocate_QRect( new QRect( ( p )->rect() ), true ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QIMAGE_RECT FP=hb_retptrGC( hbqt_gcAllocate_QRect( new QRect( ( p )->rect() ), true ) ); p is NULL" ) );
   }
}

/*
 * QImage rgbSwapped () const
 */
HB_FUNC( QT_QIMAGE_RGBSWAPPED )
{
   QImage * p = hbqt_par_QImage( 1 );
   if( p )
      hb_retptrGC( hbqt_gcAllocate_QImage( new QImage( ( p )->rgbSwapped() ), true ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QIMAGE_RGBSWAPPED FP=hb_retptrGC( hbqt_gcAllocate_QImage( new QImage( ( p )->rgbSwapped() ), true ) ); p is NULL" ) );
   }
}

/*
 * bool save ( const QString & fileName, const char * format = 0, int quality = -1 ) const
 */
HB_FUNC( QT_QIMAGE_SAVE )
{
   QImage * p = hbqt_par_QImage( 1 );
   if( p )
      hb_retl( ( p )->save( hbqt_par_QString( 2 ), hbqt_par_char( 3 ), hb_parnidef( 4, -1 ) ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QIMAGE_SAVE FP=hb_retl( ( p )->save( hbqt_par_QString( 2 ), hbqt_par_char( 3 ), hb_parnidef( 4, -1 ) ) ); p is NULL" ) );
   }
}

/*
 * bool save ( QIODevice * device, const char * format = 0, int quality = -1 ) const
 */
HB_FUNC( QT_QIMAGE_SAVE_1 )
{
   QImage * p = hbqt_par_QImage( 1 );
   if( p )
      hb_retl( ( p )->save( hbqt_par_QIODevice( 2 ), hbqt_par_char( 3 ), hb_parnidef( 4, -1 ) ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QIMAGE_SAVE_1 FP=hb_retl( ( p )->save( hbqt_par_QIODevice( 2 ), hbqt_par_char( 3 ), hb_parnidef( 4, -1 ) ) ); p is NULL" ) );
   }
}

/*
 * QImage scaled ( const QSize & size, Qt::AspectRatioMode aspectRatioMode = Qt::IgnoreAspectRatio, Qt::TransformationMode transformMode = Qt::FastTransformation ) const
 */
HB_FUNC( QT_QIMAGE_SCALED )
{
   QImage * p = hbqt_par_QImage( 1 );
   if( p )
      hb_retptrGC( hbqt_gcAllocate_QImage( new QImage( ( p )->scaled( *hbqt_par_QSize( 2 ), ( HB_ISNUM( 3 ) ? ( Qt::AspectRatioMode ) hb_parni( 3 ) : ( Qt::AspectRatioMode ) Qt::IgnoreAspectRatio ), ( HB_ISNUM( 4 ) ? ( Qt::TransformationMode ) hb_parni( 4 ) : ( Qt::TransformationMode ) Qt::FastTransformation ) ) ), true ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QIMAGE_SCALED FP=hb_retptrGC( hbqt_gcAllocate_QImage( new QImage( ( p )->scaled( *hbqt_par_QSize( 2 ), ( HB_ISNUM( 3 ) ? ( Qt::AspectRatioMode ) hb_parni( 3 ) : ( Qt::AspectRatioMode ) Qt::IgnoreAspectRatio ), ( HB_ISNUM( 4 ) ? ( Qt::TransformationMode ) hb_parni( 4 ) : ( Qt::TransformationMode ) Qt::FastTransformation ) ) ), true ) ); p is NULL" ) );
   }
}

/*
 * QImage scaled ( int width, int height, Qt::AspectRatioMode aspectRatioMode = Qt::IgnoreAspectRatio, Qt::TransformationMode transformMode = Qt::FastTransformation ) const
 */
HB_FUNC( QT_QIMAGE_SCALED_1 )
{
   QImage * p = hbqt_par_QImage( 1 );
   if( p )
      hb_retptrGC( hbqt_gcAllocate_QImage( new QImage( ( p )->scaled( hb_parni( 2 ), hb_parni( 3 ), ( HB_ISNUM( 4 ) ? ( Qt::AspectRatioMode ) hb_parni( 4 ) : ( Qt::AspectRatioMode ) Qt::IgnoreAspectRatio ), ( HB_ISNUM( 5 ) ? ( Qt::TransformationMode ) hb_parni( 5 ) : ( Qt::TransformationMode ) Qt::FastTransformation ) ) ), true ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QIMAGE_SCALED_1 FP=hb_retptrGC( hbqt_gcAllocate_QImage( new QImage( ( p )->scaled( hb_parni( 2 ), hb_parni( 3 ), ( HB_ISNUM( 4 ) ? ( Qt::AspectRatioMode ) hb_parni( 4 ) : ( Qt::AspectRatioMode ) Qt::IgnoreAspectRatio ), ( HB_ISNUM( 5 ) ? ( Qt::TransformationMode ) hb_parni( 5 ) : ( Qt::TransformationMode ) Qt::FastTransformation ) ) ), true ) ); p is NULL" ) );
   }
}

/*
 * QImage scaledToHeight ( int height, Qt::TransformationMode mode = Qt::FastTransformation ) const
 */
HB_FUNC( QT_QIMAGE_SCALEDTOHEIGHT )
{
   QImage * p = hbqt_par_QImage( 1 );
   if( p )
      hb_retptrGC( hbqt_gcAllocate_QImage( new QImage( ( p )->scaledToHeight( hb_parni( 2 ), ( HB_ISNUM( 3 ) ? ( Qt::TransformationMode ) hb_parni( 3 ) : ( Qt::TransformationMode ) Qt::FastTransformation ) ) ), true ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QIMAGE_SCALEDTOHEIGHT FP=hb_retptrGC( hbqt_gcAllocate_QImage( new QImage( ( p )->scaledToHeight( hb_parni( 2 ), ( HB_ISNUM( 3 ) ? ( Qt::TransformationMode ) hb_parni( 3 ) : ( Qt::TransformationMode ) Qt::FastTransformation ) ) ), true ) ); p is NULL" ) );
   }
}

/*
 * QImage scaledToWidth ( int width, Qt::TransformationMode mode = Qt::FastTransformation ) const
 */
HB_FUNC( QT_QIMAGE_SCALEDTOWIDTH )
{
   QImage * p = hbqt_par_QImage( 1 );
   if( p )
      hb_retptrGC( hbqt_gcAllocate_QImage( new QImage( ( p )->scaledToWidth( hb_parni( 2 ), ( HB_ISNUM( 3 ) ? ( Qt::TransformationMode ) hb_parni( 3 ) : ( Qt::TransformationMode ) Qt::FastTransformation ) ) ), true ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QIMAGE_SCALEDTOWIDTH FP=hb_retptrGC( hbqt_gcAllocate_QImage( new QImage( ( p )->scaledToWidth( hb_parni( 2 ), ( HB_ISNUM( 3 ) ? ( Qt::TransformationMode ) hb_parni( 3 ) : ( Qt::TransformationMode ) Qt::FastTransformation ) ) ), true ) ); p is NULL" ) );
   }
}

/*
 * uchar * scanLine ( int i )
 */
HB_FUNC( QT_QIMAGE_SCANLINE )
{
   QImage * p = hbqt_par_QImage( 1 );
   if( p )
      hb_retc( ( const char * ) ( p )->scanLine( hb_parni( 2 ) ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QIMAGE_SCANLINE FP=hb_retc( ( const char * ) ( p )->scanLine( hb_parni( 2 ) ) ); p is NULL" ) );
   }
}

/*
 * const uchar * scanLine ( int i ) const
 */
HB_FUNC( QT_QIMAGE_SCANLINE_1 )
{
   QImage * p = hbqt_par_QImage( 1 );
   if( p )
      hb_retc( ( const char * ) ( p )->scanLine( hb_parni( 2 ) ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QIMAGE_SCANLINE_1 FP=hb_retc( ( const char * ) ( p )->scanLine( hb_parni( 2 ) ) ); p is NULL" ) );
   }
}

/*
 * void setColor ( int index, QRgb colorValue )
 */
HB_FUNC( QT_QIMAGE_SETCOLOR )
{
   QImage * p = hbqt_par_QImage( 1 );
   if( p )
      ( p )->setColor( hb_parni( 2 ), hb_parnl( 3 ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QIMAGE_SETCOLOR FP=( p )->setColor( hb_parni( 2 ), hb_parnl( 3 ) ); p is NULL" ) );
   }
}

/*
 * void setDotsPerMeterX ( int x )
 */
HB_FUNC( QT_QIMAGE_SETDOTSPERMETERX )
{
   QImage * p = hbqt_par_QImage( 1 );
   if( p )
      ( p )->setDotsPerMeterX( hb_parni( 2 ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QIMAGE_SETDOTSPERMETERX FP=( p )->setDotsPerMeterX( hb_parni( 2 ) ); p is NULL" ) );
   }
}

/*
 * void setDotsPerMeterY ( int y )
 */
HB_FUNC( QT_QIMAGE_SETDOTSPERMETERY )
{
   QImage * p = hbqt_par_QImage( 1 );
   if( p )
      ( p )->setDotsPerMeterY( hb_parni( 2 ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QIMAGE_SETDOTSPERMETERY FP=( p )->setDotsPerMeterY( hb_parni( 2 ) ); p is NULL" ) );
   }
}

/*
 * void setNumColors ( int numColors )
 */
HB_FUNC( QT_QIMAGE_SETNUMCOLORS )
{
   QImage * p = hbqt_par_QImage( 1 );
   if( p )
      ( p )->setNumColors( hb_parni( 2 ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QIMAGE_SETNUMCOLORS FP=( p )->setNumColors( hb_parni( 2 ) ); p is NULL" ) );
   }
}

/*
 * void setOffset ( const QPoint & offset )
 */
HB_FUNC( QT_QIMAGE_SETOFFSET )
{
   QImage * p = hbqt_par_QImage( 1 );
   if( p )
      ( p )->setOffset( *hbqt_par_QPoint( 2 ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QIMAGE_SETOFFSET FP=( p )->setOffset( *hbqt_par_QPoint( 2 ) ); p is NULL" ) );
   }
}

/*
 * void setPixel ( const QPoint & position, uint index_or_rgb )
 */
HB_FUNC( QT_QIMAGE_SETPIXEL )
{
   QImage * p = hbqt_par_QImage( 1 );
   if( p )
      ( p )->setPixel( *hbqt_par_QPoint( 2 ), hb_parni( 3 ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QIMAGE_SETPIXEL FP=( p )->setPixel( *hbqt_par_QPoint( 2 ), hb_parni( 3 ) ); p is NULL" ) );
   }
}

/*
 * void setPixel ( int x, int y, uint index_or_rgb )
 */
HB_FUNC( QT_QIMAGE_SETPIXEL_1 )
{
   QImage * p = hbqt_par_QImage( 1 );
   if( p )
      ( p )->setPixel( hb_parni( 2 ), hb_parni( 3 ), hb_parni( 4 ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QIMAGE_SETPIXEL_1 FP=( p )->setPixel( hb_parni( 2 ), hb_parni( 3 ), hb_parni( 4 ) ); p is NULL" ) );
   }
}

/*
 * void setText ( const QString & key, const QString & text )
 */
HB_FUNC( QT_QIMAGE_SETTEXT )
{
   QImage * p = hbqt_par_QImage( 1 );
   if( p )
      ( p )->setText( hbqt_par_QString( 2 ), hbqt_par_QString( 3 ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QIMAGE_SETTEXT FP=( p )->setText( hbqt_par_QString( 2 ), hbqt_par_QString( 3 ) ); p is NULL" ) );
   }
}

/*
 * QSize size () const
 */
HB_FUNC( QT_QIMAGE_SIZE )
{
   QImage * p = hbqt_par_QImage( 1 );
   if( p )
      hb_retptrGC( hbqt_gcAllocate_QSize( new QSize( ( p )->size() ), true ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QIMAGE_SIZE FP=hb_retptrGC( hbqt_gcAllocate_QSize( new QSize( ( p )->size() ), true ) ); p is NULL" ) );
   }
}

/*
 * QString text ( const QString & key = QString() ) const
 */
HB_FUNC( QT_QIMAGE_TEXT )
{
   QImage * p = hbqt_par_QImage( 1 );
   if( p )
      hb_retc( ( p )->text( hbqt_par_QString( 2 ) ).toAscii().data() );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QIMAGE_TEXT FP=hb_retc( ( p )->text( hbqt_par_QString( 2 ) ).toAscii().data() ); p is NULL" ) );
   }
}

/*
 * QStringList textKeys () const
 */
HB_FUNC( QT_QIMAGE_TEXTKEYS )
{
   QImage * p = hbqt_par_QImage( 1 );
   if( p )
      hb_retptrGC( hbqt_gcAllocate_QStringList( new QStringList( ( p )->textKeys() ), true ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QIMAGE_TEXTKEYS FP=hb_retptrGC( hbqt_gcAllocate_QStringList( new QStringList( ( p )->textKeys() ), true ) ); p is NULL" ) );
   }
}

/*
 * QImage transformed ( const QMatrix & matrix, Qt::TransformationMode mode = Qt::FastTransformation ) const
 */
HB_FUNC( QT_QIMAGE_TRANSFORMED )
{
   QImage * p = hbqt_par_QImage( 1 );
   if( p )
      hb_retptrGC( hbqt_gcAllocate_QImage( new QImage( ( p )->transformed( *hbqt_par_QMatrix( 2 ), ( HB_ISNUM( 3 ) ? ( Qt::TransformationMode ) hb_parni( 3 ) : ( Qt::TransformationMode ) Qt::FastTransformation ) ) ), true ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QIMAGE_TRANSFORMED FP=hb_retptrGC( hbqt_gcAllocate_QImage( new QImage( ( p )->transformed( *hbqt_par_QMatrix( 2 ), ( HB_ISNUM( 3 ) ? ( Qt::TransformationMode ) hb_parni( 3 ) : ( Qt::TransformationMode ) Qt::FastTransformation ) ) ), true ) ); p is NULL" ) );
   }
}

/*
 * QImage transformed ( const QTransform & matrix, Qt::TransformationMode mode = Qt::FastTransformation ) const
 */
HB_FUNC( QT_QIMAGE_TRANSFORMED_1 )
{
   QImage * p = hbqt_par_QImage( 1 );
   if( p )
      hb_retptrGC( hbqt_gcAllocate_QImage( new QImage( ( p )->transformed( *hbqt_par_QTransform( 2 ), ( HB_ISNUM( 3 ) ? ( Qt::TransformationMode ) hb_parni( 3 ) : ( Qt::TransformationMode ) Qt::FastTransformation ) ) ), true ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QIMAGE_TRANSFORMED_1 FP=hb_retptrGC( hbqt_gcAllocate_QImage( new QImage( ( p )->transformed( *hbqt_par_QTransform( 2 ), ( HB_ISNUM( 3 ) ? ( Qt::TransformationMode ) hb_parni( 3 ) : ( Qt::TransformationMode ) Qt::FastTransformation ) ) ), true ) ); p is NULL" ) );
   }
}

/*
 * bool valid ( const QPoint & pos ) const
 */
HB_FUNC( QT_QIMAGE_VALID )
{
   QImage * p = hbqt_par_QImage( 1 );
   if( p )
      hb_retl( ( p )->valid( *hbqt_par_QPoint( 2 ) ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QIMAGE_VALID FP=hb_retl( ( p )->valid( *hbqt_par_QPoint( 2 ) ) ); p is NULL" ) );
   }
}

/*
 * bool valid ( int x, int y ) const
 */
HB_FUNC( QT_QIMAGE_VALID_1 )
{
   QImage * p = hbqt_par_QImage( 1 );
   if( p )
      hb_retl( ( p )->valid( hb_parni( 2 ), hb_parni( 3 ) ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QIMAGE_VALID_1 FP=hb_retl( ( p )->valid( hb_parni( 2 ), hb_parni( 3 ) ) ); p is NULL" ) );
   }
}

/*
 * int width () const
 */
HB_FUNC( QT_QIMAGE_WIDTH )
{
   QImage * p = hbqt_par_QImage( 1 );
   if( p )
      hb_retni( ( p )->width() );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QIMAGE_WIDTH FP=hb_retni( ( p )->width() ); p is NULL" ) );
   }
}


/*----------------------------------------------------------------------*/
#endif             /* #if QT_VERSION >= 0x040500 */
/*----------------------------------------------------------------------*/
