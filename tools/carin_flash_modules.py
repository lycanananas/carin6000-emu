#!/usr/bin/env python3
"""List (and optionally extract) the OS-9 modules stored in a saved CARiN main flash.

The main flash (0x1200000-0x13fffff) is four 29F040 chips on the byte lanes of the 32-bit bus;
MAME saves them as nvram/carin/flash0..3 (lane 0 = most significant byte).

Usage: carin_flash_modules.py <dir with flash0..3> [output dir]
"""
import os
import struct
import sys

BASE = 0x1200000


def load_image(directory):
    lanes = [open(os.path.join(directory, f'flash{i}'), 'rb').read() for i in range(4)]
    image = bytearray(len(lanes[0]) * 4)
    for lane, data in enumerate(lanes):
        image[lane::4] = data
    return bytes(image)


def modules(image):
    offset = 0
    while offset + 0x30 <= len(image):
        if image[offset:offset + 2] != b'\x4a\xfc':
            offset += 2
            continue
        size, name_offset = struct.unpack('>I', image[offset + 4:offset + 8])[0], struct.unpack('>I', image[offset + 12:offset + 16])[0]
        if not 0x30 <= size <= len(image) - offset or not name_offset < size:
            offset += 2
            continue
        name_start = offset + name_offset
        name = image[name_start:image.index(b'\0', name_start)].decode('latin1')
        yield offset, size, name
        offset += (size + 1) & ~1


def main():
    image = load_image(sys.argv[1])
    out = sys.argv[2] if len(sys.argv) > 2 else None
    if out:
        os.makedirs(out, exist_ok=True)
    for index, (offset, size, name) in enumerate(modules(image)):
        print(f'{BASE + offset:08X} {size:6X} {name}')
        if out:
            with open(os.path.join(out, f'{index:03d}_{name}.mod'), 'wb') as f:
                f.write(image[offset:offset + size])


if __name__ == '__main__':
    main()
