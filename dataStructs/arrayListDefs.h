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

#define freeArrayList(arrList) _Generic( arrList, ArrayList_double*: freeArrayList_double )(arrList)
// void __func(arrLs_append, VAL_T)(ARRAY_LIST_TYPE(VAL_T) * arrList, VAL_T val);
// VAL_T __func(arrLs_pop, VAL_T)(ARRAY_LIST_TYPE(VAL_T) * arrList);

// _Generic( )

#endif