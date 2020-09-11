#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <ctype.h>
#include <errno.h>

#include "str.h"
#include "array.h"

/**
 * All internal funcs or variables should start with __str_
 * All methods should start with str_
 * 
*/
char * __str_nullstring = "";

String buildStringNull() {
    String s = { NULL };
    return s;
}

String buildString(char * st) {
    String s = { NULL };

    size_t memLength = strlen(st) + 1;
    s.__ptr = calloc(memLength, 1);
    if (s.__ptr == NULL) {
        fprintf(stderr, "Could not allocate memory");
        exit(1);
    }
    strcpy(s.__ptr, st);
    return s;
}

String buildStringFromInteger(long int x) {
    String s = { NULL };

    // 2^64 is 18446744073709551616 which is 20 chars
    // 2 extra for just in case
    size_t memLength = 22 + 1;
    s.__ptr = calloc(memLength, 1);
    if (s.__ptr == NULL) {
        fprintf(stderr, "Could not allocate memory");
        exit(1);
    }
    sprintf(s.__ptr, "%ld", x);
    return s;
}

void str_free(String * self) {
    if (self->__ptr != NULL) {
        free(self->__ptr);
        self->__ptr = NULL;
        //self->__memLength = 0;
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
        calloc(str_getLen(self) + 1, 1),
        //self->__memLength
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
        //self->__memLength
    };

    self->__ptr = NULL;
    //self->__memLength = 0;

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
    // size_t newLen = self->__memLength;
    size_t stringLen = str_getLen(self);
    size_t newLen = stringLen + 1;
    char * str = str_getString(self);
    for (int i = 0; i < stringLen; ++i) {
        if (str[i] == c) {
            newLen--;
        }
    }

    String s = {
        calloc(newLen, 1)
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
        calloc(newLen, 1)
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

StringArray str_rsplit2(String * self, char c) {
    String copy = str_copy(self);

    char * s = str_getString(&copy);
    size_t len = str_getLen(self);

    int counter = 1;
    for (int i = len - 1; i >= 0; --i) {
        if (s[i] == c) {
            counter = 2;
            break;
        }
    }

    String * arr = newStringArray(counter);
    int index = 0;

    for (int i = len - 1; i >= 0; --i) {
        if (s[i] == c) {
            s[i] = '\0';
            String newStr = buildString(s + i + 1);
            arr[index] = newStr;
            index++;
            break; // max array len 2
        }
    }

    String newStr = buildString(s);
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
        if (i == 0 && s[i] == '-') {
            continue; // minus sign allowed at the beginning
        }
        if (!isdigit(s[i])) {
            return false;
        }
    }
    return true;
}

long int str_toInteger(String * self) {
    char * ptr;
    long int n = strtol(str_getString(self), &ptr, 10);

    if (n == 0) {
        if (errno == EINVAL) {
            fprintf(stderr, "Conversion error occurred: %d\n", errno);
            exit(1);
        }

        if (errno == ERANGE) {
            fprintf(stderr, "The value provided was out of range: %s\n", str_getString(self));
            exit(1);
        }

        if (strlen(ptr) > 0) {
            fprintf(stderr, "Integer Parse Error in string \"%s\": offending bit = \"%s\"\n", str_getString(self), ptr);
            exit(1);
        }

        if (str_getLen(self) == 0) {
            fprintf(stderr, "Cannot convert an empty string to integer\n");
            exit(1);
        }
    }

    return n;
}

// String str_replace(String * self, char * replaceThis, char * replaceWith) {
//     int delta = strlen(replaceWith) - strlen(replaceThis);
//     // thats increase per match
// }