	.file	"comp.c"
	.option nopic
	.text
	.section	.text.startup,"ax",@progbits
	.align	2
	.globl	main
	.type	main, @function
main:
	lui	a3,%hi(B)
	addi	sp,sp,-16
	addi	a4,a3,%lo(B)
	sd	ra,8(sp)
	addi	a3,a3,%lo(B)
	li	a5,0
	li	a2,20
.L2:
	sw	a5,0(a3)
	addiw	a5,a5,1
	addi	a3,a3,4
	bne	a5,a2,.L2
	lui	a3,%hi(A+4)
	addi	a3,a3,%lo(A+4)
	addi	a2,a4,76
.L3:
	lw	a5,0(a4)
	addi	a3,a3,4
	addi	a4,a4,4
	addiw	a5,a5,42
	sw	a5,-4(a3)
	bne	a4,a2,.L3
	lui	a5,%hi(A+76)
	lw	a0,%lo(A+76)(a5)
	call	print_roman
	ld	ra,8(sp)
	li	a0,0
	addi	sp,sp,16
	jr	ra
	.size	main, .-main
	.comm	B,80,8
	.comm	A,80,8
	.ident	"GCC: (GNU) 8.3.0"
