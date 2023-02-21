
#include "ps2.h"
static void ps2_clk_isr(void *arg);
static void *msg_looper;

void ps2_gpio_install_isr(void *looper)
{
    msg_looper = looper;
    gpio_config_t conf_clk = {
        .pin_bit_mask = 1ull << PS2_PIN_CLK,
        .mode = GPIO_MODE_INPUT,
        .pull_up_en = GPIO_PULLUP_ENABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_NEGEDGE,
    };
    gpio_config(&conf_clk);
    gpio_install_isr_service(ESP_INTR_FLAG_LEVEL1);
    gpio_isr_handler_add(PS2_PIN_CLK, ps2_clk_isr, &looper);

    gpio_config_t conf = {
        .pin_bit_mask = 1ull << PS2_PIN_DATA,
        .mode = GPIO_MODE_INPUT,
        .pull_up_en = GPIO_PULLUP_ENABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
    };
    gpio_config(&conf);
}

static void ps_event_free_cb(void *ptr)
{
    el_event_t *event = ptr;
    key_event_t *key = event->payload.user_data1;
    key->free_cb_bak(ptr);
    el_mem_free(key);
}
static void ps_event_fire_cb(el_event_t *event);

static void ps2_clk_isr(void *arg)
{
    static int ticks = 0;
    int new_ticks = el_ticks_get();

    keyboard_t *kb = &kb_one;
    if (new_ticks - ticks > 250)
    {
        kb->bit_id = kb->start_bit = kb->check_bit = \
            kb->stop_bit = kb->byte_id = kb->prefix = \
            kb->e_code = kb->code = 0;
    }
    ticks = new_ticks;
    kb->bit_cnt++;

#if 1
    if (kb->bit_id == 0)
    {
        kb->start_bit = gpio_get_level(PS2_PIN_DATA);
    }
    else if (kb->bit_id == 9)
    {
        kb->check_bit = gpio_get_level(PS2_PIN_DATA);
    }
    else if (kb->bit_id == 10)
    {
        kb->stop_bit = gpio_get_level(PS2_PIN_DATA);
    }
    else
    {
        kb->code |= gpio_get_level(PS2_PIN_DATA) << (kb->bit_id - 1);
    }
    kb->bit_id++;
    if (kb->bit_id >= 11)
    {
        int scan_over = 0;
        if (kb->byte_id == 0)
        {
            if (kb->code == 0xf0)
            {
                kb->prefix = kb->code;
                kb->e_code = 0;
            }
            else if (kb->code == 0xe0)
            {
                kb->e_code = kb->code;
                kb->prefix = 0;
            }
            else
            {
                kb->prefix = kb->e_code = 0;
                scan_over = 1;
            }
        }
        else if (kb->byte_id == 1)
        {
            if (kb->code == 0xf0)
            {
                kb->prefix = kb->code;
            }
            else if (kb->code == 0xe0)
            {
                kb->e_code = kb->code;
            }
            else
            {
                scan_over = 1;
            }
        }
        else
        {
            scan_over = 1;
        }

        kb->byte_id++;

        if (scan_over)
        {
            kb->byte_id = 0;

            el_event_t *event = el_event_create(ps_event_fire_cb);
            key_event_t *key = el_mem_malloc(sizeof(key_event_t));
            event->payload.user_data1 = key;
            key->free_cb_bak = event->free_cb;
            event->free_cb = ps_event_free_cb;
            key->prefix = kb->prefix;
            key->e_code = kb->e_code;
            key->code = kb->code;

            el_looper_post_event(msg_looper, event);
        }

        kb->code = 0;
        kb->bit_id = 0;
    }
#endif
}

static ps2_key_notify_cb_t notify_cb = NULL;

static void ps_event_fire_cb(el_event_t *event)
{
    const struct ps_code_ascii_map_t *map;
    key_event_t *key = event->payload.user_data1;
    uint16_t code_mix;
    printf("PS2 ");

    int id = 0;
    if (kb_one.ctrl)
    {
        printf("CTRL");
        id++;
    }
    if (kb_one.shift)
    {
        if (id) printf("/");
        printf("SHIFT");
        id++;
    }
    if (kb_one.alt)
    {
        if (id) printf("/");
        printf("ALT");
        id++;
    }
    if (id) printf(" + ");

    map = ps2_get_ascii_code(key);
    if (map)
    {
        code_mix = map->code;
        printf("[%d]%c", kb_one.shift, kb_one.shift ? map->shift_ascii : map->ascii);
    }
    else
    {
        code_mix = ps2_ctrl_modifier(key);
        if (code_mix == PS2_KEY_ENTER)
        {
            printf("ENTER");
        }
        else if (code_mix == PS2_KEY_BAR)
        {
            printf("BACKTR");
        }
        else if (code_mix == PS2_KEY_ARRUP)
        {
            printf("ARRORUP");
        }
        else if (code_mix == PS2_KEY_LCTRL)
        {
            printf("LEFT CTRL");
        }
        else if (code_mix == PS2_KEY_LSHFT)
        {
            printf("LEFT SHIFT");
        }
        else
        {
            printf("[KEY]%d", code_mix);
        }
    }

    if (key->prefix)
    {
        printf(" released\n");
    }
    else
    {
        printf(" pressed\n");
    }
    if (notify_cb)
    {
        notify_cb(key, map, code_mix);
    }
}

void ps2_gpio_set_notify(ps2_key_notify_cb_t cb)
{
    notify_cb = cb;
}

const struct ps_code_ascii_map_t _code_map[] = 
{
    {PS2_KEY_A, 'a', 'A'},
    {PS2_KEY_B, 'b', 'B'},
    {PS2_KEY_C, 'c', 'C'},
    {PS2_KEY_D, 'd', 'D'},
    {PS2_KEY_E, 'e', 'E'},
    {PS2_KEY_F, 'f', 'F'},
    {PS2_KEY_G, 'g', 'G'},
    {PS2_KEY_H, 'h', 'H'},
    {PS2_KEY_I, 'i', 'I'},
    {PS2_KEY_J, 'j', 'J'},
    {PS2_KEY_K, 'k', 'K'},
    {PS2_KEY_L, 'l', 'L'},
    {PS2_KEY_M, 'm', 'M'},
    {PS2_KEY_N, 'n', 'N'},
    {PS2_KEY_O, 'o', 'O'},
    {PS2_KEY_P, 'p', 'P'},
    {PS2_KEY_Q, 'q', 'Q'},
    {PS2_KEY_R, 'r', 'R'},
    {PS2_KEY_S, 's', 'S'},
    {PS2_KEY_T, 't', 'T'},
    {PS2_KEY_U, 'u', 'U'},
    {PS2_KEY_V, 'v', 'V'},
    {PS2_KEY_W, 'w', 'W'},
    {PS2_KEY_X, 'x', 'X'},
    {PS2_KEY_Y, 'y', 'Y'},
    {PS2_KEY_Z, 'z', 'Z'},
    {PS2_KEY_0, '0', ')'},
    {PS2_KEY_1, '1', '!'},
    {PS2_KEY_2, '2', '@'},
    {PS2_KEY_3, '3', '#'},
    {PS2_KEY_4, '4', '$'},
    {PS2_KEY_5, '5', '%'},
    {PS2_KEY_6, '6', '^'},
    {PS2_KEY_7, '7', '&'},
    {PS2_KEY_8, '8', '*'},
    {PS2_KEY_9, '9', '('},
    {PS2_KEY_SPAC, ' ', ' '},
    {PS2_KEY_WAV,  '`', '~'},
    {PS2_KEY_TAB,  '	', '	'},
    {PS2_KEY_BAR,  '-', '_'},
    {PS2_KEY_EQU,  '=', '+'},
    {PS2_KEY_LBRK, '[', '{'},
    {PS2_KEY_RBRK, ']', '}'},
    {PS2_KEY_PIPE, '\\', '|'},
    {PS2_KEY_SEMI, ';', ':'},
    {PS2_KEY_QUOT, '\'', '"'},
    {PS2_KEY_COMM, ',', '<'},
    {PS2_KEY_DOT,  '.', '>'},
    {PS2_KEY_RSPL, '/', '?'},
};

const struct ps_code_ascii_map_t *code_map = _code_map;
keyboard_t kb_one = {0};

const struct ps_code_ascii_map_t *ps2_get_ascii_code(key_event_t *event)
{
    const struct ps_code_ascii_map_t *result = NULL;
    int i;
    for (i = 0; i < sizeof(_code_map) / sizeof(_code_map[0]); i++)
    {
        result = &code_map[i];
        if (result->code == event->code)
        {
            return result;
        }
    }
    return NULL;
}

uint16_t ps2_ctrl_modifier(key_event_t *event)
{
    keyboard_t *kb = &kb_one;
    int is_ctrl = 0;
    int is_shift = 0;
    int is_alt = 0;

    uint16_t code = event->e_code;
    code <<= 8;
    code |= event->code;

    if (code == PS2_KEY_LCTRL || code == PS2_KEY_RCTRL)
        is_ctrl = 1;
    else 
    if (code == PS2_KEY_LSHFT || code == PS2_KEY_RSHFT)
        is_shift = 1;
    else 
    if (code == PS2_KEY_LALT || code == PS2_KEY_RALT)
        is_alt = 1;

    //printf("modifer prefix:%x is_sht:%d\n", event->prefix, is_shift);
    if (event->prefix)
    {
        if (is_ctrl) kb->ctrl = 0;
        if (is_shift) kb->shift = 0;
        if (is_alt) kb->alt = 0;
    }
    else
    {
        if (is_ctrl) kb->ctrl = 1;
        if (is_shift) kb->shift = 1;
        if (is_alt) kb->alt = 1;
    }
    return code;
}
