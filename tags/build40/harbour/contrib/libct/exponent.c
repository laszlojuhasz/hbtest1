/*
 * $Id$
 */

/*
 * Harbour Project source code:
 *   CT3 Number and bit manipulation functions: - MANTISSA()
 *                                              - EXPONENT()
 *
 * Copyright 2002 Walter Negro - FOEESITRA" <waltern@foeesitra.org.ar>
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

#include "ct.h"

/* undefine the following if you want to evaluate the mantissa and exponent from the doubles' bit representation */
/* #define CT_EXPONENT_MANTISSA_BIT 1 */

/*  $DOC$
 *  $FUNCNAME$
 *      MANTISSA()
 *  $CATEGORY$
 *      CT3 number and bit manipulation functions
 *  $ONELINER$
 *      Evaluate the mantissa of a floating point number
 *  $SYNTAX$
 *      MANTISSA( <nFloatingPointNumber> ) --> nMantissa
 *  $ARGUMENTS$
 *      <nFloatingPointNumber> Designate any Harbour number.
 *  $RETURNS$
 *      MANTISSA() returns the mantissa of the <nFloatingPointNumber> number.
 *  $DESCRIPTION$
 *      This function supplements EXPONENT() to return the mantissa of the
 *      <nFloatingPointNumber> number.
 *
 *      Note:  The mantissa value can be 0 or in the range of 1 to 2.
 *
 *             The following calculation reproduces the original value:
 *
 *      MANTISSA(<nFloatingPointNumber>)* 2^EXPONENT(<nFloatingPointNumber>) =
 *      <nFloatingPointNumber>
 *      
 *      TODO: add documentation
 *  $EXAMPLES$
 *  $TESTS$
 *  $STATUS$
 *      Started
 *  $COMPLIANCE$
 *      MANTISSA() is compatible with CT3's MANTISSA().
 *  $PLATFORMS$
 *      All
 *  $FILES$
 *      Source is exponent.c, library is libct.
 *  $SEEALSO$
 *      EXPONENT()
 *  $END$
 */

HB_FUNC( MANTISSA )
{

#ifdef CT_EXPONENT_MANTISSA_BIT

  union 
  {
     double value;
     char   string[ sizeof( double )];
  }  xConvert;

  xConvert.value = hb_parnd( 1 );

  if( xConvert.value != 0 )
  {
     xConvert.string[6] |= 0xF0;
     xConvert.string[7] |= 0x3F;
     xConvert.string[7] &= 0xBF;
  }

  hb_retnd( xConvert.value );

#else

  double dValue;

  dValue = hb_parnd( 1 );

  if (dValue == 0.0)
  {
    hb_retnd( 0.0 );
    return;
  }

  if (fabs(dValue)<1.0)
  {
    while (fabs(dValue)<1.0)
      dValue *= 2.0;
  }
  else if (fabs(dValue)>=2.0)
  {
    while (fabs(dValue)>=2.0)
      dValue /= 2.0;
  }
  hb_retnd( dValue );

#endif

}


/*  $DOC$
 *  $FUNCNAME$
 *      EXPONENT()
 *  $CATEGORY$
 *      CT3 number and bit manipulation functions
 *  $ONELINER$
 *      Evaluate the exponent of a floating point number
 *  $SYNTAX$
 *      EXPONENT( <nFloatingPointNumber> ) --> nExponent
 *  $ARGUMENTS$
 *      <nFloatingPointNumber> Designate any Harbour number.
 *  $RETURNS$
 *      EXPONENT() returns the exponent of the <nFloatingPointNumber> number
 *      in base 2.
 *  $DESCRIPTION$
 *      This function supplements MANTISSA() to return the exponent of the
 *      <nFloatingPointNumber> number.
 *
 *      Values > 1 or values < -1 return a positive number 0 to 1023.
 *
 *      Values < 1 or values > -1 return a negative number -1 to -1023.
 *
 *      The EXPONENT( 0 ), return 0.
 *
 *      The following calculation reproduces the original value:
 *
 *      2^EXPONENT(<nFloatingPointNumber>) * MANTISSA(<nFloatingPointNumber>) =
 *      <nFloatingPointNumber>
 *      
 *      TODO: add documentation
 *  $EXAMPLES$
 *  $TESTS$
 *  $STATUS$
 *      Started
 *  $COMPLIANCE$
 *      EXPONENT() is compatible with CT3's EXPONENT()
 *  $PLATFORMS$
 *      All
 *  $FILES$
 *      Source is exponent.c, library is libct.
 *  $SEEALSO$
 *      MANTISSA()
 *  $END$
 */

HB_FUNC( EXPONENT )
{

#ifdef CT_EXPONENT_MANTISSA_BIT

  int  iExponent = 0;

  union 
  {
     double value;
     char   string[ sizeof( double )];
  }  xConvert;

  xConvert.value = hb_parnd( 1 );

  if( xConvert.value != 0 ) 
  {
     iExponent = ( int ) ( xConvert.string[7] & 0x07F );
     iExponent = iExponent << 4;
     iExponent += ( int ) ( ( xConvert.string[6] & 0xF0 ) >> 4 );
     iExponent -= 1023;
  }

  hb_retni( iExponent );

#else

  int iExponent = 0;
  double dValue;

  dValue = hb_parnd( 1 );

  if (dValue == 0.0)
  {
    hb_retni( 0 );
    return;
  }

  if( fabs( dValue ) < 1.0 )
  {
    while ( fabs( dValue ) < 1.0 )
    {
      dValue *= 2.0;
      iExponent--;
    }
  }
  else if ( fabs( dValue ) >= 2.0 )
  {
    while ( fabs( dValue ) >= 2.0 )
    {
      dValue /= 2.0;
      iExponent++;
    }
  }
  hb_retni( iExponent );

#endif

}

