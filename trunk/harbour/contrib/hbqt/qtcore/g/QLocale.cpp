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

#if QT_VERSION >= 0x040500

/*
 *  enum Country { AnyCountry, Afghanistan, Albania, Algeria, ..., Zimbabwe }
 *  enum FormatType { LongFormat, ShortFormat, NarrowFormat }
 *  enum Language { C, Abkhazian, Afan, Afar, ..., Chewa }
 *  enum MeasurementSystem { MetricSystem, ImperialSystem }
 *  enum NumberOption { OmitGroupSeparator, RejectGroupSeparator }
 *  flags NumberOptions
 */

/*
 *  Constructed[ 55/55 [ 100.00% ] ]
 *
 *
 *  *** Commented out protostypes ***
 *
 *  //QList<Country> countriesForLanguage ( Language language )
 */

#include <QtCore/QPointer>

#include <QtCore/QLocale>
#include <QtCore/QDate>

/* QLocale ()
 * QLocale ( const QString & name )
 * QLocale ( QLocale::Language language, QLocale::Country country = AnyCountry )
 * QLocale ( const QLocale & other )
 */

typedef struct
{
   QLocale * ph;
   bool bNew;
   PHBQT_GC_FUNC func;
   int type;
} HBQT_GC_T_QLocale;

HBQT_GC_FUNC( hbqt_gcRelease_QLocale )
{
   HBQT_GC_T * p = ( HBQT_GC_T * ) Cargo;

   if( p && p->bNew )
   {
      if( p->ph )
      {
         delete ( ( QLocale * ) p->ph );
         p->ph = NULL;
      }
      else
         p->ph = NULL;
   }
   else
      p->ph = NULL;
}

void * hbqt_gcAllocate_QLocale( void * pObj, bool bNew )
{
   HBQT_GC_T * p = ( HBQT_GC_T * ) hb_gcAllocate( sizeof( HBQT_GC_T ), hbqt_gcFuncs() );

   p->ph = ( QLocale * ) pObj;
   p->bNew = bNew;
   p->func = hbqt_gcRelease_QLocale;
   p->type = HBQT_TYPE_QLocale;

   return p;
}

HB_FUNC( QT_QLOCALE )
{
   QLocale * pObj = NULL;

   pObj = new QLocale() ;

   hb_retptrGC( hbqt_gcAllocate_QLocale( ( void * ) pObj, true ) );
}

/* QString amText () const */
HB_FUNC( QT_QLOCALE_AMTEXT )
{
   QLocale * p = hbqt_par_QLocale( 1 );
   if( p )
      hb_retstr_utf8( ( p )->amText().toUtf8().data() );
}

/* Country country () const */
HB_FUNC( QT_QLOCALE_COUNTRY )
{
   QLocale * p = hbqt_par_QLocale( 1 );
   if( p )
      hb_retni( ( QLocale::Country ) ( p )->country() );
}

/* QString dateFormat ( FormatType format = LongFormat ) const */
HB_FUNC( QT_QLOCALE_DATEFORMAT )
{
   QLocale * p = hbqt_par_QLocale( 1 );
   if( p )
      hb_retstr_utf8( ( p )->dateFormat( ( HB_ISNUM( 2 ) ? ( QLocale::FormatType ) hb_parni( 2 ) : ( QLocale::FormatType ) QLocale::LongFormat ) ).toUtf8().data() );
}

/* QString dateTimeFormat ( FormatType format = LongFormat ) const */
HB_FUNC( QT_QLOCALE_DATETIMEFORMAT )
{
   QLocale * p = hbqt_par_QLocale( 1 );
   if( p )
      hb_retstr_utf8( ( p )->dateTimeFormat( ( HB_ISNUM( 2 ) ? ( QLocale::FormatType ) hb_parni( 2 ) : ( QLocale::FormatType ) QLocale::LongFormat ) ).toUtf8().data() );
}

/* QString dayName ( int day, FormatType type = LongFormat ) const */
HB_FUNC( QT_QLOCALE_DAYNAME )
{
   QLocale * p = hbqt_par_QLocale( 1 );
   if( p )
      hb_retstr_utf8( ( p )->dayName( hb_parni( 2 ), ( HB_ISNUM( 3 ) ? ( QLocale::FormatType ) hb_parni( 3 ) : ( QLocale::FormatType ) QLocale::LongFormat ) ).toUtf8().data() );
}

/* QChar decimalPoint () const */
HB_FUNC( QT_QLOCALE_DECIMALPOINT )
{
   QLocale * p = hbqt_par_QLocale( 1 );
   if( p )
      hb_retptrGC( hbqt_gcAllocate_QChar( new QChar( ( p )->decimalPoint() ), true ) );
}

/* QChar exponential () const */
HB_FUNC( QT_QLOCALE_EXPONENTIAL )
{
   QLocale * p = hbqt_par_QLocale( 1 );
   if( p )
      hb_retptrGC( hbqt_gcAllocate_QChar( new QChar( ( p )->exponential() ), true ) );
}

/* QChar groupSeparator () const */
HB_FUNC( QT_QLOCALE_GROUPSEPARATOR )
{
   QLocale * p = hbqt_par_QLocale( 1 );
   if( p )
      hb_retptrGC( hbqt_gcAllocate_QChar( new QChar( ( p )->groupSeparator() ), true ) );
}

/* Language language () const */
HB_FUNC( QT_QLOCALE_LANGUAGE )
{
   QLocale * p = hbqt_par_QLocale( 1 );
   if( p )
      hb_retni( ( QLocale::Language ) ( p )->language() );
}

/* MeasurementSystem measurementSystem () const */
HB_FUNC( QT_QLOCALE_MEASUREMENTSYSTEM )
{
   QLocale * p = hbqt_par_QLocale( 1 );
   if( p )
      hb_retni( ( QLocale::MeasurementSystem ) ( p )->measurementSystem() );
}

/* QString monthName ( int month, FormatType type = LongFormat ) const */
HB_FUNC( QT_QLOCALE_MONTHNAME )
{
   QLocale * p = hbqt_par_QLocale( 1 );
   if( p )
      hb_retstr_utf8( ( p )->monthName( hb_parni( 2 ), ( HB_ISNUM( 3 ) ? ( QLocale::FormatType ) hb_parni( 3 ) : ( QLocale::FormatType ) QLocale::LongFormat ) ).toUtf8().data() );
}

/* QString name () const */
HB_FUNC( QT_QLOCALE_NAME )
{
   QLocale * p = hbqt_par_QLocale( 1 );
   if( p )
      hb_retstr_utf8( ( p )->name().toUtf8().data() );
}

/* QChar negativeSign () const */
HB_FUNC( QT_QLOCALE_NEGATIVESIGN )
{
   QLocale * p = hbqt_par_QLocale( 1 );
   if( p )
      hb_retptrGC( hbqt_gcAllocate_QChar( new QChar( ( p )->negativeSign() ), true ) );
}

/* NumberOptions numberOptions () const */
HB_FUNC( QT_QLOCALE_NUMBEROPTIONS )
{
   QLocale * p = hbqt_par_QLocale( 1 );
   if( p )
      hb_retni( ( QLocale::NumberOptions ) ( p )->numberOptions() );
}

/* QChar percent () const */
HB_FUNC( QT_QLOCALE_PERCENT )
{
   QLocale * p = hbqt_par_QLocale( 1 );
   if( p )
      hb_retptrGC( hbqt_gcAllocate_QChar( new QChar( ( p )->percent() ), true ) );
}

/* QString pmText () const */
HB_FUNC( QT_QLOCALE_PMTEXT )
{
   QLocale * p = hbqt_par_QLocale( 1 );
   if( p )
      hb_retstr_utf8( ( p )->pmText().toUtf8().data() );
}

/* QChar positiveSign () const */
HB_FUNC( QT_QLOCALE_POSITIVESIGN )
{
   QLocale * p = hbqt_par_QLocale( 1 );
   if( p )
      hb_retptrGC( hbqt_gcAllocate_QChar( new QChar( ( p )->positiveSign() ), true ) );
}

/* void setNumberOptions ( NumberOptions options ) */
HB_FUNC( QT_QLOCALE_SETNUMBEROPTIONS )
{
   QLocale * p = hbqt_par_QLocale( 1 );
   if( p )
      ( p )->setNumberOptions( ( QLocale::NumberOptions ) hb_parni( 2 ) );
}

/* QString standaloneDayName ( int day, FormatType type = LongFormat ) const */
HB_FUNC( QT_QLOCALE_STANDALONEDAYNAME )
{
   QLocale * p = hbqt_par_QLocale( 1 );
   if( p )
      hb_retstr_utf8( ( p )->standaloneDayName( hb_parni( 2 ), ( HB_ISNUM( 3 ) ? ( QLocale::FormatType ) hb_parni( 3 ) : ( QLocale::FormatType ) QLocale::LongFormat ) ).toUtf8().data() );
}

/* QString standaloneMonthName ( int month, FormatType type = LongFormat ) const */
HB_FUNC( QT_QLOCALE_STANDALONEMONTHNAME )
{
   QLocale * p = hbqt_par_QLocale( 1 );
   if( p )
      hb_retstr_utf8( ( p )->standaloneMonthName( hb_parni( 2 ), ( HB_ISNUM( 3 ) ? ( QLocale::FormatType ) hb_parni( 3 ) : ( QLocale::FormatType ) QLocale::LongFormat ) ).toUtf8().data() );
}

/* QString timeFormat ( FormatType format = LongFormat ) const */
HB_FUNC( QT_QLOCALE_TIMEFORMAT )
{
   QLocale * p = hbqt_par_QLocale( 1 );
   if( p )
      hb_retstr_utf8( ( p )->timeFormat( ( HB_ISNUM( 2 ) ? ( QLocale::FormatType ) hb_parni( 2 ) : ( QLocale::FormatType ) QLocale::LongFormat ) ).toUtf8().data() );
}

/* QDate toDate ( const QString & string, FormatType format = LongFormat ) const */
HB_FUNC( QT_QLOCALE_TODATE )
{
   QLocale * p = hbqt_par_QLocale( 1 );
   if( p )
   {
      void * pText;
      hb_retptrGC( hbqt_gcAllocate_QDate( new QDate( ( p )->toDate( hb_parstr_utf8( 2, &pText, NULL ), ( HB_ISNUM( 3 ) ? ( QLocale::FormatType ) hb_parni( 3 ) : ( QLocale::FormatType ) QLocale::LongFormat ) ) ), true ) );
      hb_strfree( pText );
   }
}

/* QDate toDate ( const QString & string, const QString & format ) const */
HB_FUNC( QT_QLOCALE_TODATE_1 )
{
   QLocale * p = hbqt_par_QLocale( 1 );
   if( p )
   {
      void * pText;
      hb_retptrGC( hbqt_gcAllocate_QDate( new QDate( ( p )->toDate( hb_parstr_utf8( 2, &pText, NULL ), hb_parstr_utf8( 3, &pText, NULL ) ) ), true ) );
      hb_strfree( pText );
   }
}

/* QDateTime toDateTime ( const QString & string, FormatType format = LongFormat ) const */
HB_FUNC( QT_QLOCALE_TODATETIME )
{
   QLocale * p = hbqt_par_QLocale( 1 );
   if( p )
   {
      void * pText;
      hb_retptrGC( hbqt_gcAllocate_QDateTime( new QDateTime( ( p )->toDateTime( hb_parstr_utf8( 2, &pText, NULL ), ( HB_ISNUM( 3 ) ? ( QLocale::FormatType ) hb_parni( 3 ) : ( QLocale::FormatType ) QLocale::LongFormat ) ) ), true ) );
      hb_strfree( pText );
   }
}

/* QDateTime toDateTime ( const QString & string, const QString & format ) const */
HB_FUNC( QT_QLOCALE_TODATETIME_1 )
{
   QLocale * p = hbqt_par_QLocale( 1 );
   if( p )
   {
      void * pText;
      hb_retptrGC( hbqt_gcAllocate_QDateTime( new QDateTime( ( p )->toDateTime( hb_parstr_utf8( 2, &pText, NULL ), hb_parstr_utf8( 3, &pText, NULL ) ) ), true ) );
      hb_strfree( pText );
   }
}

/* double toDouble ( const QString & s, bool * ok = 0 ) const */
HB_FUNC( QT_QLOCALE_TODOUBLE )
{
   QLocale * p = hbqt_par_QLocale( 1 );
   bool iOk = 0;

   if( p )
   {
      void * pText;
      hb_retnd( ( p )->toDouble( hb_parstr_utf8( 2, &pText, NULL ), &iOk ) );
      hb_strfree( pText );
   }

   hb_stornl( iOk, 3 );
}

/* float toFloat ( const QString & s, bool * ok = 0 ) const */
HB_FUNC( QT_QLOCALE_TOFLOAT )
{
   QLocale * p = hbqt_par_QLocale( 1 );
   bool iOk = 0;

   if( p )
   {
      void * pText;
      hb_retnd( ( p )->toFloat( hb_parstr_utf8( 2, &pText, NULL ), &iOk ) );
      hb_strfree( pText );
   }

   hb_stornl( iOk, 3 );
}

/* int toInt ( const QString & s, bool * ok = 0, int base = 0 ) const */
HB_FUNC( QT_QLOCALE_TOINT )
{
   QLocale * p = hbqt_par_QLocale( 1 );
   bool iOk = 0;

   if( p )
   {
      void * pText;
      hb_retni( ( p )->toInt( hb_parstr_utf8( 2, &pText, NULL ), &iOk, hb_parni( 4 ) ) );
      hb_strfree( pText );
   }

   hb_stornl( iOk, 3 );
}

/* qlonglong toLongLong ( const QString & s, bool * ok = 0, int base = 0 ) const */
HB_FUNC( QT_QLOCALE_TOLONGLONG )
{
   QLocale * p = hbqt_par_QLocale( 1 );
   bool iOk = 0;

   if( p )
   {
      void * pText;
      hb_retnint( ( p )->toLongLong( hb_parstr_utf8( 2, &pText, NULL ), &iOk, hb_parni( 4 ) ) );
      hb_strfree( pText );
   }

   hb_stornl( iOk, 3 );
}

/* short toShort ( const QString & s, bool * ok = 0, int base = 0 ) const */
HB_FUNC( QT_QLOCALE_TOSHORT )
{
   QLocale * p = hbqt_par_QLocale( 1 );
   bool iOk = 0;

   if( p )
   {
      void * pText;
      hb_retni( ( p )->toShort( hb_parstr_utf8( 2, &pText, NULL ), &iOk, hb_parni( 4 ) ) );
      hb_strfree( pText );
   }

   hb_stornl( iOk, 3 );
}

/* QString toString ( qlonglong i ) const */
HB_FUNC( QT_QLOCALE_TOSTRING )
{
   QLocale * p = hbqt_par_QLocale( 1 );
   if( p )
      hb_retstr_utf8( ( p )->toString( ( qlonglong ) hb_parnint( 2 ) ).toUtf8().data() );
}

/* QString toString ( const QDate & date, const QString & format ) const */
HB_FUNC( QT_QLOCALE_TOSTRING_1 )
{
   QLocale * p = hbqt_par_QLocale( 1 );
   if( p )
   {
      void * pText;
      hb_retstr_utf8( ( p )->toString( *hbqt_par_QDate( 2 ), hb_parstr_utf8( 3, &pText, NULL ) ).toUtf8().data() );
      hb_strfree( pText );
   }
}

/* QString toString ( const QDate & date, FormatType format = LongFormat ) const */
HB_FUNC( QT_QLOCALE_TOSTRING_2 )
{
   QLocale * p = hbqt_par_QLocale( 1 );
   if( p )
      hb_retstr_utf8( ( p )->toString( *hbqt_par_QDate( 2 ), ( HB_ISNUM( 3 ) ? ( QLocale::FormatType ) hb_parni( 3 ) : ( QLocale::FormatType ) QLocale::LongFormat ) ).toUtf8().data() );
}

/* QString toString ( const QTime & time, const QString & format ) const */
HB_FUNC( QT_QLOCALE_TOSTRING_3 )
{
   QLocale * p = hbqt_par_QLocale( 1 );
   if( p )
   {
      void * pText;
      hb_retstr_utf8( ( p )->toString( *hbqt_par_QTime( 2 ), hb_parstr_utf8( 3, &pText, NULL ) ).toUtf8().data() );
      hb_strfree( pText );
   }
}

/* QString toString ( const QTime & time, FormatType format = LongFormat ) const */
HB_FUNC( QT_QLOCALE_TOSTRING_4 )
{
   QLocale * p = hbqt_par_QLocale( 1 );
   if( p )
      hb_retstr_utf8( ( p )->toString( *hbqt_par_QTime( 2 ), ( HB_ISNUM( 3 ) ? ( QLocale::FormatType ) hb_parni( 3 ) : ( QLocale::FormatType ) QLocale::LongFormat ) ).toUtf8().data() );
}

/* QString toString ( const QDateTime & dateTime, FormatType format = LongFormat ) const */
HB_FUNC( QT_QLOCALE_TOSTRING_5 )
{
   QLocale * p = hbqt_par_QLocale( 1 );
   if( p )
      hb_retstr_utf8( ( p )->toString( *hbqt_par_QDateTime( 2 ), ( HB_ISNUM( 3 ) ? ( QLocale::FormatType ) hb_parni( 3 ) : ( QLocale::FormatType ) QLocale::LongFormat ) ).toUtf8().data() );
}

/* QString toString ( const QDateTime & dateTime, const QString & format ) const */
HB_FUNC( QT_QLOCALE_TOSTRING_6 )
{
   QLocale * p = hbqt_par_QLocale( 1 );
   if( p )
   {
      void * pText;
      hb_retstr_utf8( ( p )->toString( *hbqt_par_QDateTime( 2 ), hb_parstr_utf8( 3, &pText, NULL ) ).toUtf8().data() );
      hb_strfree( pText );
   }
}

/* QString toString ( qulonglong i ) const */
HB_FUNC( QT_QLOCALE_TOSTRING_7 )
{
   QLocale * p = hbqt_par_QLocale( 1 );
   if( p )
      hb_retstr_utf8( ( p )->toString( ( qulonglong ) hb_parnint( 2 ) ).toUtf8().data() );
}

/* QString toString ( double i, char f = 'g', int prec = 6 ) const */
HB_FUNC( QT_QLOCALE_TOSTRING_8 )
{
   QLocale * p = hbqt_par_QLocale( 1 );
   if( p )
      hb_retstr_utf8( ( p )->toString( hb_parnd( 2 ), ( char ) hb_parni( 3 ), hb_parnidef( 4, 6 ) ).toUtf8().data() );
}

/* QString toString ( short i ) const */
HB_FUNC( QT_QLOCALE_TOSTRING_9 )
{
   QLocale * p = hbqt_par_QLocale( 1 );
   if( p )
      hb_retstr_utf8( ( p )->toString( hb_parni( 2 ) ).toUtf8().data() );
}

/* QString toString ( ushort i ) const */
HB_FUNC( QT_QLOCALE_TOSTRING_10 )
{
   QLocale * p = hbqt_par_QLocale( 1 );
   if( p )
      hb_retstr_utf8( ( p )->toString( hb_parni( 2 ) ).toUtf8().data() );
}

/* QString toString ( int i ) const */
HB_FUNC( QT_QLOCALE_TOSTRING_11 )
{
   QLocale * p = hbqt_par_QLocale( 1 );
   if( p )
      hb_retstr_utf8( ( p )->toString( hb_parni( 2 ) ).toUtf8().data() );
}

/* QString toString ( uint i ) const */
HB_FUNC( QT_QLOCALE_TOSTRING_12 )
{
   QLocale * p = hbqt_par_QLocale( 1 );
   if( p )
      hb_retstr_utf8( ( p )->toString( hb_parni( 2 ) ).toUtf8().data() );
}

/* QString toString ( float i, char f = 'g', int prec = 6 ) const */
HB_FUNC( QT_QLOCALE_TOSTRING_13 )
{
   QLocale * p = hbqt_par_QLocale( 1 );
   if( p )
      hb_retstr_utf8( ( p )->toString( hb_parnd( 2 ), ( char ) hb_parni( 3 ), hb_parnidef( 4, 6 ) ).toUtf8().data() );
}

/* QTime toTime ( const QString & string, FormatType format = LongFormat ) const */
HB_FUNC( QT_QLOCALE_TOTIME )
{
   QLocale * p = hbqt_par_QLocale( 1 );
   if( p )
   {
      void * pText;
      hb_retptrGC( hbqt_gcAllocate_QTime( new QTime( ( p )->toTime( hb_parstr_utf8( 2, &pText, NULL ), ( HB_ISNUM( 3 ) ? ( QLocale::FormatType ) hb_parni( 3 ) : ( QLocale::FormatType ) QLocale::LongFormat ) ) ), true ) );
      hb_strfree( pText );
   }
}

/* QTime toTime ( const QString & string, const QString & format ) const */
HB_FUNC( QT_QLOCALE_TOTIME_1 )
{
   QLocale * p = hbqt_par_QLocale( 1 );
   if( p )
   {
      void * pText;
      hb_retptrGC( hbqt_gcAllocate_QTime( new QTime( ( p )->toTime( hb_parstr_utf8( 2, &pText, NULL ), hb_parstr_utf8( 3, &pText, NULL ) ) ), true ) );
      hb_strfree( pText );
   }
}

/* uint toUInt ( const QString & s, bool * ok = 0, int base = 0 ) const */
HB_FUNC( QT_QLOCALE_TOUINT )
{
   QLocale * p = hbqt_par_QLocale( 1 );
   bool iOk = 0;

   if( p )
   {
      void * pText;
      hb_retni( ( p )->toUInt( hb_parstr_utf8( 2, &pText, NULL ), &iOk, hb_parni( 4 ) ) );
      hb_strfree( pText );
   }

   hb_stornl( iOk, 3 );
}

/* qlonglong toULongLong ( const QString & s, bool * ok = 0, int base = 0 ) const */
HB_FUNC( QT_QLOCALE_TOULONGLONG )
{
   QLocale * p = hbqt_par_QLocale( 1 );
   bool iOk = 0;

   if( p )
   {
      void * pText;
      hb_retnint( ( p )->toULongLong( hb_parstr_utf8( 2, &pText, NULL ), &iOk, hb_parni( 4 ) ) );
      hb_strfree( pText );
   }

   hb_stornl( iOk, 3 );
}

/* ushort toUShort ( const QString & s, bool * ok = 0, int base = 0 ) const */
HB_FUNC( QT_QLOCALE_TOUSHORT )
{
   QLocale * p = hbqt_par_QLocale( 1 );
   bool iOk = 0;

   if( p )
   {
      void * pText;
      hb_retni( ( p )->toUShort( hb_parstr_utf8( 2, &pText, NULL ), &iOk, hb_parni( 4 ) ) );
      hb_strfree( pText );
   }

   hb_stornl( iOk, 3 );
}

/* QChar zeroDigit () const */
HB_FUNC( QT_QLOCALE_ZERODIGIT )
{
   QLocale * p = hbqt_par_QLocale( 1 );
   if( p )
      hb_retptrGC( hbqt_gcAllocate_QChar( new QChar( ( p )->zeroDigit() ), true ) );
}

/* QLocale c () */
HB_FUNC( QT_QLOCALE_C )
{
   QLocale * p = hbqt_par_QLocale( 1 );
   if( p )
      hb_retptrGC( hbqt_gcAllocate_QLocale( new QLocale( ( p )->c() ), true ) );
}

/* QString countryToString ( Country country ) */
HB_FUNC( QT_QLOCALE_COUNTRYTOSTRING )
{
   QLocale * p = hbqt_par_QLocale( 1 );
   if( p )
      hb_retstr_utf8( ( p )->countryToString( ( QLocale::Country ) hb_parni( 2 ) ).toUtf8().data() );
}

/* QString languageToString ( Language language ) */
HB_FUNC( QT_QLOCALE_LANGUAGETOSTRING )
{
   QLocale * p = hbqt_par_QLocale( 1 );
   if( p )
      hb_retstr_utf8( ( p )->languageToString( ( QLocale::Language ) hb_parni( 2 ) ).toUtf8().data() );
}

/* void setDefault ( const QLocale & locale ) */
HB_FUNC( QT_QLOCALE_SETDEFAULT )
{
   QLocale * p = hbqt_par_QLocale( 1 );
   if( p )
      ( p )->setDefault( *hbqt_par_QLocale( 2 ) );
}

/* QLocale system () */
HB_FUNC( QT_QLOCALE_SYSTEM )
{
   QLocale * p = hbqt_par_QLocale( 1 );
   if( p )
      hb_retptrGC( hbqt_gcAllocate_QLocale( new QLocale( ( p )->system() ), true ) );
}


#endif /* #if QT_VERSION >= 0x040500 */
