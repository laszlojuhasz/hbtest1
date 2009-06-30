/*
 * $Id$
 */

/*
 * Harbour Project source code:
 * Source file for the Xbp*Classes
 *
 * Copyright 2009 Pritpal Bedi <pritpal@vouchcac.com>
 * http://www.harbour-project.org
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
/*----------------------------------------------------------------------*/
/*----------------------------------------------------------------------*/
/*
 *                                EkOnkar
 *                          ( The LORD is ONE )
 *
 *                   Xbase++ xbpStatic compatible Class
 *
 *                  Pritpal Bedi <pritpal@vouchcac.com>
 *                               29Jun2009
 */
/*----------------------------------------------------------------------*/
/*----------------------------------------------------------------------*/
/*----------------------------------------------------------------------*/

#include "hbclass.ch"
#include "common.ch"

#include "xbp.ch"
#include "appevent.ch"
#include "hbqt.ch"

/*----------------------------------------------------------------------*/

CLASS XbpStatic  INHERIT  XbpWindow

   DATA     autoSize                              INIT .F.
   DATA     caption                               INIT ""
   DATA     clipParent                            INIT .T.
   DATA     clipSiblings                          INIT .F.
   DATA     options                               INIT -1
   DATA     type                                  INIT -1

   DATA     hBitmap

   METHOD   new()
   METHOD   create()
   METHOD   configure()
   METHOD   destroy()

   METHOD   setCaption( xCaption, cDll )

   METHOD   handleEvent()

   ENDCLASS
/*----------------------------------------------------------------------*/

METHOD XbpStatic:new( oParent, oOwner, aPos, aSize, aPresParams, lVisible )

   ::xbpWindow:init( oParent, oOwner, aPos, aSize, aPresParams, lVisible )

   RETURN Self

/*----------------------------------------------------------------------*/

METHOD XbpStatic:create( oParent, oOwner, aPos, aSize, aPresParams, lVisible )

   ::xbpWindow:create( oParent, oOwner, aPos, aSize, aPresParams, lVisible )

   DO CASE
   CASE ::type == XBPSTATIC_TYPE_TEXT
      ::oWidget := QLabel():new( ::pParent )

      IF ( hb_bitAnd( ::options, XBPSTATIC_TEXT_LEFT ) == XBPSTATIC_TEXT_LEFT )
         ::oWidget:setAlignment( Qt_AlignLeft )
      ENDIF
      IF ( hb_bitAnd( ::options, XBPSTATIC_TEXT_RIGHT ) == XBPSTATIC_TEXT_RIGHT )
         ::oWidget:setAlignment( Qt_AlignRight )
      ENDIF
      IF ( hb_bitAnd( ::options, XBPSTATIC_TEXT_CENTER ) == XBPSTATIC_TEXT_CENTER )
         ::oWidget:setAlignment( Qt_AlignCenter )
      ENDIF
      IF ( hb_bitAnd( ::options, XBPSTATIC_TEXT_TOP ) == XBPSTATIC_TEXT_TOP )
         ::oWidget:setAlignment( hb_bitOr( ::oWidget:alignment, Qt_AlignTop ) )
      ENDIF
      IF ( hb_bitAnd( ::options, XBPSTATIC_TEXT_VCENTER ) == XBPSTATIC_TEXT_VCENTER )
         ::oWidget:setAlignment( hb_bitOr( ::oWidget:alignment, Qt_AlignVCenter ) )
      ENDIF
      IF ( hb_bitAnd( ::options, XBPSTATIC_TEXT_BOTTOM ) == XBPSTATIC_TEXT_BOTTOM )
         ::oWidget:setAlignment( hb_bitOr( ::oWidget:alignment, Qt_AlignBottom ) )
      ENDIF
      IF ( hb_bitAnd( ::options, XBPSTATIC_TEXT_WORDBREAK ) == XBPSTATIC_TEXT_WORDBREAK )
         ::oWidget:setWordWrap( .T. )
      ENDIF

   CASE ::type == XBPSTATIC_TYPE_GROUPBOX
      ::oWidget := QGroupBox():new( ::pParent )

   CASE ::type == XBPSTATIC_TYPE_FGNDFRAME     // rectangle in foreground color, not filled
      ::oWidget := QFrame():new( ::pParent )

   CASE ::type == XBPSTATIC_TYPE_BGNDFRAME
      ::oWidget := QFrame():new( ::pParent )

   CASE ::type == XBPSTATIC_TYPE_HALFTONEFRAME
      ::oWidget := QFrame():new( ::pParent )

   CASE ::type == XBPSTATIC_TYPE_FGNDRECT
      ::oWidget := QFrame():new( ::pParent )

   CASE ::type == XBPSTATIC_TYPE_BGNDRECT
      ::oWidget := QFrame():new( ::pParent )

   CASE ::type == XBPSTATIC_TYPE_RAISEDRECT
      ::oWidget := QFrame():new( ::pParent )
      ::oWidget:setFrameStyle( QFrame_Panel + QFrame_Raised )

   CASE ::type == XBPSTATIC_TYPE_RECESSEDRECT
      ::oWidget := QFrame():new( ::pParent )
      ::oWidget:setFrameStyle( QFrame_Panel + QFrame_Sunken )

   CASE ::type == XBPSTATIC_TYPE_HALFTONERECT
      ::oWidget := QFrame():new( ::pParent )

   CASE ::type == XBPSTATIC_TYPE_RAISEDBOX
      ::oWidget := QFrame():new( ::pParent )
      ::oWidget:setFrameStyle( QFrame_Panel + QFrame_Raised )

   CASE ::type == XBPSTATIC_TYPE_RECESSEDBOX
      ::oWidget := QFrame():new( ::pParent )
      ::oWidget:setFrameStyle( QFrame_Panel + QFrame_Sunken )

   CASE ::type == XBPSTATIC_TYPE_RAISEDLINE
      ::oWidget := QFrame():new( ::pParent )
      IF ::aPos[ 1 ] + ::aSize[ 1 ] >= ::aPos[ 2 ] + ::aSize[ 2 ]
         ::oWidget:setFrameStyle( QFrame_HLine + QFrame_Raised )
      ELSE
         ::oWidget:setFrameStyle( QFrame_VLine + QFrame_Raised )
      ENDIF

   CASE ::type == XBPSTATIC_TYPE_RECESSEDLINE
      ::oWidget := QFrame():new( ::pParent )
      IF ::aPos[ 1 ] + ::aSize[ 1 ] >= ::aPos[ 2 ] + ::aSize[ 2 ]
         ::oWidget:setFrameStyle( QFrame_HLine + QFrame_Sunken )
      ELSE
         ::oWidget:setFrameStyle( QFrame_VLine + QFrame_Sunken )
      ENDIF

   CASE ::type == XBPSTATIC_TYPE_ICON

   CASE ::type == XBPSTATIC_TYPE_SYSICON

   CASE ::type == XBPSTATIC_TYPE_BITMAP
      IF     ::options == XBPSTATIC_BITMAP_TILED
      ELSEIF ::options == XBPSTATIC_BITMAP_SCALED
      ELSE
      ENDIF

   ENDCASE

   #if 0
   /* Options */
   IF ( ascan( { XBPSTATIC_TYPE_FGNDFRAME, XBPSTATIC_TYPE_BGNDFRAME, XBPSTATIC_TYPE_HALFTONEFRAME }, ::type ) > 0 )
      IF     ( hb_bitAnd( ::options, XBPSTATIC_FRAMETHIN ) == XBPSTATIC_FRAMETHIN )
         ::style += WS_BORDER

      ELSEIF ( hb_bitAnd( ::options, XBPSTATIC_FRAMETHICK ) == XBPSTATIC_FRAMETHICK )
         ::style += WS_DLGFRAME

      ENDIF
   ENDIF

   IF ::type == XBPSTATIC_TYPE_TEXT
      IF ::options == XBPSTATIC_FRAMETHIN
         ::style += WS_BORDER
      ELSEIF ::options == XBPSTATIC_FRAMETHICK
         ::style += WS_DLGFRAME
      ENDIF
   ENDIF
   #endif


   ::setPosAndSize()
   IF ::visible
      ::show()
   ENDIF

   ::setCaption( ::caption )

   ::oParent:addChild( SELF )
   RETURN Self

/*----------------------------------------------------------------------*/

METHOD XbpStatic:handleEvent( nEvent, mp1, mp2 )

   HB_SYMBOL_UNUSED( nEvent )
   HB_SYMBOL_UNUSED( mp1    )
   HB_SYMBOL_UNUSED( mp2    )

   RETURN HBXBP_EVENT_UNHANDLED

/*----------------------------------------------------------------------*/

METHOD XbpStatic:destroy()

   ::xbpWindow:destroy()

   RETURN NIL

/*----------------------------------------------------------------------*/

METHOD XbpStatic:configure( oParent, oOwner, aPos, aSize, aPresParams, lVisible )

   ::Initialize( oParent, oOwner, aPos, aSize, aPresParams, lVisible )

   RETURN Self

/*----------------------------------------------------------------------*/

METHOD XbpStatic:setCaption( xCaption, cDll )

   HB_SYMBOL_UNUSED( cDll )

   DEFAULT xCaption TO ::caption
   ::caption := xCaption

   IF !empty( ::caption )
      DO CASE
      CASE ::type == XBPSTATIC_TYPE_GROUPBOX
         ::oWidget:setTitle( ::caption )

      CASE ::type == XBPSTATIC_TYPE_TEXT
         ::oWidget:setText( ::caption )

      CASE ::type == XBPSTATIC_TYPE_BITMAP
      ENDCASE
   ENDIF

   RETURN Self

/*----------------------------------------------------------------------*/
