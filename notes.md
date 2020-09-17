# Code policies

Crashing
- throw_error if the user is at fault
- assert if the code is at fault


# What to do with minus?

- `a - b`   => `a + -b`
- `a * -b`  => `a * (-b)`

Difference:
- if `-` is the only symbol between two vars/const, then replace it with `+ -`.


## Product & Division
- a * b * c * d => ((a * b) * c) * d
- a * b / c * d => ((a * b) / c) * d

## How to print fractions?

a * b - 4  
\---------  
x - 1

## Assigning vars

define x = 1 ?

