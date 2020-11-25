#include "str.h"

#ifndef ARRAYLIST_DEFS_H_
#define ARRAYLIST_DEFS_H_

typedef char * cString_t;
typedef void * voidptr_t;

#ifdef VAL_T
    #undef VAL_T
#endif

// #define VAL_T String
// #include "arrayList.h"
// #undef VAL_T

#ifdef INCLUDE_DEFINITIONS_ARRAYLIST
    #undef INCLUDE_DEFINITIONS_ARRAYLIST
#endif

#define VAL_T String
#include "arrayList.h"
#undef VAL_T

#define VAL_T int
#include "arrayList.h"
#undef VAL_T

#define VAL_T long
#include "arrayList.h"
#undef VAL_T

#define VAL_T double
#include "arrayList.h"
#undef VAL_T

#define VAL_T cString_t
#include "arrayList.h"
#undef VAL_T

#define VAL_T voidptr_t
#include "arrayList.h"
#undef VAL_T

#define freeArrayList(arrList) _Generic( arrList,                   \
    ARRAY_LIST_TYPE(String)*: __func(freeArrayList, String),        \
    ARRAY_LIST_TYPE(int)*: __func(freeArrayList, int),              \
    ARRAY_LIST_TYPE(long)*: __func(freeArrayList, long),            \
    ARRAY_LIST_TYPE(double)*: __func(freeArrayList, double),        \
    ARRAY_LIST_TYPE(cString_t)*: __func(freeArrayList, cString_t),  \
    ARRAY_LIST_TYPE(voidptr_t)*: __func(freeArrayList, voidptr_t)   \
    )(arrList)

#define arrLs_append(arrList, val) _Generic( arrList,              \
    ARRAY_LIST_TYPE(String)*: __func(arrLs_append, String),        \
    ARRAY_LIST_TYPE(int)*: __func(arrLs_append, int),              \
    ARRAY_LIST_TYPE(long)*: __func(arrLs_append, long),            \
    ARRAY_LIST_TYPE(double)*: __func(arrLs_append, double),        \
    ARRAY_LIST_TYPE(cString_t)*: __func(arrLs_append, cString_t),  \
    ARRAY_LIST_TYPE(voidptr_t)*: __func(arrLs_append, voidptr_t)   \
    )(arrList, val)

#define arrLs_pop(arrList) _Generic( arrList,                   \
    ARRAY_LIST_TYPE(String)*: __func(arrLs_pop, String),        \
    ARRAY_LIST_TYPE(int)*: __func(arrLs_pop, int),              \
    ARRAY_LIST_TYPE(long)*: __func(arrLs_pop, long),            \
    ARRAY_LIST_TYPE(double)*: __func(arrLs_pop, double),        \
    ARRAY_LIST_TYPE(cString_t)*: __func(arrLs_pop, cString_t),  \
    ARRAY_LIST_TYPE(voidptr_t)*: __func(arrLs_pop, voidptr_t)   \
    )(arrList)

#endif