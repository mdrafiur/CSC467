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

enum {
  DP3 = 0, 
  LIT = 1, 
  RSQ = 2
};

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

// defines the yyval union
%union {
  int as_int;
  int as_vec;
  float as_float;
  char *as_str;
  int as_func;
}

%token          FLOAT_T
%token          INT_T
%token          BOOL_T
%token          CONST
%token          FALSE_C TRUE_C
%token          FUNC
%token          WHILE IF
%token          AND OR NEQ EQ LEQ GEQ

// links specific values of tokens to yyval
%token <as_vec>   VEC_T
%token <as_vec>   BVEC_T
%token <as_vec>   IVEC_T
%token <as_float> FLOAT_C
%token <as_int>   INT_C
%token <as_str>   ID

//this is to solve the dangling if-else problem
//the operators are defined in an order in which the one with lowest precedence gets defined first
%right    IFX ELSE
%left     OR                        
%left     AND                       
%left     EQ NEQ '<' LEQ '>' GEQ    
%left     '+' '-'                   
%left     '*' '/'                   
%right    '^'                       
%right    '!' UMINUS                
%left     '(' '['
                 

%start    program

%%

/***********************************************************************
 *  Yacc/Bison rules
 *  Phase 2:
 *    1. Replace grammar found here with something reflecting the source
 *       language grammar
 *    2. Implement the trace parser option of the compiler
 ***********************************************************************/
program
  : scope 
      {yTRACE("program -> scope\n")} 
  ;
scope
  : '{' declarations statements '}'
     {yTRACE("scope -> { declarations statements }\n")}
  ;
declarations
  : declarations declaration
      {yTRACE("declarations -> declarations declaration\n")}
  | 
      {yTRACE("declarations -> empty \n")}
  ;

statements
  : statements statement
      {yTRACE("statements -> statements statement\n")}
  | 
      {yTRACE("statements -> empty \n")}
  ;

/*removed the epsilon to avoid conflicts*/
declaration
  : type ID ';' 
      {yTRACE("declaration -> type ID ;\n")}
  | type ID '=' expression ';'
      {yTRACE("declaration -> type ID = expression ;\n")}
  | CONST type ID '=' expression ';'
      {yTRACE("declaration -> CONST type ID = expression ;\n")}  
  ;

statement
  : variable '=' expression ';'
      {yTRACE("statement -> variable = expression ;\n")}
  | IF '(' expression ')' statement ELSE statement
	{yTRACE("statement -> IF ( expression ) statement ELSE statement \n")}
  | IF '(' expression ')' statement %prec IFX
      {yTRACE("statement -> IF ( expression ) statement \n")}
  
  | WHILE '(' expression ')' statement  
      {yTRACE("statement -> WHILE ( expression ) statement \n")}
  | scope 
      {yTRACE("statement -> scope \n")}
  | ';'
      {yTRACE("statement -> ; \n")}
  ;

type
  : INT_T 
      {yTRACE ("type -> INT_T \n")}
  | IVEC_T 
      {yTRACE ("type -> IVEC_T \n")}
  | BOOL_T 
      {yTRACE ("type -> BOOL_T \n")}
  | BVEC_T 
      {yTRACE ("type -> BVEC_T \n")}
  | FLOAT_T 
      {yTRACE ("type -> FLOAT_T \n")}
  | VEC_T 
      {yTRACE ("type -> VEC_T \n")}
  ;
  

expression
  : constructor 
      {yTRACE ("expression -> constructor \n")}
  | function 
      {yTRACE ("expression -> function \n")}
  | INT_C 
      {yTRACE ("expression -> INT_C \n")}
  | FLOAT_C 
      {yTRACE ("expression -> FLOAT_C \n")}
  | variable 
      {yTRACE ("expression -> variable \n")}
/*use %prec to define unary minus and solve conflicts between "expression binary_opt expression" and "unary_opt expression*/
  | '-' expression %prec UMINUS 
     {yTRACE ("expression -> unary_opt UMINUS expression \n")}
  | '!' expression 
      {yTRACE ("expression -> unary_opt ! expression \n")}    
  | expression AND expression 
      {yTRACE("expression -> expression binary_opt AND expression \n")}
  | expression OR expression 
      {yTRACE("expression -> expression binary_opt OR expression \n")}
  | expression EQ expression 
      {yTRACE("expression -> expression binary_opt EQ expression \n")}
  | expression NEQ expression 
      {yTRACE("expression -> expression binary_opt NEQ expression \n")}
  | expression '<' expression 
      {yTRACE("expression -> expression binary_opt < expression \n")}
  | expression LEQ expression 
      {yTRACE("expression -> expression binary_opt LEQ expression \n")}
  | expression '>' expression 
      {yTRACE("expression -> expression binary_opt > expression \n")}
  | expression GEQ expression 
      {yTRACE("expression -> expression binary_opt GEQ expression \n")}
  | expression '+' expression 
      {yTRACE("expression -> expression binary_opt + expression \n")}
  | expression '-' expression 
      {yTRACE("expression -> expression binary_opt - expression \n")}
  | expression '*' expression 
      {yTRACE("expression -> expression binary_opt * expression \n")}
  | expression '/' expression 
      {yTRACE("expression -> expression binary_opt / expression \n")}
  | expression '^' expression 
      {yTRACE("expression -> expression binary_opt ^ expression \n")}
  | TRUE_C
      {yTRACE("expression -> TRUE_C \n")}
  | FALSE_C
      {yTRACE("expression -> FALSE_C \n")}
  | '(' expression ')'
      {yTRACE("expression -> ( expression ) \n")}
  ;
  
variable
  : ID 
      {yTRACE ("variable -> ID \n")}
  | ID '[' INT_C ']' 
      {yTRACE ("variable -> ID [ INT_C ] \n")}
  ;
 

constructor
  : type '(' arguments_opt ')' 
      {yTRACE ("constructor -> type ( arguments_opt ) \n")}
  ;
	
function
  : function_name '(' arguments_opt ')' 
      {yTRACE ("function -> function_name ( arguments_opt ) \n")}
  ;
  
function_name
  : FUNC 
      {yTRACE ("function_name -> FUNC \n")}
  ;
  
arguments_opt
  : arguments 
      {yTRACE ("arguments_opt -> arguments \n")}
  | 
      { yTRACE ("arguments -> empty \n")}
  ;
  
arguments
  : arguments ',' expression 
      {yTRACE ("arguments -> arguments , expression \n")}
  | expression 
      {yTRACE ("arguments -> expression \n")}
  ;

%%

/***********************************************************************ol
 * Extra C code.
 *
 * The given yyerror function should not be touched. You may add helper
 * functions as necessary in subsequent phases.
 ***********************************************************************/
void yyerror(char* s) {
  if(errorOccurred) {
    return;    /* Error has already been reported by scanner */
  } else {
    errorOccurred = 1;
  }

  fprintf(errorFile, "\nPARSER ERROR, LINE %d", yyline);
  
  if(strcmp(s, "parse error")) {
    if(strncmp(s, "parse error, ", 13)) {
      fprintf(errorFile, ": %s\n", s);
    } else {
      fprintf(errorFile, ": %s\n", s+13);
    }
  } else {
    fprintf(errorFile, ": Reading token %s\n", yytname[YYTRANSLATE(yychar)]);
  }
}
