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

#include "hbqtcore.h"
#include "hbqtgui.h"

/*----------------------------------------------------------------------*/
#if QT_VERSION >= 0x040500
/*----------------------------------------------------------------------*/

/*
 *  Constructed[ 9/9 [ 100.00% ] ]
 *
 */

#include <QtCore/QPointer>

#include <QtGui/QTextList>


/*
 *
 *
 */

typedef struct
{
   QPointer< QTextList > ph;
   bool bNew;
   PHBQT_GC_FUNC func;
   int type;
} HBQT_GC_T_QTextList;

HBQT_GC_FUNC( hbqt_gcRelease_QTextList )
{
   HB_SYMBOL_UNUSED( Cargo );
   HBQT_GC_T * p = ( HBQT_GC_T * ) Cargo;

   if( p && p->bNew )
   {
      p->ph = NULL;
   }
}

void * hbqt_gcAllocate_QTextList( void * pObj, bool bNew )
{
   HBQT_GC_T_QTextList * p = ( HBQT_GC_T_QTextList * ) hb_gcAllocate( sizeof( HBQT_GC_T_QTextList ), hbqt_gcFuncs() );

   new( & p->ph ) QPointer< QTextList >( ( QTextList * ) pObj );
   p->bNew = bNew;
   p->func = hbqt_gcRelease_QTextList;
   p->type = HBQT_TYPE_QTextList;

   if( bNew )
   {
      HB_TRACE( HB_TR_DEBUG, ( "ph=%p    _new_QTextList  under p->pq", pObj ) );
   }
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "ph=%p NOT_new_QTextList", pObj ) );
   }
   return p;
}

HB_FUNC( QT_QTEXTLIST )
{

}

/*
 * void add ( const QTextBlock & block )
 */
HB_FUNC( QT_QTEXTLIST_ADD )
{
   QTextList * p = hbqt_par_QTextList( 1 );
   if( p )
   {
      ( p )->add( *hbqt_par_QTextBlock( 2 ) );
   }
}

/*
 * int count () const
 */
HB_FUNC( QT_QTEXTLIST_COUNT )
{
   QTextList * p = hbqt_par_QTextList( 1 );
   if( p )
   {
      hb_retni( ( p )->count() );
   }
}

/*
 * QTextListFormat format () const
 */
HB_FUNC( QT_QTEXTLIST_FORMAT )
{
   QTextList * p = hbqt_par_QTextList( 1 );
   if( p )
   {
      hb_retptrGC( hbqt_gcAllocate_QTextListFormat( new QTextListFormat( ( p )->format() ), true ) );
   }
}

/*
 * QTextBlock item ( int i ) const
 */
HB_FUNC( QT_QTEXTLIST_ITEM )
{
   QTextList * p = hbqt_par_QTextList( 1 );
   if( p )
   {
      hb_retptrGC( hbqt_gcAllocate_QTextBlock( new QTextBlock( ( p )->item( hb_parni( 2 ) ) ), true ) );
   }
}

/*
 * int itemNumber ( const QTextBlock & block ) const
 */
HB_FUNC( QT_QTEXTLIST_ITEMNUMBER )
{
   QTextList * p = hbqt_par_QTextList( 1 );
   if( p )
   {
      hb_retni( ( p )->itemNumber( *hbqt_par_QTextBlock( 2 ) ) );
   }
}

/*
 * QString itemText ( const QTextBlock & block ) const
 */
HB_FUNC( QT_QTEXTLIST_ITEMTEXT )
{
   QTextList * p = hbqt_par_QTextList( 1 );
   if( p )
   {
      hb_retstr_utf8( ( p )->itemText( *hbqt_par_QTextBlock( 2 ) ).toUtf8().data() );
   }
}

/*
 * void remove ( const QTextBlock & block )
 */
HB_FUNC( QT_QTEXTLIST_REMOVE )
{
   QTextList * p = hbqt_par_QTextList( 1 );
   if( p )
   {
      ( p )->remove( *hbqt_par_QTextBlock( 2 ) );
   }
}

/*
 * void removeItem ( int i )
 */
HB_FUNC( QT_QTEXTLIST_REMOVEITEM )
{
   QTextList * p = hbqt_par_QTextList( 1 );
   if( p )
   {
      ( p )->removeItem( hb_parni( 2 ) );
   }
}

/*
 * void setFormat ( const QTextListFormat & format )
 */
HB_FUNC( QT_QTEXTLIST_SETFORMAT )
{
   QTextList * p = hbqt_par_QTextList( 1 );
   if( p )
   {
      ( p )->setFormat( *hbqt_par_QTextListFormat( 2 ) );
   }
}


/*----------------------------------------------------------------------*/
#endif             /* #if QT_VERSION >= 0x040500 */
/*----------------------------------------------------------------------*/
