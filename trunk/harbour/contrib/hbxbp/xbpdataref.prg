/*
 * $Id$
 */

/*
 * Harbour Project source code:
 * Source file for the Xbp*Classes
 *
 * Copyright 2009 Pritpal Bedi <pritpal@vouchcac.com>
 * http://www.harbour-project.org
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
 *                    Xbase++ dataRef Compatible Class
 *
 *                  Pritpal Bedi <pritpal@vouchcac.com>
 *                               14Jun2009
 */
/*----------------------------------------------------------------------*/
/*----------------------------------------------------------------------*/
/*----------------------------------------------------------------------*/

#include "hbclass.ch"
#include "common.ch"

#include "xbp.ch"
#include "appevent.ch"
#include "apig.ch"
#include "hbqt.ch"

/*----------------------------------------------------------------------*/

CLASS XbpDataRef

   DATA     changed                               INIT .F.
   DATA     dataLink                              INIT NIL
   DATA     lastValid                             INIT .T.
   DATA     sl_undo                               INIT NIL
   DATA     undoBuffer                            INIT NIL
   DATA     sl_validate                           INIT NIL

   METHOD   new()

   DATA     sl_editBuffer
   DATA     sl_buffer

   ACCESS   editBuffer                             INLINE ::sl_editBuffer
   ASSIGN   editBuffer( xData )                    INLINE ::sl_editBuffer := xData

   METHOD   getData()
   METHOD   setData()
   METHOD   undo()

   METHOD   validate( xParam )                     SETGET

   ENDCLASS

/*----------------------------------------------------------------------*/

METHOD XbpDataRef:new()

   RETURN self

/*----------------------------------------------------------------------*/

METHOD XbpDataRef:getData()
   LOCAL cClass := __ObjGetClsName( self )

   DO CASE
   CASE cClass $ "XBPSLE,XBPMLE" //::className == "EDIT"
      //::sl_editBuffer := Win_GetMessageText( ::hWnd, WM_GETTEXT, ::bufferLength + 1 )

   CASE cClass == "XBPRADIOBUTTON"
      ::sl_editBuffer := ::oWidget:isChecked()

//   CASE cClass == "XBPLISTBOX"
//      RETURN ::nCurSelected

   ENDCASE

   IF hb_isBlock( ::dataLink )
      eval( ::dataLink, ::sl_editBuffer )
   ENDIF

   RETURN ::sl_editBuffer

/*----------------------------------------------------------------------*/

METHOD XbpDataRef:setData( xValue, mp2 )
   LOCAL cClass := __ObjGetClsName( self )

   HB_SYMBOL_UNUSED( mp2 )

   IF hb_isBlock( ::dataLink )
      ::sl_editBuffer := eval( ::dataLink  )

   ELSEIF xValue <> NIL
      ::sl_editBuffer := xValue

   ENDIF

   DO CASE
   CASE cClass $ "XBPCHECKBOX,XBPRADIO"
      ::oWidget:setChecked( ::sl_editBuffer )

   CASE cClass == "XBP3STATE"
      ::oWidget:setCheckState( IF( ::sl_editBuffer == 1, 2, IF( ::sl_editBuffer == 2, 1, 0 ) ) )

   CASE cClass == "XBPLISTBOX"   //::className == "LISTBOX"    /* Single Selection */
      IF !empty( ::sl_editBuffer )
         //RETURN Win_LbSetCurSel( ::hWnd, ::sl_editBuffer - 1 ) >= 0
      ENDIF
      RETURN .f.

   CASE cClass == "XBPTREEVIEW"  //::className == "SysTreeView32"
      IF ::sl_editBuffer <> NIL .and. ::sl_editBuffer:hItem <> NIL
         //Win_TreeView_SelectItem( ::hWnd, ::sl_editBuffer:hItem )
      ENDIF

   CASE cClass $ "XBPSLE,XBPMLE" //::className == "EDIT"
      IF hb_isChar( ::sl_editBuffer )
         //Win_SendMessageText( ::hWnd, WM_SETTEXT, 0, ::sl_editBuffer )
      ENDIF

   CASE ::className == "XBPSCROLLBAR"  //"SCROLLBAR"
      IF ::sl_editBuffer <> NIL
         //WAPI_SetScrollPos( ::pWnd, SB_CTL, ::sl_editBuffer, .t. )
      ENDIF
   ENDCASE

   RETURN ::sl_editBuffer

/*----------------------------------------------------------------------*/

METHOD XbpDataRef:undo()

   RETURN .f.

/*----------------------------------------------------------------------*/

METHOD XbpDataRef:validate( xParam )

   IF PCount() == 0 .and. hb_isBlock( ::sl_validate )
      RETURN eval( ::sl_validate, self )
   ELSEIF hb_isBlock( xParam )
      ::sl_validate := xParam
   ENDIF

   RETURN .t.

/*----------------------------------------------------------------------*/
