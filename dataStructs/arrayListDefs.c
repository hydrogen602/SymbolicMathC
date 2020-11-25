#include "str.h"
#include "arrayListDefs.h"

// The array list for a few different possible types

#ifdef VAL_T
    #undef VAL_T
#endif

#define VAL_T String
#define INCLUDE_DEFINITIONS_ARRAYLIST
#include "arrayList.h"
#undef VAL_T

#define VAL_T int
#define INCLUDE_DEFINITIONS_ARRAYLIST
#include "arrayList.h"
#undef VAL_T

#define VAL_T long
#define INCLUDE_DEFINITIONS_ARRAYLIST
#include "arrayList.h"
#undef VAL_T

#define VAL_T double
#define INCLUDE_DEFINITIONS_ARRAYLIST
#include "arrayList.h"
#undef VAL_T

#define VAL_T cString_t
#define INCLUDE_DEFINITIONS_ARRAYLIST
#include "arrayList.h"
#undef VAL_T

#define VAL_T voidptr_t
#define INCLUDE_DEFINITIONS_ARRAYLIST
#include "arrayList.h"
#undef VAL_T