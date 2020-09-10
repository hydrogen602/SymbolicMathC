#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <ctype.h>

#include "str.h"
#include "array.h"

/**
 * All internal funcs or variables should start with __str_
 * All methods should start with str_
 * 
*/
char * __str_nullstring = "";

String buildStringNull() {
    String s = { NULL, 0 };
    return s;
}

String buildString(char * st) {
    String s = { NULL, 0 };

    s.__memLength = strlen(st) + 1;
    s.__ptr = calloc(s.__memLength, 1);
    if (s.__ptr == NULL) {
        fprintf(stderr, "Could not allocate memory");
        exit(1);
    }
    strcpy(s.__ptr, st);
    return s;
}

void str_free(String * self) {
    if (self->__ptr != NULL) {
        free(self->__ptr);
        self->__ptr = NULL;
        self->__memLength = 0;
    }
}

/**
 * Creates a StringArray
 */
StringArray newStringArray(size_t length) {
    return newArray(length, sizeof(String));
}

/**
 * Frees all the memory.
 * arrPtr - a pointer to the array (so a pointer to a pointer)
 * This is so it can set the array to NULL and avoid dangling ptrs.
 */
void freeStringArray(StringArray * arrPtr) {
    StringArray arr = *arrPtr;
    for (int i = 0; i < len(arr); ++i) {
        str_free(arr+i);
    }
    freeArray(arr);
    *arrPtr = NULL;
}


String str_copy(String * self) {
    String s = {
        calloc(self->__memLength, 1),
        self->__memLength
    };
    if (s.__ptr == NULL) {
        fprintf(stderr, "Could not allocate memory");
        exit(1);
    }
    strcpy(s.__ptr, self->__ptr);
    return s;
}

String str_move(String * self) {
    // deletes the old to avoid dangling pointers
    String s = {
        self->__ptr,
        self->__memLength
    };

    self->__ptr = NULL;
    self->__memLength = 0;

    return s;
}

char * str_getString(String * self) {
    if (self->__ptr == NULL) {
        return __str_nullstring;
    }
    else {
        return self->__ptr;
    }
}

size_t str_getLen(String * self) {
    if (self->__ptr == NULL) {
        return 0;
    }
    else {
        return strlen(self->__ptr);
    }
}

String str_filterOutChar(String * self, char c) {
    size_t newLen = self->__memLength;
    size_t stringLen = str_getLen(self);
    char * str = str_getString(self);
    for (int i = 0; i < stringLen; ++i) {
        if (str[i] == c) {
            newLen--;
        }
    }

    String s = {
        calloc(newLen, 1),
        newLen
    };
    if (s.__ptr == NULL) {
        fprintf(stderr, "Could not allocate memory");
        exit(1);
    }

    int index = 0;
    for (int i = 0; i < stringLen; ++i) {
        if (str[i] != c) {
            s.__ptr[index] = str[i];
            index++;
        }
    }

    if (index + 1 != newLen) {
        fprintf(stderr, "Assertion Error at %d in %s\n", __LINE__, __FILE__);
        exit(1);
    }

    s.__ptr[index + 1] = '\0';

    return s;
}

String str_concat(String * self, String * other) {
    size_t newLen = str_getLen(self) + str_getLen(other) + 1;
    String s = {
        calloc(newLen, 1),
        newLen
    };
    if (s.__ptr == NULL) {
        fprintf(stderr, "Could not allocate memory");
        exit(1);
    }

    strcpy(s.__ptr, self->__ptr);
    strcat(s.__ptr, other->__ptr);
    return s;
}

String * str_split(String * self, char c) {
    String copy = str_copy(self);

    char * s = str_getString(&copy);
    size_t len = str_getLen(self);

    int counter = 1;
    for (size_t i = 0; i < len; ++i) {
        if (s[i] == c) {
            counter += 1;
        }
    }

    String * arr = newStringArray(counter);
    int index = 0;

    size_t last = 0;
    for (size_t i = 0; i < len; ++i) {
        if (s[i] == c) {
            s[i] = '\0';
            String newStr = buildString(s + last);
            arr[index] = newStr;
            index++;
            last = i + 1;
        }
    }

    String newStr = buildString(s + last);
    arr[index] = newStr;

    str_free(&copy);
    return arr;
}

bool str_contains(String * self, char c) {
    char * s = str_getString(self);
    size_t len = str_getLen(self);

    for (size_t i = 0; i < len; ++i) {
        if (s[i] == c) {
            return true;
        }
    }
    return false;
}

void str_print(String * self) {
    printf("%s", str_getString(self));
}

bool str_isInteger(String * self) {
    char * s = str_getString(self);
    size_t len = str_getLen(self);

    for (size_t i = 0; i < len; ++i) {
        if (!isdigit(s[i])) {
            return false;
        }
    }
    return true;
}
