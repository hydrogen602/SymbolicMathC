#include "lexer.h"
#include "parser.tab.h"
#include <stdio.h>
#include <stdlib.h>
#include "../dataTypes/mathobj.h"

// #include "../dataStructs/str.h"
// #define VAL_T String
// #include "../dataStructs/arrayList.h"

// int main() { 
  
//     // Explanation: 
//     // yywrap() - wraps the above rule section 
//     /* yyin - takes the file pointer  
//             which contains the input*/
//     /* yylex() - this is the main flex function 
//             which runs the Rule Section*/
//     // yytext is the text in the buffer 
    
//     // Uncomment the lines below  
//     // to take input from file 
//     // FILE *fp; 
//     // char filename[50]; 
//     // printf("Enter the filename: \n"); 
//     // scanf("%s",filename); 
//     // fp = fopen(filename,"r"); 
//     // yyin = fp; 

//     ArrayList_String tokens = newArrayList_String();


//     yy_scan_string("-3e-5 - b");
  
//     Token t = yylex();
//     while (t != Tok_EOS)
//     {
//         token_printer(t);
//         arrLs_append_String(&tokens, buildString(yytext));
//         t = yylex();
//     }

//     for (unsigned int i = 0; i < tokens.length; ++i) {
//         printf("Got token in ArrayList: %s\n", str_getString(tokens.list + i));
//     }

//     while (tokens.length > 0) {
//         String s = arrLs_pop_String(&tokens);
//         str_free(&s);
//     }
//     freeArrayList_String(&tokens);

//     yylex_destroy();
  
//     return 0; 
// } 

int main() {
    yy_scan_string("-((x + 4) * 5)");

    math_obj out = NULL;

    // Parse through the input:
    if (yyparse(&out) != 0) {
        fprintf(stderr, "Parsing failed\n");
        exit(1);
    }

    yylex_destroy();

    math_obj_debug_printer(out);
    putchar('\n');

    math_obj_free(out);

    return 0;
}