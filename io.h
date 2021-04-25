#pragma once

#include <types.h>

extern uint8_t inportb(uint16_t port);
extern uint32_t inportl(uint32_t port);
extern uint16_t inportw(uint16_t port);
extern void outportb(uint16_t port, uint8_t val);
extern void outportl(uint32_t port, uint32_t value);
extern void halt();
void exit_qemu(const int exit_status);
void enable_int();
void disable_int();
void sleep(int s);
