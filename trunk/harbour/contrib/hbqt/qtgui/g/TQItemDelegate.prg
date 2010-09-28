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


FUNCTION QItemDelegate( ... )
   RETURN HB_QItemDelegate():new( ... )


CREATE CLASS QItemDelegate INHERIT HbQtObjectHandler, HB_QAbstractItemDelegate FUNCTION HB_QItemDelegate

   METHOD  new( ... )

   METHOD  hasClipping()
   METHOD  itemEditorFactory()
   METHOD  setClipping( lClip )
   METHOD  setItemEditorFactory( pFactory )
   METHOD  createEditor( pParent, pOption, pIndex )
   METHOD  paint( pPainter, pOption, pIndex )
   METHOD  setEditorData( pEditor, pIndex )
   METHOD  setModelData( pEditor, pModel, pIndex )
   METHOD  sizeHint( pOption, pIndex )
   METHOD  updateEditorGeometry( pEditor, pOption, pIndex )

   ENDCLASS


METHOD QItemDelegate:new( ... )
   LOCAL p
   FOR EACH p IN { ... }
      hb_pvalue( p:__enumIndex(), hbqt_ptr( p ) )
   NEXT
   ::pPtr := Qt_QItemDelegate( ... )
   RETURN Self


METHOD QItemDelegate:hasClipping()
   RETURN Qt_QItemDelegate_hasClipping( ::pPtr )


METHOD QItemDelegate:itemEditorFactory()
   RETURN HB_QItemEditorFactory():from( Qt_QItemDelegate_itemEditorFactory( ::pPtr ) )


METHOD QItemDelegate:setClipping( lClip )
   RETURN Qt_QItemDelegate_setClipping( ::pPtr, lClip )


METHOD QItemDelegate:setItemEditorFactory( pFactory )
   RETURN Qt_QItemDelegate_setItemEditorFactory( ::pPtr, hbqt_ptr( pFactory ) )


METHOD QItemDelegate:createEditor( pParent, pOption, pIndex )
   RETURN HB_QWidget():from( Qt_QItemDelegate_createEditor( ::pPtr, hbqt_ptr( pParent ), hbqt_ptr( pOption ), hbqt_ptr( pIndex ) ) )


METHOD QItemDelegate:paint( pPainter, pOption, pIndex )
   RETURN Qt_QItemDelegate_paint( ::pPtr, hbqt_ptr( pPainter ), hbqt_ptr( pOption ), hbqt_ptr( pIndex ) )


METHOD QItemDelegate:setEditorData( pEditor, pIndex )
   RETURN Qt_QItemDelegate_setEditorData( ::pPtr, hbqt_ptr( pEditor ), hbqt_ptr( pIndex ) )


METHOD QItemDelegate:setModelData( pEditor, pModel, pIndex )
   RETURN Qt_QItemDelegate_setModelData( ::pPtr, hbqt_ptr( pEditor ), hbqt_ptr( pModel ), hbqt_ptr( pIndex ) )


METHOD QItemDelegate:sizeHint( pOption, pIndex )
   RETURN HB_QSize():from( Qt_QItemDelegate_sizeHint( ::pPtr, hbqt_ptr( pOption ), hbqt_ptr( pIndex ) ) )


METHOD QItemDelegate:updateEditorGeometry( pEditor, pOption, pIndex )
   RETURN Qt_QItemDelegate_updateEditorGeometry( ::pPtr, hbqt_ptr( pEditor ), hbqt_ptr( pOption ), hbqt_ptr( pIndex ) )

