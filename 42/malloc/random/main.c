/*
cc -std=c11 -O2 main.c -o malloc_testing 
# make sure you're NOT preloading your custom allocator:
nm malloc_testing | grep "malloc"
./malloc_testing
*/

#define _GNU_SOURCE 1  /* for malloc_info prototype on glibc */
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <malloc.h>     /* malloc_usable_size, malloc_info, malloc_stats */

#ifndef __GLIBC__
#warning "This test is intended for glibc (malloc_stats/malloc_info). Some functions may be unavailable."
#endif

static void banner(const char *title) {
    fprintf(stdout, "\n\033[1;36m== %s ==\033[0m (pid=%ld)\n", title, (long)getpid());
}

static void dump_stats(const char *why) {
    banner(why);
    fprintf(stdout, "-- malloc_stats() --\n");
    fflush(stdout);
    malloc_stats(); /* prints to stdout */
    fprintf(stdout, "\n-- malloc_info(0, stdout) (XML) --\n");
    (void)malloc_info(0, stdout);
    fprintf(stdout, "\n");
}

// static size_t sum_usable(void **ptrs, size_t n, size_t *out_req) {
//     size_t total = 0, req = 0;
//     for (size_t i = 0; i < n; ++i) if (ptrs[i]) {
//         total += malloc_usable_size(ptrs[i]);
//     }
//     if (out_req) *out_req = req;
//     return total;
// }

static void touch(void *p, size_t n, unsigned pattern) {
    if (!p || n == 0) return;
    memset(p, (int)(pattern & 0xFFu), n);
}

int main(void) {
    srand(12345u);

    banner("GLIBC malloc playground");

    /* Phase A: sprinkle tiny & small allocations */
    enum { N = 64 };
    void *v[N] = {0};
    size_t req[N] = {0};

    for (size_t i = 0; i < N; ++i) {
        size_t r = (size_t)(rand() % 2048) + 1;   /* 1..2048 bytes */
        req[i] = r;
        v[i] = malloc(r);
        touch(v[i], r, (unsigned)i);
        if (i % 8 == 0) {
            fprintf(stdout, "A%02zu: req=%4zu usable=%4zu ptr=%p\n",
                    i, r, v[i] ? malloc_usable_size(v[i]) : 0, v[i]);
        }
    }
    fprintf(stdout, "\nA: allocated %zu pointers (1..2048 bytes)\n", (size_t)N);
    dump_stats("after Phase A allocations");

    /* Phase B: free every other pointer to create holes */
    for (size_t i = 0; i < N; i += 2) {
        free(v[i]); v[i] = NULL;
    }
    fprintf(stdout, "B: freed every other pointer to create fragmentation\n");
    dump_stats("after Phase B frees");

    /* Phase C: realloc some survivors up/down to exercise growth/shrink */
    for (size_t i = 1; i < N; i += 4) {
        size_t old_u = v[i] ? malloc_usable_size(v[i]) : 0;
        size_t new_req = (req[i] % 2) ? (req[i] / 2 + 1) : (req[i] * 3 + 13);
        void *np = realloc(v[i], new_req);
        fprintf(stdout, "C%02zu: realloc %p (old usable=%4zu, req=%4zu) -> ",
                i, v[i], old_u, new_req);
        v[i] = np; req[i] = new_req;
        if (np) {
            size_t new_u = malloc_usable_size(np);
            fprintf(stdout, "%p (new usable=%4zu)\n", np, new_u);
            touch(np, new_req, 0xCC);
        } else {
            fprintf(stdout, "NULL (OOM?)\n");
        }
    }
    dump_stats("after Phase C realloc churn");

    /* Phase D: a few bigger spikes (SMALL/LARGE territory) */
    void *spikes[6] = {0};
    size_t spike_sz[] = { 8192, 16384, 65536, 262144, 1048576, 5*1048576 }; /* 8 KiB .. 5 MiB */
    for (size_t i = 0; i < 6; ++i) {
        spikes[i] = malloc(spike_sz[i]);
        touch(spikes[i], spike_sz[i], 0xAA);
        fprintf(stdout, "D%zu: spike req=%7zu usable=%7zu ptr=%p\n",
                i, spike_sz[i], spikes[i] ? malloc_usable_size(spikes[i]) : 0, spikes[i]);
    }
    dump_stats("after Phase D spikes");

    /* Phase E: free some smalls, some spikes, then allocate a staircase */
    for (size_t i = 3; i < N; i += 6) { if (v[i]) { free(v[i]); v[i] = NULL; } }
    for (size_t i = 1; i < 6; i += 2) { if (spikes[i]) { free(spikes[i]); spikes[i] = NULL; } }

    enum { M = 24 };
    void *stairs[M] = {0};
    for (size_t i = 0; i < M; ++i) {
        size_t r = (i+1) * 1024;  /* 1 KiB, 2 KiB, ... 24 KiB */
        stairs[i] = malloc(r);
        touch(stairs[i], r, 0x55 + (int)i);
        if (i % 6 == 0) {
            fprintf(stdout, "E%02zu: stair req=%5zu usable=%5zu ptr=%p\n",
                    i, r, stairs[i] ? malloc_usable_size(stairs[i]) : 0, stairs[i]);
        }
    }
    dump_stats("after Phase E stairs");

    /* Phase F: clean up */
    for (size_t i = 0; i < N; ++i) if (v[i]) free(v[i]);
    for (size_t i = 0; i < 6; ++i) if (spikes[i]) free(spikes[i]);
    for (size_t i = 0; i < M; ++i) if (stairs[i]) free(stairs[i]);

    dump_stats("after Phase F cleanup (should be mostly quiescent)");

    fprintf(stdout, "\nDone. Tip: run with GLIBC_TUNABLES or MALLOC_* env vars to experiment with arenas/bins.\n");
    return 0;
}
