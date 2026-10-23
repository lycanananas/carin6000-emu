#!/usr/bin/env python3
"""Annotates the resolved Ghidra export of dbq (tools/ghidra/resolve_a6.py output) with what is
known of the map database, and indexes which record types and header slots each function uses.

usage: annotate_dbq.py IN.resolved.md OUT.annotated.md [INDEX.txt] [--base 0xef8]
       (the record size table is at data offset 0x0ef8 in dbq, 0x0cf8 in pbp)

- the record size table is g_0ef8 + 2 * type (dbq FUN_00014dc4 sets defaults, FUN_00014f60 loads
  the table from block 0 offsets 0x28/0x2a): those globals become RECSIZE_xx;
- a header slot is (u16 offset, u16 count) at 8 + 4 * k from a unit or sub-header start: accesses
  `+ 8 + 4k)` / `+ 10 + 4k)` are listed per function as slot numbers (a hint, not proof).
"""
import re
import sys
from pathlib import Path

RECSIZE_BASE = 0x0EF8
RECSIZE_TYPES = 0x4E   # types the block 0 table can set (FUN_00014f60 / pbp: below 0x4e)


def main():
    args = sys.argv[1:]
    base = RECSIZE_BASE
    if "--base" in args:
        i = args.index("--base")
        base = int(args[i + 1], 0)
        del args[i:i + 2]
    if len(args) < 2 or args[0] in ("-h", "--help"):
        sys.exit(__doc__.strip())
    src, dst = Path(args[0]), Path(args[1])
    index_path = Path(args[2]) if len(args) > 2 else dst.with_suffix(".index.txt")
    text = src.read_text()

    def recsize(m):
        off = int(m.group(1), 16)
        if base <= off < base + 2 * RECSIZE_TYPES and (off - base) % 2 == 0:
            return f"RECSIZE_{(off - base) // 2:02x}"
        return m.group(0)

    text = re.sub(r"\(&g_([0-9a-f]{4})\)", lambda m: recsize(m) if recsize(m) != m.group(0) else m.group(0), text)
    text = text.replace("*(undefined2 *)RECSIZE_", "RECSIZE_").replace("*(ushort *)RECSIZE_", "RECSIZE_")
    dst.write_text(text)

    lines = []
    for f in re.split(r"\n## Function: ", text)[1:]:
        name = f.split("\n")[0]
        c = f.split("### Decompiled C")[1].split("### Assembly")[0] if "### Decompiled C" in f else ""
        types = sorted(set(re.findall(r"RECSIZE_([0-9a-f]{2})", c)))
        slots = set()
        for m in re.finditer(r"\+ (0x[0-9a-f]+|\d+)\)", c):
            v = int(m.group(1), 0)
            if 8 <= v < 8 + 4 * 32 and v % 2 == 0:
                slots.add((v - 8) // 4)
        calls = sorted(set(re.findall(r"\b(FUN_[0-9a-f]{8})\(", c)))
        if types:
            lines.append(f"{name}: types {' '.join(types)}  slots? {' '.join(map(str, sorted(slots)))}  calls {' '.join(calls)}")
    index_path.write_text("\n".join(lines) + "\n")
    print(f"{dst}; {len(lines)} functions use record sizes -> {index_path}")


if __name__ == "__main__":
    main()
