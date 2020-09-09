#include <stdlib.h>
#include <stdio.h>

#include "array.h"

void* newArray(size_t length, size_t size) {
    size_t* ptr = (size_t*) malloc(length * size + sizeof(size_t));
    if (ptr == NULL) {
        fprintf(stderr, "Memory allocation failed");
        return NULL;
    }

    *(ptr) = length;
    return (char *)(ptr + 1);
}

size_t len(void* ptr) {
    return ((size_t *)ptr)[-1];
}

void freeArray(void* ptr) {
    free((size_t*)ptr - 1);
}