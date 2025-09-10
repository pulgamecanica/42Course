#ifndef FT_LIB_H
#define FT_LIB_H

#include "libft.h"    /* for ft_list */
#include <errno.h>    /* for errno global*/
#include <stddef.h>   /* for ft_list */
#include <unistd.h>   /* for sysconf */
#include <pthread.h>  /* for mutex related */
#include <sys/mman.h> /* for mmap and munmap */

#ifdef __cplusplus
extern "C" {
#endif

typedef struct header {
    size_t mapped_len;
    size_t header_pad;   // padded header size
    size_t user;         // requested bytes (optional)
} header_t;

void free(void *ptr);
void *malloc(size_t size);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif
