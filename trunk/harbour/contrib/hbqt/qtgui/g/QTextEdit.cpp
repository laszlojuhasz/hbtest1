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
 *  flags AutoFormatting
 *  enum AutoFormattingFlag { AutoNone, AutoBulletList, AutoAll }
 *  enum LineWrapMode { NoWrap, WidgetWidth, FixedPixelWidth, FixedColumnWidth }
 */

/*
 *  Constructed[ 81/82 [ 98.78% ] ]
 *
 *  *** Unconvered Prototypes ***
 *
 *  void setExtraSelections ( const QList<ExtraSelection> & selections )
 *
 *  *** Commented out protostypes ***
 *
 *  // QList<ExtraSelection> extraSelections () const
 */

#include <QtCore/QPointer>

#include <QtGui/QTextEdit>


/* QTextEdit ( QWidget * parent = 0 )
 * QTextEdit ( const QString & text, QWidget * parent = 0 )
 * virtual ~QTextEdit ()
 */

typedef struct
{
   QPointer< QTextEdit > ph;
   bool bNew;
   PHBQT_GC_FUNC func;
   int type;
} HBQT_GC_T_QTextEdit;

HBQT_GC_FUNC( hbqt_gcRelease_QTextEdit )
{
   QTextEdit  * ph = NULL ;
   HBQT_GC_T_QTextEdit * p = ( HBQT_GC_T_QTextEdit * ) Cargo;

   if( p && p->bNew && p->ph )
   {
      ph = p->ph;
      if( ph )
      {
         const QMetaObject * m = ( ph )->metaObject();
         if( ( QString ) m->className() != ( QString ) "QObject" )
         {
            HB_TRACE( HB_TR_DEBUG, ( "ph=%p %p YES_rel_QTextEdit   /.\\   ", (void*) ph, (void*) p->ph ) );
            delete ( p->ph );
            HB_TRACE( HB_TR_DEBUG, ( "ph=%p %p YES_rel_QTextEdit   \\./   ", (void*) ph, (void*) p->ph ) );
            p->ph = NULL;
         }
         else
         {
            HB_TRACE( HB_TR_DEBUG, ( "ph=%p NO__rel_QTextEdit          ", ph ) );
            p->ph = NULL;
         }
      }
      else
      {
         HB_TRACE( HB_TR_DEBUG, ( "ph=%p DEL_rel_QTextEdit    :     Object already deleted!", ph ) );
         p->ph = NULL;
      }
   }
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "ph=%p PTR_rel_QTextEdit    :    Object not created with new=true", ph ) );
      p->ph = NULL;
   }
}

void * hbqt_gcAllocate_QTextEdit( void * pObj, bool bNew )
{
   HBQT_GC_T_QTextEdit * p = ( HBQT_GC_T_QTextEdit * ) hb_gcAllocate( sizeof( HBQT_GC_T_QTextEdit ), hbqt_gcFuncs() );

   new( & p->ph ) QPointer< QTextEdit >( ( QTextEdit * ) pObj );
   p->bNew = bNew;
   p->func = hbqt_gcRelease_QTextEdit;
   p->type = HBQT_TYPE_QTextEdit;

   if( bNew )
   {
      HB_TRACE( HB_TR_DEBUG, ( "ph=%p    _new_QTextEdit  under p->pq", pObj ) );
   }
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "ph=%p NOT_new_QTextEdit", pObj ) );
   }
   return p;
}

HB_FUNC( QT_QTEXTEDIT )
{
   QTextEdit * pObj = NULL;

   if( hb_pcount() >= 1 && HB_ISCHAR( 1 ) )
      pObj = new QTextEdit( hbqt_par_QString( 1 ), hbqt_par_QWidget( 2 ) ) ;
   else
      pObj = new QTextEdit( hbqt_par_QWidget( 1 ) ) ;

   hb_retptrGC( hbqt_gcAllocate_QTextEdit( ( void * ) pObj, true ) );
}

/*
 * bool acceptRichText () const
 */
HB_FUNC( QT_QTEXTEDIT_ACCEPTRICHTEXT )
{
   QTextEdit * p = hbqt_par_QTextEdit( 1 );
   if( p )
   {
      hb_retl( ( p )->acceptRichText() );
   }
}

/*
 * Qt::Alignment alignment () const
 */
HB_FUNC( QT_QTEXTEDIT_ALIGNMENT )
{
   QTextEdit * p = hbqt_par_QTextEdit( 1 );
   if( p )
   {
      hb_retni( ( Qt::Alignment ) ( p )->alignment() );
   }
}

/*
 * QString anchorAt ( const QPoint & pos ) const
 */
HB_FUNC( QT_QTEXTEDIT_ANCHORAT )
{
   QTextEdit * p = hbqt_par_QTextEdit( 1 );
   if( p )
   {
      hb_retstr_utf8( ( p )->anchorAt( *hbqt_par_QPoint( 2 ) ).toUtf8().data() );
   }
}

/*
 * AutoFormatting autoFormatting () const
 */
HB_FUNC( QT_QTEXTEDIT_AUTOFORMATTING )
{
   QTextEdit * p = hbqt_par_QTextEdit( 1 );
   if( p )
   {
      hb_retni( ( QTextEdit::AutoFormatting ) ( p )->autoFormatting() );
   }
}

/*
 * bool canPaste () const
 */
HB_FUNC( QT_QTEXTEDIT_CANPASTE )
{
   QTextEdit * p = hbqt_par_QTextEdit( 1 );
   if( p )
   {
      hb_retl( ( p )->canPaste() );
   }
}

/*
 * QMenu * createStandardContextMenu ()
 */
HB_FUNC( QT_QTEXTEDIT_CREATESTANDARDCONTEXTMENU )
{
   QTextEdit * p = hbqt_par_QTextEdit( 1 );
   if( p )
   {
      hb_retptrGC( hbqt_gcAllocate_QMenu( ( p )->createStandardContextMenu(), false ) );
   }
}

/*
 * QMenu * createStandardContextMenu ( const QPoint & position )
 */
HB_FUNC( QT_QTEXTEDIT_CREATESTANDARDCONTEXTMENU_1 )
{
   QTextEdit * p = hbqt_par_QTextEdit( 1 );
   if( p )
   {
      hb_retptrGC( hbqt_gcAllocate_QMenu( ( p )->createStandardContextMenu( *hbqt_par_QPoint( 2 ) ), false ) );
   }
}

/*
 * QTextCharFormat currentCharFormat () const
 */
HB_FUNC( QT_QTEXTEDIT_CURRENTCHARFORMAT )
{
   QTextEdit * p = hbqt_par_QTextEdit( 1 );
   if( p )
   {
      hb_retptrGC( hbqt_gcAllocate_QTextCharFormat( new QTextCharFormat( ( p )->currentCharFormat() ), true ) );
   }
}

/*
 * QFont currentFont () const
 */
HB_FUNC( QT_QTEXTEDIT_CURRENTFONT )
{
   QTextEdit * p = hbqt_par_QTextEdit( 1 );
   if( p )
   {
      hb_retptrGC( hbqt_gcAllocate_QFont( new QFont( ( p )->currentFont() ), true ) );
   }
}

/*
 * QTextCursor cursorForPosition ( const QPoint & pos ) const
 */
HB_FUNC( QT_QTEXTEDIT_CURSORFORPOSITION )
{
   QTextEdit * p = hbqt_par_QTextEdit( 1 );
   if( p )
   {
      hb_retptrGC( hbqt_gcAllocate_QTextCursor( new QTextCursor( ( p )->cursorForPosition( *hbqt_par_QPoint( 2 ) ) ), true ) );
   }
}

/*
 * QRect cursorRect ( const QTextCursor & cursor ) const
 */
HB_FUNC( QT_QTEXTEDIT_CURSORRECT )
{
   QTextEdit * p = hbqt_par_QTextEdit( 1 );
   if( p )
   {
      hb_retptrGC( hbqt_gcAllocate_QRect( new QRect( ( p )->cursorRect( *hbqt_par_QTextCursor( 2 ) ) ), true ) );
   }
}

/*
 * QRect cursorRect () const
 */
HB_FUNC( QT_QTEXTEDIT_CURSORRECT_1 )
{
   QTextEdit * p = hbqt_par_QTextEdit( 1 );
   if( p )
   {
      hb_retptrGC( hbqt_gcAllocate_QRect( new QRect( ( p )->cursorRect() ), true ) );
   }
}

/*
 * int cursorWidth () const
 */
HB_FUNC( QT_QTEXTEDIT_CURSORWIDTH )
{
   QTextEdit * p = hbqt_par_QTextEdit( 1 );
   if( p )
   {
      hb_retni( ( p )->cursorWidth() );
   }
}

/*
 * QTextDocument * document () const
 */
HB_FUNC( QT_QTEXTEDIT_DOCUMENT )
{
   QTextEdit * p = hbqt_par_QTextEdit( 1 );
   if( p )
   {
      hb_retptrGC( hbqt_gcAllocate_QTextDocument( ( p )->document(), false ) );
   }
}

/*
 * QString documentTitle () const
 */
HB_FUNC( QT_QTEXTEDIT_DOCUMENTTITLE )
{
   QTextEdit * p = hbqt_par_QTextEdit( 1 );
   if( p )
   {
      hb_retstr_utf8( ( p )->documentTitle().toUtf8().data() );
   }
}

/*
 * void ensureCursorVisible ()
 */
HB_FUNC( QT_QTEXTEDIT_ENSURECURSORVISIBLE )
{
   QTextEdit * p = hbqt_par_QTextEdit( 1 );
   if( p )
   {
      ( p )->ensureCursorVisible();
   }
}

/*
 * bool find ( const QString & exp, QTextDocument::FindFlags options = 0 )
 */
HB_FUNC( QT_QTEXTEDIT_FIND )
{
   QTextEdit * p = hbqt_par_QTextEdit( 1 );
   if( p )
   {
      void * pText;
      hb_retl( ( p )->find( hb_parstr_utf8( 2, &pText, NULL ), ( QTextDocument::FindFlags ) hb_parni( 3 ) ) );
      hb_strfree( pText );
   }
}

/*
 * QString fontFamily () const
 */
HB_FUNC( QT_QTEXTEDIT_FONTFAMILY )
{
   QTextEdit * p = hbqt_par_QTextEdit( 1 );
   if( p )
   {
      hb_retstr_utf8( ( p )->fontFamily().toUtf8().data() );
   }
}

/*
 * bool fontItalic () const
 */
HB_FUNC( QT_QTEXTEDIT_FONTITALIC )
{
   QTextEdit * p = hbqt_par_QTextEdit( 1 );
   if( p )
   {
      hb_retl( ( p )->fontItalic() );
   }
}

/*
 * qreal fontPointSize () const
 */
HB_FUNC( QT_QTEXTEDIT_FONTPOINTSIZE )
{
   QTextEdit * p = hbqt_par_QTextEdit( 1 );
   if( p )
   {
      hb_retnd( ( p )->fontPointSize() );
   }
}

/*
 * bool fontUnderline () const
 */
HB_FUNC( QT_QTEXTEDIT_FONTUNDERLINE )
{
   QTextEdit * p = hbqt_par_QTextEdit( 1 );
   if( p )
   {
      hb_retl( ( p )->fontUnderline() );
   }
}

/*
 * int fontWeight () const
 */
HB_FUNC( QT_QTEXTEDIT_FONTWEIGHT )
{
   QTextEdit * p = hbqt_par_QTextEdit( 1 );
   if( p )
   {
      hb_retni( ( p )->fontWeight() );
   }
}

/*
 * bool isReadOnly () const
 */
HB_FUNC( QT_QTEXTEDIT_ISREADONLY )
{
   QTextEdit * p = hbqt_par_QTextEdit( 1 );
   if( p )
   {
      hb_retl( ( p )->isReadOnly() );
   }
}

/*
 * bool isUndoRedoEnabled () const
 */
HB_FUNC( QT_QTEXTEDIT_ISUNDOREDOENABLED )
{
   QTextEdit * p = hbqt_par_QTextEdit( 1 );
   if( p )
   {
      hb_retl( ( p )->isUndoRedoEnabled() );
   }
}

/*
 * int lineWrapColumnOrWidth () const
 */
HB_FUNC( QT_QTEXTEDIT_LINEWRAPCOLUMNORWIDTH )
{
   QTextEdit * p = hbqt_par_QTextEdit( 1 );
   if( p )
   {
      hb_retni( ( p )->lineWrapColumnOrWidth() );
   }
}

/*
 * LineWrapMode lineWrapMode () const
 */
HB_FUNC( QT_QTEXTEDIT_LINEWRAPMODE )
{
   QTextEdit * p = hbqt_par_QTextEdit( 1 );
   if( p )
   {
      hb_retni( ( QTextEdit::LineWrapMode ) ( p )->lineWrapMode() );
   }
}

/*
 * virtual QVariant loadResource ( int type, const QUrl & name )
 */
HB_FUNC( QT_QTEXTEDIT_LOADRESOURCE )
{
   QTextEdit * p = hbqt_par_QTextEdit( 1 );
   if( p )
   {
      hb_retptrGC( hbqt_gcAllocate_QVariant( new QVariant( ( p )->loadResource( hb_parni( 2 ), *hbqt_par_QUrl( 3 ) ) ), true ) );
   }
}

/*
 * void mergeCurrentCharFormat ( const QTextCharFormat & modifier )
 */
HB_FUNC( QT_QTEXTEDIT_MERGECURRENTCHARFORMAT )
{
   QTextEdit * p = hbqt_par_QTextEdit( 1 );
   if( p )
   {
      ( p )->mergeCurrentCharFormat( *hbqt_par_QTextCharFormat( 2 ) );
   }
}

/*
 * void moveCursor ( QTextCursor::MoveOperation operation, QTextCursor::MoveMode mode = QTextCursor::MoveAnchor )
 */
HB_FUNC( QT_QTEXTEDIT_MOVECURSOR )
{
   QTextEdit * p = hbqt_par_QTextEdit( 1 );
   if( p )
   {
      ( p )->moveCursor( ( QTextCursor::MoveOperation ) hb_parni( 2 ), ( HB_ISNUM( 3 ) ? ( QTextCursor::MoveMode ) hb_parni( 3 ) : ( QTextCursor::MoveMode ) QTextCursor::MoveAnchor ) );
   }
}

/*
 * bool overwriteMode () const
 */
HB_FUNC( QT_QTEXTEDIT_OVERWRITEMODE )
{
   QTextEdit * p = hbqt_par_QTextEdit( 1 );
   if( p )
   {
      hb_retl( ( p )->overwriteMode() );
   }
}

/*
 * void print ( QPrinter * printer ) const
 */
HB_FUNC( QT_QTEXTEDIT_PRINT )
{
   QTextEdit * p = hbqt_par_QTextEdit( 1 );
   if( p )
   {
      ( p )->print( hbqt_par_QPrinter( 2 ) );
   }
}

/*
 * void setAcceptRichText ( bool accept )
 */
HB_FUNC( QT_QTEXTEDIT_SETACCEPTRICHTEXT )
{
   QTextEdit * p = hbqt_par_QTextEdit( 1 );
   if( p )
   {
      ( p )->setAcceptRichText( hb_parl( 2 ) );
   }
}

/*
 * void setAutoFormatting ( AutoFormatting features )
 */
HB_FUNC( QT_QTEXTEDIT_SETAUTOFORMATTING )
{
   QTextEdit * p = hbqt_par_QTextEdit( 1 );
   if( p )
   {
      ( p )->setAutoFormatting( ( QTextEdit::AutoFormatting ) hb_parni( 2 ) );
   }
}

/*
 * void setCurrentCharFormat ( const QTextCharFormat & format )
 */
HB_FUNC( QT_QTEXTEDIT_SETCURRENTCHARFORMAT )
{
   QTextEdit * p = hbqt_par_QTextEdit( 1 );
   if( p )
   {
      ( p )->setCurrentCharFormat( *hbqt_par_QTextCharFormat( 2 ) );
   }
}

/*
 * void setCursorWidth ( int width )
 */
HB_FUNC( QT_QTEXTEDIT_SETCURSORWIDTH )
{
   QTextEdit * p = hbqt_par_QTextEdit( 1 );
   if( p )
   {
      ( p )->setCursorWidth( hb_parni( 2 ) );
   }
}

/*
 * void setDocument ( QTextDocument * document )
 */
HB_FUNC( QT_QTEXTEDIT_SETDOCUMENT )
{
   QTextEdit * p = hbqt_par_QTextEdit( 1 );
   if( p )
   {
      ( p )->setDocument( hbqt_par_QTextDocument( 2 ) );
   }
}

/*
 * void setDocumentTitle ( const QString & title )
 */
HB_FUNC( QT_QTEXTEDIT_SETDOCUMENTTITLE )
{
   QTextEdit * p = hbqt_par_QTextEdit( 1 );
   if( p )
   {
      void * pText;
      ( p )->setDocumentTitle( hb_parstr_utf8( 2, &pText, NULL ) );
      hb_strfree( pText );
   }
}

/*
 * void setLineWrapColumnOrWidth ( int w )
 */
HB_FUNC( QT_QTEXTEDIT_SETLINEWRAPCOLUMNORWIDTH )
{
   QTextEdit * p = hbqt_par_QTextEdit( 1 );
   if( p )
   {
      ( p )->setLineWrapColumnOrWidth( hb_parni( 2 ) );
   }
}

/*
 * void setLineWrapMode ( LineWrapMode mode )
 */
HB_FUNC( QT_QTEXTEDIT_SETLINEWRAPMODE )
{
   QTextEdit * p = hbqt_par_QTextEdit( 1 );
   if( p )
   {
      ( p )->setLineWrapMode( ( QTextEdit::LineWrapMode ) hb_parni( 2 ) );
   }
}

/*
 * void setOverwriteMode ( bool overwrite )
 */
HB_FUNC( QT_QTEXTEDIT_SETOVERWRITEMODE )
{
   QTextEdit * p = hbqt_par_QTextEdit( 1 );
   if( p )
   {
      ( p )->setOverwriteMode( hb_parl( 2 ) );
   }
}

/*
 * void setReadOnly ( bool ro )
 */
HB_FUNC( QT_QTEXTEDIT_SETREADONLY )
{
   QTextEdit * p = hbqt_par_QTextEdit( 1 );
   if( p )
   {
      ( p )->setReadOnly( hb_parl( 2 ) );
   }
}

/*
 * void setTabChangesFocus ( bool b )
 */
HB_FUNC( QT_QTEXTEDIT_SETTABCHANGESFOCUS )
{
   QTextEdit * p = hbqt_par_QTextEdit( 1 );
   if( p )
   {
      ( p )->setTabChangesFocus( hb_parl( 2 ) );
   }
}

/*
 * void setTabStopWidth ( int width )
 */
HB_FUNC( QT_QTEXTEDIT_SETTABSTOPWIDTH )
{
   QTextEdit * p = hbqt_par_QTextEdit( 1 );
   if( p )
   {
      ( p )->setTabStopWidth( hb_parni( 2 ) );
   }
}

/*
 * void setTextCursor ( const QTextCursor & cursor )
 */
HB_FUNC( QT_QTEXTEDIT_SETTEXTCURSOR )
{
   QTextEdit * p = hbqt_par_QTextEdit( 1 );
   if( p )
   {
      ( p )->setTextCursor( *hbqt_par_QTextCursor( 2 ) );
   }
}

/*
 * void setTextInteractionFlags ( Qt::TextInteractionFlags flags )
 */
HB_FUNC( QT_QTEXTEDIT_SETTEXTINTERACTIONFLAGS )
{
   QTextEdit * p = hbqt_par_QTextEdit( 1 );
   if( p )
   {
      ( p )->setTextInteractionFlags( ( Qt::TextInteractionFlags ) hb_parni( 2 ) );
   }
}

/*
 * void setUndoRedoEnabled ( bool enable )
 */
HB_FUNC( QT_QTEXTEDIT_SETUNDOREDOENABLED )
{
   QTextEdit * p = hbqt_par_QTextEdit( 1 );
   if( p )
   {
      ( p )->setUndoRedoEnabled( hb_parl( 2 ) );
   }
}

/*
 * void setWordWrapMode ( QTextOption::WrapMode policy )
 */
HB_FUNC( QT_QTEXTEDIT_SETWORDWRAPMODE )
{
   QTextEdit * p = hbqt_par_QTextEdit( 1 );
   if( p )
   {
      ( p )->setWordWrapMode( ( QTextOption::WrapMode ) hb_parni( 2 ) );
   }
}

/*
 * bool tabChangesFocus () const
 */
HB_FUNC( QT_QTEXTEDIT_TABCHANGESFOCUS )
{
   QTextEdit * p = hbqt_par_QTextEdit( 1 );
   if( p )
   {
      hb_retl( ( p )->tabChangesFocus() );
   }
}

/*
 * int tabStopWidth () const
 */
HB_FUNC( QT_QTEXTEDIT_TABSTOPWIDTH )
{
   QTextEdit * p = hbqt_par_QTextEdit( 1 );
   if( p )
   {
      hb_retni( ( p )->tabStopWidth() );
   }
}

/*
 * QColor textBackgroundColor () const
 */
HB_FUNC( QT_QTEXTEDIT_TEXTBACKGROUNDCOLOR )
{
   QTextEdit * p = hbqt_par_QTextEdit( 1 );
   if( p )
   {
      hb_retptrGC( hbqt_gcAllocate_QColor( new QColor( ( p )->textBackgroundColor() ), true ) );
   }
}

/*
 * QColor textColor () const
 */
HB_FUNC( QT_QTEXTEDIT_TEXTCOLOR )
{
   QTextEdit * p = hbqt_par_QTextEdit( 1 );
   if( p )
   {
      hb_retptrGC( hbqt_gcAllocate_QColor( new QColor( ( p )->textColor() ), true ) );
   }
}

/*
 * QTextCursor textCursor () const
 */
HB_FUNC( QT_QTEXTEDIT_TEXTCURSOR )
{
   QTextEdit * p = hbqt_par_QTextEdit( 1 );
   if( p )
   {
      hb_retptrGC( hbqt_gcAllocate_QTextCursor( new QTextCursor( ( p )->textCursor() ), true ) );
   }
}

/*
 * Qt::TextInteractionFlags textInteractionFlags () const
 */
HB_FUNC( QT_QTEXTEDIT_TEXTINTERACTIONFLAGS )
{
   QTextEdit * p = hbqt_par_QTextEdit( 1 );
   if( p )
   {
      hb_retni( ( Qt::TextInteractionFlags ) ( p )->textInteractionFlags() );
   }
}

/*
 * QString toHtml () const
 */
HB_FUNC( QT_QTEXTEDIT_TOHTML )
{
   QTextEdit * p = hbqt_par_QTextEdit( 1 );
   if( p )
   {
      hb_retstr_utf8( ( p )->toHtml().toUtf8().data() );
   }
}

/*
 * QString toPlainText () const
 */
HB_FUNC( QT_QTEXTEDIT_TOPLAINTEXT )
{
   QTextEdit * p = hbqt_par_QTextEdit( 1 );
   if( p )
   {
      hb_retstr_utf8( ( p )->toPlainText().toUtf8().data() );
   }
}

/*
 * QTextOption::WrapMode wordWrapMode () const
 */
HB_FUNC( QT_QTEXTEDIT_WORDWRAPMODE )
{
   QTextEdit * p = hbqt_par_QTextEdit( 1 );
   if( p )
   {
      hb_retni( ( QTextOption::WrapMode ) ( p )->wordWrapMode() );
   }
}

/*
 * void append ( const QString & text )
 */
HB_FUNC( QT_QTEXTEDIT_APPEND )
{
   QTextEdit * p = hbqt_par_QTextEdit( 1 );
   if( p )
   {
      void * pText;
      ( p )->append( hb_parstr_utf8( 2, &pText, NULL ) );
      hb_strfree( pText );
   }
}

/*
 * void clear ()
 */
HB_FUNC( QT_QTEXTEDIT_CLEAR )
{
   QTextEdit * p = hbqt_par_QTextEdit( 1 );
   if( p )
   {
      ( p )->clear();
   }
}

/*
 * void copy ()
 */
HB_FUNC( QT_QTEXTEDIT_COPY )
{
   QTextEdit * p = hbqt_par_QTextEdit( 1 );
   if( p )
   {
      ( p )->copy();
   }
}

/*
 * void cut ()
 */
HB_FUNC( QT_QTEXTEDIT_CUT )
{
   QTextEdit * p = hbqt_par_QTextEdit( 1 );
   if( p )
   {
      ( p )->cut();
   }
}

/*
 * void insertHtml ( const QString & text )
 */
HB_FUNC( QT_QTEXTEDIT_INSERTHTML )
{
   QTextEdit * p = hbqt_par_QTextEdit( 1 );
   if( p )
   {
      void * pText;
      ( p )->insertHtml( hb_parstr_utf8( 2, &pText, NULL ) );
      hb_strfree( pText );
   }
}

/*
 * void insertPlainText ( const QString & text )
 */
HB_FUNC( QT_QTEXTEDIT_INSERTPLAINTEXT )
{
   QTextEdit * p = hbqt_par_QTextEdit( 1 );
   if( p )
   {
      void * pText;
      ( p )->insertPlainText( hb_parstr_utf8( 2, &pText, NULL ) );
      hb_strfree( pText );
   }
}

/*
 * void paste ()
 */
HB_FUNC( QT_QTEXTEDIT_PASTE )
{
   QTextEdit * p = hbqt_par_QTextEdit( 1 );
   if( p )
   {
      ( p )->paste();
   }
}

/*
 * void redo ()
 */
HB_FUNC( QT_QTEXTEDIT_REDO )
{
   QTextEdit * p = hbqt_par_QTextEdit( 1 );
   if( p )
   {
      ( p )->redo();
   }
}

/*
 * void scrollToAnchor ( const QString & name )
 */
HB_FUNC( QT_QTEXTEDIT_SCROLLTOANCHOR )
{
   QTextEdit * p = hbqt_par_QTextEdit( 1 );
   if( p )
   {
      void * pText;
      ( p )->scrollToAnchor( hb_parstr_utf8( 2, &pText, NULL ) );
      hb_strfree( pText );
   }
}

/*
 * void selectAll ()
 */
HB_FUNC( QT_QTEXTEDIT_SELECTALL )
{
   QTextEdit * p = hbqt_par_QTextEdit( 1 );
   if( p )
   {
      ( p )->selectAll();
   }
}

/*
 * void setAlignment ( Qt::Alignment a )
 */
HB_FUNC( QT_QTEXTEDIT_SETALIGNMENT )
{
   QTextEdit * p = hbqt_par_QTextEdit( 1 );
   if( p )
   {
      ( p )->setAlignment( ( Qt::Alignment ) hb_parni( 2 ) );
   }
}

/*
 * void setCurrentFont ( const QFont & f )
 */
HB_FUNC( QT_QTEXTEDIT_SETCURRENTFONT )
{
   QTextEdit * p = hbqt_par_QTextEdit( 1 );
   if( p )
   {
      ( p )->setCurrentFont( *hbqt_par_QFont( 2 ) );
   }
}

/*
 * void setFontFamily ( const QString & fontFamily )
 */
HB_FUNC( QT_QTEXTEDIT_SETFONTFAMILY )
{
   QTextEdit * p = hbqt_par_QTextEdit( 1 );
   if( p )
   {
      void * pText;
      ( p )->setFontFamily( hb_parstr_utf8( 2, &pText, NULL ) );
      hb_strfree( pText );
   }
}

/*
 * void setFontItalic ( bool italic )
 */
HB_FUNC( QT_QTEXTEDIT_SETFONTITALIC )
{
   QTextEdit * p = hbqt_par_QTextEdit( 1 );
   if( p )
   {
      ( p )->setFontItalic( hb_parl( 2 ) );
   }
}

/*
 * void setFontPointSize ( qreal s )
 */
HB_FUNC( QT_QTEXTEDIT_SETFONTPOINTSIZE )
{
   QTextEdit * p = hbqt_par_QTextEdit( 1 );
   if( p )
   {
      ( p )->setFontPointSize( hb_parnd( 2 ) );
   }
}

/*
 * void setFontUnderline ( bool underline )
 */
HB_FUNC( QT_QTEXTEDIT_SETFONTUNDERLINE )
{
   QTextEdit * p = hbqt_par_QTextEdit( 1 );
   if( p )
   {
      ( p )->setFontUnderline( hb_parl( 2 ) );
   }
}

/*
 * void setFontWeight ( int weight )
 */
HB_FUNC( QT_QTEXTEDIT_SETFONTWEIGHT )
{
   QTextEdit * p = hbqt_par_QTextEdit( 1 );
   if( p )
   {
      ( p )->setFontWeight( hb_parni( 2 ) );
   }
}

/*
 * void setHtml ( const QString & text )
 */
HB_FUNC( QT_QTEXTEDIT_SETHTML )
{
   QTextEdit * p = hbqt_par_QTextEdit( 1 );
   if( p )
   {
      void * pText;
      ( p )->setHtml( hb_parstr_utf8( 2, &pText, NULL ) );
      hb_strfree( pText );
   }
}

/*
 * void setPlainText ( const QString & text )
 */
HB_FUNC( QT_QTEXTEDIT_SETPLAINTEXT )
{
   QTextEdit * p = hbqt_par_QTextEdit( 1 );
   if( p )
   {
      void * pText;
      ( p )->setPlainText( hb_parstr_utf8( 2, &pText, NULL ) );
      hb_strfree( pText );
   }
}

/*
 * void setText ( const QString & text )
 */
HB_FUNC( QT_QTEXTEDIT_SETTEXT )
{
   QTextEdit * p = hbqt_par_QTextEdit( 1 );
   if( p )
   {
      void * pText;
      ( p )->setText( hb_parstr_utf8( 2, &pText, NULL ) );
      hb_strfree( pText );
   }
}

/*
 * void setTextBackgroundColor ( const QColor & c )
 */
HB_FUNC( QT_QTEXTEDIT_SETTEXTBACKGROUNDCOLOR )
{
   QTextEdit * p = hbqt_par_QTextEdit( 1 );
   if( p )
   {
      ( p )->setTextBackgroundColor( *hbqt_par_QColor( 2 ) );
   }
}

/*
 * void setTextColor ( const QColor & c )
 */
HB_FUNC( QT_QTEXTEDIT_SETTEXTCOLOR )
{
   QTextEdit * p = hbqt_par_QTextEdit( 1 );
   if( p )
   {
      ( p )->setTextColor( *hbqt_par_QColor( 2 ) );
   }
}

/*
 * void undo ()
 */
HB_FUNC( QT_QTEXTEDIT_UNDO )
{
   QTextEdit * p = hbqt_par_QTextEdit( 1 );
   if( p )
   {
      ( p )->undo();
   }
}

/*
 * void zoomIn ( int range = 1 )
 */
HB_FUNC( QT_QTEXTEDIT_ZOOMIN )
{
   QTextEdit * p = hbqt_par_QTextEdit( 1 );
   if( p )
   {
      ( p )->zoomIn( hb_parnidef( 2, 1 ) );
   }
}

/*
 * void zoomOut ( int range = 1 )
 */
HB_FUNC( QT_QTEXTEDIT_ZOOMOUT )
{
   QTextEdit * p = hbqt_par_QTextEdit( 1 );
   if( p )
   {
      ( p )->zoomOut( hb_parnidef( 2, 1 ) );
   }
}


/*----------------------------------------------------------------------*/
#endif             /* #if QT_VERSION >= 0x040500 */
/*----------------------------------------------------------------------*/
