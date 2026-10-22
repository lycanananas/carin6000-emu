| memcpy for the 68030, in place of newlib's 68000 one (7% of a frame: I_FinishUpdate copies the
| 320x200 view line by line into the frame buffer). Long words when source and destination share
| their alignment, 32 bytes per loop; words or bytes otherwise.

	.text
	.globl	memcpy
| void *memcpy(void *dst, const void *src, size_t n)
memcpy:
	move.l	4(%sp),%a0		| dst
	move.l	8(%sp),%a1		| src
	move.l	12(%sp),%d1		| n
	move.l	%a0,%d0			| return value
	beq.w	.done
	tst.l	%d1
	beq.w	.done
	move.l	%a0,-(%sp)
	move.l	%a0,%d0
	sub.l	%a1,%d0			| low bits of dst - src: 0 when equally aligned
	btst	#0,%d0
	bne.s	.bytes			| different byte alignment
	btst	#1,%d0
	bne.s	.words
| same long alignment: bytes up to a long boundary, then longs
.align_l:
	move.l	%a0,%d0
	and.l	#3,%d0
	beq.s	.longs
	move.b	(%a1)+,(%a0)+
	subq.l	#1,%d1
	bne.s	.align_l
	bra.s	.end
.longs:
	move.l	%d1,%d0
	lsr.l	#5,%d0			| 32-byte blocks
	beq.s	.tail_l
	subq.l	#1,%d0
1:	move.l	(%a1)+,(%a0)+
	move.l	(%a1)+,(%a0)+
	move.l	(%a1)+,(%a0)+
	move.l	(%a1)+,(%a0)+
	move.l	(%a1)+,(%a0)+
	move.l	(%a1)+,(%a0)+
	move.l	(%a1)+,(%a0)+
	move.l	(%a1)+,(%a0)+
	dbra	%d0,1b
	clr.w	%d0			| dbra counts 16 bits: carry the upper word
	subq.l	#1,%d0
	bcc.s	1b
.tail_l:
	and.l	#31,%d1
	move.l	%d1,%d0
	lsr.l	#2,%d0
	bra.s	3f
2:	move.l	(%a1)+,(%a0)+
3:	dbra	%d0,2b
	and.l	#3,%d1
	bra.s	.bytes
| same word alignment
.words:
	move.l	%a0,%d0
	btst	#0,%d0
	beq.s	4f
	move.b	(%a1)+,(%a0)+
	subq.l	#1,%d1
	beq.s	.end
4:	move.l	%d1,%d0
	lsr.l	#1,%d0
	bra.s	6f
5:	move.w	(%a1)+,(%a0)+
6:	subq.l	#1,%d0
	bcc.s	5b
	and.l	#1,%d1
.bytes:
	bra.s	8f
7:	move.b	(%a1)+,(%a0)+
8:	subq.l	#1,%d1
	bcc.s	7b
.end:
	move.l	(%sp)+,%d0
.done:
	rts
