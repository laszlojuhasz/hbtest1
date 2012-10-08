/*
 * $Id: numtxtru.prg 17873 2012-10-08 17:39:24Z ptsarenko $
 */

/*
 * Harbour Project source code:
 * Functions to convert a number and date to East Slavic (Russian,
 * Ukrainian and Belorussian) text
 *
 * NumToTxtRU() - convert a number
 * MnyToTxtRU() - convert a money
 * DateToTxtRU() - convert a date
 *
 * Copyright 2012 Pavel Tsarenko (tpe2 at mail.ru)
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

#define NTSR_RUS 1
#define NTSR_UKR 2
#define NTSR_BEL 3

#define NTSR_MALE   1
#define NTSR_FEMA   2
#define NTSR_MIDD   3
#define NTSR_1000_1 4
#define NTSR_1000_2 5
#define NTSR_1000_3 6
#define NTSR_CNT    7
#define NTSR_ROD    8
#define NTSR_ORDG   9
#define NTSR_CURR   10
#define NTSR_CENT   11
#define NTSR_MINUS  12
#define NTSR_MONTH  13
#define NTSR_YEAR   14

/* Russian messages */
STATIC aRus := {;
 { "����",;
   "����",;
   "���",;
   "��",;
   "����",;
   "����",;
   "����",;
   "ᥬ�",;
   "��ᥬ�",;
   "������",;
   "������",;
   "����������",;
   "���������",;
   "�ਭ�����", ;
   "���ୠ����",;
   "��⭠����",;
   "��⭠����",;
   "ᥬ������",;
   "��ᥬ������",;
   "����⭠����",;
   "�������",;
   "�ਤ���",;
   "�ப",;
   "���줥���",;
   "���줥���",;
   "ᥬ줥���",;
   "��ᥬ줥���",;
   "���ﭮ��",;
   "��",;
   "�����",;
   "����",;
   "������",;
   "������",;
   "������",;
   "ᥬ���",;
   "��ᥬ���",;
   "��������" },;
 { "����", "����", "���" },;
 { "����", "����" },;
 { "�����", "�������", "�������", "�ਫ����", "����ਫ����" },;
 { "�����", "��������", "������ठ", "�ਫ�����", "����ਫ�����" },;
 { "�����", "���������", "������म�", "�ਫ������", "����ਫ������" },;
 { "�㫥���",;
   "����",;
   "��ன",;
   "��⨩",;
   "�⢥���",;
   "����",;
   "��⮩",;
   "ᥤ쬮�",;
   "���쬮�",;
   "������",;
   "������",;
   "����������",;
   "���������",;
   "�ਭ�����", ;
   "���ୠ����",;
   "��⭠����",;
   "��⭠����",;
   "ᥬ������",;
   "��ᥬ������",;
   "����⭠����",;
   "�������",;
   "�ਤ���",;
   "�ப����",;
   "��⨤�����",;
   "��⨤�����",;
   "ᥬ�������",;
   "���쬨������",;
   "���ﭮ���",;
   "���",;
   "�������",;
   "������",;
   "��������",;
   "������",;
   "������",;
   "ᥬ����",;
   "���쬨���",;
   "��������",;
   "������", "���������", "������भ�", "�ਫ������", "����ਫ������" },;
 { "",;
   "",;
   "����",;
   "���",;
   "�����",;
   "���",;
   "���",;
   "ᥬ�",;
   "���쬨",;
   "�����",;
   "�����",;
   "���������",;
   "��������",;
   "�ਭ����", ;
   "���ୠ���",;
   "��⭠���",;
   "��⭠���",;
   "ᥬ�����",;
   "��ᥬ�����",;
   "����⭠���",;
   "������",;
   "�ਤ��",;
   "�ப�",;
   "��⨤����",;
   "��⨤����",;
   "ᥬ������",;
   "���쬨�����",;
   "���ﭮ��" },;
 { "��", "��", "��", "�", "��" },;
 { NTSR_MALE, "��.", "�㡫�", "�㡫�", "�㡫��" },;
 { NTSR_FEMA, "���.", "�������", "�������", "������" },;
   "�����",;
 { "ﭢ���",  "䥢ࠫ�",  "����", "��५�",  "���",  "���",;
   "���",  "������",  "ᥭ����", "������",  "�����",  "�������" },;
 { "���", "����" } }

/* Ukrainian messages */
STATIC aUkr := {;
 { "���",;
   "����",;
   "���",;
   "��",;
   "���",;
   "�'���",;
   "�i���",;
   "�i�",;
   "�i�i�",;
   "���'���",;
   "������",;
   "����������",;
   "����������",;
   "�ਭ������",;
   "��ୠ�����",;
   "�'�⭠�����",;
   "�i�⭠�����",;
   "�i��������",;
   "�i�i��������",;
   "���'�⭠�����",;
   "��������",;
   "�ਤ����",;
   "�ப",;
   "�'�⤥���",;
   "�i�⤥���",;
   "�i������",;
   "�i�i������",;
   "���'ﭮ��",;
   "��",;
   "��i��i",;
   "����",;
   "�����",;
   "�'����",;
   "�i����",;
   "�i���",;
   "�i�i���",;
   "���'����" },;
 { "���", "����", "��i" },;
 { "���", "����" },;
 { "����", "�i�쮭", "�i����", "�ਫ쮭", "����ਫ쮭" },;
 { "����i", "�i�쮭�", "�i���ठ", "�ਫ쮭�", "����ਫ쮭�" },;
 { "����", "�i�쮭i�", "�i�����i�", "�ਫ쮭i�", "����ਫ쮭i�" },;
 { "��쮢��",;
   "���訩",;
   "��㣨�",;
   "���i�",;
   "�⢥�⨩",;
   "�'�⨩",;
   "��⨩",;
   "�쮬��",;
   "���쬨�",;
   "���'�⨩",;
   "����⨩",;
   "��������⨩",;
   "��������⨩",;
   "�ਭ����⨩", ;
   "��ୠ���⨩",;
   "�'�⭠���⨩",;
   "�i�⭠���⨩",;
   "�i������⨩",;
   "�i�i������⨩",;
   "���'�⭠���⨩",;
   "������⨩",;
   "�ਤ��⨩",;
   "�ப����",;
   "�'�⨤���⨩",;
   "��⨤���⨩",;
   "ᥬ�����⨩",;
   "�i�쬨����⨩",;
   "���'ﭮ�⨩",;
   "�⨩",;
   "�����⨩",;
   "�����⨩",;
   "�����⨩",;
   "�'���⨩",;
   "����⨩",;
   "ᥬ��⨩",;
   "�i�i��⨩",;
   "���'���⨩",;
   "���筨�", "�i�쮭���", "�i���भ��", "�ਫ쮭���", "����ਫ쮭���" },;
 { "",;
   "",;
   "����",;
   "����",;
   "�����",;
   "�'��",;
   "���",;
   "ᥬ�",;
   "�i�쬨",;
   "���'��",;
   "�����",;
   "���������",;
   "���������",;
   "�ਭ�����", ;
   "��ୠ����",;
   "�'�⭠����",;
   "�i�⭠����",;
   "�i�������",;
   "�i�i�������",;
   "���'�⭠����",;
   "�������",;
   "�ਤ���",;
   "�ப�",;
   "�'�⨤����",;
   "��⨤����",;
   "ᥬ������",;
   "�i�쬨�����",;
   "���'ﭮ��" },;
 { "i�", "�", "�", "�", "�" },;
 { NTSR_FEMA, "��.", "�ਢ��", "�ਢ�i", "�ਢ���" },;
 { NTSR_FEMA, "���.", "���i���", "���i���", "���i���" },;
   "�i���",;
 { "�i��",  "��⮣�",  "��१��", "��i��",  "�ࠢ��",  "�ࢭ�",;
   "�����",  "�௭�",  "�����", "�����",  "���⮯���", "��㤭�" },;
 { "�i�", "ப�" } }

/* Belorussian messages */
STATIC aBel := {;
 { "���",;
   "���i�",;
   "���",;
   "���",;
   "�����",;
   "����",;
   "�����",;
   "ᥬ",;
   "��ᥬ",;
   "�������",;
   "�������",;
   "���i������",;
   "���������",;
   "��뭠����",;
   "���ୠ����",;
   "��⭠����",;
   "�᭠����",;
   "�שּׂ�����",;
   "���שּׂ�����",;
   "�����⭠����",;
   "�������",;
   "�������",;
   "�ࠪ",;
   "���줧����",;
   "����줧����",;
   "ᥬ������",;
   "��ᥬ������",;
   "����ﭮ��",;
   "��",;
   "������",;
   "�����",;
   "�������",;
   "������",;
   "��������",;
   "����",;
   "������",;
   "���������" },;
 { "���", "����", "���" },;
 { "���", "����" },;
 { "�����", "�i���", "�i����", "�����", "��������" },;
 { "������", "�i���", "�i���ठ", "�����", "��������" },;
 { "�����", "�i����", "�i���ठ�", "������", "���������" },;
 { "����",;
   "�����",;
   "���i",;
   "����i",;
   "��좥���",;
   "����",;
   "����",;
   "���",;
   "�����",;
   "������",;
   "�������",;
   "���i������",;
   "���������",;
   "��뭠����", ;
   "���ୠ����",;
   "��⭠����",;
   "�᭠����",;
   "�שּׂ�����",;
   "���שּׂ�����",;
   "�����⭠����",;
   "�������",;
   "�������",;
   "�ࠪ���",;
   "���i�������",;
   "����i�������",;
   "��i�������",;
   "����i�������",;
   "����ﭮ���",;
   "���",;
   "�������",;
   "������",;
   "��������",;
   "���i���",;
   "����i���",;
   "��i���",;
   "����i���",;
   "������i���",;
   "������", "�i����", "�i���भ�", "������", "���������" },;
 { "",;
   "",;
   "����",;
   "���",;
   "�����",;
   "���i",;
   "���i",;
   "��i",;
   "����i",;
   "������i",;
   "������i",;
   "���i�����i",;
   "��������i",;
   "��뭠���i",;
   "���ୠ���i",;
   "��⭠���i",;
   "�᭠���i",;
   "�שּׂ����i",;
   "���שּׂ����i",;
   "�����⭠���i",;
   "������i",;
   "������i",;
   "�ࠪ�",;
   "���i椧����i",;
   "���i������i",;
   "��i������i",;
   "����i������i",;
   "����ﭮ��" },;
 { "i", "��", "��", "�", "��" },;
 { NTSR_MALE, "��.", "�㡥��", "�㡫i", "�㡫��" },;
 { NTSR_FEMA, "���.", "�������", "������i", "������" },;
   "�i���",;
 { "��㤧���", "����", "ᠪ��i��", "��ᠢi��", "���", "��ࢥ��",;
   "�i����", "��i����", "�����", "�������i��", "�i�⠯���", "᭥����" },;
 { "���", "����" } }

/* test procedure
procedure main
   REQUEST HB_CODEPAGE_RU866 
   HB_CDPSelect( "RU866" )

   ? "Press ESC to break"
   ? "Russian"
   Test( NTSR_RUS )
   ? "Ukrainian"
   Test( NTSR_UKR )
   ? "Belorussian"
   Test( NTSR_BEL )
   Return

procedure test( nLang )
   Local nTemp

   dbCreate('_num'+LTrim(Str( nLang)),;
 {{'NUM', 'N', 19, 0}, {'STR1', 'C', 100, 0}, {'STR2', 'C', 100, 0}, {'STR3', 'C', 50, 0}},, .t., 'num')
   for nTemp := 1 to 1000000000
      num->(dbAppend())
      num->Num := nTemp
      num->Str1 := MnyToTxtRU( nTemp + (nTemp%100)*0.01, nLang,, 3 )
      num->Str2 := NumToTxtRU( nTemp, nLang,, .t. )
      num->Str3 := DateToTxtRU( Date()+nTemp, nLang, .t. )
      if nTemp % 1000 == 0
         ? nTemp
      endif
      if nTemp % 10000 == 0
         if inkey() == 27
             exit
         endif
      endif
   next
   close
   return
*/

FUNCTION NumToTxtRU( nValue, nLang, nGender, lOrd )
/*
 * nValue:  integer value;
 * nLang:   language Id (1-3), russian (1) by default;
 * nGender: masculine (default), feminine or neuter gender;
 * lOrd:    ordinals, cardinal numbers if omitted
 */
   LOCAL aMsg := GetLangMsg( nLang )
   LOCAL cRetVal

   if nValue < 0
      nValue := -nValue
      cRetVal := aMsg[ NTSR_MINUS ] + " "
   ELSE
      cRetVal := ""
   endif 

   nValue := Int( nValue )
   cRetVal += NumToStrRaw( nValue, aMsg, nGender, lOrd )

   RETURN cRetVal

FUNCTION MnyToTxtRU( nValue, nLang, nMode1, nMode2 )
/*
 * nValue:  integer value;
 * nLang:   language Id (1-3), russian (1) by default;
 * nMode1:  1 - in words,
 *          2 - in words and short name,
 *          3 - in numbers,
 *          4 - in numbers and short name;
 * nMode2:  mode for cents, in format as above 
 */
   LOCAL cRetVal
   LOCAL aMsg := GetLangMsg( nLang )
   LOCAL nCent

   nValue := Round( nValue, 2 )
   nCent  := Round( (nValue - Int( nValue )) * 100, 0)
   nValue := Int( nValue )

   cRetVal := MnyToStrRaw( nValue, nLang, aMsg[ NTSR_CURR ], nMode1 ) + " " + ;
              MnyToStrRaw( nCent, nLang, aMsg[ NTSR_CENT ], nMode2 )

   Return cRetVal

FUNCTION DateToTxtRU( dDate, nLang, lWord )
   LOCAL aMsg := GetLangMsg( nLang )
   LOCAL cRetVal, nTemp

   if ! Empty( dDate )
      nTemp := Day( dDate )
      if lWord != nil
         cRetVal := NumToStrRaw( nTemp, aMsg, NTSR_MIDD, .t. )
      else
         cRetVal := LTrim( Str( nTemp ) )
      endif

      cRetVal += " " + aMsg[ NTSR_MONTH, Month( dDate ) ] + " " + ;
                 Str( Year( dDate ), 4 ) + " " + aMsg[ NTSR_YEAR, 2 ]
   else
      cRetVal := ""
   endif

   Return cRetVal

STATIC FUNCTION MnyToStrRaw( nValue, nLang, aCur, nMode )
  LOCAL aMsg := GetLangMsg( nLang )
  LOCAL cRetVal
  LOCAL cTemp, nTemp
  LOCAL lShort := nMode == 2 .or. nMode == 4

  if nMode == nil
     nMode := 1
  endif
  if nMode <= 2
     if nValue == 0
        cRetVal := aMsg[ NTSR_MALE, 1 ]
     else
        cRetVal := NumToStrRaw( nValue, aMsg, aCur[ 1 ] )
     endif
  else
     cRetVal := LTrim( if( nValue < 100, StrZero( nValue, 2 ), Str( nValue ) ) )
  endif

  if ! lShort
     nTemp := Int( nValue % 100 )
     if nTemp >= 5 .and. nTemp <= 20
        cTemp := aCur[ 5 ]
     elseif nTemp % 10 == 1
        cTemp := aCur[ 3 ]
     elseif nTemp % 10 >= 2 .and. nTemp % 10 <= 4
        cTemp := aCur[ 4 ]
     else
        cTemp := aCur[ 5 ]
     endif
  else
     cTemp := aCur[ 2 ]
  endif

  Return cRetVal + " " + cTemp

STATIC FUNCTION GetLangMsg( nLang )
   LOCAL aMsg
   if nLang == nil .or. nLang == NTSR_RUS
      aMsg := aRus
   elseif nLang == NTSR_UKR
      aMsg := aUkr
   elseif nLang == NTSR_BEL
      aMsg := aBel
   endif
   RETURN aMsg

STATIC FUNCTION NumToStrRaw( nValue, aMsg, nGender, lOrd )
   LOCAL nTri := 0, nTemp, nTemp1
   LOCAL cRetVal := "", cTemp
   LOCAL lLast := .t.

   if nGender == Nil
      nGender := NTSR_MALE
   endif 
   if lOrd == Nil
      lOrd := .f.
   endif 

   while nValue != 0
      nTemp := nValue % 1000
      if nTemp != 0
         cTemp := ""
         if nTri > 0
            if lOrd .and. lLast
               if nTemp > 20 .and. nTemp % 10 != 0
                  cTemp += " "
               endif
               if nTri + 37 <= Len( aMsg[ NTSR_CNT ] )
                  cTemp += OrdToGender( aMsg[ NTSR_CNT, nTri + 37 ], aMsg, nGender )
               else
                  cTemp += "10**" + LTrim( Str( nTri*3 ) )
               endif
            elseif nTri <= Len( aMsg[ NTSR_1000_1 ] )
               cTemp += " "
               nTemp1 := ( nValue % 10 )
               if nTemp1 == 1 .and. nValue != 11
                  cTemp += aMsg[ NTSR_1000_1, nTri ]
               elseif nTemp1 >= 2 .and. nTemp1 <= 4 .and. ( nValue < 10 .or. nValue > 20 )
                  cTemp += aMsg[ NTSR_1000_2, nTri ]
               else
                  cTemp += aMsg[ NTSR_1000_3, nTri ]
               endif 
            else
               cTemp += "10**" + LTrim( Str( nTri*3 ) ) + " "
            endif 
         endif 
         cTemp := TriToStr( nTemp, aMsg, iif( nTri==0, nGender, iif(nTri == 1, 2, 1 ) ), lOrd, @lLast, nTri ) + cTemp
         if ! Empty( cRetVal )
            cRetVal := " " + cRetVal
         endif 
         cRetVal := cTemp + cRetVal
      endif 
      nValue := Int( nValue / 1000)
      nTri ++
   enddo

   RETURN cRetVal

STATIC FUNCTION TriToStr( nValue, aMsg, nGender, lOrd, lLast, nTri )
   LOCAL cRetVal, cTemp, nTemp, nIdx
   LOCAL l20 := .f.

   if nValue >= 100
      nTemp := nValue % 100
      if lOrd .and. lLast .and. nTemp == 0
         nIdx := NTSR_CNT
         lLast := .f.
      else
         nIdx := NTSR_MALE
      endif
      cRetVal := aMsg[ nIdx, Int( nValue / 100 ) + 28 ]
      if nIdx == NTSR_CNT
         cRetVal := OrdToGender( cRetVal, aMsg, nGender )
      endif
      nValue := nTemp
      if nValue != 0
         cRetVal += " "
      endif 
      l20 := .t.
   else
      cRetVal := ""
   endif 

   if nValue >= 20
      nTemp := nValue % 10
      if ! lOrd .or. nTemp # 0 .or. ! lLast
         nIdx := NTSR_MALE
      elseif lLast .and. nTemp == 0 .and. nTri == 0
         nIdx := NTSR_CNT
         lLast := .f.
      else
         nIdx := NTSR_ROD
         lLast := .f.
      endif
      cTemp := aMsg[ nIdx, Int( nValue / 10 ) - 1 + 20 ]
      if nIdx == NTSR_CNT
         cTemp := OrdToGender( cTemp, aMsg, nGender )
      endif
      cRetVal += cTemp
      nValue := nTemp
      if nValue != 0
         cRetVal += " "
      endif 
      l20 := .t.
   endif 

   if nValue > 0
      if lOrd
         if nTri >= 1 .and. lLast .and. ! l20
            nIdx := NTSR_ROD
            lLast := .f.
         else
            if lLast .and. nTri == 0
               nIdx := NTSR_CNT
               lLast := .f.
            else
               nIdx := if( nValue + 1 <= len( aMsg[ nGender ] ), nGender, NTSR_MALE )
            endif
         endif
      else
         nIdx := if( nValue + 1 <= len( aMsg[ nGender ] ), nGender, NTSR_MALE )
      endif
      cTemp := aMsg[ nIdx, nValue + 1 ]
      if nIdx == NTSR_CNT
         cTemp := OrdToGender( cTemp, aMsg, nGender )
      endif
      cRetVal += cTemp
   endif 

   RETURN cRetVal

STATIC FUNCTION OrdToGender( cValue, aMsg, nGender)
   LOCAL nTemp := Len( cValue ) - Len( aMsg[ NTSR_ORDG, 1 ] )
   if nGender == NTSR_FEMA
      cValue := Left( cValue, nTemp ) + if( Substr( cValue, nTemp + 1 ) = aMsg[ NTSR_ORDG, 1 ],;
                aMsg[ NTSR_ORDG, 2 ], aMsg[ NTSR_ORDG, 3 ] )
   elseif nGender == NTSR_MIDD
      cValue := Left( cValue, nTemp ) + if( Substr( cValue, nTemp + 1 ) = aMsg[ NTSR_ORDG, 1 ],;
                aMsg[ NTSR_ORDG, 4 ], aMsg[ NTSR_ORDG, 5 ] )
   endif
   RETURN cValue
