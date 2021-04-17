#include <stdint.h>

#include <uart.h>

#include <io.h>
#include <idt.h>
#include <string.h>
#include <log.h>

static uint8_t uart_initialized = 0;
static const uint16_t UART_PORT = 0x3f8;
static const uint16_t UART_PORT_CONTROL = 0x3f8 + 5;

static ssize_t uart_read(struct chardev_struct *dev, char *buf, size_t nbyte);
static ssize_t uart_write(chardev_t *dev, const char *buf, size_t nbyte);

#define RECVBUF_LEN 64
static char recvbuf[RECVBUF_LEN];
static int rbpos = 0;

chardev_t uartdev = { uart_read, uart_write };

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

  install_ir(32 + 4, 0x80 | 0x0E, 0x8, &uart_int);

  outportb(UART_PORT + 1, 0x01);    // Enable receive interrupts.
}

int uart_getc(void) {
    while ((inportb(UART_PORT + 5) & 1) == 0)
        __builtin_ia32_pause(); // rx empty?
    return inportb(UART_PORT + 0);
}

void uart_handler(void) {
    char c = uart_getc();
    //printf("%d %c\n", c, c);
    //XXX consoleintr(uartgetc, 0);     // Minor device 0

    if (rbpos < RECVBUF_LEN)
        recvbuf[rbpos++] = c;
    else
        klogf(LOG_EMERG, "uart overrun\n");
    
    if (c == 27) {
        exit(0);
    }
}

uint8_t uart_tx_empty(void) {
  return inportb(UART_PORT_CONTROL) & 0x20;
}

void uart_putc(char c) {
    if (!uart_initialized) uart_init();
    while (!uart_tx_empty())
        __builtin_ia32_pause();
    outportb(UART_PORT, c);
}

static ssize_t uart_read(struct chardev_struct *dev, char *buf, size_t nbyte) {
    size_t read = 0, rs;
    
    while (1) {
        if (rbpos > 0) {
            if (rbpos <= (nbyte - read)) {
                rs = rbpos;
            } else {
                rs = nbyte - read;
            }
            memcpy(buf, recvbuf, rs);
            read += rbpos;
            buf += rbpos;
            if (rbpos - rs > 0)
                memmove(recvbuf, recvbuf + rs, rbpos - rs);
            rbpos -= rs;
        }
        
        if (read >= nbyte)
            break;
        
        /* Wait for an interrupt */
        asm volatile("hlt":::"memory");
    }

    return read;
}

static ssize_t uart_write(chardev_t *dev, const char *buf, size_t nbyte) {
    size_t i;
    
    for (i=0; i < nbyte; i++) {
        uart_putc(buf[i]);
    }
    
    return i;
}
