#include <stdio.h>
#include <stdbool.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#define KEY_T int
#define VAL_T const char *
#include "avltree.h"

#define crashOnFail true

#define runTester(func) { printf("Running %s...\n", #func); func(); testConclusion(); }

void __assert(bool test, const char * expr);

#define assert(expr) __assert((expr), #expr)
#define assertNot(expr) __assert(!(expr), "!(" #expr ")")

int passedTests = 0;
int allTests = 0;

int groupsPassed = 0;
int allGroups = 0;

bool __heightWithinRestriction(int n, int h) {
    // https://en.wikipedia.org/wiki/AVL_tree

    double golden_ratio = 1.6180339887498948482;
    // change of basis
    //           ln(3)
    // log3(6) = -----
    //           ln(6)

    bool x = log2(n + 1) - 1 <= h && h < (log10(n + 2) / log10(golden_ratio)) - 0.3277;

    if (!x) {
        printf("n = %d, h = %d\n", n, h);
        printf("log2(n + 1) - 1 = %f\n", log2(n + 1) - 1);
        printf("(log(n + 2) / log(golden_ratio)) - 1.3277 = %f\n", (log10(n + 2) / log10(golden_ratio)) - 0.3277);
    }

    return x;
}

void __assert(bool test, const char * expr) {
    //printf("Testing: %s\n", expr);

    allTests++;
    if (test) {
        passedTests++;
    }
    else {
        fprintf(stderr, "Failed test: %s\n", expr);
#if crashOnFail
        printf("Passed %d out of %d tests\n", passedTests, allTests);
        exit(EXIT_FAILURE);
#endif

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
        assert((int)tr.length == i);
        avl_tree_add(&tr, keys[i], vals[i]);

        AVLTreeDetails_t t = avl_tree_get_details(tr.root);
        assert(t.height - ceil(t.bestHeight) < 2);
    }

    assert(tr.length == 7);
    assert(tr.root != NULL);

    for (int i = 7-1; i >= 0; --i) {
        AVLTreeNode * ptr = avl_tree_getNode(&tr, keys[i]);
        assert(keys[i] == ptr->key);
        assert(strcmp(vals[i], ptr->value) == 0);
    }

    assert(avl_tree_getNode(&tr, 20) == NULL);

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

        assert(__heightWithinRestriction(tr.length, tr.height));
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
    assert(details.height == tr.height);

    avl_tree_free(&tr);

    assert(0 == tr.length);
    assert(0 == tr.height);
    assert(tr.root == NULL);
}

void testIterator() {
    AVLTree tr = build_AVLTree();
    int keys[] = { 1, 3, -4, 2, 10, 4, 0 };
    const char * vals[]  = {"one", "three", "minus four", "two", "ten", "four", "zero"};
    
    for (int i = 0; i < 7; ++i) {
        assert((int)tr.length == i);
        avl_tree_add(&tr, keys[i], vals[i]);

        AVLTreeDetails_t t = avl_tree_get_details(tr.root);
        assert(t.height - ceil(t.bestHeight) < 2);
    }

    assert(tr.length == 7);
    assert(tr.root != NULL);

    int inOrder_keys[] = { -4, 0, 1, 2, 3, 4, 10 };
    const char * inOrder_vals[]  = {"minus four", "zero", "one", "two", "three", "four", "ten"};

    int index = 0;
    for (AVLTreeIterator iter = avl_tree_iterate(&tr); iter.it != NULL; avl_tree_iter_next(&iter)) {

        assert(inOrder_keys[index] == iter.it->key);
        assert(strcmp(inOrder_vals[index], iter.it->value) == 0);

        index++;
    }

    avl_tree_free(&tr);
}

// ArrayList

#ifdef VAL_T
    #undef VAL_T
#endif

#define VAL_T int
#include "arrayList.h"

void testArrList() {

    ArrayList_int ls = newArrayList_int();

    int nums[] = {-98, 98, 93, 71, 4, -31, -48, -94, 10, 61, -57, 14, 71, -82, -64, 96, -50, -27, -65, 49, -94, 37, -31, -80, -94, -3, -97, 28, 47, -97, -38, 69, -56, -92, -11};

    for (int i = 0; i < 35; ++i) {
        assert(i == ls.length);
        arrLs_append_int(&ls, nums[i]);
    }

    for (int i = 35-1; i >= 0; --i) {
        assert(ls.list[i] == nums[i]);
    } 

    for (int i = 0; i < 35; ++i) {
        assert(ls.length == 35 - i);
        assert(arrLs_pop_int(&ls) == nums[35-1-i]);
    }  

    assert(ls.length == 0);
    freeArrayList_int(&ls);
    assert(ls.list == NULL);
    assert(ls.__capacity == 0);
    assert(ls.length == 0);
}

int main() {
    set_avltree_silent(true);

    runTester(testBasic);
    runTester(testBigRandomInsert);
    runTester(testRepeatLength);
    runTester(testIterator);
    runTester(testArrList);

    printf("Passed %d out of %d test groups\n", groupsPassed, allGroups);

    return 0;
}