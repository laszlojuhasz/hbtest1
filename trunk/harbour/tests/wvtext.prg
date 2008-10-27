/*
 * $Id$
 */

//----------------------------------------------------------------------//
//----------------------------------------------------------------------//
//----------------------------------------------------------------------//
//
//                  [x]Harbour Extended Features Deno
//                                    .
//                 Pritpal Bedi <pritpal@vouchcac.com>
//
//----------------------------------------------------------------------//
//----------------------------------------------------------------------//
//----------------------------------------------------------------------//

#include "hbgtinfo.ch"
#include "inkey.ch"
#ifdef __GTWVG__
#include "hbgtwvg.ch"
#endif

#define RGB(r,g,b) ( r + ( g * 256 ) + ( b * 256 * 256 ) )

//----------------------------------------------------------------------//

STATIC nRows := 20, nCols := 60, nColorIndex := 1

//----------------------------------------------------------------------//

FUNCTION Main()
   Local nKey, lMark, lResize, lClose
   Local nHeight := 20
   Local nWidth  := Int( nHeight/2 )
   Local cFont

   Hb_GtInfo( HB_GTI_FONTNAME , cFont   )
   Hb_GtInfo( HB_GTI_FONTWIDTH, nWidth  )
   Hb_GtInfo( HB_GTI_FONTSIZE , nHeight )
   #ifdef __GTWVG__
   Hb_GtInfo( HB_GTI_ICONFILE, "..\contrib\gtwvg\tests\vr_1.ico" )
   #endif
   SetCursor( 0 )
   SetColor( "n/w" )

   HB_GtInfo( HB_GTI_NOTIFIERBLOCK, {|nEvent, ...| MyNotifier( nEvent, ... ) } )

   DispScreen()

   DO WHILE .T.
      nKey := Inkey()
      if nKey == K_ESC
         exit
      endif

      DO CASE
      CASE nKey == K_ENTER
         Alert( "<Enter> Pressed" )

      CASE nKey == K_F2
         lMark := Hb_GtInfo( HB_GTI_SELECTCOPY )
         Hb_GtInfo( HB_GTI_SELECTCOPY, !lMark )

      CASE nKey == K_F3
         lResize := Hb_GtInfo( HB_GTI_RESIZABLE )
         Hb_GtInfo( HB_GTI_RESIZABLE, !lResize )

      CASE nKey == K_F4
         lClose := Hb_GtInfo( HB_GTI_CLOSABLE )
         hb_GtInfo( HB_GTI_CLOSABLE, !lClose )

      CASE nKey == K_F5
         SetPalette( 1 )

      CASE nKey == K_F6
         SetPalette( 0 )

      CASE nKey == K_F7
         SetPaletteIndex()

      CASE nKey == K_F8
         Alert( "Menu text changed. Was: " + hb_GtInfo( HB_GTI_SELECTCOPY, DToS(Date()) + " " + Time() ) )

      CASE nKey == K_F9
         RunInSysTray()

      CASE nKey == K_F10
         hb_threadStart( @thFunc() )

      ENDCASE
   ENDDO

   RETURN NIL
//----------------------------------------------------------------------//

STATIC FUNCTION MyNotifier( nEvent, ... )

   DO CASE

   CASE nEvent == HB_GTE_SETFOCUS
      DispScreen()
      DispOutAt( maxrow(), 33, "We got focus", "B/G*" )

   CASE nEvent == HB_GTE_CLOSE
      DispScreen()
      if Alert( "Close Application", {"Yes","No" } ) == 2
         Return ( 1 )
      endif

   ENDCASE

   RETURN 0

//----------------------------------------------------------------------//

STATIC FUNCTION DispScreen()
   Local nRow := 12, nCol := 28
   Local cColor := "N/W"
   Local nMaxCol := MaxCol()+1

   DispBegin()

   CLS
   DispOutAt( 0, 0,padc( "Harbour GT - New Features", maxcol()+1 ), "N/GR*" )

   // Contributed by Massimo Belgrano
   DispOutAt( 2, 0, padc( "______  __             ______________________                        ",nMaxCol ), "W+/W" )
   DispOutAt( 3, 0, padc( "___  / / /_____ ___________ /___________  _________    __  ____/____/",nMaxCol ), "W+/W" )
   DispOutAt( 4, 0, padc( "__  /_/ /_  __ `/_  ___/_  __ \  __ \  / / /_  ___/    _  / __ __/   ",nMaxCol ), "W+/W" )
   DispOutAt( 5, 0, padc( "_  __  / / /_/ /_  /   _  /_/ / /_/ / /_/ /_  /        / /_/ / _  /  ",nMaxCol ), "W+/W" )
   DispOutAt( 6, 0, padc( "/_/ /_/  \__,_/ /_/    /_.___/\____/\__,_/ /_/         \____/  /_/   ",nMaxCol ), "W+/W" )

   DispOutAt( ++nRow, nCol, "< F2 MarkCopy    Toggle >", cColor )
   DispOutAt( ++nRow, nCol, "< F3 Resize      Toggle >", cColor )
   DispOutAt( ++nRow, nCol, "< F4 Closable    Toggle >", cColor )
   DispOutAt( ++nRow, nCol, "< F5 Palette L   Repeat >", cColor )
   DispOutAt( ++nRow, nCol, "< F6 Palette D   Repeat >", cColor )
   DispOutAt( ++nRow, nCol, "< F7 Palette By Index R >", cColor )
   DispOutAt( ++nRow, nCol, "< F8 MarkCopy menu text >", cColor )
   DispOutAt( ++nRow, nCol, "<    Click Other Window >", cColor )
   DispOutAt( ++nRow, nCol, "<    Click X Button     >", cColor )
#ifdef __GTWVG__
   DispOutAt( ++nRow, nCol, "< F9 Run in SysTray     >", cColor )
#endif
   DispOutAt( ++nRow, nCol, "< F10 Open New Window   >", cColor )

   DispOutAt( maxrow(), 0, Space( maxcol()+1 ), "N/G*" )

   DispOutAt( 0, 0                  , "TL", "N/GR*" )
   DispOutAt( 0, MaxCol() - 1       , "TR", "N/GR*" )
   DispOutAt( MaxRow(), 0           , "BL", "N/G*"  )
   DispOutAt( MaxRow(), MaxCol() - 1, "BR", "N/G*"  )

   DispEnd()
   RETURN NIL

//----------------------------------------------------------------------//

PROCEDURE HB_GTSYS()
#ifdef __GTWVG__
   REQUEST HB_GT_WVG_DEFAULT
#else
   REQUEST HB_GT_WVT_DEFAULT
   REQUEST HB_GT_WIN
#endif
   RETURN

//----------------------------------------------------------------------//

FUNCTION SetPalette( nMode )
   Local aPalette := Hb_GtInfo( HB_GTI_PALETTE )

   static nR := 198
   static nG := 198
   static nB := 198

   nR += iif( nMode == 0, -5, 5 )
   nG += iif( nMode == 0, -5, 5 )
   nB += iif( nMode == 0, -5, 5 )

   // Change "W" to slightly gray everytime you press F5
   //
   aPalette[ 8 ] := RGB( nR, nG, nB )

   Hb_GtInfo( HB_GTI_PALETTE, aPalette )
   DispScreen()

   RETURN NIL
//----------------------------------------------------------------------//

FUNCTION SetPaletteIndex()

   Hb_GtInfo( HB_GTI_PALETTE, 8, RGB( 120, 200, 240 ) )
   DispScreen()

   RETURN NIL

//----------------------------------------------------------------------//

#define NIM_ADD               0
#define NIM_MODIFY            1
#define NIM_DELETE            2

FUNCTION RunInSysTray()
   #ifdef __GTWVG__
   Alert( "Please check your System Tray area after exiting this alert,"+;
             ";then right click on the icon"+;
                 ";displaying tooltip "Harbour GT in SysTray" !" )

   Hb_GtInfo( HB_GTI_SPEC, HB_GTS_SYSTRAYICON, { NIM_ADD, NIT_FILE, ;
                "..\contrib\gtwvg\tests\vr_1.ico", "Harbour GT in SysTray" } )
   #endif
   RETURN NIL

//----------------------------------------------------------------------//

PROCEDURE thFunc()
   Local cTitle, oBrowse, lEnd, nKey, i, aStruct
   Local aColor := { 'W+/N', 'W+/B', 'W+/G', 'W+/BG', 'W+/N*', 'W+/RB', 'N/W*', 'N/GR*' }

   static nBrowser := 0
   static nZx := 0
   static nZy := 0

   nBrowser++
   nZx += 20
   nZy += 20

   /* allocate own GT driver */
   hb_gtReload( 'WVT' )
   Hb_GtInfo( HB_GTI_PALETTE, 8, RGB( 120, 200, 240 ) )

   if ( nBrowser % 2 ) != 0
      Hb_GtInfo( HB_GTI_RESIZEMODE, HB_GTI_RESIZEMODE_ROWS )
   endif
   Hb_GtInfo( HB_GTI_WINTITLE, 'Test.dbf    ['+if( ( nBrowser % 2 ) != 0, 'RESIZABLE_BY_ROWS', 'RESIZABLE_BY_FONT' )+']' )

   SetCursor( 0 )

   nColorIndex++
   if nColorIndex > len( aColor )
      nColorIndex := 1
   endif

   nRows++
   nCols += 2

   SetMode( nRows,nCols )
   SetColor( aColor[ nColorIndex ] )
   Hb_GtInfo( HB_GTI_WINTITLE, cTitle )
   Hb_GtInfo( HB_GTI_SETPOS_XY, nZx, nZy )

   cTitle := 'New Window with '+ltrim( str( MaxRow() ) )+;
                          ' Rows and '+ltrim( str( MaxCol() ) )+' Columns'
   DispOutAt( 0, 0, padc( cTitle, maxcol()+1 ), 'N/GR*' )

   use test shared
   aStruct := DbStruct()

   oBrowse := TBrowse():New( 1, 0, maxrow(), maxcol() )

   oBrowse:ColSep        := " � "
   oBrowse:HeadSep       := "���"
   oBrowse:GoTopBlock    := { || dbGoTop() }
   oBrowse:GoBottomBlock := { || dbGoBottom() }
   oBrowse:SkipBlock     := { | nSkip | dbSkipBlock( nSkip,oBrowse ) }

   for i := 1 to len( aStruct )
      oBrowse:AddColumn( TBColumnNew( aStruct[ i,1 ], BlockField( i ) ) )
   next

   oBrowse:configure()

   lEnd := .f.
   While !lEnd
      oBrowse:ForceStable()

      nKey := InKey( 0 )

      if BrwHandleKey( oBrowse, nKey, @lEnd )
         //
      else
         if nKey == HB_K_RESIZE
            cTitle := 'New Window with '+ltrim( str( MaxRow() ) )+;
                          ' Rows and '+ltrim( str( MaxCol() ) )+' Columns'
            DispOutAt( 0, 0, padc( cTitle, maxcol()+1 ), 'N/GR*' )

            oBrowse:nBottom := MaxRow()
            oBrowse:nRight := MaxCol()
            oBrowse:Configure()
            oBrowse:RefreshAll()
         endif
      endif
   end

   DbCloseArea()

   RETURN
//----------------------------------------------------------------------//
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
STATIC FUNCTION BlockField( i )
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
