/*
 * $Id$
 */

/*
 * Harbour Project source code:
 * Telepathy emulation library
 *
 * Copyright 2000, 2001 Dan Levitt <dan@boba-fett.net>
 * Copyright 2004, 2005 Maurilio Longo <maurilio.longo@libero.it>
 * www - http://www.xharbour.org
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

/*
   This is based upon a library originally made by Dan Levitt <dan@boba-fett.net>
   (see README and ChangeLog). The original files have been committed as v1.0,
   so you can always retrieve them (see CVS docs on how to)
*/


#include "common.ch"
#include "fileio.ch"
#include "telepath.ch"


#define TPFP_NAME          1               /* Structure of ports array */
#define TPFP_HANDLE        2
#define TPFP_BAUD          3
#define TPFP_DBITS         4
#define TPFP_PARITY        5
#define TPFP_SBITS         6
#define TPFP_OC            7               /* Open/Close Flag */
#define TPFP_INBUF         8
#define TPFP_INBUF_SIZE    9               /* Size of input buffer */


THREAD STATIC t_aPorts               // Array with port info
THREAD STATIC t_nErrorCode := 0      // Error code from last operation, 0 if no error


FUNCTION tp_baud( nPort, nNewBaud )

   DEFAULT nNewBaud TO 0

   IF ! isport( nPort ) .OR. Empty( t_aPorts[ nPort, TPFP_NAME ] )
      RETURN TE_NOPORT
   ENDIF

   IF ! isopenport( nPort )
      RETURN 0
   ENDIF

   IF nNewBaud > 0
      IF __tp_InitPortSpeed( t_aPorts[ nPort, TPFP_HANDLE ] ,;
                             nNewBaud,;
                             t_aPorts[ nPort, TPFP_DBITS  ] ,;
                             t_aPorts[ nPort, TPFP_PARITY ] ,;
                             t_aPorts[ nPort, TPFP_SBITS  ] ) == 0

         t_aPorts[ nPort, TPFP_BAUD ] := nNewBaud
      ELSE
         // set error code
      ENDIF
   ENDIF

   RETURN t_aPorts[ nPort, TPFP_BAUD ]


FUNCTION tp_inkey( ... )
   RETURN inkey( ... )

FUNCTION tp_idle( lNewval )
   IF lNewval == .T.
      RETURN .T.
   ENDIF
   RETURN .F.

PROCEDURE tp_delay( nTime )

   DEFAULT nTime TO 0

   IF nTime < 0
      RETURN
   ELSEIF nTime > 1800
      nTime := 1800
   ENDIF

   hb_idleSleep( nTime )

   RETURN

FUNCTION tp_close( nPort, nTimeout )

   DEFAULT nTimeout TO 0

   /* Clipper returns 0 even if a port is not open */
   IF ! isopenport( nPort )
      RETURN 0
   ENDIF

   IF nTimeout > 0
      tp_flush( nPort, nTimeout )
   ENDIF

   IF t_aPorts[ nPort, TPFP_HANDLE ] >= 0

      FClose( t_aPorts[ nPort, TPFP_HANDLE ] )

      /* Port parameters should stay the same for the case the port
         gets reopened
      */
      t_aPorts[ nPort, TPFP_OC ] := .F.
      t_aPorts[ nPort, TPFP_INBUF ] := ""
      t_aPorts[ nPort, TPFP_HANDLE ] := -1
   ENDIF

   RETURN 0

FUNCTION tp_reopen( nPort, nInSize, nOutSize )

   LOCAL nBaud, nData, cParity, nStop, cPortName

   IF ! isport( nPort ) .OR. Empty( t_aPorts[ nPort, TPFP_NAME ] )
      RETURN TE_NOPORT
   ENDIF

   cPortname   := t_aPorts[ nPort, TPFP_NAME ]
   nBaud       := t_aPorts[ nPort, TPFP_BAUD ]
   nData       := t_aPorts[ nPort, TPFP_DBITS ]
   cParity     := t_aPorts[ nPort, TPFP_PARITY ]
   nStop       := t_aPorts[ nPort, TPFP_SBITS ]

   RETURN tp_open( nPort, nInSize, nOutSize, nBaud, nData, cParity, nStop, cPortName )

FUNCTION tp_open( nPort, nInSize, nOutSize, nBaud, nData, cParity, nStop, cPortname )

   LOCAL nRes

   #if defined( __PLATFORM__UNIX )
   LOCAL lPortExist
   LOCAL nFileCase
   LOCAL nDirCase
   #endif

   DEFAULT nInSize  TO 1536
   DEFAULT nOutSize TO 1536
   DEFAULT nBaud    TO 1200
   DEFAULT nData    TO 8
   DEFAULT cParity  TO "N"
   DEFAULT nStop    TO 1

   /* Serial ports name are made up of cPortName + nPort if nPort is not NIL */
   #if defined( __PLATFORM__UNIX )
      DEFAULT cPortName TO "/dev/ttyS" + iif( ISNUMBER( nPort ), hb_ntos( nPort - 1 ), "" )
   #else
      IF hb_osIsWinNT()
         DEFAULT cPortName TO "\\.\COM"   + iif( ISNUMBER( nPort ), hb_ntos( nPort ), "" )
      ELSE
         DEFAULT cPortName TO "COM"       + iif( ISNUMBER( nPort ), hb_ntos( nPort ), "" )
      ENDIF
   #endif

   #if defined( __PLATFORM__UNIX )
      nFileCase := Set( _SET_FILECASE, 0 )
      nDirCase := Set( _SET_DIRCASE, 0 )

      lPortExist := File( cPortname ) /* Must be File(), hb_FileExists() won't work */

      Set( _SET_FILECASE, nFileCase )
      Set( _SET_DIRCASE, nDirCase )

      IF ! lPortExist
         RETURN TE_NOPORT
      ENDIF
   #endif

   IF ! isport( nPort )
      RETURN TE_NOPORT
   ENDIF

   t_aPorts[ nPort, TPFP_NAME       ] := cPortname
   t_aPorts[ nPort, TPFP_BAUD       ] := nBaud
   t_aPorts[ nPort, TPFP_DBITS      ] := nData
   t_aPorts[ nPort, TPFP_PARITY     ] := cParity
   t_aPorts[ nPort, TPFP_SBITS      ] := nStop
   t_aPorts[ nPort, TPFP_OC         ] := .F.
   t_aPorts[ nPort, TPFP_INBUF      ] := ""
   t_aPorts[ nPort, TPFP_INBUF_SIZE ] := nInSize

   // Maybe we should have a __tp_Open() on every platform
   #if defined( __PLATFORM__UNIX )
      t_aPorts[ nPort, TPFP_HANDLE ] := __tp_Open( cPortname )
   #else
      t_aPorts[ nPort, TPFP_HANDLE ] := FOpen( cPortname, FO_READWRITE ) // FO_EXCLUSIVE
   #endif

   IF t_aPorts[ nPort, TPFP_HANDLE ] != F_ERROR

      /* low level C functions are prefixed __tp_ */
      IF ( nRes := __tp_InitPortSpeed( t_aPorts[ nPort, TPFP_HANDLE ] ,;
                                       t_aPorts[ nPort, TPFP_BAUD   ] ,;
                                       t_aPorts[ nPort, TPFP_DBITS  ] ,;
                                       t_aPorts[ nPort, TPFP_PARITY ] ,;
                                       t_aPorts[ nPort, TPFP_SBITS  ] ) ) == 0

         t_aPorts[ nPort, TPFP_OC ] := .T.
         RETURN nRes
      ELSE
         tp_Close( t_aPorts[ nPort, TPFP_HANDLE ] )
         RETURN nRes
      ENDIF
   ENDIF

   // set error code to a static var to have tp_error() work as expected
   //cnHandle := FError()

   t_aPorts[ nPort, TPFP_NAME       ] := ""
   t_aPorts[ nPort, TPFP_HANDLE     ] := -1
   t_aPorts[ nPort, TPFP_BAUD       ] := 1200
   t_aPorts[ nPort, TPFP_DBITS      ] := 8
   t_aPorts[ nPort, TPFP_PARITY     ] := "N"
   t_aPorts[ nPort, TPFP_SBITS      ] := 1
   t_aPorts[ nPort, TPFP_OC         ] := .F.
   t_aPorts[ nPort, TPFP_INBUF      ] := ""
   t_aPorts[ nPort, TPFP_INBUF_SIZE ] := 0

   RETURN TE_CONFL   // maybe should return something different?

FUNCTION tp_recv( nPort, nLength, nTimeout )

   LOCAL nDone
   LOCAL cRet

   DEFAULT nLength TO t_aPorts[ nPort, TPFP_INBUF_SIZE  ]
   DEFAULT nTimeout TO 0

   FetchChars( nPort )

   nDone := Seconds() + iif( nTimeout >= 0, nTimeout, 0 )

   DO WHILE Len( t_aPorts[ nPort, TPFP_INBUF ] ) < nLength .AND.;
            ( nTimeout < 0 .OR. Seconds() < nDone )

      IF ! tp_idle()
         FetchChars( nPort )
      ELSE
         EXIT
      ENDIF
   ENDDO

   IF nLength > Len( t_aPorts[ nPort, TPFP_INBUF ] )
      cRet := t_aPorts[ nPort, TPFP_INBUF ]
      t_aPorts[ nPort, TPFP_INBUF ] := ""
   ELSE
      cRet := SubStr( t_aPorts[ nPort, TPFP_INBUF ], 1, nLength )
      t_aPorts[ nPort, TPFP_INBUF ] := SubStr( t_aPorts[ nPort, TPFP_INBUF ], nLength + 1 )
   ENDIF

   RETURN cRet

FUNCTION tp_send( nPort, cString, nTimeout )

   LOCAL nWritten, nTotWritten, nDone

   DEFAULT cString TO ""
   DEFAULT nTimeout TO 0

   IF ! isopenport( nPort )
      RETURN 0
   ENDIF

   IF Len( cString ) == 0
      RETURN 0
   ENDIF

   nDone := Seconds() + iif( nTimeout >= 0, nTimeout, 0)
   nTotWritten := 0

   DO WHILE nTotWritten < Len( cString ) .AND. ;
         ( nTimeout < 0 .OR. Seconds() <= nDone )

      nWritten := __tp_WritePort( t_aPorts[ nPort, TPFP_HANDLE ], SubStr( cString, nTotWritten + 1 ) )

      IF nWritten >= 0

         nTotWritten += nWritten

         IF nTotWritten < Len( cString )

            IF ! tp_idle()
               hb_idleSleep( 1 )
            ELSE
               EXIT
            ENDIF
         ENDIF
      ELSE     // nWritten < 0, error occurred
         EXIT
      ENDIF
   ENDDO

   RETURN nTotWritten


FUNCTION tp_sendsub( nPort, cString, nStart, nLength, nTimeout )

   DEFAULT nStart TO 1
   DEFAULT nLength TO Len( cString )

   RETURN tp_send( nPort, SubStr( cString, nStart, nLength ), nTimeout )


FUNCTION tp_recvto( nPort, cDelim, nMaxlen, nTimeout )

   LOCAL cChar
   LOCAL nAt
   LOCAL nStartPos := 1, nFirst := 0
   LOCAL nDone, cRet := ""

   IF ! isopenport( nPort )
      RETURN ""
   ENDIF

   IF ! ISCHARACTER( cDelim ) .OR. Len( cDelim ) == 0
      RETURN ""
   ENDIF

   DEFAULT nMaxlen TO 64999      /* dos telepathy def. on xharbour could be higher */
   DEFAULT nTimeout TO 0

   FetchChars( nPort )

   /* Telepathy ng: [...] If nTimeout is omitted or zero, reads until finding the
                    delimiter or the input buffer is empty. */
   IF nTimeout == 0 .AND. Len( t_aPorts[ nPort, TPFP_INBUF ] ) == 0
      RETURN ""
   ENDIF

   nDone := Seconds() + iif( nTimeout >= 0, nTimeout, 0 )

   DO WHILE ( nTimeout < 0 .OR. Seconds() < nDone )

      IF Len( cDelim ) == 1

         nAt := hb_At( cDelim, t_aPorts[ nPort, TPFP_INBUF ], nStartPos )

         IF nAt > 0 .AND. iif( nFirst > 0, nAt < nFirst, .T. )
            nFirst := nAt
         ENDIF

      ELSE

         FOR EACH cChar IN cDelim

            nAt := hb_At( cChar, t_aPorts[ nPort, TPFP_INBUF ], nStartPos )

            IF nAt > 0 .AND. iif( nFirst > 0, nAt < nFirst, .T. )
               nFirst := nAt
            ENDIF

         NEXT

      ENDIF

      // I've found it
      IF nFirst > 0
         EXIT

      ELSE
         // Next loop I don't need to search that part of the input buffer that
         // I've already just searched for
         nStartPos := Max( Len( t_aPorts[ nPort, TPFP_INBUF ] ), 1 )

         // I've read more characters than I'm allowed to, so I exit
         IF nStartPos >= nMaxLen
            EXIT
         ENDIF

         IF ! tp_idle()
            FetchChars( nPort )
         ELSE
            EXIT
         ENDIF
      ENDIF

   ENDDO

   IF nFirst > 0
      cRet := Left( t_aPorts[ nPort, TPFP_INBUF ], nFirst )
      t_aPorts[ nPort, TPFP_INBUF ] := SubStr( t_aPorts[ nPort, TPFP_INBUF ], nFirst + 1 )
   ENDIF

   RETURN cRet

/*
    here's an improvement over original TP... you can "lookfor" a string
    here rather than just a char.  yay me.
    of course, if you're using clipper/tp code and you search for a single char it will work
    the same.
*/
FUNCTION tp_lookfor( nPort, cLookfor )

   IF ! isopenport( nPort )
      RETURN 0
   ENDIF

   FetchChars( nPort )

   RETURN At( cLookfor, t_aPorts[ nPort, TPFP_INBUF ] )

FUNCTION tp_inchrs( nPort )

   IF ! isopenport( nPort )
      RETURN 0
   ENDIF

   FetchChars( nPort )

   RETURN Len( t_aPorts[ nPort, TPFP_INBUF ] )

FUNCTION tp_infree( nPort )

   IF ! isopenport( nPort )
      RETURN 0
   ENDIF

   RETURN __tp_infree( t_aPorts[ nPort, TPFP_HANDLE ] )

FUNCTION tp_outfree( nPort )

   IF ! isopenport( nPort )
      RETURN 0
   ENDIF

   RETURN __tp_outfree( t_aPorts[ nPort, TPFP_HANDLE ] )

PROCEDURE tp_clearin( nPort )

   IF isopenport( nPort )
      FetchChars( nPort )
      t_aPorts[ nPort, TPFP_INBUF ] := ""
   ENDIF

   RETURN

PROCEDURE tp_clrkbd()

   CLEAR TYPEAHEAD

   RETURN

FUNCTION tp_crc16( cString )

   RETURN hb_byteSwapW( hb_crcct( cString ) )   /* swap lo and hi bytes */

FUNCTION tp_crc32( cString )

   RETURN hb_crc32( cString )


/*                   nPort, nTimeout, acList|cString..., lIgnorecase */
FUNCTION tp_waitfor( ... )

   LOCAL aParam := hb_AParams()
   LOCAL nPort//, nTimeout, lIgnorecase

   nPort := aParam[ 1 ]
   // nTimeout := aParam[ 2 ]
   // lIgnorecase := aParam[ Len( aParam ) ]

   IF ! isopenport( nPort )
      RETURN 0
   ENDIF

   // DEFAULT nTimeout TO -1
   // DEFAULT lIgnorecase TO .F.

   /*

   IF ntimeout < 0
      nDone := _clock() + 999999
   ELSEIF ntimeout == 0
      nDone := 4
   ELSE
      nDone := _clock() + nTimeout
   ENDIF

   DO WHILE ( nDone > _clock() .OR. nFirst == 100000 ) .AND. ! tp_idle()

      IF nFirst == 100000
         nFirst := 99999
      ENDIF

      FetchChars( nPort )

      FOR x := 1 TO Len( acList )
         IF lIgnorecase
            nAt := At( Upper( acList[ x ] ), Upper( t_aPorts[ nPort, TPFP_INBUF ] ) )
         ELSE
            nAt := At( acList[ x ] , t_aPorts[ nPort, TPFP_INBUF ] )
         ENDIF
         IF nAt > 0 .AND. nAt < nFirst
            nFirst := nAt
            nRet := x
         ENDIF
      NEXT

      IF nFirst < 64000
         EXIT
      ENDIF

#if 0
      sched_yield() // C level function
#endif

   ENDDO

   IF nFirst < 64000
      tp_recv( nPort, nAt + Len( acList[ nRet ] ))
      RETURN nRet
   ENDIF
   */

   RETURN 0

/* We cannot set, well, _I_ think we cannot, CTS without setting RTS flowcontrol, so this
   function and tp_ctrlrts() do the same thing, that is set/reset CRTSCTS flowcontol */
FUNCTION tp_ctrlcts( nPort, nNewCtrl )

   LOCAL nCurValue

   IF ! isopenport( nPort )
      RETURN 0
   ENDIF

   IF Valtype( nNewCtrl ) == "U"
      nCurValue := __tp_ctrlcts( t_aPorts[ nPort, TPFP_HANDLE ] )
   ELSE
      nCurValue := __tp_ctrlcts( t_aPorts[ nPort, TPFP_HANDLE ], nNewCtrl )
   ENDIF

   RETURN nCurValue


// Simply calls tp_ctrlcts()
FUNCTION tp_ctrlrts( nPort, nNewCtrl )
   RETURN tp_ctrlcts( nPort, nNewCtrl )



/*

// telepathy says...
// returns old dtr value 0,1,2
// sets to 0 = dtr off, 1 dtr on, 2 = dtr flow control autotoggle
// I don't support 2.  who uses dtr for flow control anyway...
FUNCTION tp_ctrldtr( nPort, nParamNewval )
    LOCAL nph, nnewval, noldval

   IF ! isopenport( nPort )
      RETURN TE_PARAM
   ENDIF
   nph := t_aPorts[ nPort, TPFP_HANDLE ]

   __tp_ctrldtr( nph, @nnewval, @noldval )

   RETURN noldval
*/

FUNCTION tp_isdcd( nPort )

   IF ! isopenport( nPort )
      RETURN .F.
   ENDIF

   RETURN __tp_isdcd( t_aPorts[ nPort, TPFP_HANDLE ] )

FUNCTION tp_isri( nPort )

   IF ! isopenport( nPort )
      RETURN .F.
   ENDIF

   RETURN __tp_isri( t_aPorts[ nPort, TPFP_HANDLE ] )

FUNCTION tp_isdsr( nPort )

   IF ! isopenport( nPort )
      RETURN .F.
   ENDIF

   RETURN __tp_isdsr( t_aPorts[ nPort, TPFP_HANDLE ] )

FUNCTION tp_iscts( nPort )

   IF ! isopenport( nPort )
      RETURN .F.
   ENDIF

   RETURN __tp_iscts( t_aPorts[ nPort, TPFP_HANDLE ] )

#if defined( __PLATFORM__UNIX )
// NB: On linux i don't know how to make a drain with a timeout, so here
//     I'll wait as long as it takes to drain the port.
FUNCTION tp_flush( nPort, nTimeout )

   //LOCAL nStart := Seconds()
   LOCAL nRes

   DEFAULT nTimeout TO 0

   IF ! isopenport( nPort )
      RETURN TE_CLOSED
   ENDIF

   nRes := __tp_drain( t_aPorts[ nPort, TPFP_HANDLE ] )

   // Sleep rest of timeout
   /*
   IF nTimeout > 0 .AND. Seconds() - nStart < nTimeout
      hb_idleSleep( nTimeout - ( Seconds() - nStart ) )
   ENDIF
   */

   // NB: returns timeout on error trying to reach compatibility with other platforms
   //     to be tested
   RETURN iif( nRes == 0, 0, TE_TMOUT )

#else

FUNCTION tp_flush( nPort, nTimeout )

   LOCAL nDone

   DEFAULT nTimeout TO -1

   IF ! isopenport( nPort )
      RETURN TE_CLOSED
   ENDIF

   IF nTimeout > 1800
      nTimeout := 1800
   ENDIF

   nDone := Seconds() + iif( nTimeout >= 0, nTimeout, 0 )

   DO WHILE tp_OutFree( nPort ) > 0 .AND. ;
         ( nTimeout < 0 .OR. Seconds() < nDone )
      hb_IdleState()
   ENDDO

   RETURN iif( tp_OutFree( nPort ) > 0, TE_TMOUT, 0 )
#endif

/*

/// sorry, but ctrldsr and ctrlcts will act like isdsr and iscts... if you want
/// flow control, talk to the system.
FUNCTION tp_ctrldsr( nPort )
   RETURN tp_isdsr( nPort )

/// you can't do these things.  try rc.serial
FUNCTION tp_shared
   RETURN 0

FUNCTION tp_setport
   RETURN 0

*/

// internal (static) functions ---------------------------------------------------

STATIC FUNCTION isopenport( nPort )

   IF ! isport( nPort )
      RETURN .F.
   ENDIF

   RETURN t_aPorts[ nPort, TPFP_OC ]

STATIC FUNCTION isport( nPort )

   IF ! ISNUMBER( nPort ) .OR. nPort < 1 .OR. nPort > TP_MAXPORTS
      RETURN .F.
   ENDIF

   RETURN .T.

STATIC FUNCTION FetchChars( nPort )

   LOCAL cStr

   IF ! isopenport( nPort )
      RETURN 0
   ENDIF

   cStr := __tp_ReadPort( t_aPorts[ nPort, TPFP_HANDLE ] )

   IF Len( cStr ) > 0
      t_aPorts[ nPort, TPFP_INBUF ] += cStr
   ENDIF

   RETURN Len( cStr )

INIT PROCEDURE _tpinit()

   LOCAL x

   IF t_aPorts == NIL
      t_aPorts := Array( TP_MAXPORTS )
      FOR x := 1 TO Len( t_aPorts )
         /// port name, file handle, baud, data bits, parity, stop bits, Open?, input buffer, input buff.size
         t_aPorts[ x ] := { "", -1, 1200, 8, "N", 1, .F., "", 0 }
      NEXT
   ENDIF

   RETURN

/*
/// you can uncomment the following section for compatability with TP code... I figured
/// you'd probably want them commented so it won't compile so that you would see where
/// you have potential incomplete port problems
///FUNCTION tp_mstat
///   RETURN ""
///
///FUNCTION tp_szmodem
///   RETURN 0
///
///FUNCTION tp_noteoff
///   RETURN 0
///
///FUNCTION tp_ontime
///   RETURN 0
///
///FUNCTION tp_rzmodem
///   RETURN 0
///
///FUNCTION tp_error
///   RETURN 0
///
///FUNCTION tp_errmsg
///   RETURN ""
///
///FUNCTION tp_fifo
///   RETURN 0
///
///
///FUNCTION tp_outchrs
///   RETURN 0
///
///FUNCTION tp_keybd
///   RETURN 0
///

/// tp_debug is not a real TP function.  I included it so you can define your own debug
/// output function.
/// the point of the first parameter is a "debug level".  I keep a system variable for how
/// much debuggning output is wanted and if the tp_debug parameter is a LOWER number than
/// the global debug level I print the message.  Since I don't have your system globals,
/// I will ignore the first parameter and always print it.
/// I recommend you modify this function to suit your own debugging needs
FUNCTION tp_debug( nDebugLevel, cString )
   ? cString
   RETURN NIL
*/

PROCEDURE tp_uninstall()
   /* NOTE: dummy function, solely for compatibility. */
   RETURN
