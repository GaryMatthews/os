/*
 * This function is used through-out the kernel (included in mm and fs)
 * to indicate a major problem.
 */
#include <io.h>
#include <printf.h>

void panic() {
	printf("Kernel panic. Halting the system.\n\r");
    halt();
	//exit(1);
}
