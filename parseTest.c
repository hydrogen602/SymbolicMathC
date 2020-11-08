#include "parser2.h"
#include "dataStructs/array.h"

int main() {
    // (a+a)
    Symbol arr[] = {Term_L_Parens, Term_int, Term_Plus, Term_int, Term_R_Parens};
    SymbolArr arrCp = newArray(5, sizeof(Symbol));
    for (unsigned int i = 0; i < len(arrCp); ++i) {
        arrCp[i] = arr[i];
    }

    parser2(arrCp);

    freeArray(arrCp);

    return 0;
}