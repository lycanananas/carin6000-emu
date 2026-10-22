#!/usr/bin/env python3
"""Dumps what is known of a CARiN map database (carindb on a NAV_DB disc): block 0 header, the
record type table and the table directory. See ../docs/format.md.

usage: carindb_header.py [carindb]     (default: temp/navdb/carindb)
"""
import struct
import sys
from pathlib import Path

DEFAULT = Path(__file__).resolve().parents[2] / "temp/navdb/carindb"
BLOCK = 2048
GPS_UNIT = 1.8e-7      # degrees per unit; longitudes carry +30 degrees (same as the GPS receiver)


def u16(d, o):
    return struct.unpack_from(">H", d, o)[0]


def u32(d, o):
    return struct.unpack_from(">I", d, o)[0]


def lat(v):
    return v * GPS_UNIT


def lon(v):
    return v * GPS_UNIT - 30


def main():
    path = Path(sys.argv[1]) if len(sys.argv) > 1 else DEFAULT
    with open(path, "rb") as f:
        d = f.read(4 * BLOCK)
        size = f.seek(0, 2)
    print(f"{path}: {size} bytes, {size // BLOCK} blocks of {BLOCK}")
    print(f"cd number           : {u16(d, 0x02) & 0xff}")
    print(f"db release (0x1a)   : {u16(d, 0x1a)}  (pbp accepts 14..21, otherwise only warns)")
    print(f"block size (0x2c)   : {u16(d, 0x2c)}")
    bb = [u32(d, 0x34 + 4 * i) for i in range(4)]
    print(f"bounding box (0x34) : lon {lon(bb[0]):.4f}..{lat(bb[2]) - 30:.4f}  lat {lat(bb[1]):.4f}..{lat(bb[3]):.4f}"
          f"   raw {' '.join(f'{v:08x}' for v in bb)}")

    ids_off, ids_n = u16(d, 0x24), u16(d, 0x26)
    ids = [u16(d, ids_off + 2 * i) for i in range(ids_n)]
    print(f"table ids (0x{ids_off:x}, {ids_n}): {' '.join(f'{i:x}' for i in ids)}")

    rt_off, rt_n = u16(d, 0x28), u16(d, 0x2a)
    print(f"record types (0x{rt_off:x}, {rt_n}): type:size")
    pairs = [(u16(d, rt_off + 4 * i), u16(d, rt_off + 4 * i + 2)) for i in range(rt_n)]
    for k in range(0, len(pairs), 12):
        print("   " + "  ".join(f"{t:02x}:{s:3d}" for t, s in pairs[k:k + 12]))

    dir_off = (rt_off + 4 * rt_n + 15) & ~15   # padded to 16 bytes (0x21a -> 0x220 here; not confirmed)
    print(f"table directory (0x{dir_off:x}): id, three pointers block:low-byte")
    o = dir_off
    while o + 16 <= 0x800:
        a, b, c = u32(d, o), u32(d, o + 4), u32(d, o + 8)
        tid = u16(d, o + 12)
        if not (a or b or c or tid):
            break
        print(f"   id {tid:2x}: " + "  ".join(f"{v >> 8:6x}:{v & 0xff:02x}" for v in (a, b, c)))
        o += 16


if __name__ == "__main__":
    main()
