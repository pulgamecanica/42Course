#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/*
 * Level 1 - Stack-Based Buffer Overflow Vulnerability
 *
 * This program demonstrates a classic buffer overflow where the password
 * buffer is too small for the fgets() call, allowing an attacker to
 * overwrite the return address and redirect execution to shellcode.
 *
 * Key vulnerability: fgets(buffer, 0x64, stdin) reads 100 bytes into
 * a buffer that has less space available in the stack frame.
 */

// Global variable to store username
char a_user_name[256];

int verify_user_name(void)
{
    puts("verifying username....\n");
    return strncmp(a_user_name, "dat_wil", 7);
}

int verify_user_pass(char *password)
{
    return strncmp(password, "admin", 5);
}

int main(void)
{
    char password[64];  // Local buffer - actual stack space is smaller!
    int auth_result;

    puts("********* ADMIN LOGIN PROMPT *********");
    printf("Enter Username: ");

    // Safe: reads into global buffer with sufficient space
    fgets(a_user_name, 256, stdin);

    auth_result = verify_user_name();
    if (auth_result != 0)
    {
        puts("nope, incorrect username...\n");
        return 1;
    }

    printf("Enter Password: ");

    // VULNERABLE: reads 100 bytes (0x64) into a buffer with less stack space
    // This allows overflow past the buffer boundary
    fgets(password, 100, stdin);

    auth_result = verify_user_pass(password);
    if (auth_result == 0 || auth_result != 0)
    {
        puts("nope, incorrect password...\n");
        return 1;
    }

    return 0;
}
