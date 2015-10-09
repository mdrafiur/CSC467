%{
/**********************************************************************
 *
 * **YOUR GROUP INFO SHOULD GO HERE**
 *
 *  Interface to the scanner module for CSC467 course project
 **********************************************************************/


#include "common.h"
#include "parser.tab.h"
#include "string.h"


#define YY_USER_INIT { yyin = inputFile; }
#define	yyinput      input
#define yTRACE(x)    { if (traceScanner) fprintf(traceFile, "TOKEN %3d : %s\n", x, yytext); }
#define yERROR(x)    { fprintf(errorFile, "\nLEXICAL ERROR, LINE %d: %s\n", yyline, x); errorOccurred = TRUE; yyterminate(); }

int CheckInt(void);
int yyline = 1;

%}
%option noyywrap

WS          [ \t]
LETTER      [A-Za-z]
DIGIT       [0-9]

%%
"+"         {yTRACE((PLUS));     return(PLUS);}
"-"         {yTRACE((MINUS));     return(MINUS);}
"*"         {yTRACE((TIMES));     return(TIMES);}        
"/"         {yTRACE((DIVIDE));     return(DIVIDE);}
"^"         {yTRACE((SQUARE));     return(SQUARE);}

"true"      {yTRACE((TRUE));     return(TRUE);}
"false"     {yTRACE((FALSE));     return(FALSE);}

"!"         {yTRACE((NOT));     return(NOT);}
"&&"        {yTRACE((AND));     return(AND);}
"||"        {yTRACE((OR));     return(OR);}
"=="        {yTRACE((EQL));     return(EQL);}
"!="        {yTRACE((NEQL));     return(NEQL);}
"<"         {yTRACE((LSS));     return(LSS);}
"<="        {yTRACE((LEQ));     return(LEQ);}
">"         {yTRACE((GTR));     return(GTR);}
">="        {yTRACE((GEQ));     return(GEQ);}

"("         {yTRACE((LPAREN));     return(LPAREN);}
")"         {yTRACE((RPAREN));     return(RPAREN);}
"["         {yTRACE((LSPAREN));     return(LSPAREN);}
"]"         {yTRACE((RSPAREN));     return(RSPAREN);}
"{"         {yTRACE((LCPAREN));     return(LCPAREN);}
"}"         {yTRACE((RCPAREN));     return(RCPAREN);}

"const"     {yTRACE((CONST));     return(CONST);}
";"         {yTRACE((SEMI));     return(SEMI);}
"/*"        {yTRACE((LCMET));     return(LCMET);}
"*/"        {yTRACE((RCMET));     return(RCMET);}

"int"       {yTRACE((INT));      return(INT);}
"bool"      {yTRACE((BOOL));     return(BOOL);}
"float"     {yTRACE((FLOAT));    return(FLOAT);}
"vec2"      {yTRACE((VEC2));     return(VEC2);}
"vec3"      {yTRACE((VEC3));     return(VEC3);}
"vec4"      {yTRACE((VEC4));     return(VEC4);}
"bvec2"     {yTRACE((BVEC2));    return(BVEC2);}
"bvec3"     {yTRACE((BVEC3));    return(BVEC3);}
"bvec4"     {yTRACE((BVEC4));    return(BVEC4);}
"ivec2"     {yTRACE((IVEC2));    return(IVEC2);}
"ivec3"     {yTRACE((IVEC3));    return(IVEC3);}
"ivec4"     {yTRACE((IVEC4));    return(IVEC4);}

"lit"       {yTRACE((LIT));      return(LIT);}
"dp3"       {yTRACE((DP3));      return(DP3);}
"rsq"       {yTRACE((RSQ));      return(RSQ);}

"if"        {yTRACE((IF));       return(IF);}       
"else"      {yTRACE((ELSE));     return(ELSE);}
"while"     {yTRACE((WHILE));    return(WHILE);}



%%
"/*"        {
              int c1      = 0;
              int c2      = yyinput();
              int curline = yyline;
              for(;;) {
                if (c2 == EOF) {
                  fprintf(errorFile, "\nLEXICAL ERROR, LINE %d: Unmatched /*\n", curline);
                  errorOccurred = TRUE;
                  yyterminate();
                }
                if (c1 == '*' && c2 == '/')
                  break;
                c1 = c2;
                c2 = yyinput();
                if (c1 == '\n' && c2 != EOF)
                  yyline++;
              }
            }

.           { yERROR("Unknown token"); }

%%

