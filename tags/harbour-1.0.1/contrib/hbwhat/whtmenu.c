/*
 * $Id$
 */


// Menu functions


#define _WIN32_WINNT   0x0400

#include "hbwhat.h"

#include <windows.h>
#include <shlobj.h>
//#include <commctrl.h>

#include "hbapi.h"
#include "hbvm.h"
#include "hbstack.h"
#include "hbapiitm.h"


//-----------------------------------------------------------------------------

HB_FUNC( APPENDMENU )
{
  hb_retl( AppendMenu( (HMENU) HB_PARWH(1), hb_parni(2), hb_parni(3), hb_parcx(4)) );
}


//-----------------------------------------------------------------------------

HB_FUNC( CHECKMENUITEM )
{
  hb_retnl( CheckMenuItem((HMENU) HB_PARWH(1), hb_parni(2), hb_parni(3)) );
}

//-----------------------------------------------------------------------------

HB_FUNC( CREATEMENU )
{
  HB_RETWH( CreateMenu() );
}

//-----------------------------------------------------------------------------

HB_FUNC( CREATEPOPUPMENU )
{
  HB_RETWH( CreatePopupMenu() );
}

//-----------------------------------------------------------------------------

HB_FUNC( DELETEMENU )
{
  hb_retl( DeleteMenu( (HMENU) HB_PARWH(1), hb_parni(2), hb_parni(3) ) );
}

//-----------------------------------------------------------------------------

HB_FUNC( DESTROYMENU )

{
  hb_retl( DestroyMenu( (HMENU) HB_PARWH(1) ) );
}

//-----------------------------------------------------------------------------

HB_FUNC( GETMENU )

{
  HB_RETWH( GetMenu( (HWND) HB_PARWH(1) ) );
}

//-----------------------------------------------------------------------------

HB_FUNC( GETMENUITEMID )
{
   hb_retni( GetMenuItemID(
                            (HMENU) HB_PARWH( 1 ),  // handle to menu
                            (int) hb_parni( 2 )     // position of menu item
                          ) );
}

//-----------------------------------------------------------------------------

HB_FUNC( DRAWMENUBAR )
{
  hb_retl( DrawMenuBar( (HWND) HB_PARWH(1)));
}

//-----------------------------------------------------------------------------

HB_FUNC( ENABLEMENUITEM )
{
  hb_retl( EnableMenuItem( (HMENU) HB_PARWH(1), hb_parni(2), hb_parni(3) ) );
}

//-----------------------------------------------------------------------------

HB_FUNC( GETMENUSTATE )
{
  hb_retni( GetMenuState( (HMENU) HB_PARWH(1), hb_parni(2), hb_parni(3)) );
}

//-----------------------------------------------------------------------------

HB_FUNC( GETMENUSTRING )
{
   char cText[MAX_PATH+1] ={0};

   GetMenuString( (HMENU) HB_PARWH(1), hb_parni(2),(LPSTR) cText, MAX_PATH, hb_parni(3) );

   hb_retc( cText );

}

//-----------------------------------------------------------------------------

HB_FUNC( HILITEMENUITEM )
{
  hb_retl( HiliteMenuItem( (HWND) HB_PARWH(1), (HMENU) HB_PARWH(2), hb_parni(3), hb_parni(4)) );
}

//-----------------------------------------------------------------------------

HB_FUNC( INSERTMENU )
{
  hb_retl( InsertMenu( (HMENU) HB_PARWH(1), hb_parni(2), hb_parni(3), hb_parni(4), hb_parcx(5)) );
}

//-----------------------------------------------------------------------------

HB_FUNC( SETMENUITEMBITMAPS )
{
  hb_retl( SetMenuItemBitmaps( (HMENU) HB_PARWH(1), hb_parni(2), hb_parni(3), (HBITMAP) HB_PARWH(4), (HBITMAP)  HB_PARWH(5)) );
}

//-----------------------------------------------------------------------------

HB_FUNC( GETMENUITEMCOUNT )
{
  hb_retni( GetMenuItemCount( (HMENU) HB_PARWH(1) ));
}

//-----------------------------------------------------------------------------

HB_FUNC( SETMENU )
{
  hb_retl( SetMenu ((HWND) HB_PARWH(1), (HMENU) HB_PARWH(2) ) );
}

//-----------------------------------------------------------------------------

HB_FUNC( GETSUBMENU )
{
  HB_RETWH( GetSubMenu( (HMENU) HB_PARWH(1), hb_parni(2)) );
}

//-----------------------------------------------------------------------------

HB_FUNC( TRACKPOPUPMENU )
{
 RECT rc ;

 if (!ISNIL(7)) {
   rc.left   = hb_parni(7,1);
   rc.top    = hb_parni(7,2);
   rc.right  = hb_parni(7,3);
   rc.bottom = hb_parni(7,4);
 }
 hb_retnl( TrackPopupMenu((HMENU) HB_PARWH(1), hb_parni(2), hb_parni(3), hb_parni(4),
                         hb_parni(5), (HWND) HB_PARWH(6), (ISNIL(7) ? NULL : &rc) ) );
}

//-----------------------------------------------------------------------------

HB_FUNC( GETSYSTEMMENU )
{
  HB_RETWH( GetSystemMenu( (HWND) HB_PARWH(1), hb_parl(2) ) );
}


//-----------------------------------------------------------------------------
// WINUSERAPI HMENU WINAPI LoadMenuA( IN HINSTANCE hInstance, IN LPCSTR lpMenuName);


HB_FUNC( LOADMENU )
{
   HB_RETWH( LoadMenu( (HINSTANCE) HB_PARWH( 1 ), (LPCSTR) hb_parcx( 2 ) ) );
}


//-----------------------------------------------------------------------------

// WINUSERAPI HMENU WINAPI LoadMenuIndirectA( IN CONST MENUTEMPLATEA *lpMenuTemplate);

HB_FUNC( LOADMENUINDIRECT )
{
   MENUTEMPLATE *mt =(MENUTEMPLATE * ) hb_parc( 1 ); //hb_param( 1, HB_IT_STRING )->item.asString.value;

   HB_RETWH( LoadMenuIndirect( mt ) );
}


//-----------------------------------------------------------------------------
// WINUSERAPI BOOL WINAPI ChangeMenuA( IN HMENU hMenu, IN UINT cmd, IN LPCSTR lpszNewItem, IN UINT cmdInsert, IN UINT flags);

HB_FUNC( CHANGEMENU )
{
   hb_retl( ChangeMenu( (HMENU) HB_PARWH( 1 ),
                        (UINT) hb_parni( 2 ) ,
                        (LPCSTR) hb_parcx( 3 ),
                        (UINT) hb_parni( 4 ) ,
                        (UINT) hb_parni( 5 )
                      ) );
}


//-----------------------------------------------------------------------------
// WINUSERAPI BOOL WINAPI RemoveMenu( IN HMENU hMenu, IN UINT uPosition, IN UINT uFlags);

HB_FUNC( REMOVEMENU )
{
   hb_retl( RemoveMenu( (HMENU) HB_PARWH( 1 ),
                        (UINT) hb_parni( 2 ) ,
                        (UINT) hb_parni( 3 )
                      ) );
}


//-----------------------------------------------------------------------------
// WINUSERAPI LONG WINAPI GetMenuCheckMarkDimensions( VOID);


HB_FUNC( GETMENUCHECKMARKDIMENSIONS )
{
   hb_retnl( (LONG) GetMenuCheckMarkDimensions(  ) );
}



//-----------------------------------------------------------------------------
// WINUSERAPI BOOL WINAPI ModifyMenuA( IN HMENU hMnu, IN UINT uPosition, IN UINT uFlags, IN UINT_PTR uIDNewItem, IN LPCSTR lpNewItem );

/*

HB_FUNC( MODIFYMENU )
{
   UINT_PTR uIDNewItem ;

   // Your code goes here

   hb_retl( ModifyMenu( (HMENU) HB_PARWH( 1 ),
                        (UINT) hb_parni( 2 ) ,
                        (UINT) hb_parni( 3 ) ,
                        uIDNewItem           ,
                        (LPCSTR) hb_parcx( 5 )
                      ) );
}

*/




//-----------------------------------------------------------------------------
// WINUSERAPI BOOL WINAPI GetMenuInfo( IN HMENU, OUT LPMENUINFO);

/*

HB_FUNC( GETMENUINFO )
{
   LPMENUINFO lpmenuInfo ;

   // Your code goes here

   hb_retl( GetMenuInfo( (HMENU) HB_PARWH( 1 ), lpmenuInfo ) );
}

*/

//-----------------------------------------------------------------------------
// WINUSERAPI BOOL WINAPI SetMenuInfo( IN HMENU, IN LPCMENUINFO);


/*
HB_FUNC( SETMENUINFO )
{
   LPCMENUINFO lpcmenuInfo =(LPCMENUINFO) hb_param( 2, HB_IT_STRING )->item.asString.value;
   hb_retl( SetMenuInfo( (HMENU) HB_PARWH( 1 ), lpcmenuInfo ) );
}
*/


//-----------------------------------------------------------------------------
// WINUSERAPI BOOL WINAPI EndMenu( VOID);

#if (WINVER >=0x0500)
HB_FUNC( ENDMENU )
{
   hb_retl( EndMenu() );
}
#endif
//-----------------------------------------------------------------------------
// WINUSERAPI BOOL WINAPI InsertMenuItemA( IN HMENU, IN UINT, IN BOOL, IN LPCMENUITEMINFOA );



HB_FUNC( INSERTMENUITEM )
{
   LPCMENUITEMINFOA lpcmenuitemInfoa =(LPCMENUITEMINFOA) hb_parc( 4 ); //hb_param( 4, HB_IT_STRING )->item.asString.value;

   hb_retl( InsertMenuItem( (HMENU) HB_PARWH( 1 ),
                            (UINT) hb_parni( 2 ) ,
                            hb_parl( 3 )         ,
                            lpcmenuitemInfoa
                          ) );
}



//-----------------------------------------------------------------------------
// WINUSERAPI BOOL WINAPI GetMenuItemInfoA( IN HMENU, IN UINT, IN BOOL, IN OUT LPMENUITEMINFOA );



HB_FUNC( GETMENUITEMINFO )
{
   LPCMENUITEMINFOA lpcmenuitemInfoa =(LPCMENUITEMINFOA) hb_parc( 4 ); //hb_param( 4, HB_IT_STRING )->item.asString.value;
   hb_retl( GetMenuItemInfo( (HMENU) HB_PARWH( 1 ),
                             (UINT) hb_parni( 2 ) ,
                             hb_parl( 3 )         ,
                             ( struct tagMENUITEMINFOA * ) lpcmenuitemInfoa
                           ) );
}



//-----------------------------------------------------------------------------
// WINUSERAPI BOOL WINAPI SetMenuItemInfoA( IN HMENU, IN UINT, IN BOOL, IN LPCMENUITEMINFOA );



HB_FUNC( SETMENUITEMINFO )
{
   LPCMENUITEMINFOA lpcmenuitemInfoa =(LPCMENUITEMINFOA) hb_parc( 4 ); //hb_param( 4, HB_IT_STRING )->item.asString.value;
   hb_retl( SetMenuItemInfo( (HMENU) HB_PARWH( 1 ),
                             (UINT) hb_parni( 2 ) ,
                             hb_parl( 3 )         ,
                             lpcmenuitemInfoa
                           ) );
}




//-----------------------------------------------------------------------------
// WINUSERAPI BOOL WINAPI GetMenuBarInfo( IN HWND hwnd, IN LONG idObject, IN LONG idItem, OUT PMENUBARINFO pmbi );

/*

HB_FUNC( GETMENUBARINFO )
{
   PMENUBARINFO pmbi     ;

   // Your code goes here

   hb_retl( GetMenuBarInfo( (HWND) HB_PARWH( 1 ),
                            hb_parnl( 2 )       ,
                            hb_parnl( 3 )       ,
                            pmbi
                          ) );
}

*/





//-----------------------------------------------------------------------------

//BOOL CheckMenuRadioItem( HMENU hmenu, UINT idFirst, UINT idLast, UINT idCheck, UINT uFlags );

HB_FUNC( CHECKMENURADIOITEM )
{
  hb_retl( CheckMenuRadioItem( (HMENU) HB_PARWH( 1 ), (UINT) hb_parni(2),
                               (UINT) hb_parni(3), (UINT) hb_parni(4), (UINT) hb_parni(5) ) );
}

HB_FUNC( ISMENU )
{
   hb_retl( IsMenu((HMENU) HB_PARWH(1) ) );
}
