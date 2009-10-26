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


#include "hbclass.ch"


CREATE CLASS QWebHistory

   VAR     pPtr

   METHOD  new()
   METHOD  configure( xObject )

   METHOD  back()
   METHOD  backItem()
   METHOD  canGoBack()
   METHOD  canGoForward()
   METHOD  clear()
   METHOD  count()
   METHOD  currentItem()
   METHOD  currentItemIndex()
   METHOD  forward()
   METHOD  forwardItem()
   METHOD  goToItem( pItem )
   METHOD  itemAt( nI )
   METHOD  maximumItemCount()
   METHOD  setMaximumItemCount( nCount )

   ENDCLASS

/*----------------------------------------------------------------------*/

METHOD QWebHistory:new( pParent )
   ::pPtr := Qt_QWebHistory( pParent )
   RETURN Self


METHOD QWebHistory:configure( xObject )
   IF hb_isObject( xObject )
      ::pPtr := xObject:pPtr
   ELSEIF hb_isPointer( xObject )
      ::pPtr := xObject
   ENDIF
   RETURN Self


METHOD QWebHistory:back()
   RETURN Qt_QWebHistory_back( ::pPtr )


METHOD QWebHistory:backItem()
   RETURN Qt_QWebHistory_backItem( ::pPtr )


METHOD QWebHistory:canGoBack()
   RETURN Qt_QWebHistory_canGoBack( ::pPtr )


METHOD QWebHistory:canGoForward()
   RETURN Qt_QWebHistory_canGoForward( ::pPtr )


METHOD QWebHistory:clear()
   RETURN Qt_QWebHistory_clear( ::pPtr )


METHOD QWebHistory:count()
   RETURN Qt_QWebHistory_count( ::pPtr )


METHOD QWebHistory:currentItem()
   RETURN Qt_QWebHistory_currentItem( ::pPtr )


METHOD QWebHistory:currentItemIndex()
   RETURN Qt_QWebHistory_currentItemIndex( ::pPtr )


METHOD QWebHistory:forward()
   RETURN Qt_QWebHistory_forward( ::pPtr )


METHOD QWebHistory:forwardItem()
   RETURN Qt_QWebHistory_forwardItem( ::pPtr )


METHOD QWebHistory:goToItem( pItem )
   RETURN Qt_QWebHistory_goToItem( ::pPtr, pItem )


METHOD QWebHistory:itemAt( nI )
   RETURN Qt_QWebHistory_itemAt( ::pPtr, nI )


METHOD QWebHistory:maximumItemCount()
   RETURN Qt_QWebHistory_maximumItemCount( ::pPtr )


METHOD QWebHistory:setMaximumItemCount( nCount )
   RETURN Qt_QWebHistory_setMaximumItemCount( ::pPtr, nCount )

