#pragma once

#include <stdint.h>

void console_init();
void console_putc(char c);
void console_text(uint32_t x, uint32_t y, char *text);
void console_paint();
