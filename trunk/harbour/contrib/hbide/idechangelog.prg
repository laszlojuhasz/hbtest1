/*
 * $Id$
 */

/*
 * Harbour Project source code:
 *
 * Copyright 2011 Pritpal Bedi <bedipritpal@hotmail.com>
 * www - http://harbour-project.org
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
 *                            Harbour-Qt IDE
 *
 *                 Pritpal Bedi <bedipritpal@hotmail.com>
 *                               25May2011
 */
/*----------------------------------------------------------------------*/
/*----------------------------------------------------------------------*/
/*----------------------------------------------------------------------*/

#include "hbide.ch"
#include "common.ch"
#include "hbclass.ch"
#include "hbqtgui.ch"

/*----------------------------------------------------------------------*/

CLASS IdeChangeLog INHERIT IdeObject

   DATA   aLog                                    INIT {}
   DATA   cUser                                   INIT ""
   DATA   nCntr                                   INIT 0

   METHOD new( oIde )
   METHOD create( oIde )
   METHOD destroy()
   METHOD show()
   METHOD execEvent( cEvent, p )
   METHOD updateLog()
   METHOD refresh()
   METHOD buildLogEntry()

   ENDCLASS

/*----------------------------------------------------------------------*/

METHOD IdeChangeLog:new( oIde )

   ::oIde := oIde

   RETURN Self

/*----------------------------------------------------------------------*/

METHOD IdeChangeLog:create( oIde )

   DEFAULT oIde TO ::oIde
   ::oIde := oIde

   RETURN Self

/*----------------------------------------------------------------------*/

METHOD IdeChangeLog:destroy()

   IF !empty( ::oUI )
      ::oUI:q_buttonNew   :disconnect( "clicked()" )

      ::oUI:destroy()
   ENDIF

   RETURN Self

/*----------------------------------------------------------------------*/

METHOD IdeChangeLog:show()

   IF empty( ::oUI )
      ::oUI := hbide_getUI( "changelog", ::oDlg:oWidget )
      ::oUI:setWindowFlags( Qt_Sheet )
      ::oUI:setWindowIcon( hbide_image( "hbide" ) )

      ::oUI:q_buttonChangelog :setIcon( hbide_image( "dc_folder"  ) )
      ::oUI:q_buttonAddSrc    :setIcon( hbide_image( "dc_plus"  ) )

      ::oUI:q_buttonChangelog :connect( "clicked()", {|| ::execEvent( "buttonChangelog_clicked" ) } )
      ::oUI:q_buttonAddSrc    :connect( "clicked()", {|| ::execEvent( "buttonAddSrc_clicked"    ) } )
      ::oUI:q_buttonDone      :connect( "clicked()", {|| ::execEvent( "buttonDone_clicked"      ) } )
      ::oUI:q_buttonRefresh   :connect( "clicked()", {|| ::execEvent( "buttonRefresh_clicked"   ) } )
      ::oUI:q_buttonSave      :connect( "clicked()", {|| ::execEvent( "buttonSave_clicked"      ) } )
      ::oUI:q_buttonSrcDescOK :connect( "clicked()", {|| ::execEvent( "buttonSrcDesc_clicked"   ) } )

      ::oUI:q_editChangelog   :connect( "textChanged(QString)", {|p| ::execEvent( "editChangelog_textChanged", p   ) } )

      ::oUI:q_comboAction:addItem( "! Fixed     : " )
      ::oUI:q_comboAction:addItem( "& Changed   : " )
      ::oUI:q_comboAction:addItem( "% Optimized : " )
      ::oUI:q_comboAction:addItem( "+ Added     : " )
      ::oUI:q_comboAction:addItem( "- Removed   : " )
      ::oUI:q_comboAction:addItem( "; Comment   : " )
      ::oUI:q_comboAction:addItem( "@ TODO      : " )
      ::oUI:q_comboAction:addItem( "| Moved     : " )

      ::oUI:q_editChangelog:setText( ::oINI:cChangeLog )
      ::updateLog()

      ::cUser := hbide_fetchAString( ::oDlg:oWidget, , , "Developer Name" )

      ::oUI:q_comboAuthor:addItem( ::cUser )
   ENDIF

   ::oUI:show()

   RETURN Self

/*----------------------------------------------------------------------*/

STATIC FUNCTION hbide_eol()
   RETURN hb_eol() // chr( 13 ) + chr( 10 )

/*----------------------------------------------------------------------*/

STATIC FUNCTION hbide_getLogCounter( cBuffer )
   LOCAL n, n1, nCntr := 0

   IF ( n := at( "$<", cBuffer ) ) > 0
      n1 := at( ">", cBuffer )
      nCntr := val( substr( cBuffer, n + 2, n1 - n - 2 ) )
   ENDIF

   RETURN nCntr + 1

/*----------------------------------------------------------------------*/

METHOD IdeChangeLog:execEvent( cEvent, p )
   LOCAL cTmp, cTmp1, s, n

   HB_SYMBOL_UNUSED( p )

   SWITCH cEvent

   CASE "buttonSave_clicked"
      //IF !empty( cTmp := ::oUI:q_plainLogEntry:toPlainText() )
      IF ! empty( ::aLog )
         cTmp1 := hb_memoread( ::oINI:cChangeLog )
         ::nCntr := hbide_getLogCounter( cTmp1 )
         s := "$<" + strzero( ::nCntr, 6 ) + "> " + hbide_dtosFmt() + " " + left( time(), 5 ) + " " + ::cUser

         IF ( n := at( "$<", cTmp1 ) ) > 0
            cTmp1 := substr( cTmp1, 1, n - 1 ) + s + hbide_eol() + ::buildLogEntry() + hbide_eol() + substr( cTmp1, n )
         ELSE
            cTmp1 += hbide_eol() + s + hbide_eol() + cTmp + hbide_eol()
         ENDIF
         hb_memowrit( ::oINI:cChangeLog, cTmp1 )  /* TODO: put it under locking protocol */
         ::aLog := {}
         ::updateLog()
      ENDIF
      EXIT
   CASE "buttonRefresh_clicked"
      ::aLog := {}
      ::refresh()
      EXIT
   CASE "buttonSrcDesc_clicked"
      IF ! empty( cTmp := ::oUI:q_editSource:text() )
         aadd( ::aLog, { "Source", cTmp, "" } )
      ENDIF
      IF ! empty( cTmp := ::oUI:q_plainCurrentLog:toPlainText() )
         aadd( ::aLog, { "Desc", ::oUI:q_comboAction:currentText(), cTmp } )
         ::oUI:q_plainCurrentLog:clear()
      ENDIF
      ::refresh()
      EXIT
   CASE "buttonDone_clicked"
      IF ! empty( cTmp := ::oUI:q_plainCurrentLog:toPlainText() )
         aadd( ::aLog, { "Desc", ::oUI:q_comboAction:currentText(), cTmp } )
         ::oUI:q_plainCurrentLog:clear()
         ::refresh()
      ENDIF
      EXIT
   CASE "buttonAddSrc_clicked"
      IF ! empty( cTmp := ::oUI:q_editSource:text() )
         aadd( ::aLog, { "Source", cTmp, "" } )
         ::refresh()
      ENDIF
      EXIT
   CASE "buttonChangelog_clicked"
      cTmp := hbide_fetchAFile( ::oDlg, "Select a ChangeLog File" )
      IF ! empty( cTmp ) .AND. hb_fileExists( cTmp )
         ::oINI:cChangeLog := cTmp
         ::oUI:q_editChangelog:setText( ::oINI:cChangeLog )
      ENDIF
      EXIT
   CASE "editChangelog_textChanged"
      IF ! empty( p ) .AND. hb_fileExists( p )
         ::oUI:q_editChangelog:setStyleSheet( "" )
         ::updateLog()
      ELSE
         ::oUI:q_editChangelog:setStyleSheet( "background-color: rgba( 240,120,120,255 );" )
      ENDIF
      EXIT

   ENDSWITCH

   RETURN NIL

/*----------------------------------------------------------------------*/

METHOD IdeChangeLog:updateLog()

   ::oUI:q_plainLogEntry:clear()
   ::oUI:q_plainCurrentLog:clear()
   ::oUI:q_plainChangelog:clear()

   ::oUI:q_plainChangelog:setPlainText( hb_memoread( ::oINI:cChangeLog ) )

   ::refresh()

   RETURN Self

/*----------------------------------------------------------------------*/

STATIC FUNCTION hbide_dtosFmt( dDate )
   LOCAL s

   DEFAULT dDate TO date()

   s := dtos( dDate )

   RETURN substr( s, 1, 4 ) + "-" + substr( s, 5, 2 ) + "-" + substr( s, 7, 2 )

/*----------------------------------------------------------------------*/

METHOD IdeChangeLog:refresh()

   ::oUI:q_plainLogEntry:clear()
   ::oUI:q_plainLogEntry:setPlainText( ::buildLogEntry() )

   RETURN Self

/*----------------------------------------------------------------------*/

METHOD IdeChangeLog:buildLogEntry()
   LOCAL s := "", a_, k, e

   FOR EACH a_ IN ::aLog
      IF a_[ 1 ] == "Source"
         s += "  * " + upper( a_[ 2 ] ) + hbide_eol()
      ELSEIF a_[ 1 ] == "Desc"
         k := hbide_memoToArray( a_[ 3 ] )
         FOR EACH e IN k
            IF e:__enumIndex() == 1
               s += "    " + a_[ 2 ] + e + hbide_eol()
            ELSE
               s += "    " + space( 14 ) + e + hbide_eol()
            ENDIF
         NEXT
      ENDIF
   NEXT

   RETURN s

/*----------------------------------------------------------------------*/
