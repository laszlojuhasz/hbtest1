/*
 * $Id$
 */

/*
 * Harbour Project source code:
 * Language Support Module (CSUTF)
 *
 * Copyright 2000 Viktor Szakats (harbour syenar.net) (English, from msg_tpl.c)
 * Copyright 2000 Roman Masek <woodoo@iol.cz>
 * Copyright 2000 Davor Siklic <siki@msoft.cz>
 * Copyright 2006 Vojtech Obrdlik <vobrdlik@centrum.cz>
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

/* Language name: Czech */
/* ISO language code (2 chars): CS */
/* Codepage: UTF-8 */

#include "hbapilng.h"

static HB_LANG s_lang =
{
   {
      /* Identification */

      "CSUTF",                     /* ID */
      "Czech",                     /* Name (in English) */
      "Česky",                     /* Name (in native language) */
      "CS",                        /* RFC ID */
      "UTF-8",                     /* Codepage */
      "",                          /* Version */

      /* Month names */

      "leden",
      "únor",
      "březen",
      "duben",
      "květen",
      "červen",
      "červenec",
      "srpen",
      "září",
      "říjen",
      "listopad",
      "prosinec",

      /* Day names */

      "neděle",
      "pondělí",
      "úterý",
      "středa",
      "čtvrtek",
      "pátek",
      "sobota",

      /* CA-Cl*pper compatible natmsg items */

      "Databáze          # Záznamů    Aktualizace Velikost",
      "Chcete více příkladů?",
      "Strana",
      "** Subtotal **",
      "* Subsubtotal *",
      "*** Total ***",
      "Ins",
      "   ",
      "Chybné datum",
      "Rozsah: ",
      " - ",
      "A/N",
      "CHYBNÝ VÝRAZ",

      /* Error description names */

      "Neznámá chyba",
      "Chyba argumentu",
      "Chyba mezí",
      "Přetečení řetězce",
      "Přetečení čísla",
      "Dělení nulou",
      "Numerická chyba",
      "Chyba syntaxe",
      "Operace příliš komplexní",
      "",
      "",
      "Nedostatek paměti",
      "Nedefinovaná funkce",
      "Neznámá metoda",
      "Proměnná neexistuje",
      "Alias neexistuje",
      "Neznámá proměnná",
      "Nepovolené znaky v aliasu",
      "Alias již použit",
      "",
      "Chyba vytvoření",
      "Chyba otevření",
      "Chyba zavření",
      "Chyba čtení",
      "Chyba zápisu",
      "Chyba tisku",
      "",
      "",
      "",
      "",
      "Operace není podporována",
      "Překročen limit",
      "Index poškozen",
      "Typ dat se neshoduje",
      "Chyba šířky dat",
      "Pracovní oblast není použita",
      "Není otevřen index",
      "Požadováno uzamknutí",
      "Zámek při přidání záznamu selhal",
      "Zámek selhal",
      "Append lock failed",
      "Lock Failure",
      "",
      "",
      "",
      "",
      "přístup k poli",
      "přiřazení pole",
      "změna dimenze pole",
      "není pole",
      "podmínka",

       /* Internal error names */

      "Unrecoverable error %d: ",
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
      "hb_xgrab requested to allocate zero bytes",
      "hb_xrealloc requested to resize to zero bytes",
      "hb_xalloc requested to allocate zero bytes",

      /* Texts */

      "DD.MM.YYYY",
      "A",
      "N"
   }
};

#define HB_LANG_ID      CSUTF
#include "hbmsgreg.h"
