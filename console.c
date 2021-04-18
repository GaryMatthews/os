#include <console.h>
#include <video.h>
#include <window.h>
#include <kheap.h>
#include <stddef.h>
#include <stdarg.h>
#include <string.h>
#include <keyboard.h>
#include <printf.h>

static size_t rows  = 0;
static size_t cols  = 0;
static size_t cur_x = 0;
static size_t cur_y = 0;

static size_t *fb = NULL;
static size_t fb_height;
static size_t fb_width;

uint8_t console_initialized = 0;

window_t *window;
text_area_t *text_area;

void console_init() {
    fb = (size_t *) vbemem.buffer;
    fb_height = vbemem.yres;
    fb_width  = vbemem.xres;
    rows = fb_height / 16;
    cols = fb_width  / 8;

    window = window_create("Console", 0, 16, 640, 464);
    text_area = create_text_area(0, 0, 624, 432);
    add_component(window, text_area);

    console_initialized = 1;
}

void console_gui_print(char *str) {
    text_area_append(text_area, str);
}

void console_print(char *buffer, ...) {
    char str[256];
    va_list args;

    va_start(args, buffer);
    vsnprintf(str, 256, buffer, args);
    va_end(args);

    console_gui_print(str);
}

int character_check(char c) {
    if(((int) c >= 32) && ((int) c <= 122)) {
        return 1;
    } else {
        return 0;
    }
}

void console_run_gui() {
    char *buffer = kmalloc(64);
    char c = 0;

    console_print("$ ");
    int buffer_counter = 0;
    while(1) {
        c = keyboard_get_lastkey();
        if(c == 0)
            continue;
        keyboard_invalidate_lastkey();
        if(character_check(c)) {
            buffer[buffer_counter++] = c;
        } else if(c == '\b') { // Backspace
            if(buffer_counter > 0) {
                buffer_counter--;
            } else {
                continue;
            }
        }

        console_print("%c", c);
        if(c == '\n') { // Enter pressed
            buffer[buffer_counter] = '\0';
            console_exec(buffer);
            buffer_counter = 0;
            console_print("$ ");
        }
    }
    kfree(buffer);
}
