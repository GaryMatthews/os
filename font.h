#pragma once

#include <stdint.h>

#define FNT_FONTHEIGHT 8
#define FNT_FONTWIDTH 8

uint8_t * get_font_array();

void draw_text(char * text, int start_x, int start_y, uint32_t color);
