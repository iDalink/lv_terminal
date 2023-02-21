#include "term_app.h"

static void wifi_list_item_click(lv_event_t *event);
static void rescan_item_click(lv_event_t *event);

static void refresh_ui(view_wifi_list_t *state)
{
    int i;
    if (state->aps_count == 0)
    {
        lv_obj_add_flag(state->list_page, LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(state->scan_label, LV_OBJ_FLAG_HIDDEN);
        if (state->is_scan)
            lv_label_set_text(state->scan_label, "Scanning WiFi...");
        else
            lv_label_set_text(state->scan_label, "None WiFi arroud.");
    }
    else
    {
        lv_obj_clear_flag(state->list_page, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(state->scan_label, LV_OBJ_FLAG_HIDDEN);

        for (i = 0; i < state->aps_count; i++)
        {
            printf("i:%d ssid:%s\n", i, state->aps[i].ssid);
            const char *title = (const char *)state->aps[i].ssid;

            lv_obj_t *btn = create_card(state->list_page, title, SCREEN_WIDTH - 10 * 2, 40);
            lv_obj_set_pos(btn, 10, i * 50);
            btn->user_data = (void *)i;
            lv_obj_add_event_cb(btn, wifi_list_item_click, LV_EVENT_CLICKED, state);
        }
    }
}


static void wifi_scan_over(wifi_ap_record_t *aps, int count)
{
    view_wifi_list_t *state = wifi_list;
    if (state == NULL) return;
    if (state->aps)
    {
        el_mem_free(state->aps);
        state->aps = NULL;
    }
    state->aps_count = count;
    if (count > 0)
    {
        int len = sizeof(wifi_ap_record_t) * count;
        state->aps = el_mem_malloc(len);
        memcpy(state->aps, aps, len);
    }
    refresh_ui(state);
}

static void wifi_scan_err(int err)
{
    view_wifi_list_t *state = wifi_list;
    if (state == NULL) return;
    lv_label_set_text(state->scan_label, "Scan failed");
}

static void wifi_conn_ok(void)
{
    view_wifi_list_destroy();
    view_login_create();
}

static void wifi_conn_err(int err)
{
    view_wifi_list_t *state = wifi_list;
    if (state == NULL) return;
    if (state->hud)
    {
        hud_destroy(state->hud);
        state->hud = NULL;
    }
    if (state->aps)
    {
        el_mem_free(state->aps);
        state->aps = NULL;
    }
    state->aps_count = 0;
    refresh_ui(state);
    lv_label_set_text(state->scan_label, "Connect failed");
}

static void key_notify_cb(void *state, key_event_t  *event, 
            const struct ps_code_ascii_map_t *map, uint16_t code)
{
    lv_obj_t *ta = lv_group_get_focused(lv_group_get_default());
    if (wifi_list->input == NULL) return;
    if (ta == wifi_list->input->ta)
    {
        textarea_input(ta, event, map, code);
    }
}

view_wifi_list_t *view_wifi_list_create(void)
{
    view_wifi_list_t *state = el_mem_malloc_from_spiram(sizeof(*state));
    memset(state, 0, sizeof(*state));
    state->key_cb = key_notify_cb;
    esp_wifi_disconnect();
    
    lv_obj_t *scr = lv_scr_act();
    lv_obj_t *frame = lv_obj_create(scr);
    lv_obj_set_size(frame, SCREEN_WIDTH, SCREEN_HEIGHT);
    lv_obj_refr_size(frame);

    state->navbar = navbar_create(frame, "WiFi Connect");
    lv_obj_add_flag(state->navbar->back_btn, LV_OBJ_FLAG_HIDDEN);

    lv_obj_t * scan_label = lv_label_create(frame);
    lv_label_set_text(scan_label, "Scanning WiFi...");
    lv_obj_center(scan_label);
    lv_obj_add_flag(scan_label, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_set_ext_click_area(scan_label, 50);
    lv_obj_add_event_cb(scan_label, rescan_item_click, LV_EVENT_CLICKED, state);


    lv_obj_t *page = lv_obj_create(frame);
    lv_obj_add_flag(page, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_pos(page, 0, 36);
    lv_obj_set_size(page, SCREEN_WIDTH, SCREEN_HEIGHT - 36);
    lv_obj_add_flag(page, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_align_to(page, NULL, LV_ALIGN_BOTTOM_MID, 0, 0);
    lv_obj_set_style_pad_top(page, 10, 0);
    lv_obj_set_style_pad_bottom(page, 10, 0);

    state->frame = frame;
    state->list_page = page;
    state->scan_label = scan_label;
    state->wifi_conn_err = wifi_conn_err;
    state->wifi_conn_ok = wifi_conn_ok;
    state->wifi_scan_err = wifi_scan_err;
    state->wifi_scan_over = wifi_scan_over;

    esp_wifi_scan_start(NULL, 0);
    state->is_scan = 1;
    refresh_ui(state);
    wifi_list = state;
    return state;
}


void view_wifi_list_destroy(void)
{
    view_wifi_list_t *state = wifi_list;
    if (!state) return;
    lv_obj_del(state->frame);
    if (state->aps) el_mem_free(state->aps);
    wifi_list = NULL;
}

static void password_cancel(lv_event_t *event)
{
    view_wifi_list_t *state = event->user_data;
    if (state->input)
    {
        lv_obj_del((lv_obj_t *)state->input);
        state->input = NULL;
    }
}

static void password_confirm(lv_event_t *event)
{
    view_wifi_list_t *state = event->user_data;
    int i  = state->focus_id;
    ESP_LOGI("WiFi", "connect i:%d %s", i, state->aps[i].ssid);

    static wifi_config_t wifi_config = {
        .sta = {
            .ssid = "A",
            .password = "A",
            .threshold.authmode = 0,

            .pmf_cfg = {
                .capable = true,
                .required = false
            },
        },
    };

    if (state->input)
    {
        const char *password = lv_textarea_get_text(state->input->ta);
        strcpy((char *)wifi_config.sta.password, password);
    }
    strcpy((char *)wifi_config.sta.ssid, (char *)state->aps[i].ssid);
    wifi_config.sta.threshold.authmode = state->aps[i].authmode;
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &wifi_config));
    ESP_ERROR_CHECK(esp_wifi_connect());

    password_cancel(event);
    state->hud = hud_make(state->frame, "Connecting...");
}

static void wifi_list_item_click(lv_event_t *event)
{
    view_wifi_list_t *state = event->user_data;
    int i  = state->focus_id = (int)event->target->user_data;

    if (state->aps[i].authmode != WIFI_AUTH_OPEN)
    {
        state->input = input_create(state->frame, "Password");
        lv_obj_add_event_cb(state->input->confirm_btn, password_confirm, LV_EVENT_CLICKED, (void *)state);
        lv_obj_add_event_cb(state->input->cancel_btn,  password_cancel,  LV_EVENT_CLICKED, (void *)state);
        lv_textarea_set_password_mode(state->input->ta, true);
        lv_textarea_set_text(state->input->ta, "12345678");
    }
    else
    {
        password_confirm(event);
    }
}


static void rescan_item_click(lv_event_t *event)
{
    view_wifi_list_t *state = event->user_data;
    if (state->aps_count)
    {
        el_mem_free(state->aps);
        state->aps = NULL;
        state->aps_count = 0;
    }
    esp_wifi_scan_start(NULL, 0);
    state->is_scan = 1;
    refresh_ui(state);
}
