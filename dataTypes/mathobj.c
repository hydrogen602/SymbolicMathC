#include "mathobj.h"
#include <stdio.h>


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
    
    return m;
}

void math_obj_free(math_obj self) {
    if (self == NULL) return;

    str_free(& (self->label));
    math_obj_free(self->childA);
    math_obj_free(self->childB);

    self->childCount = 0;
    self->typeTag = NOTHING;
    free(self);
}

math_obj __buildMathObjectCustom(String s, math_obj a, math_obj b, int childCount, int typeTag) {
    math_obj m = buildMathObjectNull();

    m->label = s;
    m->childA = a;
    m->childB = b;
    m->childCount = childCount;
    m->typeTag = typeTag;
    
    return m;
}

math_obj buildMathObjectVariable(String * label) {
    return __buildMathObjectCustom(str_move(label), NULL, NULL, 0, VARIABLE);
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