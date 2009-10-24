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


CREATE CLASS QImageWriter

   VAR     pParent
   VAR     pPtr

   METHOD  New()
   METHOD  Configure( xObject )

   METHOD  canWrite()
   METHOD  compression()
   METHOD  device()
   METHOD  error()
   METHOD  errorString()
   METHOD  fileName()
   METHOD  format()
   METHOD  gamma()
   METHOD  quality()
   METHOD  setCompression( nCompression )
   METHOD  setDevice( pDevice )
   METHOD  setFileName( cFileName )
   METHOD  setFormat( pFormat )
   METHOD  setGamma( nGamma )
   METHOD  setQuality( nQuality )
   METHOD  setText( cKey, cText )
   METHOD  supportsOption( nOption )
   METHOD  write( pImage )

   ENDCLASS

/*----------------------------------------------------------------------*/

METHOD QImageWriter:New( pParent )
   ::pParent := pParent
   ::pPtr := Qt_QImageWriter( pParent )
   RETURN Self


METHOD QImageWriter:Configure( xObject )
   IF hb_isObject( xObject )
      ::pPtr := xObject:pPtr
   ELSEIF hb_isPointer( xObject )
      ::pPtr := xObject
   ENDIF
   RETURN Self


METHOD QImageWriter:canWrite()
   RETURN Qt_QImageWriter_canWrite( ::pPtr )


METHOD QImageWriter:compression()
   RETURN Qt_QImageWriter_compression( ::pPtr )


METHOD QImageWriter:device()
   RETURN Qt_QImageWriter_device( ::pPtr )


METHOD QImageWriter:error()
   RETURN Qt_QImageWriter_error( ::pPtr )


METHOD QImageWriter:errorString()
   RETURN Qt_QImageWriter_errorString( ::pPtr )


METHOD QImageWriter:fileName()
   RETURN Qt_QImageWriter_fileName( ::pPtr )


METHOD QImageWriter:format()
   RETURN Qt_QImageWriter_format( ::pPtr )


METHOD QImageWriter:gamma()
   RETURN Qt_QImageWriter_gamma( ::pPtr )


METHOD QImageWriter:quality()
   RETURN Qt_QImageWriter_quality( ::pPtr )


METHOD QImageWriter:setCompression( nCompression )
   RETURN Qt_QImageWriter_setCompression( ::pPtr, nCompression )


METHOD QImageWriter:setDevice( pDevice )
   RETURN Qt_QImageWriter_setDevice( ::pPtr, pDevice )


METHOD QImageWriter:setFileName( cFileName )
   RETURN Qt_QImageWriter_setFileName( ::pPtr, cFileName )


METHOD QImageWriter:setFormat( pFormat )
   RETURN Qt_QImageWriter_setFormat( ::pPtr, pFormat )


METHOD QImageWriter:setGamma( nGamma )
   RETURN Qt_QImageWriter_setGamma( ::pPtr, nGamma )


METHOD QImageWriter:setQuality( nQuality )
   RETURN Qt_QImageWriter_setQuality( ::pPtr, nQuality )


METHOD QImageWriter:setText( cKey, cText )
   RETURN Qt_QImageWriter_setText( ::pPtr, cKey, cText )


METHOD QImageWriter:supportsOption( nOption )
   RETURN Qt_QImageWriter_supportsOption( ::pPtr, nOption )


METHOD QImageWriter:write( pImage )
   RETURN Qt_QImageWriter_write( ::pPtr, pImage )

