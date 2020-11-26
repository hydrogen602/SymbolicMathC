#ifndef SIMPLIFY_H_
#define SIMPLIFY_H_

#include "mathobjheader.h"

math_obj math_obj_simplify_negate(math_obj self);
math_obj math_obj_simplify_plus(math_obj self);
math_obj math_obj_simplify_product(math_obj self);
math_obj math_obj_simplify_fraction(math_obj self);

#endif