#include <memmap.h>

uint8_t e820counter = 0;
e820memmap_t e820table[MEMMAP_E820_MAX_RECORDS];
