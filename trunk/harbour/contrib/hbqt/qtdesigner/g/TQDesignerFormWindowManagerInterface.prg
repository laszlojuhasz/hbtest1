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


#include "hbclass.ch"


FUNCTION QDesignerFormWindowManagerInterface( ... )
   RETURN HB_QDesignerFormWindowManagerInterface():new( ... )


CREATE CLASS QDesignerFormWindowManagerInterface INHERIT HbQtObjectHandler, HB_QObject FUNCTION HB_QDesignerFormWindowManagerInterface

   METHOD  new( ... )

   METHOD  actionAdjustSize()
   METHOD  actionBreakLayout()
   METHOD  actionCopy()
   METHOD  actionCut()
   METHOD  actionDelete()
   METHOD  actionFormLayout()
   METHOD  actionGridLayout()
   METHOD  actionHorizontalLayout()
   METHOD  actionLower()
   METHOD  actionPaste()
   METHOD  actionRaise()
   METHOD  actionRedo()
   METHOD  actionSelectAll()
   METHOD  actionSimplifyLayout()
   METHOD  actionSplitHorizontal()
   METHOD  actionSplitVertical()
   METHOD  actionUndo()
   METHOD  actionVerticalLayout()
   METHOD  activeFormWindow()
   METHOD  core()
   METHOD  createFormWindow( pParent, nFlags )
   METHOD  formWindow( nIndex )
   METHOD  formWindowCount()
   METHOD  addFormWindow( pFormWindow )
   METHOD  removeFormWindow( pFormWindow )
   METHOD  setActiveFormWindow( pFormWindow )

   ENDCLASS


METHOD QDesignerFormWindowManagerInterface:new( ... )
   LOCAL p
   FOR EACH p IN { ... }
      hb_pvalue( p:__enumIndex(), hbqt_ptr( p ) )
   NEXT
   ::pPtr := Qt_QDesignerFormWindowManagerInterface( ... )
   RETURN Self


METHOD QDesignerFormWindowManagerInterface:actionAdjustSize()
   RETURN HB_QAction():from( Qt_QDesignerFormWindowManagerInterface_actionAdjustSize( ::pPtr ) )


METHOD QDesignerFormWindowManagerInterface:actionBreakLayout()
   RETURN HB_QAction():from( Qt_QDesignerFormWindowManagerInterface_actionBreakLayout( ::pPtr ) )


METHOD QDesignerFormWindowManagerInterface:actionCopy()
   RETURN HB_QAction():from( Qt_QDesignerFormWindowManagerInterface_actionCopy( ::pPtr ) )


METHOD QDesignerFormWindowManagerInterface:actionCut()
   RETURN HB_QAction():from( Qt_QDesignerFormWindowManagerInterface_actionCut( ::pPtr ) )


METHOD QDesignerFormWindowManagerInterface:actionDelete()
   RETURN HB_QAction():from( Qt_QDesignerFormWindowManagerInterface_actionDelete( ::pPtr ) )


METHOD QDesignerFormWindowManagerInterface:actionFormLayout()
   RETURN HB_QAction():from( Qt_QDesignerFormWindowManagerInterface_actionFormLayout( ::pPtr ) )


METHOD QDesignerFormWindowManagerInterface:actionGridLayout()
   RETURN HB_QAction():from( Qt_QDesignerFormWindowManagerInterface_actionGridLayout( ::pPtr ) )


METHOD QDesignerFormWindowManagerInterface:actionHorizontalLayout()
   RETURN HB_QAction():from( Qt_QDesignerFormWindowManagerInterface_actionHorizontalLayout( ::pPtr ) )


METHOD QDesignerFormWindowManagerInterface:actionLower()
   RETURN HB_QAction():from( Qt_QDesignerFormWindowManagerInterface_actionLower( ::pPtr ) )


METHOD QDesignerFormWindowManagerInterface:actionPaste()
   RETURN HB_QAction():from( Qt_QDesignerFormWindowManagerInterface_actionPaste( ::pPtr ) )


METHOD QDesignerFormWindowManagerInterface:actionRaise()
   RETURN HB_QAction():from( Qt_QDesignerFormWindowManagerInterface_actionRaise( ::pPtr ) )


METHOD QDesignerFormWindowManagerInterface:actionRedo()
   RETURN HB_QAction():from( Qt_QDesignerFormWindowManagerInterface_actionRedo( ::pPtr ) )


METHOD QDesignerFormWindowManagerInterface:actionSelectAll()
   RETURN HB_QAction():from( Qt_QDesignerFormWindowManagerInterface_actionSelectAll( ::pPtr ) )


METHOD QDesignerFormWindowManagerInterface:actionSimplifyLayout()
   RETURN HB_QAction():from( Qt_QDesignerFormWindowManagerInterface_actionSimplifyLayout( ::pPtr ) )


METHOD QDesignerFormWindowManagerInterface:actionSplitHorizontal()
   RETURN HB_QAction():from( Qt_QDesignerFormWindowManagerInterface_actionSplitHorizontal( ::pPtr ) )


METHOD QDesignerFormWindowManagerInterface:actionSplitVertical()
   RETURN HB_QAction():from( Qt_QDesignerFormWindowManagerInterface_actionSplitVertical( ::pPtr ) )


METHOD QDesignerFormWindowManagerInterface:actionUndo()
   RETURN HB_QAction():from( Qt_QDesignerFormWindowManagerInterface_actionUndo( ::pPtr ) )


METHOD QDesignerFormWindowManagerInterface:actionVerticalLayout()
   RETURN HB_QAction():from( Qt_QDesignerFormWindowManagerInterface_actionVerticalLayout( ::pPtr ) )


METHOD QDesignerFormWindowManagerInterface:activeFormWindow()
   RETURN HB_QDesignerFormWindowInterface():from( Qt_QDesignerFormWindowManagerInterface_activeFormWindow( ::pPtr ) )


METHOD QDesignerFormWindowManagerInterface:core()
   RETURN HB_QDesignerFormEditorInterface():from( Qt_QDesignerFormWindowManagerInterface_core( ::pPtr ) )


METHOD QDesignerFormWindowManagerInterface:createFormWindow( pParent, nFlags )
   RETURN HB_QDesignerFormWindowInterface():from( Qt_QDesignerFormWindowManagerInterface_createFormWindow( ::pPtr, hbqt_ptr( pParent ), nFlags ) )


METHOD QDesignerFormWindowManagerInterface:formWindow( nIndex )
   RETURN HB_QDesignerFormWindowInterface():from( Qt_QDesignerFormWindowManagerInterface_formWindow( ::pPtr, nIndex ) )


METHOD QDesignerFormWindowManagerInterface:formWindowCount()
   RETURN Qt_QDesignerFormWindowManagerInterface_formWindowCount( ::pPtr )


METHOD QDesignerFormWindowManagerInterface:addFormWindow( pFormWindow )
   RETURN Qt_QDesignerFormWindowManagerInterface_addFormWindow( ::pPtr, hbqt_ptr( pFormWindow ) )


METHOD QDesignerFormWindowManagerInterface:removeFormWindow( pFormWindow )
   RETURN Qt_QDesignerFormWindowManagerInterface_removeFormWindow( ::pPtr, hbqt_ptr( pFormWindow ) )


METHOD QDesignerFormWindowManagerInterface:setActiveFormWindow( pFormWindow )
   RETURN Qt_QDesignerFormWindowManagerInterface_setActiveFormWindow( ::pPtr, hbqt_ptr( pFormWindow ) )

