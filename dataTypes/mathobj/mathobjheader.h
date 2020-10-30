#ifndef MATH_OBJ_HEADER_H_
#define MATH_OBJ_HEADER_H_

#include "../str.h"
#include "../array.h"
#include "../header.h"

typedef enum __MATH_OBJ_TYPE {
    NOTHING,
    EQUATION,
    CONSTANT_LONG,
    CONSTANT_DOUBLE,
    VARIABLE,
    PLUS,
    NEGATE,
    PRODUCT,
    FRACTION
} math_type;

union __MATH_OBJ_VALUE {
   long int i;
   double f;
};

typedef union __MATH_OBJ_NODE_DATA
{
    struct Math_Object ** children; // node - operator
    union __MATH_OBJ_VALUE permValue; // leaf - constant
    String label; // leaf - variable
};


typedef struct Math_Object {
    union __MATH_OBJ_NODE_DATA data; 
    math_type typeTag;
} math_struct;

int x = sizeof(union __MATH_OBJ_NODE_DATA);

typedef math_struct * math_obj;
typedef math_obj * math_obj_array;


inline math_obj_array newMathObjectArray(size_t c) {
    return newArray(c, sizeof(math_obj));
}

#endif