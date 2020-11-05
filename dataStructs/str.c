#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <ctype.h>
#include <errno.h>

#include "str.h"
#include "../dataStructs/array.h"

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
    size_t memLength = strlen(st) + 1;
    String s = buildStringOfRawSize(memLength);
    strcpy(s.__ptr, st);
    return s;
}

String buildStringOfSize(size_t n) {
    return buildStringOfRawSize(n + 1);
}

String buildStringOfRawSize(size_t n) {
    String s = { NULL };

    s.__ptr = calloc(n, sizeof(char));
    if (s.__ptr == NULL) {
        fprintf(stderr, "Could not allocate memory");
        exit(1);
    }
    // calloc should set everything to null
    return s;
}

String buildStringFromInteger(long int x) {
    // 2^64 is 18446744073709551616 which is 20 chars
    // 2 extra for just in case
    size_t memLength = 22 + 1;
    String s = buildStringOfSize(memLength-1); // func adds 1

    snprintf(s.__ptr, 22+1, "%ld", x);
    return s;
}

String buildStringFromDouble(double d) {
    // 24 sounds like a good length
    size_t memLength = 24;

    String s = buildStringOfSize(memLength-1); // func adds 1
    snprintf(s.__ptr, memLength, "%f", d);
    return s;
}

String buildStringFromStdin(unsigned int maxLen) {

    char buf[maxLen+1];
    fgets(buf, maxLen + 1, stdin);

    for (unsigned int i = 0; i < maxLen + 1; ++i) {
        if (buf[i] == '\n') {
            buf[i] = '\0';
            break;
        }
    }
    size_t length = strlen(buf);

    String s = buildStringOfSize(length);
    strcpy(s.__ptr, buf);
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
    for (unsigned int i = 0; i < len(arr); ++i) {
        str_free(arr+i);
    }
    freeArray(arr);
    *arrPtr = NULL;
}


String str_copy(String * self) {
    String s = buildStringOfSize(str_getLen(self));
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

String str_slice(String * self, unsigned int startIndex) {
    if (startIndex >= str_getLen(self)) {
        return buildStringNull();
    }
    String s = buildStringOfSize(str_getLen(self) - startIndex);
    strcpy(s.__ptr, self->__ptr + startIndex);
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
    for (unsigned int i = 0; i < stringLen; ++i) {
        if (str[i] == c) {
            newLen--;
        }
    }

    String s = buildStringOfRawSize(newLen);

    unsigned int index = 0;
    for (unsigned int i = 0; i < stringLen; ++i) {
        if (str[i] != c) {
            s.__ptr[index] = str[i];
            index++;
        }
    }

    if (index + 1 != newLen) {
        fprintf(stderr, "Assertion Error at %d in %s\n", __LINE__, __FILE__);
        exit(1);
    }

    s.__ptr[index] = '\0';

    return s;
}

String str_concat(String * self, String * other) {
    size_t newLen = str_getLen(self) + str_getLen(other);
    String s = buildStringOfSize(newLen);

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

int str_index(String * self, char c) {
    char * s = str_getString(self);
    size_t len = str_getLen(self);

    for (size_t i = 0; i < len; ++i) {
        if (s[i] == c) {
            return i;
        }
    }
    return -1;
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

bool str_isDouble(String * self) {
    // double accepts integers as well, so check for integer first
    char * s = str_getString(self);

    char * endptr;
    errno = 0;
    strtod(s, &endptr);
    if (errno != 0 || *endptr != '\0') {
        return false;
    }
    return true;
}

bool str_isNumber(String * self) {
    return str_isInteger(self) || str_isDouble(self);
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

double str_toDouble(String * self) {
    //double strtod(const char *str, char **endptr)
    char * ptr;
    double n = strtod(str_getString(self), &ptr);

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

bool str_isEqual(String * self, String * other) {
    char * s = str_getString(self);
    char * o = str_getString(other);

    return strcmp(s, o) == 0;
}

int str_cmp(String * self, String * other) {
    char * s = str_getString(self);
    char * o = str_getString(other);

    return strcmp(s, o);
}

bool str_startswith(String * self, String * other) {
    char * s = str_getString(self);
    char * o = str_getString(other);

    if (str_getLen(self) < str_getLen(other)) {
        return false;
    }

    for (unsigned int i = 0; i < str_getLen(other); ++i) {
        if (s[i] != o[i]) {
            return false;
        }
    }

    return true;
}

bool str_startswithCString(String * self, char * other) {
    if (other == NULL) {
        return false;
    }
    char * s = str_getString(self);

    if (str_getLen(self) < strlen(other)) {
        return false;
    }

    for (unsigned int i = 0; i < strlen(other); ++i) {
        if (s[i] != other[i]) {
            return false;
        }
    }

    return true;
}

// String str_replace(String * self, char * replaceThis, char * replaceWith) {
//     int delta = strlen(replaceWith) - strlen(replaceThis);
//     // thats increase per match
// }