/*
 * $Id$
*/
//DO NOT RUN THIS PROGRAM - ITS PURPOSE IS THE SYNTAX CHECK ONLY!

STATIC nExt, bEgin, bReak, cAse, do, wHile, wIth

Function Main()

//just to prevent any disaster if someone will want to run it
  IF( .T. )
    RETURN nil
  ENDIF

//
//DANGER!!
//One line comments that ends with '**/' doesn't work!!!
//
//

//   NEXT(nExt)	//in Clipper: NEXT does not match FOR
   BEGIN( bEgin +';' + ;;   //////Clipper doesn't like more then one ';'
          ";" ; /* ;;;;;Clipper doesn't like this comment after ';' ;;;;;;; */
        )
   BREAK_(bReak)
//   CASE(case)	//it is not possible to call case() in this context
   case :=CASE( CASE )	//this is valid

   //DO is reserved function name in Clipper!
   DO_( nExt+bEgin/bReak-do*wHile%cAse $ wIth )
//   WHILE( while ) //it is not possible to call while() in this context

RETURN nil

/*================================================================
* * * * * ** Checking for NEXT
*/
FUNCTION NeXT( next_next/*next next*/ )
Local nExt, nExt7, nExtNEXT

    For NExt := 1 To 10

       OutStd( nExT ) // Actually this needs to use str()

   Next /*next*/ nExt     //next

   FOR nExt = 1 TO nExt
     QOut( nExt )
   NEXT

   FOR nExt :=1 TO 10
     QOut( nExt )
   NEXT ; ////////// ;
   NEXT

   nExt := 10 //next

   nExt++	/*in Clipper: NEXT does not match FOR*/
   --nExt	/*next*/
   nExt7 :=7
   nExtNEXT := nExt	//next

   nExt[ 1 ] :=nExt	//in Clipper: NEXT does not match FOR
   nExt[ nExt ] := NEXT( nExt[nExt+nExt] * nExt *(nExt+nExt*5) )

   next->next :=next->next + next->next

  IF( nExt > nExt+4 )
    nExt =nExt +nExt * 5
  ELSEIF( nExt > 0 )
    nExt = nExt * 6 + nExt
  ELSE
    nExt +=5
  ENDIF

RETURN( nExt * /*next*/ nExt )

/*===================================================================
* Checking for BEGIN
**/
FUNCTION BEGIN( BEGIN_BEGIN )
LOCAL bEgin
LOCAL xbEgin
LOCAL bEginBEGIN
LOCAL bEgin0, /* BEGIN OF BEGIN */ ;    /* in Clipper: Incomplete statement */
	bEgin1
LOCAL xbEginBEGIN := 100

BEGIN SEQUENCE
    bEgin0 :=0
    FOR bEgin:=1 TO 10
      QOUT( bEgin )
      xbEgin :=bEgin
      bEginBEGIN :=xbEgin * 10
      bEgin0 +=bEginBEGIN
      --xbEginBEGIN
      bEgin++
      --bEgin
    NEXT bEgin

    begin->begin :=begin->begin +; /************************/
	begin->begin
    bEgin :=BEGIN( xbegin +bEgin +bEginBEGIN -bEgin0 * xbEginBEGIN ) +;
            bEgin[ bEgin ]

END SEQUENCE

BEGIN /* BEGIN */ SEQU

    BEGIN_BEGIN :=bEgin//begin

END /* BEGIN */ SEQUENC

RETURN bEgin ^ 2

/*====================================================================
* Test for BREAK and BEGIN/RECOVER sequence
**/
FUNCTION BREAK_( break_break )
LOCAL bReak:=0

  ++bReak
  IF( bReak = 0 )
    Break /*break*/ ( nil )
    BREAK   /* break to beggining */
    Break()	//in Clipper: syntax error: ')'
  ENDIF

  Break /*** break ***/ ;
; //////////////////////////////////////////////////
; //What
; //the
; //comment
; //like
; //it
; //is
; //doing
; //in
; //place
; //like
; //this?
; //Do
; //you
; //use
; //it
; //often?
;///////////////////////////////////////////////////
  ()	//in Clipper: incomplete statement or unbalanced delimiters

begin sequence
  FOR bReak:=1 To 10
    QOut( bReak * bReak )
  NEXT bReak
  break->break :=break->break +break->break
  BREAK
  bReak :=IIF( bReak=1, BREAK(0), BREAK(bReak) )
recover USING bReak
  BREAK( Break( break() ) )
end

BREAK
RETURN bReak[ bReak ]	//in Clipper: syntax error ' BREAK '

/*====================================================================
* Test for CASE/DO CASE
*/
FUNCTION CASE( case_ )
LOCAL case

  FOR case:=1 TO 15
    QUOT( case )
    QOUT( case * ; ////
case )
  NEXT case

  case[ case ] :=case	//in Clipper: Case is not immediatelly within DO CASE
  case[ 2 ] :=2	//in Clipper: the same as above - Harbour compiles both
  case =case + case - case
  case :={|case| case( case )}
  case :={|| case }

  DO /* case */ CASE
  CASE 1
    case =case ** case
  CASE 2+case
    case =case +1
  CASE case++
//    case--	//sorry -Clipper also doesn't compile this line
//    case++	//sorry -Clipper also doesn't compile this line
  CASE ++case
    ++case
  CASE ;
    case( case )
  CASE CASE
  CASE !CASE
  CASE -CASE
  CASE +CASE
/*
*
*
*/
  CASE( CASE )
    case =case != case
  CASE( CASE )	//new CASE or function call? :)
  CASE case->case
    case->case :=case->case +1
  OTHERWISE
    case *=case
  ENDCASE

RETURN case

/*====================================================================
* Test for DO CASE / DO WHILE / DO / WITH
*/
FUNCTION DO_( do )
LOCAL case
LOCAL while
LOCAL with

  do case
  case do
  case case
  endcase

  do/***do***/case/**/
  case do
  endcase

  DO CASE
  ENDCASE

  while .t.
  enddo

  do ;
   while do
   do :=1
  enddo

  do/***do***/while do/***do do do, da da da***/
   do :=do
  enddo

  do :={|do| do}
  do while do
    do()
    do->do :=do()
    do++
    do--
  enddo

  do while while
    ++do
  enddo

  do++
  do +=do
  do->do :=do->do +1
  do[ 1 ] :=do
  do[ do ] :=do[ do ][ do ]

  DO do WITH do
  DO do WITH do()
  DO do

  DO while;
   while
   while :=while()
  ENDDO

  while while
//   while++	//incomplete statement or unbalanced delimiter
//   while--	//incomplete statement or unbalanced delimiter
   --while
   while +=while
   while->while :=while() +while->while
  enddo

  while[ 1 ] :=while 
  while[ while ] :=while[ while ]
  while :={|while| while}
  while while++ < 10
  enddo

  DO /* ****  */;
;
;
 while

  DO while WITH
   do :=with + while
  ENDDO

  do WITH;
;
;

  with()

  DO while WITH while
  DO while WITH while()
  DO while WITH with
  DO do WITH with
  DO do WITH with()

  with++
  ++with
  with +=with
  with->with :=with() +with->with +1
  with :={|with| with}
  with[ with ] :=with[ with ][ with ]
  with[ with ][ with ] =with[ with ][ with ]

  DO with WITH with
  DO with WITH with()
  DO with WITH do
  DO with WITH while
  DO with WITH do()
  DO with WITH while()

RETURN DO


FUNCTION WHILE( while )
RETURN while

FUNCTION With( with )
RETURN with
