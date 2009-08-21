/*
 * $Id$
 */

/*
 * Harbour Project source code:
 * Source file for the Xbp*Classes
 *
 * Copyright 2009 Pritpal Bedi <pritpal@vouchcac.com>
 * http://www.harbour-project.org
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
 *                    Xbase++ Compatible XbpRtf Class
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
#include "hbqt.ch"

/*----------------------------------------------------------------------*/

#define HBQT_BRW_CELLVALUE                        1001
#define HBQT_BRW_COLCOUNT                         1002
#define HBQT_BRW_ROWCOUNT                         1003
#define HBQT_BRW_COLHEADER                        1004
#define HBQT_BRW_ROWHEADER                        1005
#define HBQT_BRW_COLALIGN                         1006
#define HBQT_BRW_COLFGCOLOR                       1007
#define HBQT_BRW_COLBGCOLOR                       1008
#define HBQT_BRW_DATFGCOLOR                       1009
#define HBQT_BRW_DATBGCOLOR                       1010
#define HBQT_BRW_COLHEIGHT                        1011
#define HBQT_BRW_DATHEIGHT                        1012
#define HBQT_BRW_DATALIGN                         1013
#define HBQT_BRW_CELLDECORATION                   1014

/*----------------------------------------------------------------------*/
/*----------------------------------------------------------------------*/
/*----------------------------------------------------------------------*/
//
//                              XbpBrowse
//
/*----------------------------------------------------------------------*/
/*----------------------------------------------------------------------*/
/*----------------------------------------------------------------------*/

#define HB_CLS_NOTOBJECT

#include "hbclass.ch"

#include "button.ch"
#include "color.ch"
#include "common.ch"
#include "error.ch"
#include "inkey.ch"
#include "setcurs.ch"
#include "tbrowse.ch"

#define _TBCI_COLOBJECT       1   // column object
#define _TBCI_COLWIDTH        2   // width of the column
#define _TBCI_COLPOS          3   // column position on screen
#define _TBCI_CELLWIDTH       4   // width of the cell
#define _TBCI_CELLPOS         5   // cell position in column
#define _TBCI_COLSEP          6   // column separator
#define _TBCI_SEPWIDTH        7   // width of the separator
#define _TBCI_HEADING         8   // column heading
#define _TBCI_FOOTING         9   // column footing
#define _TBCI_HEADSEP        10   // heading separator
#define _TBCI_FOOTSEP        11   // footing separator
#define _TBCI_DEFCOLOR       12   // default color
#define _TBCI_FROZENSPACE    13   // space after frozen columns
#define _TBCI_LASTSPACE      14   // space after last visible column
#define _TBCI_SIZE           14   // size of array with TBrowse column data

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

#define ISFROZEN( n )  ( ascan( ::aLeftFrozen, n ) > 0 .OR. ascan( ::aRightFrozen, n ) > 0 )

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

   VAR bSkipBlock             AS BLOCK            INIT {|| NIL } // 11. Code block used to reposition data source
   VAR bGoTopBlock            AS BLOCK            INIT {|| NIL } // 12. Code block executed by TBrowse:goTop()
   VAR bGoBottomBlock         AS BLOCK            INIT {|| NIL } // 13. Code block executed by TBrowse:goBottom()
   VAR bFirstPosBlock         AS BLOCK            INIT {|| NIL }
   VAR bLastPosBlock          AS BLOCK            INIT {|| NIL }
   VAR bPhyPosBlock           AS BLOCK            INIT {|| NIL }
   VAR bPosBlock              AS BLOCK            INIT {|| NIL }
   VAR bGoPosBlock            AS BLOCK            INIT {|| NIL }
   VAR bHitBottomBlock        AS BLOCK            INIT {|| NIL }
   VAR bHitTopBlock           AS BLOCK            INIT {|| NIL }
   VAR bStableBlock           AS BLOCK            INIT {|| NIL }

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
   METHOD exeBlock( nMode )                                 // executes view events
   METHOD supplyInfo( nMode )                               // supplies cell parameters to Qt engine
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
   METHOD setPosition( nPos )                               // synchronize record position with the buffer
   METHOD readRecord()                                      // read current record into the buffer

   METHOD setVisible()                                      // set visible columns
   METHOD setCursorPos()                                    // set screen cursor position at current cell
   METHOD scrollBuffer( nRows )                             // scroll internal buffer for given row numbers
   METHOD colorValue( nColorIndex )                         // get color value for given index
   METHOD cellValue( nRow, nCol )                           // get cell value formatted
   METHOD cellValueA( nRow, nCol )                          // get cell value actual
   METHOD cellColor( nRow, nCol )                           // get cell formatted value
   METHOD dispFrames()                                      // display TBrowse border, columns' headings, footings and separators
   METHOD dispRow( nRow )                                   // display TBrowse data

   FRIEND FUNCTION _mBrwPos                                 // helper function for mRow() and mCol() methods

   DATA     oDbfModel
   DATA     oModelIndex                           INIT      QModelIndex()
   DATA     oVHeaderView
   DATA     oHeaderView                       INIT      QHeaderView()
   DATA     oVScrollBar                        INIT      QScrollBar()
   DATA     oHScrollBar                        INIT      QScrollBar()
   DATA     oViewport                       INIT      QWidget()
   DATA     oFont                                 INIT      QFont()
   DATA     pCurIndex

   DATA     lFirst                                INIT      .t.
   DATA     nRowsInView                           INIT      1

   METHOD   setCurrentIndex()
   METHOD   setHorzOffset()
   METHOD   setVertScrollBarRange()
   METHOD   setHorzScrollBarRange()
   METHOD   updateVertScrollBar()
   METHOD   updatePosition()

EXPORTED:
   METHOD   footerRbDown()                        SETGET
   METHOD   headerRbDown()                        SETGET
   METHOD   itemMarked()                          SETGET
   METHOD   itemRbDown()                          SETGET
   METHOD   itemSelected()                        SETGET

   //METHOD   forceStable()                         SETGET
   METHOD   navigate()                            SETGET
   METHOD   pan()                                 SETGET

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
   METHOD   fetchColumnInfo()

   METHOD   setLeftFrozen( aColumns )
   METHOD   setRightFrozen( aColumns )
   DATA     aLeftFrozen                             INIT   {}
   DATA     aRightFrozen                            INIT   {}
   DATA     nLeftFrozen                             INIT   0
   DATA     nRightFrozen                            INIT   0

   ENDCLASS

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

   RETURN Self

/*----------------------------------------------------------------------*/

METHOD XbpBrowse:buildLeftFreeze()

   /*  Left Freeze */
   ::oLeftView := HbTableView():new()
   //
   ::oLeftView:setHorizontalScrollBarPolicy( Qt_ScrollBarAlwaysOff )
   ::oLeftView:setVerticalScrollBarPolicy( Qt_ScrollBarAlwaysOff )
   ::oLeftView:setTabKeyNavigation( .t. )
   ::oLeftView:setShowGrid( .t. )
   ::oLeftView:setGridStyle( Qt_DotLine )   /* to be based on column definition */
   ::oLeftView:setSelectionMode( QAbstractItemView_SingleSelection )
   ::oLeftView:setSelectionBehavior( IF( ::cursorMode == XBPBRW_CURSOR_ROW, QAbstractItemView_SelectRows, QAbstractItemView_SelectItems ) )
   //
   /*  Veritical Header because of Performance boost */
   ::oLeftVHeaderView := QHeaderView()
   ::oLeftVHeaderView:configure( ::oLeftView:verticalHeader() )
   ::oLeftVHeaderView:hide()
   /*  Horizontal Header Fine Tuning */
   ::oLeftHeaderView := QHeaderView()
   ::oLeftHeaderView:configure( ::oLeftView:horizontalHeader() )
   ::oLeftHeaderView:setHighlightSections( .F. )

   ::oLeftDbfModel := HbDbfModel():new( {|p1,p2,p3,p4| ::supplyInfo( 151, p1, p2, p3, p4 ) } )
   ::oLeftView:setModel( QT_PTROF( ::oLeftDbfModel ) )
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
   ::oLeftFooterModel := HbDbfModel():new( {|p1,p2,p3,p4| ::supplyInfo( 152, p1, p2, p3, p4 ) } )
   ::oLeftFooterView:setModel( QT_PTROF( ::oLeftFooterModel ) )
   //
   //::oLeftFooterView:hide()

   RETURN Self

/*----------------------------------------------------------------------*/

METHOD XbpBrowse:buildRightFreeze()
   LOCAL oVHdr

   /*  Left Freeze */
   ::oRightView := HbTableView():new()
   //
   ::oRightView:setHorizontalScrollBarPolicy( Qt_ScrollBarAlwaysOff )
   ::oRightView:setVerticalScrollBarPolicy( Qt_ScrollBarAlwaysOff )
   ::oRightView:setTabKeyNavigation( .t. )
   ::oRightView:setShowGrid( .t. )
   ::oRightView:setGridStyle( Qt_DotLine )   /* to be based on column definition */
   ::oRightView:setSelectionMode( QAbstractItemView_SingleSelection )
   ::oRightView:setSelectionBehavior( IF( ::cursorMode == XBPBRW_CURSOR_ROW, QAbstractItemView_SelectRows, QAbstractItemView_SelectItems ) )
   //
   /*  Veritical Header because of Performance boost */
   oVHdr := QHeaderView()
   oVHdr:configure( ::oRightView:verticalHeader() )
   oVHdr:hide()
   /*  Horizontal Header Fine Tuning */
   ::oRightHeaderView := QHeaderView()
   ::oRightHeaderView:configure( ::oRightView:horizontalHeader() )
   ::oRightHeaderView:setHighlightSections( .F. )

   ::oRightDbfModel := HbDbfModel():new( {|p1,p2,p3,p4| ::supplyInfo( 161, p1, p2, p3, p4 ) } )
   ::oRightView:setModel( QT_PTROF( ::oRightDbfModel ) )

   /*  Horizontal Footer */
   ::oRightFooterView := QHeaderView():new( Qt_Horizontal )
   //
   ::oRightFooterView:setHighlightSections( .F. )
   ::oRightFooterView:setMinimumHeight( 20 )
   ::oRightFooterView:setMaximumHeight( 20 )
   ::oRightFooterView:setResizeMode( QHeaderView_Fixed )
   ::oRightFooterView:setFocusPolicy( Qt_NoFocus )
   //
   ::oRightFooterModel := HbDbfModel():new( {|p1,p2,p3,p4| ::supplyInfo( 162, p1, p2, p3, p4 ) } )
   ::oRightFooterView:setModel( QT_PTROF( ::oRightFooterModel ) )

   RETURN Self

/*----------------------------------------------------------------------*/

METHOD XbpBrowse:create( oParent, oOwner, aPos, aSize, aPresParams, lVisible )

   ::xbpWindow:create( oParent, oOwner, aPos, aSize, aPresParams, lVisible )

   ::oWidget := QFrame():new( ::pParent )
   ::oWidget:setFrameStyle( QFrame_Panel + QFrame_Plain )

   /* Important here as other parts will be based on it*/
   ::setPosAndSize()

   /* Subclass of QTableView */
   ::oTableView := HbTableView():new()

   /* Some parameters */
   ::oTableView:setTabKeyNavigation( .t. )
   ::oTableView:setShowGrid( .t. )
   ::oTableView:setGridStyle( Qt_DotLine )   /* to be based on column definition */
   ::oTableView:setSelectionMode( QAbstractItemView_SingleSelection )
   ::oTableView:setSelectionBehavior( IF( ::cursorMode == XBPBRW_CURSOR_ROW, QAbstractItemView_SelectRows, QAbstractItemView_SelectItems ) )

   /* Connect Keyboard Events */
   ::connect( QT_PTROF( ::oTableView ), "keyPressEvent()"           , {|o,p   | ::exeBlock( 1, p, o ) } )
   ::connect( QT_PTROF( ::oTableView ), "mousePressEvent()"         , {|o,p   | ::exeBlock( 2, p, o ) } )
   ::connect( QT_PTROF( ::oTableView ), "mouseDoubleClickEvent()"   , {|o,p   | ::exeBlock( 3, p, o ) } )
   ::connect( QT_PTROF( ::oTableView ), "wheelEvent()"              , {|o,p   | ::exeBlock( 4, p, o ) } )
   ::connect( QT_PTROF( ::oTableView ), "scrollContentsBy(int,int)" , {|o,p,p1| ::exeBlock(11, p, p1, o ) } )

   /* Finetune Horizontal Scrollbar */
   ::oTableView:setHorizontalScrollBarPolicy( Qt_ScrollBarAlwaysOff )
   //
   ::oHScrollBar := QScrollBar():new()
   ::oHScrollBar:setOrientation( Qt_Horizontal )
   ::connect( QT_PTROF( ::oHScrollBar ), "actionTriggered(int)"     , {|o,i| ::exeBlock( 103, i, o ) } )
   ::connect( QT_PTROF( ::oHScrollBar ), "sliderReleased()"         , {|o,i| ::exeBlock( 104, i, o ) } )

   /*  Replace Vertical Scrollbar with our own */
   ::oTableView:setVerticalScrollBarPolicy( Qt_ScrollBarAlwaysOff )
   //
   ::oVScrollBar := QScrollBar():new()
   ::oVScrollBar:setOrientation( Qt_Vertical )
   ::connect( QT_PTROF( ::oVScrollBar ), "actionTriggered(int)"     , {|o,i| ::exeBlock( 101, i, o ) } )
   ::connect( QT_PTROF( ::oVScrollBar ), "sliderReleased()"         , {|o,i| ::exeBlock( 102, i, o ) } )

   /*  Veritical Header because of Performance boost */
   ::oVHeaderView := QHeaderView()
   ::oVHeaderView:configure( ::oTableView:verticalHeader() )
   ::oVHeaderView:hide()

   /*  Horizontal Header Fine Tuning */
   ::oHeaderView := QHeaderView()
   ::oHeaderView:configure( ::oTableView:horizontalHeader() )
   ::oHeaderView:setHighlightSections( .F. )
   //
   ::connect( QT_PTROF( ::oHeaderView ), "sectionPressed(int)"        , {|o,i      | ::exeBlock( 111, i, o ) } )
   ::connect( QT_PTROF( ::oHeaderView ), "sectionResized(int,int,int)", {|o,i,i1,i2| ::exeBlock( 121, i, i1, i2, o ) } )

   /* .DBF Manipulation Model */
   ::oDbfModel := HbDbfModel():new( {|p1,p2,p3,p4| ::supplyInfo( 141, p1, p2, p3, p4 ) } )
   /*  Attach Model with the View */
   ::oTableView:setModel( QT_PTROF( ::oDbfModel ) )
   /*  Set Initial Column and Row */
   ::oTableView:setCurrentIndex( QModelIndex():new():sibling( 0,0 ) )
   ::pCurIndex := ::oTableView:currentIndex()

   /*  Horizontal Footer */
   ::oFooterView := QHeaderView():new( Qt_Horizontal )
   //
   ::oFooterView:setHighlightSections( .F. )

   ::oFooterView:setMinimumHeight( 20 )
   ::oFooterView:setMaximumHeight( 20 )
   ::oFooterView:setResizeMode( QHeaderView_Fixed )
   ::oFooterView:setFocusPolicy( Qt_NoFocus )
   //
   ::oFooterModel := HbDbfModel():new( {|p1,p2,p3,p4| ::supplyInfo( 142, p1, p2, p3, p4 ) } )

   ::oFooterView:setModel( QT_PTROF( ::oFooterModel ) )
   //
   //::oFooterView:hide()

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
   ::oGridLayout:addWidget_1( QT_PTROF( ::oLeftView        ), 0, 0, 1, 1 )
   ::oGridLayout:addWidget_1( QT_PTROF( ::oLeftFooterView  ), 1, 0, 1, 1 )
   //
   ::oGridLayout:addWidget_1( QT_PTROF( ::oTableView       ), 0, 1, 1, 1 )
   ::oGridLayout:addWidget_1( QT_PTROF( ::oFooterView      ), 1, 1, 1, 1 )
   //
   ::oGridLayout:addWidget_1( QT_PTROF( ::oRightView       ), 0, 2, 1, 1 )
   ::oGridLayout:addWidget_1( QT_PTROF( ::oRightFooterView ), 1, 2, 1, 1 )
   //
   ::oGridLayout:addWidget_1( QT_PTROF( ::oHScrollBar      ), 2, 0, 1, 3 )
   /*  Columns */
   ::oGridLayout:addWidget_1( QT_PTROF( ::oVScrollBar      ), 0, 3, 2, 1 )

   IF ::visible
      ::show()
   ENDIF
   ::oParent:AddChild( SELF )

   /* Viewport */
   ::oViewport:configure( ::oTableView:viewport() )

   RETURN Self

/*----------------------------------------------------------------------*/

METHOD XbpBrowse:exeBlock( nEvent, p1, p2, p3 )
   LOCAL oWheelEvent, oMouseEvent, i, nRow, nRowPos, nCol, nColPos, oPoint

   HB_SYMBOL_UNUSED( p2 )

   DO CASE
   CASE nEvent == 1                   /* Keypress Event */
      SetAppEvent( xbeP_Keyboard, XbpQKeyEventToAppEvent( p1 ), NIL, self )

   CASE nEvent == 2                   /* Mousepress */
      oMouseEvent := QMouseEvent():configure( p1 )

      IF hb_isPointer( QT_PTROF( ::oModelIndex ) )
         ::oModelIndex:destroy()
      ENDIF
      oPoint := QPoint():new( oMouseEvent:x(), oMouseEvent:y() )
      ::oModelIndex:configure( ::oTableView:indexAt( QT_PTROF( oPoint ) ) )
      oPoint:destroy()

      /* Reposition the record pointer */
      IF ::oModelIndex:isValid()
         nRow    := ::oModelIndex:row() + 1
         nRowPos := ::rowPos
         //
         IF nRow < ::rowPos
            FOR i := 1 TO nRowPos - nRow
               ::up()
            NEXT
         ELSEIF nRow > ::rowPos
            FOR i := 1 TO nRow - nRowPos
               ::down()
            NEXT
         ENDIF

         nCol    := ::oModelIndex:column() + 1
         nColPos := ::colPos
         //
         IF nCol < nColPos
            FOR i := 1 TO nColPos - nCol
               ::left()
            NEXT
         ELSEIF nCol > nColPos
            FOR i := 1 TO nCol - nColPos
               ::right()
            NEXT
         ENDIF
      ENDIF

      IF oMouseEvent:button() == Qt_LeftButton
         SetAppEvent( xbeBRW_ItemMarked, { ::rowPos, ::colPos }, NIL, Self )

      ELSEIF oMouseEvent:button() == Qt_RightButton
         SetAppEvent( xbeBRW_ItemRbDown, { oMouseEvent:x(), oMouseEvent:y() }, { ::rowPos, ::colPos }, Self )

      ENDIF

   CASE nEvent == 3                   /* xbeBRW_ItemSelected */
      oMouseEvent := QMouseEvent():configure( p1 )
      IF oMouseEvent:button() == Qt_LeftButton
         SetAppEvent( xbeBRW_ItemSelected, NIL, NIL, Self )
      ENDIF

   CASE nEvent == 4                   /* wheelEvent */
      oWheelEvent := QWheelEvent():configure( p1 )
      IF oWheelEvent:orientation() == Qt_Vertical
         IF oWheelEvent:delta() > 0
            ::up()
         ELSE
            ::down()
         ENDIF
      ELSE
         IF oWheelEvent:delta() > 0
            ::right()
         ELSE
            ::left()
         ENDIF
      ENDIF

   CASE nEvent == 11                  /* Horizontal Scroll Position : sent by Qt */
      IF p1 <> 0
         ::setHorzOffset()
      ENDIF

   CASE nEvent == 101                 /* Vertical Scrollbar Movements by the User */
      SWITCH p1
      CASE QAbstractSlider_SliderNoAction
         RETURN NIL
      CASE QAbstractSlider_SliderSingleStepAdd
         ::down()
         ::updateVertScrollBar()
         EXIT
      CASE QAbstractSlider_SliderSingleStepSub
         ::up()
         ::updateVertScrollBar()
         EXIT
      CASE QAbstractSlider_SliderPageStepAdd
         ::pageDown()
         ::updateVertScrollBar()
         EXIT
      CASE QAbstractSlider_SliderPageStepSub
         ::pageUp()
         ::updateVertScrollBar()
         EXIT
      CASE QAbstractSlider_SliderToMinimum
         ::goTop()
         ::updateVertScrollBar()
         EXIT
      CASE QAbstractSlider_SliderToMaximum
         ::goBottom()
         ::updateVertScrollBar()
         EXIT
      CASE QAbstractSlider_SliderMove
         ::updatePosition()
         EXIT
      ENDSWITCH

   CASE nEvent == 102                 /* Vertical Scrollbar: Slider Released */
      ::updatePosition()

   CASE nEvent == 103                 /* Horizontal Scrollbar: Slider moved */
      nCol    := ::oHScrollBar:value()+1
      nColPos := ::colPos
      IF nCol < nColPos
         FOR i := 1 TO ( nColPos - nCol )
            ::left()
         NEXT
      ELSEIF nCol > nColPos
         FOR i := 1 TO ( nCol - nColPos )
            ::right()
         NEXT
      ENDIF

   CASE nEvent == 104                 /* Horizontal Scrollbar: Slider Released */
      nCol    := ::oHScrollBar:value()+1
      nColPos := ::colPos
      IF nCol < nColPos
         FOR i := 1 TO ( nColPos - nCol )
            ::left()
         NEXT
      ELSEIF nCol > nColPos
         FOR i := 1 TO ( nCol - nColPos )
            ::right()
         NEXT
      ENDIF

   CASE nEvent == 111                 /* Column Header Pressed */
      SetAppEvent( xbeBRW_HeaderRbDown, { 0,0 }, p1+1, Self )

   CASE nEvent == 121                 /* Header Section Resized */
      ::oFooterView:resizeSection( p1, p3 )

   CASE nEvent == 122                 /* Footer Section Resized */
      ::oHeaderView:resizeSection( p1, p3 )

   ENDCASE

   RETURN Self

/*----------------------------------------------------------------------*/

METHOD handleEvent( nEvent, mp1, mp2 ) CLASS XbpBrowse

   HB_SYMBOL_UNUSED( mp1 )
   HB_SYMBOL_UNUSED( mp2 )

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

      ENDSWITCH

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
      CASE mp1 == XBPBRW_Navigate_PrevLine
      CASE mp1 == XBPBRW_Navigate_NextPage
      CASE mp1 == XBPBRW_Navigate_PrevPage
      CASE mp1 == XBPBRW_Navigate_GoTop
      CASE mp1 == XBPBRW_Navigate_GoBottom
      CASE mp1 == XBPBRW_Navigate_Skip
      CASE mp1 == XBPBRW_Navigate_NextCol
      CASE mp1 == XBPBRW_Navigate_PrevCol
      CASE mp1 == XBPBRW_Navigate_FirstCol
      CASE mp1 == XBPBRW_Navigate_LastCol
      CASE mp1 == XBPBRW_Navigate_GoPos
      CASE mp1 == XBPBRW_Navigate_SkipCols
      CASE mp1 == XBPBRW_Navigate_GotoItem
      CASE mp1 == XBPBRW_Navigate_GotoRecord
      ENDCASE

   ENDCASE

   RETURN Self

/*----------------------------------------------------------------------*/

METHOD XbpBrowse:supplyInfo( nMode, nInfo, p2, p3 )

   DO CASE
   CASE nMode == 141       /* Main View Header|Data */
      IF nInfo == HBQT_BRW_COLCOUNT
         IF ::colCount > 0
            ::forceStable()
            ::setHorzScrollBarRange( .t. )
         ENDIF
         RETURN ::colCount
      ELSEIF nInfo == HBQT_BRW_ROWCOUNT
         IF ::colCount > 0
            ::forceStable()
            ::setVertScrollBarRange( .f. )
         ENDIF
         RETURN ::rowCount
      ELSE
         RETURN ::fetchColumnInfo( nInfo, 0, p2, p3 )
      ENDIF

   CASE nMode == 142       /* Main View Footer */
      IF nInfo == HBQT_BRW_COLCOUNT
         IF ::colCount > 0
            ::forceStable()
         ENDIF
         RETURN ::colCount
      ELSE
         RETURN ::fetchColumnInfo( nInfo, 1, p2, p3 )
      ENDIF

   CASE nMode == 151       /* Left Frozen Header|Data */
      IF nInfo == HBQT_BRW_COLCOUNT
         IF ::nLeftFrozen > 0
            ::forceStable()
         ENDIF
         RETURN ::nLeftFrozen
      ELSEIF nInfo == HBQT_BRW_ROWCOUNT
         IF ::nLeftFrozen > 0
            ::forceStable()
         ENDIF
         RETURN ::rowCount
      ELSE
         RETURN ::fetchColumnInfo( nInfo, 0, p2, ::aLeftFrozen[ p3 ] )
      ENDIF

   CASE nMode == 152       /* Left Frozen Footer */
      IF nInfo == HBQT_BRW_COLCOUNT
         IF ::nLeftFrozen > 0
            ::forceStable()
         ENDIF
         RETURN ::nLeftFrozen
      ELSE
         RETURN ::fetchColumnInfo( nInfo, 1, p2, ::aLeftFrozen[ p3 ] )
      ENDIF

   CASE nMode == 161       /* Right Frozen Header|Data */
      IF nInfo == HBQT_BRW_COLCOUNT
         IF ::nRightFrozen > 0
            ::forceStable()
         ENDIF
         RETURN ::nRightFrozen
      ELSEIF nInfo == HBQT_BRW_ROWCOUNT
         IF ::nRightFrozen > 0
            ::forceStable()
         ENDIF
         RETURN ::rowCount
      ELSE
         RETURN ::fetchColumnInfo( nInfo, 0, p2, ::aRightFrozen[ p3 ] )
      ENDIF

   CASE nMode == 162       /* Right Frozen Footer */
      IF nInfo == HBQT_BRW_COLCOUNT
         IF ::nRightFrozen > 0
            ::forceStable()
         ENDIF
         RETURN ::nRightFrozen
      ELSE
         RETURN ::fetchColumnInfo( nInfo, 1, p2, ::aRightFrozen[ p3 ] )
      ENDIF

   ENDCASE

   RETURN ""

/*----------------------------------------------------------------------*/

METHOD fetchColumnInfo( nInfo, nArea, nRow, nCol ) CLASS XbpBrowse
   LOCAL aColor
   LOCAL oCol := ::columns[ nCol ]

   SWITCH ( nInfo )

   /* Data Area */
   CASE HBQT_BRW_DATFGCOLOR
      IF hb_isBlock( oCol:colorBlock )
         aColor := eval( oCol:colorBlock, ::cellValueA( nRow, nCol ) )
         IF hb_isArray( aColor ) .and. hb_isNumeric( aColor[ 1 ] )
            RETURN ConvertAFact( "Color", XBTOQT_FROM_XB, aColor[ 1 ] )
         ELSE
            RETURN oCol:dFgColor
         ENDIF
      ELSE
         RETURN oCol:dFgColor
      ENDIF

   CASE HBQT_BRW_DATBGCOLOR
      IF hb_isBlock( oCol:colorBlock )
         aColor := eval( oCol:colorBlock, ::cellValueA( nRow, nCol ) )
         IF hb_isArray( aColor ) .and. hb_isNumeric( aColor[ 2 ] )
            RETURN ConvertAFact( "Color", XBTOQT_FROM_XB, aColor[ 2 ] )
         ELSE
            RETURN oCol:dBgColor
         ENDIF
      ELSE
         RETURN oCol:dBgColor
      ENDIF

   CASE HBQT_BRW_DATALIGN
      RETURN oCol:dAlignment

   CASE HBQT_BRW_DATHEIGHT
      RETURN oCol:dHeight

   CASE HBQT_BRW_CELLDECORATION
      IF oCol:type == XBPCOL_TYPE_FILEICON
         RETURN trim( ::cellValue( nRow, nCol ) )
      ELSE
         RETURN ""
      ENDIF

   CASE HBQT_BRW_CELLVALUE
      IF oCol:type == XBPCOL_TYPE_FILEICON
         RETURN ""
      ELSE
         RETURN ::cellValue( nRow, nCol )
      ENDIF

   OTHERWISE
      IF nArea == 0                    /* Header Area */
         SWITCH nInfo
         CASE HBQT_BRW_COLHEIGHT
            RETURN oCol:hHeight
         CASE HBQT_BRW_COLHEADER
            RETURN oCol:heading
         CASE HBQT_BRW_COLALIGN
            RETURN oCol:hAlignment
         CASE HBQT_BRW_COLFGCOLOR
            RETURN oCol:hFgColor
         CASE HBQT_BRW_COLBGCOLOR
            RETURN oCol:hBgColor
         ENDSWITCH
      ELSE                             /* Footer Area */
         SWITCH nInfo
         CASE HBQT_BRW_COLHEIGHT
            RETURN oCol:fHeight
         CASE HBQT_BRW_COLHEADER
            RETURN oCol:footing
         CASE HBQT_BRW_COLALIGN
            RETURN oCol:fAlignment
         CASE HBQT_BRW_COLFGCOLOR
            RETURN oCol:fFgColor
         CASE HBQT_BRW_COLBGCOLOR
            RETURN oCol:fBgColor
         ENDSWITCH
      ENDIF
   ENDSWITCH

   RETURN ""

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
   //
   IF lReset
      ::oDbfModel:reset()                         /* Important */
      IF hb_isObject( ::oLeftDbfModel )
         ::oLeftDbfModel:reset()
      ENDIF
      IF hb_isObject( ::oRightDbfModel )
         ::oRightDbfModel:reset()
      ENDIF
   ENDIF

   Qt_QModelIndex_destroy( ::pCurIndex )
   ::pCurIndex := ::oDbfModel:index( ::rowPos - 1, ::colPos - 1 )
   ::oTableView:setCurrentIndex( ::pCurIndex )

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
      nWidth   := IIF( cType $ "CMNDTL", Len( Transform( xValue, oCol:picture ) ), 0 )

      /* Control the picture spec of Bitmaps|Icon files */
      IF oCol:type != XBPCOL_TYPE_TEXT
         nWidth := 256
      ENDIF

      #if 0
      cColSep  := oCol:colSep
      IF cColSep == NIL
         cColSep := ::cColSep
      ENDIF

      cHeadSep := oCol:headSep
      IF !ISCHARACTER( cHeadSep ) .OR. cHeadSep == ""
         cHeadSep := ::cHeadSep
         IF !ISCHARACTER( cHeadSep )
            cHeadSep := ""
         ENDIF
      ENDIF

      cFootSep := oCol:footSep
      IF !ISCHARACTER( cFootSep ) .OR. cFootSep == ""
         cFootSep := ::cFootSep
         IF !ISCHARACTER( cFootSep )
            cFootSep := ""
         ENDIF
      ENDIF
      #endif
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

   /* Clipper does not set refreshAll flag in Configure */
   /* ::lRefresh := .T. */

   ::nLastRow := nRowCount
   ::nLastScroll := 0

   /* CA-Cl*pper update visible columns here but without
    * colPos repositioning. [druzus]
    */
   #if 0
   _SETVISIBLE( ::aColData, _TBR_COORD( ::n_Right ) - _TBR_COORD( ::n_Left ) + 1, ;
                @::nFrozen, @::nLeftVisible, @::nRightVisible )
   #endif

   ::nLastPos := 0

   IF ::nRowPos > nRowCount
      ::nRowPos := nRowCount
   ELSEIF ::nRowPos < 1
      ::nRowPos := 1
   ENDIF

   /* Qt Specifics follows */
   //
   IF !( ::lHScroll )
      ::oHScrollBar:hide()
   ENDIF
   IF !( ::lVScroll )
      ::oVScrollBar:hide()
   ENDIF

   ::oTableView:setSelectionBehavior( IF( ::cursorMode == XBPBRW_CURSOR_ROW, QAbstractItemView_SelectRows, QAbstractItemView_SelectItems ) )

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
      nViewH := ::oViewport:height()
      ::nRowsInView := Int( nViewH / nMaxCellH )
      IF ( nViewH % nMaxCellH ) > ( nMaxCellH / 2 )
         ::nRowsInView++
      ENDIF

      /* Probably this is the appropriate time to update row heights */
      FOR i := 1 TO ::nRowsInView
         ::oTableView:setRowHeight( i-1, nMaxCellH )
         ::oLeftView:setRowHeight( i-1, nMaxCellH )
         ::oRightView:setRowHeight( i-1, nMaxCellH )
      NEXT

      /* Implement Column Resizing Mode */
      ::oHeaderView:setResizeMode( IF( ::lSizeCols, QHeaderView_Interactive, QHeaderView_Fixed ) )
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

   IF ::nLeftFrozen == 0 .and. hb_isObject( ::oLeftView )
      ::oLeftView:hide()
      ::oLeftFooterView:hide()
   ELSEIF ::nLeftFrozen > 0 .and. hb_isObject( ::oLeftView )
      ::oLeftView:show()
      ::oLeftFooterView:show()
   ENDIF
   IF ::nRightFrozen == 0 .and. hb_isObject( ::oRightView )
      ::oRightView:hide()
      ::oRightFooterView:hide()
   ELSEIF ::nRightFrozen > 0 .and. hb_isObject( ::oRightView )
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

   ::setHorzScrollBarRange()

   /* Tell Qt to Reload Everything */
   ::oDbfModel:reset()
   //
   IF hb_isObject( ::oLeftDbfModel )
      ::oLeftDbfModel:reset()
   ENDIF
   IF hb_isObject( ::oRightDbfModel )
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
      RETURN Self
   ENDIF
   IF hb_isBlock( ::sl_xbeBRW_ItemSelected )
      eval( ::sl_xbeBRW_ItemSelected, NIL, NIL, self )
   ENDIF
   RETURN Self

/*----------------------------------------------------------------------*/

METHOD pan( p1 ) CLASS XbpBrowse
   LOCAL xRet
   IF hb_isBlock( p1 )
      ::sl_xbeBRW_Pan := p1
   ENDIF
   IF hb_isNumeric( p1 ) .and. hb_isBlock( ::sl_xbeBRW_Pan )
      xRet := eval( ::sl_xbeBRW_Pan, p1, NIL, self )
      IF xRet != NIL
         ::handleEvent( xbeBRW_Pan, p1, NIL )
      ENDIF
   ELSEIF hb_isNumeric( p1 )
      ::handleEvent( xbeBRW_Pan, p1, NIL )
   ENDIF
   RETURN Self

/*----------------------------------------------------------------------*/

METHOD navigate( p1, p2 ) CLASS XbpBrowse
   LOCAL xRet
   IF hb_isBlock( p1 )
      ::sl_xbeBRW_Navigate := p1
   ENDIF
   IF hb_isNumeric( p1 ) .and. hb_isBlock( ::sl_xbeBRW_Navigate )
      xRet := eval( ::sl_xbeBRW_Navigate, p1, p2, self )
      IF xRet != NIL
         ::handleEvent( xbeBRW_Navigate, p1, p2 )
      ENDIF
   ELSEIF hb_isNumeric( p1 )
      ::handleEvent( xbeBRW_Navigate, p1, p2 )
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

   DO WHILE !::stabilize()
   ENDDO

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
      #if 0  /* Physical scroll - in GUI not required */
      hb_scroll( ::n_Top + ::nHeadHeight + iif( ::lHeadSep, 1, 0 ), ::n_Left, ;
                 ::n_Bottom - ::nFootHeight - iif( ::lFootSep, 1, 0 ), ::n_Right, ;
                 nRows,, ::colorValue( _TBC_CLR_STANDARD ) )
      #endif

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


STATIC PROCEDURE _SETVISIBLE( aColData, nWidth, nFrozen, nLeft, nRight )

   LOCAL nPos, nFirst
   LOCAL lLeft, lRight, lFirst
   LOCAL nColCount := Len( aColData )

   /* Check if frozen columns are still valid, if not reset it to 0
    * It also calculates the size left for unfrozen columns [druzus]
    */
   nFrozen := _MAXFREEZE( nFrozen, aColData, @nWidth )

   /* CA-Cl*pper checks here only for columns number and does not check
    * if at least one column is visible (oCol:width > 0) and if not then
    * wrongly calculates visible columns and some internal indexes.
    * Using linkers like EXOSPACE with memory protection it causes
    * application crash with GPF. [druzus]
    */
   IF nColCount == 0 .OR. _NEXTCOLUMN( aColData, 1 ) == 0
      nLeft := nRight := 0
   ELSE
      /* This algorithms keeps CA-Cl*pper precedence in visible column
       * updating. It's also important for proper working panLeft and
       * panRight methods which use leftVisible and rightVisible values
       * for horizontal scrolling just like in CA-Cl*pper. [druzus]
       */
      IF nWidth >= 1
         lRight := nRight > nFrozen .AND. nRight <= nColCount .AND. ;
                   aColData[ nRight ][ _TBCI_CELLWIDTH ] > 0
         lLeft  := nLeft > nFrozen .AND. nLeft <= nColCount .AND. ;
                   aColData[ nLeft ][ _TBCI_CELLWIDTH ] > 0
         IF !lLeft
            IF lRight
               IF ( nLeft := _PREVCOLUMN( aColData, nRight ) ) < nFrozen
                  nLeft := nRight
               ENDIF
            ELSE
               nPos := _NEXTCOLUMN( aColData, Max( nLeft + 1, nFrozen + 1 ) )
               IF nPos == 0
                  nPos := _PREVCOLUMN( aColData, Min( nColCount, nLeft - 1 ) )
               ENDIF
               IF nPos > nFrozen
                  nLeft := nPos
                  lLeft := .T.
               ENDIF
            ENDIF
         ENDIF
         lFirst := .T.
         nFirst := _PREVCOLUMN( aColData, nFrozen )
      ELSE
         lLeft := lRight := .F.
      ENDIF
      IF lLeft
         nRight := _SETCOLUMNS( nLeft, nColCount, 1, aColData, @nFirst, @nWidth, @lFirst )
         nLeft := _SETCOLUMNS( nLeft - 1, nFrozen + 1, -1, aColData, @nFirst, @nWidth, @lFirst )
      ELSEIF lRight
         nLeft := _SETCOLUMNS( nRight, nFrozen + 1, -1, aColData, @nFirst, @nWidth, @lFirst )
         nRight := _SETCOLUMNS( nRight + 1, nColCount, 1, aColData, @nFirst, @nWidth, @lFirst )
      ELSE
         nLeft := nFrozen + 1
         nRight := nFrozen
      ENDIF
   ENDIF

   RETURN


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
   LOCAL nCol, nLeft, nFrozen, nLast, nColumns, nWidth, nColPos
   LOCAL lFirst, lFrames
   LOCAL aCol

   nColPos := ::nColPos
   IF nColPos < 1 .OR. nColPos > ::colCount .OR. ::nLastPos != nColPos .OR. ;
      ::lFrames .OR. ::nLeftVisible == 0 .OR. ::nRightVisible == 0 .OR. ;
      ::aColData[ nColPos ][ _TBCI_COLPOS ] == NIL

      lFrames := .F.
      nWidth := _TBR_COORD( ::n_Right ) - _TBR_COORD( ::n_Left ) + 1
      nColumns := Len( ::aColData )

      IF nColPos > nColumns
         ::nColPos := nColumns
         ::nLeftVisible := nColumns
         ::nRightVisible := nColumns
      ELSEIF ::nColPos < 1
         ::nColPos := 1
         ::nLeftVisible := 1
         ::nRightVisible := 1
      ELSEIF nColPos != ::nLastPos
         IF nColPos > ::nRightVisible
            ::nRightVisible := ::nColPos
            ::nLeftVisible := 0
         ELSEIF nColPos < ::nLeftVisible
            ::nLeftVisible := ::nColPos
            ::nRightVisible := 0
         ENDIF
      ELSEIF ::nColPos <= ::nFrozen .AND. ::nLeftVisible == 0
         nCol := _NEXTCOLUMN( ::aColData, ::nFrozen + 1 )
         ::nColPos := iif( nCol == 0, nColumns, nCol )
      ENDIF

      _SETVISIBLE( ::aColData, @nWidth, ;
                   @::nFrozen, @::nLeftVisible, @::nRightVisible )

      IF ::nColPos > ::nRightVisible
         ::nColPos := ::nRightVisible
      ELSEIF ::nColPos > ::nFrozen .AND. ::nColPos < ::nLeftVisible
         ::nColPos := ::nLeftVisible
      ENDIF

      /* update column size and positions on the screen */
      nLeft := _TBR_COORD( ::n_Left )
      lFirst := .T.
      FOR nCol := 1 TO ::nRightVisible
         aCol := ::aColData[ nCol ]
         IF aCol[ _TBCI_CELLWIDTH ] > 0 .AND. ;
            ( nCol <= ::nFrozen .OR. nCol >= ::nLeftVisible )

            nFrozen := iif( nCol == ::nLeftVisible, Int( nWidth / 2 ), 0 )
            nColPos := nLeft += nFrozen
            nLeft += aCol[ _TBCI_COLWIDTH ]
            IF lFirst
               lFirst := .F.
            ELSE
               nLeft += aCol[ _TBCI_SEPWIDTH ]
            ENDIF
            nLast := iif( nCol == ::nRightVisible, ;
                          _TBR_COORD( ::n_Right ) - nLeft + 1, 0 )

            IF aCol[ _TBCI_COLPOS      ] != nColPos  .OR. ;
               aCol[ _TBCI_FROZENSPACE ] != nFrozen  .OR. ;
               aCol[ _TBCI_LASTSPACE   ] != nLast

               lFrames := .T.
               aCol[ _TBCI_COLPOS      ] := nColPos
               aCol[ _TBCI_FROZENSPACE ] := nFrozen
               aCol[ _TBCI_LASTSPACE   ] := nLast
            ENDIF
         ELSE
            IF aCol[ _TBCI_COLPOS ] != NIL
               lFrames := .T.
            ENDIF
            aCol[ _TBCI_COLPOS ] := NIL
         ENDIF
      NEXT
      FOR nCol := ::nRightVisible + 1 TO nColumns
         aCol := ::aColData[ nCol ]
         IF aCol[ _TBCI_COLPOS ] != NIL
            lFrames := .T.
         ENDIF
         aCol[ _TBCI_COLPOS ] := NIL
      NEXT

      ::nLastPos := ::nColPos

      IF lFrames
         ::lFrames := .T.
      ENDIF

   ENDIF

   RETURN Self


METHOD hiLite() CLASS XbpBrowse
#if 0
   LOCAL cValue, cColor

   IF ::nConfigure != 0
      ::doConfigure()
   ENDIF

   DispBegin()

   IF ::setCursorPos()
      IF ( cValue := ::cellValue( ::nRowPos, ::nColPos ) ) != NIL
         cColor := ::colorValue( ::cellColor( ::nRowPos, ::nColPos )[ _TBC_CLR_SELECTED ] )
         IF ::n_Col + Len( cValue ) > _TBR_COORD( ::n_Right )
            cValue := Left( cValue, _TBR_COORD( ::n_Right ) - ::n_Col + 1 )
         ENDIF
         hb_dispOutAt( ::n_Row, ::n_Col, cValue, cColor )
         SetPos( ::n_Row, ::n_Col )
         ::lHiLited := .T.
      ENDIF
   ENDIF

   DispEnd()
#endif
   RETURN Self


METHOD deHilite() CLASS XbpBrowse
#if 0
   LOCAL cValue, cColor

   IF ::nConfigure != 0
      ::doConfigure()
   ENDIF

   DispBegin()

   IF ::setCursorPos()
      IF ( cValue := ::cellValue( ::nRowPos, ::nColPos ) ) != NIL
         cColor := ::colorValue( ::cellColor( ::nRowPos, ::nColPos )[ _TBC_CLR_STANDARD ] )
         IF ::n_Col + Len( cValue ) > _TBR_COORD( ::n_Right )
            cValue := Left( cValue, _TBR_COORD( ::n_Right ) - ::n_Col + 1 )
         ENDIF
         hb_dispOutAt( ::n_Row, ::n_Col, cValue, cColor )
         SetPos( ::n_Row, ::n_Col )
      ENDIF
   ENDIF
   ::lHiLited := .F.

   DispEnd()
#endif
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
         _SETVISIBLE( ::aColData, _TBR_COORD( ::n_Right ) - _TBR_COORD( ::n_Left ) + 1, ;
                      @::nFrozen, @::nLeftVisible, @::nRightVisible )
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
//   LOCAL nRows := 6

   IF ::nConfigure != 0
      ::doConfigure()
   ENDIF

   #if 0
   nRows := _TBR_COORD( ::n_Bottom ) - _TBR_COORD( ::n_Top ) + 1 - ;
            ::nHeadHeight - iif( ::lHeadSep, 1, 0 ) - ;
            ::nFootHeight - iif( ::lFootSep, 1, 0 )

   RETURN iif( nRows > 0, nRows, 0 )
   #endif

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
   //::configure( _TBR_CONF_COLUMNS )

   ::doConfigure()  /* QT */

   RETURN Self


/* Delete a column object from a browse */
METHOD delColumn( nColumn ) CLASS XbpBrowse
   LOCAL oCol

   oCol := ::columns[ nColumn ]
   ADel( ::columns, nColumn )
   ASize( ::columns, Len( ::columns ) - 1 )
   ::configure( _TBR_CONF_COLUMNS )

   RETURN oCol


/* Insert a column object in a browse */
METHOD insColumn( nColumn, oCol ) CLASS XbpBrowse

   HB_AIns( ::columns, nColumn, oCol, .T. )
   ::configure( _TBR_CONF_COLUMNS )

   RETURN oCol


/* Replaces one TBColumn object with another */
METHOD setColumn( nColumn, oCol ) CLASS XbpBrowse
   LOCAL oPrevCol

   IF nColumn != NIL .AND. oCol != NIL
      nColumn := __eInstVar53( Self, "COLUMN", nColumn, "N", 1001 )
      oCol := __eInstVar53( Self, "COLUMN", oCol, "O", 1001 )

      oPrevCol := ::columns[ nColumn ]
      ::columns[ nColumn ] := oCol
      ::configure( _TBR_CONF_COLUMNS )
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

   LOCAL nTop, nLeft, nBottom, nRight, nRet, nCol
   LOCAL lFirst
   LOCAL aCol

   IF ::nConfigure != 0
      ::doConfigure()
   ENDIF

   ::mRowPos := ::mColPos := 0

   IF !ISNUMBER( mRow ) .OR. !ISNUMBER( mCol ) .OR. ;
      mRow < ( nTop    := _TBR_COORD( ::n_Top    ) ) .OR. ;
      mRow > ( nBottom := _TBR_COORD( ::n_Bottom ) ) .OR. ;
      mCol < ( nLeft   := _TBR_COORD( ::n_Left   ) ) .OR. ;
      mCol > ( nRight  := _TBR_COORD( ::n_Right  ) )
      RETURN HTNOWHERE
   ENDIF

   nRet := HTNOWHERE

   IF !Empty( ::cBorder )
      IF mRow == nTop - 1
         IF mCol == nLeft - 1
            nRet := HTTOPLEFT
         ELSEIF mCol == nRight + 1
            nRet := HTTOPRIGHT
         ELSE
            nRet := HTTOP
         ENDIF
      ELSEIF mRow == nBottom + 1
         IF mCol == nLeft - 1
            nRet := HTBOTTOMLEFT
         ELSEIF mCol == nRight + 1
            nRet := HTBOTTOMRIGHT
         ELSE
            nRet := HTBOTTOM
         ENDIF
      ELSEIF mCol == nLeft - 1
         nRet := HTLEFT
      ELSEIF mCol == nRight + 1
         nRet := HTRIGHT
      ENDIF
   ENDIF

   IF nRet == HTNOWHERE
      IF mRow < nTop + ::nHeadHeight
         nRet := HTHEADING
      ELSEIF ::lHeadSep .AND. mRow == nTop + ::nHeadHeight
         nRet := HTHEADSEP
      ELSEIF ::lFootSep .AND. mRow == nBottom - ::nFootHeight
         nRet := HTFOOTSEP
      ELSEIF mRow > nBottom - ::nFootHeight
         nRet := HTFOOTING
      ELSE
         nRet := HTCELL
         ::mRowPos := mRow - nTop - ::nHeadHeight - iif( ::lHeadSep, 1, 0 )

         lFirst := .T.
         nCol := 1
         DO WHILE nCol <= ::nRightVisible
            aCol := ::aColData[ nCol ]
            IF aCol[ _TBCI_COLPOS ] != NIL
               IF lFirst
                  lFirst := .F.
               ELSE
                  /* NOTE: CA-Cl*pper has bug here, it takes the size of
                   *       next column separator instead of the current one
                   */
                  IF ( nLeft += aCol[ _TBCI_SEPWIDTH ] ) > mCol
                     nRet := HTCOLSEP
                     EXIT
                  ENDIF
               ENDIF

               ::mColPos := nCol

               IF ( nLeft += aCol[ _TBCI_COLWIDTH ] + ;
                             aCol[ _TBCI_FROZENSPACE ] + ;
                             aCol[ _TBCI_LASTSPACE ] ) > mCol
                  EXIT
               ENDIF
            ENDIF
            IF nCol == ::nFrozen .AND. nCol < ::nLeftVisible
               nCol := ::nLeftVisible
            ELSE
               nCol++
            ENDIF
         ENDDO
      ENDIF
   ENDIF

   RETURN nRet


STATIC PROCEDURE _mBrwPos( oBrw, mRow, mCol )

   LOCAL nTop, nLeft, nBottom, nPos, nCol, aCol

   mRow := MRow()
   mCol := MCol()

   IF mRow >= ( nTop    := _TBR_COORD( oBrw:n_Top    ) ) .AND. ;
      mRow <= ( nBottom := _TBR_COORD( oBrw:n_Bottom ) ) .AND. ;
      mCol >= ( nLeft   := _TBR_COORD( oBrw:n_Left   ) ) .AND. ;
      mCol <= (            _TBR_COORD( oBrw:n_Right  ) )

      IF mRow < nTop + oBrw:nHeadHeight + iif( oBrw:lHeadSep, 1, 0 ) .OR. ;
         mRow > nBottom - oBrw:nFootHeight - iif( oBrw:lFootSep, 1, 0 )
         mRow := 0
      ELSE
         mRow -= nTop + oBrw:nHeadHeight - iif( oBrw:lHeadSep, 0, 1 )
      ENDIF

      nPos := 0
      nCol := 1
      DO WHILE nCol <= oBrw:nRightVisible
         aCol := oBrw:aColData[ nCol ]
         IF aCol[ _TBCI_COLPOS ] != NIL
            IF nPos != 0
               IF ( nLeft += aCol[ _TBCI_SEPWIDTH ] ) > mCol
                  EXIT
               ENDIF
            ENDIF
            nPos := nCol
            IF ( nLeft += aCol[ _TBCI_COLWIDTH ] + ;
                          aCol[ _TBCI_FROZENSPACE ] + ;
                          aCol[ _TBCI_LASTSPACE ] ) > mCol
               EXIT
            ENDIF
         ENDIF
         IF nCol == oBrw:nFrozen .AND. nCol < oBrw:nLeftVisible
            nCol := oBrw:nLeftVisible
         ELSE
            nCol++
         ENDIF
      ENDDO
      mCol := nPos
      IF nPos == 0
         mRow := 0
      ENDIF
   ELSE
      mRow := mCol := 0
   ENDIF

   RETURN


METHOD mRowPos() CLASS XbpBrowse

   LOCAL mRow, mCol

   IF ::nConfigure != 0
      ::doConfigure()
   ENDIF

   _mBrwPos( self, @mRow, @mCol )

   RETURN mRow


METHOD mColPos() CLASS XbpBrowse
   LOCAL mRow, mCol

   IF ::nConfigure != 0
      ::doConfigure()
   ENDIF

   _mBrwPos( self, @mRow, @mCol )

   RETURN mCol


METHOD border( cBorder ) CLASS XbpBrowse

   IF cBorder != NIL

      cBorder := __eInstVar53( Self, "BORDER", cBorder, "C", 1001 )

      IF Len( cBorder ) == 0 .OR. Len( cBorder ) == 8

         IF Empty( ::cBorder ) .AND. !Empty( cBorder )
            ::n_Top++
            ::n_Left++
            ::n_Bottom--
            ::n_Right--
            ::configure( _TBR_CONF_COLUMNS )
         ELSEIF !Empty( ::cBorder ) .AND. Empty( cBorder )
            ::n_Top--
            ::n_Left--
            ::n_Bottom++
            ::n_Right++
            ::configure( _TBR_CONF_COLUMNS )
         ENDIF

         ::cBorder := cBorder
      ENDIF
   ENDIF

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

   LOCAL bReturn
   LOCAL nPos

   /* NOTE: Assigned codeblock receives two parameters:
            {| oTBrowse, nKey | <action> } */

   IF ::keys == NIL
      ::keys := { { K_DOWN       , {| o | o:Down()    , TBR_CONTINUE   } },;
                  { K_END        , {| o | o:End()     , TBR_CONTINUE   } },;
                  { K_CTRL_PGDN  , {| o | o:GoBottom(), TBR_CONTINUE   } },;
                  { K_CTRL_PGUP  , {| o | o:GoTop()   , TBR_CONTINUE   } },;
                  { K_HOME       , {| o | o:Home()    , TBR_CONTINUE   } },;
                  { K_LEFT       , {| o | o:Left()    , TBR_CONTINUE   } },;
                  { K_PGDN       , {| o | o:PageDown(), TBR_CONTINUE   } },;
                  { K_PGUP       , {| o | o:PageUp()  , TBR_CONTINUE   } },;
                  { K_CTRL_END   , {| o | o:PanEnd()  , TBR_CONTINUE   } },;
                  { K_CTRL_HOME  , {| o | o:PanHome() , TBR_CONTINUE   } },;
                  { K_CTRL_LEFT  , {| o | o:PanLeft() , TBR_CONTINUE   } },;
                  { K_CTRL_RIGHT , {| o | o:PanRight(), TBR_CONTINUE   } },;
                  { K_RIGHT      , {| o | o:Right()   , TBR_CONTINUE   } },;
                  { K_UP         , {| o | o:Up()      , TBR_CONTINUE   } },;
                  { K_ESC        , {|   |               TBR_EXIT       } },;
                  { K_LBUTTONDOWN, {| o | TBMouse( o, MRow(), MCol() ) } } }

         AAdd( ::keys, { K_MWFORWARD  , {| o | o:Up()      , TBR_CONTINUE   } } )
         AAdd( ::keys, { K_MWBACKWARD , {| o | o:Down()    , TBR_CONTINUE   } } )
   ENDIF

   IF ( nPos := AScan( ::keys, {| x | x[ _TBC_SETKEY_KEY ] == nKey } ) ) == 0
      IF ISBLOCK( bBlock )
         AAdd( ::keys, { nKey, bBlock } )
      ENDIF
      bReturn := bBlock
   ELSEIF ISBLOCK( bBlock )
      ::keys[ nPos ][ _TBC_SETKEY_BLOCK ] := bBlock
      bReturn := bBlock
   ELSEIF PCount() == 1
      bReturn := ::keys[ nPos ][ _TBC_SETKEY_BLOCK ]
   ELSE
      bReturn := ::keys[ nPos ][ _TBC_SETKEY_BLOCK ]
      IF PCount() == 2 .AND. bBlock == NIL .AND. nKey != 0
         ADel( ::keys, nPos )
         ASize( ::keys, Len( ::keys ) - 1 )
      ENDIF
   ENDIF

   RETURN bReturn


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


FUNCTION TBMouse( oBrw, nMRow, nMCol )

   LOCAL n

   IF oBrw:hitTest( nMRow, nMCol ) == HTCELL

      IF ( n := oBrw:mRowPos - oBrw:rowPos ) < 0
         DO WHILE ++n <= 0
            oBrw:up()
         ENDDO
      ELSEIF n > 0
         DO WHILE --n >= 0
            oBrw:down()
         ENDDO
      ENDIF

      IF ( n := oBrw:mColPos - oBrw:colPos ) < 0
         DO WHILE ++n <= 0
            oBrw:left()
         ENDDO
      ELSEIF n > 0
         DO WHILE --n >= 0
            oBrw:right()
         ENDDO
      ENDIF

      RETURN TBR_CONTINUE
   ENDIF

   RETURN TBR_EXCEPTION

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
   METHOD setStyle( nStyle, lSetting )

   METHOD new( cHeading, bBlock )
   METHOD create( p1, p2, p3, p4, aPresParams )
   METHOD configure( p1, p2, p3, p4, aPresParams )

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
   ::blankVariable := IF( ::valtype == "N", 0, IF( ::valtype == "D", ctod( "" ), IF( ::valtype == "L", .f., "" ) ) )

   ::configure( aPresParams )

   RETURN Self

/*----------------------------------------------------------------------*/

METHOD configure( aPresParams ) CLASS XbpColumn
   LOCAL i, n

   /* Only pres parameters are to be adjusted again */

   IF !empty( aPresParams )
      FOR i := 1 TO len( aPresParams )
         Xbp_SetPresParam( ::aPresParams, aPresParams[ i,1 ], aPresParams[ i,2 ] )
      NEXT
   ENDIF

   /*  Heading Area */
   IF ( n := ascan( ::aPresParams, {|e_| e_[ 1 ] == XBP_PP_COL_HA_CAPTION } ) ) > 0
      ::cHeading := ::aPresParams[ n,2 ]
   ENDIF
   IF ( n := ascan( ::aPresParams, {|e_| e_[ 1 ] == XBP_PP_COL_HA_FGCLR } ) ) > 0
      ::hFgColor := ConvertAFact( "Color", XBTOQT_FROM_XB, ::aPresParams[ n,2 ] )
   ENDIF
   IF ( n := ascan( ::aPresParams, {|e_| e_[ 1 ] == XBP_PP_COL_HA_BGCLR } ) ) > 0
      ::hBgColor := ConvertAFact( "Color", XBTOQT_FROM_XB, ::aPresParams[ n,2 ] )
   ENDIF
   IF ( n := ascan( ::aPresParams, {|e_| e_[ 1 ] == XBP_PP_COL_HA_HEIGHT } ) ) > 0
      ::hHeight := ::aPresParams[ n,2 ]
   ENDIF
   IF ( n := ascan( ::aPresParams, {|e_| e_[ 1 ] == XBP_PP_COL_HA_ALIGNMENT } ) ) > 0
      ::hAlignment := ConvertAFact( "Alignment", XBTOQT_FROM_XB, ::aPresParams[ n,2 ] )
      ::hAlignment += Qt_AlignVCenter
   ENDIF

   /*  Data Area  */
   IF ( n := ascan( ::aPresParams, {|e_| e_[ 1 ] == XBP_PP_COL_DA_CELLALIGNMENT } ) ) > 0
      ::dAlignment := ::aPresParams[ n,2 ]
   ELSE
      ::dAlignment := IF( ::valtype == "N", Qt_AlignRight, IF( ::valtype $ "DL", Qt_AlignHCenter, Qt_AlignLeft ) )
      ::dAlignment += Qt_AlignVCenter
   ENDIF
   IF ( n := ascan( ::aPresParams, {|e_| e_[ 1 ] == XBP_PP_COL_DA_FGCLR } ) ) > 0
      ::dFgColor := ConvertAFact( "Color", XBTOQT_FROM_XB, ::aPresParams[ n,2 ] )
   ENDIF
   IF ( n := ascan( ::aPresParams, {|e_| e_[ 1 ] == XBP_PP_COL_DA_BGCLR } ) ) > 0
      ::dBgColor := ConvertAFact( "Color", XBTOQT_FROM_XB, ::aPresParams[ n,2 ] )
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
      ::fFgColor := ConvertAFact( "Color", XBTOQT_FROM_XB, ::aPresParams[ n,2 ] )
   ENDIF
   IF ( n := ascan( ::aPresParams, {|e_| e_[ 1 ] == XBP_PP_COL_FA_BGCLR } ) ) > 0
      ::fBgColor := ConvertAFact( "Color", XBTOQT_FROM_XB, ::aPresParams[ n,2 ] )
   ENDIF
   IF ( n := ascan( ::aPresParams, {|e_| e_[ 1 ] == XBP_PP_COL_FA_HEIGHT } ) ) > 0
      ::fHeight := ::aPresParams[ n,2 ]
   ENDIF
   IF ( n := ascan( ::aPresParams, {|e_| e_[ 1 ] == XBP_PP_COL_FA_ALIGNMENT } ) ) > 0
      ::fAlignment := ConvertAFact( "Alignment", XBTOQT_FROM_XB, ::aPresParams[ n,2 ] )
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



