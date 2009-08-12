/*
 * $Id$
 */

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


CREATE CLASS QSizeF

   VAR     pParent
   VAR     pPtr

   METHOD  New()
   METHOD  Configure( xObject )
   METHOD  Destroy()                           INLINE  Qt_QSizeF_destroy( ::pPtr )

   METHOD  boundedTo( pOtherSize )             INLINE  Qt_QSizeF_boundedTo( ::pPtr, pOtherSize )
   METHOD  expandedTo( pOtherSize )            INLINE  Qt_QSizeF_expandedTo( ::pPtr, pOtherSize )
   METHOD  height()                            INLINE  Qt_QSizeF_height( ::pPtr )
   METHOD  isEmpty()                           INLINE  Qt_QSizeF_isEmpty( ::pPtr )
   METHOD  isNull()                            INLINE  Qt_QSizeF_isNull( ::pPtr )
   METHOD  isValid()                           INLINE  Qt_QSizeF_isValid( ::pPtr )
   METHOD  rheight()                           INLINE  Qt_QSizeF_rheight( ::pPtr )
   METHOD  rwidth()                            INLINE  Qt_QSizeF_rwidth( ::pPtr )
   METHOD  scale( nWidth, nHeight, nMode )     INLINE  Qt_QSizeF_scale( ::pPtr, nWidth, nHeight, nMode )
   METHOD  scale_1( pSize, nMode )             INLINE  Qt_QSizeF_scale_1( ::pPtr, pSize, nMode )
   METHOD  setHeight( nHeight )                INLINE  Qt_QSizeF_setHeight( ::pPtr, nHeight )
   METHOD  setWidth( nWidth )                  INLINE  Qt_QSizeF_setWidth( ::pPtr, nWidth )
   METHOD  toSize()                            INLINE  Qt_QSizeF_toSize( ::pPtr )
   METHOD  transpose()                         INLINE  Qt_QSizeF_transpose( ::pPtr )
   METHOD  width()                             INLINE  Qt_QSizeF_width( ::pPtr )

   ENDCLASS

/*----------------------------------------------------------------------*/

METHOD New( ... ) CLASS QSizeF

   ::pPtr := Qt_QSizeF( ... )

   RETURN Self

/*----------------------------------------------------------------------*/

METHOD Configure( xObject ) CLASS QSizeF

   IF hb_isObject( xObject )
      ::pPtr := xObject:pPtr
   ELSEIF hb_isPointer( xObject )
      ::pPtr := xObject
   ENDIF

   RETURN Self

/*----------------------------------------------------------------------*/
