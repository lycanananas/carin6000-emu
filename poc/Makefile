# CARiN PoC disc: replaces the installer module "il" in loader.ini of the NAV_SW disc.
#   make         -> build/il.mod and ../cd/carin_poc.img/.cue
#   make chd     -> also ../cd/carin_poc.chd for the emulator
CROSS   ?= m68k-elf-
CC      := $(CROSS)gcc
OBJCOPY := $(CROSS)objcopy
OBJDUMP := $(CROSS)objdump
SIZE    := $(CROSS)size

CFLAGS  := -mcpu=68030 -Os -std=gnu11 -Wall -Wextra -ffreestanding -fno-builtin -nostdlib \
           -mpcrel -fno-pic -fno-jump-tables -fno-common -fomit-frame-pointer
LDFLAGS := -nostdlib -T link.ld -Wl,--emit-relocs -Wl,--no-warn-rwx-segments

SRC_IMG := ../cd/carin_nav_sw.img
IL_ORIG := ../../re-safrane/extracted/loader.ini/65_il.mod
IL_SIZE := $(shell stat -c %s $(IL_ORIG) 2>/dev/null)
OUT     := ../cd/carin_poc

all: $(OUT).img

build:
	mkdir -p build

build/poc.elf: src/start.S src/main.c src/font.h link.ld | build
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ src/start.S src/main.c -lgcc
	@# Position independence: no writable data and no absolute 32-bit relocations.
	@if [ -n "$$($(OBJDUMP) -h $@ | awk '$$2==".data"||$$2==".bss"{ if (strtonum("0x"$$3)) print }')" ]; then \
		echo "error: writable globals (.data/.bss) are not allowed"; exit 1; fi
	@if $(OBJDUMP) -r $@ | grep -q 'R_68K_32 '; then \
		echo "error: absolute relocations:"; $(OBJDUMP) -r $@ | grep 'R_68K_32 '; exit 1; fi
	$(SIZE) $@

build/poc.bin: build/poc.elf
	$(OBJCOPY) -O binary -j .text $< $@

build/il.mod: build/poc.bin tools/mkmod.py Makefile
	python3 tools/mkmod.py $< il $@ --size $(IL_SIZE) --mem 0x88e --stack 0x4000

$(OUT).img: build/il.mod tools/mkdisc.py tools/cdsector.py
	python3 tools/mkdisc.py $(SRC_IMG) $(IL_ORIG) $< $@

chd: $(OUT).img
	chdman createcd -i $(OUT).cue -o $(OUT).chd --force

clean:
	rm -rf build

.PHONY: all chd clean
