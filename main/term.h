#ifndef __TERM_H__
#define __TERM_H__

#include <stdio.h>
#include <stdint.h>

typedef struct {
    uint32_t code;    /* character code */
    uint32_t fg;      /* foreground  */
    uint32_t bg;      /* background  */
    uint32_t mode;
    uint8_t dirty;
    int wrap;
} term_chess_t;

typedef struct
{
    int chess_cnt;
    term_chess_t *chess;
} term_line_t;

typedef struct
{
    term_chess_t attr;
    int x;
    int y;
} term_cursor_t;

typedef struct
{
    int max_line;
    int WIDTH;
    int HEIGHT;
    term_cursor_t cursor;
    int line_cnt;     // Must greater or euqal than HEIGHT
    term_line_t **lines;

    int esc_stat;
    void *utf8;
    void *csi;
} term_t;

void term_init(term_t *term);
void term_set_max_line(term_t *term, int max);
void term_resize(term_t *term, int w, int h);

void term_process(term_t *term, uint8_t *buffer, int len);

#endif
