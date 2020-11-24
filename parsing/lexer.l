
%option never-interactive

%option nounput
%option noinput
%option noyywrap

%{
#include "tokens.h"
#include "lexer.h"
#include "parser.tab.h"
#include <stdio.h>
#include <string.h>
#include "../dataStructs/str.h"

int yylineno;

int yylex();
int yyparse();
int yyerror(const char* s);

char * strdup(const char * s);

// "="         { return Tok_Equal; }
// "+"         { return Tok_Plus; }
// "*"         { return Tok_Times; }
// "/"         { return Tok_Divide; }
// "-"         { return Tok_Minus; }

%}

integer			-?[0-9]+
exponent        -?[0-9]+(\.[0-9]+)?[eE][+-]?[0-9]+
float 			-?[0-9]+\.[0-9]+
variable        [[:alpha:]_][[:alpha:][:digit:]_]*

%%

"define"    { return Tok_define; }

{exponent}  { yylval.fval = str_toDouble_cString(yytext); return CONST_FLOAT; }
{integer}   { yylval.ival = str_toInteger_cString(yytext); return CONST_INT; }
{float}     { yylval.fval = str_toDouble_cString(yytext); return CONST_FLOAT; }
{variable}  { yylval.sval = strdup(yytext); return VARIABLE; }


" "         { /* do nothing */ }
.		    { yyerror(yytext); }
\n          { ++yylineno; return '\n'; }

%%

int yyerror(const char* s)
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
