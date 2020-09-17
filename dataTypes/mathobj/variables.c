#include "variables.h"
#include "../../errors.h"
#include "mathobj.h"
#include "mathobjheader.h"
#include <stdio.h>

typedef struct Name_And_Value_Pair
{
    String name;
    math_obj value;
} varNameValuePair_t;

varNameValuePair_t * definitionsArray = NULL;
size_t currIndex = 0;
// newArray(10, sizeof(varNameValuePair_t));

void variables_add(String * name, math_obj m) {
    if (definitionsArray == NULL) {
        definitionsArray = newArray(10, sizeof(varNameValuePair_t));
    }

    if (str_getLen(name) == 0) {
        throw_error("Variable Name Missing", "");
    }

    for (int i = 0; i < currIndex; ++i) {
        if (str_isEqual(&definitionsArray[i].name, name)) {
            throw_error("Variable Already Exist", str_getString(name));
        }
    }

    varNameValuePair_t pair = {
        str_move(name),
        m
    };

    if (currIndex == len(definitionsArray)) {
        throw_error("Too Many Variables Defined", str_getString(name));
    }

    definitionsArray[currIndex] = pair;
    currIndex += 1;
}

math_obj variables_get(String * name) {
    if (definitionsArray == NULL) {
        return NULL;
    }

    if (str_getLen(name) == 0) {
        throw_error("Variable Name Missing", "");
    }

    for (int i = 0; i < len(definitionsArray); ++i) {
        if (str_isEqual(&definitionsArray[i].name, name)) {
            return math_obj_copy(definitionsArray[i].value);
        }
    }

    return NULL;
}

void variables_cleanup() {
    #if DEBUG
    printf("Cleaning up variable list\n");
    #endif
    if (definitionsArray == NULL) {
        return;
    }
    
    for (int i = 0; i < currIndex; ++i) {
        math_obj_free(definitionsArray->value);
        str_free(&definitionsArray->name);
    }
    freeArray(definitionsArray);
}