/*
 * $Id$
 */

/*
 * Harbour Project source code:
 * SMS library
 *
 * Copyright 2009 Viktor Szakats (harbour.01 syenar.hu)
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

/* TODO: - Unicode support */
/* TODO: - Receive support */

/* NOTE: Source of information:
         http://www.smssolutions.net/tutorials/gsm/receivesmsat/
         http://www.developershome.com/sms/readSmsByAtCommands.asp 
         [vszakats] */

/* NOTE: As it happens ATM, Harbour doesn't have a unified
         serial interface. hbtpathy works only for *nix systems,
         and OS/2, but not for Windows. hbwin obviously only
         works for Windows platforms. For this reason in this
         lib I have to resort to two different libs depending
         on the platform. Hopefully hbtpathy will eventually
         fixed to either use hbwin or implement nativ port
         access. [vszakats] */

#define HB_HAS_TELEPATHY

#include "common.ch"

#include "telepath.ch"

STATIC FUNCTION port_send( h, s, t )
   RETURN tp_send( h, s, t )

STATIC FUNCTION port_rece( h, n, t )
   DEFAULT n TO 64
   DEFAULT t TO 0.2
   RETURN tp_recv( h, n, t )

FUNCTION sms_Send( cPort, cPhoneNo, cText, lNotification, cPIN )
   LOCAL smsctx
   LOCAL lRetVal

   IF ! Empty( smsctx := smsctx_New( cPort ) )
      smsctx_PIN( smsctx, cPIN )
      lRetVal := smsctx_Send( smsctx, cPhoneNo, cText, lNotification )
      smsctx_Close( smsctx )
   ELSE
      lRetVal := .F.
   ENDIF

   RETURN lRetVal

FUNCTION sms_ReceiveAll( cPort, cPIN )
   LOCAL smsctx
   LOCAL aRetVal

   IF ! Empty( smsctx := smsctx_New( cPort ) )
      smsctx_PIN( smsctx, cPIN )
      aRetVal := smsctx_Receive( smsctx )
      smsctx_Close( smsctx )
   ELSE
      aRetVal := NIL
   ENDIF

   RETURN aRetVal

/* --------------------- */

#define _SMSCTX_xHnd          1
#define _SMSCTX_cPIN          2
#define _SMSCTX_MAX_          2

FUNCTION smsctx_New( cPort )
   LOCAL smsctx[ _SMSCTX_MAX_ ]

   smsctx[ _SMSCTX_xHnd ] := 1
   IF tp_open( smsctx[ _SMSCTX_xHnd ], NIL, NIL, 9600, 8, "N", 1, cPort ) == 0
      RETURN smsctx
   ENDIF

   RETURN NIL

FUNCTION smsctx_Send( smsctx, cPhoneNo, cText, lNotification )
   LOCAL tmp

   IF ! ISARRAY( smsctx ) .OR. Len( smsctx ) != _SMSCTX_MAX_
      RETURN -1
   ENDIF

   port_send( smsctx[ _SMSCTX_xHnd ], "ATE0V1Q0" + Chr( 13 ) )
   IF IsOK( port_rece( smsctx[ _SMSCTX_xHnd ] ) )

      port_send( smsctx[ _SMSCTX_xHnd ], "AT+CMGF=1" + Chr( 13 ) )
      IF StripCRLF( port_rece( smsctx[ _SMSCTX_xHnd ] ) ) == "OK"

         IF ! Empty( smsctx[ _SMSCTX_cPIN ] )
            port_send( smsctx[ _SMSCTX_xHnd ], 'AT+CPIN="' + smsctx[ _SMSCTX_cPIN ] + '"' + Chr( 13 ) )
            IF !( StripCRLF( port_rece( smsctx[ _SMSCTX_xHnd ] ) ) == "OK" )
               RETURN -5
            ENDIF
         ENDIF

         port_send( smsctx[ _SMSCTX_xHnd ], "AT+CMGF=1" + Chr( 13 ) )
         IF StripCRLF( port_rece( smsctx[ _SMSCTX_xHnd ] ) ) == "OK"

            IF ISLOGICAL( lNotification )
               port_send( smsctx[ _SMSCTX_xHnd ], "AT+CSMP?" + Chr( 13 ) )
               tmp := GetLines( port_rece( smsctx[ _SMSCTX_xHnd ] ) )
               IF Len( tmp ) < 2
                  RETURN -6
               ENDIF
               IF !( ATail( tmp ) == "OK" )
                  RETURN -7
               ENDIF
               IF !( Left( tmp[ 1 ], Len( "+CSMP: " ) ) == "+CSMP: " )
                  RETURN -8
               ENDIF
               tmp := GetList( SubStr( tmp[ 1 ], Len( "+CSMP: " ) + 1 ) )
               IF Len( tmp ) > 1
                  IF lNotification
                     tmp[ 1 ] := hb_ntos( hb_bitSet( Val( tmp[ 1 ] ), 5 ) )
                  ELSE
                     tmp[ 1 ] := hb_ntos( hb_bitReset( Val( tmp[ 1 ] ), 5 ) )
                  ENDIF
                  port_send( smsctx[ _SMSCTX_xHnd ], "AT+CSMP=" + MakeList( tmp ) + Chr( 13 ) )
                  IF !( StripCRLF( port_rece( smsctx[ _SMSCTX_xHnd ] ) ) == "OK" )
                     RETURN -9
                  ENDIF
               ENDIF
            ENDIF

            port_send( smsctx[ _SMSCTX_xHnd ], 'AT+CMGS="' + cPhoneNo + '"' + Chr( 13 ) )
            IF StripCRLF( port_rece( smsctx[ _SMSCTX_xHnd ] ) ) == "> "
               port_send( smsctx[ _SMSCTX_xHnd ], StrTran( cText, Chr( 13 ) ) + Chr( 26 ) )
               tmp := StripCRLF( port_rece( smsctx[ _SMSCTX_xHnd ], NIL, 2 ) )
               IF Left( tmp, Len( "+CMGS: " ) ) == "+CMGS: "
                  RETURN 0
               ELSE
                  RETURN -10
               ENDIF
            ELSE
               RETURN -11
            ENDIF
         ELSE
            RETURN -12
         ENDIF
      ELSE
         RETURN -4
      ENDIF
   ELSE
      RETURN -3
   ENDIF

   RETURN -2

FUNCTION smsctx_Receive( smsctx )

   IF ! ISARRAY( smsctx ) .OR. Len( smsctx ) != _SMSCTX_MAX_
      RETURN NIL
   ENDIF

   // ...

   RETURN {}

FUNCTION smsctx_PIN( smsctx, cPIN )
   LOCAL cOldValue

   IF ! ISARRAY( smsctx ) .OR. Len( smsctx ) != _SMSCTX_MAX_
      RETURN NIL
   ENDIF

   cOldValue := smsctx[ _SMSCTX_cPIN ]
   IF cPIN == NIL .OR. ( ISCHARACTER( cPIN ) .AND. Len( cPIN ) == 4 )
      smsctx[ _SMSCTX_cPIN ] := cPIN
   ENDIF

   RETURN cOldValue

FUNCTION smsctx_Close( smsctx )

   IF ! ISARRAY( smsctx ) .OR. Len( smsctx ) != _SMSCTX_MAX_
      RETURN .F.
   ENDIF

   RETURN tp_close( smsctx[ _SMSCTX_xHnd ] ) == 0

STATIC FUNCTION StripCR( cString )
   RETURN StrTran( cString, Chr( 13 ) )

STATIC FUNCTION StripCRLF( cString )
   RETURN StrTran( cString, Chr( 13 ) + Chr( 10 ) )

STATIC FUNCTION IsOK( cString )
   LOCAL tmp := GetLines( cString )

   RETURN ! Empty( tmp ) .AND. ATail( tmp ) == "OK"

STATIC FUNCTION GetLines( cString )
   LOCAL aLine := {}
   LOCAL tmp

   IF Left( cString, 2 ) == Chr( 13 ) + Chr( 10 )
      cString := SubStr( cString, Len( Chr( 13 ) + Chr( 10 ) ) + 1 )
   ENDIF
   IF Right( cString, 2 ) == Chr( 13 ) + Chr( 10 )
      cString := hb_StrShrink( cString, Len( Chr( 13 ) + Chr( 10 ) ) )
   ENDIF

   FOR EACH tmp IN hb_ATokens( StrTran( cString, Chr( 13 ) ), Chr( 10 ) )
      AAdd( aLine, tmp )
   NEXT

   RETURN aLine

STATIC FUNCTION GetList( cString )
   LOCAL aList := {}
   LOCAL tmp

   FOR EACH tmp IN hb_ATokens( cString, "," )
      AAdd( aList, tmp )
   NEXT

   RETURN aList

STATIC FUNCTION MakeList( aList )
   LOCAL cString := ""
   LOCAL tmp

   FOR EACH tmp IN aList
      cString += tmp + ","
   NEXT

   RETURN hb_StrShrink( cString, 1 )
