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
 * Copyright 2009-2010 Pritpal Bedi <bedipritpal@hotmail.com>
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
/*                            C R E D I T S                             */
/*----------------------------------------------------------------------*/
/*
 * Marcos Antonio Gambeta
 *    for providing first ever prototype parsing methods. Though the current
 *    implementation is diametrically different then what he proposed, still
 *    current code shaped on those footsteps.
 *
 * Viktor Szakats
 *    for directing the project with futuristic vision;
 *    for designing and maintaining a complex build system for hbQT, hbIDE;
 *    for introducing many constructs on PRG and C++ levels;
 *    for streamlining signal/slots and events management classes;
 *
 * Istvan Bisz
 *    for introducing QPointer<> concept in the generator;
 *    for testing the library on numerous accounts;
 *    for showing a way how a GC pointer can be detached;
 *
 * Francesco Perillo
 *    for taking keen interest in hbQT development and peeking the code;
 *    for providing tips here and there to improve the code quality;
 *    for hitting bulls eye to describe why few objects need GC detachment;
 *
 * Carlos Bacco
 *    for implementing HBQT_TYPE_Q*Class enums;
 *    for peeking into the code and suggesting optimization points;
 *
 * Przemyslaw Czerpak
 *    for providing tips and trick to manipulate HVM internals to the best
 *    of its use and always showing a path when we get stuck;
 *    A true tradition of a MASTER...
*/
/*----------------------------------------------------------------------*/

#include "hbqtcore.h"
#include "hbqtgui.h"

/*----------------------------------------------------------------------*/
#if QT_VERSION >= 0x040500
/*----------------------------------------------------------------------*/

/*
 *  enum Mode { Normal, Disabled, Active, Selected }
 *  enum State { Off, On }
 */

/*
 *  Constructed[ 10/10 [ 100.00% ] ]
 *
 *
 *  *** Commented out protostypes ***
 *
 *  //QPixmap pixmap ( int extent, Mode mode = Normal, State state = Off ) const        // Not Implemented
 */

#include <QtCore/QPointer>

#include <QtGui/QIcon>


/*
 * QIcon ()
 * QIcon ( const QPixmap & pixmap )
 * QIcon ( const QIcon & other )
 * QIcon ( const QString & fileName )
 * QIcon ( QIconEngine * engine )
 * QIcon ( QIconEngineV2 * engine )
 * ~QIcon ()
 */

typedef struct
{
   QIcon * ph;
   bool bNew;
   PHBQT_GC_FUNC func;
   int type;
} HBQT_GC_T_QIcon;

HBQT_GC_FUNC( hbqt_gcRelease_QIcon )
{
   HBQT_GC_T * p = ( HBQT_GC_T * ) Cargo;

   if( p && p->bNew )
   {
      if( p->ph )
      {
         HB_TRACE( HB_TR_DEBUG, ( "ph=%p    _rel_QIcon   /.\\", p->ph ) );
         delete ( ( QIcon * ) p->ph );
         HB_TRACE( HB_TR_DEBUG, ( "ph=%p YES_rel_QIcon   \\./", p->ph ) );
         p->ph = NULL;
      }
      else
      {
         HB_TRACE( HB_TR_DEBUG, ( "ph=%p DEL_rel_QIcon    :     Object already deleted!", p->ph ) );
         p->ph = NULL;
      }
   }
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "ph=%p PTR_rel_QIcon    :    Object not created with new=true", p->ph ) );
      p->ph = NULL;
   }
}

void * hbqt_gcAllocate_QIcon( void * pObj, bool bNew )
{
   HBQT_GC_T * p = ( HBQT_GC_T * ) hb_gcAllocate( sizeof( HBQT_GC_T ), hbqt_gcFuncs() );

   p->ph = ( QIcon * ) pObj;
   p->bNew = bNew;
   p->func = hbqt_gcRelease_QIcon;
   p->type = HBQT_TYPE_QIcon;

   if( bNew )
   {
      HB_TRACE( HB_TR_DEBUG, ( "ph=%p    _new_QIcon", pObj ) );
   }
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "ph=%p NOT_new_QIcon", pObj ) );
   }
   return p;
}

HB_FUNC( QT_QICON )
{
   QIcon * pObj = NULL;

   if( hb_pcount() == 1 && HB_ISCHAR( 1 ) )
   {
      pObj = new QIcon( hbqt_par_QString( 1 ) ) ;
   }
   else if( hb_pcount() == 1 && HB_ISPOINTER( 1 ) )
   {
      HBQT_GC_T * q = ( HBQT_GC_T * ) hb_parptrGC( hbqt_gcFuncs(), 1 );
      if( q )
      {
         if( q->type == HBQT_TYPE_QIcon )
         {
            pObj = new QIcon( *hbqt_par_QIcon( 1 ) ) ;
         }
         else if( q->type == HBQT_TYPE_QPixmap )
         {
            pObj = new QIcon( *hbqt_par_QPixmap( 1 ) ) ;
         }
      }
      else
      {
         pObj = new QIcon( *hbqt_par_QIcon( 1 ) ) ;
      }
   }
   else
   {
      pObj = new QIcon() ;
   }

   hb_retptrGC( hbqt_gcAllocate_QIcon( ( void * ) pObj, true ) );
}

/*
 * QSize actualSize ( const QSize & size, Mode mode = Normal, State state = Off ) const
 */
HB_FUNC( QT_QICON_ACTUALSIZE )
{
   QIcon * p = hbqt_par_QIcon( 1 );
   if( p )
   {
      hb_retptrGC( hbqt_gcAllocate_QSize( new QSize( ( p )->actualSize( *hbqt_par_QSize( 2 ), ( HB_ISNUM( 3 ) ? ( QIcon::Mode ) hb_parni( 3 ) : ( QIcon::Mode ) QIcon::Normal ), ( HB_ISNUM( 4 ) ? ( QIcon::State ) hb_parni( 4 ) : ( QIcon::State ) QIcon::Off ) ) ), true ) );
   }
}

/*
 * void addFile ( const QString & fileName, const QSize & size = QSize(), Mode mode = Normal, State state = Off )
 */
HB_FUNC( QT_QICON_ADDFILE )
{
   QIcon * p = hbqt_par_QIcon( 1 );
   if( p )
   {
      void * pText;
      ( p )->addFile( hb_parstr_utf8( 2, &pText, NULL ), ( HB_ISPOINTER( 3 ) ? *hbqt_par_QSize( 3 ) : QSize() ), ( HB_ISNUM( 4 ) ? ( QIcon::Mode ) hb_parni( 4 ) : ( QIcon::Mode ) QIcon::Normal ), ( HB_ISNUM( 5 ) ? ( QIcon::State ) hb_parni( 5 ) : ( QIcon::State ) QIcon::Off ) );
      hb_strfree( pText );
   }
}

/*
 * void addPixmap ( const QPixmap & pixmap, Mode mode = Normal, State state = Off )
 */
HB_FUNC( QT_QICON_ADDPIXMAP )
{
   QIcon * p = hbqt_par_QIcon( 1 );
   if( p )
   {
      ( p )->addPixmap( *hbqt_par_QPixmap( 2 ), ( HB_ISNUM( 3 ) ? ( QIcon::Mode ) hb_parni( 3 ) : ( QIcon::Mode ) QIcon::Normal ), ( HB_ISNUM( 4 ) ? ( QIcon::State ) hb_parni( 4 ) : ( QIcon::State ) QIcon::Off ) );
   }
}

/*
 * QList<QSize> availableSizes ( Mode mode = Normal, State state = Off ) const
 */
HB_FUNC( QT_QICON_AVAILABLESIZES )
{
   QIcon * p = hbqt_par_QIcon( 1 );
   if( p )
   {
      hb_retptrGC( hbqt_gcAllocate_QList( new QList<QSize>( ( p )->availableSizes( ( HB_ISNUM( 2 ) ? ( QIcon::Mode ) hb_parni( 2 ) : ( QIcon::Mode ) QIcon::Normal ), ( HB_ISNUM( 3 ) ? ( QIcon::State ) hb_parni( 3 ) : ( QIcon::State ) QIcon::Off ) ) ), true ) );
   }
}

/*
 * qint64 cacheKey () const
 */
HB_FUNC( QT_QICON_CACHEKEY )
{
   QIcon * p = hbqt_par_QIcon( 1 );
   if( p )
   {
      hb_retnint( ( p )->cacheKey() );
   }
}

/*
 * bool isNull () const
 */
HB_FUNC( QT_QICON_ISNULL )
{
   QIcon * p = hbqt_par_QIcon( 1 );
   if( p )
   {
      hb_retl( ( p )->isNull() );
   }
}

/*
 * void paint ( QPainter * painter, const QRect & rect, Qt::Alignment alignment = Qt::AlignCenter, Mode mode = Normal, State state = Off ) const
 */
HB_FUNC( QT_QICON_PAINT )
{
   QIcon * p = hbqt_par_QIcon( 1 );
   if( p )
   {
      ( p )->paint( hbqt_par_QPainter( 2 ), *hbqt_par_QRect( 3 ), ( HB_ISNUM( 4 ) ? ( Qt::Alignment ) hb_parni( 4 ) : ( Qt::Alignment ) Qt::AlignCenter ), ( HB_ISNUM( 5 ) ? ( QIcon::Mode ) hb_parni( 5 ) : ( QIcon::Mode ) QIcon::Normal ), ( HB_ISNUM( 6 ) ? ( QIcon::State ) hb_parni( 6 ) : ( QIcon::State ) QIcon::Off ) );
   }
}

/*
 * void paint ( QPainter * painter, int x, int y, int w, int h, Qt::Alignment alignment = Qt::AlignCenter, Mode mode = Normal, State state = Off ) const
 */
HB_FUNC( QT_QICON_PAINT_1 )
{
   QIcon * p = hbqt_par_QIcon( 1 );
   if( p )
   {
      ( p )->paint( hbqt_par_QPainter( 2 ), hb_parni( 3 ), hb_parni( 4 ), hb_parni( 5 ), hb_parni( 6 ), ( HB_ISNUM( 7 ) ? ( Qt::Alignment ) hb_parni( 7 ) : ( Qt::Alignment ) Qt::AlignCenter ), ( HB_ISNUM( 8 ) ? ( QIcon::Mode ) hb_parni( 8 ) : ( QIcon::Mode ) QIcon::Normal ), ( HB_ISNUM( 9 ) ? ( QIcon::State ) hb_parni( 9 ) : ( QIcon::State ) QIcon::Off ) );
   }
}

/*
 * QPixmap pixmap ( const QSize & size, Mode mode = Normal, State state = Off ) const
 */
HB_FUNC( QT_QICON_PIXMAP )
{
   QIcon * p = hbqt_par_QIcon( 1 );
   if( p )
   {
      hb_retptrGC( hbqt_gcAllocate_QPixmap( new QPixmap( ( p )->pixmap( *hbqt_par_QSize( 2 ), ( HB_ISNUM( 3 ) ? ( QIcon::Mode ) hb_parni( 3 ) : ( QIcon::Mode ) QIcon::Normal ), ( HB_ISNUM( 4 ) ? ( QIcon::State ) hb_parni( 4 ) : ( QIcon::State ) QIcon::Off ) ) ), true ) );
   }
}

/*
 * QPixmap pixmap ( int w, int h, Mode mode = Normal, State state = Off ) const
 */
HB_FUNC( QT_QICON_PIXMAP_1 )
{
   QIcon * p = hbqt_par_QIcon( 1 );
   if( p )
   {
      hb_retptrGC( hbqt_gcAllocate_QPixmap( new QPixmap( ( p )->pixmap( hb_parni( 2 ), hb_parni( 3 ), ( HB_ISNUM( 4 ) ? ( QIcon::Mode ) hb_parni( 4 ) : ( QIcon::Mode ) QIcon::Normal ), ( HB_ISNUM( 5 ) ? ( QIcon::State ) hb_parni( 5 ) : ( QIcon::State ) QIcon::Off ) ) ), true ) );
   }
}


/*----------------------------------------------------------------------*/
#endif             /* #if QT_VERSION >= 0x040500 */
/*----------------------------------------------------------------------*/
