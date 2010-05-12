/*
 * $Id$
 */

/*
 * Harbour Project source code:
 *
 * Copyright 2009-2010 Pritpal Bedi <bedipritpal@hotmail.com>
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
/*----------------------------------------------------------------------*/
/*----------------------------------------------------------------------*/
/*----------------------------------------------------------------------*/
/*
 *                                EkOnkar
 *                          ( The LORD is ONE )
 *
 *                            Harbour-Qt IDE
 *
 *                  Pritpal Bedi <pritpal@vouchcac.com>
 *                               08May2010
 */
/*----------------------------------------------------------------------*/
/*----------------------------------------------------------------------*/
/*----------------------------------------------------------------------*/

#include "common.ch"
#include "hbclass.ch"
#include "inkey.ch"
#include "hbide.ch"
#include "hbhrb.ch"

/*----------------------------------------------------------------------*/

STATIC s_aPlugins := { { "", NIL } }
STATIC s_aLoaded  := { { "", .f. } }

/*----------------------------------------------------------------------*/

FUNCTION hbide_loadPlugins( oIde, cVer )
   LOCAL cPath := hb_dirBase() + hb_osPathSeparator() + "plugins" + hb_osPathSeparator()
   LOCAL dir_, a_, cFile, pHrb, bBlock

   dir_:= directory( cPath + "*.hrb" )

   FOR EACH a_ IN dir_
      hb_fNameSplit( a_[ 1 ], , @cFile )
      pHrb := hb_hrbLoad( HB_HRB_BIND_OVERLOAD, cPath + cFile )

      IF ! Empty( pHrb ) .AND. ! Empty( hb_hrbGetFunSym( pHrb, cFile + "_init" ) )
         bBlock := &( "{|...| " + cFile + "_init(...) }" )

         IF eval( bBlock, oIde, cVer )
            IF ! Empty( hb_hrbGetFunSym( pHrb, cFile + "_exec" ) )
               aadd( s_aPlugins, { lower( cFile ), &( "{|...| " + cFile + "_exec(...) }" ), pHrb } )

            ENDIF
         ENDIF
      ENDIF
   NEXT

   RETURN bBlock

/*----------------------------------------------------------------------*/

FUNCTION hbide_execPlugin( cPlugin, oIde, ... )
   LOCAL n, lLoaded

   cPlugin := lower( cPlugin )

   IF ( n := ascan( s_aLoaded, {|e_| e_[ 1 ] == cPlugin } ) ) == 0
      lLoaded := hbide_loadAPlugin( cPlugin, oIde, "1.0" )
   ELSE
      lLoaded := s_aLoaded[ n,2 ]
   ENDIF
   IF lLoaded
      IF ( n := ascan( s_aPlugins, {|e_| e_[ 1 ] == cPlugin } ) ) > 0
         RETURN eval( s_aPlugins[ n, 2 ], oIde, ... )
      ENDIF
   ENDIF

   RETURN NIL

/*----------------------------------------------------------------------*/

STATIC FUNCTION hbide_loadAPlugin( cPlugin, oIde, cVer )
   LOCAL cPath, pHrb, bBlock, lLoaded

   cPath := hb_dirBase() + hb_osPathSeparator() + "plugins" + hb_osPathSeparator() + cPlugin + ".hrb"

   IF ( lLoaded := hb_fileExists( cPath ) )
      pHrb := hb_hrbLoad( HB_HRB_BIND_OVERLOAD, cPath )

      IF ! Empty( pHrb ) .AND. ! Empty( hb_hrbGetFunSym( pHrb, cPlugin + "_init" ) )
         bBlock := &( "{|...| " + cPlugin + "_init(...) }" )

         IF eval( bBlock, oIde, cVer )

            IF ! Empty( hb_hrbGetFunSym( pHrb, cPlugin + "_exec" ) )
               aadd( s_aPlugins, { cPlugin, &( "{|...| " + cPlugin + "_exec(...) }" ), pHrb } )
               lLoaded := .t.

            ENDIF
         ENDIF
      ENDIF
   ENDIF

   aadd( s_aLoaded, { cPlugin, lLoaded } )

   RETURN lLoaded

/*----------------------------------------------------------------------*/

