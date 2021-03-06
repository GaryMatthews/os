#pragma once

#include <types.h>

#define MAX_INTERRUPTS 256

struct idt_info {
    uint16_t base_low;
    uint16_t sel;
    uint8_t reserved;
    uint8_t flags;
    uint16_t base_high;
} __attribute__((__packed__));

struct idt_ptr {
    uint16_t limit;
    uint32_t base;
} __attribute__((__packed__));

void idt_init(uint16_t code);
void default_ir_handler();
void install_ir(uint32_t i, uint16_t flags, uint16_t sel, void *irq);
extern void idt_set(struct idt_ptr *ptr);
extern void gen_int(uint32_t code);

