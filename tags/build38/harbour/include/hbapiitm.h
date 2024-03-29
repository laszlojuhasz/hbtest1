/*
 * $Id$
 */

/*
 * Harbour Project source code:
 * Header file for the Item API
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

#ifndef HB_APIITM_H_
#define HB_APIITM_H_

#include "hbapi.h"

#if defined(HB_EXTERN_C)
extern "C" {
#endif

#define HB_EVAL_PARAM_MAX_ 9

typedef struct
{
   USHORT   paramCount;
   PHB_ITEM pItems[ HB_EVAL_PARAM_MAX_ + 1 ];
} EVALINFO, * PEVALINFO, * EVALINFO_PTR;

extern PHB_ITEM hb_evalLaunch   ( PEVALINFO pEvalInfo );
extern BOOL     hb_evalNew      ( PEVALINFO pEvalInfo, PHB_ITEM pItem );
extern BOOL     hb_evalPutParam ( PEVALINFO pEvalInfo, PHB_ITEM pItem );
extern BOOL     hb_evalRelease  ( PEVALINFO pEvalInfo );

extern PHB_ITEM hb_itemDo       ( PHB_ITEM pItem, ULONG ulPCount, ... );
extern PHB_ITEM hb_itemDoC      ( char * szFunc, ULONG ulPCount, ... );

extern PHB_ITEM hb_itemArrayGet ( PHB_ITEM pArray, ULONG ulIndex );
extern PHB_ITEM hb_itemArrayNew ( ULONG ulLen );
extern PHB_ITEM hb_itemArrayPut ( PHB_ITEM pArray, ULONG ulIndex, PHB_ITEM pItem );
extern ULONG    hb_itemCopyC    ( PHB_ITEM pItem, char * szBuffer, ULONG ulLen );
extern BOOL     hb_itemFreeC    ( char * szText );
extern char *   hb_itemGetC     ( PHB_ITEM pItem );
extern char *   hb_itemGetCPtr  ( PHB_ITEM pItem );
extern ULONG    hb_itemGetCLen  ( PHB_ITEM pItem );
extern char *   hb_itemGetDS    ( PHB_ITEM pItem, char * szDate );
extern long     hb_itemGetDL    ( PHB_ITEM pItem );
extern BOOL     hb_itemGetL     ( PHB_ITEM pItem );
extern double   hb_itemGetND    ( PHB_ITEM pItem );
extern int      hb_itemGetNI    ( PHB_ITEM pItem );
extern long     hb_itemGetNL    ( PHB_ITEM pItem );
extern void     hb_itemGetNLen  ( PHB_ITEM pItem, int * piWidth, int * piDec );
extern void *   hb_itemGetPtr   ( PHB_ITEM pItem );
extern PHB_ITEM hb_itemNew      ( PHB_ITEM pNull );
extern void     hb_itemInit     ( PHB_ITEM pItem );
extern USHORT   hb_itemPCount   ( void );
extern PHB_ITEM hb_itemParam    ( USHORT uiParam );
extern PHB_ITEM hb_itemPutC     ( PHB_ITEM pItem, char * szText );
extern PHB_ITEM hb_itemPutCPtr  ( PHB_ITEM pItem, char * szText, ULONG ulLen );
extern PHB_ITEM hb_itemPutCL    ( PHB_ITEM pItem, char * szText, ULONG ulLen );
extern void     hb_itemSetCMemo ( PHB_ITEM pItem );
extern PHB_ITEM hb_itemPutD     ( PHB_ITEM pItem, long lYear, long lMonth, long lDay );
extern PHB_ITEM hb_itemPutDS    ( PHB_ITEM pItem, char * szDate );
extern PHB_ITEM hb_itemPutDL    ( PHB_ITEM pItem, long lJulian );
extern PHB_ITEM hb_itemPutL     ( PHB_ITEM pItem, BOOL bValue );
extern PHB_ITEM hb_itemPutND    ( PHB_ITEM pItem, double dNumber );
extern PHB_ITEM hb_itemPutNI    ( PHB_ITEM pItem, int iNumber );
extern PHB_ITEM hb_itemPutNL    ( PHB_ITEM pItem, long lNumber );
extern PHB_ITEM hb_itemPutNLen  ( PHB_ITEM pItem, double dNumber, int iWidth, int iDec );
extern PHB_ITEM hb_itemPutNDLen ( PHB_ITEM pItem, double dNumber, int iWidth, int iDec );
extern PHB_ITEM hb_itemPutNILen ( PHB_ITEM pItem, int iNumber, int iWidth );
extern PHB_ITEM hb_itemPutNLLen ( PHB_ITEM pItem, long lNumber, int iWidth );
extern PHB_ITEM hb_itemPutPtr   ( PHB_ITEM pItem, void * pValue );
extern BOOL     hb_itemRelease  ( PHB_ITEM pItem );
extern PHB_ITEM hb_itemReturn   ( PHB_ITEM pItem );
extern ULONG    hb_itemSize     ( PHB_ITEM pItem );
extern USHORT   hb_itemType     ( PHB_ITEM pItem );
extern char *   hb_itemTypeStr  ( PHB_ITEM pItem );

/* Non Clipper compliant internal API */

extern PHB_ITEM hb_itemParamPtr ( USHORT uiParam, int iMask );
extern PHB_ITEM hb_itemReturnPtr( void );
extern int      hb_itemStrCmp   ( PHB_ITEM pFirst, PHB_ITEM pSecond, BOOL bForceExact ); /* our string compare */
extern void     hb_itemCopy     ( PHB_ITEM pDest, PHB_ITEM pSource ); /* copies an item to one place to another respecting its containts */
extern void     hb_itemClear    ( PHB_ITEM pItem );
extern PHB_ITEM hb_itemUnRef    ( PHB_ITEM pItem ); /* de-references passed variable */
extern char *   hb_itemStr      ( PHB_ITEM pNumber, PHB_ITEM pWidth, PHB_ITEM pDec ); /* convert a number to a string */
extern char *   hb_itemString   ( PHB_ITEM pItem, ULONG * ulLen, BOOL * bFreeReq );  /* Convert any scalar to a string */
extern PHB_ITEM hb_itemValToStr ( PHB_ITEM pItem ); /* Convert any scalar to a string */
extern char *   hb_itemPadConv  ( PHB_ITEM pItem, char * buffer, ULONG * pulSize );
extern void     hb_itemSwap     ( PHB_ITEM pItem1, PHB_ITEM pItem2 );

#if defined(HB_EXTERN_C)
}
#endif

#endif /* HB_APIITM_H_ */
