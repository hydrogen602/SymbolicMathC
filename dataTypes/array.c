#include <stdlib.h>
#include <stdio.h>

#include "array.h"

#define __ARRAY_WARN_NULLPTR 0


void* newArray(size_t length, size_t size) {
    size_t* ptr = (size_t*) malloc(length * size + sizeof(size_t));
    if (ptr == NULL) {
        fprintf(stderr, "Memory allocation failed");
        //return NULL;
        exit(1);
    }

    *(ptr) = length;
    return (char *)(ptr + 1);
}

size_t len(void* ptr) {    
    if (ptr == NULL) {
#if __ARRAY_WARN_NULLPTR
        fprintf(stderr, "len() called on nullptr at %d in %s\n", __LINE__, __FILE__);
#endif
        return 0;
    }

    return ((size_t *)ptr)[-1];
}

void freeArray(void* ptr) {
    if (ptr == NULL) {
#if __ARRAY_WARN_NULLPTR
        fprintf(stderr, "len() called on nullptr at %d in %s\n", __LINE__, __FILE__);
#endif
        return;
    }

    free((size_t*)ptr - 1);
}