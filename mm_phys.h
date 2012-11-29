
#define RAM_SIZE 64*1024*1024
#define RAM_BASE 0xA0000000
#define PAGE_SIZE 4096;

typedef unsigned int uint32_t;
typedef void* phys_addr_t;

struct phys_ram
{
	phys_addr_t	base_address;
	uint32_t		ram_size;
	
	uint32_t		*usage_bitmap;
};

void 		init_phys_mm();
phys_addr_t	alloc_physical_page();
phys_addr_t	alloc_physical_sequential_pages(uint32_t num_pages);
void 		dealloc_physical_page(phys_addr_t page_address);


