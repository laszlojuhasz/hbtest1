/*
 * $Id$
 */

/*
 * Harbour Project source code:
 * Video subsystem for Windows using GUI windows instead of Console
 *
 *    Copyright 2007 Pritpal Bedi <pritpal@vouchcac.com>
 * based on:
 *
 *    Copyright 2003 Peter Rees <peter@rees.co.nz>
 *                    Rees Software & Systems Ltd
 * based on
 *   Bcc ConIO Video subsystem by
 *     Copyright 2002 Marek Paliwoda <paliwoda@inteia.pl>
 *     Copyright 2002 Przemyslaw Czerpak <druzus@polbox.com>
 *   Video subsystem for Windows compilers
 *     Copyright 1999-2000 Paul Tucker <ptucker@sympatico.ca>
 *     Copyright 2002 Przemyslaw Czerpak <druzus@polbox.com>
 *
 * See doc/license.txt for licensing terms.
 *
 * www - http://www.harbour-project.org
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
//-------------------------------------------------------------------//
//-------------------------------------------------------------------//
//-------------------------------------------------------------------//
//-------------------------------------------------------------------//
//
//             Direct WinApi Functions - Prefixed WIN_*()
//
//-------------------------------------------------------------------//
//-------------------------------------------------------------------//
//-------------------------------------------------------------------//

#define HB_OS_WIN_USED

#include "gtwvg.h"
#include "hbwapi.h"
#include <windowsx.h>

#define WIN_STATUSBAR_MAX_PARTS         256

//----------------------------------------------------------------------//

#define wvg_parwparam( n )  ( ( WPARAM )  ( HB_PTRDIFF ) hb_parnint( n ) )
#define wvg_parlparam( n )  ( ( LPARAM )  ( HB_PTRDIFF ) hb_parnint( n ) )
#define wvg_parhandle( n )  ( ( HANDLE )  ( HB_PTRDIFF ) hb_parnint( n ) )
#define wvg_parhwnd( n )    ( ( HWND )    ( HB_PTRDIFF ) hb_parnint( n ) )
#define wvg_parwndproc( n ) ( ( WNDPROC ) ( HB_PTRDIFF ) hb_parnint( n ) )
#define wvg_parhdc( n )     ( ( HDC )     ( HB_PTRDIFF ) hb_parnint( n ) )
#define wvg_parcolor( n )   ( ( COLORREF )( HB_PTRDIFF ) hb_parnint( n ) )

#define wvg_rethandle( n )  ( hb_retnint( ( HB_PTRDIFF ) n ) )

//----------------------------------------------------------------------//

#if defined(__BORLANDC__) && !defined(HB_ARCH_64BIT)
    #undef MAKELONG
    #define MAKELONG(a,b) ((LONG)(((WORD)((DWORD_PTR)(a) & 0xffff)) | \
                          (((DWORD)((WORD)((DWORD_PTR)(b) & 0xffff))) << 16)))
#endif

//----------------------------------------------------------------------//

static HANDLE wvg_hInstance( void )
{
   HANDLE hInstance;

   hb_winmainArgGet( &hInstance, NULL, NULL );

   return hInstance;
}

//----------------------------------------------------------------------//

HB_FUNC( WVG_HINSTANCE )
{
   wapi_ret_HANDLE( wvg_hInstance() );
}

/*----------------------------------------------------------------------*/

HB_FUNC( WIN_SENDMESSAGE )
{
   LPTSTR cText = NULL;

   if( ISBYREF( 4 ) )
   {
      cText = HB_TCHAR_CONVTO( hb_parc( 4 ) );
   }

   hb_retnl( ( ULONG ) SendMessage( ( HWND ) ( HB_PTRDIFF ) hb_parnint( 1 ),
                                    ( UINT ) hb_parni( 2 ),
                                    ( ISNIL( 3 ) ? 0 : ( WPARAM ) hb_parnint( 3 ) ),
                                    ( ISNIL( 4 ) ? 0 : ( cText ? ( LPARAM ) ( LPSTR ) cText :
                                       ( ISCHAR( 4 ) ? ( LPARAM )( LPSTR ) hb_parc( 4 ) :
                                           ( LPARAM ) hb_parnint( 4 ) ) ) ) )
           );

   if( cText )
   {
      char * szText = HB_TCHAR_CONVFROM( cText );
      hb_storc( szText, 4 );
      HB_TCHAR_FREE( szText );
      HB_TCHAR_FREE( cText );
   }
}

//-------------------------------------------------------------------//

HB_FUNC( WIN_SENDDLGITEMMESSAGE )
{
   PHB_ITEM pText = hb_param( 5, HB_IT_STRING );
   char     *cText = NULL;
   int      iLen = 0;

   if( pText )
   {
      iLen  = hb_itemGetCLen( pText );
      cText = (char*) hb_xgrab( iLen+1 );
      hb_xmemcpy( cText, hb_itemGetCPtr( pText ), iLen+1 );
   }

   hb_retnl( (LONG) SendDlgItemMessage( (HWND) ( HB_PTRDIFF ) hb_parnint( 1 ) ,
                                        (int)  hb_parni( 2 ) ,
                                        (UINT) hb_parni( 3 ) ,
                                        (ISNIL(4) ? 0 : (WPARAM) hb_parnint( 4 ))   ,
                                        (cText ? (LPARAM) cText : (LPARAM) hb_parnint( 5 ))
                                      ) );

   if( cText )
   {
      if( ISBYREF( 5 ) )
      {
         hb_storclen( cText, iLen, 5 ) ;
      }
      hb_xfree( cText );
   }
}

//-------------------------------------------------------------------//
//
//  WIN_SetTimer( hWnd, nIdentifier, nTimeOut )
//
HB_FUNC( WIN_SETTIMER )
{
   hb_retl( SetTimer( ( HWND ) ( HB_PTRDIFF ) hb_parnint( 1 ), hb_parni( 2 ), hb_parni( 3 ), NULL ) != 0 );
}

//-------------------------------------------------------------------//

HB_FUNC( WIN_SETFOCUS )
{
   SetFocus( ( HWND ) ( HB_PTRDIFF ) hb_parnint( 1 ) );
}

//-------------------------------------------------------------------//

HB_FUNC( WIN_SETTEXTCOLOR )
{
   hb_retnl( ( ULONG ) SetTextColor( ( HDC ) ( HB_PTRDIFF ) hb_parnint( 1 ), ( COLORREF ) hb_parnl( 2 ) ) );
}

//-------------------------------------------------------------------//

HB_FUNC( WIN_SETBKCOLOR )
{
   hb_retnl( ( ULONG ) SetBkColor( ( HDC ) ( HB_PTRDIFF ) hb_parnint( 1 ), ( COLORREF ) hb_parnl( 2 ) ) );
}

//-------------------------------------------------------------------//
#if 0
HB_FUNC( WIN_SETBKMODE )
{
   hb_retni( ( int ) SetBkMode( ( HDC ) ( HB_PTRDIFF ) hb_parnint( 1 ), hb_parni( 2 ) ) );
}
#endif
//-------------------------------------------------------------------//

HB_FUNC( WIN_GETSTOCKOBJECT )
{
   hb_retnint( ( HB_PTRDIFF ) GetStockObject( hb_parni( 1 ) ) );
}

//-------------------------------------------------------------------//

HB_FUNC( WIN_DELETEOBJECT )
{
   hb_retl( DeleteObject( ( HGDIOBJ ) ( HB_PTRDIFF ) hb_parnint( 1 ) ) );
}

//-------------------------------------------------------------------//

HB_FUNC( WIN_SELECTOBJECT )
{
   hb_retnint( ( HB_PTRDIFF ) SelectObject( ( HDC ) ( HB_PTRDIFF ) hb_parnint( 1 ), ( HGDIOBJ ) ( HB_PTRDIFF ) hb_parnint( 2 ) ) );
}

//-------------------------------------------------------------------//

HB_FUNC( WIN_LOWORD )
{
   hb_retnl( LOWORD( hb_parnl( 1 ) ) );
}

//-------------------------------------------------------------------//

HB_FUNC( WIN_HIWORD )
{
   hb_retnl( HIWORD( hb_parnl( 1 ) ) );
}

//-------------------------------------------------------------------//
#if 0
HB_FUNC( WIN_MULDIV )
{
   hb_retni( MulDiv( hb_parni( 1 ), hb_parni( 2 ), hb_parni( 3 ) ) );
}
#endif
//-------------------------------------------------------------------//

HB_FUNC( WIN_GETDIALOGBASEUNITS )
{
   hb_retnl( ( LONG ) GetDialogBaseUnits() ) ;
}

//-------------------------------------------------------------------//

HB_FUNC( WIN_SETDLGITEMTEXT )
{
   LPTSTR lpBuffer = HB_TCHAR_CONVTO( hb_parc( 3 ) );
   SetDlgItemText( ( HWND ) ( HB_PTRDIFF ) hb_parnint( 1 ), hb_parni( 2 ), lpBuffer );
   HB_TCHAR_FREE( lpBuffer );
}

//-------------------------------------------------------------------//

HB_FUNC( WIN_GETDLGITEMTEXT )
{
   USHORT iLen = ( USHORT ) SendMessage( GetDlgItem( ( HWND ) ( HB_PTRDIFF ) hb_parnint( 1 ), hb_parni( 2 ) ), WM_GETTEXTLENGTH, 0, 0 ) + 1 ;
   LPTSTR cText = ( LPTSTR ) hb_xgrab( iLen * sizeof( TCHAR ) );
   char * szText;
   UINT iResult;

   iResult = GetDlgItemText( ( HWND ) ( HB_PTRDIFF ) hb_parnint( 1 ), // handle of dialog box
                             hb_parni( 2 ),            // identifier of control
                             cText,                    // address of buffer for text
                             iLen                      // maximum size of string
                            );

   cText[ iResult ] = '\0';
   szText = HB_TCHAR_CONVFROM( cText );
   hb_retc( szText );
   HB_TCHAR_FREE( szText );
   hb_xfree( cText );
}

//-------------------------------------------------------------------//

HB_FUNC( WIN_CHECKDLGBUTTON )
{
   hb_retl( CheckDlgButton( ( HWND ) ( HB_PTRDIFF ) hb_parnint( 1 ), hb_parni( 2 ),
                            ( UINT )( ISNUM( 3 ) ? hb_parni( 3 ) : hb_parl( 3 ) ) ) );
}

//-------------------------------------------------------------------//

HB_FUNC( WIN_ISDLGBUTTONCHECKED )
{
   hb_retni( IsDlgButtonChecked( ( HWND ) ( HB_PTRDIFF ) hb_parnint( 1 ), hb_parni( 2 ) ) ) ;
}

//-------------------------------------------------------------------//

HB_FUNC( WIN_CHECKRADIOBUTTON )
{
    hb_retl( CheckRadioButton( ( HWND ) ( HB_PTRDIFF ) hb_parnint( 1 ), // handle of dialog box
                                        hb_parni( 2 ),   // identifier of first radio button in group
                                        hb_parni( 3 ),   // identifier of last radio button in group
                                        hb_parni( 4 )    // identifier of radio button to select
                              ) );
}

//-------------------------------------------------------------------//

HB_FUNC( WIN_GETDLGITEM )
{
   hb_retnint( ( HB_PTRDIFF ) GetDlgItem( ( HWND ) ( HB_PTRDIFF ) hb_parnint( 1 ), hb_parni( 2 ) ) );
}

//-------------------------------------------------------------------//

HB_FUNC( WIN_MESSAGEBOX )
{
   HWND   hWnd = ISNIL( 1 ) ? GetActiveWindow() : ( HWND ) ( HB_PTRDIFF ) hb_parnint( 1 ) ;
   LPTSTR lpMsg = HB_TCHAR_CONVTO( ISNIL( 2 ) ? "" : hb_parc( 2 ) );
   LPTSTR lpTitle = HB_TCHAR_CONVTO( ISNIL( 3 ) ? "Info" : hb_parc( 3 ) );

   hb_retni( MessageBox( hWnd, lpMsg, lpTitle, ISNIL( 4 ) ? MB_OK : hb_parni( 4 ) ) );

   HB_TCHAR_FREE( lpTitle );
   HB_TCHAR_FREE( lpMsg );
}

//-------------------------------------------------------------------//

HB_FUNC( WIN_INVALIDATERECT )
{
   if( ISNIL( 2 ) )
      hb_retl( InvalidateRect( ( HWND ) ( HB_PTRDIFF ) hb_parnint( 1 ), NULL, TRUE ) );
   else
   {
      RECT rc = { 0, 0, 0, 0 };

      rc.left   = hb_parni( 2,1 );
      rc.top    = hb_parni( 2,2 );
      rc.right  = hb_parni( 2,3 );
      rc.bottom = hb_parni( 2,4 );

      hb_retl( InvalidateRect( ( HWND ) ( HB_PTRDIFF ) hb_parnint( 1 ), &rc, TRUE ) );
   }
}

//-------------------------------------------------------------------//
//
//  Win_LoadIcon( ncIcon )
//
HB_FUNC( WIN_LOADICON )
{
   HICON hIcon;

   if ( ISNUM( 1 ) )
   {
      hIcon = LoadIcon( ( HINSTANCE ) wvg_hInstance(), MAKEINTRESOURCE( hb_parni( 1 ) ) );
   }
   else
   {
      LPTSTR lpBuffer = HB_TCHAR_CONVTO( hb_parc( 1 ) );
      hIcon = ( HICON ) LoadImage( ( HINSTANCE ) NULL, lpBuffer, IMAGE_ICON, 0, 0, LR_LOADFROMFILE );
      HB_TCHAR_FREE( lpBuffer );
   }

   hb_retnint( ( HB_PTRDIFF ) hIcon );
}

//-------------------------------------------------------------------//
//
//  Win_LoadImage( ncImage, nSource ) -> hImage
//    nSource == 0 ResourceIdByNumber
//    nSource == 1 ResourceIdByName
//    nSource == 2 ImageFromDiskFile
//
HB_FUNC( WIN_LOADIMAGE )
{
   HBITMAP hImage = 0;
   LPTSTR  lpBuffer = HB_TCHAR_CONVTO( hb_parc( 1 ) );
   int     iSource = hb_parni( 2 );

   switch ( iSource )
   {
      case 0:
         hImage = LoadBitmap( ( HINSTANCE ) wvg_hInstance(), MAKEINTRESOURCE( hb_parni( 1 ) ) );
         break;

      case 1:
         hImage = LoadBitmap( ( HINSTANCE ) wvg_hInstance(), lpBuffer );
         break;

      case 2:
         hImage = ( HBITMAP ) LoadImage( ( HINSTANCE ) NULL, lpBuffer, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE );
         break;
   }

   HB_TCHAR_FREE( lpBuffer );
   hb_retnint( ( HB_PTRDIFF ) hImage );
}

//-------------------------------------------------------------------//

HB_FUNC( WIN_GETCLIENTRECT )
{
   RECT     rc = { 0,0,0,0 };
   PHB_ITEM info = hb_itemArrayNew( 4 );

   GetClientRect( ( HWND ) ( HB_PTRDIFF ) hb_parnint( 1 ), &rc );

   hb_arraySetNI( info, 1, rc.left   );
   hb_arraySetNI( info, 2, rc.top    );
   hb_arraySetNI( info, 3, rc.right  );
   hb_arraySetNI( info, 4, rc.bottom );

   hb_itemReturnRelease( info );
}

//-------------------------------------------------------------------//
//
//    Win_DrawImage( hdc, nLeft, nTop, nWidth, nHeight, cImage ) in Pixels
//
HB_FUNC( WIN_DRAWIMAGE )
{
   hb_retl( hb_wvt_DrawImage( ( HDC ) ( HB_PTRDIFF ) hb_parnint( 1 ), hb_parni( 2 ), hb_parni( 3 ),
                                   hb_parni( 4 ), hb_parni( 5 ), hb_parc( 6 ) ) );
}

//-------------------------------------------------------------------//

HB_FUNC( WIN_GETDC )
{
   hb_retnint( ( HB_PTRDIFF ) GetDC( ISNIL( 1 ) ? NULL : ( HWND ) ( HB_PTRDIFF ) hb_parnint( 1 ) ) );
}

//-------------------------------------------------------------------//

HB_FUNC( WIN_RELEASEDC )
{
   hb_retl( ReleaseDC( ( HWND ) ( HB_PTRDIFF ) hb_parnint( 1 ), ( HDC ) ( HB_PTRDIFF ) hb_parnint( 2 ) ) );
}

//-------------------------------------------------------------------//
#if 0
HB_FUNC( WIN_RECTANGLE )
{
   Rectangle( ( HDC ) ( HB_PTRDIFF ) hb_parnint( 1 ), hb_parni( 2 ), hb_parni( 3 ), hb_parni( 4 ), hb_parni( 5 ) );
}
#endif
//-------------------------------------------------------------------//

HB_FUNC( WIN_CREATEBRUSH )
{
   LOGBRUSH lb = { 0,0,0 };

   lb.lbStyle = hb_parni( 1 );
   lb.lbColor = ISNIL( 2 ) ? RGB( 0, 0, 0 ) : ( COLORREF ) hb_parnl( 2 ) ;
   lb.lbHatch = ISNIL( 3 ) ? 0 : hb_parni( 3 );

   hb_retnint( ( HB_PTRDIFF ) CreateBrushIndirect( &lb ) );
}

//-------------------------------------------------------------------//
//
//   Win_DrawText( hDC, cText, aRect, nFormat )
//
HB_FUNC( WIN_DRAWTEXT )
{
   RECT rc = { 0,0,0,0 };
   LPTSTR lpBuffer = HB_TCHAR_CONVTO( hb_parc( 2 ) );

   rc.left   = hb_parni( 3,1 );
   rc.top    = hb_parni( 3,2 );
   rc.right  = hb_parni( 3,3 );
   rc.bottom = hb_parni( 3,4 );

   hb_retl( DrawText( ( HDC ) ( HB_PTRDIFF ) hb_parnint( 1 ), lpBuffer, lstrlen( lpBuffer ), &rc, hb_parni( 4 ) ) );
   HB_TCHAR_FREE( lpBuffer );
}

//-------------------------------------------------------------------//

HB_FUNC( WIN_GETWINDOWRECT )
{
   RECT rc;
   PHB_ITEM info = hb_itemArrayNew( 4 );

   GetWindowRect( ( HWND ) ( HB_PTRDIFF ) hb_parnint( 1 ), &rc );

   hb_arraySetNI( info, 1, rc.left   );
   hb_arraySetNI( info, 2, rc.top    );
   hb_arraySetNI( info, 3, rc.right  );
   hb_arraySetNI( info, 4, rc.bottom );

   hb_itemReturnRelease( info );
}

//-------------------------------------------------------------------//
// Win_MoveWindow( hWnd, nLeft, nTop, nWidth, nHeight, lRePaint )
//
HB_FUNC( WIN_MOVEWINDOW )
{
   MoveWindow( (HWND) ( HB_PTRDIFF ) hb_parnint( 1 ), hb_parnl( 2 ), hb_parnl( 3 ), hb_parnl( 4 ), hb_parnl( 5 ), hb_parl( 6 ) );
}

//-------------------------------------------------------------------//

HB_FUNC( WIN_GETDESKTOPWINDOW )
{
   wvg_rethandle( GetDesktopWindow() );
}

//----------------------------------------------------------------------//

HB_FUNC( WIN_SETPARENT )
{
   hb_retnint( ( HB_PTRDIFF ) SetParent( (HWND) ( HB_PTRDIFF ) hb_parnint( 1 ), (HWND) ( HB_PTRDIFF ) hb_parnint( 2 ) ) );
}

//-------------------------------------------------------------------//

HB_FUNC( WIN_BRINGWINDOWTOTOP )
{
   hb_retl( BringWindowToTop( wvg_parhwnd( 1 ) ) );
}

//----------------------------------------------------------------------//

HB_FUNC( WIN_SETFOREGROUNDWINDOW )
{
   hb_retl( BringWindowToTop( wvg_parhwnd( 1 ) ) );
}

//----------------------------------------------------------------------//

HB_FUNC( WIN_SETWINDOWTEXT )
{
   LPTSTR text = HB_TCHAR_CONVTO( hb_parc( 2 ) );
   SetWindowText( wvg_parhwnd( 1 ), text );
   HB_TCHAR_FREE( text );
}

//----------------------------------------------------------------------//

HB_FUNC( WIN_SETWINDOWLONG )
{
   hb_retnl( SetWindowLong( ( HWND ) ( HB_PTRDIFF ) hb_parnint( 1 ), hb_parni( 2 ), hb_parnl( 3 ) ) );
}

//-------------------------------------------------------------------//

HB_FUNC( WIN_ISWINDOW )
{
   hb_retl( IsWindow( (HWND) ( HB_PTRDIFF ) hb_parnint( 1 ) ) );
}

//-------------------------------------------------------------------//

HB_FUNC( WIN_ENABLEWINDOW )
{
   hb_retl( EnableWindow( wvg_parhwnd( 1 ), hb_parl( 2 ) ) );
}

//-------------------------------------------------------------------//

HB_FUNC( WIN_DESTROYWINDOW )
{
   hb_retl( DestroyWindow( (HWND) ( HB_PTRDIFF ) hb_parnint( 1 ) ) );
}

//-------------------------------------------------------------------//

HB_FUNC( WIN_CLIENTTOSCREEN )
{
   POINT    Point ;
   PHB_ITEM pArray = hb_param( 2 , HB_IT_ARRAY );

   if ( wvt_Array2Point( pArray ,&Point ) )
   {
      if ( ClientToScreen( (HWND) ( HB_PTRDIFF ) hb_parnint( 1 ), &Point ) )
      {
          wvt_Point2ArrayEx( &Point, pArray );
          hb_retl( TRUE ) ;
      }
      else
      {
         hb_retl( FALSE ) ;
      }
   }
   else
   {
      hb_retl( FALSE ) ;
   }
}

//----------------------------------------------------------------------//

HB_FUNC( WIN_SCREENTOCLIENT )
{
   POINT    Point ;
   PHB_ITEM pArray = hb_param( 2 , HB_IT_ARRAY );

   if ( wvt_Array2Point( pArray, &Point ) )
   {
      if( ScreenToClient( (HWND) ( HB_PTRDIFF ) hb_parnint( 1 ), &Point ) > 0 )
      {
          wvt_Point2ArrayEx( &Point, pArray );
          hb_retl( TRUE ) ;
      }
      else
      {
         hb_retl( FALSE ) ;
      }
   }
   else
   {
      hb_retl( FALSE ) ;
   }
}

//----------------------------------------------------------------------//

HB_FUNC( WIN_AND )
{
   hb_retnl( hb_parnl(1) & hb_parnl(2) ) ;
}

//----------------------------------------------------------------------//

HB_FUNC( WIN_OR )
{
   hb_retnl( hb_parnl(1) | hb_parnl(2) ) ;
}

//----------------------------------------------------------------------//

HB_FUNC( WIN_NOT )
{
   hb_retnl( ~( hb_parnl(1) ) ) ;
}

//----------------------------------------------------------------------//

HB_FUNC( WIN_TRACKPOPUPMENU )
{
   HMENU hMenu  = ( HMENU ) ( HB_PTRDIFF ) hb_parnint( 1 );
   UINT  uFlags = ISNIL( 2 ) ? TPM_CENTERALIGN | TPM_RETURNCMD : hb_parnl( 2 );
   int   x      = ISNIL( 3 ) ? 0 : hb_parni( 3 );
   int   y      = ISNIL( 4 ) ? 0 : hb_parni( 4 );
   HWND  hWnd   = ISNIL( 5 ) ? GetActiveWindow() : ( HWND ) ( HB_PTRDIFF ) hb_parnint( 5 );

   POINT xy = { 0,0 };

   if( ISNIL( 3 ) )
   {
      GetCursorPos( &xy );
   }
   else
   {
      xy.x = x;
      xy.y = y;
   }

   hb_retnl( TrackPopupMenu( hMenu, uFlags, xy.x, xy.y, 0, hWnd, NULL ) );
}

//-------------------------------------------------------------------//

HB_FUNC( WIN_CHOOSECOLOR )
{
   CHOOSECOLOR cc ;
   COLORREF    crCustClr[ 16 ] ;
   int         i ;

   for( i = 0 ; i < 16 ; i++ )
   {
     crCustClr[ i ] = ( ISARRAY( 2 ) ? ( COLORREF ) hb_parnl( 2, i+1 ) : GetSysColor( COLOR_BTNFACE ) ) ;
   }

   cc.lStructSize   = sizeof( CHOOSECOLOR ) ;
   cc.hwndOwner     = ISNIL( 4 ) ? NULL : (HWND) ( HB_PTRDIFF ) hb_parnint( 4 );
   cc.rgbResult     = ISNIL( 1 ) ?  0 : ( COLORREF ) hb_parnl( 1 ) ;
   cc.lpCustColors  = crCustClr ;
   cc.Flags         = ( WORD ) ( ISNIL( 3 ) ? CC_ANYCOLOR | CC_RGBINIT | CC_FULLOPEN : hb_parnl( 3 ) );

   if ( ChooseColor( &cc ) )
   {
      hb_retnl( cc.rgbResult ) ;
   }
   else
   {
      hb_retnl( -1 );
   }
}

//-------------------------------------------------------------------//

HB_FUNC( WIN_FINDWINDOW )
{
   HWND hwnd;
   LPTSTR lpStr;

   lpStr = HB_TCHAR_CONVTO( hb_parc( 1 ) );
   hwnd = FindWindow( NULL, lpStr );
   HB_TCHAR_FREE( lpStr );

   if ( hwnd )
   {
      hb_retnint( ( HB_PTRDIFF ) hwnd );
   }
   else
   {
      hb_retnint( -1 );
   }
}

//----------------------------------------------------------------------//

HB_FUNC( WIN_SLEEP )
{
   Sleep( hb_parni( 1 ) );
}

//----------------------------------------------------------------------//
//                         Menu Manipulations
//----------------------------------------------------------------------//

HB_FUNC( WIN_SETMENU )
{
   HWND hWnd = ( HWND ) ( HB_PTRDIFF ) hb_parnint( 1 );

   #if 1
   BOOL bSet;
   RECT wi = { 0, 0, 0, 0 };
   RECT ci = { 0, 0, 0, 0 };
   int height, width;

   bSet = SetMenu( hWnd, ( HMENU ) ( HB_PTRDIFF ) hb_parnint( 2 ) );

   GetWindowRect( hWnd, &wi );
   GetClientRect( hWnd, &ci );
   height = ( ci.bottom - ci.top );
   width  = ( ci.right - ci.left );

   width  += ( wi.right - wi.left - ci.right );
   height += ( wi.bottom - wi.top - ci.bottom );

   SetWindowPos( hWnd, NULL, wi.left, wi.top, width, height, SWP_NOZORDER );

   hb_retl( bSet );
   #endif

   #if 0
   hb_retl( SetMenu( hWnd, ( HMENU ) ( HB_PTRDIFF ) hb_parnint( 2 ) ) );
   #endif
}

//-------------------------------------------------------------------//

HB_FUNC( WIN_CREATEMENU )
{
  hb_retnint( ( HB_PTRDIFF ) CreateMenu() );
}

//-------------------------------------------------------------------//

HB_FUNC( WIN_CREATEPOPUPMENU )
{
  hb_retnint( ( HB_PTRDIFF ) CreatePopupMenu() );
}

//-------------------------------------------------------------------//

HB_FUNC( WIN_APPENDMENU )
{
   LPTSTR  buffer;
   int     i,iLen ;

   if ( ISCHAR( 4 ) )
   {
      iLen = hb_parclen( 4 );
      if ( iLen > 0 && iLen < 256 )   // Translate '~' to '&'
      {
         char * text = ( char * ) hb_xgrab( iLen + 1 );

         memcpy( text, hb_parc( 4 ), iLen + 1 );

         for ( i = 0; i < iLen; i++ )
         {
            if( text[ i ] == '~' )
               text[ i ] = '&';
         }

         buffer = HB_TCHAR_CONVTO( text );
         hb_retl( AppendMenu( ( HMENU ) ( HB_PTRDIFF ) hb_parnint( 1 ), ( UINT ) hb_parni( 2 ), ( HB_PTRDIFF ) hb_parnint( 3 ), buffer ) );
         HB_TCHAR_FREE( buffer );
         hb_xfree( text );
      }
      else
      {
         buffer = HB_TCHAR_CONVTO( hb_parc( 4 ) );
         hb_retl( AppendMenu( ( HMENU ) ( HB_PTRDIFF ) hb_parnint( 1 ), ( UINT ) hb_parni( 2 ), ( HB_PTRDIFF ) hb_parnint( 3 ), buffer ) );
         HB_TCHAR_FREE( buffer );
      }
   }
   else
   {  // It is a SEPARATOR or Submenu
      LPCTSTR lpszCaption = ( LPCTSTR ) ( HB_PTRDIFF ) hb_parnint( 4 );
      hb_retl( AppendMenu( ( HMENU ) ( HB_PTRDIFF ) hb_parnint( 1 ), ( UINT ) hb_parni( 2 ), ( HB_PTRDIFF ) hb_parnint( 3 ), ( LPCTSTR ) lpszCaption ) );
   }
}

//-------------------------------------------------------------------//

HB_FUNC( WIN_DELETEMENU )
{
  hb_retl( DeleteMenu( ( HMENU ) ( HB_PTRDIFF ) hb_parnint( 1 ), ( UINT ) hb_parni( 2 ), ( UINT ) hb_parni( 3 ) ) );
}

//-------------------------------------------------------------------//

HB_FUNC( WIN_DESTROYMENU )
{
  hb_retl( DestroyMenu( ( HMENU ) ( HB_PTRDIFF ) hb_parnint( 1 ) ) );
}

//-------------------------------------------------------------------//

HB_FUNC( WIN_ENABLEMENUITEM )
{
   hb_retl( EnableMenuItem( ( HMENU ) ( HB_PTRDIFF ) hb_parnint( 1 ), ( UINT ) hb_parni( 2 ), ( UINT ) hb_parni( 3 ) ) );
}

//-------------------------------------------------------------------//

HB_FUNC( WIN_CHECKMENUITEM )
{
   hb_retni( CheckMenuItem( ( HMENU ) ( HB_PTRDIFF ) hb_parnint( 1 ), ( UINT ) hb_parni( 2 ), ( UINT ) hb_parni( 3 ) ) );
}

//----------------------------------------------------------------------//

HB_FUNC( WIN_DRAWMENUBAR )
{
   DrawMenuBar( ( HWND ) ( HB_PTRDIFF ) hb_parnint( 1 ) ) ;
}

//-------------------------------------------------------------------//

HB_FUNC( WIN_UPDATEWINDOW )
{
   hb_retl( UpdateWindow( ( HWND ) ( HB_PTRDIFF ) hb_parnint( 1 ) ) );
}

//----------------------------------------------------------------------//

HB_FUNC( WIN_SHOWWINDOW )
{
   hb_retl( ShowWindow( ( HWND ) ( HB_PTRDIFF ) hb_parnint( 1 ), hb_parni( 2 ) ) );
}

//----------------------------------------------------------------------//

HB_FUNC( WIN_MAKELPARAM )
{
   hb_retnint( MAKELPARAM( hb_parnint( 1 ), hb_parnint( 2 ) ) );
}

//----------------------------------------------------------------------//

HB_FUNC( WIN_CREATEWINDOWEX )
{
   HWND hWnd;
   LPTSTR szWinName;
   LPTSTR szClassName;

   szClassName = HB_TCHAR_CONVTO( hb_parc( 2 ) );
   szWinName = HB_TCHAR_CONVTO( ISNIL( 3 ) ? "" : hb_parc( 3 ) );

   hWnd = CreateWindowEx( ( HB_PTRDIFF ) hb_parnint( 1 ),
                          szClassName,
                          szWinName,
                          ( HB_PTRDIFF ) hb_parnint( 4 ),
                          hb_parni( 5 ), hb_parni( 6 ),
                          hb_parni( 7 ), hb_parni( 8 ),
                          ( HWND ) ( HB_PTRDIFF ) hb_parnint( 9 ),
                          ISNIL( 10 ) ? NULL : ( HMENU ) ( HB_PTRDIFF ) hb_parnint( 10 ),
                          ISNIL( 11 ) ? wvg_hInstance() : ( HINSTANCE ) ( HB_PTRDIFF ) hb_parnint( 11 ),
                          NULL );

   HB_TCHAR_FREE( szClassName );
   HB_TCHAR_FREE( szWinName );

   hb_retnint( ( HB_PTRDIFF ) hWnd );
}
//----------------------------------------------------------------------//

HB_FUNC( WIN_CREATETOOLBAREX )
{
   HWND hWnd;

   hWnd = CreateToolbarEx( ( HWND ) ( HB_PTRDIFF ) hb_parnint( 1 ),
                           ( HB_PTRDIFF ) hb_parnint( 2 ),
                           hb_parni( 3 ),
                           hb_parni( 4 ),
                           ISNIL( 5 ) ? NULL : ( HINSTANCE ) ( HB_PTRDIFF ) hb_parnint( 5 ),
                           ISNIL( 6 ) ? 0 : hb_parnint( 6 ),
                           ISNIL( 7 ) ? NULL : ( HANDLE ) ( HB_PTRDIFF ) hb_parnint( 7 ),
                           hb_parni(  8 ),
                           hb_parni(  9 ),
                           hb_parni( 10 ),
                           hb_parni( 11 ),
                           hb_parni( 12 ),
                           sizeof( TBBUTTON ) );

   hb_retnint( ( HB_PTRDIFF ) hWnd );
}
//----------------------------------------------------------------------//
//
//              Bitmap Management Function . Coutesy GTWVW
//
//----------------------------------------------------------------------//
static BITMAPINFO * PackedDibLoad( PTSTR szFileName )
{
   BITMAPFILEHEADER bmfh ;
   BITMAPINFO     * pbmi ;
   BOOL             bSuccess ;
   DWORD            dwPackedDibSize, dwBytesRead ;
   HANDLE           hFile ;

   hFile = CreateFile( szFileName, GENERIC_READ, FILE_SHARE_READ, NULL,
                       OPEN_EXISTING, FILE_FLAG_SEQUENTIAL_SCAN, NULL ) ;

   if( hFile == INVALID_HANDLE_VALUE )
      return NULL ;

   bSuccess = ReadFile( hFile, &bmfh, sizeof (BITMAPFILEHEADER), &dwBytesRead, NULL ) ;

   if( !bSuccess || ( dwBytesRead != sizeof( BITMAPFILEHEADER ) )
                 || ( bmfh.bfType != * ( WORD * ) "BM" ) )
   {
      CloseHandle( hFile ) ;
      return NULL ;
   }

   dwPackedDibSize = bmfh.bfSize - sizeof( BITMAPFILEHEADER ) ;

   pbmi = ( BITMAPINFO * ) hb_xgrab( dwPackedDibSize ) ;

   bSuccess = ReadFile( hFile, pbmi, dwPackedDibSize, &dwBytesRead, NULL ) ;
   CloseHandle( hFile ) ;

   if ( !bSuccess || ( dwBytesRead != dwPackedDibSize ) )
   {
      hb_xfree( pbmi ) ;
      return NULL ;
   }

   return pbmi ;
}

static int PackedDibGetWidth( BITMAPINFO * pPackedDib )
{
   if( pPackedDib->bmiHeader.biSize == sizeof( BITMAPCOREHEADER ) )
      return ( ( PBITMAPCOREINFO ) pPackedDib )->bmciHeader.bcWidth ;
   else
      return pPackedDib->bmiHeader.biWidth ;
}

static int PackedDibGetHeight( BITMAPINFO * pPackedDib )
{
   if( pPackedDib->bmiHeader.biSize == sizeof( BITMAPCOREHEADER ) )
      return ( ( PBITMAPCOREINFO ) pPackedDib )->bmciHeader.bcHeight ;
   else
      return abs( pPackedDib->bmiHeader.biHeight ) ;
}

static int PackedDibGetBitCount( BITMAPINFO * pPackedDib )
{
   if( pPackedDib->bmiHeader.biSize == sizeof( BITMAPCOREHEADER ) )
      return ( ( PBITMAPCOREINFO ) pPackedDib )->bmciHeader.bcBitCount ;
   else
      return pPackedDib->bmiHeader.biBitCount ;
 }

static int PackedDibGetInfoHeaderSize( BITMAPINFO * pPackedDib )
{
   if( pPackedDib->bmiHeader.biSize == sizeof( BITMAPCOREHEADER ) )
      return ( ( PBITMAPCOREINFO ) pPackedDib )->bmciHeader.bcSize ;

   else if( pPackedDib->bmiHeader.biSize == sizeof( BITMAPINFOHEADER ) )
      return pPackedDib->bmiHeader.biSize +
                  ( pPackedDib->bmiHeader.biCompression == BI_BITFIELDS ? 12 : 0 ) ;

   else return pPackedDib->bmiHeader.biSize ;
}

static int PackedDibGetColorsUsed( BITMAPINFO * pPackedDib )
{
   if( pPackedDib->bmiHeader.biSize == sizeof( BITMAPCOREHEADER ) )
      return 0 ;
   else
      return pPackedDib->bmiHeader.biClrUsed ;
}

static int PackedDibGetNumColors( BITMAPINFO * pPackedDib )
{
   int iNumColors ;

   iNumColors = PackedDibGetColorsUsed( pPackedDib ) ;

   if( iNumColors == 0 && PackedDibGetBitCount( pPackedDib ) < 16 )
      iNumColors = 1 << PackedDibGetBitCount( pPackedDib ) ;

   return iNumColors ;
}

static int PackedDibGetColorTableSize( BITMAPINFO * pPackedDib )
{
   if( pPackedDib->bmiHeader.biSize == sizeof( BITMAPCOREHEADER ) )
      return PackedDibGetNumColors( pPackedDib ) * sizeof( RGBTRIPLE ) ;
   else
      return PackedDibGetNumColors( pPackedDib ) * sizeof( RGBQUAD ) ;
}

#if 0
static RGBQUAD * PackedDibGetColorTablePtr( BITMAPINFO * pPackedDib )
{
   if( PackedDibGetNumColors( pPackedDib ) == 0 )
      return 0 ;

   return ( RGBQUAD * ) ( ( ( BYTE * ) pPackedDib ) + PackedDibGetInfoHeaderSize( pPackedDib ) ) ;
}

static RGBQUAD * PackedDibGetColorTableEntry( BITMAPINFO * pPackedDib, int i )
{
   if( PackedDibGetNumColors( pPackedDib ) == 0 )
      return 0 ;

   if( pPackedDib->bmiHeader.biSize == sizeof( BITMAPCOREHEADER ) )
      return ( RGBQUAD * ) ( ( ( RGBTRIPLE * ) PackedDibGetColorTablePtr( pPackedDib ) ) + i ) ;
   else
      return PackedDibGetColorTablePtr( pPackedDib ) + i ;
}
#endif

static BYTE * PackedDibGetBitsPtr( BITMAPINFO * pPackedDib )
{
   return ( ( BYTE * ) pPackedDib ) + PackedDibGetInfoHeaderSize( pPackedDib ) +
                                      PackedDibGetColorTableSize( pPackedDib ) ;
}

static HBITMAP hPrepareBitmap( char * szBitmapX, UINT uiBitmap,
                               int iExpWidth, int iExpHeight,
                               BOOL bMap3Dcolors,
                               HWND hCtrl,
                               int  iMode )
{
   HBITMAP hBitmap = NULL;

   switch( iMode )
   {
   case 0:
   {
   if( szBitmapX )
   {
      int iWidth, iHeight;
      {
         BITMAPINFO * pPackedDib = NULL;
         HDC          hdc;
         TCHAR *      szBitmap;

         szBitmap = HB_TCHAR_CONVTO( szBitmapX );

         if( !bMap3Dcolors )
         {
            pPackedDib = PackedDibLoad( szBitmap );
         }

         if( pPackedDib || bMap3Dcolors )
         {
            hdc = GetDC( hCtrl );

            if( !bMap3Dcolors )
            {
               hBitmap = CreateDIBitmap( hdc,
                                         ( PBITMAPINFOHEADER ) pPackedDib,
                                         CBM_INIT,
                                         PackedDibGetBitsPtr( pPackedDib ),
                                         pPackedDib,
                                         DIB_RGB_COLORS ) ;
               if( hBitmap == NULL )
               {
                  return NULL;
               }

               iWidth = PackedDibGetWidth( pPackedDib );
               iHeight = PackedDibGetHeight( pPackedDib );
            }
            else
            {
               hBitmap = ( HBITMAP ) LoadImage( ( HINSTANCE ) NULL,
                                     szBitmap,
                                     IMAGE_BITMAP,
                                     iExpWidth,
                                     iExpHeight,
                                     LR_LOADFROMFILE | LR_LOADMAP3DCOLORS );
               if( hBitmap == NULL )
               {
                   return NULL;
               }
               iWidth = iExpWidth;
               iHeight = iExpHeight;
            }

            if( iExpWidth == 0 && iExpHeight == 0 )
            {
               iWidth = iExpWidth;
               iHeight = iExpHeight;
            }

            if( iExpWidth != iWidth || iExpHeight != iHeight )
            {
               HDC     hdcSource, hdcTarget;
               HBITMAP hBitmap2;
               BOOL    bResult;

               hdcSource = CreateCompatibleDC( hdc );
               SelectObject( hdcSource, hBitmap );

               hdcTarget = CreateCompatibleDC( hdc );
               hBitmap2 = CreateCompatibleBitmap( hdcSource, iExpWidth, iExpHeight );
               SelectObject( hdcTarget, hBitmap2 );

               bResult = StretchBlt(
                                     hdcTarget,      /* handle to destination DC                 */
                                     0,              /* x-coord of destination upper-left corner */
                                     0,              /* y-coord of destination upper-left corner */
                                     iExpWidth,      /* width of destination rectangle           */
                                     iExpHeight,     /* height of destination rectangle          */
                                     hdcSource,      /* handle to source DC                      */
                                     0,              /* x-coord of source upper-left corner      */
                                     0,              /* y-coord of source upper-left corner      */
                                     iWidth,         /* width of source rectangle                */
                                     iHeight,        /* height of source rectangle               */
                                     SRCCOPY         /* raster operation code                    */
                                   );

               if( !bResult )
               {
                  DeleteObject( hBitmap2 );
               }
               else
               {
                  DeleteObject( hBitmap );
                  hBitmap = hBitmap2;
               }

               DeleteDC( hdcSource );
               DeleteDC( hdcTarget );
            }

            HB_TCHAR_FREE( szBitmap );
            ReleaseDC( hCtrl, hdc ) ;
            if( pPackedDib )
            {
               hb_xfree( pPackedDib ) ;
            }
         }
      }
   }
   }
   break;
   case 1:
   {
      UINT uiOptions = bMap3Dcolors ? LR_LOADMAP3DCOLORS : LR_DEFAULTCOLOR;
      TCHAR *      szBitmap;

      szBitmap = HB_TCHAR_CONVTO( szBitmapX );

      hBitmap = ( HBITMAP ) LoadImage(
                                  wvg_hInstance(),
                                  ( LPCTSTR ) szBitmap,
                                  IMAGE_BITMAP,
                                  iExpWidth,
                                  iExpHeight,
                                  uiOptions );
      HB_TCHAR_FREE( szBitmap );
      if( hBitmap == NULL )
      {
         return NULL;
      }
   }
   break;
   case 2: /* loading from resourceid */
   {
      UINT uiOptions = bMap3Dcolors ? LR_LOADMAP3DCOLORS : LR_DEFAULTCOLOR;
      char szResname[ MAX_PATH + 1 ];

      hb_snprintf( szResname, sizeof( szResname ), "?%u", uiBitmap );

      hBitmap = ( HBITMAP ) LoadImage(
                                  wvg_hInstance(),
                                  ( LPCTSTR ) MAKEINTRESOURCE( ( WORD ) uiBitmap ),
                                  IMAGE_BITMAP,
                                  iExpWidth,
                                  iExpHeight,
                                  uiOptions );
      if( hBitmap == NULL )
      {
         return NULL;
      }
   }     /* loading from resources */
   break;
   }

   return hBitmap;
}
//----------------------------------------------------------------------//

HB_FUNC( WVG_PREPAREBITMAPFROMFILE )
{
   HBITMAP hBitmap;

   hBitmap = hPrepareBitmap( hb_parc( 1 ), 0, hb_parni( 2 ), hb_parni( 3 ), hb_parl( 4 ),
                             ( HWND ) ( HB_PTRDIFF ) hb_parnint( 5 ), 0 );

   hb_retnint( ( HB_PTRDIFF ) hBitmap );
}

//----------------------------------------------------------------------//

HB_FUNC( WVG_PREPAREBITMAPFROMRESOURCEID )
{
   HBITMAP hBitmap;

   hBitmap = hPrepareBitmap( ( char * ) NULL, hb_parni( 1 ), hb_parni( 2 ), hb_parni( 3 ), hb_parl( 4 ),
                             ( HWND ) ( HB_PTRDIFF ) hb_parnint( 5 ), 2 );

   hb_retnint( ( HB_PTRDIFF ) hBitmap );
}

//----------------------------------------------------------------------//

HB_FUNC( WVG_PREPAREBITMAPFROMRESOURCENAME )
{
   HBITMAP hBitmap;

   hBitmap = hPrepareBitmap( hb_parc( 1 ), 0, hb_parni( 2 ), hb_parni( 3 ), hb_parl( 4 ),
                             ( HWND ) ( HB_PTRDIFF ) hb_parnint( 5 ), 1 );

   hb_retnint( ( HB_PTRDIFF ) hBitmap );
}
//----------------------------------------------------------------------//

HB_FUNC( WVG_STATUSBARCREATEPANEL )
{
   HWND hWndSB = ( HWND ) ( HB_PTRDIFF ) hb_parnint( 1 );
   int  iMode = hb_parni( 2 );

   if ( hWndSB == NULL || !IsWindow( hWndSB ) )
   {
      hb_retl( FALSE );
      return;
   }

   switch( iMode )
   {
      case 0:
      {
         int    ptArray[ WIN_STATUSBAR_MAX_PARTS ];
         int    iParts;
         RECT   rc = { 0, 0, 0, 0 };
         int    n ;
         USHORT width;

         iParts = SendMessage( hWndSB, SB_GETPARTS, WIN_STATUSBAR_MAX_PARTS, ( LPARAM ) ( LPINT ) ptArray );

         GetClientRect( hWndSB, &rc );
         width = rc.right / ( iParts + 1 );
         for( n = 0; n < iParts; n++ )
         {
            ptArray[ n ] = ( width * ( n + 1 ) );
         }
         ptArray[ iParts ] = -1;

         if( SendMessage( hWndSB, SB_SETPARTS, iParts + 1, ( LPARAM ) ( LPINT ) ptArray ) )
         {
            hb_retl( TRUE );
            return;
         }
      }
      case -1:
      {
         RECT rc = { 0, 0, 0, 0 };
         int  ptArray[ WIN_STATUSBAR_MAX_PARTS ];

         if ( GetClientRect( hWndSB, &rc ) )
         {
            ptArray[ 0 ] = rc.right;

            SendMessage( hWndSB, SB_SETPARTS, 1, ( LPARAM ) ( LPINT ) ptArray );

            hb_retl( TRUE );
            return;
         }
      }
   }

   hb_retl( FALSE );
}

//----------------------------------------------------------------------//

HB_FUNC( WVG_STATUSBARSETTEXT )
{
   HWND   hWndSB = ( HWND ) ( HB_PTRDIFF ) hb_parnint( 1 );

   if( hWndSB && IsWindow( hWndSB ) )
   {
      int    iPart = ISNIL( 2 ) ? 1 : hb_parni( 2 );
      TCHAR  szText[ 1024 ];
      int    iFlags;
      TCHAR  *szCaption;

      iPart -= 1;           // Zero based

      iFlags = ( int ) HIWORD( SendMessage( hWndSB, SB_GETTEXT, ( WPARAM ) iPart, ( LPARAM ) szText ) );

      szCaption = HB_TCHAR_CONVTO( hb_parc( 3 ) );
      SendMessage( hWndSB, SB_SETTEXT, ( WPARAM ) iPart | iFlags, ( LPARAM ) szCaption );
      HB_TCHAR_FREE( szCaption );
   }
}

//----------------------------------------------------------------------//

HB_FUNC( WVG_STATUSBARREFRESH )
{
   #if 0
   HWND hWndSB = ( HWND ) ( HB_PTRDIFF ) hb_parnint( 1 );

   if( hWndSB && IsWindow( hWndSB ) )
   {
      int    ptArray[ WIN_STATUSBAR_MAX_PARTS ];
      int    iParts,i;

      iParts = SendMessage( hWndSB, SB_GETPARTS, WIN_STATUSBAR_MAX_PARTS, ( LPARAM ) ( LPINT ) ptArray );

      ptArray[ iParts-1 ] = -1;

      if( SendMessage( hWndSB, SB_SETPARTS, iParts, ( LPARAM ) ( LPINT ) ptArray ) )
      {
         hb_retl( TRUE );
         return;
      }
   }
   hb_retl( FALSE );
   #endif
}

//----------------------------------------------------------------------//

HB_FUNC( WIN_SENDMESSAGETEXT )
{
   LPTSTR lpBuffer = HB_TCHAR_CONVTO( hb_parc( 4 ) );

   SendMessage( ( HWND ) ( HB_PTRDIFF ) hb_parnint( 1 ), hb_parni( 2 ),
                                               ( WPARAM ) hb_parni( 3 ), ( LPARAM ) lpBuffer );
   HB_TCHAR_FREE( lpBuffer );
}

//----------------------------------------------------------------------//

HB_FUNC( WIN_GETMESSAGETEXT )
{
   TCHAR cText[ 32000 ];

   SendMessage( wvg_parhwnd( 1 ), ( UINT ) hb_parni( 2 ), wvg_parwparam( 3 ), ( LPARAM ) &cText );

   {
      char * szText = HB_TCHAR_CONVFROM( cText );
      hb_retc( szText );
      HB_TCHAR_FREE( szText );
   }
}

//-------------------------------------------------------------------//

HB_FUNC( WIN_SETWNDPROC )
{
   HWND    hWnd = ( HWND ) ( HB_PTRDIFF ) hb_parnint( 1 );
   WNDPROC wndProc = ( WNDPROC ) ( HB_PTRDIFF ) hb_parnint( 2 );
   WNDPROC oldProc;

   oldProc = ( WNDPROC ) SetWindowLong( hWnd, GWL_WNDPROC, ( long ) wndProc ) ;

   hb_retnint( ( HB_PTRDIFF ) oldProc );
   //wvg_rethandle( oldProc );
}

//----------------------------------------------------------------------//

HB_FUNC( WIN_DEFWINDOWPROC )
{
   hb_retnint( DefWindowProc( wvg_parhwnd( 1 ),
                              hb_parni( 2 ),
                              wvg_parwparam( 3 ),
                              wvg_parlparam( 4 ) ) );
}

//----------------------------------------------------------------------//

HB_FUNC( WIN_CALLWINDOWPROC )
{
   hb_retnint( CallWindowProc( wvg_parwndproc( 1 ),
                               wvg_parhwnd( 2 ),
                               hb_parnint( 3 ),
                               wvg_parwparam( 4 ),
                               wvg_parlparam( 5 ) ) );
}

//----------------------------------------------------------------------//
// Wvg_GetNMHInfo( nlParam )
//
HB_FUNC( WVG_GETNMHDRINFO )
{
   LPNMHDR  lpnmh     = ( LPNMHDR ) wvg_parlparam( 1 );
   PHB_ITEM pEvParams = hb_itemNew( NULL );

   hb_arrayNew( pEvParams, 3 );

   hb_arraySetNI( pEvParams, 1, lpnmh->code );
   hb_arraySetNInt( pEvParams, 2, ( HB_PTRDIFF ) lpnmh->idFrom   );
   hb_arraySetNInt( pEvParams, 3, ( HB_PTRDIFF ) lpnmh->hwndFrom );

   hb_itemReturnRelease( pEvParams );
}
//----------------------------------------------------------------------//
// Wvg_GetNMMouseInfo( nlParam )
//
HB_FUNC( WVG_GETNMMOUSEINFO )
{
   LPNMMOUSE nmm       = ( LPNMMOUSE ) wvg_parlparam( 1 );
   NMHDR     nmh       = nmm->hdr;
   PHB_ITEM  pEvParams = hb_itemNew( NULL );

   hb_arrayNew( pEvParams, 4 );

   hb_arraySetNI( pEvParams  , 1, nmh.code );
   hb_arraySetNInt( pEvParams, 2, ( HB_PTRDIFF ) nmh.idFrom   );
   hb_arraySetNInt( pEvParams, 3, ( HB_PTRDIFF ) nmh.hwndFrom );
   hb_arraySetNL( pEvParams  , 4, nmm->dwItemSpec );

   hb_itemReturnRelease( pEvParams );
}
/*----------------------------------------------------------------------*/
// Wvg_SetToolbarButtonTip( nlParam, cToolTip )
//
HB_FUNC( WVG_SETTOOLBARBUTTONTIP )
{
   LPNMTBGETINFOTIP lptbgit = ( LPNMTBGETINFOTIP ) wapi_par_LPARAM( 1 );
   LPTSTR pszText = HB_TCHAR_CONVTO( hb_parc( 2 ) );

   lptbgit->cchTextMax = strlen( hb_parc( 2 ) );
   //memcpy( lptbgit->pszText, pszText, strlen( hb_parc( 2 ) ) );
   lptbgit->pszText = pszText;
//hb_ToOutDebug( hb_parc( 2 ) );
//   HB_TCHAR_FREE( pszText );
}
//----------------------------------------------------------------------//
// Wvg_GetNMTreeViewInfo( nlParam )
//
HB_FUNC( WVG_GETNMTREEVIEWINFO )
{
   LPNMTREEVIEW pnmtv  = ( LPNMTREEVIEW ) wvg_parlparam( 1 );
   NMHDR        nmh    = pnmtv->hdr;

   PHB_ITEM  pEvParams = hb_itemNew( NULL );

   hb_arrayNew( pEvParams, 4 );

   hb_arraySetNI( pEvParams  , 1, nmh.code );
   hb_arraySetNInt( pEvParams, 2, ( HB_PTRDIFF ) nmh.idFrom   );
   hb_arraySetNInt( pEvParams, 3, ( HB_PTRDIFF ) nmh.hwndFrom );
   hb_arraySetNI( pEvParams  , 4, pnmtv->action );

   hb_itemReturnRelease( pEvParams );
}
//----------------------------------------------------------------------//
//                         TreeView Functions
//----------------------------------------------------------------------//

HB_FUNC( WIN_TREEVIEW_SETTEXTCOLOR )
{
   hb_retl( TreeView_SetTextColor( wvg_parhwnd( 1 ), wvg_parcolor( 2 ) ) );
}

//----------------------------------------------------------------------//

HB_FUNC( WIN_TREEVIEW_SETBKCOLOR )
{
   hb_retl( TreeView_SetBkColor( wvg_parhwnd( 1 ), wvg_parcolor( 2 ) ) );
}

//----------------------------------------------------------------------//

HB_FUNC( WIN_TREEVIEW_SETLINECOLOR )
{
   //hb_retl( TreeView_SetLineColor( wvg_parhwnd( 1 ), wvg_parcolor( 2 ) ) );
}

//----------------------------------------------------------------------//

HB_FUNC( WIN_TREEVIEW_SELECTITEM )
{
   hb_retl( TreeView_SelectItem( wvg_parhwnd( 1 ), wvg_parhandle( 2 ) ) );
}

//----------------------------------------------------------------------//
//  Wvg_TreeView_GetSelectionInfo( ::hWnd, nlParam, @cParent, @cText, @hParentOfSelected, @hItemSelected )
//
HB_FUNC( WVG_TREEVIEW_GETSELECTIONINFO )
{
   LPNMTREEVIEW pnmtv     = ( LPNMTREEVIEW ) wvg_parlparam( 2 );
   HTREEITEM    hSelected = pnmtv->itemNew.hItem;

   if( hSelected != NULL )
   {
      TCHAR     text[ MAX_PATH + 1 ];
      TCHAR     Parent[ MAX_PATH + 1 ];
      TV_ITEM   item;
      HTREEITEM hParent;

      hb_stornl( ( long ) hSelected, 6 );

      item.mask        = TVIF_HANDLE | TVIF_TEXT | TVIF_IMAGE;
      item.hItem       = hSelected;
      item.pszText     = text;
      item.cchTextMax  = MAX_PATH;

      if( TreeView_GetItem( wvg_parhwnd( 1 ), &item ) )
      {
         char * szText = HB_TCHAR_CONVFROM( text );
         hb_storclen( szText, strlen( szText ), 4 );
         HB_TCHAR_FREE( szText );
      }

      hParent = TreeView_GetParent( wvg_parhwnd( 1 ), hSelected );
      hb_stornl( ( long ) hParent, 5 );

      item.mask        = TVIF_HANDLE | TVIF_TEXT;
      item.hItem       = hParent;
      item.pszText     = Parent;
      item.cchTextMax  = MAX_PATH;

      if( TreeView_GetItem( wvg_parhwnd( 1 ), &item ) )
      {
         char * szText = HB_TCHAR_CONVFROM( Parent );
         hb_storclen( szText, strlen( szText ), 3 );
         HB_TCHAR_FREE( szText );
      }
   }
}

//----------------------------------------------------------------------//
//
//   hItem := Wvg_TreeView_AddItem( oItem:hTree, hParent, oItem:Caption )
//
HB_FUNC( WVG_TREEVIEW_ADDITEM )
{
   #ifdef UNICODE
   typedef struct tagTVINSERTSTRUCTW
   {
     HTREEITEM hParent;
     HTREEITEM hInsertAfter;
     TV_ITEMW  item;
   } TVINSERTSTRUCTW, FAR *LPTVINSERTSTRUCTW;
   #else
   typedef struct tagTVINSERTSTRUCTA
   {
     HTREEITEM hParent;
     HTREEITEM hInsertAfter;
     TV_ITEMA  item;
   } TVINSERTSTRUCTA, FAR *LPTVINSERTSTRUCTA;
   #endif

   TVINSERTSTRUCT tvis;
   LPTSTR text = HB_TCHAR_CONVTO( hb_parc( 3 ) );

   tvis.hInsertAfter    = TVI_LAST;
   tvis.item.mask       = TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_STATE;
   tvis.item.cchTextMax = MAX_PATH + 1;
   tvis.item.stateMask  = TVIS_BOLD | TVIS_CUT | TVIS_DROPHILITED |
                          TVIS_EXPANDEDONCE | TVIS_SELECTED | TVIS_EXPANDPARTIAL |
                          TVIS_OVERLAYMASK | TVIS_STATEIMAGEMASK | TVIS_USERMASK ;

   tvis.item.state      = 0;        // TVI_BOLD
   tvis.hParent         = ISNIL( 2 ) ? NULL : wvg_parhandle( 2 );
   tvis.item.pszText    = text;

   hb_retnint( ( long ) TreeView_InsertItem( wvg_parhwnd( 1 ), &tvis ) );

   HB_TCHAR_FREE( text );
}

//----------------------------------------------------------------------//

HB_FUNC( WIN_TREEVIEW_EXPAND )
{
   hb_retl( TreeView_Expand( wvg_parhwnd( 1 ), wvg_parhandle( 2 ), ( hb_parl( 3 ) ? TVE_EXPAND : TVE_COLLAPSE ) ) );
}

//----------------------------------------------------------------------//

HB_FUNC( WIN_TVIS_EXPANDED )
{
   //hb_retl( TreeView_GetItemState( wvg_parhwnd( 1 ), wvg_parhandle( 2 ), ( UINT ) TVIS_EXPANDED ) );
}

//----------------------------------------------------------------------//

HB_FUNC( WVG_TREEVIEW_SHOWEXPANDED )
{
   HWND      hwnd    = wvg_parhwnd( 1 );
   HTREEITEM hroot, hitem, hitem1, hitem2, hitem3;
   int       iExpand = ( hb_parl( 2 ) ? TVE_EXPAND : TVE_COLLAPSE );
   int       iLevels = hb_parni( 3 ) <= 0 ? 5 : hb_parni( 3 );

   hroot = TreeView_GetRoot( hwnd );
   if( hroot )
   {
      ( void ) TreeView_Expand( hwnd, hroot, iExpand );
      if( iLevels >= 2 )
      {
         hitem = TreeView_GetNextItem( hwnd, hroot, TVGN_CHILD );
         while( hitem )
         {
            ( void ) TreeView_Expand( hwnd, hitem, iExpand );
            if( iLevels >= 3 )
            {
               hitem1 = TreeView_GetNextItem( hwnd, hitem, TVGN_CHILD );
               while( hitem1 )
               {
                  ( void ) TreeView_Expand( hwnd, hitem1, iExpand );
                  if( iLevels >= 4 )
                  {
                     hitem2 = TreeView_GetNextItem( hwnd, hitem1, TVGN_CHILD );
                     while( hitem2 )
                     {
                        ( void ) TreeView_Expand( hwnd, hitem2, iExpand );
                        if( iLevels >= 5 )
                        {
                           hitem3 = TreeView_GetNextItem( hwnd, hitem2, TVGN_CHILD );
                           while( hitem3 )
                           {
                              ( void ) TreeView_Expand( hwnd, hitem3, iExpand );
                              hitem3 = TreeView_GetNextItem( hwnd, hitem3, TVGN_NEXT );
                           }
                        }
                        hitem2 = TreeView_GetNextItem( hwnd, hitem2, TVGN_NEXT );
                     }
                  }
                  hitem1 = TreeView_GetNextItem( hwnd, hitem1, TVGN_NEXT );
               }
            }
            hitem = TreeView_GetNextItem( hwnd, hitem, TVGN_NEXT );
         }
      }
   }
}

//----------------------------------------------------------------------//
//                          ListBox Functions
//----------------------------------------------------------------------//

HB_FUNC( WIN_LBGETTEXT )
{
   TCHAR  text[ MAX_PATH + 1 ];
   char * szText;

   SendMessage( wvg_parhwnd( 1 ), LB_GETTEXT, wvg_parwparam( 2 ), ( LPARAM ) text  );

   szText = HB_TCHAR_CONVFROM( text );
   hb_retc( szText );
   HB_TCHAR_FREE( szText );
}

//----------------------------------------------------------------------//

HB_FUNC( WIN_LBGETCURSEL )
{
   hb_retni( ListBox_GetCurSel( wvg_parhwnd( 1 ) ) );
}

//----------------------------------------------------------------------//

HB_FUNC( WIN_LBSETCURSEL )
{
   hb_retni( ListBox_SetCurSel( wvg_parhwnd( 1 ), hb_parni( 2 ) ) );
}

//----------------------------------------------------------------------//
//                                Buttons
//----------------------------------------------------------------------//

HB_FUNC( WIN_BUTTON_GETCHECK )
{
   hb_retnl( Button_GetCheck( wvg_parhwnd( 1 ) ) );
}

//----------------------------------------------------------------------//

HB_FUNC( WIN_ISICONIC )
{
   hb_retl( IsIconic( wvg_parhwnd( 1 ) ) );
}

//----------------------------------------------------------------------//

HB_FUNC( WIN_ISZOOMED )
{
   hb_retl( IsZoomed( wvg_parhwnd( 1 ) ) );
}

//----------------------------------------------------------------------//
// Win_SetDCBrushColor( hDC, nRGB )
//
HB_FUNC( WIN_SETDCBRUSHCOLOR )
{
#if ( _WIN32_WINNT >= 0x0500 )
   wvg_rethandle( SetDCBrushColor( wvg_parhdc( 1 ), wvg_parcolor( 2 ) ) );
#else
   wvg_rethandle( NULL );
#endif
}

//----------------------------------------------------------------------//
// Win_SetDCPenColor( hDC, nRGB )
//
HB_FUNC( WIN_SETDCPENCOLOR )
{
#if ( _WIN32_WINNT >= 0x0500 )
   wvg_rethandle( SetDCPenColor( wvg_parhdc( 1 ), wvg_parcolor( 2 ) ) );
#else
   wvg_rethandle( NULL );
#endif
}

//----------------------------------------------------------------------//
// Win_GetCurrentObject( hDC, nObjType )
//
HB_FUNC( WIN_GETCURRENTOBJECT )
{
   wvg_rethandle( GetCurrentObject( wvg_parhdc( 1 ), hb_parni( 2 ) ) );
}

//----------------------------------------------------------------------//
// Win_GetCurrentBrush( hDC )
//
HB_FUNC( WIN_GETCURRENTBRUSH )
{
   wvg_rethandle( GetCurrentObject( wvg_parhdc( 1 ), OBJ_BRUSH ) );
}

//----------------------------------------------------------------------//
// Win_GetCurrentFornt( hDC )
//
HB_FUNC( WIN_GETCURRENTFONT )
{
   wvg_rethandle( GetCurrentObject( wvg_parhdc( 1 ), OBJ_FONT ) );
}

//----------------------------------------------------------------------//

HB_FUNC( WIN_SETWINDOWPOSTOBACK )
{
   hb_retl( SetWindowPos( wvg_parhwnd( 1 ), HWND_BOTTOM, 0, 0, 0, 0 ,
                  SWP_NOSIZE | SWP_NOMOVE | SWP_NOACTIVATE ) );
}

//----------------------------------------------------------------------//

HB_FUNC( WIN_SETWINDOWPOSTOTOP )
{
   hb_retl( SetWindowPos( wvg_parhwnd( 1 ), HWND_BOTTOM, 0, 0, 0, 0 ,
                                  SWP_NOSIZE | SWP_NOMOVE  ) );
}

//----------------------------------------------------------------------//

HB_FUNC( WIN_SETWINDOWSIZE )
{
   hb_retl( SetWindowPos( wvg_parhwnd( 1 ), NULL, 0, 0, hb_parni( 2 ), hb_parni( 3 ),
                 hb_parl( 4 ) ? 0 : SWP_NOREDRAW | SWP_NOZORDER | SWP_NOMOVE | SWP_NOACTIVATE  ) );
}

//----------------------------------------------------------------------//

HB_FUNC( WIN_SETWINDOWPOSITION )
{
   hb_retl( SetWindowPos( wvg_parhwnd( 1 ), NULL, hb_parni( 2 ), hb_parni( 3 ), 0, 0,
                hb_parl( 4 ) ? 0 : SWP_NOREDRAW | SWP_NOZORDER | SWP_NOSIZE | SWP_NOACTIVATE ) );
}

//----------------------------------------------------------------------//

HB_FUNC( WIN_SETWINDOWPOSANDSIZE )
{
   hb_retl( SetWindowPos( wvg_parhwnd( 1 ), NULL, hb_parni( 2 ), hb_parni( 3 ),
                                                  hb_parni( 4 ), hb_parni( 5 ),
                hb_parl( 6 ) ? 0 : SWP_NOREDRAW | SWP_NOZORDER | SWP_NOACTIVATE ) );
}

//----------------------------------------------------------------------//
//                            WvgFontDialog()
//----------------------------------------------------------------------//

PHB_ITEM wvg_logfontTOarray( LPLOGFONT lf, BOOL bEmpty )
{
   PHB_ITEM aFont = hb_itemNew( NULL );
   hb_arrayNew( aFont, 15 );

   if( bEmpty )
   {
      hb_arraySetC( aFont ,   1, "" );
      hb_arraySetNL( aFont,   2, 0  );
      hb_arraySetNL( aFont,   3, 0  );
      hb_arraySetNL( aFont,   4, 0  );
      hb_arraySetL(  aFont,   5, 0  );
      hb_arraySetL(  aFont,   6, 0  );
      hb_arraySetL(  aFont,   7, 0  );
      hb_arraySetNI( aFont,   8, 0  );
      hb_arraySetNI( aFont,   9, 0  );
      hb_arraySetNI( aFont,  10, 0  );
      hb_arraySetNI( aFont,  11, 0  );
      hb_arraySetNI( aFont,  12, 0  );
      hb_arraySetNI( aFont,  13, 0  );
      hb_arraySetNI( aFont,  14, 0  );
      hb_arraySetNInt( aFont,15, 0  );
   }
   else
   {
      char *szFaceName = HB_TCHAR_CONVFROM( lf->lfFaceName );

      hb_arraySetC(  aFont,  1, szFaceName           );
      hb_arraySetNL( aFont,  2, lf->lfHeight         );
      hb_arraySetNL( aFont,  3, lf->lfWidth          );
      hb_arraySetNL( aFont,  4, lf->lfWeight         );
      hb_arraySetL(  aFont,  5, lf->lfItalic         );
      hb_arraySetL(  aFont,  6, lf->lfUnderline      );
      hb_arraySetL(  aFont,  7, lf->lfStrikeOut      );
      hb_arraySetNI( aFont,  8, lf->lfCharSet        );
      hb_arraySetNI( aFont,  9, lf->lfEscapement     );
      hb_arraySetNI( aFont, 10, lf->lfOrientation    );
      hb_arraySetNI( aFont, 11, lf->lfOutPrecision   );
      hb_arraySetNI( aFont, 12, lf->lfClipPrecision  );
      hb_arraySetNI( aFont, 13, lf->lfQuality        );
      hb_arraySetNI( aFont, 14, lf->lfPitchAndFamily );

      HB_TCHAR_FREE( szFaceName );
   }

   return( aFont );
}

//----------------------------------------------------------------------//
// Wvg_ChooseFont( hWnd, nWndProc, familyName, nominalPointSize, ;
//                 viewScreenFonts, viewPrinterFonts )
//
HB_FUNC( WVG_CHOOSEFONT )
{
   CHOOSEFONT  cf;  // = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };
   LOGFONT     lf;  // = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0 };
   DWORD       Flags;
   LONG        PointSize = 0;
   HWND        hWnd  = wvg_parhwnd( 1 );
   TCHAR       szStyle[ MAX_PATH + 1 ];

   if ( ISCHAR( 3 ) )
   {
      HB_TCHAR_CPTO( lf.lfFaceName, hb_parc( 3 ), sizeof( lf.lfFaceName ) - 1 );
   }
   if ( ( ! ISNIL( 4 ) ) && hb_parnl( 4 ) )
   {
      HDC hdc = GetDC( hWnd );
      PointSize = -MulDiv( ( LONG ) hb_parnl( 4 ), GetDeviceCaps( hdc, LOGPIXELSY ), 72 ) ;
      ReleaseDC( hWnd, hdc );
   }
   lf.lfHeight         = PointSize;
   lf.lfWidth          = 0;
   lf.lfWeight         = 0;
   lf.lfItalic         = 0;
   lf.lfUnderline      = 0;
   lf.lfStrikeOut      = 0;
   lf.lfCharSet        = DEFAULT_CHARSET;
   lf.lfQuality        = DEFAULT_QUALITY;
   lf.lfPitchAndFamily = FF_DONTCARE;

   Flags = CF_EFFECTS | CF_SHOWHELP | CF_APPLY | CF_INITTOLOGFONTSTRUCT | CF_ENABLEHOOK;

   #if 0
   Flags = Flags | CF_TTONLY;
   Flags = Flags | CF_FIXEDPITCHONLY;
   Flags = Flags | CF_SCALABLEONLY;
   Flags = Flags | CF_NOVECTORFONTS;
   Flags = Flags | CF_NOSCRIPTSEL;
   Flags = Flags | CF_NOSIMULATIONS;        // ::synthesizeFonts  == .f.
   #endif

   if( ISLOG( 5 ) &&  hb_parl( 5 ) )
      Flags = Flags | CF_SCREENFONTS;
   if( ISLOG( 6 ) &&  hb_parl( 6 ) )
      Flags = Flags | CF_PRINTERFONTS;

   cf.lStructSize      = sizeof( CHOOSEFONT );
   cf.hwndOwner        = hWnd;
   cf.hDC              = ( HDC ) NULL;      // only when ::oPrinterPS is defined
   cf.lpLogFont        = &lf;
   cf.iPointSize       = PointSize;
   cf.Flags            = Flags;
   cf.rgbColors        = RGB( 0,0,0 );
   cf.lCustData        = 0L;
   cf.lpfnHook         = ( LPCFHOOKPROC ) wvg_parwndproc( 2 );
   cf.lpTemplateName   = ( LPTSTR ) NULL;
   cf.hInstance        = ( HINSTANCE ) NULL;
   cf.lpszStyle        = ( LPTSTR ) szStyle;
   cf.nFontType        = SCREEN_FONTTYPE;   // ??
   cf.nSizeMin         = 0;
   cf.nSizeMax         = 0;

   if ( ChooseFont( &cf ) )
   {
      PHB_ITEM aFont = wvg_logfontTOarray( &lf, FALSE );
      PHB_ITEM aInfo = hb_itemNew( NULL );

      hb_arrayNew( aInfo, 4 );
      hb_arraySetNI( aInfo, 1, cf.iPointSize );
      hb_arraySetNInt( aInfo, 2, cf.rgbColors  );
      hb_arraySetNI( aInfo, 3, cf.nFontType  );
      {
         char * szText = HB_TCHAR_CONVFROM( cf.lpszStyle );
         hb_arraySetC( aInfo, 4, szText );
         HB_TCHAR_FREE( szText );
      }
      hb_arraySet( aFont, 15, aInfo );

      hb_itemReturnRelease( aFont );
      hb_itemRelease( aInfo );
   }
}

//----------------------------------------------------------------------//

HB_FUNC( WVG_CHOOSEFONT_GETLOGFONT )
{
   LOGFONT  lf;
   PHB_ITEM aFont;

   memset( &lf, 0, sizeof( LOGFONT ) );

   SendMessage( wvg_parhwnd( 1 ), WM_CHOOSEFONT_GETLOGFONT, ( WPARAM ) 0, ( LPARAM ) &lf );

   aFont = wvg_logfontTOarray( &lf, FALSE );

   hb_itemReturnRelease( aFont );
}

//----------------------------------------------------------------------//

HB_FUNC( WVG_FONTCREATE )
{
   LOGFONT lf;
   HFONT   hFont;

   memset( &lf, 0, sizeof( LOGFONT ) );

   HB_TCHAR_CPTO( lf.lfFaceName,  hb_parc( 1, 1 ), sizeof( lf.lfFaceName ) - 1 );
   lf.lfHeight         = ( LONG ) hb_parnl( 1, 2 );
   lf.lfWidth          = ( LONG ) hb_parnl( 1, 3 );
   lf.lfWeight         = ( LONG ) hb_parnl( 1, 4 );
   lf.lfItalic         = ( BYTE ) hb_parl(  1, 5 );
   lf.lfUnderline      = ( BYTE ) hb_parl(  1, 6 );
   lf.lfStrikeOut      = ( BYTE ) hb_parl(  1, 7 );
   lf.lfCharSet        = ( BYTE ) hb_parni( 1, 8 );
   lf.lfEscapement     = ( BYTE ) hb_parni( 1, 9 );
   lf.lfOrientation    = ( BYTE ) hb_parni( 1,10 );
   lf.lfOutPrecision   = ( BYTE ) hb_parni( 1,11 );
   lf.lfClipPrecision  = ( BYTE ) hb_parni( 1,12 );
   lf.lfQuality        = ( BYTE ) hb_parni( 1,13 );
   lf.lfPitchAndFamily = ( BYTE ) hb_parni( 1,14 );

   hFont = CreateFontIndirect( &lf );

   if( hFont )
   {
      PHB_ITEM aFont = wvg_logfontTOarray( &lf, FALSE );
      hb_arraySetNInt( aFont, 15, ( HB_PTRDIFF ) hFont );
      hb_itemReturnRelease( aFont );
   }
   else
   {
      PHB_ITEM aFont = wvg_logfontTOarray( &lf, TRUE );
      hb_itemReturnRelease( aFont );
   }
}

//----------------------------------------------------------------------//
// Wvg_PointSizeToHeight( hdc, nPointSize )
//
HB_FUNC( WVG_POINTSIZETOHEIGHT )
{
   HDC hdc = ISNIL( 1 ) ? GetDC( GetDesktopWindow() ) : wvg_parhdc( 1 );

   hb_retnl( ( LONG ) -MulDiv( ( LONG ) hb_parnl( 2 ), GetDeviceCaps( hdc, LOGPIXELSY ), 72 ) ) ;

   if( ISNIL( 1 ) )
      ReleaseDC( GetDesktopWindow(), hdc );
}

//----------------------------------------------------------------------//
// Wvg_HeightToPointSize( hdc, nHeight )
//
HB_FUNC( WVG_HEIGHTTOPOINTSIZE )
{
   HDC hdc = ISNIL( 1 ) ? GetDC( GetDesktopWindow() ) : wvg_parhdc( 1 );

   hb_retnl( ( LONG ) -MulDiv( hb_parnl( 2 ), 72, GetDeviceCaps( hdc, LOGPIXELSY ) ) ) ;

   if( ISNIL( 1 ) )
      ReleaseDC( GetDesktopWindow(), hdc );
}

//----------------------------------------------------------------------//
HB_FUNC( WVG_SETCURRENTBRUSH )
{
   SetClassLong( wvg_parhwnd( 1 ), GCL_HBRBACKGROUND, ( DWORD ) hb_parnint( 2 ) );
}
//----------------------------------------------------------------------//
// Win_SetLayeredWindowAttributes( hWnd, nRGB, nOpacityFactor [0-255] )
//
HB_FUNC( WIN_SETLAYEREDWINDOWATTRIBUTES )
{
#if ( _WIN32_WINNT >= 0x0500 )
   HINSTANCE   h;
   wvtSetLayeredWindowAttributes pfnLayered;

   h = LoadLibraryEx( TEXT( "user32.dll" ), NULL, 0 );
   if( h )
   {
#if defined( UNICODE ) && defined( GetProcAddress )
      pfnLayered = ( wvtSetLayeredWindowAttributes ) GetProcAddressW( h, TEXT( "SetLayeredWindowAttributes" ) );
#else
      pfnLayered = ( wvtSetLayeredWindowAttributes ) GetProcAddress( h, "SetLayeredWindowAttributes" );
#endif
      if( pfnLayered )
      {
         HWND hWnd = wapi_par_HWND( 1 );
         COLORREF cr = ISNIL( 2 ) ? RGB( 255,255,255 ) : wapi_par_COLORREF( 2 );

         SetWindowLong( hWnd, GWL_EXSTYLE, GetWindowLong( hWnd, GWL_EXSTYLE ) | WS_EX_LAYERED );

         if( pfnLayered( hWnd, cr, hb_parni( 3 ),  /*LWA_COLORKEY |*/ LWA_ALPHA ) == 0 );
         {
//hb_ToOutDebug( "ysyysys" );
         }
      }
      FreeLibrary( h );
   }
#endif
}
//----------------------------------------------------------------------//
//                                IL  | DL
//  Wvg_AddToolbarButton( hWndTB, nBtn|hBitmap, cCaption, nButtonID, nMode, lIsTooltip )
//
HB_FUNC( WVG_ADDTOOLBARBUTTON )
{
   TBBUTTON    tbb;
   TBADDBITMAP tbab;
   BOOL        bSuccess;
   HWND        hWndTB = ( HWND ) ( HB_PTRDIFF ) hb_parnint( 1 );
   int         iCommand = hb_parni( 4 );
   TCHAR *     szCaption;

   switch( hb_parni( 5 ) )
   {
      case 1:  // button from image
      {
         int iNewBitmap, iNewString;

         // set bitmap
         //
         tbab.hInst = NULL;
         tbab.nID   = ( UINT ) wapi_par_HBITMAP( 2 );
         iNewBitmap = SendMessage( hWndTB, TB_ADDBITMAP, ( WPARAM ) 1, ( LPARAM ) &tbab );

         // set string
         //
         szCaption = HB_TCHAR_CONVTO( hb_parc( 3 ) );
         iNewString = SendMessage( hWndTB, TB_ADDSTRING, ( WPARAM ) 0, ( LPARAM ) szCaption );
         HB_TCHAR_FREE( szCaption );

         #if 1
         if( ISLOG( 6 ) && ( hb_parl( 6 ) ) )
         {
            SendMessage( hWndTB, TB_SETMAXTEXTROWS, ( WPARAM ) 0, ( LPARAM ) 0 );
         }
         #endif
         // add button
         //
         tbb.iBitmap   = iNewBitmap;
         tbb.idCommand = iCommand;
         tbb.fsState   = TBSTATE_ENABLED;
         tbb.fsStyle   = TBSTYLE_BUTTON | TBSTYLE_AUTOSIZE;
         tbb.dwData    = 0;
         tbb.iString   = iNewString;

         bSuccess = SendMessage( hWndTB, TB_ADDBUTTONS, ( WPARAM ) 1, ( LPARAM ) ( LPTBBUTTON ) &tbb );

         SendMessage( hWndTB, TB_SETPADDING, ( WPARAM ) 0, ( LPARAM ) MAKELPARAM(  10,10 ) );

         hb_retl( bSuccess );
         return;
      }

      case 2:  // system bitmap


      case 3:  // separator
      {
         tbb.iBitmap   = 0;  // Can be width of the separator
         tbb.idCommand = 0;
         tbb.fsState   = TBSTATE_ENABLED;
         tbb.fsStyle   = TBSTYLE_SEP;
         tbb.dwData    = 0;
         tbb.iString   = 0;

         bSuccess = SendMessage( hWndTB, TB_ADDBUTTONS, ( WPARAM ) 1, ( LPARAM ) ( LPTBBUTTON ) &tbb );
         hb_retl( bSuccess );
         return;
      }
   }
}
//----------------------------------------------------------------------//

HB_FUNC( WIN_SENDTOOLBARMESSAGE )
{
   HWND hTB = wapi_par_HWND( 1 );
   int  msg = wapi_par_INT( 2 );


   switch( msg )
   {
      case TB_ADDBITMAP            :
      {
         TBADDBITMAP tbab;
         int iBitmap;

         tbab.hInst = NULL;
         tbab.nID   = ( UINT ) wapi_par_HBITMAP( 3 );

         iBitmap = SendMessage( hTB, TB_ADDBITMAP, ( WPARAM ) 1, ( LPARAM ) &tbab );

         wapi_ret_NI( iBitmap );
      }
      break;
      case TB_ADDBUTTONS           :
      {
         TBBUTTON tbb;

         tbb.iBitmap   = wapi_par_INT( 3 );
         tbb.idCommand = wapi_par_INT( 4 );
         tbb.fsState   = TBSTATE_ENABLED;
         tbb.fsStyle   = TBSTYLE_BUTTON ;
         tbb.dwData    = 0;
         tbb.iString   = wapi_par_INT( 5 );

         wapi_ret_L( SendMessage( hTB, TB_ADDBUTTONS, ( WPARAM ) 1, ( LPARAM ) ( LPTBBUTTON ) &tbb ) );
      }
      break;
      case TB_ADDSTRING            :
      {
         int iString;
         LPTSTR szCaption;

         szCaption = HB_TCHAR_CONVTO( hb_parc( 3 ) );
         iString = SendMessage( hTB, TB_ADDSTRING, ( WPARAM ) NULL, ( LPARAM ) szCaption );
         HB_TCHAR_FREE( szCaption );

         wapi_ret_NI( iString );
      }
      break;
      case TB_AUTOSIZE             :
      {
         SendMessage( hTB, TB_AUTOSIZE, ( WPARAM ) 0, ( LPARAM ) 0 );
      }
      break;
      case TB_BUTTONCOUNT          :
      {
      }
      break;
      case TB_BUTTONSTRUCTSIZE     :
      {
         SendMessage( hTB, TB_BUTTONSTRUCTSIZE, sizeof( TBBUTTON ), 0 );
      }
      break;
      case TB_CHANGEBITMAP         :
      {
      }
      break;
      case TB_CHECKBUTTON          :
      {
      }
      break;
      case TB_COMMANDTOINDEX       :
      {
      }
      break;
      case TB_CUSTOMIZE            :
      {
      }
      break;
      case TB_DELETEBUTTON         :
      {
      }
      break;
      case TB_ENABLEBUTTON         :
      {
      }
      break;
      case TB_GETANCHORHIGHLIGHT   :
      {
      }
      break;
      case TB_GETBITMAP            :
      {
      }
      break;
      case TB_GETBITMAPFLAGS       :
      {
      }
      break;
      case TB_GETBUTTON            :
      {
      }
      break;
      case TB_GETBUTTONINFO        :
      {
      }
      break;
      case TB_GETBUTTONSIZE        :
      {
      }
      break;
      case TB_GETBUTTONTEXT        :
      {
      }
      break;
      case TB_GETCOLORSCHEME       :
      {
      }
      break;
      case TB_GETDISABLEDIMAGELIST :
      {
      }
      break;
      case TB_GETEXTENDEDSTYLE     :
      {
      }
      break;
      case TB_GETHOTIMAGELIST      :
      {
      }
      break;
      case TB_GETHOTITEM           :
      {
      }
      break;
      #if 0
      case TB_GETIDEALSIZE         :
      {
      }
      break;
      #endif
      case TB_GETIMAGELIST         :
      {
      }
      break;
      #if 0
      case TB_GETIMAGELISTCOUNT    :
      {
      }
      break;
      #endif
      case TB_GETINSERTMARK        :
      {
      }
      break;
      case TB_GETINSERTMARKCOLOR   :
      {
      }
      break;
      #if 0
      case TB_GETITEMDROPDOWNRECT  :
      {
      }
      break;
      #endif
      case TB_GETITEMRECT          :
      {
      }
      break;
      case TB_GETMAXSIZE           :
      {
      }
      break;
      #if 0
      case TB_GETMETRICS           :
      {
      }
      break;
      #endif
      case TB_GETOBJECT            :
      {
      }
      break;
      case TB_GETPADDING           :
      {
      }
      break;
      #if 0
      case TB_GETPRESSEDIMAGELIST  :
      {
      }
      break;
      #endif
      case TB_GETRECT              :
      {
      }
      break;
      case TB_GETROWS              :
      {
      }
      break;
      case TB_GETSTATE             :
      {
      }
      break;
      #if 0
      case TB_GETSTRING            :
      {
      }
      break;
      #endif
      case TB_GETSTYLE             :
      {
      }
      break;
      case TB_GETTEXTROWS          :
      {
      }
      break;
      case TB_GETTOOLTIPS          :
      {
      }
      break;
      case TB_GETUNICODEFORMAT     :
      {
      }
      break;
      case TB_HIDEBUTTON           :
      {
      }
      break;
      case TB_HITTEST              :
      {
      }
      break;
      case TB_INDETERMINATE        :
      {
      }
      break;
      case TB_INSERTBUTTON         :
      {
      }
      break;
      case TB_INSERTMARKHITTEST    :
      {
      }
      break;
      case TB_ISBUTTONCHECKED      :
      {
      }
      break;
      case TB_ISBUTTONENABLED      :
      {
      }
      break;
      case TB_ISBUTTONHIDDEN       :
      {
      }
      break;
      case TB_ISBUTTONHIGHLIGHTED  :
      {
      }
      break;
      case TB_ISBUTTONINDETERMINATE:
      {
      }
      break;
      case TB_ISBUTTONPRESSED      :
      {
      }
      break;
      case TB_LOADIMAGES           :
      {
      }
      break;
      case TB_MAPACCELERATOR       :
      {
      }
      break;
      case TB_MARKBUTTON           :
      {
         SendMessage( hTB, TB_MARKBUTTON, ( WPARAM ) wapi_par_INT( 3 ), ( LPARAM ) MAKELONG( hb_parl( 4 ),0 ) );
      }
      break;
      case TB_MOVEBUTTON           :
      {
      }
      break;
      case TB_PRESSBUTTON          :
      {
      }
      break;
      case TB_REPLACEBITMAP        :
      {
      }
      break;
      case TB_SAVERESTORE          :
      {
      }
      break;
      case TB_SETANCHORHIGHLIGHT   :
      {
      }
      break;
      case TB_SETBITMAPSIZE        :
      {
         SendMessage( hTB, TB_SETBITMAPSIZE, ( WPARAM ) 0,
                      ( LPARAM ) MAKELONG( wapi_par_INT( 3 ), wapi_par_INT( 4 ) ) );
      }
      break;
      case TB_SETBUTTONINFO        :
      {
      }
      break;
      case TB_SETBUTTONSIZE        :
      {
         SendMessage( hTB, TB_SETBUTTONSIZE, ( WPARAM ) 0,
                      ( LPARAM ) MAKELONG( wapi_par_INT( 3 ), wapi_par_INT( 4 ) ) );
      }
      break;
      case TB_SETBUTTONWIDTH       :
      {
         SendMessage( hTB, TB_SETBUTTONWIDTH, ( WPARAM ) 0,
                      ( LPARAM ) MAKELONG( wapi_par_INT( 3 ), wapi_par_INT( 4 ) ) );
      }
      break;
      case TB_SETCMDID             :
      {
      }
      break;
      case TB_SETCOLORSCHEME       :
      {
      }
      break;
      case TB_SETDISABLEDIMAGELIST :
      {
      }
      break;
      case TB_SETDRAWTEXTFLAGS     :
      {
      }
      break;
      case TB_SETEXTENDEDSTYLE     :
      {
      }
      break;
      case TB_SETHOTIMAGELIST      :
      {
      }
      break;
      case TB_SETHOTITEM           :
      {
      }
      break;
      #if 0
      case TB_SETHOTITEM2          :
      {
      }
      break;
      #endif
      case TB_SETIMAGELIST         :
      {
         SendMessage( hTB, TB_SETIMAGELIST, ( WPARAM ) 0, ( LPARAM ) wapi_par_HIMAGELIST( 3 ) );
      }
      break;
      case TB_SETINDENT            :
      {
         SendMessage( hTB, TB_SETINDENT, ( WPARAM ) wapi_par_INT( 3 ), ( LPARAM ) 0 );
      }
      break;
      case TB_SETINSERTMARK        :
      {
      }
      break;
      case TB_SETINSERTMARKCOLOR   :
      {
      }
      break;
      #if 0
      case TB_SETLISTGAP           :
      {
      }
      break;
      #endif
      case TB_SETMAXTEXTROWS       :
      {
         SendMessage( hTB, TB_SETMAXTEXTROWS, ( WPARAM ) wapi_par_INT( 2 ), ( LPARAM ) 0 );
      }
      break;
      #if 0
      case TB_SETMETRICS           :
      {
      }
      break;
      #endif
      case TB_SETPADDING           :
      {
         SendMessage( hTB, TB_SETPADDING, ( WPARAM ) 0,
                           ( LPARAM ) MAKELPARAM( wapi_par_INT( 2 ), wapi_par_INT( 3 ) ) );
      }
      break;
      case TB_SETPARENT            :
      {
      }
      break;
      #if 0
      case TB_SETPRESSEDIMAGELIST  :
      {
      }
      break;
      #endif
      case TB_SETROWS              :
      {
      }
      break;
      case TB_SETSTATE             :
      {
      }
      break;
      case TB_SETSTYLE             :
      {
      }
      break;
      case TB_SETTOOLTIPS          :
      {
      }
      break;
      case TB_SETUNICODEFORMAT     :
      {
      }
      break;
      #if 0
      case TB_SETWINDOWTHEME       :
      {
      }
      break;
      case TB_TRANSLATEACCELERATOR :
      {
      }
      break;
      #endif
   }

}

//----------------------------------------------------------------------//
// Wvg_RegisterClass( cClassName,
//
HB_FUNC( WVG_REGISTERCLASS_BYNAME )
{
   WNDCLASS wndclass;
   LPTSTR  szClass = HB_TCHAR_CONVTO( hb_parc( 1 ) );

   memset( &wndclass, 0, sizeof( WNDCLASS ) );
   wndclass.style         = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
   wndclass.lpfnWndProc   = DefWindowProc;
   wndclass.hInstance     = wvg_hInstance();
   wndclass.hIcon         = NULL;
   wndclass.hCursor       = LoadCursor( NULL, IDC_ARROW );
   wndclass.hbrBackground = NULL;
   wndclass.lpszMenuName  = NULL;
   wndclass.lpszClassName = szClass;

   if( ! RegisterClass( &wndclass ) )
   {
      int iError = GetLastError();
      if( iError != 1410 )
      {
         hb_errInternal( 10001, "Failed to register DA window class", NULL, NULL );
      }
   }
   HB_TCHAR_FREE( szClass );
}

//----------------------------------------------------------------------//
//  Function with Win_FillRect() exists in hbwin:win_parn1.c with different approach.
//                        **************************************
HB_FUNC( WVG_FILLRECT )
{
   RECT rc;

   rc.left   = hb_parni( 2,1 );
   rc.top    = hb_parni( 2,2 );
   rc.right  = hb_parni( 2,3 );
   rc.bottom = hb_parni( 2,4 );

   FillRect( wapi_par_HDC( 1 ), &rc, wapi_par_HBRUSH( 3 ) );
}

//----------------------------------------------------------------------//

HB_FUNC( WVG_BEGINMOUSETRACKING )
{
   TRACKMOUSEEVENT tmi;

   tmi.cbSize      = sizeof( TRACKMOUSEEVENT );
   tmi.dwFlags     = TME_LEAVE | TME_HOVER ;
   tmi.hwndTrack   = wapi_par_HWND( 1 );
   tmi.dwHoverTime = 1;
   wapi_ret_L( _TrackMouseEvent( &tmi ) );
}

/*----------------------------------------------------------------------*/

#include "hbapi.h"
#include "hbapiitm.h"
#include "hbapierr.h"
#include "hbstack.h"


#define HB_MEMSTRU_OSVERSIONINFO                1000001
#define HB_MEMSTRU_LOGFONT                      1000002

#define HB_ERR_MEMSTRU_NOT_MEM_BLOCK            4001
#define HB_ERR_MEMSTRU_WRONG_MEMSTRU_BLOCK      4002
#define HB_ERR_MEMSTRU_DESTROYED                4003

HB_EXTERN_BEGIN

extern PHB_ITEM hb_memstru_itemPut( PHB_ITEM pItem, void * pMemAddr, int iType );
extern void * hb_memstru_itemGet( PHB_ITEM pItem, int iType, BOOL fError );
extern void hb_memstru_ret( void * pMemAddr, int iType );
extern void * hb_memstru_param( int iParam, int iType, BOOL fError );

HB_EXTERN_END

/* end of header file */



typedef struct
{
   int      type;
   void *   mem;
}
HB_MEMSTRU_HOLDER, * PHB_MEMSTRU_HOLDER;

static HB_GARBAGE_FUNC( hb_memstru_destructor )
{
   PHB_MEMSTRU_HOLDER pStructHolder = ( PHB_MEMSTRU_HOLDER ) Cargo;

   if( pStructHolder->mem )
   {
      hb_xfree( pStructHolder->mem );
      pStructHolder->mem = NULL;
   }
}

PHB_ITEM hb_memstru_itemPut( PHB_ITEM pItem, void * pMemAddr, int iType )
{
   PHB_MEMSTRU_HOLDER pStructHolder;

   if( pItem )
   {
      if( HB_IS_COMPLEX( pItem ) )
         hb_itemClear( pItem );
   }
   else
      pItem = hb_itemNew( pItem );

   pStructHolder = ( PHB_MEMSTRU_HOLDER )
             hb_gcAlloc( sizeof( HB_MEMSTRU_HOLDER ), hb_memstru_destructor );
   pStructHolder->mem = pMemAddr;
   pStructHolder->type = iType;

   return hb_itemPutPtrGC( pItem, pStructHolder );
}

void * hb_memstru_itemGet( PHB_ITEM pItem, int iType, BOOL fError )
{
   PHB_MEMSTRU_HOLDER pStructHolder = ( PHB_MEMSTRU_HOLDER )
                               hb_itemGetPtrGC( pItem, hb_memstru_destructor );
   int iError;

   if( !pStructHolder )
      iError = HB_ERR_MEMSTRU_NOT_MEM_BLOCK;
   else if( pStructHolder->type != iType )
      iError = HB_ERR_MEMSTRU_WRONG_MEMSTRU_BLOCK;
   else if( !pStructHolder->mem )
      iError = HB_ERR_MEMSTRU_DESTROYED;
   else
      return pStructHolder->mem;

   if( fError )
      hb_errRT_BASE_SubstR( EG_ARG, iError, NULL,
                            HB_ERR_FUNCNAME, HB_ERR_ARGS_BASEPARAMS );

   return NULL;
}

void hb_memstru_ret( void * pMemAddr, int iType )
{
   hb_memstru_itemPut( hb_stackReturnItem(), pMemAddr, iType );
}

void * hb_memstru_param( int iParam, int iType, BOOL fError )
{
   return hb_memstru_itemGet( hb_param( iParam, HB_IT_POINTER ), iType, fError );
}

BOOL hb_memstru_store( void * pMemAddr, int iType, int iParam )
{
   PHB_ITEM pItem = hb_param( iParam, HB_IT_BYREF );
   if( !pItem )
      return FALSE;

   hb_memstru_itemPut( pItem, pMemAddr, iType );
   return TRUE;
}


/*
hb_memstru_*() functions can be used to pass structures between
C and .prg code.
*/

HB_FUNC( WAPI_STRUCT_OSVERSIONINFO )
{
   OSVERSIONINFO * osvi;

   osvi = ( OSVERSIONINFO * ) memset( hb_xgrab( sizeof( OSVERSIONINFO ) ),
                                      0, sizeof( OSVERSIONINFO ) );
   osvi->dwOSVersionInfoSize = sizeof( OSVERSIONINFO );

   hb_memstru_ret( osvi, HB_MEMSTRU_OSVERSIONINFO );
}

HB_FUNC( WAPI_GETVERSIONEX )
{
   OSVERSIONINFO * osvi;

   osvi = ( OSVERSIONINFO * )
          hb_memstru_param( 1, HB_MEMSTRU_OSVERSIONINFO, TRUE );

   if( osvi )
      hb_retl( GetVersionEx( osvi ) );
}


/*
You can also define some general functions to access given structure
members if you will find it useful in some cases.
*/
HB_FUNC( WAPI_MEMBER_OSVERSIONINFO )
{
   OSVERSIONINFO * osvi;
   const char * szMember;

   osvi = ( OSVERSIONINFO * )
          hb_memstru_param( 1, HB_MEMSTRU_OSVERSIONINFO, TRUE );
   szMember = hb_parc( 2 );
   if( osvi && szMember )
   {
      if( !strcmp( szMember, "dwMajorVersion" ) )
         hb_retnint( osvi->dwMajorVersion );
      else if( !strcmp( szMember, "dwMinorVersion" ) )
         hb_retnint( osvi->dwMinorVersion );
      else if( !strcmp( szMember, "dwBuildNumber" ) )
         hb_retnint( osvi->dwBuildNumber );
      else if( !strcmp( szMember, "dwPlatformId" ) )
         hb_retnint( osvi->dwPlatformId );
      else if( !strcmp( szMember, "szCSDVersion" ) )
         ;//wapi_rettstr( osvi->dwPlatformId );
      else
      {
         /* NIL for unknown members or RT error */
      }
   }
}
/*----------------------------------------------------------------------*/
