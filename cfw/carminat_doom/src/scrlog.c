/*
 * Text log on the screen (384 x 240, RRRGGGBB), for following the start on the real unit without
 * a serial cable. 5x7 font from the PoC in 6x8 cells: 64 columns, 28 lines (the display shows
 * the top 228 lines); scrolls when full, redrawing from its own copy of the text (no reads from
 * video RAM). Used by the loader (position independent, no globals:
 * all state in struct scrlog) and by Doom (stdout/stderr).
 */
#include "scrlog.h"
#include "../../poc/src/font.h"

#define CELL_W 6
#define CELL_H 8
#define COLS SCRLOG_COLS
#define ROWS SCRLOG_ROWS

static void draw_char(struct scrlog *l, int col, int row, char c, unsigned char color)
{
	if (c >= 'a' && c <= 'z')
		c -= 'a' - 'A';
	if (c < FONT_FIRST || c > FONT_LAST)
		c = ' ';
	const unsigned char *glyph = font_data[c - FONT_FIRST];
	volatile unsigned char *p = l->fb + row * CELL_H * SCRLOG_W + col * CELL_W;
	for (int y = 0; y < CELL_H; y++, p += SCRLOG_W) {
		unsigned bits = y < FONT_H ? glyph[y] : 0;
		for (int x = 0; x < CELL_W; x++)
			p[x] = (x < FONT_W && (bits & (0x10 >> x))) ? color : SCRLOG_BG;
	}
}

static void scroll(struct scrlog *l)
{
	for (int r = 0; r < ROWS - 1; r++)
		for (int c = 0; c < COLS; c++) {
			char ch = l->text[r + 1][c];
			unsigned char co = l->colors[r + 1][c];
			if (ch != l->text[r][c] || (ch != ' ' && co != l->colors[r][c])) {
				l->text[r][c] = ch;
				l->colors[r][c] = co;
				draw_char(l, c, r, ch, co);
			}
		}
	for (int c = 0; c < COLS; c++)
		if (l->text[ROWS - 1][c] != ' ') {
			l->text[ROWS - 1][c] = ' ';
			draw_char(l, c, ROWS - 1, ' ', SCRLOG_BG);
		}
	l->row = ROWS - 1;
}

void scrlog_init(struct scrlog *l, volatile unsigned char *fb, int row)
{
	l->fb = fb;
	l->row = row;
	l->col = 0;
	l->color = SCRLOG_TEXT;
	for (int r = 0; r < ROWS; r++)
		for (int c = 0; c < COLS; c++) {
			l->text[r][c] = ' ';
			l->colors[r][c] = SCRLOG_BG;
		}
	if (row == 0) {
		volatile unsigned *p = (volatile unsigned *)fb;
		for (int i = 0; i < SCRLOG_W * SCRLOG_H / 4; i++)
			p[i] = SCRLOG_BG * 0x01010101u;
	}
}

void scrlog_putc(struct scrlog *l, char c)
{
	if (c == '\r')
		return;
	if (c == '\n') {
		l->col = 0;
		if (++l->row >= ROWS)
			scroll(l);
		return;
	}
	if (l->col >= COLS) {
		l->col = 0;
		if (++l->row >= ROWS)
			scroll(l);
	}
	if (c >= 'a' && c <= 'z')
		c -= 'a' - 'A';
	l->text[l->row][l->col] = c;
	l->colors[l->row][l->col] = l->color;
	draw_char(l, l->col++, l->row, c, l->color);
}

void scrlog_puts(struct scrlog *l, const char *s)
{
	while (*s)
		scrlog_putc(l, *s++);
}
