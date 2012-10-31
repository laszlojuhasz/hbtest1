/*
 * $Id$
 */

/*
 * Harbour Project source code:
 *
 * Copyright 2010 Viktor Szakats (harbour syenar.net)
 * www - http://harbour-project.org
 *
 */

#include "simpleio.ch"

PROCEDURE Main()

   ? hb_libName( NIL )
   ? hb_libName( "" )
   ? hb_libName( "name" )
   ? hb_libName( "name.ext" )
   ? hb_libName( "name." )
   ? hb_libName( "name.ext", NIL )
   ? hb_libName( "dir\name.ext", NIL )
   ? hb_libName( "name.ext", "mydir" )
   ? hb_libName( "dir\name.ext", "mydir" )
   ? hb_libName( "name.ext", "mydir\" )
   ? hb_libName( "dir\name.ext", "mydir\" )

   RETURN
