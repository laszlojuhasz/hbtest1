/*
 * $Id$
 */

/* -------------------------------------------------------------------- */
/* WARNING: Automatically generated source file. DO NOT EDIT!           */
/*          Instead, edit corresponding .qth file,                      */
/*          or the generator tool itself, and run regenarate.           */
/* -------------------------------------------------------------------- */

/*
 * Harbour Project QT wrapper
 *
 * Copyright 2009-2010 Pritpal Bedi <bedipritpal@hotmail.com>
 * www - http://harbour-project.org
 *
 * For full copyright message and credits, see: CREDITS.txt
 *
 */

#include "hbqtcore.h"
#include "hbqtgui.h"

#if QT_VERSION >= 0x040500

/*
 *  Constructed[ 2/2 [ 100.00% ] ]
 *
 */

#include <QtCore/QPointer>

#include <QtGui/QResizeEvent>


/*
 * QResizeEvent ( const QSize & size, const QSize & oldSize )
 */

typedef struct
{
   QResizeEvent * ph;
   bool bNew;
   PHBQT_GC_FUNC func;
   int type;
} HBQT_GC_T_QResizeEvent;

HBQT_GC_FUNC( hbqt_gcRelease_QResizeEvent )
{
   HBQT_GC_T * p = ( HBQT_GC_T * ) Cargo;

   if( p && p->bNew )
      p->ph = NULL;
}

void * hbqt_gcAllocate_QResizeEvent( void * pObj, bool bNew )
{
   HBQT_GC_T * p = ( HBQT_GC_T * ) hb_gcAllocate( sizeof( HBQT_GC_T ), hbqt_gcFuncs() );

   p->ph = ( QResizeEvent * ) pObj;
   p->bNew = bNew;
   p->func = hbqt_gcRelease_QResizeEvent;
   p->type = HBQT_TYPE_QResizeEvent;

   return p;
}

HB_FUNC( QT_QRESIZEEVENT )
{
   // __HB_RETPTRGC__( new QResizeEvent() );
}

/* const QSize & oldSize () const */
HB_FUNC( QT_QRESIZEEVENT_OLDSIZE )
{
   QResizeEvent * p = hbqt_par_QResizeEvent( 1 );
   if( p )
      hb_retptrGC( hbqt_gcAllocate_QSize( new QSize( ( p )->oldSize() ), true ) );
}

/* const QSize & size () const */
HB_FUNC( QT_QRESIZEEVENT_SIZE )
{
   QResizeEvent * p = hbqt_par_QResizeEvent( 1 );
   if( p )
      hb_retptrGC( hbqt_gcAllocate_QSize( new QSize( ( p )->size() ), true ) );
}


#endif /* #if QT_VERSION >= 0x040500 */
