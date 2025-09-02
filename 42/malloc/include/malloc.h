#ifndef FT_LIB_H
#define FT_LIB_H

#include "libft.h"    /* for ft_list */
#include <errno.h>    /* for errno global*/
#include <stddef.h>   /* for ft_list */
#include <unistd.h>   /* for sysconf */
#include <pthread.h>  /* for mutex related */
#include <sys/mman.h> /* for mmap and munmap */

#ifndef MALLOC_DEBUG
#define MALLOC_DEBUG 0
// #include <assert.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

/*
  Used to compare chunck sizes.
  This should be at least as wide as size_t and unsigned.
  Chunks are the representations of the memory allocations made by
  mmap.

  Take for example:
  ptr = mmap(NULL, 1024, PROT_WRITE|PROT_READ, MAP_ANON, -1, 0);
  Here we claimed 1024 bytes of memory, and we can map it into chunks
  of smaller sizes, the size of each individual chunk will be
  CHUNK_SIZE_T.

	Ex (one chunk):
  |_____________________________________________________|
  |	CHUNK |                                             |
  |	 ---> | 0000 - 0000 - 0000 - 0000 ... 32 or 64 bits |
  |_____________________________________________________|

	One "Chunk" can be made from multiple "Chunks", we know it's
	always a multiple of CHUNK_SIZE_T.

	Ex ("Chunk" with capacity to hold 5 individual "Chunks"):
	|_____________________________________________________|
  | CHUNK |                                             |
  | 0---> | 0000 - 0000 - 0000 - 0000 ... CHUNK_SIZE_T  |
  | 1---> | 0000 - 0000 - 0000 - 0000 ... CHUNK_SIZE_T  |
  | 2---> | 0000 - 0000 - 0000 - 0000 ... CHUNK_SIZE_T  |
  | 3---> | 0000 - 0000 - 0000 - 0000 ... CHUNK_SIZE_T  |
  | 4---> | 0000 - 0000 - 0000 - 0000 ... CHUNK_SIZE_T  |
  |_____________________________________________________|

 	We always define a header at the beggining of a "Chunk" with information
 	about the allocation.
*/
#define CHUNK_SIZE_T unsigned long

/*
	This type is used to represent addresses when they are treated as
	integers (long). (intptr_t would be enough but not all systems
	have it.
	in 32x_ architectures this would be a 4bytes int
	for 64x_ it would be an 8byte int

	|____________________________x32_bits________________________________|
	| 0              10              20                 30               |
	| 1234   5678   9012   3456   7890   1234   5678   9012 = 32bits     |
	| 0000 - 0000 | 0000 - 0000 | 0000 - 0000 | 0000 - 0000 : 4294967296 |
	| Pointers are represented with 32bits, so we have 2**32 pointers    |
  |____________________________________________________________________|
*/
#define PTR_UINT unsigned long

#define SIZE_SZ sizeof(size_t)

/*
  MALLOC_ALIGNMENT: min alignment for allocated chunks.
*/
#define MALLOC_ALIGNMENT      (2 * sizeof(size_t))
#define MALLOC_ALIGN_MASK     (MALLOC_ALIGNMENT - 1)
#define MALLOC_FAILURE_ACTION errno = ENOMEM;

#define PAGE_SIZE             sysconf(_SC_PAGE_SIZE)

#ifndef NULL
#define NULL (void *)0
#endif

// https://man7.org/linux/man-pages/man3/mallinfo.3.html
typedef struct mallinfo {
  int arena;    /* non-mmapped space allocated from system */
  int ordblks;  /* number of free chunks */
  int smblks;   /* number of fastbin blocks */
  int hblks;    /* number of mmapped regions */
  int hblkhd;   /* space in mmapped regions */
  int usmblks;  /* maximum total allocated space */
  int fsmblks;  /* space available in freed fastbin blocks */
  int uordblks; /* total allocated space */
  int fordblks; /* total free space */
  int keepcost; /* top-most, releasable (via malloc_trim) space */
} t_mallinfo;

typedef struct malloc_chunk {
  size_t      prev_size;   /* Size of previous chunk (if free).  */
  size_t      size;        /* Size in bytes, including overhead. */

  struct malloc_chunk* fd; /* double links -- used only if free. */
  struct malloc_chunk* bk;
} t_malloc_chunk;

typedef struct malloc_state {
	/* The maximum chunk size to be eligible for fastbin */
  // size_t  max_fast;   /* low 2 bits used as flags */
  // /* Fastbins */
  // mfastbinptr      fastbins[NFASTBINS];
  // /* Base of the topmost chunk -- not otherwise kept in a bin */
  // mchunkptr        top;
  // /* The remainder from the most recent split of a small request */
  // mchunkptr        last_remainder;
  // /* Normal bins packed as described above */
  // mchunkptr        bins[NBINS * 2];
  // /* Bitmap of bins. Trailing zero map handles cases of largest binned size */
  // unsigned int     binmap[BINMAPSIZE+1];

  /* Tunable parameters */
  // CHUNK_SIZE_T     trim_threshold;
  // size_t  top_pad;
  // size_t  mmap_threshold;

  /* Memory map support */
  // int              n_mmaps;
  // int              n_mmaps_max;
  // int              max_n_mmaps;

  /* Cache malloc_getpagesize */
  unsigned int     pagesize;

  /* Track properties of MORECORE */
  // unsigned int     morecore_properties;

  /* Statistics */
  // size_t  mmapped_mem;
  // size_t  sbrked_mem; // NON util
  // size_t  max_sbrked_mem; // NON util
  // size_t  max_mmapped_mem;
  // size_t  max_total_mem;
} t_malloc_state;

typedef t_malloc_chunk* t_mchunkptr;

typedef struct header {
    size_t mapped;   // total bytes mapped (incl. header, rounded to page size)
    size_t user;     // requested size (optional but handy for debugging)
} header_t;

void free(void *ptr);
void *malloc(size_t size);
void *realloc(void *ptr, size_t size);
int mallopt(int num, int val);
t_mallinfo mallinfo(void);
void show_alloc_mem(void);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif
