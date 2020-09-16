
# What to do with minus?

- `a - b`   => `a + -b`
- `a * -b`  => `a * (-b)`

Difference:
- if `-` is the only symbol between two vars/const, then replace it with `+ -`.


## Product & Division
- a * b * c * d => ((a * b) * c) * d
- a * b / c * d => ((a * b) / c) * d