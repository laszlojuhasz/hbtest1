/*
 * $Id$
 */

/*
 * Harbour Project source code:
 * Regression tests for the runtime library (strings)
 *
 * Copyright 1999 Victor Szakats <info@szelvesz.hu>
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

#include "rt_main.ch"

/* Don't change the position of this #include. */
#include "rt_vars.ch"

FUNCTION Main_STRA()

   /* STR() */

   TEST_LINE( Str(NIL)                        , "E BASE 1099 Argument error STR F:S" )
   TEST_LINE( Str("A", 10, 2)                 , "E BASE 1099 Argument error STR F:S" )
   TEST_LINE( Str(100, 10, "A")               , "E BASE 1099 Argument error STR F:S" )
   TEST_LINE( Str(100, 10, NIL)               , "E BASE 1099 Argument error STR F:S" )
   TEST_LINE( Str(100, NIL, NIL)              , "E BASE 1099 Argument error STR F:S" )
   TEST_LINE( Str( w_TEST->TYPE_N_I )         , "        100"    )
   TEST_LINE( Str( w_TEST->TYPE_N_IE )        , "          0"    )
   TEST_LINE( Str( w_TEST->TYPE_N_D )         , "    101.127"    )
   TEST_LINE( Str( w_TEST->TYPE_N_DE )        , "      0.000"    )
   TEST_LINE( Str(5000000000.0)               , "5000000000.0"   )
   TEST_LINE( Str(50000000)                   , "  50000000"     )
   TEST_LINE( Str(500000000)                  , " 500000000"     )
   TEST_LINE( Str(5000000000)                 , " 5000000000"    )
   TEST_LINE( Str(50000000000)                , " 50000000000"   )
   TEST_LINE( Str(-5000000000.0)              , "         -5000000000.0" )
   TEST_LINE( Str(-5000000000)                , "         -5000000000"   )
   TEST_LINE( Str(2.0000000000000001)         , "         2.0000000000000000" )
   TEST_LINE( Str(2.0000000000000009)         , "         2.0000000000000010" )
   TEST_LINE( Str(2.000000000000001)          , "         2.000000000000001"  )
   TEST_LINE( Str(2.000000000000009)          , "         2.000000000000009"  )
   TEST_LINE( Str(2.00000000000001)           , "         2.00000000000001"   )
   TEST_LINE( Str(2.00000000000009)           , "         2.00000000000009"   )
   TEST_LINE( Str(2.000000000001)             , "         2.000000000001"     )
   TEST_LINE( Str(2.00000000001)              , "         2.00000000001"      )
   TEST_LINE( Str(10)                         , "        10"     )
   TEST_LINE( Str(10.0)                       , "        10.0"   )
   TEST_LINE( Str(10.00)                      , "        10.00"  )
   TEST_LINE( Str(10.50)                      , "        10.50"  )
   TEST_LINE( Str(100000)                     , "    100000"     )
   TEST_LINE( Str(-10)                        , "       -10"     )
   TEST_LINE( Str(-10.0)                      , "       -10.0"   )
   TEST_LINE( Str(-10.00)                     , "       -10.00"  )
   TEST_LINE( Str(-10.50)                     , "       -10.50"  )
   TEST_LINE( Str(-100000)                    , "   -100000"     )
   TEST_LINE( Str(10, 5)                      , "   10"          )
   TEST_LINE( Str(10.0, 5)                    , "   10"          )
   TEST_LINE( Str(10.00, 5)                   , "   10"          )
   TEST_LINE( Str(10.50, 5)                   , "   11"          )
   TEST_LINE( Str(100000, 5)                  , "*****"          )
   TEST_LINE( Str(100000, 8)                  , "  100000"       )
   TEST_LINE( Str(-10, 5)                     , "  -10"          )
   TEST_LINE( Str(-10.0, 5)                   , "  -10"          )
   TEST_LINE( Str(-10.00, 5)                  , "  -10"          )
   TEST_LINE( Str(-10.50, 5)                  , "  -11"          )
   TEST_LINE( Str(-100000, 5)                 , "*****"          )
   TEST_LINE( Str(-100000, 6)                 , "******"         )
   TEST_LINE( Str(-100000, 8)                 , " -100000"       )
#ifndef __XPP__ /* Internal structures corrupted */
   TEST_LINE( Str(10, -5)                     , "        10"     )
   TEST_LINE( Str(10.0, -5)                   , "        10"     )
   TEST_LINE( Str(10.00, -5)                  , "        10"     )
   TEST_LINE( Str(10.50, -5)                  , "        11"     )
   TEST_LINE( Str(100000, -5)                 , "    100000"     )
   TEST_LINE( Str(100000, -8)                 , "    100000"     )
   TEST_LINE( Str(-10, -5)                    , "       -10"     )
   TEST_LINE( Str(-10.0, -5)                  , "       -10"     )
   TEST_LINE( Str(-10.00, -5)                 , "       -10"     )
   TEST_LINE( Str(-10.50, -5)                 , "       -11"     )
   TEST_LINE( Str(-100000, -5)                , "   -100000"     )
   TEST_LINE( Str(-100000, -6)                , "   -100000"     )
   TEST_LINE( Str(-100000, -8)                , "   -100000"     )
#endif
   TEST_LINE( Str(10, 5, 0)                   , "   10"          )
   TEST_LINE( Str(10.0, 5, 0)                 , "   10"          )
   TEST_LINE( Str(10.00, 5, 0)                , "   10"          )
   TEST_LINE( Str(10.50, 5, 0)                , "   11"          )
   TEST_LINE( Str(100000, 5, 0)               , "*****"          )
   TEST_LINE( Str(-10, 5, 0)                  , "  -10"          )
   TEST_LINE( Str(-10.0, 5, 0)                , "  -10"          )
   TEST_LINE( Str(-10.00, 5, 0)               , "  -10"          )
   TEST_LINE( Str(-10.50, 5, 0)               , "  -11"          )
   TEST_LINE( Str(-100000, 5, 0)              , "*****"          )
   TEST_LINE( Str(-100000, 6, 0)              , "******"         )
   TEST_LINE( Str(-100000, 8, 0)              , " -100000"       )
   TEST_LINE( Str(10, 5, 1)                   , " 10.0"          )
   TEST_LINE( Str(10.0, 5, 1)                 , " 10.0"          )
   TEST_LINE( Str(10.00, 5, 1)                , " 10.0"          )
   TEST_LINE( Str(10.50, 5, 1)                , " 10.5"          )
   TEST_LINE( Str(100000, 5, 1)               , "*****"          )
   TEST_LINE( Str(-10, 5, 1)                  , "-10.0"          )
   TEST_LINE( Str(-10.0, 5, 1)                , "-10.0"          )
   TEST_LINE( Str(-10.00, 5, 1)               , "-10.0"          )
   TEST_LINE( Str(-10.50, 5, 1)               , "-10.5"          )
   TEST_LINE( Str(-100000, 5, 1)              , "*****"          )
   TEST_LINE( Str(-100000, 6, 1)              , "******"         )
   TEST_LINE( Str(-100000, 8, 1)              , "********"       )
#ifndef __XPP__ /* Internal structures corrupted */
   TEST_LINE( Str(10, 5, -1)                  , "   10"          )
   TEST_LINE( Str(10.0, 5, -1)                , "   10"          )
   TEST_LINE( Str(10.00, 5, -1)               , "   10"          )
   TEST_LINE( Str(10.50, 5, -1)               , "   11"          )
   TEST_LINE( Str(100000, 5, -1)              , "*****"          )
   TEST_LINE( Str(-10, 5, -1)                 , "  -10"          )
   TEST_LINE( Str(-10.0, 5, -1)               , "  -10"          )
   TEST_LINE( Str(-10.00, 5, -1)              , "  -10"          )
   TEST_LINE( Str(-10.50, 5, -1)              , "  -11"          )
   TEST_LINE( Str(-100000, 5, -1)             , "*****"          )
   TEST_LINE( Str(-100000, 6, -1)             , "******"         )
   TEST_LINE( Str(-100000, 8, -1)             , " -100000"       )
#endif

   /* STRZERO() */

#ifdef HARBOUR_STRICT_CLIPPER_COMPATIBILITY
   TEST_LINE( StrZero(NIL)                    , "E BASE 1099 Argument error STR F:S" )
   TEST_LINE( StrZero("A", 10, 2)             , "E BASE 1099 Argument error STR F:S" )
   TEST_LINE( StrZero(100, 10, "A")           , "E BASE 1099 Argument error STR F:S" )
   TEST_LINE( StrZero(100, 10, NIL)           , "E BASE 1099 Argument error STR F:S" )
   TEST_LINE( StrZero(100, NIL, NIL)          , "E BASE 1099 Argument error STR F:S" )
#else
   TEST_LINE( StrZero(NIL)                    , "E BASE 9999 Argument error STRZERO F:S" )
   TEST_LINE( StrZero("A", 10, 2)             , "E BASE 9999 Argument error STRZERO F:S" )
   TEST_LINE( StrZero(100, 10, "A")           , "E BASE 9999 Argument error STRZERO F:S" )
   TEST_LINE( StrZero(100, 10, NIL)           , "E BASE 9999 Argument error STRZERO F:S" )
   TEST_LINE( StrZero(100, NIL, NIL)          , "E BASE 9999 Argument error STRZERO F:S" )
#endif
   TEST_LINE( StrZero(10)                     , "0000000010"     )
   TEST_LINE( StrZero(10.0)                   , "0000000010.0"   )
   TEST_LINE( StrZero(10.00)                  , "0000000010.00"  )
   TEST_LINE( StrZero(10.50)                  , "0000000010.50"  )
   TEST_LINE( StrZero(100000)                 , "0000100000"     )
   TEST_LINE( StrZero(-10)                    , "-000000010"     )
   TEST_LINE( StrZero(-10.0)                  , "-000000010.0"   )
   TEST_LINE( StrZero(-10.00)                 , "-000000010.00"  )
   TEST_LINE( StrZero(-10.50)                 , "-000000010.50"  )
   TEST_LINE( StrZero(-100000)                , "-000100000"     )
   TEST_LINE( StrZero(10, 5)                  , "00010"          )
   TEST_LINE( StrZero(10.0, 5)                , "00010"          )
   TEST_LINE( StrZero(10.00, 5)               , "00010"          )
   TEST_LINE( StrZero(10.50, 5)               , "00011"          )
   TEST_LINE( StrZero(100000, 5)              , "*****"          )
   TEST_LINE( StrZero(100000, 8)              , "00100000"       )
   TEST_LINE( StrZero(-10, 5)                 , "-0010"          )
   TEST_LINE( StrZero(-10.0, 5)               , "-0010"          )
   TEST_LINE( StrZero(-10.00, 5)              , "-0010"          )
   TEST_LINE( StrZero(-10.50, 5)              , "-0011"          )
   TEST_LINE( StrZero(-100000, 5)             , "*****"          )
   TEST_LINE( StrZero(-100000, 6)             , "******"         )
   TEST_LINE( StrZero(-100000, 8)             , "-0100000"       )
#ifndef __XPP__ /* Internal structures corrupted */
   TEST_LINE( StrZero(10, -5)                 , "0000000010"     )
   TEST_LINE( StrZero(10.0, -5)               , "0000000010"     )
   TEST_LINE( StrZero(10.00, -5)              , "0000000010"     )
   TEST_LINE( StrZero(10.50, -5)              , "0000000011"     )
   TEST_LINE( StrZero(100000, -5)             , "0000100000"     )
   TEST_LINE( StrZero(100000, -8)             , "0000100000"     )
   TEST_LINE( StrZero(-10, -5)                , "-000000010"     )
   TEST_LINE( StrZero(-10.0, -5)              , "-000000010"     )
   TEST_LINE( StrZero(-10.00, -5)             , "-000000010"     )
   TEST_LINE( StrZero(-10.50, -5)             , "-000000011"     )
   TEST_LINE( StrZero(-100000, -5)            , "-000100000"     )
   TEST_LINE( StrZero(-100000, -6)            , "-000100000"     )
   TEST_LINE( StrZero(-100000, -8)            , "-000100000"     )
#endif
   TEST_LINE( StrZero(10, 5, 0)               , "00010"          )
   TEST_LINE( StrZero(10.0, 5, 0)             , "00010"          )
   TEST_LINE( StrZero(10.50, 5, 0)            , "00011"          )
   TEST_LINE( StrZero(100000, 5, 0)           , "*****"          )
   TEST_LINE( StrZero(-10, 5, 0)              , "-0010"          )
   TEST_LINE( StrZero(-10.0, 5, 0)            , "-0010"          )
   TEST_LINE( StrZero(-10.00, 5, 0)           , "-0010"          )
   TEST_LINE( StrZero(-10.50, 5, 0)           , "-0011"          )
   TEST_LINE( StrZero(-100000, 5, 0)          , "*****"          )
   TEST_LINE( StrZero(-100000, 6, 0)          , "******"         )
   TEST_LINE( StrZero(-100000, 8, 0)          , "-0100000"       )
   TEST_LINE( StrZero(10, 5, 1)               , "010.0"          )
   TEST_LINE( StrZero(10.0, 5, 1)             , "010.0"          )
   TEST_LINE( StrZero(10.50, 5, 1)            , "010.5"          )
   TEST_LINE( StrZero(100000, 5, 1)           , "*****"          )
   TEST_LINE( StrZero(-10, 5, 1)              , "-10.0"          )
   TEST_LINE( StrZero(-10.0, 5, 1)            , "-10.0"          )
   TEST_LINE( StrZero(-10.00, 5, 1)           , "-10.0"          )
   TEST_LINE( StrZero(-10.50, 5, 1)           , "-10.5"          )
   TEST_LINE( StrZero(-100000, 5, 1)          , "*****"          )
   TEST_LINE( StrZero(-100000, 6, 1)          , "******"         )
   TEST_LINE( StrZero(-100000, 8, 1)          , "********"       )
#ifndef __XPP__ /* Internal structures corrupted */
   TEST_LINE( StrZero(10, 5, -1)              , "00010"          )
   TEST_LINE( StrZero(10.0, 5, -1)            , "00010"          )
   TEST_LINE( StrZero(10.50, 5, -1)           , "00011"          )
   TEST_LINE( StrZero(100000, 5, -1)          , "*****"          )
   TEST_LINE( StrZero(-10, 5, -1)             , "-0010"          )
   TEST_LINE( StrZero(-10.0, 5, -1)           , "-0010"          )
   TEST_LINE( StrZero(-10.00, 5, -1)          , "-0010"          )
   TEST_LINE( StrZero(-10.50, 5, -1)          , "-0011"          )
   TEST_LINE( StrZero(-100000, 5, -1)         , "*****"          )
   TEST_LINE( StrZero(-100000, 6, -1)         , "******"         )
   TEST_LINE( StrZero(-100000, 8, -1)         , "-0100000"       )
#endif

   RETURN NIL

FUNCTION Comp_Str()
   LOCAL old_exact := SET( _SET_EXACT, .F. )

   TEST_LINE( "ABC" == "", .F. )
   TEST_LINE( "ABC" = "", .T. )
   TEST_LINE( "ABC" != "", .F. )
   TEST_LINE( "ABC" < "", .F. )
   TEST_LINE( "ABC" <= "", .T. )
   TEST_LINE( "ABC" > "", .F. )
   TEST_LINE( "ABC" >= "", .T. )
   TEST_LINE( "" == "ABC", .F. )
   TEST_LINE( "" = "ABC", .F. )
   TEST_LINE( "" != "ABC", .T. )
   TEST_LINE( "" < "ABC", .T. )
   TEST_LINE( "" <= "ABC", .T. )
   TEST_LINE( "" > "ABC", .F. )
   TEST_LINE( "" >= "ABC", .F. )
   TEST_LINE( "ABC" == " ", .F. )
   TEST_LINE( "ABC" = " ", .F. )
   TEST_LINE( "ABC" != " ", .T. )
   TEST_LINE( "ABC" < " ", .F. )
   TEST_LINE( "ABC" <= " ", .F. )
   TEST_LINE( "ABC" > " ", .T. )
   TEST_LINE( "ABC" >= " ", .T. )
   TEST_LINE( " " == "ABC", .F. )
   TEST_LINE( " " = "ABC", .F. )
   TEST_LINE( " " != "ABC", .T. )
   TEST_LINE( " " < "ABC", .T. )
   TEST_LINE( " " <= "ABC", .T. )
   TEST_LINE( " " > "ABC", .F. )
   TEST_LINE( " " >= "ABC", .F. )
   TEST_LINE( "ABC" == "ABC", .T. )
   TEST_LINE( "ABC" = "ABC", .T. )
   TEST_LINE( "ABC" != "ABC", .F. )
   TEST_LINE( "ABC" < "ABC", .F. )
   TEST_LINE( "ABC" <= "ABC", .T. )
   TEST_LINE( "ABC" > "ABC", .F. )
   TEST_LINE( "ABC" >= "ABC", .T. )
   TEST_LINE( "ABC" == "ABCD", .F. )
   TEST_LINE( "ABC" = "ABCD", .F. )
   TEST_LINE( "ABC" != "ABCD", .T. )
   TEST_LINE( "ABC" < "ABCD", .T. )
   TEST_LINE( "ABC" <= "ABCD", .T. )
   TEST_LINE( "ABC" > "ABCD", .F. )
   TEST_LINE( "ABC" >= "ABCD", .F. )
   TEST_LINE( "ABCD" == "ABC", .F. )
   TEST_LINE( "ABCD" = "ABC", .T. )
   TEST_LINE( "ABCD" != "ABC", .F. )
   TEST_LINE( "ABCD" < "ABC", .F. )
   TEST_LINE( "ABCD" <= "ABC", .T. )
   TEST_LINE( "ABCD" > "ABC", .F. )
   TEST_LINE( "ABCD" >= "ABC", .T. )
   TEST_LINE( "ABC" == "ABC ", .F. )
   TEST_LINE( "ABC" = "ABC ", .F. )
   TEST_LINE( "ABC" != "ABC ", .T. )
   TEST_LINE( "ABC" < "ABC ", .T. )
   TEST_LINE( "ABC" <= "ABC ", .T. )
   TEST_LINE( "ABC" > "ABC ", .F. )
   TEST_LINE( "ABC" >= "ABC ", .F. )
   TEST_LINE( "ABC " == "ABC", .F. )
   TEST_LINE( "ABC " = "ABC", .T. )
   TEST_LINE( "ABC " != "ABC", .F. )
   TEST_LINE( "ABC " < "ABC", .F. )
   TEST_LINE( "ABC " <= "ABC", .T. )
   TEST_LINE( "ABC " > "ABC", .F. )
   TEST_LINE( "ABC " >= "ABC", .T. )
   TEST_LINE( "ABC" == "DEF", .F. )
   TEST_LINE( "ABC" = "DEF", .F. )
   TEST_LINE( "ABC" != "DEF", .T. )
   TEST_LINE( "ABC" < "DEF", .T. )
   TEST_LINE( "ABC" <= "DEF", .T. )
   TEST_LINE( "ABC" > "DEF", .F. )
   TEST_LINE( "ABC" >= "DEF", .F. )
   TEST_LINE( "DEF" == "ABC", .F. )
   TEST_LINE( "DEF" = "ABC", .F. )
   TEST_LINE( "DEF" != "ABC", .T. )
   TEST_LINE( "DEF" < "ABC", .F. )
   TEST_LINE( "DEF" <= "ABC", .F. )
   TEST_LINE( "DEF" > "ABC", .T. )
   TEST_LINE( "DEF" >= "ABC", .T. )
   TEST_LINE( "ABC" == "DEFG", .F. )
   TEST_LINE( "ABC" = "DEFG", .F. )
   TEST_LINE( "ABC" != "DEFG", .T. )
   TEST_LINE( "ABC" < "DEFG", .T. )
   TEST_LINE( "ABC" <= "DEFG", .T. )
   TEST_LINE( "ABC" > "DEFG", .F. )
   TEST_LINE( "ABC" >= "DEFG", .F. )
   TEST_LINE( "DEFG" == "ABC", .F. )
   TEST_LINE( "DEFG" = "ABC", .F. )
   TEST_LINE( "DEFG" != "ABC", .T. )
   TEST_LINE( "DEFG" < "ABC", .F. )
   TEST_LINE( "DEFG" <= "ABC", .F. )
   TEST_LINE( "DEFG" > "ABC", .T. )
   TEST_LINE( "DEFG" >= "ABC", .T. )
   TEST_LINE( "ABCD" == "DEF", .F. )
   TEST_LINE( "ABCD" = "DEF", .F. )
   TEST_LINE( "ABCD" != "DEF", .T. )
   TEST_LINE( "ABCD" < "DEF", .T. )
   TEST_LINE( "ABCD" <= "DEF", .T. )
   TEST_LINE( "ABCD" > "DEF", .F. )
   TEST_LINE( "ABCD" >= "DEF", .F. )
   TEST_LINE( "DEF" == "ABCD", .F. )
   TEST_LINE( "DEF" = "ABCD", .F. )
   TEST_LINE( "DEF" != "ABCD", .T. )
   TEST_LINE( "DEF" < "ABCD", .F. )
   TEST_LINE( "DEF" <= "ABCD", .F. )
   TEST_LINE( "DEF" > "ABCD", .T. )
   TEST_LINE( "DEF" >= "ABCD", .T. )

   SET( _SET_EXACT, old_exact )
   RETURN NIL

FUNCTION Exact_Str()
   LOCAL old_exact := SET( _SET_EXACT, .T. )

   TEST_LINE( "ABC" == "", .F. )
   TEST_LINE( "ABC" = "", .F. )
   TEST_LINE( "ABC" != "", .T. )
   TEST_LINE( "ABC" < "", .F. )
   TEST_LINE( "ABC" <= "", .F. )
   TEST_LINE( "ABC" > "", .T. )
   TEST_LINE( "ABC" >= "", .T. )
   TEST_LINE( "" == "ABC", .F. )
   TEST_LINE( "" = "ABC", .F. )
   TEST_LINE( "" != "ABC", .T. )
   TEST_LINE( "" < "ABC", .T. )
   TEST_LINE( "" <= "ABC", .T. )
   TEST_LINE( "" > "ABC", .F. )
   TEST_LINE( "" >= "ABC", .F. )
   TEST_LINE( "ABC" == " ", .F. )
   TEST_LINE( "ABC" = " ", .F. )
   TEST_LINE( "ABC" != " ", .T. )
   TEST_LINE( "ABC" < " ", .F. )
   TEST_LINE( "ABC" <= " ", .F. )
   TEST_LINE( "ABC" > " ", .T. )
   TEST_LINE( "ABC" >= " ", .T. )
   TEST_LINE( " " == "ABC", .F. )
   TEST_LINE( " " = "ABC", .F. )
   TEST_LINE( " " != "ABC", .T. )
   TEST_LINE( " " < "ABC", .T. )
   TEST_LINE( " " <= "ABC", .T. )
   TEST_LINE( " " > "ABC", .F. )
   TEST_LINE( " " >= "ABC", .F. )
   TEST_LINE( "ABC" == "ABC", .T. )
   TEST_LINE( "ABC" = "ABC", .T. )
   TEST_LINE( "ABC" != "ABC", .F. )
   TEST_LINE( "ABC" < "ABC", .F. )
   TEST_LINE( "ABC" <= "ABC", .T. )
   TEST_LINE( "ABC" > "ABC", .F. )
   TEST_LINE( "ABC" >= "ABC", .T. )
   TEST_LINE( "ABC" == "ABCD", .F. )
   TEST_LINE( "ABC" = "ABCD", .F. )
   TEST_LINE( "ABC" != "ABCD", .T. )
   TEST_LINE( "ABC" < "ABCD", .T. )
   TEST_LINE( "ABC" <= "ABCD", .T. )
   TEST_LINE( "ABC" > "ABCD", .F. )
   TEST_LINE( "ABC" >= "ABCD", .F. )
   TEST_LINE( "ABCD" == "ABC", .F. )
   TEST_LINE( "ABCD" = "ABC", .F. )
   TEST_LINE( "ABCD" != "ABC", .T. )
   TEST_LINE( "ABCD" < "ABC", .F. )
   TEST_LINE( "ABCD" <= "ABC", .F. )
   TEST_LINE( "ABCD" > "ABC", .T. )
   TEST_LINE( "ABCD" >= "ABC", .T. )
   TEST_LINE( "ABC" == "ABC ", .F. )
   TEST_LINE( "ABC" = "ABC ", .T. )
   TEST_LINE( "ABC" != "ABC ", .F. )
   TEST_LINE( "ABC" < "ABC ", .F. )
   TEST_LINE( "ABC" <= "ABC ", .T. )
   TEST_LINE( "ABC" > "ABC ", .F. )
   TEST_LINE( "ABC" >= "ABC ", .T. )
   TEST_LINE( "ABC " == "ABC", .F. )
   TEST_LINE( "ABC " = "ABC", .T. )
   TEST_LINE( "ABC " != "ABC", .F. )
   TEST_LINE( "ABC " < "ABC", .F. )
   TEST_LINE( "ABC " <= "ABC", .T. )
   TEST_LINE( "ABC " > "ABC", .F. )
   TEST_LINE( "ABC " >= "ABC", .T. )
   TEST_LINE( "ABC" == "DEF", .F. )
   TEST_LINE( "ABC" = "DEF", .F. )
   TEST_LINE( "ABC" != "DEF", .T. )
   TEST_LINE( "ABC" < "DEF", .T. )
   TEST_LINE( "ABC" <= "DEF", .T. )
   TEST_LINE( "ABC" > "DEF", .F. )
   TEST_LINE( "ABC" >= "DEF", .F. )
   TEST_LINE( "DEF" == "ABC", .F. )
   TEST_LINE( "DEF" = "ABC", .F. )
   TEST_LINE( "DEF" != "ABC", .T. )
   TEST_LINE( "DEF" < "ABC", .F. )
   TEST_LINE( "DEF" <= "ABC", .F. )
   TEST_LINE( "DEF" > "ABC", .T. )
   TEST_LINE( "DEF" >= "ABC", .T. )
   TEST_LINE( "ABC" == "DEFG", .F. )
   TEST_LINE( "ABC" = "DEFG", .F. )
   TEST_LINE( "ABC" != "DEFG", .T. )
   TEST_LINE( "ABC" < "DEFG", .T. )
   TEST_LINE( "ABC" <= "DEFG", .T. )
   TEST_LINE( "ABC" > "DEFG", .F. )
   TEST_LINE( "ABC" >= "DEFG", .F. )
   TEST_LINE( "DEFG" == "ABC", .F. )
   TEST_LINE( "DEFG" = "ABC", .F. )
   TEST_LINE( "DEFG" != "ABC", .T. )
   TEST_LINE( "DEFG" < "ABC", .F. )
   TEST_LINE( "DEFG" <= "ABC", .F. )
   TEST_LINE( "DEFG" > "ABC", .T. )
   TEST_LINE( "DEFG" >= "ABC", .T. )
   TEST_LINE( "ABCD" == "DEF", .F. )
   TEST_LINE( "ABCD" = "DEF", .F. )
   TEST_LINE( "ABCD" != "DEF", .T. )
   TEST_LINE( "ABCD" < "DEF", .T. )
   TEST_LINE( "ABCD" <= "DEF", .T. )
   TEST_LINE( "ABCD" > "DEF", .F. )
   TEST_LINE( "ABCD" >= "DEF", .F. )
   TEST_LINE( "DEF" == "ABCD", .F. )
   TEST_LINE( "DEF" = "ABCD", .F. )
   TEST_LINE( "DEF" != "ABCD", .T. )
   TEST_LINE( "DEF" < "ABCD", .F. )
   TEST_LINE( "DEF" <= "ABCD", .F. )
   TEST_LINE( "DEF" > "ABCD", .T. )
   TEST_LINE( "DEF" >= "ABCD", .T. )

   SET( _SET_EXACT, old_exact )
   RETURN NIL

FUNCTION New_STRINGS()

#ifdef __HARBOUR__

   TEST_LINE( HB_ValToStr( 4 )                   , "         4"    )
   TEST_LINE( HB_ValToStr( 4.0 / 2 )             , "         2.00" )
   TEST_LINE( HB_ValToStr( "String" )            , "String"        )
   TEST_LINE( HB_ValToStr( SToD( "20010101" ) )  , "2001.01.01"    )
   TEST_LINE( HB_ValToStr( NIL )                 , "NIL"           )
   TEST_LINE( HB_ValToStr( .F. )                 , ".F."           )
   TEST_LINE( HB_ValToStr( .T. )                 , ".T."           )

#endif

   RETURN NIL

FUNCTION Long_STRINGS()

   TEST_LINE( RIGHT( SPACE( 64 * 1024 - 5 ) + "12345 7890", 10                      ), "12345 7890"                                 )
   TEST_LINE( LEN( SPACE( 81910 ) + "1234567890"                                    ), 81920                                        )
   TEST_LINE( ( "1234567890" + SPACE( 810910 ) ) - ( "1234567890" + SPACE( 810910 ) ), "12345678901234567890" + SPACE( 810910 * 2 ) )

   RETURN NIL

/* Don't change the position of this #include. */
#include "rt_init.ch"
