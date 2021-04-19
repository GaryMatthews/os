#pragma once

#include <types.h>

void qsort(void*, size_t, size_t, int (*)(const void*, const void*));
void qsort_r(void*, size_t, size_t, int (*)(const void*, const void*, void*), void*);

