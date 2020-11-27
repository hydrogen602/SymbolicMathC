#ifndef FUNCTIONS_H_
#define FUNCTIONS_H_

#include "../dataStructs/str.h"
#include "mathobj.h"

typedef struct FUNCTION_TYPE {
    String name;
    math_obj (*func)(math_obj);
} function;

function buildFunction(String *name, math_obj (*func)(math_obj));

void function_addBuiltin(function func);

void function_init();

void function_cleanup();

math_obj function_callBuiltin(String *name, math_obj input);

math_obj builtin_getChildrenCount(math_obj m);

math_obj builtin_distribute(math_obj m);

#endif