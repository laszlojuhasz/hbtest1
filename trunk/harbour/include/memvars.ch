/*
 * $Id$
 */

/*
 * Harbour Project source code:
 * Header file for memory variable management
 *
 * Copyright 1999 Ryszard Glab <rglab@imid.med.pl>
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

/* NOTE: This file is also used by C code. */

#ifndef _MEMVARS_CH
#define _MEMVARS_CH

/* Values returned from __MVSCOPE() function */
#define MV_NOT_FOUND      -2   /* not found in the symbols table */
#define MV_UNKNOWN        -1   /* not created yet */
#define MV_ERROR           0   /* information cannot be obtained */
#define MV_PUBLIC          1   /* PUBLIC variable */
#define MV_PRIVATE_GLOBAL  2   /* PRIVATE created outside of current function/procedure */
#define MV_PRIVATE_LOCAL   4   /* PRIVATE created in current function/procedure */
#define MV_PRIVATE         6   /* PRIVATE variable */

#endif /* _MEMVARS_CH */
