#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>

#ifdef FT_MODE
#include "malloc.h"
#else
#include <stdlib.h>
#endif

#define handle_error(msg) \
           do { perror(msg); } while (0)

int main(void) {

	char	*ptr1;
	int		*ptr2;
	bool	*ptr3;

	(void)ptr1; (void)ptr2; (void)ptr3;

	ptr1 = malloc(sizeof(char) * 10);
	if (ptr1 == NULL)
		handle_error("malloc");

	ptr2 = malloc(sizeof(int) * 10);
	if (ptr2 == NULL)
    handle_error("malloc");

	ptr3 = malloc(sizeof(bool) * 10);
	if (ptr3 == NULL)
    handle_error("malloc");

	fprintf(stdout, "Program finished, freeing everything!\n");

	free(ptr1);
	free(ptr2);
	free(ptr3);
	return 0;
}
