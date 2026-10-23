#!/usr/bin/env python3
"""Round trip of the parcel byte layout: unpack an original parcel, read it into RawParcel, write
it again, compare byte for byte. usage: test_parcelfmt.py [every-nth-level-0-parcel] (default 50)"""
import sys
from collections import Counter
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parents[1]))
from libs import parcelfmt
from libs.carindb import TEXT_TABLES, header_info, read_unit, table_directory, units
from libs.unpack import Unpacker

ROOT = Path(__file__).resolve().parents[2]


def main():
    step = int(sys.argv[1]) if len(sys.argv) > 1 else 50
    f = open(ROOT / "temp/navdb/carindb", "rb")
    tables = table_directory(f)
    sizes, release = header_info(f)
    unpacker = Unpacker(sizes, release, TEXT_TABLES)
    result, bad = Counter(), []
    for tid, level in ((1, 0), (0x1E, 1), (3, 2), (2, 3)):
        (_, _), (data, _), (end, _) = tables[tid]
        for k, (b, length, head) in enumerate(units(f, data, end)):
            if level == 0 and k % step:
                continue
            d = bytes(unpacker.unpack(read_unit(f, (b << 8) | length)))
            out = parcelfmt.write(parcelfmt.read(d), len(d) // 2048)
            result[level, out == d] += 1
            if out != d and len(bad) < 5:
                i = next(i for i in range(len(d)) if out[i] != d[i])
                bad.append(f"level {level} unit {b:#x}: first difference at {i:#x}")
    for (level, ok), n in sorted(result.items()):
        print(f"level {level}: {n} {'identical' if ok else 'DIFFERENT'}")
    print("\n".join(bad))
    return 1 if bad else 0


if __name__ == "__main__":
    sys.exit(main())
