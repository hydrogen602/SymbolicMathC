#include "dataTypes/array.h"
#include <stdio.h>

#include "dataTypes/str.h"

int test();

int parseString(char * c) {
    String s = buildString(c);

    StringArray arr = str_split(&s, '=');

    switch (len(arr))
    {
    case 0:
        fprintf(stderr, "Illegal State at line %d in %s\n", __LINE__, __FILE__);
        exit(1);
        break;
    case 1:
        // expression
        printf("Expression\n");
        break;
    
    case 2:
        // eq
        printf("Equation\n");
        break;
    
    default:
        fprintf(stderr, "Parse Error at line %d in %s\n", __LINE__, __FILE__);
        exit(1);
        break;
    }



    return 0;
}

int main() {
    test();

    parseString("y + x + 3");
}



int test() {
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

    String sTest = buildString("a + b + c");

    String * arr = str_split(&sTest, '+');

    str_free(&sTest);

    for (int i = 0; i < len(arr); ++i) {
        printf("arr[%d] = \"%s\"\n", i, str_getString(arr+i));
    }

    freeStringArray(&arr);

    return 0;
}