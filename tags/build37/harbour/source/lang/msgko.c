/*
 * $Id$
 */

/*
 * Harbour Project source code:
 * Language Support Module (KR)
 *
 * Copyright 2000 Kwon, Oh-Jun  <ohjun@fivetech.net>
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

/* Language name: <Your language> */
/* ISO language code (2 chars): (please look it up in /doc/lang_id.txt) */
/* Codepage: <Your codepage> */

#include "hbapilng.h"

static HB_LANG s_lang =
{
   {
      /* Identification */

      "KO",                        /* ID */
      "Korean",                    /* Name (in English) */
      "한국어",                    /* Name (in native language) */
      "KO",                        /* RFC ID */
      "949",                       /* Codepage */
      "$Revision$ $Date$",         /* Version */

      /* Month names */

      "1월",
      "2월",
      "3월",
      "4월",
      "5월",
      "6월",
      "7월",
      "8월",
      "9월",
      "10월",
      "11월",
      "12월",

      /* Day names */

      "일요일",
      "월요일",
      "화요일",
      "수요일"
      "목요일",
      "금요일",
      "토요일",

      /* CA-Cl*pper compatible natmsg items */

      "데이터 파일  s    # 레코드수   최종 수정일     크기",
      "다른 예제를 원하십니까?",
      "페이지 번호.",
      "** 소계 **",
      "* (중간)소계 *",
      "*** 총계 ***",
      "삽입",
      "    ",
      "잘못된 날짜",
      "범위: ",
      " - ",
      "예/아니오",
      "잘못된 표현식",

      /* Error description names */

      "Unknown error",
      "Argument error",
      "Bound error",
      "String overflow",
      "Numeric overflow",
      "Zero divisor",
      "Numeric error",
      "Syntax error",
      "Operation too complex",
      "",
      "",
      "Memory low",
      "Undefined function",
      "No exported method",
      "Variable does not exist",
      "Alias does not exist",
      "No exported variable",
      "Illegal characters in alias",
      "Alias already in use",
      "",
      "Create error",
      "Open error",
      "Close error",
      "Read error",
      "Write error",
      "Print error",
      "",
      "",
      "",
      "",
      "Operation not supported",
      "Limit exceeded",
      "Corruption detected",
      "Data type error",
      "Data width error",
      "Workarea not in use",
      "Workarea not indexed",
      "Exclusive required",
      "Lock required",
      "Write not allowed",
      "Append lock failed",
      "Lock Failure",
      "",
      "",
      "",
      "",
      "array access",
      "array assign",
      "array dimension",
      "not an array",
      "conditional",

      /* Internal error names */

      "Unrecoverable error %lu: ",
      "Error recovery failure",
      "No ERRORBLOCK() for error",
      "Too many recursive error handler calls",
      "RDD invalid or failed to load",
      "Invalid method type from %s",
      "hb_xgrab can't allocate memory",
      "hb_xrealloc called with a NULL pointer",
      "hb_xrealloc called with an invalid pointer",
      "hb_xrealloc can't reallocate memory",
      "hb_xfree called with an invalid pointer",
      "hb_xfree called with a NULL pointer",
      "Can\'t locate the starting procedure: \'%s\'",
      "No starting procedure",
      "Unsupported VM opcode",
      "Symbol item expected from %s",
      "Invalid symbol type for self from %s",
      "Codeblock expected from %s",
      "Incorrect item type on the stack trying to pop from %s",
      "Stack underflow",
      "An item was going to be copied to itself from %s",
      "Invalid symbol item passed as memvar %s",
      "Memory buffer overflow",

      /* Texts */

      "YYYY/MM/DD",
      "Y",
      "N"
   }
};

HB_LANG_ANNOUNCE( KO );

HB_CALL_ON_STARTUP_BEGIN( hb_lang_Init_KO )
   hb_langRegister( &s_lang );
HB_CALL_ON_STARTUP_END( hb_lang_Init_KO )
#if ! defined(__GNUC__) && ! defined(_MSC_VER)
   #pragma startup hb_lang_Init_KO
#endif
