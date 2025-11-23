#include <stdio.h>
#include <stdlib.h>

/*
 * Helper program to find the address of an environment variable
 * Used to locate shellcode stored in environment variables
 *
 * Usage: ./getenv VARIABLE_NAME
 */

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Usage: %s <env_variable_name>\n", argv[0]);
        return 1;
    }

    char *addr = getenv(argv[1]);

    if (addr == NULL)
    {
        printf("Environment variable '%s' not found\n", argv[1]);
        return 1;
    }

    printf("%p\n", addr);
    return 0;
}
