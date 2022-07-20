	.file	"hello.c"
	.option nopic
	.attribute arch, "rv64i2p0_m2p0_a2p0_f2p0_d2p0"
	.attribute unaligned_access, 0
	.attribute stack_align, 16
	.text
	.align	2
	.globl	main
	.type	main, @function
main:
	li	a5,512
	nop
	nop
	nop
	li	a4,104
	nop
	nop
	nop
	sb	a4,0(a5)
	nop
	nop
	nop
	li	a4,101
	nop
	nop
	nop
	sb	a4,0(a5)
	nop
	nop
	nop
	li	a4,108
	nop
	nop
	nop
	sb	a4,0(a5)
	nop
	nop
	sb	a4,0(a5)
	nop
	nop
	nop
	li	a4,111
	nop
	nop
	nop
	sb	a4,0(a5)
	nop
	nop
	nop
	li	a4,10
	nop
	nop
	nop
	sb	a4,0(a5)
	nop
	nop
	nop
	li	a0,0
	nop
	nop
	nop
	ret
	nop
	nop
	nop
	.size	main, .-main
	.ident	"GCC: (GNU) 11.1.0"
