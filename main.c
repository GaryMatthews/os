#include <uart.h>
#include <multiboot.h>
#include <multiboot2.h>
#include <printf.h>
#include <io.h>
#include <font.h>

#include <gdt.h>
#include <idt.h>
#include <pic.h>
#include <pit.h>

#include <keyboard.h>
#include <mouse.h>

#include <syscall.h>
#include <tss.h>

static const char *e820names[] = {
    "invalid",
    "available",
    "reserved",
    "acpi",
    "nvs",
    "unusable"
};

extern uintptr_t bfb_addr;
extern uint32_t bfb_width;
extern uint32_t bfb_height;
extern uint16_t bfb_bpp;

uint32_t * screen = NULL;

int get_pixel_idx(int x, int y) {
    return bfb_width * y + x;
}

void set_pixel(uint32_t val, int x, int y) {
    screen[get_pixel_idx(x, y)] = val;
}

void draw_rect(uint32_t val, int x, int y, int width, int height) {
    for (int i = y; i < y + height; i++) {
        for (int j = x; j < x + width; j++) {
            set_pixel(val, j, i);
        }
    }
}

void kernel_main(unsigned long magic, unsigned long addr) {
  unsigned size = *(unsigned *) addr;

  uart_init();

  if (magic == MULTIBOOT_LOADER_MAGIC) {
      printf("MultiBoot 1 addr: 0x%x magic: 0x%x size: 0x%x\n",
             (uintptr_t)addr, (unsigned) magic, size);

      multiboot_info_parse(magic, (const multiboot_info_t *)addr);
  } else if (magic == MULTIBOOT2_LOADER_MAGIC) {
      printf("MultiBoot 2 addr: 0x%x magic: 0x%x size: 0x%x\n",
             (uintptr_t)addr, (unsigned) magic, size);

      multiboot2_info_parse(magic, (const multiboot2_info_t *)addr);
  } else {
      printf("Error: no multiboot, magic: 0x%x. Exiting.", magic);
      exit(1);
  }

  for (int i = 0; i < e820counter; i++) {
      struct e820memmap map = e820table[i];

      printf(" BIOS-e820 [addr: 0x%x%x, size: 0x%x%x] %s\n",
             (unsigned) (map.base_address >> 32),
             (unsigned) (map.base_address & 0xffffffff),
             (unsigned) (map.size >> 32),
             (unsigned) (map.size & 0xffffffff),
             e820names[(unsigned) map.type]);
  }

  if (bfb_addr != 0) {
      screen = (uint32_t*)bfb_addr;
      draw_rect(0xff0000, 20, 20, 20, 20);
      draw_rect(0x00ff00, 20+25, 20, 20, 20);
      draw_rect(0x0000ff, 20+50, 20, 20, 20);

      draw_text("Hello, world!", 0, 0, 0xffffff);
  }

  disable_int();
  gdt_init();
  idt_init(0x8);
  pic_init(0x20, 0x28);
  pit_init();
  pit_start_counter(100, PIT_COUNTER_0, PIT_MODE_SQUAREWAVEGEN);

  keyboard_init();
  mouse_init();

  syscall_init();
  install_tss();

  enable_int();
  
  if (bfb_addr != 0) {
      for(;;);
  }

  exit(0);
}
