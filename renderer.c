#include <renderer.h>

#include <video.h>

void r_init(void) {
}


#define RGB(r, g, b) (((uint32_t) r) << 16) | (((uint32_t) g) << 8) | ((uint32_t) b)

void r_draw_rect(mu_Rect rect, mu_Color color) {
    draw_rect(rect.x, rect.y, rect.w, rect.h, RGB(color.r, color.g, color.b));
}

void r_draw_text(const char *text, mu_Vec2 pos, mu_Color color) {
    (void)color;
    draw_string(pos.x, pos.y, text, RGB(color.r, color.g, color.b));
}


void r_draw_icon(int id, mu_Rect rect, mu_Color color) {
    int c = 0, w = 0, h = 0;
    mu_Vec2 pos;
    char buf[2];
    switch (id) {
    case MU_ICON_CLOSE: c = 'x'; break;
    case MU_ICON_CHECK: c = 'X'; break;
    case MU_ICON_COLLAPSED: c = '>'; break;
    case MU_ICON_EXPANDED: c = 'v'; break;
    }
    buf[0] = c; buf[1] = 0;
    w = r_get_text_width(buf, 1);
    h = r_get_text_height();
    pos.x = rect.x + (rect.w - w) / 2;
    pos.y = rect.y + (rect.h - h) / 2;
    r_draw_text(buf, pos, color);
}


int r_get_text_width(const char *text, int len) {
    (void)text;
    return 8*len;
}


int r_get_text_height(void) {
    return 16;
}


void r_set_clip_rect(mu_Rect rect) {
    (void)rect;
    //draw_rect(rect.x, rect.y, rect.w, rect.h, 0xffffff);
}


void r_clear(mu_Color clr) {
    (void)clr;
}


void r_present(void) {
}
