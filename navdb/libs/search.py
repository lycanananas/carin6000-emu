"""Search tables for the encoder (docs/format.md section 7): countries (b, block 4) and their name
trie (c, block 3), city name trie (e), cities (d), street tries per city (10), streets with house
number ranges (f). POI tables (7, 11, 12) are left empty for now.

Cities are the gminas (municipality = powiat) and the settlements (municipality = gmina), as in
the original; a gmina's streets have one entry for the whole gmina plus one per settlement, a
settlement's streets one entry each. Every name also gets its word rotations ("ulica adama
mickiewicza" -> "adama mickiewicza ulica", "mickiewicza ulica adama"; flags 0x1012 instead of
0x12), so a street can be found by any word.
"""
import struct
from dataclasses import dataclass, field

from .carindb import BLOCK

LANG = 0x12              # Polish
ROTATED = 0x1000
LEAF_MAX = 150           # a trie node becomes a leaf at this many entries (original leaves: up to ~160)
UNIT_MAX = 8 * BLOCK


def rotations(name, street=True):
    """The name and its word rotations: for streets the leading street word ("ulica") goes where
    the rotation cuts; no rotation starts with a number."""
    words = name.split()
    out = [(name, LANG)]
    if street and len(words) >= 2:
        head, rest = words[0], words[1:]
        for i in range(len(rest)):
            if not rest[i].isdigit():
                out.append((" ".join(rest[i:] + [head] + rest[:i]), LANG | ROTATED))
    elif not street and len(words) >= 2:
        for i in range(1, len(words)):
            if not words[i].isdigit():
                out.append((" ".join(words[i:] + words[:i]), LANG | ROTATED))
    return out


@dataclass(eq=False)
class Item:
    """A run of consecutive segments of one street in one parcel (table f slot 2)."""
    pos: tuple                     # position of the settlement / town (GPS units)
    parcel: object
    seg_index: int                 # index in the parcel's segment order
    count: int
    numbers: tuple                 # even from, even to, odd from, odd to


@dataclass(eq=False)
class City:
    name: str
    muni: str
    pos: tuple
    seg: tuple                     # (parcel, segment index) of the city's destination segment
    entries: list = field(default_factory=list)   # [(name, flags, settlement or None, [Item])]
    roots: object = None           # trie node list (set by the builder)


def _numbers(pieces):
    even, odd = [], []
    for pc in pieces:
        hn = getattr(pc, "hn", None)
        if hn:
            for v in hn[:4]:
                if v != 32767:
                    (even if v % 2 == 0 else odd).append(v)
    e = (min(even), max(even)) if even else (32767, 32767)
    o = (min(odd), max(odd)) if odd else (32767, 32767)
    return (*e, *o)


def collect(parcels, sorted_pieces, settlement_pos):
    """Cities with their street entries from the level 0 parcels. settlement_pos(name, city) ->
    position of a settlement or town."""
    streets = {}                   # (city, muni, settlement, name) -> [Item]
    for pc in parcels:
        pieces = sorted_pieces(pc)
        i = 0
        while i < len(pieces):
            rd = pieces[i].road
            j = i
            while j + 1 < len(pieces) and pieces[j + 1].road.name == rd.name and \
                    pieces[j + 1].road.settlement == rd.settlement and pieces[j + 1].road.city == rd.city:
                j += 1
            if rd.name:
                key = (rd.city, rd.muni, rd.settlement, rd.name)
                pos = settlement_pos(rd.settlement or rd.city, rd.city)
                streets.setdefault(key, []).append(Item(pos, pc, i, j - i + 1, _numbers(pieces[i:j + 1])))
            i = j + 1
    cities = {}

    def city(name, muni):
        if (name, muni) not in cities:
            cities[(name, muni)] = City(name, muni, settlement_pos(name, None if name == muni else muni), None)
        return cities[(name, muni)]

    by_gmina = {}
    for (c, m, s, n), items in streets.items():
        by_gmina.setdefault((c, m, n), []).append((s, items))
        if s:
            city(s, c).entries.append((n, None, None, items))
    for (c, m, n), parts in by_gmina.items():
        g = city(c, m)
        g.entries.append((n, None, None, [it for _, its in parts for it in its]))
        for s, its in parts:
            if s:
                g.entries.append((n, None, s, its))
    for c in cities.values():
        rows = []
        for n, _, s, items in c.entries:
            for text, flags in rotations(n):
                rows.append((text, flags, s, items))
        c.entries = sorted(rows, key=lambda r: (r[0], r[2] or ""))
    return list(cities.values())


# --- tries -------------------------------------------------------------------------------

@dataclass(eq=False)
class Node:
    char: int
    leaf: bool
    first: int                     # leaf: index of the first entry in the sorted list
    count: int
    children: list = None


def trie(names, depth=0, lo=0, hi=None):
    """Node list for names[lo:hi] (sorted) at this depth."""
    hi = len(names) if hi is None else hi
    nodes = []
    i = lo
    while i < hi:
        ch = names[i][depth] if depth < len(names[i]) else " "
        j = i
        while j < hi and (names[j][depth] if depth < len(names[j]) else " ") == ch:
            j += 1
        exhausted = any(depth + 1 >= len(names[k]) for k in range(i, j))
        if j - i <= LEAF_MAX or exhausted or depth > 30:
            nodes.append(Node(ord(ch), True, i, j - i))
        else:
            kids = trie(names, depth + 1, i, j)
            nodes.append(Node(ord(ch), False, i, len(kids), kids))
        i = j
    return nodes


def leaves(nodes):
    for nd in nodes:
        if nd.leaf:
            yield nd
        else:
            yield from leaves(nd.children)


def node_lists(nodes):
    """Every node list of a trie, parents first."""
    out = [nodes]
    for nd in nodes:
        if not nd.leaf:
            out += node_lists(nd.children)
    return out


# --- units -------------------------------------------------------------------------------

def _a4(n):
    return (n + 3) & ~3


class Texts:
    def __init__(self):
        self.order, self.size = [], 0

    def need(self, *texts):
        return sum(len(t) + 1 for t in set(texts) if t and t not in self.order)

    def add(self, t):
        if t and t not in self.order:
            self.order.append(t)
            self.size += len(t) + 1


class NameUnit:
    """One unit of table d or f: name entries (slot 0) with their records, then the texts."""

    def __init__(self, kind):
        self.kind, self.rows, self.texts = kind, [], Texts()
        self.n_runs = self.n_items = 0

    def size_with(self, rows):
        e = len(self.rows) + len(rows)
        if self.kind == 0x0E:
            runs = self.n_runs + len(rows)
            items = self.n_items + sum(len(r[3]) for r in rows)
            body = _a4(0x30 + 8 * e) + _a4(6 * runs) + 24 * items
        else:
            body = _a4(0x30 + 8 * e) + 24 * e
        texts = Texts()
        texts.order, texts.size = list(self.texts.order), self.texts.size
        for r in rows:
            for t in (r[0], r[2]):
                texts.add(t)
        return body + texts.size

    def add(self, rows):
        for r in rows:
            self.rows.append(r)
            self.texts.add(r[0])
            self.texts.add(r[2])
            self.n_runs += 1
            self.n_items += len(r[3]) if self.kind == 0x0E else 0


def pack(groups, kind):
    """Units for consecutive row groups (a trie leaf's rows stay in one unit). Returns the units
    and, per group, (unit index, first row index)."""
    units, where = [], []
    for rows in groups:
        if not units or units[-1].size_with(rows) > UNIT_MAX:
            units.append(NameUnit(kind))
        u = units[-1]
        if u.size_with(rows) > UNIT_MAX:
            raise ValueError(f"{len(rows)} names do not fit one unit")
        where.append((len(units) - 1, len(u.rows)))
        u.add(rows)
    return units


def name_unit_bytes(u, own, nxt, prv, parcel_ptr, record):
    """Bytes of a d / f unit. record(row, texts) -> record bytes (24 B city record, or the f run
    and items, handled here)."""
    off = {}
    n = len(u.rows)
    o_runs = _a4(0x30 + 8 * n)
    if u.kind == 0x0E:
        o_items = _a4(o_runs + 6 * n)
        o_text = o_items + 24 * sum(len(r[3]) for r in u.rows)
    else:
        o_items = None
        o_text = o_runs + 24 * n
    pos = o_text
    for t in u.texts.order:
        off[t] = pos
        pos += len(t) + 1
    d = bytearray(-(-pos // BLOCK) * BLOCK)
    if u.kind == 0x0E:
        struct.pack_into(">IHHHHHHHH", d, 0, own, u.kind, 0, 0x30, n, o_runs, n, o_items,
                         sum(len(r[3]) for r in u.rows))
    else:
        struct.pack_into(">IHHHHHHHHHHHH", d, 0, own, u.kind, 0, 0x30, n, o_runs, n, 0, 0,
                         o_text, 0, 0, 0)
    struct.pack_into(">II", d, 0x28, nxt, prv)
    item_pos = o_items
    for i, r in enumerate(u.rows):
        name, flags, second, payload = r
        rec = o_runs + (6 if u.kind == 0x0E else 24) * i
        struct.pack_into(">HHHH", d, 0x30 + 8 * i, off[name], flags, off.get(second, 0), rec)
        if u.kind == 0x0E:
            struct.pack_into(">HBBH", d, rec, item_pos, min(len(payload), 255), 1, 0)
            for it in payload[:255]:
                struct.pack_into(">II4HIHH", d, item_pos, it.pos[0], it.pos[1], *it.numbers,
                                 parcel_ptr(it.parcel), it.parcel.seg_base + 32 * it.seg_index, it.count)
                item_pos += 24
        else:
            d[rec:rec + 24] = record(payload)
    for t, o in off.items():
        d[o:o + len(t)] = t.encode("latin1")
    return bytes(d)


def trie_unit_bytes(own, kind, lists, place):
    """Bytes of a table 10 / e / c unit holding node lists; place(node) -> (ptr, off, count) for a
    leaf; children lists are in the same unit."""
    start, pos = {}, 0x0C
    for lst in lists:
        start[id(lst)] = pos
        pos += 12 * len(lst)
    d = bytearray(-(-pos // BLOCK) * BLOCK)
    struct.pack_into(">IHHHH", d, 0, own, kind, 0, 0x0C, (pos - 0x0C) // 12)
    for lst in lists:
        o = start[id(lst)]
        for nd in lst:
            if nd.leaf:
                ptr, off, cnt = place(nd)
                struct.pack_into(">IBBHHH", d, o, ptr, nd.char, 1, off, cnt, 0)
            else:
                struct.pack_into(">IBBHHH", d, o, own, nd.char, 0, start[id(nd.children)], len(nd.children), 0)
            o += 12
    return bytes(d), start


def add_tables(db, cities, country, template_country, Unit, Table, parcel_ptr):
    """Registers tables e, d, 10, f and blocks 3 / 4 in a dbwrite.Database. template_country = the
    56-byte country record of the original (fields not known are kept); parcel_ptr(parcel) gives a
    level 0 parcel's unit pointer."""
    cities = sorted(cities, key=lambda c: (c.name, c.muni))
    # f: street entries per city, leaves kept in one unit
    f_groups, f_leaf = [], {}
    for c in cities:
        names = [r[0] for r in c.entries]
        c.trie = trie(names)
        for leaf in leaves(c.trie):
            f_leaf[id(leaf)] = len(f_groups)
            f_groups.append(c.entries[leaf.first:leaf.first + leaf.count])
    f_units = pack(f_groups, 0x0E)
    placement, ui, used = [], 0, 0          # (unit index, first row) per group, as pack() placed them
    unit_rows = [len(u.rows) for u in f_units]
    for rows in f_groups:
        if used + len(rows) > unit_rows[ui]:
            ui, used = ui + 1, 0
        placement.append((ui, used))
        used += len(rows)
    f_keys = [("f", i) for i in range(len(f_units))]

    def f_place(leaf):
        ui_, row = placement[f_leaf[id(leaf)]]
        return db.resolve(f_keys[ui_]), 0x30 + 8 * row, leaf.count

    # 10: one unit per city with its node lists
    t10_units = []
    for k, c in enumerate(cities):
        lists = node_lists(c.trie)
        c.t10_key = ("t10", k)
        c.t10_lists = lists
        t10_units.append(Unit(c.t10_key, lambda resolve, lists=lists, key=c.t10_key:
                              trie_unit_bytes(resolve(key), 0x0F, lists, f_place)[0]))

    def roots_of(c):
        _, start = trie_unit_bytes(0, 0x0F, c.t10_lists, lambda nd: (0, 0, 0))
        return db.resolve(c.t10_key), start[id(c.t10_lists[0])], len(c.t10_lists[0])

    # d: city name rows (with rotations), one city record per row
    rows = []
    for c in cities:
        for text, flags in rotations(c.name, street=False):
            rows.append((text, flags, c.muni, c))
    rows.sort(key=lambda r: (r[0], r[2] or ""))
    d_trie = trie([r[0] for r in rows])
    d_groups, d_leaf = [], {}
    for leaf in leaves(d_trie):
        d_leaf[id(leaf)] = len(d_groups)
        d_groups.append(rows[leaf.first:leaf.first + leaf.count])
    d_units = pack(d_groups, 0x0C)
    d_place, ui, used = [], 0, 0
    for g in d_groups:
        if used + len(g) > len(d_units[ui].rows):
            ui, used = ui + 1, 0
        d_place.append((ui, used))
        used += len(g)
    d_keys = [("d", i) for i in range(len(d_units))]

    def d_place_of(leaf):
        ui_, row = d_place[d_leaf[id(leaf)]]
        return db.resolve(d_keys[ui_]), 0x30 + 8 * row, leaf.count

    def city_record(c, slot3):
        root_ptr, root_off, root_n = roots_of(c)
        parcel, idx = c.seg
        return struct.pack(">IHHHHIH6x", root_ptr, root_off, root_n, slot3, 0, parcel_ptr(parcel),
                           parcel.seg_base + 32 * idx)

    def name_units(units, keys, kind, record):
        out = []
        for i, u in enumerate(units):
            def build(resolve, i=i, u=u):
                nxt = resolve(keys[i + 1]) if i + 1 < len(units) else 0
                prv = resolve(keys[i - 1]) if i else 0
                return name_unit_bytes(u, resolve(keys[i]), nxt, prv, parcel_ptr, record(u))
            out.append(Unit(keys[i], build))
        return out

    def d_record(u):
        n = len(u.rows)
        o_text = _a4(0x30 + 8 * n) + 24 * n
        return lambda c: city_record(c, o_text)

    db.add(Table(0x0F, name_units(f_units, f_keys, 0x0E, lambda u: None)))
    db.add(Table(0x10, t10_units))
    db.add(Table(0x0D, name_units(d_units, d_keys, 0x0C, d_record)))
    e_lists = node_lists(d_trie)
    db.add(Table(0x0E, [Unit(("e", 0), lambda resolve: trie_unit_bytes(resolve(("e", 0)), 0x0D, e_lists, d_place_of)[0])]))

    # blocks 3 (country trie) and 4 (countries)
    name, number, iso = country

    def block4(resolve):
        _, start = trie_unit_bytes(0, 0x0D, e_lists, lambda nd: (0, 0, 0))
        d = bytearray(BLOCK)
        rec, txt = 0x38, 0x38 + 56
        struct.pack_into(">IHHHHHHHHHHHH", d, 0, 4 << 8 | 1, 0x0A, 0, 0x30, 1, rec, 1, 0, 0, txt, 0, 0, 0)
        struct.pack_into(">HHHH", d, 0x30, txt, LANG, 0, rec)
        r = bytearray(template_country)
        struct.pack_into(">IHH", r, 0, resolve(("e", 0)), start[id(e_lists[0])], len(e_lists[0]))
        struct.pack_into(">HH", r, 24, txt, 0)
        struct.pack_into(">H", r, 40, number)
        r[46:48] = iso.encode()
        d[rec:rec + 56] = r
        d[txt:txt + len(name)] = name.encode("latin1")
        return bytes(d)

    def block3(resolve):
        d = bytearray(BLOCK)
        struct.pack_into(">IHHHH", d, 0, 3 << 8 | 1, 0x0B, 0, 0x0C, 1)
        struct.pack_into(">IBBHHH", d, 0x0C, 4 << 8 | 1, ord(name[0]), 1, 0x30, 1, 0)
        return bytes(d)

    db.blocks[3], db.blocks[4] = block3, block4
    db.country_roots = 1                    # block 0 names the country trie root (block 3, 0x0c, count)
    db.country_numbers = {number}           # block 2: per-country settings of our countries only
