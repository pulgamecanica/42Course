// tests/main.c
#include "malloc.h"
#include <string.h>   // memset
#include <unistd.h>   // write

static void say(const char *s) { (void)!write(1, s, (size_t)strlen(s)); }

int main(void) {
    void *p[16] = {0};

    /* Phase 1: assorted sizes across TINY/SMALL/LARGE */
    size_t sz[] = {
        8,    32,   200,  400,  512,   /* TINY (<= FT_TINY_MAX) */
        513,  700,  2048, 8192,        /* SMALL (<= FT_SMALL_MAX) */
        9000                           /* LARGE  (> FT_SMALL_MAX) */
    };
    int nsz = (int)(sizeof(sz)/sizeof(sz[0]));

    for (int i = 0; i < nsz; ++i) {
        p[i] = malloc(sz[i]);
        if (p[i]) memset(p[i], 0xA5, sz[i]); // touch memory
    }

    say("\n--- after initial allocations ---\n");
    show_alloc_mem();

    /* Phase 2: free a few to create gaps (both tiny and small) */
    free(p[2]);  p[2]  = NULL;  // 200 (TINY)
    free(p[4]);  p[4]  = NULL;  // 512 (TINY)
    free(p[6]);  p[6]  = NULL;  // 700 (SMALL)
    free(p[8]);  p[8]  = NULL;  // 8192 (SMALL)

    say("\n--- after frees (created holes) ---\n");
    show_alloc_mem();

    /* Phase 3: allocate sizes that should fit into freed holes (exercise first-fit + split) */
    void *a = malloc(128);   if (a) memset(a, 0x5A, 128);   // likely lands in freed 200/512 area
    void *b = malloc(300);   if (b) memset(b, 0x5B, 300);   // should also reuse TINY free space
    void *c = malloc(4096);  if (c) memset(c, 0x5C, 4096);  // should reuse SMALL free space

    say("\n--- after more allocations (reusing holes) ---\n");
    show_alloc_mem();

    /* Phase 4: clean up */
    for (int i = 0; i < nsz; ++i) if (p[i]) free(p[i]);
    if (a) free(a);
    if (b) free(b);
    if (c) free(c);

    say("\n--- after final frees ---\n");
    show_alloc_mem();

    /* Note: LARGE allocations are not listed by show_alloc_mem yet (by design). */
    return 0;
}
