
typedef enum Token {
    Tok_EOS,        

    Tok_define,         // define
    Tok_Equal,          // =
    Tok_Plus,           // +
    Tok_var,            // var
    Tok_int,            // const int
    Tok_float,          // const decimal
    Tok_Minus,          // -
    Tok_Divide,         // /
    Tok_Times           // *

} Token;

void token_printer(Token tok);