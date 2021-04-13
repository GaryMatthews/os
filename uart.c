#include <stdint.h>
#include <string.h>

#include <uart.h>

#include <io.h>
//XXX#include <intr.h>

#include <printf.h>

static uint8_t uart_initialized = 0;
static const uint16_t UART_PORT = 0x3f8;
static const uint16_t UART_PORT_CONTROL = 0x3f8 + 5;

extern void uart_int();

void uart_init(void) {
    // Turn off the FIFO
  outportb(UART_PORT+2, 0);

  // 9600 baud, 8 data bits, 1 stop bit, parity off.
  outportb(UART_PORT + 3, 0x80);    // Unlock divisor
  outportb(UART_PORT + 0, 115200/9600);
  outportb(UART_PORT + 1, 0);
  outportb(UART_PORT + 3, 0x03);    // Lock divisor, 8 data bits.
  outportb(UART_PORT + 4, 0);
  outportb(UART_PORT + 1, 0x01);    // Enable receive interrupts.

  // If status is 0xFF, no serial port.
  if (inportb(UART_PORT + 5) == 0xFF)
    return;

  uart_initialized = 1;
}

void uart_rx_ir(void) {
  // Acknowledge pre-existing interrupt conditions;
  // enable interrupts.
  inportb(UART_PORT + 2);
  inportb(UART_PORT + 0);

  //idt_set_entry(32+4, &uart_int, 0x8, IDT_FLAG_INTERRUPT_GATE | IDT_FLAG_RING0 | IDT_FLAG_PRESENT);
  //XXXinstall_ir(32 + 4, 0x80 | 0x0E, 0x8, &uart_int);
}

int uart_getc(void) {
    while ((inportb(UART_PORT + 5) & 1) == 0); // rx empty?
    return inportb(UART_PORT + 0);
}

void uart_int(void) {
    char c = uart_getc();
    printf("%d %c\n", c, c);
    outportb(0xa0, 0x20); // EOI an Slave-PIC
    //outportb(0x20, 0x20); // EOI an Master-PIC
    //XXX consoleintr(uartgetc, 0);     // Minor device 0
}

uint8_t uart_tx_empty(void) {
  return inportb(UART_PORT_CONTROL) & 0x20;
}

void _putchar(char c) {
    if (!uart_initialized) uart_init();;
    while (!uart_tx_empty());
    outportb(UART_PORT, c);
}
