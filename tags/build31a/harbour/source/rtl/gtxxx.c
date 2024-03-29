/*
 * $Id$
 */

/*
 * Harbour Project source code:
 * Terminal low-level module includer for GNU compilers
 *
 * Copyright 1999 {list of individual authors and e-mail addresses}
 * www - http://www.harbour-project.org
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version, with one exception:
 *
 * The exception is that if you link the Harbour Runtime Library (HRL)
 * and/or the Harbour Virtual Machine (HVM) with other files to produce
 * an executable, this does not by itself cause the resulting executable
 * to be covered by the GNU General Public License. Your use of that
 * executable is in no way restricted on account of linking the HRL
 * and/or HVM code into it.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA (or visit
 * their web site at http://www.gnu.org/).
 *
 */

#include "hbsetup.h"

#if   defined(HARBOUR_USE_STD_GTAPI)
   #include "gt/gtstd.c"
#elif defined(HARBOUR_USE_DOS_GTAPI)
   #include "gt/gtdos.c"
#elif defined(HARBOUR_USE_OS2_GTAPI)
   #include "gt/gtos2.c"
#elif defined(HARBOUR_USE_WIN_GTAPI)
   #include "gt/gtwin.c"
#elif defined(HARBOUR_USE_CRS_GTAPI)
   #include "gt/gtcrs.c"
#elif defined(HARBOUR_USE_SLN_GTAPI)
   #include "gt/gtsln.c"
#else
   #include "gt/gtstd.c"
#endif
