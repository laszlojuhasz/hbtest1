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
 *  enum InsertPolicy { NoInsert, InsertAtTop, InsertAtCurrent, InsertAtBottom, ..., InsertAlphabetically }
 *  enum SizeAdjustPolicy { AdjustToContents, AdjustToContentsOnFirstShow, AdjustToMinimumContentsLength, AdjustToMinimumContentsLengthWithIcon }
 */

/*
 *  Constructed[ 60/60 [ 100.00% ] ]
 *
 */

#include <QtCore/QPointer>

#include <QtGui/QComboBox>


/*
 * QComboBox ( QWidget * parent = 0 )
 * ~QComboBox ()
 */

typedef struct
{
   QPointer< QComboBox > ph;
   bool bNew;
   PHBQT_GC_FUNC func;
   int type;
} HBQT_GC_T_QComboBox;

HBQT_GC_FUNC( hbqt_gcRelease_QComboBox )
{
   HBQT_GC_T_QComboBox * p = ( HBQT_GC_T_QComboBox * ) Cargo;

   if( p )
   {
      if( p->bNew && p->ph )
      {
         QComboBox * ph = p->ph;
         const QMetaObject * m = ( ph )->metaObject();
         if( ( QString ) m->className() != ( QString ) "QObject" )
            delete ( p->ph );
      }
      p->ph = NULL;
   }
}

void * hbqt_gcAllocate_QComboBox( void * pObj, bool bNew )
{
   HBQT_GC_T_QComboBox * p = ( HBQT_GC_T_QComboBox * ) hb_gcAllocate( sizeof( HBQT_GC_T_QComboBox ), hbqt_gcFuncs() );

   new( & p->ph ) QPointer< QComboBox >( ( QComboBox * ) pObj );
   p->bNew = bNew;
   p->func = hbqt_gcRelease_QComboBox;
   p->type = HBQT_TYPE_QComboBox;

   return p;
}

HB_FUNC( QT_QCOMBOBOX )
{
   QComboBox * pObj = NULL;

   pObj = new QComboBox( hbqt_par_QWidget( 1 ) ) ;

   hb_retptrGC( hbqt_gcAllocate_QComboBox( ( void * ) pObj, true ) );
}

/* QComboBox ( QWidget * parent = 0 ) */
HB_FUNC( QT_QCOMBOBOX_QCOMBOBOX )
{
   hb_retptrGC( hbqt_gcAllocate_QComboBox( new QComboBox( hbqt_par_QWidget( 2 ) ), true ) );
}

/* void addItem ( const QString & text, const QVariant & userData = QVariant() ) */
HB_FUNC( QT_QCOMBOBOX_ADDITEM )
{
   QComboBox * p = hbqt_par_QComboBox( 1 );
   if( p )
   {
      void * pText;
      ( p )->addItem( hb_parstr_utf8( 2, &pText, NULL ), ( HB_ISOBJECT( 3 ) ? *hbqt_par_QVariant( 3 ) : QVariant() ) );
      hb_strfree( pText );
   }
}

/* void addItem ( const QIcon & icon, const QString & text, const QVariant & userData = QVariant() ) */
HB_FUNC( QT_QCOMBOBOX_ADDITEM_1 )
{
   QComboBox * p = hbqt_par_QComboBox( 1 );
   if( p )
   {
      void * pText;
      ( p )->addItem( ( HB_ISCHAR( 2 ) ? QIcon( hbqt_par_QString( 2 ) ) : *hbqt_par_QIcon( 2 )), hb_parstr_utf8( 3, &pText, NULL ), ( HB_ISOBJECT( 4 ) ? *hbqt_par_QVariant( 4 ) : QVariant() ) );
      hb_strfree( pText );
   }
}

/* void addItems ( const QStringList & texts ) */
HB_FUNC( QT_QCOMBOBOX_ADDITEMS )
{
   QComboBox * p = hbqt_par_QComboBox( 1 );
   if( p )
      ( p )->addItems( *hbqt_par_QStringList( 2 ) );
}

/* QCompleter * completer () const */
HB_FUNC( QT_QCOMBOBOX_COMPLETER )
{
   QComboBox * p = hbqt_par_QComboBox( 1 );
   if( p )
      hb_retptrGC( hbqt_gcAllocate_QCompleter( ( p )->completer(), false ) );
}

/* int count () const */
HB_FUNC( QT_QCOMBOBOX_COUNT )
{
   QComboBox * p = hbqt_par_QComboBox( 1 );
   if( p )
      hb_retni( ( p )->count() );
}

/* int currentIndex () const */
HB_FUNC( QT_QCOMBOBOX_CURRENTINDEX )
{
   QComboBox * p = hbqt_par_QComboBox( 1 );
   if( p )
      hb_retni( ( p )->currentIndex() );
}

/* QString currentText () const */
HB_FUNC( QT_QCOMBOBOX_CURRENTTEXT )
{
   QComboBox * p = hbqt_par_QComboBox( 1 );
   if( p )
      hb_retstr_utf8( ( p )->currentText().toUtf8().data() );
}

/* bool duplicatesEnabled () const */
HB_FUNC( QT_QCOMBOBOX_DUPLICATESENABLED )
{
   QComboBox * p = hbqt_par_QComboBox( 1 );
   if( p )
      hb_retl( ( p )->duplicatesEnabled() );
}

/* int findData ( const QVariant & data, int role = Qt::UserRole, Qt::MatchFlags flags = Qt::MatchExactly | Qt::MatchCaseSensitive ) const */
HB_FUNC( QT_QCOMBOBOX_FINDDATA )
{
   QComboBox * p = hbqt_par_QComboBox( 1 );
   if( p )
      hb_retni( ( p )->findData( *hbqt_par_QVariant( 2 ), hb_parnidef( 3, Qt::UserRole ), ( HB_ISNUM( 4 ) ? ( Qt::MatchFlags ) hb_parni( 4 ) : ( Qt::MatchFlags ) Qt::MatchExactly | Qt::MatchCaseSensitive ) ) );
}

/* int findText ( const QString & text, Qt::MatchFlags flags = Qt::MatchExactly | Qt::MatchCaseSensitive ) const */
HB_FUNC( QT_QCOMBOBOX_FINDTEXT )
{
   QComboBox * p = hbqt_par_QComboBox( 1 );
   if( p )
   {
      void * pText;
      hb_retni( ( p )->findText( hb_parstr_utf8( 2, &pText, NULL ), ( HB_ISNUM( 3 ) ? ( Qt::MatchFlags ) hb_parni( 3 ) : ( Qt::MatchFlags ) Qt::MatchExactly | Qt::MatchCaseSensitive ) ) );
      hb_strfree( pText );
   }
}

/* bool hasFrame () const */
HB_FUNC( QT_QCOMBOBOX_HASFRAME )
{
   QComboBox * p = hbqt_par_QComboBox( 1 );
   if( p )
      hb_retl( ( p )->hasFrame() );
}

/* virtual void hidePopup () */
HB_FUNC( QT_QCOMBOBOX_HIDEPOPUP )
{
   QComboBox * p = hbqt_par_QComboBox( 1 );
   if( p )
      ( p )->hidePopup();
}

/* QSize iconSize () const */
HB_FUNC( QT_QCOMBOBOX_ICONSIZE )
{
   QComboBox * p = hbqt_par_QComboBox( 1 );
   if( p )
      hb_retptrGC( hbqt_gcAllocate_QSize( new QSize( ( p )->iconSize() ), true ) );
}

/* void insertItem ( int index, const QString & text, const QVariant & userData = QVariant() ) */
HB_FUNC( QT_QCOMBOBOX_INSERTITEM )
{
   QComboBox * p = hbqt_par_QComboBox( 1 );
   if( p )
   {
      void * pText;
      ( p )->insertItem( hb_parni( 2 ), hb_parstr_utf8( 3, &pText, NULL ), ( HB_ISOBJECT( 4 ) ? *hbqt_par_QVariant( 4 ) : QVariant() ) );
      hb_strfree( pText );
   }
}

/* void insertItem ( int index, const QIcon & icon, const QString & text, const QVariant & userData = QVariant() ) */
HB_FUNC( QT_QCOMBOBOX_INSERTITEM_1 )
{
   QComboBox * p = hbqt_par_QComboBox( 1 );
   if( p )
   {
      void * pText;
      ( p )->insertItem( hb_parni( 2 ), ( HB_ISCHAR( 3 ) ? QIcon( hbqt_par_QString( 3 ) ) : *hbqt_par_QIcon( 3 )), hb_parstr_utf8( 4, &pText, NULL ), ( HB_ISOBJECT( 5 ) ? *hbqt_par_QVariant( 5 ) : QVariant() ) );
      hb_strfree( pText );
   }
}

/* void insertItems ( int index, const QStringList & list ) */
HB_FUNC( QT_QCOMBOBOX_INSERTITEMS )
{
   QComboBox * p = hbqt_par_QComboBox( 1 );
   if( p )
      ( p )->insertItems( hb_parni( 2 ), *hbqt_par_QStringList( 3 ) );
}

/* InsertPolicy insertPolicy () const */
HB_FUNC( QT_QCOMBOBOX_INSERTPOLICY )
{
   QComboBox * p = hbqt_par_QComboBox( 1 );
   if( p )
      hb_retni( ( QComboBox::InsertPolicy ) ( p )->insertPolicy() );
}

/* void insertSeparator ( int index ) */
HB_FUNC( QT_QCOMBOBOX_INSERTSEPARATOR )
{
   QComboBox * p = hbqt_par_QComboBox( 1 );
   if( p )
      ( p )->insertSeparator( hb_parni( 2 ) );
}

/* bool isEditable () const */
HB_FUNC( QT_QCOMBOBOX_ISEDITABLE )
{
   QComboBox * p = hbqt_par_QComboBox( 1 );
   if( p )
      hb_retl( ( p )->isEditable() );
}

/* QVariant itemData ( int index, int role = Qt::UserRole ) const */
HB_FUNC( QT_QCOMBOBOX_ITEMDATA )
{
   QComboBox * p = hbqt_par_QComboBox( 1 );
   if( p )
      hb_retptrGC( hbqt_gcAllocate_QVariant( new QVariant( ( p )->itemData( hb_parni( 2 ), hb_parnidef( 3, Qt::UserRole ) ) ), true ) );
}

/* QAbstractItemDelegate * itemDelegate () const */
HB_FUNC( QT_QCOMBOBOX_ITEMDELEGATE )
{
   QComboBox * p = hbqt_par_QComboBox( 1 );
   if( p )
      hb_retptrGC( hbqt_gcAllocate_QAbstractItemDelegate( ( p )->itemDelegate(), false ) );
}

/* QIcon itemIcon ( int index ) const */
HB_FUNC( QT_QCOMBOBOX_ITEMICON )
{
   QComboBox * p = hbqt_par_QComboBox( 1 );
   if( p )
      hb_retptrGC( hbqt_gcAllocate_QIcon( new QIcon( ( p )->itemIcon( hb_parni( 2 ) ) ), true ) );
}

/* QString itemText ( int index ) const */
HB_FUNC( QT_QCOMBOBOX_ITEMTEXT )
{
   QComboBox * p = hbqt_par_QComboBox( 1 );
   if( p )
      hb_retstr_utf8( ( p )->itemText( hb_parni( 2 ) ).toUtf8().data() );
}

/* QLineEdit * lineEdit () const */
HB_FUNC( QT_QCOMBOBOX_LINEEDIT )
{
   QComboBox * p = hbqt_par_QComboBox( 1 );
   if( p )
      hb_retptrGC( hbqt_gcAllocate_QLineEdit( ( p )->lineEdit(), false ) );
}

/* int maxCount () const */
HB_FUNC( QT_QCOMBOBOX_MAXCOUNT )
{
   QComboBox * p = hbqt_par_QComboBox( 1 );
   if( p )
      hb_retni( ( p )->maxCount() );
}

/* int maxVisibleItems () const */
HB_FUNC( QT_QCOMBOBOX_MAXVISIBLEITEMS )
{
   QComboBox * p = hbqt_par_QComboBox( 1 );
   if( p )
      hb_retni( ( p )->maxVisibleItems() );
}

/* int minimumContentsLength () const */
HB_FUNC( QT_QCOMBOBOX_MINIMUMCONTENTSLENGTH )
{
   QComboBox * p = hbqt_par_QComboBox( 1 );
   if( p )
      hb_retni( ( p )->minimumContentsLength() );
}

/* QAbstractItemModel * model () const */
HB_FUNC( QT_QCOMBOBOX_MODEL )
{
   QComboBox * p = hbqt_par_QComboBox( 1 );
   if( p )
      hb_retptrGC( hbqt_gcAllocate_QAbstractItemModel( ( p )->model(), false ) );
}

/* int modelColumn () const */
HB_FUNC( QT_QCOMBOBOX_MODELCOLUMN )
{
   QComboBox * p = hbqt_par_QComboBox( 1 );
   if( p )
      hb_retni( ( p )->modelColumn() );
}

/* void removeItem ( int index ) */
HB_FUNC( QT_QCOMBOBOX_REMOVEITEM )
{
   QComboBox * p = hbqt_par_QComboBox( 1 );
   if( p )
      ( p )->removeItem( hb_parni( 2 ) );
}

/* QModelIndex rootModelIndex () const */
HB_FUNC( QT_QCOMBOBOX_ROOTMODELINDEX )
{
   QComboBox * p = hbqt_par_QComboBox( 1 );
   if( p )
      hb_retptrGC( hbqt_gcAllocate_QModelIndex( new QModelIndex( ( p )->rootModelIndex() ), true ) );
}

/* void setCompleter ( QCompleter * completer ) */
HB_FUNC( QT_QCOMBOBOX_SETCOMPLETER )
{
   QComboBox * p = hbqt_par_QComboBox( 1 );
   if( p )
      ( p )->setCompleter( hbqt_par_QCompleter( 2 ) );
}

/* void setDuplicatesEnabled ( bool enable ) */
HB_FUNC( QT_QCOMBOBOX_SETDUPLICATESENABLED )
{
   QComboBox * p = hbqt_par_QComboBox( 1 );
   if( p )
      ( p )->setDuplicatesEnabled( hb_parl( 2 ) );
}

/* void setEditable ( bool editable ) */
HB_FUNC( QT_QCOMBOBOX_SETEDITABLE )
{
   QComboBox * p = hbqt_par_QComboBox( 1 );
   if( p )
      ( p )->setEditable( hb_parl( 2 ) );
}

/* void setFrame ( bool ) */
HB_FUNC( QT_QCOMBOBOX_SETFRAME )
{
   QComboBox * p = hbqt_par_QComboBox( 1 );
   if( p )
      ( p )->setFrame( hb_parl( 2 ) );
}

/* void setIconSize ( const QSize & size ) */
HB_FUNC( QT_QCOMBOBOX_SETICONSIZE )
{
   QComboBox * p = hbqt_par_QComboBox( 1 );
   if( p )
      ( p )->setIconSize( *hbqt_par_QSize( 2 ) );
}

/* void setInsertPolicy ( InsertPolicy policy ) */
HB_FUNC( QT_QCOMBOBOX_SETINSERTPOLICY )
{
   QComboBox * p = hbqt_par_QComboBox( 1 );
   if( p )
      ( p )->setInsertPolicy( ( QComboBox::InsertPolicy ) hb_parni( 2 ) );
}

/* void setItemData ( int index, const QVariant & value, int role = Qt::UserRole ) */
HB_FUNC( QT_QCOMBOBOX_SETITEMDATA )
{
   QComboBox * p = hbqt_par_QComboBox( 1 );
   if( p )
      ( p )->setItemData( hb_parni( 2 ), *hbqt_par_QVariant( 3 ), hb_parnidef( 4, Qt::UserRole ) );
}

/* void setItemDelegate ( QAbstractItemDelegate * delegate ) */
HB_FUNC( QT_QCOMBOBOX_SETITEMDELEGATE )
{
   QComboBox * p = hbqt_par_QComboBox( 1 );
   if( p )
      ( p )->setItemDelegate( hbqt_par_QAbstractItemDelegate( 2 ) );
}

/* void setItemIcon ( int index, const QIcon & icon ) */
HB_FUNC( QT_QCOMBOBOX_SETITEMICON )
{
   QComboBox * p = hbqt_par_QComboBox( 1 );
   if( p )
      ( p )->setItemIcon( hb_parni( 2 ), ( HB_ISCHAR( 3 ) ? QIcon( hbqt_par_QString( 3 ) ) : *hbqt_par_QIcon( 3 )) );
}

/* void setItemText ( int index, const QString & text ) */
HB_FUNC( QT_QCOMBOBOX_SETITEMTEXT )
{
   QComboBox * p = hbqt_par_QComboBox( 1 );
   if( p )
   {
      void * pText;
      ( p )->setItemText( hb_parni( 2 ), hb_parstr_utf8( 3, &pText, NULL ) );
      hb_strfree( pText );
   }
}

/* void setLineEdit ( QLineEdit * edit ) */
HB_FUNC( QT_QCOMBOBOX_SETLINEEDIT )
{
   QComboBox * p = hbqt_par_QComboBox( 1 );
   if( p )
      ( p )->setLineEdit( hbqt_par_QLineEdit( 2 ) );
}

/* void setMaxCount ( int max ) */
HB_FUNC( QT_QCOMBOBOX_SETMAXCOUNT )
{
   QComboBox * p = hbqt_par_QComboBox( 1 );
   if( p )
      ( p )->setMaxCount( hb_parni( 2 ) );
}

/* void setMaxVisibleItems ( int maxItems ) */
HB_FUNC( QT_QCOMBOBOX_SETMAXVISIBLEITEMS )
{
   QComboBox * p = hbqt_par_QComboBox( 1 );
   if( p )
      ( p )->setMaxVisibleItems( hb_parni( 2 ) );
}

/* void setMinimumContentsLength ( int characters ) */
HB_FUNC( QT_QCOMBOBOX_SETMINIMUMCONTENTSLENGTH )
{
   QComboBox * p = hbqt_par_QComboBox( 1 );
   if( p )
      ( p )->setMinimumContentsLength( hb_parni( 2 ) );
}

/* void setModel ( QAbstractItemModel * model ) */
HB_FUNC( QT_QCOMBOBOX_SETMODEL )
{
   QComboBox * p = hbqt_par_QComboBox( 1 );
   if( p )
      ( p )->setModel( hbqt_par_QAbstractItemModel( 2 ) );
}

/* void setModelColumn ( int visibleColumn ) */
HB_FUNC( QT_QCOMBOBOX_SETMODELCOLUMN )
{
   QComboBox * p = hbqt_par_QComboBox( 1 );
   if( p )
      ( p )->setModelColumn( hb_parni( 2 ) );
}

/* void setRootModelIndex ( const QModelIndex & index ) */
HB_FUNC( QT_QCOMBOBOX_SETROOTMODELINDEX )
{
   QComboBox * p = hbqt_par_QComboBox( 1 );
   if( p )
      ( p )->setRootModelIndex( *hbqt_par_QModelIndex( 2 ) );
}

/* void setSizeAdjustPolicy ( SizeAdjustPolicy policy ) */
HB_FUNC( QT_QCOMBOBOX_SETSIZEADJUSTPOLICY )
{
   QComboBox * p = hbqt_par_QComboBox( 1 );
   if( p )
      ( p )->setSizeAdjustPolicy( ( QComboBox::SizeAdjustPolicy ) hb_parni( 2 ) );
}

/* void setValidator ( const QValidator * validator ) */
HB_FUNC( QT_QCOMBOBOX_SETVALIDATOR )
{
   QComboBox * p = hbqt_par_QComboBox( 1 );
   if( p )
      ( p )->setValidator( hbqt_par_QValidator( 2 ) );
}

/* void setView ( QAbstractItemView * itemView )   [*D=1*] */
HB_FUNC( QT_QCOMBOBOX_SETVIEW )
{
   QComboBox * p = hbqt_par_QComboBox( 1 );
   if( p )
   {
      hbqt_detachgcpointer( 2 );
      ( p )->setView( hbqt_par_QAbstractItemView( 2 ) );
   }
}

/* virtual void showPopup () */
HB_FUNC( QT_QCOMBOBOX_SHOWPOPUP )
{
   QComboBox * p = hbqt_par_QComboBox( 1 );
   if( p )
      ( p )->showPopup();
}

/* SizeAdjustPolicy sizeAdjustPolicy () const */
HB_FUNC( QT_QCOMBOBOX_SIZEADJUSTPOLICY )
{
   QComboBox * p = hbqt_par_QComboBox( 1 );
   if( p )
      hb_retni( ( QComboBox::SizeAdjustPolicy ) ( p )->sizeAdjustPolicy() );
}

/* virtual const QValidator * validator () const */
HB_FUNC( QT_QCOMBOBOX_VALIDATOR )
{
   QComboBox * p = hbqt_par_QComboBox( 1 );
   if( p )
      hb_retptrGC( hbqt_gcAllocate_QValidator( ( void * ) ( p )->validator(), false ) );
}

/* QAbstractItemView * view () const */
HB_FUNC( QT_QCOMBOBOX_VIEW )
{
   QComboBox * p = hbqt_par_QComboBox( 1 );
   if( p )
      hb_retptrGC( hbqt_gcAllocate_QAbstractItemView( ( p )->view(), false ) );
}

/* void clear () */
HB_FUNC( QT_QCOMBOBOX_CLEAR )
{
   QComboBox * p = hbqt_par_QComboBox( 1 );
   if( p )
      ( p )->clear();
}

/* void clearEditText () */
HB_FUNC( QT_QCOMBOBOX_CLEAREDITTEXT )
{
   QComboBox * p = hbqt_par_QComboBox( 1 );
   if( p )
      ( p )->clearEditText();
}

/* void setCurrentIndex ( int index ) */
HB_FUNC( QT_QCOMBOBOX_SETCURRENTINDEX )
{
   QComboBox * p = hbqt_par_QComboBox( 1 );
   if( p )
      ( p )->setCurrentIndex( hb_parni( 2 ) );
}

/* void setEditText ( const QString & text ) */
HB_FUNC( QT_QCOMBOBOX_SETEDITTEXT )
{
   QComboBox * p = hbqt_par_QComboBox( 1 );
   if( p )
   {
      void * pText;
      ( p )->setEditText( hb_parstr_utf8( 2, &pText, NULL ) );
      hb_strfree( pText );
   }
}


#endif /* #if QT_VERSION >= 0x040500 */
