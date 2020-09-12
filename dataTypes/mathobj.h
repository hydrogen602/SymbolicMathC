#ifndef MATH_OBJ_H_
#define MATH_OBJ_H_

#include "str.h"
#include "array.h"

#define newMathObjectArray(c) newArray(c, sizeof(math_obj))

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
#define NEGATE 5

union __MATH_OBJ_VALUE {
   long int i;
   double f;
};

typedef struct Math_Object {
    String label;
    struct Math_Object ** children;
    union __MATH_OBJ_VALUE permValue;
    unsigned char typeTag;
    value_type_t permValueType: 2;
} math_struct;

typedef math_struct * math_obj;
typedef math_obj * math_obj_array;

math_obj buildMathObjectNull();

math_obj buildMathObjectVariable(String * label);

math_obj buildMathObjectConstant(String * label);

math_obj buildMathObjectEquation(math_obj a, math_obj b);

math_obj buildMathObjectPlus(math_obj_array arr);

math_obj buildMathObjectNegate(math_obj m);

void math_obj_free(math_obj self);

void math_obj_printer(math_obj self);

void math_obj_debug_printer(math_obj self);

math_obj math_obj_eval(math_obj self);

#endif