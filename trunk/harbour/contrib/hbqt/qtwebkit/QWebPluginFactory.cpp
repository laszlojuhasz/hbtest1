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

/*
 *  class ExtensionOption
 *  enum Extension { }
 */

/*
 *  Constructed[ 3/5 [ 60.00% ] ]
 *
 *  *** Unconvered Prototypes ***
 *  -----------------------------
 *
 *  virtual QList<Plugin> plugins () const = 0
 *
 *  *** Commented out protos which construct fine but do not compile ***
 *
 *  // virtual bool extension ( Extension extension, const ExtensionOption * option = 0, ExtensionReturn * output = 0 )
 */

#include <QtCore/QPointer>

#include <QtWebKit/QWebPluginFactory>
#include "hbqt_garbage.h"
#include "hbqt_local.h"


/*
 * QWebPluginFactory ( QObject * parent = 0 )
 * virtual ~QWebPluginFactory ()
 */

typedef struct
{
   QPointer< QWebPluginFactory > ph;
   bool bNew;
   QT_G_FUNC_PTR func;
} QGC_POINTER_QWebPluginFactory;

QT_G_FUNC( hbqt_gcRelease_QWebPluginFactory )
{
   HB_SYMBOL_UNUSED( Cargo );
   QGC_POINTER * p = ( QGC_POINTER * ) Cargo;

   if( p && p->bNew )
   {
      p->ph = NULL;
   }
}

void * hbqt_gcAllocate_QWebPluginFactory( void * pObj, bool bNew )
{
   QGC_POINTER_QWebPluginFactory * p = ( QGC_POINTER_QWebPluginFactory * ) hb_gcAllocate( sizeof( QGC_POINTER_QWebPluginFactory ), hbqt_gcFuncs() );

   new( & p->ph ) QPointer< QWebPluginFactory >( ( QWebPluginFactory * ) pObj );
   p->bNew = bNew;
   p->func = hbqt_gcRelease_QWebPluginFactory;

   if( bNew )
   {
      HB_TRACE( HB_TR_DEBUG, ( "ph=%p    _new_QWebPluginFactory  under p->pq", pObj ) );
   }
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "ph=%p NOT_new_QWebPluginFactory", pObj ) );
   }
   return p;
}

HB_FUNC( QT_QWEBPLUGINFACTORY )
{
   //hb_retptr( ( QWebPluginFactory* ) new QWebPluginFactory() );
}

/*
 * virtual QObject * create ( const QString & mimeType, const QUrl & url, const QStringList & argumentNames, const QStringList & argumentValues ) const = 0
 */
HB_FUNC( QT_QWEBPLUGINFACTORY_CREATE )
{
   QWebPluginFactory * p = hbqt_par_QWebPluginFactory( 1 );
   if( p )
      hb_retptrGC( hbqt_gcAllocate_QObject( ( p )->create( QWebPluginFactory::tr( hb_parc( 2 ) ), *hbqt_par_QUrl( 3 ), *hbqt_par_QStringList( 4 ), *hbqt_par_QStringList( 5 ) ), false ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QWEBPLUGINFACTORY_CREATE FP=hb_retptrGC( hbqt_gcAllocate_QObject( ( p )->create( QWebPluginFactory::tr( hb_parc( 2 ) ), *hbqt_par_QUrl( 3 ), *hbqt_par_QStringList( 4 ), *hbqt_par_QStringList( 5 ) ), false ) ); p is NULL" ) );
   }
}

/*
 * virtual void refreshPlugins ()
 */
HB_FUNC( QT_QWEBPLUGINFACTORY_REFRESHPLUGINS )
{
   QWebPluginFactory * p = hbqt_par_QWebPluginFactory( 1 );
   if( p )
      ( p )->refreshPlugins();
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QWEBPLUGINFACTORY_REFRESHPLUGINS FP=( p )->refreshPlugins(); p is NULL" ) );
   }
}

/*
 * virtual bool supportsExtension ( Extension extension ) const
 */
HB_FUNC( QT_QWEBPLUGINFACTORY_SUPPORTSEXTENSION )
{
   QWebPluginFactory * p = hbqt_par_QWebPluginFactory( 1 );
   if( p )
      hb_retl( ( p )->supportsExtension( ( QWebPluginFactory::Extension ) hb_parni( 2 ) ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QWEBPLUGINFACTORY_SUPPORTSEXTENSION FP=hb_retl( ( p )->supportsExtension( ( QWebPluginFactory::Extension ) hb_parni( 2 ) ) ); p is NULL" ) );
   }
}


/*----------------------------------------------------------------------*/
#endif             /* #if QT_VERSION >= 0x040500 */
/*----------------------------------------------------------------------*/
