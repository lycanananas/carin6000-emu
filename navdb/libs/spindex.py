"""Spatial index parts of the map tables (units of kind 8 and 9 between a table's index start and its
data start) and the index directory in block 2, which gives every indexed table its grid extent and
zoom range.

Index directory (table 4, block 2): 28-byte records from 0x14: u32 index start pointer, i32 x0, y0,
x1, y1 (the grid extent, 2^29 units square, the same for all tables), u16 a (meaning open), u16 zoom
from, u16 zoom to, u16 0.

Kind 8 units: slot 0 (word 8 offset, word 10 count) holds u32 pointers to kind 9 units, 0 = empty;
all kind 8 units of a table together form an N x N grid over the extent, cell i = column i // N
(x, west to east), row i % N (y, south to north). Kind 9 units: slot 0 holds n x n u16 offsets
(subcells in the same order), slot 1 the unit pointers; an offset is the byte position of a pointer
in this unit (0 = no data there). Every subcell points to the one data unit covering it.
"""
import math
import struct

from .carindb import BLOCK, read_unit


def directory(f):
    """{index start pointer: (x0, y0, x1, y1, a, zoom_from, zoom_to)} from block 2."""
    f.seek(2 * BLOCK)
    d = f.read(BLOCK)
    out = {}
    for o in range(0x14, BLOCK - 28, 28):
        ptr, x0, y0, x1, y1, a, z0, z1, pad = struct.unpack_from(">I4i4H", d, o)
        if ptr == 0:
            if x0 or y0 or x1 or y1:
                break
            continue
        if x1 - x0 != 1 << 29:
            break
        out[ptr] = (x0, y0, x1, y1, a, z0, z1)
    return out


def cells(f, first, data):
    """Yield (x0, y0, size, unit pointer) for every subcell of the index between block first and the
    data start, using the extent from the block 2 directory."""
    f.seek(first * BLOCK)
    ptr0, = struct.unpack_from(">I", f.read(4), 0)
    ext = directory(f)[ptr0]
    top, b = [], first
    while b < data:
        u = read_unit(f, (b << 8) | 8)
        n = u[3]
        if struct.unpack_from(">H", u, 4)[0] != 8:
            break
        off, cnt = struct.unpack_from(">HH", u, 8)
        top += struct.unpack_from(f">{cnt}I", u, off)
        b += n
    grid = math.isqrt(len(top))
    leaf = (ext[2] - ext[0]) // grid
    for i, p in enumerate(top):
        if not p:
            continue
        u = read_unit(f, p)
        off, cnt = struct.unpack_from(">HH", u, 8)
        sub = math.isqrt(cnt)
        size = leaf // sub
        lx, ly = ext[0] + (i // grid) * leaf, ext[1] + (i % grid) * leaf
        for k, o in enumerate(struct.unpack_from(f">{cnt}H", u, off)):
            if o:
                yield lx + (k // sub) * size, ly + (k % sub) * size, size, struct.unpack_from(">I", u, o)[0]
