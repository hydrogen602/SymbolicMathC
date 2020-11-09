#include "abstractSyntaxTree.h"
#include "array.h"
#include <stddef.h>
#include <stdlib.h>
#include "../errors.h"

AST ast_build() {
    AST t = { ast_new_node(NonTerm_S) };
    return t;
}

ASTNode ast_new_node(Symbol sym) {
    // ASTNode * node = malloc(sizeof(ASTNode));
    // if (node == NULL) {
    //     throw_error("Malloc failed", "Cannot allocate memory");
    // }
    ASTNode node;
    node.children = NULL;
    node.sym = sym;
    node.parent = NULL;
    return node;
}

// int ast_add(ASTNode* root, Symbol sym) {
//     ASTNode * astN = __ast_new_node(sym);
//     return ast_add_node(root, astN);
// }

int ast_add_children(ASTNode* parent, SymbolArr symbols) {
    for (unsigned int i = 0; i < len(symbols); ++i) {
        if (symbols[i] == Invalid_Symbol) {
            return EXIT_FAILURE;
        }
    }
    if (parent->children != NULL) {
        return EXIT_FAILURE; // already has children
    }

    parent->children = newArray(len(symbols), sizeof(ASTNode));

    for (unsigned int i = 0; i < len(symbols); ++i) {
        ASTNode node = ast_new_node(symbols[i]);
        node.parent = parent;
        parent->children[i] = node;
    }

    return EXIT_SUCCESS;
}