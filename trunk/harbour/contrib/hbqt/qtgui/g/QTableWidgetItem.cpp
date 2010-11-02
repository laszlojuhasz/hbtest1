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
 *  enum ItemType { Type, UserType }
 */

/*
 *  Constructed[ 35/35 [ 100.00% ] ]
 *
 */

#include <QtCore/QPointer>

#include <QtGui/QTableWidgetItem>


/*
 * QTableWidgetItem ( int type = Type )
 * QTableWidgetItem ( const QString & text, int type = Type )
 * QTableWidgetItem ( const QIcon & icon, const QString & text, int type = Type )
 * QTableWidgetItem ( const QTableWidgetItem & other )
 * virtual ~QTableWidgetItem ()
 */

typedef struct
{
   QTableWidgetItem * ph;
   bool bNew;
   PHBQT_GC_FUNC func;
   int type;
} HBQT_GC_T_QTableWidgetItem;

HBQT_GC_FUNC( hbqt_gcRelease_QTableWidgetItem )
{
   HBQT_GC_T * p = ( HBQT_GC_T * ) Cargo;

   if( p )
   {
      if( p->bNew && p->ph )
         delete ( ( QTableWidgetItem * ) p->ph );
      p->ph = NULL;
   }
}

void * hbqt_gcAllocate_QTableWidgetItem( void * pObj, bool bNew )
{
   HBQT_GC_T * p = ( HBQT_GC_T * ) hb_gcAllocate( sizeof( HBQT_GC_T ), hbqt_gcFuncs() );

   p->ph = ( QTableWidgetItem * ) pObj;
   p->bNew = bNew;
   p->func = hbqt_gcRelease_QTableWidgetItem;
   p->type = HBQT_TYPE_QTableWidgetItem;

   return p;
}

HB_FUNC( QT_QTABLEWIDGETITEM )
{
   QTableWidgetItem * pObj = NULL;

   if( hb_pcount() == 2 && HB_ISCHAR( 1 ) && HB_ISNUM( 2 ) )
   {
      pObj = new QTableWidgetItem( hbqt_par_QString( 1 ), hb_parni( 2 ) ) ;
   }
   else if( hb_pcount() == 1 && HB_ISNUM( 1 ) )
   {
      pObj = new QTableWidgetItem( hb_parni( 1 ) ) ;
   }
   else if( hb_pcount() == 1 && HB_ISPOINTER( 1 ) )
   {
      pObj = new QTableWidgetItem( *hbqt_par_QTableWidgetItem( 1 ) ) ;
   }
   else
   {
      pObj = new QTableWidgetItem( 0 ) ;
   }

   hb_retptrGC( hbqt_gcAllocate_QTableWidgetItem( ( void * ) pObj, true ) );
}

/* QBrush background () const */
HB_FUNC( QT_QTABLEWIDGETITEM_BACKGROUND )
{
   QTableWidgetItem * p = hbqt_par_QTableWidgetItem( 1 );
   if( p )
      hb_retptrGC( hbqt_gcAllocate_QBrush( new QBrush( ( p )->background() ), true ) );
}

/* Qt::CheckState checkState () const */
HB_FUNC( QT_QTABLEWIDGETITEM_CHECKSTATE )
{
   QTableWidgetItem * p = hbqt_par_QTableWidgetItem( 1 );
   if( p )
      hb_retni( ( Qt::CheckState ) ( p )->checkState() );
}

/* virtual QTableWidgetItem * clone () const */
HB_FUNC( QT_QTABLEWIDGETITEM_CLONE )
{
   QTableWidgetItem * p = hbqt_par_QTableWidgetItem( 1 );
   if( p )
      hb_retptrGC( hbqt_gcAllocate_QTableWidgetItem( ( p )->clone(), false ) );
}

/* int column () const */
HB_FUNC( QT_QTABLEWIDGETITEM_COLUMN )
{
   QTableWidgetItem * p = hbqt_par_QTableWidgetItem( 1 );
   if( p )
      hb_retni( ( p )->column() );
}

/* virtual QVariant data ( int role ) const */
HB_FUNC( QT_QTABLEWIDGETITEM_DATA )
{
   QTableWidgetItem * p = hbqt_par_QTableWidgetItem( 1 );
   if( p )
      hb_retptrGC( hbqt_gcAllocate_QVariant( new QVariant( ( p )->data( hb_parni( 2 ) ) ), true ) );
}

/* Qt::ItemFlags flags () const */
HB_FUNC( QT_QTABLEWIDGETITEM_FLAGS )
{
   QTableWidgetItem * p = hbqt_par_QTableWidgetItem( 1 );
   if( p )
      hb_retni( ( Qt::ItemFlags ) ( p )->flags() );
}

/* QFont font () const */
HB_FUNC( QT_QTABLEWIDGETITEM_FONT )
{
   QTableWidgetItem * p = hbqt_par_QTableWidgetItem( 1 );
   if( p )
      hb_retptrGC( hbqt_gcAllocate_QFont( new QFont( ( p )->font() ), true ) );
}

/* QBrush foreground () const */
HB_FUNC( QT_QTABLEWIDGETITEM_FOREGROUND )
{
   QTableWidgetItem * p = hbqt_par_QTableWidgetItem( 1 );
   if( p )
      hb_retptrGC( hbqt_gcAllocate_QBrush( new QBrush( ( p )->foreground() ), true ) );
}

/* QIcon icon () const */
HB_FUNC( QT_QTABLEWIDGETITEM_ICON )
{
   QTableWidgetItem * p = hbqt_par_QTableWidgetItem( 1 );
   if( p )
      hb_retptrGC( hbqt_gcAllocate_QIcon( new QIcon( ( p )->icon() ), true ) );
}

/* bool isSelected () const */
HB_FUNC( QT_QTABLEWIDGETITEM_ISSELECTED )
{
   QTableWidgetItem * p = hbqt_par_QTableWidgetItem( 1 );
   if( p )
      hb_retl( ( p )->isSelected() );
}

/* virtual void read ( QDataStream & in ) */
HB_FUNC( QT_QTABLEWIDGETITEM_READ )
{
   QTableWidgetItem * p = hbqt_par_QTableWidgetItem( 1 );
   if( p )
      ( p )->read( *hbqt_par_QDataStream( 2 ) );
}

/* int row () const */
HB_FUNC( QT_QTABLEWIDGETITEM_ROW )
{
   QTableWidgetItem * p = hbqt_par_QTableWidgetItem( 1 );
   if( p )
      hb_retni( ( p )->row() );
}

/* void setBackground ( const QBrush & brush ) */
HB_FUNC( QT_QTABLEWIDGETITEM_SETBACKGROUND )
{
   QTableWidgetItem * p = hbqt_par_QTableWidgetItem( 1 );
   if( p )
      ( p )->setBackground( *hbqt_par_QBrush( 2 ) );
}

/* void setCheckState ( Qt::CheckState state ) */
HB_FUNC( QT_QTABLEWIDGETITEM_SETCHECKSTATE )
{
   QTableWidgetItem * p = hbqt_par_QTableWidgetItem( 1 );
   if( p )
      ( p )->setCheckState( ( Qt::CheckState ) hb_parni( 2 ) );
}

/* virtual void setData ( int role, const QVariant & value ) */
HB_FUNC( QT_QTABLEWIDGETITEM_SETDATA )
{
   QTableWidgetItem * p = hbqt_par_QTableWidgetItem( 1 );
   if( p )
      ( p )->setData( hb_parni( 2 ), *hbqt_par_QVariant( 3 ) );
}

/* void setFlags ( Qt::ItemFlags flags ) */
HB_FUNC( QT_QTABLEWIDGETITEM_SETFLAGS )
{
   QTableWidgetItem * p = hbqt_par_QTableWidgetItem( 1 );
   if( p )
      ( p )->setFlags( ( Qt::ItemFlags ) hb_parni( 2 ) );
}

/* void setFont ( const QFont & font ) */
HB_FUNC( QT_QTABLEWIDGETITEM_SETFONT )
{
   QTableWidgetItem * p = hbqt_par_QTableWidgetItem( 1 );
   if( p )
      ( p )->setFont( *hbqt_par_QFont( 2 ) );
}

/* void setForeground ( const QBrush & brush ) */
HB_FUNC( QT_QTABLEWIDGETITEM_SETFOREGROUND )
{
   QTableWidgetItem * p = hbqt_par_QTableWidgetItem( 1 );
   if( p )
      ( p )->setForeground( *hbqt_par_QBrush( 2 ) );
}

/* void setIcon ( const QIcon & icon ) */
HB_FUNC( QT_QTABLEWIDGETITEM_SETICON )
{
   QTableWidgetItem * p = hbqt_par_QTableWidgetItem( 1 );
   if( p )
      ( p )->setIcon( ( HB_ISCHAR( 2 ) ? QIcon( hbqt_par_QString( 2 ) ) : *hbqt_par_QIcon( 2 )) );
}

/* void setSelected ( bool select ) */
HB_FUNC( QT_QTABLEWIDGETITEM_SETSELECTED )
{
   QTableWidgetItem * p = hbqt_par_QTableWidgetItem( 1 );
   if( p )
      ( p )->setSelected( hb_parl( 2 ) );
}

/* void setSizeHint ( const QSize & size ) */
HB_FUNC( QT_QTABLEWIDGETITEM_SETSIZEHINT )
{
   QTableWidgetItem * p = hbqt_par_QTableWidgetItem( 1 );
   if( p )
      ( p )->setSizeHint( *hbqt_par_QSize( 2 ) );
}

/* void setStatusTip ( const QString & statusTip ) */
HB_FUNC( QT_QTABLEWIDGETITEM_SETSTATUSTIP )
{
   QTableWidgetItem * p = hbqt_par_QTableWidgetItem( 1 );
   if( p )
   {
      void * pText;
      ( p )->setStatusTip( hb_parstr_utf8( 2, &pText, NULL ) );
      hb_strfree( pText );
   }
}

/* void setText ( const QString & text ) */
HB_FUNC( QT_QTABLEWIDGETITEM_SETTEXT )
{
   QTableWidgetItem * p = hbqt_par_QTableWidgetItem( 1 );
   if( p )
   {
      void * pText;
      ( p )->setText( hb_parstr_utf8( 2, &pText, NULL ) );
      hb_strfree( pText );
   }
}

/* void setTextAlignment ( int alignment ) */
HB_FUNC( QT_QTABLEWIDGETITEM_SETTEXTALIGNMENT )
{
   QTableWidgetItem * p = hbqt_par_QTableWidgetItem( 1 );
   if( p )
      ( p )->setTextAlignment( hb_parni( 2 ) );
}

/* void setToolTip ( const QString & toolTip ) */
HB_FUNC( QT_QTABLEWIDGETITEM_SETTOOLTIP )
{
   QTableWidgetItem * p = hbqt_par_QTableWidgetItem( 1 );
   if( p )
   {
      void * pText;
      ( p )->setToolTip( hb_parstr_utf8( 2, &pText, NULL ) );
      hb_strfree( pText );
   }
}

/* void setWhatsThis ( const QString & whatsThis ) */
HB_FUNC( QT_QTABLEWIDGETITEM_SETWHATSTHIS )
{
   QTableWidgetItem * p = hbqt_par_QTableWidgetItem( 1 );
   if( p )
   {
      void * pText;
      ( p )->setWhatsThis( hb_parstr_utf8( 2, &pText, NULL ) );
      hb_strfree( pText );
   }
}

/* QSize sizeHint () const */
HB_FUNC( QT_QTABLEWIDGETITEM_SIZEHINT )
{
   QTableWidgetItem * p = hbqt_par_QTableWidgetItem( 1 );
   if( p )
      hb_retptrGC( hbqt_gcAllocate_QSize( new QSize( ( p )->sizeHint() ), true ) );
}

/* QString statusTip () const */
HB_FUNC( QT_QTABLEWIDGETITEM_STATUSTIP )
{
   QTableWidgetItem * p = hbqt_par_QTableWidgetItem( 1 );
   if( p )
      hb_retstr_utf8( ( p )->statusTip().toUtf8().data() );
}

/* QTableWidget * tableWidget () const */
HB_FUNC( QT_QTABLEWIDGETITEM_TABLEWIDGET )
{
   QTableWidgetItem * p = hbqt_par_QTableWidgetItem( 1 );
   if( p )
      hb_retptrGC( hbqt_gcAllocate_QTableWidget( ( p )->tableWidget(), false ) );
}

/* QString text () const */
HB_FUNC( QT_QTABLEWIDGETITEM_TEXT )
{
   QTableWidgetItem * p = hbqt_par_QTableWidgetItem( 1 );
   if( p )
      hb_retstr_utf8( ( p )->text().toUtf8().data() );
}

/* int textAlignment () const */
HB_FUNC( QT_QTABLEWIDGETITEM_TEXTALIGNMENT )
{
   QTableWidgetItem * p = hbqt_par_QTableWidgetItem( 1 );
   if( p )
      hb_retni( ( p )->textAlignment() );
}

/* QString toolTip () const */
HB_FUNC( QT_QTABLEWIDGETITEM_TOOLTIP )
{
   QTableWidgetItem * p = hbqt_par_QTableWidgetItem( 1 );
   if( p )
      hb_retstr_utf8( ( p )->toolTip().toUtf8().data() );
}

/* int type () const */
HB_FUNC( QT_QTABLEWIDGETITEM_TYPE )
{
   QTableWidgetItem * p = hbqt_par_QTableWidgetItem( 1 );
   if( p )
      hb_retni( ( p )->type() );
}

/* QString whatsThis () const */
HB_FUNC( QT_QTABLEWIDGETITEM_WHATSTHIS )
{
   QTableWidgetItem * p = hbqt_par_QTableWidgetItem( 1 );
   if( p )
      hb_retstr_utf8( ( p )->whatsThis().toUtf8().data() );
}

/* virtual void write ( QDataStream & out ) const */
HB_FUNC( QT_QTABLEWIDGETITEM_WRITE )
{
   QTableWidgetItem * p = hbqt_par_QTableWidgetItem( 1 );
   if( p )
      ( p )->write( *hbqt_par_QDataStream( 2 ) );
}


#endif /* #if QT_VERSION >= 0x040500 */
