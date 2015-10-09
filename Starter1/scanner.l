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
SIGN[-+]
FLOAT       {SIGN}?{DIGIT}*"."{DIGIT}*
IDENTIFIER  {LETTER}*|"_"*({LETTER}|{DIGIT}|"_")*
BOOL        "true"|"false"

%%
<INITIAL>"/*"                   {BEGIN(F_COMMENT);}
<F_COMMENT>"*/"                 {BEGIN(INITIAL); } 
<F_COMMENT>[.\n]+               { }

"+"         {yTRACE(PLUS);     return PLUS;} 
"-"         {yTRACE(MINUS);    return MINUS;} 
"*"         {yTRACE(TIMES);    return TIMES;}        
"/"         {yTRACE(DIVIDE);   return DIVIDE;}
"^"         {yTRACE(SQUARE);   return SQUARE;}

"true"      {yTRACE(BOOL); yylval.boolean = strdup(yytext); return BOOL;}
"false"     {yTRACE(BOOL); yylval.boolean = strdup(yytext); return BOOL;}

"!"         {yTRACE(NOT);  yylval.relop   return NOT;}
"&&"        {yTRACE(AND);  yylval.relop   return AND;}
"||"        {yTRACE(OR);   yylval.relop   return OR;}
"=="        {yTRACE(EQL);  yylval.relop   return EQL;}
"!="        {yTRACE(NEQL); yylval.relop   return NEQL;}
"<"         {yTRACE(LSS);  yylval.relop   return LSS;}
"<="        {yTRACE(LEQ);  yylval.relop   return LEQ;}
">"         {yTRACE(GTR);  yylval.relop   return GTR;}
">="        {yTRACE(GEQ);  yylval.relop   return GEQ;}

"("         {yTRACE(LPAREN);      return LPAREN;}
")"         {yTRACE(RPAREN);      return RPAREN;}
"["         {yTRACE(LSPAREN);     return LSPAREN;}
"]"         {yTRACE(RSPAREN);     return RSPAREN;}
"{"         {yTRACE(LCPAREN);     return LCPAREN;}
"}"         {yTRACE(RCPAREN);     return RCPAREN;}

"const"     {yTRACE(CONST);    return CONST;}
";"         {yTRACE(SEMI);     return SEMI;}

"int"       {yTRACE(TYPE); yylval.type = strdup(yytext);    return TYPE;}
"bool"      {yTRACE(TYPE); yylval.type = strdup(yytext);    return TYPE;}
"float"     {yTRACE(TYPE); yylval.type = strdup(yytext);    return TYPE;}
"vec2"      {yTRACE(TYPE); yylval.type = strdup(yytext);    return TYPE;}
"vec3"      {yTRACE(TYPE); yylval.type = strdup(yytext);    return TYPE;}
"vec4"      {yTRACE(TYPE); yylval.type = strdup(yytext);    return TYPE;}
"bvec2"     {yTRACE(TYPE); yylval.type = strdup(yytext);    return TYPE;}
"bvec3"     {yTRACE(TYPE); yylval.type = strdup(yytext);    return TYPE;}
"bvec4"     {yTRACE(TYPE); yylval.type = strdup(yytext);    return TYPE;}
"ivec2"     {yTRACE(TYPE); yylval.type = strdup(yytext);    return TYPE;}
"ivec3"     {yTRACE(TYPE); yylval.type = strdup(yytext);    return TYPE;}
"ivec4"     {yTRACE(TYPE); yylval.type = strdup(yytext);    return TYPE;}

"lit"       {yTRACE(FUNCNAME); yylval.funcName = strdup(yytext);  return FUNCNAME;}
"dp3"       {yTRACE(FUNCNAME); yylval.funcName = strdup(yytext);  return FUNCNAME;}
"rsq"       {yTRACE(FUNCNAME); yylval.funcName = strdup(yytext);  return FUNCNAME;}

"if"        {yTRACE(IF);       return IF;}       
"else"      {yTRACE(ELSE);     return ELSE;}
"while"     {yTRACE(WHILE);    return WHILE;}


{FLOAT}     {yTRACE(FLOAT); yylval.fnum = atof(yytext); return (strcmp(yytext,".")) ? FLOAT : Error; }
{SIGN}?{DIGIT}+ {
  yTRACE(INT);
  yylval.num = atoi(yytext);
  if(yylval.num > MAX_INTEGER)
		  yERROR("Maximum size of Integer exceeded.");
  
	else
		  return INT;
}	
{LETTER}({LETTER}|{DIGIT})*("_"({LETTER}|{DIGIT})+)*   {yTRACE((VARN)); yylval.string = strdup(yytext); return(VARN);}
("lit"|"dp3"|"rsq")"(""("((({DIGIT},)*{DIGIT})| (({FLOAT},)*{FLOAT}))?")"{yTRACE((FCONSTR)); yylval.string = strdup(yytext); return(FCONSTR);}
{LETTER}({LETTER}|{DIGIT})*("_"({LETTER}|{DIGIT})+)* "[" {DIGIT}+"]"{yTRACE((ARRYN)); yylval.string = strdup(yytext); return(ARRYN);}

{IDENTIFIER}  {
  yTRACE(IDENTIFIER);
  if (strlen(yytext) > MAX_IDENTIFIER)
		  yERROR("Maximum length of Identifier exceeded.");
   
  else
	    return IDENTIFIER;
}


"vec2""("{FLOAT}","{FLOAT}")"{yTRACE((VEC2));yylval.string = strdup(yytext);return(VEC2);}
"vec3""("{FLOAT}","{FLOAT}","{FLOAT}")"{yTRACE((VEC3));yylval.string = strdup(yytext);return(VEC3);}
"vec4""("{FLOAT}","{FLOAT}","{FLOAT}","{FLOAT}")"{yTRACE((VEC4));yylval.string = strdup(yytext);return(VEC4);}
"bvec2""("{BOOL}","{BOOL}")"{yTRACE((BVEC2));yylval.string = strdup(yytext);return(BVEC2);}
"bvec3""("{BOOL}","{BOOL}","{BOOL}")"{yTRACE((BVEC3));yylval.string = strdup(yytext);return(BVEC3);}
"bvec4""("{BOOL}","{BOOL}","{BOOL}","{BOOL}")"{yTRACE((BVEC4));yylval.string = strdup(yytext);return(BVEC4);}
"ivec2""("{DIGIT}+","{DIGIT}+")"{yTRACE((VEC2));yylval.string = strdup(yytext);return(VEC2);}
"ivec3""("{DIGIT}+","{DIGIT}+","{DIGIT}+")"{yTRACE((IVEC3));yylval.string = strdup(yytext);return(IVEC3);}
"ivec4""("{DIGIT}+","{DIGIT}+","{DIGIT}+","{DIGIT}+")"{yTRACE((IVEC4));yylval.string = strdup(yytext);return(IVEC4);}



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

