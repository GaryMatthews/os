#include <console.h>
#include <video.h>
#include <window.h>

#include <stddef.h>
#include <string.h>

static size_t rows  = 0;
static size_t cols  = 0;
static size_t cur_x = 0;
static size_t cur_y = 0;

static size_t *fb = NULL;
static size_t fb_height;
static size_t fb_width;

uint8_t console_initialized = 0;

window_t *window;
text_area_t *text_area;

void console_init() {
    fb = (size_t *) vbemem.buffer;
    fb_height = vbemem.yres;
    fb_width  = vbemem.xres;
    rows = fb_height / 16;
    cols = fb_width  / 8;

    window = window_create("Console", 0, 16, 640, 460);
    text_area = create_text_area(0, 0, 624, 428);
    add_component(window, text_area);
    text_area_set_text(text_area, "$ ");

    console_initialized = 1;
}

static void scroll() {
    for (size_t i = 0; i < fb_height-16; i++) {
        for (size_t j = 0; j < fb_width; j++) {
            fb[(i*fb_width) + j] = fb[((i+16) * fb_width) + j];
        }
    }
    for (size_t i = fb_height-16; i < fb_height; i++) {
        for (size_t j = 0; j < fb_width; j++) {
            fb[(i*fb_width) + j] = 0;
        }
    }
}

void console_putc(char ch) {
    if (!console_initialized) return;
    
    if (cur_y >= cols) {
        cur_x++;
        cur_y = 0;
    }
    if (cur_x >= rows) {
        scroll();
        cur_x = rows-1;
        cur_y = 0;
    }
    if (ch == '\n' || ch == '\r') {
        cur_x++;
        cur_y = 0;
        if (cur_x >= rows) {
            scroll();
            cur_x = rows-1;
        }
        return;
    }
    char buf[2];
    buf[0] = ch; buf[1] = 0;
    draw_string(cur_x*16, cur_y*8, buf);
    cur_y++;
}
