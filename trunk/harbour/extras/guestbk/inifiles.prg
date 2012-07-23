/*
 * $Id$
 */

FUNCTION TIniFile()

   STATIC oClass

   IF oClass == nil
      oClass := HBClass():New( "TINIFILE" ) // starts a new class definition

      oClass:AddData( "FileName" )           // define this class objects datas
      oClass:AddData( "Contents" )

      oClass:AddMethod( "New",  @New() )  // define this class objects methods
      oClass:AddMethod( "ReadString", @ReadString() )
      oClass:AddMethod( "WriteString", @WriteString() )
      oClass:AddMethod( "ReadNumber", @ReadNumber() )
      oClass:AddMethod( "WriteNumber", @WriteNumber() )
      oClass:AddMethod( "ReadDate", @ReadDate() )
      oClass:AddMethod( "WriteDate", @WriteDate() )
      oClass:AddMethod( "ReadBool", @ReadBool() )
      oClass:AddMethod( "WriteBool", @WriteBool() )
      oClass:AddMethod( "ReadSection", @ReadSection() )
      oClass:AddMethod( "ReadSections", @ReadSections() )
      oClass:AddMethod( "DeleteKey", @DeleteKey() )
      oClass:AddMethod( "EraseSection", @EraseSection() )
      oClass:AddMethod( "UpdateFile", @UpdateFile() )

      oClass:Create()                     // builds this class
   ENDIF

   RETURN oClass:Instance()                  // builds an object of this class

STATIC FUNCTION New( cFileName )

   LOCAL Self := QSelf()
   LOCAL Done, hFile, cFile, cLine, cIdent, nPos
   LOCAL CurrArray

   IF Empty( cFileName )
      // raise an error?
      OutErr( "No filename passed to TIniFile():New()" )
      RETURN nil

   ELSE
      ::FileName := cFilename
      ::Contents := {}
      CurrArray := ::Contents

      IF File( cFileName )
         hFile := FOpen( cFilename, 0 )
      ELSE
         hFile := FCreate( cFilename )
      ENDIF

      cLine := ""
      Done := .F.
      WHILE ! Done
         cFile := Space( 256 )
         Done := ( FRead( hFile, @cFile, 256 ) <= 0 )

         cFile := StrTran( cFile, Chr( 10 ), "" ) // so we can just search for CHR(13)

         // prepend last read
         cFile := cLine + cFile
         WHILE ! Empty( cFile )
            IF ( nPos := At( Chr(13 ), cFile ) ) > 0
               cLine := Left( cFile, nPos - 1 )
               cFile := SubStr( cFile, nPos + 1 )

               IF ! Empty( cLine )
                  IF Left( cLine, 1 ) == "[" // new section
                     IF ( nPos := At( "]", cLine ) ) > 1
                        cLine := SubStr( cLine, 2, nPos - 2 )
                     ELSE
                        cLine := SubStr( cLine, 2 )
                     ENDIF

                     AAdd( ::Contents, { cLine, { /* this will be CurrArray */ } } )
                     CurrArray := ::Contents[ Len( ::Contents ) ][ 2 ]

                  ELSEIF Left( cLine, 1 ) == ";" // preserve comments
                     AAdd( CurrArray, { NIL, cLine } )

                  ELSE
                     IF ( nPos := At( "=", cLine ) ) > 0
                        cIdent := Left( cLine, nPos - 1 )
                        cLine := SubStr( cLine, nPos + 1 )

                        AAdd( CurrArray, { cIdent, cLine } )

                     ELSE
                        AAdd( CurrArray, { cLine, "" } )
                     ENDIF
                  ENDIF
                  cLine := "" // to stop prepend later on
               ENDIF

            ELSE
               cLine := cFile
               cFile := ""
            ENDIF
         end
      end

      FClose( hFile )
   ENDIF

   RETURN Self

STATIC FUNCTION ReadString( cSection, cIdent, cDefault )

   LOCAL Self := QSelf()
   LOCAL cResult := cDefault
   LOCAL i, j, cFind

   IF Empty( cSection )
      cFind := Lower( cIdent )
      j := AScan( ::Contents, {| x | HB_ISSTRING( x[ 1 ] ) .AND. Lower( x[ 1 ] ) == cFind .AND. HB_ISSTRING( x[ 2 ] ) } )

      IF j > 0
         cResult := ::Contents[ j ][ 2 ]
      ENDIF

   ELSE
      cFind := Lower( cSection )
      i := AScan( ::Contents, {| x | HB_ISSTRING( x[ 1 ] ) .AND. Lower( x[ 1 ] ) == cFind } )

      IF i > 0
         cFind := Lower( cIdent )
         j := AScan( ::Contents[ i ][ 2 ], {| x | HB_ISSTRING( x[ 1 ] ) .AND. Lower( x[ 1 ] ) == cFind } )

         IF j > 0
            cResult := ::Contents[ i ][ 2 ][ j ][ 2 ]
         ENDIF
      ENDIF
   ENDIF

   RETURN cResult

STATIC PROCEDURE WriteString( cSection, cIdent, cString )

   LOCAL Self := QSelf()
   LOCAL i, j, cFind

   IF Empty( cIdent )
      OutErr( "Must specify an identifier" )

   ELSEIF Empty( cSection )
      cFind := Lower( cIdent )
      j := AScan( ::Contents, {| x | HB_ISSTRING( x[ 1 ] ) .AND. Lower( x[ 1 ] ) == cFind .AND. HB_ISSTRING( x[ 2 ] ) } )

      IF j > 0
         ::Contents[ j ][ 2 ] := cString
      ELSE
         AAdd( ::Contents, nil )
         AIns( ::Contents, 1 )
         ::Contents[ 1 ] := { cIdent, cString }
      ENDIF

   ELSE
      cFind := Lower( cSection )
      IF ( i := AScan( ::Contents, {| x | HB_ISSTRING(x[ 1 ] ) .AND. Lower(x[ 1 ] ) == cFind .AND. HB_ISARRAY(x[ 2 ] ) } ) ) > 0
         cFind := Lower( cIdent )
         j := AScan( ::Contents[ i ][ 2 ], {| x | HB_ISSTRING( x[ 1 ] ) .AND. Lower( x[ 1 ] ) == cFind } )

         IF j > 0
            ::Contents[ i ][ 2 ][ j ][ 2 ] := cString
         ELSE
            AAdd( ::Contents[ i ][ 2 ], { cIdent, cString } )
         ENDIF

      ELSE
         AAdd( ::Contents, { cSection, { {cIdent, cString} } } )
      ENDIF
   ENDIF

   RETURN

STATIC FUNCTION ReadNumber( cSection, cIdent, nDefault )

   LOCAL Self := QSelf()

   RETURN Val( ::ReadString( cSection, cIdent, Str(nDefault ) ) )

STATIC PROCEDURE WriteNumber( cSection, cIdent, nNumber )

   LOCAL Self := QSelf()

   ::WriteString( cSection, cIdent, hb_ntos( nNumber ) )

   RETURN

STATIC FUNCTION ReadDate( cSection, cIdent, dDefault )

   LOCAL Self := QSelf()

   RETURN SToD( ::ReadString( cSection, cIdent, DToS(dDefault ) ) )

STATIC PROCEDURE WriteDate( cSection, cIdent, dDate )

   LOCAL Self := QSelf()

   ::WriteString( cSection, cIdent, DToS( dDate ) )

   RETURN

STATIC FUNCTION ReadBool( cSection, cIdent, lDefault )

   LOCAL Self := QSelf()
   LOCAL cDefault := iif( lDefault, ".t.", ".f." )

   RETURN ::ReadString( cSection, cIdent, cDefault ) == ".t."

STATIC PROCEDURE WriteBool( cSection, cIdent, lBool )

   LOCAL Self := QSelf()

   ::WriteString( cSection, cIdent, iif( lBool, ".t.", ".f." ) )

   RETURN

STATIC PROCEDURE DeleteKey( cSection, cIdent )

   LOCAL Self := QSelf()
   LOCAL i, j

   cSection := Lower( cSection )
   i := AScan( ::Contents, {| x | HB_ISSTRING( x[ 1 ] ) .AND. Lower( x[ 1 ] ) == cSection } )

   IF i > 0
      cIdent := Lower( cIdent )
      j := AScan( ::Contents[ i ][ 2 ], {| x | HB_ISSTRING( x[ 1 ] ) .AND. Lower( x[ 1 ] ) == cIdent } )

      ADel( ::Contents[ i ][ 2 ], j )
      ASize( ::Contents[ i ][ 2 ], Len( ::Contents[ i ][ 2 ] ) - 1 )
   ENDIF

   RETURN

STATIC PROCEDURE EraseSection( cSection )

   LOCAL Self := QSelf()
   LOCAL i

   IF Empty( cSection )
      WHILE ( i := AScan( ::Contents, {| x | HB_ISSTRING(x[ 1 ] ) .AND. HB_ISSTRING(x[ 2 ] ) } ) ) > 0
         ADel( ::Contents, i )
         ASize( ::Contents, Len( ::Contents ) - 1 )
      end

   ELSE
      cSection := Lower( cSection )
      IF ( i := AScan( ::Contents, {| x | HB_ISSTRING(x[ 1 ] ) .AND. Lower(x[ 1 ] ) == cSection .AND. HB_ISARRAY(x[ 2 ] ) } ) ) > 0
         ADel( ::Contents, i )
         ASize( ::Contents, Len( ::Contents ) - 1 )
      ENDIF
   ENDIF

   RETURN

STATIC FUNCTION ReadSection( cSection )

   LOCAL Self := QSelf()
   LOCAL i, j, aSection := {}

   IF Empty( cSection )
      FOR i := 1 TO Len( ::Contents )
         IF HB_ISSTRING( ::Contents[ i ][ 1 ] ) .AND. HB_ISSTRING( ::Contents[ i ][ 2 ] )
            AAdd( aSection, ::Contents[ i ][ 1 ] )
         ENDIF
      NEXT

   ELSE
      cSection := Lower( cSection )
      IF ( i := AScan( ::Contents, {| x | HB_ISSTRING(x[ 1 ] ) .AND. x[ 1 ] == cSection .AND. HB_ISARRAY(x[ 2 ] ) } ) ) > 0

         FOR j := 1 TO Len( ::Contents[ i ][ 2 ] )

            IF ::Contents[ i ][ 2 ][ j ][ 1 ] != NIL
               AAdd( aSection, ::Contents[ i ][ 2 ][ j ][ 1 ] )
            ENDIF
         NEXT
      ENDIF
   ENDIF

   RETURN aSection

STATIC FUNCTION ReadSections()

   LOCAL Self := QSelf()
   LOCAL i, aSections := {}

   FOR i := 1 TO Len( ::Contents )

      IF HB_ISARRAY( ::Contents[ i ][ 2 ] )
         AAdd( aSections, ::Contents[ i ][ 1 ] )
      ENDIF
   NEXT

   RETURN aSections

STATIC PROCEDURE UpdateFile()

   LOCAL Self := QSelf()
   LOCAL i, j, hFile

   hFile := FCreate( ::Filename )

   FOR i := 1 TO Len( ::Contents )
      if ::Contents[ i ][ 1 ] == NIL
         FWrite( hFile, ::Contents[ i ][ 2 ] + Chr( 13 ) + Chr( 10 ) )

      ELSEIF HB_ISARRAY( ::Contents[ i ][ 2 ] )
         FWrite( hFile, "[" + ::Contents[ i ][ 1 ] + "]" + Chr( 13 ) + Chr( 10 ) )
         FOR j := 1 TO Len( ::Contents[ i ][ 2 ] )

            if ::Contents[ i ][ 2 ][ j ][ 1 ] == NIL
               FWrite( hFile, ::Contents[ i ][ 2 ][ j ][ 2 ] + Chr( 13 ) + Chr( 10 ) )
            ELSE
               FWrite( hFile, ::Contents[ i ][ 2 ][ j ][ 1 ] + "=" + ::Contents[ i ][ 2 ][ j ][ 2 ] + Chr( 13 ) + Chr( 10 ) )
            ENDIF
         NEXT
         FWrite( hFile, Chr( 13 ) + Chr( 10 ) )

      ELSEIF HB_ISSTRING( ::Contents[ i ][ 2 ] )
         FWrite( hFile, ::Contents[ i ][ 1 ] + "=" + ::Contents[ i ][ 2 ] + Chr( 13 ) + Chr( 10 ) )

      ENDIF
   NEXT
   FClose( hFile )

   RETURN
