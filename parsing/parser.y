%{
#include <stdio.h>
#include <stdlib.h>
#include "lexer.h"
//#include "../dataTypes/mathobj.h"
%}

%union {
    long int ival;
    double fval;
    char *sval;
}

%parse-param { int *out }

%token <ival> CONST_INT
%token <fval> CONST_FLOAT
%token <sval> VARIABLE

%type <ival> start

%token DEFINE

%start	start

%%

start: 
    CONST_INT '+' CONST_INT { *out = $1 + $3; }

%%