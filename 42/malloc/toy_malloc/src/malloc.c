#include "malloc.h"
#include <stdint.h>
#include <stdalign.h>

#ifdef __cplusplus
extern "C" {
#endif

/*
	Inspired from Doug Leah's implementation, very useful shortcut.
*/
#define MMAP(size, prot, flags) \
 (mmap(NULL, (size), (prot), (flags)|MAP_ANONYMOUS, -1, 0))

static inline size_t align_up(size_t x, size_t a) {
    return (x + a - 1) / a * a; // generic, a>0
}

void *malloc(size_t n) {
    if (n == 0) n = 1;

    size_t A = _Alignof(max_align_t); // The max align for any given object's 't' address
    size_t header_pad = align_up(sizeof(header_t), A);

    long ps_l = sysconf(_SC_PAGESIZE);
    size_t ps = (ps_l > 0) ? (size_t)ps_l : 4096; // protection

    size_t need = header_pad + n;
    size_t mlen = align_up(need, ps);

    void *base = MMAP(mlen, PROT_READ | PROT_WRITE, MAP_PRIVATE);
    if (base == MAP_FAILED) { errno = ENOMEM; return NULL; }

    header_t *h = (header_t*)base;
    h->mapped_len = mlen;
    h->header_pad = header_pad;
    h->user = n;
    return (char*)base + header_pad;
}

void free(void *p) {
    if (!p) return;

    const size_t A  = _Alignof(max_align_t);
    const size_t hp = align_up(sizeof(header_t), A);

    header_t *h = (header_t*)((char*)p - hp);   // header is at base
    void *base  = (char*)p - hp;

    // sanity-check h->header_pad == hp
    munmap(base, h->mapped_len);
}

#ifdef __cplusplus
} /* extern "C" */
#endif
