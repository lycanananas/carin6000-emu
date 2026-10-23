"""Byte layout of an (uncompressed or unpacked) routing parcel unit, for writing (../docs/format.md
section 4): the 116-byte header (unit header + 15 slot descriptors + box + level fields), the
slot regions in memory order 0-7, 9, 8, 10-13, each starting on a 4-byte boundary, then the text
area (slot 14). An empty slot points at the current position.

RawParcel keeps every slot region as bytes (records plus what follows them: the extra record of
slot 3, the sentinel segment of slot 4, padding), so read() + write() reproduce a unit exactly;
the builder in parcelbuild.py fills the regions from a road network. A slot whose descriptor is
(0, 0) (slot 13 after pbp's release > 0x14 pass, slot 14 on levels 1-3, which have no names) has
region / text None and stays (0, 0).
"""
import struct

from .carindb import BLOCK

HEADER = 0x74
MEMORY_ORDER = (0, 1, 2, 3, 4, 5, 6, 7, 9, 8, 10, 11, 12, 13)
TEXT_SLOT = 14


def align4(n):
    return (n + 3) & ~3


class RawParcel:
    def __init__(self, header, counts, regions, text):
        self.header = bytearray(header)    # HEADER bytes; slot descriptors are rewritten by write()
        self.counts = counts               # slot -> record count (as in the descriptor)
        self.regions = regions             # slot -> bytes
        self.text = text                   # text area bytes (slot 14 region)


def read(d):
    """RawParcel from an unpacked parcel buffer."""
    w = lambda o: struct.unpack_from(">H", d, o)[0]
    offs = {k: w(8 + 4 * k) for k in range(15)}
    counts = {k: w(10 + 4 * k) for k in range(15)}
    regions = {}
    placed = [k for k in MEMORY_ORDER if offs[k]]
    for k in MEMORY_ORDER:
        if not offs[k]:
            regions[k] = None
        elif k != placed[-1]:
            regions[k] = bytes(d[offs[k]:offs[placed[placed.index(k) + 1]]])
        else:                       # the last slot ends at the text area, or at the used data
            end = offs[TEXT_SLOT] or len(bytes(d).rstrip(b"\0"))
            regions[k] = bytes(d[offs[k]:max(end, offs[k])])
    text = bytes(d[offs[TEXT_SLOT]:]).rstrip(b"\0") if offs[TEXT_SLOT] else None
    return RawParcel(d[:HEADER], counts, regions, text)


def write(p, blocks=None):
    """Unit bytes; slot offsets are recomputed from the region sizes. blocks: pad to this many
    blocks (default: as few as needed)."""
    out = bytearray(p.header)
    pos = HEADER
    for k in MEMORY_ORDER:
        if p.regions[k] is None:
            struct.pack_into(">HH", out, 8 + 4 * k, 0, p.counts[k])
            continue
        pos = align4(pos)
        out += bytes(pos - len(out))
        struct.pack_into(">HH", out, 8 + 4 * k, pos, p.counts[k])
        out += p.regions[k]
        pos = len(out)
    if p.text is not None:
        pos = align4(pos)
        out += bytes(pos - len(out))
        struct.pack_into(">HH", out, 8 + 4 * TEXT_SLOT, pos, p.counts[TEXT_SLOT])
        out += p.text
    else:
        struct.pack_into(">HH", out, 8 + 4 * TEXT_SLOT, 0, p.counts[TEXT_SLOT])
    n = blocks or -(-len(out) // BLOCK)
    if len(out) > n * BLOCK:
        raise ValueError(f"parcel needs {len(out)} bytes, more than {n} blocks")
    return bytes(out + bytes(n * BLOCK - len(out)))
