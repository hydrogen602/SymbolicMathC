#include <stdio.h>
#include "dataTypes/array.h"
#include "dataTypes/str.h"
#include "dataTypes/mathobj.h"
#include "dataTypes/util.h"
#include "dataTypes/header.h"

#include "parser.h"

#include <string.h>
#include <assert.h>

math_obj parseHelper(String s) {
    char * c = str_getString(&s);
    int nextLen = str_getLen(&s);
    for (int i = 1; i < strlen(c); ++i) {
        if (c[i] == '-' && !util_isBinaryMathOperator(c[i-1])) {
            // replace with + -
            nextLen++;
        }
    }
    String sEdited = buildStringOfSize(nextLen);
    char * c2 = str_getString(&sEdited);
    int index_c2 = 0;
    for (int i = 0; i < strlen(c); ++i) {
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

    int state = PLUS;

    if (len(arr) == 1) {
        arr = str_split(&sEdited, '*');
        state = PRODUCT;
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
        else if ( str_isInteger(arr + 0) ) {
            #if DEBUG
            printf("Constant\n");
            #endif
            result = buildMathObjectConstant(arr + 0);
        } 
        else {
            #if DEBUG
            printf("Var\n");
            #endif
            result = buildMathObjectVariable(arr + 0);
        }
        break;
    
    default: ;
        #if DEBUG
        printf("Sum\n");
        #endif
        math_obj_array mathArr = newMathObjectArray(len(arr));
        for (int i = 0; i < len(arr); ++i) {
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

math_obj parseString(String *s) {
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