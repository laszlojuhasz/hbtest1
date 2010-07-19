/*
 * $Id$
 */

/*
 * Harbour Project source code:
 * Source file for the Xbp*Classes
 *
 * Copyright 2009 Pritpal Bedi <pritpal@vouchcac.com>
 * http://harbour-project.org
 *
 * Navigation Based on TBrowse.prg
 * Copyright 2008 Przemyslaw Czerpak <druzus / at / priv.onet.pl>
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
/*----------------------------------------------------------------------*/
/*----------------------------------------------------------------------*/
/*----------------------------------------------------------------------*/
/*
 *                                EkOnkar
 *                          ( The LORD is ONE )
 *
 *                  Xbase++ Compatible XbpBrowse Class
 *
 *                  Pritpal Bedi <pritpal@vouchcac.com>
 *                              10Jul2009
 */
/*----------------------------------------------------------------------*/
/*----------------------------------------------------------------------*/
/*----------------------------------------------------------------------*/

#include "hbclass.ch"
#include "common.ch"
#include "set.ch"

#include "xbp.ch"
#include "gra.ch"
#include "appevent.ch"

#include "button.ch"
#include "color.ch"
#include "error.ch"
#include "inkey.ch"
#include "setcurs.ch"
#include "tbrowse.ch"

#define HB_CLS_NOTOBJECT

#define _TBCI_COLOBJECT       1    // column object
#define _TBCI_COLWIDTH        2    // width of the column
#define _TBCI_COLPOS          3    // column position on screen
#define _TBCI_CELLWIDTH       4    // width of the cell
#define _TBCI_CELLPOS         5    // cell position in column
#define _TBCI_COLSEP          6    // column separator
#define _TBCI_SEPWIDTH        7    // width of the separator
#define _TBCI_HEADING         8    // column heading
#define _TBCI_FOOTING         9    // column footing
#define _TBCI_HEADSEP         10   // heading separator
#define _TBCI_FOOTSEP         11   // footing separator
#define _TBCI_DEFCOLOR        12   // default color
#define _TBCI_FROZENSPACE     13   // space after frozen columns
#define _TBCI_LASTSPACE       14   // space after last visible column
#define _TBCI_SIZE            14   // size of array with TBrowse column data

#define _TBC_SETKEY_KEY       1
#define _TBC_SETKEY_BLOCK     2

#define _TBC_CLR_STANDARD     1
#define _TBC_CLR_SELECTED     2
#define _TBC_CLR_HEADING      3
#define _TBC_CLR_FOOTING      4
#define _TBC_CLR_MAX          4

#define _TBR_CONF_COLORS      1
#define _TBR_CONF_COLUMNS     2
#define _TBR_CONF_ALL         3

/* Footing/heading line separator. */
#define _TBR_CHR_LINEDELIMITER ";"

#define _TBR_COORD( n )       Int( n )

#define ISFROZEN( n )         ( ascan( ::aLeftFrozen, n ) > 0 .OR. ascan( ::aRightFrozen, n ) > 0 )

/*----------------------------------------------------------------------*/

#define __ev_keypress__                    1                /* Keypress Event */
#define __ev_mousepress_on_frozen__        31               /* Mousepress on Frozen */
#define __ev_mousepress__                  2                /* Mousepress */
#define __ev_xbpBrw_itemSelected__         3                /* xbeBRW_ItemSelected */
#define __ev_wheel__                       4                /* wheelEvent */
#define __ev_horzscroll_via_qt__           11               /* Horizontal Scroll Position : sent by Qt */
#define __ev_vertscroll_via_user__         101              /* Vertical Scrollbar Movements by the User */
#define __ev_vertscroll_sliderreleased__   102              /* Vertical Scrollbar: Slider Released */
#define __ev_horzscroll_slidermoved__      103              /* Horizontal Scrollbar: Slider moved */
#define __ev_horzscroll_sliderreleased__   104              /* Horizontal Scrollbar: Slider Released */
#define __ev_columnheader_pressed__        111              /* Column Header Pressed */
#define __ev_headersec_resized__           121              /* Header Section Resized */
#define __ev_footersec_resized__           122              /* Footer Section Resized */
#define __ev_frame_resized__               2001

/*----------------------------------------------------------------------*/

CREATE CLASS XbpBrowse INHERIT XbpWindow

   VAR cargo                  AS USUAL          EXPORTED    // 01. User-definable variable

PROTECTED:
   VAR n_Top                  AS NUMERIC          INIT 0    // 02. Top row number for the TBrowse display
   VAR n_Left                 AS NUMERIC          INIT 0    // 03. Leftmost column for the TBrowse display
   VAR n_Bottom               AS NUMERIC          INIT 0    // 04. Bottom row number for the TBrowse display
   VAR n_Right                AS NUMERIC          INIT 0    // 05. Rightmost column for the TBrowse display

   VAR columns                AS ARRAY            INIT {}   // 06. Array of TBrowse columns
   VAR columnsC               AS ARRAY            INIT {}
   VAR columnsL               AS ARRAY            INIT {}
   VAR columnsR               AS ARRAY            INIT {}

   VAR cHeadSep               AS CHARACTER        INIT ""   // 07. Heading separator characters
   VAR cColSep                AS CHARACTER        INIT " "  // 08. Column separator characters
   VAR cFootSep               AS CHARACTER        INIT ""   // 09. Footing separator characters

   VAR cColorSpec             AS CHARACTER                  // 10. Color table for the TBrowse display

   VAR bSkipBlock                                 INIT {|| NIL } // 11. Code block used to reposition data source
   VAR bGoTopBlock                                INIT {|| NIL } // 12. Code block executed by TBrowse:goTop()
   VAR bGoBottomBlock                             INIT {|| NIL } // 13. Code block executed by TBrowse:goBottom()
   VAR bFirstPosBlock                             INIT {|| NIL }
   VAR bLastPosBlock                              INIT {|| NIL }
   VAR bPhyPosBlock                               INIT {|| NIL }
   VAR bPosBlock                                  INIT {|| NIL }
   VAR bGoPosBlock                                INIT {|| NIL }
   VAR bHitBottomBlock                            INIT {|| NIL }
   VAR bHitTopBlock                               INIT {|| NIL }
   VAR bStableBlock                               INIT {|| NIL }

   VAR dummy                                      INIT ""   // 14. ??? In Clipper it's character variable with internal C level structure containing browse data
   VAR cBorder                AS CHARACTER                  // 15. character value defining characters drawn around object
   VAR cMessage                                             // 16. character string displayed on status bar
   VAR keys                   AS ARRAY                      // 17. array with SetKey() method values
   VAR styles                 AS ARRAY                      // 18. array with SetStyle() method values

EXPORTED:

#if 0
   VAR mRowPos                AS INTEGER          INIT 0    // numeric value indicating the data row of the mouse position
   VAR mColPos                AS INTEGER          INIT 0    // numeric value indicating the data column of the mouse position
#else
   METHOD mRowPos                                 SETGET    // numeric value indicating the data row of the mouse position
   METHOD mColPos                                 SETGET    // numeric value indicating the data column of the mouse position
#endif

   METHOD setStyle( nStyle, lNewValue )                     // maintains a dictionary within an object
   METHOD setKey( nKey, bBlock )                            // get/set a code block associated with an INKEY() value
   METHOD applyKey( nKey )                                  // evaluate the code block associated with given INKEY() value
   METHOD hitTest( mRow, mCol )                             // indicate position of mouse cursor relative to TBrowse
   METHOD nRow                                    SETGET    // screen row number for the actual cell
   METHOD nCol                                    SETGET    // screen column number for the actual cell
   METHOD border( cBorder )                       SETGET    // get/set character value used for TBrowse are border
   METHOD message( cMessage )                     SETGET    // get/set character string displayed on status bar

   METHOD nTop( nTop )                            SETGET    // get/set top row number for the TBrowse display
   METHOD nLeft( nLeft )                          SETGET    // get/set leftmost column for the TBrowse display
   METHOD nBottom( nBottom )                      SETGET    // get/set bottom row number for the TBrowse display
   METHOD nRight( nRight )                        SETGET    // get/set rightmost column for the TBrowse display

   METHOD headSep( cHeadSep )                     SETGET    // get/set heading separator characters
   METHOD colSep( cColSep )                       SETGET    // get/set column separator characters
   METHOD footSep( cFootSep )                     SETGET    // get/set footing separator characters

   METHOD skipBlock( bSkipBlock )                 SETGET    // get/set code block used to reposition data source
   METHOD goTopBlock( bBlock )                    SETGET    // get/set code block executed by TBrowse:goTop()
   METHOD goBottomBlock( bBlock )                 SETGET    // get/set code block executed by TBrowse:goBottom()
   /* Xbase++ */
   METHOD firstPosBlock( bBlock )                 SETGET
   METHOD lastPosBlock( bBlock )                  SETGET
   METHOD phyPosBlock( bBlock )                   SETGET
   METHOD posBlock( bBlock )                      SETGET
   METHOD goPosBlock( bBlock )                    SETGET
   METHOD hitBottomBlock( bBlock )                SETGET
   METHOD hitTopBlock( bBlock )                   SETGET
   METHOD stableBlock( bBlock )                   SETGET

   METHOD colorSpec( cColorSpec )                 SETGET    // get/set string value with color table for the TBrowse display

   ACCESS rowPos                                  METHOD getRowPos      // get current cursor row position
   ASSIGN rowPos                                  METHOD setRowPos      // set current cursor row position

   ACCESS colPos                                  METHOD getColPos      // get current cursor column position
   ASSIGN colPos                                  METHOD setColPos      // set current cursor column position

   ACCESS freeze                                  METHOD getFrozen      // get number of frozen columns
   ASSIGN freeze                                  METHOD freeze         // set number of columns to freeze

   ACCESS hitTop                                  METHOD getTopFlag     // get the beginning of available data flag
   ASSIGN hitTop                                  METHOD setTopFlag     // set the beginning of available data flag

   ACCESS hitBottom                               METHOD getBottomFlag  // get the end of available data flag
   ASSIGN hitBottom                               METHOD setBottomFlag  // set the end of available data flag

   ACCESS autoLite                                METHOD getAutoLite    // get automatic highlighting state
   ASSIGN autoLite                                METHOD setAutoLite    // set automatic highlighting

   ACCESS stable                                  METHOD getStableFlag  // get flag indicating if the TBrowse object is stable
   ASSIGN stable                                  METHOD setStableFlag  // set flag indicating if the TBrowse object is stable

   METHOD addColumn( oCol )                                 // adds a TBColumn object to the TBrowse object
   METHOD delColumn( nColumn )                              // delete a column object from a browse
   METHOD insColumn( nColumn, oCol )                        // insert a column object in a browse
   METHOD setColumn( nColumn, oCol )                        // replaces one TBColumn object with another
   METHOD getColumn( nColumn )                              // gets a specific TBColumn object

   METHOD rowCount()                                        // number of visible data rows in the TBrowse display
   METHOD colCount()                                        // number of browse columns

   METHOD colWidth( nColumn )                               // returns the display width of a particular column

   METHOD leftVisible()                                     // indicates position of leftmost unfrozen column in display
   METHOD rightVisible()                                    // indicates position of rightmost unfrozen column in display

   METHOD hilite()                                          // highlights the current cell
   METHOD deHilite()                                        // dehighlights the current cell
   METHOD refreshAll()                                      // causes all data to be recalculated during the next stabilize
   METHOD refreshCurrent()                                  // causes the current row to be refilled and repainted on next stabilize
   METHOD forceStable()                                     // performs a full stabilization
   METHOD invalidate()                                      // forces entire redraw during next stabilization

   METHOD up()                                              // moves the cursor up one row
   METHOD down()                                            // moves the cursor down one row
   METHOD pageUp()                                          // repositions the data source upward
   METHOD pageDown()                                        // repositions the data source downward
   METHOD goTop()                                           // repositions the data source to the top of file
   METHOD goBottom()                                        // repositions the data source to the bottom of file

   METHOD left()                                            // moves the cursor left one column
   METHOD right()                                           // moves the cursor right one column
   METHOD home()                                            // moves the cursor to the leftmost visible data column
   METHOD end()                                             // moves the cursor to the rightmost visible data column
   METHOD panHome()                                         // moves the cursor to the leftmost visible data column
   METHOD panEnd()                                          // moves the cursor to the rightmost data column
   METHOD lastCol()
   METHOD firstCol()
   METHOD panLeft()                                         // pans left without changing the cursor position
   METHOD panRight()                                        // pans right without changing the cursor position

   METHOD stabilize()                                       // performs incremental stabilization
   METHOD colorRect( aRect, aColors )                       // alters the color of a rectangular group of cells

   METHOD viewArea()                                        // Xbase++ compatible method
   METHOD firstScrCol()                                     // Xbase++ compatible method

   MESSAGE _left()                                METHOD Left()
   MESSAGE _right()                               METHOD Right()
   MESSAGE _end()                                 METHOD End()

   METHOD new( nTop, nLeft, nBottom, nRight )               // constructor, NOTE: This method is a Harbour extension [vszakats]
   METHOD create( oParent, oOwner, aPos, aSize, aPresParams, lVisible )   // constructor, NOTE: This method is a Harbour extension [vszakats]
   METHOD execSlot( nEvent, p1, p2, p3 )                    // executes view events
   METHOD supplyInfo( nMode, nCall, nRole, nX, nY )                // supplies cell parameters to Qt engine
   METHOD configure( nMode )                                // mark that the internal settings of the TBrowse object should be reconfigured
   METHOD handleEvent( nEvent, mp1, mp2 )

PROTECTED:
   VAR nRowPos       AS INTEGER                   INIT 1    // current cursor row position
   VAR nColPos       AS INTEGER                   INIT 1    // current cursor column position
   VAR nLeftVisible  AS INTEGER                   INIT 0    // indicates position of leftmost unfrozen column in display
   VAR nRightVisible AS INTEGER                   INIT 0    // indicates position of rightmost unfrozen column in display
   VAR n_Row         AS INTEGER                   INIT 0    // current cursor screen row position
   VAR n_Col         AS INTEGER                   INIT 0    // current cursor screen column position
   VAR nHeadHeight   AS INTEGER                   INIT 0    // heading vertical size
   VAR nFootHeight   AS INTEGER                   INIT 0    // footing vertical size
   VAR nFrozen       AS INTEGER                   INIT 0    // number of frozen columns
   VAR nBufferPos    AS INTEGER                   INIT 1    // position in row buffer
   VAR nMoveOffset   AS INTEGER                   INIT 0    // requested repositioning
   VAR nLastRow      AS INTEGER                   INIT 0    // last row in the buffer
   VAR nLastScroll   AS INTEGER                   INIT 0    // last srcoll value
   VAR nConfigure    AS INTEGER                   INIT _TBR_CONF_ALL // configuration status
   VAR nLastPos      AS INTEGER                   INIT 0    // last calculated column position
   VAR lHitTop       AS LOGICAL                   INIT .F.  // indicates the beginning of available data
   VAR lHitBottom    AS LOGICAL                   INIT .F.  // indicates the end of available data
   VAR lHiLited      AS LOGICAL                   INIT .F.  // indicates if current cell is highlighted
   VAR lAutoLite     AS LOGICAL                   INIT .T.  // logical value to control highlighting
   VAR lStable       AS LOGICAL                   INIT .F.  // indicates if the TBrowse object is stable
   VAR lInvalid      AS LOGICAL                   INIT .T.  // indicates that TBrowse object data should be fully redrawn
   VAR lRefresh      AS LOGICAL                   INIT .F.  // indicates that record buffer should be discarded in next stabilization
   VAR lFrames       AS LOGICAL                   INIT .F.  // indicates that headings and footings should be redrawn
   VAR lHeadSep      AS LOGICAL                   INIT .F.  // indicates if heading separator exists
   VAR lFootSep      AS LOGICAL                   INIT .F.  // indicates if footing separator exists
   VAR aColData      AS ARRAY                     INIT {}   // column information, see _TBCI_*
   VAR aColors       AS ARRAY                     INIT {}   // array with TBrowse colors, see _TBC_CLR_*
   VAR aDispStatus   AS ARRAY                     INIT {}   // record buffer status
   VAR aCellStatus   AS ARRAY                     INIT {}   // record buffer status
   VAR aCellValues   AS ARRAY                     INIT {}   // cell values buffers for each record - transformed
   VAR aCellValuesA  AS ARRAY                     INIT {}   // cell values buffers for each record - actual
   VAR aCellColors   AS ARRAY                     INIT {}   // cell colors buffers for each record

   METHOD doConfigure()                                     // reconfigures the internal settings of the TBrowse object
   METHOD setUnstable()                                     // set TBrows in unstable mode resetting flags
   METHOD setPosition()                                     // synchronize record position with the buffer
   METHOD readRecord( nRow )                                // read current record into the buffer

   METHOD setVisible()                                      // set visible columns
   METHOD setCursorPos()                                    // set screen cursor position at current cell
   METHOD scrollBuffer( nRows )                             // scroll internal buffer for given row numbers
   METHOD colorValue( nColorIndex )                         // get color value for given index
   METHOD cellValue( nRow, nCol )                           // get cell value formatted
   METHOD cellValueA( nRow, nCol )                          // get cell value actual
   METHOD cellColor( nRow, nCol )                           // get cell formatted value
   METHOD dispFrames()                                      // display TBrowse border, columns' headings, footings and separators
   METHOD dispRow( nRow )                                   // display TBrowse data

   METHOD compatColor(nColor)
   METHOD compatIcon(cIcon)

   FRIEND FUNCTION _mBrwPos                                 // helper function for mRow() and mCol() methods

   DATA     oDbfModel
   DATA     oModelIndex                           INIT      QModelIndex()
   DATA     oVHeaderView
   DATA     oHeaderView                           INIT      QHeaderView()
   DATA     oVScrollBar                           INIT      QScrollBar()
   DATA     oHScrollBar                           INIT      QScrollBar()
   DATA     oViewport                             INIT      QWidget()
   DATA     oFont                                 INIT      QFont()
   DATA     pCurIndex

   DATA     lFirst                                INIT      .t.
   DATA     nRowsInView                           INIT      1

   METHOD   setHorzOffset()
   METHOD   setVertScrollBarRange( lPageStep )
   METHOD   setHorzScrollBarRange( lPageStep )
   METHOD   updateVertScrollBar()
   METHOD   updatePosition()

EXPORTED:
   METHOD   footerRbDown( p1, p2 )                SETGET
   METHOD   headerRbDown( p1, p2 )                SETGET
   METHOD   itemMarked( p1 )                      SETGET
   METHOD   itemRbDown( p1, p2 )                  SETGET
   METHOD   itemSelected( p1 )                    SETGET
   METHOD   navigate( p1, p2 )                    SETGET
   METHOD   pan( p1 )                             SETGET

   DATA     sl_xbeBRW_FooterRbDown
   DATA     sl_xbeBRW_HeaderRbDown
   DATA     sl_xbeBRW_ItemMarked
   DATA     sl_xbeBRW_ItemRbDown
   DATA     sl_xbeBRW_ItemSelected
   DATA     sl_xbeBRW_ForceStable
   DATA     sl_xbeBRW_Navigate
   DATA     sl_xbeBRW_Pan

   DATA     lHScroll                              INIT      .T.
   METHOD   hScroll                               SETGET
   DATA     lVScroll                              INIT      .T.
   METHOD   vScroll                               SETGET
   DATA     nCursorMode                           INIT      XBPBRW_CURSOR_CELL
   METHOD   cursorMode                            SETGET

   DATA     lSizeCols                             INIT      .T.
   METHOD   sizeCols                              SETGET

   DATA     softTrack                             INIT      .T.
   DATA     nHorzOffset                           INIT      -1
   DATA     lReset                                INIT      .F.
   DATA     lHorzMove                             INIT      .f.

   DATA     oTableView
   DATA     oGridLayout
   DATA     oFooterView
   DATA     oFooterModel

   DATA     oLeftView
   DATA     oLeftVHeaderView
   DATA     oLeftHeaderView
   DATA     oLeftFooterView
   DATA     oLeftFooterModel
   DATA     oLeftDbfModel

   DATA     oRightView
   DATA     oRightVHeaderView
   DATA     oRightHeaderView
   DATA     oRightFooterView
   DATA     oRightFooterModel
   DATA     oRightDbfModel

   METHOD   buildLeftFreeze()
   METHOD   buildRightFreeze()
   METHOD   fetchColumnInfo( nCall, nRole, nArea, nRow, nCol )

   METHOD   setLeftFrozen( aColFrozens )
   METHOD   setRightFrozen( aColFrozens )
   DATA     aLeftFrozen                             INIT   {}
   DATA     aRightFrozen                            INIT   {}
   DATA     nLeftFrozen                             INIT   0
   DATA     nRightFrozen                            INIT   0

   DATA     gridStyle                               INIT   Qt_SolidLine

   METHOD   destroy()
   DATA     nCellHeight                             INIT   20
   DATA     oDefaultCellSize
   METHOD   setCellHeight( nCellHeight )
   METHOD   setCurrentIndex( lReset )
   METHOD   getCurrentIndex()                       INLINE ::oDbfModel:index( ::rowPos - 1, ::colPos - 1 )
   METHOD   openPersistentEditor()

   METHOD   setFocus()                              INLINE ::oTableView:setFocus()

   ENDCLASS

/*----------------------------------------------------------------------*/

METHOD XbpBrowse:destroy()
   LOCAL i

   ::disconnect()

   ::bSkipBlock               := NIL
   ::bGoTopBlock              := NIL
   ::bGoBottomBlock           := NIL
   ::bFirstPosBlock           := NIL
   ::bLastPosBlock            := NIL
   ::bPhyPosBlock             := NIL
   ::bPosBlock                := NIL
   ::bGoPosBlock              := NIL
   ::bHitBottomBlock          := NIL
   ::bHitTopBlock             := NIL
   ::bStableBlock             := NIL

   ::sl_xbeBRW_FooterRbDown   := NIL
   ::sl_xbeBRW_HeaderRbDown   := NIL
   ::sl_xbeBRW_ItemMarked     := NIL
   ::sl_xbeBRW_ItemRbDown     := NIL
   ::sl_xbeBRW_ItemSelected   := NIL
   ::sl_xbeBRW_ForceStable    := NIL
   ::sl_xbeBRW_Navigate       := NIL
   ::sl_xbeBRW_Pan            := NIL

   FOR i := 1 TO ::colCount
      ::columns[ i ]:destroy()
      ::columns[ i ] := NIL
   NEXT

   IF !empty( ::oModelIndex )
      ::oModelIndex:pPtr      := 0
   ENDIF

   ::oHScrollBar:pPtr         := 0
   ::oVScrollBar:pPtr         := 0

   ::oLeftView:pPtr           := 0
   ::oLeftDbfModel:pPtr       := 0
   ::oLeftVHeaderView:pPtr    := 0
   ::oLeftHeaderView:pPtr     := 0
   ::oLeftFooterView:pPtr     := 0
   ::oLeftFooterModel:pPtr    := 0

   ::oRightView:pPtr          := 0
   ::oRightHeaderView:pPtr    := 0
   ::oRightDbfModel:pPtr      := 0
   ::oRightFooterView:pPtr    := 0
   ::oRightFooterModel:pPtr   := 0

   ::oTableView:pPtr          := 0
   ::oVHeaderView:pPtr        := 0
   ::oDbfModel:pPtr           := 0

   ::oFooterView:pPtr         := 0
   ::oFooterModel:pPtr        := 0

   ::oGridLayout:pPtr         := 0

   ::oGridLayout              := NIL
   ::oFooterModel             := NIL
   ::oFooterView              := NIL

   ::xbpWindow:destroy()

   RETURN nil

/*----------------------------------------------------------------------*/

/* Just to retain TBrowse functionality: in the future */
METHOD new( nTop, nLeft, nBottom, nRight ) CLASS XbpBrowse

   DEFAULT nTop    TO 0
   DEFAULT nLeft   TO 0
   DEFAULT nBottom TO MaxRow()
   DEFAULT nRight  TO MaxCol()

   ::nTop    := nTop
   ::nLeft   := nLeft
   ::nBottom := nBottom
   ::nRight  := nRight

   ::colorSpec := SetColor()

   ::oDefaultCellSize := QSize():New(20,::nCellHeight)

   RETURN Self

/*----------------------------------------------------------------------*/

METHOD XbpBrowse:openPersistentEditor()

   ::oTableView:openPersistentEditor( ::oDbfModel:index( ::rowPos - 1, ::colPos - 1 ) )

   RETURN Self

/*------------------------------------------------------------------------*/

METHOD XbpBrowse:buildLeftFreeze()

   /*  Left Freeze */
   ::oLeftView := HBQTableView():new()
   //
   ::oLeftView:setHorizontalScrollBarPolicy( Qt_ScrollBarAlwaysOff )
   ::oLeftView:setVerticalScrollBarPolicy( Qt_ScrollBarAlwaysOff )
   ::oLeftView:setTabKeyNavigation( .t. )
   ::oLeftView:setShowGrid( .t. )
   ::oLeftView:setGridStyle( ::gridStyle )   /* to be based on column definition */
   ::oLeftView:setSelectionMode( QAbstractItemView_SingleSelection )
   ::oLeftView:setSelectionBehavior( iif( ::cursorMode == XBPBRW_CURSOR_ROW, QAbstractItemView_SelectRows, QAbstractItemView_SelectItems ) )
   //
   /*  Veritical Header because of Performance boost */
   ::oLeftVHeaderView := QHeaderView()
   ::oLeftVHeaderView:configure( ::oLeftView:verticalHeader() )
   ::oLeftVHeaderView:hide()
   /*  Horizontal Header Fine Tuning */
   ::oLeftHeaderView := QHeaderView()
   ::oLeftHeaderView:configure( ::oLeftView:horizontalHeader() )
   ::oLeftHeaderView:setHighlightSections( .F. )

   ::oLeftDbfModel := HBQAbstractItemModel():new( {|t,role,x,y| ::supplyInfo( 151, t, role, x, y ) } )

   ::oLeftView:setModel( ::oLeftDbfModel )
   //
   //::oLeftView:hide()

   /*  Horizontal Footer */
   ::oLeftFooterView := QHeaderView():new( Qt_Horizontal )
   //
   ::oLeftFooterView:setHighlightSections( .F. )
   ::oLeftFooterView:setMinimumHeight( 20 )
   ::oLeftFooterView:setMaximumHeight( 20 )
   ::oLeftFooterView:setResizeMode( QHeaderView_Fixed )
   ::oLeftFooterView:setFocusPolicy( Qt_NoFocus )
   //
   ::oLeftFooterModel := HBQAbstractItemModel():new( {|t,role,x,y| ::supplyInfo( 152, t, role, x, y ) } )

   ::oLeftFooterView:setModel( ::oLeftFooterModel )
   //
   //::oLeftFooterView:hide()

   ::connect( ::oLeftView      , "mousePressEvent()"  , {|p| ::execSlot( __ev_mousepress_on_frozen__, p ) } )
   ::connect( ::oLeftHeaderView, "sectionPressed(int)", {|i| ::execSlot( __ev_mousepress_on_frozen__, i ) } )
   ::connect( ::oLeftFooterView, "sectionPressed(int)", {|i| ::execSlot( __ev_mousepress_on_frozen__, i ) } )

   RETURN Self

/*----------------------------------------------------------------------*/

METHOD XbpBrowse:buildRightFreeze()
   LOCAL oVHdr

   /*  Left Freeze */
   ::oRightView := HBQTableView():new()
   //
   ::oRightView:setHorizontalScrollBarPolicy( Qt_ScrollBarAlwaysOff )
   ::oRightView:setVerticalScrollBarPolicy( Qt_ScrollBarAlwaysOff )
   ::oRightView:setTabKeyNavigation( .t. )
   ::oRightView:setShowGrid( .t. )
   ::oRightView:setGridStyle( ::gridStyle )   /* to be based on column definition */
   ::oRightView:setSelectionMode( QAbstractItemView_SingleSelection )
   ::oRightView:setSelectionBehavior( iif( ::cursorMode == XBPBRW_CURSOR_ROW, QAbstractItemView_SelectRows, QAbstractItemView_SelectItems ) )
   //
   /*  Veritical Header because of Performance boost */
   oVHdr := QHeaderView()
   oVHdr:configure( ::oRightView:verticalHeader() )
   oVHdr:hide()
   /*  Horizontal Header Fine Tuning */
   ::oRightHeaderView := QHeaderView()
   ::oRightHeaderView:configure( ::oRightView:horizontalHeader() )
   ::oRightHeaderView:setHighlightSections( .F. )

   ::oRightDbfModel := HBQAbstractItemModel():new( {|t,role,x,y| ::supplyInfo( 161, t, role, x, y ) } )

   ::oRightView:setModel( ::oRightDbfModel )

   /*  Horizontal Footer */
   ::oRightFooterView := QHeaderView():new( Qt_Horizontal )
   //
   ::oRightFooterView:setHighlightSections( .F. )
   ::oRightFooterView:setMinimumHeight( 20 )
   ::oRightFooterView:setMaximumHeight( 20 )
   ::oRightFooterView:setResizeMode( QHeaderView_Fixed )
   ::oRightFooterView:setFocusPolicy( Qt_NoFocus )
   //
   ::oRightFooterModel := HBQAbstractItemModel():new( {|t,role,x,y| ::supplyInfo( 162, t, role, x, y ) } )

   ::oRightFooterView:setModel( ::oRightFooterModel )

   ::connect( ::oRightView      , "mousePressEvent()"  , {|p| ::execSlot( __ev_mousepress_on_frozen__, p ) } )
   ::connect( ::oRightHeaderView, "sectionPressed(int)", {|i| ::execSlot( __ev_mousepress_on_frozen__, i ) } )
   ::connect( ::oRightFooterView, "sectionPressed(int)", {|i| ::execSlot( __ev_mousepress_on_frozen__, i ) } )

   RETURN Self

/*----------------------------------------------------------------------*/

METHOD XbpBrowse:create( oParent, oOwner, aPos, aSize, aPresParams, lVisible )
   LOCAL qRect

   ::xbpWindow:create( oParent, oOwner, aPos, aSize, aPresParams, lVisible )

   ::oWidget := QFrame():new( ::pParent )
   ::oWidget:setFrameStyle( QFrame_Panel + QFrame_Plain )

   /* Important here as other parts will be based on it*/
   ::setPosAndSize()

   /* Subclass of QTableView */
   ::oTableView := HBQTableView():new()

   /* Some parameters */
   ::oTableView:setTabKeyNavigation( .t. )
   ::oTableView:setShowGrid( .t. )
   ::oTableView:setGridStyle( ::gridStyle )   /* to be based on column definition */
   ::oTableView:setSelectionMode( QAbstractItemView_SingleSelection )
   ::oTableView:setSelectionBehavior( iif( ::cursorMode == XBPBRW_CURSOR_ROW, QAbstractItemView_SelectRows, QAbstractItemView_SelectItems ) )
   ::oTableView:setAlternatingRowColors( .t. )

   /* Connect Keyboard Events */
   ::connect( ::oTableView, "keyPressEvent()"           , {|p   | ::execSlot( __ev_keypress__           , p     ) } )
   ::connect( ::oTableView, "mousePressEvent()"         , {|p   | ::execSlot( __ev_mousepress__         , p     ) } )
   ::connect( ::oTableView, "mouseDoubleClickEvent()"   , {|p   | ::execSlot( __ev_xbpBrw_itemSelected__, p     ) } )
   ::connect( ::oTableView, "wheelEvent()"              , {|p   | ::execSlot( __ev_wheel__              , p     ) } )
   ::connect( ::oTableView, "scrollContentsBy(int,int)" , {|p,p1| ::execSlot( __ev_horzscroll_via_qt__  , p, p1 ) } )

   /* Finetune Horizontal Scrollbar */
   ::oTableView:setHorizontalScrollBarPolicy( Qt_ScrollBarAlwaysOff )
   //
   ::oHScrollBar := QScrollBar():new()
   ::oHScrollBar:setOrientation( Qt_Horizontal )
   ::connect( ::oHScrollBar, "actionTriggered(int)"     , {|i| ::execSlot( __ev_horzscroll_slidermoved__   , i ) } )
   ::connect( ::oHScrollBar, "sliderReleased()"         , {|i| ::execSlot( __ev_horzscroll_sliderreleased__, i ) } )

   /*  Replace Vertical Scrollbar with our own */
   ::oTableView:setVerticalScrollBarPolicy( Qt_ScrollBarAlwaysOff )
   //
   ::oVScrollBar := QScrollBar():new()
   ::oVScrollBar:setOrientation( Qt_Vertical )
   ::connect( ::oVScrollBar, "actionTriggered(int)"     , {|i| ::execSlot( __ev_vertscroll_via_user__      , i ) } )
   ::connect( ::oVScrollBar, "sliderReleased()"         , {|i| ::execSlot( __ev_vertscroll_sliderreleased__, i ) } )

   /*  Veritical Header because of Performance boost */
   ::oVHeaderView := QHeaderView()
   ::oVHeaderView:configure( ::oTableView:verticalHeader() )
   ::oVHeaderView:hide()

   /*  Horizontal Header Fine Tuning */
   ::oHeaderView := QHeaderView()
   ::oHeaderView:configure( ::oTableView:horizontalHeader() )
   ::oHeaderView:setHighlightSections( .F. )
   //
   ::connect( ::oHeaderView, "sectionPressed(int)"        , {|i      | ::execSlot( __ev_columnheader_pressed__, i         ) } )
   ::connect( ::oHeaderView, "sectionResized(int,int,int)", {|i,i1,i2| ::execSlot( __ev_headersec_resized__   , i, i1, i2 ) } )

   /* .DBF Manipulation Model */
   ::oDbfModel := HBQAbstractItemModel():new( {|t,role,x,y| ::supplyInfo( 141, t, role, x, y ) } )

   /*  Attach Model with the View */
   ::oTableView:setModel( ::oDbfModel )

   /*  Horizontal Footer */
   ::oFooterView := QHeaderView():new( Qt_Horizontal )
   //
   ::oFooterView:setHighlightSections( .F. )

   ::oFooterView:setMinimumHeight( 20 )
   ::oFooterView:setMaximumHeight( 20 )
   ::oFooterView:setResizeMode( QHeaderView_Fixed )
   ::oFooterView:setFocusPolicy( Qt_NoFocus )
   //
   ::oFooterModel := HBQAbstractItemModel():new( {|t,role,x,y| ::supplyInfo( 142, t, role, x, y ) } )

   ::oFooterView:setModel( ::oFooterModel )
   ::oFooterView:setFocusPolicy( Qt_NoFocus )

   /*  Widget for ::setLeftFrozen( aColumns )  */
   ::buildLeftFreeze()
   /*  Widget for ::setRightFrozen( aColumns )  */
   ::buildRightFreeze()

   /* Place all widgets in a Grid Layout */
   ::oGridLayout := QGridLayout():new( ::pWidget )
   ::oGridLayout:setContentsMargins( 0,0,0,0 )
   ::oGridLayout:setHorizontalSpacing( 0 )
   ::oGridLayout:setVerticalSpacing( 0 )
   /*  Rows */
   ::oGridLayout:addWidget_1( ::oLeftView       , 0, 0, 1, 1 )
   ::oGridLayout:addWidget_1( ::oLeftFooterView , 1, 0, 1, 1 )
   //
   ::oGridLayout:addWidget_1( ::oTableView      , 0, 1, 1, 1 )
   ::oGridLayout:addWidget_1( ::oFooterView     , 1, 1, 1, 1 )
   //
   ::oGridLayout:addWidget_1( ::oRightView      , 0, 2, 1, 1 )
   ::oGridLayout:addWidget_1( ::oRightFooterView, 1, 2, 1, 1 )
   //
   ::oGridLayout:addWidget_1( ::oHScrollBar     , 2, 0, 1, 3 )
   /*  Columns */
   ::oGridLayout:addWidget_1( ::oVScrollBar     , 0, 3, 2, 1 )

   IF ::visible
      ::show()
   ENDIF
   ::oParent:AddChild( SELF )

   ::oFooterView:hide()

   /* Viewport */
   ::oViewport:configure( ::oTableView:viewport() )

   ::oWidget:installEventFilter( ::pEvents )
   ::connectEvent( ::oWidget, QEvent_Resize, {|| ::execSlot( __ev_frame_resized__ ) } )

   qRect := QRect():from( ::oWidget:geometry() )
   ::oWidget:setGeometry( qRect )

   RETURN Self

/*----------------------------------------------------------------------*/

METHOD XbpBrowse:execSlot( nEvent, p1, p2, p3 )
   LOCAL oWheelEvent, oMouseEvent, oPoint, nRowPos, nColPos // i, nCol, nColPos
   LOCAL nOff

   HB_SYMBOL_UNUSED( p2 )
   // HB_TRACE( HB_TR_DEBUG, "   XbpBrowse:execSlot:", nEvent, 0, memory( 1001 ) )

   DO CASE

   CASE nEvent == __ev_keypress__
      SetAppEvent( xbeP_Keyboard, XbpQKeyEventToAppEvent( p1 ), NIL, self )

   CASE nEvent == __ev_mousepress_on_frozen__
      ::oTableView:setFocus()

   CASE nEvent == __ev_mousepress__
      oMouseEvent := QMouseEvent():configure( p1 )

      oPoint := QPoint():new( oMouseEvent:x(), oMouseEvent:y() )
      ::oModelIndex:configure( ::oTableView:indexAt( oPoint ) )
      IF ::oModelIndex:isValid()      /* Reposition the record pointer */
         SetAppEvent( xbeBRW_Navigate, XBPBRW_Navigate_Skip, ( ::oModelIndex:row() + 1 ) - ::rowPos, Self )

         SetAppEvent( xbeBRW_Navigate, XBPBRW_Navigate_SkipCols, ( ::oModelIndex:column() + 1 ) - ::colPos, Self )
      ENDIF

      IF oMouseEvent:button() == Qt_LeftButton
         SetAppEvent( xbeBRW_ItemMarked, { ::rowPos, ::colPos }, NIL, Self )

      ELSEIF oMouseEvent:button() == Qt_RightButton
         SetAppEvent( xbeBRW_ItemRbDown, { oMouseEvent:x(), oMouseEvent:y() }, { ::rowPos, ::colPos }, Self )

      ENDIF

   CASE nEvent == __ev_xbpBrw_itemSelected__
      oMouseEvent := QMouseEvent():configure( p1 )
      IF oMouseEvent:button() == Qt_LeftButton
         SetAppEvent( xbeBRW_ItemSelected, NIL, NIL, Self )
      ENDIF

   CASE nEvent == __ev_wheel__
      oWheelEvent := QWheelEvent():configure( p1 )
      IF oWheelEvent:orientation() == Qt_Vertical
         IF oWheelEvent:delta() > 0
            SetAppEvent( xbeBRW_Navigate, XBPBRW_Navigate_Skip, -1, Self )
         ELSE
            SetAppEvent( xbeBRW_Navigate, XBPBRW_Navigate_Skip, 1, Self )
         ENDIF
      ELSE
         IF oWheelEvent:delta() > 0
            SetAppEvent( xbeBRW_Navigate, XBPBRW_Navigate_SkipCols, 1, Self )
         ELSE
            SetAppEvent( xbeBRW_Navigate, XBPBRW_Navigate_SkipCols, -1, Self )
         ENDIF
      ENDIF

   CASE nEvent == __ev_horzscroll_via_qt__
      IF p1 <> 0
         ::setHorzOffset()
      ENDIF

   CASE nEvent == __ev_vertscroll_via_user__
      SWITCH p1
      CASE QAbstractSlider_SliderNoAction
         RETURN NIL
      CASE QAbstractSlider_SliderSingleStepAdd
         SetAppEvent( xbeBRW_Navigate, XBPBRW_Navigate_Skip, 1, Self )
         ::updateVertScrollBar()
         EXIT
      CASE QAbstractSlider_SliderSingleStepSub
         SetAppEvent( xbeBRW_Navigate, XBPBRW_Navigate_Skip, -1, Self )
         ::updateVertScrollBar()
         EXIT
      CASE QAbstractSlider_SliderPageStepAdd
         SetAppEvent( xbeBRW_Navigate, XBPBRW_Navigate_NextPage, 1, Self )
         ::updateVertScrollBar()
         EXIT
      CASE QAbstractSlider_SliderPageStepSub
         SetAppEvent( xbeBRW_Navigate, XBPBRW_Navigate_PrevPage, 1, Self )
         ::updateVertScrollBar()
         EXIT
      CASE QAbstractSlider_SliderToMinimum
         SetAppEvent( xbeBRW_Navigate, XBPBRW_Navigate_GoTop, 1, Self )
         ::updateVertScrollBar()
         EXIT
      CASE QAbstractSlider_SliderToMaximum
         SetAppEvent( xbeBRW_Navigate, XBPBRW_Navigate_GoBottom, 1, Self )
         ::updateVertScrollBar()
         EXIT
      CASE QAbstractSlider_SliderMove
         ::updatePosition()
         EXIT
      ENDSWITCH
      ::oTableView:setFocus()

   CASE nEvent == __ev_vertscroll_sliderreleased__
      ::updatePosition()
      ::oTableView:setFocus()

   CASE nEvent == __ev_horzscroll_slidermoved__
      SetAppEvent( xbeBRW_Navigate, XBPBRW_Navigate_SkipCols, ( ::oHScrollBar:value() + 1 ) - ::colPos, Self )
      ::oTableView:setFocus()

   CASE nEvent == __ev_horzscroll_sliderreleased__
      SetAppEvent( xbeBRW_Navigate, XBPBRW_Navigate_SkipCols, ( ::oHScrollBar:value() + 1 ) - ::colPos, Self )
      ::oTableView:setFocus()

   CASE nEvent == __ev_columnheader_pressed__
      SetAppEvent( xbeBRW_HeaderRbDown, { 0,0 }, p1+1, Self )

   CASE nEvent == __ev_headersec_resized__
      ::oFooterView:resizeSection( p1, p3 )

   CASE nEvent == __ev_footersec_resized__
      ::oHeaderView:resizeSection( p1, p3 )

   CASE nEvent == __ev_frame_resized__
      ::oHeaderView:resizeSection( 0, ::oHeaderView:sectionSize( 0 )+1 )
      ::oHeaderView:resizeSection( 0, ::oHeaderView:sectionSize( 0 )-1 )

      nRowPos := ::rowPos()
      nColPos := ::colPos()
      //
      ::nConfigure := 9
      ::doConfigure()
      //
      ::colPos := nColPos
      IF nRowPos > ::rowCount()
         nOff := nRowPos - ::rowCount()
         ::rowPos := ::rowCount()
      ELSE
         nOff := 0
      ENDIF
      ::refreshAll()
      ::forceStable()
      ::setCurrentIndex( nRowPos > ::rowCount() )
      IF nOff > 0
         SetAppEvent( xbeBRW_Navigate, XBPBRW_Navigate_Skip, nOff, Self )
      ENDIF
   ENDCASE

   RETURN Self

/*----------------------------------------------------------------------*/

METHOD handleEvent( nEvent, mp1, mp2 ) CLASS XbpBrowse
   LOCAL i
   LOCAL lNavgt := .t.

   DO CASE
   CASE nEvent == xbeP_Keyboard
      SWITCH mp1

      CASE xbeK_UP                    /* Vertical Navigation */
         ::up()
         EXIT
      CASE xbeK_DOWN
         ::down()
         EXIT
      CASE xbeK_PGUP
         ::pageUp()
         EXIT
      CASE xbeK_PGDN
         ::pageDown()
         EXIT
      CASE xbeK_CTRL_PGUP
         ::goTop()
         EXIT
      CASE xbeK_CTRL_PGDN
         ::goBottom()
         EXIT
      CASE xbeK_RIGHT                 /* Horizontal Navigation */
         ::right()
         EXIT
      CASE xbeK_LEFT
         ::left()
         EXIT
      CASE xbeK_HOME
         ::firstCol()
         EXIT
      CASE xbeK_END
         ::lastCol()
         EXIT
      CASE xbeK_ENTER
         ::itemSelected( mp1, mp2 )
         EXIT
      OTHERWISE
         lNavgt := .f.
         EXIT
      ENDSWITCH

      IF lNavgt
         ::navigate( mp1, mp2 )
      ELSE
         ::keyboard( mp1 )
      ENDIF

   CASE nEvent == xbeBRW_ItemSelected
      ::itemSelected( mp1, mp2 )

   CASE nEvent == xbeBRW_ItemMarked
      ::itemMarked( mp1, mp2 )

   CASE nEvent == xbeBRW_ItemRbDown
      ::itemRbDown( mp1, mp2 )

   CASE nEvent == xbeBRW_ForceStable
      ::forceStable()

   CASE nEvent == xbeBRW_HeaderRbDown
      ::headerRBDown( mp1, mp2 )

   CASE nEvent == xbeBRW_Pan
      DO CASE
      CASE mp1 == XBPBRW_Pan_Left
      CASE mp1 == XBPBRW_Pan_Right
      CASE mp1 == XBPBRW_Pan_FirstCol
      CASE mp1 == XBPBRW_Pan_LastCol
      CASE mp1 == XBPBRW_Pan_Track
      ENDCASE

   CASE nEvent == xbeBRW_Navigate
      DO CASE
      CASE mp1 == XBPBRW_Navigate_NextLine
         ::down()
      CASE mp1 == XBPBRW_Navigate_PrevLine
         ::up()
      CASE mp1 == XBPBRW_Navigate_NextPage
         ::pageDown()
      CASE mp1 == XBPBRW_Navigate_PrevPage
         ::pageUp()
      CASE mp1 == XBPBRW_Navigate_GoTop
         ::goTop()
      CASE mp1 == XBPBRW_Navigate_GoBottom
         ::goBottom()
      CASE mp1 == XBPBRW_Navigate_Skip
         IF mp2 < 0
            FOR i := 1 TO abs( mp2 )
               ::up()
            NEXT
         ELSEIF mp2 > 0
            FOR i := 1 TO mp2
               ::down()
            NEXT
         ENDIF
      CASE mp1 == XBPBRW_Navigate_NextCol
         ::right()
      CASE mp1 == XBPBRW_Navigate_PrevCol
         ::left()
      CASE mp1 == XBPBRW_Navigate_FirstCol
         ::firstCol()
      CASE mp1 == XBPBRW_Navigate_LastCol
         ::lastCol()
      CASE mp1 == XBPBRW_Navigate_GoPos
         //
      CASE mp1 == XBPBRW_Navigate_SkipCols
         IF mp2 < 0
            FOR i := 1 TO abs( mp2 )
               ::left()
            NEXT
         ELSEIF mp2 > 0
            FOR i := 1 TO mp2
               ::right()
            NEXT
         ENDIF

      CASE mp1 == XBPBRW_Navigate_GotoItem
         //
      CASE mp1 == XBPBRW_Navigate_GotoRecord
         //
      OTHERWISE
         lNavgt := .f.
      ENDCASE

      IF lNavgt
         ::navigate( mp1, mp2 )
      ENDIF
   ENDCASE

   RETURN Self

/*----------------------------------------------------------------------*/

METHOD navigate( p1, p2 ) CLASS XbpBrowse

   IF hb_isBlock( p1 )
      ::sl_xbeBRW_Navigate := p1

   ELSEIF hb_isNumeric( p1 )
      /* ::handleEvent( xbeBRW_Navigate, p1, p2 ) */

      IF hb_isBlock( ::sl_xbeBRW_Navigate )
         eval( ::sl_xbeBRW_Navigate, p1, p2, self )
      ENDIF
   ENDIF

   RETURN Self

/*----------------------------------------------------------------------*/

METHOD XbpBrowse:supplyInfo( nMode, nCall, nRole, nX, nY )

   IF nCall == HBQT_QAIM_headerData .and. nX == Qt_Vertical
      RETURN Nil
   End

   DO CASE
   CASE nMode == 141       /* Main View Header|Data */
      IF nCall == HBQT_QAIM_columnCount
         IF ::colCount > 0
            ::forceStable()
            ::setHorzScrollBarRange( .t. )
         ENDIF
         RETURN ::colCount
      ELSEIF nCall == HBQT_QAIM_rowCount
         IF ::colCount > 0
            ::forceStable()
            ::setVertScrollBarRange( .f. )
         ENDIF
         RETURN ::rowCount
      ELSEIF nCall == HBQT_QAIM_data
         RETURN ::fetchColumnInfo( nCall,nRole, 0, nY+1, nX+1 )
      ELSEIF nCall == HBQT_QAIM_headerData
         RETURN ::fetchColumnInfo( nCall,nRole, 0, 0 , nY+1 )
      ENDIF
      RETURN nil

   CASE nMode == 142       /* Main View Footer */
      IF nCall == HBQT_QAIM_columnCount
         IF ::colCount > 0
            ::forceStable()
         ENDIF
         RETURN ::colCount
      ELSEIF nCall == HBQT_QAIM_data
         RETURN ::fetchColumnInfo( nCall,nRole, 1, nY+1, nX+1 )
      ELSEIF nCall == HBQT_QAIM_headerData
         RETURN ::fetchColumnInfo( nCall,nRole, 1, 0 , nY+1 )
      ENDIF
      RETURN nil

   CASE nMode == 151       /* Left Frozen Header|Data */
      IF nCall == HBQT_QAIM_columnCount
         IF ::nLeftFrozen > 0
            ::forceStable()
         ENDIF
         RETURN ::nLeftFrozen
      ELSEIF nCall == HBQT_QAIM_rowCount
         IF ::nLeftFrozen > 0
            ::forceStable()
         ENDIF
         RETURN ::rowCount
      ELSEIF nCall == HBQT_QAIM_data
         RETURN ::fetchColumnInfo( nCall,nRole, 0, nY+1, ::aLeftFrozen[ nX+1 ] )
      ELSEIF nCall == HBQT_QAIM_headerData
         RETURN ::fetchColumnInfo( nCall,nRole, 0, 0, ::aLeftFrozen[ nY+1 ] )
      ENDIF
      RETURN nil

   CASE nMode == 152       /* Left Frozen Footer */
      IF nCall == HBQT_QAIM_columnCount
         IF ::nLeftFrozen > 0
            ::forceStable()
         ENDIF
         RETURN ::nLeftFrozen
      ELSEIF nCall == HBQT_QAIM_data
         RETURN ::fetchColumnInfo( nCall,nRole, 1, nY+1, ::aLeftFrozen[ nX+1 ] )
      ELSEIF nCall == HBQT_QAIM_headerData
         RETURN ::fetchColumnInfo( nCall,nRole, 1, 0, ::aLeftFrozen[ nY+1 ] )
      ENDIF

   CASE nMode == 161       /* Right Frozen Header|Data */
      IF nCall == HBQT_QAIM_columnCount
         IF ::nRightFrozen > 0
            ::forceStable()
         ENDIF
         RETURN ::nRightFrozen
      ELSEIF nCall == HBQT_QAIM_rowCount
         IF ::nRightFrozen > 0
            ::forceStable()
         ENDIF
         RETURN ::rowCount
      ELSEIF nCall == HBQT_QAIM_data
         RETURN ::fetchColumnInfo( nCall,nRole, 0, nY+1, ::aRightFrozen[ nX+1 ] )
      ELSEIF nCall == HBQT_QAIM_headerData
         RETURN ::fetchColumnInfo( nCall,nRole, 0, 0, ::aRightFrozen[ nY+1 ] )
      ENDIF

   CASE nMode == 162       /* Right Frozen Footer */
      IF nCall == HBQT_QAIM_columnCount
         IF ::nRightFrozen > 0
            ::forceStable()
         ENDIF
         RETURN ::nRightFrozen
      ELSEIF nCall == HBQT_QAIM_data
         RETURN ::fetchColumnInfo( nCall,nRole, 1, nY+1, ::aRightFrozen[ nX+1 ] )
      ELSEIF nCall == HBQT_QAIM_headerData
         RETURN ::fetchColumnInfo( nCall,nRole, 1, 0, ::aRightFrozen[ nY+1 ] )
      ENDIF

   ENDCASE

   RETURN nil

/*----------------------------------------------------------------------*/

METHOD fetchColumnInfo( nCall, nRole, nArea, nRow, nCol ) CLASS XbpBrowse
   LOCAL aColor
   LOCAL oCol := ::columns[ nCol ]

   SWITCH nCall
   CASE HBQT_QAIM_data

      SWITCH ( nRole )
      CASE Qt_ForegroundRole
         IF hb_isBlock( oCol:colorBlock )
            aColor := eval( oCol:colorBlock, ::cellValueA( nRow, nCol ) )
            IF hb_isArray( aColor ) .and. hb_isNumeric( aColor[ 1 ] )
               RETURN ::compatColor( hbxbp_ConvertAFactFromXBP( "Color", aColor[ 1 ] ) )
            ELSE
               RETURN ::compatColor( oCol:dFgColor )
            ENDIF
         ELSE
            RETURN ::compatColor( oCol:dFgColor )
         ENDIF

      CASE Qt_BackgroundRole
         IF hb_isBlock( oCol:colorBlock )
            aColor := eval( oCol:colorBlock, ::cellValueA( nRow, nCol ) )
            IF hb_isArray( aColor ) .and. hb_isNumeric( aColor[ 2 ] )
               RETURN ::compatColor( hbxbp_ConvertAFactFromXBP( "Color", aColor[ 2 ] ) )
            ELSE
               RETURN ::compatColor( oCol:dBgColor )
            ENDIF
         ELSE
            RETURN ::compatColor( oCol:dBgColor )
         ENDIF

      CASE Qt_TextAlignmentRole
         RETURN oCol:dAlignment

      CASE Qt_SizeHintRole
         RETURN ::oDefaultCellSize

      CASE Qt_DecorationRole
         IF oCol:type == XBPCOL_TYPE_FILEICON
            RETURN ::compatIcon( ::cellValue( nRow, nCol ) )
         ELSE
            RETURN nil
         ENDIF
      CASE Qt_EditRole
      CASE Qt_DisplayRole
         IF oCol:type == XBPCOL_TYPE_FILEICON
            RETURN nil
         ELSE
            RETURN ::cellValue( nRow, nCol )
         ENDIF
      ENDSWITCH
      RETURN nil

   CASE HBQT_QAIM_headerData
      IF nArea == 0                    /* Header Area */
         SWITCH nRole
         CASE Qt_SizeHintRole
            RETURN ::oDefaultCellSize //oCol:hHeight
         CASE Qt_DisplayRole
            RETURN oCol:heading
         CASE Qt_TextAlignmentRole
            RETURN oCol:hAlignment
         CASE Qt_ForegroundRole
            RETURN ::compatColor( oCol:hFgColor )
         CASE Qt_BackgroundRole
            RETURN ::compatColor( oCol:hBgColor )
         ENDSWITCH
      ELSE                             /* Footer Area */
         SWITCH nRole
         CASE Qt_SizeHintRole
            RETURN ::oDefaultCellSize //oCol:fHeight
         CASE Qt_DisplayRole
            RETURN oCol:footing
         CASE Qt_TextAlignmentRole
            RETURN oCol:fAlignment
         CASE Qt_ForegroundRole
            RETURN ::compatColor( oCol:fFgColor )
         CASE Qt_BackgroundRole
            RETURN ::compatColor( oCol:fBgColor )
         ENDSWITCH
      ENDIF
      RETURN nil
   ENDSWITCH

   RETURN nil

/*----------------------------------------------------------------------*/
//
// TODO: Review the color < 25 case when resolved in HBQt, and avoid unnecessary creation of new QColor/Qicon GC objects
//
//   However, tested with medium data sets, seems not being a big issue by now
//   Implementation choice will depend on planned HBQt evolution of pseudo casts and bypass functions (non GC QColor, QIcon, etc)

METHOD compatColor( nColor )
   RETURN QColor():new( nColor )

/*----------------------------------------------------------------------*/

METHOD compatIcon( cIcon )
   RETURN QIcon():new( QPixmap():new( Trim( cIcon ) ) )

/*----------------------------------------------------------------------*/

METHOD setVertScrollBarRange( lPageStep ) CLASS XbpBrowse
   LOCAL nMin, nMax

   DEFAULT lPageStep TO .f.

   IF hb_isNumeric( nMin := eval( ::bFirstPosBlock  ) ) .and. hb_isNumeric( nMax := eval( ::bLastPosBlock ) )
      ::oVScrollBar:setMinimum( nMin - 1 )
      ::oVScrollBar:setMaximum( nMax - 1 )
      ::oVScrollBar:setSingleStep( 1 )
      //
      IF lPageStep
         ::oVScrollBar:setPageStep( ::rowCount() )
      ENDIF
   ENDIF

   RETURN Self

/*----------------------------------------------------------------------*/

METHOD setHorzScrollBarRange( lPageStep ) CLASS XbpBrowse

   DEFAULT lPageStep TO .f.

   ::oHScrollBar:setMinimum( 0 )
   ::oHScrollBar:setMaximum( ::colCount - 1 )
   ::oHScrollBar:setSingleStep( 1 )
   ::oHScrollBar:setPageStep( 1 )

   RETURN Self

/*----------------------------------------------------------------------*/

METHOD updatePosition() CLASS XbpBrowse

   IF hb_isBlock( ::goPosBlock )
      eval( ::goPosBlock, ::oVScrollBar:value() + 1 )
      ::refreshAll()
      ::forceStable()
      ::setCurrentIndex()
   ENDIF

   RETURN Self

/*----------------------------------------------------------------------*/

METHOD updateVertScrollBar() CLASS XbpBrowse

   ::oVScrollBar:setValue( eval( ::posBlock ) - 1 )

   RETURN Self

/*----------------------------------------------------------------------*/

METHOD setHorzOffset() CLASS XbpBrowse

   IF ::colPos == ::colCount
      ::oHeaderView:setOffsetToLastSection()
      ::oFooterView:setOffsetToLastSection()
   ELSE
      ::oHeaderView:setOffsetToSectionPosition( ::colPos - 1 )
      ::oFooterView:setOffsetToSectionPosition( ::colPos - 1 )
   ENDIF

   RETURN Self

/*----------------------------------------------------------------------*/

METHOD setCurrentIndex( lReset ) CLASS XbpBrowse

   DEFAULT lReset TO .t.

   IF lReset
      ::oDbfModel:reset()                         /* Important */
      //
      IF hb_isObject( ::oLeftDbfModel )
         ::oLeftDbfModel:reset()
      ENDIF
      IF hb_isObject( ::oRightDbfModel )
         ::oRightDbfModel:reset()
      ENDIF
   ENDIF
   ::oTableView:setCurrentIndex( ::oDbfModel:index( ::rowPos - 1, ::colPos - 1 ) )

   RETURN Self

/*----------------------------------------------------------------------*/

METHOD XbpBrowse:hScroll( lYes )

   IF hb_isLogical( lYes )
      ::lHScroll := lYes
      ::setUnstable()
      ::configure( 128 )
   ENDIF

   RETURN ::lHScroll

/*----------------------------------------------------------------------*/

METHOD XbpBrowse:vScroll( lYes )

   IF hb_isLogical( lYes )
      ::lVScroll := lYes
      ::setUnstable()
      ::configure( 128 )
   ENDIF

   RETURN ::lHScroll

/*----------------------------------------------------------------------*/

METHOD XbpBrowse:sizeCols( lYes )

   IF hb_isLogical( lYes )
      ::lSizeCols := lYes
      ::setUnstable()
      ::configure( 128 )
   ENDIF

   RETURN Self

/*----------------------------------------------------------------------*/

METHOD XbpBrowse:cursorMode( nMode )

   IF hb_isNumeric( nMode )
      ::nCursorMode := nMode
      ::setUnstable()
      ::configure( 128 )
   ENDIF

   RETURN ::nCursorMode

/*----------------------------------------------------------------------*/

METHOD XbpBrowse:setRightFrozen( aColFrozens )
   LOCAL aFrozen := aclone( ::aRightFrozen )

   IF hb_isArray( aColFrozens )
      ::aRightFrozen := aColFrozens
      ::nRightFrozen := len( ::aRightFrozen )
      ::setUnstable()
      ::configure( 128 )
      ::forceStable()
   ENDIF

   RETURN aFrozen

/*----------------------------------------------------------------------*/

METHOD XbpBrowse:setLeftFrozen( aColFrozens )
   LOCAL aFrozen := aclone( ::aLeftFrozen )

   IF hb_isArray( aColFrozens )
      ::aLeftFrozen := aColFrozens
      ::nLeftFrozen := len( ::aLeftFrozen )
      ::setUnstable()
      ::configure( 128 )
      ::forceStable()
   ENDIF

   RETURN aFrozen

/*----------------------------------------------------------------------*/

METHOD setCellHeight( nCellHeight ) CLASS XbpBrowse
   LOCAL i

   FOR i := 1 TO ::nRowsInView
      ::oTableView : setRowHeight( i-1, nCellHeight )
      IF !empty( ::oLeftView )
         ::oLeftView  : setRowHeight( i-1, nCellHeight )
      ENDIF
      IF !empty( ::oRightView )
         ::oRightView : setRowHeight( i-1, nCellHeight )
      ENDIF
   NEXT
   RETURN Self

/*------------------------------------------------------------------------*/

METHOD doConfigure() CLASS XbpBrowse
   LOCAL oCol
   LOCAL aCol, aVal, aValA
   LOCAL nWidth, nHeight, nColCount, nRowCount
   LOCAL xValue
   LOCAL cType
   LOCAL cColSep
   LOCAL cHeadSep, cHeading
   LOCAL nHeadHeight
   LOCAL cFootSep, cFooting
   LOCAL nFootHeight
   LOCAL lHeadSep, lFootSep
   LOCAL nMaxCellH := 0
   LOCAL nViewH, i, xVal, oFontMetrics, n, nLeftWidth

   ::nConfigure := 0

   nColCount := Len( ::columns )

   /* update color table */
   ::aColors := _DECODECOLORS( ::cColorSpec )

   /* update column data */
   nHeadHeight := nFootHeight := 0
   lHeadSep    := lFootSep := .F.
   ASize( ::aColData, nColCount )

   FOR EACH oCol, aCol IN ::columns, ::aColData
      xValue   := Eval( oCol:block )
      cType    := ValType( xValue )
      nWidth   := iif( cType $ "CMNDTL", Len( Transform( xValue, oCol:picture ) ), 0 )

      /* Control the picture spec of Bitmaps|Icon files */
      IF oCol:type != XBPCOL_TYPE_TEXT
         nWidth := 256
      ENDIF

      cColSep  := ""
      cHeadSep := ""
      cFootSep := ""

      aCol := Array( _TBCI_SIZE )
      aCol[ _TBCI_COLOBJECT   ] := oCol
      aCol[ _TBCI_COLWIDTH    ] := nWidth
      aCol[ _TBCI_COLPOS      ] := NIL
      aCol[ _TBCI_CELLWIDTH   ] := nWidth
      aCol[ _TBCI_CELLPOS     ] := 0
      aCol[ _TBCI_COLSEP      ] := cColSep
      aCol[ _TBCI_SEPWIDTH    ] := Len( cColSep )
      aCol[ _TBCI_HEADSEP     ] := cHeadSep
      aCol[ _TBCI_FOOTSEP     ] := cFootSep
      aCol[ _TBCI_DEFCOLOR    ] := _COLDEFCOLORS( oCol:defColor, Len( ::aColors ) )
      aCol[ _TBCI_FROZENSPACE ] := 0
      aCol[ _TBCI_LASTSPACE   ] := 0
      IF Len( cHeadSep ) > 0
         lHeadSep := .T.
      ENDIF
      IF Len( cFootSep ) > 0
         lFootSep := .T.
      ENDIF
      cHeading := oCol:heading
      IF _DECODE_FH( @cHeading, @nHeight, @nWidth )
         aCol[ _TBCI_COLWIDTH ] := Max( aCol[ _TBCI_COLWIDTH ], nWidth )
         IF nHeight > nHeadHeight
            nHeadHeight := nHeight
         ENDIF
      ENDIF

      aCol[ _TBCI_HEADING ] := cHeading
      cFooting := oCol:footing
      IF _DECODE_FH( @cFooting, @nHeight, @nWidth )
         aCol[ _TBCI_COLWIDTH ] := Max( aCol[ _TBCI_COLWIDTH ], nWidth )
         IF nHeight > nFootHeight
            nFootHeight := nHeight
         ENDIF
      ENDIF
      aCol[ _TBCI_FOOTING ] := cFooting
      nWidth := oCol:width
      IF nWidth != NIL
         IF nWidth > 0
            aCol[ _TBCI_COLWIDTH ] := nWidth
            IF nWidth < aCol[ _TBCI_CELLWIDTH ] .OR. cType == "C"
               aCol[ _TBCI_CELLWIDTH ] := nWidth
            ENDIF
         ELSE
            aCol[ _TBCI_CELLWIDTH ] := 0
         ENDIF
      ENDIF
      IF aCol[ _TBCI_CELLWIDTH ] > 0
         IF aCol[ _TBCI_COLWIDTH ] > aCol[ _TBCI_CELLWIDTH ]
            IF cType == "N"
               aCol[ _TBCI_CELLPOS ] := aCol[ _TBCI_COLWIDTH ] - aCol[ _TBCI_CELLWIDTH ]
            ELSEIF cType == "L"
               aCol[ _TBCI_CELLPOS ] := Int( ( aCol[ _TBCI_COLWIDTH ] - aCol[ _TBCI_CELLWIDTH ] ) / 2 )
            ENDIF
         ENDIF
      ENDIF
   NEXT

   nHeight := Max( _TBR_COORD( ::n_Bottom ) - _TBR_COORD( ::n_Top ), 0 )
   IF lHeadSep .AND. nHeight > 0
      --nHeight
   ELSE
      lHeadSep := .F.
   ENDIF
   IF lFootSep .AND. nHeight > 0
      --nHeight
   ELSE
      lFootSep := .F.
   ENDIF
   IF nHeadHeight >= nHeight
      nHeadHeight := nHeight
      nHeight := 0
   ELSE
      nHeight -= nHeadHeight
   ENDIF
   IF nFootHeight >= nHeight
      nFootHeight := nHeight
      nHeight := 0
   ENDIF
   ::lHeadSep    := .f.  // lHeadSep
   ::nHeadHeight := nHeadHeight
   ::lFootSep    := .f.  // lFootSep
   ::nFootHeight := nFootHeight

   /* update headings to maximum size and missing head/foot separators */
   FOR EACH aCol IN ::aColData
      aCol[ _TBCI_HEADING ] := Replicate( _TBR_CHR_LINEDELIMITER, nHeadHeight - ;
             hb_TokenCount( aCol[ _TBCI_HEADING ], _TBR_CHR_LINEDELIMITER ) ) + aCol[ _TBCI_HEADING ]
      IF lHeadSep .AND. aCol[ _TBCI_HEADSEP ] == ""
         aCol[ _TBCI_HEADSEP ] := " "
      ENDIF
      IF lFootSep .AND. aCol[ _TBCI_FOOTSEP ] == ""
         aCol[ _TBCI_FOOTSEP ] := " "
      ENDIF
   NEXT

   /* Qt Specifics follows */
   //
   IF !( ::lHScroll )
      ::oHScrollBar:hide()
   ENDIF
   IF !( ::lVScroll )
      ::oVScrollBar:hide()
   ENDIF

   ::oTableView:setSelectionBehavior( iif( ::cursorMode == XBPBRW_CURSOR_ROW, QAbstractItemView_SelectRows, QAbstractItemView_SelectItems ) )

   /* Calculate how many rows fit in the view */
   IF len( ::columns ) > 0
      nMaxCellH := 0
      aeval( ::columns, {|o| nMaxCellH := max( nMaxCellH, o:hHeight ) } )
      //
      ::oHeaderView:setMaximumHeight( nMaxCellH )
      ::oHeaderView:setMinimumHeight( nMaxCellH )
      //
      ::oLeftHeaderView:setMaximumHeight( nMaxCellH )
      ::oLeftHeaderView:setMinimumHeight( nMaxCellH )
      //
      ::oRightHeaderView:setMaximumHeight( nMaxCellH )
      ::oRightHeaderView:setMinimumHeight( nMaxCellH )

      nMaxCellH := 0
      aeval( ::columns, {|o| nMaxCellH := max( nMaxCellH, o:fHeight ) } )
      //
      ::oFooterView:setMaximumHeight( nMaxCellH )
      //
      ::oLeftFooterView:setMaximumHeight( nMaxCellH )
      //
      ::oRightFooterView:setMaximumHeight( nMaxCellH )

      nMaxCellH := 0
      aeval( ::columns, {|o| nMaxCellH := max( nMaxCellH, o:dHeight ) } )
      //
      nViewH := ::oViewport:height() //- ::oHeaderView:height()
      ::nRowsInView := Int( nViewH / nMaxCellH )
      IF ( nViewH % nMaxCellH ) > ( nMaxCellH / 2 )
         ::nRowsInView++
      ENDIF

      /* Probably this is the appropriate time to update row heights */
      ::nCellHeight := nMaxCellH
      ::setCellHeight( nMaxCellH )

      /* Implement Column Resizing Mode */
      ::oHeaderView:setResizeMode( iif( ::lSizeCols, QHeaderView_Interactive, QHeaderView_Fixed ) )
      ::oFooterView:setResizeMode( QHeaderView_Fixed )
      //
      ::oLeftHeaderView:setResizeMode( QHeaderView_Fixed )
      ::oLeftFooterView:setResizeMode( QHeaderView_Fixed )
      //
      ::oRightHeaderView:setResizeMode( QHeaderView_Fixed )
      ::oRightFooterView:setResizeMode( QHeaderView_Fixed )

      /* Set column widths */
      oFontMetrics := QFontMetrics():new( "QFont", ::oTableView:font() )
      //
      FOR i := 1 TO len( ::columns )
         IF ::columns[ i ]:nColWidth != NIL
            ::oHeaderView:resizeSection( i-1, ::columns[ i ]:nColWidth )
            ::oFooterView:resizeSection( i-1, ::columns[ i ]:nColWidth )
         ELSE
            xVal := transform( eval( ::columns[ i ]:block ), ::columns[ i ]:picture )
            ::oHeaderView:resizeSection( i-1, oFontMetrics:width( xVal, -1 ) + 8 )
            ::oFooterView:resizeSection( i-1, oFontMetrics:width( xVal, -1 ) + 8 )
         ENDIF
      NEXT

      nLeftWidth := 0
      FOR n := 1 TO ::nLeftFrozen
         i := ::aLeftFrozen[ n ]
         IF ::columns[ i ]:nColWidth != NIL
            ::oLeftHeaderView:resizeSection( n-1, ::columns[ i ]:nColWidth )
            ::oLeftFooterView:resizeSection( n-1, ::columns[ i ]:nColWidth )
         ELSE
            xVal := transform( eval( ::columns[ i ]:block ), ::columns[ i ]:picture )
            ::oLeftHeaderView:resizeSection( n-1, oFontMetrics:width( xVal, -1 ) + 8 )
            ::oLeftFooterView:resizeSection( n-1, oFontMetrics:width( xVal, -1 ) + 8 )
         ENDIF
         nLeftWidth += ::oLeftHeaderView:sectionSize( n-1 )
      NEXT
      ::oLeftView:setFixedWidth( 4 + nLeftWidth )
      //::oLeftHeaderView:setFixedWidth( nLeftWidth )
      ::oLeftFooterView:setFixedWidth( 4 + nLeftWidth )

      nLeftWidth := 0
      FOR n := 1 TO ::nRightFrozen
         i := ::aRightFrozen[ n ]
         IF ::columns[ i ]:nColWidth != NIL
            ::oRightHeaderView:resizeSection( n-1, ::columns[ i ]:nColWidth )
            ::oRightFooterView:resizeSection( n-1, ::columns[ i ]:nColWidth )
         ELSE
            xVal := transform( eval( ::columns[ i ]:block ), ::columns[ i ]:picture )
            ::oRightHeaderView:resizeSection( n-1, oFontMetrics:width( xVal, -1 ) + 8 )
            ::oRightFooterView:resizeSection( n-1, oFontMetrics:width( xVal, -1 ) + 8 )
         ENDIF
         nLeftWidth += ::oRightHeaderView:sectionSize( n-1 )
      NEXT
      ::oRightView:setFixedWidth( 4 + nLeftWidth )
      //::oRightHeaderView:setFixedWidth( nLeftWidth )
      ::oRightFooterView:setFixedWidth( 4 + nLeftWidth )

   ENDIF

   IF ::nLeftFrozen == 0 .AND. hb_isObject( ::oLeftView )
      ::oLeftView:hide()
      ::oLeftFooterView:hide()
   ELSEIF ::nLeftFrozen > 0 .AND. hb_isObject( ::oLeftView )
      ::oLeftView:show()
      ::oLeftFooterView:show()
   ENDIF
   IF ::nRightFrozen == 0 .AND. hb_isObject( ::oRightView )
      ::oRightView:hide()
      ::oRightFooterView:hide()
   ELSEIF ::nRightFrozen > 0 .AND. hb_isObject( ::oRightView )
      ::oRightView:show()
      ::oRightFooterView:show()
   ENDIF

   FOR i := 1 TO ::colCount
      IF ISFROZEN( i )
         ::oTableView:hideColumn( i - 1 )
         ::oFooterView:setSectionHidden( i - 1, .t. )
      ELSE
         ::oTableView:showColumn( i - 1 )
         ::oFooterView:setSectionHidden( i - 1, .f. )
      ENDIF
   NEXT


   nRowCount := ::rowCount
   IF nRowCount == 0
      _GENLIMITRTE()
   ENDIF

   /* create new record buffer */
   ASize( ::aCellStatus , nRowCount )
   ASize( ::aDispStatus , nRowCount )
   ASize( ::aCellValues , nRowCount )
   ASize( ::aCellValuesA, nRowCount )
   ASize( ::aCellColors , nRowCount )
   AFill( ::aCellStatus , .F. )
   AFill( ::aDispStatus , .T. )
   FOR EACH aVal, aValA, aCol IN ::aCellValues, ::aCellValuesA, ::aCellColors
      IF aVal == NIL
         aVal := Array( nColCount )
      ELSE
         ASize( aVal, nColCount )
      ENDIF
      IF aValA == NIL
         aValA := Array( nColCount )
      ELSE
         ASize( aValA, nColCount )
      ENDIF
      IF aCol == NIL
         aCol := Array( nColCount )
      ELSE
         ASize( aCol, nColCount )
      ENDIF
   NEXT

   ::lStable := .F.
   ::lFrames := .T.
   ::nLastRow := nRowCount
   ::nLastScroll := 0
   ::nLastPos := 0
   IF ::nRowPos > nRowCount
      ::nRowPos := nRowCount
   ELSEIF ::nRowPos < 1
      ::nRowPos := 1
   ENDIF

   ::setHorzScrollBarRange()

   ::setCellHeight( ::nCellHeight )

   /* Inform Qt about number of rows and columns browser implements */
   //::oDbfModel:hbSetRowColumns( ::rowCount - 1, ::colCount - 1 )
   /* Tell Qt to Reload Everything */
   ::oDbfModel:reset()
   //
   IF hb_isObject( ::oLeftDbfModel )
      //::oLeftDbfModel:hbSetRowColumns( ::rowCount - 1, ::nLeftFrozen - 1 ) // Dangling code
      ::oLeftDbfModel:reset()
   ENDIF
   IF hb_isObject( ::oRightDbfModel )
      //::oRightDbfModel:hbSetRowColumns( ::rowCount - 1, ::nRightFrozen - 1 )
      ::oRightDbfModel:reset()
   ENDIF

   RETURN Self

/*----------------------------------------------------------------------*/

METHOD footerRbDown( p1, p2 ) CLASS XbpBrowse
   IF hb_isBlock( p1 )
      ::sl_xbeBRW_FooterRbDown := p1
   ENDIF
   IF hb_isArray( p1 ) .and. hb_isBlock( ::sl_xbeBRW_FooterRbDown )
      eval( ::sl_xbeBRW_FooterRbDown, p1, p2, self )
   ENDIF
   RETURN Self

/*----------------------------------------------------------------------*/

METHOD headerRbDown( p1, p2 ) CLASS XbpBrowse
   IF hb_isBlock( p1 )
      ::sl_xbeBRW_HeaderRbDown := p1
   ENDIF
   IF hb_isArray( p1 ) .and. hb_isBlock( ::sl_xbeBRW_HeaderRbDown )
      eval( ::sl_xbeBRW_HeaderRbDown, p1, p2, self )
   ENDIF
   RETURN Self

/*----------------------------------------------------------------------*/

METHOD itemMarked( p1 ) CLASS XbpBrowse
   IF hb_isBlock( p1 )
      ::sl_xbeBRW_ItemMarked := p1
   ENDIF
   IF hb_isArray( p1 ) .and. hb_isBlock( ::sl_xbeBRW_ItemMarked )
      eval( ::sl_xbeBRW_ItemMarked, p1, NIL, self )
   ENDIF
   RETURN Self

/*----------------------------------------------------------------------*/

METHOD itemRbDown( p1, p2 ) CLASS XbpBrowse
   IF hb_isBlock( p1 )
      ::sl_xbeBRW_ItemRbDown := p1
   ENDIF
   IF hb_isArray( p1 ) .and. hb_isBlock( ::sl_xbeBRW_ItemRbDown )
      eval( ::sl_xbeBRW_ItemRbDown, p1, p2, self )
   ENDIF
   RETURN Self

/*----------------------------------------------------------------------*/

METHOD itemSelected( p1 ) CLASS XbpBrowse

   IF hb_isBlock( p1 )
      ::sl_xbeBRW_ItemSelected := p1

   ELSEIF hb_isBlock( ::sl_xbeBRW_ItemSelected )
      eval( ::sl_xbeBRW_ItemSelected, NIL, NIL, self )

   ENDIF

   RETURN Self

/*----------------------------------------------------------------------*/

METHOD pan( p1 ) CLASS XbpBrowse

   IF hb_isBlock( p1 )
      ::sl_xbeBRW_Pan := p1

   ELSEIF hb_isNumeric( p1 )
      ::handleEvent( xbeBRW_Pan, p1, NIL )

      IF hb_isBlock( ::sl_xbeBRW_Pan )
         eval( ::sl_xbeBRW_Pan, p1, NIL, self )
      ENDIF
   ENDIF

   RETURN Self

/*----------------------------------------------------------------------*/

STATIC FUNCTION _SKIP_RESULT( xResult )
   RETURN iif( ISNUMBER( xResult ), Int( xResult ), 0 )

/*----------------------------------------------------------------------*/

METHOD configure( nMode ) CLASS XbpBrowse

   IF !ISNUMBER( nMode ) .OR. nMode == 0 .OR. nMode > _TBR_CONF_ALL
      nMode := _TBR_CONF_ALL
   ENDIF
   ::nConfigure := HB_BITOR( ::nConfigure, nMode )

   RETURN Self

/*----------------------------------------------------------------------*/

METHOD forceStable() CLASS XbpBrowse
   DO WHILE !::stabilize() ; ENDDO
   RETURN Self

/*----------------------------------------------------------------------*/

METHOD stabilize() CLASS XbpBrowse
   LOCAL nRowCount, nToMove, nMoved
   LOCAL lDisp, lRead, lStat

   IF ::nConfigure != 0
      ::doConfigure()
   ENDIF

   IF !::lStable .OR. ::lInvalid .OR. ::lFrames .OR. ::lRefresh .OR. ;
                     ::nMoveOffset != 0 .OR. ::nBufferPos != ::nRowPos

      nRowCount := ::rowCount

      IF ::lRefresh
         AFill( ::aCellStatus, .F. )
         ::nLastRow    := nRowCount
         ::nLastScroll := 0
         ::lRefresh    := .F.
      ENDIF

      ::setVisible()

      IF ::lFrames
         ::dispFrames()
         AFill( ::aDispStatus, .T. )
      ENDIF

      lRead := .F.
      IF ::nMoveOffset != 0
         ::setPosition()
         lRead := .T.
      ENDIF

      IF ::nLastScroll > 0
         FOR EACH lStat, lDisp IN ::aCellStatus, ::aDispStatus DESCEND
            IF !lStat
               IF lRead
                  RETURN .F.
               ENDIF
               lRead := ::readRecord( lStat:__enumIndex() )
            ENDIF
            IF lDisp
               ::dispRow( lDisp:__enumIndex() )
            ENDIF
         NEXT
      ELSE
         FOR EACH lStat, lDisp IN ::aCellStatus, ::aDispStatus
            IF !lStat
               IF lRead
                  RETURN .F.
               ENDIF
               lRead := ::readRecord( lStat:__enumIndex() )
            ENDIF
            IF lDisp
               ::dispRow( lDisp:__enumIndex() )
            ENDIF
         NEXT
      ENDIF

      /* We reach here when browse is stable : synchronize the record pointer with ::rowPos */
      IF ::nRowPos > ::nLastRow
         ::nRowPos := ::nLastRow
      ENDIF
      IF ::nBufferPos != ::nRowPos
         nToMove := ::nRowPos - ::nBufferPos

         nMoved := _SKIP_RESULT( Eval( ::bSkipBlock, nToMove ) )

         IF nToMove > 0
            IF nMoved < 0
               nMoved := 0
            ENDIF
         ELSEIF nToMove < 0
            nMoved := nToMove
         ELSE
            nMoved := 0
         ENDIF
         ::nBufferPos += nMoved
         ::nRowPos := ::nBufferPos
      ENDIF

      ::lStable := .T.
      ::lInvalid := .F.

   ENDIF

   IF ::autoLite
      ::hilite()
   ELSE
      ::setCursorPos()
   ENDIF

   IF ::lStable
      IF hb_isBlock( ::bStableBlock )
         eval( ::bStableBlock( Self ) )
      ENDIF
   ENDIF

   RETURN .T.

/*----------------------------------------------------------------------*/

METHOD setPosition() CLASS XbpBrowse
   LOCAL nMoved
   LOCAL nRowCount   := ::rowCount
   LOCAL nMoveOffset := ::nMoveOffset + ( ::nRowPos - ::nBufferPos )
   LOCAL nNewPos     := ::nBufferPos + nMoveOffset
   LOCAL lSetPos     := .T.

   IF nNewPos < 1
      IF ::nMoveOffset < -1
         nMoveOffset -= ::nRowPos - 1
      ENDIF
   ELSEIF nNewPos > ::nLastRow
      IF ::nMoveOffset > 1
         nMoveOffset += ::nLastRow - ::nRowPos
      ENDIF
   ELSEIF lSetPos
      ::nRowPos := nNewPos
   ENDIF

   nMoved := _SKIP_RESULT( Eval( ::bSkipBlock, nMoveOffset ) )
   IF nMoved > 0
      ::nBufferPos += nMoved
      IF ::nBufferPos > ::nLastRow
         AFill( ::aCellStatus, .F., ::nLastRow + 1, ::nBufferPos - ::nLastRow )
      ENDIF
      IF ::nBufferPos > nRowCount
         ::scrollBuffer( ::nBufferPos - nRowCount )
         ::nBufferPos := nRowCount
         lSetPos := .F.
      ENDIF
      IF ::nBufferPos > ::nLastRow
         ::nLastRow := ::nBufferPos
         IF nMoved != nMoveOffset
            lSetPos := .F.
         ENDIF
      ENDIF
   ELSEIF nMoved < 0
      ::nBufferPos += nMoved
      IF ::nBufferPos < 1
         ::nLastRow := Min( nRowCount, ::nLastRow - ::nBufferPos + 1 )
         ::scrollBuffer( ::nBufferPos - 1 )
         ::nBufferPos := 1
         lSetPos := .F.
      ENDIF
   ELSE  /* nMoved == 0 */
      IF nMoveOffset > 0
         IF nMoveOffset != 0 .AND. ::nBufferPos == ::nRowPos
            IF hb_isBlock( ::bHitBottomBlock )
               eval( ::bHitBottomBlock, Self )
            ENDIF
            ::lHitBottom := .T.
         ENDIF
         ::nLastRow := ::nBufferPos
         AFill( ::aCellStatus, .F., ::nLastRow + 1 )
      ELSEIF nMoveOffset < 0
         IF nMoveOffset != 0 .AND. ::nBufferPos == ::nRowPos
            IF hb_isBlock( ::bHitTopBlock )
               eval( ::bHitTopBlock, Self )
            ENDIF
            ::lHitTop := .T.
         ENDIF
         IF ::nBufferPos > 1
            ::scrollBuffer( ::nBufferPos - 1 )
            ::nBufferPos := 1
         ENDIF
      ENDIF
   ENDIF

   IF lSetPos
      ::nRowPos := ::nBufferPos
   ENDIF

   ::nMoveOffset := 0

   RETURN Self

/*----------------------------------------------------------------------*/

METHOD scrollBuffer( nRows ) CLASS XbpBrowse
   LOCAL nRowCount := ::rowCount
   LOCAL aValues, aValuesA, aColors

   /* Store last scroll value to chose refresh order. [druzus] */
   ::nLastScroll := nRows

   IF nRows >= nRowCount .OR. nRows <= -nRowCount
      AFill( ::aCellStatus, .F. )
      ::lReset := .t.
   ELSE
      IF nRows > 0
         DO WHILE --nRows >= 0
            aValues := ::aCellValues[ 1 ]
            aValuesA := ::aCellValuesA[ 1 ]
            aColors := ::aCellColors[ 1 ]
            ADel( ::aCellValues, 1 )
            ADel( ::aCellValuesA, 1 )
            ADel( ::aCellColors, 1 )
            ADel( ::aCellStatus, 1 )
            ADel( ::aDispStatus, 1 )
            ::aCellValues[ nRowCount ] := aValues
            ::aCellValuesA[ nRowCount ] := aValuesA
            ::aCellColors[ nRowCount ] := aColors
            ::aCellStatus[ nRowCount ] := .F.
            ::aDispStatus[ nRowCount ] := .T.
         ENDDO
         ::lReset := .t.
      ELSEIF nRows < 0
         DO WHILE ++nRows <= 0
            HB_AIns( ::aCellValues, 1, ATail( ::aCellValues ), .F. )
            HB_AIns( ::aCellValuesA, 1, ATail( ::aCellValuesA ), .F. )
            HB_AIns( ::aCellColors, 1, ATail( ::aCellColors ), .F. )
            HB_AIns( ::aCellStatus, 1, .F., .F. )
            HB_AIns( ::aDispStatus, 1, .T., .F. )
         ENDDO
         ::lReset := .t.
      ENDIF
   ENDIF

   RETURN Self

/*----------------------------------------------------------------------*/

METHOD readRecord( nRow ) CLASS XbpBrowse
   LOCAL aCol
   LOCAL oCol
   LOCAL cValue, cValueA
   LOCAL aColor
   LOCAL nColors, nToMove, nMoved
   LOCAL nRowCount := ::rowCount
   LOCAL lRead := .F.

   IF nRow >= 1 .AND. nRow <= nRowCount .AND. !::aCellStatus[ nRow ]
      IF nRow <= ::nLastRow
         nToMove := nRow - ::nBufferPos
         nMoved  := _SKIP_RESULT( Eval( ::bSkipBlock, nToMove ) )
         IF nToMove > 0
            IF nMoved < 0
               nMoved := 0
            ENDIF
         ELSEIF nToMove < 0
            nMoved := nToMove
         ELSE
            nMoved := 0
         ENDIF
         ::nBufferPos += nMoved
         IF nToMove > 0 .AND. nMoved < nToMove
            ::nLastRow := ::nBufferPos
         ELSE
            lRead := .T.
         ENDIF
      ENDIF
      nColors := Len( ::aColors )
      IF nRow <= ::nLastRow
         FOR EACH aCol, cValue, cValueA, aColor IN ::aColData, ::aCellValues[ nRow ], ::aCellValuesA[ nRow ], ::aCellColors[ nRow ]
            oCol := aCol[ _TBCI_COLOBJECT ]
            cValueA := cValue := Eval( oCol:block )
            aColor := _CELLCOLORS( aCol, cValue, nColors )
            IF ValType( cValue ) $ "CMNDTL"
               cValue := PadR( Transform( cValue, oCol:picture ), aCol[ _TBCI_CELLWIDTH ] )
            ELSE
               cValue := Space( aCol[ _TBCI_CELLWIDTH ] )
            ENDIF
         NEXT
      ELSE
         FOR EACH aCol, cValue, cValueA, aColor IN ::aColData, ::aCellValues[ nRow ], ::aCellValuesA[ nRow ], ::aCellColors[ nRow ]
            aColor := { aCol[ _TBCI_DEFCOLOR ][ 1 ], aCol[ _TBCI_DEFCOLOR ][ 2 ] }
            cValue := Space( aCol[ _TBCI_CELLWIDTH ] )
            cValueA := aCol[ _TBCI_COLOBJECT ]:blankVariable
         NEXT
      ENDIF

      ::aCellStatus[ nRow ] := .T.
      ::aDispStatus[ nRow ] := .T.
   ENDIF

   RETURN lRead

/*----------------------------------------------------------------------*/

METHOD cellValue( nRow, nCol ) CLASS XbpBrowse

   IF nRow >= 1 .AND. nRow <= ::rowCount .AND. ;
      nCol >= 1 .AND. nCol <= ::colCount .AND. ;
      ::aCellStatus[ nRow ]

      RETURN ::aCellValues[ nRow, nCol ]
   ENDIF

   RETURN NIL

/*----------------------------------------------------------------------*/

METHOD cellValueA( nRow, nCol ) CLASS XbpBrowse

   IF nRow >= 1 .AND. nRow <= ::rowCount .AND. ;
      nCol >= 1 .AND. nCol <= ::colCount .AND. ;
      ::aCellStatus[ nRow ]

      RETURN ::aCellValuesA[ nRow, nCol ]
   ENDIF

   RETURN NIL

/*----------------------------------------------------------------------*/

METHOD setUnstable() CLASS XbpBrowse

   IF ::nConfigure != 0
      ::doConfigure()
   ENDIF

   IF ::lHiLited
      ::deHilite()
   ENDIF

   ::lHitTop    := .F.
   ::lHitBottom := .F.
   ::lStable    := .F.

   RETURN Self

/*----------------------------------------------------------------------*/

METHOD invalidate() CLASS XbpBrowse

   ::setUnstable()
   ::lInvalid := .T.
   ::lFrames := .T.

   RETURN Self

/*----------------------------------------------------------------------*/

METHOD refreshAll() CLASS XbpBrowse

   ::setUnstable()

   Eval( ::bSkipBlock, 1 - ::nBufferPos )
   ::nBufferPos := 1
   ::lFrames    := .T.
   ::lRefresh   := .T.

   RETURN Self

/*----------------------------------------------------------------------*/

METHOD refreshCurrent() CLASS XbpBrowse

   ::setUnstable()

   IF ::nRowPos >= 1 .AND. ::nRowPos <= ::rowCount
      ::aCellStatus[ ::nRowPos ] := .F.
   ENDIF

   RETURN Self

/*----------------------------------------------------------------------*/
/*                         Vertical Navigation                          */
/*----------------------------------------------------------------------*/

METHOD up() CLASS XbpBrowse
   LOCAL lReset := ::rowPos == 1

   ::setUnstable()
   ::nMoveOffset--

   ::forceStable()
   ::setCurrentIndex( lReset )
   ::updateVertScrollBar()

   RETURN Self

/*----------------------------------------------------------------------*/

METHOD down() CLASS XbpBrowse
   LOCAL lReset := ::rowPos >= ::rowCount

   ::setUnstable()
   ::nMoveOffset++

   ::forceStable()
   ::setCurrentIndex( lReset )
   ::updateVertScrollBar()

   RETURN Self

/*----------------------------------------------------------------------*/

METHOD pageUp() CLASS XbpBrowse

   ::setUnstable()
   ::nMoveOffset -= ::rowCount

   ::forceStable()
   ::setCurrentIndex( .t. )
   ::updateVertScrollBar()

   RETURN Self

/*----------------------------------------------------------------------*/

METHOD pageDown() CLASS XbpBrowse

   ::setUnstable()
   ::nMoveOffset += ::rowCount

   ::forceStable()
   ::setCurrentIndex( .t. )
   ::updateVertScrollBar()

   RETURN Self

/*----------------------------------------------------------------------*/

METHOD goTop() CLASS XbpBrowse

   ::setUnstable()

   Eval( ::bGoTopBlock )
   ::lRefresh    := .T.
   ::nRowPos     := 1
   ::nBufferPos  := 1
   ::nMoveOffset := 0
   Eval( ::bSkipBlock, 0 )

   ::forceStable()
   ::setCurrentIndex( .t. )
   ::updateVertScrollBar()

   RETURN Self

/*----------------------------------------------------------------------*/

METHOD goBottom() CLASS XbpBrowse
   LOCAL nMoved

   ::setUnstable()

   Eval( ::bGoBottomBlock )
   nMoved := _SKIP_RESULT( Eval( ::bSkipBlock, -( ::rowCount - 1 ) ) )
   ::lRefresh    := .T.
   ::nRowPos     := 1
   ::nBufferPos  := 1
   ::nMoveOffset := -nMoved
   Eval( ::bSkipBlock, 0 )

   ::forceStable()
   ::setCurrentIndex( .t. )
   ::updateVertScrollBar()

   RETURN Self

/*----------------------------------------------------------------------*/
/*                       Horizontal Navigation                          */
/*----------------------------------------------------------------------*/

METHOD left() CLASS XbpBrowse
   LOCAL n, nCol
   LOCAL lUpdate := .f.

   nCol := ::colPos

   IF nCol > 1
      DO WHILE --nCol >= 1
         IF !ISFROZEN( nCol )
            lUpdate := .t.
            EXIT
         ENDIF
      ENDDO
   ENDIF
   IF lUpdate
      ::colPos := nCol

      n  := ::oHeaderView:sectionViewportPosition( ::colPos-1 )
      IF n < 0
         ::oHeaderView:setOffset( ::oHeaderView:offSet() + n )
         ::oFooterView:setOffset( ::oFooterView:offSet() + n )
         ::setCurrentIndex( .t. )
      ELSE
         ::setCurrentIndex( .f. )
      ENDIF

      ::oHScrollBar:setValue( ::colPos - 1 )
   ENDIF

   RETURN Self

/*----------------------------------------------------------------------*/

METHOD right() CLASS XbpBrowse
   LOCAL n, n1, n2, nLnWidth, nCol
   LOCAL lUpdate := .f.

   IF ::colPos < ::colCount
      nCol := ::colPos

      DO WHILE ++nCol <= ::colCount
         IF !( ISFROZEN( nCol ) )
            lUpdate := .t.
            EXIT
         ENDIF
      ENDDO

      IF lUpdate
         ::colPos := nCol

         n  := ::oHeaderView:sectionViewportPosition( ::colPos - 1 )
         n1 := ::oHeaderView:sectionSize( ::colPos-1 )
         n2 := ::oViewport:width()
         IF n + n1 > n2
            nLnWidth := ::oTableView:lineWidth()
            IF n1 > n2
               ::oHeaderView:setOffset( ::oHeaderView:sectionPosition( ::colPos - 1 ) )
               ::oFooterView:setOffset( ::oHeaderView:sectionPosition( ::colPos - 1 ) )

            ELSE
               ::oHeaderView:setOffset( ::oHeaderView:offSet()+(n1-(n2-n)+1) - nLnWidth )
               ::oFooterView:setOffset( ::oFooterView:offSet()+(n1-(n2-n)+1) - nLnWidth )

            ENDIF
            ::setCurrentIndex( .t. )
         ELSE
            ::setCurrentIndex( .f. )
         ENDIF

         ::oHScrollBar:setValue( ::colPos - 1 )
      ENDIF
   ENDIF

   RETURN Self

/*----------------------------------------------------------------------*/

METHOD firstCol() CLASS XbpBrowse
   LOCAL n, nCol
   LOCAL lUpdate := .f.

   nCol := 0

   DO WHILE ++nCol <= ::colCount
      IF !ISFROZEN( nCol )
         lUpdate := .t.
         EXIT
      ENDIF
   ENDDO

   IF lUpdate
      ::setUnstable()
      ::colPos := nCol

      n  := ::oHeaderView:sectionViewportPosition( ::colPos-1 )
      IF n < 0
         ::oHeaderView:setOffset( ::oHeaderView:offSet() + n )
         ::oFooterView:setOffset( ::oFooterView:offSet() + n )
         ::setCurrentIndex( .t. )
      ELSE
         ::setCurrentIndex( .f. )
      ENDIF
      ::oHScrollBar:setValue( ::colPos - 1 )
   ENDIF
   RETURN Self

/*----------------------------------------------------------------------*/

METHOD lastCol() CLASS XbpBrowse
   LOCAL n, n1, n2, nCol
   LOCAL lUpdate := .f.

   nCol := ::colCount + 1

   DO WHILE --nCol >= 1
      IF !ISFROZEN( nCol )
         lUpdate := .t.
         EXIT
      ENDIF
   ENDDO

   IF lUpdate
      ::setUnstable()
      ::colPos := nCol

      n  := ::oHeaderView:sectionViewportPosition( ::colPos-1 )
      n1 := ::oHeaderView:sectionSize( ::colPos-1 )
      n2 := ::oViewport:width()
      IF n + n1 > n2
         IF n1 > n2
            ::oHeaderView:setOffset( ::oHeaderView:sectionPosition( ::colPos - 1 ) )
            ::oFooterView:setOffset( ::oHeaderView:sectionPosition( ::colPos - 1 ) )
         ELSE
            ::oHeaderView:setOffset( ::oHeaderView:offSet()+(n1-(n2-n)+1) - ::oTableView:lineWidth() )
            ::oFooterView:setOffset( ::oFooterView:offSet()+(n1-(n2-n)+1) - ::oTableView:lineWidth() )
         ENDIF
         ::setCurrentIndex( .t. )
      ELSE
         ::setCurrentIndex( .f. )
      ENDIF
      ::oHScrollBar:setValue( ::colPos - 1 )
   ENDIF

   RETURN Self

/*----------------------------------------------------------------------*/

METHOD home() CLASS XbpBrowse

   ::colPos := max( 1, ::oHeaderView:visualIndexAt( 1 ) + 1 )
   ::setCurrentIndex( .t. )

   RETURN Self

/*----------------------------------------------------------------------*/

METHOD end() CLASS XbpBrowse

   ::nRightVisible := ::oHeaderView:visualIndexAt( ::oViewport:width()-2 ) + 1
   IF ::nRightVisible == 0
      ::nRightVisible := ::colCount
   ENDIF
   ::colPos := ::nRightVisible
   ::setCurrentIndex( .t. )

   RETURN Self

/*----------------------------------------------------------------------*/

METHOD panHome() CLASS XbpBrowse

   ::oHeaderView:setOffset( 0 )
   ::oFooterView:setOffset( 0 )
   ::oDbfModel:reset()

   RETURN Self

/*----------------------------------------------------------------------*/

METHOD panEnd() CLASS XbpBrowse
   LOCAL nOffset

   IF ::oHeaderView:sectionSize( ::colCount - 1 ) > ::oViewport:width()
      nOffSet := ::oHeaderView:sectionPosition( ::colCount - 1 )
   ELSE
      nOffSet := ::oHeaderView:sectionPosition( ::colCount - 1 ) + ;
                 ::oHeaderView:sectionSize( ::colCount - 1 ) - ::oViewport:width()
   ENDIF
   ::oHeaderView:setOffset( nOffSet )
   ::oFooterView:setOffset( nOffSet )
   ::oDbfModel:reset()

   RETURN Self

/*----------------------------------------------------------------------*/

METHOD panLeft() CLASS XbpBrowse
   LOCAL nLeftVisible := ::oHeaderView:visualIndexAt( 1 )+1

   IF nLeftVisible > 1
      ::oHeaderView:setOffSet( ::oHeaderView:sectionPosition( nLeftVisible - 2 ) )
      ::oFooterView:setOffSet( ::oFooterView:sectionPosition( nLeftVisible - 2 ) )
   ENDIF
   ::oDbfModel:reset()

   RETURN Self

/*----------------------------------------------------------------------*/

METHOD panRight() CLASS XbpBrowse
   LOCAL nNewPos

   ::setUnstable()
   nNewPos := _NEXTCOLUMN( ::aColData, Max( 1, ::nRightVisible + 1 ) )

   IF nNewPos != 0 .AND. nNewPos != ::nRightVisible
      ::nLeftVisible := 0
      ::nRightVisible := nNewPos
   ENDIF

   RETURN Self

/*----------------------------------------------------------------------*/
/*                                                                      */
/*----------------------------------------------------------------------*/

STATIC PROCEDURE _GENLIMITRTE()
   LOCAL oError := ErrorNew()

   oError:severity    := ES_ERROR
   oError:genCode     := EG_LIMIT
   oError:subSystem   := "TBROWSE"
   oError:subCode     := 0
   oError:description := hb_LangErrMsg( EG_LIMIT )
   oError:canRetry    := .F.
   oError:canDefault  := .F.
   oError:fileName    := ""
   oError:osCode      := 0

   Eval( ErrorBlock(), oError )
   __errInHandler()

   RETURN

/*----------------------------------------------------------------------*/
/* helper function to take headings and footing data */
STATIC FUNCTION _DECODE_FH( cName, nHeight, nWidth )
   LOCAL i

   nHeight := nWidth := 0
   IF ISCHARACTER( cName )

      IF Len( cName ) > 0
         /* When last character of heading/footing is ';' then CA-Cl*pper
          * does not calculate it as separator
          */
         IF Right( cName, 1 ) == _TBR_CHR_LINEDELIMITER
            cName := Left( cName, Len( cName ) - 1 )
         ENDIF
         nHeight := hb_TokenCount( cName, _TBR_CHR_LINEDELIMITER )
         FOR i := 1 TO nHeight
            nWidth := Max( nWidth, Len( hb_TokenGet( cName, i, _TBR_CHR_LINEDELIMITER ) ) )
         NEXT
      ENDIF

   ELSE
      /* CA-Cl*per bug, it accepts non character values though cannot
       * display them properly
       */
      /* nHeight := 1 */
      cName := ""
   ENDIF

   RETURN nHeight != 0


STATIC FUNCTION _MAXFREEZE( nColumns, aColData, nWidth )
   LOCAL aCol
   LOCAL lFirst
   LOCAL nCol, nColWidth, nTot

   IF nColumns > Len( aColData ) .OR. nColumns < 1
      RETURN 0
   ENDIF

   nTot := nWidth
   lFirst := .T.
   FOR nCol := 1 TO nColumns
      aCol := aColData[ nCol ]
      IF aCol[ _TBCI_CELLWIDTH ] > 0
         nColWidth := aCol[ _TBCI_COLWIDTH ]
         IF lFirst
            lFirst := .F.
         ELSE
            nColWidth += aCol[ _TBCI_SEPWIDTH ]
         ENDIF
         IF ( nWidth -= nColWidth ) < 0
            EXIT
         ENDIF
      ENDIF
   NEXT

   /* CA-Cl*pper allows to freeze all columns only when they
    * are fully visible, otherwise it reserves at least one
    * character for 1-st unfrozen column [druzus]
    */
   IF nWidth > 0 .OR. ;
      nWidth == 0 .AND. _NEXTCOLUMN( aColData, nColumns + 1 ) == 0

      RETURN nColumns
   ENDIF

   nWidth := nTot

   RETURN 0


STATIC FUNCTION _NEXTCOLUMN( aColData, nCol )
   LOCAL aCol

   DO WHILE nCol <= Len( aColData )
      aCol := aColData[ nCol ]
      IF aCol[ _TBCI_CELLWIDTH ] > 0
         RETURN nCol
      ENDIF
      ++nCol
   ENDDO

   RETURN 0


//STATIC PROCEDURE _DISP_FHSEP( nRow, nType, cColor, aColData )
STATIC PROCEDURE _DISP_FHSEP()
   /* GUI does not implements it */
   RETURN

//STATIC PROCEDURE _DISP_FHNAME( nRow, nHeight, nLeft, nRight, nType, nColor, aColors, aColData )
STATIC PROCEDURE _DISP_FHNAME()
   /* GUI does not implements it */
   RETURN

METHOD dispFrames() CLASS XbpBrowse
   /* GUI do not implements it */
   ::lFrames := .F.
   RETURN Self

METHOD dispRow( nRow ) CLASS XbpBrowse
   /* GUI do not implements it */
   HB_SYMBOL_UNUSED( nRow )
   RETURN Self

METHOD colorRect( aRect, aColors ) CLASS XbpBrowse
   /* GUI does not implements it */
   HB_SYMBOL_UNUSED( aRect   )
   HB_SYMBOL_UNUSED( aColors )
   RETURN Self

STATIC FUNCTION _PREVCOLUMN( aColData, nCol )
   LOCAL aCol

   DO WHILE nCol >= 1
      aCol := aColData[ nCol ]
      IF aCol[ _TBCI_CELLWIDTH ] > 0
         RETURN nCol
      ENDIF
      --nCol
   ENDDO

   RETURN 0


STATIC FUNCTION _SETCOLUMNS( nFrom, nTo, nStep, aColData, nFirst, nWidth, lFirst )
   LOCAL aCol
   LOCAL nCol, nColWidth
   LOCAL nLast := 0

   IF nWidth > 0
      FOR nCol := nFrom TO nTo STEP nStep
         aCol := aColData[ nCol ]
         IF aCol[ _TBCI_CELLWIDTH ] > 0
            IF nFirst == 0 .OR. nCol == nFirst
               nColWidth := aCol[ _TBCI_COLWIDTH ]
            ELSEIF nCol < nFirst
               nColWidth := aCol[ _TBCI_COLWIDTH ] + aColData[ nFirst ][ _TBCI_SEPWIDTH ]
            ELSE
               nColWidth := aCol[ _TBCI_COLWIDTH ] + aCol[ _TBCI_SEPWIDTH ]
            ENDIF
            IF nWidth >= nColWidth
               nLast := nCol
               nWidth -= nColWidth
               lFirst := .F.
               IF nFirst == 0 .OR. nCol < nFirst
                  nFirst := nCol
               ENDIF
            ELSE
               IF lFirst
                  nLast := nCol
                  nWidth := 0
                  lFirst := .F.
               ENDIF
               EXIT
            ENDIF
         ENDIF
      NEXT
   ENDIF
   RETURN iif( nLast == 0, nFrom - nStep, nLast )

METHOD colorValue( nColorIndex ) CLASS XbpBrowse
   IF ::nConfigure != 0
      ::doConfigure()
   ENDIF

   IF ISNUMBER( nColorIndex )
      IF nColorIndex >= 1 .AND. nColorIndex <= Len( ::aColors )
         RETURN ::aColors[ nColorIndex ]
      ELSEIF nColorIndex == 0
         RETURN "N/N"
      ENDIF
   ENDIF
   RETURN ::aColors[ _TBC_CLR_STANDARD ]

METHOD cellColor( nRow, nCol ) CLASS XbpBrowse
   IF nRow >= 1 .AND. nRow <= ::rowCount .AND. ;
      nCol >= 1 .AND. nCol <= ::colCount .AND. ;
      ::aCellStatus[ nRow ]

      RETURN ::aCellColors[ nRow, nCol ]
   ENDIF
   RETURN NIL

STATIC FUNCTION _DECODECOLORS( cColorSpec )
   LOCAL aColors := {}
   LOCAL nColors := hb_TokenCount( cColorSpec, "," )
   LOCAL cColor
   LOCAL nPos

   FOR nPos := 1 TO nColors
      cColor := hb_tokenGet( cColorSpec, nPos, "," )
      IF nPos <= 2 .AND. hb_colorToN( cColor ) == -1
         cColor := iif( nPos == 1, "W/N", "N/W" )
      ENDIF
      AAdd( aColors, cColor )
   NEXT
   IF Empty( aColors )
      AAdd( aColors, "W/N" )
   ENDIF
   IF Len( aColors ) < 2
      AAdd( aColors, "N/W" )
   ENDIF
   DO WHILE Len( aColors ) < _TBC_CLR_MAX
      AAdd( aColors, aColors[ _TBC_CLR_STANDARD ] )
   ENDDO
   RETURN aColors

STATIC FUNCTION _COLDEFCOLORS( aDefColorsIdx, nMaxColorIndex )
   LOCAL aColorsIdx := { _TBC_CLR_STANDARD, _TBC_CLR_SELECTED, ;
                         _TBC_CLR_STANDARD, _TBC_CLR_STANDARD }
   LOCAL nColorIndex
   LOCAL nPos

   IF ISARRAY( aDefColorsIdx )
      FOR nPos := 1 TO _TBC_CLR_MAX
         IF nPos <= Len( aDefColorsIdx ) .AND. ;
            ISNUMBER( nColorIndex := aDefColorsIdx[ nPos ] ) .AND. ;
            ( nColorIndex := Int( nColorIndex ) ) >= 0 .AND. ;
            nColorIndex <= nMaxColorIndex

            aColorsIdx[ nPos ] := nColorIndex
         ELSEIF nPos > 2
            aColorsIdx[ nPos ] := aColorsIdx[ 1 ]
         ENDIF
      NEXT
   ENDIF

   RETURN aColorsIdx

STATIC FUNCTION _CELLCOLORS( aCol, xValue, nMaxColorIndex )
   LOCAL aColors := { aCol[ _TBCI_DEFCOLOR ][ _TBC_CLR_STANDARD ], ;
                      aCol[ _TBCI_DEFCOLOR ][ _TBC_CLR_SELECTED ] }
   LOCAL xColor := Eval( aCol[ _TBCI_COLOBJECT ]:colorBlock, xValue )
   LOCAL nColorIndex
   LOCAL nPos, nMax

   IF ISARRAY( xColor )
      nMax := Min( Len( xColor ), 2 )
      FOR nPos := 1 TO nMax
         nColorIndex := xColor[ nPos ]
         IF ISNUMBER( nColorIndex )
            nColorIndex := Int( nColorIndex )
            IF nColorIndex >= 0 .AND. nColorIndex <= nMaxColorIndex
               aColors[ nPos ] := nColorIndex
            ENDIF
         ENDIF
      NEXT
   ENDIF
   RETURN aColors

METHOD setCursorPos() CLASS XbpBrowse
   LOCAL aCol
   LOCAL nRow, nCol

   IF ::nConfigure != 0
      ::doConfigure()
   ENDIF

   nRow := ::nRowPos
   nCol := ::nColPos

   IF nRow >= 1 .AND. nRow <= ::rowCount .AND. ;
      nCol >= 1 .AND. nCol <= ::colCount .AND. ;
      ( aCol := ::aColData[ nCol ] )[ _TBCI_COLPOS ] != NIL

      ::n_Row := ::n_Top + ::nHeadHeight + iif( ::lHeadSep, 0, -1 ) + nRow
      ::n_Col := ::aColData[ nCol ][ _TBCI_COLPOS ] + ;
                 ::aColData[ nCol ][ _TBCI_CELLPOS ]
      IF aCol[ _TBCI_SEPWIDTH ] > 0
         DO WHILE --nCol >= 1
            IF ::aColData[ nCol ][ _TBCI_COLPOS ] != NIL
               ::n_Col += aCol[ _TBCI_SEPWIDTH ]
               EXIT
            ENDIF
         ENDDO
      ENDIF
      SetPos( ::n_Row, ::n_Col )
      RETURN .T.
   ENDIF
   RETURN .F.

/* set visible columns */
METHOD setVisible() CLASS XbpBrowse
   RETURN Self

METHOD hiLite() CLASS XbpBrowse
   RETURN Self

METHOD deHilite() CLASS XbpBrowse
   RETURN Self

/* Returns the display width of a particular column */
METHOD colWidth( nColumn ) CLASS XbpBrowse
   IF ::nConfigure != 0
      ::doConfigure()
   ENDIF

   IF ISNUMBER( nColumn ) .AND. nColumn >= 1 .AND. nColumn <= ::colCount
      RETURN ::aColData[ nColumn ][ _TBCI_COLWIDTH ]
   ENDIF
   RETURN 0

/* get number of frozen columns */
METHOD getFrozen() CLASS XbpBrowse
   IF ::nConfigure != 0
      ::doConfigure()
   ENDIF
   RETURN ::nFrozen

/* set number of columns to freeze */
METHOD freeze( nColumns ) CLASS XbpBrowse
   LOCAL nCols

   IF ::nConfigure != 0
      ::doConfigure()
   ENDIF

   IF ISNUMBER( nColumns )

      nCols := Int( nColumns )
      IF _MAXFREEZE( nCols, ::aColData, _TBR_COORD( ::n_Right ) - _TBR_COORD( ::n_Left ) + 1 ) == nCols

         ::nFrozen := nCols
         ::lFrames := .T.
         ::nLastPos := 0
      ENDIF

      RETURN nCols
   ENDIF
   RETURN ::nFrozen

METHOD colorSpec( cColorSpec ) CLASS XbpBrowse
   IF cColorSpec != NIL
      ::cColorSpec := __eInstVar53( Self, "COLORSPEC", cColorSpec, "C", 1001 )
      ::configure( _TBR_CONF_COLORS )
   ENDIF
   RETURN ::cColorSpec

METHOD colCount() CLASS XbpBrowse
   RETURN Len( ::columns )

METHOD rowCount() CLASS XbpBrowse
   IF ::nConfigure != 0
      ::doConfigure()
   ENDIF
   RETURN ::nRowsInView

METHOD setRowPos( nRowPos ) CLASS XbpBrowse
   LOCAL nRow
   LOCAL nRowCount := ::rowCount
   IF ISNUMBER( nRowPos )
      nRow := Int( nRowPos )
      ::nRowPos := iif( nRow > nRowCount, nRowCount, ;
                     iif( nRow < 1, 1, nRow ) )
      RETURN nRow
   ELSE
      ::nRowPos := Min( nRowCount, 1 )
      RETURN 0
   ENDIF
   RETURN ::nRowPos

METHOD getRowPos() CLASS XbpBrowse
   IF ::nConfigure != 0
      ::doConfigure()
   ENDIF
   RETURN ::nRowPos

METHOD setColPos( nColPos ) CLASS XbpBrowse
   IF ::nConfigure != 0
      ::doConfigure()
   ENDIF
   IF ISNUMBER( nColPos )
      ::nColPos := nColPos
   ELSE
      ::nColPos := 0
   ENDIF
   RETURN ::nColPos

METHOD getColPos() CLASS XbpBrowse
   IF ::nConfigure != 0
      ::doConfigure()
   ENDIF
   RETURN ::nColPos

METHOD getTopFlag() CLASS XbpBrowse
   IF ::nConfigure != 0
      ::doConfigure()
   ENDIF
   RETURN ::lHitTop

METHOD setTopFlag( lTop ) CLASS XbpBrowse
   IF ::nConfigure != 0
      ::doConfigure()
   ENDIF
   IF !ISLOGICAL( lTop )
      RETURN .T.
   ENDIF
   ::lHitTop := lTop
   RETURN lTop

METHOD getBottomFlag() CLASS XbpBrowse
   IF ::nConfigure != 0
      ::doConfigure()
   ENDIF
   RETURN ::lHitBottom

METHOD setBottomFlag( lBottom ) CLASS XbpBrowse
   IF ::nConfigure != 0
      ::doConfigure()
   ENDIF
   IF !ISLOGICAL( lBottom )
      RETURN .T.
   ENDIF
   ::lHitBottom := lBottom
   RETURN lBottom

METHOD getAutoLite() CLASS XbpBrowse
   IF ::nConfigure != 0
      ::doConfigure()
   ENDIF
   RETURN ::lAutoLite

METHOD setAutoLite( lAutoLite ) CLASS XbpBrowse
   IF ::nConfigure != 0
      ::doConfigure()
   ENDIF
   IF !ISLOGICAL( lAutoLite )
      RETURN .T.
   ENDIF
   ::lAutoLite := lAutoLite
   RETURN lAutoLite

METHOD getStableFlag() CLASS XbpBrowse
   IF ::nConfigure != 0
      ::doConfigure()
   ENDIF
   RETURN ::lStable

METHOD setStableFlag( lStable ) CLASS XbpBrowse
   IF ::nConfigure != 0
      ::doConfigure()
   ENDIF
   IF !ISLOGICAL( lStable )
      RETURN .T.
   ENDIF
   ::lStable := lStable
   RETURN lStable

METHOD leftVisible() CLASS XbpBrowse
   IF ::nConfigure != 0
      ::doConfigure()
   ENDIF
   RETURN ::nLeftVisible

METHOD rightVisible() CLASS XbpBrowse
   IF ::nConfigure != 0
      ::doConfigure()
   ENDIF
   RETURN ::nRightVisible

/* Adds a TBColumn object to the TBrowse object */
METHOD addColumn( oCol ) CLASS XbpBrowse
   AAdd( ::columns, oCol )
   ::doConfigure()  /* QT */
   RETURN Self

/* Delete a column object from a browse */
METHOD delColumn( nColumn ) CLASS XbpBrowse
   LOCAL oCol
   oCol := ::columns[ nColumn ]
   ADel( ::columns, nColumn )
   ASize( ::columns, Len( ::columns ) - 1 )
   ::doConfigure()
   RETURN oCol

/* Insert a column object in a browse */
METHOD insColumn( nColumn, oCol ) CLASS XbpBrowse
   HB_AIns( ::columns, nColumn, oCol, .T. )
   ::doConfigure()
   RETURN oCol

/* Replaces one TBColumn object with another */
METHOD setColumn( nColumn, oCol ) CLASS XbpBrowse
   LOCAL oPrevCol
   IF nColumn != NIL .AND. oCol != NIL
      nColumn := __eInstVar53( Self, "COLUMN", nColumn, "N", 1001 )
      oCol := __eInstVar53( Self, "COLUMN", oCol, "O", 1001 )

      oPrevCol := ::columns[ nColumn ]
      ::columns[ nColumn ] := oCol
      ::doConfigure()
   ENDIF
   RETURN oPrevCol

/* Gets a specific TBColumn object */
METHOD getColumn( nColumn ) CLASS XbpBrowse
   RETURN iif( nColumn >= 1 .AND. nColumn <= ::colCount, ::columns[ nColumn ], NIL )

METHOD footSep( cFootSep ) CLASS XbpBrowse
   IF cFootSep != NIL
      ::cFootSep := __eInstVar53( Self, "FOOTSEP", cFootSep, "C", 1001 )
   ENDIF
   RETURN ::cFootSep

METHOD colSep( cColSep ) CLASS XbpBrowse

   IF cColSep != NIL
      ::cColSep := __eInstVar53( Self, "COLSEP", cColSep, "C", 1001 )
   ENDIF

   RETURN ::cColSep

METHOD headSep( cHeadSep ) CLASS XbpBrowse
   IF cHeadSep != NIL
      ::cHeadSep := __eInstVar53( Self, "HEADSEP", cHeadSep, "C", 1001 )
   ENDIF
   RETURN ::cHeadSep

METHOD skipBlock( bSkipBlock ) CLASS XbpBrowse
   IF bSkipBlock != NIL
      ::bSkipBlock := __eInstVar53( Self, "SKIPBLOCK", bSkipBlock, "B", 1001 )
   ENDIF
   RETURN ::bSkipBlock

METHOD goTopBlock( bBlock ) CLASS XbpBrowse
   IF bBlock != NIL
      ::bGoTopBlock := __eInstVar53( Self, "GOTOPBLOCK", bBlock, "B", 1001 )
   ENDIF
   RETURN ::bGoTopBlock

METHOD goBottomBlock( bBlock ) CLASS XbpBrowse
   IF bBlock != NIL
      ::bGoBottomBlock := __eInstVar53( Self, "GOBOTTOMBLOCK", bBlock, "B", 1001 )
   ENDIF
   RETURN ::bGoBottomBlock

METHOD firstPosBlock( bBlock ) CLASS XbpBrowse
   IF bBlock != NIL
      ::bFirstPosBlock := __eInstVar53( Self, "FIRSTPOSBLOCK", bBlock, "B", 1001 )
   ENDIF
   RETURN ::bFirstPosBlock

METHOD lastPosBlock( bBlock ) CLASS XbpBrowse
   IF bBlock != NIL
      ::bLastPosBlock := __eInstVar53( Self, "LASTPOSBLOCK", bBlock, "B", 1001 )
   ENDIF
   RETURN ::bLastPosBlock

METHOD phyPosBlock( bBlock ) CLASS XbpBrowse
   IF bBlock != NIL
      ::bPhyPosBlock := __eInstVar53( Self, "PHYPOSBLOCK", bBlock, "B", 1001 )
   ENDIF
   RETURN ::bPhyPosBlock

METHOD posBlock( bBlock ) CLASS XbpBrowse
   IF bBlock != NIL
      ::bPosBlock := __eInstVar53( Self, "POSBLOCK", bBlock, "B", 1001 )
   ENDIF
   RETURN ::bPosBlock

METHOD goPosBlock( bBlock ) CLASS XbpBrowse
   IF bBlock != NIL
      ::bGoPosBlock := __eInstVar53( Self, "GOPOSBLOCK", bBlock, "B", 1001 )
   ENDIF
   RETURN ::bGoPosBlock

METHOD hitBottomBlock( bBlock ) CLASS XbpBrowse
   IF bBlock != NIL
      ::bHitBottomBlock := __eInstVar53( Self, "HITBOTTOMBLOCK", bBlock, "B", 1001 )
   ENDIF
   RETURN ::bHitBottomBlock

METHOD hitTopBlock( bBlock ) CLASS XbpBrowse
   IF bBlock != NIL
      ::bHitTopBlock := __eInstVar53( Self, "HITTOPBLOCK", bBlock, "B", 1001 )
   ENDIF
   RETURN ::bHitTopBlock

METHOD stableBlock( bBlock ) CLASS XbpBrowse
   IF bBlock != NIL
      ::bStableBlock := __eInstVar53( Self, "STABLEBLOCK", bBlock, "B", 1001 )
   ENDIF
   RETURN ::bStableBlock

METHOD nTop( nTop ) CLASS XbpBrowse

   IF nTop != NIL
         ::n_Top := __eInstVar53( Self, "NTOP", nTop, "N", 1001 )
         IF !Empty( ::cBorder )
            ::n_Top++
         ENDIF
      ::configure( _TBR_CONF_COLUMNS )
   ENDIF

      IF !Empty( ::cBorder )
         RETURN ::n_Top - 1
      ENDIF

   RETURN ::n_Top


METHOD nLeft( nLeft ) CLASS XbpBrowse

   IF nLeft != NIL
         ::n_Left := __eInstVar53( Self, "NLEFT", nLeft, "N", 1001 )
         IF !Empty( ::cBorder )
            ::n_Left++
         ENDIF
      ::configure( _TBR_CONF_COLUMNS )
   ENDIF

      IF !Empty( ::cBorder )
         RETURN ::n_Left - 1
      ENDIF

   RETURN ::n_Left

METHOD nBottom( nBottom ) CLASS XbpBrowse

   IF nBottom != NIL
      ::n_Bottom := __eInstVar53( Self, "NBOTTOM", nBottom, "N", 1001, {| o, x | x >= o:nTop } )
      IF !Empty( ::cBorder )
         ::n_Bottom--
      ENDIF
      ::configure( _TBR_CONF_COLUMNS )
   ENDIF

   IF !Empty( ::cBorder )
      RETURN ::n_Bottom + 1
   ENDIF

   RETURN ::n_Bottom

METHOD nRight( nRight ) CLASS XbpBrowse

   IF nRight != NIL
      ::n_Right := __eInstVar53( Self, "NRIGHT", nRight, "N", 1001, {| o, x | x >= o:nLeft } )
      IF !Empty( ::cBorder )
         ::n_Right--
      ENDIF
      ::configure( _TBR_CONF_COLUMNS )
   ENDIF

   IF !Empty( ::cBorder )
      RETURN ::n_Right + 1
   ENDIF

   RETURN ::n_Right

METHOD viewArea() CLASS XbpBrowse
   LOCAL nWidth, nFrozenWidth

   IF ::nConfigure != 0
      ::doConfigure()
   ENDIF

   nWidth := nFrozenWidth := _TBR_COORD( ::n_Right ) - _TBR_COORD( ::n_Left ) + 1
   _MAXFREEZE( ::nFrozen, ::aColData, @nWidth )
   nFrozenWidth -= nWidth

   RETURN { ::n_Top + ::nHeadHeight + iif( ::lHeadSep, 1, 0 ),;
            ::n_Left,;
            ::n_Bottom - ::nFootHeight - iif( ::lFootSep, 1, 0 ),;
            ::n_Right,;
            nFrozenWidth }

METHOD firstScrCol() CLASS XbpBrowse
   IF ::nConfigure != 0
      ::doConfigure()
   ENDIF
   RETURN iif( ::leftVisible == 0, 0, ::aColData[ ::leftVisible ][ _TBCI_COLPOS ] )

METHOD nRow() CLASS XbpBrowse
   IF ::nConfigure != 0
      ::doConfigure()
   ENDIF
   RETURN ::n_Row

METHOD nCol() CLASS XbpBrowse
   IF ::nConfigure != 0
      ::doConfigure()
   ENDIF
   RETURN ::n_Col

METHOD hitTest( mRow, mCol ) CLASS XbpBrowse
   HB_SYMBOL_UNUSED( mRow )
   HB_SYMBOL_UNUSED( mCol )
   RETURN HTNOWHERE

STATIC PROCEDURE _mBrwPos( oBrw, mRow, mCol )
   HB_SYMBOL_UNUSED( oBrw )
   HB_SYMBOL_UNUSED( mRow )
   HB_SYMBOL_UNUSED( mCol )
   RETURN

METHOD mRowPos() CLASS XbpBrowse
   RETURN 0

METHOD mColPos() CLASS XbpBrowse
   RETURN 0

METHOD border( cBorder ) CLASS XbpBrowse
   HB_SYMBOL_UNUSED( cBorder )
   RETURN ::cBorder

METHOD message( cMessage ) CLASS XbpBrowse
   IF cMessage != NIL
      ::cMessage := __eInstVar53( Self, "MESSAGE", cMessage, "C", 1001 )
   ENDIF
   RETURN ::cMessage


METHOD applyKey( nKey ) CLASS XbpBrowse
   LOCAL bBlock := ::SetKey( nKey )

   IF bBlock == NIL
      bBlock := ::SetKey( 0 )

      IF bBlock == NIL
         RETURN TBR_EXCEPTION
      ENDIF
   ENDIF

   RETURN Eval( bBlock, Self, nKey )


METHOD setKey( nKey, bBlock ) CLASS XbpBrowse
   HB_SYMBOL_UNUSED( nKey )
   HB_SYMBOL_UNUSED( bBlock )
   RETURN .f.


METHOD setStyle( nStyle, lNewValue ) CLASS XbpBrowse

   /* NOTE: CA-Cl*pper 5.3 will initialize this var on the first
            :setStyle() method call. [vszakats] */

   DEFAULT ::styles TO { .F., .F., .F., .F., .F., NIL }

   /* NOTE: CA-Cl*pper 5.3 does no checks on the value of nStyle, so in case
            it is zero or non-numeric, a regular RTE will happen. [vszakats] */

   IF nStyle > Len( ::styles ) .AND. ;
      nStyle <= 4096 /* some reasonable limit for maximum number of styles */
      ASize( ::styles, nStyle )
   ENDIF

   IF ISLOGICAL( lNewValue )
      ::styles[ nStyle ] := lNewValue
   ENDIF

   RETURN ::styles[ nStyle ]

/*----------------------------------------------------------------------*/
/*----------------------------------------------------------------------*/
/*----------------------------------------------------------------------*/
//
//                             XbpColumn
//
/*----------------------------------------------------------------------*/
/*----------------------------------------------------------------------*/
/*----------------------------------------------------------------------*/

CREATE CLASS XbpColumn  INHERIT XbpWindow, XbpDataRef

   EXPORTED:

   VAR    cargo                                                          /* 01. User-definable variable */
   VAR    nWidth                        PROTECTED                        /* 02. */
   VAR    bBlock                        PROTECTED                        /* 03. */
   VAR    aDefColor                     PROTECTED INIT { 1, 2 }          /* 04. NOTE: Default value for both CA-Cl*pper 5.2 and 5.3. */
   VAR    bColorBlock                   PROTECTED INIT {|| NIL }         /* 05. */
   VAR    cHeading                      PROTECTED INIT ""                /* 06. */
   VAR    cHeadSep                      PROTECTED                        /* 07. */
   VAR    cColSep                       PROTECTED                        /* 08. */
   VAR    cFootSep                      PROTECTED                        /* 09. */
   VAR    cFooting                      PROTECTED INIT ""                /* 10. */
   VAR    picture                                                        /* 11. Column picture string */

   VAR    bPreBlock                     PROTECTED                        /* 12. */
   VAR    bPostBlock                    PROTECTED                        /* 13. */
   VAR    aSetStyle                     PROTECTED INIT { .F., .F., .F. } /* 14. TBC_READWRITE, TBC_MOVE, TBC_SIZE */

   METHOD block( bBlock )                         SETGET                 /* Code block to retrieve data for the column */
   METHOD colorBlock( bColorBlock )               SETGET                 /* Code block that determines color of data items */
   METHOD defColor( aDefColor )                   SETGET                 /* Array of numeric indexes into the color table */
   METHOD colSep( cColSep )                       SETGET                 /* Column separator character */
   METHOD heading( cHeading )                     SETGET                 /* Column heading */
   METHOD footing( cFooting )                     SETGET                 /* Column footing */
   METHOD headSep( cHeadSep )                     SETGET                 /* Heading separator character */
   METHOD footSep( cFootSep )                     SETGET                 /* Footing separator character */
   METHOD width( nWidth )                         SETGET                 /* Column display width */

   METHOD preBlock( bPreBlock )                   SETGET                 /* Code block determining editing */
   METHOD postBlock( bPostBlock )                 SETGET                 /* Code block validating values */
   METHOD setStyle( nStyle, lNewValue )

   METHOD new( cHeading, bBlock )
   METHOD create( oParent, oOwner, aPos, aSize, aPresParams, lVisible )
   METHOD configure()

   METHOD datalink( bBlock )                      SETGET

   DATA   hAlignment                              INIT      Qt_AlignHCenter + Qt_AlignVCenter
   DATA   hHeight                                 INIT      16
   DATA   hFgColor                                INIT      Qt_black
   DATA   hBgColor                                INIT      Qt_darkGray

   DATA   fAlignment                              INIT      Qt_AlignHCenter + Qt_AlignVCenter
   DATA   fHeight                                 INIT      16
   DATA   fFgColor                                INIT      Qt_black
   DATA   fBgColor                                INIT      Qt_darkGray

   DATA   dAlignment                              INIT      Qt_AlignLeft
   DATA   dHeight                                 INIT      16
   DATA   dFgColor                                INIT      Qt_black
   DATA   dBgColor                                INIT      Qt_white
   DATA   nColWidth

   DATA   valtype
   DATA   type                                    INIT      XBPCOL_TYPE_TEXT
   DATA   blankVariable

   METHOD destroy()
   ENDCLASS

/*----------------------------------------------------------------------*/

METHOD datalink( bBlock ) CLASS XbpColumn

   IF bBlock != NIL
      ::bBlock := __eInstVar53( Self, "BLOCK", bBlock, "B", 1001 )
   ENDIF

   RETURN ::bBlock

/*----------------------------------------------------------------------*/

METHOD block( bBlock ) CLASS XbpColumn

   IF bBlock != NIL
      ::bBlock := __eInstVar53( Self, "BLOCK", bBlock, "B", 1001 )
   ENDIF

   RETURN ::bBlock

/*----------------------------------------------------------------------*/

METHOD colorBlock( bColorBlock ) CLASS XbpColumn

   IF bColorBlock != NIL
      ::bColorBlock := __eInstVar53( Self, "COLORBLOCK", bColorBlock, "B", 1001 )
   ENDIF

   RETURN ::bColorBlock

/*----------------------------------------------------------------------*/

METHOD defColor( aDefColor ) CLASS XbpColumn

   IF aDefColor != NIL
      ::aDefColor := __eInstVar53( Self, "DEFCOLOR", aDefColor, "A", 1001 )
   ENDIF

   RETURN ::aDefColor

/*----------------------------------------------------------------------*/

METHOD colSep( cColSep ) CLASS XbpColumn

   IF cColSep != NIL
      ::cColSep := __eInstVar53( Self, "COLSEP", cColSep, "C", 1001 )
   ENDIF

   RETURN ::cColSep

/*----------------------------------------------------------------------*/

METHOD heading( cHeading ) CLASS XbpColumn

   IF cHeading != NIL
      ::cHeading := __eInstVar53( Self, "HEADING", cHeading, "C", 1001 )
   ENDIF

   RETURN ::cHeading

/*----------------------------------------------------------------------*/

METHOD footing( cFooting ) CLASS XbpColumn

   IF cFooting != NIL
      ::cFooting := __eInstVar53( Self, "FOOTING", cFooting, "C", 1001 )
   ENDIF

   RETURN ::cFooting

/*----------------------------------------------------------------------*/

METHOD headSep( cHeadSep ) CLASS XbpColumn

   IF cHeadSep != NIL
      ::cHeadSep := __eInstVar53( Self, "HEADSEP", cHeadSep, "C", 1001 )
   ENDIF

   RETURN ::cHeadSep

/*----------------------------------------------------------------------*/

METHOD footSep( cFootSep ) CLASS XbpColumn

   IF cFootSep != NIL
      ::cFootSep := __eInstVar53( Self, "FOOTSEP", cFootSep, "C", 1001 )
   ENDIF

   RETURN ::cFootSep

/*----------------------------------------------------------------------*/

METHOD width( nWidth ) CLASS XbpColumn

   IF nWidth != NIL
      ::nWidth := __eInstVar53( Self, "WIDTH", nWidth, "N", 1001 )
   ENDIF

   RETURN ::nWidth

/*----------------------------------------------------------------------*/

METHOD preBlock( bPreBlock ) CLASS XbpColumn

   IF bPreBlock != NIL
      ::bPreBlock := __eInstVar53( Self, "PREBLOCK", bPreBlock, "B", 1001 )
   ENDIF

   RETURN ::bPreBlock

/*----------------------------------------------------------------------*/

METHOD postBlock( bPostBlock ) CLASS XbpColumn

   IF bPostBlock != NIL
      ::bPostBlock := __eInstVar53( Self, "POSTBLOCK", bPostBlock, "B", 1001 )
   ENDIF

   RETURN ::bPostBlock

/*----------------------------------------------------------------------*/

METHOD setStyle( nStyle, lNewValue ) CLASS XbpColumn
   IF nStyle > Len( ::aSetStyle ) .AND. nStyle <= 4096
      ASize( ::aSetStyle, nStyle )
   ENDIF

   IF ISLOGICAL( lNewValue )
      ::aSetStyle[ nStyle ] := lNewValue
   ENDIF

   RETURN ::aSetStyle[ nStyle ]

/*----------------------------------------------------------------------*/

METHOD new( cHeading, bBlock ) CLASS XbpColumn

   ::cHeading := cHeading
   ::bBlock := bBlock

   RETURN Self

/*----------------------------------------------------------------------*/

METHOD create( oParent, oOwner, aPos, aSize, aPresParams, lVisible ) CLASS XbpColumn

   ::xbpWindow:create( oParent, oOwner, aPos, aSize, aPresParams, lVisible )

   ::valtype       := valtype( ::setData() )
   ::blankVariable := iif( ::valtype == "N", 0, iif( ::valtype == "D", ctod( "" ), iif( ::valtype == "L", .f., "" ) ) )

   ::configure()

   RETURN Self

/*----------------------------------------------------------------------*/

METHOD destroy() CLASS XbpColumn

   ::bPreBlock       := NIL
   ::bPostBlock      := NIL
   ::bBlock          := NIL
   ::bColorBlock     := NIL

   ::clearSlots()
   ::xbpPartHandler:destroy()

   //::xbpWindow:destroy()

   RETURN nil

/*----------------------------------------------------------------------*/

METHOD configure() CLASS XbpColumn
   LOCAL n

   /*  Heading Area */
   IF ( n := ascan( ::aPresParams, {|e_| e_[ 1 ] == XBP_PP_COL_HA_CAPTION } ) ) > 0
      ::cHeading := ::aPresParams[ n,2 ]
   ENDIF
   IF ( n := ascan( ::aPresParams, {|e_| e_[ 1 ] == XBP_PP_COL_HA_FGCLR } ) ) > 0
      ::hFgColor := hbxbp_ConvertAFactFromXBP( "Color", ::aPresParams[ n,2 ] )
   ENDIF
   IF ( n := ascan( ::aPresParams, {|e_| e_[ 1 ] == XBP_PP_COL_HA_BGCLR } ) ) > 0
      ::hBgColor := hbxbp_ConvertAFactFromXBP( "Color", ::aPresParams[ n,2 ] )
   ENDIF
   IF ( n := ascan( ::aPresParams, {|e_| e_[ 1 ] == XBP_PP_COL_HA_HEIGHT } ) ) > 0
      ::hHeight := ::aPresParams[ n,2 ]
   ENDIF
   IF ( n := ascan( ::aPresParams, {|e_| e_[ 1 ] == XBP_PP_COL_HA_ALIGNMENT } ) ) > 0
      ::hAlignment := hbxbp_ConvertAFactFromXBP( "Alignment", ::aPresParams[ n,2 ] )
      ::hAlignment += Qt_AlignVCenter
   ENDIF

   /*  Data Area  */
   IF ( n := ascan( ::aPresParams, {|e_| e_[ 1 ] == XBP_PP_COL_DA_CELLALIGNMENT } ) ) > 0
      ::dAlignment := ::aPresParams[ n,2 ]
   ELSE
      ::dAlignment := iif( ::valtype == "N", Qt_AlignRight, iif( ::valtype $ "DL", Qt_AlignHCenter, Qt_AlignLeft ) )
      ::dAlignment += Qt_AlignVCenter
   ENDIF
   IF ( n := ascan( ::aPresParams, {|e_| e_[ 1 ] == XBP_PP_COL_DA_FGCLR } ) ) > 0
      ::dFgColor := hbxbp_ConvertAFactFromXBP( "Color", ::aPresParams[ n,2 ] )
   ENDIF
   IF ( n := ascan( ::aPresParams, {|e_| e_[ 1 ] == XBP_PP_COL_DA_BGCLR } ) ) > 0
      ::dBgColor := hbxbp_ConvertAFactFromXBP( "Color", ::aPresParams[ n,2 ] )
   ENDIF
   IF ( n := ascan( ::aPresParams, {|e_| e_[ 1 ] == XBP_PP_COL_DA_ROWHEIGHT } ) ) > 0
      ::dHeight := ::aPresParams[ n,2 ]
   ENDIF
   IF ( n := ascan( ::aPresParams, {|e_| e_[ 1 ] == XBP_PP_COL_DA_ROWWIDTH } ) ) > 0
      ::nColWidth := ::aPresParams[ n,2 ]
   ENDIF

   /*  Footer Area  */
   IF ( n := ascan( ::aPresParams, {|e_| e_[ 1 ] == XBP_PP_COL_FA_CAPTION } ) ) > 0
      ::cFooting := ::aPresParams[ n,2 ]
   ENDIF
   IF ( n := ascan( ::aPresParams, {|e_| e_[ 1 ] == XBP_PP_COL_FA_FGCLR } ) ) > 0
      ::fFgColor := hbxbp_ConvertAFactFromXBP( "Color", ::aPresParams[ n,2 ] )
   ENDIF
   IF ( n := ascan( ::aPresParams, {|e_| e_[ 1 ] == XBP_PP_COL_FA_BGCLR } ) ) > 0
      ::fBgColor := hbxbp_ConvertAFactFromXBP( "Color", ::aPresParams[ n,2 ] )
   ENDIF
   IF ( n := ascan( ::aPresParams, {|e_| e_[ 1 ] == XBP_PP_COL_FA_HEIGHT } ) ) > 0
      ::fHeight := ::aPresParams[ n,2 ]
   ENDIF
   IF ( n := ascan( ::aPresParams, {|e_| e_[ 1 ] == XBP_PP_COL_FA_ALIGNMENT } ) ) > 0
      ::fAlignment := hbxbp_ConvertAFactFromXBP( "Alignment", ::aPresParams[ n,2 ] )
      ::fAlignment += Qt_AlignVCenter
   ENDIF

   RETURN Self


/*----------------------------------------------------------------------*/
/*----------------------------------------------------------------------*/
/*----------------------------------------------------------------------*/
/*----------------------------------------------------------------------*/
//
//                              XbpCellGroup
//
/*----------------------------------------------------------------------*/
/*----------------------------------------------------------------------*/
/*----------------------------------------------------------------------*/

CLASS XbpCellGroup INHERIT XbpWindow

   METHOD   new()
   METHOD   create()
   METHOD   configure()
   METHOD   destroy()

   DATA     clipParent                            INIT      .T.
   DATA     maxRow                                INIT      XBP_AUTOSIZE
   DATA     referenceString                       INIT      "W"
   DATA     type                                  INIT      XBPCOL_TYPE_TEXT

   METHOD   cellFromPos( aPos )
   METHOD   cellRect( nRowPos, lInnerRect )
   METHOD   drawCell( naRowPos, lRepaint )
   METHOD   getCell( nRowPos )
   METHOD   getCellColor()
   METHOD   hiliteCell()
   METHOD   rowCount()
   METHOD   scrollDown()
   METHOD   scrollUp()
   METHOD   setCellColor()
   METHOD   itemMarked()                          SETGET
   METHOD   itemSelected()                        SETGET

   DATA     sl_xbeCELLGR_ItemMarked
   DATA     sl_xbeCELLGR_ItemSelected

   ENDCLASS

/*----------------------------------------------------------------------*/

METHOD XbpCellGroup:new()

   RETURN Self

/*----------------------------------------------------------------------*/

METHOD XbpCellGroup:create()

   RETURN Self

/*----------------------------------------------------------------------*/

METHOD XbpCellGroup:configure()

   RETURN Self

/*----------------------------------------------------------------------*/

METHOD XbpCellGroup:destroy()

   RETURN Self

/*----------------------------------------------------------------------*/

METHOD XbpCellGroup:cellFromPos( aPos )

   HB_SYMBOL_UNUSED( aPos )

   RETURN Self

/*----------------------------------------------------------------------*/

METHOD XbpCellGroup:cellRect( nRowPos, lInnerRect )

   HB_SYMBOL_UNUSED( nRowPos )
   HB_SYMBOL_UNUSED( lInnerRect )

   RETURN Self

/*----------------------------------------------------------------------*/

METHOD XbpCellGroup:drawCell( naRowPos, lRepaint )

   HB_SYMBOL_UNUSED( naRowPos )
   HB_SYMBOL_UNUSED( lRepaint )

   RETURN Self

/*----------------------------------------------------------------------*/

METHOD XbpCellGroup:getCell( nRowPos )

   HB_SYMBOL_UNUSED( nRowPos )

   RETURN Self

/*----------------------------------------------------------------------*/

METHOD XbpCellGroup:getCellColor()

   RETURN Self

/*----------------------------------------------------------------------*/

METHOD XbpCellGroup:hiliteCell()

   RETURN Self

/*----------------------------------------------------------------------*/

METHOD XbpCellGroup:rowCount()

   RETURN Self

/*----------------------------------------------------------------------*/

METHOD XbpCellGroup:scrollDown()

   RETURN Self

/*----------------------------------------------------------------------*/

METHOD XbpCellGroup:scrollUp()

   RETURN Self

/*----------------------------------------------------------------------*/

METHOD XbpCellGroup:setCellColor()

   RETURN Self

/*----------------------------------------------------------------------*/

METHOD XbpCellGroup:itemMarked()

   RETURN Self

/*----------------------------------------------------------------------*/

METHOD XbpCellGroup:itemSelected()

   RETURN Self

/*----------------------------------------------------------------------*/
