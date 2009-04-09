/*
 * $Id$
 */

/*
 * Harbour Project source code:
 * QT wrapper main header
 *
 * Copyright 2009 Marcos Antonio Gambeta <marcosgambeta at gmail dot com>
 * Copyright 2009 Pritpal Bedi <pritpal@vouchcac.com>
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
#include "hbqt.h"

/*----------------------------------------------------------------------*/
#if QT_VERSION >= 0x040500
/*----------------------------------------------------------------------*/

/*
 *  Constructed[ 18/19 [ 94.74% ] ]
 *
 *  *** Unconvered Prototypes ***
 *  -----------------------------
 *
 *  QVector<QRect> rects () const
 */


#include <QtGui/QRegion>


/*
 * QRegion ()
 * QRegion ( int x, int y, int w, int h, RegionType t = Rectangle )
 * QRegion ( const QPolygon & a, Qt::FillRule fillRule = Qt::OddEvenFill )
 * QRegion ( const QRegion & r )
 * QRegion ( const QBitmap & bm )
 * QRegion ( const QRect & r, RegionType t = Rectangle )
 */
HB_FUNC( QT_QREGION )
{
   hb_retptr( ( QRegion* ) new QRegion( hb_parni( 1 ), hb_parni( 2 ),
                                        hb_parni( 3 ), hb_parni( 4 ),
      ( QRegion::RegionType ) ( HB_ISNUM( 5 ) ? hb_parni( 5 ) : QRegion::Rectangle ) ) );
}

/*
 * QRect boundingRect () const
 */
HB_FUNC( QT_QREGION_BOUNDINGRECT )
{
   hb_retptr( new QRect( hbqt_par_QRegion( 1 )->boundingRect() ) );
}

/*
 * bool contains ( const QPoint & p ) const
 */
HB_FUNC( QT_QREGION_CONTAINS )
{
   hb_retl( hbqt_par_QRegion( 1 )->contains( *hbqt_par_QPoint( 2 ) ) );
}

/*
 * bool contains ( const QRect & r ) const
 */
HB_FUNC( QT_QREGION_CONTAINS_1 )
{
   hb_retl( hbqt_par_QRegion( 1 )->contains( *hbqt_par_QRect( 2 ) ) );
}

/*
 * QRegion intersected ( const QRegion & r ) const
 */
HB_FUNC( QT_QREGION_INTERSECTED )
{
   hb_retptr( new QRegion( hbqt_par_QRegion( 1 )->intersected( *hbqt_par_QRegion( 2 ) ) ) );
}

/*
 * QRegion intersected ( const QRect & rect ) const
 */
HB_FUNC( QT_QREGION_INTERSECTED_1 )
{
   hb_retptr( new QRegion( hbqt_par_QRegion( 1 )->intersected( *hbqt_par_QRect( 2 ) ) ) );
}

/*
 * bool intersects ( const QRegion & region ) const
 */
HB_FUNC( QT_QREGION_INTERSECTS )
{
   hb_retl( hbqt_par_QRegion( 1 )->intersects( *hbqt_par_QRegion( 2 ) ) );
}

/*
 * bool intersects ( const QRect & rect ) const
 */
HB_FUNC( QT_QREGION_INTERSECTS_1 )
{
   hb_retl( hbqt_par_QRegion( 1 )->intersects( *hbqt_par_QRect( 2 ) ) );
}

/*
 * bool isEmpty () const
 */
HB_FUNC( QT_QREGION_ISEMPTY )
{
   hb_retl( hbqt_par_QRegion( 1 )->isEmpty() );
}

/*
 * int numRects () const
 */
HB_FUNC( QT_QREGION_NUMRECTS )
{
   hb_retni( hbqt_par_QRegion( 1 )->numRects() );
}

/*
 * void setRects ( const QRect * rects, int number )
 */
HB_FUNC( QT_QREGION_SETRECTS )
{
   hbqt_par_QRegion( 1 )->setRects( hbqt_par_QRect( 2 ), hb_parni( 3 ) );
}

/*
 * QRegion subtracted ( const QRegion & r ) const
 */
HB_FUNC( QT_QREGION_SUBTRACTED )
{
   hb_retptr( new QRegion( hbqt_par_QRegion( 1 )->subtracted( *hbqt_par_QRegion( 2 ) ) ) );
}

/*
 * void translate ( int dx, int dy )
 */
HB_FUNC( QT_QREGION_TRANSLATE )
{
   hbqt_par_QRegion( 1 )->translate( hb_parni( 2 ), hb_parni( 3 ) );
}

/*
 * void translate ( const QPoint & point )
 */
HB_FUNC( QT_QREGION_TRANSLATE_1 )
{
   hbqt_par_QRegion( 1 )->translate( *hbqt_par_QPoint( 2 ) );
}

/*
 * QRegion translated ( int dx, int dy ) const
 */
HB_FUNC( QT_QREGION_TRANSLATED )
{
   hb_retptr( new QRegion( hbqt_par_QRegion( 1 )->translated( hb_parni( 2 ), hb_parni( 3 ) ) ) );
}

/*
 * QRegion translated ( const QPoint & p ) const
 */
HB_FUNC( QT_QREGION_TRANSLATED_1 )
{
   hb_retptr( new QRegion( hbqt_par_QRegion( 1 )->translated( *hbqt_par_QPoint( 2 ) ) ) );
}

/*
 * QRegion united ( const QRegion & r ) const
 */
HB_FUNC( QT_QREGION_UNITED )
{
   hb_retptr( new QRegion( hbqt_par_QRegion( 1 )->united( *hbqt_par_QRegion( 2 ) ) ) );
}

/*
 * QRegion united ( const QRect & rect ) const
 */
HB_FUNC( QT_QREGION_UNITED_1 )
{
   hb_retptr( new QRegion( hbqt_par_QRegion( 1 )->united( *hbqt_par_QRect( 2 ) ) ) );
}

/*
 * QRegion xored ( const QRegion & r ) const
 */
HB_FUNC( QT_QREGION_XORED )
{
   hb_retptr( new QRegion( hbqt_par_QRegion( 1 )->xored( *hbqt_par_QRegion( 2 ) ) ) );
}


/*----------------------------------------------------------------------*/
#endif             /* #if QT_VERSION >= 0x040500 */
/*----------------------------------------------------------------------*/

