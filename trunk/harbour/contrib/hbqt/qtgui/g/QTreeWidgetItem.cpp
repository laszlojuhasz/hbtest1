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
 * Copyright 2009-2010 Pritpal Bedi <bedipritpal@hotmail.com>
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
/*                            C R E D I T S                             */
/*----------------------------------------------------------------------*/
/*
 * Marcos Antonio Gambeta
 *    for providing first ever prototype parsing methods. Though the current
 *    implementation is diametrically different then what he proposed, still
 *    current code shaped on those footsteps.
 *
 * Viktor Szakats
 *    for directing the project with futuristic vision;
 *    for designing and maintaining a complex build system for hbQT, hbIDE;
 *    for introducing many constructs on PRG and C++ levels;
 *    for streamlining signal/slots and events management classes;
 *
 * Istvan Bisz
 *    for introducing QPointer<> concept in the generator;
 *    for testing the library on numerous accounts;
 *    for showing a way how a GC pointer can be detached;
 *
 * Francesco Perillo
 *    for taking keen interest in hbQT development and peeking the code;
 *    for providing tips here and there to improve the code quality;
 *    for hitting bulls eye to describe why few objects need GC detachment;
 *
 * Carlos Bacco
 *    for implementing HBQT_TYPE_Q*Class enums;
 *    for peeking into the code and suggesting optimization points;
 *
 * Przemyslaw Czerpak
 *    for providing tips and trick to manipulate HVM internals to the best
 *    of its use and always showing a path when we get stuck;
 *    A true tradition of a MASTER...
*/
/*----------------------------------------------------------------------*/

#include "hbqtcore.h"
#include "hbqtgui.h"

/*----------------------------------------------------------------------*/
#if QT_VERSION >= 0x040500
/*----------------------------------------------------------------------*/

/*
 *  enum ChildIndicatorPolicy { ShowIndicator, DontShowIndicator, DontShowIndicatorWhenChildless }
 *  enum ItemType { Type, UserType }
 */

/*
 *  Constructed[ 53/55 [ 96.36% ] ]
 *
 *  *** Unconvered Prototypes ***
 *
 *  void addChildren ( const QList<QTreeWidgetItem *> & children )
 *  void insertChildren ( int index, const QList<QTreeWidgetItem *> & children )
 */

#include <QtCore/QPointer>

#include <QtGui/QTreeWidgetItem>


/*
 * QTreeWidgetItem ( int type = Type )
 * QTreeWidgetItem ( const QStringList & strings, int type = Type )
 * QTreeWidgetItem ( QTreeWidget * parent, int type = Type )
 * QTreeWidgetItem ( QTreeWidget * parent, const QStringList & strings, int type = Type )
 * QTreeWidgetItem ( QTreeWidget * parent, QTreeWidgetItem * preceding, int type = Type )
 * QTreeWidgetItem ( QTreeWidgetItem * parent, int type = Type )
 * QTreeWidgetItem ( QTreeWidgetItem * parent, const QStringList & strings, int type = Type )
 * QTreeWidgetItem ( QTreeWidgetItem * parent, QTreeWidgetItem * preceding, int type = Type )
 * QTreeWidgetItem ( const QTreeWidgetItem & other )
 * virtual ~QTreeWidgetItem ()
 */

typedef struct
{
   QTreeWidgetItem * ph;
   bool bNew;
   PHBQT_GC_FUNC func;
   int type;
} HBQT_GC_T_QTreeWidgetItem;

HBQT_GC_FUNC( hbqt_gcRelease_QTreeWidgetItem )
{
   HBQT_GC_T * p = ( HBQT_GC_T * ) Cargo;

   if( p && p->bNew )
   {
      if( p->ph )
      {
         HB_TRACE( HB_TR_DEBUG, ( "ph=%p    _rel_QTreeWidgetItem   /.\\", p->ph ) );
         delete ( ( QTreeWidgetItem * ) p->ph );
         HB_TRACE( HB_TR_DEBUG, ( "ph=%p YES_rel_QTreeWidgetItem   \\./", p->ph ) );
         p->ph = NULL;
      }
      else
      {
         HB_TRACE( HB_TR_DEBUG, ( "ph=%p DEL_rel_QTreeWidgetItem    :     Object already deleted!", p->ph ) );
         p->ph = NULL;
      }
   }
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "ph=%p PTR_rel_QTreeWidgetItem    :    Object not created with new=true", p->ph ) );
      p->ph = NULL;
   }
}

void * hbqt_gcAllocate_QTreeWidgetItem( void * pObj, bool bNew )
{
   HBQT_GC_T * p = ( HBQT_GC_T * ) hb_gcAllocate( sizeof( HBQT_GC_T ), hbqt_gcFuncs() );

   p->ph = ( QTreeWidgetItem * ) pObj;
   p->bNew = bNew;
   p->func = hbqt_gcRelease_QTreeWidgetItem;
   p->type = HBQT_TYPE_QTreeWidgetItem;

   if( bNew )
   {
      HB_TRACE( HB_TR_DEBUG, ( "ph=%p    _new_QTreeWidgetItem", pObj ) );
   }
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "ph=%p NOT_new_QTreeWidgetItem", pObj ) );
   }
   return p;
}

HB_FUNC( QT_QTREEWIDGETITEM )
{
   QTreeWidgetItem * pObj = NULL;

   if( hb_pcount() >= 1 && HB_ISNUM( 1 ) )
   {
      pObj = new QTreeWidgetItem( hb_parni( 1 ) ) ;
   }
   else
   {
      pObj = new QTreeWidgetItem( hbqt_par_QTreeWidget( 1 ), hb_parni( 2 ) ) ;
   }

   hb_retptrGC( hbqt_gcAllocate_QTreeWidgetItem( ( void * ) pObj, true ) );
}

/*
 * void addChild ( QTreeWidgetItem * child )   [*D=1*]
 */
HB_FUNC( QT_QTREEWIDGETITEM_ADDCHILD )
{
   QTreeWidgetItem * p = hbqt_par_QTreeWidgetItem( 1 );
   if( p )
   {
      hbqt_detachgcpointer( 2 );
      ( p )->addChild( hbqt_par_QTreeWidgetItem( 2 ) );
   }
}

/*
 * QBrush background ( int column ) const
 */
HB_FUNC( QT_QTREEWIDGETITEM_BACKGROUND )
{
   QTreeWidgetItem * p = hbqt_par_QTreeWidgetItem( 1 );
   if( p )
   {
      hb_retptrGC( hbqt_gcAllocate_QBrush( new QBrush( ( p )->background( hb_parni( 2 ) ) ), true ) );
   }
}

/*
 * Qt::CheckState checkState ( int column ) const
 */
HB_FUNC( QT_QTREEWIDGETITEM_CHECKSTATE )
{
   QTreeWidgetItem * p = hbqt_par_QTreeWidgetItem( 1 );
   if( p )
   {
      hb_retni( ( Qt::CheckState ) ( p )->checkState( hb_parni( 2 ) ) );
   }
}

/*
 * QTreeWidgetItem * child ( int index ) const
 */
HB_FUNC( QT_QTREEWIDGETITEM_CHILD )
{
   QTreeWidgetItem * p = hbqt_par_QTreeWidgetItem( 1 );
   if( p )
   {
      hb_retptrGC( hbqt_gcAllocate_QTreeWidgetItem( ( p )->child( hb_parni( 2 ) ), false ) );
   }
}

/*
 * int childCount () const
 */
HB_FUNC( QT_QTREEWIDGETITEM_CHILDCOUNT )
{
   QTreeWidgetItem * p = hbqt_par_QTreeWidgetItem( 1 );
   if( p )
   {
      hb_retni( ( p )->childCount() );
   }
}

/*
 * QTreeWidgetItem::ChildIndicatorPolicy childIndicatorPolicy () const
 */
HB_FUNC( QT_QTREEWIDGETITEM_CHILDINDICATORPOLICY )
{
   QTreeWidgetItem * p = hbqt_par_QTreeWidgetItem( 1 );
   if( p )
   {
      hb_retni( ( QTreeWidgetItem::ChildIndicatorPolicy ) ( p )->childIndicatorPolicy() );
   }
}

/*
 * virtual QTreeWidgetItem * clone () const
 */
HB_FUNC( QT_QTREEWIDGETITEM_CLONE )
{
   QTreeWidgetItem * p = hbqt_par_QTreeWidgetItem( 1 );
   if( p )
   {
      hb_retptrGC( hbqt_gcAllocate_QTreeWidgetItem( ( p )->clone(), false ) );
   }
}

/*
 * int columnCount () const
 */
HB_FUNC( QT_QTREEWIDGETITEM_COLUMNCOUNT )
{
   QTreeWidgetItem * p = hbqt_par_QTreeWidgetItem( 1 );
   if( p )
   {
      hb_retni( ( p )->columnCount() );
   }
}

/*
 * virtual QVariant data ( int column, int role ) const
 */
HB_FUNC( QT_QTREEWIDGETITEM_DATA )
{
   QTreeWidgetItem * p = hbqt_par_QTreeWidgetItem( 1 );
   if( p )
   {
      hb_retptrGC( hbqt_gcAllocate_QVariant( new QVariant( ( p )->data( hb_parni( 2 ), hb_parni( 3 ) ) ), true ) );
   }
}

/*
 * Qt::ItemFlags flags () const
 */
HB_FUNC( QT_QTREEWIDGETITEM_FLAGS )
{
   QTreeWidgetItem * p = hbqt_par_QTreeWidgetItem( 1 );
   if( p )
   {
      hb_retni( ( Qt::ItemFlags ) ( p )->flags() );
   }
}

/*
 * QFont font ( int column ) const
 */
HB_FUNC( QT_QTREEWIDGETITEM_FONT )
{
   QTreeWidgetItem * p = hbqt_par_QTreeWidgetItem( 1 );
   if( p )
   {
      hb_retptrGC( hbqt_gcAllocate_QFont( new QFont( ( p )->font( hb_parni( 2 ) ) ), true ) );
   }
}

/*
 * QBrush foreground ( int column ) const
 */
HB_FUNC( QT_QTREEWIDGETITEM_FOREGROUND )
{
   QTreeWidgetItem * p = hbqt_par_QTreeWidgetItem( 1 );
   if( p )
   {
      hb_retptrGC( hbqt_gcAllocate_QBrush( new QBrush( ( p )->foreground( hb_parni( 2 ) ) ), true ) );
   }
}

/*
 * QIcon icon ( int column ) const
 */
HB_FUNC( QT_QTREEWIDGETITEM_ICON )
{
   QTreeWidgetItem * p = hbqt_par_QTreeWidgetItem( 1 );
   if( p )
   {
      hb_retptrGC( hbqt_gcAllocate_QIcon( new QIcon( ( p )->icon( hb_parni( 2 ) ) ), true ) );
   }
}

/*
 * int indexOfChild ( QTreeWidgetItem * child ) const
 */
HB_FUNC( QT_QTREEWIDGETITEM_INDEXOFCHILD )
{
   QTreeWidgetItem * p = hbqt_par_QTreeWidgetItem( 1 );
   if( p )
   {
      hb_retni( ( p )->indexOfChild( hbqt_par_QTreeWidgetItem( 2 ) ) );
   }
}

/*
 * void insertChild ( int index, QTreeWidgetItem * child )
 */
HB_FUNC( QT_QTREEWIDGETITEM_INSERTCHILD )
{
   QTreeWidgetItem * p = hbqt_par_QTreeWidgetItem( 1 );
   if( p )
   {
      ( p )->insertChild( hb_parni( 2 ), hbqt_par_QTreeWidgetItem( 3 ) );
   }
}

/*
 * bool isDisabled () const
 */
HB_FUNC( QT_QTREEWIDGETITEM_ISDISABLED )
{
   QTreeWidgetItem * p = hbqt_par_QTreeWidgetItem( 1 );
   if( p )
   {
      hb_retl( ( p )->isDisabled() );
   }
}

/*
 * bool isExpanded () const
 */
HB_FUNC( QT_QTREEWIDGETITEM_ISEXPANDED )
{
   QTreeWidgetItem * p = hbqt_par_QTreeWidgetItem( 1 );
   if( p )
   {
      hb_retl( ( p )->isExpanded() );
   }
}

/*
 * bool isFirstColumnSpanned () const
 */
HB_FUNC( QT_QTREEWIDGETITEM_ISFIRSTCOLUMNSPANNED )
{
   QTreeWidgetItem * p = hbqt_par_QTreeWidgetItem( 1 );
   if( p )
   {
      hb_retl( ( p )->isFirstColumnSpanned() );
   }
}

/*
 * bool isHidden () const
 */
HB_FUNC( QT_QTREEWIDGETITEM_ISHIDDEN )
{
   QTreeWidgetItem * p = hbqt_par_QTreeWidgetItem( 1 );
   if( p )
   {
      hb_retl( ( p )->isHidden() );
   }
}

/*
 * bool isSelected () const
 */
HB_FUNC( QT_QTREEWIDGETITEM_ISSELECTED )
{
   QTreeWidgetItem * p = hbqt_par_QTreeWidgetItem( 1 );
   if( p )
   {
      hb_retl( ( p )->isSelected() );
   }
}

/*
 * QTreeWidgetItem * parent () const
 */
HB_FUNC( QT_QTREEWIDGETITEM_PARENT )
{
   QTreeWidgetItem * p = hbqt_par_QTreeWidgetItem( 1 );
   if( p )
   {
      hb_retptrGC( hbqt_gcAllocate_QTreeWidgetItem( ( p )->parent(), false ) );
   }
}

/*
 * virtual void read ( QDataStream & in )
 */
HB_FUNC( QT_QTREEWIDGETITEM_READ )
{
   QTreeWidgetItem * p = hbqt_par_QTreeWidgetItem( 1 );
   if( p )
   {
      ( p )->read( *hbqt_par_QDataStream( 2 ) );
   }
}

/*
 * void removeChild ( QTreeWidgetItem * child )
 */
HB_FUNC( QT_QTREEWIDGETITEM_REMOVECHILD )
{
   QTreeWidgetItem * p = hbqt_par_QTreeWidgetItem( 1 );
   if( p )
   {
      ( p )->removeChild( hbqt_par_QTreeWidgetItem( 2 ) );
   }
}

/*
 * void setBackground ( int column, const QBrush & brush )
 */
HB_FUNC( QT_QTREEWIDGETITEM_SETBACKGROUND )
{
   QTreeWidgetItem * p = hbqt_par_QTreeWidgetItem( 1 );
   if( p )
   {
      ( p )->setBackground( hb_parni( 2 ), *hbqt_par_QBrush( 3 ) );
   }
}

/*
 * void setCheckState ( int column, Qt::CheckState state )
 */
HB_FUNC( QT_QTREEWIDGETITEM_SETCHECKSTATE )
{
   QTreeWidgetItem * p = hbqt_par_QTreeWidgetItem( 1 );
   if( p )
   {
      ( p )->setCheckState( hb_parni( 2 ), ( Qt::CheckState ) hb_parni( 3 ) );
   }
}

/*
 * void setChildIndicatorPolicy ( QTreeWidgetItem::ChildIndicatorPolicy policy )
 */
HB_FUNC( QT_QTREEWIDGETITEM_SETCHILDINDICATORPOLICY )
{
   QTreeWidgetItem * p = hbqt_par_QTreeWidgetItem( 1 );
   if( p )
   {
      ( p )->setChildIndicatorPolicy( ( QTreeWidgetItem::ChildIndicatorPolicy ) hb_parni( 2 ) );
   }
}

/*
 * virtual void setData ( int column, int role, const QVariant & value )
 */
HB_FUNC( QT_QTREEWIDGETITEM_SETDATA )
{
   QTreeWidgetItem * p = hbqt_par_QTreeWidgetItem( 1 );
   if( p )
   {
      ( p )->setData( hb_parni( 2 ), hb_parni( 3 ), *hbqt_par_QVariant( 4 ) );
   }
}

/*
 * void setDisabled ( bool disabled )
 */
HB_FUNC( QT_QTREEWIDGETITEM_SETDISABLED )
{
   QTreeWidgetItem * p = hbqt_par_QTreeWidgetItem( 1 );
   if( p )
   {
      ( p )->setDisabled( hb_parl( 2 ) );
   }
}

/*
 * void setExpanded ( bool expand )
 */
HB_FUNC( QT_QTREEWIDGETITEM_SETEXPANDED )
{
   QTreeWidgetItem * p = hbqt_par_QTreeWidgetItem( 1 );
   if( p )
   {
      ( p )->setExpanded( hb_parl( 2 ) );
   }
}

/*
 * void setFirstColumnSpanned ( bool span )
 */
HB_FUNC( QT_QTREEWIDGETITEM_SETFIRSTCOLUMNSPANNED )
{
   QTreeWidgetItem * p = hbqt_par_QTreeWidgetItem( 1 );
   if( p )
   {
      ( p )->setFirstColumnSpanned( hb_parl( 2 ) );
   }
}

/*
 * void setFlags ( Qt::ItemFlags flags )
 */
HB_FUNC( QT_QTREEWIDGETITEM_SETFLAGS )
{
   QTreeWidgetItem * p = hbqt_par_QTreeWidgetItem( 1 );
   if( p )
   {
      ( p )->setFlags( ( Qt::ItemFlags ) hb_parni( 2 ) );
   }
}

/*
 * void setFont ( int column, const QFont & font )
 */
HB_FUNC( QT_QTREEWIDGETITEM_SETFONT )
{
   QTreeWidgetItem * p = hbqt_par_QTreeWidgetItem( 1 );
   if( p )
   {
      ( p )->setFont( hb_parni( 2 ), *hbqt_par_QFont( 3 ) );
   }
}

/*
 * void setForeground ( int column, const QBrush & brush )
 */
HB_FUNC( QT_QTREEWIDGETITEM_SETFOREGROUND )
{
   QTreeWidgetItem * p = hbqt_par_QTreeWidgetItem( 1 );
   if( p )
   {
      ( p )->setForeground( hb_parni( 2 ), *hbqt_par_QBrush( 3 ) );
   }
}

/*
 * void setHidden ( bool hide )
 */
HB_FUNC( QT_QTREEWIDGETITEM_SETHIDDEN )
{
   QTreeWidgetItem * p = hbqt_par_QTreeWidgetItem( 1 );
   if( p )
   {
      ( p )->setHidden( hb_parl( 2 ) );
   }
}

/*
 * void setIcon ( int column, const QIcon & icon )
 */
HB_FUNC( QT_QTREEWIDGETITEM_SETICON )
{
   QTreeWidgetItem * p = hbqt_par_QTreeWidgetItem( 1 );
   if( p )
   {
      ( p )->setIcon( hb_parni( 2 ), ( HB_ISCHAR( 3 ) ? QIcon( hbqt_par_QString( 3 ) ) : *hbqt_par_QIcon( 3 )) );
   }
}

/*
 * void setSelected ( bool select )
 */
HB_FUNC( QT_QTREEWIDGETITEM_SETSELECTED )
{
   QTreeWidgetItem * p = hbqt_par_QTreeWidgetItem( 1 );
   if( p )
   {
      ( p )->setSelected( hb_parl( 2 ) );
   }
}

/*
 * void setSizeHint ( int column, const QSize & size )
 */
HB_FUNC( QT_QTREEWIDGETITEM_SETSIZEHINT )
{
   QTreeWidgetItem * p = hbqt_par_QTreeWidgetItem( 1 );
   if( p )
   {
      ( p )->setSizeHint( hb_parni( 2 ), *hbqt_par_QSize( 3 ) );
   }
}

/*
 * void setStatusTip ( int column, const QString & statusTip )
 */
HB_FUNC( QT_QTREEWIDGETITEM_SETSTATUSTIP )
{
   QTreeWidgetItem * p = hbqt_par_QTreeWidgetItem( 1 );
   if( p )
   {
      void * pText;
      ( p )->setStatusTip( hb_parni( 2 ), hb_parstr_utf8( 3, &pText, NULL ) );
      hb_strfree( pText );
   }
}

/*
 * void setText ( int column, const QString & text )
 */
HB_FUNC( QT_QTREEWIDGETITEM_SETTEXT )
{
   QTreeWidgetItem * p = hbqt_par_QTreeWidgetItem( 1 );
   if( p )
   {
      void * pText;
      ( p )->setText( hb_parni( 2 ), hb_parstr_utf8( 3, &pText, NULL ) );
      hb_strfree( pText );
   }
}

/*
 * void setTextAlignment ( int column, int alignment )
 */
HB_FUNC( QT_QTREEWIDGETITEM_SETTEXTALIGNMENT )
{
   QTreeWidgetItem * p = hbqt_par_QTreeWidgetItem( 1 );
   if( p )
   {
      ( p )->setTextAlignment( hb_parni( 2 ), hb_parni( 3 ) );
   }
}

/*
 * void setToolTip ( int column, const QString & toolTip )
 */
HB_FUNC( QT_QTREEWIDGETITEM_SETTOOLTIP )
{
   QTreeWidgetItem * p = hbqt_par_QTreeWidgetItem( 1 );
   if( p )
   {
      void * pText;
      ( p )->setToolTip( hb_parni( 2 ), hb_parstr_utf8( 3, &pText, NULL ) );
      hb_strfree( pText );
   }
}

/*
 * void setWhatsThis ( int column, const QString & whatsThis )
 */
HB_FUNC( QT_QTREEWIDGETITEM_SETWHATSTHIS )
{
   QTreeWidgetItem * p = hbqt_par_QTreeWidgetItem( 1 );
   if( p )
   {
      void * pText;
      ( p )->setWhatsThis( hb_parni( 2 ), hb_parstr_utf8( 3, &pText, NULL ) );
      hb_strfree( pText );
   }
}

/*
 * QSize sizeHint ( int column ) const
 */
HB_FUNC( QT_QTREEWIDGETITEM_SIZEHINT )
{
   QTreeWidgetItem * p = hbqt_par_QTreeWidgetItem( 1 );
   if( p )
   {
      hb_retptrGC( hbqt_gcAllocate_QSize( new QSize( ( p )->sizeHint( hb_parni( 2 ) ) ), true ) );
   }
}

/*
 * void sortChildren ( int column, Qt::SortOrder order )
 */
HB_FUNC( QT_QTREEWIDGETITEM_SORTCHILDREN )
{
   QTreeWidgetItem * p = hbqt_par_QTreeWidgetItem( 1 );
   if( p )
   {
      ( p )->sortChildren( hb_parni( 2 ), ( Qt::SortOrder ) hb_parni( 3 ) );
   }
}

/*
 * QString statusTip ( int column ) const
 */
HB_FUNC( QT_QTREEWIDGETITEM_STATUSTIP )
{
   QTreeWidgetItem * p = hbqt_par_QTreeWidgetItem( 1 );
   if( p )
   {
      hb_retstr_utf8( ( p )->statusTip( hb_parni( 2 ) ).toUtf8().data() );
   }
}

/*
 * QTreeWidgetItem * takeChild ( int index )
 */
HB_FUNC( QT_QTREEWIDGETITEM_TAKECHILD )
{
   QTreeWidgetItem * p = hbqt_par_QTreeWidgetItem( 1 );
   if( p )
   {
      hb_retptrGC( hbqt_gcAllocate_QTreeWidgetItem( ( p )->takeChild( hb_parni( 2 ) ), false ) );
   }
}

/*
 * QList<QTreeWidgetItem *> takeChildren ()
 */
HB_FUNC( QT_QTREEWIDGETITEM_TAKECHILDREN )
{
   QTreeWidgetItem * p = hbqt_par_QTreeWidgetItem( 1 );
   if( p )
   {
      hb_retptrGC( hbqt_gcAllocate_QList( new QList<QTreeWidgetItem *>( ( p )->takeChildren() ), true ) );
   }
}

/*
 * QString text ( int column ) const
 */
HB_FUNC( QT_QTREEWIDGETITEM_TEXT )
{
   QTreeWidgetItem * p = hbqt_par_QTreeWidgetItem( 1 );
   if( p )
   {
      hb_retstr_utf8( ( p )->text( hb_parni( 2 ) ).toUtf8().data() );
   }
}

/*
 * int textAlignment ( int column ) const
 */
HB_FUNC( QT_QTREEWIDGETITEM_TEXTALIGNMENT )
{
   QTreeWidgetItem * p = hbqt_par_QTreeWidgetItem( 1 );
   if( p )
   {
      hb_retni( ( p )->textAlignment( hb_parni( 2 ) ) );
   }
}

/*
 * QString toolTip ( int column ) const
 */
HB_FUNC( QT_QTREEWIDGETITEM_TOOLTIP )
{
   QTreeWidgetItem * p = hbqt_par_QTreeWidgetItem( 1 );
   if( p )
   {
      hb_retstr_utf8( ( p )->toolTip( hb_parni( 2 ) ).toUtf8().data() );
   }
}

/*
 * QTreeWidget * treeWidget () const
 */
HB_FUNC( QT_QTREEWIDGETITEM_TREEWIDGET )
{
   QTreeWidgetItem * p = hbqt_par_QTreeWidgetItem( 1 );
   if( p )
   {
      hb_retptrGC( hbqt_gcAllocate_QTreeWidget( ( p )->treeWidget(), false ) );
   }
}

/*
 * int type () const
 */
HB_FUNC( QT_QTREEWIDGETITEM_TYPE )
{
   QTreeWidgetItem * p = hbqt_par_QTreeWidgetItem( 1 );
   if( p )
   {
      hb_retni( ( p )->type() );
   }
}

/*
 * QString whatsThis ( int column ) const
 */
HB_FUNC( QT_QTREEWIDGETITEM_WHATSTHIS )
{
   QTreeWidgetItem * p = hbqt_par_QTreeWidgetItem( 1 );
   if( p )
   {
      hb_retstr_utf8( ( p )->whatsThis( hb_parni( 2 ) ).toUtf8().data() );
   }
}


/*----------------------------------------------------------------------*/
#endif             /* #if QT_VERSION >= 0x040500 */
/*----------------------------------------------------------------------*/
