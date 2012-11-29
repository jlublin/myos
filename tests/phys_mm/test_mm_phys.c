
#include <stdio.h>
#include "mm_phys.h"

extern struct phys_ram main_memory;

int main()
{
	init_phys_mm();

	int i;
	for(i = 0; i < 68; i++)
		printf("Allocated: %p\n", alloc_physical_memory());
	
	dealloc_physical_memory(0xa0229000);

	for(i = 0; i < 24; i++)
		printf("Bitmap[%d]: 0x%08x\n", i, main_memory.usage_bitmap[i]);

	return 0;
}

