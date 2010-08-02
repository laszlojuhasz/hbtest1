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
 *  enum SequenceFormat { NativeText, PortableText }
 *  enum SequenceMatch { NoMatch, PartialMatch, ExactMatch }
 *  enum StandardKey { AddTab, Back, Bold, Close, ..., ZoomOut }
 */

#include <QtCore/QPointer>

#include <QtGui/QKeySequence>


/*
 * QKeySequence ()
 * QKeySequence ( const QString & key )
 * QKeySequence ( int k1, int k2 = 0, int k3 = 0, int k4 = 0 )
 * QKeySequence ( const QKeySequence & keysequence )
 * QKeySequence ( StandardKey key )
 * ~QKeySequence ()
 */

typedef struct
{
   QKeySequence * ph;
   bool bNew;
   QT_G_FUNC_PTR func;
   int type;
} QGC_POINTER_QKeySequence;

QT_G_FUNC( hbqt_gcRelease_QKeySequence )
{
   QGC_POINTER * p = ( QGC_POINTER * ) Cargo;

   if( p && p->bNew )
   {
      if( p->ph )
      {
         HB_TRACE( HB_TR_DEBUG, ( "ph=%p    _rel_QKeySequence   /.\\", p->ph ) );
         delete ( ( QKeySequence * ) p->ph );
         HB_TRACE( HB_TR_DEBUG, ( "ph=%p YES_rel_QKeySequence   \\./", p->ph ) );
         p->ph = NULL;
      }
      else
      {
         HB_TRACE( HB_TR_DEBUG, ( "ph=%p DEL_rel_QKeySequence    :     Object already deleted!", p->ph ) );
         p->ph = NULL;
      }
   }
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "ph=%p PTR_rel_QKeySequence    :    Object not created with new=true", p->ph ) );
      p->ph = NULL;
   }
}

void * hbqt_gcAllocate_QKeySequence( void * pObj, bool bNew )
{
   QGC_POINTER * p = ( QGC_POINTER * ) hb_gcAllocate( sizeof( QGC_POINTER ), hbqt_gcFuncs() );

   p->ph = ( QKeySequence * ) pObj;
   p->bNew = bNew;
   p->func = hbqt_gcRelease_QKeySequence;
   p->type = HBQT_TYPE_QKeySequence;

   if( bNew )
   {
      HB_TRACE( HB_TR_DEBUG, ( "ph=%p    _new_QKeySequence", pObj ) );
   }
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "ph=%p NOT_new_QKeySequence", pObj ) );
   }
   return p;
}

HB_FUNC( QT_QKEYSEQUENCE )
{
   QKeySequence * pObj = NULL;

   if( HB_ISPOINTER( 1 ) )
      pObj = ( QKeySequence * ) new QKeySequence( *hbqt_par_QKeySequence( 1 ) ) ;
   else if( HB_ISCHAR( 1 ) )
      pObj = ( QKeySequence * ) new QKeySequence( hbqt_par_QString( 1 ) ) ;
   else if( HB_ISNUM( 1 ) )
      pObj = ( QKeySequence * ) new QKeySequence( hb_parni( 1 ) ) ;
   else
      pObj = ( QKeySequence * ) new QKeySequence() ;

   hb_retptrGC( hbqt_gcAllocate_QKeySequence( ( void * ) pObj, true ) );
}

/*
 * uint count () const
 */
HB_FUNC( QT_QKEYSEQUENCE_COUNT )
{
   QKeySequence * p = hbqt_par_QKeySequence( 1 );
   if( p )
      hb_retni( ( p )->count() );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QKEYSEQUENCE_COUNT FP=hb_retni( ( p )->count() ); p is NULL" ) );
   }
}

/*
 * bool isEmpty () const
 */
HB_FUNC( QT_QKEYSEQUENCE_ISEMPTY )
{
   QKeySequence * p = hbqt_par_QKeySequence( 1 );
   if( p )
      hb_retl( ( p )->isEmpty() );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QKEYSEQUENCE_ISEMPTY FP=hb_retl( ( p )->isEmpty() ); p is NULL" ) );
   }
}

/*
 * SequenceMatch matches ( const QKeySequence & seq ) const
 */
HB_FUNC( QT_QKEYSEQUENCE_MATCHES )
{
   QKeySequence * p = hbqt_par_QKeySequence( 1 );
   if( p )
      hb_retni( ( QKeySequence::SequenceMatch ) ( p )->matches( *hbqt_par_QKeySequence( 2 ) ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QKEYSEQUENCE_MATCHES FP=hb_retni( ( QKeySequence::SequenceMatch ) ( p )->matches( *hbqt_par_QKeySequence( 2 ) ) ); p is NULL" ) );
   }
}

/*
 * QString toString ( SequenceFormat format = PortableText ) const
 */
HB_FUNC( QT_QKEYSEQUENCE_TOSTRING )
{
   QKeySequence * p = hbqt_par_QKeySequence( 1 );
   if( p )
      hb_retc( ( p )->toString( ( HB_ISNUM( 2 ) ? ( QKeySequence::SequenceFormat ) hb_parni( 2 ) : ( QKeySequence::SequenceFormat ) QKeySequence::PortableText ) ).toAscii().data() );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QKEYSEQUENCE_TOSTRING FP=hb_retc( ( p )->toString( ( HB_ISNUM( 2 ) ? ( QKeySequence::SequenceFormat ) hb_parni( 2 ) : ( QKeySequence::SequenceFormat ) QKeySequence::PortableText ) ).toAscii().data() ); p is NULL" ) );
   }
}

/*
 * QKeySequence fromString ( const QString & str, SequenceFormat format = PortableText )
 */
HB_FUNC( QT_QKEYSEQUENCE_FROMSTRING )
{
   QKeySequence * p = hbqt_par_QKeySequence( 1 );
   if( p )
      hb_retptrGC( hbqt_gcAllocate_QKeySequence( new QKeySequence( ( p )->fromString( hbqt_par_QString( 2 ), ( HB_ISNUM( 3 ) ? ( QKeySequence::SequenceFormat ) hb_parni( 3 ) : ( QKeySequence::SequenceFormat ) QKeySequence::PortableText ) ) ), true ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QKEYSEQUENCE_FROMSTRING FP=hb_retptrGC( hbqt_gcAllocate_QKeySequence( new QKeySequence( ( p )->fromString( hbqt_par_QString( 2 ), ( HB_ISNUM( 3 ) ? ( QKeySequence::SequenceFormat ) hb_parni( 3 ) : ( QKeySequence::SequenceFormat ) QKeySequence::PortableText ) ) ), true ) ); p is NULL" ) );
   }
}

/*
 * QList<QKeySequence> keyBindings ( StandardKey key )
 */
HB_FUNC( QT_QKEYSEQUENCE_KEYBINDINGS )
{
   QKeySequence * p = hbqt_par_QKeySequence( 1 );
   if( p )
      hb_retptrGC( hbqt_gcAllocate_QList( new QList<QKeySequence>( ( p )->keyBindings( ( QKeySequence::StandardKey ) hb_parni( 2 ) ) ), true ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QKEYSEQUENCE_KEYBINDINGS FP=hb_retptrGC( hbqt_gcAllocate_QList( new QList<QKeySequence>( ( p )->keyBindings( ( QKeySequence::StandardKey ) hb_parni( 2 ) ) ), true ) ); p is NULL" ) );
   }
}

/*
 * QKeySequence mnemonic ( const QString & text )
 */
HB_FUNC( QT_QKEYSEQUENCE_MNEMONIC )
{
   QKeySequence * p = hbqt_par_QKeySequence( 1 );
   if( p )
      hb_retptrGC( hbqt_gcAllocate_QKeySequence( new QKeySequence( ( p )->mnemonic( hbqt_par_QString( 2 ) ) ), true ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QKEYSEQUENCE_MNEMONIC FP=hb_retptrGC( hbqt_gcAllocate_QKeySequence( new QKeySequence( ( p )->mnemonic( hbqt_par_QString( 2 ) ) ), true ) ); p is NULL" ) );
   }
}


/*----------------------------------------------------------------------*/
#endif             /* #if QT_VERSION >= 0x040500 */
/*----------------------------------------------------------------------*/
