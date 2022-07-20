	.file	"hello.c"
	.option nopic
	.attribute arch, "rv64i2p0_m2p0_a2p0_f2p0_d2p0_c2p0"
	.attribute unaligned_access, 0
	.attribute stack_align, 16
	.text
	.align	1
	.globl	putchar
	.type	putchar, @function
putchar:
	addi	sp,sp,-48
	sd	s0,40(sp)
	addi	s0,sp,48
	mv	a5,a0
	sw	a5,-36(s0)
	li	a5,512
	sd	a5,-24(s0)
	lw	a5,-36(s0)
	andi	a4,a5,0xff
	ld	a5,-24(s0)
	sb	a4,0(a5)
	li	a5,0
	mv	a0,a5
	ld	s0,40(sp)
	addi	sp,sp,48
	jr	ra
	.size	putchar, .-putchar
	.align	1
	.globl	main
	.type	main, @function
main:
	addi	sp,sp,-16
	sd	ra,8(sp)
	sd	s0,0(sp)
	addi	s0,sp,16
	li	a0,104
	call	putchar
	li	a0,101
	call	putchar
	li	a0,108
	call	putchar
	li	a0,108
	call	putchar
	li	a0,111
	call	putchar
	li	a0,10
	call	putchar
	li	a5,0
	mv	a0,a5
	ld	ra,8(sp)
	ld	s0,0(sp)
	addi	sp,sp,16
	jr	ra
	.size	main, .-main
	.ident	"GCC: (GNU) 11.1.0"
