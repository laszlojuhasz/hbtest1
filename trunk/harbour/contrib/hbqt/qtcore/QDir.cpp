/*
 * $Id$
 */

/* -------------------------------------------------------------------- */
/* WARNING: Automatically generated source file. DO NOT EDIT!           */
/*          Instead, edit corresponding .qth file,                      */
/*          or the generator tool itself, and run regenarate.           */
/* -------------------------------------------------------------------- */

/*
 * Harbour Project source code:
 * QT wrapper main header
 *
 * Copyright 2009-2010 Pritpal Bedi <pritpal@vouchcac.com>
 *
 * Copyright 2009 Marcos Antonio Gambeta <marcosgambeta at gmail dot com>
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

#include "../hbqt.h"

/*----------------------------------------------------------------------*/
#if QT_VERSION >= 0x040500
/*----------------------------------------------------------------------*/

/*
 *  enum Filter { Dirs, AllDirs, Files, Drives, ..., CaseSensitive }
 *  enum SortFlag { Name, Time, Size, Type, ..., LocaleAware }
 *  flags Filters
 *  flags SortFlags
 */

#include <QtCore/QPointer>

#include <QtCore/QDir>


/*
 * QDir ( const QDir & dir )
 * QDir ( const QString & path = QString() )
 * QDir ( const QString & path, const QString & nameFilter, SortFlags sort = SortFlags( Name | IgnoreCase ), Filters filters = AllEntries )
 * ~QDir ()
 */

typedef struct
{
   void * ph;
   bool bNew;
   QT_G_FUNC_PTR func;
} QGC_POINTER_QDir;

QT_G_FUNC( hbqt_gcRelease_QDir )
{
   QGC_POINTER * p = ( QGC_POINTER * ) Cargo;

   if( p && p->bNew )
   {
      if( p->ph )
      {
         HB_TRACE( HB_TR_DEBUG, ( "YES_rel_QDir   /.\\    ph=%p", p->ph ) );
         delete ( ( QDir * ) p->ph );
         HB_TRACE( HB_TR_DEBUG, ( "YES_rel_QDir   \\./    ph=%p", p->ph ) );
         p->ph = NULL;
      }
      else
      {
         HB_TRACE( HB_TR_DEBUG, ( "DEL_rel_QDir    :     Object already deleted!" ) );
      }
   }
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "PTR_rel_QDir    :    Object not created with new()" ) );
      p->ph = NULL;
   }
}

void * hbqt_gcAllocate_QDir( void * pObj, bool bNew )
{
   QGC_POINTER * p = ( QGC_POINTER * ) hb_gcAllocate( sizeof( QGC_POINTER ), hbqt_gcFuncs() );

   p->ph = pObj;
   p->bNew = bNew;
   p->func = hbqt_gcRelease_QDir;

   if( bNew )
   {
      HB_TRACE( HB_TR_DEBUG, ( "   _new_QDir                       ph=%p %i B %i KB", pObj, ( int ) hb_xquery( 1001 ), hbqt_getmemused() ) );
   }
   return p;
}

HB_FUNC( QT_QDIR )
{
   void * pObj = NULL;

   pObj = new QDir( hbqt_par_QString( 1 ) ) ;

   hb_retptrGC( hbqt_gcAllocate_QDir( pObj, true ) );
}

/*
 * QString absoluteFilePath ( const QString & fileName ) const
 */
HB_FUNC( QT_QDIR_ABSOLUTEFILEPATH )
{
   hb_retc( hbqt_par_QDir( 1 )->absoluteFilePath( hbqt_par_QString( 2 ) ).toAscii().data() );
}

/*
 * QString absolutePath () const
 */
HB_FUNC( QT_QDIR_ABSOLUTEPATH )
{
   hb_retc( hbqt_par_QDir( 1 )->absolutePath().toAscii().data() );
}

/*
 * QString canonicalPath () const
 */
HB_FUNC( QT_QDIR_CANONICALPATH )
{
   hb_retc( hbqt_par_QDir( 1 )->canonicalPath().toAscii().data() );
}

/*
 * bool cd ( const QString & dirName )
 */
HB_FUNC( QT_QDIR_CD )
{
   hb_retl( hbqt_par_QDir( 1 )->cd( hbqt_par_QString( 2 ) ) );
}

/*
 * bool cdUp ()
 */
HB_FUNC( QT_QDIR_CDUP )
{
   hb_retl( hbqt_par_QDir( 1 )->cdUp() );
}

/*
 * uint count () const
 */
HB_FUNC( QT_QDIR_COUNT )
{
   hb_retni( hbqt_par_QDir( 1 )->count() );
}

/*
 * QString dirName () const
 */
HB_FUNC( QT_QDIR_DIRNAME )
{
   hb_retc( hbqt_par_QDir( 1 )->dirName().toAscii().data() );
}

/*
 * QStringList entryList ( const QStringList & nameFilters, Filters filters = NoFilter, SortFlags sort = NoSort ) const
 */
HB_FUNC( QT_QDIR_ENTRYLIST )
{
   hb_retptrGC( hbqt_gcAllocate_QStringList( new QStringList( hbqt_par_QDir( 1 )->entryList( *hbqt_par_QStringList( 2 ), ( HB_ISNUM( 3 ) ? ( QDir::Filters ) hb_parni( 3 ) : ( QDir::Filters ) QDir::NoFilter ), ( HB_ISNUM( 4 ) ? ( QDir::SortFlags ) hb_parni( 4 ) : ( QDir::SortFlags ) QDir::NoSort ) ) ), true ) );
}

/*
 * QStringList entryList ( Filters filters = NoFilter, SortFlags sort = NoSort ) const
 */
HB_FUNC( QT_QDIR_ENTRYLIST_1 )
{
   hb_retptrGC( hbqt_gcAllocate_QStringList( new QStringList( hbqt_par_QDir( 1 )->entryList( ( HB_ISNUM( 2 ) ? ( QDir::Filters ) hb_parni( 2 ) : ( QDir::Filters ) QDir::NoFilter ), ( HB_ISNUM( 3 ) ? ( QDir::SortFlags ) hb_parni( 3 ) : ( QDir::SortFlags ) QDir::NoSort ) ) ), true ) );
}

/*
 * bool exists ( const QString & name ) const
 */
HB_FUNC( QT_QDIR_EXISTS )
{
   hb_retl( hbqt_par_QDir( 1 )->exists( hbqt_par_QString( 2 ) ) );
}

/*
 * bool exists () const
 */
HB_FUNC( QT_QDIR_EXISTS_1 )
{
   hb_retl( hbqt_par_QDir( 1 )->exists() );
}

/*
 * QString filePath ( const QString & fileName ) const
 */
HB_FUNC( QT_QDIR_FILEPATH )
{
   hb_retc( hbqt_par_QDir( 1 )->filePath( hbqt_par_QString( 2 ) ).toAscii().data() );
}

/*
 * Filters filter () const
 */
HB_FUNC( QT_QDIR_FILTER )
{
   hb_retni( ( QDir::Filters ) hbqt_par_QDir( 1 )->filter() );
}

/*
 * bool isAbsolute () const
 */
HB_FUNC( QT_QDIR_ISABSOLUTE )
{
   hb_retl( hbqt_par_QDir( 1 )->isAbsolute() );
}

/*
 * bool isReadable () const
 */
HB_FUNC( QT_QDIR_ISREADABLE )
{
   hb_retl( hbqt_par_QDir( 1 )->isReadable() );
}

/*
 * bool isRelative () const
 */
HB_FUNC( QT_QDIR_ISRELATIVE )
{
   hb_retl( hbqt_par_QDir( 1 )->isRelative() );
}

/*
 * bool isRoot () const
 */
HB_FUNC( QT_QDIR_ISROOT )
{
   hb_retl( hbqt_par_QDir( 1 )->isRoot() );
}

/*
 * bool makeAbsolute ()
 */
HB_FUNC( QT_QDIR_MAKEABSOLUTE )
{
   hb_retl( hbqt_par_QDir( 1 )->makeAbsolute() );
}

/*
 * bool mkdir ( const QString & dirName ) const
 */
HB_FUNC( QT_QDIR_MKDIR )
{
   hb_retl( hbqt_par_QDir( 1 )->mkdir( hbqt_par_QString( 2 ) ) );
}

/*
 * bool mkpath ( const QString & dirPath ) const
 */
HB_FUNC( QT_QDIR_MKPATH )
{
   hb_retl( hbqt_par_QDir( 1 )->mkpath( hbqt_par_QString( 2 ) ) );
}

/*
 * QStringList nameFilters () const
 */
HB_FUNC( QT_QDIR_NAMEFILTERS )
{
   hb_retptrGC( hbqt_gcAllocate_QStringList( new QStringList( hbqt_par_QDir( 1 )->nameFilters() ), true ) );
}

/*
 * QString path () const
 */
HB_FUNC( QT_QDIR_PATH )
{
   hb_retc( hbqt_par_QDir( 1 )->path().toAscii().data() );
}

/*
 * void refresh () const
 */
HB_FUNC( QT_QDIR_REFRESH )
{
   hbqt_par_QDir( 1 )->refresh();
}

/*
 * QString relativeFilePath ( const QString & fileName ) const
 */
HB_FUNC( QT_QDIR_RELATIVEFILEPATH )
{
   hb_retc( hbqt_par_QDir( 1 )->relativeFilePath( hbqt_par_QString( 2 ) ).toAscii().data() );
}

/*
 * bool remove ( const QString & fileName )
 */
HB_FUNC( QT_QDIR_REMOVE )
{
   hb_retl( hbqt_par_QDir( 1 )->remove( hbqt_par_QString( 2 ) ) );
}

/*
 * bool rename ( const QString & oldName, const QString & newName )
 */
HB_FUNC( QT_QDIR_RENAME )
{
   hb_retl( hbqt_par_QDir( 1 )->rename( hbqt_par_QString( 2 ), hbqt_par_QString( 3 ) ) );
}

/*
 * bool rmdir ( const QString & dirName ) const
 */
HB_FUNC( QT_QDIR_RMDIR )
{
   hb_retl( hbqt_par_QDir( 1 )->rmdir( hbqt_par_QString( 2 ) ) );
}

/*
 * bool rmpath ( const QString & dirPath ) const
 */
HB_FUNC( QT_QDIR_RMPATH )
{
   hb_retl( hbqt_par_QDir( 1 )->rmpath( hbqt_par_QString( 2 ) ) );
}

/*
 * void setFilter ( Filters filters )
 */
HB_FUNC( QT_QDIR_SETFILTER )
{
   hbqt_par_QDir( 1 )->setFilter( ( QDir::Filters ) hb_parni( 2 ) );
}

/*
 * void setNameFilters ( const QStringList & nameFilters )
 */
HB_FUNC( QT_QDIR_SETNAMEFILTERS )
{
   hbqt_par_QDir( 1 )->setNameFilters( *hbqt_par_QStringList( 2 ) );
}

/*
 * void setPath ( const QString & path )
 */
HB_FUNC( QT_QDIR_SETPATH )
{
   hbqt_par_QDir( 1 )->setPath( hbqt_par_QString( 2 ) );
}

/*
 * void setSorting ( SortFlags sort )
 */
HB_FUNC( QT_QDIR_SETSORTING )
{
   hbqt_par_QDir( 1 )->setSorting( ( QDir::SortFlags ) hb_parni( 2 ) );
}

/*
 * SortFlags sorting () const
 */
HB_FUNC( QT_QDIR_SORTING )
{
   hb_retni( ( QDir::SortFlags ) hbqt_par_QDir( 1 )->sorting() );
}

/*
 * void addSearchPath ( const QString & prefix, const QString & path )
 */
HB_FUNC( QT_QDIR_ADDSEARCHPATH )
{
   hbqt_par_QDir( 1 )->addSearchPath( hbqt_par_QString( 2 ), hbqt_par_QString( 3 ) );
}

/*
 * QString cleanPath ( const QString & path )
 */
HB_FUNC( QT_QDIR_CLEANPATH )
{
   hb_retc( hbqt_par_QDir( 1 )->cleanPath( hbqt_par_QString( 2 ) ).toAscii().data() );
}

/*
 * QDir current ()
 */
HB_FUNC( QT_QDIR_CURRENT )
{
   hb_retptrGC( hbqt_gcAllocate_QDir( new QDir( hbqt_par_QDir( 1 )->current() ), true ) );
}

/*
 * QString currentPath ()
 */
HB_FUNC( QT_QDIR_CURRENTPATH )
{
   hb_retc( hbqt_par_QDir( 1 )->currentPath().toAscii().data() );
}

/*
 * QString fromNativeSeparators ( const QString & pathName )
 */
HB_FUNC( QT_QDIR_FROMNATIVESEPARATORS )
{
   hb_retc( hbqt_par_QDir( 1 )->fromNativeSeparators( hbqt_par_QString( 2 ) ).toAscii().data() );
}

/*
 * QDir home ()
 */
HB_FUNC( QT_QDIR_HOME )
{
   hb_retptrGC( hbqt_gcAllocate_QDir( new QDir( hbqt_par_QDir( 1 )->home() ), true ) );
}

/*
 * QString homePath ()
 */
HB_FUNC( QT_QDIR_HOMEPATH )
{
   hb_retc( hbqt_par_QDir( 1 )->homePath().toAscii().data() );
}

/*
 * bool isAbsolutePath ( const QString & path )
 */
HB_FUNC( QT_QDIR_ISABSOLUTEPATH )
{
   hb_retl( hbqt_par_QDir( 1 )->isAbsolutePath( hbqt_par_QString( 2 ) ) );
}

/*
 * bool isRelativePath ( const QString & path )
 */
HB_FUNC( QT_QDIR_ISRELATIVEPATH )
{
   hb_retl( hbqt_par_QDir( 1 )->isRelativePath( hbqt_par_QString( 2 ) ) );
}

/*
 * bool match ( const QString & filter, const QString & fileName )
 */
HB_FUNC( QT_QDIR_MATCH )
{
   hb_retl( hbqt_par_QDir( 1 )->match( hbqt_par_QString( 2 ), hbqt_par_QString( 3 ) ) );
}

/*
 * bool match ( const QStringList & filters, const QString & fileName )
 */
HB_FUNC( QT_QDIR_MATCH_1 )
{
   hb_retl( hbqt_par_QDir( 1 )->match( *hbqt_par_QStringList( 2 ), hbqt_par_QString( 3 ) ) );
}

/*
 * QDir root ()
 */
HB_FUNC( QT_QDIR_ROOT )
{
   hb_retptrGC( hbqt_gcAllocate_QDir( new QDir( hbqt_par_QDir( 1 )->root() ), true ) );
}

/*
 * QString rootPath ()
 */
HB_FUNC( QT_QDIR_ROOTPATH )
{
   hb_retc( hbqt_par_QDir( 1 )->rootPath().toAscii().data() );
}

/*
 * QStringList searchPaths ( const QString & prefix )
 */
HB_FUNC( QT_QDIR_SEARCHPATHS )
{
   hb_retptrGC( hbqt_gcAllocate_QStringList( new QStringList( hbqt_par_QDir( 1 )->searchPaths( hbqt_par_QString( 2 ) ) ), true ) );
}

/*
 * QChar separator ()
 */
HB_FUNC( QT_QDIR_SEPARATOR )
{
   hb_retptrGC( hbqt_gcAllocate_QChar( new QChar( hbqt_par_QDir( 1 )->separator() ), true ) );
}

/*
 * bool setCurrent ( const QString & path )
 */
HB_FUNC( QT_QDIR_SETCURRENT )
{
   hb_retl( hbqt_par_QDir( 1 )->setCurrent( hbqt_par_QString( 2 ) ) );
}

/*
 * void setSearchPaths ( const QString & prefix, const QStringList & searchPaths )
 */
HB_FUNC( QT_QDIR_SETSEARCHPATHS )
{
   hbqt_par_QDir( 1 )->setSearchPaths( hbqt_par_QString( 2 ), *hbqt_par_QStringList( 3 ) );
}

/*
 * QDir temp ()
 */
HB_FUNC( QT_QDIR_TEMP )
{
   hb_retptrGC( hbqt_gcAllocate_QDir( new QDir( hbqt_par_QDir( 1 )->temp() ), true ) );
}

/*
 * QString tempPath ()
 */
HB_FUNC( QT_QDIR_TEMPPATH )
{
   hb_retc( hbqt_par_QDir( 1 )->tempPath().toAscii().data() );
}

/*
 * QString toNativeSeparators ( const QString & pathName )
 */
HB_FUNC( QT_QDIR_TONATIVESEPARATORS )
{
   hb_retc( hbqt_par_QDir( 1 )->toNativeSeparators( hbqt_par_QString( 2 ) ).toAscii().data() );
}


/*----------------------------------------------------------------------*/
#endif             /* #if QT_VERSION >= 0x040500 */
/*----------------------------------------------------------------------*/
