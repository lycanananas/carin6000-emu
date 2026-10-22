/*
 * Carminat Doom loader: runs from the disc in place of the installer's UI ("ui_handler"; the
 * installer "il" is the PoC's sleeping stub, so nothing touches the flash).
 *
 * Takes the two big free RAM blocks (the 2 MB Extension RAM for Doom's zone, the ~1 MB of system
 * RAM for the image, bss, malloc arena and stack), reads the Doom image from the disc, relocates it
 * and calls it. Never asks for "the largest block": other processes still need memory, and the
 * free list also shows video RAM (0x600500) and the slave's shared RAM (0x9F0100) as free.
 *
 * Built like the PoC: position independent, no writable globals, state on the stack.
 */
#include "../src/carminat.h"

#define DOOM_IMAGE "/cd/language_cd/d_f.csf"
#define DOOM_WAD "/cd/language_cd/e_f.csf"
#define ZONE_SIZE (2048 * 1024 - 64)	/* the Extension RAM block minus the allocator's rounding */
#define HEAP_SIZE (160 * 1024)		/* newlib malloc: file buffers, Doom's few mallocs */
#define STACK_SIZE (64 * 1024)
#define STDOUT 1

void run_image(void (*entry)(struct carminat_env *), struct carminat_env *env, void *stack_top);

static void log_line(const char *s)
{
	char line[96];
	int n = 0;
	while (s[n] && n < (int)sizeof(line) - 1) {
		line[n] = s[n];
		n++;
	}
	line[n++] = '\r';
	os9_writeln(STDOUT, line, n);
}

static char *str_cat(char *p, const char *s)
{
	while (*s)
		*p++ = *s++;
	*p = 0;
	return p;
}

static char *fmt_hex(char *p, unsigned v, int digits)
{
	for (int i = digits - 1; i >= 0; i--) {
		unsigned d = (v >> (i * 4)) & 0xf;
		*p++ = d < 10 ? '0' + d : 'A' + d - 10;
	}
	*p = 0;
	return p;
}

static void fail(const char *what, int err)
{
	char line[80];
	fmt_hex(str_cat(str_cat(str_cat(line, "doom loader: "), what), ", error "), -err, 3);
	log_line(line);
	for (;;)
		os9_sleep(0);
}

/* The frame buffer: the first "load VSR" instruction of videodr's display program. */
static unsigned find_vsr(void)
{
	for (unsigned a = VSC_DCP; a < VSC_DCP + 0x40; a += 4) {
		unsigned cmd = (VSC_DRAM[a] << 8) | VSC_DRAM[a + 1];
		unsigned data = (VSC_DRAM[a + 2] << 8) | VSC_DRAM[a + 3];
		unsigned op = cmd >> 12;
		if (op == 4 || op == 5)		/* LOAD_VSR: address bits 16-19 in the command */
			return ((cmd & 0xf) << 16) | data;
	}
	return 0x500;
}

static int read_all(int path, void *buf, unsigned size)
{
	unsigned char *p = buf;
	while (size) {
		int n = os9_read(path, p, size > 0x10000 ? 0x10000 : size);
		if (n <= 0)
			return n < 0 ? n : -0xd3;	/* E$EOF */
		p += n;
		size -= n;
	}
	return 0;
}

int main(void)
{
	char line[80];
	struct flat_header h;
	struct carminat_env env;
	void *zone, *block;

	log_line("");
	log_line("*** Carminat Doom loader ***");

	int got = os9_srqmem(ZONE_SIZE, &zone);
	if (got < 0)
		fail("no RAM for the zone", got);
	env.zone = zone;
	env.zone_size = got;

	int path = os9_open(DOOM_IMAGE, 1);
	if (path < 0)
		fail("cannot open " DOOM_IMAGE, path);
	int err = read_all(path, &h, sizeof(h));
	if (err < 0 || h.magic != FLAT_MAGIC)
		fail("bad image header", err ? err : -1);

	unsigned need = h.image_size + h.bss_size + HEAP_SIZE + STACK_SIZE;
	got = os9_srqmem(need, &block);
	if (got < 0)
		fail("no RAM for the image", got);
	unsigned char *base = block;
	fmt_hex(str_cat(fmt_hex(str_cat(fmt_hex(str_cat(line, "image at "), (unsigned)base, 8), " size "),
		need, 6), " zone at "), (unsigned)zone, 8);
	log_line(line);

	err = read_all(path, base, h.image_size);
	if (err < 0)
		fail("reading the image", err);
	/* Relocations, a chunk at a time (they can be a few hundred KB). */
	unsigned chunk[256];
	unsigned left = h.reloc_count;
	while (left) {
		unsigned n = left > 256 ? 256 : left;
		err = read_all(path, chunk, n * 4);
		if (err < 0)
			fail("reading relocations", err);
		for (unsigned i = 0; i < n; i++) {
			unsigned *word = (unsigned *)(base + chunk[i]);
			*word += (unsigned)base;
		}
		left -= n;
	}
	os9_close(path);
	for (unsigned i = 0; i < h.bss_size; i++)
		base[h.image_size + i] = 0;

	env.heap = base + h.image_size + h.bss_size;
	env.heap_end = env.heap + HEAP_SIZE;
	env.fb = VSC_DRAM + find_vsr();
	env.wad = DOOM_WAD;
	env.nav = os9_open("/nav_bus", 3);
	if (env.nav < 0)
		fail("cannot open /nav_bus", env.nav);

	log_line("starting doom");
	run_image((void (*)(struct carminat_env *))(base + h.entry), &env, base + need);
	return 0;
}
