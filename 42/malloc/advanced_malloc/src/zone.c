#include "../include/malloc.h"
#include "zone.h"

#include "align.h"     /* FT_ALIGN_UP, FT_MALLOC_ALIGN */
#include "fit.h"       /* ft_first_fit */
#include "utils.h"     /* pagesize_cached */
#include <string.h>    /* memset */
#include <sys/mman.h>  /* mmap/munmap */

#include "libft.h"

ft_zone_mgr_t g_zone_mgr = {0};

/* Tiny helpers so callers don’t touch t_list directly */
static inline t_list **ft_zone_list_head(uint8_t kind) {
  return (kind == FT_ALLOC_TINY) ? &g_zone_mgr.tiny_zones : &g_zone_mgr.small_zones;
}
static inline ft_zone_t *ft_zone_first(uint8_t kind) {
  t_list *h = (kind == FT_ALLOC_TINY) ? g_zone_mgr.tiny_zones : g_zone_mgr.small_zones;
  return h ? (ft_zone_t*)h->content : NULL;
}
static inline ft_zone_t *ft_zone_next(const ft_zone_t *z) {
  return (z && z->link.next) ? (ft_zone_t*)z->link.next->content : NULL;
}

/* ---- free-list helpers (double ended queue) ---- */
static inline void flist_push_front(ft_zone_t *z, ft_block_t *b) {
  b->is_free   = 1;
  b->prev_free = NULL;
  b->next_free = z->free_head;
  if (z->free_head) z->free_head->prev_free = b;
  z->free_head = b;
}
static inline void flist_remove(ft_zone_t *z, ft_block_t *b) {
  (void)z;
  if (b->prev_free) b->prev_free->next_free = b->next_free;
  else              z->free_head = b->next_free;
  if (b->next_free) b->next_free->prev_free = b->prev_free;
  b->prev_free = b->next_free = NULL;
  b->is_free   = 0;
}

/* ---- create a new zone and link it into the manager ---- */
static ft_zone_t *zone_new(uint8_t kind) {
  const size_t zs = (size_t)FT_ZONE_ALIGNED_SIZE;
  const size_t ps   = pagesize_cached();
  const size_t zpg  = (kind == FT_ALLOC_TINY) ? (size_t)FT_TINY_ZONE_PAGES
                                              : (size_t)FT_SMALL_ZONE_PAGES;
  const size_t mlen = zpg * ps;

  void *base = MMAP(mlen);
  if (base == MAP_FAILED) return NULL;

  ft_zone_t *z = (ft_zone_t *)base;
  memset(z, 0, sizeof(*z));
  z->mapped_len = mlen;
  z->kind       = kind;

  /* Align usable region after the zone header */
  char  *after_hdr = (char *)base + zs;
  size_t usable    = (size_t)((char *)base + mlen - after_hdr);

  if (usable < (size_t)FT_HEAD_ROOM) {
    munmap(base, mlen); /* Check the page parameters, this should never happen... */
    return NULL;
  }

  z->usable_base = after_hdr;
  z->usable_len  = usable;
  z->free_head   = NULL;

  /* embedded list node points back to this zone */
  z->link.content = z;
  z->link.next    = NULL;

  /* initial big free block, if it can host at least 100 allocations */
  ft_block_t *b = (ft_block_t *)after_hdr;
  memset(b, 0, sizeof(*b));
  size_t cap = usable - (size_t)FT_HEAD_ROOM;   /* payload capacity */
  b->size = FT_ALIGN_UP(cap, FT_MALLOC_ALIGN);  /* keep payload multiple of A */
  flist_push_front(z, b);

  /* attach to manager list (push-front) */
  ft_lstadd_front(ft_zone_list_head(kind), &z->link);
  return z;
}

/* ---- search zones of a class for a first-fit block ---- */
ft_zone_t *ft_zone_find_with_space(uint8_t kind, size_t need_payload, ft_block_t **out_block) {
  for (t_list *n = *ft_zone_list_head(kind); n; n = n->next) {
    ft_zone_t *z = (ft_zone_t *)n->content;
    ft_block_t *b = ft_first_fit(z->free_head, need_payload); // later use V-table
    if (b) { *out_block = b; return z; }
  }
  /* none fit; create one zone and retry once (not recursive) */
  ft_zone_t *znew = zone_new(kind);
  if (!znew) return NULL;
  ft_block_t *b = ft_first_fit(znew->free_head, need_payload);
  if (!b) return NULL; /* extremely unlikely with a fresh zone */
  *out_block = b; return znew;
}

/* params:
  mantion that need_payload is an aligned size
  user req can be anything
  z and b are gotten from ft_zone_find_with_space */
/* ---- allocate inside a zone (split if remainder is big enough) ---- */
void *ft_zone_alloc(ft_zone_t *z, ft_block_t *b, size_t user_req, size_t need_payload) {
  flist_remove(z, b);

  size_t orig_payload = b->size;
  size_t leftover     = (orig_payload >= need_payload) ? (orig_payload - need_payload) : 0;

  const size_t min_payload = (size_t)FT_MALLOC_ALIGN;
  /* Setup the free list with the reminder after the requested payload*/
  if (leftover >= (size_t)FT_HEAD_ROOM + min_payload) {
    char *alloc_region = (char *)b; /* to advance the pointer per byte(s) */
    size_t alloc_bytes = (size_t)FT_HEAD_ROOM + need_payload;

    /* No need to align the `rem` pointer since need_payload is already aligned */
    ft_block_t *rem = (ft_block_t *)(alloc_region + alloc_bytes); /* advancing the pointer */
    memset(rem, 0, sizeof(*rem));
    rem->size = leftover - (size_t)FT_HEAD_ROOM;
    flist_push_front(z, rem);
  }

  /* set allocated block's payload to what we actually used */
  b->size = need_payload;

  /* write alloc header and return user pointer */
  ft_alloc_hdr_t *h = (ft_alloc_hdr_t *)((char *)b + (ptrdiff_t)FT_BLOCK_ALIGNED_SIZE);
  memset(h, 0, sizeof(*h));
  h->magic     = FT_MALLOC_MAGIC;
  h->kind      = z->kind;           /* FT_ALLOC_TINY / FT_ALLOC_SMALL */
  h->user_size = user_req;
  h->u.z.zone  = z;                 /* set zone for O(1) free */
  return (void *)((char *)h + (ptrdiff_t)FT_ALLOC_HDR_ALIGNED_SIZE);
}

/* ---- free: find owning zone by range and push block back to its free list ---- */
int ft_zone_free_from_header(ft_alloc_hdr_t *h) {
  if (h->kind != FT_ALLOC_TINY && h->kind != FT_ALLOC_SMALL) return 0;
  
  ft_zone_t *z = (ft_zone_t *)h->u.z.zone;
  ft_block_t *b = (ft_block_t *)((char *)h - (ptrdiff_t)FT_BLOCK_ALIGNED_SIZE);
  flist_push_front(z, b);

  return 1;
}
