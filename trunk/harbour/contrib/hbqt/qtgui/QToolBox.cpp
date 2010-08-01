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
#include "hbqtgui_garbage.h"
#include "hbqtcore_garbage.h"

/*----------------------------------------------------------------------*/
#if QT_VERSION >= 0x040500
/*----------------------------------------------------------------------*/

#include <QtCore/QPointer>

#include <QtGui/QToolBox>


/*
 * QToolBox ( QWidget * parent = 0, Qt::WindowFlags f = 0 )
 * ~QToolBox ()
 */

typedef struct
{
   QPointer< QToolBox > ph;
   bool bNew;
   QT_G_FUNC_PTR func;
   int type;
} QGC_POINTER_QToolBox;

QT_G_FUNC( hbqt_gcRelease_QToolBox )
{
   QToolBox  * ph = NULL ;
   QGC_POINTER_QToolBox * p = ( QGC_POINTER_QToolBox * ) Cargo;

   if( p && p->bNew && p->ph )
   {
      ph = p->ph;
      if( ph )
      {
         const QMetaObject * m = ( ph )->metaObject();
         if( ( QString ) m->className() != ( QString ) "QObject" )
         {
            HB_TRACE( HB_TR_DEBUG, ( "ph=%p %p YES_rel_QToolBox   /.\\   ", (void*) ph, (void*) p->ph ) );
            delete ( p->ph );
            HB_TRACE( HB_TR_DEBUG, ( "ph=%p %p YES_rel_QToolBox   \\./   ", (void*) ph, (void*) p->ph ) );
            p->ph = NULL;
         }
         else
         {
            HB_TRACE( HB_TR_DEBUG, ( "ph=%p NO__rel_QToolBox          ", ph ) );
            p->ph = NULL;
         }
      }
      else
      {
         HB_TRACE( HB_TR_DEBUG, ( "ph=%p DEL_rel_QToolBox    :     Object already deleted!", ph ) );
         p->ph = NULL;
      }
   }
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "ph=%p PTR_rel_QToolBox    :    Object not created with new=true", ph ) );
      p->ph = NULL;
   }
}

void * hbqt_gcAllocate_QToolBox( void * pObj, bool bNew )
{
   QGC_POINTER_QToolBox * p = ( QGC_POINTER_QToolBox * ) hb_gcAllocate( sizeof( QGC_POINTER_QToolBox ), hbqt_gcFuncs() );

   new( & p->ph ) QPointer< QToolBox >( ( QToolBox * ) pObj );
   p->bNew = bNew;
   p->func = hbqt_gcRelease_QToolBox;
   p->type = HBQT_TYPE_QToolBox;

   if( bNew )
   {
      HB_TRACE( HB_TR_DEBUG, ( "ph=%p    _new_QToolBox  under p->pq", pObj ) );
   }
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "ph=%p NOT_new_QToolBox", pObj ) );
   }
   return p;
}

HB_FUNC( QT_QTOOLBOX )
{
   QToolBox * pObj = NULL;

   pObj =  new QToolBox( hbqt_par_QWidget( 1 ), ( Qt::WindowFlags ) hb_parni( 2 ) ) ;

   hb_retptrGC( hbqt_gcAllocate_QToolBox( ( void * ) pObj, true ) );
}

/*
 * int addItem ( QWidget * widget, const QIcon & iconSet, const QString & text )
 */
HB_FUNC( QT_QTOOLBOX_ADDITEM )
{
   QToolBox * p = hbqt_par_QToolBox( 1 );
   if( p )
      hb_retni( ( p )->addItem( hbqt_par_QWidget( 2 ), QIcon( hbqt_par_QString( 3 ) ), QToolBox::tr( hb_parc( 4 ) ) ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QTOOLBOX_ADDITEM FP=hb_retni( ( p )->addItem( hbqt_par_QWidget( 2 ), QIcon( hbqt_par_QString( 3 ) ), QToolBox::tr( hb_parc( 4 ) ) ) ); p is NULL" ) );
   }
}

/*
 * int addItem ( QWidget * w, const QString & text )
 */
HB_FUNC( QT_QTOOLBOX_ADDITEM_1 )
{
   QToolBox * p = hbqt_par_QToolBox( 1 );
   if( p )
      hb_retni( ( p )->addItem( hbqt_par_QWidget( 2 ), QToolBox::tr( hb_parc( 3 ) ) ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QTOOLBOX_ADDITEM_1 FP=hb_retni( ( p )->addItem( hbqt_par_QWidget( 2 ), QToolBox::tr( hb_parc( 3 ) ) ) ); p is NULL" ) );
   }
}

/*
 * int count () const
 */
HB_FUNC( QT_QTOOLBOX_COUNT )
{
   QToolBox * p = hbqt_par_QToolBox( 1 );
   if( p )
      hb_retni( ( p )->count() );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QTOOLBOX_COUNT FP=hb_retni( ( p )->count() ); p is NULL" ) );
   }
}

/*
 * int currentIndex () const
 */
HB_FUNC( QT_QTOOLBOX_CURRENTINDEX )
{
   QToolBox * p = hbqt_par_QToolBox( 1 );
   if( p )
      hb_retni( ( p )->currentIndex() );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QTOOLBOX_CURRENTINDEX FP=hb_retni( ( p )->currentIndex() ); p is NULL" ) );
   }
}

/*
 * QWidget * currentWidget () const
 */
HB_FUNC( QT_QTOOLBOX_CURRENTWIDGET )
{
   QToolBox * p = hbqt_par_QToolBox( 1 );
   if( p )
      hb_retptrGC( hbqt_gcAllocate_QWidget( ( p )->currentWidget(), false ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QTOOLBOX_CURRENTWIDGET FP=hb_retptrGC( hbqt_gcAllocate_QWidget( ( p )->currentWidget(), false ) ); p is NULL" ) );
   }
}

/*
 * int indexOf ( QWidget * widget ) const
 */
HB_FUNC( QT_QTOOLBOX_INDEXOF )
{
   QToolBox * p = hbqt_par_QToolBox( 1 );
   if( p )
      hb_retni( ( p )->indexOf( hbqt_par_QWidget( 2 ) ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QTOOLBOX_INDEXOF FP=hb_retni( ( p )->indexOf( hbqt_par_QWidget( 2 ) ) ); p is NULL" ) );
   }
}

/*
 * int insertItem ( int index, QWidget * widget, const QIcon & icon, const QString & text )
 */
HB_FUNC( QT_QTOOLBOX_INSERTITEM )
{
   QToolBox * p = hbqt_par_QToolBox( 1 );
   if( p )
      hb_retni( ( p )->insertItem( hb_parni( 2 ), hbqt_par_QWidget( 3 ), QIcon( hbqt_par_QString( 4 ) ), QToolBox::tr( hb_parc( 5 ) ) ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QTOOLBOX_INSERTITEM FP=hb_retni( ( p )->insertItem( hb_parni( 2 ), hbqt_par_QWidget( 3 ), QIcon( hbqt_par_QString( 4 ) ), QToolBox::tr( hb_parc( 5 ) ) ) ); p is NULL" ) );
   }
}

/*
 * int insertItem ( int index, QWidget * widget, const QString & text )
 */
HB_FUNC( QT_QTOOLBOX_INSERTITEM_1 )
{
   QToolBox * p = hbqt_par_QToolBox( 1 );
   if( p )
      hb_retni( ( p )->insertItem( hb_parni( 2 ), hbqt_par_QWidget( 3 ), QToolBox::tr( hb_parc( 4 ) ) ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QTOOLBOX_INSERTITEM_1 FP=hb_retni( ( p )->insertItem( hb_parni( 2 ), hbqt_par_QWidget( 3 ), QToolBox::tr( hb_parc( 4 ) ) ) ); p is NULL" ) );
   }
}

/*
 * bool isItemEnabled ( int index ) const
 */
HB_FUNC( QT_QTOOLBOX_ISITEMENABLED )
{
   QToolBox * p = hbqt_par_QToolBox( 1 );
   if( p )
      hb_retl( ( p )->isItemEnabled( hb_parni( 2 ) ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QTOOLBOX_ISITEMENABLED FP=hb_retl( ( p )->isItemEnabled( hb_parni( 2 ) ) ); p is NULL" ) );
   }
}

/*
 * QIcon itemIcon ( int index ) const
 */
HB_FUNC( QT_QTOOLBOX_ITEMICON )
{
   QToolBox * p = hbqt_par_QToolBox( 1 );
   if( p )
      hb_retptrGC( hbqt_gcAllocate_QIcon( new QIcon( ( p )->itemIcon( hb_parni( 2 ) ) ), true ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QTOOLBOX_ITEMICON FP=hb_retptrGC( hbqt_gcAllocate_QIcon( new QIcon( ( p )->itemIcon( hb_parni( 2 ) ) ), true ) ); p is NULL" ) );
   }
}

/*
 * QString itemText ( int index ) const
 */
HB_FUNC( QT_QTOOLBOX_ITEMTEXT )
{
   QToolBox * p = hbqt_par_QToolBox( 1 );
   if( p )
      hb_retc( ( p )->itemText( hb_parni( 2 ) ).toAscii().data() );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QTOOLBOX_ITEMTEXT FP=hb_retc( ( p )->itemText( hb_parni( 2 ) ).toAscii().data() ); p is NULL" ) );
   }
}

/*
 * QString itemToolTip ( int index ) const
 */
HB_FUNC( QT_QTOOLBOX_ITEMTOOLTIP )
{
   QToolBox * p = hbqt_par_QToolBox( 1 );
   if( p )
      hb_retc( ( p )->itemToolTip( hb_parni( 2 ) ).toAscii().data() );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QTOOLBOX_ITEMTOOLTIP FP=hb_retc( ( p )->itemToolTip( hb_parni( 2 ) ).toAscii().data() ); p is NULL" ) );
   }
}

/*
 * void removeItem ( int index )
 */
HB_FUNC( QT_QTOOLBOX_REMOVEITEM )
{
   QToolBox * p = hbqt_par_QToolBox( 1 );
   if( p )
      ( p )->removeItem( hb_parni( 2 ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QTOOLBOX_REMOVEITEM FP=( p )->removeItem( hb_parni( 2 ) ); p is NULL" ) );
   }
}

/*
 * void setItemEnabled ( int index, bool enabled )
 */
HB_FUNC( QT_QTOOLBOX_SETITEMENABLED )
{
   QToolBox * p = hbqt_par_QToolBox( 1 );
   if( p )
      ( p )->setItemEnabled( hb_parni( 2 ), hb_parl( 3 ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QTOOLBOX_SETITEMENABLED FP=( p )->setItemEnabled( hb_parni( 2 ), hb_parl( 3 ) ); p is NULL" ) );
   }
}

/*
 * void setItemIcon ( int index, const QIcon & icon )
 */
HB_FUNC( QT_QTOOLBOX_SETITEMICON )
{
   QToolBox * p = hbqt_par_QToolBox( 1 );
   if( p )
      ( p )->setItemIcon( hb_parni( 2 ), QIcon( hbqt_par_QString( 3 ) ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QTOOLBOX_SETITEMICON FP=( p )->setItemIcon( hb_parni( 2 ), QIcon( hbqt_par_QString( 3 ) ) ); p is NULL" ) );
   }
}

/*
 * void setItemText ( int index, const QString & text )
 */
HB_FUNC( QT_QTOOLBOX_SETITEMTEXT )
{
   QToolBox * p = hbqt_par_QToolBox( 1 );
   if( p )
      ( p )->setItemText( hb_parni( 2 ), QToolBox::tr( hb_parc( 3 ) ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QTOOLBOX_SETITEMTEXT FP=( p )->setItemText( hb_parni( 2 ), QToolBox::tr( hb_parc( 3 ) ) ); p is NULL" ) );
   }
}

/*
 * void setItemToolTip ( int index, const QString & toolTip )
 */
HB_FUNC( QT_QTOOLBOX_SETITEMTOOLTIP )
{
   QToolBox * p = hbqt_par_QToolBox( 1 );
   if( p )
      ( p )->setItemToolTip( hb_parni( 2 ), QToolBox::tr( hb_parc( 3 ) ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QTOOLBOX_SETITEMTOOLTIP FP=( p )->setItemToolTip( hb_parni( 2 ), QToolBox::tr( hb_parc( 3 ) ) ); p is NULL" ) );
   }
}

/*
 * QWidget * widget ( int index ) const
 */
HB_FUNC( QT_QTOOLBOX_WIDGET )
{
   QToolBox * p = hbqt_par_QToolBox( 1 );
   if( p )
      hb_retptrGC( hbqt_gcAllocate_QWidget( ( p )->widget( hb_parni( 2 ) ), false ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QTOOLBOX_WIDGET FP=hb_retptrGC( hbqt_gcAllocate_QWidget( ( p )->widget( hb_parni( 2 ) ), false ) ); p is NULL" ) );
   }
}

/*
 * void setCurrentIndex ( int index )
 */
HB_FUNC( QT_QTOOLBOX_SETCURRENTINDEX )
{
   QToolBox * p = hbqt_par_QToolBox( 1 );
   if( p )
      ( p )->setCurrentIndex( hb_parni( 2 ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QTOOLBOX_SETCURRENTINDEX FP=( p )->setCurrentIndex( hb_parni( 2 ) ); p is NULL" ) );
   }
}

/*
 * void setCurrentWidget ( QWidget * widget )
 */
HB_FUNC( QT_QTOOLBOX_SETCURRENTWIDGET )
{
   QToolBox * p = hbqt_par_QToolBox( 1 );
   if( p )
      ( p )->setCurrentWidget( hbqt_par_QWidget( 2 ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QTOOLBOX_SETCURRENTWIDGET FP=( p )->setCurrentWidget( hbqt_par_QWidget( 2 ) ); p is NULL" ) );
   }
}


/*----------------------------------------------------------------------*/
#endif             /* #if QT_VERSION >= 0x040500 */
/*----------------------------------------------------------------------*/
