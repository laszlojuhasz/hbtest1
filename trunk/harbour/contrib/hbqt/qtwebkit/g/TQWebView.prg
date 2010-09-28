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


FUNCTION QWebView( ... )
   RETURN HB_QWebView():new( ... )


CREATE CLASS QWebView INHERIT HbQtObjectHandler, HB_QWidget FUNCTION HB_QWebView

   METHOD  new( ... )

   METHOD  findText( cSubString, nOptions )
   METHOD  history()
   METHOD  icon()
   METHOD  isModified()
   METHOD  load( ... )
   METHOD  page()
   METHOD  pageAction( nAction )
   METHOD  selectedText()
   METHOD  setContent( pData, cMimeType, pBaseUrl )
   METHOD  setHtml( cHtml, pBaseUrl )
   METHOD  setPage( pPage )
   METHOD  setTextSizeMultiplier( nFactor )
   METHOD  setUrl( pUrl )
   METHOD  setZoomFactor( nFactor )
   METHOD  settings()
   METHOD  textSizeMultiplier()
   METHOD  title()
   METHOD  triggerPageAction( nAction, lChecked )
   METHOD  url()
   METHOD  zoomFactor()
   METHOD  back()
   METHOD  forward()
   METHOD  print( pPrinter )
   METHOD  reload()
   METHOD  stop()

   ENDCLASS


METHOD QWebView:new( ... )
   LOCAL p
   FOR EACH p IN { ... }
      hb_pvalue( p:__enumIndex(), hbqt_ptr( p ) )
   NEXT
   ::pPtr := Qt_QWebView( ... )
   RETURN Self


METHOD QWebView:findText( cSubString, nOptions )
   RETURN Qt_QWebView_findText( ::pPtr, cSubString, nOptions )


METHOD QWebView:history()
   RETURN HB_QWebHistory():from( Qt_QWebView_history( ::pPtr ) )


METHOD QWebView:icon()
   RETURN HB_QIcon():from( Qt_QWebView_icon( ::pPtr ) )


METHOD QWebView:isModified()
   RETURN Qt_QWebView_isModified( ::pPtr )


METHOD QWebView:load( ... )
   SWITCH PCount()
   CASE 3
      DO CASE
      CASE hb_isObject( hb_pvalue( 1 ) ) .AND. hb_isNumeric( hb_pvalue( 2 ) ) .AND. hb_isObject( hb_pvalue( 3 ) )
         RETURN Qt_QWebView_load_1( ::pPtr, ... )
      ENDCASE
      EXIT
   CASE 1
      DO CASE
      CASE hb_isObject( hb_pvalue( 1 ) )
         SWITCH __objGetClsName( hb_pvalue( 1 ) )
         CASE "QURL"
            RETURN Qt_QWebView_load( ::pPtr, ... )
         CASE "QNETWORKREQUEST"
            RETURN Qt_QWebView_load_1( ::pPtr, ... )
         ENDSWITCH
      ENDCASE
      EXIT
   ENDSWITCH
   RETURN hbqt_error()


METHOD QWebView:page()
   RETURN HB_QWebPage():from( Qt_QWebView_page( ::pPtr ) )


METHOD QWebView:pageAction( nAction )
   RETURN HB_QAction():from( Qt_QWebView_pageAction( ::pPtr, nAction ) )


METHOD QWebView:selectedText()
   RETURN Qt_QWebView_selectedText( ::pPtr )


METHOD QWebView:setContent( pData, cMimeType, pBaseUrl )
   RETURN Qt_QWebView_setContent( ::pPtr, hbqt_ptr( pData ), cMimeType, hbqt_ptr( pBaseUrl ) )


METHOD QWebView:setHtml( cHtml, pBaseUrl )
   RETURN Qt_QWebView_setHtml( ::pPtr, cHtml, hbqt_ptr( pBaseUrl ) )


METHOD QWebView:setPage( pPage )
   RETURN Qt_QWebView_setPage( ::pPtr, hbqt_ptr( pPage ) )


METHOD QWebView:setTextSizeMultiplier( nFactor )
   RETURN Qt_QWebView_setTextSizeMultiplier( ::pPtr, nFactor )


METHOD QWebView:setUrl( pUrl )
   RETURN Qt_QWebView_setUrl( ::pPtr, hbqt_ptr( pUrl ) )


METHOD QWebView:setZoomFactor( nFactor )
   RETURN Qt_QWebView_setZoomFactor( ::pPtr, nFactor )


METHOD QWebView:settings()
   RETURN HB_QWebSettings():from( Qt_QWebView_settings( ::pPtr ) )


METHOD QWebView:textSizeMultiplier()
   RETURN Qt_QWebView_textSizeMultiplier( ::pPtr )


METHOD QWebView:title()
   RETURN Qt_QWebView_title( ::pPtr )


METHOD QWebView:triggerPageAction( nAction, lChecked )
   RETURN Qt_QWebView_triggerPageAction( ::pPtr, nAction, lChecked )


METHOD QWebView:url()
   RETURN HB_QUrl():from( Qt_QWebView_url( ::pPtr ) )


METHOD QWebView:zoomFactor()
   RETURN Qt_QWebView_zoomFactor( ::pPtr )


METHOD QWebView:back()
   RETURN Qt_QWebView_back( ::pPtr )


METHOD QWebView:forward()
   RETURN Qt_QWebView_forward( ::pPtr )


METHOD QWebView:print( pPrinter )
   RETURN Qt_QWebView_print( ::pPtr, hbqt_ptr( pPrinter ) )


METHOD QWebView:reload()
   RETURN Qt_QWebView_reload( ::pPtr )


METHOD QWebView:stop()
   RETURN Qt_QWebView_stop( ::pPtr )

