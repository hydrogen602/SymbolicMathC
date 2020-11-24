%{
#include <stdio.h>
#include "lexer.h"
%}

%union {
    long int ival;
    double fval;
    char *sval;
}

%token <ival> CONST_INT
%token <fval> CONST_FLOAT
%token <sval> VARIABLE

%%

start: 
    start CONST_INT { printf("Int: %ld\n", $2); }
    | start CONST_FLOAT { printf("Float: %f\n", $2); }
    | start VARIABLE { printf("string: %s\n", $2); free($2); }
    | CONST_INT { printf("Int: %ld\n", $1); }
    | CONST_FLOAT { printf("Float: %f\n", $1); }
    | VARIABLE { printf("string: %s\n", $1); free($1); }
%%

int main() {
  yy_scan_string("abc d -4");
  // Parse through the input:
  yyparse();

  yylex_destroy();
}