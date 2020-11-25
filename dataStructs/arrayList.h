/*
 * How to use:
 * 
 * #define VAL_T <your type>   
 * 
 * To include definitions:
 *      #define INCLUDE_DEFINITIONS_ARRAYLIST
 * 
 * Then:
 * #include { path to this file }
 * 
 * Possibly useful for globals:
 *      ARRAY_LIST_NULL_INIT
 */

#include <stdbool.h>
#include <stdlib.h>
#include "exception.h"

#ifndef ARRAY_LIST_NULL_INIT
    #define ARRAY_LIST_NULL_INIT { NULL, 0UL, 0UL }
#endif

#ifndef CONCAT_
    #define CONCAT_(A, B) A ## _ ## B
#endif

#ifndef VAL_T
    #warning "VAL_T needs to be defined, defaulting to int"
    #define VAL_T int
#endif

#ifndef __func
    #define __func(name, type) CONCAT_(name, type)
#endif

#ifdef ARRAY_LIST_TYPE
    #undef ARRAY_LIST_TYPE
#endif

#ifndef ARRAY_LIST_TYPE
    #define ARRAY_LIST_TYPE(type) CONCAT_(ArrayList, type)
#endif

#ifndef INCLUDE_DEFINITIONS_ARRAYLIST

typedef struct ARRAY_LIST_TYPE(VAL_T)
{
    VAL_T * list;
    size_t __capacity;
    size_t length;
} ARRAY_LIST_TYPE(VAL_T);

ARRAY_LIST_TYPE(VAL_T) __func(newArrayList, VAL_T)();
void __func(freeArrayList, VAL_T)(ARRAY_LIST_TYPE(VAL_T) * arrList);
void __func(arrLs_append, VAL_T)(ARRAY_LIST_TYPE(VAL_T) * arrList, VAL_T val);
VAL_T __func(arrLs_pop, VAL_T)(ARRAY_LIST_TYPE(VAL_T) * arrList);

//#ifdef INCLUDE_DEFINITIONS_ARRAYLIST
#else
#undef INCLUDE_DEFINITIONS_ARRAYLIST

ARRAY_LIST_TYPE(VAL_T) __func(newArrayList, VAL_T)() {
    ARRAY_LIST_TYPE(VAL_T) t;
    t.length = 0;
    t.__capacity = 16;
    t.list = (VAL_T *) calloc(t.__capacity, sizeof(VAL_T));
    if (t.list == NULL) {
        exception("Memory Allocation Failed", "calloc returned NULL");
    }
    return t;
}

void __func(freeArrayList, VAL_T)(ARRAY_LIST_TYPE(VAL_T) * arrList) {
    if (arrList != NULL && arrList->list != NULL) {
        free(arrList->list);
        arrList->list = NULL;
    }
    arrList->length = 0;
    arrList->__capacity = 0;
}

void __func(arrLs_append, VAL_T)(ARRAY_LIST_TYPE(VAL_T) * arrList, VAL_T val) {
    if (arrList == NULL) {
        exception("NullPointerException", "arrList");
    }

    if (arrList->list == NULL) {
        arrList->__capacity = 16;
        arrList->list = (VAL_T *) calloc(arrList->__capacity, sizeof(VAL_T));
        if (arrList->list == NULL) {
            exception("Memory Allocation Failed", "calloc returned NULL");
        }
    }
    else if (arrList->length >= arrList->__capacity) {
        VAL_T * newArr = (VAL_T *) calloc(arrList->__capacity * 2, sizeof(VAL_T));
        if (newArr == NULL) {
            exception("Memory Allocation Failed", "calloc returned NULL");
        }
        for (size_t i = 0; i < arrList->__capacity; ++i) {
            newArr[i] = arrList->list[i];
        }
        free(arrList->list);
        arrList->list = newArr;
        arrList->__capacity = arrList->__capacity * 2;
    }

    if (arrList->length >= arrList->__capacity) {
        exception("Invalid State", "Length is larger or equal to capacity");
    }

    arrList->list[arrList->length] = val;
    ++(arrList->length);
}

VAL_T __func(arrLs_pop, VAL_T)(ARRAY_LIST_TYPE(VAL_T) * arrList) {
    if (arrList == NULL) {
        exception("NullPointerException", "arrList");
    }
    if (arrList->length == 0) {
        exception("Error", "Pop from empty list");
    }

    --(arrList->length);
    return arrList->list[arrList->length];
}

#endif