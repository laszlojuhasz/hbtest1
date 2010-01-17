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


CREATE CLASS QMdiArea INHERIT HbQtObjectHandler, QAbstractScrollArea

   METHOD  new( ... )

   METHOD  activationOrder()
   METHOD  activeSubWindow()
   METHOD  addSubWindow( pWidget, nWindowFlags )
   METHOD  background()
   METHOD  currentSubWindow()
   METHOD  documentMode()
   METHOD  removeSubWindow( pWidget )
   METHOD  setActivationOrder( nOrder )
   METHOD  setBackground( pBackground )
   METHOD  setDocumentMode( lEnabled )
   METHOD  setOption( nOption, lOn )
   METHOD  setTabPosition( nPosition )
   METHOD  setTabShape( nShape )
   METHOD  setViewMode( nMode )
   METHOD  tabPosition()
   METHOD  tabShape()
   METHOD  testOption( nOption )
   METHOD  viewMode()
   METHOD  activateNextSubWindow()
   METHOD  activatePreviousSubWindow()
   METHOD  cascadeSubWindows()
   METHOD  closeActiveSubWindow()
   METHOD  closeAllSubWindows()
   METHOD  setActiveSubWindow( pWindow )
   METHOD  tileSubWindows()

   ENDCLASS


METHOD QMdiArea:new( ... )
   LOCAL p
   FOR EACH p IN { ... }
      hb_pvalue( p:__enumIndex(), hbqt_ptr( p ) )
   NEXT
   ::pPtr := Qt_QMdiArea( ... )
   RETURN Self


METHOD QMdiArea:activationOrder()
   RETURN Qt_QMdiArea_activationOrder( ::pPtr )


METHOD QMdiArea:activeSubWindow()
   RETURN Qt_QMdiArea_activeSubWindow( ::pPtr )


METHOD QMdiArea:addSubWindow( pWidget, nWindowFlags )
   RETURN Qt_QMdiArea_addSubWindow( ::pPtr, hbqt_ptr( pWidget ), nWindowFlags )


METHOD QMdiArea:background()
   RETURN Qt_QMdiArea_background( ::pPtr )


METHOD QMdiArea:currentSubWindow()
   RETURN Qt_QMdiArea_currentSubWindow( ::pPtr )


METHOD QMdiArea:documentMode()
   RETURN Qt_QMdiArea_documentMode( ::pPtr )


METHOD QMdiArea:removeSubWindow( pWidget )
   RETURN Qt_QMdiArea_removeSubWindow( ::pPtr, hbqt_ptr( pWidget ) )


METHOD QMdiArea:setActivationOrder( nOrder )
   RETURN Qt_QMdiArea_setActivationOrder( ::pPtr, nOrder )


METHOD QMdiArea:setBackground( pBackground )
   RETURN Qt_QMdiArea_setBackground( ::pPtr, hbqt_ptr( pBackground ) )


METHOD QMdiArea:setDocumentMode( lEnabled )
   RETURN Qt_QMdiArea_setDocumentMode( ::pPtr, lEnabled )


METHOD QMdiArea:setOption( nOption, lOn )
   RETURN Qt_QMdiArea_setOption( ::pPtr, nOption, lOn )


METHOD QMdiArea:setTabPosition( nPosition )
   RETURN Qt_QMdiArea_setTabPosition( ::pPtr, nPosition )


METHOD QMdiArea:setTabShape( nShape )
   RETURN Qt_QMdiArea_setTabShape( ::pPtr, nShape )


METHOD QMdiArea:setViewMode( nMode )
   RETURN Qt_QMdiArea_setViewMode( ::pPtr, nMode )


METHOD QMdiArea:tabPosition()
   RETURN Qt_QMdiArea_tabPosition( ::pPtr )


METHOD QMdiArea:tabShape()
   RETURN Qt_QMdiArea_tabShape( ::pPtr )


METHOD QMdiArea:testOption( nOption )
   RETURN Qt_QMdiArea_testOption( ::pPtr, nOption )


METHOD QMdiArea:viewMode()
   RETURN Qt_QMdiArea_viewMode( ::pPtr )


METHOD QMdiArea:activateNextSubWindow()
   RETURN Qt_QMdiArea_activateNextSubWindow( ::pPtr )


METHOD QMdiArea:activatePreviousSubWindow()
   RETURN Qt_QMdiArea_activatePreviousSubWindow( ::pPtr )


METHOD QMdiArea:cascadeSubWindows()
   RETURN Qt_QMdiArea_cascadeSubWindows( ::pPtr )


METHOD QMdiArea:closeActiveSubWindow()
   RETURN Qt_QMdiArea_closeActiveSubWindow( ::pPtr )


METHOD QMdiArea:closeAllSubWindows()
   RETURN Qt_QMdiArea_closeAllSubWindows( ::pPtr )


METHOD QMdiArea:setActiveSubWindow( pWindow )
   RETURN Qt_QMdiArea_setActiveSubWindow( ::pPtr, hbqt_ptr( pWindow ) )


METHOD QMdiArea:tileSubWindows()
   RETURN Qt_QMdiArea_tileSubWindows( ::pPtr )

