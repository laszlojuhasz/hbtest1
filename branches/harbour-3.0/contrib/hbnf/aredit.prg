/*
 * $Id$
 */

/*
 * File......: aredit.prg
 * Author....: James J. Orlowski, M.D.
 * CIS ID....: 72707,601
 *
 *
 * Modification history:
 * ---------------------
 *
 *    Rev 1.2   15 Aug 1991 23:05:56   GLENN
 * Forest Belt proofread/edited/cleaned up doc
 *
 *    Rev 1.1   12 Jun 1991 00:42:38   GLENN
 * A referee suggested changing the documentation such that the return value
 * is shown as "xElement" rather than "cElement" because the function
 * can return different types.
 *
 *    Rev 1.0   07 Jun 1991 23:03:24   GLENN
 * Initial revision.
 *
 *
 */

/*

    Some notes:

       The tbmethods section is a short cut from Spence's book instead
       of using the longer DO CASE method.

       Jim Gale showed me the basic array browser and Robert DiFalco
       showed me the improved  skipblock in public messages on Nanforum.

       I added the functionality of the "Edit Get" code block
       (ie bGetFunc), TestGet() demo, and the add/delete rows.

*/

#include "inkey.ch"

* Default heading, column, footer separators
#define DEF_HSEP    "���"
#define DEF_CSEP    " � "
#define DEF_FSEP    "���"

* Default info for tb_methods section
#define KEY_ELEM 1
#define BLK_ELEM 2

#ifdef FT_TEST
   PROCEDURE Test
      * Thanks to Jim Gale for helping me understand the basics
      LOCAL i, ar[3, 26], aBlocks[3], aHeadings[3], nElem := 1, bGetFunc, cRet
      * set up 2 dimensional array ar[]
      FOR i := 1 TO 26
         ar[1, i] := i          //  1  ->  26  Numeric
         ar[2, i] := CHR(i+64)  // "A" -> "Z"  Character
         ar[3, i] := CHR(91-i)  // "Z" -> "A"  Character
      NEXT i
      * Set Up aHeadings[] for column headings
      aHeadings  := { "Numbers", "Letters", "Reverse" }
      * Set Up Blocks Describing Individual Elements in Array ar[]
      aBlocks[1] := {|| STR(ar[1, nElem], 2)}  // to prevent default 10 spaces
      aBlocks[2] := {|| ar[2, nElem]}
      aBlocks[3] := {|| ar[3, nElem]}
      * Set up TestGet() as bGetFunc
      bGetFunc   := {|b, ar, nDim, nElem|TestGet(b, ar, nDim, nElem)}

      SET SCOREBOARD OFF
      SetColor( "W/N")
      CLEAR SCREEN
      @ 21,4 SAY "Use Cursor Keys To Move Between Fields, <F7> = Delete Row, <F8> = Add Row"
      @ 22,7 SAY "<ESC> = Quit Array Edit, <Enter> or <Any Other Key> Edits Element"
      SetColor( "N/W, W/N, , , W/N" )
      cRet := FT_ArEdit(3, 5, 18, 75, ar, @nElem, aHeadings, aBlocks, bGetFunc)
      SetColor( "W/N")
      CLEAR SCREEN
      ? cRet
      ? "Lastkey() = ESC:", LASTKEY() == K_ESC
   RETURN

   FUNCTION TestGet( b, ar, nDim, nElem)
      LOCAL GetList   := {}
      LOCAL nRow      := ROW()
      LOCAL nCol      := COL()
      LOCAL cSaveScrn := SAVESCREEN(21, 0, 22, MaxCol())
      LOCAL cOldColor := SetColor( "W/N")
      @ 21, 0 CLEAR TO 22, MaxCol()
      @ 21,29 SAY "Editing Array Element"
      SetColor(cOldColor)
      DO CASE
         CASE nDim == 1
            @ nRow, nCol GET ar[1, nElem] PICTURE "99"
            READ
            b:refreshAll()
         CASE nDim == 2
            @ nRow, nCol GET ar[2, nElem] PICTURE "!"
            READ
            b:refreshAll()
         CASE nDim == 3
            @ nRow, nCol GET ar[3, nElem] PICTURE "!"
            READ
            b:refreshAll()
      ENDCASE
      RESTSCREEN(21, 0, 22, MaxCol(), cSaveScrn)
      @ nRow, nCol SAY ""
   RETURN .t.
#endif

FUNCTION FT_ArEdit( nTop, nLeft, nBot, nRight, ;
                   ar, nElem, aHeadings, aBlocks, bGetFunc)
   * ANYTYPE[]   ar        - Array to browse
   * NUMERIC     nElem     - Element In Array
   * CHARACTER[] aHeadings - Array of Headings for each column
   * BLOCK[]     aBlocks   - Array containing code block for each column.
   * CODE BLOCK  bGetFunc  - Code Block For Special Get Processing
   *  NOTE: When evaluated a code block is passed the array element to
   *          be edited

   LOCAL exit_requested, nKey, meth_no, ;
         cSaveWin, i, b, column
   LOCAL nDim, cType, cVal
   LOCAL tb_methods := ;
         { ;
           {K_DOWN,       {|b| b:down()}}, ;
           {K_UP,         {|b| b:up()}}, ;
           {K_PGDN,       {|b| b:pagedown()}}, ;
           {K_PGUP,       {|b| b:pageup()}}, ;
           {K_CTRL_PGUP,  {|b| b:gotop()}}, ;
           {K_CTRL_PGDN,  {|b| b:gobottom()}}, ;
           {K_RIGHT,      {|b| b:right()}}, ;
           {K_LEFT,       {|b| b:left()}}, ;
           {K_HOME,       {|b| b:home()}}, ;
           {K_END,        {|b| b:end()}}, ;
           {K_CTRL_LEFT,  {|b| b:panleft()}}, ;
           {K_CTRL_RIGHT, {|b| b:panright()}}, ;
           {K_CTRL_HOME,  {|b| b:panhome()}}, ;
           {K_CTRL_END,   {|b| b:panend()}} ;
         }

   cSaveWin := SaveScreen(nTop, nLeft, nBot, nRight)
   @ nTop, nLeft TO nBot, nRight

   b := TBrowseNew(nTop + 1, nLeft + 1, nBot - 1, nRight - 1)
   b:headsep := DEF_HSEP
   b:colsep  := DEF_CSEP
   b:footsep := DEF_FSEP

   b:gotopblock    := {|| nElem := 1}
   b:gobottomblock := {|| nElem := LEN(ar[1])}

   * skipblock originally coded by Robert DiFalco
   b:SkipBlock     := {|nSkip, nStart| nStart := nElem,;
      nElem := MAX( 1, MIN( LEN(ar[1]), nElem + nSkip ) ),;
      nElem - nStart }

   FOR i := 1 TO LEN(aBlocks)
       column := TBColumnNew(aHeadings[i], aBlocks[i] )
       b:addcolumn(column)
   NEXT

   exit_requested := .F.
   DO WHILE !exit_requested

      DO WHILE NEXTKEY() == 0 .AND. !b:stabilize()
      ENDDO

      nKey := INKEY(0)

      meth_no := ASCAN(tb_methods, {|elem| nKey == elem[KEY_ELEM]})
      IF meth_no != 0
          EVAL(tb_methods[meth_no, BLK_ELEM], b)
      ELSE
          DO CASE
              CASE nKey == K_F7
                  FOR nDim := 1 TO LEN(ar)
                     ADEL(ar[nDim], nElem)
                     ASIZE(ar[nDim], LEN(ar[nDim]) - 1)
                  NEXT
                  b:refreshAll()

              CASE nKey == K_F8
                  FOR nDim := 1 TO LEN(ar)
                     * check valtype of current element before AINS()
                     cType := VALTYPE(ar[nDim, nElem])
                     cVal  := ar[nDim, nElem]
                     ASIZE(ar[nDim], LEN(ar[nDim]) + 1)
                     AINS(ar[nDim], nElem)
                     IF cType == "C"
                        ar[nDim, nElem] := SPACE(LEN(cVal))
                     ELSEIF cType == "N"
                        ar[nDim, nElem] := 0
                     ELSEIF cType == "L"
                        ar[nDim, nElem] := .f.
                     ELSEIF cType == "D"
                        ar[nDim, nElem] := CTOD("  /  /  ")
                     ENDIF
                  NEXT
                  b:refreshAll()

              CASE nKey == K_ESC
                  exit_requested := .T.

              * Other exception handling ...
              CASE VALTYPE(bGetFunc) == "B"
                 IF nKey != K_ENTER
                    * want last key to be part of GET edit so KEYBOARD it
                    KEYBOARD CHR(LASTKEY())
                 ENDIF
                 EVAL(bGetFunc, b, ar, b:colPos, nElem )
                 * after get move to next field
                 KEYBOARD iif(b:colPos < b:colCount, ;
                              CHR(K_RIGHT), CHR(K_HOME) + CHR(K_DOWN) )

              * Placing K_ENTER here below Edit Block (i.e. bGetFunc)
              * defaults K_ENTER to Edit when bGetFunc Is Present
              * BUT if no bGetFunc, then K_ENTER selects element to return
              CASE nKey == K_ENTER
                  exit_requested := .T.

          ENDCASE
      ENDIF // meth_no != 0
   ENDDO // WHILE !exit_requested
   RestScreen(nTop, nLeft, nBot, nRight, cSaveWin)
   * if no bGetFunc then ESC returns 0, otherwise return value of last element
RETURN iif( VALTYPE(bGetFunc) == NIL .AND. nKey == K_ESC, ;
            0, ar[b:colPos, nElem] )
* EOFcn FT_ArEdit()
