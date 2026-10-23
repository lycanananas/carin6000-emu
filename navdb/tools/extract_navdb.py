#!/usr/bin/env python3
"""Extracts the files of a NAV_DB map disc (ISO9660 in MODE2/2352 sectors) into a directory.

usage: extract_navdb.py [disc.img] [outdir]
       (default: cd/carin_maps_pl_2012_2013.img -> temp/navdb/)
"""
import argparse
import struct
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
SECTOR, DATA, USER = 2352, 24, 2048


def main():
    p = argparse.ArgumentParser(description="Extracts the files of a NAV_DB map disc image "
                                            "(ISO9660 in MODE2/2352 sectors), e.g. carindb.")
    p.add_argument("image", nargs="?", type=Path, default=ROOT / "cd/carin_maps_pl_2012_2013.img",
                   help="disc image (default: cd/carin_maps_pl_2012_2013.img)")
    p.add_argument("outdir", nargs="?", type=Path, default=ROOT / "temp/navdb",
                   help="output directory (default: temp/navdb)")
    args = p.parse_args()
    if not args.image.is_file():
        p.error(f"{args.image} not found")
    out = args.outdir
    out.mkdir(parents=True, exist_ok=True)
    img = open(args.image, "rb")

    def sector(lba):
        img.seek(lba * SECTOR + DATA)
        return img.read(USER)

    pvd = sector(16)
    if pvd[1:6] != b"CD001":
        sys.exit("no ISO9660 volume descriptor at LBA 16")
    root = pvd[156:156 + 34]
    root_lba, root_size = struct.unpack_from("<I", root, 2)[0], struct.unpack_from("<I", root, 10)[0]
    directory = b"".join(sector(root_lba + i) for i in range((root_size + USER - 1) // USER))
    p = 0
    while p < len(directory):
        length = directory[p]
        if length == 0:
            p = (p // USER + 1) * USER
            continue
        rec = directory[p:p + length]
        lba, size = struct.unpack_from("<I", rec, 2)[0], struct.unpack_from("<I", rec, 10)[0]
        name = rec[33:33 + rec[32]].split(b";")[0].decode("latin1")
        flags = rec[25]
        p += length
        if name in ("\0", "\1") or flags & 2:
            continue
        with open(out / name, "wb") as f:
            left, s = size, lba
            while left > 0:
                chunk = sector(s)[:left]
                f.write(chunk)
                left -= len(chunk)
                s += 1
        print(f"{name}: {size} bytes from LBA {lba}")


if __name__ == "__main__":
    main()
