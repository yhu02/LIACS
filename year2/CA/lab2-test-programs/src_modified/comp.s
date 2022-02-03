	.file	"comp.c"
	.option nopic
	.text
	.section	.text.startup,"ax",@progbits
	.align	2
	.globl	main
	.type	main, @function
main:
	nop
	nop
	nop
	lui	a3,%hi(B)
	nop
	nop
	nop
	addi	sp,sp,-16
	nop
	nop
	nop
	addi	a4,a3,%lo(B)
	nop
	nop
	nop
	sd	ra,8(sp)
	nop
	nop
	nop
	addi	a3,a3,%lo(B)
	nop
	nop
	nop
	li	a5,0
	nop
	nop
	nop
	li	a2,20
	nop
	nop
	nop
.L2:
	nop
	nop
	nop
	sw	a5,0(a3)
	nop
	nop
	nop
	addiw	a5,a5,1
	nop
	nop
	nop
	addi	a3,a3,4
	nop
	nop
	nop
	bne	a5,a2,.L2
	nop
	nop
	nop
	lui	a3,%hi(A+4)
	nop
	nop
	nop
	addi	a3,a3,%lo(A+4)
	nop
	nop
	nop
	addi	a2,a4,76
	nop
	nop
	nop
.L3:
	nop
	nop
	nop
	lw	a5,0(a4)
	nop
	nop
	nop
	addi	a3,a3,4
	nop
	nop
	nop
	addi	a4,a4,4
	nop
	nop
	nop
	addiw	a5,a5,42
	nop
	nop
	nop
	sw	a5,-4(a3)
	nop
	nop
	nop
	bne	a4,a2,.L3
	nop
	nop
	nop
	lui	a5,%hi(A+76)
	nop
	nop
	nop
	lw	a0,%lo(A+76)(a5)
	nop
	nop
	nop
	call	print_roman
	nop
	nop
	nop
	ld	ra,8(sp)
	nop
	nop
	nop
	li	a0,0
	nop
	nop
	nop
	addi	sp,sp,16
	nop
	nop
	nop
	jr	ra
	nop
	nop
	nop
	.size	main, .-main
	.comm	B,80,8
	.comm	A,80,8
	.ident	"GCC: (GNU) 8.3.0"
