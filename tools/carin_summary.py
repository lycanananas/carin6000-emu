#!/usr/bin/env python3
"""Compact text report of one CARiN emulator run, meant to be read by an AI agent.

Usage: carin_summary.py <log dir>   (default: logs/latest)
Reads <log dir>/trace/*.log, trace/summary.txt, uart_a.txt, uart_b.txt.
Writes the report to <log dir>/report.txt and prints it (target: under ~80 lines).
Module names in PC samples come from loader.ini, extracted from the CD by tools/extract_cd.py.
"""
import collections
import re
import struct
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
RUN = Path(sys.argv[1]) if len(sys.argv) > 1 else ROOT / "logs" / "latest"
TRACE = RUN / "trace"
LOADER = ROOT / "temp" / "extracted_cd" / "Mk2IC" / "0209" / "RENAULT" / "loader.ini"
LOADER_BASE = 0x808804

# Text markers worth knowing about; the report lists only the ones that occur.
MARKERS = [
    "CC-Mk2i SYSTEM SOFTWARE",
    "is initialised",
    "Failed to synchronise with slave",
    "NVR-DISK FILE SYSTEM ERROR",
    "INCORRECT LOADFILE",
    "Read I/O error",
    "Received unexpected request",
    "Starting sw-loading",
    "ui_handler",
    "progress",
    "flash",
]

UART_TAIL_LINES = 12
TRACE_TAIL_LINES = 3
MAX_LINE = 110


def load_modules():
    """Return [(start, end, name)] for modules laid out contiguously in loader.ini."""
    if not LOADER.exists():
        return []
    data = LOADER.read_bytes()
    modules = []
    pos = 0
    while pos + 48 <= len(data):
        if data[pos:pos + 2] != b"\x4a\xfc":
            pos += 2
            continue
        size = struct.unpack(">I", data[pos + 4:pos + 8])[0]
        name_off = struct.unpack(">I", data[pos + 12:pos + 16])[0]
        if size == 0 or pos + size > len(data):
            pos += 2
            continue
        raw = bytearray()
        np = pos + name_off
        while np < pos + size:
            b = data[np]
            if b == 0:
                break
            raw.append(b & 0x7F)
            if b & 0x80:
                break
            np += 1
        name = raw.decode("ascii", errors="replace") or f"mod@{pos:X}"
        modules.append((LOADER_BASE + pos, LOADER_BASE + pos + size, name))
        pos += size
    return modules


def symbolize(pc, modules):
    for start, end, name in modules:
        if start <= pc < end:
            return f"{name}+{pc - start:X}"
    if pc < 0x10000:
        return "bootrom/kernel-low"
    return "?"


def clip(text):
    return text if len(text) <= MAX_LINE else text[:MAX_LINE - 3] + "..."


def read_lines(path):
    if not path.exists():
        return []
    return path.read_text(errors="replace").splitlines()


def uart_lines(path):
    """Printable, deduplicated lines of a UART capture; repeated lines become 'text xN'."""
    if not path.exists():
        return []
    text = path.read_bytes().decode("latin-1")
    text = text.replace("\r", "")
    lines = [re.sub(r"[^\x20-\x7e]", "", line).strip() for line in text.split("\n")]
    lines = [line for line in lines if line]
    out = []
    for line in lines:
        if out and out[-1][0] == line:
            out[-1][1] += 1
        else:
            out.append([line, 1])
    return [f"{line} x{count}" if count > 1 else line for line, count in out]


def parse_summary():
    sections = collections.OrderedDict()
    current = "head"
    sections[current] = []
    for line in read_lines(TRACE / "summary.txt"):
        if line.startswith("## "):
            current = line[3:].split(" (")[0].strip()
            sections[current] = []
        else:
            sections[current].append(line)
    return sections


def main():
    modules = load_modules()
    summary = parse_summary()
    out = []

    head = summary.get("head", [""])
    out.append(f"# {head[0].lstrip('# ')}" if head else "# no trace/summary.txt")
    if not TRACE.exists():
        out.append("trace/ missing: run the emulator first")
        finish(out)
        return

    chans = summary.get("channels", [])
    out.append("channels: " + " ".join(re.sub(r"lines=(\d+) bytes=\d+", r"\1", c).replace(": ", "=") for c in chans))

    out.append("## events (last 25)")
    out.extend(clip(l) for l in read_lines(TRACE / "events.log")[-25:])

    counters = [c for c in summary.get("counters", []) if "=" in c]
    counters.sort(key=lambda c: -int(c.split("=")[1]))
    out.append("## counters")
    out.append(" ".join(counters[:14]))

    out.append("## where the CPU sits (sampled PCs, by module)")
    per_module = collections.Counter()
    for line in summary.get("pc samples", []):
        m = re.match(r"pc=([0-9A-F]+) n=(\d+)", line)
        if m:
            per_module[symbolize(int(m.group(1), 16), modules)] += int(m.group(2))
    out.append(" ".join(f"{name}:{n}" for name, n in per_module.most_common(8)) or "none")

    out.append("## shared RAM hot spots")
    out.extend(clip(l) for l in summary.get("shared RAM busiest", [])[:6])

    out.append("## UART markers (count)")
    uart_a = uart_lines(RUN / "uart_a.txt")
    uart_b = uart_lines(RUN / "uart_b.txt")
    joined = "\n".join(uart_a + uart_b)
    found = [f"{m}={joined.count(m)}" for m in MARKERS if m in joined]
    out.append(" ".join(found) or "none")

    out.append(f"## uart_a tail ({len(uart_a)} distinct lines)")
    out.extend(clip(l) for l in uart_a[-UART_TAIL_LINES:])
    if uart_b:
        out.append(f"## uart_b tail ({len(uart_b)} distinct lines)")
        out.extend(clip(l) for l in uart_b[-5:])

    out.append("## channel tails")
    for name in ("mailbox", "mcasic", "arcnet", "nv", "irq", "cdrom", "duart", "video"):
        lines = read_lines(TRACE / f"{name}.log")
        if lines:
            out.append(f"[{name}]")
            out.extend("  " + clip(l) for l in lines[-TRACE_TAIL_LINES:])

    finish(out)


def finish(out):
    text = "\n".join(out) + "\n"
    (RUN / "report.txt").write_text(text)
    sys.stdout.write(text)


if __name__ == "__main__":
    main()
