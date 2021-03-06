#pragma once

#include <types.h>

#define BLOCKS_PER_BYTE 8
#define BLOCKS_LEN 4096
#define BYTE_SET 0xFFFFFFFF
#define BITMAP_LEN 0x8000

#define KERNEL_SPACE_END 0x401000

typedef uint32_t mm_addr_t;

typedef uint32_t vmm_addr_t;

typedef struct mem_info {
    uint32_t size;
    uint32_t used_blocks;
    uint32_t max_blocks;
    mm_addr_t *map;
} mem_info_t;

typedef struct memory_region {
    uint32_t size;
    uint32_t addr_low;
    uint32_t addr_high;
    uint32_t len_low;
    uint32_t len_high;
    uint32_t type;
} __attribute__((__packed__)) mem_region_t;

void pmm_init(uint32_t mem_size);
void pmm_init2();
void pmm_set_bit(int bit);
void pmm_unset_bit(int bit);
int pmm_first_free();
void pmm_init_reg(mm_addr_t addr, uint32_t size);
void pmm_deinit_reg(mm_addr_t addr, uint32_t size);
void *pmm_malloc();
void pmm_free(mm_addr_t *frame);

mm_addr_t *get_mem_map();
uint32_t get_mem_size();
uint32_t get_used_blocks();
uint32_t get_max_blocks();

void enable_paging();
void load_pdbr(mm_addr_t addr);
mm_addr_t get_pdbr();
void flush_tlb(vmm_addr_t addr);
int get_cr0();
int get_cr2();

