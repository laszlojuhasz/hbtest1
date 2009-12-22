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


CREATE CLASS QTableWidgetItem

   VAR     pPtr

   ERROR HANDLER onError()

   METHOD  new()
   METHOD  configure( xObject )

   METHOD  background()
   METHOD  checkState()
   METHOD  clone()
   METHOD  column()
   METHOD  data( nRole )
   METHOD  flags()
   METHOD  font()
   METHOD  foreground()
   METHOD  icon()
   METHOD  isSelected()
   METHOD  read( pIn )
   METHOD  row()
   METHOD  setBackground( pBrush )
   METHOD  setCheckState( nState )
   METHOD  setData( nRole, pValue )
   METHOD  setFlags( nFlags )
   METHOD  setFont( pFont )
   METHOD  setForeground( pBrush )
   METHOD  setIcon( cIcon )
   METHOD  setSelected( lSelect )
   METHOD  setSizeHint( pSize )
   METHOD  setStatusTip( cStatusTip )
   METHOD  setText( cText )
   METHOD  setTextAlignment( nAlignment )
   METHOD  setToolTip( cToolTip )
   METHOD  setWhatsThis( cWhatsThis )
   METHOD  sizeHint()
   METHOD  statusTip()
   METHOD  tableWidget()
   METHOD  text()
   METHOD  textAlignment()
   METHOD  toolTip()
   METHOD  type()
   METHOD  whatsThis()
   METHOD  write( pOut )

   ENDCLASS


METHOD QTableWidgetItem:new( ... )
   LOCAL p
   FOR EACH p IN { ... }
      p := hbqt_ptr( p )
      hb_pvalue( p:__enumIndex(), p )
   NEXT
   ::pPtr := Qt_QTableWidgetItem( ... )
   RETURN Self


METHOD QTableWidgetItem:configure( xObject )
   IF hb_isObject( xObject )
      ::pPtr := xObject:pPtr
   ELSEIF hb_isPointer( xObject )
      ::pPtr := xObject
   ENDIF
   RETURN Self


METHOD QTableWidgetItem:onError()
   RETURN hbqt_showError( __GetMessage() )


METHOD QTableWidgetItem:background()
   RETURN Qt_QTableWidgetItem_background( ::pPtr )


METHOD QTableWidgetItem:checkState()
   RETURN Qt_QTableWidgetItem_checkState( ::pPtr )


METHOD QTableWidgetItem:clone()
   RETURN Qt_QTableWidgetItem_clone( ::pPtr )


METHOD QTableWidgetItem:column()
   RETURN Qt_QTableWidgetItem_column( ::pPtr )


METHOD QTableWidgetItem:data( nRole )
   RETURN Qt_QTableWidgetItem_data( ::pPtr, nRole )


METHOD QTableWidgetItem:flags()
   RETURN Qt_QTableWidgetItem_flags( ::pPtr )


METHOD QTableWidgetItem:font()
   RETURN Qt_QTableWidgetItem_font( ::pPtr )


METHOD QTableWidgetItem:foreground()
   RETURN Qt_QTableWidgetItem_foreground( ::pPtr )


METHOD QTableWidgetItem:icon()
   RETURN Qt_QTableWidgetItem_icon( ::pPtr )


METHOD QTableWidgetItem:isSelected()
   RETURN Qt_QTableWidgetItem_isSelected( ::pPtr )


METHOD QTableWidgetItem:read( pIn )
   RETURN Qt_QTableWidgetItem_read( ::pPtr, hbqt_ptr( pIn ) )


METHOD QTableWidgetItem:row()
   RETURN Qt_QTableWidgetItem_row( ::pPtr )


METHOD QTableWidgetItem:setBackground( pBrush )
   RETURN Qt_QTableWidgetItem_setBackground( ::pPtr, hbqt_ptr( pBrush ) )


METHOD QTableWidgetItem:setCheckState( nState )
   RETURN Qt_QTableWidgetItem_setCheckState( ::pPtr, nState )


METHOD QTableWidgetItem:setData( nRole, pValue )
   RETURN Qt_QTableWidgetItem_setData( ::pPtr, nRole, hbqt_ptr( pValue ) )


METHOD QTableWidgetItem:setFlags( nFlags )
   RETURN Qt_QTableWidgetItem_setFlags( ::pPtr, nFlags )


METHOD QTableWidgetItem:setFont( pFont )
   RETURN Qt_QTableWidgetItem_setFont( ::pPtr, hbqt_ptr( pFont ) )


METHOD QTableWidgetItem:setForeground( pBrush )
   RETURN Qt_QTableWidgetItem_setForeground( ::pPtr, hbqt_ptr( pBrush ) )


METHOD QTableWidgetItem:setIcon( cIcon )
   RETURN Qt_QTableWidgetItem_setIcon( ::pPtr, cIcon )


METHOD QTableWidgetItem:setSelected( lSelect )
   RETURN Qt_QTableWidgetItem_setSelected( ::pPtr, lSelect )


METHOD QTableWidgetItem:setSizeHint( pSize )
   RETURN Qt_QTableWidgetItem_setSizeHint( ::pPtr, hbqt_ptr( pSize ) )


METHOD QTableWidgetItem:setStatusTip( cStatusTip )
   RETURN Qt_QTableWidgetItem_setStatusTip( ::pPtr, cStatusTip )


METHOD QTableWidgetItem:setText( cText )
   RETURN Qt_QTableWidgetItem_setText( ::pPtr, cText )


METHOD QTableWidgetItem:setTextAlignment( nAlignment )
   RETURN Qt_QTableWidgetItem_setTextAlignment( ::pPtr, nAlignment )


METHOD QTableWidgetItem:setToolTip( cToolTip )
   RETURN Qt_QTableWidgetItem_setToolTip( ::pPtr, cToolTip )


METHOD QTableWidgetItem:setWhatsThis( cWhatsThis )
   RETURN Qt_QTableWidgetItem_setWhatsThis( ::pPtr, cWhatsThis )


METHOD QTableWidgetItem:sizeHint()
   RETURN Qt_QTableWidgetItem_sizeHint( ::pPtr )


METHOD QTableWidgetItem:statusTip()
   RETURN Qt_QTableWidgetItem_statusTip( ::pPtr )


METHOD QTableWidgetItem:tableWidget()
   RETURN Qt_QTableWidgetItem_tableWidget( ::pPtr )


METHOD QTableWidgetItem:text()
   RETURN Qt_QTableWidgetItem_text( ::pPtr )


METHOD QTableWidgetItem:textAlignment()
   RETURN Qt_QTableWidgetItem_textAlignment( ::pPtr )


METHOD QTableWidgetItem:toolTip()
   RETURN Qt_QTableWidgetItem_toolTip( ::pPtr )


METHOD QTableWidgetItem:type()
   RETURN Qt_QTableWidgetItem_type( ::pPtr )


METHOD QTableWidgetItem:whatsThis()
   RETURN Qt_QTableWidgetItem_whatsThis( ::pPtr )


METHOD QTableWidgetItem:write( pOut )
   RETURN Qt_QTableWidgetItem_write( ::pPtr, hbqt_ptr( pOut ) )

