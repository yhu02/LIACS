	.file	"hello.c"
	.option nopic
	.text
	.align	2
	.globl	main
	.type	main, @function
main:
	li	a5,512
	li	a4,104
	sb	a4,0(a5)
	li	a4,101
	sb	a4,0(a5)
	li	a4,108
	sb	a4,0(a5)
	sb	a4,0(a5)
	li	a4,111
	sb	a4,0(a5)
	li	a4,10
	sb	a4,0(a5)
	li	a0,0
	ret
	.size	main, .-main
	.ident	"GCC: (GNU) 8.3.0"
