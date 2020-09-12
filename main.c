#include <stdio.h>
#include "dataTypes/array.h"
#include "dataTypes/str.h"
#include "dataTypes/mathobj.h"
#include "dataTypes/header.h"

#include "parser.h"

#include <assert.h>
#include <string.h>


int test();

void evalTest(char * c) {
    String s = buildString(c);

    math_obj m = parseString(&s);

    str_free(&s);

    math_obj_debug_printer(m);
    //putchar('\n');
    printf("-> ");

    m = math_obj_eval(m);

    math_obj_debug_printer(m);
    putchar('\n');

    math_obj_free(m);
}

void repl() {

    puts("Math Processor System");
    puts("");

    while (true) {
        printf("\r> ");
        String in = buildStringFromStdin(80);

        if (strcmp(str_getString(&in), ":q") == 0) {
            puts("");
            str_free(&in);
            return;
        }

        math_obj m = parseString(&in);

        str_free(&in);

        m = math_obj_eval(m);

        math_obj_printer(m);
        putchar('\n');

        math_obj_free(m);
    }
}

int main() {
    #if DEBUG
    printf("Size of String: %lu\n", sizeof(String));
    printf("Size of Union: %lu\n", sizeof(union __MATH_OBJ_VALUE));
    printf("Size of Struct: %lu\n", sizeof(math_struct));

    test();
    #endif

    
    // evalTest("y = 4 + 3 + 2 + x - 5 - x");

    // evalTest("y = -5");

    // evalTest("y - x + 6 - 10");

    repl();

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