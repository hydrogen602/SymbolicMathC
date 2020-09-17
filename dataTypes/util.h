#ifndef UTIL_H_
#define UTIL_H_

#include <stdbool.h>

bool util_isBinaryMathOperator(char c);

long util_gcd(long a, long b);

double util_percentDiff(double a, double b);

#endif