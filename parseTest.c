#include "parser2.h"
#include "dataStructs/array.h"
#include <stdio.h>

int main() {
    String s = buildString("define x = y + 6");

    SymbolArr arr = lexer(s);

    for (int i = 0; i < len(arr); ++i) {
        ll1_printToken(arr[i]);
        putchar(' ');
    }
    putchar('\n');
    printf("done with lexing\n");
    // (a+a)
    // Symbol arr[] = {Term_L_Parens, Term_int, Term_Plus, Term_int, Term_R_Parens};
    // SymbolArr arrCp = newArray(5, sizeof(Symbol));
    // for (unsigned int i = 0; i < len(arrCp); ++i) {
    //     arrCp[i] = arr[i];
    // }

    parser2(arr);

    freeArray(arr);
    str_free(&s);

    return 0;
}