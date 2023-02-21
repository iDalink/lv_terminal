#ifndef __TERM_APP_H__
#define __TERM_APP_H__

#include "lvgl.h"
#include "esp_wifi.h"
#include "esp_log.h"
#include "esp_event.h"
#include "el_looper.h"
#include "term_ssh.h"
#include "term.h"
#include <el_timer.h>
#include "ps2.h"

#define DEBUG_PERFORMANCE 1
extern el_looper_t looper;

typedef void (* term_key_notify_cb_t)(void *state, key_event_t  *event, 
            const struct ps_code_ascii_map_t *map, uint16_t code);

#define SCREEN_WIDTH  320
#define SCREEN_HEIGHT 240


typedef struct {
    lv_obj_t obj;
    lv_obj_t *back_btn;
    lv_obj_t *title_label;
} navbar_t;

navbar_t *navbar_create(lv_obj_t *par, const char *title);

typedef struct
{
    lv_obj_t obj;
    lv_obj_t *card;
    lv_obj_t *ta;
    lv_obj_t *confirm_btn;
    lv_obj_t *cancel_btn;
} input_t;

input_t *input_create(lv_obj_t *par, const char *placeholder);

typedef struct 
{
    term_key_notify_cb_t key_cb;
    lv_obj_t *frame;
    navbar_t *navbar;
    lv_obj_t *list_page;
    lv_obj_t *scan_label;
    wifi_ap_record_t *aps;
    int aps_count;
    input_t *input;
    lv_obj_t *hud;
    int is_scan;
    int focus_id;
    void (*wifi_scan_over)(wifi_ap_record_t *aps, int count);
    void (*wifi_scan_err)(int err);

    void (*wifi_conn_ok)(void);
    void (*wifi_conn_err)(int err);
} view_wifi_list_t;

extern view_wifi_list_t *wifi_list;
view_wifi_list_t *view_wifi_list_create(void);
void view_wifi_list_destroy(void);


typedef struct
{
    term_key_notify_cb_t key_cb;
    lv_obj_t *frame;
    lv_obj_t *login_btn;
    navbar_t *navbar;
    lv_obj_t *server_ta;
    lv_obj_t *port_ta;
    lv_obj_t *user_ta;
    lv_obj_t *pwd_ta;
    term_ssh_t *ssh;
    lv_obj_t *hud;
} view_login_t;

extern view_login_t *login;
view_login_t *view_login_create(void);
void view_login_destroy(void);


#define CHESS_COLUMN 40
#define CHESS_ROW    15

typedef struct 
{
    term_key_notify_cb_t key_cb;
    lv_obj_t *frame;
    lv_obj_t *labels[CHESS_COLUMN][CHESS_ROW];
    lv_obj_t *cursor;
    lv_anim_t cursor_a;
    term_ssh_t *ssh;
    term_t term;
    char buffer[128];
} view_chess_t;

extern view_chess_t *chess;
view_chess_t *view_chess_create(term_ssh_t *ssh);
void view_chess_destroy(void);


void clear_scroll_flag(void);

void toast_make(char *text);

lv_obj_t *hud_make(lv_obj_t *par, char *text);
void hud_set_text(lv_obj_t *obj, char *text);
void hud_destroy(lv_obj_t *hud);

lv_obj_t *create_btn(lv_obj_t *par, const char *txt, int w, int h);
lv_obj_t *create_card(lv_obj_t *par, const char *txt, int w, int h);


void term_key_notify_cb(key_event_t  *event, 
            const struct ps_code_ascii_map_t *map, uint16_t code);

void textarea_input(lv_obj_t *ta, key_event_t *event, 
                const struct ps_code_ascii_map_t *map, uint16_t code);
#endif