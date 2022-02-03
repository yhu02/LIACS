	.text
        .align 4
	.globl	_start
	.type	_start, @function
_start:
	or 	    x1,x1,x2
	nop
	nop
	nop
	nop
	nop
	.word	0xddffccff
	.size	_start, .-_start
