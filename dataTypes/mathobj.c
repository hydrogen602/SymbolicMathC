#include "mathobj.h"
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include "header.h"
#include "array.h"


math_obj buildMathObjectNull() {
    math_obj m = malloc(sizeof(math_struct));
    if (m == NULL) {
        fprintf(stderr, "Could not allocate memory");
        exit(1);
    }
    m->label = buildStringNull();
    m->children = NULL;
    m->typeTag = NOTHING;
    m->permValue.i = 0;
    m->permValueType = MATH_OBJ_NULL;
    
    return m;
}

void math_obj_free(math_obj self) {
    if (self == NULL) return;

    str_free(& (self->label));

    for (int i = 0; i < len(self->children); ++i) {
        math_obj_free(self->children[i]);
    }
    freeArray(self->children);
    
    self->typeTag = NOTHING;
    self->permValue.i = 0;
    self->permValueType = MATH_OBJ_NULL;
    free(self);
}

math_obj __buildMathObjectCustom(String s, math_obj_array arr, int typeTag) {
    math_obj m = buildMathObjectNull();

    m->label = s;
    m->children = arr;
    m->typeTag = typeTag;
    m->permValue.i = 0;
    m->permValueType = MATH_OBJ_NULL;
    
    return m;
}

math_obj buildMathObjectVariable(String * label) {
    return __buildMathObjectCustom(str_move(label), NULL, VARIABLE);
}

math_obj buildMathObjectConstant(String * label) {
    math_obj m = __buildMathObjectCustom(str_move(label), NULL, CONSTANT);

    long int n = str_toInteger(& m->label);

    m->permValueType = MATH_OBJ_LONG;
    m->permValue.i = n;

    return m;
}

math_obj buildMathObjectConstantLong(long int n) {
    String label = buildStringFromInteger(n);
    math_obj m = __buildMathObjectCustom(str_move(&label), NULL, CONSTANT);

    m->permValueType = MATH_OBJ_LONG;
    m->permValue.i = n;

    return m;
}

bool math_obj_isConstant(math_obj self) {
    return self->typeTag == CONSTANT;
}

math_obj buildMathObjectPlus(math_obj_array arr) {
    return __buildMathObjectCustom(buildString("+"), arr, PLUS);
}

math_obj buildMathObjectNegate(math_obj m) {
    math_obj_array arr = newMathObjectArray(1);
    arr[0] = m;
    return __buildMathObjectCustom(buildString("-"), arr, NEGATE);
}

math_obj buildMathObjectEquation(math_obj a, math_obj b) {
    math_obj_array mArr = newMathObjectArray(2);
    mArr[0] = a;
    mArr[1] = b;
    return __buildMathObjectCustom(buildString("="), mArr, PLUS);
}

void math_obj_printer(math_obj self) {
    if (self == NULL) {
        printf("NULL ");
    }
    else {
        if (len(self->children) == 0) {
            str_print(& self->label);
            putchar(' ');
        }
        else if (len(self->children) == 1)
        {
            str_print(& self->label);
            //putchar(' ');

            math_obj_printer(self->children[0]);
        }
        else {
            math_obj_printer(self->children[0]);
            for (int i = 1; i < len(self->children); ++i) {
                str_print(& self->label);
                putchar(' ');

                math_obj_printer(self->children[i]);
            }
        }
    }
}

void math_obj_debug_printer(math_obj self) {
    if (self == NULL) {
        printf("NULL ");
    }
    else {
        if (len(self->children) == 0) {
            str_print(& self->label);
            putchar(' ');
        }
        else if (len(self->children) == 1)
        {
            printf("( ");
            str_print(& self->label);
            //putchar(' ');

            math_obj_debug_printer(self->children[0]);
            printf(") ");
        }
        else {
            printf("( ");
            math_obj_debug_printer(self->children[0]);
            for (int i = 1; i < len(self->children); ++i) {
                str_print(& self->label);
                putchar(' ');

                math_obj_debug_printer(self->children[i]);
            }
            printf(") ");
        }
    }
}

math_obj __math_obj_eval_plus(math_obj self, math_obj other) {
    assert(self->typeTag == CONSTANT && other->typeTag == CONSTANT);
    assert(self->permValueType == MATH_OBJ_LONG && other->permValueType == MATH_OBJ_LONG);

    long int val = self->permValue.i + other->permValue.i;

    #if DEBUG
    printf("%ld\n", val);
    #endif

    return buildMathObjectConstantLong(val);
}

math_obj __math_obj_eval_negate(math_obj self) {
    assert(self->typeTag == CONSTANT);
    assert(self->permValueType == MATH_OBJ_LONG);

    long int val = -1 * self->permValue.i;

    #if DEBUG
    printf("%ld\n", val);
    #endif

    return buildMathObjectConstantLong(val);
}

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

math_obj math_obj_eval(math_obj self) {
    // if not returning self, free self!!!

    assert(self != NULL);

    int childCount = len(self->children);

    if (childCount == 0) {
        return self; // cant simplify one thing
    }
    elif (childCount == 1) {
        self->children[0] = math_obj_eval(self->children[0]);

        switch (self->typeTag)
        {
        case NEGATE:
            assert(childCount == 1);

            if (math_obj_isConstant(self->children[0])) {
                math_obj newSelf = __math_obj_eval_negate(self->children[0]);

                math_obj_free(self);
                return newSelf;
            }

            return self;
            break;
        
        default:
            assert(false);
            break;
        }

        return self; // don't have any of these yet
    }
    else {
        for (int i = 0; i < childCount; ++i) {
            self->children[i] = math_obj_eval(self->children[i]);
        }

        switch (self->typeTag)
        {
        case EQUATION:
            return self;
            break;
        case PLUS:
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
                assert(last->permValueType == MATH_OBJ_LONG);
                if (last->permValue.i == 0) {
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
            break;
        
        default:
            assert(false);
            break;
        }
    }

    return self;
}




// math_obj math_obj_move(math_obj * self) {
//     // deletes the old to avoid dangling pointers
//     math_obj m = {
//         str_move(&(self->label)),
//         self->childA,
//         self->childB,
//         self->childCount,
//         self->typeTag
//     };

//     self->childA = NULL;
//     self->childB = NULL;
//     self->childCount = 0;
//     self->typeTag = NOTHING;

//     return m;
// }