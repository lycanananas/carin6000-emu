"""Minimal GeoPackage writer (SQLite, OGC GeoPackage 1.2) for points, lines and polygons in WGS 84,
without GDAL bindings, with the RTree spatial index extension (filled while writing, no triggers:
the file is written once). Check the result with `ogrinfo -al -so FILE.gpkg`.
"""
import sqlite3
import struct

WGS84 = 4326
GEOM_TYPES = {"POINT": 1, "LINESTRING": 2, "POLYGON": 3}


def _envelope(coords):
    xs = [c[0] for c in coords]
    ys = [c[1] for c in coords]
    return min(xs), max(xs), min(ys), max(ys)


def _blob(kind, coords):
    """GeoPackage binary: header with an envelope, then little-endian WKB."""
    if kind == "POINT":
        wkb = struct.pack("<BIdd", 1, 1, *coords[0])
        flags, env = 0x01, b""
    else:
        if kind == "LINESTRING":
            wkb = struct.pack("<BII", 1, 2, len(coords))
        else:   # one ring, closed
            ring = list(coords) + ([coords[0]] if coords[0] != coords[-1] else [])
            coords = ring
            wkb = struct.pack("<BIII", 1, 3, 1, len(ring))
        wkb += b"".join(struct.pack("<dd", x, y) for x, y in coords)
        flags, env = 0x03, struct.pack("<4d", *_envelope(coords))   # envelope xy, little endian
    return b"GP" + bytes((0, flags)) + struct.pack("<i", WGS84) + env + wkb


class GeoPackage:
    def __init__(self, path):
        self.db = sqlite3.connect(path)
        self.db.execute("PRAGMA application_id = 0x47504B47")
        self.db.execute("PRAGMA user_version = 10200")
        self.db.executescript("""
            CREATE TABLE IF NOT EXISTS gpkg_spatial_ref_sys (srs_name TEXT NOT NULL, srs_id INTEGER PRIMARY KEY,
                organization TEXT NOT NULL, organization_coordsys_id INTEGER NOT NULL, definition TEXT NOT NULL,
                description TEXT);
            CREATE TABLE IF NOT EXISTS gpkg_contents (table_name TEXT PRIMARY KEY, data_type TEXT NOT NULL,
                identifier TEXT UNIQUE, description TEXT DEFAULT '', last_change DATETIME NOT NULL
                DEFAULT (strftime('%Y-%m-%dT%H:%M:%fZ','now')), min_x DOUBLE, min_y DOUBLE, max_x DOUBLE,
                max_y DOUBLE, srs_id INTEGER);
            CREATE TABLE IF NOT EXISTS gpkg_geometry_columns (table_name TEXT NOT NULL, column_name TEXT NOT NULL,
                geometry_type_name TEXT NOT NULL, srs_id INTEGER NOT NULL, z TINYINT NOT NULL, m TINYINT NOT NULL,
                CONSTRAINT pk_geom_cols PRIMARY KEY (table_name, column_name));
            CREATE TABLE IF NOT EXISTS gpkg_extensions (table_name TEXT, column_name TEXT, extension_name TEXT NOT NULL,
                definition TEXT NOT NULL, scope TEXT NOT NULL,
                CONSTRAINT ge_tce UNIQUE (table_name, column_name, extension_name));
        """)
        self.db.executemany("INSERT OR IGNORE INTO gpkg_spatial_ref_sys VALUES (?,?,?,?,?,?)", [
            ("Undefined cartesian SRS", -1, "NONE", -1, "undefined", None),
            ("Undefined geographic SRS", 0, "NONE", 0, "undefined", None),
            ("WGS 84 geodetic", WGS84, "EPSG", WGS84,
             'GEOGCS["WGS 84",DATUM["WGS_1984",SPHEROID["WGS 84",6378137,298.257223563]],'
             'PRIMEM["Greenwich",0],UNIT["degree",0.0174532925199433]]', None),
        ])
        self.layers = {}

    def layer(self, name, kind, fields):
        """fields: [(name, 'INTEGER'|'REAL'|'TEXT'), ...]"""
        cols = ", ".join(f'"{n}" {t}' for n, t in fields)
        self.db.execute(f'DROP TABLE IF EXISTS "{name}"')
        self.db.execute(f'DROP TABLE IF EXISTS "rtree_{name}_geom"')
        self.db.execute(f'CREATE VIRTUAL TABLE "rtree_{name}_geom" USING rtree(id, minx, maxx, miny, maxy)')
        self.db.execute("INSERT OR REPLACE INTO gpkg_extensions VALUES (?, 'geom', 'gpkg_rtree_index', "
                        "'http://www.geopackage.org/spec120/#extension_rtree', 'write-only')", (name,))
        self.db.execute(f'CREATE TABLE "{name}" (fid INTEGER PRIMARY KEY AUTOINCREMENT, geom BLOB{", " + cols if cols else ""})')
        self.db.execute("DELETE FROM gpkg_contents WHERE table_name = ?", (name,))
        self.db.execute("DELETE FROM gpkg_geometry_columns WHERE table_name = ?", (name,))
        self.db.execute("INSERT INTO gpkg_contents (table_name, data_type, identifier, srs_id) VALUES (?, 'features', ?, ?)",
                        (name, name, WGS84))
        self.db.execute("INSERT INTO gpkg_geometry_columns VALUES (?, 'geom', ?, ?, 0, 0)", (name, kind, WGS84))
        self.layers[name] = (kind, [n for n, _ in fields], [None, None, None, None])

    def add(self, name, coords, values=()):
        kind, fields, ext = self.layers[name]
        x0, x1, y0, y1 = _envelope(coords)
        ext[0] = x0 if ext[0] is None else min(ext[0], x0)
        ext[1] = y0 if ext[1] is None else min(ext[1], y0)
        ext[2] = x1 if ext[2] is None else max(ext[2], x1)
        ext[3] = y1 if ext[3] is None else max(ext[3], y1)
        marks = ", ".join("?" * (1 + len(fields)))
        names = ", ".join(['geom'] + [f'"{f}"' for f in fields])
        cur = self.db.execute(f'INSERT INTO "{name}" ({names}) VALUES ({marks})', (_blob(kind, coords), *values))
        self.db.execute(f'INSERT INTO "rtree_{name}_geom" VALUES (?, ?, ?, ?, ?)', (cur.lastrowid, x0, x1, y0, y1))

    def close(self, styles=True):
        for name, (_, _, ext) in self.layers.items():
            if ext[0] is not None:
                self.db.execute("UPDATE gpkg_contents SET min_x=?, min_y=?, max_x=?, max_y=? WHERE table_name=?",
                                (*ext, name))
        if styles:
            from .qgis_style import write_styles
            write_styles(self.db, self.layers)
        self.db.commit()
        self.db.close()
