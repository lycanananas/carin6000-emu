#!/usr/bin/env python3
"""Puts new contents into files of a CD-i disc image without rebuilding its file system: the data
goes into new Mode 2 Form 1 sectors appended to the image, and the file's directory record is
pointed at them (LBN and size); the volume size in the disc label grows to match.

usage: cdfile.py IN.img OUT.img NAME=FILE [NAME=FILE ...]

NAME is a file name on the disc that occurs once (e.g. d_f.csf). The new sectors copy the
subheader of the file's first sector (file/channel/submode/coding), with EOF on the last one.
"""
import struct
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent.parent.parent / "poc" / "tools"))
from cdsector import fix_mode2_form1, is_form2  # noqa: E402

SECTOR = 2352
DATA = 24
DATA_SIZE = 2048
LABEL_LBN = 16
SYNC = b"\x00" + b"\xff" * 10 + b"\x00"


def bcd(v):
    return ((v // 10) << 4) | (v % 10)


def msf(lbn):
    f = lbn + 150
    return bytes((bcd(f // 4500), bcd(f // 75 % 60), bcd(f % 75)))


def find_record(image, name):
    """(sector, offset in user data) of the directory record called NAME; must be unique."""
    want = name.encode()
    hits = []
    for s in range(len(image) // SECTOR):
        sec = image[s * SECTOR:(s + 1) * SECTOR]
        if sec[15] != 2 or is_form2(sec):
            continue
        data = sec[DATA:DATA + DATA_SIZE]
        start = data.find(want)
        while start >= 0:
            p = start - 33
            if p >= 0 and data[p + 32] == len(want) and data[p] >= 33 + len(want):
                hits.append((s, p))
            start = data.find(want, start + 1)
    if len(hits) != 1:
        sys.exit(f"{name}: {len(hits)} directory records, expected 1")
    return hits[0]


def main():
    if len(sys.argv) < 4:
        sys.exit(__doc__)
    image = bytearray(Path(sys.argv[1]).read_bytes())
    out = Path(sys.argv[2])
    old_end = len(image) // SECTOR

    for arg in sys.argv[3:]:
        name, _, path = arg.partition("=")
        content = Path(path).read_bytes()
        s, p = find_record(image, name)
        base = s * SECTOR + DATA + p
        old_lbn, = struct.unpack_from(">I", image, base + 6)
        subheader = bytes(image[old_lbn * SECTOR + 16:old_lbn * SECTOR + 20])
        lbn = len(image) // SECTOR
        count = max(1, (len(content) + DATA_SIZE - 1) // DATA_SIZE)
        for i in range(count):
            sec = bytearray(SECTOR)
            sec[0:12] = SYNC
            sec[12:15] = msf(lbn + i)
            sec[15] = 2
            sh = bytearray(subheader)
            if i == count - 1:
                sh[2] |= 0x80          # EOF
            sec[16:20] = sh
            sec[20:24] = sh
            chunk = content[i * DATA_SIZE:(i + 1) * DATA_SIZE]
            sec[DATA:DATA + len(chunk)] = chunk
            fix_mode2_form1(sec)
            image += sec
        struct.pack_into(">I", image, base + 6, lbn)
        struct.pack_into(">I", image, base + 14, len(content))
        dsec = bytearray(image[s * SECTOR:(s + 1) * SECTOR])
        fix_mode2_form1(dsec)
        image[s * SECTOR:(s + 1) * SECTOR] = dsec
        print(f"{name}: {len(content)} bytes at LBN {lbn}..{lbn + count - 1} (was LBN {old_lbn})")

    # Volume size in the disc label (big-endian, every copy of the old value).
    new_end = len(image) // SECTOR
    label = bytearray(image[LABEL_LBN * SECTOR:(LABEL_LBN + 1) * SECTOR])
    data = label[DATA:DATA + DATA_SIZE]
    old_size, = struct.unpack_from(">I", data, 84)
    for off in range(0, DATA_SIZE - 3):
        if struct.unpack_from(">I", data, off)[0] == old_size and off % 2 == 0:
            struct.pack_into(">I", data, off, new_end)
    label[DATA:DATA + DATA_SIZE] = data
    fix_mode2_form1(label)
    image[LABEL_LBN * SECTOR:(LABEL_LBN + 1) * SECTOR] = label

    out.write_bytes(image)
    out.with_suffix(".cue").write_text(f'FILE "{out.name}" BINARY\n  TRACK 01 MODE2/2352\n    INDEX 01 00:00:00\n')
    print(f"wrote {out} ({old_end} -> {new_end} sectors, volume size {old_size} -> {new_end})")


if __name__ == "__main__":
    main()
