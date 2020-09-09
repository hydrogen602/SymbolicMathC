#ifndef STR_H_
#define STR_H_

#include <stdlib.h>

/**
 * All internal funcs or variables should start with __str_
 * All methods should start with str_
 * 
*/

typedef struct STRING_STRUCT {
    char * __ptr;
    size_t __memLength;
} String;

String buildStringNull();

String buildString(char * st);

void str_free(String * self);

String str_copy(String * self);

char * str_getString(String * self);

size_t str_getLen(String * self);

String str_concat(String * self, String * other);

/**
 * Uses the array.h stuff!
 * Also each value must be freed individually!
 */
String * str_split(String * self, char c);

#endif
