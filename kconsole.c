#include <stdint.h>

#include <kconsole.h>
#include <console.h>

void _putchar(char c) {
    kconsole->write(kconsole, &c, 1);
    console_putc(c);
}
