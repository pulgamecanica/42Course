***

# Libasm [Assembly 86x64 bits - Intel syntax]

***

[Holly Docs](https://aaronbloomfield.github.io/pdr/book/x86-64bit-asm-chapter.pdf)
[Cheats Sheet](https://cs.brown.edu/courses/cs033/docs/guides/x64_cheatsheet.pdf)
[Syscall numbers](https://filippo.io/linux-syscall-table/)
[Docs on Low Level System Software](https://refspecs.linuxbase.org/elf/x86_64-abi-0.99.pdf)
***

## Registers info

    rax - temporary register; when we call a syscal, rax must contain syscall number
    rdx - used to pass 3rd argument to functions
    rdi - used to pass 1st argument to functions
    rsi - pointer used to pass 2nd argument to functions


***

### Notes:

##### *movzx* Move with Zero-Extend
> *movzx* is an assembly instruction used in x86 and x86-64 architectures to move data
>
> from one register or memory location to another while zero-extending the source operand.
>
> This instruction is particularly useful for safely transferring smaller data types
>
> (like bytes or words) into larger registers (like double words or quad words) without
>
> inadvertently preserving any old data in the higher bits of the destination register.

##### rax
> 
> rax is also used to store the returned value for functions 
>

***

### gdb


[Reference](https://web.cecs.pdx.edu/~apt/cs510comp/gdb.pdf)

- h[elp] [keyword]
>    Displays help information.

- r[un] [args]
>    Begin program execution.
>    If the program normally takes command-line arguments (e.g., foo hi 3), you should specify them here (e.g., run hi 3).

- b[reak] [address]
>    Set a breakpoint at the specified address (or at the current address if none speci-fied).
>    Addresses can be given symbolically (e.g., foo) or numerically (e.g.*0x10a38). When execution reaches a breakpoint, you are thrown back into the gdb command line interpreter.
>

- c[ontinue]
>    Continue execution after stopping at a breakpoint.

- i[nfo] b[reak]
>    Display numbered list of all breakpoints currently set.

- d[elete] b[reakpoints] number
>    Delete specified breakpoint number.

- p[rint][/format] expr
>    Print the value of an expression using the specified format (decimal if unspecified).
>    Expressions can involve program variables or registers, which are specified using a $ rather than a % sign. Useful formats include:
>    • d decimal
>    • x hex
>    • t binary
>    • f floating point
>    • i instruction
>    • c character
>    For example, to display the value of register %rdi in decimal, type p/x $rdi.
>    Note that you need to use the 64-bit (%r) forms of register names. To see the
>    value of the current program counter, type p/x $rip.

- i[nfo] r[egisters] register
>    An alternative way to print the value of a register (or, if none is specified, of all registers) in hex and decimal.
>    Specify the register without a leading %, e.g., rdi.

- x/[count][format] [address]
>    Examine the contents of a specified memory address, or the current address if none specified.
>    If count is specified, displays specified number of words.
>    Addresses can be symbolic (e.g., main) or numeric (e.g., 0x10a44). Formats are as for print.
>    Particularly useful for printing the program text, e.g., x/100i foo disassembles and prints 100 instructions starting at foo.n

- disas[semble] address [,address]
>    Another way to print the assembly program text surrounding an address, or between two addresses.

- set var = expr
>    Set specified register or memory location to value of expression. Examples: set
>   $rdi=0x456789AB or set myVar=myVar*2.
>

- s[tep]i
>    Execute a single instruction and then return to the command line interpreter.

- n[ext]i
>    Like stepi, except that if the instruction is a subroutine call, the entire subroutine is executed before control returns to the interpreter.

- whe[re]
>    Show current activation stack.

- q[uit]
>    Exit from gdb.

```gdb
i r             // info registers
disas           s// dissasemble the memory
set <reg>=<val> // Set register to value
info variables
info locals
info args
```


***

nm -D /usr/lib/libc.so.6 | grep "__errno_location"
