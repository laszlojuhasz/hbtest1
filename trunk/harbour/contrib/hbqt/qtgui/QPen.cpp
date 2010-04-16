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
 *  Constructed[ 21/23 [ 91.30% ] ]
 *
 *  *** Unconvered Prototypes ***
 *  -----------------------------
 *
 *  QVector<qreal> dashPattern () const
 *  void setDashPattern ( const QVector<qreal> & pattern )
 */

#include <QtCore/QPointer>

#include <QtGui/QPen>


/*
 * QPen ()
 * QPen ( Qt::PenStyle style )
 * QPen ( const QColor & color )
 * QPen ( const QBrush & brush, qreal width, Qt::PenStyle style = Qt::SolidLine, Qt::PenCapStyle cap = Qt::SquareCap, Qt::PenJoinStyle join = Qt::BevelJoin )
 * QPen ( const QPen & pen )
 * ~QPen ()
 */

typedef struct
{
   QPen * ph;
   bool bNew;
   QT_G_FUNC_PTR func;
} QGC_POINTER_QPen;

QT_G_FUNC( hbqt_gcRelease_QPen )
{
   QGC_POINTER * p = ( QGC_POINTER * ) Cargo;

   if( p && p->bNew )
   {
      if( p->ph )
      {
         HB_TRACE( HB_TR_DEBUG, ( "ph=%p    _rel_QPen   /.\\", p->ph ) );
         delete ( ( QPen * ) p->ph );
         HB_TRACE( HB_TR_DEBUG, ( "ph=%p YES_rel_QPen   \\./", p->ph ) );
         p->ph = NULL;
      }
      else
      {
         HB_TRACE( HB_TR_DEBUG, ( "ph=%p DEL_rel_QPen    :     Object already deleted!", p->ph ) );
         p->ph = NULL;
      }
   }
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "ph=%p PTR_rel_QPen    :    Object not created with new=true", p->ph ) );
      p->ph = NULL;
   }
}

void * hbqt_gcAllocate_QPen( void * pObj, bool bNew )
{
   QGC_POINTER * p = ( QGC_POINTER * ) hb_gcAllocate( sizeof( QGC_POINTER ), hbqt_gcFuncs() );

   p->ph = ( QPen * ) pObj;
   p->bNew = bNew;
   p->func = hbqt_gcRelease_QPen;

   if( bNew )
   {
      HB_TRACE( HB_TR_DEBUG, ( "ph=%p    _new_QPen", pObj ) );
   }
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "ph=%p NOT_new_QPen", pObj ) );
   }
   return p;
}

HB_FUNC( QT_QPEN )
{
   QPen * pObj = NULL;

   if( hb_pcount() == 1 && HB_ISNUM( 1 ) )
   {
      pObj =  new QPen( ( Qt::PenStyle ) hb_parni( 1 ) ) ;
   }
   else if( hb_pcount() == 1 && HB_ISPOINTER( 1 ) )
   {
      pObj =  new QPen( *hbqt_par_QPen( 1 ) ) ;
   }
   else if( hb_pcount() == 2 && HB_ISCHAR( 1 ) && HB_ISPOINTER( 2 ) )
   {
      QString objName = ( QString ) hbqt_par_QString( 1 );

      if( objName == ( QString ) "QColor" )
      {
         pObj =  new QPen( *hbqt_par_QColor( 2 ) ) ;
      }
      else
      {
         pObj =  new QPen() ;
      }
   }
   else if( hb_pcount() >= 2 && HB_ISPOINTER( 1 ) && HB_ISNUM( 2 ) )
   {
      Qt::PenStyle iStyle = HB_ISNUM( 3 ) ? ( Qt::PenStyle ) hb_parni( 3 ) : Qt::SolidLine;
      Qt::PenCapStyle iCap = HB_ISNUM( 4 ) ? ( Qt::PenCapStyle ) hb_parni( 4 ) : Qt::SquareCap;
      Qt::PenJoinStyle iJoin = HB_ISNUM( 5 ) ? ( Qt::PenJoinStyle ) hb_parni( 5 ) : Qt::BevelJoin;

      pObj =  new QPen( *hbqt_par_QBrush( 1 ), hb_parnd( 2 ), iStyle, iCap, iJoin ) ;
   }
   else
   {
      pObj =  new QPen() ;
   }

   hb_retptrGC( hbqt_gcAllocate_QPen( ( void * ) pObj, true ) );
}

/*
 * QBrush brush () const
 */
HB_FUNC( QT_QPEN_BRUSH )
{
   QPen * p = hbqt_par_QPen( 1 );
   if( p )
      hb_retptrGC( hbqt_gcAllocate_QBrush( new QBrush( ( p )->brush() ), true ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QPEN_BRUSH FP=hb_retptrGC( hbqt_gcAllocate_QBrush( new QBrush( ( p )->brush() ), true ) ); p is NULL" ) );
   }
}

/*
 * Qt::PenCapStyle capStyle () const
 */
HB_FUNC( QT_QPEN_CAPSTYLE )
{
   QPen * p = hbqt_par_QPen( 1 );
   if( p )
      hb_retni( ( Qt::PenCapStyle ) ( p )->capStyle() );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QPEN_CAPSTYLE FP=hb_retni( ( Qt::PenCapStyle ) ( p )->capStyle() ); p is NULL" ) );
   }
}

/*
 * QColor color () const
 */
HB_FUNC( QT_QPEN_COLOR )
{
   QPen * p = hbqt_par_QPen( 1 );
   if( p )
      hb_retptrGC( hbqt_gcAllocate_QColor( new QColor( ( p )->color() ), true ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QPEN_COLOR FP=hb_retptrGC( hbqt_gcAllocate_QColor( new QColor( ( p )->color() ), true ) ); p is NULL" ) );
   }
}

/*
 * qreal dashOffset () const
 */
HB_FUNC( QT_QPEN_DASHOFFSET )
{
   QPen * p = hbqt_par_QPen( 1 );
   if( p )
      hb_retnd( ( p )->dashOffset() );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QPEN_DASHOFFSET FP=hb_retnd( ( p )->dashOffset() ); p is NULL" ) );
   }
}

/*
 * bool isCosmetic () const
 */
HB_FUNC( QT_QPEN_ISCOSMETIC )
{
   QPen * p = hbqt_par_QPen( 1 );
   if( p )
      hb_retl( ( p )->isCosmetic() );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QPEN_ISCOSMETIC FP=hb_retl( ( p )->isCosmetic() ); p is NULL" ) );
   }
}

/*
 * bool isSolid () const
 */
HB_FUNC( QT_QPEN_ISSOLID )
{
   QPen * p = hbqt_par_QPen( 1 );
   if( p )
      hb_retl( ( p )->isSolid() );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QPEN_ISSOLID FP=hb_retl( ( p )->isSolid() ); p is NULL" ) );
   }
}

/*
 * Qt::PenJoinStyle joinStyle () const
 */
HB_FUNC( QT_QPEN_JOINSTYLE )
{
   QPen * p = hbqt_par_QPen( 1 );
   if( p )
      hb_retni( ( Qt::PenJoinStyle ) ( p )->joinStyle() );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QPEN_JOINSTYLE FP=hb_retni( ( Qt::PenJoinStyle ) ( p )->joinStyle() ); p is NULL" ) );
   }
}

/*
 * qreal miterLimit () const
 */
HB_FUNC( QT_QPEN_MITERLIMIT )
{
   QPen * p = hbqt_par_QPen( 1 );
   if( p )
      hb_retnd( ( p )->miterLimit() );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QPEN_MITERLIMIT FP=hb_retnd( ( p )->miterLimit() ); p is NULL" ) );
   }
}

/*
 * void setBrush ( const QBrush & brush )
 */
HB_FUNC( QT_QPEN_SETBRUSH )
{
   QPen * p = hbqt_par_QPen( 1 );
   if( p )
      ( p )->setBrush( *hbqt_par_QBrush( 2 ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QPEN_SETBRUSH FP=( p )->setBrush( *hbqt_par_QBrush( 2 ) ); p is NULL" ) );
   }
}

/*
 * void setCapStyle ( Qt::PenCapStyle style )
 */
HB_FUNC( QT_QPEN_SETCAPSTYLE )
{
   QPen * p = hbqt_par_QPen( 1 );
   if( p )
      ( p )->setCapStyle( ( Qt::PenCapStyle ) hb_parni( 2 ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QPEN_SETCAPSTYLE FP=( p )->setCapStyle( ( Qt::PenCapStyle ) hb_parni( 2 ) ); p is NULL" ) );
   }
}

/*
 * void setColor ( const QColor & color )
 */
HB_FUNC( QT_QPEN_SETCOLOR )
{
   QPen * p = hbqt_par_QPen( 1 );
   if( p )
      ( p )->setColor( *hbqt_par_QColor( 2 ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QPEN_SETCOLOR FP=( p )->setColor( *hbqt_par_QColor( 2 ) ); p is NULL" ) );
   }
}

/*
 * void setCosmetic ( bool cosmetic )
 */
HB_FUNC( QT_QPEN_SETCOSMETIC )
{
   QPen * p = hbqt_par_QPen( 1 );
   if( p )
      ( p )->setCosmetic( hb_parl( 2 ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QPEN_SETCOSMETIC FP=( p )->setCosmetic( hb_parl( 2 ) ); p is NULL" ) );
   }
}

/*
 * void setDashOffset ( qreal offset )
 */
HB_FUNC( QT_QPEN_SETDASHOFFSET )
{
   QPen * p = hbqt_par_QPen( 1 );
   if( p )
      ( p )->setDashOffset( hb_parnd( 2 ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QPEN_SETDASHOFFSET FP=( p )->setDashOffset( hb_parnd( 2 ) ); p is NULL" ) );
   }
}

/*
 * void setJoinStyle ( Qt::PenJoinStyle style )
 */
HB_FUNC( QT_QPEN_SETJOINSTYLE )
{
   QPen * p = hbqt_par_QPen( 1 );
   if( p )
      ( p )->setJoinStyle( ( Qt::PenJoinStyle ) hb_parni( 2 ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QPEN_SETJOINSTYLE FP=( p )->setJoinStyle( ( Qt::PenJoinStyle ) hb_parni( 2 ) ); p is NULL" ) );
   }
}

/*
 * void setMiterLimit ( qreal limit )
 */
HB_FUNC( QT_QPEN_SETMITERLIMIT )
{
   QPen * p = hbqt_par_QPen( 1 );
   if( p )
      ( p )->setMiterLimit( hb_parnd( 2 ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QPEN_SETMITERLIMIT FP=( p )->setMiterLimit( hb_parnd( 2 ) ); p is NULL" ) );
   }
}

/*
 * void setStyle ( Qt::PenStyle style )
 */
HB_FUNC( QT_QPEN_SETSTYLE )
{
   QPen * p = hbqt_par_QPen( 1 );
   if( p )
      ( p )->setStyle( ( Qt::PenStyle ) hb_parni( 2 ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QPEN_SETSTYLE FP=( p )->setStyle( ( Qt::PenStyle ) hb_parni( 2 ) ); p is NULL" ) );
   }
}

/*
 * void setWidth ( int width )
 */
HB_FUNC( QT_QPEN_SETWIDTH )
{
   QPen * p = hbqt_par_QPen( 1 );
   if( p )
      ( p )->setWidth( hb_parni( 2 ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QPEN_SETWIDTH FP=( p )->setWidth( hb_parni( 2 ) ); p is NULL" ) );
   }
}

/*
 * void setWidthF ( qreal width )
 */
HB_FUNC( QT_QPEN_SETWIDTHF )
{
   QPen * p = hbqt_par_QPen( 1 );
   if( p )
      ( p )->setWidthF( hb_parnd( 2 ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QPEN_SETWIDTHF FP=( p )->setWidthF( hb_parnd( 2 ) ); p is NULL" ) );
   }
}

/*
 * Qt::PenStyle style () const
 */
HB_FUNC( QT_QPEN_STYLE )
{
   QPen * p = hbqt_par_QPen( 1 );
   if( p )
      hb_retni( ( Qt::PenStyle ) ( p )->style() );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QPEN_STYLE FP=hb_retni( ( Qt::PenStyle ) ( p )->style() ); p is NULL" ) );
   }
}

/*
 * int width () const
 */
HB_FUNC( QT_QPEN_WIDTH )
{
   QPen * p = hbqt_par_QPen( 1 );
   if( p )
      hb_retni( ( p )->width() );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QPEN_WIDTH FP=hb_retni( ( p )->width() ); p is NULL" ) );
   }
}

/*
 * qreal widthF () const
 */
HB_FUNC( QT_QPEN_WIDTHF )
{
   QPen * p = hbqt_par_QPen( 1 );
   if( p )
      hb_retnd( ( p )->widthF() );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QPEN_WIDTHF FP=hb_retnd( ( p )->widthF() ); p is NULL" ) );
   }
}


/*----------------------------------------------------------------------*/
#endif             /* #if QT_VERSION >= 0x040500 */
/*----------------------------------------------------------------------*/
