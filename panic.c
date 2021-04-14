/*
 * This function is used through-out the kernel (includeinh mm and fs)
 * to indicate a major problem.
 */
#include <printf.h>

void panic() {
	//printf("Kernel panic: %s\n\r",s);
	for(;;);
}
