/*
 * $Id$
 */

/*
 * Harbour Project source code:
 * Header file for the Get system
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

#ifndef _GETEXIT_CH
#define _GETEXIT_CH

/* get:exitState values */
#define GE_NOEXIT       0       /* no exit attempted (blank) */
#define GE_UP           1
#define GE_DOWN         2
#define GE_TOP          3
#define GE_BOTTOM       4
#define GE_ENTER        5
#define GE_WRITE        6
#define GE_ESCAPE       7
#define GE_WHEN         8       /* when clause unsatisfied */

#endif /* _GETEXIT_CH */
