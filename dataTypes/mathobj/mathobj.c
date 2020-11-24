#include "mathobj.h"
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <ctype.h>
#include "../../errors.h"
#include "types/typedefs.h"

/*
typedef union __MATH_OBJ_NODE_DATA
{
    struct Math_Object ** children; // node - operator
    union __MATH_OBJ_VALUE permValue; // leaf - constant
    String label; // leaf - variable
};
*/

inline bool __usesLabel(math_type t) {
    return t == VARIABLE;
}

inline bool __usesPermValue(math_type t) {
    return t == CONSTANT_DOUBLE || t == CONSTANT_LONG;
}

inline bool __usesChildren(math_type t) {
    return !__usesPermValue(t) && !__usesLabel(t);
}

inline bool math_obj_isConstant(math_obj self) {
    return __usesPermValue(self->typeTag);
}

inline math_obj_array math_obj_getChildren(math_obj self) {
    if (__usesChildren(self->typeTag)) {
        return self->data.children;
    }
    return NULL;
}

math_obj __buildMathObjectNull() {
    math_obj m = malloc(sizeof(math_struct));
    if (m == NULL) {
        fprintf(stderr, "Could not allocate memory");
        exit(1);
    }
    m->typeTag = NOTHING;
    m->data.children = NULL;
    
    return m;
}

void math_obj_free(math_obj self) {
    if (self == NULL) return;

    if (__usesLabel(self->typeTag)) {
        str_free(&self->data.label);
    }
    elif (__usesChildren(self->typeTag)) {
        for (unsigned int i = 0; i < len(self->data.children); ++i) {
            math_obj_free(self->data.children[i]);
        }
        freeArray(self->data.children);
        self->data.children = NULL;
    }
    
    self->typeTag = NOTHING;
    self->data.permValue.i = 0;
    free(self);
}

math_obj math_obj_copy(math_obj self) {
    if (self == NULL) return NULL;

    if (usesChildren(self)) {
        math_obj_array newArr = newMathObjectArray(len(self->data.children));
        for (unsigned int i = 0; i < len(self->data.children); ++i) {
            newArr[i] = math_obj_copy(self->data.children[i]);
        }
        return __buildMathObjectOperatorLike(newArr, self->typeTag);
    }
    elif (usesLabel(self)) {
        String s = str_copy(&self->data.label);
        return __buildMathObjectVarLike(&s, self->typeTag);
    }
    elif (usesPermValue(self)) {
        return __buildMathObjectConstantLike(self->data.permValue, self->typeTag);
    }
    else {
        assert(false);
    }
    return NULL;
}

math_obj_array buildMathObjectArrayFrom2(math_obj a, math_obj b) {
    math_obj_array arr = newArray(2, sizeof(math_obj));
    arr[0] = a;
    arr[1] = b;
    return arr;
}

math_obj __buildMathObjectVarLike(String * label, math_type typeTag) {
    char * c = str_getString(label);
    if (strcmp(c, "define") == 0) {
        throw_error("Invalid Variable Name", str_getString(label));
    }
    size_t len = str_getLen(label);
    if (len <= 0) {
        throw_error("Zero length variable", str_getString(label));
    }
    elif (!isalpha(c[0])) {
        throw_error("Variables must begin with a letter", c);
    }

    for (unsigned int i = 0; i < len; ++i) {
        if (!isalnum(c[i]) && c[i] != '_') {
            throw_error("Invalid char in variable name", c);
        }
    }

    assert(__usesLabel(typeTag));
    math_obj m = __buildMathObjectNull();
    m->typeTag = typeTag;
    m->data.label = str_move(label);
    return m;
}

math_obj __buildMathObjectOperatorLike(math_obj_array arr, math_type typeTag) {
    assert(__usesChildren(typeTag));
    math_obj m = __buildMathObjectNull();
    m->typeTag = typeTag;
    m->data.children = arr;
    return m;
}

math_obj __buildMathObjectConstantLike(union __MATH_OBJ_VALUE val, math_type typeTag) {
    assert(__usesPermValue(typeTag));
    math_obj m = __buildMathObjectNull();
    m->typeTag = typeTag;
    m->data.permValue = val;
    return m;
}

void math_obj_printer(math_obj self) {
    if (self == NULL) {
        printf("NULL ");
    }
    elif (usesChildren(self)) {
        if (len(self->data.children) == 0) {
            putchar(math_obj_getOpSymbol(self->typeTag));
            putchar(' ');
        }
        else if (len(self->data.children) == 1)
        {
            putchar(math_obj_getOpSymbol(self->typeTag));
            //putchar(' ');

            math_obj_printer(self->data.children[0]);
        }
        else {
            math_obj_printer(self->data.children[0]);
            for (unsigned int i = 1; i < len(self->data.children); ++i) {
                putchar(math_obj_getOpSymbol(self->typeTag));
                putchar(' ');

                math_obj_printer(self->data.children[i]);
            }
        }
    }
    elif (usesLabel(self)) {
        str_print(&self->data.label);
        putchar(' ');
    }
    elif (usesPermValue(self)) {
        if (self->typeTag == CONSTANT_LONG) {
            String s = buildStringFromInteger(math_obj_mvalue_getAsLong(self));
            str_print(&s);
            str_free(&s);
        }
        elif (self->typeTag == CONSTANT_DOUBLE) {
            String s = buildStringFromDouble(math_obj_mvalue_getAsDouble(self));
            str_print(&s);
            str_free(&s);
        }
        else {
            assert(false);
        }
    }
}

void math_obj_debug_printer(math_obj self) {
    if (self == NULL) {
        printf("NULL ");
    }
    elif (usesChildren(self)) {
        printf("( ");
        if (len(self->data.children) == 0) {
            putchar(math_obj_getOpSymbol(self->typeTag));
            putchar(' ');
        }
        else if (len(self->data.children) == 1)
        {
            putchar(math_obj_getOpSymbol(self->typeTag));
            //putchar(' ');

            math_obj_debug_printer(self->data.children[0]);
        }
        else {
            math_obj_debug_printer(self->data.children[0]);
            for (unsigned int i = 1; i < len(self->data.children); ++i) {
                putchar(math_obj_getOpSymbol(self->typeTag));
                putchar(' ');

                math_obj_debug_printer(self->data.children[i]);
            }
        }
        printf(") ");
    }
    elif (usesLabel(self)) {
        str_print(&self->data.label);
        putchar(' ');
    }
    elif (usesPermValue(self)) {
        if (self->typeTag == CONSTANT_LONG) {
            String s = buildStringFromInteger(math_obj_mvalue_getAsLong(self));
            str_print(&s);
            str_free(&s);
        }
        elif (self->typeTag == CONSTANT_DOUBLE) {
            String s = buildStringFromDouble(math_obj_mvalue_getAsDouble(self));
            str_print(&s);
            str_free(&s);
        }
        else {
            assert(false);
        }
        putchar(' ');
    }
}

#define eprintf(...) for(int i = 0; i < depth; ++i) { fprintf(stderr, "  "); } fprintf(stderr, __VA_ARGS__)
#define eprintf_if(var, case) if ((var) == (case)) { fprintf(stderr, #case); }

void __math_obj_debug_dump_helper(math_obj m, int depth) {
    eprintf("Math Object\n");
    eprintf("typeTag: ");
    int tag = m->typeTag;
    eprintf_if(tag, NOTHING)
    else eprintf_if(tag, EQUATION)
    else eprintf_if(tag, CONSTANT_DOUBLE)
    else eprintf_if(tag, CONSTANT_LONG)
    else eprintf_if(tag, VARIABLE)
    else eprintf_if(tag, PLUS)
    else eprintf_if(tag, NEGATE)
    else eprintf_if(tag, PRODUCT)
    else eprintf_if(tag, FRACTION)
    else { eprintf("Unknown Type Tag"); }

    if (usesLabel(m))
        eprintf("label: %s\n", str_getString(&m->data.label));
    eprintf("\n");

    if (m->typeTag == NOTHING) {
        eprintf("permValueType: MATH_OBJ_NULL\n");
        eprintf("permValue: NULL\n");
    }
    elif (m->typeTag == CONSTANT_LONG) {
        eprintf("permValueType: MATH_OBJ_LONG\n");
        eprintf("permValue: %ld\n", m->data.permValue.i);
    }
    elif (m->typeTag == CONSTANT_DOUBLE) {
        eprintf("permValueType: MATH_OBJ_DOUBLE\n");
        eprintf("permValue: %f\n", m->data.permValue.f);
    }

    int childCount = len(math_obj_getChildren(m));
    eprintf("len(children): %d\n", childCount);

    if (childCount > 0) { eprintf("[\n"); }
    for (int i = 0; i < childCount; ++i) {
        __math_obj_debug_dump_helper(m->data.children[i], depth + 1);

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

char math_obj_getOpSymbol(math_type t) {
    switch (t)
    {
    case NOTHING:
        return 0;
    case EQUATION:
        return '=';
    case CONSTANT_LONG:
        return 0;
    case CONSTANT_DOUBLE:
        return 0;
    case VARIABLE:
        return 0;
    case PLUS:
        return '+';
    case NEGATE:
        return '-';
    case PRODUCT:
        return '*';
    case FRACTION:
        return '/';
    default:;
        char str[12];
        for (int i = 0; i < 12; ++i) {
            str[i] = 0;
        }
        snprintf(str, 12, "%d", t);
        throw_error("This shouldn\'t happen. Unknown type", str);
        return 0;
    }
}