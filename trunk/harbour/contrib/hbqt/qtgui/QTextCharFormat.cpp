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
 *  enum UnderlineStyle { NoUnderline, SingleUnderline, DashUnderline, DotLine, ..., SpellCheckUnderline }
 *  enum VerticalAlignment { AlignNormal, AlignSuperScript, AlignSubScript, AlignMiddle, AlignBottom, AlignTop }
 */

#include <QtCore/QPointer>

#include <QtGui/QTextCharFormat>


/*
 * QTextCharFormat ()
 *
 */

typedef struct
{
   QTextCharFormat * ph;
   bool bNew;
   QT_G_FUNC_PTR func;
} QGC_POINTER_QTextCharFormat;

QT_G_FUNC( hbqt_gcRelease_QTextCharFormat )
{
   QGC_POINTER * p = ( QGC_POINTER * ) Cargo;

   if( p && p->bNew )
   {
      if( p->ph )
      {
         HB_TRACE( HB_TR_DEBUG, ( "ph=%p    _rel_QTextCharFormat   /.\\", p->ph ) );
         delete ( ( QTextCharFormat * ) p->ph );
         HB_TRACE( HB_TR_DEBUG, ( "ph=%p YES_rel_QTextCharFormat   \\./", p->ph ) );
         p->ph = NULL;
      }
      else
      {
         HB_TRACE( HB_TR_DEBUG, ( "ph=%p DEL_rel_QTextCharFormat    :     Object already deleted!", p->ph ) );
         p->ph = NULL;
      }
   }
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "ph=%p PTR_rel_QTextCharFormat    :    Object not created with new=true", p->ph ) );
      p->ph = NULL;
   }
}

void * hbqt_gcAllocate_QTextCharFormat( void * pObj, bool bNew )
{
   QGC_POINTER * p = ( QGC_POINTER * ) hb_gcAllocate( sizeof( QGC_POINTER ), hbqt_gcFuncs() );

   p->ph = ( QTextCharFormat * ) pObj;
   p->bNew = bNew;
   p->func = hbqt_gcRelease_QTextCharFormat;

   if( bNew )
   {
      HB_TRACE( HB_TR_DEBUG, ( "ph=%p    _new_QTextCharFormat", pObj ) );
   }
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "ph=%p NOT_new_QTextCharFormat", pObj ) );
   }
   return p;
}

HB_FUNC( QT_QTEXTCHARFORMAT )
{
   QTextCharFormat * pObj = NULL;

   pObj =  new QTextCharFormat() ;

   hb_retptrGC( hbqt_gcAllocate_QTextCharFormat( ( void * ) pObj, true ) );
}

/*
 * QString anchorHref () const
 */
HB_FUNC( QT_QTEXTCHARFORMAT_ANCHORHREF )
{
   QTextCharFormat * p = hbqt_par_QTextCharFormat( 1 );
   if( p )
      hb_retc( ( p )->anchorHref().toAscii().data() );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QTEXTCHARFORMAT_ANCHORHREF FP=hb_retc( ( p )->anchorHref().toAscii().data() ); p is NULL" ) );
   }
}

/*
 * QStringList anchorNames () const
 */
HB_FUNC( QT_QTEXTCHARFORMAT_ANCHORNAMES )
{
   QTextCharFormat * p = hbqt_par_QTextCharFormat( 1 );
   if( p )
      hb_retptrGC( hbqt_gcAllocate_QStringList( new QStringList( ( p )->anchorNames() ), true ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QTEXTCHARFORMAT_ANCHORNAMES FP=hb_retptrGC( hbqt_gcAllocate_QStringList( new QStringList( ( p )->anchorNames() ), true ) ); p is NULL" ) );
   }
}

/*
 * QFont font () const
 */
HB_FUNC( QT_QTEXTCHARFORMAT_FONT )
{
   QTextCharFormat * p = hbqt_par_QTextCharFormat( 1 );
   if( p )
      hb_retptrGC( hbqt_gcAllocate_QFont( new QFont( ( p )->font() ), true ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QTEXTCHARFORMAT_FONT FP=hb_retptrGC( hbqt_gcAllocate_QFont( new QFont( ( p )->font() ), true ) ); p is NULL" ) );
   }
}

/*
 * QFont::Capitalization fontCapitalization () const
 */
HB_FUNC( QT_QTEXTCHARFORMAT_FONTCAPITALIZATION )
{
   QTextCharFormat * p = hbqt_par_QTextCharFormat( 1 );
   if( p )
      hb_retni( ( QFont::Capitalization ) ( p )->fontCapitalization() );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QTEXTCHARFORMAT_FONTCAPITALIZATION FP=hb_retni( ( QFont::Capitalization ) ( p )->fontCapitalization() ); p is NULL" ) );
   }
}

/*
 * QString fontFamily () const
 */
HB_FUNC( QT_QTEXTCHARFORMAT_FONTFAMILY )
{
   QTextCharFormat * p = hbqt_par_QTextCharFormat( 1 );
   if( p )
      hb_retc( ( p )->fontFamily().toAscii().data() );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QTEXTCHARFORMAT_FONTFAMILY FP=hb_retc( ( p )->fontFamily().toAscii().data() ); p is NULL" ) );
   }
}

/*
 * bool fontFixedPitch () const
 */
HB_FUNC( QT_QTEXTCHARFORMAT_FONTFIXEDPITCH )
{
   QTextCharFormat * p = hbqt_par_QTextCharFormat( 1 );
   if( p )
      hb_retl( ( p )->fontFixedPitch() );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QTEXTCHARFORMAT_FONTFIXEDPITCH FP=hb_retl( ( p )->fontFixedPitch() ); p is NULL" ) );
   }
}

/*
 * bool fontItalic () const
 */
HB_FUNC( QT_QTEXTCHARFORMAT_FONTITALIC )
{
   QTextCharFormat * p = hbqt_par_QTextCharFormat( 1 );
   if( p )
      hb_retl( ( p )->fontItalic() );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QTEXTCHARFORMAT_FONTITALIC FP=hb_retl( ( p )->fontItalic() ); p is NULL" ) );
   }
}

/*
 * bool fontKerning () const
 */
HB_FUNC( QT_QTEXTCHARFORMAT_FONTKERNING )
{
   QTextCharFormat * p = hbqt_par_QTextCharFormat( 1 );
   if( p )
      hb_retl( ( p )->fontKerning() );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QTEXTCHARFORMAT_FONTKERNING FP=hb_retl( ( p )->fontKerning() ); p is NULL" ) );
   }
}

/*
 * qreal fontLetterSpacing () const
 */
HB_FUNC( QT_QTEXTCHARFORMAT_FONTLETTERSPACING )
{
   QTextCharFormat * p = hbqt_par_QTextCharFormat( 1 );
   if( p )
      hb_retnd( ( p )->fontLetterSpacing() );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QTEXTCHARFORMAT_FONTLETTERSPACING FP=hb_retnd( ( p )->fontLetterSpacing() ); p is NULL" ) );
   }
}

/*
 * bool fontOverline () const
 */
HB_FUNC( QT_QTEXTCHARFORMAT_FONTOVERLINE )
{
   QTextCharFormat * p = hbqt_par_QTextCharFormat( 1 );
   if( p )
      hb_retl( ( p )->fontOverline() );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QTEXTCHARFORMAT_FONTOVERLINE FP=hb_retl( ( p )->fontOverline() ); p is NULL" ) );
   }
}

/*
 * qreal fontPointSize () const
 */
HB_FUNC( QT_QTEXTCHARFORMAT_FONTPOINTSIZE )
{
   QTextCharFormat * p = hbqt_par_QTextCharFormat( 1 );
   if( p )
      hb_retnd( ( p )->fontPointSize() );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QTEXTCHARFORMAT_FONTPOINTSIZE FP=hb_retnd( ( p )->fontPointSize() ); p is NULL" ) );
   }
}

/*
 * bool fontStrikeOut () const
 */
HB_FUNC( QT_QTEXTCHARFORMAT_FONTSTRIKEOUT )
{
   QTextCharFormat * p = hbqt_par_QTextCharFormat( 1 );
   if( p )
      hb_retl( ( p )->fontStrikeOut() );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QTEXTCHARFORMAT_FONTSTRIKEOUT FP=hb_retl( ( p )->fontStrikeOut() ); p is NULL" ) );
   }
}

/*
 * QFont::StyleHint fontStyleHint () const
 */
HB_FUNC( QT_QTEXTCHARFORMAT_FONTSTYLEHINT )
{
   QTextCharFormat * p = hbqt_par_QTextCharFormat( 1 );
   if( p )
      hb_retni( ( QFont::StyleHint ) ( p )->fontStyleHint() );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QTEXTCHARFORMAT_FONTSTYLEHINT FP=hb_retni( ( QFont::StyleHint ) ( p )->fontStyleHint() ); p is NULL" ) );
   }
}

/*
 * QFont::StyleStrategy fontStyleStrategy () const
 */
HB_FUNC( QT_QTEXTCHARFORMAT_FONTSTYLESTRATEGY )
{
   QTextCharFormat * p = hbqt_par_QTextCharFormat( 1 );
   if( p )
      hb_retni( ( QFont::StyleStrategy ) ( p )->fontStyleStrategy() );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QTEXTCHARFORMAT_FONTSTYLESTRATEGY FP=hb_retni( ( QFont::StyleStrategy ) ( p )->fontStyleStrategy() ); p is NULL" ) );
   }
}

/*
 * bool fontUnderline () const
 */
HB_FUNC( QT_QTEXTCHARFORMAT_FONTUNDERLINE )
{
   QTextCharFormat * p = hbqt_par_QTextCharFormat( 1 );
   if( p )
      hb_retl( ( p )->fontUnderline() );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QTEXTCHARFORMAT_FONTUNDERLINE FP=hb_retl( ( p )->fontUnderline() ); p is NULL" ) );
   }
}

/*
 * int fontWeight () const
 */
HB_FUNC( QT_QTEXTCHARFORMAT_FONTWEIGHT )
{
   QTextCharFormat * p = hbqt_par_QTextCharFormat( 1 );
   if( p )
      hb_retni( ( p )->fontWeight() );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QTEXTCHARFORMAT_FONTWEIGHT FP=hb_retni( ( p )->fontWeight() ); p is NULL" ) );
   }
}

/*
 * qreal fontWordSpacing () const
 */
HB_FUNC( QT_QTEXTCHARFORMAT_FONTWORDSPACING )
{
   QTextCharFormat * p = hbqt_par_QTextCharFormat( 1 );
   if( p )
      hb_retnd( ( p )->fontWordSpacing() );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QTEXTCHARFORMAT_FONTWORDSPACING FP=hb_retnd( ( p )->fontWordSpacing() ); p is NULL" ) );
   }
}

/*
 * bool isAnchor () const
 */
HB_FUNC( QT_QTEXTCHARFORMAT_ISANCHOR )
{
   QTextCharFormat * p = hbqt_par_QTextCharFormat( 1 );
   if( p )
      hb_retl( ( p )->isAnchor() );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QTEXTCHARFORMAT_ISANCHOR FP=hb_retl( ( p )->isAnchor() ); p is NULL" ) );
   }
}

/*
 * bool isValid () const
 */
HB_FUNC( QT_QTEXTCHARFORMAT_ISVALID )
{
   QTextCharFormat * p = hbqt_par_QTextCharFormat( 1 );
   if( p )
      hb_retl( ( p )->isValid() );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QTEXTCHARFORMAT_ISVALID FP=hb_retl( ( p )->isValid() ); p is NULL" ) );
   }
}

/*
 * void setAnchor ( bool anchor )
 */
HB_FUNC( QT_QTEXTCHARFORMAT_SETANCHOR )
{
   QTextCharFormat * p = hbqt_par_QTextCharFormat( 1 );
   if( p )
      ( p )->setAnchor( hb_parl( 2 ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QTEXTCHARFORMAT_SETANCHOR FP=( p )->setAnchor( hb_parl( 2 ) ); p is NULL" ) );
   }
}

/*
 * void setAnchorHref ( const QString & value )
 */
HB_FUNC( QT_QTEXTCHARFORMAT_SETANCHORHREF )
{
   QTextCharFormat * p = hbqt_par_QTextCharFormat( 1 );
   if( p )
      ( p )->setAnchorHref( hbqt_par_QString( 2 ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QTEXTCHARFORMAT_SETANCHORHREF FP=( p )->setAnchorHref( hbqt_par_QString( 2 ) ); p is NULL" ) );
   }
}

/*
 * void setAnchorNames ( const QStringList & names )
 */
HB_FUNC( QT_QTEXTCHARFORMAT_SETANCHORNAMES )
{
   QTextCharFormat * p = hbqt_par_QTextCharFormat( 1 );
   if( p )
      ( p )->setAnchorNames( *hbqt_par_QStringList( 2 ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QTEXTCHARFORMAT_SETANCHORNAMES FP=( p )->setAnchorNames( *hbqt_par_QStringList( 2 ) ); p is NULL" ) );
   }
}

/*
 * void setFont ( const QFont & font )
 */
HB_FUNC( QT_QTEXTCHARFORMAT_SETFONT )
{
   QTextCharFormat * p = hbqt_par_QTextCharFormat( 1 );
   if( p )
      ( p )->setFont( *hbqt_par_QFont( 2 ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QTEXTCHARFORMAT_SETFONT FP=( p )->setFont( *hbqt_par_QFont( 2 ) ); p is NULL" ) );
   }
}

/*
 * void setFontCapitalization ( QFont::Capitalization capitalization )
 */
HB_FUNC( QT_QTEXTCHARFORMAT_SETFONTCAPITALIZATION )
{
   QTextCharFormat * p = hbqt_par_QTextCharFormat( 1 );
   if( p )
      ( p )->setFontCapitalization( ( QFont::Capitalization ) hb_parni( 2 ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QTEXTCHARFORMAT_SETFONTCAPITALIZATION FP=( p )->setFontCapitalization( ( QFont::Capitalization ) hb_parni( 2 ) ); p is NULL" ) );
   }
}

/*
 * void setFontFamily ( const QString & family )
 */
HB_FUNC( QT_QTEXTCHARFORMAT_SETFONTFAMILY )
{
   QTextCharFormat * p = hbqt_par_QTextCharFormat( 1 );
   if( p )
      ( p )->setFontFamily( hbqt_par_QString( 2 ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QTEXTCHARFORMAT_SETFONTFAMILY FP=( p )->setFontFamily( hbqt_par_QString( 2 ) ); p is NULL" ) );
   }
}

/*
 * void setFontFixedPitch ( bool fixedPitch )
 */
HB_FUNC( QT_QTEXTCHARFORMAT_SETFONTFIXEDPITCH )
{
   QTextCharFormat * p = hbqt_par_QTextCharFormat( 1 );
   if( p )
      ( p )->setFontFixedPitch( hb_parl( 2 ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QTEXTCHARFORMAT_SETFONTFIXEDPITCH FP=( p )->setFontFixedPitch( hb_parl( 2 ) ); p is NULL" ) );
   }
}

/*
 * void setFontItalic ( bool italic )
 */
HB_FUNC( QT_QTEXTCHARFORMAT_SETFONTITALIC )
{
   QTextCharFormat * p = hbqt_par_QTextCharFormat( 1 );
   if( p )
      ( p )->setFontItalic( hb_parl( 2 ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QTEXTCHARFORMAT_SETFONTITALIC FP=( p )->setFontItalic( hb_parl( 2 ) ); p is NULL" ) );
   }
}

/*
 * void setFontKerning ( bool enable )
 */
HB_FUNC( QT_QTEXTCHARFORMAT_SETFONTKERNING )
{
   QTextCharFormat * p = hbqt_par_QTextCharFormat( 1 );
   if( p )
      ( p )->setFontKerning( hb_parl( 2 ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QTEXTCHARFORMAT_SETFONTKERNING FP=( p )->setFontKerning( hb_parl( 2 ) ); p is NULL" ) );
   }
}

/*
 * void setFontLetterSpacing ( qreal spacing )
 */
HB_FUNC( QT_QTEXTCHARFORMAT_SETFONTLETTERSPACING )
{
   QTextCharFormat * p = hbqt_par_QTextCharFormat( 1 );
   if( p )
      ( p )->setFontLetterSpacing( hb_parnd( 2 ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QTEXTCHARFORMAT_SETFONTLETTERSPACING FP=( p )->setFontLetterSpacing( hb_parnd( 2 ) ); p is NULL" ) );
   }
}

/*
 * void setFontOverline ( bool overline )
 */
HB_FUNC( QT_QTEXTCHARFORMAT_SETFONTOVERLINE )
{
   QTextCharFormat * p = hbqt_par_QTextCharFormat( 1 );
   if( p )
      ( p )->setFontOverline( hb_parl( 2 ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QTEXTCHARFORMAT_SETFONTOVERLINE FP=( p )->setFontOverline( hb_parl( 2 ) ); p is NULL" ) );
   }
}

/*
 * void setFontPointSize ( qreal size )
 */
HB_FUNC( QT_QTEXTCHARFORMAT_SETFONTPOINTSIZE )
{
   QTextCharFormat * p = hbqt_par_QTextCharFormat( 1 );
   if( p )
      ( p )->setFontPointSize( hb_parnd( 2 ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QTEXTCHARFORMAT_SETFONTPOINTSIZE FP=( p )->setFontPointSize( hb_parnd( 2 ) ); p is NULL" ) );
   }
}

/*
 * void setFontStrikeOut ( bool strikeOut )
 */
HB_FUNC( QT_QTEXTCHARFORMAT_SETFONTSTRIKEOUT )
{
   QTextCharFormat * p = hbqt_par_QTextCharFormat( 1 );
   if( p )
      ( p )->setFontStrikeOut( hb_parl( 2 ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QTEXTCHARFORMAT_SETFONTSTRIKEOUT FP=( p )->setFontStrikeOut( hb_parl( 2 ) ); p is NULL" ) );
   }
}

/*
 * void setFontStyleHint ( QFont::StyleHint hint, QFont::StyleStrategy strategy = QFont::PreferDefault )
 */
HB_FUNC( QT_QTEXTCHARFORMAT_SETFONTSTYLEHINT )
{
   QTextCharFormat * p = hbqt_par_QTextCharFormat( 1 );
   if( p )
      ( p )->setFontStyleHint( ( QFont::StyleHint ) hb_parni( 2 ), ( HB_ISNUM( 3 ) ? ( QFont::StyleStrategy ) hb_parni( 3 ) : ( QFont::StyleStrategy ) QFont::PreferDefault ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QTEXTCHARFORMAT_SETFONTSTYLEHINT FP=( p )->setFontStyleHint( ( QFont::StyleHint ) hb_parni( 2 ), ( HB_ISNUM( 3 ) ? ( QFont::StyleStrategy ) hb_parni( 3 ) : ( QFont::StyleStrategy ) QFont::PreferDefault ) ); p is NULL" ) );
   }
}

/*
 * void setFontStyleStrategy ( QFont::StyleStrategy strategy )
 */
HB_FUNC( QT_QTEXTCHARFORMAT_SETFONTSTYLESTRATEGY )
{
   QTextCharFormat * p = hbqt_par_QTextCharFormat( 1 );
   if( p )
      ( p )->setFontStyleStrategy( ( QFont::StyleStrategy ) hb_parni( 2 ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QTEXTCHARFORMAT_SETFONTSTYLESTRATEGY FP=( p )->setFontStyleStrategy( ( QFont::StyleStrategy ) hb_parni( 2 ) ); p is NULL" ) );
   }
}

/*
 * void setFontUnderline ( bool underline )
 */
HB_FUNC( QT_QTEXTCHARFORMAT_SETFONTUNDERLINE )
{
   QTextCharFormat * p = hbqt_par_QTextCharFormat( 1 );
   if( p )
      ( p )->setFontUnderline( hb_parl( 2 ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QTEXTCHARFORMAT_SETFONTUNDERLINE FP=( p )->setFontUnderline( hb_parl( 2 ) ); p is NULL" ) );
   }
}

/*
 * void setFontWeight ( int weight )
 */
HB_FUNC( QT_QTEXTCHARFORMAT_SETFONTWEIGHT )
{
   QTextCharFormat * p = hbqt_par_QTextCharFormat( 1 );
   if( p )
      ( p )->setFontWeight( hb_parni( 2 ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QTEXTCHARFORMAT_SETFONTWEIGHT FP=( p )->setFontWeight( hb_parni( 2 ) ); p is NULL" ) );
   }
}

/*
 * void setFontWordSpacing ( qreal spacing )
 */
HB_FUNC( QT_QTEXTCHARFORMAT_SETFONTWORDSPACING )
{
   QTextCharFormat * p = hbqt_par_QTextCharFormat( 1 );
   if( p )
      ( p )->setFontWordSpacing( hb_parnd( 2 ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QTEXTCHARFORMAT_SETFONTWORDSPACING FP=( p )->setFontWordSpacing( hb_parnd( 2 ) ); p is NULL" ) );
   }
}

/*
 * void setTextOutline ( const QPen & pen )
 */
HB_FUNC( QT_QTEXTCHARFORMAT_SETTEXTOUTLINE )
{
   QTextCharFormat * p = hbqt_par_QTextCharFormat( 1 );
   if( p )
      ( p )->setTextOutline( *hbqt_par_QPen( 2 ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QTEXTCHARFORMAT_SETTEXTOUTLINE FP=( p )->setTextOutline( *hbqt_par_QPen( 2 ) ); p is NULL" ) );
   }
}

/*
 * void setToolTip ( const QString & text )
 */
HB_FUNC( QT_QTEXTCHARFORMAT_SETTOOLTIP )
{
   QTextCharFormat * p = hbqt_par_QTextCharFormat( 1 );
   if( p )
      ( p )->setToolTip( hbqt_par_QString( 2 ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QTEXTCHARFORMAT_SETTOOLTIP FP=( p )->setToolTip( hbqt_par_QString( 2 ) ); p is NULL" ) );
   }
}

/*
 * void setUnderlineColor ( const QColor & color )
 */
HB_FUNC( QT_QTEXTCHARFORMAT_SETUNDERLINECOLOR )
{
   QTextCharFormat * p = hbqt_par_QTextCharFormat( 1 );
   if( p )
      ( p )->setUnderlineColor( *hbqt_par_QColor( 2 ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QTEXTCHARFORMAT_SETUNDERLINECOLOR FP=( p )->setUnderlineColor( *hbqt_par_QColor( 2 ) ); p is NULL" ) );
   }
}

/*
 * void setUnderlineStyle ( UnderlineStyle style )
 */
HB_FUNC( QT_QTEXTCHARFORMAT_SETUNDERLINESTYLE )
{
   QTextCharFormat * p = hbqt_par_QTextCharFormat( 1 );
   if( p )
      ( p )->setUnderlineStyle( ( QTextCharFormat::UnderlineStyle ) hb_parni( 2 ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QTEXTCHARFORMAT_SETUNDERLINESTYLE FP=( p )->setUnderlineStyle( ( QTextCharFormat::UnderlineStyle ) hb_parni( 2 ) ); p is NULL" ) );
   }
}

/*
 * void setVerticalAlignment ( VerticalAlignment alignment )
 */
HB_FUNC( QT_QTEXTCHARFORMAT_SETVERTICALALIGNMENT )
{
   QTextCharFormat * p = hbqt_par_QTextCharFormat( 1 );
   if( p )
      ( p )->setVerticalAlignment( ( QTextCharFormat::VerticalAlignment ) hb_parni( 2 ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QTEXTCHARFORMAT_SETVERTICALALIGNMENT FP=( p )->setVerticalAlignment( ( QTextCharFormat::VerticalAlignment ) hb_parni( 2 ) ); p is NULL" ) );
   }
}

/*
 * QPen textOutline () const
 */
HB_FUNC( QT_QTEXTCHARFORMAT_TEXTOUTLINE )
{
   QTextCharFormat * p = hbqt_par_QTextCharFormat( 1 );
   if( p )
      hb_retptrGC( hbqt_gcAllocate_QPen( new QPen( ( p )->textOutline() ), true ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QTEXTCHARFORMAT_TEXTOUTLINE FP=hb_retptrGC( hbqt_gcAllocate_QPen( new QPen( ( p )->textOutline() ), true ) ); p is NULL" ) );
   }
}

/*
 * QString toolTip () const
 */
HB_FUNC( QT_QTEXTCHARFORMAT_TOOLTIP )
{
   QTextCharFormat * p = hbqt_par_QTextCharFormat( 1 );
   if( p )
      hb_retc( ( p )->toolTip().toAscii().data() );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QTEXTCHARFORMAT_TOOLTIP FP=hb_retc( ( p )->toolTip().toAscii().data() ); p is NULL" ) );
   }
}

/*
 * QColor underlineColor () const
 */
HB_FUNC( QT_QTEXTCHARFORMAT_UNDERLINECOLOR )
{
   QTextCharFormat * p = hbqt_par_QTextCharFormat( 1 );
   if( p )
      hb_retptrGC( hbqt_gcAllocate_QColor( new QColor( ( p )->underlineColor() ), true ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QTEXTCHARFORMAT_UNDERLINECOLOR FP=hb_retptrGC( hbqt_gcAllocate_QColor( new QColor( ( p )->underlineColor() ), true ) ); p is NULL" ) );
   }
}

/*
 * UnderlineStyle underlineStyle () const
 */
HB_FUNC( QT_QTEXTCHARFORMAT_UNDERLINESTYLE )
{
   QTextCharFormat * p = hbqt_par_QTextCharFormat( 1 );
   if( p )
      hb_retni( ( QTextCharFormat::UnderlineStyle ) ( p )->underlineStyle() );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QTEXTCHARFORMAT_UNDERLINESTYLE FP=hb_retni( ( QTextCharFormat::UnderlineStyle ) ( p )->underlineStyle() ); p is NULL" ) );
   }
}

/*
 * VerticalAlignment verticalAlignment () const
 */
HB_FUNC( QT_QTEXTCHARFORMAT_VERTICALALIGNMENT )
{
   QTextCharFormat * p = hbqt_par_QTextCharFormat( 1 );
   if( p )
      hb_retni( ( QTextCharFormat::VerticalAlignment ) ( p )->verticalAlignment() );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QTEXTCHARFORMAT_VERTICALALIGNMENT FP=hb_retni( ( QTextCharFormat::VerticalAlignment ) ( p )->verticalAlignment() ); p is NULL" ) );
   }
}


/*----------------------------------------------------------------------*/
#endif             /* #if QT_VERSION >= 0x040500 */
/*----------------------------------------------------------------------*/
