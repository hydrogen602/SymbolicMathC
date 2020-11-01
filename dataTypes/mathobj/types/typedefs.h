#ifndef MATH_OBJ_TYPEDEFS_H_
#define MATH_OBJ_TYPEDEFS_H_
#include <stdbool.h>

/* 
===================================================================
                        constants.c
===================================================================
*/

math_obj buildMathObjectConstantLong(long int n);

math_obj buildMathObjectConstantDouble(double n);

math_obj buildMathObjectConstantFromString(String * label);

long int math_obj_mvalue_getAsLong(math_obj self);

double math_obj_mvalue_getAsDouble(math_obj self);

bool math_obj_mvalue_isEqualToDouble(math_obj self, double d);

bool math_obj_mvalue_isEqualToLong(math_obj self, long n);

/* 
===================================================================
                        variables.c
===================================================================
*/

math_obj buildMathObjectVariable(String * label);

/* 
===================================================================
                        operators.c
===================================================================
*/

#endif