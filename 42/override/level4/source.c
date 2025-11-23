#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/ptrace.h>
#include <sys/prctl.h>
#include <signal.h>

/*
 * Level 4 - Buffer Overflow with Fork and Ptrace Protection
 *
 * This program demonstrates a buffer overflow vulnerability complicated by:
 * - Process forking: Child process handles user input
 * - Ptrace anti-debugging: Detects use of execve() syscalls
 * - Parent monitoring: Watches child process for suspicious behavior
 *
 * Key vulnerability: gets() with no bounds checking in child process
 * Challenge: Cannot use execve-based shellcode due to ptrace detection
 * Solution: Use open/read/write shellcode to read password file
 */

int main(void)
{
    char buffer[128];      // Buffer at ESP + 0x20
    pid_t child_pid;
    int status;
    int signal_num;
    int ptrace_result;

    // Fork the process
    child_pid = fork();

    // Initialize buffer
    memset(buffer, 0, sizeof(buffer));
    status = 0;

    if (child_pid == 0)
    {
        // CHILD PROCESS

        // Enable parent tracing
        prctl(PR_SET_PTRACER, 1, 0, 0, 0);

        // Allow parent to trace this process
        ptrace(PTRACE_TRACEME, 0, 0, 0);

        // Display prompt
        puts("Give me some shellcode, k");

        // VULNERABLE: gets() with no bounds checking!
        gets(buffer);
    }
    else
    {
        // PARENT PROCESS

        // Wait for child to change state
        while (1)
        {
            wait(&status);

            // Check if child exited normally
            if (WIFEXITED(status) || WIFSIGNALED(status))
            {
                puts("child is exiting...");
                break;
            }

            // Get signal that stopped the child
            ptrace_result = ptrace(PTRACE_PEEKUSER, child_pid, 44, 0);

            // Check if child tried to execute syscall 0xb (execve)
            if (ptrace_result == 0xb)
            {
                puts("no exec() for you");
                kill(child_pid, SIGKILL);
                break;
            }
        }
    }

    return 0;
}
