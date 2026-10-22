| Stand-in for the installer "il" on the PoC disc: sysgo still starts it, it only sleeps, so the
| flash programming code never runs (whatever the hardware variant or the state of the unit).

	.section .text.start,"ax"
	.globl	_start
_start:
	moveq	#0,%d0			| sleep until a signal arrives
	trap	#0
	.short	0x000a			| F$Sleep
	bra.s	_start
