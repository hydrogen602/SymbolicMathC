#include <stdio.h>
#include "dataTypes/array.h"
#include "dataTypes/str.h"
#include "dataTypes/mathobj.h"
#include <string.h>

#ifdef assert
#undef assert
#endif

#include <assert.h>

#define assert2(expr) assert(expr); printf("\tPassed: %s\n", #expr)

void testStr();

int main() {
    testStr();

    puts("Tests passed\n");

    return 0;
}

bool str_compare(String a, char * b) {
    return 0 == strcmp(str_getString(&a), b);
}

void testStr() {
    {
        puts("Testing buildStringNull");

        String s = buildStringNull();
        assert2(str_getLen(&s) == 0);
        assert2(str_compare(s, ""));

        str_free(&s);
        assert2(str_getLen(&s) == 0);
        assert2(str_compare(s, ""));
    }
    
    {
        puts("Testing buildString");

        String s = buildString("abc");
        assert2(str_getLen(&s) == 3);
        assert2(str_compare(s, "abc"));
        str_free(&s);
    }

    {
        puts("Testing buildStringFromInteger");
        String s = buildStringFromInteger(-31415);
        assert2(str_getLen(&s) == 6);
        assert2(str_compare(s, "-31415"));
        str_free(&s);
    }

    {
        puts("Testing str_copy");
        String s = buildString("abc");
        
        String s2 = str_copy(&s);

        assert2(str_getLen(&s) == 3);
        assert2(str_compare(s, "abc"));

        str_free(&s);

        assert2(str_getLen(&s2) == 3);
        assert2(str_compare(s2, "abc"));

        
        str_free(&s2);
    }

    {
        puts("Testing str_move");

        String s = buildString("abc");
        
        String s2 = str_move(&s);

        assert2(str_getLen(&s) == 0);
        assert2(str_compare(s, ""));

        str_free(&s);

        assert2(str_getLen(&s2) == 3);
        assert2(str_compare(s2, "abc"));

        
        str_free(&s2);
    }

    {
        puts("Testing str_filterOutChar");

        String s = buildString("abcb");
        
        String s2 = str_filterOutChar(&s, 'b');

        assert2(str_getLen(&s) == 4);
        assert2(str_compare(s, "abcb"));

        str_free(&s);

        assert2(str_getLen(&s2) == 2);
        assert2(str_compare(s2, "ac"));

        
        str_free(&s2);
    }

    {
        puts("Testing str_getString");

        String s = buildString("abc");
        
        char * s2 = str_getString(&s);

        assert2(0 == strcmp(s2, "abc"));
        assert2(str_compare(s, s2));;

        str_free(&s);
    }

    {
        puts("Testing str_concat");

        String s = buildString("abc");
        
        String s2 = buildString("def");

        String s3 = str_concat(&s, &s2);

        assert2(str_getLen(&s) == 3);
        assert2(str_compare(s, "abc"));

        assert2(str_getLen(&s2) == 3);
        assert2(str_compare(s2, "def"));

        str_free(&s);
        str_free(&s2);

        assert2(str_getLen(&s3) == 6);
        assert2(str_compare(s3, "abcdef"));

        
        str_free(&s3);
    }

    {
        puts("Testing str_contains");

        String s = buildString("abc");
        
        assert2(str_contains(&s, 'c'));

        str_free(&s);

        String s2 = buildString("abc");
        
        assert2(!str_contains(&s2, 'd'));

        str_free(&s2);
    }

    {
        puts("Testing str_rsplit2");

        String s = buildString("abc");
        
        StringArray arr = str_rsplit2(&s, 'd');

        assert2(len(arr) == 1);
        assert2(str_compare(arr[0], "abc"));

        str_free(&s);
        freeStringArray(&arr);
    }

    {
        puts("Testing str_slice");

        String s = buildString("abcdef");
        
        String s2 = str_slice(&s, 2);

        str_free(&s);
        
        assert2(str_compare(s2, "cdef"));

        str_free(&s2);
    }

    {
        puts("Testing buildStringOfSize");

        String s = buildStringOfSize(10);

        assert2(str_compare(s, ""));
        assert2(str_getLen(&s) == 0);

        char * c = str_getString(&s);

        for (int i = 0; i < 10; ++i) {
            c[i] = 'a' + i;
        }

        assert2(str_compare(s, "abcdefghij"));
        assert2(str_getLen(&s) == 10);
        
        str_free(&s);
    }


    {
        puts("Testing str_toDouble");

        String s;
        s = buildString("3.14");

        double d = str_toDouble(&s);

        str_free(&s);

        assert2(d == 3.14);

        s = buildString("3.14e-2");

        assert2(str_toDouble(&s) == 3.14e-2);

        str_free(&s);

        s = buildString("3e+5");

        assert2(str_toDouble(&s) == 3e+5);

        str_free(&s);

        s = buildString("3");

        assert2(str_toDouble(&s) == 3);

        str_free(&s);

        s = buildString("-3E5");

        assert2(str_toDouble(&s) == -300000);

        str_free(&s);
    }

    {
        puts("Testing str_isDouble");

        String s;
        s = buildString("3.14");

        assert2(str_isDouble(&s));

        str_free(&s);

        s = buildString("-3.14e-2");

        assert2(str_isDouble(&s));

        str_free(&s);

        s = buildString(".3e+5");

        assert2(str_isDouble(&s));

        str_free(&s);

        s = buildString("1e-3");

        assert2(str_isDouble(&s));

        str_free(&s);

        s = buildString("..3");

        assert2(!str_isDouble(&s));

        str_free(&s);

        s = buildString("aaa");

        assert2(!str_isDouble(&s));

        str_free(&s);

        s = buildString("3d");

        assert2(!str_isDouble(&s));

        str_free(&s);

        s = buildString("3.14e5a");

        assert2(!str_isDouble(&s));

        str_free(&s);
    }

    {
        puts("Testing str_startswith");
        {
            String s1 = buildString("abc");
            String s2 = buildString("abcd");
            String s3 = buildString("de");

            assert2(!str_startswith(&s1, &s2));
            assert2(str_startswith(&s2, &s1));
            assert2(!str_startswith(&s2, &s3));
            assert2(!str_startswith(&s1, &s3));
            
            str_free(&s1);
            str_free(&s2);
            str_free(&s3);
        }
    }
    
    {
        puts("Testing str_startswithCString");
        {
            String s1 = buildString("abc");
            String s2 = buildString("abcd");
            String s3 = buildString("de");

            assert2(!str_startswithCString(&s1, "abcd"));
            assert2(str_startswithCString(&s2, "a"));
            assert2(!str_startswithCString(&s2, "e"));
            assert2(str_startswithCString(&s3, "de"));
            
            str_free(&s1);
            str_free(&s2);
            str_free(&s3);
        }
    }
}