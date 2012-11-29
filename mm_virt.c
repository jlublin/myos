#include "mm_virt.h"

/*

Using backwards-compatible (not ARMv6)
no APX -> 0 is used

** Level 1, 1 MB section
31-20 - Section base address
19 - NS (TrustZone stuff, ignored)
18 - 0	(0 = 1 MB, 1 = 16 MB)
17-15 - 0
14-12 - TEX use with C/B
11-10 - AP Acces permission (kernel/user), 0b00 No Access, 0b01 (RW/NA), 0b10 (RW/RO), 0b11(RW/RW)
9 - P (ECC enabled for region)
8-5 - Domain
4 - 0
3 - C Use with TEX
2 - B Use with TEX
1 - 1
0 - 0

** Level 1, use 2nd page table
31-10 - Coarse page table base address
9 - P (ECC enable for region)
8-5 - Domain
4 - SBZ
3 - NS (TrustZone stuff, ignored)
2 - SBZ
1 - 0
0 - 1

** Level 2, 4 kB
31-12 Small page base address
11-10 AP3 Acces permission (kernel/user), 0b00 No Access, 0b01 (RW/NA), 0b10 (RW/RO), 0b11(RW/RW)
9-8 AP2 = AP3
7-6 AP1 = AP3
5-4 AP0 = AP3
3 - C
2 - B
1 - 1
0 - 0

Use a pool of 32(64?)k pages. Bitmap them.

*/

/*
	Flags could give info about: domain, kernel/user space (TTBR0/TTBR1), executable
*/
int alloc_virtual_page(mem_struct_t *mem, uint32_t virtual_page_address, uint32_t flags)
{	// TODO
	uint32_t vpage_address = virtual_page_address & 0xFFFFF000;

	// Check that virtual memory isn't already used and create level 2 table if
	// it doesn't exist for the page
	uint32_t virt_page1 = mem->page_table[(virtual_page_address & 0xFFF00000) >> 20]
	if

	// Alloc physcal page
	phys_addr_t phys_page = alloc_physical_page();

	// Map virtual page to physical page
}

void dealloc_virtual_page(mem_struct_t *mem, void* virtual_page_address)
{	// TODO
	// 
}

int iomap_virtual_page(mem_struct_t *mem, void* virtual_page_address, phys_addr_t physical_page_address)
{	// TODO
	return -1;
}


int init_user_memory_space(memory_space_t *mem_space)
{	// TODO
	// 4 Page table that maps first 4 MiB which includes the page tables
	phys_addr_t first_level2_page_tables;

	mem_space->ttb_register = USER_SPACE;
	mem_space->page_table_2[0] = 0;
	mem_space->page_table_2[1] = 0;
	mem_space->page_table_2[2] = 0;

	// Assume user space use 8 kB level 1 page tables

	if((mem_space->page_table_1 = alloc_physical_sequential_pages(2)) == NULL)
		return -1; // Error: out of memory

	if((first_level2_page_tables = alloc_physical_page()) == NULL)
	{
		dealloc_physical_page(mem_space->page_table_1);
		dealloc_physical_page(mem_space->page_table_1 + PAGE_SIZE);
		return -1; // Error: out of memory
	}

	// Map page_table_1 page 2 to temporary page (where is it located? kernel space somewhere?)
	// memset(tmp_page_virt_addr, 0, PAGE_SIZE)

	// Map page_table_1 page 1 to temporary page (where is it located? kernel space somewhere?)
	// memset(tmp_page_virt_addr, 0, PAGE_SIZE)

	// Set up first 4 MiB to first_level2_page_tables

	// Map first_level2_page_tables to temporary page (where is it located? kernel space somewhere?)
	// memset(tmp_page_virt_addr, 0, PAGE_SIZE)

	// Set up first 4 MiB in level 2 page tables

	return 0;
}

void remove_memory_space(memory_space_t *mem_space)
{	// TODO
	// Iterate through whole page table to dealloc all physical pages used
	// This might need a lock on physical memory manager

	uint32_t i, j;

	// Skip first 4 because they are special
	for(i = 4; i < NUM_PAGE_TABLE1_ENTRIES; i++)
	{
		for(j = 0; j < NUM_PAGE_TABLE2_ENTRIES; j++)
		{
			if(IS_PHYS_MEM(PAGE_TABLE_ENTRY(mem_space->page_table_1, i, j))
				// TODO Possible race condition or worse? Can the same physical
				//		page be mapped several times (or shared between processes?)
				dealloc_physical_page(PAGE_TABLE_ENTRY(mem_space->page_table_1, i, j));
		}
		
		// TODO Possible race condition, multiple deallocs of physical pages
		// Fix: Zero entries and remove when all four are zero
		dealloc_physical_page(PAGE_TABLE_2(mem_space->page_table_1, i));
	}

	// Dealloc level 1 page table
}

void switch_user_space(memory_space_t *new_mem_space)
{	// TODO
	// Update TTBR0 register
	// Invalidate TLB cache
}


