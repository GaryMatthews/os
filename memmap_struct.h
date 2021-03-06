#pragma once

#define E820MEMMAP_OFFSET_BASE_ADDRESS  0x00
#define E820MEMMAP_OFFSET_SIZE          0x08
#define E820MEMMAP_OFFSET_TYPE          0x10
#define E820MEMMAP_SIZE                 0x14

#define E820MEMMAP_SIZE_SIZE            0x08

#ifndef __ASSEMBLER__

#include <types.h>

typedef struct e820memmap {
	uint64_t base_address;
	uint64_t size;
	uint32_t type;
} __attribute__((packed)) e820memmap_t;

#endif
