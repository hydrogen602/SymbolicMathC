#include "mathobj.h"

math_obj buildMathObjectNull() {
    math_obj m = {
        buildStringNull(),
        NULL,
        NULL,
        0,
        NOTHING
    };
    return m;
}

math_obj buildMathObjectVariable(String * label) {
    math_obj m = {
        str_move(label),
        NULL,
        NULL,
        0,
        VARIABLE
    };
    return m;
}

math_obj buildMathObjectPlus(math_obj * a, math_obj * b) {
    math_obj m = {
        buildString("+"),
        a,
        b,
        2,
        PLUS
    };
    return m;
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