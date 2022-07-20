# Example of how to write tests for store instructions. In order to
# execute a store instruction, a writable memory must be available.
# To do so, we allocate a memory in the assembly program ("stack" in
# this case). After assembling the program, the memory location of the
# array must be determined. This can be done using "objdump". In this
# case, the location is 0x11100 (69632 decimal). We initialize the "sp"
# (R2) register with the value 69632+4096=73728. This way, the store
# instruction in the program below will write to a valid memory address.

	.bss
	.align 8
	.local	stack
	.comm	stack,4096,8
	.size	stack,4096
	.text
	.align 4
	.globl	_start
	.type	_start, @function
_start:
	add	sp,sp,-32
	sd	s0,24(sp)
	.word	0xddffccff
	.size	_start, .-_start
