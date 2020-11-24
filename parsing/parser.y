%{
#include <stdio.h>
#include <stdlib.h>
#include "lexer.h"
#include "../dataTypes/mathobj.h"
%}

%union {
    long int ival;
    double fval;
    char *sval;
    math_obj mobj;
}

%parse-param { math_obj *out }

%token <ival> TOK_CONST_INT
%token <fval> TOK_CONST_FLOAT
%token <sval> TOK_VARIABLE

%type <mobj> E T F F2 value

%token DEFINE

%start  start

%%

start: E { *out = $1; }

E:    T { $$ = $1; } 
    | E '+' T { $$ = buildMathObjectPlus(buildMathObjectArrayFrom2($1, $3)); }
    | E '-' T { $$ = buildMathObjectPlus(buildMathObjectArrayFrom2($1, buildMathObjectNegate($3))); }

T:    F { $$ = $1; }
    | T '*' F { $$ = buildMathObjectProduct(buildMathObjectArrayFrom2($1, $3)); }
    | T '/' F { $$ = buildMathObjectFraction($1, $3); }

F:    F2     { $$ = $1; }
    | '-' F2 { $$ = buildMathObjectNegate($2); }

F2:   '(' E ')' { $$ = $2; }
    | value     { $$ = $1; }

value:
    TOK_CONST_INT { $$ = buildMathObjectConstantLong($1); }
    | TOK_CONST_FLOAT { $$ = buildMathObjectConstantDouble($1); }
    | TOK_VARIABLE { String s = buildString($1); $$ = buildMathObjectVariable(&s); free($1); }

%%