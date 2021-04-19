#include <lib/string.h>
#include <mm.h>
#include <kheap.h>
#include <printf.h>
#include <sched.h>

void print_meminfo() {
    printf("Total mem: %d MB\nFree mem: %d MB\n",
           get_mem_size() / 1024, (get_max_blocks() - get_used_blocks()) * 4 / 1024);
    printf("Heap size: %d KB Free heap: %d KB\n",
           get_heap_size() / 1024, (get_heap_size() - get_used_heap()) / 1024);
    printf("cr0: %x cr2: %x cr3: %x\n", get_cr0(), get_cr2(), get_pdbr());
}

void console_exec(char *buf) {
    if (strcmp(buf, "help") == 0) {
        printf("help - shows help\nmem  - prints RAM info\nps   - prints process information\n");
    } else if(strcmp(buf, "mem") == 0) {
        print_meminfo();
    } else if(strcmp(buf, "ps") == 0) {
        print_procs();
    } else {
        printf("Command '%s' not found.\n", buf);
    }
}
