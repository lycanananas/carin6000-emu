"""Routing levels 1-3 for the encoder (docs/format.md section 4, routing levels): level k keeps the
roads of class <= CLASSES[k], joins chains through nodes of degree 2 into one segment, and its
parcels are quadtree ancestors of the parcels below (parent pointer +0x58, child grid in slot 8).
A node kept on level k is kept on every level below, so its top level is well defined; the node
byte of every level stores c = 3 - top level.
"""
import struct
from dataclasses import dataclass

from . import parcelfmt
from .carindb import POINT_STEP
from .osmnet import Edge
from .parcelbuild import Box, heading, node_type, segment_defaults, sorted_pieces, _shared_mid

CLASSES = {1: 2, 2: 1, 3: 0}
KIND = {0: 0, 1: 3, 2: 2, 3: 1}         # unit kind (word +4) of the parcels of each level
SEG = 26                                # segment record type 0x09
INFINITE = 0x20C49B                     # header 0x60-0x68 above level 0


def same_road(a, b):
    return (a.cls, a.fow, a.figure, a.access, a.kind, a.paved) == (b.cls, b.fow, b.figure, b.access, b.kind, b.paved)


def kept_nodes(edges, cls_max):
    """Nodes of the level's network before joining: ends of edges of class <= cls_max with
    degree != 2, or where the road attributes change."""
    inc = {}
    for i, e in enumerate(edges):
        if e.road.cls <= cls_max and e.road.kind != 5:
            inc.setdefault(e.a, []).append(i)
            inc.setdefault(e.b, []).append(i)
    keep = set()
    for pt, lst in inc.items():
        if len(lst) != 2:
            keep.add(pt)
            continue
        e1, e2 = edges[lst[0]], edges[lst[1]]
        if not same_road(e1.road, e2.road) or (e1.road.oneway == 0) != (e2.road.oneway == 0):
            keep.add(pt)
    return keep, inc


def join(edges, cls_max, keep, inc):
    """Edges of the level: chains of level 0 edges between kept nodes (shape = all points)."""
    used = set()
    out = []
    for start in keep:
        for i in inc.get(start, ()):
            if i in used:
                continue
            pts, length, cur, road = [start], 0, start, edges[i].road
            j = i
            forward = None
            while True:
                used.add(j)
                e = edges[j]
                seq = e.points() if e.a == cur else e.points()[::-1]
                fwd = e.a == cur
                if forward is None:
                    forward = fwd
                pts += seq[1:]
                length += e.length
                cur = seq[-1]
                if cur in keep:
                    break
                nxt = [k for k in inc[cur] if k not in used]
                if not nxt:
                    break
                j = nxt[0]
            r = road
            if r.oneway in (1, 2) and not forward:
                r = _flip(r)
            out.append(Edge(pts[0], pts[-1], pts[1:-1], r, length))
    return out


def _flip(road):
    import copy
    r = copy.copy(road)
    r.oneway = {1: 2, 2: 1}[road.oneway]
    return r


def tops(edges):
    """Top routing level of every level 0 node, and the kept node sets per level."""
    keep_by = {}
    inc_by = {}
    for k in (3, 2, 1):
        keep, inc = kept_nodes(edges, CLASSES[k])
        keep_by[k], inc_by[k] = keep, inc
    for k in (2, 1):                    # a node of a higher level stays a node below
        keep_by[k] |= {p for p in keep_by[k + 1] if p in inc_by[k]}
    top = {}
    for k in (1, 2, 3):
        for p in keep_by[k]:
            top[p] = k
    return top, keep_by, inc_by


MAX_UPPER = 1 << 21                     # parcel side on levels 1-3 in the original; points are u16 steps of 64


def upper_tiles(lower, edges):
    """Leaf boxes of a level: quadtree ancestors of the lower level's parcels (each child lies in
    one parent), at most MAX_UPPER wide and split while too big. The child grid (slot 8) names the
    child at each cell centre, so children may be smaller than a cell (as in the original)."""
    from .parcelbuild import MAX_BYTES, _touches
    load = [(e.points(), 40 + 6 * len(e.shape)) for e in edges]
    from .parcelbuild import ORIGIN
    root = Box(lower[0].box.x, lower[0].box.y, lower[0].box.size)
    while not all(_inside(c.box, root) for c in lower):
        size = root.size * 2
        root = Box(ORIGIN[0] + (root.x - ORIGIN[0]) // size * size, ORIGIN[1] + (root.y - ORIGIN[1]) // size * size, size)

    def split(box):
        kids = [c for c in lower if _inside(c.box, box)]
        if not kids:
            return []
        largest = max(c.box.size for c in kids)
        items = [x for x in load if _touches(box, x[0])]
        too_big = sum(c for _, c in items) > MAX_BYTES or box.size > MAX_UPPER
        if too_big and box.size // 2 >= largest:
            h = box.size // 2
            return [leaf for dx in (0, h) for dy in (0, h) for leaf in split(Box(box.x + dx, box.y + dy, h))]
        return [box]

    return split(root)


def _inside(a, b):
    return b.x <= a.x and a.x + a.size <= b.x + b.size and b.y <= a.y and a.y + a.size <= b.y + b.size


def build_upper(parcel, level, ctx):
    """RawParcel of a level 1-3 parcel (type 09 segments, no names, child grid)."""
    box = parcel.box
    pieces = sorted_pieces(parcel)
    ends = {}
    for i, pc in enumerate(pieces):
        ends.setdefault(pc.points[0], []).append((i, 0))
        ends.setdefault(pc.points[-1], []).append((i, 2))
    border = set(parcel.borders)
    best = {pt: min(pieces[i].road.cls for i, _ in inc) for pt, inc in ends.items()}
    inner = sorted((pt for pt in ends if pt not in border), key=lambda pt: (best[pt], pt))
    edge_pts = sorted((pt for pt in ends if pt in border), key=lambda pt: (best[pt], pt))

    def a4(n):
        return (n + 3) & ~3
    o0 = parcelfmt.HEADER
    o1, o2, o3 = o0 + 20, o0 + 40, o0 + 60
    o4 = a4(o3 + 32)
    o5 = a4(o4 + SEG * (len(pieces) + 1))
    o6 = a4(o5 + 8 * len(inner))
    o7 = a4(o6 + 16 * len(edge_pts))
    n_shape = sum(len(pc.points) - 2 for pc in pieces)
    o9 = a4(o7 + 6 * n_shape)
    neighbours = sorted({id(n): n for n in parcel.borders.values()}.values(), key=lambda n: (n.box.x, n.box.y))
    o8 = a4(o9 + 8 * len(neighbours))
    # child grid: cell = the smallest child (at least 1/16 of the box), n x n cells column-major
    cell = max(min((c.box.size for c in parcel.children), default=box.size), box.size // 16)
    n_grid = box.size // cell
    o10 = o8 + 4 * n_grid * n_grid
    node_off = {pt: o5 + 8 * k for k, pt in enumerate(inner)}
    node_off.update({pt: o6 + 16 * k for k, pt in enumerate(edge_pts)})
    parcel.offsets = node_off
    seg_off = [o4 + SEG * i for i in range(len(pieces))]

    r3 = b""
    for k in range(7):
        si = next((i for i, pc in enumerate(pieces) if pc.road.cls >= k), len(pieces))
        ni = next((j for j, pt in enumerate(inner) if best[pt] >= k), len(inner))
        r3 += struct.pack(">HH", o4 + SEG * si, o5 + 8 * ni)
    r3 += struct.pack(">HH", o4 + SEG * len(pieces), o5 + 8 * len(inner))
    ring = {}
    for pt, inc in ends.items():
        if pt in border and len(inc) == 1:
            ring[inc[0]] = 0
            continue
        for k, (i, end) in enumerate(inc):
            ring[(i, end)] = seg_off[inc[(k + 1) % len(inc)][0]]
    r4, r7 = b"", b""
    shape_pos = o7
    for i, pc in enumerate(pieces):
        rd = pc.road
        speed, b6, b7, b17, w28 = segment_defaults(rd)
        b17 = (rd.access << 4) | rd.figure if (rd.figure or rd.access != 2) else b17
        pts = pc.points
        r4 += struct.pack(">HHHHHBBHBBBBHHHH", node_off[pts[0]], node_off[pts[-1]], shape_pos,
                          ring[(i, 0)], ring[(i, 2)], (rd.paved << 7) | speed,
                          (b6 << 6) | (rd.oneway << 4) | rd.fow, min(pc.length, 0xFFFF),
                          heading(pts[0], pts[1]), heading(pts[-1], pts[-2]),
                          (b7 << 7) | (rd.kind << 4) | rd.cls, b17, o10, o10, o10, 0)
        for q in pts[1:-1]:
            r7 += struct.pack(">HHBB", (q[0] - box.x) // POINT_STEP, (q[1] - box.y) // POINT_STEP, 0, 0)
            shape_pos += 6
    r4 += struct.pack(">HHH", 0, 0, shape_pos) + bytes(12) + struct.pack(">HHHH", o10, o10, o10, 0)
    first_seg = {pt: seg_off[inc[0][0]] for pt, inc in ends.items()}
    r5 = b"".join(struct.pack(">HHHBB", (pt[0] - box.x) // POINT_STEP, (pt[1] - box.y) // POINT_STEP,
                              first_seg[pt], (ctx.code(pt) << 6) | 0x10 | node_type(len(ends[pt])), 0)
                  for pt in inner)
    r6 = b""
    for pt in edge_pts:
        nb = parcel.borders[pt]
        r6 += struct.pack(">HHHBBIHH", (pt[0] - box.x) // POINT_STEP, (pt[1] - box.y) // POINT_STEP,
                          first_seg[pt], ((3 - level) << 6) | 0x20 | 5, 0, ctx.ptr(nb), nb.offsets.get(pt, 0), 0)
    r9 = b""
    for nb in neighbours:
        mx, my = _shared_mid(box, nb.box)
        r9 += struct.pack(">IHH", ctx.ptr(nb), (mx - box.x) // POINT_STEP, (my - box.y) // POINT_STEP)
    r8 = b""
    for i in range(n_grid):             # column-major like the spatial index
        for j in range(n_grid):
            cx, cy = box.x + i * cell + cell // 2, box.y + j * cell + cell // 2
            child = next((c for c in parcel.children if c.box.contains((cx, cy))), None)
            r8 += struct.pack(">I", ctx.ptr(child) if child else 0)
    header = bytearray(parcelfmt.HEADER)
    struct.pack_into(">IHBB", header, 0, ctx.ptr(parcel), KIND[level], 0, 0)
    struct.pack_into(">4i", header, 0x44, box.x, box.y, box.x + box.size, box.y + box.size)
    struct.pack_into(">IIIIIIII", header, 0x54, 0, ctx.ptr(parcel.parent) if parcel.parent else 0, cell,
                     INFINITE, INFINITE, INFINITE, 0, 1 << 16)

    def pad(b):
        return b + bytes(a4(len(b)) - len(b))
    regions = {0: bytes(20), 1: bytes(20), 2: bytes(20), 3: r3, 4: r4, 5: r5, 6: r6, 7: pad(r7), 9: r9,
               8: r8, 10: b"", 11: b"", 12: b"", 13: b""}
    counts = {0: 2, 1: 2, 2: 2, 3: 7, 4: len(pieces), 5: len(inner), 6: len(edge_pts), 7: n_shape,
              8: n_grid * n_grid, 9: len(neighbours), 10: 0, 11: 0, 12: 0, 13: 0, 14: 0}
    return parcelfmt.RawParcel(header, counts, regions, None)
