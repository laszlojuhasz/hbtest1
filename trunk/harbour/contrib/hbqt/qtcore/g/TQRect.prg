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


#include "hbclass.ch"


FUNCTION QRect( ... )
   RETURN HB_QRect():new( ... )


CREATE CLASS QRect INHERIT HbQtObjectHandler FUNCTION HB_QRect

   METHOD  new( ... )

   METHOD  adjust( nDx1, nDy1, nDx2, nDy2 )
   METHOD  adjusted( nDx1, nDy1, nDx2, nDy2 )
   METHOD  bottom()
   METHOD  bottomLeft()
   METHOD  bottomRight()
   METHOD  center()
   METHOD  contains( ... )
   METHOD  getCoords( nX1, nY1, nX2, nY2 )
   METHOD  getRect( nX, nY, nWidth, nHeight )
   METHOD  height()
   METHOD  intersected( pRectangle )
   METHOD  intersects( pRectangle )
   METHOD  isEmpty()
   METHOD  isNull()
   METHOD  isValid()
   METHOD  left()
   METHOD  moveBottom( nY )
   METHOD  moveBottomLeft( pPosition )
   METHOD  moveBottomRight( pPosition )
   METHOD  moveCenter( pPosition )
   METHOD  moveLeft( nX )
   METHOD  moveRight( nX )
   METHOD  moveTo( ... )
   METHOD  moveTop( nY )
   METHOD  moveTopLeft( pPosition )
   METHOD  moveTopRight( pPosition )
   METHOD  normalized()
   METHOD  right()
   METHOD  setBottom( nY )
   METHOD  setBottomLeft( pPosition )
   METHOD  setBottomRight( pPosition )
   METHOD  setCoords( nX1, nY1, nX2, nY2 )
   METHOD  setHeight( nHeight )
   METHOD  setLeft( nX )
   METHOD  setRect( nX, nY, nWidth, nHeight )
   METHOD  setRight( nX )
   METHOD  setSize( pSize )
   METHOD  setTop( nY )
   METHOD  setTopLeft( pPosition )
   METHOD  setTopRight( pPosition )
   METHOD  setWidth( nWidth )
   METHOD  setX( nX )
   METHOD  setY( nY )
   METHOD  size()
   METHOD  top()
   METHOD  topLeft()
   METHOD  topRight()
   METHOD  translate( ... )
   METHOD  translated( ... )
   METHOD  united( pRectangle )
   METHOD  width()
   METHOD  x()
   METHOD  y()

   ENDCLASS


METHOD QRect:new( ... )
   LOCAL p
   FOR EACH p IN { ... }
      hb_pvalue( p:__enumIndex(), hbqt_ptr( p ) )
   NEXT
   ::pPtr := Qt_QRect( ... )
   RETURN Self


METHOD QRect:adjust( nDx1, nDy1, nDx2, nDy2 )
   RETURN Qt_QRect_adjust( ::pPtr, nDx1, nDy1, nDx2, nDy2 )


METHOD QRect:adjusted( nDx1, nDy1, nDx2, nDy2 )
   RETURN Qt_QRect_adjusted( ::pPtr, nDx1, nDy1, nDx2, nDy2 )


METHOD QRect:bottom()
   RETURN Qt_QRect_bottom( ::pPtr )


METHOD QRect:bottomLeft()
   RETURN Qt_QRect_bottomLeft( ::pPtr )


METHOD QRect:bottomRight()
   RETURN Qt_QRect_bottomRight( ::pPtr )


METHOD QRect:center()
   RETURN Qt_QRect_center( ::pPtr )


METHOD QRect:contains( ... )
   LOCAL p, aP, nP, aV := {}
   aP := hb_aParams()
   nP := len( aP )
   ::valtypes( aP, aV )
   FOR EACH p IN { ... }
      hb_pvalue( p:__enumIndex(), hbqt_ptr( p ) )
   NEXT
   DO CASE
   CASE nP == 3
      DO CASE
      CASE aV[ 1 ] $ "N" .AND. aV[ 2 ] $ "N" .AND. aV[ 3 ] $ "L"
                // bool contains ( int x, int y, bool proper ) const
                // N n int, N n int, L l bool
         RETURN Qt_QRect_contains_1( ::pPtr, ... )
      ENDCASE
   CASE nP == 2
      DO CASE
      CASE aV[ 1 ] $ "N" .AND. aV[ 2 ] $ "N"
                // bool contains ( int x, int y ) const
                // N n int, N n int
         RETURN Qt_QRect_contains_2( ::pPtr, ... )
      CASE aV[ 1 ] $ "PO" .AND. aV[ 2 ] $ "L"
                // bool contains ( const QRect & rectangle, bool proper = false ) const
                // PO p QRect, L l bool
         RETURN Qt_QRect_contains_3( ::pPtr, ... )
                // bool contains ( const QPoint & point, bool proper = false ) const
                // PO p QPoint, L l bool
         // RETURN Qt_QRect_contains( ::pPtr, ... )
      ENDCASE
   CASE nP == 1
      DO CASE
      CASE aV[ 1 ] $ "PO"
                // bool contains ( const QRect & rectangle, bool proper = false ) const
                // PO p QRect, L l bool
         RETURN Qt_QRect_contains_3( ::pPtr, ... )
                // bool contains ( const QPoint & point, bool proper = false ) const
                // PO p QPoint, L l bool
         // RETURN Qt_QRect_contains( ::pPtr, ... )
      ENDCASE
   ENDCASE
   RETURN NIL


METHOD QRect:getCoords( nX1, nY1, nX2, nY2 )
   RETURN Qt_QRect_getCoords( ::pPtr, nX1, nY1, nX2, nY2 )


METHOD QRect:getRect( nX, nY, nWidth, nHeight )
   RETURN Qt_QRect_getRect( ::pPtr, nX, nY, nWidth, nHeight )


METHOD QRect:height()
   RETURN Qt_QRect_height( ::pPtr )


METHOD QRect:intersected( pRectangle )
   RETURN Qt_QRect_intersected( ::pPtr, hbqt_ptr( pRectangle ) )


METHOD QRect:intersects( pRectangle )
   RETURN Qt_QRect_intersects( ::pPtr, hbqt_ptr( pRectangle ) )


METHOD QRect:isEmpty()
   RETURN Qt_QRect_isEmpty( ::pPtr )


METHOD QRect:isNull()
   RETURN Qt_QRect_isNull( ::pPtr )


METHOD QRect:isValid()
   RETURN Qt_QRect_isValid( ::pPtr )


METHOD QRect:left()
   RETURN Qt_QRect_left( ::pPtr )


METHOD QRect:moveBottom( nY )
   RETURN Qt_QRect_moveBottom( ::pPtr, nY )


METHOD QRect:moveBottomLeft( pPosition )
   RETURN Qt_QRect_moveBottomLeft( ::pPtr, hbqt_ptr( pPosition ) )


METHOD QRect:moveBottomRight( pPosition )
   RETURN Qt_QRect_moveBottomRight( ::pPtr, hbqt_ptr( pPosition ) )


METHOD QRect:moveCenter( pPosition )
   RETURN Qt_QRect_moveCenter( ::pPtr, hbqt_ptr( pPosition ) )


METHOD QRect:moveLeft( nX )
   RETURN Qt_QRect_moveLeft( ::pPtr, nX )


METHOD QRect:moveRight( nX )
   RETURN Qt_QRect_moveRight( ::pPtr, nX )


METHOD QRect:moveTo( ... )
   LOCAL p, aP, nP, aV := {}
   aP := hb_aParams()
   nP := len( aP )
   ::valtypes( aP, aV )
   FOR EACH p IN { ... }
      hb_pvalue( p:__enumIndex(), hbqt_ptr( p ) )
   NEXT
   DO CASE
   CASE nP == 2
      DO CASE
      CASE aV[ 1 ] $ "N" .AND. aV[ 2 ] $ "N"
                // void moveTo ( int x, int y )
                // N n int, N n int
         RETURN Qt_QRect_moveTo( ::pPtr, ... )
      ENDCASE
   CASE nP == 1
      DO CASE
      CASE aV[ 1 ] $ "PO"
                // void moveTo ( const QPoint & position )
                // PO p QPoint
         RETURN Qt_QRect_moveTo_1( ::pPtr, ... )
      ENDCASE
   ENDCASE
   RETURN NIL


METHOD QRect:moveTop( nY )
   RETURN Qt_QRect_moveTop( ::pPtr, nY )


METHOD QRect:moveTopLeft( pPosition )
   RETURN Qt_QRect_moveTopLeft( ::pPtr, hbqt_ptr( pPosition ) )


METHOD QRect:moveTopRight( pPosition )
   RETURN Qt_QRect_moveTopRight( ::pPtr, hbqt_ptr( pPosition ) )


METHOD QRect:normalized()
   RETURN Qt_QRect_normalized( ::pPtr )


METHOD QRect:right()
   RETURN Qt_QRect_right( ::pPtr )


METHOD QRect:setBottom( nY )
   RETURN Qt_QRect_setBottom( ::pPtr, nY )


METHOD QRect:setBottomLeft( pPosition )
   RETURN Qt_QRect_setBottomLeft( ::pPtr, hbqt_ptr( pPosition ) )


METHOD QRect:setBottomRight( pPosition )
   RETURN Qt_QRect_setBottomRight( ::pPtr, hbqt_ptr( pPosition ) )


METHOD QRect:setCoords( nX1, nY1, nX2, nY2 )
   RETURN Qt_QRect_setCoords( ::pPtr, nX1, nY1, nX2, nY2 )


METHOD QRect:setHeight( nHeight )
   RETURN Qt_QRect_setHeight( ::pPtr, nHeight )


METHOD QRect:setLeft( nX )
   RETURN Qt_QRect_setLeft( ::pPtr, nX )


METHOD QRect:setRect( nX, nY, nWidth, nHeight )
   RETURN Qt_QRect_setRect( ::pPtr, nX, nY, nWidth, nHeight )


METHOD QRect:setRight( nX )
   RETURN Qt_QRect_setRight( ::pPtr, nX )


METHOD QRect:setSize( pSize )
   RETURN Qt_QRect_setSize( ::pPtr, hbqt_ptr( pSize ) )


METHOD QRect:setTop( nY )
   RETURN Qt_QRect_setTop( ::pPtr, nY )


METHOD QRect:setTopLeft( pPosition )
   RETURN Qt_QRect_setTopLeft( ::pPtr, hbqt_ptr( pPosition ) )


METHOD QRect:setTopRight( pPosition )
   RETURN Qt_QRect_setTopRight( ::pPtr, hbqt_ptr( pPosition ) )


METHOD QRect:setWidth( nWidth )
   RETURN Qt_QRect_setWidth( ::pPtr, nWidth )


METHOD QRect:setX( nX )
   RETURN Qt_QRect_setX( ::pPtr, nX )


METHOD QRect:setY( nY )
   RETURN Qt_QRect_setY( ::pPtr, nY )


METHOD QRect:size()
   RETURN Qt_QRect_size( ::pPtr )


METHOD QRect:top()
   RETURN Qt_QRect_top( ::pPtr )


METHOD QRect:topLeft()
   RETURN Qt_QRect_topLeft( ::pPtr )


METHOD QRect:topRight()
   RETURN Qt_QRect_topRight( ::pPtr )


METHOD QRect:translate( ... )
   LOCAL p, aP, nP, aV := {}
   aP := hb_aParams()
   nP := len( aP )
   ::valtypes( aP, aV )
   FOR EACH p IN { ... }
      hb_pvalue( p:__enumIndex(), hbqt_ptr( p ) )
   NEXT
   DO CASE
   CASE nP == 2
      DO CASE
      CASE aV[ 1 ] $ "N" .AND. aV[ 2 ] $ "N"
                // void translate ( int dx, int dy )
                // N n int, N n int
         RETURN Qt_QRect_translate( ::pPtr, ... )
      ENDCASE
   CASE nP == 1
      DO CASE
      CASE aV[ 1 ] $ "PO"
                // void translate ( const QPoint & offset )
                // PO p QPoint
         RETURN Qt_QRect_translate_1( ::pPtr, ... )
      ENDCASE
   ENDCASE
   RETURN NIL


METHOD QRect:translated( ... )
   LOCAL p, aP, nP, aV := {}
   aP := hb_aParams()
   nP := len( aP )
   ::valtypes( aP, aV )
   FOR EACH p IN { ... }
      hb_pvalue( p:__enumIndex(), hbqt_ptr( p ) )
   NEXT
   DO CASE
   CASE nP == 2
      DO CASE
      CASE aV[ 1 ] $ "N" .AND. aV[ 2 ] $ "N"
                // QRect translated ( int dx, int dy ) const
                // N n int, N n int
         RETURN QRect():from( Qt_QRect_translated( ::pPtr, ... ) )
      ENDCASE
   CASE nP == 1
      DO CASE
      CASE aV[ 1 ] $ "PO"
                // QRect translated ( const QPoint & offset ) const
                // PO p QPoint
         RETURN QRect():from( Qt_QRect_translated_1( ::pPtr, ... ) )
      ENDCASE
   ENDCASE
   RETURN NIL


METHOD QRect:united( pRectangle )
   RETURN Qt_QRect_united( ::pPtr, hbqt_ptr( pRectangle ) )


METHOD QRect:width()
   RETURN Qt_QRect_width( ::pPtr )


METHOD QRect:x()
   RETURN Qt_QRect_x( ::pPtr )


METHOD QRect:y()
   RETURN Qt_QRect_y( ::pPtr )

