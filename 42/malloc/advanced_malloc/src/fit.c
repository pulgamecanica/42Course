#include "fit.h"

/* first-fit over the free list */
ft_block_t *ft_first_fit(ft_block_t *free_head, size_t need_payload) {
  for (ft_block_t *b = free_head; b; b = b->next_free)
    if (b->is_free && b->size >= need_payload) return b;
  return NULL;
}
