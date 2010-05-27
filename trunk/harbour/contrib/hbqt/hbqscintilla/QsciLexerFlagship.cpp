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
 *  enum {
 *    Default = 0, Comment = 1, CommentLine = 2,
 *    CommentDoc = 3, Number = 4, Keyword = 5,
 *    DoubleQuotedString = 6, SingleQuotedString = 7, UUID = 8,
 *    PreProcessor = 9, Operator = 10, Identifier = 11,
 *    UnclosedString = 12, VerbatimString = 13, Regex = 14,
 *    CommentLineDoc = 15, KeywordSet2 = 16, CommentDocKeyword = 17,
 *    CommentDocKeywordError = 18, GlobalClass = 19
 *  }
 */

#include <QtCore/QPointer>

#include <qscilexerflagship.h>
#include "hbqt_garbage.h"


/*
 * QsciLexerFlagship (QObject *parent=0, bool caseInsensitiveKeywords=false)
 * virtual ~QsciLexerFlagship ()
 *
 */

typedef struct
{
   QPointer< QsciLexerFlagship > ph;
   bool bNew;
   QT_G_FUNC_PTR func;
} QGC_POINTER_QsciLexerFlagship;

QT_G_FUNC( hbqt_gcRelease_QsciLexerFlagship )
{
   QsciLexerFlagship  * ph = NULL ;
   QGC_POINTER_QsciLexerFlagship * p = ( QGC_POINTER_QsciLexerFlagship * ) Cargo;

   if( p && p->bNew && p->ph )
   {
      ph = p->ph;
      if( ph )
      {
         const QMetaObject * m = ( ph )->metaObject();
         if( ( QString ) m->className() != ( QString ) "QObject" )
         {
            HB_TRACE( HB_TR_DEBUG, ( "ph=%p %p YES_rel_QsciLexerFlagship   /.\\   ", (void*) ph, (void*) p->ph ) );
            delete ( p->ph );
            HB_TRACE( HB_TR_DEBUG, ( "ph=%p %p YES_rel_QsciLexerFlagship   \\./   ", (void*) ph, (void*) p->ph ) );
            p->ph = NULL;
         }
         else
         {
            HB_TRACE( HB_TR_DEBUG, ( "ph=%p NO__rel_QsciLexerFlagship          ", ph ) );
            p->ph = NULL;
         }
      }
      else
      {
         HB_TRACE( HB_TR_DEBUG, ( "ph=%p DEL_rel_QsciLexerFlagship    :     Object already deleted!", ph ) );
         p->ph = NULL;
      }
   }
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "ph=%p PTR_rel_QsciLexerFlagship    :    Object not created with new=true", ph ) );
      p->ph = NULL;
   }
}

void * hbqt_gcAllocate_QsciLexerFlagship( void * pObj, bool bNew )
{
   QGC_POINTER_QsciLexerFlagship * p = ( QGC_POINTER_QsciLexerFlagship * ) hb_gcAllocate( sizeof( QGC_POINTER_QsciLexerFlagship ), hbqt_gcFuncs() );

   new( & p->ph ) QPointer< QsciLexerFlagship >( ( QsciLexerFlagship * ) pObj );
   p->bNew = bNew;
   p->func = hbqt_gcRelease_QsciLexerFlagship;

   if( bNew )
   {
      HB_TRACE( HB_TR_DEBUG, ( "ph=%p    _new_QsciLexerFlagship  under p->pq", pObj ) );
   }
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "ph=%p NOT_new_QsciLexerFlagship", pObj ) );
   }
   return p;
}

HB_FUNC( QT_QSCILEXERFLAGSHIP )
{
   QsciLexerFlagship * pObj = NULL;

   if( hb_pcount() == 1 && HB_ISPOINTER( 1 ) )
   {
      pObj = new QsciLexerFlagship( hbqt_par_QObject( 1 ) ) ;
   }
   else if( hb_pcount() == 1 && HB_ISLOG( 1 ) )
   {
      pObj = new QsciLexerFlagship( 0, hb_parl( 1 ) ) ;
   }
   else if( hb_pcount() == 2 && HB_ISPOINTER( 1 ) && HB_ISLOG( 2 ) )
   {
      pObj = new QsciLexerFlagship( hbqt_par_QObject( 1 ), hb_parl( 2 ) ) ;
   }
   else
   {
      pObj = new QsciLexerFlagship() ;
   }

   hb_retptrGC( hbqt_gcAllocate_QsciLexerFlagship( ( void * ) pObj, true ) );
}

/*
 * const char * language () const
 */
HB_FUNC( QT_QSCILEXERFLAGSHIP_LANGUAGE )
{
   QsciLexerFlagship * p = hbqt_par_QsciLexerFlagship( 1 );
   if( p )
      hb_retc( ( p )->language() );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QSCILEXERFLAGSHIP_LANGUAGE FP=hb_retc( ( p )->language() ); p is NULL" ) );
   }
}

/*
 * const char * lexer () const
 */
HB_FUNC( QT_QSCILEXERFLAGSHIP_LEXER )
{
   QsciLexerFlagship * p = hbqt_par_QsciLexerFlagship( 1 );
   if( p )
      hb_retc( ( p )->lexer() );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QSCILEXERFLAGSHIP_LEXER FP=hb_retc( ( p )->lexer() ); p is NULL" ) );
   }
}

/*
 * QStringList autoCompletionWordSeparators () const
 */
HB_FUNC( QT_QSCILEXERFLAGSHIP_AUTOCOMPLETIONWORDSEPARATORS )
{
   QsciLexerFlagship * p = hbqt_par_QsciLexerFlagship( 1 );
   if( p )
      hb_retptrGC( hbqt_gcAllocate_QStringList( new QStringList( ( p )->autoCompletionWordSeparators() ), true ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QSCILEXERFLAGSHIP_AUTOCOMPLETIONWORDSEPARATORS FP=hb_retptrGC( hbqt_gcAllocate_QStringList( new QStringList( ( p )->autoCompletionWordSeparators() ), true ) ); p is NULL" ) );
   }
}

/*
 * const char * blockEnd (int *style=0) const
 */
HB_FUNC( QT_QSCILEXERFLAGSHIP_BLOCKEND )
{
   QsciLexerFlagship * p = hbqt_par_QsciLexerFlagship( 1 );
   int iStyle = 0;

   if( p )
      hb_retc( ( p )->blockEnd( &iStyle ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QSCILEXERFLAGSHIP_BLOCKEND FP=hb_retc( ( p )->blockEnd( &iStyle ) ); p is NULL" ) );
   }

   hb_storni( iStyle, 2 );
}

/*
 * const char * blockStart (int *style=0) const
 */
HB_FUNC( QT_QSCILEXERFLAGSHIP_BLOCKSTART )
{
   QsciLexerFlagship * p = hbqt_par_QsciLexerFlagship( 1 );
   int iStyle = 0;

   if( p )
      hb_retc( ( p )->blockStart( &iStyle ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QSCILEXERFLAGSHIP_BLOCKSTART FP=hb_retc( ( p )->blockStart( &iStyle ) ); p is NULL" ) );
   }

   hb_storni( iStyle, 2 );
}

/*
 * const char * blockStartKeyword (int *style=0) const
 */
HB_FUNC( QT_QSCILEXERFLAGSHIP_BLOCKSTARTKEYWORD )
{
   QsciLexerFlagship * p = hbqt_par_QsciLexerFlagship( 1 );
   int iStyle = 0;

   if( p )
      hb_retc( ( p )->blockStartKeyword( &iStyle ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QSCILEXERFLAGSHIP_BLOCKSTARTKEYWORD FP=hb_retc( ( p )->blockStartKeyword( &iStyle ) ); p is NULL" ) );
   }

   hb_storni( iStyle, 2 );
}

/*
 * int braceStyle () const
 */
HB_FUNC( QT_QSCILEXERFLAGSHIP_BRACESTYLE )
{
   QsciLexerFlagship * p = hbqt_par_QsciLexerFlagship( 1 );
   if( p )
      hb_retni( ( p )->braceStyle() );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QSCILEXERFLAGSHIP_BRACESTYLE FP=hb_retni( ( p )->braceStyle() ); p is NULL" ) );
   }
}

/*
 * const char * wordCharacters () const
 */
HB_FUNC( QT_QSCILEXERFLAGSHIP_WORDCHARACTERS )
{
   QsciLexerFlagship * p = hbqt_par_QsciLexerFlagship( 1 );
   if( p )
      hb_retc( ( p )->wordCharacters() );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QSCILEXERFLAGSHIP_WORDCHARACTERS FP=hb_retc( ( p )->wordCharacters() ); p is NULL" ) );
   }
}

/*
 * QColor defaultColor (int style) const
 */
HB_FUNC( QT_QSCILEXERFLAGSHIP_DEFAULTCOLOR )
{
   QsciLexerFlagship * p = hbqt_par_QsciLexerFlagship( 1 );
   if( p )
      hb_retptrGC( hbqt_gcAllocate_QColor( new QColor( ( p )->defaultColor( hb_parni( 2 ) ) ), true ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QSCILEXERFLAGSHIP_DEFAULTCOLOR FP=hb_retptrGC( hbqt_gcAllocate_QColor( new QColor( ( p )->defaultColor( hb_parni( 2 ) ) ), true ) ); p is NULL" ) );
   }
}

/*
 * bool defaultEolFill (int style) const
 */
HB_FUNC( QT_QSCILEXERFLAGSHIP_DEFAULTEOLFILL )
{
   QsciLexerFlagship * p = hbqt_par_QsciLexerFlagship( 1 );
   if( p )
      hb_retl( ( p )->defaultEolFill( hb_parni( 2 ) ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QSCILEXERFLAGSHIP_DEFAULTEOLFILL FP=hb_retl( ( p )->defaultEolFill( hb_parni( 2 ) ) ); p is NULL" ) );
   }
}

/*
 * QFont defaultFont (int style) const
 */
HB_FUNC( QT_QSCILEXERFLAGSHIP_DEFAULTFONT )
{
   QsciLexerFlagship * p = hbqt_par_QsciLexerFlagship( 1 );
   if( p )
      hb_retptrGC( hbqt_gcAllocate_QFont( new QFont( ( p )->defaultFont( hb_parni( 2 ) ) ), true ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QSCILEXERFLAGSHIP_DEFAULTFONT FP=hb_retptrGC( hbqt_gcAllocate_QFont( new QFont( ( p )->defaultFont( hb_parni( 2 ) ) ), true ) ); p is NULL" ) );
   }
}

/*
 * QColor defaultPaper (int style) const
 */
HB_FUNC( QT_QSCILEXERFLAGSHIP_DEFAULTPAPER )
{
   QsciLexerFlagship * p = hbqt_par_QsciLexerFlagship( 1 );
   if( p )
      hb_retptrGC( hbqt_gcAllocate_QColor( new QColor( ( p )->defaultPaper( hb_parni( 2 ) ) ), true ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QSCILEXERFLAGSHIP_DEFAULTPAPER FP=hb_retptrGC( hbqt_gcAllocate_QColor( new QColor( ( p )->defaultPaper( hb_parni( 2 ) ) ), true ) ); p is NULL" ) );
   }
}

/*
 * const char * keywords (int set) const
 */
HB_FUNC( QT_QSCILEXERFLAGSHIP_KEYWORDS )
{
   QsciLexerFlagship * p = hbqt_par_QsciLexerFlagship( 1 );
   if( p )
      hb_retc( ( p )->keywords( hb_parni( 2 ) ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QSCILEXERFLAGSHIP_KEYWORDS FP=hb_retc( ( p )->keywords( hb_parni( 2 ) ) ); p is NULL" ) );
   }
}

/*
 * QString description (int style) const
 */
HB_FUNC( QT_QSCILEXERFLAGSHIP_DESCRIPTION )
{
   QsciLexerFlagship * p = hbqt_par_QsciLexerFlagship( 1 );
   if( p )
      hb_retc( ( p )->description( hb_parni( 2 ) ).toAscii().data() );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QSCILEXERFLAGSHIP_DESCRIPTION FP=hb_retc( ( p )->description( hb_parni( 2 ) ).toAscii().data() ); p is NULL" ) );
   }
}

/*
 * void refreshProperties ()
 */
HB_FUNC( QT_QSCILEXERFLAGSHIP_REFRESHPROPERTIES )
{
   QsciLexerFlagship * p = hbqt_par_QsciLexerFlagship( 1 );
   if( p )
      ( p )->refreshProperties();
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QSCILEXERFLAGSHIP_REFRESHPROPERTIES FP=( p )->refreshProperties(); p is NULL" ) );
   }
}

/*
 * bool foldAtElse () const
 */
HB_FUNC( QT_QSCILEXERFLAGSHIP_FOLDATELSE )
{
   QsciLexerFlagship * p = hbqt_par_QsciLexerFlagship( 1 );
   if( p )
      hb_retl( ( p )->foldAtElse() );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QSCILEXERFLAGSHIP_FOLDATELSE FP=hb_retl( ( p )->foldAtElse() ); p is NULL" ) );
   }
}

/*
 * bool foldComments () const
 */
HB_FUNC( QT_QSCILEXERFLAGSHIP_FOLDCOMMENTS )
{
   QsciLexerFlagship * p = hbqt_par_QsciLexerFlagship( 1 );
   if( p )
      hb_retl( ( p )->foldComments() );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QSCILEXERFLAGSHIP_FOLDCOMMENTS FP=hb_retl( ( p )->foldComments() ); p is NULL" ) );
   }
}

/*
 * bool foldCompact () const
 */
HB_FUNC( QT_QSCILEXERFLAGSHIP_FOLDCOMPACT )
{
   QsciLexerFlagship * p = hbqt_par_QsciLexerFlagship( 1 );
   if( p )
      hb_retl( ( p )->foldCompact() );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QSCILEXERFLAGSHIP_FOLDCOMPACT FP=hb_retl( ( p )->foldCompact() ); p is NULL" ) );
   }
}

/*
 * bool foldPreprocessor () const
 */
HB_FUNC( QT_QSCILEXERFLAGSHIP_FOLDPREPROCESSOR )
{
   QsciLexerFlagship * p = hbqt_par_QsciLexerFlagship( 1 );
   if( p )
      hb_retl( ( p )->foldPreprocessor() );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QSCILEXERFLAGSHIP_FOLDPREPROCESSOR FP=hb_retl( ( p )->foldPreprocessor() ); p is NULL" ) );
   }
}

/*
 * bool stylePreprocessor () const
 */
HB_FUNC( QT_QSCILEXERFLAGSHIP_STYLEPREPROCESSOR )
{
   QsciLexerFlagship * p = hbqt_par_QsciLexerFlagship( 1 );
   if( p )
      hb_retl( ( p )->stylePreprocessor() );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QSCILEXERFLAGSHIP_STYLEPREPROCESSOR FP=hb_retl( ( p )->stylePreprocessor() ); p is NULL" ) );
   }
}

/*
 * void setDollarsAllowed (bool allowed)
 */
HB_FUNC( QT_QSCILEXERFLAGSHIP_SETDOLLARSALLOWED )
{
   QsciLexerFlagship * p = hbqt_par_QsciLexerFlagship( 1 );
   if( p )
      ( p )->setDollarsAllowed( hb_parl( 2 ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QSCILEXERFLAGSHIP_SETDOLLARSALLOWED FP=( p )->setDollarsAllowed( hb_parl( 2 ) ); p is NULL" ) );
   }
}

/*
 * bool dollarsAllowed () const
 */
HB_FUNC( QT_QSCILEXERFLAGSHIP_DOLLARSALLOWED )
{
   QsciLexerFlagship * p = hbqt_par_QsciLexerFlagship( 1 );
   if( p )
      hb_retl( ( p )->dollarsAllowed() );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QSCILEXERFLAGSHIP_DOLLARSALLOWED FP=hb_retl( ( p )->dollarsAllowed() ); p is NULL" ) );
   }
}

/*
 * virtual void setFoldAtElse (bool fold)
 */
HB_FUNC( QT_QSCILEXERFLAGSHIP_SETFOLDATELSE )
{
   QsciLexerFlagship * p = hbqt_par_QsciLexerFlagship( 1 );
   if( p )
      ( p )->setFoldAtElse( hb_parl( 2 ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QSCILEXERFLAGSHIP_SETFOLDATELSE FP=( p )->setFoldAtElse( hb_parl( 2 ) ); p is NULL" ) );
   }
}

/*
 * virtual void setFoldComments (bool fold)
 */
HB_FUNC( QT_QSCILEXERFLAGSHIP_SETFOLDCOMMENTS )
{
   QsciLexerFlagship * p = hbqt_par_QsciLexerFlagship( 1 );
   if( p )
      ( p )->setFoldComments( hb_parl( 2 ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QSCILEXERFLAGSHIP_SETFOLDCOMMENTS FP=( p )->setFoldComments( hb_parl( 2 ) ); p is NULL" ) );
   }
}

/*
 * virtual void setFoldCompact (bool fold)
 */
HB_FUNC( QT_QSCILEXERFLAGSHIP_SETFOLDCOMPACT )
{
   QsciLexerFlagship * p = hbqt_par_QsciLexerFlagship( 1 );
   if( p )
      ( p )->setFoldCompact( hb_parl( 2 ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QSCILEXERFLAGSHIP_SETFOLDCOMPACT FP=( p )->setFoldCompact( hb_parl( 2 ) ); p is NULL" ) );
   }
}

/*
 * virtual void setFoldPreprocessor (bool fold)
 */
HB_FUNC( QT_QSCILEXERFLAGSHIP_SETFOLDPREPROCESSOR )
{
   QsciLexerFlagship * p = hbqt_par_QsciLexerFlagship( 1 );
   if( p )
      ( p )->setFoldPreprocessor( hb_parl( 2 ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QSCILEXERFLAGSHIP_SETFOLDPREPROCESSOR FP=( p )->setFoldPreprocessor( hb_parl( 2 ) ); p is NULL" ) );
   }
}

/*
 * virtual void setStylePreprocessor (bool style)
 */
HB_FUNC( QT_QSCILEXERFLAGSHIP_SETSTYLEPREPROCESSOR )
{
   QsciLexerFlagship * p = hbqt_par_QsciLexerFlagship( 1 );
   if( p )
      ( p )->setStylePreprocessor( hb_parl( 2 ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QSCILEXERFLAGSHIP_SETSTYLEPREPROCESSOR FP=( p )->setStylePreprocessor( hb_parl( 2 ) ); p is NULL" ) );
   }
}


/*----------------------------------------------------------------------*/
#endif             /* #if QT_VERSION >= 0x040500 */
/*----------------------------------------------------------------------*/
