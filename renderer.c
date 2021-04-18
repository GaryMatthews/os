#include <renderer.h>

#include <video.h>

void r_init(void) {
}


static void flush(void) {
}

#define RGB(r, g, b) (((uint32_t) r) << 16) | (((uint32_t) g) << 8) | ((uint32_t) b)

void r_draw_rect(mu_Rect rect, mu_Color color) {
    draw_rect(rect.x, rect.y, rect.w, rect.h, RGB(color.r, color.g, color.b));
}

void r_draw_text(const char *text, mu_Vec2 pos, mu_Color color) {
    draw_string(pos.x, pos.y, text);
}


void r_draw_icon(int id, mu_Rect rect, mu_Color color) {
    draw_rect(rect.x, rect.y, rect.w, rect.h, RGB(color.r, color.g, color.b));
}


int r_get_text_width(const char *text, int len) {
  return 8*len;
}


int r_get_text_height(void) {
  return 16;
}


void r_set_clip_rect(mu_Rect rect) {
    //draw_rect(rect.x, rect.y, rect.w, rect.h, 0xffffff);
}


void r_clear(mu_Color clr) {
}


void r_present(void) {
}
