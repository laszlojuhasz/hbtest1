/*
 * $Id$
 */

/*
 * Harbour Project source code:
 * Not (yet) implemented CT functions
 *
 * Copyright 2010 Viktor Szakats (harbour.01 syenar.hu)
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

#include "hbapi.h"

/* Introduction Extended Drivers */
HB_FUNC( CGA40 )      {;}
HB_FUNC( CGA80 )      {;}
HB_FUNC( DSETNOLINE ) {;}
HB_FUNC( DSETQFILE )  {;}
HB_FUNC( DSETTYPE )   {;}
HB_FUNC( DSETWINDEB ) {;}
HB_FUNC( DSETWINDOW ) {;}
HB_FUNC( EGA43 )      {;}
HB_FUNC( FIRSTCOL )   {;}
HB_FUNC( FIRSTROW )   {;}
HB_FUNC( GETBOXGROW ) {;}
HB_FUNC( GETCURSOR )  {;}
HB_FUNC( GETKXTAB )   {;}
HB_FUNC( GETLINES )   {;}
HB_FUNC( GETMODE )    {;}
HB_FUNC( GETPAGE )    {;}
HB_FUNC( GETPBIOS )   {;}
HB_FUNC( GETPXLAT )   {;}
HB_FUNC( GETSCRMODE ) {;}
HB_FUNC( GETTAB )     {;}
HB_FUNC( INKEYTRAP )  {;}
HB_FUNC( INPUTMODE )  {;}
HB_FUNC( KEYREAD )    {;}
HB_FUNC( KEYSEND )    {;}
HB_FUNC( MAXCOL )     {;}
HB_FUNC( MAXPAGE )    {;}
HB_FUNC( MAXROW )     {;}
HB_FUNC( MONOCHROME ) {;}
HB_FUNC( PAGECOPY )   {;}
HB_FUNC( PRINTERROR ) {;}
HB_FUNC( SETBELL )    {;}
HB_FUNC( SETBOXGROW ) {;}
HB_FUNC( SETCURSOR )  {;}
HB_FUNC( SETKXTAB )   {;}
HB_FUNC( SETLINES )   {;}
HB_FUNC( SETMAXCOL )  {;}
HB_FUNC( SETMAXROW )  {;}
HB_FUNC( SETPAGE )    {;}
HB_FUNC( SETPBIOS )   {;}
HB_FUNC( SETPXLAT )   {;}
HB_FUNC( SETQNAME )   {;}
HB_FUNC( SETSCRMODE ) {;}
HB_FUNC( SETTAB )     {;}
HB_FUNC( TRAPANYKEY ) {;}
HB_FUNC( TRAPINPUT )  {;}
HB_FUNC( TRAPSHIFT )  {;}
HB_FUNC( VGA28 )      {;}
HB_FUNC( VGA50 )      {;}
/* Introduction Serial Communications */
HB_FUNC( COM_BREAK )  {;}
HB_FUNC( COM_CLOSE )  {;}
HB_FUNC( COM_COUNT )  {;}
HB_FUNC( COM_CRC )    {;}
HB_FUNC( COM_CTS )    {;}
HB_FUNC( COM_DCD )    {;}
HB_FUNC( COM_DOSCON ) {;}
HB_FUNC( COM_DSR )    {;}
HB_FUNC( COM_DTR )    {;}
HB_FUNC( COM_ERRCHR ) {;}
HB_FUNC( COM_EVENT )  {;}
HB_FUNC( COM_FLUSH )  {;}
HB_FUNC( COM_GETIO )  {;}
HB_FUNC( COM_GETIRQ ) {;}
HB_FUNC( COM_HARD )   {;}
HB_FUNC( COM_INIT )   {;}
HB_FUNC( COM_KEY )    {;}
HB_FUNC( COM_LSR )    {;}
HB_FUNC( COM_MCR )    {;}
HB_FUNC( COM_MSR )    {;}
HB_FUNC( COM_NUM )    {;}
HB_FUNC( COM_OPEN )   {;}
HB_FUNC( COM_READ )   {;}
HB_FUNC( COM_REMOTE ) {;}
HB_FUNC( COM_RING )   {;}
HB_FUNC( COM_RTS )    {;}
HB_FUNC( COM_SCOUNT ) {;}
HB_FUNC( COM_SEND )   {;}
HB_FUNC( COM_SETIO )  {;}
HB_FUNC( COM_SETIRQ ) {;}
HB_FUNC( COM_SFLUSH ) {;}
HB_FUNC( COM_SKEY )   {;}
HB_FUNC( COM_SMODE )  {;}
HB_FUNC( COM_SOFT )   {;}
HB_FUNC( COM_SOFT_R ) {;}
HB_FUNC( COM_SOFT_S ) {;}
HB_FUNC( XMOBLOCK )   {;}
HB_FUNC( XMOCHECK )   {;}
HB_FUNC( ZEROINSERT ) {;}
HB_FUNC( ZEROREMOVE ) {;}
/* Introduction Video Functions */
HB_FUNC( EGAPALETTE ) {;}
HB_FUNC( FONTLOAD )   {;}
HB_FUNC( FONTRESET )  {;}
HB_FUNC( FONTROTATE ) {;}
HB_FUNC( FONTSELECT ) {;}
HB_FUNC( GETFONT )    {;}
HB_FUNC( GETSCRSTR )  {;}
HB_FUNC( GETVGAPAL )  {;}
HB_FUNC( ISCGA )      {;}
HB_FUNC( ISEGA )      {;}
HB_FUNC( ISHERCULES ) {;}
HB_FUNC( ISMCGA )     {;}
HB_FUNC( ISMONO )     {;}
HB_FUNC( ISPGA )      {;}
HB_FUNC( ISVGA )      {;}
HB_FUNC( MAXFONT )    {;}
HB_FUNC( MONISWITCH ) {;}
HB_FUNC( NUMCOL )     {;}
HB_FUNC( SCREENSIZE ) {;}
HB_FUNC( SETSCRSTR )  {;}
HB_FUNC( VIDEOINIT )  {;}
HB_FUNC( VIDEOSETUP ) {;}
/* Introduction Disk Utilities */
/* HB_FUNC( DIRCHANGE )  {;} */ /* Implemented in Harbour core as C5.3 function. */
/* HB_FUNC( DIRREMOVE )  {;} */ /* Implemented in Harbour core as C5.3 function. */
/* HB_FUNC( DISKCHANGE ) {;} */ /* Implemented in Harbour core as C5.3 function. */
HB_FUNC( DISKCHECK )  {;}
HB_FUNC( DISKFORMAT ) {;}
HB_FUNC( DISKFREE )   {;}
HB_FUNC( DISKNAME )   {;}
HB_FUNC( DISKREADY )  {;}
HB_FUNC( DISKREADYW ) {;}
HB_FUNC( DISKSPEED )  {;}
HB_FUNC( DISKSTAT )   {;}
HB_FUNC( DISKTOTAL )  {;}
HB_FUNC( DISKTYPE )   {;}
HB_FUNC( FILECHECK )  {;}
HB_FUNC( FILEVALID )  {;}
HB_FUNC( FLOPPYTYPE ) {;}
HB_FUNC( GETSHARE )   {;}
HB_FUNC( NUMDISKF )   {;}
HB_FUNC( NUMDISKH )   {;}
HB_FUNC( RESTFSEEK )  {;}
HB_FUNC( SAVEFSEEK )  {;}
HB_FUNC( SETSHARE )   {;}
/* Introduction Printer Functions */
HB_FUNC( NUMPRINTER ) {;}
HB_FUNC( PRINTFILE )  {;}
HB_FUNC( PRINTINIT )  {;}
HB_FUNC( PRINTSCR )   {;}
HB_FUNC( PRINTSCRX )  {;}
HB_FUNC( SPOOLACTIV ) {;}
HB_FUNC( SPOOLADD )   {;}
HB_FUNC( SPOOLCOUNT ) {;}
HB_FUNC( SPOOLDEL )   {;}
HB_FUNC( SPOOLENTRY ) {;}
HB_FUNC( SPOOLFLUSH ) {;}
HB_FUNC( TOF )        {;}
/* Introduction Database Functions */
HB_FUNC( DBFDSKSIZE ) {;}
HB_FUNC( ISDBT )      {;}
/* Introduction Set Status */
HB_FUNC( CSETALL )    {;}
HB_FUNC( CSETCLIP )   {;}
HB_FUNC( CSETDATE )   {;}
HB_FUNC( CSETDECI )   {;}
HB_FUNC( CSETDEFA )   {;}
HB_FUNC( CSETFUNC )   {;}
HB_FUNC( CSETLDEL )   {;}
HB_FUNC( CSETMARG )   {;}
HB_FUNC( CSETPATH )   {;}
HB_FUNC( CSETRDEL )   {;}
HB_FUNC( CSETRDONLY ) {;}
HB_FUNC( CSETSNOW )   {;}
HB_FUNC( CSETXXXX )   {;}
HB_FUNC( ISDEBUG )    {;}
HB_FUNC( LASTKFUNC )  {;}
HB_FUNC( LASTKLINE )  {;}
HB_FUNC( LASTKPROC )  {;}
HB_FUNC( NUMFKEY )    {;}
/* Introduction System Information */
HB_FUNC( BIOSDATE )   {;}
HB_FUNC( BOOTCOLD )   {;}
HB_FUNC( BOOTWARM )   {;}
HB_FUNC( CPUTYPE )    {;}
HB_FUNC( ENVPARAM )   {;}
HB_FUNC( ERRORACT )   {;}
HB_FUNC( ERRORBASE )  {;}
HB_FUNC( ERRORCODE )  {;}
HB_FUNC( ERRORORG )   {;}
HB_FUNC( FILESFREE )  {;}
HB_FUNC( GETCOUNTRY ) {;}
HB_FUNC( ISANSI )     {;}
HB_FUNC( ISAT )       {;}
HB_FUNC( ISMATH )     {;}
HB_FUNC( MEMSIZE )    {;}
HB_FUNC( NUMBUFFERS ) {;}
HB_FUNC( NUMFILES )   {;}
HB_FUNC( OSVER )      {;}
HB_FUNC( PCTYPE )     {;}
HB_FUNC( SSETBREAK )  {;}
HB_FUNC( SSETVERIFY ) {;}
/* Introduction Miscellaneous Functions */
HB_FUNC( DATATYPE )   {;}
HB_FUNC( GETTIC )     {;}
HB_FUNC( KBDDISABLE ) {;}
HB_FUNC( KBDEMULATE ) {;}
HB_FUNC( KBDSPEED )   {;}
HB_FUNC( KBDTYPE )    {;}
HB_FUNC( SCANKEY )    {;}
HB_FUNC( SETTIC )     {;}
HB_FUNC( SHOWKEY )    {;}
HB_FUNC( SOUND )      {;}
HB_FUNC( SPEED )      {;}
HB_FUNC( STACKFREE )  {;}
HB_FUNC( TOOLVER )    {;}
/* Introduction PEEK/POKE Functions */
HB_FUNC( INBYTE )     {;}
HB_FUNC( INWORD )     {;}
HB_FUNC( OUTBYTE )    {;}
HB_FUNC( OUTWORD )    {;}
HB_FUNC( PEEKBYTE )   {;}
HB_FUNC( PEEKSTR )    {;}
HB_FUNC( PEEKWORD )   {;}
HB_FUNC( POKEBYTE )   {;}
HB_FUNC( POKEWORD )   {;}
