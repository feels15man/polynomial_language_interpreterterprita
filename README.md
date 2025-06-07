# Polynomial Language Intepriter

## Language
Variable names can contain only upper case characters. To set a value for variable use `=` for example ```A = x + 1```.
Its not neccessary to use `*` in polynomial expression `2*x` ~= `2x`. In polynom can be only one variable. anguage doesnt support polynoms like `2y + 7x + 3`.
For output expression result or variable value use `echo` for example `echo A` or `echo 16x + 5x`
Power sign is `^`. Language doesnt support division. There are several errors handling like 0^0, (polynom)^(polynom) etc.

## Input
The input file by default is input.txt.

## Build
Look at bison and flex paths. Change it if u need in build.sh. Also you can provide args to gcc compiler for example `./build.sh -DDEBUG` 
