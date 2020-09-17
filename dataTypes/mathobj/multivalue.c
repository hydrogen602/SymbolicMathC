#include "multivalue.h"
#include "../util.h"

bool math_obj_mvalue_isEqualToDouble(math_obj self, double d) {
    assert(self->typeTag == CONSTANT);

    double s = math_obj_mvalue_getAsDouble(self);

    return util_percentDiff(s, d) < PERCENT_DIFF_TO_BE_SAME;
}

bool math_obj_mvalue_isEqualToLong(math_obj self, long n) {
    assert(self->typeTag == CONSTANT);

    if (self->permValueType == MATH_OBJ_LONG) {
        long int s = math_obj_mvalue_getAsLong(self);
        return s == n;
    }
    else {
        double s = math_obj_mvalue_getAsDouble(self);
        return util_percentDiff(s, (double)n) < PERCENT_DIFF_TO_BE_SAME;
    }
}

long int math_obj_mvalue_getAsLong(math_obj self) {
    assert(self->typeTag == CONSTANT);

    if (self->permValueType == MATH_OBJ_LONG) {
        return self->permValue.i;
    }
    else if (self->permValueType == MATH_OBJ_DOUBLE) {
        return (long int) self->permValue.f;
    }
    else {
        assert(false);
    }
}

double math_obj_mvalue_getAsDouble(math_obj self) {
    assert(self->typeTag == CONSTANT);

    if (self->permValueType == MATH_OBJ_LONG) {
        return (double) self->permValue.i;
    }
    else if (self->permValueType == MATH_OBJ_DOUBLE) {
        return self->permValue.f;
    }
    else {
        assert(false);
    }
}

void math_obj_mvalue_assert(math_obj self) {
    assert(self->typeTag == CONSTANT);
    assert(self->permValueType == MATH_OBJ_LONG || self->permValueType == MATH_OBJ_DOUBLE);
}