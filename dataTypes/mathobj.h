#ifndef MATH_OBJ_H_
#define MATH_OBJ_H_

#include "str.h"

#define NOTHING 0
#define EQUATION 1
#define VARIABLE 2
#define PLUS 3


typedef struct Math_Object {
    String label;
    struct Math_Object * childA;
    struct Math_Object * childB;
    char childCount;
    char typeTag;
} math_struct;

typedef math_struct * math_obj;

math_obj buildMathObjectNull();

math_obj buildMathObjectVariable(String * label);

math_obj buildMathObjectEquation(math_obj a, math_obj b);

math_obj buildMathObjectPlus(math_obj a, math_obj b);

void math_obj_free(math_obj self);

void math_obj_printer(math_obj self);

#endif