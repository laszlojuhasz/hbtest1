/*
 * $Id$
 */

/*
   Copyright(C) 1999 by Antonio Linares.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published
   by the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful, but
   WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR
   PURPOSE.  See the GNU General Public License for more details.

   You should have received a copy of the GNU General Public
   License along with this program; if not, write to:

   The Free Software Foundation, Inc.,
   675 Mass Ave, Cambridge, MA 02139, USA.

   You can contact me at: alinares@fivetech.com
 */

#ifndef HB_ITEMAPI_H_
#define HB_ITEMAPI_H_

#include "extend.h"

#define HB_EVAL_PARAM_MAX_ 10

typedef struct
{
   PHB_ITEM pItems[ HB_EVAL_PARAM_MAX_ + 1 ];
} EVALINFO, * PEVALINFO;

extern PHB_ITEM hb_evalLaunch( PEVALINFO pEvalInfo );
extern BOOL     hb_evalNew( PEVALINFO pEvalInfo, PHB_ITEM pItem );
extern BOOL     hb_evalPutParam( PEVALINFO pEvalInfo, PHB_ITEM pItem );
extern BOOL     hb_evalRelease( PEVALINFO pEvalInfo );

extern PHB_ITEM hb_itemArrayGet( PHB_ITEM pArray, ULONG ulIndex );
extern PHB_ITEM hb_itemArrayNew( ULONG ulLen );
extern PHB_ITEM hb_itemArrayPut( PHB_ITEM pArray, ULONG ulIndex, PHB_ITEM pItem );
extern ULONG    hb_itemCopyC( PHB_ITEM pItem, char *szBuffer, ULONG ulLen );
extern BOOL     hb_itemFreeC( char *szText );
extern char *   hb_itemGetC( PHB_ITEM pItem );
extern char *   hb_itemGetDS( PHB_ITEM pItem, char *szDate );
extern BOOL     hb_itemGetL( PHB_ITEM pItem );
extern double   hb_itemGetND( PHB_ITEM pItem );
extern long     hb_itemGetNL( PHB_ITEM pItem );
extern PHB_ITEM hb_itemNew( PHB_ITEM pNull );
extern PHB_ITEM hb_itemParam( WORD wParam );
extern PHB_ITEM hb_itemPutC( PHB_ITEM pItem, char *szText );
extern PHB_ITEM hb_itemPutCL( PHB_ITEM pItem, char *nszText, ULONG ulLen );
extern PHB_ITEM hb_itemPutDS( PHB_ITEM pItem, char *szDate );
extern PHB_ITEM hb_itemPutL( PHB_ITEM pItem, BOOL bValue );
extern PHB_ITEM hb_itemPutND( PHB_ITEM pItem, double dNumber );
extern PHB_ITEM hb_itemPutNL( PHB_ITEM pItem, long lNumber );
extern BOOL     hb_itemRelease( PHB_ITEM pItem );
extern PHB_ITEM hb_itemReturn( PHB_ITEM pItem );
extern ULONG    hb_itemSize( PHB_ITEM pItem );
extern WORD     hb_itemType( PHB_ITEM pItem );

#endif /* HB_ITEMAPI_H_ */
