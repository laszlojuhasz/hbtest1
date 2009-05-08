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
 *    Preparing the InitClass class method (not working !!)
 *
 * Copyright 1999 Eddie Runia <eddie@runia.com>
 *    Support for inheritance
 *    Support for default DATA values
 *
 * See COPYING for licensing terms.
 *
 */

/* NOTE: This .prg is also used by the debugger subsystem,
         therefore we need this switch to avoid an infinite
         loop when launching it. [vszakats] */
#pragma DEBUGINFO=OFF

/* Harbour Class HBClass to build classes */

#include "common.ch"
#include "hboo.ch"

REQUEST HBObject

FUNCTION HBClass()

   STATIC s_hClass /* NOTE: Automatically defaults to NIL */

   LOCAL hClass

   IF s_hClass == NIL .AND. __clsLockDef( @s_hClass )

      BEGIN SEQUENCE

         hClass := __clsNew( "HBCLASS", 16,, @HBClass() )
/*       hClass := __clsNew( "HBCLASS", 17,, @HBClass())  */

         __clsAddMsg( hClass, "New"            , @New()            , HB_OO_MSG_METHOD )
         __clsAddMsg( hClass, "Create"         , @Create()         , HB_OO_MSG_METHOD )
         __clsAddMsg( hClass, "AddData"        , @AddData()        , HB_OO_MSG_METHOD )
         __clsAddMsg( hClass, "AddMultiData"   , @AddMultiData()   , HB_OO_MSG_METHOD )
         __clsAddMsg( hClass, "AddClassData"   , @AddClassData()   , HB_OO_MSG_METHOD )
         __clsAddMsg( hClass, "AddMultiClsData", @AddMultiClsData(), HB_OO_MSG_METHOD )
         __clsAddMsg( hClass, "AddInline"      , @AddInline()      , HB_OO_MSG_METHOD )
         __clsAddMsg( hClass, "AddMethod"      , @AddMethod()      , HB_OO_MSG_METHOD )
         __clsAddMsg( hClass, "AddClsMethod"   , @AddClsMethod()   , HB_OO_MSG_METHOD )
         __clsAddMsg( hClass, "AddVirtual"     , @AddVirtual()     , HB_OO_MSG_METHOD )
         __clsAddMsg( hClass, "AddDelegate"    , @AddDelegate()    , HB_OO_MSG_METHOD )
         __clsAddMsg( hClass, "AddFriendFunc"  , @AddFriendFunc()  , HB_OO_MSG_METHOD )
         __clsAddMsg( hClass, "AddFriendClass" , @AddFriendClass() , HB_OO_MSG_METHOD )
         __clsAddMsg( hClass, "Instance"       , @Instance()       , HB_OO_MSG_METHOD )
         __clsAddMsg( hClass, "SetOnError"     , @SetOnError()     , HB_OO_MSG_METHOD )
         __clsAddMsg( hClass, "SetDestructor"  , @SetDestructor()  , HB_OO_MSG_METHOD )
         __clsAddMsg( hClass, "InitClass"      , @InitClass()      , HB_OO_MSG_METHOD )
         __clsAddMsg( hClass, "cSuper"         , {| Self | iif( Empty( ::asSuper ), NIL, ::asSuper[ 1 ]:name ) }, HB_OO_MSG_INLINE )
         __clsAddMsg( hClass, "hClass"         ,  1, HB_OO_MSG_ACCESS )
         __clsAddMsg( hClass, "_hClass"        ,  1, HB_OO_MSG_ASSIGN )
         __clsAddMsg( hClass, "cName"          ,  2, HB_OO_MSG_ACCESS )
         __clsAddMsg( hClass, "_cName"         ,  2, HB_OO_MSG_ASSIGN )
         __clsAddMsg( hClass, "aDatas"         ,  3, HB_OO_MSG_ACCESS )
         __clsAddMsg( hClass, "_aDatas"        ,  3, HB_OO_MSG_ASSIGN )
         __clsAddMsg( hClass, "aMethods"       ,  4, HB_OO_MSG_ACCESS )
         __clsAddMsg( hClass, "_aMethods"      ,  4, HB_OO_MSG_ASSIGN )
         __clsAddMsg( hClass, "aClsDatas"      ,  5, HB_OO_MSG_ACCESS )
         __clsAddMsg( hClass, "_aClsDatas"     ,  5, HB_OO_MSG_ASSIGN )
         __clsAddMsg( hClass, "aClsMethods"    ,  6, HB_OO_MSG_ACCESS )
         __clsAddMsg( hClass, "_aClsMethods"   ,  6, HB_OO_MSG_ASSIGN )
         __clsAddMsg( hClass, "aInlines"       ,  7, HB_OO_MSG_ACCESS )
         __clsAddMsg( hClass, "_aInlines"      ,  7, HB_OO_MSG_ASSIGN )
         __clsAddMsg( hClass, "aVirtuals"      ,  8, HB_OO_MSG_ACCESS )
         __clsAddMsg( hClass, "_aVirtuals"     ,  8, HB_OO_MSG_ASSIGN )

         __clsAddMsg( hClass, "aDelegates"     ,  9, HB_OO_MSG_ACCESS )
         __clsAddMsg( hClass, "_aDelegates"    ,  9, HB_OO_MSG_ASSIGN )
         __clsAddMsg( hClass, "asSuper"        , 10, HB_OO_MSG_ACCESS )
         __clsAddMsg( hClass, "_asSuper"       , 10, HB_OO_MSG_ASSIGN )
         __clsAddMsg( hClass, "sOnError"       , 11, HB_OO_MSG_ACCESS )
         __clsAddMsg( hClass, "_sOnError"      , 11, HB_OO_MSG_ASSIGN )
         __clsAddMsg( hClass, "sDestructor"    , 12, HB_OO_MSG_ACCESS )
         __clsAddMsg( hClass, "_sDestructor"   , 12, HB_OO_MSG_ASSIGN )
         __clsAddMsg( hClass, "lModFriendly"   , 13, HB_OO_MSG_ACCESS )
         __clsAddMsg( hClass, "_lModFriendly"  , 13, HB_OO_MSG_ASSIGN )
         __clsAddMsg( hClass, "asFriendClass"  , 14, HB_OO_MSG_ACCESS )
         __clsAddMsg( hClass, "_asFriendClass" , 14, HB_OO_MSG_ASSIGN )
         __clsAddMsg( hClass, "asFriendFunc"   , 15, HB_OO_MSG_ACCESS )
         __clsAddMsg( hClass, "_asFriendFunc"  , 15, HB_OO_MSG_ASSIGN )
         __clsAddMsg( hClass, "sClassFunc"     , 16, HB_OO_MSG_ACCESS )
         __clsAddMsg( hClass, "_sClassFunc"    , 16, HB_OO_MSG_ASSIGN )
     /*  __clsAddMsg( hClass, "class"          , 17, HB_OO_MSG_ACCESS )
         __clsAddMsg( hClass, "_class"         , 17, HB_OO_MSG_ASSIGN ) */

      ALWAYS

         __clsUnlockDef( @s_hClass, hClass )

      END SEQUENCE

   ENDIF

   RETURN __clsInst( s_hClass )

/* xSuper is used here as the new preprocessor file (hbclass.ch) send here
   always an array (if no superclass, this will be an empty one)
   In case of direct class creation (without the help of preprocessor) xSuper can be
   either NIL or contain the name of the superclass. */

STATIC FUNCTION New( cClassName, xSuper, sClassFunc, lModuleFriendly )

   LOCAL Self := QSelf()
   LOCAL i

   DEFAULT lModuleFriendly TO .F.

   IF Empty( xSuper )
      ::asSuper := {}
   ELSEIF ISCHARACTER( xSuper )
      ::asSuper := { __DynsN2Sym( xSuper ) }
   ELSEIF hb_isSymbol( xSuper )
      ::asSuper := { xSuper }
   ELSEIF ISARRAY( xSuper )
      ::asSuper := {}
      FOR EACH i IN xSuper
         IF ! Empty( i )
            IF ISCHARACTER( i )
               AAdd( ::asSuper, __DynsN2Sym( i ) )
            ELSEIF hb_isSymbol( i )
               AAdd( ::asSuper, i )
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

STATIC PROCEDURE Create( /* MetaClass */ )

   LOCAL Self := QSelf()
   LOCAL n
   LOCAL nClassBegin
   LOCAL hClass
   LOCAL ahSuper := {}

/* Self:Class := MetaClass */

   FOR EACH n IN ::asSuper
      hClass := __clsInstSuper( n ) /* Super handle available */
      IF hClass != 0
         AAdd( ahSuper, hClass )
      ENDIF
   NEXT

   hClass := __clsNew( ::cName, Len( ::aDatas ), ahSuper, ::sClassFunc, ::lModFriendly )
   ::hClass := hClass

   IF ! Empty( ahSuper )
      IF ahSuper[ 1 ] != 0
         __clsAddMsg( hClass, "SUPER"  , 0, HB_OO_MSG_SUPER, ahSuper[ 1 ], HB_OO_CLSTP_EXPORTED )
         __clsAddMsg( hClass, "__SUPER", 0, HB_OO_MSG_SUPER, ahSuper[ 1 ], HB_OO_CLSTP_EXPORTED )
      ENDIF
   ENDIF
   __clsAddMsg( hClass, "REALCLASS" , 0, HB_OO_MSG_REALCLASS, 0     , HB_OO_CLSTP_EXPORTED )

   // We will work here on the MetaClass object to add the Class Method
   // as needed
   //FOR EACH n IN ::aClsMethods
   // // do it
   //NEXT
   ////

   /* local messages... */

   FOR EACH n IN ::aDatas
      __clsAddMsg( hClass, n[ HB_OO_DATA_SYMBOL ]       , n:__enumIndex(), ;
                   HB_OO_MSG_ACCESS, n[ HB_OO_DATA_VALUE ], n[ HB_OO_DATA_SCOPE ] )
      __clsAddMsg( hClass, "_" + n[ HB_OO_DATA_SYMBOL ] , n:__enumIndex(), ;
                   HB_OO_MSG_ASSIGN, n[ HB_OO_DATA_TYPE ] , n[ HB_OO_DATA_SCOPE ] )
   NEXT

   FOR EACH n IN ::aMethods
      __clsAddMsg( hClass, n[ HB_OO_MTHD_SYMBOL ], n[ HB_OO_MTHD_PFUNCTION ],;
                   HB_OO_MSG_METHOD, NIL, n[ HB_OO_MTHD_SCOPE ] )
   NEXT

   nClassBegin := __CLS_CNTCLSDATA( hClass )
   FOR EACH n IN ::aClsDatas
      __clsAddMsg( hClass, n[ HB_OO_CLSD_SYMBOL ]      , n:__enumIndex() + nClassBegin,;
                   HB_OO_MSG_CLSACCESS, n[ HB_OO_CLSD_VALUE ], n[ HB_OO_CLSD_SCOPE ] )
      __clsAddMsg( hClass, "_" + n[ HB_OO_CLSD_SYMBOL ], n:__enumIndex() + nClassBegin,;
                   HB_OO_MSG_CLSASSIGN,                      , n[ HB_OO_CLSD_SCOPE ] )
   NEXT

   FOR EACH n IN ::aInlines
      __clsAddMsg( hClass, n[ HB_OO_MTHD_SYMBOL ], n[ HB_OO_MTHD_PFUNCTION ],;
                   HB_OO_MSG_INLINE, NIL, n[ HB_OO_MTHD_SCOPE ] )
   NEXT

   FOR EACH n IN ::aVirtuals
      __clsAddMsg( hClass, n, n:__enumIndex(), HB_OO_MSG_VIRTUAL )
   NEXT

   IF ::sOnError != NIL
      __clsAddMsg( hClass, "__OnError", ::sOnError, HB_OO_MSG_ONERROR )
   ENDIF

   IF ::sDestructor != NIL
      __clsAddMsg( hClass, "__Destructor", ::sDestructor, HB_OO_MSG_DESTRUCTOR )
   ENDIF

   /* Friend Classes */
   FOR EACH n IN ::asFriendClass
      __clsAddFriend( ::hClass, n )
   NEXT

   /* Friend Functions */
   FOR EACH n IN ::asFriendFunc
      __clsAddFriend( ::hClass, n )
   NEXT

   RETURN

STATIC FUNCTION Instance()
   LOCAL Self := QSelf()

   RETURN __clsInst( ::hClass )

STATIC PROCEDURE AddData( cData, xInit, cType, nScope, lNoinit )

   DEFAULT lNoInit TO .F.
   DEFAULT nScope TO HB_OO_CLSTP_EXPORTED

   /* Default Init for Logical and numeric */
   IF ! lNoInit .AND. cType != NIL .AND. xInit == NIL
      SWITCH Upper( Left( cType, 1 ) )
      CASE "L"       /* Logical */
         xInit := .F.
         EXIT
      CASE "I"       /* Numeric or Integer */
      CASE "N"       /* Numeric or Integer */
         xInit := 0
         EXIT
      CASE "D"       /* Date */
         xInit := hb_SToD()
         EXIT
      CASE "T"       /* Timestamp */
         xInit := hb_SToT()
         EXIT
      ENDSWITCH
   ENDIF

   AAdd( QSelf():aDatas, { cData, xInit, cType, nScope } )

   RETURN

STATIC PROCEDURE AddMultiData( cType, xInit, nScope, aData, lNoInit )

   LOCAL i

   FOR EACH i IN aData
      IF ISCHARACTER( i )
         QSelf():AddData( i, xInit, cType, nScope, lNoInit )
      ENDIF
   NEXT

   RETURN

STATIC PROCEDURE AddClassData( cData, xInit, cType, nScope, lNoInit )

   DEFAULT lNoInit TO .F.
   DEFAULT nScope TO HB_OO_CLSTP_EXPORTED

   nScope := hb_bitOr( nScope, HB_OO_CLSTP_CLASS )

   /* Default Init for Logical and numeric */
   IF ! lNoInit .AND. cType != NIL .AND. xInit == NIL
      SWITCH Upper( Left( cType, 1 ) )
      CASE "L"       /* Logical */
         xInit := .F.
         EXIT
      CASE "I"       /* Numeric or Integer */
      CASE "N"       /* Numeric or Integer */
         xInit := 0
         EXIT
      CASE "D"       /* Date */
         xInit := hb_SToD()
         EXIT
      CASE "T"       /* Timestamp */
         xInit := hb_SToT()
         EXIT
      ENDSWITCH
   ENDIF

   AAdd( QSelf():aClsDatas, { cData, xInit, cType, nScope } )

   RETURN

STATIC PROCEDURE AddMultiClsData( cType, xInit, nScope, aData, lNoInit )

   LOCAL i

   FOR EACH i IN aData
      IF ISCHARACTER( i )
         QSelf():AddClassData( i, xInit, cType, nScope, lNoInit )
      ENDIF
   NEXT

   RETURN

STATIC PROCEDURE AddInline( cMethod, bCode, nScope )

   DEFAULT nScope TO HB_OO_CLSTP_EXPORTED

   AAdd( QSelf():aInlines, { cMethod, bCode, nScope } )

   RETURN

STATIC PROCEDURE AddMethod( cMethod, nFuncPtr, nScope )

   DEFAULT nScope TO HB_OO_CLSTP_EXPORTED

   AAdd( QSelf():aMethods, { cMethod, nFuncPtr, nScope } )

   RETURN

STATIC PROCEDURE AddClsMethod( cMethod, nFuncPtr, nScope )

   DEFAULT nScope TO HB_OO_CLSTP_EXPORTED

   nScope := hb_bitOr( nScope, HB_OO_CLSTP_CLASS )

   AAdd( QSelf():aClsMethods, { cMethod, nFuncPtr, nScope } )

   RETURN

STATIC PROCEDURE AddVirtual( cMethod )

   AAdd( QSelf():aVirtuals, cMethod )

   RETURN

STATIC PROCEDURE AddDelegate( xMethod, nAccScope, nAsgScope, cType, cDelegMsg, cDelegClass )

   LOCAL i

   IF ISCHARACTER( xMethod )
      AAdd( QSelf():aDelegates, { xMethod, nAccScope, nAsgScope, cType, cDelegMsg, cDelegClass } )
   ELSEIF ISARRAY( xMethod )
      FOR EACH i IN xMethod
         AAdd( QSelf():aDelegates, { i, nAccScope, nAsgScope, cType, cDelegMsg, cDelegClass } )
      NEXT
   ENDIF

   RETURN

STATIC PROCEDURE AddFriendClass( ... )

   LOCAL Self := QSelf()

   AEval( HB_AParams(), { | sClass | AAdd( ::asFriendClass, sClass ) } )

   RETURN

STATIC PROCEDURE AddFriendFunc( ... )

   LOCAL Self := QSelf()

   AEval( HB_AParams(), { | sFunc | AAdd( ::asFriendFunc, sFunc ) } )

   RETURN

STATIC PROCEDURE SetOnError( sFuncPtr )

   QSelf():sOnError := sFuncPtr

   RETURN

STATIC PROCEDURE SetDestructor( sFuncPtr )

   QSelf():sDestructor := sFuncPtr

   RETURN

STATIC FUNCTION InitClass()
   RETURN QSelf()
