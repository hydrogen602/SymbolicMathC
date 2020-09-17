#include <stdbool.h>

#define USE_ASSERT 1

#if USE_ASSERT
    #include <assert.h>
#else
    #define assert(expr)
#endif

#define elif else if

#define DEBUG 0

#define PERCENT_DIFF_TO_BE_SAME (1e-6)