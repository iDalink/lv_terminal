#ifndef __PS2_H__
#define __PS2_H__

#include <string.h>
#include <sys/types.h>
#include <stdio.h>
#include <el_port.h>
#include <el_looper.h>
#include "bsp_board.h"
#include "freertos/FreeRTOS.h"


#define PS2_KEY_A 0x1C
#define PS2_KEY_B 0x32
#define PS2_KEY_C 0x21
#define PS2_KEY_D 0x23
#define PS2_KEY_E 0x24
#define PS2_KEY_F 0x2B
#define PS2_KEY_G 0x34
#define PS2_KEY_H 0x33
#define PS2_KEY_I 0x43
#define PS2_KEY_J 0x3B
#define PS2_KEY_K 0x42
#define PS2_KEY_L 0x4B
#define PS2_KEY_M 0x3A
#define PS2_KEY_N 0x31
#define PS2_KEY_O 0x44
#define PS2_KEY_P 0x4D
#define PS2_KEY_Q 0x15
#define PS2_KEY_R 0x2D
#define PS2_KEY_S 0x1B
#define PS2_KEY_T 0x2C
#define PS2_KEY_U 0x3C
#define PS2_KEY_V 0x2A
#define PS2_KEY_W 0x1D
#define PS2_KEY_X 0x22
#define PS2_KEY_Y 0x35
#define PS2_KEY_Z 0x1A
#define PS2_KEY_0 0x45
#define PS2_KEY_1 0x16
#define PS2_KEY_2 0x1E
#define PS2_KEY_3 0x26
#define PS2_KEY_4 0x25
#define PS2_KEY_5 0x2E
#define PS2_KEY_6 0x36
#define PS2_KEY_7 0x3D
#define PS2_KEY_8 0x3E
#define PS2_KEY_9 0x46


#define PS2_KEY_ENTER 0x5A
#define PS2_KEY_BCKTR 0x66
#define PS2_KEY_ARRUP 0xE075
#define PS2_KEY_ARRLF 0xE06B
#define PS2_KEY_ARRRT 0xE074
#define PS2_KEY_ARRDW 0xE072
#define PS2_KEY_HOME  0xE06C
#define PS2_KEY_END   0xE069
#define PS2_KEY_DLET  0xE071
#define PS2_KEY_INSRT 0xE070
#define PS2_KEY_ESC   0x76
#define PS2_KEY_LCTRL 0x14
#define PS2_KEY_RCTRL 0xE014
#define PS2_KEY_LSHFT 0x12
#define PS2_KEY_RSHFT 0x59
#define PS2_KEY_LALT  0x11
#define PS2_KEY_RALT  0xE011

#define PS2_KEY_SPAC 0x29
#define PS2_KEY_WAV  0x0E
#define PS2_KEY_TAB  0x0D
#define PS2_KEY_BAR  0x4E
#define PS2_KEY_EQU  0x55
#define PS2_KEY_LBRK 0x54
#define PS2_KEY_RBRK 0x58
#define PS2_KEY_PIPE 0x5D
#define PS2_KEY_SEMI 0x4C
#define PS2_KEY_QUOT 0x52
#define PS2_KEY_COMM 0x41
#define PS2_KEY_DOT  0x49
#define PS2_KEY_RSPL 0x4A



const struct ps_code_ascii_map_t
{
    int code;
    char ascii;
    char shift_ascii;
} *code_map;




typedef struct
{
    int shift;
    int ctrl;
    int alt;

    int bit_id;
    uint8_t start_bit;
    uint8_t check_bit;
    uint8_t stop_bit;
    int byte_id;
    uint8_t prefix;
    uint8_t e_code; //extend code
    uint8_t code;
    int bit_cnt;
} keyboard_t;

typedef struct 
{
    el_free_cb_t free_cb_bak;
    el_event_t event;
    uint8_t prefix;
    uint8_t e_code;
    uint8_t code;
} key_event_t;

extern keyboard_t kb_one;
#define PS2_PIN_CLK   GPIO_NUM_38
#define PS2_PIN_DATA  GPIO_NUM_39

void ps2_gpio_install_isr(void *looper);

typedef void (* ps2_key_notify_cb_t)(key_event_t  *event, 
            const struct ps_code_ascii_map_t *map, uint16_t code);

void ps2_gpio_set_notify(ps2_key_notify_cb_t cb);

const struct ps_code_ascii_map_t *ps2_get_ascii_code(key_event_t *event);
uint16_t ps2_ctrl_modifier(key_event_t *event);

#endif
