#include <pcspk.h>

#include <io.h>

void beep(int value) {
  unsigned int count = 0;

#define PIT_TICK_RATE 1193182ul
  if (value > 20 && value < 32767)
    count = PIT_TICK_RATE / value;

  outportb(0x43, 0xB6);
  /* select desired HZ */
  outportb(0x42, count & 0xff);
  outportb(0x42, (count >> 8) & 0xff);
  /* enable counter 2 */
  outportb(0x61, inportb(0x61) | 3);
}

void beep_off(void) {
    outportb(0x61, inportb(0x61) & 0xFC);
}
