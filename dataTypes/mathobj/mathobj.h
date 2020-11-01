#ifndef MATH_OBJ_H_
#define MATH_OBJ_H_

#include "mathobjheader.h"

#define usesLabel(t) __usesLabel((t)->typeTag)

#define usesPermValue(t) __usesPermValue((t)->typeTag)

#define usesChildren(t) __usesChildren((t)->typeTag)

/*
 * Returns NULL if self does not use children i.e. is constant or var, not operator
 */
math_obj_array math_obj_getChildren(math_obj self);

bool __usesLabel(math_type t);

bool __usesPermValue(math_type t);

bool __usesChildren(math_type t);

/*
 * Returns 0 if there is no symbol
 */
char math_obj_getOpSymbol(math_type t);

math_obj __buildMathObjectVarLike(String * label, math_type typeTag);

math_obj __buildMathObjectOperatorLike(math_obj_array arr, math_type typeTag);

math_obj __buildMathObjectConstantLike(union __MATH_OBJ_VALUE val, math_type typeTag);

bool math_obj_isConstant(math_obj self);

void math_obj_free(math_obj self);

math_obj math_obj_copy(math_obj self);


// math_obj buildMathObjectConstant(String * label);

// math_obj buildMathObjectEquation(math_obj a, math_obj b);

// math_obj buildMathObjectPlus(math_obj_array arr);

// math_obj buildMathObjectNegate(math_obj m);

// math_obj buildMathObjectProduct(math_obj_array arr);

// math_obj buildMathObjectFraction(math_obj n, math_obj d);


void math_obj_printer(math_obj self);

void math_obj_debug_printer(math_obj self);

void math_obj_debug_dump(math_obj m);

#endif