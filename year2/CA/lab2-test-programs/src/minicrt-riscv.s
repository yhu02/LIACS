        .comm   _stack,4096,8
        .text
        .align 2
	.globl _start
        .type _start, @function
_start:
        # Initialize gp
.option push
.option norelax
        la      gp,__global_pointer$
.option pop

        # create a pseudo-stack.
        lui     sp,%hi(_stack+4096)
        add     sp,sp,%lo(_stack+4096)
        add     sp,sp,-16

        call main

        # end simulation with exit code
        sw      a0,632(zero)
        nop
        nop
        nop
        nop
        nop
        nop
        .size   _start, .-_start
