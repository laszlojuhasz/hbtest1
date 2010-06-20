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


CREATE CLASS QCompleter INHERIT HbQtObjectHandler, QObject

   METHOD  new( ... )

   METHOD  caseSensitivity()
   METHOD  completionColumn()
   METHOD  completionCount()
   METHOD  completionMode()
   METHOD  completionModel()
   METHOD  completionPrefix()
   METHOD  completionRole()
   METHOD  currentCompletion()
   METHOD  currentIndex()
   METHOD  currentRow()
   METHOD  model()
   METHOD  modelSorting()
   METHOD  pathFromIndex( pIndex )
   METHOD  popup()
   METHOD  setCaseSensitivity( nCaseSensitivity )
   METHOD  setCompletionColumn( nColumn )
   METHOD  setCompletionMode( nMode )
   METHOD  setCompletionRole( nRole )
   METHOD  setCurrentRow( nRow )
   METHOD  setModel( pModel )
   METHOD  setModelSorting( nSorting )
   METHOD  setPopup( pPopup )
   METHOD  setWidget( pWidget )
   METHOD  splitPath( cPath )
   METHOD  widget()
   METHOD  wrapAround()
   METHOD  complete( pRect )
   METHOD  setCompletionPrefix( cPrefix )
   METHOD  setWrapAround( lWrap )

   ENDCLASS


METHOD QCompleter:new( ... )
   LOCAL p
   FOR EACH p IN { ... }
      hb_pvalue( p:__enumIndex(), hbqt_ptr( p ) )
   NEXT
   ::pPtr := Qt_QCompleter( ... )
   RETURN Self


METHOD QCompleter:caseSensitivity()
   RETURN Qt_QCompleter_caseSensitivity( ::pPtr )


METHOD QCompleter:completionColumn()
   RETURN Qt_QCompleter_completionColumn( ::pPtr )


METHOD QCompleter:completionCount()
   RETURN Qt_QCompleter_completionCount( ::pPtr )


METHOD QCompleter:completionMode()
   RETURN Qt_QCompleter_completionMode( ::pPtr )


METHOD QCompleter:completionModel()
   RETURN Qt_QCompleter_completionModel( ::pPtr )


METHOD QCompleter:completionPrefix()
   RETURN Qt_QCompleter_completionPrefix( ::pPtr )


METHOD QCompleter:completionRole()
   RETURN Qt_QCompleter_completionRole( ::pPtr )


METHOD QCompleter:currentCompletion()
   RETURN Qt_QCompleter_currentCompletion( ::pPtr )


METHOD QCompleter:currentIndex()
   RETURN Qt_QCompleter_currentIndex( ::pPtr )


METHOD QCompleter:currentRow()
   RETURN Qt_QCompleter_currentRow( ::pPtr )


METHOD QCompleter:model()
   RETURN Qt_QCompleter_model( ::pPtr )


METHOD QCompleter:modelSorting()
   RETURN Qt_QCompleter_modelSorting( ::pPtr )


METHOD QCompleter:pathFromIndex( pIndex )
   RETURN Qt_QCompleter_pathFromIndex( ::pPtr, hbqt_ptr( pIndex ) )


METHOD QCompleter:popup()
   RETURN Qt_QCompleter_popup( ::pPtr )


METHOD QCompleter:setCaseSensitivity( nCaseSensitivity )
   RETURN Qt_QCompleter_setCaseSensitivity( ::pPtr, nCaseSensitivity )


METHOD QCompleter:setCompletionColumn( nColumn )
   RETURN Qt_QCompleter_setCompletionColumn( ::pPtr, nColumn )


METHOD QCompleter:setCompletionMode( nMode )
   RETURN Qt_QCompleter_setCompletionMode( ::pPtr, nMode )


METHOD QCompleter:setCompletionRole( nRole )
   RETURN Qt_QCompleter_setCompletionRole( ::pPtr, nRole )


METHOD QCompleter:setCurrentRow( nRow )
   RETURN Qt_QCompleter_setCurrentRow( ::pPtr, nRow )


METHOD QCompleter:setModel( pModel )
   RETURN Qt_QCompleter_setModel( ::pPtr, hbqt_ptr( pModel ) )


METHOD QCompleter:setModelSorting( nSorting )
   RETURN Qt_QCompleter_setModelSorting( ::pPtr, nSorting )


METHOD QCompleter:setPopup( pPopup )
   RETURN Qt_QCompleter_setPopup( ::pPtr, hbqt_ptr( pPopup ) )


METHOD QCompleter:setWidget( pWidget )
   RETURN Qt_QCompleter_setWidget( ::pPtr, hbqt_ptr( pWidget ) )


METHOD QCompleter:splitPath( cPath )
   RETURN Qt_QCompleter_splitPath( ::pPtr, cPath )


METHOD QCompleter:widget()
   RETURN Qt_QCompleter_widget( ::pPtr )


METHOD QCompleter:wrapAround()
   RETURN Qt_QCompleter_wrapAround( ::pPtr )


METHOD QCompleter:complete( pRect )
   RETURN Qt_QCompleter_complete( ::pPtr, hbqt_ptr( pRect ) )


METHOD QCompleter:setCompletionPrefix( cPrefix )
   RETURN Qt_QCompleter_setCompletionPrefix( ::pPtr, cPrefix )


METHOD QCompleter:setWrapAround( lWrap )
   RETURN Qt_QCompleter_setWrapAround( ::pPtr, lWrap )

