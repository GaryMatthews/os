#include <uart.h>
#include <multiboot.h>
#include <multiboot2.h>
#include <printf.h>
#include <io.h>

static const char *e820names[] = {
    "invalid",
    "available",
    "reserved",
    "acpi",
    "nvs",
    "unusable"
};

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
  
  exit(0);
}
