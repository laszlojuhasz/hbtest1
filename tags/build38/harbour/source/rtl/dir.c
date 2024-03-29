/*
 * $Id$
 */

/*
 * Harbour Project source code:
 * DIRECTORY() function
 *
 * Copyright 1999 Leslee Griffith <les.griffith@vantagesystems.ca>
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

/* TOFIX: Filename/path splitting and merging should be rewritten to use
          hb_FName*() functions, instead of repeating the functionality
          in the current partially buggy way. [vszakats] */

/*
 * Notes from the fringe... <ptucker@sympatico.ca>
 *
 * Clipper is a bit schizoid with the treatment of file attributes, but we've
 * emulated that weirdness here for your viewing amusement.
 *
 * In Clippers' homeworld of DOS, there are essentially 5 basic attributes:
 * 'A'rchive, 'H'idden, 'S'ystem, 'R'eadonly and 'D'irectory.  In addition, a
 * file can have no attributes, and only 1 file can have the 'V'olume label.
 *
 * For a given file request, you will receive any files that match the
 * passed filemask.  Included in this list are files which have attributes
 * matching the requested attribute as well as files that have no attribute,
 * or that have the 'A'rchive, or 'R'eadOnly attribute.
 *
 * The exception is Directory entries - these will always be excluded
 * even if they have the requested bit set. (Unless of course, you request "D"
 * as an attribute as well)
 *
 * The only valid characters that can be passed as an attribute request are
 * any of "DHS". Anything else is already implied, so it is ignored. Except
 * under NT, which may accept other attributes, but it is still a work in
 * progress - NT that is ;-).
 *
 * "V" is also valid, but is a special case - you will get back 1 entry only
 * that describes the volume label for the drive implied by the filemask.
 *
 * Differences from the 'standard':
 * Where supported, filenames will be returned in the same case as they
 * are stored in the directory.  Clipper (and VO too) will convert the
 * names to upper case.
 * Where supported, filenames will be the full filename as supported by
 * the os in use.  Under an MS Windows implimentation, an optional
 * 3rd parameter to Directory will allow you to receive the normal '8.3'
 * filename.
 *
 * TODO: - Volume label support
 *       - check that path support vis stat works on all platforms
 *       - UNC Support? ie: dir \\myserver\root
 *       - Use hb_fsFNameSplit()/Merge() for filename composing/decomposing.
 *
 */

/* NOTE: For OS/2. Must be ahead of any and all #include statements */
#define INCL_DOSFILEMGR
#define INCL_DOSERRORS

#define HB_OS_WIN_32_USED

#include <ctype.h>

#include "hbapi.h"
#include "hbapiitm.h"
#include "hb_io.h"
#include "directry.ch"

#if defined(__GNUC__) && !defined(__MINGW32__)
   #include <sys/types.h>
   #include <sys/stat.h>
   #include <fcntl.h>
   #include <errno.h>
   #include <dirent.h>
   #include <time.h>

   #if !defined(HAVE_POSIX_IO)
      #define HAVE_POSIX_IO
   #endif
#endif

#if defined(__WATCOMC__) || defined(__MINGW32__) || ( defined(_MSC_VER) && _MSC_VER >= 1000 )
   #include <sys/stat.h>
   #include <share.h>
   #include <fcntl.h>
   #include <errno.h>
   #include <direct.h>
   #include <time.h>

   #if !defined(HAVE_POSIX_IO)
      #define HAVE_POSIX_IO
   #endif
#elif defined(_MSC_VER)
   #include <sys/stat.h>
   #include <share.h>
   #include <fcntl.h>
   #include <errno.h>
   #include <direct.h>
   #include <time.h>
   #include <dos.h>

   #if !defined(HAVE_POSIX_IO)
      #define HAVE_POSIX_IO
   #endif
#endif

#if defined(HB_OS_OS2)
   #include <sys/stat.h>
   #include <time.h>

   #if !defined(HAVE_POSIX_IO)
      #define HAVE_POSIX_IO
   #endif
#endif

#if defined(__BORLANDC__)
   #include <sys\stat.h>
   #include <fcntl.h>
   #include <share.h>
   #include <dirent.h>
   #include <dir.h>
   #include <dos.h>
   #include <time.h>
   #include <errno.h>

   #if !defined(HAVE_POSIX_IO)
      #define HAVE_POSIX_IO
      #ifndef S_IEXEC
         #define S_IEXEC  0x0040 /* owner may execute <directory search> */
      #endif
      #ifndef S_IRWXU
         #define S_IRWXU  0x01c0 /* RWE permissions mask for owner */
      #endif
      #ifndef S_IRUSR
         #define S_IRUSR  0x0100 /* owner may read */
      #endif
      #ifndef S_IWUSR
         #define S_IWUSR  0x0080 /* owner may write */
      #endif
      #ifndef S_IXUSR
         #define S_IXUSR  0x0040 /* owner may execute <directory search> */
      #endif
   #endif
#endif

#if !defined(FA_RDONLY)
   #define FA_RDONLY           1   /* R */
   #define FA_HIDDEN           2   /* H */
   #define FA_SYSTEM           4   /* S */
   #define FA_LABEL            8   /* V */
   #define FA_DIREC           16   /* D */
   #define FA_ARCH            32   /* A */
#endif
/* these work under NT but are otherwise used as placeholders for
   non MS o/s support */
#if !defined(FA_DEVICE)
   #define FA_DEVICE          64   /* I */
/*   #define FA_NORMAL         128 */  /* N */  /* ignored */ /* Exists in BORLANDC */
   #define FA_TEMPORARY      256   /* T */
   #define FA_SPARSE         512   /* P */
   #define FA_REPARSE       1024   /* L */
   #define FA_COMPRESSED    2048   /* C */
   #define FA_OFFLINE       4096   /* O */
   #define FA_NOTINDEXED    8192   /* X */
   #define FA_ENCRYPTED    16384   /* E */
   #define FA_VOLCOMP      32768   /* M */
#endif

/* Conversion functions  *** commented out functions not finished-not needed */
static USHORT osToHarbourMask( USHORT usMask )
{
   USHORT usRetMask;

   HB_TRACE(HB_TR_DEBUG, ("osToHarbourMask(%hu)", usMask));

   usRetMask = usMask;

   /* probably access denied when requesting mode */
   if( usMask == (USHORT) -1 )
      return 0;

#if defined(OS_UNIX_COMPATIBLE)
   /* The use of any particular FA_ define here is meaningless */
   /* they are essentially placeholders */
   usRetMask = 0;
   if( S_ISREG( usMask ) )
      usRetMask |= FA_ARCH;        /* A */
   if( S_ISDIR( usMask ) )
      usRetMask |= FA_DIREC;       /* D */
   if( S_ISLNK( usMask ) )
      usRetMask |= FA_REPARSE;     /* L */
   if( S_ISCHR( usMask ) )
      usRetMask |= FA_COMPRESSED;  /* C */
   if( S_ISBLK( usMask ) )
      usRetMask |= FA_DEVICE;      /* B  (I) */
   if( S_ISFIFO( usMask ) )
      usRetMask |= FA_TEMPORARY;   /* F  (T) */
   if( S_ISSOCK( usMask ) )
      usRetMask |= FA_SPARSE;      /* K  (P) */
#elif defined(HB_OS_OS2)
   usRetMask = 0;
   if( usMask & FILE_ARCHIVED )
      usRetMask |= FA_ARCH;
   if( usMask & FILE_DIRECTORY )
      usRetMask |= FA_DIREC;
   if( usMask & FILE_HIDDEN )
      usRetMask |= FA_HIDDEN;
   if( usMask & FILE_READONLY )
      usRetMask |= FA_RDONLY;
   if( usMask & FILE_SYSTEM )
      usRetMask |= FA_SYSTEM;
#endif

   return usRetMask;
}

static USHORT HarbourAttributesToMask( BYTE * byAttrib )
{
   BYTE * pos = byAttrib;
   BYTE c;
   USHORT usMask = 0;

   HB_TRACE(HB_TR_DEBUG, ("HarbourAttributesToMask(%p)", byAttrib));

   while( ( c = toupper( *pos ) ) != '\0' )
   {
      switch( c )
      {
         case 'A': usMask |= FA_ARCH;       break;
         case 'D': usMask |= FA_DIREC;      break;
         case 'H': usMask |= FA_HIDDEN;     break;
         case 'R': usMask |= FA_RDONLY;     break;
         case 'S': usMask |= FA_SYSTEM;     break;
         case 'V': usMask |= FA_LABEL;      break;
         /* extensions */
/*       case 'N': usMask |= FA_NORMAL;     break; */
         case 'O': usMask |= FA_OFFLINE;    break;
         case 'T': usMask |= FA_TEMPORARY;  break;
         case 'I': usMask |= FA_DEVICE;     break;
         case 'M': usMask |= FA_VOLCOMP;    break;
         case 'E': usMask |= FA_ENCRYPTED;  break;
         case 'X': usMask |= FA_NOTINDEXED; break;
         case 'C': usMask |= FA_COMPRESSED; break;
         case 'L': usMask |= FA_REPARSE;    break;
         case 'P': usMask |= FA_SPARSE;     break;
      }

      pos++;
   }

   return usMask;
}

static BYTE * HarbourMaskToAttributes( USHORT usMask, BYTE * byAttrib )
{
   char * cAttrib = ( char * ) byAttrib;

   HB_TRACE(HB_TR_DEBUG, ("HarbourMaskToAttributes(%hu, %p)", usMask, byAttrib));

   *cAttrib = '\0';
   if( usMask & FA_RDONLY )
      strcat( cAttrib, "R" );
   if( usMask & FA_HIDDEN )
      strcat( cAttrib, "H" );
   if( usMask & FA_SYSTEM )
      strcat( cAttrib, "S" );
   if( usMask & FA_DIREC )
      strcat( cAttrib, "D" );
   if( usMask & FA_ARCH )
      strcat( cAttrib, "A" );
   if( usMask & FA_LABEL )
   {
      strcat( cAttrib, "V" );
      if( usMask & FA_VOLCOMP )
         strcat( cAttrib, "M" );  /* volume supports compression. */
   }
/* thse can be returned under NT with NTFS - I picked the letters to use.*/
/* needs testing on a Novell drive */
/* PLEASE! If these cause you trouble let me know! <ptucker@sympatico.ca> */

   if( usMask & FA_DEVICE )
      strcat( cAttrib, "I" );
/* if( usMask & FA_NORMAL )      */
/*    strcat( cAttrib, "N" );   */
   if( usMask & FA_TEMPORARY )
      strcat( cAttrib, "T" );
   if( usMask & FA_SPARSE )
      strcat( cAttrib, "P" );
   if( usMask & FA_REPARSE )
      strcat( cAttrib, "L" );
   if( usMask & FA_COMPRESSED )
      strcat( cAttrib, "C" );
   if( usMask & FA_OFFLINE )
      strcat( cAttrib, "O" );
   if( usMask & FA_NOTINDEXED )
      strcat( cAttrib, "X" );
   if( usMask & FA_ENCRYPTED )
      strcat( cAttrib, "E" );

   return byAttrib;
}

/* NOTE: The third (lEightDotThree) parameter is a Harbour extension. */

HB_FUNC( DIRECTORY )
{
#if defined(HAVE_POSIX_IO)

   PHB_ITEM pDirSpec = hb_param( 1, HB_IT_STRING );
   PHB_ITEM pAttributes = hb_param( 2, HB_IT_STRING );

#if defined(__MINGW32__) || ( defined(_MSC_VER) && _MSC_VER >= 1000 )
   PHB_ITEM pEightDotThree = hb_param( 3, HB_IT_LOGICAL );
   BOOL     bEightDotThree;
#elif defined(__WATCOMC__)
   int      iDirnameLen;
#endif

   char     fullfile[ _POSIX_PATH_MAX + 1 ];
   char     filename[ _POSIX_PATH_MAX + 1 ];
   char     pattern[ _POSIX_PATH_MAX + 1 ];
   char     dirname[ _POSIX_PATH_MAX + 1 ];
   char     string[ _POSIX_PATH_MAX + 1 ];
   char     pfname[ _POSIX_PATH_MAX + 1 ];
   char     pfext[ _POSIX_PATH_MAX + 1 ];
   char     fname[ _POSIX_PATH_MAX + 1 ];
   char     fext[ _POSIX_PATH_MAX + 1 ];
   char     ddate[ 9 ];
   char     ttime[ 9 ];
   char     aatrib[ 17 ];
   int      attrib;
   long     fsize = 0;
   time_t   ftime;
   char *   pos;
   USHORT   ushbMask = FA_ARCH;
   PHB_ITEM pDir;

   struct stat statbuf;
   struct tm * ft;

#if defined(__MINGW32__) || ( defined(_MSC_VER) && _MSC_VER >= 1000 )
   struct _finddata_t entry;
   long hFile;
#elif defined(_MSC_VER)
   struct _find_t entry;
   long hFile;
#elif defined(HB_OS_OS2)
   FILEFINDBUF3 entry;
   HDIR         hFind = HDIR_CREATE;
   ULONG        fileTypes = FILE_ARCHIVED | FILE_DIRECTORY | FILE_SYSTEM | FILE_HIDDEN | FILE_READONLY;
   ULONG        findSize = sizeof( entry );
   ULONG        findCount = 1;
#else
   struct dirent * entry;
   DIR  * dir;
#endif

   dirname[ 0 ] = '\0';
   pattern[ 0 ] = '\0';

   /* Get the passed attributes and convert them to Harbour Flags */
   if( pAttributes && hb_itemGetCLen( pAttributes ) >= 1 )
      ushbMask |= HarbourAttributesToMask( ( BYTE * ) hb_itemGetCPtr( pAttributes ) );

#if defined(__MINGW32__) || ( defined(_MSC_VER) && _MSC_VER >= 1000 )
   /* Do we want 8.3 support? */
   bEightDotThree = ( pEightDotThree ? hb_itemGetL( pEightDotThree ) : FALSE );
#endif

   pattern[ 0 ] = '\0';

   /* TODO: add supporting code */
   /* if you request the volume label, that's all you get */
   if( ushbMask & FA_LABEL )
   {
      /* get rid of anything else */
      ushbMask = FA_LABEL;

      if( pDirSpec )
      {
         strcpy( string, hb_itemGetCPtr( pDirSpec ) );
         pos = strrchr( string, ':' );
         if( pos )
            *( ++pos ) = '\0';
         else
            string[ 0 ] = '\0';

         strcpy( pattern, string );
      }
   }
   else
   {
      if( pDirSpec )
      {
         strcpy( string, hb_itemGetCPtr( pDirSpec ) );
         pos = strrchr( string, OS_PATH_DELIMITER );
         if( pos )
         {
            strcpy( pattern, pos + 1 );
            *( pos + 1 ) = '\0';
            strcpy( dirname, string );
         }
         else
         {
            strcpy( pattern, string );
            strcpy( dirname, ".X" );
            dirname[ 1 ] = OS_PATH_DELIMITER;
         }
      }
      if( !*pattern )
         strcpy( pattern, "*.*" );
   }

#if defined(__WATCOMC__)
   iDirnameLen = strlen( dirname );
   if( iDirnameLen < 1 )
   {
      strcpy( dirname, ".X" );
      dirname[ 1 ] = OS_PATH_DELIMITER;
      iDirnameLen = 2;
   }
#endif

   if( strlen( pattern ) > 0 )
   {
      strcpy( string, pattern );
      pos = strrchr( string, '.' );
      if( pos )
      {
         strcpy( pfext, pos + 1 );
         *pos = '\0';
         strcpy( pfname, string );
      }
      else
      {
         strcpy( pfname, string );
         pfext[ 0 ] = '\0';
      }
   }

   if( strlen( pfname ) < 1 )
      strcpy( pfname, "*" );

   HB_TRACE(HB_TR_INFO, ("dirname: |%s|, pattern: |%s|\n", dirname, pattern));
   HB_TRACE(HB_TR_INFO, ("pfname: |%s|, pfext: |%s|\n", pfname, pfext));

   /* should have drive, directory in dirname and filespec in pattern */

   tzset();
   pDir = hb_itemArrayNew( 0 );

#if defined(__MINGW32__) || ( defined(_MSC_VER) && _MSC_VER >=1000 )

   strcpy( string, dirname );
   strcat( string, pattern );

   if( ( hFile = _findfirst( string, &entry ) ) != -1L )
   {
      do
      {
         strcpy( string, dirname );
         strcat( string, entry.name );

         /* this needs the full path to the file */
         if( bEightDotThree )
            GetShortPathName( string, string, _POSIX_PATH_MAX );

#elif defined(_MSC_VER)

   strcpy( string, dirname );
   strcat( string, pattern );

   if( _dos_findfirst( string, ushbMask, &entry ) == 0 )
   {
      do
      {
         strcpy( string, entry.name );
#elif defined(HB_OS_OS2)

   strcpy( string, dirname );
   strcat( string, pattern );

   if( DosFindFirst( string, &hFind, fileTypes, &entry, findSize, &findCount, FIL_STANDARD ) == NO_ERROR && findCount > 0 )
   {
      do
      {
         strcpy( string, entry.achName );
#else

   #if defined(__WATCOMC__)
      /* opendir in Watcom doesn't like the path delimiter at the end of a string */
      dirname[ iDirnameLen ] = '.';
      dirname[ iDirnameLen + 1 ] = '\0';
   #endif

   dir = opendir( dirname );

   #if defined(__WATCOMC__)
      dirname[ iDirnameLen ] = '\0';
   #endif

   if( NULL == dir )
   {
      HB_TRACE(HB_TR_INFO, ("invalid dirname |%s|\n", dirname));

      hb_itemRelease( hb_itemReturn( pDir ) );
      return;
   }

   /* now put everything into an array */
   while( ( entry = readdir( dir ) ) != NULL )
   {
      strcpy( string, entry->d_name );

#endif

      pos = strrchr( string, OS_PATH_DELIMITER );
      pos = strrchr( pos ? ( pos + 1 ) : string, '.' );

      if( pos && ! ( pos == &string[ 0 ] ) )
      {
         strcpy( fext, pos + 1 );
         *pos = '\0';
      }
      else
         fext[ 0 ] = '\0';

      pos = strrchr( string, OS_PATH_DELIMITER );
      strcpy( fname, pos ? ( pos + 1 ) : string );

      if( !*fname )
         strcpy( fname, "*" );

      HB_TRACE(HB_TR_INFO, ("fname: |%s|, fext: |%s|\n", fname, fext));

      if( hb_strMatchRegExp( fname, pfname ) && hb_strMatchRegExp( fext, pfext ) )
      {
         attrib = 0;

#if defined(__MINGW32__) || ( defined(_MSC_VER) && _MSC_VER >= 1000 )

         /* due to short-name support: reconstruct the filename */
         if( bEightDotThree )
         {
            pos = strrchr( string, OS_PATH_DELIMITER );
            if( pos )
            {
               ++pos;
               if( ! *pos || ( *pos == '.' && ! pos[ 1 ] ) )
                  strcat( string, "." );
            }
            strcpy( filename, string );     /* entry.name ); */

            if( *fext )
            {
               strcat( filename, "." );
               strcat( filename, fext );
            }
            *fullfile = '\0';
         }
         else
         {
            strcpy( fullfile, dirname );
            strcpy( filename, entry.name );
         }
#elif defined(_MSC_VER)
         strcpy( filename, entry.name );
         strcpy( fullfile, dirname );
#elif defined(HB_OS_OS2)
         strcpy( filename, entry.achName );
         strcpy( fullfile, dirname );
#else
         strcpy( filename, entry->d_name );
         strcpy( fullfile, dirname );
#endif

         strcat( fullfile, filename );

         if( stat( fullfile, &statbuf ) != -1 )
         {
            fsize = statbuf.st_size;
            ftime = statbuf.st_mtime;

            #if defined(OS_UNIX_COMPATIBLE)
               /* GNU C on Linux or on other UNIX */
               attrib = statbuf.st_mode;
            #elif defined(HB_OS_OS2)
               attrib = entry.attrFile;
            #elif defined(__MINGW32__) || defined(_MSC_VER)
               attrib = entry.attrib;
               #if defined(__MINGW32__) || ( defined(_MSC_VER) && _MSC_VER >= 1000 )
               if( bEightDotThree )
               {
                   /* need to strip off the path */
                   pos = strrchr( filename, OS_PATH_DELIMITER );
                   if( pos )
                      strcpy( filename, ++pos );
               }
               #endif
            #elif defined(__BORLANDC__) && (__BORLANDC__ >= 1280)
               /* NOTE: _chmod( f, 0 ) => Get attribs
                        _chmod( f, 1, n ) => Set attribs
                        chmod() though, _will_ change the attributes */
               attrib = ( USHORT ) _rtl_chmod( fullfile, 0, 0 );
            #elif defined(__BORLANDC__)
               attrib = ( USHORT ) _chmod( fullfile, 0, 0 );
            #elif defined(__DJGPP__)
               attrib = ( USHORT ) _chmod( fullfile, 0 );
            #else
               attrib = 0;
            #endif

            attrib = osToHarbourMask( attrib );

            if( attrib & FA_DIREC )
            {
               /* MS says size for a Directory is undefined.
                  Novell uses these bits for other purposes
                */
               fsize = 0;
            }

            ft = localtime( &ftime );
            sprintf( ddate, "%04d%02d%02d",
               ft->tm_year + 1900, ft->tm_mon + 1, ft->tm_mday );

            sprintf( ttime, "%02d:%02d:%02d",
               ft->tm_hour, ft->tm_min, ft->tm_sec );

            HB_TRACE(HB_TR_INFO, ("name: |%s|, date: |%s|, time: |%s|\n", filename, ddate, ttime));
         }
         else
            HB_TRACE(HB_TR_INFO, ("invalid file |%s|\n", fullfile));

         if( !( ( ( ushbMask & FA_HIDDEN ) == 0 && ( attrib & FA_HIDDEN ) > 0 ) ||
                ( ( ushbMask & FA_SYSTEM ) == 0 && ( attrib & FA_SYSTEM ) > 0 ) ||
                ( ( ushbMask & FA_DIREC  ) == 0 && ( attrib & FA_DIREC  ) > 0 ) ) )
         {
            PHB_ITEM pSubarray = hb_itemArrayNew( F_LEN );

            PHB_ITEM pFilename = hb_itemPutC( NULL, filename );
            PHB_ITEM pSize = hb_itemPutNL( NULL, fsize );
            PHB_ITEM pDate = hb_itemPutDS( NULL, ddate );
            PHB_ITEM pTime = hb_itemPutC( NULL, ttime );
            PHB_ITEM pAttr = hb_itemPutC( NULL, ( char * ) HarbourMaskToAttributes( attrib, ( BYTE * ) aatrib ) );

            hb_itemArrayPut( pSubarray, F_NAME, pFilename );
            hb_itemArrayPut( pSubarray, F_SIZE, pSize );
            hb_itemArrayPut( pSubarray, F_DATE, pDate );
            hb_itemArrayPut( pSubarray, F_TIME, pTime );
            hb_itemArrayPut( pSubarray, F_ATTR, pAttr );

            /* NOTE: Simply ignores the situation where the array length
                     limit is reached. */
            hb_arrayAdd( pDir, pSubarray );

            hb_itemRelease( pFilename );
            hb_itemRelease( pSize );
            hb_itemRelease( pDate );
            hb_itemRelease( pTime );
            hb_itemRelease( pAttr );
            hb_itemRelease( pSubarray );
         }
      }
   }

#if defined(__MINGW32__) || (defined(_MSC_VER) && _MSC_VER >= 1000 )
   while( _findnext( hFile, &entry ) == 0 );
   _findclose( hFile );
#elif defined(_MSC_VER )
   while( _dos_findnext( &entry ) == 0 );
#elif defined(HB_OS_OS2)
   while( DosFindNext( hFind, &entry, findSize, &findCount ) == NO_ERROR && findCount > 0 );
   DosFindClose( hFind );
#else
   closedir( dir );
#endif

#if defined(_MSC_VER) || defined(__MINGW32__) || defined(HB_OS_OS2)
   }
#endif

   HB_TRACE(HB_TR_INFO, ("normal return\n"));

   hb_itemRelease( hb_itemReturn( pDir ) ); /* DIRECTORY() returns an array */

#endif /* HAVE_POSIX_IO */
}

