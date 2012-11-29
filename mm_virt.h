
#include "mm_phys.h"

#define USER_SPACE 0
#define KERNEL_SPACE 1

// TODO: Macros for accessing page table entries?

struct memory_space
{
	uint32_t *page_table_1;
	uint32_t *page_table_2[3]; // Alloc four page tables, use one and store the rest
	uint8_t ttb_register; // USER_SPACE = TTBR0 (0), KERNEL_SPACE = TTBR1 (1)
};

typedef struct memory_space memory_space_t;

/*
Allocs a physical page and maps virtual page to it.
	Args: Virtual address of page to map, flags with attributes etc..(what flags?)
	returns: 0 on sucess, -<ERROR> otherwise
*/
int alloc_virtual_page(memory_space_t *mem_space, void* virtual_page_address, uint32_t flags);
void dealloc_virtual_page(memory_space_t *mem_space, void* virtual_page_address);
int iomap_virtual_page(memory_space_t *mem_space, void* virtual_page_address, phys_addr_t physical_page_address);

int init_memory_space(memory_space_t *mem_space, uint8_t reg_space);	// Alloc Lvl 1 Page table
void remove_memory_space(memory_space_t *mem_space);	// Dealloc all alloced physical pages
void switch_user_space(memory_space_t *new_mem_space);

/*
	Private functions:
	Invalidate TLB
	Invalidate Page
*/
