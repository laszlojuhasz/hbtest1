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
 *                               EkOnkar
 *                         ( The LORD is ONE )
 *
 *                     Xbase++ Compatible Function
 *
 *                  Pritpal Bedi <pritpal@vouchcac.com>
 *                              08Jun2009
 */
/*----------------------------------------------------------------------*/
/*----------------------------------------------------------------------*/
/*----------------------------------------------------------------------*/

#include "common.ch"

#include "xbp.ch"
#include "hbqt.ch"

/*----------------------------------------------------------------------*/

#define EVENT_BUFFER    200

/*----------------------------------------------------------------------*/

STATIC ts_mutex
STATIC oApp
STATIC aEventLoop := {}

STATIC ts_events
//THREAD STATIC ts_events

THREAD STATIC nEventIn  := 0
THREAD STATIC nEventOut := 0
THREAD STATIC oDummy

THREAD STATIC oAppWindow

THREAD STATIC sEventFilter
THREAD STATIC oEventLoop

/*----------------------------------------------------------------------*/

INIT PROCEDURE Qt_Start()
   qt_qapplication()
   oApp := QApplication():new()
   RETURN

EXIT PROCEDURE Qt_End()
   qt_qapplication_quit()
   RETURN

/*----------------------------------------------------------------------*/
/*
 *  Internal to the XbpParts , Must NOT be called from Application Code
 */
FUNCTION SetAppEvent( nEvent, mp1, mp2, oXbp )

   //hb_mutexLock( ts_mutex )

   IF ++nEventIn > EVENT_BUFFER
      nEventIn := 1
   ENDIF

   ts_events[ nEventIn ] := { nEvent, mp1, mp2, oXbp, ThreadID() }

   //hb_mutexUnLock( ts_mutex )

   RETURN nil

/*----------------------------------------------------------------------*/

FUNCTION AppEvent( mp1, mp2, oXbp )
   LOCAL nEvent, n

   //hb_idleSleep( 0.1 )            /*  May be we need QT based mechanism */

   #if 0
      //hb_mutexLock( ts_mutex )
      oApp:processEvents()
      //hb_mutexUnLock( ts_mutex )
   #else
      /* Event Loop - Can be one per Application */

      //oAppWindow:oEventLoop:processEvents()
      //hb_mutexLock( ts_mutex )

      IF( n := ascan( aEventLoop, {|e_| e_[ 2 ] == threadID() } ) ) > 0
         //hb_outDebug( "<<<<<<<<<<<< "+hb_ntos( threadID() ) )
         aEventLoop[ n,1 ]:processEvents()
         //hb_outDebug( ">>>>>>>>>>>>.... "+hb_ntos( threadID() ) )
      ENDIF

      //hb_mutexUnLock( ts_mutex )
   #endif

   //hb_mutexLock( ts_mutex )

   IF ++nEventOut > EVENT_BUFFER
      nEventOut := 1
   ENDIF

   IF empty( ts_events[ nEventOut ] ) .OR. ts_events[ nEventOut,5 ] <> ThreadID()
      //nEventOut--                           /* Stay back and ensure that no event is missed */
      nEvent := 0
      mp1    := NIL
      mp2    := NIL
      oXbp   := oDummy
   ELSE
      //hb_outDebug( str( threadID() )+'  '+ str( ts_events[ nEventOut,5 ] ) )
      nEvent := ts_events[ nEventOut,1 ]
      mp1    := ts_events[ nEventOut,2 ]
      mp2    := ts_events[ nEventOut,3 ]
      oXbp   := ts_events[ nEventOut,4 ]
      ts_events[ nEventOut ] := NIL
   ENDIF

   //hb_mutexUnLock( ts_mutex )
   RETURN nEvent

/*----------------------------------------------------------------------*/

FUNCTION SetAppWindow( oXbp )
   LOCAL oldAppWindow

   //hb_outDebug( str( threadId() )+'  0' )

   IF empty( ts_mutex )
      ts_mutex := hb_mutexCreate()
   ENDIF
   IF empty( ts_events )
      ts_events := array( EVENT_BUFFER )
   ENDIF
   IF empty( oDummy )
      oDummy := XbpObject():new()
   ENDIF

   oldAppWindow := oAppWindow

   IF hb_isObject( oXbp )
      oAppWindow := oXbp
   ENDIF

   //hb_outDebug( str( threadId() )+'  1' )

   RETURN oldAppWindow

/*----------------------------------------------------------------------*/

FUNCTION SetAppFocus( oXbp )
   LOCAL oldXbpInFocus

   THREAD STATIC oXbpInFocus

   oldXbpInFocus := oXbpInFocus

   IF hb_isObject( oXbp )
      oXbpInFocus := oXbp
      oXbp:oWidget:setFocus()
   ENDIF

   RETURN oldXbpInFocus

/*----------------------------------------------------------------------*/

FUNCTION AppDesktop()

   STATIC oDeskTop

   IF oDeskTop == NIL
      oDeskTop := XbpWindow():new()
      oDeskTop:oWidget := QDesktopWidget():new()
   ENDIF

   RETURN oDeskTop

/*----------------------------------------------------------------------*/

FUNCTION MsgBox( cMsg, cTitle )
   LOCAL oMB

   DEFAULT cTitle TO "  "

   oMB := QMessageBox():new()
   oMB:setText( "<b>"+ cMsg +"</b>" )
   oMB:setIcon( QMessageBox_Information )
   oMB:setParent( SetAppWindow():pWidget )
   oMB:setWindowFlags( Qt_Dialog )
   oMB:setWindowTitle( cTitle )
   SetAppWindow():oWidget:setFocus()
   oMB:exec()

   RETURN nil

/*----------------------------------------------------------------------*/

FUNCTION GraMakeRGBColor( aRGB )
   LOCAL nRGB

   IF hb_isArray( aRGB ) .and. len( aRGB ) == 3
      IF hb_isNumeric( aRGB[ 1 ] ) .and. ( aRGB[ 1 ] >= 0 ) .and. ( aRGB[ 1 ] <= 255 )
         IF hb_isNumeric( aRGB[ 2 ] ) .and. ( aRGB[ 2 ] >= 0 ) .and. ( aRGB[ 2 ] <= 255 )
            IF hb_isNumeric( aRGB[ 3 ] ) .and. ( aRGB[ 3 ] >= 0 ) .and. ( aRGB[ 3 ] <= 255 )
               nRGB := ( aRGB[ 1 ] + ( aRGB[ 2 ] * 256 ) + ( aRGB[ 3 ] * 256 * 256 ) ) + ( 256 * 256 * 256 )
            ENDIF
         ENDIF
      ENDIF
   ENDIF

   RETURN nRGB

/*----------------------------------------------------------------------*/

FUNCTION Xbp_XtoS( xVar )
   LOCAL cType

   cType := valtype( xVar )
   DO CASE
   CASE cType == "N"
      RETURN str( xVar )
   CASE cType == "D"
      RETURN dtoc( xVar )
   CASE cType == "L"
      RETURN IF( xVar, "Yes", "No" )
   CASE cType == "M"
      RETURN xVar
   CASE cType == "C"
      RETURN xVar
   CASE cType == "A"
      RETURN "A:"+hb_ntos( len( xVar ) )
   CASE cType == "O"
      RETURN "<OBJECT>"
   OTHERWISE
      RETURN "<"+cType+">"
   ENDCASE

   RETURN xVar

/*----------------------------------------------------------------------*/

FUNCTION SetEventFilter()
   LOCAL pEventFilter

   IF empty( sEventFilter )
      pEventFilter := QT_QEventFilter()
      IF hb_isPointer( pEventFilter )
         sEventFilter := pEventFilter
      ENDIF
   ENDIF

   RETURN sEventFilter

/*----------------------------------------------------------------------*/

FUNCTION AddEventLoop( oEventLoop )

   hb_mutexLock( ts_mutex )

   aadd( aEventLoop, { oEventLoop, threadID() } )

   hb_mutexUnLock( ts_mutex )

   RETURN nil

/*----------------------------------------------------------------------*/

FUNCTION RemoveEventLoop( oEventLoop )
   LOCAL n

   n := ascan( aEventLoop, {|o_| o_[ 1 ] == oEventLoop } )
   IF n > 0
      hb_mutexLock( ts_mutex )

      aEventLoop[ n,1 ] := NIL
      aEventLoop[ n,2 ] := 0

      hb_mutexUnLock( ts_mutex )
   ENDIF

   RETURN nil

/*----------------------------------------------------------------------*/

