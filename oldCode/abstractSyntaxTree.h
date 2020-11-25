#include "parseTable.h"

typedef struct __ABSTRACT_SYNTAX_TREE_NODE {
    struct __ABSTRACT_SYNTAX_TREE_NODE * children;
    struct __ABSTRACT_SYNTAX_TREE_NODE * parent;
    Symbol sym;
} ASTNode;

//int length = sizeof(ASTNode);

typedef struct __ABSTRACT_SYNTAX_TREE {
    ASTNode root;
} AST;

typedef ASTNode* ASTNodeArr;

AST ast_build();

ASTNode ast_new_node(Symbol sym);

int ast_add_children(ASTNode* parent, SymbolArr symbols);