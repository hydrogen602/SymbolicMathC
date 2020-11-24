#include "../dataTypes/mathobj.h"
#include "lexer.h"

struct ParseOutput {
    math_obj obj;
    StmtType type;
};

struct ParseOutput parse(const char * s);