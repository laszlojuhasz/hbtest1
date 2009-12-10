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
 * Copyright 2009 Pritpal Bedi <pritpal@vouchcac.com>
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

#include "hbapi.h"
#include "../hbqt.h"

/*----------------------------------------------------------------------*/
#if QT_VERSION >= 0x040500
/*----------------------------------------------------------------------*/

/*
 *  enum FontDialogOption { NoButtons, DontUseNativeDialog }
 *  flags FontDialogOptions
 */

#include <QtCore/QPointer>

#include <QtGui/QFontDialog>


/*
 * QFontDialog ( QWidget * parent = 0 )
 * QFontDialog ( const QFont & initial, QWidget * parent = 0 )
 */

typedef struct
{
  void * ph;
  QT_G_FUNC_PTR func;
  QPointer< QFontDialog > pq;
} QGC_POINTER_QFontDialog;

QT_G_FUNC( release_QFontDialog )
{
   QGC_POINTER_QFontDialog * p = ( QGC_POINTER_QFontDialog * ) Cargo;

   HB_TRACE( HB_TR_DEBUG, ( "release_QFontDialog                  p=%p", p));
   HB_TRACE( HB_TR_DEBUG, ( "release_QFontDialog                 ph=%p pq=%p", p->ph, (void *)(p->pq)));

   if( p && p->ph && p->pq )
   {
      const QMetaObject * m = ( ( QObject * ) p->ph )->metaObject();
      if( ( QString ) m->className() != ( QString ) "QObject" )
      {
         switch( hbqt_get_object_release_method() )
         {
         case HBQT_RELEASE_WITH_DELETE:
            delete ( ( QFontDialog * ) p->ph );
            break;
         case HBQT_RELEASE_WITH_DESTRUTOR:
            ( ( QFontDialog * ) p->ph )->~QFontDialog();
            break;
         case HBQT_RELEASE_WITH_DELETE_LATER:
            ( ( QFontDialog * ) p->ph )->deleteLater();
            break;
         }
         p->ph = NULL;
         HB_TRACE( HB_TR_DEBUG, ( "release_QFontDialog                 Object deleted! %i B %i KB", ( int ) hb_xquery( 1001 ), hbqt_getmemused() ) );
      }
      else
      {
         HB_TRACE( HB_TR_DEBUG, ( "NO release_QFontDialog                 Object Name Missing!" ) );
      }
   }
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "DEL release_QFontDialog                 Object Already deleted!" ) );
   }
}

void * gcAllocate_QFontDialog( void * pObj )
{
   QGC_POINTER_QFontDialog * p = ( QGC_POINTER_QFontDialog * ) hb_gcAllocate( sizeof( QGC_POINTER_QFontDialog ), gcFuncs() );

   p->ph = pObj;
   p->func = release_QFontDialog;
   new( & p->pq ) QPointer< QFontDialog >( ( QFontDialog * ) pObj );
   HB_TRACE( HB_TR_DEBUG, ( "          new_QFontDialog                 %i B %i KB", ( int ) hb_xquery( 1001 ), hbqt_getmemused() ) );
   return( p );
}

HB_FUNC( QT_QFONTDIALOG )
{
   void * pObj = NULL;

   if( hb_pcount() == 1 && HB_ISPOINTER( 1 ) )
   {
      pObj = new QFontDialog( hbqt_par_QWidget( 1 ) ) ;
   }
   else if( hb_pcount() == 2 && HB_ISPOINTER( 1 ) && HB_ISPOINTER( 2 ) )
   {
      pObj = new QFontDialog( *hbqt_par_QFont( 1 ), hbqt_par_QWidget( 2 ) ) ;
   }
   else
   {
      pObj = new QFontDialog( 0 ) ;
   }

   hb_retptrGC( gcAllocate_QFontDialog( pObj ) );
}
/*
 * QFont currentFont () const
 */
HB_FUNC( QT_QFONTDIALOG_CURRENTFONT )
{
   hb_retptrGC( gcAllocate_QFont( new QFont( hbqt_par_QFontDialog( 1 )->currentFont() ) ) );
}

/*
 * FontDialogOptions options () const
 */
HB_FUNC( QT_QFONTDIALOG_OPTIONS )
{
   hb_retni( ( QFontDialog::FontDialogOptions ) hbqt_par_QFontDialog( 1 )->options() );
}

/*
 * QFont selectedFont () const
 */
HB_FUNC( QT_QFONTDIALOG_SELECTEDFONT )
{
   hb_retptrGC( gcAllocate_QFont( new QFont( hbqt_par_QFontDialog( 1 )->selectedFont() ) ) );
}

/*
 * void setCurrentFont ( const QFont & font )
 */
HB_FUNC( QT_QFONTDIALOG_SETCURRENTFONT )
{
   hbqt_par_QFontDialog( 1 )->setCurrentFont( *hbqt_par_QFont( 2 ) );
}

/*
 * void setOption ( FontDialogOption option, bool on = true )
 */
HB_FUNC( QT_QFONTDIALOG_SETOPTION )
{
   hbqt_par_QFontDialog( 1 )->setOption( ( QFontDialog::FontDialogOption ) hb_parni( 2 ), hb_parl( 3 ) );
}

/*
 * void setOptions ( FontDialogOptions options )
 */
HB_FUNC( QT_QFONTDIALOG_SETOPTIONS )
{
   hbqt_par_QFontDialog( 1 )->setOptions( ( QFontDialog::FontDialogOptions ) hb_parni( 2 ) );
}

/*
 * bool testOption ( FontDialogOption option ) const
 */
HB_FUNC( QT_QFONTDIALOG_TESTOPTION )
{
   hb_retl( hbqt_par_QFontDialog( 1 )->testOption( ( QFontDialog::FontDialogOption ) hb_parni( 2 ) ) );
}

/*
 * QFont getFont ( bool * ok, const QFont & initial, QWidget * parent, const QString & title, FontDialogOptions options )
 */
HB_FUNC( QT_QFONTDIALOG_GETFONT )
{
   bool iOk = 0;

   hb_retptrGC( gcAllocate_QFont( new QFont( hbqt_par_QFontDialog( 1 )->getFont( &iOk, *hbqt_par_QFont( 3 ), hbqt_par_QWidget( 4 ), hbqt_par_QString( 5 ), ( QFontDialog::FontDialogOptions ) hb_parni( 6 ) ) ) ) );

   hb_stornl( iOk, 2 );
}

/*
 * QFont getFont ( bool * ok, const QFont & initial, QWidget * parent, const char * name )
 */
HB_FUNC( QT_QFONTDIALOG_GETFONT_1 )
{
   bool iOk = 0;

   hb_retptrGC( gcAllocate_QFont( new QFont( hbqt_par_QFontDialog( 1 )->getFont( &iOk, *hbqt_par_QFont( 3 ), hbqt_par_QWidget( 4 ), hbqt_par_char( 5 ) ) ) ) );

   hb_stornl( iOk, 2 );
}

/*
 * QFont getFont ( bool * ok, const QFont & initial, QWidget * parent, const QString & title )
 */
HB_FUNC( QT_QFONTDIALOG_GETFONT_2 )
{
   bool iOk = 0;

   hb_retptrGC( gcAllocate_QFont( new QFont( hbqt_par_QFontDialog( 1 )->getFont( &iOk, *hbqt_par_QFont( 3 ), hbqt_par_QWidget( 4 ), hbqt_par_QString( 5 ) ) ) ) );

   hb_stornl( iOk, 2 );
}

/*
 * QFont getFont ( bool * ok, const QFont & initial, QWidget * parent = 0 )
 */
HB_FUNC( QT_QFONTDIALOG_GETFONT_3 )
{
   bool iOk = 0;

   hb_retptrGC( gcAllocate_QFont( new QFont( hbqt_par_QFontDialog( 1 )->getFont( &iOk, *hbqt_par_QFont( 3 ), hbqt_par_QWidget( 4 ) ) ) ) );

   hb_stornl( iOk, 2 );
}

/*
 * QFont getFont ( bool * ok, QWidget * parent = 0 )
 */
HB_FUNC( QT_QFONTDIALOG_GETFONT_4 )
{
   bool iOk = 0;

   hb_retptrGC( gcAllocate_QFont( new QFont( hbqt_par_QFontDialog( 1 )->getFont( &iOk, hbqt_par_QWidget( 3 ) ) ) ) );

   hb_stornl( iOk, 2 );
}


/*----------------------------------------------------------------------*/
#endif             /* #if QT_VERSION >= 0x040500 */
/*----------------------------------------------------------------------*/
