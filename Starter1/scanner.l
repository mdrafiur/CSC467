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
%x F_COMMENT

WS          [ \t]
LETTER      [a-zA-Z]
DIGIT       [0-9]
SIGN	    [-+]
INT	    {SIGN}?{DIGIT}+
FLOAT       {SIGN}?{DIGIT}*\.{DIGIT}*
IDENTIFIER  [a-zA-Z_][a-zA-Z0-9_]*
BOOL        "true"|"false"

%%
<INITIAL>"/*"                   {BEGIN(F_COMMENT);}
<F_COMMENT>"*/"                 {BEGIN(INITIAL); } 
<F_COMMENT>[.\n]+               { }

"\n"    { yyline++; yTRACE(NEWLINE); return NEWLINE;}
"+"         {yTRACE(PLUS);     return PLUS;} 
"-"         {yTRACE(MINUS);    return MINUS;} 
"*"         {yTRACE(MULT);     return MULT;}        
"/"         {yTRACE(DIVIDE);   return DIVIDE;}
"^"         {yTRACE(SQUARE);   return SQUARE;}

"true"      {yTRACE(BOOL);     return BOOL;}
"false"     {yTRACE(BOOL);     return BOOL;}

"!"         {yTRACE(NOT);      return NOT;}
"&&"        {yTRACE(AND);      return AND;}
"||"        {yTRACE(OR);       return OR;}
"=="        {yTRACE(EQL);      return EQL;}
"!="        {yTRACE(NEQL);     return NEQL;}
"<"         {yTRACE(LSS);      return LSS;}
"<="        {yTRACE(LEQ);      return LEQ;}
">"         {yTRACE(GTR);      return GTR;}
">="        {yTRACE(GEQ);      return GEQ;}

"("         {yTRACE(LPAREN);   return LPAREN;}
")"         {yTRACE(RPAREN);   return RPAREN;}
"["         {yTRACE(LSPAREN);  return LSPAREN;}
"]"         {yTRACE(RSPAREN);  return RSPAREN;}
"{"         {yTRACE(LCPAREN);  return LCPAREN;}
"}"         {yTRACE(RCPAREN);  return RCPAREN;}

"const"     {yTRACE(CONST);    return CONST;}
";"         {yTRACE(SEMI);     return SEMI;}
","         {yTRACE(COMMA);    return COMMA;}

"int"       {yTRACE(TYPE);     return TYPE;}
"bool"      {yTRACE(TYPE);     return TYPE;}
"float"     {yTRACE(TYPE);     return TYPE;}
"vec2"      {yTRACE(TYPE);     return TYPE;}
"vec3"      {yTRACE(TYPE);     return TYPE;}
"vec4"      {yTRACE(TYPE);     return TYPE;}
"bvec2"     {yTRACE(TYPE);     return TYPE;}
"bvec3"     {yTRACE(TYPE);     return TYPE;}
"bvec4"     {yTRACE(TYPE);     return TYPE;}
"ivec2"     {yTRACE(TYPE);     return TYPE;}
"ivec3"     {yTRACE(TYPE);     return TYPE;}
"ivec4"     {yTRACE(TYPE);     return TYPE;}

"lit"       {yTRACE(FUNCNAME); return FUNCNAME;}
"dp3"       {yTRACE(FUNCNAME); return FUNCNAME;}
"rsq"       {yTRACE(FUNCNAME); return FUNCNAME;}

"if"        {yTRACE(IF);       return IF;}       
"else"      {yTRACE(ELSE);     return ELSE;}
"while"     {yTRACE(WHILE);    return WHILE;}

{INT} {
    Ival = atoi(yytext);	
    if(Ival < INT_MIN || Ival > INT_MAX)
        yERROR("Error: Integer Overflow or Underflow");
  
    else {
        yTRACE(INT);
        return INT;
    }
}

{FLOAT} {
    Fval = atoi(yytext);	
    if(Fval < FLT_MIN || Fval > FLT_MAX)
        yERROR("Error: Floating point Overflow or Underflow");
  
    else {
        yTRACE(FLOAT);
        return FLOAT;
    }
}
{IDENTIFIER} {
    if (strlen(yytext) > MAX_IDENTIFIER)
        yERROR("Maximum Identifier length exceeded.");
   
    else {
        yTRACE(IDENTIFIER);
	return IDENTIFIER;
    }
}


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

