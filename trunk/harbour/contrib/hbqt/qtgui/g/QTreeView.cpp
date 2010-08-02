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
 *  #      These enums are defined in QAbstractItemView class
 *  #
 *  enum DragDropMode { NoDragDrop, DragOnly, DropOnly, DragDrop, InternalMove }
 *  enum EditTrigger { NoEditTriggers, CurrentChanged, DoubleClicked, SelectedClicked, ..., AllEditTriggers }
 *  enum ScrollHint { EnsureVisible, PositionAtTop, PositionAtBottom, PositionAtCenter }
 *  enum ScrollMode { ScrollPerItem, ScrollPerPixel }
 *  enum SelectionBehavior { SelectItems, SelectRows, SelectColumns }
 *  enum SelectionMode { SingleSelection, ContiguousSelection, ExtendedSelection, MultiSelection, NoSelection }
 *  flags EditTriggers
 */

#include <QtCore/QPointer>

#include <QtGui/QTreeView>


/*
 * QTreeView ( QWidget * parent = 0 )
 * ~QTreeView ()
 */

typedef struct
{
   QPointer< QTreeView > ph;
   bool bNew;
   QT_G_FUNC_PTR func;
   int type;
} QGC_POINTER_QTreeView;

QT_G_FUNC( hbqt_gcRelease_QTreeView )
{
   QTreeView  * ph = NULL ;
   QGC_POINTER_QTreeView * p = ( QGC_POINTER_QTreeView * ) Cargo;

   if( p && p->bNew && p->ph )
   {
      ph = p->ph;
      if( ph )
      {
         const QMetaObject * m = ( ph )->metaObject();
         if( ( QString ) m->className() != ( QString ) "QObject" )
         {
            HB_TRACE( HB_TR_DEBUG, ( "ph=%p %p YES_rel_QTreeView   /.\\   ", (void*) ph, (void*) p->ph ) );
            delete ( p->ph );
            HB_TRACE( HB_TR_DEBUG, ( "ph=%p %p YES_rel_QTreeView   \\./   ", (void*) ph, (void*) p->ph ) );
            p->ph = NULL;
         }
         else
         {
            HB_TRACE( HB_TR_DEBUG, ( "ph=%p NO__rel_QTreeView          ", ph ) );
            p->ph = NULL;
         }
      }
      else
      {
         HB_TRACE( HB_TR_DEBUG, ( "ph=%p DEL_rel_QTreeView    :     Object already deleted!", ph ) );
         p->ph = NULL;
      }
   }
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "ph=%p PTR_rel_QTreeView    :    Object not created with new=true", ph ) );
      p->ph = NULL;
   }
}

void * hbqt_gcAllocate_QTreeView( void * pObj, bool bNew )
{
   QGC_POINTER_QTreeView * p = ( QGC_POINTER_QTreeView * ) hb_gcAllocate( sizeof( QGC_POINTER_QTreeView ), hbqt_gcFuncs() );

   new( & p->ph ) QPointer< QTreeView >( ( QTreeView * ) pObj );
   p->bNew = bNew;
   p->func = hbqt_gcRelease_QTreeView;
   p->type = HBQT_TYPE_QTreeView;

   if( bNew )
   {
      HB_TRACE( HB_TR_DEBUG, ( "ph=%p    _new_QTreeView  under p->pq", pObj ) );
   }
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "ph=%p NOT_new_QTreeView", pObj ) );
   }
   return p;
}

HB_FUNC( QT_QTREEVIEW )
{
   QTreeView * pObj = NULL;

   pObj =  new QTreeView( hbqt_par_QWidget( 1 ) ) ;

   hb_retptrGC( hbqt_gcAllocate_QTreeView( ( void * ) pObj, true ) );
}

/*
 * bool allColumnsShowFocus () const
 */
HB_FUNC( QT_QTREEVIEW_ALLCOLUMNSSHOWFOCUS )
{
   QTreeView * p = hbqt_par_QTreeView( 1 );
   if( p )
      hb_retl( ( p )->allColumnsShowFocus() );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QTREEVIEW_ALLCOLUMNSSHOWFOCUS FP=hb_retl( ( p )->allColumnsShowFocus() ); p is NULL" ) );
   }
}

/*
 * int autoExpandDelay () const
 */
HB_FUNC( QT_QTREEVIEW_AUTOEXPANDDELAY )
{
   QTreeView * p = hbqt_par_QTreeView( 1 );
   if( p )
      hb_retni( ( p )->autoExpandDelay() );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QTREEVIEW_AUTOEXPANDDELAY FP=hb_retni( ( p )->autoExpandDelay() ); p is NULL" ) );
   }
}

/*
 * int columnAt ( int x ) const
 */
HB_FUNC( QT_QTREEVIEW_COLUMNAT )
{
   QTreeView * p = hbqt_par_QTreeView( 1 );
   if( p )
      hb_retni( ( p )->columnAt( hb_parni( 2 ) ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QTREEVIEW_COLUMNAT FP=hb_retni( ( p )->columnAt( hb_parni( 2 ) ) ); p is NULL" ) );
   }
}

/*
 * int columnViewportPosition ( int column ) const
 */
HB_FUNC( QT_QTREEVIEW_COLUMNVIEWPORTPOSITION )
{
   QTreeView * p = hbqt_par_QTreeView( 1 );
   if( p )
      hb_retni( ( p )->columnViewportPosition( hb_parni( 2 ) ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QTREEVIEW_COLUMNVIEWPORTPOSITION FP=hb_retni( ( p )->columnViewportPosition( hb_parni( 2 ) ) ); p is NULL" ) );
   }
}

/*
 * int columnWidth ( int column ) const
 */
HB_FUNC( QT_QTREEVIEW_COLUMNWIDTH )
{
   QTreeView * p = hbqt_par_QTreeView( 1 );
   if( p )
      hb_retni( ( p )->columnWidth( hb_parni( 2 ) ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QTREEVIEW_COLUMNWIDTH FP=hb_retni( ( p )->columnWidth( hb_parni( 2 ) ) ); p is NULL" ) );
   }
}

/*
 * bool expandsOnDoubleClick () const
 */
HB_FUNC( QT_QTREEVIEW_EXPANDSONDOUBLECLICK )
{
   QTreeView * p = hbqt_par_QTreeView( 1 );
   if( p )
      hb_retl( ( p )->expandsOnDoubleClick() );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QTREEVIEW_EXPANDSONDOUBLECLICK FP=hb_retl( ( p )->expandsOnDoubleClick() ); p is NULL" ) );
   }
}

/*
 * QHeaderView * header () const
 */
HB_FUNC( QT_QTREEVIEW_HEADER )
{
   QTreeView * p = hbqt_par_QTreeView( 1 );
   if( p )
      hb_retptrGC( hbqt_gcAllocate_QHeaderView( ( p )->header(), false ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QTREEVIEW_HEADER FP=hb_retptrGC( hbqt_gcAllocate_QHeaderView( ( p )->header(), false ) ); p is NULL" ) );
   }
}

/*
 * int indentation () const
 */
HB_FUNC( QT_QTREEVIEW_INDENTATION )
{
   QTreeView * p = hbqt_par_QTreeView( 1 );
   if( p )
      hb_retni( ( p )->indentation() );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QTREEVIEW_INDENTATION FP=hb_retni( ( p )->indentation() ); p is NULL" ) );
   }
}

/*
 * QModelIndex indexAbove ( const QModelIndex & index ) const
 */
HB_FUNC( QT_QTREEVIEW_INDEXABOVE )
{
   QTreeView * p = hbqt_par_QTreeView( 1 );
   if( p )
      hb_retptrGC( hbqt_gcAllocate_QModelIndex( new QModelIndex( ( p )->indexAbove( *hbqt_par_QModelIndex( 2 ) ) ), true ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QTREEVIEW_INDEXABOVE FP=hb_retptrGC( hbqt_gcAllocate_QModelIndex( new QModelIndex( ( p )->indexAbove( *hbqt_par_QModelIndex( 2 ) ) ), true ) ); p is NULL" ) );
   }
}

/*
 * QModelIndex indexBelow ( const QModelIndex & index ) const
 */
HB_FUNC( QT_QTREEVIEW_INDEXBELOW )
{
   QTreeView * p = hbqt_par_QTreeView( 1 );
   if( p )
      hb_retptrGC( hbqt_gcAllocate_QModelIndex( new QModelIndex( ( p )->indexBelow( *hbqt_par_QModelIndex( 2 ) ) ), true ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QTREEVIEW_INDEXBELOW FP=hb_retptrGC( hbqt_gcAllocate_QModelIndex( new QModelIndex( ( p )->indexBelow( *hbqt_par_QModelIndex( 2 ) ) ), true ) ); p is NULL" ) );
   }
}

/*
 * bool isAnimated () const
 */
HB_FUNC( QT_QTREEVIEW_ISANIMATED )
{
   QTreeView * p = hbqt_par_QTreeView( 1 );
   if( p )
      hb_retl( ( p )->isAnimated() );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QTREEVIEW_ISANIMATED FP=hb_retl( ( p )->isAnimated() ); p is NULL" ) );
   }
}

/*
 * bool isColumnHidden ( int column ) const
 */
HB_FUNC( QT_QTREEVIEW_ISCOLUMNHIDDEN )
{
   QTreeView * p = hbqt_par_QTreeView( 1 );
   if( p )
      hb_retl( ( p )->isColumnHidden( hb_parni( 2 ) ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QTREEVIEW_ISCOLUMNHIDDEN FP=hb_retl( ( p )->isColumnHidden( hb_parni( 2 ) ) ); p is NULL" ) );
   }
}

/*
 * bool isExpanded ( const QModelIndex & index ) const
 */
HB_FUNC( QT_QTREEVIEW_ISEXPANDED )
{
   QTreeView * p = hbqt_par_QTreeView( 1 );
   if( p )
      hb_retl( ( p )->isExpanded( *hbqt_par_QModelIndex( 2 ) ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QTREEVIEW_ISEXPANDED FP=hb_retl( ( p )->isExpanded( *hbqt_par_QModelIndex( 2 ) ) ); p is NULL" ) );
   }
}

/*
 * bool isFirstColumnSpanned ( int row, const QModelIndex & parent ) const
 */
HB_FUNC( QT_QTREEVIEW_ISFIRSTCOLUMNSPANNED )
{
   QTreeView * p = hbqt_par_QTreeView( 1 );
   if( p )
      hb_retl( ( p )->isFirstColumnSpanned( hb_parni( 2 ), *hbqt_par_QModelIndex( 3 ) ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QTREEVIEW_ISFIRSTCOLUMNSPANNED FP=hb_retl( ( p )->isFirstColumnSpanned( hb_parni( 2 ), *hbqt_par_QModelIndex( 3 ) ) ); p is NULL" ) );
   }
}

/*
 * bool isHeaderHidden () const
 */
HB_FUNC( QT_QTREEVIEW_ISHEADERHIDDEN )
{
   QTreeView * p = hbqt_par_QTreeView( 1 );
   if( p )
      hb_retl( ( p )->isHeaderHidden() );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QTREEVIEW_ISHEADERHIDDEN FP=hb_retl( ( p )->isHeaderHidden() ); p is NULL" ) );
   }
}

/*
 * bool isRowHidden ( int row, const QModelIndex & parent ) const
 */
HB_FUNC( QT_QTREEVIEW_ISROWHIDDEN )
{
   QTreeView * p = hbqt_par_QTreeView( 1 );
   if( p )
      hb_retl( ( p )->isRowHidden( hb_parni( 2 ), *hbqt_par_QModelIndex( 3 ) ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QTREEVIEW_ISROWHIDDEN FP=hb_retl( ( p )->isRowHidden( hb_parni( 2 ), *hbqt_par_QModelIndex( 3 ) ) ); p is NULL" ) );
   }
}

/*
 * bool isSortingEnabled () const
 */
HB_FUNC( QT_QTREEVIEW_ISSORTINGENABLED )
{
   QTreeView * p = hbqt_par_QTreeView( 1 );
   if( p )
      hb_retl( ( p )->isSortingEnabled() );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QTREEVIEW_ISSORTINGENABLED FP=hb_retl( ( p )->isSortingEnabled() ); p is NULL" ) );
   }
}

/*
 * bool itemsExpandable () const
 */
HB_FUNC( QT_QTREEVIEW_ITEMSEXPANDABLE )
{
   QTreeView * p = hbqt_par_QTreeView( 1 );
   if( p )
      hb_retl( ( p )->itemsExpandable() );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QTREEVIEW_ITEMSEXPANDABLE FP=hb_retl( ( p )->itemsExpandable() ); p is NULL" ) );
   }
}

/*
 * bool rootIsDecorated () const
 */
HB_FUNC( QT_QTREEVIEW_ROOTISDECORATED )
{
   QTreeView * p = hbqt_par_QTreeView( 1 );
   if( p )
      hb_retl( ( p )->rootIsDecorated() );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QTREEVIEW_ROOTISDECORATED FP=hb_retl( ( p )->rootIsDecorated() ); p is NULL" ) );
   }
}

/*
 * virtual void scrollTo ( const QModelIndex & index, ScrollHint hint = EnsureVisible )
 */
HB_FUNC( QT_QTREEVIEW_SCROLLTO )
{
   QTreeView * p = hbqt_par_QTreeView( 1 );
   if( p )
      ( p )->scrollTo( *hbqt_par_QModelIndex( 2 ), ( HB_ISNUM( 3 ) ? ( QTreeView::ScrollHint ) hb_parni( 3 ) : ( QTreeView::ScrollHint ) QTreeView::EnsureVisible ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QTREEVIEW_SCROLLTO FP=( p )->scrollTo( *hbqt_par_QModelIndex( 2 ), ( HB_ISNUM( 3 ) ? ( QTreeView::ScrollHint ) hb_parni( 3 ) : ( QTreeView::ScrollHint ) QTreeView::EnsureVisible ) ); p is NULL" ) );
   }
}

/*
 * void setAllColumnsShowFocus ( bool enable )
 */
HB_FUNC( QT_QTREEVIEW_SETALLCOLUMNSSHOWFOCUS )
{
   QTreeView * p = hbqt_par_QTreeView( 1 );
   if( p )
      ( p )->setAllColumnsShowFocus( hb_parl( 2 ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QTREEVIEW_SETALLCOLUMNSSHOWFOCUS FP=( p )->setAllColumnsShowFocus( hb_parl( 2 ) ); p is NULL" ) );
   }
}

/*
 * void setAnimated ( bool enable )
 */
HB_FUNC( QT_QTREEVIEW_SETANIMATED )
{
   QTreeView * p = hbqt_par_QTreeView( 1 );
   if( p )
      ( p )->setAnimated( hb_parl( 2 ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QTREEVIEW_SETANIMATED FP=( p )->setAnimated( hb_parl( 2 ) ); p is NULL" ) );
   }
}

/*
 * void setAutoExpandDelay ( int delay )
 */
HB_FUNC( QT_QTREEVIEW_SETAUTOEXPANDDELAY )
{
   QTreeView * p = hbqt_par_QTreeView( 1 );
   if( p )
      ( p )->setAutoExpandDelay( hb_parni( 2 ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QTREEVIEW_SETAUTOEXPANDDELAY FP=( p )->setAutoExpandDelay( hb_parni( 2 ) ); p is NULL" ) );
   }
}

/*
 * void setColumnHidden ( int column, bool hide )
 */
HB_FUNC( QT_QTREEVIEW_SETCOLUMNHIDDEN )
{
   QTreeView * p = hbqt_par_QTreeView( 1 );
   if( p )
      ( p )->setColumnHidden( hb_parni( 2 ), hb_parl( 3 ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QTREEVIEW_SETCOLUMNHIDDEN FP=( p )->setColumnHidden( hb_parni( 2 ), hb_parl( 3 ) ); p is NULL" ) );
   }
}

/*
 * void setColumnWidth ( int column, int width )
 */
HB_FUNC( QT_QTREEVIEW_SETCOLUMNWIDTH )
{
   QTreeView * p = hbqt_par_QTreeView( 1 );
   if( p )
      ( p )->setColumnWidth( hb_parni( 2 ), hb_parni( 3 ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QTREEVIEW_SETCOLUMNWIDTH FP=( p )->setColumnWidth( hb_parni( 2 ), hb_parni( 3 ) ); p is NULL" ) );
   }
}

/*
 * void setExpanded ( const QModelIndex & index, bool expanded )
 */
HB_FUNC( QT_QTREEVIEW_SETEXPANDED )
{
   QTreeView * p = hbqt_par_QTreeView( 1 );
   if( p )
      ( p )->setExpanded( *hbqt_par_QModelIndex( 2 ), hb_parl( 3 ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QTREEVIEW_SETEXPANDED FP=( p )->setExpanded( *hbqt_par_QModelIndex( 2 ), hb_parl( 3 ) ); p is NULL" ) );
   }
}

/*
 * void setExpandsOnDoubleClick ( bool enable )
 */
HB_FUNC( QT_QTREEVIEW_SETEXPANDSONDOUBLECLICK )
{
   QTreeView * p = hbqt_par_QTreeView( 1 );
   if( p )
      ( p )->setExpandsOnDoubleClick( hb_parl( 2 ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QTREEVIEW_SETEXPANDSONDOUBLECLICK FP=( p )->setExpandsOnDoubleClick( hb_parl( 2 ) ); p is NULL" ) );
   }
}

/*
 * void setFirstColumnSpanned ( int row, const QModelIndex & parent, bool span )
 */
HB_FUNC( QT_QTREEVIEW_SETFIRSTCOLUMNSPANNED )
{
   QTreeView * p = hbqt_par_QTreeView( 1 );
   if( p )
      ( p )->setFirstColumnSpanned( hb_parni( 2 ), *hbqt_par_QModelIndex( 3 ), hb_parl( 4 ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QTREEVIEW_SETFIRSTCOLUMNSPANNED FP=( p )->setFirstColumnSpanned( hb_parni( 2 ), *hbqt_par_QModelIndex( 3 ), hb_parl( 4 ) ); p is NULL" ) );
   }
}

/*
 * void setHeader ( QHeaderView * header )
 */
HB_FUNC( QT_QTREEVIEW_SETHEADER )
{
   QTreeView * p = hbqt_par_QTreeView( 1 );
   if( p )
      ( p )->setHeader( hbqt_par_QHeaderView( 2 ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QTREEVIEW_SETHEADER FP=( p )->setHeader( hbqt_par_QHeaderView( 2 ) ); p is NULL" ) );
   }
}

/*
 * void setHeaderHidden ( bool hide )
 */
HB_FUNC( QT_QTREEVIEW_SETHEADERHIDDEN )
{
   QTreeView * p = hbqt_par_QTreeView( 1 );
   if( p )
      ( p )->setHeaderHidden( hb_parl( 2 ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QTREEVIEW_SETHEADERHIDDEN FP=( p )->setHeaderHidden( hb_parl( 2 ) ); p is NULL" ) );
   }
}

/*
 * void setIndentation ( int i )
 */
HB_FUNC( QT_QTREEVIEW_SETINDENTATION )
{
   QTreeView * p = hbqt_par_QTreeView( 1 );
   if( p )
      ( p )->setIndentation( hb_parni( 2 ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QTREEVIEW_SETINDENTATION FP=( p )->setIndentation( hb_parni( 2 ) ); p is NULL" ) );
   }
}

/*
 * void setItemsExpandable ( bool enable )
 */
HB_FUNC( QT_QTREEVIEW_SETITEMSEXPANDABLE )
{
   QTreeView * p = hbqt_par_QTreeView( 1 );
   if( p )
      ( p )->setItemsExpandable( hb_parl( 2 ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QTREEVIEW_SETITEMSEXPANDABLE FP=( p )->setItemsExpandable( hb_parl( 2 ) ); p is NULL" ) );
   }
}

/*
 * void setRootIsDecorated ( bool show )
 */
HB_FUNC( QT_QTREEVIEW_SETROOTISDECORATED )
{
   QTreeView * p = hbqt_par_QTreeView( 1 );
   if( p )
      ( p )->setRootIsDecorated( hb_parl( 2 ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QTREEVIEW_SETROOTISDECORATED FP=( p )->setRootIsDecorated( hb_parl( 2 ) ); p is NULL" ) );
   }
}

/*
 * void setRowHidden ( int row, const QModelIndex & parent, bool hide )
 */
HB_FUNC( QT_QTREEVIEW_SETROWHIDDEN )
{
   QTreeView * p = hbqt_par_QTreeView( 1 );
   if( p )
      ( p )->setRowHidden( hb_parni( 2 ), *hbqt_par_QModelIndex( 3 ), hb_parl( 4 ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QTREEVIEW_SETROWHIDDEN FP=( p )->setRowHidden( hb_parni( 2 ), *hbqt_par_QModelIndex( 3 ), hb_parl( 4 ) ); p is NULL" ) );
   }
}

/*
 * void setSortingEnabled ( bool enable )
 */
HB_FUNC( QT_QTREEVIEW_SETSORTINGENABLED )
{
   QTreeView * p = hbqt_par_QTreeView( 1 );
   if( p )
      ( p )->setSortingEnabled( hb_parl( 2 ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QTREEVIEW_SETSORTINGENABLED FP=( p )->setSortingEnabled( hb_parl( 2 ) ); p is NULL" ) );
   }
}

/*
 * void setUniformRowHeights ( bool uniform )
 */
HB_FUNC( QT_QTREEVIEW_SETUNIFORMROWHEIGHTS )
{
   QTreeView * p = hbqt_par_QTreeView( 1 );
   if( p )
      ( p )->setUniformRowHeights( hb_parl( 2 ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QTREEVIEW_SETUNIFORMROWHEIGHTS FP=( p )->setUniformRowHeights( hb_parl( 2 ) ); p is NULL" ) );
   }
}

/*
 * void setWordWrap ( bool on )
 */
HB_FUNC( QT_QTREEVIEW_SETWORDWRAP )
{
   QTreeView * p = hbqt_par_QTreeView( 1 );
   if( p )
      ( p )->setWordWrap( hb_parl( 2 ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QTREEVIEW_SETWORDWRAP FP=( p )->setWordWrap( hb_parl( 2 ) ); p is NULL" ) );
   }
}

/*
 * void sortByColumn ( int column, Qt::SortOrder order )
 */
HB_FUNC( QT_QTREEVIEW_SORTBYCOLUMN )
{
   QTreeView * p = hbqt_par_QTreeView( 1 );
   if( p )
      ( p )->sortByColumn( hb_parni( 2 ), ( Qt::SortOrder ) hb_parni( 3 ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QTREEVIEW_SORTBYCOLUMN FP=( p )->sortByColumn( hb_parni( 2 ), ( Qt::SortOrder ) hb_parni( 3 ) ); p is NULL" ) );
   }
}

/*
 * bool uniformRowHeights () const
 */
HB_FUNC( QT_QTREEVIEW_UNIFORMROWHEIGHTS )
{
   QTreeView * p = hbqt_par_QTreeView( 1 );
   if( p )
      hb_retl( ( p )->uniformRowHeights() );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QTREEVIEW_UNIFORMROWHEIGHTS FP=hb_retl( ( p )->uniformRowHeights() ); p is NULL" ) );
   }
}

/*
 * virtual QRect visualRect ( const QModelIndex & index ) const
 */
HB_FUNC( QT_QTREEVIEW_VISUALRECT )
{
   QTreeView * p = hbqt_par_QTreeView( 1 );
   if( p )
      hb_retptrGC( hbqt_gcAllocate_QRect( new QRect( ( p )->visualRect( *hbqt_par_QModelIndex( 2 ) ) ), true ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QTREEVIEW_VISUALRECT FP=hb_retptrGC( hbqt_gcAllocate_QRect( new QRect( ( p )->visualRect( *hbqt_par_QModelIndex( 2 ) ) ), true ) ); p is NULL" ) );
   }
}

/*
 * bool wordWrap () const
 */
HB_FUNC( QT_QTREEVIEW_WORDWRAP )
{
   QTreeView * p = hbqt_par_QTreeView( 1 );
   if( p )
      hb_retl( ( p )->wordWrap() );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QTREEVIEW_WORDWRAP FP=hb_retl( ( p )->wordWrap() ); p is NULL" ) );
   }
}

/*
 * void collapse ( const QModelIndex & index )
 */
HB_FUNC( QT_QTREEVIEW_COLLAPSE )
{
   QTreeView * p = hbqt_par_QTreeView( 1 );
   if( p )
      ( p )->collapse( *hbqt_par_QModelIndex( 2 ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QTREEVIEW_COLLAPSE FP=( p )->collapse( *hbqt_par_QModelIndex( 2 ) ); p is NULL" ) );
   }
}

/*
 * void collapseAll ()
 */
HB_FUNC( QT_QTREEVIEW_COLLAPSEALL )
{
   QTreeView * p = hbqt_par_QTreeView( 1 );
   if( p )
      ( p )->collapseAll();
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QTREEVIEW_COLLAPSEALL FP=( p )->collapseAll(); p is NULL" ) );
   }
}

/*
 * void expand ( const QModelIndex & index )
 */
HB_FUNC( QT_QTREEVIEW_EXPAND )
{
   QTreeView * p = hbqt_par_QTreeView( 1 );
   if( p )
      ( p )->expand( *hbqt_par_QModelIndex( 2 ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QTREEVIEW_EXPAND FP=( p )->expand( *hbqt_par_QModelIndex( 2 ) ); p is NULL" ) );
   }
}

/*
 * void expandAll ()
 */
HB_FUNC( QT_QTREEVIEW_EXPANDALL )
{
   QTreeView * p = hbqt_par_QTreeView( 1 );
   if( p )
      ( p )->expandAll();
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QTREEVIEW_EXPANDALL FP=( p )->expandAll(); p is NULL" ) );
   }
}

/*
 * void expandToDepth ( int depth )
 */
HB_FUNC( QT_QTREEVIEW_EXPANDTODEPTH )
{
   QTreeView * p = hbqt_par_QTreeView( 1 );
   if( p )
      ( p )->expandToDepth( hb_parni( 2 ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QTREEVIEW_EXPANDTODEPTH FP=( p )->expandToDepth( hb_parni( 2 ) ); p is NULL" ) );
   }
}

/*
 * void hideColumn ( int column )
 */
HB_FUNC( QT_QTREEVIEW_HIDECOLUMN )
{
   QTreeView * p = hbqt_par_QTreeView( 1 );
   if( p )
      ( p )->hideColumn( hb_parni( 2 ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QTREEVIEW_HIDECOLUMN FP=( p )->hideColumn( hb_parni( 2 ) ); p is NULL" ) );
   }
}

/*
 * void resizeColumnToContents ( int column )
 */
HB_FUNC( QT_QTREEVIEW_RESIZECOLUMNTOCONTENTS )
{
   QTreeView * p = hbqt_par_QTreeView( 1 );
   if( p )
      ( p )->resizeColumnToContents( hb_parni( 2 ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QTREEVIEW_RESIZECOLUMNTOCONTENTS FP=( p )->resizeColumnToContents( hb_parni( 2 ) ); p is NULL" ) );
   }
}

/*
 * void showColumn ( int column )
 */
HB_FUNC( QT_QTREEVIEW_SHOWCOLUMN )
{
   QTreeView * p = hbqt_par_QTreeView( 1 );
   if( p )
      ( p )->showColumn( hb_parni( 2 ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QTREEVIEW_SHOWCOLUMN FP=( p )->showColumn( hb_parni( 2 ) ); p is NULL" ) );
   }
}


/*----------------------------------------------------------------------*/
#endif             /* #if QT_VERSION >= 0x040500 */
/*----------------------------------------------------------------------*/
