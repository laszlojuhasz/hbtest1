/*
 * $Id$
 */

/*
 * Harbour Project source code:
 * National Collation Support Module (SVCLIP - Clipper compatible)
 *
 * Copyright 2006 Klas Engwall <klas dot engwall at engwall dot com>
 * www - http://harbour-project.org
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

/* NOTE: The collation sequence below is almost compatible with Clipper's
   NTXSWE.OBJ which does NOT provide a correct Swedish collation. The most
   notable error in NTXSWE.OBJ is the uppercase E with an acute accent
   versus the lowercase e with a grave accent. All other accented characters
   are ignored and show up according to their ASCII values. Another oddity
   in NTXSWE.OBJ is that the Danish "AE" and "ae" compound characters
   (ASCII values 146 and 145 respectively) turn up between ASCII 135 and
   136, a rather pointless relocation IMHO. I found no way to replicate
   that behaviour in the character strings below, so if you allow chr(146)
   and chr(145) to be saved in indexed fields there WILL be index corruption
   if data is shared between Clipper and Harbour. Upper()/Lower() converson
   of those characters as well as all accented characters must be done
   programatically just like in Clipper.

   For sharing data with Clipper, assuming that the chr(146) and chr(145)
   problem is properly taken care of in your code, this codepage version
   must be used. For correct collation according to the book "Svenska
   skrivregler" (Swedish Writing Rules) by Svenska Spr�kn�mnden (the Swedish
   Language Council), use the SV850 version instead. That will of course
   not be Clipper compatible.
 */

/* NOTE2: due to above conditions human readable form of SVCLIP definition
          has been replaced with binary tables generated by tests/cpinfo.prg
          compiled by Clipper and linked with NTXSWE.OBJ so now SVCLIP is
          _fully_ Clipper compatible and should be used when data is shared
          between Clipper and Harbour applications.
 */

/*
#define HB_CP_ID        SVCLIP
#define HB_CP_INFO      "Swedish CP-437"
#define HB_CP_UNITB     HB_UNITB_437
#define HB_CP_ACSORT    HB_CDP_ACSORT_NONE
#define HB_CP_UPPER     "ABCDE�FGHIJKLMNOPQRSTUVWXY�Z���"
#define HB_CP_LOWER     "abcde�fghijklmnopqrstuvwxy�z���"
*/

#define HB_CP_ID        SV437C
#define HB_CP_INFO      "Swedish CP-437 (ntxswe.obj compatible)"
#define HB_CP_UNITB     HB_UNITB_437

#define HB_CP_RAW

static const unsigned char s_flags[ 256 ] = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,0,0,0,0,0,0,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,0,0,0,0,0,0,6,0,0,6,0,6,0,0,0,6,0,0,0,10,10,10,0,0,0,6,0,0,0,0,10,10,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };
static const unsigned char s_upper[ 256 ] = { 0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,52,53,54,55,56,57,58,59,60,61,62,63,64,65,66,67,68,69,70,71,72,73,74,75,76,77,78,79,80,81,82,83,84,85,86,87,88,89,90,91,92,93,94,95,96,65,66,67,68,69,70,71,72,73,74,75,76,77,78,79,80,81,82,83,84,85,86,87,88,89,90,123,124,125,126,127,128,154,130,131,142,133,143,135,136,137,144,139,140,141,142,143,144,145,146,147,153,149,150,151,152,153,154,155,156,157,158,159,160,161,162,163,164,165,166,167,168,169,170,171,172,173,174,175,176,177,178,179,180,181,182,183,184,185,186,187,188,189,190,191,192,193,194,195,196,197,198,199,200,201,202,203,204,205,206,207,208,209,210,211,212,213,214,215,216,217,218,219,220,221,222,223,224,225,226,227,228,229,230,231,232,233,234,235,236,237,238,239,240,241,242,243,244,245,246,247,248,249,250,251,252,253,254,255 };
static const unsigned char s_lower[ 256 ] = { 0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,52,53,54,55,56,57,58,59,60,61,62,63,64,97,98,99,100,101,102,103,104,105,106,107,108,109,110,111,112,113,114,115,116,117,118,119,120,121,122,91,92,93,94,95,96,97,98,99,100,101,102,103,104,105,106,107,108,109,110,111,112,113,114,115,116,117,118,119,120,121,122,123,124,125,126,127,128,129,130,131,132,133,134,135,136,137,138,139,140,141,132,134,130,145,146,147,148,149,150,151,152,148,129,155,156,157,158,159,160,161,162,163,164,165,166,167,168,169,170,171,172,173,174,175,176,177,178,179,180,181,182,183,184,185,186,187,188,189,190,191,192,193,194,195,196,197,198,199,200,201,202,203,204,205,206,207,208,209,210,211,212,213,214,215,216,217,218,219,220,221,222,223,224,225,226,227,228,229,230,231,232,233,234,235,236,237,238,239,240,241,242,243,244,245,246,247,248,249,250,251,252,253,254,255 };
static const unsigned char s_sort [ 256 ] = { 0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,52,53,54,55,56,57,58,59,60,61,62,63,64,65,66,67,68,69,71,72,73,74,75,76,77,78,79,80,81,82,83,84,85,86,87,88,89,90,92,96,97,98,99,100,101,102,103,104,105,106,108,109,110,111,112,113,114,115,116,117,118,119,120,121,122,123,124,125,126,127,129,133,134,135,136,137,138,128,139,140,131,141,130,142,145,146,107,147,148,149,94,93,70,144,143,150,132,151,152,153,154,95,91,155,156,157,158,159,160,161,162,163,164,165,166,167,168,169,170,171,172,173,174,175,176,177,178,179,180,181,182,183,184,185,186,187,188,189,190,191,192,193,194,195,196,197,198,199,200,201,202,203,204,205,206,207,208,209,210,211,212,213,214,215,216,217,218,219,220,221,222,223,224,225,226,227,228,229,230,231,232,233,234,235,236,237,238,239,240,241,242,243,244,245,246,247,248,249,250,251,252,253,254,255 };

/* include CP registration code */
#include "hbcdpreg.h"
