
# program -> statement_list
# assignment -> int Var
#
#

# lets have S always be start

'''
E -> E + T | T
T -> T * F | F
F -> ( E ) | int
'''
rules = '''
S  -> E $
E  -> T E'
E' -> + T E' | ε
T  -> F T'
T' -> * F T' | ε
F  -> ( E ) | int
'''

##rules = '''
##S -> a B D h
##B -> c C
##C -> b C | ε
##D -> E F
##E -> g | ε
##F -> f | ε'''


def processRules(rules):# -> dict:
    ruleBook = {}
    for rule in rules.split('\n'):
        if rule.strip() == "":
            continue
        nonTerm, prod = [r.strip() for r in rule.split('->')]
        ls = [tuple(e.strip().split()) for e in prod.split('|')]
        ruleBook[nonTerm] = ls

    return ruleBook


'''
Rules to compute FIRST set:

If x is a terminal, then FIRST(x) = { ‘x’ }
If x-> ε, is a production rule, then add ε to FIRST(x).
If X->Y1 Y2 Y3….Yn is a production,
    FIRST(X) = FIRST(Y1)
    If FIRST(Y1) contains ε then FIRST(X) = { FIRST(Y1) – ε } U { FIRST(Y2) }
    If FIRST (Yi) contains ε for all i = 1 to n, then add ε to FIRST(X).
'''

def firstHelper(ruleBook, nonTerm, prod):
    #print(prod)
    if prod[0] not in ruleBook:
        # terminal
        #result.add(prod[0])
        return { prod[0] }

    s = set()
    for p in prod:
        result = first(ruleBook, p)
        if 'ε' in result:
            result.discard('ε')
            s = s.union(result)
        else:
            s = s.union(result)
            break
    else:
        # never broke out, so all have 'ε'
        s.add('ε')
    return s

def first(ruleBook, nonTerm):# -> set:
    v = ruleBook[nonTerm]
    result = set()
    for prod in v:
        result = result.union(firstHelper(ruleBook, nonTerm, prod))
    return result

def computeFirst(ruleBook):
    firstBook = {}
    for k in ruleBook:
        
        firstBook[k] = first(ruleBook, k)

    return firstBook

def computeLast(ruleBook, firstBook):# -> dict:
    d = {}
    for k in ruleBook:
        if k == 'S':
            d[k] = {'$'}
        else:
            d[k] = set()

    def helperFirst(sym):# -> dict:
        if sym in firstBook:
            #if sym == 'D': print(firstBook[sym])
            return firstBook[sym].copy()
        else:
            return { sym }

    class Follow:
        def __init__(self, arg):
            self.arg = arg
        def __repr__(self):
            return f'Follow({self.arg})'

    

    for nonTerminal, productions in ruleBook.items():
        for prod in productions:
            #print(prod)
            for index, inter in enumerate(prod[:-1]):
                if inter in ruleBook:
                    # nonTerminal!
                    tmp = helperFirst(prod[index+1])
                    if 'ε' not in tmp:
                        d[inter].update(tmp)
                    else:
                        tmp.discard('ε')
                        d[inter].update(tmp)
                        # see if there are any nonterms following it
                        afterwards = prod[index+1:]
                        data = set()
                        for a in afterwards:
                            data.update(helperFirst(a))
                            #if inter == 'B': print('data', data)
                            if 'ε' not in data:
                                break
                            data.discard('ε')
                            #if inter == 'B': print('epsilon', data)
                        else:
                            # still epsilon
                            d[inter].add(Follow(nonTerminal))
                            #if inter == 'B': print('epsilon final', data)
                        d[inter].update(data)
                        #if inter == 'B': print(nonTerminal, data)
                        

            last = prod[-1]
            if last in ruleBook and last != nonTerminal:
                d[last].add(Follow(nonTerminal))
                # d[last].update(helperFirst(nonTerminal))


    def resolver(nonTerm):# -> set:
        s = d[nonTerm]
        newSet = set()
        for elem in list(s):
            if isinstance(elem, Follow):
                newSet.update(resolver(elem.arg))
            else:
                newSet.add(elem)
        return newSet

    for nonTerm in d:
        d[nonTerm] = resolver(nonTerm)
        
    return d


def getParsingTable(ruleBook, firsts, follows):
    table = {}
    nonTerms = set(ruleBook.keys())
    terms = set()
    for v in ruleBook.values():
        for prod in v:
            terms.update(prod)

    terms.difference_update(nonTerms)
    terms.discard('ε')

    result = {}
    for nT in nonTerms:
        row = dict((t, None) for t in terms)

        productions = ruleBook[nT]
        for index, prod in enumerate(productions):
            chars = firstHelper(ruleBook, nT, prod)
            if 'ε' in chars:
                chars.discard('ε')
                chars.update(followBook[nT])

            for c in chars:
                assert c in row
                if row[c] is not None:
                    raise Exception('Parse Table Conflict')
                row[c] = prod

        result[nT] = row


    return result, terms, nonTerms


def convertToCSymbolTerminal(t):
    symbolMap = {
        '(': 'L_Parens',
        ')': 'R_Parens',
        '+': 'Plus',
        '-': 'Minus',
        '*': 'Times',
        '/': 'Divide',
        '$': 'EOS',
        'ε': 'Epsilon',
        '=': 'Equal'
    }

    tmp = t
    if t in symbolMap:
        tmp = symbolMap[t]
    return f'Term_{tmp}'


def convertToCSymbolNonTerm(t):
    tmp = t.replace("'", '_Prime')
    return f'NonTerm_{tmp}'


if __name__ == '__main__':
    # rules = ''
    with open('grammar.txt') as f:
        lines = f.readlines()
        rules = '\n'.join(line for line in lines if not line.startswith('#'))

    ruleBook = processRules(rules)
    # for k, v in ruleBook.items():
    #     print(f'{k} = {v}')
    # print()

    firstBook = computeFirst(ruleBook)
    # for k, v in firstBook.items():
    #     print(f'{k} = {v}')

    # print()
    followBook = computeLast(ruleBook, firstBook)
    # for k, v in followBook.items():
    #     print(f'{k} = {v}')

    table, terms, nonTerms = getParsingTable(ruleBook, firstBook, followBook)
    # columnHeader = list(x.values())[0]
    # s = ''
    # for cH in columnHeader:
    #     s += f'| {cH:>10} '
    # print(f'\t  {s}')
    # print('-' * len(columnHeader) * (10+3) + '-' * 10)
    # for k, v in x.items():
    #     s = ''
    #     for e in v.values():
    #         s += f'| {str(e):>10} '
    #     print(f'{k}\t= {s}')

    

    # parseTable.h
    out = \
'''// This file is automatically generated

#ifndef PARSE_TABLE_H_
#define PARSE_TABLE_H_

#include <stdbool.h>

typedef enum Symbol {
    Invalid_Symbol,

'''
    cNonTermNames = []

    for t in nonTerms:
        sym = convertToCSymbolNonTerm(t)
        cNonTermNames.append(sym)
        sym += ','
        out += f'    {sym:20} // {t}\n'
    out += '\n'

    cTermNames = []

    for t in terms.union('ε'):
        sym = convertToCSymbolTerminal(t)
        cTermNames.append(sym)
        sym += ','
        out += f'    {sym:20} // {t}\n'

    out += \
'''
} Symbol;

typedef Symbol * SymbolArr;

bool ll1_isTerminal(enum Symbol s);
bool ll1_isNonTerminal(enum Symbol s);

SymbolArr ll1_lookupTable(enum Symbol nonTerm, enum Symbol nextTerm);

void ll1_printToken(enum Symbol s);

#endif
'''

    with open('parseTable.h', 'w') as f:
        f.write(out)

    def case(nT):
        out = f'        case {convertToCSymbolNonTerm(nT)}: {{\n'

        first = True
        for t in terms:
            out += ' ' * 12
            tmp = 'else ' if not first else ''
            first = False
            out += f'{tmp}if (nextTerm == {convertToCSymbolTerminal(t)}) {{\n'

            prod = table[nT][t]
            if prod is None:
                out += ' '*16 + 'return NULL;\n'
            else:
                out += ' '*16 + f'allocArr({len(prod)});\n'
                for index, p in enumerate(prod):
                    if p in terms or p == 'ε':
                        sym = convertToCSymbolTerminal(p)
                    else:
                        sym = convertToCSymbolNonTerm(p)

                    out += ' '*16 + f'arr[{index}] = {sym};\n'
                out += ' '*16 + 'return arr;\n'
            out += ' '*12 + '}\n'

        out += ' '*12 + 'else { unknownSym(nextTerm); }\n'
        out += '\n        }'
        return out
    
    # parseTable.c
    out = \
'''// This file is automatically generated

#include <stdio.h>
#include "parseTable.h"
#include "dataStructs/array.h"
#include "errors.h"

#define allocArr(n) enum Symbol * arr = newArray((n), sizeof(enum Symbol))

#define unknownSym(s) char cause[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}; snprintf(cause, 12, "%d", (s)); throw_error("Unknown Symbol", cause); return NULL

bool ll1_isTerminal(enum Symbol s) {
    switch (s) {
''' + '\n'.join([f'        case {sym}: return true;' for sym in cTermNames]) + \
'''
        default: return false;
    }
}

bool ll1_isNonTerminal(enum Symbol s) {
    return !ll1_isTerminal(s) && s != Invalid_Symbol;
}

SymbolArr ll1_lookupTable(enum Symbol nonTerm, enum Symbol nextTerm) {
    switch (nonTerm) {
''' + \
'\n'.join(case(l) for l in nonTerms) \
+ '''
        default:;
            unknownSym(nonTerm);
    }
}

void ll1_printToken(enum Symbol s) {
    switch (s) {
''' + \
'\n'.join(' '*8 + f'case {t}: printf("{t}"); break;\n' for t in (cTermNames + cNonTermNames))    \
+ '''
        default:
            printf("Unknown Token: %d\\n", s);
    }
}
'''

    '''
    int ll1_lookupToken(char c, Symbol * writeInto) {
        switch (c) {
    ''' + \
    '\n'.join(' '*8+ f'case \'{t}\': return {convertToCSymbolTerminal(t)};' for t in terms)    \
    + '''
            default:
                return 1;
        }
    }
    '''

    with open('parseTable.c', 'w') as f:
        f.write(out)
    
