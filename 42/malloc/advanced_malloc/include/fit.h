#ifndef FT_FIT_H
#define FT_FIT_H

#include "block.h"
#include <stddef.h>
#include <stdint.h>


/* Return the first free block with capacity >= need.
 * NOTE: 'need' should already be aligned to your FT_MALLOC_ALIGN.
 */
ft_block_t *ft_first_fit(ft_block_t *free_head, size_t need);

// ft_block_t *ft_last_fit(ft_block_t *free_head, size_t need);
// ft_block_t *ft_best_fit(ft_block_t *free_head, size_t need);
// ft_block_t *ft_worst_fit(ft_block_t *free_head, size_t need);
// ft_block_t *ft_next_fit(ft_block_t *free_head, size_t need);
// ft_block_t *ft_next_best_fit(ft_block_t *free_head, size_t need);

#endif /* FT_FIT_H */
