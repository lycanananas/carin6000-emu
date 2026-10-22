| Loader process entry (sysgo starts it as "ui_handler") and the jump into the Doom image.

	.section .text.start,"ax"
	.globl	_start
_start:
	bsr.l	main
	move.l	%d0,%d1
	trap	#0
	.short	0x0006			| F$Exit, status in d1.w

	.text

| void run_image(void (*entry)(struct carminat_env *), struct carminat_env *env, void *stack_top)
| Switches to the image's own stack (the module's is only a few KB) and calls the entry; Doom
| does not return.
	.globl	run_image
run_image:
	move.l	4(%sp),%a0
	move.l	8(%sp),%d0
	move.l	12(%sp),%sp
	move.l	%d0,-(%sp)
	jsr	(%a0)
1:	moveq	#0,%d1
	trap	#0
	.short	0x0006			| F$Exit
	bra.s	1b
