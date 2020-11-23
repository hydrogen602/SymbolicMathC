#include "lexer.h"
#include "tokens.h"
#include <stdio.h>

#include "../dataStructs/str.h"
#define VAL_T String
#include "../dataStructs/arrayList.h"

char *yytext;

int main() { 
  
    // Explanation: 
    // yywrap() - wraps the above rule section 
    /* yyin - takes the file pointer  
            which contains the input*/
    /* yylex() - this is the main flex function 
            which runs the Rule Section*/
    // yytext is the text in the buffer 
    
    // Uncomment the lines below  
    // to take input from file 
    // FILE *fp; 
    // char filename[50]; 
    // printf("Enter the filename: \n"); 
    // scanf("%s",filename); 
    // fp = fopen(filename,"r"); 
    // yyin = fp; 

    ArrayList_String tokens = newArrayList_String();


    yy_scan_string("a+a");
  
    Token t = yylex();
    while (t != Tok_EOS)
    {
        printf("Got token %d\n", t);
        arrLs_append_String(&tokens, buildString(yytext));
        t = yylex();
    }

    for (int i = 0; i < tokens.length; ++i) {
        printf("Got token in ArrayList: %s\n", str_getString(tokens.list + i));
    }

    while (tokens.length > 0) {
        String s = arrLs_pop_String(&tokens);
        str_free(&s);
    }
    freeArrayList_String(&tokens);

    yylex_destroy();
  
    return 0; 
} 