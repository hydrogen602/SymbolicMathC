#include <stdio.h>
#include "dataTypes/array.h"
#include "dataTypes/str.h"
#include "dataTypes/mathobj.h"

#include <assert.h>


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

        
        if ( str_isInteger(arr + 0) ) {
            printf("Constant\n");
            result = buildMathObjectConstant(arr + 0);
        } 
        else {
            printf("Var\n");
            result = buildMathObjectVariable(arr + 0);
        }
        break;
    
    default:
        // eq
        printf("Sum\n");
        math_obj_array mathArr = newMathObjectArray(len(arr));
        for (int i = 0; i < len(arr); ++i) {
            mathArr[i] = parseHelper(arr[i]);
        }

        result = buildMathObjectPlus(mathArr);
        break;
    }

    freeStringArray(&arr);

    return result;
}

math_obj parseString(char * c) {
    StringArray arr;

    {
        String s = buildString(c);

        String s2 = str_filterOutChar(&s, ' ');

        arr = str_split(&s2, '=');

        str_free(&s);
        str_free(&s2);
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
    printf("Size of String: %lu\n", sizeof(String));
    printf("Size of Union: %lu\n", sizeof(union __MATH_OBJ_VALUE));
    printf("Size of Struct: %lu\n", sizeof(math_struct));

    test();

    math_obj m = parseString("y = 4 + 3 + 2 + x + 5 + x");

    math_obj_debug_printer(m);
    putchar('\n');

    m = math_obj_eval(m);

    math_obj_debug_printer(m);
    putchar('\n');

    math_obj_free(m);


}

int test() {
    String sNUll = buildStringNull();
    printf("String is: %s\n", str_getString(&sNUll));

    String s = buildString("test");
    printf("String is: %s\n", str_getString(&s));


    String s2 = buildString("abc_some_stuff");

    String a3 = str_concat(&s, &s2);

    printf("String is: %s\n", str_getString(&a3));

    str_free(&sNUll);
    str_free(&s);
    str_free(&s2);

    String a4 = str_filterOutChar(&a3, 's');

    printf("String is: %s\n", str_getString(&a4));

    str_free(&a3);
    str_free(&a4);

    String sTest = buildString("a + b + c");

    String * arr = str_split(&sTest, '+');

    str_free(&sTest);

    for (int i = 0; i < len(arr); ++i) {
        printf("arr[%d] = \"%s\"\n", i, str_getString(arr+i));
    }

    freeStringArray(&arr);

    return 0;
}