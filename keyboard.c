#include <keyboard.h>
#include <io.h>
#include <idt.h>
#include <pit.h>
#include <printf.h>

enum KBD_PORTS {
	KBD_CHECK = 0x64,
	KBD_IN = 0x60,
};

static uint8_t lastkey = 0;
/*
static uint8_t caps_lock = 0;
static uint8_t num_pad = 0;
static uint8_t scroll_led = 0;

static uint8_t ctrl = 0;
static uint8_t shift = 0;
static uint8_t alt = 0;*/

extern void keyboard_int();

void keyboard_init() {
    install_ir(33, 0x80 | 0x0E, 0x8, &keyboard_int);
    outportb(KBD_CHECK, 0xAE);
}

void keyboard_read_key() {
    lastkey = 0;
    if(inportb(KBD_CHECK) & 1) {
        lastkey = keyboard_to_ascii(inportb(KBD_IN));
	}
}

char keyboard_get_lastkey() {
    return lastkey;
}

void keyboard_invalidate_lastkey() {
    lastkey = 0;
}

static char* qwertzuiop = "qwertzuiop"; // 0x10-0x1c
static char* asdfghjkl = "asdfghjkl";
static char* yxcvbnm = "yxcvbnm";
static char* num = "123456789";
uint8_t keyboard_to_ascii(uint8_t key){
	if(key == 0x1C) return '\n';
	if(key == 0x39) return ' ';
	if(key == 0xE) return '\b';
	if(key == POINT_RELEASED) return '.';
	if(key == SLASH_RELEASED) return '/';
	if(key == ZERO_PRESSED) return '0';
	if(key == BACKSLASH_PRESSED) return '\\';
	if(key >= ONE_PRESSED && key <= NINE_PRESSED)
		return num[key - ONE_PRESSED];
	else if(key >= 0x10 && key <= 0x1C)
		return qwertzuiop[key - 0x10];
	else if(key >= 0x1E && key <= 0x26)
		return asdfghjkl[key - 0x1E];
	else if(key >= 0x2C && key <= 0x32)
		return yxcvbnm[key - 0x2C];
	return 0;
}

void keyboard_set_leds(int num, int capslock, int scroll) {
    uint8_t cmd = 0;
    cmd = (scroll) ? (cmd | 1) : (cmd & 1);
    cmd = (num) ? (cmd | 2) : (cmd & 2);
    cmd = (capslock) ? (cmd | 4) : (cmd & 4);
    outportb(KBD_IN, 0xED);
    outportb(KBD_IN, cmd);
}

char getchar() {
    enable_int();
    char c = 0;
    while(1) {
        c = keyboard_get_lastkey();
        if(c == 0)
            continue;
        keyboard_invalidate_lastkey();
        return c;
    }
}

void gets(char *str) {
    int count = 0;
    char c;

    enable_int();
    sched_state(0);
    while(1) {
        c = keyboard_get_lastkey();
        if(c == 0)
            continue;
        keyboard_invalidate_lastkey();
        if(((int) c >= 32) && ((int) c <= 122))
            str[count++] = c;
        else if(c == '\b')
            if(count > 0)
                count--;
        printf("%c", c);
        if(c == '\n') {
            str[count] = '\0';
            break;
        }
    }
    sched_state(1);
}
