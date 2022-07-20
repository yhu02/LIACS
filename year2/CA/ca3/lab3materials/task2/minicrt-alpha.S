	.set noreorder
	.set volatile
	.set noat
	.arch ev6
	.comm   _stack,4096,1
	.text
	.ent _start
	.globl _start
_start:
	# this first branch ensures that $27 contains the initial
	# program counter.
	bsr $27, _first
_first:
	# load the GP pointer for variable addresses.
	ldgp $29, 0($27)

	# create a pseudo-stack.
	lda $30, _stack
	lda $30, 4096($30)

	# call main(void).
	lda $27, main
	jsr $26,($27),0
	ldgp $29, 0($27)

	# back from main(), $0 contains the return value that we retain.

        # stop the simulation by triggering an OSF exit system call
	mov $0, $16		# move return value to $16 ($a0)
	lda $0, 1($31)		# set $v0 to 1, system call code for exit.
	callsys
	nop
	nop
	nop
	nop
	nop
	nop
	.end _start
