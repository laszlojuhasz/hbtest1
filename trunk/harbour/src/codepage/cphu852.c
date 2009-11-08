/*
 * $Id$
 */

/*
 * Harbour Project source code:
 * National Collation Support Module (HU852)
 *
 * Copyright 1999-2007 Viktor Szakats (harbour.01 syenar.hu)
 * www - http://www.harbour-project.org
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this software; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 59 Temple Place, Suite 330,
 * Boston, MA 02111-1307 USA (or visit the web site http://www.gnu.org/).
 *
 * As a special exception, the Harbour Project gives permission for
 * additional uses of the text contained in its release of Harbour.
 *
 * The exception is that, if you link the Harbour libraries with other
 * files to produce an executable, this does not by itself cause the
 * resulting executable to be covered by the GNU General Public License.
 * Your use of that executable is in no way restricted on account of
 * linking the Harbour library code into it.
 *
 * This exception does not however invalidate any other reasons why
 * the executable file might be covered by the GNU General Public License.
 *
 * This exception applies only to the code released by the Harbour
 * Project under the name Harbour.  If you copy code from other
 * Harbour Project or Free Software Foundation releases into a copy of
 * Harbour, as the General Public License permits, the exception does
 * not apply to the code that you add in this way.  To avoid misleading
 * anyone as to the status of such modified files, you must delete
 * this exception notice from them.
 *
 * If you write modifications of your own for Harbour, it is your choice
 * whether to permit this exception to apply to your modifications.
 * If you do not wish that, delete this exception notice.
 *
 */

/* Language name: Hungarian */
/* ISO language code (2 chars): HU */
/* Codepage: 852 (ntxhu852 compatible) */

/* NOTE: Several chars have been added above the standard 852 Hungarian
         ones to make it 100% compatible with ntxhu852.obj for CA-Cl*pper 5.x.
         Moreover the extra chars had to be replicated in the alternative
         codepages (WIN, ISO) too, to keep the Harbour codepage translation
         work. [vszakats] */

/* NOTE: Since there is no possibility in Harbour to have different number
         of uppercase and lowercase accented chars, a simple workaround
         was used to solve the problem; notice that some uppercase chars
         have the same lowercase values. Testing showed that both the
         ordering and Lower()/Upper() functions worked alright.
         [20070410] [vszakats] */

#define HB_CP_ID        HU852
#define HB_CP_INFO      "Hungarian CP-852 (ntxhu852 compatible)"
#define HB_CP_UNITB     HB_UNITB_852
#define HB_CP_ACSORT    HB_CDP_ACSORT_NONE
#define HB_CP_UPPER     "A���BCDE�FGHI��JKLMNO�����PQRSTU�隘�VWXYZ"
#define HB_CP_LOWER     "a���bcde�fghi��jklmno�����pqrstu�����vwxyz"

/* include CP registration code */
#include "hbcdpreg.h"
