#include "util.h"

bool util_isBinaryMathOperator(char c) {
    char * ops = "^*=+/><-";
    int len = 8;
    for (int i = 0; i < len; ++i) {
        if (ops[i] == c) {
            return true;
        }
    }
    return false;
}