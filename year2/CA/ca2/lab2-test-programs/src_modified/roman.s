	.file	"roman.c"
	.option nopic
	.text
	.align	2
	.globl	print_roman
	.type	print_roman, @function
print_roman:
	nop
	nop
	nop
	lui	a4,%hi(.LANCHOR0)
	nop
	nop
	nop
	addi	a4,a4,%lo(.LANCHOR0)
	nop
	nop
	nop
	li	a5,1000
	nop
	nop
	nop
.L9:
	nop
	nop
	nop
	bgt	a5,a0,.L11
	nop
	nop
	nop
.L3:
	nop
	nop
	nop
	lbu	a5,4(a4)
	nop
	nop
	nop
	sb	a5,512(zero)
	nop
	nop
	nop
	lw	a5,0(a4)
	nop
	nop
	nop
	subw	a0,a0,a5
	nop
	nop
	nop
	ble	a5,a0,.L3
	nop
	nop
	nop
.L11:
	nop
	nop
	nop
	addi	a4,a4,8
	nop
	nop
	nop
	lw	a5,0(a4)
	nop
	nop
	nop
	bnez	a5,.L9
	nop
	nop
	nop
	li	a5,10
	nop
	nop
	nop
	sb	a5,512(zero)
	nop
	nop
	nop
	ret
	nop
	nop
	nop
	.size	print_roman, .-print_roman
	.section	.rodata
	.align	3
	.set	.LANCHOR0,. + 0
	.type	roman_table, @object
	.size	roman_table, 64
roman_table:
	.word	1000
	.byte	77
	.zero	3
	.word	500
	.byte	68
	.zero	3
	.word	100
	.byte	67
	.zero	3
	.word	50
	.byte	76
	.zero	3
	.word	10
	.byte	88
	.zero	3
	.word	5
	.byte	86
	.zero	3
	.word	1
	.byte	73
	.zero	3
	.word	0
	.byte	0
	.zero	3
	.ident	"GCC: (GNU) 8.3.0"
