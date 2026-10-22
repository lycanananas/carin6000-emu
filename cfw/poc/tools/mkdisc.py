#!/usr/bin/env python3
"""Builds a PoC disc: a copy of the NAV_SW image with OS-9 modules replaced by name inside every
loader.ini on it (both hardware variants, Mk2IC/0109 and Mk2IC/0209, each in RENAULT/ and
RENAULT/app_sw/), byte for byte with the same length; EDC/ECC of the touched sectors redone.

usage: mkdisc.py SRC.img OUT.img NAME=NEW.mod [NAME=NEW.mod ...]

Every new module must be exactly as long as each module of that name on the disc (the copies in
the variants may differ in content, not in length, otherwise the build stops).
"""
import struct
import sys
from pathlib import Path

from cdsector import fix_mode2_form1, is_form2

SECTOR = 2352
DATA = 24          # Mode 2 Form 1 user data offset
DATA_SIZE = 2048
COPIES = 4         # loader.ini files on NAV_SW: 2 variants x (RENAULT/, RENAULT/app_sw/)


def modules(stream, name):
    """Offsets and sizes of the OS-9 modules called NAME in the data stream (header parity checked)."""
    want = name.encode() + b"\0"
    at = stream.find(b"\x4a\xfc\x00\x01")
    while at >= 0:
        size, _, name_off = struct.unpack_from(">III", stream, at + 4)
        parity = 0
        for i in range(0, 0x30, 2):
            parity ^= struct.unpack_from(">H", stream, at + i)[0]
        if parity == 0xFFFF and 0x30 <= name_off < size and stream[at + name_off:at + name_off + len(want)] == want:
            yield at, size
        at = stream.find(b"\x4a\xfc\x00\x01", at + 2)


def main():
    if len(sys.argv) < 4:
        sys.exit(__doc__)
    src, out = Path(sys.argv[1]), Path(sys.argv[2])
    replacements = []
    for arg in sys.argv[3:]:
        name, _, path = arg.partition("=")
        replacements.append((name, Path(path).read_bytes()))

    image = bytearray(src.read_bytes())
    sectors = len(image) // SECTOR
    # The data stream of all Form 1 sectors, with a map back to the sector numbers.
    stream = bytearray()
    where = []
    for s in range(sectors):
        sec = image[s * SECTOR:(s + 1) * SECTOR]
        if sec[15] == 2 and not is_form2(sec):
            stream += sec[DATA:DATA + DATA_SIZE]
            where.append(s)

    touched = set()
    for name, new_mod in replacements:
        found = list(modules(stream, name))
        if len(found) != COPIES:
            sys.exit(f"{name}: found {len(found)} copies on {src}, expected {COPIES}")
        for start, size in found:
            if size != len(new_mod):
                sys.exit(f"{name} at stream offset {start:#x} is {size} bytes, the new one {len(new_mod)}")
            first, last = start // DATA_SIZE, (start + size - 1) // DATA_SIZE
            if where[last] - where[first] != last - first:
                sys.exit(f"{name} at stream offset {start:#x} is not in consecutive sectors")
            stream[start:start + size] = new_mod
            touched.update(range(first, last + 1))
            print(f"{name}: replaced in sectors {where[first]}..{where[last]}")

    for i in sorted(touched):
        s = where[i]
        sec = image[s * SECTOR:(s + 1) * SECTOR]
        sec[DATA:DATA + DATA_SIZE] = stream[i * DATA_SIZE:(i + 1) * DATA_SIZE]
        fix_mode2_form1(sec)
        image[s * SECTOR:(s + 1) * SECTOR] = sec

    out.write_bytes(image)
    out.with_suffix(".cue").write_text(f'FILE "{out.name}" BINARY\n  TRACK 01 MODE2/2352\n    INDEX 01 00:00:00\n')
    print(f"wrote {out} and {out.with_suffix('.cue').name}")


if __name__ == "__main__":
    main()
