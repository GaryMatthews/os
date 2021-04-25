#include <cpu.h>

uint64_t rdtsc() {
    uint64_t result;
    asm volatile("rdtsc" : "=A"(result));
    return result;
}

