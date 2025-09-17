This is an advanced malloc.

Keeping it as a plain byte keeps the header small and layout predictable.

alloc_kind_t (the enum) is a C type, but C doesn’t guarantee its size. By default, an enum is usually an int (32 bits), which is overkill if you only need 2–3 values.

So the choice of uint8_t was about packing efficiency:

magic → 32 bits

header_sz → 16 bits

kind → 8 bits

flags → 8 bits

// Why using an enum and not a define?

We could use a define, we just prefer enum.

Bottom line: we used the enum here because it gives a clean, storage-free, compile-time constant without macro gotchas—and your value comfortably fits in int. If you prefer a typed size_t, switch to static const size_t and you’re still golden.


---


block_start
  + BLOCK_HDR          → alloc_hdr_start  (A-aligned)
  + ALLOC_HDR          → user_ptr         (A-aligned)
  + PAYLOAD            → next_block_start (A-aligned)


---

zone mapping
┌──────────────────────────────────────────────────────────────────────┐
│ [ zone header + padding ]                                            │
│                                                                      │
│  Block #k (ALLOCATED)                                                │
│  ┌──────────────┬───────────────┬───────────────────────────────┐    │
│  │ BlockHeader  │ AllocHeader   │  user payload …               │    │
│  └──────────────┴───────────────┴───────────────────────────────┘    │
│   ^ start of block              ^ user pointer is returned here      │
│                                                                      │
│  Block #k+1 (FREE)                                                   │
│  ┌──────────────┬───────────────────────────────────────────────┐    │
│  │ BlockHeader  │   (bytes unused until next allocation)        │    │
│  └──────────────┴───────────────────────────────────────────────┘    │
│                                                                      │
└──────────────────────────────────────────────────────────────────────┘
