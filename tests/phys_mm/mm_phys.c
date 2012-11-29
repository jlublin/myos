
/*
physical memory manager:

bitmap usage of RAM in 4K pages (no reason to use other sizes)

typedef (void*) phys_addr_t

struct phys_ram
{
	phys_addr_t	base_address;
	uint32_t	ram_size;
	
	uint32_t	*usage_bitmap;	// Or shold this be static 128 kB ?
}

void		init_phys_mm()
phys_addr_t	alloc_physical_memory() // Finds free page, allocs it and returns phys address
void		dealloc_physical_memory(phys_addr_t address)

Memory map:
phys_mem_data = ALIGN(0x1000);
. = phys_mem_data + 0x1000;
end_of_phys = .

*/

#include "mm_phys.h"

struct phys_ram main_memory;
uint32_t phys_mem_data[1024];

/*
	Arguments: none
	Returns: nothing
*/
void init_phys_mm()
{
	main_memory.base_address = (phys_addr_t)0xA0000000;
	main_memory.ram_size = 64*1024*1024; // 64 MB in bytes
	main_memory.usage_bitmap = phys_mem_data;

	/*
	Some pages are already in use, mark them as in use
	First two megabytes -> 512 pages -> 32 bits as 16 uint32_t
	*/

	int i;
	for(i = 0; i < 16; i++)
		main_memory.usage_bitmap[i] = 0xFFFFFFFF;
}

/*
	Arguments: none
	Returns: Physical address to a new allocated page or 0 if all memory is used
*/
phys_addr_t	alloc_physical_memory()
{
	int i, j;
	phys_addr_t address = 0;

	for(i = 0; i < (main_memory.ram_size >> 17); i++)
		if(main_memory.usage_bitmap[i] != 0xFFFFFFFF)
			break;
	
	for(j = 0; j < 32; j++)
		if(((main_memory.usage_bitmap[i] >> j) & 0x1) == 0)
		{
			address = main_memory.base_address
					+ (32*i + j)*4096;
			break;
		}

	main_memory.usage_bitmap[i] |= 1 << j;

	printf("Debug: (%d,%d)\n", i, j);

	return address;
}

/*
	Arguments: Address of page to dealloc
	returns: nothing
*/
void dealloc_physical_memory(phys_addr_t address)
{
	phys_addr_t aligned_address = (uint32_t)(address - main_memory.base_address) & 0xFFFFF000;

	int i = (unsigned int)aligned_address >> 17;
	int j = ((unsigned int)aligned_address & 0x0001F000) >> 12;

	main_memory.usage_bitmap[i] &= ~(1 << j);
}



