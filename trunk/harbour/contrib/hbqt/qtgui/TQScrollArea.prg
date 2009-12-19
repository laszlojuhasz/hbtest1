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


CREATE CLASS QScrollArea INHERIT QAbstractScrollArea

   VAR     pPtr

   METHOD  new()
   METHOD  configure( xObject )

   METHOD  alignment()
   METHOD  ensureVisible( nX, nY, nXmargin, nYmargin )
   METHOD  ensureWidgetVisible( pChildWidget, nXmargin, nYmargin )
   METHOD  setAlignment( nQt_Alignment )
   METHOD  setWidget( pWidget )
   METHOD  setWidgetResizable( lResizable )
   METHOD  takeWidget()
   METHOD  widget()
   METHOD  widgetResizable()

   ENDCLASS

/*----------------------------------------------------------------------*/

METHOD QScrollArea:new( ... )
   LOCAL p
   FOR EACH p IN { ... }
      p := hbqt_ptr( p )
      hb_pvalue( p:__enumIndex(), p )
   NEXT
   ::pPtr := Qt_QScrollArea( ... )
   RETURN Self


METHOD QScrollArea:configure( xObject )
   IF hb_isObject( xObject )
      ::pPtr := xObject:pPtr
   ELSEIF hb_isPointer( xObject )
      ::pPtr := xObject
   ENDIF
   RETURN Self


METHOD QScrollArea:alignment()
   RETURN Qt_QScrollArea_alignment( ::pPtr )


METHOD QScrollArea:ensureVisible( nX, nY, nXmargin, nYmargin )
   RETURN Qt_QScrollArea_ensureVisible( ::pPtr, nX, nY, nXmargin, nYmargin )


METHOD QScrollArea:ensureWidgetVisible( pChildWidget, nXmargin, nYmargin )
   RETURN Qt_QScrollArea_ensureWidgetVisible( ::pPtr, hbqt_ptr( pChildWidget ), nXmargin, nYmargin )


METHOD QScrollArea:setAlignment( nQt_Alignment )
   RETURN Qt_QScrollArea_setAlignment( ::pPtr, nQt_Alignment )


METHOD QScrollArea:setWidget( pWidget )
   RETURN Qt_QScrollArea_setWidget( ::pPtr, hbqt_ptr( pWidget ) )


METHOD QScrollArea:setWidgetResizable( lResizable )
   RETURN Qt_QScrollArea_setWidgetResizable( ::pPtr, lResizable )


METHOD QScrollArea:takeWidget()
   RETURN Qt_QScrollArea_takeWidget( ::pPtr )


METHOD QScrollArea:widget()
   RETURN Qt_QScrollArea_widget( ::pPtr )


METHOD QScrollArea:widgetResizable()
   RETURN Qt_QScrollArea_widgetResizable( ::pPtr )

