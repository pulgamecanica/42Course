#!/usr/bin/env python3
"""
Verify the XOR decryption for level03
Demonstrates how the encrypted string is decrypted with key 18
"""

# Encrypted string from binary (hex values from memory)
encrypted_hex = "757c7d51 67667360 7b66737e 33617c7d".replace(' ', '')
encrypted_bytes = bytes.fromhex(encrypted_hex)

# Convert to ASCII string (little-endian)
encrypted = ""
for i in range(0, len(encrypted_bytes), 4):
    # Reverse each 4-byte chunk (little-endian)
    chunk = encrypted_bytes[i:i+4][::-1]
    encrypted += chunk.decode('ascii', errors='ignore')

# Alternative: direct string representation
encrypted = "Q}|u`sfg~sf{}|a3"
target = "Congratulations!"
magic_number = 322424845

print("=" * 60)
print("Level 03 - XOR Decryption Analysis")
print("=" * 60)

# Find XOR key by comparing encrypted and target
print("\nFinding XOR key:")
xor_keys = []
for i in range(len(target)):
    key = ord(encrypted[i]) ^ ord(target[i])
    xor_keys.append(key)
    print(f"  '{encrypted[i]}' (0x{ord(encrypted[i]):02x}) XOR '{target[i]}' (0x{ord(target[i]):02x}) = {key}")

# Verify all keys are the same
if len(set(xor_keys)) == 1:
    xor_key = xor_keys[0]
    print(f"\n✓ All characters use the same XOR key: {xor_key}")
else:
    print(f"\n✗ Different keys found: {set(xor_keys)}")
    xor_key = max(set(xor_keys), key=xor_keys.count)
    print(f"  Using most common: {xor_key}")

# Decrypt with found key
print(f"\nDecryption verification:")
decrypted = ''.join(chr(ord(c) ^ xor_key) for c in encrypted)

print(f"  Encrypted:  '{encrypted}'")
print(f"  XOR key:    {xor_key}")
print(f"  Decrypted:  '{decrypted}'")
print(f"  Target:     '{target}'")
print(f"  Match:      {decrypted == target} {'✓' if decrypted == target else '✗'}")

# Calculate input needed
input_needed = magic_number - xor_key

print(f"\nInput calculation:")
print(f"  Magic number:        {magic_number} (0x{magic_number:08x})")
print(f"  XOR key needed:      {xor_key}")
print(f"  Difference formula:  magic_number - input = xor_key")
print(f"  Input needed:        {magic_number} - {xor_key} = {input_needed}")

# Verify input is in valid range
valid_ranges = list(range(1, 10)) + list(range(16, 22))
if xor_key in valid_ranges:
    print(f"  Valid range check:   {xor_key} is in {valid_ranges} ✓")
else:
    print(f"  Valid range check:   {xor_key} is NOT in {valid_ranges} ✗")

print("\n" + "=" * 60)
print(f"SOLUTION: Enter {input_needed} as password")
print("=" * 60)
