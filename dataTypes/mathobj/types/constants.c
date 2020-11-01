#include "typedefs.h"
#include "../mathobjheader.h"
#include "../mathobj.h"
#include "../../../errors.h"
#include "../../util.h"

math_obj buildMathObjectConstantLong(long int n) {
    union __MATH_OBJ_VALUE value;
    value.i = n;

    return __buildMathObjectConstantLike(value, CONSTANT_LONG);
}

math_obj buildMathObjectConstantDouble(double n) {
    union __MATH_OBJ_VALUE value;
    value.f = n;

    return __buildMathObjectConstantLike(value, CONSTANT_LONG);
}

math_obj buildMathObjectConstantFromString(String * label) {
    //math_obj m = __buildMathObjectCustom(str_move(label), NULL, CONSTANT);

    assert(str_getLen(label) > 0);

    if (str_isInteger(label)) {
        long int n = str_toInteger(label);
        return buildMathObjectConstantLong(n);
    }
    else {
        if (str_contains(label, 'e') || str_contains(label, 'E')) {
            throw_error("Invalid number", str_getString(label));
        }
        double d = str_toDouble(label);

        return buildMathObjectConstantDouble(d);
    }
}



bool math_obj_mvalue_isEqualToDouble(math_obj self, double d) {
    assert(self->typeTag == CONSTANT_DOUBLE || self->typeTag == CONSTANT_LONG);

    double s = math_obj_mvalue_getAsDouble(self);

    return util_percentDiff(s, d) < PERCENT_DIFF_TO_BE_SAME;
}

bool math_obj_mvalue_isEqualToLong(math_obj self, long n) {
    assert(self->typeTag == CONSTANT_DOUBLE || self->typeTag == CONSTANT_LONG);

    if (self->typeTag == CONSTANT_LONG) {
        long int s = math_obj_mvalue_getAsLong(self);
        return s == n;
    }
    else {
        double s = math_obj_mvalue_getAsDouble(self);
        return util_percentDiff(s, (double)n) < PERCENT_DIFF_TO_BE_SAME;
    }
}

long int math_obj_mvalue_getAsLong(math_obj self) {
    assert(self->typeTag == CONSTANT_DOUBLE || self->typeTag == CONSTANT_LONG);

    if (self->typeTag == CONSTANT_LONG) {
        return self->data.permValue.i;
    }
    else if (self->typeTag == CONSTANT_DOUBLE) {
        return (long int) self->data.permValue.f;
    }
    else {
        assert(false);
    }
}

double math_obj_mvalue_getAsDouble(math_obj self) {
    assert(self->typeTag == CONSTANT_DOUBLE || self->typeTag == CONSTANT_LONG);

    if (self->typeTag == CONSTANT_LONG) {
        return (double) self->data.permValue.i;
    }
    else if (self->typeTag == CONSTANT_DOUBLE) {
        return self->data.permValue.f;
    }
    else {
        assert(false);
    }
}

inline void math_obj_mvalue_assert(math_obj self) {
    assert(usesPermValue(self));
}