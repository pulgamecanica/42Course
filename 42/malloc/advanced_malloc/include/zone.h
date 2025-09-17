#ifndef FT_ZONE_H
#define FT_ZONE_H

#include <stddef.h>
#include <stdint.h>
#include "libft.h"      /* t_list */
#include "block.h"      /* ft_block_t, FT_BLOCK_ALIGNED_SIZE */
#include "alloc_hdr.h"  /* ft_alloc_hdr_t, FT_ALLOC_HDR_ALIGNED_SIZE */

/* Config: pages per zone (tweak later) */
#ifndef FT_TINY_ZONE_PAGES
#  define FT_TINY_ZONE_PAGES   16u
#endif
#ifndef FT_SMALL_ZONE_PAGES
#  define FT_SMALL_ZONE_PAGES  64u
#endif

/* Per-allocation overhead inside a zone */
enum { FT_HEAD_ROOM = FT_BLOCK_ALIGNED_SIZE + FT_ALLOC_HDR_ALIGNED_SIZE };

/* Zone object (lives at the start of an mmap region). */
typedef struct ft_zone {
  t_list          link;         /* embedded list node; link.content == this zone */
  size_t          mapped_len;
  void           *usable_base;  /* A-aligned start of first block */
  size_t          usable_len;   /* bytes after usable_base */
  ft_block_t     *free_head;    /* free-list head for THIS zone */
  uint8_t         kind;         /* FT_ALLOC_TINY / FT_ALLOC_SMALL */
} ft_zone_t;

enum { FT_ZONE_ALIGNED_SIZE = FT_ALIGN_UP(sizeof(ft_zone_t), FT_MALLOC_ALIGN) };

#if __STDC_VERSION__ >= 201112L
_Static_assert((FT_ZONE_ALIGNED_SIZE % FT_MALLOC_ALIGN) == 0, "header not padded to A");
#endif

/* One global manager holding both class lists */
typedef struct ft_zone_mgr {
  t_list *tiny_zones;           /* list of ft_zone_t* via node->content */
  t_list *small_zones;
} ft_zone_mgr_t;

/* API the allocator uses (see zone.c for more info) */
ft_zone_t *ft_zone_find_with_space(uint8_t kind, size_t need_payload, ft_block_t **out_block);
void      *ft_zone_alloc(ft_zone_t *z, ft_block_t *b, size_t user_req, size_t need_payload);
int        ft_zone_free_from_header(ft_alloc_hdr_t *h);

/***************************************************************************************************/
/* Cycle: Find a zone with space  -> allocate in the zone you founded -> free the zone you founded */
/*                                                                                                 */
/*        ft_zone_find_with_space -> ft_zone_alloc                    -> ft_zone_free_from_header  */
/***************************************************************************************************/

#endif /* FT_ZONE_H */
