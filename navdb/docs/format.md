# CARiN map database (`carindb`) — what is known

Disc `NAV_DB` (ISO9660, MapScape / Continental, CD 21435, 2013-04-08): `carindb` (733 470 720 bytes, the database, "eeu_nok_nok_12Q4 … reduced_hnr.cpr"), `CARINET` (757 760 bytes, TMC event texts `carinet16s2048`, not the road network), `ABSTRACT`, `BIBLIOGR`, `COPYRIGH`. Built by `dbconv` from GDF 2.1 (Nokia/Navteq 2012 Q4), db release 34, with RDS/TMC.

Everything below comes from the file itself and the OS-9 modules that read it (static analysis, flash 0209). Big-endian throughout. **Confirmed** = seen in code; **seen** = only from the data, still a hypothesis.

## Reading path in OS-9

| Module | Role |
|---|---|
| `dbd` | detects the disc (`NAV_DB`, file `CARINDB`), reads `DB-REL`, `CD-ID` |
| `pbp` | block server: reads 2 KB blocks from `/cd`, cache (buddy allocator, "CL" list), checks block 0 |
| `dbc` | request dispatcher (pipes `dbc_request_pipe`, `dbq_req/rsp`, `pbp_rsp`) |
| `dbq` | queries: decodes records (no debug strings; to be read from the code) |
| `dbpa` | client library (used by map_manager, rpmod, …), also reads `/pack/position` |

## Blocks

2048-byte blocks (block 0 offset 0x2c; 358 140 blocks). Data is read in **units** of 1..8 consecutive blocks (up to 16 KB). A pointer is 32-bit: upper 24 bits = first block, low byte = unit length in blocks. **Seen** twice over:
- every unit starts with its own pointer (block 0x22a67 starts `022a 6708`, 0x23fab `023f ab01`, 0x232d `0023 2d06`, block 5 `0000 0508`);
- table `e` (block 5, 8 blocks) is a list of pointers to the units of table `d` (`0000 2406`, `0000 6c06`, `0000 8108`, …), each with a key and a count.

## Block 0 (header)

| Offset | Value here | Meaning |
|---|---|---|
| 0x02 | 0x0001 | low byte: CD number (**confirmed**, pbp compares it) |
| 0x08 | 0x0060 | offset of a section (at 0x60) — **seen** |
| 0x1a | 0x0022 = 34 | db release number (**confirmed**: pbp warns unless 14..21; matches the text "db release number: 34") |
| 0x24 / 0x26 | 0x68 / 31 | offset / count of the table id list (ids 0..0x1e) — **seen** |
| 0x28 / 0x2a | 0xa6 / 93 | offset / count of the record type table — **seen** |
| 0x2c | 0x0800 | block size — **seen** |
| 0x34 | 4 longs | bounding box in GPS units: 1.8e-7°, longitude +30° (first corner 12.108°E, 41.265°N = west of Czechia, south of Bulgaria) — **seen** |
| 0x50 | 4 longs | the same box again, last value differs — **seen** |
| 0xa6 | 93 × (u16 type, u16 size) | record types and their fixed sizes in bytes (types 0x01..0x5a, 0x8a, 0x97, 0x9d) — **seen** |
| 0x220 | 16-byte entries | table directory (start padded to 16 bytes — not confirmed) |
| 0x838 | text | "no label", "no description", the conversion report (name, content, oem, conv tool, date, release, GDF 2.1, rds/tmc) |
| 0x13b0, 0x25fc | text | country names (ceska republika, balgaria, hrvatska, latvija, lietuva, magyarorszag, polska, slovenija, slovenska republika) |

Block 1 (0x800) starts with a similar header whose box is filler (111111111, 222222222, 333333333, 444444444).

## Table directory (0x220)

Entry: `u32 a, u32 b, u32 c, u16 id, u16 0`. The tables lie one after another without overlap: **seen** `a` = index start (0 when there is none), `b` = data start, `c` = end.

| id | a (block) | b | c | size (blocks) |
|---|---|---|---|---|
| 1 | 22a67 | 23fab | 45871 | ~143 900 (largest: geometry?) |
| 2 | 5118c | 51198 | 51549 | |
| 3 | 5092e | 5095d | 5118b | |
| 1e | 4ee78 | 4eed8 | 5092d | |
| 1d | 5725f | 57261 | 5728d | |
| 14 | 571a8 | 571ab | 5725e | |
| 1c | 570e6 | 570ec | 571a7 | |
| 15 | 56d68 | 56da5 | 570e5 | |
| 16 | 55d73 | 55dad | 56d67 | |
| 6 | 5154a | 515a4 | 55d72 | |
| 7 | 1f990 | 1fee4 | 22a61 | |
| 4, b, c, 13, 17 | – | 2, 4, 3, 0, 1 | same | single header blocks |
| d | – | 24 | 17e9 | ~6 100 |
| e | – | 5 | 1d | |
| f | – | 232d | 14d84 | ~76 400 |
| 10 | – | 17ee | 2326 | |
| 11 | – | 15a54 | 1f98e | ~40 600 |
| 12 | – | 14d8c | 15a4c | |
| 18 | – | 5728f | 576f7 | |
| 0 | – | 5728e | 5728e | |

`tools/carindb_header.py` prints all of this.

## Units (seen)

A unit starts with its own pointer, then a word (0x000c in the name units; a type?), a zero word, then **slots** `(u16 offset, u16 count)`, then the data. Each slot holds the records of one kind.

Table `d` (names, alphabetical, the `e` index points at its units by first letter):
- slot 1 at 0x30: 0x65 records of **8 bytes** ((0x358 − 0x30) / 0x65), e.g. `2389 00ff 2393 0358`: offset of the name, 0x00ff, offset of a second name (municipality), offset of the slot 2 record;
- slot 2 at 0x358: 0x65 records of **24 bytes** ((0xcd0 − 0x358) / 0x65);
- slots 4 (0xcd0, 0x18e) and 6 (0x1f84, 0x55): probably the name text. Names are lower case latin: "ablanitsa", "hadzhidimovo", POI brands "dsk bank", "lukoil", "hertz".

Table 1 units (0x23fab): 27 slots, data from 0x74; offsets do not divide evenly by the counts, so either variable records or another meaning of the count.

Ghidra (headless, `tools/ghidra/ExportOS9Modules.java`, project `temp/navdb/ghidra/`, output `temp/navdb/ghidra_out/`): dbq 621 functions, dbpa 229, pbp 285, dbc 184, dbd 146, all decompiled.

## Record sizes in dbq (confirmed)

dbq keeps the record size per type in a global array (data offset 0x0ef8 + 2 × type): `FUN_00014dc4` sets defaults for old releases, `FUN_00014f60` overwrites them from the block 0 table (offsets 0x28/0x2a) when the release is above 0x11, for types below 0x4e. Code walks records with these sizes (`RECSIZE_xx` in `navdb/tools/annotate_dbq.py` output). Success code of the db functions: 499 (0x1f3).

## Table 1: map parcels (confirmed by code and data)

49 031 units from block 0x23fab to 0x45871, back to back (1..7 blocks each), no gaps. Unit layout (dbq `FUN_00001890`):

| Offset | Record | Content |
|---|---|---|
| 0x00 | type 05 (8 B) | unit header: own pointer, word +4 (0 here: the segment records are type 08 of 32 B, otherwise type 09 of 26 B), word +6 |
| 0x08 | type 11 (60 B) | parcel header: (u16 offset, u16 count) pairs into the unit, e.g. +0x10/+0x12 = the segment records (type 08/09; field +0x10 bits 4-6 = a class, 5 is skipped there) |
| 0x44 | (box) | parcel bounding box: min lon, min lat, max lon, max lat (u32, GPS units), aligned to a grid; parcels get smaller where the map is dense (cities) |

Rendering all boxes gives the outline of the covered countries (Poland, Czechia, Slovakia, Hungary, Slovenia, Croatia, Bulgaria, Lithuania, Latvia) with ferry strips across the Baltic. Layer `parcels` in `navdb/tools/carindb2gpkg.py`.

Blocks 0x22a67..0x23fab (`a` of table 1) are its index; not decoded yet.

### Compression (confirmed: pbp, ported in `navdb/tools/parcel.py`)

Unit byte +6 bit 0 set = compressed (47 791 of 49 031 parcels), byte +7 = blocks after unpacking. pbp reads the blocks as they are (`lseek`/`read`, no decoding there) and `FUN_00003508` expands them into a buffer of +7 blocks, by unit kind (word +4): 0 = parcel `FUN_00003eb2`, 0x0e `FUN_000043a8`, 0x14..0x16 `FUN_00004840` (not ported yet). Uncompressed units are copied.

Parcel expansion (`FUN_00003eb2`):
1. copy the first 116 bytes (record type 0x0b: unit header, parcel header, box, …) as they are;
2. bit widths from the header counts (`FUN_00004cc2` = bits for 0..v-1);
3. 2 raw bytes (globals 0x0d98/0x0d99: default width of a segment field, width of the point deltas);
4. raw copies of slots 3 (type 0x12, count+1), 9 (0x0f), 10 (0x14), 12 (0x15, if any);
5. from here an MSB-first bit stream (68k `bfextu`; width 0 = 32): slot 0, 1, 2 (type 0x40), 4 (segments, 0x08), 5 (points, 0x10), 6 (points, 0x06), 7 (points, 0x0c), 11 (0x13), then the text area; with release > 0x14 a second pass over slots 0 and 4.

Point coordinates: the first record absolute (16 + 16 bits), then per axis `0` = previous + d, `10` = previous − d, `11` = absolute 16 bits, d with the 0x0d99 width.

Text (`FUN_00004abc`): the byte range [first, last] (N-bit offsets) is coded per byte with a 2-bit code: 0 → 1 bit into "ae", 1 → 2 bits into "str\0", 2 → 3 bits into " dghilno", 3 → 7 bits: ≤ 0x1b index into 28 accented letters (Latin-1 0xe0..0xfd, 0xac), 0x21..0x26 one of 6 phrases (5-bit length + 7-bit bytes, read first), otherwise the byte itself. The tables come from pbp's initialised data. Check: the Warsaw parcel 0x3e1b0 decodes to "aleje jerozolimskie", "ulica marszalkowska", "plac defilad", "srodmiescie", "warszawa" in 12 languages.

### Inside a parcel (after unpacking)

Slots of the parcel header (pair k at 8 + 4k; offsets are into the unpacked buffer):

| Slot | Record | Content |
|---|---|---|
| 0, 1, 2 | 0x40 (10 B) | **place hierarchy**: +0 name, +2 second name (text offsets, 0 = none), +4 parent record. Slot 0 = country ("polska"), slot 1 = city ("warszawa"), slot 2 = street in a city ("ulica marszalkowska" / "srodmiescie" -> warszawa) |
| 3 | 0x12 (4 B) | raw |
| 4 | 0x08 (32 B) | **road segments**, see below |
| 5 | 0x10 (8 B) | **points** inside the parcel: x, y, owning segment, 2 attribute bytes |
| 6 | 0x06 (16 B) | points on the parcel edge (x or y = 0 or max): where roads continue into the next parcel; + 32-bit and 14-bit fields |
| 7 | 0x0c (6 B) | points: x, y, 3-bit value |
| 9, 10 | 0x0f, 0x14 (8 B) | links to neighbouring parcels: u32 unit pointer (e.g. `03e19403`), u16 offset there, u16 |
| 11, 12 | 0x13, 0x15 (6 B) | referenced by segments (+30, +20); meaning open |
| 14 | – | text area (names) |

Segment record (type 0x08, 32 bytes), checked on Warsaw (83 293 segments):

| Offset | Content | How it was checked |
|---|---|---|
| +0, +2 | start and end point (offsets of point records in slot 5, 6 or 7) | the network draws correctly |
| +4 | slot 7 record | |
| +6, +8 | other segments (0 = none) | |
| +10 | ? (139, 130, 133 most common) | |
| +11 bits 4-5 | **one-way**: 0 both ways, 1 only start → end, 2 only end → start, 3 closed both ways | dual carriageways (Aleje Jerozolimskie 142/137, Puławska 96/95) split evenly into 1 and 2; with right-hand traffic the other carriageway lies left of the allowed direction for 98 %; 3 occurs exactly with kind 5 |
| +11 bits 0-3 | ? (12 most common) | |
| +12 | **length in metres** | median of geometric length / value = 1.00 in every class |
| +14, +15 | **headings** at both ends, 256 steps per turn (+15 = +14 + 128 on straight segments) | |
| +16 bits 0-3 | **road class (FRC)**: 0 motorway/expressway … 6 minor | Warsaw: S8, Wisłostrada, Trasa Łazienkowska = 0, arteries 1-2, collectors 3, local 4-6 |
| +16 bits 4-6 | **kind**: 5 = not routable (skipped by dbq `FUN_00001890`; always one-way code 3) | |
| +17 | ? (32 most common) | |
| +18 / +20 / +30 | slot 10 / 12 / 11 records | |
| +26 | **slot 2 record: street name, district, city** (and country through the parents) | Warsaw centre parcel: Aleje Jerozolimskie, Marszałkowska, Chmielna, Złota …; top names in Warsaw: Aleje Jerozolimskie, Puławska, Wał Miedzeszyński, Modlińska |

**Point coordinates are steps of 64 GPS units (~1 m) from the parcel corner**: 0..512 in the smallest parcels (32 768 units wide), 0..1024, …, 0..8192 in larger ones (seen for every parcel size around Warsaw). Drawing every segment as a line from its start to its end point gives the road network (layer `roads`): Warsaw with the Vistula, the bridges and the centre.

## Unit kinds and tables (survey)

Word +4 of a unit is its kind; pbp picks the unpacker by it (only compressed units, byte +6 bit 0).

| Table | Share | Kind | Compressed | Content | Decoder |
|---|---|---|---|---|---|
| 1 | 38 % | 0 | yes | **routing level 0**: the full road network in parcels | `roads` level 0, `parcels` |
| 1e | 1.9 % | 3 | no | **routing level 1**: classes 0-2 | `roads` level 1 |
| 3 | 0.6 % | 2 | no | **routing level 2**: classes 0-1 | `roads` level 2 |
| 2 | 0.3 % | 1 | no | **routing level 3**: class 0 | `roads` level 3 |
| 6, 16, 1c | 6.5 % | 0x16, 0x15, 0x14 | mostly | **background**, three levels of detail: areas and generalised roads | `areas`, `overview_roads` |
| 7 | 3.1 % | 6 | no | **POI** | `poi` |
| 11 | 11.4 % | 0x10 | no | POI names (8-byte entries like table d, with detail records) | via `poi` |
| f | 21.3 % | 0x0e | no | street name search index (sorted, per city) | – |
| d / e | 1.7 % | 0x0c / 0x0d | no | city names / alphabetic index | – |
| 10, 12, 18 | 2 % | 0x0f, 0x11, 0x17 | no | indexes (pointer lists) | – |
| 15, 14, 1d, 0, 4, a, b, c, 13, 17 | small | various | some | ? | – |

## Routing levels (tables 1e, 3, 2)

Same parcel layout as table 1 (slots, points, places), not compressed, bigger parcels, only the more important roads: level 1 has classes 0-2, level 2 classes 0-1, level 3 class 0; the class 0 length is the same on all levels (378 km around Warsaw). Segments are type **0x09 (26 bytes)** there (dbq `FUN_00001890`: type 08 when the unit kind is 0, else 09): the same fields up to +25, no street reference at +26. Word 0x58 of every parcel points at the parcel of the level above (table 1 → 1e → 3 → 2).

## Background (tables 6, 16, 1c; pbp `FUN_00004840`, `FUN_000044a2`)

Layout after the unit header: slots 0-4 of a 0x3d-byte header (52 bytes), the unit box at 0x20. Unpacking: copy 52 bytes, 4 raw bytes of widths (globals 0x0dce..0x0dd1), then the bit stream with the generic expander:

| Slot | Record | Content |
|---|---|---|
| 0 | 0x3b (4 B) | groups of slot 1 by **area type**: 7-bit type, 1 bit, offset of the first slot 1 record of the group |
| 1 | 0x3a (20 B) | **areas**: +0 name, +2 first point, +4 **area in m²** (Las Kabacki 9.0 km²), +8/+12 label position (GPS units) |
| 2 | 0x3c (16 B) | **generalised roads**: +0 name (road number: "8", "61", "631"), +2 first point, +4 attribute |
| 3 | 8 or 4 B | **points**: absolute GPS pairs (u32, u32) when header +0x10+size(0x3f) is 0, else 16-bit x/y deltas in steps of 64 from the box corner |
| 4 | 0x15 (6 B) | second pass, release > 0x13 |

A record's shape runs from its first point to the next record's first point (count + 1 records, the last is a sentinel). Area types seen: 1 water (seas, lakes, rivers), 3 forest / park, 5 built-up, 6 cemetery, 8 island.

Bit reader detail: `FUN_00004c16` adds the width to the bit position, then `bfextu` extracts; a width of 0 extracts 32 bits **without moving** (68k bitfield width 0 = 32). The background units use a width of 0 for some fields.

## POI (table 7, kind 6, not compressed)

Unit: +8 slot (offset, count) of 28-byte records, +0x10 unit box. Record: +0 unit pointer and +4 offset of the **name** entry in table 11 (u16 offset of the text there), +6/+8 x/y (steps of 64 GPS units from the box corner), +10 **category**, +12 **brand** (text offset in this unit, e.g. "lukoil", "pko bank polski"), +16 u32 (?). 698 013 POI in 44 categories; names for the codes from the names under them (`POI_CATEGORIES` in the decoder): 12 fuel, 22 restaurant, 23 bank, 30 ATM, 36 church, 46 pharmacy, 56 school, 51 railway station, 52 airport, …

## Search indexes (first look, seen in the data)

Table d unit (kind 0x0c): slot 0 at 0x30 = 8-byte name entries (name, 0x00ff, second name, offset of a slot 1 record), slot 1 = 24-byte **city records**:

| Offset | Content |
|---|---|
| +0 | unit pointer into table 10, +4 offset there, +6 count (the city's street list?) |
| +8 | offset of a slot 2 run in this unit, +10 count |
| +12 | unit pointer into **table 1** (a parcel), +16 offset of the city's place record (slot 1) in that parcel |

Slot 2 = 8-byte (unit pointer into table 1, offset, count) entries; slot 4 = 12-byte entries (unit pointer into **table 11** = POI names, offset, count, name offset): the POI of the city. Table 10 units (kind 0x0f) hold 12-byte entries pointing back into table d units. To be decoded: the exact meaning of each pointer target, table f (street names) and 12/18.

## Next

1. Search indexes: f (streets), d/e (cities), 11 (POI names), 10/12/18; the index parts (`a`) of tables 1, 7, 1e, 3, 2, 6, …
2. Connections between parcels (slots 9/10, edge points of slot 6), the remaining segment bytes (+10, +11 low bits, +17), slots 7, 11, 12, the 32-bit POI field.
3. Small tables 15, 14, 1d, 0, 4, a, b, c, 13, 17 (header data?), unit kind 0x0e unpacker (for completeness; table f is not compressed).
