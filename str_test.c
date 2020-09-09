#include "string.h"
#include <stdio.h>

int main() {

    String sNUll = buildStringNull();
    printf("String is: %s\n", str_getString(&sNUll));

    String s = buildString("test");
    printf("String is: %s\n", str_getString(&s));


    String s2 = buildString("abc");

    String a3 = str_concat(&s, &s2);

    printf("String is: %s\n", str_getString(&a3));

    str_free(&sNUll);
    str_free(&s);
    str_free(&s2);
    str_free(&a3);
}  