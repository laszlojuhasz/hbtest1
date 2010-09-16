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
#include "hbqtdesigner.h"

/*----------------------------------------------------------------------*/
#if QT_VERSION >= 0x040500
/*----------------------------------------------------------------------*/

#include <QtCore/QPointer>

#include "hbqtgui.h"

#include <QtDesigner/QDesignerActionEditorInterface>


/*
 * QDesignerActionEditorInterface ( QWidget * parent, Qt::WindowFlags flags = 0 )
 * ~QDesignerActionEditorInterface ()
 *
 */

typedef struct
{
   QPointer< QDesignerActionEditorInterface > ph;
   bool bNew;
   PHBQT_GC_FUNC func;
   int type;
} HBQT_GC_T_QDesignerActionEditorInterface;

HBQT_GC_FUNC( hbqt_gcRelease_QDesignerActionEditorInterface )
{
   HB_SYMBOL_UNUSED( Cargo );
   HBQT_GC_T * p = ( HBQT_GC_T * ) Cargo;

   if( p && p->bNew )
   {
      p->ph = NULL;
   }
}

void * hbqt_gcAllocate_QDesignerActionEditorInterface( void * pObj, bool bNew )
{
   HBQT_GC_T_QDesignerActionEditorInterface * p = ( HBQT_GC_T_QDesignerActionEditorInterface * ) hb_gcAllocate( sizeof( HBQT_GC_T_QDesignerActionEditorInterface ), hbqt_gcFuncs() );

   new( & p->ph ) QPointer< QDesignerActionEditorInterface >( ( QDesignerActionEditorInterface * ) pObj );
   p->bNew = bNew;
   p->func = hbqt_gcRelease_QDesignerActionEditorInterface;
   p->type = HBQT_TYPE_QDesignerActionEditorInterface;

   if( bNew )
   {
      HB_TRACE( HB_TR_DEBUG, ( "ph=%p    _new_QDesignerActionEditorInterface  under p->pq", pObj ) );
   }
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "ph=%p NOT_new_QDesignerActionEditorInterface", pObj ) );
   }
   return p;
}

HB_FUNC( QT_QDESIGNERACTIONEDITORINTERFACE )
{
   //hb_retptr( new QDesignerActionEditorInterface() );
}

/*
 * virtual QDesignerFormEditorInterface * core () const
 */
HB_FUNC( QT_QDESIGNERACTIONEDITORINTERFACE_CORE )
{
   QDesignerActionEditorInterface * p = hbqt_par_QDesignerActionEditorInterface( 1 );
   if( p )
   {
      hb_retptrGC( hbqt_gcAllocate_QDesignerFormEditorInterface( ( p )->core(), false ) );
   }
}

/*
 * virtual void manageAction ( QAction * action ) = 0
 */
HB_FUNC( QT_QDESIGNERACTIONEDITORINTERFACE_MANAGEACTION )
{
   QDesignerActionEditorInterface * p = hbqt_par_QDesignerActionEditorInterface( 1 );
   if( p )
   {
      ( p )->manageAction( hbqt_par_QAction( 2 ) );
   }
}

/*
 * virtual void unmanageAction ( QAction * action ) = 0
 */
HB_FUNC( QT_QDESIGNERACTIONEDITORINTERFACE_UNMANAGEACTION )
{
   QDesignerActionEditorInterface * p = hbqt_par_QDesignerActionEditorInterface( 1 );
   if( p )
   {
      ( p )->unmanageAction( hbqt_par_QAction( 2 ) );
   }
}

/*
 * virtual void setFormWindow ( QDesignerFormWindowInterface * formWindow ) = 0
 */
HB_FUNC( QT_QDESIGNERACTIONEDITORINTERFACE_SETFORMWINDOW )
{
   QDesignerActionEditorInterface * p = hbqt_par_QDesignerActionEditorInterface( 1 );
   if( p )
   {
      ( p )->setFormWindow( hbqt_par_QDesignerFormWindowInterface( 2 ) );
   }
}


/*----------------------------------------------------------------------*/
#endif             /* #if QT_VERSION >= 0x040500 */
/*----------------------------------------------------------------------*/
