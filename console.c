#include <console.h>
#include <video.h>

#include <stddef.h>

#define SSFN_NOIMPLEMENTATION
#define SSFN_CONSOLEBITMAP_TRUECOLOR
#include "ssfn.h"

extern ssfn_font_t _binary_unifont_sfn_start;

static size_t rows  = 0;
static size_t cols  = 0;
static size_t cur_x = 0;
static size_t cur_y = 0;

static size_t *fb = NULL;
static size_t fb_height;
static size_t fb_width;

uint8_t console_initialized = 0;

void console_init() {
    fb = (size_t *) vbemem.buffer;
    fb_height = vbemem.yres;
    fb_width  = vbemem.xres;
    rows = fb_height / 16;
    cols = fb_width  / 8;

    ssfn_font = &_binary_unifont_sfn_start;
    ssfn_dst_ptr = (uint8_t *)vbemem.buffer;
    ssfn_dst_pitch = vbemem.pitch;
    ssfn_fg = 0xFFFFFF;
    ssfn_x = 0;
    ssfn_y = 0;

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
    ssfn_y = cur_x*16;
    ssfn_x = cur_y*8;
    cur_y++;
    ssfn_putc(ch);
}
