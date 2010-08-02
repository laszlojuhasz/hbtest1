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
#include "hbqscintilla.h"

/*----------------------------------------------------------------------*/
#if QT_VERSION >= 0x040500
/*----------------------------------------------------------------------*/

#include <QtCore/QPointer>

#include <qscicommandset.h>


/*
 *
 *
 */

typedef struct
{
   QsciCommandSet * ph;
   bool bNew;
   PHBQT_GC_FUNC func;
   int type;
} HBQT_GC_T_QsciCommandSet;

HBQT_GC_FUNC( hbqt_gcRelease_QsciCommandSet )
{
   HB_SYMBOL_UNUSED( Cargo );
   HBQT_GC_T * p = ( HBQT_GC_T * ) Cargo;

   if( p && p->bNew )
   {
      p->ph = NULL;
   }
}

void * hbqt_gcAllocate_QsciCommandSet( void * pObj, bool bNew )
{
   HBQT_GC_T * p = ( HBQT_GC_T * ) hb_gcAllocate( sizeof( HBQT_GC_T ), hbqt_gcFuncs() );

   p->ph = ( QsciCommandSet * ) pObj;
   p->bNew = bNew;
   p->func = hbqt_gcRelease_QsciCommandSet;
   p->type = HBQT_TYPE_QsciCommandSet;

   if( bNew )
   {
      HB_TRACE( HB_TR_DEBUG, ( "ph=%p    _new_QsciCommandSet", pObj ) );
   }
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "ph=%p NOT_new_QsciCommandSet", pObj ) );
   }
   return p;
}

HB_FUNC( QT_QSCICOMMANDSET )
{
   //hb_retptr( new QsciCommandSet() );
}

/*
 * bool readSettings (QSettings &qs, const char *prefix="/Scintilla")
 */
HB_FUNC( QT_QSCICOMMANDSET_READSETTINGS )
{
   QsciCommandSet * p = hbqt_par_QsciCommandSet( 1 );
   if( p )
      hb_retl( ( p )->readSettings( *hbqt_par_QSettings( 2 ), hbqt_par_char( 3 ) ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QSCICOMMANDSET_READSETTINGS FP=hb_retl( ( p )->readSettings( *hbqt_par_QSettings( 2 ), hbqt_par_char( 3 ) ) ); p is NULL" ) );
   }
}

/*
 * bool writeSettings (QSettings &qs, const char *prefix="/Scintilla")
 */
HB_FUNC( QT_QSCICOMMANDSET_WRITESETTINGS )
{
   QsciCommandSet * p = hbqt_par_QsciCommandSet( 1 );
   if( p )
      hb_retl( ( p )->writeSettings( *hbqt_par_QSettings( 2 ), hbqt_par_char( 3 ) ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QSCICOMMANDSET_WRITESETTINGS FP=hb_retl( ( p )->writeSettings( *hbqt_par_QSettings( 2 ), hbqt_par_char( 3 ) ) ); p is NULL" ) );
   }
}

/*
 * QList< QsciCommand * > & commands ()
 */
HB_FUNC( QT_QSCICOMMANDSET_COMMANDS )
{
   QsciCommandSet * p = hbqt_par_QsciCommandSet( 1 );
   if( p )
      hb_retptrGC( hbqt_gcAllocate_QList( new QList< QsciCommand * > &( ( p )->commands() ), true ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QSCICOMMANDSET_COMMANDS FP=hb_retptrGC( hbqt_gcAllocate_QList( new QList< QsciCommand * > &( ( p )->commands() ), true ) ); p is NULL" ) );
   }
}

/*
 * void clearKeys ()
 */
HB_FUNC( QT_QSCICOMMANDSET_CLEARKEYS )
{
   QsciCommandSet * p = hbqt_par_QsciCommandSet( 1 );
   if( p )
      ( p )->clearKeys();
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QSCICOMMANDSET_CLEARKEYS FP=( p )->clearKeys(); p is NULL" ) );
   }
}

/*
 * void clearAlternateKeys ()
 */
HB_FUNC( QT_QSCICOMMANDSET_CLEARALTERNATEKEYS )
{
   QsciCommandSet * p = hbqt_par_QsciCommandSet( 1 );
   if( p )
      ( p )->clearAlternateKeys();
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QSCICOMMANDSET_CLEARALTERNATEKEYS FP=( p )->clearAlternateKeys(); p is NULL" ) );
   }
}


/*----------------------------------------------------------------------*/
#endif             /* #if QT_VERSION >= 0x040500 */
/*----------------------------------------------------------------------*/
