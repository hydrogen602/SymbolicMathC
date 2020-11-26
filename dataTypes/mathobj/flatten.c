#include "mathobjheader.h"
#include "types/typedefs.h"
#include "mathobj.h"
#include "flatten.h"

math_obj math_obj_flatten_plus(math_obj self) {
    unsigned int childCount = len(math_obj_getChildren(self));
    assert(self->typeTag == PLUS);
    assert(childCount > 1);

    int newChildCount = 0;

    for (unsigned int i = 0; i < childCount; ++i) {
        math_obj child = self->data.children[i];
        if (child->typeTag == PLUS) {
            child = math_obj_flatten_plus(child);
            self->data.children[i] = child; // put possible change into tree

            newChildCount += len(math_obj_getChildren(child));
        }
        else {
            newChildCount += 1;
        }
    }

    // math_obj buildMathObjectPlus(math_obj_array arr);

    math_obj_array mArr = newMathObjectArray(newChildCount);
    unsigned int index = 0;

    for (unsigned int i = 0; i < childCount; ++i) {
        math_obj child = self->data.children[i];
        if (child->typeTag == PLUS) {
            //child = math_obj_flatten_plus(child);
            unsigned int grandchildrenCount = len(math_obj_getChildren(child));
            for (unsigned int j = 0; j < grandchildrenCount; ++j) {
                mArr[index] = child->data.children[j];
                child->data.children[j] = NULL;
                ++index;
            }
            math_obj_free(child);
        }
        else {
            mArr[index] = self->data.children[i];
            ++index;
        }
        self->data.children[i] = NULL;
    }
    math_obj_free(self);

    return buildMathObjectPlus(mArr);

}