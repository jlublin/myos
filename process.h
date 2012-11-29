
//#include "threads.h"
#include "mm_phys.h"

typedef struct thread thread_t;
typedef struct process process_t;

struct thread
{
	process_t *owner;
	uint32_t registers[16];
};

struct process
{
	thread_t *thread;	// Just one thread to begin with
	memory_space_t mem_space;

	uint32_t pid;	// Process ID
	uint32_t uid;	// Owner User ID
};
