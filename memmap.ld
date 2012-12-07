
SECTIONS
{
	. = 0x00000000;
	.init :
	{
		*(.init);
		*(.init_rodata);
	}

	.init_uart_map = 0x40100000;

	. += 0x80000000;

	. = ALIGN(4);
	bin_text = . - 0x80000000;
	size_text = SIZEOF(.text);

	.text : AT (bin_text)
	{
		*(.text);
	}

	. = ALIGN(4);
	bin_rodata = . - 0x80000000;

	.rodata : AT (bin_rodata)
	{
		*(.rodata);
	}

	. = ALIGN(0x4000);

	bin_data = . - 0x80000000;

	.data : AT (bin_data)
	{
		*(.data);
		*(.mmu_kernel_map);
	}
	end_of_static_kernel = . - 0x80000000;

	. = ALIGN(0x1000)
	.mmu_kernel_map_lvl2 = .
	. += 0x200000

	.tmp_mmu_lvl2_entry = .
	. += 0x1000		/* 1 page */

	.bss :
	{
		. = ALIGN(0x1000);
		page_pool = .;
		. += 0x8000; /* 32 k entries */
		*(.bss);
	}
	end_of_bss = .;
	size_bss = SIZEOF(.bss);

	.fiq_barrier = 	ALIGN(0x1000); /* To detect stack overflows */
	.fiq_stack = 	.fiq_barrier + 0x1000;
	.irq_barrier = 	.fiq_stack + 0x1000;
	.irq_stack = 	.irq_barrier + 0x1000;
	.abt_barrier = 	.irq_stack + 0x1000;
	.abt_stack = 	.abt_barrier + 0x1000;
	.mon_barrier = 	.abt_stack + 0x1000;
	.mon_stack = 	.mon_barrier + 0x1000;
	.und_barrier = 	.mon_stack + 0x1000;
	.und_stack	 = 	.und_barrier + 0x1000;
	. = .und_stack + 0x1000;	
	
	phys_mem_data = ALIGN(0x1000);
	. = phys_mem_data + 0x1000;

	.heap = ALIGN(0x1000);

	/* stack barrier ? */

	. = 0xFFFFF000;
	stack_top = .;

	.user_exec : AT (bin_user_exec)	/* kernel code executable by user space, eg. return to userspace */
	{
		*(.user_exec);
	}

}
