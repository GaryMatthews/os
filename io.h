#pragma once

#include <stdint.h>

extern uint8_t inportb(uint16_t port);
extern uint16_t inportw(uint16_t port);
extern void outportb(uint16_t port, uint8_t val);
extern void halt();
void exit(const int exit_status);
void enable_int();
void disable_int();
