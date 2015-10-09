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
"*"         {yTRACE((TIMES));     return(TIMES);}        
"/"         {yTRACE((DIVIDE));     return(DIVIDE);}
"^"         {yTRACE((SQUARE));     return(SQUARE);}

"true"      {yTRACE((BOOL)); yylval.string = strdup(yytext); return(BOOL);}
"false"     {yTRACE((BOOL)); yylval.string = strdup(yytext); return(BOOL);}

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

"int"       {yTRACE((TYPE)); yylval.string = strdup(yytext);      return(TYPE);}
"bool"      {yTRACE((TYPE)); yylval.string = strdup(yytext);      return(TYPE);}
"float"     {yTRACE((TYPE)); yylval.string = strdup(yytext);      return(TYPE);}

"vec2"      {yTRACE((TYPE)); yylval.string = strdup(yytext);      return(TYPE);}
"vec3"      {yTRACE((TYPE)); yylval.string = strdup(yytext);      return(TYPE);}
"vec4"      {yTRACE((TYPE)); yylval.string = strdup(yytext);      return(TYPE);}
"bvec2"     {yTRACE((TYPE)); yylval.string = strdup(yytext);      return(TYPE);}
"bvec3"     {yTRACE((TYPE)); yylval.string = strdup(yytext);      return(TYPE);}
"bvec4"     {yTRACE((TYPE)); yylval.string = strdup(yytext);      return(TYPE);}
"ivec2"     {yTRACE((TYPE)); yylval.string = strdup(yytext);      return(TYPE);}
"ivec3"     {yTRACE((TYPE)); yylval.string = strdup(yytext);      return(TYPE);}
"ivec4"     {yTRACE((TYPE)); yylval.string = strdup(yytext);      return(TYPE);}

"lit"       {yTRACE((FUNCNAME)); yylval.string = strdup(yytext);  return(FUNCNAME);}
"dp3"       {yTRACE((FUNCNAME)); yylval.string = strdup(yytext);  return(FUNCNAME);}
"rsq"       {yTRACE((FUNCNAME)); yylval.string = strdup(yytext);  return(FUNCNAME);}

"if"        {yTRACE((IF));       return(IF);}       
"else"      {yTRACE((ELSE));     return(ELSE);}
"while"     {yTRACE((WHILE));    return(WHILE);}


([0-9]*.[0-9]*)  {yTRACE((FLOAT)); yylval.fnum = atof(yytext); return (strcmp(yytext,".")) ? FLOAT : Error; }????????
{DIGIT}+         {yTRACE((INT));   yylval.num = atoi(yytext);  return(INT);}
{LETTER}({LETTER}|{DIGIT})*+"_"({LETTER}|{DIGIT})+    {yTRACE((VARN)); yylval.string = strdup(yytext); return(VARN);}






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

