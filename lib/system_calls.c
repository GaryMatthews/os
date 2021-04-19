#include <lib/string.h>
#include <lib/system_calls.h>

void system(char *arg) {
    if(strcmp(arg, "clear") == 0) {
        syscall_call(2);
    }
}

char *pwd() {
    return (char *) syscall_call(8);
}

void end_process_return() {
    asm volatile("mov %eax, %ebx");
    syscall_call(5);
}

void *syscall_call(int n) {
    void *ret;
    asm volatile("mov %0, %%eax; \
	              int $0x72" : : "a" (n));
    asm volatile("mov %%eax, %0" : "=r" (ret));
    return ret;
}
