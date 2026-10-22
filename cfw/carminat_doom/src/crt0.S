| Doom image entry, called by the loader as void _start(struct carminat_env *env) on its own stack.

	.section .text.start,"ax"
	.globl	_start
_start:
	move.l	4(%sp),carminat_env
	jsr	carminat_main
1:	moveq	#0,%d1
	trap	#0
	.short	0x0006			| F$Exit
	bra.s	1b
