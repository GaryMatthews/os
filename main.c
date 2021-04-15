#include <uart.h>
#include <multiboot.h>
#include <multiboot2.h>
#include <printf.h>
#include <io.h>
#include <font.h>

#include <memory.h>

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
extern uint32_t bfb_scanline;

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

extern uint32_t multiboot2_mem_size;

void kernel_main(unsigned long magic, unsigned long addr) {
  unsigned size = *(unsigned *) addr;

  uart_init();

  disable_int();

  if (magic == MULTIBOOT_LOADER_MAGIC) {
      printf("MultiBoot 1 addr: 0x%x magic: 0x%x size: 0x%x\n",
             (uintptr_t)addr, (unsigned) magic, size);

      multiboot_info_parse(magic, (const multiboot_info_t *)addr);

      multiboot_info_t *info = (multiboot_info_t *)addr;

      printf("multiboot1_mem_size = %d\n", info->mem_upper + info->mem_lower);
      
      pmm_init(info->mem_upper + info->mem_lower);
      
  } else if (magic == MULTIBOOT2_LOADER_MAGIC) {
      printf("MultiBoot 2 addr: 0x%x magic: 0x%x size: 0x%x\n",
             (uintptr_t)addr, (unsigned) magic, size);

      multiboot2_info_parse(magic, (const multiboot2_info_t *)addr);

      multiboot2_mem_size = 261631; //XXX

      printf("multiboot2_mem_size = %d\n", multiboot2_mem_size);

      pmm_init(multiboot2_mem_size);
} else {
      printf("Error: no multiboot, magic: 0x%x. Exiting.", magic);
      exit(1);
  }

  for (int i = 0; i < e820counter; i++) {
      struct e820memmap map = e820table[i];

      // Check if the memory region is available
      if (map.type == 1) {
          printf(" BIOS-e820 [addr: 0x%x%x, size: 0x%x%x] %s\n",
                 (unsigned) (map.base_address >> 32),
                 (unsigned) (map.base_address & 0xffffffff),
                 (unsigned) (map.size >> 32),
                 (unsigned) (map.size & 0xffffffff),
                 e820names[(unsigned) map.type]);

          pmm_init_reg(map.base_address & 0xffffffff, map.size & 0xffffffff);
      }
  }

  pmm_init2();

  vmm_init();
  kheap_init();

  uint32_t baddr = (uint32_t) bfb_addr;
  uint32_t buffer_size = bfb_width*bfb_height*bfb_bpp/8;
  uint32_t baddr_buf = baddr + buffer_size;

  for (int i = 0; i < (int) buffer_size / PAGE_SIZE;
      i++, baddr += PAGE_SIZE, baddr_buf += PAGE_SIZE) {
      vmm_map(get_kern_directory(), baddr_buf, PAGE_PRESENT | PAGE_RW);
      vmm_map_phys(get_kern_directory(), baddr, baddr, PAGE_PRESENT | PAGE_RW);
  }

  screen = (uint32_t*)bfb_addr;

  if (bfb_addr != 0) {
      draw_rect(0xff0000, 120, 20, 20, 20);
      draw_rect(0x00ff00, 120+25, 40, 20, 20);
      draw_rect(0x0000ff, 120+50, 60, 20, 20);
  }

  int y = 0;

  draw_text("gdt       ", y++, 0, 0xffffff);
  gdt_init();
  draw_text("idt       ", y++, 0, 0xffffff);
  idt_init(0x8);
  draw_text("pic       ", y++, 0, 0xffffff);
  pic_init(0x20, 0x28);
  draw_text("pit       ", y++, 0, 0xffffff);
  pit_init();
  pit_start_counter(100, PIT_COUNTER_0, PIT_MODE_SQUAREWAVEGEN);

  draw_text("keyboard  ", y++, 0, 0xffffff);
  keyboard_init();
  draw_text("mouse     ", y++, 0, 0xffffff);
  mouse_init();
  draw_text("uart      ", y++, 0, 0xffffff);
  uart_rx_ir();

  draw_text("syscalls  ", y++, 0, 0xffffff);
  syscall_init();
  draw_text("tss       ", y++, 0, 0xffffff);
  install_tss();
  draw_text("ready!    ", y++, 0, 0xffffff);

  enable_int();

  while(1) halt();
  
  printf("Exiting.\n");
  exit(0);
}
