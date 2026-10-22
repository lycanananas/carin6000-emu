#ifndef SCRLOG_INCLUDED
#define SCRLOG_INCLUDED

#define SCRLOG_W 384
#define SCRLOG_H 240
#define SCRLOG_VISIBLE_H 228
/* RRRGGGBB */
#define SCRLOG_BG 0x00
#define SCRLOG_TEXT 0xff
#define SCRLOG_GREEN 0x1c
#define SCRLOG_RED 0xe0
#define SCRLOG_YELLOW 0xfc

#define SCRLOG_COLS 64		/* 6x8 cells */
#define SCRLOG_ROWS 28

/* The text is kept here: scrolling redraws it instead of reading video RAM back. */
struct scrlog {
	volatile unsigned char *fb;
	int row, col;
	unsigned char color;
	char text[SCRLOG_ROWS][SCRLOG_COLS];
	unsigned char colors[SCRLOG_ROWS][SCRLOG_COLS];
};

/* row 0 clears the screen; otherwise the log continues on that line (Doom after the loader). */
void scrlog_init(struct scrlog *l, volatile unsigned char *fb, int row);
void scrlog_putc(struct scrlog *l, char c);
void scrlog_puts(struct scrlog *l, const char *s);

#endif
