"""Raw CD sectors (2352 bytes): EDC/ECC for Mode 1 and Mode 2 Form 1 (ECMA-130)."""

EDC_TABLE = []
for i in range(256):
    edc = i
    for _ in range(8):
        edc = (edc >> 1) ^ (0xD8018001 if edc & 1 else 0)
    EDC_TABLE.append(edc)

ECC_F = [0] * 256
ECC_B = [0] * 256
for i in range(256):
    j = (i << 1) ^ (0x11D if i & 0x80 else 0)
    ECC_F[i] = j
    ECC_B[i ^ j] = i


def edc(data):
    value = 0
    for b in data:
        value = (value >> 8) ^ EDC_TABLE[(value ^ b) & 0xFF]
    return value


def _ecc_pq(sector, major_count, minor_count, major_mult, minor_inc, out):
    # Computes P (86 x 24) or Q (52 x 43) parity over bytes 12.. of the sector.
    size = major_count * minor_count
    for major in range(major_count):
        index = (major >> 1) * major_mult + (major & 1)
        ecc_a = 0
        ecc_b = 0
        for _ in range(minor_count):
            temp = sector[12 + index]
            index += minor_inc
            if index >= size:
                index -= size
            ecc_a ^= temp
            ecc_b ^= temp
            ecc_a = ECC_F[ecc_a]
        ecc_a = ECC_B[ECC_F[ecc_a] ^ ecc_b]
        out[major] = ecc_a
        out[major + major_count] = ecc_a ^ ecc_b


def fix_mode2_form1(sector):
    """Recomputes EDC and ECC of a Mode 2 Form 1 sector (bytearray of 2352) in place."""
    sector[2072:2076] = edc(sector[16:2072]).to_bytes(4, "little")
    # Mode 2 ECC is computed with the 4 header bytes (address + mode) taken as zero.
    header = bytes(sector[12:16])
    sector[12:16] = b"\0\0\0\0"
    p = bytearray(172)
    _ecc_pq(sector, 86, 24, 2, 86, p)
    sector[2076:2248] = p
    q = bytearray(104)
    _ecc_pq(sector, 52, 43, 86, 88, q)
    sector[2248:2352] = q
    sector[12:16] = header


def is_form2(sector):
    return bool(sector[18] & 0x20)
