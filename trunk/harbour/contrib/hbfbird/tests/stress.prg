/*
 * $Id$
 */

/* VERY IMPORTANT: Don't use this query as sample, they are used for stress tests !!! */

FUNCTION Main()

   LOCAL oServer, oQuery, oRow, i, x

   LOCAL cServer := "127.0.0.1:" + hb_dirBase() + "stress.gdb"
   LOCAL cUser := "sysdba"
   LOCAL cPass := "masterkey"
   LOCAL nDialect := 1
   LOCAL cQuery

   CLEAR SCREEN

   IF ! hb_FileExists( hb_dirBase() + "stress.gdb" )
      ? FBCreateDB( hb_dirBase() + "stress.gdb", cuser, cpass, 1024, "WIN1251", nDialect )
   ENDIF

   ? "Connecting..."

   oServer := TFBServer():New( cServer, cUser, cPass, nDialect )

   IF oServer:NetErr()
      ? oServer:Error()
      QUIT
   ENDIF

   IF oServer:TableExists("test")
      ? oServer:Execute("DROP TABLE Test")
      ? oServer:Execute("DROP DOMAIN boolean_field")
   ENDIF

   ? "Creating domain for boolean fields..."

   ? oServer:Execute("create domain boolean_field as smallint default 0 not null check (value in (0,1))")

   ? "Creating test table..."
   cQuery := "CREATE TABLE test("
   cQuery += "     Code SmallInt not null primary key, "
   cQuery += "     dept Integer, "
   cQuery += "     Name Varchar(40), "
   cQuery += "     Sales boolean_field, "
   cQuery += "     Tax Float, "
   cQuery += "     Salary Double Precision, "
   cQuery += "     Budget Numeric(12,2), "
   cQuery += "     Discount Decimal(5,2), "
   cQuery += "     Creation Date, "
   cQuery += "     Description blob sub_type 1 segment size 40 ) "

   ? oServer:Execute(cQuery)

   oQuery := oServer:Query("SELECT code, dept, name, sales, salary, creation FROM test")

   oServer:StartTransaction()

   FOR i := 1 TO 10000
      @ 15,0 say "Inserting values...." + str(i)

      oRow := oQuery:Blank()

      oRow:Fieldput(1, i)
      oRow:Fieldput(2, i+1)
      oRow:Fieldput(3, "DEPARTMENT NAME " + strzero(i) )
      oRow:Fieldput(4, (mod(i,10) == 0) )
      oRow:Fieldput(5, 3000 + i )
      oRow:fieldput(6, Date() )

      oServer:Append(oRow)

      IF mod(i,100) == 0
         oServer:Commit()
         oServer:StartTransaction()
      ENDIF
   NEXT

   FOR i := 5000 TO 7000
      @ 16,0 say "Deleting values...." + str(i)

      oRow := oQuery:Blank()
      oServer:Delete(oRow, "code = " + str(i))

      IF mod(i,100) == 0
         oServer:Commit()
         oServer:StartTransaction()
      ENDIF
   NEXT

   FOR i := 2000 TO 3000
      @ 17,0 say "Updating values...." + str(i)

      oRow := oQuery:Blank()
      oRow:Fieldput(5, 4000+i)
      oServer:update(oRow, "code = " + str(i))

      IF mod(i,100) == 0
         oServer:Commit()
         oServer:StartTransaction()
      ENDIF
   NEXT

   oQuery := oServer:Query("SELECT sum(salary) sum_salary FROM test WHERE code between 1 and 4000")

   IF ! oQuery:Neterr()
      oQuery:Fetch()
      @ 18,0 say "Sum values...." + Str(oQuery:Fieldget(1))
      oQuery:Destroy()
   ENDIF

   x := 0
   FOR i := 1 TO 4000
      oQuery := oServer:Query("SELECT * FROM test WHERE code = " + str(i))

      IF ! oQuery:Neterr()
         oQuery:Fetch()
         oRow := oQuery:getrow()

         oQuery:destroy()
         x += oRow:fieldget(oRow:fieldpos("salary"))

         @ 19,0 say "Sum values...." + str(x)
      ENDIF
   NEXT

   oServer:Destroy()

   ? "Closing..."

   RETURN NIL
