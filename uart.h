#pragma once

#include <chardev.h>
#include <stdint.h>

void uart_init(void);
void uart_rx_ir(void);
uint8_t uart_tx_empty(void);
void uart_putc(char c);
int uart_getc(void);
void uart_handler(void);

extern chardev_t uartdev;

