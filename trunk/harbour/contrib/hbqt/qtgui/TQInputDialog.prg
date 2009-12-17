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


CREATE CLASS QInputDialog INHERIT QDialog

   VAR     pPtr

   METHOD  new()
   METHOD  configure( xObject )

   METHOD  cancelButtonText()
   METHOD  comboBoxItems()
   METHOD  done( nResult )
   METHOD  doubleDecimals()
   METHOD  doubleMaximum()
   METHOD  doubleMinimum()
   METHOD  doubleValue()
   METHOD  inputMode()
   METHOD  intMaximum()
   METHOD  intMinimum()
   METHOD  intStep()
   METHOD  intValue()
   METHOD  isComboBoxEditable()
   METHOD  labelText()
   METHOD  okButtonText()
   METHOD  open( pReceiver, pMember )
   METHOD  options()
   METHOD  setCancelButtonText( cText )
   METHOD  setComboBoxEditable( lEditable )
   METHOD  setComboBoxItems( pItems )
   METHOD  setDoubleDecimals( nDecimals )
   METHOD  setDoubleMaximum( nMax )
   METHOD  setDoubleMinimum( nMin )
   METHOD  setDoubleRange( nMin, nMax )
   METHOD  setDoubleValue( nValue )
   METHOD  setInputMode( nMode )
   METHOD  setIntMaximum( nMax )
   METHOD  setIntMinimum( nMin )
   METHOD  setIntRange( nMin, nMax )
   METHOD  setIntStep( nStep )
   METHOD  setIntValue( nValue )
   METHOD  setLabelText( cText )
   METHOD  setOkButtonText( cText )
   METHOD  setOption( nOption, lOn )
   METHOD  setOptions( nOptions )
   METHOD  setTextEchoMode( nMode )
   METHOD  setTextValue( cText )
   METHOD  testOption( nOption )
   METHOD  textEchoMode()
   METHOD  textValue()
   METHOD  getDouble( pParent, cTitle, cLabel, nValue, nMin, nMax, nDecimals, lOk, nFlags )
   METHOD  getInt( pParent, cTitle, cLabel, nValue, nMin, nMax, nStep, lOk, nFlags )
   METHOD  getItem( pParent, cTitle, cLabel, pItems, nCurrent, lEditable, lOk, nFlags )
   METHOD  getText( pParent, cTitle, cLabel, nMode, cText, lOk, nFlags )

   ENDCLASS

/*----------------------------------------------------------------------*/

METHOD QInputDialog:new( pParent )
   ::pPtr := Qt_QInputDialog( hbqt_ptr( pParent ) )
   RETURN Self


METHOD QInputDialog:configure( xObject )
   IF hb_isObject( xObject )
      ::pPtr := xObject:pPtr
   ELSEIF hb_isPointer( xObject )
      ::pPtr := xObject
   ENDIF
   RETURN Self


METHOD QInputDialog:cancelButtonText()
   RETURN Qt_QInputDialog_cancelButtonText( ::pPtr )


METHOD QInputDialog:comboBoxItems()
   RETURN Qt_QInputDialog_comboBoxItems( ::pPtr )


METHOD QInputDialog:done( nResult )
   RETURN Qt_QInputDialog_done( ::pPtr, nResult )


METHOD QInputDialog:doubleDecimals()
   RETURN Qt_QInputDialog_doubleDecimals( ::pPtr )


METHOD QInputDialog:doubleMaximum()
   RETURN Qt_QInputDialog_doubleMaximum( ::pPtr )


METHOD QInputDialog:doubleMinimum()
   RETURN Qt_QInputDialog_doubleMinimum( ::pPtr )


METHOD QInputDialog:doubleValue()
   RETURN Qt_QInputDialog_doubleValue( ::pPtr )


METHOD QInputDialog:inputMode()
   RETURN Qt_QInputDialog_inputMode( ::pPtr )


METHOD QInputDialog:intMaximum()
   RETURN Qt_QInputDialog_intMaximum( ::pPtr )


METHOD QInputDialog:intMinimum()
   RETURN Qt_QInputDialog_intMinimum( ::pPtr )


METHOD QInputDialog:intStep()
   RETURN Qt_QInputDialog_intStep( ::pPtr )


METHOD QInputDialog:intValue()
   RETURN Qt_QInputDialog_intValue( ::pPtr )


METHOD QInputDialog:isComboBoxEditable()
   RETURN Qt_QInputDialog_isComboBoxEditable( ::pPtr )


METHOD QInputDialog:labelText()
   RETURN Qt_QInputDialog_labelText( ::pPtr )


METHOD QInputDialog:okButtonText()
   RETURN Qt_QInputDialog_okButtonText( ::pPtr )


METHOD QInputDialog:open( pReceiver, pMember )
   RETURN Qt_QInputDialog_open( ::pPtr, hbqt_ptr( pReceiver ), hbqt_ptr( pMember ) )


METHOD QInputDialog:options()
   RETURN Qt_QInputDialog_options( ::pPtr )


METHOD QInputDialog:setCancelButtonText( cText )
   RETURN Qt_QInputDialog_setCancelButtonText( ::pPtr, cText )


METHOD QInputDialog:setComboBoxEditable( lEditable )
   RETURN Qt_QInputDialog_setComboBoxEditable( ::pPtr, lEditable )


METHOD QInputDialog:setComboBoxItems( pItems )
   RETURN Qt_QInputDialog_setComboBoxItems( ::pPtr, hbqt_ptr( pItems ) )


METHOD QInputDialog:setDoubleDecimals( nDecimals )
   RETURN Qt_QInputDialog_setDoubleDecimals( ::pPtr, nDecimals )


METHOD QInputDialog:setDoubleMaximum( nMax )
   RETURN Qt_QInputDialog_setDoubleMaximum( ::pPtr, nMax )


METHOD QInputDialog:setDoubleMinimum( nMin )
   RETURN Qt_QInputDialog_setDoubleMinimum( ::pPtr, nMin )


METHOD QInputDialog:setDoubleRange( nMin, nMax )
   RETURN Qt_QInputDialog_setDoubleRange( ::pPtr, nMin, nMax )


METHOD QInputDialog:setDoubleValue( nValue )
   RETURN Qt_QInputDialog_setDoubleValue( ::pPtr, nValue )


METHOD QInputDialog:setInputMode( nMode )
   RETURN Qt_QInputDialog_setInputMode( ::pPtr, nMode )


METHOD QInputDialog:setIntMaximum( nMax )
   RETURN Qt_QInputDialog_setIntMaximum( ::pPtr, nMax )


METHOD QInputDialog:setIntMinimum( nMin )
   RETURN Qt_QInputDialog_setIntMinimum( ::pPtr, nMin )


METHOD QInputDialog:setIntRange( nMin, nMax )
   RETURN Qt_QInputDialog_setIntRange( ::pPtr, nMin, nMax )


METHOD QInputDialog:setIntStep( nStep )
   RETURN Qt_QInputDialog_setIntStep( ::pPtr, nStep )


METHOD QInputDialog:setIntValue( nValue )
   RETURN Qt_QInputDialog_setIntValue( ::pPtr, nValue )


METHOD QInputDialog:setLabelText( cText )
   RETURN Qt_QInputDialog_setLabelText( ::pPtr, cText )


METHOD QInputDialog:setOkButtonText( cText )
   RETURN Qt_QInputDialog_setOkButtonText( ::pPtr, cText )


METHOD QInputDialog:setOption( nOption, lOn )
   RETURN Qt_QInputDialog_setOption( ::pPtr, nOption, lOn )


METHOD QInputDialog:setOptions( nOptions )
   RETURN Qt_QInputDialog_setOptions( ::pPtr, nOptions )


METHOD QInputDialog:setTextEchoMode( nMode )
   RETURN Qt_QInputDialog_setTextEchoMode( ::pPtr, nMode )


METHOD QInputDialog:setTextValue( cText )
   RETURN Qt_QInputDialog_setTextValue( ::pPtr, cText )


METHOD QInputDialog:testOption( nOption )
   RETURN Qt_QInputDialog_testOption( ::pPtr, nOption )


METHOD QInputDialog:textEchoMode()
   RETURN Qt_QInputDialog_textEchoMode( ::pPtr )


METHOD QInputDialog:textValue()
   RETURN Qt_QInputDialog_textValue( ::pPtr )


METHOD QInputDialog:getDouble( pParent, cTitle, cLabel, nValue, nMin, nMax, nDecimals, lOk, nFlags )
   RETURN Qt_QInputDialog_getDouble( ::pPtr, hbqt_ptr( pParent ), cTitle, cLabel, nValue, nMin, nMax, nDecimals, lOk, nFlags )


METHOD QInputDialog:getInt( pParent, cTitle, cLabel, nValue, nMin, nMax, nStep, lOk, nFlags )
   RETURN Qt_QInputDialog_getInt( ::pPtr, hbqt_ptr( pParent ), cTitle, cLabel, nValue, nMin, nMax, nStep, lOk, nFlags )


METHOD QInputDialog:getItem( pParent, cTitle, cLabel, pItems, nCurrent, lEditable, lOk, nFlags )
   RETURN Qt_QInputDialog_getItem( ::pPtr, hbqt_ptr( pParent ), cTitle, cLabel, hbqt_ptr( pItems ), nCurrent, lEditable, lOk, nFlags )


METHOD QInputDialog:getText( pParent, cTitle, cLabel, nMode, cText, lOk, nFlags )
   RETURN Qt_QInputDialog_getText( ::pPtr, hbqt_ptr( pParent ), cTitle, cLabel, nMode, cText, lOk, nFlags )

