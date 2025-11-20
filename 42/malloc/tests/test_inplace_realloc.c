/* ************************************************************************** */
/*                                                                            */
/*   test_inplace_realloc.c                                                   */
/*   Test for in-place realloc optimization                                   */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"
#include <string.h>
#include <unistd.h>
#include <stdio.h>

static void	ft_write(const char *s, size_t len)
{
	ssize_t	ret;

	ret = write(1, s, len);
	(void)ret;
}

#define WRITE(s) ft_write(s, sizeof(s) - 1)

void	test_inplace_extension(void)
{
	void	*ptr1;
	void	*ptr2;
	void	*original_addr;
	void	*new_ptr;

	WRITE("\n=== Test 1: In-place extension (should work) ===\n");

	ptr1 = malloc(50);
	printf("Allocated ptr1 (50 bytes) at: %p\n", ptr1);
	memset(ptr1, 'A', 50);

	ptr2 = malloc(50);
	printf("Allocated ptr2 (50 bytes) at: %p\n", ptr2);
	memset(ptr2, 'B', 50);

	WRITE("\nMemory state:\n");
	show_alloc_mem();

	WRITE("\nFreeing ptr2 (creates free block after ptr1)...\n");
	free(ptr2);
	show_alloc_mem();

	original_addr = ptr1;
	printf("Original address: %p\n", original_addr);
	WRITE("\nReallocating ptr1 from 50 to 80 bytes...\n");
	new_ptr = realloc(ptr1, 80);

	printf("New address:      %p\n", new_ptr);

	if (new_ptr == original_addr)
	{
		WRITE("SUCCESS: Extended in place (same address)!\n");

		// Verify data wasn't corrupted
		int i;
		char *data = (char *)new_ptr;
		for (i = 0; i < 50; i++)
		{
			if (data[i] != 'A')
			{
				WRITE("ERROR: Data corrupted!\n");
				return;
			}
		}
		WRITE("Data integrity verified!\n");
	}
	else
	{
		WRITE("FAIL: Allocated new block (different address)\n");
	}

	WRITE("\nFinal memory state:\n");
	show_alloc_mem();

	free(new_ptr);
}

void	test_cannot_extend(void)
{
	void	*ptr1;
	void	*ptr2;
	void	*original_addr;
	void	*new_ptr;

	WRITE("\n=== Test 2: Cannot extend (next block allocated) ===\n");

	ptr1 = malloc(50);
	printf("Allocated ptr1 (50 bytes) at: %p\n", ptr1);
	memset(ptr1, 'A', 50);

	ptr2 = malloc(50);
	printf("Allocated ptr2 (50 bytes) at: %p\n", ptr2);
	memset(ptr2, 'B', 50);

	WRITE("\nMemory state (both allocated):\n");
	show_alloc_mem();

	original_addr = ptr1;
	printf("Original address: %p\n", original_addr);
	WRITE("\nReallocating ptr1 from 50 to 80 bytes (ptr2 still allocated)...\n");
	new_ptr = realloc(ptr1, 80);

	printf("New address:      %p\n", new_ptr);

	if (new_ptr != original_addr)
	{
		WRITE("SUCCESS: Allocated new block (as expected)\n");

		// Verify data was copied correctly
		int i;
		char *data = (char *)new_ptr;
		for (i = 0; i < 50; i++)
		{
			if (data[i] != 'A')
			{
				WRITE("ERROR: Data not copied correctly!\n");
				return;
			}
		}
		WRITE("Data copied correctly!\n");
	}
	else
	{
		WRITE("UNEXPECTED: Extended in place (shouldn't happen)\n");
	}

	WRITE("\nFinal memory state:\n");
	show_alloc_mem();

	free(new_ptr);
	free(ptr2);
}

void	test_extend_too_large(void)
{
	void	*ptr1;
	void	*ptr2;
	void	*original_addr;
	void	*new_ptr;

	WRITE("\n=== Test 3: Next block free but too small ===\n");

	ptr1 = malloc(50);
	printf("Allocated ptr1 (50 bytes) at: %p\n", ptr1);

	ptr2 = malloc(30);
	printf("Allocated ptr2 (30 bytes) at: %p\n", ptr2);

	free(ptr2);
	WRITE("Freed ptr2 (leaves only 30 bytes free)\n");

	original_addr = ptr1;
	printf("Original address: %p\n", original_addr);
	WRITE("\nReallocating ptr1 from 50 to 150 bytes (need 100 more, have 30)...\n");
	new_ptr = realloc(ptr1, 150);

	printf("New address:      %p\n", new_ptr);

	if (new_ptr != original_addr)
		WRITE("SUCCESS: Allocated new block (free block too small)\n");
	else
		WRITE("UNEXPECTED: Extended in place\n");

	free(new_ptr);
}

int	main(void)
{
	WRITE("====================================\n");
	WRITE("  IN-PLACE REALLOC OPTIMIZATION TEST\n");
	WRITE("====================================\n");

	test_inplace_extension();
	test_cannot_extend();
	test_extend_too_large();

	WRITE("\n====================================\n");
	WRITE("  ALL TESTS COMPLETED\n");
	WRITE("====================================\n");

	return (0);
}

