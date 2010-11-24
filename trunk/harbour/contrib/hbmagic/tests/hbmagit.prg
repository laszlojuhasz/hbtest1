/*
 * $Id$
 */

/*
 * Harbour Project source code:
 * hbmagic test suite
 *
 * Copyright 2010. Tamas TEVESZ <ice@extreme.hu>
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

#include "common.ch"
#include "hbmagic.ch"
#include "simpleio.ch"

#xcommand T( <(title)>, <(subject)> ) => ;
          magic_setflags( hMagic, MAGIC_NONE ) ;;
          OutStd( hb_strFormat( <title> + ": t: [%s] ", magic_buffer( hMagic, <subject> ) ) ) ;;
          magic_setflags( hMagic, MAGIC_MIME_TYPE ) ;;
          OutStd( hb_strFormat( "m: [%s]", magic_buffer( hMagic, <subject> ) ) + hb_eol() )

PROCEDURE Main()

   LOCAL cJpeg, cPng, cGif, cElf, cExe, cCom
   LOCAL hMagic

   cJpeg := hb_base64decode( ;
            "/9j/4AAQSkZJRgABAQEASABIAAD/2wBDAAEBAQEBAQEBAQEBAQEBAQEBAQEBAQEBAQEBAQEBAQEB" + ;
            "AQEBAQEBAQEBAQEBAQEBAQEBAQEBAQEBAQEBAQEBAQH/2wBDAQEBAQEBAQEBAQEBAQEBAQEBAQEB" + ;
            "AQEBAQEBAQEBAQEBAQEBAQEBAQEBAQEBAQEBAQEBAQEBAQEBAQEBAQEBAQH/wAARCAABAAEDASIA" + ;
            "AhEBAxEB/8QAHwAAAQUBAQEBAQEAAAAAAAAAAAECAwQFBgcICQoL/8QAtRAAAgEDAwIEAwUFBAQA" + ;
            "AAF9AQIDAAQRBRIhMUEGE1FhByJxFDKBkaEII0KxwRVS0fAkM2JyggkKFhcYGRolJicoKSo0NTY3" + ;
            "ODk6Q0RFRkdISUpTVFVWV1hZWmNkZWZnaGlqc3R1dnd4eXqDhIWGh4iJipKTlJWWl5iZmqKjpKWm" + ;
            "p6ipqrKztLW2t7i5usLDxMXGx8jJytLT1NXW19jZ2uHi4+Tl5ufo6erx8vP09fb3+Pn6/8QAHwEA" + ;
            "AwEBAQEBAQEBAQAAAAAAAAECAwQFBgcICQoL/8QAtREAAgECBAQDBAcFBAQAAQJ3AAECAxEEBSEx" + ;
            "BhJBUQdhcRMiMoEIFEKRobHBCSMzUvAVYnLRChYkNOEl8RcYGRomJygpKjU2Nzg5OkNERUZHSElK" + ;
            "U1RVVldYWVpjZGVmZ2hpanN0dXZ3eHl6goOEhYaHiImKkpOUlZaXmJmaoqOkpaanqKmqsrO0tba3" + ;
            "uLm6wsPExcbHyMnK0tPU1dbX2Nna4uPk5ebn6Onq8vP09fb3+Pn6/9oADAMBAAIRAxEAPwD+/iii" + ;
            "igD/2Q==" )
   cPng := hb_base64decode( ;
            "iVBORw0KGgoAAAANSUhEUgAAAAEAAAABCAIAAACQd1PeAAAAAXNSR0IArs4c6QAAAAxJREFUCNdj" + ;
            "+P//PwAF/gL+3MxZ5wAAAABJRU5ErkJggg==" )
   cGif := hb_base64decode( ;
            "R0lGODdhAQABAIAAAP///////ywAAAAAAQABAAACAkQBADs=" )
   cElf := hb_base64decode( ;
            "f0VMRgIBAQAAAAAAAAAAAAIAPgABAAAA4CZAAAAAAAA=" )
   cExe := hb_base64decode( ;
            "TVpAAAEAAAAGAAAA//8AALgAAAAAAAAAYAAAAAAAAAA=" )
   cCom := hb_base64decode( ;
            "6fEAUE1PREUvVyBWZXJzaW9uIENoZWNrIFV0aWxpdHk=" )

   hMagic := magic_open()
   IF Empty( hMagic ) .OR. magic_load( hMagic ) != 0
      OutStd( "magic_open()/magic_load() failed" + hb_eol() )
      QUIT
   ENDIF

   T( "JPEG Image", cJpeg )
   T( "PNG Image", cPng )
   T( "GIF Image", cGif )
   T( "ELF binary", cElf )
   T( "EXE binary", cExe )
   T( "COM binary", cCom )
   T( "Short buffer", " " )
   T( "Empty buffer", "" )
   T( "Null buffer", nil )

   OutStd( "hb_Magic_Simple(): t: [" + hb_Magic_Simple( hb_argv( 0 ), MAGIC_NONE ) + "] " + ;
            "m: [" + hb_Magic_Simple( hb_argv( 0 ), MAGIC_MIME_TYPE ) + "]" + hb_eol() )

   RETURN
