| Hand-written 68030 inner loops for the renderer (replacing the C ones in r_draw.c when
| CARMINAT is defined). Same results as the C versions.

	.text

| void R_DrawColumnLow(void): low detail wall/sprite column, each texel written as two pixels.
| 25% of a frame in C (gcc kept the loop variables on the stack and used bfextu).
	.globl	R_DrawColumnLow
R_DrawColumnLow:
	movem.l	%d2-%d7/%a2,-(%sp)
	move.l	dc_yh,%d7
	move.l	dc_yl,%d0
	sub.l	%d0,%d7			| count - 1 (dbra)
	bmi.s	9f
	lea	ylookup,%a0
	move.l	(%a0,%d0.l*4),%a0	| ylookup[dc_yl]
	move.l	dc_x,%d1
	add.l	%d1,%d1			| x = dc_x << 1
	lea	columnofs,%a1
	add.l	(%a1,%d1.l*4),%a0	| dest; dest2 = dest + 1 (columnofs is consecutive)
	move.l	dc_iscale,%d5		| fracstep
	sub.l	centery,%d0
	muls.l	%d5,%d0
	add.l	dc_texturemid,%d0
	move.l	%d0,%d2			| frac = dc_texturemid + (dc_yl - centery) * fracstep
	move.l	dc_source,%a1
	move.l	dc_colormap,%a2
	moveq	#0,%d4			| texel and colour: upper bits stay 0 for indexing
	moveq	#0,%d6
1:	move.l	%d2,%d3
	swap	%d3
	and.w	#127,%d3
	move.b	(%a1,%d3.w),%d4
	move.b	(%a2,%d4.w),%d6
	move.b	%d6,(%a0)+
	move.b	%d6,(%a0)
	add.l	%d5,%d2
	lea	319(%a0),%a0		| SCREENWIDTH - 1
	dbra	%d7,1b
9:	movem.l	(%sp)+,%d2-%d7/%a2
	rts
