/*
 * $Id$
 */

/*
 * Harbour Project source code:
 *
 * Copyright 2009-2010 Pritpal Bedi <pritpal@vouchcac.com>
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
/*----------------------------------------------------------------------*/
/*----------------------------------------------------------------------*/
/*
 *                                EkOnkar
 *                          ( The LORD is ONE )
 *
 *                            Harbour-Qt IDE
 *
 *                  Pritpal Bedi <pritpal@vouchcac.com>
 *                               03Jan2010
 */
/*----------------------------------------------------------------------*/
/*----------------------------------------------------------------------*/
/*----------------------------------------------------------------------*/

#include "hbide.ch"
#include "common.ch"
#include "hbclass.ch"

/*----------------------------------------------------------------------*/

CLASS IdeObject

   ACCESS pSlots                                  INLINE hbxbp_getSlotsPtr()
   ACCESS pEvents                                 INLINE hbxbp_getEventsPtr()

   DATA   xD
   DATA   xD1
   DATA   xD2

   DATA   oIde
   DATA   oUI
   DATA   qContextMenu

   ACCESS oFR                                     INLINE ::oIde:oFR
   ACCESS oEM                                     INLINE ::oIde:oEM
   ACCESS oPM                                     INLINE ::oIde:oPM
   ACCESS oDK                                     INLINE ::oIde:oDK
   ACCESS oAC                                     INLINE ::oIde:oAC
   ACCESS oSM                                     INLINE ::oIde:oSM

   ACCESS aMeta                                   INLINE ::oIde:aMeta

   ACCESS oFont                                   INLINE ::oIde:oFont
   ACCESS oThemes                                 INLINE ::oIde:oThemes
   ACCESS oSBar                                   INLINE ::oIde:oSBar
   ACCESS oDlg                                    INLINE ::oIde:oDlg
   ACCESS qDlg                                    INLINE ::oIde:oDlg:oWidget
   ACCESS oDA                                     INLINE ::oIde:oDA

   ACCESS qCurEdit                                INLINE ::oIde:qCurEdit
   ACCESS qCurDocument                            INLINE ::oIde:qCurDocument
   ACCESS oCurEditor                              INLINE ::oIde:oCurEditor
   ACCESS qTabWidget                              INLINE ::oIde:oDA:oTabWidget:oWidget
   ACCESS qBrushWrkProject                        INLINE ::oIde:qBrushWrkProject

   ACCESS cWrkProject                             INLINE ::oIde:cWrkProject
   ACCESS cWrkTheme                               INLINE ::oIde:cWrkTheme
   ACCESS cWrkCodec                               INLINE ::oIde:cWrkCodec
   ACCESS resPath                                 INLINE ::oIde:resPath
   ACCESS pathSep                                 INLINE ::oIde:pathSep

   ACCESS aProjects                               INLINE ::oIde:aProjects
   ACCESS aINI                                    INLINE ::oIde:aINI
   ACCESS aSources                                INLINE ::oIde:aSources
   ACCESS aEditorPath                             INLINE ::oIde:aEditorPath
   ACCESS aProjData                               INLINE ::oIde:aProjData
   ACCESS aTabs                                   INLINE ::oIde:aTabs

   ACCESS oDockPT                                 INLINE ::oIde:oDockPT
   ACCESS oProjTree                               INLINE ::oIde:oProjTree
   ACCESS oProjRoot                               INLINE ::oIde:oProjRoot
   ACCESS oDockED                                 INLINE ::oIde:oDockED
   ACCESS oEditTree                               INLINE ::oIde:oEditTree
   ACCESS oOpenedSources                          INLINE ::oIde:oOpenedSources
   ACCESS oDockR                                  INLINE ::oIde:oDockR
   ACCESS oFuncList                               INLINE ::oIde:oFuncList
   ACCESS oDockB                                  INLINE ::oIde:oDockB
   ACCESS oCompileResult                          INLINE ::oIde:oCompileResult
   ACCESS oDockB1                                 INLINE ::oIde:oDockB1
   ACCESS oLinkResult                             INLINE ::oIde:oLinkResult
   ACCESS oDockB2                                 INLINE ::oIde:oDockB2
   ACCESS oOutputResult                           INLINE ::oIde:oOutputResult

   ACCESS lProjTreeVisible                        INLINE ::oIde:lProjTreeVisible
   ACCESS lDockRVisible                           INLINE ::oIde:lDockRVisible
   ACCESS lDockBVisible                           INLINE ::oIde:lDockBVisible
   ACCESS lTabCloseRequested                      INLINE ::oIde:lTabCloseRequested

   DATA   aSlots                                  INIT   {}
   DATA   aEvents                                 INIT   {}
   METHOD connect( qWidget, cSlot, bBlock )
   METHOD disConnect( qWidget, cSlot )

   METHOD createTags( ... )                       INLINE ::oIde:createTags( ... )
   METHOD addSourceInTree( ... )                  INLINE ::oIde:addSourceInTree( ... )
   METHOD setPosAndSizeByIni( ... )               INLINE ::oIde:setPosAndSizeByIni( ... )
   METHOD setPosByIni( ... )                      INLINE ::oIde:setPosByIni( ... )
   METHOD setSizeByIni( ... )                     INLINE ::oIde:setSizeByIni( ... )
   METHOD execAction( ... )                       INLINE ::oIde:execAction( ... )
   METHOD manageFuncContext( ... )                INLINE ::oIde:manageFuncContext( ... )
   METHOD manageProjectContext( ... )             INLINE ::oIde:manageProjectContext( ... )
   METHOD updateFuncList( ... )                   INLINE ::oIde:updateFuncList( ... )
   METHOD gotoFunction( ... )                     INLINE ::oIde:gotoFunction( ... )
   METHOD updateProjectMenu( ... )                INLINE ::oIde:updateProjectMenu( ... )
   METHOD updateProjectTree( ... )                INLINE ::oIde:updateProjectTree( ... )
   METHOD manageItemSelected( ... )               INLINE ::oIde:manageItemSelected( ... )
   METHOD manageFocusInEditor( ... )              INLINE ::oIde:manageFocusInEditor( ... )
   METHOD setCodec( ... )                         INLINE ::oIde:setCodec( ... )
   METHOD updateTitleBar( ... )                   INLINE ::oIde:updateTitleBar( ... )

   METHOD editSource( ... )                       INLINE ::oSM:editSource( ... )
   METHOD getEditorByIndex( ... )                 INLINE ::oSM:getEditorByIndex( ... )

   ENDCLASS

/*----------------------------------------------------------------------*/

METHOD IdeObject:connect( qWidget, cSlot, bBlock )

   IF !( Qt_Slots_Connect( ::pSlots, qWidget, cSlot, bBlock ) )
      hbide_dbg( "Connection FAILED:", cSlot )
   ELSE
      hbide_dbg( "Connection SUCCEEDED:", cSlot )
   ENDIF

   RETURN Self

/*----------------------------------------------------------------------*/

METHOD IdeObject:disConnect( qWidget, cSlot )

   IF !( Qt_Slots_disConnect( ::pSlots, qWidget, cSlot ) )
      hbide_dbg( "Dis-Connection FAILED:", cSlot )
   ELSE
      hbide_dbg( "Dis-Connection SUCCEEDED:", cSlot )
   ENDIF

   RETURN Self

/*----------------------------------------------------------------------*/
