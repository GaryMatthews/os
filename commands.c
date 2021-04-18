#include <console.h>
#include <string.h>
#include <mm.h>
#include <kheap.h>

void print_meminfo() {
    console_print("Total mem: %d MB\nFree mem: %d MB\n",
           get_mem_size() / 1024, (get_max_blocks() - get_used_blocks()) * 4 / 1024);
    console_print("Heap size: %d KB Free heap: %d KB\n",
           get_heap_size() / 1024, (get_heap_size() - get_used_heap()) / 1024);
    console_print("cr0: %x cr2: %x cr3: %x\n", get_cr0(), get_cr2(), get_pdbr());
}

void console_exec(char *buf) {
    if (strcmp(buf, "help") == 0) {
        console_print("Help:\n\nhelp - shows help\nmem - prints RAM info\n");
    } else if(strcmp(buf, "mem") == 0) {
        print_meminfo();
    } else {
        console_print("Command not found\n");
    }
}
