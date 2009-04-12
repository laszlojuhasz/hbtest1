/*
 * $Id$
 */

/*
 * Harbour Project source code:
 *
 * Copyright 2009 Pritpal Bedi <pritpal@vouchcac.com>
 *
 * Video subsystem for Windows using GUI windows instead of Console
 *     Copyright 2003 Peter Rees <peter@rees.co.nz>
 *                    Rees Software & Systems Ltd
 * based on
 *   Bcc ConIO Video subsystem by
 *     Copyright 2002 Marek Paliwoda <paliwoda@inteia.pl>
 *     Copyright 2002 Przemyslaw Czerpak <druzus@polbox.com>
 *   Video subsystem for Windows compilers
 *     Copyright 1999-2000 Paul Tucker <ptucker@sympatico.ca>
 *     Copyright 2002 Przemyslaw Czerpak <druzus@polbox.com>
 *
 * Copyright 2006 Przemyslaw Czerpak <druzus /at/ priv.onet.pl>
 *    Adopted to new GT API
 *
 * The following parts are Copyright of the individual authors.
 * www - http://www.harbour-project.org
 *
 *
 * Copyright 1999 David G. Holm <dholm@jsd-llc.com>
 *    hb_gt_Tone()
 *
 * See COPYING for licensing terms.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2, or (at your option )
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this software; see the file COPYING.   If not, write to
 * the Free Software Foundation, Inc., 59 Temple Place, Suite 330,
 * Boston, MA 02111-1307 USA (or visit the web site http://www.gnu.org/ ).
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
 * not apply to the code that you add in this way.   To avoid misleading
 * anyone as to the status of such modified files, you must delete
 * this exception notice from them.
 *
 * If you write modifications of your own for Harbour, it is your choice
 * whether to permit this exception to apply to your modifications.
 * If you do not wish that, delete this exception notice.
 *
 */

/*
 * Individual authors:
 * (C) 2003-2004 Giancarlo Niccolai <gc at niccolai dot ws>
 *         Standard xplatform GT Info system,
 *         Graphical object system and event system.
 *         GTINFO() And GTO_* implementation.
 *
 * (C) 2004 Mauricio Abre <maurifull@datafull.com>
 *         Cross-GT, multiplatform Graphics API
 *
 */

#include "gtqtc.h"

static QApplication * app      = NULL;
static bool           hbqtinit = false;

#if !defined( SM_REMOTESESSION )
   #define SM_REMOTESESSION        0x1000
#endif

#define WM_MY_UPDATE_CARET         1700

static int           s_GtId;
static HB_GT_FUNCS   SuperTable;
#define HB_GTSUPER   (&SuperTable)
#define HB_GTID_PTR  (&s_GtId)

#define HB_GTWVT_GET(p) ( ( PHB_GTWVT ) HB_GTLOCAL( p ) )

static HB_CRITICAL_NEW( s_wvtMtx );
#define HB_WVT_LOCK     hb_threadEnterCriticalSection( &s_wvtMtx );
#define HB_WVT_UNLOCK   hb_threadLeaveCriticalSection( &s_wvtMtx );

static PHB_GTWVT  s_wvtWindows[ WVT_MAX_WINDOWS ];
static int        s_wvtCount = 0;

static const TCHAR s_szClassName[] = TEXT( "Harbour_WVT_Class" );

static const int K_Ctrl[] =
{
   K_CTRL_A, K_CTRL_B, K_CTRL_C, K_CTRL_D, K_CTRL_E, K_CTRL_F, K_CTRL_G,
   K_CTRL_H, K_CTRL_I, K_CTRL_J, K_CTRL_K, K_CTRL_L, K_CTRL_M, K_CTRL_N,
   K_CTRL_O, K_CTRL_P, K_CTRL_Q, K_CTRL_R, K_CTRL_S, K_CTRL_T, K_CTRL_U,
   K_CTRL_V, K_CTRL_W, K_CTRL_X, K_CTRL_Y, K_CTRL_Z
};

static LRESULT CALLBACK hb_gt_wvt_WndProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam );

void hb_ToOutDebug( const char * sTraceMsg, ... );

/*----------------------------------------------------------------------*/

static PHB_GTWVT hb_gt_wvt_Find( HWND hWnd )
{
   int iCount = s_wvtCount, iPos = 0;
   PHB_GTWVT pWVT = NULL;

   HB_WVT_LOCK

   while( iCount && iPos < WVT_MAX_WINDOWS )
   {
      if( s_wvtWindows[ iPos ] )
      {
         if( s_wvtWindows[ iPos ]->hWnd == hWnd )
         {
            pWVT = s_wvtWindows[ iPos ];
            break;
         }
         --iCount;
      }
      ++iPos;
   }

   HB_WVT_UNLOCK

   return pWVT;
}

static BOOL hb_gt_wvt_Alloc( PHB_GTWVT pWVT )
{
   BOOL fOK = FALSE;

   HB_WVT_LOCK

   if( s_wvtCount < WVT_MAX_WINDOWS )
   {
      int iPos = 0;
      do
      {
         if( s_wvtWindows[ iPos ] == NULL )
         {
            s_wvtWindows[ iPos ] = pWVT;
            pWVT->iHandle = iPos;
            fOK = TRUE;
            break;
         }
         ++iPos;
      }
      while( iPos < WVT_MAX_WINDOWS );
   }

   HB_WVT_UNLOCK

   return fOK;
}

static void hb_gt_wvt_Free( PHB_GTWVT pWVT )
{
   HB_WVT_LOCK

   s_wvtWindows[ pWVT->iHandle ] = NULL;

   if( --s_wvtCount == 0 )
   {
      app->quit();
   }

   HB_WVT_UNLOCK

   if( pWVT->pszSelectCopy )
      hb_xfree( pWVT->pszSelectCopy );

   if( pWVT->qFont )
      pWVT->qFont->~QFont();

   if( pWVT->hWnd )
      DestroyWindow( pWVT->hWnd );

   if( pWVT->hIcon && pWVT->bIconToFree )
      DestroyIcon( pWVT->hIcon );

   hb_xfree( pWVT );
}

static PHB_GTWVT hb_gt_wvt_New( PHB_GT pGT, HINSTANCE hInstance, int iCmdShow )
{
   PHB_GTWVT pWVT;
   OSVERSIONINFO osvi;

   osvi.dwOSVersionInfoSize = sizeof( OSVERSIONINFO );
   GetVersionEx( &osvi );

   pWVT = ( PHB_GTWVT ) hb_xgrab( sizeof( HB_GTWVT ) );
   memset( pWVT, 0, sizeof( HB_GTWVT ) );
   pWVT->pGT               = pGT;

   if( !hb_gt_wvt_Alloc( pWVT ) )
   {
      hb_xfree( pWVT );
      return NULL;
   }

   //pWVT->hInstance         = ( HINSTANCE ) hInstance;
   pWVT->iCmdShow          = iCmdShow;

   pWVT->ROWS              = WVT_DEFAULT_ROWS;
   pWVT->COLS              = WVT_DEFAULT_COLS;

   pWVT->COLORS[ 0]        = C_BLACK;
   pWVT->COLORS[ 1]        = C_BLUE;
   pWVT->COLORS[ 2]        = C_GREEN;
   pWVT->COLORS[ 3]        = C_CYAN;
   pWVT->COLORS[ 4]        = C_RED;
   pWVT->COLORS[ 5]        = C_MAGENTA;
   pWVT->COLORS[ 6]        = C_BROWN;
   pWVT->COLORS[ 7]        = C_WHITE;
   pWVT->COLORS[ 8]        = C_LIGHT_GRAY;
   pWVT->COLORS[ 9]        = C_BRIGHT_BLUE;
   pWVT->COLORS[10]        = C_BRIGHT_GREEN;
   pWVT->COLORS[11]        = C_BRIGHT_CYAN;
   pWVT->COLORS[12]        = C_BRIGHT_RED;
   pWVT->COLORS[13]        = C_BRIGHT_MAGENTA;
   pWVT->COLORS[14]        = C_YELLOW;
   pWVT->COLORS[15]        = C_BRIGHT_WHITE;

   /* THESE are the default font parameters, if not changed by user */
   pWVT->PTEXTSIZE.x       = WVT_DEFAULT_FONT_WIDTH;
   pWVT->PTEXTSIZE.y       = WVT_DEFAULT_FONT_HEIGHT;
   pWVT->fontWidth         = WVT_DEFAULT_FONT_WIDTH;
   pWVT->fontHeight        = WVT_DEFAULT_FONT_HEIGHT;
   pWVT->fontWeight        = FW_NORMAL;
   pWVT->fontQuality       = DEFAULT_QUALITY;
   hb_strncpy( pWVT->fontFace, WVT_DEFAULT_FONT_NAME, sizeof( pWVT->fontFace ) - 1 );

   pWVT->CaretExist        = FALSE;
   pWVT->CaretHidden       = TRUE;
   pWVT->CaretSize         = 0;
   pWVT->CaretWidth        = 0;
   pWVT->MousePos.x        = 0;
   pWVT->MousePos.y        = 0;
   pWVT->MouseMove         = TRUE;
   pWVT->hWnd              = NULL;
   pWVT->keyPointerIn      = 0;
   pWVT->keyPointerOut     = 0;
   pWVT->keyLast           = 0;

   pWVT->CentreWindow      = TRUE;            /* Default is to always display window in centre of screen */
   pWVT->CodePage          = OEM_CHARSET;     /* GetACP(); - set code page to default system */

   pWVT->Win9X             = ( osvi.dwPlatformId == VER_PLATFORM_WIN32_WINDOWS );
   pWVT->AltF4Close        = FALSE;

   pWVT->IgnoreWM_SYSCHAR  = FALSE;

   pWVT->bMaximized        = FALSE;
   pWVT->bBeingMarked      = FALSE;
   pWVT->bBeginMarked      = FALSE;

   pWVT->pszSelectCopy     = hb_strdup( "Mark and Copy" );
   pWVT->bSelectCopy       = TRUE;
   pWVT->bResizable        = TRUE;
   pWVT->bClosable         = TRUE;

   pWVT->ResizeMode        = HB_GTI_RESIZEMODE_FONT;

   pWVT->bResizing         = FALSE;
   pWVT->bAlreadySizing    = FALSE;

#ifndef HB_CDP_SUPPORT_OFF
   pWVT->hostCDP           = hb_vmCDP();
   pWVT->inCDP             = hb_vmCDP();
   pWVT->boxCDP            = hb_cdpFind( "EN" );
#endif

   return pWVT;
}

static int hb_gt_wvt_FireEvent( PHB_GTWVT pWVT, int nEvent )
{
   int nResult = 0; /* Unhandled */

   if( pWVT->pGT->pNotifierBlock )
   {
      if( hb_vmRequestReenter() )
      {
         PHB_ITEM pEvent = hb_itemPutNI( NULL, nEvent );

         nResult = hb_itemGetNI( hb_vmEvalBlockV( ( PHB_ITEM ) pWVT->pGT->pNotifierBlock, 1, pEvent ) );

         hb_itemRelease( pEvent );

         hb_vmRequestRestore();
      }
   }
   return nResult;
}

/*
 * use the standard fixed oem font, unless the caller has requested set size fonts
 */
static QFont* hb_gt_wvt_GetFont( const char * pszFace, int iHeight, int iWidth, int iWeight, int iQuality, int iCodePage )
{
   QFont *qFont = new QFont( pszFace, iHeight, iWeight, FALSE );

   if( qFont )
   {
      qFont->setFixedPitch( TRUE );
   }
   HB_SYMBOL_UNUSED( iWidth );
   HB_SYMBOL_UNUSED( iQuality );
   HB_SYMBOL_UNUSED( iCodePage );

   return qFont;
}

static POINT hb_gt_wvt_GetXYFromColRow( PHB_GTWVT pWVT, USHORT col, USHORT row )
{
   POINT xy;

   xy.x = col * pWVT->PTEXTSIZE.x;
   xy.y = row * pWVT->PTEXTSIZE.y;

   return xy;
}

static RECT hb_gt_wvt_GetXYFromColRowRect( PHB_GTWVT pWVT, RECT colrow )
{
   RECT xy;

   xy.left   = colrow.left * pWVT->PTEXTSIZE.x;
   xy.top    = colrow.top  * pWVT->PTEXTSIZE.y;
   xy.right  = ( colrow.right  + 1 ) * pWVT->PTEXTSIZE.x;
   xy.bottom = ( colrow.bottom + 1 ) * pWVT->PTEXTSIZE.y;

   return xy;
}

static void hb_gt_wvt_UpdateCaret( PHB_GTWVT pWVT )
{
   int iRow, iCol, iStyle, iCaretSize;

   HB_GTSELF_GETSCRCURSOR( pWVT->pGT, &iRow, &iCol, &iStyle );

   if( iRow < 0 || iCol < 0 || iRow >= pWVT->ROWS || iCol >= pWVT->COLS )
   {
      iCaretSize = 0;
   }
   else switch( iStyle )
   {
      case SC_INSERT:
         iCaretSize = pWVT->PTEXTSIZE.y >> 1;
         break;
      case SC_SPECIAL1:
         iCaretSize = pWVT->PTEXTSIZE.y;
         break;
      case SC_SPECIAL2:
         iCaretSize = - ( pWVT->PTEXTSIZE.y >> 1 );
         break;
      case SC_NORMAL:
         iCaretSize = HB_MAX( ( pWVT->PTEXTSIZE.y >> 2 ) - 1, 1 );
         break;
      default:
         iCaretSize = 0;
         break;
   }

   if( iCaretSize == 0 )
   {
      if( pWVT->CaretExist && !pWVT->CaretHidden )
      {
//         HideCaret( pWVT->hWnd );
         pWVT->CaretHidden = TRUE;
      }
   }
   else
   {
      if( iCaretSize != pWVT->CaretSize || pWVT->PTEXTSIZE.x != pWVT->CaretWidth ||
          !pWVT->CaretExist )
      {
         pWVT->CaretSize = iCaretSize;
         pWVT->CaretWidth = pWVT->PTEXTSIZE.x;
//         pWVT->CaretExist = CreateCaret( pWVT->hWnd, ( HBITMAP ) NULL, pWVT->PTEXTSIZE.x,
//                                         pWVT->CaretSize < 0 ? - pWVT->CaretSize : pWVT->CaretSize );
      }
      if( pWVT->CaretExist )
      {
         POINT xy;
         xy = hb_gt_wvt_GetXYFromColRow( pWVT, ( SHORT ) iCol, ( SHORT ) iRow );
//         SetCaretPos( xy.x, pWVT->CaretSize < 0 ?
//                      xy.y : xy.y + pWVT->PTEXTSIZE.y - pWVT->CaretSize );
//         ShowCaret( pWVT->hWnd );
         pWVT->CaretHidden = FALSE;
      }
   }
}

static void hb_gt_wvt_KillCaret( PHB_GTWVT pWVT )
{
   if( pWVT->CaretExist )
   {
//      DestroyCaret();
      pWVT->CaretExist = FALSE;
   }
}

/*
 *  functions for handling the input queues for the mouse and keyboard
 */
static void hb_gt_wvt_AddCharToInputQueue( PHB_GTWVT pWVT, int iKey )
{
   int iPos = pWVT->keyPointerIn;

   if( iKey == K_MOUSEMOVE || iKey == K_NCMOUSEMOVE )
   {
      /* Clipper strips repeated mouse movemnt - let's do the same */
      if( pWVT->keyLast == iKey && pWVT->keyPointerIn != pWVT->keyPointerOut )
         return;
   }

   /*
    * When the buffer is full new event overwrite the last one
    * in the buffer - it's Clipper behavior, [druzus]
    */
   pWVT->Keys[ iPos ] = pWVT->keyLast = iKey;
   if( ++iPos >= WVT_CHAR_QUEUE_SIZE )
      iPos = 0;
   if( iPos != pWVT->keyPointerOut )
      pWVT->keyPointerIn = iPos;
}

static BOOL hb_gt_wvt_GetCharFromInputQueue( PHB_GTWVT pWVT, int * iKey )
{
   if( pWVT->keyPointerOut != pWVT->keyPointerIn )
   {
      *iKey = pWVT->Keys[ pWVT->keyPointerOut ];
      if( ++pWVT->keyPointerOut >= WVT_CHAR_QUEUE_SIZE )
      {
         pWVT->keyPointerOut = 0;
      }
      return TRUE;
   }

   *iKey = 0;
   return FALSE;
}

static void hb_gt_wvt_TranslateKey( PHB_GTWVT pWVT, int key, int shiftkey, int altkey, int controlkey )
{
   int nVirtKey = GetKeyState( VK_MENU );

   if( nVirtKey & 0x8000 ) /* alt + key */
   {
      hb_gt_wvt_AddCharToInputQueue( pWVT, altkey );
   }
   else
   {
      nVirtKey = GetKeyState( VK_CONTROL );
      if( nVirtKey & 0x8000 ) /* control + key */
      {
         hb_gt_wvt_AddCharToInputQueue( pWVT, controlkey );
      }
      else
      {
         nVirtKey = GetKeyState( VK_SHIFT );
         if( nVirtKey & 0x8000 ) /* shift + key */
            hb_gt_wvt_AddCharToInputQueue( pWVT, shiftkey );
         else /* just key */
            hb_gt_wvt_AddCharToInputQueue( pWVT, key );
      }
   }
}

static int hb_gt_wvt_key_ansi_to_oem( int c )
{
   BYTE pszAnsi[ 2 ];
   BYTE pszOem[ 2 ];

   pszAnsi[ 0 ] = ( CHAR ) c;
   pszAnsi[ 1 ] = 0;
//   CharToOemBuffA( ( LPCSTR ) pszAnsi, ( LPSTR ) pszOem, 1 );

   return * pszOem;
}

static void hb_gt_wvt_FitRows( PHB_GTWVT pWVT )
{
   #if 0
   RECT wi;
   RECT ci;
   int maxWidth;
   int maxHeight;
   int borderWidth;
   int borderHeight;

   GetClientRect( pWVT->hWnd, &ci );
   GetWindowRect( pWVT->hWnd, &wi );

   borderWidth = ( wi.right - wi.left - ( ci.right - ci.left ) );
   borderHeight = ( wi.bottom - wi.top - ( ci.bottom - ci.top ) );

   if( pWVT->bMaximized )
   {
      SystemParametersInfo( SPI_GETWORKAREA, 0, &wi, 0 );

      maxHeight = wi.bottom - wi.top - borderHeight;
      maxWidth  = wi.right - wi.left - borderWidth;
   }
   else
   {
      maxWidth = ci.right - ci.left;
      maxHeight = ci.bottom - ci.top;
   }

   if( maxHeight > 0 )
   {
      BOOL bOldCentre = pWVT->CentreWindow;
      pWVT->CentreWindow = pWVT->bMaximized ? TRUE : FALSE;
      HB_GTSELF_SETMODE( pWVT->pGT, ( USHORT ) ( maxHeight / pWVT->PTEXTSIZE.y ), ( USHORT ) ( maxWidth / pWVT->PTEXTSIZE.x ) );
      pWVT->CentreWindow = bOldCentre;
   }
   #endif
}

static void hb_gt_wvt_FitSize( PHB_GTWVT pWVT )
{
   pWVT->qWnd->resize( 940,600 );
   HB_GTSELF_EXPOSEAREA( pWVT->pGT, 0, 0, pWVT->ROWS, pWVT->COLS );
}

static void hb_gt_wvt_ResetWindowSize( PHB_GTWVT pWVT )
{
   pWVT->qWnd->consoleArea->sizeByFont();
   #if 0
   QFont *qFont;
   /*
    * set the font and get it's size to determine the size of the client area
    * for the required number of rows and columns .
    * No need to call hb_gt_wvt_GetFont() if font is already existant [prtpal 20081108]
    */
   if( !pWVT->qFont )
   {
      qFont = hb_gt_wvt_GetFont( pWVT->fontFace, pWVT->fontHeight, pWVT->fontWidth,
                                 pWVT->fontWeight, pWVT->fontQuality, pWVT->CodePage );
      pWVT->qFont = qFont;
   }
   else
      qFont = pWVT->qFont;

   QPainter painter = QPainter( pWVT->qWnd->consoleArea );
   qFont = QFont( &qFont, painter.device() );
   QFontMetrics fontMetrics( &qFont );
   fontHeight = fontMetrics.height();
   fontWidth  = fontMetrics.averageCharWidth();
   #endif

   #if 0
   QFont        *qFont;
   QPaintDevice *qdc;
   QPaintEngine *qPE;

   HDC        hdc;
   USHORT     height, width;
   RECT       wi, ci;
   TEXTMETRIC tm;
   RECT       rcWorkArea;
   int        n;


   //qdc = pWVT->qWnd->PaintEngine();

   #if 0
   hdc      = GetDC( pWVT->hWnd );
   //qOldFont = ( HFONT ) SelectObject( hdc, hFont );
   GetTextMetrics( hdc, &tm );
   SetTextCharacterExtra( hdc, 0 ); /* do not add extra char spacing even if bold */
   //SelectObject( hdc, hOldFont );
   ReleaseDC( pWVT->hWnd, hdc );
   #endif
   /*
    * we will need to use the font size to handle the transformations from
    * row column space in the future, so we keep it around in a static!
    */

   pWVT->PTEXTSIZE.x = pWVT->fontWidth < 0 ? -pWVT->fontWidth :
                    tm.tmAveCharWidth; /* For fixed FONT should == tm.tmMaxCharWidth */
   pWVT->PTEXTSIZE.y = tm.tmHeight;    /* but seems to be a problem on Win9X so */
                                       /* assume proportional fonts always for Win9X */
#if defined(HB_OS_WIN_CE)
   pWVT->FixedFont = FALSE;
#else
   pWVT->FixedFont = !pWVT->Win9X && pWVT->fontWidth >= 0 &&
                     ( tm.tmPitchAndFamily & TMPF_FIXED_PITCH ) == 0 &&
                     ( pWVT->PTEXTSIZE.x == tm.tmMaxCharWidth );
#endif

   /* pWVT->FixedSize[] is used by ExtTextOut() to emulate
      fixed font when a proportional font is used */
   for( n = 0; n < pWVT->COLS; n++ )
      pWVT->FixedSize[ n ] = pWVT->PTEXTSIZE.x;

   /* resize the window to get the specified number of rows and columns */
   GetWindowRect( pWVT->hWnd, &wi );
   GetClientRect( pWVT->hWnd, &ci );

   height = ( USHORT ) ( pWVT->PTEXTSIZE.y * pWVT->ROWS );
   width  = ( USHORT ) ( pWVT->PTEXTSIZE.x * pWVT->COLS );

   width  += ( USHORT ) ( wi.right - wi.left - ci.right );
   height += ( USHORT ) ( wi.bottom - wi.top - ci.bottom );

   /* Center the window within the CLIENT area on the screen
      but only if pWVT->CentreWindow == TRUE */
   if( pWVT->CentreWindow && SystemParametersInfo( SPI_GETWORKAREA, 0, &rcWorkArea, 0 ) )
   {
      wi.left = rcWorkArea.left + ( ( rcWorkArea.right - rcWorkArea.left - width  ) / 2 );
      wi.top  = rcWorkArea.top  + ( ( rcWorkArea.bottom - rcWorkArea.top - height ) / 2 );
   }

   if( wi.left < 0 || wi.top < 0 )
   {
      pWVT->bMaximized = TRUE;

      if( pWVT->ResizeMode == HB_GTI_RESIZEMODE_FONT )
         hb_gt_wvt_FitSize( pWVT );
      else
         hb_gt_wvt_FitRows( pWVT );

      /* resize the window to get the specified number of rows and columns */
      GetWindowRect( pWVT->hWnd, &wi );
      GetClientRect( pWVT->hWnd, &ci );

      height = ( USHORT ) ( pWVT->PTEXTSIZE.y * pWVT->ROWS );
      width  = ( USHORT ) ( pWVT->PTEXTSIZE.x * pWVT->COLS );

      width  += ( USHORT ) ( wi.right - wi.left - ci.right );
      height += ( USHORT ) ( wi.bottom - wi.top - ci.bottom );

      /* Center the window within the CLIENT area on the screen
         but only if pWVT->CentreWindow == TRUE */
      if( pWVT->CentreWindow && SystemParametersInfo( SPI_GETWORKAREA, 0, &rcWorkArea, 0 ) )
      {
         wi.left = rcWorkArea.left + ( ( rcWorkArea.right - rcWorkArea.left - width  ) / 2 );
         wi.top  = rcWorkArea.top  + ( ( rcWorkArea.bottom - rcWorkArea.top - height ) / 2 );
      }
   }

   SetWindowPos( pWVT->hWnd, NULL, wi.left, wi.top, width, height, SWP_NOZORDER );

   HB_GTSELF_EXPOSEAREA( pWVT->pGT, 0, 0, pWVT->ROWS, pWVT->COLS );

   if( pWVT->CaretExist && !pWVT->CaretHidden )
      hb_gt_wvt_UpdateCaret( pWVT );

   #endif
}

static BOOL hb_gt_wvt_SetWindowSize( PHB_GTWVT pWVT, int iRow, int iCol )
{
   if( HB_GTSELF_RESIZE( pWVT->pGT, iRow, iCol ) )
   {
      pWVT->ROWS = ( USHORT ) iRow;
      pWVT->COLS = ( USHORT ) iCol;
      return TRUE;
   }
   return FALSE;
}

static BOOL hb_gt_wvt_InitWindow( PHB_GTWVT pWVT, int iRow, int iCol )
{
   BOOL fRet = hb_gt_wvt_SetWindowSize( pWVT, iRow, iCol );

   hb_gt_wvt_ResetWindowSize( pWVT );

   return fRet;
}

/*
 * get the row and column from xy pixel client coordinates
 * This works because we are using the FIXED system font
 */
static POINT hb_gt_wvt_GetColRowFromXY( PHB_GTWVT pWVT, USHORT x, USHORT y )
{
   POINT colrow;

   colrow.x = x / pWVT->PTEXTSIZE.x;
   colrow.y = y / pWVT->PTEXTSIZE.y;

   return colrow;
}

static RECT hb_gt_wvt_GetColRowFromXYRect( PHB_GTWVT pWVT, RECT xy )
{
   RECT colrow;

   colrow.left   = xy.left   / pWVT->PTEXTSIZE.x;
   colrow.top    = xy.top    / pWVT->PTEXTSIZE.y;
   colrow.right  = xy.right  / pWVT->PTEXTSIZE.x -
                   ( xy.right  % pWVT->PTEXTSIZE.x ? 0 : 1 ); /* Adjust for when rectangle */
   colrow.bottom = xy.bottom / pWVT->PTEXTSIZE.y -
                   ( xy.bottom % pWVT->PTEXTSIZE.y ? 0 : 1 ); /* EXACTLY overlaps characters */

   return colrow;
}

static void hb_gt_wvt_SetMousePos( PHB_GTWVT pWVT, int iRow, int iCol )
{
   pWVT->MousePos.y = ( SHORT ) iRow;
   pWVT->MousePos.x = ( SHORT ) iCol;
}

static void hb_gt_wvt_MouseEvent( PHB_GTWVT pWVT, UINT message, WPARAM wParam, LPARAM lParam )
{
   #if 0
   POINT xy, colrow;
   SHORT keyCode = 0;

   HB_SYMBOL_UNUSED( wParam );

   if( ! pWVT->bBeginMarked && ! pWVT->MouseMove && ( message == WM_MOUSEMOVE || message == WM_NCMOUSEMOVE ) )
      return;

   xy.x = LOWORD( lParam );
   xy.y = HIWORD( lParam );

   colrow = hb_gt_wvt_GetColRowFromXY( pWVT, ( USHORT ) xy.x, ( USHORT ) xy.y );
   hb_gt_wvt_SetMousePos( pWVT, colrow.y, colrow.x );

   switch( message )
   {
      case WM_LBUTTONDBLCLK:
         keyCode = K_LDBLCLK;
         break;

      case WM_RBUTTONDBLCLK:
         keyCode = K_RDBLCLK;
         break;

      case WM_LBUTTONDOWN:
      {
         if( pWVT->bBeginMarked )
         {
            pWVT->bBeingMarked = TRUE;

            pWVT->sRectNew.left     = xy.x;
            pWVT->sRectNew.top      = xy.y;
            pWVT->sRectNew.right    = xy.x;
            pWVT->sRectNew.bottom   = xy.y;

            pWVT->sRectOld.left   = 0;
            pWVT->sRectOld.top    = 0;
            pWVT->sRectOld.right  = 0;
            pWVT->sRectOld.bottom = 0;

            return;
         }
         else
         {
            keyCode = K_LBUTTONDOWN;
            break;
         }
      }
      case WM_RBUTTONDOWN:
         keyCode = K_RBUTTONDOWN;
         break;

      case WM_RBUTTONUP:
         keyCode = K_RBUTTONUP;
         break;

      case WM_LBUTTONUP:
      {
         if( pWVT->bBeingMarked )
         {
            pWVT->bBeginMarked = FALSE;
            pWVT->bBeingMarked = FALSE;

            RedrawWindow( pWVT->hWnd, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW );

            {
               ULONG  ulSize;
               int    irow, icol, j, top, left, bottom, right;
               char * sBuffer;
               RECT   rect = { 0, 0, 0, 0 };
               RECT   colrowRC = { 0, 0, 0, 0 };

               rect.left   = HB_MIN( pWVT->sRectNew.left, pWVT->sRectNew.right  );
               rect.top    = HB_MIN( pWVT->sRectNew.top , pWVT->sRectNew.bottom );
               rect.right  = HB_MAX( pWVT->sRectNew.left, pWVT->sRectNew.right  );
               rect.bottom = HB_MAX( pWVT->sRectNew.top , pWVT->sRectNew.bottom );

               colrowRC = hb_gt_wvt_GetColRowFromXYRect( pWVT, rect );

               left   = colrowRC.left;
               top    = colrowRC.top;
               right  = colrowRC.right;
               bottom = colrowRC.bottom;

               ulSize = ( ( bottom - top + 1 ) * ( right - left + 1 + 2 ) );
               sBuffer = ( char * ) hb_xgrab( ulSize + 1 );

               for( j = 0, irow = top; irow <= bottom; irow++ )
               {
                  for( icol = left; icol <= right; icol++ )
                  {
                     BYTE bColor, bAttr;
                     USHORT usChar;

                     if( !HB_GTSELF_GETSCRCHAR( pWVT->pGT, irow, icol, &bColor, &bAttr, &usChar ) )
                        break;

                     sBuffer[ j++ ] = ( char ) usChar;
                  }

                  sBuffer[ j++ ] = '\r';
                  sBuffer[ j++ ] = '\n';
               }
               sBuffer[ j ] = '\0';

               if( j > 0 )
               {
                  hb_gt_winapi_setClipboard( pWVT->CodePage == OEM_CHARSET ?
                                             CF_OEMTEXT : CF_TEXT,
                                             sBuffer,
                                             j );
               }

               hb_xfree( sBuffer );
            }
            return;
         }
         else
         {
            keyCode = K_LBUTTONUP;
            break;
         }
      }
      case WM_MBUTTONDOWN:
         keyCode = K_MBUTTONDOWN;
         break;

      case WM_MBUTTONUP:
         keyCode = K_MBUTTONUP;
         break;

      case WM_MBUTTONDBLCLK:
         keyCode = K_MDBLCLK;
         break;

      case WM_MOUSEMOVE:
      {
         if( pWVT->bBeingMarked )
         {
            RECT rect     = { 0, 0, 0, 0 };
            RECT colrowRC = { 0, 0, 0, 0 };

            pWVT->sRectNew.right  = xy.x;
            pWVT->sRectNew.bottom = xy.y;

            rect.left   = HB_MIN( pWVT->sRectNew.left, pWVT->sRectNew.right  );
            rect.top    = HB_MIN( pWVT->sRectNew.top , pWVT->sRectNew.bottom );
            rect.right  = HB_MAX( pWVT->sRectNew.left, pWVT->sRectNew.right  );
            rect.bottom = HB_MAX( pWVT->sRectNew.top , pWVT->sRectNew.bottom );

            colrowRC = hb_gt_wvt_GetColRowFromXYRect( pWVT, rect );
            rect     = hb_gt_wvt_GetXYFromColRowRect( pWVT, colrowRC );

            if( rect.left   != pWVT->sRectOld.left   ||
                rect.top    != pWVT->sRectOld.top    ||
                rect.right  != pWVT->sRectOld.right  ||
                rect.bottom != pWVT->sRectOld.bottom )
            {
#if !defined(HB_OS_WIN_CE)  /* WinCE does not support InvertRgn */
               /* Concept forwarded by Andy Wos - thanks. */
               HRGN rgn1 = CreateRectRgn( pWVT->sRectOld.left, pWVT->sRectOld.top, pWVT->sRectOld.right, pWVT->sRectOld.bottom );
               HRGN rgn2 = CreateRectRgn( rect.left, rect.top, rect.right, rect.bottom );
               HRGN rgn3 = CreateRectRgn( 0, 0, 0, 0 );

               if( CombineRgn( rgn3, rgn1, rgn2, RGN_XOR ) != 0 )
               {
                  HDC hdc = GetDC( pWVT->hWnd );
                  InvertRgn( hdc, rgn3 );
                  ReleaseDC( pWVT->hWnd, hdc );
               }

               DeleteObject( rgn1 );
               DeleteObject( rgn2 );
               DeleteObject( rgn3 );
#endif
               pWVT->sRectOld.left   = rect.left;
               pWVT->sRectOld.top    = rect.top;
               pWVT->sRectOld.right  = rect.right;
               pWVT->sRectOld.bottom = rect.bottom;
            }
            return;
         }
         else
         {
#if defined( __HB_GTWVT_GEN_K_MMDOWN_EVENTS )
            SHORT keyState = ( SHORT ) wParam;

            switch( keyState )
            {
               case MK_LBUTTON:
                  keyCode = K_MMLEFTDOWN;
                  break;
               case MK_RBUTTON:
                  keyCode = K_MMRIGHTDOWN;
                  break;
               case MK_MBUTTON:
                  keyCode = K_MMMIDDLEDOWN;
                  break;
               default:
                  keyCode = K_MOUSEMOVE;
            }
            break;
#else
            keyCode = K_MOUSEMOVE;
            break;
#endif
         }
      }
      case WM_MOUSEWHEEL:
      {
         SHORT keyState = ( SHORT ) HIWORD( wParam );
         keyCode = keyState > 0 ? K_MWFORWARD : K_MWBACKWARD;
         break;
      }
      case WM_NCMOUSEMOVE:
         keyCode = K_NCMOUSEMOVE;
         break;
   }

   if( keyCode != 0 )
      hb_gt_wvt_AddCharToInputQueue( pWVT, keyCode );

   #endif
}

static BOOL hb_gt_wvt_KeyEvent( PHB_GTWVT pWVT, UINT message, WPARAM wParam, LPARAM lParam )
{
   #if 0

   switch( message )
   {
      case WM_KEYDOWN:
      case WM_SYSKEYDOWN:
      {
         BOOL bAlt = GetKeyState( VK_MENU ) & 0x8000;

         pWVT->IgnoreWM_SYSCHAR = FALSE;

         switch( wParam )
         {
            case VK_LEFT:
               hb_gt_wvt_TranslateKey( pWVT, K_LEFT , K_SH_LEFT , K_ALT_LEFT , K_CTRL_LEFT  );
               break;
            case VK_RIGHT:
               hb_gt_wvt_TranslateKey( pWVT, K_RIGHT, K_SH_RIGHT, K_ALT_RIGHT, K_CTRL_RIGHT );
               break;
            case VK_UP:
               hb_gt_wvt_TranslateKey( pWVT, K_UP   , K_SH_UP   , K_ALT_UP   , K_CTRL_UP    );
               break;
            case VK_DOWN:
               hb_gt_wvt_TranslateKey( pWVT, K_DOWN , K_SH_DOWN , K_ALT_DOWN , K_CTRL_DOWN  );
               break;
            case VK_HOME:
               hb_gt_wvt_TranslateKey( pWVT, K_HOME , K_SH_HOME , K_ALT_HOME , K_CTRL_HOME  );
               break;
            case VK_END:
               hb_gt_wvt_TranslateKey( pWVT, K_END  , K_SH_END  , K_ALT_END  , K_CTRL_END   );
               break;
            case VK_DELETE:
               hb_gt_wvt_TranslateKey( pWVT, K_DEL  , K_SH_DEL  , K_ALT_DEL  , K_CTRL_DEL   );
               break;
            case VK_INSERT:
               hb_gt_wvt_TranslateKey( pWVT, K_INS  , K_SH_INS  , K_ALT_INS  , K_CTRL_INS   );
               break;
            case VK_PRIOR:
               hb_gt_wvt_TranslateKey( pWVT, K_PGUP , K_SH_PGUP , K_ALT_PGUP , K_CTRL_PGUP  );
               break;
            case VK_NEXT:
               hb_gt_wvt_TranslateKey( pWVT, K_PGDN , K_SH_PGDN , K_ALT_PGDN , K_CTRL_PGDN  );
               break;

            case VK_F1:
               hb_gt_wvt_TranslateKey( pWVT, K_F1   , K_SH_F1, K_ALT_F1   , K_CTRL_F1    );
               break;
            case VK_F2:
               hb_gt_wvt_TranslateKey( pWVT, K_F2   , K_SH_F2, K_ALT_F2   , K_CTRL_F2    );
               break;
            case VK_F3:
               hb_gt_wvt_TranslateKey( pWVT, K_F3   , K_SH_F3, K_ALT_F3   , K_CTRL_F3    );
               break;
            case VK_F4:
               if( pWVT->AltF4Close && bAlt )
                  return DefWindowProc( pWVT->hWnd, message, wParam, lParam ) != 0;
               hb_gt_wvt_TranslateKey( pWVT, K_F4   , K_SH_F4, K_ALT_F4   , K_CTRL_F4    );
               break;
            case VK_F5:
               hb_gt_wvt_TranslateKey( pWVT, K_F5   , K_SH_F5, K_ALT_F5   , K_CTRL_F5    );
               break;
            case VK_F6:
               hb_gt_wvt_TranslateKey( pWVT, K_F6   , K_SH_F6, K_ALT_F6   , K_CTRL_F6    );
               break;
            case VK_F7:
               hb_gt_wvt_TranslateKey( pWVT, K_F7   , K_SH_F7, K_ALT_F7   , K_CTRL_F7    );
               break;
            case VK_F8:
               hb_gt_wvt_TranslateKey( pWVT, K_F8   , K_SH_F8, K_ALT_F8   , K_CTRL_F8    );
               break;
            case VK_F9:
               hb_gt_wvt_TranslateKey( pWVT, K_F9   , K_SH_F9, K_ALT_F9   , K_CTRL_F9    );
               break;
            case VK_F10:
               hb_gt_wvt_TranslateKey( pWVT, K_F10  , K_SH_F10,K_ALT_F10  , K_CTRL_F10   );
               break;
            case VK_F11:
               hb_gt_wvt_TranslateKey( pWVT, K_F11  , K_SH_F11,K_ALT_F11  , K_CTRL_F11   );
               break;
            case VK_F12:
               hb_gt_wvt_TranslateKey( pWVT, K_F12  , K_SH_F12,K_ALT_F12  , K_CTRL_F12   );
               break;
            default:
            {
               BOOL bCtrl     = GetKeyState( VK_CONTROL ) & 0x8000;
               BOOL bShift    = GetKeyState( VK_SHIFT ) & 0x8000;
               int  iScanCode = HIWORD( lParam ) & 0xFF;

               if( bCtrl && iScanCode == 76 ) /* CTRL_VK_NUMPAD5 */
               {
                  hb_gt_wvt_AddCharToInputQueue( pWVT, KP_CTRL_5 );
               }
               else if( bCtrl && wParam == VK_TAB ) /* K_CTRL_TAB */
               {
                  hb_gt_wvt_AddCharToInputQueue( pWVT, bShift ? K_CTRL_SH_TAB : K_CTRL_TAB );
               }
               else if( iScanCode == 70 ) /* Ctrl_Break key OR Scroll Lock Key */
               {
                  if( bCtrl )  /* Not scroll lock */
                  {
                     hb_gt_wvt_AddCharToInputQueue( pWVT, HB_BREAK_FLAG ); /* Pretend Alt+C pressed */
                     pWVT->IgnoreWM_SYSCHAR = TRUE;
                  }
                  else
                  {
                      DefWindowProc( pWVT->hWnd, message, wParam, lParam );  /* Let windows handle ScrollLock */
                  }
               }
               else if( bCtrl && iScanCode == 53 && bShift )
               {
                  hb_gt_wvt_AddCharToInputQueue( pWVT, K_CTRL_QUESTION );
               }
               else if( ( bAlt || bCtrl ) && (
                        wParam == VK_MULTIPLY || wParam == VK_ADD ||
                        wParam == VK_SUBTRACT || wParam == VK_DIVIDE ) )
               {
                  if( bAlt )
                     pWVT->IgnoreWM_SYSCHAR = TRUE;

                  switch( wParam )
                  {
                     case VK_MULTIPLY:
                        hb_gt_wvt_TranslateKey( pWVT, '*', '*', KP_ALT_ASTERISK, KP_CTRL_ASTERISK );
                        break;
                     case VK_ADD:
                        hb_gt_wvt_TranslateKey( pWVT, '+', '+', KP_ALT_PLUS, KP_CTRL_PLUS );
                        break;
                     case VK_SUBTRACT:
                        hb_gt_wvt_TranslateKey( pWVT, '-', '-', KP_ALT_MINUS, KP_CTRL_MINUS );
                        break;
                     case VK_DIVIDE:
                        hb_gt_wvt_TranslateKey( pWVT, '/', '/', KP_ALT_SLASH, KP_CTRL_SLASH );
                        break;
                  }
               }
            }
         }
         break;
      }

      case WM_CHAR:
      {
         BOOL bCtrl     = GetKeyState( VK_CONTROL ) & 0x8000;
         int  iScanCode = HIWORD( lParam ) & 0xFF;
         int  c = ( int ) wParam;

         if( !pWVT->IgnoreWM_SYSCHAR )
         {
            if( bCtrl && iScanCode == 28 )  /* K_CTRL_RETURN */
            {
               hb_gt_wvt_AddCharToInputQueue( pWVT, K_CTRL_RETURN );
            }
            else if( bCtrl && ( c >= 1 && c <= 26 ) )  /* K_CTRL_A - Z */
            {
               hb_gt_wvt_AddCharToInputQueue( pWVT, K_Ctrl[ c - 1 ] );
            }
            else
            {
               switch( c )
               {
                  /* handle special characters */
                  case VK_BACK:
                     hb_gt_wvt_TranslateKey( pWVT, K_BS, K_SH_BS, K_ALT_BS, K_CTRL_BS );
                     break;
                  case VK_TAB:
                     hb_gt_wvt_TranslateKey( pWVT, K_TAB, K_SH_TAB, K_ALT_TAB, K_CTRL_TAB );
                     break;
                  case VK_RETURN:
                     hb_gt_wvt_TranslateKey( pWVT, K_RETURN, K_SH_RETURN, K_ALT_RETURN, K_CTRL_RETURN );
                     break;
                  case VK_ESCAPE:
                     hb_gt_wvt_AddCharToInputQueue( pWVT, K_ESC );
                     break;
                  default:
                     if( pWVT->inCDP )
                     {
                        c = hb_cdpGetChar( pWVT->inCDP, FALSE, ( USHORT ) c );
                     }
                     else if( pWVT->CodePage == OEM_CHARSET )
                        c = hb_gt_wvt_key_ansi_to_oem( c );
                     hb_gt_wvt_AddCharToInputQueue( pWVT, c );
                     break;
               }
            }
         }
         pWVT->IgnoreWM_SYSCHAR = FALSE; /* As Suggested by Peter */
         break;
      }

      case WM_SYSCHAR:
      {
         if( !pWVT->IgnoreWM_SYSCHAR )
         {
            int c, iScanCode = HIWORD( lParam ) & 0xFF;
            switch( iScanCode )
            {
               case  2:
                  c = K_ALT_1;
                  break;
               case  3:
                  c = K_ALT_2;
                  break;
               case  4:
                  c = K_ALT_3;
                  break;
               case  5:
                  c = K_ALT_4;
                  break;
               case  6:
                  c = K_ALT_5;
                  break;
               case  7:
                  c = K_ALT_6;
                  break;
               case  8:
                  c = K_ALT_7;
                  break;
               case  9:
                  c = K_ALT_8;
                  break;
               case 10:
                  c = K_ALT_9;
                  break;
               case 11:
                  c = K_ALT_0;
                  break;
               case 13:
                  c = K_ALT_EQUALS;
                  break;
               case 14:
                  c = K_ALT_BS;
                  break;
               case 16:
                  c = K_ALT_Q;
                  break;
               case 17:
                  c = K_ALT_W;
                  break;
               case 18:
                  c = K_ALT_E;
                  break;
               case 19:
                  c = K_ALT_R;
                  break;
               case 20:
                  c = K_ALT_T;
                  break;
               case 21:
                  c = K_ALT_Y;
                  break;
               case 22:
                  c = K_ALT_U;
                  break;
               case 23:
                  c = K_ALT_I;
                  break;
               case 24:
                  c = K_ALT_O;
                  break;
               case 25:
                  c = K_ALT_P;
                  break;
               case 30:
                  c = K_ALT_A;
                  break;
               case 31:
                  c = K_ALT_S;
                  break;
               case 32:
                  c = K_ALT_D;
                  break;
               case 33:
                  c = K_ALT_F;
                  break;
               case 34:
                  c = K_ALT_G;
                  break;
               case 35:
                  c = K_ALT_H;
                  break;
               case 36:
                  c = K_ALT_J;
                  break;
               case 37:
                  c = K_ALT_K;
                  break;
               case 38:
                  c = K_ALT_L;
                  break;
               case 44:
                  c = K_ALT_Z;
                  break;
               case 45:
                  c = K_ALT_X;
                  break;
               case 46:
                  c = K_ALT_C;
                  break;
               case 47:
                  c = K_ALT_V;
                  break;
               case 48:
                  c = K_ALT_B;
                  break;
               case 49:
                  c = K_ALT_N;
                  break;
               case 50:
                  c = K_ALT_M;
                  break;
               default:
                  c = ( int ) wParam;
                  break;
            }
            hb_gt_wvt_AddCharToInputQueue( pWVT, c );
         }
         pWVT->IgnoreWM_SYSCHAR = FALSE;
      }
   }
   #endif

   return 0;
}

/*
 * hb_gt_wvt_TextOut converts col and row to x and y ( pixels ) and calls
 * the Windows function TextOut with the expected coordinates
 */
static BOOL hb_gt_wvt_TextOut( PHB_GTWVT pWVT, HDC hdc, USHORT col, USHORT row, BYTE attr, LPCTSTR lpString, USHORT cbString )
{
   #if 0
   POINT xy;
   RECT  rClip;

   /* set foreground color */
   SetTextColor( hdc, pWVT->COLORS[ attr & 0x0F ] );
   /* set background color */
   SetBkColor( hdc, pWVT->COLORS[ ( attr >> 4 ) & 0x0F ] );

   SetTextAlign( hdc, TA_LEFT );

   xy = hb_gt_wvt_GetXYFromColRow( pWVT, col, row );
   SetRect( &rClip, xy.x, xy.y, xy.x + cbString * pWVT->PTEXTSIZE.x, xy.y + pWVT->PTEXTSIZE.y );

   return ExtTextOut( hdc, xy.x, xy.y, ETO_CLIPPED | ETO_OPAQUE, &rClip,
                      lpString, cbString, pWVT->FixedFont ? NULL : pWVT->FixedSize );
   #endif

   return TRUE;
}

static void hb_gt_wvt_PaintText( PHB_GTWVT pWVT, RECT updateRect )
{
   #if 0
   PAINTSTRUCT ps;
   HDC         hdc;
   RECT        rcRect;
   int         iRow, iCol, startCol, len;
   BYTE        bColor, bAttr, bOldColor = 0;
   USHORT      usChar;
   TCHAR       text[ WVT_MAX_ROWS ];

   hdc = BeginPaint( pWVT->hWnd, &ps );

   rcRect = hb_gt_wvt_GetColRowFromXYRect( pWVT, updateRect );

   for( iRow = rcRect.top; iRow <= rcRect.bottom; ++iRow )
   {
      iCol = startCol = rcRect.left;
      len = 0;

      while( iCol <= rcRect.right )
      {
         if( !HB_GTSELF_GETSCRCHAR( pWVT->pGT, iRow, iCol, &bColor, &bAttr, &usChar ) )
            break;

         usChar = hb_cdpGetU16( bAttr & HB_GT_ATTR_BOX ? pWVT->boxCDP : pWVT->hostCDP, TRUE, ( BYTE ) usChar );
         if( len == 0 )
         {
            bOldColor = bColor;
         }
         else if( bColor != bOldColor )
         {
            hb_gt_wvt_TextOut( pWVT, hdc, ( USHORT ) startCol, ( USHORT ) iRow, bOldColor, text, ( USHORT ) len );
            bOldColor = bColor;
            startCol = iCol;
            len = 0;
         }
         text[ len++ ] = ( TCHAR ) usChar;
         iCol++;
      }
      if( len > 0 )
         hb_gt_wvt_TextOut( pWVT, hdc, ( USHORT ) startCol, ( USHORT ) iRow, bOldColor, text, ( USHORT ) len );
   }

   EndPaint( pWVT->hWnd, &ps );
   #endif
}
#if 0
static LRESULT CALLBACK hb_gt_wvt_WndProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
   PHB_GTWVT pWVT = hb_gt_wvt_Find( hWnd );

   if( pWVT ) switch( message )
   {
      case WM_CREATE:
         return hb_gt_wvt_InitWindow( pWVT, pWVT->ROWS, pWVT->COLS );

      case WM_PAINT:
      {
         RECT updateRect;

         if( GetUpdateRect( hWnd, &updateRect, FALSE ) )
            hb_gt_wvt_PaintText( pWVT, updateRect );

         return 0;
      }

      case WM_MY_UPDATE_CARET:
         hb_gt_wvt_UpdateCaret( pWVT );
         return 0;

      case WM_SETFOCUS:
         hb_gt_wvt_UpdateCaret( pWVT );
         return 0;

      case WM_KILLFOCUS:
         hb_gt_wvt_KillCaret( pWVT );
         return 0;

      case WM_KEYDOWN:
      case WM_SYSKEYDOWN:
      case WM_CHAR:
      case WM_SYSCHAR:
         return hb_gt_wvt_KeyEvent( pWVT, message, wParam, lParam );

      case WM_RBUTTONDOWN:
      case WM_LBUTTONDOWN:
      case WM_RBUTTONUP:
      case WM_LBUTTONUP:
      case WM_RBUTTONDBLCLK:
      case WM_LBUTTONDBLCLK:
      case WM_MBUTTONDOWN:
      case WM_MBUTTONUP:
      case WM_MBUTTONDBLCLK:
      case WM_MOUSEMOVE:
      case WM_MOUSEWHEEL:
      case WM_NCMOUSEMOVE:
         hb_gt_wvt_MouseEvent( pWVT, message, wParam, lParam );
         return 0;

      case WM_QUERYENDSESSION: /* Closing down computer */
         hb_vmRequestQuit();
         return 0;

      case WM_CLOSE:  /* Clicked 'X' on system menu */
         if( hb_gt_wvt_FireEvent( pWVT, HB_GTE_CLOSE ) == 0 )
         {
            PHB_ITEM pItem = hb_itemPutL( NULL, TRUE );
            hb_setSetItem( HB_SET_CANCEL, pItem );
            hb_itemRelease( pItem );
            hb_vmRequestCancel();
         }
         return 0;

      case WM_QUIT:
      case WM_DESTROY:
         return 0;

      case WM_ENTERIDLE:
         /* FSG - 12/05/2004 - Signal than i'm on idle */
         hb_idleState();
         return 0;

      /* Pritpal Bedi - 06 Jun 2008 */
      case WM_ACTIVATE:
         hb_gt_wvt_FireEvent( pWVT, ( LOWORD( wParam ) == WA_INACTIVE ? HB_GTE_KILLFOCUS : HB_GTE_SETFOCUS ) );
         return 0;

      case WM_ENTERSIZEMOVE:
         if( pWVT->bMaximized )
         {
            pWVT->bMaximized = FALSE;

            /* Enable "maximize" button */

#if (defined(_MSC_VER) && (_MSC_VER <= 1200 || defined(HB_OS_WIN_CE)) || defined(__DMC__)) && !defined(HB_ARCH_64BIT)
            SetWindowLong( pWVT->hWnd, GWL_STYLE, WS_OVERLAPPED|WS_CAPTION|WS_SYSMENU|WS_MINIMIZEBOX|WS_MAXIMIZEBOX|WS_THICKFRAME );
#else
            SetWindowLongPtr( pWVT->hWnd, GWL_STYLE, WS_OVERLAPPED|WS_CAPTION|WS_SYSMENU|WS_MINIMIZEBOX|WS_MAXIMIZEBOX|WS_THICKFRAME );
#endif

            SetWindowPos( pWVT->hWnd, NULL, 0, 0, 0, 0,
                                      SWP_NOACTIVATE | SWP_DRAWFRAME | SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_DEFERERASE );
            ShowWindow( pWVT->hWnd, SW_HIDE );
            ShowWindow( pWVT->hWnd, SW_NORMAL );
         }
         pWVT->bResizing = TRUE;
         return 0;

      case WM_EXITSIZEMOVE:
         pWVT->bResizing = FALSE;

         hb_gt_wvt_FireEvent( pWVT, HB_GTE_RESIZED );
         if( pWVT->ResizeMode == HB_GTI_RESIZEMODE_ROWS )
            hb_gt_wvt_AddCharToInputQueue( pWVT, HB_K_RESIZE );
         return 0;

      case WM_SIZE:
         if( pWVT->bResizing )
         {
            if( !pWVT->bAlreadySizing )
            {
               pWVT->bAlreadySizing = TRUE;

               if( pWVT->ResizeMode == HB_GTI_RESIZEMODE_FONT )
                  hb_gt_wvt_FitSize( pWVT );
               else
                  hb_gt_wvt_FitRows( pWVT );
            }
            pWVT->bAlreadySizing = FALSE;
         }
         return 0;

      case WM_SYSCOMMAND:
         switch( wParam )
         {
            case SC_MAXIMIZE:
            {
               pWVT->bMaximized = TRUE;

               if( pWVT->ResizeMode == HB_GTI_RESIZEMODE_FONT )
                  hb_gt_wvt_FitSize( pWVT );
               else
                  hb_gt_wvt_FitRows( pWVT );

               /* Disable "maximize" button */
#if (defined(_MSC_VER) && (_MSC_VER <= 1200 || defined(HB_OS_WIN_CE)) || defined(__DMC__)) && !defined(HB_ARCH_64BIT)
               SetWindowLong( pWVT->hWnd, GWL_STYLE, WS_OVERLAPPED|WS_CAPTION|WS_SYSMENU|WS_MINIMIZEBOX|WS_THICKFRAME );
#else
               SetWindowLongPtr( pWVT->hWnd, GWL_STYLE, WS_OVERLAPPED|WS_CAPTION|WS_SYSMENU|WS_MINIMIZEBOX|WS_THICKFRAME );
#endif
               SetWindowPos( pWVT->hWnd, NULL, 0, 0, 0, 0,
                                         SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_DEFERERASE );
               ShowWindow( pWVT->hWnd, SW_HIDE );
               ShowWindow( pWVT->hWnd, SW_NORMAL );

               hb_gt_wvt_FireEvent( pWVT, HB_GTE_RESIZED );
               if( pWVT->ResizeMode == HB_GTI_RESIZEMODE_ROWS )
                  hb_gt_wvt_AddCharToInputQueue( pWVT, HB_K_RESIZE );

               return 0;
            }

            case SYS_EV_MARK:
            {
               pWVT->bBeginMarked = TRUE;
               return 0;
            }
         }
         break;
   }

   return DefWindowProc( hWnd, message, wParam, lParam );
}
#endif

static DWORD hb_gt_wvt_ProcessMessages( void )
{
   app->processEvents();
   return( 0 );
}

static BOOL hb_gt_wvt_ValidWindowSize( HWND hWnd, int rows, int cols, QFont *qFont, int iWidth )
{
   HB_SYMBOL_UNUSED( hWnd   );
   HB_SYMBOL_UNUSED( rows   );
   HB_SYMBOL_UNUSED( cols   );
   HB_SYMBOL_UNUSED( qFont  );
   HB_SYMBOL_UNUSED( iWidth );

   #if 0
   HDC        hdc;
   HFONT      hOldFont;
   USHORT     width, height, maxWidth, maxHeight;
   TEXTMETRIC tm;
   RECT       rcWorkArea;

   SystemParametersInfo( SPI_GETWORKAREA,0, &rcWorkArea, 0 );

   maxWidth  = ( USHORT ) ( rcWorkArea.right - rcWorkArea.left );
   maxHeight = ( USHORT ) ( rcWorkArea.bottom - rcWorkArea.top );

   hdc       = GetDC( hWnd );
   hOldFont  = ( HFONT ) SelectObject( hdc, hFont );
   GetTextMetrics( hdc, &tm );
   SelectObject( hdc, hOldFont ); /* Put old font back */
   ReleaseDC( hWnd, hdc );

   width     = ( USHORT ) ( iWidth < 0 ? -iWidth : tm.tmAveCharWidth * cols );  /* Total pixel width this setting would take */
   height    = ( USHORT ) ( tm.tmHeight * rows ); /* Total pixel height this setting would take */

   return ( width <= maxWidth ) && ( height <= maxHeight );
   #endif

   return TRUE;
}

static MainWindow* hb_gt_wvt_CreateWindow( BOOL bResizable )
{
   MainWindow *qWnd = new MainWindow();

   HB_SYMBOL_UNUSED( bResizable );

   return qWnd;
}

static BOOL hb_gt_wvt_CreateConsoleWindow( PHB_GTWVT pWVT )
{
   if( !pWVT->qWnd )
   {
      pWVT->qWnd = hb_gt_wvt_CreateWindow( pWVT->bResizable );
      if( !pWVT->qWnd )
         hb_errInternal( 10001, "Failed to create WVT window", NULL, NULL );

      pWVT->qWnd->pGT               = pWVT->pGT;
      pWVT->qWnd->consoleArea->pGT  = pWVT->pGT;

      hb_gt_wvt_InitWindow( pWVT, pWVT->ROWS, pWVT->COLS );

      /* Set icon */
      #if 0
      if( pWVT->hIcon )
      {
         pWVT->qWnd->setWindowIcon( pWVT->hIcon );
      }
      #endif

      /* Set default window title */
      {
         PHB_FNAME pFileName = hb_fsFNameSplit( hb_cmdargARGV()[ 0 ] );
         pWVT->qWnd->setWindowTitle( "Harbour-QT Console" );
         pWVT->qWnd->resize( 900,600 );
         pWVT->qWnd->consoleArea->sizeByFont();
         hb_xfree( pFileName );
      }

      {
         #if 0
         HMENU hSysMenu = GetSystemMenu( pWVT->hWnd, FALSE );
         if( hSysMenu )
         {
            /* Create "Mark" prompt in SysMenu to allow console type copy operation */
            LPTSTR buffer = HB_TCHAR_CONVTO( pWVT->pszSelectCopy );
            AppendMenu( hSysMenu, MF_STRING, SYS_EV_MARK, buffer );
            HB_TCHAR_FREE( buffer );

            if( ! pWVT->bClosable )
               EnableMenuItem( hSysMenu, SC_CLOSE, MF_BYCOMMAND | MF_GRAYED );
         }
         #endif
      }

      pWVT->qWnd->show();
      pWVT->qWnd->update();
   }

   return TRUE;
}

/* ********************************************************************** */
/*
 * GT Specific Functions
 */
/* ********************************************************************** */

void hbqt_exit( PHB_GT pGT )
{
   PHB_GTWVT pWVT;

   HB_TRACE(HB_TR_DEBUG, ("hbqt_exit(%p)", pGT));

   pWVT = HB_GTWVT_GET( pGT );

   /* A HACK - must be constructed differently - Still looking for the ways to control it */
   hb_gt_wvt_AddCharToInputQueue( pWVT, 27 );

   app->quit();
}

static void hbqt_Init( void * cargo )
{
   if( ! hbqtinit )
   {
      int argc;
      char ** argv;

      HB_SYMBOL_UNUSED( cargo );

      argc = hb_cmdargARGC();
      argv = hb_cmdargARGV();

      app = new QApplication( argc, argv );

      if( app )
         hbqtinit = true;

      if( ! hbqtinit )
         hb_errInternal( 11001, "hbqt_Init(): QT Initilization Error.", NULL, NULL );

      hb_cmdargInit( argc, argv );
   }
}

static void hb_gt_wvt_Init( PHB_GT pGT, HB_FHANDLE hFilenoStdin, HB_FHANDLE hFilenoStdout, HB_FHANDLE hFilenoStderr )
{
   HANDLE    hInstance = NULL;
   int       iCmdShow = 0;
   PHB_GTWVT pWVT = NULL;

   HB_TRACE( HB_TR_DEBUG, ( "hb_gt_wvt_Init(%p,%p,%p,%p)", pGT, ( void * ) ( HB_PTRDIFF ) hFilenoStdin, ( void * ) ( HB_PTRDIFF ) hFilenoStdout, ( void * ) ( HB_PTRDIFF ) hFilenoStderr ) );

   hbqt_Init( NULL );
   if( hbqtinit )
      pWVT = hb_gt_wvt_New( pGT, ( HINSTANCE ) hInstance, iCmdShow );
      if( !pWVT )
         hb_errInternal( 10001, "Maximum number of WVT windows reached, cannot create another one", NULL, NULL );

   HB_GTLOCAL( pGT ) = ( void * ) pWVT;

   /* SUPER GT initialization */
   HB_GTSUPER_INIT( pGT, hFilenoStdin, hFilenoStdout, hFilenoStderr );
   HB_GTSELF_RESIZE( pGT, pWVT->ROWS, pWVT->COLS );
   HB_GTSELF_SEMICOLD( pGT );
}

/* ********************************************************************** */

static void hb_gt_wvt_Exit( PHB_GT pGT )
{
   PHB_GTWVT pWVT;

   HB_TRACE(HB_TR_DEBUG, ("hb_gt_wvt_Exit(%p)", pGT));

   pWVT = HB_GTWVT_GET( pGT );
   HB_GTSUPER_EXIT( pGT );

   app->quit();

   if( pWVT )
      hb_gt_wvt_Free( pWVT );
}

/* ********************************************************************** */

static void hb_gt_wvt_Redraw( PHB_GT pGT, int iRow, int iCol, int iSize )
{
   PHB_GTWVT pWVT;

   HB_TRACE( HB_TR_DEBUG, ( "hb_gt_wvt_Redraw(%p,%d,%d,%d)", pGT, iRow, iCol, iSize ) );

   pWVT = HB_GTWVT_GET( pGT );
   if( pWVT )
   {
      if( pWVT->qWnd )
      {
         RECT rect;

         rect.top = rect.bottom = ( SHORT ) iRow;
         rect.left = ( SHORT ) iCol;
         rect.right = ( SHORT ) ( iCol + iSize - 1 );

         rect = hb_gt_wvt_GetXYFromColRowRect( pWVT, rect );

         /* Schedule a Repaint Event */
         pWVT->qWnd->update( QRect( iCol, iRow, iCol + iSize - 1, iRow ) );
      }
      else
         pWVT->fInit = TRUE;
   }
}

/* ********************************************************************** */

static void hb_gt_wvt_Refresh( PHB_GT pGT )
{
   PHB_GTWVT pWVT;

   HB_TRACE( HB_TR_DEBUG, ("hb_gt_wvt_Refresh(%p)", pGT) );

   HB_GTSUPER_REFRESH( pGT );

   pWVT = HB_GTWVT_GET( pGT );
   if( pWVT )
   {
      if( !pWVT->qWnd && pWVT->fInit )
         hb_gt_wvt_CreateConsoleWindow( pWVT );

      if( pWVT->qWnd )
      {
         app->processEvents();
      }
   }
}

/* ********************************************************************** */

static BOOL hb_gt_wvt_SetMode( PHB_GT pGT, int iRow, int iCol )
{
   PHB_GTWVT pWVT;
   BOOL fResult = FALSE;

   HB_TRACE( HB_TR_DEBUG, ( "hb_gt_wvt_SetMode(%p,%d,%d)", pGT, iRow, iCol ) );

   pWVT = HB_GTWVT_GET( pGT );

   if( iRow <= WVT_MAX_ROWS && iCol <= WVT_MAX_COLS )
   {
      if( pWVT->hWnd ) /* Is the window already open */
      {
         QFont *qFont = hb_gt_wvt_GetFont( pWVT->fontFace, pWVT->fontHeight, pWVT->fontWidth,
                                           pWVT->fontWeight, pWVT->fontQuality, pWVT->CodePage );

         if( qFont )
         {
            /*
             * make sure that the mode selected along with the current
             * font settings will fit in the window
             */
            if( hb_gt_wvt_ValidWindowSize( pWVT->hWnd, iRow, iCol, qFont, pWVT->fontWidth ) )
            {
               fResult = hb_gt_wvt_InitWindow( pWVT, iRow, iCol );
            }
            qFont->~QFont();
            HB_GTSELF_REFRESH( pGT );
         }
      }
      else
      {
         fResult = hb_gt_wvt_SetWindowSize( pWVT, iRow, iCol );
         HB_GTSELF_SEMICOLD( pGT );
      }
   }

   return fResult;
}

/* ********************************************************************** */

static const char * hb_gt_wvt_Version( PHB_GT pGT, int iType )
{
   HB_TRACE( HB_TR_DEBUG, ( "hb_gt_wvt_Version(%p,%d)", pGT, iType ) );

   HB_SYMBOL_UNUSED( pGT );

   if( iType == 0 )
      return HB_GT_DRVNAME( HB_GT_NAME );

   return "Harbour Terminal: Multi-Platform QT based GUI console (QTC)";
}

/* ********************************************************************** */

static int hb_gt_wvt_ReadKey( PHB_GT pGT, int iEventMask )
{
   PHB_GTWVT pWVT;
   int  c = 0;
   BOOL fKey;

   HB_TRACE( HB_TR_DEBUG, ( "hb_gt_wvt_ReadKey(%p,%d)", pGT, iEventMask ) );

   HB_SYMBOL_UNUSED( iEventMask ); /* we ignore the eventmask! */

   pWVT = HB_GTWVT_GET( pGT );

   if( pWVT->qWnd ) /* Is the window already open */
      hb_gt_wvt_ProcessMessages();

   fKey = hb_gt_wvt_GetCharFromInputQueue( pWVT, &c );

   return fKey ? c : 0;
}

/* ********************************************************************** */
/* dDuration is in 'Ticks' (18.2 per second) */
static void hb_gt_wvt_Tone( PHB_GT pGT, double dFrequency, double dDuration )
{
   HB_TRACE(HB_TR_DEBUG, ("hb_gt_wvt_Tone(%p,%lf,%lf)", pGT, dFrequency, dDuration));

   HB_SYMBOL_UNUSED( pGT );

   //hb_gt_winapi_tone( dFrequency, dDuration );  ???
}

/* ********************************************************************** */

static BOOL hb_gt_wvt_SetDispCP( PHB_GT pGT, const char * pszTermCDP, const char * pszHostCDP, BOOL fBox )
{
   HB_GTSUPER_SETDISPCP( pGT, pszTermCDP, pszHostCDP, fBox );

#ifndef HB_CDP_SUPPORT_OFF

   /*
    * We are displaying text in U16 so pszTermCDP is unimportant.
    * We only have to know what is the internal application codepage
    * to make proper translation
    */
   if( !pszHostCDP || !*pszHostCDP )
      pszHostCDP = hb_cdpID();

   if( pszHostCDP && *pszHostCDP )
   {
      PHB_CODEPAGE cdpHost = hb_cdpFind( pszHostCDP );
      if( cdpHost )
         HB_GTWVT_GET( pGT )->hostCDP = cdpHost;
   }

#endif

   return TRUE;
}

static BOOL hb_gt_wvt_SetKeyCP( PHB_GT pGT, const char * pszTermCDP, const char * pszHostCDP )
{
   HB_GTSUPER_SETKEYCP( pGT, pszTermCDP, pszHostCDP );

#ifndef HB_CDP_SUPPORT_OFF
   /*
    * We are receiving WM_CHAR events in U16 so pszTermCDP is unimportant.
    * We only have to know what is the internal application codepage
    * to make proper translation
    */
   if( !pszHostCDP || !*pszHostCDP )
      pszHostCDP = hb_cdpID();

   if( pszHostCDP && *pszHostCDP )
   {
      PHB_CODEPAGE cdpHost = hb_cdpFind( pszHostCDP );
      if( cdpHost )
         HB_GTWVT_GET( pGT )->inCDP = cdpHost;
   }
#endif

   return TRUE;
}

/* ********************************************************************** */

static BOOL hb_gt_wvt_mouse_IsPresent( PHB_GT pGT )
{
   HB_TRACE(HB_TR_DEBUG, ("hb_gt_wvt_mouse_IsPresent(%p)", pGT));

   HB_SYMBOL_UNUSED( pGT );

   return TRUE;
}

static void hb_gt_wvt_mouse_GetPos( PHB_GT pGT, int * piRow, int * piCol )
{
   PHB_GTWVT pWVT;

   HB_TRACE(HB_TR_DEBUG, ("hb_gt_wvt_mouse_GetPos(%p,%p,%p)", pGT, piRow, piCol));

   pWVT = HB_GTWVT_GET( pGT );
   *piRow = pWVT->MousePos.y;
   *piCol = pWVT->MousePos.x;
}

static BOOL hb_gt_wvt_mouse_ButtonState( PHB_GT pGT, int iButton )
{
   HB_TRACE( HB_TR_DEBUG, ("hb_gt_wvt_mouse_ButtonState(%p,%i)", pGT, iButton) );

   HB_SYMBOL_UNUSED( pGT );
   #if 0
   switch( iButton )
   {
      case 0:
         return ( GetKeyState( VK_LBUTTON ) & 0x8000 ) != 0;
      case 1:
         return ( GetKeyState( VK_RBUTTON ) & 0x8000 ) != 0;
      case 2:
         return ( GetKeyState( VK_MBUTTON ) & 0x8000 ) != 0;
   }
   #endif
   return FALSE;
}

static int hb_gt_wvt_mouse_CountButton( PHB_GT pGT )
{
   HB_TRACE( HB_TR_DEBUG, ("hb_gt_wvt_mouse_CountButton(%p)", pGT) );

   HB_SYMBOL_UNUSED( pGT );

   //return GetSystemMetrics( SM_CMOUSEBUTTONS );
   return 2;
}

/* ********************************************************************** */

static BOOL hb_gt_wvt_Info( PHB_GT pGT, int iType, PHB_GT_INFO pInfo )
{
   PHB_GTWVT pWVT;
   int iVal;

   HB_TRACE( HB_TR_DEBUG, ( "hb_gt_wvt_Info(%p,%d,%p)", pGT, iType, pInfo ) );

   pWVT = HB_GTWVT_GET( pGT );

   switch( iType )
   {
      case HB_GTI_FULLSCREEN:
      case HB_GTI_KBDSUPPORT:
      case HB_GTI_ISGRAPHIC:
         pInfo->pResult = hb_itemPutL( pInfo->pResult, TRUE );
         break;

      case HB_GTI_ISUNICODE:
         pInfo->pResult = hb_itemPutL( pInfo->pResult, TRUE );
         break;
#if 0
      case HB_GTI_INPUTFD:
         pInfo->pResult = hb_itemPutNInt( pInfo->pResult,
                              ( HB_PTRDIFF ) GetStdHandle( STD_INPUT_HANDLE ) );
         break;

      case HB_GTI_OUTPUTFD:
         pInfo->pResult = hb_itemPutNInt( pInfo->pResult,
                              ( HB_PTRDIFF ) GetStdHandle( STD_OUTPUT_HANDLE ) );
         break;

      case HB_GTI_ERRORFD:
         pInfo->pResult = hb_itemPutNInt( pInfo->pResult,
                              ( HB_PTRDIFF ) GetStdHandle( STD_ERROR_HANDLE ) );
         break;
#endif
      case HB_GTI_FONTSIZE:
         pInfo->pResult = hb_itemPutNI( pInfo->pResult, pWVT->PTEXTSIZE.y );
         iVal = hb_itemGetNI( pInfo->pNewVal );
         if( iVal > 0 )
         {
            QFont *qFont = hb_gt_wvt_GetFont( pWVT->fontFace, iVal, pWVT->fontWidth, pWVT->fontWeight, pWVT->fontQuality, pWVT->CodePage );
            if( qFont )
            {
               pWVT->fontHeight = 16;//iVal;
               if( pWVT->hWnd )
               {
                  hb_gt_wvt_ResetWindowSize( pWVT );
                  HB_GTSELF_REFRESH( pGT );
               }
            }
         }
         break;

      case HB_GTI_FONTWIDTH:
         pInfo->pResult = hb_itemPutNI( pInfo->pResult, pWVT->fontWidth );
         iVal = hb_itemGetNI( pInfo->pNewVal );
         if( iVal > 0 )
         {
            /* store font status for next operation on fontsize */
            pWVT->fontWidth = 8;//iVal;
         }
         break;

      case HB_GTI_FONTNAME:
         pInfo->pResult = hb_itemPutC( pInfo->pResult, pWVT->fontFace );
         if( hb_itemType( pInfo->pNewVal ) & HB_IT_STRING ) /* TODO */
         {
            hb_strncpy( pWVT->fontFace, hb_itemGetCPtr( pInfo->pNewVal ), sizeof( pWVT->fontFace ) - 1 );
         }
         break;

      case HB_GTI_FONTWEIGHT:
         switch( pWVT->fontWeight )
         {
            case FW_THIN:
            case FW_EXTRALIGHT:
            case FW_LIGHT:
               iVal = HB_GTI_FONTW_THIN;
            break;

            case FW_DONTCARE:
            case FW_NORMAL:
            case FW_MEDIUM:
               iVal = HB_GTI_FONTW_NORMAL;
            break;

            case FW_SEMIBOLD:
            case FW_BOLD:
            case FW_EXTRABOLD:
            case FW_HEAVY:
               iVal = HB_GTI_FONTW_BOLD;
            break;

            default:
               iVal = 0;
            break;
         }
         pInfo->pResult = hb_itemPutNI( pInfo->pResult, iVal );
         if( hb_itemType( pInfo->pNewVal ) & HB_IT_NUMERIC )
         {
            /* store font status for next operation on fontsize */
            switch( hb_itemGetNI( pInfo->pNewVal ) )
            {
               case HB_GTI_FONTW_THIN:
                  pWVT->fontWeight = FW_LIGHT;
                  break;
               case HB_GTI_FONTW_NORMAL:
                  pWVT->fontWeight = FW_NORMAL;
                  break;
               case HB_GTI_FONTW_BOLD:
                  pWVT->fontWeight = FW_BOLD;
                  break;
            }
         }
         break;

      case HB_GTI_FONTQUALITY:
         switch( pWVT->fontQuality )
         {
            case ANTIALIASED_QUALITY:
               iVal = HB_GTI_FONTQ_HIGH;
               break;
            case DEFAULT_QUALITY:
            case DRAFT_QUALITY:
               iVal = HB_GTI_FONTQ_NORMAL;
               break;
            case NONANTIALIASED_QUALITY:
            case PROOF_QUALITY:
               iVal = HB_GTI_FONTQ_DRAFT;
               break;
            default:
               iVal = 0;
               break;
         }
         pInfo->pResult = hb_itemPutNI( pInfo->pResult, iVal );
         if( hb_itemType( pInfo->pNewVal ) & HB_IT_NUMERIC )
         {
            switch( hb_itemGetNI( pInfo->pNewVal ) )
            {
               case HB_GTI_FONTQ_HIGH:
                  pWVT->fontQuality = ANTIALIASED_QUALITY;
                  break;
               case HB_GTI_FONTQ_NORMAL:
                  pWVT->fontQuality = DEFAULT_QUALITY;
                  break;
               case HB_GTI_FONTQ_DRAFT:
                  pWVT->fontQuality = DRAFT_QUALITY;
                  break;
            }
         }
         break;

      case HB_GTI_SCREENHEIGHT:
         pInfo->pResult = hb_itemPutNI( pInfo->pResult, pWVT->PTEXTSIZE.y * pWVT->ROWS );
         iVal = hb_itemGetNI( pInfo->pNewVal );
         if( iVal > 0 )
         {
            HB_GTSELF_SETMODE( pGT, ( USHORT ) ( iVal / pWVT->PTEXTSIZE.y ), pWVT->COLS );
         }
         break;

      case HB_GTI_SCREENWIDTH:
         pInfo->pResult = hb_itemPutNI( pInfo->pResult, pWVT->PTEXTSIZE.x * pWVT->COLS );
         iVal = hb_itemGetNI( pInfo->pNewVal );
         if( iVal > 0 )
         {
            HB_GTSELF_SETMODE( pGT, pWVT->ROWS, ( USHORT ) ( iVal / pWVT->PTEXTSIZE.x ) );
         }
         break;

      case HB_GTI_DESKTOPWIDTH:
      {
         QDesktopWidget *qDesk = new QDesktopWidget();
         pInfo->pResult = hb_itemPutNI( pInfo->pResult, qDesk->width() );
         break;
      }
      case HB_GTI_DESKTOPHEIGHT:
      {
         QDesktopWidget *qDesk = new QDesktopWidget();
         pInfo->pResult = hb_itemPutNI( pInfo->pResult, qDesk->height() );
         break;
      }
      case HB_GTI_DESKTOPCOLS:
      {
         QDesktopWidget *qDesk = new QDesktopWidget();
         pInfo->pResult = hb_itemPutNI( pInfo->pResult, qDesk->width() / pWVT->PTEXTSIZE.x );
         break;
      }
      case HB_GTI_DESKTOPROWS:
      {
         QDesktopWidget *qDesk = new QDesktopWidget();
         pInfo->pResult = hb_itemPutNI( pInfo->pResult, qDesk->height() / pWVT->PTEXTSIZE.y );
         break;
      }

      case HB_GTI_WINTITLE:
         if( pWVT->hWnd )
         {
            pInfo->pResult = hb_itemPutCPtr2( pInfo->pResult, pWVT->qWnd->windowTitle().toLatin1().data() );
            if( hb_itemType( pInfo->pNewVal ) & HB_IT_STRING )
               pWVT->qWnd->setWindowTitle( hb_itemGetCPtr( pInfo->pNewVal ) );
         }
         break;

      case HB_GTI_CODEPAGE:
         pInfo->pResult = hb_itemPutNI( pInfo->pResult, pWVT->CodePage );
         if( hb_itemType( pInfo->pNewVal ) & HB_IT_NUMERIC )
         {
            iVal = hb_itemGetNI( pInfo->pNewVal );
            if( iVal != pWVT->CodePage )
            {
               if( !pWVT->hWnd )
               {
                  pWVT->CodePage = iVal;
               }
               else
               {
                  QFont *qFont = hb_gt_wvt_GetFont( pWVT->fontFace, pWVT->fontHeight, pWVT->fontWidth,
                                                    pWVT->fontWeight, pWVT->fontQuality, iVal );
                  if( qFont )
                  {
                     if( pWVT->qFont )
                        pWVT->qFont->~QFont();

                     pWVT->qFont = qFont;
                     pWVT->CodePage = iVal;
                  }
               }
            }
         }
         break;

      case HB_GTI_BOXCP:
         pInfo->pResult = hb_itemPutC( pInfo->pResult,
                                       pWVT->boxCDP ? pWVT->boxCDP->id : NULL );
         if( hb_itemType( pInfo->pNewVal ) & HB_IT_STRING )
         {
            PHB_CODEPAGE cdpBox = hb_cdpFind( hb_itemGetCPtr( pInfo->pNewVal ) );
            if( cdpBox )
               pWVT->boxCDP = cdpBox;
         }
         break;

      case HB_GTI_ICONFILE:
      {
         if( ( hb_itemType( pInfo->pNewVal ) & HB_IT_STRING ) )
         {
            if( pWVT->qWnd )
            {
               pWVT->qWnd->setWindowIcon( QIcon( QString( hb_itemGetCPtr( pInfo->pNewVal ) ) ) );
            }
         }
         break;
      }
#if 0
      case HB_GTI_ICONRES:
      {
         if( hb_itemType( pInfo->pNewVal ) & HB_IT_STRING )
         {
            HICON hIconToFree = ( pWVT->hIcon && pWVT->bIconToFree ) ? pWVT->hIcon : NULL;
            LPTSTR lpIcon;

            lpIcon = HB_TCHAR_CONVTO( hb_itemGetCPtr( pInfo->pNewVal ) );
            pWVT->bIconToFree = FALSE;
            pWVT->hIcon = LoadIcon( pWVT->hInstance, lpIcon );
            HB_TCHAR_FREE( lpIcon );

            if( pWVT->hWnd )
            {
               SendNotifyMessage( pWVT->hWnd, WM_SETICON, ICON_SMALL, ( LPARAM ) pWVT->hIcon ); /* Set Title Bar Icon */
               SendNotifyMessage( pWVT->hWnd, WM_SETICON, ICON_BIG  , ( LPARAM ) pWVT->hIcon ); /* Set Task List Icon */
            }

            if( hIconToFree )
               DestroyIcon( hIconToFree );
         }
         else if( hb_itemType( pInfo->pNewVal ) & HB_IT_NUMERIC )
         {
            HICON hIconToFree = ( pWVT->hIcon && pWVT->bIconToFree ) ? pWVT->hIcon : NULL;

            pWVT->bIconToFree = FALSE;
            pWVT->hIcon = LoadIcon( pWVT->hInstance,
                                    MAKEINTRESOURCE( ( HB_LONG )
                                         hb_itemGetNInt( pInfo->pNewVal ) ) );

            if( pWVT->hWnd )
            {
               SendNotifyMessage( pWVT->hWnd, WM_SETICON, ICON_SMALL, ( LPARAM ) pWVT->hIcon ); /* Set Title Bar Icon */
               SendNotifyMessage( pWVT->hWnd, WM_SETICON, ICON_BIG  , ( LPARAM ) pWVT->hIcon ); /* Set Task List Icon */
            }

            if( hIconToFree )
               DestroyIcon( hIconToFree );
         }
         pInfo->pResult = hb_itemPutNInt( pInfo->pResult, ( HB_PTRDIFF ) pWVT->hIcon );
         break;
      }
#endif
      case HB_GTI_VIEWMAXWIDTH:
         pInfo->pResult = hb_itemPutNI( pInfo->pResult, pWVT->COLS );
         break;

      case HB_GTI_VIEWMAXHEIGHT:
         pInfo->pResult = hb_itemPutNI( pInfo->pResult, pWVT->ROWS );
         break;
#if 0
      case HB_GTI_KBDSHIFTS:
         pInfo->pResult = hb_itemPutNI( pInfo->pResult, hb_gt_winapi_getKbdState() );
         if( hb_itemType( pInfo->pNewVal ) & HB_IT_NUMERIC )
            hb_gt_winapi_setKbdState( hb_itemGetNI( pInfo->pNewVal ) );
         break;

      case HB_GTI_CLIPBOARDDATA:
         if( hb_itemType( pInfo->pNewVal ) & HB_IT_STRING )
         {
            hb_gt_winapi_setClipboard( pWVT->CodePage == OEM_CHARSET ?
                                       CF_OEMTEXT : CF_TEXT,
                                       hb_itemGetCPtr( pInfo->pNewVal ),
                                       hb_itemGetCLen( pInfo->pNewVal ) );
         }
         else
         {
            char * szClipboardData;
            ULONG ulLen;
            if( hb_gt_winapi_getClipboard( pWVT->CodePage == OEM_CHARSET ?
                                           CF_OEMTEXT : CF_TEXT,
                                           &szClipboardData, &ulLen ) )
            {
               pInfo->pResult = hb_itemPutCLPtr( pInfo->pResult,
                                                 szClipboardData,
                                                 ulLen );
            }
            else
            {
               pInfo->pResult = hb_itemPutC( pInfo->pResult, NULL );
            }
         }
         break;

      case HB_GTI_CURSORBLINKRATE:
         pInfo->pResult = hb_itemPutNI( pInfo->pResult, GetCaretBlinkTime() );
         if( hb_itemType( pInfo->pNewVal ) & HB_IT_NUMERIC )
            SetCaretBlinkTime( hb_itemGetNI( pInfo->pNewVal ) );
         break;
#endif
      case HB_GTI_SCREENSIZE:
      {
         int iX, iY;

         if( !pInfo->pResult )
         {
            pInfo->pResult = hb_itemNew( NULL );
         }
         hb_arrayNew( pInfo->pResult, 2 );
         hb_arraySetNI( pInfo->pResult, 2, pWVT->PTEXTSIZE.y * pWVT->ROWS );
         hb_arraySetNI( pInfo->pResult, 1, pWVT->PTEXTSIZE.x * pWVT->COLS );
         iY = hb_arrayGetNI( pInfo->pNewVal,2 );
         iX = hb_arrayGetNI( pInfo->pNewVal,1 );

         if( iY  > 0 )
         {
            BOOL bOldCentre = pWVT->CentreWindow;
            pWVT->CentreWindow = pWVT->bMaximized ? TRUE : FALSE;
            HB_GTSELF_SETMODE( pGT, ( USHORT ) ( iY / pWVT->PTEXTSIZE.y ), ( USHORT ) ( iX / pWVT->PTEXTSIZE.x ) );
            pWVT->CentreWindow = bOldCentre;
         }
         break;
      }
#if 0
      case HB_GTI_RESIZABLE:
         pInfo->pResult = hb_itemPutL( pInfo->pResult, pWVT->bResizable );
         if( pInfo->pNewVal )
         {
            BOOL bNewValue = hb_itemGetL( pInfo->pNewVal );
            if( bNewValue != pWVT->bResizable )
            {
               pWVT->bResizable = bNewValue;
               if( pWVT->hWnd )
               {
                  LONG_PTR style;
                  if( pWVT->bResizable )
                     style = WS_OVERLAPPED|WS_CAPTION|WS_SYSMENU|WS_MINIMIZEBOX|WS_MAXIMIZEBOX|WS_THICKFRAME;
                  else
                     style = WS_OVERLAPPED|WS_CAPTION|WS_SYSMENU|WS_MINIMIZEBOX|WS_BORDER;

                  SetWindowLongPtr( pWVT->hWnd, GWL_STYLE, style );

                  SetWindowPos( pWVT->hWnd, NULL, 0, 0, 0, 0,
                                SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_DEFERERASE );
                  ShowWindow( pWVT->hWnd, SW_HIDE );
                  ShowWindow( pWVT->hWnd, SW_NORMAL );
               }
            }
         }
         break;
#endif
      case HB_GTI_SELECTCOPY:
         pInfo->pResult = hb_itemPutL( pInfo->pResult, pWVT->bSelectCopy );

         if( hb_itemType( pInfo->pNewVal ) & HB_IT_STRING )
         {
            pInfo->pResult = hb_itemPutC( pInfo->pResult, pWVT->pszSelectCopy );

            if( hb_itemGetCLen( pInfo->pNewVal ) )
            {
               #if 0
               HMENU hSysMenu = pWVT->hWnd ? GetSystemMenu( pWVT->hWnd, FALSE ) : NULL;
               if( hSysMenu || !pWVT->hWnd )
               {
                  if( pWVT->pszSelectCopy )
                     hb_xfree( pWVT->pszSelectCopy );
                  pWVT->pszSelectCopy = hb_strdup( hb_itemGetCPtr( pInfo->pNewVal ) );
                  pWVT->bSelectCopy = TRUE;
               }
               #endif
               pWVT->pszSelectCopy = hb_strdup( hb_itemGetCPtr( pInfo->pNewVal ) );
               pWVT->bSelectCopy = TRUE;
            }
         }
         else if( pInfo->pNewVal )
         {
            BOOL bNewValue = hb_itemGetL( pInfo->pNewVal );
            if( bNewValue != pWVT->bSelectCopy )
            {
               if( pWVT->hWnd )
               {
                  #if 0
                  HMENU hSysMenu = GetSystemMenu( pWVT->hWnd, FALSE );
                  if( hSysMenu )
                  {
                     EnableMenuItem( hSysMenu, SYS_EV_MARK, MF_BYCOMMAND | ( bNewValue ? MF_ENABLED : MF_GRAYED ) );
                     pWVT->bSelectCopy = bNewValue;
                  }
                  #endif
                  pWVT->bSelectCopy = bNewValue;
               }
               else
                  pWVT->bSelectCopy = bNewValue;
            }
         }
         break;

      case HB_GTI_CLOSABLE:
         pInfo->pResult = hb_itemPutL( pInfo->pResult, pWVT->bClosable );
         if( pInfo->pNewVal )
         {
            BOOL bNewValue = hb_itemGetL( pInfo->pNewVal );
            if( bNewValue != pWVT->bClosable )
            {
               if( pWVT->qWnd )
               {
                  #if 0
                  HMENU hSysMenu = GetSystemMenu( pWVT->hWnd, FALSE );
                  if( hSysMenu )
                  {
                     EnableMenuItem( hSysMenu, SC_CLOSE, MF_BYCOMMAND | ( bNewValue ? MF_ENABLED : MF_GRAYED ) );
                     pWVT->bClosable = bNewValue;
                  }
                  #endif
                  pWVT->bClosable = bNewValue;
               }
               else
                  pWVT->bClosable = bNewValue;
            }
         }
         break;

      case HB_GTI_PALETTE:
         if( hb_itemType( pInfo->pNewVal ) & HB_IT_NUMERIC )
         {
            int iIndex = hb_itemGetNI( pInfo->pNewVal );

            if( iIndex >= 0 && iIndex < 16 )
            {
               pInfo->pResult = hb_itemPutNL( pInfo->pResult, pWVT->COLORS[ iIndex ] );

               if( hb_itemType( pInfo->pNewVal2 ) & HB_IT_NUMERIC )
               {
                  pWVT->COLORS[ iIndex ] = hb_itemGetNL( pInfo->pNewVal2 );

                  if( pWVT->hWnd )
                     HB_GTSELF_EXPOSEAREA( pWVT->pGT, 0, 0, pWVT->ROWS, pWVT->COLS );
               }
            }
         }
         else
         {
            int i;
            if( !pInfo->pResult )
            {
               pInfo->pResult = hb_itemNew( NULL );
            }
            hb_arrayNew( pInfo->pResult, 16 );
            for( i = 0; i < 16; i++ )
               hb_arraySetNL( pInfo->pResult, i + 1, pWVT->COLORS[ i ] );

            if( hb_itemType( pInfo->pNewVal ) & HB_IT_ARRAY )
            {
               if( hb_arrayLen( pInfo->pNewVal ) == 16 )
               {
                  for( i = 0; i < 16; i++ )
                     pWVT->COLORS[ i ] = hb_arrayGetNL( pInfo->pNewVal, i + 1 );

                  if( pWVT->hWnd )
                     HB_GTSELF_EXPOSEAREA( pWVT->pGT, 0, 0, pWVT->ROWS, pWVT->COLS );
               }
            }
         }
         break;

      case HB_GTI_RESIZEMODE:
         pInfo->pResult = hb_itemPutNI( pInfo->pResult, pWVT->ResizeMode );
         if( hb_itemType( pInfo->pNewVal ) & HB_IT_NUMERIC )
         {
            pWVT->ResizeMode = hb_itemGetNI( pInfo->pNewVal );
         }
         break;

      case HB_GTI_SETPOS_XY:
      case HB_GTI_SETPOS_ROWCOL:
         if( pWVT->hWnd && ( hb_itemType( pInfo->pNewVal ) & HB_IT_NUMERIC ) &&
                           ( hb_itemType( pInfo->pNewVal2 ) & HB_IT_NUMERIC ) )
         {
            int x, y;
            RECT rect = { 0,0,0,0 };
            GetWindowRect( pWVT->hWnd, &rect );

            if( iType == HB_GTI_SETPOS_ROWCOL )
            {
               y = hb_itemGetNI( pInfo->pNewVal ) * pWVT->PTEXTSIZE.y;
               x = hb_itemGetNI( pInfo->pNewVal2 ) * pWVT->PTEXTSIZE.x;
            }
            else
            {
               x = hb_itemGetNI( pInfo->pNewVal );
               y = hb_itemGetNI( pInfo->pNewVal2 );
            }
            pWVT->qWnd->move( x,y );
         }
         break;

      default:
         return HB_GTSUPER_INFO( pGT, iType, pInfo );
   }

   return TRUE;
}

/* ********************************************************************** */

static BOOL hb_gt_FuncInit( PHB_GT_FUNCS pFuncTable )
{
   HB_TRACE(HB_TR_DEBUG, ("hb_gt_FuncInit(%p)", pFuncTable));

   pFuncTable->Init                 = hb_gt_wvt_Init;
   pFuncTable->Exit                 = hb_gt_wvt_Exit;
   pFuncTable->SetMode              = hb_gt_wvt_SetMode;
   pFuncTable->Redraw               = hb_gt_wvt_Redraw;
   pFuncTable->Refresh              = hb_gt_wvt_Refresh;
   pFuncTable->Version              = hb_gt_wvt_Version;
   pFuncTable->Tone                 = hb_gt_wvt_Tone;
   pFuncTable->Info                 = hb_gt_wvt_Info;
   pFuncTable->SetDispCP            = hb_gt_wvt_SetDispCP;
   pFuncTable->SetKeyCP             = hb_gt_wvt_SetKeyCP;
   pFuncTable->ReadKey              = hb_gt_wvt_ReadKey;

   pFuncTable->MouseIsPresent       = hb_gt_wvt_mouse_IsPresent;
   pFuncTable->MouseGetPos          = hb_gt_wvt_mouse_GetPos;
   pFuncTable->MouseButtonState     = hb_gt_wvt_mouse_ButtonState;
   pFuncTable->MouseCountButton     = hb_gt_wvt_mouse_CountButton;

   return TRUE;
}

/* ********************************************************************** */

static const HB_GT_INIT gtInit = { HB_GT_DRVNAME( HB_GT_NAME ),
                                   hb_gt_FuncInit,
                                   HB_GTSUPER,
                                   HB_GTID_PTR };

HB_GT_ANNOUNCE( HB_GT_NAME )

HB_CALL_ON_STARTUP_BEGIN( _hb_startup_gt_Init_ )
   hb_gtRegister( &gtInit );
HB_CALL_ON_STARTUP_END( _hb_startup_gt_Init_ )

#if defined( HB_PRAGMA_STARTUP )
   #pragma startup _hb_startup_gt_Init_
#elif defined( HB_MSC_STARTUP )
   #if defined( HB_OS_WIN_64 )
      #pragma section( HB_MSC_START_SEGMENT, long, read )
   #endif
   #pragma data_seg( HB_MSC_START_SEGMENT )
   static HB_$INITSYM hb_vm_auto__hb_startup_gt_Init_ = _hb_startup_gt_Init_;
   #pragma data_seg()
#endif

/* ********************************************************************** */

ConsoleArea::ConsoleArea(QWidget *parent)
    : QWidget(parent)
{
   setAttribute(Qt::WA_StaticContents);
   modified   = false;
   scribbling = false;
   myPenWidth = 1;
   myPenColor = Qt::blue;

   COLORS[ 0] = BLACK;
   COLORS[ 1] = BLUE;
   COLORS[ 2] = GREEN;
   COLORS[ 3] = CYAN;
   COLORS[ 4] = RED;
   COLORS[ 5] = MAGENTA;
   COLORS[ 6] = BROWN;
   COLORS[ 7] = WHITE;
   COLORS[ 8] = LIGHT_GRAY;
   COLORS[ 9] = BRIGHT_BLUE;
   COLORS[10] = BRIGHT_GREEN;
   COLORS[11] = BRIGHT_CYAN;
   COLORS[12] = BRIGHT_RED;
   COLORS[13] = BRIGHT_MAGENTA;
   COLORS[14] = YELLOW;
   COLORS[15] = BRIGHT_WHITE;

   setFocusPolicy(Qt::StrongFocus);
   // grabKeyboard();
}

void ConsoleArea::sizeByFont(void)
{
   QPainter painter( this );
   qFont = QFont( tr("Courier New"), 12, -1, FALSE );
   qFont = QFont( qFont, painter.device() );
   QFontMetrics fontMetrics( qFont );
   fontHeight = fontMetrics.height();
   fontWidth  = fontMetrics.averageCharWidth();
   resize( fontWidth*80, fontHeight*25 );
}

bool ConsoleArea::openImage(const QString &fileName)
{
    QImage loadedImage;
    if (!loadedImage.load(fileName))
        return false;

    QSize newSize = loadedImage.size().expandedTo(size());
    resizeImage(&loadedImage, newSize);
    image = loadedImage;
    modified = false;
    update();
    return true;
}

bool ConsoleArea::saveImage(const QString &fileName, const char *fileFormat)
{
    QImage visibleImage = image;
    resizeImage(&visibleImage, size());

    if (visibleImage.save(fileName, fileFormat)) {
        modified = false;
        return true;
    } else {
        return false;
    }
}

void ConsoleArea::setPenColor(const QColor &newColor)
{
    myPenColor = newColor;
}

void ConsoleArea::setPenWidth(int newWidth)
{
    myPenWidth = newWidth;
}

void ConsoleArea::clearImage()
{
    image.fill(qRgb(255, 255, 255));
    modified = true;
    update();
}

void ConsoleArea::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        lastPoint = event->pos();
        scribbling = true;
    }
}

void ConsoleArea::keyPressEvent(QKeyEvent *event)
{
   OutputDebugString( "Key Pressed" );
}

void ConsoleArea::keyReleaseEvent(QKeyEvent *event)
{
   OutputDebugString( "Key Released" );
}

void ConsoleArea::mouseMoveEvent(QMouseEvent *event)
{
    if ((event->buttons() & Qt::LeftButton) && scribbling)
        drawLineTo(event->pos());
}

void ConsoleArea::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton && scribbling) {
        drawLineTo(event->pos());
        scribbling = false;
    }
}

void ConsoleArea::paintEvent(QPaintEvent * event)
{
OutputDebugString( "               entry" );
    QPainter painter(this);
    QRect rect = event->rect();
    //painter.drawImage(QPoint(0, 0), image);
    {
       int hgt = height();
       int fntHeight = (hgt/25);
       fntHeight = 14;
       QFont font( tr( "Courier New" ), fntHeight, 10, FALSE );
       font = QFont(font, painter.device());
       QFontMetrics fontMetrics( font );
       int height = fontMetrics.height();
       painter.setFont(font);
       painter.setBackgroundMode(Qt::OpaqueMode);
       {
          int    iRow,iCol,startCol,len ;
          int    iTop;
          USHORT usChar;
          BYTE   bColor, bAttr, bOldColor = 0;
          char   text[ 4000 ];

          iTop = 0;
          for( iRow = 0; iRow <= 24; iRow++ )
          {
             iCol = startCol = 0;
             len  = 0;
             iTop = ( iRow * height ) + height;

             while( iCol <= 79 )
             {
                if( !HB_GTSELF_GETSCRCHAR( pGT, iRow, iCol, &bColor, &bAttr, &usChar ) )
                   break;

                if( len == 0 )
                {
                   bOldColor = bColor;
                }
                else if( bColor != bOldColor )
                {
                   QPen pen( COLORS[ bOldColor & 0x0F ] );
                   painter.setPen( pen );
                   QBrush brush( COLORS[ bOldColor >> 4 ] );
                   painter.setBackground( brush );

OutputDebugString( "KKKK len=%i bOldColor=%i bColor=%i" );

                   painter.drawText( QPointF( startCol,iTop ), QString( text ) );

                   bOldColor = bColor;
                   startCol  = iCol;
                   len       = 0;
                }
                text[ len++ ] = ( char ) usChar;
                iCol++;
             }
             if( len > 0 )
             {
                QPen pen( COLORS[ bOldColor & 0x0F ] );
                painter.setPen( pen );
                QBrush brush( COLORS[ bOldColor >> 4 ] );
                painter.setBackground( brush );
//OutputDebugString( "mmmmmm len=%i bOldColor=%i bColor=%i" );
                painter.drawText(QPointF(startCol,iTop),QString(text));
             }
          }
       }
    }
OutputDebugString( "               exit" );
}

void ConsoleArea::resizeEvent(QResizeEvent *event)
{
    if (width() > image.width() || height() > image.height()) {
        int newWidth = qMax(width() + 128, image.width());
        int newHeight = qMax(height() + 128, image.height());
        resizeImage(&image, QSize(newWidth, newHeight));
        update();
    }
    QWidget::resizeEvent(event);
}

void ConsoleArea::drawLineTo(const QPoint &endPoint)
{
    QPainter painter(&image);
    painter.setPen(QPen(myPenColor, myPenWidth, Qt::SolidLine, Qt::RoundCap,
                        Qt::RoundJoin));
    painter.drawLine(lastPoint, endPoint);
    modified = true;

    int rad = (myPenWidth / 2) + 2;
    update(QRect(lastPoint, endPoint).normalized().adjusted(-rad, -rad, +rad, +rad));
    lastPoint = endPoint;
}

void ConsoleArea::resizeImage(QImage *image, const QSize &newSize)
{
    if (image->size() == newSize)
        return;

    QImage newImage(newSize, QImage::Format_RGB32);
    newImage.fill(qRgb(255, 255, 255));
    QPainter painter(&newImage);
    painter.drawImage(QPoint(0, 0), *image);
    *image = newImage;
}

void ConsoleArea::print()
{
#ifndef QT_NO_PRINTER
    QPrinter printer(QPrinter::HighResolution);

    QPrintDialog *printDialog = new QPrintDialog(&printer, this);
    if (printDialog->exec() == QDialog::Accepted) {
        QPainter painter(&printer);
        QRect rect = painter.viewport();
        QSize size = image.size();
        size.scale(rect.size(), Qt::KeepAspectRatio);
        painter.setViewport(rect.x(), rect.y(), size.width(), size.height());
        painter.setWindow(image.rect());
        painter.drawImage(0, 0, image);
    }
#endif // QT_NO_PRINTER
}

/*----------------------------------------------------------------------*/
/*
 *
 *
/*----------------------------------------------------------------------*/

MainWindow::MainWindow()
{
    consoleArea = new ConsoleArea;
    setCentralWidget(consoleArea);

    createActions();
//    createMenus();
}

void MainWindow::closeEvent(QCloseEvent *event)
{
   event->accept();
   hbqt_exit( pGT );
#if 0
    if (maybeSave()) {
        event->accept();
    } else {
        event->ignore();
    }
#endif
}

void MainWindow::open()
{
    if (maybeSave()) {
        QString fileName = QFileDialog::getOpenFileName(this,
                                   tr("Open File"), QDir::currentPath());
        if (!fileName.isEmpty())
            consoleArea->openImage(fileName);
    }
}

void MainWindow::save()
{
    QAction *action = qobject_cast<QAction *>(sender());
    QByteArray fileFormat = action->data().toByteArray();
    saveFile(fileFormat);
}

void MainWindow::penColor()
{
    QColor newColor = QColorDialog::getColor(consoleArea->penColor());
    if (newColor.isValid())
        consoleArea->setPenColor(newColor);
}

void MainWindow::penWidth()
{
    bool ok;
    int newWidth = QInputDialog::getInteger(this, tr("Scribble"),
                                            tr("Select pen width:"),
                                            consoleArea->penWidth(),
                                            1, 50, 1, &ok);
    if (ok)
        consoleArea->setPenWidth(newWidth);
}

void MainWindow::about()
{
    QMessageBox::about(this, tr("About Scribble"),
            tr("<p>The <b>Console</b> example shows how to use QMainWindow as the "
               "base widget for an application, and how to reimplement some of "
               "QWidget's event handlers to receive the events generated for "
               "the application's widgets:</p><p> We reimplement the mouse event "
               "handlers to facilitate drawing, the paint event handler to "
               "update the application and the resize event handler to optimize "
               "the application's appearance. In addition we reimplement the "
               "close event handler to intercept the close events before "
               "terminating the application.</p><p> The example also demonstrates "
               "how to use QPainter to draw an image in real time, as well as "
               "to repaint widgets.</p>"));
}

void MainWindow::createActions()
{
    openAct = new QAction(tr("&Open..."), this);
    openAct->setShortcut(tr("Ctrl+O"));
    connect(openAct, SIGNAL(triggered()), this, SLOT(open()));

    foreach (QByteArray format, QImageWriter::supportedImageFormats())
    {
        QString text = tr("%1...").arg(QString(format).toUpper());

        QAction *action = new QAction(text, this);
        action->setData(format);
        connect(action, SIGNAL(triggered()), this, SLOT(save()));
        saveAsActs.append(action);
    }

    printAct = new QAction(tr("&Print..."), this);
    connect(printAct, SIGNAL(triggered()), consoleArea, SLOT(print()));

    exitAct = new QAction(tr("E&xit"), this);
    exitAct->setShortcut(tr("Ctrl+Q"));
    connect(exitAct, SIGNAL(triggered()), this, SLOT(close()));

    penColorAct = new QAction(tr("&Pen Color..."), this);
    connect(penColorAct, SIGNAL(triggered()), this, SLOT(penColor()));

    penWidthAct = new QAction(tr("Pen &Width..."), this);
    connect(penWidthAct, SIGNAL(triggered()), this, SLOT(penWidth()));

    clearScreenAct = new QAction(tr("&Clear Screen"), this);
    clearScreenAct->setShortcut(tr("Ctrl+L"));
    connect(clearScreenAct, SIGNAL(triggered()),
            consoleArea, SLOT(clearImage()));

    aboutAct = new QAction(tr("&About"), this);
    connect(aboutAct, SIGNAL(triggered()), this, SLOT(about()));

    aboutQtAct = new QAction(tr("About &Qt"), this);
    connect(aboutQtAct, SIGNAL(triggered()), qApp, SLOT(aboutQt()));
}

void MainWindow::createMenus()
{
    saveAsMenu = new QMenu(tr("&Save As"), this);
    foreach (QAction *action, saveAsActs)
        saveAsMenu->addAction(action);

    fileMenu = new QMenu(tr("&File"), this);
    fileMenu->addAction(openAct);
    fileMenu->addMenu(saveAsMenu);
    fileMenu->addAction(printAct);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAct);

    optionMenu = new QMenu(tr("&Options"), this);
    optionMenu->addAction(penColorAct);
    optionMenu->addAction(penWidthAct);
    optionMenu->addSeparator();
    optionMenu->addAction(clearScreenAct);

    helpMenu = new QMenu(tr("&Help"), this);
    helpMenu->addAction(aboutAct);
    helpMenu->addAction(aboutQtAct);

    menuBar()->addMenu(fileMenu);
    menuBar()->addMenu(optionMenu);
    menuBar()->addMenu(helpMenu);
}

bool MainWindow::maybeSave()
{
    if (consoleArea->isModified()) {
       QMessageBox::StandardButton ret;
       ret = QMessageBox::warning(this, tr("Scribble"),
                          tr("The image has been modified.\n"
                             "Do you want to save your changes?"),
                          QMessageBox::Save | QMessageBox::Discard
                          | QMessageBox::Cancel);
        if (ret == QMessageBox::Save) {
            return saveFile("png");
        } else if (ret == QMessageBox::Cancel) {
            return false;
        }
    }
    return true;
}

bool MainWindow::saveFile(const QByteArray &fileFormat)
{
    QString initialPath = QDir::currentPath() + "/untitled." + fileFormat;

    QString fileName = QFileDialog::getSaveFileName(this, tr("Save As"),
                               initialPath,
                               tr("%1 Files (*.%2);;All Files (*)")
                               .arg(QString(fileFormat.toUpper()))
                               .arg(QString(fileFormat)));
    if (fileName.isEmpty()) {
        return false;
    } else {
        return consoleArea->saveImage(fileName, fileFormat);
    }
}

/*----------------------------------------------------------------------*/

