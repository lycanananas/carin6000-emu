#!/usr/bin/env python3
"""ELF (linked at 0 with --emit-relocs, see doom.ld) -> flat image for the Carminat loader.

usage: mkflat.py IN.elf OUT.bin

Output (big-endian): header magic "CDOM", image size, bss size, entry, relocation count; the image
(every allocated section with contents, in address order from 0); the relocation offsets: 32-bit
words in the image that hold an absolute address and get the load address added. PC-relative
relocations need nothing; any other kind stops the build.
"""
import struct
import sys
from pathlib import Path

PC_RELATIVE = {4, 5, 6}           # R_68K_PC32, R_68K_PC16, R_68K_PC8
ABS32 = 1                         # R_68K_32
IGNORED = {0}                     # R_68K_NONE


def main():
    elf = Path(sys.argv[1]).read_bytes()
    out = Path(sys.argv[2])
    if elf[:4] != b"\x7fELF" or elf[4] != 1 or elf[5] != 2:
        sys.exit("not a 32-bit big-endian ELF")
    e_entry, = struct.unpack_from(">I", elf, 24)
    e_shoff, = struct.unpack_from(">I", elf, 32)
    e_shentsize, e_shnum, e_shstrndx = struct.unpack_from(">HHH", elf, 46)
    sections = []
    for i in range(e_shnum):
        name, stype, flags, addr, off, size, link, info, align, entsize = struct.unpack_from(
            ">IIIIIIIIII", elf, e_shoff + i * e_shentsize)
        sections.append(dict(name=name, type=stype, flags=flags, addr=addr, off=off, size=size,
                             link=link, info=info, entsize=entsize))
    strtab = sections[e_shstrndx]

    def sname(s):
        start = strtab["off"] + s["name"]
        return elf[start:elf.index(b"\0", start)].decode()

    SHF_ALLOC, SHT_NOBITS, SHT_RELA, SHT_REL = 2, 8, 4, 9
    loaded = [s for s in sections if s["flags"] & SHF_ALLOC and s["size"]]
    progbits = [s for s in loaded if s["type"] != SHT_NOBITS]
    nobits = [s for s in loaded if s["type"] == SHT_NOBITS]
    image_size = max(s["addr"] + s["size"] for s in progbits)
    end = max(s["addr"] + s["size"] for s in loaded)
    image = bytearray(image_size)
    for s in progbits:
        image[s["addr"]:s["addr"] + s["size"]] = elf[s["off"]:s["off"] + s["size"]]
    if nobits and min(s["addr"] for s in nobits) < image_size:
        sys.exit("bss is not after the image")

    relocs = []
    for s in sections:
        if s["type"] not in (SHT_RELA, SHT_REL):
            continue
        target = sections[s["info"]]
        if not (target["flags"] & SHF_ALLOC) or target["type"] == SHT_NOBITS:
            continue
        step = 12 if s["type"] == SHT_RELA else 8
        for k in range(s["size"] // step):
            r_offset, r_info = struct.unpack_from(">II", elf, s["off"] + k * step)
            rtype = r_info & 0xFF
            if rtype == ABS32:
                relocs.append(r_offset)
            elif rtype not in PC_RELATIVE and rtype not in IGNORED:
                sys.exit(f"{sname(target)}+{r_offset:#x}: relocation type {rtype} not supported")
    relocs.sort()

    header = struct.pack(">5I", 0x43444F4D, image_size, end - image_size, e_entry, len(relocs))
    out.write_bytes(header + bytes(image) + struct.pack(f">{len(relocs)}I", *relocs))
    print(f"{out}: image {image_size // 1024} KB, bss {(end - image_size) // 1024} KB, "
          f"{len(relocs)} relocations, entry {e_entry:#x}")


if __name__ == "__main__":
    main()
