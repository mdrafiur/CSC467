%{

/***********************************************************************

 * --YOUR GROUP INFO SHOULD GO HERE--

 * 

 *   Interface to the parser module for CSC467 course project.

 * 

 *   Phase 2: Implement context free grammar for source language, and

 *            parse tracing functionality.

 *   Phase 3: Construct the AST for the source language program.

 ***********************************************************************/

/***********************************************************************

 *  C Definitions and external declarations for this module.

 *

 *  Phase 3: Include ast.h if needed, and declarations for other global or

 *           external vars, functions etc. as needed.

 ***********************************************************************/

#include <string.h>

#include "common.h"

//#include "ast.h"

//#include "symbol.h"

//#include "semantic.h"

#define YYERROR_VERBOSE

#define yTRACE(x)    { if (traceParser) fprintf(traceFile, "%s\n", x); }

void yyerror(char* s);    /* what to do in case of error            */

int yylex();              /* procedure for calling lexical analyzer */

extern int yyline;        /* variable holding current line number   */

%}



/***********************************************************************

 *  Yacc/Bison declarations.

 *  Phase 2:

 *    1. Add precedence declarations for operators (after %start declaration)

 *    2. If necessary, add %type declarations for some nonterminals

 *  Phase 3:

 *    1. Add fields to the union below to facilitate the construction of the

 *       AST (the two existing fields allow the lexical analyzer to pass back

 *       semantic info, so they shouldn't be touched).

 *    2. Add <type> modifiers to appropriate %token declarations (using the

 *       fields of the union) so that semantic information can by passed back

 *       by the scanner.

 *    3. Make the %type declarations for the language non-terminals, utilizing

 *       the fields of the union as well.

 ***********************************************************************/



%{

#define YYDEBUG 1

%}





// TODO:Modify me to add more data types

// Can access me from flex useing yyval



%union {

  int intNum;

  float floatNum;

  char *var;

  char *func_lit;

  char *func_dp3;

  char *func_rsq;

}

// TODO:Replace myToken with your tokens, you can use these tokens in flex

%token          TYPE_FLOAT TYPE_INT TYPE_BOOL TYPE_VEC2 TYPE_VEC3 TYPE_VEC4 TYPE_BVEC2 TYPE_BVEC3 TYPE_BVEC4 TYPE_IVEC2 TYPE_IVEC3 TYPE_IVEC4

%token          LPAREN RPAREN LSPAREN RSPAREN LCPAREN RCPAREN

%token          CONST SEMI COMMA NEWLINE

%token          PLUS MINUS MULT DIVIDE SQUARE

%token          FALSE TRUE

%token          FUNCNAME

%token          IF WHILE ELSE

%token          AND OR NOT NEQL EQL LSS LEQ GTR GEQ

%token          INT FLOAT IDENTIFIER



%start    program



%%



/***********************************************************************

 *  Yacc/Bison rules

 *  Phase 2:

 *    1. Replace grammar found here with something reflecting the source

 *       language grammar

 *    2. Implement the trace parser option of the compiler

 *  Phase 3:

 *    1. Add code to rules for construction of AST.

 ***********************************************************************/

program 



  :   tokens       

  ;

tokens

  :tokens token  

  |      

  ;

// TODO: replace myToken with the token the you defined.

token

  :     TYPE_FLOAT  {fprintf(outputFile, "TYPE_FLOAT ");}

  |     TYPE_INT {fprintf(outputFile, "TYPE_INT ");}

  |     TYPE_BOOL  {fprintf(outputFile, "TYPE_BOOL ");}

  |     TYPE_VEC2   {fprintf(outputFile, "TYPE_VEC2 ");}

  |     TYPE_VEC3 {fprintf(outputFile, "TYPE_VEC3 ");}

  |     TYPE_VEC4 {fprintf(outputFile, "TYPE_VEC4 ");}	

  |     TYPE_BVEC2{fprintf(outputFile, "TYPE_BVEC2 ");}

  |     TYPE_BVEC3{fprintf(outputFile, "TYPE_BVEC3 ");}

  |     TYPE_BVEC4{fprintf(outputFile, "TYPE_BVEC4 ");}

  |     TYPE_IVEC2{fprintf(outputFile, "TYPE_IVEC2 ");}

  |     TYPE_IVEC3{fprintf(outputFile, "TYPE_IVEC3 ");}

  |     TYPE_IVEC4{fprintf(outputFile, "TYPE_IVEC4 ");}

  |     LPAREN	{fprintf(outputFile, "LPAREN ");}

  |     RPAREN	{fprintf(outputFile, "RPAREN ");}

  |     LSPAREN{fprintf(outputFile, "LSPAREN ");}

  |     IF 	{fprintf(outputFile, "IF ");}

  |     ELSE 	{fprintf(outputFile, "ELSE ");}

  |     WHILE	{fprintf(outputFile, "WHILE ");}

  |     RSPAREN	{fprintf(outputFile, "RSPAREN ");}

  |     LCPAREN {fprintf(outputFile, "LCPAREN ");}

  |     RCPAREN {fprintf(outputFile, "RCPAREN ");}

  |	CONST	{fprintf(outputFile, "CONST ");}

  |	SEMI	{fprintf(outputFile, "SEMI ");}

  |	COMMA	{fprintf(outputFile, "COMMA ");}

  |	NEWLINE	{fprintf(outputFile, "\n ");}

  |     MULT	{fprintf(outputFile, "MULT ");}

  |     DIVIDE	{fprintf(outputFile, "DIVIDE ");}

  |     PLUS 	{fprintf(outputFile, "PLUS ");}

  |     MINUS 	{fprintf(outputFile, "MINUS ");}

  |     SQUARE 	{fprintf(outputFile, "SQUARE ");}

  |     FALSE 	{fprintf(outputFile, "FALSE ");}

  |     TRUE 	{fprintf(outputFile, "TRUE ");}

  |     FUNCNAME{fprintf(outputFile, "FUNCNAME ");}

  |     AND 	{fprintf(outputFile, "AND ");}

  |     OR 	{fprintf(outputFile, "OR ");}

  |     EQL 	{fprintf(outputFile, "EQL ");}

  |     NEQL 	{fprintf(outputFile, "NEQL ");}

  |     GTR 	{fprintf(outputFile, "GTR ");}

  |     GEQ 	{fprintf(outputFile, "GEQ ");}

  |     LSS 	{fprintf(outputFile, "LSS ");}

  |     LEQ	{fprintf(outputFile, "LEQ ");}

  |     SQUARE 	{fprintf(outputFile, "TYPE_VEC3 ");}

  |     NOT 	{fprintf(outputFile, "NOT ");}

  |     INT 	{fprintf(outputFile, "INT ");}

  |     FLOAT 	{fprintf(outputFile, "FLOAT ");}

  |     IDENTIFIER {fprintf(outputFile, "IDENTIFIER ");}

  ;





%%

/***********************************************************************ol

 * Extra C code.

 *

 * The given yyerror function should not be touched. You may add helper

 * functions as necessary in subsequent phases.

 ***********************************************************************/

void yyerror(char* s) {

  if (errorOccurred)

    return;    /* Error has already been reported by scanner */

  else

    errorOccurred = 1;

        

  fprintf(errorFile, "\nPARSER ERROR, LINE %d",yyline);

  if (strcmp(s, "parse error")) {

    if (strncmp(s, "parse error, ", 13))

      fprintf(errorFile, ": %s\n", s);

    else

      fprintf(errorFile, ": %s\n", s+13);

  } else

    fprintf(errorFile, ": Reading token %s\n", yytname[YYTRANSLATE(yychar)]);

}
