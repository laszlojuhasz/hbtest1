/*
 * $Id$
 */

/*
 * Copyright 2009 Viktor Szakats (harbour.01 syenar.hu)
 * www - http://www.harbour-project.org
 */

#include "simpleio.ch"

PROCEDURE Main()

   ? ">" + TIP_TIMESTAMP() + "<"
   ? ">" + TIP_TIMESTAMP( NIL, 200 ) + "<"
   ? ">" + TIP_TIMESTAMP( Date() ) + "<"
   ? ">" + TIP_TIMESTAMP( Date(), 200 ) + "<"
   ? ">" + TIP_TIMESTAMP( hb_DateTime() ) + "<"
   ? ">" + TIP_TIMESTAMP( hb_DateTime(), 200 ) + "<"

   RETURN
