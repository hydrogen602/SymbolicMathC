#include "parser2.h"
#include "dataStructs/array.h"

int main() {
    String s = buildString("(a+a)");

    SymbolArr arr = lexer(s);
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