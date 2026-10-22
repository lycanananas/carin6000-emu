# CARiN map database (`carindb`) — format reference

State: 2026-10-22. Decoder: `navdb/tools/carindb2gpkg.py` (+ `parcel.py`, `gpkg.py`, `qgis_style.py`).
Source disc: `NAV_DB` (ISO9660, MapScape / Continental, CD 21435, 2013-04-08), Eastern Europe
(PL, CZ, SK, HU, SI, HR, BG, LT, LV), built by `dbconv` from GDF 2.1 (Nokia/Navteq 2012 Q4),
db release 34, "reduced_hnr.cpr".

Everything here comes from the file and from the OS-9 modules that read it (flash 0209: `pbp`,
`dbq`, `dbc`, `dbd`, `dbpa`), by static analysis (Ghidra, see the end). No emulator traces.
**Confirmed** = seen in code (function given); **checked** = verified on decoded data; **seen** =
pattern in the data only.

## 1. Conventions

- Big-endian everywhere.
- **Coordinates** (GPS units): 1 unit = 1.8e-7 degree (1e-9 semicircles). Latitude = v × 1.8e-7;
  longitude = v × 1.8e-7 − 30 (stored with +30°, so all values are positive). The GPS receiver
  record and the map use the same units (`carin-gps-dr-chain` memory).
- **Blocks**: 2048 bytes. **Units**: 1..8 consecutive blocks (max 16 KB). A **unit pointer** is a
  u32: upper 24 bits = first block, low byte = length in blocks. Every unit starts with its own
  pointer (checked on all 49 031 parcels).
- **Point steps**: positions inside parcels / POI units / background units are u16 steps of
  **64 GPS units** (≈ 1 m north-south) from the unit box corner (checked: the maximum coordinate is
  box width / 64 for every parcel size).
- **Text**: lower case Latin-1, 0-terminated, in a text area of the unit; referenced by u16 offsets
  (0 = none). Names keep the language's letters (ä, ö, …) but are mostly ASCII-folded ("zlota").
- **Record sizes**: 93 record types with fixed sizes, from the block 0 table (`RECSIZE[type]`).
  Both `dbq` (globals 0x0ef8 + 2·type, `FUN_00014dc4` defaults, `FUN_00014f60` load) and `pbp`
  (0x0cf8 + 2·type, `FUN_0000325c` defaults) load it for types < 0x4e (**confirmed**).
- Success code of the db functions: 499 (0x1f3).

## 2. File layout

### Block 0 (header)

| Offset | Value | Meaning |
|---|---|---|
| 0x02 | 0x0001 | low byte: CD number (**confirmed** pbp) |
| 0x08 | 0x0060 | offset of a section at 0x60 (seen) |
| 0x1a | 34 | db release (**confirmed** pbp: warns unless 14..21; `FUN_000034c0` returns it to the unpackers) |
| 0x24 / 0x26 | 0x68 / 31 | offset / count of the table id list (seen) |
| 0x28 / 0x2a | 0xa6 / 93 | offset / count of the record size table, (u16 type, u16 size) pairs (**confirmed**) |
| 0x2c | 0x0800 | block size (seen) |
| 0x34 | 4 × u32 | bounding box min lon, min lat, max lon, max lat (checked: equals the extent of all decoded roads) |
| 0x50 | 4 × u32 | the same box again, last value differs |
| 0x220 | 16-byte entries | table directory (start = end of the size table padded to 16; seen) |
| 0x838 | text | "no label", "no description", the conversion report |
| 0x13b0, 0x25fc | text | country names |

Block 1 has a similar header with a filler box (111111111, 222222222, …).

### Table directory

Entry: `u32 a, u32 b, u32 c, u16 id, u16 0`, unit pointers. `a` = index part start (0 = none),
`b` = data start, `c` = end. Tables lie back to back. Unit kind = word +4 of each unit.

| Table | Data blocks | Index | Share | Kind | Compr. | Content | Decoded |
|---|---|---|---|---|---|---|---|
| 1 | 23fab–45871 | 22a67 | 38.4 % | 0 | 97 % | routing level 0 = full road network (parcels) | yes |
| 1e | 4eed8–5092d | 4ee78 | 1.9 % | 3 | no | routing level 1 | yes |
| 3 | 5095d–5118b | 5092e | 0.6 % | 2 | no | routing level 2 | yes |
| 2 | 51198–51549 | 5118c | 0.3 % | 1 | no | routing level 3 | yes |
| 6 | 515a4–55d72 | 5154a | 5.1 % | 0x16 | 90 % | background, fine | yes |
| 16 | 55dad–56d67 | 55d73 | 1.1 % | 0x15 | 85 % | background, medium | yes |
| 1c | 570ec–571a7 | 570e6 | 0.1 % | 0x14 | 53 % | background, coarse (wider Europe) | yes |
| 7 | 1fee4–22a61 | 1f990 | 3.1 % | 6 | no | POI | yes |
| 11 | 15a54–1f98e | – | 11.4 % | 0x10 | no | POI names (+ details) | names only |
| f | 232d–14d84 | – | 21.3 % | 0x0e | no | street name search index | no |
| d | 24–17e9 | – | 1.7 % | 0x0c | no | city names (search) | partly |
| e | 5–1d | – | – | 0x0d | no | alphabetic index into d | partly |
| 10 | 17ee–2326 | – | 0.8 % | 0x0f | no | index (from d) | no |
| 12 | 14d8c–15a4c | – | 0.9 % | 0x11 | no | index | no |
| 18 | 5728f–576f7 | – | 0.3 % | 0x17 | no | names (Czech, streets?) | no |
| 15 | 56da5–570e5 | 56d68 | 0.2 % | 0x1c | 69 % | ? | no |
| 14 | 571ab–5725e | 571a8 | – | 0x1d | 41 % | ? | no |
| 1d | 57261–5728d | 5725f | – | 0x1e | – | ? | no |
| 0, 4, a, b, c, 13, 17 | single units | – | – | various | no | small header-like tables | no |

`CARINET` (separate file) = TMC event texts; not needed (no TMC in use).

## 3. Unit unpacking (pbp, ported in `parcel.py`)

`pbp` reads the blocks of a unit (`lseek` block·2048, `read` length·2048, up to 5 tries) and, when
byte +6 bit 0 is set, `FUN_00003508` expands it into a buffer of (byte +7) blocks; uncompressed
units are copied. Kind (word +4): 0 → `FUN_00003eb2` (parcel), 0x14..0x16 → `FUN_00004840`
(background), 0x0e → `FUN_000043a8` (not ported; table f is not compressed anyway), other kinds
are zero-filled. Word +6 is cleared afterwards.

### Bit reader (`FUN_00004c16` and the fixed-width wrappers 0x4c2e.. 0x4caa)

MSB-first from a base pointer (`bfextu (A0){pos:w}`), position in bits. `FUN_00004c16(w)` adds w
to the position, then extracts: **w = 0 extracts 32 bits and does not move** (68k bitfield width 0
= 32). The background units rely on it. `N` = `bits_for(size of the unpacked unit)`;
`bits_for(v)` (`FUN_00004cc2`) = number of bits for 0..v−1 (1 for v = 1, 16-bit arithmetic).

### Text code (`FUN_00004abc`)

Read `first`, `last` (N bits each; both 0 = no text), then 6 phrases (5-bit length, 7-bit bytes).
For each byte from `first` to `last`: 2-bit code; 0 → 1 bit into `ae`; 1 → 2 bits into
`str\0`; 2 → 3 bits into ` dghilno`; 3 → 7 bits v: v > 0x26 literal, v ≤ 0x1b index into
`e0 e1 e2 e3 e4 e5 e7 e8 e9 ea eb ec ed ee ef f1 f2 f3 f4 f5 f6 f8 f9 fa fb fc fd ac`,
0x21..0x26 phrase v − 0x21 (copied, the position advances by its length). Tables from pbp data
offsets 0x1342.. (its M$IData). Checked: the Warsaw parcel gives the real street names.

### Delta-coded points

First record: x = 16 bits, y = 16 bits. Next ones, per axis: `0` → prev + d, `10` → prev − d,
`11` → 16 bits absolute; d has a per-unit width (parcels: global 0x0d99, background: 0x0dd0).

## 4. Parcels (tables 1, 1e, 3, 2)

Unit: 8-byte header (type 05: own pointer, kind, +6 compression flag, +7 unpacked blocks), then
the 60-byte parcel header (type 11) = 15 slots `(u16 offset, u16 count)` at 8 + 4k (offsets
into the unpacked unit), box (4 × u32) at 0x44, word 0x58 = **unit pointer of the parcel one
routing level up** (table 1 → 1e → 3 → 2; 0 at the top). The first 116 bytes (type 0x0b) are
never compressed.

### Unpacking (`FUN_00003eb2`)

1. copy 116 bytes; widths: W51 = bits_for(count slot 2), W53 = bits_for(count slot 4 + 1),
   W55 = slot 7 + 1, W59 = slot 10 + 1, W57 = slot 11 + 1, W5b = slot 12 + 1;
2. 2 raw bytes → globals 0x0d98 (default width of segment +12) and 0x0d99 (point delta width);
3. raw copies: slot 3 (type 0x12, count + 1 records), slot 9 (0x0f), slot 10 (0x14), slot 12
   (0x15, if count ≠ 0);
4. bit stream: slot 0 (`FUN_000035de`, mode 0x14), slots 1, 2 (`FUN_00003692`, `FUN_00003718`),
   slot 4 segments (`FUN_0000379e`, mode 0x14, with a sentinel record after the last), slot 5
   (`FUN_00003abe`), slot 6 (`FUN_00003be4`), slot 7 (`FUN_00003d28`), slot 11 (`FUN_00003e06`,
   if count ≠ 0), text; release > 0x14: W5d from slot 13, second passes over slots 0 and 4
   (mode 0x15) and slot 13 cleared (`FUN_00003e8a`).
   The exact field order of every expander is in `parcel.py` (1:1 with the C).

### Slots

| Slot | Record | Content (checked on Warsaw unless noted) |
|---|---|---|
| 0, 1, 2 | 0x40 (10 B) | **place hierarchy**: +0 name, +2 second name, +4 parent record. Slot 0 country ("polska"), slot 1 city ("warszawa"), slot 2 street in a city/district ("ulica marszalkowska" / "srodmiescie" → warszawa) |
| 3 | 0x12 (4 B) | raw, count + 1 (seen: pairs of offsets into slots 4/5) |
| 4 | 0x08 (32 B), 0x09 (26 B) on levels 1-3 | **road segments**, below |
| 5 | 0x10 (8 B) | **points** inside the parcel: x, y, +4 owning segment (offset), +6 byte, +7 3 bits |
| 6 | 0x06 (16 B) | points on the parcel edge (x or y = 0 or max), + u32 and 14-bit fields (link to the neighbour parcel?) |
| 7 | 0x0c (6 B) | points: x, y, 3 bits; referenced by segment +4 |
| 9, 10 | 0x0f, 0x14 (8 B) | **neighbour parcel links**: u32 unit pointer (e.g. `03e19403`), u16 offset there, u16 |
| 11, 12 | 0x13, 0x15 (6 B) | referenced by segment +30 / +20; meaning open |
| 13 | – | cleared after release > 0x14 pass |
| 14 | – | text area |

### Segment record (type 0x08; type 0x09 = the first 26 bytes)

| Offset | Content | Check |
|---|---|---|
| +0, +2 | start / end point (offsets of point records in slot 5, 6 or 7) | the network draws correctly everywhere |
| +4 | slot 7 record (state kept across records) | |
| +6, +8 | other segments (0 = none) | |
| +10 | ? (139, 130, 133 most common) | |
| +11 bits 4-5 | **one-way**: 0 both, 1 start → end only, 2 end → start only, 3 closed | dual carriageways split 1/2 evenly; with right-hand traffic the opposite carriageway lies left of the allowed direction (97 %); 3 ⇔ kind 5 |
| +11 bits 0-3 | ? (12 most common) | |
| +12 | **length in metres** | geometric length / value = 1.00 (median, all classes) |
| +14, +15 | **headings** at the ends, 256 steps per turn | +15 = +14 + 128 on straight segments |
| +16 bits 0-3 | **road class (FRC)** 0 motorway … 6 minor | Warsaw: S8, Wisłostrada = 0, arteries 1-2, collectors 3, local 4-6; routing level 1 has classes 0-2, level 2 0-1, level 3 0 |
| +16 bits 4-6 | **kind**: 5 = not routable (dbq `FUN_00001890` skips it) | |
| +17 | ? (32 most common) | |
| +18 / +20 / +30 | slot 10 / 12 / 11 records | |
| +26 | **slot 2 record**: street name, district, then city and country through the parents (type 08 only) | |

Counts: level 0 49 031 parcels / 8 613 555 segments; level 1 3 278 / 190 263; level 2 1 056 /
70 107; level 3 715 / 22 228.

## 5. Background (tables 6, 16, 1c; `FUN_00004840`, `FUN_000044a2`)

Unit: 8-byte header, slots 0-4 at 8 + 4k (a 0x3d = 52-byte header, copied raw), box at **0x20**.
Unpacking: copy 52 bytes; Wca = bits_for(count slot 3 + 1); 4 raw bytes → globals 0x0dce,
0x0dcf, 0x0dd0, 0x0dd1 (widths); bit stream with the generic expander `FUN_000044a2(slot, size,
code, mode)` for slot 0 (code 0x25), 1 (0x26), 2 (0x27), 3 (0x0b wide / 0x0a narrow), text;
release > 0x13: 5 bytes → 0x0dd2..0x0dd6, slot 4 (code 0x28, mode 0x14), second passes over slots
1 and 2. Codes 0x25-0x27 handle count + 1 records (sentinel).

| Slot | Record | Content |
|---|---|---|
| 0 | 0x3b (4 B) | groups of slot 1 records by **area type**: 7-bit type, 1 bit, offset of the first record of the group |
| 1 | 0x3a (20 B) | **areas**: +0 name, +2 first point (pointer into slot 3), +4 **area in m²** (Las Kabacki 9.0 km² = real), +8/+12 label position (GPS units, Warsaw → 21.01 E 52.23 N), +16/+18 second pass |
| 2 | 0x3c (16 B) | **generalised roads**: +0 name = road number ("8", "61", "631"), +2 first point, +4 attribute, +8/+10 second pass |
| 3 | 8 B (wide) / 4 B | **points**: wide = absolute u32 lon, u32 lat; narrow = 16-bit x/y deltas (steps of 64 from the box). Wide when word (8 + 0x10 + RECSIZE[0x3f]) is 0 |
| 4 | 0x15 (6 B) | three u16 (second pass); meaning open |

A record's shape = points from its +2 pointer to the next record's +2 pointer. Area types: 1
water (seas, lakes, rivers), 3 forest / park, 5 built-up, 6 cemetery, 8 island. Sea polygons are
cut along unit borders (straight lines in the sea). Counts: table 6 253 656 areas / 110 019 roads,
16 47 739 / 101 277, 1c 1 072 / 3 756.

## 6. POI (table 7, kind 6, uncompressed) and their names (table 11)

Unit: +8 slot (offset, count) of 28-byte records, +0x10 box. Record: +0 u32 unit pointer into
**table 11** and +4 u16 offset of an 8-byte name entry there (+0 = text offset of the name in that
unit); +6/+8 x/y (steps of 64 from the box); +10 **category**; +12 **brand** text offset in this
unit ("lukoil", "pko bank polski"); +16 u32 (?); rest 0. 698 013 POI, 44 categories (names in
`POI_CATEGORIES`, from the names under each code): 11 car dealer, 12 fuel, 13 car rental, 14
parking, 15 P&R, 20 sight, 21 hotel, 22 restaurant, 23 bank, 24 culture centre, 25 library,
26 court, 29 embassy, 30 ATM, 31 tourist info, 32 museum, 33 theatre, 35 sport, 36 church, 37
cemetery, 38 racing, 39 park, 40 exhibition, 41 hospital, 42 police, 43 town hall, 44 post, 45
clinic, 46 pharmacy, 47 shop, 48 locality, 49 cinema, 50 golf, 51 railway station, 52 airport, 54
bus station, 56 school, 57 nightlife, 59 car service, 61 company; 16, 53, 55, 58 not named yet.

Table 11 units (kind 0x10) have the layout of table d: 8-byte name entries (+0 name, +2 0x00ff,
+4 0, +6 offset of a detail record).

## 7. Search indexes (seen, not decoded)

Table d unit (kind 0x0c): slot 0 at 0x30 = 8-byte entries (name, 0x00ff, second name =
municipality, offset of a slot 1 record); slot 1 = 24-byte **city records**: +0 unit pointer into
table 10, +4 offset, +6 count; +8 offset of a slot 2 run here, +10 count; +12 unit pointer into
**table 1** (the parcel), +16 offset of the city's place record (slot 1) in that parcel. Slot 2 =
8-byte (table 1 unit pointer, offset, count); slot 4 = 12-byte (table 11 unit pointer, offset,
count, name offset) = the city's POI. Table e = alphabetic index into d (12-byte entries: unit
pointer, first letter + flag, offset, count). Table 10 (kind 0x0f) = 12-byte entries pointing into
d. Table f (kind 0x0e, 21 %) holds sorted street names per city ("… gatve", "ulica …"); its
records still to be read (unpacker `FUN_000043a8` exists but the table is not compressed).

## 8. For the encoder

What a writer must produce, by what we know:
1. Block 0: header fields above, record size table (copy), table directory, texts.
2. Parcels for every routing level with a consistent quadtree of boxes (64-unit grid, sizes
   32 768 · 2^k units), parent pointers (0x58), neighbour links (slots 9/10) and edge points
   (slot 6) where roads cross parcel borders, place hierarchy (slots 0-2), segments with their
   attributes, points, text; then the bit-level compression (inverse of section 3; storing units
   uncompressed is also allowed: 1 240 parcels are).
3. Background units (areas, generalised roads), POI units + table 11 names, search indexes
   (d, e, 10, 12, f, 18) and the index parts of the tables.
4. Verification: decode the output with `carindb2gpkg.py` and compare; then run it in the emulator
   (the map draws when the car position is set manually).

Open items for a complete decoder: search indexes (7), index parts of tables 1/1e/3/2/6/16/1c/7
(seen: lists of unit pointers with keys), segment bytes +10, +11 low, +17, slots 3, 7, 11, 12,
POI +16, the small tables, the `+0x54..+0x74` area of parcels (pointers to neighbouring parcels
and a 32-byte record).

## 9. Tools and workflow

- `tools/extract_navdb.py`: disc image → `temp/navdb/` (carindb, CARINET, …).
- `tools/carindb_header.py`: block 0 dump.
- `tools/carindb2gpkg.py [carindb] [out.gpkg] [--bbox lon0,lat0,lon1,lat1]`: layers `parcels`,
  `roads`, `areas`, `overview_roads`, `poi`; RTree index; QGIS styles in `layer_styles`.
  Whole database ≈ 15 min, 2.4 GB.
- Ghidra (headless, Java script): `analyzeHeadless temp/navdb/ghidra navdb_db -import
  temp/navdb/mods/*.mod -processor 68000:BE:32:MC68030 -postScript ExportOS9Modules.java
  $PWD/temp/navdb/ghidra_out -scriptPath $PWD/tools/ghidra -overwrite`, then
  `tools/ghidra/resolve_a6.py MODULE.mod EXPORT.md OUT.md` (A6 jump table and globals) and
  `navdb/tools/annotate_dbq.py IN OUT INDEX --base 0x0ef8|0x0cf8` (RECSIZE names for dbq|pbp).
  The script creates functions from `link a5` prologues and sets the Microware calling convention
  (D0, D1, then stack). Module files: `re-safrane/extracted/flash_0209/*.mod`; copies in
  `temp/navdb/mods/`. Do not use `export_os9_modules.py` there: it deletes
  `temp/ghidra/OS9_Batch_Project`.
- Key functions: pbp `FUN_00003508` dispatch, `FUN_00003eb2` parcel, `FUN_000035de`..`FUN_00003e8a`
  parcel slots, `FUN_00004840`/`FUN_000044a2` background, `FUN_00004abc` text, `FUN_00004c16` bit
  reader, `FUN_000049f2` setup, `FUN_00004a34`/`FUN_00004a5a` raw copies, `FUN_0000325c` record
  sizes, read loop at 0x11fc; dbq `FUN_000128d4` get unit (via pbp), `FUN_00012834` release,
  `FUN_00001890` segment walk, `FUN_00014dc4`/`FUN_00014f60` record sizes, `FUN_0000c5fa`/`c74c`
  record search by letter.
