#include <heap.h>
#include <mm.h>
#include <paging.h>
#include <sched.h>
#include <proc.h>
#include <printf.h>

/**
 * Init the user process heap memory
 */
void heap_init(vmm_addr_t *addr) {
    heap_info_t *heap_info = (heap_info_t *) addr;
    heap_info->start = addr + sizeof(heap_info_t);
    heap_info->size = (PAGE_SIZE * 4) - sizeof(heap_info_t);
    heap_info->used = sizeof(heap_header_t);
    heap_info->first_header = (heap_header_t *) heap_info->start;
    heap_info->first_header->magic = HEAP_MAGIC;
    heap_info->first_header->size = heap_info->size - heap_info->used;
    heap_info->first_header->is_free = 1;
    heap_info->first_header->next = 0;
}

void *umalloc(size_t len, vmm_addr_t *heap) {
    heap_info_t *heap_info = (heap_info_t *) heap;
    heap_header_t *head = (heap_header_t *) heap_info->first_header;
    
    if(heap_info->used >= heap_info->size) {
        printf("\nOut of memory\n");
        return 0;
    }
    
    while((head != 0) && ((vmm_addr_t *) head < heap + (PAGE_SIZE * 4))) {
        if((head->size >= len) && (head->is_free == 1) && (head->magic == HEAP_MAGIC)) {
            head->is_free = 0;
            heap_header_t *head2 = (heap_header_t *) head + len + sizeof(heap_header_t);
            head2->size = head->size - len - sizeof(heap_header_t);
            head2->magic = HEAP_MAGIC;
            head2->is_free = 1;
            head2->next = 0;
            head->next = head2;
            head->size = len;
            heap_info->used += len + sizeof(heap_header_t);
            return (void *) head + sizeof(heap_header_t);
        }
        head = head->next;
    }
    printf("\nOut of memory\n");
    return 0;
}

void ufree(void *ptr, vmm_addr_t *heap) {
    heap_info_t *heap_info = (heap_info_t *) heap;
    heap_header_t *head = ptr - sizeof(heap_header_t);
    if((head->is_free == 0) && (head->magic == HEAP_MAGIC)) {
        head->is_free = 1;
        heap_info->used -= head->size;
        
        // Merge contiguous free sections
        heap_header_t *app = head->next;
        while((app != 0) && (app->is_free == 1)) {
            head->size += app->size + sizeof(heap_header_t);
            head->next = app->next;
            
            app = app->next;
        }
    }
}

void *umalloc_sys(size_t len) {
    process_t *cur = get_cur_proc();
    if(cur && cur->thread_list) {
        return umalloc(len, (vmm_addr_t *) cur->thread_list->heap);
    }
    return 0;
}

void ufree_sys(void *ptr) {
    process_t *cur = get_cur_proc();
    if(cur && cur->thread_list) {
        ufree(ptr, (vmm_addr_t *) cur->thread_list->heap);
    }
}
