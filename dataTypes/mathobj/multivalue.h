#ifndef MULTIVALUE_H_
#define MULTIVALUE_H_
#include "mathobjheader.h"

long int math_obj_mvalue_getAsLong(math_obj self);

double math_obj_mvalue_getAsDouble(math_obj self);

void math_obj_mvalue_assert(math_obj self);

bool math_obj_mvalue_isEqualToDouble(math_obj self, double d);

bool math_obj_mvalue_isEqualToLong(math_obj self, long n);

#endif