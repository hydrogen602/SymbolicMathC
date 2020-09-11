#include <stdio.h>
#include "dataTypes/array.h"
#include "dataTypes/str.h"
#include "dataTypes/mathobj.h"

#include "parser.h"

#include <assert.h>


int test();



int main() {
    printf("Size of String: %lu\n", sizeof(String));
    printf("Size of Union: %lu\n", sizeof(union __MATH_OBJ_VALUE));
    printf("Size of Struct: %lu\n", sizeof(math_struct));

    test();

    String s = buildString("y = 4 + 3 + 2 + x + - 5 + x");

    math_obj m = parseString(&s);

    str_free(&s);

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