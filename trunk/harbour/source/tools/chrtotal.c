/*
 * $Id$
 */

/*
 * GT CLIPPER STANDARD HEADER
 *
 * File......: chrtotal.c
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
 *      GT_CHRTOTAL()
 *  $CATEGORY$
 *      String
 *  $ONELINER$
 *      Find number of times a set of characters appears in a string
 *  $SYNTAX$
 *      GT_ChrTotal(<cChrs>, <cStr>) --> nTotOcc
 *  $ARGUMENTS$
 *      <cChrs> - The set of characters
 *      <cStr>  - The string to search
 *  $RETURNS$
 *      nTotOcc - The number of times the characters specified in
 *                <cChrs> appears in <cStr>
 *  $DESCRIPTION$
 *      Returns the numnber of occurrences of characters belonging
 *      to the set <cChrs> in the string <cStr>.  If no characters
 *      in <cChrs> appears in <cStr> GT_ChrTotal() will return 0.
 *
 *      NOTE:
 *         invalid parameters will return -1
 *  $EXAMPLES$
 *
 *       local cStr1 := "the cat sat on the mat"
 *
 *       ? GT_ChrTotal("tae", cStr1)            // prints 10
 *       ? GT_ChrTotal("zqw", cStr1)            // prints  0
 *  $END$
 */

#include <extend.h>

HARBOUR HB_GT_CHRTOTAL( void )
{
  char *s1, *s2;
  int count, p1, p2, l2, l1;

  if (ISCHAR(1) && ISCHAR(2)) {
    s1  = hb_parc(1);
    s2  = hb_parc(2);
    l2  = hb_parclen(2);
    l1  = hb_parclen(1);

    for (count = 0, p2 = 0; p2 < l2; p2++)
      for (p1 = 0; p1 < l1; p1++)
        if (s1[p1] == s2[p2])
          count++;                    /* increment counter */

    hb_retni(count);                  /* return result */
  } else {
    hb_retni(-1);                     /* parameter mismatch - error -1 */
  }
}
