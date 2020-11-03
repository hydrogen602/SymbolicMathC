#include <stdio.h>
#include <stdbool.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include "avltree.h"

#define runTester(func) { printf("Running %s...\n", #func); func(); testConclusion(); }

#define assert(expr) __assert((expr), #expr)
#define assertNot(expr) __assert(!(expr), "!(" #expr ")")

int passedTests = 0;
int allTests = 0;

int groupsPassed = 0;
int allGroups = 0;

void __assert(bool test, const char * expr) {
    //printf("Testing: %s\n", expr);

    allTests++;
    if (test) {
        passedTests++;
    }
    else {
        fprintf(stderr, "Failed test: %s\n", expr);
    }
}

void testConclusion() {
    printf("Passed %d out of %d tests\n", passedTests, allTests);
    if (passedTests == allTests) {
        groupsPassed++;
    }
    allGroups++;

    passedTests = 0;
    allTests = 0;

    puts(" "); 
}

void testBasic() {
    AVLTree tr = build_AVLTree();
    int keys[] = { 1, 3, -4, 2, 10, 4, 0 };
    const char * vals[]  = {"one", "three", "minus four", "two", "ten", "four", "zero"};
    
    for (int i = 0; i < 7; ++i) {
        avl_tree_add(&tr, keys[i], vals[i]);
    }

    assert(tr.length == 7);
    assert(tr.root != NULL);

    for (int i = 7-1; i >= 0; --i) {
        AVLTreeNode * ptr = avl_tree_getNode(&tr, keys[i]);
        assert(keys[i] == ptr->key);
        assert(strcmp(vals[i], ptr->value) == 0);
    }

    avl_tree_free(&tr);
}

void testBigRandomInsert() {
    AVLTree tr = build_AVLTree();
    unsigned int sz = 100000;
    int * keys = calloc(sz, sizeof(int));
    char ** vals = calloc(sz, sizeof(const char *));
    for (unsigned int i = 0; i < sz; ++i) {
        do {
            if (vals[i] != NULL) {
                free(vals[i]);
            }
            keys[i] = rand();
            char * s = calloc(20, sizeof(char));
            snprintf(s, 20, "val -> %d\n", keys[i]);
            vals[i] = s;
        }
        while (avl_tree_add(&tr, keys[i], vals[i]) > 0);
    }

    assert(tr.length == sz);
    assert(tr.root != NULL);

    for (int i = sz-1; i >= 0; --i) {
        AVLTreeNode * ptr = avl_tree_getNode(&tr, keys[i]);
        assert(keys[i] == ptr->key);
        assert(strcmp(vals[i], ptr->value) == 0);
        //printf("%d -> %s\n", keys[i], vals[i]);
    }

    avl_tree_free(&tr);
    for (unsigned int i = 0; i < sz; ++i) {
        free(vals[i]);
    }
    free(keys);
    free(vals);
}

void testRepeatLength() {
    AVLTree tr = build_AVLTree();
    //int keys[] = { 1, 3, -4, 2, 10, 4, 0 };
    //const char * vals[]  = {"one", "three", "minus four", "two", "ten", "four", "zero"};
    
    int keys[] = { 0, 1, 2, 3, 4, -4, 2 };
    const char * vals[]  = {"zero", "one", "two", "three", "four", "minus four", "two"};
    

    for (int i = 0; i < 7; ++i) {
        avl_tree_add(&tr, keys[i], vals[i]);
    }

    //avl_tree_printer(&tr, true);
    struct __AVL_TREE_DETALS details = avl_tree_get_details(tr.root);

    assert(details.count == tr.length);
    assert(details.height <= tr.length);

    avl_tree_free(&tr);
}

int main() {
    set_avltree_silent(true);

    runTester(testBasic);
    runTester(testBigRandomInsert);
    runTester(testRepeatLength);

    printf("Passed %d out of %d test groups\n", groupsPassed, allGroups);

    return 0;
}