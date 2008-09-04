/*
 * $Id$
 */

/*
 * Harbour Project source code:
 * Default RDD module
 *
 * Copyright 1999 Bruno Cantero <bruno@issnet.net>
 * Copyright 2004-2007 Przemyslaw Czerpak <druzus / at / priv.onet.pl>
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
 *
 * The following functions are added by
 *       Horacio Roldan <harbour_ar@yahoo.com.ar>
 * hb_waCloseAux()
 *
 */

#include <ctype.h>

#include "hbapi.h"
#include "hbapirdd.h"
#include "hbapiitm.h"
#include "hbapierr.h"
#include "hbapilng.h"
#include "hbvm.h"
#include "hbset.h"

/*
 * -- BASIC RDD METHODS --
 */

/*
 * Determine logical beginning of file.
 */
static ERRCODE hb_waBof( AREAP pArea, BOOL * pBof )
{
   HB_TRACE(HB_TR_DEBUG, ("hb_waBof(%p, %p)", pArea, pBof));

   * pBof = pArea->fBof;
   return SUCCESS;
}

/*
 * Determine logical end of file.
 */
static ERRCODE hb_waEof( AREAP pArea, BOOL * pEof )
{
   HB_TRACE(HB_TR_DEBUG, ("hb_waEof(%p, %p)", pArea, pEof));

   * pEof = pArea->fEof;
   return SUCCESS;
}

/*
 * Determine outcome of the last search operation.
 */
static ERRCODE hb_waFound( AREAP pArea, BOOL * pFound )
{
   HB_TRACE(HB_TR_DEBUG, ("hb_waFound(%p, %p)", pArea, pFound));

   * pFound = pArea->fFound;
   return SUCCESS;
}

/*
 * Reposition cursor relative to current position.
 */
static ERRCODE hb_waSkip( AREAP pArea, LONG lToSkip )
{
   LONG lSkip;

   HB_TRACE(HB_TR_DEBUG, ("hb_waSkip(%p, %ld)", pArea, lToSkip));

   /* Flush record and exit */
   if( lToSkip == 0 )
      return SELF_SKIPRAW( pArea, 0 );

   pArea->fTop = pArea->fBottom = FALSE;

   if( lToSkip > 0 )
      lSkip = 1;
   else
   {
      lSkip = -1;
      lToSkip *= -1;
   }
   while( --lToSkip >= 0 )
   {
      if( SELF_SKIPRAW( pArea, lSkip ) != SUCCESS )
         return FAILURE;
      if( SELF_SKIPFILTER( pArea, lSkip ) != SUCCESS )
         return FAILURE;
      if( pArea->fBof || pArea->fEof )
         break;
   }

   /* Update Bof and Eof flags */
   if( lSkip < 0 )
      pArea->fEof = FALSE;
   else /* ( lSkip > 0 ) */
      pArea->fBof = FALSE;

   return SUCCESS;
}

/*
 * Reposition cursor respecting any filter setting.
 */
static ERRCODE hb_waSkipFilter( AREAP pArea, LONG lUpDown )
{
   BOOL fBottom, fDeleted;
   ERRCODE uiError;

   HB_TRACE(HB_TR_DEBUG, ("hb_waSkipFilter(%p, %ld)", pArea, lUpDown));

   if( !hb_set.HB_SET_DELETED && pArea->dbfi.itmCobExpr == NULL )
      return SUCCESS;

   /* Since lToSkip is passed to SkipRaw, it should never request more than
      a single skip.
      The implied purpose of hb_waSkipFilter is to get off of a "bad" record
      after a skip was performed, NOT to skip lToSkip filtered records.
   */
   lUpDown = ( lUpDown < 0  ? -1 : 1 );

   /* remember if we are here after SLEF_GOTOP() */
   fBottom = pArea->fBottom;

   while( !pArea->fBof && !pArea->fEof )
   {
      /* SET DELETED */
      if( hb_set.HB_SET_DELETED )
      {
         if( SELF_DELETED( pArea, &fDeleted ) != SUCCESS )
            return FAILURE;
         if( fDeleted )
         {
            if( SELF_SKIPRAW( pArea, lUpDown ) != SUCCESS )
               return FAILURE;
            continue;
         }
      }

      /* SET FILTER TO */
      if( pArea->dbfi.itmCobExpr )
      {
         if( SELF_EVALBLOCK( pArea, pArea->dbfi.itmCobExpr ) != SUCCESS )
            return FAILURE;

         if( HB_IS_LOGICAL( pArea->valResult ) &&
             !hb_itemGetL( pArea->valResult ) )
         {
            if( SELF_SKIPRAW( pArea, lUpDown ) != SUCCESS )
               return FAILURE;
            continue;
         }
      }

      break;
   }

   /*
    * The only one situation when we should repos is backward skipping
    * if we are at BOTTOM position (it's SKIPFILTER called from GOBOTTOM)
    * then GOEOF() if not then GOTOP()
    */
   if( pArea->fBof && lUpDown < 0 )
   {
      if( fBottom )
      {
         /* GOTO EOF (phantom) record -
            this is the only one place where GOTO is used by xHarbour
            directly and RDD which does not operate on numbers should
            serve this method only as SELF_GOEOF() synonym. If it's a
            problem then we can remove this if and always use SELF_GOTOP()
            but it also means second table scan if all records filtered
            are out of filter so I do not want to do that. I will prefer
            explicit add SELF_GOEOF() method
          */
         uiError = SELF_GOTO( pArea, 0 );
      }
      else
      {
         uiError = SELF_GOTOP( pArea );
         pArea->fBof = TRUE;
      }
   }
   else
   {
      uiError = SUCCESS;
   }

   return uiError;
}

/*
 * Add a field to the WorkArea.
 */
static ERRCODE hb_waAddField( AREAP pArea, LPDBFIELDINFO pFieldInfo )
{
   LPFIELD pField;
   char szFieldName[ HB_SYMBOL_NAME_LEN + 1 ], *szPtr;

   HB_TRACE(HB_TR_DEBUG, ("hb_waAddField(%p, %p)", pArea, pFieldInfo));

   /* Validate the name of field */
   szPtr = ( char * ) pFieldInfo->atomName;
   while( HB_ISSPACE( *szPtr ) )
   {
      ++szPtr;
   }
   hb_strncpyUpperTrim( szFieldName, szPtr, sizeof( szFieldName ) - 1 );
   if( strlen( szFieldName ) == 0 )
      return FAILURE;

   pField = pArea->lpFields + pArea->uiFieldCount;
   if( pArea->uiFieldCount > 0 )
      ( ( LPFIELD ) ( pField - 1 ) )->lpfNext = pField;
   pField->sym = ( void * ) hb_dynsymGetCase( szFieldName );
   pField->uiType = pFieldInfo->uiType;
   pField->uiTypeExtended = pFieldInfo->uiTypeExtended;
   pField->uiLen = pFieldInfo->uiLen;
   pField->uiDec = pFieldInfo->uiDec;
   pField->uiFlags = pFieldInfo->uiFlags;
   pField->uiArea = pArea->uiArea;
   pArea->uiFieldCount ++;
   return SUCCESS;
}

/*
 * Add all fields defined in an array to the WorkArea.
 */
static ERRCODE hb_waCreateFields( AREAP pArea, PHB_ITEM pStruct )
{
   USHORT uiItems, uiCount, uiLen, uiDec;
   ERRCODE errCode = SUCCESS;
   DBFIELDINFO pFieldInfo;
   PHB_ITEM pFieldDesc;
   int iData;

   HB_TRACE(HB_TR_DEBUG, ("hb_waCreateFields(%p, %p)", pArea, pStruct));

   uiItems = ( USHORT ) hb_arrayLen( pStruct );
   if( SELF_SETFIELDEXTENT( pArea, uiItems ) != SUCCESS )
      return FAILURE;

   for( uiCount = 0; uiCount < uiItems; uiCount++ )
   {
      pFieldInfo.uiTypeExtended = 0;
      pFieldDesc = hb_arrayGetItemPtr( pStruct, uiCount + 1 );
      pFieldInfo.atomName = ( BYTE * ) hb_arrayGetCPtr( pFieldDesc, DBS_NAME );
      iData = hb_arrayGetNI( pFieldDesc, DBS_LEN );
      if( iData < 0 )
         iData = 0;
      uiLen = pFieldInfo.uiLen = ( USHORT ) iData;
      iData = hb_arrayGetNI( pFieldDesc, DBS_DEC );
      if( iData < 0 )
         iData = 0;
      uiDec = ( USHORT ) iData;
      pFieldInfo.uiDec = 0;
#ifdef DBS_FLAG
      pFieldInfo.uiFlags = hb_arrayGetNI( pFieldDesc, DBS_FLAG );
#else
      pFieldInfo.uiFlags = 0;
#endif
      iData = toupper( hb_arrayGetCPtr( pFieldDesc, DBS_TYPE )[ 0 ] );
      switch( iData )
      {
         case 'C':
            pFieldInfo.uiType = HB_FT_STRING;
            pFieldInfo.uiLen = uiLen;
/* Too many people reported the behavior with code below as a
   Clipper compatibility bug so I commented this code. Druzus.
#ifdef HB_C52_STRICT
            pFieldInfo.uiLen = uiLen;
#else
            pFieldInfo.uiLen = uiLen + uiDec * 256;
#endif
*/
            break;

         case 'L':
            pFieldInfo.uiType = HB_FT_LOGICAL;
            pFieldInfo.uiLen = 1;
            break;

         case 'D':
            pFieldInfo.uiType = HB_FT_DATE;
            pFieldInfo.uiLen = ( uiLen == 3 || uiLen == 4 ) ? uiLen : 8;
            break;

         case 'I':
            pFieldInfo.uiType = HB_FT_INTEGER;
            pFieldInfo.uiLen = ( ( uiLen > 0 && uiLen <= 4 ) || uiLen == 8 ) ? uiLen : 4;
            pFieldInfo.uiDec = uiDec;
            break;

         case 'Y':
            pFieldInfo.uiType = HB_FT_CURRENCY;
            pFieldInfo.uiLen = 8;
            pFieldInfo.uiDec = 4;
            break;

         case 'Z':
            pFieldInfo.uiType = HB_FT_CURDOUBLE;
            pFieldInfo.uiLen = 8;
            pFieldInfo.uiDec = uiDec;
            break;

         case '2':
         case '4':
            pFieldInfo.uiType = HB_FT_INTEGER;
            pFieldInfo.uiLen = iData - '0';
            break;

         case 'B':
         case '8':
            pFieldInfo.uiType = HB_FT_DOUBLE;
            pFieldInfo.uiLen = 8;
            pFieldInfo.uiDec = uiDec;
            break;

         case 'N':
            pFieldInfo.uiType = HB_FT_LONG;
            pFieldInfo.uiDec = uiDec;
            /* DBASE documentation defines maximum numeric field size as 20
             * but Clipper alows to create longer fileds so I remove this
             * limit, Druzus
             */
            /*
            if( uiLen > 20 )
            */
            if( uiLen > 255 )
               errCode = FAILURE;
            break;

         case 'F':
            pFieldInfo.uiType = HB_FT_FLOAT;
            pFieldInfo.uiDec = uiDec;
            /* see note above */
            if( uiLen > 255 )
               errCode = FAILURE;
            break;

         case 'T':
            if( uiLen == 8 )
            {
               pFieldInfo.uiType = HB_FT_DAYTIME;
               pFieldInfo.uiLen = 8;
            }
            else
            {
               pFieldInfo.uiType = HB_FT_TIME;
               pFieldInfo.uiLen = 4;
            }
            break;

         case '@':
            pFieldInfo.uiType = HB_FT_DAYTIME;
            pFieldInfo.uiLen = 8;
            break;

         case '=':
            pFieldInfo.uiType = HB_FT_MODTIME;
            pFieldInfo.uiLen = 8;
            break;

         case '^':
            pFieldInfo.uiType = HB_FT_ROWVER;
            pFieldInfo.uiLen = 8;
            break;

         case '+':
            pFieldInfo.uiType = HB_FT_AUTOINC;
            pFieldInfo.uiLen = 4;
            break;

         case 'Q':
            pFieldInfo.uiType = HB_FT_VARLENGTH;
            pFieldInfo.uiLen = uiLen > 255 ? 255 : uiLen;
            break;

         case 'M':
            pFieldInfo.uiType = HB_FT_MEMO;
            pFieldInfo.uiLen = ( uiLen == 4 ) ? 4 : 10;
            break;

         case 'V':
            pFieldInfo.uiType = HB_FT_ANY;
            pFieldInfo.uiLen = ( uiLen < 3 || uiLen == 5 ) ? 6 : uiLen;
            break;

         case 'P':
            pFieldInfo.uiType = HB_FT_IMAGE;
            pFieldInfo.uiLen = ( uiLen == 4 ) ? 4 : 10;
            break;

         case 'W':
            pFieldInfo.uiType = HB_FT_BLOB;
            pFieldInfo.uiLen = ( uiLen == 4 ) ? 4 : 10;
            break;

         case 'G':
            pFieldInfo.uiType = HB_FT_OLE;
            pFieldInfo.uiLen = ( uiLen == 4 ) ? 4 : 10;
            break;

         default:
            errCode = FAILURE;
            break;
      }

      if( errCode != SUCCESS )
      {
         hb_errRT_DBCMD( EG_ARG, EDBCMD_DBCMDBADPARAMETER, NULL, HB_ERR_FUNCNAME );
         return errCode;
      }
      /* Add field */
      else if( SELF_ADDFIELD( pArea, &pFieldInfo ) != SUCCESS )
         return FAILURE;
   }
   return SUCCESS;
}

/*
 * Determine the number of fields in the WorkArea.
 */
static ERRCODE hb_waFieldCount( AREAP pArea, USHORT * uiFields )
{
   HB_TRACE(HB_TR_DEBUG, ("hb_waFieldCount(%p, %p)", pArea, uiFields));

   * uiFields = pArea->uiFieldCount;
   return SUCCESS;
}

/*
 * Retrieve information about a field.
 */
static ERRCODE hb_waFieldInfo( AREAP pArea, USHORT uiIndex, USHORT uiType, PHB_ITEM pItem )
{
   LPFIELD pField;

   HB_TRACE(HB_TR_DEBUG, ("hb_waFieldInfo(%p, %hu, %hu, %p)", pArea, uiIndex, uiType, pItem));

   if( uiIndex > pArea->uiFieldCount )
      return FAILURE;

   pField = pArea->lpFields + uiIndex - 1;
   switch( uiType )
   {
      case DBS_NAME:
         hb_itemPutC( pItem, hb_dynsymName( ( PHB_DYNS ) pField->sym ) );
         break;

      case DBS_TYPE:
         switch( pField->uiType )
         {
            case HB_FT_STRING:
               hb_itemPutC( pItem, "C" );
               break;

            case HB_FT_LOGICAL:
               hb_itemPutC( pItem, "L" );
               break;

            case HB_FT_DATE:
               hb_itemPutC( pItem, "D" );
               break;

            case HB_FT_LONG:
               hb_itemPutC( pItem, "N" );
               break;

            case HB_FT_INTEGER:
               hb_itemPutC( pItem, "I" );
               break;

            case HB_FT_DOUBLE:
               hb_itemPutC( pItem, "B" );
               break;

            case HB_FT_FLOAT:
               hb_itemPutC( pItem, "F" );
               break;

            case HB_FT_TIME:
               hb_itemPutC( pItem, "T" );
               break;

            case HB_FT_DAYTIME:
               hb_itemPutC( pItem, "@" );
               break;

            case HB_FT_MODTIME:
               hb_itemPutC( pItem, "=" );
               break;

            case HB_FT_ROWVER:
               hb_itemPutC( pItem, "^" );
               break;

            case HB_FT_AUTOINC:
               hb_itemPutC( pItem, "+" );
               break;

            case HB_FT_CURRENCY:
               hb_itemPutC( pItem, "Y" );
               break;

            case HB_FT_CURDOUBLE:
               hb_itemPutC( pItem, "Z" );
               break;

            case HB_FT_VARLENGTH:
               hb_itemPutC( pItem, "Q" );
               break;

            case HB_FT_MEMO:
               hb_itemPutC( pItem, "M" );
               break;

            case HB_FT_ANY:
               hb_itemPutC( pItem, "V" );
               break;

            case HB_FT_IMAGE:
               hb_itemPutC( pItem, "P" );
               break;

            case HB_FT_BLOB:
               hb_itemPutC( pItem, "W" );
               break;

            case HB_FT_OLE:
               hb_itemPutC( pItem, "G" );
               break;

            default:
               hb_itemPutC( pItem, "U" );
               break;
         }
         break;

      case DBS_LEN:
         hb_itemPutNL( pItem, pField->uiLen );
         break;

      case DBS_DEC:
         hb_itemPutNL( pItem, pField->uiDec );
         break;

#ifdef DBS_FLAG
      case DBS_FLAG:
         hb_itemPutNL( pItem, pField->uiFlags );
         break;
#endif

      default:
         return FAILURE;

   }
   return SUCCESS;
}

/*
 * Determine the name associated with a field number.
 */
static ERRCODE hb_waFieldName( AREAP pArea, USHORT uiIndex, void * szName )
{
   LPFIELD pField;

   HB_TRACE(HB_TR_DEBUG, ("hb_waFieldName(%p, %hu, %p)", pArea, uiIndex, szName));

   if( uiIndex > pArea->uiFieldExtent )
      return FAILURE;

   pField = pArea->lpFields + uiIndex - 1;
   hb_strncpy( ( char * ) szName, hb_dynsymName( ( PHB_DYNS ) pField->sym ),
               pArea->uiMaxFieldNameLength );
   return SUCCESS;
}

/*
 * Establish the extent of the array of fields for a WorkArea.
 */
static ERRCODE hb_waSetFieldExtent( AREAP pArea, USHORT uiFieldExtent )
{
   HB_TRACE(HB_TR_DEBUG, ("hb_waSetFieldExtent(%p, %hu)", pArea, uiFieldExtent));

   pArea->uiFieldExtent = uiFieldExtent;

   /* Alloc field array */
   if( uiFieldExtent )
   {
      pArea->lpFields = ( LPFIELD ) hb_xgrab( uiFieldExtent * sizeof( FIELD ) );
      memset( pArea->lpFields, 0, uiFieldExtent * sizeof( FIELD ) );
   }

   return SUCCESS;
}

/*
 * Obtain the alias of the WorkArea.
 */
static ERRCODE hb_waAlias( AREAP pArea, BYTE * szAlias )
{
   HB_TRACE(HB_TR_DEBUG, ("hb_waAlias(%p, %p)", pArea, szAlias));

   hb_strncpy( ( char * ) szAlias,
      pArea->atomAlias && hb_dynsymAreaHandle( ( PHB_DYNS ) pArea->atomAlias )
      ? hb_dynsymName( ( PHB_DYNS ) pArea->atomAlias ) : "",
      HB_RDD_MAX_ALIAS_LEN );

   return SUCCESS;
}

/*
 * Close the table in the WorkArea - helper function
 */
static ERRCODE hb_waCloseAux( AREAP pArea, void * pChildArea )
{
   USHORT uiPrevArea, uiArea;
   LPDBRELINFO lpdbRelation, lpdbRelPrev, lpdbRelTmp;

   uiArea = ( ( AREAP ) pChildArea )->uiArea;
   if( pArea->lpdbRelations )
   {
      uiPrevArea = hb_rddGetCurrentWorkAreaNumber();
      lpdbRelation = pArea->lpdbRelations;
      lpdbRelPrev = NULL;
      while( lpdbRelation )
      {
         if( lpdbRelation->lpaChild->uiArea == uiArea )
         {
            /* Clear this relation */
            hb_rddSelectWorkAreaNumber( lpdbRelation->lpaChild->uiArea );
            SELF_CHILDEND( lpdbRelation->lpaChild, lpdbRelation );
            hb_rddSelectWorkAreaNumber( uiPrevArea );
            if( lpdbRelation->itmCobExpr )
            {
               hb_itemRelease( lpdbRelation->itmCobExpr );
            }
            if( lpdbRelation->abKey )
               hb_itemRelease( lpdbRelation->abKey );
            lpdbRelTmp = lpdbRelation;
            if( lpdbRelPrev )
               lpdbRelPrev->lpdbriNext = lpdbRelation->lpdbriNext;
            else
               pArea->lpdbRelations = lpdbRelation->lpdbriNext;
            lpdbRelation = lpdbRelation->lpdbriNext;
            hb_xfree( lpdbRelTmp );
         }
         else
         {
            lpdbRelPrev  = lpdbRelation;
            lpdbRelation = lpdbRelation->lpdbriNext;
         }
      }
   }
   return SUCCESS;
}

/*
 * Close the table in the WorkArea.
 */
static ERRCODE hb_waClose( AREAP pArea )
{
   HB_TRACE(HB_TR_DEBUG, ("hb_waClose(%p)", pArea));

   /* Clear items */
   SELF_CLEARFILTER( pArea );
   SELF_CLEARREL( pArea );
   SELF_CLEARLOCATE( pArea );

   if( pArea->uiParents > 0 )
   {
      /* Clear relations that has this area as a child */
      hb_rddIterateWorkAreas( hb_waCloseAux, pArea );
   }

   if( pArea->atomAlias )
      hb_dynsymSetAreaHandle( ( PHB_DYNS ) pArea->atomAlias, 0 );

   return SUCCESS;
}

/*
 * Retrieve information about the current driver.
 */
static ERRCODE hb_waInfo( AREAP pArea, USHORT uiIndex, PHB_ITEM pItem )
{
   HB_TRACE(HB_TR_DEBUG, ("hb_waInfo(%p, %hu, %p)", pArea, uiIndex, pItem));

   switch( uiIndex )
   {
      case DBI_ISDBF:
      case DBI_CANPUTREC:
      case DBI_ISFLOCK:
      case DBI_SHARED:
         hb_itemPutL( pItem, FALSE );
         break;

      /*
       * IMHO better to return FAILURE to notice that it's not supported
       */
      case DBI_GETDELIMITER:
      case DBI_SETDELIMITER:
      case DBI_SEPARATOR:
         hb_itemPutC( pItem, NULL );
         return FAILURE;

      case DBI_GETHEADERSIZE:
      case DBI_GETRECSIZE:
      case DBI_LOCKCOUNT:
         hb_itemPutNI( pItem, 0 );
         break;

      case DBI_LASTUPDATE:
         hb_itemPutDL( pItem, 0 );
         break;

      case DBI_GETLOCKARRAY:
         hb_arrayNew( pItem, 0 );
         break;

      case DBI_CHILDCOUNT:
      {
         LPDBRELINFO lpdbRelations = pArea->lpdbRelations;
         USHORT uiCount = 0;
         while( lpdbRelations )
         {
            uiCount++;
            lpdbRelations = lpdbRelations->lpdbriNext;
         }
         hb_itemPutNI( pItem, uiCount );
         break;
      }

      case DBI_BOF:
         hb_itemPutL( pItem, pArea->fBof );
         break;

      case DBI_EOF:
         hb_itemPutL( pItem, pArea->fEof );
         break;

      case DBI_DBFILTER:
         if( pArea->dbfi.abFilterText )
            hb_itemCopy( pItem, pArea->dbfi.abFilterText );
         else
            hb_itemPutC( pItem, NULL );
         break;

      case DBI_FOUND:
         hb_itemPutL( pItem, pArea->fFound );
         break;

      case DBI_FCOUNT:
         hb_itemPutNI( pItem, pArea->uiFieldCount );
         break;

      case DBI_ALIAS:
      {
         char szAlias[ HB_RDD_MAX_ALIAS_LEN + 1 ];
         if( SELF_ALIAS( pArea, ( BYTE * ) szAlias ) != SUCCESS )
         {
            return FAILURE;
         }
         hb_itemPutC( pItem, szAlias );
         break;
      }

      case DBI_TABLEEXT:
         hb_itemClear( pItem );
         return SELF_RDDINFO( SELF_RDDNODE( pArea ), RDDI_TABLEEXT, 0, pItem );

      case DBI_SCOPEDRELATION:
      {
         int iRelNo = hb_itemGetNI( pItem );
         BOOL fScoped = FALSE;

         if( iRelNo > 0 )
         {
            LPDBRELINFO lpdbRelations = pArea->lpdbRelations;
            while( lpdbRelations )
            {
               if( --iRelNo == 0 )
               {
                  fScoped = lpdbRelations->isScoped;
                  break;
               }
               lpdbRelations = lpdbRelations->lpdbriNext;
            }
         }
         hb_itemPutL( pItem, fScoped );
         break;
      }
      case DBI_POSITIONED:
      {
         ULONG ulRecCount, ulRecNo;
         if( SELF_RECNO( pArea, &ulRecNo ) != SUCCESS )
            return FAILURE;
         if( ulRecNo == 0 )
            hb_itemPutL( pItem, TRUE );
         else if( SELF_RECCOUNT( pArea, &ulRecCount ) != SUCCESS )
            return FAILURE;
         else
            hb_itemPutL( pItem, ulRecNo != ulRecCount + 1 );
         break;
      }
      case DBI_RM_SUPPORTED:
         hb_itemPutL( pItem, FALSE );
         break;

      case DBI_DB_VERSION:
         hb_itemPutC( pItem, NULL );
         break;

      case DBI_RDD_VERSION:
         hb_itemPutC( pItem, NULL );
         break;

      default:
         return FAILURE;
   }
   return SUCCESS;
}

/*
 * Retrieve information about the current order that SELF could not.
 * Called by SELF_ORDINFO if uiIndex is not supported.
 */
static ERRCODE hb_waOrderInfo( AREAP pArea, USHORT index, LPDBORDERINFO pInfo )
{
   HB_TRACE(HB_TR_DEBUG, ("hb_waOrderInfo(%p, %hu, %p)", pArea, index, pInfo));

   HB_SYMBOL_UNUSED( pArea );
   HB_SYMBOL_UNUSED( index );

   if( pInfo->itmResult )
      hb_itemClear( pInfo->itmResult );

   /* CA-Cl*pper does not generate RT error when default ORDERINFO() method
    * is called
    */
   /* hb_errRT_DBCMD( EG_ARG, EDBCMD_BADPARAMETER, NULL, HB_ERR_FUNCNAME ); */

   return FAILURE;
}

/*
 * Clear the WorkArea for use.
 */
static ERRCODE hb_waNewArea( AREAP pArea )
{
   HB_TRACE(HB_TR_DEBUG, ("hb_waNewArea(%p)", pArea));

   pArea->valResult = hb_itemNew( NULL );
   pArea->lpdbRelations = NULL;
   pArea->uiParents = 0;
   pArea->uiMaxFieldNameLength = 10;

   return SUCCESS;
}

/*
 * Open a data store in the WorkArea.
 * Like in Clipper it's also mapped as Create() method at WA level
 */
static ERRCODE hb_waOpen( AREAP pArea, LPDBOPENINFO pInfo )
{
   if( !pArea->atomAlias && pInfo->atomAlias && pInfo->atomAlias[ 0 ] )
   {
      pArea->atomAlias = hb_rddAllocWorkAreaAlias( ( char * ) pInfo->atomAlias,
                                                   ( int ) pInfo->uiArea );
      if( ! pArea->atomAlias )
      {
         SELF_CLOSE( ( AREAP ) pArea );
         return FAILURE;
      }
   }
   return SUCCESS;
}

static ERRCODE hb_waOrderCondition( AREAP pArea, LPDBORDERCONDINFO param )
{
   if( pArea->lpdbOrdCondInfo )
   {
      if( pArea->lpdbOrdCondInfo->abFor )
         hb_xfree( pArea->lpdbOrdCondInfo->abFor );
      if( pArea->lpdbOrdCondInfo->abWhile )
         hb_xfree( pArea->lpdbOrdCondInfo->abWhile );
      if( pArea->lpdbOrdCondInfo->itmCobFor )
      {
         hb_itemRelease( pArea->lpdbOrdCondInfo->itmCobFor );
      }
      if( pArea->lpdbOrdCondInfo->itmCobWhile )
      {
         hb_itemRelease( pArea->lpdbOrdCondInfo->itmCobWhile );
      }
      if( pArea->lpdbOrdCondInfo->itmCobEval )
      {
         hb_itemRelease( pArea->lpdbOrdCondInfo->itmCobEval );
      }
      if( pArea->lpdbOrdCondInfo->itmStartRecID )
      {
         hb_itemRelease( pArea->lpdbOrdCondInfo->itmStartRecID );
      }
      if( pArea->lpdbOrdCondInfo->itmRecID )
      {
         hb_itemRelease( pArea->lpdbOrdCondInfo->itmRecID );
      }
      hb_xfree( pArea->lpdbOrdCondInfo );
   }
   pArea->lpdbOrdCondInfo = param;

   return SUCCESS;
}

/*
 * Release all references to a WorkArea.
 */
static ERRCODE hb_waRelease( AREAP pArea )
{
   HB_TRACE(HB_TR_DEBUG, ("hb_waRelease(%p)", pArea));

   /* Free all allocated pointers */
   if( pArea->lpFields )
      hb_xfree( pArea->lpFields );
   if( pArea->valResult )
      hb_itemRelease( pArea->valResult );
   if( pArea->lpdbOrdCondInfo )
      /* intentionally direct call not a method */
      hb_waOrderCondition( pArea,NULL );
   hb_xfree( pArea );
   return SUCCESS;
}

/*
 * Retrieve the size of the WorkArea structure.
 */
static ERRCODE hb_waStructSize( AREAP pArea, USHORT * uiSize )
{
   HB_TRACE(HB_TR_DEBUG, ("hb_waStrucSize(%p, %p)", pArea, uiSize));
   HB_SYMBOL_UNUSED( pArea );

   * uiSize = sizeof( AREA );
   return SUCCESS;
}

/*
 * Obtain the name of replaceable database driver (RDD) subsystem.
 */
static ERRCODE hb_waSysName( AREAP pArea, BYTE * pBuffer )
{
   HB_TRACE(HB_TR_DEBUG, ("hb_waSysName(%p, %p)", pArea, pBuffer));

   hb_strncpy( ( char * ) pBuffer, SELF_RDDNODE( pArea )->szName,
               HB_RDD_MAX_DRIVERNAME_LEN );

   return SUCCESS;
}

/*
 * Evaluate code block for each record in WorkArea.
 */
static ERRCODE hb_waEval( AREAP pArea, LPDBEVALINFO pEvalInfo )
{
   LONG lNext = 1;
   BOOL fEof, fFor;

   HB_TRACE(HB_TR_DEBUG, ("hb_waEval(%p, %p)", pArea, pEvalInfo));

   if( pEvalInfo->dbsci.itmRecID )
   {
      if( SELF_GOTOID( pArea, pEvalInfo->dbsci.itmRecID ) != SUCCESS )
         return FAILURE;
   }
   else if( pEvalInfo->dbsci.lNext )
   {
      lNext = hb_itemGetNL( pEvalInfo->dbsci.lNext );
   }
   else if( !pEvalInfo->dbsci.itmCobWhile &&
            !hb_itemGetL( pEvalInfo->dbsci.fRest ) )
   {
      if( SELF_GOTOP( pArea ) != SUCCESS )
         return FAILURE;
   }

   /* TODO: use SKIPSCOPE() method and fRest parameter */

   if( !pEvalInfo->dbsci.lNext || lNext > 0 )
   {
      while( TRUE )
      {
         if( SELF_EOF( pArea, &fEof ) != SUCCESS )
            return FAILURE;

         if( fEof )
            break;

         if( pEvalInfo->dbsci.itmCobWhile )
         {
            if( SELF_EVALBLOCK( pArea, pEvalInfo->dbsci.itmCobWhile ) != SUCCESS )
               return FAILURE;
            if( ! hb_itemGetL( pArea->valResult ) )
               break;
         }

         if( pEvalInfo->dbsci.itmCobFor )
         {
            if( SELF_EVALBLOCK( pArea, pEvalInfo->dbsci.itmCobFor ) != SUCCESS )
               return FAILURE;
            fFor = hb_itemGetL( pArea->valResult );
         }
         else
            fFor = TRUE;

         if( fFor )
         {
            if( SELF_EVALBLOCK( pArea, pEvalInfo->itmBlock ) != SUCCESS )
               return FAILURE;
         }

         if( pEvalInfo->dbsci.itmRecID || ( pEvalInfo->dbsci.lNext && --lNext < 1 ) )
            break;

         if( SELF_SKIP( pArea, 1 ) != SUCCESS )
            return FAILURE;
      }
   }

   return SUCCESS;
}

/*
 * Locate a record which pass given condition
 */
static ERRCODE hb_waLocate( AREAP pArea, BOOL fContinue )
{
   LONG lNext = 1;
   BOOL fEof;

   HB_TRACE(HB_TR_DEBUG, ("hb_waLocate(%p, %d)", pArea, fContinue));

   if( fContinue )
   {
      if( ! pArea->dbsi.itmCobFor )
         return SUCCESS;

      if( SELF_SKIP( pArea, 1 ) != SUCCESS )
         return FAILURE;
   }
   else if( pArea->dbsi.itmRecID )
   {
      if( SELF_GOTOID( pArea, pArea->dbsi.itmRecID ) != SUCCESS )
         return FAILURE;
   }
   else if( pArea->dbsi.lNext )
   {
      lNext = hb_itemGetNL( pArea->dbsi.lNext );
   }
   else if( !pArea->dbsi.itmCobWhile &&
            !hb_itemGetL( pArea->dbsi.fRest ) )
   {
      if( SELF_GOTOP( pArea ) != SUCCESS )
         return FAILURE;
   }

   pArea->fFound = FALSE;

   /* TODO: use SKIPSCOPE() method and fRest parameter */

   if( !pArea->dbsi.lNext || lNext > 0 )
   {
      while( TRUE )
      {
         if( SELF_EOF( pArea, &fEof ) != SUCCESS )
            return FAILURE;

         if( fEof )
            break;

         if( !fContinue && pArea->dbsi.itmCobWhile )
         {
            if( SELF_EVALBLOCK( pArea, pArea->dbsi.itmCobWhile ) != SUCCESS )
               return FAILURE;
            if( ! hb_itemGetL( pArea->valResult ) )
               break;
         }

         if( ! pArea->dbsi.itmCobFor )
         {
            pArea->fFound = TRUE;
            break;
         }
         else
         {
            if( SELF_EVALBLOCK( pArea, pArea->dbsi.itmCobFor ) != SUCCESS )
               return FAILURE;

            if( hb_itemGetL( pArea->valResult ) )
            {
               pArea->fFound = TRUE;
               break;
            }
         }

         if( !fContinue &&
             ( pArea->dbsi.itmRecID || ( pArea->dbsi.lNext && --lNext < 1 ) ) )
            break;

         if( SELF_SKIP( pArea, 1 ) != SUCCESS )
            return FAILURE;
      }
   }

   return SUCCESS;
}

/*
 * Copy one or more records from one WorkArea to another.
 */
static ERRCODE hb_waTrans( AREAP pArea, LPDBTRANSINFO pTransInfo )
{
   LONG lNext = 1;
   BOOL fEof, fFor;

   HB_TRACE(HB_TR_DEBUG, ("hb_waTrans(%p, %p)", pArea, pTransInfo));

   if( pTransInfo->dbsci.itmRecID )
   {
      if( SELF_GOTOID( pArea, pTransInfo->dbsci.itmRecID ) != SUCCESS )
         return FAILURE;
   }
   else if( pTransInfo->dbsci.lNext )
   {
      lNext = hb_itemGetNL( pTransInfo->dbsci.lNext );
   }
   else if( !pTransInfo->dbsci.itmCobWhile &&
            !hb_itemGetL( pTransInfo->dbsci.fRest ) )
   {
      if( SELF_GOTOP( pArea ) != SUCCESS )
         return FAILURE;
   }

   /* TODO: use SKIPSCOPE() method and fRest parameter */

   if( !pTransInfo->dbsci.lNext || lNext > 0 )
   {
      while( TRUE )
      {
         if( SELF_EOF( pArea, &fEof ) != SUCCESS )
            return FAILURE;

         if( fEof )
            break;

         if( pTransInfo->dbsci.itmCobWhile )
         {
            if( SELF_EVALBLOCK( pArea, pTransInfo->dbsci.itmCobWhile ) != SUCCESS )
               return FAILURE;
            if( ! hb_itemGetL( pArea->valResult ) )
               break;
         }

         if( pTransInfo->dbsci.itmCobFor )
         {
            if( SELF_EVALBLOCK( pArea, pTransInfo->dbsci.itmCobFor ) != SUCCESS )
               return FAILURE;
            fFor = hb_itemGetL( pArea->valResult );
         }
         else
            fFor = TRUE;

         if( fFor )
         {
            if( SELF_TRANSREC( pArea, pTransInfo ) != SUCCESS )
               return FAILURE;
         }

         if( pTransInfo->dbsci.itmRecID || ( pTransInfo->dbsci.lNext && --lNext < 1 ) )
            break;

         if( SELF_SKIP( pArea, 1 ) != SUCCESS )
            return FAILURE;
      }
   }

   return SUCCESS;
}

/*
 * Copy a record to another WorkArea.
 */
static ERRCODE hb_waTransRec( AREAP pArea, LPDBTRANSINFO pTransInfo )
{
   BOOL bDeleted;
   BYTE *pRecord;
   ERRCODE errCode;

   HB_TRACE(HB_TR_DEBUG, ("hb_waTransRec(%p, %p)", pArea, pTransInfo));

   /* Record deleted? */
   errCode = SELF_DELETED( ( AREAP ) pArea, &bDeleted );
   if( errCode != SUCCESS )
      return errCode;

   if( pTransInfo->uiFlags & DBTF_MATCH && pTransInfo->uiFlags & DBTF_PUTREC )
   {
      errCode = SELF_GETREC( ( AREAP ) pArea, &pRecord );
      if( errCode != SUCCESS )
         return errCode;

      /* Append a new record */
      errCode = SELF_APPEND( ( AREAP ) pTransInfo->lpaDest, TRUE );
      if( errCode != SUCCESS )
         return errCode;

      /* Copy record */
      errCode = SELF_PUTREC( ( AREAP ) pTransInfo->lpaDest, pRecord );
   }
   else
   {
      LPDBTRANSITEM pTransItem;
      PHB_ITEM pItem;
      USHORT uiCount;

      /* Append a new record */
      errCode = SELF_APPEND( ( AREAP ) pTransInfo->lpaDest, TRUE );
      if( errCode != SUCCESS )
         return errCode;

      pItem = hb_itemNew( NULL );
      pTransItem = pTransInfo->lpTransItems;
      for( uiCount = pTransInfo->uiItemCount; uiCount; --uiCount )
      {
         errCode = SELF_GETVALUE( ( AREAP ) pArea,
                                  pTransItem->uiSource, pItem );
         if( errCode != SUCCESS )
            break;
         errCode = SELF_PUTVALUE( ( AREAP ) pTransInfo->lpaDest,
                                  pTransItem->uiDest, pItem );
         if( errCode != SUCCESS )
            break;
         ++pTransItem;
      }
      hb_itemRelease( pItem );
   }

   /* Delete the new record if copy fail */
   if( errCode != SUCCESS )
   {
      SELF_DELETE( ( AREAP ) pTransInfo->lpaDest );
      return errCode;
   }

   /* Delete the new record */
   if( bDeleted )
      return SELF_DELETE( ( AREAP ) pTransInfo->lpaDest );

   return SUCCESS;
}

/*
 * Report end of relation.
 */
static ERRCODE hb_waChildEnd( AREAP pArea, LPDBRELINFO pRelInfo )
{
   HB_TRACE(HB_TR_DEBUG, ("hb_waChildEnd(%p, %p)", pArea, pRelInfo));

   if( pRelInfo->isScoped )
   {
      DBORDERINFO pInfo;
      pInfo.itmOrder = NULL;
      pInfo.atomBagName = NULL;
      pInfo.itmResult = hb_itemNew( NULL );
      pInfo.itmNewVal = NULL;
      SELF_ORDINFO( pArea, DBOI_SCOPETOPCLEAR, &pInfo );
      SELF_ORDINFO( pArea, DBOI_SCOPEBOTTOMCLEAR, &pInfo );
      hb_itemRelease( pInfo.itmResult );
   }

   pArea->uiParents--;
   return SUCCESS;
}

/*
 * Report initialization of a relation.
 */
static ERRCODE hb_waChildStart( AREAP pArea, LPDBRELINFO pRelInfo )
{
   HB_TRACE(HB_TR_DEBUG, ("hb_waChildStart(%p, %p)", pArea, pRelInfo));
   HB_SYMBOL_UNUSED( pRelInfo );

   pArea->uiParents ++;
   return SUCCESS;
}

/*
 * Force relational movement in child WorkAreas.
 */
static ERRCODE hb_waSyncChildren( AREAP pArea )
{

   LPDBRELINFO lpdbRelation;
   HB_TRACE(HB_TR_DEBUG, ("hb_waSyncChildren(%p)", pArea));

   lpdbRelation = pArea->lpdbRelations;
   while( lpdbRelation )
   {
      if( SELF_CHILDSYNC( lpdbRelation->lpaChild, lpdbRelation ) != SUCCESS )
         return FAILURE;
      lpdbRelation = lpdbRelation->lpdbriNext;
   }

   return SUCCESS;
}

/*
 * Clear all relations in the specified WorkArea.
 */
static ERRCODE hb_waClearRel( AREAP pArea )
{
   HB_TRACE(HB_TR_DEBUG, ("hb_waClearRel(%p)", pArea ));

   /* Free all relations */
   if( pArea->lpdbRelations )
   {
      int iCurrArea = hb_rddGetCurrentWorkAreaNumber();

      do
      {
         LPDBRELINFO lpdbRelation = pArea->lpdbRelations;

         hb_rddSelectWorkAreaNumber( lpdbRelation->lpaChild->uiArea );
         SELF_CHILDEND( lpdbRelation->lpaChild, lpdbRelation );
         pArea->lpdbRelations = lpdbRelation->lpdbriNext;

         if( lpdbRelation->itmCobExpr )
         {
            hb_itemRelease( lpdbRelation->itmCobExpr );
         }
         if( lpdbRelation->abKey )
         {
            hb_itemRelease( lpdbRelation->abKey );
         }
         hb_xfree( lpdbRelation );
      }
      while( pArea->lpdbRelations );

      hb_rddSelectWorkAreaNumber( iCurrArea );
   }

   return SUCCESS;
}

/*
 * Obtain the workarea number of the specified relation.
 */
static ERRCODE hb_waRelArea( AREAP pArea, USHORT uiRelNo, void * pRelArea )
{
   LPDBRELINFO lpdbRelations;
   USHORT uiIndex = 1;
   USHORT * pWA = (USHORT *) pRelArea;
   /*TODO: Why pRelArea declared as void*? This creates casting hassles.*/

   HB_TRACE(HB_TR_DEBUG, ("hb_waRelArea(%p, %hu, %p)", pArea, uiRelNo, pRelArea));

   *pWA = 0;
   lpdbRelations = pArea->lpdbRelations;
   while( lpdbRelations )
   {
      if( uiIndex++ == uiRelNo )
      {
         *pWA = lpdbRelations->lpaChild->uiArea;
         break;
      }
      lpdbRelations = lpdbRelations->lpdbriNext;
   }
   return *pWA ? SUCCESS : FAILURE;
}

/*
 * Evaluate a block against the relation in specified WorkArea.
 */
static ERRCODE hb_waRelEval( AREAP pArea, LPDBRELINFO pRelInfo )
{
   PHB_ITEM pResult;
   DBORDERINFO pInfo;
   ERRCODE errCode;
   int iOrder;

   HB_TRACE(HB_TR_DEBUG, ("hb_waRelEval(%p, %p)", pArea, pRelInfo));

   errCode = SELF_EVALBLOCK( pRelInfo->lpaParent, pRelInfo->itmCobExpr );

   if( errCode == SUCCESS )
   {
      /*
       *  Check the current order
       */
      pResult = pRelInfo->lpaParent->valResult;
      pRelInfo->lpaParent->valResult = NULL;
      memset( &pInfo, 0, sizeof( DBORDERINFO ) );
      pInfo.itmResult = hb_itemPutNI( NULL, 0 );
      errCode = SELF_ORDINFO( pArea, DBOI_NUMBER, &pInfo );

      if( errCode == SUCCESS )
      {
         iOrder = hb_itemGetNI( pInfo.itmResult );
         if( iOrder != 0 )
         {
            if( pRelInfo->isScoped )
            {
               pInfo.itmNewVal = pResult;
               errCode = SELF_ORDINFO( pArea, DBOI_SCOPETOP, &pInfo );
               if( errCode == SUCCESS )
                  errCode = SELF_ORDINFO( pArea, DBOI_SCOPEBOTTOM, &pInfo );
            }
            if( errCode == SUCCESS )
               errCode = SELF_SEEK( pArea, FALSE, pResult, FALSE );
         }
         else
         {
            /*
             * If current order equals to zero, use GOTOID instead of SEEK
             * Unfortunately it interacts with buggy .prg code which returns
             * non numerical values from relation expression and RDD accepts
             * only numerical record ID. In such case SELF_GOTO() works like
             * SELF_GOEOF() but SELF_GOTOID() reports error. So for Clipper
             * compatibility SELF_GOTO() is used here but if RDD can use
             * non numerical record IDs then this method should be overloaded
             * to use SELF_GOTOID(), [druzus]
             */
            /* errCode = SELF_GOTOID( pArea, pResult ); */
            errCode = SELF_GOTO( pArea, hb_itemGetNL( pResult ) );
         }
      }
      hb_itemRelease( pInfo.itmResult );
      hb_itemRelease( pResult );
   }

   return errCode;
}

/*
 * Obtain the character expression of the specified relation.
 */
static ERRCODE hb_waRelText( AREAP pArea, USHORT uiRelNo, PHB_ITEM pExpr )
{
   LPDBRELINFO lpdbRelations;
   USHORT uiIndex = 1;

   HB_TRACE(HB_TR_DEBUG, ("hb_waRelText(%p, %hu, %p)", pArea, uiRelNo, pExpr));

   lpdbRelations = pArea->lpdbRelations;

   while( lpdbRelations )
   {
      if( uiIndex++ == uiRelNo )
      {
         hb_itemCopy( pExpr, lpdbRelations->abKey );
         return SUCCESS;
      }
      lpdbRelations = lpdbRelations->lpdbriNext;
   }

   return FAILURE;
}

/*
 * Set a relation in the parent file.
 */
static ERRCODE hb_waSetRel( AREAP pArea, LPDBRELINFO lpdbRelInf )
{
   LPDBRELINFO lpdbRelations;

   HB_TRACE(HB_TR_DEBUG, ("hb_waSetRel(%p, %p)", pArea, lpdbRelInf));

   lpdbRelations = pArea->lpdbRelations;
   if( ! lpdbRelations )
   {
      pArea->lpdbRelations = ( LPDBRELINFO ) hb_xgrab( sizeof( DBRELINFO ) );
      lpdbRelations = pArea->lpdbRelations;
   }
   else
   {
      while( lpdbRelations->lpdbriNext )
         lpdbRelations = lpdbRelations->lpdbriNext;
      lpdbRelations->lpdbriNext = ( LPDBRELINFO ) hb_xgrab( sizeof( DBRELINFO ) );
      lpdbRelations = lpdbRelations->lpdbriNext;
   }
   lpdbRelations->lpaParent = pArea;
   lpdbRelations->lpaChild = lpdbRelInf->lpaChild;
   lpdbRelations->itmCobExpr = lpdbRelInf->itmCobExpr;
   lpdbRelations->isScoped = lpdbRelInf->isScoped;
   lpdbRelations->isOptimized = lpdbRelInf->isOptimized;
   lpdbRelations->abKey = lpdbRelInf->abKey;
   lpdbRelations->lpdbriNext = lpdbRelInf->lpdbriNext;

   return SELF_CHILDSTART( ( AREAP ) lpdbRelInf->lpaChild, lpdbRelations );
}

/*
 * Clear the active filter expression.
 */
static ERRCODE hb_waClearFilter( AREAP pArea )
{
   HB_TRACE(HB_TR_DEBUG, ("hb_waClearFilter(%p)", pArea));

   /* Free all items */
   if( pArea->dbfi.itmCobExpr )
   {
      hb_itemRelease( pArea->dbfi.itmCobExpr );
      pArea->dbfi.itmCobExpr = NULL;
   }
   if( pArea->dbfi.abFilterText )
   {
      hb_itemRelease( pArea->dbfi.abFilterText );
      pArea->dbfi.abFilterText = NULL;
   }
   pArea->dbfi.fOptimized = FALSE;
   pArea->dbfi.fFilter = FALSE;

   return SUCCESS;
}

/*
 * Clear the active locate expression.
 */
static ERRCODE hb_waClearLocate( AREAP pArea )
{
   HB_TRACE(HB_TR_DEBUG, ("hb_waClearLocate(%p)", pArea));

   /* Free all items */
   if( pArea->dbsi.itmCobFor )
   {
      hb_itemRelease( pArea->dbsi.itmCobFor );
      pArea->dbsi.itmCobFor = NULL;
   }
   if( pArea->dbsi.lpstrFor )
   {
      hb_itemRelease( pArea->dbsi.lpstrFor );
      pArea->dbsi.lpstrFor = NULL;
   }
   if( pArea->dbsi.itmCobWhile )
   {
      hb_itemRelease( pArea->dbsi.itmCobWhile );
      pArea->dbsi.itmCobWhile = NULL;
   }
   if( pArea->dbsi.lpstrWhile )
   {
      hb_itemRelease( pArea->dbsi.lpstrWhile );
      pArea->dbsi.lpstrWhile = NULL;
   }
   if( pArea->dbsi.lNext )
   {
      hb_itemRelease( pArea->dbsi.lNext );
      pArea->dbsi.lNext = NULL;
   }
   if( pArea->dbsi.itmRecID )
   {
      hb_itemRelease( pArea->dbsi.itmRecID );
      pArea->dbsi.itmRecID = NULL;
   }
   if( pArea->dbsi.fRest )
   {
      hb_itemRelease( pArea->dbsi.fRest );
      pArea->dbsi.fRest = NULL;
   }

   return SUCCESS;
}

/*
 * Return filter condition of the specified WorkArea.
 */
static ERRCODE hb_waFilterText( AREAP pArea, PHB_ITEM pFilter )
{
   HB_TRACE(HB_TR_DEBUG, ("hb_waFilterText(%p, %p)", pArea, pFilter));

   if( pArea->dbfi.abFilterText )
      hb_itemCopy( pFilter, pArea->dbfi.abFilterText );

   return SUCCESS;
}

/*
 * Set the filter condition for the specified WorkArea.
 */
static ERRCODE hb_waSetFilter( AREAP pArea, LPDBFILTERINFO pFilterInfo )
{
   HB_TRACE(HB_TR_DEBUG, ("hb_waSetFilter(%p, %p)", pArea, pFilterInfo));

   /* Clear the active filter expression */
   if( SELF_CLEARFILTER( pArea ) != SUCCESS )
      return FAILURE;

   if( pFilterInfo->itmCobExpr )
   {
      pArea->dbfi.itmCobExpr = hb_itemNew( pFilterInfo->itmCobExpr );
   }
   if( pFilterInfo->abFilterText )
   {
      pArea->dbfi.abFilterText = hb_itemNew( pFilterInfo->abFilterText );
   }
   pArea->dbfi.fOptimized = pFilterInfo->fOptimized;
   pArea->dbfi.fFilter = TRUE;

   return SUCCESS;
}

/*
 * Set the locate scope for the specified WorkArea.
 */
static ERRCODE hb_waSetLocate( AREAP pArea, LPDBSCOPEINFO pScopeInfo )
{
   HB_TRACE(HB_TR_DEBUG, ("hb_waSetLocate(%p, %p)", pArea, pScopeInfo));

   /* Clear the active locate expression */
   if( SELF_CLEARLOCATE( pArea ) != SUCCESS )
      return FAILURE;

   if( pScopeInfo->itmCobFor )
      pArea->dbsi.itmCobFor = hb_itemNew( pScopeInfo->itmCobFor );

   if( pScopeInfo->lpstrFor )
      pArea->dbsi.lpstrFor = hb_itemNew( pScopeInfo->lpstrFor );

   if( pScopeInfo->itmCobWhile )
      pArea->dbsi.itmCobWhile = hb_itemNew( pScopeInfo->itmCobWhile );

   if( pScopeInfo->lpstrWhile )
      pArea->dbsi.lpstrWhile = hb_itemNew( pScopeInfo->lpstrWhile );

   if( pScopeInfo->lNext )
      pArea->dbsi.lNext = hb_itemNew( pScopeInfo->lNext );

   if( pScopeInfo->itmRecID )
      pArea->dbsi.itmRecID = hb_itemNew( pScopeInfo->itmRecID );

   if( pScopeInfo->fRest )
      pArea->dbsi.fRest = hb_itemNew( pScopeInfo->fRest );

   pArea->dbsi.fIgnoreFilter     = pScopeInfo->fIgnoreFilter;
   pArea->dbsi.fIncludeDeleted   = pScopeInfo->fIncludeDeleted;
   pArea->dbsi.fLast             = pScopeInfo->fLast;
   pArea->dbsi.fIgnoreDuplicates = pScopeInfo->fIgnoreDuplicates;
   pArea->dbsi.fBackward         = pScopeInfo->fBackward;
   pArea->dbsi.fOptimized        = pScopeInfo->fOptimized;

   return SUCCESS;
}

/*
 * Compile a character expression.
 */
static ERRCODE hb_waCompile( AREAP pArea, BYTE * pExpr )
{
   HB_MACRO_PTR pMacro;

   HB_TRACE(HB_TR_DEBUG, ("hb_waCompile(%p, %p)", pArea, pExpr));

   pMacro = hb_macroCompile( ( char * ) pExpr );
   if( pMacro )
   {
      pArea->valResult = hb_itemPutPtr( pArea->valResult, ( void * ) pMacro );
      return SUCCESS;
   }
   else
      return FAILURE;
}

/*
 * Raise a runtime error.
 */
static ERRCODE hb_waError( AREAP pArea, PHB_ITEM pError )
{
   char * szRddName;

   HB_TRACE(HB_TR_DEBUG, ("hb_waError(%p, %p)", pArea, pError));

   szRddName = ( char * ) hb_xgrab( HB_RDD_MAX_DRIVERNAME_LEN + 1 );
   if( pArea && pArea->lprfsHost->sysName )
      SELF_SYSNAME( pArea, ( BYTE * ) szRddName );
   else
      hb_strncpy( szRddName, "???DRIVER", HB_RDD_MAX_DRIVERNAME_LEN );
   hb_errPutSeverity( pError, ES_ERROR );
   hb_errPutSubSystem( pError, szRddName );
   hb_xfree( szRddName );
   return hb_errLaunch( pError );
}

/*
 * Evaluate a code block.
 */
static ERRCODE hb_waEvalBlock( AREAP pArea, PHB_ITEM pBlock )
{
   PHB_ITEM pItem;
   int iCurrArea;

   HB_TRACE(HB_TR_DEBUG, ("hb_waEvalBlock(%p, %p)", pArea, pBlock));

   iCurrArea = hb_rddGetCurrentWorkAreaNumber();
   if( iCurrArea != pArea->uiArea )
      hb_rddSelectWorkAreaNumber( pArea->uiArea );

   pItem = hb_vmEvalBlockOrMacro( pBlock );
   if( ! pArea->valResult )
      pArea->valResult = hb_itemNew( NULL );
   hb_itemCopy( pArea->valResult, pItem );

   hb_rddSelectWorkAreaNumber( iCurrArea );

   return hb_vmRequestQuery() ? FAILURE : SUCCESS;
}

/*
 * RDD info
 */
static ERRCODE hb_waRddInfo( LPRDDNODE pRDD, USHORT uiIndex, ULONG ulConnection, PHB_ITEM pItem )
{
   BOOL fResult;
   int iResult;

   HB_TRACE(HB_TR_DEBUG, ("hb_rddInfo(%p, %hu, %lu, %p)", pRDD, uiIndex, ulConnection, pItem));

   HB_SYMBOL_UNUSED( pRDD );
   HB_SYMBOL_UNUSED( ulConnection );

   switch( uiIndex )
   {
      case RDDI_ISDBF:
      case RDDI_CANPUTREC:
      case RDDI_LOCAL:
      case RDDI_REMOTE:
      case RDDI_RECORDMAP:
      case RDDI_ENCRYPTION:
      case RDDI_AUTOLOCK:
      case RDDI_STRUCTORD:
      case RDDI_LARGEFILE:
      case RDDI_MULTITAG:
      case RDDI_SORTRECNO:
      case RDDI_MULTIKEY:
      case RDDI_BLOB_SUPPORT:
         hb_itemPutL( pItem, FALSE );
         break;

      case RDDI_CONNECTION:
      case RDDI_TABLETYPE:
      case RDDI_MEMOTYPE:
      case RDDI_MEMOVERSION:
         hb_itemPutNI( pItem, 0 );
         break;

      case RDDI_STRICTREAD:
         fResult = hb_set.HB_SET_STRICTREAD;
         if( hb_itemType( pItem ) == HB_IT_LOGICAL )
            hb_set.HB_SET_STRICTREAD = hb_itemGetL( pItem );
         hb_itemPutL( pItem, fResult );
         break;
      case RDDI_OPTIMIZE:
         fResult = hb_set.HB_SET_OPTIMIZE;
         if( hb_itemType( pItem ) == HB_IT_LOGICAL )
            hb_set.HB_SET_OPTIMIZE = hb_itemGetL( pItem );
         hb_itemPutL( pItem, fResult );
         break;
      case RDDI_FORCEOPT:
         fResult = hb_set.HB_SET_FORCEOPT;
         if( hb_itemType( pItem ) == HB_IT_LOGICAL )
            hb_set.HB_SET_FORCEOPT = hb_itemGetL( pItem );
         hb_itemPutL( pItem, fResult );
         break;
      case RDDI_AUTOOPEN:
         fResult = hb_set.HB_SET_AUTOPEN;
         if( hb_itemType( pItem ) == HB_IT_LOGICAL )
            hb_set.HB_SET_AUTOPEN = hb_itemGetL( pItem );
         hb_itemPutL( pItem, fResult );
         break;
      case RDDI_AUTOORDER:
         fResult = hb_set.HB_SET_AUTORDER;
         if( hb_itemType( pItem ) == HB_IT_LOGICAL )
            hb_set.HB_SET_AUTORDER = hb_itemGetL( pItem );
         hb_itemPutL( pItem, fResult );
         break;
      case RDDI_AUTOSHARE:
         fResult = hb_set.HB_SET_AUTOSHARE;
         if( hb_itemType( pItem ) == HB_IT_LOGICAL )
            hb_set.HB_SET_AUTOSHARE = hb_itemGetL( pItem );
         hb_itemPutL( pItem, fResult );
         break;
      case RDDI_LOCKSCHEME:
         iResult = hb_set.HB_SET_DBFLOCKSCHEME;
         if( hb_itemType( pItem ) & HB_IT_NUMERIC )
            hb_set.HB_SET_DBFLOCKSCHEME = hb_itemGetNI( pItem );
         hb_itemPutNI( pItem, iResult );
         break;
      case RDDI_MEMOBLOCKSIZE:
         iResult = hb_set.HB_SET_MBLOCKSIZE;
         if( hb_itemType( pItem ) & HB_IT_NUMERIC )
            hb_set.HB_SET_MBLOCKSIZE = hb_itemGetNI( pItem );
         hb_itemPutNI( pItem, iResult );
         break;
      case RDDI_MEMOEXT:
         if( hb_itemType( pItem ) & HB_IT_STRING )
         {
            if( hb_set.HB_SET_MFILEEXT )
            {
               hb_itemPutC( pItem, hb_set.HB_SET_MFILEEXT );
               hb_xfree( hb_set.HB_SET_MFILEEXT );
            }
            else
            {
               hb_itemPutC( pItem, NULL );
            }
            hb_set.HB_SET_MFILEEXT = hb_strdup( hb_itemGetCPtr( pItem ) );
            break;
         }
         else if( hb_set.HB_SET_MFILEEXT )
         {
            hb_itemPutC( pItem, hb_set.HB_SET_MFILEEXT );
            break;
         }
         /* no break - return FAILURE */
      case RDDI_TABLEEXT:
      case RDDI_ORDBAGEXT:
      case RDDI_ORDEREXT:
      case RDDI_ORDSTRUCTEXT:
      case RDDI_DELIMITER:
      case RDDI_SEPARATOR:
      case RDDI_TRIGGER:
      case RDDI_PENDINGTRIGGER:
         hb_itemPutC( pItem, NULL );
         /* no break - return FAILURE */

      default:
         return FAILURE;
   }
   return SUCCESS;
}

/*
 * Raise a runtime error if an method is not defined.
 */
static ERRCODE hb_waUnsupported( AREAP pArea )
{
   PHB_ITEM pError;

   HB_TRACE(HB_TR_DEBUG, ("hb_waUnsupported(%p)", pArea));

   pError = hb_errNew();
   hb_errPutGenCode( pError, EG_UNSUPPORTED );
   hb_errPutDescription( pError, hb_langDGetErrorDesc( EG_UNSUPPORTED ) );
   SELF_ERROR( pArea, pError );
   hb_itemRelease( pError );

   return FAILURE;
}

/*
 * Raise a runtime error if an method is not defined.
 */
static ERRCODE hb_waRddUnsupported( LPRDDNODE pRDD )
{
   PHB_ITEM pError;

   HB_TRACE(HB_TR_DEBUG, ("hb_waRDDUnsupported(%p)", pRDD));

   pError = hb_errNew();
   hb_errPutGenCode( pError, EG_UNSUPPORTED );
   hb_errPutDescription( pError, hb_langDGetErrorDesc( EG_UNSUPPORTED ) );

   hb_errPutSeverity( pError, ES_ERROR );
   hb_errPutSubSystem( pError, pRDD->szName );
   hb_errLaunch( pError );
   hb_itemRelease( pError );

   return FAILURE;
}

#if 0
/*
 * Empty method.
 */
static ERRCODE hb_waNull( AREAP pArea )
{
   HB_TRACE(HB_TR_DEBUG, ("hb_waNull(%p)", pArea));

   HB_SYMBOL_UNUSED( pArea );

   return SUCCESS;
}
#endif

/*
 * The default virtual method table for all WorkAreas.
 */
static const RDDFUNCS waTable =
{
   /* Movement and positioning methods */
/* ( DBENTRYP_BP )   */ hb_waBof,               /* Bof        */
/* ( DBENTRYP_BP )   */ hb_waEof,               /* Eof        */
/* ( DBENTRYP_BP )   */ hb_waFound,             /* Found      */
   ( DBENTRYP_V )       hb_waUnsupported,       /* GoBottom   */
   ( DBENTRYP_UL )      hb_waUnsupported,       /* GoTo       */
   ( DBENTRYP_I )       hb_waUnsupported,       /* GoToId     */
   ( DBENTRYP_V )       hb_waUnsupported,       /* GoTop      */
   ( DBENTRYP_BIB )     hb_waUnsupported,       /* Seek       */
/* ( DBENTRYP_L )    */ hb_waSkip,              /* Skip       */
/* ( DBENTRYP_L )    */ hb_waSkipFilter,        /* SkipFilter */
   ( DBENTRYP_L )       hb_waUnsupported,       /* SkipRaw    */

   /* Data management */
/* ( DBENTRYP_VF )   */ hb_waAddField,          /* AddField       */
   ( DBENTRYP_B )       hb_waUnsupported,       /* Append         */
/* ( DBENTRYP_I )    */ hb_waCreateFields,      /* CreateFields   */
   ( DBENTRYP_V )       hb_waUnsupported,       /* DeleteRec      */
   ( DBENTRYP_BP )      hb_waUnsupported,       /* Deleted        */
/* ( DBENTRYP_SP )   */ hb_waFieldCount,        /* FieldCount     */
   ( DBENTRYP_VF )      hb_waUnsupported,       /* FieldDisplay   */
/* ( DBENTRYP_SSI )  */ hb_waFieldInfo,         /* FieldInfo      */
/* ( DBENTRYP_SVP )  */ hb_waFieldName,         /* FieldName      */
   ( DBENTRYP_V )       hb_waUnsupported,       /* Flush          */
   ( DBENTRYP_PP )      hb_waUnsupported,       /* GetRec         */
   ( DBENTRYP_SI )      hb_waUnsupported,       /* GetValue       */
   ( DBENTRYP_SVL )     hb_waUnsupported,       /* GetVarLen      */
   ( DBENTRYP_V )       hb_waUnsupported,       /* GoCold         */
   ( DBENTRYP_V )       hb_waUnsupported,       /* GoHot          */
   ( DBENTRYP_P )       hb_waUnsupported,       /* PutRec         */
   ( DBENTRYP_SI )      hb_waUnsupported,       /* PutValue       */
   ( DBENTRYP_V )       hb_waUnsupported,       /* Recall         */
   ( DBENTRYP_ULP )     hb_waUnsupported,       /* RecCount       */
   ( DBENTRYP_ISI )     hb_waUnsupported,       /* RecInfo        */
   ( DBENTRYP_ULP )     hb_waUnsupported,       /* RecNo          */
   ( DBENTRYP_I )       hb_waUnsupported,       /* RecId          */
/* ( DBENTRYP_S )    */ hb_waSetFieldExtent,    /* SetFieldExtent */

   /* WorkArea/Database management */
/* ( DBENTRYP_P )    */ hb_waAlias,             /* Alias       */
/* ( DBENTRYP_V )    */ hb_waClose,             /* Close       */
   /* Like in Clipper map CREATE() method at work area level to OPEN() */
/* ( DBENTRYP_VP )   */ hb_waOpen,              /* Create      */
/* ( DBENTRYP_SI )   */ hb_waInfo,              /* Info        */
/* ( DBENTRYP_V )    */ hb_waNewArea,           /* NewArea     */
/* ( DBENTRYP_VP )   */ hb_waOpen,              /* Open        */
/* ( DBENTRYP_V )    */ hb_waRelease,           /* Release     */
/* ( DBENTRYP_SP )   */ hb_waStructSize,        /* StructSize  */
/* ( DBENTRYP_P )    */ hb_waSysName,           /* SysName     */
/* ( DBENTRYP_VEI )  */ hb_waEval,              /* Eval        */
   ( DBENTRYP_V )       hb_waUnsupported,       /* Pack        */
   ( DBENTRYP_LSP )     hb_waUnsupported,       /* PackRec     */
   ( DBENTRYP_VS )      hb_waUnsupported,       /* Sort        */
/* ( DBENTRYP_VT )   */ hb_waTrans,             /* Trans       */
/* ( DBENTRYP_VT )   */ hb_waTransRec,          /* TransRec    */
   ( DBENTRYP_V )       hb_waUnsupported,       /* Zap         */

   /* Relational Methods */
/* ( DBENTRYP_VR )   */ hb_waChildEnd,          /* ChildEnd      */
/* ( DBENTRYP_VR )   */ hb_waChildStart,        /* ChildStart    */
   ( DBENTRYP_VR )      hb_waUnsupported,       /* ChildSync     */
/* ( DBENTRYP_V )    */ hb_waSyncChildren,      /* SyncChildren  */
/* ( DBENTRYP_V )    */ hb_waClearRel,          /* ClearRel      */
   ( DBENTRYP_V )       hb_waUnsupported,       /* ForceRel      */
/* ( DBENTRYP_SVP )  */ hb_waRelArea,           /* RelArea       */
/* ( DBENTRYP_VR )   */ hb_waRelEval,           /* RelEval       */
/* ( DBENTRYP_SI )   */ hb_waRelText,           /* RelText       */
/* ( DBENTRYP_VR )   */ hb_waSetRel,            /* SetRel        */

   /* Order Management */
   ( DBENTRYP_OI )      hb_waUnsupported,       /* OrderListAdd      */
   ( DBENTRYP_V )       hb_waUnsupported,       /* OrderListClear    */
   ( DBENTRYP_OI )      hb_waUnsupported,       /* OrderListDelete   */
   ( DBENTRYP_OI )      hb_waUnsupported,       /* OrderListFocus    */
   ( DBENTRYP_V )       hb_waUnsupported,       /* OrderListRebuild  */
/* ( DBENTRYP_VOI )  */ hb_waOrderCondition,    /* OrderCondition    */
   ( DBENTRYP_VOC )     hb_waUnsupported,       /* OrderCreate       */
   ( DBENTRYP_OI )      hb_waUnsupported,       /* OrderDestroy      */
/* ( DBENTRYP_OII )  */ hb_waOrderInfo,         /* OrderInfo         */

   /* Filters and Scope Settings */
/* ( DBENTRYP_V )    */ hb_waClearFilter,       /* ClearFilter  */
/* ( DBENTRYP_V )    */ hb_waClearLocate,       /* ClearLocate  */
   ( DBENTRYP_V )       hb_waUnsupported,       /* ClearScope   */
   ( DBENTRYP_VPLP )    hb_waUnsupported,       /* CountScope   */
/* ( DBENTRYP_I )    */ hb_waFilterText,        /* FilterText   */
   ( DBENTRYP_SI )      hb_waUnsupported,       /* ScopeInfo    */
/* ( DBENTRYP_VFI )  */ hb_waSetFilter,         /* SetFilter    */
/* ( DBENTRYP_VLO )  */ hb_waSetLocate,         /* SetLocate    */
   ( DBENTRYP_VOS )     hb_waUnsupported,       /* SetScope     */
   ( DBENTRYP_VPL )     hb_waUnsupported,       /* SkipScope    */
/* ( DBENTRYP_B )    */ hb_waLocate,            /* Locate       */

   /* Miscellaneous */
/* ( DBENTRYP_P )    */ hb_waCompile,           /* Compile    */
/* ( DBENTRYP_I )    */ hb_waError,             /* Error      */
/* ( DBENTRYP_I )    */ hb_waEvalBlock,         /* EvalBlock  */

   /* Network operations */
   ( DBENTRYP_VSP )     hb_waUnsupported,       /* RawLock  */
   ( DBENTRYP_VL )      hb_waUnsupported,       /* Lock     */
   ( DBENTRYP_I )       hb_waUnsupported,       /* UnLock   */

   /* Memofile functions */
   ( DBENTRYP_V )       hb_waUnsupported,       /* CloseMemFile   */
   ( DBENTRYP_VP )      hb_waUnsupported,       /* CreateMemFile  */
   ( DBENTRYP_SVPB )    hb_waUnsupported,       /* GetValueFile   */
   ( DBENTRYP_VP )      hb_waUnsupported,       /* OpenMemFile    */
   ( DBENTRYP_SVPB )    hb_waUnsupported,       /* PutValueFile   */

   /* Database file header handling */
   ( DBENTRYP_V )       hb_waUnsupported,       /* ReadDBHeader   */
   ( DBENTRYP_V )       hb_waUnsupported,       /* WriteDBHeader  */

   /* non WorkArea functions */
   ( DBENTRYP_R )       NULL,                   /* Init    */
   ( DBENTRYP_R )       NULL,                   /* Exit    */
   ( DBENTRYP_RVVL )    hb_waRddUnsupported,    /* Drop    */
   ( DBENTRYP_RVVL )    hb_waRddUnsupported,    /* Exists  */
/* ( DBENTRYP_RSLV ) */ hb_waRddInfo,           /* RddInfo */

   /* Special and reserved methods */
   ( DBENTRYP_SVP )   hb_waUnsupported          /* WhoCares */
};

/* common for all threads list of registered RDDs */
static LPRDDNODE * s_RddList = NULL;   /* Registered RDDs */
static USHORT s_uiRddMax = 0;          /* Number of registered RDD */

/*
 * Get RDD node poionter
 */
HB_EXPORT LPRDDNODE hb_rddGetNode( USHORT uiNode )
{
   HB_TRACE(HB_TR_DEBUG, ("hb_rddGetNode(%hu)", uiNode));

   return uiNode < s_uiRddMax ? s_RddList[ uiNode ] : NULL;
}

HB_EXPORT PHB_ITEM hb_rddList( USHORT uiType )
{
   USHORT uiCount, uiIndex, uiRdds;
   PHB_ITEM pRddArray;
   LPRDDNODE pNode;

   HB_TRACE(HB_TR_DEBUG, ("hb_rddList(%hu)", uiType));

   for( uiCount = uiRdds = 0; uiCount < s_uiRddMax; ++uiCount )
   {
      if( uiType == 0 || s_RddList[ uiCount ]->uiType == uiType )
         ++uiRdds;
   }
   pRddArray = hb_itemArrayNew( uiRdds );
   for( uiCount = uiIndex = 0; uiCount < s_uiRddMax && uiIndex < uiRdds; ++uiCount )
   {
      pNode = s_RddList[ uiCount ];
      if( uiType == 0 || pNode->uiType == uiType )
         hb_itemPutC( hb_arrayGetItemPtr( pRddArray, ++uiIndex ), pNode->szName );
   }
   return pRddArray;
}

/*
 * Find a RDD node.
 */
HB_EXPORT LPRDDNODE hb_rddFindNode( const char * szDriver, USHORT * uiIndex )
{
   USHORT uiCount;

   HB_TRACE(HB_TR_DEBUG, ("hb_rddFindNode(%s, %p)", szDriver, uiIndex));

   for( uiCount = 0; uiCount < s_uiRddMax; uiCount++ )
   {
      LPRDDNODE pNode = s_RddList[ uiCount ];
      if( strcmp( pNode->szName, szDriver ) == 0 ) /* Matched RDD */
      {
         if( uiIndex )
            * uiIndex = uiCount;
         return pNode;
      }
   }
   if( uiIndex )
      * uiIndex = 0;
   return NULL;
}

/*
 * Shutdown the RDD system.
 */
HB_EXPORT void hb_rddShutDown( void )
{
   USHORT uiCount;

   HB_TRACE(HB_TR_DEBUG, ("hb_rddShutDown()"));

   if( s_uiRddMax > 0 )
   {
      hb_rddCloseAll();
      for( uiCount = 0; uiCount < s_uiRddMax; uiCount++ )
      {
         if( s_RddList[ uiCount ]->pTable.exit != NULL )
         {
            SELF_EXIT( s_RddList[ uiCount ] );
         }
         hb_xfree( s_RddList[ uiCount ] );
      }
      hb_xfree( s_RddList );
      s_RddList = NULL;
      s_uiRddMax = 0;
   }
}

/*
 * Register a RDD driver.
 */
HB_EXPORT int hb_rddRegister( const char * szDriver, USHORT uiType )
{
   LPRDDNODE pRddNewNode;
   PHB_DYNS pGetFuncTable;
   char szGetFuncTable[ HB_RDD_MAX_DRIVERNAME_LEN + 14 ];
   USHORT uiFunctions;

   HB_TRACE(HB_TR_DEBUG, ("hb_rddRegister(%s, %hu)", szDriver, uiType));

   if( hb_rddFindNode( szDriver, NULL ) )    /* Duplicated RDD */
   {
      return 1;
   }

   snprintf( szGetFuncTable, sizeof( szGetFuncTable ), "%s_GETFUNCTABLE",
             szDriver );
   pGetFuncTable = hb_dynsymFindName( szGetFuncTable );
   if( !pGetFuncTable )
   {
      return 2;              /* Not valid RDD */
   }

   /* Create a new RDD node */
   pRddNewNode = ( LPRDDNODE ) hb_xgrab( sizeof( RDDNODE ) );
   memset( pRddNewNode, 0, sizeof( RDDNODE ) );

   /* Fill the new RDD node */
   hb_strncpy( pRddNewNode->szName, szDriver, sizeof( pRddNewNode->szName ) - 1 );
   pRddNewNode->uiType = uiType;
   pRddNewNode->rddID = s_uiRddMax;

   /* Call <szDriver>_GETFUNCTABLE() */
   hb_vmPushSymbol( hb_dynsymSymbol( pGetFuncTable ) );
   hb_vmPushNil();
   hb_vmPushPointer( ( void * ) &uiFunctions );
   hb_vmPushPointer( ( void * ) &pRddNewNode->pTable );
   hb_vmPushPointer( ( void * ) &pRddNewNode->pSuperTable );
   hb_vmPushInteger( s_uiRddMax );
   hb_vmDo( 4 );
   if( hb_parni( -1 ) != SUCCESS )
   {
      hb_xfree( pRddNewNode );         /* Delete de new RDD node */
      return 3;                        /* Invalid FUNCTABLE */
   }

   if( s_uiRddMax == 0 )                /* First RDD node */
      s_RddList = (LPRDDNODE *) hb_xgrab( sizeof(LPRDDNODE) );
   else
      s_RddList = (LPRDDNODE *) hb_xrealloc( s_RddList, sizeof(LPRDDNODE) * ( s_uiRddMax + 1 ) );

   s_RddList[ s_uiRddMax++ ] = pRddNewNode;   /* Add the new RDD node */

   if( pRddNewNode->pTable.init != NULL )
   {
      SELF_INIT( pRddNewNode );
   }

   return 0;                           /* Ok */
}

/*
 * pTable - a table in new RDDNODE that will be filled
 * pSubTable - a table with a list of supported functions
 * pSuperTable - a current table in a RDDNODE
 * szDrvName - a driver name that will be inherited
 */
HB_EXPORT ERRCODE hb_rddInherit( RDDFUNCS * pTable, const RDDFUNCS * pSubTable, RDDFUNCS * pSuperTable, const char * szDrvName )
{
   LPRDDNODE pRddNode;
   USHORT uiCount;
   DBENTRYP_V * pFunction, * pSubFunction;

   HB_TRACE(HB_TR_DEBUG, ("hb_rddInherit(%p, %p, %p, %s)", pTable, pSubTable, pSuperTable, szDrvName));

   if( !pTable )
   {
      return FAILURE;
   }

   /* Copy the pSuperTable into pTable */
   if( !szDrvName || ! *szDrvName )
   {
      /* no name for inherited driver - use the default one */
      memcpy( pTable, &waTable, sizeof( RDDFUNCS ) );
      memcpy( pSuperTable, &waTable, sizeof( RDDFUNCS ) );
   }
   else
   {
      char szSuperName[ HB_RDD_MAX_DRIVERNAME_LEN + 1 ];
      hb_strncpyUpper( szSuperName, szDrvName, sizeof( szSuperName ) - 1 );
      pRddNode = hb_rddFindNode( szSuperName, NULL );

      if( !pRddNode )
      {
         return FAILURE;
      }

      memcpy( pTable, &pRddNode->pTable, sizeof( RDDFUNCS ) );
      memcpy( pSuperTable, &pRddNode->pTable, sizeof( RDDFUNCS ) );
   }

   /* Copy the non NULL entries from pSubTable into pTable */
   pFunction = ( DBENTRYP_V * ) pTable;
   pSubFunction = ( DBENTRYP_V * ) pSubTable;
   for( uiCount = 0; uiCount < RDDFUNCSCOUNT; uiCount++ )
   {
      if( * pSubFunction )
         * pFunction = * pSubFunction;
      pFunction ++;
      pSubFunction ++;
   }
   return SUCCESS;
}

HB_FUNC_EXTERN( RDDSYS );
void _hb_rddWorkAreaForceLink( void )
{
   HB_FUNC_EXEC( RDDSYS );
}
