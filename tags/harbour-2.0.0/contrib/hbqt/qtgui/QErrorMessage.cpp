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
 * Copyright 2009 Pritpal Bedi <pritpal@vouchcac.com>
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

#include <QtCore/QPointer>

#include <QtGui/QErrorMessage>


/*
 * QErrorMessage ( QWidget * parent = 0 )
 * ~QErrorMessage ()
 */

typedef struct
{
  void * ph;
  QT_G_FUNC_PTR func;
  QPointer< QErrorMessage > pq;
} QGC_POINTER_QErrorMessage;

QT_G_FUNC( hbqt_gcRelease_QErrorMessage )
{
   QGC_POINTER_QErrorMessage * p = ( QGC_POINTER_QErrorMessage * ) Cargo;

   HB_TRACE( HB_TR_DEBUG, ( "hbqt_gcRelease_QErrorMessage                p=%p", p));
   HB_TRACE( HB_TR_DEBUG, ( "hbqt_gcRelease_QErrorMessage               ph=%p pq=%p", p->ph, (void *)(p->pq)));

   if( p && p->ph && p->pq )
   {
      const QMetaObject * m = ( ( QObject * ) p->ph )->metaObject();
      if( ( QString ) m->className() != ( QString ) "QObject" )
      {
         switch( hbqt_get_object_release_method() )
         {
         case HBQT_RELEASE_WITH_DELETE:
            delete ( ( QErrorMessage * ) p->ph );
            break;
         case HBQT_RELEASE_WITH_DESTRUTOR:
            ( ( QErrorMessage * ) p->ph )->~QErrorMessage();
            break;
         case HBQT_RELEASE_WITH_DELETE_LATER:
            ( ( QErrorMessage * ) p->ph )->deleteLater();
            break;
         }
         p->ph = NULL;
         HB_TRACE( HB_TR_DEBUG, ( "hbqt_gcRelease_QErrorMessage               Object deleted! %i B %i KB", ( int ) hb_xquery( 1001 ), hbqt_getmemused() ) );
      }
      else
      {
         HB_TRACE( HB_TR_DEBUG, ( "NO hbqt_gcRelease_QErrorMessage               Object Name Missing!" ) );
      }
   }
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "DEL hbqt_gcRelease_QErrorMessage               Object Already deleted!" ) );
   }
}

void * hbqt_gcAllocate_QErrorMessage( void * pObj )
{
   QGC_POINTER_QErrorMessage * p = ( QGC_POINTER_QErrorMessage * ) hb_gcAllocate( sizeof( QGC_POINTER_QErrorMessage ), hbqt_gcFuncs() );

   p->ph = pObj;
   p->func = hbqt_gcRelease_QErrorMessage;
   new( & p->pq ) QPointer< QErrorMessage >( ( QErrorMessage * ) pObj );
   HB_TRACE( HB_TR_DEBUG, ( "          new_QErrorMessage               %i B %i KB", ( int ) hb_xquery( 1001 ), hbqt_getmemused() ) );
   return( p );
}

HB_FUNC( QT_QERRORMESSAGE )
{
   void * pObj = NULL;

   pObj = ( QErrorMessage* ) new QErrorMessage( hbqt_par_QWidget( 1 ) ) ;

   hb_retptrGC( hbqt_gcAllocate_QErrorMessage( pObj ) );
}
/*
 * void showMessage ( const QString & message )
 */
HB_FUNC( QT_QERRORMESSAGE_SHOWMESSAGE )
{
   hbqt_par_QErrorMessage( 1 )->showMessage( hbqt_par_QString( 2 ) );
}

/*
 * void showMessage ( const QString & message, const QString & type )
 */
HB_FUNC( QT_QERRORMESSAGE_SHOWMESSAGE_1 )
{
   hbqt_par_QErrorMessage( 1 )->showMessage( hbqt_par_QString( 2 ), hbqt_par_QString( 3 ) );
}


/*----------------------------------------------------------------------*/
#endif             /* #if QT_VERSION >= 0x040500 */
/*----------------------------------------------------------------------*/
