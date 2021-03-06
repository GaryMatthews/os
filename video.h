#pragma once

#include <types.h>

void vbe_init();
void refresh_screen();
void draw_pixel(int x, int y, uint32_t color);
void draw_rect(int x, int y, int w, int h, uint32_t color);
void draw_string(uint32_t x, uint32_t y, const char *text, uint32_t color);
void draw_data_with_alfa(uint32_t* data, uint32_t width, uint32_t height, uint32_t x, uint32_t y);
void draw_line(int x0, int y0, int x1, int y1, uint32_t color);

struct vbe_mem {
    uint32_t buffer_size;
    uint32_t *mem;
    uint32_t *buffer;
    uint16_t xres;
    uint16_t yres;
    uint8_t bpp;
    uint16_t pitch;
};

extern struct vbe_mem vbemem;
