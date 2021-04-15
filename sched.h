#pragma once

//#include <stdint.h>
#include <proc.h>

process_t *get_cur_proc();
process_t *get_proc_by_id(int id);
uint32_t schedule(uint32_t esp);
void sched_add_proc(process_t *proc);
void sched_remove_proc(int id);
void sched_init();
int get_nproc();
void print_procs();
