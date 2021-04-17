#include <assert.h>

#include <kconsole.h>
#include <console.h>
#include <log.h>
#include <uart.h>
#include <multiboot.h>
#include <multiboot2.h>
#include <printf.h>
#include <io.h>

#include <memory.h>

#include <video.h>

#include <gdt.h>
#include <idt.h>
#include <pic.h>
#include <pit.h>

#include <keyboard.h>
#include <mouse.h>

#include <syscall.h>
#include <tss.h>

#include <sched.h>

#include <rtc.h>
#include <pcspk.h>

static const char *e820names[] = {
    "invalid",
    "available",
    "reserved",
    "acpi",
    "nvs",
    "unusable"
};

extern uint32_t multiboot2_mem_size;
extern datetime_t current_datetime;

void kernel_main(unsigned long magic, unsigned long addr) {
  unsigned size = *(unsigned *) addr;

  uart_init();
  kconsole = &uartdev;

  disable_int();

  if (magic == MULTIBOOT_LOADER_MAGIC) {
      /*klogf(LOG_INFO, "MultiBoot 1 addr: 0x%x magic: 0x%x size: 0x%x\n",
        (uintptr_t)addr, (unsigned) magic, size);*/

      multiboot_info_parse(magic, (const multiboot_info_t *)addr);

      multiboot_info_t *info = (multiboot_info_t *)addr;

      //klogf(LOG_INFO, "multiboot1_mem_size = %d\n", info->mem_upper + info->mem_lower);
      
      pmm_init(info->mem_upper + info->mem_lower);
      
  } else if (magic == MULTIBOOT2_LOADER_MAGIC) {
      /*klogf(LOG_INFO, "MultiBoot 2 addr: 0x%x magic: 0x%x size: 0x%x\n",
        (uintptr_t)addr, (unsigned) magic, size);*/

      multiboot2_info_parse(magic, (const multiboot2_info_t *)addr);

      //klogf(LOG_INFO, "multiboot2_mem_size = %d\n", multiboot2_mem_size);

      pmm_init(multiboot2_mem_size);
} else {
      klogf(LOG_EMERG, "Error: no multiboot, magic: 0x%x. Exiting.", magic);
      exit(1);
  }

  for (int i = 0; i < e820counter; i++) {
      struct e820memmap map = e820table[i];

      // Check if the memory region is available
      if (map.type == 1) {
          /*klogf(LOG_INFO, " BIOS-e820 [addr: 0x%x%x, size: 0x%x%x] %s\n",
                 (unsigned) (map.base_address >> 32),
                 (unsigned) (map.base_address & 0xffffffff),
                 (unsigned) (map.size >> 32),
                 (unsigned) (map.size & 0xffffffff),
                 e820names[(unsigned) map.type]);*/

          pmm_init_reg(map.base_address & 0xffffffff, map.size & 0xffffffff);
      }
  }

  pmm_init2();

  vmm_init();
  kheap_init();

  vbe_init();
  console_init();

  gdt_init();
  idt_init(0x8);
  pic_init(0x20, 0x28);
  pit_init();
  pit_start_counter(100, PIT_COUNTER_0, PIT_MODE_SQUAREWAVEGEN);

  keyboard_init();
  mouse_init();
  uart_rx_ir();

  syscall_init();
  install_tss();

  rtc_init();
  printf("time: %s\n", datetime_to_str(&current_datetime));
  //beep(400);

  sched_init();
  
  klogf(LOG_INFO, "Enable int.\n");
  enable_int();

  //while(1) halt();

  klogf(LOG_INFO, "Exiting.\n");
  exit(0);
}
