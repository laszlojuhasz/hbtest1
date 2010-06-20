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

/*----------------------------------------------------------------------*/
#if QT_VERSION >= 0x040500
/*----------------------------------------------------------------------*/

/*
 *  enum ElementType { MoveToElement, LineToElement, CurveToElement, CurveToDataElement }
 */

/*
 *  Constructed[ 51/56 [ 91.07% ] ]
 *
 *  *** Unconvered Prototypes ***
 *  -----------------------------
 *
 *  QList<QPolygonF> toFillPolygons ( const QTransform & matrix ) const
 *  QList<QPolygonF> toFillPolygons ( const QMatrix & matrix = QMatrix() ) const
 *  QList<QPolygonF> toSubpathPolygons ( const QTransform & matrix ) const
 *  QList<QPolygonF> toSubpathPolygons ( const QMatrix & matrix = QMatrix() ) const
 *
 *  *** Commented out protos which construct fine but do not compile ***
 *
 *  // const QPainterPath::Element & elementAt ( int index ) const
 */

#include <QtCore/QPointer>

#include <QtGui/QPainterPath>


/* QPainterPath ()
 * QPainterPath ( const QPointF & startPoint )
 * QPainterPath ( const QPainterPath & path )
 * ~QPainterPath ()
 */

typedef struct
{
   QPainterPath * ph;
   bool bNew;
   QT_G_FUNC_PTR func;
} QGC_POINTER_QPainterPath;

QT_G_FUNC( hbqt_gcRelease_QPainterPath )
{
   QGC_POINTER * p = ( QGC_POINTER * ) Cargo;

   if( p && p->bNew )
   {
      if( p->ph )
      {
         HB_TRACE( HB_TR_DEBUG, ( "ph=%p    _rel_QPainterPath   /.\\", p->ph ) );
         delete ( ( QPainterPath * ) p->ph );
         HB_TRACE( HB_TR_DEBUG, ( "ph=%p YES_rel_QPainterPath   \\./", p->ph ) );
         p->ph = NULL;
      }
      else
      {
         HB_TRACE( HB_TR_DEBUG, ( "ph=%p DEL_rel_QPainterPath    :     Object already deleted!", p->ph ) );
         p->ph = NULL;
      }
   }
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "ph=%p PTR_rel_QPainterPath    :    Object not created with new=true", p->ph ) );
      p->ph = NULL;
   }
}

void * hbqt_gcAllocate_QPainterPath( void * pObj, bool bNew )
{
   QGC_POINTER * p = ( QGC_POINTER * ) hb_gcAllocate( sizeof( QGC_POINTER ), hbqt_gcFuncs() );

   p->ph = ( QPainterPath * ) pObj;
   p->bNew = bNew;
   p->func = hbqt_gcRelease_QPainterPath;

   if( bNew )
   {
      HB_TRACE( HB_TR_DEBUG, ( "ph=%p    _new_QPainterPath", pObj ) );
   }
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "ph=%p NOT_new_QPainterPath", pObj ) );
   }
   return p;
}

HB_FUNC( QT_QPAINTERPATH )
{
   QPainterPath * pObj = NULL;

   pObj = new QPainterPath() ;

   hb_retptrGC( hbqt_gcAllocate_QPainterPath( ( void * ) pObj, true ) );
}

/*
 * void addEllipse ( const QRectF & boundingRectangle )
 */
HB_FUNC( QT_QPAINTERPATH_ADDELLIPSE )
{
   QPainterPath * p = hbqt_par_QPainterPath( 1 );
   if( p )
      ( p )->addEllipse( *hbqt_par_QRectF( 2 ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QPAINTERPATH_ADDELLIPSE FP=( p )->addEllipse( *hbqt_par_QRectF( 2 ) ); p is NULL" ) );
   }
}

/*
 * void addEllipse ( qreal x, qreal y, qreal width, qreal height )
 */
HB_FUNC( QT_QPAINTERPATH_ADDELLIPSE_1 )
{
   QPainterPath * p = hbqt_par_QPainterPath( 1 );
   if( p )
      ( p )->addEllipse( hb_parnd( 2 ), hb_parnd( 3 ), hb_parnd( 4 ), hb_parnd( 5 ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QPAINTERPATH_ADDELLIPSE_1 FP=( p )->addEllipse( hb_parnd( 2 ), hb_parnd( 3 ), hb_parnd( 4 ), hb_parnd( 5 ) ); p is NULL" ) );
   }
}

/*
 * void addEllipse ( const QPointF & center, qreal rx, qreal ry )
 */
HB_FUNC( QT_QPAINTERPATH_ADDELLIPSE_2 )
{
   QPainterPath * p = hbqt_par_QPainterPath( 1 );
   if( p )
      ( p )->addEllipse( *hbqt_par_QPointF( 2 ), hb_parnd( 3 ), hb_parnd( 4 ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QPAINTERPATH_ADDELLIPSE_2 FP=( p )->addEllipse( *hbqt_par_QPointF( 2 ), hb_parnd( 3 ), hb_parnd( 4 ) ); p is NULL" ) );
   }
}

/*
 * void addPath ( const QPainterPath & path )
 */
HB_FUNC( QT_QPAINTERPATH_ADDPATH )
{
   QPainterPath * p = hbqt_par_QPainterPath( 1 );
   if( p )
      ( p )->addPath( *hbqt_par_QPainterPath( 2 ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QPAINTERPATH_ADDPATH FP=( p )->addPath( *hbqt_par_QPainterPath( 2 ) ); p is NULL" ) );
   }
}

/*
 * void addPolygon ( const QPolygonF & polygon )
 */
HB_FUNC( QT_QPAINTERPATH_ADDPOLYGON )
{
   QPainterPath * p = hbqt_par_QPainterPath( 1 );
   if( p )
      ( p )->addPolygon( *hbqt_par_QPolygonF( 2 ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QPAINTERPATH_ADDPOLYGON FP=( p )->addPolygon( *hbqt_par_QPolygonF( 2 ) ); p is NULL" ) );
   }
}

/*
 * void addRect ( const QRectF & rectangle )
 */
HB_FUNC( QT_QPAINTERPATH_ADDRECT )
{
   QPainterPath * p = hbqt_par_QPainterPath( 1 );
   if( p )
      ( p )->addRect( *hbqt_par_QRectF( 2 ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QPAINTERPATH_ADDRECT FP=( p )->addRect( *hbqt_par_QRectF( 2 ) ); p is NULL" ) );
   }
}

/*
 * void addRect ( qreal x, qreal y, qreal width, qreal height )
 */
HB_FUNC( QT_QPAINTERPATH_ADDRECT_1 )
{
   QPainterPath * p = hbqt_par_QPainterPath( 1 );
   if( p )
      ( p )->addRect( hb_parnd( 2 ), hb_parnd( 3 ), hb_parnd( 4 ), hb_parnd( 5 ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QPAINTERPATH_ADDRECT_1 FP=( p )->addRect( hb_parnd( 2 ), hb_parnd( 3 ), hb_parnd( 4 ), hb_parnd( 5 ) ); p is NULL" ) );
   }
}

/*
 * void addRegion ( const QRegion & region )
 */
HB_FUNC( QT_QPAINTERPATH_ADDREGION )
{
   QPainterPath * p = hbqt_par_QPainterPath( 1 );
   if( p )
      ( p )->addRegion( *hbqt_par_QRegion( 2 ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QPAINTERPATH_ADDREGION FP=( p )->addRegion( *hbqt_par_QRegion( 2 ) ); p is NULL" ) );
   }
}

/*
 * void addRoundedRect ( const QRectF & rect, qreal xRadius, qreal yRadius, Qt::SizeMode mode = Qt::AbsoluteSize )
 */
HB_FUNC( QT_QPAINTERPATH_ADDROUNDEDRECT )
{
   QPainterPath * p = hbqt_par_QPainterPath( 1 );
   if( p )
      ( p )->addRoundedRect( *hbqt_par_QRectF( 2 ), hb_parnd( 3 ), hb_parnd( 4 ), ( HB_ISNUM( 5 ) ? ( Qt::SizeMode ) hb_parni( 5 ) : ( Qt::SizeMode ) Qt::AbsoluteSize ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QPAINTERPATH_ADDROUNDEDRECT FP=( p )->addRoundedRect( *hbqt_par_QRectF( 2 ), hb_parnd( 3 ), hb_parnd( 4 ), ( HB_ISNUM( 5 ) ? ( Qt::SizeMode ) hb_parni( 5 ) : ( Qt::SizeMode ) Qt::AbsoluteSize ) ); p is NULL" ) );
   }
}

/*
 * void addRoundedRect ( qreal x, qreal y, qreal w, qreal h, qreal xRadius, qreal yRadius, Qt::SizeMode mode = Qt::AbsoluteSize )
 */
HB_FUNC( QT_QPAINTERPATH_ADDROUNDEDRECT_1 )
{
   QPainterPath * p = hbqt_par_QPainterPath( 1 );
   if( p )
      ( p )->addRoundedRect( hb_parnd( 2 ), hb_parnd( 3 ), hb_parnd( 4 ), hb_parnd( 5 ), hb_parnd( 6 ), hb_parnd( 7 ), ( HB_ISNUM( 8 ) ? ( Qt::SizeMode ) hb_parni( 8 ) : ( Qt::SizeMode ) Qt::AbsoluteSize ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QPAINTERPATH_ADDROUNDEDRECT_1 FP=( p )->addRoundedRect( hb_parnd( 2 ), hb_parnd( 3 ), hb_parnd( 4 ), hb_parnd( 5 ), hb_parnd( 6 ), hb_parnd( 7 ), ( HB_ISNUM( 8 ) ? ( Qt::SizeMode ) hb_parni( 8 ) : ( Qt::SizeMode ) Qt::AbsoluteSize ) ); p is NULL" ) );
   }
}

/*
 * void addText ( const QPointF & point, const QFont & font, const QString & text )
 */
HB_FUNC( QT_QPAINTERPATH_ADDTEXT )
{
   QPainterPath * p = hbqt_par_QPainterPath( 1 );
   if( p )
      ( p )->addText( *hbqt_par_QPointF( 2 ), *hbqt_par_QFont( 3 ), hbqt_par_QString( 4 ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QPAINTERPATH_ADDTEXT FP=( p )->addText( *hbqt_par_QPointF( 2 ), *hbqt_par_QFont( 3 ), hbqt_par_QString( 4 ) ); p is NULL" ) );
   }
}

/*
 * void addText ( qreal x, qreal y, const QFont & font, const QString & text )
 */
HB_FUNC( QT_QPAINTERPATH_ADDTEXT_1 )
{
   QPainterPath * p = hbqt_par_QPainterPath( 1 );
   if( p )
      ( p )->addText( hb_parnd( 2 ), hb_parnd( 3 ), *hbqt_par_QFont( 4 ), hbqt_par_QString( 5 ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QPAINTERPATH_ADDTEXT_1 FP=( p )->addText( hb_parnd( 2 ), hb_parnd( 3 ), *hbqt_par_QFont( 4 ), hbqt_par_QString( 5 ) ); p is NULL" ) );
   }
}

/*
 * qreal angleAtPercent ( qreal t ) const
 */
HB_FUNC( QT_QPAINTERPATH_ANGLEATPERCENT )
{
   QPainterPath * p = hbqt_par_QPainterPath( 1 );
   if( p )
      hb_retnd( ( p )->angleAtPercent( hb_parnd( 2 ) ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QPAINTERPATH_ANGLEATPERCENT FP=hb_retnd( ( p )->angleAtPercent( hb_parnd( 2 ) ) ); p is NULL" ) );
   }
}

/*
 * void arcMoveTo ( const QRectF & rectangle, qreal angle )
 */
HB_FUNC( QT_QPAINTERPATH_ARCMOVETO )
{
   QPainterPath * p = hbqt_par_QPainterPath( 1 );
   if( p )
      ( p )->arcMoveTo( *hbqt_par_QRectF( 2 ), hb_parnd( 3 ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QPAINTERPATH_ARCMOVETO FP=( p )->arcMoveTo( *hbqt_par_QRectF( 2 ), hb_parnd( 3 ) ); p is NULL" ) );
   }
}

/*
 * void arcMoveTo ( qreal x, qreal y, qreal width, qreal height, qreal angle )
 */
HB_FUNC( QT_QPAINTERPATH_ARCMOVETO_1 )
{
   QPainterPath * p = hbqt_par_QPainterPath( 1 );
   if( p )
      ( p )->arcMoveTo( hb_parnd( 2 ), hb_parnd( 3 ), hb_parnd( 4 ), hb_parnd( 5 ), hb_parnd( 6 ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QPAINTERPATH_ARCMOVETO_1 FP=( p )->arcMoveTo( hb_parnd( 2 ), hb_parnd( 3 ), hb_parnd( 4 ), hb_parnd( 5 ), hb_parnd( 6 ) ); p is NULL" ) );
   }
}

/*
 * void arcTo ( const QRectF & rectangle, qreal startAngle, qreal sweepLength )
 */
HB_FUNC( QT_QPAINTERPATH_ARCTO )
{
   QPainterPath * p = hbqt_par_QPainterPath( 1 );
   if( p )
      ( p )->arcTo( *hbqt_par_QRectF( 2 ), hb_parnd( 3 ), hb_parnd( 4 ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QPAINTERPATH_ARCTO FP=( p )->arcTo( *hbqt_par_QRectF( 2 ), hb_parnd( 3 ), hb_parnd( 4 ) ); p is NULL" ) );
   }
}

/*
 * void arcTo ( qreal x, qreal y, qreal width, qreal height, qreal startAngle, qreal sweepLength )
 */
HB_FUNC( QT_QPAINTERPATH_ARCTO_1 )
{
   QPainterPath * p = hbqt_par_QPainterPath( 1 );
   if( p )
      ( p )->arcTo( hb_parnd( 2 ), hb_parnd( 3 ), hb_parnd( 4 ), hb_parnd( 5 ), hb_parnd( 6 ), hb_parnd( 7 ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QPAINTERPATH_ARCTO_1 FP=( p )->arcTo( hb_parnd( 2 ), hb_parnd( 3 ), hb_parnd( 4 ), hb_parnd( 5 ), hb_parnd( 6 ), hb_parnd( 7 ) ); p is NULL" ) );
   }
}

/*
 * QRectF boundingRect () const
 */
HB_FUNC( QT_QPAINTERPATH_BOUNDINGRECT )
{
   QPainterPath * p = hbqt_par_QPainterPath( 1 );
   if( p )
      hb_retptrGC( hbqt_gcAllocate_QRectF( new QRectF( ( p )->boundingRect() ), true ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QPAINTERPATH_BOUNDINGRECT FP=hb_retptrGC( hbqt_gcAllocate_QRectF( new QRectF( ( p )->boundingRect() ), true ) ); p is NULL" ) );
   }
}

/*
 * void closeSubpath ()
 */
HB_FUNC( QT_QPAINTERPATH_CLOSESUBPATH )
{
   QPainterPath * p = hbqt_par_QPainterPath( 1 );
   if( p )
      ( p )->closeSubpath();
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QPAINTERPATH_CLOSESUBPATH FP=( p )->closeSubpath(); p is NULL" ) );
   }
}

/*
 * void connectPath ( const QPainterPath & path )
 */
HB_FUNC( QT_QPAINTERPATH_CONNECTPATH )
{
   QPainterPath * p = hbqt_par_QPainterPath( 1 );
   if( p )
      ( p )->connectPath( *hbqt_par_QPainterPath( 2 ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QPAINTERPATH_CONNECTPATH FP=( p )->connectPath( *hbqt_par_QPainterPath( 2 ) ); p is NULL" ) );
   }
}

/*
 * bool contains ( const QPointF & point ) const
 */
HB_FUNC( QT_QPAINTERPATH_CONTAINS )
{
   QPainterPath * p = hbqt_par_QPainterPath( 1 );
   if( p )
      hb_retl( ( p )->contains( *hbqt_par_QPointF( 2 ) ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QPAINTERPATH_CONTAINS FP=hb_retl( ( p )->contains( *hbqt_par_QPointF( 2 ) ) ); p is NULL" ) );
   }
}

/*
 * bool contains ( const QRectF & rectangle ) const
 */
HB_FUNC( QT_QPAINTERPATH_CONTAINS_1 )
{
   QPainterPath * p = hbqt_par_QPainterPath( 1 );
   if( p )
      hb_retl( ( p )->contains( *hbqt_par_QRectF( 2 ) ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QPAINTERPATH_CONTAINS_1 FP=hb_retl( ( p )->contains( *hbqt_par_QRectF( 2 ) ) ); p is NULL" ) );
   }
}

/*
 * bool contains ( const QPainterPath & p ) const
 */
HB_FUNC( QT_QPAINTERPATH_CONTAINS_2 )
{
   QPainterPath * p = hbqt_par_QPainterPath( 1 );
   if( p )
      hb_retl( ( p )->contains( *hbqt_par_QPainterPath( 2 ) ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QPAINTERPATH_CONTAINS_2 FP=hb_retl( ( p )->contains( *hbqt_par_QPainterPath( 2 ) ) ); p is NULL" ) );
   }
}

/*
 * QRectF controlPointRect () const
 */
HB_FUNC( QT_QPAINTERPATH_CONTROLPOINTRECT )
{
   QPainterPath * p = hbqt_par_QPainterPath( 1 );
   if( p )
      hb_retptrGC( hbqt_gcAllocate_QRectF( new QRectF( ( p )->controlPointRect() ), true ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QPAINTERPATH_CONTROLPOINTRECT FP=hb_retptrGC( hbqt_gcAllocate_QRectF( new QRectF( ( p )->controlPointRect() ), true ) ); p is NULL" ) );
   }
}

/*
 * void cubicTo ( const QPointF & c1, const QPointF & c2, const QPointF & endPoint )
 */
HB_FUNC( QT_QPAINTERPATH_CUBICTO )
{
   QPainterPath * p = hbqt_par_QPainterPath( 1 );
   if( p )
      ( p )->cubicTo( *hbqt_par_QPointF( 2 ), *hbqt_par_QPointF( 3 ), *hbqt_par_QPointF( 4 ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QPAINTERPATH_CUBICTO FP=( p )->cubicTo( *hbqt_par_QPointF( 2 ), *hbqt_par_QPointF( 3 ), *hbqt_par_QPointF( 4 ) ); p is NULL" ) );
   }
}

/*
 * void cubicTo ( qreal c1X, qreal c1Y, qreal c2X, qreal c2Y, qreal endPointX, qreal endPointY )
 */
HB_FUNC( QT_QPAINTERPATH_CUBICTO_1 )
{
   QPainterPath * p = hbqt_par_QPainterPath( 1 );
   if( p )
      ( p )->cubicTo( hb_parnd( 2 ), hb_parnd( 3 ), hb_parnd( 4 ), hb_parnd( 5 ), hb_parnd( 6 ), hb_parnd( 7 ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QPAINTERPATH_CUBICTO_1 FP=( p )->cubicTo( hb_parnd( 2 ), hb_parnd( 3 ), hb_parnd( 4 ), hb_parnd( 5 ), hb_parnd( 6 ), hb_parnd( 7 ) ); p is NULL" ) );
   }
}

/*
 * QPointF currentPosition () const
 */
HB_FUNC( QT_QPAINTERPATH_CURRENTPOSITION )
{
   QPainterPath * p = hbqt_par_QPainterPath( 1 );
   if( p )
      hb_retptrGC( hbqt_gcAllocate_QPointF( new QPointF( ( p )->currentPosition() ), true ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QPAINTERPATH_CURRENTPOSITION FP=hb_retptrGC( hbqt_gcAllocate_QPointF( new QPointF( ( p )->currentPosition() ), true ) ); p is NULL" ) );
   }
}

/*
 * int elementCount () const
 */
HB_FUNC( QT_QPAINTERPATH_ELEMENTCOUNT )
{
   QPainterPath * p = hbqt_par_QPainterPath( 1 );
   if( p )
      hb_retni( ( p )->elementCount() );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QPAINTERPATH_ELEMENTCOUNT FP=hb_retni( ( p )->elementCount() ); p is NULL" ) );
   }
}

/*
 * Qt::FillRule fillRule () const
 */
HB_FUNC( QT_QPAINTERPATH_FILLRULE )
{
   QPainterPath * p = hbqt_par_QPainterPath( 1 );
   if( p )
      hb_retni( ( Qt::FillRule ) ( p )->fillRule() );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QPAINTERPATH_FILLRULE FP=hb_retni( ( Qt::FillRule ) ( p )->fillRule() ); p is NULL" ) );
   }
}

/*
 * QPainterPath intersected ( const QPainterPath & p ) const
 */
HB_FUNC( QT_QPAINTERPATH_INTERSECTED )
{
   QPainterPath * p = hbqt_par_QPainterPath( 1 );
   if( p )
      hb_retptrGC( hbqt_gcAllocate_QPainterPath( new QPainterPath( ( p )->intersected( *hbqt_par_QPainterPath( 2 ) ) ), true ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QPAINTERPATH_INTERSECTED FP=hb_retptrGC( hbqt_gcAllocate_QPainterPath( new QPainterPath( ( p )->intersected( *hbqt_par_QPainterPath( 2 ) ) ), true ) ); p is NULL" ) );
   }
}

/*
 * bool intersects ( const QRectF & rectangle ) const
 */
HB_FUNC( QT_QPAINTERPATH_INTERSECTS )
{
   QPainterPath * p = hbqt_par_QPainterPath( 1 );
   if( p )
      hb_retl( ( p )->intersects( *hbqt_par_QRectF( 2 ) ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QPAINTERPATH_INTERSECTS FP=hb_retl( ( p )->intersects( *hbqt_par_QRectF( 2 ) ) ); p is NULL" ) );
   }
}

/*
 * bool intersects ( const QPainterPath & p ) const
 */
HB_FUNC( QT_QPAINTERPATH_INTERSECTS_1 )
{
   QPainterPath * p = hbqt_par_QPainterPath( 1 );
   if( p )
      hb_retl( ( p )->intersects( *hbqt_par_QPainterPath( 2 ) ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QPAINTERPATH_INTERSECTS_1 FP=hb_retl( ( p )->intersects( *hbqt_par_QPainterPath( 2 ) ) ); p is NULL" ) );
   }
}

/*
 * bool isEmpty () const
 */
HB_FUNC( QT_QPAINTERPATH_ISEMPTY )
{
   QPainterPath * p = hbqt_par_QPainterPath( 1 );
   if( p )
      hb_retl( ( p )->isEmpty() );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QPAINTERPATH_ISEMPTY FP=hb_retl( ( p )->isEmpty() ); p is NULL" ) );
   }
}

/*
 * qreal length () const
 */
HB_FUNC( QT_QPAINTERPATH_LENGTH )
{
   QPainterPath * p = hbqt_par_QPainterPath( 1 );
   if( p )
      hb_retnd( ( p )->length() );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QPAINTERPATH_LENGTH FP=hb_retnd( ( p )->length() ); p is NULL" ) );
   }
}

/*
 * void lineTo ( const QPointF & endPoint )
 */
HB_FUNC( QT_QPAINTERPATH_LINETO )
{
   QPainterPath * p = hbqt_par_QPainterPath( 1 );
   if( p )
      ( p )->lineTo( *hbqt_par_QPointF( 2 ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QPAINTERPATH_LINETO FP=( p )->lineTo( *hbqt_par_QPointF( 2 ) ); p is NULL" ) );
   }
}

/*
 * void lineTo ( qreal x, qreal y )
 */
HB_FUNC( QT_QPAINTERPATH_LINETO_1 )
{
   QPainterPath * p = hbqt_par_QPainterPath( 1 );
   if( p )
      ( p )->lineTo( hb_parnd( 2 ), hb_parnd( 3 ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QPAINTERPATH_LINETO_1 FP=( p )->lineTo( hb_parnd( 2 ), hb_parnd( 3 ) ); p is NULL" ) );
   }
}

/*
 * void moveTo ( const QPointF & point )
 */
HB_FUNC( QT_QPAINTERPATH_MOVETO )
{
   QPainterPath * p = hbqt_par_QPainterPath( 1 );
   if( p )
      ( p )->moveTo( *hbqt_par_QPointF( 2 ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QPAINTERPATH_MOVETO FP=( p )->moveTo( *hbqt_par_QPointF( 2 ) ); p is NULL" ) );
   }
}

/*
 * void moveTo ( qreal x, qreal y )
 */
HB_FUNC( QT_QPAINTERPATH_MOVETO_1 )
{
   QPainterPath * p = hbqt_par_QPainterPath( 1 );
   if( p )
      ( p )->moveTo( hb_parnd( 2 ), hb_parnd( 3 ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QPAINTERPATH_MOVETO_1 FP=( p )->moveTo( hb_parnd( 2 ), hb_parnd( 3 ) ); p is NULL" ) );
   }
}

/*
 * qreal percentAtLength ( qreal len ) const
 */
HB_FUNC( QT_QPAINTERPATH_PERCENTATLENGTH )
{
   QPainterPath * p = hbqt_par_QPainterPath( 1 );
   if( p )
      hb_retnd( ( p )->percentAtLength( hb_parnd( 2 ) ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QPAINTERPATH_PERCENTATLENGTH FP=hb_retnd( ( p )->percentAtLength( hb_parnd( 2 ) ) ); p is NULL" ) );
   }
}

/*
 * QPointF pointAtPercent ( qreal t ) const
 */
HB_FUNC( QT_QPAINTERPATH_POINTATPERCENT )
{
   QPainterPath * p = hbqt_par_QPainterPath( 1 );
   if( p )
      hb_retptrGC( hbqt_gcAllocate_QPointF( new QPointF( ( p )->pointAtPercent( hb_parnd( 2 ) ) ), true ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QPAINTERPATH_POINTATPERCENT FP=hb_retptrGC( hbqt_gcAllocate_QPointF( new QPointF( ( p )->pointAtPercent( hb_parnd( 2 ) ) ), true ) ); p is NULL" ) );
   }
}

/*
 * void quadTo ( const QPointF & c, const QPointF & endPoint )
 */
HB_FUNC( QT_QPAINTERPATH_QUADTO )
{
   QPainterPath * p = hbqt_par_QPainterPath( 1 );
   if( p )
      ( p )->quadTo( *hbqt_par_QPointF( 2 ), *hbqt_par_QPointF( 3 ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QPAINTERPATH_QUADTO FP=( p )->quadTo( *hbqt_par_QPointF( 2 ), *hbqt_par_QPointF( 3 ) ); p is NULL" ) );
   }
}

/*
 * void quadTo ( qreal cx, qreal cy, qreal endPointX, qreal endPointY )
 */
HB_FUNC( QT_QPAINTERPATH_QUADTO_1 )
{
   QPainterPath * p = hbqt_par_QPainterPath( 1 );
   if( p )
      ( p )->quadTo( hb_parnd( 2 ), hb_parnd( 3 ), hb_parnd( 4 ), hb_parnd( 5 ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QPAINTERPATH_QUADTO_1 FP=( p )->quadTo( hb_parnd( 2 ), hb_parnd( 3 ), hb_parnd( 4 ), hb_parnd( 5 ) ); p is NULL" ) );
   }
}

/*
 * void setElementPositionAt ( int index, qreal x, qreal y )
 */
HB_FUNC( QT_QPAINTERPATH_SETELEMENTPOSITIONAT )
{
   QPainterPath * p = hbqt_par_QPainterPath( 1 );
   if( p )
      ( p )->setElementPositionAt( hb_parni( 2 ), hb_parnd( 3 ), hb_parnd( 4 ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QPAINTERPATH_SETELEMENTPOSITIONAT FP=( p )->setElementPositionAt( hb_parni( 2 ), hb_parnd( 3 ), hb_parnd( 4 ) ); p is NULL" ) );
   }
}

/*
 * void setFillRule ( Qt::FillRule fillRule )
 */
HB_FUNC( QT_QPAINTERPATH_SETFILLRULE )
{
   QPainterPath * p = hbqt_par_QPainterPath( 1 );
   if( p )
      ( p )->setFillRule( ( Qt::FillRule ) hb_parni( 2 ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QPAINTERPATH_SETFILLRULE FP=( p )->setFillRule( ( Qt::FillRule ) hb_parni( 2 ) ); p is NULL" ) );
   }
}

/*
 * QPainterPath simplified () const
 */
HB_FUNC( QT_QPAINTERPATH_SIMPLIFIED )
{
   QPainterPath * p = hbqt_par_QPainterPath( 1 );
   if( p )
      hb_retptrGC( hbqt_gcAllocate_QPainterPath( new QPainterPath( ( p )->simplified() ), true ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QPAINTERPATH_SIMPLIFIED FP=hb_retptrGC( hbqt_gcAllocate_QPainterPath( new QPainterPath( ( p )->simplified() ), true ) ); p is NULL" ) );
   }
}

/*
 * qreal slopeAtPercent ( qreal t ) const
 */
HB_FUNC( QT_QPAINTERPATH_SLOPEATPERCENT )
{
   QPainterPath * p = hbqt_par_QPainterPath( 1 );
   if( p )
      hb_retnd( ( p )->slopeAtPercent( hb_parnd( 2 ) ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QPAINTERPATH_SLOPEATPERCENT FP=hb_retnd( ( p )->slopeAtPercent( hb_parnd( 2 ) ) ); p is NULL" ) );
   }
}

/*
 * QPainterPath subtracted ( const QPainterPath & p ) const
 */
HB_FUNC( QT_QPAINTERPATH_SUBTRACTED )
{
   QPainterPath * p = hbqt_par_QPainterPath( 1 );
   if( p )
      hb_retptrGC( hbqt_gcAllocate_QPainterPath( new QPainterPath( ( p )->subtracted( *hbqt_par_QPainterPath( 2 ) ) ), true ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QPAINTERPATH_SUBTRACTED FP=hb_retptrGC( hbqt_gcAllocate_QPainterPath( new QPainterPath( ( p )->subtracted( *hbqt_par_QPainterPath( 2 ) ) ), true ) ); p is NULL" ) );
   }
}

/*
 * QPolygonF toFillPolygon ( const QTransform & matrix ) const
 */
HB_FUNC( QT_QPAINTERPATH_TOFILLPOLYGON )
{
   QPainterPath * p = hbqt_par_QPainterPath( 1 );
   if( p )
      hb_retptrGC( hbqt_gcAllocate_QPolygonF( new QPolygonF( ( p )->toFillPolygon( *hbqt_par_QTransform( 2 ) ) ), true ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QPAINTERPATH_TOFILLPOLYGON FP=hb_retptrGC( hbqt_gcAllocate_QPolygonF( new QPolygonF( ( p )->toFillPolygon( *hbqt_par_QTransform( 2 ) ) ), true ) ); p is NULL" ) );
   }
}

/*
 * QPolygonF toFillPolygon ( const QMatrix & matrix = QMatrix() ) const
 */
HB_FUNC( QT_QPAINTERPATH_TOFILLPOLYGON_1 )
{
   QPainterPath * p = hbqt_par_QPainterPath( 1 );
   if( p )
      hb_retptrGC( hbqt_gcAllocate_QPolygonF( new QPolygonF( ( p )->toFillPolygon( ( HB_ISPOINTER( 2 ) ? *hbqt_par_QMatrix( 2 ) : QMatrix() ) ) ), true ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QPAINTERPATH_TOFILLPOLYGON_1 FP=hb_retptrGC( hbqt_gcAllocate_QPolygonF( new QPolygonF( ( p )->toFillPolygon( ( HB_ISPOINTER( 2 ) ? *hbqt_par_QMatrix( 2 ) : QMatrix() ) ) ), true ) ); p is NULL" ) );
   }
}

/*
 * QPainterPath toReversed () const
 */
HB_FUNC( QT_QPAINTERPATH_TOREVERSED )
{
   QPainterPath * p = hbqt_par_QPainterPath( 1 );
   if( p )
      hb_retptrGC( hbqt_gcAllocate_QPainterPath( new QPainterPath( ( p )->toReversed() ), true ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QPAINTERPATH_TOREVERSED FP=hb_retptrGC( hbqt_gcAllocate_QPainterPath( new QPainterPath( ( p )->toReversed() ), true ) ); p is NULL" ) );
   }
}

/*
 * QPainterPath united ( const QPainterPath & p ) const
 */
HB_FUNC( QT_QPAINTERPATH_UNITED )
{
   QPainterPath * p = hbqt_par_QPainterPath( 1 );
   if( p )
      hb_retptrGC( hbqt_gcAllocate_QPainterPath( new QPainterPath( ( p )->united( *hbqt_par_QPainterPath( 2 ) ) ), true ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QPAINTERPATH_UNITED FP=hb_retptrGC( hbqt_gcAllocate_QPainterPath( new QPainterPath( ( p )->united( *hbqt_par_QPainterPath( 2 ) ) ), true ) ); p is NULL" ) );
   }
}


/*----------------------------------------------------------------------*/
#endif             /* #if QT_VERSION >= 0x040500 */
/*----------------------------------------------------------------------*/
