
%option never-interactive

%option nounput
%option noinput
%option noyywrap

%{
#include "tokens.h"
#include "lexer.h"
#include <stdio.h>

int yylineno;

int yyerror(const char* s);

%}

integer			-?[0-9]+
exponent        -?[0-9]+(\.[0-9]+)?[eE][+-]?[0-9]+
float 			-?[0-9]+\.[0-9]+
variable        [[:alpha:]_][[:alpha:][:digit:]_]*

%%

"define"    { return Tok_define; }

{exponent}  { return Tok_float; }
{integer}   { return Tok_int; }
{float}     { return Tok_float; }
{variable}  { return Tok_var; }

"="         { return Tok_Equal; }
"+"         { return Tok_Plus; }
"*"         { return Tok_Times; }
"/"         { return Tok_Divide; }
"-"         { return Tok_Minus; }


" "         { /* do nothing */ }
.		    { yyerror(yytext); }
\n          { ++yylineno; return 0; }

%%

int yyerror(const char* s)
{
    fprintf(stderr, "Invalid token on line %d: %s\n", yylineno, s);

    exit(1);
}

