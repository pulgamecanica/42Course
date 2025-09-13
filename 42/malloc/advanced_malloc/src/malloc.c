// src/malloc.c
#include "../include/malloc.h"

#include <sys/mman.h>
#include <unistd.h>   /* sysconf */
#include <errno.h>
#include <stdint.h>
#include <string.h>   /* memset */
#include <stddef.h>   /* SIZE_MAX */

/* MAP_ANONYMOUS shim for platforms that use MAP_ANON (e.g., macOS) */
#ifndef MAP_ANONYMOUS
#  define MAP_ANONYMOUS MAP_ANON
#endif

#define MMAP(sz) mmap(NULL, (sz), PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0)

/* --- helpers --- */

static inline size_t pagesize_cached(void) {
    static size_t ps = 0;
    if (!ps) {
        long lps = sysconf(_SC_PAGESIZE);
        ps = (lps > 0) ? (size_t)lps : 4096u;
    }
    return ps;
}

/* align up without (x + a - 1) overflow */
static inline size_t align_up_sz(size_t x, size_t a) {
    size_t rem = x % a;
    if (rem == 0) return x;
    size_t add = a - rem;
    if (x > SIZE_MAX - add) return 0; /* overflow */
    return x + add;
}

#define FAIL_ENOMEM() do { errno = ENOMEM; return NULL; } while (0)

/* --- public api --- */

void *malloc(size_t n)
{
    if (n == 0) return NULL;

    const size_t hs = (size_t)FT_HDR_ALIGNED_SIZE;

    /* hs + n overflow guard */
    if (n > SIZE_MAX - hs) FAIL_ENOMEM();

    size_t need = hs + n;

    /* page-align the mapping length */
    const size_t ps = pagesize_cached();
    size_t mlen = align_up_sz(need, ps);
    if (mlen == 0) FAIL_ENOMEM();

    void *base = MMAP(mlen);
    if (base == MAP_FAILED) FAIL_ENOMEM();

    /* header at mapping base; user at base + hs */
    alloc_hdr_t *h = (alloc_hdr_t *)base;
    /* Zero just the header; no recursion risk */
    memset(h, 0, sizeof(*h));
    h->magic            = FT_MALLOC_MAGIC;
    h->kind             = ALLOC_LARGE;   /* for now everything is LARGE */
    h->user_size        = n;
    h->u.lg.mapped_len  = mlen;

    return (void *)((char *)base + hs);
}

void free(void *p)
{
    if (!p) return;

    const size_t   hs = (size_t)FT_HDR_ALIGNED_SIZE;
    alloc_hdr_t   *h  = (alloc_hdr_t *)((char *)p - hs);

    if (h->magic != FT_MALLOC_MAGIC) {
        /* bad free / foreign pointer — ignore in this minimal build */
        return;
    }

    if (h->kind == ALLOC_LARGE) {
        /* header is at mapping base */
        munmap((void *)h, h->u.lg.mapped_len);
        return;
    }

    /* TINY/SMALL zones will land here later */
}

void show_alloc_mem(void)
{
    static const char msg[] = "show_alloc_mem: not implemented yet\n";
    (void)!write(2, msg, sizeof msg - 1);
}
