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
DIGIT       [0-9]
IDENTIFIER  [a-zA-Z_][a-zA-Z0-9_]*

%%
<INITIAL>"/*"                   {BEGIN(F_COMMENT);}
<F_COMMENT>"*/"                 {BEGIN(INITIAL); } 
<F_COMMENT>[.\n]+               { }

"\n"        {yyline++;  yTRACE(NEWLINE);  return NEWLINE;}
"+"         {yTRACE(PLUS);    return PLUS;} 
"-"         {yTRACE(MINUS);   return MINUS;} 
"*"         {yTRACE(MULT);    return MULT;}        
"/"         {yTRACE(DIVIDE);  return DIVIDE;}
"^"         {yTRACE(EXP);  return EXP;}

"true"      {yTRACE(BOOL_TRUE);    return TRUE;}
"false"     {yTRACE(BOOL_FALSE);   return FALSE;}

"!"         {yTRACE(NOT);    return NOT;}
"&&"        {yTRACE(AND);    return AND;}
"||"        {yTRACE(OR);     return OR;}
"=="        {yTRACE(EQL);    return EQL;}
"!="        {yTRACE(NEQL);   return NEQL;}
"<"         {yTRACE(LSS);    return LSS;}
"<="        {yTRACE(LEQ);    return LEQ;}
">"         {yTRACE(GTR);    return GTR;}
">="        {yTRACE(GEQ);    return GEQ;}

"("         {yTRACE(LPAREN);   return LPAREN;}
")"         {yTRACE(RPAREN);   return RPAREN;}
"["         {yTRACE(LSPAREN);  return LSPAREN;}
"]"         {yTRACE(RSPAREN);  return RSPAREN;}
"{"         {yTRACE(LCPAREN);  return LCPAREN;}
"}"         {yTRACE(RCPAREN);  return RCPAREN;}

"const"     {yTRACE(CONST);    return CONST;}
";"         {yTRACE(SEMI);     return SEMI;}
","         {yTRACE(COMMA);    return COMMA;}

"int"       {yTRACE(TYPE_INT);     return TYPE_INT;}
"bool"      {yTRACE(TYPE_BOOL);    return TYPE_BOOL;}
"float"     {yTRACE(TYPE_FLOAT);   return TYPE_FLOAT;}
"vec2"      {yTRACE(TYPE_VEC2);    return TYPE_VEC2;}
"vec3"      {yTRACE(TYPE_VEC3);    return TYPE_VEC3;}
"vec4"      {yTRACE(TYPE_VEC4);    return TYPE_VEC4;}
"bvec2"     {yTRACE(TYPE_BVEC2);   return TYPE_BVEC2;}
"bvec3"     {yTRACE(TYPE_BVEC3);   return TYPE_BVEC3;}
"bvec4"     {yTRACE(TYPE_BVEC4);   return TYPE_BVEC4;}
"ivec2"     {yTRACE(TYPE_IVEC2);   return TYPE_IVEC2;}
"ivec3"     {yTRACE(TYPE_IVEC3);   return TYPE_IVEC3;}
"ivec4"     {yTRACE(TYPE_IVEC4);   return TYPE_IVEC4;}

"lit"       {yylval.func_lit = (char *)malloc((yyleng+1)*(sizeof(char)));
             strcpy(yylval.funcLit, yytext);	
             yTRACE(FUNCNAME); 
             return FUNCNAME;}
"dp3"       {yylval.func_dp3 = (char *)malloc((yyleng+1)*(sizeof(char)));
             strcpy(yylval.func_dp3, yytext); 
             yTRACE(FUNCNAME); 
             return FUNCNAME;}
"rsq"       {yylval.func_rsq = (char *)malloc((yyleng+1)*(sizeof(char)));
             strcpy(yylval.func_rsq, yytext); 
             yTRACE(FUNCNAME); 
             return FUNCNAME;}

"if"        {yTRACE(IF);       return IF;}       
"else"      {yTRACE(ELSE);     return ELSE;}
"while"     {yTRACE(WHILE);    return WHILE;}

0 	    {yylval.intNum = 0;    yTRACE(INT);    return INT;}
[1-9][0-9]* {
    intVal = atoi(yytext);	
    if(intVal < MIN_INT || intVal > MAX_INT) {
        yERROR("Error: Integer Overflow or Underflow");
        yyterminate();
    }
    else {
    	yylval.intNum = intVal;
        yTRACE(INT);
        return INT;
    }
}

\.{DIGIT}+ 	{yylval.floatNum = floatVal; yTRACE(FLOAT); return FLOAT;}

{DIGIT}+\.{DIGIT}* {
    floatVal = atoi(yytext);
    if(floatVal < FLT_MIN || floatVal > FLT_MAX) {
        yERROR("Error: Floating point Overflow or Underflow");
  	yyterminate();
    }
    else {
    	yylval.floatNum = floatVal;
        yTRACE(FLOAT);
        return FLOAT;
    }
}

0{DIGIT}+  {yERROR("Error: Octal numbers not supported.");  yyterminate();}

{IDENTIFIER} {
    if (strlen(yytext) > MAX_IDENTIFIER) {
        yERROR("Maximum Identifier length exceeded.");
   	yyterminate();
    }
    else {
    	yylval.var = (char *)malloc((yyleng+1)*(sizeof(char)));
        strcpy(yylval.var, yytext);
        yTRACE(IDENTIFIER);
	return IDENTIFIER;
    }
}

.           {yERROR("Unknown token"); yyterminate();}

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

%%
