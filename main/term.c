#include <string.h>
#include <ctype.h>
#include <stdint.h>
#define _XOPEN_SOURCE
#define __USE_XOPEN
#include <wchar.h>
#include "term.h"
#include "el_port.h"

#define BETWEEN(x, a, b)	((a) <= (x) && (x) <= (b))
#define ISCONTROLC0(c)		(BETWEEN(c, 0, 0x1f) || (c) == 0x7f)
#define DEFAULT(a, b)		(a) = (a) ? (a) : (b)
#define MIN(X, Y)               ((X) < (Y) ? (X) : (Y))

#define ESC_NULL  0
#define ESC_START 1
#define ESC_CSI   2
#define ESC_OSC   3

#define malloc el_mem_malloc_from_spiram
#define realloc el_mem_realloc_from_spiram
static void nope(char *x, ...){}
#define printf nope

static int term_top(term_t *term)
{
    if (term->line_cnt <= term->HEIGHT)
        return 0;
    return term->line_cnt - term->HEIGHT;
}

#include "term_util.c"

static void line_free(term_line_t *line)
{
    if (line->chess)
        free(line->chess);
    free(line);
}

static void line_expand_to(term_line_t *line, int cnt)
{
	int i = sizeof(term_chess_t) * line->chess_cnt;
	int j = sizeof(term_chess_t) * cnt;
    if (line->chess_cnt >= cnt)
        return;
    line->chess = realloc(line->chess, j);
    memset((char *)line->chess + i, 0, j - i);
    line->chess_cnt = cnt;
}

static void line_delete_n(term_line_t *line, int x, int cnt)
{
    int dst, src, size;
    if (!line) return;

    if (x < 0) x = 0;
    if (x >= line->chess_cnt)    x = line->chess_cnt;
    
    dst = x;
    src = x + cnt;
    if (src > line->chess_cnt) src = line->chess_cnt;
    size = line->chess_cnt - src;

    memmove(&line->chess[dst], &line->chess[src],       \
            size * sizeof(term_chess_t));
    line->chess_cnt = dst + size;
}

static term_line_t *line_new(void)
{
    term_line_t *line = malloc(sizeof(*line));
    line->chess = NULL;
    line->chess_cnt = 0;
    return line;
}

static void line_clear_chess(term_line_t *line, int x1, int x2);

static void cursor_push_pop(term_t *term, int push)
{
    static int id = 0;
    static term_cursor_t saved[3];
    int cnt = sizeof(saved) / sizeof(saved[0]);

    if (push)
    {
        saved[id] = term->cursor;
        id++;
        if (id >= cnt)  id = 0;
    }
    else
    {
        if (id == 0) id = cnt - 1;
        else         id--;
        term->cursor = saved[id];
    }
}

static term_line_t *term_init_line(term_t *term, int id)
{
    int i, j;
    if (id + 1 <= term->line_cnt)
        goto cot;
    if (term->max_line && id + 1 > term->max_line)
    {
        term_line_t **temp_lines;
        term_line_t *temp_line_local;
        if (term->line_cnt > term->max_line)
        {
            while(1) printf("term line:%d beyond max:%d\n", term->line_cnt, term->max_line);
        }
        else if (term->line_cnt < term->max_line)
        {
            term_init_line(term, term->max_line - 1); //expand first
        }
        int move_line_cnt = MIN(id + 1 - term->max_line, term->line_cnt);
        int left_line_cnt = term->line_cnt - move_line_cnt;
        int move_byte = sizeof(term->lines[0]) * move_line_cnt;
        int left_byte = sizeof(term->lines[0]) * left_line_cnt;

        temp_lines = move_line_cnt == 1 ? &temp_line_local : malloc(move_byte);

        for (i = 0; i < move_line_cnt; i++)
        {
            term_line_t *line = term->lines[i];
            line_clear_chess(line, 0, line->chess_cnt - 1);
        }

        memmove(temp_lines, term->lines, move_byte);
        memmove(term->lines, term->lines + move_line_cnt, left_byte);
        memmove(term->lines + left_line_cnt, temp_lines, move_byte);
        if (temp_lines != &temp_line_local) free(temp_lines);
        id = term->line_cnt - 1;
        goto cot;
    }
    i = sizeof(*term->lines) * term->line_cnt;
    j = sizeof(*term->lines) * (id + 1);
    term->lines = realloc(term->lines, j);
    memset((char *)term->lines + i, 0, j - i);

    term->line_cnt = id + 1;
cot:
    i = id;
    if (!term->lines[i])
        term->lines[i] = line_new();
    return term->lines[i];
}

static term_line_t *term_get_line(term_t *term, int y)
{
    if (y >= term->line_cnt)
        return NULL;
    return term->lines[y];
}

static void cursor_mv(term_t *term, int x, int y)
{
    int top = term_top(term);
    term_line_t *line;
    x = MIN(x, term->WIDTH - 1);
    y = MIN(y, term->HEIGHT - 1);
    line = term_init_line(term, top + y);
    line_expand_to(line, x + 1);
    term->cursor.x = x;
    term->cursor.y = y;
}

static void term_reset(term_t *term)
{
    int i;

    for (i = 0; i < term->line_cnt; i++)
    {
        if (term->lines[i])
            line_free(term->lines[i]);
    }
    term->line_cnt = 0;
    term->lines = NULL;
    
    term->cursor.attr.code  = 0;
    term->cursor.attr.fg    = defaultfg;
    term->cursor.attr.bg    = defaultbg;
    term->cursor.attr.mode  = 0;
    term->cursor.attr.dirty = 0;
    term->cursor.x = 0;
    term->cursor.y = 0;
    term->esc_stat = ESC_NULL;

    term->utf8 = malloc(sizeof(utf8_t));
    term->csi  = malloc(sizeof(csi_t));
    utf8_reset(term->utf8);
    csi_reset(term->csi);
    cursor_mv(term, term->cursor.x, term->cursor.y);
}

void term_init(term_t *term)
{
    memset(term, 0, sizeof(*term));
    term->WIDTH  = 20;
    term->HEIGHT = 4;
    term->max_line = 0;
    term_reset(term);
}

void term_set_max_line(term_t *term, int max)
{
    if (max < term->HEIGHT)
    {
        while(1) printf("term set max line must beyond HEIGHT\n");
    }
    term->max_line = max;
    if (term->line_cnt > max)
    {
        int lines_bytes = sizeof(term->lines[0]) * max;
        int i, line_cnt;
        term_line_t **new_lines = malloc(lines_bytes);
        line_cnt = term->line_cnt - max;
        for (i = 0; i < line_cnt; i++)
        {
            void *line = term->lines[i];
            if (line) line_free(line);
        }
        memcpy(new_lines, term->lines, lines_bytes);
        free(term->lines);
        term->lines = new_lines;
        term->line_cnt = max;
    }
}

void term_resize(term_t *term, int w, int h)
{
    term->WIDTH  = w;
    term->HEIGHT = h;
    cursor_mv(term, term->cursor.x, term->cursor.y);
}


static void line_clear_chess(term_line_t *line, int x1, int x2)
{
    int i;
    if (!line) return;

    for (i = x1; i < MIN(x2 + 1, line->chess_cnt); i++)
    {
        term_chess_t *chess = line->chess + i;
        if (chess == NULL) break;
        chess->code = 0;
    }
}

static void term_clear_chess(term_t *term, int x1, int y1, int x2, int y2)
{
    int i;
    int top = term_top(term);
    if (x1 == -1)
    {
        x1 = term->cursor.x;
        y1 = term->cursor.y;
    }
    if (x2 == -1)
    {
        x2 = term->cursor.x;
        y2 = term->cursor.y;
    }
    for (i = y1; i <= y2; i++)
    {
        term_line_t *line = term_get_line(term, top + i);
        if (!line) continue;

        if (i == y1)
            line_clear_chess(line, x1, term->WIDTH - 1);
        else if (i == y2)
            line_clear_chess(line, 0, x2);
        else
            line_clear_chess(line, 0, term->WIDTH - 1);
    }
}

static void mem_chk(term_t *term)
{
	int top = term_top(term);
	term_line_t *line = term_get_line(term, term->cursor.y + top);
	if (line == NULL)
	{
		fprintf(stderr, "mem_chk err line:%p\n", line);
	}
	if (line->chess_cnt <= term->cursor.x)
	{
		printf("mem_chk err chess_cnt:%d x:%d\n",
				line->chess_cnt, term->cursor.x);
		abort();
	}
}

static void term_append_char(term_t *term, int width, int code)
{
    int top, i;
    term_line_t *line;
    term_chess_t *chess;
    int new_line = 0;

    if (code == 10140)
    	width = 1;
    if (code == 10007)
    	width = 1;
    if(width <= 0)
    {
        printf("append char:%d code:%d\n", width, code);
        printf("wc:%d\n", wcwidth(width));
        if (code < 128)
        	width = 1;
        else
        	width = 2;
        //abort();
    }
    int hit = 0;
#if 0
	//if (code == 'L')
    if (term->cursor.x >= 58)
    {
    	printf("a\n");
    	hit = 1;
    }
#endif
    if (term->cursor.x == term->WIDTH - 1)
    {
    	top = term_top(term);
    	line = term_get_line(term, term->cursor.y +top);
    	if (hit)
    	{
    		term_show(term);
    		mem_chk(term);
    	}
    	chess = &line->chess[term->cursor.x];
    	if (chess->wrap)
    	{
    		new_line = 1;
    	}
    }

    if (term->cursor.x + width > term->WIDTH)
    {
		new_line = 1;
    }
    if (new_line)
    {
        if (term->cursor.y == term->HEIGHT - 1)
        {
            term_init_line(term, term->line_cnt);
            cursor_mv(term, 0, term->cursor.y);
        }
        else
        {
            cursor_mv(term, 0, term->cursor.y + 1);
        }
    }

    top = term_top(term);
    line = term_init_line(term, top + term->cursor.y);
    line_expand_to(line, term->cursor.x + 1 + width);
    for  (i = 0; i < width; i++)
    {
        term_chess_t *chess = &line->chess[term->cursor.x + i];
        *chess = term->cursor.attr;
        chess->code = i == 0 ? code : 0;
    }
    int wrap;
    if (hit)
    {
    	term_show(term);
    	mem_chk(term);
    }
    if (term->cursor.x + width < term->WIDTH)
    {
        cursor_mv(term, term->cursor.x + width, term->cursor.y);
        wrap = 0;
    }
    else
    {
        cursor_mv(term, term->cursor.x + width - 1, term->cursor.y);
        wrap =1;
    }
    if (hit)
    {
    	term_show(term);
    	mem_chk(term);
    }
    top = term_top(term);
	mem_chk(term);
	line = term_get_line(term, term->cursor.y + top);
	chess = &line->chess[term->cursor.x];
    chess->wrap = wrap;
}

static void term_scr_down(term_t *term, int org, int cnt)
{
	//printf("####down org:%d cnt:%d\n", org, cnt);
	int top = term_top(term);
	int cur_cnt = term->line_cnt - top;
	int new_cnt = cur_cnt + cnt;
	if (new_cnt > term->HEIGHT) new_cnt = term->HEIGHT;

	term_init_line(term, top + new_cnt - 1);
	int i;
	for (i = cur_cnt - 1; i >= org; i--)
	{
		int dst = i + cnt;
		if (dst >= term->HEIGHT) continue;
		term_line_t **src_line = &term->lines[top + i];
		term_line_t **dst_line = &term->lines[top + dst];

		void *temp = *dst_line;
		*dst_line = *src_line;
		*src_line = temp;
	}
	cursor_mv(term, term->cursor.x, term->cursor.y);
}

static void term_scr_up(term_t *term, int org, int cnt)
{
	int top = term_top(term);
	int cur_cnt = term->line_cnt - top;

	//printf("line_cnt:%d\n", term->line_cnt);
	//printf("####up org:%d cnt:%d top:%d cur_cnt:%d\n", org, cnt, top, cur_cnt);

	int i;
	for (i = org; i < cur_cnt - cnt; i++)
	{
		int src = i + cnt;
		term_line_t **dst_line = &term->lines[top + i];
		if (*dst_line)
		{
			free(*dst_line);
			*dst_line = NULL;
			printf("dst_line %d\n", top + i);
		}
		if (src >= cur_cnt) continue;

		term_line_t **src_line = &term->lines[top + src];


		*dst_line = *src_line;
		*src_line = NULL;
	}
	cursor_mv(term, term->cursor.x, term->cursor.y);
}

static void csihandle(term_t *term, csi_t *csi)
{
    //char buf[40];
    int len;

    switch (csi->mode[0]) {
    case '@': /* ICH -- Insert <n> blank char */
        DEFAULT(csi->arg[0], 1);
        for (len = 0; len < csi->arg[0]; len++)
            term_append_char(term, 1, 0);
        break;
    case 'A': /* CUU -- Cursor <n> Up */
        DEFAULT(csi->arg[0], 1);
        cursor_mv(term, term->cursor.x, term->cursor.y-csi->arg[0]);
        break;
    case 'B': /* CUD -- Cursor <n> Down */
    case 'e': /* VPR --Cursor <n> Down */
        DEFAULT(csi->arg[0], 1);
        cursor_mv(term, term->cursor.x, term->cursor.y+csi->arg[0]);
        break;
    case 'C': /* CUF -- Cursor <n> Forward */
    case 'a': /* HPR -- Cursor <n> Forward */
        DEFAULT(csi->arg[0], 1);
        cursor_mv(term, term->cursor.x+csi->arg[0], term->cursor.y);
        break;
    case 'D': /* CUB -- Cursor <n> Backward */
        DEFAULT(csi->arg[0], 1);
        cursor_mv(term, term->cursor.x-csi->arg[0], term->cursor.y);
        break;
    case 'E': /* CNL -- Cursor <n> Down and first col */
        DEFAULT(csi->arg[0], 1);
        cursor_mv(term, 0, term->cursor.y+csi->arg[0]);
        break;
    case 'F': /* CPL -- Cursor <n> Up and first col */
        DEFAULT(csi->arg[0], 1);
        cursor_mv(term, 0, term->cursor.y-csi->arg[0]);
        break;
    case 'G': /* CHA -- Move to <col> */
    case '`': /* HPA */
        DEFAULT(csi->arg[0], 1);
        cursor_mv(term, csi->arg[0]-1, term->cursor.y);
        break;
    case 'H': /* CUP -- Move to <row> <col> */
    case 'f': /* HVP */
        DEFAULT(csi->arg[0], 1);
        DEFAULT(csi->arg[1], 1);
        cursor_mv(term, csi->arg[1]-1, csi->arg[0]-1);
        break;
    case 'J': /* ED -- Clear screen */
        switch (csi->arg[0]) {
        case 0: /* below */
            term_clear_chess(term,
                             -1, -1,
                             term->WIDTH - 1, term->HEIGHT - 1);
            break;
        case 1: /* above */
            term_clear_chess(term, 0, 0, -1, -1);
            break;
        case 2: /* all */
            term_clear_chess(term,
                             0, 0,
                             term->WIDTH - 1, term->HEIGHT - 1);
            break;
        default:
            fprintf(stderr, "Unknow CSI J cmd arg0:%d", csi->arg[0]);
            break;
        }
        break;
    case 'K': /* EL -- Clear line */
        switch (csi->arg[0]) {
        case 0: /* right */
            term_clear_chess(term,
                             -1, -1,
                             term->WIDTH - 1, term->cursor.y);
            break;
        case 1: /* left */
            term_clear_chess(term,
                             0, term->cursor.y,
                             -1, -1);
            break;
        case 2: /* all */
            term_clear_chess(term,
                             0, term->cursor.y,
                             term->WIDTH - 1, term->cursor.y);
            break;
        }
        break;
    case 'S': /* SU -- Scroll <n> line up */
    	DEFAULT(csi->arg[0], 1);
        term_scr_up(term, 0, csi->arg[0]);
    	//term_init_line(term, term->line_cnt - 1 + csi->arg[0]);
        abort();
        break;
    case 'T': /* SD -- Scroll <n> line down */
    	DEFAULT(csi->arg[0], 1);
        term_scr_down(term, 0, csi->arg[0]);
        abort();
        break;
    case 'L': /* IL -- Insert <n> blank lines */
        DEFAULT(csi->arg[0], 1);
        printf("###### insert blank lines\n");
        term_scr_down(term, term->cursor.y, csi->arg[0]);
        //tinsertblankline(csi->arg[0]);
        break;
    case 'M': /* DL -- Delete <n> lines */
        DEFAULT(csi->arg[0], 1);
        printf("###### delete blank lines\n");
        term_scr_up(term, term->cursor.y, csi->arg[0]);
        //abort();
        //tdeleteline(csi->arg[0]);
        break;
    case 'X': /* ECH -- Erase <n> char */
        DEFAULT(csi->arg[0], 1);
        term_clear_chess(term, term->cursor.x, term->cursor.y,          \
                         term->cursor.x + csi->arg[0] - 1, term->cursor.y);
        break;
    case 'P': /* DCH -- Delete <n> char */
        DEFAULT(csi->arg[0], 1);
        line_delete_n(term_get_line(term, term->cursor.y),
                      term->cursor.x, csi->arg[0]);
        break;
    case 'd': /* VPA -- Move to <row> */
        DEFAULT(csi->arg[0], 1);
        cursor_mv(term, term->cursor.x, csi->arg[0]-1);
        break;
    case 'm': /* SGR -- Terminal attribute (color) */
        tsetattr(term, csi->arg, csi->narg);
        break;
    case 's': /* DECSC -- Save cursor position (ANSI.SYS) */
        cursor_push_pop(term, 1);
        break;
    case 'u': /* DECRC -- Restore cursor position (ANSI.SYS) */
        cursor_push_pop(term, 0);
        break;
    case 'r':
    	//term_reset(term);
    	break;
    default:
        fprintf(stderr, "unknow csi ");
        csi_dump(csi);
        break;
    }
}

static void term_process_char(term_t *term, uint8_t c)
{
    int control = ISCONTROLC0(c);
#if 1
    if (term->esc_stat >= ESC_START)
    {
        if (c == '\a' || c == 030 || c == 032 || c == 033)
        {
            term->esc_stat = ESC_NULL;
        }
    }
#endif
    if (term->esc_stat == ESC_START)
    {
        switch (c)
        {
        case '[':
            csi_reset(term->csi);
            term->esc_stat = ESC_CSI;
            break;
        case ']':
        	term->esc_stat = ESC_OSC;
        	break;
        case '7':
            cursor_push_pop(term, 1);
            break;
        case '8':
            cursor_push_pop(term, 0);
            break;
        case 'c':
        	term_reset(term);
        	return;
        	break;
        default:
            fprintf(stderr, "Unknow esc:0x%02x\n", c);
            break;
        }
    }
    else if (term->esc_stat == ESC_CSI)
    {
    	int rev = csi_push(term->csi, c);
        if (BETWEEN(c, 0x40, 0x7E) || rev == -1)
        {
            term->esc_stat = ESC_NULL;
            csi_t *csi = term->csi;
            csiparse(csi);
            csihandle(term, csi);
        }
    }
    else if (term->esc_stat == ESC_NULL && !control)
    {
        if (c >= 0x80)
        {
            utf8_t *utf8 = term->utf8;
            utf8_push(utf8, c);
            if (utf8->stat == UTF8_END)
            {
                term_append_char(term, wcwidth(utf8->unicode), utf8->unicode);
            }
        }
        else
        {
            term_append_char(term, 1, c);
        }
    }

    if (control)
    {
    	//printf("control %d 0x%02x %c\n", c, c, c);
		switch (c)
		{
		case '\t':
			term_append_char(term, 4, c);
			break;
		case '\b':
			if (term->cursor.x > 0)     term->cursor.x--;
			break;
		case '\r':
			term->cursor.x = 0;
			break;
		case '\f':
		case '\v':
		case '\n':
		{
			int y = term->cursor.y;
			if (term->cursor.y == term->HEIGHT - 1)
			{
				term_init_line(term, term->line_cnt);
				y = term->cursor.y;
			}
			else
			{
				y = term->cursor.y + 1;
			}
			cursor_mv(term, term->cursor.x, y);
			//cursor_mv(term, 0, y);
			break;
		}
		case 033:
			term->esc_stat = ESC_START;
			break;
		case 0:
			break;
		default:
			fprintf(stderr, "Unknow ctrl:%d hx:0x%02x\n", c, c);
			break;
		}
    }
}

void term_process(term_t *term, uint8_t *buffer, int len)
{
    int i;
    for (i = 0; i < len; i++)
    {
    	uint8_t c = buffer[i];
        //printf("proccess_char %d, 0x%02x, %c\n", c, c, c);
        term_process_char(term, c);
        //term_show(term);
        mem_chk(term);
    }
}



