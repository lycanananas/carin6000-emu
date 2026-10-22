#!/usr/bin/env python3
"""Strips a Doom IWAD down to one map, so the engine's tables and cache fit the CARiN's RAM.

usage: wadstrip.py IN.wad OUT.wad [--map E1M1] [--info .../info.c --items .../d_items.c]

Kept: the map's lumps; the textures its sidedefs use (TEXTURE1/PNAMES rewritten, only their
patches kept), its flats; the sprites of everything the map can show (computed from the engine's
own state tables: map things, their states, the missiles they fire, what they drop, the player's
weapons); animation ranges the map touches; the sky; status bar, menu, HUD font and intermission
graphics; PLAYPAL, COLORMAP and the identity lumps. Dropped: other maps, sounds, music, demos.
"""
import argparse
import re
import struct
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
DOOM_SRC = ROOT / "third_party/doomgeneric/doomgeneric"

MAP_LUMPS = ("THINGS", "LINEDEFS", "SIDEDEFS", "VERTEXES", "SEGS", "SSECTORS", "NODES", "SECTORS",
             "REJECT", "BLOCKMAP")
# Whole lumps by exact name or prefix (graphics drawn directly, engine tables).
KEEP_NAMES = {"PLAYPAL", "COLORMAP", "ENDOOM", "FREEDOOM", "TITLEPIC", "HELP1", "HELP2", "CREDIT",
              "INTERPIC", "VICTORY2", "ENDPIC"}
KEEP_PREFIXES = ("ST", "M_", "WI", "AMMNUM", "BRDR_")
# Texture/flat animations (p_spec.c animdefs): if the map uses any member, the whole range stays.
TEXTURE_ANIMS = [("BLODGR1", "BLODGR4"), ("SLADRIP1", "SLADRIP3"), ("BLODRIP1", "BLODRIP4"),
                 ("FIREWALA", "FIREWALL"), ("GSTFONT1", "GSTFONT3"), ("FIRELAVA", "FIRELAV3"),
                 ("FIREMAG1", "FIREMAG3"), ("FIREBLU1", "FIREBLU2"), ("ROCKRED1", "ROCKRED3"),
                 ("BFALL1", "BFALL4"), ("SFALL1", "SFALL4"), ("WFALL1", "WFALL4"), ("DBRAIN1", "DBRAIN4")]
FLAT_ANIMS = [("NUKAGE1", "NUKAGE3"), ("FWATER1", "FWATER4"), ("SWATER1", "SWATER4"),
              ("LAVA1", "LAVA4"), ("BLOOD1", "BLOOD3"), ("RROCK05", "RROCK08"), ("SLIME01", "SLIME04"),
              ("SLIME05", "SLIME08"), ("SLIME09", "SLIME12")]
# Mobjs spawned by code rather than by state fields: action function -> spawned types.
ACTION_SPAWNS = {
    "A_TroopAttack": ["MT_TROOPSHOT"], "A_HeadAttack": ["MT_HEADSHOT"], "A_BruisAttack": ["MT_BRUISERSHOT"],
    "A_CyberAttack": ["MT_ROCKET"], "A_FatAttack1": ["MT_FATSHOT"], "A_FatAttack2": ["MT_FATSHOT"],
    "A_FatAttack3": ["MT_FATSHOT"], "A_SkelMissile": ["MT_TRACER"], "A_Tracer": ["MT_SMOKE"],
    "A_BspiAttack": ["MT_ARACHPLAZ"], "A_VileTarget": ["MT_FIRE"], "A_PainAttack": ["MT_SKULL"],
    "A_PainDie": ["MT_SKULL"], "A_FireMissile": ["MT_ROCKET"], "A_FirePlasma": ["MT_PLASMA"],
    "A_FireBFG": ["MT_BFG"], "A_BFGSpray": ["MT_EXTRABFG"], "A_BrainSpit": ["MT_SPAWNSHOT"],
    "A_SpawnFly": ["MT_SPAWNFIRE"], "A_BrainScream": ["MT_ROCKET"], "A_BrainExplode": ["MT_ROCKET"],
}
DROPS = {"MT_WOLFSS": "MT_CLIP", "MT_POSSESSED": "MT_CLIP", "MT_SHOTGUY": "MT_SHOTGUN", "MT_CHAINGUY": "MT_CHAINGUN"}
# Always there: the player, hitscan puffs and blood, fogs.
ALWAYS = ["MT_PLAYER", "MT_PUFF", "MT_BLOOD", "MT_TFOG", "MT_IFOG"]
# Weapon pickups -> weapon index in weaponinfo (fist and pistol are always available).
PICKUP_WEAPON = {"MT_SHOTGUN": 2, "MT_CHAINGUN": 3, "MT_MISC27": 4, "MT_MISC28": 5, "MT_MISC25": 6,
                 "MT_MISC26": 7, "MT_SUPERSHOTGUN": 8}
WEAPON_SPAWNS = {4: ["MT_ROCKET"], 5: ["MT_PLASMA"], 6: ["MT_BFG", "MT_EXTRABFG"]}
STATE_FIELDS = ("spawnstate", "seestate", "painstate", "meleestate", "missilestate", "deathstate",
                "xdeathstate", "raisestate")


def read_wad(path):
    data = path.read_bytes()
    ident, count, offset = struct.unpack_from("<4sii", data, 0)
    lumps = []
    for i in range(count):
        pos, size, name = struct.unpack_from("<ii8s", data, offset + 16 * i)
        lumps.append((name.rstrip(b"\0").decode("latin1").upper(), data[pos:pos + size]))
    return ident, lumps


def write_wad(path, ident, lumps):
    body = bytearray(b"\0" * 12)
    directory = bytearray()
    for name, data in lumps:
        directory += struct.pack("<ii8s", len(body), len(data), name.encode("latin1"))
        body += data
    struct.pack_into("<4sii", body, 0, ident, len(lumps), len(body))
    path.write_bytes(bytes(body + directory))


def parse_states(info_c):
    """states[]: name -> (sprite, action, next)."""
    states = {}
    for m in re.finditer(r"\{SPR_(\w+),\s*[^,]+,\s*[^,]+,\s*\{(\w+)\},\s*(\w+),[^}]*\},\s*//\s*(\w+)", info_c):
        sprite, action, nxt, name = m.groups()
        states[name] = (sprite, action, nxt)
    return states


def parse_mobjs(info_c):
    """mobjinfo[]: MT name -> {field: value}."""
    mobjs = {}
    for m in re.finditer(r"\{\s*//\s*(MT_\w+)(.*?)\n\s*\}", info_c, re.S):
        fields = {}
        for v, f in re.findall(r"\n\s*([^\n]*?),?\s*//\s*(\w+)", m.group(2)):
            fields[f] = v.strip().rstrip(",")
        mobjs[m.group(1)] = fields
    return mobjs


def parse_weapons(items_c):
    body = items_c[items_c.index("weaponinfo[NUMWEAPONS]"):]
    weapons = []
    for block in re.findall(r"\{\s*//[^\n]*\n(.*?)\}", body, re.S)[:9]:
        weapons.append([s for s in re.findall(r"\b(S_\w+)", block)])
    return weapons


def sprites_for_map(things, info_c, items_c):
    states = parse_states(info_c)
    mobjs = parse_mobjs(info_c)
    weapons = parse_weapons(items_c)
    by_ednum = {}
    for name, f in mobjs.items():
        try:
            by_ednum[int(f.get("doomednum", "-1"))] = name
        except ValueError:
            pass
    types = set(ALWAYS)
    for t in things:
        if t in by_ednum:
            types.add(by_ednum[t])
    weapon_ids = {0, 1}
    sprites, seen_states, todo = set(), set(), list(types)
    done_types = set()

    def walk(state):
        while state and state != "S_NULL" and state not in seen_states and state in states:
            seen_states.add(state)
            sprite, action, nxt = states[state]
            sprites.add(sprite)
            for spawned in ACTION_SPAWNS.get(action, []):
                todo.append(spawned)
            state = nxt

    while todo:
        t = todo.pop()
        if t in done_types or t not in mobjs:
            continue
        done_types.add(t)
        if t in DROPS:
            todo.append(DROPS[t])
        if t in PICKUP_WEAPON:
            weapon_ids.add(PICKUP_WEAPON[t])
        for field in STATE_FIELDS:
            walk(mobjs[t].get(field))
    for w in sorted(weapon_ids):
        if w < len(weapons):
            for s in weapons[w]:
                walk(s)
        for spawned in WEAPON_SPAWNS.get(w, []):
            todo.append(spawned)
    while todo:   # missiles of the weapons
        t = todo.pop()
        if t in done_types or t not in mobjs:
            continue
        done_types.add(t)
        for field in STATE_FIELDS:
            walk(mobjs[t].get(field))
    return sprites


def textures_by_name(texture1):
    count = struct.unpack_from("<i", texture1, 0)[0]
    offsets = struct.unpack_from(f"<{count}i", texture1, 4)
    result = []
    for off in offsets:
        name = texture1[off:off + 8].rstrip(b"\0").decode("latin1").upper()
        patchcount = struct.unpack_from("<h", texture1, off + 20)[0]
        result.append((name, texture1[off:off + 22 + 10 * patchcount]))
    return result


def anim_range(names, start, end):
    if start in names and end in names:
        a, b = names.index(start), names.index(end)
        return names[min(a, b):max(a, b) + 1]
    return []


def main():
    p = argparse.ArgumentParser()
    p.add_argument("inp", type=Path)
    p.add_argument("out", type=Path)
    p.add_argument("--map", default="E1M1")
    p.add_argument("--info", type=Path, default=DOOM_SRC / "info.c")
    p.add_argument("--items", type=Path, default=DOOM_SRC / "d_items.c")
    a = p.parse_args()

    ident, lumps = read_wad(a.inp)
    names = [n for n, _ in lumps]
    lump = {n: d for n, d in reversed(lumps)}   # first occurrence wins
    mi = names.index(a.map.upper())
    map_lumps = [(a.map.upper(), b"")] + lumps[mi + 1:mi + 1 + len(MAP_LUMPS)]
    md = {n: d for n, d in map_lumps}

    # Textures and flats the map uses.
    used_tex, used_flats = set(), set()
    sd = md["SIDEDEFS"]
    for i in range(0, len(sd), 30):
        for k in (4, 12, 20):
            used_tex.add(sd[i + k:i + k + 8].rstrip(b"\0").decode("latin1").upper())
    sec = md["SECTORS"]
    for i in range(0, len(sec), 26):
        used_flats.add(sec[i + 4:i + 12].rstrip(b"\0").decode("latin1").upper())
        used_flats.add(sec[i + 12:i + 20].rstrip(b"\0").decode("latin1").upper())
    used_tex.discard("-")
    used_tex.add("SKY1")
    used_flats.add("F_SKY1")
    used_flats.add("FLOOR7_2")   # view border (R_FillBackScreen)

    textures = textures_by_name(lump["TEXTURE1"])
    if "TEXTURE2" in lump:   # Freedoom (like registered Doom) has a second list; merged into one
        textures += textures_by_name(lump["TEXTURE2"])
    tex_names = [n for n, _ in textures]
    for s, e in TEXTURE_ANIMS:
        r = anim_range(tex_names, s, e)
        if used_tex & set(r):
            used_tex |= set(r)
    flat_lumps = lumps[names.index("F_START") + 1:names.index("F_END")]
    flat_names = [n for n, d in flat_lumps if len(d) == 4096]
    for s, e in FLAT_ANIMS:
        r = anim_range(flat_names, s, e)
        if used_flats & set(r):
            used_flats |= set(r)

    # TEXTURE1/PNAMES with only the kept textures (index 0 stays: it means "no texture").
    pn = lump["PNAMES"]
    pcount = struct.unpack_from("<i", pn, 0)[0]
    pnames = [pn[4 + 8 * i:12 + 8 * i].rstrip(b"\0").decode("latin1").upper() for i in range(pcount)]
    kept = [t for i, t in enumerate(textures) if i == 0 or t[0] in used_tex]
    new_pnames, remap = [], {}
    rebuilt = []
    for name, raw in kept:
        raw = bytearray(raw)
        for k in range(struct.unpack_from("<h", raw, 20)[0]):
            old = struct.unpack_from("<h", raw, 22 + 10 * k + 4)[0]
            if old not in remap:
                remap[old] = len(new_pnames)
                new_pnames.append(pnames[old])
            struct.pack_into("<h", raw, 22 + 10 * k + 4, remap[old])
        rebuilt.append(bytes(raw))
    head = 4 + 4 * len(rebuilt)
    offs, pos = [], head
    for r in rebuilt:
        offs.append(pos)
        pos += len(r)
    texture1 = struct.pack(f"<i{len(rebuilt)}i", len(rebuilt), *offs) + b"".join(rebuilt)
    pnames_lump = struct.pack("<i", len(new_pnames)) + b"".join(n.encode().ljust(8, b"\0") for n in new_pnames)

    things = md["THINGS"]
    thing_types = {struct.unpack_from("<h", things, i + 6)[0] for i in range(0, len(things), 10)}
    sprites = sprites_for_map(thing_types, (a.info).read_text(), (a.items).read_text())

    out = []
    for n, d in lumps:
        if n in KEEP_NAMES or n.startswith(KEEP_PREFIXES):
            if n not in ("STEP1", "STEP2", "STARTAN1"):   # ST* also matches some flats/textures
                out.append((n, d))
    out.append(("TEXTURE1", texture1))
    out.append(("PNAMES", pnames_lump))
    out += map_lumps
    out.append(("P_START", b""))
    seen = set()
    for n, d in lumps:
        if n in new_pnames and n not in seen and d:
            out.append((n, d))
            seen.add(n)
    missing = set(new_pnames) - seen
    out.append(("P_END", b""))
    out.append(("F_START", b""))
    out += [(n, d) for n, d in flat_lumps if n in used_flats]
    out.append(("F_END", b""))
    out.append(("S_START", b""))
    sprite_lumps = lumps[names.index("S_START") + 1:names.index("S_END")]
    out += [(n, d) for n, d in sprite_lumps if n[:4] in sprites]
    out.append(("S_END", b""))
    write_wad(a.out, ident, out)

    size = a.out.stat().st_size
    print(f"{a.out}: {len(out)} lumps, {size // 1024} KB ({a.map}: {len(kept)} textures, "
          f"{len(new_pnames)} patches, {len(used_flats & set(flat_names))} flats, {len(sprites)} sprites)")
    if missing:
        print("warning: patches not found:", " ".join(sorted(missing)))
    print("sprites:", " ".join(sorted(sprites)))


if __name__ == "__main__":
    main()
