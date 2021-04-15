#include <graphics.h>

#include <video.h>
#include <mouse.h>

void paint_mouse() {
    draw_rect(get_mouse_info()->x, get_mouse_info()->y, 5, 5, 0xff0000);
    draw_pixel(get_mouse_info()->x, get_mouse_info()->y, 0xffffff);
}

void paint_desktop() {
    static uint32_t i, j, noise, carry, seed = 0xbeef;
    
    draw_rect(0, 0, 640, 480, 0x2C2C2C);

#define RGB(r, g, b) (((uint32_t) r) << 16) | (((uint32_t) g) << 8) | ((uint32_t) b)
    for (i = 0; i < 640; ++i) {
        for (j = 0; j < 480; ++j) {
            noise = seed;
            noise >>= 3;
            noise ^= seed;
            carry = noise & 1;
            noise >>= 1;
            seed >>= 1;
            seed |= (carry << 30);
            noise &= 0xFF;
            draw_pixel(i, j, RGB(noise, noise, noise));
        }
    }
    
    paint_mouse();
}


