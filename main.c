
#define FFUART (void*)0x40100000

void debug_uart(char *string);

void main()
{
	volatile unsigned int *uart = FFUART;

	debug_uart("C kernel is running\n");

	while(1) {}
}

void debug_uart(char *string)
{
	int i = 0;

	volatile unsigned int *uart = FFUART;

	while(string[i])
		*uart = string[i++];
}


