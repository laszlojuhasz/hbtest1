/*
 * $Id$
 */

/* -------------------------------------------------------------------- */
/* WARNING: Automatically generated source file. DO NOT EDIT!           */
/*          Instead, edit corresponding .qth file,                      */
/*          or the generator tool itself, and run regenarate.           */
/* -------------------------------------------------------------------- */

/*
 * Harbour Project QT wrapper
 *
 * Copyright 2009-2010 Pritpal Bedi <bedipritpal@hotmail.com>
 * www - http://harbour-project.org
 *
 * For full copyright message and credits, see: CREDITS.txt
 *
 */


#include "hbclass.ch"


REQUEST __HBQTCORE


FUNCTION QTextStream( ... )
   RETURN HB_QTextStream():new( ... )

FUNCTION QTextStreamFromPointer( ... )
   RETURN HB_QTextStream():fromPointer( ... )


CREATE CLASS QTextStream INHERIT HbQtObjectHandler FUNCTION HB_QTextStream

   METHOD  new( ... )

   METHOD  padChar                       // (  )                                               -> oQChar
   METHOD  atEnd                         // (  )                                               -> lBool
   METHOD  autoDetectUnicode             // (  )                                               -> lBool
   METHOD  codec                         // (  )                                               -> oQTextCodec
   METHOD  device                        // (  )                                               -> oQIODevice
   METHOD  fieldAlignment                // (  )                                               -> nFieldAlignment
   METHOD  fieldWidth                    // (  )                                               -> nInt
   METHOD  flush                         // (  )                                               -> NIL
   METHOD  generateByteOrderMark         // (  )                                               -> lBool
   METHOD  integerBase                   // (  )                                               -> nInt
   METHOD  locale                        // (  )                                               -> oQLocale
   METHOD  numberFlags                   // (  )                                               -> nNumberFlags
   METHOD  pos                           // (  )                                               -> nQint64
   METHOD  read                          // ( nMaxlen )                                        -> cQString
   METHOD  readAll                       // (  )                                               -> cQString
   METHOD  readLine                      // ( nMaxlen )                                        -> cQString
   METHOD  realNumberNotation            // (  )                                               -> nRealNumberNotation
   METHOD  realNumberPrecision           // (  )                                               -> nInt
   METHOD  reset                         // (  )                                               -> NIL
   METHOD  resetStatus                   // (  )                                               -> NIL
   METHOD  seek                          // ( nPos )                                           -> lBool
   METHOD  setAutoDetectUnicode          // ( lEnabled )                                       -> NIL
   METHOD  setCodec                      // ( oQTextCodec )                                    -> NIL
                                         // ( cCodecName )                                     -> NIL
   METHOD  setDevice                     // ( oQIODevice )                                     -> NIL
   METHOD  setFieldAlignment             // ( nMode )                                          -> NIL
   METHOD  setFieldWidth                 // ( nWidth )                                         -> NIL
   METHOD  setGenerateByteOrderMark      // ( lGenerate )                                      -> NIL
   METHOD  setIntegerBase                // ( nBase )                                          -> NIL
   METHOD  setLocale                     // ( oQLocale )                                       -> NIL
   METHOD  setNumberFlags                // ( nFlags )                                         -> NIL
   METHOD  setPadChar                    // ( oQChar )                                         -> NIL
   METHOD  setRealNumberNotation         // ( nNotation )                                      -> NIL
   METHOD  setRealNumberPrecision        // ( nPrecision )                                     -> NIL
   METHOD  setStatus                     // ( nStatus )                                        -> NIL
   METHOD  skipWhiteSpace                // (  )                                               -> NIL
   METHOD  status                        // (  )                                               -> nStatus

   ENDCLASS


METHOD QTextStream:new( ... )
   LOCAL p
   FOR EACH p IN { ... }
      hb_pvalue( p:__enumIndex(), __hbqt_ptr( p ) )
   NEXT
   ::pPtr := Qt_QTextStream( ... )
   RETURN Self


METHOD QTextStream:padChar( ... )
   SWITCH PCount()
   CASE 0
      RETURN QCharFromPointer( Qt_QTextStream_padChar( ::pPtr, ... ) )
   ENDSWITCH
   RETURN __hbqt_error()


METHOD QTextStream:atEnd( ... )
   SWITCH PCount()
   CASE 0
      RETURN Qt_QTextStream_atEnd( ::pPtr, ... )
   ENDSWITCH
   RETURN __hbqt_error()


METHOD QTextStream:autoDetectUnicode( ... )
   SWITCH PCount()
   CASE 0
      RETURN Qt_QTextStream_autoDetectUnicode( ::pPtr, ... )
   ENDSWITCH
   RETURN __hbqt_error()


METHOD QTextStream:codec( ... )
   SWITCH PCount()
   CASE 0
      RETURN QTextCodecFromPointer( Qt_QTextStream_codec( ::pPtr, ... ) )
   ENDSWITCH
   RETURN __hbqt_error()


METHOD QTextStream:device( ... )
   SWITCH PCount()
   CASE 0
      RETURN QIODeviceFromPointer( Qt_QTextStream_device( ::pPtr, ... ) )
   ENDSWITCH
   RETURN __hbqt_error()


METHOD QTextStream:fieldAlignment( ... )
   SWITCH PCount()
   CASE 0
      RETURN Qt_QTextStream_fieldAlignment( ::pPtr, ... )
   ENDSWITCH
   RETURN __hbqt_error()


METHOD QTextStream:fieldWidth( ... )
   SWITCH PCount()
   CASE 0
      RETURN Qt_QTextStream_fieldWidth( ::pPtr, ... )
   ENDSWITCH
   RETURN __hbqt_error()


METHOD QTextStream:flush( ... )
   SWITCH PCount()
   CASE 0
      RETURN Qt_QTextStream_flush( ::pPtr, ... )
   ENDSWITCH
   RETURN __hbqt_error()


METHOD QTextStream:generateByteOrderMark( ... )
   SWITCH PCount()
   CASE 0
      RETURN Qt_QTextStream_generateByteOrderMark( ::pPtr, ... )
   ENDSWITCH
   RETURN __hbqt_error()


METHOD QTextStream:integerBase( ... )
   SWITCH PCount()
   CASE 0
      RETURN Qt_QTextStream_integerBase( ::pPtr, ... )
   ENDSWITCH
   RETURN __hbqt_error()


METHOD QTextStream:locale( ... )
   SWITCH PCount()
   CASE 0
      RETURN QLocaleFromPointer( Qt_QTextStream_locale( ::pPtr, ... ) )
   ENDSWITCH
   RETURN __hbqt_error()


METHOD QTextStream:numberFlags( ... )
   SWITCH PCount()
   CASE 0
      RETURN Qt_QTextStream_numberFlags( ::pPtr, ... )
   ENDSWITCH
   RETURN __hbqt_error()


METHOD QTextStream:pos( ... )
   SWITCH PCount()
   CASE 0
      RETURN Qt_QTextStream_pos( ::pPtr, ... )
   ENDSWITCH
   RETURN __hbqt_error()


METHOD QTextStream:read( ... )
   SWITCH PCount()
   CASE 1
      DO CASE
      CASE hb_isNumeric( hb_pvalue( 1 ) )
         RETURN Qt_QTextStream_read( ::pPtr, ... )
      ENDCASE
      EXIT
   ENDSWITCH
   RETURN __hbqt_error()


METHOD QTextStream:readAll( ... )
   SWITCH PCount()
   CASE 0
      RETURN Qt_QTextStream_readAll( ::pPtr, ... )
   ENDSWITCH
   RETURN __hbqt_error()


METHOD QTextStream:readLine( ... )
   SWITCH PCount()
   CASE 1
      DO CASE
      CASE hb_isNumeric( hb_pvalue( 1 ) )
         RETURN Qt_QTextStream_readLine( ::pPtr, ... )
      ENDCASE
      EXIT
   CASE 0
      RETURN Qt_QTextStream_readLine( ::pPtr, ... )
   ENDSWITCH
   RETURN __hbqt_error()


METHOD QTextStream:realNumberNotation( ... )
   SWITCH PCount()
   CASE 0
      RETURN Qt_QTextStream_realNumberNotation( ::pPtr, ... )
   ENDSWITCH
   RETURN __hbqt_error()


METHOD QTextStream:realNumberPrecision( ... )
   SWITCH PCount()
   CASE 0
      RETURN Qt_QTextStream_realNumberPrecision( ::pPtr, ... )
   ENDSWITCH
   RETURN __hbqt_error()


METHOD QTextStream:reset( ... )
   SWITCH PCount()
   CASE 0
      RETURN Qt_QTextStream_reset( ::pPtr, ... )
   ENDSWITCH
   RETURN __hbqt_error()


METHOD QTextStream:resetStatus( ... )
   SWITCH PCount()
   CASE 0
      RETURN Qt_QTextStream_resetStatus( ::pPtr, ... )
   ENDSWITCH
   RETURN __hbqt_error()


METHOD QTextStream:seek( ... )
   SWITCH PCount()
   CASE 1
      DO CASE
      CASE hb_isNumeric( hb_pvalue( 1 ) )
         RETURN Qt_QTextStream_seek( ::pPtr, ... )
      ENDCASE
      EXIT
   ENDSWITCH
   RETURN __hbqt_error()


METHOD QTextStream:setAutoDetectUnicode( ... )
   SWITCH PCount()
   CASE 1
      DO CASE
      CASE hb_isLogical( hb_pvalue( 1 ) )
         RETURN Qt_QTextStream_setAutoDetectUnicode( ::pPtr, ... )
      ENDCASE
      EXIT
   ENDSWITCH
   RETURN __hbqt_error()


METHOD QTextStream:setCodec( ... )
   SWITCH PCount()
   CASE 1
      DO CASE
      CASE hb_isChar( hb_pvalue( 1 ) )
         RETURN Qt_QTextStream_setCodec_1( ::pPtr, ... )
      CASE hb_isObject( hb_pvalue( 1 ) )
         RETURN Qt_QTextStream_setCodec( ::pPtr, ... )
      ENDCASE
      EXIT
   ENDSWITCH
   RETURN __hbqt_error()


METHOD QTextStream:setDevice( ... )
   SWITCH PCount()
   CASE 1
      DO CASE
      CASE hb_isObject( hb_pvalue( 1 ) )
         RETURN Qt_QTextStream_setDevice( ::pPtr, ... )
      ENDCASE
      EXIT
   ENDSWITCH
   RETURN __hbqt_error()


METHOD QTextStream:setFieldAlignment( ... )
   SWITCH PCount()
   CASE 1
      DO CASE
      CASE hb_isNumeric( hb_pvalue( 1 ) )
         RETURN Qt_QTextStream_setFieldAlignment( ::pPtr, ... )
      ENDCASE
      EXIT
   ENDSWITCH
   RETURN __hbqt_error()


METHOD QTextStream:setFieldWidth( ... )
   SWITCH PCount()
   CASE 1
      DO CASE
      CASE hb_isNumeric( hb_pvalue( 1 ) )
         RETURN Qt_QTextStream_setFieldWidth( ::pPtr, ... )
      ENDCASE
      EXIT
   ENDSWITCH
   RETURN __hbqt_error()


METHOD QTextStream:setGenerateByteOrderMark( ... )
   SWITCH PCount()
   CASE 1
      DO CASE
      CASE hb_isLogical( hb_pvalue( 1 ) )
         RETURN Qt_QTextStream_setGenerateByteOrderMark( ::pPtr, ... )
      ENDCASE
      EXIT
   ENDSWITCH
   RETURN __hbqt_error()


METHOD QTextStream:setIntegerBase( ... )
   SWITCH PCount()
   CASE 1
      DO CASE
      CASE hb_isNumeric( hb_pvalue( 1 ) )
         RETURN Qt_QTextStream_setIntegerBase( ::pPtr, ... )
      ENDCASE
      EXIT
   ENDSWITCH
   RETURN __hbqt_error()


METHOD QTextStream:setLocale( ... )
   SWITCH PCount()
   CASE 1
      DO CASE
      CASE hb_isObject( hb_pvalue( 1 ) )
         RETURN Qt_QTextStream_setLocale( ::pPtr, ... )
      ENDCASE
      EXIT
   ENDSWITCH
   RETURN __hbqt_error()


METHOD QTextStream:setNumberFlags( ... )
   SWITCH PCount()
   CASE 1
      DO CASE
      CASE hb_isNumeric( hb_pvalue( 1 ) )
         RETURN Qt_QTextStream_setNumberFlags( ::pPtr, ... )
      ENDCASE
      EXIT
   ENDSWITCH
   RETURN __hbqt_error()


METHOD QTextStream:setPadChar( ... )
   SWITCH PCount()
   CASE 1
      DO CASE
      CASE hb_isObject( hb_pvalue( 1 ) )
         RETURN Qt_QTextStream_setPadChar( ::pPtr, ... )
      ENDCASE
      EXIT
   ENDSWITCH
   RETURN __hbqt_error()


METHOD QTextStream:setRealNumberNotation( ... )
   SWITCH PCount()
   CASE 1
      DO CASE
      CASE hb_isNumeric( hb_pvalue( 1 ) )
         RETURN Qt_QTextStream_setRealNumberNotation( ::pPtr, ... )
      ENDCASE
      EXIT
   ENDSWITCH
   RETURN __hbqt_error()


METHOD QTextStream:setRealNumberPrecision( ... )
   SWITCH PCount()
   CASE 1
      DO CASE
      CASE hb_isNumeric( hb_pvalue( 1 ) )
         RETURN Qt_QTextStream_setRealNumberPrecision( ::pPtr, ... )
      ENDCASE
      EXIT
   ENDSWITCH
   RETURN __hbqt_error()


METHOD QTextStream:setStatus( ... )
   SWITCH PCount()
   CASE 1
      DO CASE
      CASE hb_isNumeric( hb_pvalue( 1 ) )
         RETURN Qt_QTextStream_setStatus( ::pPtr, ... )
      ENDCASE
      EXIT
   ENDSWITCH
   RETURN __hbqt_error()


METHOD QTextStream:skipWhiteSpace( ... )
   SWITCH PCount()
   CASE 0
      RETURN Qt_QTextStream_skipWhiteSpace( ::pPtr, ... )
   ENDSWITCH
   RETURN __hbqt_error()


METHOD QTextStream:status( ... )
   SWITCH PCount()
   CASE 0
      RETURN Qt_QTextStream_status( ::pPtr, ... )
   ENDSWITCH
   RETURN __hbqt_error()

