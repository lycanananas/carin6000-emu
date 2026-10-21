#!/usr/bin/env python3
"""Batch-analyse OS-9 modules with headless Ghidra and export Markdown per module.

Usage: export_os9_modules.py [modules dir] [output dir]
Defaults: temp/flash_modules -> temp/os9_ghidra_exports (Ghidra project in temp/ghidra).
Modules come from tools/carin_flash_modules.py or tools/unpack_os9.py.
"""
import shutil
import subprocess
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
MODULES = Path(sys.argv[1] if len(sys.argv) > 1 else ROOT / "temp" / "flash_modules").resolve()
OUT = Path(sys.argv[2] if len(sys.argv) > 2 else ROOT / "temp" / "os9_ghidra_exports").resolve()
PROJECT_DIR = ROOT / "temp" / "ghidra"
PROJECT = "OS9_Batch_Project"


def main():
    ghidra = shutil.which("analyzeHeadless") or shutil.which("ghidra-analyzeHeadless")
    if not ghidra:
        sys.exit("error: Ghidra analyzeHeadless not found in PATH")
    modules = sorted(MODULES.glob("*"))
    if not modules:
        sys.exit(f"error: no modules in {MODULES}")
    PROJECT_DIR.mkdir(parents=True, exist_ok=True)
    for leftover in (PROJECT_DIR / f"{PROJECT}.gpr", PROJECT_DIR / f"{PROJECT}.rep"):
        if leftover.is_dir():
            shutil.rmtree(leftover)
        elif leftover.exists():
            leftover.unlink()
    subprocess.run([
        ghidra, str(PROJECT_DIR), PROJECT,
        "-import", *map(str, modules),
        "-processor", "68000:BE:32:default",
        "-postScript", "ExportOS9Modules.java", str(OUT),
        "-scriptPath", str(Path(__file__).resolve().parent),
        "-overwrite",
    ], check=True)


if __name__ == "__main__":
    main()
