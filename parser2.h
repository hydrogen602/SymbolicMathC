#ifndef __PARSER2_H
#define __PARSER2_H

#include "dataStructs/parseTable.h"
#include "dataStructs/str.h"

void parser2(SymbolArr input);

void stackTest();

SymbolArr lexer(String str);

#endif