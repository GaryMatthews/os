#include <font.h>
#include <string.h>
#include <printf.h>

extern uint32_t * screen;
extern uint32_t bfb_width;

extern void set_pixel(uint32_t val, int x, int y);

void draw_text(char * text, int start_x, int start_y, uint32_t color) {
    unsigned i, len;
    uint32_t x, y, col, row, stop;
    uint8_t *fnt, chr;

    len = strlen(text);

    fnt = get_font_array();

    col = start_y;
    row = start_x;
    stop = 0;

    for (i = 0; i < len; i++) {
        switch (text[i]) {
            case '\n':
                row++;
                col = 0;
                chr = 0;
                break;

            case '\r':
                chr = 0;
                break;

            case '\t':
                chr = 0;
                col += 4 - col % 4;
                break;

            case '\0':
                stop = 1;
                chr = 0;
                break;

            default:
                col++;
                chr = text[i];
                break;
        }

        if (stop) {
            break;
        }

        if(chr != 0) {
            for(y = 0; y < FNT_FONTHEIGHT; y++){
                for(x = 0; x < FNT_FONTWIDTH; x++){
                    if(fnt[text[i] * FNT_FONTHEIGHT + y] >> (7 - x) & 1) {
                        //screen[((col - 1) * FNT_FONTWIDTH) + x + (y + row * FNT_FONTHEIGHT) * bfb_width] = 0xffffff00;
                        set_pixel(color, ((col - 1) * FNT_FONTWIDTH) + x, (y + row * FNT_FONTHEIGHT));
                    } else {
                        set_pixel(0x0, ((col - 1) * FNT_FONTWIDTH) + x, (y + row * FNT_FONTHEIGHT));
                    }
                }
            }
        }
    }

}
