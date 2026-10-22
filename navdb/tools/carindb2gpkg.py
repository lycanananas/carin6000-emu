#!/usr/bin/env python3
"""Decodes a CARiN map database (carindb) into a GeoPackage. Grows with ../docs/format.md.

usage: carindb2gpkg.py [carindb] [out.gpkg] [--bbox lon0,lat0,lon1,lat1]
       (default: temp/navdb/carindb -> temp/navdb/carindb.gpkg, everything)

Layers so far:
  parcels  table 1 units (the map parcels): bounding box, first block, length in blocks
  roads    the segment records of the parcels (slot 4, type 08) as lines between their two points,
           with the raw attribute bytes still unexplained (see ../docs/format.md)
"""
import struct
import sys
from pathlib import Path

from gpkg import GeoPackage
from parcel import Unpacker

ROOT = Path(__file__).resolve().parents[2]
BLOCK = 2048
GPS_UNIT = 1.8e-7     # degrees per unit; longitudes carry +30 degrees


def lon(v):
    return v * GPS_UNIT - 30


def lat(v):
    return v * GPS_UNIT


# Byte tables of the text code (pbp data offsets 0x1342.., set up from its M$IData): 1, 2, 3 and
# 7 bit codes for the most frequent letters.
TEXT_TABLES = (b"ae", b"str\0", b" dghilno",
               bytes.fromhex("e0e1e2e3e4e5e7e8e9eaebecedeeeff1f2f3f4f5f6f8f9fafbfcfdac"))
SLOT_TYPES = {5: 0x10, 6: 0x06, 7: 0x0C}    # point slots and their record types
POINT_STEP = 64                             # GPS units per point coordinate step


def header_info(f):
    """Record sizes (block 0 table at 0x28/0x2a) and the db release (0x1a)."""
    f.seek(0)
    d = f.read(BLOCK)
    rt_off, rt_n = struct.unpack_from(">HH", d, 0x28)
    sizes = {}
    for i in range(rt_n):
        t, size = struct.unpack_from(">HH", d, rt_off + 4 * i)
        sizes[t] = size
    return sizes, struct.unpack_from(">H", d, 0x1a)[0]


def table_directory(f):
    """{table id: (a, b, c)} from block 0, pointers as (block, unit length)."""
    f.seek(0)
    d = f.read(BLOCK)
    rt_off, rt_n = struct.unpack_from(">HH", d, 0x28)
    o = (rt_off + 4 * rt_n + 15) & ~15
    tables = {}
    while o + 16 <= BLOCK:
        a, b, c, tid = struct.unpack_from(">IIIH", d, o)
        if not (a or b or c or tid):
            break
        tables[tid] = tuple((v >> 8, v & 0xFF) for v in (a, b, c))
        o += 16
    return tables


def units(f, first, end):
    """(block, length, first 2 KB) of the units from block first up to end; a unit starts with its
    own pointer (block << 8 | length in blocks)."""
    b = first
    while b < end:
        f.seek(b * BLOCK)
        head = f.read(BLOCK)
        ptr, = struct.unpack_from(">I", head, 0)
        if ptr >> 8 != b or not 1 <= ptr & 0xFF <= 8:
            raise SystemExit(f"block {b:#x}: no unit header ({ptr:08x})")
        yield b, ptr & 0xFF, head
        b += ptr & 0xFF


def parcels(f, gp, tables, bbox, sizes, release):
    """Table 1. The parcel box is the record at 0x44 (after the 8-byte unit header, type 05, and
    the 60-byte parcel header, type 11): min lon, min lat, max lon, max lat. Points inside a
    parcel are steps of 64 GPS units (~1 m) from the box corner (0..512 in the smallest parcels)."""
    gp.layer("parcels", "POLYGON", [("block", "INTEGER"), ("blocks", "INTEGER"), ("compressed", "INTEGER")])
    gp.layer("roads", "LINESTRING", [("block", "INTEGER"), ("segment", "INTEGER"), ("w12", "INTEGER"),
                                     ("b10", "INTEGER"), ("b11", "INTEGER"), ("b14", "INTEGER"),
                                     ("b15", "INTEGER"), ("b16", "INTEGER"), ("b17", "INTEGER")])
    unpacker = Unpacker(sizes, release, TEXT_TABLES)
    (_, _), (data, _), (end, _) = tables[1]
    n = roads = 0
    for b, length, head in units(f, data, end):
        x1, y1, x2, y2 = struct.unpack_from(">4I", head, 0x44)
        box = (lon(x1), lat(y1), lon(x2), lat(y2))
        if bbox and (box[2] < bbox[0] or box[0] > bbox[2] or box[3] < bbox[1] or box[1] > bbox[3]):
            continue
        ring = [(box[0], box[1]), (box[2], box[1]), (box[2], box[3]), (box[0], box[3])]
        gp.add("parcels", ring, (b, length, head[6] & 1))
        n += 1
        f.seek(b * BLOCK)
        d = unpacker.unpack(f.read(length * BLOCK))
        if d is None:
            continue
        w = lambda o: struct.unpack_from(">H", d, o)[0]
        sx = sy = POINT_STEP * GPS_UNIT
        seg_off, seg_n = w(8 + 0x10), w(8 + 0x12)
        rs = sizes[0x08]
        for i in range(seg_n):
            r = seg_off + i * rs
            a, c = w(r), w(r + 2)
            if not (0 < a < len(d) - 4 and 0 < c < len(d) - 4):
                continue
            pts = [(box[0] + w(o) * sx, box[1] + w(o + 2) * sy) for o in (a, c)]
            gp.add("roads", pts, (b, i, w(r + 12), d[r + 10], d[r + 11], d[r + 14], d[r + 15], d[r + 16], d[r + 17]))
            roads += 1
    print(f"parcels: {n}, road segments: {roads}")


def main():
    args = sys.argv[1:]
    bbox = None
    if "--bbox" in args:
        i = args.index("--bbox")
        bbox = tuple(float(v) for v in args[i + 1].split(","))
        del args[i:i + 2]
    src = Path(args[0]) if args else ROOT / "temp/navdb/carindb"
    out = Path(args[1]) if len(args) > 1 else ROOT / "temp/navdb/carindb.gpkg"
    with open(src, "rb") as f:
        tables = table_directory(f)
        sizes, release = header_info(f)
        gp = GeoPackage(out)
        parcels(f, gp, tables, bbox, sizes, release)
        gp.close()
    print(f"wrote {out}")


if __name__ == "__main__":
    main()
