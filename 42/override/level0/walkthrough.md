# Level 0 - Hardcoded Password Vulnerability

## Overview

This level demonstrates a **Hardcoded Credentials** vulnerability where sensitive authentication information is embedded directly in the application binary.

## Vulnerability: Hardcoded Credentials [CWE-798](https://cwe.mitre.org/data/definitions/798.html)

### Description

Hardcoded credentials are authentication secrets (passwords, API keys, cryptographic keys) that are embedded directly in source code or compiled binaries. This is a critical security flaw because:

- The credentials can be extracted through reverse engineering
- They cannot be easily changed without recompiling
- All instances of the software share the same credentials
- They are often discovered through static analysis

## Analysis

### Initial Reconnaissance

First, let's examine the binary:

```bash
level00@OverRide:~$ ls -la
-rwsr-s---+ 1 level01 users 7280 Sep 10  2016 level00
```

The binary has the SUID bit set (`s` in permissions), meaning it runs with `level01` privileges.

### Reverse Engineering

Using GDB or a decompiler, we can examine the binary's assembly code:

```bash
level00@OverRide:~$ gdb ./level00
(gdb) disas main
```

Key assembly instruction found:
```asm
0x080484e7 <+83>:    cmp    $0x149c,%eax
```

This compares user input against the hexadecimal value `0x149c`.

### Converting the Password

```bash
$ python3 -c "print(0x149c)"
5276
```

Or using bash:
```bash
$ echo $((16#149c))
5276
```

## Exploitation

### Method 1: Direct Input

```bash
level00@OverRide:~$ ./level00
***********************************
*        -Level00 -              *
***********************************
Password:5276

Authenticated!
$ whoami
level01
$ cat /home/users/level01/.pass
uSq2ehEGT6c9S24zbshexZQBXUGrncxn5sD5QfGL
```

### Method 2: Using echo and pipe

```bash
(echo 5276; cat) | ./level00
```

This method:
1. Sends the password `5276` to the program
2. Keeps stdin open with `cat` to interact with the spawned shell

## Solution

### For Developers - How to Prevent This

1. **Never hardcode credentials** in source code
2. **Use environment variables** for configuration
3. **Implement proper authentication systems**:
   ```c
   // Bad - Hardcoded
   if (password == 5276) { ... }

   // Good - External configuration
   const char* valid_pass = getenv("APP_PASSWORD");
   if (strcmp(password, valid_pass) == 0) { ... }
   ```
4. **Use password hashing** (bcrypt, argon2, scrypt)
5. **Implement account lockout** after failed attempts
6. **Regular security audits** and static code analysis
7. **Secrets management tools** (HashiCorp Vault, AWS Secrets Manager)

## Key Takeaways

- Credentials should NEVER be hardcoded in applications
- SUID binaries with authentication are high-value targets
- Reverse engineering can reveal hardcoded secrets
- Defense in depth: even if one layer fails, others should protect the system

## Password

```
uSq2ehEGT6c9S24zbshexZQBXUGrncxn5sD5QfGL
```
