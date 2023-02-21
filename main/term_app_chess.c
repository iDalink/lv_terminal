#include "term_app.h"

static lv_color_t getColorByEscCode(int code)
{
	static lv_color_t colors[32];
	static int init = 0;
	if (init == 0)
	{
            init = 1;
            colors[0] = lv_color_make(0,     0,   0);
            colors[1] = lv_color_make(255,   0,   0);
            colors[2] = lv_color_make(102, 185, 102);
            colors[3] = lv_color_make(235, 235,  98);
            colors[4] = lv_color_make( 51, 132, 183);
            colors[5] = lv_color_make(255, 100, 100);
            colors[6] = lv_color_make(051, 185, 185);
            colors[7] = lv_color_make(255, 255, 255);
	}

    if (code >= 30 && code <= 37)
        return colors[code - 30];
    if (code >= 40 && code <= 47)
        return colors[code - 40];
    if (code >= 90 && code <= 97)
        return colors[code - 90];
    if (code >= 100 && code <= 107)
        return colors[code - 100];
    return lv_color_white();
}

static void key_notify_cb(void *state, key_event_t  *event, 
            const struct ps_code_ascii_map_t *map, uint16_t code);


static void label_pos(lv_obj_t *label, int i, int j)
{
    int w = SCREEN_WIDTH  / CHESS_COLUMN;
    int h = SCREEN_HEIGHT / CHESS_ROW;
    int x = j * w;
    int y = i * h;
    int x1 = x + (w - 10) / 2;
    int y1 = y + (h - 17) / 2;
    lv_obj_set_pos(label, x1, y1);
}

static void refresh_ui(view_chess_t *state)
{
    int i, j, top;
    term_t *term = &state->term;

    if (term->line_cnt > term->HEIGHT)
    {
        top = term->line_cnt - term->HEIGHT;
    }
    else
    {
        top = 0;
    }
    for (i = 0; i < term->HEIGHT; i++) {

        for (j = 0; j < term->WIDTH; j++) {
            if (term->HEIGHT > CHESS_ROW || term->WIDTH > CHESS_COLUMN)
            {
                printf("Abort term info err\n");
                abort();
            }
            int empty = 0;

            if (i + top >= term->line_cnt) {
                empty = 1;
            } else {
                term_line_t *line = term->lines[top + i];
                if (line == NULL || j >= line->chess_cnt) {
                    empty = 1;
                } else {
                    term_chess_t *chess = &line->chess[j];
                    //printf("code y:%d x:%d  code:%d %c color:%dx%d\n", i, j, chess->code, chess->code, chess->fg, chess->bg);
                    empty = chess->code == 0;
                }
            }
            lv_obj_t *label = state->labels[j][i];
            if (empty)
            {
                lv_obj_add_flag(label, LV_OBJ_FLAG_HIDDEN);
            }

            if (!empty) {
                lv_obj_clear_flag(label, LV_OBJ_FLAG_HIDDEN);
                term_line_t *line = term->lines[i + top];
                term_chess_t *chess = &line->chess[j];
                char text[2];
                text[0] = (char)chess->code;
                text[1] = 0;
                //lv_label_set_text_fmt(label, "%c", (char)chess->code);
                lv_label_set_text(label, text);
                int fg = chess->fg;
                if (fg == 30 || fg == 40 || fg == 90 || fg == 100)
                    fg = 37;
                lv_obj_set_style_text_color(label, getColorByEscCode(fg), LV_PART_MAIN);
                //label_pos(label, i, j);
                //lv_obj_refr_size(label);
            }
        }
    }
    {
    	int w = SCREEN_WIDTH  / CHESS_COLUMN;
		int h = SCREEN_HEIGHT / CHESS_ROW;
		int x = term->cursor.x * w;
		int y = term->cursor.y * h;
		lv_obj_set_pos(state->cursor, x, y);
    }
}

static void cusor_ready_cb(struct _lv_anim_t * anim)
{
	lv_obj_t *cursor = lv_anim_get_user_data(anim);
	if (lv_obj_has_flag(cursor, LV_OBJ_FLAG_HIDDEN))
		lv_obj_clear_flag(cursor, LV_OBJ_FLAG_HIDDEN);
	else
		lv_obj_add_flag(cursor, LV_OBJ_FLAG_HIDDEN);
	lv_anim_start(lv_obj_get_user_data(cursor));
}

static void on_ssh_conn(term_ssh_t *ssh, int err)
{
    if (err == SSH_ERR_OK)
    {
        ;
    }
    else
    {
        char buffer[128];
        snprintf(buffer, 128, "SSH err %s", ssh_err_msg[err]);
        toast_make(buffer);
        view_chess_destroy();
        view_wifi_list_create();
    }
}


static void on_ssh_recv(term_ssh_t *ssh, void *buffer, int len)
{
    int32_t rev, tb = 0;
    view_chess_t *state = ssh->user_data;
    unsigned long ava_size;
    unsigned long ini_size;
    while (1)
    {
        long long ticks1 = el_ticks_get(), ticks2, ticks3;
        rev = libssh2_channel_read(ssh->channel, ssh->buffer, sizeof(ssh->buffer));
        ticks2 = el_ticks_get();
        printf("ssh read rev:%d used:%lld\n", rev, ticks2 - ticks1);
        if (rev <= 0)
        {
            break;
        }
        tb += rev;
        term_process(&chess->term, (uint8_t *)ssh->buffer, rev);
#if DEBUG_PERFORMANCE
        printf("term process %dB used:%lld\n", rev, el_ticks_get() - ticks2);
#endif

        ava_size = 0;
        ini_size = 0;
        rev = libssh2_channel_window_read_ex(ssh->channel, &ava_size, &ini_size);
        //printf("window size rev:%d init:%ld ava:%ld\n", rev, ini_size, ava_size);

        el_sleep_ms(3); // Avoid take too much CPU time.
        if (ava_size <= 0)
        {
            break;
        }
    }
    
    if (tb > 0)
    {
#if DEBUG_PERFORMANCE
        long long ticks1 = el_ticks_get();
        refresh_ui(state);
        printf("lv refresh_ui used:%lld\n", el_ticks_get() - ticks1);
#else
        refresh_ui(state);
#endif
    }
}

static void timer_lv_cb(el_timer_t *timer)
{
    if (chess)
    {
        printf("##############\n");
        term_ssh_write(chess->ssh, "dmesg\n", 6);
        el_timer_start(timer);
    }
}

view_chess_t *view_chess_create(term_ssh_t *ssh)
{
    view_chess_t *state = el_mem_malloc_from_spiram(sizeof(*state));
    memset(state, 0, sizeof(*state));
    state->ssh = ssh;
    state->key_cb = key_notify_cb;

    ssh->user_data = state;
    term_ssh_set_conn_cb(ssh, on_ssh_conn);
    term_ssh_set_recv_cb(ssh, on_ssh_recv);


    term_init(&state->term);
    term_resize(&state->term, CHESS_COLUMN, CHESS_ROW);
    term_set_max_line(&state->term, CHESS_ROW);

    lv_obj_t *scr = lv_scr_act();
    lv_obj_t *frame = lv_obj_create(scr);
    lv_obj_set_size(frame, SCREEN_WIDTH, SCREEN_HEIGHT);
    lv_obj_set_style_bg_color(frame, lv_color_black(), 0);
    lv_obj_set_style_bg_opa(frame, LV_OPA_COVER, 0);
    lv_obj_clear_flag(frame, LV_OBJ_FLAG_SCROLLABLE);

    int i, j;
    for (i = 0; i < CHESS_ROW; i++)
    {
        for (j = 0; j < CHESS_COLUMN; j++)
        {
            lv_obj_t *label = lv_label_create(frame);
            LV_FONT_DECLARE(courier_16);
            lv_obj_set_style_text_font(label, &courier_16, LV_PART_MAIN);
            //printf("Chess[%dx%d] label:%p\n", j, i, label);
            lv_obj_set_style_text_color(label, lv_color_white(), LV_PART_MAIN);
            lv_label_set_text(label, "*");
            label_pos(label, i, j);
            state->labels[j][i] = label;
        }
    }

    state->cursor = lv_obj_create(frame);
    lv_obj_set_size(state->cursor, 1, 17);
    lv_obj_set_style_bg_opa(state->cursor, LV_OPA_COVER, 0);
    lv_obj_set_style_bg_color(state->cursor, lv_color_white(), LV_PART_MAIN);
    lv_anim_t a;
	lv_anim_init(&a);
	lv_anim_set_var(&a, state->cursor);
	lv_anim_set_ready_cb(&a, cusor_ready_cb);
	lv_anim_set_values(&a, 0, 100);
	lv_anim_set_time(&a, 800);
	lv_anim_set_user_data(&a, state->cursor);
	state->cursor_a = a;
	lv_obj_set_user_data(state->cursor, &state->cursor_a);
	lv_anim_start(&state->cursor_a);

    chess = state;
    state->frame = frame;
    //el_timer_t *lv_timer = el_timer_create(2000, timer_lv_cb);
    //el_timer_start(lv_timer);
    return state;
}

void view_chess_destroy(void)
{
    view_chess_t *state = chess;
    if (!state) return;
    lv_obj_del(state->frame);
    el_mem_free(state);
    chess = NULL;
}


static void key_notify_cb(void *state, key_event_t  *event, 
                          const struct ps_code_ascii_map_t *map, uint16_t code)
{
    view_chess_t *chess = state;
    if (event->prefix) return;
    if (kb_one.ctrl) 
    {
        char c = 0;
        switch (code) {
        case PS2_KEY_C:
            c = 3;
            break;
        case PS2_KEY_L:
            c = 12;
            break;
        case PS2_KEY_Z:
            c = 26;
            break;
        case PS2_KEY_H:
            c = 8;
            break;
        case PS2_KEY_D:
            c = 4;
            break;
        case PS2_KEY_A:
            c = 1;
            break;
        case PS2_KEY_E:
            c = 5;
            break;
        case PS2_KEY_N:
            c = 14;
            break;
        case PS2_KEY_P:
            c = 16;
            break;
        case PS2_KEY_X:
            c = 24;
            break;
        case PS2_KEY_V:
            c = 22;
            break;
        case PS2_KEY_K:
            c = 11;
            break;
        case PS2_KEY_G:
            c = 7;
            break;
        case PS2_KEY_F:
            c = 6;
            break;
        case PS2_KEY_B:
            c = 2;
            break;
        case PS2_KEY_S:
            c = 19;
            break;
        }
        if (c)
        {
            printf("ssh write:%d\n", c);
            term_ssh_write(chess->ssh, &c, 1);
        }
    }
    else if (map)
    {
        char c = kb_one.shift ? map->shift_ascii : map->ascii;
        term_ssh_write(chess->ssh, &c, 1);
    }
    else if (code == PS2_KEY_ENTER)
    {
        char c = '\n';
        term_ssh_write(chess->ssh, &c, 1);
    }
    else
    {
        char string[8] = { 0 };
        char c = 0;
        switch (code)
        {
        case PS2_KEY_ESC:
            c = 13;
            break;
        case PS2_KEY_BCKTR:
            c = 8;
            break;
        case PS2_KEY_ARRUP:
            string[0] = 0x1b;
            string[1] = '[';
            string[2] = 'A';
            break;
        case PS2_KEY_ARRLF:
            string[0] = 0x1b;
            string[1] = '[';
            string[2] = 'D';
            break;
        case PS2_KEY_ARRRT:
            string[0] = 0x1b;
            string[1] = '[';
            string[2] = 'C';
            break;
        case PS2_KEY_ARRDW:
            string[0] = 0x1b;
            string[1] = '[';
            string[2] = 'B';
            break;
        }
        if (c)
            term_ssh_write(chess->ssh, &c, 1);
        if (strlen(string))
            term_ssh_write(chess->ssh, string, strlen(string));
    }
}
