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
 *  enum FormatType { InvalidFormat, BlockFormat, CharFormat, ListFormat, ..., UserFormat }
 *  enum ObjectTypes { NoObject, ImageObject, TableObject, TableCellObject, UserObject }
 *  enum PageBreakFlag { PageBreak_Auto, PageBreak_AlwaysBefore, PageBreak_AlwaysAfter }
 *  flags PageBreakFlags
 *  enum Property { ObjectIndex, CssFloat, LayoutDirection, OutlinePen, ..., UserProperty }
 */

/*
 *  Constructed[ 41/44 [ 93.18% ] ]
 *
 *  *** Unconvered Prototypes ***
 *
 *  QVector<QTextLength> lengthVectorProperty ( int propertyId ) const
 *  QMap<int, QVariant> properties () const
 *  void setProperty ( int propertyId, const QVector<QTextLength> & value )
 *
 *  *** Commented out protostypes ***
 *
 *  // QTextTableCellFormat toTableCellFormat () const
 */

#include <QtCore/QPointer>

#include <QtGui/QTextFormat>


/*
 * QTextFormat ()
 * QTextFormat ( int type )
 * QTextFormat ( const QTextFormat & other )
 * ~QTextFormat ()
 */

typedef struct
{
   QTextFormat * ph;
   bool bNew;
   PHBQT_GC_FUNC func;
   int type;
} HBQT_GC_T_QTextFormat;

HBQT_GC_FUNC( hbqt_gcRelease_QTextFormat )
{
   HBQT_GC_T * p = ( HBQT_GC_T * ) Cargo;

   if( p && p->bNew )
   {
      if( p->ph )
      {
         HB_TRACE( HB_TR_DEBUG, ( "ph=%p    _rel_QTextFormat   /.\\", p->ph ) );
         delete ( ( QTextFormat * ) p->ph );
         HB_TRACE( HB_TR_DEBUG, ( "ph=%p YES_rel_QTextFormat   \\./", p->ph ) );
         p->ph = NULL;
      }
      else
      {
         HB_TRACE( HB_TR_DEBUG, ( "ph=%p DEL_rel_QTextFormat    :     Object already deleted!", p->ph ) );
         p->ph = NULL;
      }
   }
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "ph=%p PTR_rel_QTextFormat    :    Object not created with new=true", p->ph ) );
      p->ph = NULL;
   }
}

void * hbqt_gcAllocate_QTextFormat( void * pObj, bool bNew )
{
   HBQT_GC_T * p = ( HBQT_GC_T * ) hb_gcAllocate( sizeof( HBQT_GC_T ), hbqt_gcFuncs() );

   p->ph = ( QTextFormat * ) pObj;
   p->bNew = bNew;
   p->func = hbqt_gcRelease_QTextFormat;
   p->type = HBQT_TYPE_QTextFormat;

   if( bNew )
   {
      HB_TRACE( HB_TR_DEBUG, ( "ph=%p    _new_QTextFormat", pObj ) );
   }
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "ph=%p NOT_new_QTextFormat", pObj ) );
   }
   return p;
}

HB_FUNC( QT_QTEXTFORMAT )
{
   QTextFormat * pObj = NULL;

   pObj = new QTextFormat() ;

   hb_retptrGC( hbqt_gcAllocate_QTextFormat( ( void * ) pObj, true ) );
}

/*
 * QBrush background () const
 */
HB_FUNC( QT_QTEXTFORMAT_BACKGROUND )
{
   QTextFormat * p = hbqt_par_QTextFormat( 1 );
   if( p )
   {
      hb_retptrGC( hbqt_gcAllocate_QBrush( new QBrush( ( p )->background() ), true ) );
   }
}

/*
 * bool boolProperty ( int propertyId ) const
 */
HB_FUNC( QT_QTEXTFORMAT_BOOLPROPERTY )
{
   QTextFormat * p = hbqt_par_QTextFormat( 1 );
   if( p )
   {
      hb_retl( ( p )->boolProperty( hb_parni( 2 ) ) );
   }
}

/*
 * QBrush brushProperty ( int propertyId ) const
 */
HB_FUNC( QT_QTEXTFORMAT_BRUSHPROPERTY )
{
   QTextFormat * p = hbqt_par_QTextFormat( 1 );
   if( p )
   {
      hb_retptrGC( hbqt_gcAllocate_QBrush( new QBrush( ( p )->brushProperty( hb_parni( 2 ) ) ), true ) );
   }
}

/*
 * void clearBackground ()
 */
HB_FUNC( QT_QTEXTFORMAT_CLEARBACKGROUND )
{
   QTextFormat * p = hbqt_par_QTextFormat( 1 );
   if( p )
   {
      ( p )->clearBackground();
   }
}

/*
 * void clearForeground ()
 */
HB_FUNC( QT_QTEXTFORMAT_CLEARFOREGROUND )
{
   QTextFormat * p = hbqt_par_QTextFormat( 1 );
   if( p )
   {
      ( p )->clearForeground();
   }
}

/*
 * void clearProperty ( int propertyId )
 */
HB_FUNC( QT_QTEXTFORMAT_CLEARPROPERTY )
{
   QTextFormat * p = hbqt_par_QTextFormat( 1 );
   if( p )
   {
      ( p )->clearProperty( hb_parni( 2 ) );
   }
}

/*
 * QColor colorProperty ( int propertyId ) const
 */
HB_FUNC( QT_QTEXTFORMAT_COLORPROPERTY )
{
   QTextFormat * p = hbqt_par_QTextFormat( 1 );
   if( p )
   {
      hb_retptrGC( hbqt_gcAllocate_QColor( new QColor( ( p )->colorProperty( hb_parni( 2 ) ) ), true ) );
   }
}

/*
 * qreal doubleProperty ( int propertyId ) const
 */
HB_FUNC( QT_QTEXTFORMAT_DOUBLEPROPERTY )
{
   QTextFormat * p = hbqt_par_QTextFormat( 1 );
   if( p )
   {
      hb_retnd( ( p )->doubleProperty( hb_parni( 2 ) ) );
   }
}

/*
 * QBrush foreground () const
 */
HB_FUNC( QT_QTEXTFORMAT_FOREGROUND )
{
   QTextFormat * p = hbqt_par_QTextFormat( 1 );
   if( p )
   {
      hb_retptrGC( hbqt_gcAllocate_QBrush( new QBrush( ( p )->foreground() ), true ) );
   }
}

/*
 * bool hasProperty ( int propertyId ) const
 */
HB_FUNC( QT_QTEXTFORMAT_HASPROPERTY )
{
   QTextFormat * p = hbqt_par_QTextFormat( 1 );
   if( p )
   {
      hb_retl( ( p )->hasProperty( hb_parni( 2 ) ) );
   }
}

/*
 * int intProperty ( int propertyId ) const
 */
HB_FUNC( QT_QTEXTFORMAT_INTPROPERTY )
{
   QTextFormat * p = hbqt_par_QTextFormat( 1 );
   if( p )
   {
      hb_retni( ( p )->intProperty( hb_parni( 2 ) ) );
   }
}

/*
 * bool isBlockFormat () const
 */
HB_FUNC( QT_QTEXTFORMAT_ISBLOCKFORMAT )
{
   QTextFormat * p = hbqt_par_QTextFormat( 1 );
   if( p )
   {
      hb_retl( ( p )->isBlockFormat() );
   }
}

/*
 * bool isCharFormat () const
 */
HB_FUNC( QT_QTEXTFORMAT_ISCHARFORMAT )
{
   QTextFormat * p = hbqt_par_QTextFormat( 1 );
   if( p )
   {
      hb_retl( ( p )->isCharFormat() );
   }
}

/*
 * bool isFrameFormat () const
 */
HB_FUNC( QT_QTEXTFORMAT_ISFRAMEFORMAT )
{
   QTextFormat * p = hbqt_par_QTextFormat( 1 );
   if( p )
   {
      hb_retl( ( p )->isFrameFormat() );
   }
}

/*
 * bool isImageFormat () const
 */
HB_FUNC( QT_QTEXTFORMAT_ISIMAGEFORMAT )
{
   QTextFormat * p = hbqt_par_QTextFormat( 1 );
   if( p )
   {
      hb_retl( ( p )->isImageFormat() );
   }
}

/*
 * bool isListFormat () const
 */
HB_FUNC( QT_QTEXTFORMAT_ISLISTFORMAT )
{
   QTextFormat * p = hbqt_par_QTextFormat( 1 );
   if( p )
   {
      hb_retl( ( p )->isListFormat() );
   }
}

/*
 * bool isTableCellFormat () const
 */
HB_FUNC( QT_QTEXTFORMAT_ISTABLECELLFORMAT )
{
   QTextFormat * p = hbqt_par_QTextFormat( 1 );
   if( p )
   {
      hb_retl( ( p )->isTableCellFormat() );
   }
}

/*
 * bool isTableFormat () const
 */
HB_FUNC( QT_QTEXTFORMAT_ISTABLEFORMAT )
{
   QTextFormat * p = hbqt_par_QTextFormat( 1 );
   if( p )
   {
      hb_retl( ( p )->isTableFormat() );
   }
}

/*
 * bool isValid () const
 */
HB_FUNC( QT_QTEXTFORMAT_ISVALID )
{
   QTextFormat * p = hbqt_par_QTextFormat( 1 );
   if( p )
   {
      hb_retl( ( p )->isValid() );
   }
}

/*
 * Qt::LayoutDirection layoutDirection () const
 */
HB_FUNC( QT_QTEXTFORMAT_LAYOUTDIRECTION )
{
   QTextFormat * p = hbqt_par_QTextFormat( 1 );
   if( p )
   {
      hb_retni( ( Qt::LayoutDirection ) ( p )->layoutDirection() );
   }
}

/*
 * QTextLength lengthProperty ( int propertyId ) const
 */
HB_FUNC( QT_QTEXTFORMAT_LENGTHPROPERTY )
{
   QTextFormat * p = hbqt_par_QTextFormat( 1 );
   if( p )
   {
      hb_retptrGC( hbqt_gcAllocate_QTextLength( new QTextLength( ( p )->lengthProperty( hb_parni( 2 ) ) ), true ) );
   }
}

/*
 * void merge ( const QTextFormat & other )
 */
HB_FUNC( QT_QTEXTFORMAT_MERGE )
{
   QTextFormat * p = hbqt_par_QTextFormat( 1 );
   if( p )
   {
      ( p )->merge( *hbqt_par_QTextFormat( 2 ) );
   }
}

/*
 * int objectIndex () const
 */
HB_FUNC( QT_QTEXTFORMAT_OBJECTINDEX )
{
   QTextFormat * p = hbqt_par_QTextFormat( 1 );
   if( p )
   {
      hb_retni( ( p )->objectIndex() );
   }
}

/*
 * int objectType () const
 */
HB_FUNC( QT_QTEXTFORMAT_OBJECTTYPE )
{
   QTextFormat * p = hbqt_par_QTextFormat( 1 );
   if( p )
   {
      hb_retni( ( p )->objectType() );
   }
}

/*
 * QPen penProperty ( int propertyId ) const
 */
HB_FUNC( QT_QTEXTFORMAT_PENPROPERTY )
{
   QTextFormat * p = hbqt_par_QTextFormat( 1 );
   if( p )
   {
      hb_retptrGC( hbqt_gcAllocate_QPen( new QPen( ( p )->penProperty( hb_parni( 2 ) ) ), true ) );
   }
}

/*
 * QVariant property ( int propertyId ) const
 */
HB_FUNC( QT_QTEXTFORMAT_PROPERTY )
{
   QTextFormat * p = hbqt_par_QTextFormat( 1 );
   if( p )
   {
      hb_retptrGC( hbqt_gcAllocate_QVariant( new QVariant( ( p )->property( hb_parni( 2 ) ) ), true ) );
   }
}

/*
 * int propertyCount () const
 */
HB_FUNC( QT_QTEXTFORMAT_PROPERTYCOUNT )
{
   QTextFormat * p = hbqt_par_QTextFormat( 1 );
   if( p )
   {
      hb_retni( ( p )->propertyCount() );
   }
}

/*
 * void setBackground ( const QBrush & brush )
 */
HB_FUNC( QT_QTEXTFORMAT_SETBACKGROUND )
{
   QTextFormat * p = hbqt_par_QTextFormat( 1 );
   if( p )
   {
      ( p )->setBackground( *hbqt_par_QBrush( 2 ) );
   }
}

/*
 * void setForeground ( const QBrush & brush )
 */
HB_FUNC( QT_QTEXTFORMAT_SETFOREGROUND )
{
   QTextFormat * p = hbqt_par_QTextFormat( 1 );
   if( p )
   {
      ( p )->setForeground( *hbqt_par_QBrush( 2 ) );
   }
}

/*
 * void setLayoutDirection ( Qt::LayoutDirection direction )
 */
HB_FUNC( QT_QTEXTFORMAT_SETLAYOUTDIRECTION )
{
   QTextFormat * p = hbqt_par_QTextFormat( 1 );
   if( p )
   {
      ( p )->setLayoutDirection( ( Qt::LayoutDirection ) hb_parni( 2 ) );
   }
}

/*
 * void setObjectIndex ( int index )
 */
HB_FUNC( QT_QTEXTFORMAT_SETOBJECTINDEX )
{
   QTextFormat * p = hbqt_par_QTextFormat( 1 );
   if( p )
   {
      ( p )->setObjectIndex( hb_parni( 2 ) );
   }
}

/*
 * void setObjectType ( int type )
 */
HB_FUNC( QT_QTEXTFORMAT_SETOBJECTTYPE )
{
   QTextFormat * p = hbqt_par_QTextFormat( 1 );
   if( p )
   {
      ( p )->setObjectType( hb_parni( 2 ) );
   }
}

/*
 * void setProperty ( int propertyId, const QVariant & value )
 */
HB_FUNC( QT_QTEXTFORMAT_SETPROPERTY )
{
   QTextFormat * p = hbqt_par_QTextFormat( 1 );
   if( p )
   {
      ( p )->setProperty( hb_parni( 2 ), *hbqt_par_QVariant( 3 ) );
   }
}

/*
 * QString stringProperty ( int propertyId ) const
 */
HB_FUNC( QT_QTEXTFORMAT_STRINGPROPERTY )
{
   QTextFormat * p = hbqt_par_QTextFormat( 1 );
   if( p )
   {
      hb_retstr_utf8( ( p )->stringProperty( hb_parni( 2 ) ).toUtf8().data() );
   }
}

/*
 * QTextBlockFormat toBlockFormat () const
 */
HB_FUNC( QT_QTEXTFORMAT_TOBLOCKFORMAT )
{
   QTextFormat * p = hbqt_par_QTextFormat( 1 );
   if( p )
   {
      hb_retptrGC( hbqt_gcAllocate_QTextBlockFormat( new QTextBlockFormat( ( p )->toBlockFormat() ), true ) );
   }
}

/*
 * QTextCharFormat toCharFormat () const
 */
HB_FUNC( QT_QTEXTFORMAT_TOCHARFORMAT )
{
   QTextFormat * p = hbqt_par_QTextFormat( 1 );
   if( p )
   {
      hb_retptrGC( hbqt_gcAllocate_QTextCharFormat( new QTextCharFormat( ( p )->toCharFormat() ), true ) );
   }
}

/*
 * QTextFrameFormat toFrameFormat () const
 */
HB_FUNC( QT_QTEXTFORMAT_TOFRAMEFORMAT )
{
   QTextFormat * p = hbqt_par_QTextFormat( 1 );
   if( p )
   {
      hb_retptrGC( hbqt_gcAllocate_QTextFrameFormat( new QTextFrameFormat( ( p )->toFrameFormat() ), true ) );
   }
}

/*
 * QTextImageFormat toImageFormat () const
 */
HB_FUNC( QT_QTEXTFORMAT_TOIMAGEFORMAT )
{
   QTextFormat * p = hbqt_par_QTextFormat( 1 );
   if( p )
   {
      hb_retptrGC( hbqt_gcAllocate_QTextImageFormat( new QTextImageFormat( ( p )->toImageFormat() ), true ) );
   }
}

/*
 * QTextListFormat toListFormat () const
 */
HB_FUNC( QT_QTEXTFORMAT_TOLISTFORMAT )
{
   QTextFormat * p = hbqt_par_QTextFormat( 1 );
   if( p )
   {
      hb_retptrGC( hbqt_gcAllocate_QTextListFormat( new QTextListFormat( ( p )->toListFormat() ), true ) );
   }
}

/*
 * QTextTableFormat toTableFormat () const
 */
HB_FUNC( QT_QTEXTFORMAT_TOTABLEFORMAT )
{
   QTextFormat * p = hbqt_par_QTextFormat( 1 );
   if( p )
   {
      hb_retptrGC( hbqt_gcAllocate_QTextTableFormat( new QTextTableFormat( ( p )->toTableFormat() ), true ) );
   }
}

/*
 * int type () const
 */
HB_FUNC( QT_QTEXTFORMAT_TYPE )
{
   QTextFormat * p = hbqt_par_QTextFormat( 1 );
   if( p )
   {
      hb_retni( ( p )->type() );
   }
}


/*----------------------------------------------------------------------*/
#endif             /* #if QT_VERSION >= 0x040500 */
/*----------------------------------------------------------------------*/
