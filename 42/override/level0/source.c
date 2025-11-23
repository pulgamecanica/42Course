#include <stdio.h>
#include <stdlib.h>

/*
 * Level 0 - Hardcoded Password Vulnerability
 *
 * This program checks user input against a hardcoded password value.
 * The password (5276 / 0x149c) is stored directly in the binary
 * and can be discovered through reverse engineering.
 */

int main(void)
{
    int password;

    puts("***********************************");
    puts("* \t     -Level00 -\t\t  *");
    puts("***********************************");
    printf("Password:");

    scanf("%d", &password);

    if (password != 0x149c)  // Hardcoded password: 5276 in decimal
    {
        puts("\nInvalid Password!");
        return 1;
    }

    puts("\nAuthenticated!");
    system("/bin/sh");  // Spawns shell with elevated privileges
    return 0;
}