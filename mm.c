#include <lib/string.h>
#include <mm.h>

mem_info_t pmm;

// 32KB for the bitmap to reserve up to 4GB
static uint32_t bitmap[BITMAP_LEN] __attribute__((aligned(BLOCKS_LEN)));

/**
 * Initializes the physical memory manager
 * Every bit in the pmm.map indicates if a 4096 byte block is free or not
 * mem_size is in KB
 */
void pmm_init(uint32_t mem_size) {
    // Get the blocks number
    pmm.max_blocks = pmm.used_blocks = mem_size / 4;
    // Set the address for the memory map
    pmm.map = bitmap;
    // Set all the blocks as used
    memset(pmm.map, BYTE_SET, BITMAP_LEN);
}

void pmm_init2(void) {
    pmm_deinit_reg(0x0, KERNEL_SPACE_END);
    pmm.size = (pmm.max_blocks - pmm.used_blocks) * 4;
}

/**
 * Sets the block as used
 */
void pmm_set_bit(int bit) {
    pmm.map[bit / 32] |= (1 << (bit % 32));
}

/**
 * Sets the block as free
 */
void pmm_unset_bit(int bit) {
    pmm.map[bit / 32] &= ~(1 << (bit % 32));
}

/**
 * Gets the first free block
 */
int pmm_first_free() {
    uint32_t i;
    int j;
    
    for(i = 0; i < pmm.max_blocks / 32; i++) {
        if(pmm.map[i] != BYTE_SET) {
            for(j = 0; j < 32; j++) {
                if(!(pmm.map[i] & (1 << j))) {
                    return (i * 32) + j;
                }
            }
        }
    }
    return -1;
}

/**
 * Initializes a memory region to be used
 */
void pmm_init_reg(mm_addr_t addr, uint32_t size) {
    uint32_t i;
    uint32_t blocks = size / BLOCKS_LEN;
    uint32_t align = addr / BLOCKS_LEN;
    for(i = 0; i < blocks; i++) {
        pmm_unset_bit(align++);
        pmm.used_blocks--;
    }
    pmm_set_bit(0);
}

/**
 * Deinitializes a reserved memory region
 */
void pmm_deinit_reg(mm_addr_t addr, uint32_t size) {
    uint32_t i;
    uint32_t blocks = size / BLOCKS_LEN;
    uint32_t align;
    if(addr == 0) {
        align = 0;
    } else {
        align = addr / BLOCKS_LEN;
    }
    for(i = 0; i < blocks; i++) {
        pmm_set_bit(align++);
        pmm.used_blocks++;
    }
}

/**
 * Returns a usable block
 */
void *pmm_malloc() {
    int p = pmm_first_free();
    if(!p)
        return 0;
    pmm_set_bit(p);
    pmm.used_blocks++;
    return (void *) (BLOCKS_LEN * p);
}

/**
 * Frees a block
 */
void pmm_free(mm_addr_t *addr) {
    if((uint32_t) addr < KERNEL_SPACE_END)
        return;
    pmm_unset_bit((uint32_t) addr / BLOCKS_LEN);
    pmm.used_blocks--;
}

mm_addr_t *get_mem_map() {
    return pmm.map;
}

uint32_t get_mem_size() {
    return pmm.size;
}

uint32_t get_used_blocks() {
    return pmm.used_blocks;
}

uint32_t get_max_blocks() {
    return pmm.max_blocks;
}

/**
 * Enables paging
 */
void enable_paging() {
    uint32_t reg;
    // Enable paging
    asm volatile("mov %%cr0, %0" : "=r" (reg));
    reg |= 0x80000000;
    asm volatile("mov %0, %%cr0" : : "r" (reg));
}

/**
 * Loads the pdbr register with a physical address to a page directory
 */
void load_pdbr(mm_addr_t addr) {
    asm volatile("mov %0, %%cr3" : : "r" (addr));
}

/**
 * Gets the pdbr value
 */
mm_addr_t get_pdbr() {
    mm_addr_t ret;
    asm volatile("mov %%cr3, %0" : "=r" (ret));
    return ret;
}

/**
 * Flushes the TLB cache
 */
void flush_tlb(vmm_addr_t addr) {
    asm volatile("cli; invlpg (%0); sti" : : "r" (addr));
}

/**
 * Gets the value of the cr0 register
 */
int get_cr0() {
    int ret;
    asm volatile("mov %%cr0, %0" : "=r" (ret));
    return ret;
}

/**
 * Gets the value of the cr2 register
 */
int get_cr2() {
    int ret;
    asm volatile("mov %%cr2, %0" : "=r" (ret));
    return ret;
}
