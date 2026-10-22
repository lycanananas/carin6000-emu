#!/usr/bin/env python3
"""Decodes a CARiN map database (carindb) into a GeoPackage. Grows with ../docs/format.md.

usage: carindb2gpkg.py [carindb] [out.gpkg] [--bbox lon0,lat0,lon1,lat1]
       (default: temp/navdb/carindb -> temp/navdb/carindb.gpkg, everything)

Layers so far:
  parcels  table 1 units (the map parcels): bounding box, first block, length in blocks
  roads    the segment records of the parcels (slot 4, type 08) as lines between their two points,
           for every routing level (level 0 = table 1, the full network; 1, 2, 3 = tables 1e, 3, 2,
           the same parcel layout uncompressed, each with only the more important roads):
           street name, district, city, country (segment +26 -> slot 2 -> slot 1 -> slot 0), road
           class (FRC, byte +16 bits 0-3), kind (+16 bits 4-6; 5 = skipped by dbq), length in metres
           (+12), headings at both ends (+14/+15, 256 steps), one-way (+11 bits 4-5: 0 both ways,
           1 only start -> end, 2 only end -> start, 3 closed both ways), raw bytes left
  areas    background areas (tables 6, 16, 1c = levels 0, 1, 2, finer to coarser; slot 1 records):
           type (slot 0 groups), name, area in m2 (+4)
  overview_roads  the generalised road network of the same tables (slot 2 records), name = road number
  poi      table 7 units (28-byte records): position (16-bit steps of 64 GPS units from the unit box),
           category (+10), brand (+12, text in the unit), name (+0 unit pointer, +4 offset of an
           8-byte name entry in table 11)
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


ROUTING_LEVELS = ((1, 0), (0x1E, 1), (3, 2), (2, 3))
HN_NONE = 32767


def street_index(f, tables):
    """Table f (kind 0x0e): sorted street names; slot 0 = 8-byte entries (name, flags, 0, offset
    of a slot 1 record), slot 1 = 6-byte runs (offset into slot 2, count, flag), slot 2 = 24-byte
    items: u32 lon, u32 lat (the town), house numbers even from/to, odd from/to (32767 = none),
    u32 table 1 unit pointer, u16 offset of the first segment, u16 number of consecutive segments.
    Returns {(parcel block << 16) | segment offset: (even_from, even_to, odd_from, odd_to, count)}."""
    (_, _), (data, _), (end, _) = tables[0x0F]
    runs = {}
    for b, length, head in units(f, data, end):
        f.seek(b * BLOCK)
        u = f.read(length * BLOCK)
        off, count = struct.unpack_from(">HH", u, 8 + 8)
        for i in range(count):
            _, _, e0, e1, o0, o1, ptr, seg, n = struct.unpack_from(">IIHHHHIHH", u, off + 24 * i)
            runs[((ptr >> 8) << 16) | seg] = (e0, e1, o0, o1, n)
    print(f"street index: {len(runs)} runs with house numbers")
    return runs


def parcels(f, gp, tables, bbox, sizes, release):
    """Table 1. The parcel box is the record at 0x44 (after the 8-byte unit header, type 05, and
    the 60-byte parcel header, type 11): min lon, min lat, max lon, max lat. Points inside a
    parcel are steps of 64 GPS units (~1 m) from the box corner (0..512 in the smallest parcels)."""
    gp.layer("parcels", "POLYGON", [("level", "INTEGER"), ("block", "INTEGER"), ("blocks", "INTEGER"),
                                    ("compressed", "INTEGER"), ("parent", "INTEGER")])
    gp.layer("roads", "LINESTRING", [("level", "INTEGER"), ("block", "INTEGER"), ("segment", "INTEGER"), ("name", "TEXT"),
                                     ("district", "TEXT"), ("city", "TEXT"), ("country", "TEXT"),
                                     ("class", "INTEGER"), ("kind", "INTEGER"), ("oneway", "INTEGER"),
                                     ("length_m", "INTEGER"),
                                     ("heading_a", "REAL"), ("heading_b", "REAL"),
                                     ("hn_even_from", "INTEGER"), ("hn_even_to", "INTEGER"),
                                     ("hn_odd_from", "INTEGER"), ("hn_odd_to", "INTEGER"),
                                     ("b10", "INTEGER"), ("b11", "INTEGER"), ("b16", "INTEGER"), ("b17", "INTEGER")])
    unpacker = Unpacker(sizes, release, TEXT_TABLES)
    hn = street_index(f, tables)
    for tid, level in ROUTING_LEVELS:
        parcel_level(f, gp, tables[tid], level, bbox, sizes, unpacker, hn if level == 0 else {})


def parcel_level(f, gp, table, level, bbox, sizes, unpacker, hn):
    """One routing level: parcels and their segments. Word 0x58: the parcel of the level above
    (unit pointer, 0 at the top)."""
    (_, _), (data, _), (end, _) = table
    n = roads = 0
    for b, length, head in units(f, data, end):
        x1, y1, x2, y2 = struct.unpack_from(">4I", head, 0x44)
        box = (lon(x1), lat(y1), lon(x2), lat(y2))
        if bbox and (box[2] < bbox[0] or box[0] > bbox[2] or box[3] < bbox[1] or box[1] > bbox[3]):
            continue
        ring = [(box[0], box[1]), (box[2], box[1]), (box[2], box[3]), (box[0], box[3])]
        gp.add("parcels", ring, (level, b, length, head[6] & 1, struct.unpack_from(">I", head, 0x58)[0] >> 8))
        n += 1
        f.seek(b * BLOCK)
        d = unpacker.unpack(f.read(length * BLOCK))
        if d is None:
            continue
        w = lambda o: struct.unpack_from(">H", d, o)[0]
        sx = sy = POINT_STEP * GPS_UNIT
        text_start = w(8 + 0x38)

        def text(o):
            """A name in the text area (0 = none)."""
            if not o or o < text_start or o >= len(d):
                return None
            e = d.find(b"\0", o)
            return d[o:e].decode("latin1") if e > o else None

        def place(o):
            """Slot 0..2 records (type 0x40): name, second name, parent record."""
            if not o or o + 6 > len(d):
                return None, None, 0
            return text(w(o)), text(w(o + 2)), w(o + 4)
        seg_off, seg_n = w(8 + 0x10), w(8 + 0x12)
        # dbq FUN_00001890: type 08 (32 bytes) in kind 0 units, type 09 (26 bytes, no name
        # reference at +26) in the routing levels
        rs = sizes[0x08] if w(4) == 0 else sizes[0x09]
        run, run_left = None, 0
        for i in range(seg_n):
            r = seg_off + i * rs
            found = hn.get((b << 16) | r)
            if found:
                run, run_left = found, found[4]
            numbers = [None if v == HN_NONE else v for v in run[:4]] if run_left > 0 else [None] * 4
            run_left -= 1
            a, c = w(r), w(r + 2)
            if not (0 < a < len(d) - 4 and 0 < c < len(d) - 4):
                continue
            pts = [(box[0] + w(o) * sx, box[1] + w(o + 2) * sy) for o in (a, c)]
            street, district, parent = place(w(r + 26)) if rs >= 28 else (None, None, 0)
            city_a, city_b, parent = place(parent)
            country, _, _ = place(parent)
            gp.add("roads", pts, (level, b, i, street, district, city_a or city_b, country,
                                  d[r + 16] & 0x0F, (d[r + 16] >> 4) & 7, (d[r + 11] >> 4) & 3, w(r + 12),
                                  d[r + 14] * 360 / 256, d[r + 15] * 360 / 256, *numbers,
                                  d[r + 10], d[r + 11], d[r + 16], d[r + 17]))
            roads += 1
    print(f"level {level}: parcels {n}, road segments {roads}")


# POI categories (record +10), named from the names found under each code around Warsaw; codes
# not seen there are left unnamed.
POI_CATEGORIES = {
    11: "dealer samochodowy", 12: "stacja paliw", 13: "wypożyczalnia samochodów", 14: "parking",
    15: "park & ride", 20: "atrakcja turystyczna", 21: "hotel / nocleg", 22: "restauracja / kawiarnia",
    23: "bank", 24: "dom kultury", 25: "biblioteka", 26: "sąd / prokuratura", 29: "ambasada",
    30: "bankomat", 31: "informacja turystyczna", 32: "muzeum", 33: "teatr / sala koncertowa",
    35: "obiekt sportowy", 36: "kościół", 37: "cmentarz", 38: "tor / karting", 39: "park / rezerwat",
    40: "centrum wystawowe", 41: "szpital", 42: "policja", 43: "urząd", 44: "poczta",
    45: "przychodnia", 46: "apteka", 47: "sklep / centrum handlowe", 48: "miejscowość",
    49: "kino", 50: "pole golfowe", 51: "stacja kolejowa", 52: "lotnisko", 54: "dworzec autobusowy",
    56: "szkoła / przedszkole", 57: "rozrywka / nocne życie", 59: "serwis samochodowy",
    61: "firma / zakład",
}


AREA_TYPES = {1: "woda", 3: "las / park", 5: "zabudowa", 6: "cmentarz", 8: "wyspa"}
BACKGROUND_LEVELS = ((6, 0), (0x16, 1), (0x1C, 2))


def background(f, gp, tables, bbox, sizes, release):
    """Tables 6, 16, 1c (unit kinds 0x16, 0x15, 0x14, unpacked like pbp FUN_00004840): the unit box
    is at 0x20; slot 3 holds the points (8-byte absolute GPS pairs, or 16-bit x/y in steps of 64
    from the box corner when header +0x10+size(0x3f) is set); a record's shape runs from its point
    pointer (+2) to the next record's. Slot 0 groups the slot 1 records by type."""
    gp.layer("areas", "POLYGON", [("level", "INTEGER"), ("type", "INTEGER"), ("type_name", "TEXT"),
                                  ("name", "TEXT"), ("area_m2", "INTEGER"), ("block", "INTEGER")])
    gp.layer("overview_roads", "LINESTRING", [("level", "INTEGER"), ("name", "TEXT"), ("attr", "INTEGER"),
                                              ("block", "INTEGER")])
    unpacker = Unpacker(sizes, release, TEXT_TABLES)
    counts = {}
    for tid, level in BACKGROUND_LEVELS:
        (_, _), (data, _), (end, _) = tables[tid]
        na = nr = 0
        for b, length, head in units(f, data, end):
            f.seek(b * BLOCK)
            d = unpacker.unpack(f.read(length * BLOCK))
            if d is None:
                continue
            w = lambda o: struct.unpack_from(">H", d, o)[0]
            x0, y0, x1, y1 = struct.unpack_from(">4I", d, 0x20)
            if bbox and (lon(x1) < bbox[0] or lon(x0) > bbox[2] or lat(y1) < bbox[1] or lat(y0) > bbox[3]):
                continue
            wide = w(8 + 0x10 + sizes[0x3F]) == 0
            step = 8 if wide else 4

            def shape(r, rs):
                pts = []
                for p in range(w(r + 2), w(r + rs + 2), step):
                    if wide:
                        X, Y = struct.unpack_from(">II", d, p)
                    else:
                        X, Y = x0 + w(p) * POINT_STEP, y0 + w(p + 2) * POINT_STEP
                    pts.append((lon(X), lat(Y)))
                return pts

            g_off, g_cnt = w(8), w(10)
            groups = [(d[g_off + 4 * i], w(g_off + 4 * i + 2)) for i in range(g_cnt + 1)]
            off, cnt, rs = w(12), w(14), sizes[0x3A]
            for i in range(cnt):
                r = off + i * rs
                typ = None
                for k in range(g_cnt):
                    if groups[k][1] <= r and (k == g_cnt - 1 or r < groups[k + 1][1]):
                        typ = groups[k][0]
                pts = shape(r, rs)
                if len(pts) >= 3:
                    gp.add("areas", pts, (level, typ, AREA_TYPES.get(typ), cstring(d, w(r)),
                                          struct.unpack_from(">I", d, r + 4)[0], b))
                    na += 1
            off, cnt, rs = w(16), w(18), sizes[0x3C]
            for i in range(cnt):
                r = off + i * rs
                pts = shape(r, rs)
                if len(pts) >= 2:
                    gp.add("overview_roads", pts, (level, cstring(d, w(r)), struct.unpack_from(">I", d, r + 4)[0], b))
                    nr += 1
        counts[tid] = (na, nr)
    print("background: " + ", ".join(f"table {t:x}: {a} areas, {r} roads" for t, (a, r) in counts.items()))


class UnitCache:
    """Units read by pointer (block << 8 | blocks), for cross-table references."""

    def __init__(self, f, size=256):
        self.f, self.size, self.cache = f, size, {}

    def get(self, ptr):
        if ptr not in self.cache:
            if len(self.cache) >= self.size:
                self.cache.pop(next(iter(self.cache)))
            self.f.seek((ptr >> 8) * BLOCK)
            self.cache[ptr] = self.f.read((ptr & 0xFF) * BLOCK)
        return self.cache[ptr]


def cstring(d, o):
    if not o or o >= len(d):
        return None
    e = d.find(b"\0", o)
    return d[o:e].decode("latin1") if e > o else None


def poi(f, gp, tables, bbox):
    """Table 7: points of interest."""
    gp.layer("poi", "POINT", [("name", "TEXT"), ("brand", "TEXT"), ("category", "INTEGER"),
                              ("category_name", "TEXT"),
                              ("block", "INTEGER"), ("field16", "INTEGER")])
    refs = UnitCache(open(f.name, "rb"))
    (_, _), (data, _), (end, _) = tables[7]
    n = 0
    for b, length, head in units(f, data, end):
        f.seek(b * BLOCK)
        u = f.read(length * BLOCK)
        off, count = struct.unpack_from(">HH", u, 8)
        x0, y0, x1, y1 = struct.unpack_from(">4I", u, 0x10)
        if bbox and (lon(x1) < bbox[0] or lon(x0) > bbox[2] or lat(y1) < bbox[1] or lat(y0) > bbox[3]):
            continue
        for i in range(count):
            r = off + i * 28
            ptr, noff, x, y, cat, brand = struct.unpack_from(">IHHHHH", u, r)
            name = None
            if ptr:
                t = refs.get(ptr)
                if noff + 2 <= len(t):
                    name = cstring(t, struct.unpack_from(">H", t, noff)[0])
            pt = (lon(x0 + x * POINT_STEP), lat(y0 + y * POINT_STEP))
            gp.add("poi", [pt], (name, cstring(u, brand), cat, POI_CATEGORIES.get(cat), b,
                                 struct.unpack_from(">I", u, r + 16)[0]))
            n += 1
    print(f"poi: {n}")


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
        background(f, gp, tables, bbox, sizes, release)
        poi(f, gp, tables, bbox)
        gp.close()
    print(f"wrote {out}")


if __name__ == "__main__":
    main()
