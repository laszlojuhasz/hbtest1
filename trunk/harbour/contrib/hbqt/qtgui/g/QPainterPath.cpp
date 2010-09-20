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

#include "hbqtcore.h"
#include "hbqtgui.h"

/*----------------------------------------------------------------------*/
#if QT_VERSION >= 0x040500
/*----------------------------------------------------------------------*/

/*
 *  enum ElementType { MoveToElement, LineToElement, CurveToElement, CurveToDataElement }
 */

/*
 *  Constructed[ 41/80 [ 51.25% ] ]
 *
 *  *** Unconvered Prototypes ***
 *  -----------------------------
 *
 *  }
 *  }
 *  }
 *  }
 *  }
 *  }
 *  }
 *  }
 *  }
 *  }
 *  }
 *  }
 *
 *  *** Commented out protos which construct fine but do not compile ***
 *
 *  //void addEllipse ( const QRectF & boundingRectangle )
 *  //void addEllipse ( qreal x, qreal y, qreal width, qreal height )
 *  //void addEllipse ( const QPointF & center, qreal rx, qreal ry )
 *  //void addRect ( const QRectF & rectangle )
 *  //void addRect ( qreal x, qreal y, qreal width, qreal height )
 *  //void addRoundedRect ( const QRectF & rect, qreal xRadius, qreal yRadius, Qt::SizeMode mode = Qt::AbsoluteSize )
 *  //void addRoundedRect ( qreal x, qreal y, qreal w, qreal h, qreal xRadius, qreal yRadius, Qt::SizeMode mode = Qt::AbsoluteSize )
 *  //void addText ( const QPointF & point, const QFont & font, const QString & text )
 *  //void addText ( qreal x, qreal y, const QFont & font, const QString & text )
 *  //void arcMoveTo ( const QRectF & rectangle, qreal angle )
 *  //void arcMoveTo ( qreal x, qreal y, qreal width, qreal height, qreal angle )
 *  //void arcTo ( const QRectF & rectangle, qreal startAngle, qreal sweepLength )
 *  //void arcTo ( qreal x, qreal y, qreal width, qreal height, qreal startAngle, qreal sweepLength )
 *  //bool contains ( const QPointF & point ) const
 *  //bool contains ( const QRectF & rectangle ) const
 *  //bool contains ( const QPainterPath & p ) const
 *  //void cubicTo ( const QPointF & c1, const QPointF & c2, const QPointF & endPoint )
 *  //void cubicTo ( qreal c1X, qreal c1Y, qreal c2X, qreal c2Y, qreal endPointX, qreal endPointY )
 *  // const QPainterPath::Element & elementAt ( int index ) const
 *  //bool intersects ( const QRectF & rectangle ) const
 *  //bool intersects ( const QPainterPath & p ) const
 *  //void lineTo ( const QPointF & endPoint )
 *  //void lineTo ( qreal x, qreal y )
 *  //void moveTo ( const QPointF & point )
 *  //void moveTo ( qreal x, qreal y )
 *  //void quadTo ( const QPointF & c, const QPointF & endPoint )
 *  //void quadTo ( qreal cx, qreal cy, qreal endPointX, qreal endPointY )
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
   PHBQT_GC_FUNC func;
   int type;
} HBQT_GC_T_QPainterPath;

HBQT_GC_FUNC( hbqt_gcRelease_QPainterPath )
{
   HBQT_GC_T * p = ( HBQT_GC_T * ) Cargo;

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
   HBQT_GC_T * p = ( HBQT_GC_T * ) hb_gcAllocate( sizeof( HBQT_GC_T ), hbqt_gcFuncs() );

   p->ph = ( QPainterPath * ) pObj;
   p->bNew = bNew;
   p->func = hbqt_gcRelease_QPainterPath;
   p->type = HBQT_TYPE_QPainterPath;

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
 * void addEllipse ( ... )
 */
HB_FUNC( QT_QPAINTERPATH_ADDELLIPSE )
{
   QPainterPath * p = hbqt_par_QPainterPath( 1 );
   if( p )
   {
      if( hb_pcount() == 2 && HB_ISPOINTER( 2 ) )
      {
         ( p )->addEllipse( *hbqt_par_QRectF( 2 ) );
      }
      else if( hb_pcount() == 4 && HB_ISPOINTER( 2 ) )
      {
         ( p )->addEllipse( *hbqt_par_QPointF( 2 ), hb_parnd( 3 ), hb_parnd( 4 ) );
      }
      else if( hb_pcount() == 5 && HB_ISNUM( 2 ) )
      {
         ( p )->addEllipse( hb_parnd( 2 ), hb_parnd( 3 ), hb_parnd( 4 ), hb_parnd( 5 ) );
      }
   }
}

/*
 * void addPath ( const QPainterPath & path )
 */
HB_FUNC( QT_QPAINTERPATH_ADDPATH )
{
   QPainterPath * p = hbqt_par_QPainterPath( 1 );
   if( p )
   {
      ( p )->addPath( *hbqt_par_QPainterPath( 2 ) );
   }
}

/*
 * void addPolygon ( const QPolygonF & polygon )
 */
HB_FUNC( QT_QPAINTERPATH_ADDPOLYGON )
{
   QPainterPath * p = hbqt_par_QPainterPath( 1 );
   if( p )
   {
      ( p )->addPolygon( *hbqt_par_QPolygonF( 2 ) );
   }
}

/*
 * void addRect ( ... )
 */
HB_FUNC( QT_QPAINTERPATH_ADDRECT )
{
   QPainterPath * p = hbqt_par_QPainterPath( 1 );
   if( p )
   {
      if( hb_pcount() == 2 && HB_ISPOINTER( 2 ) )
      {
         ( p )->addRect( *hbqt_par_QRectF( 2 ) );
      }
      else if( hb_pcount() == 5 && HB_ISNUM( 2 ) )
      {
         ( p )->addRect( hb_parnd( 2 ), hb_parnd( 3 ), hb_parnd( 4 ), hb_parnd( 5 ) );
      }
   }
}

/*
 * void addRegion ( const QRegion & region )
 */
HB_FUNC( QT_QPAINTERPATH_ADDREGION )
{
   QPainterPath * p = hbqt_par_QPainterPath( 1 );
   if( p )
   {
      ( p )->addRegion( *hbqt_par_QRegion( 2 ) );
   }
}

/*
 * void addRoundedRect ( ... )
 */
HB_FUNC( QT_QPAINTERPATH_ADDROUNDEDRECT )
{
   QPainterPath * p = hbqt_par_QPainterPath( 1 );
   if( p )
   {
      if( hb_pcount() >= 4 && HB_ISPOINTER( 2 ) )
      {
         ( p )->addRoundedRect( *hbqt_par_QRectF( 2 ), hb_parnd( 3 ), hb_parnd( 4 ), ( Qt::SizeMode ) ( HB_ISNUM( 5 ) ? hb_parni( 5 ) : Qt::AbsoluteSize ) );
      }
      else if( hb_pcount() >= 7 && HB_ISNUM( 2 ) )
      {
         ( p )->addRoundedRect( hb_parnd( 2 ), hb_parnd( 3 ), hb_parnd( 4 ), hb_parnd( 5 ), hb_parnd( 6 ), hb_parnd( 7 ), ( Qt::SizeMode ) ( HB_ISNUM( 8 ) ? hb_parni( 8 ) : Qt::AbsoluteSize ) );
      }
   }
}

/*
 * void addText ( ... )
 */
HB_FUNC( QT_QPAINTERPATH_ADDTEXT )
{
   QPainterPath * p = hbqt_par_QPainterPath( 1 );
   if( p )
   {
      if( hb_pcount() == 4 && HB_ISPOINTER( 2 ) )
      {
         ( p )->addText( *hbqt_par_QPointF( 2 ), *hbqt_par_QFont( 3 ), hbqt_par_QString( 4 ) );
      }
      else if( hb_pcount() == 5 && HB_ISNUM( 2 ) )
      {
         ( p )->addText( hb_parnd( 2 ), hb_parnd( 3 ), *hbqt_par_QFont( 4 ), hbqt_par_QString( 5 ) );
      }
   }
}

/*
 * qreal angleAtPercent ( qreal t ) const
 */
HB_FUNC( QT_QPAINTERPATH_ANGLEATPERCENT )
{
   QPainterPath * p = hbqt_par_QPainterPath( 1 );
   if( p )
   {
      hb_retnd( ( p )->angleAtPercent( hb_parnd( 2 ) ) );
   }
}

/*
 * void arcMoveTo ( ... )
 */
HB_FUNC( QT_QPAINTERPATH_ARCMOVETO )
{
   QPainterPath * p = hbqt_par_QPainterPath( 1 );
   if( p )
   {
      if( hb_pcount() == 3 && HB_ISPOINTER( 2 ) )
      {
         ( p )->arcMoveTo( *hbqt_par_QRectF( 2 ), hb_parnd( 3 ) );
      }
      else if( hb_pcount() == 6 && HB_ISNUM( 2 ) )
      {
         ( p )->arcMoveTo( hb_parnd( 2 ), hb_parnd( 3 ), hb_parnd( 4 ), hb_parnd( 5 ), hb_parnd( 6 ) );
      }
   }
}

/*
 * void arcTo ( ... )
 */
HB_FUNC( QT_QPAINTERPATH_ARCTO )
{
   QPainterPath * p = hbqt_par_QPainterPath( 1 );
   if( p )
   {
      if( hb_pcount() == 4 && HB_ISPOINTER( 2 ) )
      {
         ( p )->arcTo( *hbqt_par_QRectF( 2 ), hb_parnd( 3 ), hb_parnd( 4 ) );
      }
      else if( hb_pcount() == 7 && HB_ISNUM( 2 ) )
      {
         ( p )->arcTo( hb_parnd( 2 ), hb_parnd( 3 ), hb_parnd( 4 ), hb_parnd( 5 ), hb_parnd( 6 ), hb_parnd( 7 ) );
      }
   }
}

/*
 * QRectF boundingRect () const
 */
HB_FUNC( QT_QPAINTERPATH_BOUNDINGRECT )
{
   QPainterPath * p = hbqt_par_QPainterPath( 1 );
   if( p )
   {
      hb_retptrGC( hbqt_gcAllocate_QRectF( new QRectF( ( p )->boundingRect() ), true ) );
   }
}

/*
 * void closeSubpath ()
 */
HB_FUNC( QT_QPAINTERPATH_CLOSESUBPATH )
{
   QPainterPath * p = hbqt_par_QPainterPath( 1 );
   if( p )
   {
      ( p )->closeSubpath();
   }
}

/*
 * void connectPath ( const QPainterPath & path )
 */
HB_FUNC( QT_QPAINTERPATH_CONNECTPATH )
{
   QPainterPath * p = hbqt_par_QPainterPath( 1 );
   if( p )
   {
      ( p )->connectPath( *hbqt_par_QPainterPath( 2 ) );
   }
}

/*
 * void contains ( ... )
 */
HB_FUNC( QT_QPAINTERPATH_CONTAINS )
{
   QPainterPath * p = hbqt_par_QPainterPath( 1 );
   if( p && hb_pcount() == 2 && HB_ISPOINTER( 2 ) )
   {
      HBQT_GC_T * q = ( HBQT_GC_T * ) hb_parptrGC( hbqt_gcFuncs(), 2 );

      if( q->type == HBQT_TYPE_QPointF )
      {
         ( p )->contains( *hbqt_par_QPointF( 2 ) );
      }
      else if( q->type == HBQT_TYPE_QRectF )
      {
         ( p )->contains( *hbqt_par_QRectF( 2 ) );
      }
      else if( q->type == HBQT_TYPE_QPainterPath )
      {
         ( p )->contains( *hbqt_par_QPainterPath( 2 ) );
      }
   }
}

/*
 * QRectF controlPointRect () const
 */
HB_FUNC( QT_QPAINTERPATH_CONTROLPOINTRECT )
{
   QPainterPath * p = hbqt_par_QPainterPath( 1 );
   if( p )
   {
      hb_retptrGC( hbqt_gcAllocate_QRectF( new QRectF( ( p )->controlPointRect() ), true ) );
   }
}

/*
 * void cubicTo ( ... )
 */
HB_FUNC( QT_QPAINTERPATH_CUBICTO )
{
   QPainterPath * p = hbqt_par_QPainterPath( 1 );
   if( p )
   {
      if( hb_pcount() == 4 && HB_ISPOINTER( 2 ) )
      {
         ( p )->cubicTo( *hbqt_par_QPointF( 2 ), *hbqt_par_QPointF( 3 ), *hbqt_par_QPointF( 4 ) );
      }
      else if( hb_pcount() == 7 && HB_ISNUM( 2 ) )
      {
         ( p )->cubicTo( hb_parnd( 2 ), hb_parnd( 3 ), hb_parnd( 4 ), hb_parnd( 5 ), hb_parnd( 6 ), hb_parnd( 7 ) );
      }
   }
}

/*
 * QPointF currentPosition () const
 */
HB_FUNC( QT_QPAINTERPATH_CURRENTPOSITION )
{
   QPainterPath * p = hbqt_par_QPainterPath( 1 );
   if( p )
   {
      hb_retptrGC( hbqt_gcAllocate_QPointF( new QPointF( ( p )->currentPosition() ), true ) );
   }
}

/*
 * int elementCount () const
 */
HB_FUNC( QT_QPAINTERPATH_ELEMENTCOUNT )
{
   QPainterPath * p = hbqt_par_QPainterPath( 1 );
   if( p )
   {
      hb_retni( ( p )->elementCount() );
   }
}

/*
 * Qt::FillRule fillRule () const
 */
HB_FUNC( QT_QPAINTERPATH_FILLRULE )
{
   QPainterPath * p = hbqt_par_QPainterPath( 1 );
   if( p )
   {
      hb_retni( ( Qt::FillRule ) ( p )->fillRule() );
   }
}

/*
 * QPainterPath intersected ( const QPainterPath & p ) const
 */
HB_FUNC( QT_QPAINTERPATH_INTERSECTED )
{
   QPainterPath * p = hbqt_par_QPainterPath( 1 );
   if( p )
   {
      hb_retptrGC( hbqt_gcAllocate_QPainterPath( new QPainterPath( ( p )->intersected( *hbqt_par_QPainterPath( 2 ) ) ), true ) );
   }
}

/*
 * void intersects ( ... )
 */
HB_FUNC( QT_QPAINTERPATH_INTERSECTS )
{
   QPainterPath * p = hbqt_par_QPainterPath( 1 );
   if( p && hb_pcount() == 2 && HB_ISPOINTER( 2 ) )
   {
      HBQT_GC_T * q = ( HBQT_GC_T * ) hb_parptrGC( hbqt_gcFuncs(), 2 );

      if( q->type == HBQT_TYPE_QRectF )
      {
         ( p )->intersects( *hbqt_par_QRectF( 2 ) );
      }
      else if( q->type == HBQT_TYPE_QPainterPath )
      {
         ( p )->intersects( *hbqt_par_QPainterPath( 2 ) );
      }
   }
}

/*
 * bool isEmpty () const
 */
HB_FUNC( QT_QPAINTERPATH_ISEMPTY )
{
   QPainterPath * p = hbqt_par_QPainterPath( 1 );
   if( p )
   {
      hb_retl( ( p )->isEmpty() );
   }
}

/*
 * qreal length () const
 */
HB_FUNC( QT_QPAINTERPATH_LENGTH )
{
   QPainterPath * p = hbqt_par_QPainterPath( 1 );
   if( p )
   {
      hb_retnd( ( p )->length() );
   }
}

/*
 * void lineTo ( ... )
 */
HB_FUNC( QT_QPAINTERPATH_LINETO )
{
   QPainterPath * p = hbqt_par_QPainterPath( 1 );
   if( p )
   {
      if( hb_pcount() == 2 && HB_ISPOINTER( 2 ) )
      {
         ( p )->lineTo( *hbqt_par_QPointF( 2 ) );
      }
      else if( hb_pcount() == 3 && HB_ISNUM( 2 ) )
      {
         ( p )->lineTo( hb_parnd( 2 ), hb_parnd( 3 ) );
      }
   }
}

/*
 * void moveTo ( ... )
 */
HB_FUNC( QT_QPAINTERPATH_MOVETO )
{
   QPainterPath * p = hbqt_par_QPainterPath( 1 );
   if( p )
   {
      if( hb_pcount() == 2 && HB_ISPOINTER( 2 ) )
      {
         ( p )->moveTo( *hbqt_par_QPointF( 2 ) );
      }
      else if( hb_pcount() == 3 && HB_ISNUM( 2 ) )
      {
         ( p )->moveTo( hb_parnd( 2 ), hb_parnd( 3 ) );
      }
   }
}

/*
 * qreal percentAtLength ( qreal len ) const
 */
HB_FUNC( QT_QPAINTERPATH_PERCENTATLENGTH )
{
   QPainterPath * p = hbqt_par_QPainterPath( 1 );
   if( p )
   {
      hb_retnd( ( p )->percentAtLength( hb_parnd( 2 ) ) );
   }
}

/*
 * QPointF pointAtPercent ( qreal t ) const
 */
HB_FUNC( QT_QPAINTERPATH_POINTATPERCENT )
{
   QPainterPath * p = hbqt_par_QPainterPath( 1 );
   if( p )
   {
      hb_retptrGC( hbqt_gcAllocate_QPointF( new QPointF( ( p )->pointAtPercent( hb_parnd( 2 ) ) ), true ) );
   }
}

/*
 * void quadTo ( ... )
 */
HB_FUNC( QT_QPAINTERPATH_QUADTO )
{
   QPainterPath * p = hbqt_par_QPainterPath( 1 );
   if( p )
   {
      if( hb_pcount() == 3 && HB_ISPOINTER( 2 ) )
      {
         ( p )->quadTo( *hbqt_par_QPointF( 2 ), *hbqt_par_QPointF( 3 ) );
      }
      else if( hb_pcount() == 5 && HB_ISNUM( 2 ) )
      {
         ( p )->quadTo( hb_parnd( 2 ), hb_parnd( 3 ), hb_parnd( 4 ), hb_parnd( 5 ) );
      }
   }
}

/*
 * void setElementPositionAt ( int index, qreal x, qreal y )
 */
HB_FUNC( QT_QPAINTERPATH_SETELEMENTPOSITIONAT )
{
   QPainterPath * p = hbqt_par_QPainterPath( 1 );
   if( p )
   {
      ( p )->setElementPositionAt( hb_parni( 2 ), hb_parnd( 3 ), hb_parnd( 4 ) );
   }
}

/*
 * void setFillRule ( Qt::FillRule fillRule )
 */
HB_FUNC( QT_QPAINTERPATH_SETFILLRULE )
{
   QPainterPath * p = hbqt_par_QPainterPath( 1 );
   if( p )
   {
      ( p )->setFillRule( ( Qt::FillRule ) hb_parni( 2 ) );
   }
}

/*
 * QPainterPath simplified () const
 */
HB_FUNC( QT_QPAINTERPATH_SIMPLIFIED )
{
   QPainterPath * p = hbqt_par_QPainterPath( 1 );
   if( p )
   {
      hb_retptrGC( hbqt_gcAllocate_QPainterPath( new QPainterPath( ( p )->simplified() ), true ) );
   }
}

/*
 * qreal slopeAtPercent ( qreal t ) const
 */
HB_FUNC( QT_QPAINTERPATH_SLOPEATPERCENT )
{
   QPainterPath * p = hbqt_par_QPainterPath( 1 );
   if( p )
   {
      hb_retnd( ( p )->slopeAtPercent( hb_parnd( 2 ) ) );
   }
}

/*
 * QPainterPath subtracted ( const QPainterPath & p ) const
 */
HB_FUNC( QT_QPAINTERPATH_SUBTRACTED )
{
   QPainterPath * p = hbqt_par_QPainterPath( 1 );
   if( p )
   {
      hb_retptrGC( hbqt_gcAllocate_QPainterPath( new QPainterPath( ( p )->subtracted( *hbqt_par_QPainterPath( 2 ) ) ), true ) );
   }
}

/*
 * QPolygonF toFillPolygon ( const QTransform & matrix ) const
 */
HB_FUNC( QT_QPAINTERPATH_TOFILLPOLYGON )
{
   QPainterPath * p = hbqt_par_QPainterPath( 1 );
   if( p )
   {
      hb_retptrGC( hbqt_gcAllocate_QPolygonF( new QPolygonF( ( p )->toFillPolygon( *hbqt_par_QTransform( 2 ) ) ), true ) );
   }
}

/*
 * QPolygonF toFillPolygon ( const QMatrix & matrix = QMatrix() ) const
 */
HB_FUNC( QT_QPAINTERPATH_TOFILLPOLYGON_1 )
{
   QPainterPath * p = hbqt_par_QPainterPath( 1 );
   if( p )
   {
      hb_retptrGC( hbqt_gcAllocate_QPolygonF( new QPolygonF( ( p )->toFillPolygon( ( HB_ISPOINTER( 2 ) ? *hbqt_par_QMatrix( 2 ) : QMatrix() ) ) ), true ) );
   }
}

/*
 * QList<QPolygonF> toFillPolygons ( const QTransform & matrix ) const
 */
HB_FUNC( QT_QPAINTERPATH_TOFILLPOLYGONS )
{
   QPainterPath * p = hbqt_par_QPainterPath( 1 );
   if( p )
   {
      hb_retptrGC( hbqt_gcAllocate_QList( new QList<QPolygonF>( ( p )->toFillPolygons( *hbqt_par_QTransform( 2 ) ) ), true ) );
   }
}

/*
 * QList<QPolygonF> toFillPolygons ( const QMatrix & matrix = QMatrix() ) const
 */
HB_FUNC( QT_QPAINTERPATH_TOFILLPOLYGONS_1 )
{
   QPainterPath * p = hbqt_par_QPainterPath( 1 );
   if( p )
   {
      hb_retptrGC( hbqt_gcAllocate_QList( new QList<QPolygonF>( ( p )->toFillPolygons( ( HB_ISPOINTER( 2 ) ? *hbqt_par_QMatrix( 2 ) : QMatrix() ) ) ), true ) );
   }
}

/*
 * QPainterPath toReversed () const
 */
HB_FUNC( QT_QPAINTERPATH_TOREVERSED )
{
   QPainterPath * p = hbqt_par_QPainterPath( 1 );
   if( p )
   {
      hb_retptrGC( hbqt_gcAllocate_QPainterPath( new QPainterPath( ( p )->toReversed() ), true ) );
   }
}

/*
 * QList<QPolygonF> toSubpathPolygons ( const QTransform & matrix ) const
 */
HB_FUNC( QT_QPAINTERPATH_TOSUBPATHPOLYGONS )
{
   QPainterPath * p = hbqt_par_QPainterPath( 1 );
   if( p )
   {
      hb_retptrGC( hbqt_gcAllocate_QList( new QList<QPolygonF>( ( p )->toSubpathPolygons( *hbqt_par_QTransform( 2 ) ) ), true ) );
   }
}

/*
 * QList<QPolygonF> toSubpathPolygons ( const QMatrix & matrix = QMatrix() ) const
 */
HB_FUNC( QT_QPAINTERPATH_TOSUBPATHPOLYGONS_1 )
{
   QPainterPath * p = hbqt_par_QPainterPath( 1 );
   if( p )
   {
      hb_retptrGC( hbqt_gcAllocate_QList( new QList<QPolygonF>( ( p )->toSubpathPolygons( ( HB_ISPOINTER( 2 ) ? *hbqt_par_QMatrix( 2 ) : QMatrix() ) ) ), true ) );
   }
}

/*
 * QPainterPath united ( const QPainterPath & p ) const
 */
HB_FUNC( QT_QPAINTERPATH_UNITED )
{
   QPainterPath * p = hbqt_par_QPainterPath( 1 );
   if( p )
   {
      hb_retptrGC( hbqt_gcAllocate_QPainterPath( new QPainterPath( ( p )->united( *hbqt_par_QPainterPath( 2 ) ) ), true ) );
   }
}


/*----------------------------------------------------------------------*/
#endif             /* #if QT_VERSION >= 0x040500 */
/*----------------------------------------------------------------------*/
