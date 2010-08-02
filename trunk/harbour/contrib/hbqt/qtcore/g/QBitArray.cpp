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

#include "hbqt.h"
#include "hbqtcore_garbage.h"
#include "hbqtcore.h"

/*----------------------------------------------------------------------*/
#if QT_VERSION >= 0x040500
/*----------------------------------------------------------------------*/

#include <QtCore/QPointer>

#include <QtCore/QBitArray>


/* QBitArray ()
 * QBitArray ( int size, bool value = false )
 * QBitArray ( const QBitArray & other )
 */

typedef struct
{
   QBitArray * ph;
   bool bNew;
   QT_G_FUNC_PTR func;
   int type;
} QGC_POINTER_QBitArray;

QT_G_FUNC( hbqt_gcRelease_QBitArray )
{
   QGC_POINTER * p = ( QGC_POINTER * ) Cargo;

   if( p && p->bNew )
   {
      if( p->ph )
      {
         HB_TRACE( HB_TR_DEBUG, ( "ph=%p    _rel_QBitArray   /.\\", p->ph ) );
         delete ( ( QBitArray * ) p->ph );
         HB_TRACE( HB_TR_DEBUG, ( "ph=%p YES_rel_QBitArray   \\./", p->ph ) );
         p->ph = NULL;
      }
      else
      {
         HB_TRACE( HB_TR_DEBUG, ( "ph=%p DEL_rel_QBitArray    :     Object already deleted!", p->ph ) );
         p->ph = NULL;
      }
   }
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "ph=%p PTR_rel_QBitArray    :    Object not created with new=true", p->ph ) );
      p->ph = NULL;
   }
}

void * hbqt_gcAllocate_QBitArray( void * pObj, bool bNew )
{
   QGC_POINTER * p = ( QGC_POINTER * ) hb_gcAllocate( sizeof( QGC_POINTER ), hbqt_gcFuncs() );

   p->ph = ( QBitArray * ) pObj;
   p->bNew = bNew;
   p->func = hbqt_gcRelease_QBitArray;
   p->type = HBQT_TYPE_QBitArray;

   if( bNew )
   {
      HB_TRACE( HB_TR_DEBUG, ( "ph=%p    _new_QBitArray", pObj ) );
   }
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "ph=%p NOT_new_QBitArray", pObj ) );
   }
   return p;
}

HB_FUNC( QT_QBITARRAY )
{
   QBitArray * pObj = NULL;

   pObj = new QBitArray() ;

   hb_retptrGC( hbqt_gcAllocate_QBitArray( ( void * ) pObj, true ) );
}

/*
 * bool at ( int i ) const
 */
HB_FUNC( QT_QBITARRAY_AT )
{
   QBitArray * p = hbqt_par_QBitArray( 1 );
   if( p )
      hb_retl( ( p )->at( hb_parni( 2 ) ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QBITARRAY_AT FP=hb_retl( ( p )->at( hb_parni( 2 ) ) ); p is NULL" ) );
   }
}

/*
 * void clear ()
 */
HB_FUNC( QT_QBITARRAY_CLEAR )
{
   QBitArray * p = hbqt_par_QBitArray( 1 );
   if( p )
      ( p )->clear();
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QBITARRAY_CLEAR FP=( p )->clear(); p is NULL" ) );
   }
}

/*
 * void clearBit ( int i )
 */
HB_FUNC( QT_QBITARRAY_CLEARBIT )
{
   QBitArray * p = hbqt_par_QBitArray( 1 );
   if( p )
      ( p )->clearBit( hb_parni( 2 ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QBITARRAY_CLEARBIT FP=( p )->clearBit( hb_parni( 2 ) ); p is NULL" ) );
   }
}

/*
 * int count () const
 */
HB_FUNC( QT_QBITARRAY_COUNT )
{
   QBitArray * p = hbqt_par_QBitArray( 1 );
   if( p )
      hb_retni( ( p )->count() );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QBITARRAY_COUNT FP=hb_retni( ( p )->count() ); p is NULL" ) );
   }
}

/*
 * int count ( bool on ) const
 */
HB_FUNC( QT_QBITARRAY_COUNT_1 )
{
   QBitArray * p = hbqt_par_QBitArray( 1 );
   if( p )
      hb_retni( ( p )->count( hb_parl( 2 ) ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QBITARRAY_COUNT_1 FP=hb_retni( ( p )->count( hb_parl( 2 ) ) ); p is NULL" ) );
   }
}

/*
 * bool fill ( bool value, int size = -1 )
 */
HB_FUNC( QT_QBITARRAY_FILL )
{
   QBitArray * p = hbqt_par_QBitArray( 1 );
   if( p )
      hb_retl( ( p )->fill( hb_parl( 2 ), hb_parnidef( 3, -1 ) ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QBITARRAY_FILL FP=hb_retl( ( p )->fill( hb_parl( 2 ), hb_parnidef( 3, -1 ) ) ); p is NULL" ) );
   }
}

/*
 * void fill ( bool value, int begin, int end )
 */
HB_FUNC( QT_QBITARRAY_FILL_1 )
{
   QBitArray * p = hbqt_par_QBitArray( 1 );
   if( p )
      ( p )->fill( hb_parl( 2 ), hb_parni( 3 ), hb_parni( 4 ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QBITARRAY_FILL_1 FP=( p )->fill( hb_parl( 2 ), hb_parni( 3 ), hb_parni( 4 ) ); p is NULL" ) );
   }
}

/*
 * bool isEmpty () const
 */
HB_FUNC( QT_QBITARRAY_ISEMPTY )
{
   QBitArray * p = hbqt_par_QBitArray( 1 );
   if( p )
      hb_retl( ( p )->isEmpty() );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QBITARRAY_ISEMPTY FP=hb_retl( ( p )->isEmpty() ); p is NULL" ) );
   }
}

/*
 * bool isNull () const
 */
HB_FUNC( QT_QBITARRAY_ISNULL )
{
   QBitArray * p = hbqt_par_QBitArray( 1 );
   if( p )
      hb_retl( ( p )->isNull() );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QBITARRAY_ISNULL FP=hb_retl( ( p )->isNull() ); p is NULL" ) );
   }
}

/*
 * void resize ( int size )
 */
HB_FUNC( QT_QBITARRAY_RESIZE )
{
   QBitArray * p = hbqt_par_QBitArray( 1 );
   if( p )
      ( p )->resize( hb_parni( 2 ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QBITARRAY_RESIZE FP=( p )->resize( hb_parni( 2 ) ); p is NULL" ) );
   }
}

/*
 * void setBit ( int i )
 */
HB_FUNC( QT_QBITARRAY_SETBIT )
{
   QBitArray * p = hbqt_par_QBitArray( 1 );
   if( p )
      ( p )->setBit( hb_parni( 2 ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QBITARRAY_SETBIT FP=( p )->setBit( hb_parni( 2 ) ); p is NULL" ) );
   }
}

/*
 * void setBit ( int i, bool value )
 */
HB_FUNC( QT_QBITARRAY_SETBIT_1 )
{
   QBitArray * p = hbqt_par_QBitArray( 1 );
   if( p )
      ( p )->setBit( hb_parni( 2 ), hb_parl( 3 ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QBITARRAY_SETBIT_1 FP=( p )->setBit( hb_parni( 2 ), hb_parl( 3 ) ); p is NULL" ) );
   }
}

/*
 * int size () const
 */
HB_FUNC( QT_QBITARRAY_SIZE )
{
   QBitArray * p = hbqt_par_QBitArray( 1 );
   if( p )
      hb_retni( ( p )->size() );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QBITARRAY_SIZE FP=hb_retni( ( p )->size() ); p is NULL" ) );
   }
}

/*
 * bool testBit ( int i ) const
 */
HB_FUNC( QT_QBITARRAY_TESTBIT )
{
   QBitArray * p = hbqt_par_QBitArray( 1 );
   if( p )
      hb_retl( ( p )->testBit( hb_parni( 2 ) ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QBITARRAY_TESTBIT FP=hb_retl( ( p )->testBit( hb_parni( 2 ) ) ); p is NULL" ) );
   }
}

/*
 * bool toggleBit ( int i )
 */
HB_FUNC( QT_QBITARRAY_TOGGLEBIT )
{
   QBitArray * p = hbqt_par_QBitArray( 1 );
   if( p )
      hb_retl( ( p )->toggleBit( hb_parni( 2 ) ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QBITARRAY_TOGGLEBIT FP=hb_retl( ( p )->toggleBit( hb_parni( 2 ) ) ); p is NULL" ) );
   }
}

/*
 * void truncate ( int pos )
 */
HB_FUNC( QT_QBITARRAY_TRUNCATE )
{
   QBitArray * p = hbqt_par_QBitArray( 1 );
   if( p )
      ( p )->truncate( hb_parni( 2 ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QBITARRAY_TRUNCATE FP=( p )->truncate( hb_parni( 2 ) ); p is NULL" ) );
   }
}


/*----------------------------------------------------------------------*/
#endif             /* #if QT_VERSION >= 0x040500 */
/*----------------------------------------------------------------------*/
