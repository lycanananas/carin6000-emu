//
// Copyright(C) 1993-1996 Id Software, Inc.
// Copyright(C) 2005-2014 Simon Howard
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// DESCRIPTION:
//	Fixed point arithemtics, implementation.
//


#ifndef __M_FIXED__
#define __M_FIXED__




//
// Fixed point, 32bit as 16.16.
//
#define FRACBITS		16
#define FRACUNIT		(1<<FRACBITS)

typedef int fixed_t;

#ifdef CARMINAT
// CARMINAT: the 68030's 32x32->64 multiply and 64/32 divide, inline (libgcc's 64-bit
// routines are 68000 code and were ~15% of the frame).
static inline fixed_t FixedMul(fixed_t a, fixed_t b)
{
    int hi, lo = a;
    __asm__ ("muls.l %2,%0:%1" : "=d" (hi), "+d" (lo) : "dmi" (b) : "cc");
    return (hi << 16) | ((unsigned int) lo >> 16);
}

static inline fixed_t FixedDiv(fixed_t a, fixed_t b)
{
    int aa = a < 0 ? -a : a, ab = b < 0 ? -b : b;
    int hi, lo;
    if ((aa >> 14) >= ab)
        return (a ^ b) < 0 ? (-2147483647 - 1) : 2147483647;
    hi = a >> 16;
    lo = (int) ((unsigned int) a << 16);
    __asm__ ("divs.l %2,%0:%1" : "+d" (hi), "+d" (lo) : "dm" (b) : "cc");
    return lo;
}
#else
fixed_t FixedMul	(fixed_t a, fixed_t b);
fixed_t FixedDiv	(fixed_t a, fixed_t b);
#endif



#endif
