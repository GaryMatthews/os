#include <io.h>
#include <pit.h> // for get_tick_count()
#include <printf.h>
#include <log.h>

uint8_t inportb(uint16_t port) {
    uint8_t ret;
    __asm__ volatile("inb %%dx, %%al" : "=a" (ret) : "d" (port));
    return ret;
}

uint32_t inportl(uint32_t port) {
    uint32_t ret;
    __asm__ volatile("inl %%dx,%%eax":"=a" (ret):"d"(port));
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

void outportl(uint32_t port, uint32_t val) {
    __asm__ volatile("outl %%eax,%%dx"::"d" (port), "a" (val));
}

void halt() {
    __asm__ volatile("hlt");
}

void exit_qemu(const int status_code) {
  if (status_code) {
    outportb(0xf4, status_code); // qemu isa-debug-exit port
  }
  disable_int();
  while ((inportb(0x64) & 2) != 0);
  outportb(0x64, 0xd1);
  while ((inportb(0x64) & 2) != 0);
  outportb(0x60, 0xfe); // keyboard reset
}

void enable_int() {
    klogf(LOG_INFO, "Interrupts are enabled.\n");
    __asm__ volatile("sti");
}

void disable_int() {
    klogf(LOG_INFO, "Interrupts are disabled.\n");
    __asm__ volatile("cli");
}

void sleep(int s) {
    //for (int i = 0; i < s * 1000000; i++) { } return;
    //XXX if (get_tick_count() == 0) return;
    // TODO better solution
    int ticks = get_tick_count() + s;
    int passed = 0;
    while((passed += get_tick_count()) < ticks) {
        halt();
        //printf("sleep %d ...\n", passed);
    }
}
