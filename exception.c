/*
 *  Copyright 2016 Davide Pianca
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */

//#include <console.h>
//#include <hal/hal.h>
#include <io.h>
#include <panic.h>
#include <printf.h>
#include <proc.h>
#include <mm.h>
#include <paging.h>
//#include <proc/thread.h>

void (*return_error)() = (void *) RETURN_ADDR;

void return_exception() {
    int error = 1;
    asm volatile("mov %0, %%eax" : : "r" (error));
    (*return_error)();
}

void default_ir_handler() {
    disable_int();
    printf("Unhandled exception\n");
    panic();
}

void ex_divide_by_zero() {
    printf("Division by zero\n");
    panic();
}

void ex_single_step() {
    printf("Single step\n");
    panic();
}

void ex_nmi() {
    printf("NMI trap\n");
    panic();
}

void ex_breakpoint() {
    printf("Breakpoint\n");
    panic();
}

void ex_overflow() {
    printf("Overflow\n");
    panic();
}

void ex_bounds_check() {
    printf("Bounds check\n");
    panic();
}

void ex_invalid_opcode(struct regs *re) {
    printf("Invalid opcode\n");
    printf("eip: %x cs: %x\neax: %d ebx: %d ecx: %d edx: %d\nesp: %x ebp: %x esi: %d edi: %d\nds: %x es: %x fs: %x gs: %x\n", re->eip, re->cs, re->eax, re->ebx, re->ecx, re->edx, re->esp, re->ebp, re->esi, re->edi, re->ds, re->es, re->fs, re->gs);
    if(re->es == 0x10) {
        // If an Invalid Opcode occurs in kernel mode, we don't really want to continue
        panic();
    } else {
        // If we were in user mode, just kill that thread or process
        return_exception();
    }
}

void ex_device_not_available() {
    printf("Device not available\n");
    panic();
}

void ex_double_fault() {
    printf("Double fault\n");
    panic();
}

void ex_invalid_tss() {
    printf("Invalid TSS\n");
    panic();
}

void ex_segment_not_present() {
    printf("Segment not present\n");
    panic();
}

void ex_stack_fault() {
    printf("Stack fault\n");
    panic();
}

void ex_gpf(struct regs_error *re) {
    printf("\nGeneral protection fault\nError code: %b\n", re->error);
    printf("eip: %x cs: %x\neax: %d ebx: %d ecx: %d edx: %d\nesp: %x ebp: %x esi: %d edi: %d\nds: %x es: %x fs: %x gs: %x\n", re->eip, re->cs, re->eax, re->ebx, re->ecx, re->edx, re->esp, re->ebp, re->esi, re->edi, re->ds, re->es, re->fs, re->gs);
    printf("cr2: %x cr3: %x\n", get_cr2(), get_pdbr());
    
    // If a GPF occurs in kernel mode, we don't really want to continue
    if(re->es == 0x10) {
        panic();
    } else {
        // If we were in user mode, just kill that thread or process
        return_exception();
    }
}

void ex_page_fault(struct regs_error *re) {
    int virt_addr = get_cr2();
    mm_addr_t phys_addr = (mm_addr_t) get_phys_addr(get_page_directory(), virt_addr);
    
    printf("\nPage fault at addr: 0x%x\n", (unsigned)virt_addr);
    printf("Phys addr: 0x%x\n", phys_addr);
    // If a Page Fault occurs in kernel mode, we don't really want to continue
    if(re->es == 0x10) {
        panic();
    } else {
        // If we were in user mode, just kill that thread or process
        return_exception();
    }
}

void ex_fpu_error() {
    printf("FPU error\n");
    panic();
}

void ex_alignment_check() {
    printf("Alignment check\n");
    panic();
}

void ex_machine_check() {
    printf("Machine check\n");
    panic();
}

void ex_simd_fpu() {
    printf("SIMD FPU error\n");
    panic();
}

