%{
#include <stdio.h>
#include <stdlib.h>
#include "lexer.h"
#include "../dataTypes/mathobj.h"
%}

%union {
    long int ival;
    double fval;
    String sval;
    math_obj mobj;
}

%parse-param { math_obj *out } { StmtType *stmt }

%token <ival> TOK_CONST_INT
%token <fval> TOK_CONST_FLOAT
%token <sval> TOK_VARIABLE

%type <mobj> E T F F2 value var

%token DEFINE

%start  start

%%

start: E { *out = $1; *stmt = STMT_Expression; }
    | DEFINE value '=' E { *out = buildMathObjectEquation($2, $4); *stmt = STMT_Definition; }

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
    | var '(' E ')' { $$ = buildMathObjectFunction($1, $3); }

value:
    TOK_CONST_INT { $$ = buildMathObjectConstantLong($1); }
    | TOK_CONST_FLOAT { $$ = buildMathObjectConstantDouble($1); }
    | var { $$ = $1; }

var: TOK_VARIABLE { $$ = buildMathObjectVariable(&($1)); }

%%