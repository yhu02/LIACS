	.text
        .align 4
	.globl	_start
	.type	_start, @function
_start:
	jal 	x0, 0x10008
    add 	x3,x1,x2
	nop                     #jal jumps here
	nop
	nop
    auipc   x1,0
    nop
	nop
	nop
    jalr    x1,x1,8         
    add 	x1,x1,x2        
    addi 	x2,x2,5         #jalr jumps here
    nop
	nop
	nop

	.word	0xddffccff
	.size	_start, .-_start
