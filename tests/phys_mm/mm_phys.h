
typedef unsigned int uint32_t;
typedef void* phys_addr_t;

struct phys_ram
{
	phys_addr_t	base_address;
	uint32_t		ram_size;
	
	uint32_t		*usage_bitmap;	// Or shold this be static 128 kB ?
};

void 		init_phys_mm();
phys_addr_t	alloc_physical_memory();
void 		dealloc_physical_memory(phys_addr_t address);


