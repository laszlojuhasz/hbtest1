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


FUNCTION QMimeData( ... )
   RETURN HB_QMimeData():new( ... )

FUNCTION QMimeDataFrom( ... )
   RETURN HB_QMimeData():from( ... )

FUNCTION QMimeDataFromPointer( ... )
   RETURN HB_QMimeData():fromPointer( ... )


CREATE CLASS QMimeData INHERIT HbQtObjectHandler, HB_QObject FUNCTION HB_QMimeData

   METHOD  new( ... )

   METHOD  clear                         // (  )                                               -> NIL
   METHOD  colorData                     // (  )                                               -> oQVariant
   METHOD  data                          // ( cMimeType )                                      -> oQByteArray
   METHOD  formats                       // (  )                                               -> oQStringList
   METHOD  hasColor                      // (  )                                               -> lBool
   METHOD  hasFormat                     // ( cMimeType )                                      -> lBool
   METHOD  hasHtml                       // (  )                                               -> lBool
   METHOD  hasImage                      // (  )                                               -> lBool
   METHOD  hasText                       // (  )                                               -> lBool
   METHOD  hasUrls                       // (  )                                               -> lBool
   METHOD  html                          // (  )                                               -> cQString
   METHOD  imageData                     // (  )                                               -> oQVariant
   METHOD  removeFormat                  // ( cMimeType )                                      -> NIL
   METHOD  setColorData                  // ( oQVariant )                                      -> NIL
   METHOD  setData                       // ( cMimeType, oQByteArray )                         -> NIL
   METHOD  setHtml                       // ( cHtml )                                          -> NIL
   METHOD  setImageData                  // ( oQVariant )                                      -> NIL
   METHOD  setText                       // ( cText )                                          -> NIL
   METHOD  text                          // (  )                                               -> cQString
   METHOD  urls                          // (  )                                               -> oQList_QUrl>
   METHOD  hbUrlList                     // (  )                                               -> oQStringList

   ENDCLASS


METHOD QMimeData:new( ... )
   LOCAL p
   FOR EACH p IN { ... }
      hb_pvalue( p:__enumIndex(), hbqt_ptr( p ) )
   NEXT
   ::pPtr := Qt_QMimeData( ... )
   RETURN Self


METHOD QMimeData:clear( ... )
   SWITCH PCount()
   CASE 0
      RETURN Qt_QMimeData_clear( ::pPtr, ... )
   ENDSWITCH
   RETURN hbqt_error()


METHOD QMimeData:colorData( ... )
   SWITCH PCount()
   CASE 0
      RETURN QVariantFromPointer( Qt_QMimeData_colorData( ::pPtr, ... ) )
   ENDSWITCH
   RETURN hbqt_error()


METHOD QMimeData:data( ... )
   SWITCH PCount()
   CASE 1
      DO CASE
      CASE hb_isChar( hb_pvalue( 1 ) )
         RETURN QByteArrayFromPointer( Qt_QMimeData_data( ::pPtr, ... ) )
      ENDCASE
      EXIT
   ENDSWITCH
   RETURN hbqt_error()


METHOD QMimeData:formats( ... )
   SWITCH PCount()
   CASE 0
      RETURN QStringListFromPointer( Qt_QMimeData_formats( ::pPtr, ... ) )
   ENDSWITCH
   RETURN hbqt_error()


METHOD QMimeData:hasColor( ... )
   SWITCH PCount()
   CASE 0
      RETURN Qt_QMimeData_hasColor( ::pPtr, ... )
   ENDSWITCH
   RETURN hbqt_error()


METHOD QMimeData:hasFormat( ... )
   SWITCH PCount()
   CASE 1
      DO CASE
      CASE hb_isChar( hb_pvalue( 1 ) )
         RETURN Qt_QMimeData_hasFormat( ::pPtr, ... )
      ENDCASE
      EXIT
   ENDSWITCH
   RETURN hbqt_error()


METHOD QMimeData:hasHtml( ... )
   SWITCH PCount()
   CASE 0
      RETURN Qt_QMimeData_hasHtml( ::pPtr, ... )
   ENDSWITCH
   RETURN hbqt_error()


METHOD QMimeData:hasImage( ... )
   SWITCH PCount()
   CASE 0
      RETURN Qt_QMimeData_hasImage( ::pPtr, ... )
   ENDSWITCH
   RETURN hbqt_error()


METHOD QMimeData:hasText( ... )
   SWITCH PCount()
   CASE 0
      RETURN Qt_QMimeData_hasText( ::pPtr, ... )
   ENDSWITCH
   RETURN hbqt_error()


METHOD QMimeData:hasUrls( ... )
   SWITCH PCount()
   CASE 0
      RETURN Qt_QMimeData_hasUrls( ::pPtr, ... )
   ENDSWITCH
   RETURN hbqt_error()


METHOD QMimeData:html( ... )
   SWITCH PCount()
   CASE 0
      RETURN Qt_QMimeData_html( ::pPtr, ... )
   ENDSWITCH
   RETURN hbqt_error()


METHOD QMimeData:imageData( ... )
   SWITCH PCount()
   CASE 0
      RETURN QVariantFromPointer( Qt_QMimeData_imageData( ::pPtr, ... ) )
   ENDSWITCH
   RETURN hbqt_error()


METHOD QMimeData:removeFormat( ... )
   SWITCH PCount()
   CASE 1
      DO CASE
      CASE hb_isChar( hb_pvalue( 1 ) )
         RETURN Qt_QMimeData_removeFormat( ::pPtr, ... )
      ENDCASE
      EXIT
   ENDSWITCH
   RETURN hbqt_error()


METHOD QMimeData:setColorData( ... )
   SWITCH PCount()
   CASE 1
      DO CASE
      CASE hb_isObject( hb_pvalue( 1 ) )
         RETURN Qt_QMimeData_setColorData( ::pPtr, ... )
      ENDCASE
      EXIT
   ENDSWITCH
   RETURN hbqt_error()


METHOD QMimeData:setData( ... )
   SWITCH PCount()
   CASE 2
      DO CASE
      CASE hb_isChar( hb_pvalue( 1 ) ) .AND. hb_isObject( hb_pvalue( 2 ) )
         RETURN Qt_QMimeData_setData( ::pPtr, ... )
      ENDCASE
      EXIT
   ENDSWITCH
   RETURN hbqt_error()


METHOD QMimeData:setHtml( ... )
   SWITCH PCount()
   CASE 1
      DO CASE
      CASE hb_isChar( hb_pvalue( 1 ) )
         RETURN Qt_QMimeData_setHtml( ::pPtr, ... )
      ENDCASE
      EXIT
   ENDSWITCH
   RETURN hbqt_error()


METHOD QMimeData:setImageData( ... )
   SWITCH PCount()
   CASE 1
      DO CASE
      CASE hb_isObject( hb_pvalue( 1 ) )
         RETURN Qt_QMimeData_setImageData( ::pPtr, ... )
      ENDCASE
      EXIT
   ENDSWITCH
   RETURN hbqt_error()


METHOD QMimeData:setText( ... )
   SWITCH PCount()
   CASE 1
      DO CASE
      CASE hb_isChar( hb_pvalue( 1 ) )
         RETURN Qt_QMimeData_setText( ::pPtr, ... )
      ENDCASE
      EXIT
   ENDSWITCH
   RETURN hbqt_error()


METHOD QMimeData:text( ... )
   SWITCH PCount()
   CASE 0
      RETURN Qt_QMimeData_text( ::pPtr, ... )
   ENDSWITCH
   RETURN hbqt_error()


METHOD QMimeData:urls( ... )
   SWITCH PCount()
   CASE 0
      RETURN QListFromPointer( Qt_QMimeData_urls( ::pPtr, ... ) )
   ENDSWITCH
   RETURN hbqt_error()


METHOD QMimeData:hbUrlList( ... )
   SWITCH PCount()
   CASE 0
      RETURN QStringListFromPointer( Qt_QMimeData_hbUrlList( ::pPtr, ... ) )
   ENDSWITCH
   RETURN hbqt_error()

