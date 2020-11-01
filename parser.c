#include <stdio.h>
#include <string.h>
#include "dataTypes/array.h"
#include "dataTypes/str.h"
#include "dataTypes/mathobj.h"
#include "dataTypes/util.h"
#include "dataTypes/header.h"
#include "parser.h"
#include "errors.h"

math_obj parseHelper(String s) {
    if (str_getLen(&s) == 0) {
        throw_error("Unexpected Empty String", str_getString(&s));
    }

    char * c = str_getString(&s);
    int nextLen = str_getLen(&s);
    for (unsigned int i = 1; i < strlen(c); ++i) {
        if (c[i] == '-' && !util_isBinaryMathOperator(c[i-1])) {
            // replace with + -
            nextLen++;
        }
    }
    String sEdited = buildStringOfSize(nextLen);
    char * c2 = str_getString(&sEdited);
    int index_c2 = 0;
    for (unsigned int i = 0; i < strlen(c); ++i) {
        if (i > 0 && c[i] == '-' && !util_isBinaryMathOperator(c[i-1])) {
            // replace with + -
            c2[index_c2] = '+';
            c2[index_c2+1] = '-';
            index_c2++;
        }
        else {
            c2[index_c2] = c[i];
        }
        index_c2++;
    }
    assert(index_c2 == nextLen);

    StringArray arr = str_split(&sEdited, '+');

    math_type state = PLUS;

    if (len(arr) == 1) {
        freeStringArray(&arr);
        arr = str_split(&sEdited, '*');
        state = PRODUCT;
    }

    if (len(arr) == 1) {
        freeStringArray(&arr);
        arr = str_rsplit2(&sEdited, '/');
        state = FRACTION;
    }

    math_obj result = NULL;

    switch (len(arr))
    {
    case 0:
        fprintf(stderr, "Illegal State at line %d in %s\n", __LINE__, __FILE__);
        exit(1);
        break;
    case 1:
        // single thing

        if ( str_index(arr+0, '-') == 0 ) {
            // leads with - so negate

            #if DEBUG
            printf("Negate\n");
            #endif

            String withoutNeg = str_slice(arr + 0, 1);

            result = buildMathObjectNegate( parseHelper(withoutNeg) );

            str_free(&withoutNeg);
        }
        else if ( str_isNumber(arr + 0) ) {
            #if DEBUG
            printf("Constant\n");
            #endif
            result = buildMathObjectConstantFromString(arr + 0);
        }
        else {
            #if DEBUG
            printf("Var\n");
            #endif
            result = buildMathObjectVariable(arr + 0);
        }
        break;
    case 2:
        // two things
        #if DEBUG
        printf("Fraction\n");
        #endif

        if (state == FRACTION) {
            math_obj n = parseHelper(arr[1]);
            math_obj d = parseHelper(arr[0]);

            result = buildMathObjectFraction(n, d);
            break;
        }
        // fall through
    default: ;
        #if DEBUG
        printf("Sum\n");
        #endif
        math_obj_array mathArr = newMathObjectArray(len(arr));
        for (unsigned int i = 0; i < len(arr); ++i) {
            mathArr[i] = parseHelper(arr[i]);
        }

        if (state == PLUS)
            result = buildMathObjectPlus(mathArr);
        else if (state == PRODUCT)
            result = buildMathObjectProduct(mathArr);
        else
            assert(false);
        
        break;
    }

    freeStringArray(&arr);
    str_free(&sEdited);
    return result;
}

math_obj parseString(String *s, int lineNum) {
    set_line_num(lineNum);

    if (str_getLen(s) == 0) {
        return NULL;
    }

    if (str_startswithCString(s, "define ")) {
        String sBrief = str_slice(s, 7);

        math_obj m = parseString(&sBrief, lineNum);

        if (m == NULL) {
            throw_error("Invalid Assignment", str_getString(&sBrief));
        }
        if (m->typeTag != EQUATION) {
            math_obj_debug_dump(m);
            throw_error("Invalid Assignment: Expected Equation", str_getString(&sBrief));
        }
        assert(len(math_obj_getChildren(m)) == 2);
        math_obj var = m->data.children[0];
        math_obj value = m->data.children[1];

        if (var->typeTag != VARIABLE) {
            throw_error("Left Side of Assignment Must Be A Variable", str_getString(&sBrief));
        }

        variables_add(&sBrief, value);

        m->data.children[1] = NULL; // right side is the only thing we keep
        math_obj_free(m);

        str_free(&sBrief);

        return NULL;
    }

    StringArray arr;

    {
        String s2 = str_filterOutChar(s, ' ');

        arr = str_split(&s2, '=');

        str_free(&s2);
    }

    math_obj result = NULL;

    switch (len(arr))
    {
    case 0:
        fprintf(stderr, "Illegal State at line %d in %s\n", __LINE__, __FILE__);
        exit(1);
        break;
    case 1: ;
        // expression

        #if DEBUG
        printf("Expression\n");
        #endif
        result = parseHelper(arr[0]);
        break;
    
    case 2: ;
        // eq
        #if DEBUG
        printf("Equation\n");
        #endif
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