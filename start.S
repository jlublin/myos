
#define FFUART 0x40100000

.section .init
interrupt_vector_table:
ivt_reset:		b start
ivt_undef:		b .
ivt_svc:		b .
ivt_prefetch:	b .
ivt_data:		b .
ivt_unused:		b .
ivt_irq:		b .
ivt_fiq:		b .

start:
	@ Print "MyOS Loading..."
	ldr r0, =welcomestr
	bl uart_log

	@ Print "Copying kernel to RAM"
	ldr r0, =copykernelstr
	bl uart_log

	@ Copy static kernel to RAM
	mov r0, #0x0
	mov r1, #0xA0000000
	ldr r2, =end_of_static_kernel

copy_kernel_loop:
	ldrb r3, [r0], #1
	strb r3, [r1], #1
	cmp r0, r2
	bne copy_kernel_loop

	@ Print "Initializing MMU"
	ldr r0, =initmmustr
	bl uart_log

	@ Setup domains D1 = Manager, D0 = Client, D2-D15 = No access
	mov r0, #0b1101
	mcr p15, 0, r0, c3, c0, 0
	
	@ Load TTBR0 and TTBR1 with physical address kernel map at beginning of .data
	ldr r0, =bin_data + 0xA0000000

	mcr p15, 0, r0, c2, c0, 0
	mcr p15, 0, r0, c2, c0, 1

	@ Set TTBRC to 2 GB user space/2 GB kernel space
	mov r0, #0b1
	mcr p15, 0, r0, c2, c0, 2

	@ Print "Starting MMU"
	ldr r0, =startingmmustr
	bl uart_log

	@ Start MMU
	mrc p15, 0, r0, c1, c0, 0
	orr r0, #0b1
	mcr p15, 0, r0, c1, c0, 0

	@ Print "MMU Started"
	ldr r0, =startedmmustr
	bl uart_log
	
	@ Print "Kernel space is working", call uart_log+0x8000000 (in kernel space)
	ldr r0, =kernelspaceworkingstr
	ldr r1, =uart_log
	ldr lr, =end_test
	add r1, r1, #0x80000000
	mov pc, r1

end_test:

	@ Clear bss
	@ Copy static kernel to RAM
	mov r0, #0x0
	ldr r1, =.bss
	ldr r2, =end_of_bss
	b clear_bss

clear_bss_loop:
	strb r0, [r1], #1
clear_bss:
	cmp r1, r2
	bne clear_bss_loop

	@ Set up kernel stack pointer
	ldr sp, =stack_top

	@ Print "C setup, jumping to main()"
	ldr r0, =csetupfinishedstr
	bl uart_log

	bl main
	

uart_log:	@ (r0 is null-terminated string)
	ldr r2, =FFUART

uart_log_loop:
	ldrb r1, [r0], #1
	
	cmp r1, #0
	moveq pc, lr
	str r1, [r2]

	b uart_log_loop


.align
.section .init_rodata

welcomestr:	.asciz "MyOS Loading...\n"
copykernelstr: .asciz "Copying kernel to RAM\n"
initmmustr:	.asciz "Initializing MMU\n"
startingmmustr:	.asciz "Starting MMU\n"
startedmmustr:	.asciz "MMU Started\n"
kernelspaceworkingstr:	.asciz "Kernel space is working\n"
csetupfinishedstr:	.asciz "C setup, jumping to main()\n"


@ Hmm, unsure about domain (1<<5), check how permissions work
.section .mmu_kernel_map
mmu_kernel_map:

	.long	0x2 | 1<<5

	.rept 0x400
		.long 0x0
	.endr

@ 0x4010_0000 identity mapped
	.long 0x401 << 20 | 1<<5 | 0x2

	.rept 0x3FE
		.long 0x0
	.endr

@ 0x80000000 User/Kernel split
	.long 0xA00 << 20 | 1<<5 | 0x2

	.rept 0x7FE
		.long 0x0
	.endr

	.long 0xA01 << 20 | 1<<5 | 0x2

