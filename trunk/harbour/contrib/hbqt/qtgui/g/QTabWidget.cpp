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
 *  enum TabPosition { North, South, West, East }
 *  enum TabShape { Rounded, Triangular }
 */

/*
 *  Constructed[ 41/42 [ 97.62% ] ]
 *
 *  *** Unconvered Prototypes ***
 *  -----------------------------
 *
 *  }
 */

#include <QtCore/QPointer>

#include <QtGui/QTabWidget>


/*
 * QTabWidget ( QWidget * parent = 0 )
 * ~QTabWidget ()
 */

typedef struct
{
   QPointer< QTabWidget > ph;
   bool bNew;
   PHBQT_GC_FUNC func;
   int type;
} HBQT_GC_T_QTabWidget;

HBQT_GC_FUNC( hbqt_gcRelease_QTabWidget )
{
   QTabWidget  * ph = NULL ;
   HBQT_GC_T_QTabWidget * p = ( HBQT_GC_T_QTabWidget * ) Cargo;

   if( p && p->bNew && p->ph )
   {
      ph = p->ph;
      if( ph )
      {
         const QMetaObject * m = ( ph )->metaObject();
         if( ( QString ) m->className() != ( QString ) "QObject" )
         {
            HB_TRACE( HB_TR_DEBUG, ( "ph=%p %p YES_rel_QTabWidget   /.\\   ", (void*) ph, (void*) p->ph ) );
            delete ( p->ph );
            HB_TRACE( HB_TR_DEBUG, ( "ph=%p %p YES_rel_QTabWidget   \\./   ", (void*) ph, (void*) p->ph ) );
            p->ph = NULL;
         }
         else
         {
            HB_TRACE( HB_TR_DEBUG, ( "ph=%p NO__rel_QTabWidget          ", ph ) );
            p->ph = NULL;
         }
      }
      else
      {
         HB_TRACE( HB_TR_DEBUG, ( "ph=%p DEL_rel_QTabWidget    :     Object already deleted!", ph ) );
         p->ph = NULL;
      }
   }
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "ph=%p PTR_rel_QTabWidget    :    Object not created with new=true", ph ) );
      p->ph = NULL;
   }
}

void * hbqt_gcAllocate_QTabWidget( void * pObj, bool bNew )
{
   HBQT_GC_T_QTabWidget * p = ( HBQT_GC_T_QTabWidget * ) hb_gcAllocate( sizeof( HBQT_GC_T_QTabWidget ), hbqt_gcFuncs() );

   new( & p->ph ) QPointer< QTabWidget >( ( QTabWidget * ) pObj );
   p->bNew = bNew;
   p->func = hbqt_gcRelease_QTabWidget;
   p->type = HBQT_TYPE_QTabWidget;

   if( bNew )
   {
      HB_TRACE( HB_TR_DEBUG, ( "ph=%p    _new_QTabWidget  under p->pq", pObj ) );
   }
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "ph=%p NOT_new_QTabWidget", pObj ) );
   }
   return p;
}

HB_FUNC( QT_QTABWIDGET )
{
   QTabWidget * pObj = NULL;

   pObj =  new QTabWidget( hbqt_par_QWidget( 1 ) ) ;

   hb_retptrGC( hbqt_gcAllocate_QTabWidget( ( void * ) pObj, true ) );
}

/*
 * int addTab ( QWidget * page, const QString & label )
 */
HB_FUNC( QT_QTABWIDGET_ADDTAB )
{
   HBQT_GC_T * p = ( HBQT_GC_T * ) hb_parptrGC( hbqt_gcFuncs(), 1 );
   HBQT_GC_T * q = ( HBQT_GC_T * ) hb_parptrGC( hbqt_gcFuncs(), 2 );

   HB_TRACE( HB_TR_DEBUG, ( "Entering function QT_QTABWIDGET_ADDTAB()" ) );
   if( p && p->ph && q && q->ph )
   {
      HB_TRACE( HB_TR_DEBUG, ( "QT_QTABWIDGET_ADDTAB() Qt object: %p is attached to: %p", p->ph, q->ph ) );

      q->bNew = HB_FALSE;

      hb_retni( hbqt_par_QTabWidget( 1 )->addTab( hbqt_par_QWidget( 2 ), hbqt_par_QString( 3 ) ) );
   }
}

/*
 * int addTab ( QWidget * page, const QIcon & icon, const QString & label )
 */
HB_FUNC( QT_QTABWIDGET_ADDTAB_1 )
{
   QTabWidget * p = hbqt_par_QTabWidget( 1 );
   if( p )
      hb_retni( ( p )->addTab( hbqt_par_QWidget( 2 ), ( HB_ISPOINTER( 3 ) ? *hbqt_par_QIcon( 3 ) : QIcon( hbqt_par_QString( 3 ) ) ), QTabWidget::tr( hb_parc( 4 ) ) ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QTABWIDGET_ADDTAB_1 FP=hb_retni( ( p )->addTab( hbqt_par_QWidget( 2 ), ( HB_ISPOINTER( 3 ) ? *hbqt_par_QIcon( 3 ) : QIcon( hbqt_par_QString( 3 ) ) ), QTabWidget::tr( hb_parc( 4 ) ) ) ); p is NULL" ) );
   }
}

/*
 * void clear ()
 */
HB_FUNC( QT_QTABWIDGET_CLEAR )
{
   QTabWidget * p = hbqt_par_QTabWidget( 1 );
   if( p )
      ( p )->clear();
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QTABWIDGET_CLEAR FP=( p )->clear(); p is NULL" ) );
   }
}

/*
 * QWidget * cornerWidget ( Qt::Corner corner = Qt::TopRightCorner ) const
 */
HB_FUNC( QT_QTABWIDGET_CORNERWIDGET )
{
   QTabWidget * p = hbqt_par_QTabWidget( 1 );
   if( p )
      hb_retptrGC( hbqt_gcAllocate_QWidget( ( p )->cornerWidget( ( HB_ISNUM( 2 ) ? ( Qt::Corner ) hb_parni( 2 ) : ( Qt::Corner ) Qt::TopRightCorner ) ), false ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QTABWIDGET_CORNERWIDGET FP=hb_retptrGC( hbqt_gcAllocate_QWidget( ( p )->cornerWidget( ( HB_ISNUM( 2 ) ? ( Qt::Corner ) hb_parni( 2 ) : ( Qt::Corner ) Qt::TopRightCorner ) ), false ) ); p is NULL" ) );
   }
}

/*
 * int count () const
 */
HB_FUNC( QT_QTABWIDGET_COUNT )
{
   QTabWidget * p = hbqt_par_QTabWidget( 1 );
   if( p )
      hb_retni( ( p )->count() );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QTABWIDGET_COUNT FP=hb_retni( ( p )->count() ); p is NULL" ) );
   }
}

/*
 * int currentIndex () const
 */
HB_FUNC( QT_QTABWIDGET_CURRENTINDEX )
{
   QTabWidget * p = hbqt_par_QTabWidget( 1 );
   if( p )
      hb_retni( ( p )->currentIndex() );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QTABWIDGET_CURRENTINDEX FP=hb_retni( ( p )->currentIndex() ); p is NULL" ) );
   }
}

/*
 * QWidget * currentWidget () const
 */
HB_FUNC( QT_QTABWIDGET_CURRENTWIDGET )
{
   QTabWidget * p = hbqt_par_QTabWidget( 1 );
   if( p )
      hb_retptrGC( hbqt_gcAllocate_QWidget( ( p )->currentWidget(), false ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QTABWIDGET_CURRENTWIDGET FP=hb_retptrGC( hbqt_gcAllocate_QWidget( ( p )->currentWidget(), false ) ); p is NULL" ) );
   }
}

/*
 * bool documentMode () const
 */
HB_FUNC( QT_QTABWIDGET_DOCUMENTMODE )
{
   QTabWidget * p = hbqt_par_QTabWidget( 1 );
   if( p )
      hb_retl( ( p )->documentMode() );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QTABWIDGET_DOCUMENTMODE FP=hb_retl( ( p )->documentMode() ); p is NULL" ) );
   }
}

/*
 * Qt::TextElideMode elideMode () const
 */
HB_FUNC( QT_QTABWIDGET_ELIDEMODE )
{
   QTabWidget * p = hbqt_par_QTabWidget( 1 );
   if( p )
      hb_retni( ( Qt::TextElideMode ) ( p )->elideMode() );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QTABWIDGET_ELIDEMODE FP=hb_retni( ( Qt::TextElideMode ) ( p )->elideMode() ); p is NULL" ) );
   }
}

/*
 * QSize iconSize () const
 */
HB_FUNC( QT_QTABWIDGET_ICONSIZE )
{
   QTabWidget * p = hbqt_par_QTabWidget( 1 );
   if( p )
      hb_retptrGC( hbqt_gcAllocate_QSize( new QSize( ( p )->iconSize() ), true ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QTABWIDGET_ICONSIZE FP=hb_retptrGC( hbqt_gcAllocate_QSize( new QSize( ( p )->iconSize() ), true ) ); p is NULL" ) );
   }
}

/*
 * int indexOf ( QWidget * w ) const
 */
HB_FUNC( QT_QTABWIDGET_INDEXOF )
{
   QTabWidget * p = hbqt_par_QTabWidget( 1 );
   if( p )
      hb_retni( ( p )->indexOf( hbqt_par_QWidget( 2 ) ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QTABWIDGET_INDEXOF FP=hb_retni( ( p )->indexOf( hbqt_par_QWidget( 2 ) ) ); p is NULL" ) );
   }
}

/*
 * int insertTab ( int index, QWidget * page, const QString & label )
 */
HB_FUNC( QT_QTABWIDGET_INSERTTAB )
{
   QTabWidget * p = hbqt_par_QTabWidget( 1 );
   if( p )
      hb_retni( ( p )->insertTab( hb_parni( 2 ), hbqt_par_QWidget( 3 ), QTabWidget::tr( hb_parc( 4 ) ) ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QTABWIDGET_INSERTTAB FP=hb_retni( ( p )->insertTab( hb_parni( 2 ), hbqt_par_QWidget( 3 ), QTabWidget::tr( hb_parc( 4 ) ) ) ); p is NULL" ) );
   }
}

/*
 * int insertTab ( int index, QWidget * page, const QIcon & icon, const QString & label )
 */
HB_FUNC( QT_QTABWIDGET_INSERTTAB_1 )
{
   QTabWidget * p = hbqt_par_QTabWidget( 1 );
   if( p )
      hb_retni( ( p )->insertTab( hb_parni( 2 ), hbqt_par_QWidget( 3 ), ( HB_ISPOINTER( 4 ) ? *hbqt_par_QIcon( 4 ) : QIcon( hbqt_par_QString( 4 ) ) ), QTabWidget::tr( hb_parc( 5 ) ) ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QTABWIDGET_INSERTTAB_1 FP=hb_retni( ( p )->insertTab( hb_parni( 2 ), hbqt_par_QWidget( 3 ), ( HB_ISPOINTER( 4 ) ? *hbqt_par_QIcon( 4 ) : QIcon( hbqt_par_QString( 4 ) ) ), QTabWidget::tr( hb_parc( 5 ) ) ) ); p is NULL" ) );
   }
}

/*
 * bool isMovable () const
 */
HB_FUNC( QT_QTABWIDGET_ISMOVABLE )
{
   QTabWidget * p = hbqt_par_QTabWidget( 1 );
   if( p )
      hb_retl( ( p )->isMovable() );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QTABWIDGET_ISMOVABLE FP=hb_retl( ( p )->isMovable() ); p is NULL" ) );
   }
}

/*
 * bool isTabEnabled ( int index ) const
 */
HB_FUNC( QT_QTABWIDGET_ISTABENABLED )
{
   QTabWidget * p = hbqt_par_QTabWidget( 1 );
   if( p )
      hb_retl( ( p )->isTabEnabled( hb_parni( 2 ) ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QTABWIDGET_ISTABENABLED FP=hb_retl( ( p )->isTabEnabled( hb_parni( 2 ) ) ); p is NULL" ) );
   }
}

/*
 * void removeTab ( int index )
 */
HB_FUNC( QT_QTABWIDGET_REMOVETAB )
{
   QTabWidget * p = hbqt_par_QTabWidget( 1 );
   if( p )
      ( p )->removeTab( hb_parni( 2 ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QTABWIDGET_REMOVETAB FP=( p )->removeTab( hb_parni( 2 ) ); p is NULL" ) );
   }
}

/*
 * void setCornerWidget ( QWidget * widget, Qt::Corner corner = Qt::TopRightCorner )
 */
HB_FUNC( QT_QTABWIDGET_SETCORNERWIDGET )
{
   QTabWidget * p = hbqt_par_QTabWidget( 1 );
   if( p )
      ( p )->setCornerWidget( hbqt_par_QWidget( 2 ), ( HB_ISNUM( 3 ) ? ( Qt::Corner ) hb_parni( 3 ) : ( Qt::Corner ) Qt::TopRightCorner ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QTABWIDGET_SETCORNERWIDGET FP=( p )->setCornerWidget( hbqt_par_QWidget( 2 ), ( HB_ISNUM( 3 ) ? ( Qt::Corner ) hb_parni( 3 ) : ( Qt::Corner ) Qt::TopRightCorner ) ); p is NULL" ) );
   }
}

/*
 * void setDocumentMode ( bool set )
 */
HB_FUNC( QT_QTABWIDGET_SETDOCUMENTMODE )
{
   QTabWidget * p = hbqt_par_QTabWidget( 1 );
   if( p )
      ( p )->setDocumentMode( hb_parl( 2 ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QTABWIDGET_SETDOCUMENTMODE FP=( p )->setDocumentMode( hb_parl( 2 ) ); p is NULL" ) );
   }
}

/*
 * void setElideMode ( Qt::TextElideMode )
 */
HB_FUNC( QT_QTABWIDGET_SETELIDEMODE )
{
   QTabWidget * p = hbqt_par_QTabWidget( 1 );
   if( p )
      ( p )->setElideMode( ( Qt::TextElideMode ) hb_parni( 2 ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QTABWIDGET_SETELIDEMODE FP=( p )->setElideMode( ( Qt::TextElideMode ) hb_parni( 2 ) ); p is NULL" ) );
   }
}

/*
 * void setIconSize ( const QSize & size )
 */
HB_FUNC( QT_QTABWIDGET_SETICONSIZE )
{
   QTabWidget * p = hbqt_par_QTabWidget( 1 );
   if( p )
      ( p )->setIconSize( *hbqt_par_QSize( 2 ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QTABWIDGET_SETICONSIZE FP=( p )->setIconSize( *hbqt_par_QSize( 2 ) ); p is NULL" ) );
   }
}

/*
 * void setMovable ( bool movable )
 */
HB_FUNC( QT_QTABWIDGET_SETMOVABLE )
{
   QTabWidget * p = hbqt_par_QTabWidget( 1 );
   if( p )
      ( p )->setMovable( hb_parl( 2 ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QTABWIDGET_SETMOVABLE FP=( p )->setMovable( hb_parl( 2 ) ); p is NULL" ) );
   }
}

/*
 * void setTabEnabled ( int index, bool enable )
 */
HB_FUNC( QT_QTABWIDGET_SETTABENABLED )
{
   QTabWidget * p = hbqt_par_QTabWidget( 1 );
   if( p )
      ( p )->setTabEnabled( hb_parni( 2 ), hb_parl( 3 ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QTABWIDGET_SETTABENABLED FP=( p )->setTabEnabled( hb_parni( 2 ), hb_parl( 3 ) ); p is NULL" ) );
   }
}

/*
 * void setTabIcon ( int index, const QIcon & icon )
 */
HB_FUNC( QT_QTABWIDGET_SETTABICON )
{
   QTabWidget * p = hbqt_par_QTabWidget( 1 );
   if( p )
      ( p )->setTabIcon( hb_parni( 2 ), ( HB_ISPOINTER( 3 ) ? *hbqt_par_QIcon( 3 ) : QIcon( hbqt_par_QString( 3 ) ) ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QTABWIDGET_SETTABICON FP=( p )->setTabIcon( hb_parni( 2 ), ( HB_ISPOINTER( 3 ) ? *hbqt_par_QIcon( 3 ) : QIcon( hbqt_par_QString( 3 ) ) ) ); p is NULL" ) );
   }
}

/*
 * void setTabPosition ( TabPosition )
 */
HB_FUNC( QT_QTABWIDGET_SETTABPOSITION )
{
   QTabWidget * p = hbqt_par_QTabWidget( 1 );
   if( p )
      ( p )->setTabPosition( ( QTabWidget::TabPosition ) hb_parni( 2 ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QTABWIDGET_SETTABPOSITION FP=( p )->setTabPosition( ( QTabWidget::TabPosition ) hb_parni( 2 ) ); p is NULL" ) );
   }
}

/*
 * void setTabShape ( TabShape s )
 */
HB_FUNC( QT_QTABWIDGET_SETTABSHAPE )
{
   QTabWidget * p = hbqt_par_QTabWidget( 1 );
   if( p )
      ( p )->setTabShape( ( QTabWidget::TabShape ) hb_parni( 2 ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QTABWIDGET_SETTABSHAPE FP=( p )->setTabShape( ( QTabWidget::TabShape ) hb_parni( 2 ) ); p is NULL" ) );
   }
}

/*
 * void setTabText ( int index, const QString & label )
 */
HB_FUNC( QT_QTABWIDGET_SETTABTEXT )
{
   QTabWidget * p = hbqt_par_QTabWidget( 1 );
   if( p )
      ( p )->setTabText( hb_parni( 2 ), QTabWidget::tr( hb_parc( 3 ) ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QTABWIDGET_SETTABTEXT FP=( p )->setTabText( hb_parni( 2 ), QTabWidget::tr( hb_parc( 3 ) ) ); p is NULL" ) );
   }
}

/*
 * void setTabToolTip ( int index, const QString & tip )
 */
HB_FUNC( QT_QTABWIDGET_SETTABTOOLTIP )
{
   QTabWidget * p = hbqt_par_QTabWidget( 1 );
   if( p )
      ( p )->setTabToolTip( hb_parni( 2 ), QTabWidget::tr( hb_parc( 3 ) ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QTABWIDGET_SETTABTOOLTIP FP=( p )->setTabToolTip( hb_parni( 2 ), QTabWidget::tr( hb_parc( 3 ) ) ); p is NULL" ) );
   }
}

/*
 * void setTabWhatsThis ( int index, const QString & text )
 */
HB_FUNC( QT_QTABWIDGET_SETTABWHATSTHIS )
{
   QTabWidget * p = hbqt_par_QTabWidget( 1 );
   if( p )
      ( p )->setTabWhatsThis( hb_parni( 2 ), QTabWidget::tr( hb_parc( 3 ) ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QTABWIDGET_SETTABWHATSTHIS FP=( p )->setTabWhatsThis( hb_parni( 2 ), QTabWidget::tr( hb_parc( 3 ) ) ); p is NULL" ) );
   }
}

/*
 * void setTabsClosable ( bool closeable )
 */
HB_FUNC( QT_QTABWIDGET_SETTABSCLOSABLE )
{
   QTabWidget * p = hbqt_par_QTabWidget( 1 );
   if( p )
      ( p )->setTabsClosable( hb_parl( 2 ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QTABWIDGET_SETTABSCLOSABLE FP=( p )->setTabsClosable( hb_parl( 2 ) ); p is NULL" ) );
   }
}

/*
 * void setUsesScrollButtons ( bool useButtons )
 */
HB_FUNC( QT_QTABWIDGET_SETUSESSCROLLBUTTONS )
{
   QTabWidget * p = hbqt_par_QTabWidget( 1 );
   if( p )
      ( p )->setUsesScrollButtons( hb_parl( 2 ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QTABWIDGET_SETUSESSCROLLBUTTONS FP=( p )->setUsesScrollButtons( hb_parl( 2 ) ); p is NULL" ) );
   }
}

/*
 * QIcon tabIcon ( int index ) const
 */
HB_FUNC( QT_QTABWIDGET_TABICON )
{
   QTabWidget * p = hbqt_par_QTabWidget( 1 );
   if( p )
      hb_retptrGC( hbqt_gcAllocate_QIcon( new QIcon( ( p )->tabIcon( hb_parni( 2 ) ) ), true ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QTABWIDGET_TABICON FP=hb_retptrGC( hbqt_gcAllocate_QIcon( new QIcon( ( p )->tabIcon( hb_parni( 2 ) ) ), true ) ); p is NULL" ) );
   }
}

/*
 * TabPosition tabPosition () const
 */
HB_FUNC( QT_QTABWIDGET_TABPOSITION )
{
   QTabWidget * p = hbqt_par_QTabWidget( 1 );
   if( p )
      hb_retni( ( QTabWidget::TabPosition ) ( p )->tabPosition() );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QTABWIDGET_TABPOSITION FP=hb_retni( ( QTabWidget::TabPosition ) ( p )->tabPosition() ); p is NULL" ) );
   }
}

/*
 * TabShape tabShape () const
 */
HB_FUNC( QT_QTABWIDGET_TABSHAPE )
{
   QTabWidget * p = hbqt_par_QTabWidget( 1 );
   if( p )
      hb_retni( ( QTabWidget::TabShape ) ( p )->tabShape() );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QTABWIDGET_TABSHAPE FP=hb_retni( ( QTabWidget::TabShape ) ( p )->tabShape() ); p is NULL" ) );
   }
}

/*
 * QString tabText ( int index ) const
 */
HB_FUNC( QT_QTABWIDGET_TABTEXT )
{
   QTabWidget * p = hbqt_par_QTabWidget( 1 );
   if( p )
      hb_retc( ( p )->tabText( hb_parni( 2 ) ).toAscii().data() );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QTABWIDGET_TABTEXT FP=hb_retc( ( p )->tabText( hb_parni( 2 ) ).toAscii().data() ); p is NULL" ) );
   }
}

/*
 * QString tabToolTip ( int index ) const
 */
HB_FUNC( QT_QTABWIDGET_TABTOOLTIP )
{
   QTabWidget * p = hbqt_par_QTabWidget( 1 );
   if( p )
      hb_retc( ( p )->tabToolTip( hb_parni( 2 ) ).toAscii().data() );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QTABWIDGET_TABTOOLTIP FP=hb_retc( ( p )->tabToolTip( hb_parni( 2 ) ).toAscii().data() ); p is NULL" ) );
   }
}

/*
 * QString tabWhatsThis ( int index ) const
 */
HB_FUNC( QT_QTABWIDGET_TABWHATSTHIS )
{
   QTabWidget * p = hbqt_par_QTabWidget( 1 );
   if( p )
      hb_retc( ( p )->tabWhatsThis( hb_parni( 2 ) ).toAscii().data() );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QTABWIDGET_TABWHATSTHIS FP=hb_retc( ( p )->tabWhatsThis( hb_parni( 2 ) ).toAscii().data() ); p is NULL" ) );
   }
}

/*
 * bool tabsClosable () const
 */
HB_FUNC( QT_QTABWIDGET_TABSCLOSABLE )
{
   QTabWidget * p = hbqt_par_QTabWidget( 1 );
   if( p )
      hb_retl( ( p )->tabsClosable() );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QTABWIDGET_TABSCLOSABLE FP=hb_retl( ( p )->tabsClosable() ); p is NULL" ) );
   }
}

/*
 * bool usesScrollButtons () const
 */
HB_FUNC( QT_QTABWIDGET_USESSCROLLBUTTONS )
{
   QTabWidget * p = hbqt_par_QTabWidget( 1 );
   if( p )
      hb_retl( ( p )->usesScrollButtons() );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QTABWIDGET_USESSCROLLBUTTONS FP=hb_retl( ( p )->usesScrollButtons() ); p is NULL" ) );
   }
}

/*
 * QWidget * widget ( int index ) const
 */
HB_FUNC( QT_QTABWIDGET_WIDGET )
{
   QTabWidget * p = hbqt_par_QTabWidget( 1 );
   if( p )
      hb_retptrGC( hbqt_gcAllocate_QWidget( ( p )->widget( hb_parni( 2 ) ), false ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QTABWIDGET_WIDGET FP=hb_retptrGC( hbqt_gcAllocate_QWidget( ( p )->widget( hb_parni( 2 ) ), false ) ); p is NULL" ) );
   }
}

/*
 * void setCurrentIndex ( int index )
 */
HB_FUNC( QT_QTABWIDGET_SETCURRENTINDEX )
{
   QTabWidget * p = hbqt_par_QTabWidget( 1 );
   if( p )
      ( p )->setCurrentIndex( hb_parni( 2 ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QTABWIDGET_SETCURRENTINDEX FP=( p )->setCurrentIndex( hb_parni( 2 ) ); p is NULL" ) );
   }
}

/*
 * void setCurrentWidget ( QWidget * widget )
 */
HB_FUNC( QT_QTABWIDGET_SETCURRENTWIDGET )
{
   QTabWidget * p = hbqt_par_QTabWidget( 1 );
   if( p )
      ( p )->setCurrentWidget( hbqt_par_QWidget( 2 ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QTABWIDGET_SETCURRENTWIDGET FP=( p )->setCurrentWidget( hbqt_par_QWidget( 2 ) ); p is NULL" ) );
   }
}


/*----------------------------------------------------------------------*/
#endif             /* #if QT_VERSION >= 0x040500 */
/*----------------------------------------------------------------------*/
