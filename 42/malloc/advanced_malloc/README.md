This is an advanced malloc.

Keeping it as a plain byte keeps the header small and layout predictable.

alloc_kind_t (the enum) is a C type, but C doesn’t guarantee its size. By default, an enum is usually an int (32 bits), which is overkill if you only need 2–3 values.

So the choice of uint8_t was about packing efficiency:

magic → 32 bits

header_sz → 16 bits

kind → 8 bits

flags → 8 bits