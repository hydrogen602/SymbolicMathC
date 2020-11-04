#ifndef ARRAY_H_
#define ARRAY_H_

#include <stddef.h>

/**
 * length - the count of elements
 * size - the size of an element in bytes ( use sizeof to get this )
 * returns an array that can be used in the regular fashion
 */
void* newArray(size_t length, size_t size);

/**
 * ptr - an array created from newArray
 * returns the count of elements in the array
 * returns null if given a nullptr
 */
size_t len(void* ptr);

/**
 * ptr - an array created from newArray
 * Call this once an array is no longer needed
 * 
 * DO NOT USE free(ptr). ptr does not point to the beginning
 * of the memory block so free will fail!
 */
void freeArray(void* ptr);

#endif