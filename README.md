# Symbolic Math

Well it once was about the collatz conjecture but then we got off topic and now its about processing symbolic math.

Currently only supports calculations with integers & decimals, and only summation, subtraction, division, and multiplication.

Example: `-7 + x + 10` will turn into `3 + x`

How to run:
- Clone project
- run `make` in the folder
- run `./main`

# Notes on coding standards

Avoid any call to malloc, calloc, or free.  
For allocating strings, structs, or arrays, use the functions found in dataTypes.  
This keeps memory management cleaner and deals with nullptrs.
