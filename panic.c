/*
 * This function is used through-out the kernel (includeinh mm and fs)
 * to indicate a major problem.
 */
#include <io.h>
#include <printf.h>

void panic() {
	//printf("Kernel panic: %s\n\r",s);
	exit(1);
}
