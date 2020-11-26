#ifndef MATH_EVALUATE_H_
#define MATH_EVALUATE_H_

#include "mathobjheader.h"

#ifdef EVAL_EXTRA_DECL
math_obj_array __math_obj_takeOutNull(math_obj_array m);
math_obj __math_obj_eval_plus(math_obj self, math_obj other);
math_obj __math_obj_eval_negate(math_obj self);
math_obj __math_obj_eval_product(math_obj self, math_obj other);
math_obj __math_obj_eval_division(math_obj self, math_obj other);
math_obj __math_obj_eval_fraction(math_obj self, math_obj other);
#endif

math_obj math_obj_eval(math_obj self);

#endif