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

#include "hbqt.h"
#include "hbqtgui_garbage.h"
#include "hbqtgui.h"
#include "hbqtcore_garbage.h"
#include "hbqtcore.h"

/*----------------------------------------------------------------------*/
#if QT_VERSION >= 0x040500
/*----------------------------------------------------------------------*/

/*
 *  enum TransformationType { TxNone, TxTranslate, TxScale, TxRotate, TxShear, TxProject }
 */

#include <QtCore/QPointer>

#include <QtGui/QTransform>


/* QTransform ()
 * QTransform ( qreal m11, qreal m12, qreal m13, qreal m21, qreal m22, qreal m23, qreal m31, qreal m32, qreal m33 = 1.0 )
 * QTransform ( qreal m11, qreal m12, qreal m21, qreal m22, qreal dx, qreal dy )
 * QTransform ( const QMatrix & matrix )
 */

typedef struct
{
   QTransform * ph;
   bool bNew;
   QT_G_FUNC_PTR func;
   int type;
} QGC_POINTER_QTransform;

QT_G_FUNC( hbqt_gcRelease_QTransform )
{
   QGC_POINTER * p = ( QGC_POINTER * ) Cargo;

   if( p && p->bNew )
   {
      if( p->ph )
      {
         HB_TRACE( HB_TR_DEBUG, ( "ph=%p    _rel_QTransform   /.\\", p->ph ) );
         delete ( ( QTransform * ) p->ph );
         HB_TRACE( HB_TR_DEBUG, ( "ph=%p YES_rel_QTransform   \\./", p->ph ) );
         p->ph = NULL;
      }
      else
      {
         HB_TRACE( HB_TR_DEBUG, ( "ph=%p DEL_rel_QTransform    :     Object already deleted!", p->ph ) );
         p->ph = NULL;
      }
   }
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "ph=%p PTR_rel_QTransform    :    Object not created with new=true", p->ph ) );
      p->ph = NULL;
   }
}

void * hbqt_gcAllocate_QTransform( void * pObj, bool bNew )
{
   QGC_POINTER * p = ( QGC_POINTER * ) hb_gcAllocate( sizeof( QGC_POINTER ), hbqt_gcFuncs() );

   p->ph = ( QTransform * ) pObj;
   p->bNew = bNew;
   p->func = hbqt_gcRelease_QTransform;
   p->type = HBQT_TYPE_QTransform;

   if( bNew )
   {
      HB_TRACE( HB_TR_DEBUG, ( "ph=%p    _new_QTransform", pObj ) );
   }
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "ph=%p NOT_new_QTransform", pObj ) );
   }
   return p;
}

HB_FUNC( QT_QTRANSFORM )
{
   QTransform * pObj = NULL;

   pObj = new QTransform() ;

   hb_retptrGC( hbqt_gcAllocate_QTransform( ( void * ) pObj, true ) );
}

/*
 * qreal m11 () const
 */
HB_FUNC( QT_QTRANSFORM_M11 )
{
   QTransform * p = hbqt_par_QTransform( 1 );
   if( p )
      hb_retnd( ( p )->m11() );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QTRANSFORM_M11 FP=hb_retnd( ( p )->m11() ); p is NULL" ) );
   }
}

/*
 * qreal m12 () const
 */
HB_FUNC( QT_QTRANSFORM_M12 )
{
   QTransform * p = hbqt_par_QTransform( 1 );
   if( p )
      hb_retnd( ( p )->m12() );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QTRANSFORM_M12 FP=hb_retnd( ( p )->m12() ); p is NULL" ) );
   }
}

/*
 * qreal m13 () const
 */
HB_FUNC( QT_QTRANSFORM_M13 )
{
   QTransform * p = hbqt_par_QTransform( 1 );
   if( p )
      hb_retnd( ( p )->m13() );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QTRANSFORM_M13 FP=hb_retnd( ( p )->m13() ); p is NULL" ) );
   }
}

/*
 * qreal m21 () const
 */
HB_FUNC( QT_QTRANSFORM_M21 )
{
   QTransform * p = hbqt_par_QTransform( 1 );
   if( p )
      hb_retnd( ( p )->m21() );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QTRANSFORM_M21 FP=hb_retnd( ( p )->m21() ); p is NULL" ) );
   }
}

/*
 * qreal m22 () const
 */
HB_FUNC( QT_QTRANSFORM_M22 )
{
   QTransform * p = hbqt_par_QTransform( 1 );
   if( p )
      hb_retnd( ( p )->m22() );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QTRANSFORM_M22 FP=hb_retnd( ( p )->m22() ); p is NULL" ) );
   }
}

/*
 * qreal m23 () const
 */
HB_FUNC( QT_QTRANSFORM_M23 )
{
   QTransform * p = hbqt_par_QTransform( 1 );
   if( p )
      hb_retnd( ( p )->m23() );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QTRANSFORM_M23 FP=hb_retnd( ( p )->m23() ); p is NULL" ) );
   }
}

/*
 * qreal m31 () const
 */
HB_FUNC( QT_QTRANSFORM_M31 )
{
   QTransform * p = hbqt_par_QTransform( 1 );
   if( p )
      hb_retnd( ( p )->m31() );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QTRANSFORM_M31 FP=hb_retnd( ( p )->m31() ); p is NULL" ) );
   }
}

/*
 * qreal m32 () const
 */
HB_FUNC( QT_QTRANSFORM_M32 )
{
   QTransform * p = hbqt_par_QTransform( 1 );
   if( p )
      hb_retnd( ( p )->m32() );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QTRANSFORM_M32 FP=hb_retnd( ( p )->m32() ); p is NULL" ) );
   }
}

/*
 * qreal m33 () const
 */
HB_FUNC( QT_QTRANSFORM_M33 )
{
   QTransform * p = hbqt_par_QTransform( 1 );
   if( p )
      hb_retnd( ( p )->m33() );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QTRANSFORM_M33 FP=hb_retnd( ( p )->m33() ); p is NULL" ) );
   }
}

/*
 * QTransform adjoint () const
 */
HB_FUNC( QT_QTRANSFORM_ADJOINT )
{
   QTransform * p = hbqt_par_QTransform( 1 );
   if( p )
      hb_retptrGC( hbqt_gcAllocate_QTransform( new QTransform( ( p )->adjoint() ), true ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QTRANSFORM_ADJOINT FP=hb_retptrGC( hbqt_gcAllocate_QTransform( new QTransform( ( p )->adjoint() ), true ) ); p is NULL" ) );
   }
}

/*
 * qreal det () const
 */
HB_FUNC( QT_QTRANSFORM_DET )
{
   QTransform * p = hbqt_par_QTransform( 1 );
   if( p )
      hb_retnd( ( p )->det() );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QTRANSFORM_DET FP=hb_retnd( ( p )->det() ); p is NULL" ) );
   }
}

/*
 * qreal determinant () const
 */
HB_FUNC( QT_QTRANSFORM_DETERMINANT )
{
   QTransform * p = hbqt_par_QTransform( 1 );
   if( p )
      hb_retnd( ( p )->determinant() );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QTRANSFORM_DETERMINANT FP=hb_retnd( ( p )->determinant() ); p is NULL" ) );
   }
}

/*
 * qreal dx () const
 */
HB_FUNC( QT_QTRANSFORM_DX )
{
   QTransform * p = hbqt_par_QTransform( 1 );
   if( p )
      hb_retnd( ( p )->dx() );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QTRANSFORM_DX FP=hb_retnd( ( p )->dx() ); p is NULL" ) );
   }
}

/*
 * qreal dy () const
 */
HB_FUNC( QT_QTRANSFORM_DY )
{
   QTransform * p = hbqt_par_QTransform( 1 );
   if( p )
      hb_retnd( ( p )->dy() );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QTRANSFORM_DY FP=hb_retnd( ( p )->dy() ); p is NULL" ) );
   }
}

/*
 * QTransform inverted ( bool * invertible = 0 ) const
 */
HB_FUNC( QT_QTRANSFORM_INVERTED )
{
   QTransform * p = hbqt_par_QTransform( 1 );
   bool iInvertible = 0;

   if( p )
      hb_retptrGC( hbqt_gcAllocate_QTransform( new QTransform( ( p )->inverted( &iInvertible ) ), true ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QTRANSFORM_INVERTED FP=hb_retptrGC( hbqt_gcAllocate_QTransform( new QTransform( ( p )->inverted( &iInvertible ) ), true ) ); p is NULL" ) );
   }

   hb_stornl( iInvertible, 2 );
}

/*
 * bool isAffine () const
 */
HB_FUNC( QT_QTRANSFORM_ISAFFINE )
{
   QTransform * p = hbqt_par_QTransform( 1 );
   if( p )
      hb_retl( ( p )->isAffine() );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QTRANSFORM_ISAFFINE FP=hb_retl( ( p )->isAffine() ); p is NULL" ) );
   }
}

/*
 * bool isIdentity () const
 */
HB_FUNC( QT_QTRANSFORM_ISIDENTITY )
{
   QTransform * p = hbqt_par_QTransform( 1 );
   if( p )
      hb_retl( ( p )->isIdentity() );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QTRANSFORM_ISIDENTITY FP=hb_retl( ( p )->isIdentity() ); p is NULL" ) );
   }
}

/*
 * bool isInvertible () const
 */
HB_FUNC( QT_QTRANSFORM_ISINVERTIBLE )
{
   QTransform * p = hbqt_par_QTransform( 1 );
   if( p )
      hb_retl( ( p )->isInvertible() );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QTRANSFORM_ISINVERTIBLE FP=hb_retl( ( p )->isInvertible() ); p is NULL" ) );
   }
}

/*
 * bool isRotating () const
 */
HB_FUNC( QT_QTRANSFORM_ISROTATING )
{
   QTransform * p = hbqt_par_QTransform( 1 );
   if( p )
      hb_retl( ( p )->isRotating() );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QTRANSFORM_ISROTATING FP=hb_retl( ( p )->isRotating() ); p is NULL" ) );
   }
}

/*
 * bool isScaling () const
 */
HB_FUNC( QT_QTRANSFORM_ISSCALING )
{
   QTransform * p = hbqt_par_QTransform( 1 );
   if( p )
      hb_retl( ( p )->isScaling() );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QTRANSFORM_ISSCALING FP=hb_retl( ( p )->isScaling() ); p is NULL" ) );
   }
}

/*
 * bool isTranslating () const
 */
HB_FUNC( QT_QTRANSFORM_ISTRANSLATING )
{
   QTransform * p = hbqt_par_QTransform( 1 );
   if( p )
      hb_retl( ( p )->isTranslating() );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QTRANSFORM_ISTRANSLATING FP=hb_retl( ( p )->isTranslating() ); p is NULL" ) );
   }
}

/*
 * void map ( qreal x, qreal y, qreal * tx, qreal * ty ) const
 */
HB_FUNC( QT_QTRANSFORM_MAP )
{
   QTransform * p = hbqt_par_QTransform( 1 );
   qreal qrTx = 0;
   qreal qrTy = 0;

   if( p )
      ( p )->map( hb_parnd( 2 ), hb_parnd( 3 ), &qrTx, &qrTy );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QTRANSFORM_MAP FP=( p )->map( hb_parnd( 2 ), hb_parnd( 3 ), &qrTx, &qrTy ); p is NULL" ) );
   }

   hb_stornd( qrTx, 4 );
   hb_stornd( qrTy, 5 );
}

/*
 * QPointF map ( const QPointF & p ) const
 */
HB_FUNC( QT_QTRANSFORM_MAP_1 )
{
   QTransform * p = hbqt_par_QTransform( 1 );
   if( p )
      hb_retptrGC( hbqt_gcAllocate_QPointF( new QPointF( ( p )->map( *hbqt_par_QPointF( 2 ) ) ), true ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QTRANSFORM_MAP_1 FP=hb_retptrGC( hbqt_gcAllocate_QPointF( new QPointF( ( p )->map( *hbqt_par_QPointF( 2 ) ) ), true ) ); p is NULL" ) );
   }
}

/*
 * QPoint map ( const QPoint & point ) const
 */
HB_FUNC( QT_QTRANSFORM_MAP_2 )
{
   QTransform * p = hbqt_par_QTransform( 1 );
   if( p )
      hb_retptrGC( hbqt_gcAllocate_QPoint( new QPoint( ( p )->map( *hbqt_par_QPoint( 2 ) ) ), true ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QTRANSFORM_MAP_2 FP=hb_retptrGC( hbqt_gcAllocate_QPoint( new QPoint( ( p )->map( *hbqt_par_QPoint( 2 ) ) ), true ) ); p is NULL" ) );
   }
}

/*
 * QLine map ( const QLine & l ) const
 */
HB_FUNC( QT_QTRANSFORM_MAP_3 )
{
   QTransform * p = hbqt_par_QTransform( 1 );
   if( p )
      hb_retptrGC( hbqt_gcAllocate_QLine( new QLine( ( p )->map( *hbqt_par_QLine( 2 ) ) ), true ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QTRANSFORM_MAP_3 FP=hb_retptrGC( hbqt_gcAllocate_QLine( new QLine( ( p )->map( *hbqt_par_QLine( 2 ) ) ), true ) ); p is NULL" ) );
   }
}

/*
 * QLineF map ( const QLineF & line ) const
 */
HB_FUNC( QT_QTRANSFORM_MAP_4 )
{
   QTransform * p = hbqt_par_QTransform( 1 );
   if( p )
      hb_retptrGC( hbqt_gcAllocate_QLineF( new QLineF( ( p )->map( *hbqt_par_QLineF( 2 ) ) ), true ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QTRANSFORM_MAP_4 FP=hb_retptrGC( hbqt_gcAllocate_QLineF( new QLineF( ( p )->map( *hbqt_par_QLineF( 2 ) ) ), true ) ); p is NULL" ) );
   }
}

/*
 * QPolygonF map ( const QPolygonF & polygon ) const
 */
HB_FUNC( QT_QTRANSFORM_MAP_5 )
{
   QTransform * p = hbqt_par_QTransform( 1 );
   if( p )
      hb_retptrGC( hbqt_gcAllocate_QPolygonF( new QPolygonF( ( p )->map( *hbqt_par_QPolygonF( 2 ) ) ), true ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QTRANSFORM_MAP_5 FP=hb_retptrGC( hbqt_gcAllocate_QPolygonF( new QPolygonF( ( p )->map( *hbqt_par_QPolygonF( 2 ) ) ), true ) ); p is NULL" ) );
   }
}

/*
 * QPolygon map ( const QPolygon & polygon ) const
 */
HB_FUNC( QT_QTRANSFORM_MAP_6 )
{
   QTransform * p = hbqt_par_QTransform( 1 );
   if( p )
      hb_retptrGC( hbqt_gcAllocate_QPolygon( new QPolygon( ( p )->map( *hbqt_par_QPolygon( 2 ) ) ), true ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QTRANSFORM_MAP_6 FP=hb_retptrGC( hbqt_gcAllocate_QPolygon( new QPolygon( ( p )->map( *hbqt_par_QPolygon( 2 ) ) ), true ) ); p is NULL" ) );
   }
}

/*
 * QRegion map ( const QRegion & region ) const
 */
HB_FUNC( QT_QTRANSFORM_MAP_7 )
{
   QTransform * p = hbqt_par_QTransform( 1 );
   if( p )
      hb_retptrGC( hbqt_gcAllocate_QRegion( new QRegion( ( p )->map( *hbqt_par_QRegion( 2 ) ) ), true ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QTRANSFORM_MAP_7 FP=hb_retptrGC( hbqt_gcAllocate_QRegion( new QRegion( ( p )->map( *hbqt_par_QRegion( 2 ) ) ), true ) ); p is NULL" ) );
   }
}

/*
 * QPainterPath map ( const QPainterPath & path ) const
 */
HB_FUNC( QT_QTRANSFORM_MAP_8 )
{
   QTransform * p = hbqt_par_QTransform( 1 );
   if( p )
      hb_retptrGC( hbqt_gcAllocate_QPainterPath( new QPainterPath( ( p )->map( *hbqt_par_QPainterPath( 2 ) ) ), true ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QTRANSFORM_MAP_8 FP=hb_retptrGC( hbqt_gcAllocate_QPainterPath( new QPainterPath( ( p )->map( *hbqt_par_QPainterPath( 2 ) ) ), true ) ); p is NULL" ) );
   }
}

/*
 * void map ( int x, int y, int * tx, int * ty ) const
 */
HB_FUNC( QT_QTRANSFORM_MAP_9 )
{
   QTransform * p = hbqt_par_QTransform( 1 );
   int iTx = 0;
   int iTy = 0;

   if( p )
      ( p )->map( hb_parni( 2 ), hb_parni( 3 ), &iTx, &iTy );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QTRANSFORM_MAP_9 FP=( p )->map( hb_parni( 2 ), hb_parni( 3 ), &iTx, &iTy ); p is NULL" ) );
   }

   hb_storni( iTx, 4 );
   hb_storni( iTy, 5 );
}

/*
 * QRectF mapRect ( const QRectF & rectangle ) const
 */
HB_FUNC( QT_QTRANSFORM_MAPRECT )
{
   QTransform * p = hbqt_par_QTransform( 1 );
   if( p )
      hb_retptrGC( hbqt_gcAllocate_QRectF( new QRectF( ( p )->mapRect( *hbqt_par_QRectF( 2 ) ) ), true ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QTRANSFORM_MAPRECT FP=hb_retptrGC( hbqt_gcAllocate_QRectF( new QRectF( ( p )->mapRect( *hbqt_par_QRectF( 2 ) ) ), true ) ); p is NULL" ) );
   }
}

/*
 * QRect mapRect ( const QRect & rectangle ) const
 */
HB_FUNC( QT_QTRANSFORM_MAPRECT_1 )
{
   QTransform * p = hbqt_par_QTransform( 1 );
   if( p )
      hb_retptrGC( hbqt_gcAllocate_QRect( new QRect( ( p )->mapRect( *hbqt_par_QRect( 2 ) ) ), true ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QTRANSFORM_MAPRECT_1 FP=hb_retptrGC( hbqt_gcAllocate_QRect( new QRect( ( p )->mapRect( *hbqt_par_QRect( 2 ) ) ), true ) ); p is NULL" ) );
   }
}

/*
 * QPolygon mapToPolygon ( const QRect & rectangle ) const
 */
HB_FUNC( QT_QTRANSFORM_MAPTOPOLYGON )
{
   QTransform * p = hbqt_par_QTransform( 1 );
   if( p )
      hb_retptrGC( hbqt_gcAllocate_QPolygon( new QPolygon( ( p )->mapToPolygon( *hbqt_par_QRect( 2 ) ) ), true ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QTRANSFORM_MAPTOPOLYGON FP=hb_retptrGC( hbqt_gcAllocate_QPolygon( new QPolygon( ( p )->mapToPolygon( *hbqt_par_QRect( 2 ) ) ), true ) ); p is NULL" ) );
   }
}

/*
 * void reset ()
 */
HB_FUNC( QT_QTRANSFORM_RESET )
{
   QTransform * p = hbqt_par_QTransform( 1 );
   if( p )
      ( p )->reset();
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QTRANSFORM_RESET FP=( p )->reset(); p is NULL" ) );
   }
}

/*
 * QTransform & rotate ( qreal angle, Qt::Axis axis = Qt::ZAxis )
 */
HB_FUNC( QT_QTRANSFORM_ROTATE )
{
   QTransform * p = hbqt_par_QTransform( 1 );
   if( p )
      hb_retptrGC( hbqt_gcAllocate_QTransform( new QTransform( ( p )->rotate( hb_parnd( 2 ), ( HB_ISNUM( 3 ) ? ( Qt::Axis ) hb_parni( 3 ) : ( Qt::Axis ) Qt::ZAxis ) ) ), true ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QTRANSFORM_ROTATE FP=hb_retptrGC( hbqt_gcAllocate_QTransform( new QTransform( ( p )->rotate( hb_parnd( 2 ), ( HB_ISNUM( 3 ) ? ( Qt::Axis ) hb_parni( 3 ) : ( Qt::Axis ) Qt::ZAxis ) ) ), true ) ); p is NULL" ) );
   }
}

/*
 * QTransform & rotateRadians ( qreal angle, Qt::Axis axis = Qt::ZAxis )
 */
HB_FUNC( QT_QTRANSFORM_ROTATERADIANS )
{
   QTransform * p = hbqt_par_QTransform( 1 );
   if( p )
      hb_retptrGC( hbqt_gcAllocate_QTransform( new QTransform( ( p )->rotateRadians( hb_parnd( 2 ), ( HB_ISNUM( 3 ) ? ( Qt::Axis ) hb_parni( 3 ) : ( Qt::Axis ) Qt::ZAxis ) ) ), true ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QTRANSFORM_ROTATERADIANS FP=hb_retptrGC( hbqt_gcAllocate_QTransform( new QTransform( ( p )->rotateRadians( hb_parnd( 2 ), ( HB_ISNUM( 3 ) ? ( Qt::Axis ) hb_parni( 3 ) : ( Qt::Axis ) Qt::ZAxis ) ) ), true ) ); p is NULL" ) );
   }
}

/*
 * QTransform & scale ( qreal sx, qreal sy )
 */
HB_FUNC( QT_QTRANSFORM_SCALE )
{
   QTransform * p = hbqt_par_QTransform( 1 );
   if( p )
      hb_retptrGC( hbqt_gcAllocate_QTransform( new QTransform( ( p )->scale( hb_parnd( 2 ), hb_parnd( 3 ) ) ), true ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QTRANSFORM_SCALE FP=hb_retptrGC( hbqt_gcAllocate_QTransform( new QTransform( ( p )->scale( hb_parnd( 2 ), hb_parnd( 3 ) ) ), true ) ); p is NULL" ) );
   }
}

/*
 * void setMatrix ( qreal m11, qreal m12, qreal m13, qreal m21, qreal m22, qreal m23, qreal m31, qreal m32, qreal m33 )
 */
HB_FUNC( QT_QTRANSFORM_SETMATRIX )
{
   QTransform * p = hbqt_par_QTransform( 1 );
   if( p )
      ( p )->setMatrix( hb_parnd( 2 ), hb_parnd( 3 ), hb_parnd( 4 ), hb_parnd( 5 ), hb_parnd( 6 ), hb_parnd( 7 ), hb_parnd( 8 ), hb_parnd( 9 ), hb_parnd( 10 ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QTRANSFORM_SETMATRIX FP=( p )->setMatrix( hb_parnd( 2 ), hb_parnd( 3 ), hb_parnd( 4 ), hb_parnd( 5 ), hb_parnd( 6 ), hb_parnd( 7 ), hb_parnd( 8 ), hb_parnd( 9 ), hb_parnd( 10 ) ); p is NULL" ) );
   }
}

/*
 * QTransform & shear ( qreal sh, qreal sv )
 */
HB_FUNC( QT_QTRANSFORM_SHEAR )
{
   QTransform * p = hbqt_par_QTransform( 1 );
   if( p )
      hb_retptrGC( hbqt_gcAllocate_QTransform( new QTransform( ( p )->shear( hb_parnd( 2 ), hb_parnd( 3 ) ) ), true ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QTRANSFORM_SHEAR FP=hb_retptrGC( hbqt_gcAllocate_QTransform( new QTransform( ( p )->shear( hb_parnd( 2 ), hb_parnd( 3 ) ) ), true ) ); p is NULL" ) );
   }
}

/*
 * const QMatrix & toAffine () const
 */
HB_FUNC( QT_QTRANSFORM_TOAFFINE )
{
   QTransform * p = hbqt_par_QTransform( 1 );
   if( p )
      hb_retptrGC( hbqt_gcAllocate_QMatrix( new QMatrix( ( p )->toAffine() ), true ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QTRANSFORM_TOAFFINE FP=hb_retptrGC( hbqt_gcAllocate_QMatrix( new QMatrix( ( p )->toAffine() ), true ) ); p is NULL" ) );
   }
}

/*
 * QTransform & translate ( qreal dx, qreal dy )
 */
HB_FUNC( QT_QTRANSFORM_TRANSLATE )
{
   QTransform * p = hbqt_par_QTransform( 1 );
   if( p )
      hb_retptrGC( hbqt_gcAllocate_QTransform( new QTransform( ( p )->translate( hb_parnd( 2 ), hb_parnd( 3 ) ) ), true ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QTRANSFORM_TRANSLATE FP=hb_retptrGC( hbqt_gcAllocate_QTransform( new QTransform( ( p )->translate( hb_parnd( 2 ), hb_parnd( 3 ) ) ), true ) ); p is NULL" ) );
   }
}

/*
 * QTransform transposed () const
 */
HB_FUNC( QT_QTRANSFORM_TRANSPOSED )
{
   QTransform * p = hbqt_par_QTransform( 1 );
   if( p )
      hb_retptrGC( hbqt_gcAllocate_QTransform( new QTransform( ( p )->transposed() ), true ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QTRANSFORM_TRANSPOSED FP=hb_retptrGC( hbqt_gcAllocate_QTransform( new QTransform( ( p )->transposed() ), true ) ); p is NULL" ) );
   }
}

/*
 * TransformationType type () const
 */
HB_FUNC( QT_QTRANSFORM_TYPE )
{
   QTransform * p = hbqt_par_QTransform( 1 );
   if( p )
      hb_retni( ( QTransform::TransformationType ) ( p )->type() );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QTRANSFORM_TYPE FP=hb_retni( ( QTransform::TransformationType ) ( p )->type() ); p is NULL" ) );
   }
}

/*
 * QTransform fromScale ( qreal sx, qreal sy )
 */
HB_FUNC( QT_QTRANSFORM_FROMSCALE )
{
   QTransform * p = hbqt_par_QTransform( 1 );
   if( p )
      hb_retptrGC( hbqt_gcAllocate_QTransform( new QTransform( ( p )->fromScale( hb_parnd( 2 ), hb_parnd( 3 ) ) ), true ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QTRANSFORM_FROMSCALE FP=hb_retptrGC( hbqt_gcAllocate_QTransform( new QTransform( ( p )->fromScale( hb_parnd( 2 ), hb_parnd( 3 ) ) ), true ) ); p is NULL" ) );
   }
}

/*
 * QTransform fromTranslate ( qreal dx, qreal dy )
 */
HB_FUNC( QT_QTRANSFORM_FROMTRANSLATE )
{
   QTransform * p = hbqt_par_QTransform( 1 );
   if( p )
      hb_retptrGC( hbqt_gcAllocate_QTransform( new QTransform( ( p )->fromTranslate( hb_parnd( 2 ), hb_parnd( 3 ) ) ), true ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QTRANSFORM_FROMTRANSLATE FP=hb_retptrGC( hbqt_gcAllocate_QTransform( new QTransform( ( p )->fromTranslate( hb_parnd( 2 ), hb_parnd( 3 ) ) ), true ) ); p is NULL" ) );
   }
}

/*
 * bool quadToQuad ( const QPolygonF & one, const QPolygonF & two, QTransform & trans )
 */
HB_FUNC( QT_QTRANSFORM_QUADTOQUAD )
{
   QTransform * p = hbqt_par_QTransform( 1 );
   if( p )
      hb_retl( ( p )->quadToQuad( *hbqt_par_QPolygonF( 2 ), *hbqt_par_QPolygonF( 3 ), *hbqt_par_QTransform( 4 ) ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QTRANSFORM_QUADTOQUAD FP=hb_retl( ( p )->quadToQuad( *hbqt_par_QPolygonF( 2 ), *hbqt_par_QPolygonF( 3 ), *hbqt_par_QTransform( 4 ) ) ); p is NULL" ) );
   }
}

/*
 * bool quadToSquare ( const QPolygonF & quad, QTransform & trans )
 */
HB_FUNC( QT_QTRANSFORM_QUADTOSQUARE )
{
   QTransform * p = hbqt_par_QTransform( 1 );
   if( p )
      hb_retl( ( p )->quadToSquare( *hbqt_par_QPolygonF( 2 ), *hbqt_par_QTransform( 3 ) ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QTRANSFORM_QUADTOSQUARE FP=hb_retl( ( p )->quadToSquare( *hbqt_par_QPolygonF( 2 ), *hbqt_par_QTransform( 3 ) ) ); p is NULL" ) );
   }
}

/*
 * bool squareToQuad ( const QPolygonF & quad, QTransform & trans )
 */
HB_FUNC( QT_QTRANSFORM_SQUARETOQUAD )
{
   QTransform * p = hbqt_par_QTransform( 1 );
   if( p )
      hb_retl( ( p )->squareToQuad( *hbqt_par_QPolygonF( 2 ), *hbqt_par_QTransform( 3 ) ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QTRANSFORM_SQUARETOQUAD FP=hb_retl( ( p )->squareToQuad( *hbqt_par_QPolygonF( 2 ), *hbqt_par_QTransform( 3 ) ) ); p is NULL" ) );
   }
}


/*----------------------------------------------------------------------*/
#endif             /* #if QT_VERSION >= 0x040500 */
/*----------------------------------------------------------------------*/
