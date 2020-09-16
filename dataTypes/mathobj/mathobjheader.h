#ifndef MATH_OBJ_HEADER_H_
#define MATH_OBJ_HEADER_H_

#include "../str.h"
#include "../array.h"
#include "../header.h"

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
#define PRODUCT 6
#define FRACTION 7

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

#endif