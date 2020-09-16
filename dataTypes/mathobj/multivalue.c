#include "multivalue.h"

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