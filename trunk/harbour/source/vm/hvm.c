/*
 * $Id$
 */

/*
 * The Harbour virtual machine
 * Copyright(C) 1999 by Antonio Linares.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published
 * by the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR
 * PURPOSE.  See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public
 * License along with this program; if not, write to:
 *
 * The Free Software Foundation, Inc.,
 * 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 * You can contact me at: alinares@fivetech.com
 */

#include <limits.h>
#ifndef __MPW__
 #include <malloc.h>
#endif
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "hbsetup.h"    /* main configuration file */
#include "extend.h"
#include "errorapi.h"
#include "itemapi.h"
#include "langapi.h"
#include "pcode.h"
#include "set.h"
#include "inkey.h"

extern void hb_consoleInitialize( void );
extern void hb_consoleRelease( void );
extern void ReleaseClasses( void );    /* releases all defined classes */
extern void InitSymbolTable( void );   /* initialization of runtime support symbols */

extern ULONG ulMemoryBlocks;      /* memory blocks used */
extern ULONG ulMemoryMaxBlocks;   /* maximum number of used memory blocks */
extern ULONG ulMemoryConsumed;    /* memory size consumed */
extern ULONG ulMemoryMaxConsumed; /* memory max size consumed */

typedef struct _SYMBOLS
{
   PHB_SYMB pModuleSymbols; /* pointer to a one module own symbol table */
   WORD     wModuleSymbols; /* number of symbols on that table */
   struct _SYMBOLS * pNext; /* pointer to the next SYMBOLS structure */
   SYMBOLSCOPE hScope;      /* scope collected from all symbols in module used to speed initialization code */
} SYMBOLS, * PSYMBOLS;      /* structure to keep track of all modules symbol tables */

HARBOUR HB_ERRORSYS( void );
HARBOUR HB_ERRORNEW( void );
HARBOUR HB_EVAL( void );         /* Evaluates a codeblock from Harbour */
HARBOUR HB_MAIN( void );         /* fixed entry point by now */
HARBOUR HB_VALTYPE( void );      /* returns a string description of a value */

/* currently supported virtual machine actions */
void    And( void );             /* performs the logical AND on the latest two values, removes them and leaves result on the stack */
void    ArrayAt( void );         /* pushes an array element to the stack, removing the array and the index from the stack */
void    ArrayPut( void );        /* sets an array value and pushes the value on to the stack */
void    Dec( void );             /* decrements the latest numeric value on the stack */
void    DimArray( WORD wDimensions ); /* generates a wDimensions Array and initialize those dimensions from the stack values */
void    Div( void );             /* divides the latest two values on the stack, removes them and leaves the result */
void    Do( WORD WParams );      /* invoke the virtual machine */
HARBOUR DoBlock( void );         /* executes a codeblock */
void    Duplicate( void );       /* duplicates the latest value on the stack */
void    DuplTwo( void );         /* duplicates the latest two value on the stack */
void    EndBlock( void );        /* copies the last codeblock pushed value into the return value */
void    Equal( BOOL bExact );    /* checks if the two latest values on the stack are equal, removes both and leaves result */
void    ForTest( void );         /* test for end condition of for */
void    Frame( BYTE bLocals, BYTE bParams );  /* increases the stack pointer for the amount of locals and params suplied */
void    FuncPtr( void );         /* pushes a function address pointer. Removes the symbol from the satck */
void    Function( WORD wParams ); /* executes a function saving its result */
void    GenArray( WORD wElements ); /* generates a wElements Array and fills it from the stack values */
void    Greater( void );         /* checks if the latest - 1 value is greater than the latest, removes both and leaves result */
void    GreaterEqual( void );    /* checks if the latest - 1 value is greater than or equal the latest, removes both and leaves result */
void    Inc( void );             /* increment the latest numeric value on the stack */
void    Instring( void );        /* check whether string 1 is contained in string 2 */
void    Less( void );            /* checks if the latest - 1 value is less than the latest, removes both and leaves result */
void    LessEqual( void );       /* checks if the latest - 1 value is less than or equal the latest, removes both and leaves result */
void    Message( PHB_SYMB pSymMsg ); /* sends a message to an object */
void    Minus( void );           /* substracts the latest two values on the stack, removes them and leaves the result */
void    Modulus( void );         /* calculates the modulus of latest two values on the stack, removes them and leaves the result */
void    Mult( void );            /* multiplies the latest two values on the stack, removes them and leaves the result */
void    Negate( void );          /* negates (-) the latest value on the stack */
void    Not( void );             /* changes the latest logical value on the stack */
void    NotEqual( void );        /* checks if the two latest values on the stack are not equal, removes both and leaves result */
void    OperatorCall( PHB_ITEM, PHB_ITEM, char *); /* call an overloaded operator */
void    Or( void );              /* performs the logical OR on the latest two values, removes them and leaves result on the stack */
void    Plus( void );            /* sums the latest two values on the stack, removes them and leaves the result */
long    PopDate( void );         /* pops the stack latest value and returns its date value as a LONG */
void    PopDefStat( WORD wStatic ); /* pops the stack latest value onto a static as default init */
double  PopDouble( WORD * );   /* pops the stack latest value and returns its double numeric format value */
void    PopLocal( SHORT wLocal );      /* pops the stack latest value onto a local */
int     PopLogical( void );           /* pops the stack latest value and returns its logical value */
void    PopMemvar( PHB_SYMB );      /* pops a value of memvar variable */
double  PopNumber( void );          /* pops the stack latest value and returns its numeric value */
void    PopParameter( PHB_SYMB, BYTE );  /* creates a PRIVATE variable and sets it with parameter's value */
void    PopStatic( WORD wStatic );    /* pops the stack latest value onto a static */
void    Power( void );            /* power the latest two values on the stack, removes them and leaves the result */
void    Push( PHB_ITEM pItem );     /* pushes a generic item onto the stack */
void    PushBlock( BYTE * pCode, PHB_SYMB pSymbols ); /* creates a codeblock */
void    PushDate( LONG lDate );   /* pushes a long date onto the stack */
void    PushDouble( double lNumber, WORD wDec ); /* pushes a double number onto the stack */
void    PushLocal( SHORT iLocal );     /* pushes the containts of a local onto the stack */
void    PushLocalByRef( SHORT iLocal ); /* pushes a local by refrence onto the stack */
void    PushLogical( int iTrueFalse ); /* pushes a logical value onto the stack */
void    PushLong( long lNumber ); /* pushes a long number onto the stack */
void    PushMemvar( PHB_SYMB );     /* pushes a value of memvar variable */
void    PushMemvarByRef( PHB_SYMB ); /* pushes a reference to a memvar variable */
void    PushNil( void );            /* in this case it places nil at self */
void    PushNumber( double dNumber, WORD wDec ); /* pushes a number on to the stack and decides if it is integer, long or double */
void    PushStatic( WORD wStatic );   /* pushes the containts of a static onto the stack */
void    PushStaticByRef( WORD iLocal ); /* pushes a static by refrence onto the stack */
void    PushString( char * szText, ULONG length );  /* pushes a string on to the stack */
void    PushSymbol( PHB_SYMB pSym ); /* pushes a function pointer onto the stack */
void    PushInteger( int iNumber ); /* pushes a integer number onto the stack */
void    RetValue( void );           /* pops the latest stack value into stack.Return */
void    SFrame( PHB_SYMB pSym );     /* sets the statics frame for a function */
void    Statics( PHB_SYMB pSym );    /* increases the the global statics array to hold a PRG statics */

void ProcessSymbols( PHB_SYMB pSymbols, WORD wSymbols ); /* statics symbols initialization */
void DoInitStatics( void ); /* executes all _INITSTATICS functions */
void DoInitFunctions( int argc, char * argv[] ); /* executes all defined PRGs INIT functions */
void DoExitFunctions( void ); /* executes all defined PRGs EXIT functions */
void ReleaseLocalSymbols( void );  /* releases the memory of the local symbols linked list */

#ifdef HARBOUR_OBJ_GENERATION
void ProcessObjSymbols ( void ); /* process Harbour generated OBJ symbols */

typedef struct
{
   WORD     wSymbols;             /* module local symbol table symbols amount */
   PHB_SYMB pSymbols;             /* module local symbol table address */
} OBJSYMBOLS, * POBJSYMBOLS;      /* structure used from Harbour generated OBJs */

#ifdef __cplusplus
extern "C" POBJSYMBOLS HB_FIRSTSYMBOL, HB_LASTSYMBOL;
#else
extern POBJSYMBOLS HB_FIRSTSYMBOL, HB_LASTSYMBOL;
#endif
#endif

/* stack management functions */
void StackDec( void );        /* pops an item from the stack without clearing it's contents */
void StackPop( void );        /* pops an item from the stack */
void StackFree( void );       /* releases all memory used by the stack */
void StackPush( void );       /* pushes an item on to the stack */
void StackInit( void );       /* initializes the stack */
void StackShow( void );       /* show the types of the items on the stack for debugging purposes */

static void ForceLink( void );

#define STACK_INITHB_ITEMS   100
#define STACK_EXPANDHB_ITEMS  20

int      iHB_DEBUG = 0;    /* if 1 traces the virtual machine activity */
STACK    stack;
HB_SYMB  symEval = { "__EVAL", FS_PUBLIC, DoBlock, 0 }; /* symbol to evaluate codeblocks */
PHB_SYMB pSymStart;        /* start symbol of the application. MAIN() is not required */
HB_ITEM  aStatics;         /* Harbour array to hold all application statics variables */
HB_ITEM  errorBlock;       /* errorblock */
PSYMBOLS pSymbols = 0;     /* to hold a linked list of all different modules symbol tables */
BOOL     bQuit = FALSE;    /* inmediately exit the application */
BYTE     bErrorLevel = 0;  /* application exit errorlevel */

#define HB_DEBUG( x )     if( iHB_DEBUG ) printf( x )
#define HB_DEBUG2( x, y ) if( iHB_DEBUG ) printf( x, y )

/* application entry point */

int main( int argc, char * argv[] )
{
   int i;
   void ( * DontDiscardForceLink )( void ) = &ForceLink;

   if( ! DontDiscardForceLink )  /* just to avoid warnings from the C compiler */
      iHB_DEBUG += ( int ) DontDiscardForceLink; /* just to avoid warnings from the C compiler */

   HB_DEBUG( "main\n" );

   /* initialize internal data structures */
   aStatics.type     = IT_NIL;
   errorBlock.type   = IT_NIL;
   stack.Return.type = IT_NIL;

   StackInit();
   hb_NewDynSym( &symEval );  /* initialize dynamic symbol for evaluating codeblocks */
   hb_setInitialize();        /* initialize Sets */
   hb_consoleInitialize();    /* initialize Console */
   hb_MemvarsInit();
#ifdef HARBOUR_OBJ_GENERATION
   ProcessObjSymbols(); /* initialize Harbour generated OBJs symbols */
#endif

   /* Initialize symbol table with runtime support functions */
   InitSymbolTable();

   /* Call functions that initializes static variables
    * Static variables have to be initialized before any INIT functions
    * because INIT function can use static variables
    */
   DoInitStatics();
   DoInitFunctions( argc, argv ); /* process defined INIT functions */

#ifdef HARBOUR_START_PROCEDURE
   {
     PDYNSYM pDynSym =hb_FindDynSym( HARBOUR_START_PROCEDURE );
     if( pDynSym )
       pSymStart =pDynSym->pSymbol;
     else
     {
       printf( "Can\'t locate the starting procedure: \'%s\'", HARBOUR_START_PROCEDURE );
       exit(1);
    }
   }
#endif

   PushSymbol( pSymStart ); /* pushes first FS_PUBLIC defined symbol to the stack */

   PushNil();               /* places NIL at self */

   for( i = 1; i < argc; i++ ) /* places application parameters on the stack */
      PushString( argv[ i ], strlen( argv[ i ] ) );

   Do( argc - 1 );          /* invoke it with number of supplied parameters */

   DoExitFunctions();       /* process defined EXIT functions */

   hb_itemClear( &stack.Return );
   hb_arrayRelease( &aStatics );
   hb_itemClear( &errorBlock );
   ReleaseClasses();
   ReleaseLocalSymbols();       /* releases the local modules linked list */
   hb_ReleaseDynSym();          /* releases the dynamic symbol table */
   hb_consoleRelease();         /* releases Console */
   hb_setRelease();             /* releases Sets */
   hb_MemvarsRelease();
   StackFree();
   /* hb_LogDynSym(); */
   HB_DEBUG( "Done!\n" );

   if( ulMemoryBlocks )
   {
      printf( "\n\ntotal memory blocks allocated: %lu\n", ulMemoryMaxBlocks );
      printf( "memory maximum size consumed: %ld\n", ulMemoryMaxConsumed );
      printf( "memory blocks not released: %ld\n", ulMemoryBlocks );
      printf( "memory size not released: %ld\n", ulMemoryConsumed );
   }

   return bErrorLevel;
}

void VirtualMachine( BYTE * pCode, PHB_SYMB pSymbols )
{
   BYTE bCode;
   WORD w = 0, wParams, wSize;
   ULONG ulPrivateBase = hb_MemvarGetPrivatesBase();

   HB_DEBUG( "VirtualMachine\n" );

   while( ( bCode = pCode[ w ] ) != HB_P_ENDPROC && ! bQuit )
   {
      hb_inkeyPoll();                   /* Poll the console keyboard */
      switch( bCode )
      {
         case HB_P_AND:
              And();
              w++;
              break;

         case HB_P_ARRAYAT:
              ArrayAt();
              w++;
              break;

         case HB_P_ARRAYPUT:
              ArrayPut();
              w++;
              break;

         case HB_P_DEC:
              Dec();
              w++;
              break;

         case HB_P_DIMARRAY:
              DimArray( pCode[ w + 1 ] + ( pCode[ w + 2 ] * 256 ) );
              w += 3;
              break;

         case HB_P_DIVIDE:
              Div();
              w++;
              break;

         case HB_P_DO:
              Do( pCode[ w + 1 ] + ( pCode[ w + 2 ] * 256 ) );
              w += 3;
              break;

         case HB_P_DUPLICATE:
              Duplicate();
              w++;
              break;

         case HB_P_DUPLTWO:
              DuplTwo();
              w++;
              break;

         case HB_P_ENDBLOCK:
              EndBlock();
              HB_DEBUG( "EndProc\n" );
              return;   /* end of a codeblock - stop evaluation */

         case HB_P_EQUAL:
              Equal( FALSE );
              w++;
              break;

         case HB_P_EXACTLYEQUAL:
              Equal( TRUE );
              w++;
              break;

         case HB_P_FALSE:
              PushLogical( 0 );
              w++;
              break;

         case HB_P_FORTEST:
              ForTest();
              w++;
              break;

         case HB_P_FRAME:
              Frame( pCode[ w + 1 ], pCode[ w + 2 ] );
              w += 3;
              break;

         case HB_P_FUNCPTR:
              FuncPtr();
              w++;
              break;

         case HB_P_FUNCTION:
              Function( pCode[ w + 1 ] + ( pCode[ w + 2 ] * 256 ) );
              w += 3;
              break;

         case HB_P_GENARRAY:
              GenArray( pCode[ w + 1 ] + ( pCode[ w + 2 ] * 256 ) );
              w += 3;
              break;

         case HB_P_GREATER:
              Greater();
              w++;
              break;

         case HB_P_GREATEREQUAL:
              GreaterEqual();
              w++;
              break;

         case HB_P_INC:
              Inc();
              w++;
              break;

         case HB_P_INSTRING:
              Instring();
              w++;
              break;

         case HB_P_JUMP:
              wParams = pCode[ w + 1 ] + ( pCode[ w + 2 ] * 256 );
              if( wParams )
                 w += wParams;
              else
                 w += 3;
              break;

         case HB_P_JUMPFALSE:
              if( ! PopLogical() )
                 w += pCode[ w + 1 ] + ( pCode[ w + 2 ] * 256 );
              else
                 w += 3;
              break;

         case HB_P_JUMPTRUE:
              if( PopLogical() )
                 w += pCode[ w + 1 ] + ( pCode[ w + 2 ] * 256 );
              else
                 w += 3;
              break;

         case HB_P_LESS:
              Less();
              w++;
              break;

         case HB_P_LESSEQUAL:
              LessEqual();
              w++;
              break;

         case HB_P_LINE:
              stack.pBase->item.asSymbol.lineno = pCode[ w + 1 ] + ( pCode[ w + 2 ] * 256 );
              w += 3;
              break;

         case HB_P_MESSAGE:
              wParams = pCode[ w + 1 ] + ( pCode[ w + 2 ] * 256 );
              Message( pSymbols + wParams );
              w += 3;
              break;

         case HB_P_MINUS:
              Minus();
              w++;
              break;

         case HB_P_MODULUS:
              Modulus();
              w++;
              break;

         case HB_P_MULT:
              Mult();
              w++;
              break;

         case HB_P_NEGATE:
              Negate();
              w++;
              break;

         case HB_P_NOT:
              Not();
              w++;
              break;

         case HB_P_NOTEQUAL:
              NotEqual();
              w++;
              break;

         case HB_P_OR:
              Or();
              w++;
              break;

         case HB_P_PARAMETER:
              wParams = pCode[ w + 1 ] + ( pCode[ w + 2 ] * 256 );
              PopParameter( pSymbols + wParams, pCode[ w+3 ] );
              w +=4;
              break;

         case HB_P_PLUS:
              Plus();
              w++;
              break;

         case HB_P_POP:
              StackPop();
              w++;
              break;

         case HB_P_POPLOCAL:
              PopLocal( pCode[ w + 1 ] + ( pCode[ w + 2 ] * 256 ) );
              w += 3;
              break;

         case HB_P_POPMEMVAR:
              wParams = pCode[ w + 1 ] + ( pCode[ w + 2 ] * 256 );
              PopMemvar( pSymbols + wParams );
              w += 3;
              break;

         case HB_P_POPSTATIC:
              PopStatic( pCode[ w + 1 ] + ( pCode[ w + 2 ] * 256 ) );
              w += 3;
              break;

         case HB_P_POWER:
              Power();
              w++;
              break;

         case HB_P_PUSHBLOCK:
              /* +0    -> _pushblock
               * +1 +2 -> size of codeblock
               * +3 +4 -> number of expected parameters
               * +5 +6 -> number of referenced local variables
               * +7 -> start of table with referenced local variables
               */
              PushBlock( pCode + w, pSymbols );
              w += (pCode[ w + 1 ] + ( pCode[ w + 2 ] * 256 ));
              break;

         case HB_P_PUSHDOUBLE:
              PushDouble( * ( double * ) ( &pCode[ w + 1 ] ), ( WORD ) * ( BYTE * ) &pCode[ w + 1 + sizeof( double ) ] );
              w += 1 + sizeof( double ) + 1;
              break;

         case HB_P_PUSHINT:
              PushInteger( pCode[ w + 1 ] + ( pCode[ w + 2 ] * 256 ) );
              w += 3;
              break;

         case HB_P_PUSHLOCAL:
              PushLocal( pCode[ w + 1 ] + ( pCode[ w + 2 ] * 256 ) );
              w += 3;
              break;

         case HB_P_PUSHLOCALREF:
              PushLocalByRef( pCode[ w + 1 ] + ( pCode[ w + 2 ] * 256 ) );
              w += 3;
              break;

         case HB_P_PUSHLONG:
              PushLong( * ( long * ) ( &pCode[ w + 1 ] ) );
              w += 5;
              break;

         case HB_P_PUSHMEMVAR:
              wParams = pCode[ w + 1 ] + ( pCode[ w + 2 ] * 256 );
              PushMemvar( pSymbols + wParams );
              w += 3;
              break;

         case HB_P_PUSHMEMVARREF:
              wParams = pCode[ w + 1 ] + ( pCode[ w + 2 ] * 256 );
              PushMemvarByRef( pSymbols + wParams );
              w += 3;
              break;

         case HB_P_PUSHNIL:
              PushNil();
              w++;
              break;

         case HB_P_PUSHSELF:
              Push( stack.pBase + 1 );
              w++;
              break;

         case HB_P_PUSHSTATIC:
              PushStatic( pCode[ w + 1 ] + ( pCode[ w + 2 ] * 256 ) );
              w += 3;
              break;

         case HB_P_PUSHSTATICREF:
              PushStaticByRef( pCode[ w + 1 ] + ( pCode[ w + 2 ] * 256 ) );
              w += 3;
              break;

         case HB_P_PUSHSTR:
              wSize = pCode[ w + 1 ] + ( pCode[ w + 2 ] * 256 );
              PushString( (char*)pCode + w + 3, wSize );
              w += ( wSize + 3 );
              break;

         case HB_P_PUSHSYM:
              wParams = pCode[ w + 1 ] + ( pCode[ w + 2 ] * 256 );
              PushSymbol( pSymbols + wParams );
              w += 3;
              break;

         case HB_P_RETVALUE:
              RetValue();
              w++;
              break;

         case HB_P_SFRAME:
              wParams = pCode[ w + 1 ] + ( pCode[ w + 2 ] * 256 );
              SFrame( pSymbols + wParams );
              w += 3;
              break;

         case HB_P_STATICS:
              wParams = pCode[ w + 1 ] + ( pCode[ w + 2 ] * 256 );
              Statics( pSymbols + wParams );
              w += 3;
              break;

         case HB_P_TRUE:
              PushLogical( 1 );
              w++;
              break;

         case HB_P_ZERO:
              PushInteger( 0 );
              w++;
              break;

         case HB_P_NOOP:
              /* Intentionally do nothing */
              break;

         default:
              printf( "The Harbour virtual machine can't run yet this PRG\n(unsuported pcode opcode: %i)\n", bCode );
              printf( "Line number %i in %s", stack.pBase->item.asSymbol.lineno, stack.pBase->item.asSymbol.value->szName );
              exit( 1 );
              break;
      }
   }
   hb_MemvarSetPrivatesBase( ulPrivateBase );
   HB_DEBUG( "EndProc\n" );
}

void And( void )
{
   PHB_ITEM pItem2 = stack.pPos - 1;
   PHB_ITEM pItem1 = stack.pPos - 2;
   int      iResult;

   HB_DEBUG( "And\n" );

   if( IS_LOGICAL( pItem1 ) && IS_LOGICAL( pItem2 ) )
   {
      iResult = pItem1->item.asLogical.value && pItem2->item.asLogical.value;
      StackPop();
      StackPop();
      PushLogical( iResult );
   }
   else
   {
      hb_errorRT_BASE(EG_ARG, 1078, NULL, ".AND.");
   }
}

void ArrayAt( void )
{
   double dIndex = PopNumber();
   PHB_ITEM pArray  = stack.pPos - 1;
   HB_ITEM item;

   hb_arrayGet( pArray, dIndex, &item );
   StackPop();

   hb_itemCopy( stack.pPos, &item );
   hb_itemClear( &item );
   StackPush();
}

void ArrayPut( void )
{
   PHB_ITEM pValue = stack.pPos - 1;
   PHB_ITEM pIndex = stack.pPos - 2;
   PHB_ITEM pArray = stack.pPos - 3;
   ULONG ulIndex;

   if( IS_INTEGER( pIndex ) )
      ulIndex = pIndex->item.asInteger.value;

   else if( IS_LONG( pIndex ) )
      ulIndex = pIndex->item.asLong.value;

   else if( IS_DOUBLE( pIndex ) )
      ulIndex = pIndex->item.asDouble.value;

   else ;
      /* QUESTION: Should we raise an error here ? */

   hb_arraySet( pArray, ulIndex, pValue );
   hb_itemCopy( pArray, pValue );  /* places pValue at pArray position */
   StackPop();
   StackPop();
}

void Dec( void )
{
   double dNumber;
   LONG lDate;
   WORD wDec;

   if( IS_NUMERIC( stack.pPos - 1 ) )
   {
      dNumber = PopDouble( &wDec );
      PushNumber( --dNumber, wDec );
   }
   else if( IS_DATE( stack.pPos - 1 ) )
   {
      lDate = PopDate();
      PushDate( --lDate ); /* TOFIX: Dates should decreased other way */
   }
   /* TODO: Should we check other types here and issue an error ? */
}

void DimArray( WORD wDimensions ) /* generates a wDimensions Array and initialize those dimensions from the stack values */
{
   HB_ITEM itArray;
   WORD w; // , wElements;

   itArray.type = IT_NIL;
   hb_arrayNew( &itArray, ( stack.pPos - wDimensions )->item.asLong.value );

   if( wDimensions > 1 )
   {
      printf( "HVM.c DimArray() does not supports multiple dimensions yet!" );
      exit( 1 );
   }

   // for( w = 0; w < wElements; w++ )
   //   hb_itemCopy( itArray.item.asArray.value->pItems + w,
   //             stack.pPos - wElements + w );

   for( w = 0; w < wDimensions; w++ )
      StackPop();

   hb_itemCopy( stack.pPos, &itArray );
   hb_itemClear( &itArray );
   StackPush();
}

void Div( void )
{
   WORD wDec1, wDec2;
   double d2 = PopDouble( &wDec2 );
   double d1 = PopDouble( &wDec1 );

   /* NOTE: Clipper always returns the result of a division
            with the SET number of decimal places. */
   PushNumber( d1 / d2, hb_set.HB_SET_DECIMALS );
}

void Do( WORD wParams )
{
   PHB_ITEM pItem = stack.pPos - wParams - 2;   /* procedure name */
   PHB_SYMB pSym = pItem->item.asSymbol.value;
   LONG wStackBase = stack.pBase - stack.pItems; /* as the stack memory block could change */
   LONG wItemIndex = pItem - stack.pItems;
   PHB_ITEM pSelf = stack.pPos - wParams - 1;   /* NIL, OBJECT or BLOCK */
   PHB_FUNC pFunc;
   int iStatics = stack.iStatics;              /* Return iStatics position */
   WORD wLineNo = stack.pBase->item.asSymbol.lineno;

   if( ! IS_SYMBOL( pItem ) )
   {
      StackShow();
      printf( "symbol item expected as a base from Do() in line %i\n", stack.pBase->item.asSymbol.lineno );
      exit( 1 );
   }

/*   if( ! IS_NIL( pSelf ) )
   {
      StackShow();
      printf( "invalid symbol type for self from Do()\n" );
      exit( 1 );
   } */

   pItem->item.asSymbol.lineno   = 0;
   pItem->item.asSymbol.paramcnt = wParams;
   stack.pBase    = stack.pItems + pItem->item.asSymbol.stackbase;
   pItem->item.asSymbol.stackbase = wStackBase;

   HB_DEBUG2( "Do with %i params\n", wParams );

   if( ! IS_NIL( pSelf ) ) /* are we sending a message ? */
   {
      if( pSym == &( symEval ) && IS_BLOCK( pSelf ) )
         pFunc = pSym->pFunPtr;                 /* __EVAL method = function */
      else
         pFunc = hb_GetMethod( pSelf, pSym );

      if( ! pFunc )
      {
         printf( "error: message %s not implemented for class %s in line %i\n",
         pSym->szName, hb_GetClassName( pSelf ), wLineNo );
         exit( 1 );
      }
      pFunc();
   }
   else                     /* it is a function */
   {
      pFunc = pSym->pFunPtr;
      if( ! pFunc )
      {
         printf( "error: invalid function pointer (%s) from Do() in line %i\n", pSym->szName, stack.pBase->item.asSymbol.lineno );
         exit( 1 );
      }
      pFunc();
   }

   while( stack.pPos > stack.pItems + wItemIndex )
      StackPop();

   stack.pBase = stack.pItems + wStackBase;
   stack.iStatics = iStatics;
}

HARBOUR DoBlock( void )
{
   PHB_ITEM pBlock = stack.pBase + 1;
   WORD wStackBase = stack.pBase - stack.pItems; /* as the stack memory block could change */
   int iParam;

   if( ! IS_BLOCK( pBlock ) )
   {
      printf( "error: codeblock expected from DoBlock() in line %i\n", stack.pBase->item.asSymbol.lineno );
      exit( 1 );
   }

   /* Check for valid count of parameters */
   iParam =pBlock->item.asBlock.paramcnt -hb_pcount();
   /* add missing parameters */
   while( iParam-- > 0 )
     PushNil();

   /* set pBaseCB to point to local variables of a function where
    * the codeblock was defined
    */
   stack.pBase->item.asSymbol.lineno =pBlock->item.asBlock.lineno;

   hb_CodeblockEvaluate( pBlock );

   /* restore stack pointers */
   stack.pBase = stack.pItems + wStackBase;

   HB_DEBUG( "End of DoBlock\n" );
}

void Duplicate( void )
{
   hb_itemCopy( stack.pPos, stack.pPos - 1 );
   StackPush();
}

void DuplTwo( void )
{
   hb_itemCopy( stack.pPos, stack.pPos - 2 );
   StackPush();
   hb_itemCopy( stack.pPos, stack.pPos - 2 );
   StackPush();
}

HARBOUR HB_EVAL( void )
{
   PHB_ITEM pBlock = hb_param( 1, IT_BLOCK );

   if( pBlock )
   {
      WORD w;

      PushSymbol( &symEval );
      Push( pBlock );

      for( w = 2; w <= hb_pcount(); w++ )
         Push( hb_param( w, IT_ANY ) );

      Do( hb_pcount() - 1 );
   }
   else
   {
      printf( "Not a valid codeblock on eval in line %i\n", stack.pBase->item.asSymbol.lineno );
      exit( 1 );
   }
}

void EndBlock( void )
{
   StackDec();                               /* make the last item visible */
   hb_itemCopy( &stack.Return, stack.pPos ); /* copy it */
   hb_itemClear( stack.pPos );               /* and now clear it */
   HB_DEBUG( "EndBlock\n" );
}

void Equal( BOOL bExact )
{
   PHB_ITEM pItem2 = stack.pPos - 1;
   PHB_ITEM pItem1 = stack.pPos - 2;
   int i;
   WORD wDec;

   if( IS_NIL( pItem1 ) && IS_NIL( pItem2 ) )
   {
      StackPop();
      StackPop();
      PushLogical( 1 );
   }

   else if ( IS_NIL( pItem1 ) || IS_NIL( pItem2 ) )
   {
      StackPop();
      StackPop();
      PushLogical( 0 );
   }

   else if( IS_STRING( pItem1 ) && IS_STRING( pItem2 ) )
   {
      i = hb_itemStrCmp( pItem1, pItem2, bExact );
      StackPop();
      StackPop();
      PushLogical( i == 0 );
   }

   else if( IS_LOGICAL( pItem1 ) && IS_LOGICAL( pItem2 ) )
      PushLogical( PopLogical() == PopLogical() );

   else if( IS_NUMERIC( pItem1 ) && IS_NUMERIC( pItem2 ) )
      PushLogical( PopDouble(&wDec) == PopDouble(&wDec) );

   else if( IS_OBJECT( pItem1 ) && hb_isMessage( pItem1, "==" ) )
      OperatorCall( pItem1, pItem2, "==" );

   else if( pItem1->type != pItem2->type )
   {
      hb_errorRT_BASE(EG_ARG, 1070, NULL, "==");
   }

   else
      PushLogical( 0 );
}

static void ForceLink( void )  /* To force the link of some functions */
{
   HB_ERRORSYS();
   HB_ERRORNEW();
}

void ForTest( void )        /* Test to check the end point of the FOR */
{
   double dStep;
   int    iEqual;

   if( IS_NUMERIC( stack.pPos - 1 ) )
   {
       WORD   wDec;

       dStep = PopDouble( &wDec );

       /* NOTE: step of zero will cause endless loop, as in Clipper */

       if( dStep > 0 )           /* Positive loop. Use LESS */
           Less();
       else if( dStep < 0 )      /* Negative loop. Use GREATER */
           Greater();

       iEqual = PopLogical();    /* Logical should be on top of stack */
       PushNumber( dStep, wDec );   /* Push the step expression back on the stack */
       PushLogical( iEqual );
   }
   else
   {
      hb_errorRT_BASE(EG_ARG, 1073, NULL, "<");
   }
}

void Frame( BYTE bLocals, BYTE bParams )
{
   int i, iTotal = bLocals + bParams;

   HB_DEBUG( "Frame\n" );
   if( iTotal )
      for( i = 0; i < ( iTotal - stack.pBase->item.asSymbol.paramcnt ); i++ )
         PushNil();
}

void FuncPtr( void )  /* pushes a function address pointer. Removes the symbol from the satck */
{
   PHB_ITEM pItem = stack.pPos - 1;

   if( IS_SYMBOL( pItem ) )
   {
      StackPop();
      PushLong( ( ULONG ) pItem->item.asSymbol.value->pFunPtr );
   }
   else
   {
      printf( "symbol item expected from FuncPtr()\n" );
      exit( 1 );
   }
}

void Function( WORD wParams )
{
   Do( wParams );
   hb_itemCopy( stack.pPos, &stack.Return );
   StackPush();
}

void GenArray( WORD wElements ) /* generates a wElements Array and fills it from the stack values */
{
   HB_ITEM itArray;
   WORD w;

   itArray.type = IT_NIL;
   hb_arrayNew( &itArray, wElements );
   for( w = 0; w < wElements; w++ )
      hb_itemCopy( itArray.item.asArray.value->pItems + w,
                stack.pPos - wElements + w );

   for( w = 0; w < wElements; w++ )
      StackPop();

   hb_itemCopy( stack.pPos, &itArray );
   hb_itemClear( &itArray );
   StackPush();
}

void Greater( void )
{
   double dNumber1, dNumber2;
   LONG lDate1, lDate2;
   int i;
   int iLogical1, iLogical2;

   if( IS_STRING( stack.pPos - 2 ) && IS_STRING( stack.pPos - 1 ) )
   {
      i = hb_itemStrCmp( stack.pPos - 2, stack.pPos - 1, FALSE );
      StackPop();
      StackPop();
      PushLogical( i > 0 );
   }

   else if( IS_NUMERIC( stack.pPos - 1 ) && IS_NUMERIC( stack.pPos - 2 ) )
   {
      dNumber2 = PopNumber();
      dNumber1 = PopNumber();
      PushLogical( dNumber1 > dNumber2 );
   }

   else if( IS_DATE( stack.pPos - 1 ) && IS_DATE( stack.pPos - 2 ) )
   {
      lDate2 = PopDate();
      lDate1 = PopDate();
      PushLogical( lDate1 > lDate2 );
   }

   else if( IS_LOGICAL( stack.pPos - 1 ) && IS_LOGICAL( stack.pPos -2 ) )
   {
      iLogical1 = PopLogical();
      iLogical2 = PopLogical();
      PushLogical( iLogical1 > iLogical2 );
   }

   else if( IS_OBJECT( stack.pPos - 2 ) &&
            hb_isMessage( stack.pPos - 2, ">" ) )
      OperatorCall( stack.pPos - 2, stack.pPos - 1, ">" );

   else if( ( stack.pPos - 2 )->type != ( stack.pPos - 1 )->type )
   {
      hb_errorRT_BASE(EG_ARG, 1075, NULL, ">");
   }
}

void GreaterEqual( void )
{
   double dNumber1, dNumber2;
   LONG lDate1, lDate2;
   int i;
   int iLogical1, iLogical2;

   if( IS_STRING( stack.pPos - 2 ) && IS_STRING( stack.pPos - 1 ) )
   {
      i = hb_itemStrCmp( stack.pPos - 2, stack.pPos - 1, FALSE );
      StackPop();
      StackPop();
      PushLogical( i >= 0 );
   }

   else if( IS_NUMERIC( stack.pPos - 1 ) && IS_NUMERIC( stack.pPos - 2 ) )
   {
      dNumber2 = PopNumber();
      dNumber1 = PopNumber();
      PushLogical( dNumber1 >= dNumber2 );
   }

   else if( IS_DATE( stack.pPos - 1 ) && IS_DATE( stack.pPos - 2 ) )
   {
      lDate2 = PopDate();
      lDate1 = PopDate();
      PushLogical( lDate1 >= lDate2 );
   }

   else if( IS_LOGICAL( stack.pPos - 1 ) && IS_LOGICAL( stack.pPos -2 ) )
   {
      iLogical1 = PopLogical();
      iLogical2 = PopLogical();
      PushLogical( iLogical1 >= iLogical2 );
   }

   else if( IS_OBJECT( stack.pPos - 2 ) &&
            hb_isMessage( stack.pPos - 2, ">=" ) )
      OperatorCall( stack.pPos - 2, stack.pPos - 1, ">=" );

   else if( ( stack.pPos - 2 )->type != ( stack.pPos - 1 )->type )
   {
      hb_errorRT_BASE(EG_ARG, 1076, NULL, ">=");
   }
}

void Inc( void )
{
   double dNumber;
   LONG lDate;
   WORD wDec;

   if( IS_NUMERIC( stack.pPos - 1 ) )
   {
      dNumber = PopDouble( &wDec );
      PushNumber( ++dNumber, wDec );
   }
   else if( IS_DATE( stack.pPos - 1 ) )
   {
      lDate = PopDate();
      PushDate( ++lDate );
   }
   else
   {
      hb_errorRT_BASE(EG_ARG, 1086, NULL, "++");
   }
}

void Instring( void )
{
   PHB_ITEM pItem1 = stack.pPos - 2;
   PHB_ITEM pItem2 = stack.pPos - 1;
   int   iResult;

   if( IS_STRING( pItem1 ) && IS_STRING( pItem2 ) )
   {
      iResult = hb_strAt( pItem1->item.asString.value, pItem1->item.asString.length,
                          pItem2->item.asString.value, pItem2->item.asString.length );
      StackPop();
      StackPop();
      PushLogical( iResult == 0 ? 0 : 1 );
   }
   else
   {
      hb_errorRT_BASE(EG_ARG, 1109, NULL, "$");
   }
}

void Less( void )
{
   double dNumber1, dNumber2;
   LONG lDate1, lDate2;
   int i;
   int iLogical1, iLogical2;

   if( IS_STRING( stack.pPos - 2 ) && IS_STRING( stack.pPos - 1 ) )
   {
      i = hb_itemStrCmp( stack.pPos - 2, stack.pPos - 1, FALSE );
      StackPop();
      StackPop();
      PushLogical( i < 0 );
   }

   else if( IS_NUMERIC( stack.pPos - 1 ) && IS_NUMERIC( stack.pPos - 2 ) )
   {
      dNumber2 = PopNumber();
      dNumber1 = PopNumber();
      PushLogical( dNumber1 < dNumber2 );
   }

   else if( IS_DATE( stack.pPos - 1 ) && IS_DATE( stack.pPos - 2 ) )
   {
      lDate2 = PopDate();
      lDate1 = PopDate();
      PushLogical( lDate1 < lDate2 );
   }

   else if( IS_LOGICAL( stack.pPos - 1 ) && IS_LOGICAL( stack.pPos -2 ) )
   {
      iLogical1 = PopLogical();
      iLogical2 = PopLogical();
      PushLogical( iLogical1 < iLogical2 );
   }

   else if( IS_OBJECT( stack.pPos - 2 ) &&
            hb_isMessage( stack.pPos - 2, "<" ) )
      OperatorCall( stack.pPos - 2, stack.pPos - 1, "<" );

   else if( ( stack.pPos - 2 )->type != ( stack.pPos - 1 )->type )
   {
      hb_errorRT_BASE(EG_ARG, 1073, NULL, "<");
   }
}

void LessEqual( void )
{
   double dNumber1, dNumber2;
   LONG lDate1, lDate2;
   int i;
   int iLogical1, iLogical2;

   if( IS_STRING( stack.pPos - 2 ) && IS_STRING( stack.pPos - 1 ) )
   {
      i = hb_itemStrCmp( stack.pPos - 2, stack.pPos - 1, FALSE );
      StackPop();
      StackPop();
      PushLogical( i <= 0 );
   }

   else if( IS_NUMERIC( stack.pPos - 1 ) && IS_NUMERIC( stack.pPos - 2 ) )
   {
      dNumber2 = PopNumber();
      dNumber1 = PopNumber();
      PushLogical( dNumber1 <= dNumber2 );
   }

   else if( IS_DATE( stack.pPos - 1 ) && IS_DATE( stack.pPos - 2 ) )
   {
      lDate2 = PopDate();
      lDate1 = PopDate();
      PushLogical( lDate1 <= lDate2 );
   }

   else if( IS_LOGICAL( stack.pPos - 1 ) && IS_LOGICAL( stack.pPos -2 ) )
   {
      iLogical1 = PopLogical();
      iLogical2 = PopLogical();
      PushLogical( iLogical1 <= iLogical2 );
   }

   else if( IS_OBJECT( stack.pPos - 2 ) &&
            hb_isMessage( stack.pPos - 2, "<=" ) )
      OperatorCall( stack.pPos - 2, stack.pPos - 1, "<=" );

   else if( ( stack.pPos - 2 )->type != ( stack.pPos - 1 )->type )
   {
      hb_errorRT_BASE(EG_ARG, 1074, NULL, "<=");
   }
}

void Message( PHB_SYMB pSymMsg ) /* sends a message to an object */
{
   hb_itemCopy( stack.pPos, stack.pPos - 1 ); /* moves the object forward */
   hb_itemClear( stack.pPos - 1 );
   ( stack.pPos - 1 )->type = IT_SYMBOL;
   ( stack.pPos - 1 )->item.asSymbol.value = pSymMsg;
   ( stack.pPos - 1 )->item.asSymbol.stackbase = ( stack.pPos - 1 ) - stack.pItems;
   StackPush();
   HB_DEBUG2( "Message: %s\n", pSymMsg->szName );
}

void Negate( void )
{
   if( IS_INTEGER( stack.pPos - 1 ) )
      ( stack.pPos - 1 )->item.asInteger.value = -( stack.pPos - 1 )->item.asInteger.value;

   else if( IS_LONG( stack.pPos - 1 ) )
      ( stack.pPos - 1 )->item.asLong.value = -( stack.pPos - 1 )->item.asLong.value;

   else if( IS_DOUBLE( stack.pPos - 1 ) )
      ( stack.pPos - 1 )->item.asDouble.value = -( stack.pPos - 1 )->item.asDouble.value;
}

void Not( void )
{
   PHB_ITEM pItem = stack.pPos - 1;

   if( IS_LOGICAL( pItem ) )
      pItem->item.asLogical.value = ! pItem->item.asLogical.value;
   else
      hb_errorRT_BASE(EG_ARG, 1077, NULL, ".NOT.");
}

void NotEqual( void )
{
   PHB_ITEM pItem2 = stack.pPos - 1;
   PHB_ITEM pItem1 = stack.pPos - 2;
   int i;
   WORD wDec;

   if( IS_NIL( pItem1 ) && IS_NIL( pItem2 ) )
   {
      StackDec();
      StackDec();
      PushLogical( 0 );
   }

   else if ( IS_NIL( pItem1 ) || IS_NIL( pItem2 ) )
   {
      StackPop();
      StackPop();
      PushLogical( 1 );  /* TOFIX: Is this correct ? */
   }

   else if( IS_STRING( pItem1 ) && IS_STRING( pItem2 ) )
   {
      i = hb_itemStrCmp( pItem1, pItem2, FALSE );
      StackPop();
      StackPop();
      PushLogical( i != 0 );
   }

   else if( IS_NUMERIC( pItem1 ) && IS_NUMERIC( pItem2 ) )
      PushLogical( PopDouble( &wDec ) != PopDouble( &wDec ) );

   else if( IS_LOGICAL( pItem1 ) && IS_LOGICAL( pItem2 ) )
      PushLogical( PopLogical() != PopLogical() );

   else if( IS_OBJECT( pItem1 ) && hb_isMessage( pItem1, "!=" ) )
      OperatorCall( pItem1, pItem2, "!=" );

   else if( pItem1->type != pItem2->type )
   {
      hb_errorRT_BASE(EG_ARG, 1072, NULL, "<>");
   }

   else
      PushLogical( 1 );
}

void Minus( void )
{
   double dNumber1, dNumber2;
   long lDate1, lDate2;
   PHB_ITEM pItem2 = stack.pPos - 1;
   PHB_ITEM pItem1 = stack.pPos - 2;

   if( IS_NUMERIC( pItem2 ) && IS_NUMERIC( pItem1 ) )
   {
      WORD wDec2, wDec1;
      dNumber2 = PopDouble( &wDec2 );
      dNumber1 = PopDouble( &wDec1 );
      PushNumber( dNumber1 - dNumber2, (wDec1 > wDec2) ? wDec1 : wDec2 );
   }
   else if( IS_DATE( pItem2 ) && IS_DATE( pItem1 ) )
   {
      lDate2 = PopDate();
      lDate1 = PopDate();
      PushNumber( lDate1 - lDate2, hb_set.HB_SET_DECIMALS );
   }
   else if( IS_NUMERIC( pItem2 ) && IS_DATE( pItem1 ) )
   {
      dNumber2 = PopNumber();
      lDate1 = PopDate();
      PushDate( lDate1 - dNumber2 );
   }
   else if( IS_STRING( pItem1 ) && IS_STRING( pItem2 ) )
   {
      ULONG ulLen = pItem1->item.asString.length;

      pItem1->item.asString.value = (char*)hb_xrealloc( pItem1->item.asString.value, pItem1->item.asString.length + pItem2->item.asString.length + 1 );
      pItem1->item.asString.length += pItem2->item.asString.length;

      while( ulLen && pItem1->item.asString.value[ulLen - 1] == ' ' )
      {
         ulLen--;
      }

      memcpy( pItem1->item.asString.value + ulLen, pItem2->item.asString.value, pItem2->item.asString.length );
      ulLen += pItem2->item.asString.length;
      memset( pItem1->item.asString.value + ulLen, ' ', pItem1->item.asString.length - ulLen);
      pItem1->item.asString.value[ pItem1->item.asString.length ] = 0;

      if( pItem2->item.asString.value )
      {
         hb_xfree( pItem2->item.asString.value );
         pItem2->item.asString.value = NULL;
      }
      StackPop();
      return;
   }
   else if( IS_OBJECT( stack.pPos - 2 ) && hb_isMessage( stack.pPos - 2, "-" ) )
      OperatorCall( stack.pPos - 2, stack.pPos - 1, "-" );
   else
      hb_errorRT_BASE(EG_ARG, 1082, NULL, "-");

}

void Modulus( void )
{
   WORD wDec1, wDec2;
   double d2 = PopDouble( &wDec2 );
   double d1 = PopDouble( &wDec1 );

   /* NOTE: Clipper always returns the result of modulus
            with the SET number of decimal places. */
   PushNumber( fmod( d1, d2 ), hb_set.HB_SET_DECIMALS );
}

void Mult( void )
{
   WORD wDec2, wDec1;
   double d2 = PopDouble( &wDec2 );
   double d1 = PopDouble( &wDec1 );

   PushNumber( d1 * d2, wDec1 + wDec2 );
}

void OperatorCall( PHB_ITEM pItem1, PHB_ITEM pItem2, char *szSymbol )
{
   Push( pItem1 );                             /* Push object              */
   Message( hb_GetDynSym( szSymbol )->pSymbol );  /* Push operation           */
   Push( pItem2 );                             /* Push argument            */
   Function( 1 );
}

void Or( void )
{
   PHB_ITEM pItem2 = stack.pPos - 1;
   PHB_ITEM pItem1 = stack.pPos - 2;
   int   iResult;

   if( IS_LOGICAL( pItem1 ) && IS_LOGICAL( pItem2 ) )
   {
      iResult = pItem1->item.asLogical.value || pItem2->item.asLogical.value;
      StackDec(); stack.pPos->type =IT_NIL;
      StackDec();
      PushLogical( iResult );
   }
   else
   {
      hb_errorRT_BASE(EG_ARG, 1079, NULL, ".OR.");
   }
}

void Plus( void )
{
   PHB_ITEM pItem1 = stack.pPos - 2;
   PHB_ITEM pItem2 = stack.pPos - 1;
   double dNumber1, dNumber2;
   long lDate1, lDate2;

   if( IS_STRING( pItem1 ) && IS_STRING( pItem2 ) )
   {
      pItem1->item.asString.value = (char*)hb_xrealloc( pItem1->item.asString.value, pItem1->item.asString.length + pItem2->item.asString.length + 1 );
      memcpy( pItem1->item.asString.value+ pItem1->item.asString.length,
              pItem2->item.asString.value, pItem2->item.asString.length );
      pItem1->item.asString.length += pItem2->item.asString.length;
      pItem1->item.asString.value[ pItem1->item.asString.length ] = 0;
      if( pItem2->item.asString.value )
      {
         hb_xfree( pItem2->item.asString.value );
         pItem2->item.asString.value = NULL;
      }
      StackPop();
      return;
   }

   else if( IS_NUMERIC( pItem1 ) && IS_NUMERIC( pItem2 ) )
   {
      WORD wDec2, wDec1;
      dNumber2 = PopDouble( &wDec2 );
      dNumber1 = PopDouble( &wDec1 );

      PushNumber( dNumber1 + dNumber2, (wDec1>wDec2) ? wDec1 : wDec2 );
   }

   else if( IS_DATE( pItem1 ) && IS_DATE( pItem2 ) )
   {
      lDate2 = PopDate();
      lDate1 = PopDate();
      PushDate( lDate1 + lDate2 );
   }

   else if( IS_DATE( pItem1 ) && IS_NUMERIC( pItem2 ) )
   {
      WORD wDec;
      dNumber2 = PopDouble( &wDec );
      lDate1 = PopDate();
      PushDate( lDate1 + dNumber2 );
   }

   else if( IS_OBJECT( pItem1 ) && hb_isMessage( pItem2, "+" ) )
      OperatorCall( pItem1, pItem2, "+" );

   else
      hb_errorRT_BASE( EG_ARG, 1081, NULL, "+" );

   HB_DEBUG( "Plus\n" );
}

long PopDate( void )
{
   StackDec();

   if( IS_DATE( stack.pPos ) )
   {
      stack.pPos->type =IT_NIL;
      return stack.pPos->item.asDate.value;
   }
   else
   {
      printf( "incorrect item value trying to Pop a date value in line %i\n", stack.pBase->item.asSymbol.lineno );
      exit( 1 );
      return 0;
   }
}


double PopDouble( WORD *pwDec )
{
   double d;

   StackDec();
   switch( stack.pPos->type & ~IT_BYREF )
   {
      case IT_INTEGER:
           d = stack.pPos->item.asInteger.value;
           *pwDec =0;
           break;

      case IT_LONG:
           d = stack.pPos->item.asLong.value;
           *pwDec =0;
           break;

      case IT_DOUBLE:
           d = stack.pPos->item.asDouble.value;
           *pwDec =stack.pPos->item.asDouble.decimal;
           break;

      default:
           printf( "Incorrect item type trying to Pop a double in line %i\n", stack.pBase->item.asSymbol.lineno );
           exit( 1 );
           d = 0;
   }
   stack.pPos->type =IT_NIL;
   HB_DEBUG( "PopDouble\n" );
   return d;
}

void PopLocal( SHORT iLocal )
{
   PHB_ITEM pLocal;

   StackDec();

   if( iLocal >= 0 )
    {
      /* local variable or local parameter */
      pLocal = stack.pBase + 1 + iLocal;
      if( IS_BYREF( pLocal ) )
         hb_itemCopy( hb_itemUnRef( pLocal ), stack.pPos );
      else
         hb_itemCopy( pLocal, stack.pPos );
    }
   else
      /* local variable referenced in a codeblock
       * stack.pBase+1 points to a codeblock that is currently evaluated
       */
      hb_itemCopy( hb_CodeblockGetVar( stack.pBase + 1, iLocal ), stack.pPos );

   hb_itemClear( stack.pPos );
   HB_DEBUG( "PopLocal\n" );
}

int PopLogical( void )
{
   StackDec();

   if( IS_LOGICAL( stack.pPos ) )
   {
      stack.pPos->type =IT_NIL;
      return stack.pPos->item.asLogical.value;
   }
   else
   {
      hb_errorRT_BASE(EG_ARG, 1066, NULL, hb_langDGetErrorDesc(EG_CONDITION));
      return 0;
   }
}

void PopMemvar( PHB_SYMB pSym )
{
   StackDec();
   hb_MemvarSetValue( pSym, stack.pPos );
   hb_itemClear( stack.pPos );
   HB_DEBUG( "PopMemvar\n" );
}

double PopNumber( void )
{
   PHB_ITEM pItem = stack.pPos - 1;
   double dNumber;

   StackDec();

   switch( pItem->type & ~IT_BYREF )
   {
      case IT_INTEGER:
           dNumber = ( double ) pItem->item.asInteger.value;
           break;

      case IT_LONG:
           dNumber = ( double ) pItem->item.asLong.value;
           break;

      case IT_DOUBLE:
           dNumber = pItem->item.asDouble.value;
           break;

      default:
           printf( "Incorrect item on the stack trying to pop a number in line %i\n", stack.pBase->item.asSymbol.lineno );
           StackShow();
           exit( 1 );
           break;
   }
   stack.pPos->type =IT_NIL;
   return dNumber;
}

void PopParameter( PHB_SYMB pSym, BYTE bParam )
{
   hb_MemvarSetValue( pSym, stack.pBase +1 +bParam );
   HB_DEBUG( "PopParameter\n" );
}

void PopStatic( WORD wStatic )
{
   PHB_ITEM pStatic;

   StackDec();
   pStatic = aStatics.item.asArray.value->pItems + stack.iStatics + wStatic - 1;

   if( IS_BYREF( pStatic ) )
      hb_itemCopy( hb_itemUnRef( pStatic ), stack.pPos );
   else
      hb_itemCopy( pStatic, stack.pPos );

   hb_itemClear( stack.pPos );
   HB_DEBUG( "PopStatic\n" );
}

void Power( void )
{
   WORD wDec1, wDec2;
   double d2 = PopDouble( &wDec2 );
   double d1 = PopDouble( &wDec1 );

   /* NOTE: Clipper always returns the result of power
            with the SET number of decimal places. */
    PushNumber( pow( d1, d2 ), hb_set.HB_SET_DECIMALS );
}

void PushLogical( int iTrueFalse )
{
   stack.pPos->type    = IT_LOGICAL;
   stack.pPos->item.asLogical.value = iTrueFalse;
   StackPush();
   HB_DEBUG( "PushLogical\n" );
}

void PushLocal( SHORT iLocal )
{
   if( iLocal >= 0 )
   {
      PHB_ITEM pLocal;

      /* local variable or local parameter */
      pLocal = stack.pBase + 1 + iLocal;
      if( IS_BYREF( pLocal ) )
         hb_itemCopy( stack.pPos, hb_itemUnRef( pLocal ) );
      else
         hb_itemCopy( stack.pPos, pLocal );
   }
   else
      /* local variable referenced in a codeblock
       * stack.pBase+1 points to a codeblock that is currently evaluated
       */
     hb_itemCopy( stack.pPos, hb_CodeblockGetVar( stack.pBase + 1, (LONG)iLocal ) );

   StackPush();
   HB_DEBUG2( "PushLocal %i\n", iLocal );
}

void PushLocalByRef( SHORT iLocal )
{
   stack.pPos->type = IT_BYREF;
   /* we store its stack offset instead of a pointer to support a dynamic stack */
   stack.pPos->item.asRefer.value = iLocal;
   stack.pPos->item.asRefer.offset = stack.pBase - stack.pItems +1;
   stack.pPos->item.asRefer.itemsbase = &stack.pItems;

   StackPush();
   HB_DEBUG2( "PushLocalByRef %i\n", iLocal );
}

void PushMemvar( PHB_SYMB pSym )
{
   hb_MemvarGetValue( stack.pPos, pSym );
   StackPush();
   HB_DEBUG( "PushMemvar\n" );
}

void PushMemvarByRef( PHB_SYMB pSym )
{
   hb_MemvarGetRefer( stack.pPos, pSym );
   StackPush();
   HB_DEBUG( "PushMemvar\n" );
}

void PushNil( void )
{
   stack.pPos->type =IT_NIL;
   StackPush();
   HB_DEBUG( "PushNil\n" );
}

void PushNumber( double dNumber, WORD wDec )
{
   if( wDec )
      PushDouble( dNumber, wDec );

   else if( SHRT_MIN <= dNumber && dNumber <= SHRT_MAX )
      PushInteger( dNumber );

   else if( LONG_MIN <= dNumber && dNumber <= LONG_MAX )
      PushLong( dNumber );

   else
      PushDouble( dNumber, hb_set.HB_SET_DECIMALS );
}

void PushStatic( WORD wStatic )
{
   PHB_ITEM pStatic;

   pStatic = aStatics.item.asArray.value->pItems + stack.iStatics + wStatic - 1;
   if( IS_BYREF(pStatic) )
      hb_itemCopy( stack.pPos, hb_itemUnRef(pStatic) );
   else
      hb_itemCopy( stack.pPos, pStatic );
   StackPush();
   HB_DEBUG2( "PushStatic %i\n", wStatic );
}

void PushStaticByRef( WORD wStatic )
{
   stack.pPos->type = IT_BYREF;
   /* we store the offset instead of a pointer to support a dynamic stack */
   stack.pPos->item.asRefer.value = wStatic -1;
   stack.pPos->item.asRefer.offset = stack.iStatics;
   stack.pPos->item.asRefer.itemsbase = &aStatics.item.asArray.value->pItems;

   StackPush();
   HB_DEBUG2( "PushStaticByRef %i\n", wStatic );
}

void PushString( char * szText, ULONG length )
{
   char * szTemp = ( char * ) hb_xgrab( length + 1 );

   memcpy (szTemp, szText, length);
   szTemp[ length ] = 0;

   stack.pPos->type                 = IT_STRING;
   stack.pPos->item.asString.length = length;
   stack.pPos->item.asString.value  = szTemp;
   StackPush();
   HB_DEBUG( "PushString\n" );
}

void PushSymbol( PHB_SYMB pSym )
{
   stack.pPos->type   = IT_SYMBOL;
   stack.pPos->item.asSymbol.value = pSym;
   stack.pPos->item.asSymbol.stackbase   = stack.pPos - stack.pItems;
   StackPush();
   HB_DEBUG2( "PushSymbol: %s\n", pSym->szName );
}

void Push( PHB_ITEM pItem )
{
   hb_itemCopy( stack.pPos, pItem );
   StackPush();
   HB_DEBUG( "Push\n" );
}

/* +0   -> HB_P_PUSHBLOCK
* +1 +2 -> size of codeblock
* +3 +4 -> number of expected parameters
* +5 +6 -> number of referenced local variables
* +7 -> start of table with referenced local variables
*/
void PushBlock( BYTE * pCode, PHB_SYMB pSymbols )
{
   WORD wLocals;

   stack.pPos->type   = IT_BLOCK;

   wLocals =pCode[ 5 ] + ( pCode[ 6 ] * 256 );
   stack.pPos->item.asBlock.value =
         hb_CodeblockNew( pCode + 7 + wLocals*2, /* pcode buffer         */
         wLocals,                             /* number of referenced local variables */
         (WORD *)(pCode +7),                  /* table with referenced local variables */
         pSymbols );

   /* store the statics base of function where the codeblock was defined
    */
   stack.pPos->item.asBlock.statics = stack.iStatics;
   /* store the number of expected parameters
    */
   stack.pPos->item.asBlock.paramcnt = pCode[ 3 ] + ( pCode[ 4 ] * 256 );
   /* store the line number where the codeblock was defined
    */
   stack.pPos->item.asBlock.lineno = stack.pBase->item.asSymbol.lineno;
   StackPush();
   HB_DEBUG( "PushBlock\n" );
}

void PushDate( LONG lDate )
{
   stack.pPos->type   = IT_DATE;
   stack.pPos->item.asDate.value = lDate;
   StackPush();
   HB_DEBUG( "PushDate\n" );
}

void PushDouble( double dNumber, WORD wDec )
{
   stack.pPos->type   = IT_DOUBLE;
   stack.pPos->item.asDouble.value = dNumber;
   if( dNumber >= 10000000000.0 ) stack.pPos->item.asDouble.length = 20;
   else stack.pPos->item.asDouble.length = 10;
   stack.pPos->item.asDouble.decimal = (wDec > 9) ? 9 : wDec;
   StackPush();
   HB_DEBUG( "PushDouble\n" );
}

void PushInteger( int iNumber )
{
   stack.pPos->type = IT_INTEGER;
   stack.pPos->item.asInteger.value   = iNumber;
   stack.pPos->item.asInteger.length  = 10;
   stack.pPos->item.asInteger.decimal = 0;
   StackPush();
   HB_DEBUG( "PushInteger\n" );
}

void PushLong( long lNumber )
{
   stack.pPos->type   = IT_LONG;
   stack.pPos->item.asLong.value   = lNumber;
   stack.pPos->item.asLong.length  = 10;
   stack.pPos->item.asLong.decimal = 0;
   StackPush();
   HB_DEBUG( "PushLong\n" );
}

void RetValue( void )
{
   StackDec();                               /* make the last item visible */
   hb_itemCopy( &stack.Return, stack.pPos ); /* copy it */
   hb_itemClear( stack.pPos );               /* now clear it */
   HB_DEBUG( "RetValue\n" );
}

void StackPop( void )
{
   if( --stack.pPos < stack.pItems )
   {
      printf( "runtime error: stack underflow\n" );
      exit( 1 );
   }
   if( stack.pPos->type )
      hb_itemClear( stack.pPos );
}

void StackDec( void )
{
   if( --stack.pPos < stack.pItems )
   {
      printf( "runtime error: stack underflow\n" );
      exit( 1 );
   }
}

void StackFree( void )
{
   hb_xfree( stack.pItems );
   HB_DEBUG( "StackFree\n" );
}

void StackPush( void )
{
   LONG CurrIndex;   /* index of current top item */
   LONG TopIndex;    /* index of the topmost possible item */

   CurrIndex = stack.pPos - stack.pItems;
   TopIndex  = stack.wItems - 1;

   /* enough room for another item ? */
   if( !( TopIndex > CurrIndex ) )
   {
      LONG BaseIndex;   /* index of stack base */

      BaseIndex = stack.pBase - stack.pItems;

      /* no, make more headroom: */
      /* StackShow(); */
      stack.pItems = (PHB_ITEM)hb_xrealloc( stack.pItems, sizeof( HB_ITEM ) *
                                ( stack.wItems + STACK_EXPANDHB_ITEMS ) );

      /* fix possibly invalid pointers: */
      stack.pPos = stack.pItems + CurrIndex;
      stack.pBase = stack.pItems + BaseIndex;
      stack.wItems += STACK_EXPANDHB_ITEMS;
      /* StackShow(); */
   }

   /* now, push it: */
   stack.pPos++;
   stack.pPos->type = IT_NIL;
   return;
}

void StackInit( void )
{
   stack.pItems = ( PHB_ITEM ) hb_xgrab( sizeof( HB_ITEM ) * STACK_INITHB_ITEMS );
   stack.pBase  = stack.pItems;
   stack.pPos   = stack.pItems;     /* points to the first stack item */
   stack.wItems = STACK_INITHB_ITEMS;
   HB_DEBUG( "StackInit\n" );
}

void StackShow( void )
{
   PHB_ITEM p;

   for( p = stack.pBase; p <= stack.pPos; p++ )
   {
      switch( p->type )
      {
         case IT_NIL:
              printf( "NIL " );
              break;

         case IT_ARRAY:
              if( p->item.asArray.value->wClass )
                 printf( "OBJECT " );
              else
                 printf( "ARRAY " );
              break;

         case IT_BLOCK:
              printf( "BLOCK " );
              break;

         case IT_DATE:
              printf( "DATE " );
              break;

         case IT_DOUBLE:
              printf( "DOUBLE " );
              break;

         case IT_LOGICAL:
              printf( "LOGICAL[%i] ", p->item.asLogical.value );
              break;

         case IT_LONG:
              printf( "LONG" );
              break;

         case IT_INTEGER:
              printf( "INTEGER[%i] ", p->item.asInteger.value );
              break;

         case IT_STRING:
              printf( "STRING " );
              break;

         case IT_SYMBOL:
              printf( "SYMBOL(%s) ", p->item.asSymbol.value->szName );
              break;

         default:
              printf( "UNKNOWN[%i] ", p->type );
              break;
      }
   }
   printf( "\n" );
}

void SFrame( PHB_SYMB pSym )      /* sets the statics frame for a function */
{
   /* _INITSTATICS is now the statics frame. Statics() changed it! */
   stack.iStatics = ( int ) pSym->pFunPtr; /* pSym is { "$_INITSTATICS", FS_INIT | FS_EXIT, _INITSTATICS } for each PRG */
   HB_DEBUG( "SFrame\n" );
}

void Statics( PHB_SYMB pSym ) /* initializes the global aStatics array or redimensionates it */
{
   WORD wStatics = PopNumber();

   if( IS_NIL( &aStatics ) )
   {
      pSym->pFunPtr = 0;         /* statics frame for this PRG */
      hb_arrayNew( &aStatics, wStatics );
   }
   else
   {
      pSym->pFunPtr = ( PHB_FUNC )hb_arrayLen( &aStatics );
      hb_arraySize( &aStatics, hb_arrayLen( &aStatics ) + wStatics );
   }

   HB_DEBUG2( "Statics %li\n", hb_arrayLen( &aStatics ) );
}

void ProcessSymbols( PHB_SYMB pModuleSymbols, WORD wModuleSymbols ) /* module symbols initialization */
{
   PSYMBOLS pNewSymbols, pLastSymbols;
   WORD w;
   SYMBOLSCOPE hSymScope;

#ifdef HARBOUR_OBJ_GENERATION
   static int iObjChecked = 0;

   if( ! iObjChecked )
   {
      iObjChecked = 1;
      ProcessObjSymbols();   /* to asure Harbour OBJ symbols are processed first */
   }
#endif

   pNewSymbols = ( PSYMBOLS ) hb_xgrab( sizeof( SYMBOLS ) );
   pNewSymbols->pModuleSymbols = pModuleSymbols;
   pNewSymbols->wModuleSymbols = wModuleSymbols;
   pNewSymbols->pNext = 0;
   pNewSymbols->hScope =0;

   if( ! pSymbols )
      pSymbols = pNewSymbols;
   else
   {
      pLastSymbols = pSymbols;
      while( pLastSymbols->pNext ) /* locates the latest processed group of symbols */
         pLastSymbols = pLastSymbols->pNext;
      pLastSymbols->pNext = pNewSymbols;
   }

   for( w = 0; w < wModuleSymbols; w++ ) /* register each public symbol on the dynamic symbol table */
   {
      hSymScope =( pModuleSymbols + w )->cScope;
      pNewSymbols->hScope |=hSymScope;
      if( ( ! pSymStart ) && ( hSymScope == FS_PUBLIC ) )
         pSymStart = pModuleSymbols + w;  /* first public defined symbol to start execution */

      if( (hSymScope == FS_PUBLIC) || (hSymScope & ( FS_MESSAGE | FS_MEMVAR )) )
         hb_NewDynSym( pModuleSymbols + w );
   }
}

#ifdef HARBOUR_OBJ_GENERATION
void ProcessObjSymbols( void )
{
   POBJSYMBOLS pObjSymbols = ( POBJSYMBOLS ) &HB_FIRSTSYMBOL;

   static int iDone = 0;

   if( ! iDone )
   {
      iDone = 1;
      while( pObjSymbols < ( POBJSYMBOLS ) ( &HB_LASTSYMBOL - 1 ) )
      {
         ProcessSymbols( pObjSymbols->pSymbols, pObjSymbols->wSymbols );
         pObjSymbols++;
      }
   }
}
#endif

void ReleaseLocalSymbols( void )
{
   PSYMBOLS pDestroy;

   while( pSymbols )
   {
      pDestroy = pSymbols;
      pSymbols = pSymbols->pNext;
      hb_xfree( pDestroy );
   }
}

/* This calls all _INITSTATICS functions defined in the application.
 * We are using a special symbol's scope (FS_INIT | FS_EXIT) to mark
 * this function. These two bits cannot be marked at the same
 * time for normal user defined functions.
 */
void DoInitStatics( void )
{
   PSYMBOLS pLastSymbols = pSymbols;
   WORD w;
   SYMBOLSCOPE scope;

   do {
      if( (pLastSymbols->hScope & (FS_INIT | FS_EXIT)) == (FS_INIT | FS_EXIT) )
      {
         for( w = 0; w < pLastSymbols->wModuleSymbols; w++ )
         {
            scope =( pLastSymbols->pModuleSymbols + w )->cScope & (FS_EXIT | FS_INIT);
            if( scope == (FS_INIT | FS_EXIT) )
            {
               /* _INITSTATICS procedure cannot call any function and it
               * cannot use any local variable then it is safe to call
               * this procedure directly
               * PushSymbol( pLastSymbols->pModuleSymbols + w );
               * PushNil();
               * Do( 0 );
               */
               if( ( pLastSymbols->pModuleSymbols + w )->pFunPtr )
                  ( pLastSymbols->pModuleSymbols + w )->pFunPtr();
            }
         }
      }
      pLastSymbols = pLastSymbols->pNext;
   } while( pLastSymbols );
}

void DoExitFunctions( void )
{
   PSYMBOLS pLastSymbols = pSymbols;
   WORD w;
   SYMBOLSCOPE scope;

   do {
      if( pLastSymbols->hScope & FS_EXIT )
      {  /* only if module contains some EXIT functions */
         for( w = 0; w < pLastSymbols->wModuleSymbols; w++ )
         {
            scope =( pLastSymbols->pModuleSymbols + w )->cScope & (FS_EXIT | FS_INIT);
            if( scope == FS_EXIT )
            {
               PushSymbol( pLastSymbols->pModuleSymbols + w );
               PushNil();
               Do( 0 );
            }
         }
      }
      pLastSymbols = pLastSymbols->pNext;
   } while( pLastSymbols );
}

void DoInitFunctions( int argc, char * argv[] )
{
   PSYMBOLS pLastSymbols = pSymbols;
   WORD w;
   SYMBOLSCOPE scope;

   do {
      if( pLastSymbols->hScope & FS_INIT )
      {  /* only if module contains some INIT functions */
         for( w = 0; w < pLastSymbols->wModuleSymbols; w++ )
         {
            scope =( pLastSymbols->pModuleSymbols + w )->cScope & (FS_EXIT | FS_INIT);
            if( scope == FS_INIT )
            {
               int i;

               PushSymbol( pLastSymbols->pModuleSymbols + w );
               PushNil();

               for( i = 1; i < argc; i++ ) /* places application parameters on the stack */
                  PushString( argv[ i ], strlen( argv[ i ] ) );

               Do( argc - 1 );
            }
         }
      }
      pLastSymbols = pLastSymbols->pNext;
   } while( pLastSymbols );
}

/* ----------------------------- */
/* TODO: Put these to /source/rtl/?.c */

HARBOUR HB_LEN( void )
{
   PHB_ITEM pItem;

   if( hb_pcount() )
   {
      pItem = hb_param( 1, IT_ANY );

      switch( pItem->type )
      {
         case IT_ARRAY:
              hb_retnl( pItem->item.asArray.value->ulLen );
              break;

         case IT_STRING:
              hb_retnl( pItem->item.asString.length );
              break;

         default:
              hb_errorRT_BASE(EG_ARG, 1111, NULL, "LEN");
              break;
      }
   }
   else
      hb_retni( 0 );  /* QUESTION: Should we raise an error here ? */
}

HARBOUR HB_EMPTY(void)
{
   PHB_ITEM pItem = hb_param( 1, IT_ANY );

   if( pItem )
   {
      switch( pItem->type & ~IT_BYREF )
      {
         case IT_ARRAY:
              hb_retl( pItem->item.asArray.value->ulLen == 0 );
              break;

         case IT_STRING:
              hb_retl( hb_strempty( hb_parc( 1 ), hb_parclen( 1 ) ) );
              break;

         case IT_INTEGER:
              hb_retl( ! hb_parni( 1 ) );
              break;

         case IT_LONG:
              hb_retl( ! hb_parnl( 1 ) );
              break;

         case IT_DOUBLE:
              hb_retl( ! hb_parnd( 1 ) );
              break;

         case IT_DATE:
              hb_retl( atol( hb_pards( 1 ) ) == 0 );  /* Convert to long */
              break;

         case IT_LOGICAL:
              hb_retl( ! hb_parl( 1 ) );
              break;

         case IT_BLOCK:
              hb_retl( FALSE );
              break;

         default:
              hb_retl( TRUE );
              break;
      }
   }
   else
      hb_retl( TRUE );
}

HARBOUR HB_VALTYPE( void )
{
   PHB_ITEM pItem;

   if( hb_pcount() )
   {
      pItem = hb_param( 1, IT_ANY );

      switch( pItem->type & ~IT_BYREF )
      {
         case IT_ARRAY:
              if( pItem->item.asArray.value->wClass )
                 hb_retc( "O" );  /* it is an object */
              else
                 hb_retc( "A" );
              break;

         case IT_BLOCK:
              hb_retc( "B" );
              break;

         case IT_DATE:
              hb_retc( "D" );
              break;

         case IT_LOGICAL:
              hb_retc( "L" );
              break;

         case IT_INTEGER:
         case IT_LONG:
         case IT_DOUBLE:
              hb_retc( "N" );
              break;

         case IT_STRING:
              hb_retc( "C" );
              break;

         case IT_NIL:
         default:
              hb_retc( "U" );
              break;
      }
   }
   else
      hb_retc( "U" );
}

HARBOUR HB_ERRORBLOCK(void)
{
   HB_ITEM oldError;
   PHB_ITEM pNewErrorBlock = hb_param( 1, IT_BLOCK );

   oldError.type = IT_NIL;
   hb_itemCopy( &oldError, &errorBlock );

   if( pNewErrorBlock )
      hb_itemCopy( &errorBlock, pNewErrorBlock );

   hb_itemCopy( &stack.Return, &oldError );
   hb_itemClear( &oldError );
}

HARBOUR HB_PROCNAME(void)
{
   int iLevel = hb_parni( 1 ) + 1;  /* we are already inside ProcName() */
   PHB_ITEM pBase = stack.pBase;

   while( ( iLevel-- > 0 ) && pBase != stack.pItems )
      pBase = stack.pItems + pBase->item.asSymbol.stackbase;

   if( ( iLevel == -1 ) )
      hb_retc( pBase->item.asSymbol.value->szName );
   else
      hb_retc( "" );
}

HARBOUR HB_PROCLINE(void)
{
   int iLevel  = hb_parni( 1 ) + 1;  /* we are already inside ProcName() */
   PHB_ITEM pBase = stack.pBase;

   while( ( iLevel-- > 0 ) && pBase != stack.pItems )
      pBase = stack.pItems + pBase->item.asSymbol.stackbase;

   if( iLevel == -1 )
      hb_retni( pBase->item.asSymbol.lineno );
   else
      hb_retni( 0 );
}

HARBOUR HB___QUIT(void)
{
   bQuit = TRUE;
}

HARBOUR HB_ERRORLEVEL(void)
{
   BYTE bPrevValue = bErrorLevel;

   if( hb_pcount() > 0 )
      /* Only replace the error level if a parameter was passed */
      bErrorLevel = hb_parni( 1 );
   hb_retni( bPrevValue );
}

HARBOUR HB_PCOUNT(void)
{
   PHB_ITEM pBase = stack.pItems + stack.pBase->item.asSymbol.stackbase;
   WORD  wRet  = pBase->item.asSymbol.paramcnt;                /* Skip current function     */

   hb_retni( wRet );
}

HARBOUR HB_PVALUE(void)                                /* PValue( <nArg> )         */
{
   WORD  wParam = hb_parni( 1 );                  /* Get parameter            */
   PHB_ITEM pBase = stack.pItems + stack.pBase->item.asSymbol.stackbase;
                                                /* Skip function + self     */

   if( wParam && wParam <= pBase->item.asSymbol.paramcnt )     /* Valid number             */
      hb_itemReturn( pBase + 1 + wParam );
   else
   {
      hb_errorRT_BASE(EG_ARG, 3011, NULL, "PVALUE");
   }
}
