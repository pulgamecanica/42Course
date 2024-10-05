[Syscall numbers](https://filippo.io/linux-syscall-table/)

    rax - temporary register; when we call a syscal, rax must contain syscall number
    rdx - used to pass 3rd argument to functions
    rdi - used to pass 1st argument to functions
    rsi - pointer used to pass 2nd argument to functions

rax is also used to store the returned value for functions 