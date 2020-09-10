#ifndef MATH_OBJ_H_
#define MATH_OBJ_H_

#include "str.h"

#define NOTHING 0
#define VARIABLE 1
#define PLUS 2

typedef struct Math_Object {
    String label;
    struct Math_Object * childA;
    struct Math_Object * childB;
    char childCount;
    char typeTag;
} math_obj;

math_obj buildMathObjectNull();

math_obj buildMathObjectVariable(String * label);

math_obj buildMathObjectPlus(math_obj * a, math_obj * b);

#endif