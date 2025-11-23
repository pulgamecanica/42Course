#!/usr/bin/env python3
"""
Decode the password from format string leak
The password is stored in little-endian format on the stack
"""
import sys

# Raw output from format string attack (can be passed as argument or use default)
if len(sys.argv) > 1:
    leaked = sys.argv[1]
else:
    leaked = '0x756e5052343768480x45414a35617339510x377a7143574e67580x354a35686e4758730x48336750664b394d'

password = ""

# Split by '0x' and process each hex chunk
for chunk in leaked.split('0x'):
    if chunk:  # Skip empty strings
        # Decode hex to bytes and reverse (little-endian)
        password += bytes.fromhex(chunk).decode('ascii')[::-1]

print("Password:", password)
