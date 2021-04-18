#pragma once

#include <stdint.h>

void vbe_init();
void refresh_screen();
void draw_pixel(int x, int y, uint32_t color);
void draw_rect(int x, int y, int w, int h, uint32_t color);
void draw_string(uint32_t x, uint32_t y, char *text);

struct vbe_mem {
    uint32_t buffer_size;
    uint32_t *mem;
    uint32_t *buffer;
    uint16_t xres;
    uint16_t yres;
    uint8_t bpp;
    uint16_t pitch;
};

struct vbe_mem vbemem;
