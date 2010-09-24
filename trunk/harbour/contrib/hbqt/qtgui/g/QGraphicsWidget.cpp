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
 *  Constructed[ 46/48 [ 95.83% ] ]
 *
 *  *** Unconvered Prototypes ***
 *
 *  void addActions ( QList<QAction *> actions )
 *  void insertActions ( QAction * before, QList<QAction *> actions )
 *
 *  *** Commented out protostypes ***
 *
 *  // const QObjectList & children () const
 *  // virtual void paintWindowFrame ( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget = 0 )
 */

#include <QtCore/QPointer>

#include <QtGui/QGraphicsWidget>


/*
 * QGraphicsWidget ( QGraphicsItem * parent = 0, Qt::WindowFlags wFlags = 0 )
 * ~QGraphicsWidget ()
 */

typedef struct
{
   QPointer< QGraphicsWidget > ph;
   bool bNew;
   PHBQT_GC_FUNC func;
   int type;
} HBQT_GC_T_QGraphicsWidget;

HBQT_GC_FUNC( hbqt_gcRelease_QGraphicsWidget )
{
   QGraphicsWidget  * ph = NULL ;
   HBQT_GC_T_QGraphicsWidget * p = ( HBQT_GC_T_QGraphicsWidget * ) Cargo;

   if( p && p->bNew && p->ph )
   {
      ph = p->ph;
      if( ph )
      {
         const QMetaObject * m = ( ph )->metaObject();
         if( ( QString ) m->className() != ( QString ) "QObject" )
         {
            HB_TRACE( HB_TR_DEBUG, ( "ph=%p %p YES_rel_QGraphicsWidget   /.\\   ", (void*) ph, (void*) p->ph ) );
            delete ( p->ph );
            HB_TRACE( HB_TR_DEBUG, ( "ph=%p %p YES_rel_QGraphicsWidget   \\./   ", (void*) ph, (void*) p->ph ) );
            p->ph = NULL;
         }
         else
         {
            HB_TRACE( HB_TR_DEBUG, ( "ph=%p NO__rel_QGraphicsWidget          ", ph ) );
            p->ph = NULL;
         }
      }
      else
      {
         HB_TRACE( HB_TR_DEBUG, ( "ph=%p DEL_rel_QGraphicsWidget    :     Object already deleted!", ph ) );
         p->ph = NULL;
      }
   }
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "ph=%p PTR_rel_QGraphicsWidget    :    Object not created with new=true", ph ) );
      p->ph = NULL;
   }
}

void * hbqt_gcAllocate_QGraphicsWidget( void * pObj, bool bNew )
{
   HBQT_GC_T_QGraphicsWidget * p = ( HBQT_GC_T_QGraphicsWidget * ) hb_gcAllocate( sizeof( HBQT_GC_T_QGraphicsWidget ), hbqt_gcFuncs() );

   new( & p->ph ) QPointer< QGraphicsWidget >( ( QGraphicsWidget * ) pObj );
   p->bNew = bNew;
   p->func = hbqt_gcRelease_QGraphicsWidget;
   p->type = HBQT_TYPE_QGraphicsWidget;

   if( bNew )
   {
      HB_TRACE( HB_TR_DEBUG, ( "ph=%p    _new_QGraphicsWidget  under p->pq", pObj ) );
   }
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "ph=%p NOT_new_QGraphicsWidget", pObj ) );
   }
   return p;
}

HB_FUNC( QT_QGRAPHICSWIDGET )
{
   QGraphicsWidget * pObj = NULL;

   if( hb_pcount() >= 1 && HB_ISPOINTER( 1 ) )
   {
      pObj = new QGraphicsWidget( hbqt_par_QGraphicsItem( 1 ), ( Qt::WindowFlags ) ( HB_ISNUM( 2 ) ? hb_parni( 2 ) : 0 ) ) ;
   }
   else
   {
      pObj = new QGraphicsWidget() ;
   }

   hb_retptrGC( hbqt_gcAllocate_QGraphicsWidget( ( void * ) pObj, true ) );
}

/*
 * QList<QAction *> actions () const
 */
HB_FUNC( QT_QGRAPHICSWIDGET_ACTIONS )
{
   QGraphicsWidget * p = hbqt_par_QGraphicsWidget( 1 );
   if( p )
   {
      hb_retptrGC( hbqt_gcAllocate_QList( new QList<QAction *>( ( p )->actions() ), true ) );
   }
}

/*
 * void addAction ( QAction * action )
 */
HB_FUNC( QT_QGRAPHICSWIDGET_ADDACTION )
{
   QGraphicsWidget * p = hbqt_par_QGraphicsWidget( 1 );
   if( p )
   {
      ( p )->addAction( hbqt_par_QAction( 2 ) );
   }
}

/*
 * void adjustSize ()
 */
HB_FUNC( QT_QGRAPHICSWIDGET_ADJUSTSIZE )
{
   QGraphicsWidget * p = hbqt_par_QGraphicsWidget( 1 );
   if( p )
   {
      ( p )->adjustSize();
   }
}

/*
 * Qt::FocusPolicy focusPolicy () const
 */
HB_FUNC( QT_QGRAPHICSWIDGET_FOCUSPOLICY )
{
   QGraphicsWidget * p = hbqt_par_QGraphicsWidget( 1 );
   if( p )
   {
      hb_retni( ( Qt::FocusPolicy ) ( p )->focusPolicy() );
   }
}

/*
 * QGraphicsWidget * focusWidget () const
 */
HB_FUNC( QT_QGRAPHICSWIDGET_FOCUSWIDGET )
{
   QGraphicsWidget * p = hbqt_par_QGraphicsWidget( 1 );
   if( p )
   {
      hb_retptrGC( hbqt_gcAllocate_QGraphicsWidget( ( p )->focusWidget(), false ) );
   }
}

/*
 * QFont font () const
 */
HB_FUNC( QT_QGRAPHICSWIDGET_FONT )
{
   QGraphicsWidget * p = hbqt_par_QGraphicsWidget( 1 );
   if( p )
   {
      hb_retptrGC( hbqt_gcAllocate_QFont( new QFont( ( p )->font() ), true ) );
   }
}

/*
 * virtual void getContentsMargins ( qreal * left, qreal * top, qreal * right, qreal * bottom ) const
 */
HB_FUNC( QT_QGRAPHICSWIDGET_GETCONTENTSMARGINS )
{
   QGraphicsWidget * p = hbqt_par_QGraphicsWidget( 1 );
   qreal qrLeft = 0;
   qreal qrTop = 0;
   qreal qrRight = 0;
   qreal qrBottom = 0;

   if( p )
   {
      ( p )->getContentsMargins( &qrLeft, &qrTop, &qrRight, &qrBottom );
   }

   hb_stornd( qrLeft, 2 );
   hb_stornd( qrTop, 3 );
   hb_stornd( qrRight, 4 );
   hb_stornd( qrBottom, 5 );
}

/*
 * void getWindowFrameMargins ( qreal * left, qreal * top, qreal * right, qreal * bottom ) const
 */
HB_FUNC( QT_QGRAPHICSWIDGET_GETWINDOWFRAMEMARGINS )
{
   QGraphicsWidget * p = hbqt_par_QGraphicsWidget( 1 );
   qreal qrLeft = 0;
   qreal qrTop = 0;
   qreal qrRight = 0;
   qreal qrBottom = 0;

   if( p )
   {
      ( p )->getWindowFrameMargins( &qrLeft, &qrTop, &qrRight, &qrBottom );
   }

   hb_stornd( qrLeft, 2 );
   hb_stornd( qrTop, 3 );
   hb_stornd( qrRight, 4 );
   hb_stornd( qrBottom, 5 );
}

/*
 * int grabShortcut ( const QKeySequence & sequence, Qt::ShortcutContext context = Qt::WindowShortcut )
 */
HB_FUNC( QT_QGRAPHICSWIDGET_GRABSHORTCUT )
{
   QGraphicsWidget * p = hbqt_par_QGraphicsWidget( 1 );
   if( p )
   {
      hb_retni( ( p )->grabShortcut( *hbqt_par_QKeySequence( 2 ), ( HB_ISNUM( 3 ) ? ( Qt::ShortcutContext ) hb_parni( 3 ) : ( Qt::ShortcutContext ) Qt::WindowShortcut ) ) );
   }
}

/*
 * void insertAction ( QAction * before, QAction * action )
 */
HB_FUNC( QT_QGRAPHICSWIDGET_INSERTACTION )
{
   QGraphicsWidget * p = hbqt_par_QGraphicsWidget( 1 );
   if( p )
   {
      ( p )->insertAction( hbqt_par_QAction( 2 ), hbqt_par_QAction( 3 ) );
   }
}

/*
 * bool isActiveWindow () const
 */
HB_FUNC( QT_QGRAPHICSWIDGET_ISACTIVEWINDOW )
{
   QGraphicsWidget * p = hbqt_par_QGraphicsWidget( 1 );
   if( p )
   {
      hb_retl( ( p )->isActiveWindow() );
   }
}

/*
 * QGraphicsLayout * layout () const
 */
HB_FUNC( QT_QGRAPHICSWIDGET_LAYOUT )
{
   QGraphicsWidget * p = hbqt_par_QGraphicsWidget( 1 );
   if( p )
   {
      hb_retptrGC( hbqt_gcAllocate_QGraphicsLayout( ( p )->layout(), false ) );
   }
}

/*
 * Qt::LayoutDirection layoutDirection () const
 */
HB_FUNC( QT_QGRAPHICSWIDGET_LAYOUTDIRECTION )
{
   QGraphicsWidget * p = hbqt_par_QGraphicsWidget( 1 );
   if( p )
   {
      hb_retni( ( Qt::LayoutDirection ) ( p )->layoutDirection() );
   }
}

/*
 * QPalette palette () const
 */
HB_FUNC( QT_QGRAPHICSWIDGET_PALETTE )
{
   QGraphicsWidget * p = hbqt_par_QGraphicsWidget( 1 );
   if( p )
   {
      hb_retptrGC( hbqt_gcAllocate_QPalette( new QPalette( ( p )->palette() ), true ) );
   }
}

/*
 * QRectF rect () const
 */
HB_FUNC( QT_QGRAPHICSWIDGET_RECT )
{
   QGraphicsWidget * p = hbqt_par_QGraphicsWidget( 1 );
   if( p )
   {
      hb_retptrGC( hbqt_gcAllocate_QRectF( new QRectF( ( p )->rect() ), true ) );
   }
}

/*
 * void releaseShortcut ( int id )
 */
HB_FUNC( QT_QGRAPHICSWIDGET_RELEASESHORTCUT )
{
   QGraphicsWidget * p = hbqt_par_QGraphicsWidget( 1 );
   if( p )
   {
      ( p )->releaseShortcut( hb_parni( 2 ) );
   }
}

/*
 * void removeAction ( QAction * action )
 */
HB_FUNC( QT_QGRAPHICSWIDGET_REMOVEACTION )
{
   QGraphicsWidget * p = hbqt_par_QGraphicsWidget( 1 );
   if( p )
   {
      ( p )->removeAction( hbqt_par_QAction( 2 ) );
   }
}

/*
 * void resize ( const QSizeF & size )
 */
HB_FUNC( QT_QGRAPHICSWIDGET_RESIZE )
{
   QGraphicsWidget * p = hbqt_par_QGraphicsWidget( 1 );
   if( p )
   {
      ( p )->resize( *hbqt_par_QSizeF( 2 ) );
   }
}

/*
 * void resize ( qreal w, qreal h )
 */
HB_FUNC( QT_QGRAPHICSWIDGET_RESIZE_1 )
{
   QGraphicsWidget * p = hbqt_par_QGraphicsWidget( 1 );
   if( p )
   {
      ( p )->resize( hb_parnd( 2 ), hb_parnd( 3 ) );
   }
}

/*
 * void setAttribute ( Qt::WidgetAttribute attribute, bool on = true )
 */
HB_FUNC( QT_QGRAPHICSWIDGET_SETATTRIBUTE )
{
   QGraphicsWidget * p = hbqt_par_QGraphicsWidget( 1 );
   if( p )
   {
      ( p )->setAttribute( ( Qt::WidgetAttribute ) hb_parni( 2 ), hb_parl( 3 ) );
   }
}

/*
 * void setContentsMargins ( qreal left, qreal top, qreal right, qreal bottom )
 */
HB_FUNC( QT_QGRAPHICSWIDGET_SETCONTENTSMARGINS )
{
   QGraphicsWidget * p = hbqt_par_QGraphicsWidget( 1 );
   if( p )
   {
      ( p )->setContentsMargins( hb_parnd( 2 ), hb_parnd( 3 ), hb_parnd( 4 ), hb_parnd( 5 ) );
   }
}

/*
 * void setFocusPolicy ( Qt::FocusPolicy policy )
 */
HB_FUNC( QT_QGRAPHICSWIDGET_SETFOCUSPOLICY )
{
   QGraphicsWidget * p = hbqt_par_QGraphicsWidget( 1 );
   if( p )
   {
      ( p )->setFocusPolicy( ( Qt::FocusPolicy ) hb_parni( 2 ) );
   }
}

/*
 * void setFont ( const QFont & font )
 */
HB_FUNC( QT_QGRAPHICSWIDGET_SETFONT )
{
   QGraphicsWidget * p = hbqt_par_QGraphicsWidget( 1 );
   if( p )
   {
      ( p )->setFont( *hbqt_par_QFont( 2 ) );
   }
}

/*
 * virtual void setGeometry ( const QRectF & rect )
 */
HB_FUNC( QT_QGRAPHICSWIDGET_SETGEOMETRY )
{
   QGraphicsWidget * p = hbqt_par_QGraphicsWidget( 1 );
   if( p )
   {
      ( p )->setGeometry( *hbqt_par_QRectF( 2 ) );
   }
}

/*
 * void setGeometry ( qreal x, qreal y, qreal w, qreal h )
 */
HB_FUNC( QT_QGRAPHICSWIDGET_SETGEOMETRY_1 )
{
   QGraphicsWidget * p = hbqt_par_QGraphicsWidget( 1 );
   if( p )
   {
      ( p )->setGeometry( hb_parnd( 2 ), hb_parnd( 3 ), hb_parnd( 4 ), hb_parnd( 5 ) );
   }
}

/*
 * void setLayout ( QGraphicsLayout * layout )
 */
HB_FUNC( QT_QGRAPHICSWIDGET_SETLAYOUT )
{
   QGraphicsWidget * p = hbqt_par_QGraphicsWidget( 1 );
   if( p )
   {
      ( p )->setLayout( hbqt_par_QGraphicsLayout( 2 ) );
   }
}

/*
 * void setLayoutDirection ( Qt::LayoutDirection direction )
 */
HB_FUNC( QT_QGRAPHICSWIDGET_SETLAYOUTDIRECTION )
{
   QGraphicsWidget * p = hbqt_par_QGraphicsWidget( 1 );
   if( p )
   {
      ( p )->setLayoutDirection( ( Qt::LayoutDirection ) hb_parni( 2 ) );
   }
}

/*
 * void setPalette ( const QPalette & palette )
 */
HB_FUNC( QT_QGRAPHICSWIDGET_SETPALETTE )
{
   QGraphicsWidget * p = hbqt_par_QGraphicsWidget( 1 );
   if( p )
   {
      ( p )->setPalette( *hbqt_par_QPalette( 2 ) );
   }
}

/*
 * void setShortcutAutoRepeat ( int id, bool enabled = true )
 */
HB_FUNC( QT_QGRAPHICSWIDGET_SETSHORTCUTAUTOREPEAT )
{
   QGraphicsWidget * p = hbqt_par_QGraphicsWidget( 1 );
   if( p )
   {
      ( p )->setShortcutAutoRepeat( hb_parni( 2 ), hb_parl( 3 ) );
   }
}

/*
 * void setShortcutEnabled ( int id, bool enabled = true )
 */
HB_FUNC( QT_QGRAPHICSWIDGET_SETSHORTCUTENABLED )
{
   QGraphicsWidget * p = hbqt_par_QGraphicsWidget( 1 );
   if( p )
   {
      ( p )->setShortcutEnabled( hb_parni( 2 ), hb_parl( 3 ) );
   }
}

/*
 * void setStyle ( QStyle * style )
 */
HB_FUNC( QT_QGRAPHICSWIDGET_SETSTYLE )
{
   QGraphicsWidget * p = hbqt_par_QGraphicsWidget( 1 );
   if( p )
   {
      ( p )->setStyle( hbqt_par_QStyle( 2 ) );
   }
}

/*
 * void setWindowFlags ( Qt::WindowFlags wFlags )
 */
HB_FUNC( QT_QGRAPHICSWIDGET_SETWINDOWFLAGS )
{
   QGraphicsWidget * p = hbqt_par_QGraphicsWidget( 1 );
   if( p )
   {
      ( p )->setWindowFlags( ( Qt::WindowFlags ) hb_parni( 2 ) );
   }
}

/*
 * void setWindowFrameMargins ( qreal left, qreal top, qreal right, qreal bottom )
 */
HB_FUNC( QT_QGRAPHICSWIDGET_SETWINDOWFRAMEMARGINS )
{
   QGraphicsWidget * p = hbqt_par_QGraphicsWidget( 1 );
   if( p )
   {
      ( p )->setWindowFrameMargins( hb_parnd( 2 ), hb_parnd( 3 ), hb_parnd( 4 ), hb_parnd( 5 ) );
   }
}

/*
 * void setWindowTitle ( const QString & title )
 */
HB_FUNC( QT_QGRAPHICSWIDGET_SETWINDOWTITLE )
{
   QGraphicsWidget * p = hbqt_par_QGraphicsWidget( 1 );
   if( p )
   {
      void * pText;
      ( p )->setWindowTitle( hb_parstr_utf8( 2, &pText, NULL ) );
      hb_strfree( pText );
   }
}

/*
 * QSizeF size () const
 */
HB_FUNC( QT_QGRAPHICSWIDGET_SIZE )
{
   QGraphicsWidget * p = hbqt_par_QGraphicsWidget( 1 );
   if( p )
   {
      hb_retptrGC( hbqt_gcAllocate_QSizeF( new QSizeF( ( p )->size() ), true ) );
   }
}

/*
 * QStyle * style () const
 */
HB_FUNC( QT_QGRAPHICSWIDGET_STYLE )
{
   QGraphicsWidget * p = hbqt_par_QGraphicsWidget( 1 );
   if( p )
   {
      hb_retptrGC( hbqt_gcAllocate_QStyle( ( p )->style(), false ) );
   }
}

/*
 * bool testAttribute ( Qt::WidgetAttribute attribute ) const
 */
HB_FUNC( QT_QGRAPHICSWIDGET_TESTATTRIBUTE )
{
   QGraphicsWidget * p = hbqt_par_QGraphicsWidget( 1 );
   if( p )
   {
      hb_retl( ( p )->testAttribute( ( Qt::WidgetAttribute ) hb_parni( 2 ) ) );
   }
}

/*
 * void unsetLayoutDirection ()
 */
HB_FUNC( QT_QGRAPHICSWIDGET_UNSETLAYOUTDIRECTION )
{
   QGraphicsWidget * p = hbqt_par_QGraphicsWidget( 1 );
   if( p )
   {
      ( p )->unsetLayoutDirection();
   }
}

/*
 * void unsetWindowFrameMargins ()
 */
HB_FUNC( QT_QGRAPHICSWIDGET_UNSETWINDOWFRAMEMARGINS )
{
   QGraphicsWidget * p = hbqt_par_QGraphicsWidget( 1 );
   if( p )
   {
      ( p )->unsetWindowFrameMargins();
   }
}

/*
 * Qt::WindowFlags windowFlags () const
 */
HB_FUNC( QT_QGRAPHICSWIDGET_WINDOWFLAGS )
{
   QGraphicsWidget * p = hbqt_par_QGraphicsWidget( 1 );
   if( p )
   {
      hb_retni( ( Qt::WindowFlags ) ( p )->windowFlags() );
   }
}

/*
 * QRectF windowFrameGeometry () const
 */
HB_FUNC( QT_QGRAPHICSWIDGET_WINDOWFRAMEGEOMETRY )
{
   QGraphicsWidget * p = hbqt_par_QGraphicsWidget( 1 );
   if( p )
   {
      hb_retptrGC( hbqt_gcAllocate_QRectF( new QRectF( ( p )->windowFrameGeometry() ), true ) );
   }
}

/*
 * QRectF windowFrameRect () const
 */
HB_FUNC( QT_QGRAPHICSWIDGET_WINDOWFRAMERECT )
{
   QGraphicsWidget * p = hbqt_par_QGraphicsWidget( 1 );
   if( p )
   {
      hb_retptrGC( hbqt_gcAllocate_QRectF( new QRectF( ( p )->windowFrameRect() ), true ) );
   }
}

/*
 * QString windowTitle () const
 */
HB_FUNC( QT_QGRAPHICSWIDGET_WINDOWTITLE )
{
   QGraphicsWidget * p = hbqt_par_QGraphicsWidget( 1 );
   if( p )
   {
      hb_retstr_utf8( ( p )->windowTitle().toUtf8().data() );
   }
}

/*
 * Qt::WindowType windowType () const
 */
HB_FUNC( QT_QGRAPHICSWIDGET_WINDOWTYPE )
{
   QGraphicsWidget * p = hbqt_par_QGraphicsWidget( 1 );
   if( p )
   {
      hb_retni( ( Qt::WindowType ) ( p )->windowType() );
   }
}

/*
 * void setTabOrder ( QGraphicsWidget * first, QGraphicsWidget * second )
 */
HB_FUNC( QT_QGRAPHICSWIDGET_SETTABORDER )
{
   QGraphicsWidget * p = hbqt_par_QGraphicsWidget( 1 );
   if( p )
   {
      ( p )->setTabOrder( hbqt_par_QGraphicsWidget( 2 ), hbqt_par_QGraphicsWidget( 3 ) );
   }
}

/*
 * bool close ()
 */
HB_FUNC( QT_QGRAPHICSWIDGET_CLOSE )
{
   QGraphicsWidget * p = hbqt_par_QGraphicsWidget( 1 );
   if( p )
   {
      hb_retl( ( p )->close() );
   }
}


/*----------------------------------------------------------------------*/
#endif             /* #if QT_VERSION >= 0x040500 */
/*----------------------------------------------------------------------*/
