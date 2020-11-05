#include "variables.h"
#include "../../errors.h"
#include "mathobj.h"
#include "mathobjheader.h"
#include <stdio.h>
#include "../../dataStructs/str.h"

#define KEY_T String
#define VAL_T math_obj
#define __compKeys(a, b) str_cmp(&(a), &(b))
#define SILENCE_PRINTER

#include "../../dataStructs/avltree.h"

// typedef struct Name_And_Value_Pair
// {
//     String name;
//     math_obj value;
// } varNameValuePair_t;

// varNameValuePair_t * definitionsArray = NULL;
// size_t currIndex = 0;
// newArray(10, sizeof(varNameValuePair_t));

AVLTree map = BLANK_AVL_TREE;

void variables_add(String * name, math_obj m) {
    set_avltree_silent(true);
    if (str_getLen(name) == 0) {
        throw_error("Variable Name Missing", "");
    }

    int result = avl_tree_add(&map, *name, m);
    if (result == EXIT_FAILURE) {
        str_free(name);
        throw_error("Variable Already Exist", str_getString(name));
    }
}

math_obj variables_get(String * name) {
    if (str_getLen(name) == 0) {
        throw_error("Variable Name Missing", "");
    }

    AVLTreeNode * node = avl_tree_getNode(&map, *name);
    if (node != NULL) {
        return math_obj_copy(node->value);
    }

    return NULL;
}

void variables_cleanup() {
    #if DEBUG
    printf("Cleaning up variable list\n");
    #endif


    for (AVLTreeIterator iter = avl_tree_iterate(&map); iter.it != NULL; avl_tree_iter_next(&iter)) {

        math_obj_free(iter.it->value);
        str_free(&(iter.it->key));
    }

    avl_tree_free(&map);
}