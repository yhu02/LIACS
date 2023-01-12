/* Let Flex know we will not use yyunput and input functions
	- unput: fabricate input text to be scanned
	- input: directly read input
*/
%option nounput noinput

%{
/* lexeme of identifier or reserved word */
const int MAXTOKENLEN = 40;
char tokenString[MAXTOKENLEN+1];
int lineno = 1;
int start_token = 1;
%}


/* C declarations */
%{
#include <iostream>
#include "bison.compiler.h"
#include "../cpp/debug/debug.h"

/* extern C declarations */
#if defined(__cplusplus)
extern "C" {

#endif

/* should be defined in stdio.h */
extern int fileno(FILE *);
int yylex(void);

#if defined(__cplusplus)
}
#endif
%}

digit             [0-9]
number            {digit}+
newline           \n
whitespace        [ \t]+
identifier  [a-zA-Z_][a-zA-Z0-9_]*

%%
%{
if (start_token) {
    start_token = 0;
    if (CURRENT_ASSIGNMENT == ASS_ONE) {
      return START_EXPRESSION;
    } else if (CURRENT_ASSIGNMENT < ASS_THREE) {
      return START_MAIN_SINGLE;
    } else {
      return START_MAIN;
    }
}
%}

"="             { return ASSIGN; }
";"             { return SEMI; }
"-"             { return MINUS; }
"+"             { return PLUS; }
"*"             { return TIMES; }
"("             { return LPAREN; }
")"             { return RPAREN; }

"int main()"    { return MAIN; }
"int"           { return INT; }
"int8_t"        { return INT8; }
"unsigned"      { return UNSIGNED; }
"uint8_t"       { return UINT8; }
"if"            { return IF; }
"else"          { return ELSE; }
"while"         { return WHILE; }
"print"         { return PRINT; }
","             { return COMMA; }
"=="            { return EQ; }
"!="            { return NEQ; }
">"             { return GT; }
">="            { return GEQ; }
"<"             { return LT; }
"<="            { return LEQ; }
"/"             { return DIV; }
"%"             { return MOD; }
"{"             { return LBRACE; }
"}"             { return RBRACE; }

{identifier}    { yylval.idStr = strdup(yytext); return ID; }

 /* number */
{number}        { yylval.number = strdup(yytext); return NUMBER; }

 /* GENERAL */
{newline}       { ++lineno; }
{whitespace}    { /* skip whitespace */ }
<<EOF>>         { start_token = 1; return ENDFILE; }

.|\n            {
                    /* if no rule can be applied, give an error */
                    pmesg(90, "[LEXER] cannot match symbol: %s\n", yytext);
                    return 1;
                }
%%