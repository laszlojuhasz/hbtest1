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
 *  enum AttributeType { TextFormat, Cursor, Language, Ruby }
 */

/*
 *  Constructed[ 5/6 [ 83.33% ] ]
 *
 *  *** Unconvered Prototypes ***
 *  -----------------------------
 *
 *  const QList<Attribute> & attributes () const
 */

#include <QtCore/QPointer>

#include <QtGui/QInputMethodEvent>


/* QInputMethodEvent ()
 * QInputMethodEvent ( const QString & preeditText, const QList<Attribute> & attributes )
 * QInputMethodEvent ( const QInputMethodEvent & other )
 */

typedef struct
{
   void * ph;
   bool bNew;
   QT_G_FUNC_PTR func;
} QGC_POINTER_QInputMethodEvent;

QT_G_FUNC( hbqt_gcRelease_QInputMethodEvent )
{
   QGC_POINTER * p = ( QGC_POINTER * ) Cargo;

   if( p && p->bNew )
   {
      if( p->ph )
      {
         HB_TRACE( HB_TR_DEBUG, ( "YES_rel_QInputMethodEvent   /.\\    ph=%p", p->ph ) );
         delete ( ( QInputMethodEvent * ) p->ph );
         HB_TRACE( HB_TR_DEBUG, ( "YES_rel_QInputMethodEvent   \\./    ph=%p", p->ph ) );
         p->ph = NULL;
      }
      else
      {
         HB_TRACE( HB_TR_DEBUG, ( "DEL_rel_QInputMethodEvent    :     Object already deleted!" ) );
      }
   }
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "PTR_rel_QInputMethodEvent    :    Object not created with new()" ) );
      p->ph = NULL;
   }
}

void * hbqt_gcAllocate_QInputMethodEvent( void * pObj, bool bNew )
{
   QGC_POINTER * p = ( QGC_POINTER * ) hb_gcAllocate( sizeof( QGC_POINTER ), hbqt_gcFuncs() );

   p->ph = pObj;
   p->bNew = bNew;
   p->func = hbqt_gcRelease_QInputMethodEvent;

   if( bNew )
   {
      HB_TRACE( HB_TR_DEBUG, ( "   _new_QInputMethodEvent          ph=%p %i B %i KB", pObj, ( int ) hb_xquery( 1001 ), hbqt_getmemused() ) );
   }
   return p;
}

HB_FUNC( QT_QINPUTMETHODEVENT )
{
   void * pObj = NULL;

   pObj = new QInputMethodEvent() ;

   hb_retptrGC( hbqt_gcAllocate_QInputMethodEvent( pObj, true ) );
}

/*
 * const QString & commitString () const
 */
HB_FUNC( QT_QINPUTMETHODEVENT_COMMITSTRING )
{
   hb_retc( hbqt_par_QInputMethodEvent( 1 )->commitString().toAscii().data() );
}

/*
 * const QString & preeditString () const
 */
HB_FUNC( QT_QINPUTMETHODEVENT_PREEDITSTRING )
{
   hb_retc( hbqt_par_QInputMethodEvent( 1 )->preeditString().toAscii().data() );
}

/*
 * int replacementLength () const
 */
HB_FUNC( QT_QINPUTMETHODEVENT_REPLACEMENTLENGTH )
{
   hb_retni( hbqt_par_QInputMethodEvent( 1 )->replacementLength() );
}

/*
 * int replacementStart () const
 */
HB_FUNC( QT_QINPUTMETHODEVENT_REPLACEMENTSTART )
{
   hb_retni( hbqt_par_QInputMethodEvent( 1 )->replacementStart() );
}

/*
 * void setCommitString ( const QString & commitString, int replaceFrom = 0, int replaceLength = 0 )
 */
HB_FUNC( QT_QINPUTMETHODEVENT_SETCOMMITSTRING )
{
   hbqt_par_QInputMethodEvent( 1 )->setCommitString( hbqt_par_QString( 2 ), hb_parni( 3 ), hb_parni( 4 ) );
}


/*----------------------------------------------------------------------*/
#endif             /* #if QT_VERSION >= 0x040500 */
/*----------------------------------------------------------------------*/
