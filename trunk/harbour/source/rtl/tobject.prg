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
 *    1.40 07/13/2000 JFL&RAC
 *    Now supporting of New and Init method as Class(y) use it
 *    So oMyObj:new(Var1, Var2) will call oMyObj:Init(Var1, Var2)
 *    Currently limited to 20 params
 *
 *    1.41 07/18/2000 JFL&RAC
 *    Improving class(y) compatibility
 *    adding messages :error() and ::MsgNotFound()
 *
 *
 * See doc/license.txt for licensing terms.
 *
 */

/* WARNING: Can not use the preprocessor      */
/* otherwise it will auto inherit from itself */

#include "common.ch"
#include "hboo.ch"
#include "error.ch"

FUNCTION TObject()
   STATIC s_oClass
   LOCAL nScope := 1

   IF s_oClass == NIL

      s_oClass := TClass():New( "TObject",  )

      /* Those Five worked fine but their C version from classes.c are probably better in term of speed */
      /*s_oClass:AddInline( "CLASSNAME"      , {| Self | __OBJGETCLSNAME( Self )     }, nScope ) */
      /*s_oClass:AddInline( "CLASSH"         , {| Self | __CLASSH( Self )            }, nScope ) */
      /*s_oClass:AddInline( "CLASSSEL"       , {| Self | __CLASSSEL( Self:CLASSH() ) }, nScope ) */
      /*s_oClass:AddInline( "EVAL"           , {| Self | __EVAL( Self )             }, nScope ) */
      /*s_oClass:AddInline( "ISDERIVEDFROM"  , {| Self, xPar1 | __ObjDerivedFrom( Self, xPar1 ) }, nScope ) */

      s_oClass:AddMethod( "NEW"  , @TObject_New()  , nScope )
      s_oClass:AddMethod( "INIT" , @TObject_Init() , nScope )

      s_oClass:AddMethod( "ERROR", @TOBJECT_ERROR() , nScope )

      s_oClass:SetOnError( @TObject_DftonError() )

      s_oClass:AddInline( "MSGNOTFOUND" , {| Self, cMsg | ::Error( "Message not found", __OBJGETCLSNAME( Self ), cMsg, iif(substr(cMsg,1,1)=="_",1005,1004) ) }, nScope )

      /*s_oClass:AddInline( "ADDMETHOD" , { | Self, cMeth, pFunc, nScopeMeth                 |  __clsAddMsg( __CLASSH( Self ) , cMeth , pFunc ,HB_OO_MSG_METHOD , NIL, iif(nScopeMeth==NIL,1,nScopeMeth) ) }, nScope )                                */
      /*s_oClass:AddInline( "ADDVAR"    , { | Self, cVAR, nScopeMeth, uiData , hClass  |  __clsAddMsg( hClass:=__CLASSH( Self ) ,     cVar , uidata := __CLS_INCDATA(hClass) , HB_OO_MSG_DATA, NIL  , iif(nScopeMeth==NIL,1,nScopeMeth) )  , ;        */
      /*                                                                               __clsAddMsg( hClass                   , "_"+cVar , uiData                          , HB_OO_MSG_DATA, NIL  , iif(nScopeMeth==NIL,1,nScopeMeth) ) }, nScope )    */

      /*s_oClass:AddInline( "CLASS"   , {| Self | Self }, nScope )*/
      /*s_oClass:AddInline( "CLASS"   , {|| s_oClass }, nScope ) */

      /* Those one exist within Class(y), so we will probably try to implement it               */

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

return QSelf():Init(xPar0, xPar1, xPar2, xPar3, xPar4, xPar5, xPar6, xPar7, xPar8, xPar9, ;
                 xPar10,xPar11,xPar12,xPar13,xPar14,xPar15,xPar16,xPar17,xPar18,xPar19 )

static function TObject_Init()
return QSelf()

static function TObject_Dftonerror(xPar0, xPar1, xPar2, xPar3, xPar4, xPar5, xPar6, xPar7, xPar8, xPar9, ;
                                   xPar10,xPar11,xPar12,xPar13,xPar14,xPar15,xPar16,xPar17,xPar18,xPar19 )
return QSelf():MSGNOTFOUND( __GetMessage(), xPar0, xPar1, xPar2, xPar3, xPar4, xPar5, xPar6, xPar7, xPar8, xPar9, ;
                                            xPar10,xPar11,xPar12,xPar13,xPar14,xPar15,xPar16,xPar17,xPar18,xPar19 )

static function TObject_Error( cDesc, cClass, cMsg, nCode )

   DEFAULT nCode TO 1004

   IF nCode == 1005
      RETURN __errRT_SBASE( EG_NOVARMETHOD, 1005, cDesc, cClass + ":" + cMsg )
   ENDIF

   RETURN __errRT_SBASE( EG_NOMETHOD, nCode, cDesc, cClass + ":" + cMsg )
