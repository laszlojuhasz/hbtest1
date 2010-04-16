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
 *  enum DialogCode { Accepted, Rejected }
 */

#include <QtCore/QPointer>

#include  <QtGui/QDialog>


/*
 * QDialog ( QWidget * parent = 0, Qt::WindowFlags f = 0 )
 * ~QDialog ()
 */

typedef struct
{
   QPointer< QDialog > ph;
   bool bNew;
   QT_G_FUNC_PTR func;
} QGC_POINTER_QDialog;

QT_G_FUNC( hbqt_gcRelease_QDialog )
{
   QDialog  * ph = NULL ;
   QGC_POINTER_QDialog * p = ( QGC_POINTER_QDialog * ) Cargo;

   if( p && p->bNew && p->ph )
   {
      ph = p->ph;
      if( ph )
      {
         const QMetaObject * m = ( ph )->metaObject();
         if( ( QString ) m->className() != ( QString ) "QObject" )
         {
            HB_TRACE( HB_TR_DEBUG, ( "ph=%p %p YES_rel_QDialog   /.\\   ", (void*) ph, (void*) p->ph ) );
            delete ( p->ph );
            HB_TRACE( HB_TR_DEBUG, ( "ph=%p %p YES_rel_QDialog   \\./   ", (void*) ph, (void*) p->ph ) );
            p->ph = NULL;
         }
         else
         {
            HB_TRACE( HB_TR_DEBUG, ( "ph=%p NO__rel_QDialog          ", ph ) );
            p->ph = NULL;
         }
      }
      else
      {
         HB_TRACE( HB_TR_DEBUG, ( "ph=%p DEL_rel_QDialog    :     Object already deleted!", ph ) );
         p->ph = NULL;
      }
   }
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "ph=%p PTR_rel_QDialog    :    Object not created with new=true", ph ) );
      p->ph = NULL;
   }
}

void * hbqt_gcAllocate_QDialog( void * pObj, bool bNew )
{
   QGC_POINTER_QDialog * p = ( QGC_POINTER_QDialog * ) hb_gcAllocate( sizeof( QGC_POINTER_QDialog ), hbqt_gcFuncs() );

   new( & p->ph ) QPointer< QDialog >( ( QDialog * ) pObj );
   p->bNew = bNew;
   p->func = hbqt_gcRelease_QDialog;

   if( bNew )
   {
      HB_TRACE( HB_TR_DEBUG, ( "ph=%p    _new_QDialog  under p->pq", pObj ) );
   }
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "ph=%p NOT_new_QDialog", pObj ) );
   }
   return p;
}

HB_FUNC( QT_QDIALOG )
{
   QDialog * pObj = NULL;

   pObj = new QDialog( hbqt_par_QWidget( 1 ), ( Qt::WindowFlags ) hb_parni( 2 ) ) ;

   hb_retptrGC( hbqt_gcAllocate_QDialog( ( void * ) pObj, true ) );
}

/*
 * bool isSizeGripEnabled () const
 */
HB_FUNC( QT_QDIALOG_ISSIZEGRIPENABLED )
{
   QDialog * p = hbqt_par_QDialog( 1 );
   if( p )
      hb_retl( ( p )->isSizeGripEnabled() );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QDIALOG_ISSIZEGRIPENABLED FP=hb_retl( ( p )->isSizeGripEnabled() ); p is NULL" ) );
   }
}

/*
 * int result () const
 */
HB_FUNC( QT_QDIALOG_RESULT )
{
   QDialog * p = hbqt_par_QDialog( 1 );
   if( p )
      hb_retni( ( p )->result() );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QDIALOG_RESULT FP=hb_retni( ( p )->result() ); p is NULL" ) );
   }
}

/*
 * void setModal ( bool modal )
 */
HB_FUNC( QT_QDIALOG_SETMODAL )
{
   QDialog * p = hbqt_par_QDialog( 1 );
   if( p )
      ( p )->setModal( hb_parl( 2 ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QDIALOG_SETMODAL FP=( p )->setModal( hb_parl( 2 ) ); p is NULL" ) );
   }
}

/*
 * void setResult ( int i )
 */
HB_FUNC( QT_QDIALOG_SETRESULT )
{
   QDialog * p = hbqt_par_QDialog( 1 );
   if( p )
      ( p )->setResult( hb_parni( 2 ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QDIALOG_SETRESULT FP=( p )->setResult( hb_parni( 2 ) ); p is NULL" ) );
   }
}

/*
 * void setSizeGripEnabled ( bool )
 */
HB_FUNC( QT_QDIALOG_SETSIZEGRIPENABLED )
{
   QDialog * p = hbqt_par_QDialog( 1 );
   if( p )
      ( p )->setSizeGripEnabled( hb_parl( 2 ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QDIALOG_SETSIZEGRIPENABLED FP=( p )->setSizeGripEnabled( hb_parl( 2 ) ); p is NULL" ) );
   }
}

/*
 * virtual void accept ()
 */
HB_FUNC( QT_QDIALOG_ACCEPT )
{
   QDialog * p = hbqt_par_QDialog( 1 );
   if( p )
      ( p )->accept();
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QDIALOG_ACCEPT FP=( p )->accept(); p is NULL" ) );
   }
}

/*
 * virtual void done ( int r )
 */
HB_FUNC( QT_QDIALOG_DONE )
{
   QDialog * p = hbqt_par_QDialog( 1 );
   if( p )
      ( p )->done( hb_parni( 2 ) );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QDIALOG_DONE FP=( p )->done( hb_parni( 2 ) ); p is NULL" ) );
   }
}

/*
 * int exec ()
 */
HB_FUNC( QT_QDIALOG_EXEC )
{
   QDialog * p = hbqt_par_QDialog( 1 );
   if( p )
      hb_retni( ( p )->exec() );
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QDIALOG_EXEC FP=hb_retni( ( p )->exec() ); p is NULL" ) );
   }
}

/*
 * void open ()
 */
HB_FUNC( QT_QDIALOG_OPEN )
{
   QDialog * p = hbqt_par_QDialog( 1 );
   if( p )
      ( p )->open();
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QDIALOG_OPEN FP=( p )->open(); p is NULL" ) );
   }
}

/*
 * virtual void reject ()
 */
HB_FUNC( QT_QDIALOG_REJECT )
{
   QDialog * p = hbqt_par_QDialog( 1 );
   if( p )
      ( p )->reject();
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "............................... F=QT_QDIALOG_REJECT FP=( p )->reject(); p is NULL" ) );
   }
}


/*----------------------------------------------------------------------*/
#endif             /* #if QT_VERSION >= 0x040500 */
/*----------------------------------------------------------------------*/
