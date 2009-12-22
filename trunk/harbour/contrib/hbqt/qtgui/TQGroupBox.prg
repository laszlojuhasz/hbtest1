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


CREATE CLASS QGroupBox INHERIT QWidget

   VAR     pPtr

   ERROR HANDLER onError()

   METHOD  new()
   METHOD  configure( xObject )

   METHOD  alignment()
   METHOD  isCheckable()
   METHOD  isChecked()
   METHOD  isFlat()
   METHOD  setAlignment( nAlignment )
   METHOD  setCheckable( lCheckable )
   METHOD  setFlat( lFlat )
   METHOD  setTitle( cTitle )
   METHOD  title()
   METHOD  setChecked( lChecked )

   ENDCLASS


METHOD QGroupBox:new( ... )
   LOCAL p
   FOR EACH p IN { ... }
      p := hbqt_ptr( p )
      hb_pvalue( p:__enumIndex(), p )
   NEXT
   ::pPtr := Qt_QGroupBox( ... )
   RETURN Self


METHOD QGroupBox:configure( xObject )
   IF hb_isObject( xObject )
      ::pPtr := xObject:pPtr
   ELSEIF hb_isPointer( xObject )
      ::pPtr := xObject
   ENDIF
   RETURN Self


METHOD QGroupBox:onError()
   RETURN hbqt_showError( __GetMessage() )


METHOD QGroupBox:alignment()
   RETURN Qt_QGroupBox_alignment( ::pPtr )


METHOD QGroupBox:isCheckable()
   RETURN Qt_QGroupBox_isCheckable( ::pPtr )


METHOD QGroupBox:isChecked()
   RETURN Qt_QGroupBox_isChecked( ::pPtr )


METHOD QGroupBox:isFlat()
   RETURN Qt_QGroupBox_isFlat( ::pPtr )


METHOD QGroupBox:setAlignment( nAlignment )
   RETURN Qt_QGroupBox_setAlignment( ::pPtr, nAlignment )


METHOD QGroupBox:setCheckable( lCheckable )
   RETURN Qt_QGroupBox_setCheckable( ::pPtr, lCheckable )


METHOD QGroupBox:setFlat( lFlat )
   RETURN Qt_QGroupBox_setFlat( ::pPtr, lFlat )


METHOD QGroupBox:setTitle( cTitle )
   RETURN Qt_QGroupBox_setTitle( ::pPtr, cTitle )


METHOD QGroupBox:title()
   RETURN Qt_QGroupBox_title( ::pPtr )


METHOD QGroupBox:setChecked( lChecked )
   RETURN Qt_QGroupBox_setChecked( ::pPtr, lChecked )

