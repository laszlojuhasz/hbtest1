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

#include <QtCore/QPointer>

#include <QtGui/QSyntaxHighlighter>
#include "../hbqt_hbqsyntaxhighlighter.h"

/*
 * QSyntaxHighlighter ( QObject * parent )
 * QSyntaxHighlighter ( QTextDocument * parent )
 * QSyntaxHighlighter ( QTextEdit * parent )
 * virtual ~QSyntaxHighlighter ()
 */

typedef struct
{
   QPointer< QSyntaxHighlighter > ph;
   bool bNew;
   QT_G_FUNC_PTR func;
   int type;
} QGC_POINTER_QSyntaxHighlighter;

QT_G_FUNC( hbqt_gcRelease_QSyntaxHighlighter )
{
   HB_SYMBOL_UNUSED( Cargo );
   QGC_POINTER * p = ( QGC_POINTER * ) Cargo;

   if( p && p->bNew )
   {
      p->ph = NULL;
   }
}

void * hbqt_gcAllocate_QSyntaxHighlighter( void * pObj, bool bNew )
{
   QGC_POINTER_QSyntaxHighlighter * p = ( QGC_POINTER_QSyntaxHighlighter * ) hb_gcAllocate( sizeof( QGC_POINTER_QSyntaxHighlighter ), hbqt_gcFuncs() );

   new( & p->ph ) QPointer< QSyntaxHighlighter >( ( QSyntaxHighlighter * ) pObj );
   p->bNew = bNew;
   p->func = hbqt_gcRelease_QSyntaxHighlighter;
   p->type = QT_TYPE_QSyntaxHighlighter;

   if( bNew )
   {
      HB_TRACE( HB_TR_DEBUG, ( "ph=%p    _new_QSyntaxHighlighter  under p->pq", pObj ) );
   }
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "ph=%p NOT_new_QSyntaxHighlighter", pObj ) );
   }
   return p;
}

HB_FUNC( QT_QSYNTAXHIGHLIGHTER )
{

}

/*
 * QTextDocument * document () const
 */
HB_FUNC( QT_QSYNTAXHIGHLIGHTER_DOCUMENT )
{
   QSyntaxHighlighter * p = hbqt_par_QSyntaxHighlighter( 1 );
   if( p )
      hb_retptrGC( hbqt_gcAllocate_QTextDocument( ( p )->document(), false ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QSYNTAXHIGHLIGHTER_DOCUMENT FP=hb_retptrGC( hbqt_gcAllocate_QTextDocument( ( p )->document(), false ) ); p is NULL" ) );
   }
}

/*
 * void setDocument ( QTextDocument * doc )
 */
HB_FUNC( QT_QSYNTAXHIGHLIGHTER_SETDOCUMENT )
{
   QSyntaxHighlighter * p = hbqt_par_QSyntaxHighlighter( 1 );
   if( p )
      ( p )->setDocument( hbqt_par_QTextDocument( 2 ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QSYNTAXHIGHLIGHTER_SETDOCUMENT FP=( p )->setDocument( hbqt_par_QTextDocument( 2 ) ); p is NULL" ) );
   }
}

/*
 * void rehighlight ()
 */
HB_FUNC( QT_QSYNTAXHIGHLIGHTER_REHIGHLIGHT )
{
   QSyntaxHighlighter * p = hbqt_par_QSyntaxHighlighter( 1 );
   if( p )
      ( p )->rehighlight();
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QSYNTAXHIGHLIGHTER_REHIGHLIGHT FP=( p )->rehighlight(); p is NULL" ) );
   }
}


/*----------------------------------------------------------------------*/
#endif             /* #if QT_VERSION >= 0x040500 */
/*----------------------------------------------------------------------*/
