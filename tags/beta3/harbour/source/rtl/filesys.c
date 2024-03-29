/*
 * $Id$
 */

/*
 * Harbour Project source code:
 * The FileSys API (C level)
 *
 * Copyright 1999 {list of individual authors and e-mail addresses}
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

/*
 * The following parts are Copyright of the individual authors.
 * www - http://www.harbour-project.org
 *
 * Copyright 1999-2001 Viktor Szakats <viktor.szakats@syenar.hu>
 *    hb_fsSetError()
 *    hb_fsSetDevMode()
 *    hb_fsReadLarge()
 *    hb_fsWriteLarge()
 *    hb_fsCurDirBuff()
 *
 * Copyright 1999 Jose Lalin <dezac@corevia.com>
 *    hb_fsChDrv()
 *    hb_fsCurDrv()
 *    hb_fsIsDrv()
 *    hb_fsIsDevice()
 *
 * Copyright 2000 Luiz Rafael Culik <culik@sl.conex.net>
 *            and David G. Holm <dholm@jsd-llc.com>
 *    hb_fsEof()
 *
 * Copyright 2001 Jose Gimenez (JFG) <jfgimenez@wanadoo.es>
 *                                   <tecnico.sireinsa@ctv.es>
 *    Added __WIN32__ check for any compiler to use the Win32
 *    API calls to allow openning an unlimited number of files
 *    simultaneously.
 *
 * See doc/license.txt for licensing terms.
 *
 */

/* NOTE: In DOS/DJGPP under WinNT4 hb_fsSeek( fhnd, offset < 0, FS_SET) will
         set the file pointer to the passed negative value, and the subsequent
         hb_fsWrite() call will fail. In CA-Clipper hb_fsSeek() will fail,
         the pointer will not be moved, and thus the hb_fsWrite() call will
         successfully write the buffer to the current file position. [vszakats]

   This has been corrected by ptucker
 */

#if defined(HB_OS_LINUX)
#  define _LARGEFILE64_SOURCE
#endif

#ifndef HB_OS_WIN_32_USED
   #define HB_OS_WIN_32_USED
#endif

#include <string.h>
#include <ctype.h>

#include "hbapi.h"
#include "hbapifs.h"
#include "hbapierr.h"
#include "hb_io.h"
#include "hbset.h"

#if defined(OS_UNIX_COMPATIBLE)
   #include <unistd.h>
   #include <signal.h>
   #include <sys/types.h>
   #include <sys/wait.h>
   #if defined( HB_OS_DARWIN )
      #include <crt_externs.h>
      #define environ (*_NSGetEnviron())
   #elif !defined( __WATCOMC__ )
      extern char **environ;
   #endif
#endif

#if ( defined(__DMC__) || defined(__BORLANDC__) || defined(__IBMCPP__) || defined(_MSC_VER) || \
      defined(__MINGW32__) || defined(__WATCOMC__) ) && !defined( HB_OS_UNIX )
   #include <sys/stat.h>
   #include <share.h>
   #include <fcntl.h>
   #include <errno.h>
   #include <direct.h>
   #include <process.h>
   #if defined(__BORLANDC__)
      #include <dir.h>
      #include <dos.h>
   #elif defined(__WATCOMC__)
      #include <dos.h>
   #endif

   #if defined(_MSC_VER) || defined(__MINGW32__) || defined(__DMC__)
      #include <sys/locking.h>
      #define ftruncate _chsize
      #if defined(__MINGW32__) && !defined(_LK_UNLCK)
         #define _LK_UNLCK _LK_UNLOCK
      #endif
   #else
      #define ftruncate chsize
   #endif
   #if !defined(HAVE_POSIX_IO)
      #define HAVE_POSIX_IO
   #endif
#elif defined(__GNUC__) || defined(HB_OS_UNIX)
   #include <sys/types.h>
   #include <sys/stat.h>
   #include <fcntl.h>
   #include <errno.h>
   #if defined(__CYGWIN__)
      #include <io.h>
   #elif defined(__DJGPP__)
      #include <dir.h>
   #endif
   #if !defined(HAVE_POSIX_IO)
      #define HAVE_POSIX_IO
   #endif
#endif

#if defined(__MPW__)
   #include <fcntl.h>
#endif

#if defined(HB_OS_HPUX)
   extern int fdatasync(int fildes);
#elif defined(HB_OS_DOS)
   #include <dos.h>
#elif defined(HB_OS_OS2)
   #include <sys/signal.h>
   #include <sys/process.h>
   #include <sys/wait.h>
   #include <share.h>
   #ifndef SH_COMPAT
      #define SH_COMPAT    0x0000
   #endif
#elif defined( HB_WIN32_IO )
   #include <windows.h>

   #if ( defined(__DMC__) || defined( _MSC_VER ) || defined( __LCC__ ) ) && !defined( INVALID_SET_FILE_POINTER )
      #define INVALID_SET_FILE_POINTER ((DWORD)-1)
   #endif
#endif
#if defined( HB_USE_SHARELOCKS ) && defined( HB_USE_BSDLOCKS )
   #include <sys/file.h>
#endif

/* 27/08/2004 - <maurilio.longo@libero.it>
                HB_FS_GETDRIVE() should return a number in the range 0..25 ('A'..'Z')
                HB_FS_SETDRIVE() should accept a number inside same range.

                If a particular platform/compiler returns/accepts different ranges of
                values, simply define a branch for that platform.

                NOTE: There is not an implicit "current disk", ALWAYS use

                        my_func( hb_fsCurDrv(), ...)

                      to refer to current disk
*/

#if defined( __DJGPP__ )
   #define HB_FS_GETDRIVE(n)  do { n = getdisk(); } while( 0 )
   #define HB_FS_SETDRIVE(n)  setdisk( n )

#elif defined( __WATCOMC__ )
   #define HB_FS_GETDRIVE(n)  do { _dos_getdrive( &( n ) ); --( n ); } while( 0 )
   #define HB_FS_SETDRIVE(n)  do { \
                                 UINT uiDummy; \
                                 _dos_setdrive( ( n ) + 1, &uiDummy ); \
                              } while( 0 )

#elif defined(HB_OS_OS2)
   #define HB_FS_GETDRIVE(n)  do { n = _getdrive() - 'A'; } while( 0 )
   #define HB_FS_SETDRIVE(n)  _chdrive( ( n ) + 'A' )

#else
   #define HB_FS_GETDRIVE(n)  do { \
                                 n = _getdrive(); \
                                 n -= ( ( n ) < 'A' ) ? 1 : 'A'; \
                              } while( 0 )
   #define HB_FS_SETDRIVE(n)  _chdrive( ( n ) + 1 )

#endif

#ifndef O_BINARY
   #define O_BINARY     0       /* O_BINARY not defined on Linux */
#endif

#ifndef O_LARGEFILE
   #define O_LARGEFILE  0       /* O_LARGEFILE is used for LFS in 32-bit Linux */
#endif

#if defined(HAVE_POSIX_IO) || defined( HB_WIN32_IO ) || defined(_MSC_VER) || defined(__MINGW32__) || defined(__LCC__) || defined(__DMC__)
/* Only compilers with Posix or Posix-like I/O support are supported */
   #define HB_FS_FILE_IO
#endif

#if defined(__DMC__) || defined(_MSC_VER) || defined(__MINGW32__) || defined(__IBMCPP__) || defined(__WATCOMC__) || defined(HB_OS_OS2)
/* These compilers use sopen() rather than open(), because their
   versions of open() do not support combined O_ and SH_ flags */
   #define HB_FS_SOPEN
#endif

#if UINT_MAX == USHRT_MAX
   #define LARGE_MAX ( UINT_MAX - 1L )
#else
   #define HB_FS_LARGE_OPTIMIZED
#endif

static BOOL s_fUseWaitLocks = TRUE;

#if defined(HB_FS_FILE_IO)

#if defined(HB_WIN32_IO)

   #if defined( __LCC__ )
      __inline void * LongToHandle( const long h )
      {
          return((void *) (INT_PTR) h );
      }
   #endif

   #if ( defined(__DMC__) || ( defined( _MSC_VER ) && ( _MSC_VER >= 1010 ) && ( ! defined( _BASETSD_H_) || ! defined( HandleToLong ) || defined(__USE_INLINE__) ) && ! defined( __POCC__ ) ) )
       #if defined(__DMC__) && !defined(INT_PTR)
          #ifdef _WIN64
             typedef __int64 INT_PTR, *PINT_PTR;
          #else
             typedef long INT_PTR, *PINT_PTR;
          #endif
       #endif

      #ifndef _WIN64 /* MSVC64 already provides these funtions */ 
      __inline void * LongToHandle( const long h )
      {
          return((void *) (INT_PTR) h );
      }

      __inline long HandleToLong( const void *h )
      {
         return((long) h );
      }
      #endif
   #endif

HANDLE DostoWinHandle( FHANDLE fHandle )
{
   HANDLE hHandle = (HANDLE) LongToHandle( fHandle );

   switch( fHandle )
   {
      case 0:
        return GetStdHandle(STD_INPUT_HANDLE);

      case 1:
        return GetStdHandle(STD_OUTPUT_HANDLE);

      case 2:
        return GetStdHandle(STD_ERROR_HANDLE);

      default:
        return hHandle;
   }
}

static void convert_open_flags( BOOL fCreate, USHORT uiAttr, USHORT uiFlags,
                                DWORD *dwMode, DWORD *dwShare,
                                DWORD *dwCreat, DWORD *dwAttr )
{
   if( fCreate )
   {
      *dwCreat = CREATE_ALWAYS;
      *dwMode = GENERIC_READ | GENERIC_WRITE;
   }
   else
   {
      if( uiFlags & FO_CREAT )
      {
         if( uiFlags & FO_EXCL )
            *dwCreat = CREATE_NEW;
         else if( uiFlags & FO_TRUNC )
            *dwCreat = CREATE_ALWAYS;
         else
            *dwCreat = OPEN_ALWAYS;
      }
      else if( uiFlags & FO_TRUNC )
      {
         *dwCreat = TRUNCATE_EXISTING;
      }
      else
      {
         *dwCreat = OPEN_EXISTING;
      }

      *dwMode = 0;
      switch( uiFlags & ( FO_READ | FO_WRITE | FO_READWRITE ) )
      {
         case FO_READWRITE:
            *dwMode |= GENERIC_READ | GENERIC_WRITE;
            break;
         case FO_WRITE:
            *dwMode |= GENERIC_WRITE;
            break;
         case FO_READ:
            *dwMode |= GENERIC_READ;
            break;
      }
   }

   /* shared flags */
   switch( uiFlags & ( FO_DENYREAD | FO_DENYWRITE | FO_EXCLUSIVE | FO_DENYNONE ) )
   {
      case FO_DENYREAD:
         *dwShare = FILE_SHARE_WRITE;
         break;
      case FO_DENYWRITE:
         *dwShare = FILE_SHARE_READ;
         break;
      case FO_EXCLUSIVE:
         *dwShare = 0;
         break;
      default:
         *dwShare = FILE_SHARE_WRITE | FILE_SHARE_READ;
         break;
   }

   /* file attributes flags */
   if( uiAttr == FC_NORMAL )
   {
      *dwAttr = FILE_ATTRIBUTE_NORMAL;
   }
   else
   {
      *dwAttr = FILE_ATTRIBUTE_ARCHIVE;
      if( uiAttr & FC_READONLY )
         *dwAttr |= FILE_ATTRIBUTE_READONLY;
      if( uiAttr & FC_HIDDEN )
         *dwAttr |= FILE_ATTRIBUTE_HIDDEN;
      if( uiAttr & FC_SYSTEM )
         *dwAttr |= FILE_ATTRIBUTE_SYSTEM;
   }
}

#else

static void convert_open_flags( BOOL fCreate, USHORT uiAttr, USHORT uiFlags,
                                int *flags, unsigned *mode,
                                int *share, int *attr )
{
   HB_TRACE(HB_TR_DEBUG, ("convert_open_flags(%d, %hu, %hu, %p, %p, %p, %p)", fCreate, uiAttr, uiFlags, flags, mode, share, attr));

   /* file access mode */
#if defined( HB_OS_UNIX )
   *mode = ( uiAttr & FC_HIDDEN ) ? S_IRUSR : ( S_IRUSR | S_IRGRP | S_IROTH );
   if( !( uiAttr & FC_READONLY ) )
   {
      if( *mode & S_IRUSR ) *mode |= S_IWUSR;
      if( *mode & S_IRGRP ) *mode |= S_IWGRP;
      if( *mode & S_IROTH ) *mode |= S_IWOTH;
   }
   if( uiAttr & FC_SYSTEM )
   {
      if( *mode & S_IRUSR ) *mode |= S_IXUSR;
      if( *mode & S_IRGRP ) *mode |= S_IXGRP;
      if( *mode & S_IROTH ) *mode |= S_IXOTH;
   }
#else
   *mode = S_IREAD |
           ( ( uiAttr & FC_READONLY ) ? 0 : S_IWRITE ) |
           ( ( uiAttr & FC_SYSTEM ) ? S_IEXEC : 0 );
#endif

   /* dos file attributes */
#if defined(HB_FS_DOSATTR)
   if( uiAttr == FC_NORMAL )
   {
      *attr = _A_NORMAL;
   }
   else
   {
      *attr = _A_ARCH;
      if( uiAttr & FC_READONLY )
         *attr |= _A_READONLY;
      if( uiAttr & FC_HIDDEN )
         *attr |= _A_HIDDEN;
      if( uiAttr & FC_SYSTEM )
         *attr |= _A_SYSTEM;
   }
#else
   *attr = 0;
#endif

   if( fCreate )
   {
      *flags = O_RDWR | O_CREAT | O_TRUNC | O_BINARY | O_LARGEFILE |
               ( ( uiFlags & FO_EXCL ) ? O_EXCL : 0 );
   }
   else
   {
      *attr = 0;
      *flags = O_BINARY | O_LARGEFILE;
      switch( uiFlags & ( FO_READ | FO_WRITE | FO_READWRITE ) )
      {
         case FO_READ:
            *flags |= O_RDONLY;
            break;
         case FO_WRITE:
            *flags |= O_WRONLY;
            break;
         case FO_READWRITE:
            *flags |= O_RDWR;
            break;
         default:
            /* this should not happen and it's here to force default OS behavior */
            *flags |= ( O_RDONLY | O_WRONLY | O_RDWR );
            break;
      }

      if( uiFlags & FO_CREAT ) *flags |= O_CREAT;
      if( uiFlags & FO_TRUNC ) *flags |= O_TRUNC;
      if( uiFlags & FO_EXCL  ) *flags |= O_EXCL;
   }

   /* shared flags (HB_FS_SOPEN) */
#if defined(_MSC_VER) || defined(__DMC__)
   if( ( uiFlags & FO_DENYREAD ) == FO_DENYREAD )
      *share = _SH_DENYRD;
   else if( uiFlags & FO_EXCLUSIVE )
      *share = _SH_DENYRW;
   else if( uiFlags & FO_DENYWRITE )
      *share = _SH_DENYWR;
   else if( uiFlags & FO_DENYNONE )
      *share = _SH_DENYNO;
   else
      *share = _SH_COMPAT;
#elif !defined( HB_OS_UNIX )
   if( ( uiFlags & FO_DENYREAD ) == FO_DENYREAD )
      *share = SH_DENYRD;
   else if( uiFlags & FO_EXCLUSIVE )
      *share = SH_DENYRW;
   else if( uiFlags & FO_DENYWRITE )
      *share = SH_DENYWR;
   else if( uiFlags & FO_DENYNONE )
      *share = SH_DENYNO;
   else
      *share = SH_COMPAT;
#else
   *share = 0;
#endif

   HB_TRACE(HB_TR_INFO, ("convert_open_flags: flags=0x%04x, mode=0x%04x, share=0x%04x, attr=0x%04x", *flags, *mode, *share, *attr));

}
#endif

static int convert_seek_flags( USHORT uiFlags )
{
   /* by default FS_SET is set */
   int result_flags = SEEK_SET;

   HB_TRACE(HB_TR_DEBUG, ("convert_seek_flags(%hu)", uiFlags));

   if( uiFlags & FS_RELATIVE )
      result_flags = SEEK_CUR;

   if( uiFlags & FS_END )
      result_flags = SEEK_END;

   return result_flags;
}

#endif


/*
 * FILESYS.API FUNCTIONS --
 */

HB_EXPORT FHANDLE  hb_fsGetOsHandle( FHANDLE hFileHandle )
{
   HB_TRACE(HB_TR_DEBUG, ("hb_fsGetOsHandle(%p)", hFileHandle));

#if defined(HB_WIN32_IO)
   return ( FHANDLE ) DostoWinHandle( hFileHandle );
#else
   return hFileHandle;
#endif
}

HB_EXPORT FHANDLE hb_fsPOpen( BYTE * pFilename, BYTE * pMode )
{
   FHANDLE hFileHandle = FS_ERROR;

   HB_TRACE(HB_TR_DEBUG, ("hb_fsPOpen(%p, %s)", pFilename, pMode));

#if defined(OS_UNIX_COMPATIBLE)
   {
      FHANDLE hPipeHandle[2], hNullHandle;
      pid_t pid;
      BYTE * pbyTmp;
      BOOL bRead;
      ULONG ulLen;
      int iMaxFD;

      ulLen = strlen( ( char * ) pFilename );
      if( pMode && ( *pMode == 'r' || *pMode == 'w' ) )
         bRead = ( *pMode == 'r' );
      else
      {
         if( pFilename[0] == '|' )
            bRead = FALSE;
         else if( pFilename[ ulLen - 1 ] == '|' )
            bRead = TRUE;
         else
            bRead = FALSE;
      }

      if( pFilename[0] == '|' )
      {
          ++pFilename;
          --ulLen;
      }
      if( pFilename[ ulLen - 1 ] == '|' )
      {
          pbyTmp = ( BYTE * ) hb_strdup( ( char * ) pFilename );
          pbyTmp[--ulLen] = 0;
          pFilename = pbyTmp;
      } else
          pbyTmp = NULL;

      if( pipe( hPipeHandle ) == 0 ) {
         if( ( pid = fork() ) != -1 ) {
            if( pid != 0 ) {
               if( bRead ) {
                  close( hPipeHandle[ 1 ] );
                  hFileHandle = hPipeHandle[ 0 ];
               } else {
                  close( hPipeHandle[ 0 ] );
                  hFileHandle = hPipeHandle[ 1 ];
               }
            } else {
               char *argv[4];
               argv[0] = "sh";
               argv[1] = "-c";
               argv[2] = ( char * ) pFilename;
               argv[3] = ( char * ) 0;
               hNullHandle = open("/dev/null", O_RDWR);
               if( bRead ) {
                  close( hPipeHandle[ 0 ] );
                  dup2( hPipeHandle[ 1 ], 1 );
                  dup2( hNullHandle, 0 );
                  dup2( hNullHandle, 2 );
               } else {
                  close( hPipeHandle[ 1 ] );
                  dup2( hPipeHandle[ 0 ], 0 );
                  dup2( hNullHandle, 1 );
                  dup2( hNullHandle, 2 );
               }
               iMaxFD = sysconf( _SC_OPEN_MAX );
               if ( iMaxFD < 3 )
                  iMaxFD = 1024;
               for( hNullHandle = 3; hNullHandle < iMaxFD; ++hNullHandle )
                  close(hNullHandle);
               setuid(getuid());
               setgid(getgid());
               execve("/bin/sh", argv, environ);
               exit(1);
            }
         }
         else
         {
            close( hPipeHandle[0] );
            close( hPipeHandle[1] );
         }
      }

      hb_fsSetIOError( hFileHandle != FS_ERROR, 0 );

      if( pbyTmp )
         hb_xfree( pbyTmp );
   }
#else

   HB_SYMBOL_UNUSED( pFilename );
   HB_SYMBOL_UNUSED( pMode );

   hb_fsSetError( (USHORT) FS_ERROR );

#endif

   return hFileHandle;
}

HB_EXPORT FHANDLE hb_fsOpen( BYTE * pFilename, USHORT uiFlags )
{
   FHANDLE hFileHandle;

   HB_TRACE(HB_TR_DEBUG, ("hb_fsOpen(%p, %hu)", pFilename, uiFlags));

   pFilename = hb_fileNameConv( hb_strdup( ( char * ) pFilename) );

#if defined(HB_WIN32_IO)
   {
      DWORD dwMode, dwShare, dwCreat, dwAttr;
      HANDLE hFile;

      convert_open_flags( FALSE, FC_NORMAL, uiFlags, &dwMode, &dwShare, &dwCreat, &dwAttr );

      hFile = ( HANDLE ) CreateFile( ( char * ) pFilename, dwMode, dwShare,
                                     NULL, dwCreat, dwAttr, NULL );

      hb_fsSetIOError( hFile != ( HANDLE ) INVALID_HANDLE_VALUE, 0 );

      hFileHandle = HandleToLong(hFile);
   }
#elif defined(HB_FS_FILE_IO)
   {
      int flags, share, attr;
      unsigned mode;

      convert_open_flags( FALSE, FC_NORMAL, uiFlags, &flags, &mode, &share, &attr );
#if defined(_MSC_VER) || defined(__DMC__)
      if( share )
         hFileHandle = _sopen( ( char * ) pFilename, flags, share, mode );
      else
         hFileHandle = _open( ( char * ) pFilename, flags, mode );
#elif defined(HB_FS_SOPEN)
      if( share )
         hFileHandle = sopen( ( char * ) pFilename, flags, share, mode );
      else
         hFileHandle = open( ( char * ) pFilename, flags, mode );
#else
      hFileHandle = open( ( char * ) pFilename, flags | share, mode );
#endif
      hb_fsSetIOError( hFileHandle != FS_ERROR, 0 );
   }
#else

   hFileHandle = FS_ERROR;
   hb_fsSetError( (USHORT) FS_ERROR );

#endif

   hb_xfree( pFilename );

   return hFileHandle;
}

HB_EXPORT FHANDLE hb_fsCreate( BYTE * pFilename, USHORT uiAttr )
{
   FHANDLE hFileHandle;

   HB_TRACE(HB_TR_DEBUG, ("hb_fsCreate(%p, %hu)", pFilename, uiAttr));

   pFilename = hb_fileNameConv( hb_strdup( ( char * ) pFilename ) );

#if defined(HB_WIN32_IO)
   {
      DWORD dwMode, dwShare, dwCreat, dwAttr;
      HANDLE hFile;

      convert_open_flags( TRUE, uiAttr, FO_EXCLUSIVE, &dwMode, &dwShare, &dwCreat, &dwAttr );

      hFile = ( HANDLE ) CreateFile( ( char * ) pFilename, dwMode, dwShare,
                                     NULL, dwCreat, dwAttr, NULL );

      hb_fsSetIOError( hFile != ( HANDLE ) INVALID_HANDLE_VALUE, 0 );

      hFileHandle = HandleToLong(hFile);
   }
#elif defined(HB_FS_FILE_IO)
   {
      int flags, share, attr;
      unsigned mode;
      convert_open_flags( TRUE, uiAttr, FO_EXCLUSIVE, &flags, &mode, &share, &attr );

#if defined(HB_FS_DOSCREAT)
      hFileHandle = _creat( ( char * ) pFilename, attr );
#elif defined(HB_FS_SOPEN)
      hFileHandle = open( ( char * ) pFilename, flags, mode );
#else
      hFileHandle = open( ( char * ) pFilename, flags | share, mode );
#endif
      hb_fsSetIOError( hFileHandle != FS_ERROR, 0 );
   }
#else

   hFileHandle = FS_ERROR;
   hb_fsSetError( (USHORT) FS_ERROR );

#endif

   hb_xfree( pFilename );

   return hFileHandle;
}

/* Derived from hb_fsCreate()

   NOTE: The default opening mode differs from the one used in hb_fsCreate()
         [vszakats]
 */

HB_EXPORT FHANDLE hb_fsCreateEx( BYTE * pFilename, USHORT uiAttr, USHORT uiFlags )
{
   FHANDLE hFileHandle;

   HB_TRACE(HB_TR_DEBUG, ("hb_fsCreateEx(%p, %hu, %hu)", pFilename, uiAttr, uiFlags));

   pFilename = hb_fileNameConv( hb_strdup( ( char * ) pFilename ) );

#if defined( HB_WIN32_IO )
   {
      DWORD dwMode, dwShare, dwCreat, dwAttr;
      HANDLE hFile;

      convert_open_flags( TRUE, uiAttr, uiFlags, &dwMode, &dwShare, &dwCreat, &dwAttr );

      hFile = ( HANDLE ) CreateFile( ( char * ) pFilename, dwMode, dwShare,
                                     NULL, dwCreat, dwAttr, NULL );

      hb_fsSetIOError( hFile != ( HANDLE ) INVALID_HANDLE_VALUE, 0 );

      hFileHandle = HandleToLong(hFile);
   }
#elif defined(HB_FS_FILE_IO)
   {
      int flags, share, attr;
      unsigned mode;
      convert_open_flags( TRUE, uiAttr, uiFlags, &flags, &mode, &share, &attr );

#if defined(HB_FS_SOPEN)
      hFileHandle = open( ( char * ) pFilename, flags, mode );
#else
      hFileHandle = open( ( char * ) pFilename, flags | share, mode );
#endif
      hb_fsSetIOError( hFileHandle != FS_ERROR, 0 );
   }
#else

   hFileHandle = FS_ERROR;
   hb_fsSetError( (USHORT) FS_ERROR );

#endif

   hb_xfree( pFilename );

   return hFileHandle;
}

HB_EXPORT void hb_fsClose( FHANDLE hFileHandle )
{
   HB_TRACE(HB_TR_DEBUG, ("hb_fsClose(%p)", hFileHandle));

#if defined(HB_FS_FILE_IO)

   #if defined(HB_WIN32_IO)
      hb_fsSetIOError( CloseHandle( DostoWinHandle( hFileHandle ) ), 0 );
   #else
      hb_fsSetIOError( close( hFileHandle ) == 0, 0 );
   #endif

#else

   hb_fsSetError( (USHORT) FS_ERROR );

#endif
}

HB_EXPORT BOOL    hb_fsSetDevMode( FHANDLE hFileHandle, USHORT uiDevMode )
{
   HB_TRACE(HB_TR_DEBUG, ("hb_fsSetDevMode(%p, %hu)", hFileHandle, uiDevMode));

   /* TODO: HB_WIN32_IO support */

#if defined(__BORLANDC__) || defined(__IBMCPP__) || defined(__DJGPP__) || \
    defined(__CYGWIN__) || defined(__WATCOMC__) || defined(HB_OS_OS2)
{
   int iRet = 0;

#if defined(HB_WIN32_IO)
   if( hFileHandle > 2 )
      iRet = -1;
   else
#endif
   switch( uiDevMode )
   {
      case FD_BINARY:
         iRet = setmode( hFileHandle, O_BINARY );
         break;

      case FD_TEXT:
         iRet = setmode( hFileHandle, O_TEXT );
         break;
   }

   hb_fsSetIOError( iRet != -1, 0 );

   return iRet != -1;
}
#elif defined(_MSC_VER) || defined(__MINGW32__) || defined(__DMC__)
{
   int iRet = 0;

#if defined(HB_WIN32_IO)
   if( hFileHandle > 2 )
      iRet = -1;
   else
#endif
   switch( uiDevMode )
   {
      case FD_BINARY:
         iRet = _setmode( hFileHandle, _O_BINARY );
         break;

      case FD_TEXT:
         iRet = _setmode( hFileHandle, _O_TEXT );
         break;
   }

   hb_fsSetIOError( iRet != -1, 0 );

   return iRet != -1;
}
#elif defined( HB_OS_UNIX )

   HB_SYMBOL_UNUSED( hFileHandle );

   if( uiDevMode == FD_TEXT )
   {
      hb_fsSetError( ( USHORT ) FS_ERROR );
      return FALSE;
   }

   hb_fsSetError( 0 );
   return TRUE;

#else

   hb_fsSetError( ( USHORT ) FS_ERROR );
   return FALSE;

#endif
}

HB_EXPORT USHORT  hb_fsRead( FHANDLE hFileHandle, BYTE * pBuff, USHORT uiCount )
{
   USHORT uiRead;

   HB_TRACE(HB_TR_DEBUG, ("hb_fsRead(%p, %p, %hu)", hFileHandle, pBuff, uiCount));

#if defined(HB_FS_FILE_IO)

   #if defined(HB_WIN32_IO)
      {
         DWORD dwRead ;
         BOOL fResult;

         fResult = ReadFile( DostoWinHandle(hFileHandle), pBuff, (DWORD)uiCount, &dwRead, NULL );
         hb_fsSetIOError( fResult, 0 );

         uiRead = fResult ? ( USHORT ) dwRead : 0;
      }
   #else
      uiRead = read( hFileHandle, pBuff, uiCount );
      hb_fsSetIOError( uiRead != ( USHORT ) -1, 0 );
   #endif

   if( uiRead == ( USHORT ) -1 )
      uiRead = 0;

#else

   uiRead = 0;
   hb_fsSetError( (USHORT) FS_ERROR );

#endif

   return uiRead;
}

HB_EXPORT USHORT  hb_fsWrite( FHANDLE hFileHandle, const BYTE * pBuff, USHORT uiCount )
{
   USHORT uiWritten;

   HB_TRACE(HB_TR_DEBUG, ("hb_fsWrite(%p, %p, %hu)", hFileHandle, pBuff, uiCount));

#if defined(HB_FS_FILE_IO)

   #if defined(HB_WIN32_IO)
      {
         DWORD dwWritten = 0;
         BOOL fResult;

         if ( uiCount )
         {
             fResult = WriteFile( DostoWinHandle(hFileHandle), pBuff, uiCount, &dwWritten, NULL );
         }
         else
         {
             dwWritten = 0;
             fResult = SetEndOfFile( DostoWinHandle(hFileHandle) );
         }
         hb_fsSetIOError( fResult, 0 );

         uiWritten = fResult ? ( USHORT ) dwWritten : 0;
      }
   #else
      if( uiCount )
      {
         uiWritten = write( hFileHandle, pBuff, uiCount );
         hb_fsSetIOError( uiWritten != ( USHORT ) -1, 0 );
         if( uiWritten == ( USHORT ) -1 )
            uiWritten = 0;
      }
      else
      {
#if defined(HB_OS_LINUX) && defined(__USE_LARGEFILE64)
         /*
          * The macro: __USE_LARGEFILE64 is set when _LARGEFILE64_SOURCE is
          * define and efectively enables lseek64/flock64/ftruncate64 functions
          * on 32bit machines.
          */
         hb_fsSetIOError( ftruncate64( hFileHandle, lseek64( hFileHandle, 0L, SEEK_CUR ) ) != -1, 0 );
#else
         hb_fsSetIOError( ftruncate( hFileHandle, lseek( hFileHandle, 0L, SEEK_CUR ) ) != -1, 0 );
#endif
         uiWritten = 0;
      }
   #endif
#else

   uiWritten = 0;
   hb_fsSetError( (USHORT) FS_ERROR );

#endif

   return uiWritten;
}

HB_EXPORT ULONG hb_fsReadLarge( FHANDLE hFileHandle, BYTE * pBuff, ULONG ulCount )
{
   ULONG ulRead;

   HB_TRACE(HB_TR_DEBUG, ("hb_fsReadLarge(%p, %p, %lu)", hFileHandle, pBuff, ulCount));

#if defined(HB_FS_FILE_IO)

   #if defined(HB_WIN32_IO)
   {
      hb_fsSetIOError( ReadFile( DostoWinHandle(hFileHandle),
                                 pBuff, ulCount, &ulRead, NULL ), 0 );
   }
   #elif defined(HB_FS_LARGE_OPTIMIZED)
   {
      ulRead = read( hFileHandle, pBuff, ulCount );
      hb_fsSetIOError( ulRead != (ULONG) -1, 0 );
   }
   #else
   {
      ULONG ulLeftToRead = ulCount;
      USHORT uiToRead;
      USHORT uiRead;
      BYTE * pPtr = pBuff;

      ulRead = 0;

      while( ulLeftToRead )
      {
         /* Determine how much to read this time */
         if( ulLeftToRead > ( ULONG ) INT_MAX )
         {
            uiToRead = INT_MAX;
            ulLeftToRead -= ( ULONG ) uiToRead;
         }
         else
         {
            uiToRead = ( USHORT ) ulLeftToRead;
            ulLeftToRead = 0;
         }

         uiRead = read( hFileHandle, pPtr, uiToRead );
         /* -1 on bad hFileHandle
             0 on disk full
          */

         if( uiRead == 0 )
            break;

         if( uiRead == ( USHORT ) -1 )
         {
            uiRead = 0;
            break;
         }

         ulRead += ( ULONG ) uiRead;
         pPtr += uiRead;
      }
      hb_fsSetIOError( ulLeftToRead == 0, 0 );
   }
   #endif

#else

   ulRead = 0;
   hb_fsSetError( (USHORT) FS_ERROR );

#endif

   return ulRead;
}

HB_EXPORT ULONG hb_fsWriteLarge( FHANDLE hFileHandle, const BYTE * pBuff, ULONG ulCount )
{
   ULONG ulWritten;

   HB_TRACE(HB_TR_DEBUG, ("hb_fsWriteLarge(%p, %p, %lu)", hFileHandle, pBuff, ulCount));

#if defined(HB_FS_FILE_IO)

   #if defined(HB_WIN32_IO)
   {
      ulWritten = 0;
      if( ulCount )
      {
         hb_fsSetIOError( WriteFile( DostoWinHandle( hFileHandle), pBuff, ulCount, &ulWritten, NULL ), 0 );
      }
      else
      {
         hb_fsSetIOError( SetEndOfFile( DostoWinHandle(hFileHandle) ), 0 );
      }
   }
   #else
      if( ulCount )
      #if defined(HB_FS_LARGE_OPTIMIZED)
         {
            ulWritten = write( hFileHandle, pBuff, ulCount );
            hb_fsSetIOError( ulWritten != ( ULONG ) -1, 0 );
            if( ulWritten == ( ULONG ) -1 )
               ulWritten = 0;
         }
      #else
         {
            ULONG ulLeftToWrite = ulCount;
            USHORT uiToWrite;
            USHORT uiWritten;
            BYTE * pPtr = ( BYTE * ) pBuff;

            ulWritten = 0;

            while( ulLeftToWrite )
            {
               /* Determine how much to write this time */
               if( ulLeftToWrite > ( ULONG ) INT_MAX )
               {
                  uiToWrite = INT_MAX;
                  ulLeftToWrite -= ( ULONG ) uiToWrite;
               }
               else
               {
                  uiToWrite = ( USHORT ) ulLeftToWrite;
                  ulLeftToWrite = 0;
               }

               uiWritten = write( hFileHandle, pPtr, uiToWrite );

               /* -1 on bad hFileHandle
                   0 on disk full
                */

               if( uiWritten == 0 )
                  break;

               if( uiWritten == ( USHORT ) -1 )
               {
                  uiWritten = 0;
                  break;
               }

               ulWritten += ( ULONG ) uiWritten;
               pPtr += uiWritten;
            }
            hb_fsSetIOError( ulLeftToWrite == 0, 0 );
         }
      #endif
      else
      {
#if defined(HB_OS_LINUX) && defined(__USE_LARGEFILE64)
         /*
          * The macro: __USE_LARGEFILE64 is set when _LARGEFILE64_SOURCE is
          * define and efectively enables lseek64/flock64/ftruncate64 functions
          * on 32bit machines.
          */
         hb_fsSetIOError( ftruncate64( hFileHandle, lseek64( hFileHandle, 0L, SEEK_CUR ) ) != -1, 0 );
#else
         hb_fsSetIOError( ftruncate( hFileHandle, lseek( hFileHandle, 0L, SEEK_CUR ) ) != -1, 0 );
#endif
         ulWritten = 0;
      }

   #endif

#else

   hb_fsSetError( (USHORT) FS_ERROR );

#endif

   return ulWritten;
}

HB_EXPORT void hb_fsCommit( FHANDLE hFileHandle )
{
   HB_TRACE(HB_TR_DEBUG, ("hb_fsCommit(%p)", hFileHandle));

#if defined(HB_OS_WIN_32)
   {
      #if defined(HB_WIN32_IO)
         hb_fsSetIOError( FlushFileBuffers( ( HANDLE ) DostoWinHandle( hFileHandle ) ), 0 );
      #else
         #if defined(__WATCOMC__)
            hb_fsSetIOError( fsync( hFileHandle ) == 0, 0 );
         #else
            hb_fsSetIOError( _commit( hFileHandle ) == 0, 0 );
         #endif
      #endif
   }

#elif defined(HB_OS_OS2)

   {
      errno = 0;
      /* TODO: what about error code from DosResetBuffer() call? */
      DosResetBuffer( hFileHandle );
      hb_fsSetIOError( errno == 0, 0 );
   }

#elif defined(HB_OS_UNIX)

   /* NOTE: close() functions releases all lock regardles if it is an
    * original or duplicated file handle
   */
   #if defined(_POSIX_SYNCHRONIZED_IO) && _POSIX_SYNCHRONIZED_IO + 0 > 0
      /* faster - flushes data buffers only, without updating directory info
      */
      hb_fsSetIOError( fdatasync( hFileHandle ) == 0, 0 );
   #else
      /* slower - flushes all file data buffers and i-node info
      */
      hb_fsSetIOError( fsync( hFileHandle ) == 0, 0 );
   #endif

#elif defined(__WATCOMC__)

   hb_fsSetIOError( fsync( hFileHandle ) == 0, 0 );

#elif defined(HB_FS_FILE_IO) && !defined(HB_OS_OS2) && !defined(HB_OS_UNIX)

   /* This hack is very dangerous. POSIX standard define that if _ANY_
      file handle is closed all locks set by the process on the file
      pointed by this descriptor are removed. It doesn't matter they
      were done using different descriptor. It means that we now clean
      all locks on hFileHandle with the code below if the OS is POSIX
      compilant. I vote to disable it.
    */
   {
      int dup_handle;
      BOOL fResult = FALSE;

      dup_handle = dup( hFileHandle );
      if( dup_handle != -1 )
      {
         close( dup_handle );
         fResult = TRUE;
      }
      hb_fsSetIOError( fResult, 0 );
   }

#else

   hb_fsSetError( (USHORT) FS_ERROR );

#endif
}

HB_EXPORT  BOOL hb_fsLock   ( FHANDLE hFileHandle, ULONG ulStart,
                      ULONG ulLength, USHORT uiMode )
{
   BOOL bResult;

   HB_TRACE(HB_TR_DEBUG, ("hb_fsLock(%p, %lu, %lu, %hu)", hFileHandle, ulStart, ulLength, uiMode));

#if defined(HB_WIN32_IO)
   switch( uiMode & FL_MASK )
   {
      case FL_LOCK:
      {
         if( hb_iswinnt() )
         {
            OVERLAPPED sOlap ;
            DWORD dwFlags ;
            memset( &sOlap, 0, sizeof( OVERLAPPED ) ) ;
            sOlap.Offset = ( ULONG ) ulStart ;
            dwFlags = ( uiMode & FLX_SHARED ) ? 0 : LOCKFILE_EXCLUSIVE_LOCK ;
            if( !s_fUseWaitLocks || !( uiMode & FLX_WAIT ) )
            {
               dwFlags |= LOCKFILE_FAIL_IMMEDIATELY ;
            }
            bResult = LockFileEx( DostoWinHandle( hFileHandle ), dwFlags, 0, ulLength, 0, &sOlap );
         }
         else
         {
             bResult = LockFile( DostoWinHandle( hFileHandle ), ulStart, 0, ulLength,0 );
         }
         break;
      }
      case FL_UNLOCK:
      {
         if( hb_iswinnt() )
         {
            OVERLAPPED sOlap ;
            memset( &sOlap, 0, sizeof( OVERLAPPED ) ) ;
            sOlap.Offset = ( ULONG ) ulStart ;
            bResult = UnlockFileEx( DostoWinHandle( hFileHandle ), 0, ulLength,0, &sOlap );
         }
         else
         {
            bResult = UnlockFile( DostoWinHandle( hFileHandle ), ulStart, 0, ulLength,0 );
         }
         break;

      }
      default:
         bResult = FALSE;
   }
   hb_fsSetIOError( bResult, 0 );
#elif defined(HB_OS_OS2)
   {
      struct _FILELOCK fl, ful;

      switch( uiMode & FL_MASK )
      {
      case FL_LOCK:

         fl.lOffset = ulStart;
         fl.lRange = ulLength;
         ful.lOffset = 0;
         ful.lRange = 0;

         /* lock region, 2 seconds timeout, exclusive access - no atomic */
         bResult = ( DosSetFileLocks( hFileHandle, &ful, &fl, 2000L, 0L ) == 0 );
         break;

      case FL_UNLOCK:

         fl.lOffset = 0;
         fl.lRange = 0;
         ful.lOffset = ulStart;
         ful.lRange = ulLength;

         /* unlock region, 2 seconds timeout, exclusive access - no atomic */
         bResult = ( DosSetFileLocks( hFileHandle, &ful, &fl, 2000L, 0L ) == 0 );
         break;

      default:
         bResult = FALSE;
      }
      hb_fsSetIOError( bResult, 0 );
   }
#elif defined(_MSC_VER) || defined(__DMC__)
   {
      ULONG ulOldPos = lseek( hFileHandle, 0L, SEEK_CUR );

      lseek( hFileHandle, ulStart, SEEK_SET );
      switch( uiMode & FL_MASK )
      {
         case FL_LOCK:
            bResult = ( locking( hFileHandle, _LK_NBLCK, ulLength ) == 0 );
            break;

         case FL_UNLOCK:
            bResult = ( locking( hFileHandle, _LK_UNLCK, ulLength ) == 0 );
            break;

         default:
            bResult = FALSE;
      }
      hb_fsSetIOError( bResult, 0 );
      lseek( hFileHandle, ulOldPos, SEEK_SET );
   }
#elif defined(__MINGW32__)
   {
      ULONG ulOldPos = lseek( hFileHandle, 0L, SEEK_CUR );

      lseek( hFileHandle, ulStart, SEEK_SET );
      switch( uiMode & FL_MASK )
      {
         case FL_LOCK:
            bResult = ( _locking( hFileHandle, _LK_LOCK, ulLength ) == 0 );
            break;

         case FL_UNLOCK:
            bResult = ( _locking( hFileHandle, _LK_UNLCK, ulLength ) == 0 );
            break;

         default:
            bResult = FALSE;
      }
      hb_fsSetIOError( bResult, 0 );
      lseek( hFileHandle, ulOldPos, SEEK_SET );
   }
#elif defined(HB_OS_UNIX)
   {
      /* TODO: check for append locks (SEEK_END)
       */
      struct flock lock_info;

      switch( uiMode & FL_MASK )
      {
         case FL_LOCK:

            lock_info.l_type   = (uiMode & FLX_SHARED) ? F_RDLCK : F_WRLCK;
            lock_info.l_start  = ulStart;
            lock_info.l_len    = ulLength;
            lock_info.l_whence = SEEK_SET;   /* start from the beginning of the file */
            lock_info.l_pid    = getpid();

            bResult = ( fcntl( hFileHandle,
                               (uiMode & FLX_WAIT) ? F_SETLKW: F_SETLK,
                               &lock_info ) >= 0 );
            break;

         case FL_UNLOCK:

            lock_info.l_type   = F_UNLCK;   /* unlock */
            lock_info.l_start  = ulStart;
            lock_info.l_len    = ulLength;
            lock_info.l_whence = SEEK_SET;
            lock_info.l_pid    = getpid();

            bResult = ( fcntl( hFileHandle, F_SETLK, &lock_info ) >= 0 );
            break;

         default:
            bResult = FALSE;
      }
      hb_fsSetIOError( bResult, 0 );
   }
#elif defined(HAVE_POSIX_IO) && !defined(__IBMCPP__) && ( !defined(__GNUC__) || defined(__DJGPP__) )

   switch( uiMode & FL_MASK )
   {
      case FL_LOCK:
         bResult = ( lock( hFileHandle, ulStart, ulLength ) == 0 );
         break;

      case FL_UNLOCK:
         bResult = ( unlock( hFileHandle, ulStart, ulLength ) == 0 );
         break;

      default:
         bResult = FALSE;
   }
   hb_fsSetIOError( bResult, 0 );

#else

   bResult = FALSE;
   hb_fsSetError( (USHORT) FS_ERROR );

#endif

   return bResult;
}

HB_EXPORT BOOL hb_fsLockLarge( FHANDLE hFileHandle, HB_FOFFSET ulStart,
                               HB_FOFFSET ulLength, USHORT uiMode )
{
   BOOL bResult;

   HB_TRACE(HB_TR_DEBUG, ("hb_fsLockLarge(%p, %" PFHL "u, %" PFHL "u, %hu)", hFileHandle, ulStart, ulLength, uiMode));

#if defined(HB_WIN32_IO)
   {
      DWORD dwOffsetLo = ( DWORD ) ( ulStart & 0xFFFFFFFF ),
            dwOffsetHi = ( DWORD ) ( ulStart >> 32 ),
            dwLengthLo = ( DWORD ) ( ulLength & 0xFFFFFFFF ),
            dwLengthHi = ( DWORD ) ( ulLength >> 32 );

      switch( uiMode & FL_MASK )
      {
         case FL_LOCK:
            if( hb_iswinnt() )
            {
               OVERLAPPED sOlap ;
               DWORD dwFlags ;

               dwFlags = ( ( uiMode & FLX_SHARED ) ? 0 : LOCKFILE_EXCLUSIVE_LOCK );
               if( !s_fUseWaitLocks || !( uiMode & FLX_WAIT ) )
               {
                  dwFlags |= LOCKFILE_FAIL_IMMEDIATELY ;
               }

               memset( &sOlap, 0, sizeof( OVERLAPPED ) );
               sOlap.Offset = dwOffsetLo;
               sOlap.OffsetHigh = dwOffsetHi;

               bResult = LockFileEx( DostoWinHandle( hFileHandle ), dwFlags, 0,
                                     dwLengthLo, dwLengthHi, &sOlap );
            }
            else
            {
               bResult = LockFile( DostoWinHandle( hFileHandle ),
                                   dwOffsetLo, dwOffsetHi,
                                   dwLengthLo, dwLengthHi );
            }
            break;

         case FL_UNLOCK:
            if( hb_iswinnt() )
            {
               OVERLAPPED sOlap ;

               memset( &sOlap, 0, sizeof( OVERLAPPED ) );
               sOlap.Offset = dwOffsetLo;
               sOlap.OffsetHigh = dwOffsetHi;

               bResult = UnlockFileEx( DostoWinHandle( hFileHandle ), 0,
                                       dwLengthLo, dwLengthHi, &sOlap );
            }
            else
            {
               bResult = UnlockFile( DostoWinHandle( hFileHandle ),
                                     dwOffsetLo, dwOffsetHi,
                                     dwLengthLo, dwLengthHi );
            }
            break;

         default:
            bResult = FALSE;
      }
      hb_fsSetIOError( bResult, 0 );
   }
#elif defined(HB_OS_LINUX) && defined(__USE_LARGEFILE64)
   /*
    * The macro: __USE_LARGEFILE64 is set when _LARGEFILE64_SOURCE is
    * define and efectively enables lseek64/flock64/ftruncate64 functions
    * on 32bit machines.
    */
   {
      struct flock64 lock_info;

      switch( uiMode & FL_MASK )
      {
         case FL_LOCK:

            lock_info.l_type   = (uiMode & FLX_SHARED) ? F_RDLCK : F_WRLCK;
            lock_info.l_start  = ulStart;
            lock_info.l_len    = ulLength;
            lock_info.l_whence = SEEK_SET;   /* start from the beginning of the file */
            lock_info.l_pid    = getpid();

            bResult = ( fcntl( hFileHandle,
                               (uiMode & FLX_WAIT) ? F_SETLKW64: F_SETLK64,
                               &lock_info ) != -1 );
            break;

         case FL_UNLOCK:

            lock_info.l_type   = F_UNLCK;   /* unlock */
            lock_info.l_start  = ulStart;
            lock_info.l_len    = ulLength;
            lock_info.l_whence = SEEK_SET;
            lock_info.l_pid    = getpid();

            bResult = ( fcntl( hFileHandle, F_SETLK64, &lock_info ) != -1 );
            break;

         default:
            bResult = FALSE;
      }
      hb_fsSetIOError( bResult, 0 );
   }
#else
   bResult = hb_fsLock( hFileHandle, (ULONG) ulStart, (ULONG) ulLength, uiMode );
#endif

   return bResult;
}

HB_EXPORT ULONG hb_fsSeek( FHANDLE hFileHandle, LONG lOffset, USHORT uiFlags )
{
   ULONG ulPos;

   HB_TRACE(HB_TR_DEBUG, ("hb_fsSeek(%p, %ld, %hu)", hFileHandle, lOffset, uiFlags));

#if defined(HB_FS_FILE_IO)
{
   USHORT Flags = convert_seek_flags( uiFlags );

   #if defined(HB_OS_OS2)
   {
      APIRET ret;

      /* This DOS hack creates 2GB file size limit, Druzus */
      if( lOffset < 0 && Flags == SEEK_SET )
      {
         ret = 1;
         hb_fsSetError( 25 ); /* 'Seek Error' */
      }
      else
      {
         ret = DosSetFilePtr( hFileHandle, lOffset, Flags, &ulPos );
         /* TODO: what we should do with this error code? Is it DOS compatible? */
         hb_fsSetError(( USHORT ) ret );
      }
      if( ret != 0 )
      {
         /* FIXME: it should work if DosSetFilePtr is lseek compatible
            but maybe OS2 has DosGetFilePtr too, if not then remove this
            comment, Druzus */
         if ( DosSetFilePtr( hFileHandle, 0, SEEK_CUR, &ulPos ) != 0 )
         {
            ulPos = 0;
         }
      }
   }
   #elif defined(HB_WIN32_IO)
      /* This DOS hack creates 2GB file size limit, Druzus */
      if( lOffset < 0 && Flags == SEEK_SET )
      {
         ulPos = (ULONG) INVALID_SET_FILE_POINTER;
         hb_fsSetError( 25 ); /* 'Seek Error' */
      }
      else
      {
         ulPos = (DWORD) SetFilePointer( DostoWinHandle(hFileHandle), lOffset, NULL, (DWORD)Flags );
         hb_fsSetIOError( (DWORD) ulPos != INVALID_SET_FILE_POINTER, 0 );
      }

      if ( (DWORD) ulPos == INVALID_SET_FILE_POINTER )
      {
         ulPos = (DWORD) SetFilePointer( DostoWinHandle(hFileHandle), 0, NULL, SEEK_CUR );
      }
   #else
      /* This DOS hack creates 2GB file size limit, Druzus */
      if( lOffset < 0 && Flags == SEEK_SET )
      {
         ulPos = (ULONG) -1;
         hb_fsSetError( 25 ); /* 'Seek Error' */
      }
      else
      {
         ulPos = lseek( hFileHandle, lOffset, Flags );
         hb_fsSetIOError( ulPos != (ULONG) -1, 0 );
      }
      if ( ulPos == (ULONG) -1 )
      {
         ulPos = lseek( hFileHandle, 0L, SEEK_CUR );
         if ( ulPos == (ULONG) -1 )
         {
            ulPos = 0;
         }
      }
   #endif
   }
#else
   hb_fsSetError( 25 );
   ulPos = 0;
#endif

   return ulPos;
}

HB_EXPORT HB_FOFFSET hb_fsSeekLarge( FHANDLE hFileHandle, HB_FOFFSET llOffset, USHORT uiFlags )
{
   HB_FOFFSET llPos;

   HB_TRACE(HB_TR_DEBUG, ("hb_fsSeekLarge(%p, %" PFHL "u, %hu)", hFileHandle, llOffset, uiFlags));

#if defined(HB_WIN32_IO)
   {
      USHORT Flags = convert_seek_flags( uiFlags );

      ULONG ulOffsetLow  = ( ULONG ) ( llOffset & ULONG_MAX ),
            ulOffsetHigh = ( ULONG ) ( llOffset >> 32 );

      if( llOffset < 0 && Flags == SEEK_SET )
      {
         llPos = ( HB_FOFFSET ) INVALID_SET_FILE_POINTER;
         hb_fsSetError( 25 ); /* 'Seek Error' */
      }
      else
      {
         ulOffsetLow = SetFilePointer( DostoWinHandle( hFileHandle ),
                                       ulOffsetLow, (PLONG) &ulOffsetHigh,
                                       ( DWORD ) Flags );
         llPos = ( ( HB_FOFFSET ) ulOffsetHigh << 32 ) | ulOffsetLow;
         hb_fsSetIOError( llPos != ( HB_FOFFSET ) INVALID_SET_FILE_POINTER, 0 );
      }

      if ( llPos == ( HB_FOFFSET ) INVALID_SET_FILE_POINTER )
      {
         ulOffsetHigh = 0;
         ulOffsetLow = SetFilePointer( DostoWinHandle( hFileHandle ),
                                       0, (PLONG) &ulOffsetHigh, SEEK_CUR );
         llPos = ( ( HB_FOFFSET ) ulOffsetHigh << 32 ) | ulOffsetLow;
      }
   }
#elif defined(HB_OS_LINUX) && defined(__USE_LARGEFILE64)
   /*
    * The macro: __USE_LARGEFILE64 is set when _LARGEFILE64_SOURCE is
    * define and efectively enables lseek64/flock64/ftruncate64 functions
    * on 32bit machines.
    */
   {
      USHORT Flags = convert_seek_flags( uiFlags );

      if( llOffset < 0 && Flags == SEEK_SET )
      {
         llPos = (HB_FOFFSET) -1;
         hb_fsSetError( 25 ); /* 'Seek Error' */
      }
      else
      {
         llPos = lseek64( hFileHandle, llOffset, Flags );
         hb_fsSetIOError( llPos != (HB_FOFFSET) -1, 0 );
      }

      if ( llPos == (HB_FOFFSET) -1 )
      {
         llPos = lseek64( hFileHandle, 0L, SEEK_CUR );
      }
   }
#else
   llPos = (HB_FOFFSET) hb_fsSeek( hFileHandle, (LONG) llOffset, uiFlags );
#endif

   return llPos;
}

HB_EXPORT ULONG   hb_fsTell( FHANDLE hFileHandle )
{
   ULONG ulPos;

   HB_TRACE(HB_TR_DEBUG, ("hb_fsTell(%p)", hFileHandle));

#if defined(HB_FS_FILE_IO)

   #if defined(HB_WIN32_IO)
      ulPos = (DWORD) SetFilePointer( DostoWinHandle(hFileHandle), 0, NULL, FILE_CURRENT );
      hb_fsSetIOError( (DWORD) ulPos != INVALID_SET_FILE_POINTER, 0 );
   #else
      ulPos = lseek( hFileHandle, 0L, SEEK_CUR );
      hb_fsSetIOError( ulPos != (ULONG) -1, 0 );
   #endif

#else

   ulPos = (ULONG) -1;
   hb_fsSetError( (USHORT) FS_ERROR );

#endif

   return ulPos;
}

HB_EXPORT BOOL hb_fsDelete( BYTE * pFilename )
{
   BOOL bResult;

   HB_TRACE(HB_TR_DEBUG, ("hb_fsDelete(%s)", (char*) pFilename));

   pFilename = hb_fileNameConv( hb_strdup( ( char * ) pFilename ) );

#if defined(HB_OS_WIN_32)

   bResult = DeleteFile( ( char * ) pFilename );
   hb_fsSetIOError( bResult, 0 );

#elif defined(HAVE_POSIX_IO)

   bResult = ( remove( ( char * ) pFilename ) == 0 );
   hb_fsSetIOError( bResult, 0 );

#elif defined(_MSC_VER) || defined(__MINGW32__) || defined(__DMC__)

   bResult = ( remove( ( char * ) pFilename ) == 0 );
   hb_fsSetIOError( bResult, 0 );

#else

   bResult = FALSE;
   hb_fsSetError( (USHORT) FS_ERROR );

#endif

   hb_xfree( pFilename ) ;

   return bResult;
}

HB_EXPORT BOOL hb_fsRename( BYTE * pOldName, BYTE * pNewName )
{
   BOOL bResult;

   HB_TRACE(HB_TR_DEBUG, ("hb_fsRename(%s, %s)", (char*) pOldName, (char*) pNewName));

   pOldName = hb_fileNameConv( hb_strdup( ( char * ) pOldName ) );
   pNewName = hb_fileNameConv( hb_strdup( ( char * ) pNewName ) );

#if defined(HB_OS_WIN_32)

   bResult = MoveFile( ( char * ) pOldName, ( char * ) pNewName );
   hb_fsSetIOError( bResult, 0 );

#elif defined(HB_FS_FILE_IO)

   bResult = ( rename( ( char * ) pOldName, ( char * ) pNewName ) == 0 );
   hb_fsSetIOError( bResult, 0 );

#else

   bResult = FALSE;
   hb_fsSetError( (USHORT) FS_ERROR );

#endif

   hb_xfree( pOldName ) ;
   hb_xfree( pNewName ) ;

   return bResult;
}

HB_EXPORT BOOL hb_fsMkDir( BYTE * pDirname )
{
   BOOL bResult;

   HB_TRACE(HB_TR_DEBUG, ("hb_fsMkDir(%s)", (char*) pDirname));

   pDirname = hb_fileNameConv( hb_strdup( ( char * ) pDirname ) );

   HB_TRACE(HB_TR_DEBUG, ("hb_fsMkDir(%s)", (char*) pDirname));

#if defined(HB_OS_WIN_32)

   bResult = CreateDirectory( ( char * ) pDirname, NULL );
   hb_fsSetIOError( bResult, 0 );

#elif defined(HAVE_POSIX_IO) || defined(__MINGW32__)

#  if ! defined(HB_OS_UNIX) && \
      ( defined(__WATCOMC__) || defined(__BORLANDC__) || \
        defined(__IBMCPP__) || defined(__MINGW32__) )
      bResult = ( mkdir( ( char * ) pDirname ) == 0 );
#  else
      bResult = ( mkdir( ( char * ) pDirname, S_IRWXU | S_IRWXG | S_IRWXO ) == 0 );
#  endif
   hb_fsSetIOError( bResult, 0 );

#else

   bResult = FALSE;
   hb_fsSetError( (USHORT) FS_ERROR );

#endif

   hb_xfree(pDirname) ;

   return bResult;
}

HB_EXPORT BOOL hb_fsChDir( BYTE * pDirname )
{
   BOOL bResult;

   HB_TRACE(HB_TR_DEBUG, ("hb_fsChDir(%s)", (char*) pDirname));

   pDirname = hb_fileNameConv( hb_strdup( ( char * ) pDirname ) );

#if defined(HB_OS_WIN_32)

   bResult = SetCurrentDirectory( ( char * ) pDirname );
   hb_fsSetIOError( bResult, 0 );

#elif defined(HAVE_POSIX_IO) || defined(__MINGW32__)

   bResult = ( chdir( ( char * ) pDirname ) == 0 );
   hb_fsSetIOError( bResult, 0 );

#else

   bResult = FALSE;
   hb_fsSetError( (USHORT) FS_ERROR );

#endif

   hb_xfree( pDirname );

   return bResult;
}

HB_EXPORT BOOL hb_fsRmDir( BYTE * pDirname )
{
   BOOL bResult;

   HB_TRACE(HB_TR_DEBUG, ("hb_fsRmDir(%s)", (char*) pDirname));

   pDirname = hb_fileNameConv( hb_strdup( ( char * ) pDirname ) );

#if defined(HB_OS_WIN_32)

   bResult = RemoveDirectory( ( char * ) pDirname );
   hb_fsSetIOError( bResult, 0 );

#elif defined(HAVE_POSIX_IO) || defined(__MINGW32__)

   bResult = ( rmdir( ( char * ) pDirname ) == 0 );
   hb_fsSetIOError( bResult, 0 );

#else

   bResult = FALSE;
   hb_fsSetError( (USHORT) FS_ERROR );

#endif

   hb_xfree( pDirname );

   return bResult;
}

/* NOTE: This is not thread safe function, it's there for compatibility. */
/* NOTE: 0 = current drive, 1 = A, 2 = B, 3 = C, etc. */

HB_EXPORT BYTE *  hb_fsCurDir( USHORT uiDrive )
{
   static BYTE s_byDirBuffer[ _POSIX_PATH_MAX + 1 ];

   HB_TRACE(HB_TR_DEBUG, ("hb_fsCurDir(%hu)", uiDrive));

   hb_fsCurDirBuff( uiDrive, s_byDirBuffer, _POSIX_PATH_MAX + 1 );

   return ( BYTE * ) s_byDirBuffer;
}

/* NOTE: Thread safe version of hb_fsCurDir() */
/* NOTE: 0 = current drive, 1 = A, 2 = B, 3 = C, etc. */

HB_EXPORT USHORT  hb_fsCurDirBuff( USHORT uiDrive, BYTE * pbyBuffer, ULONG ulLen )
{
   USHORT uiCurDrv = uiDrive, usError;
   BOOL fResult;

   HB_TRACE(HB_TR_DEBUG, ("hb_fsCurDirBuff(%hu)", uiDrive));

   pbyBuffer[ 0 ] = '\0';

   /*
    * do not cover this code by OS_HAS_DRIVE_LETTER macro
    * It will allow us to add drive emulation in hb_fsCurDrv()/hb_fsChDrv()
    * and hb_fileNameConv()
    */
#if !defined(HB_OS_OS2)
   if( uiDrive )
   {
      uiCurDrv = hb_fsCurDrv() + 1;
      if( uiDrive != uiCurDrv )
      {
         hb_fsChDrv( uiDrive - 1 );
      }
   }
#endif

#if defined(HB_OS_WIN_32)

   fResult = GetCurrentDirectory( ulLen, ( char * ) pbyBuffer );
   hb_fsSetIOError( fResult, 0 );

#elif defined(HB_OS_OS2)

   fResult = ( _getcwd1( (char *) pbyBuffer, uiDrive + 'A' - 1 ) == 0 );
   hb_fsSetIOError( fResult, 0 );

#elif defined(HAVE_POSIX_IO)

   fResult = ( getcwd( ( char * ) pbyBuffer, ulLen ) != NULL );
   hb_fsSetIOError( fResult, 0 );

#elif defined(__MINGW32__)

   fResult = ( _getdcwd( uiDrive, pbyBuffer, ulLen ) != NULL );
   hb_fsSetIOError( fResult, 0 );

#else

   fResult = FALSE;
   hb_fsSetError( (USHORT) FS_ERROR );

#endif

   usError = hb_fsError();

   if( uiDrive != uiCurDrv )
   {
      hb_fsChDrv( uiCurDrv - 1 );
      hb_fsSetError( usError );
   }

   if( usError == 0 && pbyBuffer[ 0 ] )
   {
      BYTE * pbyStart = pbyBuffer;

      /* Strip the leading drive spec, and leading backslash if there's one. */
      /* NOTE: A trailing underscore is not returned on this platform,
               so we don't need to strip it. [vszakats] */

      ulLen = strlen( ( char * ) pbyBuffer );

#if defined(OS_HAS_DRIVE_LETTER)
      if( pbyStart[ 1 ] == OS_DRIVE_DELIMITER )
      {
         pbyStart += 2;
         ulLen -= 2;
      }
#endif
      if( strchr( OS_PATH_DELIMITER_LIST, pbyStart[ 0 ] ) )
      {
         pbyStart++;
         ulLen--;
      }

      /* Strip the trailing (back)slash if there's one */
      if( ulLen && strchr( OS_PATH_DELIMITER_LIST, pbyStart[ ulLen - 1 ] ) )
         ulLen--;

      if( ulLen && pbyBuffer != pbyStart )
         memmove( pbyBuffer, pbyStart, ulLen );

      pbyBuffer[ ulLen ] = '\0';
   }

   return usError;
}

/* NOTE: 0=A:, 1=B:, 2=C:, 3=D:, ... */

HB_EXPORT USHORT hb_fsChDrv( BYTE nDrive )
{
   USHORT uiResult;

   HB_TRACE(HB_TR_DEBUG, ("hb_fsChDrv(%d)", (int) nDrive));

#if defined(OS_HAS_DRIVE_LETTER)
   {
      /* 'unsigned int' _have to_ be used in Watcom */
      UINT uiSave, uiNewDrive;

      HB_FS_GETDRIVE( uiSave );
      HB_FS_SETDRIVE( nDrive );
      HB_FS_GETDRIVE( uiNewDrive );

      if( ( UINT ) nDrive == uiNewDrive )
      {
         uiResult = 0;
         hb_fsSetError( 0 );
      }
      else
      {
         HB_FS_SETDRIVE( uiSave );

         uiResult = (USHORT) FS_ERROR;
         hb_fsSetError( (USHORT) FS_ERROR );
      }
    }
#else

   HB_SYMBOL_UNUSED( nDrive );
   uiResult = ( USHORT ) FS_ERROR;
   hb_fsSetError( ( USHORT ) FS_ERROR );

#endif

   return uiResult;
}

/* NOTE: 0=A:, 1=B:, 2=C:, 3=D:, ... */

/* TOFIX: This isn't fully compliant because CA-Cl*pper doesn't access
          the drive before checking. hb_fsIsDrv only returns TRUE
          if there is a disk in the drive. */

HB_EXPORT USHORT hb_fsIsDrv( BYTE nDrive )
{
   USHORT uiResult;

   HB_TRACE(HB_TR_DEBUG, ("hb_fsIsDrv(%d)", (int) nDrive));

#if defined(OS_HAS_DRIVE_LETTER)
   {
      /* 'unsigned int' _have to_ be used in Watcom
       */
      UINT uiSave, uiNewDrive;

      HB_FS_GETDRIVE( uiSave );
      HB_FS_SETDRIVE( nDrive );
      HB_FS_GETDRIVE( uiNewDrive );
      if( ( UINT ) nDrive != uiNewDrive )
      {
         uiResult = (USHORT) FS_ERROR;
         hb_fsSetError( (USHORT) FS_ERROR );
      }
      else
      {
         uiResult = 0;
         hb_fsSetError( 0 );
      }
      HB_FS_SETDRIVE( uiSave );
   }

#else

   HB_SYMBOL_UNUSED( nDrive );
   uiResult = ( USHORT ) FS_ERROR;
   hb_fsSetError( ( USHORT ) FS_ERROR );

#endif

   return uiResult;
}

HB_EXPORT  BOOL hb_fsIsDevice( FHANDLE hFileHandle )
{
   BOOL bResult;

   HB_TRACE(HB_TR_DEBUG, ("hb_fsIsDevice(%p)", hFileHandle));

#if defined(HB_OS_WIN_32)

   bResult = GetFileType( DostoWinHandle( hFileHandle ) ) == FILE_TYPE_CHAR;
   hb_fsSetIOError( bResult, 0 );

#elif defined(HB_FS_FILE_IO)

#if defined( _MSC_VER ) || defined( __MINGW32__ )
   bResult = _isatty( hFileHandle ) != 0;
#else
   bResult = isatty( hFileHandle ) != 0;
#endif
   hb_fsSetIOError( bResult, 0 );

#else

   bResult = FALSE;
   hb_fsSetError( (USHORT) FS_ERROR );
   HB_SYMBOL_UNUSED( hFileHandle );

#endif

   return bResult;
}

/* NOTE: 0=A:, 1=B:, 2=C:, 3=D:, ... */

HB_EXPORT  BYTE hb_fsCurDrv( void )
{
   UINT uiResult;

   HB_TRACE(HB_TR_DEBUG, ("hb_fsCurDrv()"));

#if defined(OS_HAS_DRIVE_LETTER)

   HB_FS_GETDRIVE( uiResult );

#else

   uiResult = 0;
   hb_fsSetError( ( USHORT ) FS_ERROR );

#endif

   return ( BYTE ) uiResult; /* Return the drive number, base 0. */
}

/* copied from xHarbour */
HB_EXPORT  FHANDLE hb_fsExtOpen( BYTE * pFilename, BYTE * pDefExt,
                                 USHORT uiExFlags, BYTE * pPaths,
                                 PHB_ITEM pError )
{
   HB_PATHNAMES *pSearchPath = NULL, *pNextPath;
   PHB_FNAME pFilepath;
   FHANDLE hFile;
   BOOL fIsFile = FALSE;
   BYTE * szPath;
   USHORT uiFlags;

   HB_TRACE(HB_TR_DEBUG, ("hb_fsExtOpen(%s, %s, %hu, %p, %p)", pFilename, pDefExt, uiExFlags, pPaths, pError));

/*
   #define FXO_TRUNCATE  0x0100   // Create (truncate if exists)
   #define FXO_APPEND    0x0200   // Create (append if exists)
   #define FXO_UNIQUE    0x0400   // Create unique file FO_EXCL ???
   #define FXO_FORCEEXT  0x0800   // Force default extension
   #define FXO_DEFAULTS  0x1000   // Use SET command defaults
   #define FXO_DEVICERAW 0x2000   // Open devices in raw mode
   // xHarbour extension
   #define FXO_SHARELOCK 0x4000   // emulate DOS SH_DENY* mode in POSIX OS
   #define FXO_COPYNAME  0x8000   // copy final szPath into pFilename

   hb_errGetFileName( pError );
*/

   szPath = (BYTE *) hb_xgrab( _POSIX_PATH_MAX + 1 );

   uiFlags = uiExFlags & 0xff;
   if( uiExFlags & ( FXO_TRUNCATE | FXO_APPEND | FXO_UNIQUE ) )
   {
      uiFlags |= FO_CREAT;
      if( uiExFlags & FXO_UNIQUE )
         uiFlags |= FO_EXCL;
#if !defined( HB_USE_SHARELOCKS )
      else if( uiExFlags & FXO_TRUNCATE )
         uiFlags |= FO_TRUNC;
#endif
   }

   pFilepath = hb_fsFNameSplit( ( char * ) pFilename );

   if( pDefExt && ( ( uiExFlags & FXO_FORCEEXT ) || !pFilepath->szExtension ) )
   {
      pFilepath->szExtension = ( char * ) pDefExt;
   }

   if( pFilepath->szPath )
   {
      hb_fsFNameMerge( ( char * ) szPath, pFilepath );
   }
   else if( uiExFlags & FXO_DEFAULTS )
   {
      if( hb_set.HB_SET_DEFAULT )
      {
         pFilepath->szPath = hb_set.HB_SET_DEFAULT;
         hb_fsFNameMerge( ( char * ) szPath, pFilepath );
         fIsFile = hb_fsFile( szPath );
      }
      if( !fIsFile && hb_set.HB_SET_PATH )
      {
         pNextPath = hb_setGetFirstSetPath();
         while( !fIsFile && pNextPath )
         {
            pFilepath->szPath = pNextPath->szPath;
            hb_fsFNameMerge( ( char * ) szPath, pFilepath );
            fIsFile = hb_fsFile( szPath );
            pNextPath = pNextPath->pNext;
         }
      }
      if( !fIsFile )
      {
         pFilepath->szPath = hb_set.HB_SET_DEFAULT ? hb_set.HB_SET_DEFAULT : NULL;
         hb_fsFNameMerge( ( char * ) szPath, pFilepath );
      }
   }
   else if( pPaths && *pPaths )
   {
      hb_fsAddSearchPath( ( char * ) pPaths, &pSearchPath );
      pNextPath = pSearchPath;
      while( !fIsFile && pNextPath )
      {
         pFilepath->szPath = pNextPath->szPath;
         hb_fsFNameMerge( ( char * ) szPath, pFilepath );
         fIsFile = hb_fsFile( szPath );
         pNextPath = pNextPath->pNext;
      }
      hb_fsFreeSearchPath( pSearchPath );
      if( !fIsFile )
      {
         pFilepath->szPath = NULL;
         hb_fsFNameMerge( ( char * ) szPath, pFilepath );
      }
   }
   else
   {
      hb_fsFNameMerge( ( char * ) szPath, pFilepath );
   }
   hb_xfree( pFilepath );

   hFile = hb_fsOpen( szPath, uiFlags );

#if defined( HB_USE_SHARELOCKS )
   if( hFile != FS_ERROR && uiExFlags & FXO_SHARELOCK )
   {
#if defined( HB_USE_BSDLOCKS )
      int iLock;
      if( ( uiFlags & ( FO_READ | FO_WRITE | FO_READWRITE ) ) == FO_READ ||
          ( uiFlags & ( FO_DENYREAD | FO_DENYWRITE | FO_EXCLUSIVE ) ) == 0 )
         iLock = LOCK_SH | LOCK_NB;
      else
         iLock = LOCK_EX | LOCK_NB;
      if( flock( hFile, iLock ) != 0 )
#else
      USHORT uiLock;
      if( ( uiFlags & ( FO_READ | FO_WRITE | FO_READWRITE ) ) == FO_READ ||
          ( uiFlags & ( FO_DENYREAD | FO_DENYWRITE | FO_EXCLUSIVE ) ) == 0 )
         uiLock = FL_LOCK | FLX_SHARED;
      else
         uiLock = FL_LOCK | FLX_EXCLUSIVE;

      if( !hb_fsLockLarge( hFile, HB_SHARELOCK_POS, HB_SHARELOCK_SIZE, uiLock ) )
#endif
      {
         hb_fsClose( hFile );
         hFile = FS_ERROR;
         /*
          * fix for neterr() support and Clipper compatibility,
          * should be revised with a better multi platform solution.
          */
         hb_fsSetError( ( uiExFlags & FXO_TRUNCATE ) ? 5 : 32 );
      }
      else if( uiExFlags & FXO_TRUNCATE )
      {
         /* truncate the file only if properly locked */
         hb_fsSeek( hFile, 0, FS_SET );
         hb_fsWrite( hFile, NULL, 0 );
         if( hb_fsError() != 0 )
         {
            hb_fsClose( hFile );
            hFile = FS_ERROR;
            hb_fsSetError( 5 );
         }
      }
   }
#elif 1
   /*
    * Temporary fix for neterr() support and Clipper compatibility,
    * should be revised with a better solution.
    */
   if( ( uiExFlags & ( FXO_TRUNCATE | FXO_APPEND | FXO_UNIQUE ) ) == 0 &&
       hb_fsError() == 5 )
   {
      hb_fsSetError( 32 );
   }
#endif

   if( pError )
   {
      hb_errPutFileName( pError, ( char * ) szPath );
      if( hFile == FS_ERROR )
      {
         hb_errPutOsCode( pError, hb_fsError() );
         hb_errPutGenCode( pError, ( uiExFlags & FXO_TRUNCATE ) ? EG_CREATE : EG_OPEN );
      }
   }

   if( uiExFlags & FXO_COPYNAME && hFile != FS_ERROR )
      hb_strncpy( ( char * ) pFilename, ( char * ) szPath, _POSIX_PATH_MAX );

   hb_xfree( szPath );
   return hFile;
}

HB_EXPORT BOOL hb_fsEof( FHANDLE hFileHandle )
{
#if defined(__DJGPP__) || defined(__CYGWIN__) || defined(OS_UNIX_COMPATIBLE)
   LONG curPos;
   LONG endPos;
   LONG newPos;
   BOOL fResult = FALSE;

   curPos = lseek( hFileHandle, 0L, SEEK_CUR );
   if ( curPos != -1 )
   {
      endPos = lseek( hFileHandle, 0L, SEEK_END );
      newPos = lseek( hFileHandle, curPos, SEEK_SET );
      fResult = ( endPos != -1 && newPos == curPos );
   }
   else
   {
      endPos = -1;
   }
   hb_fsSetIOError( fResult, 0 );

   return ( !fResult || curPos == endPos );
#else
   return eof( hFileHandle ) != 0;
#endif
}

HB_EXPORT BYTE * hb_fileNameConv( char *str ) {
/*
   Convert file and dir case. The allowed SET options are:
   LOWER - Convert all caracters of file to lower
   UPPER - Convert all caracters of file to upper
   MIXED - Leave as is

   The allowed environment options are:
   FILECASE - define the case of file
   DIRCASE - define the case of path
   DIRSEPARATOR - define separator of path (Ex. "/")
*/
   char *filename;
   ULONG ulDirLen, ulFileLen;

#ifdef __XHARBOUR__
   if( hb_set.HB_SET_TRIMFILENAME )
   {
      char *szFileTrim;
      ULONG ulLen;

      ulLen = hb_strRTrimLen( str, strlen( str ), FALSE );
      szFileTrim = hb_strLTrim( str, &ulLen );
      if ( str != szFileTrim )
      {
         memmove( str, szFileTrim, ulLen );
      }
      str[ulLen] = '\0';
   }
#endif

   if( hb_set.HB_SET_DIRSEPARATOR != '\\' )
   {
      char *p = str;
      while( *p )
      {
         if( *p == '\\' )
         {
            *p = hb_set.HB_SET_DIRSEPARATOR;
         }
         p++;
      }
   }

   /* Look for filename (Last "\" or DIRSEPARATOR) */
   if( ( filename = strrchr( str, hb_set.HB_SET_DIRSEPARATOR ) ) != NULL )
   {
      filename++;
   }
   else
   {
      filename = str;
   }
   ulFileLen = strlen( filename );
   ulDirLen = filename - str;

   /* FILECASE */
   if( ulFileLen > 0 )
   {
      if( hb_set.HB_SET_FILECASE == HB_SET_CASE_LOWER )
         hb_strLower( filename, strlen(filename) );
      else if( hb_set.HB_SET_FILECASE == HB_SET_CASE_UPPER )
         hb_strUpper( filename, strlen(filename) );
   }

   /* DIRCASE */
   if( ulDirLen > 0 )
   {
      if( hb_set.HB_SET_DIRCASE == HB_SET_CASE_LOWER )
         hb_strLower( str, ulDirLen );
      else if( hb_set.HB_SET_DIRCASE == HB_SET_CASE_UPPER )
         hb_strUpper( str, ulDirLen );
   }
   return ( BYTE * ) str;
}

HB_EXPORT BOOL hb_fsDisableWaitLocks( int iSet )
{
   BOOL fRetVal = s_fUseWaitLocks;

   if ( iSet >= 0 )
   {
      s_fUseWaitLocks = ( iSet == 0 );
   }
   return fRetVal;
}
