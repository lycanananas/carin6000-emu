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
	bcs.w	error
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
	bcs.w	error
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
	bcs.w	error
	move.l	%d1,%d0
	rts

| int os9_ready(int path) -> bytes waiting (SS_Ready)
	.globl	os9_ready
os9_ready:
	move.l	4(%sp),%d0
	moveq	#1,%d1			| SS_Ready
	trap	#0
	.short	0x008d			| I$GetStt
	bcs.w	error
	move.l	%d1,%d0
	rts

| int os9_sleep(int ticks)
	.globl	os9_sleep
os9_sleep:
	move.l	4(%sp),%d0
	trap	#0
	.short	0x000a			| F$Sleep
	bcs.w	error
	moveq	#0,%d0
	rts

| int os9_srqmem(unsigned size, void **block) -> bytes granted; size 0xffffffff = largest free block
	.globl	os9_srqmem
os9_srqmem:
	move.l	%a2,-(%sp)
	move.l	8(%sp),%d0
	trap	#0
	.short	0x0028			| F$SRqMem
	bcs.s	1f
	move.l	12(%sp),%a0
	move.l	%a2,(%a0)
	move.l	(%sp)+,%a2
	rts
1:	move.l	(%sp)+,%a2
	bra.w	error

| int os9_srtmem(void *block, unsigned size)
	.globl	os9_srtmem
os9_srtmem:
	move.l	%a2,-(%sp)
	move.l	8(%sp),%a2
	move.l	12(%sp),%d0
	trap	#0
	.short	0x0029			| F$SRtMem
	move.l	(%sp)+,%a2
	bcs.w	error
	moveq	#0,%d0
	rts

| int os9_gblkmp(void *map, unsigned size, unsigned *out): free memory map (address, size pairs)
| without allocating; out[0] fragments, out[1] total RAM, out[2] free RAM. Returns 0 or -error.
	.globl	os9_gblkmp
os9_gblkmp:
	movem.l	%d2-%d3,-(%sp)
	move.l	12(%sp),%a0
	move.l	16(%sp),%d1
	moveq	#0,%d0			| from address 0
	trap	#0
	.short	0x0019			| F$GBlkMp
	bcs.s	1f
	move.l	20(%sp),%a0
	move.l	%d1,(%a0)+
	move.l	%d2,(%a0)+
	move.l	%d3,(%a0)
	movem.l	(%sp)+,%d2-%d3
	moveq	#0,%d0
	rts
1:	movem.l	(%sp)+,%d2-%d3
	bra.w	error

| unsigned os9_ticks(unsigned *rate): ticks since midnight (F$Time, Julian with ticks), *rate per second
	.globl	os9_ticks
os9_ticks:
	movem.l	%d2-%d3,-(%sp)
	moveq	#3,%d0
	trap	#0
	.short	0x0015			| F$Time
	bcs.s	1f
	moveq	#0,%d1
	move.w	%d3,%d1			| current tick within the second
	swap	%d3
	and.l	#0xffff,%d3		| ticks per second
	move.l	12(%sp),%a0
	move.l	%d3,(%a0)
	mulu.l	%d3,%d0			| seconds since midnight * rate
	add.l	%d1,%d0
	movem.l	(%sp)+,%d2-%d3
	rts
1:	movem.l	(%sp)+,%d2-%d3
	bra.w	error

error:
	moveq	#0,%d0
	move.w	%d1,%d0
	neg.l	%d0
	rts
