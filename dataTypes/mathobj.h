#ifndef MATH_OBJ_H_
#define MATH_OBJ_H_

#include "str.h"

// permanent value types
#define MATH_OBJ_NULL 0
#define MATH_OBJ_LONG 1
#define MATH_OBJ_DOUBLE 2

typedef unsigned char value_type_t;

#define NOTHING 0
#define EQUATION 1
#define CONSTANT 2
#define VARIABLE 3
#define PLUS 4

union __MATH_OBJ_VALUE {
   long int i;
   double f;
};

typedef struct Math_Object {
    String label;
    struct Math_Object * childA;
    struct Math_Object * childB;
    union __MATH_OBJ_VALUE permValue;
    unsigned char childCount: 2;
    unsigned char typeTag;
    value_type_t permValueType: 2;
} math_struct;

typedef math_struct * math_obj;

math_obj buildMathObjectNull();

math_obj buildMathObjectVariable(String * label);

math_obj buildMathObjectConstant(String * label);

math_obj buildMathObjectEquation(math_obj a, math_obj b);

math_obj buildMathObjectPlus(math_obj a, math_obj b);

void math_obj_free(math_obj self);

void math_obj_printer(math_obj self);

#endif