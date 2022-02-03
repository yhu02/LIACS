	.text
	.align 4
	.global _start
	.type _start, @function
_start:
	li x1,38
	nop
	nop
	nop
	addi x2,x1,4
	nop
	nop
	nop
	addi x3,x1,-245
	nop
	nop
	nop
	sub x4,x2,x1
	nop
	nop
	nop
	nop
	.word 0xddffccff # test end marker
	.size _start, .-_start
# Post conditions:
#  x1 contains 38 (0x26)
#  x2 contains 42 (0x2a)
#  x3 contains -207 (0xffffffffffffff31)
#  x4 contains 4 (0x4)

