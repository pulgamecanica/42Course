#!/usr/bin/env python3
"""
Level 6 - Serial Number Cracker

This script implements the same hashing algorithm as the auth() function
to calculate the correct serial number for any given login string.

Usage: python3 crack_serial.py <login>
Example: python3 crack_serial.py some_user
"""

import sys

def calculate_serial(login):
    """
    Replicates the auth() hashing algorithm
    """
    # Check length (must be > 5)
    if len(login) <= 5:
        print(f"Error: Login must be longer than 5 characters", file=sys.stderr)
        return None

    # Check all characters are >= 32 (printable ASCII)
    for char in login:
        if ord(char) < 32:
            print(f"Error: All characters must be >= 32 (printable)", file=sys.stderr)
            return None

    # Initialize hash with login[3] XOR 0x1337 + 0x5eeded
    hash_val = (ord(login[3]) ^ 0x1337) + 0x5eeded

    # Loop through each character
    for i in range(len(login)):
        char = ord(login[i])

        # Check character is > 31 (0x1f)
        if char <= 0x1f:
            print(f"Error: Character '{login[i]}' at position {i} is not > 31", file=sys.stderr)
            return None

        # XOR with current hash
        temp = char ^ hash_val

        # Complex modulo operation: temp % 1337
        # This replicates the assembly: imul, sub, shr operations
        # Essentially: temp = temp % 1337
        temp = temp % 1337

        # Add to hash
        hash_val += temp

    # Return as 32-bit unsigned integer
    return hash_val & 0xFFFFFFFF

if __name__ == "__main__":
    if len(sys.argv) != 2:
        print("Usage: python3 crack_serial.py <login>", file=sys.stderr)
        print("Example: python3 crack_serial.py some_user", file=sys.stderr)
        sys.exit(1)

    login = sys.argv[1]

    serial = calculate_serial(login)

    if serial is not None:
        print(f"Login:  {login}")
        print(f"Serial: {serial}")
        print()
        print(f"Run in VM:")
        print(f"./level06")
        print(f"-> Login: {login}")
        print(f"-> Serial: {serial}")
