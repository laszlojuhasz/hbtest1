/*
 * $Id$
 */

/*
 * Harbour Project source code:
 *   Test CT3 functions BEFORATNUM()
 *
 * Copyright 2001 IntTec GmbH, Neunlindenstr 32, 79106 Freiburg, Germany
 *        Author: Martin Vogel <vogel@inttec.de>
 *
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

#include "ct.ch"

PROCEDURE Main()

   LOCAL cStr := "...This...is...a...test!"

   ctinit()

   QOut( "Begin test of BEFORATNUM()" )
   QOut( "" )
   QOut( "  Value of cStr is:" + Chr( 34 ) + cStr + Chr( 34 ) )
   QOut( "" )

   // Some simple tests
   QOut( "  Simple tests:" )
   QOut( [  beforatnum("..",cStr)     should be "...This...is...a.",] )
   QOut( [                                and is "] + beforatnum( "..", cStr ) + ["] )
   QOut( [  beforatnum("..",cStr,2)   should be "...This",] )
   QOut( [                                and is "] + beforatnum( "..", cStr, 2 ) + ["] )
   QOut( [  beforatnum("..",cStr,2,2) should be "...This...is",] )
   QOut( [                                and is "] + beforatnum( "..", cStr, 2, 2 ) + ["] )
   QOut()

   // Tests with CSetAtMuPa(.T.)
   QOut( "  Multi-Pass tests" )
   QOut( "  Setting csetatmupa() to .T." )
   csetatmupa( .T. )
   QOut( [  beforatnum("..",cStr)     should be "...This...is...a.",] )
   QOut( [                                and is "] + beforatnum( "..", cStr ) + ["] )
   QOut( [  beforatnum("..",cStr,2)   should be ".",] )
   QOut( [                                and is "] + beforatnum( "..", cStr, 2 ) + ["] )
   QOut( [  beforatnum("..",cStr,2,2) should be "...This.",] )
   QOut( [                                and is "] + beforatnum( "..", cStr, 2, 2 ) + ["] )
   QOut( "  Setting csetatmupa() to .F." )
   csetatmupa( .F. )
   QOut()

   // Tests mit SetAtlike(1)
   QOut( "  SetAtLike tests" )
   QOut( [  Setting setatlike(CT_SETATLIKE_WILDCARD, ".")] )
   setatlike( CT_SETATLIKE_WILDCARD, "." )
   QOut( [  beforatnum("..",cStr) should be "...This...is...a...tes",] )
   QOut( [                            and is "] + beforatnum( "..", cStr ) + ["] )
   QOut( [  beforatnum("..",cStr,2,2) should be "...T",] )
   QOut( [                                and is "] + beforatnum( "..", cStr, 2, 2 ) + ["] )
   QOut( [  beforatnum("..",cStr,2,10) should be "...This...is",] )
   QOut( [                                 and is "] + beforatnum( "..", cStr, 2, 10 ) + ["] )
   QOut()

   QOut( "End test of BEFORATNUM()" )
   QOut()

   ctexit()

   RETURN
