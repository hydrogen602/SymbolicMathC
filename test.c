#include <stdio.h>
#include "dataTypes/array.h"
#include "dataTypes/str.h"
#include "dataTypes/mathobj.h"
#include <string.h>
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


}