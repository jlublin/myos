
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
	ldr r3, =UART0_PADDR
	ldr r2, [r3]

	mov r1, #'M'
	str r1, [r2]

	mov r1, #'y'
	str r1, [r2]

	mov r1, #'O'
	str r1, [r2]

	mov r1, #'S'
	str r1, [r2]

	b .

UART0_PADDR:	.4byte 0x10009000
UART0_PADDR1:	.4byte 0x10009000

