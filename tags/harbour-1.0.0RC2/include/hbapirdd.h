/*
 * $Id$
 */

/*
 * Harbour Project source code:
 * Header file for the RDD API
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

#ifndef HB_APIRDD_H_
#define HB_APIRDD_H_

#include "hbapifs.h"
#include "dbinfo.ch"   /* Constants for SELF_ORDINFO, SELF_INFO(), SELF_RECINFO() */
#include "dbstruct.ch" /* Constants for SELF_FIELDINFO() */
#include "hbapicdp.h"

HB_EXTERN_BEGIN

#define HARBOUR_MAX_RDD_DRIVERNAME_LENGTH          32

#ifndef HARBOUR_MAX_RDD_ALIAS_LENGTH
   #define HARBOUR_MAX_RDD_ALIAS_LENGTH            32
#endif

/* #define HARBOUR_MAX_RDD_FIELDNAME_LENGTH        32 */
#define HARBOUR_MAX_RDD_AREA_NUM                65535

/* DBCMD errors */

#define EDBCMD_SEEK_BADPARAMETER          1001
#define EDBCMD_NOALIAS                    1002
#define EDBCMD_NOVAR                      1003
#define EDBCMD_USE_BADPARAMETER           1005
#define EDBCMD_REL_BADPARAMETER           1006
#define EDBCMD_ORDLSTADD_BADPARAMETER     1008
#define EDBCMD_FIELDNAME_BADPARAMETER     1009
#define EDBCMD_BADALIAS                   1010
#define EDBCMD_DUPALIAS                   1011
#define EDBCMD_DBCMDBADPARAMETER          1014
#define EDBCMD_BADPARAMETER               1015
#define EDBCMD_INFOBADPARAMETER           1032
#define EDBCMD_DBINFOBADPARAMETER         1034
#define EDBCMD_DBFILEPUTBADPARAMETER      1041
#define EDBCMD_DBFILEGETBADPARAMETER      1042
#define EDBCMD_NOTABLE                    2001
#define EDBCMD_EVAL_BADPARAMETER          2019



/* FIeld types */

#define HB_FT_NONE            0
#define HB_FT_STRING          1     /* "C" */
#define HB_FT_LOGICAL         2     /* "L" */
#define HB_FT_DATE            3     /* "D" */
#define HB_FT_LONG            4     /* "N" */
#define HB_FT_FLOAT           5     /* "F" */
#define HB_FT_INTEGER         6     /* "I" */
#define HB_FT_DOUBLE          7     /* "B" */
#define HB_FT_TIME            8     /* "T" */
#define HB_FT_DAYTIME         9     /* "@" */
#define HB_FT_MODTIME         10    /* "=" */
#define HB_FT_ROWVER          11    /* "^" */
#define HB_FT_AUTOINC         12    /* "+" */
#define HB_FT_CURRENCY        13    /* "Y" */
#define HB_FT_CURDOUBLE       14    /* "Z" */
#define HB_FT_VARLENGTH       15    /* "Q" */
#define HB_FT_MEMO            16    /* "M" */
#define HB_FT_ANY             17    /* "V" */
#define HB_FT_IMAGE           18    /* "P" */
#define HB_FT_BLOB            19    /* "W" */
#define HB_FT_OLE             20    /* "G" */



/* FIeld flags */

#define HB_FF_HIDDEN          0x0001 /* System Column (not visible to user) */
#define HB_FF_NULLABLE        0x0002 /* Column can store null values */
#define HB_FF_BINARY          0x0004 /* Binary column */
#define HB_FF_AUTOINC         0x0008 /* Column is autoincrementing */
#define HB_FF_COMPRESSED      0x0010 /* Column is compressed */
#define HB_FF_ENCRYPTED       0x0020 /* Column is encrypted */



/* Flags for DBTRANSINFO */

#define DBTF_MATCH         0x0001
#define DBTF_PUTREC        0x0002



/* Codes for Locking methods */

#define DBLM_EXCLUSIVE     1
#define DBLM_MULTIPLE      2
#define DBLM_FILE          3


/* Codes for RawLock types */

#define FILE_LOCK          1
#define FILE_UNLOCK        2
#define REC_LOCK           3
#define REC_UNLOCK         4
#define HEADER_LOCK        5
#define HEADER_UNLOCK      6
#define APPEND_LOCK        7
#define APPEND_UNLOCK      8



/*
 * Forward declarations
 */
struct _RDDFUNCS;
struct _AREA;
struct _RDDNODE;


/*
*  DBFIELDINFO
*  -----------
*  The field structure
*/

typedef struct
{
   BYTE *   atomName;         /* FIELD (symbol) name */
   HB_TYPE  uiType;           /* FIELD type */
   USHORT   uiTypeExtended;   /* FIELD type extended */
   USHORT   uiLen;            /* Overall FIELD length */
   USHORT   uiDec;            /* Decimal places of numeric FIELD */
   USHORT   uiFlags;          /* FIELD flags */
} DBFIELDINFO;

typedef DBFIELDINFO * LPDBFIELDINFO;



/*
 *  DBOPENINFO
 *  ----------
 *  The Open Info structure
 */

typedef struct
{
   USHORT uiArea;          /* Work Area number of the data store */
   BYTE * abName;          /* The qualified name of the data store */
   BYTE * atomAlias;       /* The logical name of the data store */
   BOOL   fShared;         /* Share mode of the data store */
   BOOL   fReadonly;       /* Readonly mode of the data store */
   BYTE * cdpId;           /* Id of a codepage */
   ULONG  ulConnection;    /* connection handler for RDDs which support it */
   void * lpdbHeader;      /* Pointer to a header of the data store */
} DBOPENINFO;

typedef DBOPENINFO * LPDBOPENINFO;



/*
 *  DBORDERCONDINFO
 *  ---------------
 *  The Create Order conditional Info structure
 */

typedef struct _DBORDERCONDINFO
{
   BOOL     fActive;
   BYTE *   abFor;
   BYTE *   abWhile;
   PHB_ITEM itmCobFor;
   PHB_ITEM itmCobWhile;
   PHB_ITEM itmCobEval;
   LONG     lStep;
   PHB_ITEM itmStartRecID;
   LONG     lNextCount;
   PHB_ITEM itmRecID;
   BOOL     fRest;
   BOOL     fDescending;
   BOOL     fScoped;
   BOOL     fAll;
   BOOL     fAdditive;
   BOOL     fUseCurrent;
   BOOL     fCustom;
   BOOL     fNoOptimize;
   BOOL     fCompound;
   BOOL     fUseFilter;
   BOOL     fTemporary;
   BOOL     fExclusive;
   void *   lpvCargo;
} DBORDERCONDINFO;

typedef DBORDERCONDINFO * LPDBORDERCONDINFO;



typedef struct
{
   BYTE *             abConstrName;       /* Name of relational integrity constraint */
   BYTE *             abTargetName;       /* Name of target relation table */
   PHB_ITEM           itmRelationKey;     /* Array of columns in source table to match target primary key */
   BOOL               fEnabled;           /* Is constraint enabled ? */
} DBCONSTRAINTINFO;

typedef DBCONSTRAINTINFO * LPDBCONSTRAINTINFO;


/*
 *  DBORDERCREATE
 *  -------------
 *  The Create Order Info structure
 */

typedef struct
{
   LPDBORDERCONDINFO  lpdbOrdCondInfo;    /* Conditional information */
   BYTE *             abBagName;          /* Name of the Order bag */
   BYTE *             atomBagName;        /* Name of the Order */
   PHB_ITEM           itmOrder;
   BOOL               fUnique;            /* Flag to determine if all keys are unique */
   PHB_ITEM           itmCobExpr;         /* Code block containing the KEY expression */
   PHB_ITEM           abExpr;             /* String containing the KEY expression */
   LPDBCONSTRAINTINFO lpdbConstraintInfo; /* Relational constraint info */
} DBORDERCREATEINFO;

typedef DBORDERCREATEINFO * LPDBORDERCREATEINFO;



/*
 *  DBORDERINFO
 *  -----------
 *  The Set Index Info structure
 */

typedef struct
{
   PHB_ITEM atomBagName;  /* Name of the Order Bag */
   PHB_ITEM itmOrder;     /* Name or Number of the Order */
   PHB_ITEM itmCobExpr;   /* Code block containing the KEY expression */
   PHB_ITEM itmResult;    /* Operation result */
   PHB_ITEM itmNewVal;    /* New Setting   */
   BOOL     fAllTags;     /* Open all tags */
} DBORDERINFO;

typedef DBORDERINFO * LPDBORDERINFO;



/*
 *  DBSCOPEINFO
 *  -----------
 *  The Scope Info structure
 */

typedef struct
{
   PHB_ITEM itmCobFor;   /* Code Block representation of a FOR clause */
   PHB_ITEM lpstrFor;    /* String representation of a FOR clause */
   PHB_ITEM itmCobWhile; /* Code Block representation of a WHILE clause */
   PHB_ITEM lpstrWhile;  /* String representation of a WHILE clause */
   PHB_ITEM lNext;       /* NEXT record */
   PHB_ITEM itmRecID;    /* single record ID */
   PHB_ITEM fRest;       /* TRUE if start from the current record */
   BOOL     fIgnoreFilter;       /* process should ignore any filter condition */
   BOOL     fIncludeDeleted;     /* process should include deleted records */
   BOOL     fLast;               /* last record of the current scope required */
   BOOL     fIgnoreDuplicates;   /* process should ignore duplicate key value */
   BOOL     fBackward;           /* skip backward */
   BOOL     fOptimized;          /* Is (should be) scope optimized */
} DBSCOPEINFO;

typedef DBSCOPEINFO * LPDBSCOPEINFO;


/*
 *  DBORDSCOPEINFO
 *  --------------
 *  The Order Scope Info structure
 */

typedef struct
{
   USHORT nScope;        /* scope operation: TOPSCOPE/ENDSCOPE */
   PHB_ITEM scopeValue;
} DBORDSCOPEINFO;

typedef DBORDSCOPEINFO * LPDBORDSCOPEINFO;


/*
 *  DBFILTERINFO
 *  ------------
 *  The Filter Info structure
 */

typedef struct
{
   PHB_ITEM itmCobExpr;       /* Block representation of the FILTER expression */
   PHB_ITEM abFilterText;     /* String representation of FILTER expression */
   BOOL     fFilter;          /* flag to indicate that filter is active */
   BOOL     fOptimized;       /* Is (should be) filter optimized */
   void *   lpvCargo;         /* RDD specific extended filter info */
} DBFILTERINFO;

typedef DBFILTERINFO * LPDBFILTERINFO;



/*
 *  DBRELINFO
 *  ---------
 *  The Relationship Info structure
 */

typedef struct _DBRELINFO
{
   PHB_ITEM            itmCobExpr;   /* Block representation of the relational SEEK key */
   PHB_ITEM            abKey;        /* String representation of the relational SEEK key */
   BOOL                isScoped;     /* Is this relation scoped */
   BOOL                isOptimized;  /* Is relation optimized */
   struct _AREA      * lpaParent;    /* The parent of this relation */
   struct _AREA      * lpaChild;     /* The parents children */
   struct _DBRELINFO * lpdbriNext;   /* Next child or parent */
} DBRELINFO;

typedef DBRELINFO * LPDBRELINFO;



/*
 *  DBEVALINFO
 *  ----------
 *  The Evaluation Info structure
 *
 *  Contains information necessary for a block evaluation
 *  on each record of the workarea
 */

typedef struct
{
   PHB_ITEM    itmBlock;   /* The block to be evaluated */
   PHB_ITEM    abBlock;    /* String representation of evaluated block */
   DBSCOPEINFO dbsci;      /* Scope info that limits the evaluation */
} DBEVALINFO;

typedef DBEVALINFO * LPDBEVALINFO;

/*
 * NOTE: If your redefine EVAL() method then you may use itmBlock as
 * string ITEM to make some operations on server side of remote RDD.
 */


/*
 *  DBTRANSITEM
 *  -----------
 *  The Transfer Item structure
 *
 *  Defines a single transfer item (usually a field) from
 *  one database to another; used by DBTRANSINFO
 */

typedef struct
{
   USHORT uiSource;       /* Field index number from the source */
   USHORT uiDest;         /* Destination field index number */
} DBTRANSITEM;

typedef DBTRANSITEM * LPDBTRANSITEM;



/*
 *  DBTRANSINFO
 *  -----------
 *  The Transfer Info structure
 *
 *  Defines a global transfer of data items from on workarea
 *  to another
 */

typedef struct
{
   struct _AREA * lpaSource;     /* Pointer to source work area */
   struct _AREA * lpaDest;       /* Pointer to dest work area */
   DBSCOPEINFO    dbsci;         /* Scope to limit transfer */
   USHORT         uiFlags;       /* Transfer attributes */
   USHORT         uiItemCount;   /* Number of items below */
   LPDBTRANSITEM  lpTransItems;  /* Array of items */
} DBTRANSINFO;

typedef DBTRANSINFO * LPDBTRANSINFO;



/*
 *  DBSORTITEM
 *  ----------
 *  The Sort Item Structure
 *
 *  An array of items that, together, indicate the key value to
 *  use while sorting data. The order of the array determines the
 *  order of the sorting.
 */

typedef struct
{
   USHORT uiField;        /* Index into the workarea->fields structure */
   USHORT uiFlags;        /* Sort flags */
} DBSORTITEM;

typedef DBSORTITEM * LPDBSORTITEM;


/* Flags for DBSORTITEM */
#define SF_ASCEND       1
#define SF_CASE         2
#define SF_DESCEND      4
#define SF_NUM         32
#define SF_DOUBLE      64
#define SF_LONG       128



/*
 *  DBSORTINFO
 *  ----------
 *  The Sort Info Structure
 *
 *  Information for a physical sort on the workarea
 */

typedef struct
{
   DBTRANSINFO   dbtri;        /* Destination workarea transfer information */
   LPDBSORTITEM  lpdbsItem;    /* Fields which compose the key values for the sort */
   USHORT        uiItemCount;  /* The number of fields above */
} DBSORTINFO;

typedef DBSORTINFO * LPDBSORTINFO;



/*
 *  DBLOCKINFO
 *  ----------
 *  The Lock Info Structure
 *
 *  Information for a record or file lock
 */

typedef struct
{
   PHB_ITEM itmRecID;
   USHORT   uiMethod;
   BOOL     fResult;
} DBLOCKINFO;

typedef DBLOCKINFO * LPDBLOCKINFO;



/*
 *  FIELD
 *  -----
 *  The Field structure
 *
 *  This is the basic unit of access for a workarea
 */

typedef struct _FIELD
{
   HB_TYPE  uiType;           /* Field type */
   USHORT   uiTypeExtended;   /* Field type - extended */
   USHORT   uiLen;            /* Field length */
   USHORT   uiDec;            /* Decimal length */
   USHORT   uiFlags;          /* FIELD flags */
   USHORT   uiArea;           /* Area this field resides in */
   void *   sym;              /* Symbol that represents the field */
   struct _FIELD * lpfNext;   /* The next field in the list */
} FIELD;

typedef FIELD * LPFIELD;

/*
 * prototype for function to evaluate against index keys
 * only for local RDDs (DBFNTX, DBFCDX, ...)
 */
typedef void ( * HB_EVALSCOPE_FUNC )( ULONG, BYTE *, ULONG, void * );


/*--------------------* WORKAREA structure *----------------------*/

/*
 *  WORKAREA
 *  --------
 *  The Workarea Structure
 *
 *  Information to administrate the workarea
 */

typedef struct _AREA
{
   struct _RDDFUNCS * lprfsHost; /* Virtual method table for this workarea */
#if 0
   /* I'll add this soon, Druzus */
   struct _RDDFUNCS * lprfsSuper;/* Virtual super method table for this workarea */
#endif
   USHORT uiArea;                /* The number assigned to this workarea */
   void * atomAlias;             /* Pointer to the alias symbol for this workarea */
   USHORT uiFieldExtent;         /* Total number of fields allocated */
   USHORT uiFieldCount;          /* Total number of fields used */
   LPFIELD lpFields;             /* Pointer to an array of fields */
   void * lpFieldExtents;        /* Void ptr for additional field properties */
   PHB_ITEM valResult;           /* All purpose result holder */
   BOOL fTop;                    /* TRUE if "top" */
   BOOL fBottom;                 /* TRUE if "bottom" */
   BOOL fBof;                    /* TRUE if "bof" */
   BOOL fEof;                    /* TRUE if "eof" */
   BOOL fFound;                  /* TRUE if "found" */
   DBSCOPEINFO dbsi;             /* Info regarding last LOCATE */
   DBFILTERINFO dbfi;            /* Filter in effect */
   LPDBORDERCONDINFO lpdbOrdCondInfo;
   LPDBRELINFO lpdbRelations;    /* Parent/Child relationships used */
   USHORT uiParents;             /* Number of parents for this area */
   USHORT heap;
   USHORT heapSize;
   USHORT rddID;
   USHORT uiMaxFieldNameLength;
   PHB_CODEPAGE cdPage;          /* Area's codepage pointer */
} AREA;

typedef AREA * LPAREA;

#ifndef AREAP
#define AREAP LPAREA
#endif


/*--------------------* Virtual Method Table *----------------------*/

typedef USHORT ( * DBENTRYP_V    )( AREAP area );
typedef USHORT ( * DBENTRYP_BP   )( AREAP area, BOOL * param );
typedef USHORT ( * DBENTRYP_B    )( AREAP area, BOOL param );
typedef USHORT ( * DBENTRYP_L    )( AREAP area, LONG param );
typedef USHORT ( * DBENTRYP_UL   )( AREAP area, ULONG param );
typedef USHORT ( * DBENTRYP_I    )( AREAP area, PHB_ITEM param );
typedef USHORT ( * DBENTRYP_SI   )( AREAP area, USHORT index, PHB_ITEM param );
typedef USHORT ( * DBENTRYP_VP   )( AREAP area, LPDBOPENINFO param );
typedef USHORT ( * DBENTRYP_VT   )( AREAP area, LPDBTRANSINFO param );
typedef USHORT ( * DBENTRYP_VF   )( AREAP area, LPDBFIELDINFO param );
typedef USHORT ( * DBENTRYP_VL   )( AREAP area, LPDBLOCKINFO param );
typedef USHORT ( * DBENTRYP_VR   )( AREAP area, LPDBRELINFO param );
typedef USHORT ( * DBENTRYP_VS   )( AREAP area, LPDBSORTINFO param );
typedef USHORT ( * DBENTRYP_VFI  )( AREAP area, LPDBFILTERINFO param );
typedef USHORT ( * DBENTRYP_VEI  )( AREAP area, LPDBEVALINFO param );
typedef USHORT ( * DBENTRYP_VLO  )( AREAP area, LPDBSCOPEINFO param );
typedef USHORT ( * DBENTRYP_VOC  )( AREAP area, LPDBORDERCREATEINFO param );
typedef USHORT ( * DBENTRYP_VOI  )( AREAP area, LPDBORDERCONDINFO param );
typedef USHORT ( * DBENTRYP_VOS  )( AREAP area, LPDBORDSCOPEINFO param );
typedef USHORT ( * DBENTRYP_OI   )( AREAP area, LPDBORDERINFO param );
typedef USHORT ( * DBENTRYP_OII  )( AREAP area, USHORT index, LPDBORDERINFO param );
typedef USHORT ( * DBENTRYP_SP   )( AREAP area, USHORT * param );
typedef USHORT ( * DBENTRYP_P    )( AREAP area, BYTE * param );
typedef USHORT ( * DBENTRYP_PP   )( AREAP area, BYTE ** param );
typedef USHORT ( * DBENTRYP_S    )( AREAP area, USHORT param );
typedef USHORT ( * DBENTRYP_LP   )( AREAP area, LONG * param );
typedef USHORT ( * DBENTRYP_ULP  )( AREAP area, ULONG * param );
typedef USHORT ( * DBENTRYP_SVP  )( AREAP area, USHORT index, void * param );
typedef USHORT ( * DBENTRYP_SVPB )( AREAP area, USHORT index, void * param, USHORT p3 );
typedef USHORT ( * DBENTRYP_VSP  )( AREAP area, USHORT action, ULONG lRecord );
typedef USHORT ( * DBENTRYP_SVL  )( AREAP area, USHORT index, ULONG * param );
typedef USHORT ( * DBENTRYP_SSI  )( AREAP area, USHORT p1, USHORT p2, PHB_ITEM p3 );
typedef USHORT ( * DBENTRYP_ISI  )( AREAP area, PHB_ITEM p1, USHORT p2, PHB_ITEM p3 );
typedef USHORT ( * DBENTRYP_BIB  )( AREAP area, BOOL p1, PHB_ITEM p2, BOOL p3 );
typedef USHORT ( * DBENTRYP_VPL  )( AREAP area, void * p1, LONG p2 );
typedef USHORT ( * DBENTRYP_VPLP )( AREAP area, void * p1, LONG * p2 );
typedef USHORT ( * DBENTRYP_LSP  )( AREAP area, ULONG p1, BOOL * p2 );

/* this methods DO USE take a Workarea but an RDDNODE */

typedef USHORT ( * DBENTRYP_R    )( struct _RDDNODE * pRDD );
typedef USHORT ( * DBENTRYP_RVVL )( struct _RDDNODE * pRDD, PHB_ITEM p1, PHB_ITEM p2, ULONG p3 );
typedef USHORT ( * DBENTRYP_RSLV )( struct _RDDNODE * pRDD, USHORT index, ULONG p1, PHB_ITEM p2 );
/*--------------------* Virtual Method Table *----------------------*/

typedef struct _RDDFUNCS
{
   /* Movement and positioning methods */

   DBENTRYP_BP   bof;               /* Determine logical beginning of file. */
   DBENTRYP_BP   eof;               /* Determine logical end of file. */
   DBENTRYP_BP   found;             /* Determine outcome of the last search operation. */
   DBENTRYP_V    goBottom;          /* Position cursor at the last record. */
   DBENTRYP_UL   go;                /* Position cursor at a specific physical record. */
   DBENTRYP_I    goToId;            /* Position the cursor to a specific, physical identity. */
   DBENTRYP_V    goTop;             /* Position cursor at the first record. */
   DBENTRYP_BIB  seek;              /*  */
   DBENTRYP_L    skip;              /* Reposition cursor relative to current position. */
   DBENTRYP_L    skipFilter;        /*-Reposition cursor respecting any filter setting. */
   DBENTRYP_L    skipRaw;           /* Reposition cursor, regardless of filter. */


   /* Data management */

   DBENTRYP_VF   addField;          /* Add a field to the WorkArea. */
   DBENTRYP_B    append;            /* Append a record to the WorkArea. */
   DBENTRYP_I    createFields;      /*-Add all fields defined in an array to the WorkArea. */
   DBENTRYP_V    deleterec;         /* Delete a record. */
   DBENTRYP_BP   deleted;           /* Determine deleted status for a record. */
   DBENTRYP_SP   fieldCount;        /*-Determine the number of fields in the WorkArea. */
   DBENTRYP_VF   fieldDisplay;      /*  */
   DBENTRYP_SSI  fieldInfo;         /*-Retrieve information about a field. */
   DBENTRYP_SVP  fieldName;         /*-Determine the name associated with a field number. */
   DBENTRYP_V    flush;             /* Write data buffer to the data store. */
   DBENTRYP_PP   getRec;            /*  */
   DBENTRYP_SI   getValue;          /* Obtain the current value of a field. */
   DBENTRYP_SVL  getVarLen;         /* Obtain the length of a field value. */
   DBENTRYP_V    goCold;            /* Perform a write of WorkArea memory to the data store. */
   DBENTRYP_V    goHot;             /* Mark the WorkArea data buffer as hot. */
   DBENTRYP_P    putRec;            /* Replace the current record. */
   DBENTRYP_SI   putValue;          /* Assign a value to a field. */
   DBENTRYP_V    recall;            /* Undelete the current record. */
   DBENTRYP_ULP  reccount;          /* Obtain number of records in WorkArea. */
   DBENTRYP_ISI  recInfo;           /*  */
   DBENTRYP_ULP  recno;             /* Obtain physical row number at current WorkArea cursor position. */
   DBENTRYP_I    recid;             /* Obtain physical row ID at current WorkArea cursor position. */
   DBENTRYP_S    setFieldExtent;    /* Establish the extent of the array of fields for a WorkArea. */


   /* WorkArea/Database management */

   DBENTRYP_P    alias;             /*-Obtain the alias of the WorkArea. */
   DBENTRYP_V    close;             /* Close the table in the WorkArea. */
   DBENTRYP_VP   create;            /* Create a data store in the specified WorkArea. */
   DBENTRYP_SI   info;              /* Retrieve information about the current driver (DBI). */
   DBENTRYP_V    newarea;           /* Clear the WorkArea for use. */
   DBENTRYP_VP   open;              /* Open a data store in the WorkArea. */
   DBENTRYP_V    release;           /*-Release all references to a WorkArea. */
   DBENTRYP_SP   structSize;        /* Retrieve the size of the WorkArea structure. */
   DBENTRYP_P    sysName;           /* Obtain the name of replaceable database driver (RDD) subsystem. */
   DBENTRYP_VEI  dbEval;            /*-Evaluate code block for each record in WorkArea. */
   DBENTRYP_V    pack;              /* Remove records marked for deletion from a database. */
   DBENTRYP_LSP  packRec;           /*  */
   DBENTRYP_VS   sort;              /* Physically reorder a database. */
   DBENTRYP_VT   trans;             /* Copy one or more records from one WorkArea to another. */
   DBENTRYP_VT   transRec;          /* Copy a record to another WorkArea. */
   DBENTRYP_V    zap;               /* Physically remove all records from data store. */


   /* Relational Methods */

   DBENTRYP_VR   childEnd;          /* Report end of relation. */
   DBENTRYP_VR   childStart;        /* Report initialization of a relation. */
   DBENTRYP_VR   childSync;         /* Post a pending relational movement. */
   DBENTRYP_V    syncChildren;      /*-Force relational movement in child WorkAreas. */
   DBENTRYP_V    clearRel;          /* Clear all relations in the specified WorkArea. */
   DBENTRYP_V    forceRel;          /* Force relational seeks in the specified WorkArea. */
   DBENTRYP_SVP  relArea;           /*-Obtain the workarea number of the specified relation. */
   DBENTRYP_VR   relEval;           /*-Evaluate a block against the relation in specified WorkArea. */
   DBENTRYP_SI   relText;           /*-Obtain the character expression of the specified relation. */
   DBENTRYP_VR   setRel;            /*-Set a relation in the parent file. */


   /* Order Management */

   DBENTRYP_OI   orderListAdd;      /*  */
   DBENTRYP_V    orderListClear;    /*  */
   DBENTRYP_OI   orderListDelete;   /*  */
   DBENTRYP_OI   orderListFocus;    /*  */
   DBENTRYP_V    orderListRebuild;  /*  */
   DBENTRYP_VOI  orderCondition;    /*  */
   DBENTRYP_VOC  orderCreate;       /*  */
   DBENTRYP_OI   orderDestroy;      /*  */
   DBENTRYP_OII  orderInfo;         /*-Retrieve information about the current order that SELF could not. */


   /* Filters and Scope Settings */

   DBENTRYP_V    clearFilter;       /*-Clear the active filter expression. */
   DBENTRYP_V    clearLocate;       /*-Clear the active locate expression. */
   DBENTRYP_V    clearScope;        /*  */
   DBENTRYP_VPLP countScope;        /*  */
   DBENTRYP_I    filterText;        /*-Return filter condition of the specified WorkArea. */
   DBENTRYP_SI   scopeInfo;         /*  */
   DBENTRYP_VFI  setFilter;         /* Set the filter condition for the specified WorkArea. */
   DBENTRYP_VLO  setLocate;         /*-Set the locate scope for the specified WorkArea. */
   DBENTRYP_VOS  setScope;          /*  */
   DBENTRYP_VPL  skipScope;         /*  */
   DBENTRYP_B    locate;            /* reposition cursor to postions set by setLocate */


   /* Miscellaneous */

   DBENTRYP_P    compile;           /*-Compile a character expression. */
   DBENTRYP_I    error;             /*-Raise a runtime error. */
   DBENTRYP_I    evalBlock;         /*-Evaluate a code block. */


   /* Network operations */

   DBENTRYP_VSP  rawlock;           /* Perform a lowlevel network lock in the specified WorkArea. */
   DBENTRYP_VL   lock;              /* Perform a network lock in the specified WorkArea. */
   DBENTRYP_I    unlock;            /* Release network locks in the specified WorkArea. */


   /* Memofile functions */

   DBENTRYP_V    closeMemFile;      /* Close a memo file in the WorkArea. */
   DBENTRYP_VP   createMemFile;     /* Create a memo file in the WorkArea. */
   DBENTRYP_SVPB getValueFile;      /*  */
   DBENTRYP_VP   openMemFile;       /* Open a memo file in the specified WorkArea. */
   DBENTRYP_SVPB putValueFile;      /*  */


   /* Database file header handling */

   DBENTRYP_V    readDBHeader;      /* Read the database file header record in the WorkArea. */
   DBENTRYP_V    writeDBHeader;     /* Write the database file header record in the WorkArea. */


   /* non WorkArea functions       */

   DBENTRYP_R    init;              /* init RDD after registration */
   DBENTRYP_R    exit;              /* unregister RDD */
   DBENTRYP_RVVL drop;              /* remove table */
   DBENTRYP_RVVL exists;            /* check if table exist */
   DBENTRYP_RSLV rddInfo;           /* RDD info */


   /* Special and reserved methods */

   DBENTRYP_SVP  whoCares;          /*  */

} RDDFUNCS;

typedef RDDFUNCS * PRDDFUNCS;

#define RDDFUNCSCOUNT   ( sizeof( RDDFUNCS ) / sizeof( DBENTRYP_V ) )

/* RDD Node structure              */
typedef struct _RDDNODE
{
   char szName[ HARBOUR_MAX_RDD_DRIVERNAME_LENGTH + 1 ]; /* Name of RDD */
   USHORT   uiType;           /* Type of RDD */
   USHORT   rddID;            /* Type of RDD */
   RDDFUNCS pTable;           /* Table of functions */
   RDDFUNCS pSuperTable;      /* Table of super functions */
   USHORT   uiAreaSize;       /* Size of the WorkArea */
   void     *lpvCargo;        /* RDD specific extended data, if used then
                                 RDD should free it in EXIT() non WA method */
} RDDNODE;

typedef RDDNODE * LPRDDNODE;


/*--------------------* SELF Methods *------------------------*/

/* Movement and positioning methods */

#define SELF_BOF(w, sp)                 ((*(w)->lprfsHost->bof)(w, sp))
#define SELF_EOF(w, sp)                 ((*(w)->lprfsHost->eof)(w, sp))
#define SELF_FOUND(w, sp)               ((*(w)->lprfsHost->found)(w, sp))
#define SELF_GOTO(w, l)                 ((*(w)->lprfsHost->go)(w, l))
#define SELF_GOTOID(w, sp)              ((*(w)->lprfsHost->goToId)(w, sp))
#define SELF_GOBOTTOM(w)                ((*(w)->lprfsHost->goBottom)(w))
#define SELF_GOTOP(w)                   ((*(w)->lprfsHost->goTop)(w))
#define SELF_SEEK(w, i1, v, i2)         ((*(w)->lprfsHost->seek)(w, i1, v, i2))
#define SELF_SKIP(w, l)                 ((*(w)->lprfsHost->skip)(w, l))
#define SELF_SKIPFILTER(w, l)           ((*(w)->lprfsHost->skipFilter)(w, l))
#define SELF_SKIPRAW(w, l)              ((*(w)->lprfsHost->skipRaw)(w, l))


/* Data management */

#define SELF_ADDFIELD(w, ip)            ((*(w)->lprfsHost->addField)(w, ip))
#define SELF_APPEND(w, b)               ((*(w)->lprfsHost->append)(w, b))
#define SELF_CREATEFIELDS(w, v)         ((*(w)->lprfsHost->createFields)(w, v))
#define SELF_DELETE(w)                  ((*(w)->lprfsHost->deleterec)(w))
#define SELF_DELETED(w, sp)             ((*(w)->lprfsHost->deleted)(w, sp))
#define SELF_FIELDCOUNT(w, sp)          ((*(w)->lprfsHost->fieldCount)(w, sp))
#define SELF_FIELDDISPLAY(w, sp)        ((*(w)->lprfsHost->fieldDisplay)(w, sp))
#define SELF_FIELDINFO(w,s1,s2,v)       ((*(w)->lprfsHost->fieldInfo)(w,s1,s2,v))
#define SELF_FIELDNAME(w, i, bp)        ((*(w)->lprfsHost->fieldName)(w, i, bp))
#define SELF_FLUSH(w)                   ((*(w)->lprfsHost->flush)(w))
#define SELF_GETREC(w, bpp)             ((*(w)->lprfsHost->getRec)(w, bpp))
#define SELF_GETVALUE(w, i, v)          ((*(w)->lprfsHost->getValue)(w, i, v))
#define SELF_GETVARLEN(w, i, lp)        ((*(w)->lprfsHost->getVarLen)(w, i, lp))
#define SELF_GOCOLD(w)                  ((*(w)->lprfsHost->goCold)(w))
#define SELF_GOHOT(w)                   ((*(w)->lprfsHost->goHot)(w))
#define SELF_PUTVALUE(w, i, v)          ((*(w)->lprfsHost->putValue)(w, i, v))
#define SELF_PUTREC(w, bp)              ((*(w)->lprfsHost->putRec)(w, bp))
#define SELF_RECALL(w)                  ((*(w)->lprfsHost->recall)(w))
#define SELF_RECCOUNT(w, lp)            ((*(w)->lprfsHost->reccount)(w, lp))
#define SELF_RECINFO(w,v1,i,v2)         ((*(w)->lprfsHost->recInfo)(w,v1,i,v2))
#define SELF_RECNO(w, lp)               ((*(w)->lprfsHost->recno)(w, lp))
#define SELF_RECID(w, i)                ((*(w)->lprfsHost->recid)(w, i))
#define SELF_SETFIELDEXTENT(w, s)       ((*(w)->lprfsHost->setFieldExtent)(w, s))


/* WorkArea/Database management */

#define SELF_ALIAS(w, bp)               ((*(w)->lprfsHost->alias)(w, bp))
#define SELF_CLOSE(w)                   ((*(w)->lprfsHost->close)(w))
#define SELF_CREATE(w, ip)              ((*(w)->lprfsHost->create)(w, ip))
#define SELF_INFO(w, i, g)              ((*(w)->lprfsHost->info)(w, i, g))
#define SELF_NEW(w)                     ((*(w)->lprfsHost->newarea)(w))
#define SELF_OPEN(w, ip)                ((*(w)->lprfsHost->open)(w, ip))
#define SELF_RELEASE(w)                 ((*(w)->lprfsHost->release)(w))
#define SELF_STRUCTSIZE(w, sp)          ((*(w)->lprfsHost->structSize)(w,sp))
#define SELF_SYSNAME(w, bp)             ((*(w)->lprfsHost->sysName)(w, bp))
#define SELF_DBEVAL(w, ip)              ((*(w)->lprfsHost->dbEval)(w, ip))
#define SELF_PACK(w)                    ((*(w)->lprfsHost->pack)(w))
#define SELF_PACKREC(w, l, sp)          ((*(w)->lprfsHost->packRec)(w, l, sp))
#define SELF_SORT(w, ip)                ((*(w)->lprfsHost->sort)(w, ip))
#define SELF_TRANS(w, ip)               ((*(w)->lprfsHost->trans)(w, ip))
#define SELF_TRANSREC(w, ip)            ((*(w)->lprfsHost->transRec)(w, ip))
#define SELF_ZAP(w)                     ((*(w)->lprfsHost->zap)(w))


/* Relational Methods */

#define SELF_CHILDEND(w, ip)            ((*(w)->lprfsHost->childEnd)(w, ip))
#define SELF_CHILDSTART(w, ip)          ((*(w)->lprfsHost->childStart)(w, ip))
#define SELF_CHILDSYNC(w, ip)           ((*(w)->lprfsHost->childSync)(w, ip))
#define SELF_SYNCCHILDREN(w)            ((*(w)->lprfsHost->syncChildren)(w))
#define SELF_CLEARREL(w)                ((*(w)->lprfsHost->clearRel)(w))
#define SELF_FORCEREL(w)                ((*(w)->lprfsHost->forceRel)(w))
#define SELF_RELAREA(w, s, sp)          ((*(w)->lprfsHost->relArea)(w, s, sp))
#define SELF_RELEVAL(w, ip)             ((*(w)->lprfsHost->relEval)(w, ip))
#define SELF_RELTEXT(w, s, bp)          ((*(w)->lprfsHost->relText)(w, s, bp))
#define SELF_SETREL(w, ip)              ((*(w)->lprfsHost->setRel)(w, ip))


/* Order Management */

#define SELF_ORDLSTADD(w, lp)           ((*(w)->lprfsHost->orderListAdd)(w, lp))
#define SELF_ORDLSTDELETE(w, lp)        ((*(w)->lprfsHost->orderListDelete)(w, lp))
#define SELF_ORDLSTFOCUS(w, lp)         ((*(w)->lprfsHost->orderListFocus)(w,lp))
#define SELF_ORDLSTREBUILD(w)           ((*(w)->lprfsHost->orderListRebuild)(w))
#define SELF_ORDLSTCLEAR(w)             ((*(w)->lprfsHost->orderListClear)(w))
#define SELF_ORDSETCOND(w, ip)          ((*(w)->lprfsHost->orderCondition)(w, ip))
#define SELF_ORDCREATE(w, ip)           ((*(w)->lprfsHost->orderCreate)(w, ip))
#define SELF_ORDDESTROY(w, p)           ((*(w)->lprfsHost->orderDestroy)(w, p))
#define SELF_ORDINFO(w, i, p)           ((*(w)->lprfsHost->orderInfo)(w, i, p))
#define SELF_ORDEXPR(w, p)              ((*(w)->lprfsHost->orderInfo)(w, DBOI_EXPRESSION, p))
#define SELF_ORDCOND(w, p)              ((*(w)->lprfsHost->orderInfo)(w, DBOI_CONDITION,  p))
#define SELF_ORDRECNO(w, p)             ((*(w)->lprfsHost->orderInfo)(w, DBOI_RECNO,      p))
#define SELF_ORDPOS(w, p)               ((*(w)->lprfsHost->orderInfo)(w, DBOI_POSITION,   p))
#define SELF_ORDNUMBER(w, p)            ((*(w)->lprfsHost->orderInfo)(w, DBOI_NUMBER,     p))
#define SELF_ORDNAME(w, p)              ((*(w)->lprfsHost->orderInfo)(w, DBOI_NAME,       p))
#define SELF_ORDBAGNAME(w, p)           ((*(w)->lprfsHost->orderInfo)(w, DBOI_BAGNAME,    p))
#define SELF_ORDBAGEXT(w,  p)           ((*(w)->lprfsHost->orderInfo)(w, DBOI_BAGEXT,     p))


/* Filters and Scope Settings */

#define SELF_CLEARFILTER(w)             ((*(w)->lprfsHost->clearFilter)(w))
#define SELF_CLEARLOCATE(w)             ((*(w)->lprfsHost->clearLocate)(w))
#define SELF_CLEARSCOPE(w)              ((*(w)->lprfsHost->clearScope)(w))
#define SELF_COUNTSCOPE(w,ip,lp)        ((*(w)->lprfsHost->countScope)(w,ip,lp))
#define SELF_FILTERTEXT(w, bp)          ((*(w)->lprfsHost->filterText)(w, bp))
#define SELF_SCOPEINFO(w,i,v)           ((*(w)->lprfsHost->scopeInfo)(w,i,v))
#define SELF_SETFILTER(w, ip)           ((*(w)->lprfsHost->setFilter)(w, ip))
#define SELF_SETLOCATE(w, ip)           ((*(w)->lprfsHost->setLocate)(w, ip))
#define SELF_SETSCOPE(w, ip)            ((*(w)->lprfsHost->setScope)(w, ip))
#define SELF_SKIPSCOPE(w, bp, l)        ((*(w)->lprfsHost->skipScope)(w, bp, l))
#define SELF_LOCATE(w, b)               ((*(w)->lprfsHost->locate)(w, b))


/* Miscellaneous */

#define SELF_COMPILE(w, bp)             ((*(w)->lprfsHost->compile)(w, bp))
#define SELF_ERROR(w, ip)               ((*(w)->lprfsHost->error)(w, ip))
#define SELF_EVALBLOCK(w, v)            ((*(w)->lprfsHost->evalBlock)(w, v))


/* Network operations */

#define SELF_GETLOCKS(w, g)             ((*(w)->lprfsHost->info)(w, DBI_GETLOCKARRAY, g))
#define SELF_RAWLOCK(w, i, l)           ((*(w)->lprfsHost->rawlock)(w, i, l))
#define SELF_LOCK(w, sp)                ((*(w)->lprfsHost->lock)(w, sp))
#define SELF_UNLOCK(w, i)               ((*(w)->lprfsHost->unlock)(w, i))


/* Memofile functions */

#define SELF_CLOSEMEMFILE(w)            ((*(w)->lprfsHost->closeMemFile)(w))
#define SELF_CREATEMEMFILE(w,bp)        ((*(w)->lprfsHost->createMemFile)(w,bp))
#define SELF_GETVALUEFILE(w,i,bp,u)     ((*(w)->lprfsHost->getValueFile)(w,i,bp,u))
#define SELF_OPENMEMFILE(w,bp)          ((*(w)->lprfsHost->openMemFile)(w,bp))
#define SELF_PUTVALUEFILE(w,i,bp,u)     ((*(w)->lprfsHost->putValueFile)(w,i,bp,u))


/* Database file header handling */

#define SELF_READDBHEADER(w)            ((*(w)->lprfsHost->readDBHeader)(w))
#define SELF_WRITEDBHEADER(w)           ((*(w)->lprfsHost->writeDBHeader)(w))


/* Info operations */

#define SELF_RECSIZE(w, lp)             ((*(w)->lprfsHost->info)(w, DBI_GETRECSIZE, lp))
#define SELF_HEADERSIZE(w, fp)          ((*(w)->lprfsHost->info)(w, DBI_GETHEADERSIZE, fp))
#define SELF_LUPDATE(w, fp)             ((*(w)->lprfsHost->info)(w, DBI_LASTUPDATE, fp ))
#define SELF_SETDELIM(w, fp)            ((*(w)->lprfsHost->info)(w, DBI_SETDELIMITER, fp))
#define SELF_GETDELIM(w, fp)            ((*(w)->lprfsHost->info)(w, DBI_GETDELIMITER, fp))
#define SELF_TABLEEXT(w, fp)            ((*(w)->lprfsHost->info)(w, DBI_TABLEEXT, fp))

#define SELF_RDDNODE(w)                 hb_rddGetNode((w)->rddID)

/* non WorkArea functions */
#define SELF_INIT(r)                    ((*(r)->pTable.init)(r))
#define SELF_EXIT(r)                    ((*(r)->pTable.exit)(r))
#define SELF_DROP(r, it, ii, l)         ((*(r)->pTable.drop)(r, it, ii, l))
#define SELF_EXISTS(r, it, ii, l)       ((*(r)->pTable.exists)(r, it, ii, l))
#define SELF_RDDINFO(r, i, l, g)        ((*(r)->pTable.rddInfo)(r, i, l, g))


/*--------------------* SUPER Methods *------------------------*/

#ifndef _SUPERTABLE
#define _SUPERTABLE(w)                  SUPERTABLE
#endif
#ifndef __SUPERTABLE
#define __SUPERTABLE(w)                 SUPERTABLE
#endif

/* Movement and positioning methods */

#define SUPER_BOF(w, sp)                ((*(_SUPERTABLE(w))->bof)(w, sp))
#define SUPER_EOF(w, sp)                ((*(_SUPERTABLE(w))->eof)(w, sp))
#define SUPER_FOUND(w, sp)              ((*(_SUPERTABLE(w))->found)(w, sp))
#define SUPER_GOTO(w, l)                ((*(_SUPERTABLE(w))->go)(w, l))
#define SUPER_GOTOID(w, sp)             ((*(_SUPERTABLE(w))->goToId)(w, sp))
#define SUPER_GOBOTTOM(w)               ((*(_SUPERTABLE(w))->goBottom)(w))
#define SUPER_GOTOP(w)                  ((*(_SUPERTABLE(w))->goTop)(w))
#define SUPER_SEEK(w, i1, v, i2)        ((*(_SUPERTABLE(w))->seek)(w, i1, v, i2))
#define SUPER_SKIP(w, l)                ((*(_SUPERTABLE(w))->skip)(w, l))
#define SUPER_SKIPFILTER(w, l)          ((*(_SUPERTABLE(w))->skipFilter)(w, l))
#define SUPER_SKIPRAW(w, l)             ((*(_SUPERTABLE(w))->skipRaw)(w, l))


/* Data management */

#define SUPER_ADDFIELD(w, ip)           ((*(_SUPERTABLE(w))->addField)(w, ip))
#define SUPER_APPEND(w, b)              ((*(_SUPERTABLE(w))->append)(w, b))
#define SUPER_CREATEFIELDS(w, v)        ((*(_SUPERTABLE(w))->createFields)(w, v))
#define SUPER_DELETE(w)                 ((*(_SUPERTABLE(w))->deleterec)(w))
#define SUPER_DELETED(w, sp)            ((*(_SUPERTABLE(w))->deleted)(w, sp))
#define SUPER_FIELDCOUNT(w, sp)         ((*(_SUPERTABLE(w))->fieldCount)(w, sp))
#define SUPER_FIELDDISPLAY(w, sp)       ((*(_SUPERTABLE(w))->fieldDisplay)(w, sp))
#define SUPER_FIELDINFO(w,s1,s2,v)      ((*(_SUPERTABLE(w))->fieldInfo)(w,s1,s2,v))
#define SUPER_FIELDNAME(w, i, bp)       ((*(_SUPERTABLE(w))->fieldName)(w, i, bp))
#define SUPER_FLUSH(w)                  ((*(_SUPERTABLE(w))->flush)(w))
#define SUPER_GETREC(w, bpp)            ((*(_SUPERTABLE(w))->getRec)(w, bpp))
#define SUPER_GETVALUE(w, i, v)         ((*(_SUPERTABLE(w))->getValue)(w, i, v))
#define SUPER_GETVARLEN(w, i, lp)       ((*(_SUPERTABLE(w))->getVarLen)(w, i, lp))
#define SUPER_GOCOLD(w)                 ((*(_SUPERTABLE(w))->goCold)(w))
#define SUPER_GOHOT(w)                  ((*(_SUPERTABLE(w))->goHot)(w))
#define SUPER_PUTVALUE(w, i, v)         ((*(_SUPERTABLE(w))->putValue)(w, i, v))
#define SUPER_PUTREC(w, bp)             ((*(_SUPERTABLE(w))->putRec)(w, bp))
#define SUPER_RECALL(w)                 ((*(_SUPERTABLE(w))->recall)(w))
#define SUPER_RECCOUNT(w, lp)           ((*(_SUPERTABLE(w))->reccount)(w, lp))
#define SUPER_RECINFO(w,v1,i,v2)        ((*(_SUPERTABLE(w))->recInfo)(w,v1,i,v2))
#define SUPER_RECNO(w, lp)              ((*(_SUPERTABLE(w))->recno)(w, lp))
#define SUPER_RECID(w, i)               ((*(_SUPERTABLE(w))->recid)(w, i))
#define SUPER_SETFIELDEXTENT(w, s)      ((*(_SUPERTABLE(w))->setFieldExtent)(w, s))


/* WorkArea/Database management */

#define SUPER_ALIAS(w, bp)              ((*(_SUPERTABLE(w))->alias)(w, bp))
#define SUPER_CLOSE(w)                  ((*(_SUPERTABLE(w))->close)(w))
#define SUPER_CREATE(w, ip)             ((*(_SUPERTABLE(w))->create)(w, ip))
#define SUPER_INFO(w, i, g)             ((*(_SUPERTABLE(w))->info)(w, i, g))
#define SUPER_NEW(w)                    ((*(_SUPERTABLE(w))->newarea)(w))
#define SUPER_OPEN(w, ip)               ((*(_SUPERTABLE(w))->open)(w, ip))
#define SUPER_RELEASE(w)                ((*(_SUPERTABLE(w))->release)(w))
#define SUPER_STRUCTSIZE(w, sp)         ((*(_SUPERTABLE(w))->structSize)(w, sp))
#define SUPER_SYSNAME(w, bp)            ((*(_SUPERTABLE(w))->sysName)(w, bp))
#define SUPER_DBEVAL(w, ip)             ((*(_SUPERTABLE(w))->dbEval)(w, ip))
#define SUPER_PACK(w)                   ((*(_SUPERTABLE(w))->pack)(w))
#define SUPER_PACKREC(w, l, sp)         ((*(_SUPERTABLE(w))->packRec)(w, l, sp))
#define SUPER_SORT(w, ip)               ((*(_SUPERTABLE(w))->sort)(w, ip))
#define SUPER_TRANS(w, ip)              ((*(_SUPERTABLE(w))->trans)(w, ip))
#define SUPER_TRANSREC(w, ip)           ((*(_SUPERTABLE(w))->transRec)(w, ip))
#define SUPER_ZAP(w)                    ((*(_SUPERTABLE(w))->zap)(w))


/* Relational Methods */

#define SUPER_CHILDEND(w, ip)           ((*(_SUPERTABLE(w))->childEnd)(w, ip))
#define SUPER_CHILDSTART(w, ip)         ((*(_SUPERTABLE(w))->childStart)(w, ip))
#define SUPER_CHILDSYNC(w, ip)          ((*(_SUPERTABLE(w))->childSync)(w, ip))
#define SUPER_SYNCCHILDREN(w)           ((*(_SUPERTABLE(w))->syncChildren)(w))
#define SUPER_CLEARREL(w)               ((*(_SUPERTABLE(w))->clearRel)(w))
#define SUPER_FORCEREL(w)               ((*(_SUPERTABLE(w))->forceRel)(w))
#define SUPER_RELAREA(w, s, sp)         ((*(_SUPERTABLE(w))->relArea)(w, s, sp))
#define SUPER_RELEVAL(w, ip)            ((*(_SUPERTABLE(w))->relEval)(w, ip))
#define SUPER_RELTEXT(w, s, bp)         ((*(_SUPERTABLE(w))->relText)(w, s, bp))
#define SUPER_SETREL(w, ip)             ((*(_SUPERTABLE(w))->setRel)(w, ip))


/* Order Management */

#define SUPER_ORDLSTADD(w, lp)          ((*(_SUPERTABLE(w))->orderListAdd)(w, lp))
#define SUPER_ORDLSTDELETE(w, lp)       ((*(_SUPERTABLE(w))->orderListDelete)(w, lp))
#define SUPER_ORDLSTFOCUS(w, lp)        ((*(_SUPERTABLE(w))->orderListFocus)(w, lp))
#define SUPER_ORDLSTREBUILD(w)          ((*(_SUPERTABLE(w))->orderListRebuild)(w))
#define SUPER_ORDLSTCLEAR(w)            ((*(_SUPERTABLE(w))->orderListClear)(w))
#define SUPER_ORDSETCOND(w,ip)          ((*(_SUPERTABLE(w))->orderCondition)(w, ip))
#define SUPER_ORDCREATE(w, ip)          ((*(_SUPERTABLE(w))->orderCreate)(w, ip))
#define SUPER_ORDDESTROY(w, ip)         ((*(_SUPERTABLE(w))->orderDestroy)(w, ip))
#define SUPER_ORDDELETE(w, ip)          ((*(_SUPERTABLE(w))->orderDelete)(w, ip))
#define SUPER_ORDINFO(w, i, p)          ((*(_SUPERTABLE(w))->orderInfo)(w, i, p))
#define SUPER_ORDEXPR(w, p)             ((*(_SUPERTABLE(w))->orderInfo)(w, DBOI_EXPRESSION, p))
#define SUPER_ORDCOND(w, p)             ((*(_SUPERTABLE(w))->orderInfo)(w, DBOI_CONDITION,  p))
#define SUPER_ORDRECNO(w, p)            ((*(_SUPERTABLE(w))->orderInfo)(w, DBOI_RECNO,      p))
#define SUPER_ORDPOS(w, p)              ((*(_SUPERTABLE(w))->orderInfo)(w, DBOI_POSITION,   p))
#define SUPER_ORDNUMBER(w, p)           ((*(_SUPERTABLE(w))->orderInfo)(w, DBOI_NUMBER,     p))
#define SUPER_ORDNAME(w, p)             ((*(_SUPERTABLE(w))->orderInfo)(w, DBOI_NAME,       p))
#define SUPER_ORDBAGNAME(w, p)          ((*(_SUPERTABLE(w))->orderInfo)(w, DBOI_BAGNAME,    p))
#define SUPER_ORDBAGEXT(w,  p)          ((*(_SUPERTABLE(w))->orderInfo)(w, DBOI_BAGEXT,     p))


/* Filters and Scope Settings */

#define SUPER_CLEARFILTER(w)            ((*(_SUPERTABLE(w))->clearFilter)(w))
#define SUPER_CLEARLOCATE(w)            ((*(_SUPERTABLE(w))->clearLocate)(w))
#define SUPER_CLEARSCOPE(w)             ((*(_SUPERTABLE(w))->clearScope)(w))
#define SUPER_COUNTSCOPE(w,ip,lp)       ((*(_SUPERTABLE(w))->countScope)(w,ip,lp))
#define SUPER_FILTERTEXT(w, bp)         ((*(_SUPERTABLE(w))->filterText)(w, bp))
#define SUPER_SCOPEINFO(w,i,v)          ((*(_SUPERTABLE(w))->scopeInfo)(w,i,v))
#define SUPER_SETFILTER(w, ip)          ((*(_SUPERTABLE(w))->setFilter)(w, ip))
#define SUPER_SETLOCATE(w, ip)          ((*(_SUPERTABLE(w))->setLocate)(w, ip))
#define SUPER_SETSCOPE(w, ip)           ((*(_SUPERTABLE(w))->setScope)(w, ip))
#define SUPER_SKIPSCOPE(w, bp, l)       ((*(_SUPERTABLE(w))->skipScope)(w, bp, l))
#define SUPER_LOCATE(w, b)              ((*(_SUPERTABLE(w))->locate)(w, b))


/* Miscellaneous */

#define SUPER_COMPILE(w, bp)            ((*(_SUPERTABLE(w))->compile)(w, bp))
#define SUPER_ERROR(w, ip)              ((*(_SUPERTABLE(w))->error)(w, ip))
#define SUPER_EVALBLOCK(w, v)           ((*(_SUPERTABLE(w))->evalBlock)(w, v))


/* Network operations */

#define SUPER_GETLOCKS(w, g)            ((*(_SUPERTABLE(w))->info)(w, DBI_GETLOCKARRAY, g))
#define SUPER_RAWLOCK(w, i, l)          ((*(_SUPERTABLE(w))->rawlock)(w, i, l))
#define SUPER_LOCK(w, sp)               ((*(_SUPERTABLE(w))->lock)(w, sp))
#define SUPER_UNLOCK(w, i)              ((*(_SUPERTABLE(w))->unlock)(w, i))


/* Memofile functions */

#define SUPER_CLOSEMEMFILE(w)           ((*(_SUPERTABLE(w))->closeMemFile)(w))
#define SUPER_CREATEMEMFILE(w,bp)       ((*(_SUPERTABLE(w))->createMemFile)(w,bp))
#define SUPER_GETVALUEFILE(w,i,bp,u)    ((*(_SUPERTABLE(w))->getValueFile)(w,i,bp,u))
#define SUPER_OPENMEMFILE(w,bp)         ((*(_SUPERTABLE(w))->openMemFile)(w,bp))
#define SUPER_PUTVALUEFILE(w,i,bp,u)    ((*(_SUPERTABLE(w))->putValueFile)(w,i,bp,u))


/* Database file header handling */

#define SUPER_READDBHEADER(w)           ((*(_SUPERTABLE(w))->readDBHeader)(w))
#define SUPER_WRITEDBHEADER(w)          ((*(_SUPERTABLE(w))->writeDBHeader)(w))


/* Info operations */

#define SUPER_RECSIZE(w, lp)            ((*(_SUPERTABLE(w))->info)(w, DBI_GETRECSIZE, lp))
#define SUPER_HEADERSIZE(w, fp)         ((*(_SUPERTABLE(w))->info)(w, DBI_GETHEADERSIZE, fp))
#define SUPER_LUPDATE(w, fp)            ((*(_SUPERTABLE(w))->info)(w, DBI_LASTUPDATE, fp ))
#define SUPER_SETDELIM(w, fp)           ((*(_SUPERTABLE(w))->info)(w, DBI_SETDELIMITER, fp))
#define SUPER_GETDELIM(w, fp)           ((*(_SUPERTABLE(w))->info)(w, DBI_GETDELIMITER, fp))
#define SUPER_TABLEEXT(w, fp)           ((*(_SUPERTABLE(w))->info)(w, DBI_TABLEEXT, fp))

/* non WorkArea functions */
#define SUPER_INIT(r)                   ((*(__SUPERTABLE(r))->init)(r))
#define SUPER_EXIT(r)                   ((*(__SUPERTABLE(r))->exit)(r))
#define SUPER_DROP(r, it, ii, l)        ((*(__SUPERTABLE(r))->drop)(r, it, ii, l))
#define SUPER_EXISTS(r, it, ii, l)      ((*(__SUPERTABLE(r))->exists)(r, it, ii, l))
#define SUPER_RDDINFO(r, i, l, g)       ((*(__SUPERTABLE(r))->rddInfo)(r, i, l, g))

#define ISSUPER_INIT(r)                 ((__SUPERTABLE(r))->init != NULL)
#define ISSUPER_EXIT(r)                 ((__SUPERTABLE(r))->exit != NULL)

/*
 *  PROTOTYPES
 *  ----------
 */
/* RDD virtual machine integration functions */

extern HB_EXPORT void    hb_rddShutDown( void );
extern HB_EXPORT ERRCODE hb_rddGetFieldValue( HB_ITEM_PTR pItem, PHB_SYMB pFieldSymbol );
extern HB_EXPORT ERRCODE hb_rddPutFieldValue( HB_ITEM_PTR pItem, PHB_SYMB pFieldSymbol );
extern HB_EXPORT ERRCODE hb_rddFieldGet( HB_ITEM_PTR pItem, PHB_SYMB pFieldSymbol );
extern HB_EXPORT ERRCODE hb_rddFieldPut( HB_ITEM_PTR pItem, PHB_SYMB pFieldSymbol );
extern HB_EXPORT int     hb_rddGetCurrentWorkAreaNumber( void );
extern HB_EXPORT ERRCODE hb_rddSelectWorkAreaNumber( int iArea );
extern HB_EXPORT ERRCODE hb_rddSelectWorkAreaAlias( const char * szAlias );
extern HB_EXPORT ERRCODE hb_rddSelectWorkAreaSymbol( PHB_SYMB pSymAlias );
extern HB_EXPORT ERRCODE hb_rddGetAliasNumber( const char * szAlias, int * iArea );

/* other functions */
extern HB_EXPORT void *  hb_rddAllocWorkAreaAlias( const char * szAlias, int iArea );
extern HB_EXPORT void *  hb_rddGetCurrentWorkAreaPointer( void );
extern HB_EXPORT void *  hb_rddGetWorkAreaPointer( int iArea );
extern HB_EXPORT USHORT  hb_rddInsertAreaNode( const char *szDriver );
extern HB_EXPORT void    hb_rddReleaseCurrentArea( void );

extern HB_EXPORT int       hb_rddRegister( const char * szDriver, USHORT uiType );
extern HB_EXPORT ERRCODE   hb_rddInherit( RDDFUNCS * pTable, const RDDFUNCS * pSubTable, RDDFUNCS * pSuperTable, const char * szDrvName );
extern HB_EXPORT LPRDDNODE hb_rddGetNode( USHORT uiNode );
extern HB_EXPORT LPRDDNODE hb_rddFindNode( const char * szDriver, USHORT * uiIndex );
extern HB_EXPORT USHORT    hb_rddFieldIndex( AREAP pArea, const char * szName );
extern HB_EXPORT USHORT    hb_rddFieldExpIndex( AREAP pArea, const char * szField );
extern HB_EXPORT const char * hb_rddDefaultDrv( const char * szDriver );
extern HB_EXPORT ERRCODE   hb_rddSelectFirstAvailable( void );
extern HB_EXPORT ERRCODE   hb_rddVerifyAliasName( const char * szAlias );
extern HB_EXPORT void *    hb_rddNewAreaNode( LPRDDNODE pRddNode, USHORT uiRddID );
extern HB_EXPORT PHB_ITEM  hb_rddList( USHORT uiType );
extern HB_EXPORT void      hb_rddCloseAll( void );
extern HB_EXPORT void      hb_rddFlushAll( void );
extern HB_EXPORT void      hb_rddUnLockAll( void );
extern HB_EXPORT BOOL      hb_rddGetNetErr( void );
extern HB_EXPORT void      hb_rddSetNetErr( BOOL fNetErr );

extern HB_EXPORT ERRCODE   hb_rddOpenTable( 
                              const char * szFileName, const char * szDriver,
                              USHORT uiArea, const char *szAlias,
                              BOOL fShared, BOOL fReadonly,
                              const char * szCpId, ULONG ulConnection,
                              PHB_ITEM pStruct, PHB_ITEM pDelim );
extern HB_EXPORT ERRCODE   hb_rddCreateTable(
                              const char * szFileName, const char * szDriver,
                              USHORT uiArea, const char *szAlias,
                              BOOL fKeepOpen,
                              const char * szCpId, ULONG ulConnection,
                              PHB_ITEM pStruct, PHB_ITEM pDelim );
extern HB_EXPORT ERRCODE   hb_dbTransStruct(
                              AREAP lpaSource, AREAP lpaDest,
                              LPDBTRANSINFO lpdbTransInfo,
                              PHB_ITEM *pStruct, PHB_ITEM pFields );
extern HB_EXPORT ERRCODE   hb_rddTransRecords(
                              AREAP pArea,
                              const char *szFileName, const char *szDriver,
                              ULONG ulConnection,
                              PHB_ITEM pFields, BOOL fExport,
                              PHB_ITEM pCobFor, PHB_ITEM pStrFor,
                              PHB_ITEM pCobWhile, PHB_ITEM pStrWhile,
                              PHB_ITEM pNext, PHB_ITEM pRecID,
                              PHB_ITEM pRest,
                              const char *szCpId,
                              PHB_ITEM pDelim );
extern HB_EXPORT void      hb_tblStructure( AREAP pArea, PHB_ITEM pStruct, USHORT uiSize );

#if 0
extern HB_EXPORT ERRCODE   hb_rddDisinherit( const char * drvName );
extern HB_EXPORT USHORT    hb_rddGetCurrentFieldPos( const char * szName );
extern HB_EXPORT USHORT    hb_rddExtendType( HB_TYPE fieldType );
extern HB_EXPORT HB_TYPE   hb_rddFieldType( USHORT extendType );
#endif
typedef ERRCODE ( * WACALLBACK )( AREAP, void * );
extern HB_EXPORT ERRCODE   hb_rddIterateWorkAreas( WACALLBACK pCallBack, void * cargo );
extern HB_EXPORT ERRCODE   hb_rddGetTempAlias( char * szAliasTmp );

HB_EXTERN_END

#endif /* HB_APIRDD_H_ */
