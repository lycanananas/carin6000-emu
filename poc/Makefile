# CARiN PoC disc: a NAV_SW copy where sysgo starts the PoC in place of the installer's UI
# ("ui_handler") and a sleeping stub in place of the installer itself ("il"), for both hardware
# variants on the disc (Mk2IC/0109 and Mk2IC/0209).
#   make         -> build/ui_handler.mod, build/il.mod and ../cd/carin_poc.img/.cue
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
OUT     := ../cd/carin_poc
# Module sizes on the disc (the same in both variants; mkdisc.py checks every copy).
UI_SIZE := 31628
IL_SIZE := 41004

all: $(OUT).img

build:
	mkdir -p build

# Position independence: no writable data and no absolute 32-bit relocations.
define check_pic
	@if [ -n "$$($(OBJDUMP) -h $@ | awk '$$2==".data"||$$2==".bss"{ if (strtonum("0x"$$3)) print }')" ]; then \
		echo "error: writable globals (.data/.bss) are not allowed"; exit 1; fi
	@if $(OBJDUMP) -r $@ | grep -q 'R_68K_32 '; then \
		echo "error: absolute relocations:"; $(OBJDUMP) -r $@ | grep 'R_68K_32 '; exit 1; fi
	$(SIZE) $@
endef

build/poc.elf: src/start.S src/main.c src/font.h link.ld | build
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ src/start.S src/main.c -lgcc
	$(check_pic)

build/il_stub.elf: src/il_stub.S link.ld | build
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ src/il_stub.S
	$(check_pic)

build/%.bin: build/%.elf
	$(OBJCOPY) -O binary -j .text $< $@

build/ui_handler.mod: build/poc.bin tools/mkmod.py Makefile
	python3 tools/mkmod.py $< ui_handler $@ --size $(UI_SIZE) --mem 0x88e --stack 0x4000

build/il.mod: build/il_stub.bin tools/mkmod.py Makefile
	python3 tools/mkmod.py $< il $@ --size $(IL_SIZE) --mem 0x100 --stack 0x400

$(OUT).img: build/ui_handler.mod build/il.mod tools/mkdisc.py tools/cdsector.py
	python3 tools/mkdisc.py $(SRC_IMG) $@ ui_handler=build/ui_handler.mod il=build/il.mod

chd: $(OUT).img
	chdman createcd -i $(OUT).cue -o $(OUT).chd --force

clean:
	rm -rf build

.PHONY: all chd clean
