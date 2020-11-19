#ifndef EXCEPTION_H_
#define EXCEPTION_H_
#include <stdio.h>
#include <stdlib.h>

#define exception(name, reason) fprintf(stderr, "Exception in %s on line %d\n\t%s: %s", __FILE__, __LINE__, (name), (reason)); exit(EXIT_FAILURE)

#endif