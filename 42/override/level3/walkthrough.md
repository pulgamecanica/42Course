# Level 3 - Logic Vulnerability with XOR Decryption

## Overview

This level demonstrates a **Logic Vulnerability** combined with cryptographic analysis. The program compares two strings after passing one through a custom decryption function. By reverse engineering the encrypted string and understanding the XOR-based decryption algorithm, we can calculate the exact input value needed to successfully decrypt and match the target string.

## Vulnerability: Insufficient Entropy [CWE-331](https://cwe.mitre.org/data/definitions/331.html)

### Description

This vulnerability involves predictable cryptographic operations where:

- **Weak encryption**: Simple XOR cipher with predictable keys
- **Logic flaws**: Input validation with exploitable ranges
- **Reversible operations**: XOR encryption can be reversed by analyzing ciphertext and plaintext
- **Magic numbers**: Hardcoded values that can be discovered through reverse engineering

XOR encryption with known plaintexts (or partially known plaintexts) is trivially broken because:
```
ciphertext = plaintext XOR key
key = plaintext XOR ciphertext
```

### Key Characteristics

- **Deterministic decryption**: Same input always produces same output
- **Known target string**: "Congratulations!" is visible in the binary
- **Calculable key**: The XOR key can be derived from ciphertext analysis
- **Input validation**: Only specific difference values (1-9, 16-21) are accepted

## Analysis

### Initial Reconnaissance

```bash
level03@OverRide:~$ ls -la
-rwsr-s---+ 1 level04 users 7677 Sep 10  2016 level03
```

The binary runs with `level04` privileges.

### Reverse Engineering

Using GDB to disassemble the binary:

```bash
level03@OverRide:~$ gdb ./level03
(gdb) disas main
```

#### Main Function Analysis

Key observations from `main`:

```asm
0x08048873 <+25>:   call   0x80484b0 <time@plt>       ; Get current time
0x0804887b <+33>:   call   0x8048500 <srand@plt>      ; Seed random generator
0x080488c1 <+103>:  call   0x8048530 <__isoc99_scanf@plt>  ; Read user input
0x080488ca <+112>:  mov    DWORD PTR [esp+0x4],0x1337d00d  ; Magic number!
0x080488d5 <+123>:  call   0x8048747 <test>           ; Call test function
```

The magic number `0x1337d00d` is loaded before calling `test()`.

Converting to decimal:
```bash
$ python3 -c "print(0x1337d00d)"
322424845
```

#### Test Function Analysis

```bash
(gdb) disas test
```

The `test` function calculates the difference between the magic number and our input:

```asm
0x0804874d <+6>:    mov    eax,DWORD PTR [ebp+0x8]    ; Load input
0x08048750 <+9>:    mov    edx,DWORD PTR [ebp+0xc]    ; Load magic number
0x08048755 <+14>:   sub    ecx,eax                     ; diff = magic - input
0x0804875c <+21>:   cmp    DWORD PTR [ebp-0xc],0x15   ; Compare diff with 21
0x08048760 <+25>:   ja     0x804884a <test+259>       ; Jump if diff > 21
```

If `diff > 21`, the function calls `rand()` for an unpredictable key. Otherwise, it uses a **jump table** to call `decrypt(diff)` with specific values.

Valid `diff` values: **1-9** and **16-21** (values 10-15 are missing from the jump table).

#### Decrypt Function Analysis

```bash
(gdb) disas decrypt
```

The decrypt function contains:

1. **Encrypted string** at `ebp-0x1d`:
   ```asm
   0x08048673 <+19>:   mov    DWORD PTR [ebp-0x1d],0x757c7d51
   0x0804867a <+26>:   mov    DWORD PTR [ebp-0x19],0x67667360
   0x08048681 <+33>:   mov    DWORD PTR [ebp-0x15],0x7b66737e
   0x08048688 <+40>:   mov    DWORD PTR [ebp-0x11],0x33617c7d
   ```

2. **XOR decryption loop**:
   ```asm
   0x080486d2 <+114>:  mov    eax,DWORD PTR [ebp+0x8]    ; Load XOR key
   0x080486d5 <+117>:  xor    eax,edx                     ; XOR character with key
   0x080486df <+127>:  mov    BYTE PTR [eax],dl           ; Store decrypted char
   ```

3. **String comparison** with target:
   ```asm
   0x080486f2 <+146>:  mov    eax,0x80489c3               ; Load target string address
   0x08048700 <+160>:  repz cmps BYTE PTR ds:[esi],BYTE PTR es:[edi]  ; Compare strings
   ```

Check the target string:
```bash
(gdb) x/s 0x80489c3
0x80489c3:	"Congratulations!"
```

### Decrypting the Encrypted String

Extract the encrypted bytes (little-endian format):

```
0x757c7d51 -> 51 7d 7c 75
0x67667360 -> 60 73 66 67
0x7b66737e -> 7e 73 66 7b
0x33617c7d -> 7d 7c 61 33
```

Encrypted string: `Q}|u`sfg~sf{}|a3` (17 bytes including null terminator)

To find the XOR key, we analyze the relationship:
```
Encrypted: Q}|u`sfg~sf{}|a3
Target:    Congratulations!
```

Calculate XOR key for first character:
```python
ord('Q') ^ ord('C') = 0x51 ^ 0x43 = 0x12 = 18
```

Verify with all characters:
```python
encrypted = "Q}|u`sfg~sf{}|a3"
target = "Congratulations!"

for i in range(len(target)):
    key = ord(encrypted[i]) ^ ord(target[i])
    print(f"{encrypted[i]} XOR {target[i]} = {key}")
# All results = 18
```

**The XOR key is 18!**

### Calculating the Input

We need:
```
diff = magic_number - input = 18
input = magic_number - 18
input = 322424845 - 18
input = 322424827
```

Verification that 18 is in valid range:
```
Valid ranges: 1-9, 16-21
18 is within 16-21 ✓
```

## Exploitation

### Method 1: Direct Calculation

```bash
level03@OverRide:~$ ./level03
***********************************
*		level03		**
***********************************
Password:322424827
$ whoami
level04
$ cat /home/users/level04/.pass
kgv3tkEb9h2mLkRsPkXRfc2mHbjMxQzvb2FrgKkf
```

### Verification with Python

Create a script to verify the XOR decryption:

```python
#!/usr/bin/env python3
"""
Verify the XOR decryption for level03
"""

# Encrypted string from binary
encrypted = "Q}|u`sfg~sf{}|a3"
target = "Congratulations!"
magic_number = 322424845

# Test XOR key
xor_key = 18

# Decrypt
decrypted = ''.join(chr(ord(c) ^ xor_key) for c in encrypted)

print(f"Encrypted:  {encrypted}")
print(f"XOR key:    {xor_key}")
print(f"Decrypted:  {decrypted}")
print(f"Target:     {target}")
print(f"Match:      {decrypted == target}")
print(f"\nInput needed: {magic_number} - {xor_key} = {magic_number - xor_key}")
```

Output:
```bash
$ python3 verify_xor.py
Encrypted:  Q}|u`sfg~sf{}|a3
XOR key:    18
Decrypted:  Congratulations!
Target:     Congratulations!
Match:      True

Input needed: 322424845 - 18 = 322424827
```

## Solution

### For Developers - How to Prevent This

1. **Never use XOR for encryption with predictable keys**:
   ```c
   // VULNERABLE - Simple XOR with small key
   for (i = 0; i < len; i++)
       data[i] ^= key;

   // BETTER - Use proper encryption libraries
   #include <openssl/aes.h>
   AES_encrypt(plaintext, ciphertext, &key);
   ```

2. **Avoid hardcoded secrets in binaries**:
   - Secrets in binaries can be extracted with `strings`, `objdump`, or disassemblers
   - Use environment variables or secure key storage
   - Implement proper key derivation functions (KDF)

3. **Don't rely on security through obscurity**:
   - Assume attackers can reverse engineer your code
   - Use cryptographically secure algorithms (AES, ChaCha20)
   - Implement proper authentication (HMAC, digital signatures)

## Key Takeaways

- XOR encryption is trivially broken when plaintext is known or guessable
- Magic numbers and constants in binaries can be extracted through reverse engineering
- Jump tables in assembly reveal program logic and valid input ranges
- Simple substitution ciphers (like XOR) provide no real security

## Password

```
kgv3tkEb9h2mLkRsPkXRfc2mHbjMxQzvb2FrgKkf
```
