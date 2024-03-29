/*
 * $Id$
 */

/*
 * Harbour Project source code:
 *    demonstration/test code for alternative RDD IO API which uses own
 *    very simple TCP/IP file server.
 *
 * Copyright 2009 Przemyslaw Czerpak <druzus / at / priv.onet.pl>
 * www - http://www.harbour-project.org
 *
 */

#define DBNAME    "net:127.0.0.1:2941:data/_tst_"

request DBFCDX

proc main()
   local pSockSrv

   set exclusive off
   rddSetDefault( "DBFCDX" )

   pSockSrv := netio_mtserver()
   if empty( pSockSrv )
      ? "Cannot start NETIO server !!!"
      wait "Press any key to exit..."
      quit
   endif

   ? "NETIO server activated."
   hb_idleSleep( 0.1 )
   wait

   ?
   ? "NETIO_CONNECT():", netio_connect()
   ?

   createdb( DBNAME )
   testdb( DBNAME )
   wait

   ?
   ? "table exists:", dbExists( DBNAME )
   wait

   ?
   ? "delete table with indexes:", dbDrop( DBNAME )
   ? "table exists:", dbExists( DBNAME )
   wait

   netio_serverstop( pSockSrv, .t. )
return

proc createdb( cName )
   local n

   dbCreate( cName, {{"F1", "C", 20, 0},;
                     {"F2", "M",  4, 0},;
                     {"F3", "N", 10, 2},;
                     {"F4", "T",  8, 0}} )
   ? "create neterr:", neterr(), hb_osError()
   use (cName)
   ? "use neterr:", neterr(), hb_osError()
   while lastrec() < 100
      dbAppend()
      n := recno() - 1
      field->F1 := chr( n % 26 + asc( "A" ) ) + " " + time()
      field->F2 := field->F1
      field->F3 := n / 100
      field->F4 := hb_dateTime()
   enddo
   index on field->F1 tag T1
   index on field->F3 tag T3
   index on field->F4 tag T4
   close
   ?
return

proc testdb( cName )
   local i, j
   use (cName)
   ? "used:", used()
   ? "nterr:", neterr()
   ? "alias:", alias()
   ? "lastrec:", lastrec()
   ? "ordCount:", ordCount()
   for i:=1 to ordCount()
      ordSetFocus( i )
      ? i, "name:", ordName(), "key:", ordKey(), "keycount:", ordKeyCount()
   next
   ordSetFocus( 1 )
   dbgotop()
   while !eof()
      if ! field->F1 == field->F2
         ? "error at record:", recno()
         ? "  ! '" + field->F1 + "' == '" + field->F2 + "'"
      endif
      dbSkip()
   enddo
   wait
   i := row()
   j := col()
   dbgotop()
   browse()
   setpos( i, j )
   close
return
