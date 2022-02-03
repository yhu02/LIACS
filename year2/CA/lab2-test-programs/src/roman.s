	.file	"roman.c"
	.option nopic
	.text
	.align	2
	.globl	print_roman
	.type	print_roman, @function
print_roman:
	lui	a4,%hi(.LANCHOR0)
	addi	a4,a4,%lo(.LANCHOR0)
	li	a5,1000
.L9:
	bgt	a5,a0,.L11
.L3:
	lbu	a5,4(a4)
	sb	a5,512(zero)
	lw	a5,0(a4)
	subw	a0,a0,a5
	ble	a5,a0,.L3
.L11:
	addi	a4,a4,8
	lw	a5,0(a4)
	bnez	a5,.L9
	li	a5,10
	sb	a5,512(zero)
	ret
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
