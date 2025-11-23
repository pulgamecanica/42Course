#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/*
 * Level 3 - Logic Vulnerability with XOR Decryption
 *
 * This program uses a custom decryption function that XORs an encrypted string
 * with a user-provided value. By reverse engineering the encrypted string and
 * understanding the decryption logic, we can calculate the exact input needed
 * to decrypt the string and match the comparison target.
 *
 * Key vulnerability: Predictable decryption algorithm with a magic number
 * that can be reverse engineered to find the correct input.
 */

void decrypt(int xor_key)
{
    char encrypted_str[17] = "Q}|u`sfg~sf{}|a3";  // Encrypted string in memory
    char target[] = "Congratulations!";
    unsigned int str_len;
    unsigned int i;

    // Calculate string length
    str_len = strlen(encrypted_str);

    // XOR each character with the key
    for (i = 0; i < str_len; i++)
    {
        encrypted_str[i] = encrypted_str[i] ^ xor_key;
    }

    // Compare decrypted string with target
    if (strncmp(encrypted_str, target, 17) == 0)
    {
        printf("Congratulations!\n");
        system("/bin/sh");
    }
    else
    {
        printf("\nInvalid Password\n");
    }
}

void test(unsigned int input, unsigned int magic_number)
{
    int diff;

    // Calculate difference between magic number and input
    diff = magic_number - input;

    // Check if difference is in valid range (1-21)
    // Values 10-15 are skipped (jump table holes)
    switch (diff)
    {
        case 1:
        case 2:
        case 3:
        case 4:
        case 5:
        case 6:
        case 7:
        case 8:
        case 9:
        case 16:
        case 17:
        case 18:  // TARGET: We need this value!
        case 19:
        case 20:
        case 21:
            decrypt(diff);
            break;
        default:
            // If difference is outside valid range, use random number
            decrypt(rand());
            break;
    }
}

int main(void)
{
    unsigned int input;

    // Initialize random number generator
    srand(time(NULL));

    // Display banner
    puts("***********************************");
    puts("*\t\tlevel03\t\t**");
    puts("***********************************");
    printf("Password:");

    // Read user input as unsigned integer
    scanf("%u", &input);

    // Call test with magic number 0x1337d00d (322424845)
    test(input, 0x1337d00d);

    return 0;
}
