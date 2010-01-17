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


CREATE CLASS QClipboard INHERIT HbQtObjectHandler, QObject

   METHOD  new( ... )

   METHOD  clear( nMode )
   METHOD  image( nMode )
   METHOD  ownsClipboard()
   METHOD  ownsFindBuffer()
   METHOD  ownsSelection()
   METHOD  pixmap( nMode )
   METHOD  setImage( pImage, nMode )
   METHOD  setMimeData( pSrc, nMode )
   METHOD  setPixmap( pPixmap, nMode )
   METHOD  setText( cText, nMode )
   METHOD  supportsFindBuffer()
   METHOD  supportsSelection()
   METHOD  text( nMode )

   ENDCLASS


METHOD QClipboard:new( ... )
   LOCAL p
   FOR EACH p IN { ... }
      hb_pvalue( p:__enumIndex(), hbqt_ptr( p ) )
   NEXT
   ::pPtr := Qt_QClipboard( ... )
   RETURN Self


METHOD QClipboard:clear( nMode )
   RETURN Qt_QClipboard_clear( ::pPtr, nMode )


METHOD QClipboard:image( nMode )
   RETURN Qt_QClipboard_image( ::pPtr, nMode )


METHOD QClipboard:ownsClipboard()
   RETURN Qt_QClipboard_ownsClipboard( ::pPtr )


METHOD QClipboard:ownsFindBuffer()
   RETURN Qt_QClipboard_ownsFindBuffer( ::pPtr )


METHOD QClipboard:ownsSelection()
   RETURN Qt_QClipboard_ownsSelection( ::pPtr )


METHOD QClipboard:pixmap( nMode )
   RETURN Qt_QClipboard_pixmap( ::pPtr, nMode )


METHOD QClipboard:setImage( pImage, nMode )
   RETURN Qt_QClipboard_setImage( ::pPtr, hbqt_ptr( pImage ), nMode )


METHOD QClipboard:setMimeData( pSrc, nMode )
   RETURN Qt_QClipboard_setMimeData( ::pPtr, hbqt_ptr( pSrc ), nMode )


METHOD QClipboard:setPixmap( pPixmap, nMode )
   RETURN Qt_QClipboard_setPixmap( ::pPtr, hbqt_ptr( pPixmap ), nMode )


METHOD QClipboard:setText( cText, nMode )
   RETURN Qt_QClipboard_setText( ::pPtr, cText, nMode )


METHOD QClipboard:supportsFindBuffer()
   RETURN Qt_QClipboard_supportsFindBuffer( ::pPtr )


METHOD QClipboard:supportsSelection()
   RETURN Qt_QClipboard_supportsSelection( ::pPtr )


METHOD QClipboard:text( nMode )
   RETURN Qt_QClipboard_text( ::pPtr, nMode )

