#define EVAL_EXTRA_DECL

#include "mathobj.h"
#include "evaluate.h"
#include "types/typedefs.h"


math_obj math_obj_simplify_negate(math_obj self) {
    assert(self->typeTag == NEGATE);
    assert(len(math_obj_getChildren(self)) == 1);

    if (math_obj_isConstant(self->data.children[0])) {
        math_obj newSelf = __math_obj_eval_negate(self->data.children[0]);

        math_obj_free(self);
        return newSelf;
    }
    elif (self->data.children[0]->typeTag == NEGATE) {
        assert(usesChildren(self->data.children[0]));
        assert(len(math_obj_getChildren(self)) == 1);
        math_obj newSelf = self->data.children[0]->data.children[0];
        self->data.children[0]->data.children[0] = NULL;

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