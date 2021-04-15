#include <graphics.h>

#include <video.h>
#include <mouse.h>

void paint_mouse() {
    draw_rect(get_mouse_info()->x, get_mouse_info()->y, 5, 5, 0xcccccc);
    draw_pixel(get_mouse_info()->x, get_mouse_info()->y, 0xffffff);
}

void paint_desktop() {
    draw_rect(0, 0, 640, 480, 0x2C2C2C);
    paint_mouse();
}


