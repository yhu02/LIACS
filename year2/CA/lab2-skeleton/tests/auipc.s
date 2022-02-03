	.text
        .align 4
	.globl	_start
	.type	_start, @function
_start:
	auipc 	    x2,2
	nop
	nop
	nop
	nop
	nop
	.word	0xddffccff
	.size	_start, .-_start
