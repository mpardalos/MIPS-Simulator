# Notes

See [m8pple/arch2-2018-cw](https://github.com/m8pple/arch2-2018-cw) for full
spec

## Dependencies

Can be used as long as it's properly attributed and it's not something like
`mips_framework`

## IO
* Memory-mapped. 1 char wide areas for input and output. 

* Writing to output address writes out one char to the stdout, reading from
  input address consumes one char of input.

* Reading from output or writing to input is a Memory Exception (see Errors
  section)

## Errors

* When an error is encountered, exit with an exit code:
    * -10: Arithmetic exception
    * -11: Memory exception
    * -12: Invalid Instruction

## Testbench

Has to be build by running
```
make testbench
```
Which should result in an executable 
```
bin/mips_testbench
```

Does **not** have to be in c/c++