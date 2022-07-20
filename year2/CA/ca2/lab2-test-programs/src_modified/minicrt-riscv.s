        .comm   _stack,4096,8
        .text
        .align 2
	.globl _start
        .type _start, @function
_start:
        # Initialize gp
.option push
.option norelax
        nop
	nop
	nop
        lui     gp,%hi(__global_pointer$)
	nop
	nop
	nop
        addi    gp,gp,%lo(__global_pointer$)
        nop
	nop
	nop
.option pop

        # create a pseudo-stack.
        nop
	nop
	nop
        lui     sp,%hi(_stack+4096)
 	nop
	nop
	nop
        add     sp,sp,%lo(_stack+4096)
   	nop
	nop
	nop
        add     sp,sp,-16
	nop
	nop
	nop
        call main
	nop
	nop
	nop

        # end simulation with exit code
        nop
	nop
	nop
        sw      a0,632(zero)
	nop
	nop
	nop
        .size   _start, .-_start
