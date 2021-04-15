#include <sched.h>

#include <printf.h>

uint32_t schedule(uint32_t esp) {
    printf("schedule: 0x%x\n", esp);
    
    return esp;
}
