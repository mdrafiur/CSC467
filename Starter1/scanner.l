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
letter      [A-Za-z]
digit       [0-9]
true        "true"
false       "false"
sign        [\+\-]

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

"int"       { yTRACE((TOKEN_INT_DEC));      return (TOKEN_INT_DEC);}
"bool"      { yTRACE((TOKEN_BOOL));         return (TOKEN_BOOL);}
"float"     { yTRACE((TOKEN_FLOAT_DEC));    return (TOKEN_FLOAT_DEC);}
"vec2"      { yTRACE((TOKEN_VEC2));         return (TOKEN_VEC2);}
"vec3"      { yTRACE((TOKEN_VEC3));         return (TOKEN_VEC3);}
"vec4"      { yTRACE((TOKEN_VEC4));         return (TOKEN_VEC4);}
"bvec2"     { yTRACE((TOKEN_BVEC2));        return (TOKEN_BVEC2);}
"bvec3"     { yTRACE((TOKEN_BVEC3));        return (TOKEN_BVEC3);}
"bvec4"     { yTRACE((TOKEN_BVEC4));        return (TOKEN_BVEC4);}
"ivec2"     { yTRACE((TOKEN_IVEC2));        return (TOKEN_IVEC2);}
"ivec3"     { yTRACE((TOKEN_IVEC3));        return (TOKEN_IVEC3);}
"ivec4"     { yTRACE((TOKEN_IVEC4));        return (TOKEN_IVEC4);}
%%

