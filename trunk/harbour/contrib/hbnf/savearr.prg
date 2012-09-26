/*
 * $Id$
 */

/*
 * File......: savearr.prg
 * Author....: David Barrett
 * CIS ID....: 72037,105
 *
 * This is an original work by David Barrett and is placed in the
 * public domain.
 *
 * Modification history:
 * ---------------------
 *
 *    Rev 1.3   28 Sep 1992 22:04:18   GLENN
 * A few users have reported that these functions do not support
 * multi-dimensional arrays.  Until the bugs are verified and
 * workarounds or re-writes devised, a warning has been placed in the
 * documentation.
 *
 *    Rev 1.2   15 Aug 1991 23:06:06   GLENN
 * Forest Belt proofread/edited/cleaned up doc
 *
 *    Rev 1.1   14 Jun 1991 19:52:54   GLENN
 * Minor edit to file header
 *
 *    Rev 1.0   07 Jun 1991 23:39:38   GLENN
 * Initial revision.
 *
 *
 */

MEMVAR lRet

#ifdef FT_TEST              // test program to demonstrate functions

LOCAL  aArray := {;
      { 'Invoice 1', SToD( "19910415" ), 1234.32, .T. }, ;
      { 'Invoice 2', Date(), 234.98, .F. }, ;
      { 'Invoice 3', Date() + 1, 0, .T. }  }, aSave
LOCAL nErrorCode := 0

   FT_SAVEARR( aArray, 'invoice.dat', @nErrorCode )
   IF nErrorCode == 0
      CLS
      DispArray( aArray )
      aSave := FT_RESTARR( 'invoice.dat', @nErrorCode )
      IF nErrorCode == 0
         DispArray( aSave )
      ELSE
         ? 'Error restoring array'
      ENDIF
   ELSE
      ? 'Error writing array'
   ENDIF

   RETURN

FUNCTION DispArray( aTest )

   LOCAL nk

   FOR nk := 1 TO Len( aTest )
      ? aTest[ nk, 1 ]
      ?? '  '
      ?? DToC( aTest[ nk, 2 ] )
      ?? '  '
      ?? Str( aTest[ nk, 3 ] )
      ?? '  '
      ?? iif( aTest[ nk, 4 ], 'true', 'false' )
   NEXT

   RETURN Nil

#endif

FUNCTION FT_SAVEARR( aArray, cFileName, nErrorCode )

   LOCAL nHandle, lRet

   nHandle := FCreate( cFileName )
   nErrorCode := FError()
   IF nErrorCode == 0
      lRet := _ftsavesub( aArray, nHandle, @nErrorCode )
      FClose( nHandle )
      IF ( lRet ) .AND. ( FError() != 0 )
         nErrorCode := FError()
         lRet := .F.
      ENDIF
   ELSE
      lRet := .F.
   ENDIF

   RETURN lRet

STATIC FUNCTION _ftsavesub( xMemVar, nHandle, nErrorCode )

   LOCAL cValType, nLen, cString
   PRIVATE lRet       // accessed in code block

   lRet := .T.
   cValType := ValType( xMemVar )
   FWrite( nHandle, cValType, 1 )
   IF FError() == 0
      DO CASE
      CASE cValType == "A"
         nLen := Len( xMemVar )
         FWrite( nHandle, L2Bin( nLen ), 4 )
         IF FError() == 0
            AEval( xMemVar, {|xMemVar1| lRet := _ftsavesub( xMemVar1, nHandle ) } )
         ELSE
            lRet := .F.
         ENDIF
      CASE cValType == "B"
         lRet := .F.
      CASE cValType == "C"
         nLen := Len( xMemVar )
         FWrite( nHandle, L2Bin( nLen ), 4 )
         FWrite( nHandle, xMemVar )
      CASE cValType == "D"
         nLen := 8
         FWrite( nHandle, L2Bin( nLen ), 4 )
         FWrite( nHandle, DToC( xMemVar ) )
      CASE cValType == "L"
         nLen := 1
         FWrite( nHandle, L2Bin( nLen ), 4 )
         FWrite( nHandle, iif( xMemVar, "T", "F" ) )
      CASE cValType == "N"
         cString := Str( xMemVar )
         nLen := Len( cString )
         FWrite( nHandle, L2Bin( nLen ), 4 )
         FWrite( nHandle, cString )
      ENDCASE
   ELSE
      lRet := .F.
   ENDIF
   nErrorCode := FError()

   RETURN lRet

FUNCTION FT_RESTARR( cFileName, nErrorCode )

   LOCAL nHandle, aArray

   nHandle := FOpen( cFileName )
   nErrorCode := FError()
   IF nErrorCode == 0
      aArray := _ftrestsub( nHandle, @nErrorCode )
      FClose( nHandle )
   ELSE
      aArray := {}
   ENDIF

   RETURN aArray

STATIC FUNCTION _ftrestsub( nHandle, nErrorCode )

   LOCAL cValType, nLen, cLenStr, xMemVar, cMemVar, nk

   cValType := ' '
   FRead( nHandle, @cValType, 1 )
   cLenStr := Space( 4 )
   FRead( nHandle, @cLenStr, 4 )
   nLen := Bin2L( cLenStr )
   nErrorCode := FError()
   IF nErrorCode == 0
      DO CASE
      CASE cValType == "A"
         xMemVar := {}
         FOR nk := 1 TO nLen
            AAdd( xMemVar, _ftrestsub( nHandle ) )      // Recursive call
         NEXT
      CASE cValType == "C"
         xMemVar := Space( nLen )
         FRead( nHandle, @xMemVar, nLen )
      CASE cValType == "D"
         cMemVar := Space( 8 )
         FRead( nHandle, @cMemVar, 8 )
         xMemVar := CToD( cMemVar )
      CASE cValType == "L"
         cMemVar := ' '
         FRead( nHandle, @cMemVar, 1 )
         xMemVar := ( cMemVar == "T" )
      CASE cValType == "N"
         cMemVar := Space( nLen )
         FRead( nHandle, @cMemVar, nLen )
         xMemVar := Val( cMemVar )
      ENDCASE
      nErrorCode := FError()
   ENDIF

   RETURN xMemVar
