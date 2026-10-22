#!/usr/bin/env python3
"""Builds a PoC disc: a copy of the NAV_SW image with one OS-9 module replaced inside every
loader.ini that holds it (byte for byte, same length), EDC/ECC of the touched sectors redone.

usage: mkdisc.py SRC.img ORIGINAL.mod NEW.mod OUT.img
"""
import sys
from pathlib import Path

from cdsector import fix_mode2_form1, is_form2

SECTOR = 2352
DATA = 24          # Mode 2 Form 1 user data offset
DATA_SIZE = 2048


def main():
    src, original, new, out = (Path(a) for a in sys.argv[1:5])
    image = bytearray(src.read_bytes())
    old_mod = original.read_bytes()
    new_mod = new.read_bytes()
    if len(old_mod) != len(new_mod):
        sys.exit(f"{new} is {len(new_mod)} bytes, must be {len(old_mod)} like {original}")

    sectors = len(image) // SECTOR
    # The data stream of all Form 1 sectors, with a map back to (sector, offset).
    stream = bytearray()
    where = []
    for s in range(sectors):
        sec = image[s * SECTOR:(s + 1) * SECTOR]
        if sec[15] == 2 and not is_form2(sec):
            stream += sec[DATA:DATA + DATA_SIZE]
            where.append(s)

    hits = 0
    start = stream.find(old_mod)
    while start >= 0:
        first, last = start // DATA_SIZE, (start + len(old_mod) - 1) // DATA_SIZE
        if where[last] - where[first] != last - first:
            sys.exit(f"module at stream offset {start:#x} is not in consecutive sectors")
        stream[start:start + len(new_mod)] = new_mod
        for i in range(first, last + 1):
            s = where[i]
            sec = image[s * SECTOR:(s + 1) * SECTOR]
            sec[DATA:DATA + DATA_SIZE] = stream[i * DATA_SIZE:(i + 1) * DATA_SIZE]
            fix_mode2_form1(sec)
            image[s * SECTOR:(s + 1) * SECTOR] = sec
        print(f"replaced module in sectors {where[first]}..{where[last]}")
        hits += 1
        start = stream.find(old_mod, start + 1)

    if not hits:
        sys.exit(f"{original} not found on {src}")
    out.write_bytes(image)
    out.with_suffix(".cue").write_text(f'FILE "{out.name}" BINARY\n  TRACK 01 MODE2/2352\n    INDEX 01 00:00:00\n')
    print(f"wrote {out} and {out.with_suffix('.cue').name}")


if __name__ == "__main__":
    main()
