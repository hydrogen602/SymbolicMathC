#include "functions.h"
#include "../dataStructs/exception.h"
#include "../dataStructs/array.h"
#include "mathobj/flatten.h"

#define VAL_T function
#include "../dataStructs/arrayList.h" // decleartions
#define INCLUDE_DEFINITIONS_ARRAYLIST
#include "../dataStructs/arrayList.h" // definitions

ArrayList_function builtins = ARRAY_LIST_NULL_INIT;

function buildFunction(String *name, math_obj (*func)(math_obj)) {
    function f;
    f.name = str_move(name);
    f.func = func;
    return f;
}

void function_addBuiltin(function func) {
    for (unsigned i = 0; i < builtins.length; ++i) {
        if (str_isEqual(&builtins.list[i].name, &func.name)) {
            exception("Builtin func name taken", str_getString(&func.name));
        }
    }
    arrLs_append_function(&builtins, func);
}

void function_init() {
    String s = buildString("len");
    function_addBuiltin(buildFunction(&s, builtin_getChildrenCount));

    s = buildString("distribute");
    function_addBuiltin(buildFunction(&s, builtin_distribute));
}

void function_cleanup() {
    while (builtins.length > 0) {
        function f = arrLs_pop_function(&builtins);
        str_free(&f.name);
    }
    freeArrayList_function(&builtins);
}

math_obj function_callBuiltin(String *name, math_obj input) {
    for (unsigned i = 0; i < builtins.length; ++i) {
        if (str_isEqual(&builtins.list[i].name, name)) {
            return builtins.list[i].func(input);
        }
    }
    return NULL;
}

math_obj builtin_getChildrenCount(math_obj m) {
    long n = (long) len(math_obj_getChildren(m));
    math_obj_free(m);

    return buildMathObjectConstantLong(n);
}

math_obj builtin_distribute(math_obj m) {
    math_obj_array children = math_obj_getChildren(m);
    // for (unsigned i = 0; i < len(children); ++i) {
    //     children[i] = builtin_expand(children[i]);
    // }

    switch (m->typeTag)
    {
    case PLUS:
        return math_obj_flatten_plus(m);
    case NEGATE:
        assert(len(children) == 1);
        math_obj grandchild = children[0];
        if (grandchild->typeTag == PRODUCT) {
            grandchild->data.children[0] = buildMathObjectNegate(grandchild->data.children[0]);
            children[0] = NULL;
            math_obj_free(m);
            m = grandchild;
        }
        elif (grandchild->typeTag == PLUS) {
            math_obj_array greatgrandchildren = math_obj_getChildren(grandchild);
            for (unsigned i = 0; i < len(greatgrandchildren); ++i) {
                greatgrandchildren[i] = buildMathObjectNegate(greatgrandchildren[i]);
            }
            children[0] = NULL;
            math_obj_free(m);
            m = grandchild;
        }
        
        return math_obj_eval(m);

    default:
        break;
    }

    return math_obj_eval(m);
}