/*
 * $Id$
 */

/*
 * Harbour Project source code:
 * Video subsystem for plain ANSI C stream IO
 *
 * Copyright 1999-2001 Viktor Szakats <viktor.szakats@syenar.hu>
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

/* NOTE: User programs should never call this layer directly! */

/* TODO: include any standard headers here */

#include "hbapifs.h"
#include "hbapigt.h"

#if defined( OS_UNIX_COMPATIBLE )
   #include <unistd.h>  /* read() function requires it */
   #include <termios.h>
#else
   #if defined(_MSC_VER)
      #include <io.h>
      #include <conio.h>
   #endif
#endif

/* Add time function for BEL flood throttling.. */

#include <time.h>
#if defined( HB_OS_BSD )
   #include <sys/time.h>
#elif defined( OS_UNIX_COMPATIBLE )
   #include <sys/timeb.h>
#else
   #include <sys\timeb.h>
#endif


static SHORT  s_iRow;
static SHORT  s_iCol;
static USHORT s_uiMaxRow;
static USHORT s_uiMaxCol;
static USHORT s_uiCursorStyle;
static BOOL   s_bBlink;
static int    s_iFilenoStdout;
static USHORT s_uiDispCount;
static BYTE * s_szCrLf;
static ULONG  s_ulCrLf;

#if defined( OS_UNIX_COMPATIBLE )
   static struct termios startup_attributes;
#endif

#if defined(_MSC_VER)
   static BOOL s_bStdinConsole;
#endif


void hb_gt_Init( int iFilenoStdin, int iFilenoStdout, int iFilenoStderr )
{
   HB_TRACE(HB_TR_DEBUG, ("hb_gt_Init()"));

   HB_SYMBOL_UNUSED( iFilenoStdin );
   HB_SYMBOL_UNUSED( iFilenoStderr );

#if defined( OS_UNIX_COMPATIBLE )
   {
      struct termios ta;
      
      tcgetattr( STDIN_FILENO, &startup_attributes );
/*      atexit( restore_input_mode ); */
      
      tcgetattr( STDIN_FILENO, &ta );
      ta.c_lflag &= ~( ICANON | ECHO );
      ta.c_iflag &= ~ICRNL;
      ta.c_cc[ VMIN ] = 0;
      ta.c_cc[ VTIME ] = 0;
      tcsetattr( STDIN_FILENO, TCSAFLUSH, &ta );
   }
#endif

#if defined(_MSC_VER)
   s_bStdinConsole = _isatty(0);
#endif

   s_uiDispCount = 0;

   s_iRow = 0;
   s_iCol = 0;

/* #if defined(OS_UNIX_COMPATIBLE) */
   s_uiMaxRow = 24;
   s_uiMaxCol = 80;
/*
#else
   s_uiMaxRow = 32767;
   s_uiMaxCol = 32767;
#endif */

   s_uiCursorStyle = SC_NORMAL;
   s_bBlink = FALSE;
   s_iFilenoStdout = iFilenoStdout;
   hb_fsSetDevMode( s_iFilenoStdout, FD_BINARY );

   s_szCrLf = (BYTE *) hb_conNewLine();
   s_ulCrLf = strlen( (char *) s_szCrLf );
   
   hb_mouse_Init();
}

void hb_gt_Exit( void )
{
   HB_TRACE(HB_TR_DEBUG, ("hb_gt_Exit()"));

   hb_mouse_Exit();

#if defined( OS_UNIX_COMPATIBLE )
   tcsetattr( STDIN_FILENO, TCSANOW, &startup_attributes );
#endif
}

int hb_gt_ExtendedKeySupport()
{
   return 0;
}

static void out_stdout( BYTE * pStr, ULONG ulLen )
{
   unsigned uiErrorOld = hb_fsError(); /* Save current user file error code */
   hb_fsWriteLarge( s_iFilenoStdout, pStr, ulLen );
   hb_fsSetError( uiErrorOld );        /* Restore last user file error code */
}

static void out_newline( void )
{
   out_stdout( s_szCrLf, s_ulCrLf );
}

int hb_gt_ReadKey( HB_inkey_enum eventmask )
{
   int ch = 0;
   
   HB_TRACE(HB_TR_DEBUG, ("hb_gt_ReadKey(%d)", (int) eventmask));

   HB_SYMBOL_UNUSED( eventmask );

#if defined(OS_UNIX_COMPATIBLE)
   if( ! read( STDIN_FILENO, &ch, 1 ) )
      ch = 0;
#else

   #if defined(_MSC_VER)
   if( s_bStdinConsole )
   {
      if( _kbhit() ) ch = _getch();
   }
   else
   {
      if(! _eof(0) ) _read(0, &ch, 1);
   }
   #endif

#endif

   /* TODO: */

   return ch;
}

/* Parse out a string to determine the new cursor position */

BOOL hb_gt_AdjustPos( BYTE * pStr, ULONG ulLen )
{
   USHORT row = s_iRow;
   USHORT col = s_iCol;
   ULONG ulCount;

   HB_TRACE(HB_TR_DEBUG, ("hb_gt_AdjustPos(%s, %lu)", pStr, ulLen ));

   for( ulCount = 0; ulCount < ulLen; ulCount++ )
   {
      switch( *pStr++  )
      {
         case HB_CHAR_BEL:
            break;

         case HB_CHAR_BS:
            if( col )
               col--;
            else
            {
               col = s_uiMaxCol;
               if( row )
                  row--;
            }
            break;

         case HB_CHAR_LF:
            if( row < s_uiMaxRow )
               row++;
            break;

         case HB_CHAR_CR:
            col = 0;
            break;

         default:
            if( col < s_uiMaxCol )
               col++;
            else
            {
               col = 0;
               if( row < s_uiMaxRow )
                  row++;
            }
      }
   }
   
   s_iRow = row;
   s_iCol = col;

   return TRUE;
}

BOOL hb_gt_IsColor( void )
{
   HB_TRACE(HB_TR_DEBUG, ("hb_gt_IsColor()"));

   return FALSE;
}

USHORT hb_gt_GetScreenWidth( void )
{
   HB_TRACE(HB_TR_DEBUG, ("hb_gt_GetScreenWidth()"));

   return s_uiMaxCol;
}

USHORT hb_gt_GetScreenHeight( void )
{
   HB_TRACE(HB_TR_DEBUG, ("hb_gt_GetScreenHeight()"));

   return s_uiMaxRow;
}

void hb_gt_SetPos( SHORT iRow, SHORT iCol, SHORT iMethod )
{
   BYTE szBuffer[2] = { 0, 0 };
   
   HB_TRACE(HB_TR_DEBUG, ("hb_gt_SetPos(%hd, %hd, %hd)", iRow, iCol, iMethod));

   if( iMethod == HB_GT_SET_POS_BEFORE )
   {
      /* Only set the screen position when the cursor
         position is changed BEFORE text is displayed.

         Updates to cursor position AFTER test output is handled
         within this driver itself
      */
      
      if( iRow != s_iRow )
      {
         /* always go to a newline, even if request to go to row above.
            Although can't actually do unward movement, at least start
            a new line to avoid possibly overwriting text already on
            the current row */
            
         out_newline();
         s_iCol = 0;
         if (s_iRow < iRow)
         {
            /* if requested to move down more than one row, do extra
              newlines to render the correct vertical distance */
              
            while( ++s_iRow < iRow )
               out_newline();
         }
      }

      /* Use space and backspace to adjust horizontal position.. */
   
      if( s_iCol < iCol )
      {
         szBuffer[0] = ' ';
         while( s_iCol++ < iCol )
            out_stdout( szBuffer, 1 );
      }
      else if( s_iCol > iCol )
      {
         szBuffer[0] = HB_CHAR_BS;
         while( s_iCol-- > iCol )
            out_stdout( szBuffer, 1 );
      }

      s_iRow = iRow;
      s_iCol = iCol;
   }
}

SHORT hb_gt_Col( void )
{
   HB_TRACE(HB_TR_DEBUG, ("hb_gt_Col()"));

   return s_iCol;
}

SHORT hb_gt_Row( void )
{
   HB_TRACE(HB_TR_DEBUG, ("hb_gt_Row()"));

   return s_iRow;
}

USHORT hb_gt_GetCursorStyle( void )
{
   HB_TRACE(HB_TR_DEBUG, ("hb_gt_GetCursorStyle()"));

   return s_uiCursorStyle;
}

void hb_gt_SetCursorStyle( USHORT uiCursorStyle )
{
   HB_TRACE(HB_TR_DEBUG, ("hb_gt_SetCursorStyle(%hu)", uiCursorStyle));

   s_uiCursorStyle = uiCursorStyle;
}

static void hb_gt_xPutch( USHORT uiRow, USHORT uiCol, BYTE byAttr, BYTE byChar )
{
   BYTE szBuffer[ 2 ];

   HB_TRACE(HB_TR_DEBUG, ("hb_gt_xPutch(%hu, %hu, %d, %i)", uiRow, uiCol, (int) byAttr, byAttr));

   HB_SYMBOL_UNUSED( byAttr );

   hb_gt_SetPos( uiRow, uiCol, HB_GT_SET_POS_BEFORE );

   /* make the char into a string so it can be passed to AdjustPos
      as well as being output */
      
   szBuffer[ 0 ] = byChar;
   szBuffer[ 1 ] = 0;
   out_stdout( szBuffer, 1 );

   hb_gt_AdjustPos( szBuffer, 1 );
}

void hb_gt_Puts( USHORT uiRow, USHORT uiCol, BYTE byAttr, BYTE * pbyStr, ULONG ulLen )
{
   HB_TRACE(HB_TR_DEBUG, ("hb_gt_Puts(%hu, %hu, %d, %p, %lu)", uiRow, uiCol, (int) byAttr, pbyStr, ulLen));

   HB_SYMBOL_UNUSED( byAttr );

   hb_gt_SetPos( uiRow, uiCol, HB_GT_SET_POS_BEFORE );

   out_stdout( pbyStr, ulLen );

   hb_gt_AdjustPos( pbyStr, ulLen );
}

int hb_gt_RectSize( USHORT rows, USHORT cols )
{
   return rows * cols * 2;
}

void hb_gt_GetText( USHORT uiTop, USHORT uiLeft, USHORT uiBottom, USHORT uiRight, BYTE * pbyDst )
{
   HB_TRACE(HB_TR_DEBUG, ("hb_gt_GetText(%hu, %hu, %hu, %hu, %p)", uiTop, uiLeft, uiBottom, uiRight, pbyDst));

   HB_SYMBOL_UNUSED( uiTop );
   HB_SYMBOL_UNUSED( uiLeft );
   HB_SYMBOL_UNUSED( uiBottom );
   HB_SYMBOL_UNUSED( uiRight );
   HB_SYMBOL_UNUSED( pbyDst );
}

void hb_gt_PutText( USHORT uiTop, USHORT uiLeft, USHORT uiBottom, USHORT uiRight, BYTE * pbySrc )
{
   HB_TRACE(HB_TR_DEBUG, ("hb_gt_PutText(%hu, %hu, %hu, %hu, %p)", uiTop, uiLeft, uiBottom, uiRight, pbySrc));

   HB_SYMBOL_UNUSED( uiTop );
   HB_SYMBOL_UNUSED( uiLeft );
   HB_SYMBOL_UNUSED( uiBottom );
   HB_SYMBOL_UNUSED( uiRight );
   HB_SYMBOL_UNUSED( pbySrc );
}

void hb_gt_SetAttribute( USHORT uiTop, USHORT uiLeft, USHORT uiBottom, USHORT uiRight, BYTE byAttr )
{
   HB_TRACE(HB_TR_DEBUG, ("hb_gt_PutText(%hu, %hu, %hu, %hu, %d)", uiTop, uiLeft, uiBottom, uiRight, (int) byAttr));

   HB_SYMBOL_UNUSED( uiTop );
   HB_SYMBOL_UNUSED( uiLeft );
   HB_SYMBOL_UNUSED( uiBottom );
   HB_SYMBOL_UNUSED( uiRight );
   HB_SYMBOL_UNUSED( byAttr );
}

void hb_gt_Scroll( USHORT uiTop, USHORT uiLeft, USHORT uiBottom, USHORT uiRight, BYTE byAttr, SHORT iRows, SHORT iCols )
{
   HB_TRACE(HB_TR_DEBUG, ("hb_gt_Scroll(%hu, %hu, %hu, %hu, %d, %hu, %hu)", uiTop, uiLeft, uiBottom, uiRight, (int) byAttr, iRows, iCols));

   HB_SYMBOL_UNUSED( byAttr );

   /* Provide some basic scroll support for full screen */
   
   if( uiTop == 0 &&
      uiBottom >= (s_uiMaxRow - 1 ) &&
      uiLeft == 0 &&
      uiRight >= (s_uiMaxCol - 1 ) )
   {
   
      if ( iRows == 0 && iCols == 0 )
      {
         /* clear screen request.. */

         for( ; uiBottom; uiBottom-- )
            out_newline();

         s_iRow = 0;
         s_iCol = 0;
      }
      else
      {
         /* no true scroll capability */
         /* but newline for each upward scroll */
         /* as gtapi.c will call scroll when on last row */
         /* and not change the row value itself */

         while( iRows-- > 0 )
            out_newline();
         
         s_iCol = 0;
      }
   }
}
 
void hb_gt_DispBegin( void )
{
   HB_TRACE(HB_TR_DEBUG, ("hb_gt_DispBegin()"));

   ++s_uiDispCount;
   /* Do nothing else */
}

void hb_gt_DispEnd()
{
   HB_TRACE(HB_TR_DEBUG, ("hb_gt_DispEnd()"));

   --s_uiDispCount;
   /* Do nothing else */
}

BOOL hb_gt_SetMode( USHORT uiMaxRow, USHORT uiMaxCol )
{
   HB_TRACE(HB_TR_DEBUG, ("hb_gt_SetMode(%hu, %hu)", uiMaxRow, uiMaxCol));

   s_uiMaxRow = uiMaxRow;
   s_uiMaxCol = uiMaxCol;

   return FALSE;
}

BOOL hb_gt_GetBlink()
{
   HB_TRACE(HB_TR_DEBUG, ("hb_gt_GetBlink()"));

   return s_bBlink;
}

void hb_gt_SetBlink( BOOL bBlink )
{
   HB_TRACE(HB_TR_DEBUG, ("hb_gt_SetBlink(%d)", (int) bBlink));

   s_bBlink = bBlink;
}

static int gtstd_get_seconds( void )
{
#if defined(_MSC_VER)
   #define timeb _timeb
   #define ftime _ftime
#endif
#if defined(HB_OS_BSD)
   struct timeval oTime;
   struct timezone oZone;
   gettimeofday( &oTime, &oZone );
   return ( oTime.tv_sec );
#else
   struct timeb tb;
   struct tm * oTime;

   ftime( &tb );
   oTime = localtime( &tb.time );

   return ( (int) oTime->tm_sec );
#endif
}

void hb_gt_Tone( double dFrequency, double dDuration )
{
   BYTE szBell[] = { HB_CHAR_BEL, 0 };
   static int iSinceBell = -1;
   int iNow;
   
   HB_TRACE(HB_TR_DEBUG, ("hb_gt_Tone(%lf, %lf)", dFrequency, dDuration));

   HB_SYMBOL_UNUSED( dFrequency );
   HB_SYMBOL_UNUSED( dDuration );

   /* Output an ASCII BEL character to cause a sound */
   /* but throttle to max once per second, in case of sound */
   /* effects prgs calling lots of short tone sequences in */
   /* succession leading to BEL hell on the terminal */

   iNow = gtstd_get_seconds();

   if ( iNow != iSinceBell )
      out_stdout( szBell, 1 );
   
   iSinceBell = iNow;
}

char * hb_gt_Version( void )
{
   return "Harbour Terminal: Standard stream console";
}

USHORT hb_gt_DispCount()
{
   return s_uiDispCount;
}

void hb_gt_Replicate( USHORT uiRow, USHORT uiCol, BYTE byAttr, BYTE byChar, ULONG nLength )
{
   HB_TRACE(HB_TR_DEBUG, ("hb_gt_Replicate(%hu, %hu, %i, %i, %lu)", uiRow, uiCol, byAttr, byChar, nLength));

   while( nLength-- )
      hb_gt_xPutch( uiRow, uiCol++, byAttr, byChar );
}

USHORT hb_gt_Box( SHORT Top, SHORT Left, SHORT Bottom, SHORT Right,
                  BYTE * szBox, BYTE byAttr )
{
   USHORT ret = 1;
   SHORT Row;
   SHORT Col;
   SHORT Height;
   SHORT Width;

   if( Left >= 0 || Left < hb_gt_GetScreenWidth()
   || Right >= 0 || Right < hb_gt_GetScreenWidth()
   || Top >= 0 || Top < hb_gt_GetScreenHeight()
   || Bottom >= 0 || Bottom < hb_gt_GetScreenHeight() )
   {

      /* Ensure that box is drawn from top left to bottom right. */
      if( Top > Bottom )
      {
         SHORT tmp = Top;
         Top = Bottom;
         Bottom = tmp;
      }
      if( Left > Right )
      {
         SHORT tmp = Left;
         Left = Right;
         Right = tmp;
      }

      /* Draw the box or line as specified */
      Height = Bottom - Top + 1;
      Width  = Right - Left + 1;

      hb_gt_DispBegin();

      if( Height > 1 && Width > 1 && Top >= 0 && Top < hb_gt_GetScreenHeight() && Left >= 0 && Left < hb_gt_GetScreenWidth() )
         hb_gt_xPutch( Top, Left, byAttr, szBox[ 0 ] ); /* Upper left corner */

      Col = ( Height > 1 ? Left + 1 : Left );
      if(Col < 0 )
      {
         Width += Col;
         Col = 0;
      }
      if( Right >= hb_gt_GetScreenWidth() )
      {
         Width -= Right - hb_gt_GetScreenWidth();
      }

      if( Col <= Right && Col < hb_gt_GetScreenWidth() && Top >= 0 && Top < hb_gt_GetScreenHeight() )
         hb_gt_Replicate( Top, Col, byAttr, szBox[ 1 ], Width + ( (Right - Left) > 1 ? -2 : 0 ) ); /* Top line */

      if( Height > 1 && (Right - Left) > 1 && Right < hb_gt_GetScreenWidth() && Top >= 0 && Top < hb_gt_GetScreenHeight() )
         hb_gt_xPutch( Top, Right, byAttr, szBox[ 2 ] ); /* Upper right corner */

      if( szBox[ 8 ] && Height > 2 && Width > 2 )
      {
         for( Row = Top + 1; Row < Bottom; Row++ )
         {
            if( Row >= 0 && Row < hb_gt_GetScreenHeight() )
            {
               Col = Left;
               if( Col < 0 )
                  Col = 0; /* The width was corrected earlier. */
               else
                  hb_gt_xPutch( Row, Col++, byAttr, szBox[ 7 ] ); /* Left side */
               hb_gt_Replicate( Row, Col, byAttr, szBox[ 8 ], Width - 2 ); /* Fill */
               if( Right < hb_gt_GetScreenWidth() )
                  hb_gt_xPutch( Row, Right, byAttr, szBox[ 3 ] ); /* Right side */
            }
         }
      }
      else
      {
         for( Row = ( Width > 1 ? Top + 1 : Top ); Row < ( (Right - Left ) > 1 ? Bottom : Bottom + 1 ); Row++ )
         {
            if( Row >= 0 && Row < hb_gt_GetScreenHeight() )
            {
               if( Left >= 0 && Left < hb_gt_GetScreenWidth() )
                  hb_gt_xPutch( Row, Left, byAttr, szBox[ 7 ] ); /* Left side */
               if( ( Width > 1 || Left < 0 ) && Right < hb_gt_GetScreenWidth() )
                  hb_gt_xPutch( Row, Right, byAttr, szBox[ 3 ] ); /* Right side */
            }
         }
      }

      if( Height > 1 && Width > 1 )
      {
         if( Left >= 0 && Bottom < hb_gt_GetScreenHeight() )
            hb_gt_xPutch( Bottom, Left, byAttr, szBox[ 6 ] ); /* Bottom left corner */

         Col = Left + 1;
         if( Col < 0 )
            Col = 0; /* The width was corrected earlier. */

         if( Col <= Right && Bottom < hb_gt_GetScreenHeight() )
            hb_gt_Replicate( Bottom, Col, byAttr, szBox[ 5 ], Width - 2 ); /* Bottom line */

         if( Right < hb_gt_GetScreenWidth() && Bottom < hb_gt_GetScreenHeight() )
            hb_gt_xPutch( Bottom, Right, byAttr, szBox[ 4 ] ); /* Bottom right corner */
      }
      hb_gt_DispEnd();
      ret = 0;
   }

   return ret;
}

USHORT hb_gt_BoxD( SHORT Top, SHORT Left, SHORT Bottom, SHORT Right, BYTE * pbyFrame, BYTE byAttr )
{
   return hb_gt_Box( Top, Left, Bottom, Right, pbyFrame, byAttr );
}

USHORT hb_gt_BoxS( SHORT Top, SHORT Left, SHORT Bottom, SHORT Right, BYTE * pbyFrame, BYTE byAttr )
{
   return hb_gt_Box( Top, Left, Bottom, Right, pbyFrame, byAttr );
}

USHORT hb_gt_HorizLine( SHORT Row, SHORT Left, SHORT Right, BYTE byChar, BYTE byAttr )
{
   USHORT ret = 1;
   if( Row >= 0 && Row < hb_gt_GetScreenHeight() )
   {
      if( Left < 0 )
         Left = 0;
      else if( Left >= hb_gt_GetScreenWidth() )
         Left = hb_gt_GetScreenWidth() - 1;
   
      if( Right < 0 )
         Right = 0;
      else if( Right >= hb_gt_GetScreenWidth() )
         Right = hb_gt_GetScreenWidth() - 1;

      if( Left < Right )
         hb_gt_Replicate( Row, Left, byAttr, byChar, Right - Left + 1 );
      else
         hb_gt_Replicate( Row, Right, byAttr, byChar, Left - Right + 1 );
      ret = 0;
   }
   return ret;
}

USHORT hb_gt_VertLine( SHORT Col, SHORT Top, SHORT Bottom, BYTE byChar, BYTE byAttr )
{
   USHORT ret = 1;
   SHORT Row;

   if( Col >= 0 && Col < hb_gt_GetScreenWidth() )
   {
      if( Top < 0 )
         Top = 0;
      else if( Top >= hb_gt_GetScreenHeight() )
         Top = hb_gt_GetScreenHeight() - 1;

      if( Bottom < 0 )
         Bottom = 0;
      else if( Bottom >= hb_gt_GetScreenHeight() )
         Bottom = hb_gt_GetScreenHeight() - 1;

      if( Top <= Bottom )
         Row = Top;
      else
      {
         Row = Bottom;
         Bottom = Top;
      }
      while( Row <= Bottom )
         hb_gt_xPutch( Row++, Col, byAttr, byChar );
      ret = 0;
   }
   return ret;
}

BOOL hb_gt_PreExt()
{
   return TRUE;
}

BOOL hb_gt_PostExt()
{
   return TRUE;
}

BOOL hb_gt_Suspend()
{
   return TRUE;
}

BOOL hb_gt_Resume()
{
   return TRUE;
}


