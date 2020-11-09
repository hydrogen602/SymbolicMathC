#include "dataStructs/parseTable.h"
#include "dataStructs/array.h"
#include "dataStructs/str.h"
#include "dataStructs/abstractSyntaxTree.h"
#include "errors.h"
#include <stdio.h>
#include <regex.h>

typedef struct __PARSER2_STACK {
    Symbol * stack;
    int size;
} Parser2Stack;

Parser2Stack __buildStack() {
    Parser2Stack s;
    s.stack = newArray(16, sizeof(Symbol));
    s.size = 0;
    return s;
}

void __push(Parser2Stack *s, Symbol sym) {
    if (s->stack == NULL) {
        throw_error("NullPtrException", "stack is null");
    }
    int currStackSz = len(s->stack);
    if (s->size >= currStackSz) {
        // increase stack size!!!
        
        Symbol * newArr = newArray(currStackSz * 2, sizeof(Symbol));
        for (int i = 0; i < currStackSz; ++i) {
            newArr[i] = s->stack[i];
        }
        freeArray(s->stack);
        s->stack = newArr;
    }

    if (s->size >= (long) len(s->stack)) {
        throw_error("This should never happen", "Stack Overflow in expanding stack???");
    }

    s->stack[s->size] = sym;
    ++(s->size);
}

Symbol __peek(Parser2Stack *s) {
    if (s->stack == NULL) {
        throw_error("NullPtrException", "stack is null");
    }
    if (s->size <= 0) {
        char cause[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
        snprintf(cause, 12, "%d", (s->size));
        throw_error("Stack underflow", cause);
        return Term_EOS;
    }
    return s->stack[s->size - 1];
}

Symbol __pop(Parser2Stack *s) {
    Symbol sym = __peek(s);
    --(s->size);
    return sym;
}

void __freeStack(Parser2Stack *s) {
    freeArray(s->stack);
    s->size = 0;
    s->stack = NULL;
}

void __printStack(Parser2Stack *s) {
    printf("Stack size = %d, ptr = %p\n", s->size, (void*)s->stack);
    printf("Stack dump: ");
    for (int i = 0; i < s->size; ++i) {
        ll1_printToken(s->stack[i]);
        putchar(' ');
    }
    putchar('\n');
}

/*
 * 
 * Parser
 * 
 */

void parser2(SymbolArr input) {
    Parser2Stack st = __buildStack();
    //__push(&st, Term_EOS); Term_EOS is part of S production
    __push(&st, NonTerm_S);

    //Parser2Stack semanticSt = __buildStack();

    //AST tree = ast_build();

    unsigned int curIndex = 0;

    while(st.size > 0) {
        Symbol x = __pop(&st);

        Symbol a;
        if (curIndex >= len(input)) {
            a = Term_EOS;
        }
        else {
            a = input[curIndex]; // look ahead
        }

        __printStack(&st);

        printf("x = ");
        ll1_printToken(x);
        putchar('\n');

        printf("a = ");
        ll1_printToken(a);
        putchar('\n');
        

        if (ll1_isTerminal(x) || x == Term_EOS) { // the second part is redundent but added just in case I mess things up
            if (x == Term_Epsilon) {
                printf("Read epsilon\n");
            }
            else if (x == a) {
                ++curIndex;
                printf("Read token ");
                ll1_printToken(a);
                putchar('\n');
            }
            else {
                throw_error("Parse Error", "Terminal mismatch");
            }
        }
        else {
            SymbolArr production = ll1_lookupTable(x, a);
            if (production == NULL) {
                throw_error("Parse Error", "Lookup table returns NULL");
            }
            else {
                for (int i = len(production) -1; i >= 0 ; --i) {
                    __push(&st, production[i]);
                }

                freeArray(production);
            }
        }
    }

    if (curIndex < len(input)) {
        printf("curIndex = %d, len(input) = %lu\n", curIndex, len(input));
        throw_error("Parse Error", "Not all tokens consumned");
    }
    else {
        // accept
        printf("Accept\n");
    }


    __freeStack(&st);
}

void stackTest() {
    Parser2Stack st = __buildStack();
    for (int i = 0; i < 10; ++i) {
        __push(&st, i);
    }

    for (int i = 9; i >= 0; --i) {
        if (__pop(&st) != i) {
            fprintf(stderr, "fail at %d\n", i); 
        }
    }    

    __freeStack(&st);
}

// Lexer

SymbolArr lexer(String str) {
    //char * s = str_getString(&str);
    //int length = str_getLen(&str);

    Parser2Stack ps = __buildStack();

    // const int maxTokenLen = 50;

    // int soFarIndex = 0;

    // for (int i = 0; i < length; ++i) {
    //     char c = s[i];
    //     // char soFar[maxTokenLen+1];
    //     // for (int j = 0; j < maxTokenLen+1; ++j) {
    //     //     soFar[j] = 0;
    //     // }

    //     Symbol tok = Invalid_Symbol;        

    //     if (tok == Invalid_Symbol) {
    //         throw_error("Cannot parse", s);
    //     }

    //     __push(&ps, tok);
    // }

    regex_t patternConst;
    const char constRegex[] = "^[-]?[0-9]+([.][0-9]*)?$";
    regex_t patternVar;
    const char varRegex[] = "^[[:alpha:]_][[:alpha:][:digit:]_]*$";

    if (regcomp(&patternConst, constRegex, REG_EXTENDED) != 0) {
        throw_error("Regex invalid", constRegex);
    }
    if (regcomp(&patternVar, varRegex, REG_EXTENDED) != 0) {
        throw_error("Regex invalid", varRegex);
    }

    StringArray tokens = str_split(&str, ' ');

    for (unsigned int i = 0; i < len(tokens); ++i) {
        const char * tok = str_getString(tokens+i);
        if (str_isEqualCString(tokens+i, "define")) {
            __push(&ps, Term_define);
        }
        else if (str_isEqualCString(tokens+i, "+")) {
            __push(&ps, Term_Plus);
        }
        else if (str_isEqualCString(tokens+i, "-")) {
            __push(&ps, Term_Minus);
        }
        else if (str_isEqualCString(tokens+i, "*")) {
            __push(&ps, Term_Times);
        }
        else if (str_isEqualCString(tokens+i, "/")) {
            __push(&ps, Term_Divide);
        }
        else if (str_isEqualCString(tokens+i, "=")) {
            __push(&ps, Term_Equal);
        }
        else if (!regexec(&patternConst, tok, 0, NULL, 0)) {
            __push(&ps, Term_const);
        }
        else if (!regexec(&patternVar, tok, 0, NULL, 0)) {
            __push(&ps, Term_var);
        }
        else {
            throw_error("Unrecognized symbol", tok);
        }

    }

    __push(&ps, Term_EOS);

    SymbolArr arr = newArray(ps.size, sizeof(Symbol));
    for (int i = ps.size - 1; i >= 0; --i) {
        arr[i] = __pop(&ps);
    }

    __freeStack(&ps);
    return arr;
}