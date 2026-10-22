# Carminat Doom: doomgeneric on the CARiN / Carminat (68EC030, OS-9), started from a NAV_SW disc copy.
#   make         -> build/doom.bin (image), build/carminat.wad, build/ui_handler.mod (loader),
#                   ../../cd/carminat_doom.img/.cue
#   make chd     -> also ../../cd/carminat_doom.chd for the emulator
# Needs m68k-elf-gcc with newlib, ../../cd/carin_nav_sw.img and the Freedoom zip (fetched).
CROSS   ?= m68k-elf-
CC      := $(CROSS)gcc
OBJDUMP := $(CROSS)objdump
OBJCOPY := $(CROSS)objcopy
SIZE    := $(CROSS)size

POC     := ../poc
SRC_IMG := ../../cd/carin_nav_sw.img
OUT     := ../../cd/carminat_doom
DG      := third_party/doomgeneric/doomgeneric
FREEDOOM_URL := https://github.com/freedoom/freedoom/releases/download/v0.13.0/freedoom-0.13.0.zip
MAP     ?= E1M1
# Module sizes on the disc (both variants), see ../poc.
UI_SIZE := 31628
IL_SIZE := 41004

# --- Doom image ---------------------------------------------------------------------------------
DOOM_SRC := dummy am_map doomdef doomstat dstrings d_event d_items d_iwad d_loop d_main d_mode d_net \
	f_finale f_wipe g_game hu_lib hu_stuff info i_cdmus i_endoom i_joystick i_scale i_sound i_system \
	i_timer memio m_argv m_bbox m_cheat m_config m_controls m_fixed m_menu m_misc m_random p_ceilng \
	p_doors p_enemy p_floor p_inter p_lights p_map p_maputl p_mobj p_plats p_pspr p_saveg p_setup \
	p_sight p_spec p_switch p_telept p_tick p_user r_bsp r_data r_draw r_main r_plane r_segs r_sky \
	r_things sha1 sounds statdump st_lib st_stuff s_sound tables v_video wi_stuff w_checksum w_file \
	w_main w_wad z_zone w_file_stdc i_input i_video doomgeneric
DOOM_OBJS := $(addprefix build/doom/,$(addsuffix .o,$(DOOM_SRC))) \
	build/doom/dg_carminat.o build/doom/syscalls.o build/doom/fastmem.o build/doom/r_draw68k.o build/doom/os9.o build/doom/crt0.o

DOOM_CFLAGS := -mcpu=68030 -msoft-float -Os -MMD -std=gnu99 -fno-common -ffunction-sections -fdata-sections \
	-DCARMINAT -DCMAP256 -DDOOMGENERIC_RESX=384 -DDOOMGENERIC_RESY=240 -DNORMALUNIX -D_DEFAULT_SOURCE \
	-Isrc -I$(DG) -Wall -Wno-unused -Wno-pointer-sign -Wno-missing-braces
DOOM_LDFLAGS := -nostartfiles -T doom.ld -Wl,--emit-relocs -Wl,--gc-sections -Wl,--no-warn-rwx-segments \
	-Wl,-Map,build/doom.map

# --- loader (PIC module, like the PoC) ----------------------------------------------------------
LOADER_CFLAGS := -mcpu=68030 -msoft-float -Os -std=gnu11 -Wall -Wextra -ffreestanding -fno-builtin -nostdlib \
	-mpcrel -fno-pic -fno-jump-tables -fno-common -fomit-frame-pointer
LOADER_LDFLAGS := -nostdlib -T $(POC)/link.ld -Wl,--emit-relocs -Wl,--no-warn-rwx-segments

all: $(OUT).img

build build/doom build/dl:
	mkdir -p $@

build/doom/%.o: $(DG)/%.c | build/doom
	$(CC) $(DOOM_CFLAGS) -c -o $@ $<
build/doom/%.o: src/%.c src/carminat.h | build/doom
	$(CC) $(DOOM_CFLAGS) -c -o $@ $<
build/doom/%.o: src/%.S | build/doom
	$(CC) $(DOOM_CFLAGS) -c -o $@ $<

build/doom.elf: $(DOOM_OBJS) doom.ld
	$(CC) $(DOOM_CFLAGS) $(DOOM_LDFLAGS) -o $@ $(DOOM_OBJS) -lc -lgcc
	$(SIZE) $@

build/doom.bin: build/doom.elf tools/mkflat.py
	@if $(OBJDUMP) -d $< | grep -qE '\sf(move|add|sub|mul|div|cmp|tst|int|abs|neg|sqrt)'; then \
		echo "error: FPU instructions (the 68EC030 has no FPU)"; exit 1; fi
	python3 tools/mkflat.py $< $@

build/loader.elf: loader/start.S loader/loader.c src/os9.S src/carminat.h | build
	$(CC) $(LOADER_CFLAGS) $(LOADER_LDFLAGS) -o $@ loader/start.S loader/loader.c src/os9.S -lgcc
	@if $(OBJDUMP) -r $@ | grep -q 'R_68K_32 '; then \
		echo "error: absolute relocations in the loader:"; $(OBJDUMP) -r $@ | grep 'R_68K_32 '; exit 1; fi
	$(SIZE) $@

build/il_stub.elf: $(POC)/src/il_stub.S | build
	$(CC) $(LOADER_CFLAGS) $(LOADER_LDFLAGS) -o $@ $<

build/%.raw: build/%.elf
	$(OBJCOPY) -O binary -j .text $< $@

build/ui_handler.mod: build/loader.raw $(POC)/tools/mkmod.py
	python3 $(POC)/tools/mkmod.py $< ui_handler $@ --size $(UI_SIZE) --mem 0x200 --stack 0x2000

build/il.mod: build/il_stub.raw $(POC)/tools/mkmod.py
	python3 $(POC)/tools/mkmod.py $< il $@ --size $(IL_SIZE) --mem 0x100 --stack 0x400

# --- WAD ----------------------------------------------------------------------------------------
build/dl/freedoom.zip: | build/dl
	curl -sSL -o $@ $(FREEDOOM_URL)

build/dl/freedoom1.wad: build/dl/freedoom.zip
	unzip -o -j -q $< 'freedoom-0.13.0/freedoom1.wad' -d build/dl
	touch $@

build/carminat.wad: build/dl/freedoom1.wad tools/wadstrip.py
	python3 tools/wadstrip.py $< $@ --map $(MAP)

# --- disc ---------------------------------------------------------------------------------------
# The loader replaces ui_handler, the stub replaces il; the image and the WAD go to the end of the
# disc and two voice files' directory entries point at them (tools/cdfile.py).
build/stage1.img: build/ui_handler.mod build/il.mod $(POC)/tools/mkdisc.py
	python3 $(POC)/tools/mkdisc.py $(SRC_IMG) $@ ui_handler=build/ui_handler.mod il=build/il.mod

$(OUT).img: build/stage1.img build/doom.bin build/carminat.wad tools/cdfile.py
	python3 tools/cdfile.py $< $@ d_f.csf=build/doom.bin e_f.csf=build/carminat.wad

chd: $(OUT).img
	chdman createcd -i $(OUT).cue -o $(OUT).chd --force

clean:
	rm -rf build/doom build/*.elf build/*.raw build/*.mod build/*.bin build/*.img build/*.wad build/doom.map

.PHONY: all chd clean

-include $(wildcard build/doom/*.d)
