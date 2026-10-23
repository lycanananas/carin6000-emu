#!/usr/bin/env python3
"""OpenStreetMap -> CARiN / Carminat map database (carindb). Work in progress: see the status in
navdb/README.md.

Examples:
  navdb/encoder.py --area piaseczno              small test area -> temp/navdb/enc/carindb
  navdb/encoder.py --bbox 20.98,52.22,21.04,52.25 -o /tmp/carindb

Input is a GeoPackage made from a Geofabrik .osm.pbf with ogr2ogr (see navdb/README.md); the
original carindb (temp/navdb/carindb) is the template for the header blocks. Check the result with
  navdb/decoder.py temp/navdb/enc/carindb -o temp/navdb/enc/check.gpkg
  navdb/tests/check_db.py temp/navdb/enc/carindb
"""
import argparse
import heapq
import sys
import time
from pathlib import Path

from libs import dbwrite, levels, osmaddr, osmnet, osmplaces, parcelbuild, parcelfmt, spindex
from libs.osmnet import gps

ROOT = Path(__file__).resolve().parents[1]
AREAS = {
    "piaseczno": (20.95, 52.03, 21.12, 52.12),
    "warszawa": (20.85, 52.10, 21.27, 52.37),
    "mazowieckie": (19.25, 51.00, 23.15, 53.50),
}
TABLES = {0: 1, 1: 0x1E, 2: 3, 3: 2}         # routing level -> table id
GRIDS = {0: 512, 1: 32, 2: 32, 3: 8}         # spatial index grid N per level (as the original)
# the router's default speeds in m/s (rpmod data 0x18ca): form of way 0 (motorway), 3, others; by class
SPEEDS = {0: (36, 36, 36, 36, 7.5, 4.5, 3), 3: (28, 28, 16.5, 12, 7.5, 4.5, 3),
          -1: (24, 24, 16.5, 12, 7.5, 4.5, 3)}


class Context:
    """What the parcel builders ask: places, node codes and unit pointers."""

    def __init__(self, db, keys, top):
        self.db, self.keys, self.top = db, keys, top
        self.country = ("polska", 171)

    def city_of(self, road):
        return road.city, road.muni

    def district_of(self, road):
        return road.settlement

    def code(self, point):
        return 3 - self.top.get(point, 0)

    def ptr(self, parcel):
        return self.db.resolve(self.keys[id(parcel)])

    def hn_ptr(self, parcel):
        key = self.keys.get(("hn", id(parcel)))
        return self.db.resolve(key) if key else 0


def travel_times(edges, top, parcels):
    """Header 0x60 / 0x64 / 0x68 of the level 0 parcels: mean time (0.1 s) from a parcel's nodes to
    the nearest node of level 3 / 2 / 1, with the router's default speeds."""
    back = {}
    for e in edges:
        t = 10 * e.length / SPEEDS.get(e.road.fow if e.road.fow in (0, 3) else -1)[e.road.cls]
        if e.road.oneway in (0, 1):
            back.setdefault(e.b, []).append((e.a, t))
        if e.road.oneway in (0, 2):
            back.setdefault(e.a, []).append((e.b, t))
    result = {}
    for k in (3, 2, 1):
        dist = {p: 0.0 for p, lv in top.items() if lv >= k}
        heap = [(0.0, p) for p in dist]
        while heap:
            d, u = heapq.heappop(heap)
            if d > dist[u]:
                continue
            for v, t in back.get(u, ()):
                if d + t < dist.get(v, 1e18):
                    dist[v] = d + t
                    heapq.heappush(heap, (d + t, v))
        result[k] = dist
    for pc in parcels:
        nodes = {p for piece in pc.pieces for p in (piece.points[0], piece.points[-1]) if p not in pc.borders}
        times = []
        for k in (3, 2, 1):
            ts = [result[k][p] for p in nodes if p in result[k]]
            times.append(round(sum(ts) / len(ts)) if ts else levels.INFINITE)
        pc.times = tuple(times)


def main():
    p = argparse.ArgumentParser(prog="encoder.py", description=__doc__.split("\n")[0])
    p.add_argument("-a", "--area", choices=AREAS, default="piaseczno")
    p.add_argument("-b", "--bbox", metavar="LON0,LAT0,LON1,LAT1")
    p.add_argument("--osm", type=Path, default=ROOT / "temp/osm/mazowieckie.gpkg",
                   help="GeoPackage from ogr2ogr (default: temp/osm/mazowieckie.gpkg)")
    p.add_argument("--template", type=Path, default=ROOT / "temp/navdb/carindb",
                   help="original carindb for the header blocks")
    p.add_argument("-o", "--output", type=Path, default=ROOT / "temp/navdb/enc/carindb")
    args = p.parse_args()
    bbox = tuple(float(v) for v in args.bbox.split(",")) if args.bbox else AREAS[args.area]
    for path, what in ((args.osm, "OSM GeoPackage"), (args.template, "template carindb")):
        if not path.is_file():
            p.error(f"{what} {path} not found (see navdb/README.md)")
    t0 = time.monotonic()

    # network and places
    net = osmnet.load(args.osm, bbox)
    places = osmplaces.Places(args.osm, bbox)
    roads = {}
    for e in net.edges:
        r = e.road
        if id(r) not in roads:
            mid = e.points()[len(e.points()) // 2]
            r.city, r.muni = places.city_of(mid)
            r.settlement = places.settlement_of(mid)
            r.name = osmplaces.street_name(r.name) or r.ref
            roads[id(r)] = r
    print(f"roads: {len(net.edges)} edges, {len(net.nodes)} nodes; "
          f"{len({(r.city, r.muni) for r in roads.values()})} cities, "
          f"{len({r.settlement for r in roads.values()} - {None})} settlements")

    # level 0 parcels and house numbers
    gbox = (*gps(bbox[0], bbox[1]), *gps(bbox[2], bbox[3]))
    leaves = parcelbuild.tile(net.edges, gbox)
    parcelbuild.nudge_edges(net.edges, leaves)
    by_level = {0: parcelbuild.clip(net.edges, leaves)}
    addresses = osmaddr.load(args.osm, bbox)
    numbered = osmaddr.assign(by_level[0], addresses)

    # routing levels 1-3
    top, keep_by, inc_by = levels.tops(net.edges)
    for k in (1, 2, 3):
        edges = levels.join(net.edges, levels.CLASSES[k], keep_by[k], inc_by[k])
        lower = by_level[k - 1]
        if not edges or not lower:
            by_level[k] = []
            continue
        by_level[k] = parcelbuild.clip(edges, levels.upper_tiles(lower, edges))
        for child in lower:
            child.parent = next((pc for pc in by_level[k] if levels._inside(child.box, pc.box)), None)
            if child.parent:
                child.parent.children.append(child)
    travel_times(net.edges, top, by_level[0])
    for k, pcs in by_level.items():
        print(f"level {k}: {len(pcs)} parcels, {sum(len(pc.pieces) for pc in pcs)} segments, "
              f"{sum(len(pc.borders) for pc in pcs) // 2} border crossings"
              + (f", {len(addresses)} addresses on {numbered} segments" if k == 0 else ""))

    # tables
    db = dbwrite.Database(args.template)
    keys = {}
    for k, pcs in by_level.items():
        for n, pc in enumerate(pcs):
            keys[id(pc)] = (f"p{k}", n)
    ctx = Context(db, keys, top)
    for k, pcs in by_level.items():
        if k == 0:
            build = lambda pc: parcelfmt.write(parcelbuild.build_level0(pc, ctx))
        else:
            build = lambda pc, k=k: parcelfmt.write(levels.build_upper(pc, k, ctx))
        units = [dbwrite.Unit(keys[id(pc)], lambda resolve, pc=pc, build=build: build(pc)) for pc in pcs]
        index = spindex.IndexPart(GRIDS[k], [(pc.box.x, pc.box.y, pc.box.x + pc.box.size,
                                              pc.box.y + pc.box.size, keys[id(pc)]) for pc in pcs])
        db.add(dbwrite.Table(TABLES[k], units, index))
    hn_units = []
    for n, pc in enumerate(by_level[0]):
        if any(getattr(piece, "hn", None) for piece in pc.pieces):
            keys[("hn", id(pc))] = ("hn", n)
            hn_units.append(dbwrite.Unit(("hn", n), lambda resolve, pc=pc, n=n: osmaddr.unit(
                resolve(keys[id(pc)]), resolve(("hn", n)), parcelbuild.sorted_pieces(pc))))
    db.add(dbwrite.Table("hn", hn_units))

    args.output.parent.mkdir(parents=True, exist_ok=True)
    blocks = db.write(args.output, gbox)
    print(f"wrote {args.output}: {blocks} blocks ({blocks * 2048 / 1e6:.1f} MB) in {time.monotonic() - t0:.1f} s")


if __name__ == "__main__":
    sys.exit(main())
