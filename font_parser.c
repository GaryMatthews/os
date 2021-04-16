#include <font.h>
#include <string.h>
#include <printf.h>
#include <video.h>

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
                        draw_pixel(((col - 1) * FNT_FONTWIDTH) + x, (y + row * FNT_FONTHEIGHT), color);
                    } else {
                        draw_pixel(((col - 1) * FNT_FONTWIDTH) + x, (y + row * FNT_FONTHEIGHT), 0x0);
                    }
                }
            }
        }
    }

}
