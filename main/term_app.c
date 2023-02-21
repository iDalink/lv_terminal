#include "term_app.h"

el_looper_t looper;
view_wifi_list_t *wifi_list;
view_login_t *login;
view_chess_t *chess;



void clear_scroll_flag(void)
{
    lv_obj_t *scr = lv_scr_act();
    while (scr)
    {
    	lv_obj_clear_flag(scr, LV_OBJ_FLAG_SCROLLABLE);
    	scr = lv_obj_get_parent(scr);
    }
}

typedef struct {
    lv_obj_t obj;
    lv_obj_t *card;
    lv_obj_t *dot1;
    lv_obj_t *dot2;
    lv_obj_t *dot3;
    lv_obj_t *label;
    uint32_t create_ticks;
} hud_t;

const lv_obj_class_t hud_class = {
    .instance_size = sizeof(hud_t),
    .base_class = &lv_obj_class,
};


static void hud_dot_anim_cb(void * var, int32_t v)
{
	hud_t *hud = var;
	int i;
	lv_obj_t *objs[] = {hud->dot1, hud->dot2, hud->dot3};

	const uint32_t ticks = lv_tick_get();
	for (i = 0; i < 3; i++)
	{
		var = objs[i];
	    int center_x = lv_obj_get_x(var) + lv_obj_get_width(var) / 2;
	    int center_y = lv_obj_get_y(var) + lv_obj_get_height(var) / 2;
	    uint32_t var_ticks = ticks + hud->create_ticks  + i * 800 / 3;
	    var_ticks %= 800;
	    if (var_ticks > 400) var_ticks = 800 - var_ticks;
	    int size = var_ticks / 16;
	    lv_obj_set_size(var, size, size);
	    lv_obj_set_x(var, center_x - size / 2);
	    lv_obj_set_y(var, center_y - size / 2);
	}
}

lv_obj_t *hud_make(lv_obj_t *par, char *text)
{
    lv_obj_t *hud = lv_obj_class_create_obj(&hud_class, par);
    lv_obj_class_init_obj(hud);
    lv_obj_set_size(hud, SCREEN_WIDTH, SCREEN_HEIGHT);
    lv_obj_set_style_bg_opa(hud, LV_OPA_TRANSP, 0);

    lv_obj_t *card = lv_obj_create(hud);
    lv_obj_set_style_border_width(card, 1, LV_PART_MAIN);
    lv_obj_set_style_radius(card, 10, LV_PART_MAIN);
    lv_obj_set_style_bg_color(card, lv_color_white(), LV_PART_MAIN);
    lv_obj_set_style_bg_opa(card, LV_OPA_COVER, 0);
    lv_obj_set_size(card, 120, 120);
    lv_obj_center(card);

    lv_obj_t *label = lv_label_create(card);
    lv_label_set_text(label, text);
    lv_obj_align_to(label, NULL, LV_ALIGN_BOTTOM_MID, 0, -20);
    
    lv_obj_t *dot1 = lv_obj_create(card);
    lv_obj_t *dot2 = lv_obj_create(card);
    lv_obj_t *dot3 = lv_obj_create(card);
    dot1->user_data = (void *)0;
    dot2->user_data = (void *)1;
    dot3->user_data = (void *)2;

    lv_obj_set_size(dot1, 10, 10);
    lv_obj_set_size(dot2, 10, 10);
    lv_obj_set_size(dot3, 10, 10);
    lv_obj_set_style_radius(dot1, 100, LV_PART_MAIN);
    lv_obj_set_style_radius(dot2, 100, LV_PART_MAIN);
    lv_obj_set_style_radius(dot3, 100, LV_PART_MAIN);
    lv_obj_set_style_bg_opa(dot1, LV_OPA_COVER, 0);
    lv_obj_set_style_bg_opa(dot2, LV_OPA_COVER, 0);
    lv_obj_set_style_bg_opa(dot3, LV_OPA_COVER, 0);
    lv_obj_set_style_bg_color(dot1, lv_color_black(), LV_PART_MAIN);
    lv_obj_set_style_bg_color(dot2, lv_color_black(), LV_PART_MAIN);
    lv_obj_set_style_bg_color(dot3, lv_color_black(), LV_PART_MAIN);
    lv_obj_align_to(dot2, NULL, LV_ALIGN_TOP_MID, 0, 40);
    lv_obj_align_to(dot1, dot2, LV_ALIGN_OUT_LEFT_MID, -30, 0);
    lv_obj_align_to(dot3, dot2, LV_ALIGN_OUT_RIGHT_MID, 30, 0);

    lv_obj_refr_pos(dot1);
    lv_obj_refr_pos(dot2);
    lv_obj_refr_pos(dot3);

	lv_anim_t a;
	lv_anim_init(&a);
	lv_anim_set_var(&a, hud);
	lv_anim_set_exec_cb(&a, hud_dot_anim_cb);
	lv_anim_set_values(&a, 0, 100);
	lv_anim_set_time(&a, 400);
	lv_anim_set_playback_time(&a, 400);
	lv_anim_set_repeat_count(&a, LV_ANIM_REPEAT_INFINITE);

    hud_t *_hud = (hud_t *)hud;
    _hud->card = card;
    _hud->dot1 = dot1;
    _hud->dot2 = dot2;
    _hud->dot3 = dot3;
    _hud->label = label;
    _hud->create_ticks = lv_tick_get();

	lv_anim_start(&a);
    return hud;
}

void hud_set_text(lv_obj_t *obj, char *text)
{
    lv_label_set_text(obj->user_data, text);
}

void hud_destroy(lv_obj_t *hud)
{
    lv_obj_del(hud);
}

const lv_obj_class_t navbar_class = {
    .instance_size = sizeof(navbar_t),
    .base_class = &lv_obj_class,
};
navbar_t *navbar_create(lv_obj_t *par, const char *title)
{
    lv_obj_t *navbar = lv_obj_class_create_obj(&navbar_class, par);
    lv_obj_class_init_obj(navbar);
    lv_obj_set_size(navbar, lv_obj_get_width(par), 35);
    lv_obj_set_style_bg_color(navbar, lv_color_hex(0xC8), LV_PART_MAIN);
    lv_obj_set_style_bg_opa(navbar, LV_OPA_COVER, LV_PART_MAIN);

    lv_obj_t *title_label = lv_label_create(navbar);
    lv_label_set_text(title_label, title);
    lv_obj_set_style_text_color(title_label, lv_color_hex(0xffffff), LV_PART_MAIN);
    lv_obj_align_to(title_label, NULL, LV_ALIGN_TOP_MID, 0, 5);
    LV_FONT_DECLARE(courier_24);
    lv_obj_set_style_text_font(title_label, &courier_24, LV_PART_MAIN);

    lv_obj_t * back_btn = lv_img_create(navbar);
    LV_IMG_DECLARE(ic_back);
    lv_img_set_src(back_btn, &ic_back);
    lv_obj_set_style_img_recolor(back_btn, lv_color_white(), 0);
    lv_obj_set_style_img_recolor_opa(back_btn, LV_OPA_COVER, 0);
    lv_obj_set_pos(back_btn, 10, 3);
    lv_obj_add_flag(back_btn, LV_OBJ_FLAG_CLICKABLE);

    navbar_t *_navbar = (navbar_t *)navbar;
    _navbar->back_btn = back_btn;
    _navbar->title_label = title_label;
    return _navbar;
}

static void toast_ready_cb(lv_anim_t * a)
{
    lv_obj_del(a->var);
}

void toast_make(char *text)
{
    lv_obj_t *scr = lv_layer_top();
    lv_obj_t *toast = lv_obj_create(scr);

    lv_obj_set_style_bg_opa(toast, LV_OPA_COVER, 0);
    lv_obj_set_style_bg_color(toast, lv_color_hex(0xA52A2A), LV_PART_MAIN);
    lv_obj_set_size(toast, lv_obj_get_width(scr), 28);

    lv_obj_t *label = lv_label_create(toast);
    lv_label_set_text(label, text);
    lv_obj_set_style_text_color(label, lv_color_white(), LV_PART_MAIN);
    lv_label_set_long_mode(label, LV_LABEL_LONG_WRAP);
    lv_obj_set_width(label, 320 - 40);
    lv_obj_set_pos(label, 20, 5);
    LV_FONT_DECLARE(courier_24);
    lv_obj_set_style_text_font(label, &courier_24, LV_PART_MAIN);

    //lv_obj_set_height(toast, lv_obj_get_y(label) + lv_obj_get_height(label) + 5);
    lv_obj_align_to(toast, NULL, LV_ALIGN_BOTTOM_MID, 0, 0);


    lv_anim_t a;
    lv_anim_init(&a);
    lv_anim_set_var(&a, toast);
    lv_anim_set_ready_cb(&a, toast_ready_cb);
    lv_anim_set_values(&a, 0, 100);
    lv_anim_set_time(&a, 2000);
    lv_anim_start(&a);
}

lv_obj_t *create_btn(lv_obj_t *par, const char *txt, int w, int h)
{
    lv_obj_t * btn = lv_btn_create(par);
    lv_obj_set_style_border_width(btn, 1, LV_PART_MAIN);
    lv_obj_set_style_border_width(btn, 0, LV_PART_MAIN | LV_STATE_FOCUSED);
    lv_obj_set_style_border_opa(btn, LV_OPA_COVER, LV_PART_MAIN);
    lv_obj_set_style_border_opa(btn, LV_OPA_TRANSP, LV_PART_MAIN | LV_STATE_FOCUSED);

    lv_obj_set_style_border_color(btn, lv_color_hex(0xC8), LV_PART_MAIN);
    lv_obj_set_style_bg_color(btn, lv_color_hex(0xC8), LV_PART_MAIN | LV_STATE_FOCUSED);
    lv_obj_set_style_bg_opa(btn, LV_OPA_TRANSP, LV_PART_MAIN);
    lv_obj_set_style_bg_opa(btn, LV_OPA_COVER, LV_PART_MAIN | LV_STATE_FOCUSED);
    lv_obj_set_style_text_color(btn, lv_color_hex(0xC8), LV_PART_MAIN);
    lv_obj_set_style_text_color(btn, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_FOCUSED);

    lv_obj_t * label = lv_label_create(btn);
    lv_label_set_text(label, txt);
    LV_FONT_DECLARE(courier_24);
    lv_obj_set_style_text_font(label, &courier_24, LV_PART_MAIN);

    lv_obj_set_size(btn, w, h);
    lv_obj_center(label);
    return btn;
}


lv_obj_t *create_card(lv_obj_t *par, const char *txt, int w, int h)
{
    lv_obj_t * btn = lv_btn_create(par);
    lv_obj_set_style_border_width(btn, 1, LV_PART_MAIN);
    lv_obj_set_style_border_width(btn, 2, LV_PART_MAIN | LV_STATE_FOCUSED);
    lv_obj_set_style_border_opa(btn, LV_OPA_COVER, LV_PART_MAIN);
    lv_obj_set_style_border_color(btn, lv_color_hex(0x333333), LV_PART_MAIN);
    lv_obj_set_style_border_color(btn, lv_color_hex(0xC8), LV_PART_MAIN | LV_STATE_FOCUSED);

    lv_obj_set_style_radius(btn, 6, LV_PART_MAIN);
    lv_obj_set_style_bg_color(btn, lv_color_white(), LV_PART_MAIN);
    lv_obj_set_style_bg_opa(btn, LV_OPA_COVER, LV_PART_MAIN);
    lv_obj_set_style_text_color(btn, lv_color_hex(0x333333), LV_PART_MAIN);
    lv_obj_set_style_text_color(btn, lv_color_hex(0xC8), LV_PART_MAIN | LV_STATE_FOCUSED);

    lv_obj_t * label = lv_label_create(btn);
    lv_label_set_text(label, txt);
    LV_FONT_DECLARE(courier_24);
    lv_obj_set_style_text_font(label, &courier_24, LV_PART_MAIN);

    lv_obj_set_size(btn, w, h);
    lv_obj_align_to(label, NULL, LV_ALIGN_LEFT_MID, 10, 0);
    return btn;
}

const lv_obj_class_t input_class = {
    .instance_size = sizeof(input_t),
    .base_class = &lv_obj_class,
};

static void input_cancel_item_click(lv_event_t *event)
{
	lv_obj_t *input = lv_obj_get_parent(event->target);
	input = lv_obj_get_parent(input);
	lv_obj_del(input);
}

input_t *input_create(lv_obj_t *par, const char *placeholder)
{
	input_t *_input;
    lv_obj_t *input = lv_obj_class_create_obj(&input_class, par);
    _input = (input_t *)input;
    lv_obj_class_init_obj(input);
    lv_obj_set_size(input, SCREEN_WIDTH, SCREEN_HEIGHT);
    lv_obj_set_style_bg_opa(input, LV_OPA_80, 0);
	lv_obj_set_style_bg_color(input, lv_color_hex(0xd3d3d3), LV_PART_MAIN);

    lv_obj_t *card = lv_obj_create(input);
    lv_obj_set_style_bg_opa(card, LV_OPA_TRANSP, LV_PART_MAIN);
    lv_obj_set_size(card, 260, 80);
    lv_obj_refr_size(card);
    lv_obj_center(card);

    _input->confirm_btn = create_btn(card, "OK", 40, 28);
    lv_obj_align_to(_input->confirm_btn, NULL, LV_ALIGN_BOTTOM_RIGHT, 0, 0);

    _input->cancel_btn = create_btn(card, "CANCEL", 80, 28);
    lv_obj_align_to(_input->cancel_btn, _input->confirm_btn, LV_ALIGN_OUT_LEFT_MID, -10, 0);

    lv_obj_t *ta = lv_textarea_create(card);
    lv_obj_set_size(ta, lv_obj_get_width(card), 40);
    lv_obj_set_pos(ta, 0, 0);
    LV_FONT_DECLARE(courier_24);
    lv_obj_set_style_text_font(ta, &courier_24, LV_PART_MAIN);
	lv_obj_set_style_pad_top(ta, 6, LV_PART_MAIN);
	lv_obj_set_style_pad_hor(ta, 3, LV_PART_MAIN);

    lv_obj_set_style_border_width(ta, 2, LV_PART_MAIN);
	lv_obj_set_style_border_opa(ta, LV_OPA_COVER, LV_PART_MAIN);
	lv_obj_set_style_border_color(ta, lv_color_hex(0xc8), LV_PART_MAIN);
	lv_obj_set_style_bg_color(ta, lv_color_white(), LV_PART_MAIN);
	lv_obj_set_style_bg_opa(ta, LV_OPA_COVER, LV_PART_MAIN);
	lv_obj_set_style_text_color(ta, lv_color_black(), LV_PART_MAIN);

    lv_obj_set_style_anim_time(ta, 1000, LV_PART_CURSOR | LV_STATE_FOCUSED);
    lv_obj_set_style_bg_opa(ta, LV_OPA_COVER, LV_PART_CURSOR);

    lv_textarea_set_placeholder_text(ta, placeholder);
    lv_obj_set_style_text_color(ta, lv_color_hex(0x999999), LV_PART_TEXTAREA_PLACEHOLDER);
    lv_group_focus_obj(ta);

    lv_obj_add_event_cb(_input->cancel_btn, input_cancel_item_click, LV_EVENT_CLICKED, NULL);

    lv_group_get_focused(NULL);
    _input->card = card;
    _input->ta = ta;
    return _input;
}


void term_key_notify_cb(key_event_t  *event, 
            const struct ps_code_ascii_map_t *map, uint16_t code)
{
    if (wifi_list && wifi_list->key_cb)
    {
        wifi_list->key_cb(wifi_list, event, map, code);
    }
    if (login && login->key_cb)
    {
        login->key_cb(login, event, map, code);
    }
    if (chess && chess->key_cb)
    {
        chess->key_cb(chess, event, map, code);
    }
}

void textarea_input(lv_obj_t *ta, key_event_t *event, 
                const struct ps_code_ascii_map_t *map, uint16_t code)
{
    if (event->prefix) return;
    if (code == PS2_KEY_BCKTR)
    {
        lv_textarea_del_char(ta);
    }
    else if (code == PS2_KEY_DLET)
    {
        lv_textarea_del_char_forward(ta);
    }
    else if (code == PS2_KEY_ARRLF)
    {
        lv_textarea_cursor_left(ta);
    }
    else if (code == PS2_KEY_ARRRT)
    {
        lv_textarea_cursor_right(ta);
    }
    else if (code == PS2_KEY_HOME)
    {
        lv_textarea_set_cursor_pos(ta, 0);
    }
    else if (code == PS2_KEY_END)
    {
        lv_textarea_set_cursor_pos(ta, 1000000);
    }
    else if (map)
    {
        lv_textarea_add_char(ta, kb_one.shift ? map->shift_ascii : map->ascii);
    }
}
