#include <stdint.h>
#include "mm_virt.h"

#define FFUART (void*)0x40100000

void debug_uart(char *string);

void main()
{
	volatile unsigned int *uart = FFUART;

	debug_uart("C kernel is running\n");

	memory_space_t kernel_space;

	init_kernel_memory_space(&kernel_space);

	debug_uart("Kernel is still running, nothing bad happened?\n");

	while(1) {}
}

void debug_uart(char *string)
{
	int i = 0;

	volatile unsigned int *uart = FFUART;

	while(string[i])
		*uart = string[i++];
}


