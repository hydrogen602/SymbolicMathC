#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <math.h>
#include "avltree.h"

int __avl_tree_add(AVLTree * tree, AVLTreeNode * newNode);
AVLTreeNode * __build_AVLTreeNode(int key, const char * value);
void __free_AVLTreeNode(AVLTreeNode * ptr);
int __avl_tree_add(AVLTree * tree, AVLTreeNode * newNode);
void __avl_tree_printer(AVLTreeNode * node, int depth, bool displayBalance);


bool avltree_silent = false;

void set_avltree_silent(bool s) {
    avltree_silent = s;
}

AVLTree build_AVLTree() {
    AVLTree t;
    t.length = 0;
    t.root = NULL;
    return t;
}

AVLTreeNode * __build_AVLTreeNode(int key, const char * value) {
    AVLTreeNode * ptr = malloc(sizeof(AVLTreeNode));
    ptr->value = value;
    ptr->key = key;
    ptr->balanceFactor = 0;
    ptr->left = NULL;
    ptr->right = NULL;
    return ptr;
}

void free_AVLTreeNode(AVLTreeNode * ptr) {
    if (ptr == NULL) {
        return;
    }
    free_AVLTreeNode(ptr->left);
    free_AVLTreeNode(ptr->right);
    free(ptr);
}

void avl_tree_free(AVLTree * tree) {
    free_AVLTreeNode(tree->root);
    tree->length = 0;
    tree->root = NULL;
}

int avl_tree_add(AVLTree * tree, int key, const char * value) {
    AVLTreeNode * newNode = __build_AVLTreeNode(key, value);
    int result = __avl_tree_add(tree, newNode);
    if (result != EXIT_SUCCESS) {
        free_AVLTreeNode(newNode); // free the memory again
    }
    return result;
}

/*
 *
 * Returns 1 (EXIT_FAILURE) if the node cannot be added
 * Returns 0 (EXIT_SUCCESS) if the node was added and the height is the same
 * Returns -1 if the node was added and the height changed
 */
int __avl_tree_add_helper(AVLTreeNode * parent, AVLTreeNode * newNode) {
    bool isLeaf = parent->left == NULL && parent->right == NULL;

    // if things go towards zero, balance factor doesn't need to be propagated

    if (newNode->key < parent->key) {
        // ========= LEFT =========

        // (parent->balanceFactor)--;
        if (parent->left == NULL) {
            parent->left = newNode;
            
            (parent->balanceFactor)--;
            if (isLeaf) {
                
                return -1;
            }
        }
        else {
            int result = __avl_tree_add_helper(parent->left, newNode);
            if (result == -1) {
                (parent->balanceFactor)--;
                if (parent->balanceFactor < 0) {
                    return -1;
                }
            }
            else if (result == EXIT_FAILURE) {
                return EXIT_FAILURE;
            }
        }
    }
    else if (newNode->key > parent->key) {
        // ========= RIGHT =========

        // (parent->balanceFactor)++;
        if (parent->right == NULL) {
            parent->right = newNode;

            (parent->balanceFactor)++;
            if (isLeaf) {
                return -1;
            }
        }
        else {
            int result = __avl_tree_add_helper(parent->right, newNode);
            if (result == -1) {
                (parent->balanceFactor)++;
                if (parent->balanceFactor > 0) {
                    return -1;
                }
            }
            else if (result == EXIT_FAILURE) {
                return EXIT_FAILURE;
            }
        }
    }
    else {
        // ========= EQUAL =========
        if (!avltree_silent)
            fprintf(stderr, "Error: Key already exists: %d\n", newNode->key);
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

int __avl_tree_add(AVLTree * tree, AVLTreeNode * newNode) {
    if (tree->root == NULL) {
        tree->root = newNode;
        ++(tree->length);
    }
    else {
        int result = __avl_tree_add_helper(tree->root, newNode);
        if (result == EXIT_FAILURE) {
            return EXIT_FAILURE;
        }
        else {
            ++(tree->length);
        }
    }

    return EXIT_SUCCESS;
}

AVLTreeNode * avl_tree_getNode(AVLTree * tree, int key) {
    AVLTreeNode * ptr = tree->root;

    while (ptr != NULL && ptr->key != key) {
        if (key < ptr->key) {
            ptr = ptr->left;
        }
        else {
            ptr = ptr->right;
        }
    }

    if (ptr->key == key) {
        return ptr;
    }
    return NULL;
}

void __avl_tree_printer(AVLTreeNode * node, int depth, bool displayBalance) {
    if (node == NULL) {
        return;
    }

    __avl_tree_printer(node->right, depth + 1, displayBalance);
    for (int i = 0; i < depth; ++i) {
        printf("\t");
    }
    if (displayBalance) {
        printf("(%d) %d: %s\n", node->balanceFactor, node->key, node->value);
    }
    else {
        printf("%d: %s\n", node->key, node->value);
    }
    __avl_tree_printer(node->left, depth + 1, displayBalance);
}

void avl_tree_printer(AVLTree * tree, bool displayBalance) {
    __avl_tree_printer(tree->root, 0, displayBalance);
}

struct __AVL_TREE_DETALS avl_tree_get_details(AVLTreeNode * node) {
    if (node == NULL) {
        struct __AVL_TREE_DETALS data;
        data.count = 0;
        data.height = 0;
        return data;
    }
    else {
        struct __AVL_TREE_DETALS data;

        struct __AVL_TREE_DETALS dataLeft = avl_tree_get_details(node->left);
        struct __AVL_TREE_DETALS dataRight = avl_tree_get_details(node->right);

        unsigned int maxHeight = dataLeft.height;
        if (maxHeight < dataRight.height) {
            maxHeight = dataRight.height;
        }

        data.height = maxHeight + 1;
        data.count = dataLeft.count + dataRight.count + 1;

        return data;
    }
}

void avl_tree_print_details(AVLTree * tree) {
    struct __AVL_TREE_DETALS data = avl_tree_get_details(tree->root);
    printf("Node Count Actual = %d\n", data.count);
    printf("Node Count Tracked = %d\n", tree->length);
    printf("Tree height = %d\n", data.height);
    /*
     * sum of 2^i from 0 to h-1 => 2^h - 1
     * count = 2^h - 1
     * solve for h
     * h = log2(count + 1)
     */
    double optimalHeight = log2((double) data.count + 1);
    printf("Optimal Tree height = %.1f\n", optimalHeight);
}

