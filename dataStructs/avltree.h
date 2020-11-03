#ifndef AVL_TREE_H_
#define AVL_TREE_H_

#include <stddef.h>
#include <stdbool.h>

typedef struct AVL_TREE_NODE {
    struct AVL_TREE_NODE * left;
    struct AVL_TREE_NODE * right;
    const char * value;
    int key;
    char balanceFactor;
} AVLTreeNode;

typedef struct AVL_TREE {
    AVLTreeNode * root;
    unsigned int length;
} AVLTree;

struct __AVL_TREE_DETALS {
    int height;
    int count;
};

void set_avltree_silent(bool s);

AVLTree build_AVLTree();

void avl_tree_free(AVLTree * tree);

int avl_tree_add(AVLTree * tree, int key, const char * value);

AVLTreeNode * avl_tree_getNode(AVLTree * tree, int key);


/* =============================================
 *                 Debug Stuff 
 * =============================================
 */

void avl_tree_printer(AVLTree * tree, bool displayBalance);

void avl_tree_print_details(AVLTree * tree);

struct __AVL_TREE_DETALS avl_tree_get_details(AVLTreeNode * node);

#endif