#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * Level 5 - Format String Vulnerability with GOT Overwrite
 *
 * This program demonstrates a format string vulnerability that can be exploited
 * to overwrite the Global Offset Table (GOT) entry for exit(). By using the
 * %n format specifier with precise padding, we can redirect program execution
 * to shellcode stored in an environment variable.
 *
 * Key vulnerability: printf(buffer) without format string protection
 * Exploitation technique: GOT overwrite using %hn (short write)
 * Challenge: Address too large for single %n, requires two %hn writes
 */

int main(void)
{
    char buffer[100];  // Buffer at ESP + 0x28
    int i;
    int len;

    i = 0;

    // Read input from stdin (safe with size limit)
    fgets(buffer, 100, stdin);

    // Loop through buffer and convert uppercase to lowercase
    for (i = 0; i < strlen(buffer); i++)
    {
        if (buffer[i] >= 'A' && buffer[i] <= 'Z')
        {
            buffer[i] ^= 0x20;  // Toggle case bit
        }
    }

    // VULNERABLE: Unprotected printf with user-controlled format string
    printf(buffer);

    // Program exits here - exit() GOT entry can be overwritten
    exit(0);
}
