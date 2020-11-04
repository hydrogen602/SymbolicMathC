#ifndef AVL_TREE_H_
#define AVL_TREE_H_

#include <stddef.h>
#include <stdbool.h>

typedef struct AVL_TREE_NODE {
    struct AVL_TREE_NODE * left;
    struct AVL_TREE_NODE * right;
    const char * value;
    int key;
    unsigned int height;
} AVLTreeNode;

typedef struct AVL_TREE {
    AVLTreeNode * root;
    unsigned int length;
    unsigned int height;
} AVLTree;

typedef struct __AVL_TREE_DETALS {
    unsigned int height;
    unsigned int count;
    double bestHeight;
} AVLTreeDetails_t;

typedef struct AVL_TREE_ITERATOR {
    AVLTreeNode ** __parents;
    AVLTreeNode * it;
    int __stackIndex;
} AVLTreeIterator;

void set_avltree_silent(bool s);

AVLTree build_AVLTree();

void avl_tree_free(AVLTree * tree);

void free_AVLTreeNode(AVLTreeNode * ptr);

int avl_tree_add(AVLTree * tree, int key, const char * value);

AVLTreeNode * avl_tree_getNode(AVLTree * tree, int key);

AVLTreeIterator avl_tree_iterate(AVLTree * tree);

void avl_tree_iter_next(AVLTreeIterator * iter);

//AVLTreeNode * avl_tree_removeNode(AVLTree * tree, int key);

/* =============================================
 *                 Debug Stuff 
 * =============================================
 */

void avl_tree_printer(AVLTree * tree, bool displayBalance);

void avl_tree_print_details(AVLTree * tree);

AVLTreeDetails_t avl_tree_get_details(AVLTreeNode * node);

#endif