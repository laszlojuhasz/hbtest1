/*
 * $Id$
 */

/*
 * Harbour Project source code:
 * OLE library C header
 *
 * Copyright 2009 Viktor Szakats (harbour.01 syenar.hu)
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

#ifndef __HBWINOLE_H
#define __HBWINOLE_H

#include "hbapi.h"
#include "hbapiitm.h"
#include "hbapicls.h"
#include "hbapierr.h"
#include "hbvm.h"
#include "hbstack.h"
#include "hbdate.h"

#if defined( HB_OS_WIN )

/* This option can resolve compilation problems in C++ mode for some
 * compilers like OpenWatcom but not for all, f.e. it will not help
 * BCC when used with -P (C++ mode) switch.
 */
/*
#if defined( __cplusplus ) && !defined( CINTERFACE )
   #define CINTERFACE 1
#endif
*/

/* This code uses named union so this declaration is necessary for
 * compilers where nameless unions are default
 */
#if !defined( NONAMELESSUNION )
   #define NONAMELESSUNION
#endif

#include <windows.h>
#include <ole2.h>
#include <ocidl.h>

/* macros used to hide type of interface: C or C++
 */
#if defined( __cplusplus ) && !defined( CINTERFACE ) && \
   ( defined( __BORLANDC__ ) || \
     defined( _MSC_VER ) || \
     defined( __MINGW32__ ) || \
     ( defined( __WATCOMC__ ) && ( __WATCOMC__ >= 1270 ) ) )
#  define HB_ID_REF( id )     ( id )
#  define HB_VTBL( pSelf )    ( pSelf )
#  define HB_THIS( pSelf )
#  define HB_THIS_( pSelf )
#else
#  define HB_OLE_C_API        1
#  define HB_ID_REF( id )     ( &id )
#  define HB_VTBL( pSelf )    ( pSelf )->lpVtbl
#  define HB_THIS( pSelf )    ( pSelf )
#  define HB_THIS_( pSelf )   ( pSelf ),
#endif

HB_EXTERN_BEGIN

typedef HB_BOOL ( * HB_OLEOBJ_FUNC )( VARIANT*, PHB_ITEM );

extern HB_EXPORT HB_BOOL    hb_oleInit( void );
extern HB_EXPORT HRESULT    hb_oleGetError( void );
extern HB_EXPORT void       hb_oleSetError( HRESULT lOleError );
extern HB_EXPORT void       hb_oleVariantToItem( PHB_ITEM pItem, VARIANT * pVariant );
extern HB_EXPORT void       hb_oleItemToVariant( VARIANT * pVariant, PHB_ITEM pItem );
extern HB_EXPORT void       hb_oleItemToVariantEx( VARIANT* pVariant, PHB_ITEM pItem, HB_OLEOBJ_FUNC pObjFunc );
extern HB_EXPORT void       hb_oleVariantUpdate( VARIANT * pVariant, PHB_ITEM pItem, HB_OLEOBJ_FUNC pObjFunc );
extern HB_EXPORT IDispatch* hb_oleParam( int iParam );
extern HB_EXPORT IDispatch* hb_oleItemGet( PHB_ITEM pItem );
extern HB_EXPORT PHB_ITEM   hb_oleItemPut( PHB_ITEM pItem, IDispatch * pDisp );
extern HB_EXPORT PHB_ITEM   hb_oleItemGetCallBack( PHB_ITEM pItem );
extern HB_EXPORT void       hb_oleItemSetCallBack( PHB_ITEM pItem, PHB_ITEM * pCallBack );
extern HB_EXPORT HB_BOOL    hb_oleDispInvoke( PHB_SYMB pSym, PHB_ITEM pObject, PHB_ITEM pParam,
                                              DISPPARAMS * pParams, VARIANT* pVarResult,
                                              HB_OLEOBJ_FUNC pObjFunc );

/* activex control */
extern HB_EXPORT HB_BOOL    hb_oleAxInit( void );
extern HB_EXPORT PHB_ITEM   hb_oleAxControlNew( PHB_ITEM pItem, HWND hWnd );

HB_EXTERN_END

#endif

#endif /* __HBWINOLE_H */
