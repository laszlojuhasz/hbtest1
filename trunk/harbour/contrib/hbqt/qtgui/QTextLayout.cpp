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
 *  enum CursorMode { SkipCharacters, SkipWords }
 */

/*
 *  Constructed[ 29/32 [ 90.63% ] ]
 *
 *  *** Unconvered Prototypes ***
 *  -----------------------------
 *
 *  QList<FormatRange> additionalFormats () const
 *  void draw ( QPainter * p, const QPointF & pos, const QVector<FormatRange> & selections = QVector<FormatRange> (), const QRectF & clip = QRectF() ) const
 *  void setAdditionalFormats ( const QList<FormatRange> & formatList )
 */

#include <QtCore/QPointer>

#include <QtGui/QTextLayout>


/*
 * QTextLayout ()
 * QTextLayout ( const QString & text )
 * QTextLayout ( const QString & text, const QFont & font, QPaintDevice * paintdevice = 0 )
 * ~QTextLayout ()
 */

typedef struct
{
  void * ph;
  bool bNew;
  QT_G_FUNC_PTR func;
} QGC_POINTER_QTextLayout;

QT_G_FUNC( hbqt_gcRelease_QTextLayout )
{
      QGC_POINTER * p = ( QGC_POINTER * ) Cargo;

   if( p && p->bNew )
   {
      if( p->ph )
      {
         delete ( ( QTextLayout * ) p->ph );
         HB_TRACE( HB_TR_DEBUG, ( "YES_rel_QTextLayout                ph=%p %i B %i KB", p->ph, ( int ) hb_xquery( 1001 ), hbqt_getmemused() ) );
         p->ph = NULL;
      }
      else
      {
         HB_TRACE( HB_TR_DEBUG, ( "DEL_rel_QTextLayout                 Object already deleted!" ) );
      }
   }
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "PTR_rel_QTextLayout                 Object not created with - new" ) );
      p->ph = NULL;
   }
}

void * hbqt_gcAllocate_QTextLayout( void * pObj, bool bNew )
{
   QGC_POINTER * p = ( QGC_POINTER * ) hb_gcAllocate( sizeof( QGC_POINTER ), hbqt_gcFuncs() );

   p->ph = pObj;
   p->bNew = bNew;
   p->func = hbqt_gcRelease_QTextLayout;

   if( bNew )
   {
      HB_TRACE( HB_TR_DEBUG, ( "   _new_QTextLayout                ph=%p %i B %i KB", pObj, ( int ) hb_xquery( 1001 ), hbqt_getmemused() ) );
   }
   return p;
}

HB_FUNC( QT_QTEXTLAYOUT )
{
   void * pObj = NULL;

   pObj = ( QTextLayout* ) new QTextLayout() ;

   hb_retptrGC( hbqt_gcAllocate_QTextLayout( pObj, true ) );
}
/*
 * void beginLayout ()
 */
HB_FUNC( QT_QTEXTLAYOUT_BEGINLAYOUT )
{
   hbqt_par_QTextLayout( 1 )->beginLayout();
}

/*
 * QRectF boundingRect () const
 */
HB_FUNC( QT_QTEXTLAYOUT_BOUNDINGRECT )
{
   hb_retptrGC( hbqt_gcAllocate_QRectF( new QRectF( hbqt_par_QTextLayout( 1 )->boundingRect() ), true ) );
}

/*
 * bool cacheEnabled () const
 */
HB_FUNC( QT_QTEXTLAYOUT_CACHEENABLED )
{
   hb_retl( hbqt_par_QTextLayout( 1 )->cacheEnabled() );
}

/*
 * void clearAdditionalFormats ()
 */
HB_FUNC( QT_QTEXTLAYOUT_CLEARADDITIONALFORMATS )
{
   hbqt_par_QTextLayout( 1 )->clearAdditionalFormats();
}

/*
 * void clearLayout ()
 */
HB_FUNC( QT_QTEXTLAYOUT_CLEARLAYOUT )
{
   hbqt_par_QTextLayout( 1 )->clearLayout();
}

/*
 * QTextLine createLine ()
 */
HB_FUNC( QT_QTEXTLAYOUT_CREATELINE )
{
   hb_retptrGC( hbqt_gcAllocate_QTextLine( new QTextLine( hbqt_par_QTextLayout( 1 )->createLine() ), true ) );
}

/*
 * void drawCursor ( QPainter * painter, const QPointF & position, int cursorPosition, int width ) const
 */
HB_FUNC( QT_QTEXTLAYOUT_DRAWCURSOR )
{
   hbqt_par_QTextLayout( 1 )->drawCursor( hbqt_par_QPainter( 2 ), *hbqt_par_QPointF( 3 ), hb_parni( 4 ), hb_parni( 5 ) );
}

/*
 * void drawCursor ( QPainter * painter, const QPointF & position, int cursorPosition ) const
 */
HB_FUNC( QT_QTEXTLAYOUT_DRAWCURSOR_1 )
{
   hbqt_par_QTextLayout( 1 )->drawCursor( hbqt_par_QPainter( 2 ), *hbqt_par_QPointF( 3 ), hb_parni( 4 ) );
}

/*
 * void endLayout ()
 */
HB_FUNC( QT_QTEXTLAYOUT_ENDLAYOUT )
{
   hbqt_par_QTextLayout( 1 )->endLayout();
}

/*
 * QFont font () const
 */
HB_FUNC( QT_QTEXTLAYOUT_FONT )
{
   hb_retptrGC( hbqt_gcAllocate_QFont( new QFont( hbqt_par_QTextLayout( 1 )->font() ), true ) );
}

/*
 * bool isValidCursorPosition ( int pos ) const
 */
HB_FUNC( QT_QTEXTLAYOUT_ISVALIDCURSORPOSITION )
{
   hb_retl( hbqt_par_QTextLayout( 1 )->isValidCursorPosition( hb_parni( 2 ) ) );
}

/*
 * QTextLine lineAt ( int i ) const
 */
HB_FUNC( QT_QTEXTLAYOUT_LINEAT )
{
   hb_retptrGC( hbqt_gcAllocate_QTextLine( new QTextLine( hbqt_par_QTextLayout( 1 )->lineAt( hb_parni( 2 ) ) ), true ) );
}

/*
 * int lineCount () const
 */
HB_FUNC( QT_QTEXTLAYOUT_LINECOUNT )
{
   hb_retni( hbqt_par_QTextLayout( 1 )->lineCount() );
}

/*
 * QTextLine lineForTextPosition ( int pos ) const
 */
HB_FUNC( QT_QTEXTLAYOUT_LINEFORTEXTPOSITION )
{
   hb_retptrGC( hbqt_gcAllocate_QTextLine( new QTextLine( hbqt_par_QTextLayout( 1 )->lineForTextPosition( hb_parni( 2 ) ) ), true ) );
}

/*
 * qreal maximumWidth () const
 */
HB_FUNC( QT_QTEXTLAYOUT_MAXIMUMWIDTH )
{
   hb_retnd( hbqt_par_QTextLayout( 1 )->maximumWidth() );
}

/*
 * qreal minimumWidth () const
 */
HB_FUNC( QT_QTEXTLAYOUT_MINIMUMWIDTH )
{
   hb_retnd( hbqt_par_QTextLayout( 1 )->minimumWidth() );
}

/*
 * int nextCursorPosition ( int oldPos, CursorMode mode = SkipCharacters ) const
 */
HB_FUNC( QT_QTEXTLAYOUT_NEXTCURSORPOSITION )
{
   hb_retni( hbqt_par_QTextLayout( 1 )->nextCursorPosition( hb_parni( 2 ), ( HB_ISNUM( 3 ) ? ( QTextLayout::CursorMode ) hb_parni( 3 ) : ( QTextLayout::CursorMode ) QTextLayout::SkipCharacters ) ) );
}

/*
 * QPointF position () const
 */
HB_FUNC( QT_QTEXTLAYOUT_POSITION )
{
   hb_retptrGC( hbqt_gcAllocate_QPointF( new QPointF( hbqt_par_QTextLayout( 1 )->position() ), true ) );
}

/*
 * int preeditAreaPosition () const
 */
HB_FUNC( QT_QTEXTLAYOUT_PREEDITAREAPOSITION )
{
   hb_retni( hbqt_par_QTextLayout( 1 )->preeditAreaPosition() );
}

/*
 * QString preeditAreaText () const
 */
HB_FUNC( QT_QTEXTLAYOUT_PREEDITAREATEXT )
{
   hb_retc( hbqt_par_QTextLayout( 1 )->preeditAreaText().toAscii().data() );
}

/*
 * int previousCursorPosition ( int oldPos, CursorMode mode = SkipCharacters ) const
 */
HB_FUNC( QT_QTEXTLAYOUT_PREVIOUSCURSORPOSITION )
{
   hb_retni( hbqt_par_QTextLayout( 1 )->previousCursorPosition( hb_parni( 2 ), ( HB_ISNUM( 3 ) ? ( QTextLayout::CursorMode ) hb_parni( 3 ) : ( QTextLayout::CursorMode ) QTextLayout::SkipCharacters ) ) );
}

/*
 * void setCacheEnabled ( bool enable )
 */
HB_FUNC( QT_QTEXTLAYOUT_SETCACHEENABLED )
{
   hbqt_par_QTextLayout( 1 )->setCacheEnabled( hb_parl( 2 ) );
}

/*
 * void setFont ( const QFont & font )
 */
HB_FUNC( QT_QTEXTLAYOUT_SETFONT )
{
   hbqt_par_QTextLayout( 1 )->setFont( *hbqt_par_QFont( 2 ) );
}

/*
 * void setPosition ( const QPointF & p )
 */
HB_FUNC( QT_QTEXTLAYOUT_SETPOSITION )
{
   hbqt_par_QTextLayout( 1 )->setPosition( *hbqt_par_QPointF( 2 ) );
}

/*
 * void setPreeditArea ( int position, const QString & text )
 */
HB_FUNC( QT_QTEXTLAYOUT_SETPREEDITAREA )
{
   hbqt_par_QTextLayout( 1 )->setPreeditArea( hb_parni( 2 ), hbqt_par_QString( 3 ) );
}

/*
 * void setText ( const QString & string )
 */
HB_FUNC( QT_QTEXTLAYOUT_SETTEXT )
{
   hbqt_par_QTextLayout( 1 )->setText( hbqt_par_QString( 2 ) );
}

/*
 * void setTextOption ( const QTextOption & option )
 */
HB_FUNC( QT_QTEXTLAYOUT_SETTEXTOPTION )
{
   hbqt_par_QTextLayout( 1 )->setTextOption( *hbqt_par_QTextOption( 2 ) );
}

/*
 * QString text () const
 */
HB_FUNC( QT_QTEXTLAYOUT_TEXT )
{
   hb_retc( hbqt_par_QTextLayout( 1 )->text().toAscii().data() );
}

/*
 * QTextOption textOption () const
 */
HB_FUNC( QT_QTEXTLAYOUT_TEXTOPTION )
{
   hb_retptrGC( hbqt_gcAllocate_QTextOption( new QTextOption( hbqt_par_QTextLayout( 1 )->textOption() ), true ) );
}


/*----------------------------------------------------------------------*/
#endif             /* #if QT_VERSION >= 0x040500 */
/*----------------------------------------------------------------------*/
