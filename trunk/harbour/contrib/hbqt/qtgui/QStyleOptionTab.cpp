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
 *  enum CornerWidget { NoCornerWidgets, LeftCornerWidget, RightCornerWidget }
 *  flags CornerWidgets
 *  enum SelectedPosition { NotAdjacent, NextIsSelected, PreviousIsSelected }
 *  enum StyleOptionType { Type }
 *  enum StyleOptionVersion { Version }
 *  enum TabPosition { Beginning, Middle, End, OnlyOneTab }
 */

#include <QtCore/QPointer>

#include <QtGui/QStyleOptionTab>


/*
 * QStyleOptionTab ()
 * QStyleOptionTab ( const QStyleOptionTab & other )
 */

typedef struct
{
   QStyleOptionTab * ph;
   bool bNew;
   QT_G_FUNC_PTR func;
   int type;
} QGC_POINTER_QStyleOptionTab;

QT_G_FUNC( hbqt_gcRelease_QStyleOptionTab )
{
   QGC_POINTER * p = ( QGC_POINTER * ) Cargo;

   if( p && p->bNew )
   {
      if( p->ph )
      {
         HB_TRACE( HB_TR_DEBUG, ( "ph=%p    _rel_QStyleOptionTab   /.\\", p->ph ) );
         delete ( ( QStyleOptionTab * ) p->ph );
         HB_TRACE( HB_TR_DEBUG, ( "ph=%p YES_rel_QStyleOptionTab   \\./", p->ph ) );
         p->ph = NULL;
      }
      else
      {
         HB_TRACE( HB_TR_DEBUG, ( "ph=%p DEL_rel_QStyleOptionTab    :     Object already deleted!", p->ph ) );
         p->ph = NULL;
      }
   }
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "ph=%p PTR_rel_QStyleOptionTab    :    Object not created with new=true", p->ph ) );
      p->ph = NULL;
   }
}

void * hbqt_gcAllocate_QStyleOptionTab( void * pObj, bool bNew )
{
   QGC_POINTER * p = ( QGC_POINTER * ) hb_gcAllocate( sizeof( QGC_POINTER ), hbqt_gcFuncs() );

   p->ph = ( QStyleOptionTab * ) pObj;
   p->bNew = bNew;
   p->func = hbqt_gcRelease_QStyleOptionTab;
   p->type = HBQT_TYPE_QStyleOptionTab;

   if( bNew )
   {
      HB_TRACE( HB_TR_DEBUG, ( "ph=%p    _new_QStyleOptionTab", pObj ) );
   }
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "ph=%p NOT_new_QStyleOptionTab", pObj ) );
   }
   return p;
}

HB_FUNC( QT_QSTYLEOPTIONTAB )
{
   QStyleOptionTab * pObj = NULL;

   pObj =  new QStyleOptionTab() ;

   hb_retptrGC( hbqt_gcAllocate_QStyleOptionTab( ( void * ) pObj, true ) );
}

/*
 * CornerWidgets cornerWidgets
 */
HB_FUNC( QT_QSTYLEOPTIONTAB_CORNERWIDGETS )
{
   hb_retni( ( QStyleOptionTab::CornerWidgets ) hbqt_par_QStyleOptionTab( 1 )->cornerWidgets );
}

/*
 * QIcon icon
 */
HB_FUNC( QT_QSTYLEOPTIONTAB_ICON )
{
   hb_retptrGC( hbqt_gcAllocate_QIcon( new QIcon( hbqt_par_QStyleOptionTab( 1 )->icon ), true ) );
}

/*
 * TabPosition position
 */
HB_FUNC( QT_QSTYLEOPTIONTAB_POSITION )
{
   hb_retni( ( QStyleOptionTab::TabPosition ) hbqt_par_QStyleOptionTab( 1 )->position );
}

/*
 * int row
 */
HB_FUNC( QT_QSTYLEOPTIONTAB_ROW )
{
   hb_retni( hbqt_par_QStyleOptionTab( 1 )->row );
}

/*
 * SelectedPosition selectedPosition
 */
HB_FUNC( QT_QSTYLEOPTIONTAB_SELECTEDPOSITION )
{
   hb_retni( ( QStyleOptionTab::SelectedPosition ) hbqt_par_QStyleOptionTab( 1 )->selectedPosition );
}

/*
 * QTabBar::Shape shape
 */
HB_FUNC( QT_QSTYLEOPTIONTAB_SHAPE )
{
   hb_retni( ( QTabBar::Shape ) hbqt_par_QStyleOptionTab( 1 )->shape );
}

/*
 * QString text
 */
HB_FUNC( QT_QSTYLEOPTIONTAB_TEXT )
{
   hb_retc( hbqt_par_QStyleOptionTab( 1 )->text.toLatin1().data() );
}


/*----------------------------------------------------------------------*/
#endif             /* #if QT_VERSION >= 0x040500 */
/*----------------------------------------------------------------------*/
