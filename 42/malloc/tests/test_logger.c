#include <stdio.h>
#include <string.h>

#ifdef MALLOC_LOGGING
# include "mem_logger.h"
#endif

#include "malloc.h"

int main(void)
{
	void *tiny1, *tiny2, *tiny3, *tiny4;
	void *small1, *small2, *small3;
	void *large1, *large2, *large3;

	printf("=== Phase 1: Initial TINY allocations ===\n");
	tiny1 = malloc(50);    // TINY
	tiny2 = malloc(100);   // TINY
	tiny3 = malloc(75);    // TINY
	if (tiny1) memset(tiny1, 'A', 50);
	if (tiny2) memset(tiny2, 'B', 100);
	if (tiny3) memset(tiny3, 'C', 75);

	printf("=== Phase 2: SMALL allocations ===\n");
	small1 = malloc(200);  // SMALL
	small2 = malloc(512);  // SMALL
	small3 = malloc(800);  // SMALL
	if (small1) memset(small1, 'D', 200);
	if (small2) memset(small2, 'E', 512);
	if (small3) memset(small3, 'F', 800);

	printf("=== Phase 3: LARGE allocations ===\n");
	large1 = malloc(4096);  // LARGE
	large2 = malloc(8192);  // LARGE
	large3 = malloc(2048);  // LARGE
	if (large1) memset(large1, 'G', 4096);
	if (large2) memset(large2, 'H', 8192);
	if (large3) memset(large3, 'I', 2048);

	printf("=== Phase 4: Free some blocks (fragmentation) ===\n");
	free(tiny2);   // Free middle TINY block
	free(small2);  // Free middle SMALL block
	free(large2);  // Free middle LARGE block

	printf("=== Phase 5: Realloc operations ===\n");
	// In-place realloc (smaller)
	tiny1 = realloc(tiny1, 40);
	printf("  Reallocated tiny1: 50 -> 40 bytes (in-place shrink)\n");

	// Realloc to larger size (might move)
	tiny3 = realloc(tiny3, 120);
	printf("  Reallocated tiny3: 75 -> 120 bytes (might move)\n");

	// Realloc SMALL block to different size
	small1 = realloc(small1, 600);
	printf("  Reallocated small1: 200 -> 600 bytes\n");

	// Realloc to TINY from SMALL (shrink)
	small3 = realloc(small3, 100);
	printf("  Reallocated small3: 800 -> 100 bytes (zone change!)\n");

	// Realloc LARGE block
	large1 = realloc(large1, 6000);
	printf("  Reallocated large1: 4096 -> 6000 bytes\n");

	printf("=== Phase 6: Allocate more blocks ===\n");
	tiny2 = malloc(64);    // Reuse freed TINY slot?
	small2 = malloc(450);  // Reuse freed SMALL slot?
	tiny4 = malloc(128);   // Another TINY
	if (tiny2) memset(tiny2, 'J', 64);
	if (small2) memset(small2, 'K', 450);
	if (tiny4) memset(tiny4, 'L', 128);

	printf("=== Phase 7: More reallocs ===\n");
	// Realloc NULL (acts like malloc)
	large2 = realloc(NULL, 5000);
	printf("  Reallocated NULL -> 5000 bytes (new allocation)\n");

	// Realloc to zero (acts like free)
	tiny4 = realloc(tiny4, 0);
	printf("  Reallocated tiny4 to 0 bytes (free)\n");

	// Complex realloc chain
	small2 = realloc(small2, 300);  // 450 -> 300
	small2 = realloc(small2, 900);  // 300 -> 900
	small2 = realloc(small2, 150);  // 900 -> 150
	printf("  Chained realloc small2: 450->300->900->150\n");

	printf("=== Phase 8: Free everything ===\n");
	free(tiny1);
	free(tiny2);
	free(tiny3);
	// tiny4 already freed by realloc(ptr, 0)
	free(small1);
	free(small2);
	free(small3);
	free(large1);
	free(large2);
	free(large3);

	printf("\n=== Test complete! Check malloc_log.json ===\n");
	printf("Expected snapshots: ~25-30 (depends on implementation)\n");
	return (0);
}
