#include <stdint.h>

#include <kconsole.h>

void _putchar(char c) {
    kconsole->write(kconsole, &c, 1);
}
