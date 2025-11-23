#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * Level 2 - Format String Vulnerability (Information Disclosure)
 *
 * This program reads the next level's password into a stack buffer, then
 * uses an unprotected printf() to display the username. This allows an
 * attacker to use format string specifiers to read arbitrary stack memory,
 * including the password buffer.
 *
 * Key vulnerability: printf(username) instead of printf("%s", username)
 * This allows format string attacks using %p, %x, and positional parameters.
 */

int main(int argc, char **argv)
{
    char username[112];      // Buffer at RBP - 0x70
    char password[100];      // Buffer at RBP - 0x110
    char flag_buffer[41];    // Buffer at RBP - 0xa0 (holds level03 password)
    FILE *flag_file;
    int bytes_read;

    // Initialize buffers to zero
    memset(username, 0, sizeof(username));
    memset(flag_buffer, 0, sizeof(flag_buffer));
    memset(password, 0, sizeof(password));

    // VULNERABILITY SETUP: Read next level's password into stack
    flag_file = fopen("/home/users/level03/.pass", "r");
    if (!flag_file)
    {
        fwrite("ERROR: failed to open password file\n", 1, 36, stderr);
        exit(1);
    }

    // Read 41 bytes (0x29) from password file into stack buffer
    bytes_read = fread(flag_buffer, 1, 0x29, flag_file);

    // Remove trailing newline
    flag_buffer[strcspn(flag_buffer, "\n")] = 0;

    if (bytes_read != 0x29)
    {
        fwrite("ERROR: failed to read password file\n", 1, 36, stderr);
        fwrite("ERROR: failed to read password file\n", 1, 36, stderr);
        exit(1);
    }

    fclose(flag_file);

    // Display login prompt
    puts("===== [ Secure Access System v1.0 ] =====");
    puts("/***************************************\\");
    puts("| You must login to access this system. |");
    puts("\\**************************************/");

    printf("--[ Username: ");
    fgets(username, 100, stdin);
    username[strcspn(username, "\n")] = 0;  // Remove newline

    printf("--[ Password: ");
    fgets(password, 100, stdin);
    password[strcspn(password, "\n")] = 0;  // Remove newline

    puts("*****************************************");

    // Compare password with the flag we read earlier
    if (strncmp(flag_buffer, password, 0x29) == 0)
    {
        printf("Greetings, %s!\n", username);
        system("/bin/sh");
        return 0;
    }

    // VULNERABILITY: Unprotected printf() - allows format string exploitation
    printf(username);
    puts(" does not have access!");
    exit(1);
}
