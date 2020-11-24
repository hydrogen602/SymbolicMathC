## Basic Grammar for +, *, (), constants

- S  -> E $
- E  -> T E'
- E' -> + T E' | ε
- T  -> F T'
- T' -> * F T' | ε
- F  -> ( E ) | int

## Grammar for +, *, -, /, constants, vars

- E -> define var = T E' | T E'
- E' -> + T E' | ε
- T -> F T'
- T' -> * F T' | ε
- F -> O F' | - O F'
- F' -> / O | ε
- O -> var | const

Stmt ::= define var = T E'
Stmt ::= Eq E = E
Stmt ::= E
E ::= T E'
E' ::= + T E'
E' ::= ''
T ::= F T'
T' ::= * F T'
T' ::= ''
F ::= O F'
F ::= - O F'
F' ::= / O
F' ::= ''
O ::= var
O ::= const