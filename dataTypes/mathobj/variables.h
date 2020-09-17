#ifndef VARIABLES_H_
#define VARIABLES_H_

#include "mathobjheader.h"

void variables_add(String * name, math_obj m);

/**
 * May return NULL!
 */
math_obj variables_get(String * name);

/**
 * Call before terminating the program!
 */
void variables_cleanup();

#endif