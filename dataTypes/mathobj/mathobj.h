#ifndef MATH_OBJ_H_
#define MATH_OBJ_H_

#include "mathobjheader.h"


math_obj buildMathObjectNull();

math_obj buildMathObjectVariable(String * label);

math_obj buildMathObjectConstant(String * label);

math_obj buildMathObjectEquation(math_obj a, math_obj b);

math_obj buildMathObjectConstantLong(long int n);

math_obj buildMathObjectConstantDouble(double n);

math_obj buildMathObjectPlus(math_obj_array arr);

math_obj buildMathObjectNegate(math_obj m);

math_obj buildMathObjectProduct(math_obj_array arr);

math_obj buildMathObjectFraction(math_obj n, math_obj d);

void math_obj_free(math_obj self);

void math_obj_printer(math_obj self);

void math_obj_debug_printer(math_obj self);

bool math_obj_isConstant(math_obj self);

#endif