/*
 * $Id$
 */

/*
 * Harbour Project source code:
 * Base Class for internal handling of class creation
 *
 * Copyright 1999 Antonio Linares <alinares@fivetech.com>
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

/*
 * The following parts are Copyright of the individual authors.
 * www - http://www.harbour-project.org
 *
 * Copyright 2000 J. Lefebvre <jfl@mafact.com> & RA. Cuylen <rac@mafact.com>
 *    Multiple inheritance
 *    Support shared class DATA
 *    scoping (hidden, protected, readOnly)
 *    Use of __cls_param function to allow multiple superclass declaration
 *    Suppress of SetType and SetInit not more nedded
 *    Delegation and forwarding
 *    Preparing the InitClass class method (not working !! )
 *
 * Copyright 1999 Eddie Runia <eddie@runia.com>
 *    Support for inheritance
 *    Support for default DATA values
 *
 * See doc/license.txt for licensing terms.
 *
 */

// Harbour Class HBClass to build classes

#include "common.ch"
#include "hboo.ch"

REQUEST HBObject

FUNCTION HBClass()

   STATIC s_hClass /* NOTE: Automatically default to NIL */

   IF s_hClass == NIL
      s_hClass := __clsNew( "HBCLASS", 16,, @HBClass() )
/*    s_hClass := __clsNew( "HBCLASS", 17,, @HBClass())  */

      __clsAddMsg( s_hClass, "New"            , @New()            , HB_OO_MSG_METHOD )
      __clsAddMsg( s_hClass, "Create"         , @Create()         , HB_OO_MSG_METHOD )
      __clsAddMsg( s_hClass, "AddData"        , @AddData()        , HB_OO_MSG_METHOD )
      __clsAddMsg( s_hClass, "AddMultiData"   , @AddMultiData()   , HB_OO_MSG_METHOD )
      __clsAddMsg( s_hClass, "AddClassData"   , @AddClassData()   , HB_OO_MSG_METHOD )
      __clsAddMsg( s_hClass, "AddMultiClsData", @AddMultiClsData(), HB_OO_MSG_METHOD )
      __clsAddMsg( s_hClass, "AddInline"      , @AddInline()      , HB_OO_MSG_METHOD )
      __clsAddMsg( s_hClass, "AddMethod"      , @AddMethod()      , HB_OO_MSG_METHOD )
      __clsAddMsg( s_hClass, "AddClsMethod"   , @AddClsMethod()   , HB_OO_MSG_METHOD )
      __clsAddMsg( s_hClass, "AddVirtual"     , @AddVirtual()     , HB_OO_MSG_METHOD )
      __clsAddMsg( s_hClass, "AddDelegate"    , @AddDelegate()    , HB_OO_MSG_METHOD )
      __clsAddMsg( s_hClass, "AddFriendFunc"  , @AddFriendFunc()  , HB_OO_MSG_METHOD )
      __clsAddMsg( s_hClass, "AddFriendClass" , @AddFriendClass() , HB_OO_MSG_METHOD )
      __clsAddMsg( s_hClass, "Instance"       , @Instance()       , HB_OO_MSG_METHOD )
      __clsAddMsg( s_hClass, "SetOnError"     , @SetOnError()     , HB_OO_MSG_METHOD )
      __clsAddMsg( s_hClass, "SetDestructor"  , @SetDestructor()  , HB_OO_MSG_METHOD )
      __clsAddMsg( s_hClass, "InitClass"      , @InitClass()      , HB_OO_MSG_METHOD )
      __clsAddMsg( s_hClass, "cSuper"         , {| Self | iif( Empty( ::asSuper ), NIL, ::asSuper[ 1 ]:name ) }, HB_OO_MSG_INLINE )
      __clsAddMsg( s_hClass, "hClass"         ,  1, HB_OO_MSG_ACCESS )
      __clsAddMsg( s_hClass, "_hClass"        ,  1, HB_OO_MSG_ASSIGN )
      __clsAddMsg( s_hClass, "cName"          ,  2, HB_OO_MSG_ACCESS )
      __clsAddMsg( s_hClass, "_cName"         ,  2, HB_OO_MSG_ASSIGN )
      __clsAddMsg( s_hClass, "aDatas"         ,  3, HB_OO_MSG_ACCESS )
      __clsAddMsg( s_hClass, "_aDatas"        ,  3, HB_OO_MSG_ASSIGN )
      __clsAddMsg( s_hClass, "aMethods"       ,  4, HB_OO_MSG_ACCESS )
      __clsAddMsg( s_hClass, "_aMethods"      ,  4, HB_OO_MSG_ASSIGN )
      __clsAddMsg( s_hClass, "aClsDatas"      ,  5, HB_OO_MSG_ACCESS )
      __clsAddMsg( s_hClass, "_aClsDatas"     ,  5, HB_OO_MSG_ASSIGN )
      __clsAddMsg( s_hClass, "aClsMethods"    ,  6, HB_OO_MSG_ACCESS )
      __clsAddMsg( s_hClass, "_aClsMethods"   ,  6, HB_OO_MSG_ASSIGN )
      __clsAddMsg( s_hClass, "aInlines"       ,  7, HB_OO_MSG_ACCESS )
      __clsAddMsg( s_hClass, "_aInlines"      ,  7, HB_OO_MSG_ASSIGN )
      __clsAddMsg( s_hClass, "aVirtuals"      ,  8, HB_OO_MSG_ACCESS )
      __clsAddMsg( s_hClass, "_aVirtuals"     ,  8, HB_OO_MSG_ASSIGN )

      __clsAddMsg( s_hClass, "aDelegates"     ,  9, HB_OO_MSG_ACCESS )
      __clsAddMsg( s_hClass, "_aDelegates"    ,  9, HB_OO_MSG_ASSIGN )
      __clsAddMsg( s_hClass, "asSuper"        , 10, HB_OO_MSG_ACCESS )
      __clsAddMsg( s_hClass, "_asSuper"       , 10, HB_OO_MSG_ASSIGN )
      __clsAddMsg( s_hClass, "nOnError"       , 11, HB_OO_MSG_ACCESS )
      __clsAddMsg( s_hClass, "_nOnError"      , 11, HB_OO_MSG_ASSIGN )
      __clsAddMsg( s_hClass, "nDestructor"    , 12, HB_OO_MSG_ACCESS )
      __clsAddMsg( s_hClass, "_nDestructor"   , 12, HB_OO_MSG_ASSIGN )
      __clsAddMsg( s_hClass, "lModFriendly"   , 13, HB_OO_MSG_ACCESS )
      __clsAddMsg( s_hClass, "_lModFriendly"  , 13, HB_OO_MSG_ASSIGN )
      __clsAddMsg( s_hClass, "asFriendClass"  , 14, HB_OO_MSG_ACCESS )
      __clsAddMsg( s_hClass, "_asFriendClass" , 14, HB_OO_MSG_ASSIGN )
      __clsAddMsg( s_hClass, "asFriendFunc"   , 15, HB_OO_MSG_ACCESS )
      __clsAddMsg( s_hClass, "_asFriendFunc"  , 15, HB_OO_MSG_ASSIGN )
      __clsAddMsg( s_hClass, "sClassFunc"     , 16, HB_OO_MSG_ACCESS )
      __clsAddMsg( s_hClass, "_sClassFunc"    , 16, HB_OO_MSG_ASSIGN )
  /*  __clsAddMsg( s_hClass, "class"          , 17, HB_OO_MSG_ACCESS )
      __clsAddMsg( s_hClass, "_class"         , 17, HB_OO_MSG_ASSIGN ) */

   ENDIF

   RETURN __clsInst( s_hClass )

//----------------------------------------------------------------------------//

// xSuper is used here as the new preprocessor file (HBCLASS.CH) send here
// always an array (if no superclass, this will be an empty one)
// In case of direct class creation (without the help of preprocessor) xSuper can be
// either NIL or contain the name of the superclass.

STATIC FUNCTION New( cClassName, xSuper, sClassFunc, lModuleFriendly )

   LOCAL Self := QSelf()
   LOCAL nSuper, i

   DEFAULT lModuleFriendly TO .F.

   IF Empty( xSuper )
      ::asSuper := {}
   ELSEIF VALTYPE( xSuper ) == "C"
      ::asSuper := { __DynsN2Sym( xSuper ) }
   ELSEIF VALTYPE( xSuper ) == "S"
      ::asSuper := { xSuper }
   ELSEIF VALTYPE( xSuper ) == "A"
      ::asSuper := {}
      nSuper := Len( xSuper )
      FOR i := 1 TO nSuper
         IF !Empty( xSuper[ i ] )
            IF VALTYPE( xSuper[ i ] ) == "C"
               AADD( ::asSuper, __DynsN2Sym( xSuper[ i ] ) )
            ELSEIF VALTYPE( xSuper[ i ] ) == "S"
               AADD( ::asSuper, xSuper[ i ] )
            ENDIF
         ENDIF
      NEXT
   ENDIF

   ::cName         := Upper( cClassName )
   ::sClassFunc    := sClassFunc
   ::lModFriendly  := lModuleFriendly

   ::aDatas        := {}
   ::aMethods      := {}
   ::aClsDatas     := {}
   ::aClsMethods   := {}
   ::aInlines      := {}
   ::aVirtuals     := {}
   ::aDelegates    := {}
   ::asFriendClass := {}
   ::asFriendFunc  := {}

   RETURN QSelf()

//----------------------------------------------------------------------------//

/* STATIC PROCEDURE Create(MetaClass) */
STATIC PROCEDURE Create()

   LOCAL Self := QSelf()
   LOCAL n
   LOCAL nLen
   LOCAL nLenDatas := Len( ::aDatas ) //Datas local to the class !!
   LOCAL nClassBegin
   LOCAL hClass
   LOCAL ahSuper := {}

/* Self:Class := MetaClass */

   nLen := Len( ::asSuper )
   FOR n := 1 TO nLen
      hClass := __clsInstSuper( ::asSuper[ n ] ) // Super handle available
      IF hClass != 0
         AAdd( ahSuper, hClass )
      ENDIF
   NEXT

   hClass := __clsNew( ::cName, nLenDatas, ahSuper, ::sClassFunc, ::lModFriendly )
   ::hClass := hClass

   IF !EMPTY( ahSuper )
      IF ahSuper[ 1 ] != 0
         __clsAddMsg( hClass, "SUPER"  , 0, HB_OO_MSG_SUPER, ahSuper[ 1 ], HB_OO_CLSTP_EXPORTED )
         __clsAddMsg( hClass, "__SUPER", 0, HB_OO_MSG_SUPER, ahSuper[ 1 ], HB_OO_CLSTP_EXPORTED )
      ENDIF
   ENDIF
   __clsAddMsg( hClass, "REALCLASS" , 0, HB_OO_MSG_REALCLASS, 0     , HB_OO_CLSTP_EXPORTED )

   // We will work here on the MetaClass object to add the Class Method
   // as needed
   //nLen := Len( ::aClsMethods )
   //FOR n := 1 TO nLen
   // // do it
   //NEXT
   ////

   //Local message...

   FOR n := 1 TO nLenDatas
      __clsAddMsg( hClass, ::aDatas[ n ][ HB_OO_DATA_SYMBOL ]       , n, ;
                   HB_OO_MSG_ACCESS, ::aDatas[ n ][ HB_OO_DATA_VALUE ], ::aDatas[ n ][ HB_OO_DATA_SCOPE ] )
      __clsAddMsg( hClass, "_" + ::aDatas[ n ][ HB_OO_DATA_SYMBOL ] , n, ;
                   HB_OO_MSG_ASSIGN, ::aDatas[ n ][ HB_OO_DATA_TYPE ] , ::aDatas[ n ][ HB_OO_DATA_SCOPE ] )
   NEXT

   nLen := Len( ::aMethods )
   FOR n := 1 TO nLen
      __clsAddMsg( hClass, ::aMethods[ n ][ HB_OO_MTHD_SYMBOL ], ::aMethods[ n ][ HB_OO_MTHD_PFUNCTION ],;
                   HB_OO_MSG_METHOD, NIL, ::aMethods[ n ][ HB_OO_MTHD_SCOPE ] )
   NEXT

   nLen := Len( ::aClsDatas )
   nClassBegin := __CLS_CNTCLSDATA( hClass )
   FOR n := 1 TO nLen
      __clsAddMsg( hClass, ::aClsDatas[ n ][ HB_OO_CLSD_SYMBOL ]      , n + nClassBegin,;
                   HB_OO_MSG_CLSACCESS, ::aClsDatas[ n ][ HB_OO_CLSD_VALUE ], ::aClsDatas[ n ][ HB_OO_CLSD_SCOPE ] )
      __clsAddMsg( hClass, "_" + ::aClsDatas[ n ][ HB_OO_CLSD_SYMBOL ], n + nClassBegin,;
                   HB_OO_MSG_CLSASSIGN,                                     , ::aClsDatas[ n ][ HB_OO_CLSD_SCOPE ] )
   NEXT

   nLen := Len( ::aInlines )
   FOR n := 1 TO nLen
      __clsAddMsg( hClass, ::aInlines[ n ][ HB_OO_MTHD_SYMBOL ], ::aInlines[ n ][ HB_OO_MTHD_PFUNCTION ],;
                   HB_OO_MSG_INLINE, NIL, ::aInlines[ n ][ HB_OO_MTHD_SCOPE ] )
   NEXT

   nLen := Len( ::aVirtuals )
   FOR n := 1 TO nLen
      __clsAddMsg( hClass, ::aVirtuals[ n ], n, HB_OO_MSG_VIRTUAL )
   NEXT

   IF ::nOnError != NIL
      __clsAddMsg( hClass, "__OnError", ::nOnError, HB_OO_MSG_ONERROR )
   ENDIF

   IF ::nDestructor != NIL
      __clsAddMsg( hClass, "__Destructor", ::nDestructor, HB_OO_MSG_DESTRUCTOR )
   ENDIF

   //Friend Classes
   nLen := Len( ::asFriendClass )
   FOR n := 1 TO nLen
      __clsAddFriend( ::hClass, ::asFriendClass[ n ] )
   NEXT

   //Friend Functions
   nLen := Len( ::asFriendFunc )
   FOR n := 1 TO nLen
      __clsAddFriend( ::hClass, ::asFriendFunc[ n ] )
   NEXT

   RETURN

//----------------------------------------------------------------------------//

STATIC FUNCTION Instance()
 LOCAL Self := QSelf()
 Local oInstance := __clsInst( ::hClass )
 /*oInstance:Class := Self:Class*/
RETURN oInstance

//----------------------------------------------------------------------------//

STATIC PROCEDURE AddData( cData, xInit, cType, nScope, lNoinit )

   DEFAULT lNoInit TO .F.
   DEFAULT nScope TO HB_OO_CLSTP_EXPORTED

   // Default Init for Logical and numeric
   IF ! lNoInit .AND. cType != NIL .AND. xInit == NIL
      IF Upper( Left( cType, 1 ) ) == "L"
         xInit := .F.
      ELSEIF Upper( Left( cType, 1 ) ) $ "NI"   /* Numeric Int */
         xInit := 0
      ENDIF
   ENDIF

   AAdd( QSelf():aDatas, { cData, xInit, cType, nScope } )

   RETURN

//----------------------------------------------------------------------------//

STATIC PROCEDURE AddMultiData( cType, xInit, nScope, aData, lNoInit )

   LOCAL i
   LOCAL nParam := Len( aData )

   FOR i := 1 TO nParam
      IF VALTYPE( aData[ i ] ) == "C"
         QSelf():AddData( aData[ i ], xInit, cType, nScope, lNoInit )
      ENDIF
   NEXT

   RETURN

//----------------------------------------------------------------------------//

STATIC PROCEDURE AddClassData( cData, xInit, cType, nScope, lNoInit )

   LOCAL c

   DEFAULT lNoInit TO .F.

   // Default Init for Logical and numeric
   IF ! lNoInit .AND. cType != NIL .AND. xInit == NIL
      c := Upper( Left( cType, 1 ) )
      IF c == "L"       /* Logical */
         xInit := .F.
      ELSEIF c $ "NI"   /* Numeric or Integer */
         xInit := 0
      ENDIF
   ENDIF

   AAdd( QSelf():aClsDatas, { cData, xInit, cType, nScope } )

   RETURN

//----------------------------------------------------------------------------//

STATIC PROCEDURE AddMultiClsData( cType, xInit, nScope, aData, lNoInit )

   LOCAL i
   LOCAL nParam := Len( aData )

   FOR i := 1 TO nParam
      IF VALTYPE( aData[ i ] ) == "C"
         QSelf():AddClassData( aData[ i ], xInit, cType, nScope, lNoInit )
      ENDIF
   NEXT

   RETURN

//----------------------------------------------------------------------------//

STATIC PROCEDURE AddInline( cMethod, bCode, nScope )

   DEFAULT nScope TO HB_OO_CLSTP_EXPORTED

   AAdd( QSelf():aInlines, { cMethod, bCode, nScope } )

   RETURN

//----------------------------------------------------------------------------//

STATIC PROCEDURE AddMethod( cMethod, nFuncPtr, nScope )

   DEFAULT nScope TO HB_OO_CLSTP_EXPORTED

   AAdd( QSelf():aMethods, { cMethod, nFuncPtr, nScope } )

   RETURN

//----------------------------------------------------------------------------//

STATIC PROCEDURE AddClsMethod( cMethod, nFuncPtr, nScope )

   AAdd( QSelf():aClsMethods, { cMethod, nFuncPtr, nScope } )

   RETURN

//----------------------------------------------------------------------------//

STATIC PROCEDURE AddVirtual( cMethod )

   AAdd( QSelf():aVirtuals, cMethod )

   RETURN

STATIC PROCEDURE AddDelegate( xMethod, nAccScope, nAsgScope, cType, cDelegMsg, cDelegClass )

   LOCAL i

   IF VALTYPE( xMethod ) == "C"
      AAdd( QSelf():aDelegates, { xMethod, nAccScope, nAsgScope, cType, cDelegMsg, cDelegClass } )
   ELSEIF VALTYPE( xMethod ) == "A"
      FOR i := 1 TO LEN( xMethod )
         AAdd( QSelf():aDelegates, { xMethod[ i ], nAccScope, nAsgScope, cType, cDelegMsg, cDelegClass } )
      NEXT
   ENDIF

   RETURN

//----------------------------------------------------------------------------//

STATIC PROCEDURE AddFriendClass( ... )

   LOCAL Self := QSelf()

   AEval( HB_AParams(), { | sClass | AAdd( ::asFriendClass, sClass ) } )

   RETURN

//----------------------------------------------------------------------------//

STATIC PROCEDURE AddFriendFunc( ... )

   LOCAL Self := QSelf()

   AEval( HB_AParams(), { | sFunc | AAdd( ::asFriendFunc, sFunc ) } )

   RETURN

//----------------------------------------------------------------------------//

STATIC PROCEDURE SetOnError( nFuncPtr )

   QSelf():nOnError := nFuncPtr

   RETURN

STATIC PROCEDURE SetDestructor( nFuncPtr )

   QSelf():nDestructor := nFuncPtr

   RETURN

//----------------------------------------------------------------------------//

STATIC FUNCTION InitClass()

   RETURN QSelf()

//----------------------------------------------------------------------------//
