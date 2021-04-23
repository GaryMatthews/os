#include <io.h>
#include <printf.h>

void panic(const char *format, ...) {
    disable_int();

	printf("\n\nKernel panic:\n");
    
    va_list args;
    va_start(args, format);
    vprintf(format, args);
    
    printf("\nHalting the system.\n\n");
    halt();
}
