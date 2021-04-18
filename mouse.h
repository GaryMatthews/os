#pragma once

#include <stdint.h>

#define MOUSE_PORT   0x60
#define MOUSE_STATUS 0x64
#define MOUSE_ABIT   0x02
#define MOUSE_BBIT   0x01
#define MOUSE_WRITE  0xD4
#define MOUSE_F_BIT  0x20
#define MOUSE_V_BIT  0x08

#define LEFT_CLICK      0x1
#define RIGHT_CLICK     0x2
#define MIDDLE_CLICK    0x4

#define MOUSE_LEFT_BUTTON(flag) (flag & 0x1)
#define MOUSE_RIGHT_BUTTON(flag) (flag & 0x2)
#define MOUSE_MIDDLE_BUTTON(flag) (flag & 0x4)

typedef struct mouse_info {
    int x;
    int y;
    volatile uint32_t prev_button;
    volatile uint32_t curr_button;
} mouse_info_t;

void mouse_wait(uint8_t type);
void mouse_write(uint8_t write);
uint8_t mouse_read();
mouse_info_t *get_mouse_info();
void mouse_handler();
void mouse_check_bounds();
void mouse_init();

int mouse_left_button_down();
int mouse_left_button_up();
int mouse_right_button_down();
int mouse_right_button_up();

mouse_info_t mouse_info;

