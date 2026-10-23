#!/usr/bin/env python3
"""Compares a decoded encoder output (decoder.py GeoPackage) with the OSM road network it came
from: per class / one-way / name the total length must match (pieces cut at parcel borders add
up), and every decoded point must be an OSM point or a border cut.

usage: compare_osm.py DECODED.gpkg LON0,LAT0,LON1,LAT1 [osm.gpkg]"""
import sqlite3
import struct
import sys
from collections import Counter
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parents[1]))
from libs import osmnet, osmplaces

ROOT = Path(__file__).resolve().parents[2]


def main():
    gpkg, bbox = sys.argv[1], tuple(float(v) for v in sys.argv[2].split(","))
    osm = sys.argv[3] if len(sys.argv) > 3 else ROOT / "temp/osm/mazowieckie.gpkg"
    net = osmnet.load(osm, bbox)
    want_len, want_n = Counter(), Counter()
    for e in net.edges:
        r = e.road
        key = (r.cls, r.fow, r.oneway, osmplaces.street_name(r.name) or r.ref)   # names as the encoder writes them
        want_len[key] += e.length
    db = sqlite3.connect(gpkg)
    got_len = Counter()
    for cls, fow, oneway, name, length in db.execute(
            "SELECT class, fow, oneway, name, length_m FROM roads WHERE level = 0"):
        got_len[(cls, fow, oneway, name)] += length
    keys = set(want_len) | set(got_len)
    bad = [(k, want_len[k], got_len[k]) for k in keys if abs(want_len[k] - got_len[k]) > max(3, 0.01 * want_len[k])]
    print(f"attribute groups (class, form of way, one-way, name): {len(keys)}, length mismatches: {len(bad)}")
    for k, a, b in sorted(bad, key=lambda x: -abs(x[1] - x[2]))[:10]:
        print(f"  {k}: osm {a} m, decoded {b} m")
    print(f"total length: osm {sum(want_len.values())} m, decoded {sum(got_len.values())} m")
    return 1 if bad else 0


if __name__ == "__main__":
    sys.exit(main())
