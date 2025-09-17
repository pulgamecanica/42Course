#include "../include/malloc.h" // will change later

#include "alloc_hdr.h"  /* ft_alloc_hdr_t, FT_ALLOC_LARGE */
#include "align.h"      /* FT_MALLOC_ALIGN, FT_ALIGN_UP, align_up_sz */
#include "block.h"      /* ft_block_t, FT_BLOCK_ALIGNED_SIZE */
#include "fit.h"        /* ft_first_fit */
#include "zone.h"       /* ft_zone_t */
#include "utils.h"      /* pagesize_cached */
#include "libft.h"      /* ft_memset */

/* --- helpers to manage the two types of allocations (large & other) ---*/

static void *__large_malloc__(size_t n) {
  const size_t hs = (size_t)FT_ALLOC_HDR_ALIGNED_SIZE;
  size_t need = hs + n;

  /* page-align the mapping length */
  const size_t ps = pagesize_cached();
  size_t mlen = align_up_sz(need, ps);
  if (mlen == 0) FAIL_ENOMEM();

  void *base = MMAP(mlen);
  if (base == MAP_FAILED) FAIL_ENOMEM();

  /* header at mapping base; user at base + hs */
  ft_alloc_hdr_t *h = (ft_alloc_hdr_t *)base;

  /* Zero just the header; no recursion risk */
  ft_memset(h, 0, sizeof(*h));
  h->magic            = FT_MALLOC_MAGIC;
  h->kind             = FT_ALLOC_LARGE;   /* for now everything is LARGE */
  h->user_size        = n;
  h->u.lg.mapped_len  = mlen;

  return (void *)((char *)base + hs);
}

static void __large_malloc_free__(ft_alloc_hdr_t *h) {
  munmap((void *)h, h->u.lg.mapped_len);
}

static void *__managed_malloc__(size_t n) {
  const uint8_t kind = (n <= FT_TINY_MAX) ? FT_ALLOC_TINY : FT_ALLOC_SMALL;
  const size_t  need = FT_ALIGN_UP(n, FT_MALLOC_ALIGN);

  ft_block_t *b = NULL;
  ft_zone_t  *z = ft_zone_find_with_space(kind, need, &b);
  if (!z || !b) FAIL_ENOMEM();
  return ft_zone_alloc(z, b, n, need);
}

static void __managed_malloc_free__(ft_alloc_hdr_t *h) {
  (void)ft_zone_free_from_header(h); /* ignore failure */
}

/* --- public api --- */
void *malloc(size_t n) {
  const size_t hs = (size_t)FT_ALLOC_HDR_ALIGNED_SIZE;

  if (n == 0) return NULL;

  /* hs + n overflow guard */
  if (n > SIZE_MAX - hs) FAIL_ENOMEM();

  if (n > FT_SMALL_MAX) return __large_malloc__(n);
  else                  return __managed_malloc__(n);
}

void free(void *p) {
  const size_t    hs = (size_t)FT_ALLOC_HDR_ALIGNED_SIZE;
  ft_alloc_hdr_t  *h;

  if (!p) return;
  
  h = (ft_alloc_hdr_t *)((char *)p - hs);

  if (h->magic != FT_MALLOC_MAGIC) return; /* bad free / foreign pointer */

  if (h->kind == FT_ALLOC_LARGE)  __large_malloc_free__(h);
  else                            __managed_malloc_free__(h);
}

void show_alloc_mem(void)
{
  static const char msg[] = "show_alloc_mem: not implemented yet\n";
  (void)!write(2, msg, sizeof msg - 1);
}
