
%option never-interactive

%option nounput
%option noinput
%option noyywrap

%{
#include "lexer.h"
#include "parser.tab.h"
#include <stdio.h>
#include <string.h>
#include "../dataStructs/str.h"

int yylineno;

char * strdup(const char * s);

%}

integer			[0-9]+
exponent        [0-9]+(\.[0-9]+)?[eE][+-]?[0-9]+
float 			[0-9]+\.[0-9]+
variable        [[:alpha:]_][[:alpha:][:digit:]_]*

%%

"define"    { return DEFINE; }

{exponent}  { yylval.fval = str_toDouble_cString(yytext); return TOK_CONST_FLOAT; }
{integer}   { yylval.ival = str_toInteger_cString(yytext); return TOK_CONST_INT; }
{float}     { yylval.fval = str_toDouble_cString(yytext); return TOK_CONST_FLOAT; }
{variable}  { yylval.sval = strdup(yytext); return TOK_VARIABLE; }

[=+*/\-()]     { return yytext[0]; }

" "         { /* do nothing */ }
.		    { yyerror(NULL, NULL, yytext); }
\n          { ++yylineno; return '\n'; }

%%

int yyerror(math_obj *out, StmtType *stmt, const char* s)
{
    fprintf(stderr, "Invalid token on line %d: %s\n", yylineno, s);

    exit(1);
}

char * strdup(const char *s) {
    size_t length = strlen(s) + 1;
    char * c = calloc(length, sizeof(char));
    if (c == NULL) {
        fprintf(stderr, "Calloc failed\n");
        exit(1);
    }
    strcpy(c, s);
    return c;
}
