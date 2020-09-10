#include "dataTypes/array.h"
#include <stdio.h>

#include "dataTypes/str.h"

#include "dataTypes/mathobj.h"

int test();

math_obj parseHelper(String s) {
    StringArray arr = str_split(&s, '+');

    math_obj result = NULL;

    switch (len(arr))
    {
    case 0:
        fprintf(stderr, "Illegal State at line %d in %s\n", __LINE__, __FILE__);
        exit(1);
        break;
    case 1:
        // expression

        printf("Var\n");
        result = buildMathObjectVariable(arr + 0);
        break;
    
    case 2:
        // eq
        printf("Sum\n");
        math_obj a = parseHelper(arr[0]);
        math_obj b = parseHelper(arr[1]);

        result = buildMathObjectPlus(a, b);
        break;
    
    default:
        fprintf(stderr, "Parse Error at line %d in %s\n", __LINE__, __FILE__);
        exit(1);
        break;
    }

    freeStringArray(&arr);

    return result;
}

math_obj parseString(char * c) {
    StringArray arr;

    {
        String s = buildString(c);

        arr = str_split(&s, '=');

        str_free(&s);
    }

    math_obj result = NULL;

    switch (len(arr))
    {
    case 0:
        fprintf(stderr, "Illegal State at line %d in %s\n", __LINE__, __FILE__);
        exit(1);
        break;
    case 1:
        // expression

        printf("Expression\n");
        result = parseHelper(arr[0]);
        break;
    
    case 2:
        // eq
        printf("Equation\n");
        math_obj a = parseHelper(arr[0]);
        math_obj b = parseHelper(arr[1]);

        result = buildMathObjectEquation(a, b);
        break;
    
    default:
        fprintf(stderr, "Parse Error at line %d in %s\n", __LINE__, __FILE__);
        exit(1);
        break;
    }

    freeStringArray(&arr);

    return result;
}

int main() {
    test();

    math_obj m = parseString("y = x + 3");

    math_obj_free(m);
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