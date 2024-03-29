/*
 * $Id$
 */

/*
 * Harbour Project source code:
 * The eval stack
 *
 * Copyright 1999 Antonio Linares <alinares@fivetech.com>
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

/* TOFIX: There are several things in this file which are not part of the
          standard Harbour API, in other words these things are not
          guaranteed to remain unchanged. To avoid confusion these should be
          moved to somewhere else (like hbrtl.h). [vszakats] */

#ifndef HB_STACK_H_
#define HB_STACK_H_

#include "hbvmpub.h"

HB_EXTERN_BEGIN

#ifdef _HB_API_INTERNAL_

/* stack managed by the virtual machine */
typedef struct
{
   PHB_ITEM * pItems;         /* pointer to the stack items */
   PHB_ITEM * pPos;           /* pointer to the latest used item */
   PHB_ITEM * pEnd;           /* pointer to the end of stack items */
   LONG       wItems;         /* total items that may be holded on the stack */
   HB_ITEM    Return;         /* latest returned value */
   PHB_ITEM * pBase;          /* stack frame position for the current function call */
   PHB_ITEM * pEvalBase;      /* stack frame position for the evaluated codeblock */
   LONG       lStatics;       /* statics base for the current function call */
   LONG       lWithObject;    /* stack offset to base current WITH OBJECT item */
   LONG       lRecoverBase;   /* current SEQUENCE envelope offset or 0 if no SEQUENCE is active */
   USHORT     uiActionRequest;/* Request for some action - stop processing of opcodes */
   HB_STACK_STATE state;      /* first (default) stack state frame */
   char       szDate[ 9 ];    /* last returned date from _pards() yyyymmdd format */
} HB_STACK;

#if defined(HB_STACK_MACROS)
extern HB_STACK hb_stack;
#endif

#endif

extern HB_ITEM_PTR hb_stackItemFromTop( int nFromTop );
extern HB_ITEM_PTR hb_stackItemFromBase( int nFromBase );
extern LONG        hb_stackTopOffset( void );
extern LONG        hb_stackBaseOffset( void );
extern LONG        hb_stackTotalItems( void );
extern HB_ITEM_PTR hb_stackBaseItem( void );
extern HB_ITEM_PTR hb_stackItem( LONG iItemPos );
extern HB_ITEM_PTR hb_stackSelfItem( void );   /* returns Self object at C function level */
extern HB_ITEM_PTR hb_stackReturnItem( void ); /* returns RETURN Item from stack */
extern char *      hb_stackDateBuffer( void );
extern void *      hb_stackId( void );

extern void        hb_stackDec( void );        /* pops an item from the stack without clearing it's contents */
extern HB_EXPORT void hb_stackPop( void );        /* pops an item from the stack */
extern void        hb_stackPush( void );       /* pushes an item on to the stack */
extern HB_ITEM_PTR hb_stackAllocItem( void );  /* allocates new item on the top of stack, returns pointer to it */
extern void        hb_stackPushReturn( void );
extern void        hb_stackPopReturn( void );
extern void        hb_stackRemove( LONG lUntilPos );

/* stack management functions */
extern int        hb_stackCallDepth( void );
extern LONG       hb_stackBaseProcOffset( int iLevel );
extern void       hb_stackBaseProcInfo( char * szProcName, USHORT * puiProcLine ); /* get current .prg function name and line number */
extern void       hb_stackDispLocal( void );  /* show the types of the items on the stack for debugging purposes */
extern void       hb_stackDispCall( void );
extern void       hb_stackFree( void );       /* releases all memory used by the stack */
extern void       hb_stackInit( void );       /* initializes the stack */
extern void       hb_stackIncrease( void );   /* increase the stack size */

#ifdef _HB_API_INTERNAL_
extern void        hb_stackDecrease( ULONG ulItems );
extern HB_ITEM_PTR hb_stackNewFrame( PHB_STACK_STATE pStack, USHORT uiParams );
extern void        hb_stackOldFrame( PHB_STACK_STATE pStack );
extern void        hb_stackClearMevarsBase( void );

extern HB_ITEM_PTR hb_stackLocalVariable( int *piFromBase );
extern PHB_ITEM ** hb_stackItemBasePtr( void );

extern LONG        hb_stackGetRecoverBase( void );
extern void        hb_stackSetRecoverBase( LONG lBase );
extern USHORT      hb_stackGetActionRequest( void );
extern void        hb_stackSetActionRequest( USHORT uiAction );

extern void        hb_stackSetStaticsBase( LONG lBase );
extern LONG        hb_stackGetStaticsBase( void );

extern PHB_ITEM    hb_stackWithObjectItem( void );
extern LONG        hb_stackWithObjectOffset( void );
extern void        hb_stackWithObjectSetOffset( LONG );
#endif

#if defined(HB_STACK_MACROS)

#define hb_stackItemFromTop( n )    ( * ( hb_stack.pPos + ( int ) (n) ) )
#define hb_stackItemFromBase( n )   ( * ( hb_stack.pBase + ( int ) (n) + 1 ) )
#define hb_stackTopOffset( )        ( hb_stack.pPos - hb_stack.pItems )
#define hb_stackBaseOffset( )       ( hb_stack.pBase - hb_stack.pItems + 1 )
#define hb_stackTotalItems( )       ( hb_stack.wItems )
#define hb_stackBaseItem( )         ( * hb_stack.pBase )
#define hb_stackSelfItem( )         ( * ( hb_stack.pBase + 1 ) )
#define hb_stackItem( iItemPos )    ( * ( hb_stack.pItems + ( iItemPos ) ) )
#define hb_stackReturnItem( )       ( &hb_stack.Return )
#define hb_stackDateBuffer( )       ( hb_stack.szDate )
#define hb_stackItemBasePtr( )      ( &hb_stack.pItems )
#define hb_stackGetStaticsBase( )   ( hb_stack.lStatics )
#define hb_stackSetStaticsBase( n ) do { hb_stack.lStatics = ( n ); } while ( 0 )
#define hb_stackGetRecoverBase( )   ( hb_stack.lRecoverBase )
#define hb_stackSetRecoverBase( n ) do { hb_stack.lRecoverBase = ( n ); } while( 0 )
#define hb_stackGetActionRequest( ) ( hb_stack.uiActionRequest )
#define hb_stackSetActionRequest( n )     do { hb_stack.uiActionRequest = ( n ); } while( 0 )
#define hb_stackWithObjectItem( )   ( hb_stack.lWithObject ? * ( hb_stack.pItems + hb_stack.lWithObject ) : NULL )
#define hb_stackWithObjectOffset( ) ( hb_stack.lWithObject )
#define hb_stackWithObjectSetOffset( n )  do { hb_stack.lWithObject = ( n ); } while( 0 )

#define hb_stackId( )               ( ( void * ) &hb_stack )

#define hb_stackAllocItem( )        ( ( ++hb_stack.pPos == hb_stack.pEnd ? \
                                        hb_stackIncrease() : (void) 0 ), \
                                      * ( hb_stack.pPos - 1 ) )

#ifdef HB_STACK_SAFEMACROS

#define hb_stackDecrease( n )       do { \
                                       if( ( hb_stack.pPos -= (n) ) <= hb_stack.pBase ) \
                                          hb_errInternal( HB_EI_STACKUFLOW, NULL, NULL, NULL ); \
                                    } while ( 0 )

#define hb_stackDec( )              do { \
                                       if( --hb_stack.pPos <= hb_stack.pBase ) \
                                          hb_errInternal( HB_EI_STACKUFLOW, NULL, NULL, NULL ); \
                                    } while ( 0 )

#define hb_stackPop( )              do { \
                                       if( --hb_stack.pPos <= hb_stack.pBase ) \
                                          hb_errInternal( HB_EI_STACKUFLOW, NULL, NULL, NULL ); \
                                       if( HB_IS_COMPLEX( * hb_stack.pPos ) ) \
                                          hb_itemClear( * hb_stack.pPos ); \
                                    } while ( 0 )

#define hb_stackPopReturn( )        do { \
                                       if( HB_IS_COMPLEX( &hb_stack.Return ) ) \
                                          hb_itemClear( &hb_stack.Return ); \
                                       if( --hb_stack.pPos <= hb_stack.pBase ) \
                                          hb_errInternal( HB_EI_STACKUFLOW, NULL, NULL, NULL ); \
                                       hb_itemRawMove( &hb_stack.Return, * hb_stack.pPos ); \
                                    } while ( 0 )

#else

#define hb_stackDecrease( n )       do { hb_stack.pPos -= (n); } while ( 0 )
#define hb_stackDec( )              do { --hb_stack.pPos; } while ( 0 )
#define hb_stackPop( )              do { --hb_stack.pPos; \
                                       if( HB_IS_COMPLEX( * hb_stack.pPos ) ) \
                                          hb_itemClear( * hb_stack.pPos ); \
                                    } while ( 0 )
#define hb_stackPopReturn( )        do { \
                                       if( HB_IS_COMPLEX( &hb_stack.Return ) ) \
                                          hb_itemClear( &hb_stack.Return ); \
                                       --hb_stack.pPos; \
                                       hb_itemRawMove( &hb_stack.Return, * hb_stack.pPos ); \
                                    } while ( 0 )

#endif

#define hb_stackPush( )             do { \
                                       if( ++hb_stack.pPos == hb_stack.pEnd ) \
                                          hb_stackIncrease(); \
                                    } while ( 0 )

#define hb_stackPushReturn( )       do { \
                                       hb_itemRawMove( * hb_stack.pPos, &hb_stack.Return ); \
                                       if( ++hb_stack.pPos == hb_stack.pEnd ) \
                                          hb_stackIncrease(); \
                                    } while ( 0 )

#define hb_stackLocalVariable( p )  ( ( ( ( *hb_stack.pBase )->item.asSymbol.paramcnt > \
                                          ( * hb_stack.pBase )->item.asSymbol.paramdeclcnt ) && \
                                        ( * (p) ) > ( * hb_stack.pBase )->item.asSymbol.paramdeclcnt ) ? \
                                      ( * ( hb_stack.pBase + ( int ) ( * (p) += \
                                          ( * hb_stack.pBase )->item.asSymbol.paramcnt - \
                                          ( * hb_stack.pBase )->item.asSymbol.paramdeclcnt ) + 1 ) ) : \
                                      ( * ( hb_stack.pBase + ( int ) ( * (p) ) + 1 ) ) )

#endif


HB_EXTERN_END

#endif /* HB_STACK_H_ */
