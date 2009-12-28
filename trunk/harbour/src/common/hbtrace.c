/*
 * $Id$
 */

/*
 * Harbour Project source code:
 * Tracing functions.
 *
 * Copyright 2009 Viktor Szakats (harbour.01 syenar.hu)
 * Copyright 1999 Gonzalo Diethelm <gonzalo.diethelm@iname.com>
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

#define HB_OS_WIN_USED

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "hbapi.h"
#include "hbapifs.h"
#include "hb_io.h"
#include "hbtrace.h"

static int s_enabled = 1;
static int s_level   = -1;
static int s_flush   = 0;
#if defined( HB_OS_WIN ) && ! defined( HB_OS_WIN_CE )
static int s_winout  = 0;
#endif

static FILE * s_fp = NULL;

static const char * s_slevel[ HB_TR_LAST ] =
{
   "HB_TR_ALWAYS",
   "HB_TR_FATAL",
   "HB_TR_ERROR",
   "HB_TR_WARNING",
   "HB_TR_INFO",
   "HB_TR_DEBUG"
};

int hb_tracestate( int new_state )
{
   int old_state = s_enabled;

   if( new_state == 0 ||
       new_state == 1 )
      s_enabled = new_state;

   return old_state;
}

int hb_tracelevel( int new_level )
{
   int old_level = hb_tr_level();

   if( new_level >= HB_TR_ALWAYS &&
       new_level <  HB_TR_LAST )
      s_level = new_level;

   return old_level;
}

int hb_tr_level( void )
{
   if( s_level == -1 )
   {
      char * env;

      s_level = HB_TR_DEFAULT;

      /* ; */

      env = hb_getenv( "HB_TR_OUTPUT" );
      if( env != NULL && env[ 0 ] != '\0' )
      {
         s_fp = hb_fopen( env, "w" );

         if( s_fp == NULL )
            s_fp = stderr;
      }
      else
         s_fp = stderr;

      if( env )
         hb_xfree( ( void * ) env );

      /* ; */

      env = hb_getenv( "HB_TR_LEVEL" );
      if( env != NULL && env[ 0 ] != '\0' )
      {
         int i;

         for( i = 0; i < HB_TR_LAST; ++i )
         {
            if( hb_stricmp( env, s_slevel[ i ] ) == 0 ||
                hb_stricmp( env, s_slevel[ i ] + 6 ) == 0 )
            {
               s_level = i;
               break;
            }
         }
      }

      if( env )
         hb_xfree( ( void * ) env );

      /* ; */

#if defined( HB_OS_WIN ) && ! defined( HB_OS_WIN_CE )

      env = hb_getenv( "HB_TR_WINOUT" );
      if( env != NULL && env[ 0 ] != '\0' )
         s_winout = 1;
      else
         s_winout = 0;

      if( env )
         hb_xfree( ( void * ) env );

#endif

      /* ; */

      env = hb_getenv( "HB_TR_FLUSH" );
      if( env != NULL && env[ 0 ] != '\0' )
         s_flush = 1;
      else
         s_flush = 0;

      if( env )
         hb_xfree( ( void * ) env );
   }

   return s_level;
}

static void hb_tracelog_( int level, const char * file, int line, const char * proc,
                          const char * fmt, va_list ap )
{
   const char * pszLevel;

   /*
    * Clean up the file, so that instead of showing
    *
    *   ../../../foo/bar/baz.c
    *
    * we just show
    *
    *   foo/bar/baz.c
    */
   while( *file == '.' || *file == '/' || *file == '\\' )
      file++;

   pszLevel = level < 0 ? "(\?\?\?)" : s_slevel[ level ];

   /*
    * Print file and line.
    */
   if( proc )
      fprintf( s_fp, "%s:%d:%s(): %s ", file, line, proc, pszLevel );
   else
      fprintf( s_fp, "%s:%d: %s ", file, line, pszLevel );

   /*
    * Print the name and arguments for the function.
    */
   vfprintf( s_fp, fmt, ap );

   if( s_winout )
   /* TOFIX: va_end() is _required_ here according to all available documentation. */
   /* va_end( ap ); Generates access violation in the subsequent hb_vsnprintf */

   /*
    * Print a new-line.
    */
   fprintf( s_fp, "\n" );

   if( s_flush )
      fflush( s_fp );

#if defined( HB_OS_WIN ) && ! defined( HB_OS_WIN_CE )

   if( s_winout )
   {
      char buffer1[ 1024 ];
      char buffer2[ 1024 ];

      hb_vsnprintf( buffer1, sizeof( buffer1 ), fmt, ap );

      /* We add \r\n at the end of the buffer to make WinDbg display look readable. */
      if( proc )
         hb_snprintf( buffer2, sizeof( buffer2 ), "%s:%d:%s() %s %s\r\n",
                      file, line, proc, pszLevel, buffer1 );
      else
         hb_snprintf( buffer2, sizeof( buffer2 ), "%s:%d: %s %s\r\n",
                      file, line, pszLevel, buffer1 );

      #if defined( UNICODE )
      {
         TCHAR lpOutputString[ 2048 ];
         MultiByteToWideChar( CP_ACP, 0, buffer2, -1, lpOutputString, sizeof( lpOutputString ) );
         OutputDebugString( lpOutputString );
      }
      #else
         OutputDebugString( buffer2 );
      #endif
   }

#endif
}

void hb_tracelog( int level, const char * file, int line, const char * proc,
                  const char * fmt, ... )
{
   /*
    * If tracing is disabled, do nothing.
    */
   if( s_enabled && level <= hb_tr_level() )
   {
      va_list ap;
      va_start( ap, fmt );
      hb_tracelog_( level, file, line, proc, fmt, ap );
      va_end( ap );
   }
}

const char * hb_tr_file_ = "";
int          hb_tr_line_ = 0;
int          hb_tr_level_ = 0;

void hb_tr_trace( const char * fmt, ... )
{
   /*
    * If tracing is disabled, do nothing.
    */
   if( s_enabled )
   {
      va_list ap;
      va_start( ap, fmt );
      hb_tracelog_( hb_tr_level_, hb_tr_file_, hb_tr_line_, NULL, fmt, ap );
      va_end( ap );

      /*
       * Reset file and line.
       */
      hb_tr_level_ = -1;
      /* NOTE: resetting file name/line number will cause that we will unable
       * to report the location of code that allocated unreleased memory blocks
       * See hb_xalloc/hb_xgrab in src/vm/fm.c
       */
      if( hb_tr_level() < HB_TR_DEBUG )
      {
         hb_tr_file_ = "";
         hb_tr_line_ = -1;
      }
   }
}
