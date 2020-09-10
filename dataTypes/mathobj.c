#include "mathobj.h"
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include "header.h"


math_obj buildMathObjectNull() {
    math_obj m = malloc(sizeof(math_struct));
    if (m == NULL) {
        fprintf(stderr, "Could not allocate memory");
        exit(1);
    }
    m->label = buildStringNull();
    m->childA = NULL;
    m->childB = NULL;
    m->childCount = 0;
    m->typeTag = NOTHING;
    m->permValue.i = 0;
    m->permValueType = MATH_OBJ_NULL;
    
    return m;
}

void math_obj_free(math_obj self) {
    if (self == NULL) return;

    str_free(& (self->label));
    math_obj_free(self->childA);
    math_obj_free(self->childB);

    self->childCount = 0;
    self->typeTag = NOTHING;
    self->permValue.i = 0;
    self->permValueType = MATH_OBJ_NULL;
    free(self);
}

math_obj __buildMathObjectCustom(String s, math_obj a, math_obj b, int childCount, int typeTag) {
    math_obj m = buildMathObjectNull();

    m->label = s;
    m->childA = a;
    m->childB = b;
    m->childCount = childCount;
    m->typeTag = typeTag;
    m->permValue.i = 0;
    m->permValueType = MATH_OBJ_NULL;
    
    return m;
}

math_obj buildMathObjectVariable(String * label) {
    return __buildMathObjectCustom(str_move(label), NULL, NULL, 0, VARIABLE);
}

math_obj buildMathObjectConstant(String * label) {
    math_obj m = __buildMathObjectCustom(str_move(label), NULL, NULL, 0, CONSTANT);

    long int n = str_toInteger(label);

    m->permValueType = MATH_OBJ_LONG;
    m->permValue.i = n;

    return m;
}

bool math_obj_isConstant(math_obj self) {
    return self->typeTag == CONSTANT;
}

math_obj buildMathObjectPlus(math_obj a, math_obj b) {
    return __buildMathObjectCustom(buildString("+"), a, b, 2, PLUS);
}

math_obj buildMathObjectEquation(math_obj a, math_obj b) {
    return __buildMathObjectCustom(buildString("="), a, b, 2, PLUS);
}

void math_obj_printer(math_obj self) {
    if (self == NULL) {
        printf("NULL ");
    }
    else {
        if (self->childCount == 0) {
            str_print(& self->label);
            putchar(' ');
        }
        else if (self->childCount == 1)
        {
            str_print(& self->label);
            putchar(' ');

            math_obj_printer(self->childA);
        }
        else if (self->childCount == 2)
        {
            math_obj_printer(self->childA);

            str_print(& self->label);
            putchar(' ');

            math_obj_printer(self->childB);
        }
        else {
            fprintf(stderr, "Illegal value for childCount of struct Math_Object: Expected 0-2, but got %d at line %d in %s\n", self->childCount, __LINE__, __FILE__);
            exit(1);
        }
    }
}

math_obj __math_obj_eval_plus(math_obj self, math_obj other) {
    assert(self->typeTag == CONSTANT && other->typeTag == CONSTANT);


}

math_obj math_obj_eval(math_obj self) {
    assert(self != NULL);

    if (self->childCount == 0) {
        return self; // cant simplify one thing
    }
    elif (self->childCount == 1) {
        self->childA = math_obj_eval(self->childA);
        return self; // don't have any of these yet
    }
    elif (self->childCount == 2) {
        self->childA = math_obj_eval(self->childA);
        self->childB = math_obj_eval(self->childB);

        switch (self->typeTag)
        {
        case EQUATION:
            return self;
            break;
        case PLUS:
            // analyze for possible simplification
            if (math_obj_isConstant(self->childA) && math_obj_isConstant(self->childB)) {
                // can be added
                math_obj newObj = __math_obj_eval_plus(self->childA, self->childB);
                math_obj_free(self);

                return newObj;
            }

            return self;
            break;
        
        default:
            assert(false);
            break;
        }
    }
    else {
        assert(self->childCount <= 2); // def fails
    }
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