#ifndef STR_H_
#define STR_H_

#include <stdlib.h>
#include <stdbool.h>

/**
 * All internal funcs or variables should start with __str_
 * All methods should start with str_
 * 
*/

typedef struct STRING_STRUCT {
    char * __ptr;
    // size_t __memLength;
} String;


// String array

typedef String * StringArray;

/**
 * Creates a StringArray
 */
StringArray newStringArray(size_t length);

/**
 * Frees all the memory.
 * arrPtr - a pointer to the array (so a pointer to a pointer)
 * This is so it can set the array to NULL and avoid dangling ptrs.
 */
void freeStringArray(StringArray * arrPtr);



String buildStringNull();

String buildString(char * st);

String buildStringFromInteger(long int x);

void str_free(String * self);

String str_copy(String * self);

String str_move(String * self);

String str_filterOutChar(String * self, char c);

char * str_getString(String * self);

size_t str_getLen(String * self);

String str_concat(String * self, String * other);

bool str_contains(String * self, char c);

void str_print(String * self);

bool str_isInteger(String * self);

long int str_toInteger(String * self);

/**
 * Uses the array.h stuff!
 * Also each value must be freed individually!
 */
StringArray str_split(String * self, char c);

#endif
