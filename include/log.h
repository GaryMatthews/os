#pragma once

#include <printf.h>

/* Use as as defined in RFC5424:

           Numerical         Severity
             Code

              0       Emergency: system is unusable
              1       Alert: action must be taken immediately
              2       Critical: critical conditions
              3       Error: error conditions
              4       Warning: warning conditions
              5       Notice: normal but significant condition
              6       Informational: informational messages
              7       Debug: debug-level messages
 */
#define LOG_EMERG	0
#define LOG_ALERT	1
#define LOG_CRIT	2
#define LOG_ERR 	3
#define LOG_WARNING	4
#define LOG_NOTICE	5
#define LOG_INFO	6
#define LOG_DEBUG	7

#define klogf(prio, str, ...) \
do { \
	printf("%s:%d %s\t", __FILE__, __LINE__, #prio + 4); \
	printf(str, ##__VA_ARGS__); \
} while (0)

#ifdef DEBUG

#define assert(x) \
do { \
	if (!(x)) { \
		klogf(LOG_CRIT, "Assertion failed: " #x "\n"); \
	} \
} while (0) \

#else
#define assert(x) while(0)
#endif

