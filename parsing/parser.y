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

%type <mobj> start

%token DEFINE

%start	start

%%

start: 
    TOK_CONST_INT { *out = buildMathObjectConstantLong($1); }
    | TOK_CONST_FLOAT { *out = buildMathObjectConstantDouble($1); }

%%