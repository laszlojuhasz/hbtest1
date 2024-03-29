/*
 * $Id$
 */

/*
 * Harbour Project source code:
 * Text file browser class
 *
 * Copyright 2001 Maurilio Longo <maurilio.longo@libero.it>
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

#include "hbclass.ch"
#include "common.ch"
#include "fileio.ch"
#include "inkey.ch"


// Color definitions and positions inside ::cColorSpec
#define  CLR_CODE       0        // color of code
#define  CLR_CURSOR     1        // color of highlighted line (the line to be executed)
#define  CLR_BKPT       2        // color of breakpoint line
#define  CLR_HIBKPT     3        // color of highlighted breakpoint line


CLASS TBrwText FROM HBEditor

   DATA  cFileName      // the name of the browsed file
   DATA  nActiveLine    // Active line inside Code Window (the line to be executed)

   DATA  aBreakPoints   // Array with line numbers of active Break Points

   DATA  lLineNumbers   // If .T. source code lines are preceded by their number

   ACCESS colorSpec INLINE ::cColorSpec
   ASSIGN colorSpec(cClr) INLINE ::cColorSpec:=cClr

   METHOD   New(nTop, nLeft, nBottom, nRight, cFileName, cColor)

   METHOD   GoTop()           // Methods available on a standard TBrowse, needed to handle a HBEditor like a TBrowse
   METHOD   GoBottom()
   METHOD   Up()
   METHOD   Down()
   METHOD   Left()
   METHOD   Right()
   METHOD   PageUp()
   METHOD   PageDown()
   METHOD   RefreshAll()
   METHOD   RefreshCurrent()
   METHOD   ForceStable() INLINE NIL

   METHOD   GotoLine(n)                      // Moves active line cursor
   METHOD   SetActiveLine( n )               // Sets the line to be executed

   METHOD   GetLine(nRow)                    // Redefine HBEditor method to add line number
   METHOD   LineColor(nRow)                  // Redefine HBEditor method to handle line coloring

   METHOD   ToggleBreakPoint(nRow, lSet)     // if lSet is .T. there is a BreakPoint active at nRow,
                                             // if lSet is .F. BreakPoint at nRow has to be removed
   METHOD   Search( cString, lCaseSensitive, nMode ) // 0 from Begining to end, 1 Forward, 2 Backwards

   METHOD   LoadFile(cFileName)

ENDCLASS


METHOD New(nTop, nLeft, nBottom, nRight, cFileName, cColor, lLineNumbers) CLASS TBrwText

   DEFAULT cColor TO SetColor()
   DEFAULT lLineNumbers TO .T.

   ::cFileName := cFileName
   ::nActiveLine := 1

   ::aBreakPoints := {}

   ::lLineNumbers := lLineNumbers

   Super:New("", nTop, nLeft, nBottom, nRight, .T.)
   Super:SetColor(cColor)

   Super:LoadFile(cFileName)

return Self


METHOD LoadFile(cFileName) CLASS TBrwText

   Super:LoadFile(cFileName)

return Self


METHOD GoTop() CLASS TBrwText
   ::MoveCursor(K_CTRL_PGUP)
return Self


METHOD GoBottom() CLASS TBrwText
   ::MoveCursor(K_CTRL_PGDN)
return Self


METHOD Up() CLASS TBrwText
   ::MoveCursor(K_UP)
return Self


METHOD Left() CLASS TBrwText
   ::MoveCursor(K_LEFT)
return Self


METHOD Right() CLASS TBrwText
   ::MoveCursor(K_RIGHT)
return Self


METHOD Down() CLASS TBrwText
   ::MoveCursor(K_DOWN)
return Self


METHOD PageUp() CLASS TBrwText
   ::MoveCursor(K_PGUP)
return Self


METHOD PageDown() CLASS TBrwText
   ::MoveCursor(K_PGDN)
return Self


METHOD RefreshAll() CLASS TBrwText
   ::RefreshWindow()
return Self


METHOD RefreshCurrent() CLASS TBrwText
   ::RefreshLine()
return Self


METHOD SetActiveLine( n ) CLASS TBrwText
   ::nActiveLine := n
   ::RefreshWindow()
return Self


METHOD GotoLine(n) CLASS TBrwText

   Super:GotoLine(n)

return Self


METHOD GetLine(nRow) CLASS TBrwText

return iif(::lLineNumbers, AllTrim(Str(nRow)) + ": ", "") + Super:GetLine(nRow)


METHOD LineColor(nRow) CLASS TBrwText

   local cColor, lHilited, lBreak, nIndex := CLR_CODE

   lHilited := (nRow == ::nActiveLine)
   lBreak := AScan(::aBreakPoints, nRow) > 0

   if lHilited
     nIndex += CLR_CURSOR
   endif
   if lBreak
     nIndex += CLR_BKPT
   endif

   cColor := hb_ColorIndex(::cColorSpec, nIndex)

return cColor


METHOD ToggleBreakPoint(nRow, lSet) CLASS TBrwText

   local nAt := AScan(::aBreakPoints, nRow)

   if lSet
      // add it only if not present
      if nAt == 0
         AAdd(::aBreakPoints, nRow)
      endif

   else
      if nAt <> 0
         ADel( ::aBreakPoints, nAt )
         ASize( ::aBreakPoints, Len( ::aBreakPoints ) - 1 )
      endif

   endif

return Self

METHOD Search( cString, lCaseSensitive, nMode ) CLASS TBrwText

   local nFrom, nTo, nStep, nFor
   local lFound

   DEFAULT lCaseSensitive TO .f., ;
           nMode          TO 0

   lFound := .f.

   if !lCaseSensitive
      cString := Upper( cString )
   endif

   do case
   case nMode == 0 // From Top
      nFrom := 1
      nTo   := ::naTextLen
      nStep := 1
   case nMode == 1 // Forward
      nFrom := Min( ::nRow + 1, ::naTextLen )
      nTo   := ::naTextLen
      nStep := 1
   case nMode == 2 // Backward
      nFrom := Max( ::nRow - 1, 1 )
      nTo   := 1
      nStep := -1
   end case

   for nFor := nFrom to nTo Step nStep
      if cString $ iif( lCaseSensitive, ::GetLine( nFor ), Upper( ::GetLine( nFor ) ) )
         lFound := .t.
         ::GotoLine( nFor )
         exit
      endif
   next

return lFound

