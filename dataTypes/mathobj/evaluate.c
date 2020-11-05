#include "evaluate.h"
#include "mathobj.h"
#include "../../errors.h"
#include "../util.h"
#include "types/typedefs.h"
#include "variables.h"

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

math_obj math_obj_simplify_negate(math_obj self) {
    assert(self->typeTag == NEGATE);
    assert(len(math_obj_getChildren(self)) == 1);

    if (math_obj_isConstant(self->data.children[0])) {
        math_obj newSelf = __math_obj_eval_negate(self->data.children[0]);

        math_obj_free(self);
        return newSelf;
    }

    return self;
}

math_obj math_obj_simplify_plus(math_obj self) {
    unsigned int childCount = len(math_obj_getChildren(self));
    assert(self->typeTag == PLUS);
    assert(childCount > 1);
    // analyze for possible simplification
    math_obj last = NULL;
    int indexOfFirst = -1;

    for (unsigned int i = 0; i < childCount; ++i) {
        if (math_obj_isConstant(self->data.children[i])) {
            if (last == NULL) {
                last = self->data.children[i];
                self->data.children[i] = NULL;
                indexOfFirst = i;
            }
            else {
                math_obj newlast = __math_obj_eval_plus(last, self->data.children[i]);
                math_obj_free(last);
                math_obj_free(self->data.children[i]);
                self->data.children[i] = NULL;

                last = newlast;
            }
        }
    }

    if (last != NULL) {
        assert(math_obj_isConstant(last));
        
        if (math_obj_mvalue_isEqualToLong(last, 0)) {
            math_obj_free(last);
            self->data.children[indexOfFirst] = NULL;
        }
        else {
            self->data.children[indexOfFirst] = last;
        }
        
    }

    self->data.children = __math_obj_takeOutNull(self->data.children);

    if (len(math_obj_getChildren(self)) == 1) {
        // only one left
        math_obj tmp = self->data.children[0];
        self->data.children[0] = NULL;
        math_obj_free(self);
        return tmp;
    }
    else if (len(math_obj_getChildren(self)) == 0) {
        // nothing left
        math_obj tmp = buildMathObjectConstantLong(0);
        math_obj_free(self);
        return tmp;
    }

    return self;
}

math_obj math_obj_simplify_product(math_obj self) {
    assert(self->typeTag == PRODUCT);
    unsigned int childCount = len(math_obj_getChildren(self));

    assert(childCount > 1);
    // analyze for possible simplification
    math_obj last = NULL;
    int indexOfFirst = -1;

    for (unsigned int i = 0; i < childCount; ++i) {
        if (math_obj_isConstant(self->data.children[i])) {
            
            if (math_obj_mvalue_isEqualToLong(self->data.children[i], 0)) {
                // everything is zero
                math_obj_free(self);
                return buildMathObjectConstantLong(0);
            }

            if (last == NULL) {
                last = self->data.children[i];
                self->data.children[i] = NULL;
                indexOfFirst = i;
            }
            else {
                math_obj newlast = __math_obj_eval_product(last, self->data.children[i]);
                math_obj_free(last);
                math_obj_free(self->data.children[i]);
                self->data.children[i] = NULL;

                last = newlast;
            }
        }
    }

    if (last != NULL) {
        assert(math_obj_isConstant(last));
        if (math_obj_mvalue_isEqualToLong(last, 1)) {
            math_obj_free(last);
            self->data.children[indexOfFirst] = NULL;
        }
        else {
            self->data.children[indexOfFirst] = last;
        }
        
    }

    self->data.children = __math_obj_takeOutNull(self->data.children);

    if (len(self->data.children) == 1) {
        // only one left
        math_obj tmp = self->data.children[0];
        self->data.children[0] = NULL;
        math_obj_free(self);
        return tmp;
    }
    else if (len(self->data.children) == 0) {
        // nothing left
        math_obj tmp = buildMathObjectConstantLong(1);
        math_obj_free(self);
        return tmp;
    }

    return self;
}

math_obj math_obj_simplify_fraction(math_obj self) {
    assert(self->typeTag == FRACTION);
    int childCount = len(self->data.children);
    assert(childCount == 2);

    math_obj num = self->data.children[0];
    math_obj denom = self->data.children[1];

    if (math_obj_isConstant(num) && math_obj_mvalue_isEqualToLong(num, 0)) {
        math_obj_free(self);
        return buildMathObjectConstantLong(0);
    }
    else if (math_obj_isConstant(denom) && math_obj_mvalue_isEqualToLong(denom, 1)) {
        self->data.children[0] = NULL;
        math_obj_free(self);
        return num;
    }

    if (math_obj_isConstant(num) && math_obj_isConstant(denom)) {
        // TODO
        math_obj m = __math_obj_eval_fraction(num, denom);
        math_obj_free(self);
        return m;
    }

    // math_obj newSelf = __math_obj_eval_negate(self->children[0]);

    // math_obj_free(self);
    // return newSelf;


    return self;
}

math_obj math_obj_eval(math_obj self) {
    // if not returning self, free self!!!
    if (self == NULL) {
        return NULL;
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
