| OS-9/68k entry and system call wrappers (C calling convention: arguments on the stack, result in d0).
| A system call is "trap #0" followed by the function code; carry set means error, code in d1.w.
| Every wrapper returns the result (>= 0) or -error.

	.section .text.start,"ax"
	.globl	_start
_start:
	bsr.l	main
	move.l	%d0,%d1
	trap	#0
	.short	0x0006			| F$Exit, status in d1.w

	.text

| int os9_open(const char *name, int mode) -> path
	.globl	os9_open
os9_open:
	move.l	4(%sp),%a0
	move.l	8(%sp),%d0
	trap	#0
	.short	0x0084			| I$Open
	bcs.s	error
	ext.l	%d0
	rts

| int os9_read(int path, void *buf, int count) -> bytes read
	.globl	os9_read
os9_read:
	move.l	4(%sp),%d0
	move.l	8(%sp),%a0
	move.l	12(%sp),%d1
	trap	#0
	.short	0x0089			| I$Read
	bcs.s	error
	move.l	%d1,%d0
	rts

| int os9_writeln(int path, const void *buf, int count) -> bytes written
	.globl	os9_writeln
os9_writeln:
	move.l	4(%sp),%d0
	move.l	8(%sp),%a0
	move.l	12(%sp),%d1
	trap	#0
	.short	0x008c			| I$WritLn
	bcs.s	error
	move.l	%d1,%d0
	rts

| int os9_ready(int path) -> bytes waiting (SS_Ready)
	.globl	os9_ready
os9_ready:
	move.l	4(%sp),%d0
	moveq	#1,%d1			| SS_Ready
	trap	#0
	.short	0x008d			| I$GetStt
	bcs.s	error
	move.l	%d1,%d0
	rts

| int os9_sleep(int ticks)
	.globl	os9_sleep
os9_sleep:
	move.l	4(%sp),%d0
	trap	#0
	.short	0x000a			| F$Sleep
	bcs.s	error
	moveq	#0,%d0
	rts

error:
	moveq	#0,%d0
	move.w	%d1,%d0
	neg.l	%d0
	rts
