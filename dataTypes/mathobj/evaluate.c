#include "evaluate.h"
#include "mathobj.h"
#include "multivalue.h"
#include "../../errors.h"
#include "../util.h"

/**
 * Frees the incoming array if creating a new one
 */
math_obj_array __math_obj_takeOutNull(math_obj_array m) {
    int len2 = len(m);
    for (int i = 0; i < len(m); ++i) {
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
    for (int i = 0; i < len(m); ++i) {
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

    if (self->permValueType == MATH_OBJ_DOUBLE || other->permValueType == MATH_OBJ_DOUBLE) {
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

    if (self->permValueType == MATH_OBJ_DOUBLE) {
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

    if (self->permValueType == MATH_OBJ_DOUBLE || other->permValueType == MATH_OBJ_DOUBLE) {
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

    if (math_obj_mvalue_getAsLong(other) == 0) {
        throw_error("Division by zero", str_getString(&other->label));
    }

    if (self->permValueType == MATH_OBJ_DOUBLE || other->permValueType == MATH_OBJ_DOUBLE) {
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

    if (math_obj_mvalue_getAsLong(other) == 0) {
        throw_error("Division By Zero", str_getString(&other->label));
    }

    if (math_obj_mvalue_getAsLong(self) == 0) {
        return buildMathObjectConstantLong(0);
    }

    if (self->permValueType == MATH_OBJ_DOUBLE || other->permValueType == MATH_OBJ_DOUBLE) {
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
    assert(len(self->children) == 1);

    if (math_obj_isConstant(self->children[0])) {
        math_obj newSelf = __math_obj_eval_negate(self->children[0]);

        math_obj_free(self);
        return newSelf;
    }

    return self;
}

math_obj math_obj_simplify_plus(math_obj self) {
    int childCount = len(self->children);
    assert(self->typeTag == PLUS);
    assert(childCount > 1);
    // analyze for possible simplification
    math_obj last = NULL;
    int indexOfFirst = -1;

    for (int i = 0; i < childCount; ++i) {
        if (math_obj_isConstant(self->children[i])) {
            if (last == NULL) {
                last = self->children[i];
                self->children[i] = NULL;
                indexOfFirst = i;
            }
            else {
                math_obj newlast = __math_obj_eval_plus(last, self->children[i]);
                math_obj_free(last);
                math_obj_free(self->children[i]);
                self->children[i] = NULL;

                last = newlast;
            }
        }
    }

    if (last != NULL) {
        assert(math_obj_isConstant(last));
        long int val = math_obj_mvalue_getAsLong(last);
        if (val == 0) {
            math_obj_free(last);
            self->children[indexOfFirst] = NULL;
        }
        else {
            self->children[indexOfFirst] = last;
        }
        
    }

    self->children = __math_obj_takeOutNull(self->children);

    if (len(self->children) == 1) {
        // only one left
        math_obj tmp = self->children[0];
        self->children[0] = NULL;
        math_obj_free(self);
        return tmp;
    }
    else if (len(self->children) == 0) {
        // nothing left
        math_obj tmp = buildMathObjectConstantLong(0);
        math_obj_free(self);
        return tmp;
    }

    return self;
}

math_obj math_obj_simplify_product(math_obj self) {
    assert(self->typeTag == PRODUCT);
    int childCount = len(self->children);

    assert(childCount > 1);
    // analyze for possible simplification
    math_obj last = NULL;
    int indexOfFirst = -1;

    for (int i = 0; i < childCount; ++i) {
        if (math_obj_isConstant(self->children[i])) {

            long int val = math_obj_mvalue_getAsLong(self->children[i]);
            
            if (val == 0) {
                // everything is zero
                math_obj_free(self);
                return buildMathObjectConstantLong(0);
            }

            if (last == NULL) {
                last = self->children[i];
                self->children[i] = NULL;
                indexOfFirst = i;
            }
            else {
                math_obj newlast = __math_obj_eval_product(last, self->children[i]);
                math_obj_free(last);
                math_obj_free(self->children[i]);
                self->children[i] = NULL;

                last = newlast;
            }
        }
    }

    if (last != NULL) {
        assert(math_obj_isConstant(last));
        long int val = math_obj_mvalue_getAsLong(last);
        if (val == 1) {
            math_obj_free(last);
            self->children[indexOfFirst] = NULL;
        }
        else {
            self->children[indexOfFirst] = last;
        }
        
    }

    self->children = __math_obj_takeOutNull(self->children);

    if (len(self->children) == 1) {
        // only one left
        math_obj tmp = self->children[0];
        self->children[0] = NULL;
        math_obj_free(self);
        return tmp;
    }
    else if (len(self->children) == 0) {
        // nothing left
        math_obj tmp = buildMathObjectConstantLong(1);
        math_obj_free(self);
        return tmp;
    }

    return self;
}

math_obj math_obj_simplify_fraction(math_obj self) {
    assert(self->typeTag == FRACTION);
    int childCount = len(self->children);
    assert(childCount == 2);

    math_obj num = self->children[0];
    math_obj denom = self->children[1];

    if (math_obj_isConstant(num) && math_obj_mvalue_getAsLong(num) == 0) {
        math_obj_free(self);
        return buildMathObjectConstantLong(0);
    }
    else if (math_obj_isConstant(denom) && math_obj_mvalue_getAsLong(denom) == 1) {
        self->children[0] = NULL;
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
    assert(self != NULL);

    int childCount = len(self->children);

    for (int i = 0; i < childCount; ++i) {
        self->children[i] = math_obj_eval(self->children[i]);
    }

    if (childCount == 0) {
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