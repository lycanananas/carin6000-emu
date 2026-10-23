#!/usr/bin/env python3
"""Disassembles a range of an OS-9 68k module with capstone, decoding the Microware math trap
(trap #15 followed by a function word) that Ghidra stops at. A6-relative operands are shown as
g_XXXX (data offset = 0x8000 + displacement, as resolve_a6.py names them).

usage: dis68k.py MODULE.mod START END      (hex addresses)
"""
import re
import struct
import sys

import capstone


def main():
    if len(sys.argv) != 4:
        sys.exit(__doc__.strip())
    mod = open(sys.argv[1], "rb").read()
    pc, end = int(sys.argv[2], 16), int(sys.argv[3], 16)
    md = capstone.Cs(capstone.CS_ARCH_M68K, capstone.CS_MODE_M68K_030)

    def a6(m):
        disp = int(m.group(1), 16)
        disp = disp - 0x100000000 if disp & 0x80000000 else disp
        return f"g_{0x8000 + disp:04x}"

    while pc < end:
        if mod[pc:pc + 2] == b"\x4e\x4f":
            code, = struct.unpack_from(">H", mod, pc + 2)
            print(f"{pc:06x}:  trap #15  math {code:#06x}")
            pc += 4
            continue
        ins = next(md.disasm(mod[pc:pc + 10], pc), None)
        if not ins:
            print(f"{pc:06x}:  dc.w ${mod[pc]:02x}{mod[pc + 1]:02x}")
            pc += 2
            continue
        ops = re.sub(r"\$([0-9a-f]+)\(a6, invalid\.w\)", a6, ins.op_str)
        print(f"{pc:06x}:  {ins.mnemonic} {ops}")
        pc += ins.size


if __name__ == "__main__":
    main()
