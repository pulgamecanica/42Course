#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ptrace.h>

/*
 * Level 6 - Anti-Debugging with Custom Hash Validation
 *
 * This program demonstrates anti-debugging techniques using ptrace() and
 * implements a custom hashing algorithm to validate a serial number based
 * on a username. The goal is to reverse-engineer the hashing algorithm to
 * calculate the correct serial for any given username.
 *
 * Key features:
 * - Ptrace anti-debugging check
 * - Custom hash algorithm with XOR and modulo operations
 * - Input validation (length > 5, characters > 31)
 */

int auth(char *login, unsigned int serial)
{
    int login_len;
    unsigned int hash;
    int i;

    // Remove newline from login
    login[strcspn(login, "\n")] = '\0';

    // Get login length (max 32 characters)
    login_len = strnlen(login, 32);

    // Check length > 5
    if (login_len <= 5)
    {
        return 1;
    }

    // Anti-debugging check using ptrace
    if (ptrace(PTRACE_TRACEME, 0, 1, 0) == -1)
    {
        puts("\x1b[32m.---------------------------.");
        puts("\x1b[31m| !! TAMPERING DETECTED !!  |");
        puts("\x1b[32m'---------------------------'");
        return 1;
    }

    // Initialize hash with login[3]
    hash = (login[3] ^ 0x1337) + 0x5eeded;

    // Loop through each character
    for (i = 0; i < login_len; i++)
    {
        // Check character > 31 (0x1f)
        if (login[i] <= 31)
        {
            return 1;
        }

        // Calculate hash
        // XOR character with current hash, then modulo 1337, add to hash
        unsigned int temp = (unsigned char)login[i] ^ hash;
        hash += (temp % 1337);
    }

    // Compare calculated hash with provided serial
    if (serial == hash)
    {
        return 0;  // Success
    }

    return 1;  // Failure
}

int main(int argc, char **argv)
{
    char login[32];
    unsigned int serial;

    // Display banner
    puts("***********************************");
    puts("*\t\tlevel06\t\t  *");
    puts("***********************************");

    // Get login
    printf("-> Enter Login: ");
    fgets(login, 32, stdin);

    // Display separator
    puts("***********************************");
    puts("***** NEW ACCOUNT DETECTED ********");
    puts("***********************************");

    // Get serial
    printf("-> Enter Serial: ");
    scanf("%u", &serial);

    // Authenticate
    if (auth(login, serial) == 0)
    {
        puts("Authenticated!");
        system("/bin/sh");
        return 0;
    }

    return 1;
}
