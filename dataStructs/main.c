#include <stdio.h>
#include "avltree.h"

int main() {
    puts("Hello World!");

    AVLTree tr = build_AVLTree();
    //int keys[] = { 1, 3, -4, 2, 10, 4, 0 };
    //const char * vals[]  = {"one", "three", "minus four", "two", "ten", "four", "zero"};
    
    int keys[] = { 0, 1, 2, 3, 4, -4, 10 };
    const char * vals[]  = {"zero", "one", "two", "three", "four", "minus four", "ten"};
    

    for (int i = 0; i < 7; ++i) {
        avl_tree_add(&tr, keys[i], vals[i]);
        
        printf("%d\n", tr.length);
        avl_tree_printer(&tr, true);
    }

    avl_tree_printer(&tr, true);

    avl_tree_print_details(&tr);

    avl_tree_free(&tr);

    return 0;
}