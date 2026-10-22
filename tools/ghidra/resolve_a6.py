#!/usr/bin/env python3
"""Post-processes an ExportOS9Modules.java export of a Microware C module: resolves calls through
the A6 jump table and names A6-relative globals.

usage: resolve_a6.py MODULE.mod EXPORT.md [OUT.md]   (default: rewrite EXPORT.md in place)

The process data area (A6 = data area + 0x8000) is set up by F$Fork from the module's M$IData
block: [bytes to clear][bytes to copy][the bytes], copied after the cleared part. The copied part
holds a table of "jmp abs.l" thunks (module offsets, relocated by M$IRefs), so
`(**(code **)(unaff_A6 + -N))` calls the thunk at data offset 0x8000 - N, i.e. its target.
Other `unaff_A6 + -N` accesses become `g_XXXX` (data offset, hex).
"""
import re
import struct
import sys
from pathlib import Path

BIAS = 0x8000


def thunks(mod):
    idata, = struct.unpack_from(">I", mod, 0x40)
    clear, copy = struct.unpack_from(">II", mod, idata)
    data = mod[idata + 8:idata + 8 + copy]
    table = {}
    for i in range(0, len(data) - 5, 2):
        if data[i] == 0x4E and data[i + 1] == 0xF9:
            target, = struct.unpack_from(">I", data, i + 2)
            if target < len(mod):
                table[clear + i] = target
    return table


def main():
    mod = Path(sys.argv[1]).read_bytes()
    src = Path(sys.argv[2])
    dst = Path(sys.argv[3]) if len(sys.argv) > 3 else src
    table = thunks(mod)
    text = src.read_text()

    def call(m):
        off = BIAS - int(m.group(1), 16)
        target = table.get(off)
        return f"FUN_{target:08x}" if target is not None else f"(*g_{off:04x})"

    text, ncall = re.subn(r"\(\*\*\(code \*\*\)\(unaff_A6 \+ -0x([0-9a-f]+)\)\)", call, text)
    text, nglob = re.subn(r"\(unaff_A6 \+ -0x([0-9a-f]+)\)",
                          lambda m: f"(&g_{BIAS - int(m.group(1), 16):04x})", text)
    dst.write_text(text)
    print(f"{dst}: {len(table)} thunks, {ncall} calls resolved, {nglob} globals named")


if __name__ == "__main__":
    main()
