#pragma once

#include <stdint.h>

void uart_init(void);
void uart_rx_ir(void);
uint8_t uart_tx_empty(void);
void _putchar(char c);
int uart_getc(void);
void uart_intr(void);
