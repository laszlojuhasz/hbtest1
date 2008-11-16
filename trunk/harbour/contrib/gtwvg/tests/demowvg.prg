/*
 * $Id$
 */

//-------------------------------------------------------------------//
//-------------------------------------------------------------------//
//-------------------------------------------------------------------//
//
//                   GTWVT Console GUI Interface
//
//               Pritpal Bedi <pritpal@vouchcac.com>
//
//       I have tried to simulate the gui controls through GDI
//        functions and found a way to refresh those controls
//          through WM_PAINT message issued to the Window.
//                               and
//             I feel that IF this functionality is built
//                 into the GT itself, what a wonder!
//
//   This protocol opens up the the distinct possibilities and hope
//            you all will cooperate to enhance it further.
//
//           Thanks Peter Rees! You have laid the foundation!
//
//-------------------------------------------------------------------//
//-------------------------------------------------------------------//

#include      "inkey.ch"
#include     "common.ch"
#include     "wvtwin.ch"
#include   "hbgtinfo.ch"
#include    "hbgtwvg.ch"

REQUEST DbfCdx

//-------------------------------------------------------------------//
//
//   WvtSetObjects() array structure
//
#define WVT_OBJ_TYPE               1
#define WVT_OBJ_ID                 2
#define WVT_OBJ_ROW                3
#define WVT_OBJ_COL                4
#define WVT_OBJ_ROWTO              5
#define WVT_OBJ_COLTO              6
#define WVT_OBJ_ONDISP             7
#define WVT_OBJ_ONMOUSEOVER        8
#define WVT_OBJ_ONBUTTONDOWN       9
#define WVT_OBJ_ONBUTTONUP        10
#define WVT_OBJ_TOOLTIP           11
#define WVT_OBJ_STATE             12
#define WVT_OBJ_DUMMY             13

#define WVT_OBJ_VRBLS             13

//   WVT_OBJ_TYPE  Constants
//
#define OBJ_TYPE_BUTTON            1

//   WVT_OBJ_STATE
//
#define OBJ_STATE_HIDE             0
#define OBJ_STATE_DISP             1
#define OBJ_STATE_MOUSEOVER        2
#define OBJ_STATE_BUTTONDOWN       3
#define OBJ_STATE_BUTTONUP         4

//-------------------------------------------------------------------//

#define  IMAGE_VOUCH          "vouch1.bmp"
#define  IMAGE_BROWSE         "v_browse.ico"
#define  IMAGE_VR             "vr_1.ico"
#define  IMAGE_NOTES          "v_notes.ico"
#define  IMAGE_TOOLS          "v_tools.ico"
#define  IMAGE_HELP           "v_notes.ico"

//-------------------------------------------------------------------//
//
//   Candidate for inkey.ch
//
#define K_SBLINEUP              1051
#define K_SBLINEDOWN            1052
#define K_SBPAGEUP              1053
#define K_SBPAGEDOWN            1054

#define K_SBLINELEFT            1055
#define K_SBLINERIGHT           1056
#define K_SBPAGELEFT            1057
#define K_SBPAGERIGHT           1058

#define K_SBTHUMBTRACKVERT      1059
#define K_SBTHUMBTRACKHORZ      1060

//-------------------------------------------------------------------//

#ifndef __SQL__
ANNOUNCE Hb_NoStartUpWindow
#endif

//-------------------------------------------------------------------//

#define CRLF   chr( 13 )+chr( 10 )

//-------------------------------------------------------------------//

MEMVAR cCdxExp, First, Last, City

//-------------------------------------------------------------------//

thread static wvtScreen := {}
thread static pic_:= { , , , , , , , , , , , , , , , , , , , }
thread static keys_:= { , , , , , , , , , , , , , , , , , , , }

thread static ahFonts := {}
thread static aSlides := {}
thread static shIcon, shImage

#ifdef __XCC__
static paint_:= { { "", {} } }
#endif

static pGT_:= { NIL,NIL,NIL }
//-------------------------------------------------------------------//
EXIT PROCEDURE KillGTs()

   pGT_[ 1 ] := NIL
   pGT_[ 2 ] := NIL
   pGT_[ 3 ] := NIL

   RETURN
//----------------------------------------------------------------------//
PROCEDURE Main( cDSN )

   LOCAL aLastPaint, clr, scr, bWhen, bValid, a_:={}
   LOCAL dDate   := ctod( "" )
   LOCAL cName   := Pad( "Pritpal Bedi", 35 )
   LOCAL cAdd1   := Pad( "60, New Professor Colony", 35 )
   LOCAL cAdd2   := Pad( "Ludhiana, INDIA", 35 )
   LOCAL cAdd3   := Pad( "http://www.vouchcac.com", 35 )
   LOCAL nSlry   := 20000
   LOCAL aBlocks := {}
   LOCAL nColGet := 8
   LOCAL GetList := {}
   LOCAL nTop    := 4
   LOCAL nLft    := 4
   LOCAL nBtm    := 20
   LOCAL nRgt    := 75
   LOCAL nMaxRows:= MaxRow()
   LOCAL nBtnRow := nMaxRows - 1
   LOCAL cLabel  := "(x)Harbour simulated GUI."
   LOCAL aObjects:= WvtSetObjects( {} )
   LOCAL aObj    := {}
   LOCAL hPopup
   LOCAL oMenu
   LOCAL nConxn
   LOCAL pGT
   SET DATE BRITISH

   SET( _SET_EVENTMASK, INKEY_ALL )

   Wvt_SetGui( .t. )
   WvtSetKeys( .t. )
   Popups( 1 )
   Wvt_SetMouseMove( .t. )
   Wvt_SetFont( "Courier New", 18, 0, 0 )
   CLS
   Wvt_ShowWindow( SW_RESTORE )

   hPopup  := Wvt_SetPopupMenu()
   oMenu   := CreateMainMenu()

   pGT_[ 1 ] := hb_gtSelect()

   //  Force mouse pointer right below the xHarbour label
   //
   Wvt_SetMousePos( 2,40 )

   aAdd( aBlocks, {|| Wvt_SetIcon( "vr_1.ico" ) } )
   aAdd( aBlocks, {|| Wvt_SetTitle( "Vouch" ) } )
   aAdd( aBlocks, {|| Wvt_DrawLabel( 1,40, cLabel,6,, rgb(255,255,255), rgb(198,198,198), "Arial", 26, , , , , .t., .t. ) } )
   aAdd( aBlocks, {|| Wvt_DrawBoxRaised( nTop, nLft, nBtm, nRgt ) } )
   aAdd( aBlocks, {|| Wvt_DrawBoxRecessed( 7, 61, 13, 70 ) } )
   aAdd( aBlocks, {|| Wvt_DrawBoxGroup( 15, 59, 18, 72 ) } )
   aAdd( aBlocks, {|| Wvt_DrawBoxGroup( 5, 6, 19, 44 ) } )
   aAdd( aBlocks, {|| Wvt_DrawImage( 8,62,12,69, IMAGE_VOUCH ) } )
   aAdd( aBlocks, {|| Wvt_DrawBoxRecessed( 7, 48, 13, 55 ) } )
   aAdd( aBlocks, {|| Wvt_DrawLine( maxrow()-2,0,maxrow()-2,maxcol(),WVT_LINE_HORZ,WVT_LINE_RECESSED,WVT_LINE_BOTTOM ) } )

   aAdd( aBlocks, {|| Wvt_DrawLine( maxrow()-1, 4,maxrow(), 4,WVT_LINE_VERT,WVT_LINE_RECESSED,WVT_LINE_CENTER ) } )
   aAdd( aBlocks, {|| Wvt_DrawLine( maxrow()-1,41,maxrow(),41,WVT_LINE_VERT,WVT_LINE_RECESSED,WVT_LINE_CENTER ) } )

   aAdd( aBlocks, {|| aEval( GetList, {|oGet| Wvt_DrawBoxGet( oGet:Row, oGet:Col, Len( Transform( oGet:VarGet(), oGet:Picture ) ) ) } ) } )

   #define btnFDisp   WVT_BTN_FORMAT_FLAT
   #define btnFMOver  WVT_BTN_FORMAT_RAISED
   #define btnFBDown  WVT_BTN_FORMAT_RECESSED
   #define btnFBUp    WVT_BTN_FORMAT_FLAT

   WvtSetObjects( { OBJ_TYPE_BUTTON, 1,  nBtnRow, 6,nBtnRow+1, 9, ;
       {|| Wvt_DrawButton( nBtnRow, 6,nBtnRow+1, 9,  ,IMAGE_VOUCH,btnFDisp  ) },;
       {|| Wvt_DrawButton( nBtnRow, 6,nBtnRow+1, 9,  ,IMAGE_VOUCH,btnFMOver ) },;
       {|| Wvt_DrawButton( nBtnRow, 6,nBtnRow+1, 9,  ,IMAGE_VOUCH,btnFBDown ) },;
       {|| Wvt_DrawButton( nBtnRow, 6,nBtnRow+1, 9,  ,IMAGE_VOUCH,btnFBUp   )  ,;
              eval( SetKey( K_F2 ) ) } ;
                    } )

   WvtSetObjects( { OBJ_TYPE_BUTTON, 2,  nBtnRow,11,nBtnRow+1,14, ;
       {|| Wvt_DrawButton( nBtnRow,11,nBtnRow+1,14,  ,IMAGE_BROWSE, btnFDisp  ) },;
       {|| Wvt_DrawButton( nBtnRow,11,nBtnRow+1,14,  ,IMAGE_BROWSE, btnFMOver ) },;
       {|| Wvt_DrawButton( nBtnRow,11,nBtnRow+1,14,  ,IMAGE_BROWSE, btnFBDown ) },;
       {|| Wvt_DrawButton( nBtnRow,11,nBtnRow+1,14,  ,IMAGE_BROWSE, btnFBUp   )  ,;
              eval( SetKey( K_F5 ) ) } ;
                    } )

   WvtSetObjects( { OBJ_TYPE_BUTTON, 3,  nBtnRow,16,nBtnRow+1,19, ;
       {|| Wvt_DrawButton( nBtnRow,16,nBtnRow+1,19,"Expand",IMAGE_NOTES,btnFDisp  ) },;
       {|| Wvt_DrawButton( nBtnRow,16,nBtnRow+1,19,"Expand",IMAGE_NOTES,btnFMOver ) },;
       {|| Wvt_DrawButton( nBtnRow,16,nBtnRow+1,19,"Expand",IMAGE_NOTES,btnFBDown ) },;
       {|| Wvt_DrawButton( nBtnRow,16,nBtnRow+1,19,"Expand",IMAGE_NOTES,btnFBUp   )  ,;
              eval( SetKey( K_F3 ) ) } ;
                   } )

   WvtSetObjects( { OBJ_TYPE_BUTTON, 4,  nBtnRow,21,nBtnRow+1,24, ;
       {|| Wvt_DrawButton( nBtnRow,21,nBtnRow+1,24,"Shrink",  , btnFDisp , rgb( 100,22,241 ), rgb( 0,100,0 ) ) },;
       {|| Wvt_DrawButton( nBtnRow,21,nBtnRow+1,24,"Shrink",  , btnFMOver, rgb( 100,22,241 ), rgb( 0,100,0 ) ) },;
       {|| Wvt_DrawButton( nBtnRow,21,nBtnRow+1,24,"Shrink",  , btnFBDown, rgb( 100,22,241 ), rgb( 0,100,0 ) ) },;
       {|| Wvt_DrawButton( nBtnRow,21,nBtnRow+1,24,"Shrink",  , btnFBUp  , rgb( 100,22,241 ), rgb( 0,100,0 ) )  ,;
              eval( SetKey( K_F4 ) ) } ;
                   } )

   WvtSetObjects( { OBJ_TYPE_BUTTON, 5,  nBtnRow,26,nBtnRow+1,29, ;
       {|| Wvt_DrawButton( nBtnRow,26,nBtnRow+1,29,"Minimize",IMAGE_TOOLS, btnFDisp  ) },;
       {|| Wvt_DrawButton( nBtnRow,26,nBtnRow+1,29,"Minimize",IMAGE_TOOLS, btnFMOver ) },;
       {|| Wvt_DrawButton( nBtnRow,26,nBtnRow+1,29,"Minimize",IMAGE_TOOLS, btnFBDown ) },;
       {|| Wvt_DrawButton( nBtnRow,26,nBtnRow+1,29,"Minimize",IMAGE_TOOLS, btnFBUp   )  ,;
              eval( SetKey( K_F6 ) ) },;
                   } )

   WvtSetObjects( { OBJ_TYPE_BUTTON, 6, nBtnRow,31,nBtnRow+1,34, ;
       {|| Wvt_DrawButton( nBtnRow,31,nBtnRow+1,34,"Partial",IMAGE_HELP, btnFDisp  ) },;
       {|| Wvt_DrawButton( nBtnRow,31,nBtnRow+1,34,"Partial",IMAGE_HELP, btnFMOver ) },;
       {|| Wvt_DrawButton( nBtnRow,31,nBtnRow+1,34,"Partial",IMAGE_HELP, btnFBDown ) },;
       {|| Wvt_DrawButton( nBtnRow,31,nBtnRow+1,34,"Partial",IMAGE_HELP, btnFBUp   )  ,;
              eval( SetKey( K_F7 ) ) },;
                   } )

   WvtSetObjects( { OBJ_TYPE_BUTTON, 7,  nBtnRow,36,nBtnRow+1,39, ;
       {|| Wvt_DrawButton( nBtnRow,36,nBtnRow+1,39,"Lines",IMAGE_VR, btnFDisp , rgb( 100,22,241 ), rgb( 0,100,0 ) ) },;
       {|| Wvt_DrawButton( nBtnRow,36,nBtnRow+1,39,"Lines",IMAGE_VR, btnFMOver, rgb( 100,22,241 ), rgb( 0,100,0 ) ) },;
       {|| Wvt_DrawButton( nBtnRow,36,nBtnRow+1,39,"Lines",IMAGE_VR, btnFBDown, rgb( 100,22,241 ), rgb( 0,100,0 ) ) },;
       {|| Wvt_DrawButton( nBtnRow,36,nBtnRow+1,39,"Lines",IMAGE_VR, btnFBUp  , rgb( 100,22,241 ), rgb( 0,100,0 ) )  ,;
              eval( SetKey( K_F8 ) ) } ;
                   } )

   aAdd( aBlocks, {|| Wvt_Mouse( -1000001 ) } )

   aLastPaint := WvtSetBlocks( aBlocks )

   scr := SaveScreen( 0,0,maxrow(),maxcol() )
   clr := SetColor( "N/W" )
   CLS
   SetColor( "N/W,N/GR*,,,N/W*" )

   Wvt_SetMenu( oMenu:hMenu )

   SetKey( Wvt_SetMenuKeyEvent(), { || ActivateMenu( oMenu ) } )

   @  6, nColGet SAY "< Date >"
   @  9, nColGet SAY "<" + PadC( "Name", 33 ) + ">"
   @ 12, nColGet SAY "<" + PadC( "Address", 33 ) + ">"
   @ 16, 61      SAY "< Salary >"

   dDate := ctod( "04/01/04" )

   @  7, nColGet GET dDate WHEN  DispStatusMsg( "Date must be Valid" );
                           VALID ClearStatusMsg()
   @ 10, nColGet GET cName WHEN  DispStatusMsg( "Must be one of the list!" );
                           VALID ( VouChoice() < 7 .and. ClearStatusMsg() )
   @ 13, nColGet GET cAdd1
   @ 15, nColGet GET cAdd2
   @ 17, nColGet GET cAdd3
   @ 17, 61      GET nSlry PICTURE "@Z 9999999.99"

   READ

   //  Restore Environment
   //
   WvtSetBlocks( aLastPaint )
   WvtSetObjects( aObjects )
   SetColor( clr )
   RestScreen( 0,0,maxrow(),maxcol(), scr )
   WvtSetKeys( .f. )
   Wvt_SetPopupMenu( hPopup )

   pGT_[ 1 ] := NIL

   RETURN
//-------------------------------------------------------------------//
Function HB_GTSYS()
   REQUEST HB_GT_WVG_DEFAULT
   REQUEST HB_GT_WVT
   Return NIL
//------------------------------------------------------------------//
PROCEDURE WvtConsoleGets()

   IF hb_mtvm()
      Hb_ThreadStart( {||  hb_gtReload( 'WVT' ), ;
                           WvtNextGetsConsole();
                    } )
   ENDIF

   RETURN
//----------------------------------------------------------------------//
PROCEDURE WvtNextGetsConsole()
   LOCAL dDate      := ctod( "" )
   LOCAL cName      := Space( 35 )
   LOCAL cAdd1      := Space( 35 )
   LOCAL cAdd2      := Space( 35 )
   LOCAL cAdd3      := Space( 35 )
   LOCAL nSlry      := 0
   LOCAL nColGet    := 8
   LOCAL GetList    := {}
   LOCAL cLabel     := "VOUCH, that GROWS with you"

   SetMode( 20,51 )
   SetColor( "N/W,N/GR*,,,N/W*" )
   CLS
   hb_gtInfo( HB_GTI_WINTITLE, "WVT Console in WVG Application" )

   @ MaxRow(), 0 SAY PadC( "GTWVT in GTWVG Console Gets", maxcol()+1 ) COLOR "W+/B*"

   @  2, nColGet SAY "< Date >"
   @  5, nColGet SAY "<" + PadC( "Name", 33 ) + ">"
   @  8, nColGet SAY "<" + PadC( "Address", 33) + ">"
   @ 15, nColGet SAY "< Salary >"

   @  3, nColGet GET dDate
   @  6, nColGet GET cName
   @  9, nColGet GET cAdd1
   @ 11, nColGet GET cAdd2
   @ 13, nColGet GET cAdd3
   @ 16, nColGet GET nSlry PICTURE "@Z 9999999.99"

   READ

   RETURN
//-------------------------------------------------------------------//
PROCEDURE WvtNextGets()

   IF hb_mtvm()
      Hb_ThreadStart( {||  Hb_gtReload( 'WVG' ), Wvt_setFont( 'Terminal',20 ), ;
                           hb_clear(), Wvt_ShowWindow( SW_RESTORE ), WvtNextGets_X() } )

   ELSE
      WvtNextGets_X()
   ENDIF

   RETURN

//----------------------------------------------------------------------//

PROCEDURE WvtNextGets_X()

   LOCAL aLastPaint, clr
   LOCAL dDate      := ctod( "" )
   LOCAL cName      := Space( 35 )
   LOCAL cAdd1      := Space( 35 )
   LOCAL cAdd2      := Space( 35 )
   LOCAL cAdd3      := Space( 35 )
   LOCAL nSlry      := 0
   LOCAL aBlocks    := {}
   LOCAL nColGet    := 8
   LOCAL GetList    := {}
   LOCAL nTop       := 4
   LOCAL nLft       := 4
   LOCAL nBtm       := 20
   LOCAL nRgt       := 75
   LOCAL kf2        := SetKey( K_F2, {|| WvtGets() } )
   LOCAL kf3        := SetKey( K_F3, {|| WvtWindowExpand(  1 ) } )
   LOCAL kf4        := SetKey( K_F4, {|| WvtWindowExpand( -1 ) } )
   LOCAL cLabel     := "VOUCH, that GROWS with you"
   LOCAL aPalette   := Wvt_GetPalette()
   LOCAL aNewPalette:= aclone( aPalette )
   LOCAL aObjects   := WvtSetObjects( {} )
   LOCAL nRow       := Row()
   LOCAL nCol       := Col()
   LOCAL scr        := SaveScreen( 0,0,maxrow(),maxcol() )
   LOCAL wvtScr     := Wvt_SaveScreen( 0,0,maxrow(),maxcol() )

   Static nPalletMultiplier := 0

   // Change the values of pallatte arbitrarily though yu can fine tune
   // these values with realistic values.
   //
   aNewPalette[ 8 ] := aNewPalette[ 8 ] + ( 100000 * ++nPalletMultiplier )

   Wvt_SetPalette( aNewPalette )

   aAdd( aBlocks, {|| Wvt_SetTitle( "Wvt Gets 2nd Window with Different Palette" ) } )
   aAdd( aBlocks, {|| Wvt_DrawLine( maxrow()-1,0,maxrow()-1,maxcol() ) })
   aAdd( aBlocks, {|| Wvt_SetBrush( 0, rgb( 32,255,100 ) ) } )
   aAdd( aBlocks, {|| Wvt_DrawEllipse( 6,50,10,58 ) } )
   aAdd( aBlocks, {|| Wvt_SetBrush( 2, rgb( 255,255,100 ),1 ) } )
   aAdd( aBlocks, {|| Wvt_DrawRectangle( 11, 50, 13, 58 ) } )
   aAdd( aBlocks, {|| Wvt_DrawBoxGroupRaised( 5, 6, 19, 72 ) } )
   aAdd( aBlocks, {|| aEval( GetList, {|oGet| Wvt_DrawBoxGet( oGet:Row, oGet:Col, Len( Transform( oGet:VarGet(), oGet:Picture ) ) ) } ) } )

   aAdd( aBlocks, {|| Wvt_DrawButton( 21, 6,22, 9,"New"   ,"vouch1.bmp" ) } )
   aAdd( aBlocks, {|| Wvt_DrawButton( 21,11,22,14,"Browse","vouch1.bmp", 1, rgb( 255,255,255 ) ) } )
   aAdd( aBlocks, {|| Wvt_DrawButton( 21,16,22,19, ,"vouch1.bmp" ) } )
   aAdd( aBlocks, {|| Wvt_DrawButton( 21,21,22,24,"Data",, 0, rgb( 100,22,241 ), rgb( 198,198,198 ) ) } )
   aAdd( aBlocks, {|| Wvt_DrawButton( 21,26,22,29,"Flat",IMAGE_VR,2 ) } )
   aAdd( aBlocks, {|| Wvt_DrawButton( 21,31,22,34,"Outline",IMAGE_VR,3 ) } )
   aAdd( aBlocks, {|| Wvt_DrawButton( 22,36,22,41,"Data",, 0, rgb( 100,22,241 ), rgb( 198,198,198 ) ) } )

   aLastPaint := WvtSetBlocks( aBlocks )

   clr := SetColor( "N/W,N/GR*,,,N/W*" )
   CLS

   @ MaxRow(), 0 SAY PadC( "(x)Harbour + WVT Console GUI Screen",80 ) COLOR "R+/W"

   @  6, nColGet SAY "< Date >"
   @  9, nColGet SAY "<" + PadC( "Name", 33 ) + ">"
   @ 12, nColGet SAY "<" + PadC( "Address", 33) + ">"
   @ 16, 61      SAY "< Salary >"

   @  7, nColGet GET dDate
   @ 10, nColGet GET cName
   @ 13, nColGet GET cAdd1
   @ 15, nColGet GET cAdd2
   @ 17, nColGet GET cAdd3
   @ 17, 61      GET nSlry PICTURE "@Z 9999999.99"

   READ

   // Restore Environment
   //
   Wvt_SetPalette( aPalette )
   WvtSetObjects( aObjects )
   WvtSetBlocks( aLastPaint )
   SetColor( clr )

   RestScreen( 0, 0,maxrow(), maxcol(), scr )
   Wvt_RestScreen( wvtScr )
   SetPos( nRow, nCol )
   RETURN

//-------------------------------------------------------------------//
FUNCTION WvtSetKeys( lSet )

   if lSet
      keys_[ 2 ] := SetKey( K_F2, {|| WvtNextGets()         } )
      keys_[ 3 ] := SetKey( K_F3, {|| WvtWindowExpand( 1 )  } )
      keys_[ 4 ] := SetKey( K_F4, {|| WvtWindowExpand( -1 ) } )
      keys_[ 5 ] := SetKey( K_F5, {|| WvtMyBrowse()         } )
      keys_[ 6 ] := SetKey( K_F6, {|| Wvt_Minimize()        } )
      keys_[ 7 ] := SetKey( K_F7, {|| WvtPartialScreen()    } )
      keys_[ 8 ] := SetKey( K_F8, {|| WvtLines()            } )
      keys_[ 9 ] := SetKey( K_F9, {|| Wvt_ChooseFont()      } )
      keys_[ 10] := SetKey( K_F10,{|| Wvt_ChooseColor()     } )
   else
      SetKey( K_F2,  keys_[ 2 ] )
      SetKey( K_F3,  keys_[ 3 ] )
      SetKey( K_F4,  keys_[ 4 ] )
      SetKey( K_F5,  keys_[ 5 ] )
      SetKey( K_F6,  keys_[ 6 ] )
      SetKey( K_F7,  keys_[ 7 ] )
      SetKey( K_F8,  keys_[ 8 ] )
      SetKey( K_F9,  keys_[ 9 ] )
      SetKey( K_F10, keys_[ 10] )
   endif

   RETURN Nil
//-------------------------------------------------------------------//
//      Wvt_Paint() must be a FUNCTION in your application
//      as it is called when Window gets WM_PAINT message.
//-------------------------------------------------------------------//
FUNCTION Wvt_Paint()
   LOCAL aBlocks := WvtSetBlocks()

   aEval( aBlocks, {|e| eval( e ) } )

   WvtPaintObjects()

   RETURN 0
//-------------------------------------------------------------------//
//      Wvt_SetFocus() must be a FUNCTION in your application
//      needs to process messages sent through WM_SETFOCUS message
//      received by the window.
//-------------------------------------------------------------------//
FUNCTION Wvt_SetFocus( hWnd )

   LOCAL nRow := row()
   LOCAL nCol := col()

   DispOutAt( 1,3, "Focus Gained!", "R/W" )

   DevPos( nRow, nCol )

   RETURN nil
//-------------------------------------------------------------------//
//      Wvt_KillFocus() must be a FUNCTION in your application
//      needs to process messages sent through WM_KILLFOCUS message
//      received by the window.
//-------------------------------------------------------------------//
FUNCTION Wvt_KillFocus( hWnd )

   LOCAL nRow := row()
   LOCAL nCol := col()

   DispOutAt( 1,3, "Focus Lost...", "B/W" )

   DevPos( nRow, nCol )

   RETURN nil
//-------------------------------------------------------------------//
//
//      Wvt_Mouse() must be present if you want to catch and fire
//      mouse call back outside of the inkey() loop.
//
//-------------------------------------------------------------------//
FUNCTION Wvt_Mouse( nKey, nRow, nCol )
   LOCAL i, nLen, aObjects := WvtSetObjects()
   LOCAL nObj

   STATIC nLastObj := 0
   STATIC nLastKey := 0

   if ( nLen := len( aObjects ) ) == 0
      return nil
   endif

   if !SetMouseCheck()
      return nil
   endif

   if nKey == -1000001
      for nObj :=   1 to nLen
         DO CASE
         CASE aObjects[ nObj, WVT_OBJ_STATE ] == OBJ_STATE_DISP
            eval( aObjects[ nObj, WVT_OBJ_ONDISP ] )
         CASE aObjects[ nObj, WVT_OBJ_STATE ] == OBJ_STATE_MOUSEOVER
            eval( aObjects[ nObj, WVT_OBJ_ONMOUSEOVER ] )
         CASE aObjects[ nObj, WVT_OBJ_STATE ] == OBJ_STATE_BUTTONDOWN
            eval( aObjects[ nObj, WVT_OBJ_ONBUTTONDOWN ] )
         CASE aObjects[ nObj, WVT_OBJ_STATE ] == OBJ_STATE_BUTTONUP
            eval( aObjects[ nObj, WVT_OBJ_ONDISP ] )
         CASE aObjects[ nObj, WVT_OBJ_STATE ] == OBJ_STATE_HIDE

         ENDCASE
      next
      return nil
   endif

   nObj := ascan( aObjects, {|e_| e_[ WVT_OBJ_ROW   ] <= nRow .and. ;
                                  e_[ WVT_OBJ_ROWTO ] >= nRow .and. ;
                                  e_[ WVT_OBJ_COL   ] <= nCol .and. ;
                                  e_[ WVT_OBJ_COLTO ] >= nCol     } )
   if nObj == 0
      if nLastObj > 0
         aObjects[ nLastObj, WVT_OBJ_STATE ] := OBJ_STATE_DISP
         eval( aObjects[ nLastObj, WVT_OBJ_ONDISP ] )
         nLastObj := 0
      endif
      return nil
   endif

   if nLastObj == nObj .and. nLastKey == nKey
      return nil
   endif

   nLastObj := nObj
   nLastKey := nKey

   DO CASE
   CASE nKey == K_MOUSEMOVE
      if aObjects[ nLastObj, WVT_OBJ_STATE ] != OBJ_STATE_MOUSEOVER
           aObjects[ nLastObj, WVT_OBJ_STATE ] := OBJ_STATE_MOUSEOVER
         if aObjects[ nObj, WVT_OBJ_ONMOUSEOVER ] != nil
            eval( aObjects[ nObj, WVT_OBJ_ONMOUSEOVER ] )
         endif
      endif

   CASE nKey == K_LBUTTONDOWN
        aObjects[ nLastObj, WVT_OBJ_STATE ] := OBJ_STATE_BUTTONDOWN
        if aObjects[ nObj, WVT_OBJ_ONBUTTONDOWN ] != nil
           eval( aObjects[ nObj, WVT_OBJ_ONBUTTONDOWN ] )
      endif

   CASE nKey == K_LBUTTONUP
        aObjects[ nLastObj, WVT_OBJ_STATE ] := OBJ_STATE_DISP
        if aObjects[ nObj, WVT_OBJ_ONBUTTONUP ] != nil
           eval( aObjects[ nObj, WVT_OBJ_ONBUTTONUP ] )
      endif

   ENDCASE

   RETURN nil
//-------------------------------------------------------------------//
//  WvtSetBlocks() is a get/set FUNCTION to be used by Wvt_Paint()
//-------------------------------------------------------------------//
FUNCTION WvtSetBlocks( a_ )

   LOCAL o
   THREAD STATIC s := {}

   o := aclone( s )

   IF a_ != nil
      s := aclone( a_ )
   ENDIF

   RETURN o
//-------------------------------------------------------------------//
//  WvtSetObjects() is a get/set FUNCTION to be used by Wvt_Mouse()
//-------------------------------------------------------------------//
FUNCTION WvtSetObjects( aObject )

   LOCAL oObjects
   THREAD STATIC aObjects := {}

   oObjects := aclone( aObjects )

   if aObject != nil
      if empty( aObject )
         aObjects := {}
      else
         if valtype( aObject[ 1 ] ) == "A"
            aeval( aObject, {|e_| aadd( aObjects, e_ ) } )
         else
            aSize( aObject, WVT_OBJ_VRBLS )

            DEFAULT aObject[ WVT_OBJ_STATE ] TO OBJ_STATE_DISP

            aadd( aObjects, aObject )
         endif
      endif
   endif

   RETURN oObjects
//-------------------------------------------------------------------//
FUNCTION SetMouseCheck( lYes )
   LOCAL lOYes
   STATIC lSYes := .t.

   lOYes := lSYes
   if lYes != nil
      lSYes := lYes
   endif

   RETURN lOYes
//-------------------------------------------------------------------//
FUNCTION WvtWindowExpand( nUnits )

   STATIC sUnits := 18

   sUnits += nUnits

   Wvt_setFont( "Courier New", sUnits )

   RETURN .t.
//-------------------------------------------------------------------//
STATIC FUNCTION rgb( r,g,b )
   RETURN ( r + ( g * 256 ) + ( b * 256 * 256 ) )
//-------------------------------------------------------------------//
FUNCTION VouChoice( aChoices )

   LOCAL scr, clr, nChoice

   DEFAULT aChoices TO { "One","Two","Three","Four","Five","Six","Seven" }

   scr := SaveScreen( 7,48,13,55 )
   clr := SetColor( "N/W*,GR+/B*,,,GR+/B" )

   nChoice := aChoice( 7, 48, 13, 55, aChoices )

   setColor( clr )
   RestScreen( 7, 48, 13, 55, scr )

   RETURN nChoice
//-------------------------------------------------------------------//
FUNCTION Hb_Clear()
   CLS
   RETURN .f.
//----------------------------------------------------------------------//
FUNCTION WvtMyBrowse()

   IF hb_mtvm()
      Hb_ThreadStart( {|oCrt|  oCrt := WvgCrt():New( , , { -1,-2 }, { 34,69 }, , .T. ), ;
                            oCrt:resizeMode := HB_GTI_RESIZEMODE_ROWS,;
                            oCrt:icon := "dia_excl.ico",;
                            oCrt:create(),;
                            Wvt_SetGui( .t. ),;
                            WvtMyBrowse_X(),;
                            oCrt:destroy();
                  } )

   ELSE
      WvtMyBrowse_X()
   ENDIF

   Return NIL
//----------------------------------------------------------------------//
FUNCTION WvtMyBrowse_X()
   LOCAL nKey, bBlock, oBrowse , aLastPaint, i, aLastPaint1
   LOCAL lEnd    := .f.
   LOCAL aBlocks := {}
   LOCAL info_   := {}
   LOCAL nTop    :=  3
   LOCAL nLeft   :=  3
   LOCAL nBottom := maxrow() - 2
   LOCAL nRight  := maxcol() - 3
   LOCAL nCursor := setCursor( 0 )
   LOCAL nRow    := row()
   LOCAL nCol    := col()
   LOCAL cColor  := SetColor( "N/W*,N/GR*,,,N/W*" )
   LOCAL cScr    := SaveScreen( 0,0,maxrow(),maxcol() )
   LOCAL aObjects:= WvtSetObjects( {} )
   LOCAL hPopup  := Wvt_SetPopupMenu()
   LOCAL stru_:={}, cDbfFile, cSqlFile, cFileIndex, cFileDbf, cRDD, nIndex
   Local pGT, pGT1, nStStyle, nExStyle

   STATIC nStyle := 0
   THREAD STATIC nFactor := 200

   pGT_[ 2 ] := hb_gtSelect()

   cRDD       := "DBFCDX"
   cFileDbf   := "test.dbf"
   cFileIndex := "test.z01"

   USE ( cFileDbf ) NEW SHARED VIA ( cRDD )
   if NetErr()
      return nil
   endif
   if fLock()
      INDEX ON Test->FIRST TAG "001" TO ( cFileIndex )
      INDEX ON Test->LAST  TAG "002" TO ( cFileIndex )
      INDEX ON Test->CITY  TAG "003" TO ( cFileIndex )
      dbUnlock()
   endif
   SET INDEX TO
   SET INDEX TO ( cFileIndex )
   SET ORDER TO 1
   DbGoTo( 50 )

   info_:= DbStruct()

   Popups( 2 )

   oBrowse := TBrowseWVG():New( nTop + 3, nLeft + 2, nBottom - 1, nRight - 2 )

   oBrowse:ColSep        := "  "
   oBrowse:HeadSep       := "__"
   oBrowse:GoTopBlock    := { || dbGoTop() }
   oBrowse:GoBottomBlock := { || dbGoBottom() }
   oBrowse:SkipBlock     := { | nSkip | dbSkipBlock( nSkip,oBrowse ) }

   for i := 1 to len( info_ )
      bBlock := VouBlockField( i )
      oBrowse:AddColumn( TBColumnNew( info_[ i,1 ], bBlock ) )
   next

   oBrowse:configure()

   if nStyle > 5
      nStyle := 0
   endif

   Wvt_SetPen( nStyle, 0, rgb( 210,1210,210 ) )

   nStyle++

   //hb_gtInfo( HB_GTI_ICONFILE, "dia_excl.ico" )
   hb_gtInfo( HB_GTI_WINTITLE, "WVT Gui TBrowse()" )

   aAdd( aBlocks, {|| Wvt_DrawBoxRaised( oBrowse:nTop-3, oBrowse:nLeft-2, oBrowse:nBottom+1, oBrowse:nRight+2 ) } )
   aAdd( aBlocks, {|| Wvt_DrawBoxRecessed( oBrowse:nTop, oBrowse:nLeft, oBrowse:nBottom, oBrowse:nRight ) } )
   aAdd( aBlocks, {|| Wvt_DrawGridHorz( oBrowse:nTop+3, oBrowse:nLeft, oBrowse:nRight, oBrowse:nBottom - oBrowse:nTop - 2 ) } )
   aAdd( aBlocks, {|| Wvt_DrawGridVert( oBrowse:nTop, oBrowse:nBottom, oBrowse:aColumnsSep, len( oBrowse:aColumnsSep ) ) } )

   aLastPaint := WvtSetBlocks( aBlocks )

   DispBox( 0, 0, maxrow(), maxcol(), "         ", "N/W" )
   DispOutAt( oBrowse:nTop-2, oBrowse:nleft, padc( CurDrive()+":\"+CurDir()+"\"+"test.dbf", oBrowse:nRight-oBrowse:nLeft+1 ), "W+/W" )
   DispOutAt( maxrow(), 0, padc( '<F3 Modal Window> <F4 Maximize> <F11 Transp++> <F12 Transp--> <Thread'+str(ThreadID(),3)+'>',maxcol()+1), 'B/W' )

   While !lEnd
      oBrowse:ForceStable()

      nKey := InKey( 0, INKEY_ALL )

      do case
      case nKey == K_F12
         nFactor--
         hb_gtInfo( HB_GTI_SPEC, HB_GTS_FACTOR, nFactor )

      case nKey == K_F11
         nFactor++
         hb_gtInfo( HB_GTI_SPEC, HB_GTS_FACTOR, nFactor )

      case BrwHandleKey( oBrowse, nKey, @lEnd )

      case nKey == HB_K_RESIZE
         oBrowse:nBottom := maxrow() - 3
         oBrowse:nRight  := maxcol() - 5

         DispBox( 0, 0, maxrow(), maxcol(), "         ", "N/W" )
         DispOutAt( oBrowse:nTop-2, oBrowse:nleft, padc( CurDrive()+":\"+CurDir()+"\"+"test.dbf", oBrowse:nRight - oBrowse:nLeft + 1 ), "W+/W" )
         DispOutAt( maxrow(), 0, padc( '<F3 Modal Window> <F4 Maximize> <F11 Transp++> <F12 Transp--> <Thread'+str(ThreadID(),3)+'>',maxcol()+1), 'B/W' )
         oBrowse:configure()

      case nKey == K_F2
         nIndex := IndexOrd()
         nIndex++
         if nIndex > 3
            nIndex := 1
         endif
         Set Order To ( nIndex )
         oBrowse:RefreshAll()

      case nKey == K_F3
         aLastPaint1 := WvtSetBlocks( {} )
         DoModalWindow()
         WvtSetBlocks( aLastPaint1 )

      case nKey == K_F4
         hb_gtInfo( HB_GTI_SPEC, HB_GTS_WNDSTATE, HB_GTS_WS_MAXIMIZED )

      endcase
   end

   Wvt_SetPen( 0 )
   WvtSetBlocks( aLastPaint )
   WvtSetObjects( aObjects )

   DevPos( nRow, nCol )
   SetColor( cColor )
   SetCursor( nCursor )

   DBCloseArea()
   RestScreen( 0, 0, maxrow(), maxcol(), cScr )
   Wvt_setPopupMenu( hPopup )
   pGT_[ 2 ] := NIL

   RETURN nil
//-------------------------------------------------------------------//
STATIC FUNCTION DbSkipBlock( n, oTbr )

   LOCAL nSkipped := 0

   if n == 0
      DBSkip( 0 )

   elseif n > 0
      do while nSkipped != n .and. TBNext( oTbr )
         nSkipped++
      enddo
   else
      do while nSkipped != n .and. TBPrev( oTbr )
         nSkipped--
      enddo
   endif

   RETURN  nSkipped
//-------------------------------------------------------------------//
STATIC FUNCTION TBNext( oTbr )

   LOCAL nSaveRecNum := recno()
   LOCAL lMoved := .T.

   if Eof()
      lMoved := .F.
   else
      DBSkip( 1 )
      if Eof()
         lMoved := .F.
         DBGoTo( nSaveRecNum )
      endif
   endif

   RETURN lMoved
//-------------------------------------------------------------------//
STATIC FUNCTION TBPrev( oTbr )
   LOCAL nSaveRecNum := Recno()
   LOCAL lMoved := .T.

   DBSkip( -1 )

   if Bof()
      DBGoTo( nSaveRecNum )
      lMoved := .F.
   endif

   RETURN lMoved
//-------------------------------------------------------------------//
STATIC FUNCTION VouBlockField( i )

   RETURN  {|| fieldget( i ) }
//-------------------------------------------------------------------//
STATIC FUNCTION BrwHandleKey( oBrowse, nKey, lEnd )
   LOCAL lRet := .t.

   do case
   case nKey == K_ESC
      lEnd := .t.

   case nKey == K_ENTER
      lEnd := .t.

   case nKey == K_DOWN
      oBrowse:Down()

   case nKey == K_UP
      oBrowse:Up()

   case nKey == K_LEFT
      oBrowse:Left()

   case nKey == K_RIGHT
      oBrowse:Right()

   case nKey == K_PGDN
      oBrowse:pageDown()

   case nKey == K_PGUP
      oBrowse:pageUp()

   case nKey == K_CTRL_PGUP
      oBrowse:goTop()

   case nKey == K_CTRL_PGDN
      oBrowse:goBottom()

   case nKey == K_HOME
      oBrowse:home()

   case nKey == K_END
      oBrowse:end()

   case nKey == K_CTRL_LEFT
      oBrowse:panLeft()

   case nKey == K_CTRL_RIGHT
      oBrowse:panRight()

   case nKey == K_CTRL_HOME
      oBrowse:panHome()

   case nKey == K_CTRL_END
      oBrowse:panEnd()

   case nKey == K_MWBACKWARD
      oBrowse:down()

   case nKey == K_MWFORWARD
      oBrowse:up()

   otherwise
      lRet := .f.

   endcase

   RETURN lRet

//-------------------------------------------------------------------//

STATIC FUNCTION BrwOnEvent( oWvtBrw, cPaintID, oBrowse, nKey )
   LOCAL lRet := .t., lRefAll := .f.

   do case
   case nKey == K_DOWN
      oBrowse:Down()

   case nKey == K_UP
      oBrowse:Up()

   case nKey == K_LEFT
      oBrowse:Left()

   case nKey == K_RIGHT
      oBrowse:Right()

   case nKey == K_PGDN
      oBrowse:pageDown()
      lRefAll := .t.

   case nKey == K_PGUP
      oBrowse:pageUp()
      lRefAll := .t.

   case nKey == K_CTRL_PGUP
      oBrowse:goTop()
      lRefAll := .t.

   case nKey == K_CTRL_PGDN
      oBrowse:goBottom()
      lRefAll := .t.

   case nKey == K_HOME
      oBrowse:home()

   case nKey == K_END
      oBrowse:end()

   case nKey == K_CTRL_LEFT
      oBrowse:panLeft()

   case nKey == K_CTRL_RIGHT
      oBrowse:panRight()

   case nKey == K_CTRL_HOME
      oBrowse:panHome()

   case nKey == K_CTRL_END
      oBrowse:panEnd()

   case nKey == K_MWBACKWARD
      oBrowse:down()

   case nKey == K_MWFORWARD
      oBrowse:up()

   case nKey == K_SBTHUMBTRACKVERT
      OrdKeyGoTo( oWvtBrw:oVBar:GetPos() )
      lRefAll := .t.

   case nKey == K_SBTHUMBTRACKHORZ
      oBrowse:ColPos := oWvtBrw:oHBar:GetPos()

   case nKey == K_SBLINEUP
      oBrowse:up()

   case nKey == K_SBLINEDOWN
      oBrowse:down()

   case nKey == K_SBPAGEUP
     oBrowse:PageUp()

   case nKey == K_SBPAGEDOWN
      oBrowse:PageDown()

   case nKey == K_SBLINELEFT
      oBrowse:Left()

   case nKey == K_SBLINERIGHT
      oBrowse:Right()

   case nKey == K_SBPAGELEFT
      oBrowse:Left()

   case nKey == K_SBPAGERIGHT
      oBrowse:right()

   otherwise
      lRet := .f.

   endcase

   if lRet
      if lRefAll
         oBrowse:RefreshAll()
      endif
      oBrowse:ForceStable()

      oWvtBrw:oVBar:SetPos( OrdKeyCount(),OrdKeyNo() )
      oWvtBrw:oHBar:SetPos( oBrowse:ColCount, oBrowse:ColPos )
   endif

   RETURN lRet

//-------------------------------------------------------------------//

STATIC FUNCTION CfgMyBrowse( aFields, cUseAlias, aTLBR, cDesc, oParent, cColorSpec, nID )
   LOCAL info_, oWvtBrw, oBrowse, i, bBlock
   LOCAL aPopup := {}

   aadd( aPopup, { "Down"     , {|| oBrowse:Down()    , oBrowse:ForceStable() } } )
   aadd( aPopup, { "Up"       , {|| oBrowse:Up()      , oBrowse:ForceStable() } } )
   aadd( aPopup, { "Page Down", {|| oBrowse:PageDown(), oBrowse:ForceStable() } } )
   aadd( aPopup, { "Page Up"  , {|| oBrowse:PageUp()  , oBrowse:ForceStable() } } )
   aadd( aPopup, { "Top"      , {|| oBrowse:GoTop()   , oBrowse:ForceStable() } } )
   aadd( aPopup, { "Bottom"   , {|| oBrowse:GoBottom(), oBrowse:ForceStable() } } )

   Select( cUseAlias )
   info_:= DbStruct()

   //oBrowse := TBrowseNew( aTLBR[ 1 ], aTLBR[ 2 ], aTLBR[ 3 ], aTLBR[ 4 ] )
   oBrowse := TBrowseWVG():New( aTLBR[ 1 ], aTLBR[ 2 ], aTLBR[ 3 ], aTLBR[ 4 ] )

   oBrowse:ColSep        := "  "
   oBrowse:HeadSep       := "__"
   oBrowse:ColorSpec     := cColorSpec
   oBrowse:GoTopBlock    := { || dbGoTop() }
   oBrowse:GoBottomBlock := { || dbGoBottom() }
   oBrowse:SkipBlock     := { | nSkip | dbSkipBlock( nSkip,oBrowse ) }

   for i := 1 to len( aFields )
      bBlock := VouBlockField( aFields[ i ] )
      oBrowse:AddColumn( TBColumnNew( info_[ aFields[ i ],1 ], bBlock ) )
   next

   oBrowse:configure()

   oWvtBrw := WvtBrowse():New( oParent,nID )

   oWvtBrw:nTop         := aTLBR[ 1 ]
   oWvtBrw:nLeft        := aTLBR[ 2 ]
   oWvtBrw:nBottom      := aTLBR[ 3 ]
   oWvtBrw:nRight       := aTLBR[ 4 ]
   oWvtBrw:cAlias       := cUseAlias
   oWvtBrw:oBrw         := oBrowse
   oWvtBrw:cDesc        := cDesc
   oWvtBrw:nPointer     := WVT_IDC_HAND
   oWvtBrw:cColorHilite := "W+/B*"
   oWvtBrw:Tooltip      := cDesc
   oWvtBrw:aPopup       := aPopup

   oWvtBrw:bHandleEvent := {|oWvtBrw,cPaintID,oBrowse,nKey| BrwOnEvent( oWvtBrw,cPaintID,oBrowse,nKey ) }

   RETURN oWvtBrw

//-------------------------------------------------------------------//

FUNCTION WvtPartialScreen()
   LOCAL scr        := SaveScreen( 7,20,15,60 )
   LOCAL wvtScr     := Wvt_SaveScreen( 0, 0, MaxRow(), MaxCol() )
   LOCAL wvtScr1
   LOCAL aLastPaint
   LOCAL hPopup     := Wvt_SetPopupMenu()

   aLastPaint := WvtSetBlocks( {} )

   DispBox( 7, 20, 15, 60, "         ", "W/GR*" )
   @ 10,25 SAY "Wvt_SaveScreen()" COLOR "N/GR*"
   @ 11,25 SAY "Wvt_RestScreen()" COLOR "N/GR*"
   @ 13,25 SAY "Press Esc "       COLOR "N/GR*"
   Wvt_DrawBoxRecessed( 8,22,14,58 )

   wvtScr1 := Wvt_SaveScreen( 7,20,15,60 )

   do while inkey( 0 ) != K_ESC
   enddo

   DispBox( 7, 20, 15, 60, "         ", "W/B*" )
   @ 10,25 SAY "Wvt_SaveScreen()" COLOR "N/B*"
   @ 11,25 SAY "Wvt_RestScreen()" COLOR "N/B*"
   @ 13,25 SAY "Press Esc "       COLOR "N/B*"
   Wvt_DrawBoxRecessed( 8,22,14,58 )

   do while inkey( 0 ) != K_ESC
   enddo

   Wvt_RestScreen( 7,20,15,60, wvtScr1 )

   do while inkey( 0 ) != K_ESC
   enddo

   RestScreen( 7,20,15,60,scr )
   Wvt_RestScreen( 0, 0, MaxRow(), MaxCol(), wvtScr )
   WvtSetBlocks( aLastPaint )
   Wvt_SetPopupMenu( hPopup )

   RETURN NIL

//-------------------------------------------------------------------//

function WvtLines()
   LOCAL scr        := SaveScreen( 0,0,maxrow(),maxcol() )
   LOCAL clr        := SetColor( "N/W" )
   LOCAL nRows      := maxrow()
   LOCAL nCols      := maxcol()
   LOCAL aLastPaint := WvtSetBlocks( {} )
   LOCAL aObjects   := WvtSetObjects( {} )
   LOCAL hPopup     := Wvt_SetPopupMenu()
   LOCAL aBlocks    := {}

   CLS

   aAdd( aBlocks, {|| Wvt_DrawLine( 0, 0, 0, nCols, WVT_LINE_HORZ, WVT_LINE_RAISED  , WVT_LINE_CENTER ) } )
   aAdd( aBlocks, {|| Wvt_DrawLine( 1, 0, 1, nCols, WVT_LINE_HORZ, WVT_LINE_RECESSED, WVT_LINE_TOP ) } )
   aAdd( aBlocks, {|| Wvt_DrawLine( 2, 0, 2, nCols, WVT_LINE_HORZ, WVT_LINE_PLAIN   , WVT_LINE_CENTER, WVT_LINE_SOLID, 4, Rgb( 255,255,255 ) ) } )
   aAdd( aBlocks, {|| Wvt_DrawLine( 3, 0, 3, nCols, WVT_LINE_HORZ, WVT_LINE_RAISED  , WVT_LINE_CENTER, WVT_LINE_DASH , 0, Rgb( 255,0,0 ) ) } )
   aAdd( aBlocks, {|| Wvt_DrawLine( 4, 0, 4, nCols, WVT_LINE_HORZ, WVT_LINE_RECESSED, WVT_LINE_BOTTOM ) } )

   @ 0, 1 SAY "Center Raised"
   @ 1,11 say "Top Recessed"
   @ 2,21 say "Center Plain White 3 Pixels"
   @ 3,31 say "Center Raised Dotted"
   @ 4,41 SAY "Bottom Recessed"
   @ 5, 1 SAY "Bottom Checked"

   @ nRows, 0 Say PadC( "Press ESC to Quit", nCols+1 ) COLOR "GR+/W"

   aAdd( aBlocks, {|| Wvt_DrawLine( 11, 5,nRows-2, 5, WVT_LINE_VERT, WVT_LINE_RAISED  , WVT_LINE_CENTER ) } )
   aAdd( aBlocks, {|| Wvt_DrawLine( 11, 6,nRows-2, 6, WVT_LINE_VERT, WVT_LINE_RECESSED, WVT_LINE_CENTER ) } )
   aAdd( aBlocks, {|| Wvt_DrawLine( 11, 7,nRows-2, 7, WVT_LINE_VERT, WVT_LINE_PLAIN   , WVT_LINE_LEFT   ) } )
   aAdd( aBlocks, {|| Wvt_DrawLine( 11, 8,nRows-2, 8, WVT_LINE_VERT, WVT_LINE_PLAIN   , WVT_LINE_CENTER ) } )
   aAdd( aBlocks, {|| Wvt_DrawLine( 11, 9,nRows-2, 9, WVT_LINE_VERT, WVT_LINE_PLAIN   , WVT_LINE_RIGHT  ) } )
   aAdd( aBlocks, {|| Wvt_DrawLine( 11,10,nRows-2,10, WVT_LINE_VERT, WVT_LINE_PLAIN   , WVT_LINE_CENTER, WVT_LINE_DOT,     0, RGB( 0,0,255 ) ) } )
   aAdd( aBlocks, {|| Wvt_DrawLine( 11,11,nRows-2,11, WVT_LINE_VERT, WVT_LINE_PLAIN   , WVT_LINE_CENTER, WVT_LINE_DASH,    0, RGB( 255,0,0 ) ) } )
   aAdd( aBlocks, {|| Wvt_DrawLine( 11,12,nRows-2,12, WVT_LINE_VERT, WVT_LINE_PLAIN   , WVT_LINE_CENTER, WVT_LINE_DASHDOT, 0, RGB( 0,255,0 ) ) } )

   WvtSetBlocks( aBlocks )

   @ 12,5 Say "A"
   @ 13,6 Say "B"
   @ 14,7 Say "C"
   @ 15,8 Say "D"
   @ 16,9 Say "E"

   do while ( inkey(0) != 27 )
   enddo

   //  Restore Environments
   //
   SetColor( clr )

   WvtSetBlocks( aLastPaint )
   WvtSetObjects( aObjects )
   Wvt_SetPopupMenu( hPopup )

   RestScreen( 0,0,maxrow(),maxcol(), scr )

   RETURN nil

//-------------------------------------------------------------------//

FUNCTION DispStatusMsg( cMsg )

   Wvt_DrawLabel( MaxRow(), 60, cMsg, 6, , 0, rgb(198,198,198), "Arial", 18, , 900 )

   RETURN .t.

//-------------------------------------------------------------------//

FUNCTION ClearStatusMsg()
   LOCAL nRow := Row()
   LOCAL nCol := Col()

   DispOutAt( MaxRow(), 42, space( 37 ), "W/W" )

   SetPos( nRow, nCol )

   RETURN .t.

//-------------------------------------------------------------------//

FUNCTION Popups( nID, lDestroy )
   LOCAL hPop, hPop1
   LOCAL nPrompt := MF_ENABLED+MF_STRING

   THREAD STATIC hPop_:= { , , , , , , , , }

   if nID == nil
      Wvt_SetPopupMenu()
      return nil
   endif

   if lDestroy != nil
      Wvt_DestroyMenu( hPop_[ nID ] )
      return nil
   endif

   hPop := hPop_[ nID ]

   do case
   case nID == 1   //  Data Entry Module

      if hPop == nil
         hPop := Wvt_CreatePopupMenu()
         Wvt_AppendMenu( hPop, nPrompt, K_F2, "Second Get Screen" )
         Wvt_AppendMenu( hPop, nPrompt, K_F3, "Expand Window"     )
         Wvt_AppendMenu( hPop, nPrompt, K_F4, "Shrink Window"     )
         Wvt_AppendMenu( hPop, nPrompt, K_F5, "Browse"            )
         Wvt_AppendMenu( hPop, nPrompt, K_F6, "Minimize"          )
         Wvt_AppendMenu( hPop, nPrompt, K_F7, "Partial Screen"    )
         Wvt_AppendMenu( hPop, nPrompt, K_F8, "Lines"             )
         Wvt_AppendMenu( hPop, nPrompt, K_F9, "Choose Font"       )
         Wvt_AppendMenu( hPop, nPrompt, K_F10,"Choose Color"      )

         Wvt_AppendMenu( hPop, MF_SEPARATOR )

         Wvt_AppendMenu( hPop, nPrompt, K_F5, "Browse"  )

      endif

   case nID == 2   //  Browser

      if hPop == nil
         hPop := Wvt_CreatePopupMenu()
         Wvt_AppendMenu( hPop, nPrompt, K_DOWN     , "Down"      )
         Wvt_AppendMenu( hPop, nPrompt, K_UP       , "Up"        )
         Wvt_AppendMenu( hPop, nPrompt, K_PGDN     , "Page Down" )
         Wvt_AppendMenu( hPop, nPrompt, K_PGUP     , "Page Up"   )
         Wvt_AppendMenu( hPop, nPrompt, K_CTRL_PGUP, "Top"       )
         Wvt_AppendMenu( hPop, nPrompt, K_CTRL_PGDN, "Bottom"    )

         Wvt_AppendMenu( hPop, MF_SEPARATOR )

         hPop1 := Wvt_CreatePopupMenu()
         Wvt_AppendMenu( hPop1, nPrompt, K_RIGHT   , "Right"     )
         Wvt_AppendMenu( hPop1, nPrompt, K_LEFT    , "Left"      )
         Wvt_AppendMenu( hPop1, nPrompt, K_END     , "End"       )
         Wvt_AppendMenu( hPop1, nPrompt, K_HOME    , "Home"      )

         Wvt_AppendMenu( hPop, MF_ENABLED+MF_POPUP, hPop1, "Column Movement" )

      endif

   endcase

   hPop_[ nID ] := hPop

   RETURN Wvt_SetPopupMenu( hPop_[ nID ] )

//-------------------------------------------------------------------//

FUNCTION WvtPictures( nSlot,cFilePic )

   if nSlot != nil .and. nSlot <= 20 .and. file( cFilePic )
      if pic_[ nSlot ] != cFilePic
         if Wvt_LoadPicture( cFilePic, nSlot )
            pic_[ nSlot ] := cFilePic
         endif
      endif
   endif

   RETURN nil

//-------------------------------------------------------------------//

FUNCTION WvtExePicture( nTop, nLeft, nBottom, nRight, nSlot, aOffset )

   if pic_[ nSlot ] != nil
      Wvt_DrawPicture( nTop, nLeft, nBottom, nRight, nSlot, aOffSet )
   endif

   RETURN nil

//-------------------------------------------------------------------//

FUNCTION CreateMainMenu()
   LOCAL oMenu
   LOCAL g_oMenuBar := wvtMenu():new():create()

   oMenu := WvtMenu():new():create()
   oMenu:Caption:= "Wvt*Classes"
   oMenu:AddItem( "Dialog One", {|| MyDialogOne() } )
   oMenu:AddItem( "Dialog Two", {|| MyDialogTwo() } )
   oMenu:AddItem( "-" )
   oMenu:AddItem( "Exit"      , {|| __keyboard( K_ESC ) } )
   g_oMenuBar:addItem( "",oMenu )

   oMenu := wvtMenu():new():create()
   oMenu:Caption := "Traditional"
   oMenu:AddItem( "Next Gets"     , {|| WvtNextGets()      } )
   oMenu:AddItem( "Gets : Console", {|| WvtConsoleGets()   } )
   oMenu:AddItem( "Browser"       , {|| WvtMyBrowse()      } )
   oMenu:AddItem( "Partial Screen", {|| WvtPartialScreen() } )
   oMenu:AddItem( "-")
   oMenu:AddItem( "Wvt Lines"     , {|| WvtLines()         } )
   g_oMenuBar:addItem( "",oMenu )

   oMenu := wvtMenu():new():create()
   oMenu:Caption:= "Common Dialogs"
   oMenu:AddItem( "Fonts" ,{|| Wvt_ChooseFont() } )
   oMenu:AddItem( "-")
   oMenu:AddItem( "Colors",{|| Wvt_ChooseColor() } )
   g_oMenuBar:addItem( "",oMenu)

   oMenu := wvtMenu():new():create()
   oMenu:Caption:= "Functionality"
   oMenu:AddItem( "Expand" ,{|| WvtWindowExpand( 1 ) } )
   oMenu:AddItem( "Shrink" ,{|| WvtWindowExpand( -1 ) } )
   oMenu:AddItem( "-")
   oMenu:AddItem( "Minimize",{|| Wvt_Minimize() } )
   g_oMenuBar:addItem( "",oMenu)

   oMenu := wvtMenu():new():create()
   oMenu:Caption:= "Modeless Dialogs"
   oMenu:AddItem( "Dialog First" ,{|| DynDialog_2() } )
   oMenu:AddItem( "-")
   oMenu:AddItem( "Slide Show"   ,{|| DlgSlideShow() } )
   oMenu:AddItem( "-")
   oMenu:AddItem( "Dialog Scond" ,{|| DynDialog_1() } )
   g_oMenuBar:addItem( "",oMenu)

   oMenu := wvtMenu():new():create()
   oMenu:Caption:= "Active-X Controls"
   oMenu:AddItem( "ActiveX - Analog Clock"     , {|| Hb_ThreadStart( {|| ExecuteActiveX( 2 ) } ) } )
   oMenu:AddItem( "-")
   oMenu:AddItem( "ActiveX - Internet Explorer", {|| Hb_ThreadStart( {|| ExecuteActiveX( 1 ) } ) } )
   oMenu:AddItem( "-")
   oMenu:AddItem( "ActiveX - Visualize a PDF"  , {|| Hb_ThreadStart( {|| ExecuteActiveX( 3 ) } ) } )
   oMenu:AddItem( "-")
   oMenu:AddItem( "ActiveX - Explorer . DHTML" , {|| Hb_ThreadStart( {|| ExecuteActiveX( 11 ) } ) } )
   oMenu:AddItem( "-")
   oMenu:AddItem( "ActiveX - RMChart"          , {|| Hb_ThreadStart( {|| ExecuteActiveX( 4 ) } ) } )
   g_oMenuBar:addItem( "",oMenu)

   RETURN g_oMenuBar

//-------------------------------------------------------------------//

STATIC FUNCTION ActivateMenu( oMenu )
   LOCAL nMenu := Wvt_GetLastMenuEvent()
   LOCAL aMenuItem

   IF !EMPTY( nMenu )

     IF HB_ISOBJECT( oMenu )
       IF !EMPTY( aMenuItem := oMenu:FindMenuItemById( nMenu ) )
         IF HB_ISBLOCK( aMenuItem[ WVT_MENU_ACTION ] )
           EVAL( aMenuItem[ WVT_MENU_ACTION ] )
         ENDIF
       ENDIF
      ENDIF
   ENDIF

   RETURN ( NIL )

//-------------------------------------------------------------------//

STATIC FUNCTION MyDialogOne()
   LOCAL aObjects:= WvtSetBlocks( {} )
   Local nWinRows, nWinCols, cWinTitle, cFont, nHeight, nWidth
   Local oDlg, obj_, oBar, d_, nN, cUseAlias
   Local oText, oTBar, aImg_, oImg, oLine, oBox, oBtn, oBtn2
   Local oBBox, oCon, oGet, oGet2, nRowG, oBBox2, oBnr, oTBx
   Local oBRsd, cTxt, oRct, nGetCol, nSayCol, bBlock, bBlock1
   Local oMnu, oWvtBrw, oWvtBrw1, lOpen, lOpen1, cUseAlias1, oGetArea, oGet1
   LOCAL hPopup, nGetRow, aGets_, lChkMouse
   LOCAL g_oMenuBar, oMenu, oPBar2,oPBar3

   WvtSetKeys( .f. )
   lChkMouse := SetMouseCheck( .f. )

   hPopup := Wvt_SetPopupMenu()
   Popups()

   cTxt := "GtWvt is capable of designing virtually any preceivable control "
   cTxt := cTxt + "Windows offers."
   cTxt := cTxt + CRLF + CRLF
   cTxt := cTxt + "This text is placed in a WvtTextBox() control with "
   cTxt := cTxt + "font and alignment adjustments!"
   cTxt := cTxt + CRLF + CRLF
   cTxt := cTxt + "Enjoy - Pritpal Bedi, INDIA"

   aImg_:={}
   aadd( aImg_, "v_lock.bmp"   )
   aadd( aImg_, "v_new.bmp"    )
   aadd( aImg_, "v_clclt.bmp"  )
   aadd( aImg_, "v_calend.bmp" )
   aadd( aImg_, "v_index.bmp"  )
   aadd( aImg_, "v_notes1.bmp" )
   aadd( aImg_, "v_selct1.bmp" )

   nWinRows  := 55
   nWinCols  := 185
   cWinTitle := "WvtGui Dialog One"
   cFont     := "Courier New"
   nHeight   := 13

   oDlg := WvtDialog():New( nWinRows, nWinCols, cWinTitle, cFont, nHeight )
   oDlg:nTooltipWidth     := 300
   oDlg:nTooltipTextColor := RGB( 255,0,0 )

   oBar := WvtStatusBar():New( oDlg,201 )
   oBar:SetPanels( { 50,100 } )
   oBar:SetText( 1, "Tab.SH_Tab.Left_Click - Select a Browse" )
   oBar:SetText( 2, "GtWvt is Fantastic", "w+/W" )
   oBar:SetText( 3, "WOW" )
   oBar:nPointer := WVT_IDC_HAND
   oBar:Tooltip  := "GtWvt Statusbar with 3 panels"
   oDlg:AddObject( oBar )

   oBox := WvtStatic():New( oDlg,110,4,oDlg:MaxCol()-40,7,oDlg:MaxCol()-2 )
   oBox:nStatic := WVT_STATIC_BOXRECESSED
   oDlg:AddObject( oBox )

   oText := WvtLabel():New( oDlg, 101, 4, oDlg:MaxCol()-40, 7,oDlg:MaxCol()-2 )
   oText:Text              := "(x)Harbour"
   oText:nFontHeight       := 36
   oText:nAlignHorz        := 2
   oText:nAlignVert        := 2
   oText:nFontWeight       := 700
   oText:nTextColor        := RGB( 100, 255,  12 )
   oText:nBackColor        := RGB(   0,   0, 255 )
   oText:nTextColorHoverOn := RGB( 255, 255,   0 )
   oText:nBackColorHoverOn := RGB( 255, 100,  12 )
   oText:lItalic           := .t.
   oText:ToolTip           := "Software that GROWS with you"
   oText:bOnSelect         := {|o,v| .t. }
   oDlg:AddObject( oText )

   oImg := WvtImage():New( oDlg,102,20,oDlg:MaxCol()-40,37,oDlg:MaxCol()-2 )
   oImg:cImage  := aImg_[ 5 ]
   oImg:Tooltip := "WvtImage():New()"
   oDlg:AddObject( oImg )

   oTBar := WvtToolbar():New( oDlg,103, 0,0,2 )
   oTBar:lFloating := .f.
   oTBar:Tooltip   := "Toolbar"
   oTBar:AddButton( aImg_[ 1 ], {|| oImg:SetImage( aImg_[ 1 ] ) } , "Lock" )
   oTBar:AddButton( aImg_[ 2 ], {|| oImg:SetImage( aImg_[ 2 ] ), oText:SetText( "(x)Harbour" ) } , "New" )
   oTBar:AddButton( aImg_[ 3 ], {|| oImg:SetImage( aImg_[ 3 ] ) } , "Calculator" )
   oTBar:AddButton()
   oTBar:AddButton( aImg_[ 5 ], {|| oImg:SetImage( aImg_[ 5 ] ) } , "Restore" )
   oTBar:AddButton( aImg_[ 4 ], {|| oImg:SetImage( aImg_[ 4 ] ), oText:SetText( "Vouch" )    } , "Calendar" )
   oTBar:AddButton( aImg_[ 6 ], {|| oImg:SetImage( aImg_[ 6 ] ) } , "Notes" )
   oTBar:AddButton( aImg_[ 7 ], {|| oImg:SetImage( aImg_[ 7 ] ) } , "Press to Send Browse on Top" )
   oTBar:AddButton()
   oDlg:AddObject( oTBar )

   oLine := WvtStatic():New( oDlg, 105, 39, 0, 39, oDlg:MaxCol() )
   oLine:nStatic := WVT_STATIC_LINE
   oDlg:AddObject( oLine )

   oBBox := WvtStatic():New( oDlg,125,4,127,37,139 )
   oBBox:nStatic := WVT_STATIC_BOXGROUP
   oDlg:AddObject( oBBox )

   oBtn := WvtPushButton():New(oDlg, 124, 6, 129, 7, 137 )
   oBtn:cCaption  := "Print"
   oBtn:bOnLeftUp := {|| Wvt_Keyboard( 379 ) }
   oBtn:Tooltip   := "Open Printing Dialog for the Browser in Focus"
   oDlg:AddObject( oBtn )

   oBtn2 := WvtPushButton():New( oDlg, 124, 9, 129, 12, 137 )
   oBtn2:cFileImage := aImg_[ 3 ]
   oBtn2:block      := {|| ExeProgressBar( oPBar2, oPBar3 ) }
   oBtn2:Tooltip    := "Execute Progress Bar"
   oDlg:AddObject( oBtn2 )

   oPBar2 := WvtProgressBar():New( oDlg, , 14, 129, 25, 137 )
   oPBar2:nBarColor  := RGB( 240,240,0 )
   oPBar2:cBackColor := "W/N*"
   oPBar2:lVertical  := .t.
   oPBar2:nDirection := 0
   oPBar2:cImage     := "vouch1.bmp"
   oDlg:AddObject( oPBar2 )

   oPBar3 := WvtProgressBar():New( oDlg, , 26, 129, 36, 137 )
   oPBar3:nBarColor  := RGB( 240,240,0 )
   oPBar3:cBackColor := "W/N*"
   oPBar3:lVertical  := .t.
   oPBar3:nDirection := 1
   oPBar3:cImage     := "vouch1.bmp"
   oDlg:AddObject( oPBar3 )

   oBBox2 := WvtStatic():New( oDlg, , 9, oDlg:MaxCol()-40, 18, oDlg:Maxcol()-2 )
   oBBox2:nStatic := WVT_STATIC_BOXGROUP
   oDlg:AddObject( oBBox2 )

   oCon := WvtConsole():New( oDlg )
   oDlg:AddObject( oCon )

   nGetCol := 158
   bBlock  := {|| oCon:Say( 12, 148, "Name"  ,"N/W" ),;
                  oCon:Say( 14, 148, "Date"  ,"N/W" ),;
                  oCon:Say( 16, 148, "Amount","N/W" ) }

   oGet := WvtGets():New( oDlg, 210, 9, oDlg:Maxcol()-40, 18, oDlg:Maxcol()-2 )
   oGet:AddGets( 12, nGetCol, "GTWvt               ", "@! ","W+/B*,N/W*" )
   oGet:AddGets( 14, nGetCol, date() )
   oGet:AddGets( 16, nGetCol, 2122.57, "@Z 99999999.99","w+/R,GR+/B" )
   oGet:Tooltip   := "WvtGets():New() - ReadModal() like Clipper"
   oGet:cDesc     := "Normal Get Box"
   oGet:bOnCreate := bBlock
   oDlg:AddObject( oGet )

   oBnr := WvtBanner():New( oDlg, 101, 0, 127, 1, oDlg:MaxCol()-2 )
   oBnr:nTimeDelay        := 0.25
   oBnr:cText             := "the compiler that EXTENDS with you"
   oBnr:nFontHeight       := 24
   oBnr:nFontWeight       := 0
   oBnr:nDirection        := 0
   oBnr:nAlignVert        := 2
   oBnr:nTextColor        := RGB( 253,251,170 )
   oBnr:nBackColor        := RGB( 128,227,142 )
   oBnr:nTextColorHoverOn := RGB( 255,255,  0 )
   oBnr:nBackColorHoverOn := RGB( 255,100, 12 )
   oBnr:Tooltip           := "WvtBanner():New()"
   oDlg:AddObject( oBnr )

   oBRsd := WvtStatic():New( oDlg, , 41,127,52,oDlg:MaxCol()-2 )
   oBRsd:nStatic := WVT_STATIC_BOXGROUPRAISED
   oDlg:AddObject( oBRsd )

   oRct := WvtStatic():New( oDlg, , 41,127,52,oDlg:MaxCol()-2 )
   oRct:nStatic := WVT_STATIC_SHADEDRECT
   oRct:aRGBb   := { 0xffff, 0x0000, 0x0000, 0x0000 }
   oRct:aRGBe   := { 0x0000, 0xffff, 0xffff, 0x0000 }
   oDlg:AddObject( oRct )

   oTBx := WvtTextBox():New( oDlg, , 42,129,51,oDlg:MaxCol()-4 )
   oTBx:cText       := cTxt
   oTBx:Tooltip     := "WvtTextBox():New()"
   oTBx:nFontHeight := 16
   oTBx:lItalic     := .t.
   oTBx:lUnderline  := .t.
   oTBx:nAlignHorz  := 2
   oTBx:nTextColor  := RGB( 255,255,255 )
   oTBx:nTextColorHoverOn := RGB( 0,0,255 )
   oTBx:aPopup      := {}
   aadd( oTBx:aPopup, { "Getsome" , {|| .t. } } )
   aadd( oTBx:aPopup, { "Getsome2", {|| .t. } } )
   oDlg:AddObject( oTBx )

   oGetArea := WvtStatic():New( oDlg, , 4, 2, 37, 62 )
   oGetArea:nStatic := WVT_STATIC_BOXRAISED
   oDlg:AddObject( oGetArea )

   nGetCol := 20
   nSayCol := 7
   nGetRow := 7
   bBlock1 := {|| oCon:Say( nGetRow+00, nSayCol, "First Name"  ,"N/W" ),;
                  oCon:Say( nGetRow+02, nSayCol, "Last Name "  ,"N/W" ),;
                  oCon:Say( nGetRow+04, nSayCol, "Street"      ,"N/W" ),;
                  oCon:Say( nGetRow+06, nSayCol, "City"        ,"W+/W"),;
                  oCon:Say( nGetRow+08, nSayCol, "State"       ,"N/W" ),;
                  oCon:Say( nGetRow+10, nSayCol, "Zip"         ,"B+/W"),;
                  oCon:Say( nGetRow+12, nSayCol, "Date Hired"  ,"B+/W"),;
                  oCon:Say( nGetRow+14, nSayCol, "Married"     ,"B+/W"),;
                  oCon:Say( nGetRow+16, nSayCol, "Age"         ,"B+/W"),;
                  oCon:Say( nGetRow+18, nSayCol, "Salary"      ,"B+/W"),;
                  oCon:Say( nGetRow+20, nSayCol, "Notes",      ,"B+/W") ;
                  }

   aGets_:= { pad("Pritpal",20 ), pad( "Bedi",20 ), pad( "60, New Professor Colony",30 ), ;
              pad( "Ludhiana, INDIA",30 ),;
              "PB", pad( "141004",10 ), ctod( "22/06/04" ), .t., 48, 17000, ;
              pad( "Wvtgui is a classical example of (x)Harbour capabilities...",65 ) }

   oGet1 := WvtGets():New( oDlg, , 4, 2, 37, 62 )
   oGet1:AddGets( nGetRow+00, nGetCol, aGets_[ 1 ], "@ "       , "N/W*,N/GR*" )
   oGet1:AddGets( nGetRow+02, nGetCol, aGets_[ 2 ], "@ "       , "N/W*,N/GR*" )
   oGet1:AddGets( nGetRow+04, nGetCol, aGets_[ 3 ], "@ "       , "N/W*,N/GR*" )
   oGet1:AddGets( nGetRow+06, nGetCol, aGets_[ 4 ], "@ "       , "N/W*,N/GR*" )
   oGet1:AddGets( nGetRow+08, nGetCol, aGets_[ 5 ], "@ "       , "N/W*,N/GR*" )
   oGet1:AddGets( nGetRow+10, nGetCol, aGets_[ 6 ], "@ "       , "N/W*,N/GR*" )
   oGet1:AddGets( nGetRow+12, nGetCol, aGets_[ 7 ], "@ "       , "N/W*,N/GR*" )
   oGet1:AddGets( nGetRow+14, nGetCol, aGets_[ 8 ], "@Y"       , "N/W*,N/GR*" )
   oGet1:AddGets( nGetRow+16, nGetCol, aGets_[ 9 ], "@Z 99"    , "N/W*,N/GR*" )
   oGet1:AddGets( nGetRow+18, nGetCol, aGets_[ 10], "@Z 999999", "N/W*,N/GR*" )
   oGet1:AddGets( nGetRow+20, nGetCol, aGets_[ 11], "@S35"     , "N/W*,N/GR*" )
   oGet1:cDesc     := "test.dbf Fields"
   oGet1:Tooltip   := "Double Click to Activate ReadModal()"
   oGet1:bOnCreate := bBlock1
   oDlg:AddObject( oGet1 )

   g_oMenuBar := WvtMenu():new():create()
   oMenu      := WvtMenu():new():create()
   oMenu:Caption := "Other Dialogs"
   oMenu:AddItem( "Dialog Two", {|| MyDialogTwo() } )
   oMenu:AddItem( "-" )
   oMenu:AddItem( "Exit",       {|| Wvt_Keyboard( K_ESC ) } )
   g_oMenuBar:addItem( "",oMenu )

   oDlg:oMenu := g_oMenuBar

   lOpen := .f.
   cUseAlias := "TEST"
   USE "test" NEW ALIAS ( cUseAlias ) SHARED
   if !NetErr()
      lOpen := .t.
      oWvtBrw := CfgMyBrowse( { 1,7,9,10,8 }, cUseAlias, { 6,67,36,120 }, "test.dbf - 1,7,9,10,8", oDlg, "N/W*,N/GR*",1001 )
      oDlg:AddObject( oWvtBrw )
   endif

   lOpen1 := .f.
   cUseAlias1 := "TEST1"
   USE "test" NEW ALIAS ( cUseAlias1 ) SHARED
   if !NetErr()
      lOpen1 := .t.
      oWvtBrw1 := CfgMyBrowse( { 1,2,3,4,5,6 }, cUseAlias1, { 43,4,51,120 }, "test.dbf - 1,2,3,4,5,6",oDlg, "N/BG*,N/W*",1002 )
      oDlg:AddObject( oWvtBrw1 )
   endif

   Setkey( K_F12, {|| hb_gtInfo( HB_GTI_SPEC, HB_GTS_FACTOR, 200 ) } )

   oDlg:Create()
   oDlg:Execute()
   oDlg:Destroy()

   if lOpen
      Select( cUseAlias )
      USE
   endif
   if lOpen1
      Select( cUseAlias1 )
      USE
   endif

   WvtSetBlocks( aObjects )
   WvtSetKeys( .t. )
   Wvt_SetPopupMenu( hPopup )
   SetMouseCheck( lChkMouse )

   RETURN Nil

//-------------------------------------------------------------------//

STATIC FUNCTION MyDialogTwo()
   LOCAL aObjects := WvtSetBlocks( {} )
   LOCAL oDlg     := WvtDialog():New( 30,90,"My Dialog Two" )
   LOCAL g_oMenuBar, oMenu, oPBar
   LOCAL oPBar1, oPBar2, oPBar3, oPBar4

   g_oMenuBar    := WvtMenu():new():create()
   oMenu         := wvtMenu():new():create()
   oMenu:Caption := "Miscellaneous"
   oMenu:AddItem( "Progressbar", {|| ExeProgBar( oPBar,oPBar1,oPBar2,oPBar3, oPBar4 ) } )
   oMenu:AddItem( "-" )
   oMenu:AddItem( "Exit",        {|| Wvt_Keyboard( K_ESC ) } )
   g_oMenuBar:addItem( "",oMenu )

   oDlg:oMenu := g_oMenuBar

   oPBar := WvtProgressBar():New( oDlg, , 3, 10, 5, 80 )
   oPBar:nBarColor   := RGB( 0,240,240 )
   oPBar:cBackColor  := "W/N*"
   oPBar:nDirection  := 1
   oPBar:cImage      := "vouch1.bmp"
   oDlg:AddObject( oPBar )

   oPBar1 := WvtProgressBar():New( oDlg, , 7, 10, 8, 80 )
   oPBar1:nBarColor  := RGB( 11,255,196 )
   oPBar1:cBackColor := "W/N*"
   oPBar1:nDirection := 0
   oDlg:AddObject( oPBar1 )

   oPBar2 := WvtProgressBar():New( oDlg, , 11, 10, 28, 19 )
   oPBar2:nBarColor  := RGB( 240,240,0 )
   oPBar2:cBackColor := "W/N*"
   oPBar2:lVertical  := .t.
   oPBar2:nDirection := 0
   oPBar2:cImage     := "v_notes.ico"
   oDlg:AddObject( oPBar2 )

   oPBar3 := WvtProgressBar():New( oDlg, , 11, 77, 28, 80 )
   oPBar3:nBarColor  := RGB( 0,0,255 )
   oPBar3:cBackColor := "W/N*"
   oPBar3:lVertical  := .t.
   oPBar3:nDirection := 1
   oDlg:AddObject( oPBar3 )

   oPBar4 := WvtProgressBar():New( oDlg, , 22, 22, 28, 74 )
   oPBar4:nBarColor  := RGB( 255,255,0 )
   oPBar4:cBackColor := "W/N*"
   oPBar4:lVertical  := .t.
   oPBar4:nDirection := 0
   oDlg:AddObject( oPBar4 )

   oDlg:Create()
   oDlg:Execute()
   oDlg:Destroy()

   WvtSetBlocks( aObjects )
   RETURN Nil

//-------------------------------------------------------------------//

STATIC FUNCTION ExeProgBar( oPBar,oPBar1,oPBar2,oPBar3,oPBar4 )
   LOCAL i

   oPBar:Activate()
   oPBar1:Activate()
   oPBar2:Activate()
   oPBar3:Activate()
   oPBar4:Activate()
   for i := 1 to 100
      oPBar:Display( i, 100 )
      oPBar1:Display( i, 100 )
      oPBar2:Display( i, 100 )
      oPBar3:Display( i, 100 )
      oPBar4:Display( i, 100 )
      inkey( 0.3 )
   next
   inkey( 0 )
   oPBar:DeActivate()
   oPBar1:DeActivate()
   oPBar2:DeActivate()
   oPBar3:DeActivate()
   oPBar4:DeActivate()

   RETURN nil
//-------------------------------------------------------------------//

STATIC FUNCTION ExeProgressBar( oPBar, oPBar3 )
   LOCAL i

   oPBar:Activate()
   oPBar3:Activate()
   for i := 1 to 100
      oPBar:Display( i, 100 )
      oPBar3:Display( i, 100 )
      inkey( 0.3 )
   next
   oPBar:DeActivate()
   oPBar3:DeActivate()

   RETURN NIL

//-------------------------------------------------------------------//

Function DynDialog_1()
   Local hDlg, aDlg, nStyle

   Static nInfo := 1
   nInfo++

   nStyle := + WS_CAPTION    + WS_SYSMENU               ;
             + WS_GROUP      + WS_TABSTOP + DS_SETFONT  ;
             + WS_THICKFRAME + WS_VISIBLE + WS_POPUP

   aDlg := Wvt_MakeDlgTemplate( 1, 2, 15, 40, {0,0,0,0},  ;
                     ltrim( str( nInfo,10,0 ) ) + " - Modeless Dialog", nStyle )

   nStyle := WS_VISIBLE + WS_TABSTOP + ES_AUTOVSCROLL + ES_MULTILINE + ES_WANTRETURN + WS_BORDER + WS_VSCROLL
   aDlg   := Wvt_AddDlgItem( aDlg,  1, 2, 9, 28, {}, 10, "EDIT"  , nStyle, /* cText, nHelpId, nExStyle */ )

   nStyle := WS_VISIBLE + SS_ETCHEDHORZ
   aDlg   := Wvt_AddDlgItem( aDlg, 12, 2, 1, 36, {}, 12, "STATIC", nStyle )

   nStyle := WS_VISIBLE + WS_TABSTOP + BS_AUTOCHECKBOX
   aDlg   := Wvt_AddDlgItem( aDlg, 13, 2, 1, 10, {}, 11, "BUTTON", nStyle, "Is It Checked?" )

   hDlg := Wvt_CreateDialog( aDlg, .f., "DynDlgProc_1" )

   Return hDlg

//-------------------------------------------------------------------//

Function DynDlgProc_1( hDlg, nMsg, wParam, lParam )
   Local cText, lClicked

   Switch ( nMsg )

   case WM_INITDIALOG
      Win_SetDlgItemText( hDlg, 10, "This is multiline text which will be displayed in the edit window!" )
      Win_CheckDlgButton( hDlg, 11, .t. )
      exit

   case WM_DESTROY
      // Do whatevert you want to do with cText
      // Each box will retrieve its own text.
      //
      cText := Win_GetDlgItemText( hDlg, 10 )

      exit

   case WM_TIMER
      // Do some processing

      exit

   case WM_COMMAND
      do case

      case wParam == 11
         lClicked := ( Win_IsDlgButtonChecked( hDlg,11 ) == 1 )
         Win_MessageBox( hDlg, "Button " + iif( lClicked, "Clicked", "Unclicked" ), "CheckBoxStatus" )

      endcase
      exit

   end

   Return .f.

//-------------------------------------------------------------------//

#define ID_BTN_OK          1
#define ID_MLE            10
#define ID_CHK_SATIS      11
#define ID_EDT_TIME       51
#define ID_LST_LIST       13
#define ID_CMB_COMBO      31
#define ID_RDO_XH         21
#define ID_RDO_CLIP       22
#define ID_RDO_XBASE      23
#define ID_EDT_TEXT       14
#define ID_EDT_NUMB       15
#define ID_STA_TEXT       71
#define ID_STA_IMAGE      72
#define ID_ICO_VOUCH      81

#define ID_GRP_COMP      113

#define ID_MNU_FILE      201
#define ID_MNU_CONTROL   202

//-------------------------------------------------------------------//

Function DynDialog_2()
   Local hDlg, aDlg, nStyle, nTimerTicks, cDlgIcon, cDlgProc, lOnTop, hMenu, nProc, bDlgProc

   Static nInfo := 0
   nInfo++

   nStyle := DS_SETFONT + WS_VISIBLE + WS_POPUP + WS_CAPTION + WS_SYSMENU + WS_THICKFRAME + WS_MINIMIZEBOX

   aDlg := Wvt_MakeDlgTemplate( 1, 4, 21, 60, {0,0,0,0},  ;
               "Dialog First [ " + ltrim( str( nInfo,10,0 ) ) + " ] "+;
                        iif( nInfo%2==0, "Modeless", "Modal" )," Dialog !", nStyle )

   // Multi line edit control
   //
   nStyle := WS_CHILD + WS_VISIBLE + WS_TABSTOP + ES_AUTOVSCROLL + ES_MULTILINE + ;
             ES_WANTRETURN + WS_BORDER  + WS_VSCROLL
   aDlg   := Wvt_AddDlgItem( aDlg,  1, 2, 15, 35, {}, ID_MLE       , "EDIT"   , nStyle, /* cText, nHelpId, nExStyle */ )

   // Two Horz and Vert Lines
   //
   nStyle := WS_CHILD + WS_VISIBLE + SS_ETCHEDVERT
   aDlg   := Wvt_AddDlgItem( aDlg, 1, 39,  16, 1, {}, 111          , "STATIC" , nStyle )
   nStyle := WS_CHILD + WS_VISIBLE + SS_ETCHEDHORZ
   aDlg   := Wvt_AddDlgItem( aDlg, 17, 2,  1, 56, {}, 112          , "STATIC" , nStyle )

   // Icon
   nStyle := WS_CHILD + WS_VISIBLE + SS_ICON //+ SS_CENTERIMAGE
   aDlg   := Wvt_AddDlgItem( aDlg, 18, 2, 2, 6, {}, ID_ICO_VOUCH  , "STATIC" , nStyle, "" )
/*
   // Bitmap
   nStyle := WS_CHILD + WS_VISIBLE + SS_BITMAP + SS_REALSIZEIMAGE
   aDlg   := Wvt_AddDlgItem( aDlg, 18, 41, 2,8, {-3,0,3}, ID_STA_IMAGE, "STATIC" , nStyle, "" )
*/
   nStyle := WS_CHILD + WS_VISIBLE + WS_TABSTOP + BS_AUTOCHECKBOX
   aDlg   := Wvt_AddDlgItem( aDlg, 18, 15,  1, 10, {}, ID_CHK_SATIS , "BUTTON" , nStyle, "Satisfied?" )

   nStyle := WS_CHILD + WS_VISIBLE + WS_TABSTOP + ES_RIGHT + ES_READONLY
   aDlg   := Wvt_AddDlgItem( aDlg, 18, 30, 1,  7, {3}, ID_EDT_TIME , "EDIT" , nStyle, "" )

   nStyle := WS_CHILD + WS_VISIBLE + WS_TABSTOP + LBS_NOTIFY + WS_VSCROLL + WS_BORDER
   aDlg   := Wvt_AddDlgItem( aDlg, 1, 41,  4, 17, {}, ID_LST_LIST  , "LISTBOX", nStyle, "ListBox"  )

   nStyle := WS_CHILD + WS_VISIBLE + SS_LEFT
   aDlg   := Wvt_AddDlgItem( aDlg, 4, 41,  1, 17, {3,0,0,0}, -1    , "STATIC" , nStyle, "Degree"     )
   nStyle := WS_VISIBLE + WS_TABSTOP + CBS_DROPDOWNLIST + WS_BORDER + WS_VSCROLL
   aDlg   := Wvt_AddDlgItem( aDlg, 5, 41,  6, 17, {}, ID_CMB_COMBO , "COMBOBOX" , nStyle, "Combo" )

   nStyle := WS_CHILD + WS_VISIBLE + WS_TABSTOP + BS_GROUPBOX
   aDlg   := Wvt_AddDlgItem( aDlg, 7, 41,  4, 17, {0,0,4,0},ID_GRP_COMP, "BUTTON" , nStyle, "Compiler" )
   nStyle := WS_CHILD + WS_VISIBLE + WS_TABSTOP + BS_AUTORADIOBUTTON
   aDlg   := Wvt_AddDlgItem( aDlg, 8, 43,  1, 14, {}, ID_RDO_XH    , "BUTTON" , nStyle, "(x)Harbour" )
   aDlg   := Wvt_AddDlgItem( aDlg, 9, 43,  1, 14, {}, ID_RDO_CLIP  , "BUTTON" , nStyle, "Clipper"  )
   aDlg   := Wvt_AddDlgItem( aDlg,10, 43,  1, 14, {}, ID_RDO_XBASE , "BUTTON" , nStyle, "Xbase++"  )

   nStyle := WS_CHILD + WS_VISIBLE + SS_LEFT
   aDlg   := Wvt_AddDlgItem( aDlg, 12, 41, 1, 17, {3,0,0,0}, ID_STA_TEXT, "STATIC" , nStyle, "Scrollable Text"    )
   nStyle := WS_CHILD + WS_VISIBLE + WS_TABSTOP + ES_AUTOHSCROLL + WS_BORDER
   aDlg   := Wvt_AddDlgItem( aDlg, 13, 41, 1, 17, {}, ID_EDT_TEXT  , "EDIT"   , nStyle, "This is Text Field" )

   nStyle := WS_CHILD + WS_VISIBLE + SS_LEFT
   aDlg   := Wvt_AddDlgItem( aDlg, 14, 41, 1, 17, {3,0,0,0}, -1, "STATIC" , nStyle, "Right Justified Numerics" )
   nStyle := WS_CHILD + WS_VISIBLE + WS_TABSTOP + ES_AUTOHSCROLL + ES_NUMBER + ES_RIGHT + WS_BORDER
   aDlg   := Wvt_AddDlgItem( aDlg, 15, 41, 1, 17, {}, ID_EDT_NUMB  , "EDIT"   , nStyle, "1234567" )

   nStyle := WS_CHILD + WS_VISIBLE + WS_TABSTOP + BS_PUSHBUTTON
   aDlg   := Wvt_AddDlgItem( aDlg, 18, 50, 1,  8, {-3,0,3,0}, ID_BTN_OK, "BUTTON" , nStyle, "OK" )

   hMenu  := Wvt_CreateMenu()
   Wvt_AppendMenu( hMenu, MF_STRING + MF_ENABLED, ID_MNU_FILE   , "File"     )
   Wvt_AppendMenu( hMenu, MF_STRING + MF_ENABLED, ID_MNU_CONTROL, "Controls" )

   lOnTop      := .f.
   cDlgProc    := "DynDlgProc_2"
   bDlgProc    := {|a,b,c,d| DYNDLGPROC_2(a,b,c,d) }
   cDlgIcon    := "v_notes.ico"
   nTimerTicks := 1000  // 1 second

   if nInfo % 2 == 1
      // Modal Dialog
      //
      //hDlg := Wvt_DialogBox( aDlg, bDlgProc, Wvt_GetWindowHandle() )
      hDlg := Wvt_DialogBox( aDlg, cDlgProc, Wvt_GetWindowHandle() )
   else
      // Modeless Dialog
      //
      hDlg := Wvt_CreateDialog( aDlg, lOnTop, cDlgProc, cDlgIcon, /*nTimerTicks*/, hMenu )

      // Using Function name.
      //hDlg  := Wvt_CreateDialog( aDlg, lOnTop, cDlgProc, cDlgIcon, nTimerTicks, hMenu, lModal )
   endif

   Return hDlg

//-------------------------------------------------------------------//

Function DynDlgProc_2( hDlg, nMsg, wParam, lParam )
   Local cText, lClicked, cPrompt, nIndex, hFont

   Switch ( nMsg )

   case WM_TIMER
      Win_SetDlgItemText( hDlg, ID_EDT_TIME, Time() )
      exit

   case WM_COMMAND
      do case

      case wParam == ID_CHK_SATIS
         lClicked := ( Win_IsDlgButtonChecked( hDlg,ID_CHK_SATIS ) == 1 )
         Win_MessageBox( hDlg, iif( lClicked, "Satisfied", "UnSatisfied" ), "CheckBoxStatus" )

      case wParam == ID_RDO_XH
         Win_MessageBox( hDlg, "(x)Harbour", "Compiler" )

      case wParam == ID_RDO_CLIP
         Win_MessageBox( hDlg, "Clipper", "Compiler" )

      case wParam == ID_RDO_XBASE
         Win_MessageBox( hDlg, "Xbase++", "Compiler" )

      case wParam == ID_MNU_FILE
         Win_MessageBox( hDlg, "Execute Menu Action!", "File" )

      case wParam == ID_MNU_CONTROL
         Win_MessageBox( hDlg, "Controls are from Windows!", "Controls" )

      case Win_LoWord( wParam ) == ID_LST_LIST
         if Win_HiWord( wParam ) == LBN_SELCHANGE
            nIndex  := Win_SendMessage( Win_GetDlgItem( hDlg, ID_LST_LIST ), LB_GETCURSEL, 0, 0 )
            cPrompt := space( 20 )
            Win_SendMessage( Win_GetDlgItem( hDlg, ID_LST_LIST ), LB_GETTEXT, nIndex, @cPrompt )
            Win_MessageBox( hDlg, cPrompt, "ListBox" )
         endif

      case Win_LoWord( wParam ) == ID_CMB_COMBO
         if Win_HiWord( wParam ) == CBN_SELCHANGE
            nIndex  := Win_SendMessage( Win_GetDlgItem( hDlg, ID_CMB_COMBO ), CB_GETCURSEL, 0, 0 )
            cPrompt := space( 20 )
            Win_SendMessage( Win_GetDlgItem( hDlg, ID_CMB_COMBO ), CB_GETLBTEXT, nIndex, @cPrompt )
            Win_MessageBox( hDlg, cPrompt, "Combo Box" )
         endif

      endcase
      exit

   case WM_CTLCOLOREDIT
      if ( Win_GetDlgItem( hDlg,ID_MLE ) == lParam )
         Win_SetTextColor( wParam, RGB( 0,0,255 ) )
         Win_SetBkColor( wParam, RGB( 255,255,200 ) )
         return ( 1 )
      elseif ( Win_GetDlgItem( hDlg,ID_EDT_TEXT ) == lParam )
         Win_SetTextColor( wParam, RGB( 255,255,255 ) )
         Win_SetBkColor( wParam, RGB( 10,200,45 ) )
         Return ( 1 )
      endif

      exit

   case WM_CTLCOLORSTATIC
      if ( Win_GetDlgItem( hDlg,ID_STA_TEXT ) == lParam )
         Win_SetTextColor( wParam, RGB( 255,255,255 ) )
         Return ( 1 )
      endif
      exit

   case WM_INITDIALOG
      Win_SetTimer( hDlg, 5001, 1000 ) // 1 sec

      if empty( ahFonts )
         if ( hFont := Wvt_CreateFont( "Times New Roman", 18 ) ) != 0
            aadd( ahFonts, hFont )
         endif
      endif

      if len( ahFonts ) > 0
         Win_SendMessage( Win_GetDlgItem( hDlg, ID_MLE ), WM_SETFONT, ahFonts[ 1 ], 0 )
      endif

      if shIcon == nil
         shIcon := Win_LoadIcon( "vr_1.ico" )
      endif
      if shIcon != nil .or. shIcon != 0
         Win_SendMessage( Win_GetDlgItem( hDlg, ID_ICO_VOUCH ), STM_SETIMAGE, IMAGE_ICON, shIcon )
      endif

      /*
      if shImage == nil
         shImage := Win_LoadImage( "vouch1.bmp", 2 )
      endif
      if shImage != nil .and. shImage != 0
         Win_SendMessage( Win_GetDlgItem( hDlg, ID_STA_IMAGE ), STM_SETIMAGE, IMAGE_BITMAP, shImage )
      endif
      */
      Win_SetDlgItemText( hDlg, ID_MLE      , GetEditText() )
      Win_CheckDlgButton( hDlg, ID_CHK_SATIS, .t.           )

      Win_CheckRadioButton( hDlg, ID_RDO_XH, ID_RDO_XBASE, ID_RDO_XH )

      Wvt_LBAddString( hDlg, ID_LST_LIST, "(x)Harbour"  )
      Wvt_LBAddString( hDlg, ID_LST_LIST, "Gtwvt"     )
      Wvt_LBAddString( hDlg, ID_LST_LIST, "Wvtgui"    )
      Wvt_LBAddString( hDlg, ID_LST_LIST, "Modeless"  )
      Wvt_LBAddString( hDlg, ID_LST_LIST, "Dialogs"   )
      Wvt_LBAddString( hDlg, ID_LST_LIST, "WVT"       )

      Wvt_LBSetCurSel( hDlg, ID_LST_LIST, 1 )

      Wvt_CBAddString( hDlg, ID_CMB_COMBO, "First"    )
      Wvt_CBAddString( hDlg, ID_CMB_COMBO, "Second"   )
      Wvt_CBAddString( hDlg, ID_CMB_COMBO, "Third"    )
      Wvt_CBAddString( hDlg, ID_CMB_COMBO, "Fourth"   )
      Wvt_CBAddString( hDlg, ID_CMB_COMBO, "Fifth"    )

      Wvt_CBSetCurSel( hDlg, ID_CMB_COMBO, 1 )

      Win_InvalidateRect( hDlg )

      exit

   case WM_DESTROY
      // Do whatevert you want to do with cText
      // Each box will retrieve its own text.
      //
      cText := Win_GetDlgItemText( hDlg, ID_MLE )
      cText := nil
      exit

   end

   Return ( 0 )

//-------------------------------------------------------------------//

static function GetEditText()
   Local cText := ""

   cText += "Welcome in the Wonderful World of (x)Harbour!"
   cText += CRLF + CRLF
   cText += "When Peter Rees first published GTWVT, a Windows "
   cText += "Terminal Driver, on 22 Dec 2003, everybody took it "
   cText += "lightly, except for me, as I was aware that what "
   cText += "wonderful contribution to (x)Harbour he has made, "
   cText += "what immense possibilities he has opened for (x)Harbour "
   cText += "developers, what limitations he has cleared for Clipper "
   cText += "savvy user base."
   cText += CRLF + CRLF
   cText += "With a little effort I could extend GTWVT "
   cText += "to give it a GUI look. I also tried to give it "
   cText += "an event driven functionality, and up came Wvt*Classes."
   cText += CRLF + CRLF
   cText += "And yet another feather is added in the cap of GTWVT "
   cText += "as it is now capable of firing modeless dialogs like the one "
   cText += "you are viewing. These dialogs can be constructed dynamically ( Courtesy hbwhat32 ) "
   cText += "at run time or can be one of resources. At present 20 such dialogs "
   cText += "can be active at any given time. Also note that dialogs created "
   cText += "dynamically respect Top, Left, Rows, Cols coordinates, which is an "
   cText += "undisputed productivity boost!"
   cText += CRLF + CRLF
   cText += "Enjoy!" + CRLF
   cText += "Pritpal Bedi, INDIA"

   Return cText

//-------------------------------------------------------------------//

EXIT PROCEDURE CleanHandles()
   LOCAL i

   for i := 1 to len( ahFonts )
      Win_DeleteObject( ahFonts[ i ] )
   next

   if shIcon != nil
      Win_DeleteObject( shIcon )
   endif

   if shImage != nil
      Win_DeleteObject( shImage )
   endif

   Return

//-------------------------------------------------------------------//

FUNCTION DlgSlideShow()
   LOCAL hDlg, aDlg, nStyle

   aSlides := { "vouch1.bmp", "v_notes.ico", "2000.gif", "v_lock.bmp", "v_help.ico" }

   nStyle  := DS_SETFONT + WS_VISIBLE + WS_POPUP + WS_CAPTION + WS_SYSMENU + WS_THICKFRAME + WS_MINIMIZEBOX

   aDlg    := Wvt_MakeDlgTemplate( 0, 0, 20, 40, {}, "Slide Show", nStyle )

   hDlg    := Wvt_CreateDialog( aDlg, .f., "DlgSlideShowProc", "vr_1.ico", 5000 )

   Return hDlg

//-------------------------------------------------------------------//

FUNCTION DlgSlideShowProc( hDlg, nMsg, wParam, lParam )
   LOCAL  aRect, hDC
   THREAD STATIC nSlide := 1

   Switch nMsg

   case WM_INITDIALOG
      DrawSlide( hDlg, nSlide )
      exit

   case WM_PAINT
      DrawSlide( hDlg, nSlide )
      exit

   case WM_TIMER
      nSlide++
      if nSlide > len( aSlides )
         nSlide := 1
      endif
      DrawSlide( hDlg, nSlide )

      exit

   end

   Return ( 0 )
//-------------------------------------------------------------------//
FUNCTION DrawSlide( hDlg, nSlide )
   LOCAL hDC, aRect

   hDC   := Win_GetDC( hDlg )
   aRect := Win_GetClientRect( hDlg )

   Win_Rectangle( hDC, aRect[ 1 ]+10, aRect[ 2 ]+10, aRect[ 3 ]-10, aRect[ 4 ]-10 )
   Win_DrawImage( hDC, aRect[ 1 ]+10, aRect[ 2 ]+10, aRect[ 3 ] - aRect[ 1 ] -20, ;
                                  aRect[ 4 ] - aRect[ 2 ] - 20, aSlides[ nSlide ] )

   Win_ReleaseDC( hDlg,hDC )

   Return nil
//----------------------------------------------------------------------//
// The function has to be called via hb_threadStart( {|| ExecuteActiveX( nActiveX ) } )
//
Function ExecuteActiveX( nActiveX )
   Local oCrt

   oCrt := WvgCrt():New( , , { 5,5 }, { 29,59 }, , .f. )
   oCrt:closable := .f.
   oCrt:create()

   SetCursor( 0 )
   SetColor( 'N/W' )
   CLS

   oCrt:show()

   ExeActiveX( oCrt, nActiveX )

   oCrt:Destroy()
   Return nil
//----------------------------------------------------------------------//
Static Function Resize( oCom )
   Win_MoveWindow( oCom:hWnd, 0, 0, hb_gtInfo( HB_GTI_SCREENWIDTH ),  hb_gtInfo( HB_GTI_SCREENHEIGHT ), .F. )
   Return nil
//----------------------------------------------------------------------//
Static Function ExeActiveX( oCrt, nActiveX )
   Local oCom, nKey, cDhtml, cServer, cNavigate, sData, cTitle
   Local lEnd := .f.

   static nTurn := 0

   hb_gtInfo( HB_GTI_DEFERPAINT, .T. )

   DEFAULT nActiveX TO 2

   oCom := WvgActiveXControl():New( oCrt, , { 0,0 }, { 200,200 }, , .t. )

   do case
   case nActiveX == 1
      hb_gtInfo( HB_GTI_WINTITLE, 'Shell.Explorer.2'+'  [  '+'http://www.harbour.vouch.info'+'  ]' )
      oCom:CLSID := 'Shell.Explorer.2'
      oCom:mapEvent( 269, { {|| QOut( ' E X P L O R E R - 2 6 9' ) } } )

   case nActiveX == 11
      hb_gtInfo( HB_GTI_WINTITLE, 'Shell.Explorer.2'+'  [  '+'MSHTML Demo'+'  ]' )
      oCom:CLSID := "MSHTML:" + "<html><h1>Stream Test</h1><p>This HTML content is being loaded from a stream.</html>"

   case nActiveX == 2
      #define evClick     1
      #define evDblClk    2
      #define evBtnDown   3
      #define evMouseMove 4
      #define evBtnUp     5

      hb_gtInfo( HB_GTI_WINTITLE, 'AnalogClockControl.AnalogClock' )
      oCom:CLSID := 'AnalogClockControl.AnalogClock'
      oCom:Id    := 5
      oCom:mapEvent( evDblClk, {|| DoModalWindow()               ,;
                               oCom:Value     := .75632          ,;
                               oCom:BackColor := RGB( 0,140,210 ),;
                               oCom:Refresh()                    ,;
                               oCom:ShowSecondsHand := .t.       ,;
                               oCom:Hands3D   := .t.             ,;
                               oCom:Refresh()                   } )

      oCom:mapEvent( evBtnUp, {|nBtn,nShift,nX,nY| if( nBtn == 2, lEnd := .t., NIL ) } )

   case nActiveX == 3
      hb_gtInfo( HB_GTI_WINTITLE, 'file://C:\harbour\contrib\gtwvg\tests\myharu.pdf' )
      oCom:CLSID := 'file://C:\harbour\contrib\gtwvg\tests\myharu.pdf'

   case nActiveX == 4
      hb_gtInfo( HB_GTI_WINTITLE, 'RM Chart [ F12:Attributes F11:Other Charts ]' )
      oCom:CLSID := 'RMChart.RMChartX'

      // RMChart does not have event interface.
      // Trying to set it generates GPF.
      // Please download RMChart.ocx from http://www.rmchart.com/ . It is free in everysense.

   endcase

   oCom:create()

   // After :CREATE() Messages
   //
   if nActiveX == 1
      oCom:AddressBar := .t.
      oCom:Navigate( 'http://www.harbour.vouch.info' )

   elseif nActiveX == 4
      ConfigureRMChart( oCom )

      oCom:Draw( .t. )
      oCom:Draw2Clipboard()

   endif

   do while !( lEnd )
      nKey := inkey()

      if nKey == HB_K_RESIZE
         Resize( oCom )

      elseif nKey == K_F12
         if nActiveX == 1
            oCom:Navigate( 'www.vouch.info' )

         elseif nActiveX == 4
            oCom:RMCBackColor     := 23456142
            oCom:RMCStyle         := 1
            oCom:RMCUserWatermark := 'Vouch'

            //oCom:AddRegion()
            oCom:Region(1):SetProperties( 5.0,5.0,-5.0,-5.0 )

            oCom:Draw( .t. )
         endif

      elseif nKey == K_F11
         if nActiveX == 4
            nTurn++
            if nTurn > 6
               nTurn := 1
            endif
            sData := ''

            do case
            case nTurn == 1
               hb_gtInfo( HB_GTI_WINTITLE,'RMChart [ Next:F11 ] ' + 'Stacked Bars' )
               SetMode( 30,100 )

               sData += "00003600|00004450|000051|000061|000073|00008-6972|00009412|00011Tahoma|100011|10"
               sData += "0035|1000410|10005-5|10006-5|1000911|100101|100111|100181|100200|1002150000|1002"
               sData += "211|100238|100331|100341|100356|100378|100411|100468|100484|100494|10051-6972|10"
               sData += "052-16777216|10053-1828|100541|100558|10056-16777216|10057-16777216|10060-167772"
               sData += "16|10061-16777216|1006315|10064-32|100652|10066-16776961|10180this is the footer"
               sData += "|10181Example of stacked bars|10182Apples*Pears*Cherries*Strawberries|10183 $|10"
               sData += "184This is an optional axis text, sized 9 points and bold\9b|10187Label Nr. 1*La"
               sData += "bel Nr. 2*Label Nr. 3*Label Nr. 4*Label Nr. 5*Label Nr. 6|10196This is an option"
               sData += "al label axis text|110011|110023|110033|110045|110055|11006-1|1100923|110131|110"
               sData += "14-1|110171|11019-16777077|1102111|110221|110236|1105310000*10000*16000*12000*20"
               sData += "000*10000|120011|120023|120033|120045|120055|12006-1|1200927|120131|12014-1|1201"
               sData += "71|12019-16751616|1202111|120221|120236|120535000*7000*4000*15000*10000*10000|13"
               sData += "0011|130023|130033|130045|130055|13006-1|1300982|130131|13014-1|130171|13019-838"
               sData += "8608|1302111|130221|130236|1305310000*3000*12000*10000*5000*20000|140011|140023|"
               sData += "140033|140045|140055|14006-1|1400925|140131|14014-1|140171|14019-4684277|1402111"
               sData += "|140221|140236|140535000*9000*12000*6000*10000*5000"

            case nTurn == 2
               hb_gtInfo( HB_GTI_WINTITLE,'RMChart [ Next:F11 ] '+'Floating Bars' )
               SetMode( 20,90 )

               sData += "00003550|00004300|000051|000073|00008-2894893|00009412|00011Tahoma|100011|100035"
               sData += "|100045|10005-5|10006-5|1000911|100101|100111|100131|100181|100201|1002113|10022"
               sData += "13|100238|100331|100341|100356|100378|100411|100468|100482|10052-16777216|10053-"
               sData += "1120086|100544|100555|10056-16777216|10057-16777216|10060-16777216|10061-1677721"
               sData += "6|1006316|10064-5383962|100652|10066-16777011|10181Birth of a Killer App|10182Sc"
               sData += "hedule*Reality|10187Design*Development*Testing*Bug Fixing*Documentation*Marketin"
               sData += "g|1020104/01*04/02*04/03*04/04*04/05*04/06*04/07*04/08*04/09*04/10*04/11*04/12*0"
               sData += "5/01|110011|110026|110044|110101|110131|11019-6751336|1102111|110221|1102312|110"
               sData += "531*3*4*6*6*4*7*4*9*3*10*3|120011|120026|120044|120101|120132|12019-47872|120211"
               sData += "1|120221|1202312|120531*.5*1.5*10.5*12*1*12*1*12.5*.5*2*11"

            case nTurn == 3
               hb_gtInfo( HB_GTI_WINTITLE,'RMChart [ Next:F11 ] '+'Four Regions' )
               SetMode( 40,120 )

               sData += "00003700|00004500|000054|000061|000071|00008-984833|00009412|00011Tahoma|100011|"
               sData += "100032|100042|10005348|10006248|1000910|100101|100111|100181|100200|10021100|100"
               sData += "2211|100238|100331|100341|100355|100378|100481|100491|10051-984833|10052-1677721"
               sData += "6|10053-657956|100541|100558|10056-16777216|10057-16777216|10060-16777216|10061-"
               sData += "16777216|10187Label 1*Label 2*Label 3*Label 4*Label 5|110011|110021|110031|11004"
               sData += "6|110056|11006-1|110091|110131|11014-1|110171|1102111|110221|110235|1105330*40*7"
               sData += "0*60*20|200011|20003352|200042|20005-2|20006248|2000910|200101|200111|200181|200"
               sData += "200|20021100|2002211|200238|200331|200341|200355|200378|200484|200491|20051-9848"
               sData += "33|20052-16777216|20053-657956|200544|200555|20056-16777216|20057-16777216|20060"
               sData += "-16777216|20061-16777216|20187Label 1*Label 2*Label 3*Label 4*Label 5|210011|210"
               sData += "023|210033|210045|210055|21006-1|210091|210101|210131|21014-1|210171|2102111|210"
               sData += "221|210235|2105320*10*15*25*30|220011|220023|220033|220045|220055|22006-1|220091"
               sData += "|220101|220131|22014-1|220171|2202111|220221|220235|2205325*30*10*20*15|230011|2"
               sData += "30023|230033|230045|230055|23006-1|230091|230101|230131|23014-1|230171|2302111|2"
               sData += "30221|230235|2305310*20*40*20*30|240011|240023|240033|240045|240055|24006-1|2400"
               sData += "91|240101|240131|24014-1|240171|2402111|240221|240235|2405340*30*20*30*20|300011"
               sData += "|300032|30004252|30005348|30006-2|3000910|300101|300181|300481|300491|30051-9848"
               sData += "33|30052-16777216|30053-657956|310011|3100251|310031|3100454|310054|310071|31009"
               sData += "1|310121|310151|310161|310171|310182|310211|310221|310235|3105330*50*20*40*60|40"
               sData += "0011|40003352|40004252|40005-2|40006-2|4000910|400101|400111|400131|400181|40020"
               sData += "100|40021250|4002211|400238|400281|400292|400300|400310|400322|400331|400341|400"
               sData += "3510|400378|400482|400492|40051-984833|40052-16777216|40053-984833|400541|400558"
               sData += "|40056-16776961|40057-16777216|400592|40060-16777216|40061-16777216|40183$ |4018"
               sData += "5 %|410011|410021|410031|410043|410053|41006-1|4100950|410131|41014-1|410171|410"
               sData += "19-10496|4102111|410221|4102310|41053240*230*220*180*170*160*145*130*125*115|420"
               sData += "011|4200221|420035|4200422|420052|420061|420071|4200963|420111|420121|420131|420"
               sData += "171|42019-16744448|4202115|420221|4202310|420261|420538.1*6.2*4.3*2.2*1.2*3.1*5."
               sData += "2*11.4*7.3*4.2"

            case nTurn == 4
               hb_gtInfo( HB_GTI_WINTITLE,'RMChart [ Next:F11 ] '+'10 Biggest Companies' )
               SetMode( 25,90 )

               sData += "00003670|00004450|000051|000061|000071|00008-10185235|00009412|00011Tahoma|10001"
               sData += "1|100035|1000410|10005-5|10006-5|1000912|100101|100111|100131|100181|10020100000"
               sData += "|10021250000|1002211|100239|100281|100292|100300|100310|100322|100331|100341|100"
               sData += "3510|100378|100482|100492|10051-32944|10052-1296|10053-983041|100541|100558|1005"
               sData += "6-1828|10057-16777216|100592|10060-1828|10061-16777216|10180data source: F.A.Z|1"
               sData += "0181The world's 10 biggest industrial companies 2003|10183$ |10184Total turnover"
               sData += " in Mill. Dollar|10185 %|10186Net operating margin|10187Exxon Mobil*Royal Dutch "
               sData += "/ Shell*BP*General Motors*Daimler Chrysler*Ford Motor*Toyota Motor*General Elect"
               sData += "ric*TOTAL*Chevron Texaco|110011|110021|110031|110043|110053|11006-1|1100950|1101"
               sData += "31|11014-1|110171|11019-10496|1102111|110221|1102310|11053242365*235598*232571*1"
               sData += "85524*170457*164196*149321*132797*130067*119703|120011|1200221|120035|1200422|12"
               sData += "0052|120061|120071|1200970|120111|120121|120131|1201421|120171|12019-16744448|12"
               sData += "02115|120221|1202310|120261|120538.9*4.1*4.4*2.1*.3*.3*5.9*11.3*6.7*6"

            case nTurn == 5
               hb_gtInfo( HB_GTI_WINTITLE,'RMChart [ Next:F11 ] '+'Grouped Bars' )
               SetMode( 25,80 )

               sData += "00003600|00004450|000051|000061|000075|00008-2|00009412|00010paper.jpg|00011Taho"
               sData += "ma|100011|100035|100045|10005-5|10006-5|1000910|100101|100111|100181|100200|1002"
               sData += "1100|1002211|100238|100331|100341|100355|100378|100468|100484|10053-2|100541|100"
               sData += "558|100631|100651|10182First quarter*Second quarter*Third quarter*Fourth quarter"
               sData += "|101872000*2001*2002*2003*2004|110011|110022|110044|110131|1102111|110221|110235"
               sData += "|1105330*20*40*60*10|120011|120022|120044|120131|1202111|120221|120235|1205330*2"
               sData += "0*50*70*60|130011|130022|130044|130131|1302111|130221|130235|1305340*10*30*20*80"
               sData += "|140011|140022|140044|140131|1402111|140221|140235|1405370*50*80*40*30"

            case nTurn == 6
               hb_gtInfo( HB_GTI_WINTITLE,'RMChart [ Next:F11 ] '+'Flow Chart' )
               SetMode( 30,50 )

               sData += "00003305|00004400|000051|00008-984833|00009412|00011Tahoma|100011|100035|100045|"
               sData += "10005-5|10006-5|10180\7C|010011|010051|010072|010081|0101050|0101125|01012100|01"
               sData += "01325|01014-5952982|01015-5952982|01016255|010191|0102010|01026Start|01030-256|0"
               sData += "10012|010054|0100721|01014-16776961|010222|01024100*100|0102550*75|01026|010272|"
               sData += "010293|010013|010051|010071|010081|0101050|0101175|01012100|0101325|01014-669788"
               sData += "2|01015-6697882|01016255|010191|0102010|01026i = 1|01030-16777216|010014|010054|"
               sData += "0100721|01014-16776961|010222|01024100*100|01025100*150|01026|010272|010293|0100"
               sData += "15|010051|010073|010081|0101050|01011150|01012100|0101350|01014-65536|01015-6553"
               sData += "6|01016255|010191|0102010|01026i = 39?|01030-256|010016|010054|0100721|01014-167"
               sData += "76961|010222|01024100*100|01025200*225|01026|010272|010293|010017|010051|010071|"
               sData += "010081|0101050|01011225|01012100|0101325|01014-6697882|01015-6697882|01016255|01"
               sData += "0191|0102010|01026i =  i + 1|01030-16777216|010018|010054|0100721|01014-16776961"
               sData += "|010222|01024100*100|01025250*275|01026|010272|010293|010019|010051|010073|01008"
               sData += "1|0101050|01011275|01012100|0101350|01014-65536|01015-65536|01016255|010191|0102"
               sData += "010|01026i <= 100|01030-256|0100110|010054|0100721|01014-16776961|010222|0102410"
               sData += "0*100|01025325*350|01026|010272|010293|0100111|010051|010072|010081|0101050|0101"
               sData += "1350|01012100|0101325|01014-5952982|01015-5952982|01016255|010191|0102010|01026S"
               sData += "top|01030-256|0100113|010054|0100721|01014-16776961|010222|01024150*200|01025175"
               sData += "*175|01026|010272|010293|0100114|010051|010081|01010145|01011157|0101250|01014-1"
               sData += "6776961|01015-5383962|01016255|010191|0102010|01026yes|01030-16777216|0100115|01"
               sData += "0051|010071|010081|01010200|01011162|01012100|0101325|01014-6697882|01015-669788"
               sData += "2|01016255|010191|0102010|01026i = 69|01030-16777216|0100116|010054|0100721|0101"
               sData += "4-16776961|010224|0102450*10*10*100|01025300*300*125*125|01026|010272|010293|010"
               sData += "0117|010054|0100721|01014-16776961|010223|01024250*250*100|01025162*125*125|0102"
               sData += "6|010272|010293|0100118|010051|010081|01010100|01011200|0101250|0101325|01014-16"
               sData += "776961|01016255|010191|0102010|01026no|01030-16777216|0100119|010051|010081|0101"
               sData += "010|01011280|0101240|01014-16776961|01016255|010191|0102010|01026yes|01030-16777"
               sData += "216|0100120|010051|010081|01010100|01011322|0101250|01014-16776961|01016255|0101"
               sData += "91|0102010|01026no|01030-16777216|0100120|010051|010079|01010180|01011280|010121"
               sData += "20|01013100|01015-39322|010191|010209|01026RMChart is not a flowchart tool. This"
               sData += " is just an example for the use of CustomObjects!|01030-256"

            endcase

            oCom:Reset()
            oCom:RMCFile := sData
            oCom:Draw( .t. )
            Resize( oCom )

         endif

      endif

      if nKey == 27
         exit
      endif
   enddo

   oCom:Destroy()

   Return nil
//----------------------------------------------------------------------//
Function ConfigureRMChart( RMChart )

   #define RMC_CTRLSTYLEFLAT          0
   #define RMC_PIE_GRADIENT           52
   #define RMC_FULL                   1
   #define RMC_EXPLODE_NONE           0
   #define RMC_VLABEL_ABSOLUTE        6
   #define RMC_HATCHBRUSH_OFF         0

   /* The code pulled from freewin sources */
   WITH OBJECT  RMChart
      :Font             := "Tahoma"
      :RMCStyle         := RMC_CTRLSTYLEFLAT

      :AddRegion()
      WITH OBJECT :Region( 1 )
         :Footer = "http://vouch.info"

         :AddCaption()
         WITH OBJECT :Caption()
            :Titel     := "GTWVG Active-X Demo"
            :FontSize  := 10
            :Bold      := .T.
         END

         :AddGridlessSeries()

         WITH OBJECT :GridLessSeries
            :SeriesStyle      := RMC_PIE_GRADIENT
            :Alignment        := RMC_FULL
            :Explodemode      := RMC_EXPLODE_NONE
            :Lucent           := .F.
            :ValueLabelOn     := RMC_VLABEL_ABSOLUTE
            :HatchMode        := RMC_HATCHBRUSH_OFF
            :StartAngle       := 0
            :DataString       := "10*5*20*25"
         END
      END
   END

   Return nil
//----------------------------------------------------------------------//
Static Function DoModalWindow()
   Local oCrt, nSel

   /* This part can be clubbed in a separate prg for different dialogs
    * OR can be loaded from a data dictionary.
    */
   oCrt := WvgCrt():New( , , { 4,8 }, { 12,49 }, , .T. )

   oCrt:lModal      := .t.
   oCrt:exStyle     := WS_EX_DLGMODALFRAME
   oCrt:resizable   := .f.
   oCrt:closable    := .f.
   oCrt:title       := 'Information! [R:4 C:8]'

   oCrt:rbUp        := {|aPos,uNIL,obj| DispOutAt( maxrow(), 0, padc( 'rbUp', maxcol()+1 ),'W+/R*' ) }
   oCrt:lbUp        := {|aPos,uNIL,obj| DispOutAt( maxrow(), 0, padc( 'lbUp', maxcol()+1 ),'W+/B*' ) }
   oCrt:leave       := {|| DispOutAt( maxrow(), 0, padc( 'Leaving', maxcol()+1 ), 'W+/RB' ) }
   oCrt:enter       := {|| DispOutAt( maxrow(), 0, padc( 'Entering', maxcol()+1 ), 'W+/B' ) }

   oCrt:Create()
   oCrt:show()

   pGT_[ 3 ] := hb_gtSelect()

   #if 0
   hb_toOutDebug( "Browser:title     = %s", hb_gtInfoEx( pGT_[ 2 ], HB_GTI_WINTITLE ) )
   hb_toOutDebug( "Application:title = %s", hb_gtInfoEx( pGT_[ 1 ], HB_GTI_WINTITLE ) )
   hb_toOutDebug( "This:title        = %s", hb_gtInfoEx( pGT_[ 3 ], HB_GTI_WINTITLE ) )
   #endif

   // Here goes the Clipper Code
   //
   SetColor( 'N/W' )
   CLS
   do while .t.
      nSel := alert( 'I am in modal window !;< Try: MMove LBUp RBUp >;Click Parent Window', { 'OK' } )

      if nSel == 0  .or. nSel == 1
         exit

      elseif nSel == 1
         #if 0
         hb_gtInfoEx( pGT_[ 2 ], HB_GTI_SETPOS_XY, 40, 40 )
         hb_gtInfoEx( pGT_[ 2 ], HB_GTI_SPEC, HB_GTS_WNDSTATE, HB_GTS_WS_MAXIMIZED )
         #endif

      endif
   enddo

   // Cleanup
   //
   pGT_[ 3 ] := NIL
   oCrt:Destroy()

   Return nil
//----------------------------------------------------------------------//
