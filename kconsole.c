#include <stdint.h>

#include <kconsole.h>
#include <graphics.h>

void _putchar(char c) {
    kconsole->write(kconsole, &c, 1);
    char buf[2];
    buf[0] = c;
    buf[1] = 0;
    write_log(buf);
}
