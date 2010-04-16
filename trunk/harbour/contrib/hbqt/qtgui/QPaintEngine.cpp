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

/*
 *  enum DirtyFlag { DirtyPen, DirtyBrush, DirtyBrushOrigin, DirtyFont, ..., AllDirty }
 *  flags DirtyFlags
 *  enum PaintEngineFeature { AlphaBlend, Antialiasing, BlendModes, BrushStroke, ..., AllFeatures }
 *  flags PaintEngineFeatures
 *  enum PolygonDrawMode { OddEvenMode, WindingMode, ConvexMode, PolylineMode }
 *  enum Type { X11, Windows, MacPrinter, CoreGraphics, ..., MaxUser }
 */

#include <QtCore/QPointer>

#include <QtGui/QPaintEngine>


/*
 * QPaintEngine ( PaintEngineFeatures caps = 0 )
 * virtual ~QPaintEngine ()
 */

typedef struct
{
   QPaintEngine * ph;
   bool bNew;
   QT_G_FUNC_PTR func;
} QGC_POINTER_QPaintEngine;

QT_G_FUNC( hbqt_gcRelease_QPaintEngine )
{
   HB_SYMBOL_UNUSED( Cargo );
   QGC_POINTER * p = ( QGC_POINTER * ) Cargo;

   if( p && p->bNew )
   {
      p->ph = NULL;
   }
}

void * hbqt_gcAllocate_QPaintEngine( void * pObj, bool bNew )
{
   QGC_POINTER * p = ( QGC_POINTER * ) hb_gcAllocate( sizeof( QGC_POINTER ), hbqt_gcFuncs() );

   p->ph = ( QPaintEngine * ) pObj;
   p->bNew = bNew;
   p->func = hbqt_gcRelease_QPaintEngine;

   if( bNew )
   {
      HB_TRACE( HB_TR_DEBUG, ( "ph=%p    _new_QPaintEngine", pObj ) );
   }
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "ph=%p NOT_new_QPaintEngine", pObj ) );
   }
   return p;
}

HB_FUNC( QT_QPAINTENGINE )
{

}

/*
 * virtual bool begin ( QPaintDevice * pdev ) = 0
 */
HB_FUNC( QT_QPAINTENGINE_BEGIN )
{
   QPaintEngine * p = hbqt_par_QPaintEngine( 1 );
   if( p )
      hb_retl( ( p )->begin( hbqt_par_QPaintDevice( 2 ) ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QPAINTENGINE_BEGIN FP=hb_retl( ( p )->begin( hbqt_par_QPaintDevice( 2 ) ) ); p is NULL" ) );
   }
}

/*
 * virtual void drawEllipse ( const QRectF & rect )
 */
HB_FUNC( QT_QPAINTENGINE_DRAWELLIPSE )
{
   QPaintEngine * p = hbqt_par_QPaintEngine( 1 );
   if( p )
      ( p )->drawEllipse( *hbqt_par_QRectF( 2 ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QPAINTENGINE_DRAWELLIPSE FP=( p )->drawEllipse( *hbqt_par_QRectF( 2 ) ); p is NULL" ) );
   }
}

/*
 * virtual void drawEllipse ( const QRect & rect )
 */
HB_FUNC( QT_QPAINTENGINE_DRAWELLIPSE_1 )
{
   QPaintEngine * p = hbqt_par_QPaintEngine( 1 );
   if( p )
      ( p )->drawEllipse( *hbqt_par_QRect( 2 ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QPAINTENGINE_DRAWELLIPSE_1 FP=( p )->drawEllipse( *hbqt_par_QRect( 2 ) ); p is NULL" ) );
   }
}

/*
 * virtual void drawImage ( const QRectF & rectangle, const QImage & image, const QRectF & sr, Qt::ImageConversionFlags flags = Qt::AutoColor )
 */
HB_FUNC( QT_QPAINTENGINE_DRAWIMAGE )
{
   QPaintEngine * p = hbqt_par_QPaintEngine( 1 );
   if( p )
      ( p )->drawImage( *hbqt_par_QRectF( 2 ), *hbqt_par_QImage( 3 ), *hbqt_par_QRectF( 4 ), ( HB_ISNUM( 5 ) ? ( Qt::ImageConversionFlags ) hb_parni( 5 ) : ( Qt::ImageConversionFlags ) Qt::AutoColor ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QPAINTENGINE_DRAWIMAGE FP=( p )->drawImage( *hbqt_par_QRectF( 2 ), *hbqt_par_QImage( 3 ), *hbqt_par_QRectF( 4 ), ( HB_ISNUM( 5 ) ? ( Qt::ImageConversionFlags ) hb_parni( 5 ) : ( Qt::ImageConversionFlags ) Qt::AutoColor ) ); p is NULL" ) );
   }
}

/*
 * virtual void drawLines ( const QLineF * lines, int lineCount )
 */
HB_FUNC( QT_QPAINTENGINE_DRAWLINES )
{
   QPaintEngine * p = hbqt_par_QPaintEngine( 1 );
   if( p )
      ( p )->drawLines( hbqt_par_QLineF( 2 ), hb_parni( 3 ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QPAINTENGINE_DRAWLINES FP=( p )->drawLines( hbqt_par_QLineF( 2 ), hb_parni( 3 ) ); p is NULL" ) );
   }
}

/*
 * virtual void drawLines ( const QLine * lines, int lineCount )
 */
HB_FUNC( QT_QPAINTENGINE_DRAWLINES_1 )
{
   QPaintEngine * p = hbqt_par_QPaintEngine( 1 );
   if( p )
      ( p )->drawLines( hbqt_par_QLine( 2 ), hb_parni( 3 ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QPAINTENGINE_DRAWLINES_1 FP=( p )->drawLines( hbqt_par_QLine( 2 ), hb_parni( 3 ) ); p is NULL" ) );
   }
}

/*
 * virtual void drawPath ( const QPainterPath & path )
 */
HB_FUNC( QT_QPAINTENGINE_DRAWPATH )
{
   QPaintEngine * p = hbqt_par_QPaintEngine( 1 );
   if( p )
      ( p )->drawPath( *hbqt_par_QPainterPath( 2 ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QPAINTENGINE_DRAWPATH FP=( p )->drawPath( *hbqt_par_QPainterPath( 2 ) ); p is NULL" ) );
   }
}

/*
 * virtual void drawPixmap ( const QRectF & r, const QPixmap & pm, const QRectF & sr ) = 0
 */
HB_FUNC( QT_QPAINTENGINE_DRAWPIXMAP )
{
   QPaintEngine * p = hbqt_par_QPaintEngine( 1 );
   if( p )
      ( p )->drawPixmap( *hbqt_par_QRectF( 2 ), *hbqt_par_QPixmap( 3 ), *hbqt_par_QRectF( 4 ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QPAINTENGINE_DRAWPIXMAP FP=( p )->drawPixmap( *hbqt_par_QRectF( 2 ), *hbqt_par_QPixmap( 3 ), *hbqt_par_QRectF( 4 ) ); p is NULL" ) );
   }
}

/*
 * virtual void drawPoints ( const QPointF * points, int pointCount )
 */
HB_FUNC( QT_QPAINTENGINE_DRAWPOINTS )
{
   QPaintEngine * p = hbqt_par_QPaintEngine( 1 );
   if( p )
      ( p )->drawPoints( hbqt_par_QPointF( 2 ), hb_parni( 3 ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QPAINTENGINE_DRAWPOINTS FP=( p )->drawPoints( hbqt_par_QPointF( 2 ), hb_parni( 3 ) ); p is NULL" ) );
   }
}

/*
 * virtual void drawPoints ( const QPoint * points, int pointCount )
 */
HB_FUNC( QT_QPAINTENGINE_DRAWPOINTS_1 )
{
   QPaintEngine * p = hbqt_par_QPaintEngine( 1 );
   if( p )
      ( p )->drawPoints( hbqt_par_QPoint( 2 ), hb_parni( 3 ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QPAINTENGINE_DRAWPOINTS_1 FP=( p )->drawPoints( hbqt_par_QPoint( 2 ), hb_parni( 3 ) ); p is NULL" ) );
   }
}

/*
 * virtual void drawPolygon ( const QPointF * points, int pointCount, PolygonDrawMode mode )
 */
HB_FUNC( QT_QPAINTENGINE_DRAWPOLYGON )
{
   QPaintEngine * p = hbqt_par_QPaintEngine( 1 );
   if( p )
      ( p )->drawPolygon( hbqt_par_QPointF( 2 ), hb_parni( 3 ), ( QPaintEngine::PolygonDrawMode ) hb_parni( 4 ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QPAINTENGINE_DRAWPOLYGON FP=( p )->drawPolygon( hbqt_par_QPointF( 2 ), hb_parni( 3 ), ( QPaintEngine::PolygonDrawMode ) hb_parni( 4 ) ); p is NULL" ) );
   }
}

/*
 * virtual void drawPolygon ( const QPoint * points, int pointCount, PolygonDrawMode mode )
 */
HB_FUNC( QT_QPAINTENGINE_DRAWPOLYGON_1 )
{
   QPaintEngine * p = hbqt_par_QPaintEngine( 1 );
   if( p )
      ( p )->drawPolygon( hbqt_par_QPoint( 2 ), hb_parni( 3 ), ( QPaintEngine::PolygonDrawMode ) hb_parni( 4 ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QPAINTENGINE_DRAWPOLYGON_1 FP=( p )->drawPolygon( hbqt_par_QPoint( 2 ), hb_parni( 3 ), ( QPaintEngine::PolygonDrawMode ) hb_parni( 4 ) ); p is NULL" ) );
   }
}

/*
 * virtual void drawRects ( const QRectF * rects, int rectCount )
 */
HB_FUNC( QT_QPAINTENGINE_DRAWRECTS )
{
   QPaintEngine * p = hbqt_par_QPaintEngine( 1 );
   if( p )
      ( p )->drawRects( hbqt_par_QRectF( 2 ), hb_parni( 3 ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QPAINTENGINE_DRAWRECTS FP=( p )->drawRects( hbqt_par_QRectF( 2 ), hb_parni( 3 ) ); p is NULL" ) );
   }
}

/*
 * virtual void drawRects ( const QRect * rects, int rectCount )
 */
HB_FUNC( QT_QPAINTENGINE_DRAWRECTS_1 )
{
   QPaintEngine * p = hbqt_par_QPaintEngine( 1 );
   if( p )
      ( p )->drawRects( hbqt_par_QRect( 2 ), hb_parni( 3 ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QPAINTENGINE_DRAWRECTS_1 FP=( p )->drawRects( hbqt_par_QRect( 2 ), hb_parni( 3 ) ); p is NULL" ) );
   }
}

/*
 * virtual void drawTextItem ( const QPointF & p, const QTextItem & textItem )
 */
HB_FUNC( QT_QPAINTENGINE_DRAWTEXTITEM )
{
   QPaintEngine * p = hbqt_par_QPaintEngine( 1 );
   if( p )
      ( p )->drawTextItem( *hbqt_par_QPointF( 2 ), *hbqt_par_QTextItem( 3 ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QPAINTENGINE_DRAWTEXTITEM FP=( p )->drawTextItem( *hbqt_par_QPointF( 2 ), *hbqt_par_QTextItem( 3 ) ); p is NULL" ) );
   }
}

/*
 * virtual void drawTiledPixmap ( const QRectF & rect, const QPixmap & pixmap, const QPointF & p )
 */
HB_FUNC( QT_QPAINTENGINE_DRAWTILEDPIXMAP )
{
   QPaintEngine * p = hbqt_par_QPaintEngine( 1 );
   if( p )
      ( p )->drawTiledPixmap( *hbqt_par_QRectF( 2 ), *hbqt_par_QPixmap( 3 ), *hbqt_par_QPointF( 4 ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QPAINTENGINE_DRAWTILEDPIXMAP FP=( p )->drawTiledPixmap( *hbqt_par_QRectF( 2 ), *hbqt_par_QPixmap( 3 ), *hbqt_par_QPointF( 4 ) ); p is NULL" ) );
   }
}

/*
 * virtual bool end () = 0
 */
HB_FUNC( QT_QPAINTENGINE_END )
{
   QPaintEngine * p = hbqt_par_QPaintEngine( 1 );
   if( p )
      hb_retl( ( p )->end() );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QPAINTENGINE_END FP=hb_retl( ( p )->end() ); p is NULL" ) );
   }
}

/*
 * bool hasFeature ( PaintEngineFeatures feature ) const
 */
HB_FUNC( QT_QPAINTENGINE_HASFEATURE )
{
   QPaintEngine * p = hbqt_par_QPaintEngine( 1 );
   if( p )
      hb_retl( ( p )->hasFeature( ( QPaintEngine::PaintEngineFeatures ) hb_parni( 2 ) ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QPAINTENGINE_HASFEATURE FP=hb_retl( ( p )->hasFeature( ( QPaintEngine::PaintEngineFeatures ) hb_parni( 2 ) ) ); p is NULL" ) );
   }
}

/*
 * bool isActive () const
 */
HB_FUNC( QT_QPAINTENGINE_ISACTIVE )
{
   QPaintEngine * p = hbqt_par_QPaintEngine( 1 );
   if( p )
      hb_retl( ( p )->isActive() );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QPAINTENGINE_ISACTIVE FP=hb_retl( ( p )->isActive() ); p is NULL" ) );
   }
}

/*
 * QPaintDevice * paintDevice () const
 */
HB_FUNC( QT_QPAINTENGINE_PAINTDEVICE )
{
   QPaintEngine * p = hbqt_par_QPaintEngine( 1 );
   if( p )
      hb_retptrGC( hbqt_gcAllocate_QPaintDevice( ( p )->paintDevice(), false ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QPAINTENGINE_PAINTDEVICE FP=hb_retptrGC( hbqt_gcAllocate_QPaintDevice( ( p )->paintDevice(), false ) ); p is NULL" ) );
   }
}

/*
 * QPainter * painter () const
 */
HB_FUNC( QT_QPAINTENGINE_PAINTER )
{
   QPaintEngine * p = hbqt_par_QPaintEngine( 1 );
   if( p )
      hb_retptrGC( hbqt_gcAllocate_QPainter( ( p )->painter(), false ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QPAINTENGINE_PAINTER FP=hb_retptrGC( hbqt_gcAllocate_QPainter( ( p )->painter(), false ) ); p is NULL" ) );
   }
}

/*
 * void setActive ( bool state )
 */
HB_FUNC( QT_QPAINTENGINE_SETACTIVE )
{
   QPaintEngine * p = hbqt_par_QPaintEngine( 1 );
   if( p )
      ( p )->setActive( hb_parl( 2 ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QPAINTENGINE_SETACTIVE FP=( p )->setActive( hb_parl( 2 ) ); p is NULL" ) );
   }
}

/*
 * virtual Type type () const = 0
 */
HB_FUNC( QT_QPAINTENGINE_TYPE )
{
   QPaintEngine * p = hbqt_par_QPaintEngine( 1 );
   if( p )
      hb_retni( ( QPaintEngine::Type ) ( p )->type() );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QPAINTENGINE_TYPE FP=hb_retni( ( QPaintEngine::Type ) ( p )->type() ); p is NULL" ) );
   }
}

/*
 * virtual void updateState ( const QPaintEngineState & state ) = 0
 */
HB_FUNC( QT_QPAINTENGINE_UPDATESTATE )
{
   QPaintEngine * p = hbqt_par_QPaintEngine( 1 );
   if( p )
      ( p )->updateState( *hbqt_par_QPaintEngineState( 2 ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QPAINTENGINE_UPDATESTATE FP=( p )->updateState( *hbqt_par_QPaintEngineState( 2 ) ); p is NULL" ) );
   }
}


/*----------------------------------------------------------------------*/
#endif             /* #if QT_VERSION >= 0x040500 */
/*----------------------------------------------------------------------*/
