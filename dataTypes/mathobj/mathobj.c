#include "mathobj.h"
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <ctype.h>
#include "../../errors.h"
#include "multivalue.h"

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

    #if DEBUG
    printf("debug: %s\n", str_getString(& m->label));
    #endif
    
    return m;
}

math_obj math_obj_copy(math_obj self) {
    if (self == NULL) return NULL;

    math_obj_array newArr = newMathObjectArray(len(self->children));
    for (int i = 0; i < len(self->children); ++i) {
        newArr[i] = math_obj_copy(self->children[i]);
    }

    math_obj newObj = __buildMathObjectCustom(str_copy(&self->label), newArr, self->typeTag);

    newObj->permValue = self->permValue;
    newObj->permValueType = self->permValueType;

    return newObj;
}

math_obj buildMathObjectVariable(String * label) {
    char * c = str_getString(label);
    if (strcmp(c, "define") == 0) {
        throw_error("Invalid Variable Name", str_getString(label));
    }
    size_t len = str_getLen(label);

    assert(len > 0);

    if (!isalpha(c[0])) {
        throw_error("Invalid Variable Name", c);
    }

    for (int i = 0; i < len; ++i) {
        if (!isalnum(c[i]) && c[i] != '_') {
            throw_error("Invalid Variable Name", c);
        }
    }


    return __buildMathObjectCustom(str_move(label), NULL, VARIABLE);
}

math_obj buildMathObjectConstant(String * label) {
    math_obj m = __buildMathObjectCustom(str_move(label), NULL, CONSTANT);

    assert(str_getLen(& m->label) > 0);

    if (str_isInteger(& m->label)) {
        long int n = str_toInteger(& m->label);

        m->permValueType = MATH_OBJ_LONG;
        m->permValue.i = n;
    }
    else {
        if (str_contains(& m->label, 'e') || str_contains(& m->label, 'E')) {
            throw_error("Invalid number", str_getString(& m->label));
        }
        double d = str_toDouble(& m->label);

        m->permValueType = MATH_OBJ_DOUBLE;
        m->permValue.f = d;
    }
    

    return m;
}

math_obj buildMathObjectConstantLong(long int n) {
    String label = buildStringFromInteger(n);
    math_obj m = __buildMathObjectCustom(str_move(&label), NULL, CONSTANT);

    m->permValueType = MATH_OBJ_LONG;
    m->permValue.i = n;

    return m;
}

math_obj buildMathObjectConstantDouble(double n) {
    String label = buildStringFromDouble(n);
    math_obj m = __buildMathObjectCustom(str_move(&label), NULL, CONSTANT);

    m->permValueType = MATH_OBJ_DOUBLE;
    m->permValue.f = n;

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

math_obj buildMathObjectProduct(math_obj_array arr) {
    return __buildMathObjectCustom(buildString("*"), arr, PRODUCT);
}

math_obj buildMathObjectFraction(math_obj n, math_obj d) {
    math_obj_array arr = newMathObjectArray(2);
    arr[0] = n;
    arr[1] = d;

    if (math_obj_isConstant(d) && math_obj_mvalue_isEqualToLong(d, 0)) {
        throw_error("Division By Zero", str_getString(&d->label));
    }
    // check if d is zero
    return __buildMathObjectCustom(buildString("/"), arr, FRACTION);
}

math_obj buildMathObjectEquation(math_obj a, math_obj b) {
    math_obj_array mArr = newMathObjectArray(2);
    mArr[0] = a;
    mArr[1] = b;
    return __buildMathObjectCustom(buildString("="), mArr, EQUATION);
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

#define eprintf(...) for(int i = 0; i < depth; ++i) { fprintf(stderr, "  "); } fprintf(stderr, __VA_ARGS__)
#define eprintf_if(var, case) if ((var) == (case)) { fprintf(stderr, #case); }

void __math_obj_debug_dump_helper(math_obj m, int depth) {
    eprintf("Math Object\n");
    eprintf("label: %s\n", str_getString(&m->label));
    eprintf("typeTag: ");
    int tag = m->typeTag;
    eprintf_if(tag, NOTHING)
    else eprintf_if(tag, EQUATION)
    else eprintf_if(tag, CONSTANT)
    else eprintf_if(tag, VARIABLE)
    else eprintf_if(tag, PLUS)
    else eprintf_if(tag, NEGATE)
    else eprintf_if(tag, PRODUCT)
    else eprintf_if(tag, FRACTION)
    else { eprintf("Unknown Type Tag"); }
    eprintf("\n");

    int valueType = m->permValueType;
    if (valueType == MATH_OBJ_NULL) {
        eprintf("permValueType: MATH_OBJ_NULL\n");
        eprintf("permValue: NULL\n");
    }
    elif (valueType == MATH_OBJ_LONG) {
        eprintf("permValueType: MATH_OBJ_LONG\n");
        eprintf("permValue: %ld\n", m->permValue.i);
    }
    elif (valueType == MATH_OBJ_DOUBLE) {
        eprintf("permValueType: MATH_OBJ_DOUBLE\n");
        eprintf("permValue: %f\n", m->permValue.f);
    }

    int childCount = len(m->children);
    eprintf("len(children): %d\n", childCount);

    if (childCount > 0) { eprintf("[\n"); }
    for (int i = 0; i < childCount; ++i) {
        __math_obj_debug_dump_helper(m->children[i], depth + 1);

        if (i + 1 != childCount) {
            // not last one
            eprintf(",\n");
        }
    }
    if (childCount > 0) { eprintf("]\n"); }
}

void math_obj_debug_dump(math_obj m) {
    __math_obj_debug_dump_helper(m, 0);
}