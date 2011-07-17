/*
 * $Id$
 */

/*
 * Harbour Project source code:
 *
 *   Test CT3 math functions 
 *   - FLOOR
 *   - CEILING
 *   - LOG10
 *   - SIGN
 *   - FACT
 *
 * Copyright 2001   Alejandro de Garate  <alex_degarate@hotmail.com>
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


 PROCEDURE MAIN

 CTINIT()

 SET DECIMAL TO 2
 CLS
 ? "Test of mathematical functions"
 ?

 ? "FLOOR( 1.9) = 1       // CT3"
 ? SPACE(4), FLOOR( 1.9),"  <-- CT for Harbour"
 ?

 ? "FLOOR( 1.1) = 1       // CT3"
 ? SPACE(4), FLOOR( 1.1),"  <-- CT for Harbour"
 ?

 ? "FLOOR( 0.9) = 0       // CT3"
 ? SPACE(4), FLOOR( 0.9),"  <-- CT for Harbour"
 ?

 ? "FLOOR( -0.1) = -1       // CT3"
 ? SPACE(6), FLOOR( -0.1),"  <-- CT for Harbour"
 ?

 ? "FLOOR( -0.9) = -1       // CT3"
 ? SPACE(6), FLOOR( -0.9),"  <-- CT for Harbour"
 ?

 ? "FLOOR( -1.1) = -2       // CT3"
 ? SPACE(6), FLOOR( -1.1),"  <-- CT for Harbour"
 ?
 WAIT4()

 ? "CEILING( 1.9) = 2       // CT3"
 ? SPACE(6), CEILING( 1.9),"  <-- CT for Harbour"
 ?

 ? "CEILING( 1.1) = 2       // CT3"
 ? SPACE(6), CEILING( 1.1),"  <-- CT for Harbour"
 ?

 ? "CEILING( 0.9) = 1       // CT3"
 ? SPACE(6), CEILING( 0.9),"  <-- CT for Harbour"
 ?

 ? "CEILING( -0.1) = 0       // CT3"
 ? SPACE(7), CEILING( -0.1),"  <-- CT for Harbour"
 ?

 ? "CEILING( -0.9) = 0       // CT3"
 ? SPACE(7), CEILING( -0.9),"  <-- CT for Harbour"
 ?

 ? "CEILING( -1.1) = -1       // CT3"
 ? SPACE(8), CEILING( -1.1),"  <-- CT for Harbour"
 ?

 WAIT4()


 ? "LOG10( 0.01 ) = -2.00    // CT3"
 ?  SPACE(7), LOG10( 0.01 ),"  <-- CT for Harbour"
 ?

 ? "LOG10( 2 ) = 0.30    // CT3"
 ?  SPACE(3), LOG10( 2 ),"  <-- CT for Harbour"
 ?

 ? "LOG10( 100 ) = 2.00    // CT3"
 ?  SPACE(5), LOG10( 100 ),"  <-- CT for Harbour"
 ?
 ?

 ? "SIGN( 48335 ) = 1    // CT3"
 ? SPACE(6), SIGN( 48335 ),"  <-- CT for Harbour"
 ?

 ? "SIGN( -258 ) = -1    // CT3"
 ? SPACE(6), SIGN( -258 ),"  <-- CT for Harbour"
 ?

 WAIT4()

 SET DECIMALS TO 0
 ? "FACT( 1 ) = 1    // CT3"
 ? SPACE(2), FACT( 1 ), "  <-- CT for Harbour"
 ?

 ? "FACT( 5 ) = 120    // CT3"
 ? SPACE(4), FACT( 5), "  <-- CT for Harbour"
 ?

 ? "FACT( 21 ) = 51090942171709440000    // CT3"
 ? SPACE(12), FACT( 21), "  <-- CT for Harbour"
 ?

 ? "FACT( 25 ) = -1    // CT3"
 ? SPACE(4), FACT( 25), "  <-- CT for Harbour"
 ?

 ? "FACT( 0 ) = 1    // CT3"
 ? SPACE(2), FACT( 0), "  <-- CT for Harbour"
 ?

 CTEXIT()

RETURN

PROCEDURE WAIT4
 ? "  PRESS ANY KEY"
 INKEY(0)
 CLS

RETURN







