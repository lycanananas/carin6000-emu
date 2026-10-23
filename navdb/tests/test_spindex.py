#!/usr/bin/env python3
"""Rebuilds the spatial index part of original tables from their data unit boxes and compares it
byte for byte with the original. usage: test_spindex.py [table ids, hex] (default: 1e 3 2 6 16 15 1c 14 1d)"""
import struct
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parents[1]))
from libs import spindex
from libs.carindb import read_unit, table_directory, units

ROOT = Path(__file__).resolve().parents[2]
GRID = {1: 512, 7: 256, 0x1E: 32, 3: 32, 2: 8, 6: 32, 0x16: 32, 0x15: 32, 0x1C: 1, 0x14: 1, 0x1D: 1}
BOX_AT = {1: 0x44, 0x1E: 0x44, 3: 0x44, 2: 0x44, 7: 0x10}


def main():
    tids = [int(t, 16) for t in sys.argv[1:]] or [0x1E, 3, 2, 6, 0x16, 0x15, 0x1C, 0x14, 0x1D]
    f = open(ROOT / "temp/navdb/carindb", "rb")
    tables = table_directory(f)
    bad = 0
    for tid in tids:
        (a, _), (b, _), (c, _) = tables[tid]
        items = []
        for blk, length, head in units(f, b, c):
            x0, y0, x1, y1 = struct.unpack_from(">4i", head, BOX_AT.get(tid, 0x20))
            items.append((x0, y0, x1, y1, (blk << 8) | length))
        part = spindex.IndexPart(GRID[tid], items)
        made = b"".join(part.units(a, lambda k: k))
        f.seek(a * 2048)
        orig = f.read((b - a) * 2048)
        same = made == orig
        bad += not same
        msg = "identical" if same else f"DIFFERENT (sizes {len(made)} / {len(orig)})"
        if not same and len(made) == len(orig):
            i = next(i for i in range(len(orig)) if made[i] != orig[i])
            msg += f", first difference at block {a + i // 2048:#x} +{i % 2048:#x}"
        print(f"table {tid:x}: {msg}")
    return 1 if bad else 0


if __name__ == "__main__":
    sys.exit(main())
