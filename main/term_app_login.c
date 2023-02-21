#include "term_app.h"


static void on_ssh_conn(term_ssh_t *ssh, int err)
{
    view_login_t *state = login;
    if (err == SSH_ERR_OK)
    {
        state->ssh = NULL;
        view_login_destroy();
        view_chess_create(ssh);
    }
    else
    {
        if (state->hud)
        {
            hud_destroy(state->hud);
            state->hud = NULL;
        }
        char buffer[128];
        snprintf(buffer, 128, "Login err %s", ssh_err_msg[err]);
        toast_make(buffer);
    }
}

static void login_item_click(lv_event_t *event)
{
    view_login_t *state = login;
    const char *server = lv_textarea_get_text(state->server_ta);
    const char *port = lv_textarea_get_text(state->port_ta);
    const char *user = lv_textarea_get_text(state->user_ta);
    const char *pwd = lv_textarea_get_text(state->pwd_ta);
    if (port == NULL || strlen(port) == 0) port = "22"; 
    printf("Login info:%s@%s:%s:%s\n", user, pwd, server, port);

    state->ssh = term_ssh_create((char *)server, (char *)port, (char *)user, (char *)pwd);
    if (state->ssh == NULL)
    {
        toast_make("WiFi errr.");
        return;
    }
    term_ssh_set_looper(state->ssh, &looper);
    term_ssh_set_conn_cb(state->ssh, on_ssh_conn);
    term_ssh_connect(state->ssh);
    state->hud = hud_make(state->frame, "Login...");
}

static void back_item_click(lv_event_t *event)
{
    view_login_destroy();
    view_wifi_list_create();
}

static void key_notify_cb(void *state, key_event_t  *event, 
            const struct ps_code_ascii_map_t *map, uint16_t code)
{
    lv_obj_t *ta = lv_group_get_focused(lv_group_get_default());
    
    printf("login key_notifiy_cb ta:%p ta1:%p ta2:%p\n", ta, 
        login->server_ta, login->port_ta);
    if (ta == NULL) return;
    if (code  == PS2_KEY_ENTER || code == PS2_KEY_TAB)
    {
        if (event->prefix)
            return;
        if (ta == login->server_ta)
            lv_group_focus_obj(login->port_ta);
        else if (ta == login->port_ta)
            lv_group_focus_obj(login->user_ta);
        else if (ta == login->user_ta)
            lv_group_focus_obj(login->pwd_ta);
        else if (ta == login->pwd_ta)
            lv_group_focus_obj(login->login_btn);
        else if (ta == login->login_btn && code == PS2_KEY_ENTER)
            login_item_click(NULL);
        return;
    }
    if (ta == login->server_ta || ta == login->port_ta || 
        ta == login->user_ta || ta == login->pwd_ta)
    {
        textarea_input(ta, event, map, code);
    }
}

view_login_t *view_login_create(void)
{
    int i;
    view_login_t *state = el_mem_malloc_from_spiram(sizeof(*state));
    memset(state, 0, sizeof(*state));
    state->key_cb = key_notify_cb;

    lv_obj_t *scr = lv_scr_act();
    lv_obj_t *frame = lv_obj_create(scr);
    lv_obj_set_size(frame, SCREEN_WIDTH, SCREEN_HEIGHT);
    lv_obj_refr_size(frame);

    state->navbar = navbar_create(frame, "SSH Account");
    lv_obj_add_event_cb(state->navbar->back_btn, back_item_click, LV_EVENT_CLICKED, NULL);

    void *param[] = 
    {
        "IP", &state->server_ta,
        "Port", &state->port_ta,
        "User", &state->user_ta,
        "Password", &state->pwd_ta,
    };

    for (i = 0; i < sizeof(param) / sizeof(param[0]); i += 2)
    {
        char *title = param[i];
        lv_obj_t **ta_p = param[i + 1];

        lv_obj_t *ta = lv_textarea_create(frame);
        lv_obj_set_style_border_width(ta, 1, LV_PART_MAIN);
        lv_obj_set_style_border_width(ta, 2, LV_PART_MAIN | LV_STATE_FOCUSED);
        lv_obj_set_size(ta, 260, 40);
        lv_obj_set_pos(ta, 30, (i / 2) * (30 + 5) + 45);
        lv_obj_add_flag(ta, LV_OBJ_FLAG_CLICK_FOCUSABLE);
        LV_FONT_DECLARE(courier_24);
        lv_obj_set_style_text_font(ta, &courier_24, LV_PART_MAIN);

        lv_obj_set_style_radius(ta, 15, LV_PART_MAIN);
        lv_obj_set_style_pad_hor(ta, 15, LV_PART_MAIN);
        lv_obj_set_style_border_color(ta, lv_color_hex(0x999999), LV_PART_MAIN);
        lv_obj_set_style_border_color(ta, lv_color_hex(0xc8), LV_PART_MAIN | LV_STATE_FOCUSED);

        lv_obj_set_style_text_color(ta, lv_color_hex(0x999999), LV_PART_TEXTAREA_PLACEHOLDER);
        lv_obj_set_style_text_color(ta, lv_color_hex(0x0), LV_PART_MAIN);
        lv_obj_set_style_text_color(ta, lv_color_hex(0xc8), LV_PART_MAIN | LV_STATE_FOCUSED);
        lv_obj_set_style_border_width(ta, 2, LV_PART_MAIN);

        lv_obj_set_style_anim_time(ta, 1000, LV_PART_CURSOR | LV_STATE_FOCUSED);
        lv_obj_set_style_bg_opa(ta, LV_OPA_TRANSP, LV_PART_CURSOR);

        lv_textarea_set_placeholder_text(ta, title);
        lv_textarea_set_one_line(ta, true);

        if (strstr(title, "IP"))
        {
            lv_textarea_set_text(ta, "192.168.0.1");
        }
        if (strstr(title, "User"))
        {
            lv_textarea_set_text(ta, "");
        }
        if (strstr(title, "Password"))
        {
            lv_textarea_set_text(ta, "");
        }
        if (strstr(title, "Port"))
        {
            lv_textarea_set_text(ta, "22");
        }
        *ta_p = ta;
    }

    lv_obj_t * login_btn = create_btn(frame, "Login", 260, 30);
    lv_obj_align(login_btn, LV_ALIGN_TOP_MID, 0, 190);
    lv_obj_set_style_radius(login_btn, 15, LV_PART_MAIN);
    lv_obj_add_event_cb(login_btn, login_item_click, LV_EVENT_CLICKED, NULL);
    
    state->frame = frame;
    state->login_btn = login_btn;
    login = state;
    return state;
}

void view_login_destroy(void)
{
    view_login_t *state = login;
    if (!state) return;
    lv_obj_del(state->frame);
    el_mem_free(state);
    login = NULL;
}