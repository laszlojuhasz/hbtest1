/*
 * $Id$
 */

#include "dbinfo.ch"
#include "error.ch"

#include "hbrddsql.ch"

REQUEST SDDMY, SQLMIX

ANNOUNCE RDDSYS

FIELD RESIDENTS

PROCEDURE Main()

   rddSetDefault( "SQLMIX" )

   AEval( rddList(), {| X | QOut( X ) } )

   IF rddInfo( RDDI_CONNECT, { "MYSQL", "localhost", "test", , "test" } ) == 0
      ? "Unable connect to the server"
      RETURN
   ENDIF

   CreateTable()

   ? "Let's browse table (press any key)"
   Inkey( 0 )
   dbUseArea( .T. , , "SELECT * FROM country", "country" )
   Browse()

   ? "Let's browse table ordered by resident count (press any key)"
   Inkey( 0 )
   INDEX ON RESIDENTS TAG residents TO country
   Browse()

   dbCloseAll()

   RETURN

STATIC PROC CreateTable()

   ? rddInfo( RDDI_EXECUTE, "DROP TABLE country" )
   ? rddInfo( RDDI_EXECUTE, "CREATE TABLE country (CODE char(3), NAME char(50), RESIDENTS int(11))" )
   ? rddInfo( RDDI_EXECUTE, "INSERT INTO country values ('LTU', 'Lithuania', 3369600), ('USA', 'United States of America', 305397000), ('POR', 'Portugal', 10617600), ('POL', 'Poland', 38115967), ('AUS', 'Australia', 21446187), ('FRA', 'France', 64473140), ('RUS', 'Russia', 141900000)" )

   RETURN
