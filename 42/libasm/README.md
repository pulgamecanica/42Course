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

#### gdb

```gdb
i r             // info registers
disas           s// dissasemble the memory
set <reg>=<val> // Set register to value
info variables
info locals
info args
```