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


#include "hbclass.ch"


CREATE CLASS QGraphicsLayoutItem INHERIT HbQtObjectHandler

   METHOD  new( ... )

   METHOD  contentsRect()
   METHOD  effectiveSizeHint( nWhich, pConstraint )
   METHOD  geometry()
   METHOD  getContentsMargins( nLeft, nTop, nRight, nBottom )
   METHOD  graphicsItem()
   METHOD  isLayout()
   METHOD  maximumHeight()
   METHOD  maximumSize()
   METHOD  maximumWidth()
   METHOD  minimumHeight()
   METHOD  minimumSize()
   METHOD  minimumWidth()
   METHOD  ownedByLayout()
   METHOD  parentLayoutItem()
   METHOD  preferredHeight()
   METHOD  preferredSize()
   METHOD  preferredWidth()
   METHOD  setGeometry( pRect )
   METHOD  setMaximumHeight( nHeight )
   METHOD  setMaximumSize( pSize )
   METHOD  setMaximumSize_1( nW, nH )
   METHOD  setMaximumWidth( nWidth )
   METHOD  setMinimumHeight( nHeight )
   METHOD  setMinimumSize( pSize )
   METHOD  setMinimumSize_1( nW, nH )
   METHOD  setMinimumWidth( nWidth )
   METHOD  setParentLayoutItem( pParent )
   METHOD  setPreferredHeight( nHeight )
   METHOD  setPreferredSize( pSize )
   METHOD  setPreferredSize_1( nW, nH )
   METHOD  setPreferredWidth( nWidth )
   METHOD  setSizePolicy( pPolicy )
   METHOD  setSizePolicy_1( nHPolicy, nVPolicy, nControlType )
   METHOD  sizePolicy()
   METHOD  updateGeometry()

   ENDCLASS


METHOD QGraphicsLayoutItem:new( ... )
   LOCAL p
   FOR EACH p IN { ... }
      hb_pvalue( p:__enumIndex(), hbqt_ptr( p ) )
   NEXT
   ::pPtr := Qt_QGraphicsLayoutItem( ... )
   RETURN Self


METHOD QGraphicsLayoutItem:contentsRect()
   RETURN Qt_QGraphicsLayoutItem_contentsRect( ::pPtr )


METHOD QGraphicsLayoutItem:effectiveSizeHint( nWhich, pConstraint )
   RETURN Qt_QGraphicsLayoutItem_effectiveSizeHint( ::pPtr, nWhich, hbqt_ptr( pConstraint ) )


METHOD QGraphicsLayoutItem:geometry()
   RETURN Qt_QGraphicsLayoutItem_geometry( ::pPtr )


METHOD QGraphicsLayoutItem:getContentsMargins( nLeft, nTop, nRight, nBottom )
   RETURN Qt_QGraphicsLayoutItem_getContentsMargins( ::pPtr, nLeft, nTop, nRight, nBottom )


METHOD QGraphicsLayoutItem:graphicsItem()
   RETURN Qt_QGraphicsLayoutItem_graphicsItem( ::pPtr )


METHOD QGraphicsLayoutItem:isLayout()
   RETURN Qt_QGraphicsLayoutItem_isLayout( ::pPtr )


METHOD QGraphicsLayoutItem:maximumHeight()
   RETURN Qt_QGraphicsLayoutItem_maximumHeight( ::pPtr )


METHOD QGraphicsLayoutItem:maximumSize()
   RETURN Qt_QGraphicsLayoutItem_maximumSize( ::pPtr )


METHOD QGraphicsLayoutItem:maximumWidth()
   RETURN Qt_QGraphicsLayoutItem_maximumWidth( ::pPtr )


METHOD QGraphicsLayoutItem:minimumHeight()
   RETURN Qt_QGraphicsLayoutItem_minimumHeight( ::pPtr )


METHOD QGraphicsLayoutItem:minimumSize()
   RETURN Qt_QGraphicsLayoutItem_minimumSize( ::pPtr )


METHOD QGraphicsLayoutItem:minimumWidth()
   RETURN Qt_QGraphicsLayoutItem_minimumWidth( ::pPtr )


METHOD QGraphicsLayoutItem:ownedByLayout()
   RETURN Qt_QGraphicsLayoutItem_ownedByLayout( ::pPtr )


METHOD QGraphicsLayoutItem:parentLayoutItem()
   RETURN Qt_QGraphicsLayoutItem_parentLayoutItem( ::pPtr )


METHOD QGraphicsLayoutItem:preferredHeight()
   RETURN Qt_QGraphicsLayoutItem_preferredHeight( ::pPtr )


METHOD QGraphicsLayoutItem:preferredSize()
   RETURN Qt_QGraphicsLayoutItem_preferredSize( ::pPtr )


METHOD QGraphicsLayoutItem:preferredWidth()
   RETURN Qt_QGraphicsLayoutItem_preferredWidth( ::pPtr )


METHOD QGraphicsLayoutItem:setGeometry( pRect )
   RETURN Qt_QGraphicsLayoutItem_setGeometry( ::pPtr, hbqt_ptr( pRect ) )


METHOD QGraphicsLayoutItem:setMaximumHeight( nHeight )
   RETURN Qt_QGraphicsLayoutItem_setMaximumHeight( ::pPtr, nHeight )


METHOD QGraphicsLayoutItem:setMaximumSize( pSize )
   RETURN Qt_QGraphicsLayoutItem_setMaximumSize( ::pPtr, hbqt_ptr( pSize ) )


METHOD QGraphicsLayoutItem:setMaximumSize_1( nW, nH )
   RETURN Qt_QGraphicsLayoutItem_setMaximumSize_1( ::pPtr, nW, nH )


METHOD QGraphicsLayoutItem:setMaximumWidth( nWidth )
   RETURN Qt_QGraphicsLayoutItem_setMaximumWidth( ::pPtr, nWidth )


METHOD QGraphicsLayoutItem:setMinimumHeight( nHeight )
   RETURN Qt_QGraphicsLayoutItem_setMinimumHeight( ::pPtr, nHeight )


METHOD QGraphicsLayoutItem:setMinimumSize( pSize )
   RETURN Qt_QGraphicsLayoutItem_setMinimumSize( ::pPtr, hbqt_ptr( pSize ) )


METHOD QGraphicsLayoutItem:setMinimumSize_1( nW, nH )
   RETURN Qt_QGraphicsLayoutItem_setMinimumSize_1( ::pPtr, nW, nH )


METHOD QGraphicsLayoutItem:setMinimumWidth( nWidth )
   RETURN Qt_QGraphicsLayoutItem_setMinimumWidth( ::pPtr, nWidth )


METHOD QGraphicsLayoutItem:setParentLayoutItem( pParent )
   RETURN Qt_QGraphicsLayoutItem_setParentLayoutItem( ::pPtr, hbqt_ptr( pParent ) )


METHOD QGraphicsLayoutItem:setPreferredHeight( nHeight )
   RETURN Qt_QGraphicsLayoutItem_setPreferredHeight( ::pPtr, nHeight )


METHOD QGraphicsLayoutItem:setPreferredSize( pSize )
   RETURN Qt_QGraphicsLayoutItem_setPreferredSize( ::pPtr, hbqt_ptr( pSize ) )


METHOD QGraphicsLayoutItem:setPreferredSize_1( nW, nH )
   RETURN Qt_QGraphicsLayoutItem_setPreferredSize_1( ::pPtr, nW, nH )


METHOD QGraphicsLayoutItem:setPreferredWidth( nWidth )
   RETURN Qt_QGraphicsLayoutItem_setPreferredWidth( ::pPtr, nWidth )


METHOD QGraphicsLayoutItem:setSizePolicy( pPolicy )
   RETURN Qt_QGraphicsLayoutItem_setSizePolicy( ::pPtr, hbqt_ptr( pPolicy ) )


METHOD QGraphicsLayoutItem:setSizePolicy_1( nHPolicy, nVPolicy, nControlType )
   RETURN Qt_QGraphicsLayoutItem_setSizePolicy_1( ::pPtr, nHPolicy, nVPolicy, nControlType )


METHOD QGraphicsLayoutItem:sizePolicy()
   RETURN Qt_QGraphicsLayoutItem_sizePolicy( ::pPtr )


METHOD QGraphicsLayoutItem:updateGeometry()
   RETURN Qt_QGraphicsLayoutItem_updateGeometry( ::pPtr )

