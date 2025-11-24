#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * Level 7 - Arbitrary Memory Write via Integer Array
 *
 * This program implements a simple number storage interface with three commands:
 * - store: saves a number at a given index
 * - read: displays the number at a given index
 * - quit: exits the program
 *
 * Vulnerability: The program allows writing to arbitrary memory locations
 * by manipulating the index parameter. There are checks to prevent certain
 * indices, but they can be bypassed using integer overflow.
 *
 * Key features:
 * - Clears environment variables and argv (prevents shellcode in env)
 * - Stack canary protection enabled
 * - NX disabled (stack executable)
 * - Checks: index % 3 != 0 and (number >> 24) != 0xb7
 */

unsigned int get_unum(void)
{
    unsigned int result = 0;
    fflush(stdout);
    scanf("%u", &result);
    // Clear stdin
    while (getchar() != '\n');
    return result;
}

int store_number(unsigned int *database)
{
    unsigned int number;
    unsigned int index;

    printf(" Number: ");
    number = get_unum();

    printf(" Index: ");
    index = get_unum();

    // Check 1: index must not be divisible by 3
    // Check 2: high byte of number must not be 0xb7
    if ((index % 3 == 0) || ((number >> 24) == 0xb7))
    {
        puts(" *** ERROR! ***");
        puts("   This index is reserved for wil!");
        puts(" *** ERROR! ***");
        return 1;
    }

    // VULNERABLE: Write to database[index] without bounds checking!
    // index is shifted left by 2 (multiplied by 4) to get byte offset
    database[index] = number;

    return 0;
}

int read_number(unsigned int *database)
{
    unsigned int index;

    printf(" Index: ");
    index = get_unum();

    // Read from database[index] without bounds checking!
    printf(" Number at data[%u] is %u\n", index, database[index]);

    return 0;
}

int main(int argc, char **argv, char **envp)
{
    char command[20];
    unsigned int database[100];  // Array at ESP+0x24
    int ret = 0;

    // Clear all argv strings (prevents storing shellcode in args)
    while (*argv)
    {
        memset(*argv, 0, strlen(*argv));
        argv++;
    }

    // Clear all environment variables (prevents storing shellcode in env)
    while (*envp)
    {
        memset(*envp, 0, strlen(*envp));
        envp++;
    }

    puts("----------------------------------------------------\n"
         "  Welcome to wil's crappy number storage service!   \n"
         "----------------------------------------------------\n"
         " Commands:                                          \n"
         "    store - store a number into the data storage    \n"
         "    read  - read a number from the data storage     \n"
         "    quit  - exit the program                        \n"
         "----------------------------------------------------\n"
         "   wil has reserved some storage :>                 \n"
         "----------------------------------------------------\n");

    while (ret == 0)
    {
        printf("Input command: ");
        ret = 1;

        fgets(command, 20, stdin);
        command[strlen(command) - 1] = '\0';  // Remove newline

        if (strncmp(command, "store", 5) == 0)
            ret = store_number(database);
        else if (strncmp(command, "read", 4) == 0)
            ret = read_number(database);
        else if (strncmp(command, "quit", 4) == 0)
            break;

        if (ret == 0)
        {
            printf(" Completed %s command successfully\n", command);
        }
        else
        {
            printf(" Failed to do %s command\n", command);
        }
    }

    return 0;
}
