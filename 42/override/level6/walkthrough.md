# Level 6 - Anti-Debugging with Hash-Based Authentication

## Overview

This level demonstrates **anti-debugging techniques** combined with a **custom hashing algorithm** for authentication. The program uses `ptrace()` to detect debuggers and implements a hash function that converts a username into a serial number. To exploit this, we must either bypass the ptrace check in GDB to extract the hash, or reverse-engineer the algorithm to calculate valid serials independently.

## Vulnerability: Predictable Algorithm [CWE-330](https://cwe.mitre.org/data/definitions/330.html)

### Description

This program implements security through a custom algorithm rather than cryptographic security:

**Anti-Debugging**:
- Uses `ptrace(PTRACE_TRACEME)` to detect if being debugged
- Returns -1 if already being traced (e.g., by GDB, ltrace)
- Can be bypassed by jumping over the check in GDB

**Custom Hash Algorithm**:
- Deterministic: same input always produces same output
- No cryptographic strength - can be reverse-engineered
- Based on XOR operations and modulo arithmetic

**Authentication Flow**:
1. Read username and serial number
2. Hash the username
3. Compare hash with serial
4. If match, spawn `/bin/sh`

### Key Characteristics

- **Reversible algorithm**: Can be reimplemented to calculate serials
- **No randomness**: No salt, no unpredictability
- **Debugger detection**: ptrace prevents simple debugging
- **Input validation**: Username must be >5 chars, all chars >31

## Analysis

### Initial Reconnaissance

```bash
level06@OverRide:~$ ls -la
-rwsr-s---+ 1 level07 users 12216 Sep 10  2016 level06
```

The binary runs with `level07` privileges.

### Reverse Engineering

Using GDB to analyze:

```bash
level06@OverRide:~$ gdb ./level06
(gdb) disas main
(gdb) disas auth
```

#### Main Function

Key observations:
1. **Reads login** with `fgets(login, 32, stdin)`
2. **Reads serial** with `scanf("%u", &serial)`
3. **Calls auth(login, serial)**
4. If auth returns 0, calls `system("/bin/sh")`

#### Auth Function - The Hash Algorithm

```asm
; Check length > 5
0x08048786 <+62>:   cmp    DWORD PTR [ebp-0xc],0x5
0x0804878a <+66>:   jg     0x8048796 <auth+78>

; Ptrace anti-debug check
0x080487b5 <+109>:  call   0x80485f0 <ptrace@plt>
0x080487ba <+114>:  cmp    eax,0xffffffff
0x080487bd <+117>:  jne    0x80487ed <auth+165>

; Initialize hash with login[3]
0x080487ed <+165>:  mov    eax,DWORD PTR [ebp+0x8]  ; login address
0x080487f0 <+168>:  add    eax,0x3                   ; login[3]
0x080487f3 <+171>:  movzx  eax,BYTE PTR [eax]       ; Read byte
0x080487f9 <+177>:  xor    eax,0x1337                ; XOR with 0x1337
0x080487fe <+182>:  add    eax,0x5eeded              ; Add 0x5eeded
0x08048803 <+187>:  mov    DWORD PTR [ebp-0x10],eax ; Store as hash

; Hashing loop
0x0804880f <+199>:  ; Loop start
0x08048815 <+205>:  movzx  eax,BYTE PTR [eax]       ; login[i]
0x08048818 <+208>:  cmp    al,0x1f                   ; Check > 31
0x0804882f <+231>:  xor    ecx,DWORD PTR [ebp-0x10] ; char XOR hash
; ... modulo 1337 operation ...
0x08048854 <+268>:  add    DWORD PTR [ebp-0x10],eax ; hash += result

; Compare with serial
0x08048863 <+283>:  mov    eax,DWORD PTR [ebp+0xc]  ; Load serial
0x08048866 <+286>:  cmp    eax,DWORD PTR [ebp-0x10] ; Compare with hash
0x08048869 <+289>:  je     0x8048872 <auth+298>      ; Jump if equal
```

**Hash Algorithm**:
```c
hash = (login[3] ^ 0x1337) + 0x5eeded;

for (i = 0; i < len(login); i++) {
    if (login[i] <= 31) return 1;
    temp = login[i] ^ hash;
    hash += (temp % 1337);
}

return (hash == serial) ? 0 : 1;
```

## Exploitation

### Method 1: Using GDB to Extract Hash

The ptrace check prevents normal debugging, but we can bypass it by jumping over the check.

**Step 1**: Set breakpoints

```bash
level06@OverRide:~$ gdb ./level06
(gdb) break *0x080487b5    # Before ptrace call auth<+109>
(gdb) break *0x08048866    # After hashing loop, before comparison auth<+286>
```

**Step 2**: Run and bypass ptrace

```bash
(gdb) run
Starting program: /home/users/level06/level06
***********************************
*               level06           *
***********************************
-> Enter Login: pulgamecanica
***********************************
***** NEW ACCOUNT DETECTED ********
***********************************
-> Enter Serial: 0

Breakpoint 1, 0x080487b5 in auth ()
(gdb) jump *0x080487ed    # Jump past ptrace check auth<+165>
Continuing at 0x80487ed.

Breakpoint 2, 0x08048866 in auth ()
(gdb) print *(int*)($ebp-0x10)
$1 = 6233767
```

**Step 3**: Use the extracted hash

```bash
level06@OverRide:~$ ./level06
***********************************
*               level06           *
***********************************
-> Enter Login: bonjour
***********************************
***** NEW ACCOUNT DETECTED ********
***********************************
-> Enter Serial: 6233767
Authenticated!
$ cat /home/users/level07/.pass
GbcPDRgsFK77LNnnuh7QyFYA2942Gp8yKj9KrWD8
```

### Method 2: Reverse-Engineering the Algorithm

Create a Python script that implements the same hashing algorithm:

```python
#!/usr/bin/env python3

def calculate_serial(login):
    # Validation
    if len(login) <= 5:
        return None

    for char in login:
        if ord(char) < 32:
            return None

    # Initialize hash with login[3]
    hash_val = (ord(login[3]) ^ 0x1337) + 0x5eeded

    # Loop through each character
    for i in range(len(login)):
        char = ord(login[i])

        if char <= 0x1f:
            return None

        # XOR with hash, modulo 1337, add to hash
        temp = char ^ hash_val
        hash_val += (temp % 1337)

    return hash_val & 0xFFFFFFFF

# Usage
login = "bonjour"
serial = calculate_serial(login)
print(f"Login: {login}")
print(f"Serial: {serial}")
```

**Run the script**:

```bash
level06@OverRide:~$ python3 crack_serial.py bonjour
Login:  bonjour
Serial: 6233767
```

### Method 3: Using the Provided Script

```bash
level06@OverRide:~$ python3 crack_serial.py myusername
Login:  myusername
Serial: 6232936

level06@OverRide:~$ ./level06
-> Enter Login: myusername
-> Enter Serial: 6232936
Authenticated!
$ whoami
level07
```

## Solution

### For Developers - How to Prevent This

1. **Never implement custom cryptography**:
   ```c
   // BAD - Custom hash algorithm
   hash = custom_hash(password);

   // GOOD - Use proven cryptography
   bcrypt_hash(password, salt, cost);
   ```

2. **Use proper authentication**:
   - Password hashing: bcrypt, argon2, scrypt
   - Add salts (random per-user data)
   - Use key derivation functions (KDF)

3. **Anti-debugging is not security**:
   - Ptrace can be bypassed
   - Debugger detection is security through obscurity
   - Use actual cryptographic protections

4. **Don't rely on algorithm secrecy** (Kerckhoffs's principle):
   - Assume attackers can reverse-engineer your code
   - Security should be in the keys, not the algorithm
   - Use well-tested, public algorithms

5. **Implement rate limiting**:
   ```c
   // Limit authentication attempts
   if (failed_attempts > 3) {
       sleep(5);  // Exponential backoff
   }
   ```

6. **Use secure libraries**:
   - OpenSSL / LibreSSL
   - libsodium
   - PAM (Pluggable Authentication Modules)

## Key Takeaways

- Custom cryptographic algorithms are almost always weaker than standard ones
- Anti-debugging with ptrace can be easily bypassed in GDB
- Deterministic algorithms without randomness are predictable
- Security through obscurity provides no real protection
- Reverse engineering assembly code reveals algorithm logic
- Proper authentication requires cryptographically secure hashing
- Never trust client-side validation - always validate server-side
- Use established crypto libraries instead of rolling your own

## Password

```
GbcPDRgsFK77LNnnuh7QyFYA2942Gp8yKj9KrWD8
```
