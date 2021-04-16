#include <graphics.h>

#include <video.h>
#include <mouse.h>
#include <font.h>

short mouse_icon[] =  {
        1,0,0,0,0,0,0,0,0,0,0,
        1,1,0,0,0,0,0,0,0,0,0,
        1,2,1,0,0,0,0,0,0,0,0,
        1,2,2,1,0,0,0,0,0,0,0,
        1,2,2,2,1,0,0,0,0,0,0,
        1,2,2,2,2,1,0,0,0,0,0,
        1,2,2,2,2,2,1,0,0,0,0,
        1,2,2,2,2,2,2,1,0,0,0,
        1,2,2,2,2,2,2,2,1,0,0,
        1,2,2,2,2,2,2,2,2,1,0,
        1,2,2,2,2,1,1,1,1,1,1,
        1,2,2,2,1,0,0,0,0,0,0,
        1,2,2,1,0,0,0,0,0,0,0,
        1,2,1,0,0,0,0,0,0,0,0,
        1,1,0,0,0,0,0,0,0,0,0,
        1,0,0,0,0,0,0,0,0,0,0,
};

uint32_t mouse_color_mapping[] = {0, 0xFFFFFFFF, 0};

void paint_mouse() {
    short* buf = mouse_icon;
    int mouse_x = get_mouse_info()->x;
    int mouse_y = get_mouse_info()->y;
    for (int i=0; i<16; i++) {
        for (int j=0; j<11; j++) {
            if (*buf) {
                uint32_t color = mouse_color_mapping[*buf];
                if (mouse_x + j >= 0 && mouse_x + j < 640 &&
                    mouse_y + i >= 0 && mouse_y + i < 480) {
                    draw_pixel(mouse_x + j, mouse_y + i, color);
                }
            }
            buf++;
        }
    }
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

    draw_text("Hello, world!", 0, 0, 0xff0000);
    
    paint_mouse();
}


