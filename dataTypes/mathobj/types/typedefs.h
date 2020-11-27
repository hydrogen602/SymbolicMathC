#ifndef MATH_OBJ_TYPEDEFS_H_
#define MATH_OBJ_TYPEDEFS_H_
#include <stdbool.h>
#include "../mathobjheader.h"

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

void math_obj_mvalue_assert(math_obj self);

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

math_obj buildMathObjectPlus(math_obj_array arr);

math_obj buildMathObjectNegate(math_obj m);

math_obj buildMathObjectProduct(math_obj_array arr);

math_obj buildMathObjectFraction(math_obj n, math_obj d);

math_obj buildMathObjectEquation(math_obj a, math_obj b);

/* 
===================================================================
                        functions.c
===================================================================
*/

math_obj buildMathObjectFunction(math_obj label, math_obj m);

#endif