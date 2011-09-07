/*
 * $Id$
 */

FUNCTION main()

   LOCAL oError := ErrorNew()

   LOCAL a := { 3, 2, 1 }
   LOCAL b := { 10 }
   LOCAL c := { 2, .T. , "B", NIL, { 1 }, {|| b }, oError, Date(), 1, .F. , "A", NIL, Date() - 1, { 0 }, {|| a }, oError }
   LOCAL t

   ?
   ?
   ? "Original.....:", aDump( t := a )
   ? "Asort.c......:", aDump( ASort( t := AClone( a ) ) )
   ? "Asort.c.block:", aDump( ASort( t := AClone( a ), , , {| x, y | x < y } ) )
   ?
   ? "Original.....:", aDump( t := b )
   ? "Asort.c......:", aDump( ASort( t := AClone( b ) ) )
   ? "Asort.c.block:", aDump( ASort( t := AClone( b ), , , {| x, y | x < y } ) )
   ?
   ? "Original.....:", aDump( t := c )
   ? "Asort.c......:", aDump( ASort( t := AClone( c ) ) )
   ? "Asort.c.block:", aDump( ASort( t := AClone( c ), , , {| x, y | xToStr( x ) < xToStr( y ) } ) )

   RETURN nil

FUNCTION aDump( a )

   LOCAL cStr := ""
   LOCAL n := Len( a )
   LOCAL i

   FOR i := 1 TO n
      cStr += AllTrim( xToStr( a[ i ] ) ) + " "
   NEXT

   RETURN cStr

FUNCTION xToStr( xValue )

   LOCAL cType := ValType( xValue )

   DO CASE
   CASE cType == "C" .OR. cType == "M"
      RETURN xValue
   CASE cType == "N"
      RETURN AllTrim( Str( xValue ) )
   CASE cType == "D"
      RETURN DToC( xValue )
   CASE cType == "L"
      RETURN iif( xValue, ".T.", ".F." )
   CASE cType == "U"
      RETURN "NIL"
   CASE cType == "A"
      RETURN "{.}"
   CASE cType == "B"
      RETURN "{|| }"
   CASE cType == "O"
      RETURN "[O]"
   ENDCASE

   RETURN xValue
