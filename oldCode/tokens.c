#include "tokens.h"
#include <stdio.h>

#define TOKEN_PRINTER_HELPER(token) case (token): puts(#token); break

void token_printer(Token tok) {
    switch (tok)
    {
        TOKEN_PRINTER_HELPER(Tok_EOS);
        TOKEN_PRINTER_HELPER(Tok_define);
        TOKEN_PRINTER_HELPER(Tok_Equal);
        TOKEN_PRINTER_HELPER(Tok_Plus);
        TOKEN_PRINTER_HELPER(Tok_var);
        TOKEN_PRINTER_HELPER(Tok_int);
        TOKEN_PRINTER_HELPER(Tok_float);
        TOKEN_PRINTER_HELPER(Tok_Minus);
        TOKEN_PRINTER_HELPER(Tok_Divide);
        TOKEN_PRINTER_HELPER(Tok_Times);
    default:
        printf("Unknown toke: %d\n", tok);
        break;
    }
}