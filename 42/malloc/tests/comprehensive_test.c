/* ************************************************************************** */
/*                                                                            */
/*   comprehensive_test.c                                                     */
/*   Comprehensive test for ft_malloc                                         */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"
#include <string.h>
#include <unistd.h>

static void	ft_write(const char *s, size_t len)
{
	ssize_t	ret;

	ret = write(1, s, len);
	(void)ret;
}

#define WRITE(s) ft_write(s, sizeof(s) - 1)

void	test_tiny_allocations(void)
{
	void	*ptrs[10];
	int		i;

	WRITE("\n=== Testing TINY allocations (1-128 bytes) ===\n");
	for (i = 0; i < 10; i++)
	{
		ptrs[i] = malloc(50);
		if (!ptrs[i])
		{
			WRITE("ERROR: malloc failed\n");
			return ;
		}
		memset(ptrs[i], 'A' + i, 50);
	}

	WRITE("Allocated 10 blocks of 50 bytes each\n");
	show_alloc_mem();

	WRITE("\nFreeing every other block...\n");
	for (i = 0; i < 10; i += 2)
		free(ptrs[i]);

	show_alloc_mem();

	WRITE("\nFreeing remaining blocks...\n");
	for (i = 1; i < 10; i += 2)
		free(ptrs[i]);

	show_alloc_mem();
}

void	test_small_allocations(void)
{
	void	*ptrs[5];
	int		i;

	WRITE("\n=== Testing SMALL allocations (129-1024 bytes) ===\n");
	for (i = 0; i < 5; i++)
	{
		ptrs[i] = malloc(500);
		if (!ptrs[i])
		{
			WRITE("ERROR: malloc failed\n");
			return ;
		}
	}

	WRITE("Allocated 5 blocks of 500 bytes each\n");
	show_alloc_mem();

	WRITE("\nFreeing all blocks...\n");
	for (i = 0; i < 5; i++)
		free(ptrs[i]);

	show_alloc_mem();
}

void	test_large_allocations(void)
{
	void	*ptrs[3];
	int		i;

	WRITE("\n=== Testing LARGE allocations (1025+ bytes) ===\n");
	for (i = 0; i < 3; i++)
	{
		ptrs[i] = malloc(5000);
		if (!ptrs[i])
		{
			WRITE("ERROR: malloc failed\n");
			return ;
		}
	}

	WRITE("Allocated 3 blocks of 5000 bytes each\n");
	show_alloc_mem();

	WRITE("\nFreeing all blocks...\n");
	for (i = 0; i < 3; i++)
		free(ptrs[i]);

	show_alloc_mem();
}

void	test_realloc(void)
{
	void	*ptr;

	WRITE("\n=== Testing realloc ===\n");

	ptr = malloc(50);
	WRITE("Allocated 50 bytes\n");
	memset(ptr, 'X', 50);
	show_alloc_mem();

	WRITE("\nReallocating to 100 bytes...\n");
	ptr = realloc(ptr, 100);
	if (!ptr)
	{
		WRITE("ERROR: realloc failed\n");
		return ;
	}
	show_alloc_mem();

	WRITE("\nReallocating to 2000 bytes (should change zone type)...\n");
	ptr = realloc(ptr, 2000);
	if (!ptr)
	{
		WRITE("ERROR: realloc failed\n");
		return ;
	}
	show_alloc_mem();

	WRITE("\nFreeing...\n");
	free(ptr);
	show_alloc_mem();
}

void	test_edge_cases(void)
{
	void	*ptr;

	WRITE("\n=== Testing edge cases ===\n");

	WRITE("malloc(0) should return NULL: ");
	ptr = malloc(0);
	if (ptr == NULL)
		WRITE("PASS\n");
	else
		WRITE("FAIL\n");

	WRITE("free(NULL) should not crash: ");
	free(NULL);
	WRITE("PASS\n");

	WRITE("realloc(NULL, 100) should work like malloc: ");
	ptr = realloc(NULL, 100);
	if (ptr != NULL)
	{
		WRITE("PASS\n");
		free(ptr);
	}
	else
		WRITE("FAIL\n");
}

int	main(void)
{
	WRITE("====================================\n");
	WRITE("  FT_MALLOC COMPREHENSIVE TEST\n");
	WRITE("====================================\n");

	test_tiny_allocations();
	test_small_allocations();
	test_large_allocations();
	test_realloc();
	test_edge_cases();

	WRITE("\n====================================\n");
	WRITE("  ALL TESTS COMPLETED\n");
	WRITE("====================================\n");

	return (0);
}

