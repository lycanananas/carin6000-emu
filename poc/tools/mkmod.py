#!/usr/bin/env python3
"""Wraps a flat 68k binary (linked at 0, entry at offset 0) into an OS-9/68k program module.

usage: mkmod.py CODE.bin NAME OUT.mod [--size BYTES] [--mem BYTES] [--stack BYTES]
--size pads the module to an exact length (to replace a module in loader.ini in place).
"""
import argparse
import struct
from pathlib import Path

HEADER = 0x48   # program module header: common part up to 0x30, then exec..irefs


def crc24(data):
    crc = 0xFFFFFF
    for b in data:
        crc ^= b << 16
        for _ in range(8):
            crc <<= 1
            if crc & 0x1000000:
                crc ^= 0x800063
        crc &= 0xFFFFFF
    return crc


def main():
    p = argparse.ArgumentParser()
    p.add_argument("code")
    p.add_argument("name")
    p.add_argument("out")
    p.add_argument("--size", type=lambda v: int(v, 0))
    p.add_argument("--mem", type=lambda v: int(v, 0), default=0x400)
    p.add_argument("--stack", type=lambda v: int(v, 0), default=0x2000)
    a = p.parse_args()

    code = Path(a.code).read_bytes()
    name = a.name.encode() + b"\0"
    body = bytearray(HEADER)
    exec_offset = len(body)
    body += code
    while len(body) & 1:
        body += b"\0"
    name_offset = len(body)
    body += name
    while len(body) & 1:
        body += b"\0"
    # F$Fork always runs the initialised data block: [bytes to clear][bytes to copy][data], then
    # reads the reference tables right after it ([count] for code and for data references).
    # An empty block: nothing to clear or copy (the rest of the data area is zeroed), no references.
    idata_offset = len(body)
    body += bytes(8)
    irefs_offset = len(body)
    body += bytes(8)
    size = len(body) + 3
    if a.size:
        if a.size < size:
            raise SystemExit(f"module needs {size} bytes, more than --size {a.size}")
        body += bytes(a.size - size)
        size = a.size

    # Common header: sync, sysrev 1, size, owner 0, name, access 0x555, type/lang program/68k,
    # attr reentrant, revision 1, edition 1, no usage/symbol; parity at 0x2e.
    struct.pack_into(">HHIIIHBBBBH", body, 0, 0x4AFC, 1, size, 0, name_offset, 0x0555, 1, 1, 0x80, 1, 1)
    # Program part: exec, exception, static data size, stack size (the kernel adds them, like in
    # the original modules), initialised data and references.
    struct.pack_into(">IIIIII", body, 0x30, exec_offset, 0, a.mem, a.stack, idata_offset, irefs_offset)
    parity = 0
    for i in range(0, 0x2E, 2):
        parity ^= struct.unpack_from(">H", body, i)[0]
    struct.pack_into(">H", body, 0x2E, ~parity & 0xFFFF)
    body += (~crc24(body) & 0xFFFFFF).to_bytes(3, "big")
    assert len(body) == size
    Path(a.out).write_bytes(body)
    print(f"{a.out}: {a.name}, {size} bytes, code {len(code)} bytes")


if __name__ == "__main__":
    main()
