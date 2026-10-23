"""Places for the encoder, the way the original (Navteq) names them in Poland (docs/format.md
section 4, place hierarchy): city = gmina (OSM admin_level 7, without "gmina"), its municipality
= powiat (admin_level 6); a street's second text = the settlement it lies in (admin_level 8 area,
or the nearest place node), none in the gmina's own town. Street names get "ulica " unless they
start with another street word, as the original's do ("ulica adama mickiewicza").
"""
import math
import sqlite3
import struct

from .osmnet import fold, gps

STREET_WORDS = ("ulica", "aleja", "al.", "plac", "rondo", "osiedle", "skwer", "bulwar", "most",
                "wybrzeze", "trakt", "droga", "sciezka", "pasaz", "park", "zaulek", "szosa",
                "rynek", "deptak", "wal", "estakada", "tunel", "wiadukt", "przejscie", "podworze")
VILLAGES = ("village", "hamlet", "suburb", "quarter", "neighbourhood", "isolated_dwelling", "locality")
NEAR = 2500       # metres: a street belongs to the nearest place node within this distance


def street_name(name):
    if not name:
        return None
    first = name.split(" ", 1)[0]
    return name if first in STREET_WORDS else "ulica " + name


def _polygons(blob):
    """GeoPackage (multi)polygon -> list of outer rings in GPS units."""
    flags = blob[3]
    env = {0: 0, 1: 32, 2: 48, 3: 48, 4: 64}[(flags >> 1) & 7]
    wkb, pos = blob[8 + env:], 0
    rings = []

    def poly(p):
        order = "<" if wkb[p] == 1 else ">"
        n_rings, = struct.unpack_from(order + "I", wkb, p + 5)
        p += 9
        for r in range(n_rings):
            n, = struct.unpack_from(order + "I", wkb, p)
            pts = [gps(*struct.unpack_from(order + "dd", wkb, p + 4 + 16 * i)) for i in range(n)]
            if r == 0:
                rings.append(pts)
            p += 4 + 16 * n
        return p

    order = "<" if wkb[0] == 1 else ">"
    kind, = struct.unpack_from(order + "I", wkb, 1)
    if kind == 3:
        poly(0)
    elif kind == 6:
        n, = struct.unpack_from(order + "I", wkb, 5)
        p = 9
        for _ in range(n):
            p = poly(p)
    return rings


def _inside(pt, ring):
    x, y = pt
    inside = False
    j = len(ring) - 1
    for i in range(len(ring)):
        xi, yi = ring[i]
        xj, yj = ring[j]
        if (yi > y) != (yj > y) and x < (xj - xi) * (y - yi) / (yj - yi) + xi:
            inside = not inside
        j = i
    return inside


class Area:
    def __init__(self, name, rings):
        self.name, self.rings = name, rings
        xs = [p[0] for r in rings for p in r]
        ys = [p[1] for r in rings for p in r]
        self.box = (min(xs), min(ys), max(xs), max(ys))

    def contains(self, pt):
        b = self.box
        return b[0] <= pt[0] <= b[2] and b[1] <= pt[1] <= b[3] and any(_inside(pt, r) for r in self.rings)


class Places:
    def __init__(self, gpkg, bbox):
        db = sqlite3.connect(gpkg)
        self.levels = {6: [], 7: [], 8: [], 9: []}
        for name, level, geom in db.execute(
                "SELECT m.name, m.admin_level, m.geom FROM multipolygons m JOIN rtree_multipolygons_geom r "
                "ON m.fid = r.id WHERE r.maxx >= ? AND r.minx <= ? AND r.maxy >= ? AND r.miny <= ? AND "
                "m.boundary = 'administrative' AND m.admin_level IN ('6', '7', '8', '9')",
                (bbox[0], bbox[2], bbox[1], bbox[3])):
            rings = _polygons(geom)
            if rings and name:
                self.levels[int(level)].append(Area(fold(name), rings))
        self.nodes = []
        for name, place, geom in db.execute(
                "SELECT p.name, p.place, p.geom FROM points p JOIN rtree_points_geom r ON p.fid = r.id "
                "WHERE r.maxx >= ? AND r.minx <= ? AND r.maxy >= ? AND r.miny <= ? AND p.place IN (%s)"
                % ",".join("?" * len(VILLAGES)),
                (bbox[0] - 0.05, bbox[2] + 0.05, bbox[1] - 0.05, bbox[3] + 0.05, *VILLAGES)):
            if name:
                lon, lat = struct.unpack_from("<dd", geom, 8 + _env(geom) + 5)     # WKB point
                self.nodes.append((gps(lon, lat), fold(name)))
        self.cache = {}

    def _area(self, level, pt):
        for a in self.levels[level]:
            if a.contains(pt):
                return a.name
        return None

    def city_of(self, pt):
        """(city, municipality) at a point: (gmina, powiat) as the original."""
        key = (pt[0] >> 14, pt[1] >> 14)      # ~300 m cells: neighbours share the answer
        if key not in self.cache:
            gmina = self._area(7, pt)
            powiat = self._area(6, pt)
            city = gmina[6:] if gmina and gmina.startswith("gmina ") else gmina
            town = self._area(8, pt)
            if town == city:                # the gmina's own town: its district if it has any (Warsaw)
                town = self._area(9, pt) or town
            self.cache[key] = (city or town or "?", powiat or city or "?", town)
        return self.cache[key][:2]

    def settlement_of(self, pt):
        """The settlement a street lies in (admin_level 8: towns and, in Poland, villages too);
        none in the town the gmina is named after, as in the original. Without an admin_level 8
        area: the nearest place node."""
        city = self.city_of(pt)[0]
        place = self.cache[(pt[0] >> 14, pt[1] >> 14)][2]
        if place:
            return None if place == city else place
        best, name = NEAR / 1.8e-7 / 111320, None
        for q, n in self.nodes:
            d = math.hypot((q[0] - pt[0]) * 0.62, q[1] - pt[1])
            if d < best:
                best, name = d, n
        return name


def _env(blob):
    return {0: 0, 1: 32, 2: 48, 3: 48, 4: 64}[(blob[3] >> 1) & 7]
