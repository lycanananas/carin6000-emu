/*
 * CARiN PoC: started from CD in place of the installer's UI (module "ui_handler"; the disc's "il"
 * is a stub that only sleeps, src/il_stub.S), so nothing touches the flash.
 * Draws a big HELLO WORLD into the frame buffer set up by videodr, counts panel key presses read
 * from /nav_bus (frames 3A 3B 48 <code> for keys, 3A 3B 49 <steps> for the knob) and logs to the
 * console (UART A).
 *
 * Built position independent without writable globals (OS-9 maps the module read-only and gives
 * the process its data area in a6): all state lives on the stack, constants in .rodata.
 */
#include "font.h"

int os9_open(const char *name, int mode);
int os9_read(int path, void *buf, int count);
int os9_writeln(int path, const void *buf, int count);
int os9_ready(int path);
int os9_sleep(int ticks);
int os9_srqmem(unsigned size, void **block);
int os9_srtmem(void *block, unsigned size);
unsigned os9_ticks(unsigned *rate);
int os9_gblkmp(void *map, unsigned size, unsigned *out);

#define STDOUT 1
#define E_NOTRDY 0xf6

/* SCC66470 DRAM as seen by the CPU; videodr's display program (DCA) starts at DRAM 0x3ec. */
#define VSC_DRAM ((volatile unsigned char *)0x600000)
#define VSC_DCP 0x3ec
#define FB_FALLBACK 0x52f94
#define FB_W 384
#define FB_H 240
/* Lines the display shows while booted from CD (the rest is below the visible area). */
#define VIS_H 228

/* Colour indexes the original software draws with (palette not known yet). */
#define C_BG 0x49
#define C_TEXT 0xff
#define C_ACCENT 0x92
#define C_DARK 0x00

struct poc {
	volatile unsigned char *fb;
	int nav;
	int count;
	int presses;
	int last_code;
	int knob;
	char probe[48];
};

static int str_len(const char *s)
{
	int n = 0;
	while (s[n])
		n++;
	return n;
}

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

static char *fmt_int(char *p, int v, int width)
{
	char tmp[12];
	int n = 0;
	int neg = v < 0;
	unsigned u = neg ? -v : v;
	do {
		tmp[n++] = '0' + u % 10;
		u /= 10;
	} while (u);
	if (neg)
		tmp[n++] = '-';
	while (n < width)
		tmp[n++] = '0';
	while (n)
		*p++ = tmp[--n];
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

static char *str_cat(char *p, const char *s)
{
	while (*s)
		*p++ = *s++;
	*p = 0;
	return p;
}

/* Finds the frame buffer: the first "load VSR" instruction of videodr's display program. */
static unsigned find_vsr(void)
{
	for (unsigned a = VSC_DCP; a < VSC_DCP + 0x40; a += 4) {
		unsigned cmd = (VSC_DRAM[a] << 8) | VSC_DRAM[a + 1];
		unsigned data = (VSC_DRAM[a + 2] << 8) | VSC_DRAM[a + 3];
		unsigned op = cmd >> 12;
		if (op == 4 || op == 5)
			return ((cmd & 0xf) << 16) | data;
	}
	return FB_FALLBACK;
}

static void fill(struct poc *s, int x, int y, int w, int h, unsigned char c)
{
	for (int j = y; j < y + h; j++) {
		if (j < 0 || j >= FB_H)
			continue;
		volatile unsigned char *row = s->fb + j * FB_W;
		for (int i = x; i < x + w; i++)
			if (i >= 0 && i < FB_W)
				row[i] = c;
	}
}

static int text_width(const char *t, int scale)
{
	return str_len(t) * (FONT_W + 1) * scale - scale;
}

static void draw_text(struct poc *s, int x, int y, const char *t, int scale, unsigned char c)
{
	for (; *t; t++, x += (FONT_W + 1) * scale) {
		const unsigned char *g = font_glyph(*t);
		for (int row = 0; row < FONT_H; row++)
			for (int col = 0; col < FONT_W; col++)
				if (g[row] & (0x10 >> col))
					fill(s, x + col * scale, y + row * scale, scale, scale, c);
	}
}

static void draw_centered(struct poc *s, int y, const char *t, int scale, unsigned char c)
{
	draw_text(s, (FB_W - text_width(t, scale)) / 2, y, t, scale, c);
}

static const char *key_name(int code)
{
	switch (code) {
	case 0: return "MUTE NAV";
	case 1: return "LAST MESSAGE";
	case 2: return "VOL +";
	case 3: return "VOL -";
	case 4: return "KNOB PUSH";
	case 8: return "STAND BY";
	case 9: return "ADDRESS BOOK";
	case 7:
	case 12: return "AUTO TEST";
	default: return "UNKNOWN";
	}
}

static void draw_status(struct poc *s)
{
	char line[40];
	char *p;

	fill(s, 0, 100, FB_W, VIS_H - 12 - 100, C_BG);

	p = str_cat(line, "COUNT ");
	fmt_int(p, s->count, 4);
	draw_centered(s, 104, line, 4, C_TEXT);

	p = str_cat(line, "PRESSES ");
	fmt_int(p, s->presses, 0);
	draw_centered(s, 140, line, 2, C_ACCENT);

	if (s->last_code >= 0) {
		p = str_cat(line, "KEY ");
		p = fmt_int(p, s->last_code, 0);
		p = str_cat(p, " ");
		str_cat(p, key_name(s->last_code));
	} else if (s->knob) {
		str_cat(line, s->knob > 0 ? "KNOB >" : "KNOB <");
	} else {
		str_cat(line, "PRESS A KEY");
	}
	draw_centered(s, 162, line, 2, C_TEXT);
}

/* Pi, digit by digit (Rabinowitz-Wagon spigot), drawn as the digits come; blocks until done. */
#define PI_DIGITS 700
#define PI_LEN (PI_DIGITS * 10 / 3 + 1)
#define PI_TOP 100
#define PI_COLS 63

static void pi_put(struct poc *s, int *pos, char digit)
{
	char t[2] = { digit, 0 };
	int col = *pos % PI_COLS, row = *pos / PI_COLS;
	if (PI_TOP + row * 9 + FONT_H < VIS_H - 12)
		draw_text(s, 4 + col * (FONT_W + 1), PI_TOP + row * 9, t, 1, C_TEXT);
	(*pos)++;
	if (*pos == 1) {
		draw_text(s, 4 + (FONT_W + 1), PI_TOP, ".", 1, C_TEXT);
		(*pos)++;
	}
}

static void run_pi(struct poc *s)
{
	int a[PI_LEN];
	int pos = 0, nines = 0, predigit = 0;

	log_line("pi: start");
	fill(s, 0, PI_TOP, FB_W, VIS_H - 12 - PI_TOP, C_BG);
	for (int i = 0; i < PI_LEN; i++)
		a[i] = 2;
	for (int j = 1; j <= PI_DIGITS; j++) {
		int q = 0;
		for (int i = PI_LEN; i > 0; i--) {
			int x = 10 * a[i - 1] + q * i;
			a[i - 1] = x % (2 * i - 1);
			q = x / (2 * i - 1);
		}
		a[0] = q % 10;
		q /= 10;
		if (q == 9) {
			nines++;
		} else if (q == 10) {
			pi_put(s, &pos, '0' + predigit + 1);
			for (; nines; nines--)
				pi_put(s, &pos, '0');
			predigit = 0;
		} else {
			if (j > 1)
				pi_put(s, &pos, '0' + predigit);
			predigit = q;
			for (; nines; nines--)
				pi_put(s, &pos, '9');
		}
	}
	pi_put(s, &pos, '0' + predigit);
	log_line("pi: done");
}

/*
 * Doom feasibility probe, shown in the title bar and logged:
 * - free RAM: the free memory map (F$GBlkMp, nothing allocated), in total and the largest block;
 * - COPY: one 320x200 frame from RAM into the frame buffer (VSC DRAM);
 * - DRAW: a texture-mapped column fill of 320x200 in RAM, like Doom's R_DrawColumn (the inner loop
 *   that dominates its frame time).
 * Times are in ms per frame, averaged over BENCH_N frames.
 */
#define BENCH_N 10
#define BENCH_W 320
#define BENCH_H 200

static unsigned ms_now(void)
{
	unsigned rate;
	unsigned t = os9_ticks(&rate);
	return rate ? t * (1000 / rate) : 0;
}

static void probe(struct poc *s, char *summary)
{
	unsigned map[2 * 32], info[3];
	unsigned total = 0, largest = 0;
	char line[80];
	if (os9_gblkmp(map, sizeof(map), info) == 0) {
		total = info[2];
		for (unsigned i = 0; i < info[0] && i < 32; i++) {
			if (map[2 * i + 1] > largest)
				largest = map[2 * i + 1];
			if (map[2 * i + 1] >= 0x4000) {
				fmt_int(str_cat(fmt_hex(str_cat(line, "free block "), map[2 * i], 8), " size "), map[2 * i + 1], 0);
				log_line(line);
			}
		}
	}
	void *block = 0;
	int got = os9_srqmem(2 * BENCH_W * BENCH_H + 256, &block);
	unsigned copy_ms = 0, draw_ms = 0;
	int bench = got > 0;
	if (bench) {
		unsigned char *src = block;
		unsigned char *dst = src + BENCH_W * BENCH_H;
		unsigned char *cmap = dst + BENCH_W * BENCH_H;
		for (int i = 0; i < BENCH_W * BENCH_H; i++)
			src[i] = (i * 7) ^ (i >> 8);
		for (int i = 0; i < 256; i++)
			cmap[i] = 255 - i;
		unsigned t0 = ms_now();
		for (int f = 0; f < BENCH_N; f++)
			for (int y = 0; y < BENCH_H; y++) {
				const unsigned *from = (const unsigned *)(src + y * BENCH_W);
				volatile unsigned *to = (volatile unsigned *)(s->fb + (y + 14) * FB_W + (FB_W - BENCH_W) / 2);
				for (int x = 0; x < BENCH_W / 4; x++)
					to[x] = from[x];
			}
		unsigned t1 = ms_now();
		for (int f = 0; f < BENCH_N; f++)
			for (int x = 0; x < BENCH_W; x++) {
				unsigned frac = x << 12, step = 0x9000 + f;
				const unsigned char *tex = src + (x & 63) * 128;
				unsigned char *to = dst + x;
				for (int y = 0; y < BENCH_H; y++) {
					*to = cmap[tex[(frac >> 16) & 127]];
					to += BENCH_W;
					frac += step;
				}
			}
		unsigned t2 = ms_now();
		copy_ms = (t1 - t0) / BENCH_N;
		draw_ms = (t2 - t1) / BENCH_N;
	}
	if (bench)
		os9_srtmem(block, got);
	char *p = fmt_int(str_cat(summary, "RAM "), total >> 10, 0);
	p = fmt_int(str_cat(p, "K MAX "), largest >> 10, 0);
	p = str_cat(p, "K");
	if (bench) {
		p = fmt_int(str_cat(p, " COPY "), copy_ms, 0);
		p = fmt_int(str_cat(p, " DRAW "), draw_ms, 0);
		str_cat(p, " MS");
	}
	log_line(summary);
}

static void draw_screen(struct poc *s)
{
	fill(s, 0, 0, FB_W, FB_H, C_BG);
	fill(s, 0, 0, FB_W, 14, C_DARK);
	draw_text(s, 4, 3, "CARIN POC FROM CD", 1, C_ACCENT);
	draw_text(s, FB_W - 4 - text_width(s->probe, 1), 3, s->probe, 1, C_TEXT);
	fill(s, 12, 20, FB_W - 24, 72, C_DARK);
	draw_centered(s, 26, "HELLO", 5, C_TEXT);
	draw_centered(s, 64, "WORLD", 3, C_ACCENT);
	fill(s, 0, VIS_H - 12, FB_W, 12, C_DARK);
	draw_text(s, 4, VIS_H - 10, "VOL+/- KNOB: COUNT  PUSH: RESET  LAST MSG: PI", 1, C_TEXT);
	draw_status(s);
}

static void handle_frame(struct poc *s, const unsigned char *f, int n)
{
	char line[96];
	char *p = str_cat(line, "nav_bus:");
	for (int i = 0; i < n && i < 16; i++) {
		p = str_cat(p, " ");
		p = fmt_hex(p, f[i], 2);
	}
	log_line(line);

	if (n < 4 || f[0] != 0x3a || f[1] != 0x3b)
		return;
	if (f[2] == 0x48) {
		int code = f[3] & 0x3f;
		s->presses++;
		s->last_code = code;
		s->knob = 0;
		if (code == 2)
			s->count++;
		else if (code == 3)
			s->count--;
		else if (code == 4)
			s->count = 0;
		else if (code == 1) {
			run_pi(s);
			return;
		}
	} else if (f[2] == 0x49) {
		int steps = f[3] & 0x3f;
		if (f[3] & 0x40)
			steps = -steps;
		s->count += steps;
		s->presses++;
		s->last_code = -1;
		s->knob = steps;
	} else {
		return;
	}
	draw_status(s);
}

int main(void)
{
	struct poc s;
	char line[64];
	unsigned char frame[0x26];

	log_line("");
	log_line("*** CARiN PoC: hello world from CD (flash untouched) ***");

	unsigned vsr = find_vsr();
	s.fb = VSC_DRAM + vsr;
	s.count = 0;
	s.presses = 0;
	s.last_code = -1;
	s.knob = 0;
	str_cat(fmt_hex(str_cat(line, "frame buffer at DRAM "), vsr, 5), "");
	log_line(line);
	probe(&s, s.probe);
	draw_screen(&s);

	s.nav = os9_open("/nav_bus", 3);
	if (s.nav < 0) {
		fmt_hex(str_cat(line, "cannot open /nav_bus, error "), -s.nav, 3);
		log_line(line);
		draw_centered(&s, 162, "NO NAV BUS", 2, C_TEXT);
		for (;;)
			os9_sleep(0);
	}
	log_line("reading /nav_bus");

	for (;;) {
		int n = os9_read(s.nav, frame, sizeof(frame) - 1);
		if (n > 0) {
			handle_frame(&s, frame, n);
		} else if (n == -E_NOTRDY || n == 0) {
			os9_sleep(2);
		} else {
			fmt_hex(str_cat(line, "read error "), -n, 3);
			log_line(line);
			os9_sleep(50);
		}
	}
}
