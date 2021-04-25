#pragma once

#include <log.h>

#ifdef DEBUG

#define assert(x) \
do { \
	if (!(x)) { \
		klogf(LOG_CRIT, "Assertion failed: " #x "\n"); \
        halt(); \
	} \
} while (0) \

#else
#define assert(x) while(0)
#endif

inline void notImplemented() { assert(0); };
