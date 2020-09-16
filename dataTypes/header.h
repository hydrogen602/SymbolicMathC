#include <stdbool.h>

#define USE_ASSERT 1

#if USE_ASSERT
    #include <assert.h>
#else
    #define assert(expr)
#endif

#define elif else if

#define DEBUG 0