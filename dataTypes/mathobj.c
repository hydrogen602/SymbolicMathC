#include "mathobj.h"
#include <stdio.h>


math_obj buildMathObjectNull() {
    math_obj m = malloc(sizeof(math_struct));
    puts("malloc");
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

    printf("Freeing stuff\n");

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

    printf("created with label %s\n", str_getString(&s));
    
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