#include <video.h>

#include <graphics.h>
#include <memory.h>
#include <string.h>

#include <printf.h>

extern uintptr_t bfb_addr;
extern uint32_t bfb_width;
extern uint32_t bfb_height;
extern uint16_t bfb_bpp;
extern uint32_t bfb_scanline;

void vbe_init() {
    vbemem.buffer_size = bfb_width * bfb_height * (bfb_bpp / 8);
    vbemem.mem = (uint32_t *) bfb_addr;
    vbemem.xres = bfb_width;
    vbemem.yres = bfb_height;
    vbemem.bpp = bfb_bpp;
    vbemem.pitch = bfb_scanline;

    /*
    printf("vbe buffer_size %d, mem 0x%x, %dx%d %d bpp, pitch %d\n",
           vbemem.buffer_size, vbemem.mem, vbemem.xres, vbemem.yres,
           vbemem.bpp, vbemem.pitch);*/

    uint32_t addr = (uint32_t) bfb_addr;
    uint32_t addr_buf = addr + vbemem.buffer_size;
    vbemem.buffer = (uint32_t *) addr_buf;
    //printf("vbe buffer 0x%x\n", vbemem.buffer);
    
    for (int i = 0; i < (int) vbemem.buffer_size / PAGE_SIZE;
         i++, addr += PAGE_SIZE, addr_buf += PAGE_SIZE) {
        vmm_map(get_kern_directory(), addr_buf, PAGE_PRESENT | PAGE_RW);
        vmm_map_phys(get_kern_directory(), addr, addr,
                     PAGE_PRESENT | PAGE_RW);
    }
}

void refresh_screen() {
    paint_desktop();
    memcpy(vbemem.mem, vbemem.buffer, vbemem.buffer_size);
}

void draw_pixel(int x, int y, uint32_t color) {
    if(x < 0 || x > vbemem.xres || y < 0 || y > vbemem.yres)
        return;
    x = x * (vbemem.bpp / 8);
    y = y * vbemem.pitch;

    register uint8_t *pixel = (uint8_t *) ((uint32_t) vbemem.buffer) + x + y;
    pixel[0] = color & 0xFF;
    pixel[1] = (color >> 8) & 0xFF;
    pixel[2] = (color >> 16) & 0xFF;
}

void draw_rect(int x, int y, int w, int h, uint32_t color) {
    if(x < 0 || x > vbemem.xres || y < 0 || y > vbemem.yres)
        return;

    uint8_t r = color & 0xFF;
    uint8_t g = (color >> 8) & 0xFF;
    uint8_t b = (color >> 16) & 0xFF;

    uint8_t *where = (uint8_t *) (((uint32_t) vbemem.buffer) + (x * (vbemem.bpp / 8)) + (y * vbemem.pitch));
    uint32_t row = vbemem.xres * (vbemem.bpp / 8);

    for(int i = 0; i < h; i++) {
        for(int j = 0; j < w; j++) {
            where[j * 4] = r;
            where[j * 4 + 1] = g;
            where[j * 4 + 2] = b;
        }
        where += row;
    }
}