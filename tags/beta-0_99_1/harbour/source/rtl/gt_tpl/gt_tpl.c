/*
 * $Id$
 */

/*
 * Harbour Project source code:
 * {Video subsystem template}
 *
 * Copyright 1999 {list of individual authors and e-mail addresses}
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

/* NOTE: User programs should never call this layer directly! */

/* TODO: include any standard headers here */

#include "hbgtcore.h"
#include "hbinit.h"

static HB_GT_FUNCS SuperTable;
#define HB_GTSUPER (&SuperTable)

static void hb_gt_tpl_Init( FHANDLE hFilenoStdin, FHANDLE hFilenoStdout, FHANDLE hFilenoStderr )
{
   HB_TRACE( HB_TR_DEBUG, ( "hb_gt_tpl_Init(%p,%p,%p)", hFilenoStdin, hFilenoStdout, hFilenoStderr ) );

   /* TODO: */

   HB_GTSUPER_INIT( hFilenoStdin, hFilenoStdout, hFilenoStderr );
}

static void hb_gt_tpl_Exit( void )
{
   HB_TRACE(HB_TR_DEBUG, ("hb_gt_tpl_Exit()"));

   HB_GTSUPER_EXIT();

   /* TODO: */
}


static int hb_gt_tpl_ReadKey( int iEventMask )
{
   HB_TRACE(HB_TR_DEBUG, ("hb_gt_tpl_ReadKey(%d)", iEventMask));

   HB_SYMBOL_UNUSED( iEventMask );

   /* TODO: check the input queue (incoming mouse and keyboard events)
            and return the inkey code if any */

   return 0;
}

static char * hb_gt_tpl_Version( int iType )
{
   HB_TRACE( HB_TR_DEBUG, ( "hb_gt_tpl_Version(%d)", iType ) );

   if( iType == 0 )
      return HB_GT_DRVNAME( HB_GT_NAME );

   return "Harbour Terminal: (template)";
}

static BOOL hb_gt_tpl_SetMode( int iRows, int iCols )
{
   HB_TRACE(HB_TR_DEBUG, ("hb_gt_tpl_SetMode(%d, %d)", iRows, iCols));

   HB_SYMBOL_UNUSED( iRows );
   HB_SYMBOL_UNUSED( iCols );

   /* TODO: if possible change the size of the screen and return TRUE */

   return FALSE;
}

static void hb_gt_tpl_Redraw( int iRow, int iCol, int iSize )
{
   BYTE bColor, bAttr;
   USHORT usChar;

   HB_TRACE( HB_TR_DEBUG, ( "hb_gt_tpl_Redraw(%d, %d, %d)", iRow, iCol, iSize ) );

   while( iSize-- )
   {
      if( !hb_gt_GetScrChar( iRow, iCol, &bColor, &bAttr, &usChar ) )
         break;
      /* TODO: display usChar at iRow, iCol position with color bColor */
      ++iCol;
   }
}

static void hb_gt_tpl_Refresh( void )
{
   int iRow, iCol, iStyle;

   HB_TRACE( HB_TR_DEBUG, ( "hb_gt_tpl_Refresh()" ) );

   HB_GTSUPER_REFRESH();
   hb_gt_GetScrCursor( &iRow, &iCol, &iStyle );

   /* TODO: set cursor position and shape */
}


/* *********************************************************************** */

static BOOL hb_gt_FuncInit( PHB_GT_FUNCS pFuncTable )
{
   HB_TRACE(HB_TR_DEBUG, ("hb_gt_FuncInit(%p)", pFuncTable));

   pFuncTable->Init                       = hb_gt_tpl_Init;
   pFuncTable->Exit                       = hb_gt_tpl_Exit;
   pFuncTable->ReadKey                    = hb_gt_tpl_ReadKey;
   pFuncTable->Version                    = hb_gt_tpl_Version;
   pFuncTable->SetMode                    = hb_gt_tpl_SetMode;
   pFuncTable->Redraw                     = hb_gt_tpl_Redraw;
   pFuncTable->Refresh                    = hb_gt_tpl_Refresh;

   return TRUE;
}

/* ********************************************************************** */

static HB_GT_INIT gtInit = { HB_GT_DRVNAME( HB_GT_NAME ),
                             hb_gt_FuncInit,
                             HB_GTSUPER };

HB_GT_ANNOUNCE( HB_GT_NAME );

HB_CALL_ON_STARTUP_BEGIN( _hb_startup_gt_Init_ )
   hb_gtRegister( &gtInit );
HB_CALL_ON_STARTUP_END( _hb_startup_gt_Init_ )

#if defined( HB_PRAGMA_STARTUP )
   #pragma startup _hb_startup_gt_Init_
#elif defined(HB_MSC_STARTUP)
   #if _MSC_VER >= 1010
      #pragma data_seg( ".CRT$XIY" )
      #pragma comment( linker, "/Merge:.CRT=.data" )
   #else
      #pragma data_seg( "XIY" )
   #endif
   static HB_$INITSYM hb_vm_auto__hb_startup_gt_Init_ = _hb_startup_gt_Init_;
   #pragma data_seg()
#endif

/* *********************************************************************** */
