#include <string.h>
#include <sys/types.h>
#include <fcntl.h>
#include <errno.h>
#include <stdio.h>
#include <ctype.h>
#include "esp_log.h"
#include "bsp_board.h"
#include "bsp_lcd.h"
#include "lv_port.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "lvgl.h"
#include "nvs_flash.h"
#include "esp_wifi.h"
#include <el_looper.h>
#include <el_timer.h>
#include <esp_task_wdt.h>
#include "term_app.h"
#include "ps2.h"

static const char* TAG = "MAIN";


static void event_cb_scan_done(el_event_t *event)
{
    if (wifi_list == NULL) return;
    uint16_t aps_count = 0;
    wifi_ap_record_t *aps = NULL;

    ESP_ERROR_CHECK(esp_wifi_scan_get_ap_num(&aps_count));
    ESP_LOGI(TAG, "WiFi scan found %d",(int)aps_count);
    if (aps_count > 0)
    {
        int len = sizeof(wifi_ap_record_t) * aps_count;
        aps = el_mem_malloc(len);
        ESP_ERROR_CHECK(esp_wifi_scan_get_ap_records(&aps_count, aps));
    }
    wifi_list->wifi_scan_over(aps, (int)aps_count);
    if (aps) el_mem_free(aps);
}

static void event_cb_conn_ok(el_event_t *event)
{
    if (wifi_list == NULL) return;
    wifi_list->wifi_conn_ok();
}

static void event_cb_conn_failed(el_event_t *event)
{
    if (wifi_list == NULL) return;
    wifi_list->wifi_conn_err(-1);
}

static void event_handler(void* arg, esp_event_base_t event_base,
                          int32_t event_id, void* event_data)
{
    if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_SCAN_DONE)
    {
        ESP_LOGI(TAG, "WIFI_EVENT_SCAN_DONE\n");
        el_event_t *event = el_event_create(event_cb_scan_done);
        el_looper_post_event(&looper, event);
    }
    else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_START)
    {
        ESP_LOGI(TAG, "WIFI_EVENT_STA_START\n");
    } 
    else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_DISCONNECTED)
    {
        ESP_LOGI(TAG, "WIFI_EVENT_STA_DISCONNECTED\n");
        el_event_t *event = el_event_create(event_cb_conn_failed);
        el_looper_post_event(&looper, event);
    }
    else if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP)
    {
        //ip_event_got_ip_t* ip_event = (ip_event_got_ip_t*) event_data;
        ESP_LOGI(TAG, "IP_EVENT_STA_GOT_IP IP\n");
        el_event_t *event = el_event_create(event_cb_conn_ok);
        el_looper_post_event(&looper, event);
    }
}


static void timer_lv_cb(el_timer_t *timer)
{
#if DEBUG_PERFORMANCE
    long long ticks1 = el_ticks_get();
    lv_task_handler();
    if (el_ticks_get() - ticks1 > 2) printf("lv handler used:%lld\n", el_ticks_get() - ticks1);
#else
    lv_task_handler();
#endif

    el_timer_start(timer);
}

static void timer_watch_cb(el_timer_t *timer)
{
    heap_caps_print_heap_info(MALLOC_CAP_8BIT | MALLOC_CAP_INTERNAL);
    heap_caps_print_heap_info(MALLOC_CAP_8BIT | MALLOC_CAP_SPIRAM);
    el_timer_start(timer);
}


static void empty_event_handler(void* arg, esp_event_base_t event_base,
                          int32_t event_id, void* event_data)
{

}
void app_main2(void)
{

    nvs_flash_init();
    ESP_ERROR_CHECK(bsp_board_init());

    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());
    esp_netif_create_default_wifi_sta();
    
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));


    esp_event_handler_instance_t instance_any_id;
    esp_event_handler_instance_t instance_got_ip;
    ESP_ERROR_CHECK(esp_event_handler_instance_register(WIFI_EVENT,
                                                        ESP_EVENT_ANY_ID,
                                                        &empty_event_handler,
                                                        NULL,
                                                        &instance_any_id));
    ESP_ERROR_CHECK(esp_event_handler_instance_register(IP_EVENT,
                                                        IP_EVENT_STA_GOT_IP,
                                                        &empty_event_handler,
                                                        NULL,
                                                        &instance_got_ip));
}


extern view_chess_t *view_chess_create2(void);
void app_main(void)
{
    nvs_flash_init();
    ESP_ERROR_CHECK(bsp_board_init());

    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());
    esp_netif_create_default_wifi_sta();
    
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
    ESP_ERROR_CHECK(esp_wifi_start());

    esp_event_handler_instance_t instance_any_id;
    esp_event_handler_instance_t instance_got_ip;
    ESP_ERROR_CHECK(esp_event_handler_instance_register(WIFI_EVENT,
                                                        ESP_EVENT_ANY_ID,
                                                        &event_handler,
                                                        NULL,
                                                        &instance_any_id));
    ESP_ERROR_CHECK(esp_event_handler_instance_register(IP_EVENT,
                                                        IP_EVENT_STA_GOT_IP,
                                                        &event_handler,
                                                        NULL,
                                                        &instance_got_ip));


    ESP_ERROR_CHECK(lv_port_init());
    lv_group_t * g = lv_group_create();
    lv_group_set_default(g);
    lv_theme_t * th = lv_theme_basic_init(lv_disp_get_next(NULL));
    //LV_FONT_DECLARE(courier_24);
    //lv_theme_t * th = lv_theme_mono_init(lv_disp_get_next(NULL), 0, &courier_24);
    lv_disp_set_theme(lv_disp_get_next(NULL), th);
    bsp_lcd_set_backlight(true);

    printf("App begin\n");

    el_looper_init(&looper);
    el_timer_set_runloop(&looper);

    view_wifi_list_create();
    //view_login_create();

    el_timer_t *lv_timer = el_timer_create(10, timer_lv_cb);
    el_timer_start(lv_timer);
    el_timer_t *watch_timer = el_timer_create(10000, timer_watch_cb);
    el_timer_start(watch_timer);

    ps2_gpio_install_isr(&looper);
    ps2_gpio_set_notify(term_key_notify_cb);
    el_looper_run(&looper, 1);
}
