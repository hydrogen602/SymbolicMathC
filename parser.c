#include <stdio.h>
#include "dataTypes/array.h"
#include "dataTypes/str.h"
#include "dataTypes/mathobj.h"

#include "parser.h"

#include <assert.h>

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