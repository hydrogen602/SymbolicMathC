#include "evaluate.h"
#include "mathobj.h"
#include "../../errors.h"
#include "../util.h"
#include "types/typedefs.h"
#include "variables.h"
#include "simplify.h"
#include "flatten.h"

/**
 * Frees the incoming array if creating a new one
 */
math_obj_array __math_obj_takeOutNull(math_obj_array m) {
    int len2 = len(m);
    for (unsigned int i = 0; i < len(m); ++i) {
        if (m[i] == NULL) {
            len2--;
        }
    }

    if (len2 == 0) {
        freeArray(m);
        return NULL;
    }

    math_obj_array out = newMathObjectArray(len2);

    int index = 0;
    for (unsigned int i = 0; i < len(m); ++i) {
        if (m[i] != NULL) {
            out[index] = m[i];
            index++;
        }
    }

    freeArray(m);
    return out;
}

math_obj __math_obj_eval_plus(math_obj self, math_obj other) {
    math_obj_mvalue_assert(self);
    math_obj_mvalue_assert(other);

    if (self->typeTag == CONSTANT_DOUBLE || other->typeTag == CONSTANT_DOUBLE) {
        // double calc

        double val = math_obj_mvalue_getAsDouble(self) + math_obj_mvalue_getAsDouble(other);

        return buildMathObjectConstantDouble(val);
    }
    else {
        // int calc

        long int val = math_obj_mvalue_getAsLong(self) + math_obj_mvalue_getAsLong(other);

        return buildMathObjectConstantLong(val);
    }
}

math_obj __math_obj_eval_negate(math_obj self) {
    math_obj_mvalue_assert(self);

    if (self->typeTag == CONSTANT_DOUBLE) {
        // double calc

        double val = - math_obj_mvalue_getAsDouble(self);

        return buildMathObjectConstantDouble(val);
    }
    else {
        // int calc

        long int val = - math_obj_mvalue_getAsLong(self);

        return buildMathObjectConstantLong(val);
    }
}

math_obj __math_obj_eval_product(math_obj self, math_obj other) {
    math_obj_mvalue_assert(self);
    math_obj_mvalue_assert(other);

    if (self->typeTag == CONSTANT_DOUBLE || other->typeTag == CONSTANT_DOUBLE) {
        // double calc

        double val = math_obj_mvalue_getAsDouble(self) * math_obj_mvalue_getAsDouble(other);

        return buildMathObjectConstantDouble(val);
    }
    else {
        // int calc

        long int val = math_obj_mvalue_getAsLong(self) * math_obj_mvalue_getAsLong(other);

        return buildMathObjectConstantLong(val);
    }
}

math_obj __math_obj_eval_division(math_obj self, math_obj other) {
    math_obj_mvalue_assert(self);
    math_obj_mvalue_assert(other);

    if (math_obj_mvalue_isEqualToLong(other, 0)) {
        throw_error("Division by zero", "0");
    }

    if (self->typeTag == CONSTANT_DOUBLE || other->typeTag == CONSTANT_DOUBLE) {
        // double calc

        double val = math_obj_mvalue_getAsDouble(self) / math_obj_mvalue_getAsDouble(other);

        return buildMathObjectConstantDouble(val);
    }
    else {
        // int calc

        long int num = math_obj_mvalue_getAsLong(self);
        long int denom = math_obj_mvalue_getAsLong(other);

        if (num % denom == 0) {
            // clean divide
            return buildMathObjectConstantLong(num / denom);
        }
        return buildMathObjectConstantDouble( (double)num / (double)denom );
    }
}

math_obj __math_obj_eval_fraction(math_obj self, math_obj other) {
    math_obj_mvalue_assert(self);
    math_obj_mvalue_assert(other);

    if (math_obj_mvalue_isEqualToLong(other, 0)) {
        throw_error("Division By Zero", "0");
    }

    if (math_obj_mvalue_isEqualToLong(self, 0)) {
        return buildMathObjectConstantLong(0);
    }

    if (self->typeTag == CONSTANT_DOUBLE || other->typeTag == CONSTANT_DOUBLE) {
        // double calc
        return __math_obj_eval_division(self, other);
    }
    else {
        // int calc

        long int num = math_obj_mvalue_getAsLong(self);
        long int denom = math_obj_mvalue_getAsLong(other);

        long int divisor = util_gcd(num, denom);

        num = num / divisor;
        denom = denom / divisor;

        if (denom == 1) {
            return buildMathObjectConstantLong(num);
        }

        math_obj n = buildMathObjectConstantLong(num);
        math_obj d = buildMathObjectConstantLong(denom);

        return buildMathObjectFraction(n, d);
    }
}

math_obj math_obj_eval(math_obj self) {
    // if not returning self, free self!!!
    if (self == NULL) {
        return NULL;
    }

    if (self->typeTag == PLUS) {
        self = math_obj_flatten_plus(self);
    }

    unsigned int childCount = len(math_obj_getChildren(self));

    for (unsigned int i = 0; i < childCount; ++i) {
        self->data.children[i] = math_obj_eval(self->data.children[i]);
    }

    if (childCount == 0) {
        if (self->typeTag == VARIABLE) {
            math_obj m = variables_get(&self->data.label);
            if (m != NULL) {
                math_obj_free(self);
                return m;
            }
        }
        return self; // cant simplify one thing
    }
    elif (childCount == 1) {
        switch (self->typeTag)
        {
        case NEGATE:
            return math_obj_simplify_negate(self);        
        default:
            assert(false);
            break;
        }

        return self; // don't have any of these yet
    }
    else {
        switch (self->typeTag)
        {
        case EQUATION:
            return self;
        case PLUS: 
            return math_obj_simplify_plus(self);
        case PRODUCT:
            return math_obj_simplify_product(self);
        case FRACTION:
            return math_obj_simplify_fraction(self);
            break;
        default:
            assert(false);
            break;
        }
    }

    return self;
}
