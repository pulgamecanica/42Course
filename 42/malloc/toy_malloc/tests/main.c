#include "malloc.h"

#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdalign.h>   // C11: alignof
#include <time.h>
#include <string.h>
#include <pthread.h>
#include <errno.h>
#include <stdarg.h>
#include <assert.h>

static int g_pass = 0, g_fail = 0;

static void check_impl(int cond, const char *file, int line, const char *fmt, ...) {
    va_list ap;
    if (cond) {
        ++g_pass;
        fputs("[PASS] ", stdout);
        va_start(ap, fmt);
        vprintf(fmt, ap);
        va_end(ap);
        putchar('\n');
    } else {
        ++g_fail;
        fputs("[FAIL] ", stdout);
        va_start(ap, fmt);
        vprintf(fmt, ap);
        va_end(ap);
        printf("  (at %s:%d)\n", file, line);
    }
}

/* Note: fmt is inside __VA_ARGS__ now */
#define CHECK(cond, ...) check_impl((cond), __FILE__, __LINE__, __VA_ARGS__)


static inline bool is_aligned_to(void *p, size_t A) {
    return ((uintptr_t)p % A) == 0;
}

static void touch_bytes(void *p, size_t n, unsigned seed) {
    // Write a pattern, then verify
    unsigned char *b = (unsigned char*)p;
    for (size_t i = 0; i < n; ++i) b[i] = (unsigned char)((i * 131u + seed) & 0xFF);
    for (size_t i = 0; i < n; ++i) if (b[i] != (unsigned char)((i * 131u + seed) & 0xFF)) {
        fprintf(stderr, "Memory verification failed at offset %zu\n", i);
        abort();
    }
}

static void test_alignment_and_basic_sizes(void) {
    size_t A = alignof(max_align_t);

    size_t sizes[] = {1, 2, 3, 7, 8, 15, 16, 31, 32, 63, 64, 127, 128, 255, 256, 1024, 4096, 16384};
    for (size_t i = 0; i < sizeof(sizes)/sizeof(sizes[0]); ++i) {
        size_t n = sizes[i];
        void *p = malloc(n);
        CHECK(p != NULL, "malloc(%zu) returned non-NULL", n);
        if (!p) continue;
        CHECK(is_aligned_to(p, A), "pointer alignment for size %zu is multiple of %zu", n, A);
        touch_bytes(p, n, (unsigned)(0xA5A5 + n));
        free(p);
    }
}

static void test_zero_size_policy(void) {
    // Your malloc turns n==0 into 1. Make sure it returns non-NULL and is writable for 1 byte.
    void *p = malloc(0);
    CHECK(p != NULL, "malloc(0) returns non-NULL per your policy");
    if (p) {
        ((unsigned char*)p)[0] = 0x5A;
        CHECK(((unsigned char*)p)[0] == 0x5A, "malloc(0) gave writable byte");
        free(p);
    }
}

static void test_large_alloc(void) {
    size_t n = 8*1024*1024; // 8 MiB
    void *p = malloc(n);
    CHECK(p != NULL, "malloc(8MiB) success");
    if (p) {
        // Touch first/last page
        ((unsigned char*)p)[0] = 0x11;
        ((unsigned char*)p)[n-1] = 0x22;
        CHECK(1, "large block touch OK");
        free(p);
    }
}

// Optional: this test will likely FAIL until you add overflow checks.
// Comment this out if you don't want a failing test yet.
// void test_huge_should_fail_or_detect_overflow(void) {
//     // Try to force failure; if your malloc lacks overflow checks, this may succeed (bad).
//     size_t suspicious = (size_t)-4096; // extremely large, almost SIZE_MAX
//     errno = 0;
//     void *p = malloc(suspicious);
//     if (p == NULL) {
//         CHECK(errno == ENOMEM || errno == 0, "huge malloc returned NULL (errno=%d ok)", errno);
//     } else {
//         // If this passes, your allocator probably overflowed the size arithmetic.
//         CHECK(0, "huge malloc unexpectedly succeeded (likely integer overflow bug)");
//         free(p);
//     }
// }

static void test_many_small_blocks(void) {
    enum { N = 2000 };
    size_t A = alignof(max_align_t);
    void *ptrs[N];

    for (int i = 0; i < N; ++i) {
        size_t n = (size_t)(i % 64 + 1);
        ptrs[i] = malloc(n);
        CHECK(ptrs[i] != NULL, "small alloc #%d size=%zu non-NULL", i, n);
        if (ptrs[i]) {
            CHECK(is_aligned_to(ptrs[i], A), "small alloc #%d alignment ok", i);
            touch_bytes(ptrs[i], n, (unsigned)(0xBEEF + i));
        }
    }
    for (int i = 0; i < N; ++i) {
        free(ptrs[i]);
    }
    CHECK(1, "freed %d small blocks", N);
}

int main(void) {
    printf("Running toy malloc tests…\n");
    srand((unsigned)time(NULL));

    test_alignment_and_basic_sizes();
    test_zero_size_policy();
    test_large_alloc();
    test_many_small_blocks();
    // Enable if you want to expose the missing overflow checks:
    // test_huge_should_fail_or_detect_overflow();

    printf("\nSUMMARY: %d passed, %d failed\n", g_pass, g_fail);
    return (g_fail == 0) ? 0 : 1;
}
