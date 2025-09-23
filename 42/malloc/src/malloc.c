#include "malloc.h"
#include <valgrind/valgrind.h>
#include <valgrind/memcheck.h>

#ifdef __cplusplus
extern "C" {
#endif

/*
	Safeguard when using threads.
	This are wrappers used in all the public functions.
*/
#ifdef USE_MALLOC_LOCK
#define PRE_MALLOC   pthread_mutex_lock(&g_malloc_mutex)
#define POST_MALLOC  pthread_mutex_unlock(&g_malloc_mutex)
#else
#define PRE_MALLOC (1)
#define POST_MALLOC (1)
#endif

/*
	Taken from Doug Leah's implementation, very useful shortcut.
*/
#define MMAP(addr, size, prot, flags) \
 (mmap((addr), (size), (prot), (flags)|MAP_ANONYMOUS, -1, 0))


/*
	Util MACROS to deal with chunk header offsets
*/
#define chunk2mem(p)   ((void *)((char*)(p) + 2*SIZE_SZ))
#define mem2chunk(mem) ((t_mchunkptr)((char*)(mem) - 2*SIZE_SZ))
#define MIN_CHUNK_SIZE (sizeof(t_malloc_chunk))
#define MINSIZE  \
  (CHUNK_SIZE_T)(((MIN_CHUNK_SIZE+MALLOC_ALIGN_MASK) & ~MALLOC_ALIGN_MASK))
#define aligned_OK(m)  (((PTR_UINT)((m)) & (MALLOC_ALIGN_MASK)) == 0)

// static t_malloc_state av_;  /* never directly referenced */

// #define get_malloc_state() (&(av_))

// 0x00000ff0123fee
static inline size_t round_up(size_t x, size_t a) {
    return (x + a - 1) & ~(a - 1);
}

static inline void *error() {
	// MALLOC_FAILURE_ACTION;
	return NULL;
}

// static void malloc_init_state(t_malloc_state *av) {
// 	// int     i;
//   // mbinptr bin;

//   /* Establish circular links for normal bins */
//   // for (i = 1; i < NBINS; ++i) {
//   //   bin = bin_at(av,i);
//   //   bin->fd = bin->bk = bin;
//   // }

// 	// av->top_pad        = DEFAULT_TOP_PAD;
//   // av->n_mmaps_max    = DEFAULT_MMAP_MAX;
//   // av->mmap_threshold = DEFAULT_MMAP_THRESHOLD;
//   // av->trim_threshold = DEFAULT_TRIM_THRESHOLD;

//   // av->top            = initial_top(av);//depending on the bin?
//   av->pagesize       = PAGE_SIZE;
// }

/*
	PTHREAD_MUTEX_INITIALIZER vs pthread_mutex_init.
	https://pages.cs.wisc.edu/~remzi/OSTEP/threads-api.pdf ; page6.
*/
// static pthread_mutex_t g_malloc_mutex = PTHREAD_MUTEX_INITIALIZER;

#include <stdio.h>

void *malloc(size_t n) {
	int r = write(1, "Called malloc\n", 15);
	(void)r;
	if (PRE_MALLOC) return error();
	
	size_t need  = sizeof(header_t) + n;
	size_t mlen  = round_up(need, (size_t)PAGE_SIZE);

	void *base = MMAP(NULL, mlen, PROT_READ | PROT_WRITE,
										MAP_PRIVATE | MAP_ANONYMOUS);
	if (base == MAP_FAILED) return error();

	header_t *h = (header_t *)base;
	h->mapped = mlen;
	h->user   = n;

	void *user = (void *)((char *)base + sizeof(header_t));


	VALGRIND_MALLOCLIKE_BLOCK(user, n, 0, 0);

	if (!POST_MALLOC) return error();

	// small:
	// return chunk2mem(victim);
	// medium:
	// return chunk2mem(victim);
	// big:
	// return chunk2mem(victim);
	return user;
}

void free(void *ptr) {
	if (!PRE_MALLOC || !ptr) return;

	// Mark as freed to Memcheck before unmapping (order doesn’t really matter here)
	VALGRIND_FREELIKE_BLOCK(ptr, /*redzone*/0);

	header_t *h = (header_t *)((char *)ptr - sizeof(header_t));
	VALGRIND_MAKE_MEM_NOACCESS(ptr, h->user);

	// Actually release memory back to the OS
	munmap((void *)h, h->mapped);
	puts("HEY, im freeee!\n");
	if (!POST_MALLOC) return;
}

void *realloc(void *ptr, size_t size) {
	(void)size;
	if (!PRE_MALLOC || !ptr) return NULL;
	// TODO
	if (!POST_MALLOC) {
	}
	return ptr;
}

int mallopt(int num, int val) {
	int res;

	res = 0;
	(void)num;
	(void)val;
	if (!PRE_MALLOC) return 0;
	// TODO
	if (!POST_MALLOC) {
	}
	return res;
}

// t_mallinfo mallinfo(void) {
// 	t_mallinfo m;

// 	if (!PRE_MALLOC || !ptr) {
// 		return (t_mallinfo){ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
// 	}
// 	// TODO
// 	if (!POST_MALLOC) {
// 	}
// 	return m;
// }

void show_alloc_mem(void) {
	if (!PRE_MALLOC) return;
	// TODO
	if (!POST_MALLOC) {
	}
}

#ifdef __cplusplus
} /* extern "C" */
#endif
