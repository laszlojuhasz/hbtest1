/*
 * $Id$
 */

/*
 * GT CLIPPER STANDARD HEADER
 *
 * File......: strpbrk.c
 * Author....: Andy M Leighton
 * BBS.......: The Dark Knight Returns
 * Net/Node..: 050/069
 * User Name.: Andy Leighton
 * Date......: 23/05/93
 * Revision..: 1.00
 *
 * This is an original work by Andy Leighton and is placed in the
 * public domain.
 */

/*
 *  $DOC$
 *  $FUNCNAME$
 *      GT_STRPBRK()
 *  $CATEGORY$
 *      String
 *  $ONELINER$
 *      Return string after 1st char from a set
 *  $SYNTAX$
 *      GT_StrpBrk(<cStr>, <cSet>) --> cString
 *  $ARGUMENTS$
 *      <cStr>   - The input string
 *      <cSet>   - The set of characters to find
 *  $RETURNS$
 *      cString  - The input string after the first occurance of any
 *                 character from <cSet>
 *  $DESCRIPTION$
 *      Return a string after the first occurance of any character from
 *      the input set <cSet>.
 *  $EXAMPLES$
 *
 *      ? GT_Strpbrk("This is a test", "sa ")  // prints "s is a test"
 *      ? GT_Strpbrk("This is a test", "et")   // prints "test"
 *
 *  $END$
 */


#include "extend.h"


HARBOUR HB_GT_STRPBRK( void )
{
  char *string;
  char *cset;
  int l1, l2;
  int p1, p2;

  if (ISCHAR(1) && ISCHAR(2)) {
    string = hb_parc(1);
    cset   = hb_parc(2);
    l1     = hb_parclen(1);
    l2     = hb_parclen(2);
    p1     = p2 = 0;

    do {
      for (p2 = 0; (p2 < l2) && (cset[p2] != string[p1]); ++p2)
         ;
      if (p2 < l2) {
         hb_retc(string + p1);
         break;
      }
    } while (p1++ < l1);

    if (p2 >= l2)
      hb_retc((char *) NULL);

  } else {
    hb_retc((char *) NULL);               /* parameter mismatch - error NullStr */
  }
}

