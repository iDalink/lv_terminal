#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "term.h"

#define UTF8_END 0
#define UTF8_TXT 1

typedef struct
{
    int stat;
    int wanna_cnt;
    int txt_cnt;
    uint8_t txt[4];
    int unicode;
} utf8_t;

static void utf8_reset(utf8_t *u)
{
    u->stat = UTF8_END;
    u->wanna_cnt = 0;
    u->txt_cnt = 0;
}

static void utf8_push(utf8_t *u, uint8_t c)
{
    uint8_t bit;

    if ((c & 0xc0) == 0xc0)
    {
        u->stat      = UTF8_TXT;
        u->wanna_cnt = 0;
        u->txt_cnt   = 0;
        u->unicode   = 0;

        if ((c & 0xf0) == 0xf0)
        {
            u->wanna_cnt = 4;
            bit = c & 0x08;
        }
        else if ((c & 0xe0) == 0xe0)
        {
            u->wanna_cnt = 3;
            bit = c & 0x0f;
        }
        else
        {
            u->wanna_cnt = 2;
            bit = c & 0x1f;
        }
    }
    else //0x80
    {
        bit = c & 0x3f;
    }


    if (u->stat == UTF8_TXT)
    {
        u->txt[u->txt_cnt] = c;
        u->txt_cnt++;
        u->unicode = (u->unicode << 6) | bit;
        if (u->txt_cnt == u->wanna_cnt)
        {
            u->stat = UTF8_END;
        }
    }
    else
    {
        u->stat = UTF8_END;
        u->wanna_cnt = 1;
        u->txt_cnt   = 1;
        u->txt[0]    = c;
        u->unicode   = bit;
    }
}

#define ESC_ARG_SIZ   16
typedef struct
{
    char txt[100];
    int len;

    char priv;
    int arg[ESC_ARG_SIZ];
    int narg;              /* nb of args */
    char mode[2];
} csi_t;

static void csi_reset(csi_t *csi)
{
    csi->len = 0;
    memset(csi->txt, 0, sizeof(csi->txt));
}

static int csi_push(csi_t *csi, uint8_t c)
{
    if (csi->len >= sizeof(csi->txt) - 1)
    {
    	fprintf(stderr, "csi push overflow\n");
        return -1;
    }
    csi->txt[csi->len++] = c;
    return 0;
}

static void csi_dump(csi_t *csi)
{
    size_t i;
    uint32_t c;

    fprintf(stderr, "ESC[");
    for (i = 0; i < csi->len; i++) {
        c = csi->txt[i] & 0xff;
        if (isprint(c)) {
            putc(c, stderr);
        } else if (c == '\n') {
            fprintf(stderr, "(\\n)");
        } else if (c == '\r') {
            fprintf(stderr, "(\\r)");
        } else if (c == 0x1b) {
            fprintf(stderr, "(\\e)");
        } else {
            fprintf(stderr, "(%02x)", c);
        }
    }
    putc('\n', stderr);
}

static void csiparse(csi_t *csi)
{
    char *p = csi->txt, *np;
    long v;

    csi->narg = 0;
    if (*p == '?') {
        csi->priv = 1;
        p++;
    }

    csi->txt[csi->len] = '\0';
    while (p < csi->txt + csi->len) {
        np = NULL;
        v = strtol(p, &np, 10);
        if (np == p)
            v = 0;
        if (v == LONG_MAX || v == LONG_MIN)
            v = -1;
        csi->arg[csi->narg++] = v;
        p = np;
        if (*p != ';' || csi->narg == ESC_ARG_SIZ)
            break;
        p++;
    }
    csi->mode[0] = *p++;
    csi->mode[1] = (p < csi->txt + csi->len) ? *p : '\0';
}

enum glyph_attribute {
	ATTR_NULL       = 0,
	ATTR_BOLD       = 1 << 0,
	ATTR_FAINT      = 1 << 1,
	ATTR_ITALIC     = 1 << 2,
	ATTR_UNDERLINE  = 1 << 3,
	ATTR_BLINK      = 1 << 4,
	ATTR_REVERSE    = 1 << 5,
	ATTR_INVISIBLE  = 1 << 6,
	ATTR_STRUCK     = 1 << 7,
	ATTR_WRAP       = 1 << 8,
	ATTR_WIDE       = 1 << 9,
	ATTR_WDUMMY     = 1 << 10,
	ATTR_BOLD_FAINT = ATTR_BOLD | ATTR_FAINT,
};

unsigned int defaultfg = 37;
unsigned int defaultbg = 40;


static int32_t tdefcolor(const int *attr, int *npar, int l)
{
	int32_t idx = -1;
	uint r, g, b;

	switch (attr[*npar + 1]) {
	case 2: /* direct color in RGB space */
		break;
	case 5: /* indexed color */
		if (*npar + 2 >= l) {
			fprintf(stderr,
				"erresc(38): Incorrect number of parameters (%d)\n",
				*npar);
			break;
		}
		*npar += 2;
		if (!BETWEEN(attr[*npar], 0, 255))
			fprintf(stderr, "erresc: bad fgcolor %d\n", attr[*npar]);
		else
			idx = attr[*npar];
		break;
	case 0: /* implemented defined (only foreground) */
	case 1: /* transparent */
	case 3: /* direct color in CMY space */
	case 4: /* direct color in CMYK space */
	default:
		fprintf(stderr,
		        "erresc(38): gfx attr %d unknown\n", attr[*npar]);
		break;
	}

	printf("idx:%d\n", idx);
	return idx;
}

static void tsetattr(term_t *term, const int *attr, int l)
{
    int i;
    int32_t idx;

    for (i = 0; i < l; i++) {
        switch (attr[i]) {
        case 0:
            term->cursor.attr.mode &= ~(
                ATTR_BOLD       |
                ATTR_FAINT      |
                ATTR_ITALIC     |
                ATTR_UNDERLINE  |
                ATTR_BLINK      |
                ATTR_REVERSE    |
                ATTR_INVISIBLE  |
                ATTR_STRUCK     );
            term->cursor.attr.fg = defaultfg;
            term->cursor.attr.bg = defaultbg;
            break;
        case 1:
            term->cursor.attr.mode |= ATTR_BOLD;
            break;
        case 2:
            term->cursor.attr.mode |= ATTR_FAINT;
            break;
        case 3:
            term->cursor.attr.mode |= ATTR_ITALIC;
            break;
        case 4:
            term->cursor.attr.mode |= ATTR_UNDERLINE;
            break;
        case 5: /* slow blink */
            /* FALLTHROUGH */
        case 6: /* rapid blink */
            term->cursor.attr.mode |= ATTR_BLINK;
            break;
        case 7:
            term->cursor.attr.mode |= ATTR_REVERSE;
            break;
        case 8:
            term->cursor.attr.mode |= ATTR_INVISIBLE;
            break;
        case 9:
            term->cursor.attr.mode |= ATTR_STRUCK;
            break;
        case 22:
            term->cursor.attr.mode &= ~(ATTR_BOLD | ATTR_FAINT);
            break;
        case 23:
            term->cursor.attr.mode &= ~ATTR_ITALIC;
            break;
        case 24:
            term->cursor.attr.mode &= ~ATTR_UNDERLINE;
            break;
        case 25:
            term->cursor.attr.mode &= ~ATTR_BLINK;
            break;
        case 27:
            term->cursor.attr.mode &= ~ATTR_REVERSE;
            break;
        case 28:
            term->cursor.attr.mode &= ~ATTR_INVISIBLE;
            break;
        case 29:
            term->cursor.attr.mode &= ~ATTR_STRUCK;
            break;
        case 38:
            if ((idx = tdefcolor(attr, &i, l)) >= 0)
                term->cursor.attr.fg = idx;
            break;
        case 39:
            term->cursor.attr.fg = defaultfg;
            break;
        case 48:
            if ((idx = tdefcolor(attr, &i, l)) >= 0)
                term->cursor.attr.bg = idx;
            break;
        case 49:
            term->cursor.attr.bg = defaultbg;
            break;
        default:
            if (BETWEEN(attr[i], 30, 37)) {
                term->cursor.attr.fg = attr[i];
            } else if (BETWEEN(attr[i], 40, 47)) {
                term->cursor.attr.bg = attr[i];
            } else if (BETWEEN(attr[i], 90, 97)) {
                term->cursor.attr.fg = attr[i];
            } else if (BETWEEN(attr[i], 100, 107)) {
                term->cursor.attr.bg = attr[i];
            } else {
                fprintf(stderr,
                        "erresc(default): gfx attr %d unknown\n",
                        attr[i]);
                csi_dump(term->csi);
            }
            break;
        }
    }
}

static void term_show(term_t *term)
{
	int i;
	printf("term W:%d H:%d cursor:%dx%d", term->WIDTH, term->HEIGHT, term->cursor.x, term->cursor.y);
	printf("    line_cnt:%d esc:%d\n", term->line_cnt, term->esc_stat);

#if 0
	int top = term_top(term);
	for (i = 0; i < term->HEIGHT; i++) {
		if (top + i >= term->line_cnt) break;
		term_line_t *line = term->lines[i + top];
		if (!line) continue;
		printf("    line:%d  cnt:%d\n", i, line->chess_cnt);
	}
#endif
	printf("\n");
}
