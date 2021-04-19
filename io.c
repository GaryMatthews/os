#include <io.h>
#include <pit.h> // for get_tick_count()

uint8_t inportb(uint16_t port) {
    uint8_t ret;
    __asm__ volatile("inb %%dx, %%al" : "=a" (ret) : "d" (port));
    return ret;
}

uint16_t inportw(uint16_t port) {
    uint16_t ret;
    __asm__ volatile("inw %%dx, %%ax" : "=a" (ret) : "d" (port));
    return ret;
}

void outportb(uint16_t port, uint8_t val) {
    __asm__ volatile("outb %%al, %%dx" : : "d" (port), "a" (val));
}

void halt() {
    __asm__ volatile("hlt");
}

void exit_qemu(const int status_code) {
  if (status_code) {
    outportb(0xf4, status_code); // qemu isa-debug-exit port
  }
  __asm__ volatile ("cli");
  while ((inportb(0x64) & 2) != 0);
  outportb(0x64, 0xd1);
  while ((inportb(0x64) & 2) != 0);
  outportb(0x60, 0xfe); // keyboard reset
}

void enable_int() {
    __asm__ volatile("sti");
}

void disable_int() {
    __asm__ volatile("cli");
}

void sleep(int s) {
    // TODO better solution
    int ticks = get_tick_count() + s;
    int passed = 0;
    while((passed += get_tick_count()) < ticks);
}
