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


CREATE CLASS QTextLine

   VAR     pPtr

   METHOD  new()
   METHOD  configure( xObject )

   METHOD  ascent()
   METHOD  cursorToX( nCursorPos, nEdge )
   METHOD  cursorToX_1( nCursorPos, nEdge )
   METHOD  descent()
   METHOD  height()
   METHOD  isValid()
   METHOD  lineNumber()
   METHOD  naturalTextRect()
   METHOD  naturalTextWidth()
   METHOD  position()
   METHOD  rect()
   METHOD  setLineWidth( nWidth )
   METHOD  setNumColumns( nNumColumns )
   METHOD  setNumColumns_1( nNumColumns, nAlignmentWidth )
   METHOD  setPosition( pPos )
   METHOD  textLength()
   METHOD  textStart()
   METHOD  width()
   METHOD  x()
   METHOD  xToCursor( nX, nCpos )
   METHOD  y()

   ENDCLASS

/*----------------------------------------------------------------------*/

METHOD QTextLine:new( pParent )
   ::pPtr := Qt_QTextLine( hbqt_ptr( pParent ) )
   RETURN Self


METHOD QTextLine:configure( xObject )
   IF hb_isObject( xObject )
      ::pPtr := xObject:pPtr
   ELSEIF hb_isPointer( xObject )
      ::pPtr := xObject
   ENDIF
   RETURN Self


METHOD QTextLine:ascent()
   RETURN Qt_QTextLine_ascent( ::pPtr )


METHOD QTextLine:cursorToX( nCursorPos, nEdge )
   RETURN Qt_QTextLine_cursorToX( ::pPtr, nCursorPos, nEdge )


METHOD QTextLine:cursorToX_1( nCursorPos, nEdge )
   RETURN Qt_QTextLine_cursorToX_1( ::pPtr, nCursorPos, nEdge )


METHOD QTextLine:descent()
   RETURN Qt_QTextLine_descent( ::pPtr )


METHOD QTextLine:height()
   RETURN Qt_QTextLine_height( ::pPtr )


METHOD QTextLine:isValid()
   RETURN Qt_QTextLine_isValid( ::pPtr )


METHOD QTextLine:lineNumber()
   RETURN Qt_QTextLine_lineNumber( ::pPtr )


METHOD QTextLine:naturalTextRect()
   RETURN Qt_QTextLine_naturalTextRect( ::pPtr )


METHOD QTextLine:naturalTextWidth()
   RETURN Qt_QTextLine_naturalTextWidth( ::pPtr )


METHOD QTextLine:position()
   RETURN Qt_QTextLine_position( ::pPtr )


METHOD QTextLine:rect()
   RETURN Qt_QTextLine_rect( ::pPtr )


METHOD QTextLine:setLineWidth( nWidth )
   RETURN Qt_QTextLine_setLineWidth( ::pPtr, nWidth )


METHOD QTextLine:setNumColumns( nNumColumns )
   RETURN Qt_QTextLine_setNumColumns( ::pPtr, nNumColumns )


METHOD QTextLine:setNumColumns_1( nNumColumns, nAlignmentWidth )
   RETURN Qt_QTextLine_setNumColumns_1( ::pPtr, nNumColumns, nAlignmentWidth )


METHOD QTextLine:setPosition( pPos )
   RETURN Qt_QTextLine_setPosition( ::pPtr, hbqt_ptr( pPos ) )


METHOD QTextLine:textLength()
   RETURN Qt_QTextLine_textLength( ::pPtr )


METHOD QTextLine:textStart()
   RETURN Qt_QTextLine_textStart( ::pPtr )


METHOD QTextLine:width()
   RETURN Qt_QTextLine_width( ::pPtr )


METHOD QTextLine:x()
   RETURN Qt_QTextLine_x( ::pPtr )


METHOD QTextLine:xToCursor( nX, nCpos )
   RETURN Qt_QTextLine_xToCursor( ::pPtr, nX, nCpos )


METHOD QTextLine:y()
   RETURN Qt_QTextLine_y( ::pPtr )

