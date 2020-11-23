
%option never-interactive

%option nounput
%option noinput

%{
#include "tokens.h"
#include "lexer.h"
#include <stdio.h>

int yylineno;

int yyerror(const char* s);

%}

%%

"+"     { return Tok_Plus; }
"*"     { return Tok_Times; }
"a"     { return Tok_const; }

.		{ yyerror(yytext); }
\n      { ++yylineno; return 0; }

%%

int yywrap() { return 1; } 

int yyerror(const char* s)
{
    fprintf(stderr, "Invalid token on line %d: %s\n", yylineno, s);

    exit(1);
}

