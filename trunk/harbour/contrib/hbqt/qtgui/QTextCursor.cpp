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
 *  enum MoveMode { MoveAnchor, KeepAnchor }
 *  enum MoveOperation { NoMove, Start, StartOfLine, StartOfBlock, ..., PreviousRow }
 *  enum SelectionType { Document, BlockUnderCursor, LineUnderCursor, WordUnderCursor }
 */

#include <QtCore/QPointer>

#include <QtGui/QTextBlock>
#include <QtGui/QTextCursor>
#include <QtGui/QTextDocumentFragment>


/*
 * QTextCursor ()
 * QTextCursor ( QTextDocument * document )
 * QTextCursor ( QTextFrame * frame )
 * QTextCursor ( const QTextBlock & block )
 * QTextCursor ( const QTextCursor & cursor )
 * ~QTextCursor ()
 */

typedef struct
{
   void * ph;
   bool bNew;
   QT_G_FUNC_PTR func;
} QGC_POINTER_QTextCursor;

QT_G_FUNC( hbqt_gcRelease_QTextCursor )
{
   QGC_POINTER * p = ( QGC_POINTER * ) Cargo;

   if( p && p->bNew )
   {
      if( p->ph )
      {
         HB_TRACE( HB_TR_DEBUG, ( "YES_rel_QTextCursor   /.\\    ph=%p", p->ph ) );
         delete ( ( QTextCursor * ) p->ph );
         HB_TRACE( HB_TR_DEBUG, ( "YES_rel_QTextCursor   \\./    ph=%p", p->ph ) );
         p->ph = NULL;
      }
      else
      {
         HB_TRACE( HB_TR_DEBUG, ( "DEL_rel_QTextCursor    :     Object already deleted!" ) );
      }
   }
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "PTR_rel_QTextCursor    :    Object not created with new()" ) );
      p->ph = NULL;
   }
}

void * hbqt_gcAllocate_QTextCursor( void * pObj, bool bNew )
{
   QGC_POINTER * p = ( QGC_POINTER * ) hb_gcAllocate( sizeof( QGC_POINTER ), hbqt_gcFuncs() );

   p->ph = pObj;
   p->bNew = bNew;
   p->func = hbqt_gcRelease_QTextCursor;

   if( bNew )
   {
      HB_TRACE( HB_TR_DEBUG, ( "   _new_QTextCursor                ph=%p %i B %i KB", pObj, ( int ) hb_xquery( 1001 ), hbqt_getmemused() ) );
   }
   return p;
}

HB_FUNC( QT_QTEXTCURSOR )
{
   void * pObj = NULL;

   if( hb_pcount() == 1 && HB_ISPOINTER( 1 ) )
   {
      pObj = ( QTextCursor* ) new QTextCursor( *hbqt_par_QTextCursor( 1 ) ) ;
   }
   else if( hb_pcount() == 2 && HB_ISCHAR( 1 ) && HB_ISPOINTER( 2 ) )
   {
      QString object = hbqt_par_QString( 1 );

      if( object == ( QString ) "QTextDocument" )
      {
         pObj = ( QTextCursor* ) new QTextCursor( hbqt_par_QTextDocument( 2 ) ) ;
      }
      if( object == ( QString ) "QTextBlock" )
      {
         pObj = ( QTextCursor* ) new QTextCursor( *hbqt_par_QTextBlock( 2 ) ) ;
      }
      if( object == ( QString ) "QTextFrame" )
      {
         pObj = ( QTextCursor* ) new QTextCursor( hbqt_par_QTextFrame( 2 ) ) ;
      }
      else
      {
         pObj = ( QTextCursor* ) new QTextCursor() ;
      }
   }
   else
   {
      pObj = ( QTextCursor* ) new QTextCursor() ;
   }

   hb_retptrGC( hbqt_gcAllocate_QTextCursor( pObj, true ) );
}

/*
 * int anchor () const
 */
HB_FUNC( QT_QTEXTCURSOR_ANCHOR )
{
   hb_retni( hbqt_par_QTextCursor( 1 )->anchor() );
}

/*
 * bool atBlockEnd () const
 */
HB_FUNC( QT_QTEXTCURSOR_ATBLOCKEND )
{
   hb_retl( hbqt_par_QTextCursor( 1 )->atBlockEnd() );
}

/*
 * bool atBlockStart () const
 */
HB_FUNC( QT_QTEXTCURSOR_ATBLOCKSTART )
{
   hb_retl( hbqt_par_QTextCursor( 1 )->atBlockStart() );
}

/*
 * bool atEnd () const
 */
HB_FUNC( QT_QTEXTCURSOR_ATEND )
{
   hb_retl( hbqt_par_QTextCursor( 1 )->atEnd() );
}

/*
 * bool atStart () const
 */
HB_FUNC( QT_QTEXTCURSOR_ATSTART )
{
   hb_retl( hbqt_par_QTextCursor( 1 )->atStart() );
}

/*
 * void beginEditBlock ()
 */
HB_FUNC( QT_QTEXTCURSOR_BEGINEDITBLOCK )
{
   hbqt_par_QTextCursor( 1 )->beginEditBlock();
}

/*
 * QTextBlock block () const
 */
HB_FUNC( QT_QTEXTCURSOR_BLOCK )
{
   hb_retptrGC( hbqt_gcAllocate_QTextBlock( new QTextBlock( hbqt_par_QTextCursor( 1 )->block() ), true ) );
}

/*
 * QTextCharFormat blockCharFormat () const
 */
HB_FUNC( QT_QTEXTCURSOR_BLOCKCHARFORMAT )
{
   hb_retptrGC( hbqt_gcAllocate_QTextCharFormat( new QTextCharFormat( hbqt_par_QTextCursor( 1 )->blockCharFormat() ), true ) );
}

/*
 * QTextBlockFormat blockFormat () const
 */
HB_FUNC( QT_QTEXTCURSOR_BLOCKFORMAT )
{
   hb_retptrGC( hbqt_gcAllocate_QTextBlockFormat( new QTextBlockFormat( hbqt_par_QTextCursor( 1 )->blockFormat() ), true ) );
}

/*
 * int blockNumber () const
 */
HB_FUNC( QT_QTEXTCURSOR_BLOCKNUMBER )
{
   hb_retni( hbqt_par_QTextCursor( 1 )->blockNumber() );
}

/*
 * QTextCharFormat charFormat () const
 */
HB_FUNC( QT_QTEXTCURSOR_CHARFORMAT )
{
   hb_retptrGC( hbqt_gcAllocate_QTextCharFormat( new QTextCharFormat( hbqt_par_QTextCursor( 1 )->charFormat() ), true ) );
}

/*
 * void clearSelection ()
 */
HB_FUNC( QT_QTEXTCURSOR_CLEARSELECTION )
{
   hbqt_par_QTextCursor( 1 )->clearSelection();
}

/*
 * int columnNumber () const
 */
HB_FUNC( QT_QTEXTCURSOR_COLUMNNUMBER )
{
   hb_retni( hbqt_par_QTextCursor( 1 )->columnNumber() );
}

/*
 * QTextList * createList ( const QTextListFormat & format )
 */
HB_FUNC( QT_QTEXTCURSOR_CREATELIST )
{
   hb_retptrGC( hbqt_gcAllocate_QTextList( hbqt_par_QTextCursor( 1 )->createList( *hbqt_par_QTextListFormat( 2 ) ), false ) );
}

/*
 * QTextList * createList ( QTextListFormat::Style style )
 */
HB_FUNC( QT_QTEXTCURSOR_CREATELIST_1 )
{
   hb_retptrGC( hbqt_gcAllocate_QTextList( hbqt_par_QTextCursor( 1 )->createList( ( QTextListFormat::Style ) hb_parni( 2 ) ), false ) );
}

/*
 * QTextFrame * currentFrame () const
 */
HB_FUNC( QT_QTEXTCURSOR_CURRENTFRAME )
{
   hb_retptrGC( hbqt_gcAllocate_QTextFrame( hbqt_par_QTextCursor( 1 )->currentFrame(), false ) );
}

/*
 * QTextList * currentList () const
 */
HB_FUNC( QT_QTEXTCURSOR_CURRENTLIST )
{
   hb_retptrGC( hbqt_gcAllocate_QTextList( hbqt_par_QTextCursor( 1 )->currentList(), false ) );
}

/*
 * void deleteChar ()
 */
HB_FUNC( QT_QTEXTCURSOR_DELETECHAR )
{
   hbqt_par_QTextCursor( 1 )->deleteChar();
}

/*
 * void deletePreviousChar ()
 */
HB_FUNC( QT_QTEXTCURSOR_DELETEPREVIOUSCHAR )
{
   hbqt_par_QTextCursor( 1 )->deletePreviousChar();
}

/*
 * QTextDocument * document () const
 */
HB_FUNC( QT_QTEXTCURSOR_DOCUMENT )
{
   hb_retptrGC( hbqt_gcAllocate_QTextDocument( hbqt_par_QTextCursor( 1 )->document(), false ) );
}

/*
 * void endEditBlock ()
 */
HB_FUNC( QT_QTEXTCURSOR_ENDEDITBLOCK )
{
   hbqt_par_QTextCursor( 1 )->endEditBlock();
}

/*
 * bool hasComplexSelection () const
 */
HB_FUNC( QT_QTEXTCURSOR_HASCOMPLEXSELECTION )
{
   hb_retl( hbqt_par_QTextCursor( 1 )->hasComplexSelection() );
}

/*
 * bool hasSelection () const
 */
HB_FUNC( QT_QTEXTCURSOR_HASSELECTION )
{
   hb_retl( hbqt_par_QTextCursor( 1 )->hasSelection() );
}

/*
 * void insertBlock ()
 */
HB_FUNC( QT_QTEXTCURSOR_INSERTBLOCK )
{
   hbqt_par_QTextCursor( 1 )->insertBlock();
}

/*
 * void insertBlock ( const QTextBlockFormat & format )
 */
HB_FUNC( QT_QTEXTCURSOR_INSERTBLOCK_1 )
{
   hbqt_par_QTextCursor( 1 )->insertBlock( *hbqt_par_QTextBlockFormat( 2 ) );
}

/*
 * void insertBlock ( const QTextBlockFormat & format, const QTextCharFormat & charFormat )
 */
HB_FUNC( QT_QTEXTCURSOR_INSERTBLOCK_2 )
{
   hbqt_par_QTextCursor( 1 )->insertBlock( *hbqt_par_QTextBlockFormat( 2 ), *hbqt_par_QTextCharFormat( 3 ) );
}

/*
 * void insertFragment ( const QTextDocumentFragment & fragment )
 */
HB_FUNC( QT_QTEXTCURSOR_INSERTFRAGMENT )
{
   hbqt_par_QTextCursor( 1 )->insertFragment( *hbqt_par_QTextDocumentFragment( 2 ) );
}

/*
 * QTextFrame * insertFrame ( const QTextFrameFormat & format )
 */
HB_FUNC( QT_QTEXTCURSOR_INSERTFRAME )
{
   hb_retptrGC( hbqt_gcAllocate_QTextFrame( hbqt_par_QTextCursor( 1 )->insertFrame( *hbqt_par_QTextFrameFormat( 2 ) ), false ) );
}

/*
 * void insertHtml ( const QString & html )
 */
HB_FUNC( QT_QTEXTCURSOR_INSERTHTML )
{
   hbqt_par_QTextCursor( 1 )->insertHtml( hbqt_par_QString( 2 ) );
}

/*
 * void insertImage ( const QString & name )
 */
HB_FUNC( QT_QTEXTCURSOR_INSERTIMAGE )
{
   hbqt_par_QTextCursor( 1 )->insertImage( hbqt_par_QString( 2 ) );
}

/*
 * void insertImage ( const QTextImageFormat & format )
 */
HB_FUNC( QT_QTEXTCURSOR_INSERTIMAGE_1 )
{
   hbqt_par_QTextCursor( 1 )->insertImage( *hbqt_par_QTextImageFormat( 2 ) );
}

/*
 * void insertImage ( const QTextImageFormat & format, QTextFrameFormat::Position alignment )
 */
HB_FUNC( QT_QTEXTCURSOR_INSERTIMAGE_2 )
{
   hbqt_par_QTextCursor( 1 )->insertImage( *hbqt_par_QTextImageFormat( 2 ), ( QTextFrameFormat::Position ) hb_parni( 3 ) );
}

/*
 * void insertImage ( const QImage & image, const QString & name = QString() )
 */
HB_FUNC( QT_QTEXTCURSOR_INSERTIMAGE_3 )
{
   hbqt_par_QTextCursor( 1 )->insertImage( *hbqt_par_QImage( 2 ), hbqt_par_QString( 3 ) );
}

/*
 * QTextList * insertList ( const QTextListFormat & format )
 */
HB_FUNC( QT_QTEXTCURSOR_INSERTLIST )
{
   hb_retptrGC( hbqt_gcAllocate_QTextList( hbqt_par_QTextCursor( 1 )->insertList( *hbqt_par_QTextListFormat( 2 ) ), false ) );
}

/*
 * QTextList * insertList ( QTextListFormat::Style style )
 */
HB_FUNC( QT_QTEXTCURSOR_INSERTLIST_1 )
{
   hb_retptrGC( hbqt_gcAllocate_QTextList( hbqt_par_QTextCursor( 1 )->insertList( ( QTextListFormat::Style ) hb_parni( 2 ) ), false ) );
}

/*
 * void insertText ( const QString & text )
 */
HB_FUNC( QT_QTEXTCURSOR_INSERTTEXT )
{
   hbqt_par_QTextCursor( 1 )->insertText( hbqt_par_QString( 2 ) );
}

/*
 * void insertText ( const QString & text, const QTextCharFormat & format )
 */
HB_FUNC( QT_QTEXTCURSOR_INSERTTEXT_1 )
{
   hbqt_par_QTextCursor( 1 )->insertText( hbqt_par_QString( 2 ), *hbqt_par_QTextCharFormat( 3 ) );
}

/*
 * bool isCopyOf ( const QTextCursor & other ) const
 */
HB_FUNC( QT_QTEXTCURSOR_ISCOPYOF )
{
   hb_retl( hbqt_par_QTextCursor( 1 )->isCopyOf( *hbqt_par_QTextCursor( 2 ) ) );
}

/*
 * bool isNull () const
 */
HB_FUNC( QT_QTEXTCURSOR_ISNULL )
{
   hb_retl( hbqt_par_QTextCursor( 1 )->isNull() );
}

/*
 * void joinPreviousEditBlock ()
 */
HB_FUNC( QT_QTEXTCURSOR_JOINPREVIOUSEDITBLOCK )
{
   hbqt_par_QTextCursor( 1 )->joinPreviousEditBlock();
}

/*
 * void mergeBlockCharFormat ( const QTextCharFormat & modifier )
 */
HB_FUNC( QT_QTEXTCURSOR_MERGEBLOCKCHARFORMAT )
{
   hbqt_par_QTextCursor( 1 )->mergeBlockCharFormat( *hbqt_par_QTextCharFormat( 2 ) );
}

/*
 * void mergeBlockFormat ( const QTextBlockFormat & modifier )
 */
HB_FUNC( QT_QTEXTCURSOR_MERGEBLOCKFORMAT )
{
   hbqt_par_QTextCursor( 1 )->mergeBlockFormat( *hbqt_par_QTextBlockFormat( 2 ) );
}

/*
 * void mergeCharFormat ( const QTextCharFormat & modifier )
 */
HB_FUNC( QT_QTEXTCURSOR_MERGECHARFORMAT )
{
   hbqt_par_QTextCursor( 1 )->mergeCharFormat( *hbqt_par_QTextCharFormat( 2 ) );
}

/*
 * bool movePosition ( MoveOperation operation, MoveMode mode = MoveAnchor, int n = 1 )
 */
HB_FUNC( QT_QTEXTCURSOR_MOVEPOSITION )
{
   hb_retl( hbqt_par_QTextCursor( 1 )->movePosition( ( QTextCursor::MoveOperation ) hb_parni( 2 ), ( HB_ISNUM( 3 ) ? ( QTextCursor::MoveMode ) hb_parni( 3 ) : ( QTextCursor::MoveMode ) QTextCursor::MoveAnchor ), ( HB_ISNUM( 4 ) ? hb_parni( 4 ) : 1 ) ) );
}

/*
 * int position () const
 */
HB_FUNC( QT_QTEXTCURSOR_POSITION )
{
   hb_retni( hbqt_par_QTextCursor( 1 )->position() );
}

/*
 * void removeSelectedText ()
 */
HB_FUNC( QT_QTEXTCURSOR_REMOVESELECTEDTEXT )
{
   hbqt_par_QTextCursor( 1 )->removeSelectedText();
}

/*
 * void select ( SelectionType selection )
 */
HB_FUNC( QT_QTEXTCURSOR_SELECT )
{
   hbqt_par_QTextCursor( 1 )->select( ( QTextCursor::SelectionType ) hb_parni( 2 ) );
}

/*
 * void selectedTableCells ( int * firstRow, int * numRows, int * firstColumn, int * numColumns ) const
 */
HB_FUNC( QT_QTEXTCURSOR_SELECTEDTABLECELLS )
{
   int iFirstRow = 0;
   int iNumRows = 0;
   int iFirstColumn = 0;
   int iNumColumns = 0;

   hbqt_par_QTextCursor( 1 )->selectedTableCells( &iFirstRow, &iNumRows, &iFirstColumn, &iNumColumns );

   hb_storni( iFirstRow, 2 );
   hb_storni( iNumRows, 3 );
   hb_storni( iFirstColumn, 4 );
   hb_storni( iNumColumns, 5 );
}

/*
 * QString selectedText () const
 */
HB_FUNC( QT_QTEXTCURSOR_SELECTEDTEXT )
{
   hb_retc( hbqt_par_QTextCursor( 1 )->selectedText().toAscii().data() );
}

/*
 * QTextDocumentFragment selection () const
 */
HB_FUNC( QT_QTEXTCURSOR_SELECTION )
{
   hb_retptrGC( hbqt_gcAllocate_QTextDocumentFragment( new QTextDocumentFragment( hbqt_par_QTextCursor( 1 )->selection() ), true ) );
}

/*
 * int selectionEnd () const
 */
HB_FUNC( QT_QTEXTCURSOR_SELECTIONEND )
{
   hb_retni( hbqt_par_QTextCursor( 1 )->selectionEnd() );
}

/*
 * int selectionStart () const
 */
HB_FUNC( QT_QTEXTCURSOR_SELECTIONSTART )
{
   hb_retni( hbqt_par_QTextCursor( 1 )->selectionStart() );
}

/*
 * void setBlockCharFormat ( const QTextCharFormat & format )
 */
HB_FUNC( QT_QTEXTCURSOR_SETBLOCKCHARFORMAT )
{
   hbqt_par_QTextCursor( 1 )->setBlockCharFormat( *hbqt_par_QTextCharFormat( 2 ) );
}

/*
 * void setBlockFormat ( const QTextBlockFormat & format )
 */
HB_FUNC( QT_QTEXTCURSOR_SETBLOCKFORMAT )
{
   hbqt_par_QTextCursor( 1 )->setBlockFormat( *hbqt_par_QTextBlockFormat( 2 ) );
}

/*
 * void setCharFormat ( const QTextCharFormat & format )
 */
HB_FUNC( QT_QTEXTCURSOR_SETCHARFORMAT )
{
   hbqt_par_QTextCursor( 1 )->setCharFormat( *hbqt_par_QTextCharFormat( 2 ) );
}

/*
 * void setPosition ( int pos, MoveMode m = MoveAnchor )
 */
HB_FUNC( QT_QTEXTCURSOR_SETPOSITION )
{
   hbqt_par_QTextCursor( 1 )->setPosition( hb_parni( 2 ), ( HB_ISNUM( 3 ) ? ( QTextCursor::MoveMode ) hb_parni( 3 ) : ( QTextCursor::MoveMode ) QTextCursor::MoveAnchor ) );
}

/*
 * void setVisualNavigation ( bool b )
 */
HB_FUNC( QT_QTEXTCURSOR_SETVISUALNAVIGATION )
{
   hbqt_par_QTextCursor( 1 )->setVisualNavigation( hb_parl( 2 ) );
}

/*
 * bool visualNavigation () const
 */
HB_FUNC( QT_QTEXTCURSOR_VISUALNAVIGATION )
{
   hb_retl( hbqt_par_QTextCursor( 1 )->visualNavigation() );
}


/*----------------------------------------------------------------------*/
#endif             /* #if QT_VERSION >= 0x040500 */
/*----------------------------------------------------------------------*/
