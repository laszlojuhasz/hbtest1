/*
 * $Id$
 */

/*
 * Harbour Project source code:
 * Base Object from wich all object finally inherit
 *
 * Copyright 2000 JfL&RaC <jfl@mafact.com>, <rac@mafact.com>
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
 * MERCHANTABILITY or FITNESS for A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA (or visit
 * their web site at http://www.gnu.org/).
 *
 */

/*
 * The following parts are Copyright of the individual authors.
 * www - http://www.harbour-project.org
 *
 * Copyright 2000 J. Lefebvre <jfl@mafact.com> & RA. Cuylen <rac@mafact.com>
 *    Now supporting of New and Init method as Class(y) use it
 *    So oMyObj:new(Var1, Var2) will call oMyObj:Init(Var1, Var2)
 *    Currently limited to 20 params
 *
 * See doc/license.txt for licensing terms.
 *
 */

/* WARNING: Can not use the preprocessor      */
/* otherwise it will auto inherit from itself */

#include "common.ch"
#include "hboo.ch"

FUNCTION TObject()
   STATIC s_oClass
   LOCAL nScope := 1

   IF s_oClass == NIL

      s_oClass := TClass():New( "TObject", {} )

      s_oClass:AddInline( "CLASSNAME"      , {| Self | __OBJGETCLSNAME( Self )     }, nScope )
      s_oClass:AddInline( "CLASSH"         , {| Self | __CLASSH( Self )            }, nScope )
      s_oClass:AddInline( "CLASSSEL"       , {| Self | __CLASSSEL( Self:CLASSH() ) }, nScope )

      s_oClass:AddMethod( "NEW" , @TObject_New() , nScope )
      s_oClass:AddMethod( "INIT", @TObject_Init(), nScope )

      /* For later use */
      /*s_oClass:AddInline( "CLASS"   , {|| s_oClass }, nScope )*/

      /*s_oClass:AddInline( "EVAL"           , {| Self | __EVAL( Self )             }, nScope ) */
      /*s_oClass:AddInline( "ISDERIVEDFROM"  , {| Self, xPar1 | __ObjDerivedFrom( Self, xPar1 ) }, nScope ) */

      /* Those one exist within Class(y), so we will probably try to implement it               */

      /*s_oClass:AddInline( "MSGNOTFOUND"    , {| Self |                            }, nScope ) */

      /*s_oClass:AddInline( "ISKINDOF"       , {| Self |                            }, nScope ) */
      /*s_oClass:AddInline( "asString"       , {| Self | ::class:name + " object"   }, nScope ) */
      /*s_oClass:AddInline( "asExpStr"       , {| Self |                            }, nScope ) */
      /*s_oClass:AddInline( "basicSize"      , {| Self | Len( Self )                }, nScope ) */
      /*s_oClass:AddInline( "become"         , {| Self |                            }, nScope ) */
      /*s_oClass:AddInline( "isEqual"        , {| Self |                            }, nScope ) */
      /*s_oClass:AddInline( "isScalar"       , {| Self |                            }, nScope ) */
      /*s_oClass:AddInline( "copy"           , {| Self |                            }, nScope ) */
      /*s_oClass:AddInline( "deepCopy"       , {| Self |                            }, nScope ) */

      /*s_oClass:AddInline( "deferred"       , {| Self |                            }, nScope ) */

      /*s_oClass:AddInline( "exec"           , {| Self |                            }, nScope ) */
      /*s_oClass:AddInline( "error           , {| Self |                            }, nScope ) */
      /*s_oClass:AddInline( "hash"           , {| Self |                            }, nScope ) */
      /*s_oClass:AddInline( "null"           , {| Self |                            }, nScope ) */
      /*s_oClass:AddInline( "size"           , {| Self | Len( Self )                }, nScope ) */

      /* Those three are already treated within Classes.c */
      /*s_oClass:AddInline( "protectErr"     , {| Self |                            }, nScope ) */
      /*s_oClass:AddInline( "hiddenErr"      , {| Self |                            }, nScope ) */
      /*s_oClass:AddInline( "readOnlyErr"    , {| Self |                            }, nScope ) */

      /* No idea when those two could occur !!? */
      /*s_oClass:AddInline( "wrongClass"     , {| Self |                            }, nScope ) */
      /*s_oClass:AddInline( "badMethod"      , {| Self |                            }, nScope ) */

      /* this one exist within VO and seem to be Auto Called when object ran out of scope */
      /*s_oClass:AddInline( "Axit"           , {| Self |  }, nScope ) */

      s_oClass:Create()

      /* For later use */
      /*s_oClass:InitClass()*/

   ENDIF

   RETURN s_oClass:Instance()


/* Currently limited to 20 param */
/* Will be re-written in C later to avoid this */

static function TObject_New(xPar0, xPar1, xPar2, xPar3, xPar4, xPar5, xPar6, xPar7, xPar8, xPar9, ;
                            xPar10,xPar11,xPar12,xPar13,xPar14,xPar15,xPar16,xPar17,xPar18,xPar19 )
local Self := QSelf()

return Self:Init(xPar0, xPar1, xPar2, xPar3, xPar4, xPar5, xPar6, xPar7, xPar8, xPar9, ;
                 xPar10,xPar11,xPar12,xPar13,xPar14,xPar15,xPar16,xPar17,xPar18,xPar19 )


static function TObject_Init()
local Self := QSelf()

return Self


