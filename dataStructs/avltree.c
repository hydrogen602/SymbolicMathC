#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <math.h>
#include "avltree.h"
#include "array.h"

#define heightDiff(parent) ( ( ((parent)->right == NULL) ? 0 : (int)((parent)->right->height) ) - ( ((parent)->left == NULL) ? 0 : (int)((parent)->left->height) ) )


AVLTreeNode * __rotateRight(AVLTreeNode * parent);
AVLTreeNode * __rotateLeft(AVLTreeNode * parent);
int __compKeys(int a, int b);
AVLTreeNode * __build_AVLTreeNode(int key, const char * value);
unsigned int __avl_tree_getHeight(AVLTreeNode * node);
int __avl_tree_add_helper(AVLTreeNode ** parentPtr, AVLTreeNode * newNode);
int __avl_tree_add(AVLTree * tree, AVLTreeNode * newNode);
void __avl_tree_printer(AVLTreeNode * node, int depth, bool displayBalance);


bool avltree_silent = false;

/*
https://www.geeksforgeeks.org/avl-tree-set-1-insertion/
    T1, T2 and T3 are subtrees of the tree 
    rooted with y (on the left side) or x (on 
    the right side)           
         y                               x
        / \     Right Rotation          /  \
       x   T3   - - - - - - - >        T1   y 
      / \       < - - - - - - -            / \
     T1  T2     Left Rotation            T2  T3
*/

void set_avltree_silent(bool s) {
    avltree_silent = s;
}

AVLTreeNode * __rotateRight(AVLTreeNode * parent) {
    AVLTreeNode * y = parent;
    AVLTreeNode * x = y->left; 

    if (x == NULL) {
        fprintf(stderr, "How is this supposed to be right rotated?\n");
        exit(EXIT_FAILURE);
    }

    AVLTreeNode * t2 = x->right;
    x->right = y;
    y->left = t2;

    y->height = __avl_tree_getHeight(y);
    x->height = __avl_tree_getHeight(x);

    return x;
}

AVLTreeNode * __rotateLeft(AVLTreeNode * parent) {
    AVLTreeNode * x = parent;
    AVLTreeNode * y = x->right; 

    if (y == NULL) {
        fprintf(stderr, "How is this supposed to be left rotated?\n");
        exit(EXIT_FAILURE);
    }

    AVLTreeNode * t2 = y->left;
    y->left = x;
    x->right = t2;

    x->height = __avl_tree_getHeight(x);
    y->height = __avl_tree_getHeight(y);

    return y;
}

/*
 * Makes it easier to switch between
 * key types
 */
int __compKeys(int a, int b) {
    return a - b;
}

/* ====================== Main Funcs ====================== */

AVLTree build_AVLTree() {
    AVLTree t;
    t.length = 0;
    t.root = NULL;
    t.height = 0;
    return t;
}

AVLTreeNode * __build_AVLTreeNode(int key, const char * value) {
    AVLTreeNode * ptr = malloc(sizeof(AVLTreeNode));
    ptr->value = value;
    ptr->key = key;
    ptr->height = 1;
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
    tree->height = 0;
}

int avl_tree_add(AVLTree * tree, int key, const char * value) {
    AVLTreeNode * newNode = __build_AVLTreeNode(key, value);
    int result = __avl_tree_add(tree, newNode);
    if (result != EXIT_SUCCESS) {
        free_AVLTreeNode(newNode); // free the memory again
    }
    return result;
}

unsigned int __avl_tree_getHeight(AVLTreeNode * node) {
    if (node->left == NULL && node->right == NULL) {
        return 1;
    }
    else if (node->left == NULL) {
        return node->right->height + 1;
    }
    else if (node->right == NULL) {
        return node->left->height + 1;
    }
    else {
        unsigned int x = node->left->height;
        if (x < node->right->height) {
            x = node->right->height;
        }
        return x + 1;
    }
}

/*
 *
 * Returns 1 (EXIT_FAILURE) if the node cannot be added
 * Returns 0 (EXIT_SUCCESS) if the node was added and the height is the same
 * Returns -1 if the node was added and the height changed
 */
int __avl_tree_add_helper(AVLTreeNode ** parentPtr, AVLTreeNode * newNode) {
    AVLTreeNode * parent = *parentPtr;
    // if things go towards zero, balance factor doesn't need to be propagated
    if (__compKeys(newNode->key, parent->key) < 0) {
        // ========= LEFT =========

        // (parent->balanceFactor)--;
        if (parent->left == NULL) {
            parent->left = newNode;
        }
        else {
            int result = __avl_tree_add_helper(&(parent->left), newNode);
            if (result == EXIT_FAILURE)
                return result;
        }
    }
    else if (__compKeys(newNode->key, parent->key) > 0) {
        // ========= RIGHT =========

        // (parent->balanceFactor)++;
        if (parent->right == NULL) {
            parent->right = newNode;
        }
        else {
            int result = __avl_tree_add_helper(&(parent->right), newNode);
            if (result == EXIT_FAILURE)
                return result;
        }
    }
    else {
        // ========= EQUAL =========
        if (!avltree_silent)
            fprintf(stderr, "Error: Key already exists: %d\n", newNode->key);
        return EXIT_FAILURE;
    }

    parent->height = __avl_tree_getHeight(parent);

    int diff = heightDiff(parent);

    /*
    IF tree is right heavy
    {
        IF tree's right subtree is left heavy
        {
            Perform Double Left rotation
        }
        ELSE
        {
            Perform Single Left rotation
        }
    }
     */
    if (diff > 1) {
        //puts("right heavy");

        // rotate left
        if (parent->right != NULL && heightDiff(parent->right) < 0) {
            // Double left
            parent->right = __rotateRight(parent->right);
        }
        
        (*parentPtr) = __rotateLeft(parent);
    }
    
    /*
    ELSE IF tree is left heavy
    {
        IF tree's left subtree is right heavy
        {
            Perform Double Right rotation
        }
        ELSE
        {
            Perform Single Right rotation
        }
    } 
     */
    else if (diff < -1) {
        //puts("left heavy");
        if (parent->left != NULL && heightDiff(parent->left) > 0) {
            // Double left
            parent->left = __rotateLeft(parent->left);
        }
        
        (*parentPtr) = __rotateRight(parent);
    }
    

    return EXIT_SUCCESS;
}

int __avl_tree_add(AVLTree * tree, AVLTreeNode * newNode) {
    if (tree->root == NULL) {
        tree->root = newNode;
        ++(tree->length);
    }
    else {
        int result = __avl_tree_add_helper(&(tree->root), newNode);
        if (result == EXIT_FAILURE) {
            return EXIT_FAILURE;
        }
        else {
            ++(tree->length);
        }
    }

    if (tree->root != NULL)
        tree->height = __avl_tree_getHeight(tree->root);
    else
        fprintf(stderr, "Error: Empty tree after insert, this should never happen!");

    return EXIT_SUCCESS;
}

AVLTreeNode * avl_tree_getNode(AVLTree * tree, int key) {
    AVLTreeNode * ptr = tree->root;

    while (ptr != NULL && ptr->key != key) {
        if (__compKeys(key, ptr->key) < 0) {
            ptr = ptr->left;
        }
        else {
            ptr = ptr->right;
        }
    }

    if (__compKeys(ptr->key, key) == 0) {
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
        printf("(%d) %d: %s\n", node->height, node->key, node->value);
    }
    else {
        printf("%d: %s\n", node->key, node->value);
    }
    __avl_tree_printer(node->left, depth + 1, displayBalance);
}

void avl_tree_printer(AVLTree * tree, bool displayBalance) {
    __avl_tree_printer(tree->root, 0, displayBalance);
}

AVLTreeDetails_t avl_tree_get_details(AVLTreeNode * node) {
    if (node == NULL) {
        AVLTreeDetails_t data;
        data.count = 0;
        data.height = 0;
        data.bestHeight = 0;
        return data;
    }
    else {
        AVLTreeDetails_t data;

        AVLTreeDetails_t dataLeft = avl_tree_get_details(node->left);
        AVLTreeDetails_t dataRight = avl_tree_get_details(node->right);

        unsigned int maxHeight = dataLeft.height;
        if (maxHeight < dataRight.height) {
            maxHeight = dataRight.height;
        }

        data.height = maxHeight + 1;
        data.count = dataLeft.count + dataRight.count + 1;
        /*
         * sum of 2^i from 0 to h-1 => 2^h - 1
         * count = 2^h - 1
         * solve for h
         * h = log2(count + 1)
         */
        data.bestHeight = log2((double) data.count + 1);

        return data;
    }
}

void avl_tree_print_details(AVLTree * tree) {
    AVLTreeDetails_t data = avl_tree_get_details(tree->root);
    printf("Node count actual = %d\n", data.count);
    printf("Node count tracked = %d\n", tree->length);
    printf("Tree height actual = %d\n", data.height);
    printf("Tree height tracked = %d\n", tree->height);
    printf("Optimal Tree height = %.1f\n", data.bestHeight);
}

AVLTreeIterator avl_tree_iterate(AVLTree * tree) {
    AVLTreeIterator iter;
    iter.it = tree->root;

    if (iter.it == NULL) {
        iter.__parents = NULL;
        iter.__stackIndex = -1;
    }
    else {
        iter.__parents = newArray(tree->height, sizeof(AVLTreeNode *));
        iter.__stackIndex = 0;
        while (iter.it->left != NULL) {
            iter.__parents[iter.__stackIndex] = iter.it;
            (iter.__stackIndex)++;
            iter.it = iter.it->left;
        }
    }

    return iter;
}

void avl_tree_iter_next(AVLTreeIterator * iter) {
    if (iter->it == NULL) {
        fprintf(stderr, "Called next on empty iterator\n");
        exit(EXIT_FAILURE);
    }
    // in order traversal
    // so first left, then mid, then right
    
    // we always go as left as possible, so never traverse left
    if (iter->it->right != NULL) {
        iter->__parents[iter->__stackIndex] = iter->it;
        (iter->__stackIndex)++;
        iter->it = iter->it->right;

        while (iter->it->left != NULL) {
            iter->__parents[iter->__stackIndex] = iter->it;
            (iter->__stackIndex)++;
            iter->it = iter->it->left;
        }
    }
    else {
        AVLTreeNode * src;
        do {
            if (iter->__stackIndex <= 0) {
                iter->it = NULL; // iterator exhausted
                freeArray(iter->__parents);
                iter->__parents = NULL;
                break;
            }
            src = iter->it;
            iter->it = iter->__parents[iter->__stackIndex-1]; // up one
            (iter->__stackIndex)--;
        } while(src == iter->it->right); // while coming from right child, keep going up
    }
}