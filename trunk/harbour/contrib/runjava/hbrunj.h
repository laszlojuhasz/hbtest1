/*
 * $Id$
 */

/*
 * Harbour Project source code:
 * Header file for the Cursor style constants
 *
 * Copyright 1999 Matteo Baccan <baccan@isanet.it>
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

#include <jni.h>
/* Header for class Harbour */

#ifndef HB_RUNJ_H_
#define HB_RUNJ_H_
#ifdef __cplusplus
extern "C" {
#endif
/*
 * Class:     Harbour
 * Method:    Run
 * Signature: ([B)J
 */
JNIEXPORT jlong JNICALL Java_Harbour_Run
  (JNIEnv *, jclass, jintArray);

#ifdef __cplusplus
}
#endif
#endif
