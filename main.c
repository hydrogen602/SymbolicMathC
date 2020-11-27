#include <stdio.h>
#include <string.h>
#include "dataStructs/array.h"
#include "dataStructs/str.h"
#include "dataTypes/mathobj.h"
#include "dataTypes/header.h"
#include "dataTypes/functions.h"
#include "parsing/parse.h"
#include "dataTypes/mathobj/variables.h"
#include "dataStructs/exception.h"
#include "errors.h"


int test();

// void evalTest(char * c) {
//     String s = buildString(c);

//     math_obj m = parse(str_getString(&s));
//     //parseString(&s, 1);

//     str_free(&s);

//     math_obj_debug_printer(m);
//     //putchar('\n');
//     printf("-> ");

//     m = math_obj_eval(m);

//     math_obj_debug_printer(m);
//     putchar('\n');

//     math_obj_free(m);
// }

void repl() {

    puts("Math Processor System");
    puts("Enter :q to quit");

    int lineCounter = 0;

    while (true) {
        lineCounter++;
        printf("\r> ");
        String in = buildStringFromStdin(80);

        if (strcmp(str_getString(&in), ":q") == 0) {
            puts("");
            str_free(&in);
            return;
        }

        struct ParseOutput pOut = parse(str_getString(&in));//parseString(&in, lineCounter);

        str_free(&in);

        math_obj m = pOut.obj;

        if (m == NULL) {
            puts("NULL");
        }
        else {
            switch (pOut.type)
            {
            case STMT_Expression:
                m = math_obj_eval(m);
                math_obj_debug_printer(m);
                putchar('\n');

                math_obj_printer(m);
                putchar('\n');
                break;

            case STMT_Definition:
                m = math_obj_eval(m);

                if (m->typeTag != EQUATION) {
                    char s[25];
                    for (int i = 0; i < 25; ++i) {
                        s[i] = '\0';
                    }
                    snprintf(s, 11, "typeTag = %d", m->typeTag);
                    exception("Invalid math obj, expected equation", s);
                }

                assert(len(math_obj_getChildren(m)) == 2);
                math_obj var = m->data.children[0];
                math_obj value = m->data.children[1];

                if (var->typeTag != VARIABLE) {
                    char s[25];
                    for (int i = 0; i < 25; ++i) {
                        s[i] = '\0';
                    }
                    snprintf(s, 11, "typeTag = %d", var->typeTag);
                    exception("Invalid math obj, expected variable on left side", s);
                }

                String cpy = str_copy(&var->data.label);
                variables_add(&cpy, value);

                m->data.children[1] = NULL; // right side is the only thing we keep

                break;
            default:;
                char s[12];
                for (int i = 0; i < 12; ++i) {
                    s[i] = '\0';
                }
                snprintf(s, 11, "%d", pOut.type);
                exception("Unknown statement type", s);
                break;
            }
        }

        math_obj_free(m);
    }
}

int main() {
    function_init();
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

    variables_cleanup();
    function_cleanup();
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

    for (unsigned int i = 0; i < len(arr); ++i) {
        printf("arr[%d] = \"%s\"\n", i, str_getString(arr+i));
    }

    freeStringArray(&arr);

    return 0;
}