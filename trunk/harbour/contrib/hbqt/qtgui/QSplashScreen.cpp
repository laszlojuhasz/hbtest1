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

#include <QtGui/QSplashScreen>


/*
 * QSplashScreen ( const QPixmap & pixmap = QPixmap(), Qt::WindowFlags f = 0 )
 * QSplashScreen ( QWidget * parent, const QPixmap & pixmap = QPixmap(), Qt::WindowFlags f = 0 )
 * virtual ~QSplashScreen ()
 */

typedef struct
{
  void * ph;
  QT_G_FUNC_PTR func;
  QPointer< QSplashScreen > pq;
} QGC_POINTER_QSplashScreen;

QT_G_FUNC( release_QSplashScreen )
{
   QGC_POINTER_QSplashScreen * p = ( QGC_POINTER_QSplashScreen * ) Cargo;

   HB_TRACE( HB_TR_DEBUG, ( "release_QSplashScreen                p=%p", p));
   HB_TRACE( HB_TR_DEBUG, ( "release_QSplashScreen               ph=%p pq=%p", p->ph, (void *)(p->pq)));

   if( p && p->ph && p->pq )
   {
      const QMetaObject * m = ( ( QObject * ) p->ph )->metaObject();
      if( ( QString ) m->className() != ( QString ) "QObject" )
      {
         switch( hbqt_get_object_release_method() )
         {
         case HBQT_RELEASE_WITH_DELETE:
            delete ( ( QSplashScreen * ) p->ph );
            break;
         case HBQT_RELEASE_WITH_DESTRUTOR:
            ( ( QSplashScreen * ) p->ph )->~QSplashScreen();
            break;
         case HBQT_RELEASE_WITH_DELETE_LATER:
            ( ( QSplashScreen * ) p->ph )->deleteLater();
            break;
         }
         p->ph = NULL;
         HB_TRACE( HB_TR_DEBUG, ( "release_QSplashScreen               Object deleted! %i B %i KB", ( int ) hb_xquery( 1001 ), hbqt_getmemused() ) );
      }
      else
      {
         HB_TRACE( HB_TR_DEBUG, ( "NO release_QSplashScreen               Object Name Missing!" ) );
      }
   }
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "DEL release_QSplashScreen               Object Already deleted!" ) );
   }
}

void * hbqt_gcAllocate_QSplashScreen( void * pObj )
{
   QGC_POINTER_QSplashScreen * p = ( QGC_POINTER_QSplashScreen * ) hb_gcAllocate( sizeof( QGC_POINTER_QSplashScreen ), hbqt_gcFuncs() );

   p->ph = pObj;
   p->func = release_QSplashScreen;
   new( & p->pq ) QPointer< QSplashScreen >( ( QSplashScreen * ) pObj );
   HB_TRACE( HB_TR_DEBUG, ( "          new_QSplashScreen               %i B %i KB", ( int ) hb_xquery( 1001 ), hbqt_getmemused() ) );
   return( p );
}

HB_FUNC( QT_QSPLASHSCREEN )
{
   void * pObj = NULL;

   pObj = new QSplashScreen( hbqt_par_QWidget( 1 ) ) ;

   hb_retptrGC( hbqt_gcAllocate_QSplashScreen( pObj ) );
}
/*
 * void finish ( QWidget * mainWin )
 */
HB_FUNC( QT_QSPLASHSCREEN_FINISH )
{
   hbqt_par_QSplashScreen( 1 )->finish( hbqt_par_QWidget( 2 ) );
}

/*
 * const QPixmap pixmap () const
 */
HB_FUNC( QT_QSPLASHSCREEN_PIXMAP )
{
   hb_retptrGC( hbqt_gcAllocate_QPixmap( new QPixmap( hbqt_par_QSplashScreen( 1 )->pixmap() ) ) );
}

/*
 * void repaint ()
 */
HB_FUNC( QT_QSPLASHSCREEN_REPAINT )
{
   hbqt_par_QSplashScreen( 1 )->repaint();
}

/*
 * void setPixmap ( const QPixmap & pixmap )
 */
HB_FUNC( QT_QSPLASHSCREEN_SETPIXMAP )
{
   hbqt_par_QSplashScreen( 1 )->setPixmap( *hbqt_par_QPixmap( 2 ) );
}

/*
 * void clearMessage ()
 */
HB_FUNC( QT_QSPLASHSCREEN_CLEARMESSAGE )
{
   hbqt_par_QSplashScreen( 1 )->clearMessage();
}

/*
 * void showMessage ( const QString & message, int alignment = Qt::AlignLeft, const QColor & color = Qt::black )
 */
HB_FUNC( QT_QSPLASHSCREEN_SHOWMESSAGE )
{
   hbqt_par_QSplashScreen( 1 )->showMessage( hbqt_par_QString( 2 ), ( HB_ISNUM( 3 ) ? hb_parni( 3 ) : Qt::AlignLeft ), *hbqt_par_QColor( 4 ) );
}


/*----------------------------------------------------------------------*/
#endif             /* #if QT_VERSION >= 0x040500 */
/*----------------------------------------------------------------------*/
